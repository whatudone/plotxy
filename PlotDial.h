#ifndef PLOTDIAL_H
#define PLOTDIAL_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include <QMap>
#include <QString>
#include <QTimer>

class PlotDial : public PlotItemBase
{
    Q_OBJECT

public:
    PlotDial(QWidget* parent = Q_NULLPTR);
    ~PlotDial() override;

    static int m_instanceCount; //实体个数
    PlotType plotType() const override
    {
        return Type_PlotDial;
    }

private:
    void updateDataForDataPairsByTime(double secs) override;
    void customPainting(QPainter& painter) override;
    void updateGraphByDataPair(DataPair* data) override;
    void setCoordRangeX(double lower, double upper) override;
    void updateCenterPoint();

private:
    bool m_bThinStyle;

    QColor m_dialColor;
    QColor m_pointColor;
    QColor m_capColor;

    QPoint m_centerPoint;
    int m_circleRadius;
    int m_tickCount;

    int m_startAngle;
    int m_endAngle;

    QMap<QString, QVector<QPointF>> m_valueMap;
};

#endif // PLOTDIAL_H
