#include "PlotAScope.h"

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
	m_bottomPadding = 20;

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

PlotAScope::~PlotAScope() {}

void PlotAScope::initPlot()
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
	m_customPlot->axisRect()->setBackground(m_gridFillColor);
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

	m_customPlot->replot();
}

void PlotAScope::onGetCurrentSeconds(double secs)
{
    if(getDataPairs().isEmpty())
	{
		return;
	}
    int isize = getDataPairs().size();
    for(int i = 0; i < isize; ++i)
	{
        QString xcolumn = getDataPairs().at(i)->getDataPair().first;
        QString ycolumn = getDataPairs().at(i)->getDataPair().second;
	}
}

void PlotAScope::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();
	//»­±Ê
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

    m_customPlot->setGeometry(m_leftPadding,
                              h + m_topPadding,
                              width - m_leftPadding - m_rightPadding,
                              height - h - m_topPadding - m_bottomPadding);
    PlotItemBase::paintEvent(event);
}

void PlotAScope::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotAScope::setPaddings(double top, double bottom, double left, double right)
{
	m_topPadding = top;
	m_bottomPadding = bottom;
	m_leftPadding = left;
	m_rightPadding = right;
	update();
}

void PlotAScope::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_customPlot->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotAScope::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_customPlot->yAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotAScope::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_customPlot->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotAScope::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_customPlot->yAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotAScope::setTitle(const QString& str)
{
	m_title = str;
	update();
}

void PlotAScope::setTitleColor(const QColor& color)
{
	m_titleColor = color;
	update();
}

void PlotAScope::setTitleFont(const QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotAScope::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotAScope::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotAScope::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotAScope::setxAxisLabel(const QString& str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
	m_customPlot->replot();
}

void PlotAScope::setyAxisLabel(const QString& str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotAScope::setAxisLabelColor(QColor& color)
{
	m_axisColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->replot();
}

void PlotAScope::setAxisLabelFont(QFont& font)
{
	m_axisLabelFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
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

void PlotAScope::setCoordRangeX(double lower, double upper)
{
    if(m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotAScope::setCoordRangeY(double lower, double upper)
{
    if(m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotAScope::setHorzGrids(uint count)
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

void PlotAScope::setVertGrids(uint count)
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

void PlotAScope::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotAScope::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotAScope::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
	m_customPlot->axisRect()->setBackground(color);
	m_customPlot->replot();
}

void PlotAScope::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);
	m_customPlot->replot();
}

void PlotAScope::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->replot();
}

void PlotAScope::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->replot();
}

void PlotAScope::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotAScope::setGridStyle(GridStyle style)
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

void PlotAScope::setGridDensity(GridDensity density) {}

void PlotAScope::setMinimumMargins(const QMargins& margins)
{
	m_customPlot->axisRect()->setMinimumMargins(margins);
}
