#pragma once
/* * @filename: PlotLight.h
* @brief: PlotLight控件类
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
	//void updateItems();
	void drawRect(double itemIndex, bool bHorizontal, double itemLength, double leftBoundary, double rightBoundary, QColor color);
public:
	static int m_instanceCount;			//实体个数
public slots:
	

private:

	bool m_bHorizontal;
	bool m_started;

	QTimer* m_timer;


	QColor m_gridColor;
	QColor m_gridFillColor;
	QColor m_axisColor;


	QStringList m_entityTypeList;
	QStringList m_entityAttrList;

	int m_horiGridNum;
	int m_verGridNum;



	int m_leftPadding;
	int m_rightPadding;
	int m_interPadding;

	int m_currTimeIndex;


	QMap<QString, QMap<int, QColor>>m_thresholdColorMap;
	QColor m_defaultColor;

protected:
	virtual void paintEvent(QPaintEvent* event);


};
#endif // _PLOT_LIGHT_H_ 