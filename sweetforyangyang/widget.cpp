#include "widget.h"
#include "ui_widget.h"
#include <QDialog>
#include <QMessageBox>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QRect>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    dislikeFlag = false;
    setMouseTracking(true);
    ui->pushButton_2->setMouseTracking(true);
    setWindowTitle("love yangyang");

}

Widget::~Widget()
{
    delete ui;
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    if(count <=5)
    {
        if( ui->pushButton_2->frameGeometry().contains(event->pos()))
        {
            if(!dislikeFlag)
            {
                count++;
                dislikeFlag = true;
                ui->pushButton_2->move( ui->pushButton_2->pos().x(),ui->pushButton_2->pos().y() - 50);
            }
            else
            {
                count++;
                dislikeFlag = false;
                ui->pushButton_2->move( ui->pushButton_2->pos().x(),ui->pushButton_2->pos().y() + 50);
            }
        }
    }
}

void Widget::closeEvent(QCloseEvent *event)
{
    QMessageBox::information(this, "info", "Closing the window won't change the fact that you like me!!!",
                             QMessageBox::Yes, QMessageBox::Yes);
     event->ignore(); // 忽略退出信号，程序继续进行
}

void Widget::on_pushButton_toggled(bool checked)
{


}

void Widget::on_pushButton_clicked()
{
    if(ui->pushButton->text() == "yes")
    {
        QMessageBox::information(this, "info", "I knew you liked me!!!, love you!!!",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        ui->pushButton->setText("yes");
        ui->pushButton_2->setText("no");
    }
}

void Widget::on_pushButton_2_clicked()
{
    if(ui->pushButton_2->text() == "yes")
    {
        QMessageBox::information(this, "info", "I knew you liked me!!!, love you!!!",
                                 QMessageBox::Yes, QMessageBox::Yes);
    }
    else
    {
        ui->pushButton_2->setText("yes");
        ui->pushButton->setText("no");
    }
}
