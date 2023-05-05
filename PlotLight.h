/* * @filename: PlotLight.h
* @brief: PlotLight控件类
* @author: zhangdingwen
* @create time: 2022-11-1
*  */

#ifndef PLOT_LIGHT_H
#define PLOT_LIGHT_H

#include "PlotItemBase.h"
#include <QColor>
#include <QMap>

#include <tuple>

class PlotLight : public PlotItemBase
{
	Q_OBJECT
public:
	PlotLight(QWidget* parent = Q_NULLPTR);
	~PlotLight();

    static int m_instanceCount; //实体个数

    PlotType plotType() const override
    {
        return Type_PlotLight;
    }

public slots:
    void onLightConstraintUpdate(
        const QList<std::tuple<int32_t, QString, QString, double, QString>>& constraintList);

private:
    void updateDataForDataPairsByTime(double secs) override;

    // 通过约束信息解析数据，生成最终需要的绘制数据
    void processDataByConstraints();
    void initPlot();
    // 通过最终绘图数据更新图表
    void updatePlotByDrawData();
    // 清理上次绘制内容
    void clearPlotContent();
    // 根据约束条件返回指定数据的颜色信息
    QColor getColorByDataPairWithCon(int32_t id, const QString& attr, double value);
    // 数据源没有发生变化，仅当一些设置发生变化时，调用此刷新接口
    void updatePlotByCurrentData();
    // 动态计算圆圈适合的半径
    void calculateRaidus();

private:
    // 原始的属性-值约束信息，由添加数据对时的界面进行编辑，和数据对是独立的两对数据 entityid,attr,condition,threshold,colorName
    QList<std::tuple<int32_t, QString, QString, double, QString>> m_constraintList;
    // 默认灯颜色，当没有约束信息时，显示默认颜色
    QColor m_defaultLightColor = QColor(Qt::green);
    // 默认文本颜色
    QColor m_defaultTextColor = QColor(Qt::yellow);
    // 缓存数据 desc,entityid,entityName,attr,value
    QList<std::tuple<QString, int32_t, QString, QString, double>> m_dataList;
    // 处理后的用于直接绘制的数据 desc,color
    QList<QPair<QString, QColor>> m_drawDataList;
    // 绘制的元素指针列表，用于清除之后重绘
    QList<QCPAbstractItem*> m_drawItemList;
    // 状态灯和文本之间的距离间隔
    uint32_t m_verPadding = 100;
    // 每一行数据之间的水平间隔
    uint32_t m_horPadding = 20;
    // 外边框与内容之间的内间距
    uint32_t m_innerPadding = 10;
    // 状态灯半径
    uint32_t m_circleRadius = 100;
    // 是否按照行显示数据
    bool m_isFilledByRow = true;
};
#endif // _PLOT_LIGHT_H_
