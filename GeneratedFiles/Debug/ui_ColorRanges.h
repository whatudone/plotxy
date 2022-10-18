/********************************************************************************
** Form generated from reading UI file 'ColorRanges.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORRANGES_H
#define UI_COLORRANGES_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorRanges
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_26;
    QCheckBox *checkBox_8;
    QSpacerItem *horizontalSpacer_25;
    QHBoxLayout *horizontalLayout_27;
    QLabel *label;
    QPushButton *pushButton;
    QSpacerItem *horizontalSpacer_26;
    QHBoxLayout *horizontalLayout_28;
    QLabel *label_18;
    QComboBox *comboBox_5;
    QSpacerItem *horizontalSpacer_27;
    QHBoxLayout *horizontalLayout_29;
    QLabel *label_19;
    QPushButton *pushButton_more;
    QSpacerItem *horizontalSpacer_28;
    QSpacerItem *verticalSpacer;

    void setupUi(QWidget *ColorRanges)
    {
        if (ColorRanges->objectName().isEmpty())
            ColorRanges->setObjectName(QStringLiteral("ColorRanges"));
        ColorRanges->resize(454, 287);
        verticalLayout = new QVBoxLayout(ColorRanges);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_26 = new QHBoxLayout();
        horizontalLayout_26->setObjectName(QStringLiteral("horizontalLayout_26"));
        checkBox_8 = new QCheckBox(ColorRanges);
        checkBox_8->setObjectName(QStringLiteral("checkBox_8"));

        horizontalLayout_26->addWidget(checkBox_8);

        horizontalSpacer_25 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_26->addItem(horizontalSpacer_25);


        verticalLayout->addLayout(horizontalLayout_26);

        horizontalLayout_27 = new QHBoxLayout();
        horizontalLayout_27->setObjectName(QStringLiteral("horizontalLayout_27"));
        label = new QLabel(ColorRanges);
        label->setObjectName(QStringLiteral("label"));
        label->setFrameShape(QFrame::Box);

        horizontalLayout_27->addWidget(label);

        pushButton = new QPushButton(ColorRanges);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout_27->addWidget(pushButton);

        horizontalSpacer_26 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_27->addItem(horizontalSpacer_26);


        verticalLayout->addLayout(horizontalLayout_27);

        horizontalLayout_28 = new QHBoxLayout();
        horizontalLayout_28->setObjectName(QStringLiteral("horizontalLayout_28"));
        label_18 = new QLabel(ColorRanges);
        label_18->setObjectName(QStringLiteral("label_18"));
        label_18->setFrameShape(QFrame::Box);

        horizontalLayout_28->addWidget(label_18);

        comboBox_5 = new QComboBox(ColorRanges);
        comboBox_5->setObjectName(QStringLiteral("comboBox_5"));

        horizontalLayout_28->addWidget(comboBox_5);

        horizontalSpacer_27 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_28->addItem(horizontalSpacer_27);


        verticalLayout->addLayout(horizontalLayout_28);

        horizontalLayout_29 = new QHBoxLayout();
        horizontalLayout_29->setObjectName(QStringLiteral("horizontalLayout_29"));
        label_19 = new QLabel(ColorRanges);
        label_19->setObjectName(QStringLiteral("label_19"));
        label_19->setFrameShape(QFrame::Box);

        horizontalLayout_29->addWidget(label_19);

        pushButton_more = new QPushButton(ColorRanges);
        pushButton_more->setObjectName(QStringLiteral("pushButton_more"));

        horizontalLayout_29->addWidget(pushButton_more);

        horizontalSpacer_28 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_29->addItem(horizontalSpacer_28);


        verticalLayout->addLayout(horizontalLayout_29);

        verticalSpacer = new QSpacerItem(20, 137, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        retranslateUi(ColorRanges);

        QMetaObject::connectSlotsByName(ColorRanges);
    } // setupUi

    void retranslateUi(QWidget *ColorRanges)
    {
        ColorRanges->setWindowTitle(QApplication::translate("ColorRanges", "Form", Q_NULLPTR));
        checkBox_8->setText(QApplication::translate("ColorRanges", "\345\274\200\345\220\257", Q_NULLPTR));
        label->setText(QApplication::translate("ColorRanges", "\351\273\230\350\256\244\351\242\234\350\211\262", Q_NULLPTR));
        pushButton->setText(QString());
        label_18->setText(QApplication::translate("ColorRanges", "\346\230\276\347\244\272\351\200\211\351\241\271", Q_NULLPTR));
        comboBox_5->clear();
        comboBox_5->insertItems(0, QStringList()
         << QApplication::translate("ColorRanges", "\345\215\225\350\211\262\346\230\276\347\244\272", Q_NULLPTR)
         << QApplication::translate("ColorRanges", "\345\244\232\346\256\265\346\230\276\347\244\272", Q_NULLPTR)
         << QApplication::translate("ColorRanges", "\346\270\220\345\217\230", Q_NULLPTR)
        );
        label_19->setText(QApplication::translate("ColorRanges", "\347\274\226\350\276\221\350\211\262\345\275\251\350\214\203\345\233\264:", Q_NULLPTR));
        pushButton_more->setText(QApplication::translate("ColorRanges", "\346\233\264\345\244\232>>", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ColorRanges: public Ui_ColorRanges {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORRANGES_H
