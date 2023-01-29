#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QPair>
#include <QList>
#include <QPainterPath>
#include <QBrush>
#include <QFontMetricsF>

#include "PlotLight.h"
#include "DataManager.h"
#include "PlotItemBase.h"


int PlotLight::m_instanceCount = 1;
PlotLight::PlotLight(QWidget* parent)
	:PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;

	m_currTimeIndex = 0;
	m_started = false;
	//m_brushColor = Qt::gray;

	QString name = QString("Light%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_temBrush.setColor(Qt::gray);
	m_defaultColor = Qt::gray;
	m_timer = new QTimer(this);
	//connect(m_timer, &QTimer::timeout, this, &PlotLight::onTimeout);
}

PlotLight::~PlotLight()
{

}

void PlotLight::paintEvent(QPaintEvent* event)
{
	//以下为绘制表头
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	QFont font;
	QRect rect;
	QSet<QString> yset;
	QPainterPath path;
	QVector<DataPair*> dataVector = getDataPair();

	m_axisColor = Qt::white;
	pen.setColor(m_axisColor);
	font.setPointSize(20);
	QFontMetricsF fm(font);
	double as = fm.ascent();
	rect.setRect(0, 0, width(), as);
	painter.setPen(pen);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter, "Events");


	//以下为绘制文字的内容和框框
	double verGridWidth = 0;
	double horGridWidth = 0;
	m_horiGridNum = 1;

	if (!dataVector.empty())
	{
		rect.setRect(0.05*width(), 0.1*height(), 0.9*width(), 0.85*height());
		painter.drawRect(rect);

	}
	painter.setBrush(QBrush(Qt::gray));
	for (int i = 0; i < dataVector.size(); i++)
	{	
		painter.setBrush(QBrush(Qt::gray));
		yset.insert(dataVector.at(i)->getDataPair().first);
		m_verGridNum = dataVector.size();
		verGridWidth = 0.85*height() / m_verGridNum;
		horGridWidth = (0.85*width() - 0.1*height()) / m_horiGridNum;//整个宽减去框框宽，减去圆圈占宽
		rect.setRect(0.1*width() + 0.1*height(), 0.1*height() + i*verGridWidth, horGridWidth, verGridWidth);
		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, dataVector.at(i)->getDataPair().first);
		judgeLight();	
	}
	drawLight(painter, verGridWidth);
	if (!m_brush.isEmpty())
		m_brush.clear();
}

void PlotLight::drawLight(QPainter &painter,double &verGridWidth)
{
	for (int i = 0; i < getDataPair().size(); i++)
	{
		painter.setBrush(QBrush(Qt::gray));
		if (m_brush.isEmpty())
			painter.setBrush(QBrush(Qt::gray));
		else if ((m_brush.at(i).color() == QColor(0,0,0,255)))
			painter.setBrush(QBrush(Qt::gray));
		else
			painter.setBrush(QBrush(m_brush.at(i).color()));

		painter.drawEllipse(0.1*width(), 0.1*height() + (i + 0.5)*verGridWidth - 0.05*height(), 0.1*height(), 0.1*height());
		painter.setBrush(QBrush(Qt::gray));
	}
}


void PlotLight::judgeLight()
{
	if (m_lightMap.isEmpty())
		return;
	QBrush iBrush;
	QString entityAndAtrr;
	QString judge;
	QString threshold;
	QString redOrGreen;
	QList<QString> partUserLightData;
	QStringList docEntityAndAttr;
	QString temEntityAndAtrr = " ";
	for (int i = 0; i < getDataPair().size(); i++)
	{
		docEntityAndAttr.push_back(getDataPair().at(i)->getDataPair().first);
	}
	int isize = 0;
	int icount = 0;

	if (m_userLightData.size() > 1)
	{
		for (int j = 0; j < getDataPair().size(); j++)
		{
			iBrush.setColor(Qt::gray);
			for (int i = 0; i < m_userLightData.size(); i++)
			{
				
				icount = i * 5;
				partUserLightData = m_userLightData.at(i);
				QString temEntity = partUserLightData.at(icount++);
				entityAndAtrr = temEntity + "+" + partUserLightData.at(icount++);
				judge = partUserLightData.at(icount++);
				threshold = partUserLightData.at(icount++);
	 			redOrGreen = partUserLightData.at(icount++);
				//iBrush.setColor(Qt::gray);

				if (QString::compare(redOrGreen, "G/R/Y") == 0)
					continue;
				QList <long double> lightThreshold = m_lightMap.value(entityAndAtrr);
				if (!lightThreshold.size() == 0)
				{
					if (entityAndAtrr == docEntityAndAttr.at(j))
					{
						if (QString::compare(judge, ">") == 0)
						{
							if (lightThreshold.back() > threshold.toDouble())
							{
								if (QString::compare(redOrGreen, "G") == 0)
									iBrush.setColor(Qt::green);
								else if (QString::compare(redOrGreen, "R") == 0)
									iBrush.setColor(Qt::red);
								else if (QString::compare(redOrGreen, "Y") == 0)
									iBrush.setColor(Qt::yellow);
							}
						}
						else if (QString::compare(judge, "<") == 0)
						{
							if (lightThreshold.back() < threshold.toDouble())
							{
								if (QString::compare(redOrGreen, "G") == 0)
									iBrush.setColor(Qt::green);
								else if (QString::compare(redOrGreen, "R") == 0)
									iBrush.setColor(Qt::red);
								else if (QString::compare(redOrGreen, "Y") == 0)
									iBrush.setColor(Qt::yellow);
							}
						}
					}
				}	
			}
				m_brush.push_back(iBrush);

		}
	}
	update();
}


