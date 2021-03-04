#include "widget.h"
#include <QApplication>
#include <QList>
#include <QByteArray>
#include <QDebug>
#include <QTextCodec>
int main(int argc, char *argv[])
{


    QTextCodec *tc = QTextCodec::codecForName("UTF8");
    QApplication a(argc, argv);
    Widget w;
    w.show();
    QString str("821\0cas");
    QByteArray byteArray;
    byteArray += '1';
    byteArray += '2';
    byteArray += '\0';
    byteArray += '3';
    byteArray += '4';
    //QString str = tc->toUnicode(byteArray);
    int size = str.length();
    str.toUtf8();
    qDebug() << byteArray;
    qDebug() << str;
    return a.exec();
}
