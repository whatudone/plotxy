#include "data_manager_data.h"

#include <QApplication>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QSettings>
#include <QTemporaryDir>
#include <QTextCodec>

#include <iostream>
#include <limits>

#include "TimeClass.h"
#include "Utils.h"

DataManagerData::DataManagerData()
{
    m_timeLimit = 0;

    loadConfig();
}

DataManagerData::~DataManagerData() {}

bool DataManagerData::saveDataToASI(const QString& asiFileName)
{
    QFile file(asiFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "DataManagerData::saveDataToASI read file failure.";
        return false;
    }
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);

    auto idList = m_newEntityDataMap.keys();
    if(idList.isEmpty())
    {
        file.close();
        return false;
    }
    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("utf-8"));
    stream << "# Scenario Initialization Keywords"
           << "\r\n";
    stream << "RefLLA"
           << " " << m_refLLAPoint.x() << " " << m_refLLAPoint.y() << " " << m_refLLAPoint.z()
           << "\r\n";
    stream << "ReferenceYear"
           << " " << m_refYear << "\r\n";
    stream << "\r\n";
    // 循环输出平台数据
    for(int32_t id : idList)
    {
        auto platDataHash = m_newEntityDataMap.value(id);
        auto platName = getEntityNameByID(id);
        auto platIcon = getEntityIconByID(id);
        stream << "# Platform Keywords"
               << "\r\n";
        stream << "PlatformID"
               << " " << id << "\r\n";
        stream << "PlatformName"
               << " " << id << " \"" << platName << "\"\r\n";
        stream << "PlatformIcon"
               << " " << id << " \"" << platIcon << "\"\r\n";
        stream << "\r\n";

        //开始写入属性与数据
        auto attrUnitList = m_attrUnitHash.value(id);
        stream << "#Keyword"
               << ", "
               << "PlatformID";
        // 写入属性名列表
        for(const auto& pair : attrUnitList)
        {
            stream << ", " << pair.first << "(" << pair.second << ")";
        }
        stream << "\r\n";
        // 循环写入本平台的数据列表
        if(platDataHash.contains("Time"))
        {
            int32_t dataSize = platDataHash.value("Time").size();
            for(int32_t i = 0; i < dataSize; ++i)
            {
                stream << "PlatformData"
                       << " " << id;
                for(const auto& pair : attrUnitList)
                {
                    QString attr = pair.first;
                    double value = platDataHash.value(attr).at(i);
                    // 原始ASI中保存了8位数据小数的精度
                    stream << " " << QString::number(value, 'f', 8);
                }
                stream << "\r\n";
            }
        }
        stream << "\r\n";
        // 循环写入本平台的事件
        if(m_genericMap.contains(id))
        {
            auto tags = m_genericMap.value(id).keys();
            auto genericDataMap = m_genericMap.value(id);
            for(const auto& tag : tags)
            {
                auto genericList = genericDataMap.value(tag);
                for(const auto& g : genericList)
                {
                    stream << "GenericData"
                           << " " << id << " \"" << g.m_eventType << "\" \"" << g.m_eventName
                           << "\" " << g.m_relativeTime << " " << g.m_timeOffset << "\r\n";
                }
            }
        }
        stream << "\r\n";
    }

    file.close();

    return true;
}

