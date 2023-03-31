/* * @filename: PlotText.h
* @brief: PlotText控件类
* @author: zhangdingwen
* @create time: 2022-10-10
*  */

#ifndef PLOT_TEXT_H_
#define PLOT_TEXT_H_

#include "BaseData.h"
#include "PlotItemBase.h"
#include <QColor>
#include <QDebug>
#include <QMap>
#include <QTimer>
#include <QVector>

class PlotText : public PlotItemBase
{
	Q_OBJECT
public:
	PlotText(QWidget* parent = Q_NULLPTR);
	~PlotText();
	//void onTimeout();

	bool m_flag;
    static int m_instanceCount; //实体个数
    PlotType plotType() const override
    {
        return Type_PlotText;
    }

private:
    void updateDataForDataPairsByTime(double secs) override;

protected:
    void paintEvent(QPaintEvent* event);

private:
    void setGridColorWidth(QColor color, uint width);
    void setGridStyle(GridStyle);

	bool m_bHorizontal;
    bool m_started;
	QStringList m_xColumnList;
	QStringList m_yColumnList;
	QStringList m_entityTypeList;
	QStringList m_entityAttrList;
	int m_horiGridNum;
    int m_verGridNum;
	int m_leftPadding;
	int m_rightPadding;
	int m_interPadding;
    int m_currTimeIndex;
    QMap<QString, QMap<int, QColor>> m_thresholdColorMap;
	QVector<QList<double>> m_valueListVector;
	QColor m_defaultColor;
	QList<double> m_valueList;
	QList<QList<double>> m_temValueList;
	QList<QString> m_entityName, m_attriName;
	double m_secValue;
    void drawTitleText(QPainter& painter, QRect& rect);
    void drawXYTitle(QPainter& painter,
                     int& horiGridWidth,
                     int& verGridWidth,
                     const QVector<DataPair*>& dataVector,
                     double& as);
    void drawNMCell(QPainter& painter,
                    QSet<QString>& xset,
                    QSet<QString>& yset,
                    const QVector<DataPair*>& dataVector,
                    int& horiGridWidth,
                    int& verGridWidth,
                    double& as);
	void drawData(QSet<QString>& xset, QSet<QString>& yset, int& horiGridWidth, int& verGridWidth);
};

#endif // _PLOT_TEXT_H_
