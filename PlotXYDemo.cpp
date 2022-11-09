#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QFileDialog>
#include <QMetaType>

#include "PlotXYDemo.h"
#include "FreeWidgetWraper.h"
#include "PlotItemBase.h"
#include "AdvancedDataManager.h"
#include "PlotManager.h"
#include "AddPlotPair.h"
#include "PlotBar.h"
#include "PlotPolar.h"
#include "DataManager.h"
#include "PlotAttitude.h"
#include "PlotText.h"
#include "PlotLight.h"



PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	initTime();

    //m_plotItem = nullptr;
    //m_freeWidgetWraper = nullptr;
	m_AdvancedDataManager = new AdvancedDataManager(this);
    m_plotManager = new PlotManager();
	m_addPlotPair = AddPlotPair::m_getInstance();
	
	connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)),m_plotManager, SLOT(onAddPlotPair(QString, QString)));
	connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)),m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));
	connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
	connect(this, SIGNAL(sgn_loadDataReady()), m_timeCtrl, SLOT(onUpdateData()));
	connect(m_timeCtrl, &TimeControls::sgn_setSliderRange, m_timeCtrl, &TimeControls::onSetSliderRange);
	connect(m_timeCtrl, &TimeControls::sgn_setSliderRange, this, &PlotXYDemo::onSetSliderRange);

	connect(this, &PlotXYDemo::sgn_sliderValueChanged, m_timeCtrl, &TimeControls::onRemoteSliderValueChanged);
	connect(m_timeCtrl, &TimeControls::sgn_sliderValueChanged, this, &PlotXYDemo::onRemoteSliderValueChanged);

	m_curBaseInfo.Base_TabName = nullptr;
	m_curBaseInfo.Base_PlotName = nullptr;
	qRegisterMetaType<BaseInfo>("BaseInfo");

    setMinimumSize(1600, 900);
    showMaximized();

    //statusbar设置左侧信息提示
	/*QLabel* info = new QLabel;
	info->setText(QString::fromLocal8Bit("已选择的图层："));
	ui.statusBar->addWidget(info);*/
	initStatusBar();

    connect(ui.actionAdvanced_Data_Manager, &QAction::triggered, this, &PlotXYDemo::onAdvancedData);
    connect(ui.actionPlot_Manager_Ctrl_M, &QAction::triggered, this, &PlotXYDemo::onPlotManager);
    connect(ui.actionAdd_Plot_Pair_Ctrl_A, &QAction::triggered, this, &PlotXYDemo::onAddPlotPair);
    connect(ui.actionopen, &QAction::triggered, this, &PlotXYDemo::onOpenFile);

	connect(m_plotManager, SIGNAL(sigAddPlotPair()), this, SLOT(onAddPlotPair()));
    
    //右键菜单栏
	ui.tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onCustomContextMenuRequested(const QPoint&)));
	ui.tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.tabWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
    
	m_nowFocusWidget = new QWidget(this);
	connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)), this, SLOT(onFocusChanged(QWidget*, QWidget*)));

	m_lastSelectedType = PlotType::Type_PlotScatter;
}

PlotXYDemo::~PlotXYDemo()
{
	if(m_plotManager)
		m_plotManager->deleteLater();
	if(m_addPlotPair)
		m_addPlotPair->deleteLater();
	if(m_AdvancedDataManager)
		m_AdvancedDataManager->deleteLater();
}

void PlotXYDemo::onAdvancedData()
{
    m_AdvancedDataManager->show();
}

void PlotXYDemo::onPlotManager()
{
    if (!m_plotManager)
    {
        return;
    }
    m_plotManager->show();
}

void PlotXYDemo::onAddPlotPair()
{
    if (!m_addPlotPair)
    {
		m_addPlotPair = AddPlotPair::m_getInstance();
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_plotManager, SLOT(onAddPlotPair(QString, QString)));
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));
		connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
        m_addPlotPair->init(getCurrentFocusPlot());
    }
	m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
	m_addPlotPair->setPlotBaseInfo(m_curBaseInfo);
    m_addPlotPair->show();
}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "open File", "/home", tr("Microsoft CSV(*.csv)"));
//    DataManager::getInstance()->loadCSV(path);
	DataManager::getInstance()->loadCSV_stringTime(path);

	sgn_loadDataReady();
}

