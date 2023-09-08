#include "SubSettingWidgetContainer.h"
#include "ToolBox.h"
#include <QColorDialog>
#include <QTextEdit>

SubSettingWidgetContainer::SubSettingWidgetContainer(QWidget* parent)
	: QMainWindow(parent)
{
    toolBox = new ToolBox(this);

	//General
	m_general = new General(this);
    toolBox->addWidget("通用", m_general);

	//Extrapolation
    m_extrapolation = new Extrapolation(this);
    toolBox->addWidget("推断", m_extrapolation);

	//icon
	m_iconSetting = new IconSetting(this);
    toolBox->addWidget("图标", m_iconSetting);

	//Label Settings
	m_labelSettings = new LabelSettings(this);
    toolBox->addWidget("标签设置", m_labelSettings);

	//Label Text
	m_labelText = new LabelText(this);
    toolBox->addWidget("标签文本", m_labelText);

	//Stipple
	m_stippleSetting = new StippleSetting(this);
    toolBox->addWidget("纹理", m_stippleSetting);

	//Event
	m_eventSetting = new EventSetting(this);
    toolBox->addWidget("事件", m_eventSetting);

	//Color Ranges
	m_colorRanges = new ColorRanges(this);
    toolBox->addWidget("颜色范围", m_colorRanges);

    // Connections
    m_connections = new Connections(this);
    toolBox->addWidget("连接", m_connections);
	setCentralWidget(toolBox);

    connect(m_labelSettings,
            &LabelSettings::sigCheckBox_5StateChanged,
            m_labelText,
            &LabelText::onWidgetEnabled);
    connect(m_general,
            &General::sigCheckBox_14Color,
            m_iconSetting,
            &IconSetting::setPushButton_2Color);
}

SubSettingWidgetContainer::~SubSettingWidgetContainer() {}

void SubSettingWidgetContainer::updateVisibleOnPlotTypeChanged(PlotType curType)
{
    QList<QWidget*> list;
    list << toolBox->pageWidget("通用") << toolBox->pageWidget("推断")
         << toolBox->pageWidget("图标") << toolBox->pageWidget("标签设置")
         << toolBox->pageWidget("标签文本") << toolBox->pageWidget("纹理")
         << toolBox->pageWidget("事件") << toolBox->pageWidget("颜色范围")
         << toolBox->pageWidget("连接");
    QList<bool> visibleList;
    if(curType == PlotType::Type_PlotAScope)
    {
        visibleList << true << false << false << false << false << true << false << false << false;
    }
    else if(curType == PlotType::Type_PlotAttitude)
    {
        visibleList << true << false << false << false << false << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotBar)
    {
        visibleList << true << false << false << true << true << false << false << true << false;
    }
    else if(curType == PlotType::Type_PlotDial)
    {
        visibleList << true << false << false << false << false << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotLight)
    {
        visibleList << true << false << false << true << true << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotPolar)
    {
        visibleList << true << false << true << true << true << true << false << false << false;
    }
    else if(curType == PlotType::Type_PlotDoppler)
    {
        visibleList << true << false << false << false << false << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotRTI)
    {
        visibleList << true << false << false << false << false << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotScatter)
    {
        visibleList << true << false << true << true << true << true << true << false << true;
    }
    else if(curType == PlotType::Type_PlotText)
    {
        visibleList << true << false << false << true << true << false << false << false << false;
    }
    else if(curType == PlotType::Type_PlotTrack)
    {
        visibleList << true << false << false << true << false << false << false << false << false;
    }
    else
    {
        return;
    }

    for(int var = 0; var < list.size(); ++var)
    {
        list.at(var)->setVisible(visibleList.at(var));
    }
}

