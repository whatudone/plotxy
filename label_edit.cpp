#include "label_edit.h"
#include "ui_label_edit.h"

#include <QKeyEvent>
LabelEdit::LabelEdit(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::LabelEdit)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);
    ui->label->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    setContentsMargins(0, 0, 0, 0);

    setAutoFillBackground(true);
    ui->stackedWidget->setAutoFillBackground(true);
    ui->stackedWidget->widget(0)->setAutoFillBackground(true);
    ui->label->setAutoFillBackground(true);
    ui->lineEdit->setAutoFillBackground(true);
    /*
     * 将stackwidget背景设置为透明，然后底下的子控件全部背景变为透明
     * 最上层LabelWidget负责显示窗口背景色
     * 最底下的Label负责显示文本前景色
     * lineedit需要手动设置qss，避免背景透明之后，文本默认黑色无法看清
    */
    ui->stackedWidget->setStyleSheet("background: transparent;");

    ui->lineEdit->setStyleSheet("background: white;color: black");
}

LabelEdit::~LabelEdit()
{
    delete ui;
}

void LabelEdit::setLabelText(const QString& text)
{
    ui->label->setText(text);
}

void LabelEdit::setLabelFont(const QFont& font)
{
    ui->label->setFont(font);
}

void LabelEdit::setLabelColor(const QColor& textColor, const QColor& fillColor)
{
    QPalette pe(ui->label->palette());
    pe.setColor(QPalette::WindowText, textColor);
    pe.setColor(QPalette::Background, fillColor);

    setPalette(pe);
    ui->label->setPalette(pe);
}

bool LabelEdit::eventFilter(QObject* obj, QEvent* evt)
{
    if(obj == ui->label)
    {
        if(evt->type() == QEvent::MouseButtonDblClick)
        {
            ui->lineEdit->setText(ui->label->text());
            ui->stackedWidget->setCurrentIndex(1);
        }
    }
    else if(obj == ui->lineEdit)
    {
        if(evt->type() == QEvent::KeyPress)
        {
            QKeyEvent* keyevt = static_cast<QKeyEvent*>(evt);
            if((keyevt->key() == Qt::Key_Return) || (keyevt->key() == Qt::Key_Escape) ||
               (keyevt->key() == Qt::Key_Enter))
            {
                ui->label->setText(ui->lineEdit->text());
                ui->stackedWidget->setCurrentIndex(0);
                emit textChanged(ui->lineEdit->text());
            }
        }
        else if(evt->type() == QEvent::FocusOut)
        {
            ui->label->setText(ui->lineEdit->text());
            ui->stackedWidget->setCurrentIndex(0);
        }
    }

    return QWidget::eventFilter(obj, evt);
}