void DataManagerData::loadCSVData(const QString& filePath)
{

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataManagerData::loadCSV read file failure." << endl;
        return;
    }
    m_entityDataMap.clear();
    m_timeDataVector.clear();

    QTextStream source(&file);
    QStringList allLines = source.readAll().split("\n");

    QString currEntityType;
    QStringList currEntityAttrs;
    QVector<QStringList> currEntityAttrStringVec;

    //Time
    m_minTime = SECPERDAY * 365;
    m_maxTime = -1;
    m_refYear = -1;
    auto lineCount = allLines.size();
    for(int i = 0; i < lineCount; ++i)
    {

        QStringList currLineItems = allLines.at(i).split(",");

        if(currLineItems.size() <= 1)
            continue;
        QStringList currEntityAttrValues;

        QString firstItem = currLineItems.at(0);
        //判断首字符是否为#	entityType entityAttr1 entityAttr2 entityAttr3 entityAttr4
        if(firstItem.at(0) == '#')
        {
            //标志着上一组数据处理完毕，需要将上一组数据添加进map
            if(!currEntityType.isEmpty())
            {
                QMap<QString, QList<double>> lastGroupDataMap;

                //遍历数据行，并按列组装数据，Rows->Columns
                for(int k = 0; k < currEntityAttrStringVec.size(); k++)
                {
                    for(int t = 0; t < currEntityAttrStringVec[k].size(); t++)
                    {
                        double currEntityAttrValue = 0.0;

                        QString currEntityAttr;
                        if(t < currEntityAttrs.size())
                            currEntityAttr = currEntityAttrs.at(t);

                        if(!currEntityAttr.isEmpty())
                        {
                            //Time时间转成double类型
                            if(currEntityAttr.compare("Time") == 0)
                            {
                                QString timeString = currEntityAttrStringVec.at(k).at(t);
                                timeString.remove(QChar('\"'));
                                QStringList timeStringList;

                                if(OrdinalTimeFormatter::canConvert(timeString, timeStringList))
                                {
                                    // 目前csv中没有参考年份的独立字段，所以参考年份默认从数据中读取，立面上不正确。
                                    if(m_refYear == -1)
                                    {
                                        m_refYear = timeStringList.at(1).toInt();
                                    }
                                    currEntityAttrValue = OrdinalTimeFormatter::convertToSeconds(
                                        timeString, m_refYear);
                                    // 实时更新整个数据文件中的最大和最小时间，用于时间轴中设置起始和终止时间
                                    m_minTime = (currEntityAttrValue < m_minTime)
                                                    ? currEntityAttrValue
                                                    : m_minTime;
                                    m_maxTime = (currEntityAttrValue > m_maxTime)
                                                    ? currEntityAttrValue
                                                    : m_maxTime;
                                }
                                else
                                    currEntityAttrValue = 0.0;

                                m_timeDataVector.append(currEntityAttrValue);
                            }
                            else
                            {
                                currEntityAttrValue =
                                    currEntityAttrStringVec.at(k).at(t).toDouble();
                            }
                            lastGroupDataMap[currEntityAttr].push_back(currEntityAttrValue);
                        }
                    }
                }
                currEntityAttrStringVec.clear();
                m_entityDataMap.insert(currEntityType, lastGroupDataMap);
            }

            //开始处理新一组数据
            currEntityType = firstItem.remove('#');

            currEntityAttrs.clear();
            for(int j = 1; j < currLineItems.size(); j++)
            {
                QString currEntityAttr = currLineItems.at(j);
                currEntityAttrs.push_back(currEntityAttr);
            }
        }
        else
        {
            for(int j = 1; j < currLineItems.size(); j++)
            {
                QString currEntityAttrValue = currLineItems.at(j);
                currEntityAttrValues.push_back(currEntityAttrValue);
            }
            currEntityAttrStringVec.push_back(currEntityAttrValues);
        }
    }

    //排序
    std::sort(m_timeDataVector.begin(), m_timeDataVector.end());
    //去重
    auto it = std::unique(m_timeDataVector.begin(), m_timeDataVector.end());
    m_timeDataVector.erase(it, m_timeDataVector.end());

    file.close();
}