/////////////////////////////General//////////////////////////////////
General::General(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_more, &QPushButton::clicked, this, &General::onPushbuttonMoreClicked);
	connect(ui.pushButton_12, &QPushButton::clicked, this, &General::onPushButton_12Clicked);
	connect(ui.checkBox_16, &QCheckBox::stateChanged, this, &General::onCheckBox_16StateChanged);
    connect(ui.checkBox_14, &QCheckBox::stateChanged, this, &General::onCheckBox_14StateChanged);
    connect(ui.checkBoxLineMode,
            &QCheckBox::stateChanged,
            this,
            &General::onCheckBoxLineModeStateChanged);
    connect(
        ui.lineEditWidth, &QLineEdit::editingFinished, this, &General::onLineEditWidthEditFinished);
    connect(ui.lineEditGeneralSecsLimit, &QLineEdit::editingFinished, this, [this]() {
        emit secsLimitChanged(ui.lineEditGeneralSecsLimit->text().toDouble());
    });
    connect(ui.spinBoxGeneralPointsLimit,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            [this](int value) { emit pointsLimitChanged(value); });
}

General::~General() {}

void General::updateVisibleOnPlotTypeChanged(PlotType curType)
{
    static QList<QWidget*> list = QList<QWidget*>()
                                  << ui.label_30 << ui.checkBox_16 << ui.labelLineMode
                                  << ui.checkBoxLineMode << ui.labelWidth << ui.lineEditWidth
                                  << ui.label_31 << ui.checkBox_14 << ui.label_26
                                  << ui.pushButton_12 << ui.label_27 << ui.lineEditGeneralSecsLimit
                                  << ui.label_28 << ui.spinBoxGeneralPointsLimit << ui.label_32
                                  << ui.checkBox_15 << ui.label_29 << ui.pushButton_more;
    /*
     * visibleList存储九行界面的visible属性，如果只有一个数据
     * 表示所有行都是该属性
    */
    QList<bool> visibleList;
    if(curType == PlotType::Type_PlotAScope)
    {
        visibleList = QList<bool>()
                      << true << true << true << false << true << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotBar || curType == PlotType::Type_PlotLight)
    {
        visibleList = QList<bool>()
                      << true << false << false << true << true << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotDial)
    {
        visibleList = QList<bool>()
                      << true << false << false << false << true << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotAttitude)
    {
        visibleList = QList<bool>()
                      << true << false << false << false << false << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotPolar || curType == PlotType::Type_PlotScatter)
    {
        visibleList = QList<bool>()
                      << true << true << true << true << true << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotDoppler)
    {
        visibleList = QList<bool>() << true << false << false << false << false << false << false
                                    << false << false;
    }
    else if(curType == PlotType::Type_PlotRTI)
    {
        visibleList = QList<bool>()
                      << true << false << false << true << true << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotText)
    {
        visibleList = QList<bool>()
                      << true << false << false << false << false << true << true << false << false;
    }
    else if(curType == PlotType::Type_PlotTrack)
    {
        visibleList = QList<bool>()
                      << true << false << false << false << true << true << true << false << false;
    }
    for(int var = 0; var < list.size(); ++var)
    {
        int32_t index = 0;
        if(visibleList.size() == 1)
        {
            index = 0;
        }
        else
        {

            index = var / 2;
        }
        list.at(var)->setVisible(visibleList.at(index));
    }
}

void General::onCheckBox_14StateChanged()
{
	emit sigCheckBox_14StateChanged(ui.checkBox_14->checkState());
    if(ui.checkBox_14->isChecked())
		emit sigCheckBox_14Color(ui.pushButton_12->color());
}

void General::onCheckBox_16StateChanged()
{
	emit sigCheckBox_16StateChanged(ui.checkBox_16->checkState());
}

void General::onPushButton_12Clicked()
{
	emit sigPushButton_12Clicked(ui.pushButton_12->color());
    if(ui.checkBox_14->isChecked())
        emit sigCheckBox_14Color(ui.pushButton_12->color());
}

