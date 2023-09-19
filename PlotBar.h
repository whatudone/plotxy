/* * @filename: PlotBar.h
* @brief: PlotBar控件类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef PLOT_BAR_H
#define PLOT_BAR_H

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

    int getBarBetweenPadding() const;
    void setBarBetweenPadding(int barBetweenPadding);

    int getBarRightPadding() const;
    void setBarRightPadding(int barRightPadding);

    int getBarLeftPadding() const;
    void setBarLeftPadding(int barLeftPadding);

public:
    static int m_instanceCount; //实体个数

private:
    void initPlot();
    void updateKeyAxisTickLabel();

    // 交换keyAxis 和valueAxis上的ticker,仅在切换水平和垂直时才使用
    void exchangeKeyAndValueAxisTickLabel();

    bool isValidColorRange(DataPair* data);

    // 针对target-属性轴
    QCPAxis* keyAxis();
    // 针对数字value轴
    QCPAxis* valueAxis();
    // 根据实体数量和坐标范围更新三个间距值
    void updateBarPads();
    // 根据间距调整坐标范围
    void updateCoordRangeByMidPad();
    void updateCoordRangeByLeftPad(int32_t newLeftPad, int32_t oldleftPad);
    void updateCoordRangeByRightPad(int32_t newRightPad, int32_t oldRightPad);

protected:
    DataPair* addPlotDataPair(int32_t xEntityID,
                              const QString& xAttrName,
                              const QString& xAttrUnitName,
                              int32_t yEntityID,
                              const QString& yAttrName,
                              const QString& yAttrUnitName,
                              const QHash<QString, QVariant>& extraParams,
                              bool isFromJson = false) override;

    void delPlotPairData(const QString& uuid) override;

    void updateDataForDataPairsByTime(double secs) override;

    void updateGraphByDataPair(DataPair* dataPair, double) override;

    void setIsHorizonBar(bool isHorizonBar) override;

    void resizeEvent(QResizeEvent* event) override;

private:
    QMap<QString, QList<std::tuple<QString, double, QColor>>>
        m_allColorInfoList; //QString:colorname，double:lower limit,QColor:color

    QMap<QString, QMap<double, QColor>>
        m_barColorInfoMap; // 最终界面上的所有QCPBar  QString:uuid  内层QMap对应当前目标上的所有QCPBar：double:阈值 QColor:显示的颜色

    QMap<QString, QList<QCPBars*>> m_allBar;
    QMap<QString, QString>
        m_tickLabelMap; // 用来存放每个item对应的描述，显示在y轴左边 QString:uuid  QString:target_attr
    QMap<QString, double> m_itemData; //用来存放每个target对应的最小值
    QMap<QString, double> m_curValue; //用来存放当前secs对应的数据
    // 每个uuid数据对对应一个label显示当前Bar的数值
    QHash<QString, QCPItemText*> m_barValueLabelHash;

    double m_barBetweenPadding; // Bar之间的间隔,像素值大小
    double m_barLeftPadding; // Bar和左边的间隔
    double m_barRightPadding; // Bar和右边的间隔
};

#endif // _PLOT_BAR_H
