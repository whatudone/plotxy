﻿#include "AdvancedDataManager.h"
#include "PlotManagerData.h"
#include <QColor>
#include <QColorDialog>
#include <QTableWidgetSelectionRange>

AdvancedDataManager::AdvancedDataManager(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);

	ui.pushButton_copy->setEnabled(false);
	ui.pushButton_autofit->setEnabled(false);
	ui.pushButton_delete->setEnabled(false);

	subSettingWidgetContainer = new SubSettingWidgetContainer(this);
	ui.verticalLayout_8->addWidget(subSettingWidgetContainer);
	subSettingWidgetContainer->setEnabled(false);

	ui.stackedWidget_aDMrpart->setCurrentIndex(0);
    initConnections();
}

AdvancedDataManager::~AdvancedDataManager() {}

void AdvancedDataManager::onTableWidget_plotpairItemSelectionChanged()
{
	int row = ui.tableWidget_plotpair->currentRow();
	m_curSelectPlot = nullptr;
	m_curSelectDatapair = nullptr;
	ui.pushButton_copy->setEnabled(false);
	ui.pushButton_autofit->setEnabled(false);
	ui.pushButton_delete->setEnabled(false);
	subSettingWidgetContainer->setEnabled(false);

    if(row < 0)
		return;

	ui.pushButton_copy->setEnabled(true);
	ui.pushButton_autofit->setEnabled(true);
	ui.pushButton_delete->setEnabled(true);
	subSettingWidgetContainer->setEnabled(true);

	QString plotName = ui.tableWidget_plotpair->item(row, 2)->text();
	QString tabName = ui.tableWidget_plotpair->item(row, 3)->text();
    // 取出datapair uuid
    QString uuid = ui.tableWidget_plotpair->item(row, 0)->data(Qt::UserRole + 1).toString();
    m_curSelectPlot = PlotManagerData::getInstance()->getPlotByTabAndName(tabName, plotName);
    if(m_curSelectPlot)
    {
        m_curSelectDatapair = m_curSelectPlot->getDataPariByUuid(uuid);
        if(m_curSelectDatapair)
        {
            refreshUI();
        }
    }
}

void AdvancedDataManager::onPushButton_addClicked()
{
    // 跳转到添加数据对界面
	emit sgnAddPlotPair();
}

void AdvancedDataManager::onPushButton_copyClicked() {}

void AdvancedDataManager::onPushButton_autofitClicked() {}

void AdvancedDataManager::onLineEditChanged()
{
    if(m_curSelectDatapair == nullptr)
		return;

	int sortIndex = ui.comboBox_Sort->currentIndex();
	QString sortText = ui.lineEdit->text();
    switch(sortIndex)
	{
    case 0: {

		break;
	}
    case 1: {

		break;
	}
    case 2: {

		break;
	}
    case 3: {

		break;
	}
	default:
		break;
	}

	emit sgnFilterSort(sortText);
}

void AdvancedDataManager::onPushButton_closeClicked()
{
	close();
}

void AdvancedDataManager::onPushButton_deleteClicked()
{
    if(m_curSelectPlot == nullptr || m_curSelectDatapair == nullptr)
		return;

    // TODO:需要查看后续是否自动触发了数据对表格的刷新函数，那里面重置了这两个选中指针
    m_curSelectPlot->delPlotPairData(m_curSelectDatapair->getUuid());
}

void AdvancedDataManager::onGeneral_draw(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setDraw(on);
}

void AdvancedDataManager::onGeneral_color(QColor color)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setColor(color);
}

void AdvancedDataManager::onGeneral_matchColor(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

    m_curSelectDatapair->setMatchColor(on);
}

void AdvancedDataManager::onLineModeChanged(bool lineMode)
{
    if(m_curSelectDatapair)
    {
        m_curSelectDatapair->setLineMode(lineMode);
    }
}

void AdvancedDataManager::onLineWidthChanged(int32_t lineWidth)
{
    if(m_curSelectDatapair)
    {
        m_curSelectDatapair->setLineWidth(lineWidth);
    }
}

