/* * @filename: PlotBar.h
* @brief: PlotBar�ؼ���
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

class PlotBar : public PlotItemBase
{
    Q_OBJECT
public:
    PlotBar(QWidget *parent = Q_NULLPTR);
    ~PlotBar();

    void setPlotItemExtentionDirec(bool);

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);
    void drawRect(int itemIndex, bool bHorizontal, int itemLength, int leftBoundary, int rightBoundary, QColor color);

public:
    static int m_instanceCount;         //ʵ�����

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
    void onGetCurrentSecond(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event);
    void getDataInfo(double secs);
    void updateData(int itemIndex, QString x, QString y, double secs);

private:
    bool m_bHorizontal;
	bool m_titleShow;				//�����Ƿ���ʾ

    QLine m_xAxis;
    QLine m_yAxis;

    int m_leftPadding;
    int m_rightPadding;
    int m_interPadding;
	int m_topPadding;
	int m_bottomPadding;

    int m_currTimeIndex;

    //QMap<QString,QString> m_plotDataPair;   //ע��ʵ�����ͣ�ʵ������   //���ﲻ����map
//     QStringList m_entityTypeList;
//     QStringList m_entityAttrList;

    QMap<QString, QMap<int, QColor>> m_thresholdColorMap;   //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;
	QColor m_titleColor;				//������ɫ
	QFont m_titleFont;					//��������
	QString m_title;					//����
    QColor m_gridColor;
    QColor m_gridFillColor;
    QColor m_axisColor;

    int m_horiGridNum;
    int m_verGridNum;

    double m_seconds;
};

#endif // _PLOT_BAR_H_ 
