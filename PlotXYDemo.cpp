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
#include "PlotTrack.h"
#include "PlotAScope.h"
#include "PlotRTI.h"
#include "PlotDoppler.h"
#include "PlotScatter.h"
#include "PlotDial.h"
#include "renameTab.h"
#include "PlotManagerData.h"


PlotXYDemo::PlotXYDemo(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	setMinimumSize(1600, 900);
    setWindowTitle("绘图");


    init();

    m_AdvancedDataManager = new AdvancedDataManager();
    m_plotManager = new PlotManager();
    m_addPlotPair = AddPlotPair::m_getInstance();

    connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
    connect(this, SIGNAL(sgn_loadDataReady()), m_timeCtrl, SLOT(onUpdateData()));
    connect(m_timeCtrl, &TimeControls::sgn_setSliderRange, m_timeCtrl, &TimeControls::onSetSliderRange);
    connect(m_timeCtrl, &TimeControls::sgn_setSliderRange, this, &PlotXYDemo::onSetSliderRange);

    connect(this, &PlotXYDemo::sgn_sliderValueChanged, m_timeCtrl, &TimeControls::onRemoteSliderValueChanged);
    connect(m_timeCtrl, &TimeControls::sgn_sliderValueChanged, this, &PlotXYDemo::onRemoteSliderValueChanged);
    connect(this, &PlotXYDemo::sgn_enableActionStop, m_timeCtrl, &TimeControls::onEnableActionStop);
	connect(this, SIGNAL(sgn_renameTabPage(QString, QString)), PlotManagerData::getInstance(), SLOT(slotChangeTabName(QString, QString)));

    m_curBaseInfo.Base_TabName = nullptr;
    m_curBaseInfo.Base_PlotName = nullptr;
    qRegisterMetaType<BaseInfo>("BaseInfo");

    connect(m_plotManager, &PlotManager::sigAddPlotPair, this, QOverload<>::of(&PlotXYDemo::onAddPlotPair));
	connect(m_plotManager, SIGNAL(sigAdvancedDataManager()), this, SLOT(onAdvancedData()));
	connect(this, &PlotXYDemo::sgn_sendTabWidgetRect, m_plotManager, &PlotManager::onGetTabWidgetRect);
	connect(m_plotManager, &PlotManager::sigGetTabRect, this, &PlotXYDemo::onSendTabRect);
	connect(m_AdvancedDataManager, SIGNAL(sgnAddPlotPair()), this, SLOT(onAddPlotPair()));
	QRect tabRect = ui.tabWidget->rect();
	emit sgn_sendTabWidgetRect(tabRect);

    //右键菜单栏
    ui.tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget->tabBar(), SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomContextMenuRequested(const QPoint &)));
    ui.tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onContextMenu(const QPoint &)));

    m_lastSelectedType = PlotType::Type_PlotScatter;
}

PlotXYDemo::~PlotXYDemo()
{
    if (m_plotManager)
        m_plotManager->deleteLater();
    if (m_addPlotPair)
        m_addPlotPair->deleteLater();
    if (m_AdvancedDataManager)
        m_AdvancedDataManager->deleteLater();
}

void PlotXYDemo::onAdvancedData()
{
    m_AdvancedDataManager->show();
	m_AdvancedDataManager->activateWindow();
}

void PlotXYDemo::onPlotWizard()
{
}

void PlotXYDemo::onPlotManager()
{
    if (!m_plotManager) {
        return;
    }
	m_plotManager->onSelectedPlot(m_curBaseInfo.Base_TabName, m_curBaseInfo.Base_PlotName);
    m_plotManager->show();
	m_plotManager->activateWindow();
}

void PlotXYDemo::onWidgetEditor()
{
}

void PlotXYDemo::onExportToGOG()
{
}

void PlotXYDemo::onAddPlotPair()
{
    if (!m_addPlotPair) {
        m_addPlotPair = AddPlotPair::m_getInstance();
        connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
        m_addPlotPair->init(getCurrentFocusPlot());
    }
    m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
    m_addPlotPair->setPlotBaseInfo(m_curBaseInfo);
    m_addPlotPair->show();
	m_addPlotPair->activateWindow();
}

void PlotXYDemo::onAddPlotPair(const QString &tabName, const QString &plotName)
{
    if (!m_addPlotPair) {
		m_addPlotPair = AddPlotPair::m_getInstance();
		connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
		m_addPlotPair->init(getCurrentFocusPlot());
	}
	m_curBaseInfo.Base_TabName = tabName;
	m_curBaseInfo.Base_PlotName = plotName;
	m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
	m_addPlotPair->setPlotBaseInfo(m_curBaseInfo);
	m_addPlotPair->show();
	m_addPlotPair->activateWindow();
}

void PlotXYDemo::onDiscoveryRules()
{
}

void PlotXYDemo::onShowObjectAliases()
{
}

void PlotXYDemo::onReferencePoints()
{
}

void PlotXYDemo::onEntityStatus()
{
}

