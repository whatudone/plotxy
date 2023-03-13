#pragma once

#include <QWidget>
#include "ui_StippleSetting.h"

class StippleSetting : public QWidget
{
	Q_OBJECT

public:
	StippleSetting(QWidget *parent = Q_NULLPTR);
	~StippleSetting();

private:
	Ui::StippleSetting ui;
};
