#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QObject>
#include <QColor>
#include <QPushButton>

class ColorButton : public QPushButton
{
    Q_OBJECT

public:
    ColorButton(QWidget* parent = nullptr);
    virtual ~ColorButton();

    QColor color() const;
    bool showAlpha() const;

    /**
    * draws a colored rectangle using the provided painter, creating a blend with the alpha channel if indicated
    * @param painter  used to draw the rectangle provided
    * @param rect  rectangle to draw
    * @param color  color to apply to the rectangle
    * @param showAlpha  if true, will apply alpha blending to the color, if false, just paints color directly
    */
    static void paintColoredSquare(QPainter* painter, const QRect& rect, const QColor& color, bool showAlpha = true);

public slots:
    void setColor(const QColor& value);
    void setShowAlpha(bool value);

protected:
    virtual void paintEvent(QPaintEvent* ev);
    virtual void mousePressEvent(QMouseEvent* ev);

private:
   void paintItemBackground_(QPainter* painter) const;
   void paintColoredSquare_(QPainter* painter) const;

   bool m_bShowAlpha;
   QColor m_color;
};

#endif // COLORBUTTON_H
