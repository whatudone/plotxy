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

    virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setGridFillColor(QColor color);

	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);

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

    virtual PlotType plotType() const
    {
        return Type_PlotPolar;
    }

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;

	//axis
    void setCoordRangeX(double lower, double upper) override;
    void setCoordRangeY(double lower, double upper) override;

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
