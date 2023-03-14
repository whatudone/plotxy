#include <QtWidgets/QApplication>
#include <QFile>

#include "PlotXYDemo.h"
void loadQss(const QString &fileName){

    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)) {
        qApp->setStyleSheet(file.readAll());
    }
    file.close();
}
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    loadQss(":/qss/app.qss");
    PlotXYDemo w;
    w.showMaximized();

    return a.exec();
}
