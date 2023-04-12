#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include "qcustomplot.h"

#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    PlotScatter(QWidget* parent = nullptr);
    ~PlotScatter();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

    struct ScatterInfo
    {
        QPointer<QCPGraph> graph;
        QPointer<QCPItemTracer> tracer;
        QPointer<QCPItemText> tracerText;

        ScatterInfo()
        {
            graph = nullptr;
            tracer = nullptr;
            tracerText = nullptr;
        }
    };

    static int m_instanceCount; //Plot实体个数

	void setPaddings(double top, double bottom, double left, double right);

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

	void rescale_xAxis(bool);
	void rescale_yAxis(bool);
	void rescaleAxis(bool);

    void addPlotPairData(const QPair<QString, QString>& pair) override;
    void delPlotPairData(const QPair<QString, QString>&);
    void updatePlotPairData(const QPair<QString, QString>& oldPair,
                            const QPair<QString, QString>& newPair);

    void setHorzGrids(uint);
    void setVertGrids(uint);

    void setAxisColorWidth(QColor, uint);
    void setGridColorWidth(QColor, uint);

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
        return Type_PlotScatter;
    }

    void exportDataToFile(const QString& filename) const override;

private:
	void initPlot();
    void updateDataForDataPairsByTime(double secs);
    void updateGraph(double secs, int index, DataPair* data);

private:
    QMap<QPair<QString, QString>, ScatterInfo> m_mapScatter;
};

#endif // PLOTSCATTER_H
