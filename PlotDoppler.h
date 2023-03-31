#pragma once
#include "PlotAScope.h"
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotDoppler : public PlotItemBase
{
	Q_OBJECT

public:
	PlotDoppler(QWidget* parent = Q_NULLPTR);
	~PlotDoppler();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();

    QString getxAxisLabel() const
    {
        return m_xAxisLabel;
    }
    QString getyAxisLabel() const
    {
        return m_yAxisLabel;
    }
    QColor getAxisLabelColor() const
    {
        return m_axisLabelColor;
    }
    QFont getAxisLabelFont() const
    {
        return m_axisLabelFont;
    }

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

    void setOuterFillColor(QColor color);

    void setCoordRangeX(double lower, double upper) override;
    void setCoordRangeY(double lower, double upper) override;

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
    void setTitleFont(QFont& font);
    void setTitleFontSize(int size);
    void setTitleVisible(bool show);

    PlotType plotType() const override
    {
        return Type_PlotDoppler;
    }

public:
    static int m_instanceCount; //实体个数

private:
    QCPColorMap* m_colorMap;
    QCPColorScale* m_colorScale;
    QCPMarginGroup* m_marginGroup;

	PlotAScope* m_horizon_AScope;
	PlotAScope* m_vertical_AScope;

    double m_topPadding; //绘图间隔-top
    double m_bottomPadding; //绘图间隔-bottom
    double m_leftPadding; //绘图间隔-left
    double m_rightPadding; //绘图间隔-right

public slots:
	//mouseEvent
	void slot_setMouseEventEnable(bool on);

protected:
    void paintEvent(QPaintEvent* event);
};
