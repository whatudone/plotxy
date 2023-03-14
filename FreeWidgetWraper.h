#ifndef _FREE_WIDGET_WRAPER_H_
#define FREE_WIDGET_WRAPER_H_

/**
 * 无边框窗体类
 * 1. 可以指定需要无边框的widget。
 * 2. 边框四周八个方位都可以自由拉伸。
 * 3. 可设置对应位置的边距，以便识别更大区域。
 * 4. 可设置是否允许拖动。
 * 5. 可设置是否允许拉伸。
 */

#include <QtWidgets/QWidget>

// 鼠标操作模式，每种模式对应一种鼠标控制行为，属于互斥行为
enum class MouseMode
{
    SelectPlot = 0,
    Pan,
    CenterPlot,
    BoxZoom,
    MeasureDistance,
    CreatePlot,
    MovePlot

};

class PlotBase;
class FreeWidgetWraper : public QObject
{
    Q_OBJECT
public:
    explicit FreeWidgetWraper(QObject *parent = nullptr);
    ~FreeWidgetWraper();

    MouseMode mouseMode() const;
    void setMouseMode(const MouseMode &mouseMode);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    //边距+可移动+可拉伸
    int padding;
    bool moveEnable;
    bool resizeEnable;

    //无边框窗体
    QWidget *widget= nullptr;

    //鼠标是否按下+按下坐标+按下时窗体区域
    bool mousePressed;
    QPoint mousePoint;
    QRect mouseRect;

    //鼠标是否按下某个区域+按下区域的大小
    //依次为 左侧+右侧+上侧+下侧+左上侧+右上侧+左下侧+右下侧
    QList<bool> pressedArea;
    QList<QRect> pressedRect;

    //记录是否最小化
    bool isMin;
    //存储窗体默认的属性
    Qt::WindowFlags flags;

    MouseMode m_mouseMode = MouseMode::SelectPlot;

public Q_SLOTS:
    //设置边距
    void setPadding(int padding);
    //设置是否可拖动+拉伸
    void setMoveEnable(bool moveEnable);
    void setResizeEnable(bool resizeEnable);
    //修复部分控件不能自动识别 MouseButtonRelease 的BUG
    void setMousePressed(bool mousePressed);

    //设置要无边框的窗体
    void setWidget(QWidget*widget);

signals:
    void sgnMouseEventDone(QWidget*);
};

#endif // _FREE_WIDGET_WRAPER_H_
