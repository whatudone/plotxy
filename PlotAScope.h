#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotAScope : public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
	~PlotAScope();

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

    void updateGraphByDataPair(DataPair* data);

public:
    static int m_instanceCount; //实体个数
private:
    //  一个数据对对应一个绘图图例，自绘图形需要在paintEvent中自行处理
    QMap<QString, QCPGraph*> m_graphMap;
    //  缓存数据
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
};
