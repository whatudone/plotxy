#include "PlotAScope.h"
#include "DataManager.h"

int PlotAScope::m_instanceCount = 1;
PlotAScope::PlotAScope(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("A-Scope%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
    m_title = "A-Scope";

    m_axisLabelFont.setFamily("Microsoft YaHei");
    m_axisLabelFont.setPointSizeF(10.0);
    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Voltage(v)";

    m_coordBgn_x = 0;
    m_coordEnd_x = 100;
    m_coordBgn_y = 0;
    m_coordEnd_y = 100;

    m_horzGrids = 5;
    m_vertGrids = 5;
    m_axisWidth = 1;
    m_gridWidth = 1;
    m_axisColor = Qt::white;
    m_gridColor = QColor(200, 200, 200);
    m_gridVisible = true;
    m_tickLabelColor = Qt::white;
    m_tickLabelFontSize = 8;
    m_tickLabelFont.setFamily("Microsoft YaHei");
    m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);
    m_gridStyle = Qt::DotLine;
    m_gridDensity = GridDensity::LESS;

    m_showUnits_x = false;
    m_showUnits_y = false;

    initPlot();
    setupLayout();
}

PlotAScope::~PlotAScope() {}

void PlotAScope::initPlot()
{
    m_customPlot = new QCustomPlot();
    m_customPlot->axisRect()->setupFullAxesBox(true);

    m_customPlot->xAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->yAxis->ticker()->setTickStepStrategy(QCPAxisTicker::tssMeetTickCount);
    m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
    m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
    m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
    m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
    m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
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

    m_customPlot->setBackground(m_outerFillColor);
    m_customPlot->axisRect()->setBackground(m_gridFillColor);
    m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
}

void PlotAScope::delPlotPairData(const QString& uuid)
{
    if(m_dataPairs.isEmpty())
        return;
    if(m_graphMap.contains(uuid))
    {
        auto graph = m_graphMap.take(uuid);
        m_customPlot->removeGraph(graph);
    }

    PlotItemBase::delPlotPairData(uuid);
}

void PlotAScope::updateDataForDataPairsByTime(double secs)
{
    m_dataHash.clear();
    int itemCnt = m_dataPairs.size();
    for(int i = 0; i < itemCnt; ++i)
    {
        auto data = getDataPairs().at(i);
        QString uuid = data->getUuid();
        QVector<double> x, y;
        int32_t eid = data->getEntityIDX();
        auto pair = DataManagerInstance->getSliceDataByTime(eid, secs);
        x = pair.first;
        y = pair.second;
        m_dataHash.insert(uuid, qMakePair(x, y));
    }
    for(int i = 0; i < itemCnt; ++i)
    {
        updateGraphByDataPair(m_dataPairs.at(i));
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotAScope::updateGraphByDataPair(DataPair* data)
{
    if(!data)
    {
        return;
    }
    QString uuid = data->getUuid();
    QCPGraph* graph = nullptr;
    if(m_graphMap.contains(uuid))
    {
        graph = m_graphMap.value(uuid);
    }
    else
    {
        graph = m_customPlot->addGraph();
        m_graphMap.insert(uuid, graph);
    }
    if(!graph)
    {
        return;
    }
    if(data->isDraw())
    {
        graph->setVisible(true);
        auto x = m_dataHash.value(uuid).first;
        auto y = m_dataHash.value(uuid).second;
        if(x.isEmpty() || y.isEmpty())
            return;
        graph->setData(x, y);
        graph->setPen(QPen(data->dataColor(), data->lineWidth()));
    }
    else
    {
        graph->setVisible(false);
    }
}

void PlotAScope::setAxisVisible(bool on, AxisType type)
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

void PlotAScope::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotAScope::setMinimumMargins(const QMargins& margins)
{
    m_customPlot->axisRect()->setMinimumMargins(margins);
}
