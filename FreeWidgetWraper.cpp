﻿#include "FreeWidgetWraper.h"
#include "PlotItemBase.h"

#include <QDebug>
#include <QEvent>
static const QMap<int, Qt::CursorShape> CursorMap{
    {0, Qt::SizeHorCursor},
    {1, Qt::SizeHorCursor},
    {2, Qt::SizeVerCursor},
    {3, Qt::SizeVerCursor},
    {4, Qt::SizeFDiagCursor},
    {5, Qt::SizeBDiagCursor},
    {6, Qt::SizeBDiagCursor},
    {7, Qt::SizeFDiagCursor},
};
FreeWidgetWraper::FreeWidgetWraper(QObject* parent)
    : QObject(parent)
{
    padding = 8; //default
    moveEnable = true;
    resizeEnable = true;

    mousePressed = false;
    mousePoint = QPoint(0, 0);
    mouseRect = QRect(0, 0, 0, 0);

    for(int i = 0; i < 8; ++i)
    {
        pressedArea << false;
        pressedRect << QRect(0, 0, 0, 0);
    }
}

FreeWidgetWraper::~FreeWidgetWraper() {}

bool FreeWidgetWraper::eventFilter(QObject* watched, QEvent* event)
{
    if(!m_pBindWidget || watched != m_pBindWidget)
    {
        return QObject::eventFilter(watched, event);
    }

    if(event->type() == QEvent::Resize)
    {
        //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
        int width = m_pBindWidget->width();
        int height = m_pBindWidget->height();

        //左侧描点区域
        pressedRect[0] = QRect(0, padding, padding, height - padding * 2);
        //右侧描点区域
        pressedRect[1] = QRect(width - padding, padding, padding, height - padding * 2);
        //上侧描点区域
        pressedRect[2] = QRect(padding, 0, width - padding * 2, padding);
        //下侧描点区域
        pressedRect[3] = QRect(padding, height - padding, width - padding * 2, padding);

        //左上角描点区域
        pressedRect[4] = QRect(0, 0, padding, padding);
        //右上角描点区域
        pressedRect[5] = QRect(width - padding, 0, padding, padding);
        //左下角描点区域
        pressedRect[6] = QRect(0, height - padding, padding, padding);
        //右下角描点区域
        pressedRect[7] = QRect(width - padding, height - padding, padding, padding);
    }
    else if(event->type() == QEvent::HoverMove)
    {
        //设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
        QHoverEvent* hoverEvent = static_cast<QHoverEvent*>(event);
        QPoint point = hoverEvent->pos();
        if(resizeEnable)
        {
            bool isInCorner = false;
            for(int index = 0; index < pressedRect.size(); ++index)
            {
                if(pressedRect.at(index).contains(point))
                {
                    m_pBindWidget->setCursor(CursorMap.value(index));
                    isInCorner = true;
                    break;
                }
            }
            if(!isInCorner)
            {
                m_pBindWidget->setCursor(Qt::ArrowCursor);
            }
        }

        //根据当前鼠标位置,计算XY轴移动了多少
        int offsetX = point.x() - mousePoint.x();
        int offsetY = point.y() - mousePoint.y();

        //根据按下处的位置判断是否是移动控件还是拉伸控件
        if(moveEnable && mousePressed)
        {
            m_pBindWidget->move(m_pBindWidget->x() + offsetX, m_pBindWidget->y() + offsetY);
        }

        if(resizeEnable)
        {
            int rectX = mouseRect.x();
            int rectY = mouseRect.y();
            int rectW = mouseRect.width();
            int rectH = mouseRect.height();

            if(pressedArea.at(0))
            {
                int resizeW = m_pBindWidget->width() - offsetX;
                if(m_pBindWidget->minimumWidth() <= resizeW)
                {
                    m_pBindWidget->setGeometry(m_pBindWidget->x() + offsetX, rectY, resizeW, rectH);
                }
            }
            else if(pressedArea.at(1))
            {
                m_pBindWidget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
            }
            else if(pressedArea.at(2))
            {
                int resizeH = m_pBindWidget->height() - offsetY;
                if(m_pBindWidget->minimumHeight() <= resizeH)
                {
                    m_pBindWidget->setGeometry(rectX, m_pBindWidget->y() + offsetY, rectW, resizeH);
                }
            }
            else if(pressedArea.at(3))
            {
                m_pBindWidget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
            }
            else if(pressedArea.at(4))
            {
                int resizeW = m_pBindWidget->width() - offsetX;
                int resizeH = m_pBindWidget->height() - offsetY;
                if(m_pBindWidget->minimumWidth() <= resizeW)
                {
                    m_pBindWidget->setGeometry(
                        m_pBindWidget->x() + offsetX, m_pBindWidget->y(), resizeW, resizeH);
                }
                if(m_pBindWidget->minimumHeight() <= resizeH)
                {
                    m_pBindWidget->setGeometry(
                        m_pBindWidget->x(), m_pBindWidget->y() + offsetY, resizeW, resizeH);
                }
            }
            else if(pressedArea.at(5))
            {
                int resizeW = rectW + offsetX;
                int resizeH = m_pBindWidget->height() - offsetY;
                if(m_pBindWidget->minimumHeight() <= resizeH)
                {
                    m_pBindWidget->setGeometry(
                        m_pBindWidget->x(), m_pBindWidget->y() + offsetY, resizeW, resizeH);
                }
            }
            else if(pressedArea.at(6))
            {
                int resizeW = m_pBindWidget->width() - offsetX;
                int resizeH = rectH + offsetY;
                if(m_pBindWidget->minimumWidth() <= resizeW)
                {
                    m_pBindWidget->setGeometry(
                        m_pBindWidget->x() + offsetX, m_pBindWidget->y(), resizeW, resizeH);
                }
                if(m_pBindWidget->minimumHeight() <= resizeH)
                {
                    m_pBindWidget->setGeometry(
                        m_pBindWidget->x(), m_pBindWidget->y(), resizeW, resizeH);
                }
            }
            else if(pressedArea.at(7))
            {
                int resizeW = rectW + offsetX;
                int resizeH = rectH + offsetY;
                m_pBindWidget->setGeometry(
                    m_pBindWidget->x(), m_pBindWidget->y(), resizeW, resizeH);
            }
        }
    }
    else if(event->type() == QEvent::MouseButtonPress)
    {
        //记住鼠标按下的坐标+窗体区域
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        mousePoint = mouseEvent->pos();
        mouseRect = m_pBindWidget->geometry();
        //判断按下的手柄的区域位置
        for(const auto& rect : pressedRect)
        {
            if(rect.contains(mousePoint))
            {
                int index = pressedRect.indexOf(rect);
                pressedArea[index] = true;
                return false;
            }
        }

        mousePressed = true;
    }
    else if(event->type() == QEvent::MouseMove)
    {
        //改成用HoverMove识别
    }
    else if(event->type() == QEvent::MouseButtonRelease)
    {
        //恢复所有
        m_pBindWidget->setCursor(Qt::ArrowCursor);
        mousePressed = false;
        for(int i = 0; i < 8; ++i)
        {
            pressedArea[i] = false;
        }

        if(m_pBindWidget->objectName() == "PlotItemBase")
            emit sgnMouseEventDone(m_pBindWidget);
    }

    return QObject::eventFilter(watched, event);
}

