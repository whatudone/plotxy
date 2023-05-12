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
    explicit AddPlotPair(QWidget* parent = 0);
    ~AddPlotPair();
    void init(PlotType index = PlotType::Type_PlotScatter);

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

    bool getCurrentSelectParam(int32_t& xEntityID,
                               QString& xAttrName,
                               QString& xAttrUnitName,
                               int32_t& yEntityID,
                               QString& yAttrName,
                               QString& yAttrUnitName);

    // 根据点击的实体选项切换对应表格里面的属性列表
    void updateAttrTableWidgetOnEntityChanged(QTableWidgetItem* entityItem,
                                              QTableWidget* attrTableWidget);

signals:
	void sgn_addPlotPair(QString, QString, QString, QString);
    void sgn_updatePlotPair(QString, QString); //tabName, plotName
    void
    lightConstraintUpdate(const QList<std::tuple<int32_t, QString, QString, double, QString>>&);
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
    // light约束条件表格双击
    void onLightSetDbClicked(QTableWidgetItem* item);
	void onTableWidgetItemClicked_Attitude1(QTableWidgetItem*);
	void onTableWidgetItemClicked_Attitude2(QTableWidgetItem*);
    // 加载数据后重新更新所有的实体表格数据
    void onUpdateEntityTableByDataChanged();
	void onAddPlot(const QString&, PlotItemBase*);
    void onDoubleClickedTreeWidgetItem(QTreeWidgetItem* item, int column);
	void onBtnLightAddClicked();
	void onBtnLightDeleteClicked();
	void onBtnLightUpdateClicked();
    void onUpdatePlotPair(PlotItemBase* pBaseItem);

private:
    Ui::AddPlotPair ui;

    QString m_currTabName;
    QString m_currPlotName;
    // 添加数据内部选中的图表，方便直接操作，不影响主界面上选中的图表
    PlotItemBase* m_pCurSelectedPlot = nullptr;

	QTreeWidget* m_treePlot;
	QMenu* m_menuPlot;
	QWidgetAction* m_widgetActionPlot;
};

#endif // _ADD_PLOT_PAIR_H_
