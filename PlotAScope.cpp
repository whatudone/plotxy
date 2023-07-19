#include "PlotAScope.h"
#include "DataManager.h"
#include "Utils.h"

int PlotAScope::m_instanceCount = 1;
PlotAScope::PlotAScope(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("A-Scope%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "A-Scope";

    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Voltage(v)";

    m_showUnits_x = false;
    m_showUnits_y = false;

    m_isDrawGate = false;
    m_gateHeight = 50;
    m_gateColor = Qt::red;
    m_isAutofitX = false;
    m_isAutofitY = false;

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

    m_customPlot->axisRect()->setBackground(m_gridFillColor);
    m_customPlot->xAxis->setLabelColor(m_xAxisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_yAxisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_xAxisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_yAxisLabelFont);

    m_gateRectItem = new QCPItemRect(m_customPlot);
    m_gateText = new QCPItemText(m_customPlot);
    drawGate();
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
    m_minX = std::numeric_limits<double>::min();
    m_maxX = std::numeric_limits<double>::max();
    m_minY = std::numeric_limits<double>::min();
    m_maxY = std::numeric_limits<double>::max();
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
        if(!x.isEmpty() && !y.isEmpty())
        {
            if(math::doubleEqual(m_minX, std::numeric_limits<double>::min()))
                m_minX = x.at(0);
            if(math::doubleEqual(m_minY, std::numeric_limits<double>::min()))
                m_minY = y.at(0);
            if(math::doubleEqual(m_maxX, std::numeric_limits<double>::max()))
                m_maxX = x.at(0);
            if(math::doubleEqual(m_maxY, std::numeric_limits<double>::max()))
                m_maxY = y.at(0);
        }

        for(int index = 0; index < x.size(); index++)
        {
            m_minX = m_minX < x.at(index) ? m_minX : x.at(index);
            m_maxX = m_maxX > x.at(index) ? m_maxX : x.at(index);
            m_minY = m_minY < y.at(index) ? m_minY : y.at(index);
            m_maxY = m_maxY > y.at(index) ? m_maxY : y.at(index);
        }
    }
    if(!m_isAutofitX || math::doubleEqual(m_minX, std::numeric_limits<double>::min()))
        m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
    else
        m_customPlot->xAxis->setRange(m_minX, m_maxX);
    if(!m_isAutofitY || math::doubleEqual(m_minY, std::numeric_limits<double>::min()))
        m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);
    else
        m_customPlot->yAxis->setRange(m_minY, m_maxY);
    for(int i = 0; i < itemCnt; ++i)
    {
        updateGraphByDataPair(m_dataPairs.at(i), secs);
    }
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotAScope::updateGraphByDataPair(DataPair* data, double curSecs)
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
        graph->setAdaptiveSampling(false);
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

        graph->setData(x, y, true);

        graph->setPen(QPen(data->dataColor(), data->width()));
        //line mode
        if(data->isLineMode())
        {
            graph->setLineStyle(QCPGraph::lsLine);
            Qt::PenStyle style = Qt::SolidLine;
            // 线性模式下才支持stipple
            if(data->getIsStippleEnable())
            {
                style = data->getStipplePattern();
            }
            QPen pen = graph->pen();
            pen.setStyle(style);
            graph->setPen(pen);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
        }
        else
        {
            graph->setLineStyle(QCPGraph::lsNone);
            graph->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, data->width()));
        }
    }
    else
    {
        graph->setVisible(false);
    }
}

void PlotAScope::drawGate()
{
    if(m_isDrawGate)
    {
        m_gateRectItem->setVisible(true);
        m_gateRectItem->setBrush(QBrush(m_gateColor));
        m_gateRectItem->topLeft->setType(QCPItemPosition::ptAxisRectRatio);
        m_gateRectItem->bottomRight->setType(QCPItemPosition::ptAxisRectRatio);
        m_gateRectItem->topLeft->setCoords(0.45, 1);
        m_gateRectItem->bottomRight->setCoords(0.55, 0);

        m_gateText->setVisible(true);
        m_gateText->position->setType(QCPItemPosition::ptAxisRectRatio);
        m_gateText->setText("Gate");
        m_gateText->setColor(Qt::white);
        m_gateText->setPositionAlignment(Qt::AlignHCenter);
        m_gateText->position->setCoords(0.5, (100 - m_gateHeight) / 100.0);
    }
    else
    {
        m_gateRectItem->setVisible(false);
        m_gateText->setVisible(false);
    }
}

bool PlotAScope::isDrawGate() const
{
    return m_isDrawGate;
}

void PlotAScope::setIsDrawGate(bool isDrawGate)
{
    m_isDrawGate = isDrawGate;
    drawGate();
    m_customPlot->replot();
}

int PlotAScope::gateHeight() const
{
    return m_gateHeight;
}

void PlotAScope::setGateHeight(int gateHeight)
{
    m_gateHeight = gateHeight;
    drawGate();
    m_customPlot->replot();
}

QColor PlotAScope::gateColor() const
{
    return m_gateColor;
}

void PlotAScope::setGateColor(const QColor& gateColor)
{
    m_gateColor = gateColor;
    drawGate();
    m_customPlot->replot();
}

bool PlotAScope::isAutofitX() const
{
    return m_isAutofitX;
}

void PlotAScope::setIsAutofitX(bool isAutofitX)
{
    m_isAutofitX = isAutofitX;
    m_customPlot->replot();
}

bool PlotAScope::isAutofitY() const
{
    return m_isAutofitY;
}

void PlotAScope::setIsAutofitY(bool isAutofitY)
{
    m_isAutofitY = isAutofitY;
    m_customPlot->replot();
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
