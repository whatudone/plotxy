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

public:
    static int m_instanceCount; //实体个数

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;
    void onGetCurrentSeconds(double secs) override;

protected:
    virtual void paintEvent(QPaintEvent* event);
    void getDataInfo(double secs);
    void updateData(int itemIndex, QString entityType, double secs); //实现核心绘制逻辑

private:
    QLine m_xAxis;
    QLine m_yAxis;

    double m_topPadding; //绘图间隔-top
    double m_bottomPadding; //绘图间隔-bottom
    double m_leftPadding; //绘图间隔-left
    double m_rightPadding; //绘图间隔-right
    int m_interPadding;

    QMap<QString, QMap<int, QColor>>
        m_thresholdColorMap; //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    QFont m_axisFont; //坐标标题字体

    int m_horiGridNum;
    int m_verGridNum;
    double m_seconds;
    int m_itemCnt;

    bool m_titleShow; //标题是否显示
};

#endif // _PLOT_TRACK_H_
