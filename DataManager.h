/* * @filename: DataManager.h
* @brief: DataManagerʵ�����ݼ��ع���
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
	int getEntityAttr_MaxIndex_List(QString entity, QString attr, double secs);	//��ȡsecsʱ���ڵ�ʵ��-���Ե����index
	QList<double> getEntityAttr_Value_List(QString entity, QString attr = "Time");	//��ȡʵ��-���Ե�ȫ���ݣ�����Ĭ��ΪTime
	QList<double> getEntityAttr_MaxPartValue_List(QString entity, QString attr, double secs);	//��ȡ��Сʱ�䵽secsʱ���ڵ�ʵ��-��������list
	QList<double> getEntityAttr_PartValue_List(QString entity, QString attr, int minIndex, int maxIndex);	//��ȡminIndex��maxIndex�ڵ�ʵ��-��������list
	QVector<double> getTimeData_vector();
};



#endif // ! 