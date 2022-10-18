/********************************************************************************
** Form generated from reading UI file 'AdvancedDataManager.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ADVANCEDDATAMANAGER_H
#define UI_ADVANCEDDATAMANAGER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AdvancedDataManager
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout_6;
    QTreeWidget *treeWidget_xy;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButton_add;
    QPushButton *pushButton_copy;
    QPushButton *pushButton_autofit;
    QPushButton *pushButton_delete;
    QHBoxLayout *horizontalLayout_5;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_3;
    QWidget *widget_drawer;
    QPushButton *pushButton;
    QStackedWidget *stackedWidget_aDMrpart;
    QWidget *page;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *pushButton_clear;
    QPushButton *pushButton_insert;
    QPushButton *pushButton_choosecolor;
    QSpacerItem *horizontalSpacer;
    QPushButton *pushButton_insertcolor;
    QHBoxLayout *horizontalLayout_4;
    QPushButton *pushButton_save;
    QPushButton *pushButton_test;
    QPushButton *pushButton_help;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QWidget *page_2;
    QHBoxLayout *horizontalLayout_7;
    QGroupBox *groupBox_4;
    QVBoxLayout *verticalLayout_7;
    QHBoxLayout *horizontalLayout_11;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QGroupBox *groupBox_6;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_10;
    QLabel *label_3;
    QLineEdit *lineEdit_name;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label_4;
    QLineEdit *lineEdit_value;
    QLabel *label_5;
    QPushButton *pushButton_color;
    QHBoxLayout *horizontalLayout_12;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *pushButton_add_2;
    QPushButton *pushButton_4;
    QPushButton *pushButton_5;
    QHBoxLayout *horizontalLayout_13;
    QTreeWidget *treeWidget_union;
    QWidget *page_3;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *pushButton_close;

    void setupUi(QMainWindow *AdvancedDataManager)
    {
        if (AdvancedDataManager->objectName().isEmpty())
            AdvancedDataManager->setObjectName(QStringLiteral("AdvancedDataManager"));
        AdvancedDataManager->resize(1079, 684);
        centralwidget = new QWidget(AdvancedDataManager);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        verticalLayout_6 = new QVBoxLayout(centralwidget);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        treeWidget_xy = new QTreeWidget(centralwidget);
        treeWidget_xy->setObjectName(QStringLiteral("treeWidget_xy"));

        verticalLayout_6->addWidget(treeWidget_xy);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralwidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout->addWidget(lineEdit);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        pushButton_add = new QPushButton(centralwidget);
        pushButton_add->setObjectName(QStringLiteral("pushButton_add"));

        horizontalLayout->addWidget(pushButton_add);

        pushButton_copy = new QPushButton(centralwidget);
        pushButton_copy->setObjectName(QStringLiteral("pushButton_copy"));

        horizontalLayout->addWidget(pushButton_copy);

        pushButton_autofit = new QPushButton(centralwidget);
        pushButton_autofit->setObjectName(QStringLiteral("pushButton_autofit"));

        horizontalLayout->addWidget(pushButton_autofit);

        pushButton_delete = new QPushButton(centralwidget);
        pushButton_delete->setObjectName(QStringLiteral("pushButton_delete"));

        horizontalLayout->addWidget(pushButton_delete);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 8);
        horizontalLayout->setStretch(3, 1);
        horizontalLayout->setStretch(4, 1);
        horizontalLayout->setStretch(5, 1);
        horizontalLayout->setStretch(6, 1);

        verticalLayout_6->addLayout(horizontalLayout);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        groupBox = new QGroupBox(centralwidget);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_3 = new QVBoxLayout(groupBox);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        widget_drawer = new QWidget(groupBox);
        widget_drawer->setObjectName(QStringLiteral("widget_drawer"));
        pushButton = new QPushButton(widget_drawer);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(11, 139, 93, 28));

        verticalLayout_3->addWidget(widget_drawer);


        horizontalLayout_5->addWidget(groupBox);

        stackedWidget_aDMrpart = new QStackedWidget(centralwidget);
        stackedWidget_aDMrpart->setObjectName(QStringLiteral("stackedWidget_aDMrpart"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        verticalLayout_4 = new QVBoxLayout(page);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox_2 = new QGroupBox(page);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_2 = new QVBoxLayout(groupBox_2);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        pushButton_clear = new QPushButton(groupBox_2);
        pushButton_clear->setObjectName(QStringLiteral("pushButton_clear"));

        horizontalLayout_2->addWidget(pushButton_clear);

        pushButton_insert = new QPushButton(groupBox_2);
        pushButton_insert->setObjectName(QStringLiteral("pushButton_insert"));

        horizontalLayout_2->addWidget(pushButton_insert);

        pushButton_choosecolor = new QPushButton(groupBox_2);
        pushButton_choosecolor->setObjectName(QStringLiteral("pushButton_choosecolor"));
        pushButton_choosecolor->setStyleSheet(QStringLiteral(""));

        horizontalLayout_2->addWidget(pushButton_choosecolor);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        pushButton_insertcolor = new QPushButton(groupBox_2);
        pushButton_insertcolor->setObjectName(QStringLiteral("pushButton_insertcolor"));

        horizontalLayout_2->addWidget(pushButton_insertcolor);


        horizontalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        pushButton_save = new QPushButton(groupBox_2);
        pushButton_save->setObjectName(QStringLiteral("pushButton_save"));

        horizontalLayout_4->addWidget(pushButton_save);

        pushButton_test = new QPushButton(groupBox_2);
        pushButton_test->setObjectName(QStringLiteral("pushButton_test"));

        horizontalLayout_4->addWidget(pushButton_test);

        pushButton_help = new QPushButton(groupBox_2);
        pushButton_help->setObjectName(QStringLiteral("pushButton_help"));

        horizontalLayout_4->addWidget(pushButton_help);


        horizontalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_3->setStretch(0, 2);
        horizontalLayout_3->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout_3);

        groupBox_3 = new QGroupBox(groupBox_2);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout = new QVBoxLayout(groupBox_3);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit = new QTextEdit(groupBox_3);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);


        verticalLayout_2->addWidget(groupBox_3);


        verticalLayout_4->addWidget(groupBox_2);

        stackedWidget_aDMrpart->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        horizontalLayout_7 = new QHBoxLayout(page_2);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        groupBox_4 = new QGroupBox(page_2);
        groupBox_4->setObjectName(QStringLiteral("groupBox_4"));
        verticalLayout_7 = new QVBoxLayout(groupBox_4);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QStringLiteral("groupBox_5"));
        horizontalLayout_8 = new QHBoxLayout(groupBox_5);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_2 = new QLabel(groupBox_5);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_8->addWidget(label_2);


        horizontalLayout_11->addWidget(groupBox_5);

        groupBox_6 = new QGroupBox(groupBox_4);
        groupBox_6->setObjectName(QStringLiteral("groupBox_6"));
        verticalLayout_5 = new QVBoxLayout(groupBox_6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        label_3 = new QLabel(groupBox_6);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_10->addWidget(label_3);

        lineEdit_name = new QLineEdit(groupBox_6);
        lineEdit_name->setObjectName(QStringLiteral("lineEdit_name"));

        horizontalLayout_10->addWidget(lineEdit_name);


        verticalLayout_5->addLayout(horizontalLayout_10);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label_4 = new QLabel(groupBox_6);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_9->addWidget(label_4);

        lineEdit_value = new QLineEdit(groupBox_6);
        lineEdit_value->setObjectName(QStringLiteral("lineEdit_value"));

        horizontalLayout_9->addWidget(lineEdit_value);

        label_5 = new QLabel(groupBox_6);
        label_5->setObjectName(QStringLiteral("label_5"));

        horizontalLayout_9->addWidget(label_5);

        pushButton_color = new QPushButton(groupBox_6);
        pushButton_color->setObjectName(QStringLiteral("pushButton_color"));

        horizontalLayout_9->addWidget(pushButton_color);


        verticalLayout_5->addLayout(horizontalLayout_9);


        horizontalLayout_11->addWidget(groupBox_6);


        verticalLayout_7->addLayout(horizontalLayout_11);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_12->addItem(horizontalSpacer_4);

        pushButton_add_2 = new QPushButton(groupBox_4);
        pushButton_add_2->setObjectName(QStringLiteral("pushButton_add_2"));

        horizontalLayout_12->addWidget(pushButton_add_2);

        pushButton_4 = new QPushButton(groupBox_4);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));

        horizontalLayout_12->addWidget(pushButton_4);

        pushButton_5 = new QPushButton(groupBox_4);
        pushButton_5->setObjectName(QStringLiteral("pushButton_5"));

        horizontalLayout_12->addWidget(pushButton_5);


        verticalLayout_7->addLayout(horizontalLayout_12);

        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        treeWidget_union = new QTreeWidget(groupBox_4);
        treeWidget_union->setObjectName(QStringLiteral("treeWidget_union"));
        treeWidget_union->setIndentation(0);

        horizontalLayout_13->addWidget(treeWidget_union);


        verticalLayout_7->addLayout(horizontalLayout_13);


        horizontalLayout_7->addWidget(groupBox_4);

        stackedWidget_aDMrpart->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName(QStringLiteral("page_3"));
        stackedWidget_aDMrpart->addWidget(page_3);

        horizontalLayout_5->addWidget(stackedWidget_aDMrpart);


        verticalLayout_6->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_3);

        pushButton_close = new QPushButton(centralwidget);
        pushButton_close->setObjectName(QStringLiteral("pushButton_close"));

        horizontalLayout_6->addWidget(pushButton_close);


        verticalLayout_6->addLayout(horizontalLayout_6);

        AdvancedDataManager->setCentralWidget(centralwidget);

        retranslateUi(AdvancedDataManager);

        stackedWidget_aDMrpart->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(AdvancedDataManager);
    } // setupUi

    void retranslateUi(QMainWindow *AdvancedDataManager)
    {
        AdvancedDataManager->setWindowTitle(QApplication::translate("AdvancedDataManager", "MainWindow", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem = treeWidget_xy->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("AdvancedDataManager", "\346\211\200\345\234\250Tab", Q_NULLPTR));
        ___qtreewidgetitem->setText(2, QApplication::translate("AdvancedDataManager", "\345\233\276\350\241\250\345\220\215\347\247\260", Q_NULLPTR));
        ___qtreewidgetitem->setText(1, QApplication::translate("AdvancedDataManager", "Y \350\275\264", Q_NULLPTR));
        ___qtreewidgetitem->setText(0, QApplication::translate("AdvancedDataManager", "X \350\275\264", Q_NULLPTR));
        label->setText(QApplication::translate("AdvancedDataManager", "\347\255\233\351\200\211\345\231\250\357\274\232", Q_NULLPTR));
        pushButton_add->setText(QApplication::translate("AdvancedDataManager", "\346\267\273\345\212\240", Q_NULLPTR));
        pushButton_copy->setText(QApplication::translate("AdvancedDataManager", "\345\244\215\345\210\266", Q_NULLPTR));
        pushButton_autofit->setText(QApplication::translate("AdvancedDataManager", "\350\207\252\345\212\250\351\200\202\345\272\224", Q_NULLPTR));
        pushButton_delete->setText(QApplication::translate("AdvancedDataManager", "\345\210\240\351\231\244", Q_NULLPTR));
        groupBox->setTitle(QString());
        pushButton->setText(QApplication::translate("AdvancedDataManager", "PushButton", Q_NULLPTR));
        groupBox_2->setTitle(QApplication::translate("AdvancedDataManager", "\351\242\235\345\244\226\350\256\276\347\275\256", Q_NULLPTR));
        pushButton_clear->setText(QApplication::translate("AdvancedDataManager", "\346\270\205\347\220\206", Q_NULLPTR));
        pushButton_insert->setText(QApplication::translate("AdvancedDataManager", "\346\217\222\345\205\245 If/Else", Q_NULLPTR));
        pushButton_choosecolor->setText(QString());
        pushButton_insertcolor->setText(QApplication::translate("AdvancedDataManager", "\346\217\222\345\205\245\351\242\234\350\211\262", Q_NULLPTR));
        pushButton_save->setText(QApplication::translate("AdvancedDataManager", "\344\277\235\345\255\230", Q_NULLPTR));
        pushButton_test->setText(QApplication::translate("AdvancedDataManager", "\346\265\213\350\257\225", Q_NULLPTR));
        pushButton_help->setText(QApplication::translate("AdvancedDataManager", "\345\270\256\345\212\251...", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("AdvancedDataManager", "Code", Q_NULLPTR));
        textEdit->setHtml(QApplication::translate("AdvancedDataManager", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'SimSun'; font-size:9pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">#xVal and yVal contain the x and y value of the plot data for 2d plots.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">#xVal contains the value of the plot data  for 1D plots.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">#plot Data can be used in set functions to set properties of the plot pair.</p>\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0"
                        "; text-indent:0px;\">Set Color (olot Data,RGBA(255,0,0,255))</p>\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", Q_NULLPTR));
        groupBox_4->setTitle(QApplication::translate("AdvancedDataManager", "\351\242\235\345\244\226\350\256\276\347\275\256", Q_NULLPTR));
        groupBox_5->setTitle(QApplication::translate("AdvancedDataManager", "\345\205\263\344\272\216\351\230\210\345\200\274", Q_NULLPTR));
        label_2->setText(QApplication::translate("AdvancedDataManager", "<html><head/><body><p align=\"justify\">\351\230\210\345\200\274\345\214\205\345\220\253\345\200\274\345\222\214\351\242\234\350\211\262\343\200\202\350\257\245\345\200\274\350\241\250\347\244\272\351\242\234\350\211\262\350\214\203\345\233\264\347\232\204\346\234\200\345\260\217\345\200\274\357\274\214</p><p align=\"justify\">\350\257\245\351\242\234\350\211\262\347\224\250\344\272\216\345\241\253\345\205\205\350\257\245\350\214\203\345\233\264\343\200\202\351\242\234\350\211\262\350\214\203\345\233\264\344\273\216\344\270\200\344\270\252\351\230\210\345\200\274\345\274\200\345\247\213\357\274\214</p><p align=\"justify\">\345\210\260\344\270\213\344\270\200\344\270\252\351\230\210\345\200\274\347\273\223\346\235\237\343\200\202\345\220\215\347\247\260\345\222\214\351\242\234\350\211\262\345\217\257\344\273\245\345\234\250\351\230\210\345\200\274\344\271\213\351\227\264\345\205\261</p><p align=\"justify\">\344\272\253\357\274\214\344\275\206\345\200\274\344\270\215\350\203\275\345\205\261\344\272\253\343\200\202"
                        "</p></body></html>", Q_NULLPTR));
        groupBox_6->setTitle(QApplication::translate("AdvancedDataManager", "\351\230\210\345\200\274\351\200\211\351\241\271", Q_NULLPTR));
        label_3->setText(QApplication::translate("AdvancedDataManager", "\345\220\215\347\247\260:", Q_NULLPTR));
        label_4->setText(QApplication::translate("AdvancedDataManager", "  \345\200\274:", Q_NULLPTR));
        label_5->setText(QApplication::translate("AdvancedDataManager", "\351\242\234\350\211\262:", Q_NULLPTR));
        pushButton_color->setText(QString());
        pushButton_add_2->setText(QApplication::translate("AdvancedDataManager", "\346\267\273\345\212\240", Q_NULLPTR));
        pushButton_4->setText(QApplication::translate("AdvancedDataManager", "\346\233\264\346\226\260", Q_NULLPTR));
        pushButton_5->setText(QApplication::translate("AdvancedDataManager", "\347\247\273\351\231\244", Q_NULLPTR));
        QTreeWidgetItem *___qtreewidgetitem1 = treeWidget_union->headerItem();
        ___qtreewidgetitem1->setText(2, QApplication::translate("AdvancedDataManager", "\351\242\234\350\211\262", Q_NULLPTR));
        ___qtreewidgetitem1->setText(1, QApplication::translate("AdvancedDataManager", "\345\200\274", Q_NULLPTR));
        ___qtreewidgetitem1->setText(0, QApplication::translate("AdvancedDataManager", "\345\220\215\347\247\260", Q_NULLPTR));
        pushButton_close->setText(QApplication::translate("AdvancedDataManager", "\345\205\263\351\227\255", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AdvancedDataManager: public Ui_AdvancedDataManager {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ADVANCEDDATAMANAGER_H
