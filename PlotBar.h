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

protected:
    void updateDataForDataPairsByTime(double secs) override;

private:
    void initPlot();
    void updateGraph(double secs, DataPair* data);
    void updateLabelAndTick();
    void setBarData(const QString& uuid, double value, int index);

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QVariantList& extraParams) override;
    virtual void delPlotPairData(const QString& uuid) override;

private:
    QColor m_defaultColor;

    QList<std::tuple<QString, double, QColor>>
        m_colorInfoList; //QString:colorname，double:lower limit,QColor:color

    QVector<double> m_barTicks;
    QMap<QString, QMap<double, QColor>>
        m_barColorInfoMap; // 最终界面上的所有QCPBar  QString:uuid  内层QMap对应当前目标上的所有QCPBar：double:阈值 QColor:显示的颜色

    QMap<QString, QList<QCPBars*>> m_allBar;
    QMap<QString, QString>
        m_itemInfo; // 用来存放每个item对应的描述，显示在y轴左边 QString:uuid  QString:target_attr
    QMap<QString, double> m_itemData; //用来存放每个target对应的最小值

    // 范围的最小值和最大值，用于缩放图表坐标范围，保证图表始终能完全显示
    double m_min;
    double m_max;
};

#endif // _PLOT_BAR_H_
