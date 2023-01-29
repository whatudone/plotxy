#include "AdvancedDataManager.h"
#include "SubSettingWidgetContainer.h"
#include "PlotManagerData.h"

#include <QColorDialog>
#include <QColor>

AdvancedDataManager::AdvancedDataManager(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(QString::fromLocal8Bit("AdvancedDataManager"));
//    this->resize(1440,960);

	SubSettingWidgetContainer* subSettingWidgetContainer = new SubSettingWidgetContainer(this);
	ui.verticalLayout_8->addWidget(subSettingWidgetContainer);

	ui.stackedWidget_aDMrpart->setCurrentIndex(0);
	connect(ui.pushButton_add_2, SIGNAL(clicked()), this, SLOT(onBtnAdd()));

	connect(subSettingWidgetContainer->m_general, SIGNAL(sigBtnGenneralMoreclicked()), this, SLOT(onBtnMore()));
	connect(subSettingWidgetContainer->m_colorRanges, SIGNAL(sigBtnColorRangesMoreclicked()), this, SLOT(onBtnColorMore()));
	connect(subSettingWidgetContainer->m_eventSetting, SIGNAL(sgn_BtnMoreClicked()), this, SLOT(onEventBtnMoreClicked()));

	connect(PlotManagerData::getInstance(), SIGNAL(sgnUpdatePlotManager()), this, SLOT(onUpdatePlotPair()));
	//connect(ui.treeWidget_xy,SIGNAL(currentItemChanged(QTreeWidgetItem * , QTreeWidgetItem * )),
	//		this,SLOT(onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
}

AdvancedDataManager::~AdvancedDataManager()
{

}

void AdvancedDataManager::onCurrentItemChanged(QTreeWidgetItem* currItem, QTreeWidgetItem* preItem)
{
	//清除老的，更新新的
	ui.treeWidget_union->clear();

	QString x_y;
	QString x, y;
	x = currItem->text(0);
	y = currItem->text(1);
	x_y = x + "_" + y;

	for (auto it = m_map[x_y].begin();it != m_map[x_y].end();it++)
	{
		QTreeWidgetItem* addUnion = new QTreeWidgetItem;
		addUnion->setText(1, QString::number(it.key()));

		ui.treeWidget_union->addTopLevelItem(addUnion);
	}
}

void AdvancedDataManager::onBtnAdd()
{
	QString x_y;
	QString x, y;
	//QTreeWidgetItem* item;
	//item = ui.treeWidget_xy->currentItem();
	QList<QTableWidgetItem*> item;
	item = ui.tableWidget_plotpair->selectedItems();
	x = item.at(0)->text();
	y = item.at(1)->text();
	x_y = x + "_" + y;

	QString key = ui.lineEdit_value->text();
	int intKey = key.toInt();
	QPalette pal = ui.pushButton_color->palette();
	QBrush brush = pal.base();
	QColor value = brush.color();

	m_map[x_y].insert(intKey, value);

	QTreeWidgetItem* addUnion = new QTreeWidgetItem;
	addUnion->setText(0, ui.lineEdit_name->text());
	addUnion->setText(1, ui.lineEdit_value->text());
	addUnion->setBackground(2, brush);

	ui.treeWidget_union->addTopLevelItem(addUnion);

	emit updateColorThresholdMap(m_map);
}

void AdvancedDataManager::onBtnMore()
{
	ui.stackedWidget_aDMrpart->setCurrentIndex(1);
}

void AdvancedDataManager::onBtnColorMore()
{
	ui.stackedWidget_aDMrpart->setCurrentIndex(2);
}

void AdvancedDataManager::onEventBtnMoreClicked()
{
	ui.stackedWidget_aDMrpart->setCurrentIndex(3);
}

void AdvancedDataManager::onAddPlot(const QString &tabName, PlotItemBase *plotItem)
{
	//数据层更新
	m_plotManager[tabName].append(plotItem);
}

void AdvancedDataManager::onUpdatePlotPair()
{
	m_plotManager = PlotManagerData::getInstance()->getPlotManagerData();

	if (m_plotManager.isEmpty())
		return;

	ui.tableWidget_plotpair->setRowCount(0);
	for (int i = 0; i < m_plotManager.size(); ++i)
	{
		QString tabString = m_plotManager.keys().at(i);
		for (int j = 0; j < m_plotManager[tabString].size(); ++j)
		{
			PlotItemBase *tempPlot = m_plotManager[tabString].at(j);
			QVector<DataPair*> dataPair = tempPlot->getDataPair();
			for (int k = 0; k < dataPair.size(); ++k)
			{
				//界面更新
				QTableWidgetItem* data1 = new QTableWidgetItem(dataPair[k]->getDataPair().first);
				QTableWidgetItem* data2 = new QTableWidgetItem(dataPair[k]->getDataPair().second);
				QTableWidgetItem* data3 = new QTableWidgetItem(tempPlot->currName());
				QTableWidgetItem* data4 = new QTableWidgetItem(tempPlot->currTabName());
				int row = ui.tableWidget_plotpair->rowCount();
				ui.tableWidget_plotpair->insertRow(row);
				ui.tableWidget_plotpair->setItem(row, 0, data1);
				ui.tableWidget_plotpair->setItem(row, 1, data2);
				ui.tableWidget_plotpair->setItem(row, 2, data3);
				ui.tableWidget_plotpair->setItem(row, 3, data4);
			}
		}
	}
}
