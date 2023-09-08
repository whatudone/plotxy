#include "PlotDoppler.h"
#include "DataManager.h"
#include "PlotXYDemo.h"
#include "Utils.h"

#include <QGridLayout>
int PlotDoppler::m_instanceCount = 1;
PlotDoppler::PlotDoppler(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("Doppler%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_title = "Range Doppler";

    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Voltage(V)";

    m_showUnits_x = false;
    m_showUnits_y = false;

    initColorRangeMap();
    initPlot();
    setupLayout();
}

PlotDoppler::~PlotDoppler() {}

void PlotDoppler::initPlot()
{
    m_customPlot = new QCustomPlot(this);
    QCPAxisRect* rtiPlot = new QCPAxisRect(m_customPlot);
    rtiPlot->setupFullAxesBox(true);
    rtiPlot->setMargins(QMargins(0, 0, 0, 0));
    QCPAxisRect* horPlot = new QCPAxisRect(m_customPlot);
    QCPAxisRect* verPlot = new QCPAxisRect(m_customPlot);
    m_colorScale = new QCPColorScale(m_customPlot);
    m_colorScale->setType(QCPAxis::atLeft);

    m_customPlot->plotLayout()->clear();
    m_customPlot->plotLayout()->addElement(0, 0, m_colorScale);
    m_customPlot->plotLayout()->addElement(0, 1, rtiPlot);
    m_customPlot->plotLayout()->addElement(0, 2, verPlot);
    m_customPlot->plotLayout()->addElement(1, 1, horPlot);
    m_customPlot->plotLayout()->setColumnStretchFactor(0, 0.1);
    m_customPlot->plotLayout()->setColumnStretchFactor(1, 0.6);
    m_customPlot->plotLayout()->setColumnStretchFactor(2, 0.3);
    m_customPlot->plotLayout()->setRowStretchFactor(0, 0.6);
    m_customPlot->plotLayout()->setRowStretchFactor(1, 0.4);
    m_customPlot->plotLayout()->setRowSpacing(0);
    m_customPlot->plotLayout()->setColumnSpacing(0);

    m_customPlot->addGraph(rtiPlot->axis(QCPAxis::atBottom), rtiPlot->axis(QCPAxis::atLeft));
    m_customPlot->addGraph(horPlot->axis(QCPAxis::atBottom), horPlot->axis(QCPAxis::atLeft));
    m_customPlot->addGraph(verPlot->axis(QCPAxis::atRight), verPlot->axis(QCPAxis::atBottom));
    m_colorMap = new QCPColorMap(rtiPlot->axis(QCPAxis::atBottom), rtiPlot->axis(QCPAxis::atLeft));
    m_colorMap->setColorScale(m_colorScale);
    m_colorMap->setGradient(QCPColorGradient::gpJet);

    m_customPlot->graph(0)->setName("rtiPlot");
    m_customPlot->graph(1)->setName("horPlot");
    m_customPlot->graph(2)->setName("verPlot");
    auto rtiPlotX = rtiPlot->axis(QCPAxis::atBottom);
    auto rtiPlotY = rtiPlot->axis(QCPAxis::atLeft);
    rtiPlotX->setVisible(false);
    rtiPlotY->setVisible(false);

    auto horPlotX = horPlot->axis(QCPAxis::atBottom);
    auto horPlotY = horPlot->axis(QCPAxis::atLeft);
    horPlotX->setVisible(true);
    horPlotY->setVisible(true);
    horPlotX->setTicks(true);
    horPlotX->setTickLabels(true);
    horPlotY->setTickLabels(false);

    auto verPlotX = verPlot->axis(QCPAxis::atRight);
    auto verPlotY = verPlot->axis(QCPAxis::atBottom);
    verPlotX->setVisible(true);
    verPlotY->setVisible(true);

    horPlotX->setTickLabelColor(m_xTickLabelColor);
    verPlotX->setTickLabelColor(m_yTickLabelColor);
    horPlotX->setTickLabelFont(m_xTickLabelFont);
    verPlotX->setTickLabelFont(m_yTickLabelFont);

    horPlotX->setBasePen(QPen(m_axisColor, m_axisWidth));
    verPlotX->setBasePen(QPen(m_axisColor, m_axisWidth));

    horPlotX->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    horPlotY->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    horPlotY->grid()->setVisible(true);
    verPlotX->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    verPlotY->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));

    horPlotX->setLabelColor(m_xAxisLabelColor);
    verPlotX->setLabelColor(m_yAxisLabelColor);

    connect(m_customPlot, &QCustomPlot::mouseRelease, this, &PlotDoppler::onMouseRelease);
    connect(m_customPlot, &QCustomPlot::mouseMove, this, &PlotDoppler::onShowToolTips);
}

