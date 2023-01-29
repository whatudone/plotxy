#include "PlotScatter.h"
#include "DataManager.h"

#include <QDebug>
#include <QPainter>
#include <QTime>

int PlotScatter::m_instanceCount = 1;

PlotScatter::PlotScatter(QWidget *parent)
    : PlotItemBase(parent)
{
    QString name = QString("Scatter%1").arg(m_instanceCount);
    this->setName(name);
    m_instanceCount += 1;

//     qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
//     for (int i = 0; i < 20; i++) {
//         m_clrList << QColor::fromRgb(qrand() % 255, qrand() % 255, qrand() % 255);
//     }
	m_outerFillColor = Qt::black;
	m_gridFillColor = Qt::black;
	m_title = "Scatter Plot";
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFontSize = 16;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
	m_titleVisible = true;

	m_axisLabelColor = Qt::white;
	m_axisLabelFont.setFamily("Microsoft YaHei");
	m_axisLabelFont.setPointSizeF(10.0);
	m_xAxisLabel = "X Axis";
	m_yAxisLabel = "Y Axis";

	m_leftPadding = 10;
	m_rightPadding = 10;
	m_topPadding = 10;
	m_bottomPadding = 10;

	m_coordBgn_x = 0;
	m_coordEnd_x = 2000;
	m_coordBgn_y = 0;
	m_coordEnd_y = 2000;

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

PlotScatter::~PlotScatter()
{
	
}

void PlotScatter::initPlot()
{
	m_customPlot = new QCustomPlot(this);
	m_customPlot->setAttribute(Qt::WA_TransparentForMouseEvents, true);
 	m_customPlot->axisRect()->setupFullAxesBox(true);
	m_customPlot->axisRect()->setMinimumMargins(QMargins(30, 15, 30, 15));

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

//  	m_customPlot->xAxis->setNumberPrecision(3);
//  	m_customPlot->yAxis->setNumberPrecision(3);

	m_customPlot->setBackground(m_outerFillColor);
	m_customPlot->axisRect()->setBackground(m_gridFillColor);
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);

	m_customPlot->replot();
}

void PlotScatter::getDataInfo(double secs)
{
	if (getDataPair().isEmpty())
		return;

	m_customPlot->clearGraphs();

	int itemCnt = m_dataPair.size();
	for (int i = 0; i < itemCnt; ++i)
	{
		updateData(secs, i, m_dataPair.at(i));
	}
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
//	update();
}

void PlotScatter::updateData(double secs, int index, DataPair * data)
{
	QVector<double> x, y;
	QString xEntityType = data->getDataPair().first;
	QString yEntityType = data->getDataPair().second;
	QStringList xlist = xEntityType.split("+");
	QStringList ylist = yEntityType.split("+");
	if (xlist.size() == 1 && ylist.size() == 1)
	{
		x = DataManager::getInstance()->getTimeData_vector();
		y = DataManager::getInstance()->getTimeData_vector();
	}
	else if (xlist.size() == 1 && ylist.size() == 2)
	{
		x = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), xlist.at(0), secs).toVector();
		y = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs).toVector();
	}
	else if (xlist.size() == 2 && ylist.size() == 1)
	{
		x = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs).toVector();
		y = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), ylist.at(0), secs).toVector();
	}
	else
	{
		x = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs).toVector();
		y = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs).toVector();
	}

	if (x.isEmpty() || y.isEmpty())
		return;

	m_customPlot->addGraph();
	if (data->isDraw())
	{
		m_customPlot->graph(index)->setPen(QPen(data->dataColor(), data->lineWidth()));
		//line mode
		if (data->isLineMode())
		{ 
			m_customPlot->graph(index)->setLineStyle(QCPGraph::lsLine);
			m_customPlot->graph(index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone));
		}
		else
		{ 
			m_customPlot->graph(index)->setLineStyle(QCPGraph::lsNone);
			m_customPlot->graph(index)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, data->lineWidth()));
		}
		//icon
		if (data->hasIcon())
		{
			//test
			QPixmap pix(":/statusbar/centerPlot.bmp");
			pix = pix.scaled(data->iconSize(), Qt::IgnoreAspectRatio);
			QTransform trans;
			trans.rotate(45);
			pix = pix.transformed(trans);
			QCPScatterStyle style(pix);
			m_customPlot->graph(index)->setScatterStyle(style);
			//test end
		}

		m_customPlot->graph(index)->addData(x, y);
	}
}

void PlotScatter::onGetCurrentSeconds(double secs)
{
    m_curSeconds = secs;
//    update();
	getDataInfo(m_curSeconds);
}

void PlotScatter::paintEvent(QPaintEvent *event)
{
	int width = this->width();
	int height = this->height();

    QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	//绘制标题
	if (m_titleVisible)
	{
		painter.setFont(m_titleFont);
		painter.setPen(m_titleColor);
		painter.fillRect((width - w + m_leftPadding - m_rightPadding) / 2, m_topPadding, w, h, m_titleFillColor);
		painter.drawText(QPoint((width - w + m_leftPadding - m_rightPadding) / 2, as + m_topPadding), m_title);
	}

	m_customPlot->setGeometry(m_leftPadding, h + m_topPadding,
		width - m_leftPadding - m_rightPadding, height - h - m_topPadding - m_bottomPadding);

//    getDataInfo(m_curSeconds);
}

void PlotScatter::setPaddings(double top, double bottom, double left, double right)
{
	m_topPadding = top;
	m_bottomPadding = bottom;
	m_leftPadding = left;
	m_rightPadding = right;
	update();
}

void PlotScatter::setTitle(QString & str)
{
	m_title = str;
	update();
}

