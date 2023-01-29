#include "PlotManager.h"
#include "addplotpair.h"
#include <QColorDialog>
#include <QTreeWidget>
#include <QTableWidgetItem>
#include <QDebug>
#include <QAction>
#include <QString>
#include <QStyleFactory>
#include "PlotItemBase.h"
#include "PlotManagerData.h"
#include "PlotAttitude.h"

//#include "PlotBar.h"

PlotManager::PlotManager(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton_close, &QPushButton::clicked, this, &PlotManager::onBtnCloseClicked);
	this->setWindowTitle(QString::fromLocal8Bit("ͼ�������"));
	init();

	connect(PlotManagerData::getInstance(), SIGNAL(sgnUpdatePlotManager()), this, SLOT(onUpdatePlotManager()));
	connect(this, SIGNAL(sigChangePlotName()), PlotManagerData::getInstance(), SLOT(slotChangePlotName()));

	ui.treeWidget_selectedPlots->setStyle(QStyleFactory::create("windows"));
	ui.treeWidget_selectedPlots->setHeaderHidden(true);
	ui.treeWidget_selectedPlots->expandAll();

	ui.stackedWidget->setCurrentIndex(0);

	connect(ui.treeWidget_selectedPlots, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSPclicked(QTreeWidgetItem*, int)));

	connect(ui.spinBox_between, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxBetweenChanged);
	connect(ui.spinBox_left, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxLeftChanged);
	connect(ui.spinBox_right, QOverload<int>::of(&QSpinBox::valueChanged), this, &PlotManager::spinboxRightChanged);
}

PlotManager::~PlotManager()
{

}

void PlotManager::init()
{
	initTreeWidgetSettings();
	initGeneralUI();
	initAxisGridUI();

	initTextLightUI();
	initPlotDataUI();
	initTextEditUI();
	initAttitudeUI();
}

void PlotManager::initTreeWidgetSettings()
{
	ui.treeWidget_settings->setHeaderHidden(false);
	ui.treeWidget_settings->setHeaderLabel(QString::fromLocal8Bit("����"));
	ui.treeWidget_settings->setIndentation(15);

	connect(ui.treeWidget_settings, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(onTWSclicked(QTreeWidgetItem*, int)));


	m_itemGeneral = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("ͨ������")));
	m_itemAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("���������������")));
	m_itemLinkedAxis = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("������")));
	m_itemPlotData = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("��������")));
	m_itemText = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("�ı���Ϣ")));
	m_itemScatterPlot = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Scatter����")));
	m_itemAScope = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("A-Scope����")));
	m_itemRTI = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("RTI����")));
	m_itemTextLight = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Text/Light����")));
	m_itemBar = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Bar����")));
	m_itemDial = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Dials����")));
	m_itemAttitude = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Attitude����")));
	m_itemTrackStatus = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Track Status����")));
	m_itemRangeDoppler = new QTreeWidgetItem(ui.treeWidget_settings, QStringList(QString::fromLocal8Bit("Range Doppler����")));

	m_itemGOG = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("GOG����")));
	m_itemLimits = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("����")));
	m_itemPlotMarkers = new QTreeWidgetItem(m_itemScatterPlot, QStringList(QString::fromLocal8Bit("���")));
	m_itemTimeLine = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Time Line"));
	m_itemHandsOff = new QTreeWidgetItem(m_itemScatterPlot, QStringList("Hands-Off"));

	//	ui.treeWidget_settings->setCurrentItem(m_itemGeneral);
	m_itemScatterPlot->setExpanded(true);

	ui.treeWidget_settings->setEnabled(false);
}

void PlotManager::initGeneralUI()
{
	ui.pushButton_flipHeight->setVisible(false);
	ui.pushButton_flipWidth->setVisible(false);
	ui.radioButton_percent->setChecked(false);
	ui.radioButton_pixel->setChecked(true);
	m_radioPixelChecked = true;
	connect(ui.radioButton_percent, &QRadioButton::clicked, this, &PlotManager::onRadioPercentClicked);
	connect(ui.radioButton_pixel, &QRadioButton::clicked, this, &PlotManager::onRadioPixelClicked);
	connect(ui.lineEdit_plotPositionX, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotPositionY, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotWidth, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.lineEdit_plotHeight, &QLineEdit::editingFinished, this, &PlotManager::onPlotRectEditFinished);
	connect(ui.checkBox_draw, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_drawStateChanged);
	connect(ui.lineEdit_plotName, &QLineEdit::editingFinished, this, &PlotManager::onLineEditPlotNameEditingFinished);
	connect(ui.pushButton_outerFillColor, &QPushButton::clicked, this, &PlotManager::onPushButton_outerFillColorClicked);
	connect(ui.pushButton_outlineColor, &QPushButton::clicked, this, &PlotManager::onPushButton_outlineColorClicked);
}

void PlotManager::initAxisGridUI()
{
	ui.pushButton_flipXValues->setVisible(false);
	ui.pushButton_flipYValues->setVisible(false);

	ui.lineEdit_23->setText("1");
	/*ui.pushButton_gridColor->setColor(Qt::gray);*/
	ui.lineEdit_10->setEnabled(false);
	ui.lineEdit_11->setEnabled(false);
	ui.lineEdit_12->setEnabled(false);
	ui.lineEdit_13->setEnabled(false);
	ui.tableWidget->setEnabled(false);
	ui.tableWidget_2->setEnabled(false);
	ui.comboBox_2->setCurrentIndex(0);

	connect(ui.checkBox_4, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_4StateChanged);
	connect(ui.checkBox_5, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_5StateChanged);
	connect(ui.lineEdit_limitBgnX, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitXEditingFinished);
	connect(ui.lineEdit_LimitBgnY, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitYEditingFinished);
	connect(ui.lineEdit_limitEndX, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitXEditingFinished);
	connect(ui.lineEdit_limitEndY, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_limitYEditingFinished);
	connect(ui.pushButton_flipXValues, &QPushButton::clicked, this, &PlotManager::onPushButton_flipXValuesClicked);
	connect(ui.pushButton_flipYValues, &QPushButton::clicked, this, &PlotManager::onPushButton_flipYValuesClicked);
	connect(ui.lineEdit_hrozGrids, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_horzGridsEditingFinished);
	connect(ui.lineEdit_vertGrids, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_vertGridsEditingFinished);
	connect(ui.lineEdit_21, &QLineEdit::editingFinished, this, &PlotManager::onSetAxisColorWidth);
	connect(ui.lineEdit_23, &QLineEdit::editingFinished, this, &PlotManager::onSetGridColorWidth);
	connect(ui.pushButton_axisColor, &QPushButton::clicked, this, &PlotManager::onSetAxisColorWidth);
	connect(ui.pushButton_gridColor, &QPushButton::clicked, this, &PlotManager::onSetGridColorWidth);
	connect(ui.pushButton_gridFill, &QPushButton::clicked, this, &PlotManager::onPushButton_gridFillClicked);
	connect(ui.checkBox_6, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_6StateChanged);
	connect(ui.pushButton_10, &QPushButton::clicked, this, &PlotManager::onPushButton_10Clicked);
	connect(ui.fontComboBox_3, &QFontComboBox::currentFontChanged, this, &PlotManager::onfontComboBox_3CurrentFontChanged);
	connect(ui.comboBox_AxisGrid_FontSize, &QComboBox::currentTextChanged, this, &PlotManager::onComboBox_AxisGrid_FontSizeCurrentTextChanged);
	connect(ui.comboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_2CurrentIndexChanged(int)));
	connect(ui.comboBox_3, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_3CurrentIndexChanged(int)));
	QFontDatabase FontDb;
	foreach(int size, FontDb.standardSizes()) {
		ui.comboBox_AxisGrid_FontSize->addItem(QString::number(size));
	}
}


