/* * @filename: PlotAttitude.h
* @brief: PlotAttitude控件类
* @author: wangzhen
* @create time: 2022-10-10
*  */

#pragma once
#include "PlotItemBase.h"
#include <QTimer>
class PlotAttitude :
	public PlotItemBase
{
	Q_OBJECT
public:
	PlotAttitude(QWidget* parent = Q_NULLPTR);
	~PlotAttitude();

protected:
	virtual void paintEvent(QPaintEvent* event);
public:
	void drawTitle(QPainter *painter, int radius);
	void drawBorder(QPainter* painted, int radius);
	void drawBg(QPainter *painter, int radius);
	void drawScale_roll(QPainter *painter, int radius);
	void drawScale_pitch(QPainter *painter, int radius);
	void drawLine_roll(QPainter *painter, int radius);
	void drawLine_pitch(QPainter *painter, int radius);
	void drawText_roll(QPainter *painter, int radius);
	void drawText_pitch(QPainter *painter, int radius);
private:
	QColor m_border_ColorStart;			//边框渐变开始颜色
	QColor m_border_ColorEnd;			//边框渐变结束颜色
	QColor m_rollColor;					//roll主题颜色
	QColor m_pitchColor;				//pitch主题颜色


	double m_pitchValue;				//旋转角度
	double m_rollValue;					//滚动值

	int	   m_decision_roll;				//roll小数点精度


	int	   m_decision_pitch;			//pitch小数点精度

	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right

	int	   m_tickRadiusPercentage;		//刻度半径百分比
	int    m_textPercentage;			//文本半径百分比
	int    m_dialPercentage;			//表盘半径百分比

	QList<double> m_xValueList;
	QList<double> m_yValueList;

public:
	static int m_instanceCount;			//实体个数
public:
	QColor getBorderOutColorStart() const;
	QColor getBorderOutColorEnd()   const;
	QColor getRollColor()			const;
	QColor getPitchColor()			const;

	float getPitchValue()           const;
	float getRollValue()            const;

	QSize sizeHint()                const;
	QSize minimumSizeHint()         const;

	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);

	virtual uint getHorzGrids() { return m_horzGrids; }
	virtual uint getVertGrids() { return m_vertGrids; }
	virtual uint getAxisWidth() { return m_axisWidth; }
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getAxisColor() { return m_axisColor; }
	virtual QColor getGridColor() { return m_gridColor; }
	virtual QFont getTickLabelFont() { return m_tickLabelFont; }
	virtual int getTickLabelFontSize() { return m_tickLabelFontSize; }

	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitle(QString& title);
	virtual void setTitleVisible(bool show);
	virtual void setTitleColor(QColor &titleColor);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont &font);
	virtual void setTitleFontSize(int size);
	virtual bool unitsShowX() { return m_showUnits_x; }
	virtual bool unitsShowY() { return m_showUnits_y; }
	virtual QString getUnitsX() { return m_units_x; }
	virtual QString getUnitsY() { return m_units_y; }
	virtual bool getTitleVisible() { return m_titleVisible; }
	virtual QString getTitle() { return m_title; }
	virtual QColor getTitleColor() { return m_titleColor; }
	virtual QColor getTitleFillColor() { return m_titleFillColor; }
	virtual QFont getTitleFont() { return m_titleFont; }
	virtual int getTitleFontSize() { return m_titleFontSize; }

	int getTickRadiusPercentage() { return m_tickRadiusPercentage; }
	int getTextPercentage() { return m_textPercentage; }
	int getDialPercentage() { return m_dialPercentage; }

	void updateItems();

public slots:
	//
	void setTickRadiusPercentage(int);
	void setTextPercentage(int);
	void setDialPercentage(int);
	
	//设置边框渐变颜色
	void slot_setBorderColorStart(const QColor &borderOutColorStart);
	void slot_setBorderColorEnd(const QColor &borderOutColorEnd);

	//设置背景色
	void setGridFillColor(QColor bgColor);

	//设置刻度尺颜色
	void setRollColor(const QColor &color);
	void setPitchColor(const QColor &color);

	//设置刻度字体
	void setTickLabelFont(QFont &font);
	void setTickLabelFontSize(int size);

	//设置文本字体
	void setAxisLabelFont(QFont &font);
	void setAxisLabelFontSize(int size);

	//设置旋转角度值
	void slot_setPitchValue(double pitchValue);

	//设置前进旋转值
	void slot_setRollValue(double rollValue);

	//接收当前时间
	void onGetCurrentSeconds(double);
};

