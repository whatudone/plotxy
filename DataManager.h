/* * @filename: DataManager.h
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#ifndef  _DATA_MANAGER_H_
#define  _DATA_MANAGER_H_

#include "BaseData.h"
#include <QMutex>
#include <QMap>
#include <memory>

class OrdinalTimeFormatter;
class DataManager;
class DataManagerSafeDeletor
{
public:
    void operator()(DataManager* sf)
    {
        delete sf;
    }
};

class DataManager 
{
private:
    DataManager();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    ~DataManager();

    static std::shared_ptr<DataManager> m_pInstance;
    static QMutex m_mutex;

    friend class DataManagerSafeDeletor;

    QMap<QString,QMap<QString,QList<double>>> m_entityDataMap;
	QVector<double> m_timeDataVector;

	double m_minTime, m_maxTime;
	int	   m_refYear = -1;

public:
    static std::shared_ptr<DataManager> getInstance()
    {
        if (m_pInstance != nullptr)
        {
            return m_pInstance;
        }

        m_mutex.lock();
        if (m_pInstance != nullptr)
        {
            m_mutex.unlock();
            return m_pInstance;
        }
        //deleter
        m_pInstance = std::shared_ptr<DataManager>(new DataManager, DataManagerSafeDeletor());
        m_mutex.unlock();
        return m_pInstance;
    }

    void loadCSV(const QString&);
	void loadCSV_stringTime(const QString&);
    QMap<QString, QMap<QString, QList<double>>>& getDataMap();
	void getMinMaxTime(double &minTime, double &maxTime);
	int getRefYear();
	int getEntityAttr_MaxIndex_List(QString entity, QString attr, double secs);	//获取secs时间内的实体-属性的最大index
	QList<double> getEntityAttr_Value_List(QString entity, QString attr = "Time");	//获取实体-属性的全数据，属性默认为Time
	QList<double> getEntityAttr_MaxPartValue_List(QString entity, QString attr, double secs);	//获取最小时间到secs时间内的实体-属性数据list
	QList<double> getEntityAttr_PartValue_List(QString entity, QString attr, int minIndex, int maxIndex);	//获取minIndex到maxIndex内的实体-属性数据list
	QVector<double> getTimeData_vector();
};



#endif // ! 