void PlotManager::initTextLightUI()
{
	ui.label_171->setVisible(false);
	ui.checkBox_30->setVisible(false);
	ui.label_172->setVisible(false);
	ui.checkBox_31->setVisible(false);
	connect(ui.lineEdit_23, &QLineEdit::editingFinished, this, [=]() {
		ui.spinBox_10->setValue(ui.lineEdit_23->text().toInt());
	});
	connect(ui.pushButton_gridColor, &QPushButton::clicked, this, [=]() {
		ui.pushButton_71->setColor(ui.pushButton_gridColor->color());
	});
	connect(ui.pushButton_gridFill, &QPushButton::clicked, this, [=]() {
		ui.pushButton_73->setColor(ui.pushButton_gridFill->color());
	});
	connect(ui.pushButton_66,&QPushButton::clicked,this,&PlotManager::onPushButton_66Clicked);
	connect(ui.pushButton_67, &QPushButton::clicked, this, &PlotManager::onPushButton_67Clicked);
	connect(ui.tableWidget_TextDataSort, &QTableWidget::itemSelectionChanged, this, &PlotManager::onTableWidget_textDataSortItemSelectionChanged);
	connect(ui.spinBox_10, &QSpinBox::editingFinished, this, &PlotManager::onSpinbox_10Changed);
	connect(ui.pushButton_71, &QPushButton::clicked, this, &PlotManager::onPushButton_71Clicked);
	connect(ui.pushButton_73, &QPushButton::clicked, this, &PlotManager::onPushButton_73Clicked);
}
void PlotManager::initPlotDataUI()
{
	connect(ui.pushButton_addNew, SIGNAL(clicked()), this, SLOT(onAddNewClicked()));
	connect(ui.tableWidget_plotData, &QTableWidget::itemSelectionChanged, this, &PlotManager::onTableWidget_plotDataItemSelectionChanged);
	connect(ui.lineEdit_24, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_24EditingFinished);
	connect(ui.pushButton_15, &QPushButton::clicked, this, &PlotManager::onPushButton_15Clicked);
	connect(ui.pushButton_16, &QPushButton::clicked, this, &PlotManager::onPushButton_16Clicked);
	connect(ui.pushButton_18, &QPushButton::clicked, this, &PlotManager::onPushButton_18Clicked);
	connect(ui.pushButton_19, &QPushButton::clicked, this, &PlotManager::onPushButton_19Clicked);
	connect(ui.pushButton_20, &QPushButton::clicked, this, &PlotManager::onPushButton_20Clicked);
	connect(ui.pushButton_21, &QPushButton::clicked, this, &PlotManager::onPushButton_21Clicked);
	connect(ui.checkBox_10, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_10StateChanged);
	connect(ui.checkBox_11, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_11StateChanged);
	connect(ui.toolButton, &QToolButton::clicked, this, &PlotManager::onToolButtonClicked);
	connect(ui.comboBox_6, SIGNAL(currentIndexChanged(int)), this, SLOT(onComboBox_6CurrentIndexChanged(int)));

	ui.pushButton_15->setEnabled(false);
	ui.pushButton_16->setEnabled(false);
	ui.pushButton_18->setEnabled(false);
	ui.pushButton_19->setEnabled(false);
}

void PlotManager::initTextEditUI()
{
	connect(ui.checkBox_12, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_12StateChanged);
	connect(ui.checkBox_13, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_13StateChanged);
	connect(ui.checkBox_14, &QCheckBox::stateChanged, this, &PlotManager::onCheckBox_14StateChanged);
	connect(ui.lineEdit_26, &QLineEdit::editingFinished, this, &PlotManager::onLineEdit_26EditingFinished);
	connect(ui.pushButton_22, &QPushButton::clicked, this, &PlotManager::onPushButton_22Clicked);
	connect(ui.pushButton_23, &QPushButton::clicked, this, &PlotManager::onPushButton_23Clicked);
	QFontDatabase FontDb;
	foreach(int size, FontDb.standardSizes()) {
		ui.comboBox_Text_fontSize->addItem(QString::number(size));
	}
	connect(ui.fontComboBox_2, &QFontComboBox::currentFontChanged, this, &PlotManager::onfontComboBox_2CurrentFontChanged);
	connect(ui.comboBox_Text_fontSize, &QComboBox::currentTextChanged, this, &PlotManager::onComboBox_Text_fontSizeCurrentTextChanged);

}

void PlotManager::initAttitudeUI()
{
	connect(ui.pushButton_80, &QPushButton::clicked, this, &PlotManager::onPushButton_80Clicked);
	connect(ui.pushButton_81, &QPushButton::clicked, this, &PlotManager::onPushButton_81Clicked);
	connect(ui.spinBox_29, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_29ValueChanged(int)));
	connect(ui.spinBox_30, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_30ValueChanged(int)));
	connect(ui.spinBox_31, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_31ValueChanged(int)));
}

void PlotManager::refreshTreeWidgetSettingEnabled(PlotItemBase * plot)
{
	ui.treeWidget_settings->setEnabled(true);
	QString name = plot->metaObject()->className();

	if (name.compare("PlotScatter") == 0)
	{
		enableItem_Scatter();
		//plotPair����
		refreshPlotDataUI(m_curSelectPlot);
	}
	else if (name.compare("PlotAScope") == 0)
	{
		enableItem_AScope();
	}
	else if (name.compare("PlotRTI") == 0)
	{
		enableItem_RTI();
	}
	else if (name.compare("PlotText") == 0)
	{
		enableItem_Text_Light();
		//Text&Light����
		refreshLightTextUI(m_curSelectPlot);
	}
	else if (name.compare("PlotLight") == 0)
	{
		enableItem_Text_Light();
		//Text&Light����
		refreshLightTextUI(m_curSelectPlot);
	}
	else if (name.compare("PlotBar") == 0)
	{
		enableItem_Bar();
	}
	else if (name.compare("PlotDial") == 0)
	{
		enableItem_Dial();
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		enableItem_Attitude();
		refreshAttitudeUI(m_curSelectPlot);
	}
	else if (name.compare("PlotPolar") == 0)
	{
		enableItem_Polar();
	}
	else if (name.compare("PlotTrack") == 0)
	{
		enableItem_Track();
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		enableItem_Doppler();
	}
}

