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
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AddPlotPair
{
public:
    QVBoxLayout *verticalLayout_2;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QVBoxLayout *verticalLayout_3;
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
    QWidget *page_2;
    QVBoxLayout *verticalLayout_6;
    QHBoxLayout *horizontalLayout_19;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_15;
    QRadioButton *radioButton_7;
    QSpacerItem *horizontalSpacer_13;
    QHBoxLayout *horizontalLayout_13;
    QRadioButton *radioButton_5;
    QSpacerItem *horizontalSpacer_11;
    QHBoxLayout *horizontalLayout_14;
    QRadioButton *radioButton_6;
    QSpacerItem *horizontalSpacer_12;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_16;
    QRadioButton *radioButton_8;
    QSpacerItem *horizontalSpacer_14;
    QHBoxLayout *horizontalLayout_8;
    QRadioButton *radioButton_3;
    QSpacerItem *horizontalSpacer_7;
    QHBoxLayout *horizontalLayout_9;
    QRadioButton *radioButton_4;
    QSpacerItem *horizontalSpacer_8;
    QHBoxLayout *horizontalLayout_20;
    QTreeWidget *treeWidget_Entity_3;
    QTreeWidget *treeWidget_Entity_2;
    QHBoxLayout *horizontalLayout_21;
    QHBoxLayout *horizontalLayout_17;
    QLabel *label_5;
    QLineEdit *lineEdit_5;
    QToolButton *toolButton_3;
    QSpacerItem *horizontalSpacer_15;
    QPushButton *pushButton_8;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_4;
    QLineEdit *lineEdit_4;
    QToolButton *toolButton_2;
    QSpacerItem *horizontalSpacer_10;
    QPushButton *pushButton_7;
    QHBoxLayout *horizontalLayout_22;
    QTreeWidget *treeWidget_nameUnits_3;
    QTreeWidget *treeWidget_nameUnits_2;
    QHBoxLayout *horizontalLayout_23;
    QHBoxLayout *horizontalLayout_18;
    QLabel *label_6;
    QLineEdit *lineEdit_6;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_3;
    QLineEdit *lineEdit_3;
    QTreeWidget *treeWidget_union;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_add;
    QPushButton *pushButton_update;
    QPushButton *pushButton_remove;
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
        stackedWidget = new QStackedWidget(AddPlotPair);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_3 = new QVBoxLayout(page);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox = new QGroupBox(page);
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


        verticalLayout_3->addWidget(groupBox);

        treeWidget_Entity = new QTreeWidget(page);
        treeWidget_Entity->setObjectName(QStringLiteral("treeWidget_Entity"));
        treeWidget_Entity->setIndentation(0);

        verticalLayout_3->addWidget(treeWidget_Entity);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label = new QLabel(page);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit = new QLineEdit(page);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);

        toolButton = new QToolButton(page);
        toolButton->setObjectName(QStringLiteral("toolButton"));

        horizontalLayout_3->addWidget(toolButton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_3);

        pushButton_4 = new QPushButton(page);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_3->addWidget(pushButton_4);


        verticalLayout_3->addLayout(horizontalLayout_3);

        treeWidget_nameUnits = new QTreeWidget(page);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("\345\220\215\347\247\260"));
        treeWidget_nameUnits->setHeaderItem(__qtreewidgetitem);
        treeWidget_nameUnits->setObjectName(QStringLiteral("treeWidget_nameUnits"));
        treeWidget_nameUnits->setIndentation(0);

        verticalLayout_3->addWidget(treeWidget_nameUnits);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_2 = new QLabel(page);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_4->addWidget(label_2);

        lineEdit_2 = new QLineEdit(page);
        lineEdit_2->setObjectName(QStringLiteral("lineEdit_2"));

        horizontalLayout_4->addWidget(lineEdit_2);


        verticalLayout_3->addLayout(horizontalLayout_4);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        verticalLayout_6 = new QVBoxLayout(page_2);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        horizontalLayout_19 = new QHBoxLayout();
        horizontalLayout_19->setObjectName(QStringLiteral("horizontalLayout_19"));
        groupBox_3 = new QGroupBox(page_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_15 = new QHBoxLayout();
        horizontalLayout_15->setObjectName(QStringLiteral("horizontalLayout_15"));
        radioButton_7 = new QRadioButton(groupBox_3);
        radioButton_7->setObjectName(QStringLiteral("radioButton_7"));

        horizontalLayout_15->addWidget(radioButton_7);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_15->addItem(horizontalSpacer_13);


        verticalLayout_5->addLayout(horizontalLayout_15);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        radioButton_5 = new QRadioButton(groupBox_3);
        radioButton_5->setObjectName(QStringLiteral("radioButton_5"));

        horizontalLayout_13->addWidget(radioButton_5);

        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_13->addItem(horizontalSpacer_11);


        verticalLayout_5->addLayout(horizontalLayout_13);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        radioButton_6 = new QRadioButton(groupBox_3);
        radioButton_6->setObjectName(QStringLiteral("radioButton_6"));

        horizontalLayout_14->addWidget(radioButton_6);

        horizontalSpacer_12 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_14->addItem(horizontalSpacer_12);


        verticalLayout_5->addLayout(horizontalLayout_14);


        horizontalLayout_19->addWidget(groupBox_3);

        groupBox_2 = new QGroupBox(page_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_16 = new QHBoxLayout();
        horizontalLayout_16->setObjectName(QStringLiteral("horizontalLayout_16"));
        radioButton_8 = new QRadioButton(groupBox_2);
        radioButton_8->setObjectName(QStringLiteral("radioButton_8"));

        horizontalLayout_16->addWidget(radioButton_8);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_16->addItem(horizontalSpacer_14);


        verticalLayout_4->addLayout(horizontalLayout_16);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        radioButton_3 = new QRadioButton(groupBox_2);
        radioButton_3->setObjectName(QStringLiteral("radioButton_3"));

        horizontalLayout_8->addWidget(radioButton_3);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_8->addItem(horizontalSpacer_7);


        verticalLayout_4->addLayout(horizontalLayout_8);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        radioButton_4 = new QRadioButton(groupBox_2);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));

        horizontalLayout_9->addWidget(radioButton_4);

        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_8);


        verticalLayout_4->addLayout(horizontalLayout_9);


        horizontalLayout_19->addWidget(groupBox_2);


        verticalLayout_6->addLayout(horizontalLayout_19);

        horizontalLayout_20 = new QHBoxLayout();
        horizontalLayout_20->setObjectName(QStringLiteral("horizontalLayout_20"));
        treeWidget_Entity_3 = new QTreeWidget(page_2);
        treeWidget_Entity_3->setObjectName(QStringLiteral("treeWidget_Entity_3"));
        treeWidget_Entity_3->setIndentation(0);

        horizontalLayout_20->addWidget(treeWidget_Entity_3);

        treeWidget_Entity_2 = new QTreeWidget(page_2);
        treeWidget_Entity_2->setObjectName(QStringLiteral("treeWidget_Entity_2"));
        treeWidget_Entity_2->setIndentation(0);

        horizontalLayout_20->addWidget(treeWidget_Entity_2);


        verticalLayout_6->addLayout(horizontalLayout_20);

        horizontalLayout_21 = new QHBoxLayout();
        horizontalLayout_21->setObjectName(QStringLiteral("horizontalLayout_21"));
        horizontalLayout_17 = new QHBoxLayout();
        horizontalLayout_17->setObjectName(QStringLiteral("horizontalLayout_17"));
        label_5 = new QLabel(page_2);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_17->addWidget(label_5);

        lineEdit_5 = new QLineEdit(page_2);
        lineEdit_5->setObjectName(QStringLiteral("lineEdit_5"));

        horizontalLayout_17->addWidget(lineEdit_5);

        toolButton_3 = new QToolButton(page_2);
        toolButton_3->setObjectName(QStringLiteral("toolButton_3"));

        horizontalLayout_17->addWidget(toolButton_3);

        horizontalSpacer_15 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_17->addItem(horizontalSpacer_15);

        pushButton_8 = new QPushButton(page_2);
        pushButton_8->setObjectName(QStringLiteral("pushButton_8"));

        horizontalLayout_17->addWidget(pushButton_8);


        horizontalLayout_21->addLayout(horizontalLayout_17);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_4 = new QLabel(page_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_12->addWidget(label_4);

        lineEdit_4 = new QLineEdit(page_2);
        lineEdit_4->setObjectName(QStringLiteral("lineEdit_4"));

        horizontalLayout_12->addWidget(lineEdit_4);

        toolButton_2 = new QToolButton(page_2);
        toolButton_2->setObjectName(QStringLiteral("toolButton_2"));

        horizontalLayout_12->addWidget(toolButton_2);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_10);

        pushButton_7 = new QPushButton(page_2);
        pushButton_7->setObjectName(QStringLiteral("pushButton_7"));

        horizontalLayout_12->addWidget(pushButton_7);


        horizontalLayout_21->addLayout(horizontalLayout_12);


        verticalLayout_6->addLayout(horizontalLayout_21);

        horizontalLayout_22 = new QHBoxLayout();
        horizontalLayout_22->setObjectName(QStringLiteral("horizontalLayout_22"));
        treeWidget_nameUnits_3 = new QTreeWidget(page_2);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QString::fromUtf8("\345\220\215\347\247\260"));
        treeWidget_nameUnits_3->setHeaderItem(__qtreewidgetitem1);
        treeWidget_nameUnits_3->setObjectName(QStringLiteral("treeWidget_nameUnits_3"));
        treeWidget_nameUnits_3->setIndentation(0);

        horizontalLayout_22->addWidget(treeWidget_nameUnits_3);

        treeWidget_nameUnits_2 = new QTreeWidget(page_2);
        QTreeWidgetItem *__qtreewidgetitem2 = new QTreeWidgetItem();
        __qtreewidgetitem2->setText(0, QString::fromUtf8("\345\220\215\347\247\260"));
        treeWidget_nameUnits_2->setHeaderItem(__qtreewidgetitem2);
        treeWidget_nameUnits_2->setObjectName(QStringLiteral("treeWidget_nameUnits_2"));
        treeWidget_nameUnits_2->setIndentation(0);

        horizontalLayout_22->addWidget(treeWidget_nameUnits_2);


        verticalLayout_6->addLayout(horizontalLayout_22);

        horizontalLayout_23 = new QHBoxLayout();
        horizontalLayout_23->setObjectName(QStringLiteral("horizontalLayout_23"));
        horizontalLayout_18 = new QHBoxLayout();
        horizontalLayout_18->setObjectName(QStringLiteral("horizontalLayout_18"));
        label_6 = new QLabel(page_2);
        label_6->setObjectName(QStringLiteral("label_6"));

        horizontalLayout_18->addWidget(label_6);

        lineEdit_6 = new QLineEdit(page_2);
        lineEdit_6->setObjectName(QStringLiteral("lineEdit_6"));

        horizontalLayout_18->addWidget(lineEdit_6);


        horizontalLayout_23->addLayout(horizontalLayout_18);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_3 = new QLabel(page_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_10->addWidget(label_3);

        lineEdit_3 = new QLineEdit(page_2);
        lineEdit_3->setObjectName(QStringLiteral("lineEdit_3"));

        horizontalLayout_10->addWidget(lineEdit_3);


        horizontalLayout_23->addLayout(horizontalLayout_10);


        verticalLayout_6->addLayout(horizontalLayout_23);

        stackedWidget->addWidget(page_2);

        verticalLayout_2->addWidget(stackedWidget);

        treeWidget_union = new QTreeWidget(AddPlotPair);
        treeWidget_union->setObjectName(QStringLiteral("treeWidget_union"));
        treeWidget_union->setIndentation(0);

        verticalLayout_2->addWidget(treeWidget_union);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_4);

        pushButton_add = new QPushButton(AddPlotPair);
        pushButton_add->setObjectName(QStringLiteral("pushButton_add"));

        horizontalLayout_5->addWidget(pushButton_add);

        pushButton_update = new QPushButton(AddPlotPair);
        pushButton_update->setObjectName(QStringLiteral("pushButton_update"));

        horizontalLayout_5->addWidget(pushButton_update);

        pushButton_remove = new QPushButton(AddPlotPair);
        pushButton_remove->setObjectName(QStringLiteral("pushButton_remove"));

        horizontalLayout_5->addWidget(pushButton_remove);


        verticalLayout_2->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_5);

        pushButton_close = new QPushButton(AddPlotPair);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));

        horizontalLayout_6->addWidget(pushButton_close);


        verticalLayout_2->addLayout(horizontalLayout_6);


        retranslateUi(AddPlotPair);

        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(AddPlotPair);
    } // setupUi

    void retranslateUi(QWidget *AddPlotPair)
    {
        AddPlotPair->setWindowTitle(QApplication::translate("AddPlotPair", "Form", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("AddPlotPair", "\346\225\260\346\215\256\346\272\220", Q_NULLPTR));
        radioButton->setText(QApplication::translate("AddPlotPair", "\345\217\202\346\225\260", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("AddPlotPair", "\350\214\203\345\233\264\350\256\241\347\256\227", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_Entity->headerItem();
        ___qtreewidgetitem->setText(0, QApplication::translate("AddPlotPair", "\345\256\236\344\275\223", Q_NULLPTR));
        label->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211\345\231\250", Q_NULLPTR));
        toolButton->setText(QApplication::translate("AddPlotPair", "...", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_nameUnits->headerItem();
        ___qtreewidgetitem1->setText(1, QApplication::translate("AddPlotPair", "\345\215\225\344\275\215", Q_NULLPTR));
        label_2->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211:", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AddPlotPair", "\350\247\222\345\272\246\346\225\260\346\215\256\346\272\220", Q_NULLPTR));
        radioButton_7->setText(QApplication::translate("AddPlotPair", "\346\227\266\351\227\264", Q_NULLPTR));
        radioButton_5->setText(QApplication::translate("AddPlotPair", "\345\217\202\346\225\260", Q_NULLPTR));
        radioButton_6->setText(QApplication::translate("AddPlotPair", "\350\214\203\345\233\264\350\256\241\347\256\227", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AddPlotPair", "\351\207\217\347\272\247\346\225\260\346\215\256\346\272\220", Q_NULLPTR));
        radioButton_8->setText(QApplication::translate("AddPlotPair", "\346\227\266\351\227\264", Q_NULLPTR));
        radioButton_3->setText(QApplication::translate("AddPlotPair", "\345\217\202\346\225\260", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("AddPlotPair", "\350\214\203\345\233\264\350\256\241\347\256\227", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem2 = treeWidget_Entity_3->headerItem();
        ___qtreewidgetitem2->setText(0, QApplication::translate("AddPlotPair", "\345\256\236\344\275\223", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem3 = treeWidget_Entity_2->headerItem();
        ___qtreewidgetitem3->setText(0, QApplication::translate("AddPlotPair", "\345\256\236\344\275\223", Q_NULLPTR));
        label_5->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211\345\231\250", Q_NULLPTR));
        toolButton_3->setText(QApplication::translate("AddPlotPair", "...", Q_NULLPTR));
        pushButton_8->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211", Q_NULLPTR));
        label_4->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211\345\231\250", Q_NULLPTR));
        toolButton_2->setText(QApplication::translate("AddPlotPair", "...", Q_NULLPTR));
        pushButton_7->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem4 = treeWidget_nameUnits_3->headerItem();
        ___qtreewidgetitem4->setText(1, QApplication::translate("AddPlotPair", "\345\215\225\344\275\215", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem5 = treeWidget_nameUnits_2->headerItem();
        ___qtreewidgetitem5->setText(1, QApplication::translate("AddPlotPair", "\345\215\225\344\275\215", Q_NULLPTR));
        label_6->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211:", Q_NULLPTR));
        label_3->setText(QApplication::translate("AddPlotPair", "\347\255\233\351\200\211:", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem6 = treeWidget_union->headerItem();
        ___qtreewidgetitem6->setText(1, QApplication::translate("AddPlotPair", "Y \350\275\264", Q_NULLPTR));
        ___qtreewidgetitem6->setText(0, QApplication::translate("AddPlotPair", "X \350\275\264", Q_NULLPTR));
        pushButton_add->setText(QApplication::translate("AddPlotPair", "\346\267\273\345\212\240", Q_NULLPTR));
        pushButton_update->setText(QApplication::translate("AddPlotPair", "\346\233\264\346\226\260", Q_NULLPTR));
        pushButton_remove->setText(QApplication::translate("AddPlotPair", "\347\247\273\351\231\244", Q_NULLPTR));
        pushButton_close->setText(QApplication::translate("AddPlotPair", "\345\205\263\351\227\255", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AddPlotPair: public Ui_AddPlotPair {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADDPLOTPAIR_H
