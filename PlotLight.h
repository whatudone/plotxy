#pragma once
/* * @filename: PlotLight.h
* @brief: PlotLight�ؼ���
* @author: zhangdingwen
* @create time: 2022-11-1
*  */

#ifndef  _PLOT_LIGHT_H_
#define  _PLOT_LIGHT_H_ 

#include "PlotItemBase.h"
#include "BaseData.h"
#include <QTimer>
#include <QColor>
#include <QMap>
#include <QPaintEvent>

class PlotLight :public PlotItemBase
{
	Q_OBJECT
public:
	PlotLight(QWidget* parent = Q_NULLPTR);
	~PlotLight();
	void onTimeout();
	void onSwitch(bool bOn);
	static int m_instanceCount;			//ʵ�����
	//void updateItems();

protected:
	virtual void paintEvent(QPaintEvent* event);

private:

	bool m_bHorizontal;
	bool m_started;

	QTimer* m_timer;

	QColor m_gridColor;
	QColor m_gridFillColor;
	QColor m_axisColor;

	QStringList m_entityTypeList;
	QStringList m_entityAttrList;


	QColor m_defaultColor;
	QMap<QString,QBrush> m_brushColor;

	QList<QPair<QString, QPair<QString, double>>> m_thresholdData;
	QPair<QString, QPair<QString, double>> m_innerThresholdData;
	QPair<QString,double> m_innerInnerThresholdData;

	QBrush m_temBrush;

	QList<QString> m_entityAndAttr;
	QList<QBrush> m_brush;
	QList<double> m_valueList;
	QList<QString> m_entityName, m_attriName;
	QList<QList<double>> m_temValueList;
	QList<long double> m_lightDataList;
	QList<QString> m_lightValueList;
	QMap<QString, QList<long double>> m_lightMap;
	QList<QList<QString>> m_userLightData;

	int m_horiGridNum;
	int m_verGridNum;
	int m_leftPadding;
	int m_rightPadding;
	int m_interPadding;
	int m_currTimeIndex;

	void judgeLight();
	void drawLight(QPainter &,double &);

public slots:
	void slot_getLightData(QList<QList<QString>>);
	void slot_getCurrentSeconds(double secs);
	//���º����base ��sgn_����
	void slot_onAddButtonClicked();



};
#endif // _PLOT_LIGHT_H_ 