void DataManagerData::loadASIData(const QString& asiFileName)
{
    QFile file(asiFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataManagerData::loadASIData read file failure.";
        return;
    }
    // 清理数据
    clearData();

    // 循环读取每一行，避免一次性读取内存占用过大
    while(!file.atEnd())
    {
        QString lineData = file.readLine();
        // 通用属性
        if(lineData.startsWith("RefLLA"))
        {
            QStringList list = lineData.split(" ", QString::SkipEmptyParts);
            if(list.size() == 4)
            {
                m_refLLAPoint.setX(list.at(1).toFloat());
                m_refLLAPoint.setY(list.at(2).toFloat());
                m_refLLAPoint.setZ(list.at(3).toFloat());
            }
        }
        if(lineData.startsWith("ReferenceYear"))
        {
            QStringList list = lineData.split(" ", QString::SkipEmptyParts);
            if(list.size() == 2)
            {
                m_refYear = list.at(1).toInt();
            }
        }
        if(lineData.startsWith("GOGFile"))
        {
            QStringList list = lineData.split(" ", QString::SkipEmptyParts);
            if(list.size() == 2)
            {
                QFileInfo info(asiFileName);
                QString gogFileName = list.at(1);
                gogFileName.remove("\"");

                // 将相对路径转化为绝对路径
                auto absFilePath = info.dir().absoluteFilePath(gogFileName.simplified());
                if(QFileInfo::exists(absFilePath))
                {
                    m_gogFileList.append(absFilePath);
                }
            }
        }

        // Platform属性起点
        if(lineData.startsWith("# Platform Keywords"))
        {
            Platform p;

            while(!file.atEnd())
            {
                lineData = file.readLine();
                /*
                 * "# "作为大的数据段的起点，读取到这里，说明上个大数据段解析完成，需要退出此循环进入下个Platform读取周期
                 * 此处暂时只解析"# Platform Keywords",还有"# Beam Keywords"等其他类型数据暂不解析
                */
                if(lineData.startsWith("# "))
                {
                    // 将文件指针回溯到改行读取之前
                    file.seek(file.pos() - lineData.length());
                    break;
                }
                // Platform通用属性
                if(lineData.startsWith("PlatformID"))
                {
                    QStringList list = lineData.split(" ", QString::SkipEmptyParts);
                    if(list.size() == 2)
                    {
                        p.m_platformDataID = list.at(1).toInt();
                    }
                }
                if(lineData.startsWith("PlatformName"))
                {
                    QStringList list = parsePlatformData(lineData);
                    if(list.size() == 3)
                    {
                        // 去掉数据中自带的双引号
                        p.m_platformName = list[2].remove('\"');
                    }
                }
                if(lineData.startsWith("PlatformIcon"))
                {
                    QStringList list = lineData.split(" ", QString::SkipEmptyParts);
                    if(list.size() == 3)
                    {
                        p.m_platformIcon = list[2].remove('\"').remove('\r').remove('\n');
                    }
                }
                // Platform数据
                if(lineData.startsWith("#Keyword"))
                {
                    QHash<QString, QVector<double>> attrDataMap;
                    // 属性名称
                    QStringList attrNameList = lineData.split(",", QString::SkipEmptyParts);
                    // 每一个实例对应的属性-单位列表，可能不同
                    QList<QPair<QString, QString>> attrUnitList;
                    for(const auto& attr : attrNameList)
                    {
                        QString tmpAttr = attr.trimmed();
                        if(tmpAttr == "#Keyword" || tmpAttr == "PlatformID")
                        {
                            continue;
                        }
                        QStringList attrList = tmpAttr.remove(")").split("(");
                        if(attrList.size() == 2)
                        {
                            auto pair = qMakePair(attrList.at(0), attrList.at(1));
                            attrUnitList.append(pair);
                            attrDataMap.insert(attrList.at(0), QVector<double>());
                        }
                    }
                    m_attrUnitHash.insert(p.m_platformDataID, attrUnitList);
                    while(!file.atEnd())
                    {
                        lineData = file.readLine();
                        if(lineData.startsWith("PlatformData"))
                        {
                            // 时间字符串中包含空格，无法直接按照空格分割数据，先分离出时间字符串
                            QStringList attrValueList = parsePlatformData(lineData);

                            auto valueSize = attrValueList.size();
                            // 从2开始，去掉前面无用的数据
                            for(int32_t i = 2; i < valueSize; ++i)
                            {
                                QString tmpAttrValue = attrValueList.at(i).trimmed();
                                QString attrName = attrUnitList.at(i - 2).first;
                                double value = 0.0;
                                /*
                                 * 常规数据直接转为double类型
                                 * Time数据有两种情况:
                                 * 1、直接就是double型的相对时间，无需转化
                                 * 2、001 2000 00:00:00 这种形式的数据，需要根据偏移时间转化为
                                */
                                if(attrName == "Time")
                                {
                                    value = OrdinalTimeFormatter::getSecondsFromTimeStr(
                                        tmpAttrValue, m_refYear);
                                    // set自动去重和排序
                                    m_timeDataSet.insert(value);
                                }
                                else
                                {
                                    value = tmpAttrValue.toDouble();
                                }
                                attrDataMap[attrUnitList.at(i - 2).first].append(value);
                            }
                        }
                        else
                        {
                            break;
                        }
                    }
                    // 此处有两种情况1、PlatformData数据后面直接文件结束；2、PlatformData读取完成进入新的数据段
                    m_newEntityDataMap.insert(p.m_platformDataID, attrDataMap);
                    m_platformMap.insert(p.m_platformDataID, p);
                }
                // 事件(Event)等其他类型数据.tag为了兼容在线数据，也可能是其他类型
                if(lineData.startsWith("GenericData"))
                {
                    // 时间数据中可能存在空格无法直接按照空格来分割数据
                    QStringList eventDataList = parsePlatformData(lineData);
                    if(eventDataList.size() != 6)
                    {
                        continue;
                    }
                    GenericData g;
                    g.m_eventType = eventDataList[2].remove('\"');
                    g.m_eventName = eventDataList.at(3);
                    g.m_eventName = g.m_eventName.remove("\"").trimmed();
                    g.m_relativeTime = OrdinalTimeFormatter::getSecondsFromTimeStr(
                        eventDataList.at(4).trimmed(), m_refYear);
                    g.m_timeOffset = eventDataList.at(5).trimmed().toInt();

                    if(m_genericMap.contains(p.m_platformDataID) &&
                       m_genericMap.value(p.m_platformDataID).contains(g.m_eventType))
                    {
                        // 直接通过引用修改值
                        m_genericMap[p.m_platformDataID][g.m_eventType].append(g);
                    }
                    else if(m_genericMap.contains(p.m_platformDataID) &&
                            !m_genericMap.value(p.m_platformDataID).contains(g.m_eventType))
                    {
                        QList<GenericData> tmpList;
                        tmpList.append(g);
                        m_genericMap[p.m_platformDataID].insert(g.m_eventType, tmpList);
                    }
                    else
                    {
                        QMap<QString, QList<GenericData>> tmpMap;
                        tmpMap.insert(g.m_eventType, QList<GenericData>() << g);
                        m_genericMap.insert(p.m_platformDataID, tmpMap);
                    }
                }
            }
        }
    }
    file.close();
    if(m_timeDataSet.size() >= 1)
    {
        m_minTime = *m_timeDataSet.cbegin();
        m_maxTime = *m_timeDataSet.rbegin();
    }

    m_dataFileName = asiFileName;
}

