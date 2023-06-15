﻿#include "PlotManager.h"
#include "AddPlotPair.h"
#include "DataManager.h"

#include "PlotAScope.h"
#include "PlotAttitude.h"
#include "PlotDial.h"
#include "PlotManagerData.h"
#include "PlotScatter.h"

#include <QAction>
#include <QColorDialog>
#include <QDebug>
#include <QHeaderView>
#include <QSlider>
#include <QString>
#include <QStyleFactory>
#include <QTableWidgetItem>
#include <QTreeWidget>

PlotManager::PlotManager(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.pushButton_close, &QPushButton::clicked, this, &PlotManager::onBtnCloseClicked);
    this->setWindowTitle(QString("图表管理器"));
    init();

    connect(PlotManagerData::getInstance(),
            &PlotManagerData::plotDataChanged,
            this,
            &PlotManager::onUpdatePlotManager);
    connect(this,
            SIGNAL(sigChangePlotName()),
            PlotManagerData::getInstance(),
            SLOT(slotChangePlotName()));
    ui.treeWidget_selectedPlots->setStyle(QStyleFactory::create("windows"));
    ui.treeWidget_selectedPlots->setHeaderHidden(true);
    ui.treeWidget_selectedPlots->expandAll();
    ui.stackedWidget->setCurrentIndex(0);
    connect(ui.treeWidget_selectedPlots,
            SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(onTWSPclicked(QTreeWidgetItem*, int)));
    connect(ui.spinBox_between,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &PlotManager::spinboxBetweenChanged);
    connect(ui.spinBox_left,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &PlotManager::spinboxLeftChanged);
    connect(ui.spinBox_right,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &PlotManager::spinboxRightChanged);
    connect(ui.checkBox_32, &QCheckBox::stateChanged, this, &PlotManager::onBarHorizonChanged);
}

PlotManager::~PlotManager() {}

void PlotManager::init()
{
    initTreeWidgetSettings();
    initGeneralUI();
    initAxisGridUI();
    initGOGUI();

    initScatterLimitUI();
    initPlotDataUI();
    initTextEditUI();
    initAttitudeUI();
    initTextLightUI();
    initDialUI();

    initEditableMap();
}

void PlotManager::addPlot(const QString& tabName, PlotItemBase* plotItem)
{
    auto plotDataMap = PlotManagerData::getInstance()->getPlotManagerData();

    //显示层更新
    if(plotDataMap.contains(tabName))
    {
        QList<QTreeWidgetItem*> topWidget =
            ui.treeWidget_selectedPlots->findItems(tabName, Qt::MatchCaseSensitive, 0);
        if(topWidget.size() != 0)
        {
            QTreeWidgetItem* itemselPlotI =
                new QTreeWidgetItem(QStringList() << plotItem->getName());
            topWidget[0]->addChild(itemselPlotI);
        }
    }
    else
    {
        QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabName);
        ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
        ui.treeWidget_selectedPlots->expandAll();

        QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->getName());
        itemselPlotH->addChild(itemselPlotI);

        //comboBox_tabName
        ui.comboBox_tabName->addItem(tabName);
    }
}

void PlotManager::initTreeWidgetSettings()
{
    ui.treeWidget_settings->setHeaderHidden(false);
    ui.treeWidget_settings->setHeaderLabel(QString("设置"));
    ui.treeWidget_settings->setIndentation(15);

    connect(ui.treeWidget_settings,
            SIGNAL(itemClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(onTWSclicked(QTreeWidgetItem*, int)));

    m_itemGeneral = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("通用设置")));
    m_itemAxis =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("坐标轴和网格设置")));
    m_itemLinkedAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("链接轴")));
    m_itemPlotData = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("数据设置")));
    m_itemGOG = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("GOG曲线")));
    m_itemText = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("文本信息")));
    m_itemScatterPlot =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Scatter设置")));
    m_itemAScope = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("A-Scope设置")));
    m_itemRTI = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("RTI设置")));
    m_itemTextLight =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Text/Light设置")));
    m_itemBar = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Bar设置")));
    m_itemDial = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Dials设置")));
    m_itemAttitude =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Attitude设置")));
    m_itemTrackStatus =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Track Status设置")));
    m_itemRangeDoppler =
        new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString("Range Doppler设置")));

    m_itemLimits = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString("限制")));
    m_itemPlotMarkers = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString("标记")));
    m_itemTimeLine = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Time Line"));
    m_itemHandsOff = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Hands-Off"));

    //	ui.treeWidget_settings->setCurrentItem(m_itemGeneral);
    m_itemScatterPlot->setExpanded(true);

    ui.treeWidget_settings->setEnabled(true);
}

void PlotManager::initGeneralUI()
{
    ui.pushButton_flipHeight->setVisible(false);
    ui.pushButton_flipWidth->setVisible(false);
    ui.radioButton_percent->setChecked(true);
    ui.radioButton_pixel->setChecked(false);

    connect(
        ui.radioButton_percent, &QRadioButton::clicked, this, &PlotManager::onRadioPercentClicked);
    connect(ui.radioButton_pixel, &QRadioButton::clicked, this, &PlotManager::onRadioPixelClicked);
    connect(ui.lineEdit_plotPositionX,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onPlotRectEditFinished);
    connect(ui.lineEdit_plotPositionY,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onPlotRectEditFinished);
    connect(ui.lineEdit_plotWidth,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onPlotRectEditFinished);
    connect(ui.lineEdit_plotHeight,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onPlotRectEditFinished);
    connect(ui.checkBox_draw,
            &QCheckBox::stateChanged,
            this,
            &PlotManager::onCheckBox_drawStateChanged);
    connect(ui.lineEdit_plotName,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEditPlotNameEditingFinished);
    connect(ui.pushButton_outerFillColor,
            &QPushButton::clicked,
            this,
            &PlotManager::onPushButton_outerFillColorClicked);
    connect(ui.pushButton_outlineColor,
            &QPushButton::clicked,
            this,
            &PlotManager::onPushButton_outlineColorClicked);
}

void PlotManager::initAxisGridUI()
{
    ui.pushButton_flipXValues->setVisible(false);
    ui.pushButton_flipYValues->setVisible(false);

    ui.lineEdit_23->setText("1");
    /*ui.pushButton_gridColor->setColor(Qt::gray);*/
    ui.lineEdit_10->setEnabled(false);
    ui.lineEdit_11->setEnabled(false);
    ui.lineEdit_12->setEnabled(false);
    ui.lineEdit_13->setEnabled(false);
    ui.tableWidget->setEnabled(false);
    ui.tableWidget_2->setEnabled(false);
    ui.comboBox_2->setCurrentIndex(0);

    connect(ui.checkBox_4, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_4StateChanged);
    connect(ui.checkBox_5, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_5StateChanged);
    connect(ui.lineEdit_limitBgnX,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_limitXEditingFinished);
    connect(ui.lineEdit_LimitBgnY,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_limitYEditingFinished);
    connect(ui.lineEdit_limitEndX,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_limitXEditingFinished);
    connect(ui.lineEdit_limitEndY,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_limitYEditingFinished);
    connect(ui.pushButton_flipXValues,
            &QPushButton::clicked,
            this,
            &PlotManager::onPushButton_flipXValuesClicked);
    connect(ui.pushButton_flipYValues,
            &QPushButton::clicked,
            this,
            &PlotManager::onPushButton_flipYValuesClicked);
    connect(ui.lineEdit_hrozGrids,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_horzGridsEditingFinished);
    connect(ui.lineEdit_vertGrids,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_vertGridsEditingFinished);
    connect(ui.lineEdit_21, &QLineEdit::editingFinished, this, &PlotManager::onSetAxisColorWidth);
    connect(ui.lineEdit_23, &QLineEdit::editingFinished, this, &PlotManager::onSetGridColorWidth);
    connect(
        ui.pushButton_axisColor, &QPushButton::clicked, this, &PlotManager::onSetAxisColorWidth);
    connect(
        ui.pushButton_gridColor, &QPushButton::clicked, this, &PlotManager::onSetGridColorWidth);
    connect(ui.pushButton_gridFill,
            &QPushButton::clicked,
            this,
            &PlotManager::onPushButton_gridFillClicked);
    connect(ui.checkBox_6, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_6StateChanged);
    connect(ui.pushButton_10, &QPushButton::clicked, this, &PlotManager::onPushButton_10Clicked);
    connect(ui.fontComboBox_3,
            &QFontComboBox::currentFontChanged,
            this,
            &PlotManager::onfontComboBox_3CurrentFontChanged);
    connect(ui.comboBox_AxisGrid_FontSize,
            &QComboBox::currentTextChanged,
            this,
            &PlotManager::onComboBox_AxisGrid_FontSizeCurrentTextChanged);
    connect(ui.comboBox_2,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBox_2CurrentIndexChanged(int)));
    connect(ui.comboBox_3,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBox_3CurrentIndexChanged(int)));
    QFontDatabase FontDb;
    foreach(int size, FontDb.standardSizes())
    {
        ui.comboBox_AxisGrid_FontSize->addItem(QString::number(size));
    }
}

void PlotManager::initTextLightUI()
{
    ui.label_171->setVisible(false);
    ui.checkBox_30->setVisible(false);
    ui.label_172->setVisible(false);
    ui.checkBox_31->setVisible(false);
    connect(ui.lineEdit_23, &QLineEdit::editingFinished, this, [=]() {
        ui.spinBox_10->setValue(ui.lineEdit_23->text().toInt());
    });
    connect(ui.pushButton_gridColor, &QPushButton::clicked, this, [=]() {
        ui.pushButton_71->setColor(ui.pushButton_gridColor->color());
    });
    connect(ui.pushButton_gridFill, &QPushButton::clicked, this, [=]() {
        ui.pushButton_73->setColor(ui.pushButton_gridFill->color());
    });
    connect(ui.pushButton_68,
            SIGNAL(clicked()),
            this,
            SLOT(onAddNewClicked())); //这个是召唤addplotpair的按钮，懒得自己新建了，就直接用了
    connect(ui.pushButton_69, &QPushButton::clicked, this, &PlotManager::onPushButton_69Clicked);
    connect(ui.pushButton_66, &QPushButton::clicked, this, &PlotManager::onPushButton_66Clicked);
    connect(ui.pushButton_67, &QPushButton::clicked, this, &PlotManager::onPushButton_67Clicked);
    connect(ui.tableWidget_TextDataSort,
            &QTableWidget::itemSelectionChanged,
            this,
            &PlotManager::onTableWidget_textLightDataSortItemSelectionChanged);
    connect(ui.tableWidget_LightDataSort,
            &QTableWidget::itemSelectionChanged,
            this,
            &PlotManager::onTableWidget_textLightDataSortItemSelectionChanged);
    connect(ui.spinBox_10, &QSpinBox::editingFinished, this, &PlotManager::onSpinbox_10Changed);
    connect(ui.pushButton_71, &QPushButton::clicked, this, &PlotManager::onPushButton_71Clicked);
    connect(ui.pushButton_73, &QPushButton::clicked, this, &PlotManager::onPushButton_73Clicked);
}

