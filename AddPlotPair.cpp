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
	//update();
	//m_textUserX = ui.spinBox_textX->value();
	//m_textUserY = ui.spinBox_textY->value();
	//m_textUser = ui.lineEdit_textEdit->text();

    this->setWindowTitle("Add Plot Pair");
	ui.tableWidget_Entity->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_2->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_3->setStyleSheet("QHeaderView::section{background:lightgray;}");
	ui.tableWidget_Entity_4->setStyleSheet("QHeaderView::section{background:lightgray;}");
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
	ui.tableWidget_Entity_4->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_4->verticalHeader()->hide();
	ui.tableWidget_Entity_Attitude1->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_Attitude1->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude1->verticalHeader()->hide();
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget_Entity_Attitude2->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	ui.tableWidget_Entity_Attitude2->verticalHeader()->hide();
	ui.stackedWidget_textEdit->setCurrentIndex(0);


	connect(ui.pushButton_add, SIGNAL(clicked()), this, SLOT(onBtnAddClicked()));
	connect(ui.pushButton_close, SIGNAL(clicked()), this, SLOT(onBtnCloseClicked()));


	connect(ui.radioButton_userDefine,&QRadioButton::toggled, this, [=](){
		ui.stackedWidget_textEdit->setCurrentIndex(1);});
	connect(ui.radioButton_12, &QRadioButton::toggled, this, [=]() {
		ui.stackedWidget_textEdit->setCurrentIndex(0); });
	connect(ui.radioButton_9, &QRadioButton::toggled, this, [=]() {
		ui.stackedWidget_textEdit->setCurrentIndex(0); });
	connect(ui.radioButton_10, &QRadioButton::toggled, this, [=]() {
		ui.stackedWidget_textEdit->setCurrentIndex(0); });
	connect(ui.tableWidget_Entity, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_2(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_3, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_3(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_4, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_4(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude1, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude1(QTableWidgetItem*)));
	connect(ui.tableWidget_Entity_Attitude2, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(onTableWidgetItemClicked_Attitude2(QTableWidgetItem*)));

	initTreePlot();
}

AddPlotPair::~AddPlotPair()
{
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
		ui.stackedWidget->setCurrentIndex(1);
		break;
	case Type_PlotAScope:
		break;
	case Type_PlotRTI:
		break;
	case Type_PlotText:
		ui.stackedWidget->setCurrentIndex(3);
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
		ui.stackedWidget->setCurrentIndex(3);
		break;
	}
}

void AddPlotPair::setPlotBaseInfo(BaseInfo info)
{
	memcpy(&m_curPlotInfo, &info, sizeof(BaseInfo));

	QList<QTreeWidgetItem*> plotItems = m_treePlot->findItems(m_curPlotInfo.Base_PlotName, Qt::MatchCaseSensitive|Qt::MatchRecursive, 0);
	if (plotItems.size() != 0)
	{
		m_treePlot->itemDoubleClicked(plotItems[0], 0);
	}
}

void AddPlotPair::updatePlotTrees()
{
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

void AddPlotPair::onBtnAddClicked()
{
	int index = ui.stackedWidget->currentIndex();

	QString strEntity1, strNameUnit1, strSum1, strEntity2, strNameUnit2, strSum2,strSum3;




	QString strEntity1, strNameUnit1, strSum1, strEntity2, strNameUnit2, strSum2;

	QPair<QString, QString> p1, p2;

	switch (index)
	{
	case 0:
		if (ui.tableWidget_Entity->currentItem() == NULL || ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0) == NULL)
			return;

		strEntity1 = ui.tableWidget_Entity->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits->item(ui.tableWidget_nameUnits->currentRow(), 0)->text();

		strSum1 = strEntity1 + " " + strNameUnit1;
		strSum2 = "Time";

		emit sigAddPlotPair(strEntity1, strNameUnit1);
		break;

	case 1:
		if (ui.tableWidget_Entity_2->currentItem() == NULL || ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0) == NULL ||
			ui.tableWidget_Entity_3->currentItem() == NULL || ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_2->currentRow(), 0) == NULL)
			return;

		strEntity1 = ui.tableWidget_Entity_2->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_2->item(ui.tableWidget_nameUnits_2->currentRow(), 0)->text();
		strSum1 = strEntity1 + " " + strNameUnit1;
		strEntity2 = ui.tableWidget_Entity_3->currentItem()->text();
		strNameUnit2 = ui.tableWidget_nameUnits_3->item(ui.tableWidget_nameUnits_3->currentRow(), 0)->text();
		strSum2 = strEntity2 + " " + strNameUnit2;

		break;
	case 2:
		if (ui.tableWidget_Entity_Attitude1->currentItem() == NULL || ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0) == NULL ||
			ui.tableWidget_Entity_Attitude2->currentItem() == NULL || ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0) == NULL)
			return;

		strEntity1 = ui.tableWidget_Entity_Attitude1->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_Attitude1->item(ui.tableWidget_nameUnits_Attitude1->currentRow(), 0)->text();
		strSum1 = strEntity1 + " " + strNameUnit1;
		strEntity2 = ui.tableWidget_Entity_Attitude2->currentItem()->text();
		strNameUnit2 = ui.tableWidget_nameUnits_Attitude2->item(ui.tableWidget_nameUnits_Attitude2->currentRow(), 0)->text();
		strSum2 = strEntity2 + " " + strNameUnit2;

		break;
	case 3:

		QTableWidgetItem *item1=nullptr;
		QTableWidgetItem *item2=nullptr;

		if (ui.stackedWidget_textEdit->currentIndex() == 0)
		{
			strEntity1 = ui.tableWidget_Entity_4->currentItem()->text();
			strNameUnit1 = ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0)->text();

			strSum1 = strEntity1 + " " + strNameUnit1;
			strSum2 = "Time";

		if (ui.tableWidget_Entity_4->currentItem() == NULL || ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0) == NULL)
			return;

		strEntity1 = ui.tableWidget_Entity_4->currentItem()->text();
		strNameUnit1 = ui.tableWidget_nameUnits_4->item(ui.tableWidget_nameUnits_4->currentRow(), 0)->text();

		strSum1 = strEntity1 + " " + strNameUnit1;
		strSum2 = "Time";

			m_entityTypeList.append(strEntity1);
			m_entityAttrList.append(strNameUnit1);

		}
		else
		{
			m_textUserX = ui.spinBox_textX->value();
			m_textUserY = ui.spinBox_textY->value();
			strSum1 = QString::number(m_textUserX,10);
			strSum2 = QString::number(m_textUserY, 10);
			strSum3 = ui.lineEdit_textEdit->text();
		}
		break;

	}
