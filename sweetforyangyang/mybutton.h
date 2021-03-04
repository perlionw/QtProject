#ifndef MYBUTTON_H
#define MYBUTTON_H

#include <QObject>
#include <QPushButton>
class MyButton : public QPushButton
{
    Q_OBJECT
public:
    MyButton();

public:
    vocid mouseMoveEvent(QMouseEvent* event);
};

#endif // MYBUTTON_H
