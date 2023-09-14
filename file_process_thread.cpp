#include "file_process_thread.h"
#include "TimeClass.h"
#include "data_manager_data.h"

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

FileProcessThread::FileProcessThread(TaskType taskType, QObject* parent)
    : QThread(parent)
    , m_taskType(taskType)
{}

void FileProcessThread::run()
{
    if(m_taskType == TaskType::LoadFile)
    {
        loadASIData(m_fileName);
    }
    else
    {
        DataManagerDataInstance->saveDataToASI(m_fileName);
    }
}

QString FileProcessThread::fileName() const
{
    return m_fileName;
}

void FileProcessThread::setFileName(const QString& fileName)
{
    m_fileName = fileName;
}

void FileProcessThread::loadASIData(const QString& asiFileName)
{
    QFile file(asiFileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "DataManagerData::loadASIData read file failure.";
        return;
    }
    // 清理数据
    DataManagerDataInstance->clearData();
    int64_t fileSize = file.size();
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
                DataManagerDataInstance->setRefLLAPoint(
                    QVector3D(list.at(1).toFloat(), list.at(2).toFloat(), list.at(3).toFloat()));
            }
        }
        if(lineData.startsWith("ReferenceYear"))
        {
            QStringList list = lineData.split(" ", QString::SkipEmptyParts);
            if(list.size() == 2)
            {
                DataManagerDataInstance->setRefYear(list.at(1).toInt());
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
                    DataManagerDataInstance->insertGOGFile(absFilePath);
                }
            }
        }

        // Platform属性起点
        if(lineData.startsWith("# Platform Keywords"))
        {
            DataManagerData::Platform p;

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
                    DataManagerDataInstance->insertAttrUnit(p.m_platformDataID, attrUnitList);
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
                                        tmpAttrValue, DataManagerDataInstance->getRefYear());
                                    // set自动去重和排序
                                    DataManagerDataInstance->insertTimeData(value);
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
                    DataManagerDataInstance->insertEntityData(p.m_platformDataID, attrDataMap);
                    DataManagerDataInstance->insertEntity(p.m_platformDataID, p);
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
                        eventDataList.at(4).trimmed(), DataManagerDataInstance->getRefYear());
                    g.m_timeOffset = eventDataList.at(5).trimmed().toInt();
                    DataManagerDataInstance->insertEventByID(p.m_platformDataID, g);
                    //                    auto genericMap = DataManagerDataInstance->getGenericMap();
                    //                    if(genericMap.contains(p.m_platformDataID) &&
                    //                       genericMap.value(p.m_platformDataID).contains(g.m_eventType))
                    //                    {

                    //                    }
                    //                    else if(genericMap.contains(p.m_platformDataID) &&
                    //                            !genericMap.value(p.m_platformDataID).contains(g.m_eventType))
                    //                    {
                    //                        QList<GenericData> tmpList;
                    //                        tmpList.append(g);
                    //                        m_genericMap[p.m_platformDataID].insert(g.m_eventType, tmpList);
                    //                    }
                    //                    else
                    //                    {
                    //                        QMap<QString, QList<GenericData>> tmpMap;
                    //                        tmpMap.insert(g.m_eventType, QList<GenericData>() << g);
                    //                        m_genericMap.insert(p.m_platformDataID, tmpMap);
                    //                    }
                }
            }
        }
        int64_t readSize = fileSize - file.bytesAvailable();
        int32_t progress = readSize / static_cast<double>(fileSize) * 100;
        emit fileReadProgress(progress);
    }
    file.close();
    auto timeDataSet = DataManagerDataInstance->getTimeDataSet();
    if(timeDataSet.size() >= 1)
    {
        double minTime = *timeDataSet.cbegin();
        double maxTime = *timeDataSet.rbegin();
        DataManagerDataInstance->setMinMaxTime(minTime, maxTime);
    }

    DataManagerDataInstance->setDataFileName(asiFileName);
}

QStringList FileProcessThread::parsePlatformData(const QString& data)
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
