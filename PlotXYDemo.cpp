#include "PlotXYDemo.h"

#include <QApplication>
#include <QButtonGroup>
#include <QCloseEvent>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMetaType>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>

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

double PlotXYDemo::m_seconds = 0.0;

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

    connect(this,
            SIGNAL(sgn_renameTabPage(QString, QString)),
            PlotManagerData::getInstance(),
            SLOT(slotChangeTabName(QString, QString)));

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

void PlotXYDemo::onShowTimeSlider() {}

void PlotXYDemo::onShowTabbar() {}

void PlotXYDemo::onShowRTIControls() {}

void PlotXYDemo::onShowAllBars() {}

void PlotXYDemo::onShowPlotTime() {}

void PlotXYDemo::onShowSystemTime() {}

void PlotXYDemo::onShowLogo() {}

void PlotXYDemo::onShowFrameRate() {}

void PlotXYDemo::onShowFullScreen() {}

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

        emit sgn_renameTabPage(oldName, newName);
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

    //发送数据时间
    emit currentSecsChanged(m_seconds);
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
        name = QString("Tab ") + QString::number(currCount + 1);
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
    connect(this, &PlotXYDemo::currentSecsChanged, plotItem, &PlotItemBase::onGetCurrentSeconds);
    connect(
        plotItem, &PlotItemBase::dataPairsChanged, m_addPlotPair, &AddPlotPair::onUpdatePlotPair);
    connect(plotItem, &PlotItemBase::dataPairsChanged, m_plotManager, &PlotManager::onSelectedPlot);
    connect(plotItem,
            &PlotItemBase::dataPairsChanged,
            m_AdvancedDataManager,
            &AdvancedDataManager::onUpdatePlotPair);

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

void PlotXYDemo::savePXYData(const QString& pxyFileName)
{
    QFile file(pxyFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QJsonObject allObject;
    // 通用信息
    allObject.insert("Date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    // 在线模式先存储数据
    QString dataFileName;
    if(DataManagerInstance->getIsRealTime())
    {
        dataFileName = QFileInfo(pxyFileName).absolutePath() + "/" +
                       QFileInfo(pxyFileName).baseName() + ".asi";
        if(!DataManagerInstance->saveDataToASI(dataFileName))
        {
            qDebug() << "保存在线数据失败";
            dataFileName = "";
        }
    }
    else
    {
        dataFileName = DataManagerInstance->getDataFileName();
    }
    allObject.insert("DataPath", dataFileName);

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

    QJsonDocument jsonDoc;
    jsonDoc.setObject(allObject);
    file.write(jsonDoc.toJson());
    file.close();
}

void PlotXYDemo::loadPXYData(const QString& pxyFileName)
{
    QFile inFile(pxyFileName);
    inFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray data = inFile.readAll();
    inFile.close();

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    if(error.error != QJsonParseError::NoError)
    {
        qDebug() << "Parse failed";
    }
    QJsonObject rootObj = doc.object();
    QString dataFileName = rootObj.value("DataPath").toString();
    if(!dataFileName.isEmpty())
    {
        DataManagerInstance->loadFileData(dataFileName);
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
    PlotManagerData::getInstance()->blockSignals(false);
    // 屏蔽掉PlotManagerData中的信号，等全部加载完了之后，在触发信号，统一刷新一次三个界面
    emit PlotManagerData::getInstance()->plotDataChanged();
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
    // 坐标变换的部分需要放到最后，前面有些设置还原时会影响坐标轴的效果，比如Bar切换横竖
    double lower, upper;
    lower = plotObject.value("PlotOriginX").toDouble();
    upper = plotObject.value("PlotEndX").toDouble();
    plot->setCoordRangeX(lower, upper);
    lower = plotObject.value("PlotOriginY").toDouble();
    upper = plotObject.value("PlotEndY").toDouble();
    plot->setCoordRangeY(lower, upper);

    return plot;
}

void PlotXYDemo::saveDataPairToJson(DataPair* dataPair, QJsonObject& object, PlotType type)
{
    // 通用数据对信息
    object.insert("UUID", dataPair->getUuid());
    object.insert("Visible", dataPair->isDraw());
    object.insert("XEntityID", dataPair->getEntityIDX());
    object.insert("YEntityID", dataPair->getEntityIDY());
    object.insert("XAttrName", dataPair->getAttr_x());
    object.insert("YAttrName", dataPair->getAttr_y());
    object.insert("XAttrUnitName", dataPair->getUnit_x());
    object.insert("YAttrUnitName", dataPair->getUnit_y());
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
    bool isChecked = ui.actionReal_Time->isChecked();
    DataManager::getInstance()->setIsRealTime(isChecked);
    if(isChecked)
    {
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
}

void PlotXYDemo::initMenuFile()
{
    connect(ui.actionopen, &QAction::triggered, this, &PlotXYDemo::onOpenFile);
    connect(ui.actionopenNetwork, &QAction::triggered, this, &PlotXYDemo::onOpenNetwork);
    connect(ui.actionexport, &QAction::triggered, this, &PlotXYDemo::onExportDataStore);
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
    m_statusBar_StackLock->setToolTip("Stacking Lock");
    m_statusBar_selectPlot->setToolTip("Select Plot");
    m_statusBar_pan->setToolTip("Pan");
    m_statusBar_centerPlot->setToolTip("Center Plot");
    m_statusBar_zoom->setToolTip("Zoom");
    m_statusBar_boxZoom->setToolTip("Box Zoom");
    m_statusBar_measure->setToolTip("Measure");
    m_statusBar_createPlot->setToolTip("Create Plot");
    m_statusBar_movePlot->setToolTip("Move Plot");

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
        m_statusBar_info->setText(QString("当前选择的Plot：%1-%2")
                                      .arg(m_pCurSelectedPlot->getTabName())
                                      .arg(m_pCurSelectedPlot->getName()));
    }
    else
    {
        m_statusBar_info->setText(QString("当前选择的Plot：- "));
    }
}

void PlotXYDemo::onOpenNetwork()
{
    NetSettingDialog* dialog = new NetSettingDialog;
    dialog->show();
}

void PlotXYDemo::onExportDataStore()
{
    QString pxyFileName =
        QFileDialog::getSaveFileName(nullptr, "保存", getDatasPath(), "PXY (*.pxy)");
    if(!pxyFileName.isEmpty())
    {
        savePXYData(pxyFileName);
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
