#ifndef _SUB_SETTING_WIDGET_CONTAINER_H
#define _SUB_SETTING_WIDGET_CONTAINER_H

#include <QMainWindow>
#include "ui_General.h"
#include "ui_Extrapolation.h"
#include "ui_LabelSettings.h"
#include "ui_LabelText.h"
#include "ui_ColorRanges.h"
#include "IconSetting.h"
#include "StippleSetting.h"
#include "EventSetting.h"

class General;
class ColorRanges;

class SubSettingWidgetContainer : public QMainWindow
{
	Q_OBJECT

public:
	SubSettingWidgetContainer(QWidget* parent = nullptr);
	~SubSettingWidgetContainer();

	General* m_general;
	ColorRanges* m_colorRanges;
	IconSetting* m_iconSetting;
	StippleSetting* m_stippleSetting;
	EventSetting* m_eventSetting;
};

class General :public QWidget
{
	Q_OBJECT

public:
	explicit General(QWidget* parent = 0);
	~General();

signals:
	void sigBtnGenneralMoreclicked();

public slots:
	void onPushbuttonMoreClicked();
	void onBtnColorClicked();

private:
	Ui::General ui;
};

class Extrapolation :public QWidget
{
	Q_OBJECT

public:
	explicit Extrapolation(QWidget* parent = 0);
	~Extrapolation();
private:
	Ui::Extrapolation ui;
};

class LabelSettings :public QWidget
{
	Q_OBJECT

public:
	explicit LabelSettings(QWidget* parent = 0);
	~LabelSettings();
private:
	Ui::LabelSettings ui;
};

class LabelText :public QWidget
{
	Q_OBJECT

public:
	explicit LabelText(QWidget* parent = 0);
	~LabelText();
private:
	Ui::LabelText ui;
};

class ColorRanges :public QWidget
{
	Q_OBJECT

public:
	explicit ColorRanges(QWidget* parent = 0);
	~ColorRanges();
signals:
	void sigBtnColorRangesMoreclicked();
public slots:
	void onBtnCRMoreclicked();
	void onBtnColorClicked();
private:
	Ui::ColorRanges ui;
};



#endif // _SUB_SETTING_WIDGET_CONTAINER_H
