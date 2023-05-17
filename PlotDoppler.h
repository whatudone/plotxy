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
    void loadCustomData();
    // 通过指定X值获取对应的切片数据
    void getYToValueVecByX(double x, QVector<double>& yVec, QVector<double>& dataVec);
    // 通过指定Y值获取对应的切片数据
    void getXToValueVecByY(double y, QVector<double>& xVec, QVector<double>& dataVec);

private:
    QCPColorMap* m_colorMap = nullptr;
    QCPColorScale* m_colorScale = nullptr;
    // 水平和垂直的AScope图表
    QCustomPlot* m_horizonAScope = nullptr;
    QCustomPlot* m_verticalAScope = nullptr;
    // 三维强度数据
    QMap<QPair<double, double>, double> m_dataMap;
    // 水平数据 y -> <x,value>
    QMultiMap<double, QPair<double, double>> m_horizonDataMap;
    // 垂直数据 x -> <y,value>
    QMultiMap<double, QPair<double, double>> m_verticalDataMap;
};
