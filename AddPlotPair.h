#ifndef _ADD_PLOT_PAIR_H_
#define _ADD_PLOT_PAIR_H_

#include <QWidget>
#include "ui_AddPlotPair.h"

class AddPlotPair : public QWidget
{
    Q_OBJECT

public:
    explicit AddPlotPair(QWidget *parent = 0);
    ~AddPlotPair();

    void init();

    QStringList m_entityTypeList;
    QStringList m_entityAttrList;

signals:
    void sigAddPlotPair(QString,QString);

public slots:
    void onBtnAddClicked();
    void onBtnCloseClicked();
    void onEntityTreeWidgetItemClicked(QTreeWidgetItem*, QTreeWidgetItem*);

private:
    Ui::AddPlotPair ui;
};

#endif // _ADD_PLOT_PAIR_H_
