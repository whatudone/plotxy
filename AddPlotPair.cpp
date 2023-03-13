#include "AddPlotPair.h"
#include "ui_addplotpair.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QSet>
#include <qradiobutton>
#include <QPoint>
#include <QSpinBox>
#include <QMessageBox>
#include <QStyleFactory>
#include "DataManager.h"
#include "PlotManagerData.h"
AddPlotPair* AddPlotPair::thispoint = nullptr;
AddPlotPair* AddPlotPair::m_getInstance()
{
	if (thispoint == nullptr)
		thispoint = new AddPlotPair;
	return thispoint;
}

AddPlotPair::AddPlotPair(QWidget *parent) :
	QWidget(parent)
{
	ui.setupUi(this);
	this->setWindowTitle("Add Plot Pair");

	initStackedWidget_page1();
	initStackedWidget_page2();
	initStackedWidget_page3();
	initStackedWidget_page4();
	initStackedWidget_pageLight();
	initStackedWidget_pageAScope();


	initTreePlot();

	connect(PlotManagerData::getInstance(), SIGNAL(sgnUpdatePlotManager()), this, SLOT(updatePlotTrees()));

	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));
	connect(ui.pushButton_remove, SIGNAL(clicked()), this, SLOT(onBtnRemoveClicked()));
	connect(ui.pushButton_update, SIGNAL(clicked()), this, SLOT(onBtnUpdateClicked()));

}

AddPlotPair::~AddPlotPair()
{
}

void AddPlotPair::initStackedWidget_page1()
{
	ui.tableWidget_Entity->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity->verticalHeader()->hide();
	ui.tableWidget_Entity_9->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_10->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_9->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_9->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_9->verticalHeader()->hide();
	ui.tableWidget_Entity_10->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_10->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_10->verticalHeader()->hide();

	connect(ui.tableWidget_Entity, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
	connect(ui.radioButton, &QRadioButton::clicked, this, [=] {ui.stackedWidget_4->setCurrentIndex(0); });
	connect(ui.radioButton_2, &QRadioButton::clicked, this, [=] {ui.stackedWidget_4->setCurrentIndex(1); });

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
	ui.tableWidget_Entity_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_2->verticalHeader()->hide();
	ui.tableWidget_Entity_3->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_3->verticalHeader()->hide();

	ui.tableWidget_Entity_4->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_4->verticalHeader()->hide();
	ui.tableWidget_Entity_5->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_5->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_5->verticalHeader()->hide();

	ui.tableWidget_Entity_5->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_5->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_5->verticalHeader()->hide();
	ui.tableWidget_Entity_6->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_6->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_6->verticalHeader()->hide();
	ui.tableWidget_Entity_7->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_7->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_7->verticalHeader()->hide();
	ui.tableWidget_Entity_8->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_8->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_8->verticalHeader()->hide();

	connect(ui.tableWidget_Entity_2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_3, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));

	connect(ui.radioButton_7, &QRadioButton::clicked, this, [=] {ui.stackedWidget_2->setCurrentIndex(0); });
	connect(ui.radioButton_5, &QRadioButton::clicked, this, [=] {ui.stackedWidget_2->setCurrentIndex(1); });
	connect(ui.radioButton_6, &QRadioButton::clicked, this, [=] {ui.stackedWidget_2->setCurrentIndex(2); });

	connect(ui.radioButton_8, &QRadioButton::clicked, this, [=] {ui.stackedWidget_3->setCurrentIndex(0); });
	connect(ui.radioButton_3, &QRadioButton::clicked, this, [=] {ui.stackedWidget_3->setCurrentIndex(1); });
	connect(ui.radioButton_4, &QRadioButton::clicked, this, [=] {ui.stackedWidget_3->setCurrentIndex(2); });

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
	ui.tableWidget_Entity_Attitude1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude1->verticalHeader()->hide();
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude2->verticalHeader()->hide();

	connect(ui.tableWidget_Entity, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_3, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_4, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_4(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude1, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude1(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude2(QTableWidgetItem*)));
}

void AddPlotPair::initStackedWidget_page4()
{
	ui.tableWidget_Entity_4->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_4->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_4->verticalHeader()->hide();

	connect(ui.tableWidget_Entity_4, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_4(QTableWidgetItem*)));
	connect(ui.radioButton_12, &QRadioButton::clicked, this, [=] {ui.stackedWidget_5->setCurrentIndex(0); });
	connect(ui.radioButton_9, &QRadioButton::clicked, this, [=] {ui.stackedWidget_5->setCurrentIndex(1); });
	connect(ui.radioButton_10, &QRadioButton::clicked, this, [=] {ui.stackedWidget_5->setCurrentIndex(2); });
	connect(ui.radioButton_11, &QRadioButton::clicked, this, [=] {ui.stackedWidget_5->setCurrentIndex(3); });

	ui.radioButton_9->clicked();
	ui.radioButton_9->setChecked(true);
}

