#include "mydialpointer.h"

/*
使用方法如下：
    MyDialPointer* pointer3 = new MyDialPointer(m_plot);
    pointer3->setColor(Qt::red);
    pointer3->setValue(120);
    pointer3->setRadius(2);
    pointer3->start->setCoords(2, 2);
value即为角度
radius表示表盘上的长度
start表示圆心位置
*/

MyDialPointer::MyDialPointer(QCustomPlot* parentPlot)
    : QCPAbstractItem(parentPlot)
    , start(createPosition(QLatin1String("start")))
    , end(createPosition(QLatin1String("end")))
{
    start->setCoords(2, 2);
    end->setCoords(3, 3);

    setPen(QPen(Qt::green));
}

MyDialPointer::~MyDialPointer() {}

void MyDialPointer::setPen(const QPen& pen)
{
    mPen = pen;
}

void MyDialPointer::setSelectedPen(const QPen& pen)
{
    mSelectedPen = pen;
}

void MyDialPointer::setHead(const QCPLineEnding& head)
{
    mHead = head;
}

void MyDialPointer::setTail(const QCPLineEnding& tail)
{
    mTail = tail;
}

void MyDialPointer::setColor(const QColor& color)
{
    mColor = color;
}

void MyDialPointer::setValue(double value)
{
    m_Value = value;
}

void MyDialPointer::setRadius(double radius)
{
    m_radius = radius;
}

void MyDialPointer::draw(QCPPainter* painter)
{
    QBrush pointerBrush(mColor, Qt::SolidPattern);
    painter->setBrush(pointerBrush);

    double angle = int(90 - m_Value) % 360;
    end->setCoords(start->coords().x() + m_radius * cos(angle * M_PI / 180),
                   start->coords().y() + m_radius * sin(angle * M_PI / 180));

    QPointF centerPoint = start->pixelPosition();
    double r = sqrt(pow(start->pixelPosition().x() - end->pixelPosition().x(), 2) +
                    pow(start->pixelPosition().y() - end->pixelPosition().y(), 2));
    QPointF point1, point2;
    point1.setX(centerPoint.x() + r * 0.1 * cos((angle - 30) * M_PI / 180));
    point1.setY(centerPoint.y() - r * 0.1 * sin((angle - 30) * M_PI / 180));

    point2.setX(centerPoint.x() + r * 0.1 * cos((angle + 30) * M_PI / 180));
    point2.setY(centerPoint.y() - r * 0.1 * sin((angle + 30) * M_PI / 180));

    m_clockHandPoints[0] = start->pixelPosition();
    m_clockHandPoints[1] = point1;
    m_clockHandPoints[2] = end->pixelPosition();
    m_clockHandPoints[3] = point2;

    painter->drawPolygon(m_clockHandPoints, 4);
}

/* inherits documentation from base class */
double MyDialPointer::selectTest(const QPointF& pos, bool onlySelectable, QVariant* details) const
{
    Q_UNUSED(details)
    if(onlySelectable && !mSelectable)
        return -1;

    return qSqrt(
        QCPVector2D(pos).distanceSquaredToLine(start->pixelPosition(), end->pixelPosition()));
}