//for (int i = 0; i < m_userLightData.size(); i++)
//{
//	//用户输入的数据
//	QBrush iBrush;
//	partUserLightData = m_userLightData.at(i);
//	QString temEntity = partUserLightData.at(icount++);
//	entityAndAtrr = temEntity + "+" + partUserLightData.at(icount++);
//	judge = partUserLightData.at(icount++);
//	threshold = partUserLightData.at(icount++);
//	redOrGreen = partUserLightData.at(icount++);
//	m_brush.push_back(iBrush);

//	//文档拿来的数据

//	QList <long double> lightThreshold = m_lightMap.value(entityAndAtrr);
//	//判断颜色
//	for (int i = 0; i < m_userLightData.size(); i++)
//	{
//		for (int j = 0; j < getPlotPairData().size(); j++)
//		{
//			if (entityAndAtrr == docEntityAndAttr.at(j))
//			{
//				if (judge == ">")
//				{
//					if (lightThreshold.back() > threshold.toDouble())
//					{
//						iBrush = m_brush.at(j);
//						if (redOrGreen == QString::fromLocal8Bit("绿") || redOrGreen == QString::fromLocal8Bit("绿色"))
//							iBrush.setColor(Qt::green);
//						else if (redOrGreen == QString::fromLocal8Bit("红") || redOrGreen == QString::fromLocal8Bit("红色"))
//							iBrush.setColor(Qt::red);
//						else if (redOrGreen == QString::fromLocal8Bit("黄") || redOrGreen == QString::fromLocal8Bit("黄色"))
//							iBrush.setColor(Qt::yellow);
//					}
//				}
//				else if (judge == "<")
//				{
//					if (lightThreshold.back() < threshold.toDouble())
//					{
//						if (redOrGreen == QString::fromLocal8Bit("绿") || redOrGreen == QString::fromLocal8Bit("绿色"))
//							iBrush.setColor(Qt::green);
//						else if (redOrGreen == QString::fromLocal8Bit("红") || redOrGreen == QString::fromLocal8Bit("红色"))
//							iBrush.setColor(Qt::red);
//						else if (redOrGreen == QString::fromLocal8Bit("黄") || redOrGreen == QString::fromLocal8Bit("黄色"))
//							iBrush.setColor(Qt::yellow);
//					}
//				}
//			}
//		}
//	}




void PlotLight::slot_getLightData(QList<QList<QString>> userLightData)
{
	if (userLightData.size() > 1)
	{
		
		m_userLightData = userLightData;
		update();
	}
}
void PlotLight::slot_onAddButtonClicked()
{

	update();
}

void PlotLight::slot_getCurrentSeconds(double secs)
{
	if (getDataPair().isEmpty())
		return;
	int isize = getDataPair().size();
	int entityNum = 0;
	int attriNum = 0;
	//m_entityName.clear();
	//m_attriName.clear();
	for (int i = 0; i < isize; i++)
	{
		QString getLightData = getDataPair().at(i)->getDataPair().first;
		QList<QString> lightValueList = getLightData.split("+");
		m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(lightValueList.front(), lightValueList.back(), secs);
		m_lightDataList.push_back(m_valueList.back());
		m_lightMap.insert(getLightData, m_lightDataList);
	}

	//	if (m_entityName.isEmpty())
	//		m_entityName.push_back(textValueList.front());
	//	for (int i = 0; i < m_entityName.size(); i++)
	//	{
	//		if (textValueList.front() == m_entityName.at(i))
	//			entityNum++;
	//	}
	//	if (entityNum == 0)
	//		m_entityName.push_back(textValueList.front());
	//	entityNum = 0;

	//	if (m_attriName.isEmpty())
	//		m_attriName.push_back(textValueList.back());
	//	for (int i = 0; i < m_attriName.size(); i++)
	//	{
	//		if (textValueList.back() == m_attriName.at(i))
	//			attriNum++;
	//	}
	//	if (attriNum == 0)
	//		m_attriName.push_back(textValueList.back());
	//	attriNum = 0;
	//}
	//for (auto ite = m_entityName.begin(); ite != m_entityName.end(); ite++)
	//{
	//	for (auto ita = m_attriName.begin(); ita != m_attriName.end(); ita++)
	//	{
	//		m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(*ite, *ita, secs);
	//		if (m_valueList.isEmpty())
	//			m_valueList.push_back(0);
	//		m_temValueList.push_back(m_valueList);

	//	}
	//}
	update();
}