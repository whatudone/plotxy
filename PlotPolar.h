#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotPolar : public PlotItemBase
{
	Q_OBJECT
public:
	PlotPolar(QWidget* parent = Q_NULLPTR);
	~PlotPolar();

	void initPlot();

	virtual void setOuterFillColor(QColor color);
    virtual QColor getOuterFillColor()
    {
        return m_outerFillColor;
    }
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setGridFillColor(QColor color);

    virtual uint getHorzGrids()
    {
        return m_horzGrids;
    }
    virtual uint getVertGrids()
    {
        return m_vertGrids;
    }
    virtual uint getAxisWidth()
    {
        return m_axisWidth;
    }
    virtual uint getGridWidth()
    {
        return m_gridWidth;
    }
    virtual QColor getAxisColor()
    {
        return m_axisColor;
    }
    virtual QColor getGridColor()
    {
        return m_gridColor;
    }
    virtual QColor getGridFillColor()
    {
        return m_gridFillColor;
    }

	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);
    virtual bool getGridVisible()
    {
        return m_gridVisible;
    }
    virtual QColor getTickLabelColor()
    {
        return m_tickLabelColor;
    }
    virtual QFont getTickLabelFont()
    {
        return m_tickLabelFont;
    }
    virtual int getTickLabelFontSize()
    {
        return m_tickLabelFontSize;
    }
    virtual Qt::PenStyle getGridStyle()
    {
        return m_gridStyle;
    }
    virtual GridDensity getGridDensity()
    {
        return m_gridDensity;
    }

	//title
	void setTitle(QString&);
    void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
    void setTitleFont(QFont& font);
	virtual void setTitleFontSize(int size);
	void setTitleVisible(bool show);
	//unit
	void setUnitsShowX(bool on);
	void setUnitsShowY(bool on);
	void setUnitsX(const QString&);
	void setUnitsY(const QString&);
    virtual bool unitsShowX()
    {
        return m_showUnits_x;
    }
    virtual bool unitsShowY()
    {
        return m_showUnits_y;
    }
    virtual QString getUnitsX()
    {
        return m_units_x;
    }
    virtual QString getUnitsY()
    {
        return m_units_y;
    }
    virtual bool getTitleVisible()
    {
        return m_titleVisible;
    }
    virtual QString getTitle()
    {
        return m_title;
    }
    virtual QColor getTitleColor()
    {
        return m_titleColor;
    }
    virtual QColor getTitleFillColor()
    {
        return m_titleFillColor;
    }
    virtual QFont getTitleFont()
    {
        return m_titleFont;
    }
    virtual int getTitleFontSize()
    {
        return m_titleFontSize;
    }

    virtual PlotType plotType() const
    {
        return Type_PlotPolar;
    }

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;

	//axis
	void setCoordRangeX(double lower, double upper);
	void setCoordRangeY(double lower, double upper);

	//style
	void slot_setRangeDrag(bool);
	void slot_setRangeZoom(bool);

	//接收当前时间
    void onGetCurrentSeconds(double) override;

	//mouseEvent
	void slot_setMouseEventEnable(bool);

public:
    static int m_instanceCount; //实体个数
private:
    QCustomPlot* m_customPlot;
    QCPPolarAxisAngular* m_angularAxis;
	QMap<QString, QMap<int, QColor>> m_thresholdColorMap;

	double m_angularRange_lower;
	double m_angularRange_upper;
	double m_radialRange_lower;
	double m_radialRange_upper;

	QList<double> m_valueList;

protected:
	virtual void paintEvent(QPaintEvent* event);
};
