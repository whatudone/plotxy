#include "choose_plot_type_dialog.h"
#include "constdef.h"
#include "ui_choose_plot_type_dialog.h"

#include <QButtonGroup>
ChoosePlotTypeDialog::ChoosePlotTypeDialog(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::ChoosePlotTypeDialog)
    , m_type(PlotType::Type_PlotUnknown)
{
    ui->setupUi(this);
    setWindowTitle("选择图表类型");
    initButtonGroup();
}

ChoosePlotTypeDialog::~ChoosePlotTypeDialog()
{
    delete ui;
}

PlotType ChoosePlotTypeDialog::getPlotType()
{
    return m_type;
}

void ChoosePlotTypeDialog::initButtonGroup()
{
    QButtonGroup* group = new QButtonGroup(this);
    group->addButton(ui->radioButtonAScope, PlotType::Type_PlotAScope);
    group->addButton(ui->radioButtonAttitude, PlotType::Type_PlotAttitude);
    group->addButton(ui->radioButtonBar, PlotType::Type_PlotBar);
    group->addButton(ui->radioButtonDial, PlotType::Type_PlotDial);
    group->addButton(ui->radioButtonDoppler, PlotType::Type_PlotDoppler);
    group->addButton(ui->radioButtonLight, PlotType::Type_PlotLight);
    group->addButton(ui->radioButtonPolar, PlotType::Type_PlotPolar);
    group->addButton(ui->radioButtonRTI, PlotType::Type_PlotRTI);
    group->addButton(ui->radioButtonScatter, PlotType::Type_PlotScatter);
    group->addButton(ui->radioButtonText, PlotType::Type_PlotText);
    group->addButton(ui->radioButtonTrack, PlotType::Type_PlotTrack);
    group->setExclusive(true);

    connect(group,
            QOverload<int>::of(&QButtonGroup::buttonClicked),
            this,
            &ChoosePlotTypeDialog::onBtnClicked,
            Qt::UniqueConnection);
}

void ChoosePlotTypeDialog::onBtnClicked(int index)
{
    m_type = static_cast<PlotType>(index);
}
