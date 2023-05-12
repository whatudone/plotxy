#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"

#include <QMap>
#include <QPointF>
#include <unordered_map>

/*
 * RTI图是一种把三维数据转化为二维显示的图。
 * x和y分别为第三个维度的数据的索引，通过这个索引，来确定z的value
 * 然后在[x,y]对应的点上绘制带颜色信息的点，不同颜色表示不同的值大小，
 * 比如显示 [范围-时间-信号强度]这种多因素影响的信号强度图表
 * 根据说明书描述，y轴固定为时间轴
*/
class PlotRTI : public PlotItemBase
{
	Q_OBJECT

public:
	PlotRTI(QWidget* parent = Q_NULLPTR);
	~PlotRTI();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

	void initPlot();

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

    PlotType plotType() const override
    {
        return Type_PlotRTI;
    }

private:
    void updateDataForDataPairsByTime(double secs) override;

    void updateGraph(double secs, DataPair* data);
    // 加载测试数据
    void loadCustomData();

public:
    static int m_instanceCount; //实体个数

private:
    // 颜色二维图例,只存在这一个数据显示图例
    QCPColorMap* m_colorMap = nullptr;
    // 颜色对应的值
    QCPColorScale* m_colorScale = nullptr;
    // 三维数据
    QMap<QPair<double, double>, double> m_dataMap;
};
