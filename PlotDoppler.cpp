#include "PlotDoppler.h"

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
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFontSize = 16;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
	m_titleVisible = true;

	m_axisLabelFont.setFamily("Microsoft YaHei");
	m_axisLabelFont.setPointSizeF(10.0);
	m_xAxisLabel = "Range(m)";
	m_yAxisLabel = "Voltage(V)";

	m_leftPadding = 10;
	m_rightPadding = 20;
	m_topPadding = 0;
	m_bottomPadding = 10;

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
}

PlotDoppler::~PlotDoppler() {}

void PlotDoppler::initPlot()
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
	m_customPlot->plotLayout()->addElement(0, 1, m_colorScale);
	m_customPlot->plotLayout()->setColumnSpacing(0);
	m_colorScale->setType(QCPAxis::atRight);
	m_colorMap->setColorScale(m_colorScale);
	m_colorMap->setGradient(QCPColorGradient::gpJet);
	m_colorMap->rescaleDataRange();

	m_marginGroup = new QCPMarginGroup(m_customPlot);
	m_customPlot->axisRect()->setMinimumMargins(QMargins(0, 15, 0, 0));
	m_customPlot->axisRect()->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);
	m_colorScale->setMarginGroup(QCP::msBottom | QCP::msTop, m_marginGroup);

    //	m_customPlot->rescaleAxes();

	m_horizon_AScope = new PlotAScope(this);
    m_horizon_AScope->setTitle(QString(""));
	m_horizon_AScope->setTitleFont(QFont("Microsoft YaHei", 1));
	m_horizon_AScope->setPaddings(0, 0, 0, 0);
	m_horizon_AScope->setTitleVisible(false);
	m_horizon_AScope->setxAxisLabel(QString(""));
	m_horizon_AScope->setyAxisLabel(QString(""));
	m_horizon_AScope->setAxisTickLabelShow(false, PlotAScope::yAxis);
    m_horizon_AScope->setMinimumMargins(QMargins(15, 1, 15, 15));
	m_horizon_AScope->setVertGrids(m_vertGrids);
    //	m_horizon_AScope->setHorzGrids(m_horzGrids);
	m_horizon_AScope->setOuterFillColor(m_outerFillColor);
	m_horizon_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_horizon_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_horizon_AScope->setGridFillColor(m_gridFillColor);
	m_horizon_AScope->setGridStyle(GridStyle(m_gridStyle - 1));
	m_horizon_AScope->setTickLabelColor(m_tickLabelColor);
	m_horizon_AScope->setTickLabelFont(m_tickLabelFont);
	m_horizon_AScope->setTickLabelFontSize(m_tickLabelFontSize);

	m_vertical_AScope = new PlotAScope(this);
	m_vertical_AScope->setTitle(QString(""));
	m_vertical_AScope->setTitleFont(QFont("Microsoft YaHei", 1));
	m_vertical_AScope->setPaddings(0, 0, 0, 0);
	m_vertical_AScope->setTitleVisible(false);
	m_vertical_AScope->setxAxisLabel(QString(""));
	m_vertical_AScope->setyAxisLabel(QString(""));
	m_vertical_AScope->setAxisTickLabelShow(false, PlotAScope::xAxis);
	m_vertical_AScope->setMinimumMargins(QMargins(15, 15, 1, 15));
    //	m_vertical_AScope->setVertGrids(m_vertGrids);
	m_vertical_AScope->setHorzGrids(m_horzGrids);
	m_vertical_AScope->setOuterFillColor(m_outerFillColor);
	m_vertical_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_vertical_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_vertical_AScope->setGridFillColor(m_gridFillColor);
	m_vertical_AScope->setGridStyle(GridStyle(m_gridStyle - 1));
	m_vertical_AScope->setTickLabelColor(m_tickLabelColor);
	m_vertical_AScope->setTickLabelFont(m_tickLabelFont);
	m_vertical_AScope->setTickLabelFontSize(m_tickLabelFontSize);

	setCoordRangeX(m_coordBgn_x, m_coordEnd_x);
	setCoordRangeY(m_coordBgn_y, m_coordEnd_y);
}

