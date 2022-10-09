/* * @filename: PlotBar.h
* @brief: PlotBar控件类
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef  _PLOT_BAR_H_
#define  _PLOT_BAR_H_ 

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QMap>
#include <QString>

class PlotBar :public PlotItemBase
{
public:
    PlotBar(QWidget* parent = Q_NULLPTR);
    ~PlotBar();
       
    void setPlotItemExtentionDirec(bool);

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);

    void addPlotDataPair(const QString&, const QString&);

    void updateItems(); //实现核心绘制逻辑

    void drawRect(int itemIndex,bool bHorizontal,int itemLength,int leftBoundary, int rightBoundary, QColor color);

public slots:
    void onSwitch(bool);
    void onTimeout();
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);

protected:
    virtual void paintEvent(QPaintEvent* event);


private:
    QTimer* m_timer;

    bool m_bHorizontal;

    QLine   m_xAxis;
    QLine   m_yAxis; 

    int m_leftPadding;
    int m_rightPadding;
    int m_interPadding;

    int m_currTimeIndex;

    //QMap<QString,QString> m_plotDataPair;   //注：实体类型，实体属性   //这里不能用map
    QStringList m_entityTypeList;
    QStringList m_entityAttrList;

    QMap<QString,QMap<int, QColor>> m_thresholdColorMap;    //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    QColor m_gridColor;
    QColor m_gridFillColor;
    QColor m_axisColor;

    int m_horiGridNum;
    int m_verGridNum;

    bool m_started;
};

#endif // _PLOT_BAR_H_ 