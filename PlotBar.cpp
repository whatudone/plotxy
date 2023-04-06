#include "PlotBar.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget* parent)
    : PlotItemBase(parent)
{
    m_bHorizontal = true;
	m_titleShow = true;

    //    m_leftPadding = 50;
    //    m_rightPadding = 50;
    //	m_topPadding = 0;
    //	m_bottomPadding = 50;
    m_interPadding = 20;

    m_currTimeIndex = 0;

    m_horiGridNum = 4;
    m_verGridNum = 5;

    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::gray;
    m_gridColor = Qt::darkGray;
	m_axisColor = Qt::white;
	m_titleColor = Qt::white;
	m_title = "Bar";
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);
}

PlotBar::~PlotBar() {}

void PlotBar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
    m_thresholdColorMap = targetMap;
}

void PlotBar::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
		return;

    update();
}

void PlotBar::setPlotItemExtentionDirec(bool horizontal)
{
    m_bHorizontal = horizontal;
}

void PlotBar::setLeftPadding(int leftPadding)
{
    m_leftPadding = leftPadding;
}

void PlotBar::setRightPadding(int rightPadding)
{
    m_rightPadding = rightPadding;
}

void PlotBar::setInterPadding(int interPadding)
{
    m_interPadding = interPadding;
}

void PlotBar::drawRect(int itemIndex,
                       bool bHorizontal,
                       int itemLength,
                       int leftBoundary,
                       int rightBoundary,
                       const QColor& color,
                       QPainter& painter)
{
    QPen pen;
    pen.setColor(color);
    QBrush brush;
    brush.setColor(color);
    brush.setStyle(Qt::SolidPattern);

    painter.setPen(pen);
    painter.setBrush(brush);
    int height = this->height();

    QRect rect;
    if(bHorizontal)
    {
        rect.setRect(leftBoundary,
                     height -
                         (m_leftPadding + itemIndex * (itemLength + m_interPadding) + itemLength),
                     rightBoundary - leftBoundary,
                     itemLength);
    }
    else
    {
        rect.setRect(m_leftPadding + itemIndex * (itemLength + m_interPadding),
                     height - rightBoundary,
                     itemLength,
                     rightBoundary - leftBoundary);
    }
    painter.drawRect(rect);
}

void PlotBar::drawPairData(
    int itemIndex, QString x, int32_t perItemLength, double secs, QPainter& painter)
{
    QStringList xlist = x.split("+");
    QList<double> xSecList =
        DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs);

    if(xSecList.isEmpty())
        return;

    //*获取当前Attr值
    double currValue = xSecList.last();

    QString currKey = xlist.at(0) + '_' + xlist.at(1);
    /*
     * 绘制分为两种大类情况：
     * 1、没有设置颜色阈值，直接从left->lastValue
     * 2、有阈值设置的，又分为三种情况：
     * 2.1、lastValue在阈值左侧
     * 2.2、lastValue在阈值范围内
     * 2.3、lastValue在阈值右侧
    */
    if(!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).isEmpty())
    {
        //没有设置阈值，则无需分开绘制多个矩形，以默认颜色绘制一个即可
        drawRect(itemIndex,
                 m_bHorizontal,
                 perItemLength,
                 m_leftPadding,
                 currValue,
                 m_defaultColor,
                 painter);
        return;
    }

    //根据颜色阈值来分开绘制
    auto colorMap = m_thresholdColorMap.value(currKey);
    QList<int> thresholdList = colorMap.keys();
    if(currValue < thresholdList.first())
    {
        drawRect(itemIndex,
                 m_bHorizontal,
                 perItemLength,
                 m_leftPadding,
                 currValue,
                 m_defaultColor,
                 painter);
    }
    else
    {
        //以默认颜色绘制第一个矩形
        drawRect(itemIndex,
                 m_bHorizontal,
                 perItemLength,
                 m_leftPadding,
                 thresholdList.first(),
                 m_defaultColor,
                 painter);

        auto thresSize = thresholdList.size();
        for(int i = 0; i < thresSize; ++i)
        {
            QColor currColor = colorMap.value(thresholdList.at(i));

            auto curThreshold = thresholdList.at(i);
            auto nextThreadHold = thresholdList.at(i) + perItemLength;
            if(i != thresSize - 1)
            {

                if(currValue < nextThreadHold)
                {
                    drawRect(itemIndex,
                             m_bHorizontal,
                             perItemLength,
                             curThreshold,
                             currValue,
                             currColor,
                             painter);
                    break;
                }
                else
                {
                    drawRect(itemIndex,
                             m_bHorizontal,
                             perItemLength,
                             curThreshold,
                             nextThreadHold,
                             currColor,
                             painter);
                }
            }
            else
            {
                drawRect(itemIndex,
                         m_bHorizontal,
                         perItemLength,
                         curThreshold,
                         currValue,
                         currColor,
                         painter);
            }
        }
    }
}

