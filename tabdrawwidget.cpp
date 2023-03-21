#include "tabdrawwidget.h"
#include <QMouseEvent>

TabDrawWidget::TabDrawWidget(QWidget* parent)
    : QWidget(parent)
{}

void TabDrawWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if(event->button() == Qt::LeftButton)
        emit mouseRelease(event->pos());
    return QWidget::mouseReleaseEvent(event);
}
