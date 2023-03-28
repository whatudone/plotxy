#ifndef _ADD_PLOT_PAIR_H_
#define _ADD_PLOT_PAIR_H_

#include "PlotItemBase.h"
#include "constdef.h"
#include "ui_AddPlotPair.h"
#include <QMap>
#include <QMenu>
#include <QPair>
#include <QString>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>
#include <QWidgetAction>

struct textUserData
{
	int row;
	int column;
	QString str;
};

class AddPlotPair : public QWidget
{
    Q_OBJECT

public:
    static AddPlotPair* m_getInstance();
    ~AddPlotPair();
    void init(PlotType index = PlotType::Type_PlotScatter);
	//int textRowCount();
	//int textCloumnCount();
	//QList<textUserData> getUserText();
	//int m_textUserX;
	//int m_textUserY;
	//QString m_textUser;

	void onChangeStackIndex(PlotType index);
    void setPlotBaseInfo(PlotItemBase* pBaseItem);

private:
	void initStackedWidget_page1();
	void initStackedWidget_page2();
	void initStackedWidget_page3();
	void initStackedWidget_page4();
	void initStackedWidget_pageLight();
    void initStackedWidget_pageAScope();

    void initTreePlot();
	PlotType getPlotType(PlotItemBase*);
    bool getCurrentSelectParam(QString&, QString&);

signals:
	void sgn_addPlotPair(QString, QString, QString, QString);
    void sgn_updatePlotPair(QString, QString); //tabName, plotName
	void sgn_getLightData(QList<QList<QString>>);
	void sgn_onTextLightBtnClicked();

public slots:
    void updatePlotTrees(const QMap<QString, QList<PlotItemBase*>>& plotData);
    void onBtnAddClicked();
	void onBtnUpdateClicked();
	void onBtnRemoveClicked();
    void onBtnCloseClicked();
	void onTableWidgetItemClicked(QTableWidgetItem*);
	void onTableWidgetItemClicked_2(QTableWidgetItem*);
	void onTableWidgetItemClicked_3(QTableWidgetItem*);
	void onTableWidgetItemClicked_4(QTableWidgetItem*);
    void onTableWidgetLightEntityClicked(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude1(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude2(QTableWidgetItem*);
	void onUpdateData();
	void onAddPlot(const QString&, PlotItemBase*);
    void onDoubleClickedTreeWidgetItem(QTreeWidgetItem* item, int column);
	void onBtnLightAddClicked();
	void onBtnLightDeleteClicked();
	void onBtnLightUpdateClicked();
    void onUpdatePlotPair(PlotItemBase* pBaseItem);

private:
    Ui::AddPlotPair ui;
    static AddPlotPair* m_ptr;
    explicit AddPlotPair(QWidget* parent = 0);

	BaseInfo m_curPlotInfo;
    PlotItemBase* m_pCurSelectedPlot = nullptr;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName

	QTreeWidget* m_treePlot;
	QMenu* m_menuPlot;
	QWidgetAction* m_widgetActionPlot;
};

#endif // _ADD_PLOT_PAIR_H_
