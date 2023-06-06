#include "PlotDoppler.h"
#include "DataManager.h"
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

    horPlotX->setTickLabelColor(m_tickLabelColor);
    verPlotX->setTickLabelColor(m_tickLabelColor);
    horPlotX->setTickLabelFont(m_tickLabelFont);
    verPlotX->setTickLabelFont(m_tickLabelFont);

    horPlotX->setBasePen(QPen(m_axisColor, m_axisWidth));
    verPlotX->setBasePen(QPen(m_axisColor, m_axisWidth));

    horPlotX->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    horPlotY->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    horPlotY->grid()->setVisible(true);
    verPlotX->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
    verPlotY->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));

    horPlotX->setLabelColor(m_axisLabelColor);
    verPlotX->setLabelColor(m_axisLabelColor);
    m_customPlot->setBackground(m_outerFillColor);
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
    updateGraphByDataPair(data);
    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotDoppler::updateGraphByDataPair(DataPair* data)
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
}
