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

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

    void setHorzGrids(uint count);
    void setVertGrids(uint count);
    void setAxisColorWidth(QColor color, uint width);
    void setGridColorWidth(QColor color, uint width);

    void setGridFillColor(QColor color);

    void setGridVisible(bool enable);
    void setTickLabelColor(QColor& color);
    void setTickLabelFont(QFont& font);
    void setTickLabelFontSize(int size);
    void setGridStyle(GridStyle style);
    void setGridDensity(GridDensity density);

    void setUnitsShowX(bool on);
    void setUnitsShowY(bool on);
    void setUnitsX(const QString& units);
    void setUnitsY(const QString& units);
    void setTitle(QString& str);
    void setTitleColor(QColor& color);
    void setTitleFillColor(QColor& color);
    void setTitleFontSize(int size);
    void setTitleFont(QFont& font);
    void setTitleVisible(bool show);

    PlotType plotType() const override
    {
        return Type_PlotRTI;
    }

private:
    //    void updateDataForDataPairsByTime(double secs) override;

    //    void updateGraph(double secs, int index, DataPair* data);
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
