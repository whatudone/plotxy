#ifndef _ADVANCED_DATA_MANAGER_H_
#define _ADVANCED_DATA_MANAGER_H_

#include <QWidget>
#include <QColor>
#include <QColorDialog>
#include <QVBoxLayout>
#include "ui_AdvancedDataManager.h"
#include "PlotItemBase.h"


class AdvancedDataManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedDataManager(QWidget* parent = 0);
    ~AdvancedDataManager();

    QMap<QString, QMap<int, QColor>> m_map;

signals:
    void updateColorThresholdMap(QMap<QString, QMap<int, QColor>>);

public slots:
    void onBtnMore();
    void onBtnColorMore();
    void onBtnAdd();
	void onEventBtnMoreClicked();
	void onAddPlot(const QString&, PlotItemBase*);
	void onUpdatePlotPair();
    void onCurrentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*);

private:
    Ui::AdvancedDataManager ui;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName

};

#endif // _ADVANCED_DATA_MANAGER_H_