void PlotXYDemo::onActionTimeControl()
{
	m_timeCtrl->show();
}

void PlotXYDemo::onCustomContextMenuRequested(const QPoint& point)
{
	qDebug() << sender();
    QMenu* pMenu = new QMenu(this);

    QAction* addTabPage = new QAction(QString::fromLocal8Bit("添加tab页面"), this);
    QAction* removeTabPage = new QAction(QString::fromLocal8Bit("删除tab页面"), this);
    QAction* renameTabPage = new QAction(QString::fromLocal8Bit("重命名tab页面"), this);

    /* 添加菜单项 */
    pMenu->addAction(addTabPage);
    pMenu->addAction(removeTabPage);
    pMenu->addAction(renameTabPage);

    /* 连接槽函数 */
    connect(addTabPage, SIGNAL(triggered()), this, SLOT(onAddTabPage()));
    connect(removeTabPage, SIGNAL(triggered()), this, SLOT(onRemoveTabPage()));
    connect(renameTabPage, SIGNAL(triggered()), this, SLOT(onRenameTabPage()));

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(QCursor::pos());

    /* 释放内存 */
    QList<QAction*> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

void PlotXYDemo::onContextMenu(const QPoint& point)
{
	getCurrentFocusPlot();

	QWidget* subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	QString name = subWidget->objectName();
	if (name == "PlotItemBase")
	{
		name = dynamic_cast<PlotItemBase*>(subWidget)->currName();
	}
	else
	{
		name = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
	}
		
	QMenu* pMenu = new QMenu(this);
	//QWidgetAction
	QWidgetAction* object_action = new QWidgetAction(this);
	QLabel* label = new QLabel(name);
	label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
	label->setStyleSheet("color:rgb(0,128,0);font:Bold");
	object_action->setDefaultWidget(label);

	//createPlot菜单
	QMenu* createPlotMenu = new QMenu(QString::fromLocal8Bit("创建绘图组件"));
	QAction* addBarPlot = new QAction(QString::fromLocal8Bit("添加Bar组件"), this);
	QAction* addAttitudePlot = new QAction(QString::fromLocal8Bit("添加Attitude组件"), this);
	QAction* addTextPlot = new QAction(QString::fromLocal8Bit("添加Text组件"), this);
	QAction* addPolarPlot = new QAction(QString::fromLocal8Bit("添加Polar组件"), this);
	QAction* addLightPlot = new QAction(QString::fromLocal8Bit("添加Light组件"), this);
	/* 添加菜单项 */
	createPlotMenu->addAction(addBarPlot);
	createPlotMenu->addAction(addAttitudePlot);
	createPlotMenu->addAction(addTextPlot);
	createPlotMenu->addAction(addPolarPlot);
	createPlotMenu->addAction(addLightPlot);
	/* 连接槽函数 */
	connect(addBarPlot, SIGNAL(triggered()), this, SLOT(onAddBarPlot()));
	connect(addAttitudePlot, SIGNAL(triggered()), this, SLOT(onAddAttitudePlot()));
	connect(addTextPlot, SIGNAL(triggered()), this, SLOT(onAddTextPlot()));
	connect(addPolarPlot, SIGNAL(triggered()), this, SLOT(onAddPolarPlot()));
	connect(addLightPlot, SIGNAL(triggered()), this, SLOT(onAddLightPlot()));

	//QAction
	QAction* Undo_Action = new QAction(QString::fromLocal8Bit("撤销"), this);
	QAction* Cut_Action = new QAction(QString::fromLocal8Bit("剪切"), this);
	QAction* Copy_Action = new QAction(QString::fromLocal8Bit("复制"), this);
	QAction* Paste_Action = new QAction(QString::fromLocal8Bit("粘贴"), this);
	QAction* Delete_Action = new QAction(QString::fromLocal8Bit("删除"), this);

	QMenu* autofitMenu = new QMenu(QString::fromLocal8Bit("自动适应大小"));
	QAction* autofit_x = new QAction(QString::fromLocal8Bit("X"), this);
	QAction* autofit_y = new QAction(QString::fromLocal8Bit("Y"), this);
	autofitMenu->addAction(autofit_x);
	autofitMenu->addAction(autofit_y);

	QAction* One2One_Action = new QAction(QString::fromLocal8Bit("One-To-One"), this);
	QAction* RoundRanges_Action = new QAction(QString::fromLocal8Bit("Round Ranges"), this);

	QAction* PlotManager_Action = new QAction(QString::fromLocal8Bit("绘图管理器"), this);
	QAction* WidgetEditor_Action = new QAction(QString::fromLocal8Bit("控件编辑"), this);
	QAction* DataManager_Action = new QAction(QString::fromLocal8Bit("高级数据管理器"), this);
	QAction* PlotPair_Action = new QAction(QString::fromLocal8Bit("添加数据对"), this);
	QAction* Screenshot_Action = new QAction(QString::fromLocal8Bit("保存截图"), this);

	connect(PlotManager_Action, SIGNAL(triggered()), this, SLOT(onPlotManager()));
	connect(DataManager_Action, SIGNAL(triggered()), this, SLOT(onAdvancedData()));
	connect(PlotPair_Action, SIGNAL(triggered()), this, SLOT(onAddPlotPair()));
	

	QAction* GOG_Action = new QAction(QString::fromLocal8Bit("导出GOG"), this);
	QAction* HDF5_Action = new QAction(QString::fromLocal8Bit("导出HDF5"), this);

	QMenu* OrderMenu = new QMenu(QString::fromLocal8Bit("Order"));
	QMenu* ViewMenu = new QMenu(QString::fromLocal8Bit("View"));
	QMenu* SelectPlotMenu = new QMenu(QString::fromLocal8Bit("Select Plot"));
	//主菜单
	pMenu->addAction(object_action);
	pMenu->addSeparator();
	pMenu->addMenu(createPlotMenu);
	pMenu->addSeparator();
	pMenu->addAction(Undo_Action);
	pMenu->addSeparator();
	pMenu->addAction(Cut_Action);
	pMenu->addAction(Copy_Action);
	pMenu->addAction(Paste_Action);
	pMenu->addAction(Delete_Action);
	pMenu->addSeparator();
	pMenu->addMenu(autofitMenu);
	pMenu->addAction(One2One_Action);
	pMenu->addAction(RoundRanges_Action);
	pMenu->addSeparator();
	pMenu->addAction(PlotManager_Action);
	pMenu->addAction(WidgetEditor_Action);
	pMenu->addAction(DataManager_Action);
	pMenu->addAction(PlotPair_Action);
	pMenu->addAction(Screenshot_Action);
	pMenu->addSeparator();
	pMenu->addAction(GOG_Action);
	pMenu->addAction(HDF5_Action);
	pMenu->addSeparator();
	pMenu->addMenu(OrderMenu);
	pMenu->addMenu(ViewMenu);
	pMenu->addMenu(SelectPlotMenu);

	/* 在鼠标右键处显示菜单 */
	pMenu->exec(QCursor::pos());
}

void PlotXYDemo::onAddTabPage()
{
    QWidget* tabWidgetItem = new QWidget();
    int currCount = ui.tabWidget->count();
    QString genTabName = QString("Tab ") + QString::number(currCount+1);
    ui.tabWidget->addTab(tabWidgetItem, genTabName);
}

void PlotXYDemo::onRemoveTabPage()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    ui.tabWidget->removeTab(currTabIndex);
}

