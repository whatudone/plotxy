/********************************************************************************
** Form generated from reading UI file 'AddPlotPair.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADDPLOTPAIR_H
#define UI_ADDPLOTPAIR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddPlotPair
{
public:
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QRadioButton *radioButton_2;
    QSpacerItem *horizontalSpacer_2;
    QTreeWidget *treeWidget_Entity;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    QLineEdit *lineEdit;
    QToolButton *toolButton;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_4;
    QTreeWidget *treeWidget_nameUnits;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_add;
    QPushButton *pushButton_3;
    QPushButton *pushButton_2;
    QTreeWidget *treeWidget_union;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *pushButton_close;

    void setupUi(QWidget *AddPlotPair)
    {
        if (AddPlotPair->objectName().isEmpty())
            AddPlotPair->setObjectName(QStringLiteral("AddPlotPair"));
        AddPlotPair->resize(711, 709);
        verticalLayout_2 = new QVBoxLayout(AddPlotPair);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox = new QGroupBox(AddPlotPair);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        radioButton = new QRadioButton(groupBox);
        radioButton->setObjectName(QStringLiteral("radioButton"));

        horizontalLayout->addWidget(radioButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        radioButton_2 = new QRadioButton(groupBox);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));

        horizontalLayout_2->addWidget(radioButton_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox);

        treeWidget_Entity = new QTreeWidget(AddPlotPair);
        treeWidget_Entity->setObjectName(QStringLiteral("treeWidget_Entity"));
        treeWidget_Entity->setIndentation(0);

        verticalLayout_2->addWidget(treeWidget_Entity);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(AddPlotPair);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(AddPlotPair);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        toolButton = new QToolButton(AddPlotPair);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        horizontalLayout_3->addWidget(toolButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButton_4 = new QPushButton(AddPlotPair);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_3->addWidget(pushButton_4);


        verticalLayout_2->addLayout(horizontalLayout_3);

        treeWidget_nameUnits = new QTreeWidget(AddPlotPair);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("Name"));
        treeWidget_nameUnits->setHeaderItem(__qtreewidgetitem);
        treeWidget_nameUnits->setObjectName(QStringLiteral("treeWidget_nameUnits"));
        treeWidget_nameUnits->setIndentation(0);

        verticalLayout_2->addWidget(treeWidget_nameUnits);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(AddPlotPair);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        lineEdit_2 = new QLineEdit(AddPlotPair);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_4->addWidget(lineEdit_2);


        verticalLayout_2->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        pushButton_add = new QPushButton(AddPlotPair);
        pushButton_add->setObjectName(QStringLiteral("pushButton_add"));

        horizontalLayout_5->addWidget(pushButton_add);

        pushButton_3 = new QPushButton(AddPlotPair);
        pushButton_3->setObjectName(QStringLiteral("pushButton_3"));

        horizontalLayout_5->addWidget(pushButton_3);

        pushButton_2 = new QPushButton(AddPlotPair);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));

        horizontalLayout_5->addWidget(pushButton_2);


        verticalLayout_2->addLayout(horizontalLayout_5);

        treeWidget_union = new QTreeWidget(AddPlotPair);
        treeWidget_union->setObjectName(QStringLiteral("treeWidget_union"));
        treeWidget_union->setIndentation(0);

        verticalLayout_2->addWidget(treeWidget_union);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        pushButton_close = new QPushButton(AddPlotPair);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));

        horizontalLayout_6->addWidget(pushButton_close);


        verticalLayout_2->addLayout(horizontalLayout_6);


        retranslateUi(AddPlotPair);

        QMetaObject::connectSlotsByName(AddPlotPair);
    } // setupUi

    void retranslateUi(QWidget *AddPlotPair)
    {
        AddPlotPair->setWindowTitle(QApplication::translate("AddPlotPair", "Form", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AddPlotPair", "Data Source", Q_NULLPTR));
        radioButton->setText(QApplication::translate("AddPlotPair", "Parameter", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("AddPlotPair", "Range Calculation", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_Entity->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("AddPlotPair", "Entity", Q_NULLPTR));
        label->setText(QApplication::translate("AddPlotPair", "Filter:", Q_NULLPTR));
        toolButton->setText(QApplication::translate("AddPlotPair", "...", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AddPlotPair", "Sort", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_nameUnits->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("AddPlotPair", "Units", Q_NULLPTR));
        label_2->setText(QApplication::translate("AddPlotPair", "Filter:", Q_NULLPTR));
        pushButton_add->setText(QApplication::translate("AddPlotPair", "Add", Q_NULLPTR));
        pushButton_3->setText(QApplication::translate("AddPlotPair", "Update", Q_NULLPTR));
        pushButton_2->setText(QApplication::translate("AddPlotPair", "Remove", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget_union->headerItem();
        ___qtreewidgetitem2->setText(1, QApplication::translate("AddPlotPair", "Y Column", Q_NULLPTR));
        ___qtreewidgetitem2->setText(0, QApplication::translate("AddPlotPair", "X Column", Q_NULLPTR));
        pushButton_close->setText(QApplication::translate("AddPlotPair", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AddPlotPair: public Ui_AddPlotPair {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDPLOTPAIR_H
