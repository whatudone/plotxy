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

    void setxTickLabelVisible(bool show) override;
    void setxTickLabelColor(const QColor& color) override;
    void setxTickLabelFont(const QFont& font) override;
    void setxTickLabelFontSize(int size) override;

    void setyTickLabelVisible(bool show) override;
    void setyTickLabelColor(const QColor& color) override;
    void setyTickLabelFont(const QFont& font) override;
    void setyTickLabelFontSize(int size) override;

	//unit
    void setUnitsShowX(bool on) override;
    void setUnitsShowY(bool on) override;
    void setUnitsX(const QString&) override;
    void setUnitsY(const QString&) override;

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

    void setGridFillColor(const QColor& color) override;

    void drawGOGData() override;
    struct DrawComponents
    {
        QPointer<QCPPolarGraph> graph;
        QPointer<QCPItemText> tracerText;
        QPointer<QCPItemPixmap> pixmap;

        DrawComponents()
        {
            graph = nullptr;
            tracerText = nullptr;
            pixmap = nullptr;
        }
    };

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data, double curSecs) override;
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
    QHash<QString, DrawComponents> m_graphHash;
    // <uuid,data>
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;

    QList<QCPPolarGraph*> m_gogGraphList;

private Q_SLOTS:
    void onXAxisRangeChanged(const QCPRange& range);
    void onYAxisRangeChanged(const QCPRange& range);
};
