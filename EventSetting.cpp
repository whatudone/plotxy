#include "EventSetting.h"

EventSetting::EventSetting(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	connect(ui.pushButton_more, &QPushButton::clicked, this, &EventSetting::onBtnMoreClicked);
}

EventSetting::~EventSetting()
{
}

void EventSetting::onBtnMoreClicked()
{
	emit sgn_BtnMoreClicked();
}