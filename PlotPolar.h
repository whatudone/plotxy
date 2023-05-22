#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotPolar : public PlotItemBase
{
	Q_OBJECT
public:
	PlotPolar(QWidget* parent = Q_NULLPTR);
    ~PlotPolar() override;

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

	//unit
	void setUnitsShowX(bool on);
	void setUnitsShowY(bool on);
	void setUnitsX(const QString&);
	void setUnitsY(const QString&);

    PlotType plotType() const override
    {
        return Type_PlotPolar;
    }
    void setCoordRangeX(double lower, double upper) override;
    void setCoordRangeY(double lower, double upper) override;

    void getCoordRangeX(double& lower, double& upper) override;
    void getCoordRangeY(double& lower, double& upper) override;

public slots:

	//axis

	//style
	void slot_setRangeDrag(bool);
	void slot_setRangeZoom(bool);

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data) override;
    void delPlotPairData(const QString& uuid) override;

public:
    static int m_instanceCount; //实体个数
private:
    QCPPolarAxisAngular* m_angularAxis;

	double m_angularRange_lower;
	double m_angularRange_upper;
	double m_radialRange_lower;
	double m_radialRange_upper;
    // <uuid,polar>
    QHash<QString, QCPPolarGraph*> m_graphHash;
    // <uuid,data>
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
};
