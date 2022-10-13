#include <QtWidgets/QApplication>
#include <QDebug>
#include "PlotXYDemo.h"
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    PlotXYDemo w;
    //w.init();
    w.show();

    return a.exec();
}