/* * @filename: PlotLight.h
* @brief: PlotLight控件类
* @author: zhangdingwen
* @create time: 2022-11-1
*  */

#ifndef PLOT_LIGHT_H
#define PLOT_LIGHT_H

#include "BaseData.h"
#include "PlotItemBase.h"
#include <QColor>
#include <QMap>
#include <QPaintEvent>
#include <QTimer>

class PlotLight : public PlotItemBase
{
	Q_OBJECT
public:
	PlotLight(QWidget* parent = Q_NULLPTR);
	~PlotLight();
	void onTimeout();
	void onSwitch(bool bOn);
    static int m_instanceCount; //实体个数
	//void updateItems();
    PlotType plotType() const override
    {
        return Type_PlotLight;
    }

private:
	bool m_bHorizontal;
	bool m_started;

	QColor m_defaultColor;
	QStringList m_entityTypeList;
	QStringList m_entityAttrList;
    QMap<QString, QBrush> m_brushColor;
	QList<QPair<QString, QPair<QString, double>>> m_thresholdData;
	QPair<QString, QPair<QString, double>> m_innerThresholdData;
    QPair<QString, double> m_innerInnerThresholdData;
	QBrush m_temBrush;
	QList<QString> m_entityAndAttr;
	QList<QBrush> m_brush;
	QList<double> m_valueList;
	QList<QString> m_entityName, m_attriName;
	QList<QList<double>> m_temValueList;
	QList<long double> m_lightDataList;
	QList<QString> m_lightValueList;
    // uuid-dataList
	QMap<QString, QList<long double>> m_lightMap;
	QList<QList<QString>> m_userLightData;
	int m_horiGridNum;
	int m_verGridNum;

	int m_interPadding;
	int m_currTimeIndex;
	int m_circleRadius;
	void judgeLight();
    void setCircleRadius(double&);
    void drawLight(QPainter&, double&, double&);
    void drawTitle(QPainter&, QRect&);
    // void dataPairOrder();
    // void setGridColorWidth(QColor color, uint width);
    void setGridStyle(GridStyle);

public slots:
	void slot_getLightData(QList<QList<QString>>);
	//更新后的由base 的sgn_发出
	void slot_onAddButtonClicked();

private:
    void updateDataForDataPairsByTime(double secs) override;
    void customPainting(QPainter& painter) override;
};
#endif // _PLOT_LIGHT_H_
