#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
#include "PlotAScope.h"
class PlotDoppler :
	public PlotItemBase
{
	Q_OBJECT

public:
	PlotDoppler(QWidget* parent = Q_NULLPTR);
	~PlotDoppler();

	enum AxisType {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();

	QString getxAxisLabel() const { return m_xAxisLabel; }
	QString getyAxisLabel() const { return m_yAxisLabel; }
	QColor getAxisLabelColor() const { return m_axisLabelColor; }
	QFont getAxisLabelFont() const { return m_axisLabelFont; }

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

	virtual void setOuterFillColor(QColor color);
	virtual QColor getOuterFillColor() { return m_outerFillColor; }
	void setCoordRangeX(double lower, double upper);
	void setCoordRangeY(double lower, double upper);
	void getCoordRangeX(double& lower, double& upper);
	void getCoordRangeY(double& lower, double& upper);
	
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual uint getHorzGrids() { return m_horzGrids; }
	virtual uint getVertGrids() { return m_vertGrids; }
	virtual uint getAxisWidth() { return m_axisWidth; }
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getAxisColor() { return m_axisColor; }
	virtual QColor getGridColor() { return m_gridColor; }
	virtual void setGridFillColor(QColor color);
	virtual QColor getGridFillColor() { return m_gridFillColor; }
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


public:
	static int m_instanceCount;			//实体个数

private:
	QCustomPlot *m_customPlot;
	QCPColorMap *m_colorMap;
	QCPColorScale *m_colorScale;
	QCPMarginGroup *m_marginGroup;

	PlotAScope* m_horizon_AScope;
	PlotAScope* m_vertical_AScope;

	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right


public slots:
	//mouseEvent
	void slot_setMouseEventEnable(bool on);

protected:
	virtual void paintEvent(QPaintEvent* event);
};

