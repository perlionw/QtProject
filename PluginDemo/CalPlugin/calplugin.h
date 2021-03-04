#ifndef CALPLUGIN_H
#define CALPLUGIN_H

#include <QObject>
#include "calinterface.h"
class CalPlugin : public QObject, public CalInterface
{
    Q_OBJECT

    Q_INTERFACES(CalInterface)
    Q_PLUGIN_METADATA(IID CalInterface_iid FILE "calplugin.json")
public:
    explicit CalPlugin(QObject *parent = 0);
    int add(int a,int b);
signals:

public slots:
};

#endif // CALPLUGIN_H
