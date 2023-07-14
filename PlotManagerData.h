/* * 
* @brief: 用于管理软件添加的所有plotItemBase，方便在更改plotItemBase后可以同步刷新
*  */
#pragma once

#include <QMap>
#include <QObject>
#include <QUuid>
#include <QVector>

class PlotItemBase;
class PlotManagerData : public QObject
{
	Q_OBJECT

private:
    PlotManagerData(QObject* parent = nullptr);
	~PlotManagerData();

public:
    struct LinkedAxesGroupSet
    {
        QString groupName;
        bool isX = true;
        QList<PlotItemBase*> plotList;
        bool operator==(const LinkedAxesGroupSet& set)
        {
            return groupName == set.groupName;
        }
    };
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
    QVector<LinkedAxesGroupSet> getLinkedAxesSets() const;
    void setLinkedAxesSets(const QVector<LinkedAxesGroupSet>& linkedAxesSets);

    void addLinkedAxesSet(const QString& groupName, bool isX, const QList<PlotItemBase*>& plots);
    void removeLinkedAxesSet(const QString& groupName);
    void clearLinkedAxesSet();

    bool groupNameExists(const QString& groupName);

signals:
    void plotDataChanged();

public slots:
    void onPlotCoordRangeChanged(bool isX, double min, double max);

private:
    // 图表列表数据
    QMap<QString, QList<PlotItemBase*>> m_plotMgrDataMap;
    // 链接轴数据
    QVector<LinkedAxesGroupSet> m_linkedAxesSets;
};
