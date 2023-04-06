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

    void setAxisColorWidth(QColor color, uint width);
    void setGridColorWidth(QColor color, uint width);
    void setHorzGrids(uint count);
    void setVertGrids(uint count);
    void setGridFillColor(QColor color);

    void setGridVisible(bool enable);
    void setTickLabelColor(QColor& color);
    void setTickLabelFont(QFont& font);
    void setTickLabelFontSize(int size);
    void setGridStyle(GridStyle style);
    void setGridDensity(GridDensity density);

	//title
	void setTitle(QString&);
    void setTitleColor(QColor& color);
    void setTitleFillColor(QColor& color);
    void setTitleFont(QFont& font);
    void setTitleFontSize(int size);
	void setTitleVisible(bool show);
	//unit
	void setUnitsShowX(bool on);
	void setUnitsShowY(bool on);
	void setUnitsX(const QString&);
	void setUnitsY(const QString&);

    PlotType plotType() const override
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

private:
    void updateDataForDataPairsByTime(double secs) override;

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
};
