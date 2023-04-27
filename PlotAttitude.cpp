#include "PlotAttitude.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>

int PlotAttitude::m_instanceCount = 1;
PlotAttitude::PlotAttitude(QWidget* parent)
	: PlotItemBase(parent)
{

	m_axisColor = Qt::white;
	m_gridColor = QColor(200, 200, 200);
	m_border_ColorStart = Qt::white;
	m_border_ColorEnd = Qt::white;
	m_gridFillColor = Qt::black;
	m_rollColor = Qt::white;
	m_pitchColor = Qt::red;

	m_tickRadiusPercentage = 100;
	m_textPercentage = 125;
	m_dialPercentage = 100;

	m_pitchValue = 0.0;
	m_rollValue = 0.0;

	m_coordBgn_y = 0.0;
	m_coordEnd_y = 180;
	m_coordBgn_x = 0;
	m_coordEnd_x = 360;
	m_horzGrids = 4;
	m_vertGrids = 5;

	m_units_x = QString("°");
	m_units_y = QString("°");
	m_showUnits_x = true;
	m_showUnits_y = true;
	m_decision_roll = 0;
	m_decision_pitch = 0;
	m_title = "Attitude";

	m_tickLabelFontSize = 10;
	m_tickLabelFont.setFamily("Microsoft YaHei");
	m_tickLabelFont.setPointSizeF(m_tickLabelFontSize);

	m_axisLabelFontSize = 10;
	m_axisLabelFont.setFamily("Microsoft YaHei");
	m_axisLabelFont.setPointSizeF(m_axisLabelFontSize);

	m_xAxisLabel = "Roll";
	m_yAxisLabel = "Pitch";

	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}

PlotAttitude::~PlotAttitude() {}

void PlotAttitude::drawTitle(QPainter* painter, int radius)
{
	painter->save();
	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	double h0 = fm.size(Qt::TextSingleLine, m_title).height();
	double as = fm.ascent();
	QFontMetricsF fm1(m_tickLabelFont);
	double h = fm1.size(Qt::TextSingleLine, m_title).height();

	painter->setFont(m_titleFont);
	painter->setPen(m_titleColor);
    if(m_titleVisible)
	{
        painter->fillRect(-w / 2, -radius - 1.25 * h - h0, w, h0 + 0.25 * h, m_titleFillColor);
        painter->drawText(QPoint(-w / 2, -radius - h * 1.5), m_title);
	}

	painter->restore();
}

void PlotAttitude::drawBorder(QPainter* painter, int radius)
{
	painter->save();
	painter->setPen(QPen(m_axisColor, m_axisWidth, Qt::SolidLine));

	painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
	painter->drawLine(0, -radius, 0, radius);
	painter->restore();
}

void PlotAttitude::drawBg(QPainter* painter, int radius)
{
	radius -= 2;
	painter->save();
	painter->setPen(Qt::NoPen);
	QLinearGradient borderGradient(0, -radius, 0, radius);
	m_gridFillColor.setAlpha(255);
	borderGradient.setColorAt(0, m_gridFillColor);
	m_gridFillColor.setAlpha(255);
	borderGradient.setColorAt(1, m_gridFillColor);
	painter->setBrush(borderGradient);
	painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

	painter->restore();
}

void PlotAttitude::drawScale_roll(QPainter* painter, int radius)
{
	QFontMetricsF fm(m_tickLabelFont);

	painter->save();
	painter->setFont(m_tickLabelFont);
	double eachMajor_roll = (m_coordEnd_x - m_coordBgn_x) / (double)m_horzGrids;
	double x, y, w, h;
    QString str, strNum;

    for(uint i = 0; i < m_horzGrids; ++i)
	{
		strNum = QString::number(eachMajor_roll * i + m_coordBgn_x, 'f', m_decision_roll);
        if(m_showUnits_x)
			str = QString("%1%2").arg(strNum).arg(m_units_x);
		else
			str = QString("%1").arg(strNum);
		w = fm.size(Qt::TextSingleLine, strNum).width() + 5;
		h = fm.size(Qt::TextSingleLine, strNum).height();
		double rad = qDegreesToRadians(360.0 / (double)m_horzGrids * i);
		x = radius * sin(rad);
		y = radius * cos(rad) * (-1);

		painter->setPen(QPen(m_gridColor, m_gridWidth));
		painter->drawLine(0, 0, x, y);

        int R = radius * m_tickRadiusPercentage / 100 + sqrt(pow(w / 2, 2) + pow(h / 2, 2));
        int x_R = R * sin(rad) - w / 2;
		int y_R = R * cos(rad) * (-1) + h / 4;

		painter->setPen(QPen(m_rollColor));
		painter->drawText(x_R, y_R, str);
	}
	painter->restore();
}

