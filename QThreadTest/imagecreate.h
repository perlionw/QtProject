#ifndef IMAGECREATE_H
#define IMAGECREATE_H

#include <QObject>
#include <QImage>
class ImageCreate : public QObject
{
    Q_OBJECT
public:
    explicit ImageCreate(QObject *parent = 0);


    void drawImage();

signals:
    void update_image(QImage image);
public slots:
};

#endif // IMAGECREATE_H
