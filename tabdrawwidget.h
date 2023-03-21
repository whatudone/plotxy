#ifndef TABDRAWWIDGET_H
#define TABDRAWWIDGET_H

#include <QWidget>
class QRubberBand;
enum class MouseMode : uint8_t;
class TabDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabDrawWidget(QWidget* parent = nullptr);

    MouseMode mouseMode() const;
    void setMouseMode(const MouseMode& mouseMode);

protected:
    void mouseReleaseEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);
signals:
    void mouseRelease(QPoint point);

    void boxZoomed(const QRect& rec);
public slots:
    void onMouseModeChanged(MouseMode mode);

private:
    QRubberBand* m_pRubberBand;
    QPoint m_originPoint;
    MouseMode m_mouseMode;
};

#endif // TABDRAWWIDGET_H