void PlotManager::initScatterLimitUI()
{
    connect(ui.pushButtonLimitAdd, &QPushButton::clicked, this, &PlotManager::onAddScatterLimit);
    connect(
        ui.pushButtonLimitRemove, &QPushButton::clicked, this, &PlotManager::onRemoveScatterLimit);
    connect(
        ui.pushButtonLimitUpdate, &QPushButton::clicked, this, &PlotManager::onUpdateScatterLimit);
    connect(
        ui.treeWidgetLimit, &QTreeWidget::itemClicked, this, &PlotManager::onCurrentLimitChanged);
}

void PlotManager::initEditableMap()
{
    m_itemTextEditableMap.insert(PlotType::Type_PlotScatter,
                                 QList<QString>() << "Title"
                                                  << "X-Axis Description"
                                                  << "Y-Axis Description"
                                                  << "X-Axis Data"
                                                  << "Y-Axis Data");
    m_itemTextEditableMap.insert(PlotType::Type_PlotAScope, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotRTI, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotText, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotLight, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotBar, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotDial, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotAttitude, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotPolar, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotTrack, QList<QString>() << "Title");
    m_itemTextEditableMap.insert(PlotType::Type_PlotDoppler, QList<QString>() << "Title");
}

void PlotManager::showScatterEditableItem(PlotType type)
{
    ui.listWidget->clear();
    ui.listWidget->addItems(m_itemTextEditableMap.value(type));
}

void PlotManager::initPlotDataUI()
{
    connect(ui.pushButton_addNew, SIGNAL(clicked()), this, SLOT(onAddNewClicked()));
    connect(ui.tableWidget_plotData,
            &QTableWidget::itemSelectionChanged,
            this,
            &PlotManager::onTableWidget_plotDataItemSelectionChanged);
    connect(ui.lineEdit_24,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_24EditingFinished);
    connect(ui.pushButton_15, &QPushButton::clicked, this, &PlotManager::onPushButton_15Clicked);
    connect(ui.pushButton_16, &QPushButton::clicked, this, &PlotManager::onPushButton_16Clicked);
    connect(ui.pushButton_18, &QPushButton::clicked, this, &PlotManager::onPushButton_18Clicked);
    connect(ui.pushButton_19, &QPushButton::clicked, this, &PlotManager::onPushButton_19Clicked);
    connect(ui.pushButton_20, &QPushButton::clicked, this, &PlotManager::onPushButton_20Clicked);
    connect(ui.pushButton_21, &QPushButton::clicked, this, &PlotManager::onPushButton_21Clicked);
    connect(
        ui.checkBox_10, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_10StateChanged);
    connect(
        ui.checkBox_11, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_11StateChanged);
    connect(ui.toolButton, &QToolButton::clicked, this, &PlotManager::onToolButtonClicked);
    connect(ui.comboBox_6,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBox_6CurrentIndexChanged(int)));

    ui.pushButton_15->setEnabled(false);
    ui.pushButton_16->setEnabled(false);
    ui.pushButton_18->setEnabled(false);
    ui.pushButton_19->setEnabled(false);
}

void PlotManager::initTextEditUI()
{
    connect(
        ui.checkBox_12, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_12StateChanged);
    connect(
        ui.checkBox_13, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_13StateChanged);
    connect(
        ui.checkBox_14, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_14StateChanged);
    connect(ui.lineEdit_26,
            &QLineEdit::editingFinished,
            this,
            &PlotManager::onLineEdit_26EditingFinished);
	connect(ui.pushButton_22, &QPushButton::clicked, this, &PlotManager::onPushButton_22Clicked);
    connect(ui.pushButton_23, &QPushButton::clicked, this, &PlotManager::onPushButton_23Clicked);
    connect(ui.fontComboBox_2,
            &QFontComboBox::currentFontChanged,
            this,
            &PlotManager::onfontComboBox_2CurrentFontChanged);
    connect(
        ui.spinBox_FontSize, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_FontSizeChanged(int)));
    connect(ui.lineEdit_29, &QLineEdit::textChanged, this, &PlotManager::onOffsetValueChanged);
    connect(ui.lineEdit_30, &QLineEdit::textChanged, this, &PlotManager::onOffsetValueChanged);

    connect(ui.listWidget,
            &QListWidget::currentItemChanged,
            this,
            &PlotManager::onListWidgetItemChanged);
}

void PlotManager::initAttitudeUI()
{
    connect(ui.pushButton_80, &QPushButton::clicked, this, &PlotManager::onPushButton_80Clicked);
    connect(ui.pushButton_81, &QPushButton::clicked, this, &PlotManager::onPushButton_81Clicked);
    connect(ui.spinBox_29, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_29ValueChanged(int)));
    connect(ui.spinBox_30, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_30ValueChanged(int)));
    connect(ui.spinBox_31, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_31ValueChanged(int)));
}

void PlotManager::initGOGUI()
{
    // Filled有三种状态：未选中：不填充   半选中：根据GOG文件来  全选中：填充
    ui.checkBox_16->setTristate(true);
    connect(ui.pushButton_24, &QPushButton::clicked, this, &PlotManager::onPushButton_24Clicked);
    connect(ui.pushButton_25, &QPushButton::clicked, this, &PlotManager::onPushButton_25Clicked);
    connect(ui.pushButton_26, &QPushButton::clicked, this, &PlotManager::onPushButton_26Clicked);
    connect(ui.pushButton_27, &QPushButton::clicked, this, &PlotManager::onPushButton_27Clicked);
    connect(ui.pushButton_28, &QPushButton::clicked, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(ui.checkBox_15, &QCheckBox::stateChanged, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(ui.checkBox_16, &QCheckBox::stateChanged, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(ui.checkBox_17, &QCheckBox::stateChanged, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(ui.checkBox_25, &QCheckBox::stateChanged, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(
        ui.lineEdit_31, &QLineEdit::editingFinished, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(
        ui.lineEdit_32, &QLineEdit::editingFinished, this, &PlotManager::onUpdateGOGCustomSetting);
    connect(ui.fontComboBox,
            &QComboBox::currentTextChanged,
            this,
            &PlotManager::onUpdateGOGCustomSetting);

    connect(ui.listWidget_2, &QListWidget::itemClicked, this, &PlotManager::onListWidget_2Clicked);
}

void PlotManager::initDialUI()
{
    connect(ui.spinBox_21, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_21ValueChanged(int)));
    connect(ui.spinBox_22, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_22ValueChanged(int)));
    connect(ui.spinBox_23, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_23ValueChanged(int)));
    connect(ui.spinBox_24, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_24ValueChanged(int)));
    connect(ui.spinBox_25, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_25ValueChanged(int)));
    connect(ui.spinBox_26, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_26ValueChanged(int)));
    connect(ui.spinBox_27, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_27ValueChanged(int)));
    connect(ui.spinBox_28, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_28ValueChanged(int)));
    connect(ui.pushButton_74, &QPushButton::clicked, this, &PlotManager::onPushButton_74Clicked);
    connect(ui.pushButton_75, &QPushButton::clicked, this, &PlotManager::onPushButton_75Clicked);
    connect(ui.pushButton_76, &QPushButton::clicked, this, &PlotManager::onPushButton_76Clicked);
    connect(ui.pushButton_77, &QPushButton::clicked, this, &PlotManager::onPushButton_77Clicked);
    connect(ui.comboBox_20,
            &QComboBox::currentTextChanged,
            this,
            &PlotManager::onComboBox_dialStyleCurrentTextChanged);
    connect(
        ui.checkBox_34, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_34StateChanged);
    connect(
        ui.checkBox_35, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_35StateChanged);
    connect(
        ui.checkBox_36, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_36StateChanged);
    connect(
        ui.checkBox_37, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_37StateChanged);
}

void PlotManager::refreshTreeWidgetSettingEnabled(PlotItemBase* plot)
{
    ui.treeWidget_settings->setEnabled(true);
    PlotType type = plot->plotType();
    showScatterEditableItem(type);

    if(type == PlotType::Type_PlotScatter)
    {
        enableItem_Scatter();
        //plotPair界面
        refreshPlotDataUI(m_curSelectPlot);
        refreshScatterLimitUI(m_curSelectPlot);
    }
    else if(type == PlotType::Type_PlotAScope)
    {
        enableItem_AScope();
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        enableItem_RTI();
    }
    else if(type == PlotType::Type_PlotText)
    {
        enableItem_Text_Light();
        //Text&Light界面
        refreshLightTextUI(m_curSelectPlot);
    }
    else if(type == PlotType::Type_PlotLight)
    {
        enableItem_Text_Light();
        //Text&Light界面
        refreshLightTextUI(m_curSelectPlot);
    }
    else if(type == PlotType::Type_PlotBar)
    {
        enableItem_Bar();
    }
    else if(type == PlotType::Type_PlotDial)
    {
        enableItem_Dial();
        refreshDialUI(m_curSelectPlot);
    }
    else if(type == PlotType::Type_PlotAttitude)
    {
        enableItem_Attitude();
        refreshAttitudeUI(m_curSelectPlot);
    }
    else if(type == PlotType::Type_PlotPolar)
    {
        enableItem_Polar();
    }
    else if(type == PlotType::Type_PlotTrack)
    {
        enableItem_Track();
    }
    else if(type == PlotType::Type_PlotDoppler)
    {
        enableItem_Doppler();
    }
}

