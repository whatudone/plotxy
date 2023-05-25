#ifndef PLOTXY_DEMO_H_
#define PLOTXY_DEMO_H_

#include "TimeControls.h"
#include "constdef.h"
#include "ui_PlotXYDemo.h"
#include <QContextMenuEvent>
#include <QtWidgets/QMainWindow>

class PlotItemBase;
class PlotManager;
class AddPlotPair;
class AdvancedDataManager;
class TabDrawWidget;
class DataPair;
enum class MouseMode : uint8_t;
class PlotXYDemo : public QMainWindow
{
    Q_OBJECT

public:
    PlotXYDemo(QWidget* parent = Q_NULLPTR);
    ~PlotXYDemo();

    void init(); //初始化函数，连接信号槽
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
    void updateStatusBarInfo();

public slots:
	//menu-file
	void onOpenFile();
	void onOpenNetwork();
    // 导出数据成pxy或者csv，目前先保存为pxy
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
    // 最顶上
    void onBringToTop();
    // 最底下
    void onSendToBottom();
    // 往前
    void onBringForward();
    // 往后
    void onSendBackward();
    // 水平对齐
    void onHorizonAlign();
    // 垂直对齐
    void onVerticalAlign();

    void onLockDataDisplay(bool checked);
    void onLockStackingOrder(bool checked);
	//menu-graph
	void onPlotWizard();
	void onPlotManager();
	void onWidgetEditor();
    void onExportToGOG();

    void onAddPlot();

    void onAutofit_Full(bool checked);
    void onAutofit_X(bool checked);
    void onAutofit_Y(bool checked);
	void onOneToOne();
	void onRoundRanges();
	void onInvertColors();
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
    // 将上述信号槽合并成一个
    void onStatusBtnClicked(int index);

    void onSelectedPlot(const QList<PlotItemBase*> plots);
    void onCustomContextMenuRequested(const QPoint&);
    void onContextMenu(const QPoint& point);

	void onSendTabRect();

    void onSetSliderRange(int min, int max, int singleStep);
    void onSliderValueChanged(int);
    void onRemoteSliderValueChanged(int);

    void onTimeOut();
    void onUpdateLocalTime();

    void onTabDrawWidgetCreatePlot(PlotType type, const QRect& rect);

signals:
    // 通知图表时间轴当前时间发送变化
    void currentSecsChanged(double secs);
    void sgn_enableActionStop(bool);
	void sgn_sendTabWidgetRect(QRect);
	void sgn_renameTabPage(QString, QString);
    // 状态栏鼠标模式切换
    void mouseModeChanged(MouseMode mode);

private:
    void addTabPage(const QString& tabName = QString());
    // 将添加图表控件操作合并到一个函数
    PlotItemBase*
    addPlotWidget(PlotType type, const QRect& geo = QRect(), const QString& plotName = QString());
    TabDrawWidget* getCurDrawWidget();
    // 保存PXY格式工程信息
    void savePXYData(const QString& pxyFileName);
    // 加载PXY格式工程信息，里面带了图表空间信息和数据信息
    void loadPXYData(const QString& pxyFileName);
    // 保存图表信息到Json
    void savePlotInfoToJson(PlotItemBase* plot, QJsonObject& plotObject);
    // 保存数据对信息
    void saveDataPairToJson(DataPair* dataPair, QJsonObject& object);
    // 清理历史创建的tab页和其中的图表
    void clearAllTab();

private:
    Ui::PlotXYDemo ui;
    // 当前选中的图表
    PlotItemBase* m_pCurSelectedPlot = nullptr;
    PlotType m_lastSelectedType;
    MouseMode m_mouseMode;
    PlotManager* m_plotManager;
    AddPlotPair* m_addPlotPair;
    AdvancedDataManager* m_AdvancedDataManager;

    TimeControls* m_timeCtrl;

    QTimer* m_timer;
    int m_timerInterval;
    bool m_bIsPlayForward;

    QTimer* m_localTimer;

    //statusBar widget
    QLabel* m_statusBar_info;
    QToolButton* m_statusBar_EditLock;
    QToolButton* m_statusBar_StackLock;
    QLabel* m_statusBar_dataTime;
    QLabel* m_statusBar_localTime;
    QToolButton* m_statusBar_selectPlot;
    QToolButton* m_statusBar_pan;
    QToolButton* m_statusBar_centerPlot;
    QToolButton* m_statusBar_zoom;
    QToolButton* m_statusBar_boxZoom;
    QToolButton* m_statusBar_measure;
    QToolButton* m_statusBar_createPlot;
    QToolButton* m_statusBar_movePlot;
    QLabel* m_statusBar_null;
};

#endif // !
