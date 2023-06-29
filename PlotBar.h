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
    void updateKeyAxisTickLabel();

    // 交换keyAxis 和valueAxis上的ticker,仅在切换水平和垂直时才使用
    void exchangeKeyAndValueAxisTickLabel();

    bool isValidColorRange(DataPair* data);

    // 针对target-属性轴
    QCPAxis* keyAxis();
    // 针对数字value轴
    QCPAxis* valueAxis();

protected:
    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QHash<QString, QVariant>& extraParams,
                                      bool isFromJson = false) override;
    virtual void delPlotPairData(const QString& uuid) override;
    virtual void updateGraphByDataPair(DataPair* dataPair) override;

    void setIsHorizonBar(bool isHorizonBar) override;

    void setyTickLabelVisible(bool show) override;
    void setyTickLabelColor(const QColor& color) override;
    void setyTickLabelFont(const QFont& font) override;
    void setyTickLabelFontSize(int size) override;

    void setyAxisLabelVisible(bool on) override;
    void setyAxisLabel(const QString& label) override;
    void setyAxisLabelColor(const QColor& color) override;
    void setyAxisLabelFont(const QFont& font) override;
    void setyAxisLabelFontSize(int size) override;

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
};

#endif // _PLOT_BAR_H_
