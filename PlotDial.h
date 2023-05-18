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
    void updateCenterPoint();

private:
    bool m_bThinStyle;

    QColor m_dialColor;
    QColor m_pointColor;

    QPoint m_centerPoint;
    int m_circleRadius;
    int m_startAngle;
    int m_endAngle;
    // 四个点形成的钟表指针
    QPoint m_clockHandPoints[4];
};

#endif // PLOTDIAL_H