void PlotXYDemo::onRenameTabPage()
{
    //todo:
    int currTabIndex = ui.tabWidget->currentIndex();
    ui.tabWidget->setTabText(currTabIndex,QString());
}

void PlotXYDemo::onAddBarPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotBar* plotItem = new PlotBar(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
    //bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
    bool res = connect(ui.actionStop, &QAction::triggered, plotItem, &PlotBar::onSwitch);
    res = connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
                 plotItem, &PlotBar::onUpdateColorThresholdMap);

    initWidget(plotItem);

   // 控制其自由移动和缩放
    FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
    m_freeWidgetWraper->setWidget(plotItem);

    m_freeWidgetWraper->setMoveEnable(true);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotBar;
    m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}
void PlotXYDemo::onAddTextPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotText* plotItem = new PlotText(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	//connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
	//	plotItem, &PlotText::onUpdateColorThresholdMap);
	// 好像是数据处理有关的东西

	initWidget(plotItem);

	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotText;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}

void PlotXYDemo::onAddLightPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotLight* plotItem = new PlotLight(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	//connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	//connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
	//	plotItem, &PlotText::onUpdateColorThresholdMap);


	initWidget(plotItem);

	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotLight;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);
}

void PlotXYDemo::onAddPolarPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotPolar* plotItem = new PlotPolar(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	bool res = connect(ui.actionStop, &QAction::triggered, plotItem, &PlotPolar::onSwitch);
	res = connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
		plotItem, &PlotPolar::onUpdateColorThresholdMap);

	initWidget(plotItem);
	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_lastSelectedType = PlotType::Type_PlotPolar;
	m_plotManager->addPlot(currTabText, plotItem); 
	m_addPlotPair->onAddPlot(currTabText, plotItem);

}