void PlotXYDemo::onClassification()
{
}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "open File", "/home", tr("Microsoft CSV(*.csv)"));
//    DataManager::getInstance()->loadCSV(path);
    DataManager::getInstance()->loadCSV_stringTime(path);

    sgn_loadDataReady();
}

void PlotXYDemo::onUserManual()
{
}

void PlotXYDemo::onSelectPlot()
{
}

void PlotXYDemo::onPan()
{
}

void PlotXYDemo::onCenterPlot()
{
}

void PlotXYDemo::onZoom()
{
}

void PlotXYDemo::onBoxZoom()
{
}

void PlotXYDemo::onMeasureDistance()
{
}

void PlotXYDemo::onCreatePlot()
{
}

void PlotXYDemo::onMovePlot()
{
}

void PlotXYDemo::onSelectedPlot(QWidget* widget)
{
	m_curBaseInfo.Base_TabName = dynamic_cast<PlotItemBase *>(widget)->currTabName();
	m_curBaseInfo.Base_PlotName = dynamic_cast<PlotItemBase *>(widget)->currName();
	updateStatusBar_info(m_curBaseInfo.Base_PlotName);
}

void PlotXYDemo::onTimeControls()
{
    m_timeCtrl->show();
	m_timeCtrl->activateWindow();
}

void PlotXYDemo::onCustomContextMenuRequested(const QPoint &point)
{
    qDebug() << sender();
    QMenu *pMenu = new QMenu(this);

    QAction *addTabPage = new QAction(QString::fromLocal8Bit("添加tab页面"), this);
    QAction *removeTabPage = new QAction(QString::fromLocal8Bit("删除tab页面"), this);
    QAction *renameTabPage = new QAction(QString::fromLocal8Bit("重命名tab页面"), this);

    /* 添加菜单项 */
    pMenu->addAction(addTabPage);
    pMenu->addAction(removeTabPage);
    pMenu->addAction(renameTabPage);

    /* 连接槽函数 */
    connect(addTabPage, SIGNAL(triggered()), this, SLOT(onNewTab()));
    connect(removeTabPage, SIGNAL(triggered()), this, SLOT(onCloseTab()));
    connect(renameTabPage, SIGNAL(triggered()), this, SLOT(onRenameTab()));

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(QCursor::pos());

    /* 释放内存 */
    QList<QAction *> list = pMenu->actions();
    foreach(QAction * pAction, list) delete pAction;
    delete pMenu;
}

void PlotXYDemo::onContextMenu(const QPoint &point)
{
    getCurrentFocusPlot();

    QWidget *subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	if (subWidget == nullptr)
		return;

    QString name = subWidget->objectName();
    if (name == "PlotItemBase") {
        name = dynamic_cast<PlotItemBase *>(subWidget)->currName();
    } else {
        name = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
    }

    QMenu *pMenu = new QMenu(this);
    //QWidgetAction
    QWidgetAction *object_action = new QWidgetAction(this);
    QLabel *label = new QLabel(name);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setStyleSheet("color:rgb(0,128,0);font:Bold");
    object_action->setDefaultWidget(label);

    QMenu *ViewMenu = new QMenu(QString::fromLocal8Bit("View"));

    //主菜单
    pMenu->addAction(object_action);
    pMenu->addSeparator();
    pMenu->addMenu(ui.menu_addPlot);
    pMenu->addSeparator();
    pMenu->addAction(ui.actionUndo_Ctrl_Z);
    pMenu->addSeparator();
    pMenu->addAction(ui.actionCut_Ctrl_X);
    pMenu->addAction(ui.actionCopy_Ctrl_C);
    pMenu->addAction(ui.actionPaste_Ctrl_V);
    pMenu->addAction(ui.actionDelete);
    pMenu->addSeparator();
    pMenu->addMenu(ui.menu_autofit);
    pMenu->addAction(ui.actionOne_To_One);
    pMenu->addAction(ui.actionRound_Ranges);
    pMenu->addSeparator();
    pMenu->addAction(ui.actionPlot_Manager_Ctrl_M);
    pMenu->addAction(ui.actionWidget_Editor);
    pMenu->addAction(ui.actionAdvanced_Data_Manager);
    pMenu->addAction(ui.actionAdd_Plot_Pair_Ctrl_A);
    pMenu->addAction(ui.actionSave_Screenshot);
    pMenu->addSeparator();
    pMenu->addAction(ui.actionExport_to_GOG);
    pMenu->addSeparator();
    pMenu->addMenu(ui.menuOrder);
    pMenu->addMenu(ViewMenu);
    pMenu->addMenu(ui.menuSelect_Plot);

    /* 在鼠标右键处显示菜单 */
    pMenu->exec(QCursor::pos());
}

void PlotXYDemo::onNewTab()
{
    QWidget *tabWidgetItem = new QWidget();
    int currCount = ui.tabWidget->count();
    QString genTabName = QString("Tab ") + QString::number(currCount + 1);
    ui.tabWidget->addTab(tabWidgetItem, genTabName);
	ui.tabWidget->setCurrentIndex(currCount);
}

