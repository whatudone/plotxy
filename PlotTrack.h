#ifndef _PLOT_TRACK_H_
#define _PLOT_TRACK_H_

#include "BaseData.h"
#include "PlotItemBase.h"
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
    ~PlotTrack();

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);

    void drawRect(int itemIndex, QList<QColor> dataList);
    PlotType plotType() const override
    {
        return Type_PlotTrack;
    }

public:
    static int m_instanceCount; //实体个数

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;

protected:
    void updateDataForDataPairsByTime(double secs) override;
    void updateData(int itemIndex, QString entityType, double secs); //实现核心绘制逻辑
private:
    void customPainting(QPainter& painter) override;

private:
    QLine m_xAxis;
    QLine m_yAxis;

    int m_interPadding;

    QMap<QString, QMap<int, QColor>>
        m_thresholdColorMap; //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    QFont m_axisFont; //坐标标题字体

    int m_horiGridNum;
    int m_verGridNum;

    int m_itemCnt;
    QMap<int, QList<QColor>> m_trackDrawDataMap;
};

#endif // _PLOT_TRACK_H_
