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

	ui.pushButton_copy->setEnabled(false);
	ui.pushButton_autofit->setEnabled(false);
	ui.pushButton_delete->setEnabled(false);

	SubSettingWidgetContainer* subSettingWidgetContainer = new SubSettingWidgetContainer(this);
	ui.verticalLayout_8->addWidget(subSettingWidgetContainer);

	ui.stackedWidget_aDMrpart->setCurrentIndex(0);
	connect(ui.pushButton_add_2, SIGNAL(clicked()), this, SLOT(onBtnAdd()));
	connect(ui.pushButton_add, &QPushButton::clicked, this, &AdvancedDataManager::onPushButton_addClicked);
	connect(ui.pushButton_copy, &QPushButton::clicked, this, &AdvancedDataManager::onPushButton_copyClicked);
	connect(ui.pushButton_autofit, &QPushButton::clicked, this, &AdvancedDataManager::onPushButton_autofitClicked);
	connect(ui.pushButton_delete, &QPushButton::clicked, this, &AdvancedDataManager::onPushButton_deleteClicked);
	connect(ui.tableWidget_plotpair, &QTableWidget::itemSelectionChanged, this, &AdvancedDataManager::onTableWidget_plotpairItemSelectionChanged);

	connect(subSettingWidgetContainer->m_general, SIGNAL(sigBtnGenneralMoreclicked()), this, SLOT(onBtnMore()));
	connect(subSettingWidgetContainer->m_colorRanges, SIGNAL(sigBtnColorRangesMoreclicked()), this, SLOT(onBtnColorMore()));
	connect(subSettingWidgetContainer->m_eventSetting, SIGNAL(sgn_BtnMoreClicked()), this, SLOT(onEventBtnMoreClicked()));

	connect(PlotManagerData::getInstance(), SIGNAL(sgnUpdatePlotManager()), this, SLOT(onUpdatePlotPair()));
}

AdvancedDataManager::~AdvancedDataManager()
{

}

void AdvancedDataManager::onTableWidget_plotpairItemSelectionChanged()
{
	int row = ui.tableWidget_plotpair->currentRow();
	if (row < 0)
	{
		m_curSelectPlot = nullptr;
		m_curSelectDatapair = nullptr;
		ui.pushButton_copy->setEnabled(false);
		ui.pushButton_autofit->setEnabled(false);
		ui.pushButton_delete->setEnabled(false);
		return;
	}

	ui.pushButton_copy->setEnabled(true);
	ui.pushButton_autofit->setEnabled(true);
	ui.pushButton_delete->setEnabled(true);

	QString xText = ui.tableWidget_plotpair->item(row, 0)->text();
	QString yText = ui.tableWidget_plotpair->item(row, 1)->text();
	QString plotName = ui.tableWidget_plotpair->item(row, 2)->text();
	QString tabName = ui.tableWidget_plotpair->item(row, 3)->text();
	QPair<QString, QString> tempPair = qMakePair(xText, yText);

	if (m_plotManager.contains(tabName))
	{
		for (int i = 0; i < m_plotManager[tabName].size(); ++i)
		{
			PlotItemBase* tempPlot = m_plotManager[tabName].at(i);
			if (plotName == tempPlot->currName())
			{
				m_curSelectPlot = tempPlot;
				for (int k = 0; k < m_curSelectPlot->getDataPair().size(); ++k)
				{
					if (m_curSelectPlot->getDataPair().at(k)->getDataPair() == tempPair)
					{
						m_curSelectDatapair = m_curSelectPlot->getDataPair().at(k);
					}
				}
			}
		}
	}
}

void AdvancedDataManager::onPushButton_addClicked()
{
	emit sgnAddPlotPair();
}

void AdvancedDataManager::onPushButton_copyClicked()
{
}

void AdvancedDataManager::onPushButton_autofitClicked()
{
}

void AdvancedDataManager::onPushButton_deleteClicked()
{
	if (m_curSelectPlot == nullptr || m_curSelectDatapair == nullptr)
		return;

	for (int i = 0; i < m_curSelectPlot->getDataPair().size(); ++i)
	{
		if (*m_curSelectDatapair == m_curSelectPlot->getDataPair().at(i)->getDataPair())
		{
			QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
			vec.remove(i);
			m_curSelectPlot->setDataPair(vec);

			break;
		}
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
