#include "calplugin.h"

CalPlugin::CalPlugin(QObject *parent) : QObject(parent)
{

}

int CalPlugin::add(int a, int b)
{
    return a+b;
}
