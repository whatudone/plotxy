#include "PlotXYDemo.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QCloseEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QPushButton>
#include <QTemporaryDir>

#include "AddPlotPair.h"
#include "AdvancedDataManager.h"
#include "DataManager.h"
#include "NetSettingDialog.h"
#include "PlotAScope.h"
#include "PlotAttitude.h"
#include "PlotBar.h"
#include "PlotDial.h"
#include "PlotDoppler.h"
#include "PlotItemBase.h"
#include "PlotLight.h"
#include "PlotManager.h"
#include "PlotManagerData.h"
#include "PlotPolar.h"
#include "PlotRTI.h"
#include "PlotScatter.h"
#include "PlotText.h"
#include "PlotTrack.h"
#include "Utils.h"
#include "rename_tab_dialog.h"
#include "tabdrawwidget.h"

#include "H5Cpp.h"
using namespace H5;
double PlotXYDemo::m_seconds = 0.0;
bool PlotXYDemo::m_isRealTime = false;

PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
    , m_mouseMode(MouseMode::SelectPlot)
{
    ui.setupUi(this);
#ifndef QT_DEBUG
    setWindowFlag(Qt::FramelessWindowHint, true);
#endif

    init();

    m_AdvancedDataManager = new AdvancedDataManager();
    m_plotManager = new PlotManager();
    m_addPlotPair = new AddPlotPair();

    connect(DataManager::getInstance(),
            &DataManager::loadDataFinished,
            m_addPlotPair,
            &AddPlotPair::onUpdateEntityTableByDataChanged);
    connect(
        DataManager::getInstance(), SIGNAL(loadDataFinished()), m_timeCtrl, SLOT(onUpdateData()));
    connect(DataManager::getInstance(),
            &DataManager::updateRealTime,
            m_timeCtrl,
            &TimeControls::onUpdateRealData);

    connect(m_plotManager,
            &PlotManager::sigAddPlotPair,
            this,
            QOverload<>::of(&PlotXYDemo::onAddPlotPair));
    connect(m_plotManager, SIGNAL(sigAdvancedDataManager()), this, SLOT(onAdvancedData()));

    connect(m_plotManager, &PlotManager::sigGetTabRect, this, &PlotXYDemo::onSendTabRect);
    connect(m_AdvancedDataManager, SIGNAL(sgnAddPlotPair()), this, SLOT(onAddPlotPair()));

    m_plotManager->setTabWidgetRect(ui.tabWidget->rect());
    // 默认增加一页
    addTabPage();
    //右键菜单栏
    ui.tabWidget->tabBar()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget->tabBar(),
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(onCustomContextMenuRequested(const QPoint&)));
    ui.tabWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.tabWidget,
            SIGNAL(customContextMenuRequested(const QPoint&)),
            this,
            SLOT(onContextMenu(const QPoint&)));
    m_lastSelectedType = PlotType::Type_PlotScatter;
}

PlotXYDemo::~PlotXYDemo()
{
    if(m_plotManager)
        m_plotManager->deleteLater();
    if(m_addPlotPair)
    {
        m_addPlotPair->deleteLater();
    }
    if(m_AdvancedDataManager)
        m_AdvancedDataManager->deleteLater();
    if(m_timeCtrl)
    {
        m_timeCtrl->deleteLater();
    }
}

void PlotXYDemo::onAdvancedData()
{
    m_AdvancedDataManager->onUpdatePlotPair();
    m_AdvancedDataManager->showNormal();
    m_AdvancedDataManager->activateWindow();
}

void PlotXYDemo::onPlotWizard() {}

void PlotXYDemo::onPlotManager()
{
    if(!m_plotManager)
    {
        return;
    }
    m_plotManager->onSelectedPlot(m_pCurSelectedPlot);
    m_plotManager->showNormal();
    m_plotManager->activateWindow();
}

void PlotXYDemo::onWidgetEditor() {}

void PlotXYDemo::onExportToGOG()
{
    // 导出的DataPair需要是绘制在界面上的数据，而不是所有数据
    QString gogFilePath =
        QFileDialog::getSaveFileName(nullptr, "导出GOG格式数据", ".", "GOG (*.gog)");
    if(gogFilePath.isEmpty())
    {
        return;
    }
    if(m_pCurSelectedPlot)
    {
        m_pCurSelectedPlot->exportDataToFile(gogFilePath);
    }
}

void PlotXYDemo::onAddPlotPair()
{
    m_addPlotPair->onUpdateEntityTableByDataChanged();
    m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
    m_addPlotPair->setPlotBaseInfo(m_pCurSelectedPlot);
    m_addPlotPair->onUpdatePlotPair(m_pCurSelectedPlot);
    m_addPlotPair->show();
    m_addPlotPair->activateWindow();
}

void PlotXYDemo::onAddPlotPair(const QString& tabName, const QString& plotName)
{
    // TODO:后续需要确认此处是否是需要当前选中的图表数据，或者是在其他图表列表中任意选择的图表，在后台更新他的数据
    if(m_pCurSelectedPlot && (m_pCurSelectedPlot->getTabName() == tabName) &&
       (m_pCurSelectedPlot->getName() == plotName))
    {
        m_addPlotPair->onChangeStackIndex(m_lastSelectedType);
        m_addPlotPair->setPlotBaseInfo(m_pCurSelectedPlot);
        m_addPlotPair->onUpdatePlotPair(m_pCurSelectedPlot);
        m_addPlotPair->show();
        m_addPlotPair->activateWindow();
    }
}

void PlotXYDemo::onDiscoveryRules() {}

void PlotXYDemo::onShowObjectAliases() {}

void PlotXYDemo::onReferencePoints() {}

void PlotXYDemo::onEntityStatus() {}

void PlotXYDemo::onClassification() {}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(
        this, "打开文件", getDatasPath(), tr("ASI(*.asi) ;; PXY(*.pxy)"));
    if(path.isEmpty())
    {
        return;
    }
    QFileInfo info(path);

    QString suffix = info.suffix().toLower();
    if(suffix == "asi")
    {
        DataManager::getInstance()->loadFileData(path);
    }
    else
    {
        loadPXYData(path);
    }
    setWindowTitle(QString("仿真数据分析:[%1]").arg(path));
}

void PlotXYDemo::onUserManual() {}

void PlotXYDemo::onSelectPlot() {}

void PlotXYDemo::onPan() {}

void PlotXYDemo::onCenterPlot() {}

void PlotXYDemo::onZoom() {}

void PlotXYDemo::onBoxZoom() {}

void PlotXYDemo::onMeasureDistance() {}

void PlotXYDemo::onCreatePlot() {}

void PlotXYDemo::onMovePlot() {}

void PlotXYDemo::onStatusBtnClicked(int index)
{
    MouseMode mode = static_cast<MouseMode>(index);
    m_mouseMode = mode;
    // 通知已经存在的图表刷新鼠标模式
    TabDrawWidget::setMouseMode(mode);
    // 刷新光标
    auto draw = getCurDrawWidget();
    if(!draw)
        return;
    // 切换到其他
    if(m_mouseMode != MouseMode::MovePlot)
    {
        draw->updatePlotsBorderVisible(false);
    }
    else
    {
        draw->updatePlotsBorderVisible(true);
    }

    if(!m_pCurSelectedPlot)
        return;
    m_pCurSelectedPlot->clearInter();
    switch(m_mouseMode)
    {
    // 移动缩放和测量的功能不能同时生效，有bug需要修改
    case MouseMode::SelectPlot:
        draw->setCursor(Qt::ArrowCursor);
        m_pCurSelectedPlot->setCustomPlotMouseTransparent(true);
        break;
    case MouseMode::Pan:
        draw->setCursor(QCursor(QPixmap(":/pan.svg")));
        m_pCurSelectedPlot->setCustomPlotMouseTransparent(false);
        m_pCurSelectedPlot->setInteract(QCP::iRangeDrag);
        break;
    case MouseMode::CenterPlot:
        draw->setCursor(QCursor(QPixmap(":/center.svg")));
        m_pCurSelectedPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        break;
    case MouseMode::Zoom:
        draw->setCursor(QCursor(QPixmap(":/zoom.svg")));
        m_pCurSelectedPlot->setCustomPlotMouseTransparent(false);
        m_pCurSelectedPlot->setZoom(1);
        break;
    case MouseMode::BoxZoom:
        draw->setCursor(QCursor(QPixmap(":/box_zoom.svg")));
        m_pCurSelectedPlot->setCustomPlotMouseTransparent(false);
        m_pCurSelectedPlot->setZoom(2);
        break;
    case MouseMode::MeasureDistance:
        draw->setCursor(QCursor(QPixmap(":/measure.svg")));
        m_pCurSelectedPlot->setCustomPlotMouseTransparent(false, true);
        m_pCurSelectedPlot->setIsDrawMeasureLine(true);
        break;
    case MouseMode::CreatePlot:
        draw->setCursor(QCursor(QPixmap(":/create_plot.svg")));
        m_pCurSelectedPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        break;
    case MouseMode::MovePlot:
        draw->setCursor(QCursor(QPixmap(":/move.svg")));
        m_pCurSelectedPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
        break;
    }
}

void PlotXYDemo::onSelectedPlot(const QList<PlotItemBase*> plots)
{
    if(plots.size() > 0)
    {
        m_pCurSelectedPlot = plots.at(0);
        updateStatusBarInfo();
        // 切换新的选中图表之后需要将当前鼠标状态应用于此图表，不然新选中的图表不会应用当前鼠标状态
        onStatusBtnClicked(static_cast<int>(m_mouseMode));
    }
}

void PlotXYDemo::onTimeControls()
{
    m_timeCtrl->show();
    m_timeCtrl->activateWindow();
}

void PlotXYDemo::onCustomContextMenuRequested(const QPoint& /* point*/)
{
    QMenu menu;

    QAction* addTabPage = new QAction(QString("添加tab页面"), this);
    QAction* removeTabPage = new QAction(QString("删除tab页面"), this);
    QAction* renameTabPage = new QAction(QString("重命名tab页面"), this);

    /* 添加菜单项 */
    menu.addAction(addTabPage);
    menu.addAction(removeTabPage);
    menu.addAction(renameTabPage);

    /* 连接槽函数 */
    connect(addTabPage, SIGNAL(triggered()), this, SLOT(onNewTab()));
    connect(removeTabPage, SIGNAL(triggered()), this, SLOT(onCloseTab()));
    connect(renameTabPage, SIGNAL(triggered()), this, SLOT(onRenameTab()));

    /* 在鼠标右键处显示菜单 */
    menu.exec(QCursor::pos());
}

void PlotXYDemo::onContextMenu(const QPoint& /*point*/)
{
    QString name;
    if(m_pCurSelectedPlot)
    {
        name = m_pCurSelectedPlot->getName();
    }
    else
    {
        name = ui.tabWidget->tabText(ui.tabWidget->currentIndex());
    }

    QMenu pMenu;
    // QWidgetAction
    QWidgetAction* object_action = new QWidgetAction(this);
    QLabel* label = new QLabel(name);
    label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    label->setStyleSheet("color:rgb(0,128,0);font:Bold");
    object_action->setDefaultWidget(label);

    //主菜单
    pMenu.addAction(object_action);
    pMenu.addSeparator();
    pMenu.addMenu(ui.menu_addPlot);
    pMenu.addSeparator();
    pMenu.addAction(ui.actionUndo_Ctrl_Z);
    pMenu.addSeparator();
    pMenu.addAction(ui.actionCut_Ctrl_X);
    pMenu.addAction(ui.actionCopy_Ctrl_C);
    pMenu.addAction(ui.actionPaste_Ctrl_V);
    pMenu.addAction(ui.actionDelete);
    pMenu.addSeparator();
    pMenu.addMenu(ui.menu_autofit);
    pMenu.addAction(ui.actionOne_To_One);
    pMenu.addAction(ui.actionRound_Ranges);
    pMenu.addSeparator();
    pMenu.addAction(ui.actionPlot_Manager_Ctrl_M);
    pMenu.addAction(ui.actionWidget_Editor);
    pMenu.addAction(ui.actionAdvanced_Data_Manager);
    pMenu.addAction(ui.actionAdd_Plot_Pair_Ctrl_A);
    pMenu.addAction(ui.actionSave_Screenshot);
    pMenu.addSeparator();
    if(m_pCurSelectedPlot && m_pCurSelectedPlot->plotType() == Type_PlotScatter)
    {
        // 只有针对散点图才能导出GOG格式的数据
        pMenu.addAction(ui.actionExport_to_GOG);
        pMenu.addSeparator();
    }
    pMenu.addMenu(ui.menuOrder);
    pMenu.addSeparator();
    pMenu.addMenu(ui.menuView);

    /* 在鼠标右键处显示菜单 */
    pMenu.exec(QCursor::pos());
}

void PlotXYDemo::onNewTab()
{
    addTabPage();
}

