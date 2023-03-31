#include "PlotText.h"
#include "AddPlotPair.h"
#include "DataManager.h"
#include "PlotItemBase.h"
#include "PlotManager.h"
#include "colorbutton.h"
#include "constdef.h"
#include <QDebug>
#include <QFont>
#include <QList>
#include <QPainter>
#include <QPen>
#include <QStringList>

int PlotText::m_instanceCount = 1;
PlotText::PlotText(QWidget* parent)
    : PlotItemBase(parent)
{
	m_bHorizontal = true;

	m_leftPadding = 50;
	m_rightPadding = 50;
	m_interPadding = 20;
	m_horiGridNum = m_verGridNum = 1;
	m_currTimeIndex = 0;

	m_gridWidth = 1;
	m_gridColor = Qt::white;
	m_started = false;
	m_axisColor = Qt::white;
	m_axisWidth = 1;
	m_secValue = -1;
	QString name = QString("Text%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;
	//connect(m_timer, &QTimer::timeout, this, &PlotText::onTimeout);
}

PlotText::~PlotText() {}

void PlotText::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	QPen pen;
	QFont font, titleFont;
	QRect rect;
    QVector<DataPair*> dataVector = getDataPairs();
	QSet<QString> xset, yset;
	int i = 0, j = 0;
	int horGT = 0;
	int verGT = 0;
	pen.setColor(Qt::white);
	painter.setPen(pen);
	font.setPointSize(20);
	titleFont.setPointSize(getTitleFontSize());
	painter.setFont(font);
	QFontMetricsF fm(titleFont);
	double as = fm.ascent();
	rect.setRect(0, 0, width(), as);
	int horiGridWidth = 0;
    if(m_horiGridNum) //item水平方向延展
	{
        horiGridWidth = 0.9 * width() / m_horiGridNum;
	}
	int verGridWidth = 0;
    if(m_verGridNum) //item水平方向延展
	{
        verGridWidth = (0.93 * height() - as) / m_verGridNum;
	}
	//以下为绘制表格title名字
    drawTitleText(painter, rect);
	//以下绘制n×m的格子
	drawNMCell(painter, xset, yset, dataVector, horiGridWidth, verGridWidth, as);
	//以下为绘制X/Y轴item名字
	pen.setColor(Qt::white);
	pen.setWidth(3);
	pen.setStyle(Qt::SolidLine);
	painter.setPen(pen);
    if(!getDataPairs().isEmpty())
	{
		drawXYTitle(painter, horiGridWidth, verGridWidth, dataVector, as);
		//以下为绘制对应的数据，没有调用drawData函数，后期如果需要可以调用下。
        if(m_temValueList.isEmpty())
			return;
		else
		{
            painter.drawText(0.05 * width(),
                             as + 0.02 * height(),
                             horiGridWidth,
                             verGridWidth,
                             Qt::AlignCenter | Qt::TextWrapAnywhere,
                             QString("0表示0或无数据"));
            for(int i = m_entityName.size() - 1; i != -1; i--)
			{
                for(int j = m_attriName.size() - 1; j != -1; j--)
				{
                    rect.setRect(0.05 * width() + (1 + horGT) * horiGridWidth,
                                 as + 0.02 * height() + (1 + verGT) * verGridWidth,
                                 horiGridWidth,
                                 verGridWidth);
					//painter.drawText(rect, QString::number(*(m_valueListVector.at(j).end()),'f',2));
                    painter.drawText(rect,
                                     Qt::AlignCenter | Qt::TextWrapAnywhere,
                                     QString::number(m_temValueList
                                                         .at(m_temValueList.size() - 1 - j -
                                                             i * m_attriName.size())
                                                         .back(),
                                                     'f',
                                                     6));
					update();
					verGT++;
				}
				horGT++;
				verGT = 0;
			}
		}
	}
    PlotItemBase::paintEvent(event);
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

void PlotText::drawTitleText(QPainter& painter, QRect& rect)
{
	QFont titleFont;
	QPen pen;
	titleFont = getTitleFont();
	titleFont.setPointSize(getTitleFontSize());
	pen.setColor(getTitleColor());
	painter.setFont(titleFont);
	QString titleName = getTitle();
	painter.setPen(pen);
    if(titleName.isEmpty())
		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, "Text Plot");
	else
		painter.drawText(rect, Qt::AlignCenter | Qt::TextWordWrap, titleName);
}

