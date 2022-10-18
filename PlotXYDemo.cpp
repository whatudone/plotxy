#include <QtWidgets/QPushButton>
#include <QtWidgets/QCheckBox>
#include <QFileDialog>

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


PlotXYDemo::PlotXYDemo(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    //m_plotItem = nullptr;
    //m_freeWidgetWraper = nullptr;
    m_plotManager = new PlotManager();
    m_addPlotPair = NULL;
    m_AdvancedDataManager = new AdvancedDataManager();

    setMinimumSize(1600, 1200);
    showMaximized();

    //statusbar设置左侧信息提示
    QLabel* info = new QLabel;
    info->setText(QString::fromLocal8Bit("已选择的图层："));
    ui.statusBar->addWidget(info);

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
}

PlotXYDemo::~PlotXYDemo()
{

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
        m_addPlotPair = new AddPlotPair();
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_plotManager, SLOT(onAddPlotPair(QString, QString)));
        connect(m_addPlotPair, SIGNAL(sigAddPlotPair(QString, QString)), 
            m_AdvancedDataManager, SLOT(onAdvancedDataManagerAddPair(QString, QString)));
		connect(this, SIGNAL(sgn_loadDataReady()), m_addPlotPair, SLOT(onUpdateData()));
        m_addPlotPair->init(getCurrentFocusPlot());
    }
	m_addPlotPair->onChangeStackIndex(getCurrentFocusPlot());
    m_addPlotPair->show();
}

void PlotXYDemo::onOpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, "open File", "/home", tr("Microsoft CSV(*.csv)"));
    DataManager::getInstance()->loadCSV(path);

	sgn_loadDataReady();
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
	/* 添加菜单项 */
	createPlotMenu->addAction(addBarPlot);
	createPlotMenu->addAction(addAttitudePlot);
	createPlotMenu->addAction(addTextPlot);
	createPlotMenu->addAction(addPolarPlot);
	/* 连接槽函数 */
	connect(addBarPlot, SIGNAL(triggered()), this, SLOT(onAddBarPlot()));
	connect(addAttitudePlot, SIGNAL(triggered()), this, SLOT(onAddAttitudePlot()));
	connect(addTextPlot, SIGNAL(triggered()), this, SLOT(onAddTextPlot()));
	connect(addPolarPlot, SIGNAL(triggered()), this, SLOT(onAddPolarPlot()));

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

    m_plotManager->addPlot(currTabText, plotItem); //tab页可能变更，存在bug
}
void PlotXYDemo::onAddTextPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotText* plotItem = new PlotText(ui.tabWidget->currentWidget());
	//bool res = connect(ui.actionStop,SIGNAL(triggered(bool)), plotItem, SLOT(onSwitch(bool)));
	connect(ui.actionStop, &QAction::triggered, plotItem, &PlotText::onSwitch);
	connect(m_AdvancedDataManager, &AdvancedDataManager::updateColorThresholdMap,
		plotItem, &PlotText::onUpdateColorThresholdMap);
	// 好像是数据处理有关的东西

	initWidget(plotItem);

	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();

	m_plotManager->addPlot(currTabText, plotItem); //tab页可能变更，存在bug
}

void PlotXYDemo::onAddPolarPlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotPolar* plotItem = new PlotPolar(ui.tabWidget->currentWidget());
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

	m_plotManager->addPlot(currTabText, plotItem); 

}

void PlotXYDemo::onFocusChanged(QWidget * oldWidget, QWidget * newWidget)
{
	m_nowFocusWidget = newWidget;
}

void PlotXYDemo::onAddAttitudePlot()
{
	int currTabIndex = ui.tabWidget->currentIndex();
	QString currTabText = ui.tabWidget->tabText(currTabIndex);

	PlotAttitude* plotItem = new PlotAttitude(ui.tabWidget->currentWidget());
	initWidget(plotItem);

	// 控制其自由移动和缩放
	FreeWidgetWraper* m_freeWidgetWraper = new FreeWidgetWraper();
	m_freeWidgetWraper->setWidget(plotItem);

	m_freeWidgetWraper->setMoveEnable(true);
	m_freeWidgetWraper->setMoveEnable(true);

	plotItem->show();
	plotItem->update();
	m_plotManager->addPlot(currTabText, plotItem); //tab页可能变更，存在bug
}

void PlotXYDemo::init()
{

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
	if (name.compare("PlotPlotScatter") == 0)
	{
		return PlotType::Type_PlotScatter;
	}
	else if (name.compare("PlotAScope") == 0)
	{
		return PlotType::Type_PlotAScope;
	} 
	else if (name.compare("PlotRTI") == 0)
	{
		return PlotType::Type_PlotRTI;
	}
	else if (name.compare("PlotText") == 0)
	{
		return PlotType::Type_PlotText;
	}
	else if (name.compare("PlotLight") == 0)
	{
		return PlotType::Type_PlotLight;
	}
	else if (name.compare("PlotBar") == 0)
	{
		return PlotType::Type_PlotBar;
	}
	else if (name.compare("PlotDial") == 0)
	{
		return PlotType::Type_PlotDial;
	}
	else if (name.compare("PlotAttitude") == 0)
	{
		return PlotType::Type_PlotAttitude;
	}
	else if (name.compare("PlotPolar") == 0)
	{
		return PlotType::Type_PlotPolar;
	}
	else if (name.compare("PlotTrack") == 0)
	{
		return PlotType::Type_PlotTrack;
	}
	else if (name.compare("PlotDoppler") == 0)
	{
		return PlotType::Type_PlotDoppler;
	}
	else
		return PlotType::Type_PlotScatter;
}


