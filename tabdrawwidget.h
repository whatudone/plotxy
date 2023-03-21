#ifndef TABDRAWWIDGET_H
#define TABDRAWWIDGET_H

#include <QWidget>

class TabDrawWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TabDrawWidget(QWidget* parent = nullptr);

protected:
    void mouseReleaseEvent(QMouseEvent* event);

signals:
    void mouseRelease(QPoint point);
public slots:
};

#endif // TABDRAWWIDGET_H
