#pragma once

#include <QWidget>
#include "ui_IconSetting.h"

class IconSetting : public QWidget
{
	Q_OBJECT

public:
	IconSetting(QWidget *parent = Q_NULLPTR);
	~IconSetting();

private:
	Ui::IconSetting ui;

signals:
	void sigCheckBoxStateChanged(bool);
	void sigPushButtonClicked(QString);
	void sigComboBoxCurrentIndexChanged(int);
	void sigCheckBox_2StateChanged(bool);
	void sigCheckBox_3StateChanged(bool);
	void sigSpinBoxValueChanged(int);
	void sigSpinBox_2ValueChanged(int);
	void sigPushButton_2Clicked(QColor);

public slots:
	void onCheckBoxStateChanged(int);
	void onPushButtonClicked();
	void onComboBoxCurrentIndexChanged(int);
	void onCheckBox_2StateChanged(int);
	void onCheckBox_3StateChanged(int);
	void onSpinBoxValueChanged(int);
	void onSpinBox_2ValueChanged(int);
	void onPushButton_2Clicked();

	void setCheckBoxCheckState(bool);
	void setLineEditText(QString);
	void setComboBoxCurrentIndex(int);
	void setCheckBox_2CheckState(bool);
	void setCheckBox_3CheckState(bool);
	void setSpinBoxValue(int);
	void setSpinBox_2Value(int);
	void setPushButton_2Color(QColor);
};
