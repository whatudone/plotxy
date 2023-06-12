#include "AddPlotPair.h"
#include "DataManager.h"
#include "PlotManagerData.h"
#include "ui_AddPlotPair.h"
#include <QDebug>
#include <QMessageBox>
#include <QPoint>
#include <QRadioButton>
#include <QSet>
#include <QSpinBox>
#include <QStyleFactory>
#include <QTreeWidget>
#include <QTreeWidgetItem>

AddPlotPair::AddPlotPair(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    initStackedWidget_page1();
    initStackedWidget_page2();
    initStackedWidget_page3();
    initStackedWidget_page4();
    initStackedWidget_pageLight();
    initStackedWidget_pageAScope();

    initTreePlot();

    connect(PlotManagerData::getInstance(),
            &PlotManagerData::plotDataChanged,
            this,
            &AddPlotPair::updatePlotTrees);

    connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
    connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));
    connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(onBtnRemoveClicked()));
    connect(ui.pushButton_update, SIGNAL(clicked()), this, SLOT(onBtnUpdateClicked()));
}

AddPlotPair::~AddPlotPair() {}

void AddPlotPair::initStackedWidget_page1()
{
    ui.tableWidget_Entity->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui.tableWidget_Entity->verticalHeader()->hide();
    ui.tableWidget_Entity_9->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_10->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_9->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_9->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_9->verticalHeader()->hide();
    ui.tableWidget_Entity_10->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_10->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_10->verticalHeader()->hide();

    connect(ui.tableWidget_Entity,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
    connect(ui.radioButton, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_4->setCurrentIndex(0);
    });
    connect(ui.radioButton_2, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_4->setCurrentIndex(1);
    });

    ui.radioButton->clicked();
    ui.radioButton->setChecked(true);
}

void AddPlotPair::initStackedWidget_page2()
{
    ui.tableWidget_Entity_2->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_3->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_5->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_6->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_7->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_8->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_2->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_2->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_2->verticalHeader()->hide();
    ui.tableWidget_Entity_3->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_3->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_3->verticalHeader()->hide();

    ui.tableWidget_Entity_4->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_4->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_4->verticalHeader()->hide();
    ui.tableWidget_Entity_5->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_5->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_5->verticalHeader()->hide();

    ui.tableWidget_Entity_5->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_5->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_5->verticalHeader()->hide();
    ui.tableWidget_Entity_6->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_6->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_6->verticalHeader()->hide();
    ui.tableWidget_Entity_7->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_7->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_7->verticalHeader()->hide();
    ui.tableWidget_Entity_8->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_8->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_8->verticalHeader()->hide();

    connect(ui.tableWidget_Entity_2,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_3,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));

    connect(ui.radioButton_7, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_2->setCurrentIndex(0);
    });
    connect(ui.radioButton_5, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_2->setCurrentIndex(1);
    });
    connect(ui.radioButton_6, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_2->setCurrentIndex(2);
    });

    connect(ui.radioButton_8, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_3->setCurrentIndex(0);
    });
    connect(ui.radioButton_3, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_3->setCurrentIndex(1);
    });
    connect(ui.radioButton_4, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_3->setCurrentIndex(2);
    });

    ui.radioButton_5->clicked();
    ui.radioButton_5->setChecked(true);
    ui.radioButton_3->clicked();
    ui.radioButton_3->setChecked(true);
}

void AddPlotPair::initStackedWidget_page3()
{
    ui.tableWidget_Entity_Attitude1->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_Attitude2->setStyleSheet("QHeaderView::section{background:lightgray;}");

    ui.tableWidget_Entity_Attitude1->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_Attitude1->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_Attitude1->verticalHeader()->hide();
    ui.tableWidget_Entity_Attitude2->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_Attitude2->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_Attitude2->verticalHeader()->hide();

    connect(ui.tableWidget_Entity,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_2,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_3,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_4,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_4(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_Attitude1,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_Attitude1(QTableWidgetItem*)));
    connect(ui.tableWidget_Entity_Attitude2,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_Attitude2(QTableWidgetItem*)));
}