const QMap<int32_t, QHash<QString, QVector<double>>>& DataManagerData::getDataMap()
{
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    return m_newEntityDataMap;
}

void DataManagerData::getMinMaxTime(double& minTime, double& maxTime)
{
    minTime = m_minTime;
    maxTime = m_maxTime;
}

void DataManagerData::getMinMaxRealTime(double& minTime, double& maxTime)
{
    minTime = m_minRealTime;
    maxTime = m_maxRealTime;
}

int DataManagerData::getRefYear()
{
    return m_refYear;
}

QVector<double>
DataManagerData::getEntityAttrValueList(int32_t entityID, const QString& attr, double rate)
{
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    QVector<double> resultValueVec;
    if(m_newEntityDataMap.contains(entityID) && m_newEntityDataMap.value(entityID).contains(attr))
    {
        QVector<double> valueVec = m_newEntityDataMap.value(entityID).value(attr);
        int32_t valueSize = valueVec.size();
        resultValueVec.resize(valueSize);
        for(int32_t i = 0; i < valueSize; ++i)
        {
            resultValueVec[i] = valueVec.at(i) * rate;
        }
    }
    return resultValueVec;
}

bool DataManagerData::entityDataIsEmpty()
{
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    return m_newEntityDataMap.isEmpty();
}

QVector<double> DataManagerData::getEntityAttrValueListByMaxTime(int32_t entityID,
                                                                 const QString& attr,
                                                                 double secs,
                                                                 double rate)
{
    int index = getEntityAttrMaxIndexByTime(entityID, secs);
    QVector<double> valueList;
    if(index >= 0)
    {
        valueList = getEntityAttrValueList(entityID, attr, rate);
        // length = index +1
        valueList = valueList.mid(0, index + 1);
    }
    return valueList;
}

QVector<double> DataManagerData::getEntityAttrValueListByMaxTimeAndIniFile(int32_t entityID,
                                                                           const QString& attr,
                                                                           double secs,
                                                                           double rate)
{
    int index = getEntityAttrMaxIndexByTime(entityID, secs);
    QVector<double> valueList;
    if(index >= 0)
    {
        valueList = getEntityAttrValueList(entityID, attr, rate);
        int limitIndex = getEntityAttrMaxIndexByTime(entityID, secs - m_timeLimit);
        int nStart = 0;
        int len = 0;

        if(m_timeLimit == 0)
        {
            nStart = 0;
            len = index + 1;
        }
        else
        {
            if(limitIndex == -1)
            {
                // 无限制，显示所有数据
                nStart = 0;
                len = index + 1;
            }
            else
            {
                nStart = limitIndex;
                len = index + 1 - limitIndex;
            }
        }

        // length = index +1
        valueList = valueList.mid(nStart, len);
    }
    return valueList;
}

double DataManagerData::getEntityAttrValueByMaxTime(int32_t entityID,
                                                    const QString& attr,
                                                    double secs,
                                                    double rate)
{
    int index = getEntityAttrMaxIndexByTime(entityID, secs);
    QVector<double> valueList;
    if(index >= 0)
    {
        valueList = getEntityAttrValueList(entityID, attr, rate);
        // length = index +1
        valueList = valueList.mid(0, index + 1);
    }
    return (valueList.size() == 0) ? std::numeric_limits<double>::max() : valueList.last();
}