void PlotText::drawData(QSet<QString>& xset,
                        QSet<QString>& yset,
                        int& horiGridWidth,
                        int& verGridWidth)
{
    if(getDataPairs().isEmpty())
	{
		return;
	}
    if(m_temValueList.isEmpty())
	{
		return;
	}
	int i = 0;
	int j = 0;
	QRect rect;
	QPainter painter;
    for(auto it = xset.begin(); it != xset.end(); it++)
	{
        for(auto it2 = yset.begin(); it2 != yset.end(); it2++)
		{
			j++;
            rect.setRect(0.05 * width() + (1 + i) * horiGridWidth,
                         0.1 * height() + j * verGridWidth,
                         horiGridWidth,
                         verGridWidth);
            painter.drawText(rect,
                             Qt::AlignCenter | Qt::TextWrapAnywhere,
                             QString::number((m_temValueList.at(j).back()), 'f', 2));
		}
		i++;
		j = 0;
	}
	//m_attriName.clear();
	//m_entityName.clear();
	//update();
}

void PlotText::updateDataForDataPairsByTime(double secs)
{

    int isize = getDataPairs().size();
    int entityNum = 0;
    int attriNum = 0;
    m_entityName.clear();
    m_attriName.clear();
    m_temValueList.clear();
    m_valueList.clear();
    for(int i = 0; i < isize; i++)
    {
        QString getTextData = getDataPairs().at(i)->getDataPair().first;
        QList<QString> textValueList = getTextData.split("+");
        if(m_entityName.isEmpty())
            m_entityName.push_back(textValueList.front());
        for(int i = 0; i < m_entityName.size(); i++)
        {
            if(textValueList.front() == m_entityName.at(i))
                entityNum++;
        }
        if(entityNum == 0)
            m_entityName.push_back(textValueList.front());
        entityNum = 0;

        if(m_attriName.isEmpty())
            m_attriName.push_back(textValueList.back());
        for(int i = 0; i < m_attriName.size(); i++)
        {
            if(textValueList.back() == m_attriName.at(i))
                attriNum++;
        }
        if(attriNum == 0)
            m_attriName.push_back(textValueList.back());
        attriNum = 0;
    }
    for(auto ite = m_entityName.begin(); ite != m_entityName.end(); ite++)
    {
        for(auto ita = m_attriName.begin(); ita != m_attriName.end(); ita++)
        {
            m_valueList =
                DataManager::getInstance()->getEntityAttr_MaxPartValue_List(*ite, *ita, secs);
            if(m_valueList.isEmpty())
                m_valueList.push_back(0);
            m_temValueList.push_back(m_valueList);
        }
    }
}

void PlotText::drawXYTitle(QPainter& painter,
                           int& horiGridWidth,
                           int& verGridWidth,
                           const QVector<DataPair*>& dataVector,
                           double& as)
{
    QRect rectXName;

	int icount = 0;
	QFont font;
	QPen fontPen;
	fontPen.setColor(getTickLabelColor());
	font = getTickLabelFont();
	font.setPointSize(getTickLabelFontSize());
	painter.setFont(font);
	painter.setPen(fontPen);
    for(int i = 0; i < dataVector.size(); i++)
	{
        DataPair* temDataPair = dataVector.at(i);
		QPair<QString, QString> temPair = temDataPair->getDataPair();
		QString xIncludePlus = temPair.first;
		QString xColumn = xIncludePlus.split("+").front();
		QString yColumn = xIncludePlus.split("+").back();
        if(m_xColumnList.isEmpty())
			m_xColumnList.push_back(xColumn);
		else
		{
            for(int j = 0; j < m_xColumnList.size(); j++)
			{
                if(QString::compare(xColumn, m_xColumnList.at(j)) == 0)
					icount++;
			}
            if(icount == 0)
				m_xColumnList.push_back(xColumn);
			icount = 0;
		}
        if(m_yColumnList.isEmpty())
			m_yColumnList.push_back(yColumn);
		else
		{
            for(int j = 0; j < m_yColumnList.size(); j++)
			{
                if(QString::compare(yColumn, m_yColumnList.at(j)) == 0)
					icount++;
			}
            if(icount == 0)
				m_yColumnList.push_back(yColumn);
			icount = 0;
		}
		//delete temDataPair;
	}
    for(int i = 0; i < m_xColumnList.size(); i++)
	{
        rectXName.setRect(0.05 * width() + (i + 1) * horiGridWidth,
                          as + 0.02 * height(),
                          horiGridWidth,
                          verGridWidth);
		painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, m_xColumnList.at(i));
	}
    for(int i = 0; i < m_yColumnList.size(); i++)
	{
        rectXName.setRect(0.05 * width(),
                          as + 0.02 * height() + (1 + i) * verGridWidth,
                          horiGridWidth,
                          verGridWidth);
		painter.drawText(rectXName, Qt::AlignCenter | Qt::TextWordWrap, m_yColumnList.at(i));
	}
	m_xColumnList.clear();
	m_yColumnList.clear();
    if(m_secValue != -1)
        updateDataForDataPairsByTime(m_secValue);
	update();
}

