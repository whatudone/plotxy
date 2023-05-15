#include "IconSetting.h"
#include <QFileDialog>

IconSetting::IconSetting(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.checkBox, &QCheckBox::stateChanged, this, &IconSetting::onCheckBoxStateChanged);
	connect(ui.pushButton, &QPushButton::clicked, this, &IconSetting::onPushButtonClicked);
    connect(ui.comboBox,
            SIGNAL(currentIndexChanged(int)),
            this,
            SLOT(onComboBoxCurrentIndexChanged(int)));
	connect(ui.checkBox_2, &QCheckBox::stateChanged, this, &IconSetting::onCheckBox_2StateChanged);
	connect(ui.checkBox_3, &QCheckBox::stateChanged, this, &IconSetting::onCheckBox_3StateChanged);
	connect(ui.spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxValueChanged(int)));
	connect(ui.spinBox_2, SIGNAL(valueChanged(int)), this, SLOT(onSpinBox_2ValueChanged(int)));
	connect(ui.pushButton_2, &QPushButton::clicked, this, &IconSetting::onPushButton_2Clicked);
}

IconSetting::~IconSetting() {}

void IconSetting::onCheckBoxStateChanged(int state)
{
	emit sigCheckBoxStateChanged((bool)state);
}

void IconSetting::onPushButtonClicked()
{
    QString fileName = QFileDialog::getOpenFileName(
        this, "Select icon", QDir::currentPath(), "Images (*.png *.icon *.jpg *.bmp)");
    if(!fileName.isEmpty())
	{
		ui.lineEdit->setText(fileName);
		emit sigPushButtonClicked(fileName);
	}
}

void IconSetting::onComboBoxCurrentIndexChanged(int index)
{
	emit sigComboBoxCurrentIndexChanged(index);
}

void IconSetting::onCheckBox_2StateChanged(int state)
{
	emit sigCheckBox_2StateChanged((bool)state);
}

void IconSetting::onCheckBox_3StateChanged(int state)
{
	emit sigCheckBox_3StateChanged((bool)state);
}

void IconSetting::onSpinBoxValueChanged(int value)
{
	emit sigSpinBoxValueChanged(value);
}

void IconSetting::onSpinBox_2ValueChanged(int value)
{
	emit sigSpinBox_2ValueChanged(value);
}

void IconSetting::onPushButton_2Clicked()
{
	emit sigPushButton_2Clicked(ui.pushButton_2->color());
}

void IconSetting::setCheckBoxCheckState(bool state)
{
	ui.checkBox->setChecked(state);
}

void IconSetting::setLineEditText(QString text)
{
	ui.lineEdit->setText(text);
}

void IconSetting::setComboBoxCurrentIndex(int index)
{
	ui.comboBox->setCurrentIndex(index);
}

void IconSetting::setCheckBox_2CheckState(bool state)
{
	ui.checkBox_2->setChecked(state);
}

void IconSetting::setCheckBox_3CheckState(bool state)
{
	ui.checkBox_3->setChecked(state);
}

void IconSetting::setSpinBoxValue(int value)
{
	ui.spinBox->setValue(value);
}

void IconSetting::setSpinBox_2Value(int value)
{
	ui.spinBox_2->setValue(value);
}

void IconSetting::setPushButton_2Color(QColor color)
{
    ui.pushButton_2->setColor(color);
}
