#include <QCoreApplication>
#include <QStyledItemDelegate>
extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/pixfmt.h"
#include "libswscale/swscale.h"
#include "libswresample/swresample.h"
}

#include "SDL.h"
#define MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

//swr
struct SwrContext* au_convert_ctx; // 重采样上下文
int out_buffer_size; // 重采样后的缓冲区
uint8_t* out_buffer; // sdl调用音频数据的缓冲区

//audio decode
int au_stream_index = -1; // 音频流在文件中的位置
AVFormatContext* pFormatCtx = nullptr; // 文件上下文
AVCodecParameters* audioCodecParameter; // 音频解码器参数
AVCodecContext* audioCodecCtx = nullptr; // 音频解码器上下文
AVCodec* audioCodec = nullptr; // 音频解码器

// sdl
static Uint32 audio_len; // 音频数据缓冲区中未读数据剩余的长度
static Uint8* audio_pos; // 音频缓冲区中读取的位置
SDL_AudioSpec wanted_spec; // sdl播放音频的参数


// sdl配置中的系统播放音频的回调。
// udata：我们自己设置的参数，
// stream：系统读取音频数据的buffer由我们在这个函数中把音频数据拷贝到这个buffer中，
// len：系统希望读取的长度（可以比这个小，但不能给多）
void sdl_audio_callback(void* udata, Uint8* stream, int len)
{
    //SDL 2.0之后的函数。很像memset在这里用来清空指定内存
    SDL_memset(stream, 0, len);
    if (audio_len == 0)
        return;
    len = ((Uint32)len > audio_len ? audio_len : len); //比较剩余未读取的音频数据的长度和所需要的长度。尽最大可能的给予其音频数据

    SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME); //SDL_MixAudio的作用和memcpy类似，这里将audio_pos的数据传递给stream

    //audio_pos是记录out_buffer（存放我们读取音频数据的缓冲区）当前读取的位置
    //audio_len是记录out_buffer剩余未读数据的长度
    audio_pos += len; //audio_pos前进到新的位置
    audio_len -= len; //audio_len的长度做相应的减少
}


// 初始化编码器，重采样器所需的各项参数
int init_audio_parameters() {
    // 获取音频解码器参数
    audioCodecParameter = pFormatCtx->streams[au_stream_index]->codecpar;
    // 获取音频解码器
    audioCodec = avcodec_find_decoder(audioCodecParameter->codec_id);
    if (audioCodec == nullptr) {
        printf_s("audio avcodec_find_decoder failed.\n");
        return -1;
    }
    // 获取解码器上下文
    audioCodecCtx = avcodec_alloc_context3(audioCodec);
    if (avcodec_parameters_to_context(audioCodecCtx, audioCodecParameter) < 0) {
        printf_s("audio avcodec_parameters_to_context failed\n");
        return -1;
    }
    // 根据上下文配置音频解码器
    avcodec_open2(audioCodecCtx, audioCodec, nullptr);
    // -------------------设置重采样相关参数-------------------------//
    uint64_t out_channel_layout = AV_CH_LAYOUT_STEREO; // 双声道输出
    int out_channels = av_get_channel_layout_nb_channels(out_channel_layout);

    AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16; // 输出的音频格式
    int out_sample_rate = 44100; // 采样率
    int64_t in_channel_layout = av_get_default_channel_layout(audioCodecCtx->channels); //输入通道数
    audioCodecCtx->channel_layout = in_channel_layout;
    au_convert_ctx = swr_alloc(); // 初始化重采样结构体
    au_convert_ctx = swr_alloc_set_opts(au_convert_ctx, out_channel_layout, out_sample_fmt, out_sample_rate,
                                        in_channel_layout, audioCodecCtx->sample_fmt, audioCodecCtx->sample_rate, 0, nullptr); //配置重采样率
    swr_init(au_convert_ctx); // 初始化重采样率

    int out_nb_samples = audioCodecCtx->frame_size;
    // 计算出重采样后需要的buffer大小，后期储存转换后的音频数据时用
    out_buffer_size = av_samples_get_buffer_size(NULL, out_channels, out_nb_samples, out_sample_fmt, 1);
    out_buffer = (uint8_t*)av_malloc(MAX_AUDIO_FRAME_SIZE * 2);
    // -------------------设置 SDL播放音频时的参数 ---------------------------//
    wanted_spec.freq = out_sample_rate;//44100;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.channels = out_channels;
    wanted_spec.silence = 0;
    wanted_spec.samples = out_nb_samples;
    wanted_spec.callback = sdl_audio_callback; //sdl系统会掉。上面有说明
    wanted_spec.userdata = nullptr; // 回调时想带进去的参数

    // SDL打开音频播放设备
    if (SDL_OpenAudio(&wanted_spec, NULL) < 0) {
        printf_s("can't open audio.\n");
        return -1;
    }
    // 暂停/播放音频，参数为0播放音频，非0暂停音频
    SDL_PauseAudio(0);
    return 0;
}

