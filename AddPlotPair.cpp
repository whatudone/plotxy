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
	ui.treeWidget_Entity->setHeaderLabel("111");
	ui.treeWidget_Entity->setHeaderHidden(true);

	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));

	connect(ui.treeWidget_Entity, SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem *)), 
			this, SLOT(onEntityTreeWidgetItemClicked(QTreeWidgetItem*, QTreeWidgetItem*)));

}

AddPlotPair::~AddPlotPair()
{
}

void AddPlotPair::init()
{
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
	{
		qDebug() << QString::fromLocal8Bit("尚未加载数据,当前数据为空") << endl;
		QMessageBox::information(NULL, QString::fromLocal8Bit("提示信息"), QString::fromLocal8Bit("尚未加载数据,当前数据为空"));
		return;
	}

	for (auto it = dataMap.begin();it!=dataMap.end();it++)
	{
		QString currEntityType = it.key();
		QTreeWidgetItem* currTreeWidgetItem = new QTreeWidgetItem();
		currTreeWidgetItem->setText(0, currEntityType);
		ui.treeWidget_Entity->addTopLevelItem(currTreeWidgetItem);
	}
}

void AddPlotPair::onBtnAddClicked()
{
	QTreeWidgetItem* treewidgetEntity = new QTreeWidgetItem;
	treewidgetEntity = ui.treeWidget_Entity->currentItem();
	m_entityTypeList.append(treewidgetEntity->text(0));

	QTreeWidgetItem* treewidgetNameUnits = new QTreeWidgetItem;
	treewidgetNameUnits = ui.treeWidget_nameUnits->currentItem();
	m_entityAttrList.append(treewidgetNameUnits->text(0));

	QTreeWidgetItem* addplot = new QTreeWidgetItem;
	addplot->setText(0, treewidgetEntity->text(0) + " " + treewidgetNameUnits->text(0));
	addplot->setText(1, "Time");
	ui.treeWidget_union->addTopLevelItem(addplot);

	emit sigAddPlotPair(treewidgetEntity->text(0), treewidgetNameUnits->text(0));
}

void AddPlotPair::onEntityTreeWidgetItemClicked(QTreeWidgetItem* currItem, QTreeWidgetItem* preItem)
{
	QString currEntityTypeSelected = currItem->text(0);
	auto dataMap = DataManager::getInstance()->getDataMap();
	if (dataMap.isEmpty())
		return;

	if (!dataMap.contains(currEntityTypeSelected))
		return;

	QStringList currEntityAttrList = dataMap.value(currEntityTypeSelected).keys();

	for (int i = 0;i < currEntityAttrList.size();i++)
	{
		QString currEntityAttr = currEntityAttrList.at(i);

		QTreeWidgetItem* currTreeWidgetItem = new QTreeWidgetItem();
		currTreeWidgetItem->setText(0, currEntityAttr);
		ui.treeWidget_nameUnits->addTopLevelItem(currTreeWidgetItem);
	}
}

void AddPlotPair::onBtnCloseClicked()
{
	close();
}
