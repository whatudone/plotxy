#ifndef _ADVANCED_DATA_MANAGER_H_
#define _ADVANCED_DATA_MANAGER_H_

#include <QWidget>
#include <QColor>
#include <QColorDialog>
#include <QVBoxLayout>
#include "ui_AdvancedDataManager.h"
#include "PlotItemBase.h"
#include "DataPair.h"

class AdvancedDataManager : public QWidget
{
    Q_OBJECT

public:
    explicit AdvancedDataManager(QWidget* parent = 0);
    ~AdvancedDataManager();

    QMap<QString, QMap<int, QColor>> m_map;

signals:
    void updateColorThresholdMap(QMap<QString, QMap<int, QColor>>);
	void sgnAddPlotPair();

public slots:
    void onBtnMore();
    void onBtnColorMore();
    void onBtnAdd();
	void onEventBtnMoreClicked();
	void onUpdatePlotPair();

	void onTableWidget_plotpairItemSelectionChanged();
	void onPushButton_addClicked();
	void onPushButton_copyClicked();
	void onPushButton_autofitClicked();
	void onPushButton_deleteClicked();

private:
    Ui::AdvancedDataManager ui;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName
	PlotItemBase* m_curSelectPlot = nullptr;
	DataPair* m_curSelectDatapair = nullptr;
};

#endif // _ADVANCED_DATA_MANAGER_H_