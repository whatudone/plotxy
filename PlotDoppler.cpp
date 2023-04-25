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
#ifdef TEST_SCOPE_DATA
    loadCustomData();
#endif
}

PlotDoppler::~PlotDoppler() {}

void PlotDoppler::initPlot()
{
	m_customPlot = new QCustomPlot(this);

    m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, Qt::DotLine));

	m_customPlot->xAxis->setTickLabels(false);
	m_customPlot->yAxis->setTickLabels(false);
	m_customPlot->xAxis->setVisible(false);
	m_customPlot->yAxis->setVisible(false);

	m_customPlot->setBackground(m_outerFillColor);
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

	m_colorMap = new QCPColorMap(m_customPlot->xAxis, m_customPlot->yAxis);
	m_colorScale = new QCPColorScale(m_customPlot);
    m_customPlot->plotLayout()->insertColumn(0);
    m_customPlot->plotLayout()->addElement(0, 0, m_colorScale);
	m_customPlot->plotLayout()->setColumnSpacing(0);
    m_colorScale->setType(QCPAxis::atLeft);
	m_colorMap->setColorScale(m_colorScale);
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();

    auto marginGroup = new QCPMarginGroup(m_customPlot);
	m_customPlot->axisRect()->setMinimumMargins(QMargins(0, 15, 0, 0));
    // 图表内部布局
    m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);
    m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, marginGroup);

    m_verticalAScope = new QCustomPlot(this);
    m_verticalAScope->yAxis->setVisible(false);
    m_verticalAScope->yAxis2->setVisible(true);
    m_verticalAScope->xAxis->setTickLabels(false);
    // Doppler图表无法同时显示多个数据对，所以每个图表默认一个图例
    m_verticalAScope->addGraph(m_verticalAScope->yAxis2, m_verticalAScope->xAxis);

    m_horizonAScope = new QCustomPlot(this);
    m_horizonAScope->yAxis->setTickLabels(false);
    m_horizonAScope->addGraph(m_horizonAScope->xAxis, m_horizonAScope->yAxis);
    // 图表之间的布局
    QGridLayout* pLayout = new QGridLayout(this);
    pLayout->addWidget(m_customPlot, 0, 0);
    pLayout->addWidget(m_verticalAScope, 0, 1);
    pLayout->addWidget(m_horizonAScope, 1, 0);
    pLayout->setColumnStretch(0, 4);
    pLayout->setColumnStretch(1, 1);
    pLayout->setRowStretch(0, 4);
    pLayout->setRowStretch(1, 1);
    // top需要考虑标题栏的字体高度，先用60
    pLayout->setContentsMargins(m_leftPadding, 60, m_rightPadding, m_bottomPadding);
    setLayout(pLayout);

	setCoordRangeX(m_coordBgn_x, m_coordEnd_x);
	setCoordRangeY(m_coordBgn_y, m_coordEnd_y);
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
    m_customPlot->rescaleAxes();
    // 默认用第一个数据的切片显示水平、垂直图表
    const auto& pair = m_dataMap.firstKey();
    // 水平曲线图加载数据
    QVector<double> hXDatas;
    QVector<double> hValues;
    getXToValueVecByY(pair.second, hXDatas, hValues);
    m_horizonAScope->graph(0)->setData(hXDatas, hValues);
    m_horizonAScope->rescaleAxes();
    m_horizonAScope->replot();
    // 垂直曲线图加载数据
    QVector<double> vYDatas;
    QVector<double> vValues;
    getYToValueVecByX(pair.first, vYDatas, vValues);
    m_verticalAScope->graph(0)->setData(vYDatas, vValues);
    m_verticalAScope->rescaleAxes();
    m_verticalAScope->replot();
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

void PlotDoppler::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
    m_horizonAScope->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
    m_verticalAScope->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsX(const QString& units)
{
	m_units_x = units;
    m_horizonAScope->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsY(const QString& units)
{
	m_units_y = units;
    m_verticalAScope->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotDoppler::setTitle(QString& str)
{
	m_title = str;
	update();
}

void PlotDoppler::setTitleColor(QColor& color)
{
	m_titleColor = color;
	update();
}

void PlotDoppler::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotDoppler::setTitleFont(QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotDoppler::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotDoppler::setTitleVisible(bool show)
{
	m_titleVisible = show;
    update();
}

void PlotDoppler::onPlotMouseEventEnable(bool on)
{
    m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
    m_horizonAScope->setAttribute(Qt::WA_TransparentForMouseEvents, on);
    m_verticalAScope->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotDoppler::setxAxisLabel(QString& str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->replot();
}

void PlotDoppler::setyAxisLabel(QString& str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotDoppler::setAxisLabelColor(QColor& color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->replot();
}

void PlotDoppler::setAxisLabelFont(QFont& font)
{
	m_axisLabelFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->replot();
}

void PlotDoppler::setAxisVisible(bool on, AxisType type)
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

void PlotDoppler::setAxisTickLabelShow(bool on, AxisType type)
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

void PlotDoppler::setOuterFillColor(QColor color)
{
	m_outerFillColor = color;
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, m_outerFillColor);
	this->setPalette(palette);

	m_customPlot->setBackground(color);
    m_horizonAScope->setBackground(color);
    m_verticalAScope->setBackground(color);
	m_customPlot->replot();
}

void PlotDoppler::setCoordRangeX(double lower, double upper)
{
    if(m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
    m_horizonAScope->xAxis->setRange(lower, upper);

    m_customPlot->rescaleAxes();
    m_customPlot->replot();
}

void PlotDoppler::setCoordRangeY(double lower, double upper)
{
    if(m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
    m_verticalAScope->xAxis->setRange(lower, upper);

    m_customPlot->rescaleAxes();
	m_customPlot->replot();
}

void PlotDoppler::setHorzGrids(uint count)
{
    if(m_horzGrids == count)
	{
		return;
	}
	m_horzGrids = count;
    if(count == 0)
	{
		m_customPlot->yAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->yAxis->grid()->setVisible(true);
		m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
	}

	m_customPlot->replot();
}

void PlotDoppler::setVertGrids(uint count)
{
    if(m_vertGrids == count)
	{
		return;
	}
	m_vertGrids = count;
    if(count == 0)
	{
		m_customPlot->xAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->xAxis->grid()->setVisible(true);
		m_customPlot->xAxis->ticker()->setTickCount(m_vertGrids);
	}

	m_customPlot->replot();
}

void PlotDoppler::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));

    m_horizonAScope->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_horizonAScope->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));

    m_verticalAScope->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
    m_verticalAScope->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotDoppler::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_horizonAScope->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_horizonAScope->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_verticalAScope->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
    m_verticalAScope->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotDoppler::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
}

void PlotDoppler::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);

	m_customPlot->replot();
}

void PlotDoppler::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);

	m_customPlot->replot();
}

void PlotDoppler::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);

	m_customPlot->replot();
}

void PlotDoppler::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotDoppler::setGridStyle(GridStyle style)
{
    switch(style)
	{
	case GridStyle::SOLIDLINE:
		m_gridStyle = Qt::SolidLine;
		break;
	case GridStyle::DASHLINE:
		m_gridStyle = Qt::DashLine;
		break;
	case GridStyle::DOTLINE:
		m_gridStyle = Qt::DotLine;
		break;
	case GridStyle::DASHDOTLINE:
		m_gridStyle = Qt::DashDotLine;
		break;
	default:
		m_gridStyle = Qt::SolidLine;
		break;
	}
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

	m_customPlot->replot();
}

void PlotDoppler::setGridDensity(GridDensity density) {}