void PlotManager::refreshGeneralUI(PlotItemBase * plot)
{
	emit sigGetTabRect();

	ui.lineEdit_plotName->setText(plot->currName());
	ui.comboBox_tabName->setCurrentText(plot->currTabName());
	ui.pushButton_outerFillColor->setColor(plot->getOuterFillColor());
	ui.pushButton_outlineColor->setColor(plot->getOutlineColor());
	if (ui.radioButton_pixel->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(plot->currPosition().x()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(plot->currPosition().y()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg(plot->currWidth()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg(plot->currHeight()));

	}
	else if (ui.radioButton_percent->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg((float)plot->currPosition().x() / m_tabWidgetRect.width()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg((float)plot->currPosition().y() / m_tabWidgetRect.height()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg((float)plot->currWidth() / m_tabWidgetRect.width()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg((float)plot->currHeight() / m_tabWidgetRect.height()));
	}
}

void PlotManager::refreshAxisGridUI(PlotItemBase * plot)
{
	double x0, x1, y0, y1;
	plot->getCoordRangeX(x0, x1);
	plot->getCoordRangeY(y0, y1);
	ui.lineEdit_limitBgnX->setText(QString("%1").arg(x0));
	ui.lineEdit_limitEndX->setText(QString("%1").arg(x1));
	ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y0));
	ui.lineEdit_limitEndY->setText(QString("%1").arg(y1));
	ui.lineEdit_hrozGrids->setText(QString("%1").arg(plot->getHorzGrids()));
	ui.lineEdit_vertGrids->setText(QString("%1").arg(plot->getVertGrids()));
	ui.lineEdit_21->setText(QString("%1").arg(plot->getAxisWidth()));
	ui.lineEdit_23->setText(QString("%1").arg(plot->getGridWidth()));
	ui.pushButton_axisColor->setColor(plot->getAxisColor());
	ui.pushButton_gridColor->setColor(plot->getGridColor());
	ui.pushButton_gridFill->setColor(plot->getGridFillColor());
	ui.checkBox_6->setChecked(plot->getGridVisible());
	ui.pushButton_10->setColor(plot->getTickLabelColor());
	ui.fontComboBox_3->setCurrentFont(plot->getTickLabelFont());
	ui.comboBox_AxisGrid_FontSize->setCurrentText(QString("%1").arg(plot->getTickLabelFontSize()));
	ui.comboBox_2->setCurrentIndex(int(plot->getGridStyle()) - 1);
	switch (plot->getGridDensity())
	{
	case GridDensity::LESS:
		ui.comboBox_3->setCurrentIndex(0);
		break;
	case GridDensity::NORMAL:
		ui.comboBox_3->setCurrentIndex(1);
		break;
	case GridDensity::MORE:
		ui.comboBox_3->setCurrentIndex(2);
		break;
	}
}

void PlotManager::refreshPlotDataUI(PlotItemBase * plot)
{
	ui.tableWidget_plotData->setRowCount(0);
	QVector<DataPair*> dataPair = plot->getDataPair();
//	QList<QPair<QString, QString>> plotPairData = plot->getPlotPairData();
	for (int k = 0; k < dataPair.size(); ++k)
	{
		//�������
		QTableWidgetItem* addplot1 = new QTableWidgetItem(dataPair[k]->getDataPair().first);
		QTableWidgetItem* addplot2 = new QTableWidgetItem(dataPair[k]->getDataPair().second);
		int row = ui.tableWidget_plotData->rowCount();
		ui.tableWidget_plotData->insertRow(row);
		ui.tableWidget_plotData->setItem(row, 0, addplot1);
		ui.tableWidget_plotData->setItem(row, 1, addplot2);
	}
}

void PlotManager::refreshLightTextUI(PlotItemBase * plot)
{
	if (!(plot == nullptr))
	{
		if (plot->currName().startsWith("Text"))
		{
			ui.stackedWidget_LightTextDataSort->setCurrentIndex(0);
			ui.groupBox_29->setVisible(false);
		}
		else
		{
			ui.stackedWidget_LightTextDataSort->setCurrentIndex(1);
			ui.groupBox_29->setVisible(true);
		}

		for (int i = ui.tableWidget_TextDataSort->rowCount(); i > 0; i--)
		{
			ui.tableWidget_TextDataSort->removeRow(ui.tableWidget_TextDataSort->rowCount() - 1);
		}


		auto dataPair = plot->getDataPair();

		for (int i = 0; i < plot->getDataPair().size(); i++)
		{

			//QTableWidgetItem* addplot1 = new QTableWidgetItem(dataPair[i+1]->getDataPair().first);

			QString temFirst = dataPair[i]->getDataPair().first;
			QString temEntityString = temFirst.split("+").front();
			QString temAttriString = temFirst.split("+").back();
			QTableWidgetItem *temEntity = new QTableWidgetItem(temEntityString);
			QTableWidgetItem *temAttri = new QTableWidgetItem(temAttriString);

			int row = ui.tableWidget_TextDataSort->rowCount();
			ui.tableWidget_TextDataSort->insertRow(row);
			ui.tableWidget_TextDataSort->setItem(row, 0, temEntity);
			ui.tableWidget_TextDataSort->setItem(row, 1, temAttri);
		}
	}
}




void PlotManager::refreshTextEditUI(PlotItemBase * plot)
{
	ui.checkBox_12->setChecked(plot->unitsShowX());
	ui.checkBox_13->setChecked(plot->unitsShowY());
	ui.checkBox_14->setChecked(plot->getTitleVisible());
	ui.lineEdit_26->setText(plot->getTitle());
	ui.pushButton_22->setColor(plot->getTitleColor());
	ui.pushButton_23->setColor(plot->getTitleFillColor());
	ui.fontComboBox_2->setCurrentFont(plot->getTitleFont());
	ui.comboBox_Text_fontSize->setCurrentText(QString("%1").arg(plot->getTitleFontSize()));
}

void PlotManager::refreshAttitudeUI(PlotItemBase * plot)
{
	if (dynamic_cast<PlotAttitude*>(plot) == nullptr)
		return;

	ui.pushButton_80->setColor(dynamic_cast<PlotAttitude*>(plot)->getRollColor());
	ui.pushButton_81->setColor(dynamic_cast<PlotAttitude*>(plot)->getPitchColor());
	ui.spinBox_29->setValue(dynamic_cast<PlotAttitude*>(plot)->getTickRadiusPercentage());
	ui.spinBox_30->setValue(dynamic_cast<PlotAttitude*>(plot)->getTextPercentage());
	ui.spinBox_31->setValue(dynamic_cast<PlotAttitude*>(plot)->getDialPercentage());
}

void PlotManager::enableItem_Scatter()
{
	m_itemLinkedAxis->setDisabled(false);
	m_itemPlotData->setDisabled(false);
	m_itemScatterPlot->setDisabled(false);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_AScope()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(false);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_RTI()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(false);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Text_Light()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(false);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Bar()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(false);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Dial()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(false);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Attitude()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(false);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Polar()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Track()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(false);
	m_itemRangeDoppler->setDisabled(true);
}

void PlotManager::enableItem_Doppler()
{
	m_itemLinkedAxis->setDisabled(true);
	m_itemPlotData->setDisabled(true);
	m_itemScatterPlot->setDisabled(true);
	m_itemAScope->setDisabled(true);
	m_itemRTI->setDisabled(true);
	m_itemTextLight->setDisabled(true);
	m_itemBar->setDisabled(true);
	m_itemDial->setDisabled(true);
	m_itemAttitude->setDisabled(true);
	m_itemTrackStatus->setDisabled(true);
	m_itemRangeDoppler->setDisabled(false);
}

void PlotManager::onTWSPclicked(QTreeWidgetItem* item, int column)
{
	QTreeWidgetItem *parent = item->parent();
	if (NULL == parent)
		return;

	QString parent_text = parent->text(0);
	QString child_text = item->text(column);

	if (m_plotManager.contains(parent_text))
	{
		for (int i = 0; i < m_plotManager[parent_text].size(); ++i)
		{
			PlotItemBase *tempPlot = m_plotManager[parent_text].at(i);
			if (child_text == tempPlot->currName())
			{
				m_curSelectPlot = tempPlot;
				//ˢ��treeWidgetSetting��ʹ��״̬
				refreshTreeWidgetSettingEnabled(m_curSelectPlot);
				//general����
				refreshGeneralUI(m_curSelectPlot);
				//Axis&Grid����
				refreshAxisGridUI(m_curSelectPlot);
				//Text Edit
				refreshTextEditUI(m_curSelectPlot);

				//
			//	tempPlot->deleteLater();
				break;
			}
			m_curSelectPlot = nullptr;
		}
	}
	else
		m_curSelectPlot = nullptr;

	/*if (child_text == "Bar")
	{
		m_itemBar->setDisabled(false);
	}*/
}

void PlotManager::onAddNewClicked()
{
	emit sigAddPlotPair(m_curSelectPlot->currTabName(), m_curSelectPlot->currName());
}

void PlotManager::onSelectedPlot(QString tabName, QString plotName)
{
	if (tabName == nullptr || plotName == nullptr)
		return;

	QList<QTreeWidgetItem*> items = ui.treeWidget_selectedPlots->findItems(plotName, Qt::MatchExactly | Qt::MatchRecursive);
	if (items.size() != 0)
	{
		for each (QTreeWidgetItem* item in items)
		{
			if (item->parent() != NULL && item->parent()->text(0) == tabName)
			{
				QTreeWidgetItemIterator it(ui.treeWidget_selectedPlots);
				while (*it)
				{
					(*it)->setSelected(false);
					++it;
				}

				ui.treeWidget_selectedPlots->itemClicked(item, 0);
				item->setSelected(true);
				break;
			}
		}
	}
}

void PlotManager::onUpdatePlotManager()
{
	m_plotManager = PlotManagerData::getInstance()->getPlotManagerData();
	if (m_plotManager.isEmpty())
	{
		return;
	}

	ui.comboBox_tabName->clear();
	ui.treeWidget_selectedPlots->clear();

	for (int i = 0; i < m_plotManager.size(); ++i)
	{
		QString tabName = m_plotManager.keys().at(i);
		QTreeWidgetItem* itemselPlotH = new QTreeWidgetItem(QStringList() << tabName);
		ui.treeWidget_selectedPlots->addTopLevelItem(itemselPlotH);
		ui.treeWidget_selectedPlots->expandAll();
		for (int k = 0; k < m_plotManager[tabName].size(); ++k)
		{
			QTreeWidgetItem* itemselPlotI = new QTreeWidgetItem(QStringList() << m_plotManager[tabName].at(k)->currName());
			itemselPlotH->addChild(itemselPlotI);
		}

		//comboBox_tabName
		ui.comboBox_tabName->addItem(tabName);
	}
}

void PlotManager::onTWSclicked(QTreeWidgetItem* item, int column)
{
	QString compare;
	compare = item->text(column);

	if (item->isDisabled() == true)
	{
		return;
	}
	else
	{

		if (compare == QString::fromLocal8Bit("ͨ������"))

		{
			ui.stackedWidget->setCurrentIndex(0);
		}
		else if (compare == QString::fromLocal8Bit("�����������"))
		{
			ui.stackedWidget->setCurrentIndex(1);
		}
		else if (compare == QString::fromLocal8Bit("��ͼ����"))
		{
			ui.stackedWidget->setCurrentIndex(2);
		}
		else if (compare == QString::fromLocal8Bit("�ı���Ϣ"))
		{
			ui.stackedWidget->setCurrentIndex(3);
		}
		else if (compare == QString::fromLocal8Bit("������"))
		{
			ui.stackedWidget->setCurrentIndex(5);
		}
		else if (compare == QString::fromLocal8Bit("Scatter����"))
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (compare == QString::fromLocal8Bit("GOG����"))
		{
			ui.stackedWidget->setCurrentIndex(4);
		}
		else if (compare == QString::fromLocal8Bit("����"))
		{
			ui.stackedWidget->setCurrentIndex(6);
		}
		else if (compare == QString::fromLocal8Bit("���"))
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
		else if (compare == QString::fromLocal8Bit("A-Scope����"))
		{
			ui.stackedWidget->setCurrentIndex(10);
		}
		else if (compare == QString::fromLocal8Bit("RTI����"))
		{
			ui.stackedWidget->setCurrentIndex(11);
		}
		else if (compare == QString::fromLocal8Bit("Text/Light����"))
		{
			ui.stackedWidget->setCurrentIndex(12);
			if (m_curSelectPlot == nullptr)
				return;
			refreshLightTextUI(m_curSelectPlot);
		}
		else if (compare == QString::fromLocal8Bit("Bar����"))
		{
			ui.stackedWidget->setCurrentIndex(13);
		}
		else if (compare == QString::fromLocal8Bit("Dials����"))
		{
			ui.stackedWidget->setCurrentIndex(14);
		}
		else if (compare == QString::fromLocal8Bit("Attitude����"))
		{
			ui.stackedWidget->setCurrentIndex(15);
		}
		else if (compare == QString::fromLocal8Bit("Track Status����"))
		{
			ui.stackedWidget->setCurrentIndex(16);
		}
		else if (compare == QString::fromLocal8Bit("Range Doppler����"))
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

void PlotManager::onRadioPixelClicked()
{
	ui.label_xPos->setText("pixel");
	ui.label_yPos->setText("pixel");
	ui.label_widthPos->setText("pixel");
	ui.label_heightPos->setText("pixel");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = true;
		return;
	}
	if (!m_radioPixelChecked)
	{
		float percent = ui.lineEdit_plotPositionX->text().toFloat();
		int pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotPositionY->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotWidth->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.width());
		ui.lineEdit_plotWidth->setText(QString("%1").arg(pixel));

		percent = ui.lineEdit_plotHeight->text().toFloat();
		pixel = (int)(percent * m_tabWidgetRect.height());
		ui.lineEdit_plotHeight->setText(QString("%1").arg(pixel));
	}
	m_radioPixelChecked = true;
}

void PlotManager::onRadioPercentClicked()
{
	ui.label_xPos->setText("%");
	ui.label_yPos->setText("%");
	ui.label_widthPos->setText("%");
	ui.label_heightPos->setText("%");

	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		m_radioPixelChecked = false;
		return;
	}
	if (m_radioPixelChecked)
	{
		int pixel = ui.lineEdit_plotPositionX->text().toInt();
		float percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotPositionY->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotWidth->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.width();
		ui.lineEdit_plotWidth->setText(QString("%1").arg(percent));

		pixel = ui.lineEdit_plotHeight->text().toInt();
		percent = (float)pixel / m_tabWidgetRect.height();
		ui.lineEdit_plotHeight->setText(QString("%1").arg(percent));
	}
	m_radioPixelChecked = false;
}

void PlotManager::onGetTabWidgetRect(QRect rect)
{
	m_tabWidgetRect = rect;
}

void PlotManager::onBtnCloseClicked()
{
	close();
}

void PlotManager::onPushButton_71Clicked()
{
	ui.pushButton_gridColor->setColor(ui.pushButton_71->color());
	ui.lineEdit_23->setText(QString("%1").arg(ui.spinBox_10->value()));
	m_curSelectPlot->setGridColorWidth(ui.pushButton_71->color(), ui.spinBox_10->value());
}

void PlotManager::onSpinbox_10Changed()
{
	ui.pushButton_gridColor->setColor(ui.pushButton_71->color());
	ui.lineEdit_23->setText(QString("%1").arg(ui.spinBox_10->value()));
	m_curSelectPlot->setGridColorWidth(ui.pushButton_71->color(), ui.spinBox_10->value());
}

void PlotManager::onPushButton_73Clicked()
{
	QColor color = ui.pushButton_73->color();
	if (!(color == ui.pushButton_gridFill->color()))
	{
		m_curSelectPlot->setGridFillColor(color);
	}
	ui.pushButton_gridFill->setColor(color);
}

void PlotManager::onPlotRectEditFinished()
{
	if (ui.lineEdit_plotPositionX->text() == nullptr || ui.lineEdit_plotPositionY->text() == nullptr ||
		ui.lineEdit_plotWidth->text() == nullptr || ui.lineEdit_plotHeight->text() == nullptr)
	{
		return;
	}

	bool bX, bY, bW, bH;
	int x, y, w, h;
	if (m_radioPixelChecked)
	{
		x = ui.lineEdit_plotPositionX->text().toInt(&bX);
		y = ui.lineEdit_plotPositionY->text().toInt(&bY);
		w = ui.lineEdit_plotWidth->text().toInt(&bW);
		h = ui.lineEdit_plotHeight->text().toInt(&bH);
	}
	else
	{
		x = (int)(ui.lineEdit_plotPositionX->text().toFloat(&bX) * m_tabWidgetRect.width());
		y = (int)(ui.lineEdit_plotPositionY->text().toFloat(&bY) * m_tabWidgetRect.height());
		w = (int)(ui.lineEdit_plotWidth->text().toFloat(&bW) * m_tabWidgetRect.width());
		h = (int)(ui.lineEdit_plotHeight->text().toFloat(&bH) * m_tabWidgetRect.height());
	}

	if (bX && bY && bW && bH && (m_curSelectPlot != nullptr))
	{
		//m_curSelectPlot->setRect(QRect(x, y, w, h));
		connect(this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
		emit sigRectChanged(QRect(x, y, w, h));
		disconnect(this, SIGNAL(sigRectChanged(QRect)), m_curSelectPlot, SLOT(slot_updateRect(QRect)));
	}


}

void PlotManager::onLineEditPlotNameEditingFinished()
{
	if (m_curSelectPlot != nullptr)
	{
		QString oldName = m_curSelectPlot->currName();
		QString newName = ui.lineEdit_plotName->text();
		if (newName.compare(oldName) != 0)
		{
			m_curSelectPlot->setName(newName);
			emit sigChangePlotName();
		}
	}
}

void PlotManager::onCheckBox_drawStateChanged()
{
	if (m_curSelectPlot != nullptr)
	{
		connect(this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
		emit sigSetPlotVisible(ui.checkBox_draw->isChecked());
		disconnect(this, SIGNAL(sigSetPlotVisible(bool)), m_curSelectPlot, SLOT(slot_setVisible(bool)));
	}
}

void PlotManager::onPushButton_outerFillColorClicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setOuterFillColor(ui.pushButton_outerFillColor->color());
}

void PlotManager::onPushButton_outlineColorClicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setOutlineColor(ui.pushButton_outlineColor->color());
}

void PlotManager::onMouseEventDone()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	emit sigGetTabRect();

	if (ui.radioButton_pixel->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg(m_curSelectPlot->currPosition().x()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg(m_curSelectPlot->currPosition().y()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg(m_curSelectPlot->currWidth()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg(m_curSelectPlot->currHeight()));

	}
	else if (ui.radioButton_percent->isChecked())
	{
		ui.lineEdit_plotPositionX->setText(QString("%1").arg((float)m_curSelectPlot->currPosition().x() / m_tabWidgetRect.width()));
		ui.lineEdit_plotPositionY->setText(QString("%1").arg((float)m_curSelectPlot->currPosition().y() / m_tabWidgetRect.height()));
		ui.lineEdit_plotWidth->setText(QString("%1").arg((float)m_curSelectPlot->currWidth() / m_tabWidgetRect.width()));
		ui.lineEdit_plotHeight->setText(QString("%1").arg((float)m_curSelectPlot->currHeight() / m_tabWidgetRect.height()));
	}
}

void PlotManager::onCheckBox_4StateChanged()
{
	ui.lineEdit_10->setEnabled(ui.checkBox_4->isChecked());
	ui.lineEdit_11->setEnabled(ui.checkBox_4->isChecked());
	ui.tableWidget->setEnabled(ui.checkBox_4->isChecked());
}

void PlotManager::onCheckBox_5StateChanged()
{
	ui.lineEdit_12->setEnabled(ui.checkBox_5->isChecked());
	ui.lineEdit_13->setEnabled(ui.checkBox_5->isChecked());
	ui.tableWidget_2->setEnabled(ui.checkBox_5->isChecked());
}

void PlotManager::onLineEdit_limitXEditingFinished()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool bx0, bx1;
	double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
	double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
	if (bx0 && bx1)
		m_curSelectPlot->setCoordRangeX(x0, x1);
}

void PlotManager::onLineEdit_limitYEditingFinished()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool by0, by1;
	double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
	double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
	if (by0 && by1)
		m_curSelectPlot->setCoordRangeY(y0, y1);
}

