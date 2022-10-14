#include "PlotPolar.h"
#include <qlabel.h>
#include "QResizeEvent"

int PlotPolar::m_instanceCount = 1;
PlotPolar::PlotPolar(QWidget * parent)
	:PlotItemBase(parent)
{
	QString name = QString("Polar%1").arg(m_instanceCount);
	this->setName(name);
	m_instanceCount += 1;

	m_started = false;
	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &PlotPolar::onTimeout);
	initPlot();
//	m_layout = new QHBoxLayout(this);
//	m_layout->addWidget(customPlot);


}
//void PlotPolar::resizeEvent(QResizeEvent *event)
//{
//	customPlot->resize(event->size());
//}
PlotPolar::~PlotPolar()
{
}

void PlotPolar::initPlot()
{
	//QLabel* lb = new QLabel(this);
	//lb->setGeometry(0, 0, width(),height());
	//lb->setObjectName("lb1");
	//lb->setStyleSheet("QLabel#lb1{border-width:3px;border-style:solid;border-color:rgb(125,125,125);}");
	//for (auto i:this->children())
	//{
	//	QString classname = i->metaObject()->className();
	//	if (classname == "QPushButton" && i->isWidgetType())
	//	{
	//		((QWidget*)i)->setParent(lb);
	//		break;
	//	}
	//}
#if 1


	customPlot = new QCustomPlot(this);
	customPlot->setBackground(QBrush(QColor(0, 0, 0)));
	//customPlot->setGeometry(width()/2 - (height() - 20)/2, 10, height() - 20, height() - 20);
	customPlot->setGeometry(0,0,width(),height());
	customPlot->plotLayout()->clear();
	QCPPolarAxisAngular *angularAxis = new QCPPolarAxisAngular(customPlot);
	angularAxis->setBasePen(QPen(QColor(255, 255, 255), 2));
	customPlot->plotLayout()->addElement(0, 0, angularAxis);
	//customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	angularAxis->setRangeDrag(false);
	angularAxis->setTickLabelMode(QCPPolarAxisAngular::lmUpright);
	angularAxis->setFormat(QString::fromLocal8Bit("бу"));
	angularAxis->setTickLabelColor(QColor(255, 255, 255));
	angularAxis->setTickPen(QPen(QColor(255, 255, 255),2));

	//angularAxis->radialAxis()->setTickPen(QPen(QColor(255, 255, 255),2));
	angularAxis->radialAxis()->setTickLabelColor(QColor(255, 255, 255));
	angularAxis->radialAxis()->setFormat(QString::fromLocal8Bit("бу"));
	angularAxis->radialAxis()->setTickLabelMode(QCPPolarAxisRadial::lmUpright);
	angularAxis->radialAxis()->setTickLabelRotation(0);
	angularAxis->radialAxis()->setBasePen(QPen(QColor(255, 255, 255), 2));
	angularAxis->radialAxis()->setAngle(0);

	//angularAxis->radialAxis()->setNumberFormat("e");
	angularAxis->grid()->setAngularPen(QPen(QColor(255, 255, 255), 0, Qt::SolidLine));
	angularAxis->grid()->setSubGridType(QCPPolarGrid::gtNone);
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
	angularAxis->radialAxis()->setRange(0, 90);
#endif

	//customPlot->replot();
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