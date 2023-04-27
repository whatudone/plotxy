#include "AdvancedDataManager.h"
#include "PlotManagerData.h"
#include <QColor>
#include <QColorDialog>
#include <QTableWidgetSelectionRange>

AdvancedDataManager::AdvancedDataManager(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    this->setWindowTitle(QString("AdvancedDataManager"));

	ui.pushButton_copy->setEnabled(false);
	ui.pushButton_autofit->setEnabled(false);
	ui.pushButton_delete->setEnabled(false);

	subSettingWidgetContainer = new SubSettingWidgetContainer(this);
	ui.verticalLayout_8->addWidget(subSettingWidgetContainer);
	subSettingWidgetContainer->setEnabled(false);

	ui.stackedWidget_aDMrpart->setCurrentIndex(0);
	connect(ui.pushButton_add_2, SIGNAL(clicked()), this, SLOT(onBtnAdd()));
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
    connect(subSettingWidgetContainer->m_general,
            SIGNAL(sigBtnGenneralMoreclicked()),
            this,
            SLOT(onBtnMore()));
    connect(subSettingWidgetContainer->m_general,
            SIGNAL(sigCheckBox_16StateChanged(bool)),
            this,
            SLOT(onGeneral_draw(bool)));
    connect(subSettingWidgetContainer->m_general,
            SIGNAL(sigPushButton_12Clicked(QColor)),
            this,
            SLOT(onGeneral_color(QColor)));
    connect(subSettingWidgetContainer->m_general,
            SIGNAL(sigCheckBox_14StateChanged(bool)),
            this,
            SLOT(onGeneral_matchColor(bool)));
    connect(this,
            SIGNAL(sgnGeneral_draw(bool)),
            subSettingWidgetContainer->m_general,
            SLOT(setCheckBox_16CheckState(bool)));
    connect(this,
            SIGNAL(sgnGeneral_color(QColor)),
            subSettingWidgetContainer->m_general,
            SLOT(setPushButton_12Color(QColor)));
    connect(this,
            SIGNAL(sgnGeneral_matchColor(bool)),
            subSettingWidgetContainer->m_general,
            SLOT(setCheckBox_14CheckState(bool)));

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
    connect(this,
            &AdvancedDataManager::sgnIconSetting_draw,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setCheckBoxCheckState);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_flipHorz,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setCheckBox_2CheckState);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_flipVert,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setCheckBox_3CheckState);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_name,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setLineEditText);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_width,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setSpinBoxValue);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_height,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setSpinBox_2Value);
    connect(this,
            &AdvancedDataManager::sgnIconSetting_color,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setPushButton_2Color);
    connect(subSettingWidgetContainer->m_general,
            &General::sigCheckBox_14Color,
            subSettingWidgetContainer->m_iconSetting,
            &IconSetting::setPushButton_2Color);

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
            &LabelSettings::sigCheckBoxStateChanged,
            this,
            &AdvancedDataManager::onLabelSettings_transparent);
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
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_draw,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setCheckBox_5CheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_color,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setPushButton_5Color);
    connect(subSettingWidgetContainer->m_general,
            &General::sigCheckBox_14Color,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setPushButton_5Color);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_background,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setPushButton_6Color);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_transparent,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setCheckBoxStateChanged);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_font,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setFontComboBoxFont);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_fontSize,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setComboBox_4Text);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_xUnit,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setComboBoxText);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_yUnit,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setComboBox_3Text);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_position,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setComboBox_2Text);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_precision_x,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setSpinBox_2Value);
    connect(this,
            &AdvancedDataManager::sgnLabelSettings_precision_y,
            subSettingWidgetContainer->m_labelSettings,
            &LabelSettings::setSpinBox_3Value);

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
    connect(this,
            &AdvancedDataManager::sgnLabelText_format,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setComboBox_3CurrentIndex);
    connect(this,
            &AdvancedDataManager::sgnLabelText_prefix,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setCheckBoxCheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelText_object,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setCheckBox_2CheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelText_attr,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setCheckBox_3CheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelText_data,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setCheckBox_4CheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelText_unit,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setCheckBox_5CheckState);
    connect(this,
            &AdvancedDataManager::sgnLabelText_custom,
            subSettingWidgetContainer->m_labelText,
            &LabelText::setLineEdit_4Text);

	//Color Ranges
    connect(subSettingWidgetContainer->m_colorRanges,
            SIGNAL(sigBtnColorRangesMoreclicked()),
            this,
            SLOT(onBtnColorMore()));

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
    auto plotDataMap = PlotManagerData::getInstance()->getPlotManagerData();
    if(plotDataMap.contains(tabName))
	{
        for(int i = 0; i < plotDataMap[tabName].size(); ++i)
		{
            PlotItemBase* tempPlot = plotDataMap[tabName].at(i);
            if(plotName == tempPlot->getName())
			{
				m_curSelectPlot = tempPlot;
                auto dataPair = m_curSelectPlot->getDataPairs();
                auto dataSize = dataPair.size();
                for(int k = 0; k < dataSize; ++k)
				{
                    if(dataPair.at(k)->getUuid() == uuid)
					{
                        m_curSelectDatapair = dataPair.at(k);
						break;
					}
                }
                if(m_curSelectDatapair != nullptr)
				{
					refreshUI();
					break;
                }
			}
		}
	}
}

