#ifndef FREE_WIDGET_WRAPER_H_
#define FREE_WIDGET_WRAPER_H_

/**
 * 无边框窗体类
 * 1. 可以指定需要无边框的widget。
 * 2. 边框四周八个方位都可以自由拉伸。
 * 3. 可设置对应位置的边距，以便识别更大区域。
 * 4. 可设置是否允许拖动。
 * 5. 可设置是否允许拉伸。
 */

#include <QWidget>

// 鼠标操作模式，每种模式对应一种鼠标控制行为，属于互斥行为
enum class MouseMode : uint8_t
{
    SelectPlot = 0,
    Pan,
    CenterPlot,
    Zoom,
    BoxZoom,
    MeasureDistance,
    CreatePlot,
    MovePlot

};

class PlotItemBase;
class FreeWidgetWraper : public QObject
{
    Q_OBJECT
public:
    explicit FreeWidgetWraper(QObject* parent = nullptr);
    ~FreeWidgetWraper();

    MouseMode mouseMode() const;
    void setMouseMode(const MouseMode& mouseMode);

protected:
    bool eventFilter(QObject* watched, QEvent* event);

private:
    // 事件类型+鼠标模式的各个处理函数
    void handleResize();
    void handleMouseButtonPressWithCenterPlot();
    void handleMouseButtonPressWithBoxZoom();

private:
    //边距+可移动+可拉伸
    int padding = 8;
    bool m_moveEnable = false;
    bool m_resizeEnable = false;

    // 绘图控件
    PlotItemBase* m_pBindWidget = nullptr;

    //鼠标是否按下+按下坐标+按下时窗体区域
    bool mousePressed = false;
    QPoint mousePoint{0, 0};
    QRect mouseRect{0, 0, 0, 0};

    //鼠标是否按下某个区域+按下区域的大小
    //依次为 左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
    QList<bool> pressedArea;
    QList<QRect> pressedRect;

    //记录是否最小化
    bool isMin;

    MouseMode m_mouseMode = MouseMode::SelectPlot;

public:
    //设置边距
    void setPadding(int padding);
    //设置是否可拖动+拉伸
    void setMoveEnable(bool m_moveEnable);
    void setResizeEnable(bool m_resizeEnable);
    //修复部分控件不能自动识别 MouseButtonRelease 的BUG
    void setMousePressed(bool mousePressed);

    // 绑定所有想要过滤事件的绘图控件
    void bindWidget(PlotItemBase* widget);
    // 设置当前需要处理的绘图控件，避免其他没选中的控件也可以进行操作
    void setCurHandlePlot(PlotItemBase* widget);
public Q_SLOTS:
    //接受主窗口鼠标模式切换信号，设置当前鼠标模式
    void onMouseModeChanged(MouseMode mode);

signals:
    void sgnMouseEventDone(PlotItemBase*);
};

#endif // _FREE_WIDGET_WRAPER_H_
