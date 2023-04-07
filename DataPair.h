#pragma once

#include <QColor>
#include <QFont>
#include <QObject>
#include <QPixmap>
#include <QSize>

class DataPair : public QObject
{
	Q_OBJECT

public:
    enum TEXT_POSITION
    {
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

    enum TEXT_FROMAT
    {
        format_default = 0,
        format_custom,
        format_script
    };

    enum ICON_ROTATION
    {
        no_rotation = 0,
        rotation_90,
        rotation_180,
        rotation_270
    };
    DataPair(QObject* parent = nullptr);
    DataPair(const QPair<QString, QString>& pair);
	~DataPair();

    const QPair<QString, QString>& getDataPair()
    {
        return m_dataPair;
    }
    void setDataPair(const QPair<QString, QString>& data);

    int lineWidth()
    {
        return m_lineWidth;
    }
	void setLineWidth(int width);

    bool isDraw()
    {
        return m_isDraw;
    }
	void setDraw(bool on);

    QColor dataColor()
    {
        return m_color;
    }
    void setColor(const QColor& color);

    bool isLineMode()
    {
        return m_isLineMode;
    }
	void setLineMode(bool on);

    bool matchColor()
    {
        return m_matchColor;
    }
	void setMatchColor(bool);

	//Icon
    bool isIconDraw()
    {
        return m_iconDraw;
    }
	void setIconDraw(bool on);

    QString iconName()
    {
        return m_iconName;
    }
	void setIconName(QString name);

    QSize iconSize()
    {
        return m_iconSize;
    }
	void setIconSize(QSize);

    int iconWidth()
    {
        return m_iconSize.width();
    }
	void setIconWidth(int);

    int iconHeight()
    {
        return m_iconSize.height();
    }
	void setIconHeight(int);

    int iconRotation()
    {
        return m_iconRotation;
    }
    void setIconRotation(ICON_ROTATION);

    bool iconFlipHorz()
    {
        return m_iconFlipHorz;
    }
	void setIconFlipHorz(bool);

    bool iconFlipVert()
    {
        return m_iconFlipVert;
    }
	void setIconFlipVert(bool);

    QColor iconColor()
    {
        return m_iconColor;
    }
    void setIconColor(const QColor&);

    QPixmap rotateIcon(const QPixmap& pix, float angle);

	//Label Text
    QString getLabelText()
    {
        return m_labelText;
    }
    void setLabelText(const QString&);

    int getTextFormat()
    {
        return (int)m_format;
    }
    void setTextFormat(TEXT_FROMAT);

    QString getCustomText()
    {
        return m_customText;
    }
    void setCustomText(const QString&);

    bool isLabelTextShow()
    {
        return m_labelText_show;
    }
	void setLabelTextShow(bool);

    QColor getLabelColor()
    {
        return m_labelColor;
    }
    void setLabelColor(const QColor&);

    QColor getLabelBackground()
    {
        return m_labelBackground;
    }
    void setLabelBackground(const QColor&);

    bool getLabelBackTransparent()
    {
        return m_backTransparent;
    }
	void setLabelBackTransparent(bool);

    QFont getLabelFont()
    {
        return m_labelFont;
    }
    void setLabelFont(const QFont&);

    int getLabelFontSize()
    {
        return m_labelFontSize;
    }
	void setLabelFontSize(int);

    int getLabelPosition()
    {
        return (int)m_textPosition;
    }
    void setLabelPosition(TEXT_POSITION);

    int getLabelPrecision_x()
    {
        return m_labelPrec_x;
    }
	void setLabelPrecision_x(int);

    int getLabelPrecision_y()
    {
        return m_labelPrec_y;
    }
	void setLabelPrecision_y(int);

    bool isPrefixShow()
    {
        return m_prefix_show;
    }
	void setPrefixShow(bool);

    bool isObjectShow()
    {
        return m_object_show;
    }
	void setObjectShow(bool);

    bool isAttrShow()
    {
        return m_attr_show;
    }
	void setAttrShow(bool);

    bool isDataShow()
    {
        return m_data_show;
    }
	void setDataShow(bool);

    bool isUnitShow()
    {
        return m_unit_show;
    }
	void setUnitShow(bool);

    QString getObjectName_x()
    {
        return m_object_x;
    }
    QString getObjectName_y()
    {
        return m_object_y;
    }
    QString getAttrName_x()
    {
        return m_attr_x;
    }
    QString getAttrName_y()
    {
        return m_attr_y;
    }
    QString getUnit_x()
    {
        return m_unit_x;
    }
    QString getUnit_y()
    {
        return m_unit_y;
    }

	void updatePairText(QPair<QString, QString> pair);
    bool operator==(const DataPair& other) const
    {
        return this->m_dataPair == other.m_dataPair;
    }

private:
    QPair<QString, QString> m_dataPair; //数据对
    int m_lineWidth; //线宽
    bool m_isDraw; //是否绘制
    QColor m_color; //数据颜色
    bool m_isLineMode; //是否为线模式
    bool m_matchColor; //颜色匹配

    bool m_iconDraw; //是否显示图标
    QString m_iconName; //图标所在完整路径
    ICON_ROTATION m_iconRotation; //图标旋转
    bool m_iconFlipHorz;
    bool m_iconFlipVert;
    QColor m_iconColor;
    QSize m_iconSize; //图标大小
    QPixmap m_icon; //图标

    TEXT_FROMAT m_format; //文本格式
    QString m_labelText; //显示文本
    QString m_customText; //自定义文本
    QColor m_labelColor; //文本颜色
    QColor m_labelBackground; //文本背景色
    bool m_backTransparent; //透明背景
    QFont m_labelFont; //文本字体
    int m_labelFontSize; //文本字体大小
    int m_labelPrec_x; //x轴显示精度
    int m_labelPrec_y; //y轴显示精度
    QString m_unit_x; //x轴单位
    QString m_unit_y; //y轴单位
    TEXT_POSITION m_textPosition; //文本位置
    bool m_labelText_show; //文本是否显示
    bool m_prefix_show; //前缀是否显示
    QString m_object_x; //x轴实体名
    QString m_object_y; //y轴实体名
    bool m_object_show; //实体名是否显示
    QString m_attr_x; //x轴属性
    QString m_attr_y; //y轴属性
    bool m_attr_show; //属性是否显示
    bool m_data_show; //数据是否显示
    bool m_unit_show; //单位是否显示

signals:
	void dataUpdate();
};
