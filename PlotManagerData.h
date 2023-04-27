/* * 
* @brief: 用于管理软件添加的所有plotItemBase，方便在更改plotItemBase后可以同步刷新
*  */
#pragma once

#include "PlotItemBase.h"
#include <QObject>

class PlotManagerData : public QObject
{
	Q_OBJECT

private:
    PlotManagerData(QObject* parent = nullptr);
	~PlotManagerData();

    QMap<QString, QList<PlotItemBase*>> m_plotMgrDataMap; //tabName

public:
    static PlotManagerData* getInstance()
	{
        static PlotManagerData instance;

        return &instance;
	}

	void addPlotManagerData(const QString&, PlotItemBase*);
    const QMap<QString, QList<PlotItemBase*>>& getPlotManagerData();
    // TODO:需要把PlotName这个唯一标识符替换成UUID
    PlotItemBase* getPlotByTabAndName(const QString& tabName, const QString& plotName);

public slots:
	void slotChangePlotName();
	void slotChangeTabName(QString oldName, QString newName);
signals:
    void plotDataChanged(const QMap<QString, QList<PlotItemBase*>>& plotData);
};
