#ifndef CHOOSE_PLOT_TYPE_DIALOG_H
#define CHOOSE_PLOT_TYPE_DIALOG_H

#include <QDialog>

namespace Ui
{
class ChoosePlotTypeDialog;
} // namespace Ui
class QRadioButton;
enum PlotType : int32_t;
class ChoosePlotTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChoosePlotTypeDialog(QWidget* parent = nullptr);
    ~ChoosePlotTypeDialog();

    PlotType getPlotType();

private:
    void initButtonGroup();

private slots:
    void onBtnClicked(int index);

private:
    Ui::ChoosePlotTypeDialog* ui;

    PlotType m_type;
};

#endif // CHOOSE_PLOT_TYPE_DIALOG_H
