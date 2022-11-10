#include "PlotAttitude.h"
#include <QDebug>
#include <QPainter>
#include <QtMath>
#include "DataManager.h"

int PlotAttitude::m_instanceCount = 1;
PlotAttitude::PlotAttitude(QWidget* parent)
	: PlotItemBase(parent)
{
	m_titleColor = Qt::white;
	m_border_ColorStart = Qt::white;
	m_border_ColorEnd = Qt::white;
	m_bgColor = Qt::black;
	m_scaleColor_roll = Qt::white;
	m_lineColor_roll = Qt::white;
	m_textColor_roll = Qt::white;

	m_scaleColor_pitch = Qt::red;
	m_lineColor_pitch = Qt::red;	
	m_textColor_pitch = Qt::red;

	m_pitchValue = 0.0;
	m_rollValue = 0.0;

	m_startAngle_pitch = 0.0;
	m_endAngle_pitch = 360.0;
	m_startAngle_roll = 0.0;
	m_endAngle_roll = 360.0;
	m_coordNum_roll = 4;
	m_coordNum_pitch = 3;

	m_unit_roll = QString::fromLocal8Bit("°");
	m_unit_pitch = QString::fromLocal8Bit("°");
	m_decision_roll = 0;
	m_decision_pitch = 0;
	m_title = "Attitude";

	m_leftPadding = 40;
	m_rightPadding = 40;
	m_topPadding = 50;
	m_bottomPadding = 40;

	m_titleFont.setFamily("Microsoft YaHei");
	m_titleFont.setPointSizeF(16.0);

	m_scaleFont.setFamily("Microsoft YaHei");
	m_scaleFont.setPointSizeF(10.0);

	m_textFont.setFamily("Microsoft YaHei");
	m_textFont.setPointSizeF(10.0);

	QString name = QString("Attitude%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
}

PlotAttitude::~PlotAttitude()
{
}

void PlotAttitude::paintEvent(QPaintEvent* event)
{
	int width = this->width();
	int height = this->height();

	QFontMetricsF fm(m_titleFont);
	double topPadding = fm.size(Qt::TextSingleLine, m_title).height() + m_topPadding;
	int side = qMin(width - m_leftPadding - m_rightPadding, height - topPadding - m_bottomPadding);

	//画笔
	QPainter painter(this);
	painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
	painter.translate(width / 2, height / 2);

	//绘制标题
	drawTitle(&painter, side / 2);
	//绘制外边框
	drawBorder(&painter, side / 2);
	//绘制背景
	drawBg(&painter, side / 2);
	//绘制刻度尺
	drawScale_roll(&painter, side / 2);
	drawScale_pitch(&painter, side / 2);
	//绘制线条
	drawLine_roll(&painter, side / 2);
	drawLine_pitch(&painter, side / 2);
	//绘制文本
	drawText_roll(&painter, side / 2);
	drawText_pitch(&painter, side / 2);

	updateItems();
}

void PlotAttitude::drawTitle(QPainter * painter, int radius)
{
	painter->save();
	QFontMetricsF fm(m_titleFont);
	double w = fm.size(Qt::TextSingleLine, m_title).width();
	QFontMetricsF fm1(m_scaleFont);
	double h = fm1.size(Qt::TextSingleLine, m_title).height();
	
	painter->setFont(m_titleFont);
	painter->setPen(m_titleColor);
	painter->drawText(QPoint(-w / 2, -radius - h), m_title);

	painter->restore();
}

void PlotAttitude::drawBorder(QPainter *painter, int radius)
{
	painter->save();
	painter->setPen(Qt::SolidLine);
	QLinearGradient borderGradient(0, -radius, 0, radius);
	borderGradient.setColorAt(0, m_border_ColorStart);
	borderGradient.setColorAt(1, m_border_ColorEnd);
	painter->setBrush(borderGradient);
	painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
	painter->restore();
}

void PlotAttitude::drawBg(QPainter *painter, int radius)
{
	radius -= 2;
	painter->save();
	painter->setPen(Qt::NoPen);
	QLinearGradient borderGradient(0, -radius, 0, radius);
	m_bgColor.setAlpha(255);
	borderGradient.setColorAt(0, m_bgColor);
	m_bgColor.setAlpha(255);
	borderGradient.setColorAt(1, m_bgColor);
	painter->setBrush(borderGradient);
	painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);

	painter->restore();
}