void AddPlotPair::initStackedWidget_pageLight()
{
	QComboBox *comboBoxEntity = new QComboBox;
	QComboBox *comboBoxAttr = new QComboBox;
	QComboBox *comboBoxCompare = new QComboBox;
	QComboBox *comboBoxColor = new QComboBox;
	comboBoxEntity->addItem("Target+Altitude");
	comboBoxEntity->addItem("Target2+Mach");
	ui.tableWidget_LightSet->setCellWidget(0, 0, comboBoxEntity);
	comboBoxAttr->addItem("Time");
	ui.tableWidget_LightSet->setCellWidget(0, 1, comboBoxAttr);
	comboBoxCompare->addItem(QString::fromLocal8Bit("≥"));
	comboBoxCompare->addItem("<");
	ui.tableWidget_LightSet->setCellWidget(0, 2, comboBoxCompare);
	comboBoxColor->addItem("G/R/Y");
	ui.tableWidget_LightSet->setCellWidget(0, 4, comboBoxColor);

	ui.tableWidget_lightEntity->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_lightEntity->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_lightEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_lightEntity->verticalHeader()->hide();
	ui.tableWidget_lightNameUnits->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.radioButton_lightParameter->setChecked(true);
	ui.tableWidget_LightSet->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	connect(ui.tableWidget_lightEntity, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetLightEntityClicked(QTableWidgetItem*)));
	connect(ui.pushButton_LightAdd, SIGNAL(clicked()), this, SLOT(onBtnLightAddClicked()));
	connect(ui.pushButton_LightDelete, SIGNAL(clicked()), this, SLOT(onBtnLightDeleteClicked()));
	connect(ui.pushButton_LightUpdate, SIGNAL(clicked()), this, SLOT(onBtnLightUpdateClicked()));
}

void AddPlotPair::initStackedWidget_pageAScope()
{
	ui.tableWidget_AScopeEntity->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_AScopeEntity->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_AScopeEntity->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
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

	connect(m_treePlot, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onDoubleClickedTreeWidgetItem(QTreeWidgetItem*, int)));
}

void AddPlotPair::init(PlotType index)
{
	onChangeStackIndex(index);
	onUpdateData();
}

void AddPlotPair::onChangeStackIndex(PlotType index)
{
	switch (index)
	{
	case Type_PlotScatter:
		ui.groupBox_3->setTitle(QString::fromLocal8Bit("X轴数据源"));
		ui.groupBox_2->setTitle(QString::fromLocal8Bit("Y轴数据源"));
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotPolar:
		ui.groupBox_3->setTitle(QString::fromLocal8Bit("角度数据源"));
		ui.groupBox_2->setTitle(QString::fromLocal8Bit("幅度数据源"));
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotAScope:
		ui.label_XAxis->setText(QString::fromLocal8Bit("X轴：Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y轴：Voltage(v)"));
		ui.stackedWidget->setCurrentIndex(5);
		break;
	case Type_PlotRTI:
		ui.label_XAxis->setText(QString::fromLocal8Bit("X轴：Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y轴：Time(s)"));
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
	case Type_PlotDoppler:
		ui.label_XAxis->setText(QString::fromLocal8Bit("X轴：Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y轴：Range(m)"));
		ui.stackedWidget->setCurrentIndex(5);
		break;
	default:
		ui.stackedWidget->setCurrentIndex(0);
		break;
	}
}

