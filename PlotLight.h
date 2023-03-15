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
	static int m_instanceCount;			//实体个数
	//void updateItems();

protected:
	virtual void paintEvent(QPaintEvent* event);

private:
	bool m_bHorizontal;
	bool m_started;

	QColor m_defaultColor;
	QStringList m_entityTypeList;
	QStringList m_entityAttrList;
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
	int m_circleRadius;
	void judgeLight();
	void setCircleRadius(double &);
	void drawLight(QPainter &,double &,double &);
	void drawTitle(QPainter &,QRect&);
	//virtual void dataPairOrder();
	//virtual void setGridColorWidth(QColor color, uint width);
	virtual void setGridStyle(GridStyle);
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getGridColor() { return m_gridColor; }
	virtual Qt::PenStyle getGridStyle() { return m_gridStyle; }
public slots:
	void slot_getLightData(QList<QList<QString>>);
    void onGetCurrentSeconds(double secs) override;
	//更新后的由base 的sgn_发出
	void slot_onAddButtonClicked();
};
#endif // _PLOT_LIGHT_H_ 