void PlotXYDemo::onCloseTab()
{
    if(ui.tabWidget->count() <= 1)
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

void PlotXYDemo::onShowMenubar(bool checked)
{
    menuBar()->setVisible(checked);
}

void PlotXYDemo::onShowToolbar(bool checked)
{
    ui.mainToolBar->setVisible(checked);
}

void PlotXYDemo::onShowStatusbar(bool checked)
{
    ui.statusBar->setVisible(checked);
}

void PlotXYDemo::onShowTimeSlider(bool checked)
{
    ui.timeSlider->setVisible(checked);
}

void PlotXYDemo::onShowTabbar(bool checked)
{
    ui.tabWidget->tabBar()->setVisible(checked);
}

void PlotXYDemo::onShowRTIControls() {}

void PlotXYDemo::onShowAllBars() {}

void PlotXYDemo::onShowPlotTime() {}

void PlotXYDemo::onShowSystemTime() {}

void PlotXYDemo::onShowLogo() {}

void PlotXYDemo::onShowFrameRate() {}

void PlotXYDemo::onShowFullScreen(bool checked)
{
    ui.actionShow_Menubar_Ctrl_Alt_M->setChecked(!checked);
    ui.actionShow_Menubar_Ctrl_Alt_M->triggered(!checked);

    ui.actionShow_Toolbar_Ctrl_Alt_C->setChecked(!checked);
    ui.actionShow_Toolbar_Ctrl_Alt_C->triggered(!checked);
}

void PlotXYDemo::onSavePreferences() {}

void PlotXYDemo::onSavePreferencesAs() {}

void PlotXYDemo::onLoadPreferences() {}

void PlotXYDemo::onUserFunctions() {}

void PlotXYDemo::onFunctionConsole() {}

void PlotXYDemo::onHotKeys() {}

void PlotXYDemo::onOptions() {}

void PlotXYDemo::onRenameTab()
{
    RenameTabDialog dialog;
    if(dialog.exec() == QDialog::Accepted)
    {
        int currTabIndex = ui.tabWidget->currentIndex();
        QString oldName = ui.tabWidget->tabText(currTabIndex);
        QString newName = dialog.getNewTabName();
        ui.tabWidget->setTabText(currTabIndex, newName);
        PlotManagerData::getInstance()->changeTabName(oldName, newName);
    }
}

void PlotXYDemo::onSendTabRect()
{
    m_plotManager->setTabWidgetRect(ui.tabWidget->rect());
}

void PlotXYDemo::onAddPlot()
{
    if(sender() == ui.actionAddBarPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotBar;
    }
    else if(sender() == ui.actionAddAttitudePlot)
    {
        m_lastSelectedType = PlotType::Type_PlotAttitude;
    }
    else if(sender() == ui.actionAddTextPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotText;
    }
    else if(sender() == ui.actionAddPolarPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotPolar;
    }
    else if(sender() == ui.actionAddLightPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotLight;
    }
    else if(sender() == ui.actionAddTrackPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotTrack;
    }
    else if(sender() == ui.actionAddAScopePlot)
    {
        m_lastSelectedType = PlotType::Type_PlotAScope;
    }
    else if(sender() == ui.actionAddRTIPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotRTI;
    }
    else if(sender() == ui.actionAddDopplerPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotDoppler;
    }
    else if(sender() == ui.actionAddScatterPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotScatter;
    }
    else if(sender() == ui.actionAddDialPlot)
    {
        m_lastSelectedType = PlotType::Type_PlotDial;
    }
    else
    {
        return;
    }
    addPlotWidget(m_lastSelectedType);
}

void PlotXYDemo::onAutofit_Full()
{
    if(m_pCurSelectedPlot)
    {
        m_pCurSelectedPlot->rescaleAxis();
    }
}

void PlotXYDemo::onAutofit_X()
{
    if(m_pCurSelectedPlot)
    {
        m_pCurSelectedPlot->rescaleXAxis();
    }
}

void PlotXYDemo::onAutofit_Y()
{
    if(m_pCurSelectedPlot)
    {
        m_pCurSelectedPlot->rescaleYAxis();
    }
}

void PlotXYDemo::onOneToOne() {}

void PlotXYDemo::onRoundRanges() {}

void PlotXYDemo::onLockDataDisplay(bool checked)
{
    if(auto widget = getCurDrawWidget())
    {
        widget->setIsLockingEdit(checked);
        widget->setEnabled(!checked);
    }
    //避免两个操作状态不同步， TODO 后续将两个操作合并成一个action
    ui.actionLock_Data_Display->setChecked(checked);
    m_statusBar_EditLock->setChecked(checked);
}

void PlotXYDemo::onLockStackingOrder(bool checked)
{
    if(auto widget = getCurDrawWidget())
    {
        widget->setIsLockingStack(checked);
    }
    ui.menuOrder->setEnabled(!checked);
    ui.actionLock_Stacking_Order->setChecked(checked);
    m_statusBar_StackLock->setChecked(checked);
}

void PlotXYDemo::onInvertColors() {}

void PlotXYDemo::onBringToTop()
{
    auto drawWidget = getCurDrawWidget();
    if(drawWidget)
    {
        drawWidget->bringToTop();
    }
}

void PlotXYDemo::onSendToBottom()
{
    auto drawWidget = getCurDrawWidget();
    if(drawWidget)
    {
        drawWidget->sendToBottom();
    }
}

void PlotXYDemo::onBringForward() {}

void PlotXYDemo::onSendBackward() {}

void PlotXYDemo::onHorizonAlign()
{
    auto drawWidget = getCurDrawWidget();
    if(drawWidget)
    {
        drawWidget->horizonAlign();
    }
}

void PlotXYDemo::onVerticalAlign()
{
    auto drawWidget = getCurDrawWidget();
    if(drawWidget)
    {
        drawWidget->verticalAlign();
    }
}

void PlotXYDemo::onSetSliderRange(int min, int max, int singleStep)
{
    ui.timeSlider->setPageStep(singleStep);
    ui.timeSlider->setMinimum(min);
    ui.timeSlider->setMaximum(max);
}

void PlotXYDemo::onSliderValueChanged(int value)
{
    m_seconds = (double)value / m_timeCtrl->getMultiplizer();
    int refYear = m_timeCtrl->getRefYear();
    // show data time
    QString dataTime = OrdinalTimeFormatter::toString(m_seconds, refYear);
    m_statusBar_dataTime->setText(dataTime);

    /*
     * 通知图表刷新数据，在线模式，如果数据接收频率很快，那么会频繁刷新界面，导致界面卡顿
     * 采取限制措施，每秒限制刷新次数，然后给用户设置入口，可以进行设置。
     * 离线模式不必限制，可以持续刷新
    */
    if(ui.actionReal_Time->isChecked())
    {
        int64_t curMSecs = QDateTime::currentMSecsSinceEpoch();
        // 单位ms，目前设置刷新间隔为100ms
        if(curMSecs - m_lastUpdateTime < 500)
        {
            return;
        }
        else
        {
            m_lastUpdateTime = curMSecs;
            emit currentSecsChanged(m_seconds);
        }
    }
    else
    {
        emit currentSecsChanged(m_seconds);
    }
}

void PlotXYDemo::onRemoteSliderValueChanged(int value)
{
    ui.timeSlider->setValue(value);
}

void PlotXYDemo::onTimeOut()
{
    int curValue = ui.timeSlider->value();
    int step = m_timeCtrl->getStepFactor() * m_timeCtrl->getMultiplizer();

    if(m_bIsPlayForward)
    {
        if(curValue == ui.timeSlider->maximum())
        {
            if(m_timeCtrl->getLoopTime())
            {
                ui.timeSlider->setValue(ui.timeSlider->minimum());
            }
            else
            {
                m_timer->stop();
                ui.actionStop->setEnabled(false);
                ui.actionPlay->setChecked(false);
                ui.actionReverse->setChecked(false);
                emit sgn_enableActionStop(false);
            }
        }
        else
            ui.timeSlider->setValue(curValue + step);
    }
    else
    {
        if(curValue == ui.timeSlider->minimum())
        {
            if(m_timeCtrl->getLoopTime())
            {
                ui.timeSlider->setValue(ui.timeSlider->maximum());
            }
            else
            {
                m_timer->stop();
                ui.actionStop->setEnabled(false);
                ui.actionPlay->setChecked(false);
                ui.actionReverse->setChecked(false);
                emit sgn_enableActionStop(false);
            }
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

void PlotXYDemo::onTabDrawWidgetCreatePlot(PlotType type, const QRect& rect)
{
    addPlotWidget(type, rect);
}

void PlotXYDemo::addTabPage(const QString& tabName)
{
    TabDrawWidget* tabWidgetItem = new TabDrawWidget();
    tabWidgetItem->setMouseMode(m_mouseMode);
    int currCount = ui.tabWidget->count();
    QString name = tabName;
    if(name.isEmpty())
    {
        // 产生一个默认名称
        name = QString("标签 ") + QString::number(currCount + 1);
    }
    ui.tabWidget->addTab(tabWidgetItem, name);
    ui.tabWidget->setCurrentIndex(currCount);

    connect(tabWidgetItem,
            &TabDrawWidget::selectedPlotChanged,
            m_plotManager,
            &PlotManager::onMouseEventDone);
    connect(tabWidgetItem, &TabDrawWidget::selectedPlotChanged, this, &PlotXYDemo::onSelectedPlot);

    connect(
        tabWidgetItem, &TabDrawWidget::createPlot, this, &PlotXYDemo::onTabDrawWidgetCreatePlot);
}

PlotItemBase* PlotXYDemo::addPlotWidget(PlotType type, const QRect& geo, const QString& plotName)
{
    PlotItemBase* plotItem = nullptr;
    if(type == PlotType::Type_PlotBar)
    {
        plotItem = new PlotBar(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotAttitude)
    {
        plotItem = new PlotAttitude(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotText)
    {
        plotItem = new PlotText(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotPolar)
    {
        plotItem = new PlotPolar(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotLight)
    {
        plotItem = new PlotLight(ui.tabWidget->currentWidget());
        auto plotLight = static_cast<PlotLight*>(plotItem);
        connect(m_addPlotPair,
                &AddPlotPair::lightConstraintUpdate,
                plotLight,
                &PlotLight::onLightConstraintUpdate);
    }
    else if(type == PlotType::Type_PlotTrack)
    {
        plotItem = new PlotTrack(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotAScope)
    {
        plotItem = new PlotAScope(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        plotItem = new PlotRTI(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotDoppler)
    {
        plotItem = new PlotDoppler(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotScatter)
    {
        plotItem = new PlotScatter(ui.tabWidget->currentWidget());
    }
    else if(type == PlotType::Type_PlotDial)
    {
        plotItem = new PlotDial(ui.tabWidget->currentWidget());
    }
    else
    {
        return nullptr;
    }
    int currTabIndex = ui.tabWidget->currentIndex();
    QString currTabText = ui.tabWidget->tabText(currTabIndex);
    plotItem->setTabName(currTabText);
    // 时间轴刷新处理
    connect(this, &PlotXYDemo::currentSecsChanged, plotItem, &PlotItemBase::onGetCurrentSeconds);
    // 数据对数量修改后刷新三个界面
    connect(
        plotItem, &PlotItemBase::dataPairsChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
    connect(plotItem, &PlotItemBase::dataPairsChanged, m_plotManager, &PlotManager::onSelectedPlot);
    connect(plotItem,
            &PlotItemBase::dataPairsChanged,
            m_AdvancedDataManager,
            &AdvancedDataManager::onUpdatePlotPair);
    // 链接轴，一个图形刷新了x或y的范围，同步刷新其他绑定的图形
    connect(plotItem,
            &PlotItemBase::coordRangeChanged,
            PlotManagerData::getInstance(),
            &PlotManagerData::onPlotCoordRangeChanged,
            Qt::UniqueConnection);

    plotItem->show();
    plotItem->setCustomPlotMouseTransparent(true);
    if(geo.isValid())
    {
        plotItem->setGeometry(geo);
    }
    if(!plotName.isEmpty())
    {
        plotItem->setName(plotName);
    }

    // 默认选中添加的图表
    TabDrawWidget::setCurSelectedPlots(QList<PlotItemBase*>() << plotItem);
    m_pCurSelectedPlot = plotItem;
    // 根据模式更新图表的鼠标穿透属性
    onStatusBtnClicked(static_cast<int>(m_mouseMode));
    updateStatusBarInfo();

    PlotManagerData::getInstance()->addPlotByTab(currTabText, plotItem);
    return plotItem;
}

TabDrawWidget* PlotXYDemo::getCurDrawWidget()
{
    return static_cast<TabDrawWidget*>(ui.tabWidget->currentWidget());
}

void PlotXYDemo::savePXYData(const QString& pxyFileName, bool isSaveData)
{
    QJsonObject allObject;
    // 通用信息
    allObject.insert("Date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    // 不区分离线还是在线，保存时统一先在一个临时文件中存储数据
    QString dataFileName;
    // 临时文件夹会自动析构时会自动删除整个文件夹，不需要手动释放里面的文件
    QTemporaryDir dir;
    if(dir.isValid())
    {
        dataFileName = dir.path() + "/tmp.asi";
        if(isSaveData)
        {
            if(!DataManagerInstance->saveDataToASI(dataFileName))
            {
                qDebug() << "保存数据失败";
                dataFileName = "";
            }
        }
    }
    else
    {
        qDebug() << "创建临时文件夹失败";
    }

    // 图表空间信息,多个tab，每个tab包含多个plot
    int32_t tabSize = ui.tabWidget->count();

    // Object会对内容排序，为了保准按照顺序保存和加载，采用Array存储tab和plot
    QJsonArray allTabJsonArray;
    for(int i = 0; i < tabSize; ++i)
    {
        QJsonObject tabObjct;
        QString tabName = ui.tabWidget->tabText(i);
        TabDrawWidget* draw = static_cast<TabDrawWidget*>(ui.tabWidget->widget(i));
        auto plotList = draw->findAllPlots();

        QJsonArray plotArray;
        for(auto plot : plotList)
        {
            QJsonObject plotObject;
            savePlotInfoToJson(plot, plotObject);
            plotArray.append(plotObject);
        }
        tabObjct.insert("TabName", tabName);
        tabObjct.insert("Plots", plotArray);
        tabObjct.insert("BackgroundColor", "Test");
        allTabJsonArray.append(tabObjct);
    }
    allObject.insert("Tabs", allTabJsonArray);
    QJsonObject plotManagerObj;
    savePlotManagerData(plotManagerObj);
    allObject.insert("PlotManagerData", plotManagerObj);

    QJsonDocument jsonDoc;
    jsonDoc.setObject(allObject);

    QByteArray asiData;
    if(isSaveData && !dataFileName.isEmpty())
    {
        QFile asiFile(dataFileName);
        asiFile.open(QFile::ReadOnly);
        asiData = asiFile.readAll();
        asiFile.close();
    }
    writeHDF5(pxyFileName, jsonDoc.toJson(), asiData);
}

void PlotXYDemo::loadPXYData(const QString& pxyFileName)
{
    QByteArray asiData;
    QFile inFile(pxyFileName);
    bool isJsonFormat = false;
    QByteArray pxyData;
    if(inFile.open(QIODevice::ReadOnly))
    {
        pxyData = inFile.readAll();
        inFile.close();
    }
    else
    {
        return;
    }

    // 兼容旧版本的json格式，先尝试用json格式解析，如果失败就表示需要使用新的格式解析
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(pxyData, &error);
    if(error.error != QJsonParseError::NoError)
    {
        isJsonFormat = false;
        readHDF5(pxyFileName, pxyData, asiData);
        doc = QJsonDocument::fromJson(pxyData, &error);
    }
    else
    {
        isJsonFormat = true;
    }
    QJsonObject rootObj = doc.object();
    if(isJsonFormat)
    {
        QString dataFileName = rootObj.value("DataPath").toString();
        if(!dataFileName.isEmpty())
        {
            DataManagerInstance->loadFileData(dataFileName);
        }
    }
    else
    {
        // 兼容原来的接口，将数据先写进一个临时文件，然后加载之后删除临时文件
        if(!asiData.isEmpty())
        {
            QTemporaryDir dir;
            if(dir.isValid())
            {
                QString tmpPath = dir.path() + "/tmp.asi";
                qDebug() << tmpPath;
                QFile file(tmpPath);
                if(file.open(QFile::WriteOnly))
                {
                    file.write(asiData);
                    file.close();
                    DataManagerInstance->loadFileData(tmpPath);
                    DataManagerInstance->setDataFileName("");
                }
                file.remove();
            }
        }
    }
    QJsonArray allTabJsonArray = rootObj.value("Tabs").toArray();
    int32_t tabSize = allTabJsonArray.size();
    PlotManagerData::getInstance()->blockSignals(true);
    // 先清理创建好的tab页
    clearAllTab();
    for(int i = 0; i < tabSize; ++i)
    {
        auto tabObject = allTabJsonArray.at(i).toObject();
        QString tabName = tabObject.value("TabName").toString();
        // 创建Tab页
        addTabPage(tabName);
        // 创建图表
        QJsonArray plotArray = tabObject.value("Plots").toArray();
        int32_t plotSize = plotArray.size();
        for(int j = 0; j < plotSize; ++j)
        {
            auto plotObject = plotArray.at(j).toObject();
            auto plot = loadPlotJson(plotObject);
            QJsonArray dataPairArray = plotObject.value("DataPairs").toArray();
            auto dataPairSize = dataPairArray.size();
            for(int m = 0; m < dataPairSize; ++m)
            {
                QJsonObject dataPairObject = dataPairArray.at(m).toObject();
                loadDataPairJson(dataPairObject, plot);
            }
            // 全部数据对加载完之后再统一刷新图表
            if(dataPairSize > 0)
            {
                plot->updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
            }
        }
    }
    QJsonObject plotManagerObj = rootObj.value("PlotManagerData").toObject();
    loadPlotManagerData(plotManagerObj);

    PlotManagerData::getInstance()->blockSignals(false);
    // 屏蔽掉PlotManagerData中的信号，等全部加载完了之后，在触发信号，统一刷新一次三个界面
    emit PlotManagerData::getInstance()->plotDataChanged();
}

void PlotXYDemo::writeHDF5(const QString& outputFileName,
                           const QByteArray& pxyData,
                           const QByteArray& asiData)
{

    H5::H5File file(outputFileName.toLocal8Bit().data(), H5F_ACC_TRUNC);
    hsize_t pxyDim[1] = {static_cast<uint64_t>(pxyData.size())};
    H5::DataSpace pxyDataspace(1, pxyDim);
    H5::DataSet pxyDataset =
        file.createDataSet("PXY_DATASET", H5::PredType::NATIVE_CHAR, pxyDataspace);
    pxyDataset.write(pxyData.data(), H5::PredType::NATIVE_CHAR);

    hsize_t asiDim[1] = {static_cast<uint64_t>(asiData.size())};
    H5::DataSpace asiDataspace(1, asiDim);
    H5::DataSet asiDataset =
        file.createDataSet("ASI_DATASET", H5::PredType::NATIVE_CHAR, asiDataspace);
    asiDataset.write(asiData.data(), H5::PredType::NATIVE_CHAR);

    file.close();
}
void PlotXYDemo::readHDF5(const QString& inputFileName, QByteArray& pxyData, QByteArray& asiData)
{
    try
    {
        H5::H5File file(inputFileName.toLocal8Bit().data(), H5F_ACC_RDONLY);
        H5::DataSet pxyDataset = file.openDataSet("PXY_DATASET");
        DataSpace pxyDataSpace = pxyDataset.getSpace();
        hsize_t pxyDim = 0;
        pxyDataSpace.getSimpleExtentDims(&pxyDim);
        char* pxyBuffer = new char[pxyDim];
        memset(pxyBuffer, 0, pxyDim);
        pxyDataset.read(pxyBuffer, H5::PredType::NATIVE_CHAR);
        pxyData = QByteArray(pxyBuffer, pxyDim);

        H5::DataSet asiDataset = file.openDataSet("ASI_DATASET");
        DataSpace asiDataSpace = asiDataset.getSpace();
        hsize_t asiDim = 0;
        asiDataSpace.getSimpleExtentDims(&asiDim);
        char* asiBuffer = new char[asiDim];
        memset(asiBuffer, 0, asiDim);
        asiDataset.read(asiBuffer, H5::PredType::NATIVE_CHAR);
        asiData = QByteArray(asiBuffer, asiDim);
        file.close();
    }
    catch(const H5::FileIException& e)
    {
        QMessageBox::warning(nullptr, "提示", "无法加载中文路径文档");
    }
}

void PlotXYDemo::savePlotInfoToJson(PlotItemBase* plot, QJsonObject& plotObject)
{
    QString plotName = plot->getName();
    auto type = plot->plotType();
    plotObject.insert("IsDraw", plot->getBVisible());
    plotObject.insert("PlotName", plotName);
    plotObject.insert("Title", plot->getTitle());
    plotObject.insert("TitleVisible", plot->getTitleVisible());
    plotObject.insert("TitleColor", color_transfer::QColorToRGBAStr(plot->getTitleColor()));
    plotObject.insert("TitleFillColor", color_transfer::QColorToRGBAStr(plot->getTitleFillColor()));
    plotObject.insert("TitleFontFamily", plot->getTitleFont().family());
    plotObject.insert("TitleFontSize", plot->getTitleFontSize());
    plotObject.insert("X", plot->x());
    plotObject.insert("Y", plot->y());
    plotObject.insert("Width", plot->width());
    plotObject.insert("Height", plot->height());
    plotObject.insert("PlotType", type);
    plotObject.insert("PlotOuterFillColor",
                      color_transfer::QColorToRGBAStr(plot->getOuterFillColor()));
    plotObject.insert("PlotOutLineColor", color_transfer::QColorToRGBAStr(plot->getOutlineColor()));

    plotObject.insert("LabelXVisible", plot->getxAxisLabelVisible());
    plotObject.insert("LabelX", plot->getxAxisLabel());
    plotObject.insert("LabelXUnit", plot->getUnitsX());
    plotObject.insert("LabelXColor", color_transfer::QColorToRGBAStr(plot->getxAxisLabelColor()));
    plotObject.insert("LabelXFontFamily", plot->getxAxisLabelFont().family());
    plotObject.insert("LabelXFontSize", plot->getxAxisLabelFontSize());

    plotObject.insert("LabelYVisible", plot->getyAxisLabelVisible());
    plotObject.insert("LabelY", plot->getyAxisLabel());
    plotObject.insert("LabelYUnit", plot->getUnitsY());
    plotObject.insert("LabelYColor", color_transfer::QColorToRGBAStr(plot->getyAxisLabelColor()));
    plotObject.insert("LabelYFontFamily", plot->getyAxisLabelFont().family());
    plotObject.insert("LabelYFontSize", plot->getyAxisLabelFontSize());

    double lower, upper;
    plot->getCoordRangeX(lower, upper);
    plotObject.insert("PlotOriginX", lower);
    plotObject.insert("PlotEndX", upper);
    plot->getCoordRangeY(lower, upper);
    plotObject.insert("PlotOriginY", lower);
    plotObject.insert("PlotEndY", upper);

    plotObject.insert("HorzGrids", plot->getHorzGrids());
    plotObject.insert("VertGrids", plot->getVertGrids());
    plotObject.insert("AxisWidth", plot->getAxisWidth());
    plotObject.insert("GridWidth", plot->getGridWidth());
    plotObject.insert("AxisColor", color_transfer::QColorToRGBAStr(plot->getAxisColor()));
    plotObject.insert("GridColor", color_transfer::QColorToRGBAStr(plot->getGridColor()));
    plotObject.insert("GridFillColor", color_transfer::QColorToRGBAStr(plot->getGridFillColor()));
    plotObject.insert("ShowUnitX", plot->unitsShowX());
    plotObject.insert("ShowUnitY", plot->unitsShowY());
    plotObject.insert("XRate", plot->getXRate());
    plotObject.insert("YRate", plot->getYRate());

    // 图表特殊设置
    if(type == PlotType::Type_PlotBar)
    {
        plotObject.insert("IsHorizonBar", plot->getIsHorizonBar());
    }
    else if(type == PlotType::Type_PlotDial)
    {
        PlotDial* dialPlot = dynamic_cast<PlotDial*>(plot);
        if(dialPlot)
        {
            plotObject.insert("DialTickRadius", dialPlot->getTickRadiusRate());
            plotObject.insert("DialColorRate", dialPlot->getColorRate());
            plotObject.insert("DialTextRate", dialPlot->getTextRate());
            plotObject.insert("DialRate", dialPlot->getDialRate());
            plotObject.insert("DialCapRate", dialPlot->getDialCapRate());
            plotObject.insert("DialStartAngle", dialPlot->getStartAngle());
            plotObject.insert("DialEndAngle", dialPlot->getEndAngle());
            plotObject.insert("DialCapColor",
                              color_transfer::QColorToRGBAStr(dialPlot->getCapColor()));
            plotObject.insert("DialDrawFirstTick", dialPlot->getDrawFirstTick());
            plotObject.insert("DialDrawLastTick", dialPlot->getDrawLastTick());
            plotObject.insert("DialDrawFirstTextLabel", dialPlot->getDrawFirstTextLabel());
            plotObject.insert("DialDrawLastTextLabel", dialPlot->getDrawLastTextLabel());
            plotObject.insert("DialStyle", dialPlot->getDialStyle());

            QJsonArray colorInfo;
            QList<DialColorInfo> list = dialPlot->getColorInfoList();
            for(auto item : list)
            {
                QJsonObject object;
                object.insert("DialColorStart", item.start);
                object.insert("DialColorEnd", item.end);
                object.insert("DialColor", color_transfer::QColorToRGBAStr(item.clr));
                object.insert("DialColorOutline", color_transfer::QColorToRGBAStr(item.outline));
                object.insert("DialColorWidth", item.width);
                colorInfo.append(object);
            }
            plotObject.insert("DialColorInfo", colorInfo);
        }
    }
    else if(type == PlotType::Type_PlotAttitude)
    {
        PlotAttitude* attPlot = dynamic_cast<PlotAttitude*>(plot);
        if(attPlot)
        {
            plotObject.insert("AttitudeTickRadius", attPlot->getTickRadiusPercentage());
            plotObject.insert("AttitudeTextPercentage", attPlot->getTextPercentage());
            plotObject.insert("AttitudeDialPercentage", attPlot->getDialPercentage());
            plotObject.insert("AttitudeRollColor",
                              color_transfer::QColorToRGBAStr(attPlot->getRollColor()));
            plotObject.insert("AttitudePitchColor",
                              color_transfer::QColorToRGBAStr(attPlot->getPitchColor()));
        }
    }
    else if(type == PlotType::Type_PlotScatter)
    {
        PlotScatter* scatter = dynamic_cast<PlotScatter*>(plot);
        if(scatter)
        {
            // marks
            auto markers = scatter->getPlotMarkers();
            QJsonArray array;
            for(const auto& mark : markers)
            {
                QJsonObject object;
                object.insert("MarkerUUID", mark.uuid);
                object.insert("MarkerX", mark.x);
                object.insert("MarkerXUnit", mark.xUnit);
                object.insert("MarkerY", mark.y);
                object.insert("MarkerYUnit", mark.yUnit);
                object.insert("MarkerTime", mark.time);
                object.insert("MarkerIconType", mark.iconType);
                object.insert("MarkerText", mark.text);
                object.insert("MarkerColor", color_transfer::QColorToRGBAStr(mark.color));
                object.insert("MarkerFontFamily", mark.fontFamily);
                object.insert("MarkerFontSize", mark.fontSize);

                array.append(object);
            }
            if(!array.isEmpty())
            {
                plotObject.insert("ScatterMarkers", array);
            }
            // GOG
            auto gogList = scatter->getGOGFileList();
            QJsonArray gogArray;
            auto settingMap = scatter->getGogCustomSettings();
            for(const auto& gogFile : gogList)
            {
                GOGCustomSetting setting = settingMap.value(gogFile);

                QJsonObject object;

                object.insert("GOGFilePath", gogFile);
                object.insert("GOGIsDraw", setting.isDraw);
                object.insert("GOGColor", color_transfer::QColorToRGBAStr(setting.fillColor));
                object.insert("GOGLineWidth", setting.lineWidth);

                gogArray.append(object);
            }
            if(!gogArray.isEmpty())
            {
                plotObject.insert("ScatterGOGs", gogArray);
            }
            // Event
            auto eventList = scatter->getEventList();
            QJsonArray eventArray;
            for(const auto& event : eventList)
            {

                QJsonObject object;

                object.insert("ScatterEventFontFamily", event.m_eventFontFamily);
                object.insert("ScatterEventFontSize", event.m_eventFontSize);
                object.insert("ScatterEventColor",
                              color_transfer::QColorToRGBAStr(event.m_eventColor));
                object.insert("ScatterEventStyle", event.m_eventStyle);
                object.insert("ScatterEventIncludeTag", event.m_isIncludeTag);
                object.insert("ScatterEventType", event.m_type);
                object.insert("ScatterEventEID", event.m_entityID);
                object.insert("ScatterEventEName", event.m_entityName);

                eventArray.append(object);
            }
            if(!eventArray.isEmpty())
            {
                plotObject.insert("ScatterEvents", eventArray);
            }
            // connections连线
            auto conHash = scatter->getConHash();
            QJsonArray conArray;
            for(const auto& conSetting : conHash)
            {
                QJsonObject object;
                object.insert("ScatterConUUID", conSetting.uuid);
                object.insert("ScatterConColor", color_transfer::QColorToRGBAStr(conSetting.color));
                object.insert("ScatterConWidth", conSetting.width);
                object.insert("ScatterConStipple", conSetting.stipple);
                object.insert("ScatterConSpeed", conSetting.speed);
                object.insert("ScatterStartUUID", conSetting.startDataPairUuid);
                object.insert("ScatterEndUUID", conSetting.endDataPairUuid);
                object.insert("ScatterConXEntityAttr", conSetting.endXEntityAttr);
                object.insert("ScatterConYEntityAttr", conSetting.endYEntityAttr);

                conArray.append(object);
            }
            if(!conArray.isEmpty())
            {
                plotObject.insert("ScatterCons", conArray);
            }
            // scroll
            plotObject.insert("XScrollOn", scatter->getXScrollOn());
            plotObject.insert("XFollow", scatter->getXFollow());
            plotObject.insert("XLead", scatter->getXLead());
            plotObject.insert("XTimeFormat", scatter->getXTimeTickFormat());
            plotObject.insert("YTimeFormat", scatter->getYTimeTickFormat());

            auto xScrollHash = scatter->getXScrollHash();
            auto xUuids = xScrollHash.keys();
            QJsonObject xDataPairEnableObj;
            for(const auto& xUuid : xUuids)
            {
                xDataPairEnableObj.insert(xUuid, xScrollHash.value(xUuid, false));
            }
            plotObject.insert("XDataPairEnableMap", xDataPairEnableObj);

            plotObject.insert("YScrollOn", scatter->getYScrollOn());
            plotObject.insert("YFollow", scatter->getYFollow());
            plotObject.insert("YLead", scatter->getYLead());
            auto yScrollHash = scatter->getYScrollHash();
            auto yUuids = yScrollHash.keys();
            QJsonObject yDataPairEnableObj;
            for(const auto& yUuid : yUuids)
            {
                yDataPairEnableObj.insert(yUuid, yScrollHash.value(yUuid, false));
            }
            plotObject.insert("YDataPairEnableMap", yDataPairEnableObj);
            // limits
            auto bgMap = scatter->getBkgLimitSegMap();
            QJsonArray bgArray;
            for(const auto& seg : bgMap)
            {
                QJsonObject obj;
                obj.insert("BGLimitName", seg.m_limitName);
                obj.insert("BGLimitValue", seg.m_limitValue);
                obj.insert("BGLimitLineColor", color_transfer::QColorToRGBAStr(seg.m_lineColor));
                obj.insert("BGLimitLineWidth", seg.m_lineWidth);
                obj.insert("BGLimitFillAColor",
                           color_transfer::QColorToRGBAStr(seg.m_fillAboveColor));
                obj.insert("BGLimitFillBColor",
                           color_transfer::QColorToRGBAStr(seg.m_fillBelowColor));
                bgArray.append(obj);
            }
            if(!bgArray.isEmpty())
            {
                plotObject.insert("BGLimits", bgArray);
            }
        }
    }
    else if(type == PlotType::Type_PlotText)
    {
        PlotText* textPlot = dynamic_cast<PlotText*>(plot);
        if(textPlot)
        {
            plotObject.insert("TextRowGridsVisible", textPlot->isRowGridVisible());
            plotObject.insert("TextColGridsVisible", textPlot->isColGridVisible());
            plotObject.insert("TextGridWidth", textPlot->getGridWidth());
            plotObject.insert("TextGridColor",
                              color_transfer::QColorToRGBAStr(textPlot->getGridColor()));
            plotObject.insert("TextOutlineWidth", textPlot->getOutlineWidth());
            plotObject.insert("TextOutlineColor",
                              color_transfer::QColorToRGBAStr(textPlot->getOutlineColor()));
            plotObject.insert("TextOuterFillColor",
                              color_transfer::QColorToRGBAStr(textPlot->getOuterFillColor()));
            plotObject.insert("TextLeftPad", textPlot->textLeftOffset());
            plotObject.insert("TextRightPad", textPlot->textRightOffset());
            plotObject.insert("TextIsFillByRow", textPlot->isFillByRow());
            plotObject.insert("TextNumRows", textPlot->rowsNum());
            plotObject.insert("TextNumCols", textPlot->colsNum());
        }
    }
    else if(type == PlotType::Type_PlotLight)
    {
        PlotLight* lightPlot = dynamic_cast<PlotLight*>(plot);
        if(lightPlot)
        {
            plotObject.insert("LightRowGridsVisible", lightPlot->getIsRowGridVisible());
            plotObject.insert("LightColGridsVisible", lightPlot->getIsColGridVisible());
            plotObject.insert("LightGridWidth", lightPlot->getGridWidth());
            plotObject.insert("LightGridColor",
                              color_transfer::QColorToRGBAStr(lightPlot->getGridColor()));
            plotObject.insert("LightOutlineWidth", lightPlot->getOutlineWidth());
            plotObject.insert("LightOutlineColor",
                              color_transfer::QColorToRGBAStr(lightPlot->getOutlineColor()));
            plotObject.insert("LightOuterFillColor",
                              color_transfer::QColorToRGBAStr(lightPlot->getOuterFillColor()));
            plotObject.insert("LightLeftPad", lightPlot->getTextLeftOffset());
            plotObject.insert("LightRightPad", lightPlot->getTextRightOffset());
            plotObject.insert("LightIsFillByRow", lightPlot->getIsFillByRow());
            plotObject.insert("LightNumRows", lightPlot->getRowsNum());
            plotObject.insert("LightNumCols", lightPlot->getColsNum());
            plotObject.insert("LightType", lightPlot->getLightType());
            plotObject.insert("LightWidth", lightPlot->getLightWidth());
            plotObject.insert("LightHeight", lightPlot->getLightHeight());
            plotObject.insert("LightTextYPos", lightPlot->getLightTextYPos());
            plotObject.insert("LightIndOutlineWidth", lightPlot->getLightOutlineWidth());
            plotObject.insert("LightIndOutlineColor",
                              color_transfer::QColorToRGBAStr(lightPlot->getLightOutlineColor()));
        }
    }
    else if(type == PlotType::Type_PlotAScope)
    {
        PlotAScope* ascopePlot = dynamic_cast<PlotAScope*>(plot);
        if(ascopePlot)
        {
            plotObject.insert("AScopeIsDrawGate", ascopePlot->isDrawGate());
            plotObject.insert("AScopeGateHeight", ascopePlot->gateHeight());
            plotObject.insert("AScopeGateColor",
                              color_transfer::QColorToRGBAStr(ascopePlot->gateColor()));
            plotObject.insert("AScopeIsAutofitX", ascopePlot->isAutofitX());
            plotObject.insert("AScopeIsAutofitY", ascopePlot->isAutofitY());
        }
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        PlotRTI* rtiPlot = dynamic_cast<PlotRTI*>(plot);
        if(rtiPlot)
        {
            plotObject.insert("RTITimeSpan", rtiPlot->getTimeSpan());
            plotObject.insert("RTILabelDensity", rtiPlot->getGridDensity());

            QJsonArray colorInfo;
            QMap<double, QColor> colorMap = rtiPlot->getColorRangeMap();
            QList<double> valueList = colorMap.keys();
            for(auto value : valueList)
            {
                QJsonObject object;
                object.insert("RTIColorRangeValue", value);
                object.insert("RTIColorRangeColor",
                              color_transfer::QColorToRGBAStr(colorMap.value(value)));
                colorInfo.append(object);
            }
            plotObject.insert("RTIColorRangeInfo", colorInfo);
        }
    }
    else if(type == PlotType::Type_PlotDoppler)
    {
        PlotDoppler* dopplerPlot = dynamic_cast<PlotDoppler*>(plot);
        if(dopplerPlot)
        {
            plotObject.insert("DopplerShowToolTip", dopplerPlot->getIsShowToolTip());

            QJsonArray colorInfo;
            QMap<double, QColor> colorMap = dopplerPlot->getColorRangeMap();
            QList<double> valueList = colorMap.keys();
            for(auto value : valueList)
            {
                QJsonObject object;
                object.insert("DopplerColorRangeValue", value);
                object.insert("DopplerColorRangeColor",
                              color_transfer::QColorToRGBAStr(colorMap.value(value)));
                colorInfo.append(object);
            }
            plotObject.insert("DopplerColorRangeInfo", colorInfo);
        }
    }

    // 图表存在多个数据对
    QJsonArray dataPairArray;
    auto dataPairs = plot->getDataPairs();
    for(DataPair* dataPair : dataPairs)
    {
        QJsonObject dataPairObject;
        saveDataPairToJson(dataPair, dataPairObject, plot->plotType());
        dataPairArray.append(dataPairObject);
    }
    plotObject.insert("DataPairs", dataPairArray);
}

PlotItemBase* PlotXYDemo::loadPlotJson(const QJsonObject& plotObject)
{
    int32_t x = plotObject.value("X").toInt();
    int32_t y = plotObject.value("Y").toInt();
    int32_t width = plotObject.value("Width").toInt();
    int32_t height = plotObject.value("Height").toInt();
    PlotType type = static_cast<PlotType>(plotObject.value("PlotType").toInt());
    QString plotName = plotObject.value("PlotName").toString();

    auto plot = addPlotWidget(type, QRect(x, y, width, height), plotName);
    plot->blockSignals(true);
    plot->setBVisible(plotObject.value("IsDraw").toBool());
    plot->setOuterFillColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("PlotOuterFillColor").toString()));
    plot->setOutlineColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("PlotOutLineColor").toString()));
    plot->setTitle(plotObject.value("Title").toString());
    plot->setTitleVisible(plotObject.value("TitleVisible").toBool());
    plot->setTitleColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("TitleColor").toString()));
    plot->setTitleFillColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("TitleFillColor").toString()));
    QFont font;
    font.setFamily(plotObject.value("TitleFontFamily").toString());
    font.setPixelSize(plotObject.value("TitleFontSize").toInt());
    plot->setTitleFont(font);
    plot->setTitleFontSize(plotObject.value("TitleFontSize").toInt());
    // label
    // show属性需要放在前面设置，后续label会根据show进行组合
    plot->setUnitsShowX(plotObject.value("ShowUnitX").toBool());
    plot->setUnitsShowY(plotObject.value("ShowUnitY").toBool());
    plot->setXRate(plotObject.value("XRate").toDouble());
    plot->setYRate(plotObject.value("YRate").toDouble());

    plot->setUnitsX(plotObject.value("LabelXUnit").toString());
    plot->setxAxisLabel(plotObject.value("LabelX").toString());
    plot->setxAxisLabelVisible(plotObject.value("LabelXVisible").toBool());
    plot->setxAxisLabelColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("LabelXColor").toString()));
    QFont xLabelFont;
    xLabelFont.setFamily(plotObject.value("LabelXFontFamily").toString());
    xLabelFont.setPixelSize(plotObject.value("LabelXFontSize").toInt());
    plot->setxAxisLabelFont(xLabelFont);
    plot->setxAxisLabelFontSize(plotObject.value("LabelXFontSize").toInt());

    plot->setUnitsY(plotObject.value("LabelYUnit").toString());
    plot->setyAxisLabel(plotObject.value("LabelY").toString());
    plot->setyAxisLabelVisible(plotObject.value("LabelYVisible").toBool());
    plot->setyAxisLabelColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("LabelYColor").toString()));
    QFont yLabelFont;
    yLabelFont.setFamily(plotObject.value("LabelYFontFamily").toString());
    yLabelFont.setPixelSize(plotObject.value("LabelYFontSize").toInt());
    plot->setyAxisLabelFont(yLabelFont);
    plot->setyAxisLabelFontSize(plotObject.value("LabelYFontSize").toInt());

    plot->setHorzGrids(plotObject.value("HorzGrids").toInt());
    plot->setVertGrids(plotObject.value("VertGrids").toInt());
    plot->setAxisColorWidth(
        color_transfer::QColorFromRGBAStr(plotObject.value("AxisColor").toString()),
        plotObject.value("AxisWidth").toInt());
    plot->setGridColorWidth(
        color_transfer::QColorFromRGBAStr(plotObject.value("GridColor").toString()),
        plotObject.value("GridWidth").toInt());

    plot->setGridFillColor(
        color_transfer::QColorFromRGBAStr(plotObject.value("GridFillColor").toString()));

    //图表特殊设置部分
    if(type == PlotType::Type_PlotBar)
    {
        plot->setIsHorizonBar(plotObject.value("IsHorizonBar").toBool());
    }
    else if(type == PlotType::Type_PlotDial)
    {
        PlotDial* dialPlot = dynamic_cast<PlotDial*>(plot);
        if(dialPlot)
        {
            dialPlot->setTickRadiusRate(plotObject.value("DialTickRadius").toInt());
            dialPlot->setColorRate(plotObject.value("DialColorRate").toInt());
            dialPlot->setTextRate(plotObject.value("DialTextRate").toInt());
            dialPlot->setDialRate(plotObject.value("DialRate").toInt());
            dialPlot->setDialCapRate(plotObject.value("DialCapRate").toInt());
            dialPlot->setStartAngle(plotObject.value("DialStartAngle").toInt());
            dialPlot->setEndAngle(plotObject.value("DialEndAngle").toInt());
            dialPlot->setCapColor(
                color_transfer::QColorFromRGBAStr(plotObject.value("DialCapColor").toString()));
            dialPlot->setDrawFirstTick(plotObject.value("DialDrawFirstTick").toBool());
            dialPlot->setDrawLastTick(plotObject.value("DialDrawLastTick").toBool());
            dialPlot->setDrawFirstTextLabel(plotObject.value("DialDrawFirstTextLabel").toBool());
            dialPlot->setDrawLastTextLabel(plotObject.value("DialDrawLastTextLabel").toBool());
            dialPlot->setDialStyle(plotObject.value("DialStyle").toString());

            QJsonArray colorInfo = plotObject.value("DialColorInfo").toArray();
            int size = colorInfo.size();
            QList<DialColorInfo> list;
            for(int i = 0; i < size; i++)
            {
                DialColorInfo item;
                QJsonObject obj = colorInfo.at(i).toObject();
                item.start = obj.value("DialColorStart").toInt();
                item.end = obj.value("DialColorEnd").toInt();
                item.clr = color_transfer::QColorFromRGBAStr(obj.value("DialColor").toString());
                item.outline =
                    color_transfer::QColorFromRGBAStr(obj.value("DialColorOutline").toString());
                item.width = obj.value("DialColorWidth").toInt();
                list.append(item);
            }
            dialPlot->setColorInfoList(list);
        }
    }
    else if(type == PlotType::Type_PlotAttitude)
    {
        PlotAttitude* attPlot = dynamic_cast<PlotAttitude*>(plot);
        if(attPlot)
        {
            attPlot->setTickRadiusPercentage(plotObject.value("AttitudeTickRadius").toInt());
            attPlot->setTextPercentage(plotObject.value("AttitudeTextPercentage").toInt());
            attPlot->setDialPercentage(plotObject.value("AttitudeDialPercentage").toInt());
            attPlot->setRollColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("AttitudeRollColor").toString()));
            attPlot->setPitchColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("AttitudePitchColor").toString()));
        }
    }
    else if(type == PlotType::Type_PlotScatter)
    {
        PlotScatter* scatter = dynamic_cast<PlotScatter*>(plot);
        if(scatter)
        {
            auto array = plotObject.value("ScatterMarkers").toArray();
            int32_t size = array.size();
            for(int32_t i = 0; i < size; ++i)
            {
                auto markObject = array.at(i).toObject();
                PlotMarker plotMarker;
                plotMarker.uuid = markObject.value("MarkerUUID").toString();
                plotMarker.x = markObject.value("MarkerX").toDouble();
                plotMarker.xUnit = markObject.value("MarkerXUnit").toString();
                plotMarker.y = markObject.value("MarkerY").toDouble();
                plotMarker.yUnit = markObject.value("MarkerYUnit").toString();
                plotMarker.time = markObject.value("MarkerTime").toDouble();
                plotMarker.iconType = markObject.value("MarkerIconType").toString();
                plotMarker.text = markObject.value("MarkerText").toString();
                plotMarker.color =
                    color_transfer::QColorFromRGBAStr(markObject.value("MarkerColor").toString());
                plotMarker.fontFamily = markObject.value("MarkerFontFamily").toString();
                plotMarker.fontSize = markObject.value("MarkerFontSize").toInt();

                scatter->addMarker(plotMarker);
            }

            auto gogArray = plotObject.value("ScatterGOGs").toArray();
            QMap<QString, GOGCustomSetting> gogSettingMap;
            for(int32_t i = 0; i < gogArray.size(); i++)
            {
                auto gogObject = gogArray.at(i).toObject();
                GOGCustomSetting setting;
                setting.isDraw = gogObject.value("GOGIsDraw").toBool();
                setting.fillColor =
                    color_transfer::QColorFromRGBAStr(gogObject.value("GOGColor").toString());
                setting.lineWidth = gogObject.value("GOGLineWidth").toInt();
                QString gogFilePath = gogObject.value("GOGFilePath").toString();

                scatter->addGOGFile(gogFilePath);
                gogSettingMap.insert(gogFilePath, setting);
            }
            scatter->setGogCustomSettings(gogSettingMap);

            // Event
            auto eventArray = plotObject.value("ScatterEvents").toArray();

            for(int32_t i = 0; i < eventArray.size(); i++)
            {

                auto eventObject = eventArray.at(i).toObject();
                EventSettings set;

                set.m_eventFontFamily = eventObject.value("ScatterEventFontFamily").toString();
                set.m_eventFontSize = eventObject.value("ScatterEventFontSize").toInt();
                set.m_eventColor = color_transfer::QColorFromRGBAStr(
                    eventObject.value("ScatterEventColor").toString());
                set.m_eventStyle = eventObject.value("ScatterEventStyle").toString();
                set.m_isIncludeTag = eventObject.value("ScatterEventIncludeTag").toBool();
                set.m_type = eventObject.value("ScatterEventType").toString();
                set.m_entityID = eventObject.value("ScatterEventEID").toInt();
                set.m_entityName = eventObject.value("ScatterEventEName").toString();
                scatter->addEvent(set);
            }
            // 连线
            auto conArray = plotObject.value("ScatterCons").toArray();

            for(int32_t i = 0; i < conArray.size(); i++)
            {

                auto conObject = conArray.at(i).toObject();
                ConnectionSetting set;

                set.uuid = conObject.value("ScatterConUUID").toString();
                set.color = color_transfer::QColorFromRGBAStr(
                    conObject.value("ScatterConColor").toString());
                set.width = conObject.value("ScatterConWidth").toInt();
                set.stipple = conObject.value("ScatterConStipple").toString();
                set.speed = conObject.value("ScatterConSpeed").toInt();
                set.startDataPairUuid = conObject.value("ScatterStartUUID").toString();
                set.endDataPairUuid = conObject.value("ScatterEndUUID").toString();
                set.endXEntityAttr = conObject.value("ScatterConXEntityAttr").toString();
                set.endYEntityAttr = conObject.value("ScatterConYEntityAttr").toString();
                scatter->addConnection(set);
            }
            // scroll
            scatter->setXScrollOn(plotObject.value("XScrollOn").toBool());
            scatter->setXFollow(plotObject.value("XFollow").toDouble());
            scatter->setXLead(plotObject.value("XLead").toDouble());
            QHash<QString, bool> xScrollHash;
            QJsonObject xDataPairEnableObj = plotObject.value("XDataPairEnableMap").toObject();
            for(auto it = xDataPairEnableObj.begin(); it != xDataPairEnableObj.end(); ++it)
            {
                xScrollHash.insert(it.key(), it.value().toBool());
            }
            scatter->setXScrollHash(xScrollHash);

            scatter->setYScrollOn(plotObject.value("YScrollOn").toBool());
            scatter->setYFollow(plotObject.value("YFollow").toDouble());
            scatter->setYLead(plotObject.value("YLead").toDouble());
            scatter->setTimeTickerFormat(plotObject.value("XTimeFormat").toString(), true);
            scatter->setTimeTickerFormat(plotObject.value("YTimeFormat").toString(), false);
            QHash<QString, bool> yScrollHash;
            QJsonObject yDataPairEnableObj = plotObject.value("YDataPairEnableMap").toObject();
            for(auto it = yDataPairEnableObj.begin(); it != yDataPairEnableObj.end(); ++it)
            {
                yScrollHash.insert(it.key(), it.value().toBool());
            }
            scatter->setYScrollHash(yScrollHash);

            // limits
            if(plotObject.contains("BGLimits"))
            {
                QMap<double, PlotScatter::BackgroundLimitSeg> bkgLimitSegMap;
                QJsonArray bgArray = plotObject.value("BGLimits").toArray();
                for(const auto value : bgArray)
                {
                    QJsonObject bgObj = value.toObject();
                    PlotScatter::BackgroundLimitSeg seg;
                    seg.m_limitName = bgObj.value("BGLimitName").toString();
                    seg.m_limitValue = bgObj.value("BGLimitValue").toDouble();
                    seg.m_lineColor = color_transfer::QColorFromRGBAStr(
                        bgObj.value("BGLimitLineColor").toString());
                    seg.m_lineWidth = bgObj.value("BGLimitLineWidth").toInt();
                    seg.m_fillAboveColor = color_transfer::QColorFromRGBAStr(
                        bgObj.value("BGLimitFillAColor").toString());
                    seg.m_fillBelowColor = color_transfer::QColorFromRGBAStr(
                        bgObj.value("BGLimitFillBColor").toString());
                    bkgLimitSegMap.insert(seg.m_limitValue, seg);
                }
                scatter->setBkgLimitSegMap(bkgLimitSegMap);
            }
        }
    }
    else if(type == PlotType::Type_PlotText)
    {
        PlotText* textPlot = dynamic_cast<PlotText*>(plot);
        if(textPlot)
        {
            textPlot->setIsRowGridVisible(plotObject.value("TextRowGridsVisible").toBool());
            textPlot->setIsColGridVisible(plotObject.value("TextColGridsVisible").toBool());
            textPlot->setGridColorWidth(
                color_transfer::QColorFromRGBAStr(plotObject.value("TextGridColor").toString()),
                plotObject.value("TextGridWidth").toInt());
            textPlot->setOutlineWidth(plotObject.value("TextOutlineWidth").toInt());
            textPlot->setOutlineColor(
                color_transfer::QColorFromRGBAStr(plotObject.value("TextOutlineColor").toString()));
            textPlot->setOuterFillColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("TextOuterFillColor").toString()));
            textPlot->setTextLeftOffset(plotObject.value("TextLeftPad").toInt());
            textPlot->setTextRightOffset(plotObject.value("TextRightPad").toInt());
            textPlot->setIsFillByRow(plotObject.value("TextIsFillByRow").toBool());
            textPlot->setRowsNum(plotObject.value("TextNumRows").toInt());
            textPlot->setColsNum(plotObject.value("TextNumCols").toInt());
        }
    }
    else if(type == PlotType::Type_PlotLight)
    {
        PlotLight* lightPlot = dynamic_cast<PlotLight*>(plot);
        if(lightPlot)
        {
            lightPlot->setIsRowGridVisible(plotObject.value("LightRowGridsVisible").toBool());
            lightPlot->setIsColGridVisible(plotObject.value("LightColGridsVisible").toBool());
            lightPlot->setGridColorWidth(
                color_transfer::QColorFromRGBAStr(plotObject.value("LightGridColor").toString()),
                plotObject.value("LightGridWidth").toInt());
            lightPlot->setOutlineWidth(plotObject.value("LightOutlineWidth").toInt());
            lightPlot->setOutlineColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("LightOutlineColor").toString()));
            lightPlot->setOuterFillColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("LightOuterFillColor").toString()));
            lightPlot->setTextLeftOffset(plotObject.value("LightLeftPad").toInt());
            lightPlot->setTextRightOffset(plotObject.value("LightRightPad").toInt());
            lightPlot->setIsFillByRow(plotObject.value("LightIsFillByRow").toBool());
            lightPlot->setRowsNum(plotObject.value("LightNumRows").toInt());
            lightPlot->setColsNum(plotObject.value("LightNumCols").toInt());
            lightPlot->setLightType(plotObject.value("LightType").toString());
            lightPlot->setLightWidth(plotObject.value("LightWidth").toInt());
            lightPlot->setLightHeight(plotObject.value("LightHeight").toInt());
            lightPlot->setLightTextYPos(plotObject.value("LightTextYPos").toInt());
            lightPlot->setLightOutlineWidth(plotObject.value("LightIndOutlineWidth").toInt());
            lightPlot->setLightOutlineColor(color_transfer::QColorFromRGBAStr(
                plotObject.value("LightIndOutlineColor").toString()));
        }
    }
    else if(type == PlotType::Type_PlotAScope)
    {
        PlotAScope* ascopePlot = dynamic_cast<PlotAScope*>(plot);
        if(ascopePlot)
        {
            ascopePlot->setIsDrawGate(plotObject.value("AScopeIsDrawGate").toBool());
            ascopePlot->setGateHeight(plotObject.value("AScopeGateHeight").toInt());
            ascopePlot->setGateColor(
                color_transfer::QColorFromRGBAStr(plotObject.value("AScopeGateColor").toString()));
            ascopePlot->setIsAutofitX(plotObject.value("AScopeIsAutofitX").toBool());
            ascopePlot->setIsAutofitY(plotObject.value("AScopeIsAutofitY").toBool());
        }
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        PlotRTI* rtiPlot = dynamic_cast<PlotRTI*>(plot);
        if(rtiPlot)
        {
            rtiPlot->setTimeSpan(plotObject.value("RTITimeSpan").toInt());
            rtiPlot->setGridDensity(GridDensity(plotObject.value("RTILabelDensity").toInt()));

            QJsonArray colorInfo = plotObject.value("RTIColorRangeInfo").toArray();
            int size = colorInfo.size();
            QMap<double, QColor> colorMap;
            for(int i = 0; i < size; i++)
            {
                QJsonObject obj = colorInfo.at(i).toObject();
                double value = obj.value("RTIColorRangeValue").toDouble();
                QColor color =
                    color_transfer::QColorFromRGBAStr(obj.value("RTIColorRangeColor").toString());
                colorMap.insert(value, color);
            }
            rtiPlot->setColorRangeMap(colorMap);
        }
    }
    else if(type == PlotType::Type_PlotDoppler)
    {
        PlotDoppler* dopplerPlot = dynamic_cast<PlotDoppler*>(plot);
        if(dopplerPlot)
        {
            dopplerPlot->setIsShowToolTip(plotObject.value("DopplerShowToolTip").toBool());

            QJsonArray colorInfo = plotObject.value("DopplerColorRangeInfo").toArray();
            int size = colorInfo.size();
            QMap<double, QColor> colorMap;
            for(int i = 0; i < size; i++)
            {
                QJsonObject obj = colorInfo.at(i).toObject();
                double value = obj.value("DopplerColorRangeValue").toDouble();
                QColor color = color_transfer::QColorFromRGBAStr(
                    obj.value("DopplerColorRangeColor").toString());
                colorMap.insert(value, color);
            }
            dopplerPlot->setColorRangeMap(colorMap);
        }
    }

    // 坐标变换的部分需要放到最后，前面有些设置还原时会影响坐标轴的效果，比如Bar切换横竖
    double lower, upper;
    lower = plotObject.value("PlotOriginX").toDouble();
    upper = plotObject.value("PlotEndX").toDouble();
    plot->setCoordRangeX(lower, upper);
    lower = plotObject.value("PlotOriginY").toDouble();
    upper = plotObject.value("PlotEndY").toDouble();
    plot->setCoordRangeY(lower, upper);
    plot->blockSignals(false);

    return plot;
}

