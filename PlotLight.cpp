#include "PlotLight.h"
#include "DataManager.h"
#include "PlotItemBase.h"
#include <QBrush>
#include <QDebug>
#include <QFontMetricsF>
#include <QList>
#include <QMessageBox>
#include <QPainter>
#include <QPainterPath>
#include <QPair>
#include <QPen>
#include <QString>

int PlotLight::m_instanceCount = 1;
PlotLight::PlotLight(QWidget* parent)
    : PlotItemBase(parent)
{
	m_bHorizontal = true;
	m_circleRadius = 0;
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
}

PlotLight::~PlotLight() {}

void PlotLight::paintEvent(QPaintEvent* event)
{
	//以下为绘制表头
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);
	QPen pen;
	QFont font;
	QRect rect;
	QSet<QString> yset;

	QVector<DataPair*> dataVector = getDataPair();
	QFontMetricsF fm(font);
	double as = fm.ascent();
	m_axisColor = Qt::white;
	pen.setColor(m_axisColor);
	font.setPointSize(20);
    rect.setRect(0, 0, width(), 0.1 * height() + as);
	painter.setPen(pen);
	painter.setFont(font);
    drawTitle(painter, rect);
	//以下为绘制文字的内容和框框
	double verGridWidth = 0;
	double horGridWidth = 0;
	m_horiGridNum = 1;
    if(!dataVector.empty())
	{
		pen.setStyle(getGridStyle());
		painter.setPen(pen);
        rect.setRect(0.05 * width(), 0.1 * height() + as, 0.9 * width(), 0.85 * height() - as);
		painter.drawRect(rect);
	}
    for(int i = 0; i < dataVector.size(); i++)
	{
		painter.setBrush(QBrush(Qt::gray));
		yset.insert(dataVector.at(i)->getDataPair().first);
		m_verGridNum = dataVector.size();
        verGridWidth = (0.85 * height() - as) / m_verGridNum;
        horGridWidth =
            (0.8 * width() - 2 * m_circleRadius) / m_horiGridNum; //整个宽减去框框宽，减去圆圈占宽
        rect.setRect(0.15 * width() + 2 * m_circleRadius,
                     as + 0.1 * height() + i * verGridWidth,
                     horGridWidth,
                     verGridWidth);
        painter.drawText(
            rect, Qt::AlignCenter | Qt::TextWordWrap, dataVector.at(i)->getDataPair().first);
		judgeLight();
	}
    drawLight(painter, verGridWidth, as);
    if(!m_brush.isEmpty())
		m_brush.clear();
    PlotItemBase::paintEvent(event);
}

void PlotLight::drawTitle(QPainter& painter, QRect& rect)
{
	QFont font;
	QString lightTitle;
	QPen pen;
	font = getTitleFont();
	font.setPointSize(getTitleFontSize());
	lightTitle = getTitle();
	pen.setColor(getTitleColor());
	painter.setFont(font);
	painter.setPen(pen);
    if(lightTitle.isEmpty())
		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "Events");
	else
		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, lightTitle);
	update();
}

void PlotLight::drawLight(QPainter& painter, double& verGridWidth, double& as)
{
	setCircleRadius(as);
    for(int i = 0; i < getDataPair().size(); ++i)
	{
		painter.setBrush(QBrush(Qt::gray));
        if(m_brush.isEmpty())
			painter.setBrush(QBrush(Qt::gray));
        else if((m_brush.at(i).color() == QColor(0, 0, 0, 255)))
			painter.setBrush(QBrush(Qt::gray));
		else
			painter.setBrush(QBrush(m_brush.at(i).color()));
		//painter.drawEllipse(0.1*width(), as+0.11*height() + i*verGridWidth, m_circleRadius*2, 2*m_circleRadius);
        QPointF temPoint(0.1 * width() + m_circleRadius,
                         as + 0.1 * height() + i * verGridWidth + verGridWidth / 2);
		painter.drawEllipse(temPoint, m_circleRadius, m_circleRadius);
		painter.setBrush(QBrush(Qt::gray));
	}
}

