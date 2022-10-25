#include "PlotText.h"
#include "DataManager.h"
#include "AddPlotPair.h"
#include <QPainter>
#include <QStringList>
#include <QPen>
#include <QList>


int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
	:PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;

	m_currTimeIndex = 0;



	m_horiGridNum = 4;
	m_verGridNum = 5;

	m_started = false;

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_defaultColor = Qt::gray;
	m_timer = new QTimer(this);
	bool res = connect(m_timer, &QTimer::timeout, this, &PlotText::onTimeout);
}

PlotText::~PlotText()
{

}

void PlotText::onSwitch(bool bOn)
{
	if (!m_started)
	{
		m_timer->start(500);
		m_started = true;
	}
	else
	{
		m_timer->stop();
		m_started = false;
	}
}

void PlotText::paintEvent(QPaintEvent* event)
{
	AddPlotPair* addPP = AddPlotPair::m_getInstance();
	m_horiGridNum = addPP->textRowCount()+1;
	m_verGridNum = addPP->textCloumnCount()+1;

	//绘制x轴和y轴
	QPainter painter(this);
	QPen pen;
	QFont font;
	m_axisColor = Qt::white;
	pen.setColor(m_axisColor);
	pen.setWidth(2);

	painter.setPen(pen);
	painter.drawLine(QPoint(0, height()), QPoint(width(), height()));	//x轴
	painter.drawLine(QPoint(0, height()), QPoint(0, 0));		//y轴

																//绘制网格
	m_gridColor = Qt::white;
	pen.setColor(m_gridColor);
	painter.setPen(pen);

	QBrush brush;   //画刷。填充几何图形的调色板，由颜色和填充风格组成
	m_gridFillColor = Qt::white;
	brush.setColor(m_gridFillColor);
	brush.setStyle(Qt::SolidPattern);
	//painter.setBrush(brush);

	//拿set里面的string
	QSet<QString> set1,set2;
	set1 = addPP->m_temSet1;
	set2 = addPP->m_temSet2;
	int i = 0,j = 0;
	double horiGridWidth = 0;
	QList<double> list;


	if (m_horiGridNum)		//item水平方向延展
	{
		horiGridWidth = width() / m_horiGridNum;
	}
	pen.setColor(Qt::white);
	brush.setColor(Qt::white);

	font.setPointSize(18);
	painter.setFont(font);
	//横向（列）
	for (QSet<QString>::iterator it = set1.begin(); it != set1.end();it++)
	{
		i++;
		QRect gridRect,rect;
		gridRect.setRect(i* horiGridWidth, 0, horiGridWidth, height());
		painter.drawRect(gridRect);

		rect.setRect(i*horiGridWidth, 0, horiGridWidth, (height() / m_verGridNum));

		painter.drawText(rect,Qt::AlignCenter| Qt::TextWordWrap, *it);
	}

	double verGridWidth = 0;
	if (m_verGridNum)		//item水平方向延展
	{
		verGridWidth = height() / m_verGridNum;
	}
	//纵向（行）
	for (QSet<QString>::iterator it = set2.begin(); it != set2.end(); it++)
	{
		QRect gridRect,rect;
		gridRect.setRect(0, (j + 1) * verGridWidth, width(), verGridWidth);
		painter.drawRect(gridRect);

		rect.setRect(0, (j+1)  * verGridWidth, horiGridWidth, verGridWidth);

		painter.drawText(rect, Qt::AlignCenter|Qt::TextWordWrap, *it);
		j++;
	}
	//画数据
	i = 0;
	j = 0;
	
	for (QSet<QString>::iterator it = set1.begin(); it != set1.end(); it++)
	{
		i++;
		for (QSet<QString>::iterator it2 = set2.begin(); it2 != set2.end(); it2++)
		{
			j++;
			//动态数据
			QRect rect;
			QString currEntityType = *it;
			QString currEntityAttr = *it2;
			auto dataMap = DataManager::getInstance()->getDataMap();
			if (!dataMap.contains(currEntityType))
			{
				continue;
			}
			if (!dataMap.value(currEntityType).contains(currEntityAttr))
			{
				continue;
			}

			if (m_currTimeIndex >= dataMap.value(currEntityType).value(currEntityAttr).size())
			{
				continue;
			}

			//*获取当前Attr值
			double currValue = dataMap.value(currEntityType).value(currEntityAttr).at(m_currTimeIndex);
			rect.setRect(i*horiGridWidth, j*verGridWidth, horiGridWidth, verGridWidth);
			painter.drawText(rect,Qt::AlignCenter|Qt::TextWrapAnywhere, QString::number(currValue, 'f', 2));
		}
		j = 0;
	}


}
void PlotText::onTimeout()
{
	m_currTimeIndex++;
	update();
}


void PlotText::drawRect(double itemIndex, bool bHorizontal, double itemLength, double leftBoundary, double rightBoundary, QColor color)
{
	QPainter painter(this);
	QPen pen;
	color = Qt::white;
	pen.setColor(color);
	QBrush brush;
	brush.setColor(color);
	brush.setStyle(Qt::SolidPattern);

	painter.setPen(pen);
	painter.setBrush(brush);

	


	double height = this->height();
	double width = this->width();

	QRect rect;
	if (bHorizontal)
	{
		rect.setRect(leftBoundary, height - (m_leftPadding + itemIndex * (itemLength + m_interPadding) + itemLength), rightBoundary - leftBoundary, itemLength);
	}
	else
	{
		rect.setRect(m_leftPadding + itemIndex * (itemLength + m_interPadding), height - rightBoundary, itemLength, rightBoundary - leftBoundary);
	}
	painter.drawRect(rect);
}