void PlotXYDemo::saveDataPairToJson(DataPair* dataPair, QJsonObject& object, PlotType type)
{
    // 通用数据对信息
    object.insert("UUID", dataPair->getUuid());
    object.insert("Visible", dataPair->isDraw());
    object.insert("XEntityID", dataPair->getEntityIDX());
    object.insert("YEntityID", dataPair->getEntityIDY());
    object.insert("XEntityName", dataPair->getEntity_x());
    object.insert("YEntityName", dataPair->getEntity_y());
    object.insert("XAttrName", dataPair->getAttr_x());
    object.insert("YAttrName", dataPair->getAttr_y());
    object.insert("XAttrUnitName", dataPair->getUnit_x());
    object.insert("YAttrUnitName", dataPair->getUnit_y());
    auto xDataType = dataPair->getXDataType();
    object.insert("XDataType", static_cast<int32_t>(xDataType));
    if(xDataType == DataPair::RangeCalculation)
    {
        object.insert("XCalType", static_cast<int32_t>(dataPair->getXCalType()));
        object.insert("XTargetID", dataPair->getTargetEntityIDX());
        object.insert("XTargetEntityName", dataPair->getTargetEntityX());
    }
    auto yDataType = dataPair->getYDataType();
    object.insert("YDataType", static_cast<int32_t>(yDataType));
    if(yDataType == DataPair::RangeCalculation)
    {
        object.insert("YCalType", static_cast<int32_t>(dataPair->getYCalType()));
        object.insert("YTargetID", dataPair->getTargetEntityIDY());
        object.insert("YTargetEntityName", dataPair->getTargetEntityY());
    }

    object.insert("SecondsLimit", dataPair->getSecondsLimit());
    object.insert("PointsLimit", dataPair->getPointsLimit());
    // 图表特殊信息,先保存已经支持的信息
    if(type == PlotType::Type_PlotAScope)
    {
        // general
        object.insert("LineMode", dataPair->isLineMode());
        object.insert("Width", dataPair->width());
        object.insert("MatchColors", dataPair->matchColor());

        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));
        // stipple
        object.insert("StippleEnable", dataPair->getIsStippleEnable());
        object.insert("StipplePattern", dataPair->getStipplePattern());
    }
    else if(type == PlotType::Type_PlotAttitude || type == PlotType::Type_PlotDoppler)
    {}
    else if(type == PlotType::Type_PlotBar)
    {
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelSecondColor",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelSecColor()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
        object.insert("LabelXUint", dataPair->getUnit_x());
        object.insert("LabelXPrecision", dataPair->getLabelPrecision_x());
        object.insert("LabelPosition", dataPair->getLabelPosition());

        object.insert("ColorRangesEnable", dataPair->getColorRangeEnable());
        object.insert("ColorRangesDefColor",
                      color_transfer::QColorToRGBAStr(dataPair->getColorRangeDefaultColor()));
        object.insert("ColorRangesMode", dataPair->getColorRangeMode());
        object.insert("ColorRangesDesc", dataPair->colorRangesToString());
    }
    else if(type == PlotType::Type_PlotDial)
    {
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));
    }
    else if(type == PlotType::Type_PlotLight)
    {
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
        object.insert("LabelPosition", dataPair->getLabelPosition());
    }
    else if(type == PlotType::Type_PlotPolar)
    {
        object.insert("LineMode", dataPair->isLineMode());
        object.insert("Width", dataPair->width());
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
        object.insert("LabelXUint", dataPair->getUnit_x());
        object.insert("LabelXPrecision", dataPair->getLabelPrecision_x());
        object.insert("LabelYUint", dataPair->getUnit_y());
        object.insert("LabelYPrecision", dataPair->getLabelPrecision_y());
        object.insert("LabelPosition", dataPair->getLabelPosition());
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));
    }
    else if(type == PlotType::Type_PlotScatter)
    {
        object.insert("LineMode", dataPair->isLineMode());
        object.insert("Width", dataPair->width());
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        // icon
        object.insert("IconEnable", dataPair->isIconDraw());
        object.insert("IconFileName", dataPair->iconName());
        object.insert("IconRotation", dataPair->iconRotation());
        object.insert("IconFlipHoriz", dataPair->iconFlipHorz());
        object.insert("IconFlipVert", dataPair->iconFlipVert());
        object.insert("IconWidth", dataPair->iconWidth());
        object.insert("IconHeight", dataPair->iconHeight());
        object.insert("IconOverrideColor", color_transfer::QColorToRGBAStr(dataPair->iconColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
        object.insert("LabelXUint", dataPair->getUnit_x());
        object.insert("LabelXPrecision", dataPair->getLabelPrecision_x());
        object.insert("LabelYUint", dataPair->getUnit_y());
        object.insert("LabelYPrecision", dataPair->getLabelPrecision_y());
        object.insert("LabelPosition", dataPair->getLabelPosition());

        object.insert("LabelTextFormat", dataPair->getTextFormat());
        object.insert("LabelTextIncludePrefix", dataPair->isPrefixShow());
        object.insert("LabelTextIncludeObject", dataPair->isObjectShow());
        object.insert("LabelTextIncludeAttr", dataPair->isAttrShow());
        object.insert("LabelTextIncludeData", dataPair->isDataShow());
        object.insert("LabelTextIncludeUnit", dataPair->isUnitShow());
        object.insert("LabelTextCustomText", dataPair->getCustomText());

        object.insert("StippleEnable", dataPair->getIsStippleEnable());
        object.insert("StipplePattern", dataPair->getStipplePattern());
    }
    else if(type == PlotType::Type_PlotText)
    {
        object.insert("MatchColors", dataPair->matchColor());
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        object.insert("IconEnable", dataPair->isIconDraw());
        object.insert("IconFileName", dataPair->iconName());
        object.insert("IconRotation", dataPair->iconRotation());
        object.insert("IconFlipHoriz", dataPair->iconFlipHorz());
        object.insert("IconFlipVert", dataPair->iconFlipVert());
        object.insert("IconWidth", dataPair->iconWidth());
        object.insert("IconHeight", dataPair->iconHeight());
        object.insert("IconOverrideColor", color_transfer::QColorToRGBAStr(dataPair->iconColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
        object.insert("LabelXUint", dataPair->getUnit_x());
        object.insert("LabelXPrecision", dataPair->getLabelPrecision_x());
        object.insert("LabelPosition", dataPair->getLabelPosition());
    }
    else if(type == PlotType::Type_PlotTrack)
    {
        object.insert("Color", color_transfer::QColorToRGBAStr(dataPair->dataColor()));

        object.insert("LabelSettingsEnable", dataPair->isLabelTextShow());
        object.insert("LabelColor", color_transfer::QColorToRGBAStr(dataPair->getLabelColor()));
        object.insert("LabelBackground",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelBackground()));
        object.insert("LabelSecondColor",
                      color_transfer::QColorToRGBAStr(dataPair->getLabelSecColor()));
        object.insert("LabelFont", dataPair->getLabelFont().family());
        object.insert("LabelFontSize", dataPair->getLabelFontSize());
    }
}

void PlotXYDemo::loadDataPairJson(const QJsonObject& dataPairObject, PlotItemBase* plot)
{
    QString uuid = dataPairObject.value("UUID").toString();
    bool visible = dataPairObject.value("Visible").toBool();
    int32_t xEntityID = dataPairObject.value("XEntityID").toInt();
    QString xAttrName = dataPairObject.value("XAttrName").toString();
    QString xAttrUnitName = dataPairObject.value("XAttrUnitName").toString();

    int32_t yEntityID = dataPairObject.value("YEntityID").toInt();
    QString yAttrName = dataPairObject.value("YAttrName").toString();
    QString yAttrUnitName = dataPairObject.value("YAttrUnitName").toString();
    QHash<QString, QVariant> params;
    params.insert("UUID", uuid);
    // 兼容以前不存在属性
    if(dataPairObject.contains("XEntityName"))
    {
        QString xEntityName = dataPairObject.value("XEntityName").toString();
        params.insert("XEntityName", xEntityName);
    }
    if(dataPairObject.contains("YEntityName"))
    {
        QString yEntityName = dataPairObject.value("YEntityName").toString();
        params.insert("YEntityName", yEntityName);
    }
    if(dataPairObject.contains("XDataType"))
    {
        params.insert("XDataType", dataPairObject.value("XDataType").toInt());
    }
    if(dataPairObject.contains("YDataType"))
    {
        params.insert("YDataType", dataPairObject.value("YDataType").toInt());
    }
    if(dataPairObject.contains("XCalType"))
    {
        params.insert("XCalType", dataPairObject.value("XCalType").toInt());
    }
    if(dataPairObject.contains("YCalType"))
    {
        params.insert("YCalType", dataPairObject.value("YCalType").toInt());
    }
    if(dataPairObject.contains("XTargetID"))
    {
        params.insert("XTargetID", dataPairObject.value("XTargetID").toInt());
    }
    if(dataPairObject.contains("YTargetID"))
    {
        params.insert("YTargetID", dataPairObject.value("YTargetID").toInt());
    }
    if(dataPairObject.contains("XTargetEntityName"))
    {
        params.insert("XTargetEntityName", dataPairObject.value("XTargetEntityName").toString());
    }
    if(dataPairObject.contains("YTargetEntityName"))
    {
        params.insert("YTargetEntityName", dataPairObject.value("YTargetEntityName").toString());
    }

    auto dataPair = plot->addPlotDataPair(
        xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName, params, true);
    dataPair->blockSignals(true);
    dataPair->setDraw(visible);
    dataPair->setSecondsLimit(dataPairObject.value("SecondsLimit").toDouble());
    dataPair->setPointsLimit(dataPairObject.value("PointsLimit").toInt());
    PlotType type = plot->plotType();
    if(type == PlotType::Type_PlotAScope)
    {
        dataPair->setLineMode(dataPairObject.value("LineMode").toBool());
        dataPair->setWidth(dataPairObject.value("Width").toInt());
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setIsStippleEnable(dataPairObject.value("StippleEnable").toBool());
        dataPair->setStipplePattern(
            static_cast<Qt::PenStyle>(dataPairObject.value("StipplePattern").toInt()));
    }
    else if(type == PlotType::Type_PlotAttitude || type == PlotType::Type_PlotDoppler)
    {}
    else if(type == PlotType::Type_PlotBar)
    {
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelSecColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelSecondColor").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
        dataPair->setUnit_x(dataPairObject.value("LabelXUint").toString());
        dataPair->setLabelPrecision_x(dataPairObject.value("LabelXPrecision").toInt());
        dataPair->setLabelPosition(
            static_cast<DataPair::TEXT_POSITION>(dataPairObject.value("LabelPosition").toInt()));

        dataPair->setColorRangeEnable(dataPairObject.value("ColorRangesEnable").toBool());
        dataPair->setColorRangeDefaultColor(
            QColor(dataPairObject.value("ColorRangesDefColor").toString()));
        dataPair->setColorRangeMode(
            static_cast<DataPair::ColorRangeMode>(dataPairObject.value("ColorRangesMode").toInt()));
        dataPair->colorRangesFromString(dataPairObject.value("ColorRangesDesc").toString());
    }
    else if(type == PlotType::Type_PlotDial)
    {
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));
    }
    else if(type == PlotType::Type_PlotLight)
    {
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
        dataPair->setLabelPosition(
            static_cast<DataPair::TEXT_POSITION>(dataPairObject.value("LabelPosition").toInt()));
    }
    else if(type == PlotType::Type_PlotPolar)
    {
        dataPair->setLineMode(dataPairObject.value("LineMode").toBool());
        dataPair->setWidth(dataPairObject.value("Width").toInt());
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
        dataPair->setUnit_x(dataPairObject.value("LabelXUint").toString());
        dataPair->setLabelPrecision_x(dataPairObject.value("LabelXPrecision").toInt());
        dataPair->setUnit_y(dataPairObject.value("LabelYUint").toString());
        dataPair->setLabelPrecision_y(dataPairObject.value("LabelYPrecision").toInt());
        dataPair->setLabelPosition(
            static_cast<DataPair::TEXT_POSITION>(dataPairObject.value("LabelPosition").toInt()));
    }
    else if(type == PlotType::Type_PlotRTI)
    {
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));
    }
    else if(type == PlotType::Type_PlotScatter)
    {
        dataPair->setLineMode(dataPairObject.value("LineMode").toBool());
        dataPair->setWidth(dataPairObject.value("Width").toInt());
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setIconDraw(dataPairObject.value("IconEnable").toBool());
        dataPair->setIconName(dataPairObject.value("IconFileName").toString());
        dataPair->setIconRotation(
            static_cast<DataPair::ICON_ROTATION>(dataPairObject.value("IconRotation").toInt()));
        dataPair->setIconFlipHorz(dataPairObject.value("IconFlipHoriz").toBool());
        dataPair->setIconFlipVert(dataPairObject.value("IconFlipVert").toBool());
        dataPair->setIconWidth(dataPairObject.value("IconWidth").toInt());
        dataPair->setIconHeight(dataPairObject.value("IconHeight").toInt());
        dataPair->setIconColor(color_transfer::QColorFromRGBAStr(
            dataPairObject.value("IconOverrideColor").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
        dataPair->setUnit_x(dataPairObject.value("LabelXUint").toString());
        dataPair->setLabelPrecision_x(dataPairObject.value("LabelXPrecision").toInt());
        dataPair->setUnit_y(dataPairObject.value("LabelYUint").toString());
        dataPair->setLabelPrecision_y(dataPairObject.value("LabelYPrecision").toInt());
        dataPair->setLabelPosition(
            static_cast<DataPair::TEXT_POSITION>(dataPairObject.value("LabelPosition").toInt()));

        dataPair->setTextFormat(
            static_cast<DataPair::TEXT_FROMAT>(dataPairObject.value("LabelTextFormat").toInt()));
        dataPair->setPrefixShow(dataPairObject.value("LabelTextIncludePrefix").toBool());
        dataPair->setObjectShow(dataPairObject.value("LabelTextIncludeObject").toBool());
        dataPair->setAttrShow(dataPairObject.value("LabelTextIncludeAttr").toBool());
        dataPair->setDataShow(dataPairObject.value("LabelTextIncludeData").toBool());
        dataPair->setUnitShow(dataPairObject.value("LabelTextIncludeUnit").toBool());
        dataPair->setCustomText(dataPairObject.value("LabelTextCustomText").toString());

        dataPair->setIsStippleEnable(dataPairObject.value("StippleEnable").toBool());
        dataPair->setStipplePattern(
            static_cast<Qt::PenStyle>(dataPairObject.value("StipplePattern").toInt()));
    }
    else if(type == PlotType::Type_PlotText)
    {
        dataPair->setMatchColor(dataPairObject.value("MatchColors").toBool());
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setIconDraw(dataPairObject.value("IconEnable").toBool());
        dataPair->setIconName(dataPairObject.value("IconFileName").toString());
        dataPair->setIconRotation(
            static_cast<DataPair::ICON_ROTATION>(dataPairObject.value("IconRotation").toInt()));
        dataPair->setIconFlipHorz(dataPairObject.value("IconFlipHoriz").toBool());
        dataPair->setIconFlipVert(dataPairObject.value("IconFlipVert").toBool());
        dataPair->setIconWidth(dataPairObject.value("IconWidth").toInt());
        dataPair->setIconHeight(dataPairObject.value("IconHeight").toInt());
        dataPair->setIconColor(color_transfer::QColorFromRGBAStr(
            dataPairObject.value("IconOverrideColor").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
        dataPair->setUnit_x(dataPairObject.value("LabelXUint").toString());
        dataPair->setLabelPrecision_x(dataPairObject.value("LabelXPrecision").toInt());
        dataPair->setLabelPosition(
            static_cast<DataPair::TEXT_POSITION>(dataPairObject.value("LabelPosition").toInt()));
    }
    else if(type == PlotType::Type_PlotTrack)
    {
        dataPair->setColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("Color").toString()));

        dataPair->setLabelTextShow(dataPairObject.value("LabelSettingsEnable").toBool());
        dataPair->setLabelColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelColor").toString()));
        dataPair->setLabelBackground(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelBackground").toString()));
        dataPair->setLabelSecColor(
            color_transfer::QColorFromRGBAStr(dataPairObject.value("LabelSecondColor").toString()));
        dataPair->setLabelFont(QFont(dataPairObject.value("LabelFont").toString()));
        dataPair->setLabelFontSize(dataPairObject.value("LabelFontSize").toInt());
    }
    dataPair->blockSignals(false);
    // 每次set接口会调用一次刷新，为了提高性能，这里等到全部参数设置完成才进行通知刷新
    emit dataPair->dataUpdate();
}

void PlotXYDemo::savePlotManagerData(QJsonObject& plotManagerObject)
{
    auto linkedAxesSets = PlotManagerData::getInstance()->getLinkedAxesSets();
    QJsonArray linkedAxesArray;
    for(const auto& set : linkedAxesSets)
    {
        QJsonObject setObj;
        setObj.insert("GroupName", set.groupName);
        setObj.insert("IsXAxes", set.isX);
        QString plotNameList;
        auto plotList = set.plotList;
        for(auto plot : plotList)
        {
            plotNameList.append(plot->getTabName() + "," + plot->getName() + ";");
        }
        setObj.insert("PlotNameList", plotNameList);

        linkedAxesArray.append(setObj);
    }

    plotManagerObject.insert("LinkedAxesSets", linkedAxesArray);
}

void PlotXYDemo::loadPlotManagerData(const QJsonObject& plotManagerObject)
{
    QJsonArray linkedAxesArray = plotManagerObject.value("LinkedAxesSets").toArray();
    QVector<PlotManagerData::LinkedAxesGroupSet> sets;
    for(const auto& setValue : linkedAxesArray)
    {
        QJsonObject setObj = setValue.toObject();
        PlotManagerData::LinkedAxesGroupSet set;
        set.groupName = setObj.value("GroupName").toString();
        set.isX = setObj.value("IsXAxes").toBool();
        QString plotListStr = setObj.value("PlotNameList").toString();
        QStringList plotInfoList = plotListStr.split(";", QString::SkipEmptyParts);
        for(const QString& tmpPlotInfo : plotInfoList)
        {
            QStringList plotInfo = tmpPlotInfo.split(",", QString::SkipEmptyParts);
            if(plotInfo.size() == 2)
            {
                QString tabName = plotInfo.at(0);
                QString plotName = plotInfo.at(1);
                auto plot = PlotManagerData::getInstance()->getPlotByTabAndName(tabName, plotName);
                set.plotList.append(plot);
            }
        }
        sets.append(set);
    }
    PlotManagerData::getInstance()->setLinkedAxesSets(sets);
}

void PlotXYDemo::clearAllTab()
{
    PlotManagerData::getInstance()->clearPlotData();
    auto count = ui.tabWidget->count();
    for(int var = 0; var < count; ++var)
    {
        delete ui.tabWidget->widget(var);
    }
    ui.tabWidget->clear();
}

void PlotXYDemo::closeEvent(QCloseEvent* e)
{
    //关闭本窗口时先关闭其他顶层窗口，才能结束进程
    e->accept();
    qApp->quit();
}

bool PlotXYDemo::getIsRealTime()
{
    return m_isRealTime;
}

double PlotXYDemo::getSeconds()
{
    return m_seconds;
}

void PlotXYDemo::onPlay()
{
    m_bIsPlayForward = true;
    ui.actionPlay->setChecked(true);
    ui.actionReverse->setChecked(false);
    if(m_timer->isActive())
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
    if(m_timer->isActive())
        m_timer->stop();
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onFrameReverse()
{
    if(m_timer->isActive())
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
    if(m_timer->isActive())
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
    if(m_timerInterval >= 8000)
        return;

    if(m_timer->isActive())
        m_timer->stop();

    m_timerInterval *= 2;
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onIncreaseStep()
{
    if(m_timerInterval <= 125)
        return;

    if(m_timer->isActive())
        m_timer->stop();

    m_timerInterval /= 2;
    m_timer->start(m_timerInterval);
    ui.actionStop->setEnabled(true);
    emit sgn_enableActionStop(true);
}

void PlotXYDemo::onTimeServer() {}

void PlotXYDemo::onTimeClient() {}

void PlotXYDemo::onRealTime()
{
    m_isRealTime = ui.actionReal_Time->isChecked();
    DataManager::getInstance()->loadLiveEventType();
    if(m_isRealTime)
    {
        // 清理离线数据
        DataManager::getInstance()->clearData();
        DataManager::getInstance()->getRecvThread()->start();
    }
    else
    {
        DataManager::getInstance()->getRecvThread()->stop();
    }
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
    ui.actionShow_RTI_Controls->setVisible(false);
    ui.actionShow_All_Bars_Ctrl_B->setVisible(false);
    ui.actionShow_Plot_Time_Ctrl_Alt_P->setVisible(false);
    ui.actionShow_System_Time_Ctrl_Alt_Y->setVisible(false);
    ui.actionShow_Corner_Logo->setVisible(false);
    ui.actionShow_Frame_Rate_Alt_F->setVisible(false);

    ui.menuRecent_Files->menuAction()->setVisible(false);
    ui.menuRecent_PMLs->menuAction()->setVisible(false);
    ui.menuPlug_ins->menuAction()->setVisible(false);
    ui.menuTools->menuAction()->setVisible(false);
    ui.menuBookmarks->menuAction()->setVisible(false);
}

void PlotXYDemo::initMenuFile()
{
    connect(ui.actionopen, &QAction::triggered, this, &PlotXYDemo::onOpenFile);
    connect(ui.actionopenNetwork, &QAction::triggered, this, &PlotXYDemo::onOpenNetwork);
    connect(ui.action_ExportDataAndFile, &QAction::triggered, this, &PlotXYDemo::onExportDataStore);
    connect(ui.action_ExportOnlyFile, &QAction::triggered, this, &PlotXYDemo::onExportOnlyFile);
    connect(ui.actionclose, &QAction::triggered, this, &PlotXYDemo::onClose_Disconnect);
    connect(ui.actionRun_Python_Script_Ctrl_E,
            &QAction::triggered,
            this,
            &PlotXYDemo::onRunPythonScript);
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
    connect(
        ui.actionLock_Stacking_Order, &QAction::triggered, this, &PlotXYDemo::onLockStackingOrder);
    connect(ui.actionInvert_Colores, &QAction::triggered, this, &PlotXYDemo::onInvertColors);
    // 排序菜单信号槽
    connect(ui.actionBring_To_Top, &QAction::triggered, this, &PlotXYDemo::onBringToTop);
    connect(ui.actionSend_To_Bottom, &QAction::triggered, this, &PlotXYDemo::onSendToBottom);
    connect(ui.actionBring_Forward, &QAction::triggered, this, &PlotXYDemo::onBringForward);
    connect(ui.actionSend_Backward, &QAction::triggered, this, &PlotXYDemo::onSendBackward);
    connect(ui.actionTile_Horizontally, &QAction::triggered, this, &PlotXYDemo::onHorizonAlign);
    connect(ui.actionTile_Vertically, &QAction::triggered, this, &PlotXYDemo::onVerticalAlign);

    connect(ui.actionAddBarPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddAttitudePlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddTextPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddPolarPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddLightPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddTrackPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddAScopePlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddRTIPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddDopplerPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddScatterPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
    connect(ui.actionAddDialPlot, &QAction::triggered, this, &PlotXYDemo::onAddPlot);
}

void PlotXYDemo::initMenuData()
{
    connect(ui.actionAdvanced_Data_Manager, &QAction::triggered, this, &PlotXYDemo::onAdvancedData);
    connect(ui.actionAdd_Plot_Pair_Ctrl_A, SIGNAL(triggered()), this, SLOT(onAddPlotPair()));
    connect(ui.actionDiscovery_Rules, &QAction::triggered, this, &PlotXYDemo::onDiscoveryRules);
    connect(ui.actionShow_Object_Aliases_Ctrl_Alt_A,
            SIGNAL(triggered()),
            this,
            SLOT(onShowObjectAliases()));
    connect(ui.actionReference_Points_Ctrl_R,
            &QAction::triggered,
            this,
            &PlotXYDemo::onReferencePoints);
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

    connect(m_timeCtrl, &TimeControls::sgn_setSliderRange, this, &PlotXYDemo::onSetSliderRange);

    connect(ui.timeSlider,
            &QSlider::valueChanged,
            m_timeCtrl,
            &TimeControls::onRemoteSliderValueChanged);
    connect(m_timeCtrl,
            &TimeControls::sgn_sliderValueChanged,
            this,
            &PlotXYDemo::onRemoteSliderValueChanged);
    connect(this, &PlotXYDemo::sgn_enableActionStop, m_timeCtrl, &TimeControls::onEnableActionStop);

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

    // TimeControl
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
    connect(ui.actionPrevious_Tab_Ctrl_Shift_Tab,
            &QAction::triggered,
            this,
            &PlotXYDemo::onPreviousTab);
}

void PlotXYDemo::initMenuView()
{
    connect(
        ui.actionShow_Menubar_Ctrl_Alt_M, &QAction::triggered, this, &PlotXYDemo::onShowMenubar);
    connect(
        ui.actionShow_Toolbar_Ctrl_Alt_C, &QAction::triggered, this, &PlotXYDemo::onShowToolbar);
    connect(ui.actionShow_Statusbar_Ctrl_Alt_B,
            &QAction::triggered,
            this,
            &PlotXYDemo::onShowStatusbar);
    connect(ui.actionShow_Time_Slider, &QAction::triggered, this, &PlotXYDemo::onShowTimeSlider);
    connect(ui.actionShow_Tabbar, &QAction::triggered, this, &PlotXYDemo::onShowTabbar);
    connect(ui.actionShow_RTI_Controls, &QAction::triggered, this, &PlotXYDemo::onShowRTIControls);
    connect(ui.actionShow_All_Bars_Ctrl_B, &QAction::triggered, this, &PlotXYDemo::onShowAllBars);
    connect(
        ui.actionShow_Plot_Time_Ctrl_Alt_P, &QAction::triggered, this, &PlotXYDemo::onShowPlotTime);
    connect(ui.actionShow_System_Time_Ctrl_Alt_Y,
            &QAction::triggered,
            this,
            &PlotXYDemo::onShowSystemTime);
    connect(ui.actionShow_Corner_Logo, &QAction::triggered, this, &PlotXYDemo::onShowLogo);
    connect(
        ui.actionShow_Frame_Rate_Alt_F, &QAction::triggered, this, &PlotXYDemo::onShowFrameRate);
    connect(ui.actionFullScreen_Shift_F, &QAction::triggered, this, &PlotXYDemo::onShowFullScreen);
}

void PlotXYDemo::initMenuTools()
{
    connect(ui.actionSave_Preferences, &QAction::triggered, this, &PlotXYDemo::onSavePreferences);
    connect(
        ui.actionSave_Preferences_As, &QAction::triggered, this, &PlotXYDemo::onSavePreferencesAs);
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
    m_statusBar_StackLock = new QToolButton(this);
    m_statusBar_dataTime = new QLabel("<数据时间>", this);
    m_statusBar_localTime = new QLabel("<系统时间>", this);
    m_statusBar_selectPlot = new QToolButton(this);
    m_statusBar_pan = new QToolButton(this);
    m_statusBar_centerPlot = new QToolButton(this);
    m_statusBar_zoom = new QToolButton(this);
    m_statusBar_boxZoom = new QToolButton(this);
    m_statusBar_measure = new QToolButton(this);
    m_statusBar_createPlot = new QToolButton(this);
    m_statusBar_movePlot = new QToolButton(this);
    m_statusBar_null = new QLabel(this);

    m_statusBar_EditLock->setToolTip("锁定编辑");
    m_statusBar_StackLock->setToolTip("锁定堆叠");
    m_statusBar_selectPlot->setToolTip("选择图表");
    m_statusBar_pan->setToolTip("平移");
    m_statusBar_centerPlot->setToolTip("居中图表");
    m_statusBar_zoom->setToolTip("缩放");
    m_statusBar_boxZoom->setToolTip("框选缩放");
    m_statusBar_measure->setToolTip("测距");
    m_statusBar_createPlot->setToolTip("创建图表");
    m_statusBar_movePlot->setToolTip("移动图表");

    m_statusBar_EditLock->setIcon(QIcon(":/statusbar/editingLock.bmp"));
    m_statusBar_StackLock->setIcon(QIcon(":/statusbar/stackingLock.bmp"));
    m_statusBar_selectPlot->setIcon(QIcon(":/statusbar/selectPlot.bmp"));
    m_statusBar_pan->setIcon(QIcon(":/statusbar/pan.bmp"));
    m_statusBar_centerPlot->setIcon(QIcon(":/statusbar/centerPlot.bmp"));
    m_statusBar_zoom->setIcon(QIcon(":/statusbar/zoom.bmp"));
    m_statusBar_boxZoom->setIcon(QIcon(":/statusbar/boxZoom.bmp"));
    m_statusBar_measure->setIcon(QIcon(":/statusbar/measureDistance.bmp"));
    m_statusBar_createPlot->setIcon(QIcon(":/statusbar/createPlot.bmp"));
    m_statusBar_movePlot->setIcon(QIcon(":/statusbar/movePlot.bmp"));

    m_statusBar_EditLock->setIconSize(QSize(25, 25));
    m_statusBar_StackLock->setIconSize(QSize(25, 25));
    m_statusBar_selectPlot->setIconSize(QSize(25, 25));
    m_statusBar_pan->setIconSize(QSize(25, 25));
    m_statusBar_centerPlot->setIconSize(QSize(25, 25));
    m_statusBar_zoom->setIconSize(QSize(25, 25));
    m_statusBar_boxZoom->setIconSize(QSize(25, 25));
    m_statusBar_measure->setIconSize(QSize(25, 25));
    m_statusBar_createPlot->setIconSize(QSize(25, 25));
    m_statusBar_movePlot->setIconSize(QSize(25, 25));

    m_statusBar_EditLock->setCheckable(true);
    m_statusBar_StackLock->setCheckable(true);
    m_statusBar_selectPlot->setCheckable(true);
    // 默认是选择第一种模式
    m_statusBar_selectPlot->setChecked(true);
    m_statusBar_pan->setCheckable(true);
    m_statusBar_centerPlot->setCheckable(true);
    m_statusBar_zoom->setCheckable(true);
    m_statusBar_boxZoom->setCheckable(true);
    m_statusBar_measure->setCheckable(true);
    m_statusBar_createPlot->setCheckable(true);
    m_statusBar_movePlot->setCheckable(true);

    m_statusBar_info->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusBar_dataTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    m_statusBar_localTime->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    m_statusBar_dataTime->setAlignment(Qt::AlignCenter | Qt::AlignHCenter);
    m_statusBar_null->setMinimumSize(QSize(40, 10));
    m_statusBar_null->setEnabled(false);

    ui.statusBar->addWidget(m_statusBar_info, 1);
    ui.statusBar->addWidget(m_statusBar_EditLock);
    ui.statusBar->addWidget(m_statusBar_StackLock);
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

    connect(m_statusBar_EditLock, &QToolButton::clicked, this, &PlotXYDemo::onLockDataDisplay);
    connect(m_statusBar_StackLock, &QToolButton::clicked, this, &PlotXYDemo::onLockStackingOrder);

    QButtonGroup* statusBtnGroup = new QButtonGroup(this);
    statusBtnGroup->addButton(m_statusBar_selectPlot, static_cast<int>(MouseMode::SelectPlot));
    statusBtnGroup->addButton(m_statusBar_pan, static_cast<int>(MouseMode::Pan));
    statusBtnGroup->addButton(m_statusBar_centerPlot, static_cast<int>(MouseMode::CenterPlot));
    statusBtnGroup->addButton(m_statusBar_zoom, static_cast<int>(MouseMode::Zoom));
    statusBtnGroup->addButton(m_statusBar_boxZoom, static_cast<int>(MouseMode::BoxZoom));
    statusBtnGroup->addButton(m_statusBar_measure, static_cast<int>(MouseMode::MeasureDistance));
    statusBtnGroup->addButton(m_statusBar_movePlot, static_cast<int>(MouseMode::MovePlot));
    statusBtnGroup->addButton(m_statusBar_createPlot, static_cast<int>(MouseMode::CreatePlot));
    statusBtnGroup->setExclusive(true);
    connect(statusBtnGroup,
            QOverload<int>::of(&QButtonGroup::buttonClicked),
            this,
            &PlotXYDemo::onStatusBtnClicked);

    m_localTimer = new QTimer(this);
    connect(m_localTimer, &QTimer::timeout, this, &PlotXYDemo::onUpdateLocalTime);
    m_localTimer->start(1000);

    updateStatusBarInfo();
}

void PlotXYDemo::updateStatusBarInfo()
{
    if(m_pCurSelectedPlot)
    {
        m_statusBar_info->setText(QString("当前选择的图表：%1-%2")
                                      .arg(m_pCurSelectedPlot->getTabName())
                                      .arg(m_pCurSelectedPlot->getName()));
    }
    else
    {
        m_statusBar_info->setText(QString("当前选择的图表：- "));
    }
}

void PlotXYDemo::onOpenNetwork()
{
    NetSettingDialog dialog;
    dialog.exec();
}

void PlotXYDemo::onExportDataStore()
{
    QString pxyFileName =
        QFileDialog::getSaveFileName(nullptr, "保存", getDatasPath(), "PXY (*.pxy)");
    if(!pxyFileName.isEmpty())
    {
        savePXYData(pxyFileName, true);
    }
}

void PlotXYDemo::onExportOnlyFile()
{
    QString pxyFileName =
        QFileDialog::getSaveFileName(nullptr, "保存", getDatasPath(), "PXY (*.pxy)");
    if(!pxyFileName.isEmpty())
    {
        savePXYData(pxyFileName, false);
    }
}

void PlotXYDemo::onClose_Disconnect()
{
    //清空上次加载数据
    DataManagerInstance->clearData();
}

void PlotXYDemo::onRunPythonScript() {}

void PlotXYDemo::onLoadPML() {}

void PlotXYDemo::onSaveToPML() {}

void PlotXYDemo::onSaveScreenshot()
{
    QString filename =
        QFileDialog::getSaveFileName(nullptr, "保存截图", ".", "JPG (*.jpg);;PNG (*.png)");
    if(filename.isEmpty())
    {
        return;
    }
    if(auto plot = getCurDrawWidget())
    {
        QPixmap pixmap = plot->grab();
        pixmap.save(filename);
    }
}

void PlotXYDemo::onQuit()
{
    qApp->quit();
}

void PlotXYDemo::onUndo() {}

void PlotXYDemo::onRedo() {}

void PlotXYDemo::onCut() {}

void PlotXYDemo::onCopy() {}

void PlotXYDemo::onPaste() {}
// 删除选中图表
void PlotXYDemo::onDelete()
{
    if(m_pCurSelectedPlot)
    {
        PlotManagerData::getInstance()->deletePlotByTab(m_pCurSelectedPlot->getTabName(),
                                                        m_pCurSelectedPlot);
        m_pCurSelectedPlot->deleteLater();
        m_pCurSelectedPlot = nullptr;
        updateStatusBarInfo();
    }
}
