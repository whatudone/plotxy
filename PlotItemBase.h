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
    virtual void setHorzGrids(int32_t count);
    virtual void setVertGrids(int32_t count);
    virtual void setAxisColorWidth(const QColor& color, int32_t width);
    virtual void setGridColorWidth(const QColor& color, int32_t width);
    virtual void setGridVisible(bool enable);
    virtual void setxTickLabelVisible(bool show);
    virtual void setyTickLabelVisible(bool show);
    virtual void setxTickLabelColor(const QColor& color);
    virtual void setxTickLabelFont(const QFont& font);
    virtual void setxTickLabelFontSize(int size);
    virtual void setyTickLabelColor(const QColor& color);
    virtual void setyTickLabelFont(const QFont& font);
    virtual void setyTickLabelFontSize(int size);
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
    virtual void setxAxisLabelVisible(bool on);
    virtual void setyAxisLabelVisible(bool on);
    virtual void setxAxisLabel(const QString& label);
    virtual void setyAxisLabel(const QString& label);
    virtual void setxAxisLabelColor(const QColor& color);
    virtual void setxAxisLabelFont(const QFont& font);
    virtual void setxAxisLabelFontSize(int size);
    virtual void setyAxisLabelColor(const QColor& color);
    virtual void setyAxisLabelFont(const QFont& font);
    virtual void setyAxisLabelFontSize(int size);
    virtual void setXPrecision(int value);
    virtual void setYPrecision(int value);

    // 滑块时间变化时,数据整体发生变化，需要全部更新
    virtual void updateDataForDataPairsByTime(double);

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
    int32_t getHorzGrids()
    {
        return m_horzGrids;
    }
    int32_t getVertGrids()
    {
        return m_vertGrids;
    }
    int32_t getAxisWidth()
    {
        return m_axisWidth;
    }
    int32_t getGridWidth()
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
    QColor getxTickLabelColor()
    {
        return m_xTickLabelColor;
    }
    QFont getxTickLabelFont()
    {
        return m_xTickLabelFont;
    }
    int getxTickLabelFontSize()
    {
        return m_xTickLabelFontSize;
    }
    QColor getyTickLabelColor()
    {
        return m_yTickLabelColor;
    }
    QFont getyTickLabelFont()
    {
        return m_yTickLabelFont;
    }
    int getyTickLabelFontSize()
    {
        return m_yTickLabelFontSize;
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
    bool getxAxisLabelVisible()
    {
        return m_xAxisLabelVisible;
    }
    bool getyAxisLabelVisible()
    {
        return m_yAxisLabelVisible;
    }
    QString getxAxisLabel()
    {
        return m_xAxisLabel;
    }
    QString getyAxisLabel()
    {
        return m_yAxisLabel;
    }
    QColor getxAxisLabelColor()
    {
        return m_xAxisLabelColor;
    }
    QFont getxAxisLabelFont()
    {
        return m_xAxisLabelFont;
    }
    int getxAxisLabelFontSize()
    {
        return m_xAxisLabelFontSize;
    }
    QColor getyAxisLabelColor()
    {
        return m_yAxisLabelColor;
    }
    QFont getyAxisLabelFont()
    {
        return m_yAxisLabelFont;
    }
    int getyAxisLabelFontSize()
    {
        return m_yAxisLabelFontSize;
    }
    bool getxAxisTickLabelVisible()
    {
        return m_xTickLabelVisible;
    }
    bool getyAxisTickLabelVisible()
    {
        return m_yTickLabelVisible;
    }
    int getXPrecision()
    {
        return m_xPrecision;
    }
    int getYPrecision()
    {
        return m_yPrecision;
    }

    virtual DataPair*
    addPlotDataPair(int32_t xEntityID,
                    const QString& xAttrName,
                    const QString& xAttrUnitName,
                    int32_t yEntityID,
                    const QString& yAttrName,
                    const QString& yAttrUnitName,
                    const QHash<QString, QVariant>& extraParams = QHash<QString, QVariant>(),
                    bool isFromJson = false);
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

    virtual void rescaleXAxis();
    virtual void rescaleYAxis();
    virtual void rescaleAxis();

    void addEvent(const EventSettings& event);
    void removeEvent(const QString& entityName, const QString& type);

    QList<EventSettings> getEventList() const;
    void setEventList(const QList<EventSettings>& eventList);

    bool getBVisible() const;
    void setBVisible(bool bVisible);
    virtual void drawGOGData();

    void addGOGFile(const QString& fileName);
    void removeGOGFile(const QString& fileName);
    QList<QString> getGOGFileList();

    QMap<QString, QList<GOGDataInfo>> getGogDataMap() const;

    QMap<QString, GOGCustomSetting> getGogCustomSettings() const;
    void setGogCustomSettings(const QMap<QString, GOGCustomSetting>& gogCustomSettings);

    int getOutlineWidth() const;
    void setOutlineWidth(int outlineWidth);

    double getXRate() const;
    void setXRate(double xRate);
    double getYRate() const;
    void setYRate(double yRate);

private:
    void updateResizeFocusPos();
    QRect getRectByDirection(ResizeDirection direction);
    void setCursorByDirection();

    void drawBorderAndControls();
    void loadGOGFile(const QString& fileName);
    void updateXLabelDescrption();
    void updateYLabelDescrption();

private slots:
    // 响应整体数据变化
    void onDataPairsChanged();
    void onXAxisRangeChanged(const QCPRange& range);
    void onYAxisRangeChanged(const QCPRange& range);

protected:
    void paintEvent(QPaintEvent* event) override;

    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;

    virtual bool eventFilter(QObject* obj, QEvent* event) override;

    // 刷新时间线事件
    virtual void updateTimelineGraph() {}

protected:
    // 数据对列表
    QVector<DataPair*> m_dataPairs;
    //General
    QColor m_outerFillColor; // 整体背景
    QColor m_outlineColor; // 外边框线
    int m_outlineWidth; // 外边框线宽度

    //Axis and Grid
    double m_coordBgn_x; //x坐标起始值
    double m_coordEnd_x; //x坐标结束值
    double m_coordBgn_y; //y坐标起始值
    double m_coordEnd_y; //y坐标结束值
    int32_t m_horzGrids; //横向grid个数
    int32_t m_vertGrids; //纵向grid个数
    int32_t m_axisWidth; //坐标轴宽度
    int32_t m_gridWidth; //grid宽度
    QColor m_axisColor; //坐标轴颜色
    QColor m_gridColor; //grid颜色
    bool m_gridVisible; //是否显示grid
    bool m_xTickLabelVisible; //是否显示x轴刻度
    bool m_yTickLabelVisible; //是否显示y轴刻度
    QColor m_xTickLabelColor; //坐标刻度值颜色
    QFont m_xTickLabelFont; //坐标刻度值字体
    int m_xTickLabelFontSize; //坐标刻度值字体尺寸
    QColor m_yTickLabelColor; //坐标刻度值颜色
    QFont m_yTickLabelFont; //坐标刻度值字体
    int m_yTickLabelFontSize; //坐标刻度值字体尺寸
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
    bool m_xAxisLabelVisible; //是否显示X轴描述信息
    bool m_yAxisLabelVisible; //是否显示y轴描述信息
    QColor m_xAxisLabelColor; //坐标轴标题颜色
    QFont m_xAxisLabelFont; //坐标轴标题字体
    int m_xAxisLabelFontSize; //坐标轴标题字体尺寸
    QColor m_yAxisLabelColor; //坐标轴标题颜色
    QFont m_yAxisLabelFont; //坐标轴标题字体
    int m_yAxisLabelFontSize; //坐标轴标题字体尺寸

    bool m_isHorizonBar; // Bar是否是水平
    int m_barBetweenPadding; // Bar之间的间隔
    int m_barLeftPadding; // Bar和左边的间隔
    int m_barRightPadding; // Bar和右边的间隔

    int32_t m_topPadding; //绘图间隔-top
    int32_t m_bottomPadding; //绘图间隔-bottom
    int32_t m_leftPadding; //绘图间隔-left
    int32_t m_rightPadding; //绘图间隔-right

    int m_xPrecision; //X轴数据精度
    int m_yPrecision; //Y轴数据精度
    double m_xRate; //x轴数据倍率（用于坐标转换）
    double m_yRate; //y轴数据倍率（用于坐标转换）

    // gog数据
    QMap<QString, QList<GOGDataInfo>> m_gogDataMap;
    QList<QString> m_allGOGFileList;
    QMap<QString, GOGCustomSetting> m_gogCustomSettings; //用户自定义设置  filename,settings

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
    // 数据对属性发生变化时，数据没有发生变化，但是需要重新绘图，此时不需要重新拉取数据
    virtual void updateGraphByDataPair(DataPair* dataPair, double curSecs);
    // 自绘图表需要填充此虚函数实现自己的绘制逻辑
    virtual void customPainting(QPainter&);
signals:
    void dataPairsChanged(PlotItemBase* pBaseItem); //整体数据发生变化
    void coordRangeChanged(bool isX, double min, double max);

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

    // Event
    QList<EventSettings> m_eventList;
};
Q_DECLARE_OPAQUE_POINTER(PlotItemBase)
#endif // !
