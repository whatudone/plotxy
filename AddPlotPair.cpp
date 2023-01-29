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
#include "PlotLight.h"
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
	initStackedWidget_page5();
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

void AddPlotPair::initStackedWidget_page5()
{
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
		ui.groupBox_3->setTitle(QString::fromLocal8Bit("X������Դ"));
		ui.groupBox_2->setTitle(QString::fromLocal8Bit("Y������Դ"));
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotPolar:
		ui.groupBox_3->setTitle(QString::fromLocal8Bit("�Ƕ�����Դ"));
		ui.groupBox_2->setTitle(QString::fromLocal8Bit("��������Դ"));
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotAScope:
		ui.label_XAxis->setText(QString::fromLocal8Bit("X�᣺Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y�᣺Voltage(v)"));
		ui.stackedWidget->setCurrentIndex(5);
		break;
	case Type_PlotRTI:
		ui.label_XAxis->setText(QString::fromLocal8Bit("X�᣺Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y�᣺Time(s)"));
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
		ui.label_XAxis->setText(QString::fromLocal8Bit("X�᣺Range(m)"));
		ui.label_YAxis->setText(QString::fromLocal8Bit("Y�᣺Range(m)"));
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
		qDebug() << QString::fromLocal8Bit("��δ��������,��ǰ����Ϊ��") << endl;
		QMessageBox::information(NULL, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("��δ��������,��ǰ����Ϊ��"));
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
					//�������
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
			if (ui.tableWidget_LightSet->item(j, i) == nullptr)
			{
				QMessageBox::critical(NULL, QString::fromLocal8Bit("��ʾ��Ϣ"), QString::fromLocal8Bit("�뽫������д����"));
				return;
			}
			else
				rowLightData.push_back(ui.tableWidget_LightSet->item(j, i)->text());
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
	int iRow = ui.tableWidget_LightSet->rowCount();
	ui.tableWidget_LightSet->setRowCount(iRow + 1);
}

void AddPlotPair::onBtnLightDeleteClicked()
{
	int rowIdx = ui.tableWidget_LightSet->currentRow();
	if (rowIdx != -1)
	{
		if (rowIdx == 0)
			return;
		else
			ui.tableWidget_LightSet->removeRow(rowIdx);
	}
}

void AddPlotPair::onBtnCloseClicked()
{
	close();
}


