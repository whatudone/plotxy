#include "SubSettingWidgetContainer.h"
#include "ToolBox.h"
#include <QTextEdit>


SubSettingWidgetContainer::SubSettingWidgetContainer(QWidget* parent)
	: QMainWindow(parent)
{
	this->setFixedWidth(610);

	QString pageTitle;
	ToolBox* toolBox = new ToolBox(this);

	//第一项General
	m_general = new General(this);
	toolBox->addWidget("General", m_general);

	//第二项Extrapolation
	Extrapolation* extrapolation = new Extrapolation(this);
	toolBox->addWidget("Extrapolation", extrapolation);

	//第三项Label Settings
	LabelSettings* label_Settings = new LabelSettings(this);
	toolBox->addWidget("labelSettings", label_Settings);

	//第四项Label Text
	LabelText* label_Text = new LabelText(this);
	toolBox->addWidget("labelText", label_Text);

	//第五项Color Ranges
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