void AddPlotPair::initStackedWidget_page4()
{
    ui.tableWidget_Entity_4->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_Entity_4->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_Entity_4->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_Entity_4->verticalHeader()->hide();

    connect(ui.tableWidget_Entity_4,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetItemClicked_4(QTableWidgetItem*)));
    connect(ui.radioButton_12, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_5->setCurrentIndex(0);
    });
    connect(ui.radioButton_9, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_5->setCurrentIndex(1);
    });
    connect(ui.radioButton_10, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_5->setCurrentIndex(2);
    });
    connect(ui.radioButton_11, &QRadioButton::clicked, this, [=] {
        ui.stackedWidget_5->setCurrentIndex(3);
    });

    ui.radioButton_9->clicked();
    ui.radioButton_9->setChecked(true);
}

void AddPlotPair::initStackedWidget_pageLight()
{
    ui.tableWidget_lightEntity->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_lightEntity->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_lightEntity->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_lightEntity->verticalHeader()->hide();
    ui.tableWidget_lightNameUnits->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.radioButton_lightParameter->setChecked(true);
    ui.tableWidget_LightSet->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    connect(ui.tableWidget_lightEntity,
            SIGNAL(itemClicked(QTableWidgetItem*)),
            this,
            SLOT(onTableWidgetLightEntityClicked(QTableWidgetItem*)));

    connect(ui.tableWidget_LightSet,
            &QTableWidget::itemDoubleClicked,
            this,
            &AddPlotPair::onLightSetDbClicked);
    connect(ui.pushButton_LightAdd, SIGNAL(clicked()), this, SLOT(onBtnLightAddClicked()));
    connect(ui.pushButton_LightDelete, SIGNAL(clicked()), this, SLOT(onBtnLightDeleteClicked()));
    connect(ui.pushButton_LightUpdate, SIGNAL(clicked()), this, SLOT(onBtnLightUpdateClicked()));
}

void AddPlotPair::initStackedWidget_pageAScope()
{
    ui.tableWidget_AScopeEntity->setStyleSheet("QHeaderView::section{background:lightgray;}");
    ui.tableWidget_AScopeEntity->horizontalHeader()->setStretchLastSection(true);
    ui.tableWidget_AScopeEntity->horizontalHeader()->setSectionResizeMode(
        QHeaderView::ResizeToContents);
    ui.tableWidget_AScopeEntity->verticalHeader()->hide();
}

void AddPlotPair::initTreePlot()
{
    m_treePlot = new QTreeWidget();
    m_menuPlot = new QMenu(this);
    m_widgetActionPlot = new QWidgetAction(m_treePlot);
    m_treePlot->setFrameShape(QFrame::NoFrame);
    m_treePlot->setFixedWidth(ui.toolButton_plot->maximumWidth());
    m_treePlot->setStyle(QStyleFactory::create("windows"));
    m_treePlot->setHeaderHidden(true);
    m_treePlot->expandAll();
    m_widgetActionPlot->setDefaultWidget(m_treePlot);
    m_menuPlot->addAction(m_widgetActionPlot);
    ui.toolButton_plot->setMenu(m_menuPlot);

    connect(m_treePlot,
            SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)),
            this,
            SLOT(onDoubleClickedTreeWidgetItem(QTreeWidgetItem*, int)));
}

void AddPlotPair::init(PlotType index)
{
    onChangeStackIndex(index);
    onUpdateEntityTableByDataChanged();
}

