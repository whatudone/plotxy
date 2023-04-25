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

#ifdef TEST_SCOPE_DATA
    loadCustomData();
#endif
}

PlotRTI::~PlotRTI() {}

void PlotRTI::initPlot()
{
	m_customPlot = new QCustomPlot(this);
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
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

void PlotRTI::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_customPlot->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotRTI::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_customPlot->yAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotRTI::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_customPlot->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotRTI::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_customPlot->yAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotRTI::setTitle(QString& str)
{
	m_title = str;
	update();
}

void PlotRTI::setTitleColor(QColor& color)
{
	m_titleColor = color;
	update();
}

void PlotRTI::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotRTI::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotRTI::setTitleFont(QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotRTI::setTitleVisible(bool show)
{
	m_titleVisible = show;
    update();
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

void PlotRTI::setxAxisLabel(QString& str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->replot();
}

void PlotRTI::setyAxisLabel(QString& str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotRTI::setAxisLabelColor(QColor& color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisColor);
	m_customPlot->yAxis->setLabelColor(m_axisColor);
	m_customPlot->replot();
}

void PlotRTI::setAxisLabelFont(QFont& font)
{
	m_axisLabelFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->replot();
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

void PlotRTI::setHorzGrids(uint count)
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

void PlotRTI::setVertGrids(uint count)
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

void PlotRTI::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotRTI::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotRTI::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
    // 	m_customPlot->axisRect()->setBackground(color);
    // 	m_customPlot->replot();
}

void PlotRTI::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);
	m_customPlot->replot();
}

void PlotRTI::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->replot();
}

void PlotRTI::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->replot();
}

void PlotRTI::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotRTI::setGridStyle(GridStyle style)
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

void PlotRTI::setGridDensity(GridDensity density) {}
