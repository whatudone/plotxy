#include "PlotDial.h"
#include "DataManager.h"
#include "Utils.h"

#include <QDebug>
#include <QPainter>
#include <QVector2D>
#include <QtMath>

int PlotDial::m_instanceCount = 1;
PlotDial::PlotDial(QWidget* parent)
    : PlotItemBase(parent)
{
    m_dialColor = Qt::white;
    m_capColor = Qt::red;

    QString name = QString("Dial%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    // 临时写死，后期适配
    m_startAngle = -120;
    m_endAngle = 120;
    m_radiusRate = 1000;
    m_colorRangeRate = 50;
    m_textRate = 1000;
    m_dialRate = 50;
    m_dialCapRate = 950;
    m_dialStyle = "Thin";

    m_title = "Dial";

    m_widget = new QWidget;
    updateCenterPoint();
    setupLayout();
}

PlotDial::~PlotDial() {}

void PlotDial::setTickRadiusRate(int rate)
{
    m_radiusRate = rate;
    update();
}

int PlotDial::getTickRadiusRate()
{
    return m_radiusRate;
}

void PlotDial::setColorRate(int rate)
{
    m_colorRangeRate = rate;
    update();
}

int PlotDial::getColorRate()
{
    return m_colorRangeRate;
}

void PlotDial::setTextRate(int rate)
{
    m_textRate = rate;
    update();
}

int PlotDial::getTextRate()
{
    return m_textRate;
}

void PlotDial::setDialRate(int rate)
{
    m_dialRate = rate;
    update();
}

int PlotDial::getDialRate()
{
    return m_dialRate;
}

void PlotDial::setDialCapRate(int rate)
{
    m_dialCapRate = rate;
    update();
}

int PlotDial::getDialCapRate()
{
    return m_dialCapRate;
}

void PlotDial::setStartAngle(int angle)
{
    m_startAngle = angle;
    update();
}

int PlotDial::getStartAngle()
{
    return m_startAngle;
}

void PlotDial::setEndAngle(int angle)
{
    m_endAngle = angle;
    update();
}

int PlotDial::getEndAngle()
{
    return m_endAngle;
}

void PlotDial::setCapColor(QColor color)
{
    m_capColor = color;
    update();
}

const QColor PlotDial::getCapColor()
{
    return m_capColor;
}

void PlotDial::setDrawFirstTick(bool draw)
{
    m_drawFirstTick = draw;
    update();
}

bool PlotDial::getDrawFirstTick()
{
    return m_drawFirstTick;
}

void PlotDial::setDrawLastTick(bool draw)
{

    m_drawLastTick = draw;
    update();
}

bool PlotDial::getDrawLastTick()
{
    return m_drawLastTick;
}

void PlotDial::setDrawFirstTextLabel(bool draw)
{

    m_drawFirstTextLabel = draw;
    update();
}

bool PlotDial::getDrawFirstTextLabel()
{
    return m_drawFirstTextLabel;
}

void PlotDial::setDrawLastTextLabel(bool draw)
{

    m_drawLastTextLabel = draw;
    update();
}

void PlotDial::setColorInfoList(const QList<DialColorInfo>& colorInfoList)
{
    m_colorInfoList = colorInfoList;
    update();
}

bool PlotDial::getDrawLastTextLabel()
{
    return m_drawLastTextLabel;
}

const QList<DialColorInfo> PlotDial::getColorInfoList()
{
    return m_colorInfoList;
}

void PlotDial::setDialStyle(const QString& style)
{
    m_dialStyle = style;
    update();
}

const QString PlotDial::getDialStyle()
{
    return m_dialStyle;
}

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
        double currValue =
            DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);
        m_valueMap.insert(uuid, currValue);
    }

    update();
}