void PlotManager::onPushButton_flipXValuesClicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool bx0, bx1;
	double x0 = ui.lineEdit_limitBgnX->text().toDouble(&bx0);
	double x1 = ui.lineEdit_limitEndX->text().toDouble(&bx1);
	if (bx0 && bx1)
	{
		m_curSelectPlot->setCoordRangeX(x1, x0);
		ui.lineEdit_limitBgnX->setText(QString("%1").arg(x1));
		ui.lineEdit_limitEndX->setText(QString("%1").arg(x0));
	}
}

void PlotManager::onPushButton_flipYValuesClicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	bool by0, by1;
	double y0 = ui.lineEdit_LimitBgnY->text().toDouble(&by0);
	double y1 = ui.lineEdit_limitEndY->text().toDouble(&by1);
	if (by0 && by1)
	{
		m_curSelectPlot->setCoordRangeX(y1, y0);
		ui.lineEdit_LimitBgnY->setText(QString("%1").arg(y1));
		ui.lineEdit_limitEndY->setText(QString("%1").arg(y0));
	}
}

void PlotManager::onLineEdit_horzGridsEditingFinished()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	bool on;
	uint count = ui.lineEdit_hrozGrids->text().toUInt(&on);
	if (!on || count < 0)
	{
		return;
	}

	m_curSelectPlot->setHorzGrids(count);
}

