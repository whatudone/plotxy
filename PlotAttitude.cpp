#include "PlotAttitude.h"
#include "DataManager.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>

int PlotAttitude::m_instanceCount = 1;
PlotAttitude::PlotAttitude(QWidget* parent)
	: PlotItemBase(parent)
{
	m_border_ColorStart = Qt::white;
	m_border_ColorEnd = Qt::white;
	m_rollColor = Qt::white;
	m_pitchColor = Qt::red;

	m_tickRadiusPercentage = 100;
	m_textPercentage = 125;
	m_dialPercentage = 100;

	m_pitchValue = 0.0;
	m_rollValue = 0.0;

    m_coordBgn_y = -90.0;
    m_coordEnd_y = 90.0;
	m_coordBgn_x = 0;
    m_coordEnd_x = 360;

    // 本图初始特定设置
    m_horzGrids = 4;
    m_vertGrids = 3;

	m_units_x = QString("°");
    m_units_y = QString("°");
    m_decision_roll = 0;
    m_decision_pitch = 0;
	m_title = "Attitude";

    m_xTickLabelFontSize = 10;
    m_xTickLabelFont.setFamily("Microsoft YaHei");
    m_xTickLabelFont.setPixelSize(m_xTickLabelFontSize);

	m_xAxisLabel = "Roll";
	m_yAxisLabel = "Pitch";

	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

    m_widget = new QWidget;
    setupLayout();
}

PlotAttitude::~PlotAttitude() {}

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
    QFontMetricsF fm(m_xTickLabelFont);

	painter->save();
    painter->setFont(m_xTickLabelFont);
	double eachMajor_roll = (m_coordEnd_x - m_coordBgn_x) / (double)m_horzGrids;
	double x, y, w, h;
    QString str, strNum;

    for(uint i = 0; i < m_horzGrids; ++i)
	{
        double rollTickNum = eachMajor_roll * i + m_coordBgn_x;
        // roll刻度不超过180
        if(rollTickNum > 180.0)
        {
            rollTickNum = rollTickNum - 360;
        }
        strNum = QString::number(rollTickNum, 'f', m_decision_roll);
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
	painter->save();
    QFontMetricsF fm(m_xTickLabelFont);
    double x, y;

    painter->setFont(m_xTickLabelFont);
	painter->setPen(QPen(m_pitchColor));
	QString str, strNum;
    double eachMajor_pitch = (m_coordEnd_y - m_coordBgn_y) / (m_vertGrids - 1);
    // 最底下的刻度点
    QPointF tickPoint(0, radius);
    int32_t step = 2 * radius / (m_vertGrids - 1);
    // 从上往下绘制
    for(uint i = 0; i < m_vertGrids; ++i)
	{
		strNum = QString::number(eachMajor_pitch * i + m_coordBgn_y, 'f', m_decision_pitch);
		if(m_showUnits_y)
			str = QString("%1%2").arg(strNum).arg(m_units_y);
		else
			str = QString("%1").arg(strNum);
        double tickLabelWidth = fm.size(Qt::TextSingleLine, str).width() + 10;
        double tickLabelHeight = fm.size(Qt::TextSingleLine, str).height();
        x = -tickLabelWidth;
        y = tickPoint.y() - i * step - tickLabelHeight;
        painter->drawText(QPointF(x, y), str);
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

	double translate_y = radius - (fmodf(m_pitchValue, range) - m_coordBgn_y) / range * 2 * radius;
	painter->translate(0, translate_y);
	painter->drawLine(QPoint(-radius / 2, 0), QPoint(radius / 2, 0));
	painter->restore();
}

void PlotAttitude::drawText_roll(QPainter* painter, int radius)
{
	radius = radius * m_textPercentage / 100;
	painter->save();

    painter->setFont(m_xAxisLabelFont);
	painter->setPen(QPen(m_rollColor));

    QFontMetricsF fm(m_xAxisLabelFont);
	double h = fm.size(Qt::TextSingleLine, m_xAxisLabel).height();
	double w = fm.size(Qt::TextSingleLine, m_xAxisLabel).width();
	double rad = qDegreesToRadians(45.0);

    int xPos = radius * cos(rad) - w / 2;
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
    painter->setFont(m_xAxisLabelFont);
	painter->setPen(QPen(m_pitchColor));

    QFontMetricsF fm(m_xAxisLabelFont);
	double h = fm.size(Qt::TextSingleLine, m_yAxisLabel).height();
	double w = fm.size(Qt::TextSingleLine, m_yAxisLabel).width();
	double rad = qDegreesToRadians(45.0);

    int xPos = (-1) * radius * cos(rad) - w / 2;
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
    m_titleFont.setPixelSize(size);
	update();
}

void PlotAttitude::setTickLabelFont(QFont& font)
{
    m_xTickLabelFont = font;
	update();
}

void PlotAttitude::setTickLabelFontSize(int size)
{
    m_xTickLabelFontSize = size;
    m_xTickLabelFont.setPixelSize(size);
	update();
}

void PlotAttitude::setxAxisLabelFont(QFont& font)
{
    m_xAxisLabelFont = font;
	update();
}

void PlotAttitude::setxAxisLabelFontSize(int size)
{
    m_xAxisLabelFontSize = size;
    m_xAxisLabelFont.setPixelSize(size);
	update();
}

void PlotAttitude::setyAxisLabelFont(QFont& font)
{
    m_yAxisLabelFont = font;
    update();
}

void PlotAttitude::setyAxisLabelFontSize(int size)
{
    m_yAxisLabelFontSize = size;
    m_yAxisLabelFont.setPixelSize(size);
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
    m_rollValue = DataManager::getInstance()->getEntityAttrValueByMaxTime(xEntityID, xAttr, secs);
    m_pitchValue = DataManager::getInstance()->getEntityAttrValueByMaxTime(yEntityID, yAttr, secs);
    update();
}

void PlotAttitude::updateGraphByDataPair(DataPair* data)
{
    Q_UNUSED(data)
    // 暂时无法单独更新每个DataPair数据，只能全局刷新
    update();
}

void PlotAttitude::customPainting(QPainter& painter)
{
    int width = m_widget->width();
    int height = m_widget->height();
    QFontMetricsF fm(m_xAxisLabelFont);
    double h = fm.size(Qt::TextSingleLine, m_yAxisLabel).height();
    double yLabelAscent = fm.ascent();
    int radius = qMin(width, static_cast<int>(height - h - yLabelAscent)) / 2;
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
    if(getDataPairs().isEmpty())
    {
        return;
    }
    auto dataPair = getDataPairs().last();
    if(dataPair->isDraw())
    {
        //绘制线条
        drawLine_roll(&painter, radius);
        drawLine_pitch(&painter, radius);
        //绘制文本
        drawText_roll(&painter, radius);
        drawText_pitch(&painter, radius);
    }
}