void PlotText::drawNMCell(QPainter& painter,
                          QSet<QString>& xset,
                          QSet<QString>& yset,
                          const QVector<DataPair*>& dataVector,
                          int& horiGridWidth,
                          int& verGridWidth,
                          double& as)
{

    for(int i = 0; i < dataVector.size(); i++)
	{
		QString xIncludePlus = dataVector.at(i)->getDataPair().first;
		int pos = xIncludePlus.indexOf("+");
		QString xColumn = xIncludePlus.mid(0, pos);
		QString yColumn = xIncludePlus.mid(pos + 1);
		xset.insert(xColumn);
		yset.insert(yColumn);
		m_horiGridNum = xset.size() + 1;
		m_verGridNum = yset.size() + 1;
	}
    if(!dataVector.empty())
	{
		//先画GridFill
        for(int i = 0; i < m_verGridNum; i++)
		{
			QRect gridFillRect;
            gridFillRect.setRect(0.05 * width(),
                                 i * verGridWidth + 0.02 * height() + as,
                                 0.9 * width(),
                                 verGridWidth);
			painter.fillRect(gridFillRect, getGridFillColor());
			//painter.drawRect(gridFillRect);
		}
		//改变Axis画笔
		QPen pen;
		pen.setColor(getAxisColor());
		pen.setWidth(getAxisWidth());
		painter.setPen(pen);
		//再画Axis
		QRect gridRect;
        gridRect.setRect(
            0.05 * width(), as + 0.02 * height(), 0.9 * width(), m_verGridNum * verGridWidth);
		painter.drawRect(gridRect);
		//改变Grid画笔
		pen.setColor(getGridColor());
		pen.setWidth(getGridWidth());
		pen.setStyle(getGridStyle());
		painter.setPen(pen);
		//再画HorizonGrid
        for(int i = 1; i < m_verGridNum; i++)
		{
            painter.drawLine(0.05 * width() + 0.5 * getGridWidth() + 0.5 * getAxisWidth(),
                             as + 0.02 * height() + i * verGridWidth,
                             0.05 * width() + m_horiGridNum * horiGridWidth - 0.5 * getGridWidth() -
                                 0.5 * getAxisWidth(),
                             as + 0.02 * height() + i * verGridWidth);
		}
		//最后画VerticalGrid
        for(int i = 1; i < m_horiGridNum; i++)
		{
            painter.drawLine(0.05 * width() + i * horiGridWidth,
                             0.02 * height() + as + 0.5 * getGridWidth() + 0.5 * getAxisWidth(),
                             0.05 * width() + i * horiGridWidth,
                             0.02 * height() + as + verGridWidth * m_verGridNum -
                                 0.5 * getGridWidth() - 0.5 * getAxisWidth());
		}
	}
}

void PlotText::setGridStyle(GridStyle gridStyle)
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

void PlotText::setGridColorWidth(QColor color, uint width)
{
	m_gridColor = color;
	m_gridWidth = width;
}
