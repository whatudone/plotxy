#include "PlotBar.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
int PlotBar::m_instanceCount = 1;
PlotBar::PlotBar(QWidget* parent)
    : PlotItemBase(parent)
    , m_min(std::numeric_limits<double>::min())
    , m_max(std::numeric_limits<double>::max())
{
    QString name = QString("Bar%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_defaultColor = Qt::green;
    m_gridColor = Qt::darkGray;
	m_axisColor = Qt::white;
	m_titleColor = Qt::white;
    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
	m_title = "Bar";
	m_titleFont.setFamily("Microsoft YaHei");
    m_titleFont.setPointSizeF(16.0);

    m_coordBgn_x = 0;
    m_coordEnd_x = 8;
    m_coordBgn_y = 0;
    m_coordEnd_y = 12.1;

    m_horzGrids = 5;
    m_vertGrids = 5;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = QColor(200, 200, 200);
    m_gridVisible = true;
    m_tickLabelColor = Qt::white;
    m_tickLabelFont.setFamily("Microsoft YaHei");
    m_tickLabelFont.setPointSizeF(8.0);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;

    initPlot();
}

PlotBar::~PlotBar() {}

void PlotBar::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;

    for(int i = 0; i < m_dataPairs.size(); ++i)
    {
        updateGraph(secs, m_dataPairs.at(i));
    }

    m_customPlot->replot();
}

void PlotBar::initPlot()
{
    m_customPlot = new QCustomPlot();
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
    m_customPlot->axisRect()->setupFullAxesBox(true);
    m_customPlot->axisRect()->setMinimumMargins(QMargins(30, 15, 30, 15));

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(int(m_vertGrids));
    m_customPlot->yAxis->ticker()->setTickCount(int(m_horzGrids));
    m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

    m_customPlot->setBackground(m_outerFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);

    m_customPlot->yAxis->setTickLabelRotation(60); // 轴刻度文字旋转60度
    m_customPlot->yAxis->setSubTicks(false); // 不显示子刻度
    m_customPlot->yAxis->setTickLength(0, 4); // 轴内外刻度的长度分别是0,4,也就是轴内的刻度线不显示
    m_customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->xAxis->setPadding(30);
    m_customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_customPlot->replot();
}

void PlotBar::updateGraph(double secs, DataPair* data)
{
    if(!data)
    {
        return;
    }

    auto uuid = data->getUuid();
    auto xEntityID = data->getEntityIDX();
    auto xAttr = data->getAttr_x();

    double value = DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);

    if(abs(value - std::numeric_limits<double>::max()) < 1) // 表示value无效
    {
        value = 0;
    }
    int index = 0;
    int cnt = 0; // 标记是哪一条Target对应的Bar
    for(auto it = m_itemInfo.constBegin(); it != m_itemInfo.constEnd(); it++)
    {
        index++;
        if(it.key() == uuid)
        {
            cnt = index;
            break;
        }
    }

    QList<std::tuple<QString, double, QColor>> colorList = data->getColorInfoList();
    if(data->getColorInfoList().isEmpty())
    {
        // 表示未设置显示范围，则全用默认颜色绘制
        m_allBar[uuid].at(0)->setData(QVector<double>() << cnt, QVector<double>() << value);
        return;
    }
    for(int i = 0; i < colorList.size(); i++)
    {
        // 如果用户设置的colorRange范围在所有数据的最小值和最大值之外，则认为设置无效，不绘制图像
        if(std::get<1>(colorList.at(i)) < m_min || std::get<1>(colorList.at(i)) > m_max)
        {
            return;
        }
    }

    // 不相等表示用户重新设置了阈值，这个时候需要重新创建堆叠的柱状图;相等表示没有重新设置，不用创建和修改
    if(m_colorInfoList != colorList)
    {
        m_colorInfoList = colorList;
        m_barColorInfoMap[uuid].clear();
        for(int i = 0; i < m_allBar[uuid].size(); i++)
        {
            m_customPlot->removePlottable(m_allBar[uuid].at(i));
        }
        QList<QCPBars*> tarBar;
        for(int i = 0; i < m_colorInfoList.size(); i++)
        {
            double thresholdValue = std::get<1>(m_colorInfoList.at(i));
            QColor color = std::get<2>(m_colorInfoList.at(i));
            m_barColorInfoMap[uuid].insert(thresholdValue, color);
        }

        // colorRange下限前面的部分，用默认颜色绘制
        QCPBars* subBar = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
        subBar->setPen(QPen(m_defaultColor.lighter(130)));
        subBar->setBrush(m_defaultColor);
        tarBar.push_back(subBar);

        for(auto it = m_barColorInfoMap[uuid].constBegin();
            it != m_barColorInfoMap[uuid].constEnd();
            it++)
        {
            QCPBars* subBar = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
            subBar->setPen(QPen(it.value().lighter(130)));
            subBar->setBrush(it.value());
            tarBar.push_back(subBar);
        }

        if(tarBar.size() > 1)
        {
            for(int i = 1; i < tarBar.size(); i++)
            {
                tarBar.at(i)->moveAbove(tarBar.at(i - 1));
            }
        }
        m_allBar.insert(uuid, tarBar);
    }

    // 设置了正确的colorRange，每个target上绘制多个Bar
    setBarData(uuid, value, cnt);
}

