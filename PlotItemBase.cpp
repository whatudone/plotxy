/* * @filename: PlotItemBase.cpp
* @brief: PlotBase Plot»ùÀà
* @author: zhameng
* @create time: 2022-09-21
*  */

#include "PlotItemBase.h"


PlotItemBase::PlotItemBase(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    
    QWidget* centerWindow = new QWidget;
    this->setCentralWidget(centerWindow);

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

//void PlotItemBase::setVisible(bool bVisible)
//{
//    m_bVisible = bVisible;
//}
// 
//bool PlotItemBase::bVisible()
//{
//    return m_bVisible;
//}