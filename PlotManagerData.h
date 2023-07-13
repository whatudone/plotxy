﻿/* * 
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

    void addPlotByTab(const QString&, PlotItemBase*);
    void deletePlotByTab(const QString& tabName, PlotItemBase* plot);
    const QMap<QString, QList<PlotItemBase*>>& getPlotManagerData();
    // TODO:需要把PlotName这个唯一标识符替换成UUID
    PlotItemBase* getPlotByTabAndName(const QString& tabName, const QString& plotName);
    void clearPlotData();

    void changeTabName(const QString& oldName, const QString& newName);
signals:
    void plotDataChanged();

public slots:
    void onPlotCoordRangeChanged(bool isX, double min, double max);
};