void PlotManager::refreshGeneralUI(PlotItemBase* plot)
{
    // 从主界面获取TabRect
    emit sigGetTabRect();

    ui.lineEdit_plotName->setText(plot->getName());
    ui.comboBox_tabName->setCurrentText(plot->getTabName());
    ui.pushButton_outerFillColor->setColor(plot->getOuterFillColor());
    ui.pushButton_outlineColor->setColor(plot->getOutlineColor());
    if(ui.radioButton_pixel->isChecked())
    {
        ui.lineEdit_plotPositionX->setText(QString("%1").arg(plot->currPosition().x()));
        ui.lineEdit_plotPositionY->setText(QString("%1").arg(plot->currPosition().y()));
        ui.lineEdit_plotWidth->setText(QString("%1").arg(plot->currWidth()));
        ui.lineEdit_plotHeight->setText(QString("%1").arg(plot->currHeight()));
    }
    else if(ui.radioButton_percent->isChecked())
    {
        ui.lineEdit_plotPositionX->setText(
            QString("%1").arg((float)plot->currPosition().x() / m_tabWidgetRect.width()));
        ui.lineEdit_plotPositionY->setText(
            QString("%1").arg((float)plot->currPosition().y() / m_tabWidgetRect.height()));
        ui.lineEdit_plotWidth->setText(
            QString("%1").arg((float)plot->currWidth() / m_tabWidgetRect.width()));
        ui.lineEdit_plotHeight->setText(
            QString("%1").arg((float)plot->currHeight() / m_tabWidgetRect.height()));
    }
}

void PlotManager::refreshAxisGridUI(PlotItemBase* plot)
{
	double x0, x1, y0, y1;
	plot->getCoordRangeX(x0, x1);
	plot->getCoordRangeY(y0, y1);
	ui.lineEdit_limitBgnX->setText(QString("%1").arg(x0));
	ui.lineEdit_limitEndX->setText(QString("%1").arg(x1));
	ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y0));
	ui.lineEdit_limitEndY->setText(QString("%1").arg(y1));
	ui.lineEdit_hrozGrids->setText(QString("%1").arg(plot->getHorzGrids()));
	ui.lineEdit_vertGrids->setText(QString("%1").arg(plot->getVertGrids()));
	ui.lineEdit_21->setText(QString("%1").arg(plot->getAxisWidth()));
	ui.lineEdit_23->setText(QString("%1").arg(plot->getGridWidth()));
	ui.pushButton_axisColor->setColor(plot->getAxisColor());
	ui.pushButton_gridColor->setColor(plot->getGridColor());
	ui.pushButton_gridFill->setColor(plot->getGridFillColor());
	ui.checkBox_6->setChecked(plot->getGridVisible());
    ui.pushButton_10->setColor(plot->getxTickLabelColor());
    ui.fontComboBox_3->setCurrentFont(plot->getxTickLabelFont());
    ui.comboBox_AxisGrid_FontSize->setCurrentText(QString("%1").arg(plot->getxTickLabelFontSize()));
    ui.comboBox_2->setCurrentIndex(int(plot->getGridStyle()) - 1);
    switch(plot->getGridDensity())
    {
    case GridDensity::LESS:
        ui.comboBox_3->setCurrentIndex(0);
        break;
    case GridDensity::NORMAL:
        ui.comboBox_3->setCurrentIndex(1);
        break;
    case GridDensity::MORE:
        ui.comboBox_3->setCurrentIndex(2);
        break;
    }
}

void PlotManager::refreshPlotDataUI(PlotItemBase* plot)
{
    QVector<DataPair*> dataPairs = plot->getDataPairs();
    ui.tableWidget_plotData->setRowCount(dataPairs.size());

    for(int row = 0; row < dataPairs.size(); ++row)
    {
        //界面更新
        auto dataPair = dataPairs.at(row);
        QTableWidgetItem* addplot1 = new QTableWidgetItem(dataPair->getXEntityAttrPair());
        QTableWidgetItem* addplot2 = new QTableWidgetItem(dataPair->getYEntityAttrPair());

        ui.tableWidget_plotData->setItem(row, 0, addplot1);
        ui.tableWidget_plotData->setItem(row, 1, addplot2);
    }
}

void PlotManager::refreshLightTextUI(PlotItemBase* plot)
{
    if(!(plot == nullptr))
    {
        auto dataPairs = plot->getDataPairs();
        if(plot->getName().startsWith("Text"))
        {
            ui.stackedWidget_LightTextDataSort->setCurrentIndex(0);
            ui.groupBox_29->setVisible(false);
            ui.tableWidget_TextDataSort->clearContents();
            auto size = plot->getDataPairs().size();
            ui.tableWidget_TextDataSort->setRowCount(size);
            for(int i = 0; i < size; i++)
            {
                auto dataPair = dataPairs.at(i);

                QString temEntityString = dataPair->getEntity_x();
                QString temAttriString = dataPair->getAttr_x();
                QTableWidgetItem* temEntity = new QTableWidgetItem(temEntityString);
                QTableWidgetItem* temAttri = new QTableWidgetItem(temAttriString);

                ui.tableWidget_TextDataSort->setItem(i, 0, temEntity);
                ui.tableWidget_TextDataSort->setItem(i, 1, temAttri);
            }
        }
        else
        {
            ui.stackedWidget_LightTextDataSort->setCurrentIndex(1);
            ui.groupBox_29->setVisible(true);
            ui.tableWidget_LightDataSort->clearContents();
            auto size = plot->getDataPairs().size();
            ui.tableWidget_LightDataSort->setRowCount(size);
            for(int i = 0; i < size; i++)
            {
                QString temFirst = dataPairs[i]->getXEntityAttrPair();
                QTableWidgetItem* temEntityAndAttri = new QTableWidgetItem(temFirst);
                ui.tableWidget_LightDataSort->setItem(i, 0, temEntityAndAttri);
            }
        }
    }
    else
        return;
}

void PlotManager::refreshGOGUI(PlotItemBase* plot)
{
    QList<QString> fileList = plot->getGOGFileList();
    ui.listWidget_2->clear();
    for(auto fileName : fileList)
    {
        ui.listWidget_2->addItem(fileName);
    }
}

void PlotManager::refreshTextEditUI(PlotItemBase* plot)
{
	ui.checkBox_12->setChecked(plot->unitsShowX());
	ui.checkBox_13->setChecked(plot->unitsShowY());
	ui.checkBox_14->setChecked(plot->getTitleVisible());
	ui.lineEdit_26->setText(plot->getTitle());
	ui.pushButton_22->setColor(plot->getTitleColor());
	ui.pushButton_23->setColor(plot->getTitleFillColor());
	ui.fontComboBox_2->setCurrentFont(plot->getTitleFont());
    ui.spinBox_FontSize->setValue(plot->getTitleFontSize());
}

void PlotManager::refreshAttitudeUI(PlotItemBase* plot)
{
    auto item = dynamic_cast<PlotAttitude*>(plot);
    if(item == nullptr)
        return;

    ui.pushButton_80->setColor(item->getRollColor());
    ui.pushButton_81->setColor(item->getPitchColor());
    ui.spinBox_29->setValue(item->getTickRadiusPercentage());
    ui.spinBox_30->setValue(item->getTextPercentage());
    ui.spinBox_31->setValue(item->getDialPercentage());
}

void PlotManager::refreshDialUI(PlotItemBase* plot)
{
    auto item = dynamic_cast<PlotDial*>(plot);
    if(item == nullptr)
        return;
    ui.spinBox_21->setValue(item->getTickRadiusRate());
    ui.spinBox_22->setValue(item->getColorRate());
    ui.spinBox_23->setValue(item->getTextRate());
    ui.spinBox_24->setValue(item->getDialRate());
    ui.spinBox_25->setValue(item->getDialCapRate());
    ui.spinBox_26->setValue(item->getStartAngle());
    ui.spinBox_27->setValue(item->getEndAngle());
    ui.pushButton_74->setColor(item->getCapColor());
    ui.checkBox_34->setChecked(item->getDrawFirstTick());
    ui.checkBox_35->setChecked(item->getDrawLastTick());
    ui.checkBox_36->setChecked(item->getDrawFirstTextLabel());
    ui.checkBox_37->setChecked(item->getDrawLastTextLabel());
    ui.comboBox_20->setCurrentText(item->getDialStyle());
}

void PlotManager::refreshScatterLimitUI(PlotItemBase* plot)
{
    if(auto scatter = dynamic_cast<PlotScatter*>(plot))
    {
        auto segMap = scatter->getBkgLimitSegMap();
        ui.treeWidgetLimit->clear();
        for(const auto& seg : segMap)
        {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, seg.m_limitName);
            item->setText(1, QString::number(seg.m_limitValue));
            //左侧功能未实现，暂时这里写一个固定的>
            item->setText(2, ">");
            ui.treeWidgetLimit->addTopLevelItem(item);
        }
    }
}

