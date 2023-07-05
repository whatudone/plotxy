/* * @filename: PlotAttitude.h
* @brief: PlotAttitude控件类
* @author: wangzhen
* @create time: 2022-10-10
*  */

#pragma once
#include "PlotItemBase.h"
#include <QTimer>
class PlotAttitude : public PlotItemBase
{
	Q_OBJECT
public:
	PlotAttitude(QWidget* parent = Q_NULLPTR);
    ~PlotAttitude() override;

public:
	void drawBorder(QPainter* painted, int radius);
    void drawBg(QPainter* painter, int radius);
    void drawScale_roll(QPainter* painter, int radius);
    void drawScale_pitch(QPainter* painter, int radius);
    void drawRollIndicator(QPainter* painter, int radius);
    void drawPitchIndicator(QPainter* painter, int radius);
    void drawText_roll(QPainter* painter, int radius);
    void drawText_pitch(QPainter* painter, int radius);

private:
    QColor m_border_ColorStart; //边框渐变开始颜色
    QColor m_border_ColorEnd; //边框渐变结束颜色
    QColor m_rollColor; //roll主题颜色,和基类中的颜色不冲突
    QColor m_pitchColor; //pitch主题颜色,和基类中的颜色不冲突

    double m_pitchValue; //旋转角度
    double m_rollValue; //滚动值

    int m_tickRadiusPercentage; //刻度半径百分比
    int m_textPercentage; //文本半径百分比
    int m_dialPercentage; //表盘半径百分比

    QVector<double> m_xValueList;
    QVector<double> m_yValueList;

public:
    static int m_instanceCount; //实体个数
public:
	QColor getBorderOutColorStart() const;
    QColor getBorderOutColorEnd() const;
    QColor getRollColor() const;
    QColor getPitchColor() const;

    void setRollColor(const QColor& color);
    void setPitchColor(const QColor& color);

    float getPitchValue() const;
    float getRollValue() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    void setHorzGrids(uint count);
    void setVertGrids(uint count);
    void setAxisColorWidth(QColor color, uint width);
    void setGridColorWidth(QColor color, uint width);

    void setUnitsShowX(bool on);
    void setUnitsShowY(bool on);
    void setUnitsX(const QString& units);
    void setUnitsY(const QString& units);
    void setTitle(QString& title);
    void setTitleVisible(bool show);
    void setTitleColor(QColor& titleColor);
    void setTitleFillColor(QColor& color);
    void setTitleFont(QFont& font);
    void setTitleFontSize(int size);

    int getTickRadiusPercentage()
    {
        return m_tickRadiusPercentage;
    }
    int getTextPercentage()
    {
        return m_textPercentage;
    }
    int getDialPercentage()
    {
        return m_dialPercentage;
    }

    PlotType plotType() const override
    {
        return Type_PlotAttitude;
    }

	//
	void setTickRadiusPercentage(int);
	void setTextPercentage(int);
	void setDialPercentage(int);

	//设置边框渐变颜色
    void slot_setBorderColorStart(const QColor& borderOutColorStart);
    void slot_setBorderColorEnd(const QColor& borderOutColorEnd);

	//设置背景色
	void setGridFillColor(QColor bgColor);

	//设置刻度字体
    void setTickLabelFont(QFont& font);
	void setTickLabelFontSize(int size);

	//设置文本字体
    void setxAxisLabelFont(QFont& font);
    void setxAxisLabelFontSize(int size);
    void setyAxisLabelFont(QFont& font);
    void setyAxisLabelFontSize(int size);

private:
    void updateDataForDataPairsByTime(double secs) override;
    void updateGraphByDataPair(DataPair* data, double curSecs) override;
    void customPainting(QPainter& painter) override;
};
