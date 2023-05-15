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
        QPointer<QCPItemPixmap> pixmap;

        ScatterInfo()
        {
            graph = nullptr;
            tracer = nullptr;
            tracerText = nullptr;
            pixmap = nullptr;
        }
    };

    static int m_instanceCount; //Plot实体个数

    void setAxisVisible(bool on, AxisType type);
    void setAxisTickLabelShow(bool on, AxisType type);

    void rescale_xAxis(bool);
    void rescale_yAxis(bool);
    void rescaleAxis(bool);

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QVariantList& extraParams) override;
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

private:
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
    // <uuid,graph>
    QMap<QString, ScatterInfo> m_mapScatter;
};

#endif // PLOTSCATTER_H
