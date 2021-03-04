#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QThread>
#include "imagecreate.h"
#include <QPushButton>
#include "mythread.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void paintEvent(QPaintEvent*);

    void getImage(QImage);
    void dealClose();

    void test();
private:
    QImage image_;
    QThread* thread_;
    ImageCreate* image_create_;
    QPushButton* draw_btn_;
    QPushButton* draw_btn_2;
    MyThread* thread2_;
};

#endif // WIDGET_H
