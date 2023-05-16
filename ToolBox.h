#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QMap>
#include <QWidget>
namespace Ui
{
class ToolBox;
}

class QVBoxLayout;
class ToolBox : public QWidget
{
    Q_OBJECT

public:
    explicit ToolBox(QWidget* parent = nullptr);
    ~ToolBox();

    void addWidget(const QString& title, QWidget* widget);

    QWidget* pageWidget(const QString& title);

private:
    Ui::ToolBox* ui;

    QVBoxLayout* m_pContentVBoxLayout;

    QMap<QString, QWidget*> m_pageMap;
};

#endif // TOOLBOX_H