void PlotXYDemo::onFocusChanged(QWidget * oldWidget, QWidget * newWidget)
{
	m_nowFocusWidget = newWidget;
}

void PlotXYDemo::onSetSliderRange(int min, int max, int singleStep)
{
	ui.timeSlider->setPageStep(singleStep);
	ui.timeSlider->setMinimum(min);
	ui.timeSlider->setMaximum(max);
}

void PlotXYDemo::onSliderValueChanged(int value)
{
	emit sgn_sliderValueChanged(value);

	double secs = (double)value / m_timeCtrl->getMultiplizer();
	int refYear = m_timeCtrl->getRefYear();
	//show data time
	OrdinalTimeFormatter timeFormat;
	QString dataTime = timeFormat.toString(secs, refYear);
	m_statusBar_dataTime->setText(dataTime);

	//发送数据时间
	emit sgn_sendCurrentSeconds(secs);
}

void PlotXYDemo::onRemoteSliderValueChanged(int value)
{
	ui.timeSlider->setValue(value);
}

void PlotXYDemo::onTimeOut()
{
	int curValue = ui.timeSlider->value();
	int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();

	if (m_bIsPlayForward)
	{
		if (curValue == ui.timeSlider->maximum())
		{
			if (m_timeCtrl->getLoopTime())
			{
				ui.timeSlider->setValue(ui.timeSlider->minimum());
			}
			else
				m_timer->stop();
		}
		else
			ui.timeSlider->setValue(curValue + step);
	}
	else
	{
		if (curValue == ui.timeSlider->minimum())
		{
			if (m_timeCtrl->getLoopTime())
			{
				ui.timeSlider->setValue(ui.timeSlider->maximum());
			}
			else
				m_timer->stop();
		}
		else
			ui.timeSlider->setValue(curValue - step);
	}
}

void PlotXYDemo::onUpdateLocalTime()
{
	QDateTime current_time = QDateTime::currentDateTime();
	QString timestr = current_time.toString("yyyy/MM/dd hh:mm:ss");
	m_statusBar_localTime->setText(timestr);
}

void PlotXYDemo::onActionPlay()
{
	m_bIsPlayForward = true;
	if (m_timer->isActive())
		m_timer->stop();
	m_timer->start(m_timerInterval);
	ui.actionStop->setEnabled(true);
}

void PlotXYDemo::onActionStop()
{
	m_timer->stop();
	ui.actionStop->setEnabled(false);
}

void PlotXYDemo::onActionReverse()
{
	m_bIsPlayForward = false;
	if (m_timer->isActive())
		m_timer->stop();
	m_timer->start(m_timerInterval);
	ui.actionStop->setEnabled(true);
}

void PlotXYDemo::onActionFrameReverse()
{
	if (m_timer->isActive())
		m_timer->stop();

	int curValue = ui.timeSlider->value();
	int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();
	ui.timeSlider->setValue(curValue - step);
	ui.actionStop->setEnabled(false);
}

void PlotXYDemo::onActionFrameForward()
{
	if (m_timer->isActive())
		m_timer->stop();

	int curValue = ui.timeSlider->value();
	int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();
	ui.timeSlider->setValue(curValue + step);
	ui.actionStop->setEnabled(false);
}

void PlotXYDemo::onActionDecreseStep()
{
	if (m_timerInterval >= 8000)
		return;

	if (m_timer->isActive())
		m_timer->stop();

	m_timerInterval *= 2;
	m_timer->start(m_timerInterval);
	ui.actionStop->setEnabled(true);
}

