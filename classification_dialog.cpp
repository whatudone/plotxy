#include "classification_dialog.h"
#include "ui_classification_dialog.h"

ClassificationDialog::ClassificationDialog(const QString& className,
                                           const QColor& color,
                                           int32_t fontSize,
                                           bool isShowIcon,
                                           QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ClassificationDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
    ui->lineEditClass->setText(className);
    ui->pushButtonColor->setColor(color);
    ui->lineEditFontSize->setText(QString::number(fontSize));
    ui->checkBoxShowIcon->setChecked(isShowIcon);
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText("确定");
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText("取消");
}

ClassificationDialog::~ClassificationDialog()
{
    delete ui;
}

QString ClassificationDialog::getClassName()
{
    return ui->lineEditClass->text();
}

QColor ClassificationDialog::getTextColor()
{
    return ui->pushButtonColor->color();
}

int32_t ClassificationDialog::getFontSize()
{
    return ui->lineEditFontSize->text().toInt();
}

bool ClassificationDialog::getIsShowIcon()
{
    return ui->checkBoxShowIcon->isChecked();
}
