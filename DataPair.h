#pragma once

#include <QObject>
#include <QColor>
#include <QSize>
#include <QPixmap>

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

	bool hasIcon() { return m_hasIcon; }
	void setHasIcon(bool on);

	QString getIconName() { return m_iconName; }
	void setIconName(QString name);

	QSize iconSize() { return m_iconSize; }
	void setIconSize(QSize);

	int iconWidth() { return m_iconSize.width(); }
	void setIconWidth(int);

	int iconHeight() { return m_iconSize.height(); }
	void setIconHeight(int);

	QPixmap rotateIcon(QPixmap pix, float angle);

	bool operator==(const DataPair& other) const { return this->m_dataPair == other.m_dataPair; }

private:
	QPair<QString, QString> m_dataPair;
	int m_lineWidth;
	bool m_isDraw;
	QColor m_color;
	bool m_isLineMode;
	bool m_hasIcon;
	QString m_iconName;
	QSize m_iconSize;
	QPixmap m_icon;

signals:
	void dataUpdate();
};