void PlotXYDemo::onActionIncreaseStep()
{
	if (m_timerInterval <= 125)
		return;

	if (m_timer->isActive())
		m_timer->stop();

	m_timerInterval /= 2;
	m_timer->start(m_timerInterval);
	ui.actionStop->setEnabled(true);
}

void PlotXYDemo::onActionTimeServer()
{
}

void PlotXYDemo::onActionTimeClient()
{
}

void PlotXYDemo::onActionRealTime()
{
}

void PlotXYDemo::onAddAttitudePlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotAttitude* plotItem = new PlotAttitude(ui.tabWidget->currentWidget());
	plotItem->setTabName(currTabText);
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotAttitude::onSwitch);
	initWidget(plotItem);

	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();
	plotItem->update();

	m_lastSelectedType = PlotType::Type_PlotAttitude;
	m_plotManager->addPlot(currTabText, plotItem);
	m_addPlotPair->onAddPlot(currTabText, plotItem);	
}

void PlotXYDemo::init()
{

}

void PlotXYDemo::initTime()
{
	m_timer = new QTimer(this);
	m_timerInterval = 1000;
	m_bIsPlayForward = true;

	m_timeCtrl = new TimeControls();

	connect(m_timer, &QTimer::timeout, this, &PlotXYDemo::onTimeOut);
	connect(ui.timeSlider, &QSlider::valueChanged, this, &PlotXYDemo::onSliderValueChanged);

	connect(ui.actionTime_Controls, &QAction::triggered, this, &PlotXYDemo::onActionTimeControl);
	connect(ui.actionPlay, &QAction::triggered, this, &PlotXYDemo::onActionPlay);
	connect(ui.actionStop, &QAction::triggered, this, &PlotXYDemo::onActionStop);
	connect(ui.actionReverse, &QAction::triggered, this, &PlotXYDemo::onActionReverse);
	connect(ui.actionFrame_Forward, &QAction::triggered, this, &PlotXYDemo::onActionFrameForward);
	connect(ui.actionFrame_Rverse, &QAction::triggered, this, &PlotXYDemo::onActionFrameReverse);
	connect(ui.actionIncrease_Step, &QAction::triggered, this, &PlotXYDemo::onActionIncreaseStep);
	connect(ui.actionDecrease_Step, &QAction::triggered, this, &PlotXYDemo::onActionDecreseStep);
	connect(ui.actionTime_Client, &QAction::triggered, this, &PlotXYDemo::onActionTimeClient);
	connect(ui.actionTime_Server, &QAction::triggered, this, &PlotXYDemo::onActionTimeServer);
	connect(ui.actionReal_Time, &QAction::triggered, this, &PlotXYDemo::onActionRealTime);

	//TimeControl
	connect(m_timeCtrl, &TimeControls::sgn_actionPlay, this, &PlotXYDemo::onActionPlay);
	connect(m_timeCtrl, &TimeControls::sgn_actionStop, this, &PlotXYDemo::onActionStop);
	connect(m_timeCtrl, &TimeControls::sgn_actionReverse, this, &PlotXYDemo::onActionReverse);
	connect(m_timeCtrl, &TimeControls::sgn_actionFrameForward, this, &PlotXYDemo::onActionFrameForward);
	connect(m_timeCtrl, &TimeControls::sgn_actionFrameReverse, this, &PlotXYDemo::onActionFrameReverse);
	connect(m_timeCtrl, &TimeControls::sgn_actionIncreaseStep, this, &PlotXYDemo::onActionIncreaseStep);
	connect(m_timeCtrl, &TimeControls::sgn_actionDecreseStep, this, &PlotXYDemo::onActionDecreseStep);
	connect(m_timeCtrl, &TimeControls::sgn_actionTimeClient, this, &PlotXYDemo::onActionTimeClient);
	connect(m_timeCtrl, &TimeControls::sgn_actionTimeServer, this, &PlotXYDemo::onActionTimeServer);
	connect(m_timeCtrl, &TimeControls::sgn_actionRealTime, this, &PlotXYDemo::onActionRealTime);
}

