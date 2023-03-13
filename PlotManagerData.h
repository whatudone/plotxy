/* * 
* @brief: 用于管理软件添加的所有plotItemBase，方便在更改plotItemBase后可以同步刷新
*  */
#pragma once

#include <QObject>
#include "PlotItemBase.h"

class PlotManagerData : public QObject
{
	Q_OBJECT

private:
	PlotManagerData(QObject *parent = nullptr);
	~PlotManagerData();
	static PlotManagerData *m_pInstance;
	QMap<QString, QList<PlotItemBase*>> m_plotManager; //tabName

public:
	static PlotManagerData *getInstance()
	{
		if (nullptr == m_pInstance)
			m_pInstance = new PlotManagerData();

		return m_pInstance;
	}

private:
	class PlotManagerDataRelease
	{
	public:
		PlotManagerDataRelease() {}
		~PlotManagerDataRelease() {
			if (nullptr != m_pInstance)
			{
				delete m_pInstance;
				m_pInstance = nullptr;
			}
		}
	};
	static PlotManagerDataRelease m_release;

public:
	void addPlotManagerData(const QString&, PlotItemBase*);
	QMap<QString, QList<PlotItemBase*>> getPlotManagerData();

public slots:
	void slotChangePlotName();
	void slotChangeTabName(QString oldName, QString newName);
signals:
	void sgnUpdatePlotManager();
};
