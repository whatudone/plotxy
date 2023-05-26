#include "StippleSetting.h"

StippleSetting::StippleSetting(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
    initPatternComboBox();
    initConnections();
}

StippleSetting::~StippleSetting() {}

void StippleSetting::setStippleEnable(bool enable)
{
    ui.checkBoxEnable->setChecked(enable);
}

void StippleSetting::setStipplePattern(Qt::PenStyle style)
{
    ui.comboBoxMode->setCurrentIndex(m_patternMap.keys().indexOf(style));
}

void StippleSetting::setStippleCustomPattern(const QString& pattern)
{
    ui.lineEditCustom->setText(pattern);
}

void StippleSetting::setStippleFactor(int factor)
{
    ui.spinBoxFactor->setValue(factor);
}

void StippleSetting::initPatternComboBox()
{
    m_patternMap.insert(Qt::SolidLine, "Solid");
    m_patternMap.insert(Qt::DashLine, "Dash");
    m_patternMap.insert(Qt::DotLine, "Dot");
    m_patternMap.insert(Qt::DashDotLine, "DashDot");

    auto keys = m_patternMap.keys();
    for(Qt::PenStyle key : keys)
    {
        ui.comboBoxMode->addItem(m_patternMap.value(key), QVariant::fromValue(key));
    }
}

void StippleSetting::initConnections()
{
    connect(ui.checkBoxEnable, &QCheckBox::toggled, this, &StippleSetting::stippleEnableChanged);
    connect(
        ui.comboBoxMode, QOverload<int>::of(&QComboBox::currentIndexChanged), [this](int index) {
            auto style = m_patternMap.keys().at(index);
            emit stipplePatternChanged(style);
        });
    connect(ui.lineEditCustom, &QLineEdit::editingFinished, this, [this]() {
        emit stippleCustomPatternChanged(ui.lineEditCustom->text());
    });
    connect(ui.spinBoxFactor,
            QOverload<int>::of(&QSpinBox::valueChanged),
            this,
            &StippleSetting::stippleFactorChanged);
}
