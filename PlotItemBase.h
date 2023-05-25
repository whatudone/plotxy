/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot基类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef PLOT_ITEM_BASE_H_
#define PLOT_ITEM_BASE_H_

#include "DataPair.h"
#include "constdef.h"
#include "qcustomplot.h"
#include "ui_PlotItemBase.h"

#include <QWidget>

class QCustomPlot;
class PlotItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = nullptr);
    ~PlotItemBase() override;
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
    void setRect(const QRect& rect);
    void setName(const QString&);
    void setTabName(const QString&);

    QPoint currPosition();
    int currWidth();
    int currHeight();
    QRect currRect();
    QString getName();
    QString getTabName();

    //setters:
    virtual void setOuterFillColor(const QColor& color);
    void setOutlineColor(const QColor& color);
    virtual void setCoordRangeX(double lower, double upper);
    virtual void setCoordRangeY(double lower, double upper);
    virtual void setHorzGrids(uint count);
    virtual void setVertGrids(uint count);
    virtual void setAxisColorWidth(const QColor& color, uint width);
    virtual void setGridColorWidth(const QColor& color, uint width);
    virtual void setGridVisible(bool enable);
    virtual void setTickLabelColor(const QColor& color);
    virtual void setTickLabelFont(const QFont& font);
    virtual void setTickLabelFontSize(int size);
    virtual void setGridStyle(GridStyle style);
    virtual void setGridDensity(GridDensity density);
    virtual void setGridFillColor(const QColor& color);
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
    virtual void setTitleOffset(int offsetX, int offsetY);
    virtual void setxAxisLabel(const QString& label);
    virtual void setyAxisLabel(const QString& label);
    virtual void setAxisLabelColor(const QColor& color);
    virtual void setAxisLabelFont(const QFont& font);
    virtual void setAxisLabelFontSize(int size);

    void setPaddings(double top, double bottom, double left, double right);
    void updateTitle();

    //getters:
    QColor getOuterFillColor()
    {
        return m_outerFillColor;
    }
    QColor getOutlineColor()
    {
        return m_outlineColor;
    }
    // 某些图坐标系不一样，比如polar
    virtual void getCoordRangeX(double& lower, double& upper);
    virtual void getCoordRangeY(double& lower, double& upper);
    uint getHorzGrids()
    {
        return m_horzGrids;
    }
    uint getVertGrids()
    {
        return m_vertGrids;
    }
    uint getAxisWidth()
    {
        return m_axisWidth;
    }
    uint getGridWidth()
    {
        return m_gridWidth;
    }
    QColor getAxisColor()
    {
        return m_axisColor;
    }
    QColor getGridColor()
    {
        return m_gridColor;
    }
    bool getGridVisible()
    {
        return m_gridVisible;
    }
    QColor getTickLabelColor()
    {
        return m_tickLabelColor;
    }
    QFont getTickLabelFont()
    {
        return m_tickLabelFont;
    }
    int getTickLabelFontSize()
    {
        return m_tickLabelFontSize;
    }
    Qt::PenStyle getGridStyle()
    {
        return m_gridStyle;
    }
    GridDensity getGridDensity()
    {
        return m_gridDensity;
    }
    QColor getGridFillColor()
    {
        return m_gridFillColor;
    }
    bool unitsShowX()
    {
        return m_showUnits_x;
    }
    bool unitsShowY()
    {
        return m_showUnits_y;
    }
    QString getUnitsX()
    {
        return m_units_x;
    }
    QString getUnitsY()
    {
        return m_units_y;
    }
    bool getTitleVisible()
    {
        return m_titleVisible;
    }
    QString getTitle()
    {
        return m_title;
    }
    QColor getTitleColor()
    {
        return m_titleColor;
    }
    QColor getTitleFillColor()
    {
        return m_titleFillColor;
    }
    QFont getTitleFont()
    {
        return m_titleFont;
    }
    int getTitleFontSize()
    {
        return m_titleFontSize;
    }
    QString getxAxisLabel()
    {
        return m_xAxisLabel;
    }
    QString getyAxisLabel()
    {
        return m_yAxisLabel;
    }
    QColor getAxisLabelColor()
    {
        return m_axisLabelColor;
    }
    QFont getAxisLabelFont()
    {
        return m_axisLabelFont;
    }
    int getAxisLabelFontSize()
    {
        return m_axisLabelFontSize;
    }

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QVariantList& extraParams = QVariantList());
    virtual void delPlotPairData(const QString& uuid);
    virtual void updatePlotPairData(const QString& uuid,
                                    int32_t xEntityID,
                                    const QString& xAttrName,
                                    const QString& xAttrUnitName,
                                    int32_t yEntityID,
                                    const QString& yAttrName,
                                    const QString& yAttrUnitName);
    const QVector<DataPair*>& getDataPairs()
    {
        return m_dataPairs;
    }

    DataPair* getDataPariByUuid(const QString& uuid);
    void setDataPair(QVector<DataPair*>& newVector);

    virtual PlotType plotType() const
    {
        return PlotType::Type_PlotUnknown;
    }

    bool getIsNeedDrawBorder() const;
    void setIsNeedDrawBorder(bool isNeedDrawBorder);

    bool isContainedInResizeRects(const QPoint& point);

    void updateGeoWithMouseMove(int offsetX, int offsetY);

    virtual void exportDataToFile(const QString& /*filename*/) const {}

    bool getXIsAdaptive() const;
    void setXIsAdaptive(bool xIsAdaptive);

    bool getYIsAdaptive() const;
    void setYIsAdaptive(bool yIsAdaptive);
    // 针对两种类型的通用图表更新接口
    void replot();

    void setInteract(QCP::Interaction inter);
    void setZoom(uint mode);
    // 还原缩放设置
    void clearInter();
    void setNewTickOrigin(const QPoint& point);
    void setIsDrawMeasureLine(bool isDraw);

    void setCustomPlotMouseTransparent(bool on);
    void setCustomPlotMouseTransparent(bool baseTransparent, bool customplotTransparent);

    void setupLayout();

    bool getIsHorizonBar() const;
    virtual void setIsHorizonBar(bool isHorizonBar);

    int getBarBetweenPadding() const;
    void setBarBetweenPadding(int barBetweenPadding);

    int getBarRightPadding() const;
    void setBarRightPadding(int barRightPadding);

    int getBarLeftPadding() const;
    void setBarLeftPadding(int barLeftPadding);

    void rescaleXAxis(bool on);
    void rescaleYAxis(bool on);
    void rescaleAxis(bool on);

