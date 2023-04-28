#include "tabdrawwidget.h"
#include "PlotItemBase.h"
#include "choose_plot_type_dialog.h"
#include "constdef.h"

#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

PlotItemBase* TabDrawWidget::m_pCurWidget = nullptr;
QList<PlotItemBase*> TabDrawWidget::m_curSelectedPlots = {};
MouseMode TabDrawWidget::m_mouseMode = MouseMode::SelectPlot;
bool TabDrawWidget::m_isLockingEdit = false;
bool TabDrawWidget::m_isLockingStack = false;

TabDrawWidget::TabDrawWidget(QWidget* parent)
    : QWidget(parent)
    , m_pRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{
    setMouseTracking(true);
}

void TabDrawWidget::mousePressEvent(QMouseEvent* event)
{
    if((event->button() == Qt::LeftButton))
    {
        m_originPoint = event->pos();
        if((m_mouseMode == MouseMode::BoxZoom) || (m_mouseMode == MouseMode::CreatePlot))
        {
            m_pRubberBand->setGeometry(QRect(m_originPoint, QSize()));
            m_pRubberBand->show();
        }
        else if(m_mouseMode == MouseMode::MovePlot)
        {
            // 如果此时是CTRL组合键，那么肯定是多选功能
            if(event->modifiers() & Qt::ControlModifier)
            {
                m_movePlotMode = MultipleSelect;
                m_pCurWidget = findPlotByMousePos(event->pos());
            }
            else
            {
                //根据点击的位置区分是缩放、移动、单选行为

                if(m_pCurWidget = findPlotByMousePos(event->pos()))
                {
                    auto plotPoint = m_pCurWidget->mapFromParent(event->pos());
                    // 需要将坐标转化到plot中
                    if(m_pCurWidget->isContainedInResizeRects(plotPoint))
                    {
                        m_movePlotMode = Resize;
                    }
                    else
                    {
                        m_movePlotMode = SingleSelect;
                    }
                }
                else
                {
                    m_movePlotMode = Move;
                }
            }
        }
    }
    QWidget::mousePressEvent(event);
}

void TabDrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(m_mouseMode == MouseMode::CreatePlot)
        {
            m_pRubberBand->setGeometry(QRect(m_originPoint, event->pos()).normalized());
        }
        else if(m_mouseMode == MouseMode::MovePlot)
        {
            QPoint point = event->pos();
            //根据当前鼠标位置,计算XY轴移动了多少
            int offsetX = point.x() - m_originPoint.x();
            int offsetY = point.y() - m_originPoint.y();
            //本模式有两种行为，一种是移动，一种是缩放
            handleMouseMoveWithMovePlot(offsetX, offsetY);
            m_originPoint = event->pos();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TabDrawWidget::mouseReleaseEvent(QMouseEvent* event)
{

    if(event->button() == Qt::LeftButton)
    {
        // 所有的模式下点击鼠标都会切换当当前选中图表
        if(auto plot = findPlotByMousePos(event->pos()))
        {
            m_curSelectedPlots.clear();
            m_curSelectedPlots.append(plot);
            emit selectedPlotChanged(m_curSelectedPlots);
        }
        else if(m_mouseMode == MouseMode::CreatePlot)
        {
            // 避免点击事件直接触发CreatePlot逻辑，需要判断橡皮筋的大小
            m_pRubberBand->hide();
            if((m_pRubberBand->size().width() < 100) || (m_pRubberBand->size().height() < 100))
                return;
            // 选择图表类型
            ChoosePlotTypeDialog dialog;
            int ret = dialog.exec();
            if(ret)
            {
                emit createPlot(dialog.getPlotType(), m_pRubberBand->geometry());
            }
        }
        else if(m_mouseMode == MouseMode::CenterPlot)
        {
            handleMouseReleaseWithCenterPlot(event->pos());
        }
        else if(m_mouseMode == MouseMode::SelectPlot)
        {
            handleMouseReleaseWithSelectPlot(event->pos());
        }
        else if(m_mouseMode == MouseMode::MovePlot)
        {
            // 通过组合键区分是否为多选行为
            // m_pCurWidget为mousePress中找到的当前点击的图表，不需要再重新查找一次，该api耗时较长
            if(m_pCurWidget)
            {
                if(m_movePlotMode == MultipleSelect)
                {
                    m_curSelectedPlots.append(m_pCurWidget);
                }
                else if(m_movePlotMode == SingleSelect)
                {
                    m_curSelectedPlots.clear();
                    m_curSelectedPlots.append(m_pCurWidget);
                }
                emit selectedPlotChanged(m_curSelectedPlots);
            }
        }
        QWidget::mouseReleaseEvent(event);
    }
}

void TabDrawWidget::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);
}

PlotItemBase* TabDrawWidget::findPlotByMousePos(const QPoint& mouseEventPoint)
{
    // 获取最上层的控件,需要将坐标转化为全局坐标
    return dynamic_cast<PlotItemBase*>(qApp->widgetAt(mapToGlobal(mouseEventPoint)));
}

void TabDrawWidget::handleMouseMoveWithMovePlot(int offsetX, int offsetY)
{
    if((m_movePlotMode == Move) || (m_movePlotMode == SingleSelect))
    {
        for(auto plot : m_curSelectedPlots)
        {
            plot->move(plot->x() + offsetX, plot->y() + offsetY);
        }
    }
    else if(m_movePlotMode == Resize)
    {
        if(m_pCurWidget)
        {
            m_pCurWidget->updateGeoWithMouseMove(offsetX, offsetY);
        }
    }
}

MouseMode TabDrawWidget::mouseMode()
{
    return m_mouseMode;
}