void PlotXYDemo::onCloseTab()
{
	if (ui.tabWidget->count() <= 1)
		return;

    int currTabIndex = ui.tabWidget->currentIndex();
    ui.tabWidget->removeTab(currTabIndex);
}

void PlotXYDemo::onNextTab()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	ui.tabWidget->setCurrentIndex(currTabIndex + 1);
}

void PlotXYDemo::onPreviousTab()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	ui.tabWidget->setCurrentIndex(currTabIndex - 1);
}

void PlotXYDemo::onShowMenubar()
{
//	menuBar()->setVisible(false);
}

void PlotXYDemo::onShowToolbar()
{

}

void PlotXYDemo::onShowStatusbar()
{
}

void PlotXYDemo::onShowTimeSlider()
{
}

void PlotXYDemo::onShowTabbar()
{
}

void PlotXYDemo::onShowRTIControls()
{
}

void PlotXYDemo::onShowAllBars()
{
}

void PlotXYDemo::onShowPlotTime()
{
}

void PlotXYDemo::onShowSystemTime()
{
}

void PlotXYDemo::onShowLogo()
{
}

void PlotXYDemo::onShowFrameRate()
{
}

void PlotXYDemo::onShowFullScreen()
{
}

void PlotXYDemo::onSavePreferences()
{
}

void PlotXYDemo::onSavePreferencesAs()
{
}

void PlotXYDemo::onLoadPreferences()
{
}

void PlotXYDemo::onUserFunctions()
{
}

void PlotXYDemo::onFunctionConsole()
{
}

void PlotXYDemo::onHotKeys()
{
}

void PlotXYDemo::onOptions()
{
}

void PlotXYDemo::onRenameTab()
{
	renameTab* renameDlg = new renameTab(this);
	int ret = renameDlg->exec();
	if (ret == QDialog::Accepted)
	{
		int currTabIndex = ui.tabWidget->currentIndex();
		QString oldName = ui.tabWidget->tabText(currTabIndex);
		QString newName = renameDlg->getNewTabName();
		ui.tabWidget->setTabText(currTabIndex, newName);

		emit sgn_renameTabPage(oldName, newName);
	}
	renameDlg->deleteLater();
}

void PlotXYDemo::onSendTabRect()
{
	QRect tabRect = ui.tabWidget->rect();
	emit sgn_sendTabWidgetRect(tabRect);
}

void PlotXYDemo::onAddBarPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotBar *plotItem = new PlotBar(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotBar::onGetCurrentSecond);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);
    connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
            plotItem, &PlotBar::onUpdateColorThresholdMap);

    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotBar;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}
void PlotXYDemo::onAddTextPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotText *plotItem = new PlotText(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotText::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);


    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();
    plotItem->update();

    m_lastSelectedType = PlotType::Type_PlotText;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddLightPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotLight *plotItem = new PlotLight(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);

	connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotLight::slot_getCurrentSeconds);
	connect(m_addPlotPair, &AddPlotPair::sgn_getLightData, plotItem, &PlotLight::slot_getLightData);
