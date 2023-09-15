#ifndef PLOTSCATTER_H
#define PLOTSCATTER_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include "constdef.h"
#include "qcustomplot.h"
#include <QMap>
#include <QString>
#include <limits>

class PlotScatter : public PlotItemBase
{
    Q_OBJECT
public:
    // 基础绘制组件
    struct DrawComponents
    {
        QPointer<QCPGraph> graph;
        QPointer<QCPItemText> tracerText;
        QPointer<QCPItemPixmap> pixmap;

        DrawComponents()
        {
            graph = nullptr;
            tracerText = nullptr;
            pixmap = nullptr;
        }
    };

    // 背景颜色信息
    struct BackgroundLimitSeg
    {
        QString m_limitName;
        double m_limitValue;
        QColor m_lineColor;
        int32_t m_lineWidth;
        QColor m_fillAboveColor;
        QColor m_fillBelowColor;
        bool operator==(const BackgroundLimitSeg& seg)
        {
            return seg.m_limitName == this->m_limitName;
        }
    };

    // event or marker绘制组合
    struct EventComponents
    {
        QPointer<QCPItemText> text = nullptr;
        QPointer<QCPItemLine> line = nullptr;
    };
    PlotScatter(QWidget* parent = nullptr);
    ~PlotScatter() override;

    static int m_instanceCount; //Plot实体个数

    virtual DataPair* addPlotDataPair(int32_t xEntityID,
                                      const QString& xAttrName,
                                      const QString& xAttrUnitName,
                                      int32_t yEntityID,
                                      const QString& yAttrName,
                                      const QString& yAttrUnitName,
                                      const QHash<QString, QVariant>& extraParams,
                                      bool isFromJson = false) override;
    void delPlotPairData(const QString& uuid) override;

    PlotType plotType() const override
    {
        return Type_PlotScatter;
    }

    void exportDataToFile(const QString& filename) const override;

    void drawGOGData() override;
    void addBackgroundColorInfo(const QString& limitName,
                                double limitValue,
                                const QColor& lineColor,
                                int32_t lineWidth,
                                const QColor& fillAboveColor,
                                const QColor& fillBelowColor);

    void removeBackgroundColorInfo(double value);

    QMap<double, BackgroundLimitSeg> getBkgLimitSegMap() const;

    void addMarker(const PlotMarker& marker);
    void removeMarker(const QString& uuid);
    void modifyMarker(const QString& uuid, const PlotMarker& marker);

    QHash<QString, PlotMarker> getPlotMarkers() const;
    PlotMarker getMarkerByUuid(const QString& uuid) const;

    void addConnection(const ConnectionSetting& con);
    void removeConnection(const QString& uuid);
    void updateConnection(const QString& uuid,
                          const QColor& color,
                          int32_t width,
                          const QString& stipple,
                          int32_t speed);

    QHash<QString, ConnectionSetting> getConHash() const;
    void setConHash(const QHash<QString, ConnectionSetting>& conHash);

    bool getXScrollOn() const;
    void setXScrollOn(bool xScrollOn);

    double getXLead() const;
    void setXLead(double xLead);

    double getXFollow() const;
    void setXFollow(double xFollow);

    QHash<QString, bool> getXScrollHash() const;
    void setXScrollHash(const QHash<QString, bool>& xScrollHash);

    bool getYScrollOn() const;
    void setYScrollOn(bool yScrollOn);

    double getYLead() const;
    void setYLead(double yLead);

    double getYFollow() const;
    void setYFollow(double yFollow);

    QHash<QString, bool> getYScrollHash() const;
    void setYScrollHash(const QHash<QString, bool>& yScrollHash);

    void setXScrollEnableByUUID(const QString& uuid, bool enable);
    bool getXScrollEnableByUUID(const QString& uuid);

    void setYScrollEnableByUUID(const QString& uuid, bool enable);
    bool getYScrollEnableByUUID(const QString& uuid);

    void setBkgLimitSegMap(const QMap<double, BackgroundLimitSeg>& bkgLimitSegMap);

    void setTimeTickerFormat(const QString& format, bool isXAxis);

    QString getXTimeTickFormat() const;

    QString getYTimeTickFormat() const;