void PlotLight::judgeLight()
{
    if(m_lightMap.isEmpty())
		return;
	QBrush iBrush;
	QString entityAndAtrr;
	QString judge;
	QString threshold;
	QString redOrGreen;
	QList<QString> partUserLightData;
	QStringList docEntityAndAttr;
	QString temEntityAndAtrr = " ";
    for(int i = 0; i < getDataPair().size(); i++)
		docEntityAndAttr.push_back(getDataPair().at(i)->getDataPair().first);
	int isize = 0;
	int icount = 0;
    if(m_userLightData.size() > 1)
	{
        for(int j = 0; j < getDataPair().size(); j++)
		{
			iBrush.setColor(Qt::gray);
            for(int i = 0; i < m_userLightData.size(); i++)
			{
				icount = i * 4;
                partUserLightData = m_userLightData.at(i);
				entityAndAtrr = partUserLightData.at(icount++);
				judge = partUserLightData.at(icount++);
				threshold = partUserLightData.at(icount++);
				redOrGreen = partUserLightData.at(icount++);
                if(QString::compare(redOrGreen, "G/R/Y") == 0)
					continue;
                QList<long double> lightThreshold = m_lightMap.value(entityAndAtrr);
                if(!lightThreshold.size() == 0)
				{
                    if(entityAndAtrr == docEntityAndAttr.at(j))
					{
                        if(QString::compare(judge, QString("≥")) == 0)
						{
                            if(lightThreshold.back() >= threshold.toDouble())
							{
                                if(QString::compare(redOrGreen, "Green") == 0)
									iBrush.setColor(Qt::green);
                                else if(QString::compare(redOrGreen, "Red") == 0)
									iBrush.setColor(Qt::red);
                                else if(QString::compare(redOrGreen, "Yellow") == 0)
									iBrush.setColor(Qt::yellow);
							}
						}
                        else if(QString::compare(judge, "<") == 0)
						{
                            if(lightThreshold.back() < threshold.toDouble())
							{
                                if(QString::compare(redOrGreen, "Green") == 0)
									iBrush.setColor(Qt::green);
                                else if(QString::compare(redOrGreen, "Red") == 0)
									iBrush.setColor(Qt::red);
                                else if(QString::compare(redOrGreen, "Yellow") == 0)
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

void PlotLight::setCircleRadius(double& as)
{
    m_circleRadius = (0.85 * height() - as) / (2 * m_verGridNum);
    if((m_circleRadius > 0.1 * height()) || (m_circleRadius > 0.1 * width()))
        (height() > width()) ? m_circleRadius = 0.1 * width() : m_circleRadius = 0.1 * height();
}

void PlotLight::setGridStyle(GridStyle gridStyle)
{
    switch(gridStyle)
	{
	case SOLIDLINE:
		m_gridStyle = Qt::SolidLine;
		break;
	case DASHLINE:
		m_gridStyle = Qt::DashLine;
		break;
	case DOTLINE:
		m_gridStyle = Qt::DotLine;
		break;
	case DASHDOTLINE:
		m_gridStyle = Qt::DashDotLine;
		break;
	default:
		break;
	}
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
//						if (redOrGreen == QString("绿") || redOrGreen == QString("绿色"))
//							iBrush.setColor(Qt::green);
//						else if (redOrGreen == QString("红") || redOrGreen == QString("红色"))
//							iBrush.setColor(Qt::red);
//						else if (redOrGreen == QString("黄") || redOrGreen == QString("黄色"))
//							iBrush.setColor(Qt::yellow);
//					}
//				}
//				else if (judge == "<")
//				{
//					if (lightThreshold.back() < threshold.toDouble())
//					{
//						if (redOrGreen == QString("绿") || redOrGreen == QString("绿色"))
//							iBrush.setColor(Qt::green);
//						else if (redOrGreen == QString("红") || redOrGreen == QString("红色"))
//							iBrush.setColor(Qt::red);
//						else if (redOrGreen == QString("黄") || redOrGreen == QString("黄色"))
//							iBrush.setColor(Qt::yellow);
//					}
//				}
//			}
//		}
//	}

void PlotLight::slot_getLightData(QList<QList<QString>> userLightData)
{
    if(userLightData.size() > 1)
	{
		m_userLightData = userLightData;
		update();
	}
}

void PlotLight::slot_onAddButtonClicked()
{
	update();
}

void PlotLight::onGetCurrentSeconds(double secs)
{
    if(getDataPair().isEmpty())
		return;
	int isize = getDataPair().size();
	int entityNum = 0;
	int attriNum = 0;
	//m_entityName.clear();
	//m_attriName.clear();
    for(int i = 0; i < isize; i++)
	{
		QString getLightData = getDataPair().at(i)->getDataPair().first;
		QList<QString> lightValueList = getLightData.split("+");
        m_valueList = DataManager::getInstance()->getEntityAttr_MaxPartValue_List(
            lightValueList.front(), lightValueList.back(), secs);
        if(!m_valueList.isEmpty())
		{
			m_lightDataList.push_back(m_valueList.back());
			m_lightMap.insert(getLightData, m_lightDataList);
		}
		else
		{
            QMessageBox* noDataMessageBox = new QMessageBox(nullptr);
			noDataMessageBox->setWindowTitle(QString("空数据警告"));
			noDataMessageBox->setText(QString("请检查所选项中是否存在空数据"));
			noDataMessageBox->show();
			return;
		}
	}
	update();
}
