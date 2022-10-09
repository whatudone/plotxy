#include <QtWidgets/QApplication>
#include <QDebug>
#include "PlotXYDemo.h"

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    PlotXYDemo w;
    //w.init();
    w.show();

    return a.exec();
}