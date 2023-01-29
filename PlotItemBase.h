/* * @filename: PlotItemBase.h
* @brief: PlotBase Plot����
* @author: zhameng
* @create time: 2022-09-21
*  */

#ifndef  _PLOT_ITEM_BASE_H_
#define  _PLOT_ITEM_BASE_H_

#include <QWidget>
#include "ui_PlotItemBase.h"
#include "DataPair.h"
#include "constdef.h"

class PlotItemBase : public QWidget
{
    Q_OBJECT

public:
    explicit PlotItemBase(QWidget* parent = 0);
    ~PlotItemBase();

public:
    void init();                        //��ʼ�������������źŲ�

    void setPosition(int x,int y);      //���ð�Χ�����Ͻ�λ��
    void setWidth(int width);           //���ÿ��
    void setHeight(int height);         //���ð�Χ�и߶�
	void setRect(QRect rect);
	void setName(const QString&);
	void setTabName(const QString&);

    QPoint currPosition();
    int currWidth();
    int currHeight();
	QRect currRect();
    QString currName();
    QString currTabName();

	//setters:
	virtual void setOuterFillColor(QColor color);
	virtual void setOutlineColor(QColor color);
	virtual void setCoordRangeX(double lower, double upper);
	virtual void setCoordRangeY(double lower, double upper);
	virtual void setHorzGrids(uint count);
	virtual void setVertGrids(uint count);
	virtual void setAxisColorWidth(QColor color, uint width);
	virtual void setGridColorWidth(QColor color, uint width);
	virtual void setGridVisible(bool enable);
	virtual void setTickLabelColor(QColor& color);
	virtual void setTickLabelFont(QFont& font);
	virtual void setTickLabelFontSize(int size);
	virtual void setGridStyle(GridStyle style);
	virtual void setGridDensity(GridDensity density);	
	virtual void setGridFillColor(QColor color);
	virtual void setUnitsShowX(bool on);
	virtual void setUnitsShowY(bool on);
	virtual void setUnitsX(const QString& units);
	virtual void setUnitsY(const QString& units);
	virtual void setTitleVisible(bool on);
	virtual void setTitle(QString& title);
	virtual void setTitleColor(QColor& color);
	virtual void setTitleFillColor(QColor& color);
	virtual void setTitleFont(QFont& font);
	virtual void setTitleFontSize(int size);
	virtual void setxAxisLabel(int label);
	virtual void setyAxisLabel(int label);
	virtual void setAxisLabelColor(QColor& color);
	virtual void setAxisLabelFont(QFont& font);
	virtual void setAxisLabelFontSize(int size);

	//getters:
	virtual QColor getOuterFillColor() { return m_outerFillColor; }
	virtual QColor getOutlineColor() { return m_outlineColor; }
	virtual void getCoordRangeX(double& lower, double& upper);
	virtual void getCoordRangeY(double& lower, double& upper);
	virtual uint getHorzGrids() { return m_horzGrids; }
	virtual uint getVertGrids() { return m_vertGrids; }
	virtual uint getAxisWidth() { return m_axisWidth; }
	virtual uint getGridWidth() { return m_gridWidth; }
	virtual QColor getAxisColor() { return m_axisColor; }
	virtual QColor getGridColor() { return m_gridColor; }
	virtual bool getGridVisible() { return m_gridVisible; }
	virtual QColor getTickLabelColor() { return m_tickLabelColor; }
	virtual QFont getTickLabelFont() { return m_tickLabelFont; }
	virtual int getTickLabelFontSize() { return m_tickLabelFontSize; }
	virtual Qt::PenStyle getGridStyle() { return m_gridStyle; }
	virtual GridDensity getGridDensity() { return m_gridDensity; }
	virtual QColor getGridFillColor() { return m_gridFillColor; }
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
	virtual QString getxAxisLabel() { return m_xAxisLabel; }
	virtual QString getyAxisLabel() { return m_yAxisLabel; }
	virtual QColor getAxisLabelColor() { return m_axisLabelColor; }
	virtual QFont getAxisLabelFont() { return m_axisLabelFont; }
	virtual int getAxisLabelFontSize() { return m_axisLabelFontSize; }

	virtual void addPlotPairData(QPair<QString, QString>);
	virtual void delPlotPairData(QPair<QString, QString>);
	virtual void updatePlotPairData(QPair<QString, QString> oldPair, QPair<QString, QString> newPair);
	QVector<DataPair*> getDataPair() { return m_dataPair; }
	void setDataPair(QVector<DataPair*> newVector);

protected:
	QVector<DataPair*> m_dataPair;
	//General
	QColor m_outerFillColor;
	QColor m_outlineColor;

	//Axis and Grid
	double  m_coordBgn_x;			//x������ʼֵ
	double  m_coordEnd_x;			//x�������ֵ
	double  m_coordBgn_y;			//y������ʼֵ
	double  m_coordEnd_y;			//y�������ֵ
	uint	m_horzGrids;			//����grid����
	uint	m_vertGrids;			//����grid����
	uint	m_axisWidth;			//��������
	uint	m_gridWidth;			//grid���
	QColor	m_axisColor;			//��������ɫ
	QColor	m_gridColor;			//grid��ɫ
	bool	m_gridVisible;			//�Ƿ���ʾgrid
	QColor	m_tickLabelColor;		//����̶�ֵ��ɫ
	QFont	m_tickLabelFont;		//����̶�ֵ����
	int		m_tickLabelFontSize;	//����̶�ֵ����ߴ�
	Qt::PenStyle m_gridStyle;		//grid���
	GridDensity m_gridDensity;		//grid�ܶ�
	QColor	m_gridFillColor;		//grid���ɫ��������ɫ

	//Text
	QString m_units_x;				//x�ᵥλ
	QString m_units_y;				//y�ᵥλ
	bool	m_showUnits_x;			//�Ƿ���ʾx�ᵥλ
	bool	m_showUnits_y;			//�Ƿ���ʾy�ᵥλ

	QString m_title;				//����
	bool	m_titleVisible;			//�Ƿ���ʾ����
	QColor	m_titleColor;			//������ɫ
	QFont	m_titleFont;			//��������
	int		m_titleFontSize;		//��������ߴ�
	QColor	m_titleFillColor;		//�������ɫ

	QString m_xAxisLabel;			//x�����
	QString m_yAxisLabel;			//y�����
	QColor  m_axisLabelColor;		//�����������ɫ
	QFont   m_axisLabelFont;		//�������������
	int		m_axisLabelFontSize;	//�������������ߴ�

public slots:
	void slot_updateRect(QRect);
	void slot_setVisible(bool);

signals:
	void sgn_dataPairChanged(QString, QString);		//���ݶԸı��ź�,tabName,plotName

private:
    QPoint  m_position;
    int m_width;
    int m_height;
    QString m_plotItemName;
    QString m_tabName;
    bool    m_bVisible;
	
    Ui::PlotItemBase ui;
};

#endif // ! 