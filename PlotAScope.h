#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotAScope : public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
	~PlotAScope();

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
    void setMinimumMargins(const QMargins& margins);

    PlotType plotType() const override
    {
        return Type_PlotAScope;
    }

    void delPlotPairData(const QString& uuid) override;

private:
    void updateDataForDataPairsByTime(double secs) override;

    void updateGraph(double secs, DataPair* data);

public:
    static int m_instanceCount; //实体个数
private:
    // 一个数据对对应一个绘图图例，自绘图形需要在paintEvent中自行处理
    QMap<QString, QCPGraph*> m_graphMap;
    // 多个图例共用一个游标,使用legend切换图例
    QCPItemTracer* m_tracer = nullptr;
    // 游标坐标显示文本,x+y合并在一起显示
    QCPItemText* m_tracerText = nullptr;
    // TODO:这三个成员变量需要移动到基类中，处理所有的qcustomplot类型图表，散点图也要对应整改
};
