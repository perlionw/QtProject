#ifndef MYTHREAD_H
#define MYTHREAD_H
#include <QThread>
#include <QImage>
class MyThread : public QThread
{
    Q_OBJECT
public:
    MyThread(QObject* parent = 0);

    ~MyThread();
    void stop();
signals:
    void updateImage(QImage);

protected:
    void run();

private:
    bool stop_flag = false;
};

#endif // MYTHREAD_H
