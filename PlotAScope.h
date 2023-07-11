#pragma once
#include "PlotItemBase.h"
#include "qcustomplot.h"
class PlotAScope : public PlotItemBase
{
	Q_OBJECT

public:
	PlotAScope(QWidget* parent = Q_NULLPTR);
    ~PlotAScope() override;

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

    void updateGraphByDataPair(DataPair* data, double curSecs) override;

    void drawGate();

public:
    static int m_instanceCount; //实体个数
    bool isDrawGate() const;
    void setIsDrawGate(bool isDrawGate);

    int gateHeight() const;
    void setGateHeight(int gateHeight);

    QColor gateColor() const;
    void setGateColor(const QColor& gateColor);

    bool isAutofitX() const;
    void setIsAutofitX(bool isAutofitX);

    bool isAutofitY() const;
    void setIsAutofitY(bool isAutofitY);

private:
    //  一个数据对对应一个绘图图例，自绘图形需要在paintEvent中自行处理
    QMap<QString, QCPGraph*> m_graphMap;
    //  缓存数据
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
    bool m_isDrawGate;
    int m_gateHeight;
    QColor m_gateColor;
    bool m_isAutofitX;
    bool m_isAutofitY;
    QCPItemRect* m_gateRectItem = nullptr;
    QCPItemText* m_gateText = nullptr;
    double m_minX;
    double m_maxX;
    double m_minY;
    double m_maxY;
};