void PlotManager::onLineEdit_vertGridsEditingFinished()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	bool on;
	uint count = ui.lineEdit_vertGrids->text().toUInt(&on);
	if (!on || count < 0)
	{
		return;
	}

	m_curSelectPlot->setVertGrids(count);
}

void PlotManager::onSetAxisColorWidth()
{
	bool on;
	uint width = ui.lineEdit_21->text().toInt(&on);
	QColor color = ui.pushButton_axisColor->color();
	if (on)
		m_curSelectPlot->setAxisColorWidth(color, width);
}

void PlotManager::onSetGridColorWidth()
{
	bool on;
	uint width = ui.lineEdit_23->text().toInt(&on);
	QColor color = ui.pushButton_gridColor->color();
	if (on)
		m_curSelectPlot->setGridColorWidth(color, width);
}

void PlotManager::onPushButton_gridFillClicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setGridFillColor(ui.pushButton_gridFill->color());
}

void PlotManager::onCheckBox_6StateChanged()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setGridVisible(ui.checkBox_6->isChecked());
}

void PlotManager::onPushButton_10Clicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTickLabelColor(ui.pushButton_10->color());
}

void PlotManager::onfontComboBox_3CurrentFontChanged(const QFont & font)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	int fontSize = ui.comboBox_AxisGrid_FontSize->currentText().toInt();
	QFont newFont;
	newFont.setFamily(font.family());
	newFont.setPointSize(fontSize);
	m_curSelectPlot->setTickLabelFont(newFont);
}

