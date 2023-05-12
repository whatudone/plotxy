#ifndef _PLOT_TRACK_H_
#define _PLOT_TRACK_H_

#include "BaseData.h"
#include "PlotItemBase.h"
#include "qcustomplot.h"

#include <QMap>
#include <QString>

class PlotTrack : public PlotItemBase
{
    Q_OBJECT
public:
    enum TrackStatus
    {
        Available, // 有效且可用
        Invalid, // 无效可用
        Unavailable // 无效不可用
    };

    PlotTrack(QWidget* parent = Q_NULLPTR);
    ~PlotTrack() override;

    void setLeftPadding(int);
    void setRightPadding(int);

    PlotType plotType() const override
    {
        return Type_PlotTrack;
    }

public:
    static int m_instanceCount; //实体个数

protected:
    void initPlot();
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraph(DataPair* dataPair, double secs); //实现核心绘制逻辑
    void updateLabelAndTick();

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QVariantList& extraParams) override;
    virtual void delPlotPairData(const QString& uuid) override;

private:
    QMap<QString, QMap<double, QColor>>
        m_trackDrawDataMap; //key:entityType+entityAttr, threshold,QColor
    QMap<TrackStatus, QColor> m_defaultColorMap;

    QVector<double> m_barTicks;

    QMap<QString, QList<QCPBars*>> m_allBar;
    QMap<QString, QString> m_itemInfo;
    QMap<QString, QList<double>> m_itemData;
    double m_minTime;
    double m_maxTime;
};

#endif // _PLOT_TRACK_H_
