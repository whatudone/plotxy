#include "tabdrawwidget.h"
#include "constdef.h"

#include <QMouseEvent>
#include <QPainter>
#include <QRubberBand>
TabDrawWidget::TabDrawWidget(QWidget* parent)
    : QWidget(parent)
    , m_pRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{}

void TabDrawWidget::mousePressEvent(QMouseEvent* event)
{
    if((event->button() == Qt::LeftButton))
    {
        if(m_mouseMode == MouseMode::BoxZoom)
        {
            m_originPoint = event->pos();
            m_pRubberBand->setGeometry(QRect(m_originPoint, QSize()));
            m_pRubberBand->show();
        }
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {
            m_originPoint = event->pos();
            m_measureLine.setPoints(m_originPoint, m_originPoint);
        }
    }
    QWidget::mousePressEvent(event);
}

void TabDrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if(m_mouseMode == MouseMode::BoxZoom)
        {
            m_pRubberBand->setGeometry(QRect(m_originPoint, event->pos()).normalized());
        }
        else if(m_mouseMode == MouseMode::MeasureDistance)
        {

            m_measureLine.setP2(event->pos());
            update();
        }
    }
    QWidget::mouseMoveEvent(event);
}

void TabDrawWidget::paintEvent(QPaintEvent* event)
{
    if(m_mouseMode == MouseMode::MeasureDistance)
    {
        if(!m_measureLine.isNull())
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
            auto xHeightOffset =
                (m_measureLine.y1() > m_measureLine.y2()) ? textRectSize : -textRectSize;
            QRect xTextRect = QRect(m_originPoint, QSize(xDistance, xHeightOffset));
            painter.drawText(xTextRect, Qt::AlignCenter, QString::number(std::abs(xDistance)));
            QLine yLine(m_originPoint, QPoint(m_originPoint.x(), m_measureLine.p2().y()));
            painter.drawLine(yLine);
            auto yWidthOffset = (m_measureLine.x1() < m_measureLine.x2()) ? -textRectSize : 0;
            auto yHeightOffset = (m_measureLine.y1() > m_measureLine.y2()) ? yDistance : 0;
            auto yWidthSizeOffset =
                (m_measureLine.y1() > m_measureLine.y2()) ? -yDistance : yDistance;
            QRect yTextRect =
                QRect(QPoint(m_originPoint.x() + yWidthOffset, m_originPoint.y() + yHeightOffset),
                      QSize(textRectSize, yWidthSizeOffset));
            painter.drawText(yTextRect, Qt::AlignCenter, QString::number(std::abs(yDistance)));
        }
    }
    QWidget::paintEvent(event);
}

void TabDrawWidget::onMouseModeChanged(MouseMode mode)
{
    m_mouseMode = mode;
}

MouseMode TabDrawWidget::mouseMode() const
{
    return m_mouseMode;
}

void TabDrawWidget::setMouseMode(const MouseMode& mouseMode)
{
    m_mouseMode = mouseMode;
}

void TabDrawWidget::mouseReleaseEvent(QMouseEvent* event)
{

    if(event->button() == Qt::LeftButton)
    {
        if((m_mouseMode == MouseMode::BoxZoom))
        {
            m_pRubberBand->hide();
            emit boxZoomed(m_pRubberBand->geometry());
        }
        else if((m_mouseMode == MouseMode::MeasureDistance))
        {
            // 重置到无效状态
            m_measureLine.setPoints(QPoint(), QPoint());
            update();
        }
        else if((m_mouseMode == MouseMode::CenterPlot))
        {
            emit mouseRelease(event->pos());
        }
    }
    QWidget::mouseReleaseEvent(event);
}