void PlotManager::enableItem_Scatter()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(false);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(false);
    m_itemScatterPlot->setDisabled(false);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_AScope()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(false);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(false);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_RTI()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(false);
    m_itemPlotData->setDisabled(true);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(false);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Text_Light()
{
    m_itemLinkedAxis->setDisabled(true);
    m_itemAxis->setDisabled(true);
    m_itemGOG->setDisabled(true);
    m_itemPlotData->setDisabled(false);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(false);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Bar()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(false);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(false);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Dial()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(true);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(false);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Attitude()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(true);
    m_itemPlotData->setDisabled(true);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(false);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Polar()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(true);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(false);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Track()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(true);
    m_itemPlotData->setDisabled(false);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(false);
    m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Doppler()
{
    m_itemAxis->setDisabled(false);
    m_itemLinkedAxis->setDisabled(false);
    m_itemPlotData->setDisabled(true);
    m_itemGOG->setDisabled(true);
    m_itemScatterPlot->setDisabled(true);
    m_itemAScope->setDisabled(true);
    m_itemRTI->setDisabled(true);
    m_itemTextLight->setDisabled(true);
    m_itemBar->setDisabled(true);
    m_itemDial->setDisabled(true);
    m_itemAttitude->setDisabled(true);
    m_itemTrackStatus->setDisabled(true);
    m_itemRangeDoppler->setDisabled(false);
}

void PlotManager::onTWSPclicked(QTreeWidgetItem* item, int column)
{
    QTreeWidgetItem* parent = item->parent();
    if(!parent)
        return;

    QString tabName = parent->text(0);
    QString plotName = item->text(column);
    m_curSelectPlot = PlotManagerData::getInstance()->getPlotByTabAndName(tabName, plotName);
    if(m_curSelectPlot)
    {
        //刷新treeWidgetSetting的使能状态
        refreshTreeWidgetSettingEnabled(m_curSelectPlot);
        //general界面
        refreshGeneralUI(m_curSelectPlot);
        //Axis&Grid界面
        refreshAxisGridUI(m_curSelectPlot);
        //Text Edit
        refreshTextEditUI(m_curSelectPlot);
        //gog界面
        refreshGOGUI(m_curSelectPlot);
    }
}

void PlotManager::onAddNewClicked()
{
    /*AddPlotPair* addPlotPair = new AddPlotPair();
    addPlotPair->show();*/

    emit sigAddPlotPair();
}

void PlotManager::onSelectedPlot(PlotItemBase* pBasePlot)
{
    if(pBasePlot == nullptr)
        return;

    QList<QTreeWidgetItem*> items = ui.treeWidget_selectedPlots->findItems(
        pBasePlot->getName(), Qt::MatchExactly | Qt::MatchRecursive);
    if(items.size() != 0)
    {
        foreach(QTreeWidgetItem* item, items)
        {
            if(item->parent() != nullptr && item->parent()->text(0) == pBasePlot->getTabName())
            {
                QTreeWidgetItemIterator it(ui.treeWidget_selectedPlots);
                while(*it)
                {
                    (*it)->setSelected(false);
                    ++it;
                }
                ui.treeWidget_selectedPlots->itemClicked(item, 0);
                item->setSelected(true);
                break;
            }
        }
    }
}

void PlotManager::onUpdatePlotManager(const QMap<QString, QList<PlotItemBase*>>& plotDataMap)
{
    if(plotDataMap.isEmpty())
    {
        return;
    }
    ui.comboBox_tabName->clear();
    ui.treeWidget_selectedPlots->clear();

    for(int i = 0; i < plotDataMap.size(); ++i)
    {
        QString tabName = plotDataMap.keys().at(i);
        QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabName);
        ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
        ui.treeWidget_selectedPlots->expandAll();
        for(int k = 0; k < plotDataMap[tabName].size(); ++k)
        {
            QTreeWidgetItem* itemselPlotI =
                new QTreeWidgetItem(QStringList() << plotDataMap[tabName].at(k)->getName());
            itemselPlotH->addChild(itemselPlotI);
        }
        //comboBox_tabName
        ui.comboBox_tabName->addItem(tabName);
    }
}

void PlotManager::onTWSclicked(QTreeWidgetItem* item, int column)
{

    if(item->isDisabled())
        return;
    else
    {
        QString compare;
        compare = item->text(column);
        if(compare == QString("通用设置"))
        {
            ui.stackedWidget->setCurrentIndex(0);
        }
        else if(compare == QString("坐标轴和网格设置"))
        {
            ui.stackedWidget->setCurrentIndex(1);
        }
        else if(compare == QString("数据设置"))
        {
            ui.stackedWidget->setCurrentIndex(2);
        }
        else if(compare == QString("文本信息"))
        {
            ui.stackedWidget->setCurrentIndex(3);
        }
        else if(compare == QString("链接轴"))
        {
            ui.stackedWidget->setCurrentIndex(5);
        }
        else if(compare == QString("Scatter设置"))
        {
            ui.stackedWidget->setCurrentIndex(4);
        }
        else if(compare == QString("GOG曲线"))
        {
            ui.stackedWidget->setCurrentIndex(4);
        }
        else if(compare == QString("限制"))
        {
            ui.stackedWidget->setCurrentIndex(6);
        }
        else if(compare == QString("标记"))
        {
            ui.stackedWidget->setCurrentIndex(7);
        }
        else if(compare == QString("Time Line"))
        {
            ui.stackedWidget->setCurrentIndex(8);
        }
        else if(compare == QString("Hands-Off"))
        {
            ui.stackedWidget->setCurrentIndex(9);
        }
        else if(compare == QString("A-Scope设置"))
        {
            ui.stackedWidget->setCurrentIndex(10);
        }
        else if(compare == QString("RTI设置"))
        {
            ui.stackedWidget->setCurrentIndex(11);
        }
        else if(compare == QString("Text/Light设置"))
        {
            ui.stackedWidget->setCurrentIndex(12);
        }
        else if(compare == QString("Bar设置"))
        {
            ui.stackedWidget->setCurrentIndex(13);
        }
        else if(compare == QString("Dials设置"))
        {
            ui.stackedWidget->setCurrentIndex(14);
        }
        else if(compare == QString("Attitude设置"))
        {
            ui.stackedWidget->setCurrentIndex(15);
        }
        else if(compare == QString("Track Status设置"))
        {
            ui.stackedWidget->setCurrentIndex(16);
        }
        else if(compare == QString("Range Doppler设置"))
        {
            ui.stackedWidget->setCurrentIndex(17);
        }
    }
}

void PlotManager::spinboxBetweenChanged()
{
    m_spinBoxBetween = ui.spinBox_between->value();
    m_curSelectPlot->setBarBetweenPadding(m_spinBoxBetween);
}

void PlotManager::spinboxLeftChanged()
{
    m_spinBoxLeft = ui.spinBox_left->value();
    m_curSelectPlot->setBarLeftPadding(m_spinBoxLeft);
}

void PlotManager::spinboxRightChanged()
{
    m_spinBoxRight = ui.spinBox_right->value();
    m_curSelectPlot->setBarRightPadding(m_spinBoxRight);
}

void PlotManager::onRadioPixelClicked()
{
    ui.label_xPos->setText("pixel");
    ui.label_yPos->setText("pixel");
    ui.label_widthPos->setText("pixel");
    ui.label_heightPos->setText("pixel");

    if(ui.lineEdit_plotPositionX->text() == nullptr ||
       ui.lineEdit_plotPositionY->text() == nullptr || ui.lineEdit_plotWidth->text() == nullptr ||
       ui.lineEdit_plotHeight->text() == nullptr)
    {
        return;
    }

    float percent = ui.lineEdit_plotPositionX->text().toFloat();
    int pixel = (int)(percent * m_tabWidgetRect.width());
    ui.lineEdit_plotPositionX->setText(QString("%1").arg(pixel));

    percent = ui.lineEdit_plotPositionY->text().toFloat();
    pixel = (int)(percent * m_tabWidgetRect.height());
    ui.lineEdit_plotPositionY->setText(QString("%1").arg(pixel));

    percent = ui.lineEdit_plotWidth->text().toFloat();
    pixel = (int)(percent * m_tabWidgetRect.width());
    ui.lineEdit_plotWidth->setText(QString("%1").arg(pixel));

    percent = ui.lineEdit_plotHeight->text().toFloat();
    pixel = (int)(percent * m_tabWidgetRect.height());
    ui.lineEdit_plotHeight->setText(QString("%1").arg(pixel));
}

void PlotManager::onRadioPercentClicked()
{
    ui.label_xPos->setText("%");
    ui.label_yPos->setText("%");
    ui.label_widthPos->setText("%");
    ui.label_heightPos->setText("%");

    if(ui.lineEdit_plotPositionX->text() == nullptr ||
       ui.lineEdit_plotPositionY->text() == nullptr || ui.lineEdit_plotWidth->text() == nullptr ||
       ui.lineEdit_plotHeight->text() == nullptr)
    {
        return;
    }

    int pixel = ui.lineEdit_plotPositionX->text().toInt();
    float percent = (float)pixel / m_tabWidgetRect.width();
    ui.lineEdit_plotPositionX->setText(QString("%1").arg(percent));

    pixel = ui.lineEdit_plotPositionY->text().toInt();
    percent = (float)pixel / m_tabWidgetRect.height();
    ui.lineEdit_plotPositionY->setText(QString("%1").arg(percent));

    pixel = ui.lineEdit_plotWidth->text().toInt();
    percent = (float)pixel / m_tabWidgetRect.width();
    ui.lineEdit_plotWidth->setText(QString("%1").arg(percent));

    pixel = ui.lineEdit_plotHeight->text().toInt();
    percent = (float)pixel / m_tabWidgetRect.height();
    ui.lineEdit_plotHeight->setText(QString("%1").arg(percent));
}

void PlotManager::onGetTabWidgetRect(QRect rect)
{
    m_tabWidgetRect = rect;
}

void PlotManager::onBtnCloseClicked()
{
    close();
}

void PlotManager::onBarHorizonChanged(int state)
{
    if(state == 2)
        m_curSelectPlot->setIsHorizonBar(true);
    else if(state == 0)
        m_curSelectPlot->setIsHorizonBar(false);
}

void PlotManager::onPushButton_71Clicked()
{
    ui.pushButton_gridColor->setColor(ui.pushButton_71->color());
    ui.lineEdit_23->setText(QString("%1").arg(ui.spinBox_10->value()));
    m_curSelectPlot->setGridColorWidth(ui.pushButton_71->color(), ui.spinBox_10->value());
}

void PlotManager::onSpinbox_10Changed()
{
    ui.pushButton_gridColor->setColor(ui.pushButton_71->color());
    ui.lineEdit_23->setText(QString("%1").arg(ui.spinBox_10->value()));
    m_curSelectPlot->setGridColorWidth(ui.pushButton_71->color(), ui.spinBox_10->value());
}

void PlotManager::onPushButton_73Clicked()
{
    QColor color = ui.pushButton_73->color();
    if(!(color == ui.pushButton_gridFill->color()))
    {
        m_curSelectPlot->setGridFillColor(color);
    }
    ui.pushButton_gridFill->setColor(color);
}

//void PlotManager::onPushButton_gridFillClicked()
//{
//	QColor color = ui.pushButton_gridFill->color();
//	m_curSelectPlot->setGridFillColor(color);
//}