QPair<double, double>
DataManagerData::getMaxAndMinEntityAttrValue(int32_t entityID, const QString& attr, double rate)
{
    QVector<double> valueList = getEntityAttrValueList(entityID, attr, rate);
    if(valueList.isEmpty())
    {
        return QPair<double, double>(std::numeric_limits<double>::min(),
                                     std::numeric_limits<double>::max());
    }
    else
    {
        double min = valueList.at(0);
        double max = valueList.at(0);
        for(int i = 0; i < valueList.size(); i++)
        {
            min = min < valueList.at(i) ? min : valueList.at(i);
            max = max > valueList.at(i) ? max : valueList.at(i);
        }
        return QPair<double, double>(min, max);
    }
}

QVector<double> DataManagerData::getTimeDataSet()
{
    // 为了方便外部使用，统一转成QVector返回值
    std::vector<double> vec(m_timeDataSet.begin(), m_timeDataSet.end());

    return QVector<double>::fromStdVector(vec);
}

QPair<QVector<double>, QVector<double>> DataManagerData::getSliceDataByTime(int32_t entityID,
                                                                            double secs)
{
    // 将与secs时间相等的数据全部返回
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    QPair<QVector<double>, QVector<double>> dataListPair;
    if(m_newEntityDataMap.contains(entityID))
    {
        auto attrDataMap = m_newEntityDataMap.value(entityID);
        QVector<double> timeList = attrDataMap.value(QString("Time"));
        if(timeList.isEmpty())
        {
            return dataListPair;
        }
        QVector<double> rangeList;
        QVector<double> voltageList;
        int32_t timeSize = timeList.count();
        for(int index = 0; index < timeSize; ++index)
        {
            double time = timeList.at(index);
            if(math::doubleEqual(time, secs))
            {
                double range = attrDataMap.value(QString("Range")).at(index);
                double voltage = attrDataMap.value(QString("Voltage")).at(index);
                rangeList.append(range);
                voltageList.append(voltage);
            }
            else
            {
                // 为了减少循环次数，暂定时间信息都是排序好的，重复的时间数据都在一起
                if(!rangeList.isEmpty())
                {
                    break;
                }
            }
        }
        dataListPair = qMakePair(rangeList, voltageList);
    }
    return dataListPair;
}

void DataManagerData::getRTIDataByTime(int32_t entityID,
                                       double secs,
                                       QVector<double>& rangeList,
                                       QVector<double>& timeList,
                                       QHash<QPair<int32_t, int32_t>, double>& valueMap)
{
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    rangeList.clear();
    timeList.clear();
    valueMap.clear();
    if(m_newEntityDataMap.contains(entityID))
    {
        auto attrDataMap = m_newEntityDataMap.value(entityID);
        // 没有去重的原始数据
        rangeList = attrDataMap.value(QString("Range"));
        //去重前需要排序
        std::sort(rangeList.begin(), rangeList.end());
        //去除容器内重复元素
        auto it = std::unique(rangeList.begin(), rangeList.end());
        rangeList.erase(it, rangeList.end());

        timeList = attrDataMap.value(QString("Time"));
        std::sort(timeList.begin(), timeList.end());
        auto it1 = std::unique(timeList.begin(), timeList.end());
        timeList.erase(it1, timeList.end());

        QVector<double> volList = attrDataMap.value(QString("Voltage"));
        int32_t rangeSize = rangeList.size();
        int32_t timeSize = timeList.size();
        for(int x = 0; x < rangeSize; ++x)
        {
            for(int y = 0; y < timeSize; ++y)
            {
                if(timeList.at(y) > secs)
                {
                    continue;
                }
                valueMap.insert(qMakePair(x, y), volList.at(x * timeSize + y));
            }
        }
    }
}
void DataManagerData::getDopplerDataByTime(
    int32_t entityID,
    double secs,
    QVector<double>& rangeList,
    QVector<double>& timeList,
    QHash<QPair<int32_t, int32_t>, double>& valueHash,
    QMultiHash<double, QPair<double, double>>& horizonDataHash,
    QMultiHash<double, QPair<double, double>>& verticalDataHash)
{
    std::shared_lock<std::shared_mutex> lck(m_entityMutex);
    rangeList.clear();
    timeList.clear();
    valueHash.clear();
    horizonDataHash.clear();
    verticalDataHash.clear();

    if(m_newEntityDataMap.contains(entityID))
    {
        auto attrDataMap = m_newEntityDataMap.value(entityID);
        // 没有去重的原始数据
        rangeList = attrDataMap.value(QString("Range"));
        //去重前需要排序
        std::sort(rangeList.begin(), rangeList.end());
        //去除容器内重复元素
        auto it = std::unique(rangeList.begin(), rangeList.end());
        rangeList.erase(it, rangeList.end());

        timeList = attrDataMap.value(QString("Time"));
        std::sort(timeList.begin(), timeList.end());
        auto it1 = std::unique(timeList.begin(), timeList.end());
        timeList.erase(it1, timeList.end());

        QVector<double> volList = attrDataMap.value(QString("Voltage"));
        int32_t rangeSize = rangeList.size();
        int32_t timeSize = timeList.size();
        for(int x = 0; x < rangeSize; ++x)
        {
            double range = rangeList.at(x);
            for(int y = 0; y < timeSize; ++y)
            {
                double time = timeList.at(y);
                if(time > secs)
                {
                    continue;
                }
                double voltage = volList.at(x * timeSize + y);
                valueHash.insert(qMakePair(x, y), voltage);
                horizonDataHash.insert(time, qMakePair(range, voltage));
                verticalDataHash.insert(range, qMakePair(time, voltage));
            }
        }
    }
}

