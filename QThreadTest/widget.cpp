#include "widget.h"
#include <QPainter>
#include <QHBoxLayout>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    QHBoxLayout* layout = new QHBoxLayout(this);


    draw_btn_ = new QPushButton("draw");
    draw_btn_2 = new QPushButton("stop");

    layout->addWidget(draw_btn_);
    layout->addWidget(draw_btn_2);

    thread_ = new QThread(this);
    thread2_ = new MyThread(this);
    image_create_ = new ImageCreate;
    image_create_->moveToThread(thread_);
    thread_->start();
    thread2_->start();
    connect(image_create_, &ImageCreate::update_image, this, &Widget::getImage);
    connect(draw_btn_, &QPushButton::pressed, image_create_, &ImageCreate::drawImage);
    connect(draw_btn_2, &QPushButton::pressed, thread2_, &MyThread::stop);
    connect(this, &Widget::destroyed, this, &Widget::dealClose);
    connect(thread2_, &MyThread::updateImage, this, &Widget::getImage);
    connect(thread2_, &MyThread::finished, thread2_, &QObject::deleteLater);
}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(50, 50, image_);
}

void Widget::getImage(QImage temp)
{
    this->image_ = temp;
    update();
}

void Widget::dealClose()
{
    thread_->quit();
    thread_->wait();
    delete thread_;

    thread2_->stop();

}
