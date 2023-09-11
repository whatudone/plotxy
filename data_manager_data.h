#ifndef DATAMANAGERDATA_H
#define DATAMANAGERDATA_H

#include "DataPair.h"
#include "constdef.h"
#include "protobuf/USimDataStructm.pb.h"

#include <QMap>
#include <QVector3D>
#include <QVector>

#include <set>
#include <shared_mutex>

// 纯数据，和GUI分离
class DataManagerData
{
    struct Platform
    {
        int32_t m_platformDataID;
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
    enum USIM_Alliance
    {
        USIM_Alliance_UNKNOW = 0,
        USIM_RED = 1,
        USIM_BLUE = 2,
        USIM_WHITE = 3,
        USIM_GREEN = 4,
        USIM_PURPLE = 5,
        USIM_ORANGE = 6,
        USIM_YELLOW = 7
    };
    struct ProtobufPlatForm
    {
        int32_t platformID;
        QString name;
        QString cmdNodeName;
        USIM_Alliance alliance;
        uint64_t color;
        uint32_t kind;
        uint32_t classType;
        QString typeName;
    };

private:
    DataManagerData();
    DataManagerData(const DataManagerData&) = delete;
    DataManagerData& operator=(const DataManagerData&) = delete;

    ~DataManagerData();
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
    QMap<int32_t, QMap<QString, QList<GenericData>>> m_genericMap;

    // 时间以s为计算单位，小数位表示ms
    double m_minTime = 0.0;
    double m_maxTime = 0.0;
    double m_minRealTime = 0.0;
    double m_maxRealTime = 0.0;
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

    QMap<QString, QList<GOGDataInfo>> m_gogDataMap;

public:
    static DataManagerData* getInstance()
    {
        static DataManagerData mgr;
        return &mgr;
    }
    // 自动根据文件扩展名称解析对应格式数据

    const QMap<int32_t, QHash<QString, QVector<double>>>& getDataMap();
    void getMinMaxTime(double& minTime, double& maxTime);
    void getMinMaxRealTime(double& minTime, double& maxTime);
    int getRefYear();

    // 获取最小时间到secs时间内的实体-属性数据list
    QVector<double> getEntityAttrValueListByMaxTime(int32_t entityID,
                                                    const QString& attr,
                                                    double secs,
                                                    double rate);
    // 获取最小时间到secs时间内的实体-属性数据list
    QVector<double> getEntityAttrValueListByMaxTimeAndIniFile(int32_t entityID,
                                                              const QString& attr,
                                                              double secs,
                                                              double rate);
    // 获取当前时间内最后一个值，用于Text-Bar这种一维数据展示
    double
    getEntityAttrValueByMaxTime(int32_t entityID, const QString& attr, double secs, double rate);

    QPair<double, double>
    getMaxAndMinEntityAttrValue(int32_t entityID, const QString& attr, double rate);
    // 获取时间轴上所有的时间节点
    QVector<double> getTimeDataSet();
    // 获取某个时间节点的切片数据list<range,voltage>，针对于ASCope
    QPair<QVector<double>, QVector<double>> getSliceDataByTime(int32_t entityID, double secs);
    // 获取某个时间节点的三维数据，针对RTI和Doppler图表
    void getRTIDataByTime(int32_t entityID,
                          double secs,
                          QVector<double>& rangeList,
                          QVector<double>& timeList,
                          QHash<QPair<int32_t, int32_t>, double>& valueMap);
    // 获取Doppler所需数据
    void getDopplerDataByTime(int32_t entityID,
                              double secs,
                              QVector<double>& rangeList,
                              QVector<double>& timeList,
                              QHash<QPair<int32_t, int32_t>, double>& valueHash,
                              QMultiHash<double, QPair<double, double>>& horizonDataHash,
                              QMultiHash<double, QPair<double, double>>& verticalDataHash);

    // 根据id获取实例(数据里面称为Platform)名称
    QString getEntityNameByID(int32_t id);
    // 根据id获取实例(数据里面称为Platform)ICON
    QString getEntityIconByID(int32_t id);
    // 获取所有的实例名称
    QStringList getEntityNameList();
    // 根据id获取实例对应的属性和单位列表
    QList<QPair<QString, QString>> getAttrAndUnitPairList(int32_t id);
    // 获取所有<entityId,entityName>键值对
    QMap<int32_t, QString> getEntityIDAndNameMap();
    // 获取某个实体对应的某个事件的所有数据
    QList<GenericData> getGenericDataListByID(int32_t entityID, const QString& tag = "Event");
    // 获取某个实体对应的所有generic data tags
    QStringList getGenericDataTagsByID(int32_t entityID);

    // 范围计算，根据两个实体以及计算类型和地球模型处理数据
    double rangeCalculationLastValue(int32_t sourceID,
                                     int32_t targetID,
                                     DataPair::RangeCalculationType type,
                                     double secs,
                                     double rate);

    QVector<double> rangeCalculationValueList(int32_t sourceID,
                                              int32_t targetID,
                                              DataPair::RangeCalculationType type,
                                              double secs,
                                              double rate);

    // 判断是否某个实体存在事件
    bool isEntityContainsGenericTags(int32_t id);
    // 获取数据路径
    QString getDataFileName() const;
    void setDataFileName(const QString& dataFileName);

    void clearData();

    void loadConfig();

    // 保存ASI数据，主要是用于在线模式回放数据
    bool saveDataToASI(const QString& file);

    QString getGroupNameByID(int32_t id);
    QString getUnitByAttr(int32_t id, const QString& attr);

    //获取实体-属性的全数据，属性默认为Time
    QVector<double>
    getEntityAttrValueList(int32_t entityID, const QString& attr = "Time", double rate = 1.0);

    bool entityDataIsEmpty();

    void insertGenericData(const GenericData& generic);

    void insertProtobufPlatinfoData(const USIM_PlatInfoMessage_Proto& plat);

private:
    //获取secs时间内的实体-属性的最大index
    int getEntityAttrMaxIndexByTime(int32_t entityID, double secs);

    // 加载带时间信息的csv数据
    void loadCSVData(const QString& filePath);
    // 加载ASI格式的数据（二维类型）
    void loadASIData(const QString& asiFileName);

    // 使用正则表达式加载ASI中特定数据格式
    QStringList parsePlatformData(const QString& data);
    int32_t findIDByName(const QString& name);

    // 在线数据时间类型配置信息
    QHash<QString, QStringList> m_realEventTypeInfo;
    int32_t m_timeLimit;

    std::shared_mutex m_mutex;

    friend class DataManager;
};
#define DataManagerDataInstance DataManagerData::getInstance()

#endif // DATAMANAGERDATA_H
