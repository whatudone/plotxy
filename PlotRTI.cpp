#include "PlotRTI.h"
#include "DataManager.h"
#include "PlotXYDemo.h"

int PlotRTI::m_instanceCount = 1;
PlotRTI::PlotRTI(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("RTI%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "RTI Display";

    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Time(s)";

    m_coordEnd_x = 5;
    m_coordBgn_y = 0;
    m_coordEnd_y = 5;
    m_timeSpan = 10.0;

    m_showUnits_x = false;
    m_showUnits_y = false;

    initColorRangeMap();

    initPlot();
    setupLayout();
}

PlotRTI::~PlotRTI() {}

void PlotRTI::initPlot()
{
    m_customPlot = new QCustomPlot(this);
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
    m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    m_customPlot->xAxis->setTickLabelColor(m_xTickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_yTickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_xTickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_yTickLabelFont);
    m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

    m_customPlot->xAxis->setLabel(m_xAxisLabel);
    m_customPlot->yAxis->setLabel(m_yAxisLabel);
    m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
    m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);

    m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);

    m_colorMap = new QCPColorMap(m_customPlot->xAxis, m_customPlot->yAxis);
    m_colorScale = new QCPColorScale(m_customPlot);
    m_customPlot->plotLayout()->addElement(0, 1, m_colorScale);
    // 设置在图例的右侧
    m_colorScale->setType(QCPAxis::atRight);
    m_colorMap->setColorScale(m_colorScale);
    // 设置预定义的颜色渐变，也可以自定义
    m_colorMap->setGradient(QCPColorGradient::gpJet);

    auto marginGroup = new QCPMarginGroup(m_customPlot);
    // 保证上下对齐
    m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
    m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
}

void PlotRTI::setAxisVisible(bool on, AxisType type)
{
    switch(type)
    {
    case AxisType::xAxis:
        m_customPlot->xAxis->setVisible(on);
        break;
    case AxisType::yAxis:
        m_customPlot->yAxis->setVisible(on);
        break;
    case AxisType::xAxis2:
        m_customPlot->xAxis2->setVisible(on);
        break;
    case AxisType::yAxis2:
        m_customPlot->yAxis2->setVisible(on);
        break;
    default:
        break;
    }
    m_customPlot->replot();
}

void PlotRTI::setAxisTickLabelShow(bool on, AxisType type)
{
    switch(type)
    {
    case AxisType::xAxis:
        m_customPlot->xAxis->setTickLabels(on);
        break;
    case AxisType::yAxis:
        m_customPlot->yAxis->setTickLabels(on);
        break;
    case AxisType::xAxis2:
        m_customPlot->xAxis2->setTickLabels(on);
        break;
    case AxisType::yAxis2:
        m_customPlot->yAxis2->setTickLabels(on);
        break;
    default:
        break;
    }
    m_customPlot->replot();
}

QMap<double, QColor> PlotRTI::getColorRangeMap()
{
    return m_colorRangeMap;
}

void PlotRTI::setColorRangeMap(const QMap<double, QColor>& colorMap)
{
    QCPColorGradient m_colorGradient;
    m_colorRangeMap = colorMap;
    auto colorKeys = colorMap.keys();
    for(auto value : colorKeys)
    {
        m_colorGradient.setColorStopAt(value, colorMap.value(value));
    }
    m_colorScale->setGradient(m_colorGradient);
    m_customPlot->replot();
}

void PlotRTI::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
    {
        return;
    }
    auto data = getDataPairs().last();
    int32_t eid = data->getEntityIDX();
    DataManagerInstance->getRTIDataByTime(eid, secs, m_rangeList, m_timeList, m_dataHash);

    updateGraphByDataPair(data, secs);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotRTI::updateGraphByDataPair(DataPair* data, double curSecs)
{
    if(data->isDraw())
    {
        m_colorMap->setVisible(true);
        m_colorScale->setVisible(true);
        if(m_rangeList.isEmpty() || m_timeList.isEmpty())
        {
            return;
        }
        int nx = m_rangeList.size();
        int ny = m_timeList.size();
        //        m_colorMap->data()->setSize(nx, ny);
        //        m_colorMap->data()->setRange(QCPRange(m_rangeList.first(), m_rangeList.last()),
        //                                     QCPRange(m_timeList.first(), m_timeList.last()));

        // 实际显示的时间跨度数据
        int showTimeData = int(m_timeSpan / (m_timeList.last() - m_timeList.first()) * ny);
        m_colorMap->data()->setSize(nx, showTimeData);
        m_colorMap->data()->setRange(QCPRange(m_rangeList.first(), m_rangeList.last()),
                                     QCPRange(m_timeList.first(), m_timeList.first() + m_timeSpan));

        for(int xIndex = 0; xIndex < nx; ++xIndex)
        {
            for(int yIndex = 0; yIndex < showTimeData; ++yIndex)
            {
                auto coord = qMakePair(xIndex, yIndex);
                double voltage = m_dataHash.value(coord);
                m_colorMap->data()->setCell(xIndex, yIndex, voltage);
            }
        }
        m_colorMap->rescaleDataRange();
        m_customPlot->rescaleAxes();
    }
    else
    {
        m_colorMap->setVisible(false);
        m_colorScale->setVisible(false);
    }
}

void PlotRTI::initColorRangeMap()
{
    m_colorRangeMap.clear();
    m_colorRangeMap.insert(0.0, QColor(0, 0, 100));
    m_colorRangeMap.insert(0.1, QColor(0, 35, 200));
    m_colorRangeMap.insert(0.2, QColor(0, 100, 255));
    m_colorRangeMap.insert(0.3, QColor(0, 200, 255));
    m_colorRangeMap.insert(0.4, QColor(45, 255, 200));
    m_colorRangeMap.insert(0.5, QColor(127, 255, 127));
    m_colorRangeMap.insert(0.6, QColor(200, 255, 45));
    m_colorRangeMap.insert(0.7, QColor(255, 200, 0));
    m_colorRangeMap.insert(0.8, QColor(255, 200, 0));
    m_colorRangeMap.insert(0.9, QColor(200, 20, 0));
    m_colorRangeMap.insert(1.0, QColor(100, 0, 0));
}

double PlotRTI::getTimeSpan() const
{
    return m_timeSpan;
}

void PlotRTI::setTimeSpan(double timeSpan)
{
    m_timeSpan = timeSpan;
    updateDataForDataPairsByTime(PlotXYDemo::getSeconds());
}