void AdvancedDataManager::onLabelSettings_draw(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelTextShow(on);
}

void AdvancedDataManager::onLabelSettings_color(QColor color)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelColor(color);
}

void AdvancedDataManager::onLabelSettings_background(QColor color)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelBackground(color);
}

void AdvancedDataManager::onLabelSettingsSecColor(const QColor& color)
{
    if(m_curSelectDatapair == nullptr)
		return;

    m_curSelectDatapair->setLabelSecColor(color);
}

void AdvancedDataManager::onLabelSettings_font(QFont font)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelFont(font);
}

void AdvancedDataManager::onLabelSettings_fontSize(int size)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelFontSize(size);
}

void AdvancedDataManager::onLabelSettings_position(int pos)
{
    if(m_curSelectDatapair == nullptr)
		return;

    m_curSelectDatapair->setLabelPosition(DataPair::TEXT_POSITION(pos));
}

void AdvancedDataManager::onLabelSettings_precision_x(int prec)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelPrecision_x(prec);
}

void AdvancedDataManager::onLabelSettings_precision_y(int prec)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelPrecision_y(prec);
}

void AdvancedDataManager::onLabelSettings_xUnit(QString) {}

void AdvancedDataManager::onLabelSettings_yUnit(QString) {}

void AdvancedDataManager::onLabelText_format(int format)
{
    if(m_curSelectDatapair == nullptr)
		return;

    m_curSelectDatapair->setTextFormat(DataPair::TEXT_FROMAT(format));
}

void AdvancedDataManager::onLabelText_prefix(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setPrefixShow(on);
}

void AdvancedDataManager::onLabelText_object(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setObjectShow(on);
}

void AdvancedDataManager::onLabelText_attr(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setAttrShow(on);
}

void AdvancedDataManager::onLabelText_data(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setDataShow(on);
}

void AdvancedDataManager::onLabelText_unit(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setUnitShow(on);
}

void AdvancedDataManager::onLabelText_custom(QString text)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setCustomText(text);
}

void AdvancedDataManager::onIconSetting_draw(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconDraw(on);
}

void AdvancedDataManager::onIconSetting_name(QString name)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconName(name);
}

void AdvancedDataManager::onIconSetting_rotation(int rotation)
{
    if(m_curSelectDatapair == nullptr)
		return;

    m_curSelectDatapair->setIconRotation(DataPair::ICON_ROTATION(rotation));
}

void AdvancedDataManager::onIconSetting_flipHorz(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconFlipHorz(on);
}

void AdvancedDataManager::onIconSetting_flipVert(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconFlipVert(on);
}

void AdvancedDataManager::onIconSetting_width(int width)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconWidth(width);
}

void AdvancedDataManager::onIconSetting_height(int height)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconHeight(height);
}

void AdvancedDataManager::onIconSetting_color(QColor color)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setIconColor(color);
}

void AdvancedDataManager::refreshUI()
{
    if(m_curSelectPlot && m_curSelectDatapair)
    {
        // 刷新visible
        auto plotType = m_curSelectPlot->plotType();
        subSettingWidgetContainer->updateVisibleOnPlotTypeChanged(plotType);
        // 刷新内容
        refreshGeneral();
        refreshIcon();
        refreshExtrapolation();
        refreshLabelSettings();
        refreshStipple();
        refreshEvent();
        refreshLabelText();
        refreshColorRanges();
    }
}

void AdvancedDataManager::refreshGeneral()
{
    auto general = subSettingWidgetContainer->m_general;
    auto plotType = m_curSelectPlot->plotType();
    general->updateVisibleOnPlotTypeChanged(plotType);
    general->setCheckBox_16CheckState(m_curSelectDatapair->isDraw());
    general->setPushButton_12Color(m_curSelectDatapair->dataColor());
    general->setCheckBox_14CheckState(m_curSelectDatapair->matchColor());
    general->setLineMode(m_curSelectDatapair->isLineMode());
    general->setLineWidth(m_curSelectDatapair->lineWidth());
}

