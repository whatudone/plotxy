#ifndef _ADVANCED_DATA_MANAGER_H_
#define _ADVANCED_DATA_MANAGER_H_

#include <QMainWindow>
#include <QColor>
#include <QColorDialog>
#include <QVBoxLayout>
#include "ui_AdvancedDataManager.h"


class AdvancedDataManager : public QMainWindow
{
    Q_OBJECT

public:
    explicit AdvancedDataManager(QWidget* parent = 0);
    ~AdvancedDataManager();

    QMap<QString, QMap<int, QColor>> m_map;

signals:
    void updateColorThresholdMap(QMap<QString, QMap<int, QColor>>);

public slots:
    void onBtnColorChooseClicked();

    void onBtnMore();
    void onBtnColorMore();
    void onBtnAdd();
    void onBtnColorClicked();

    void onAdvancedDataManagerAddPair(QString,QString);
    void onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);

private:
    Ui::AdvancedDataManager ui;

};

#endif // _ADVANCED_DATA_MANAGER_H_