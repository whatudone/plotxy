/********************************************************************************
** Form generated from reading UI file 'PlotItemBase.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PLOTITEMBASE_H
#define UI_PLOTITEMBASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_PlotItemBase
{
public:

    void setupUi(QWidget *PlotItemBase)
    {
        if (PlotItemBase->objectName().isEmpty())
            PlotItemBase->setObjectName(QStringLiteral("PlotItemBase"));
        PlotItemBase->resize(800, 600);

        retranslateUi(PlotItemBase);

        QMetaObject::connectSlotsByName(PlotItemBase);
    } // setupUi

    void retranslateUi(QWidget *PlotItemBase)
    {
        PlotItemBase->setWindowTitle(QApplication::translate("PlotItemBase", "MainWindow", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class PlotItemBase: public Ui_PlotItemBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PLOTITEMBASE_H
