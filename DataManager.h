/* * @filename: DataManager.h
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#ifndef DATA_MANAGER_H_
#define DATA_MANAGER_H_

#include "BaseData.h"
#include <QMap>

class OrdinalTimeFormatter;

class DataManager : public QObject
{
    Q_OBJECT
private:
    DataManager();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    ~DataManager();

    QMap<QString, QMap<QString, QList<double>>> m_entityDataMap;
	QVector<double> m_timeDataVector;

	double m_minTime, m_maxTime;
    int m_refYear = -1;

public:
    static DataManager* getInstance()
    {
        static DataManager mgr;
        return &mgr;
    }

    void loadCSV(const QString&);
	void loadCSV_stringTime(const QString&);
    QMap<QString, QMap<QString, QList<double>>>& getDataMap();
    void getMinMaxTime(double& minTime, double& maxTime);
	int getRefYear();
    int getEntityAttr_MaxIndex_List(QString entity,
                                    QString attr,
                                    double secs); //获取secs时间内的实体-属性的最大index
    QList<double>
    getEntityAttr_Value_List(QString entity,
                             QString attr = "Time"); //获取实体-属性的全数据，属性默认为Time
    QList<double> getEntityAttr_MaxPartValue_List(
        QString entity, QString attr, double secs); //获取最小时间到secs时间内的实体-属性数据list
    QList<double>
    getEntityAttr_PartValue_List(QString entity,
                                 QString attr,
                                 int minIndex,
                                 int maxIndex); //获取minIndex到maxIndex内的实体-属性数据list
	QVector<double> getTimeData_vector();

signals:
    void loadDataReady();
};

#endif // !
