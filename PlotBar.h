/* * @filename: PlotBar.h
* @brief: PlotBar控件类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef _PLOT_BAR_H_
#define _PLOT_BAR_H_

#include "BaseData.h"
#include "PlotItemBase.h"
#include <QMap>
#include <QString>
#include <QTimer>

class PlotBar : public PlotItemBase
{
    Q_OBJECT
public:
    PlotBar(QWidget* parent = Q_NULLPTR);
    ~PlotBar();

    void setPlotItemExtentionDirec(bool);

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);
    void drawRect(int itemIndex,
                  bool bHorizontal,
                  int itemLength,
                  int leftBoundary,
                  int rightBoundary,
                  const QColor& color,
                  QPainter& painter);

    PlotType plotType() const override
    {
        return Type_PlotBar;
    }

public:
    static int m_instanceCount; //实体个数

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;

protected:
    void updateDataForDataPairsByTime(double secs);

private:
    void customPainting(QPainter& painter) override;
    // 绘制所有数据块
    void drawPairDatas(QPainter& painter);

    void drawPairData(
        int itemIndex, QString currKey, int32_t perItemLength, double currValue, QPainter& painter);
    // 计算每个Bar的绘制长度
    int calculateItemLength();

private:
    bool m_bHorizontal;
    bool m_titleShow; //标题是否显示

    QLine m_xAxis;
    QLine m_yAxis;

    int m_interPadding;

    int m_currTimeIndex;

    QMap<QString, QMap<int, QColor>>
        m_thresholdColorMap; //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    int m_horiGridNum;
    int m_verGridNum;
    // 从数据管理器采集的临时数据，用于绘图，每次更新数据只需要更新这个临时数据即可《value,key》
    QList<QPair<double, QString>> m_dataList;
};

#endif // _PLOT_BAR_H_
