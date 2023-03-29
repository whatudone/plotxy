/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot基类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef PLOT_ITEM_BASE_H_
#define PLOT_ITEM_BASE_H_

#include "DataPair.h"
#include "constdef.h"
#include "ui_PlotItemBase.h"
#include <QWidget>

class PlotItemBase : public QWidget
{
	Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = nullptr);
	~PlotItemBase();
    enum ResizeDirection
    {
        NORTH_MIDDLE,
        NORTH_EAST,
        EAST_MIDDLE,
        SOUTH_EAST,
        SOUTH_MIDDLE,
        SOUTH_WEST,
        WEST_MIDDLE,
        NORTH_WEST
    };

public:
    void init(); //初始化函数，连接信号槽

    void setPosition(int x, int y); //设置包围盒左上角位置
    void setWidth(int width); //设置宽度
    void setHeight(int height); //设置包围盒高度
	void setRect(QRect rect);
	void setName(const QString&);
	void setTabName(const QString&);

	QPoint currPosition();
	int currWidth();
	int currHeight();
	QRect currRect();
	QString currName();
	QString currTabName();

	//setters:
	virtual void setOuterFillColor(QColor color);
	virtual void setOutlineColor(QColor color);
	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual void setGridVisible(bool enable);
    virtual void setTickLabelColor(const QColor& color);
    virtual void setTickLabelFont(const QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);
	virtual void setGridFillColor(QColor color);
	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitleVisible(bool on);
    virtual void setTitle(const QString& title);
    virtual void setTitleColor(const QColor& color);
    virtual void setTitleFillColor(const QColor& color);
    virtual void setTitleFont(const QFont& font);
	virtual void setTitleFontSize(int size);
    virtual void setxAxisLabel(const QString& label);
    virtual void setyAxisLabel(const QString& label);
    virtual void setAxisLabelColor(const QColor& color);
    virtual void setAxisLabelFont(const QFont& font);
	virtual void setAxisLabelFontSize(int size);

	//getters:
    virtual QColor getOuterFillColor()
    {
        return m_outerFillColor;
    }
    virtual QColor getOutlineColor()
    {
        return m_outlineColor;
    }
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
    virtual uint getHorzGrids()
    {
        return m_horzGrids;
    }
    virtual uint getVertGrids()
    {
        return m_vertGrids;
    }
    virtual uint getAxisWidth()
    {
        return m_axisWidth;
    }
    virtual uint getGridWidth()
    {
        return m_gridWidth;
    }
    virtual QColor getAxisColor()
    {
        return m_axisColor;
    }
    virtual QColor getGridColor()
    {
        return m_gridColor;
    }
    virtual bool getGridVisible()
    {
        return m_gridVisible;
    }
    virtual QColor getTickLabelColor()
    {
        return m_tickLabelColor;
    }
    virtual QFont getTickLabelFont()
    {
        return m_tickLabelFont;
    }
    virtual int getTickLabelFontSize()
    {
        return m_tickLabelFontSize;
    }
    virtual Qt::PenStyle getGridStyle()
    {
        return m_gridStyle;
    }
    virtual GridDensity getGridDensity()
    {
        return m_gridDensity;
    }
    virtual QColor getGridFillColor()
    {
        return m_gridFillColor;
    }
    virtual bool unitsShowX()
    {
        return m_showUnits_x;
    }
    virtual bool unitsShowY()
    {
        return m_showUnits_y;
    }
    virtual QString getUnitsX()
    {
        return m_units_x;
    }
    virtual QString getUnitsY()
    {
        return m_units_y;
    }
    virtual bool getTitleVisible()
    {
        return m_titleVisible;
    }
    virtual QString getTitle()
    {
        return m_title;
    }
    virtual QColor getTitleColor()
    {
        return m_titleColor;
    }
    virtual QColor getTitleFillColor()
    {
        return m_titleFillColor;
    }
    virtual QFont getTitleFont()
    {
        return m_titleFont;
    }
    virtual int getTitleFontSize()
    {
        return m_titleFontSize;
    }
    virtual QString getxAxisLabel()
    {
        return m_xAxisLabel;
    }
    virtual QString getyAxisLabel()
    {
        return m_yAxisLabel;
    }
    virtual QColor getAxisLabelColor()
    {
        return m_axisLabelColor;
    }
    virtual QFont getAxisLabelFont()
    {
        return m_axisLabelFont;
    }
    virtual int getAxisLabelFontSize()
    {
        return m_axisLabelFontSize;
    }

    virtual void addPlotPairData(const QPair<QString, QString>& pair);
    virtual void delPlotPairData(const QPair<QString, QString>& pair);
    virtual void updatePlotPairData(const QPair<QString, QString>& oldPair,
                                    const QPair<QString, QString>& newPair);
    QVector<DataPair*> getDataPairs()
    {
        return m_dataPairs;
    }
	void setDataPair(QVector<DataPair*> newVector);

    virtual PlotType plotType() const
    {
        return Type_PlotUnknown;
    }

    bool getIsNeedDrawBorder() const;
    void setIsNeedDrawBorder(bool isNeedDrawBorder);

    bool isContainedInResizeRects(const QPoint& point);

    void updateGeoWithMouseMove(int offsetX, int offsetY);

