#include "imagecreate.h"
#include <QPainter>
#include <QPoint>
#include <QBrush>
ImageCreate::ImageCreate(QObject *parent) : QObject(parent)
{

}

void ImageCreate::drawImage()
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
    emit update_image(image);
}
