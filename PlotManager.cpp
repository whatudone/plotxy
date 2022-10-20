#include "PlotManager.h"
#include "addplotpair.h"
#include <QColorDialog>
#include <QTreeWidget>
#include <QDebug>
#include <QAction>
#include <QString>

#include "PlotBar.h"

PlotManager::PlotManager(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	this->resize(800, 600);
	this->setWindowTitle(QString::fromLocal8Bit("图表管理器"));

	/*QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem;
	QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem;
	itemselPlotH->setText(0, "test");
	itemselPlotI->setText(0, "Bar");

	ui.treeWidget_selectedPlots->setHeaderItem(itemselPlotH);
	ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotI);*/
	ui.treeWidget_selectedPlots->setHeaderHidden(true);
	ui.treeWidget_selectedPlots->expandAll();

	ui.treeWidget_settings->setHeaderHidden(false);
	ui.treeWidget_settings->setHeaderLabel(QString::fromLocal8Bit("设置"));
	ui.treeWidget_settings->setIndentation(15);

	m_itemGeneral = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("总体设置")));
	m_itemAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("坐标轴和网格设置")));
	m_itemPlotData = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("数据设置")));
	m_itemText = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("文本信息")));
	m_itemGOG = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("GOG曲线")));
	m_itemLinkedAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("相关的轴")));
	m_itemScatterPlot = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Scatter设置")));
	m_itemAScope = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("A-Scope设置")));
	m_itemRTI = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("RTI设置")));
	m_itemTextLight = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Text/Light设置")));

	m_itemBar = new QTreeWidgetItem;
	m_itemBar->setText(0, QString::fromLocal8Bit("Bar设置"));
	ui.treeWidget_settings->addTopLevelItem(m_itemBar);

	m_itemDial = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Dials设置")));
	m_itemAttitude = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Attitude设置")));
	m_itemTrackStatus = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Track Status设置")));
	m_itemRangeDoppler = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Range Doppler设置")));

	m_itemLimits = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("限制")));
	m_itemPlotMarkers = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("标记")));
	m_itemTimeLine = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Time Line"));
	m_itemHandsOff = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Hands-Off"));

	m_itemGOG->setDisabled(true); 
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);

	ui.stackedWidget->setCurrentIndex(0);
	ui.treeWidget_settings->setCurrentItem(m_itemGeneral);
	m_itemScatterPlot->setExpanded(true);

	/*ui.textEdit->setFixedHeight(24);
	ui.textEdit_2->setFixedHeight(24);
	ui.pushButton_10->setFixedSize(21, 21);
	ui.pushButton_21->setFixedSize(21, 21);
	ui.pushButton_22->setFixedSize(21, 21);
	ui.pushButton_23->setFixedSize(21, 21);*/

	connect(ui.treeWidget_settings, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSclicked(QTreeWidgetItem*, int)));
	connect(ui.treeWidget_selectedPlots, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSPclicked(QTreeWidgetItem*, int)));
	
	connect(ui.pushButton_addNew, SIGNAL(clicked()), this, SLOT(onAddNewClicked()));
	connect(ui.pushButton_axisColor, SIGNAL(clicked()), this, SLOT(onBtnAxisColorClicked()));
	connect(ui.pushButton_gridColor, SIGNAL(clicked()), this, SLOT(onBtnGridColorClicked()));
	connect(ui.pushButton_gridFill, SIGNAL(clicked()), this, SLOT(onBtnGridFillClicked()));

	connect(ui.spinBox_between, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxBetweenChanged);
	connect(ui.spinBox_left, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxLeftChanged);
	connect(ui.spinBox_right, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxRightChanged);

	connect(ui.lineEdit_hrozGrids, &QLineEdit::textChanged, this, [=]() {
		m_hrozGrids = ui.lineEdit_hrozGrids->text().toInt();
		});
	connect(ui.lineEdit_vertGrids, &QLineEdit::textChanged, this, [=]() {
		m_vertGrids = ui.lineEdit_vertGrids->text().toInt();
		});

	ui.pushButton_axisColor->setFixedSize(21, 21);
	ui.pushButton_gridColor->setFixedSize(21, 21);
	ui.pushButton_gridFill->setFixedSize(21, 21);
	ui.textEdit->setFixedHeight(24);
	ui.textEdit_2->setFixedHeight(24);
	ui.pushButton_10->setFixedSize(21, 21);
	ui.pushButton_21->setFixedSize(21, 21);
	ui.pushButton_22->setFixedSize(21, 21);
	ui.pushButton_23->setFixedSize(21, 21);
}

