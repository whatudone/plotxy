﻿#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotRTI : public PlotItemBase
{
	Q_OBJECT

public:
	PlotRTI(QWidget* parent = Q_NULLPTR);
	~PlotRTI();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

    void setHorzGrids(uint count);
    void setVertGrids(uint count);
    void setAxisColorWidth(QColor color, uint width);
    void setGridColorWidth(QColor color, uint width);

    void setGridFillColor(QColor color);

    void setGridVisible(bool enable);
    void setTickLabelColor(QColor& color);
    void setTickLabelFont(QFont& font);
    void setTickLabelFontSize(int size);
    void setGridStyle(GridStyle style);
    void setGridDensity(GridDensity density);

    void setUnitsShowX(bool on);
    void setUnitsShowY(bool on);
    void setUnitsX(const QString& units);
    void setUnitsY(const QString& units);
    void setTitle(QString& str);
    void setTitleColor(QColor& color);
    void setTitleFillColor(QColor& color);
    void setTitleFontSize(int size);
    void setTitleFont(QFont& font);
    void setTitleVisible(bool show);

    PlotType plotType() const override
    {
        return Type_PlotRTI;
    }

public:
    static int m_instanceCount; //实体个数

private:
    QCPColorMap* m_colorMap;
    QCPColorScale* m_colorScale;
    QCPMarginGroup* m_marginGroup;
};