void PlotAttitude::drawScale_pitch(QPainter* painter, int radius)
{
	QFontMetricsF fm(m_tickLabelFont);

	painter->save();
	painter->setFont(m_tickLabelFont);
	painter->setPen(QPen(m_pitchColor));
	double x, y, w, h;
	QString str, strNum;
	double eachMajor_pitch = (m_coordEnd_y - m_coordBgn_y) / (double)(m_vertGrids - 1);
    for(uint i = 0; i < m_vertGrids; ++i)
	{
		strNum = QString::number(eachMajor_pitch * i + m_coordBgn_y, 'f', m_decision_pitch);
		if(m_showUnits_y)
			str = QString("%1%2").arg(strNum).arg(m_units_y);
		else
			str = QString("%1").arg(strNum);
		w = fm.size(Qt::TextSingleLine, str).width() + 10;
		h = fm.size(Qt::TextSingleLine, str).height();
        //		double rad = qDegreesToRadians(eachMajor_pitch * i);
        x = -w;
        if(i == 0)
			y = radius - 2 * radius / (m_vertGrids - 1) * i - h / 4;
		else if(i == (m_vertGrids - 1))
			y = radius - 2 * radius / (m_vertGrids - 1) * i + h;
		else
			y = radius - 2 * radius / (m_vertGrids - 1) * i + h / 4;

		painter->drawText(x, y, str);
	}

	painter->restore();
}

void PlotAttitude::drawLine_roll(QPainter* painter, int radius)
{
	painter->save();
	painter->setPen(QPen(m_rollColor, 4));
	double range = abs(m_coordEnd_x - m_coordBgn_x);
	painter->rotate((fmodf(m_rollValue, range) - m_coordBgn_x) * 360.0 / range);
	painter->drawLine(QPoint(-radius / 2, 0), QPoint(radius / 2, 0));
	painter->drawLine(QPoint(0, 0), QPoint(0, -radius / 2));

	painter->restore();
}

void PlotAttitude::drawLine_pitch(QPainter* painter, int radius)
{
	painter->save();
	painter->setPen(QPen(m_pitchColor, 4));
	double range = abs(m_coordEnd_y - m_coordBgn_y);
    //	if (fmodf(m_pitchValue, range) > m_coordBgn_y && fmodf(m_pitchValue, range) < m_coordEnd_y)
    //	{
	double translate_y = radius - (fmodf(m_pitchValue, range) - m_coordBgn_y) / range * 2 * radius;
	painter->translate(0, translate_y);
	painter->drawLine(QPoint(-radius / 2, 0), QPoint(radius / 2, 0));
    //	}

	painter->restore();
}

void PlotAttitude::drawText_roll(QPainter* painter, int radius)
{
	radius = radius * m_textPercentage / 100;
	painter->save();

	painter->setFont(m_axisLabelFont);
	painter->setPen(QPen(m_rollColor));

	QFontMetricsF fm(m_axisLabelFont);
	double h = fm.size(Qt::TextSingleLine, m_xAxisLabel).height();
	double w = fm.size(Qt::TextSingleLine, m_xAxisLabel).width();
	double rad = qDegreesToRadians(45.0);
	int xPos = (-1) * radius * cos(rad) - w / 2;
	int yPos = radius * sin(rad);
	painter->drawText(QPoint(xPos, yPos), m_xAxisLabel);

	QString str, strNum;
	strNum = QString::number(m_rollValue, 'f', m_decision_roll);
    if(m_showUnits_x)
		str = QString("%1%2").arg(strNum).arg(m_units_x);
	else
		str = QString("%1").arg(strNum);
	painter->drawText(QPoint(xPos, yPos + h), str);

	painter->restore();
}

