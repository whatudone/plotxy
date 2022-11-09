#ifndef  _PLOTXY_DEMO_H_
#define  _PLOTXY_DEMO_H_

#include <QtWidgets/QMainWindow>
#include "QContextMenuEvent"
#include "ui_PlotXYDemo.h"
#include "constdef.h"
#include "TimeControls.h"

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
	void initTime();
	void initStatusBar();
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
	void onAddLightPlot();

	void onFocusChanged(QWidget* oldWidget, QWidget* newWidget);

	void onSetSliderRange(int min, int max, int singleStep);
	void onSliderValueChanged(int);
	void onRemoteSliderValueChanged(int);

	void onActionPlay();
	void onActionStop();
	void onActionReverse();
	void onActionFrameReverse();
	void onActionFrameForward();
	void onActionDecreseStep();
	void onActionIncreaseStep();
	void onActionTimeServer();
	void onActionTimeClient();
	void onActionRealTime();
	void onActionTimeControl();

	void onTimeOut();
	void onUpdateLocalTime();
	
signals:
	void sgn_loadDataReady();
	void sgn_sliderValueChanged(int);
	void sgn_sendCurrentSeconds(double);
private:
    Ui::PlotXYDemo ui;
	QWidget* m_nowFocusWidget;
    //FreeWidgetWraper* m_freeWidgetWraper;
    PlotManager* m_plotManager;
    AddPlotPair* m_addPlotPair;
    AdvancedDataManager* m_AdvancedDataManager;
	BaseInfo m_curBaseInfo;
	PlotType m_lastSelectedType;
	TimeControls* m_timeCtrl;

	QTimer* m_timer;
	int m_timerInterval;
	bool m_bIsPlayForward;

	QTimer *m_localTimer;

	//statusBar widget
	QLabel		*m_statusBar_info;
	QToolButton *m_statusBar_EditLock;
	QToolButton *m_statusBar_layoutLock;
	QLabel		*m_statusBar_dataTime;
	QLabel		*m_statusBar_localTime;
	QToolButton *m_statusBar_selectPlot;
	QToolButton *m_statusBar_pan;
	QToolButton *m_statusBar_centerPlot;
	QToolButton *m_statusBar_zoom;
	QToolButton *m_statusBar_boxZoom;
	QToolButton *m_statusBar_measure;
	QToolButton *m_statusBar_createPlot;
	QToolButton *m_statusBar_movePlot;
	QLabel		*m_statusBar_null;
};

#endif // ! 