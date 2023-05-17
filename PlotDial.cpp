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

    QString name = QString("Dial%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

	m_title = "Dial";
	m_titleColor = Qt::white;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);

	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(12.0);

    //	m_leftPadding = 20;
    //	m_rightPadding = 20;
    //	m_topPadding = 0;
    //	m_bottomPadding = 20;

    m_widget = new QWidget;
    updateCenterPoint();
    setupLayout();
}

PlotDial::~PlotDial() {}

void PlotDial::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;
    auto dataPair = getDataPairs().last();
    auto xEntityID = dataPair->getEntityIDX();
    auto xAttr = dataPair->getAttr_x();
    QVector<double> m_valueList =
        DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);

    if(m_valueList.isEmpty())
        return;

    double currValue = m_valueList.last();

    //根据当前值计算指针终点
    double angle = (int)currValue % 360;

    QPoint endPoint;
    endPoint.setX(m_circleRadius * cos(qDegreesToRadians(angle)) + m_centerPoint.x());
    endPoint.setY(-m_circleRadius * sin(qDegreesToRadians(angle)) + m_centerPoint.y());

    QVector2D vec(endPoint.x() - m_centerPoint.x(), endPoint.y() - m_centerPoint.y());
    QPoint midPoint;
    midPoint.setX(m_centerPoint.x() + vec.x() * 0.1);
    midPoint.setY(m_centerPoint.y() + vec.y() * 0.1);

    QVector2D normalVec(-0.1 * ((double)endPoint.y() - (double)m_centerPoint.y()),
                        0.1 * ((double)endPoint.x() - (double)m_centerPoint.x()));

    QPoint endPoint1(midPoint.x() + normalVec.x(), midPoint.y() + normalVec.y());
    QPoint endPoint2(midPoint.x() - normalVec.x(), midPoint.y() - normalVec.y());
    m_clockHandPoints[0] = m_centerPoint;
    m_clockHandPoints[1] = endPoint1;
    m_clockHandPoints[2] = endPoint;
    m_clockHandPoints[3] = endPoint2;
    update();
}

void PlotDial::customPainting(QPainter& painter)
{
    // TODO:理论上是在控件尺寸上发生变换时才需要调用updateCenterPoint
    updateCenterPoint();
    painter.save();
    //绘制表盘圆圈
    QPen pen;
    pen.setColor(m_dialColor);
    pen.setWidth(2);
    painter.setPen(pen);

    painter.drawEllipse(m_centerPoint, m_circleRadius, m_circleRadius);

    // 绘制表盘圆弧
    double arcHeight = m_circleRadius / 20;
    painter.translate(m_centerPoint);
    QRectF rect(-m_circleRadius, -m_circleRadius, m_circleRadius << 1, m_circleRadius << 1);
    QPainterPath pathGreen;
    pathGreen.arcTo(rect, 30, 180);
    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
    pathGreen -= subPath;
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 128, 0));
    painter.drawPath(pathGreen);

    QPainterPath pathYellow;
    pathYellow.arcTo(rect, 30, -45);
    pathYellow -= subPath;
    painter.setBrush(QColor(128, 128, 0));
    painter.drawPath(pathYellow);

    QPainterPath pathRed;
    pathRed.arcTo(rect, -15, -75);
    pathRed -= subPath;
    painter.setBrush(QColor(128, 0, 0));
    painter.drawPath(pathRed);

    // 绘制刻度
    pen.setWidth(2);
    pen.setColor(m_dialColor);
    painter.setPen(pen);
    painter.drawLine(QPointF(-m_circleRadius, 0), QPointF(-m_circleRadius * 0.95, 0));
    painter.drawLine(QPointF(m_circleRadius, 0), QPointF(m_circleRadius * 0.95, 0));
    painter.drawLine(QPointF(0, -m_circleRadius * 0.95), QPointF(0, -m_circleRadius));
    painter.drawLine(QPointF(0, m_circleRadius * 0.95), QPointF(0, m_circleRadius));

    // 绘制表盘文字
    painter.setFont(m_axisFont);
    QFontMetricsF fm1(m_axisFont);
    double w = fm1.size(Qt::TextSingleLine, QString("-50°")).width();
    double h = fm1.size(Qt::TextSingleLine, QString("-50°")).height();
    painter.drawText(-m_circleRadius * 0.93, h / 3, QString("-50°"));

    w = fm1.size(Qt::TextSingleLine, QString("0°")).width();
    h = fm1.size(Qt::TextSingleLine, QString("0°")).height();
    painter.drawText(-w / 2, -m_circleRadius * 0.95 + h, QString("0°"));

    w = fm1.size(Qt::TextSingleLine, QString("50°")).width();
    h = fm1.size(Qt::TextSingleLine, QString("50°")).height();
    painter.drawText(m_circleRadius * 0.93 - w, h / 3, QString("50°"));

    w = fm1.size(Qt::TextSingleLine, QString("100°")).width();
    h = fm1.size(Qt::TextSingleLine, QString("100°")).height();
    painter.drawText(-w / 2, m_circleRadius * 0.93, QString("100°"));
    // 绘制上面的图形坐标轴是基于平移之后的，需要还原，指针的坐标是没有平移过的
    painter.restore();
    // 绘制图表的指针
    if(!getDataPairs().isEmpty())
    {
        QBrush pointerBrush(m_pointColor, Qt::SolidPattern);
        painter.setBrush(pointerBrush);
        painter.drawPolygon(m_clockHandPoints, 4);
    }
}

void PlotDial::updateCenterPoint()
{
    m_circleRadius = int(qMin(m_widget->width() * 0.95, m_widget->height() * 0.95) / 2);
    m_centerPoint = QPoint((m_widget->width()) / 2, (m_widget->height()) / 2);
}