void AddPlotPair::setPlotBaseInfo(BaseInfo info)
{
	memcpy(&m_curPlotInfo, &info, sizeof(BaseInfo));

	QList<QTreeWidgetItem*> plotItems = m_treePlot->findItems(m_curPlotInfo.Base_PlotName, Qt::MatchCaseSensitive | Qt::MatchRecursive, 0);
	if (plotItems.size() != 0)
	{
		m_treePlot->itemDoubleClicked(plotItems[0], 0);
	}
}

void AddPlotPair::updatePlotTrees()
{
	m_plotManager = PlotManagerData::getInstance()->getPlotManagerData();
	if (m_plotManager.isEmpty())
		return;

	m_treePlot->clear();

	for (int i = 0; i < m_plotManager.size(); ++i)
	{
		QString tabString = m_plotManager.keys().at(i);
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabString);
		m_treePlot->addTopLevelItem(itemselPlotH);
		m_treePlot->expandAll();

		for (int j = 0; j < m_plotManager[tabString].size(); ++j)
		{
			QString plotString = m_plotManager[tabString].at(j)->currName();
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotString);
			itemselPlotH->addChild(itemselPlotI);
		}
		//m_curPlotInfo.Base_TabName = tabString; 这个玩意需要更新下
	}
}

PlotType AddPlotPair::getPlotType(PlotItemBase *plotItem)
{
	QString name = plotItem->metaObject()->className();
	PlotType type;
	if (name.compare("PlotPlotScatter") == 0)
	{
		type = PlotType::Type_PlotScatter;
	}
	else if (name.compare("PlotAScope") == 0)
	{
		type = PlotType::Type_PlotAScope;
	}
	else if (name.compare("PlotRTI") == 0)
	{
		type = PlotType::Type_PlotRTI;
	}
	else if (name.compare("PlotText") == 0)
	{
		type = PlotType::Type_PlotText;
	}
	else if (name.compare("PlotLight") == 0)
	{
		type = PlotType::Type_PlotLight;
	}
	else if (name.compare("PlotBar") == 0)
	{
		type = PlotType::Type_PlotBar;
	}
	else if (name.compare("PlotDial") == 0)
	{
		type = PlotType::Type_PlotDial;
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		type = PlotType::Type_PlotAttitude;
	}
	else if (name.compare("PlotPolar") == 0)
	{
		type = PlotType::Type_PlotPolar;
	}
	else if (name.compare("PlotTrack") == 0)
	{
		type = PlotType::Type_PlotTrack;
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		type = PlotType::Type_PlotDoppler;
	}
	else
		type = PlotType::Type_PlotScatter;

	return type;
}

