#ifndef TABDRAWWIDGET_H
#define TABDRAWWIDGET_H

#include <QLabel>
#include <QLine>
#include <QMap>
#include <QWidget>

class QRubberBand;
enum class MouseMode : uint8_t;
enum PlotType : int32_t;
class PlotItemBase;
class TabDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabDrawWidget(QWidget* parent = nullptr);

    // MovePlot模式下有三种细分操作
    enum MovePlotMode
    {
        MultipleSelect = 0,
        SingleSelect,
        Move,
        Resize
    };

    static MouseMode mouseMode();
    static void setMouseMode(const MouseMode& mouseMode);

    QCursor getCurCursor() const;
    void setCurCursor(const QCursor& curCursor);

    static QList<PlotItemBase*> getCurSelectedPlots();
    static void setCurSelectedPlots(const QList<PlotItemBase*>& curSelectedPlots);
    static void removeUselessPlot(PlotItemBase* plot);
    // 调整图表的z轴顺序
    void bringToTop();
    void sendToBottom();
    void bringForward();
    void sendBackward();
    // 将所有图表缩放到同一大小水平排开
    void horizonAlign();
    // 将所有图表缩放到同一大小垂直排开
    void verticalAlign();
    // 查找当前tab页上所有的plot控件列表
    QList<PlotItemBase*> findAllPlots();

    static bool getIsLockingEdit();
    static void setIsLockingEdit(bool isLockingEdit);

    static bool getIsLockingStack();
    static void setIsLockingStack(bool isLockingStack);

    void updateSelectedPlotsBorderVisible();
    void updatePlotsBorderVisible(bool visible);

    void
    updateLabels(const QString& className, const QColor& color, int32_t fontSize, bool isShowIcon);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;

    void resizeEvent(QResizeEvent*) override;

signals:
    void mouseRelease(QPoint point);
    void selectedPlotChanged(const QList<PlotItemBase*>& plots);

    void boxZoomed(const QRect& rec);
    // 创建图表
    void createPlot(PlotType type, const QRect& rec);
    // 等比缩放
    void zoomed(double factor);
public slots:

private:
    PlotItemBase* findPlotByMousePos(const QPoint& mouseEventPoint);

    void handleMouseMoveWithMovePlot(int offsetX, int offsetY);
    void handleMouseReleaseWithCenterPlot(const QPoint& centerPoint);
    void handleMouseReleaseWithSelectPlot(const QPoint& mouseEventPoint);
    void handleBoxZoom(const QRect& rect);
    void handleZoomInOut(double factor);
    void handleMouseMoveWithPan(int offsetX, int offsetY);

    void initLabels();

private:
    // 框选缩放辅助矩形框
    QRubberBand* m_pRubberBand;
    QPoint m_originPoint;
    static MouseMode m_mouseMode;
    MovePlotMode m_movePlotMode;

    /*
     * 当前选中绘图控件，可能多选
     * 此控件实例不唯一，会存在多个画板，需要在多个画板内共享当前选中控件
     */
    static QList<PlotItemBase*> m_curSelectedPlots;
    /*
     * 大部分操作都是对单一图表控件进行处理，不需要对整个多选列表
     * 进行循环操作，除了整体移动这些东西。
     * 此变量不提供给外部访问，只作为内部鼠标事件之间传递的中间变量，
     * 列表的选中控件还是以list为主，避免产生歧义
    */
    static PlotItemBase* m_pCurWidget;
    // 每种模式有特定光标模式，模式内部涉及到光标切换的，后续需要恢复成这种默认模式
    QCursor m_curCursor;
    //  编辑锁定是否打开
    static bool m_isLockingEdit;
    // stack order锁定是否打开
    static bool m_isLockingStack;
    // 平铺展开时固定缩放大小
    const QSize m_tileSize{400, 400};
    // 顶部文字
    QLabel* m_topLabel = nullptr;
    // 底部文字
    QLabel* m_bottomLabel = nullptr;
    // 右上角图标
    QLabel* m_rightTopLabel = nullptr;
};

#endif // TABDRAWWIDGET_H
