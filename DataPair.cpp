#include "DataPair.h"
#include <QPainter>
#include <QTransform>

DataPair::DataPair(QObject* parent)
	: QObject(parent)
{
	m_lineWidth = 2;
	m_isDraw = true;
	m_color = Qt::white;
	m_isLineMode = false;
	m_matchColor = false;

	m_iconDraw = false;
	m_iconName = nullptr;
	m_iconRotation = ICON_ROTATION::no_rotation;
	m_iconFlipHorz = false;
	m_iconFlipVert = false;
	m_iconColor = Qt::white;
	m_iconSize = QSize(64, 64);

	m_format = TEXT_FROMAT::format_default;
	m_labelText = nullptr;
	m_customText = nullptr;
	m_labelColor = Qt::green;
	m_labelBackground = Qt::black;
	m_backTransparent = true;
	m_labelFontSize = 12;
	m_labelFont.setFamily("Microsoft YaHei");
	m_labelFont.setPointSize(m_labelFontSize);
	m_position = TEXT_POSITION::right;
	m_labelPrec_x = 2;
	m_labelPrec_y = 2;
	m_labelText_show = true;
	m_prefix_show = true;
	m_object_x = nullptr;
	m_object_y = nullptr;
	m_object_show = true;
	m_attr_x = nullptr;
	m_attr_y = nullptr;
	m_attr_show = true;
	m_data_show = true;
	m_unit_x = nullptr;
	m_unit_y = nullptr;
	m_unit_show = true;
}

DataPair::DataPair(const QPair<QString, QString>& pair)
    : m_dataPair(pair)
{
	m_lineWidth = 2;
	m_isDraw = true;
	m_color = Qt::white;
	m_isLineMode = false;
	m_matchColor = false;

	m_iconDraw = false;
	m_iconName = nullptr;
	m_iconRotation = ICON_ROTATION::no_rotation;
	m_iconFlipHorz = false;
	m_iconFlipVert = false;
	m_iconColor = Qt::white;
	m_iconSize = QSize(64, 64);

	m_format = TEXT_FROMAT::format_default;
	m_labelText = nullptr;
	m_customText = nullptr;
	m_labelColor = Qt::green;
	m_labelBackground = Qt::black;
	m_backTransparent = true;
	m_labelFontSize = 12;
	m_labelFont.setFamily("Microsoft YaHei");
	m_labelFont.setPointSize(m_labelFontSize);
	m_position = TEXT_POSITION::right;
	m_labelPrec_x = 2;
	m_labelPrec_y = 2;
	m_labelText_show = true;
	m_prefix_show = true;
	m_object_show = true;
	m_attr_show = true;
	m_data_show = true;
	m_unit_x = nullptr;
	m_unit_y = nullptr;
	m_unit_show = true;
	updatePairText(pair);
}

DataPair::~DataPair() {}

void DataPair::updatePairText(QPair<QString, QString> pair)
{
	m_object_x = nullptr;
	m_object_y = nullptr;
	m_attr_x = nullptr;
	m_attr_y = nullptr;

	QString xEntityType = pair.first;
	QString yEntityType = pair.second;
	QStringList xlist = xEntityType.split("+");
	QStringList ylist = yEntityType.split("+");
    if(xlist.size() == 1)
	{
		m_attr_x = xlist.at(0);
	}
    else if(xlist.size() == 2)
	{
		m_object_x = xlist.at(0);
		m_attr_x = xlist.at(1);
	}

    if(ylist.size() == 1)
	{
		m_attr_y = ylist.at(0);
	}
    else if(ylist.size() == 2)
	{
		m_object_y = ylist.at(0);
		m_attr_y = ylist.at(1);
	}
}

void DataPair::setDataPair(const QPair<QString, QString>& data)
{
	m_dataPair = data;
	updatePairText(data);
	emit dataUpdate();
}

void DataPair::setLineWidth(int width)
{
	m_lineWidth = width;
	emit dataUpdate();
}

