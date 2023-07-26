#include "PlotTrack.h"
#include "DataManager.h"
#include "Utils.h"

#include <QDebug>
#include <QPainter>

int PlotTrack::m_instanceCount = 1;

PlotTrack::PlotTrack(QWidget* parent)
    : PlotItemBase(parent)
    , m_minTime(std::numeric_limits<double>::min())
    , m_maxTime(std::numeric_limits<double>::max())
{
    QString name = QString("Track%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColorMap.insert(TrackStatus::Available, Qt::green);
    m_defaultColorMap.insert(TrackStatus::Invalid, Qt::gray);
    m_defaultColorMap.insert(TrackStatus::Unavailable, Qt::red);

    m_title = "Track Status";

    m_xAxisLabel = "X Axis";
    m_yAxisLabel = "Y Axis";

    initPlot();
    setupLayout();
}

PlotTrack::~PlotTrack() {}

void PlotTrack::initPlot()
{
    m_customPlot = new QCustomPlot();
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(int(m_vertGrids));
    m_customPlot->yAxis->ticker()->setTickCount(int(m_horzGrids));
    m_customPlot->xAxis->setTickLabelColor(m_xTickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_yTickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_xTickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_yTickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

    m_customPlot->axisRect()->setBackground(m_gridFillColor);

    m_customPlot->yAxis->setTickLabelRotation(45); // 轴刻度文字旋转45度
    m_customPlot->yAxis->setSubTicks(false); // 不显示子刻度
    m_customPlot->yAxis->setTickLength(0, 4); // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    m_customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->xAxis->setPadding(30);
    m_customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->yAxis->setLabel(m_yAxisLabel);
    m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);
}

void PlotTrack::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

    int itemCnt = getDataPairs().size();

    for(int i = 0; i < itemCnt; i++)
    {
        auto data = getDataPairs().at(i);
        m_curValue.insert(data->getUuid(), secs);
    }

    for(int i = 0; i < itemCnt; i++)
    {
        updateGraphByDataPair(getDataPairs().at(i), secs);
    }

    m_customPlot->replot();
}

void PlotTrack::updateGraphByDataPair(DataPair* dataPair, double curSecs)
{
    if(!dataPair)
        return;

    QString uuid = dataPair->getUuid();
    double lowLimit = std::numeric_limits<double>::min();
    double highLimit = std::numeric_limits<double>::max();

    if(m_itemData[uuid].size() == 2)
    {
        lowLimit = m_itemData[uuid].at(0);
        highLimit = m_itemData[uuid].at(1);
    }

    int index = 0;
    int cnt = 0; // 标记是哪一条Target对应的Bar
    for(auto it = m_tickLabelMap.constBegin(); it != m_tickLabelMap.constEnd(); it++)
    {
        index++;
        if(it.key() == uuid)
        {
            cnt = index;
            break;
        }
    }

    if(m_curValue.contains(uuid))
    {
        double secs = m_curValue[uuid];
        if(secs < lowLimit)
        {
            // 仅有Unavailable数据
            m_allBar[uuid].at(0)->setData(QVector<double>() << cnt, QVector<double>() << secs);
            m_allBar[uuid].at(1)->setData(QVector<double>() << cnt, QVector<double>() << 0);
            m_allBar[uuid].at(2)->setData(QVector<double>() << cnt, QVector<double>() << 0);
        }
        else if(secs > lowLimit && secs < highLimit)
        {
            // 仅有Unavailable数据和部分Available数据
            m_allBar[uuid].at(0)->setData(QVector<double>() << cnt, QVector<double>() << lowLimit);
            m_allBar[uuid].at(1)->setData(QVector<double>() << cnt,
                                          QVector<double>() << (secs - lowLimit));
            m_allBar[uuid].at(2)->setData(QVector<double>() << cnt, QVector<double>() << 0);
        }
        else if(secs > highLimit)
        {
            // 仅有Unavailable数据和全部Available数据
            // 目前超过Available的数据临时用Invalid的Bar显示，后期再进行优化
            m_allBar[uuid].at(0)->setData(QVector<double>() << cnt, QVector<double>() << lowLimit);
            m_allBar[uuid].at(1)->setData(QVector<double>() << cnt,
                                          QVector<double>() << highLimit - lowLimit);
            m_allBar[uuid].at(2)->setData(QVector<double>() << cnt,
                                          QVector<double>() << (secs - highLimit));
        }
    }
}

