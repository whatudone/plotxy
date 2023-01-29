#include "SubSettingWidgetContainer.h"
#include "ToolBox.h"
#include <QTextEdit>
#include <QColorDialog>

SubSettingWidgetContainer::SubSettingWidgetContainer(QWidget* parent)
	: QMainWindow(parent)
{
	ToolBox* toolBox = new ToolBox(this);

	//General
	m_general = new General(this);
	toolBox->addWidget("General", m_general);

	//Extrapolation
	Extrapolation* extrapolation = new Extrapolation(this);
	toolBox->addWidget("Extrapolation", extrapolation);

	//icon
	m_iconSetting = new IconSetting(this);
	toolBox->addWidget("Icon", m_iconSetting);

	//Label Settings
	LabelSettings* label_Settings = new LabelSettings(this);
	toolBox->addWidget("label Settings", label_Settings);

	//Stipple
	m_stippleSetting = new StippleSetting(this);
	toolBox->addWidget("Stipple", m_stippleSetting);

	//Event
	m_eventSetting = new EventSetting(this);
	toolBox->addWidget("Events", m_eventSetting);

	//Label Text
	LabelText* label_Text = new LabelText(this);
	toolBox->addWidget("labelText", label_Text);

	//Color Ranges
	m_colorRanges = new ColorRanges(this);
	toolBox->addWidget("colorRanges", m_colorRanges);

	setCentralWidget(toolBox);
}

SubSettingWidgetContainer::~SubSettingWidgetContainer()
{

}

General::General(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_more, &QPushButton::clicked, this, &General::onPushbuttonMoreClicked);
}

General::~General()
{
}

void General::onPushbuttonMoreClicked()
{
	emit sigBtnGenneralMoreclicked();
}


Extrapolation::Extrapolation(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);
}

Extrapolation::~Extrapolation()
{
}

LabelSettings::LabelSettings(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);
}

LabelSettings::~LabelSettings()
{
}

LabelText::LabelText(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);
}

LabelText::~LabelText()
{
}

ColorRanges::ColorRanges(QWidget* parent) :
	QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_more, &QPushButton::clicked, this, &ColorRanges::onBtnCRMoreclicked);
}

ColorRanges::~ColorRanges()
{
}

void ColorRanges::onBtnCRMoreclicked()
{
	emit sigBtnColorRangesMoreclicked();
}

