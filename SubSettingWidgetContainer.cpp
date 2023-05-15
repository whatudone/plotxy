#include "SubSettingWidgetContainer.h"
#include "ToolBox.h"
#include <QColorDialog>
#include <QTextEdit>

SubSettingWidgetContainer::SubSettingWidgetContainer(QWidget* parent)
	: QMainWindow(parent)
{
	ToolBox* toolBox = new ToolBox(this);

	//General
	m_general = new General(this);
	toolBox->addWidget("General", m_general);

	//Extrapolation
	m_extrapolation = new Extrapolation(this);
	toolBox->addWidget("Extrapolation", m_extrapolation);

	//icon
	m_iconSetting = new IconSetting(this);
	toolBox->addWidget("Icon", m_iconSetting);

	//Label Settings
	m_labelSettings = new LabelSettings(this);
	toolBox->addWidget("label Settings", m_labelSettings);

	//Label Text
	m_labelText = new LabelText(this);
	toolBox->addWidget("labelText", m_labelText);

	//Stipple
	m_stippleSetting = new StippleSetting(this);
	toolBox->addWidget("Stipple", m_stippleSetting);

	//Event
	m_eventSetting = new EventSetting(this);
	toolBox->addWidget("Events", m_eventSetting);

	//Color Ranges
	m_colorRanges = new ColorRanges(this);
	toolBox->addWidget("colorRanges", m_colorRanges);

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

/////////////////////////////General//////////////////////////////////
General::General(QWidget* parent)
    : QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_more, &QPushButton::clicked, this, &General::onPushbuttonMoreClicked);
	connect(ui.pushButton_12, &QPushButton::clicked, this, &General::onPushButton_12Clicked);
	connect(ui.checkBox_16, &QCheckBox::stateChanged, this, &General::onCheckBox_16StateChanged);
	connect(ui.checkBox_14, &QCheckBox::stateChanged, this, &General::onCheckBox_14StateChanged);
}

General::~General() {}

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
	connect(ui.checkBox, &QCheckBox::stateChanged, this, &LabelSettings::onCheckBoxStateChanged);
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

void LabelSettings::onCheckBoxStateChanged(int state)
{
	emit sigCheckBoxStateChanged((bool)state);
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

void LabelSettings::setCheckBoxStateChanged(bool state)
{
	ui.checkBox->setChecked(state);
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
	connect(ui.pushButton_more, &QPushButton::clicked, this, &ColorRanges::onBtnCRMoreclicked);
}

ColorRanges::~ColorRanges() {}

void ColorRanges::onBtnCRMoreclicked()
{
	emit sigBtnColorRangesMoreclicked();
}
