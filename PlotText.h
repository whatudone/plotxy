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

    bool isRowGridVisible() const;
    void setIsRowGridVisible(bool isRowGridVisible);

    bool isColGridVisible() const;
    void setIsColGridVisible(bool isColGridVisible);

    int textLeftOffset() const;
    void setTextLeftOffset(int textLeftOffset);

    int textRightOffset() const;
    void setTextRightOffset(int textRightOffset);

    bool isFillByRow() const;
    void setIsFillByRow(bool isFillByRow);

    int rowsNum() const;
    void setRowsNum(int rowsNum);

    int colsNum() const;
    void setColsNum(int colsNum);

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data, double curSecs) override;
    void customPainting(QPainter& painter) override;
    // 绘制网格 坐标轴Label 数据
    void drawCellData(QPainter& painter);

private:
    QList<double> m_dataList;
    bool m_isRowGridVisible = true; //是否显示行网格线
    bool m_isColGridVisible = true; //是否显示列网格线
    int m_textLeftOffset; // text和light的左右偏移
    int m_textRightOffset; // text和light的左右偏移
    bool m_isFillByRow = false; //text和light是否按行填充
    int m_rowsNum; //text和light的行数
    int m_colsNum; //text和light的列数
};

#endif // _PLOT_TEXT_H_