void PlotAttitude::drawText_pitch(QPainter* painter, int radius)
{
	radius = radius * m_textPercentage / 100;
	painter->save();
	painter->setFont(m_axisLabelFont);
	painter->setPen(QPen(m_pitchColor));

	QFontMetricsF fm(m_axisLabelFont);
	double h = fm.size(Qt::TextSingleLine, m_yAxisLabel).height();
	double w = fm.size(Qt::TextSingleLine, m_yAxisLabel).width();
	double rad = qDegreesToRadians(45.0);
	int xPos = radius * cos(rad) - w / 2;
	int yPos = radius * sin(rad);
	painter->drawText(QPoint(xPos, yPos), m_yAxisLabel);

	QString str, strNum;
	strNum = QString::number(m_pitchValue, 'f', m_decision_pitch);
    if(m_showUnits_y)
		str = QString("%1%2").arg(strNum).arg(m_units_y);
	else
		str = QString("%1").arg(strNum);
	painter->drawText(QPoint(xPos, yPos + h), str);

	painter->restore();
}

QColor PlotAttitude::getBorderOutColorStart() const
{
	return m_border_ColorStart;
}

QColor PlotAttitude::getBorderOutColorEnd() const
{
	return m_border_ColorEnd;
}

QColor PlotAttitude::getRollColor() const
{
	return m_rollColor;
}

QColor PlotAttitude::getPitchColor() const
{
	return m_pitchColor;
}

float PlotAttitude::getPitchValue() const
{
	return m_pitchValue;
}

float PlotAttitude::getRollValue() const
{
	return m_rollValue;
}

QSize PlotAttitude::sizeHint() const
{
	return QSize();
}

QSize PlotAttitude::minimumSizeHint() const
{
	return QSize();
}

void PlotAttitude::setHorzGrids(uint count)
{
    if(m_horzGrids == count || count <= 0)
	{
		return;
	}
	m_horzGrids = count;
	update();
}

void PlotAttitude::setVertGrids(uint count)
{
    if(m_vertGrids == count || count <= 0)
	{
		return;
	}
	m_vertGrids = count;
	update();
}

void PlotAttitude::setAxisColorWidth(QColor color, uint width)
{
	m_axisColor = color;
	m_axisWidth = width;
	update();
}

void PlotAttitude::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
	update();
}

void PlotAttitude::setUnitsShowX(bool on)
{
	m_showUnits_x = on;
	update();
}

void PlotAttitude::setUnitsShowY(bool on)
{
	m_showUnits_y = on;
	update();
}

void PlotAttitude::setUnitsX(const QString& units)
{
	m_units_x = units;
	update();
}

void PlotAttitude::setUnitsY(const QString& units)
{
	m_units_y = units;
	update();
}

void PlotAttitude::updateGraph()
{
	int xSize = m_xValueList.size();
	int ySize = m_yValueList.size();

    if(xSize <= ySize)
	{
        for(int i = 0; i < xSize; ++i)
		{
			slot_setRollValue(m_xValueList.at(i));
			slot_setPitchValue(m_yValueList.at(i));
		}
        for(int i = xSize; i < ySize; i++)
		{
			slot_setPitchValue(m_yValueList.at(i));
		}
	}
	else
	{
        for(int i = 0; i < ySize; ++i)
		{
			slot_setRollValue(m_xValueList.at(i));
			slot_setPitchValue(m_yValueList.at(i));
		}
        for(int i = ySize; i < xSize; i++)
		{
			slot_setRollValue(m_xValueList.at(i));
		}
	}
}

void PlotAttitude::setTickRadiusPercentage(int value)
{
	m_tickRadiusPercentage = value;
	update();
}

void PlotAttitude::setTextPercentage(int value)
{
	m_textPercentage = value;
	update();
}

void PlotAttitude::setDialPercentage(int value)
{
	m_dialPercentage = value;
	update();
}

