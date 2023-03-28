#include "rename_tab_dialog.h"

RenameTabDialog::RenameTabDialog(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	m_newTabName = "NewTab";
	connect(ui.pushButton_OK, &QPushButton::clicked, this, &RenameTabDialog::onPushButtonOKClicked);
    connect(ui.pushButton_cancel,
            &QPushButton::clicked,
            this,
            &RenameTabDialog::onPushButtonCancelClicked);
}

RenameTabDialog::~RenameTabDialog() {}

void RenameTabDialog::setNewTabName(QString str)
{
	m_newTabName = str;
	ui.lineEdit->setText(m_newTabName);
}

void RenameTabDialog::onPushButtonOKClicked()
{
	m_newTabName = ui.lineEdit->text();
	QDialog::accept();
}

void RenameTabDialog::onPushButtonCancelClicked()
{
	QDialog::reject();
}
