#pragma once

#include "ui_rename_tab_dialog.h"
#include <QDialog>

class RenameTabDialog : public QDialog
{
	Q_OBJECT

public:
    RenameTabDialog(QWidget* parent = Q_NULLPTR);
    ~RenameTabDialog();
    QString getNewTabName()
    {
        return m_newTabName;
    }
	void setNewTabName(QString);

public slots:
	void onPushButtonOKClicked();
	void onPushButtonCancelClicked();

private:
    Ui::RenameTabDialog ui;
	QString m_newTabName;
};
