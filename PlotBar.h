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
    ~PlotBar() override;

    PlotType plotType() const override
    {
        return Type_PlotBar;
    }

public:
    static int m_instanceCount; //实体个数

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>) override;

protected:
    void updateDataForDataPairsByTime(double secs) override;

private:
    void initPlot();
    void updateGraph();

private:
    QMap<QString, QMap<int, QColor>>
        m_thresholdColorMap; //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    // 从数据管理器采集的临时数据，用于绘图，每次更新数据只需要更新这个临时数据即可《value,key》
    QList<QPair<double, QString>> m_dataList;

    QVector<double> m_barTicks;
    QVector<QString> m_barLabels;
    QVector<double> m_barData;
    QCPBars* m_pBar = nullptr;
};

#endif // _PLOT_BAR_H_