void General::onCheckBoxLineModeStateChanged()
{
    emit lineModeChanged(ui.checkBoxLineMode->isChecked());
}

void General::onLineEditWidthEditFinished()
{
    emit lineWidthChanged(ui.lineEditWidth->text().toInt());
}

void General::setCheckBox_14CheckState(bool on)
{
	ui.checkBox_14->setChecked(on);
}

void General::setCheckBox_16CheckState(bool on)
{
	ui.checkBox_16->setChecked(on);
}

void General::setPushButton_12Color(QColor color)
{
    ui.pushButton_12->setColor(color);
}

void General::setLineMode(bool lineMode)
{
    ui.checkBoxLineMode->setChecked(lineMode);
}

void General::setLineWidth(int32_t width)
{
    ui.lineEditWidth->setText(QString::number(width));
}

void General::setSecsLimit(double limit)
{
    ui.lineEditGeneralSecsLimit->setText(QString::number(limit));
}

void General::setPointsLimit(double limit)
{
    ui.spinBoxGeneralPointsLimit->setValue(limit);
}

void General::onPushbuttonMoreClicked()
{
	emit sigBtnGenneralMoreclicked();
}

/////////////////////////////Extrapolation//////////////////////////////////
Extrapolation::Extrapolation(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);
}

Extrapolation::~Extrapolation() {}

/////////////////////////////LabelSetting//////////////////////////////////
LabelSettings::LabelSettings(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	QFontDatabase FontDb;
    foreach(int size, FontDb.standardSizes())
    {
		ui.comboBox_4->addItem(QString::number(size));
	}

    connect(
        ui.checkBox_5, &QCheckBox::stateChanged, this, &LabelSettings::onCheckBox_5StateChanged);
	connect(ui.pushButton_5, &QPushButton::clicked, this, &LabelSettings::onPushButton_5Clicked);
	connect(ui.pushButton_6, &QPushButton::clicked, this, &LabelSettings::onPushButton_6Clicked);
    connect(ui.pushButtonSecColor,
            &QPushButton::clicked,
            this,
            &LabelSettings::onPushButtonSecColorClicked);
    connect(ui.fontComboBox,
            &QFontComboBox::currentFontChanged,
            this,
            &LabelSettings::onFontComboBoxCurrentFontChanged);
    connect(ui.comboBox,
            &QComboBox::currentTextChanged,
            this,
            &LabelSettings::onComboBoxCurrentTextChanged);
    connect(ui.comboBox_2,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBox_2CurrentIndexChanged(int)));
    connect(ui.comboBox_3,
            &QComboBox::currentTextChanged,
            this,
            &LabelSettings::onComboBox_3CurrentTextChanged);
    connect(ui.comboBox_4,
            &QComboBox::currentTextChanged,
            this,
            &LabelSettings::onComboBox_4CurrentTextChanged);
	connect(ui.spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_2ValueChanged(int)));
	connect(ui.spinBox_3, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_3ValueChanged(int)));
}

LabelSettings::~LabelSettings() {}

