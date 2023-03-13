#ifndef PLOTXY_DEMO_H_
#define PLOTXY_DEMO_H_

#include <QtWidgets/QMainWindow>
#include <QContextMenuEvent>
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
    PlotXYDemo(QWidget *parent = Q_NULLPTR);
    ~PlotXYDemo();

    void init();            //初始化函数，连接信号槽
	void initMenuFile();
	void initMenuEdit();
	void initMenuGraph();
	void initMenuData();
    void initMenuTime();
	void initMenuTabs();
	void initMenuView();
	void initMenuTools();
	void initMenuHelp();

    void initStatusBar();
    void initWidget(PlotItemBase *w);
	void updateStatusBar_info(QString);
    PlotType getCurrentFocusPlot();

public slots:
	//menu-file
	void onOpenFile();
	void onOpenNetwork();
	void onExportDataStore();
	void onClose_Disconnect();
	void onRunPythonScript();
	void onLoadPML();
	void onSaveToPML();
	void onSaveScreenshot();
	void onQuit();
	//menu-edit
	void onUndo();
	void onRedo();
	void onCut();
	void onCopy();
	void onPaste();
	void onDelete();
	//menu-graph
	void onPlotWizard();
	void onPlotManager();
	void onWidgetEditor();
	void onExportToGOG();
	void onAddBarPlot();
	void onAddAttitudePlot();
	void onAddTextPlot();
	void onAddPolarPlot();
	void onAddLightPlot();
	void onAddTrackPlot();
	void onAddAScopePlot();
	void onAddRTIPlot();
	void onAddDopplerPolt();
	void onAddScatterPlot();
	void onAddDialPlot();
	void onAutofit_Full();
	void onAutofit_X();
	void onAutofit_Y();
	void onOneToOne();
	void onRoundRanges();
	void onLockDataDisplay();
	void onLockStackingOrder();
	void onInvertColors();
	void onBringToFront();
	void onSendToBack();
	//menu-data
	void onAdvancedData();
	void onAddPlotPair();
    void onAddPlotPair(const QString&, const QString&);
	void onDiscoveryRules();
	void onShowObjectAliases();
	void onReferencePoints();
	void onEntityStatus();
	void onClassification();
	//menu-time
	void onPlay();
	void onStop();
	void onReverse();
	void onFrameReverse();
	void onFrameForward();
	void onDecreseStep();
	void onIncreaseStep();
	void onTimeServer();
	void onTimeClient();
	void onRealTime();
	void onTimeControls();
	//menu-tabs
	void onNewTab();
	void onRenameTab();
	void onCloseTab();
	void onNextTab();
	void onPreviousTab();
	//menu-view
	void onShowMenubar();
	void onShowToolbar();
	void onShowStatusbar();
	void onShowTimeSlider();
	void onShowTabbar();
	void onShowRTIControls();
	void onShowAllBars();
	void onShowPlotTime();
	void onShowSystemTime();
	void onShowLogo();
	void onShowFrameRate();
	void onShowFullScreen();
	//menu-tools
	void onSavePreferences();
	void onSavePreferencesAs();
	void onLoadPreferences();
	void onUserFunctions();
	void onFunctionConsole();
	void onHotKeys();
	void onOptions();
	//menu-bookmarks

	//menu-plugin

	//menu-help
	void onUserManual();

	//status-toolbutton
	void onSelectPlot();
	void onPan();
	void onCenterPlot();
	void onZoom();
	void onBoxZoom();
	void onMeasureDistance();
	void onCreatePlot();
	void onMovePlot();

	void onSelectedPlot(QWidget*);
    void onCustomContextMenuRequested(const QPoint &);
    void onContextMenu(const QPoint &point);
 
	void onSendTabRect();

    void onSetSliderRange(int min, int max, int singleStep);
    void onSliderValueChanged(int);
    void onRemoteSliderValueChanged(int);

    void onTimeOut();
    void onUpdateLocalTime();

signals:
    void sgn_loadDataReady();
    void sgn_sliderValueChanged(int);
    void sgn_sendCurrentSeconds(double);
    void sgn_enableActionStop(bool);
	void sgn_sendTabWidgetRect(QRect);
	void sgn_renameTabPage(QString, QString);
private:
    Ui::PlotXYDemo ui;
    //FreeWidgetWraper* m_freeWidgetWraper;
    PlotManager *m_plotManager;
    AddPlotPair *m_addPlotPair;
    AdvancedDataManager *m_AdvancedDataManager;
    BaseInfo m_curBaseInfo;
    PlotType m_lastSelectedType;
    TimeControls *m_timeCtrl;

    QTimer *m_timer;
    int m_timerInterval;
    bool m_bIsPlayForward;

    QTimer *m_localTimer;

    //statusBar widget
    QLabel      *m_statusBar_info;
    QToolButton *m_statusBar_EditLock;
    QToolButton *m_statusBar_layoutLock;
    QLabel      *m_statusBar_dataTime;
    QLabel      *m_statusBar_localTime;
    QToolButton *m_statusBar_selectPlot;
    QToolButton *m_statusBar_pan;
    QToolButton *m_statusBar_centerPlot;
    QToolButton *m_statusBar_zoom;
    QToolButton *m_statusBar_boxZoom;
    QToolButton *m_statusBar_measure;
    QToolButton *m_statusBar_createPlot;
    QToolButton *m_statusBar_movePlot;
    QLabel      *m_statusBar_null;
};

#endif // ! 
