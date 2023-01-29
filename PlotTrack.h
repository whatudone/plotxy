#ifndef  _PLOT_TRACK_H_
#define  _PLOT_TRACK_H_

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QMap>
#include <QString>

class PlotTrack : public PlotItemBase
{
    Q_OBJECT
public:
    enum TrackStatus {
        Available,      // ��Ч�ҿ���
        Invalid,        // ��Ч����
        Unavailable     // ��Ч������
    };

    PlotTrack(QWidget *parent = Q_NULLPTR);
    ~PlotTrack();

    void setLeftPadding(int);
    void setRightPadding(int);
    void setInterPadding(int);

    void drawRect(int itemIndex, QList<QColor> dataList);
public:
    static int m_instanceCount;         //ʵ�����

public slots:
    void onUpdateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
    void onGetCurrentSeconds(double secs);

protected:
    virtual void paintEvent(QPaintEvent *event);
    void getDataInfo(double secs);
    void updateData(int itemIndex, QString entityType, double secs); //ʵ�ֺ��Ļ����߼�

private:
    QLine   m_xAxis;
    QLine   m_yAxis;

	double m_topPadding;				//��ͼ���-top
	double m_bottomPadding;				//��ͼ���-bottom
	double m_leftPadding;				//��ͼ���-left
	double m_rightPadding;				//��ͼ���-right
    int m_interPadding;

    QMap<QString, QMap<int, QColor>> m_thresholdColorMap;    //key:entityType+entityAttr, threshold,QColor
    QColor m_defaultColor;

    QColor m_gridColor;
    QColor m_gridFillColor;
    QColor m_axisColor;
	QFont  m_axisFont;					//�����������

    int m_horiGridNum;
    int m_verGridNum;
    double m_seconds;
    int m_itemCnt;

	QString m_title;
	QColor	m_titleColor;				//������ɫ
	QFont	m_titleFont;				//��������
	bool	m_titleShow;				//�����Ƿ���ʾ
};

#endif // _PLOT_TRACK_H_