void PlotDoppler::getYToValueVecByX(double x, QVector<double>& yVec, QVector<double>& dataVec)
{
    const auto& pairList = m_verticalDataHash.values(x);
    yVec.clear();
    dataVec.clear();
    for(const auto& pair : pairList)
    {
        yVec.append(pair.first);
        dataVec.append(pair.second);
    }
}

void PlotDoppler::getXToValueVecByY(double y, QVector<double>& xVec, QVector<double>& dataVec)
{
    const auto& pairList = m_horizonDataHash.values(y);
    xVec.clear();
    dataVec.clear();
    for(const auto& pair : pairList)
    {
        xVec.append(pair.first);
        dataVec.append(pair.second);
    }
}

void PlotDoppler::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
    {
        return;
    }
    auto data = getDataPairs().last();
    int32_t eid = data->getEntityIDX();
    DataManagerInstance->getDopplerDataByTime(
        eid, secs, m_rangeList, m_timeList, m_dataHash, m_horizonDataHash, m_verticalDataHash);
    if(m_dataHash.isEmpty())
    {
        return;
    }
    // 默认用第一个数据的切片显示水平、垂直图表
    const auto& pair = (*m_dataHash.keyBegin());
    int32_t rangeIndex = pair.first;
    int32_t timeIndex = pair.second;

    m_slicePoint.setX(m_timeList.at(timeIndex));
    m_slicePoint.setY(m_rangeList.at(rangeIndex));
    updateGraphByDataPair(data, secs);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotDoppler::updateGraphByDataPair(DataPair* data, double curSecs)
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
        m_colorMap->data()->setSize(nx, ny);
        m_colorMap->data()->setRange(QCPRange(m_rangeList.first(), m_rangeList.last()),
                                     QCPRange(m_timeList.first(), m_timeList.last()));

        for(int xIndex = 0; xIndex < nx; ++xIndex)
        {
            for(int yIndex = 0; yIndex < ny; ++yIndex)
            {
                auto coord = qMakePair(xIndex, yIndex);
                double voltage = m_dataHash.value(coord);
                m_colorMap->data()->setCell(xIndex, yIndex, voltage);
            }
        }

        m_colorMap->rescaleDataRange();
        updateAScopesBySlicePoint(m_slicePoint);
        m_customPlot->rescaleAxes();
    }
    else
    {
        m_colorMap->setVisible(false);
        m_colorScale->setVisible(false);
        m_customPlot->graph(1)->setVisible(false);
        m_customPlot->graph(2)->setVisible(false);
    }
}

void PlotDoppler::updateAScopesBySlicePoint(const QPointF& point)
{

                // 水平曲线图加载数据
    QVector<double> hXDatas;
    QVector<double> hValues;
    getXToValueVecByY(point.x(), hXDatas, hValues);
    m_customPlot->graph(1)->setData(hXDatas, hValues, true);

                // 垂直曲线图加载数据
    QVector<double> vYDatas;
    QVector<double> vValues;
    getYToValueVecByX(point.y(), vYDatas, vValues);
    m_customPlot->graph(2)->setData(vYDatas, vValues, true);
    m_customPlot->graph(1)->setVisible(true);
    m_customPlot->graph(2)->setVisible(true);
}

void PlotDoppler::initColorRangeMap()
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

bool PlotDoppler::isValidDataPoint(double time, double range)
{
    return m_horizonDataHash.contains(time) && m_verticalDataHash.contains(range);
}

