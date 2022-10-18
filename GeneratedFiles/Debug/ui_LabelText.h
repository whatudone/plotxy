/********************************************************************************
** Form generated from reading UI file 'LabelText.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LABELTEXT_H
#define UI_LABELTEXT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LabelText
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_21;
    QLabel *label_13;
    QComboBox *comboBox_3;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_20;
    QLabel *label_14;
    QPushButton *pushButton_9;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_19;
    QLabel *label_15;
    QLineEdit *lineEdit_4;
    QSpacerItem *horizontalSpacer_17;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *LabelText)
    {
        if (LabelText->objectName().isEmpty())
            LabelText->setObjectName(QStringLiteral("LabelText"));
        LabelText->resize(395, 286);
        verticalLayout = new QVBoxLayout(LabelText);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        label_13 = new QLabel(LabelText);
        label_13->setObjectName(QStringLiteral("label_13"));
        label_13->setFrameShape(QFrame::Box);

        horizontalLayout_21->addWidget(label_13);

        comboBox_3 = new QComboBox(LabelText);
        comboBox_3->setObjectName(QStringLiteral("comboBox_3"));

        horizontalLayout_21->addWidget(comboBox_3);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_21->addItem(horizontalSpacer_13);


        verticalLayout->addLayout(horizontalLayout_21);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        label_14 = new QLabel(LabelText);
        label_14->setObjectName(QStringLiteral("label_14"));
        label_14->setFrameShape(QFrame::Box);

        horizontalLayout_20->addWidget(label_14);

        pushButton_9 = new QPushButton(LabelText);
        pushButton_9->setObjectName(QStringLiteral("pushButton_9"));

        horizontalLayout_20->addWidget(pushButton_9);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_20->addItem(horizontalSpacer_14);


        verticalLayout->addLayout(horizontalLayout_20);

        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        label_15 = new QLabel(LabelText);
        label_15->setObjectName(QStringLiteral("label_15"));
        label_15->setFrameShape(QFrame::Box);

        horizontalLayout_19->addWidget(label_15);

        lineEdit_4 = new QLineEdit(LabelText);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        horizontalLayout_19->addWidget(lineEdit_4);

        horizontalSpacer_17 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_19->addItem(horizontalSpacer_17);


        verticalLayout->addLayout(horizontalLayout_19);

        verticalSpacer = new QSpacerItem(20, 164, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(LabelText);

        QMetaObject::connectSlotsByName(LabelText);
    } // setupUi

    void retranslateUi(QWidget *LabelText)
    {
        LabelText->setWindowTitle(QString());
        label_13->setText(QApplication::translate("LabelText", "\346\226\207\346\234\254\346\240\274\345\274\217:  ", Q_NULLPTR));
        comboBox_3->clear();
        comboBox_3->insertItems(0, QStringList()
         << QApplication::translate("LabelText", "\351\273\230\350\256\244", Q_NULLPTR)
         << QApplication::translate("LabelText", "\345\256\232\345\210\266", Q_NULLPTR)
         << QApplication::translate("LabelText", "\350\204\232\346\234\254\345\214\226", Q_NULLPTR)
        );
        label_14->setText(QApplication::translate("LabelText", "\350\204\232\346\234\254\346\226\207\346\234\254:  ", Q_NULLPTR));
        pushButton_9->setText(QApplication::translate("LabelText", "\346\233\264\345\244\232", Q_NULLPTR));
        label_15->setText(QApplication::translate("LabelText", "\346\237\261\347\212\266\345\233\276\346\240\207\351\242\230:", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class LabelText: public Ui_LabelText {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LABELTEXT_H
