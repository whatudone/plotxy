#include "PlotManagerData.h"

PlotManagerData::PlotManagerData(QObject* parent)
	: QObject(parent)
{}

PlotManagerData::~PlotManagerData() {}

void PlotManagerData::addPlotByTab(const QString& tabName, PlotItemBase* plotItem)
{
    // QMap 内部保证[]不存在会插入新的元素，所以不会访问越界
    m_plotMgrDataMap[tabName].append(plotItem);
    emit plotDataChanged();
}

void PlotManagerData::deletePlotByTab(const QString& tabName, PlotItemBase* plot)
{
    if(m_plotMgrDataMap.contains(tabName) && m_plotMgrDataMap[tabName].contains(plot))
    {
        m_plotMgrDataMap[tabName].removeOne(plot);
        emit plotDataChanged();
    }
}

const QMap<QString, QList<PlotItemBase*>>& PlotManagerData::getPlotManagerData()
{
    return m_plotMgrDataMap;
}

PlotItemBase* PlotManagerData::getPlotByTabAndName(const QString& tabName, const QString& plotName)
{
    if(m_plotMgrDataMap.contains(tabName))
    {
        auto plots = m_plotMgrDataMap.value(tabName);
        for(int i = 0; i < plots.size(); ++i)
        {
            PlotItemBase* tempPlot = plots.at(i);
            if(plotName == tempPlot->getName())
            {
                return tempPlot;
            }
        }
    }
    return nullptr;
}

void PlotManagerData::clearPlotData()
{
    m_plotMgrDataMap.clear();
    emit plotDataChanged();
}

void PlotManagerData::changeTabName(const QString& oldName, const QString& newName)
{
    if(m_plotMgrDataMap.contains(oldName))
    {
        QList<PlotItemBase*> plots = m_plotMgrDataMap.value(oldName);
        m_plotMgrDataMap[newName].append(plots);
        m_plotMgrDataMap.remove(oldName);
    }

    emit plotDataChanged();
}

void PlotManagerData::onPlotCoordRangeChanged(bool isX, double min, double max) {}
