#pragma once
#include "PlotItemBase.h"
#include <qtimer.h>
#include "qcustomplot.h"
#include <QHBoxLayout>
class PlotPolar :
	public PlotItemBase
{
	Q_OBJECT
public:
	PlotPolar(QWidget* parent = Q_NULLPTR);
	~PlotPolar();

	void initPlot();

public slots:
	void onSwitch(bool);
	void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
	void onTimeout();
	//void resizeEvent(QResizeEvent *event);
	//void onCustomContextMenuRequested(const QPoint& point);
public:
	static int m_instanceCount;			//实体个数
private:
	QCustomPlot *customPlot;
	QHBoxLayout *m_layout;
	QTimer* m_timer;
	bool m_started;
	QMap<QString, QMap<int, QColor>> m_thresholdColorMap;
};

