#ifndef PLOTDIAL_H
#define PLOTDIAL_H

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QMap>
#include <QString>

class PlotDial : public PlotItemBase
{
    Q_OBJECT

public:
    PlotDial(QWidget *parent = Q_NULLPTR);
    ~PlotDial();

    //更新指针
    void updatePointer(double secs);
    static int m_instanceCount;         //实体个数
public slots:
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event);

private:
    bool m_bThinStyle;

	QString m_title;
	QColor	m_titleColor;				//标题颜色
	QFont	m_titleFont;				//标题字体
	bool	m_titleShow;				//标题是否显示

	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	QFont	m_axisFont;					//坐标标题字体

    double m_seconds;

    QColor m_dialColor;
    QColor m_pointColor;

    QPoint m_centerPoint;
    int m_circleRadius;
    int m_startAngle;
    int m_endAngle;
};

#endif // PLOTDIAL_H
