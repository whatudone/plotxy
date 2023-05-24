#include "PlotDial.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QtMath>

int PlotDial::m_instanceCount = 1;
PlotDial::PlotDial(QWidget* parent)
    : PlotItemBase(parent)
{
    m_bThinStyle = true;

    m_dialColor = Qt::white;
    m_pointColor = Qt::green;
    m_capColor = Qt::red;

    QString name = QString("Dial%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    // 临时写死，后期适配
    m_startAngle = -135;
    m_endAngle = 135;
    m_tickCount = 7;

    m_title = "Dial";

    m_widget = new QWidget;
    updateCenterPoint();
    setupLayout();
}

PlotDial::~PlotDial() {}

void PlotDial::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

    auto dataPairList = getDataPairs();
    for(auto dataPair : dataPairList)
    {
        auto xEntityID = dataPair->getEntityIDX();
        auto xAttr = dataPair->getAttr_x();
        auto uuid = dataPair->getUuid();
        QVector<double> m_valueList =
            DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);

        if(m_valueList.isEmpty())
            return;

        double currValue = m_valueList.last();

        //根据当前值计算指针终点
        double angle = 225 - (currValue - m_coordBgn_x) / (m_coordEnd_x - m_coordBgn_x) *
                                 (m_endAngle - m_startAngle);
        //        qDebug() << currValue << angle;

        QPointF endPoint;
        endPoint.setX(m_circleRadius * cos(qDegreesToRadians(angle)) + m_centerPoint.x());
        endPoint.setY(-m_circleRadius * sin(qDegreesToRadians(angle)) + m_centerPoint.y());

        QVector2D vec(endPoint.x() - m_centerPoint.x(), endPoint.y() - m_centerPoint.y());
        QPointF midPoint;
        midPoint.setX(m_centerPoint.x() + vec.x() * 0.1);
        midPoint.setY(m_centerPoint.y() + vec.y() * 0.1);

        QVector2D normalVec(-0.1 * ((double)endPoint.y() - (double)m_centerPoint.y()),
                            0.1 * ((double)endPoint.x() - (double)m_centerPoint.x()));

        QPointF endPoint1(midPoint.x() + normalVec.x(), midPoint.y() + normalVec.y());
        QPointF endPoint2(midPoint.x() - normalVec.x(), midPoint.y() - normalVec.y());

        m_valueMap.insert(
            uuid, QVector<QPointF>() << m_centerPoint << endPoint1 << endPoint << endPoint2);
    }

    for(int i = 0; i < dataPairList.size(); i++)
    {
        updateGraphByDataPair(dataPairList.at(i));
    }
}

