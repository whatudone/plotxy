#pragma once
#include "PlotItemBase.h"
#include <qtimer.h>
#include "qcustomplot.h"
class PlotPolar :
	public PlotItemBase
{
	Q_OBJECT
public:
	PlotPolar(QWidget* parent = Q_NULLPTR);
	~PlotPolar();

	void initPlot();
	QString getTitle() { return m_title; }
	QColor getTitleColor() { return m_titleColor; }
	QFont getTitleFont() { return m_titleFont; }
	QString getAngularUnit() { return m_angularUnit; }
	QString getRadialUnit() { return m_radialUnit; }

public slots:
	void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
	//title
	void slot_setTitle(QString);
	void slot_setTitleColor(const QColor &color);
	void slot_setTitleFont(const QFont &font);

	//unit
	void slot_setAngularUnit(QString);
	void slot_setRadialUnit(QString);

	//axis
	void slot_setAngularRange(double lower, double upper);
	void slot_setRadialRange(double lower, double upper);

	//style
	void slot_setRangeDrag(bool);
	void slot_setRangeZoom(bool);

	//接收当前时间
	void slot_getCurrentSeconds(double);

public:
	static int m_instanceCount;			//实体个数
private:
	QCustomPlot *m_customPlot;
	QCPPolarAxisAngular *m_angularAxis;
	QLabel* m_titleLabel;
	QMap<QString, QMap<int, QColor>> m_thresholdColorMap;

	QString m_title;
	QColor	m_titleColor;				//标题颜色
	QFont	m_titleFont;				//标题字体

	QString m_angularUnit;
	QString m_radialUnit;

	double m_angularRange_lower;
	double m_angularRange_upper;
	double m_radialRange_lower;
	double m_radialRange_upper;

	QList<double> m_valueList;

protected:
	virtual void paintEvent(QPaintEvent* event);
	bool eventFilter(QObject *watched, QEvent *event);
};

