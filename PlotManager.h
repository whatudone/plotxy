#ifndef  _PLOT_MANAGER_H_
#define  _PLOT_MANAGER_H_

#include <QtWidgets/QMainWindow>
#include "ui_PlotManager.h"
#include "PlotItemBase.h"

class PlotManager : public QWidget
{
    Q_OBJECT

public:
    PlotManager(QWidget* parent = Q_NULLPTR);
    ~PlotManager();

    void init();								//初始化函数，连接信号槽
	void addPlot(const QString&, PlotItemBase*);			//添加Plot

	QColor m_axisColor;
	QColor m_gridColor;
	QColor m_girdFill;

	int m_spinBoxBetween;
	int m_spinBoxLeft;
	int m_spinBoxRight;

	int m_hrozGrids;
	int m_vertGrids;

public slots:
    void onTWSclicked(QTreeWidgetItem* item, int i);
    void onTWSPclicked(QTreeWidgetItem* item, int i);
    void onAddNewClicked();

	void onBtnAxisColorClicked();
	void onBtnGridColorClicked();
	void onBtnGridFillClicked();
	void spinboxBetweenChanged();
	void spinboxLeftChanged();
	void spinboxRightChanged();

	void onAddPlotPair(QString,QString);
	void onAddPlotPair(QString tabName, QString plotName, QString xColumn, QString yColumn);

signals:
	void sigAddPlotPair();

private:
    Ui::PlotManager ui;

	QTreeWidgetItem* m_itemGeneral;
	QTreeWidgetItem* m_itemAxis;
	QTreeWidgetItem* m_itemPlotData;
	QTreeWidgetItem* m_itemText;
	QTreeWidgetItem* m_itemGOG;
	QTreeWidgetItem* m_itemLinkedAxis;
	QTreeWidgetItem* m_itemScatterPlot;
	QTreeWidgetItem* m_itemLimits;
	QTreeWidgetItem* m_itemPlotMarkers;
	QTreeWidgetItem* m_itemTimeLine;
	QTreeWidgetItem* m_itemHandsOff;
	QTreeWidgetItem* m_itemAScope;
	QTreeWidgetItem* m_itemRTI;
	QTreeWidgetItem* m_itemTextLight;
	QTreeWidgetItem* m_itemBar;
	QTreeWidgetItem* m_itemDial;
	QTreeWidgetItem* m_itemAttitude;
	QTreeWidgetItem* m_itemTrackStatus;
	QTreeWidgetItem* m_itemRangeDoppler;

    QMap<QString/*int*/, QList<PlotItemBase*>> m_plotManager;	//tabName
};

#endif // ! 