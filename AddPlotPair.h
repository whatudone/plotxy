#ifndef _ADD_PLOT_PAIR_H_
#define _ADD_PLOT_PAIR_H_

#include <QWidget>
#include <QTableWidgetItem>
#include <QPair>
#include <QMap>
#include "ui_AddPlotPair.h"
#include "constdef.h"
#include "PlotItemBase.h"

class AddPlotPair : public QWidget
{
    Q_OBJECT

public:
    explicit AddPlotPair(QWidget *parent = 0);
    ~AddPlotPair();

    void init(PlotType index = PlotType::Type_PlotScatter);

    QStringList m_entityTypeList;
    QStringList m_entityAttrList;
	void onChangeStackIndex(PlotType index);
	void setPlotBaseInfo(BaseInfo);
	
signals:
    void sigAddPlotPair(QString,QString);
	void sgn_addPlotPair(QString, QString, QString, QString);

public slots:
    void onBtnAddClicked();
    void onBtnCloseClicked();
	void onTableWidgetItemClicked(QTableWidgetItem*);
	void onTableWidgetItemClicked_2(QTableWidgetItem*);
	void onTableWidgetItemClicked_3(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude1(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude2(QTableWidgetItem*);
	void onUpdateData();
	void onAddPlot(const QString&, PlotItemBase*);
	
private:
    Ui::AddPlotPair ui;
	BaseInfo m_curPlotInfo;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName
};

#endif // _ADD_PLOT_PAIR_H_
