#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include "videothread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


    void paintEvent(QPaintEvent* event);

public slots:
    void slotGetOneFrame(QImage);
private:
    Ui::Widget *ui;
    VideoThread* mPlayer;
    QImage mImage;
};

#endif // WIDGET_H
