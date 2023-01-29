#pragma once

#include <QDialog>
#include "ui_renameTab.h"

class renameTab : public QDialog
{
	Q_OBJECT

public:
	renameTab(QWidget *parent = Q_NULLPTR);
	~renameTab();
	QString getNewTabName() { return m_newTabName; }
	void setNewTabName(QString);

public slots:
	void onPushButtonOKClicked();
	void onPushButtonCancelClicked();

private:
	Ui::renameTab ui;
	QString m_newTabName;
};