void PlotManager::onComboBox_AxisGrid_FontSizeCurrentTextChanged(const QString & text)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTickLabelFontSize(text.toInt());
}

void PlotManager::onComboBox_2CurrentIndexChanged(int index)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	m_curSelectPlot->setGridStyle(GridStyle(index));
}

void PlotManager::onComboBox_3CurrentIndexChanged(int index)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	GridDensity density;
	switch (index)
	{
	case 0:
		density = GridDensity::LESS;
		break;
	case 1:
		density = GridDensity::NORMAL;
		break;
	case 2:
		density = GridDensity::MORE;
		break;
	}
	m_curSelectPlot->setGridDensity(density);
}

void PlotManager::onTableWidget_textDataSortItemSelectionChanged()
{
	//����Move Up/Move Down��enable
	if (ui.tableWidget_TextDataSort->currentRow() < 0)
		return;
	int row = ui.tableWidget_TextDataSort->rowCount();
	ui.pushButton_66->setEnabled(false);
	ui.pushButton_67->setEnabled(false);

	if (row <= 1)
	{
		ui.pushButton_66->setEnabled(false);
		ui.pushButton_67->setEnabled(false);
	}
	else if (ui.tableWidget_TextDataSort->currentRow() == 0)
	{
		ui.pushButton_66->setEnabled(false);
		ui.pushButton_67->setEnabled(true);
	}
	else if (ui.tableWidget_TextDataSort->currentRow() == row - 1)
	{
		ui.pushButton_66->setEnabled(true);
		ui.pushButton_67->setEnabled(false);
	}
	else
	{
		ui.pushButton_66->setEnabled(true);
		ui.pushButton_67->setEnabled(true);
	}
}