void AdvancedDataManager::onPushButton_addClicked()
{
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

void AdvancedDataManager::onLabelSettings_transparent(bool on)
{
    if(m_curSelectDatapair == nullptr)
		return;

	m_curSelectDatapair->setLabelBackTransparent(on);
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
	refreshGeneral();
	refreshIcon();
	refreshExtrapolation();
	refreshLabelSettings();
	refreshStipple();
	refreshEvent();
	refreshLabelText();
	refreshColorRanges();
}

void AdvancedDataManager::refreshGeneral()
{
	sgnGeneral_draw(m_curSelectDatapair->isDraw());
	sgnGeneral_color(m_curSelectDatapair->dataColor());
	sgnGeneral_matchColor(m_curSelectDatapair->matchColor());
}

void AdvancedDataManager::refreshIcon()
{
	sgnIconSetting_draw(m_curSelectDatapair->isIconDraw());
	sgnIconSetting_color(m_curSelectDatapair->iconColor());
	sgnIconSetting_width(m_curSelectDatapair->iconWidth());
	sgnIconSetting_height(m_curSelectDatapair->iconHeight());
	sgnIconSetting_flipHorz(m_curSelectDatapair->iconFlipHorz());
	sgnIconSetting_flipVert(m_curSelectDatapair->iconFlipVert());
	sgnIconSetting_rotation(m_curSelectDatapair->iconRotation());
	sgnIconSetting_name(m_curSelectDatapair->iconName());
}

void AdvancedDataManager::refreshExtrapolation() {}

void AdvancedDataManager::refreshLabelSettings()
{
	sgnLabelSettings_color(m_curSelectDatapair->getLabelColor());
	sgnLabelSettings_background(m_curSelectDatapair->getLabelBackground());
	sgnLabelSettings_transparent(m_curSelectDatapair->getLabelBackTransparent());
	sgnLabelSettings_draw(m_curSelectDatapair->isLabelTextShow());
	sgnLabelSettings_font(m_curSelectDatapair->getLabelFont());
	sgnLabelSettings_fontSize(m_curSelectDatapair->getLabelFontSize());
	sgnLabelSettings_position(m_curSelectDatapair->getLabelPosition());
	sgnLabelSettings_precision_x(m_curSelectDatapair->getLabelPrecision_x());
	sgnLabelSettings_precision_y(m_curSelectDatapair->getLabelPrecision_y());
}

void AdvancedDataManager::refreshStipple() {}

void AdvancedDataManager::refreshEvent() {}

void AdvancedDataManager::refreshLabelText()
{
	sgnLabelText_format(m_curSelectDatapair->getTextFormat());
	sgnLabelText_prefix(m_curSelectDatapair->isPrefixShow());
	sgnLabelText_object(m_curSelectDatapair->isObjectShow());
	sgnLabelText_attr(m_curSelectDatapair->isAttrShow());
	sgnLabelText_data(m_curSelectDatapair->isDataShow());
	sgnLabelText_unit(m_curSelectDatapair->isUnitShow());
	sgnLabelText_custom(m_curSelectDatapair->getCustomText());
}

void AdvancedDataManager::refreshColorRanges() {}

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