void LabelSettings::updateVisibleOnPlotTypeChanged(PlotType curType)
{
    QList<QList<QWidget*>> list = {{ui.label_13, ui.checkBox_5},
                                   {ui.label_7, ui.pushButton_5},
                                   {ui.label_8, ui.pushButton_6},
                                   {ui.label_9, ui.pushButtonSecColor},
                                   {ui.label_2, ui.fontComboBox, ui.comboBox_4},
                                   {ui.label_10, ui.comboBox},
                                   {ui.label_11, ui.spinBox_2},
                                   {ui.label_15, ui.comboBox_3},
                                   {ui.label_14, ui.spinBox_3},
                                   {ui.label_12, ui.comboBox_2}};
    /*
         * visibleList存储九行界面的visible属性，如果只有一个数据
         * 表示所有行都是该属性
         * visibleList = QList<bool>() << true << true << true << true << true << true << true << true
                                    << true << true;
        */
    QList<bool> visibleList;
    if(curType == PlotType::Type_PlotBar)
    {
        visibleList = QList<bool>() << true << true << false << false << true << false << true
                                    << false << false << true;
    }
    else if(curType == PlotType::Type_PlotLight)
    {
        visibleList = QList<bool>() << false << true << false << false << true << false << false
                                    << false << false << false;
    }
    else if(curType == PlotType::Type_PlotPolar || curType == PlotType::Type_PlotScatter)
    {
        visibleList = QList<bool>() << true << true << false << false << true << false << true
                                    << false << true << true;
    }
    else if(curType == PlotType::Type_PlotText)
    {
        visibleList = QList<bool>() << true << true << false << false << true << false << true
                                    << false << false << true;
    }
    else if(curType == PlotType::Type_PlotTrack)
    {
        visibleList = QList<bool>() << true << true << false << false << true << false << false
                                    << false << false << false;
    }
    else
    {
        // 其他类型图表不存在此设置，直接跳过
        return;
    }

    for(int var = 0; var < list.size(); ++var)
    {
        bool visible = visibleList.at(var);
        auto widgetList = list.at(var);
        for(auto widget : widgetList)
        {
            widget->setVisible(visible);
        }
    }
}

void LabelSettings::onCheckBox_5StateChanged(int state)
{
	emit sigCheckBox_5StateChanged((bool)state);
}

void LabelSettings::onPushButton_5Clicked()
{
	emit sigPushButton_5Clicked(ui.pushButton_5->color());
}

void LabelSettings::onPushButton_6Clicked()
{
	emit sigPushButton_6Clicked(ui.pushButton_6->color());
}

void LabelSettings::onPushButtonSecColorClicked()
{
    emit btnSecColorClicked(ui.pushButtonSecColor->color());
}

void LabelSettings::onFontComboBoxCurrentFontChanged(const QFont& font)
{
	emit sigFontComboBoxCurrentFontChanged(font);
}

void LabelSettings::onComboBoxCurrentTextChanged(QString text)
{
	emit sigComboBoxCurrentTextChanged(text);
}

void LabelSettings::onComboBox_2CurrentIndexChanged(int index)
{
	emit sigComboBox_2CurrentIndexChanged(index);
}

void LabelSettings::onComboBox_3CurrentTextChanged(QString text)
{
	emit sigComboBox_3CurrentTextChanged(text);
}

void LabelSettings::onComboBox_4CurrentTextChanged(QString text)
{
	emit sigComboBox_4CurrentTextChanged(text.toInt());
}

void LabelSettings::onSpinBox_2ValueChanged(int value)
{
	emit sigSpinBox_2ValueChanged(value);
}

void LabelSettings::onSpinBox_3ValueChanged(int value)
{
	emit sigSpinBox_3ValueChanged(value);
}

void LabelSettings::setCheckBox_5CheckState(bool state)
{
	ui.checkBox_5->setChecked(state);
}

void LabelSettings::setPushButton_5Color(QColor color)
{
	ui.pushButton_5->setColor(color);
}

void LabelSettings::setPushButton_6Color(QColor color)
{
	ui.pushButton_6->setColor(color);
}

void LabelSettings::setSecColor(const QColor& color)
{
    ui.pushButtonSecColor->setColor(color);
}

void LabelSettings::setFontComboBoxFont(QFont font)
{
	ui.fontComboBox->setCurrentFont(font);
}
// TODO:xunit
void LabelSettings::setComboBoxText(QString text) {}

void LabelSettings::setComboBox_3Text(QString text) {}

void LabelSettings::setComboBox_2Text(int index)
{
	ui.comboBox_2->setCurrentIndex(index);
}

void LabelSettings::setComboBox_4Text(int size)
{
	ui.comboBox_4->setCurrentText(QString("%1").arg(size));
}

