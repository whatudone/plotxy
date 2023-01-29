#include "PlotDial.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include <QVector2D>
#include "DataManager.h"

int PlotDial::m_instanceCount = 1;
PlotDial::PlotDial(QWidget *parent)
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
	m_titleShow = true;

	m_axisFont.setFamily("Microsoft YaHei");
	m_axisFont.setPointSizeF(12.0);

	m_leftPadding = 20;
	m_rightPadding = 20;
	m_topPadding = 0;
	m_bottomPadding = 20;
}

PlotDial::~PlotDial()
{

}

//更新表盘指针
void PlotDial::updatePointer(double secs)
{
    if (getDataPair().isEmpty())
        return;

    int isize = getDataPair().size();
    QString xcolumn = getDataPair().at(isize - 1)->getDataPair().first;
    QStringList list = xcolumn.split("+");
    QList<double> m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(list.at(0), list.at(1), secs);

    if (m_valueList.isEmpty())
        return;

    double currValue = m_valueList.last();

    //根据当前值计算指针终点
    double angle = (int)currValue % 360;

    QPoint endPoint;
    endPoint.setX( m_circleRadius * cos(qDegreesToRadians(angle)) + m_centerPoint.x());
    endPoint.setY(-m_circleRadius * sin(qDegreesToRadians(angle)) + m_centerPoint.y());

    QVector2D vec(endPoint.x() - m_centerPoint.x(),
                  endPoint.y() - m_centerPoint.y());
    QPoint midPoint;
    midPoint.setX(m_centerPoint.x() + vec.x() * 0.1);
    midPoint.setY(m_centerPoint.y() + vec.y() * 0.1);

    QVector2D normalVec(-0.1 * ((double)endPoint.y() - (double)m_centerPoint.y()),
                        0.1 * ((double)endPoint.x() - (double)m_centerPoint.x()));

    QPoint endPoint1(midPoint.x() + normalVec.x(),
                     midPoint.y() + normalVec.y());
    QPoint endPoint2(midPoint.x() - normalVec.x(),
                     midPoint.y() - normalVec.y());

    QPoint points[4] = {
        m_centerPoint,
        endPoint1,
        endPoint,
        endPoint2
    };

    QPainter painter(this);
    QBrush pointerBrush(m_pointColor, Qt::SolidPattern);
    painter.setBrush(pointerBrush);
    painter.drawPolygon(points, 4);
}

void PlotDial::onGetCurrentSeconds(double secs)
{
    m_seconds = secs;
    update();
}

void PlotDial::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	//绘制标题
	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();

	m_circleRadius = (width() - m_leftPadding - m_rightPadding) < (height() - h - m_topPadding - m_bottomPadding) ? (width() - m_leftPadding - m_rightPadding) / 2 : (height() - h - m_topPadding - m_bottomPadding) / 2;
	m_centerPoint = QPoint((width() + m_leftPadding - m_rightPadding) / 2, (height() + h + m_topPadding - m_bottomPadding) / 2);
	
	if (m_titleShow)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
		painter.drawText(QPoint((width() + m_leftPadding - m_rightPadding - w) / 2, m_centerPoint.y() - m_circleRadius + (as - h)), m_title);
	}

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
 	w = fm1.size(Qt::TextSingleLine, QString("-50°")).width();
 	h = fm1.size(Qt::TextSingleLine, QString("-50°")).height();
    painter.drawText(-m_circleRadius * 0.93, h/3, QString("-50°"));

	w = fm1.size(Qt::TextSingleLine, QString("0°")).width();
	h = fm1.size(Qt::TextSingleLine, QString("0°")).height();
    painter.drawText(-w/2, -m_circleRadius * 0.95 + h, QString("0°"));

	w = fm1.size(Qt::TextSingleLine, QString("50°")).width();
	h = fm1.size(Qt::TextSingleLine, QString("50°")).height();
    painter.drawText(m_circleRadius*0.93 - w, h/3, QString("50°"));

	w = fm1.size(Qt::TextSingleLine, QString("100°")).width();
	h = fm1.size(Qt::TextSingleLine, QString("100°")).height();
    painter.drawText(-w/2, m_circleRadius * 0.93, QString("100°"));

    updatePointer(m_seconds);

    return PlotItemBase::paintEvent(event);
}