void PlotDoppler::setAxisColorWidth(const QColor& color, int32_t width)
{
    m_axisColor = color;
    m_axisWidth = width;
    m_customPlot->graph(1)->keyAxis()->setBasePen(QPen(color, width));
    m_customPlot->graph(1)->valueAxis()->setBasePen(QPen(color, width));
    m_customPlot->graph(2)->keyAxis()->setBasePen(QPen(color, width));
    m_customPlot->graph(2)->valueAxis()->setBasePen(QPen(color, width));
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotDoppler::setGridColorWidth(const QColor& color, int32_t width)
{
    m_gridColor = color;
    m_gridWidth = width;
    m_customPlot->graph(1)->keyAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth));
    m_customPlot->graph(1)->valueAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth));
    m_customPlot->graph(2)->keyAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth));
    m_customPlot->graph(2)->valueAxis()->grid()->setPen(QPen(m_gridColor, m_gridWidth));
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotDoppler::setHorzGrids(int32_t count)
{
    m_horzGrids = count;
    if(count == 0)
    {
        m_customPlot->graph(1)->keyAxis()->grid()->setVisible(false);
        m_customPlot->graph(2)->keyAxis()->grid()->setVisible(false);
    }
    else
    {
        m_customPlot->graph(1)->keyAxis()->grid()->setVisible(true);
        m_customPlot->graph(2)->keyAxis()->grid()->setVisible(true);

        m_customPlot->graph(1)->keyAxis()->ticker()->setTickCount(m_horzGrids);
        m_customPlot->graph(2)->keyAxis()->ticker()->setTickCount(m_horzGrids);
    }
    m_customPlot->replot();
}

void PlotDoppler::setVertGrids(int32_t count)
{
    m_vertGrids = count;

    if(count == 0)
    {
        m_customPlot->graph(1)->valueAxis()->grid()->setVisible(false);
        m_customPlot->graph(2)->valueAxis()->grid()->setVisible(false);
    }
    else
    {
        m_customPlot->graph(1)->valueAxis()->grid()->setVisible(true);
        m_customPlot->graph(2)->valueAxis()->grid()->setVisible(true);

        m_customPlot->graph(1)->valueAxis()->ticker()->setTickCount(m_vertGrids);
        m_customPlot->graph(2)->valueAxis()->ticker()->setTickCount(m_vertGrids);
    }
    m_customPlot->replot();
}

void PlotDoppler::setGridFillColor(const QColor &color)
{
    m_gridFillColor = color;


    m_customPlot->axisRect(0)->setBackground(color);
    m_customPlot->axisRect(1)->setBackground(color);
    m_customPlot->axisRect(2)->setBackground(color);

    m_customPlot->replot();
}

bool PlotDoppler::getIsShowToolTip() const
{
    return m_isShowToolTip;
}

void PlotDoppler::setIsShowToolTip(bool isShowToolTip)
{
    m_isShowToolTip = isShowToolTip;
}

QMap<double, QColor> PlotDoppler::getColorRangeMap() const
{
    return m_colorRangeMap;
}

void PlotDoppler::setColorRangeMap(const QMap<double, QColor>& colorRangeMap)
{
    QCPColorGradient m_colorGradient;
    m_colorRangeMap = colorRangeMap;
    auto colorKeys = colorRangeMap.keys();
    for(auto value : colorKeys)
    {
        m_colorGradient.setColorStopAt(value, colorRangeMap.value(value));
    }
    m_colorScale->setGradient(m_colorGradient);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotDoppler::onMouseRelease(QMouseEvent* event)
{
    if(getDataPairs().isEmpty())
    {
        return;
    }

    double range = m_customPlot->graph(0)->keyAxis()->pixelToCoord(event->pos().x());
    double time = m_customPlot->graph(0)->valueAxis()->pixelToCoord(event->pos().y());
    if(isValidDataPoint(time, range))
    {
        m_slicePoint.setX(time);
        m_slicePoint.setY(range);
        updateAScopesBySlicePoint(m_slicePoint);
        m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
    }
}

void PlotDoppler::onShowToolTips(QMouseEvent* event)
{
    if(m_isShowToolTip)
    {
        double range = m_customPlot->graph(0)->keyAxis()->pixelToCoord(event->pos().x());
        double time = m_customPlot->graph(0)->valueAxis()->pixelToCoord(event->pos().y());
        m_customPlot->setToolTip(QString("time:%1\nrange:%2").arg(time).arg(range));
    }
    else
    {
        m_customPlot->setToolTip("");
    }
}