void PlotBar::customPainting(QPainter& painter)
{
    int width = this->width();
    int height = this->height();
    QFontMetricsF fm(m_titleFont);
    double h = fm.size(Qt::TextSingleLine, m_title).height();

    QPen pen;
    //绘制x轴和y轴
    pen.setColor(m_axisColor);
    pen.setWidth(2);
    painter.setPen(pen);
    painter.drawLine(QPoint(m_leftPadding, height - m_bottomPadding),
                     QPoint(width - m_rightPadding, height - m_bottomPadding)); //x轴
    painter.drawLine(QPoint(m_leftPadding, height - m_bottomPadding),
                     QPoint(m_leftPadding, m_topPadding + h)); //y轴

    //绘制网格
    pen.setColor(m_gridColor);
    painter.setPen(pen);

    QBrush brush; //画刷。填充几何图形的调色板，由颜色和填充风格组成
    brush.setColor(m_gridFillColor);
    brush.setStyle(Qt::SolidPattern);
    //painter.setBrush(brush);

    int horiGridWidth = 0;
    if(m_horiGridNum)
    { //item水平方向延展
        horiGridWidth = (width - m_leftPadding - m_rightPadding) / m_horiGridNum;
    }

    for(int i = 0; i < m_horiGridNum; i++)
    {
        QRect gridRect;
        gridRect.setRect(i * horiGridWidth + m_leftPadding,
                         m_topPadding + h,
                         horiGridWidth,
                         height - m_topPadding - m_bottomPadding - h);
        painter.drawRect(gridRect);
    }

    int verGridWidth = 0;
    if(m_verGridNum)
    { //item水平方向延展
        verGridWidth = (height - m_topPadding - m_bottomPadding - h) / m_verGridNum;
    }

    for(int i = 0; i < m_verGridNum; i++)
    {
        QRect gridRect;
        gridRect.setRect(m_leftPadding,
                         i * verGridWidth + m_topPadding + h,
                         width - m_leftPadding - m_rightPadding,
                         verGridWidth);
        painter.drawRect(gridRect);
    }
    // 绘制数据块
    drawPairDatas(painter);
}

void PlotBar::drawPairDatas(QPainter& painter)
{
    int isize = getDataPairs().size();
    int perItemLength = calculateItemLength();
    for(int i = 0; i < isize; i++)
    {
        QString xColumn = getDataPairs().at(i)->getDataPair().first;
        drawPairData(i, xColumn, perItemLength, m_seconds, painter);
    }
}

int PlotBar::calculateItemLength()
{
    //首先计算每个item的宽度/高度
    int cnt = getDataPairs().size();
    int perItemLength = 0;
    if(cnt <= 0)
    {
        return perItemLength;
    }
    QFontMetricsF fm(m_titleFont);
    double h = fm.size(Qt::TextSingleLine, m_title).height();
    int width = this->width() - m_leftPadding - m_rightPadding;
    int height = this->height() - m_topPadding - m_bottomPadding - h;

    if(m_bHorizontal)
    {
        //item水平方向延展
        m_horiGridNum = 5;
        m_verGridNum = 0;
        perItemLength = (height - (cnt - 1) * m_interPadding) / cnt;
    }
    else
    {
        //item垂直方向延展
        m_verGridNum = 5;
        m_horiGridNum = 0;
        perItemLength = (width - (cnt - 1) * m_interPadding) / cnt;
    }
    return perItemLength;
}
