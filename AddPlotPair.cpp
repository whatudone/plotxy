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
}

AddPlotPair::~AddPlotPair()
{
}

void AddPlotPair::init(PlotType index)
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

	updateData();
}

void AddPlotPair::onBtnAddClicked()
{
	QString str = ui.tableWidget_Entity->currentItem()->text();
	QString str1 = ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0)->text();
	m_entityTypeList.append(str);
	m_entityAttrList.append(str1);

	QTableWidgetItem* addplot1 = new QTableWidgetItem(str + " " + str1);
	QTableWidgetItem* addplot2 = new QTableWidgetItem("Time");
	int row = ui.tableWidget_union->rowCount();
	ui.tableWidget_union->insertRow(row);
	ui.tableWidget_union->setItem(row, 0, addplot1);
	ui.tableWidget_union->setItem(row, 1, addplot2);

	emit sigAddPlotPair(str, str1);
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

void AddPlotPair::updateData()
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
		ui.tableWidget_Entity_2->setItem(index, 0, item);
		ui.tableWidget_Entity_3->setItem(index, 0, item);
		ui.tableWidget_Entity_Attitude1->setItem(index, 0, item);
		ui.tableWidget_Entity_Attitude2->setItem(index, 0, item);

		index ++;
	}
}

void AddPlotPair::onBtnCloseClicked()
{
	close();
}