QString DataManagerData::getEntityNameByID(int32_t id)
{
    if(m_platformMap.contains(id))
    {
        return m_platformMap.value(id).m_platformName;
    }
    return "";
}

QString DataManagerData::getEntityIconByID(int32_t id)
{
    if(m_platformMap.contains(id))
    {
        return m_platformMap.value(id).m_platformIcon;
    }
    return "";
}

QStringList DataManagerData::getEntityNameList()
{
    QStringList list;
    for(const auto& p : m_platformMap)
    {
        list.append(p.m_platformName);
    }

    return list;
}

QList<QPair<QString, QString>> DataManagerData::getAttrAndUnitPairList(int32_t id)
{
    QList<QPair<QString, QString>> list;
    list = m_attrUnitHash.value(id);
    list.removeOne(qMakePair(QString("Time"), QString("sec")));
    return list;
}

QMap<int32_t, QString> DataManagerData::getEntityIDAndNameMap()
{
    QMap<int32_t, QString> map;
    for(const auto& p : m_platformMap)
    {
        map.insert(p.m_platformDataID, p.m_platformName);
    }
    return map;
}

QList<GenericData> DataManagerData::getGenericDataListByID(int32_t entityID,
                                                           const QString& tag,
                                                           const int32_t maxCount)
{
    std::shared_lock<std::shared_mutex> lock(m_eventMutex);
    QList<GenericData> tags;
    if(m_genericMap.contains(entityID))
    {
        tags = m_genericMap.value(entityID).value(tag);
        if(maxCount > 0)
        {
            int32_t size = tags.size();
            if(size > maxCount)
            {
                tags = tags.mid(size - maxCount);
            }
        }
    }
    return tags;
}

QStringList DataManagerData::getGenericDataTagsByID(int32_t entityID)
{
    std::shared_lock<std::shared_mutex> lock(m_eventMutex);
    if(m_genericMap.contains(entityID))
        return m_genericMap.value(entityID).keys();

    return QStringList();
}

double DataManagerData::rangeCalculationLastValue(int32_t sourceID,
                                                  int32_t targetID,
                                                  DataPair::RangeCalculationType type,
                                                  double secs,
                                                  double rate)
{
    double value = std::numeric_limits<double>::max();
    if(type == DataPair::RelativeAltitude)
    {
        double sourceAltitude = getEntityAttrValueByMaxTime(sourceID, "Alt", secs, rate);
        double targetAltitude = getEntityAttrValueByMaxTime(targetID, "Alt", secs, rate);
        if(math::doubleEqual(sourceAltitude, std::numeric_limits<double>::max()))
        {
            sourceAltitude = 0.0;
        }
        if(math::doubleEqual(targetAltitude, std::numeric_limits<double>::max()))
        {
            targetAltitude = 0.0;
        }
        value = sourceAltitude - targetAltitude;
    }
    return value;
}

