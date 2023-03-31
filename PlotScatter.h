#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include "qcustomplot.h"
#include <QMap>
#include <QString>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    PlotScatter(QWidget* parent = nullptr);
    ~PlotScatter();

    enum AxisType
    {
		xAxis = 0,
		yAxis,
		xAxis2,
		yAxis2
	};

    static int m_instanceCount; //Plot实体个数

    QString getxAxisLabel() const
    {
        return m_xAxisLabel;
    }
    QString getyAxisLabel() const
    {
        return m_yAxisLabel;
    }
    QColor getAxisLabelColor() const
    {
        return m_axisLabelColor;
    }
    QFont getAxisLabelFont() const
    {
        return m_axisLabelFont;
    }

	void setPaddings(double top, double bottom, double left, double right);

	void setxAxisLabel(QString& str);
	void setyAxisLabel(QString& str);
	void setAxisLabelColor(QColor& color);
	void setAxisLabelFont(QFont& font);

	void setAxisVisible(bool on, AxisType type);
	void setAxisTickLabelShow(bool on, AxisType type);

	void rescale_xAxis(bool);
	void rescale_yAxis(bool);
	void rescaleAxis(bool);

    void addPlotPairData(const QPair<QString, QString>& pair) override;
    void delPlotPairData(const QPair<QString, QString>&);
    void updatePlotPairData(const QPair<QString, QString>& oldPair,
                            const QPair<QString, QString>& newPair);

	virtual void setHorzGrids(uint);
	virtual void setVertGrids(uint);

	virtual void setAxisColorWidth(QColor, uint);
	virtual void setGridColorWidth(QColor, uint);

	virtual void setGridFillColor(QColor color);
    virtual QColor getGridFillColor()
    {
        return m_gridFillColor;
    }
	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitle(QString& str);
	virtual void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont& font);
	virtual void setTitleFontSize(int size);
	virtual void setTitleVisible(bool show);

    virtual PlotType plotType() const
    {
        return Type_PlotScatter;
    }

public slots:
    void onGetCurrentSeconds(double secs) override;

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
	void initPlot();
    void getDataInfo(double secs);
	void updateData(double secs, int index, DataPair* data);

private:
    double m_curSeconds;
    double m_topPadding; //绘图间隔-top
    double m_bottomPadding; //绘图间隔-bottom
    double m_leftPadding; //绘图间隔-left
    double m_rightPadding; //绘图间隔-right

    struct ScatterInfo
	{
		QPointer<QCPGraph> graph;
		QPointer<QCPItemTracer> tracer;
		QPointer<QCPItemText> tracerText;

        ScatterInfo()
        {
            graph = nullptr;
            tracer = nullptr;
            tracerText = nullptr;
        }
	};

	QMap<QPair<QString, QString>, ScatterInfo> m_mapScatter;
};

#endif // PLOTSCATTER_H
