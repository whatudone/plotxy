#include "PlotBar.h"
#include <QDebug>
#include <QPainter>
#include "DataManager.h"

PlotBar::PlotBar(QWidget* parent)
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

	m_defaultColor = Qt::gray;
	m_timer = new QTimer(this);
	bool res = connect(m_timer,&QTimer::timeout,this, &PlotBar::onTimeout);
}

PlotBar::~PlotBar()
{

}     

void PlotBar::onSwitch(bool bOn)
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

void PlotBar::onTimeout()
{
	m_currTimeIndex++; 
	update();
}

void PlotBar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
	m_thresholdColorMap = targetMap;
}


void PlotBar::setPlotItemExtentionDirec(bool horizontal)
{
	m_bHorizontal = horizontal;
}

void PlotBar::setLeftPadding(int leftPadding)
{
	m_leftPadding = leftPadding;
}

void PlotBar::setRightPadding(int rightPadding)
{
	m_rightPadding = rightPadding;
}

void PlotBar::setInterPadding(int interPadding)
{
	m_interPadding = interPadding;
}

void PlotBar::addPlotDataPair(const QString& entityType, const QString& entityAttr)
{
	if (entityType.isEmpty() || entityAttr.isEmpty())
		return;
	
	m_entityTypeList.append(entityType);
	m_entityAttrList.append(entityAttr);
}


void PlotBar::drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color)
{
	QPainter painter(this);
	QPen pen;
	pen.setColor(color);
	QBrush brush;
	brush.setColor(color);
	brush.setStyle(Qt::SolidPattern);

	painter.setPen(pen);
	painter.setBrush(brush);

	int height = this->height();
	int width = this->width();

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

//实现核心绘制逻辑
void PlotBar::updateItems()
{
	if (m_entityTypeList.isEmpty() || m_entityAttrList.isEmpty())
	{
		qDebug() << "PlotBar::updateItems either m_entityTypeList or m_entityAttrList is empty." << endl;
		return;
	}

	if (m_entityTypeList.size() != m_entityAttrList.size())
	{
		qDebug() << "PlotBar::updateItems  m_entityTypeList and m_entityAttrList don't match." << endl;
		return;
	}

	//首先计算每个item的宽度/高度
	int perItemLength = 0;
	int width = this->width();
	int height = this->height();
	if (m_bHorizontal)		//item水平方向延展
	{
		perItemLength = (height - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
	}
	else					//item垂直方向延展
	{
		perItemLength = (width - m_leftPadding - m_rightPadding - (m_entityTypeList.size() - 1) * m_interPadding) / m_entityTypeList.size();
	}

	//逐item遍历
	for (int itemIndex = 0; itemIndex < m_entityTypeList.size(); itemIndex++)
	{
		QString currEntityType = m_entityTypeList.at(itemIndex);
		QString currEntityAttr = m_entityAttrList.at(itemIndex);

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

		QString currKey = currEntityType + '_' + currEntityAttr;
		if (!m_thresholdColorMap.contains(currKey) || m_thresholdColorMap.value(currKey).keys().isEmpty())
		{
			//没有设置阈值，则无需分开绘制多个矩形，以默认颜色绘制一个即可
			drawRect(itemIndex,m_bHorizontal, perItemLength, 0,currValue, m_defaultColor);
			continue;
		}

		//根据颜色阈值来分开绘制
		auto colorMap = m_thresholdColorMap.value(currKey);
		QList<int> thresholdList = colorMap.keys();
		if (currValue < thresholdList.first())
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, 0,currValue, m_defaultColor);
			continue;
		}

		//以默认颜色绘制第一个矩形
		drawRect(itemIndex, m_bHorizontal, perItemLength, 0,thresholdList.first(), m_defaultColor);

		if (thresholdList.size() == 1)
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.first(), currValue, colorMap.value(thresholdList.first()));
			continue;
		}

		for (int i = 0;i < thresholdList.size() -1;i++)
		{
			QColor currColor = colorMap.value(thresholdList.at(i));

			if (currValue < thresholdList.at(i + 1))
			{
				drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), currValue, colorMap.value(thresholdList.at(i)));
				break;
			}
			else
			{
				drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.at(i), thresholdList.at(i+1), colorMap.value(thresholdList.at(i)));
			}
		}

		if (currValue > thresholdList.last())
		{
			drawRect(itemIndex, m_bHorizontal, perItemLength, thresholdList.last(), currValue, colorMap.value(thresholdList.last()));
			continue;
		}
	}
	update();
}

//
void PlotBar::paintEvent(QPaintEvent* event)
{
	//绘制x轴和y轴
	QPainter painter(this);
	QPen pen; 
	pen.setColor(m_axisColor);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.drawLine(QPoint(0, height()), QPoint(width(), height()));	//x轴
	painter.drawLine(QPoint(0, height()), QPoint(0,0));		//y轴

	//绘制网格
	pen.setColor(m_gridColor);
	painter.setPen(pen);

	QBrush brush;   //画刷。填充几何图形的调色板，由颜色和填充风格组成
	brush.setColor(m_gridFillColor);
	brush.setStyle(Qt::SolidPattern);
	//painter.setBrush(brush);

	int horiGridWidth = 0;
	if (m_horiGridNum)		//item水平方向延展
	{
		horiGridWidth = width() / m_horiGridNum;
	}

	for (int i=0;i < m_horiGridNum;i++)
	{
		QRect gridRect;
		gridRect.setRect(i* horiGridWidth, 0, horiGridWidth, height());
		painter.drawRect(gridRect);
	}

	int verGridWidth = 0;
	if (m_verGridNum)		//item水平方向延展
	{
		verGridWidth = height() / m_verGridNum;
	}

	for (int i = 0; i < m_verGridNum; i++)
	{
		QRect gridRect;
		gridRect.setRect(0, (i+1) * verGridWidth, width(), verGridWidth);
		painter.drawRect(gridRect);
	}

	if(m_started)
		updateItems();
}