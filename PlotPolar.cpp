#include "PlotPolar.h"
#include <qlabel.h>
PlotPolar::PlotPolar(QWidget * parent)
	:PlotItemBase(parent)
{
	m_started = false;
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &PlotPolar::onTimeout);
	
}

PlotPolar::~PlotPolar()
{
}

void PlotPolar::initPlot()
{
	QLabel* lb = new QLabel(this);
	lb->setGeometry(0, 0, width(),height());
	lb->setObjectName("lb1");
	lb->setStyleSheet("QLabel#lb1{border-width:3px;border-style:solid;border-color:rgb(0,0,0);}");
	for (auto i:this->children())
	{
		QString classname = i->metaObject()->className();
		if (classname == "QPushButton" && i->isWidgetType())
		{
			((QWidget*)i)->setParent(lb);
			break;
		}
	}
#if 1
	customPlot = new QCustomPlot(this);
	customPlot->setGeometry(width()/2 - (height() - 20)/2, 10, height() - 20, height() - 20);
	customPlot->plotLayout()->clear();
	QCPPolarAxisAngular *angularAxis = new QCPPolarAxisAngular(customPlot);
	customPlot->plotLayout()->addElement(0, 0, angularAxis);
	customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	angularAxis->setRangeDrag(false);
	angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);

	angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
	angularAxis->radialAxis()->setTickLabelRotation(0);
	angularAxis->radialAxis()->setAngle(45);

	angularAxis->grid()->setAngularPen(QPen(QColor(200, 200, 200), 0, Qt::SolidLine));
	angularAxis->grid()->setSubGridType(QCPPolarGrid::gtAll);
	/*QCPPolarGraph *g1 = new QCPPolarGraph(angularAxis, angularAxis->radialAxis());
	QCPPolarGraph *g2 = new QCPPolarGraph(angularAxis, angularAxis->radialAxis());
	g2->setPen(QPen(QColor(255, 150, 20)));
	g2->setBrush(QColor(255, 150, 20, 50));
	g1->setScatterStyle(QCPScatterStyle::ssDisc);
	for (int i = 0; i<100; ++i)
	{
		g1->addData(i / 100.0*360.0, qSin(i / 100.0*M_PI * 8) * 8 + 1);
		g2->addData(i / 100.0*360.0, qSin(i / 100.0*M_PI * 6) * 2);
	}*/
	angularAxis->setRange(0, 360);
	angularAxis->radialAxis()->setRange(-10, 10);
#endif
}

void PlotPolar::onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>> targetMap)
{
	m_thresholdColorMap = targetMap;
}

void PlotPolar::onTimeout()
{
}

void PlotPolar::onSwitch(bool b)
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