void PlotAttitude::slot_setBorderColorStart(const QColor& borderOutColorStart)
{
	m_border_ColorStart = borderOutColorStart;
	update();
}

void PlotAttitude::slot_setBorderColorEnd(const QColor& borderOutColorEnd)
{
	m_border_ColorEnd = borderOutColorEnd;
	update();
}

void PlotAttitude::setGridFillColor(QColor bgColor)
{
	m_gridFillColor = bgColor;
	update();
}

void PlotAttitude::setRollColor(const QColor& color)
{
	m_rollColor = color;
	update();
}

void PlotAttitude::setPitchColor(const QColor& color)
{
	m_pitchColor = color;
	update();
}

void PlotAttitude::setTitleColor(QColor& titleColor)
{
	m_titleColor = titleColor;
	update();
}

void PlotAttitude::setTitleFillColor(QColor& color)
{
	m_titleFillColor = color;
	update();
}

void PlotAttitude::setTitleFont(QFont& font)
{
	m_titleFont = font;
	update();
}

void PlotAttitude::setTitleFontSize(int size)
{
	m_titleFontSize = size;
	m_titleFont.setPointSize(size);
	update();
}

void PlotAttitude::setTickLabelFont(QFont& font)
{
	m_tickLabelFont = font;
	update();
}

void PlotAttitude::setTickLabelFontSize(int size)
{
	m_tickLabelFontSize = size;
	m_tickLabelFont.setPointSize(size);
	update();
}

void PlotAttitude::setAxisLabelFont(QFont& font)
{
	m_axisLabelFont = font;
	update();
}

void PlotAttitude::setAxisLabelFontSize(int size)
{
	m_axisLabelFontSize = size;
	m_axisLabelFont.setPointSize(size);
	update();
}

void PlotAttitude::setTitle(QString& title)
{
	m_title = title;
	update();
}

void PlotAttitude::setTitleVisible(bool show)
{
	m_titleVisible = show;
	update();
}

void PlotAttitude::slot_setPitchValue(double pitchValue)
{
	m_pitchValue = pitchValue;
	update();
}

void PlotAttitude::slot_setRollValue(double rollValue)
{
	m_rollValue = rollValue;
	update();
}

void PlotAttitude::updateDataForDataPairsByTime(double secs)
{
    if(getDataPairs().isEmpty())
    {
        return;
    }
    auto dataPair = getDataPairs().last();
    auto xEntityID = dataPair->getEntityIDX();
    auto yEntityID = dataPair->getEntityIDY();
    auto xAttr = dataPair->getAttr_x();
    auto yAttr = dataPair->getAttr_y();
    m_xValueList =
        DataManager::getInstance()->getEntityAttrValueListByMaxTime(xEntityID, xAttr, secs);
    m_yValueList =
        DataManager::getInstance()->getEntityAttrValueListByMaxTime(yEntityID, yAttr, secs);
    updateGraph();
}

void PlotAttitude::customPainting(QPainter& painter)
{
    int width = this->width();
    int height = this->height();

    QFontMetricsF titleFm(m_titleFont);
    double titleFontHeight = titleFm.size(Qt::TextSingleLine, m_title).height();
    double titleAscent = titleFm.ascent();
    QFontMetricsF tickLabelFm(m_tickLabelFont);

    double tickFontHeight = tickLabelFm.size(Qt::TextSingleLine, m_title).height();
    // 半径要减去（上下pad+上下文字的高度）

    int radius = qMin(width - m_leftPadding - m_rightPadding,
                      height - m_topPadding - m_bottomPadding - titleFontHeight - titleAscent -
                          tickFontHeight * 2) /
                 2;
    radius = radius * m_dialPercentage / 100;

    //画笔
    painter.translate(width / 2, height / 2);

    //绘制背景
    drawBg(&painter, radius);
    //绘制刻度尺
    drawScale_roll(&painter, radius);
    drawScale_pitch(&painter, radius);
    //绘制外边框
    drawBorder(&painter, radius);
    //绘制线条
    drawLine_roll(&painter, radius);
    drawLine_pitch(&painter, radius);
    //绘制文本
    drawText_roll(&painter, radius);
    drawText_pitch(&painter, radius);
}