QVector<double> DataManagerData::rangeCalculationValueList(int32_t sourceID,
                                                           int32_t targetID,
                                                           DataPair::RangeCalculationType type,
                                                           double secs,
                                                           double rate)
{
    QVector<double> valueList;
    if(type == DataPair::RelativeAltitude)
    {
        QVector<double> sourceAltitude =
            getEntityAttrValueListByMaxTime(sourceID, "Alt", secs, rate);
        QVector<double> targetAltitude =
            getEntityAttrValueListByMaxTime(targetID, "Alt", secs, rate);
        /*
         * 两个目标实体的属性数量可能不一致，以sourceID对应的实体为准，因为当另外一个轴为Time类型时
         * time的属性数量是从sourceID实体中获取，为了保证两个轴数据数量一致，这里如果targetID中的属性多，
         * 那么丢掉多余的，如果少，那么用0填充
        */
        int32_t sourceSize = sourceAltitude.size();
        int32_t targetSize = targetAltitude.size();
        if(sourceSize > targetSize)
        {
            targetAltitude.insert(targetAltitude.begin(), sourceSize - targetSize, 0.0);
        }
        if(sourceSize < targetSize)
        {
            targetAltitude.remove(0, targetSize - sourceSize);
        }
        valueList.resize(sourceSize);
        for(int32_t i = 0; i < sourceSize; ++i)
        {
            valueList[i] = sourceAltitude.at(i) - targetAltitude.at(i);
        }
    }
    else if(type == DataPair::GroundRng)
    {
        QVector<double> sourceLon = getEntityAttrValueListByMaxTime(sourceID, "Lon", secs, rate);
        QVector<double> targetLon = getEntityAttrValueListByMaxTime(targetID, "Lon", secs, rate);
        QVector<double> sourceLat = getEntityAttrValueListByMaxTime(sourceID, "Lat", secs, rate);
        QVector<double> targetLat = getEntityAttrValueListByMaxTime(targetID, "Lat", secs, rate);
        int32_t size = qMin(sourceLon.size(), targetLon.size());
        valueList.resize(size);
        for(int32_t i = 0; i < size; ++i)
        {
            valueList[i] = math::realDistance(
                sourceLat.at(i), sourceLon.at(i), targetLat.at(i), targetLon.at(i));
        }
    }
    return valueList;
}

bool DataManagerData::isEntityContainsGenericTags(int32_t id)
{
    std::shared_lock<std::shared_mutex> lock(m_eventMutex);
    return m_genericMap.contains(id);
}

int32_t getIndexInVector(const QVector<double> vec, double element)
{
    int32_t left = 0;
    int32_t right = vec.size() - 1;
    while(left <= right)
    {
        int32_t mid = left + (right - left) / 2;
        if(math::doubleEqual(vec.at(mid), element))
        {
            return mid;
        }
        else if(vec.at(mid) < element)
        {
            left = mid + 1;
        }
        else
        {
            right = mid - 1;
        }
    }
    return left;
}

int DataManagerData::getEntityAttrMaxIndexByTime(int32_t entityID, double secs)
{
    int index = 0;
    QVector<double> timeList = getEntityAttrValueList(entityID, "Time");
    if(!timeList.isEmpty())
    {
        index = getIndexInVector(timeList, secs);
    }
    return (index - 1);
}

QStringList DataManagerData::parsePlatformData(const QString& data)
{
    QStringList list;
    // 定义正则表达式
    QRegularExpression re("\".*?\"|\\S+");

    // 在字符串中查找匹配项
    QRegularExpressionMatchIterator matchIterator = re.globalMatch(data);
    while(matchIterator.hasNext())
    {
        QRegularExpressionMatch match = matchIterator.next();
        QString element = match.captured(0).trimmed();
        list.append(element);
    }
    return list;
}

int32_t DataManagerData::findIDByName(const QString& name)
{
    QList<int32_t> lst = m_platformMap.keys();
    for(int i = 0; i < lst.size(); i++)
    {
        if(name == m_platformMap[lst.at(i)].m_platformName)
        {
            return lst.at(i);
        }
    }
    return -1;
}

QString DataManagerData::getGroupNameByID(int32_t id)
{
    QString idStr = QString::number(id);
    QStringList typeList = m_realEventTypeInfo.keys();
    for(const QString& type : typeList)
    {
        QStringList idList = m_realEventTypeInfo.value(type);
        if(idList.contains(idStr))
        {
            return type;
        }
    }
    return QString();
}

QString DataManagerData::getUnitByAttr(int32_t id, const QString& attr)
{
    auto unitlist = m_attrUnitHash.value(id);
    for(auto pair : unitlist)
    {
        QString first = pair.first;
        if(first == attr)
            return pair.second;
    }
    return QString();
}

void DataManagerData::insertGenericData(const GenericData& generic)
{
    std::unique_lock<std::shared_mutex> lock(m_eventMutex);
    int32_t uID = -1;
    if(generic.m_ID != -1)
    {
        uID = generic.m_ID;
    }
    else if(!generic.m_platName.isEmpty())
    {
        uID = findIDByName(generic.m_platName);
    }

    if(uID == -1)
    {
        return;
    }

    if(!m_genericMap.contains(uID))
    {
        m_genericMap.insert(uID, QMap<QString, QList<GenericData>>());
    }

    m_genericMap[uID][generic.m_eventType].append(generic);
}