//	connect(m_addPlotPair,&AddPlotPair::sgn_updatePlotPair,plotItem,&PlotLight::slot_onAddButtonClicked);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotLight;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddTrackPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotTrack *plotItem = new PlotTrack(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotTrack::onGetCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);
    connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
            plotItem, &PlotTrack::onUpdateColorThresholdMap);

    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();
    plotItem->update();

    m_lastSelectedType = PlotType::Type_PlotTrack;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddAScopePlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotAScope *plotItem = new PlotAScope(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
//  connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotAScope::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);
    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotAScope;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddRTIPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotRTI *plotItem = new PlotRTI(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    //  connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotRTI::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);
    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotRTI;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddDopplerPolt()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotDoppler *plotItem = new PlotDoppler(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    //  connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotDoppler::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);
    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotDoppler;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddScatterPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotScatter *plotItem = new PlotScatter(ui.tabWidget->currentWidget());

    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotScatter::onGetCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotScatter;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAddDialPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotDial *plotItem = new PlotDial(ui.tabWidget->currentWidget());

    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotDial::onGetCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();
    plotItem->update();

    m_lastSelectedType = PlotType::Type_PlotDial;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::onAutofit_Full()
{
}

void PlotXYDemo::onAutofit_X()
{
}

void PlotXYDemo::onAutofit_Y()
{
}

void PlotXYDemo::onOneToOne()
{
}

void PlotXYDemo::onRoundRanges()
{
}

void PlotXYDemo::onLockDataDisplay()
{
}

void PlotXYDemo::onLockStackingOrder()
{
}

void PlotXYDemo::onInvertColors()
{
}

void PlotXYDemo::onBringToFront()
{
}

void PlotXYDemo::onSendToBack()
{
}

void PlotXYDemo::onAddPolarPlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotPolar *plotItem = new PlotPolar(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotPolar::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);

    initWidget(plotItem);
    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();

    m_lastSelectedType = PlotType::Type_PlotPolar;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
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

    if (m_bIsPlayForward) {
        if (curValue == ui.timeSlider->maximum()) {
            if (m_timeCtrl->getLoopTime()) {
                ui.timeSlider->setValue(ui.timeSlider->minimum());
            } else {
                m_timer->stop();
                ui.actionStop->setEnabled(false);
				ui.actionPlay->setChecked(false);
				ui.actionReverse->setChecked(false);
                emit sgn_enableActionStop(false);
            }

        } else
            ui.timeSlider->setValue(curValue + step);
    } else {
        if (curValue == ui.timeSlider->minimum()) {
            if (m_timeCtrl->getLoopTime()) {
                ui.timeSlider->setValue(ui.timeSlider->maximum());
            } else {
                m_timer->stop();
                ui.actionStop->setEnabled(false);
				ui.actionPlay->setChecked(false);
				ui.actionReverse->setChecked(false);
                emit sgn_enableActionStop(false);
            }
        } else
            ui.timeSlider->setValue(curValue - step);
    }
}

void PlotXYDemo::onUpdateLocalTime()
{
    QDateTime current_time = QDateTime::currentDateTime();
    QString timestr = current_time.toString("yyyy/MM/dd hh:mm:ss");
    m_statusBar_localTime->setText(timestr);
}

void PlotXYDemo::onPlay()
{
    m_bIsPlayForward = true;
	ui.actionPlay->setChecked(true);
	ui.actionReverse->setChecked(false);
    if (m_timer->isActive())
        m_timer->stop();
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onStop()
{
    m_timer->stop();
    ui.actionStop->setEnabled(false);
	ui.actionPlay->setChecked(false);
	ui.actionReverse->setChecked(false);
    emit sgn_enableActionStop(false);
}

void PlotXYDemo::onReverse()
{
    m_bIsPlayForward = false;
	ui.actionPlay->setChecked(false);
	ui.actionReverse->setChecked(true);
    if (m_timer->isActive())
        m_timer->stop();
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onFrameReverse()
{
    if (m_timer->isActive())
        m_timer->stop();

    int curValue = ui.timeSlider->value();
    int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();
    ui.timeSlider->setValue(curValue - step);
    ui.actionStop->setEnabled(false);
	ui.actionPlay->setChecked(false);
	ui.actionReverse->setChecked(false);
    emit sgn_enableActionStop(false);
}

void PlotXYDemo::onFrameForward()
{
    if (m_timer->isActive())
        m_timer->stop();

    int curValue = ui.timeSlider->value();
    int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();
    ui.timeSlider->setValue(curValue + step);
    ui.actionStop->setEnabled(false);
	ui.actionPlay->setChecked(false);
	ui.actionReverse->setChecked(false);
    emit sgn_enableActionStop(false);
}

void PlotXYDemo::onDecreseStep()
{
    if (m_timerInterval >= 8000)
        return;

    if (m_timer->isActive())
        m_timer->stop();

    m_timerInterval *= 2;
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onIncreaseStep()
{
    if (m_timerInterval <= 125)
        return;

    if (m_timer->isActive())
        m_timer->stop();

    m_timerInterval /= 2;
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onTimeServer()
{
}

void PlotXYDemo::onTimeClient()
{
}

void PlotXYDemo::onRealTime()
{
}

void PlotXYDemo::onAddAttitudePlot()
{
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);

    PlotAttitude *plotItem = new PlotAttitude(ui.tabWidget->currentWidget());
    plotItem->setTabName(currTabText);
    connect(this, &PlotXYDemo::sgn_sendCurrentSeconds, plotItem, &PlotAttitude::slot_getCurrentSeconds);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_plotManager, &PlotManager::onSelectedPlot);
	connect(plotItem, &PlotItemBase::sgn_dataPairChanged, m_AdvancedDataManager, &AdvancedDataManager::onUpdatePlotPair);
    initWidget(plotItem);

    // 控制其自由移动和缩放
    FreeWidgetWraper *m_freeWidgetWraper = new FreeWidgetWraper();
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, m_plotManager, &PlotManager::onMouseEventDone);
	connect(m_freeWidgetWraper, &FreeWidgetWraper::sgnMouseEventDone, this, &PlotXYDemo::onSelectedPlot);
    m_freeWidgetWraper->setWidget(plotItem);
    m_freeWidgetWraper->setMoveEnable(true);

    plotItem->show();
    plotItem->update();

    m_lastSelectedType = PlotType::Type_PlotAttitude;
	PlotManagerData::getInstance()->addPlotManagerData(currTabText, plotItem);
}

void PlotXYDemo::init()
{
	initMenuFile();
	initMenuEdit();
	initMenuGraph();
	initMenuData();
	initMenuTime();
	initMenuTabs();
	initMenuView();
	initMenuTools();
	initMenuHelp();

	initStatusBar();
}

void PlotXYDemo::initMenuFile()
{
	connect(ui.actionopen, &QAction::triggered, this, &PlotXYDemo::onOpenFile);
	connect(ui.actionopenNetwork, &QAction::triggered, this, &PlotXYDemo::onOpenNetwork);
	connect(ui.actionexport, &QAction::triggered, this, &PlotXYDemo::onExportDataStore);
	connect(ui.actionclose, &QAction::triggered, this, &PlotXYDemo::onClose_Disconnect);
	connect(ui.actionRun_Python_Script_Ctrl_E, &QAction::triggered, this, &PlotXYDemo::onRunPythonScript);
	connect(ui.actionLoad_PML, &QAction::triggered, this, &PlotXYDemo::onLoadPML);
	connect(ui.actionSave_to_PML, &QAction::triggered, this, &PlotXYDemo::onSaveToPML);
	connect(ui.actionSave_Screenshot, &QAction::triggered, this, &PlotXYDemo::onSaveScreenshot);
	connect(ui.actionQuit, &QAction::triggered, this, &PlotXYDemo::onQuit);
}

void PlotXYDemo::initMenuEdit()
{
	connect(ui.actionUndo_Ctrl_Z, &QAction::triggered, this, &PlotXYDemo::onUndo);
	connect(ui.actionRedo_Ctrl_Y, &QAction::triggered, this, &PlotXYDemo::onRedo);
	connect(ui.actionCut_Ctrl_X, &QAction::triggered, this, &PlotXYDemo::onCut);
	connect(ui.actionCopy_Ctrl_C, &QAction::triggered, this, &PlotXYDemo::onCopy);
	connect(ui.actionPaste_Ctrl_V, &QAction::triggered, this, &PlotXYDemo::onPaste);
	connect(ui.actionDelete, &QAction::triggered, this, &PlotXYDemo::onDelete);
}

void PlotXYDemo::initMenuGraph()
{
	connect(ui.actionPlot_Wizard, &QAction::triggered, this, &PlotXYDemo::onPlotWizard);
	connect(ui.actionPlot_Manager_Ctrl_M, &QAction::triggered, this, &PlotXYDemo::onPlotManager);
	connect(ui.actionWidget_Editor, &QAction::triggered, this, &PlotXYDemo::onWidgetEditor);
	connect(ui.actionExport_to_GOG, &QAction::triggered, this, &PlotXYDemo::onExportToGOG);
	connect(ui.actionAutofit_Full, &QAction::triggered, this, &PlotXYDemo::onAutofit_Full);
	connect(ui.actionAutofit_X, &QAction::triggered, this, &PlotXYDemo::onAutofit_X);
	connect(ui.actionAutofit_Y, &QAction::triggered, this, &PlotXYDemo::onAutofit_Y);
	connect(ui.actionOne_To_One, &QAction::triggered, this, &PlotXYDemo::onOneToOne);
	connect(ui.actionRound_Ranges, &QAction::triggered, this, &PlotXYDemo::onRoundRanges);
	connect(ui.actionLock_Data_Display, &QAction::triggered, this, &PlotXYDemo::onLockDataDisplay);
	connect(ui.actionLock_Stacking_Order, &QAction::triggered, this, &PlotXYDemo::onLockStackingOrder);
	connect(ui.actionInvert_Colores, &QAction::triggered, this, &PlotXYDemo::onInvertColors);
	connect(ui.actionBring_To_Front_Ctrl, &QAction::triggered, this, &PlotXYDemo::onBringToFront);
	connect(ui.actionSend_To_Back_Ctrl, &QAction::triggered, this, &PlotXYDemo::onSendToBack);

	connect(ui.actionAddBarPlot, SIGNAL(triggered()), this, SLOT(onAddBarPlot()));
	connect(ui.actionAddAttitudePlot, SIGNAL(triggered()), this, SLOT(onAddAttitudePlot()));
	connect(ui.actionAddTextPlot, SIGNAL(triggered()), this, SLOT(onAddTextPlot()));
	connect(ui.actionAddPolarPlot, SIGNAL(triggered()), this, SLOT(onAddPolarPlot()));
	connect(ui.actionAddLightPlot, SIGNAL(triggered()), this, SLOT(onAddLightPlot()));
	connect(ui.actionAddTrackPlot, SIGNAL(triggered()), this, SLOT(onAddTrackPlot()));
	connect(ui.actionAddAScopePlot, SIGNAL(triggered()), this, SLOT(onAddAScopePlot()));
	connect(ui.actionAddRTIPlot, SIGNAL(triggered()), this, SLOT(onAddRTIPlot()));
	connect(ui.actionAddDopplerPlot, SIGNAL(triggered()), this, SLOT(onAddDopplerPolt()));
	connect(ui.actionAddScatterPlot, SIGNAL(triggered()), this, SLOT(onAddScatterPlot()));
	connect(ui.actionAddDialPlot, SIGNAL(triggered()), this, SLOT(onAddDialPlot()));
}

void PlotXYDemo::initMenuData()
{
	connect(ui.actionAdvanced_Data_Manager, &QAction::triggered, this, &PlotXYDemo::onAdvancedData);
	connect(ui.actionAdd_Plot_Pair_Ctrl_A, SIGNAL(triggered()), this, SLOT(onAddPlotPair()));
	connect(ui.actionDiscovery_Rules, &QAction::triggered, this, &PlotXYDemo::onDiscoveryRules);
	connect(ui.actionShow_Object_Aliases_Ctrl_Alt_A, SIGNAL(triggered()), this, SLOT(onShowObjectAliases()));
	connect(ui.actionReference_Points_Ctrl_R, &QAction::triggered, this, &PlotXYDemo::onReferencePoints);
	connect(ui.actionEntity_Status_Ctrl_Alt_S, SIGNAL(triggered()), this, SLOT(onEntityStatus()));
	connect(ui.actionClassification, &QAction::triggered, this, &PlotXYDemo::onClassification);
}

void PlotXYDemo::initMenuTime()
{
    ui.actionStop->setEnabled(false);
    m_timer = new QTimer(this);
    m_timerInterval = 1000;
    m_bIsPlayForward = true;

    m_timeCtrl = new TimeControls();

    connect(m_timer, &QTimer::timeout, this, &PlotXYDemo::onTimeOut);
    connect(ui.timeSlider, &QSlider::valueChanged, this, &PlotXYDemo::onSliderValueChanged);

    connect(ui.actionTime_Controls, &QAction::triggered, this, &PlotXYDemo::onTimeControls);
    connect(ui.actionPlay, &QAction::triggered, this, &PlotXYDemo::onPlay);
    connect(ui.actionStop, &QAction::triggered, this, &PlotXYDemo::onStop);
    connect(ui.actionReverse, &QAction::triggered, this, &PlotXYDemo::onReverse);
    connect(ui.actionFrame_Forward, &QAction::triggered, this, &PlotXYDemo::onFrameForward);
    connect(ui.actionFrame_Rverse, &QAction::triggered, this, &PlotXYDemo::onFrameReverse);
    connect(ui.actionIncrease_Step, &QAction::triggered, this, &PlotXYDemo::onIncreaseStep);
    connect(ui.actionDecrease_Step, &QAction::triggered, this, &PlotXYDemo::onDecreseStep);
    connect(ui.actionTime_Client, &QAction::triggered, this, &PlotXYDemo::onTimeClient);
    connect(ui.actionTime_Server, &QAction::triggered, this, &PlotXYDemo::onTimeServer);
    connect(ui.actionReal_Time, &QAction::triggered, this, &PlotXYDemo::onRealTime);

    //TimeControl
    connect(m_timeCtrl, &TimeControls::sgn_actionPlay, this, &PlotXYDemo::onPlay);
    connect(m_timeCtrl, &TimeControls::sgn_actionStop, this, &PlotXYDemo::onStop);
    connect(m_timeCtrl, &TimeControls::sgn_actionReverse, this, &PlotXYDemo::onReverse);
    connect(m_timeCtrl, &TimeControls::sgn_actionFrameForward, this, &PlotXYDemo::onFrameForward);
    connect(m_timeCtrl, &TimeControls::sgn_actionFrameReverse, this, &PlotXYDemo::onFrameReverse);
    connect(m_timeCtrl, &TimeControls::sgn_actionIncreaseStep, this, &PlotXYDemo::onIncreaseStep);
    connect(m_timeCtrl, &TimeControls::sgn_actionDecreseStep, this, &PlotXYDemo::onDecreseStep);
    connect(m_timeCtrl, &TimeControls::sgn_actionTimeClient, this, &PlotXYDemo::onTimeClient);
    connect(m_timeCtrl, &TimeControls::sgn_actionTimeServer, this, &PlotXYDemo::onTimeServer);
    connect(m_timeCtrl, &TimeControls::sgn_actionRealTime, this, &PlotXYDemo::onRealTime);
}

void PlotXYDemo::initMenuTabs()
{
	connect(ui.actionNew_Tab_Ctrl_T, &QAction::triggered, this, &PlotXYDemo::onNewTab);
	connect(ui.actionRename_Tab, &QAction::triggered, this, &PlotXYDemo::onRenameTab);
	connect(ui.actionClose_Tab, &QAction::triggered, this, &PlotXYDemo::onCloseTab);
	connect(ui.actionNext_Tab_Ctrl_Tab, &QAction::triggered, this, &PlotXYDemo::onNextTab);
	connect(ui.actionPrevious_Tab_Ctrl_Shift_Tab, &QAction::triggered, this, &PlotXYDemo::onPreviousTab);
}

void PlotXYDemo::initMenuView()
{
	connect(ui.actionShow_Menubar_Ctrl_Alt_M, &QAction::triggered, this, &PlotXYDemo::onShowMenubar);
	connect(ui.actionShow_Toolbar_Ctrl_Alt_C, &QAction::triggered, this, &PlotXYDemo::onShowToolbar);
	connect(ui.actionShow_Statusbar_Ctrl_Alt_B, &QAction::triggered, this, &PlotXYDemo::onShowStatusbar);
	connect(ui.actionShow_Time_Slider, &QAction::triggered, this, &PlotXYDemo::onShowTimeSlider);
	connect(ui.actionShow_Tabbar, &QAction::triggered, this, &PlotXYDemo::onShowTabbar);
	connect(ui.actionShow_RTI_Controls, &QAction::triggered, this, &PlotXYDemo::onShowRTIControls);
	connect(ui.actionShow_All_Bars_Ctrl_B, &QAction::triggered, this, &PlotXYDemo::onShowAllBars);
	connect(ui.actionShow_Plot_Time_Ctrl_Alt_P, &QAction::triggered, this, &PlotXYDemo::onShowPlotTime);
	connect(ui.actionShow_System_Time_Ctrl_Alt_Y, &QAction::triggered, this, &PlotXYDemo::onShowSystemTime);
	connect(ui.actionShow_Corner_Logo, &QAction::triggered, this, &PlotXYDemo::onShowLogo);
	connect(ui.actionShow_Frame_Rate_Alt_F, &QAction::triggered, this, &PlotXYDemo::onShowFrameRate);
	connect(ui.actionFullScreen_Shift_F, &QAction::triggered, this, &PlotXYDemo::onShowFullScreen);
}

void PlotXYDemo::initMenuTools()
{
	connect(ui.actionSave_Preferences, &QAction::triggered, this, &PlotXYDemo::onSavePreferences);
	connect(ui.actionSave_Preferences_As, &QAction::triggered, this, &PlotXYDemo::onSavePreferencesAs);
	connect(ui.actionLoad_Preferences, &QAction::triggered, this, &PlotXYDemo::onShowPlotTime);
	connect(ui.actionUser_Functions, &QAction::triggered, this, &PlotXYDemo::onShowSystemTime);
	connect(ui.actionFunction_Console, &QAction::triggered, this, &PlotXYDemo::onFunctionConsole);
	connect(ui.actionHot_Keys_Ctrl_H, &QAction::triggered, this, &PlotXYDemo::onHotKeys);
	connect(ui.actionOptions_Ctrl_Alt_O, &QAction::triggered, this, &PlotXYDemo::onOptions);
}

void PlotXYDemo::initMenuHelp()
{
	connect(ui.actionUser_Manual_F1, &QAction::triggered, this, &PlotXYDemo::onUserManual);
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

    m_statusBar_EditLock->setToolTip("Editing Lock");
    m_statusBar_layoutLock->setToolTip("Stacking Lock");
    m_statusBar_selectPlot->setToolTip("Select Plot");
    m_statusBar_pan->setToolTip("Pan");
    m_statusBar_centerPlot->setToolTip("Center Plot");
    m_statusBar_zoom->setToolTip("Zoom");
    m_statusBar_boxZoom->setToolTip("Box Zoom");
    m_statusBar_measure->setToolTip("Measure");
    m_statusBar_createPlot->setToolTip("Create Plot");
    m_statusBar_movePlot->setToolTip("Move Plot");

    m_statusBar_EditLock->setIcon(QIcon(":/statusbar/editingLock.bmp"));
    m_statusBar_layoutLock->setIcon(QIcon(":/statusbar/stackingLock.bmp"));
    m_statusBar_selectPlot->setIcon(QIcon(":/statusbar/selectPlot.bmp"));
    m_statusBar_pan->setIcon(QIcon(":/statusbar/pan.bmp"));
    m_statusBar_centerPlot->setIcon(QIcon(":/statusbar/centerPlot.bmp"));
    m_statusBar_zoom->setIcon(QIcon(":/statusbar/zoom.bmp"));
    m_statusBar_boxZoom->setIcon(QIcon(":/statusbar/boxZoom.bmp"));
    m_statusBar_measure->setIcon(QIcon(":/statusbar/measureDistance.bmp"));
    m_statusBar_createPlot->setIcon(QIcon(":/statusbar/createPlot.bmp"));
    m_statusBar_movePlot->setIcon(QIcon(":/statusbar/movePlot.bmp"));

    m_statusBar_EditLock->setIconSize(QSize(25, 25));
    m_statusBar_layoutLock->setIconSize(QSize(25, 25));
    m_statusBar_selectPlot->setIconSize(QSize(25, 25));
    m_statusBar_pan->setIconSize(QSize(25, 25));
    m_statusBar_centerPlot->setIconSize(QSize(25, 25));
    m_statusBar_zoom->setIconSize(QSize(25, 25));
    m_statusBar_boxZoom->setIconSize(QSize(25, 25));
    m_statusBar_measure->setIconSize(QSize(25, 25));
    m_statusBar_createPlot->setIconSize(QSize(25, 25));
    m_statusBar_movePlot->setIconSize(QSize(25, 25));

    m_statusBar_info->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusBar_dataTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusBar_localTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);

//  m_statusBar_dataTime->setMinimumSize(QSize(170, 10));
    m_statusBar_dataTime->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_statusBar_null->setMinimumSize(QSize(40, 10));
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

	connect(m_statusBar_EditLock, &QToolButton::triggered, this, &PlotXYDemo::onLockDataDisplay);
	connect(m_statusBar_layoutLock, &QToolButton::triggered, this, &PlotXYDemo::onLockStackingOrder);
	connect(m_statusBar_selectPlot, &QToolButton::triggered, this, &PlotXYDemo::onSelectPlot);
	connect(m_statusBar_pan, &QToolButton::triggered, this, &PlotXYDemo::onPan);
	connect(m_statusBar_centerPlot, &QToolButton::triggered, this, &PlotXYDemo::onCenterPlot);
	connect(m_statusBar_zoom, &QToolButton::triggered, this, &PlotXYDemo::onZoom);
	connect(m_statusBar_boxZoom, &QToolButton::triggered, this, &PlotXYDemo::onBoxZoom);
	connect(m_statusBar_measure, &QToolButton::triggered, this, &PlotXYDemo::onMeasureDistance);
	connect(m_statusBar_movePlot, &QToolButton::triggered, this, &PlotXYDemo::onMovePlot);
	connect(m_statusBar_createPlot, &QToolButton::triggered, this, &PlotXYDemo::onCreatePlot);

    m_localTimer = new QTimer(this);
    connect(m_localTimer, &QTimer::timeout, this, &PlotXYDemo::onUpdateLocalTime);
    m_localTimer->start(1000);

	updateStatusBar_info("");
}

void PlotXYDemo::initWidget(PlotItemBase *w)
{
    //设置无边框属性
    w->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Widget);
//	w->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::Widget);
    //w->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
    //w->setAttribute(Qt::WA_ShowModal,true);
    w->setAutoFillBackground(true);
    w->setMinimumSize(200, 150);
    w->resize(1600, 800);

    //设置下背景颜色区别看
    QPalette palette = w->palette();
    palette.setColor(QPalette::Window, w->getOuterFillColor());
    w->setPalette(palette);

    QPushButton *btn = new QPushButton(w);
    btn->setText(QString::fromLocal8Bit("关闭"));
    btn->setGeometry(10, 10, 130, 25);
    connect(btn, SIGNAL(clicked(bool)), w, SLOT(close()));
}

void PlotXYDemo::updateStatusBar_info(QString info)
{
	m_statusBar_info->setText(QString::fromLocal8Bit("当前选择的Plot：%1").arg(info));
}

PlotType PlotXYDemo::getCurrentFocusPlot()
{
    QWidget *subWidget = QApplication::widgetAt(QCursor::pos().x(), QCursor::pos().y());
	if(subWidget == nullptr)
		return m_lastSelectedType;

    QString name = subWidget->metaObject()->className();

    QString objname = subWidget->objectName();
    if (objname == "PlotItemBase") {
        m_curBaseInfo.Base_PlotName = dynamic_cast<PlotItemBase *>(subWidget)->currName();
        m_curBaseInfo.Base_TabName = dynamic_cast<PlotItemBase *>(subWidget)->currTabName();
    } else {
        m_curBaseInfo.Base_PlotName = nullptr;
        m_curBaseInfo.Base_TabName = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
    }

    if (name.compare("PlotScatter") == 0) {
        m_lastSelectedType =  PlotType::Type_PlotScatter;
    } else if (name.compare("PlotAScope") == 0) {
        m_lastSelectedType = PlotType::Type_PlotAScope;
    } else if (name.compare("PlotRTI") == 0) {
        m_lastSelectedType = PlotType::Type_PlotRTI;
    } else if (name.compare("PlotText") == 0) {
        m_lastSelectedType = PlotType::Type_PlotText;
    } else if (name.compare("PlotLight") == 0) {
        m_lastSelectedType = PlotType::Type_PlotLight;
    } else if (name.compare("PlotBar") == 0) {
        m_lastSelectedType = PlotType::Type_PlotBar;
    } else if (name.compare("PlotDial") == 0) {
        m_lastSelectedType = PlotType::Type_PlotDial;
    } else if (name.compare("PlotAttitude") == 0) {
        m_lastSelectedType = PlotType::Type_PlotAttitude;
    } else if (name.compare("PlotPolar") == 0) {
        m_lastSelectedType = PlotType::Type_PlotPolar;
    } else if (name.compare("PlotTrack") == 0) {
        m_lastSelectedType = PlotType::Type_PlotTrack;
    } else if (name.compare("PlotDoppler") == 0) {
        m_lastSelectedType = PlotType::Type_PlotDoppler;
    } else
        m_lastSelectedType = PlotType::Type_PlotScatter;

    return m_lastSelectedType;
}

void PlotXYDemo::onOpenNetwork()
{
}

void PlotXYDemo::onExportDataStore()
{
}

void PlotXYDemo::onClose_Disconnect()
{
}

void PlotXYDemo::onRunPythonScript()
{
}

void PlotXYDemo::onLoadPML()
{
}

void PlotXYDemo::onSaveToPML()
{
}

void PlotXYDemo::onSaveScreenshot()
{
}

void PlotXYDemo::onQuit()
{
}

void PlotXYDemo::onUndo()
{
}

void PlotXYDemo::onRedo()
{
}

void PlotXYDemo::onCut()
{
}

void PlotXYDemo::onCopy()
{
}

void PlotXYDemo::onPaste()
{
}

void PlotXYDemo::onDelete()
{
}


