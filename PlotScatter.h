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
    ~PlotScatter() override;

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

    void setAxisVisible(bool on, AxisType type);
    void setAxisTickLabelShow(bool on, AxisType type);

    void rescale_xAxis(bool);
    void rescale_yAxis(bool);
    void rescaleAxis(bool);

    void delPlotPairData(const QString& uuid);

    PlotType plotType() const override
    {
        return Type_PlotScatter;
    }

    void exportDataToFile(const QString& filename) const override;

private:
    void initPlot();
    void updateDataForDataPairsByTime(double secs);
    void updateGraphByDataPair(DataPair* data);
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;

private:
    // <uuid,graph>
    QMap<QString, ScatterInfo> m_mapScatter;
};

#endif // PLOTSCATTER_H