void AdvancedDataManager::refreshIcon()
{
    auto iconSetting = subSettingWidgetContainer->m_iconSetting;
    iconSetting->setCheckBoxCheckState(m_curSelectDatapair->isIconDraw());
    iconSetting->setPushButton_2Color(m_curSelectDatapair->iconColor());
    iconSetting->setSpinBoxValue(m_curSelectDatapair->iconWidth());
    iconSetting->setSpinBox_2Value(m_curSelectDatapair->iconHeight());
    iconSetting->setCheckBox_2CheckState(m_curSelectDatapair->iconFlipHorz());
    iconSetting->setCheckBox_3CheckState(m_curSelectDatapair->iconFlipVert());
    iconSetting->setComboBoxCurrentIndex(m_curSelectDatapair->iconRotation());
    iconSetting->setLineEditText(m_curSelectDatapair->iconName());
}

void AdvancedDataManager::refreshExtrapolation() {}

void AdvancedDataManager::refreshLabelSettings()
{
    auto labelSettings = subSettingWidgetContainer->m_labelSettings;
    auto plotType = m_curSelectPlot->plotType();
    labelSettings->updateVisibleOnPlotTypeChanged(plotType);
    labelSettings->setPushButton_5Color(m_curSelectDatapair->getLabelColor());
    labelSettings->setPushButton_6Color(m_curSelectDatapair->getLabelBackground());
    labelSettings->setSecColor(m_curSelectDatapair->getLabelSecColor());
    labelSettings->setCheckBox_5CheckState(m_curSelectDatapair->isLabelTextShow());
    labelSettings->setFontComboBoxFont(m_curSelectDatapair->getLabelFont());
    labelSettings->setComboBox_4Text(m_curSelectDatapair->getLabelFontSize());
    labelSettings->setComboBox_2Text(m_curSelectDatapair->getLabelPosition());
    labelSettings->setSpinBox_2Value(m_curSelectDatapair->getLabelPrecision_x());
    labelSettings->setSpinBox_3Value(m_curSelectDatapair->getLabelPrecision_y());
    labelSettings->setComboBoxText(m_curSelectDatapair->getUnit_x());
    labelSettings->setComboBox_3Text(m_curSelectDatapair->getUnit_y());
}

void AdvancedDataManager::refreshStipple() {}

void AdvancedDataManager::refreshEvent() {}

void AdvancedDataManager::refreshLabelText()
{
    auto labelText = subSettingWidgetContainer->m_labelText;
    labelText->setComboBox_3CurrentIndex(m_curSelectDatapair->getTextFormat());
    labelText->setCheckBoxCheckState(m_curSelectDatapair->isPrefixShow());
    labelText->setCheckBox_2CheckState(m_curSelectDatapair->isObjectShow());
    labelText->setCheckBox_3CheckState(m_curSelectDatapair->isAttrShow());
    labelText->setCheckBox_4CheckState(m_curSelectDatapair->isDataShow());
    labelText->setCheckBox_5CheckState(m_curSelectDatapair->isUnitShow());
    labelText->setLineEdit_4Text(m_curSelectDatapair->getCustomText());
}

void AdvancedDataManager::refreshColorRanges() {}

void AdvancedDataManager::initConnections()
{
    initGeneralConnections();
    initIconConnections();
    initLabelSettingsConnections();
    initLabelTextConnections();
    initColorRangeConnections();
    initEventConnections();
}