private:
    void updateResizeFocusPos();
    QRect getRectByDirection(ResizeDirection direction);
    void setCursorByDirection();

    void drawBorderAndControls();

protected:
    void paintEvent(QPaintEvent* event) override;

protected:
    QVector<DataPair*> m_dataPairs;
	//General
	QColor m_outerFillColor;
	QColor m_outlineColor;

	//Axis and Grid
    double m_coordBgn_x; //x坐标起始值
    double m_coordEnd_x; //x坐标结束值
    double m_coordBgn_y; //y坐标起始值
    double m_coordEnd_y; //y坐标结束值
    uint m_horzGrids; //横向grid个数
    uint m_vertGrids; //纵向grid个数
    uint m_axisWidth; //坐标轴宽度
    uint m_gridWidth; //grid宽度
    QColor m_axisColor; //坐标轴颜色
    QColor m_gridColor; //grid颜色
    bool m_gridVisible; //是否显示grid
    QColor m_tickLabelColor; //坐标刻度值颜色
    QFont m_tickLabelFont; //坐标刻度值字体
    int m_tickLabelFontSize; //坐标刻度值字体尺寸
    Qt::PenStyle m_gridStyle; //grid风格
    GridDensity m_gridDensity; //grid密度
    QColor m_gridFillColor; //grid填充色，即背景色

	//Text
    QString m_units_x; //x轴单位
    QString m_units_y; //y轴单位
    bool m_showUnits_x; //是否显示x轴单位
    bool m_showUnits_y; //是否显示y轴单位

    QString m_title; //标题
    bool m_titleVisible; //是否显示标题
    QColor m_titleColor; //标题颜色
    QFont m_titleFont; //标题字体
    int m_titleFontSize; //标题字体尺寸
    QColor m_titleFillColor; //标题填充色

    QString m_xAxisLabel; //x轴标题
    QString m_yAxisLabel; //y轴标题
    QColor m_axisLabelColor; //坐标轴标题颜色
    QFont m_axisLabelFont; //坐标轴标题字体
    int m_axisLabelFontSize; //坐标轴标题字体尺寸

public slots:
	void slot_updateRect(QRect);
	void slot_setVisible(bool);
    virtual void onGetCurrentSeconds(double /*secs*/) {}

    virtual void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
signals:
    void sgn_dataPairChanged(PlotItemBase* pBaseItem); //数据对改变信号,tabName,plotName

private:
    QPoint m_position;
    int m_width;
    int m_height;
    QString m_plotItemName;
    QString m_tabName;
    bool m_bVisible;
    QMap<ResizeDirection, QRect> m_resizeRectMap;
    ResizeDirection m_curResizeDirection;
    bool m_isNeedDrawBorder = false;
    const int32_t m_resizeFocusSize = 20;
    Ui::PlotItemBase ui;
};

#endif // !
