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
    // 基础绘制组件
    struct DrawComponents
    {
        QPointer<QCPGraph> graph;
        QPointer<QCPItemText> tracerText;
        QPointer<QCPItemPixmap> pixmap;

        DrawComponents()
        {
            graph = nullptr;
            tracerText = nullptr;
            pixmap = nullptr;
        }
    };

    // 背景颜色信息
    struct BackgroundLimitSeg
    {
        QString m_limitName;
        double m_limitValue;
        QColor m_lineColor;
        uint32_t m_lineWidth;
        QColor m_fillAboveColor;
        QColor m_fillBelowColor;
        bool operator==(const BackgroundLimitSeg& seg)
        {
            return seg.m_limitValue == this->m_limitValue;
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
                                      const QHash<QString, QVariant>& extraParams, bool isFromJson=false) override;
    void delPlotPairData(const QString& uuid) override;

    PlotType plotType() const override
    {
        return Type_PlotScatter;
    }

    void exportDataToFile(const QString& filename) const override;

    void drawGOGData() override;
    void addBackgroundColorInfo(const QString& limitName,
                                double limitValue,
                                const QColor& lineColor,
                                uint32_t lineWidth,
                                const QColor& fillAboveColor,
                                const QColor& fillBelowColor);

    void removeBackgroundColorInfo(double value);

    QMap<double, BackgroundLimitSeg> getBkgLimitSegMap() const;

    void addMarker(const PlotMarker& marker);
    void removeMarker(const QString&uuid);
    void modifyMarker(const QString&uuid ,const PlotMarker& marker);

    QHash<QString, PlotMarker> getPlotMarkers() const;
    PlotMarker getMarkerByUuid(const QString &uuid) const;

private:
    void initPlot();
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data) override;

    // 文本标签坐标,特殊情况不调用DataPair里面的通用处理方式
    QPair<double, double> getLabelTextAlign(const QString& text, DataPair* data);
    // 删除历史事件标签
    void clearEventText();
    void clearHistoryLines();
    // 刷新时间线事件
    void updateTimelineGraph() override;
    // 根据当前背景分段信息刷新背景颜色
    void updateBackgroundColorSeg();
    // 刷新marker
    void updateMarkers(double currentSeconds);

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
    // GOG
    QList<QCPGraph*> m_gogGraphList;
    QList<QCPItemEllipse*> m_gogEllipseList;

    // 背景分段信息
    QMap<double, BackgroundLimitSeg> m_bkgLimitSegMap;
    QList<QCPAbstractItem*> m_backSegRectList;
    // markers
    QHash<QString, PlotMarker> m_plotMarkers;
    QHash<QString,QCPItemText*> m_plotMarkerItems;
    // 是否初始化过上下限
    bool m_isInitCoorRange = false;
};

#endif // PLOTSCATTER_H