void AdvancedDataManager::initGeneralConnections()
{
    connect(ui.pushButton_add,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onPushButton_addClicked);
    connect(ui.pushButton_copy,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onPushButton_copyClicked);
    connect(ui.pushButton_autofit,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onPushButton_autofitClicked);
    connect(ui.pushButton_delete,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onPushButton_deleteClicked);
    connect(ui.tableWidget_plotpair,
            &QTableWidget::itemSelectionChanged,
            this,
            &AdvancedDataManager::onTableWidget_plotpairItemSelectionChanged);
    connect(ui.pushButton_close,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onPushButton_closeClicked);
    connect(ui.lineEdit, &QLineEdit::textChanged, this, &AdvancedDataManager::onLineEditChanged);

    //General
    auto general = subSettingWidgetContainer->m_general;
    connect(general, SIGNAL(sigBtnGenneralMoreclicked()), this, SLOT(onBtnMore()));
    connect(general, SIGNAL(sigCheckBox_16StateChanged(bool)), this, SLOT(onGeneral_draw(bool)));
    connect(general, SIGNAL(sigPushButton_12Clicked(QColor)), this, SLOT(onGeneral_color(QColor)));
    connect(
        general, SIGNAL(sigCheckBox_14StateChanged(bool)), this, SLOT(onGeneral_matchColor(bool)));

    connect(general, &General::lineModeChanged, this, &AdvancedDataManager::onLineModeChanged);
    connect(general, &General::lineWidthChanged, this, &AdvancedDataManager::onLineWidthChanged);
}

void AdvancedDataManager::initIconConnections()
{
    //Icon
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigCheckBoxStateChanged,
            this,
            &AdvancedDataManager::onIconSetting_draw);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigCheckBox_2StateChanged,
            this,
            &AdvancedDataManager::onIconSetting_flipHorz);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigCheckBox_3StateChanged,
            this,
            &AdvancedDataManager::onIconSetting_flipVert);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigPushButtonClicked,
            this,
            &AdvancedDataManager::onIconSetting_name);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigComboBoxCurrentIndexChanged,
            this,
            &AdvancedDataManager::onIconSetting_rotation);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigSpinBoxValueChanged,
            this,
            &AdvancedDataManager::onIconSetting_width);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigSpinBox_2ValueChanged,
            this,
            &AdvancedDataManager::onIconSetting_height);
    connect(subSettingWidgetContainer->m_iconSetting,
            &IconSetting::sigPushButton_2Clicked,
            this,
            &AdvancedDataManager::onIconSetting_color);
}

void AdvancedDataManager::initLabelSettingsConnections()
{
    //Label Settings
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigCheckBox_5StateChanged,
            this,
            &AdvancedDataManager::onLabelSettings_draw);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigPushButton_5Clicked,
            this,
            &AdvancedDataManager::onLabelSettings_color);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigPushButton_6Clicked,
            this,
            &AdvancedDataManager::onLabelSettings_background);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::btnSecColorClicked,
            this,
            &AdvancedDataManager::onLabelSettingsSecColor);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigFontComboBoxCurrentFontChanged,
            this,
            &AdvancedDataManager::onLabelSettings_font);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigComboBox_4CurrentTextChanged,
            this,
            &AdvancedDataManager::onLabelSettings_fontSize);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigComboBox_2CurrentIndexChanged,
            this,
            &AdvancedDataManager::onLabelSettings_position);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigComboBoxCurrentTextChanged,
            this,
            &AdvancedDataManager::onLabelSettings_xUnit);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigComboBox_3CurrentTextChanged,
            this,
            &AdvancedDataManager::onLabelSettings_yUnit);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigSpinBox_2ValueChanged,
            this,
            &AdvancedDataManager::onLabelSettings_precision_x);
    connect(subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::sigSpinBox_3ValueChanged,
            this,
            &AdvancedDataManager::onLabelSettings_precision_y);

    connect(subSettingWidgetContainer->m_general,
            &General::sigCheckBox_14Color,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setPushButton_5Color);
}

void AdvancedDataManager::initLabelTextConnections()
{
    //Label Text
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigComboBox_3CurrentIndexChanged,
            this,
            &AdvancedDataManager::onLabelText_format);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigCheckBoxStateChanged,
            this,
            &AdvancedDataManager::onLabelText_prefix);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigCheckBox_2StateChanged,
            this,
            &AdvancedDataManager::onLabelText_object);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigCheckBox_3StateChanged,
            this,
            &AdvancedDataManager::onLabelText_attr);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigCheckBox_4StateChanged,
            this,
            &AdvancedDataManager::onLabelText_data);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigCheckBox_5StateChanged,
            this,
            &AdvancedDataManager::onLabelText_unit);
    connect(subSettingWidgetContainer->m_labelText,
            &LabelText::sigLineEdit_4EditingFinished,
            this,
            &AdvancedDataManager::onLabelText_custom);
}

