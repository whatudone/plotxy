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
    QMap<QString, QMap<QString, QList<double>>>& getDataMap();
};



#endif // ! 