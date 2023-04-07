#include "PlotTrack.h"

#include <QDebug>
#include <QPainter>

#include "DataManager.h"

int PlotTrack::m_instanceCount = 1;

PlotTrack::PlotTrack(QWidget* parent)
    : PlotItemBase(parent)
{
    //    m_leftPadding = 50;
    //    m_rightPadding = 50;
    //    m_topPadding = 20;
    //    m_bottomPadding = 50;
    m_interPadding = 20;

    m_horiGridNum = 1;
    m_verGridNum = 1;

    QString name = QString("Track%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
    m_axisColor = Qt::lightGray;
    m_axisFont.setFamily("Microsoft YaHei");
    m_axisFont.setPointSizeF(10.0);

    m_title = "Track Status";
    m_titleColor = Qt::white;
    m_titleFont.setFamily("Microsoft YaHei");
    m_titleFont.setPointSizeF(16.0);
}

PlotTrack::~PlotTrack() {}

void PlotTrack::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
    m_thresholdColorMap = targetMap;
}

void PlotTrack::setLeftPadding(int leftPadding)
{
    m_leftPadding = leftPadding;
}

void PlotTrack::setRightPadding(int rightPadding)
{
    m_rightPadding = rightPadding;
}

void PlotTrack::setInterPadding(int interPadding)
{
    m_interPadding = interPadding;
}

void PlotTrack::drawRect(int itemIndex, QList<QColor> dataList)
{
    QFontMetricsF fm(m_titleFont);
    double h = fm.size(Qt::TextSingleLine, m_title).height();

    QPainter painter(this);
    QPen pen;
    pen.setWidth(1);
    int xPos = m_leftPadding;
    int yPos;
    int itemHeight;
    for(int i = 0; i < dataList.size(); i++)
    {
        pen.setColor(dataList.at(i));
        painter.setPen(pen);
        if(dataList.at(i) != Qt::gray)
        {
            itemHeight = this->height() / 25 * 1.5;
        }
        else
        {
            itemHeight = this->height() / 25;
        }
        yPos =
            this->height() - m_topPadding - h - ((itemIndex + 1) * (itemHeight + m_interPadding));
        painter.drawLine(QPoint(xPos + i, yPos), QPoint(xPos + i, yPos + itemHeight));
    }
}

void PlotTrack::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

    m_itemCnt = getDataPairs().size();
    QFontMetricsF fm(m_titleFont);
    double h = fm.size(Qt::TextSingleLine, m_title).height();
    setInterPadding(
        (height() - m_topPadding - m_bottomPadding - h - this->height() / 25 * 1.5 * m_itemCnt) /
        (m_itemCnt + 1));
    m_trackDrawDataMap.clear();
    for(int i = 0; i < m_itemCnt; i++)
    {
        QString entityType = getDataPairs().at(i)->getDataPair().first;
        updateData(i, entityType, secs);
    }
}

void PlotTrack::updateData(int itemIndex, QString entityType, double secs)
{
    QStringList entityList = entityType.split("+");
    QList<double> valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(
        entityList.at(0), entityList.at(1), secs);

    if(valueList.isEmpty())
        return;

    QList<QColor> dataList;
    for(int i = 0; i < valueList.size(); i++)
    {
        if(static_cast<int32_t>(valueList.at(i)) == 0)
        { // 当前时间有跟踪数据
            dataList.push_back(Qt::gray);
        }
        else
        {
            dataList.push_back(Qt::green);
        }
    }
    m_trackDrawDataMap.insert(itemIndex, dataList);
}

void PlotTrack::customPainting(QPainter& painter)
{
    //    QPainter painter(this);

    QFontMetricsF fm(m_titleFont);
    //    double w = fm.size(Qt::TextSingleLine, m_title).width();
    double h = fm.size(Qt::TextSingleLine, m_title).height();

    //绘制x轴和y轴
    QPen pen;
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPointF(m_leftPadding, height() - m_bottomPadding),
                     QPointF(width() - m_rightPadding, height() - m_bottomPadding)); // x轴
    painter.drawLine(QPointF(m_leftPadding, height() - m_bottomPadding),
                     QPointF(m_leftPadding, m_topPadding + h)); // y轴

    //绘制网格
    pen.setColor(m_gridColor);
    pen.setWidth(1);
    painter.setPen(pen);
    double verTablePadding = (height() - m_topPadding - m_bottomPadding - h) / m_verGridNum;
    double horTablePadding = (width() - m_leftPadding - m_rightPadding) / m_horiGridNum;
    for(int i = 0; i < m_verGridNum; i++)
    {
        painter.drawLine(QPointF(m_leftPadding, m_topPadding + h + verTablePadding * i),
                         QPointF(width() - m_rightPadding, m_topPadding + h + verTablePadding * i));
    }
    for(int i = 0; i < m_horiGridNum; i++)
    {
        painter.drawLine(
            QPointF(m_leftPadding + horTablePadding * (i + 1), m_topPadding + h),
            QPointF(m_leftPadding + horTablePadding * (i + 1), height() - m_bottomPadding));
    }
    // 绘制数据块
    for(int index : m_trackDrawDataMap.keys())
    {
        drawRect(index, m_trackDrawDataMap.value(index));
    }
}
