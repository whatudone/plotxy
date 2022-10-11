#pragma once
#include "PlotItemBase.h"
#include <qtimer.h>
#include "qcustomplot.h"
class PlotPolar :
	public PlotItemBase
{
public:
	PlotPolar(QWidget* parent = Q_NULLPTR);
	~PlotPolar();

	void initPlot();

public slots:
	void onSwitch(bool);
	void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
	void onTimeout();
private:
	QCustomPlot *customPlot;
	QTimer* m_timer;
	bool m_started;
	QMap<QString, QMap<int, QColor>> m_thresholdColorMap;
};