void AddPlotPair::onChangeStackIndex(PlotType index)
{
    switch(index)
    {
    case Type_PlotScatter:
        ui.groupBox_3->setTitle(QString("X轴数据源"));
        ui.groupBox_2->setTitle(QString("Y轴数据源"));
        ui.stackedWidget->setCurrentIndex(1);
        break;
    case Type_PlotPolar:
        ui.groupBox_3->setTitle(QString("角度数据源"));
        ui.groupBox_2->setTitle(QString("幅度数据源"));
        ui.stackedWidget->setCurrentIndex(1);
        break;
    case Type_PlotAScope:
    case Type_PlotRTI:
    case Type_PlotDoppler:
        ui.label_XAxis->setText(QString("X轴：Range(m)"));
        ui.label_YAxis->setText(QString("Y轴：Voltage(v)"));
        ui.stackedWidget->setCurrentIndex(5);
        break;
    case Type_PlotText:
        ui.stackedWidget->setCurrentIndex(3);
        break;
    case Type_PlotLight:
        ui.stackedWidget->setCurrentIndex(4);
        break;
    case Type_PlotBar:
    case Type_PlotDial:
    case Type_PlotTrack:
        ui.stackedWidget->setCurrentIndex(0);
        break;
    case Type_PlotAttitude:
        ui.stackedWidget->setCurrentIndex(2);
        break;
    default:
        ui.stackedWidget->setCurrentIndex(0);
        break;
    }
}

void AddPlotPair::setPlotBaseInfo(PlotItemBase* pBaseItem)
{
    if(!pBaseItem)
        return;
    QList<QTreeWidgetItem*> plotItems =
        m_treePlot->findItems(pBaseItem->getName(), Qt::MatchCaseSensitive | Qt::MatchRecursive, 0);
    if(plotItems.size() > 0)
    {
        m_treePlot->itemDoubleClicked(plotItems[0], 0);
    }
}

void AddPlotPair::updatePlotTrees(const QMap<QString, QList<PlotItemBase*>>& plotData)
{
    if(plotData.isEmpty())
        return;

    m_treePlot->clear();

    for(int i = 0; i < plotData.size(); ++i)
    {
        QString tabString = plotData.keys().at(i);
        QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabString);
        m_treePlot->addTopLevelItem(itemselPlotH);
        m_treePlot->expandAll();

        for(int j = 0; j < plotData[tabString].size(); ++j)
        {
            QString plotString = plotData[tabString].at(j)->getName();
            QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotString);
            itemselPlotH->addChild(itemselPlotI);
        }
    }
}

