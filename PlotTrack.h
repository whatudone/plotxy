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

    PlotType plotType() const override
    {
        return Type_PlotTrack;
    }

public:
    static int m_instanceCount; //实体个数

protected:
    void initPlot();
    void updateDataForDataPairsByTime(double secs) override;
    void updateKeyAxisTickLabel();

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QHash<QString, QVariant>& extraParams,
                                      bool isFromJson = false) override;
    virtual void delPlotPairData(const QString& uuid) override;
    virtual void updateGraphByDataPair(DataPair* dataPair, double curSecs) override;

private:
    QMap<QString, QMap<double, QColor>>
        m_trackDrawDataMap; //key:entityType+entityAttr, threshold,QColor
    QMap<QString, double> m_curValue;
    QMap<TrackStatus, QColor> m_defaultColorMap;

    QVector<double> m_barTicks;

    QMap<QString, QList<QCPBars*>> m_allBar;
    QMap<QString, QString> m_tickLabelMap;
    QMap<QString, QList<double>> m_itemData;
    double m_minTime;
    double m_maxTime;
};

#endif // _PLOT_TRACK_H_