void LabelSettings::setSpinBox_2Value(int value)
{
	ui.spinBox_2->setValue(value);
}

void LabelSettings::setSpinBox_3Value(int value)
{
	ui.spinBox_3->setValue(value);
}

/////////////////////////////LabelText//////////////////////////////////
LabelText::LabelText(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	ui.label_14->setVisible(false);
	ui.label_15->setVisible(false);
	ui.pushButton_9->setVisible(false);
	ui.lineEdit_4->setVisible(false);
    connect(ui.comboBox_3,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBox_3CurrentIndexChanged(int)));
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &LabelText::onCheckBoxStateChanged);
	connect(ui.checkBox_2, &QCheckBox::stateChanged, this, &LabelText::onCheckBox_2StateChanged);
	connect(ui.checkBox_3, &QCheckBox::stateChanged, this, &LabelText::onCheckBox_3StateChanged);
	connect(ui.checkBox_4, &QCheckBox::stateChanged, this, &LabelText::onCheckBox_4StateChanged);
	connect(ui.checkBox_5, &QCheckBox::stateChanged, this, &LabelText::onCheckBox_5StateChanged);
	connect(ui.pushButton_9, &QPushButton::clicked, this, &LabelText::onPushbutton_9Clicked);
    connect(
        ui.lineEdit_4, &QLineEdit::editingFinished, this, &LabelText::onLineEdit_4EditingFinished);
}

LabelText::~LabelText() {}

void LabelText::onWidgetEnabled(bool on)
{
	this->setEnabled(on);
}

void LabelText::onComboBox_3CurrentIndexChanged(int index)
{
    switch(index)
	{
	case 0:
		ui.label->setVisible(true);
		ui.label_2->setVisible(true);
		ui.label_3->setVisible(true);
		ui.label_4->setVisible(true);
		ui.label_5->setVisible(true);
		ui.checkBox->setVisible(true);
		ui.checkBox_2->setVisible(true);
		ui.checkBox_3->setVisible(true);
		ui.checkBox_4->setVisible(true);
		ui.checkBox_5->setVisible(true);
		ui.label_14->setVisible(false);
		ui.label_15->setVisible(false);
		ui.pushButton_9->setVisible(false);
		ui.lineEdit_4->setVisible(false);
		break;
	case 1:
		ui.label->setVisible(false);
		ui.label_2->setVisible(false);
		ui.label_3->setVisible(false);
		ui.label_4->setVisible(false);
		ui.label_5->setVisible(false);
		ui.checkBox->setVisible(false);
		ui.checkBox_2->setVisible(false);
		ui.checkBox_3->setVisible(false);
		ui.checkBox_4->setVisible(false);
		ui.checkBox_5->setVisible(false);
		ui.label_14->setVisible(false);
		ui.label_15->setVisible(true);
		ui.pushButton_9->setVisible(false);
		ui.lineEdit_4->setVisible(true);
		break;
	case 2:
		ui.label->setVisible(false);
		ui.label_2->setVisible(false);
		ui.label_3->setVisible(false);
		ui.label_4->setVisible(false);
		ui.label_5->setVisible(false);
		ui.checkBox->setVisible(false);
		ui.checkBox_2->setVisible(false);
		ui.checkBox_3->setVisible(false);
		ui.checkBox_4->setVisible(false);
		ui.checkBox_5->setVisible(false);
		ui.label_14->setVisible(true);
		ui.label_15->setVisible(false);
		ui.pushButton_9->setVisible(true);
		ui.lineEdit_4->setVisible(false);
		break;
	default:
		break;
	}

	emit sigComboBox_3CurrentIndexChanged(index);
}

void LabelText::onCheckBoxStateChanged(int state)
{
	emit sigCheckBoxStateChanged((bool)state);
}

void LabelText::onCheckBox_2StateChanged(int state)
{
	emit sigCheckBox_2StateChanged((bool)state);
}