void PlotAttitude::drawScale_roll(QPainter *painter, int radius)
{
	QFontMetricsF fm(m_scaleFont);

	painter->save();
	painter->setFont(m_scaleFont);
	painter->setPen(QPen(m_scaleColor_roll, 1));
	double eachMajor_roll = (m_endAngle_roll - m_startAngle_roll) / (double)m_coordNum_roll;
	double x, y, w, h;
	QString str,strNum;

	for (int i = 0; i < m_coordNum_roll; ++i)
	{
		strNum = QString::number(eachMajor_roll * i + m_startAngle_roll, 'f', m_decision_roll);
		str = QString("%1%2").arg(strNum).arg(m_unit_roll);
		w = fm.size(Qt::TextSingleLine, strNum).width() + 5;
		h = fm.size(Qt::TextSingleLine, strNum).height();
		double rad = qDegreesToRadians(eachMajor_roll * i);
		x = radius * sin(rad);
		y = radius * cos(rad) * (-1);

		painter->drawLine(0, 0, x, y);

		int R = radius + sqrt(pow(w/2,2)+pow(h/2,2));
		int x_R = R * sin(rad) - w/2;
		int y_R = R * cos(rad) * (-1) + h / 4;
			
		painter->drawText(x_R, y_R, str);
	}
	painter->restore();
}

void PlotAttitude::drawScale_pitch(QPainter *painter, int radius)
{
	QFontMetricsF fm(m_scaleFont);

	painter->save();
	painter->setFont(m_scaleFont);
	painter->setPen(QPen(m_scaleColor_pitch, 1));
	double x, y, w, h;
	QString str, strNum;
	double eachMajor_pitch = (m_endAngle_pitch - m_startAngle_pitch) / (double)(m_coordNum_pitch - 1);
	for (int i = 0; i < m_coordNum_pitch; ++i)
	{
		strNum = QString::number(eachMajor_pitch * i + m_startAngle_pitch, 'f', m_decision_pitch);
		str = QString("%1%2").arg(strNum).arg(m_unit_pitch);
		w = fm.size(Qt::TextSingleLine, str).width() + 10;
		h = fm.size(Qt::TextSingleLine, str).height();
		double rad = qDegreesToRadians(eachMajor_pitch * i);
		x = - w;
		if (i == 0)
			y = radius - 2 * radius / (m_coordNum_pitch - 1) * i - h / 4;
		else if(i == (m_coordNum_pitch - 1))
			y = radius - 2 * radius / (m_coordNum_pitch - 1) * i + h;
		else
			y = radius - 2 * radius / (m_coordNum_pitch - 1) * i + h / 4;

		painter->drawText(x, y, str);
	}

	painter->restore();
}

void PlotAttitude::drawLine_roll(QPainter *painter, int radius)
{
	painter->save();
	painter->setPen(QPen(m_lineColor_roll, 4));
	painter->rotate(fmodf(m_rollValue, 360) - m_startAngle_roll);
	painter->drawLine(QPoint(-radius / 2, 0), QPoint(radius / 2, 0));
	painter->drawLine(QPoint(0, 0), QPoint(0, -radius / 2));

	painter->restore();
}

void PlotAttitude::drawLine_pitch(QPainter *painter, int radius)
{
	painter->save();
	painter->setPen(QPen(m_lineColor_pitch, 4));
	if (fmodf(m_pitchValue,360) > m_startAngle_pitch && fmodf(m_pitchValue, 360) < m_endAngle_pitch)
	{ 
		double translate_y = (m_endAngle_pitch - fmodf(m_pitchValue, 360)) / (m_endAngle_pitch - m_startAngle_pitch) * 2 * radius - radius;
		painter->translate(0, translate_y);
		painter->drawLine(QPoint(-radius / 2, 0), QPoint(radius / 2, 0));
	}
	
	painter->restore();
}

void PlotAttitude::drawText_roll(QPainter * painter, int radius)
{
	painter->save();

	painter->setFont(m_textFont);
	painter->setPen(QPen(m_textColor_roll));

	QFontMetricsF fm(m_textFont);
	QString str, strNum;
	str = "Roll";
	strNum = QString::number(m_rollValue, 'f', m_decision_roll);
	
	painter->drawText(QPoint(radius - 30, radius - 30), str);
	double h = fm.size(Qt::TextSingleLine, str).height();
	str = QString("%1%2").arg(strNum).arg(m_unit_roll);
	painter->drawText(QPoint(radius - 30, radius - 30 + h), str);

	painter->restore();
}

