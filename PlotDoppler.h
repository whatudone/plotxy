#pragma once
#include "PlotAScope.h"
#include "PlotItemBase.h"
#include "qcustomplot.h"

/*
 * Doppler图表由三部分组成，中间是一个RTI形式的QCPColorMap,左边附带一个QCPColorScala强度对照表
 * 右边是一个垂直的ASCope图表，下方是一个水平的AScope图表,当颜色图中选择某个具体点时，此时有一个[x,y]具体切点，
 * 此时水平图表展示y-value，以当前x为基准。垂直图展示x-value的切片数据，以当前y为基准。
*/
class PlotDoppler : public PlotItemBase
{
	Q_OBJECT

public:
	PlotDoppler(QWidget* parent = Q_NULLPTR);
	~PlotDoppler();

	void initPlot();

    PlotType plotType() const override
    {
        return Type_PlotDoppler;
    }

public:
    static int m_instanceCount; //实体个数

private:
    // 通过指定X值获取对应的切片数据
    void getYToValueVecByX(double x, QVector<double>& yVec, QVector<double>& dataVec);
    // 通过指定Y值获取对应的切片数据
    void getXToValueVecByY(double y, QVector<double>& xVec, QVector<double>& dataVec);
    // 响应时间变化刷新图表
    void updateDataForDataPairsByTime(double secs) override;
    // 响应DataPair的修改
    void updateGraphByDataPair(DataPair* data);
    // 响应游标移动时，切片点移动，更新水平和垂直的切片图表
    void updateAScopesBySlicePoint(const QPointF& point);

private:
    QCPColorMap* m_colorMap = nullptr;
    QCPColorScale* m_colorScale = nullptr;

    // 三维数据
    QHash<QPair<int32_t, int32_t>, double> m_dataHash;
    // range
    QVector<double> m_rangeList;
    // time
    QVector<double> m_timeList;
    // 水平数据 time -> <range,value>
    QMultiHash<double, QPair<double, double>> m_horizonDataHash;
    // 垂直数据 range -> <time,value>
    QMultiHash<double, QPair<double, double>> m_verticalDataHash;
    // 切片点
    QPointF m_slicePoint;
    // 游标
    QCPItemTracer* m_tracer = nullptr;
    // 文本信息
    QCPItemText* m_tracerText = nullptr;
};