void DataPair::setDraw(bool on)
{
	m_isDraw = on;
	emit dataUpdate();
}

void DataPair::setColor(QColor color)
{
	m_color = color;
    if(m_matchColor)
	{
		m_iconColor = m_color;
		m_labelColor = m_color;
	}
	emit dataUpdate();
}

void DataPair::setLineMode(bool on)
{
	m_isLineMode = on;
	emit dataUpdate();
}

void DataPair::setMatchColor(bool on)
{
	m_matchColor = on;
    if(m_matchColor)
	{
		m_iconColor = m_color;
		m_labelColor = m_color;
	}
}

void DataPair::setIconDraw(bool on)
{
	m_iconDraw = on;
	emit dataUpdate();
}

void DataPair::setIconName(QString name)
{
	m_iconName = name;
	emit dataUpdate();
}

void DataPair::setIconSize(QSize size)
{
	m_iconSize = size;
	emit dataUpdate();
}

void DataPair::setIconWidth(int w)
{
	m_iconSize.setWidth(w);
	emit dataUpdate();
}

void DataPair::setIconHeight(int h)
{
	m_iconSize.setHeight(h);
	emit dataUpdate();
}

void DataPair::setIconRotation(int rotate)
{
	m_iconRotation = ICON_ROTATION(rotate);
	emit dataUpdate();
}

void DataPair::setIconFlipHorz(bool on)
{
	m_iconFlipHorz = on;
	emit dataUpdate();
}

void DataPair::setIconFlipVert(bool on)
{
	m_iconFlipVert = on;
	emit dataUpdate();
}

void DataPair::setIconColor(QColor color)
{
	if(!m_matchColor)
		m_iconColor = color;
	emit dataUpdate();
}

QPixmap DataPair::rotateIcon(QPixmap pix, float angle)
{
	QTransform trans;
	trans.rotate(angle);
	return pix.transformed(trans);
}

void DataPair::setLabelText(QString text)
{
	m_labelText = text;
	emit dataUpdate();
}

void DataPair::setTextFormat(int format)
{
	m_format = TEXT_FROMAT(format);
	emit dataUpdate();
}

void DataPair::setCustomText(QString text)
{
	m_customText = text;
	emit dataUpdate();
}

void DataPair::setLabelTextShow(bool show)
{
	m_labelText_show = show;
	emit dataUpdate();
}

void DataPair::setLabelColor(QColor color)
{
    if(!m_matchColor)
		m_labelColor = color;
	emit dataUpdate();
}

void DataPair::setLabelBackground(QColor color)
{
	m_labelBackground = color;
	emit dataUpdate();
}

void DataPair::setLabelBackTransparent(bool on)
{
	m_backTransparent = on;
	emit dataUpdate();
}

void DataPair::setLabelFont(QFont font)
{
	m_labelFont = font;
	emit dataUpdate();
}

void DataPair::setLabelFontSize(int size)
{
	m_labelFontSize = size;
	m_labelFont.setPointSize(size);
	emit dataUpdate();
}

void DataPair::setLabelPosition(int pos)
{
	m_position = TEXT_POSITION(pos);
	emit dataUpdate();
}

void DataPair::setLabelPrecision_x(int prec)
{
	m_labelPrec_x = prec;
	emit dataUpdate();
}

void DataPair::setLabelPrecision_y(int prec)
{
	m_labelPrec_y = prec;
	emit dataUpdate();
}

void DataPair::setPrefixShow(bool show)
{
	m_prefix_show = show;
	emit dataUpdate();
}

void DataPair::setObjectShow(bool show)
{
	m_object_show = show;
	emit dataUpdate();
}

void DataPair::setAttrShow(bool show)
{
	m_attr_show = show;
	emit dataUpdate();
}

void DataPair::setDataShow(bool show)
{
	m_data_show = show;
	emit dataUpdate();
}

void DataPair::setUnitShow(bool show)
{
	m_unit_show = show;
	emit dataUpdate();
}
