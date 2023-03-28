#include "PlotPolar.h"
#include "DataManager.h"
#include <qlabel.h>

int PlotPolar::m_instanceCount = 1;
PlotPolar::PlotPolar(QWidget* parent)
    : PlotItemBase(parent)
{
	QString name = QString("Polar%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_outerFillColor = Qt::black;
	m_gridFillColor = Qt::black;
	m_title = "Polar";
	m_titleColor = Qt::white;
	m_titleFontSize = 16;
	m_titleFillColor = Qt::black;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
	m_titleVisible = true;

    m_units_x = QString("X");
    m_units_y = QString("Y");
	m_showUnits_x = true;
	m_showUnits_y = true;

	m_angularRange_lower = 0.0;
	m_angularRange_upper = 360.0;
	m_radialRange_lower = 0.0;
	m_radialRange_upper = 2000.0;

	m_horzGrids = (m_angularRange_upper - m_angularRange_lower) / 30.0;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);

	m_gridVisible = true;
	m_tickLabelColor = Qt::white;
	m_tickLabelFontSize = 8;
	m_tickLabelFont.setFamily("Microsoft YaHei");
	m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);
	m_gridStyle = Qt::SolidLine;
	m_gridDensity = GridDensity::LESS;

	initPlot();
}

PlotPolar::~PlotPolar() {}

void PlotPolar::initPlot()
{
	m_customPlot = new QCustomPlot(this);
    QHBoxLayout* pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_customPlot);
    setLayout(pLayout);
	m_customPlot->setSelectionRectMode(QCP::srmNone);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);

	m_customPlot->plotLayout()->clear();

	m_angularAxis = new QCPPolarAxisAngular(m_customPlot);
	m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->plotLayout()->addElement(0, 0, m_angularAxis);
	m_customPlot->setBackground(m_outerFillColor);
    //	m_angularAxis->setBackground(m_gridFillColor);

	//	m_customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	m_angularAxis->setRangeDrag(false);
	m_angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);
	m_angularAxis->setFormat(m_units_x);
	m_angularAxis->setTickLabelColor(m_tickLabelColor);
	m_angularAxis->setTickLabelFont(m_tickLabelFont);
	m_angularAxis->setTickPen(QPen(m_gridColor, 1));

	m_angularAxis->radialAxis()->setTickLabelColor(m_tickLabelColor);
	m_angularAxis->radialAxis()->setTickLabelFont(m_tickLabelFont);
	m_angularAxis->radialAxis()->setFormat(m_units_y);
	m_angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
	m_angularAxis->radialAxis()->setTickLabelRotation(0);
	m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_angularAxis->radialAxis()->setAngle(0);

	//angularAxis->radialAxis()->setNumberFormat("e");
	m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_angularAxis->grid()->setSubGridType(QCPPolarGrid::gtNone);

	m_angularAxis->setRange(m_angularRange_lower, m_angularRange_upper);
	m_angularAxis->radialAxis()->setRange(m_radialRange_lower, m_radialRange_upper);
	m_angularAxis->radialAxis()->setRangeReversed(false);

	m_vertGrids = m_angularAxis->radialAxis()->ticker()->tickCount();
	m_customPlot->replot();
}

void PlotPolar::setOuterFillColor(QColor color)
{
    if(m_outerFillColor == color)
		return;

	m_outerFillColor = color;
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, color);
	this->setPalette(palette);

	m_customPlot->setBackground(color);
	m_customPlot->replot();
	update();
}

void PlotPolar::getCoordRangeX(double& lower, double& upper)
{
	lower = m_angularRange_lower;
	upper = m_angularRange_upper;
}

void PlotPolar::getCoordRangeY(double& lower, double& upper)
{
	lower = m_radialRange_lower;
	upper = m_radialRange_upper;
}

void PlotPolar::setAxisColorWidth(QColor color, uint width)
{
    if(m_axisColor == color && m_axisWidth == width)
		return;

	m_axisColor = color;
	m_axisWidth = width;
	m_angularAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_angularAxis->radialAxis()->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotPolar::setGridColorWidth(QColor color, uint width)
{
    if(m_gridColor == color && m_gridWidth == width)
		return;

	m_gridColor = color;
	m_gridWidth = width;
	m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, Qt::SolidLine));
	m_angularAxis->setTickPen(QPen(m_gridColor, 1));
	m_customPlot->replot();
}

void PlotPolar::setHorzGrids(uint count)
{
    if(m_horzGrids == count || count <= 0)
	{
		return;
	}
	m_horzGrids = count;
	double range = (m_angularRange_upper - m_angularRange_lower) / count;
	m_angularAxis->ticker().dynamicCast<QCPAxisTickerFixed>().data()->setTickStep(range);
	m_customPlot->replot();
}

