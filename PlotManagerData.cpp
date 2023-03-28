#include "PlotManagerData.h"

PlotManagerData::PlotManagerData(QObject* parent)
	: QObject(parent)
{}

PlotManagerData::~PlotManagerData() {}

void PlotManagerData::addPlotManagerData(const QString& tabName, PlotItemBase* plotItem)
{
    m_plotMgrDataMap[tabName].append(plotItem);

    emit plotDataChanged(m_plotMgrDataMap);
}

const QMap<QString, QList<PlotItemBase*>>& PlotManagerData::getPlotManagerData()
{
    return m_plotMgrDataMap;
}

void PlotManagerData::slotChangeTabName(QString oldName, QString newName)
{
    if(m_plotMgrDataMap.contains(oldName))
	{
        QList<PlotItemBase*> plots = m_plotMgrDataMap.value(oldName);
        m_plotMgrDataMap[newName].append(plots);
        m_plotMgrDataMap.remove(oldName);
	}

    emit plotDataChanged(m_plotMgrDataMap);
}

void PlotManagerData::slotChangePlotName()
{
    // TODO
    emit plotDataChanged(m_plotMgrDataMap);
}
