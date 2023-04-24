/* * @filename: DataManager.h
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#ifndef DATA_MANAGER_H_
#define DATA_MANAGER_H_

#include "BaseData.h"
#include <QMap>
#include <QVector3D>

class DataManager : public QObject
{
    Q_OBJECT
public:
    struct Platform
    {
        uint32_t m_platformDataID;
        QString m_platformName;
        QString m_platformIcon;

        bool operator<(const Platform& p)
        {
            return m_platformDataID < p.m_platformDataID;
        }
        bool operator==(const Platform& p)
        {
            return m_platformDataID == p.m_platformDataID;
        }
    };

    struct Event
    {
        // 事件名称
        QString m_name;
        // 事件发生时间，以s为单位
        double m_relativeTime;
        // 时间偏移
        int32_t m_timeOffset;
    };

private:
    DataManager();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;

    ~DataManager();
    // 旧的数据结构待迁移之后替换成新的
    QMap<QString, QMap<QString, QList<double>>> m_entityDataMap;
    // 新的数据存储结构 <PID,<<Attr,Unit>,DataList>>
    QMap<int32_t, QMap<QPair<QString, QString>, QList<double>>> m_newEntityDataMap;
    // 平台map数据
    QMap<int32_t, Platform> m_platformMap;
    // 事件map数据
    QMap<int32_t, QList<Event>> m_eventMap;
    QVector<double> m_timeDataVector;
    // 时间以s为计算单位，小数位表示ms
	double m_minTime, m_maxTime;
    // 参考年份，作为相对时间的计算基准
    int m_refYear = 1970;
    // 参考点 LLA坐标
    QVector3D m_refLLAPoint;
    // ASI文件中带的gog数据路径，是相对于ASI文件的相对路径，需要读取的时候转化为绝对路径
    QList<QString> m_gogFileList;

public:
    static DataManager* getInstance()
    {
        static DataManager mgr;
        return &mgr;
    }
    // 自动根据文件扩展名称解析对应格式数据
    void loadFileData(const QString& filename);
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

private:
    // 加载带时间信息的csv数据
    void loadCSVData(const QString& filePath);
    // 加载ASI格式的数据（二维类型）
    void loadASIData(const QString& asiFileName);
    // 使用正则表达式加载ASI中特定数据格式
    QStringList parsePlatformData(const QString& data);
signals:
    void loadDataReady();
};

#endif // !