bool AddPlotPair::getCurrentSelectParam(QString &strSum1, QString &strSum2)
{
	int index = ui.stackedWidget->currentIndex();
	QString strEntity1, strNameUnit1, strEntity2, strNameUnit2, strSum3;
	QPair<QString, QString> p1, p2;

	switch (index)
	{
	case 0:
		if (ui.radioButton->isChecked())
		{
			if (ui.tableWidget_Entity->currentItem() == NULL || ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0) == NULL)
				return false;

			strEntity1 = ui.tableWidget_Entity->currentItem()->text();
			strNameUnit1 = ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0)->text();

			//			strSum1 = strEntity1 + "+" + strNameUnit1;
		}
		else if (ui.radioButton_2->isChecked())
		{
			if (ui.tableWidget_Entity_9->currentItem() == NULL || ui.tableWidget_Entity_10->currentItem() == NULL)
				return false;

			strEntity1 = ui.tableWidget_Entity_9->currentItem()->text();
			strNameUnit1 = ui.tableWidget_Entity_10->currentItem()->text();
			//			strSum1 = strEntity1 + "+" + strNameUnit1;
		}

		strSum1 = strEntity1 + "+" + strNameUnit1;
		strSum2 = "Time";

		//		emit sigAddPlotPair(strEntity1, strNameUnit1);
		break;

	case 1:
		if (ui.radioButton_7->isChecked())
		{
			strSum1 = "Time";
		}
		else if (ui.radioButton_5->isChecked())
		{
			if (ui.tableWidget_Entity_2->currentItem() == NULL || ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0) == NULL)
				return false;

			strEntity1 = ui.tableWidget_Entity_2->currentItem()->text();
			strNameUnit1 = ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0)->text();
			strSum1 = strEntity1 + "+" + strNameUnit1;
		}
		else if (ui.radioButton_6->isChecked())
		{
			if (ui.tableWidget_Entity_5->currentItem() == NULL || ui.tableWidget_Entity_6->currentItem() == NULL)
				return false;

			strEntity1 = ui.tableWidget_Entity_5->currentItem()->text();
			strNameUnit1 = ui.tableWidget_Entity_6->currentItem()->text();
			strSum1 = strEntity1 + "+" + strNameUnit1;
		}

		if (ui.radioButton_8->isChecked())
		{
			strSum2 = "Time";
		}
		else if (ui.radioButton_3->isChecked())
		{
			if (ui.tableWidget_Entity_3->currentItem() == NULL || ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0) == NULL)
				return false;

			strEntity2 = ui.tableWidget_Entity_3->currentItem()->text();
			strNameUnit2 = ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0)->text();
			strSum2 = strEntity2 + "+" + strNameUnit2;
		}
		else if (ui.radioButton_4->isChecked())
		{
			if (ui.tableWidget_Entity_7->currentItem() == NULL || ui.tableWidget_Entity_8->currentItem() == NULL)
				return false;

			strEntity2 = ui.tableWidget_Entity_7->currentItem()->text();
			strNameUnit2 = ui.tableWidget_Entity_8->currentItem()->text();
			strSum2 = strEntity2 + "+" + strNameUnit2;
		}
		break;
	case 2:
		if (ui.tableWidget_Entity_Attitude1->currentItem() == NULL || ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0) == NULL ||
			ui.tableWidget_Entity_Attitude2->currentItem() == NULL || ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0) == NULL)
			return false;

		strEntity1 = ui.tableWidget_Entity_Attitude1->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0)->text();
		strSum1 = strEntity1 + "+" + strNameUnit1;
		strEntity2 = ui.tableWidget_Entity_Attitude2->currentItem()->text();
		strNameUnit2 = ui.tableWidget_nameUnits_Attitude2->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0)->text();
		strSum2 = strEntity2 + "+" + strNameUnit2;

		break;
	case 3:
		if (ui.radioButton_12->isChecked())
		{
			strSum1 = "Time";
		}
		else if (ui.radioButton_9->isChecked())
		{
			if (ui.tableWidget_Entity_4->currentItem() == NULL || ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0) == NULL)
				return false;

			strEntity1 = ui.tableWidget_Entity_4->currentItem()->text();
			strNameUnit1 = ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0)->text();

			strSum1 = strEntity1 + "+" + strNameUnit1;
			strSum2 = "Time";
			emit sgn_onTextLightBtnClicked();
		}
		else if (ui.radioButton_10->isChecked())
		{

		}
		else if (ui.radioButton_11->isChecked())
		{

		}
		break;
	case 4:
	{
		if (ui.radioButton_lightParameter->isChecked())
		{
			if (ui.tableWidget_lightEntity->currentItem() == NULL || ui.tableWidget_lightNameUnits->item(ui.tableWidget_lightNameUnits->currentRow(), 0) == NULL)
				return false;
			strEntity1 = ui.tableWidget_lightEntity->currentItem()->text();
			strNameUnit1 = ui.tableWidget_lightNameUnits->item(ui.tableWidget_lightNameUnits->currentRow(), 0)->text();

			strSum1 = strEntity1 + "+" + strNameUnit1;
			strSum2 = "Time";
		}
		break;
	}
	default:
		break;
	}
	return true;
}

