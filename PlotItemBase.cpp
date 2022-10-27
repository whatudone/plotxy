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
    
    QWidget* centerWindow = new QWidget;
    //this->setCentralWidget(centerWindow);

    m_position = QPoint(0,0);
    m_width = 0;
    m_height = 0;
    m_bVisible = true; 
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
}

void PlotItemBase::setWidth(int width)
{
    m_width = width;
}

void PlotItemBase::setHeight(int height)
{
    m_height = height;
}

QPoint PlotItemBase::currPosition()
{
    return m_position;
}

int PlotItemBase::currWidth()
{
    return m_width;
}

int PlotItemBase::currHeight()
{
    return m_height;
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

void PlotItemBase::addPlotPairData(QPair<QString, QString> pair)
{
	m_plotPairData.append(pair);
}

void PlotItemBase::delPlotPairData(QPair<QString, QString> pair)
{
	if (m_plotPairData.isEmpty())
		return;

	for (auto &i : m_plotPairData)
	{
		if (i == pair)
		{
			m_plotPairData.removeOne(i);
			break;
		}
	}
}

void PlotItemBase::updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair)
{
	if (m_plotPairData.isEmpty())
		return;

	for (int i = 0; i < m_plotPairData.size(); ++i)
	{
		if (m_plotPairData.at(i) == oldPair)
		{
			m_plotPairData.replace(i, newPair);
			break;
		}
	}
}

QList<QPair<QString, QString>> PlotItemBase::getPlotPairData()
{
	return m_plotPairData;
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