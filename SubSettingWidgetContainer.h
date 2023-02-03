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
class Extrapolation;
class LabelSettings;
class LabelText;

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
	Extrapolation* m_extrapolation;
	LabelSettings* m_labelSettings;
	LabelText* m_labelText;
};

class General :public QWidget
{
	Q_OBJECT

public:
	explicit General(QWidget* parent = 0);
	~General();

signals:
	void sigBtnGenneralMoreclicked();
	void sigCheckBox_16StateChanged(bool);
	void sigPushButton_12Clicked(QColor);

public slots:
	void onPushbuttonMoreClicked();
	void onCheckBox_16StateChanged();
	void onPushButton_12Clicked();

	void setCheckBox_16CheckState(bool);
	void setPushButton_12Color(QColor);

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

signals:
	void sigCheckBox_5StateChanged(bool);
	void sigPushButton_5Clicked(QColor);
	void sigPushButton_6Clicked(QColor);
	void sigCheckBoxStateChanged(bool);
	void sigFontComboBoxCurrentFontChanged(QFont);
	void sigComboBoxCurrentTextChanged(QString);
	void sigComboBox_2CurrentIndexChanged(int);
	void sigComboBox_3CurrentTextChanged(QString);
	void sigComboBox_4CurrentTextChanged(int);
	void sigSpinBox_2ValueChanged(int);
	void sigSpinBox_3ValueChanged(int);

public slots:
	void onCheckBox_5StateChanged(int);
	void onPushButton_5Clicked();
	void onPushButton_6Clicked();
	void onCheckBoxStateChanged(int);
	void onFontComboBoxCurrentFontChanged(const QFont&);
	void onComboBoxCurrentTextChanged(QString);
	void onComboBox_2CurrentIndexChanged(int);
	void onComboBox_3CurrentTextChanged(QString);
	void onComboBox_4CurrentTextChanged(QString);
	void onSpinBox_2ValueChanged(int);
	void onSpinBox_3ValueChanged(int);

	void setCheckBox_5CheckState(bool);
	void setPushButton_5Color(QColor);
	void setPushButton_6Color(QColor);
	void setCheckBoxStateChanged(bool);
	void setFontComboBoxFont(QFont);
	void setComboBoxText(QString);
	void setComboBox_3Text(QString);
	void setComboBox_2Text(int);
	void setComboBox_4Text(int);
	void setSpinBox_2Value(int);
	void setSpinBox_3Value(int);
};

class LabelText :public QWidget
{
	Q_OBJECT

public:
	explicit LabelText(QWidget* parent = 0);
	~LabelText();
private:
	Ui::LabelText ui;

signals:
	void sigComboBox_3CurrentIndexChanged(int);
	void sigCheckBoxStateChanged(bool);
	void sigCheckBox_2StateChanged(bool);
	void sigCheckBox_3StateChanged(bool);
	void sigCheckBox_4StateChanged(bool);
	void sigCheckBox_5StateChanged(bool);
	void sigPushbutton_9Clicked();
	void sigLineEdit_4EditingFinished(QString);

public slots:
	void onWidgetEnabled(bool);
	void onComboBox_3CurrentIndexChanged(int);
	void onCheckBoxStateChanged(int);
	void onCheckBox_2StateChanged(int);
	void onCheckBox_3StateChanged(int);
	void onCheckBox_4StateChanged(int);
	void onCheckBox_5StateChanged(int);
	void onPushbutton_9Clicked();
	void onLineEdit_4EditingFinished();

	void setComboBox_3CurrentIndex(int);
	void setCheckBoxCheckState(bool);
	void setCheckBox_2CheckState(bool);
	void setCheckBox_3CheckState(bool);
	void setCheckBox_4CheckState(bool);
	void setCheckBox_5CheckState(bool);
	void setLineEdit_4Text(QString);
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
private:
	Ui::ColorRanges ui;
};



#endif // _SUB_SETTING_WIDGET_CONTAINER_H
