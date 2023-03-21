#include "tabdrawwidget.h"
#include "constdef.h"

#include <QMouseEvent>
#include <QRubberBand>
TabDrawWidget::TabDrawWidget(QWidget* parent)
    : QWidget(parent)
    , m_pRubberBand(new QRubberBand(QRubberBand::Rectangle, this))
{}

void TabDrawWidget::mousePressEvent(QMouseEvent* event)
{
    if((m_mouseMode == MouseMode::BoxZoom) && (event->button() == Qt::LeftButton))
    {
        m_originPoint = event->pos();
        m_pRubberBand->setGeometry(QRect(m_originPoint, QSize()));
        m_pRubberBand->show();
    }
    QWidget::mousePressEvent(event);
}

void TabDrawWidget::mouseMoveEvent(QMouseEvent* event)
{
    if((m_mouseMode == MouseMode::BoxZoom) && (event->buttons() & Qt::LeftButton))
    {
        m_pRubberBand->setGeometry(QRect(m_originPoint, event->pos()).normalized());
    }
    QWidget::mouseMoveEvent(event);
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
        else if((m_mouseMode == MouseMode::CenterPlot))
        {
            emit mouseRelease(event->pos());
        }
    }
    QWidget::mouseReleaseEvent(event);
}