void PlotPolar::setVertGrids(uint count)
{
    if(m_vertGrids == count || count < 0)
	{
		return;
	}
	m_vertGrids = count;
    if(count == 0)
	{
		m_angularAxis->grid()->setVisible(false);
	}
	else
	{
		m_angularAxis->grid()->setVisible(true);
		m_angularAxis->radialAxis()->ticker()->setTickCount(count);
	}
	m_customPlot->replot();
}

void PlotPolar::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
    // 	m_angularAxis->setBackground(color);
    // 	m_customPlot->replot();
}

void PlotPolar::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_angularAxis->grid()->setVisible(enable);
	m_customPlot->replot();
}

void PlotPolar::setTickLabelColor(QColor& color)
{
	m_tickLabelColor = color;
	m_angularAxis->setTickLabelColor(m_tickLabelColor);
	m_angularAxis->radialAxis()->setTickLabelColor(m_tickLabelColor);
	m_customPlot->replot();
}

void PlotPolar::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	m_angularAxis->setTickLabelFont(font);
	m_angularAxis->radialAxis()->setTickLabelFont(font);
	m_customPlot->replot();
}

void PlotPolar::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotPolar::setGridStyle(GridStyle style)
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
	m_angularAxis->grid()->setAngularPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_angularAxis->grid()->setRadialPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

void PlotPolar::setGridDensity(GridDensity density) {}

void PlotPolar::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
    int radius = qMin(width, int(height - 1.25 * h));

	painter.setFont(m_titleFont);
	painter.setPen(m_titleColor);
    if(width > (height - h))
	{
        m_customPlot->setGeometry((width - radius) / 2, 1.25 * h, radius, radius);
        if(m_titleVisible)
		{
            painter.fillRect((width - w) / 2, h * 0.25, w, h, m_titleFillColor);
			painter.drawText(QPoint((width - w) / 2, h), m_title);
		}
    }
	else
	{
        m_customPlot->setGeometry(0, (height + h - radius) / 2 + 0.25 * h, radius, radius);
        if(m_titleVisible)
		{
            painter.fillRect(
                (width - w) / 2, (height + h - radius) / 2 - 0.75 * h, w, h, m_titleFillColor);
			painter.drawText(QPoint((width - w) / 2, (height + h - radius) / 2), m_title);
		}
	}
    PlotItemBase::paintEvent(event);
}

void PlotPolar::setTitle(QString& title)
{
	m_title = title;
	update();
}

void PlotPolar::setTitleColor(QColor& color)
{
	m_titleColor = color;
	update();
}

void PlotPolar::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotPolar::setTitleFont(QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotPolar::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotPolar::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotPolar::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_angularAxis->setFormatShow(on);
	m_customPlot->replot();
}

void PlotPolar::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_angularAxis->radialAxis()->setFormatShow(on);
	m_customPlot->replot();
}

void PlotPolar::setUnitsX(const QString& units)
{
	m_units_x = units;
	m_angularAxis->setFormat(units);
	m_customPlot->replot();
}

void PlotPolar::setUnitsY(const QString& units)
{
	m_units_y = units;
	m_angularAxis->radialAxis()->setFormat(units);
	m_customPlot->replot();
}

void PlotPolar::setCoordRangeX(double lower, double upper)
{
	m_angularAxis->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotPolar::setCoordRangeY(double lower, double upper)
{
	m_angularAxis->radialAxis()->setRange(lower, upper);
	m_customPlot->replot();
}

void PlotPolar::slot_setRangeDrag(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeDrag, enabled);
	m_customPlot->replot();
}

void PlotPolar::slot_setRangeZoom(bool enabled)
{
	m_customPlot->setInteraction(QCP::iRangeZoom, enabled);
	m_customPlot->replot();
}

void PlotPolar::onGetCurrentSeconds(double secs)
{
    if(getDataPairs().isEmpty())
		return;

    int isize = getDataPairs().size();
	QVector<QCPPolarGraph*> graph;

    for(int i = 0; i < isize; ++i)
	{
        QString xcolumn = getDataPairs().at(i)->getDataPair().first;
        QString ycolumn = getDataPairs().at(i)->getDataPair().second;
		QStringList xlist = xcolumn.split("+");
		QStringList ylist = ycolumn.split("+");

        QVector<double> x = DataManager::getInstance()
                                ->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs)
                                .toVector();
        QVector<double> y = DataManager::getInstance()
                                ->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs)
                                .toVector();

		QCPPolarGraph* g = new QCPPolarGraph(m_angularAxis, m_angularAxis->radialAxis());
		g->setScatterStyle(QCPScatterStyle::ssDisc);
		g->setLineStyle(QCPPolarGraph::lsNone);
		graph.append(g);
		graph[i]->setPen(QPen(QColor(255, 0, 0), 2));
		graph[i]->addData(x, y);
	}
	m_customPlot->replot();

	qDeleteAll(graph);
	graph.clear();
}

void PlotPolar::slot_setMouseEventEnable(bool on)
{
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, on);
}

void PlotPolar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
	m_thresholdColorMap = targetMap;
}