void PlotDoppler::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();

	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();

    if(m_titleVisible)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
        painter.fillRect(
            (width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
        painter.drawText(
            QPoint((width + m_leftPadding - m_rightPadding - w) / 2, as + m_topPadding), m_title);
	}

    int verWidth = (width - m_leftPadding - m_rightPadding) * 0.2;
    int horHeight = (height - h - m_topPadding - m_bottomPadding) * 0.2;
	int plotWidth = width - m_leftPadding - m_rightPadding - verWidth;
	int plotHeight = height - h - m_topPadding - m_bottomPadding - horHeight;

	m_vertical_AScope->resize(verWidth, plotHeight + 15);
    m_vertical_AScope->setGeometry(m_leftPadding, h + m_topPadding, verWidth, plotHeight + 15);

	m_horizon_AScope->resize(plotWidth - m_colorScale->barWidth() + 15, horHeight);
    m_horizon_AScope->setGeometry(m_leftPadding + verWidth - 15,
                                  h + m_topPadding + plotHeight,
                                  plotWidth - m_colorScale->barWidth() + 15,
                                  horHeight);

    m_customPlot->setGeometry(m_leftPadding + verWidth, h + m_topPadding, plotWidth, plotHeight);
    PlotItemBase::paintEvent(event);
}

void PlotDoppler::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotDoppler::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_horizon_AScope->setUnitsShowX(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_vertical_AScope->setUnitsShowY(on);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_horizon_AScope->setUnitsX(units);
	m_customPlot->replot();
}

void PlotDoppler::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_vertical_AScope->setUnitsY(units);
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
	m_horizon_AScope->setOuterFillColor(color);
	m_vertical_AScope->setOuterFillColor(color);
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
	m_horizon_AScope->setCoordRangeX(lower, upper);

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
	m_horizon_AScope->setCoordRangeX(lower, upper);

    //	m_customPlot->rescaleAxes();
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
	m_vertical_AScope->setCoordRangeY(lower, upper);

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
	m_vertical_AScope->setCoordRangeY(lower, upper);

    //	m_customPlot->rescaleAxes();
	m_customPlot->replot();
}

void PlotDoppler::getCoordRangeX(double& lower, double& upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotDoppler::getCoordRangeY(double& lower, double& upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}

void PlotDoppler::setHorzGrids(uint count)
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
    //	m_horizon_AScope->setHorzGrids(m_horzGrids);
	m_vertical_AScope->setHorzGrids(m_horzGrids);
	m_customPlot->replot();
}

void PlotDoppler::setVertGrids(uint count)
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
	m_horizon_AScope->setVertGrids(m_vertGrids);
    //	m_vertical_AScope->setVertGrids(m_vertGrids);
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

	m_horizon_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_vertical_AScope->setAxisColorWidth(m_axisColor, m_axisWidth);
	m_customPlot->replot();
}

void PlotDoppler::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));

	m_horizon_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_vertical_AScope->setGridColorWidth(m_gridColor, m_gridWidth);
	m_customPlot->replot();
}

void PlotDoppler::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
	m_horizon_AScope->setGridFillColor(color);
	m_vertical_AScope->setGridFillColor(color);
}

void PlotDoppler::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);

	m_horizon_AScope->setGridVisible(enable);
	m_vertical_AScope->setGridVisible(enable);
	m_customPlot->replot();
}

void PlotDoppler::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);

	m_horizon_AScope->setTickLabelColor(color);
	m_vertical_AScope->setTickLabelColor(color);
	m_customPlot->replot();
}

void PlotDoppler::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);

	m_horizon_AScope->setTickLabelFont(font);
	m_vertical_AScope->setTickLabelFont(font);
	m_customPlot->replot();
}

void PlotDoppler::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);

	m_horizon_AScope->setTickLabelFontSize(size);
	m_vertical_AScope->setTickLabelFontSize(size);
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

	m_horizon_AScope->setGridStyle(style);
	m_vertical_AScope->setGridStyle(style);
	m_customPlot->replot();
}

void PlotDoppler::setGridDensity(GridDensity density) {}