void AddPlotPair::onBtnAddClicked()
{
	QString strSum1, strSum2;
	QPair<QString, QString> p1;

	if (false == getCurrentSelectParam(strSum1, strSum2))
		return;

	p1 = qMakePair(strSum1, strSum2);

	if (m_plotManager.contains(m_curPlotInfo.Base_TabName))
	{
		for (int i = 0; i < m_plotManager[m_curPlotInfo.Base_TabName].size(); ++i)
		{
			PlotItemBase* tempPlot = m_plotManager[m_curPlotInfo.Base_TabName].at(i);
			if (m_curPlotInfo.Base_PlotName == tempPlot->currName())
			{
				tempPlot->addPlotPairData(p1);
				break;
			}
		}
//		emit sgn_updatePlotPair(m_curPlotInfo.Base_TabName, m_curPlotInfo.Base_PlotName);
	}

// 	QTableWidgetItem* addplot1 = new QTableWidgetItem(strSum1);
// 	QTableWidgetItem* addplot2 = new QTableWidgetItem(strSum2);
// 	int row = ui.tableWidget_union->rowCount();
// 	ui.tableWidget_union->insertRow(row);
// 	ui.tableWidget_union->setItem(row, 0, addplot1);
// 	ui.tableWidget_union->setItem(row, 1, addplot2);
}