private:
    void updateResizeFocusPos();
    QRect getRectByDirection(ResizeDirection direction);
    void setCursorByDirection();

    void drawBorderAndControls();

private slots:
    // 响应整体数据变化
    void onDataPairsChanged();

protected:
    void paintEvent(QPaintEvent* event) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual bool eventFilter(QObject* obj, QEvent* event) override;

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
    int m_titleOffsetX; //标题X偏移量
    int m_titleOffsetY; //标题Y偏移量

    QString m_xAxisLabel; //x轴标题
    QString m_yAxisLabel; //y轴标题
    QColor m_axisLabelColor; //坐标轴标题颜色
    QFont m_axisLabelFont; //坐标轴标题字体
    int m_axisLabelFontSize; //坐标轴标题字体尺寸
    double m_seconds; // 时间，用于筛选数据

    bool m_isHorizonBar; // Bar是否是水平
    int m_barBetweenPadding; // Bar之间的间隔
    int m_barLeftPadding; // Bar和左边的间隔
    int m_barRightPadding; // Bar和右边的间隔

    int32_t m_topPadding; //绘图间隔-top
    int32_t m_bottomPadding; //绘图间隔-bottom
    int32_t m_leftPadding; //绘图间隔-left
    int32_t m_rightPadding; //绘图间隔-right

public slots:
    void slot_updateRect(const QRect&);
    void slot_setVisible(bool);
    // 响应主界面时间滑块时间产生的信号，按照具体时间过滤数据，数据里面都是带有时间
    void onGetCurrentSeconds(double secs);

    // 响应内部DataPair数据发送的change信号
    virtual void onDataPairUpdateData();
    // 控制图表本身是否可以响应鼠标事件，比如可以拖拽图表的坐标轴等，因为部分是自绘。所以处理方式不同
    virtual void onPlotMouseEventEnable(bool on);

private:
    // 滑块时间变化时,数据整体发生变化，需要全部更新
    virtual void updateDataForDataPairsByTime(double);
    // 数据对属性发生变化时，数据没有发生变化，但是需要重新绘图，此时不需要重新拉取数据
    virtual void updateGraphByDataPair(DataPair* dataPair);
    // 自绘图表需要填充此虚函数实现自己的绘制逻辑
    virtual void customPainting(QPainter&);
signals:
    void dataPairsChanged(PlotItemBase* pBaseItem); //整体数据发生变化
protected:
    // 部分为自绘,不需要这个控件就不需要创建，需要的子类自己初始化
    QCustomPlot* m_customPlot = nullptr;
    // 自绘窗口，则初始化这个控件
    QWidget* m_widget = nullptr;

private:
    QPoint m_position;
    int m_width;
    int m_height;
    // 目前的唯一标识符
    QString m_plotItemName;
    QString m_tabName;
    bool m_bVisible;
    QMap<ResizeDirection, QRect> m_resizeRectMap;
    ResizeDirection m_curResizeDirection;
    bool m_isNeedDrawBorder = false;
    const int32_t m_resizeFocusSize = 20;
    Ui::PlotItemBase ui;
    // 默认自适应缩放关闭
    bool m_xIsAdaptive = false;
    bool m_yIsAdaptive = false;
    bool m_isDrawMeasureLine = false;

    // 测距辅助线段
    QPoint m_originPoint;
    QCPItemLine* m_measureLineItem = nullptr;
    QCPItemText* m_measureTextItem = nullptr;

    // 标题Label
    QLabel* pTitleLabel = nullptr;
    QVBoxLayout* mainLayout = nullptr;
    QHBoxLayout* titleLayout = nullptr;
};

#endif // !
