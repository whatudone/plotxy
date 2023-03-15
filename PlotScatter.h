#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "PlotItemBase.h"
#include "BaseData.h"
#include "qcustomplot.h"
#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    PlotScatter(QWidget *parent = nullptr);
    ~PlotScatter();

	enum AxisType {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

    static int m_instanceCount;         //Plot实体个数

	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisLabelColor() const { return m_axisLabelColor; }
	QFont getAxisLabelFont() const { return m_axisLabelFont; }

	void setPaddings(double top, double bottom, double left, double right);
	
	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

	void rescale_xAxis(bool);
	void rescale_yAxis(bool);
	void rescaleAxis(bool);

	virtual void addPlotPairData(QPair<QString, QString>);
	virtual void delPlotPairData(QPair<QString, QString>);
	virtual void updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair);

	virtual void setOuterFillColor(QColor color);
	virtual QColor getOuterFillColor() { return m_outerFillColor; }
	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual void setHorzGrids(uint);
	virtual void setVertGrids(uint);
	virtual uint getHorzGrids();
	virtual uint getVertGrids();
	virtual void setAxisColorWidth(QColor, uint);
	virtual void setGridColorWidth(QColor, uint);
	virtual uint getAxisWidth();
	virtual uint getGridWidth();
	virtual QColor getAxisColor();
	virtual QColor getGridColor();
	virtual void setGridFillColor(QColor color);
	virtual QColor getGridFillColor(){ return m_gridFillColor; }
	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);
	virtual bool getGridVisible() { return m_gridVisible; }
	virtual QColor getTickLabelColor() { return m_tickLabelColor; }
	virtual QFont getTickLabelFont() { return m_tickLabelFont; }
	virtual int getTickLabelFontSize() { return m_tickLabelFontSize; }
	virtual Qt::PenStyle getGridStyle() { return m_gridStyle; }
	virtual GridDensity getGridDensity() { return m_gridDensity; }

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitle(QString& str);
	virtual void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont& font);
	virtual void setTitleFontSize(int size);
	virtual void setTitleVisible(bool show);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }
	virtual int getTitleFontSize() { return m_titleFontSize; }

public slots:
    void onGetCurrentSeconds(double secs) override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
	void initPlot();
    void getDataInfo(double secs);
	void updateData(double secs, int index, DataPair* data);

private:
    double m_curSeconds;
	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	QCustomPlot *m_customPlot;
	struct ScatterInfo 
	{
		QPointer<QCPGraph> graph;
		QPointer<QCPItemTracer> tracer;
		QPointer<QCPItemText> tracerText;

		ScatterInfo() { graph = nullptr; tracer = nullptr; tracerText = nullptr; }
	};

	QMap<QPair<QString, QString>, ScatterInfo> m_mapScatter;


//    QList<QColor> m_clrList;
};

#endif // PLOTSCATTER_H
