#include "PlotManagerData.h"
#include "PlotItemBase.h"

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

void PlotManagerData::onPlotCoordRangeChanged(bool isX, double min, double max)
{
    // 收到链接轴组内一个图表发送的range变化，控制组内其他
    if(PlotItemBase* plot = qobject_cast<PlotItemBase*>(sender()))
    {
        for(const LinkedAxesGroupSet& set : m_linkedAxesSets)
        {
            QList<PlotItemBase*> plotList = set.plotList;
            if(plotList.contains(plot) && set.isX == isX)
            {
                plotList.removeOne(plot);
                for(PlotItemBase* plot : plotList)
                {
                    // 给其他图表同步范围时，避免引入死循环，重复触发信号，所以需要先将对象信号屏蔽
                    plot->blockSignals(true);
                    if(isX)
                    {
                        plot->setCoordRangeX(min, max);
                    }
                    else
                    {
                        plot->setCoordRangeY(min, max);
                    }
                    plot->blockSignals(false);
                }
                return;
            }
        }
    }
}

QVector<PlotManagerData::LinkedAxesGroupSet> PlotManagerData::getLinkedAxesSets() const
{
    return m_linkedAxesSets;
}

void PlotManagerData::setLinkedAxesSets(const QVector<LinkedAxesGroupSet>& linkedAxesSets)
{
    m_linkedAxesSets = linkedAxesSets;
}

void PlotManagerData::addLinkedAxesSet(const QString& groupName,
                                       bool isX,
                                       const QList<PlotItemBase*>& plots)
{
    LinkedAxesGroupSet set;
    set.groupName = groupName;
    set.isX = isX;
    set.plotList = plots;
    m_linkedAxesSets.append(set);
}

void PlotManagerData::removeLinkedAxesSet(const QString& groupName)
{
    for(const auto& set : m_linkedAxesSets)
    {
        if(set.groupName == groupName)
        {
            m_linkedAxesSets.removeOne(set);
            return;
        }
    }
}

void PlotManagerData::clearLinkedAxesSet()
{
    m_linkedAxesSets.clear();
}

bool PlotManagerData::groupNameExists(const QString& groupName)
{
    for(const auto& set : m_linkedAxesSets)
    {
        if(set.groupName == groupName)
        {
            return true;
        }
    }
    return false;
}
