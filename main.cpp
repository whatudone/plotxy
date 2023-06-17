#include "PlotXYDemo.h"
#include "constdef.h"
#include <QFile>
#include <QtWidgets/QApplication>
void loadQss(const QString& fileName)
{

    QFile file(fileName);
    if(file.open(QIODevice::ReadOnly))
    {
        qApp->setStyleSheet(file.readAll());
    }
    file.close();
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    qRegisterMetaType<Z_SendHeader>("Z_SendHeader");
    qRegisterMetaType<MARS_PlatInfoDataExcect>("MARS_PlatInfoDataExcect");
    qRegisterMetaType<GenericData>("GenericData");
    loadQss(":/qss/app.qss");
    PlotXYDemo w;
    w.showMaximized();

    return a.exec();
}