void PlotManager::onPlotRectEditFinished()
{
    if(ui.lineEdit_plotPositionX->text() == nullptr ||
       ui.lineEdit_plotPositionY->text() == nullptr || ui.lineEdit_plotWidth->text() == nullptr ||
       ui.lineEdit_plotHeight->text() == nullptr)
    {
        return;
    }

    bool bX, bY, bW, bH;
    int x, y, w, h;
    if(ui.radioButton_pixel->isChecked())
    {
        x = ui.lineEdit_plotPositionX->text().toInt(&bX);
        y = ui.lineEdit_plotPositionY->text().toInt(&bY);
        w = ui.lineEdit_plotWidth->text().toInt(&bW);
        h = ui.lineEdit_plotHeight->text().toInt(&bH);
    }
    else
    {
        x = (int)(ui.lineEdit_plotPositionX->text().toFloat(&bX) * m_tabWidgetRect.width());
        y = (int)(ui.lineEdit_plotPositionY->text().toFloat(&bY) * m_tabWidgetRect.height());
        w = (int)(ui.lineEdit_plotWidth->text().toFloat(&bW) * m_tabWidgetRect.width());
        h = (int)(ui.lineEdit_plotHeight->text().toFloat(&bH) * m_tabWidgetRect.height());
    }

    if(bX && bY && bW && bH && (m_curSelectPlot != nullptr))
    {
        //m_curSelectPlot->setRect(QRect(x, y, w, h));
        connect(this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
        emit sigRectChanged(QRect(x, y, w, h));
        disconnect(
            this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
    }
}

void PlotManager::onLineEditPlotNameEditingFinished()
{
    if(m_curSelectPlot != nullptr)
    {
        QString oldName = m_curSelectPlot->getName();
        QString newName = ui.lineEdit_plotName->text();
        if(newName.compare(oldName) != 0)
        {
            m_curSelectPlot->setName(newName);
            emit sigChangePlotName();
        }
    }
}

void PlotManager::onCheckBox_drawStateChanged()
{
    if(m_curSelectPlot != nullptr)
    {
        connect(
            this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
        emit sigSetPlotVisible(ui.checkBox_draw->isChecked());
        disconnect(
            this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
    }
}

void PlotManager::onPushButton_outerFillColorClicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setOuterFillColor(ui.pushButton_outerFillColor->color());
}

void PlotManager::onPushButton_outlineColorClicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setOutlineColor(ui.pushButton_outlineColor->color());
}

void PlotManager::onMouseEventDone()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    emit sigGetTabRect();

    if(ui.radioButton_pixel->isChecked())
    {
        ui.lineEdit_plotPositionX->setText(QString("%1").arg(m_curSelectPlot->currPosition().x()));
        ui.lineEdit_plotPositionY->setText(QString("%1").arg(m_curSelectPlot->currPosition().y()));
        ui.lineEdit_plotWidth->setText(QString("%1").arg(m_curSelectPlot->currWidth()));
        ui.lineEdit_plotHeight->setText(QString("%1").arg(m_curSelectPlot->currHeight()));
    }
    else if(ui.radioButton_percent->isChecked())
    {
        ui.lineEdit_plotPositionX->setText(QString("%1").arg(
            (float)m_curSelectPlot->currPosition().x() / m_tabWidgetRect.width()));
        ui.lineEdit_plotPositionY->setText(QString("%1").arg(
            (float)m_curSelectPlot->currPosition().y() / m_tabWidgetRect.height()));
        ui.lineEdit_plotWidth->setText(
            QString("%1").arg((float)m_curSelectPlot->currWidth() / m_tabWidgetRect.width()));
        ui.lineEdit_plotHeight->setText(
            QString("%1").arg((float)m_curSelectPlot->currHeight() / m_tabWidgetRect.height()));
    }
}

void PlotManager::onCheckBox_4StateChanged()
{
    ui.lineEdit_10->setEnabled(ui.checkBox_4->isChecked());
    ui.lineEdit_11->setEnabled(ui.checkBox_4->isChecked());
    ui.tableWidget->setEnabled(ui.checkBox_4->isChecked());
}

void PlotManager::onCheckBox_5StateChanged()
{
    ui.lineEdit_12->setEnabled(ui.checkBox_5->isChecked());
    ui.lineEdit_13->setEnabled(ui.checkBox_5->isChecked());
    ui.tableWidget_2->setEnabled(ui.checkBox_5->isChecked());
}

void PlotManager::onLineEdit_limitXEditingFinished()
{
    if(m_curSelectPlot == nullptr)
        return;

    bool bx0, bx1;
    double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
    double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
    if(bx0 && bx1)
        m_curSelectPlot->setCoordRangeX(x0, x1);
}

void PlotManager::onLineEdit_limitYEditingFinished()
{
    if(m_curSelectPlot == nullptr)
        return;

    bool by0, by1;
    double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
    double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
    if(by0 && by1)
        m_curSelectPlot->setCoordRangeY(y0, y1);
}

void PlotManager::onPushButton_flipXValuesClicked()
{
    if(m_curSelectPlot == nullptr)
        return;

    bool bx0, bx1;
    double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
    double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
    if(bx0 && bx1)
    {
        m_curSelectPlot->setCoordRangeX(x1, x0);
        ui.lineEdit_limitBgnX->setText(QString("%1").arg(x1));
        ui.lineEdit_limitEndX->setText(QString("%1").arg(x0));
    }
}

void PlotManager::onPushButton_flipYValuesClicked()
{
    if(m_curSelectPlot == nullptr)
        return;

    bool by0, by1;
    double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
    double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
    if(by0 && by1)
    {
        m_curSelectPlot->setCoordRangeX(y1, y0);
        ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y1));
        ui.lineEdit_limitEndY->setText(QString("%1").arg(y0));
    }
}

void PlotManager::onLineEdit_horzGridsEditingFinished()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }
    bool on;
    uint count = ui.lineEdit_hrozGrids->text().toUInt(&on);
    if(!on)
    {
        return;
    }

    m_curSelectPlot->setHorzGrids(count);
}

void PlotManager::onLineEdit_vertGridsEditingFinished()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }
    bool on;
    uint count = ui.lineEdit_vertGrids->text().toUInt(&on);
    if(!on)
    {
        return;
    }

    m_curSelectPlot->setVertGrids(count);
}

void PlotManager::onSetAxisColorWidth()
{
    bool on;
    uint width = ui.lineEdit_21->text().toInt(&on);
    QColor color = ui.pushButton_axisColor->color();
    if(on)
        m_curSelectPlot->setAxisColorWidth(color, width);
}

void PlotManager::onSetGridColorWidth()
{
    bool on;
    uint width = ui.lineEdit_23->text().toInt(&on);
    QColor color = ui.pushButton_gridColor->color();
    if(on)
        m_curSelectPlot->setGridColorWidth(color, width);
}

void PlotManager::onPushButton_gridFillClicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setGridFillColor(ui.pushButton_gridFill->color());
}

void PlotManager::onCheckBox_6StateChanged()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setGridVisible(ui.checkBox_6->isChecked());
}

void PlotManager::onPushButton_10Clicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setxTickLabelColor(ui.pushButton_10->color());
}

void PlotManager::onfontComboBox_3CurrentFontChanged(const QFont& font)
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }
    int fontSize = ui.comboBox_AxisGrid_FontSize->currentText().toInt();
    QFont newFont;
    newFont.setFamily(font.family());
    newFont.setPixelSize(fontSize);
    m_curSelectPlot->setxTickLabelFont(newFont);
}

void PlotManager::onComboBox_AxisGrid_FontSizeCurrentTextChanged(const QString& text)
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setxTickLabelFontSize(text.toInt());
}

void PlotManager::onComboBox_2CurrentIndexChanged(int index)
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }
    m_curSelectPlot->setGridStyle(GridStyle(index));
}

void PlotManager::onComboBox_3CurrentIndexChanged(int index)
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    GridDensity density;
    switch(index)
    {
    case 0:
        density = GridDensity::LESS;
        break;
    case 1:
        density = GridDensity::NORMAL;
        break;
    case 2:
        density = GridDensity::MORE;
        break;
    }
    m_curSelectPlot->setGridDensity(density);
}

void PlotManager::onPushButton_24Clicked()
{
    // 去掉reload时加载asi中gog文件的逻辑
    return;
    // 重新加载asi文件中关联的gog文件
    //    QList<QString> list = DataManager::getInstance()->getGOGFileList();
    //    for(auto item : list)
    //    {
    //        addGOGTableItem(item);
    //    }
}

void PlotManager::onPushButton_25Clicked()
{
    QString fileName =
        QFileDialog::getOpenFileName(this, tr("打开GOG文件"), "", "GOG Files(*.gog)");
    if(!fileName.isEmpty())
    {
        addGOGTableItem(fileName);
    }
}

void PlotManager::onPushButton_26Clicked()
{
    QString fileName = ui.listWidget_2->currentItem()->data(Qt::DisplayRole).toString();
    m_curSelectPlot->removeGOGFile(fileName);
    ui.listWidget_2->takeItem(ui.listWidget_2->currentRow());

    QMap<QString, GOGCustomSetting> settings = m_curSelectPlot->getGogCustomSettings();
    if(settings.contains(fileName))
    {
        settings.remove(fileName);
        m_curSelectPlot->setGogCustomSettings(settings);
    }
}

void PlotManager::onPushButton_27Clicked() {}

void PlotManager::onListWidget_2Clicked()
{
    QString fileName = ui.listWidget_2->currentItem()->data(Qt::DisplayRole).toString();
    QMap<QString, GOGCustomSetting> gogSetting = m_curSelectPlot->getGogCustomSettings();
    if(gogSetting.contains(fileName))
    {
        GOGCustomSetting setting = gogSetting.value(fileName);
        ui.checkBox_15->setChecked(setting.isDraw);
        ui.pushButton_28->setColor(setting.fillColor);
        ui.checkBox_16->setCheckState(setting.fillState);
        ui.lineEdit_31->setText(QString::number(setting.lineWidth));
        ui.lineEdit_32->setText(QString::number(setting.pointSize));
        ui.fontComboBox->setCurrentFont(setting.font);
        ui.comboBox_25->setCurrentText(QString::number(setting.font.pixelSize()));
        ui.checkBox_17->setChecked(setting.isDrawText);
    }
}