bool AddPlotPair::getCurrentSelectParam(int32_t& xEntityID,
                                        QString& xAttrName,
                                        QString& xAttrUnitName,
                                        int32_t& yEntityID,
                                        QString& yAttrName,
                                        QString& yAttrUnitName)
{
    // 11种图表这里归纳为5种添加数据的界面
    // TODO:AScope三种图表还没实现 ，范围计算还没实现
    int index = ui.stackedWidget->currentIndex();

    switch(index)
    {
    // Bar Dial Track
    case 0:
        // parameter
        if(ui.radioButton->isChecked())
        {
            if(ui.tableWidget_Entity->currentItem() == nullptr ||
               ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0) == nullptr)
                return false;

            xEntityID = ui.tableWidget_Entity->currentItem()->data(Qt::UserRole + 1).toInt();
            xAttrName =
                ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0)->text();
            xAttrUnitName =
                ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 1)->text();
        }
        // 范围计算
        else if(ui.radioButton_2->isChecked())
        {
            if(ui.tableWidget_Entity_9->currentItem() == nullptr ||
               ui.tableWidget_Entity_10->currentItem() == nullptr)
                return false;

            //            strEntity1 = ui.tableWidget_Entity_9->currentItem()->text();
            //            strNameUnit1 = ui.tableWidget_Entity_10->currentItem()->text();
        }
        // 固定y轴为时间
        yAttrName = "Time";

        break;
    // scatter polar
    case 1:
        if(ui.radioButton_7->isChecked())
        {
            xAttrName = "Time";
        }
        else if(ui.radioButton_5->isChecked())
        {
            if(ui.tableWidget_Entity_2->currentItem() == nullptr ||
               ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0) ==
                   nullptr)
                return false;

            xEntityID = ui.tableWidget_Entity_2->currentItem()->data(Qt::UserRole + 1).toInt();
            xAttrName =
                ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0)
                    ->text();
            xAttrUnitName =
                ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 1)
                    ->text();
        }
        else if(ui.radioButton_6->isChecked())
        {
            if(ui.tableWidget_Entity_5->currentItem() == nullptr ||
               ui.tableWidget_Entity_6->currentItem() == nullptr)
                return false;

            //            strEntity1 = ui.tableWidget_Entity_5->currentItem()->text();
            //            strNameUnit1 = ui.tableWidget_Entity_6->currentItem()->text();
        }

        if(ui.radioButton_8->isChecked())
        {
            yAttrName = "Time";
        }
        else if(ui.radioButton_3->isChecked())
        {
            if(ui.tableWidget_Entity_3->currentItem() == nullptr ||
               ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0) ==
                   nullptr)
                return false;

            yEntityID = ui.tableWidget_Entity_3->currentItem()->data(Qt::UserRole + 1).toInt();
            yAttrName =
                ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0)
                    ->text();
            yAttrUnitName =
                ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 1)
                    ->text();
        }
        else if(ui.radioButton_4->isChecked())
        {
            if(ui.tableWidget_Entity_7->currentItem() == nullptr ||
               ui.tableWidget_Entity_8->currentItem() == nullptr)
                return false;

            //            strEntity2 = ui.tableWidget_Entity_7->currentItem()->text();
            //            strNameUnit2 = ui.tableWidget_Entity_8->currentItem()->text();
        }
        break;
        // Attitude
    case 2:
        if(ui.tableWidget_Entity_Attitude1->currentItem() == nullptr ||
           ui.tableWidget_nameUnits_Attitude1->item(
               ui.tableWidget_nameUnits_Attitude1->currentRow(), 0) == nullptr ||
           ui.tableWidget_Entity_Attitude2->currentItem() == nullptr ||
           ui.tableWidget_nameUnits_Attitude1->item(
               ui.tableWidget_nameUnits_Attitude2->currentRow(), 0) == nullptr)
            return false;

        xEntityID = ui.tableWidget_Entity_Attitude1->currentItem()->data(Qt::UserRole + 1).toInt();
        xAttrName = ui.tableWidget_nameUnits_Attitude1
                        ->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0)
                        ->text();
        xAttrUnitName = ui.tableWidget_nameUnits_Attitude1
                            ->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 1)
                            ->text();

        yEntityID = ui.tableWidget_Entity_Attitude2->currentItem()->data(Qt::UserRole + 1).toInt();
        yAttrName = ui.tableWidget_nameUnits_Attitude2
                        ->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0)
                        ->text();
        yAttrUnitName = ui.tableWidget_nameUnits_Attitude2
                            ->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 1)
                            ->text();

        break;
        // Text
    case 3:
        if(ui.radioButton_12->isChecked())
        {
            xAttrName = "Time";
        }
        else if(ui.radioButton_9->isChecked())
        {
            if(ui.tableWidget_Entity_4->currentItem() == nullptr ||
               ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0) ==
                   nullptr)
                return false;

            xEntityID = ui.tableWidget_Entity_4->currentItem()->data(Qt::UserRole + 1).toInt();
            xAttrName =
                ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0)
                    ->text();
            xAttrUnitName =
                ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 1)
                    ->text();

            yAttrName = "Time";
            emit sgn_onTextLightBtnClicked();
        }
        else if(ui.radioButton_10->isChecked())
        {}
        else if(ui.radioButton_11->isChecked())
        {}
        break;
        // light
    case 4: {
        if(ui.radioButton_lightParameter->isChecked())
        {
            if(ui.tableWidget_lightEntity->currentItem() == nullptr ||
               ui.tableWidget_lightNameUnits->item(ui.tableWidget_lightNameUnits->currentRow(),
                                                   0) == nullptr)
                return false;
            xEntityID = ui.tableWidget_lightEntity->currentItem()->data(Qt::UserRole + 1).toInt();
            xAttrName =
                ui.tableWidget_lightNameUnits->item(ui.tableWidget_lightNameUnits->currentRow(), 0)
                    ->text();
            xAttrUnitName =
                ui.tableWidget_lightNameUnits->item(ui.tableWidget_lightNameUnits->currentRow(), 1)
                    ->text();

            yAttrName = "Time";
        }
        break;
    }
        // ASCope
    case 5: {

        if(!ui.tableWidget_AScopeEntity->currentItem())
            return false;
        xEntityID = ui.tableWidget_AScopeEntity->currentItem()->data(Qt::UserRole + 1).toInt();
        // 目前根据需求将此处属性和单位写死，后续看是否存在动态变化的需求
        xAttrName = "Range";
        xAttrUnitName = "m";
        yEntityID = xEntityID;
        yAttrName = "Voltage";
        yAttrUnitName = "v";

        break;
    }
    default:
        break;
    }
    return true;
}