void AddPlotPair::onTableWidgetItemClicked(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetItemClicked_2(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits_2->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits_2->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetItemClicked_3(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits_3->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits_3->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetItemClicked_4(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits_4->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits_4->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetLightEntityClicked(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_lightNameUnits->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_lightNameUnits->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetItemClicked_Attitude1(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits_Attitude1->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits_Attitude1->setItem(i, 0, item);
	}
}

void AddPlotPair::onTableWidgetItemClicked_Attitude2(QTableWidgetItem * curItem)
{
	QString currEntityTypeSelected = curItem->text();
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	ui.tableWidget_nameUnits_Attitude2->setRowCount(currEntityAttrList.size());
	for (int i = 0; i < currEntityAttrList.size(); i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTableWidgetItem* item = new QTableWidgetItem(currEntityAttr);
		ui.tableWidget_nameUnits_Attitude2->setItem(i, 0, item);
	}
}

void AddPlotPair::onUpdateData()
{
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
	{
		qDebug() << QString::fromLocal8Bit("尚未加载数据,当前数据为空") << endl;
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示信息"), QString::fromLocal8Bit("尚未加载数据,当前数据为空"));
		return;
	}
	int index = 0;
	int icount = dataMap.size();
	ui.tableWidget_Entity->setRowCount(icount);
	ui.tableWidget_Entity_2->setRowCount(icount);
	ui.tableWidget_Entity_3->setRowCount(icount);
	ui.tableWidget_Entity_4->setRowCount(icount);
	ui.tableWidget_Entity_5->setRowCount(icount);
	ui.tableWidget_Entity_6->setRowCount(icount);
	ui.tableWidget_Entity_7->setRowCount(icount);
	ui.tableWidget_Entity_8->setRowCount(icount);
	ui.tableWidget_Entity_9->setRowCount(icount);
	ui.tableWidget_Entity_10->setRowCount(icount);
	ui.tableWidget_Entity_Attitude1->setRowCount(icount);
	ui.tableWidget_Entity_Attitude2->setRowCount(icount);
	ui.tableWidget_lightEntity->setRowCount(icount);

	for (auto it = dataMap.begin(); it != dataMap.end(); it++)
	{
		QString currEntityType = it.key();
		QTableWidgetItem* item = new QTableWidgetItem(currEntityType);
		ui.tableWidget_Entity->setItem(index, 0, item);
		ui.tableWidget_Entity_2->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_3->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_4->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_5->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_lightEntity->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_6->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_7->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_8->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_9->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_10->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude1->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude2->setItem(index, 0, new QTableWidgetItem(*item));

		index++;
	}
}


void AddPlotPair::onAddPlot(const QString &tabName, PlotItemBase *plotItem)
{
	//数据层更新
	m_plotManager[tabName].append(plotItem);

	updatePlotTrees();
}

void AddPlotPair::onDoubleClickedTreeWidgetItem(QTreeWidgetItem * item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent)
		return;

	QString parent_text = parent->text(0);
	QString child_text = item->text(column);
	ui.toolButton_plot->setText(child_text);

	if (m_plotManager.contains(parent_text))
	{
		for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
		{
			PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
			if (child_text == tempPlot->currName())
			{
				onChangeStackIndex(getPlotType(tempPlot));
				m_curPlotInfo.Base_TabName = parent_text;
				m_curPlotInfo.Base_PlotName = child_text;

				ui.tableWidget_union->setRowCount(0);

				QVector<DataPair*> dataPair = tempPlot->getDataPair();
				for (int k = 0; k < dataPair.size(); ++k)
				{
					//界面更新
					QTableWidgetItem* addplot1 = new QTableWidgetItem(dataPair[k]->getDataPair().first);
					QTableWidgetItem* addplot2 = new QTableWidgetItem(dataPair[k]->getDataPair().second);
					int row = ui.tableWidget_union->rowCount();
					ui.tableWidget_union->insertRow(row);
					ui.tableWidget_union->setItem(row, 0, addplot1);
					ui.tableWidget_union->setItem(row, 1, addplot2);
				}
				break;
			}
		}
	}
}

void AddPlotPair::onBtnUpdateClicked()
{
	int row = ui.tableWidget_union->currentRow();
	if (row != -1)
	{
		QString strSum1, strSum2;
		QPair<QString, QString> pOld, pNew;

		QString str1 = ui.tableWidget_union->item(row, 0)->text();
		QString str2 = ui.tableWidget_union->item(row, 1)->text();
		pOld = qMakePair(str1, str2);

		if (false == getCurrentSelectParam(strSum1, strSum2))
			return;

		pNew = qMakePair(strSum1, strSum2);

// 		ui.tableWidget_union->item(row, 0)->setText(strSum1);
// 		ui.tableWidget_union->item(row, 1)->setText(strSum2);

		if (m_plotManager.contains(m_curPlotInfo.Base_TabName))
		{
			for (int i = 0; i < m_plotManager[m_curPlotInfo.Base_TabName].size(); ++i)
			{
				PlotItemBase* tempPlot = m_plotManager[m_curPlotInfo.Base_TabName].at(i);
				if (m_curPlotInfo.Base_PlotName == tempPlot->currName())
				{
					tempPlot->updatePlotPairData(pOld, pNew);
					break;
				}
			}
		//	emit sgn_updatePlotPair(m_curPlotInfo.Base_TabName, m_curPlotInfo.Base_PlotName);
		}
	}
}

void AddPlotPair::onBtnRemoveClicked()
{
	int row = ui.tableWidget_union->currentRow();
	if (row != -1)
	{
		QString str1 = ui.tableWidget_union->item(row, 0)->text();
		QString str2 = ui.tableWidget_union->item(row, 1)->text();

		QPair<QString, QString> pair = qMakePair(str1, str2);

		if (m_plotManager.contains(m_curPlotInfo.Base_TabName))
		{
			for (int i = 0; i < m_plotManager[m_curPlotInfo.Base_TabName].size(); ++i)
			{
				PlotItemBase* tempPlot = m_plotManager[m_curPlotInfo.Base_TabName].at(i);
				if (m_curPlotInfo.Base_PlotName == tempPlot->currName())
				{
					tempPlot->delPlotPairData(pair);
					break;
				}
			}
		//	emit sgn_updatePlotPair(m_curPlotInfo.Base_TabName, m_curPlotInfo.Base_PlotName);
		}

	//	ui.tableWidget_union->removeRow(row);
	}
}

void AddPlotPair::onBtnLightUpdateClicked()
{
	QList<QList<QString>> lightData;
	QList<QString> rowLightData;
	for (int j = 0; j < ui.tableWidget_LightSet->rowCount(); j++)
	{
		for (int i = 0; i < ui.tableWidget_LightSet->columnCount(); i++)
		{
			QString temData;
			if (i == 3)
			{		
				if (ui.tableWidget_LightSet->item(j,3) == nullptr)
				{
					QString changeLine = "\n";
					QString text1 = QString::fromLocal8Bit("请将数据填写完整");
					QString text3 = QString::fromLocal8Bit("第4列存在空白");
					QString warningInfo = text1 + changeLine + text3 ;
					QMessageBox::critical(NULL, QString::fromLocal8Bit("提示信息"), warningInfo);
					return;
				}
				temData = ui.tableWidget_LightSet->item(j, 3)->text();
				rowLightData.push_back(temData);
			}
			else if (i == 1)
				continue;
			else
			{
				temData = dynamic_cast<QComboBox *>(ui.tableWidget_LightSet->cellWidget(j, i))->currentText();
				if (temData == "")
				{
					QMessageBox::critical(NULL, QString::fromLocal8Bit("提示信息"), QString::fromLocal8Bit("请将数据填写完整"));
					return;
				}
				else
					rowLightData.push_back(temData);
			}
		}
		lightData.push_back(rowLightData);
	}
	if (lightData.size() > 1)
		emit sgn_getLightData(lightData);
}

void AddPlotPair::onUpdatePlotPair(QString tabName, QString plotName)
{
	if (tabName == nullptr || plotName == nullptr)
		return;

	QList<QTreeWidgetItem*> items = m_treePlot->findItems(plotName, Qt::MatchExactly | Qt::MatchRecursive);
	if (items.size() != 0)
	{
		for each (QTreeWidgetItem* item in items)
		{
			if (item->parent() != NULL && item->parent()->text(0) == tabName)
			{
				m_treePlot->itemDoubleClicked(item, 0);

				break;
			}
		}
	}
}

void AddPlotPair::onBtnLightAddClicked()
{
	QList<QString> redYellowGreen;
	redYellowGreen.push_back("Red");
	redYellowGreen.push_back("Yellow");
	redYellowGreen.push_back("Green");
	int iRow = ui.tableWidget_LightSet->rowCount();
	ui.tableWidget_LightSet->setRowCount(iRow + 1);
	QComboBox *newCount0 = new QComboBox;
	QComboBox *newCount1 = new QComboBox;
	QComboBox *newCount2 = new QComboBox;
	QComboBox *newCount4 = new QComboBox;
	QPair<QString, QString> temPair;
	QString temEntity;
	QString temAttr;
	QString parent_text = m_curPlotInfo.Base_TabName;
	QString child_text = m_curPlotInfo.Base_PlotName;
	for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
	{
		PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
		QVector<DataPair*> temDataPair = tempPlot->getDataPair();
		for (int i = 0; i < temDataPair.size(); i++)
		{
			temPair = temDataPair.at(i)->getDataPair();
			temEntity = temPair.first;
			newCount0->addItem(temEntity);
		}
		ui.tableWidget_LightSet->setCellWidget(iRow, 0, newCount0);
	}
	newCount1->addItem("Time");
	newCount2->addItem(QString::fromLocal8Bit("≥"));
	newCount2->addItem("<");
	newCount4->addItems(redYellowGreen);
	ui.tableWidget_LightSet->setCellWidget(iRow, 1, newCount1);
	ui.tableWidget_LightSet->setCellWidget(iRow , 2, newCount2);
	ui.tableWidget_LightSet->setCellWidget(iRow , 4, newCount4);
}

void AddPlotPair::onBtnLightDeleteClicked()
{
	int rowIdx = ui.tableWidget_LightSet->currentRow();
	if (rowIdx != -1)
	{
		if (rowIdx == 0)
		{
			QMessageBox *temMessage = new QMessageBox(nullptr);
			temMessage->setText(QString::fromLocal8Bit("示例行请勿删除"));
			temMessage->show();
		}
		else
			ui.tableWidget_LightSet->removeRow(rowIdx);
	}
	else
	{
		QMessageBox *temMessage = new QMessageBox(nullptr);
		temMessage->setText(QString::fromLocal8Bit("请先点选要删除的行"));
		temMessage->show();
	}
	ui.tableWidget_LightSet->setCurrentCell(0, 0);
}

void AddPlotPair::onBtnCloseClicked()
{
	close();
}


