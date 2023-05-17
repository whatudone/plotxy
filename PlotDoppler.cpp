#include "PlotDoppler.h"
#include "Utils.h"
#include <QGridLayout>
int PlotDoppler::m_instanceCount = 1;
PlotDoppler::PlotDoppler(QWidget* parent)
    : PlotItemBase(parent)
{
    QString name = QString("Doppler%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

    m_outerFillColor = Qt::black;
    m_gridFillColor = Qt::black;
    m_title = "Range Doppler";

    m_axisLabelFont.setFamily("Microsoft YaHei");
    m_axisLabelFont.setPointSizeF(10.0);
    m_xAxisLabel = "Range(m)";
    m_yAxisLabel = "Voltage(V)";

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
#ifdef TEST_SCOPE_DATA
    loadCustomData();
#endif
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

void PlotDoppler::loadCustomData()
{
    QString dataFileName = ":/AScope.csv";
    QFile file(dataFileName);

    QVector<double> rangeDatas;
    QVector<double> timeDatas;

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
            double time = static_cast<double>(
                QDateTime::fromString(dataList.at(0), "yyyy/MM/dd hh:mm:ss").toSecsSinceEpoch());
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
            // 对于同一键值，会插入多个
            m_horizonDataMap.insert(time, qMakePair(range, voltage));
            m_verticalDataMap.insert(range, qMakePair(time, voltage));
        } while(true);
    }
    file.close();
    std::sort(rangeDatas.begin(), rangeDatas.end());
    std::sort(timeDatas.begin(), timeDatas.end());

    int nx = rangeDatas.size();
    int ny = timeDatas.size();
    // 中间主图加载数据
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

    // 默认用第一个数据的切片显示水平、垂直图表
    const auto& pair = m_dataMap.firstKey();
    // 水平曲线图加载数据
    QVector<double> hXDatas;
    QVector<double> hValues;
    getXToValueVecByY(pair.second, hXDatas, hValues);
    m_customPlot->graph(1)->setData(hXDatas, hValues);
    m_customPlot->graph(1)->rescaleAxes();

    // 垂直曲线图加载数据
    QVector<double> vYDatas;
    QVector<double> vValues;
    getYToValueVecByX(pair.first, vYDatas, vValues);
    m_customPlot->graph(2)->setData(vYDatas, vValues);
    m_customPlot->graph(2)->rescaleAxes();

    m_customPlot->rescaleAxes();
    m_customPlot->replot();
}

void PlotDoppler::getYToValueVecByX(double x, QVector<double>& yVec, QVector<double>& dataVec)
{
    const auto& pairList = m_verticalDataMap.values(x);
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
    const auto& pairList = m_horizonDataMap.values(y);
    xVec.clear();
    dataVec.clear();
    for(const auto& pair : pairList)
    {
        xVec.append(pair.first);
        dataVec.append(pair.second);
    }
}