void PlotManager::onTableWidget_plotDataItemSelectionChanged()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
	{
		ui.pushButton_15->setEnabled(false);
		ui.pushButton_16->setEnabled(false);
		ui.pushButton_18->setEnabled(false);
		ui.pushButton_19->setEnabled(false);
		return;
	}

	ui.pushButton_18->setEnabled(true);
	ui.pushButton_19->setEnabled(true);

	//�ж�ѡ��������ʹ��move��ť
	int rowCount = ui.tableWidget_plotData->rowCount();
	if (rowCount <= 1)
	{
		ui.pushButton_15->setEnabled(false);
		ui.pushButton_16->setEnabled(false);
	}
	else if (row == 0)
	{
		ui.pushButton_15->setEnabled(false);
		ui.pushButton_16->setEnabled(true);
	}
	else if ((row + 1) == rowCount)
	{
		ui.pushButton_15->setEnabled(true);
		ui.pushButton_16->setEnabled(false);
	}
	else
	{
		ui.pushButton_15->setEnabled(true);
		ui.pushButton_16->setEnabled(true);
	}

	if (m_curSelectPlot == nullptr)
		return;

	ui.lineEdit_24->setText(QString("%1").arg(m_curSelectPlot->getDataPair().at(row)->lineWidth()));
	ui.pushButton_21->setColor(m_curSelectPlot->getDataPair().at(row)->dataColor());
	ui.checkBox_11->setChecked(m_curSelectPlot->getDataPair().at(row)->isDraw());
	ui.checkBox_10->setChecked(m_curSelectPlot->getDataPair().at(row)->isLineMode());
}

void PlotManager::onLineEdit_24EditingFinished()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	int width = ui.lineEdit_24->text().toInt();
	m_curSelectPlot->getDataPair().at(row)->setLineWidth(width);
}

void PlotManager::onPushButton_21Clicked()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	m_curSelectPlot->getDataPair().at(row)->setColor(ui.pushButton_21->color());
}

void PlotManager::onCheckBox_10StateChanged()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	m_curSelectPlot->getDataPair().at(row)->setLineMode(ui.checkBox_10->checkState());
}

void PlotManager::onCheckBox_11StateChanged()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	m_curSelectPlot->getDataPair().at(row)->setDraw(ui.checkBox_11->checkState());
}

void PlotManager::onPushButton_15Clicked()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row <= 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
	vec.move(row, row - 1);
	m_curSelectPlot->setDataPair(vec);
	ui.tableWidget_plotData->setCurrentCell(row - 1, 0);
}

void PlotManager::onPushButton_16Clicked()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0 || row >= (ui.tableWidget_plotData->rowCount() - 1))
		return;
	if (m_curSelectPlot == nullptr)
		return;

	QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
	vec.move(row, row + 1);
	m_curSelectPlot->setDataPair(vec);
	ui.tableWidget_plotData->setCurrentCell(row + 1, 0);
}

void PlotManager::onPushButton_66Clicked()
{
	int row = ui.tableWidget_TextDataSort->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;
	
	if (ui.tableWidget_TextDataSort->currentColumn() == 0)
	{
		QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
		int temRow = ui.tableWidget_TextDataSort->currentRow();
		DataPair *temNowDataPair = new DataPair(nullptr);
		temNowDataPair = vec.at(temRow);		
		QString temNowFirst = temNowDataPair->getDataPair().first;
		QString temNowFront = temNowFirst.split("+").front();
		QString temNowBack = temNowFirst.split("+").back();
		DataPair * temAboveDataPair = new DataPair(nullptr);
		temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() - 1);
		QString temAboveFirst = temAboveDataPair->getDataPair().first;
		QString temAboveFront = temAboveFirst.split("+").front();
		QString temAboveBack = temAboveFirst.split("+").back();
		QPair<QString, QString> newAbove = qMakePair(temNowFront+"+"+temAboveBack, temAboveDataPair->getDataPair().second);
		QPair<QString, QString> newNow = qMakePair(temAboveFront + "+" + temNowBack, temNowDataPair->getDataPair().second);
		DataPair* newAboveDataPair = new DataPair(newAbove);
		DataPair* newNowDataPair = new DataPair(newNow);
		vec.remove(ui.tableWidget_TextDataSort->currentRow()-1);
		vec.insert(ui.tableWidget_TextDataSort->currentRow() - 1,newAboveDataPair);
		vec.remove(ui.tableWidget_TextDataSort->currentRow());
		vec.insert(ui.tableWidget_TextDataSort->currentRow(),newNowDataPair);
		m_curSelectPlot->setDataPair(vec);
		refreshLightTextUI(m_curSelectPlot);
		ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 0);
		delete temNowDataPair;
		delete temAboveDataPair;
		delete newAboveDataPair;
		delete newNowDataPair;
	}
	else if (ui.tableWidget_TextDataSort->currentColumn() == 1)
	{
		QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
		int temRow = ui.tableWidget_TextDataSort->currentRow();
		DataPair *temNowDataPair = new DataPair(nullptr);
		temNowDataPair = vec.at(temRow);
		QString temNowFirst = temNowDataPair->getDataPair().first;
		QString temNowFront = temNowFirst.split("+").front();
		QString temNowBack = temNowFirst.split("+").back();
		DataPair * temAboveDataPair = new DataPair(nullptr);
		temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() - 1);
		QString temAboveFirst = temAboveDataPair->getDataPair().first;
		QString temAboveFront = temAboveFirst.split("+").front();
		QString temAboveBack = temAboveFirst.split("+").back();
		QPair<QString, QString> newAbove = qMakePair(temAboveFront + "+" + temNowBack, temAboveDataPair->getDataPair().second);
		QPair<QString, QString> newNow = qMakePair(temNowFront + "+" + temAboveBack, temNowDataPair->getDataPair().second);
		DataPair* newAboveDataPair = new DataPair(newAbove);
		DataPair* newNowDataPair = new DataPair(newNow);
		vec.remove(ui.tableWidget_TextDataSort->currentRow() - 1);
		vec.insert(ui.tableWidget_TextDataSort->currentRow() - 1, newAboveDataPair);
		vec.remove(ui.tableWidget_TextDataSort->currentRow());
		vec.insert(ui.tableWidget_TextDataSort->currentRow(), newNowDataPair);
		m_curSelectPlot->setDataPair(vec);
		refreshLightTextUI(m_curSelectPlot);
		ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 1);
		delete temNowDataPair;
		delete temAboveDataPair;
		delete newAboveDataPair;
		delete newNowDataPair;
	}
	else
		return;
	//Light�ı��ʱ����ܻ��õ�
	//QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
	//vec.move(row, row - 1);
	//m_curSelectPlot->setDataPair(vec);
	//refreshLightTextUI(m_curSelectPlot);
	//ui.tableWidget_TextDataSort->setCurrentCell(row - 1, 0);
}

