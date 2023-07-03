/* * @filename: DataManager.cpp
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include <QApplication>
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>
#include <QTextCodec>

#include <iostream>
#include <limits>

#include "TimeClass.h"
#include "Utils.h"

DataManager::DataManager()
    : m_isRealTime(false)
{
    m_recvThread = new recvThread;
    connect(m_recvThread, &recvThread::genericReceived, this, &DataManager::onRecvGenericData);
    connect(m_recvThread,
            &recvThread::protobufPlatInfoReceived,
            this,
            &DataManager::onRecvProtobufPlatinfoData);
}

DataManager::~DataManager() {}

bool DataManager::getIsRealTime() const
{
    return m_isRealTime;
}

bool DataManager::saveDataToASI(const QString& asiFileName)
{
    QFile file(asiFileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qDebug() << "DataManager::saveDataToASI read file failure.";
        return false;
    }
    QMap<int32_t, QHash<QString, QVector<double>>> dataMap;
    if(m_isRealTime)
    {
        dataMap = m_realDataMap;
    }
    else
    {
        dataMap = m_newEntityDataMap;
    }
    auto idList = dataMap.keys();
    if(idList.isEmpty())
    {
        file.close();
        file.remove();
        return false;
    }
    QTextStream stream(&file);
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
        auto platDataHash = dataMap.value(id);
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

        QMap<int32_t, QMap<QString, QList<GenericData>>> genericMap;
        if(m_isRealTime)
        {
            genericMap = m_realGenericMap;
        }
        else
        {
            genericMap = m_genericMap;
        }
        if(genericMap.contains(id))
        {
            auto tags = genericMap.value(id).keys();
            auto genericDataMap = genericMap.value(id);
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

void DataManager::loadFileData(const QString& filename)
{
    QFileInfo info(filename);
    if(!info.exists())
    {
        QMessageBox::warning(nullptr, "警告", "文件路径不存在");
        return;
    }
    QString suffix = info.suffix().toLower();
    if(suffix == "csv")
    {
        loadCSVData(filename);
    }
    else if(suffix == "asi")
    {
        loadASIData(filename);
    }
    else
    {
        QMessageBox::warning(nullptr, "警告", "不支持的文件类型");
        return;
    }
    if(m_newEntityDataMap.isEmpty())
    {
        QMessageBox::information(
            nullptr, QString("提示信息"), QString("数据加载失败,当前数据为空"));
    }
    else
    {

        QMessageBox::information(nullptr, QString("提示信息"), QString("已成功加载数据"));
        emit loadDataFinished();
    }
}

void DataManager::loadCSVData(const QString& filePath)
{

	QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "DataManager::loadCSV read file failure." << endl;
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

void DataManager::loadASIData(const QString& asiFileName)
{
    QFile file(asiFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataManager::loadASIData read file failure.";
        return;
    }
    m_newEntityDataMap.clear();
    m_attrUnitHash.clear();
    m_timeDataSet.clear();
    m_platformMap.clear();
    m_genericMap.clear();
    m_gogFileList.clear();

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
                    if(lineData.contains("Event"))
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
    }
    file.close();
    if(m_timeDataSet.size() >= 1)
    {
        m_minTime = *m_timeDataSet.cbegin();
        m_maxTime = *m_timeDataSet.rbegin();
    }

    m_dataFileName = asiFileName;
}

const QMap<int32_t, QHash<QString, QVector<double>>>& DataManager::getDataMap()
{
    if(m_isRealTime)
        return m_realDataMap;
    else
        return m_newEntityDataMap;
}

void DataManager::getMinMaxTime(double& minTime, double& maxTime)
{
    minTime = m_minTime;
    maxTime = m_maxTime;
}

void DataManager::getMinMaxRealTime(double& minTime, double& maxTime)
{
    minTime = m_minRealTime;
    maxTime = m_maxRealTime;
}

int DataManager::getRefYear()
{
    return m_refYear;
}

QVector<double> DataManager::getEntityAttrValueList(int32_t entityID, const QString& attr)
{
    QVector<double> valueList;
    if(m_isRealTime)
    {
        if(m_realDataMap.contains(entityID) && m_realDataMap.value(entityID).contains(attr))
        {
            return m_realDataMap.value(entityID).value(attr);
        }
    }
    else if(m_newEntityDataMap.contains(entityID) &&
            m_newEntityDataMap.value(entityID).contains(attr))
    {
        return m_newEntityDataMap.value(entityID).value(attr);
    }
	return valueList;
}

QVector<double>
DataManager::getEntityAttrValueListByMaxTime(int32_t entityID, const QString& attr, double secs)
{
    int index = getEntityAttrMaxIndexByTime(entityID, secs);
    QVector<double> valueList;
    if(index >= 0)
	{
        valueList = getEntityAttrValueList(entityID, attr);
        // length = index +1
        valueList = valueList.mid(0, index + 1);
	}
    return valueList;
}

double DataManager::getEntityAttrValueByMaxTime(int32_t entityID, const QString& attr, double secs)
{
    int index = getEntityAttrMaxIndexByTime(entityID, secs);
    QVector<double> valueList;
    if(index >= 0)
    {
        valueList = getEntityAttrValueList(entityID, attr);
        // length = index +1
        valueList = valueList.mid(0, index + 1);
    }
    return (valueList.size() == 0) ? std::numeric_limits<double>::max() : valueList.last();
}

QPair<double, double> DataManager::getMaxAndMinEntityAttrValue(int32_t entityID,
                                                               const QString& attr)
{
    QVector<double> valueList = getEntityAttrValueList(entityID, attr);
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

QVector<double> DataManager::getTimeDataSet()
{
    // 为了方便外部使用，统一转成QVector返回值
    std::vector<double> vec(m_timeDataSet.begin(), m_timeDataSet.end());

    return QVector<double>::fromStdVector(vec);
}

QPair<QVector<double>, QVector<double>> DataManager::getSliceDataByTime(int32_t entityID,
                                                                        double secs)
{
    // 将与secs时间相等的数据全部返回
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

void DataManager::getRTIDataByTime(int32_t entityID,
                                   double secs,
                                   QVector<double>& rangeList,
                                   QVector<double>& timeList,
                                   QHash<QPair<int32_t, int32_t>, double>& valueMap)
{
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
void DataManager::getDopplerDataByTime(int32_t entityID,
                                       double secs,
                                       QVector<double>& rangeList,
                                       QVector<double>& timeList,
                                       QHash<QPair<int32_t, int32_t>, double>& valueHash,
                                       QMultiHash<double, QPair<double, double>>& horizonDataHash,
                                       QMultiHash<double, QPair<double, double>>& verticalDataHash)
{

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

QString DataManager::getEntityNameByID(int32_t id)
{
    if(m_isRealTime)
    {
        if(m_protobufPlatformMap.contains(id))
        {
            return m_protobufPlatformMap.value(id).name;
        }
    }
    else if(m_platformMap.contains(id))
    {
        return m_platformMap.value(id).m_platformName;
    }
    return "";
}

QString DataManager::getEntityIconByID(int32_t id)
{
    if(m_isRealTime)
    {
        return "";
    }
    else if(m_platformMap.contains(id))
    {
        return m_platformMap.value(id).m_platformIcon;
    }
    return "";
}

QStringList DataManager::getEntityNameList()
{
    QStringList list;
    if(m_isRealTime)
    {
        for(const auto& p : m_protobufPlatformMap)
        {
            list.append(p.name);
        }
    }
    else
    {
        for(const auto& p : m_platformMap)
        {
            list.append(p.m_platformName);
        }
    }
    return list;
}

QList<QPair<QString, QString>> DataManager::getAttrAndUnitPairList(int32_t id)
{
    QList<QPair<QString, QString>> list;
    if(m_isRealTime)
    {
        list = m_realUnitHash.value(id);
    }
    else
    {
        list = m_attrUnitHash.value(id);
    }
    list.removeOne(qMakePair(QString("Time"), QString("sec")));
    return list;
}

QMap<int32_t, QString> DataManager::getEntityIDAndNameMap()
{
    QMap<int32_t, QString> map;
    if(m_isRealTime)
    {
        for(const auto& p : m_protobufPlatformMap)
        {
            map.insert(p.platformID, p.name);
        }
    }
    else
    {
        for(const auto& p : m_platformMap)
        {
            map.insert(p.m_platformDataID, p.m_platformName);
        }
    }
    return map;
}

QList<GenericData> DataManager::getGenericDataListByID(int32_t entityID, const QString& tag)
{
    QList<GenericData> tags;
    if(m_isRealTime)
    {
        if(m_realGenericMap.contains(entityID))
            tags = m_realGenericMap.value(entityID).value(tag);
    }
    else
    {
        if(m_genericMap.contains(entityID))
            tags = m_genericMap.value(entityID).value(tag);
    }
    return tags;
}

QStringList DataManager::getGenericDataTagsByID(int32_t entityID)
{
    if(m_isRealTime)
    {
        if(m_realGenericMap.contains(entityID))
            return m_realGenericMap.value(entityID).keys();
    }
    else
    {
        if(m_genericMap.contains(entityID))
            return m_genericMap.value(entityID).keys();
    }
    return QStringList();
}

bool DataManager::isEntityContainsGenericTags(int32_t id)
{
    if(m_isRealTime)
    {
        return m_realGenericMap.contains(id);
    }
    else
    {
        return m_genericMap.contains(id);
    }
}

int DataManager::getEntityAttrMaxIndexByTime(int32_t entityID, double secs)
{
	int index = 0;
    QVector<double> timeList = getEntityAttrValueList(entityID, "Time");
    if(!timeList.isEmpty())
	{
        for(index = 0; index < timeList.size(); ++index)
		{
            if(timeList.at(index) > secs)
			{
				break;
			}
		}
	}
    return (index - 1);
}

QStringList DataManager::parsePlatformData(const QString& data)
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

int32_t DataManager::findIDByName(const QString& name)
{
    QList<int32_t> lst = m_protobufPlatformMap.keys();
    for(int i = 0; i < lst.size(); i++)
    {
        if(name == m_protobufPlatformMap[lst.at(i)].name)
        {
            return lst.at(i);
        }
    }
    return -1;
}

QString DataManager::getGroupNameByID(int32_t id)
{
    QStringList groupNames = m_settings->childGroups();
    foreach(QString groupName, groupNames)
    {
        QString newGroup = QString::fromUtf8(groupName.toLatin1());

        m_settings->beginGroup(groupName);

        QStringList keys = m_settings->allKeys();
        foreach(QString key, keys)
        {
            QStringList value = m_settings->value(key).toStringList();
            if(value.contains(QString::number(id)))
                return newGroup;
        }
        m_settings->endGroup();
    }
    return QString();
}

void DataManager::onRecvPlatinfoData(const MARS_PlatInfoDataExcect& plat)
{
    int32_t uID = int32_t(plat.uID);
    if(!m_realDataMap.contains(uID))
    {
        QHash<QString, QVector<double>> dataMap = m_realDataMap[uID];
        dataMap["Time"].append(plat.time * 3600);
        dataMap["Fuel"].append(plat.fuel);
        dataMap["Dammager"].append(plat.damageper);
        dataMap["Speed"].append(double(plat.Speed));
        dataMap["Bearing"].append(double(plat.Bearing));
        dataMap["Lat"].append(plat.Lat);
        dataMap["Lon"].append(plat.Lng);
        dataMap["Alt"].append(plat.Alt);
        dataMap["CW"].append(plat.Cw);
        dataMap["Pitch"].append(plat.pitch);
        dataMap["Roll"].append(plat.roll);
        dataMap["MaxSpeed"].append(plat.maxSpeed);
        dataMap["ConNUm"].append(plat.iIconNum);
        dataMap["Visible"].append(plat.iVisible);
        m_realDataMap.insert(uID, dataMap);
        m_minRealTime = dataMap["Time"].at(0);
        m_maxRealTime = plat.time * 3600;
    }

    if(!m_realUnitHash.contains(uID))
    {
        QList<QPair<QString, QString>> attrUnitList;
        attrUnitList.append(QPair<QString, QString>("Time", "sec"));
        attrUnitList.append(QPair<QString, QString>("Fuel", "sec"));
        attrUnitList.append(QPair<QString, QString>("Dammager", "na"));
        attrUnitList.append(QPair<QString, QString>("Speed", "m/sec"));
        attrUnitList.append(QPair<QString, QString>("Bearing", "na"));
        attrUnitList.append(QPair<QString, QString>("Lat", "deg"));
        attrUnitList.append(QPair<QString, QString>("Lon", "deg"));
        attrUnitList.append(QPair<QString, QString>("Alt", "m"));
        attrUnitList.append(QPair<QString, QString>("CW", "na"));
        attrUnitList.append(QPair<QString, QString>("Pitch", "deg"));
        attrUnitList.append(QPair<QString, QString>("Roll", "deg"));
        attrUnitList.append(QPair<QString, QString>("MaxSpeed", "m/sec"));
        attrUnitList.append(QPair<QString, QString>("ConNUm", "na"));
        attrUnitList.append(QPair<QString, QString>("Visible", "na"));
        m_realUnitHash.insert(uID, attrUnitList);
    }

    RealPlatform realPlatform;
    realPlatform.platformDataID = plat.uID;
    realPlatform.platformName = QString::fromLocal8Bit(plat.PlatName);
    realPlatform.OpsStatus = plat.OpsStatus;
    realPlatform.Alliance = plat.Alliance;
    realPlatform.Operation_medium = plat.Operating_medium;
    realPlatform.Icon_Type = plat.Icon_type;
    realPlatform.basePlatName = plat.BasePlatName;
    realPlatform.hullName = plat.HullName;
    realPlatform.cStandBy = plat.cStandBy;
    emit updateRealTime();
}

void DataManager::onRecvGenericData(const GenericData& generic)
{
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
    if(!m_realGenericMap.contains(uID))
    {
        m_realGenericMap.insert(uID, QMap<QString, QList<GenericData>>());
    }

    m_realGenericMap[uID][generic.m_eventType].append(generic);
    emit updateRealTime();
}

void DataManager::onRecvProtobufPlatinfoData(const USIM_PlatInfoMessage_Proto& plat)
{
    int32_t uID = int32_t(plat.uid());
    if(!m_realDataMap.contains(uID))
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
        m_realUnitHash.insert(uID, attrUnitList);

        ProtobufPlatForm protoPlatform;
        protoPlatform.platformID = plat.uid();
        protoPlatform.name = QString::fromLocal8Bit(plat.splatname().data());
        protoPlatform.cmdNodeName = QString::fromLocal8Bit(plat.scommandnodename().data());
        protoPlatform.alliance = static_cast<USIM_Alliance>(plat.ualliance());
        protoPlatform.color = plat.ucolor();
        protoPlatform.kind = plat.ukind();
        protoPlatform.classType = plat.uclass();
        protoPlatform.typeName = QString::fromLocal8Bit(plat.stype().data());
        m_protobufPlatformMap.insert(uID, protoPlatform);
    }

    QHash<QString, QVector<double>> dataMap = m_realDataMap[uID];
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
    m_realDataMap.insert(uID, dataMap);
    m_minRealTime = dataMap["Time"].at(0);
    m_maxRealTime = plat.dfsimtime() * 3600;
    // 暂时默认参考时间为当前年份，在线数据没有提供
    m_refYear = QDate::currentDate().year();

    emit updateRealTime();
}

recvThread* DataManager::getRecvThread() const
{
    return m_recvThread;
}

void DataManager::setIsRealTime(bool isRealTime)
{
    m_isRealTime = isRealTime;

    if(m_isRealTime)
    {
        // 只有在每次开始获取在线数据时更新配置文件信息
        QString iniFileName = QCoreApplication::applicationDirPath() + "/PlotXY.ini";
        if(!QFile::exists(iniFileName))
        {
            qCritical() << "File not exist";
            return;
        }
        m_settings = new QSettings(iniFileName, QSettings::IniFormat);
        m_settings->setIniCodec(QTextCodec::codecForName("utf-8"));
    }
}

QString DataManager::getDataFileName() const
{
    return m_dataFileName;
}

void DataManager::setDataFileName(const QString& dataFileName)
{
    m_dataFileName = dataFileName;
}

void DataManager::clearData()
{
    m_newEntityDataMap.clear();
    m_realDataMap.clear();
    m_platformMap.clear();
    m_realPlatformMap.clear();
    m_protobufPlatformMap.clear();
    m_genericMap.clear();
    m_realGenericMap.clear();
    m_timeDataSet.clear();
    m_gogFileList.clear();
    m_dataFileName.clear();
}
