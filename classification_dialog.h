#ifndef CLASSIFICATION_DIALOG_H
#define CLASSIFICATION_DIALOG_H

#include <QDialog>

namespace Ui
{
class ClassificationDialog;
}

class ClassificationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ClassificationDialog(const QString& className,
                                  const QColor& color,
                                  int32_t fontSize,
                                  QWidget* parent = nullptr);
    ~ClassificationDialog();

    QString getClassName();
    QColor getTextColor();
    int32_t getFontSize();

private:
    Ui::ClassificationDialog* ui;
};

#endif // CLASSIFICATION_DIALOG_H
