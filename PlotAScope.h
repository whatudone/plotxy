#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotAScope : public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
	~PlotAScope();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();

	void setPaddings(double top, double bottom, double left, double right);
    void setxAxisLabel(const QString& str);
    void setyAxisLabel(const QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);
    void setMinimumMargins(const QMargins& margins);

    void setHorzGrids(uint count) override;
    void setVertGrids(uint count) override;
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
    void setTitle(const QString& str);
    void setTitleColor(const QColor& color);
    void setTitleFont(const QFont& font);
	void setTitleVisible(bool show);
    void setTitleFontSize(int size);
    void setTitleFillColor(QColor& color);

    PlotType plotType() const override
    {
        return Type_PlotAScope;
    }

public:
    static int m_instanceCount; //实体个数
};
