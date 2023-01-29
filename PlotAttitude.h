/* * @filename: PlotAttitude.h
* @brief: PlotAttitude�ؼ���
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
	QColor m_border_ColorStart;			//�߿򽥱俪ʼ��ɫ
	QColor m_border_ColorEnd;			//�߿򽥱������ɫ
	QColor m_rollColor;					//roll������ɫ
	QColor m_pitchColor;				//pitch������ɫ

	QFont m_tickLabelFont;				//�̶�����
	QFont m_axisLabelFont;					//�ı�����

	double m_pitchValue;				//��ת�Ƕ�
	double m_rollValue;					//����ֵ
	int    m_horzGrids;					//roll�������
	double m_coordBgn_x;				//roll��ʼ�Ƕ�
	double m_coordEnd_x;				//roll�����Ƕ�
	int	   m_decision_roll;				//rollС���㾫��

	int    m_vertGrids;					//pitch�������
	double m_coordBgn_y;				//pitch��ʼ�Ƕ�
	double m_coordEnd_y;				//pitch�����Ƕ�
	int	   m_decision_pitch;			//pitchС���㾫��

	double m_topPadding;				//��ͼ���-top
	double m_bottomPadding;				//��ͼ���-bottom
	double m_leftPadding;				//��ͼ���-left
	double m_rightPadding;				//��ͼ���-right

	int	   m_tickRadiusPercentage;		//�̶Ȱ뾶�ٷֱ�
	int    m_textPercentage;			//�ı��뾶�ٷֱ�
	int    m_dialPercentage;			//���̰뾶�ٷֱ�

	QList<double> m_xValueList;
	QList<double> m_yValueList;

public:
	static int m_instanceCount;			//ʵ�����
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
	
	//���ñ߿򽥱���ɫ
	void slot_setBorderColorStart(const QColor &borderOutColorStart);
	void slot_setBorderColorEnd(const QColor &borderOutColorEnd);

	//���ñ���ɫ
	void setGridFillColor(QColor bgColor);

	//���ÿ̶ȳ���ɫ
	void setRollColor(const QColor &color);
	void setPitchColor(const QColor &color);

	//���ÿ̶�����
	void setTickLabelFont(QFont &font);
	void setTickLabelFontSize(int size);

	//�����ı�����
	void setAxisLabelFont(QFont &font);
	void setAxisLabelFontSize(int size);

	//������ת�Ƕ�ֵ
	void slot_setPitchValue(double pitchValue);

	//����ǰ����תֵ
	void slot_setRollValue(double rollValue);

	//���յ�ǰʱ��
	void slot_getCurrentSeconds(double);
};

