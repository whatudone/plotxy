#include "colorbutton.h"
#include <QPainter>
#include <QPaintEvent>
#include <QStyleOption>
#include <QColorDialog>

QColor blendColors(const QColor& src, const QColor& dest)
{
	double pct = dest.alpha() / 255.0;
	return QColor(static_cast<int>(src.red() + (dest.red() - src.red()) * pct),
	static_cast<int>(src.green() + (dest.green() - src.green()) * pct),
	static_cast<int>(src.blue() + (dest.blue() - src.blue()) * pct),
	255);
}

ColorButton::ColorButton(QWidget* parent)
    :QPushButton(parent),
      m_bShowAlpha(true),
      m_color(QColor(255,255,255,255))
{
	this->setMaximumSize(QSize(25, 25));
	this->setMinimumSize(QSize(20, 20));
}

ColorButton::~ColorButton()
{

}

void ColorButton::paintEvent(QPaintEvent *ev)
{
    QPainter painter(this);
    paintColoredSquare_(&painter);
}

void ColorButton::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
    {
        const QColor color = QColorDialog::getColor(m_color, this, "Select Color", QColorDialog::ShowAlphaChannel);
        if (color.isValid())
        {
            setColor(color);
			emit clicked();
        }
    }
}

bool ColorButton::showAlpha() const
{
    return m_bShowAlpha;
}

void ColorButton::setShowAlpha(bool value)
{
    m_bShowAlpha = value;
}

QColor ColorButton::color() const
{
    return m_color;
}

void ColorButton::setColor(const QColor& value)
{
    m_color = value;
    update();
}

void ColorButton::paintColoredSquare(QPainter* painter, const QRect& rect, const QColor& color, bool showAlpha)
{
    if (showAlpha)
    {
        // create the blended gradient if showing alpha channel
        QLinearGradient gradient(rect.topLeft(), rect.bottomRight());
        QColor whiteBlend = blendColors(Qt::white, color);
        QColor blackBlend = blendColors(Qt::black, color);
        gradient.setColorAt(0.0, whiteBlend);
        gradient.setColorAt(1.0, blackBlend);
        painter->setBrush(gradient);
    }
    else
    {
        QBrush brush = painter->brush();
        brush.setColor(color);
        brush.setStyle(Qt::SolidPattern);
        painter->setBrush(brush);
    }

    // Note that if Anti-aliasing is disabled, then a regular (not rounded) rect should be drawn.
    painter->setRenderHint(QPainter::Antialiasing);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    painter->setRenderHint(QPainter::HighQualityAntialiasing);
#endif

 //   painter->drawRoundedRect(rect, 2, 2);
	painter->drawRect(rect);
}

void ColorButton::paintColoredSquare_(QPainter* painter) const
{
    QStyleOptionButton option;
    initStyleOption(&option);

    // Calculate the rectangle size for the drawn box
    QRect rect = option.rect.adjusted(2, 2, 0, -2);
    rect.setWidth(height() - 4);
    if (isEnabled())
        ColorButton::paintColoredSquare(painter, rect, m_color, m_bShowAlpha);
    else
        ColorButton::paintColoredSquare(painter, rect, QColor(0, 0, 0, 0), true);
}