void PlotDial::customPainting(QPainter& painter)
{
    // TODO:理论上是在控件尺寸上发生变换时才需要调用updateCenterPoint
    updateCenterPoint();
    painter.save();
    painter.translate(m_centerPoint);

    QRectF rect(-m_circleRadius, -m_circleRadius, m_circleRadius * 2, m_circleRadius * 2);
    //绘制表盘圆圈
    QPen pen;

    QPainterPath outLinePath;
    QRectF capRect;
    capRect.setRect(rect.x() * m_dialCapRate / 1000.0,
                    rect.y() * m_dialCapRate / 1000.0,
                    rect.width() * m_dialCapRate / 1000.0,
                    rect.height() * m_dialCapRate / 1000.0);
    outLinePath.arcTo(capRect, 90 - m_endAngle, m_endAngle - m_startAngle);

    QPainterPath polyPath;
    QVector<QPointF> polyPoint;
    polyPoint.push_back(QPointF(0, 0));
    polyPoint.push_back(
        QPointF(m_circleRadius * m_dialCapRate / 1000.0 * sin(m_startAngle * M_PI / 180),
                m_circleRadius * m_dialCapRate / 1000.0 * (-cos(m_startAngle * M_PI / 180))));
    polyPoint.push_back(
        QPointF(m_circleRadius * m_dialCapRate / 1000.0 * sin(m_endAngle * M_PI / 180),
                m_circleRadius * m_dialCapRate / 1000.0 * (-cos(m_endAngle * M_PI / 180))));
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

    if(m_dialStyle == "Thick")
    {
        // 绘制表盘圆弧
        double arcHeight = m_circleRadius * m_colorRangeRate / 1000.0;
        for(int i = 0; i < m_colorInfoList.size(); i++)
        {
            QPainterPath path;
            double start = m_colorInfoList.at(i).start;
            double end = m_colorInfoList.at(i).end;
            double startAngle = (m_endAngle + 90) - (start - m_coordBgn_x) /
                                                        (m_coordEnd_x - m_coordBgn_x) *
                                                        (m_endAngle - m_startAngle);
            double endAngle = (m_endAngle + 90) - (end - m_coordBgn_x) /
                                                      (m_coordEnd_x - m_coordBgn_x) *
                                                      (m_endAngle - m_startAngle);
            path.arcTo(rect, startAngle, endAngle - startAngle);
            QPainterPath subPath;
            subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));
            path -= subPath;
            QPen arcPen;
            arcPen.setWidth(m_colorInfoList.at(i).width);
            arcPen.setColor(m_colorInfoList.at(i).outline);
            painter.setPen(arcPen);
            painter.setBrush(m_colorInfoList.at(i).clr);
            painter.drawPath(path);
        }
    }

    // 绘制刻度和表盘文字
    if(m_horzGrids < 2)
        m_horzGrids = 2;
    double perAngle = (m_endAngle - m_startAngle) / (m_horzGrids - 1);
    double perSpan = (m_coordEnd_x - m_coordBgn_x) / (m_horzGrids - 1);

    pen.setWidth(int(m_gridWidth));
    pen.setColor(m_axisColor);
    painter.setPen(pen);
    QFont font = m_tickLabelFont;

    // pixelSize = 0的时候，设置不生效
    if(int(m_tickLabelFont.pixelSize() * m_textRate / 1000.0) == 0)
    {
        font.setPixelSize(1);
    }
    else
    {
        font.setPixelSize(int(m_tickLabelFont.pixelSize() * m_textRate / 1000.0));
    }
    painter.setFont(font);

    auto dataPairList = getDataPairs();
    QString strUnit;
    if(!dataPairList.isEmpty())
    {
        auto firData = dataPairList.at(0);
        strUnit = firData->getUnit_x();
    }

    double posX;
    double posY;
    QFontMetrics fm(font);
    int w = 0;
    for(int i = 1; i < m_horzGrids - 1; i++)
    {
        posX = m_circleRadius * cos((m_endAngle - perAngle * i - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - perAngle * i - 90) * M_PI / 180);
        painter.drawLine(
            QPointF(posX, posY),
            QPointF(posX * (1000 - m_dialRate) / 1000, posY * (1000 - m_dialRate) / 1000));
        if(!m_showUnits_x)
        {
            w = fm.width(QString("%1").arg(m_coordEnd_x - perSpan * i));
            painter.drawText(QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                             QString("%1").arg(m_coordEnd_x - perSpan * i));
        }
        else
        {
            w = fm.width(QString("%1%2").arg(m_coordEnd_x - perSpan * i).arg(strUnit));
            painter.drawText(QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                             QString("%1%2").arg(m_coordEnd_x - perSpan * i).arg(strUnit));
        }
    }

    if(m_drawFirstTick)
    {
        posX = m_circleRadius * cos((m_endAngle - perAngle * (m_horzGrids - 1) - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - perAngle * (m_horzGrids - 1) - 90) * M_PI / 180);
        painter.drawLine(
            QPointF(posX, posY),
            QPointF(posX * (1000 - m_dialRate) / 1000, posY * (1000 - m_dialRate) / 1000));
    }
    if(m_drawFirstTextLabel)
    {
        posX = m_circleRadius * cos((m_endAngle - perAngle * (m_horzGrids - 1) - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - perAngle * (m_horzGrids - 1) - 90) * M_PI / 180);
        if(!m_showUnits_x)
        {
            w = fm.width(QString("%1").arg(m_coordEnd_x - perSpan * (m_horzGrids - 1)));
            painter.drawText(QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                             QString("%1").arg(m_coordEnd_x - perSpan * (m_horzGrids - 1)));
        }
        else
        {
            w = fm.width(
                QString("%1%2").arg(m_coordEnd_x - perSpan * (m_horzGrids - 1)).arg(strUnit));
            painter.drawText(
                QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                QString("%1%2").arg(m_coordEnd_x - perSpan * (m_horzGrids - 1)).arg(strUnit));
        }
    }
    if(m_drawLastTick)
    {
        posX = m_circleRadius * cos((m_endAngle - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - 90) * M_PI / 180);
        painter.drawLine(
            QPointF(posX, posY),
            QPointF(posX * (1000 - m_dialRate) / 1000, posY * (1000 - m_dialRate) / 1000));
    }
    if(m_drawLastTextLabel)
    {
        posX = m_circleRadius * cos((m_endAngle - 90) * M_PI / 180);
        posY = m_circleRadius * sin((m_endAngle - 90) * M_PI / 180);
        if(!m_showUnits_x)
        {
            w = fm.width(QString("%1").arg(m_coordEnd_x));
            painter.drawText(QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                             QString("%1").arg(m_coordEnd_x));
        }
        else
        {
            w = fm.width(QString("%1%2").arg(m_coordEnd_x).arg(strUnit));
            painter.drawText(QPointF(posX < 0 ? posX * 0.95 : posX * 0.95 - w, posY * 0.9),
                             QString("%1%2").arg(m_coordEnd_x).arg(strUnit));
        }
    }

    // 绘制上面的图形坐标轴是基于平移之后的，需要还原，指针的坐标是没有平移过的
    painter.restore();

    // 绘制图表的指针
    for(auto data : dataPairList)
    {
        if(data->isDraw())
        {
            auto uuid = data->getUuid();
            if(m_valueMap.contains(uuid))
            {
                //根据当前值计算指针终点
                if(math::doubleEqual(m_valueMap[uuid], std::numeric_limits<double>::max()))
                    continue;
                double angle = (m_endAngle + 90) - (m_valueMap[uuid] - m_coordBgn_x) /
                                                       (m_coordEnd_x - m_coordBgn_x) *
                                                       (m_endAngle - m_startAngle);

                QPointF endPoint;
                endPoint.setX(m_circleRadius * cos(qDegreesToRadians(angle)) + m_centerPoint.x());
                endPoint.setY(-m_circleRadius * sin(qDegreesToRadians(angle)) + m_centerPoint.y());

                QVector2D vec(float(endPoint.x()) - m_centerPoint.x(),
                              float(endPoint.y()) - m_centerPoint.y());
                QPointF midPoint;
                midPoint.setX(m_centerPoint.x() + vec.x() * 0.1);
                midPoint.setY(m_centerPoint.y() + vec.y() * 0.1);

                QVector2D normalVec(-0.1 * (endPoint.y() - double(m_centerPoint.y())),
                                    0.1 * (endPoint.x() - double(m_centerPoint.x())));

                QPointF endPoint1(midPoint.x() + normalVec.x(), midPoint.y() + normalVec.y());
                QPointF endPoint2(midPoint.x() - normalVec.x(), midPoint.y() - normalVec.y());

                QPointF clockPoint[4];
                clockPoint[0] = m_centerPoint;
                clockPoint[1] = endPoint1;
                clockPoint[2] = endPoint;
                clockPoint[3] = endPoint2;
                QBrush pointerBrush(data->dataColor(), Qt::SolidPattern);
                painter.setBrush(pointerBrush);
                painter.drawPolygon(clockPoint, 4);
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

void PlotDial::updateCenterPoint()
{
    m_circleRadius =
        int(m_widget->height() * 0.95 / (1 - cos(m_endAngle * M_PI / 180)) * m_radiusRate / 1000);
    m_centerPoint = QPoint((m_widget->width()) / 2, m_circleRadius);
}
