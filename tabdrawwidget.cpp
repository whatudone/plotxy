#include "tabdrawwidget.h"
#include "PlotItemBase.h"
#include "choose_plot_type_dialog.h"
#include "constdef.h"

#include <QApplication>
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>

PlotItemBase* TabDrawWidget::m_pCurWidget = nullptr;
QList<PlotItemBase*> TabDrawWidget::m_curSelectedPlots = {};
MouseMode TabDrawWidget::m_mouseMode = MouseMode::SelectPlot;

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
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            m_measureLine.setPoints(m_originPoint, m_originPoint);
        }
        else if(m_mouseMode == MouseMode::MovePlot)
        {
            // 如果此时是CTRL组合键，那么肯定是多选功能
            if(event->modifiers() & Qt::ControlModifier)
            {
                m_movePlotMode = MultipleSelect;
            }
            else
            {
                //根据点击的位置区分是缩放、移动、单选行为

                if(auto plot = findPlotByMousePos(mapToGlobal(event->pos())))
                {
                    auto plotPoint = plot->mapFromParent(event->pos());
                    // 需要将坐标转化到plot中
                    if(plot->isContainedInResizeRects(plotPoint))
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
        if((m_mouseMode == MouseMode::BoxZoom) || (m_mouseMode == MouseMode::CreatePlot))
        {
            m_pRubberBand->setGeometry(QRect(m_originPoint, event->pos()).normalized());
        }
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            m_measureLine.setP2(event->pos());
            update();
        }
        else if(m_mouseMode == MouseMode::Zoom)
        {
            /*
             * 实时等比缩放图表的大小
             * 向下表示缩小，此时delta为负值，facor范围为0<factor<1.0
             * 向上表示放大，此时delta为正值，facor范围为1.0<factor<正无穷大
            */
            constexpr double step = 100.0;
            auto delta = m_originPoint.y() - event->pos().y();
            double factor = 1.0;
            if(delta > 0)
            {
                factor = delta / step + 1;
            }
            else
            {
                factor = 1 - std::abs(delta) / step;
            }
            handleZoomInOut(factor);
            m_originPoint = event->pos();
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
        else if((m_mouseMode == MouseMode::Pan))
        {
            QPoint point = event->pos();
            //根据当前鼠标位置,计算XY轴移动了多少
            int offsetX = point.x() - m_originPoint.x();
            int offsetY = point.y() - m_originPoint.y();
            handleMouseMoveWithPan(offsetX, offsetY);
            m_originPoint = event->pos();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TabDrawWidget::mouseReleaseEvent(QMouseEvent* event)
{

    if(event->button() == Qt::LeftButton)
    {
        if((m_mouseMode == MouseMode::BoxZoom))
        {
            m_pRubberBand->hide();
            handleBoxZoom(m_pRubberBand->geometry());
        }
        else if((m_mouseMode == MouseMode::CreatePlot))
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
        else if((m_mouseMode == MouseMode::MeasureDistance))
        {
            // 重置到无效状态
            m_measureLine.setPoints(QPoint(), QPoint());
            update();
        }
        else if((m_mouseMode == MouseMode::CenterPlot))
        {
            handleMouseReleaseWithCenterPlot(event->pos());
        }
        else if((m_mouseMode == MouseMode::SelectPlot))
        {
            if(auto plot = findPlotByMousePos(event->pos()))
            {
                emit selectedPlotChanged(plot);
                m_curSelectedPlots.clear();
                m_curSelectedPlots.append(plot);
            }
        }
        else if((m_mouseMode == MouseMode::MovePlot))
        {
            // 通过组合键区分是否为多选行为
            auto plot = findPlotByMousePos(event->pos());
            if(plot)
            {
                emit selectedPlotChanged(plot);
                if(event->modifiers() & Qt::ControlModifier)
                {
                    m_curSelectedPlots.append(plot);
                }
                else
                {
                    m_curSelectedPlots.clear();
                    m_curSelectedPlots.append(plot);
                }
            }
        }
        QWidget::mouseReleaseEvent(event);
    }
}

void TabDrawWidget::paintEvent(QPaintEvent* event)
{
    if(m_mouseMode == MouseMode::MeasureDistance)
    {
        if(!m_measureLine.isNull())
        {
            drawMeasureLines();
        }
    }

    QWidget::paintEvent(event);
}

PlotItemBase* TabDrawWidget::findPlotByMousePos(const QPoint& point)
{
    // 获取最上层的控件
    return dynamic_cast<PlotItemBase*>(qApp->widgetAt(point));
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
        for(auto plot : m_curSelectedPlots)
        {
            plot->updateGeoWithMouseMove(offsetX, offsetY);
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
    // 切换光标类型
    if(m_mouseMode == MouseMode::MovePlot)
    {
        for(auto plot : m_curSelectedPlots)
        {
            plot->setIsNeedDrawBorder(true);
            plot->update();
        }
    }
    else
    {
        for(auto plot : m_curSelectedPlots)
        {
            plot->setIsNeedDrawBorder(false);
            plot->update();
        }
    }
}

/*
 * 按照centerPoint的坐标居中，此坐标为父窗体坐标系
*/
void TabDrawWidget::handleMouseReleaseWithCenterPlot(const QPoint& centerPoint)
{
    for(auto plot : m_curSelectedPlots)
    {
        int width = plot->width();
        int height = plot->height();
        plot->move(centerPoint.x() - width / 2, centerPoint.y() - height / 2);
    }
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

void TabDrawWidget::drawMeasureLines()
{
    constexpr int textRectSize = 30;
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    QPen pen;
    pen.setColor(Qt::red);
    pen.setWidth(4);
    pen.setStyle(Qt::SolidLine);
    painter.setPen(pen);
    painter.drawLine(m_measureLine);
    pen.setStyle(Qt::DashLine);
    painter.setPen(pen);
    int xDistance = m_measureLine.x2() - m_originPoint.x();
    int yDistance = m_measureLine.y2() - m_originPoint.y();
    QLine xLine(m_originPoint, QPoint(m_measureLine.p2().x(), m_originPoint.y()));
    painter.drawLine(xLine);
    auto xHeightOffset = (m_measureLine.y1() > m_measureLine.y2()) ? textRectSize : -textRectSize;
    QRect xTextRect = QRect(m_originPoint, QSize(xDistance, xHeightOffset));
    painter.drawText(xTextRect, Qt::AlignCenter, QString::number(std::abs(xDistance)));
    QLine yLine(m_originPoint, QPoint(m_originPoint.x(), m_measureLine.p2().y()));
    painter.drawLine(yLine);
    auto yWidthOffset = (m_measureLine.x1() < m_measureLine.x2()) ? -textRectSize : 0;
    auto yHeightOffset = (m_measureLine.y1() > m_measureLine.y2()) ? yDistance : 0;
    auto yWidthSizeOffset = (m_measureLine.y1() > m_measureLine.y2()) ? -yDistance : yDistance;
    QRect yTextRect =
        QRect(QPoint(m_originPoint.x() + yWidthOffset, m_originPoint.y() + yHeightOffset),
              QSize(textRectSize, yWidthSizeOffset));
    painter.drawText(yTextRect, Qt::AlignCenter, QString::number(std::abs(yDistance)));
}

QList<PlotItemBase*> TabDrawWidget::getCurSelectedPlots()
{
    return m_curSelectedPlots;
}

void TabDrawWidget::setCurSelectedPlots(const QList<PlotItemBase*>& curSelectedPlots)
{
    m_curSelectedPlots = curSelectedPlots;
}

QCursor TabDrawWidget::getCurCursor() const
{
    return m_curCursor;
}

void TabDrawWidget::setCurCursor(const QCursor& curCursor)
{
    m_curCursor = curCursor;
}
