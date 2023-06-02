#include "NetSettingDialog.h"
#include "ui_NetSettingDialog.h"

#include <QDebug>
#include <QFile>

NetSettingDialog::NetSettingDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::NetSettingDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowCloseButtonHint);

    QString iniFileName;
    iniFileName = QCoreApplication::applicationDirPath();
    iniFileName += "/PlotXY.ini";

    settings = new QSettings(iniFileName, QSettings::IniFormat);
    if(!QFile::exists(iniFileName))
    {
        settings->setValue("netConfig/address", "127.0.0.1");
        settings->setValue("netConfig/port", 8888);
    }
    ui->lineEdit_Address->setText(settings->value("netConfig/address").toString());
    ui->lineEdit_Port->setText(settings->value("netConfig/port").toString());

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &NetSettingDialog::onAccept);
}

NetSettingDialog::~NetSettingDialog()
{
    delete ui;
}

void NetSettingDialog::onAccept()
{
    if(settings)
    {
        settings->setValue("netConfig/address", ui->lineEdit_Address->text());
        settings->setValue("netConfig/port", ui->lineEdit_Port->text());
    }
}