void PlotXYDemo::initStatusBar()
{
	m_statusBar_info = new QLabel(this);
	m_statusBar_EditLock = new QToolButton(this);
	m_statusBar_layoutLock = new QToolButton(this);
	m_statusBar_dataTime = new QLabel("<Data Time>", this);
	m_statusBar_localTime = new QLabel("<Local Time>", this);
	m_statusBar_selectPlot = new QToolButton(this);
	m_statusBar_pan = new QToolButton(this);
	m_statusBar_centerPlot = new QToolButton(this);
	m_statusBar_zoom = new QToolButton(this);
	m_statusBar_boxZoom = new QToolButton(this);
	m_statusBar_measure = new QToolButton(this);
	m_statusBar_createPlot = new QToolButton(this);
	m_statusBar_movePlot = new QToolButton(this);
	m_statusBar_null = new QLabel(this);

	m_statusBar_info->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_statusBar_dataTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_statusBar_localTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	m_statusBar_null->setMinimumSize(QSize(50, 10));
	m_statusBar_null->setEnabled(false);

	ui.statusBar->addWidget(m_statusBar_info, 1);
	ui.statusBar->addWidget(m_statusBar_EditLock);
	ui.statusBar->addWidget(m_statusBar_layoutLock);
	ui.statusBar->addWidget(m_statusBar_dataTime);
	ui.statusBar->addWidget(m_statusBar_localTime);
	ui.statusBar->addWidget(m_statusBar_selectPlot);
	ui.statusBar->addWidget(m_statusBar_pan);
	ui.statusBar->addWidget(m_statusBar_centerPlot);
	ui.statusBar->addWidget(m_statusBar_zoom);
	ui.statusBar->addWidget(m_statusBar_boxZoom);
	ui.statusBar->addWidget(m_statusBar_measure);
	ui.statusBar->addWidget(m_statusBar_createPlot);
	ui.statusBar->addWidget(m_statusBar_movePlot);
	ui.statusBar->addWidget(m_statusBar_null);

	m_localTimer = new QTimer(this);
	connect(m_localTimer, &QTimer::timeout, this, &PlotXYDemo::onUpdateLocalTime);
	m_localTimer->start(1000);
}

void PlotXYDemo::initWidget(QWidget* w)
{
    //设置无边框属性
    w->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);
    //w->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //w->setAttribute(Qt::WA_ShowModal,true);
	w->setAutoFillBackground(true);
    w->setMinimumSize(200, 150);
    w->resize(1600,800);

    //设置下背景颜色区别看
    QPalette palette = w->palette();
    palette.setColor(QPalette::Window, QColor(0,0,0));
    w->setPalette(palette);

    QPushButton* btn = new QPushButton(w);
    btn->setText(QString::fromLocal8Bit("关闭"));
    btn->setGeometry(10, 10, 130, 25);
    connect(btn, SIGNAL(clicked(bool)), w, SLOT(close()));
}

PlotType PlotXYDemo::getCurrentFocusPlot()
{
	QWidget* subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	QString name = subWidget->metaObject()->className();

	QString objname = subWidget->objectName();
	if (objname == "PlotItemBase")
	{
		m_curBaseInfo.Base_PlotName = dynamic_cast<PlotItemBase*>(subWidget)->currName();
		m_curBaseInfo.Base_TabName = dynamic_cast<PlotItemBase*>(subWidget)->currTabName();
	}
	else
	{
		m_curBaseInfo.Base_PlotName = nullptr;
		m_curBaseInfo.Base_TabName = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
	}

	if (name.compare("PlotPlotScatter") == 0)
	{
		m_lastSelectedType =  PlotType::Type_PlotScatter;
	}
	else if (name.compare("PlotAScope") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotAScope;
	} 
	else if (name.compare("PlotRTI") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotRTI;
	}
	else if (name.compare("PlotText") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotText;
	}
	else if (name.compare("PlotLight") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotLight;
	}
	else if (name.compare("PlotBar") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotBar;
	}
	else if (name.compare("PlotDial") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotDial;
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotAttitude;
	}
	else if (name.compare("PlotPolar") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotPolar;
	}
	else if (name.compare("PlotTrack") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotTrack;
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		m_lastSelectedType = PlotType::Type_PlotDoppler;
	}
	else
		m_lastSelectedType = PlotType::Type_PlotScatter;

	return m_lastSelectedType;
}


