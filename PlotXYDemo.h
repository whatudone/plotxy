#ifndef  _PLOTXY_DEMO_H_
#define  _PLOTXY_DEMO_H_

#include <QtWidgets/QMainWindow>
#include "QContextMenuEvent"
#include "ui_PlotXYDemo.h"
#include "constdef.h"

class FreeWidgetWraper;
class PlotItemBase;
class PlotManager;
class AddPlotPair;
class AdvancedDataManager;

class PlotXYDemo : public QMainWindow
{
    Q_OBJECT

public:
    PlotXYDemo(QWidget* parent = Q_NULLPTR);
    ~PlotXYDemo();

    void init();            //初始化函数，连接信号槽
    void initWidget(QWidget* w);
	PlotType getCurrentFocusPlot();

public slots:
    void onAdvancedData();
    void onPlotManager();
    void onAddPlotPair();
    void onOpenFile();

    void onCustomContextMenuRequested(const QPoint&);
	void onContextMenu(const QPoint& point);

    void onAddTabPage();
    void onRemoveTabPage();
    void onRenameTabPage();

    void onAddBarPlot();
	void onAddAttitudePlot();
	void onAddTextPlot();
	void onAddPolarPlot();

	void onFocusChanged(QWidget* oldWidget, QWidget* newWidget);
	
signals:
	void sgn_loadDataReady();
private:
    Ui::PlotXYDemo ui;
	QWidget* m_nowFocusWidget;
    //FreeWidgetWraper* m_freeWidgetWraper;
    PlotManager* m_plotManager;
    AddPlotPair* m_addPlotPair;
    AdvancedDataManager* m_AdvancedDataManager;
};

#endif // ! 