#include "widget.h"
#include <QHBoxLayout>
#include <QDir>
#include <QApplication>
#include <QString>
#include <QPluginLoader>
#include "calinterface.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    LoadPlugin();
    param1_ledit = new QLineEdit;
    QLabel* add_label = new QLabel("+");
    param2_ledit = new QLineEdit;
    QLabel* equal_label = new QLabel("=");
    value_ledit = new QLineEdit;
    cal_btn = new QPushButton("cal");
    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(param1_ledit);
    layout->addWidget(add_label);
    layout->addWidget(param2_ledit);
    layout->addWidget(equal_label);
    layout->addWidget(value_ledit);
    layout->addWidget(cal_btn);

    connect(cal_btn, &QPushButton::clicked, this, &Widget::onCalClicked);

}

Widget::~Widget()
{

}

void Widget::onCalClicked()
{
    int a = param1_ledit->text().toInt();
    int b = param2_ledit->text().toInt();
    int value = m_cal_->add(a, b);
    value_ledit->setText(QString::number(value));
}

bool Widget::LoadPlugin()
{
    QString app_dir = qApp->applicationDirPath();
    QDir pluginsDir(app_dir);
 #if defined(Q_OS_WIN)
     if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release"){
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
 #elif defined(Q_OS_MAC)
     if (pluginsDir.dirName() == "MacOS") {
         pluginsDir.cdUp();
         pluginsDir.cdUp();
         pluginsDir.cdUp();
     }
 #endif
     pluginsDir.cd("plugins");
     foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
         QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
         QObject *plugin = pluginLoader.instance();
        // qDebug() << __FUNCTION__ << pluginLoader.errorString();
         if (plugin) {
               QString pluginName = plugin->metaObject()->className();
              if(pluginName == "CalPlugin")
              {
                 m_cal_ = qobject_cast<CalInterface *>(plugin);
                  if (m_cal_)
                      return true;
              }

         }
     }

     return false;
}
