#ifndef NETSETTINGDIALOG_H
#define NETSETTINGDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui
{
class NetSettingDialog;
}

class NetSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetSettingDialog(QWidget* parent = nullptr);
    ~NetSettingDialog();

private:
    Ui::NetSettingDialog* ui;

    QSettings* settings = nullptr;

private Q_SLOTS:
    void onAccept();
};

#endif // NETSETTINGDIALOG_H