void LabelText::onCheckBox_3StateChanged(int state)
{
	emit sigCheckBox_3StateChanged((bool)state);
}

void LabelText::onCheckBox_4StateChanged(int state)
{
	emit sigCheckBox_4StateChanged((bool)state);
}

void LabelText::onCheckBox_5StateChanged(int state)
{
	emit sigCheckBox_5StateChanged((bool)state);
}

void LabelText::onPushbutton_9Clicked()
{
	emit sigPushbutton_9Clicked();
}

void LabelText::onLineEdit_4EditingFinished()
{
	emit sigLineEdit_4EditingFinished(ui.lineEdit_4->text());
}

void LabelText::setComboBox_3CurrentIndex(int index)
{
	ui.comboBox_3->setCurrentIndex(index);
}

void LabelText::setCheckBoxCheckState(bool on)
{
	ui.checkBox->setChecked(on);
}

void LabelText::setCheckBox_2CheckState(bool on)
{
	ui.checkBox_2->setChecked(on);
}

void LabelText::setCheckBox_3CheckState(bool on)
{
	ui.checkBox_3->setChecked(on);
}

void LabelText::setCheckBox_4CheckState(bool on)
{
	ui.checkBox_4->setChecked(on);
}

void LabelText::setCheckBox_5CheckState(bool on)
{
	ui.checkBox_5->setChecked(on);
}

void LabelText::setLineEdit_4Text(QString text)
{
	ui.lineEdit_4->setText(text);
}

/////////////////////////////ColorRanges//////////////////////////////////
ColorRanges::ColorRanges(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);
    initComboBox();

    connect(ui.pushButton_more,
            &QPushButton::clicked,
            this,
            &ColorRanges::sigBtnColorRangesMoreclicked);
    connect(ui.checkBoxEnable, &QCheckBox::toggled, this, &ColorRanges::colorRangesEnable);
    connect(ui.pushButtonColor, &ColorButton::clicked, this, [this]() {
        emit colorRangesDefaultColorChange(ui.pushButtonColor->color());
    });
    connect(
        ui.comboBoxMode, QOverload<int32_t>::of(&QComboBox::currentIndexChanged), this, [this]() {
            DataPair::ColorRangeMode mode =
                static_cast<DataPair::ColorRangeMode>(ui.comboBoxMode->currentData().toInt());
            emit colorRangesModeChange(mode);
        });
}

ColorRanges::~ColorRanges() {}

void ColorRanges::setColorRangesEnable(bool enable)
{
    ui.checkBoxEnable->setChecked(enable);
}

void ColorRanges::setColorRangesDefaultColor(const QColor& color)
{
    ui.pushButtonColor->setColor(color);
}

void ColorRanges::setColorRangesMode(DataPair::ColorRangeMode mode)
{
    int32_t index = m_comboMap.value(mode);
    ui.comboBoxMode->setCurrentIndex(index);
}

void ColorRanges::initComboBox()
{
    ui.comboBoxMode->clear();
    ui.comboBoxMode->addItem("单色显示",
                             QVariant::fromValue(static_cast<int32_t>(DataPair::SingleColor)));
    m_comboMap.insert(DataPair::SingleColor, 0);
    ui.comboBoxMode->addItem("多段显示",
                             QVariant::fromValue(static_cast<int32_t>(DataPair::MutilColor)));
    m_comboMap.insert(DataPair::MutilColor, 1);
}

Connections::Connections(QWidget* parent)
{
    QLabel* pLabel = new QLabel("编辑连接");
    pLabel->setFrameShadow(QLabel::Sunken);
    pLabel->setFrameShape(QLabel::Box);
    m_pMoreConVBtn = new QPushButton("更多>>");
    QHBoxLayout* hLayout = new QHBoxLayout;
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->addWidget(pLabel);
    hLayout->addWidget(m_pMoreConVBtn);
    hLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    setLayout(hLayout);
}