void PlotScatter::setTitleColor(QColor & color)
{
	m_titleColor = color;
	update();
}

void PlotScatter::setTitleFillColor(QColor & color)
{
	m_titleFillColor = color;
	update();
}

void PlotScatter::setTitleFont(QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotScatter::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotScatter::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotScatter::setxAxisLabel(QString & str)
{
	m_xAxisLabel = str;
	m_customPlot->xAxis->setLabel(m_xAxisLabel);
}

void PlotScatter::setyAxisLabel(QString & str)
{
	m_yAxisLabel = str;
	m_customPlot->yAxis->setLabel(m_yAxisLabel);
	m_customPlot->replot();
}

void PlotScatter::setAxisLabelColor(QColor & color)
{
	m_axisLabelColor = color;
	m_customPlot->xAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->yAxis->setLabelColor(m_axisLabelColor);
	m_customPlot->replot();
}

void PlotScatter::setAxisLabelFont(QFont & font)
{
	m_axisLabelFont = font;
	m_customPlot->xAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->yAxis->setLabelFont(m_axisLabelFont);
	m_customPlot->replot();
}

void PlotScatter::setAxisVisible(bool on, AxisType type)
{
	switch (type)
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

void PlotScatter::setAxisTickLabelShow(bool on, AxisType type)
{
	switch (type)
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

void PlotScatter::rescale_xAxis(bool on)
{
	m_customPlot->xAxis->rescale(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescale_yAxis(bool on)
{
	m_customPlot->yAxis->rescale(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::rescaleAxis(bool on)
{
	m_customPlot->rescaleAxes(on);
	m_customPlot->replot(QCustomPlot::rpQueuedRefresh);
}

void PlotScatter::setOuterFillColor(QColor color)
{
	setAutoFillBackground(true);
	m_outerFillColor = color;
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, m_outerFillColor);
	this->setPalette(palette);

	m_customPlot->setBackground(color);
	m_customPlot->replot();
}

void PlotScatter::setCoordRangeX(double lower, double upper)
{
	if (m_coordBgn_x == lower && m_coordEnd_x == upper)
	{
		return;
	}

	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
	m_customPlot->xAxis->setRange(m_coordBgn_x, m_coordEnd_x);
	m_customPlot->replot();
}

void PlotScatter::setCoordRangeY(double lower, double upper)
{
	if (m_coordBgn_y == lower && m_coordEnd_y == upper)
	{
		return;
	}

	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
	m_customPlot->yAxis->setRange(m_coordBgn_y, m_coordEnd_y);
	m_customPlot->replot();
}

void PlotScatter::getCoordRangeX(double & lower, double & upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotScatter::getCoordRangeY(double & lower, double & upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}

void PlotScatter::setHorzGrids(uint count)
{
	if (m_horzGrids == count || count < 0)
	{
		return;
	}
	m_horzGrids = count;
	if (count == 0)
	{
		m_customPlot->yAxis->grid()->setVisible(false);
	}
	else
	{
		m_customPlot->yAxis->grid()->setVisible(m_gridVisible);
		m_customPlot->yAxis->ticker()->setTickCount(m_horzGrids);
	}
	m_customPlot->replot();
}

void PlotScatter::setVertGrids(uint count)
{
	if (m_vertGrids == count || count < 0)
	{
		return;
	}
	m_vertGrids = count;
	if (count == 0)
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

uint PlotScatter::getHorzGrids()
{
	return m_horzGrids;
}

uint PlotScatter::getVertGrids()
{
	return m_vertGrids;
}

void PlotScatter::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	m_customPlot->xAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->xAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->yAxis2->setBasePen(QPen(m_axisColor, m_axisWidth));
	m_customPlot->replot();
}

void PlotScatter::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	m_customPlot->xAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->yAxis->grid()->setPen(QPen(m_gridColor, m_gridWidth, m_gridStyle));
	m_customPlot->replot();
}

uint PlotScatter::getAxisWidth()
{
	return m_axisWidth;
}

uint PlotScatter::getGridWidth()
{
	return m_gridWidth;
}

QColor PlotScatter::getAxisColor()
{
	return m_axisColor;
}

QColor PlotScatter::getGridColor()
{
	return m_gridColor;
}

void PlotScatter::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
	m_customPlot->axisRect()->setBackground(color);
	m_customPlot->replot();
}

void PlotScatter::setGridVisible(bool enable)
{
	m_gridVisible = enable;
	m_customPlot->xAxis->grid()->setVisible(enable);
	m_customPlot->yAxis->grid()->setVisible(enable);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelColor(QColor & color)
{
	m_tickLabelColor = color;
	m_customPlot->xAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->yAxis->setTickLabelColor(m_tickLabelColor);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelFont(QFont & font)
{
	m_tickLabelFont = font;
	m_customPlot->xAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->yAxis->setTickLabelFont(m_tickLabelFont);
	m_customPlot->replot();
}

void PlotScatter::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	setTickLabelFont(m_tickLabelFont);
}

void PlotScatter::setGridStyle(GridStyle style)
{
	switch (style)
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

void PlotScatter::setGridDensity(GridDensity density)
{
}


void PlotScatter::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	m_customPlot->xAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotScatter::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	m_customPlot->yAxis->setAxisFormatShow(on);
	m_customPlot->replot();
}

void PlotScatter::setUnitsX(const QString & units)
{
	m_units_x = units;
	m_customPlot->xAxis->setAxisFormat(units);
	m_customPlot->replot();
}

void PlotScatter::setUnitsY(const QString & units)
{
	m_units_y = units;
	m_customPlot->yAxis->setAxisFormat(units);
	m_customPlot->replot();
}

