#include "PlotRTI.h"

int PlotRTI::m_instanceCount = 1;
PlotRTI::PlotRTI(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("RTI%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
    m_title = "RTI Display";

    m_axisLabelFont.setFamily("Microsoft YaHei");
    m_axisLabelFont.setPointSizeF(10.0);
    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Time(s)";

    m_coordBgn_x = 0;

    m_coordEnd_x = 100;
    m_coordBgn_y = 0;
    m_coordEnd_y = 100;

    m_coordEnd_x = 5;
    m_coordBgn_y = 0;
    m_coordEnd_y = 5;

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
    m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
    m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
    m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

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

void PlotRTI::loadCustomData()
{
    QString dataFileName = ":/AScope.csv";
    QFile file(dataFileName);

    QVector<double> rangeDatas;
    QVector<int64_t> timeDatas;

    if(file.open(QFile::Text | QFile::ReadOnly))
    {
        do
        {
            QString data = file.readLine();
            if(data.isEmpty())
            {
                break;
            }

            QStringList dataList = data.split(",", QString::SkipEmptyParts);
            if(dataList.size() != 3)
            {
                break;
            }

            if(dataList.at(0) == "Time")
            {
                continue;
            }
            int64_t time =
                QDateTime::fromString(dataList.at(0), "yyyy/MM/dd hh:mm:ss").toSecsSinceEpoch();
            double range = dataList.at(1).toDouble();
            double voltage = dataList.at(2).toDouble();
            if(!timeDatas.contains(time))
            {
                timeDatas.append(time);
            }
            if(!rangeDatas.contains(range))
            {
                rangeDatas.append(range);
            }

            m_dataMap.insert(qMakePair(range, time), voltage);
        } while(true);
    }
    file.close();
    std::sort(rangeDatas.begin(), rangeDatas.end());
    std::sort(timeDatas.begin(), timeDatas.end());
    int nx = rangeDatas.size();
    int ny = timeDatas.size();
    m_colorMap->data()->setSize(nx, ny);
    m_colorMap->data()->setRange(QCPRange(rangeDatas.first(), rangeDatas.last()),
                                 QCPRange(timeDatas.first(), timeDatas.last()));

    for(int xIndex = 0; xIndex < nx; ++xIndex)
    {
        for(int yIndex = 0; yIndex < ny; ++yIndex)
        {
            auto coord = qMakePair(rangeDatas.at(xIndex), timeDatas.at(yIndex));
            double voltage = m_dataMap.value(coord);
            m_colorMap->data()->setCell(xIndex, yIndex, voltage);
        }
    }

    m_colorMap->rescaleDataRange();

    m_customPlot->rescaleAxes();
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

void PlotRTI::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
        return;
    // 按照目录的理解，RTI暂时只能同事绘制一个数据对的数据
    auto lastDataPair = getDataPairs().last();
    updateGraph(secs, lastDataPair);

    m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotRTI::updateGraph(double secs, DataPair* data) {}