void PlotBar::updateLabelAndTick()
{
    if(m_itemInfo.isEmpty())
    {
        return;
    }

    m_barTicks.clear();
    QVector<QString> labels;

    int index = 1;
    for(auto it = m_itemInfo.constBegin(); it != m_itemInfo.constEnd(); it++)
    {
        m_barTicks << index++;
        labels << it.value();
    }

    QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
    textTicker->addTicks(m_barTicks, labels);
    m_customPlot->yAxis->setTicker(textTicker); // 设置为文字轴
    m_customPlot->yAxis->setRange(0, index);
    m_customPlot->xAxis->setRange(m_min, m_max);
    m_customPlot->replot();
}

void PlotBar::setBarData(const QString& uuid, double value, int index)
{
    if(!m_barColorInfoMap.contains(uuid) || !m_allBar.contains(uuid))
        return;
    QMap<double, QColor> colorMap = m_barColorInfoMap[uuid];
    QList<QCPBars*> barList = m_allBar[uuid];
    if(barList.isEmpty())
        return;

    QList<double> valueList;
    valueList.push_back(m_itemData[uuid]);
    for(auto it = colorMap.constBegin(); it != colorMap.constEnd(); it++)
    {
        if(value - it.key() > 0)
        {
            // 表示数值在标记的范围内
            valueList.push_back(it.key());
            continue;
        }
    }
    valueList.push_back(value);
    if(value < valueList.at(0))
    {
        barList.at(0)->setData(QVector<double>() << index, QVector<double>() << 0.0);
    }
    else
    {
        barList.at(0)->setData(QVector<double>() << index, QVector<double>() << valueList.at(0));
    }

    if(valueList.size() > 1)
    {
        int cnt = 1;
        for(; cnt < valueList.size() - 1; cnt++)
        {
            barList.at(cnt)->setData(QVector<double>() << index,
                                     QVector<double>()
                                         << (valueList.at(cnt + 1) - valueList.at(cnt)));
        }
    }
}

DataPair* PlotBar::addPlotDataPair(int32_t xEntityID,
                                   const QString& xAttrName,
                                   const QString& xAttrUnitName,
                                   int32_t yEntityID,
                                   const QString& yAttrName,
                                   const QString& yAttrUnitName,
                                   const QVariantList& extraParams)
{
    Q_UNUSED(extraParams)
    DataPair* data =
        new DataPair(xEntityID, xAttrName, xAttrUnitName, yEntityID, yAttrName, yAttrUnitName);
    m_dataPairs.append(data);

    // 目前界面上都是直接修改DataPair内部的数据，这里提供一个集中的入口虚函数处理。
    connect(data,
            &DataPair::dataUpdate,
            this,
            &PlotItemBase::onDataPairUpdateData,
            Qt::UniqueConnection);

    QString uuid = data->getUuid();
    QCPBars* pBar = new QCPBars(m_customPlot->yAxis, m_customPlot->xAxis);
    pBar->setAntialiased(false); // 为了更好的边框效果，关闭抗齿锯
    pBar->setPen(QPen(m_defaultColor.lighter(130)));
    pBar->setBrush(m_defaultColor);

    QMap<double, QColor> baseBarMap;
    baseBarMap.insert(0.0, m_defaultColor);
    m_barColorInfoMap.insert(uuid, baseBarMap);
    QList<QCPBars*> baseBar;
    baseBar.push_back(pBar);
    m_allBar.insert(uuid, baseBar);
    m_itemInfo.insert(uuid, data->getEntity_x() + '_' + xAttrName);

    QPair<double, double> limit =
        DataManager::getInstance()->getMaxAndMinEntityAttrValue(xEntityID, xAttrName);
    if(abs(m_min - std::numeric_limits<double>::min()) < 1)
    {
        // 表示m_min数值无意义，先赋值
        m_min = limit.first;
    }
    else
    {
        m_min = m_min < limit.first ? m_min : limit.first;
    }

    if(abs(m_max - std::numeric_limits<double>::max()) < 1)
    {
        // 表示m_max数值无意义，先赋值
        m_max = limit.second;
    }
    else
    {
        m_max = m_max > limit.second ? m_max : limit.second;
    }

    m_itemData.insert(uuid, limit.first);

    updateLabelAndTick();
    emit dataPairsChanged(this);

    return data;
}

void PlotBar::delPlotPairData(const QString& uuid)
{
    if(m_dataPairs.isEmpty())
        return;
    if(m_barColorInfoMap.contains(uuid))
    {
        m_barColorInfoMap.remove(uuid);
    }
    if(m_itemData.contains(uuid))
    {
        m_itemData.remove(uuid);
    }
    if(m_itemInfo.contains(uuid))
    {
        m_itemInfo.remove(uuid);
    }
    if(m_allBar.contains(uuid))
    {
        for(int i = 0; i < m_allBar[uuid].size(); i++)
        {
            m_customPlot->removePlottable(m_allBar[uuid].at(i));
        }
        m_allBar.remove(uuid);
    }
    updateLabelAndTick();
    PlotItemBase::delPlotPairData(uuid);
}
