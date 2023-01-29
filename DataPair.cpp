#include "DataPair.h"
#include <QPainter>
#include <QTransform>

DataPair::DataPair(QObject *parent)
	: QObject(parent)
{
	m_lineWidth = 2;
	m_isDraw = true;
	m_color = Qt::white;
	m_isLineMode = false;
	m_hasIcon = false;
	m_iconName = nullptr;
	m_iconSize = QSize(64, 64);
}

DataPair::DataPair(QPair<QString, QString> pair)
{
	m_dataPair = pair;
	m_lineWidth = 2;
	m_isDraw = true;
	m_color = Qt::white;
	m_isLineMode = false;
	m_hasIcon = false;
	m_iconName = nullptr;
	m_iconSize = QSize(64, 64);
}

DataPair::~DataPair()
{
}

void DataPair::setDataPair(QPair<QString, QString> data)
{
	m_dataPair = data;
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
	emit dataUpdate();
}

void DataPair::setLineMode(bool on)
{
	m_isLineMode = on;
	emit dataUpdate();
}

void DataPair::setHasIcon(bool on)
{
	m_hasIcon = on;
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

QPixmap DataPair::rotateIcon(QPixmap pix, float angle)
{
	QTransform trans;
	trans.rotate(angle);
	return pix.transformed(trans);
}