    bool getIsShowStaticGrid() const;
    void setIsShowStaticGrid(bool isShowStaticGrid);

    QColor getStaticTextColor() const;
    void setStaticTextColor(const QColor& staticTextColor);

    QFont getStaticFont() const;
    void setStaticFont(const QFont& staticFont);

    GridStyle getStaticGridStyle() const;
    void setStaticGridStyle(const GridStyle& staticGridStyle);

    GridDensity getStaticGridDensity() const;
    void setStaticGridDensity(const GridDensity& staticGridDensity);

    void setStaticGrid(bool isResetRange = false);

protected:
    void updateDataForDataPairsByTime(double secs) override;

    void updateGraphByDataPair(DataPair* data, double curSecs) override;

    void resizeEvent(QResizeEvent* event) override;
private slots:
    void onPlotMouseMove(QMouseEvent* event);

private:
    void initPlot();
    // 删除历史事件标签
    void clearEventText();
    void clearHistoryLines();
    // 刷新时间线事件
    void updateTimelineGraph() override;
    // 根据当前背景分段信息刷新背景颜色
    void updateBackgroundColorSeg();
    // 刷新marker
    void updateMarkers(double currentSeconds);
    // 刷新连线
    void updateConnectionLines();
    // 刷新滚动范围
    void updateScrollAxis();
    // 获取所有启用的数据对的X当前值的平均值和Y的平均值
    QPointF getCurrentAverageXYValue(bool& xNeedScroll, bool& yNeedScroll);

private:
    QHash<QString, QPair<QVector<double>, QVector<double>>> m_dataHash;
    // <uuid,graph>
    QMap<QString, DrawComponents> m_mapScatter;
    // 事件标签
    QList<EventComponents> m_eventDrawList;
    // 是否为时间线模式
    bool m_isTimeLine = false;
    QCPItemRect* m_timelineNowRect = nullptr;
    QCPItemText* m_timelineNowText = nullptr;
    QCPGraph* m_timelineGraph = nullptr;
    // GOG
    QList<QCPGraph*> m_gogGraphList;
    QList<QCPItemEllipse*> m_gogEllipseList;

    // 背景分段信息
    QMap<double, BackgroundLimitSeg> m_bkgLimitSegMap;
    QList<QCPAbstractItem*> m_backSegRectList;
    // markers
    QHash<QString, PlotMarker> m_plotMarkers;
    QHash<QString, EventComponents> m_plotMarkerItems;
    // 是否初始化过上下限
    bool m_isInitCoorRange = false;
    // 连接
    // <setting uuid,setting>
    QHash<QString, ConnectionSetting> m_conHash;
    QList<QCPItemLine*> m_connectionLines;
    //<data pari uuid,last value point>
    QHash<QString, QPointF> m_lastDataHash;

    /*
     * 开启某个轴的滚动设置之后，需要继续设置对应轴的Follow和Lead,
     * 然后选择开启启用具体的数据对，假如启用一对数据，那么基准值(value)就是当前数据对的值，
     * 如果选择多对，那么采取他们的平均值作为基准值，然后滚动轴的范围就是[value-follow,value+lead]
    */
    bool m_xScrollOn = false;
    double m_xLead = 0.0;
    double m_xFollow = 0.0;
    QHash<QString, bool> m_xScrollHash;
    bool m_yScrollOn = false;
    double m_yLead = 0.0;
    double m_yFollow = 0.0;
    QHash<QString, bool> m_yScrollHash;

    QSharedPointer<QCPAxisTicker> m_oriXAxisTicker;
    QSharedPointer<QCPAxisTicker> m_oriYAxisTicker;
    QString m_xTimeTickFormat;
    QString m_yTimeTickFormat;

    bool m_isShowStaticGrid;
    QColor m_staticTextColor;
    QFont m_staticFont;
    GridStyle m_staticGridStyle;
    GridDensity m_staticGridDensity;
    double m_tmpCoordBgnX;
    double m_tmpCoordBgnY;
    double m_tmpCoordEndX;
    double m_tmpCoordEndY;
    QCPItemPixmap* m_pixmapItem = nullptr;
};

#endif // PLOTSCATTER_H