void AddPlotPair::updateAttrTableWidgetOnEntityChanged(QTableWidgetItem* entityItem,
                                                       QTableWidget* attrTableWidget)
{
    attrTableWidget->clearContents();
    attrTableWidget->setColumnCount(2);
    QString entityName = entityItem->text();
    if(entityName == "Now")
    {
        attrTableWidget->setRowCount(1);
        attrTableWidget->setItem(0, 0, new QTableWidgetItem("Now"));
        attrTableWidget->setItem(0, 1, new QTableWidgetItem(""));
    }
    else
    {
        int32_t id = entityItem->data(Qt::UserRole + 1).toInt();
        auto pairList = DataManagerInstance->getAttrAndUnitPairList(id);
        auto pairSize = pairList.size();
        attrTableWidget->setRowCount(pairSize);
        QString attr;
        QString unit;
        for(int i = 0; i < pairSize; i++)
        {
            attr = pairList.at(i).first;
            unit = pairList.at(i).second;

            attrTableWidget->setItem(i, 0, new QTableWidgetItem(attr));
            attrTableWidget->setItem(i, 1, new QTableWidgetItem(unit));
        }
    }
}
// 添加数据对
void AddPlotPair::onBtnAddClicked()
{
    int32_t xEntityID;
    QString xAttrName;
    QString xAttrUnitName;
    int32_t yEntityID;
    QString yAttrName;
    QString yAttrUnitName;

    if(!getCurrentSelectParam(
           xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName))
    {
        return;
    }

    // TODO:m_pCurSelectedPlot初始化的没有值，需要完善
    if(m_pCurSelectedPlot)
    {
        QHash<QString, QVariant> dataHash;
        if(m_pCurSelectedPlot->plotType() == PlotType::Type_PlotLight)
        {
            dataHash.insert("Desc", ui.lineEdit_LightDesc->text());
        }
        // 数据对触发的DataPairsChanged信号会在后续触发数据对表格的刷新操作
        m_pCurSelectedPlot->addPlotDataPair(
            xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName, dataHash);
    }
}

void AddPlotPair::onTableWidgetItemClicked(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits);
}

void AddPlotPair::onTableWidgetItemClicked_2(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits_2);
}

void AddPlotPair::onTableWidgetItemClicked_3(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits_3);
}

void AddPlotPair::onTableWidgetItemClicked_4(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits_4);
}

void AddPlotPair::onTableWidgetLightEntityClicked(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_lightNameUnits);
}

void AddPlotPair::onLightSetDbClicked(QTableWidgetItem* item)
{
    // 只响应第四列的双击事件
    if(item->column() == 4)
    {
        auto color = QColorDialog::getColor();
        if(color.isValid())
        {
            item->setBackgroundColor(color);
        }
    }
}

void AddPlotPair::onTableWidgetItemClicked_Attitude1(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits_Attitude1);
}

void AddPlotPair::onTableWidgetItemClicked_Attitude2(QTableWidgetItem* curItem)
{
    updateAttrTableWidgetOnEntityChanged(curItem, ui.tableWidget_nameUnits_Attitude2);
}

