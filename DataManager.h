/* * @filename: DataManager.h
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "BaseData.h"
#include <QMap>
#include <QVector3D>

#include <set>
#include <unordered_map>

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
    // 旧的数据结构待迁移之后替换成新的,这两个旧数据目前仅存于加载csv
    QMap<QString, QMap<QString, QList<double>>> m_entityDataMap;
    QVector<double> m_timeDataVector;
    // 新的数据存储结构 <EntityID,<<Attr>,DataList>> qhash是随机排序，性能比qmap强
    QMap<int32_t, QHash<QString, QVector<double>>> m_newEntityDataMap;
    // <EntityID, <attr,unit>>
    QHash<int32_t, QList<QPair<QString, QString>>> m_attrUnitHash;
    // 平台map数据
    QMap<int32_t, Platform> m_platformMap;
    // 事件map数据
    QMap<int32_t, QList<Event>> m_eventMap;
    // 时间以s为计算单位，小数位表示ms
    double m_minTime = 0.0;
    double m_maxTime = 0.0;
    // 去重，用于确定时间轴范围的时间信息，数据中还保留原始的Time数据列表
    std::set<double> m_timeDataSet;
    // 参考年份，作为相对时间的计算基准
    int m_refYear = 1970;
    // 参考点 LLA坐标
    QVector3D m_refLLAPoint;
    // ASI文件中带的gog数据路径，是相对于ASI文件的相对路径，需要读取的时候转化为绝对路径
    QList<QString> m_gogFileList;
    // 导入数据文件路径
    QString m_dataFileName;

public:
    static DataManager* getInstance()
    {
        static DataManager mgr;
        return &mgr;
    }
    // 自动根据文件扩展名称解析对应格式数据
    void loadFileData(const QString& filename);
    const QMap<int32_t, QHash<QString, QVector<double>>>& getDataMap();
    void getMinMaxTime(double& minTime, double& maxTime);
    int getRefYear();

    // 获取最小时间到secs时间内的实体-属性数据list
    QVector<double>
    getEntityAttrValueListByMaxTime(int32_t entityID, const QString& attr, double secs);
    // 获取当前时间内最后一个值，用于Text-Bar这种一维数据展示
    double getEntityAttrValueByMaxTime(int32_t entityID, const QString& attr, double secs);

    QPair<double, double> getMaxAndMinEntityAttrValue(int32_t entityID, const QString& attr);
    // 获取时间轴上所有的时间节点
    QVector<double> getTimeDataSet();
    // 获取某个时间节点的切片数据list<range,voltage>，针对于ASCope
    QPair<QVector<double>, QVector<double>> getSliceDataByTime(int32_t entityID, double secs);
    // 获取某个时间节点的三维数据，针对RTI图表
    void getRTIDataByTime(int32_t entityID,
                          double secs,
                          QVector<double>& rangeList,
                          QVector<double>& timeList,
                          QHash<QPair<int32_t, int32_t>, double>& valueMap);
    // 根据id获取实例(数据里面称为Platform)名称
    QString getEntityNameByID(int32_t id);
    // 获取所有的实例名称
    QStringList getEntityNameList();
    // 根据id获取实例对应的属性和单位列表
    QList<QPair<QString, QString>> getAttrAndUnitPairList(int32_t id);

    QString getDataFileName() const;
    void setDataFileName(const QString& dataFileName);

    void clearData();

private:
    //获取实体-属性的全数据，属性默认为Time
    QVector<double> getEntityAttrValueList(int32_t entityID, const QString& attr = "Time");
    //获取secs时间内的实体-属性的最大index
    int getEntityAttrMaxIndexByTime(int32_t entityID, double secs);

    // 加载带时间信息的csv数据
    void loadCSVData(const QString& filePath);
    // 加载ASI格式的数据（二维类型）
    void loadASIData(const QString& asiFileName);

    // 使用正则表达式加载ASI中特定数据格式
    QStringList parsePlatformData(const QString& data);
signals:
    void loadDataFinished();
};

#define DataManagerInstance DataManager::getInstance()
#endif // !
