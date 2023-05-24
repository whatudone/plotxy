#ifndef MYDIALPOINTER_H
#define MYDIALPOINTER_H

#include <qcustomplot.h>

class MyDialPointer : public QCPAbstractItem
{
    Q_OBJECT
    /// \cond INCLUDE_QPROPERTIES
    Q_PROPERTY(QPen pen READ pen WRITE setPen)
    Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
    Q_PROPERTY(QCPLineEnding head READ head WRITE setHead)
    Q_PROPERTY(QCPLineEnding tail READ tail WRITE setTail)
    /// \endcond
public:
    explicit MyDialPointer(QCustomPlot* parentPlot);
    virtual ~MyDialPointer() override;

    // getters:
    QPen pen() const
    {
        return mPen;
    }
    QPen selectedPen() const
    {
        return mSelectedPen;
    }
    QCPLineEnding head() const
    {
        return mHead;
    }
    QCPLineEnding tail() const
    {
        return mTail;
    }
    QColor color() const
    {
        return mColor;
    }
    double mValue() const
    {
        return m_Value;
    }
    double redius() const
    {
        return m_radius;
    }

    // setters;
    void setPen(const QPen& pen);
    void setSelectedPen(const QPen& pen);
    void setHead(const QCPLineEnding& head);
    void setTail(const QCPLineEnding& tail);
    void setColor(const QColor& color);
    void setValue(double value);
    void setRadius(double radius);

    // reimplemented virtual methods:
    virtual double selectTest(const QPointF& pos,
                              bool onlySelectable,
                              QVariant* details = nullptr) const Q_DECL_OVERRIDE;

    QCPItemPosition* const start;
    QCPItemPosition* const end;

protected:
    // property members:
    QPen mPen, mSelectedPen;
    QCPLineEnding mHead, mTail;
    QColor mColor;
    double m_Value;
    double m_radius;

    // 四个点形成的钟表指针
    QPointF m_clockHandPoints[4];

    // reimplemented virtual methods:
    virtual void draw(QCPPainter* painter) Q_DECL_OVERRIDE;
};

#endif // MYDIALPOINTER_H