PlotManager::~PlotManager()
{

}


void PlotManager::init()
{

}


void PlotManager::addPlot(/*int currTabIndex*/const QString& tabName, PlotItemBase* plotItem)
{
	//显示层更新
	if (m_plotManager.contains(tabName))
	{
		QList<QTreeWidgetItem*> topWidget = ui.treeWidget_selectedPlots->findItems(tabName, Qt::MatchCaseSensitive, 0);
		if (topWidget.size() != 0)
		{
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
			topWidget[0]->addChild(itemselPlotI);
		}
	} 
	else
	{
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList()<<tabName);
		ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);

		QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << plotItem->currName());
		itemselPlotH->addChild(itemselPlotI);
	}

	//数据层更新
	m_plotManager[tabName].append(plotItem);
}

void PlotManager::onTWSPclicked(QTreeWidgetItem* item, int i)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent)
		return;

	QString parent_text = parent->text(0);
	QString child_text = item->text(i);

	if (m_plotManager.contains(parent_text))
	{
		for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
		{
			PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
			if (child_text == tempPlot->currName())
			{
				ui.treeWidget_4->clear();

				QList<QPair<QString, QString>> plotPairData = tempPlot->getPlotPairData();
				for (int k = 0; k < plotPairData.size(); ++k)
				{
					//界面更新
					QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
					addplotItem->setText(0, plotPairData[k].first);
					addplotItem->setText(1, plotPairData[k].second);

					ui.treeWidget_4->addTopLevelItem(addplotItem);
				}

				break;
			}
		}
	}

	/*if (child_text == "Bar")
	{
		m_itemBar->setDisabled(false);
	}*/
}

void PlotManager::onAddNewClicked()
{
	/*AddPlotPair* addPlotPair = new AddPlotPair();
	addPlotPair->show();*/

	emit sigAddPlotPair();
}


