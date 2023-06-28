/* * @filename: PlotLight.h
* @brief: PlotLight控件类
* @author: zhangdingwen
* @create time: 2022-11-1
*  */

#ifndef PLOT_LIGHT_H
#define PLOT_LIGHT_H

#include "PlotItemBase.h"
#include <QColor>
#include <QMap>

#include <tuple>

class PlotLight : public PlotItemBase
{
	Q_OBJECT
public:
	PlotLight(QWidget* parent = Q_NULLPTR);
    ~PlotLight() override;

    static int m_instanceCount; //实体个数

    PlotType plotType() const override
    {
        return Type_PlotLight;
    }
    struct DrawComponents
    {
        QPointer<QCPItemEllipse> ellipseGraph;
        QPointer<QCPItemRect> rectGraph;
        QPointer<QCPItemText> text;

        DrawComponents()
        {
            ellipseGraph = nullptr;
            rectGraph = nullptr;
            text = nullptr;
        }
    };

    QString getLightType() const;
    void setLightType(const QString& lightType);

    int getLightWidth() const;
    void setLightWidth(int lightWidth);

    int getLightHeight() const;
    void setLightHeight(int lightHeight);

    int getLightTextYPos() const;
    void setLightTextYPos(int lightTextYPos);

    int getLightOutlineWidth() const;
    void setLightOutlineWidth(int lightOutlineWidth);

    QColor getLightOutlineColor() const;
    void setLightOutlineColor(const QColor& lightOutlineColor);

public slots:
    void onLightConstraintUpdate(
        const QList<std::tuple<int32_t, QString, QString, double, QString>>& constraintList);

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data) override;
    void delPlotPairData(const QString& uuid) override;

    // 通过约束信息解析数据，生成最终需要的绘制数据
    void processDataByConstraints();
    void initPlot();

    // 清理上次绘制内容
    void clearPlotContent();
    // 根据约束条件返回指定数据的颜色信息
    QColor getColorByDataPairWithCon(int32_t id, const QString& attr, double value);

    // 动态计算圆圈适合的半径
    void calculateRaidus();

    void clearLineList();

private:
    // 原始的属性-值约束信息，由添加数据对时的界面进行编辑，和数据对是独立的两对数据 entityid,attr,condition,threshold,colorName
    QList<std::tuple<int32_t, QString, QString, double, QString>> m_constraintList;
    // 缓存数据 desc,entityid,entityName,attr,value
    QHash<QString, std::tuple<QString, int32_t, QString, QString, double>> m_dataHash;
    // 处理后的用于直接绘制的数据 desc,color
    QHash<QString, QPair<QString, QColor>> m_drawDataHash;
    // 绘制的元素指针列表，用于清除之后重绘 <uuid-draw>
    QHash<QString, DrawComponents> m_drawItemHash;
    //    QList<QCPAbstractItem*> m_drawItemList;
    // 状态灯和文本之间的距离间隔
    uint32_t m_verPadding = 100;
    // 每一行数据之间的水平间隔
    uint32_t m_horPadding = 20;
    // 外边框与内容之间的内间距
    uint32_t m_innerPadding = 10;
    // 状态灯半径
    uint32_t m_circleRadius = 100;

    QString m_lightType; // light的形状：1-矩形 2-正方形 3-椭圆 4-圆
    int m_lightWidth; //light的宽度
    int m_lightHeight; //light的高度
    int m_lightTextYPos; //light的文字Y位置
    int m_lightOutlineWidth; //light灯的外边框线宽度
    QColor m_lightOutlineColor; //light灯的外边框线颜色
    QList<QCPItemLine*> m_horLineList; //水平网格线
    QList<QCPItemLine*> m_verLineList; //竖直网格线
    QList<QCPItemRect*> m_outRectList; //竖直网格线
};
#endif // _PLOT_LIGHT_H_
