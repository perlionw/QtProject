#ifndef CALINTERFACE_H
#define CALINTERFACE_H
#include <QString>
//定义接口
class CalInterface
{
public:
    virtual ~CalInterface(){}

    virtual int add(int a, int b) = 0;

};

#define CalInterface_iid "Examples.Plugin.CalInterface"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(CalInterface, CalInterface_iid)
QT_END_NAMESPACE


#endif // CALINTERFACE_H
