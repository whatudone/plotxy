#pragma once

#include <QWidget>
#include "ui_EventSetting.h"

class EventSetting : public QWidget
{
	Q_OBJECT

public:
	EventSetting(QWidget *parent = Q_NULLPTR);
	~EventSetting();

private:
	Ui::EventSetting ui;

public slots:
	void onBtnMoreClicked();
signals:
	void sgn_BtnMoreClicked();
};
