#include "PlotXYDemo.h"

#include <QButtonGroup>
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
#include "rename_tab_dialog.h"
#include "tabdrawwidget.h"

PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
    , m_mouseMode(MouseMode::SelectPlot)
{
    ui.setupUi(this);
    setMinimumSize(1600, 900);
    setWindowTitle("仿真数据分析");

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

    connect(this,
            SIGNAL(sgn_renameTabPage(QString, QString)),
            PlotManagerData::getInstance(),
            SLOT(slotChangeTabName(QString, QString)));

    connect(m_plotManager,
            &PlotManager::sigAddPlotPair,
            this,
            QOverload<>::of(&PlotXYDemo::onAddPlotPair));
    connect(m_plotManager, SIGNAL(sigAdvancedDataManager()), this, SLOT(onAdvancedData()));
    connect(
        this, &PlotXYDemo::sgn_sendTabWidgetRect, m_plotManager, &PlotManager::onGetTabWidgetRect);
    connect(m_plotManager, &PlotManager::sigGetTabRect, this, &PlotXYDemo::onSendTabRect);
    connect(m_AdvancedDataManager, SIGNAL(sgnAddPlotPair()), this, SLOT(onAddPlotPair()));
    QRect tabRect = ui.tabWidget->rect();
    emit sgn_sendTabWidgetRect(tabRect);
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
    m_AdvancedDataManager->show();
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
    m_plotManager->show();
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
    QString path =
        QFileDialog::getOpenFileName(this, "Open File", ".", tr("ASI(*.asi) ;; PXY(*.pxy)"));
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
        m_pCurSelectedPlot->setAttribute(Qt::WA_TransparentForMouseEvents, false);
        m_pCurSelectedPlot->setZoom(1);
        break;
    case MouseMode::BoxZoom:
        draw->setCursor(QCursor(QPixmap(":/box_zoom.svg")));
        m_pCurSelectedPlot->setAttribute(Qt::WA_TransparentForMouseEvents, false);
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

    QMenu* ViewMenu = new QMenu(QString("View"));

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
    pMenu.addMenu(ViewMenu);
    pMenu.addMenu(ui.menuSelect_Plot);

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

void PlotXYDemo::onShowMenubar()
{
    //  menuBar()->setVisible(false);
}

void PlotXYDemo::onShowToolbar() {}

void PlotXYDemo::onShowStatusbar() {}

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
    QRect tabRect = ui.tabWidget->rect();
    emit sgn_sendTabWidgetRect(tabRect);
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

void PlotXYDemo::onAutofit_Full() {}

void PlotXYDemo::onAutofit_X() {}

void PlotXYDemo::onAutofit_Y() {}

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
    double secs = (double)value / m_timeCtrl->getMultiplizer();
    int refYear = m_timeCtrl->getRefYear();
    // show data time
    QString dataTime = OrdinalTimeFormatter::toString(secs, refYear);
    m_statusBar_dataTime->setText(dataTime);

    //发送数据时间
    emit currentSecsChanged(secs);
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
    QString dataFileName = DataManagerInstance->getDataFileName();
    if(dataFileName.isEmpty())
    {
        return;
    }

    QFile file(pxyFileName);
    if(!file.open(QIODevice::WriteOnly))
    {
        return;
    }
    QJsonObject allObject;
    // 通用信息
    allObject.insert("Date", QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
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
            QString plotName = plot->getName();
            QJsonObject plotObject;

            plotObject.insert("PlotName", plotName);
            plotObject.insert("X", plot->x());
            plotObject.insert("Y", plot->y());
            plotObject.insert("Width", plot->width());
            plotObject.insert("Height", plot->height());
            plotObject.insert("PlotType", plot->plotType());
            // 图表存在多个数据对
            QJsonArray dataPairArray;
            auto dataPairs = plot->getDataPairs();
            for(DataPair* dataPair : dataPairs)
            {
                QJsonObject dataPairObject;
                saveDataPairToJson(dataPair, dataPairObject);
                dataPairArray.append(dataPairObject);
            }
            plotObject.insert("DataPairs", dataPairArray);
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
            int32_t x = plotObject.value("X").toInt();
            int32_t y = plotObject.value("Y").toInt();
            int32_t width = plotObject.value("Width").toInt();
            int32_t height = plotObject.value("Height").toInt();
            PlotType type = static_cast<PlotType>(plotObject.value("PlotType").toInt());
            QString plotName = plotObject.value("PlotName").toString();
            auto plot = addPlotWidget(type, QRect(x, y, width, height), plotName);
            QJsonArray dataPairArray = plotObject.value("DataPairs").toArray();
            auto dataPairSize = dataPairArray.size();
            for(int m = 0; m < dataPairSize; ++m)
            {
                QJsonObject dataPairObject = dataPairArray.at(m).toObject();
                QString uuid = dataPairObject.value("UUID").toString();
                bool visible = dataPairObject.value("Visible").toBool();
                int32_t xEntityID = dataPairObject.value("XEntityID").toInt();
                QString xAttrName = dataPairObject.value("XAttrName").toString();
                QString xAttrUnitName = dataPairObject.value("XAttrUnitName").toString();

                int32_t yEntityID = dataPairObject.value("YEntityID").toInt();
                QString yAttrName = dataPairObject.value("YAttrName").toString();
                QString yAttrUnitName = dataPairObject.value("YAttrUnitName").toString();

                auto dataPair = plot->addPlotDataPair(
                    xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
                dataPair->setUuid(uuid);
                dataPair->setDraw(visible);
            }
        }
    }
}

void PlotXYDemo::saveDataPairToJson(DataPair* dataPair, QJsonObject& object)
{
    object.insert("UUID", dataPair->getUuid());
    object.insert("Visible", dataPair->isDraw());
    object.insert("XEntityID", dataPair->getEntityIDX());
    object.insert("YEntityID", dataPair->getEntityIDY());
    object.insert("XAttrName", dataPair->getAttr_x());
    object.insert("YAttrName", dataPair->getAttr_y());
    object.insert("XAttrUnitName", dataPair->getUnit_x());
    object.insert("YAttrUnitName", dataPair->getUnit_y());
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

void PlotXYDemo::onRealTime() {}

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

void PlotXYDemo::onOpenNetwork() {}

void PlotXYDemo::onExportDataStore()
{
    QString pxyFileName = QFileDialog::getSaveFileName(nullptr, "保存", ".", "PXY (*.pxy)");
    savePXYData(pxyFileName);
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

void PlotXYDemo::onQuit() {}

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
