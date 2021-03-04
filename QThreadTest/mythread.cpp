#include "mythread.h"
#include <QPainter>
#include <QPoint>
#include <thread>
MyThread::MyThread(QObject* parent) : QThread(parent)
{


}

MyThread::~MyThread()
{
    this->quit();
    this->wait();
}

void MyThread::stop()
{
    stop_flag = true;
}


void MyThread::run()
{
    while(!stop_flag)
    {
        QImage image(500, 500, QImage::Format_ARGB32);
        QPainter p(&image);

        QPen pen;
        pen.setWidth(1);
        p.setPen(pen);

        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        brush.setColor(Qt::blue);
        pen.setBrush(brush);

        QPoint a[] =
        {
            QPoint(qrand()%500, qrand()%500),
            QPoint(qrand()%500, qrand()%500),
             QPoint(qrand()%500, qrand()%500),
               QPoint(qrand()%500, qrand()%500),
               QPoint(qrand()%500, qrand()%500)
        };

        p.drawPolygon(a, 5);
        emit updateImage(image);
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

}
