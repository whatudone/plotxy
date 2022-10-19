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
	QTimer* m_timer;
	int m_currTimeIndex;
	bool m_started;


	QColor m_titleColor;				//标题颜色
	QColor m_border_ColorStart;			//边框渐变开始颜色
	QColor m_border_ColorEnd;			//边框渐变结束颜色
	QColor m_bgColor;					//背景颜色
	QColor m_scaleColor_roll;			//刻度尺颜色roll
	QColor m_scaleColor_pitch;			//刻度尺颜色pitch
	QColor m_lineColor_roll;			//线条颜色roll
	QColor m_lineColor_pitch;			//线条颜色pitch
	QColor m_textColor_roll;			//文字颜色roll
	QColor m_textColor_pitch;			//文字颜色pitch

	QFont m_titleFont;					//标题字体
	QFont m_scaleFont;					//刻度字体
	QFont m_textFont;					//文本字体

	QString m_title;					//标题
	double m_pitchValue;				//旋转角度
	double m_rollValue;					//滚动值
	int    m_coordNum_roll;				//roll坐标个数
	double m_startAngle_roll;			//roll起始角度
	double m_endAngle_roll;				//roll结束角度
	QString m_unit_roll;				//roll单位
	int		m_decision_roll;			//roll小数点精度

	int    m_coordNum_pitch;			//pitch坐标个数
	double m_startAngle_pitch;			//pitch起始角度
	double m_endAngle_pitch;			//pitch结束角度
	QString m_unit_pitch;				//pitch单位
	int		m_decision_pitch;			//pitch小数点精度

	double m_topPadding;				//绘图间隔-top
	double m_bottomPadding;				//绘图间隔-bottom
	double m_leftPadding;				//绘图间隔-left
	double m_rightPadding;				//绘图间隔-right
public:
	static int m_instanceCount;			//实体个数
public:
	QColor getBorderOutColorStart() const;
	QColor getBorderOutColorEnd()   const;
	QColor getBgColor()             const;
	QColor getScaleColor_roll()     const;
	QColor getScaleColor_pitch()    const;
	QColor getLineColor_roll()      const;
	QColor getLineColor_pitch()     const;
	QColor getTextColor_roll()      const;
	QColor getTextColor_pitch()     const;

	float getPitchValue()           const;
	float getRollValue()            const;

	QSize sizeHint()                const;
	QSize minimumSizeHint()         const;

	void updateItems();

public slots:
	//设置边框渐变颜色
	void slot_setBorderColorStart(const QColor &borderOutColorStart);
	void slot_setBorderColorEnd(const QColor &borderOutColorEnd);

	//设置背景色
	void slot_setBgColor(const QColor &bgColor);

	//设置刻度尺颜色
	void slot_setScaleColor_roll(const QColor &scaleColor);
	void slot_setScaleColor_pitch(const QColor &scaleColor);

	//设置线条颜色
	void slot_setLineColor_roll(const QColor &lineColor);
	void slot_setLineColor_pitch(const QColor &lineColor);

	//设置文字颜色
	void slot_setTextColor_roll(const QColor &textColor);
	void slot_setTextColor_pitch(const QColor &textColor);

	//设置标题颜色
	void slot_setTitleColor(const QColor &titleColor);

	//设置标题字体
	void slot_setTitleFont(const QFont &font);

	//设置刻度字体
	void slot_setScaleFont(const QFont &font);

	//设置文本字体
	void slot_setTextFont(const QFont &font);

	//设置标题
	void slot_setTitle(QString title);

	//设置旋转角度值
	void slot_setPitchValue(double pitchValue);

	//设置前进旋转值
	void slot_setRollValue(double rollValue);

	//
	void onTimerout();
	void onSwitch(bool);
};