void PlotAttitude::drawText_pitch(QPainter * painter, int radius)
{
	painter->save();
	painter->setFont(m_textFont);
	painter->setPen(QPen(m_textColor_pitch));

	QFontMetricsF fm(m_textFont);
	QString str, strNum;
	str = "Pitch";
	strNum = QString::number(m_pitchValue, 'f', m_decision_pitch);
	
	painter->drawText(QPoint(-radius, radius - 30), str);
	double h = fm.size(Qt::TextSingleLine, str).height();
	str = QString("%1%2").arg(strNum).arg(m_unit_pitch);
	painter->drawText(QPoint(-radius, radius - 30 + h), str);

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

QColor PlotAttitude::getBgColor() const
{
	return m_bgColor;
}

QColor PlotAttitude::getScaleColor_roll() const
{
	return m_scaleColor_roll;
}

QColor PlotAttitude::getScaleColor_pitch() const
{
	return m_scaleColor_pitch;
}

QColor PlotAttitude::getLineColor_roll() const
{
	return m_lineColor_roll;
}

QColor PlotAttitude::getLineColor_pitch() const
{
	return m_lineColor_pitch;
}

QColor PlotAttitude::getTextColor_roll() const
{
	return m_textColor_roll;
}

QColor PlotAttitude::getTextColor_pitch() const
{
	return m_textColor_pitch;
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

void PlotAttitude::updateItems()
{
	int xSize = m_xValueList.size();
	int ySize = m_yValueList.size();

	if (xSize <= ySize)
	{
		for (int i = 0; i < xSize; ++i)
		{
			slot_setRollValue(m_xValueList.at(i));
			slot_setPitchValue(m_yValueList.at(i));
		}
		for (int i = xSize; i < ySize; i++)
		{
			slot_setPitchValue(m_yValueList.at(i));
		}
	}
	else
	{
		for (int i = 0; i < ySize; ++i)
		{
			slot_setRollValue(m_xValueList.at(i));
			slot_setPitchValue(m_yValueList.at(i));
		}
		for (int i = ySize; i < xSize; i++)
		{
			slot_setRollValue(m_xValueList.at(i));
		}
	}
}

void PlotAttitude::slot_setBorderColorStart(const QColor &borderOutColorStart)
{
	m_border_ColorStart = borderOutColorStart;
	update();
}

void PlotAttitude::slot_setBorderColorEnd(const QColor &borderOutColorEnd)
{
	m_border_ColorEnd = borderOutColorEnd;
	update();
}

void PlotAttitude::slot_setBgColor(const QColor &bgColor)
{
	m_bgColor = bgColor;
	update();
}

void PlotAttitude::slot_setScaleColor_roll(const QColor &scaleColor)
{
	m_scaleColor_roll = scaleColor;
	update();
}

void PlotAttitude::slot_setScaleColor_pitch(const QColor &scaleColor)
{
	m_scaleColor_pitch = scaleColor;
	update();
}

void PlotAttitude::slot_setLineColor_roll(const QColor &lineColor)
{
	m_lineColor_roll = lineColor;
	update();
}

void PlotAttitude::slot_setLineColor_pitch(const QColor &lineColor)
{
	m_lineColor_pitch = lineColor;
	update();
}

void PlotAttitude::slot_setTextColor_roll(const QColor &textColor)
{
	m_textColor_roll = textColor;
	update();
}

void PlotAttitude::slot_setTextColor_pitch(const QColor &textColor)
{
	m_textColor_pitch = textColor;
	update();
}

void PlotAttitude::slot_setTitleColor(const QColor & titleColor)
{
	m_titleColor = titleColor;
	update();
}

void PlotAttitude::slot_setTitleFont(const QFont & font)
{
	m_titleFont = font;
	update();
}

void PlotAttitude::slot_setScaleFont(const QFont & font)
{
	m_scaleFont = font;
	update();
}

void PlotAttitude::slot_setTextFont(const QFont & font)
{
	m_textFont = font;
	update();
}

void PlotAttitude::slot_setTitle(QString title)
{
	m_title = title;
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

void PlotAttitude::slot_getCurrentSeconds(double secs)
{
	if (getPlotPairData().isEmpty())
		return;

	int isize = getPlotPairData().size();
	QString xcolumn = getPlotPairData().at(0).first;
	QString ycolumn = getPlotPairData().at(0).second;
	QStringList xlist = xcolumn.split("+");
	QStringList ylist = ycolumn.split("+");

	m_xValueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(xlist.at(0), xlist.at(1), secs);
	m_yValueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(ylist.at(0), ylist.at(1), secs);

	update();
}
