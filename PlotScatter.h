#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include "constdef.h"
#include "qcustomplot.h"
#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    struct DrawComponents
    {
        QPointer<QCPGraph> graph;
        QPointer<QCPItemTracer> tracer;
        QPointer<QCPItemText> tracerText;
        QPointer<QCPItemPixmap> pixmap;

        DrawComponents()
        {
            graph = nullptr;
            tracer = nullptr;
            tracerText = nullptr;
            pixmap = nullptr;
        }
    };
    PlotScatter(QWidget* parent = nullptr);
    ~PlotScatter() override;

    static int m_instanceCount; //Plot实体个数

    void setAxisVisible(bool on, AxisType type);
    void setAxisTickLabelShow(bool on, AxisType type);

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QHash<QString, QVariant>& extraParams) override;
    void delPlotPairData(const QString& uuid) override;

    PlotType plotType() const override
    {
        return Type_PlotScatter;
    }

    void exportDataToFile(const QString& filename) const override;

private:
    void initPlot();
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data) override;

    // 文本标签坐标,特殊情况不调用DataPair里面的通用处理方式
    QPair<double, double> processLabelTextPosition(const QString& text, DataPair* data);
    // 删除历史事件标签
    void clearEventText();
    // 刷新时间线事件
    void updateTimelineGraph() override;

private:
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
    // <uuid,graph>
    QMap<QString, DrawComponents> m_mapScatter;
    // 事件标签
    QList<QCPItemText*> m_eventList;
    // 是否为时间线模式
    bool m_isTimeLine = false;
    QCPItemRect* m_timelineNowRect = nullptr;
    QCPItemText* m_timelineNowText = nullptr;
    QCPGraph* m_timelineGraph = nullptr;
};

#endif // PLOTSCATTER_H
