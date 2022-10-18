/********************************************************************************
** Form generated from reading UI file 'Extrapolation.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXTRAPOLATION_H
#define UI_EXTRAPOLATION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Extrapolation
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_11;
    QCheckBox *checkBox_5;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_2;
    QLineEdit *lineEdit_3;
    QSpacerItem *horizontalSpacer_20;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *Extrapolation)
    {
        if (Extrapolation->objectName().isEmpty())
            Extrapolation->setObjectName(QStringLiteral("Extrapolation"));
        Extrapolation->resize(411, 340);
        verticalLayout = new QVBoxLayout(Extrapolation);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        checkBox_5 = new QCheckBox(Extrapolation);
        checkBox_5->setObjectName(QStringLiteral("checkBox_5"));
        checkBox_5->setLayoutDirection(Qt::RightToLeft);

        horizontalLayout_11->addWidget(checkBox_5);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_11->addItem(horizontalSpacer_8);


        verticalLayout->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_2 = new QLabel(Extrapolation);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setFrameShape(QFrame::Box);

        horizontalLayout_12->addWidget(label_2);

        lineEdit_3 = new QLineEdit(Extrapolation);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_12->addWidget(lineEdit_3);

        horizontalSpacer_20 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_20);


        verticalLayout->addLayout(horizontalLayout_12);

        verticalSpacer = new QSpacerItem(20, 256, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(Extrapolation);

        QMetaObject::connectSlotsByName(Extrapolation);
    } // setupUi

    void retranslateUi(QWidget *Extrapolation)
    {
        Extrapolation->setWindowTitle(QString());
        checkBox_5->setText(QApplication::translate("Extrapolation", "\346\216\250\347\256\227(\344\273\205\346\264\273\350\267\203\346\225\260\346\215\256)", Q_NULLPTR));
        label_2->setText(QApplication::translate("Extrapolation", "\346\216\250\347\256\227\346\227\266\351\227\264\345\201\217\347\247\273", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class Extrapolation: public Ui_Extrapolation {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXTRAPOLATION_H
