﻿/* * @filename: DataManager.cpp
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <QRegularExpression>

#include <limits>

#include "TimeClass.h"
#include "Utils.h"

DataManager::DataManager() {}

DataManager::~DataManager() {}

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

        QMessageBox::information(NULL, QString("提示信息"), QString("已成功加载数据"));
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
        qDebug() << "DataManager::loadCSV read file failure.";
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
                m_refLLAPoint.setX(list.at(1).toDouble());
                m_refLLAPoint.setY(list.at(2).toDouble());
                m_refLLAPoint.setZ(list.at(3).toDouble());
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
                QDir dir(asiFileName);
                // 将相对路径转化为绝对路径
                auto absFilePath = dir.absoluteFilePath(list.at(1));
                m_gogFileList.append(absFilePath);
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
                        p.m_platformIcon = list.at(2);
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

                                double value = 0.0;
                                /*
                                 * 常规数据直接转为double类型
                                 * Time数据有两种情况:
                                 * 1、直接就是double型的相对时间，无需转化
                                 * 2、001 2000 00:00:00 这种形式的数据，需要根据偏移时间转化为
                                 * 默认第三列数据就是Time数据
                                */
                                if(i == 2)
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
                // 事件(Event)等其他类型数据
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
                        g.m_name = eventDataList.at(3).trimmed();
                        g.m_relativeTime = OrdinalTimeFormatter::getSecondsFromTimeStr(
                            eventDataList.at(4).trimmed(), m_refYear);
                        g.m_timeOffset = eventDataList.at(5).trimmed().toInt();
                        if(m_genericMap.contains(p.m_platformDataID) &&
                           m_genericMap.value(p.m_platformDataID).contains("Event"))
                        {
                            // 直接通过引用修改值
                            m_genericMap[p.m_platformDataID]["Event"].append(g);
                        }
                        else
                        {
                            QMap<QString, QList<GenericData>> tmpMap;
                            tmpMap.insert("Event", QList<GenericData>() << g);
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
    return m_newEntityDataMap;
}

void DataManager::getMinMaxTime(double& minTime, double& maxTime)
{
	minTime = m_minTime;
	maxTime = m_maxTime;
}

int DataManager::getRefYear()
{
	return m_refYear;
}

QVector<double> DataManager::getEntityAttrValueList(int32_t entityID, const QString& attr)
{

    QVector<double> valueList;
    if(m_newEntityDataMap.contains(entityID) && m_newEntityDataMap.value(entityID).contains(attr))
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
    if(m_platformMap.contains(id))
    {
        return m_platformMap.value(id).m_platformName;
    }
    return "";
}

QStringList DataManager::getEntityNameList()
{
    QStringList list;
    for(const auto& p : m_platformMap)
    {
        list.append(p.m_platformName);
    }
    return list;
}

QList<QPair<QString, QString>> DataManager::getAttrAndUnitPairList(int32_t id)
{
    auto list = m_attrUnitHash.value(id);
    list.removeOne(qMakePair(QString("Time"), QString("sec")));
    return list;
}

QMap<int32_t, QString> DataManager::getEntityIDAndNameMap()
{
    QMap<int32_t, QString> map;
    for(const auto& p : m_platformMap)
    {
        map.insert(p.m_platformDataID, p.m_platformName);
    }
    return map;
}

QStringList DataManager::getGenericDataTagsByID(int32_t entityID)
{
    QStringList tags;
    if(m_genericMap.contains(entityID))
    {
        tags = m_genericMap.value(entityID).keys();
    }
    return tags;
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
    m_platformMap.clear();
    m_genericMap.clear();
    m_timeDataSet.clear();
    m_gogFileList.clear();
    m_dataFileName.clear();
}
