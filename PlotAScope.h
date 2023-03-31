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

	void setPaddings(double top, double bottom, double left, double right);
    void setxAxisLabel(const QString& str);
    void setyAxisLabel(const QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);
    void setMinimumMargins(const QMargins& margins);

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);

	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual void setGridFillColor(QColor color);

	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
    void setTitle(const QString& str);
    void setTitleColor(const QColor& color);
    void setTitleFont(const QFont& font);
	void setTitleVisible(bool show);
	virtual void setTitleFontSize(int size);
	virtual void setTitleFillColor(QColor& color);

    virtual PlotType plotType() const
    {
        return Type_PlotAScope;
    }

public:
    static int m_instanceCount; //实体个数

private:
    QCustomPlot* m_customPlot;

    double m_topPadding; //绘图间隔-top
    double m_bottomPadding; //绘图间隔-bottom
    double m_leftPadding; //绘图间隔-left
    double m_rightPadding; //绘图间隔-right

    QColor m_axisLabelColor; //坐标标题颜色
    QFont m_axisLabelFont; //坐标标题字体
	QString m_xAxisLabel;
	QString m_yAxisLabel;

public slots:
	//mouseEvent
	void slot_setMouseEventEnable(bool on);

	void onGetCurrentSeconds(double secs);

protected:
	virtual void paintEvent(QPaintEvent* event);
};