void PlotManager::onTWSclicked(QTreeWidgetItem* item, int i)
{
	QString compare;
	compare = item->text(i);

	if (item->isDisabled() == true)
	{
		return;
	}
	else
	{
		if (compare == QString::fromLocal8Bit("总体设置"))
		{
			ui.stackedWidget->setCurrentIndex(0);
		}
		else if (compare == QString::fromLocal8Bit("坐标轴和网格设置"))
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (compare == QString::fromLocal8Bit("数据设置"))
		{
			ui.stackedWidget->setCurrentIndex(2);
		}
		else if (compare == QString::fromLocal8Bit("文本信息"))
		{
			ui.stackedWidget->setCurrentIndex(3);
		}
		else if (compare == QString::fromLocal8Bit("GOG曲线"))
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (compare == QString::fromLocal8Bit("相关的轴"))
		{
			ui.stackedWidget->setCurrentIndex(5);
		}
		else if (compare == QString::fromLocal8Bit("Scatter设置"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("限制"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("标记"))
		{
			ui.stackedWidget->setCurrentIndex(7);
		}
		else if (compare == QString::fromLocal8Bit("Time Line"))
		{
			ui.stackedWidget->setCurrentIndex(8);
		}
		else if (compare == QString::fromLocal8Bit("Hands-Off"))
		{
			ui.stackedWidget->setCurrentIndex(9);
		}
		else if (compare == QString::fromLocal8Bit("A-Scope设置"))
		{
			ui.stackedWidget->setCurrentIndex(10);
		}
		else if (compare == QString::fromLocal8Bit("RTI设置"))
		{
			ui.stackedWidget->setCurrentIndex(11);
		}
		else if (compare == QString::fromLocal8Bit("Text/Light设置"))
		{
			ui.stackedWidget->setCurrentIndex(12);
		}
		else if (compare == QString::fromLocal8Bit("Bar设置"))
		{
			ui.stackedWidget->setCurrentIndex(13);
		}
		else if (compare == QString::fromLocal8Bit("Dials设置"))
		{
			ui.stackedWidget->setCurrentIndex(14);
		}
		else if (compare == QString::fromLocal8Bit("Attitude设置"))
		{
			ui.stackedWidget->setCurrentIndex(15);
		}
		else if (compare == QString::fromLocal8Bit("Track Status设置"))
		{
			ui.stackedWidget->setCurrentIndex(16);
		}
		else if (compare == QString::fromLocal8Bit("Range Doppler设置"))
		{
			ui.stackedWidget->setCurrentIndex(17);
		}
	}
}

void PlotManager::spinboxBetweenChanged()
{
	m_spinBoxBetween = ui.spinBox_between->value();
}
void PlotManager::spinboxLeftChanged()
{
	m_spinBoxLeft = ui.spinBox_left->value();
}
void PlotManager::spinboxRightChanged()
{
	m_spinBoxRight = ui.spinBox_right->value();
}


void PlotManager::onAddPlotPair(QString entityType, QString entityAttr)
{
	//界面更新
	QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
	addplotItem->setText(0, entityType + " " + entityAttr);
	addplotItem->setText(1, "Time");

	ui.treeWidget_4->addTopLevelItem(addplotItem);

	//数据层更新 ->更新对应Item数据
	// tmpCode
	//获取当前plotBar控件
	if (m_plotManager.isEmpty())
		return;

	auto it = m_plotManager.begin();
	PlotItemBase* currItem = it.value().at(0);
	if (currItem)
	{
		PlotBar* currBarItem = dynamic_cast<PlotBar*>(currItem);
		currBarItem->addPlotDataPair(entityType, entityAttr);
	}
}

void PlotManager::onAddPlotPair(QString tabName, QString plotName, QString xColumn, QString yColumn)
{
	//界面更新
	QTreeWidgetItem* addplotItem = new QTreeWidgetItem;
	addplotItem->setText(0, xColumn);
	addplotItem->setText(1, yColumn);
	ui.treeWidget_4->addTopLevelItem(addplotItem);

	//获取当前plotBar控件
	if (m_plotManager[tabName].isEmpty())
		return;

	for (int i = 0; i < m_plotManager[tabName].size(); ++i)
	{
		PlotItemBase* currItem = m_plotManager[tabName].at(i);
		QString name = currItem->metaObject()->className();
		if (name.compare("PlotPlotScatter") == 0)
		{
			
		}
		else if (name.compare("PlotAScope") == 0)
		{
			
		}
		else if (name.compare("PlotRTI") == 0)
		{
			
		}
		else if (name.compare("PlotText") == 0)
		{
			
		}
		else if (name.compare("PlotLight") == 0)
		{
			
		}
		else if (name.compare("PlotBar") == 0)
		{
			
		}
		else if (name.compare("PlotDial") == 0)
		{
			
		}
		else if (name.compare("PlotAttitude") == 0)
		{
			
		}
		else if (name.compare("PlotPolar") == 0)
		{
			
		}
		else if (name.compare("PlotTrack") == 0)
		{
			
		}
		else if (name.compare("PlotDoppler") == 0)
		{
			
		}
	}
}

void PlotManager::onBtnAxisColorClicked()
{

	m_axisColor = QColorDialog::getColor(Qt::yellow, this);
	if (!m_axisColor.isValid())
	{
	}
	else
	{
		ui.pushButton_axisColor->setStyleSheet("background-color: " + m_axisColor.name() + ";");
	}
}

void PlotManager::onBtnGridColorClicked()
{

	m_gridColor = QColorDialog::getColor(Qt::yellow, this);
	if (!m_gridColor.isValid())
	{
	}
	else
	{
		ui.pushButton_gridColor->setStyleSheet("background-color: " + m_gridColor.name() + ";");
	}
}
void PlotManager::onBtnGridFillClicked()
{
	m_girdFill = QColorDialog::getColor(Qt::yellow, this);
	if (!m_girdFill.isValid())
	{
	}
	else
	{
		ui.pushButton_gridFill->setStyleSheet("background-color: " + m_girdFill.name() + ";");
	}
}