void PlotManager::addGOGTableItem(const QString& fileName)
{
    bool duplicate = false;
    for(int i = 0; i < ui.listWidget_2->count(); i++)
    {
        duplicate = false;
        if(fileName == ui.listWidget_2->item(i)->data(Qt::DisplayRole).toString())
        {
            duplicate = true;
            break;
        }
    }
    if(!duplicate)
    {
        ui.listWidget_2->addItem(fileName);
        m_curSelectPlot->addGOGFile(fileName);

        QMap<QString, GOGCustomSetting> settings = m_curSelectPlot->getGogCustomSettings();
        settings.insert(fileName, GOGCustomSetting());
        m_curSelectPlot->setGogCustomSettings(settings);
    }
}

void PlotManager::onUpdateGOGCustomSetting()
{
    if(!ui.listWidget_2->currentItem())
        return;
    QString fileName = ui.listWidget_2->currentItem()->data(Qt::DisplayRole).toString();
    QMap<QString, GOGCustomSetting> allSetting = m_curSelectPlot->getGogCustomSettings();
    GOGCustomSetting setting;
    if(allSetting.contains(fileName))
    {
        setting = allSetting.value(fileName);
        setting.isDraw = ui.checkBox_15->isChecked();
        setting.fillState = ui.checkBox_16->checkState();
        setting.isDrawText = ui.checkBox_17->isChecked();
        setting.fillColor = ui.pushButton_28->color();
        setting.lineWidth = ui.lineEdit_31->text().toInt();
        setting.pointSize = ui.lineEdit_32->text().toInt();
        setting.font = ui.fontComboBox->currentFont();
        setting.font.setPixelSize(ui.comboBox_25->currentText().toInt() == 0
                                      ? 14
                                      : ui.comboBox_25->currentText().toInt());
    }
    allSetting.insert(fileName, setting);
    m_curSelectPlot->setGogCustomSettings(allSetting);
}

void PlotManager::onTableWidget_textLightDataSortItemSelectionChanged()
{
    if(m_curSelectPlot->getName().startsWith("Text"))
    {
        //设置Move Up/Move Down的enable
        if(ui.tableWidget_TextDataSort->currentRow() < 0)
            return;
        int row = ui.tableWidget_TextDataSort->rowCount();
        ui.pushButton_66->setEnabled(false);
        ui.pushButton_67->setEnabled(false);

        if(row <= 1)
        {
            ui.pushButton_66->setEnabled(false);
            ui.pushButton_67->setEnabled(false);
        }
        else if(ui.tableWidget_TextDataSort->currentRow() == 0)
        {
            ui.pushButton_66->setEnabled(false);
            ui.pushButton_67->setEnabled(true);
        }
        else if(ui.tableWidget_TextDataSort->currentRow() == row - 1)
        {
            ui.pushButton_66->setEnabled(true);
            ui.pushButton_67->setEnabled(false);
        }
        else
        {
            ui.pushButton_66->setEnabled(true);
            ui.pushButton_67->setEnabled(true);
        }
    }
    else
    {
        if(ui.tableWidget_LightDataSort->currentRow() < 0)
            return;
        int row = ui.tableWidget_LightDataSort->rowCount();
        ui.pushButton_66->setEnabled(false);
        ui.pushButton_67->setEnabled(false);

        if(row <= 1)
        {
            ui.pushButton_66->setEnabled(false);
            ui.pushButton_67->setEnabled(false);
        }
        else if(ui.tableWidget_LightDataSort->currentRow() == 0)
        {
            ui.pushButton_66->setEnabled(false);
            ui.pushButton_67->setEnabled(true);
        }
        else if(ui.tableWidget_LightDataSort->currentRow() == row - 1)
        {
            ui.pushButton_66->setEnabled(true);
            ui.pushButton_67->setEnabled(false);
        }
        else
        {
            ui.pushButton_66->setEnabled(true);
            ui.pushButton_67->setEnabled(true);
        }
    }
}

void PlotManager::onTableWidget_plotDataItemSelectionChanged()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;

    ui.pushButton_18->setEnabled(true);
    ui.pushButton_19->setEnabled(true);

    //判断选中行数，使能move按钮
    int rowCount = ui.tableWidget_plotData->rowCount();
    if(rowCount <= 1)
    {
        ui.pushButton_15->setEnabled(false);
        ui.pushButton_16->setEnabled(false);
    }
    else if(row == 0)
    {
        ui.pushButton_15->setEnabled(false);
        ui.pushButton_16->setEnabled(true);
    }
    else if((row + 1) == rowCount)
    {
        ui.pushButton_15->setEnabled(true);
        ui.pushButton_16->setEnabled(false);
    }
    else
    {
        ui.pushButton_15->setEnabled(true);
        ui.pushButton_16->setEnabled(true);
    }

    if(m_curSelectPlot == nullptr)
        return;

    auto dataPair = m_curSelectPlot->getDataPairs().at(row);
    ui.lineEdit_24->setText(QString("%1").arg(dataPair->width()));
    ui.pushButton_21->setColor(dataPair->dataColor());
    ui.checkBox_11->setChecked(dataPair->isDraw());
    ui.checkBox_10->setChecked(dataPair->isLineMode());
}

void PlotManager::onLineEdit_24EditingFinished()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    int width = ui.lineEdit_24->text().toInt();
    m_curSelectPlot->getDataPairs().at(row)->setWidth(width);
}

void PlotManager::onPushButton_21Clicked()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    m_curSelectPlot->getDataPairs().at(row)->setColor(ui.pushButton_21->color());
}

void PlotManager::onCheckBox_10StateChanged()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    m_curSelectPlot->getDataPairs().at(row)->setLineMode(ui.checkBox_10->checkState());
}

void PlotManager::onCheckBox_11StateChanged()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    m_curSelectPlot->getDataPairs().at(row)->setDraw(ui.checkBox_11->checkState());
}

void PlotManager::onPushButton_15Clicked()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row <= 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
    vec.move(row, row - 1);
    m_curSelectPlot->setDataPair(vec);
    refreshPlotDataUI(m_curSelectPlot);
    ui.tableWidget_plotData->setCurrentCell(row - 1, 0);
}

