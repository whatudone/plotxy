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
    ~PlotText() override;

    static int m_instanceCount; //实体个数
    PlotType plotType() const override
    {
        return Type_PlotText;
    }

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data) override;
    void customPainting(QPainter& painter) override;
    // 绘制网格 坐标轴Label 数据
    void drawCellData(QPainter& painter);

private:
    QStringList m_entityList;
    QStringList m_attrList;
    // 二维数组的数据
    QHash<QPair<QString, QString>, double> m_dataHash;
};

#endif // _PLOT_TEXT_H_