//	p1 = qMakePair(m_curPlotInfo.Base_TabName, m_curPlotInfo.Base_PlotName);
	p2 = qMakePair(strSum1, strSum2);

	if (m_plotManager.contains(m_curPlotInfo.Base_TabName))
	{
		for (int i = 0; i < m_plotManager[m_curPlotInfo.Base_TabName].size(); ++i)
		{
			PlotItemBase* tempPlot = m_plotManager[m_curPlotInfo.Base_TabName].at(i);
			if (m_curPlotInfo.Base_PlotName == tempPlot->currName())
			{
				tempPlot->addPlotPairData(p2);
				break;
			}
		}
	}

	QTableWidgetItem* addplot1 = new QTableWidgetItem(strSum1);
	QTableWidgetItem* addplot2 = new QTableWidgetItem(strSum2);
	QTableWidgetItem* addplot3 = new QTableWidgetItem(strSum3);
	int row = ui.tableWidget_union->rowCount();
	ui.tableWidget_union->insertRow(row);
	ui.tableWidget_union->setItem(row, 0, addplot1);
	ui.tableWidget_union->setItem(row, 1, addplot2);
	ui.tableWidget_union->setItem(row, 2, addplot3);
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
	ui.tableWidget_Entity_Attitude1->setRowCount(icount);
	ui.tableWidget_Entity_Attitude2->setRowCount(icount);

	for (auto it = dataMap.begin(); it != dataMap.end(); it++)
	{
		QString currEntityType = it.key();
		QTableWidgetItem* item = new QTableWidgetItem(currEntityType);
		ui.tableWidget_Entity->setItem(index, 0, item);
		ui.tableWidget_Entity_2->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_3->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_4->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude1->setItem(index, 0, new QTableWidgetItem(*item));
		ui.tableWidget_Entity_Attitude2->setItem(index, 0, new QTableWidgetItem(*item));

		index ++;
	}
}

int AddPlotPair::textRowCount()
{
	//计算X格子和Y格子
	QString temText;
	int textCountSum;
	for (int row = 0; row < ui.tableWidget_union->rowCount(); row++)
	{
		
		if (ui.tableWidget_union->item(row, 2)->text().isEmpty()==true)
		{
			temText = ui.tableWidget_union->item(row, 0)->text();
			m_temSet1.insert(temText);
		}
		else
			continue;
	}
	textCountSum = m_temSet1.size();
	return textCountSum;
}
int AddPlotPair::textCloumnCount()
{
	QString temText;
	int textCountSum;
	for (int row = 0; row < ui.tableWidget_union->rowCount(); row++)
	{
		if (ui.tableWidget_union->item(row, 2)->text().isEmpty() == true)
		{
			temText = ui.tableWidget_union->item(row, 1)->text();
			m_temSet2.insert(temText);
		}
		else
			continue;
	}
	textCountSum = m_temSet2.size();
	return  textCountSum;
}

QList<textUserData> AddPlotPair::getUserText()
{
	QList<textUserData> mylist;
	textUserData tUD1;
	tUD1.row = 0;
	tUD1.column = 0;
	tUD1.str = ' ';
	mylist.push_back(tUD1);
	for (int row = 0; row < ui.tableWidget_union->rowCount(); row++)
	{
		if (ui.tableWidget_union->item(row, 2)->text().isEmpty() == false)
		{
			textUserData tUD;
			tUD.row = ui.tableWidget_union->item(row, 0)->text().toInt();
			tUD.column = ui.tableWidget_union->item(row, 1)->text().toInt();
			tUD.str = ui.tableWidget_union->item(row, 2)->text();

			mylist.push_back(tUD);

		}
	}
	return mylist;
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

				ui.tableWidget_union->setRowCount(0);

				QList<QPair<QString, QString>> plotPairData = tempPlot->getPlotPairData();
				for (int k = 0; k < plotPairData.size(); ++k)
				{
					//界面更新
					QTableWidgetItem* addplot1 = new QTableWidgetItem(plotPairData[k].first);
					QTableWidgetItem* addplot2 = new QTableWidgetItem(plotPairData[k].second);
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

}

void AddPlotPair::onBtnRemoveClicked()
{
	int row = ui.tableWidget_union->currentRow();
	if (row != -1)
	{
		ui.tableWidget_union->removeRow(row);
	}
}



void AddPlotPair::onBtnCloseClicked()
{
	close();
}