void PlotManager::onPushButton_16Clicked()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0 || row >= (ui.tableWidget_plotData->rowCount() - 1))
        return;
    if(m_curSelectPlot == nullptr)
        return;

    QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
    vec.move(row, row + 1);
    m_curSelectPlot->setDataPair(vec);
    refreshPlotDataUI(m_curSelectPlot);
    ui.tableWidget_plotData->setCurrentCell(row + 1, 0);
}
// move up data pair
void PlotManager::onPushButton_66Clicked()
{
    if(m_curSelectPlot->getName().startsWith("Text"))
    {
        int row = ui.tableWidget_TextDataSort->currentRow();
        if(row < 0)
            return;
        if(m_curSelectPlot == nullptr)
            return;
        if(ui.tableWidget_TextDataSort->currentColumn() == 0)
        {
            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
            int temRow = ui.tableWidget_TextDataSort->currentRow();

            DataPair* temNowDataPair = vec.at(temRow);

            QString temNowFront = temNowDataPair->getEntity_x();
            QString temNowBack = temNowDataPair->getAttr_x();

            DataPair* temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() - 1);

            QString temAboveFront = temAboveDataPair->getEntity_x();
            QString temAboveBack = temAboveDataPair->getAttr_x();
            // TODO
            //            QPair<QString, QString> newAbove =
            //                qMakePair(temNowFront + "+" + temAboveBack, temAboveDataPair->getDataPair().second);
            //            QPair<QString, QString> newNow =
            //                qMakePair(temAboveFront + "+" + temNowBack, temNowDataPair->getDataPair().second);

            //            temNowDataPair->setDataPair(newNow);
            //            temAboveDataPair->setDataPair(newAbove);
            refreshLightTextUI(m_curSelectPlot);
            ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 0);
        }
        else if(ui.tableWidget_TextDataSort->currentColumn() == 1)
        {
            // TODO
            //            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
            //			int temRow = ui.tableWidget_TextDataSort->currentRow();

            //            DataPair* temNowDataPair = vec.at(temRow);

            //            QString temNowFront = temNowDataPair->getEntity_x();
            //            QString temNowBack = temNowDataPair->getAttr_x();

            //            DataPair* temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() - 1);
            //            QString temAboveFirst = temAboveDataPair->getDataPair().first;
            //			QString temAboveFront = temAboveFirst.split("+").front();
            //			QString temAboveBack = temAboveFirst.split("+").back();
            //            QPair<QString, QString> newAbove =
            //                qMakePair(temAboveFront + "+" + temNowBack, temAboveDataPair->getDataPair().second);
            //            QPair<QString, QString> newNow =
            //                qMakePair(temNowFront + "+" + temAboveBack, temNowDataPair->getDataPair().second);

            //            temNowDataPair->setDataPair(newNow);
            //            temAboveDataPair->setDataPair(newAbove);
            //			refreshLightTextUI(m_curSelectPlot);
            //			ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 1);
        }
        else
            return;
    }
    else if(m_curSelectPlot->getName().startsWith("Light"))
    {
        int row = ui.tableWidget_LightDataSort->currentRow();
        if(row < 0)
            return;
        if(m_curSelectPlot == nullptr)
            return;
        if(ui.tableWidget_LightDataSort->currentColumn() == 0)
        {
            // TODO
            //            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
            //			int temRow = ui.tableWidget_LightDataSort->currentRow();
            //            DataPair* temNowDataPair = vec.at(temRow);
            //			QString temNowFirst = temNowDataPair->getDataPair().first;
            //            DataPair* temAboveDataPair = vec.at(ui.tableWidget_LightDataSort->currentRow() - 1);
            //            QString temAboveFirst = temAboveDataPair->getDataPair().first;
            //            QPair<QString, QString> newAbove =
            //                qMakePair(temNowFirst, temAboveDataPair->getDataPair().second);
            //            QPair<QString, QString> newNow =
            //                qMakePair(temAboveFirst, temNowDataPair->getDataPair().second);

            //            temNowDataPair->setDataPair(newNow);
            //            temAboveDataPair->setDataPair(newAbove);
            //			refreshLightTextUI(m_curSelectPlot);
            //			ui.tableWidget_LightDataSort->setCurrentCell(row - 1, 0);
        }
        else
            return;
    }

    //Light改变的时候可能会用到
    //QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
    //vec.move(row, row - 1);
    //m_curSelectPlot->setDataPair(vec);
    //refreshLightTextUI(m_curSelectPlot);
    //ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 0);
}
// move down data pair
void PlotManager::onPushButton_67Clicked()
{
    if(m_curSelectPlot->getName().startsWith("Text"))
    {
        int row = ui.tableWidget_TextDataSort->currentRow();
        if(row < 0 || row >= (ui.tableWidget_TextDataSort->rowCount() - 1))
            return;
        if(m_curSelectPlot == nullptr)
            return;
        if(ui.tableWidget_TextDataSort->currentColumn() == 0)
        {
            // TODO
            //            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
            //			int temRow = ui.tableWidget_TextDataSort->currentRow();
            //            DataPair* temNowDataPair = vec.at(temRow);
            //			QString temNowFirst = temNowDataPair->getDataPair().first;
            //			QString temNowFront = temNowFirst.split("+").front();
            //			QString temNowBack = temNowFirst.split("+").back();

            //            DataPair* temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() + 1);
            //			QString temAboveFirst = temAboveDataPair->getDataPair().first;
            //			QString temAboveFront = temAboveFirst.split("+").front();
            //			QString temAboveBack = temAboveFirst.split("+").back();
            //            QPair<QString, QString> newAbove =
            //                qMakePair(temNowFront + "+" + temAboveBack, temAboveDataPair->getDataPair().second);
            //            QPair<QString, QString> newNow =
            //                qMakePair(temAboveFront + "+" + temNowBack, temNowDataPair->getDataPair().second);

            //            temNowDataPair->setDataPair(newNow);
            //            temAboveDataPair->setDataPair(newAbove);
            //			refreshLightTextUI(m_curSelectPlot);
            //			ui.tableWidget_TextDataSort->setCurrentCell(row + 1, 0);
        }
        else if(ui.tableWidget_TextDataSort->currentColumn() == 1)
        {
            //            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
            //			int temRow = ui.tableWidget_TextDataSort->currentRow();
            //            DataPair* temNowDataPair = vec.at(temRow);
            //			QString temNowFirst = temNowDataPair->getDataPair().first;
            //			QString temNowFront = temNowFirst.split("+").front();
            //			QString temNowBack = temNowFirst.split("+").back();
            //            DataPair* temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() + 1);
            //			QString temAboveFirst = temAboveDataPair->getDataPair().first;
            //			QString temAboveFront = temAboveFirst.split("+").front();
            //			QString temAboveBack = temAboveFirst.split("+").back();
            //            QPair<QString, QString> newAbove =
            //                qMakePair(temAboveFront + "+" + temNowBack, temAboveDataPair->getDataPair().second);
            //            QPair<QString, QString> newNow =
            //                qMakePair(temNowFront + "+" + temAboveBack, temNowDataPair->getDataPair().second);
            //            temNowDataPair->setDataPair(newNow);
            //            temAboveDataPair->setDataPair(newAbove);
            //			refreshLightTextUI(m_curSelectPlot);
            //			ui.tableWidget_TextDataSort->setCurrentCell(row + 1, 1);
        }
        else
            return;
    }
    else if(m_curSelectPlot->getName().startsWith("Light"))
    {
        //		int row = ui.tableWidget_LightDataSort->currentRow();
        //        if(row < 0 || row >= (ui.tableWidget_LightDataSort->rowCount() - 1))
        //			return;
        //        if(m_curSelectPlot == nullptr)
        //			return;
        //        if(ui.tableWidget_LightDataSort->currentColumn() == 0)
        //		{
        //            QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
        //			int temRow = ui.tableWidget_LightDataSort->currentRow();
        //            DataPair* temNowDataPair = vec.at(temRow);
        //			QString temNowFirst = temNowDataPair->getDataPair().first;
        //            DataPair* temAboveDataPair = vec.at(ui.tableWidget_LightDataSort->currentRow() + 1);
        //			QString temAboveFirst = temAboveDataPair->getDataPair().first;
        //            QPair<QString, QString> newAbove =
        //                qMakePair(temNowFirst, temAboveDataPair->getDataPair().second);
        //            QPair<QString, QString> newNow =
        //                qMakePair(temAboveFirst, temNowDataPair->getDataPair().second);
        //            temNowDataPair->setDataPair(newNow);
        //            temAboveDataPair->setDataPair(newAbove);
        //			refreshLightTextUI(m_curSelectPlot);
        //			ui.tableWidget_LightDataSort->setCurrentCell(row + 1, 0);
    }
    else
        return;
}

void PlotManager::onPushButton_69Clicked()
{
    int row = ui.tableWidget_TextDataSort->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
    vec.remove(row);
    m_curSelectPlot->setDataPair(vec);
    refreshLightTextUI(m_curSelectPlot);
}

void PlotManager::onPushButton_18Clicked() {}

void PlotManager::onPushButton_19Clicked()
{
    int row = ui.tableWidget_plotData->currentRow();
    if(row < 0)
        return;
    if(m_curSelectPlot == nullptr)
        return;

    QVector<DataPair*> vec = m_curSelectPlot->getDataPairs();
    vec.remove(row);
    m_curSelectPlot->setDataPair(vec);
    refreshPlotDataUI(m_curSelectPlot);
}

void PlotManager::onPushButton_20Clicked()
{
    emit sigAdvancedDataManager();
}

void PlotManager::onToolButtonClicked() {}

void PlotManager::onComboBox_6CurrentIndexChanged(int index) {}

void PlotManager::onCheckBox_12StateChanged()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setUnitsShowX(ui.checkBox_12->isChecked());
}

void PlotManager::onCheckBox_13StateChanged()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    m_curSelectPlot->setUnitsShowY(ui.checkBox_13->isChecked());
}

void PlotManager::onCheckBox_14StateChanged()
{
    if(m_curSelectPlot == nullptr)
	{
		return;
	}
    textSettingChanged();
}

void PlotManager::onLineEdit_26EditingFinished()
{
    if(m_curSelectPlot == nullptr)
	{
		return;
    }
    textSettingChanged();
}

void PlotManager::onPushButton_22Clicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    textSettingChanged();
}

void PlotManager::onPushButton_23Clicked()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    textSettingChanged();
}

void PlotManager::onfontComboBox_2CurrentFontChanged(const QFont& font)
{
    if(m_curSelectPlot == nullptr)
	{
		return;
	}
    //	float fontSize = ui.comboBox_Text_fontSize->currentText().toFloat();
    //	QFont newFont;
    //	newFont.setFamily(font.family());
    //    newFont.setPixelSize(fontSize);
    //	m_curSelectPlot->setTitleFont(newFont);

    textSettingChanged();
}

void PlotManager::onSpinBox_FontSizeChanged(int value)
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }

    // 	QFont font = ui.fontComboBox_2->currentFont();
    // 	font.setPixelSize(text.toFloat());
	// 	m_curSelectPlot->setTitleFont(font);
    //    m_curSelectPlot->setTitleFontSize(text.toInt());

    textSettingChanged();
}

void PlotManager::onOffsetValueChanged()
{
    if(m_curSelectPlot == nullptr)
    {
        return;
    }
    m_curSelectPlot->setTitleOffset(ui.lineEdit_29->text().toInt(), ui.lineEdit_30->text().toInt());
}

void PlotManager::onListWidgetItemChanged()
{
    if(!ui.listWidget->currentItem())
        return;

    QList<QWidget*> list;
    list << ui.checkBox_14 << ui.lineEdit_26 << ui.pushButton_22 << ui.pushButton_23
         << ui.fontComboBox_2 << ui.spinBox_FontSize;
    for(auto item : list)
        item->blockSignals(true);

    QString strItem = ui.listWidget->currentItem()->data(Qt::DisplayRole).toString();

    if(strItem == "Title")
    {
        ui.checkBox_14->setChecked(m_curSelectPlot->getTitleVisible());
        ui.lineEdit_26->setText(m_curSelectPlot->getTitle());
        ui.pushButton_22->setColor(m_curSelectPlot->getTitleColor());
        ui.pushButton_23->setColor(m_curSelectPlot->getTitleFillColor());
        ui.fontComboBox_2->setCurrentFont(m_curSelectPlot->getTitleFont());
        ui.spinBox_FontSize->setValue(m_curSelectPlot->getTitleFontSize());
    }
    else if(strItem == "X-Axis Description")
    {
        ui.checkBox_14->setChecked(m_curSelectPlot->getxAxisLabelVisible());
        ui.lineEdit_26->setText(m_curSelectPlot->getxAxisLabel());
        ui.pushButton_22->setColor(m_curSelectPlot->getxAxisLabelColor());
        ui.pushButton_23->setColor(Qt::black);
        ui.fontComboBox_2->setCurrentFont(m_curSelectPlot->getxAxisLabelFont());
        ui.spinBox_FontSize->setValue(m_curSelectPlot->getxAxisLabelFontSize());
    }
    else if(strItem == "Y-Axis Description")
    {
        ui.checkBox_14->setChecked(m_curSelectPlot->getyAxisLabelVisible());
        ui.lineEdit_26->setText(m_curSelectPlot->getyAxisLabel());
        ui.pushButton_22->setColor(m_curSelectPlot->getyAxisLabelColor());
        ui.pushButton_23->setColor(Qt::black);
        ui.fontComboBox_2->setCurrentFont(m_curSelectPlot->getyAxisLabelFont());
        ui.spinBox_FontSize->setValue(m_curSelectPlot->getyAxisLabelFontSize());
    }
    else if(strItem == "X-Axis Data")
    {
        ui.checkBox_14->setChecked(m_curSelectPlot->getxAxisLabelVisible());
        ui.lineEdit_26->setText("");
        ui.pushButton_22->setColor(m_curSelectPlot->getxTickLabelColor());
        ui.pushButton_23->setColor(Qt::black);
        ui.fontComboBox_2->setCurrentFont(m_curSelectPlot->getxTickLabelFont());
        ui.spinBox_FontSize->setValue(m_curSelectPlot->getxTickLabelFontSize());
    }
    else if(strItem == "Y-Axis Data")
    {
        ui.checkBox_14->setChecked(m_curSelectPlot->getyAxisLabelVisible());
        ui.lineEdit_26->setText("");
        ui.pushButton_22->setColor(m_curSelectPlot->getyTickLabelColor());
        ui.pushButton_23->setColor(Qt::black);
        ui.fontComboBox_2->setCurrentFont(m_curSelectPlot->getyTickLabelFont());
        ui.spinBox_FontSize->setValue(m_curSelectPlot->getyTickLabelFontSize());
    }

    for(auto item : list)
        item->blockSignals(false);
}

