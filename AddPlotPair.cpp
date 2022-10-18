#include "AddPlotPair.h"
#include "ui_addplotpair.h"
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QDebug>
#include <QMessageBox>
#include "DataManager.h"

AddPlotPair::AddPlotPair(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle("Add Plot Pair");
	ui.tableWidget_Entity->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_2->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_3->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_Attitude1->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_Attitude2->setStyleSheet("QHeaderView::section{background:lightgray;}");

	ui.tableWidget_Entity->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity->verticalHeader()->hide();
	ui.tableWidget_Entity_2->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_2->verticalHeader()->hide();
	ui.tableWidget_Entity_3->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_3->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_3->verticalHeader()->hide();
	ui.tableWidget_Entity_Attitude1->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_Attitude1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude1->verticalHeader()->hide();
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude2->verticalHeader()->hide();

	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));

	connect(ui.tableWidget_Entity, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_3, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude1, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude1(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude2(QTableWidgetItem*)));
}

AddPlotPair::~AddPlotPair()
{
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
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotAScope:
		break;
	case Type_PlotRTI:
		break;
	case Type_PlotText:
		break;
	case Type_PlotLight:
		break;
	case Type_PlotBar:
	case Type_PlotDial:
		ui.stackedWidget->setCurrentIndex(0);
		break;
	case Type_PlotAttitude:
		ui.stackedWidget->setCurrentIndex(2);
		break;
	case Type_PlotPolar:
		break;
	case Type_PlotTrack:
		break;
	case Type_PlotDoppler:
		break;
	default:
		ui.stackedWidget->setCurrentIndex(1);
		break;
	}
}

void AddPlotPair::onBtnAddClicked()
{
	int index = ui.stackedWidget->currentIndex();
	QString strEntity1, strNameUnit1, strSum1, strEntity2, strNameUnit2, strSum2;
	switch (index)
	{
	case 0:
		strEntity1 = ui.tableWidget_Entity->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0)->text();
		strSum1 = strEntity1 + " " + strNameUnit1;
		strSum2 = "Time";

		m_entityTypeList.append(strEntity1);
		m_entityAttrList.append(strNameUnit1);
		emit sigAddPlotPair(strEntity1, strNameUnit1);
		break;
	case 1:
		strEntity1 = ui.tableWidget_Entity_2->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0)->text();
		strSum1 = strEntity1 + " " + strNameUnit1;
		strEntity2 = ui.tableWidget_Entity_3->currentItem()->text();
		strNameUnit2 = ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0)->text();
		strSum2 = strEntity2 + " " + strNameUnit2;

		m_entityTypeList.append(strEntity1);
		m_entityAttrList.append(strNameUnit1);
//		emit sigAddPlotPair(strEntity1, strNameUnit1);
		m_entityTypeList.append(strEntity2);
		m_entityAttrList.append(strNameUnit2);
//		emit sigAddPlotPair(strEntity2, strNameUnit2);
		break;
	case 2:
		strEntity1 = ui.tableWidget_Entity_Attitude1->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0)->text();
		strSum1 = strEntity1 + " " + strNameUnit1;
		strEntity2 = ui.tableWidget_Entity_Attitude2->currentItem()->text();
		strNameUnit2 = ui.tableWidget_nameUnits_Attitude2->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0)->text();
		strSum2 = strEntity2 + " " + strNameUnit2;

		m_entityTypeList.append(strEntity1);
		m_entityAttrList.append(strNameUnit1);
//		emit sigAddPlotPair(strEntity1, strNameUnit1);
		m_entityTypeList.append(strEntity2);
		m_entityAttrList.append(strNameUnit2);
//		emit sigAddPlotPair(strEntity2, strNameUnit2);
		break;
	}
	
	QTableWidgetItem* addplot1 = new QTableWidgetItem(strSum1);
	QTableWidgetItem* addplot2 = new QTableWidgetItem(strSum2);
	int row = ui.tableWidget_union->rowCount();
	ui.tableWidget_union->insertRow(row);
	ui.tableWidget_union->setItem(row, 0, addplot1);
	ui.tableWidget_union->setItem(row, 1, addplot2);	
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
	ui.tableWidget_Entity_Attitude1->setRowCount(icount);
	ui.tableWidget_Entity_Attitude2->setRowCount(icount);

	for (auto it = dataMap.begin(); it != dataMap.end(); it++)
	{
		QString currEntityType = it.key();
		QTableWidgetItem* item = new QTableWidgetItem(currEntityType);
		ui.tableWidget_Entity->setItem(index, 0, item);
		ui.tableWidget_Entity_2->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_3->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude1->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude2->setItem(index, 0, new QTableWidgetItem(*item));

		index ++;
	}
}

void AddPlotPair::onBtnCloseClicked()
{
	close();
}
