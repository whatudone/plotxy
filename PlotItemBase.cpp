/* * @filename: PlotItemBase.cpp
* @brief: PlotBase Plot»ùÀà
* @author: zhameng
* @create time: 2022-09-21
*  */

#include "PlotItemBase.h"

PlotItemBase::PlotItemBase(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    
    m_position = QPoint(0,0);
    m_width = 0;
    m_height = 0;
    m_bVisible = true; 
	m_coordBgn_x = 0;
	m_coordEnd_x = 100;
	m_coordBgn_y = 0;
	m_coordEnd_y = 100;
	m_outerFillColor = Qt::black;
	m_outlineColor = Qt::black;

	m_horzGrids = 4;
	m_vertGrids = 4;
	m_axisWidth = 1;
	m_gridWidth = 1;
	m_axisColor = Qt::white;
	m_gridColor = Qt::white;
	m_gridVisible = true;
	m_tickLabelColor = Qt::white;
	m_tickLabelFontSize = 20;
	m_tickLabelFont.setFamily("Microsoft YaHei");
	m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);
	m_gridStyle = Qt::DotLine;
	m_gridDensity = GridDensity::LESS;
	m_gridFillColor = Qt::black;

	m_units_x = nullptr;
	m_units_y = nullptr;
	m_showUnits_x = true;
	m_showUnits_y = true;
	m_title = nullptr;
	m_titleVisible = true;
	m_titleColor = Qt::white;
	m_titleFillColor = Qt::black;
	m_titleFontSize = 16;
	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(m_titleFontSize);
}
   
PlotItemBase::~PlotItemBase()
{

}


void PlotItemBase::init()
{

}

void PlotItemBase::setPosition(int x, int y)
{
    m_position.setX(x);
    m_position.setY(y);
	this->setGeometry(x, y, currWidth(), currHeight());
	update();
}

void PlotItemBase::setWidth(int width)
{
    m_width = width;
	update();
}

void PlotItemBase::setHeight(int height)
{
    m_height = height;
	update();
}

void PlotItemBase::setRect(QRect rect)
{
	m_position.setX(rect.x());
	m_position.setY(rect.y());
	m_width = rect.width();
	m_height = rect.height();
	this->setGeometry(rect);
	update();
}

QPoint PlotItemBase::currPosition()
{
	m_position = this->pos();
    return m_position;
}

int PlotItemBase::currWidth()
{
	m_width = this->width();
    return m_width;
}

int PlotItemBase::currHeight()
{
	m_height = this->height();
    return m_height;
}

QRect PlotItemBase::currRect()
{
	return this->rect();
}

void PlotItemBase::setName(const QString& name)
{
    m_plotItemName = name;
}

void PlotItemBase::setTabName(const QString& tabName)
{
    m_tabName = tabName;
}


QString PlotItemBase::currName()
{
    return m_plotItemName;
}

QString PlotItemBase::currTabName()
{
    return m_tabName;
}

void PlotItemBase::setOuterFillColor(QColor color)
{
	setAutoFillBackground(true);
	m_outerFillColor = color;
	QPalette palette = this->palette();
	palette.setColor(QPalette::Window, m_outerFillColor);
	this->setPalette(palette);
}

void PlotItemBase::setOutlineColor(QColor color)
{
	m_outlineColor = color;
}

void PlotItemBase::setCoordRangeX(double lower, double upper)
{
	m_coordBgn_x = lower;
	m_coordEnd_x = upper;
}

void PlotItemBase::setCoordRangeY(double lower, double upper)
{
	m_coordBgn_y = lower;
	m_coordEnd_y = upper;
}

void PlotItemBase::getCoordRangeX(double & lower, double & upper)
{
	lower = m_coordBgn_x;
	upper = m_coordEnd_x;
}

void PlotItemBase::getCoordRangeY(double & lower, double & upper)
{
	lower = m_coordBgn_y;
	upper = m_coordEnd_y;
}

