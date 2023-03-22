#ifndef TABDRAWWIDGET_H
#define TABDRAWWIDGET_H

#include <QLine>
#include <QWidget>
class QRubberBand;
enum class MouseMode : uint8_t;
enum PlotType : int32_t;
class TabDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabDrawWidget(QWidget* parent = nullptr);

    MouseMode mouseMode() const;
    void setMouseMode(const MouseMode& mouseMode);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
signals:
    void mouseRelease(QPoint point);

    void boxZoomed(const QRect& rec);
    // 创建图表
    void createPlot(PlotType type, const QRect& rec);
public slots:
    void onMouseModeChanged(MouseMode mode);

private:
    // 框选缩放辅助矩形框
    QRubberBand* m_pRubberBand;
    QPoint m_originPoint;
    MouseMode m_mouseMode;
    // 测距辅助线段
    QLine m_measureLine;
};

#endif // TABDRAWWIDGET_H