void AddPlotPair::onUpdateEntityTableByDataChanged()
{
    // 数据更新，重新刷新实体表格数据
    auto dataMap = DataManager::getInstance()->getDataMap();
    if(dataMap.isEmpty())
    {
        qWarning() << QString("尚未加载数据,当前数据为空");
        return;
    }
    int icount = dataMap.size();
    QList<QTableWidget*> tableList;
    tableList << ui.tableWidget_Entity << ui.tableWidget_Entity_2 << ui.tableWidget_Entity_3
              << ui.tableWidget_Entity_4 << ui.tableWidget_Entity_5 << ui.tableWidget_lightEntity
              << ui.tableWidget_Entity_6 << ui.tableWidget_Entity_7 << ui.tableWidget_Entity_8
              << ui.tableWidget_Entity_9 << ui.tableWidget_Entity_10
              << ui.tableWidget_Entity_Attitude1 << ui.tableWidget_Entity_Attitude2
              << ui.tableWidget_AScopeEntity;
    for(auto tableWidget : tableList)
    {
        tableWidget->clearContents();
        tableWidget->setRowCount(icount + 1);
    }

    for(auto tableWidget : tableList)
    {
        int index = 0;
        for(auto it = dataMap.begin(); it != dataMap.end(); it++)
        {
            QString currEntityType = DataManagerInstance->getEntityNameByID(it.key());
            QTableWidgetItem* item = new QTableWidgetItem(currEntityType);
            // 将key作为隐藏数据设置到item中，方便后续通过key查找数据
            item->setData(Qt::UserRole + 1, it.key());
            tableWidget->setItem(index, 0, item);
            index++;
        }
        tableWidget->setItem(index, 0, new QTableWidgetItem("Now"));
    }
}

void AddPlotPair::onAddPlot(const QString& tabName, PlotItemBase* plotItem)
{
    //数据层更新
    PlotManagerData::getInstance()->addPlotByTab(tabName, plotItem);
}

