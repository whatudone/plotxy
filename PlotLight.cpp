#include <QDebug>
#include <QPainter>
#include <QPen>
#include <QString>
#include <QPair>
#include <QList>
#include <QPainterPath>
#include <QBrush>

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

	QString name = QString("Light%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

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
	QPen pen;
	QFont font;
	QRect rect;
	QSet<QString> yset;
	QPainterPath path;
	QList<QPair<QString, QString>> dataList;


	dataList = getPlotPairData();
	m_axisColor = Qt::white;
	pen.setColor(m_axisColor);
	rect.setRect(0, 0, width(), 0.1*height());
	painter.setPen(pen);
	font.setPointSize(20);
	painter.setFont(font);
	painter.drawText(rect, Qt::AlignCenter,"Events");
	

	//以下为绘制文字的内容和框框
	double verGridWidth = 0;
	double horGridWidth = 0;
	m_horiGridNum = 1;

	if (!dataList.empty())
	{
		rect.setRect(0.05*width(), 0.1*height(), 0.9*width(), 0.85*height());
		painter.drawRect(rect);
	}
	for (int i = 0; i < dataList.size(); i++)
	{
		yset.insert(dataList.at(i).first);
		m_verGridNum = dataList.size();
		verGridWidth = 0.85*height() / m_verGridNum;
		horGridWidth = (0.85*width()-0.1*height()) / m_horiGridNum;//整个宽减去框框宽，减去圆圈占宽
		rect.setRect(0.1*width()+0.1*height(),0.1*height()+i*verGridWidth,horGridWidth,verGridWidth);
		painter.drawText(rect,Qt::AlignCenter|Qt::TextWordWrap,dataList.at(i).first);
		path.addEllipse(0.1*width(), 0.1*height() + (i + 0.5)*verGridWidth - 0.05*height(), 0.1*height(), 0.1*height());
		painter.fillPath(path, Qt::red);
		painter.drawEllipse(0.1*width(), 0.1*height() + (i + 0.5)*verGridWidth - 0.05*height(), 0.1*height(), 0.1*height());
	}
	


	






																//绘制网格
	//m_gridColor = Qt::white;
	//pen.setColor(m_gridColor);
	//painter.setPen(pen);

	//QBrush brush;   //画刷。填充几何图形的调色板，由颜色和填充风格组成
	//m_gridFillColor = Qt::white;
	//brush.setColor(m_gridFillColor);
	//brush.setStyle(Qt::SolidPattern);





}