#pragma once

#include <QWidget>
#include "ui_TimeControls.h"

class TimeControls : public QWidget
{
	Q_OBJECT

public:
	TimeControls(QWidget *parent = Q_NULLPTR);
	~TimeControls();

private:
	Ui::TimeControls ui;
};