void PlotItemBase::setHorzGrids(uint count)
{
	m_horzGrids = count;
}

void PlotItemBase::setVertGrids(uint count)
{
	m_vertGrids = count;
}

void PlotItemBase::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
}

void PlotItemBase::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
}

//void PlotItemBase::setGridColorWidth(uint width)
//{
//
//}

void PlotItemBase::setGridVisible(bool enable)
{
	m_gridVisible = enable;
}

void PlotItemBase::setTickLabelColor(QColor & color)
{
	m_tickLabelColor = color;
}

void PlotItemBase::setTickLabelFont(QFont & font)
{
	m_tickLabelFont = font;
}

void PlotItemBase::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPixelSize(size);
}

void PlotItemBase::setGridStyle(GridStyle style)
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
}

void PlotItemBase::setGridDensity(GridDensity density)
{
	m_gridDensity = density;
}

void PlotItemBase::setGridFillColor(QColor color)
{
	m_gridFillColor = color;
}

void PlotItemBase::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
}

void PlotItemBase::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
}

void PlotItemBase::setUnitsX(const QString & units)
{
	m_units_x = units;
}

void PlotItemBase::setUnitsY(const QString & units)
{
	m_units_y = units;
}

void PlotItemBase::setTitleVisible(bool on)
{
	m_titleVisible = on;
}

void PlotItemBase::setTitle(QString & title)
{
	m_title = title;
}

void PlotItemBase::setTitleColor(QColor& color)
{
	m_titleColor = color;
}

void PlotItemBase::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
}

void PlotItemBase::setTitleFont(QFont& font)
{
	m_titleFont = font;
}

void PlotItemBase::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
}

void PlotItemBase::setxAxisLabel(int label)
{
	m_xAxisLabel = label;
}

void PlotItemBase::setyAxisLabel(int label)
{
	m_yAxisLabel = label;
}

void PlotItemBase::setAxisLabelColor(QColor & color)
{
	m_axisLabelColor = color;
}

void PlotItemBase::setAxisLabelFont(QFont & font)
{
	m_axisLabelFont = font;
}

void PlotItemBase::setAxisLabelFontSize(int size)
{
	m_axisLabelFontSize = size;
	m_axisLabelFont.setPointSize(size);
}

void PlotItemBase::addPlotPairData(QPair<QString, QString> pair)
{
	DataPair* data = new DataPair(pair);
	m_dataPair.append(data);

	emit sgn_dataPairChanged(m_tabName, m_plotItemName);
}

void PlotItemBase::delPlotPairData(QPair<QString, QString> pair)
{
	if (m_dataPair.isEmpty())
		return;

	for (int i = 0; i < m_dataPair.size(); ++i)
	{
		if (m_dataPair.at(i)->getDataPair() == pair)
		{
			m_dataPair.remove(i);

			emit sgn_dataPairChanged(m_tabName, m_plotItemName);
			break;
		}
	}
}

void PlotItemBase::updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair)
{
	if (m_dataPair.isEmpty())
		return;

	for (int i = 0; i < m_dataPair.size(); ++i)
	{
		if (m_dataPair.at(i)->getDataPair() == oldPair)
		{
			m_dataPair.at(i)->setDataPair(newPair);

			emit sgn_dataPairChanged(m_tabName, m_plotItemName);
			break;
		}
	}
}

void PlotItemBase::setDataPair(QVector<DataPair*> newVector)
{
	m_dataPair.swap(newVector);

	emit sgn_dataPairChanged(m_tabName, m_plotItemName);
}

void PlotItemBase::slot_setVisible(bool on)
{
	m_bVisible = on;
	this->setVisible(on);
}

void PlotItemBase::slot_updateRect(QRect rect)
{
	setRect(rect);
}

//void PlotItemBase::setVisible(bool bVisible)
//{
//    m_bVisible = bVisible;
//}
// 
//bool PlotItemBase::bVisible()
//{
//    return m_bVisible;
//}