void PlotDial::customPainting(QPainter& painter)
{
    // TODO:理论上是在控件尺寸上发生变换时才需要调用updateCenterPoint
    updateCenterPoint();
    painter.save();
    double arcHeight = m_circleRadius / 20;
    painter.translate(m_centerPoint);

    QRectF rect(-m_circleRadius, -m_circleRadius, m_circleRadius * 2, m_circleRadius * 2);
    //绘制表盘圆圈
    QPen pen;

    QPainterPath outLinePath;
    outLinePath.arcTo(rect, 90 - m_endAngle, m_endAngle - m_startAngle);

    QPainterPath polyPath;
    QVector<QPointF> polyPoint;
    polyPoint.push_back(QPointF(0, 0));
    polyPoint.push_back(QPointF(m_circleRadius * sin(m_startAngle * M_PI / 180),
                                m_circleRadius * (-cos(m_startAngle * M_PI / 180))));
    polyPoint.push_back(QPointF(m_circleRadius * sin(m_endAngle * M_PI / 180),
                                m_circleRadius * (-cos(m_endAngle * M_PI / 180))));
    outLinePath.addPolygon(QPolygonF(polyPoint));

    painter.setBrush(m_capColor);
    pen.setColor(m_capColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawPath(outLinePath);

    pen.setColor(m_dialColor);
    painter.setPen(pen);
    painter.drawArc(rect, 16 * (90 - m_endAngle), 16 * (m_endAngle - m_startAngle));
    painter.drawLine(QPointF(m_circleRadius * sin(m_startAngle * M_PI / 180),
                             m_circleRadius * (-cos(m_startAngle * M_PI / 180))),
                     QPointF(m_circleRadius * sin(m_endAngle * M_PI / 180),
                             m_circleRadius * (-cos(m_endAngle * M_PI / 180))));

    // 绘制表盘圆弧
    //    QPainterPath pathGreen;
    //    pathGreen.arcTo(rect, 30, 180);
    //    QPainterPath subPath;
    //    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    //    pathGreen -= subPath;
    //    painter.setPen(Qt::NoPen);
    //    painter.setBrush(QColor(0, 128, 0));
    //    painter.drawPath(pathGreen);

    //    QPainterPath pathYellow;
    //    pathYellow.arcTo(rect, 30, -45);
    //    pathYellow -= subPath;
    //    painter.setBrush(QColor(128, 128, 0));
    //    painter.drawPath(pathYellow);

    //    QPainterPath pathRed;
    //    pathRed.arcTo(rect, -15, -75);
    //    pathRed -= subPath;
    //    painter.setBrush(QColor(128, 0, 0));
    //    painter.drawPath(pathRed);

    // 绘制刻度和表盘文字
    if(m_tickCount < 2)
        m_tickCount = 2;
    double curAngle = (m_endAngle - m_startAngle) / (m_tickCount - 1);
    double curSpan = (m_coordEnd_x - m_coordBgn_x) / (m_tickCount - 1);

    pen.setWidth(2);
    pen.setColor(m_dialColor);
    painter.setPen(pen);
    painter.setFont(m_tickLabelFont);
    double posX;
    double posY;
    for(int i = 0; i < m_tickCount; i++)
    {
        posX = m_circleRadius * cos((m_endAngle - curAngle * i - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - curAngle * i - 90) * M_PI / 180);
        painter.drawLine(QPointF(posX, posY), QPointF(posX * 0.95, posY * 0.95));
        painter.drawText(QPointF(posX * 0.9, posY * 0.9),
                         QString("%1").arg(m_coordEnd_x - curSpan * i));
    }

    // 绘制上面的图形坐标轴是基于平移之后的，需要还原，指针的坐标是没有平移过的
    painter.restore();
    // 绘制图表的指针

    auto dataPairList = getDataPairs();

    if(!dataPairList.isEmpty())
    {
        QBrush pointerBrush(m_pointColor, Qt::SolidPattern);
        for(auto data : dataPairList)
        {
            if(data->isDraw())
            {
                auto uuid = data->getUuid();
                if(m_valueMap.contains(uuid))
                {
                    if(m_valueMap[uuid].size() == 4)
                    {
                        QPointF clockPoint[4];
                        for(int i = 0; i < m_valueMap[uuid].size(); i++)
                        {
                            clockPoint[i] = m_valueMap[uuid].at(i);
                        }
                        pointerBrush.setColor(data->dataColor());
                        painter.setBrush(pointerBrush);
                        painter.drawPolygon(clockPoint, 4);
                    }
                }
            }
        }
    }
}

void PlotDial::updateGraphByDataPair(DataPair* data)
{
    Q_UNUSED(data)
    // 暂时无法单独更新每个DataPair数据，只能全局刷新
    update();
}

void PlotDial::setCoordRangeX(double lower, double upper)
{
    m_coordBgn_x = lower;
    m_coordEnd_x = upper;
}

void PlotDial::updateCenterPoint()
{
    m_circleRadius = int(m_widget->height() * 0.95 / (1 - cos(m_endAngle * M_PI / 180)));
    m_centerPoint = QPoint((m_widget->width()) / 2, m_circleRadius);
}
