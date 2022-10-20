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
	static AddPlotPair* m_getInstance();
	
    ~AddPlotPair();

    void init(PlotType index = PlotType::Type_PlotScatter);
	int textRowCount();
	int textCloumnCount();

	QSet<QString> m_temSet1;
	QSet<QString> m_temSet2;



    QStringList m_entityTypeList;
    QStringList m_entityAttrList;
	void onChangeStackIndex(PlotType index);

signals:
    void sigAddPlotPair(QString,QString);

public slots:
    void onBtnAddClicked();
    void onBtnCloseClicked();
	void onTableWidgetItemClicked(QTableWidgetItem*);
	void onTableWidgetItemClicked_2(QTableWidgetItem*);
	void onTableWidgetItemClicked_3(QTableWidgetItem*);
	void onTableWidgetItemClicked_4(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude1(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude2(QTableWidgetItem*);
	void onUpdateData();
	
private:
    Ui::AddPlotPair ui;
	static AddPlotPair* thispoint;
	explicit AddPlotPair(QWidget *parent = 0);
};

#endif // _ADD_PLOT_PAIR_H_
