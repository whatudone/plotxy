#include "renameTab.h"

renameTab::renameTab(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_newTabName = "NewTab";
	connect(ui.pushButton_OK, &QPushButton::clicked, this, &renameTab::onPushButtonOKClicked);
	connect(ui.pushButton_cancel, &QPushButton::clicked, this, &renameTab::onPushButtonCancelClicked);
}

renameTab::~renameTab()
{
}

void renameTab::setNewTabName(QString str)
{
	m_newTabName = str;
	ui.lineEdit->setText(m_newTabName);
}

void renameTab::onPushButtonOKClicked()
{
	m_newTabName = ui.lineEdit->text();
	QDialog::accept();
}

void renameTab::onPushButtonCancelClicked()
{
	QDialog::reject();
}