void AddPlotPair::onDoubleClickedTreeWidgetItem(QTreeWidgetItem* item, int column)
{
    QTreeWidgetItem* parent = item->parent();
    if(nullptr == parent)
        return;

    QString tabNanme = parent->text(0);
    QString plotName = item->text(column);

    ui.toolButton_plot->setText(plotName);
    auto plot = PlotManagerData::getInstance()->getPlotByTabAndName(tabNanme, plotName);
    if(plot)
    {
        // 更新当前选中的图表
        m_pCurSelectedPlot = plot;
        m_currTabName = tabNanme;
        m_currPlotName = plotName;
        onChangeStackIndex(plot->plotType());

        QVector<DataPair*> dataPairs = plot->getDataPairs();
        ui.tableWidget_union->clearContents();
        ui.tableWidget_union->setRowCount(dataPairs.size());
        // 更新历史添加过的数据对
        for(int row = 0; row < dataPairs.size(); ++row)
        {
            auto dataPair = dataPairs.value(row);
            QTableWidgetItem* addplot1 = new QTableWidgetItem(dataPair->getXEntityAttrPair());
            QTableWidgetItem* addplot2 = new QTableWidgetItem(dataPair->getYEntityAttrPair());
            addplot1->setData(Qt::UserRole + 1, dataPair->getUuid());
            ui.tableWidget_union->setItem(row, 0, addplot1);
            ui.tableWidget_union->setItem(row, 1, addplot2);
        }
    }
}
// 更新已经添加的DataPair里面的数据
void AddPlotPair::onBtnUpdateClicked()
{
    int row = ui.tableWidget_union->currentRow();
    if(row != -1)
    {
        int32_t xEntityID;
        QString xAttrName;
        QString xAttrUnitName;
        int32_t yEntityID;
        QString yAttrName;
        QString yAttrUnitName;
        if(!getCurrentSelectParam(
               xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName))
        {
            return;
        }
        if(m_pCurSelectedPlot)
        {
            // 从data中取出添加过的uuid数据
            QString uuid = ui.tableWidget_union->item(row, 0)->data(Qt::UserRole + 1).toString();
            m_pCurSelectedPlot->updatePlotPairData(
                uuid, xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
        }
    }
}

void AddPlotPair::onBtnRemoveClicked()
{
    int row = ui.tableWidget_union->currentRow();
    if(row != -1)
    {
        if(m_pCurSelectedPlot)
        {
            // 从data中取出添加过的uuid数据
            QString uuid = ui.tableWidget_union->item(row, 0)->data(Qt::UserRole + 1).toString();
            m_pCurSelectedPlot->delPlotPairData(uuid);
        }
    }
}

void AddPlotPair::onUpdatePlotPair(PlotItemBase* pBaseItem)
{
    if(!pBaseItem)
        return;

    QList<QTreeWidgetItem*> items =
        m_treePlot->findItems(pBaseItem->getName(), Qt::MatchExactly | Qt::MatchRecursive);
    if(items.size() != 0)
    {
        foreach(QTreeWidgetItem* item, items)
        {
            if(item->parent() != nullptr && item->parent()->text(0) == pBaseItem->getTabName())
            {
                m_treePlot->itemDoubleClicked(item, 0);

                break;
            }
        }
    }
}
// light 添加约束信息
void AddPlotPair::onBtnLightAddClicked()
{
    // 选中了实体和属性之后才能添加对应的约束
    if(!ui.tableWidget_lightEntity->currentItem() || !ui.tableWidget_lightNameUnits->currentItem())
    {
        return;
    }
    int iRow = ui.tableWidget_LightSet->rowCount();
    ui.tableWidget_LightSet->setRowCount(iRow + 1);

    QString entityName = ui.tableWidget_lightEntity->currentItem()->text();
    QTableWidgetItem* entityNameItem = new QTableWidgetItem(entityName);
    entityNameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
    int32_t xEntityID = ui.tableWidget_lightEntity->currentItem()->data(Qt::UserRole + 1).toInt();
    entityNameItem->setData(Qt::UserRole + 1, xEntityID);

    QString attrName = ui.tableWidget_lightNameUnits->currentItem()->text();
    QTableWidgetItem* attrNameItem = new QTableWidgetItem(attrName);
    attrNameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QTableWidgetItem* threshold = new QTableWidgetItem("0.0");

    QTableWidgetItem* colorNameItem = new QTableWidgetItem();
    // 默认绿色
    colorNameItem->setBackgroundColor(Qt::green);
    colorNameItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    QComboBox* constraintCom = new QComboBox;
    constraintCom->addItem(QString("≥"));
    constraintCom->addItem("<");

    ui.tableWidget_LightSet->setItem(iRow, 0, entityNameItem);
    ui.tableWidget_LightSet->setItem(iRow, 1, attrNameItem);
    ui.tableWidget_LightSet->setCellWidget(iRow, 2, constraintCom);
    ui.tableWidget_LightSet->setItem(iRow, 3, threshold);
    ui.tableWidget_LightSet->setItem(iRow, 4, colorNameItem);
}

void AddPlotPair::onBtnLightDeleteClicked()
{
    int rowIdx = ui.tableWidget_LightSet->currentRow();
    if(rowIdx != -1)
    {
        ui.tableWidget_LightSet->removeRow(rowIdx);
    }

    ui.tableWidget_LightSet->setCurrentCell(0, 0);
}

void AddPlotPair::onBtnLightUpdateClicked()
{
    QList<std::tuple<int32_t, QString, QString, double, QString>> constraintList;
    for(int i = 0; i < ui.tableWidget_LightSet->rowCount(); i++)
    {
        int32_t id = -1;
        QString attrName;
        QString constraint;
        double threshold = 0.0;
        QString colorName;
        for(int j = 0; j < ui.tableWidget_LightSet->columnCount(); j++)
        {

            if(j == 2)
            {
                constraint = dynamic_cast<QComboBox*>(ui.tableWidget_LightSet->cellWidget(i, j))
                                 ->currentText();
            }
            else
            {
                auto item = ui.tableWidget_LightSet->item(i, j);
                if(!item)
                {
                    return;
                }
                if(j == 0)
                {
                    id = item->data(Qt::UserRole + 1).toInt();
                }
                if(j == 1)
                {
                    attrName = item->text();
                }
                if(j == 3)
                {
                    threshold = item->text().toDouble();
                }
                if(j == 4)
                {
                    colorName = item->backgroundColor().name();
                }
            }
        }
        constraintList.append(std::make_tuple(id, attrName, constraint, threshold, colorName));
    }
    // 更新的时候才发送信号通知图表刷新 TODO:此处信号会通知所有light图表刷新，是一个错误逻辑，应该针对当前图表进行刷新
    if(!constraintList.isEmpty())
        emit lightConstraintUpdate(constraintList);
}

void AddPlotPair::onBtnCloseClicked()
{
    close();
}