void PlotTrack::updateKeyAxisTickLabel()
{
    if(m_tickLabelMap.isEmpty())
    {
        return;
    }

    m_barTicks.clear();
    QVector<QString> labels;

    int index = 1;
    for(auto it = m_tickLabelMap.constBegin(); it != m_tickLabelMap.constEnd(); it++)
    {
        m_barTicks << index++;
        labels << it.value();
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(m_barTicks, labels);
    m_customPlot->yAxis->setTicker(textTicker); // 设置为文字轴
    m_customPlot->replot();
}

DataPair* PlotTrack::addPlotDataPair(int32_t xEntityID,
                                     const QString& xAttrName,
                                     const QString& xAttrUnitName,
                                     int32_t yEntityID,
                                     const QString& yAttrName,
                                     const QString& yAttrUnitName,
                                     const QHash<QString, QVariant>& extraParams,
                                     bool isFromJson)
{
    Q_UNUSED(extraParams)
    DataPair* data =
        new DataPair(xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);

    connect(data,
            &DataPair::dataUpdate,
            this,
            &PlotItemBase::onDataPairUpdateData,
            Qt::UniqueConnection);
    m_dataPairs.append(data);
    m_units_x = xAttrUnitName;
    m_units_y = yAttrUnitName;

    QString uuid = data->getUuid();

    // Available对应的Bar
    QCPBars* pBarAva = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
    pBarAva->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    pBarAva->setPen(QPen(m_defaultColorMap[TrackStatus::Available].lighter(130)));
    pBarAva->setBrush(m_defaultColorMap[TrackStatus::Available]);

    // Invalid对应的Bar
    QCPBars* pBarInv = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
    pBarInv->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    pBarInv->setPen(QPen(m_defaultColorMap[TrackStatus::Invalid].lighter(130)));
    pBarInv->setBrush(m_defaultColorMap[TrackStatus::Invalid]);

    // Unavailable对应的Bar
    QCPBars* pBarUna = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
    pBarUna->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    pBarUna->setPen(QPen(m_defaultColorMap[TrackStatus::Unavailable].lighter(130)));
    pBarUna->setBrush(m_defaultColorMap[TrackStatus::Unavailable]);

    double minTime;
    double maxTime;
    DataManager::getInstance()->getMinMaxTime(m_minTime, m_maxTime);
    QVector<double> timeList = DataManager::getInstance()->getEntityAttrValueListByMaxTime(
        xEntityID, "Time", m_maxTime, 1.0);
    if(!timeList.isEmpty())
    {
        // minTime和maxTime即为绘制的Available部分的下限和上限
        minTime = timeList.at(0);
        maxTime = timeList.last();
        m_itemData.insert(uuid, QList<double>() << minTime << maxTime);
    }

    QList<QCPBars*> barList;
    barList.push_back(pBarUna);
    barList.push_back(pBarAva);
    barList.push_back(pBarInv);
    pBarAva->moveAbove(pBarUna);
    pBarInv->moveAbove(pBarAva);
    m_allBar.insert(uuid, barList);
    if(extraParams.contains("XEntityName"))
        data->setEntity_x(extraParams.value("XEntityName").toString());
    if(extraParams.contains("YEntityName"))
        data->setEntity_y(extraParams.value("YEntityName").toString());
    m_tickLabelMap.insert(uuid, data->getEntity_x() + '_' + xAttrName);

    updateKeyAxisTickLabel();
    if(!isFromJson)
    {
        emit dataPairsChanged(this);
    }

    return data;
}

void PlotTrack::delPlotPairData(const QString& uuid)
{
    if(m_dataPairs.isEmpty())
        return;
    if(m_trackDrawDataMap.contains(uuid))
        m_trackDrawDataMap.remove(uuid);
    if(m_itemData.contains(uuid))
        m_itemData.remove(uuid);
    if(m_tickLabelMap.contains(uuid))
        m_tickLabelMap.remove(uuid);
    if(m_allBar.contains(uuid))
    {
        for(int i = 0; i < m_allBar[uuid].size(); i++)
        {
            m_customPlot->removePlottable(m_allBar[uuid].at(i));
        }
        m_allBar.remove(uuid);
    }
    updateKeyAxisTickLabel();
    PlotItemBase::delPlotPairData(uuid);
}
