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
};
