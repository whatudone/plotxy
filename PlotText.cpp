#include "PlotText.h"
#include "DataManager.h"
#include "AddPlotPair.h"
#include "PlotItemBase.h"
#include <QPainter>
#include <QStringList>
#include <QPen>
#include <QFont>
#include <QList>
#include <QDebug>

int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
	:PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;
	m_horiGridNum = m_verGridNum = 1;
	m_currTimeIndex = 0;

	m_started = false;

	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_defaultColor = Qt::gray;
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &PlotText::onTimeout);
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
	//以下绘制标题“Text Plot”
	QPainter painter(this);
	QPen pen;
	QFont font;
	QRect rect;

	pen.setColor(Qt::white);
	painter.setPen(pen);
	font.setPointSize(20);
	painter.setFont(font);
	rect.setRect(0, 0, width(), 0.1*height());
	painter.drawText(rect,Qt::AlignCenter|Qt::TextWordWrap,"Text Plot");

	//以下绘制n×m的格子
	QList<QPair<QString, QString>> dataList;
	QSet<QString> xset, yset;
	int i = 0,j = 0;
	dataList = getPlotPairData();
	for (int i = 0; i < dataList.size(); i++)
	{
		QString xIncludePlus = dataList.at(i).first;
		int pos = xIncludePlus.indexOf("+");
		QString xColumn = xIncludePlus.mid(0, pos);
		QString yColumn = xIncludePlus.mid(pos + 1);
		
		xset.insert(xColumn);
		yset.insert(yColumn);
		m_horiGridNum = xset.size() + 1;
		m_verGridNum = yset.size() + 1;
	}
	if (!dataList.empty())
	{
		int horiGridWidth = 0;
		if (m_horiGridNum)		//item水平方向延展
		{
			horiGridWidth = 0.9*width() / m_horiGridNum;
		}

		for (int i = 0; i < m_horiGridNum; i++)
		{
			QRect gridRect;
			//gridRect.setRect(  0.05*width()+i* horiGridWidth, 0.1*height(), horiGridWidth, 0.85*height());
			//painter.drawRect(gridRect);
			painter.drawLine(0.05*width() + i* horiGridWidth, 0.1*height(), 0.05*width() + i* horiGridWidth, 0.95*height());
		}

		int verGridWidth = 0;
		if (m_verGridNum)		//item水平方向延展
		{
			verGridWidth = 0.85*height() / m_verGridNum;
		}

		for (int i = 0; i < m_verGridNum; i++)
		{
			QRect gridRect;
			gridRect.setRect(0.05*width(), i * verGridWidth + 0.1*height(), 0.9*width(), verGridWidth);
			painter.drawRect(gridRect);

		}

		//以下为绘制X/Y轴item名字
		QRect rectXName, rectYName;
		for (auto it = xset.begin(); it != xset.end(); it++)
		{
			rectXName.setRect(0.05*width() + (i+1)* horiGridWidth, 0.1*height(),horiGridWidth,verGridWidth);
			painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, *it);
			i++;
		}
		for (auto it = yset.begin(); it != yset.end(); it++)
		{
			rectXName.setRect(0.05*width(), 0.1*height()+(1+j)*verGridWidth, horiGridWidth, verGridWidth);
			painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, *it);
			j++;
		}
		//以下为绘制对应的数据
		i = j = 0;
		for (auto it = xset.begin(); it != xset.end(); it++)
		{
			i++;
			for (auto it2 = yset.begin(); it2 != yset.end(); it2++)
			{
				j++;
				//动态数据
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
				rect.setRect(0.05*width()+i*horiGridWidth, 0.1*height()+j*verGridWidth, horiGridWidth, verGridWidth);
				painter.drawText(rect,Qt::AlignCenter|Qt::TextWrapAnywhere, QString::number(currValue, 'f', 2));
			}
			j = 0;
		}
	}




	//以下为用户自定义数据
	//QList<textUserData> list1;
	//QRect rect, rectErase;
	//list1 = addPP->getUserText();
	//for (int i = 0; i < list1.size(); i++)
	//{
	//	int x = list1.at(i).row;
	//	int y = list1.at(i).column;
	//	if (x <= m_verGridNum || y <= m_horiGridNum)
	//	{

	//		rect.setRect(y*horiGridWidth, x*verGridWidth, horiGridWidth, verGridWidth);
	//		rectErase.setRect(y*horiGridWidth + 3, x*verGridWidth + 3, horiGridWidth - 3, verGridWidth - 3);
	//		painter.eraseRect(rectErase);
	//		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, list1.at(i).str);
	//	}
	//	else
	//	{
	//		m_verGridNum = x;
	//		m_horiGridNum = y;


	//		rect.setRect(y*horiGridWidth, x*verGridWidth, horiGridWidth, verGridWidth);
	//		painter.eraseRect(rect);
	//		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, list1.at(i).str);

	//	}
	//}
}




void PlotText::onTimeout()
{
	m_currTimeIndex++;
	update();
}



