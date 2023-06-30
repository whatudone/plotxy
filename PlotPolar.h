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

    void setAxisColorWidth(const QColor& color, int32_t width) override;
    void setGridColorWidth(const QColor& color, int32_t width) override;
    void setHorzGrids(int32_t count) override;
    void setVertGrids(int32_t count) override;
    void setGridVisible(bool enable) override;
    void setGridStyle(GridStyle style) override;

    void setxTickLabelVisible(bool show);
    void setxTickLabelColor(const QColor& color);
    void setxTickLabelFont(const QFont& font);
    void setxTickLabelFontSize(int size);

    void setyTickLabelVisible(bool show);
    void setyTickLabelColor(const QColor& color);
    void setyTickLabelFont(const QFont& font);
    void setyTickLabelFontSize(int size);

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

    void rescaleXAxis() override;
    void rescaleYAxis() override;
    void rescaleAxis() override;

    void drawGOGData() override;

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

    QList<QCPPolarGraph*> m_gogGraphList;
};