// 将读取到的一个音频pkt解码成avframe。avframe中的数据就是原始的音频数据
void decode_audio_packet(AVCodecContext * code_context, AVPacket * pkt, AVFrame * frame)
{
    int i, ch;
    int ret, data_size;
    // ffmpeg3.2版本后推荐使用的方式，将一个pkt发送给解码器。之后在avcodec_receive_frame中取出解码后的avframe
    ret = avcodec_send_packet(code_context, pkt);
    if (ret < 0)
    {
        printf_s("Error submitting the packet to the decoder\n");
        system("pause");
        exit(1);
    }
    // 不断尝试取出音频数据，直到无法再取出
    while (ret >= 0)
    {
        ret = avcodec_receive_frame(code_context, frame); // 前文已经介绍，在此处取出原始音频数据

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) //该帧目前无法解出，需要再发送一个pkt
            return;
        else if (ret < 0)
        {
            printf_s("Error during decoding\n");
            system("pause");
            exit(1);
        }
        // 将音频的采样率转换成本机能播出的采样率
        swr_convert(au_convert_ctx, &out_buffer, out_buffer_size,
                    (const uint8_t * *)frame->data, code_context->frame_size);

        while (audio_len > 0) // 在此处等待sdl_audio_callback将之前传递的音频数据播放完再向其中发送新的数据
            SDL_Delay(1);

        // 将读取到的数据存入音频缓冲区
        audio_len = out_buffer_size; // 记录音频数据的长度
        audio_pos = (Uint8*)out_buffer;
    }
}

#undef main
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QStyledItemDelegate* fsa;
    char *file_path = "E:/1.mp4";
    //初始化ffmpeg的组件
    av_register_all();

    //读取文件头的文件基本信息到pFormateCtx中
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, file_path, nullptr, nullptr) != 0) {
        printf_s("avformat_open_input failed\n");
        system("pause");
        return -1;
    }
    // 在文件中找到文件中的音频流或视频流等“流”信息
    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {
        //异常处理...
    }
    // 找到音频流的位置
    for (unsigned i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (AVMEDIA_TYPE_AUDIO == pFormatCtx->streams[i]->codecpar->codec_type) {
            au_stream_index = i;
            continue;
        }
    }
    if (-1 == au_stream_index) {
        //异常处理...
    }

    init_audio_parameters();

    AVPacket packet;
    AVFrame* pFrame = NULL;
    // 开始读取文件中编码后的音频数据，并将读到的数据储存在
    while (av_read_frame(pFormatCtx, &packet) >= 0)
    {
        if (packet.stream_index == au_stream_index)
        {
            if (!pFrame)
            {
                if (!(pFrame = av_frame_alloc()))
                {
                    printf_s("Could not allocate audio frame\n");
                    system("pause");
                    exit(1);
                }
            }
            if (packet.size) {
                // 对读取到的pkt解码，并将数据传递给音频数据缓冲区
                decode_audio_packet(audioCodecCtx, &packet, pFrame);
            }

            av_frame_unref(pFrame);
            av_packet_unref(&packet);
        }
    }


    return a.exec();
}
