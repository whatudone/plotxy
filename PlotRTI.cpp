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
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFontSize = 16;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
	m_titleVisible = true;

	m_axisLabelFont.setFamily("Microsoft YaHei");
	m_axisLabelFont.setPointSizeF(10.0);
	m_xAxisLabel = "Range(m)";
	m_yAxisLabel = "Time(s)";

	m_leftPadding = 10;
	m_rightPadding = 30;
	m_topPadding = 20;
	m_bottomPadding = 10;

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
	//m_customPlot->plotLayout()->addElement(0, 1, m_colorScale);
	//m_customPlot->plotLayout()->setColumnSpacing(0);
	//m_colorScale->setType(QCPAxis::atRight);
	m_colorMap->setColorScale(m_colorScale);
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();

	m_marginGroup = new QCPMarginGroup(m_customPlot);
    //	m_customPlot->axisRect()->setMinimumMargins(QMargins(2, 2, 2, 2));
	m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
	m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);

    //	m_customPlot->rescaleAxes();
}

void PlotRTI::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	QRectF rect = fm.boundingRect(m_title);

    if(m_titleVisible)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
        painter.fillRect(
            (width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
        painter.drawText(
            QPoint((width + m_leftPadding - m_rightPadding - w) / 2, as + m_topPadding), m_title);
	}

    m_customPlot->setGeometry(m_leftPadding,
                              h + m_topPadding,
                              width - m_leftPadding - m_rightPadding,
                              height - h - m_topPadding - m_bottomPadding);
    PlotItemBase::paintEvent(event);
}

void PlotRTI::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
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

void PlotRTI::setCoordRangeX(double lower, double upper)
{
    if(m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
    //	m_customPlot->replot();
}

void PlotRTI::setCoordRangeY(double lower, double upper)
{
    if(m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
    //	m_customPlot->replot();
}

void PlotRTI::setHorzGrids(uint count)
{
    if(m_horzGrids == count || count < 0)
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
    if(m_vertGrids == count || count < 0)
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