void PlotManager::textSettingChanged()
{
    if(!ui.listWidget->currentItem())
        return;
    QString strItem = ui.listWidget->currentItem()->data(Qt::DisplayRole).toString();
    if(strItem == "Title")
    {
        m_curSelectPlot->setTitleVisible(ui.checkBox_14->isChecked());
        m_curSelectPlot->setTitle(ui.lineEdit_26->text());
        m_curSelectPlot->setTitleColor(ui.pushButton_22->color());
        m_curSelectPlot->setTitleFillColor(ui.pushButton_23->color());
        m_curSelectPlot->setTitleFont(ui.fontComboBox_2->currentFont());
        m_curSelectPlot->setTitleFontSize(ui.spinBox_FontSize->value());
    }
    else if(strItem == "X-Axis Description")
    {
        m_curSelectPlot->setxAxisLabel(ui.lineEdit_26->text());
        m_curSelectPlot->setxAxisLabelVisible(ui.checkBox_14->isChecked());
        m_curSelectPlot->setxAxisLabelColor(ui.pushButton_22->color());
        m_curSelectPlot->setxAxisLabelFont(ui.fontComboBox_2->currentFont());
        m_curSelectPlot->setxAxisLabelFontSize(ui.spinBox_FontSize->value());
    }
    else if(strItem == "Y-Axis Description")
    {
        m_curSelectPlot->setyAxisLabel(ui.lineEdit_26->text());
        m_curSelectPlot->setyAxisLabelVisible(ui.checkBox_14->isChecked());
        m_curSelectPlot->setyAxisLabelColor(ui.pushButton_22->color());
        m_curSelectPlot->setyAxisLabelFont(ui.fontComboBox_2->currentFont());
        m_curSelectPlot->setyAxisLabelFontSize(ui.spinBox_FontSize->value());
    }
    else if(strItem == "X-Axis Data")
    {
        m_curSelectPlot->setxTickLabelVisible(ui.checkBox_14->isChecked());
        m_curSelectPlot->setxTickLabelFont(ui.fontComboBox_2->currentFont());
        m_curSelectPlot->setxTickLabelFontSize(ui.spinBox_FontSize->value());
        m_curSelectPlot->setxTickLabelColor(ui.pushButton_22->color());
    }
    else if(strItem == "Y-Axis Data")
    {
        m_curSelectPlot->setyTickLabelVisible(ui.checkBox_14->isChecked());
        m_curSelectPlot->setyTickLabelFont(ui.fontComboBox_2->currentFont());
        m_curSelectPlot->setyTickLabelFontSize(ui.spinBox_FontSize->value());
        m_curSelectPlot->setyTickLabelColor(ui.pushButton_22->color());
    }
}

void PlotManager::onPushButton_80Clicked()
{
    if(auto plot = dynamic_cast<PlotAttitude*>(m_curSelectPlot))
    {
        plot->setRollColor(ui.pushButton_80->color());
    }
}

void PlotManager::onPushButton_81Clicked()
{
    if(auto plot = dynamic_cast<PlotAttitude*>(m_curSelectPlot))
    {
        plot->setPitchColor(ui.pushButton_81->color());
    }
}

void PlotManager::onSpinBox_29ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotAttitude*>(m_curSelectPlot))
    {
        plot->setTickRadiusPercentage(value);
    }
}

void PlotManager::onSpinBox_30ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotAttitude*>(m_curSelectPlot))
    {
        plot->setTextPercentage(value);
    }
}

void PlotManager::onSpinBox_31ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotAttitude*>(m_curSelectPlot))
    {
        plot->setDialPercentage(value);
    }
}

void PlotManager::onSpinBox_21ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setTickRadiusRate(value);
    }
}

void PlotManager::onSpinBox_22ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setColorRate(value);
    }
}

void PlotManager::onSpinBox_23ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setTextRate(value);
    }
}

void PlotManager::onSpinBox_24ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDialRate(value);
    }
}

void PlotManager::onSpinBox_25ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDialCapRate(value);
    }
}

void PlotManager::onSpinBox_26ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setStartAngle(value);
    }
}

void PlotManager::onSpinBox_27ValueChanged(int value)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setEndAngle(value);
    }
}

void PlotManager::onSpinBox_28ValueChanged(int) {}

void PlotManager::onPushButton_74Clicked()
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setCapColor(ui.pushButton_74->color());
    }
}

void PlotManager::onPushButton_75Clicked()
{
    double begin = ui.lineEdit_49->text().toDouble();
    double end = ui.lineEdit_50->text().toDouble();
    QColor clr = ui.pushButton_78->color();
    QColor outline = ui.pushButton_79->color();
    int outLineWidth = ui.spinBox_28->value();
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setData(0, Qt::DisplayRole, begin);
    item->setData(1, Qt::DisplayRole, end);
    item->setData(2, Qt::DisplayRole, clr);
    item->setData(3, Qt::DisplayRole, outline);
    item->setData(4, Qt::DisplayRole, outLineWidth);
    ui.treeWidget_colorRanges->addTopLevelItem(item);
}

void PlotManager::onPushButton_76Clicked()
{
    QList<DialColorInfo> dialInfoList;
    int cnt = ui.treeWidget_colorRanges->topLevelItemCount();
    if(cnt < 2)
        return;
    for(int i = 1; i < cnt; i++)
    {
        DialColorInfo dialInfo;
        dialInfo.start =
            ui.treeWidget_colorRanges->topLevelItem(i)->data(0, Qt::DisplayRole).toDouble();
        dialInfo.end =
            ui.treeWidget_colorRanges->topLevelItem(i)->data(1, Qt::DisplayRole).toDouble();
        dialInfo.clr =
            ui.treeWidget_colorRanges->topLevelItem(i)->data(2, Qt::DisplayRole).value<QColor>();
        dialInfo.outline =
            ui.treeWidget_colorRanges->topLevelItem(i)->data(3, Qt::DisplayRole).value<QColor>();
        dialInfo.width =
            ui.treeWidget_colorRanges->topLevelItem(i)->data(4, Qt::DisplayRole).toInt();
        dialInfoList.push_back(dialInfo);
    }
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setColorInfoList(dialInfoList);
    }
}

void PlotManager::onPushButton_77Clicked()
{
    for(auto item : ui.treeWidget_colorRanges->selectedItems())
        ui.treeWidget_colorRanges->takeTopLevelItem(
            ui.treeWidget_colorRanges->indexOfTopLevelItem(item));
}

void PlotManager::onComboBox_dialStyleCurrentTextChanged(const QString& text)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDialStyle(text);
    }
}

void PlotManager::onCheckBox_34StateChanged(int state)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        // 2表示选中，0表示未选中
        plot->setDrawFirstTick(state == 2);
    }
}

void PlotManager::onCheckBox_35StateChanged(int state)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDrawLastTick(state == 2);
    }
}

void PlotManager::onCheckBox_36StateChanged(int state)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDrawFirstTextLabel(state == 2);
    }
}

void PlotManager::onCheckBox_37StateChanged(int state)
{
    if(auto plot = dynamic_cast<PlotDial*>(m_curSelectPlot))
    {
        plot->setDrawLastTextLabel(state == 2);
    }
}

void PlotManager::onAddScatterLimit()
{
    QString name = ui.lineEditLimitName->text();
    double value = ui.lineEditLimitValue->text().toDouble();
    QColor lineColor = ui.pushButtonLineColor->color();
    uint32_t lineWidth = ui.spinBoxLimitLineWidth->value();
    QColor FAColor = ui.pushButtonLimitFAColor->color();
    QColor FBColor = ui.pushButtonLimitFBColor->color();
    if(auto plot = dynamic_cast<PlotScatter*>(m_curSelectPlot))
    {
        plot->addBackgroundColorInfo(name, value, lineColor, lineWidth, FAColor, FBColor);
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, name);
        item->setText(1, QString::number(value));
        //左侧功能未实现，暂时这里写一个固定的>
        item->setText(2, ">");
        ui.treeWidgetLimit->addTopLevelItem(item);
    }
}

void PlotManager::onRemoveScatterLimit()
{
    if(auto plot = dynamic_cast<PlotScatter*>(m_curSelectPlot))
    {
        if(auto curItem = ui.treeWidgetLimit->currentItem())
        {
            double value = curItem->text(1).toDouble();
            plot->removeBackgroundColorInfo(value);
            auto item =
                ui.treeWidgetLimit->takeTopLevelItem(ui.treeWidgetLimit->currentIndex().row());
            delete item;
        }
    }
}

void PlotManager::onUpdateScatterLimit() {}

void PlotManager::onCurrentLimitChanged(QTreeWidgetItem* item, int column)
{
    Q_UNUSED(column)
    if(auto plot = dynamic_cast<PlotScatter*>(m_curSelectPlot))
    {
        double value = item->text(1).toDouble();
        auto segMap = plot->getBkgLimitSegMap();
        if(segMap.contains(value))
        {
            const auto seg = segMap.value(value);
            ui.lineEditLimitName->setText(seg.m_limitName);
            ui.lineEditLimitValue->setText(QString::number(seg.m_limitValue));
            ui.pushButtonLineColor->setColor(seg.m_lineColor);
            ui.spinBoxLimitLineWidth->setValue(seg.m_lineWidth);
            ui.pushButtonLimitFAColor->setColor(seg.m_fillAboveColor);
            ui.pushButtonLimitFBColor->setColor(seg.m_fillBelowColor);
        }
    }
}