MouseMode FreeWidgetWraper::mouseMode() const
{
    return m_mouseMode;
}

void FreeWidgetWraper::setMouseMode(const MouseMode& mouseMode)
{
    m_mouseMode = mouseMode;
}

void FreeWidgetWraper::setPadding(int padding)
{
    this->padding = padding;
}

void FreeWidgetWraper::setMoveEnable(bool moveEnable)
{
    this->moveEnable = moveEnable;
}

void FreeWidgetWraper::setResizeEnable(bool resizeEnable)
{
    this->resizeEnable = resizeEnable;
}

void FreeWidgetWraper::setMousePressed(bool mousePressed)
{
    this->mousePressed = mousePressed;
}

void FreeWidgetWraper::setWidget(QWidget* widget)
{
    if(!m_pBindWidget)
    {
        m_pBindWidget = widget;
        //设置鼠标追踪为真
        m_pBindWidget->setMouseTracking(true);
        //绑定事件过滤器
        m_pBindWidget->installEventFilter(this);
        //设置悬停为真,必须设置这个,不然当父窗体里边还有子窗体全部遮挡了识别不到MouseMove,需要识别HoverMove
        m_pBindWidget->setAttribute(Qt::WA_Hover, true);

        isMin = false;
        flags = widget->windowFlags();
    }
}

void FreeWidgetWraper::onMouseModeChanged(MouseMode mode)
{
    m_mouseMode = mode;
}