void AdvancedDataManager::initColorRangeConnections()
{
    //Color Ranges
    connect(ui.pushButtonAddColorRange, SIGNAL(clicked()), this, SLOT(onBtnAddColorRange()));
    connect(ui.pushButtonUpdateColorRange,
            &QPushButton::clicked,
            this,
            &AdvancedDataManager::onBtnUpdateColorRange);
    connect(subSettingWidgetContainer->m_colorRanges,
            SIGNAL(sigBtnColorRangesMoreclicked()),
            this,
            SLOT(onBtnColorMore()));
}

void AdvancedDataManager::initEventConnections()
{
    //Event
    connect(subSettingWidgetContainer->m_eventSetting,
            SIGNAL(sgn_BtnMoreClicked()),
            this,
            SLOT(onEventBtnMoreClicked()));
    connect(PlotManagerData::getInstance(),
            &PlotManagerData::plotDataChanged,
            this,
            &AdvancedDataManager::onUpdatePlotPair);
}

void AdvancedDataManager::onBtnAddColorRange()
{
    QColor color = ui.pushButton_color->color();
	QTreeWidgetItem* addUnion = new QTreeWidgetItem;
	addUnion->setText(0, ui.lineEdit_name->text());
	addUnion->setText(1, ui.lineEdit_value->text());
    addUnion->setBackgroundColor(2, color);

	ui.treeWidget_union->addTopLevelItem(addUnion);
}

void AdvancedDataManager::onBtnUpdateColorRange()
{
    // 将ColorRange数据更新到具体的DataPair中
    if(m_curSelectDatapair)
    {
        QList<std::tuple<QString, double, QColor>> colorInfoList;
        int32_t count = ui.treeWidget_union->topLevelItemCount();
        for(int var = 0; var < count; ++var)
        {
            auto item = ui.treeWidget_union->topLevelItem(var);
            QString name = item->text(0);
            double value = item->text(1).toDouble();
            QColor color = item->backgroundColor(2);
            colorInfoList.append(std::make_tuple(name, value, color));
        }
        m_curSelectDatapair->setColorInfoList(colorInfoList);
    }
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
    auto plotData = PlotManagerData::getInstance()->getPlotManagerData();
    if(plotData.isEmpty())
		return;
    ui.tableWidget_plotpair->clearContents();
	ui.tableWidget_plotpair->setRowCount(0);
    for(int i = 0; i < plotData.size(); ++i)
	{
        QString tabString = plotData.keys().at(i);
        for(int j = 0; j < plotData[tabString].size(); ++j)
		{
            PlotItemBase* tempPlot = plotData[tabString].at(j);
            QVector<DataPair*> dataPairs = tempPlot->getDataPairs();
            for(int k = 0; k < dataPairs.size(); ++k)
			{
                auto dataPair = dataPairs[k];
				//界面更新
                QTableWidgetItem* data1 = new QTableWidgetItem(dataPair->getXEntityAttrPair());
                data1->setData(Qt::UserRole + 1, dataPair->getUuid());
                QTableWidgetItem* data2 = new QTableWidgetItem(dataPair->getYEntityAttrPair());
				QTableWidgetItem* data3 = new QTableWidgetItem(tempPlot->getName());
				QTableWidgetItem* data4 = new QTableWidgetItem(tempPlot->getTabName());
				int row = ui.tableWidget_plotpair->rowCount();
				ui.tableWidget_plotpair->insertRow(row);
				ui.tableWidget_plotpair->setItem(row, 0, data1);
				ui.tableWidget_plotpair->setItem(row, 1, data2);
				ui.tableWidget_plotpair->setItem(row, 2, data3);
				ui.tableWidget_plotpair->setItem(row, 3, data4);
                if(ui.tableWidget_plotpair->currentItem() == NULL)
				{
					ui.tableWidget_plotpair->setCurrentItem(data1);
				}
			}
		}
	}
}