void TabDrawWidget::setMouseMode(const MouseMode& mouseMode)
{
    m_mouseMode = mouseMode;
    if(m_mouseMode == MouseMode::MovePlot)
    {
        updateSelectedPlotsBorderVisible(true);
    }
    else
    {
        updateSelectedPlotsBorderVisible(false);
    }
}

/*
 * 按照centerPoint的坐标居中，此坐标为父窗体坐标系
*/
void TabDrawWidget::handleMouseReleaseWithCenterPlot(const QPoint& centerPoint)
{
    for(auto plot : m_curSelectedPlots)
    {
        auto point = plot->mapFromParent(centerPoint);
        plot->setNewTickOrigin(point);
    }
}
/*
 * 获取所有当前鼠标点击坐标下的图表，形成一个菜单显示出来，
 * 选中的图表会移动到最上层显示
*/
void TabDrawWidget::handleMouseReleaseWithSelectPlot(const QPoint& mouseEventPoint)
{
    auto allPlotList = findAllPlots();
    QList<PlotItemBase*> selectedPlotList;
    foreach(auto plot, allPlotList)
    {
        // 判断是否plot在鼠标坐标下，需要把tab上的坐标转化到plot中
        QPoint localPos = plot->mapFromParent(mouseEventPoint);
        auto rect = plot->rect();
        if(rect.contains(localPos))
        {
            selectedPlotList.append(plot);
        }
    }
    // 超过两个才需要将菜单显示出来供用户选择
    if(selectedPlotList.size() <= 1)
    {
        return;
    }

    QMenu menu;
    foreach(auto plot, selectedPlotList)
    {
        menu.addAction(plot->getName(), [plot]() { plot->raise(); });
    }
    // 适配多屏坐标
    menu.exec(QCursor::pos());
}

void TabDrawWidget::handleBoxZoom(const QRect& rect)
{
    for(auto plot : m_curSelectedPlots)
    {
        plot->setGeometry(rect);
    }
}

void TabDrawWidget::handleZoomInOut(double factor)
{
    for(auto plot : m_curSelectedPlots)
    {
        // 以原始矩形的中心点重新计算缩放后的矩形
        auto originRect = plot->geometry();
        int newWidth = static_cast<int>(originRect.width() * factor);
        int newHeight = static_cast<int>(originRect.height() * factor);
        // 当缩放到最小尺寸后，停止缩小
        if((newWidth <= plot->minimumWidth()) || (newHeight <= plot->minimumHeight()))
        {
            continue;
        }
        auto centerPoint = originRect.center();
        int newX = centerPoint.x() - newWidth / 2;
        int newY = centerPoint.y() - newHeight / 2;
        plot->setGeometry(QRect(QPoint(newX, newY), QSize(newWidth, newHeight)));
    }
}

void TabDrawWidget::handleMouseMoveWithPan(int offsetX, int offsetY)
{
    for(auto plot : m_curSelectedPlots)
    {
        plot->move(plot->x() + offsetX, plot->y() + offsetY);
    }
}

void TabDrawWidget::updateSelectedPlotsBorderVisible(bool visible)
{
    for(auto plot : m_curSelectedPlots)
    {
        plot->setIsNeedDrawBorder(visible);
        plot->update();
    }
}

bool TabDrawWidget::getIsLockingStack()
{
    return m_isLockingStack;
}

void TabDrawWidget::setIsLockingStack(bool isLockingStack)
{
    m_isLockingStack = isLockingStack;
}

bool TabDrawWidget::getIsLockingEdit()
{
    return m_isLockingEdit;
}

void TabDrawWidget::setIsLockingEdit(bool isLockingEdit)
{
    m_isLockingEdit = isLockingEdit;
}

QList<PlotItemBase*> TabDrawWidget::getCurSelectedPlots()
{
    return m_curSelectedPlots;
}

void TabDrawWidget::setCurSelectedPlots(const QList<PlotItemBase*>& curSelectedPlots)
{
    m_curSelectedPlots = curSelectedPlots;
}

void TabDrawWidget::bringToTop()
{
    if(m_curSelectedPlots.size() == 1)
    {
        auto plot = m_curSelectedPlots.at(0);
        plot->raise();
    }
}

void TabDrawWidget::sendToBottom()
{
    if(m_curSelectedPlots.size() == 1)
    {
        auto plot = m_curSelectedPlots.at(0);
        plot->lower();
    }
}

void TabDrawWidget::bringForward() {}

void TabDrawWidget::sendBackward() {}

void TabDrawWidget::horizonAlign()
{
    auto plots = findAllPlots();
    auto size = plots.size();
    QPoint startPoint(0, 0);
    for(int var = 0; var < size; ++var)
    {
        plots.at(var)->setGeometry(QRect(startPoint, m_tileSize));
        startPoint += QPoint(m_tileSize.width(), 0);
    }
}

void TabDrawWidget::verticalAlign()
{
    auto plots = findAllPlots();
    auto size = plots.size();
    QPoint startPoint(0, 0);
    for(int var = 0; var < size; ++var)
    {
        plots.at(var)->setGeometry(QRect(startPoint, m_tileSize));
        startPoint += QPoint(0, m_tileSize.height());
    }
}

QList<PlotItemBase*> TabDrawWidget::findAllPlots()
{
    return findChildren<PlotItemBase*>(QString(), Qt::FindDirectChildrenOnly);
}

QCursor TabDrawWidget::getCurCursor() const
{
    return m_curCursor;
}

void TabDrawWidget::setCurCursor(const QCursor& curCursor)
{
    m_curCursor = curCursor;
}
