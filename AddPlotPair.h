#ifndef _ADD_PLOT_PAIR_H_
#define _ADD_PLOT_PAIR_H_

#include <QWidget>
#include <QTableWidgetItem>
#include "ui_AddPlotPair.h"
#include "constdef.h"

class AddPlotPair : public QWidget
{
    Q_OBJECT

public:
    explicit AddPlotPair(QWidget *parent = 0);
    ~AddPlotPair();

    void init(PlotType index = PlotType::Type_PlotScatter);

    QStringList m_entityTypeList;
    QStringList m_entityAttrList;

signals:
    void sigAddPlotPair(QString,QString);

public slots:
    void onBtnAddClicked();
    void onBtnCloseClicked();
	void onTableWidgetItemClicked(QTableWidgetItem*);
	void updateData();
private:
    Ui::AddPlotPair ui;
};

#endif // _ADD_PLOT_PAIR_H_
