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
        // 所有的模式下点击鼠标都会切换当当前选中图表
        m_pCurWidget = findPlotByMousePos(event->pos());
        if(m_pCurWidget)
        {
            m_curSelectedPlots.clear();
            m_curSelectedPlots.append(m_pCurWidget);
            m_pCurWidget->clearInter();
            emit selectedPlotChanged(m_curSelectedPlots);
        }
        else
        {
            return QWidget::mousePressEvent(event);
        }
        if(m_mouseMode == MouseMode::CreatePlot)
        {
            m_pRubberBand->setGeometry(QRect(m_originPoint, QSize()));
            m_pRubberBand->show();
        }
        else if(m_mouseMode == MouseMode::Pan)
        {
            m_pCurWidget->setCustomPlotMouseTransparent(false);
            m_pCurWidget->setInteract(QCP::iRangeDrag);
        }
        else if(m_mouseMode == MouseMode::CenterPlot)
        {
            m_pCurWidget->setCustomPlotMouseTransparent(true);
        }
        else if(m_mouseMode == MouseMode::Zoom)
        {
            m_pCurWidget->setCustomPlotMouseTransparent(false);
            m_pCurWidget->setZoom(1);
        }
        else if(m_mouseMode == MouseMode::BoxZoom)
        {
            m_pCurWidget->setCustomPlotMouseTransparent(false);
            m_pCurWidget->setZoom(2);
        }
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            m_pCurWidget->setMeasureTransparent();

            auto customPlot = m_pCurWidget->getCustomPlot();
            if(!m_measureLineItem)
            {
                m_measureLineItem = new QCPItemLine(customPlot);
                QPen linePen(Qt::red);
                linePen.setWidth(2);
                m_measureLineItem->setPen(linePen);
            }

            if(!m_measureTextItem)
            {
                m_measureTextItem = new QCPItemText(customPlot);
                m_measureTextItem->setPositionAlignment(Qt::AlignCenter | Qt::AlignTop);
                m_measureTextItem->setColor(Qt::white);
            }

            auto tmpPoint = m_pCurWidget->mapFromParent(event->pos());
            m_originPoint = customPlot->mapFromParent(tmpPoint);

            if(customPlot && m_measureLineItem && m_measureTextItem)
            {
                m_measureLineItem->start->setCoords(
                    QPointF(customPlot->xAxis->pixelToCoord(m_originPoint.x()),
                            customPlot->yAxis->pixelToCoord(m_originPoint.y())));

                m_measureLineItem->end->setCoords(
                    QPointF(customPlot->xAxis->pixelToCoord(m_originPoint.x()),
                            customPlot->yAxis->pixelToCoord(m_originPoint.y())));
                m_measureLineItem->setVisible(true);
                m_measureTextItem->setText(QString());
                m_measureTextItem->setVisible(true);
                customPlot->replot();
            }
        }
        else if(m_mouseMode == MouseMode::MovePlot)
        {
            // 如果此时是CTRL组合键，那么肯定是多选功能
            if(event->modifiers() & Qt::ControlModifier)
            {
                m_movePlotMode = MultipleSelect;
                m_pCurWidget = findPlotByMousePos(event->pos());
                m_curSelectedPlots.append(m_pCurWidget);
            }
            else
            {
                //根据点击的位置区分是缩放、移动、单选行为

                if(m_pCurWidget)
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
                    m_curSelectedPlots.clear();
                    m_curSelectedPlots.append(m_pCurWidget);
                }
                else
                {
                    m_movePlotMode = Move;
                }

                emit selectedPlotChanged(m_curSelectedPlots);
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
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            if(event->buttons() & Qt::LeftButton)
            {
                auto customPlot = m_pCurWidget->getCustomPlot();
                auto tmpPoint = m_pCurWidget->mapFromParent(event->pos());
                auto point = customPlot->mapFromParent(tmpPoint);
                double dx = 0.0;
                double dy = 0.0;
                double l = 0.0;

                if(customPlot && m_measureLineItem && m_measureTextItem)
                {
                    m_measureLineItem->end->setCoords(
                        QPointF(customPlot->xAxis->pixelToCoord(point.x()),
                                customPlot->yAxis->pixelToCoord(point.y())));

                    m_measureTextItem->position->setCoords(QPointF(
                        customPlot->xAxis->pixelToCoord((point.x() + m_originPoint.x()) / 2),
                        customPlot->yAxis->pixelToCoord((point.y() + m_originPoint.y()) / 2)));

                    dx = customPlot->xAxis->pixelToCoord(point.x()) -
                         customPlot->xAxis->pixelToCoord(m_originPoint.x());
                    dy = customPlot->yAxis->pixelToCoord(point.y()) -
                         customPlot->yAxis->pixelToCoord(m_originPoint.y());
                    l = sqrt(pow(dx, 2) + pow(dy, 2));
                    m_measureTextItem->setText(
                        QString("dx = %1\ndy = %2\nl = %3").arg(dx).arg(dy).arg(l));
                    customPlot->replot();
                }
            }
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TabDrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(m_mouseMode == MouseMode::CreatePlot)
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
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            // 重置到无效状态
            if(m_pCurWidget->getCustomPlot() && m_measureLineItem && m_measureTextItem)
            {
                m_measureLineItem->setVisible(false);
                m_measureTextItem->setVisible(false);
                m_pCurWidget->getCustomPlot()->replot();
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
    // qApp->widgetAt可能受到Qt::WA_TransparentForMouseEvents的影响，获取能接收信号的最上层的QWidget
    // 对PlotItemBase和customplot设置鼠标事件穿透之后，TabDrawWidget就是最上层的窗口
    TabDrawWidget* plotList =
        dynamic_cast<TabDrawWidget*>(qApp->widgetAt(mapToGlobal(mouseEventPoint)));
    if(!plotList->m_curSelectedPlots.isEmpty())
    {
        return plotList->m_curSelectedPlots.at(0);
    }
    return nullptr;
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