void DataManagerData::insertProtobufPlatinfoData(const USIM_PlatInfoMessage_Proto& plat)
{
    std::unique_lock<std::shared_mutex> lock(m_entityMutex);
    int32_t uID = int32_t(plat.uid());

    if(!m_newEntityDataMap.contains(uID))
    {
        QList<QPair<QString, QString>> attrUnitList;
        attrUnitList.append(QPair<QString, QString>("Time", "sec"));
        attrUnitList.append(QPair<QString, QString>("Fuel", "pound"));
        attrUnitList.append(QPair<QString, QString>("Life", "NA"));
        attrUnitList.append(QPair<QString, QString>("Lon", "deg"));
        attrUnitList.append(QPair<QString, QString>("Lat", "deg"));
        attrUnitList.append(QPair<QString, QString>("Alt", "m"));
        attrUnitList.append(QPair<QString, QString>("Heading", "deg"));
        attrUnitList.append(QPair<QString, QString>("Vx", "m/sec"));
        attrUnitList.append(QPair<QString, QString>("Vy", "m/sec"));
        attrUnitList.append(QPair<QString, QString>("Vz", "m/sec"));
        attrUnitList.append(QPair<QString, QString>("Yaw", "deg"));
        attrUnitList.append(QPair<QString, QString>("Pitch", "deg"));
        attrUnitList.append(QPair<QString, QString>("Roll", "deg"));
        attrUnitList.append(QPair<QString, QString>("Swaying", "m"));
        attrUnitList.append(QPair<QString, QString>("Surging", "m"));
        attrUnitList.append(QPair<QString, QString>("Heaving", "m"));
        m_attrUnitHash.insert(uID, attrUnitList);

        // 在线数据中多余的信息基本没用，可以跟离线数据使用同一个平台结构
        Platform p;
        p.m_platformDataID = plat.uid();
        p.m_platformName = QString::fromLocal8Bit(plat.splatname().data());
        m_platformMap.insert(plat.uid(), p);
    }

    QHash<QString, QVector<double>> dataMap = m_newEntityDataMap[uID];
    dataMap["Time"].append(plat.dfsimtime() * 3600);
    dataMap["Fuel"].append(plat.dffuel());
    dataMap["Life"].append(plat.dflife());
    dataMap["Lon"].append(plat.dflon());
    dataMap["Lat"].append(plat.dflat());
    dataMap["Alt"].append(plat.dfalt());
    dataMap["Heading"].append(plat.dfheading());
    dataMap["Vx"].append(plat.dfvx());
    dataMap["Vy"].append(plat.dfvy());
    dataMap["Vz"].append(plat.df());
    dataMap["Yaw"].append(plat.dfyaw());
    dataMap["Pitch"].append(plat.pitch());
    dataMap["Roll"].append(plat.roll());
    dataMap["Swaying"].append(plat.dfswaying());
    dataMap["Surging"].append(plat.surging());
    dataMap["Heaving"].append(plat.heaving());
    m_newEntityDataMap.insert(uID, dataMap);
    m_minRealTime = dataMap["Time"].at(0);
    m_maxRealTime = plat.dfsimtime() * 3600;
    // 暂时默认参考时间为当前年份，在线数据没有提供
    m_refYear = QDate::currentDate().year();
}

void DataManagerData::loadConfig()
{
    // 只有在每次开始获取在线数据时更新配置文件信息
    QString iniFileName = QCoreApplication::applicationDirPath() + "/PlotXY.ini";
    if(!QFile::exists(iniFileName))
    {
        qCritical() << "File not exist";
        return;
    }
    m_realEventTypeInfo.clear();
    QSettings settings(iniFileName, QSettings::IniFormat);
    settings.setIniCodec(QTextCodec::codecForName("utf-8"));

    m_timeLimit = settings.value("DataConfig/TimeLimit").toInt();

    QStringList groupNames = settings.childGroups();
    foreach(const QString& groupName, groupNames)
    {
        if(!groupName.contains("Event"))
        {
            continue;
        }
        settings.beginGroup(groupName);
        QString type = settings.value("TypeName").toString();
        QStringList list = settings.value("EventList").toStringList();
        m_realEventTypeInfo.insert(type, list);
        settings.endGroup();
    }
}

QString DataManagerData::getDataFileName() const
{
    return m_dataFileName;
}

void DataManagerData::setDataFileName(const QString& dataFileName)
{
    m_dataFileName = dataFileName;
}

void DataManagerData::clearData()
{
    std::unique_lock<std::shared_mutex> lck(m_entityMutex);
    m_newEntityDataMap.clear();
    m_platformMap.clear();
    m_genericMap.clear();
    m_timeDataSet.clear();
    m_gogFileList.clear();
    m_dataFileName.clear();
}
