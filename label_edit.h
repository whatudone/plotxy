#ifndef LABEL_EDIT_H
#define LABEL_EDIT_H

#include <QFont>
#include <QWidget>
namespace Ui
{
class LabelEdit;
}

class LabelEdit : public QWidget
{
    Q_OBJECT

public:
    explicit LabelEdit(QWidget* parent = nullptr);
    ~LabelEdit();

    void setLabelText(const QString& text);
    void setLabelFont(const QFont& font);
    void setLabelColor(const QColor& textColor, const QColor& fillColor);

protected:
    bool eventFilter(QObject* obj, QEvent* evt) override;

signals:
    void textChanged(const QString& text);

private:
    Ui::LabelEdit* ui;
};

#endif // LABEL_EDIT_H
