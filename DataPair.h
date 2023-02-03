#pragma once

#include <QObject>
#include <QColor>
#include <QSize>
#include <QPixmap>
#include <QFont>

class DataPair : public QObject
{
	Q_OBJECT

public:
	DataPair();
	DataPair(QObject *parent = nullptr);
	DataPair(QPair<QString, QString> pair);
	~DataPair();

	QPair<QString, QString> getDataPair() { return m_dataPair; }
	void setDataPair(QPair<QString, QString> data);

	int lineWidth() { return m_lineWidth; }
	void setLineWidth(int width);

	bool isDraw() { return m_isDraw; }
	void setDraw(bool on);

	QColor dataColor() { return m_color; }
	void setColor(QColor color);

	bool isLineMode() { return m_isLineMode; }
	void setLineMode(bool on);

	//Icon
	bool isIconDraw() { return m_iconDraw; }
	void setIconDraw(bool on);

	QString iconName() { return m_iconName; }
	void setIconName(QString name);

	QSize iconSize() { return m_iconSize; }
	void setIconSize(QSize);

	int iconWidth() { return m_iconSize.width(); }
	void setIconWidth(int);

	int iconHeight() { return m_iconSize.height(); }
	void setIconHeight(int);

	int iconRotation() { return m_iconRotation; }
	void setIconRotation(int);

	bool iconFlipHorz() { return m_iconFlipHorz; }
	void setIconFlipHorz(bool);

	bool iconFlipVert() { return m_iconFlipVert; }
	void setIconFlipVert(bool);

	QColor iconColor() { return m_iconColor; }
	void setIconColor(QColor);

	QPixmap rotateIcon(QPixmap pix, float angle);

	//Label Text
	QString getLabelText() { return m_labelText; }
	void setLabelText(QString);

	int getTextFormat() { return (int)m_format; }
	void setTextFormat(int);

	QString getCustomText() { return m_customText; }
	void setCustomText(QString);

	bool isLabelTextShow() { return m_labelText_show; }
	void setLabelTextShow(bool);

	QColor getLabelColor() { return m_labelColor; }
	void setLabelColor(QColor);

	QColor getLabelBackground() { return m_labelBackground; }
	void setLabelBackground(QColor);

	bool getLabelBackTransparent() { return m_backTransparent; }
	void setLabelBackTransparent(bool);

	QFont getLabelFont() { return m_labelFont; }
	void setLabelFont(QFont);

	int getLabelFontSize() { return m_labelFontSize; }
	void setLabelFontSize(int);

	int getLabelPosition() { return (int)m_position; }
	void setLabelPosition(int);

	int getLabelPrecision_x() { return m_labelPrec_x; }
	void setLabelPrecision_x(int);

	int getLabelPrecision_y() { return m_labelPrec_y; }
	void setLabelPrecision_y(int);

	bool isPrefixShow() { return m_prefix_show; }
	void setPrefixShow(bool);

	bool isObjectShow() { return m_object_show; }
	void setObjectShow(bool);

	bool isAttrShow() { return m_attr_show; }
	void setAttrShow(bool);

	bool isDataShow() { return m_data_show; }
	void setDataShow(bool);

	bool isUnitShow() { return m_unit_show; }
	void setUnitShow(bool);

	QString getObjectName_x() { return m_object_x; }
	QString getObjectName_y() { return m_object_y; }
	QString getAttrName_x() { return m_attr_x; }
	QString getAttrName_y() { return m_attr_y; }
	QString getUnit_x() { return m_unit_x; }
	QString getUnit_y() { return m_unit_y; }

	void updatePairText(QPair<QString, QString> pair);
	bool operator==(const DataPair& other) const { return this->m_dataPair == other.m_dataPair; }

public:
	enum TEXT_POSITION {
		left_top = 0,
		top,
		right_top,
		left,
		center,
		right,
		left_bottom,
		bottom,
		right_bottom
	};

	enum TEXT_FROMAT {
		format_default = 0,
		format_custom,
		format_script
	};

	enum ICON_ROTATION {
		no_rotation = 0,
		rotation_90,
		rotation_180,
		rotation_270
	};
private:
	QPair<QString, QString> m_dataPair;		//���ݶ�
	int		m_lineWidth;		//�߿�
	bool	m_isDraw;			//�Ƿ����
	QColor	m_color;			//������ɫ
	bool	m_isLineMode;		//�Ƿ�Ϊ��ģʽ

	bool	m_iconDraw;			//�Ƿ���ʾͼ��
	QString m_iconName;			//ͼ����������·��
	ICON_ROTATION m_iconRotation;	//ͼ����ת
	bool	m_iconFlipHorz;
	bool	m_iconFlipVert;
	QColor	m_iconColor;
	QSize	m_iconSize;			//ͼ���С
	QPixmap m_icon;				//ͼ��

	TEXT_FROMAT m_format;		//�ı���ʽ
	QString m_labelText;		//��ʾ�ı�
	QString m_customText;		//�Զ����ı�
	QColor	m_labelColor;		//�ı���ɫ
	QColor	m_labelBackground;	//�ı�����ɫ
	bool	m_backTransparent;	//͸������
	QFont	m_labelFont;		//�ı�����
	int		m_labelFontSize;	//�ı������С
	int		m_labelPrec_x;		//x����ʾ����
	int		m_labelPrec_y;		//y����ʾ����
	QString m_unit_x;			//x�ᵥλ
	QString m_unit_y;			//y�ᵥλ
	TEXT_POSITION m_position;		//�ı�λ��
	bool	m_labelText_show;	//�ı��Ƿ���ʾ
	bool	m_prefix_show;		//ǰ׺�Ƿ���ʾ
	QString m_object_x;			//x��ʵ����
	QString m_object_y;			//y��ʵ����
	bool	m_object_show;		//ʵ�����Ƿ���ʾ
	QString m_attr_x;			//x������
	QString m_attr_y;			//y������
	bool	m_attr_show;		//�����Ƿ���ʾ
	bool	m_data_show;		//�����Ƿ���ʾ
	bool	m_unit_show;		//��λ�Ƿ���ʾ

signals:
	void dataUpdate();
};
