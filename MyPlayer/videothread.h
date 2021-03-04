#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H
#include <QThread>
#include <QImage>
class VideoThread : public QThread
{
    Q_OBJECT
public:
    explicit VideoThread(QObject *parent = 0);

    ~VideoThread();
signals:
    void sig_GetOneFrame(QImage);

protected:
    void run();


};

#endif // VIDEOTHREAD_H