void PlotManager::onPushButton_67Clicked()
{
	int row = ui.tableWidget_TextDataSort->currentRow();
	if (row < 0 || row >= (ui.tableWidget_TextDataSort->rowCount() - 1))
		return;
	if (m_curSelectPlot == nullptr)
		return;

	if (ui.tableWidget_TextDataSort->currentColumn() == 0)
	{
		QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
		int temRow = ui.tableWidget_TextDataSort->currentRow();
		DataPair *temNowDataPair = new DataPair(nullptr);
		temNowDataPair = vec.at(temRow);
		QString temNowFirst = temNowDataPair->getDataPair().first;
		QString temNowFront = temNowFirst.split("+").front();
		QString temNowBack = temNowFirst.split("+").back();
		DataPair * temAboveDataPair = new DataPair(nullptr);
		temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() + 1);
		QString temAboveFirst = temAboveDataPair->getDataPair().first;
		QString temAboveFront = temAboveFirst.split("+").front();
		QString temAboveBack = temAboveFirst.split("+").back();
		QPair<QString, QString> newAbove = qMakePair(temNowFront + "+" + temAboveBack, temAboveDataPair->getDataPair().second);
		QPair<QString, QString> newNow = qMakePair(temAboveFront + "+" + temNowBack, temNowDataPair->getDataPair().second);
		DataPair* newAboveDataPair = new DataPair(newAbove);
		DataPair* newNowDataPair = new DataPair(newNow);
		vec.remove(ui.tableWidget_TextDataSort->currentRow() + 1);
		vec.insert(ui.tableWidget_TextDataSort->currentRow() + 1, newAboveDataPair);
		vec.remove(ui.tableWidget_TextDataSort->currentRow());
		vec.insert(ui.tableWidget_TextDataSort->currentRow(), newNowDataPair);
		m_curSelectPlot->setDataPair(vec);
		refreshLightTextUI(m_curSelectPlot);
		ui.tableWidget_TextDataSort->setCurrentCell(row + 1, 0);
		delete temNowDataPair;
		delete temAboveDataPair;
		delete newAboveDataPair;
		delete newNowDataPair;
	}
	else if (ui.tableWidget_TextDataSort->currentColumn() == 1)
	{
		QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
		int temRow = ui.tableWidget_TextDataSort->currentRow();
		DataPair *temNowDataPair = new DataPair(nullptr);
		temNowDataPair = vec.at(temRow);
		QString temNowFirst = temNowDataPair->getDataPair().first;
		QString temNowFront = temNowFirst.split("+").front();
		QString temNowBack = temNowFirst.split("+").back();
		DataPair * temAboveDataPair = new DataPair(nullptr);
		temAboveDataPair = vec.at(ui.tableWidget_TextDataSort->currentRow() + 1);
		QString temAboveFirst = temAboveDataPair->getDataPair().first;
		QString temAboveFront = temAboveFirst.split("+").front();
		QString temAboveBack = temAboveFirst.split("+").back();
		QPair<QString, QString> newAbove = qMakePair(temAboveFront + "+" + temNowBack, temAboveDataPair->getDataPair().second);
		QPair<QString, QString> newNow = qMakePair(temNowFront + "+" + temAboveBack, temNowDataPair->getDataPair().second);
		DataPair* newAboveDataPair = new DataPair(newAbove);
		DataPair* newNowDataPair = new DataPair(newNow);
		vec.remove(ui.tableWidget_TextDataSort->currentRow() + 1);
		vec.insert(ui.tableWidget_TextDataSort->currentRow() + 1, newAboveDataPair);
		vec.remove(ui.tableWidget_TextDataSort->currentRow());
		vec.insert(ui.tableWidget_TextDataSort->currentRow(), newNowDataPair);
		m_curSelectPlot->setDataPair(vec);
		refreshLightTextUI(m_curSelectPlot);
		ui.tableWidget_TextDataSort->setCurrentCell(row + 1, 1);
		delete temNowDataPair;
		delete temAboveDataPair;
		delete newAboveDataPair;
		delete newNowDataPair;
	}
	else
		return;
}

void PlotManager::onPushButton_18Clicked()
{
	
}

void PlotManager::onPushButton_19Clicked()
{
	int row = ui.tableWidget_plotData->currentRow();
	if (row < 0)
		return;
	if (m_curSelectPlot == nullptr)
		return;

	QVector<DataPair*> vec = m_curSelectPlot->getDataPair();
	vec.remove(row);
	m_curSelectPlot->setDataPair(vec);
}

void PlotManager::onPushButton_20Clicked()
{
	emit sigAdvancedDataManager();
}

void PlotManager::onToolButtonClicked()
{
}

void PlotManager::onComboBox_6CurrentIndexChanged(int index)
{
}

void PlotManager::onCheckBox_12StateChanged()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setUnitsShowX(ui.checkBox_12->isChecked());
}

void PlotManager::onCheckBox_13StateChanged()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setUnitsShowY(ui.checkBox_13->isChecked());
}

void PlotManager::onCheckBox_14StateChanged()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTitleVisible(ui.checkBox_14->isChecked());
}

void PlotManager::onLineEdit_26EditingFinished()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTitle(ui.lineEdit_26->text());
}

void PlotManager::onPushButton_22Clicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTitleColor(ui.pushButton_22->color());
}

void PlotManager::onPushButton_23Clicked()
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	m_curSelectPlot->setTitleFillColor(ui.pushButton_23->color());
}

void PlotManager::onfontComboBox_2CurrentFontChanged(const QFont & font)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}
	float fontSize = ui.comboBox_Text_fontSize->currentText().toFloat();
	QFont newFont;
	newFont.setFamily(font.family());
	newFont.setPointSizeF(fontSize);
	m_curSelectPlot->setTitleFont(newFont);
}

void PlotManager::onComboBox_Text_fontSizeCurrentTextChanged(const QString & text)
{
	if (m_curSelectPlot == nullptr)
	{
		return;
	}

	// 	QFont font = ui.fontComboBox_2->currentFont();
	// 	font.setPointSizeF(text.toFloat());
	// 	m_curSelectPlot->setTitleFont(font);
	m_curSelectPlot->setTitleFontSize(text.toInt());
}

void PlotManager::onPushButton_80Clicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	dynamic_cast<PlotAttitude*>(m_curSelectPlot)->setRollColor(ui.pushButton_80->color());
}

void PlotManager::onPushButton_81Clicked()
{
	if (m_curSelectPlot == nullptr)
		return;

	dynamic_cast<PlotAttitude*>(m_curSelectPlot)->setPitchColor(ui.pushButton_81->color());
}

void PlotManager::onSpinBox_29ValueChanged(int value)
{
	if (m_curSelectPlot == nullptr)
		return;

	dynamic_cast<PlotAttitude*>(m_curSelectPlot)->setTickRadiusPercentage(value);
}

void PlotManager::onSpinBox_30ValueChanged(int value)
{
	if (m_curSelectPlot == nullptr)
		return;

	dynamic_cast<PlotAttitude*>(m_curSelectPlot)->setTextPercentage(value);
}

void PlotManager::onSpinBox_31ValueChanged(int value)
{
	if (m_curSelectPlot == nullptr)
		return;

	dynamic_cast<PlotAttitude*>(m_curSelectPlot)->setDialPercentage(value);
}

