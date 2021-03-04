#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QtPlugin>
#include "calinterface.h"
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();

    void onCalClicked();
    bool LoadPlugin();
private:

    QLineEdit* param1_ledit;
    QLineEdit* param2_ledit;
    QPushButton* cal_btn;
    QLineEdit* value_ledit;

    CalInterface* m_cal_;
};

#endif // WIDGET_H
