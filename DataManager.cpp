/* * @filename: DataManager.cpp
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>

#include "TimeClass.h"

std::shared_ptr<DataManager> DataManager::m_pInstance = nullptr;
QMutex DataManager::m_mutex;

DataManager::DataManager()
{

}

DataManager::~DataManager()
{

}

void DataManager::loadCSV(const QString& filePath)
{
	if (filePath.isEmpty())
	{
		qDebug() << "DataManager::loadCSV filePath is empty." << endl;
		return;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "DataManager::loadCSV read file failure." << endl;
		return;
	}
	
	QTextStream source(&file);
	QStringList allLines = source.readAll().split("\n");
	
	QString currEntityType;
	QStringList currEntityAttrs;
	QVector<QList<double>> currEntityAttrValuesVec;

	for (int i = 0; i < allLines.size(); i++)
	{
		QList<double> currEntityAttrValues;

		QStringList currLineItems = allLines.at(i).split(",");

		if (currLineItems.size() <= 0)
			continue;
		if (currLineItems.size() == 1)
			continue;
		QString firstItem = currLineItems.at(0);

		if (firstItem.at(0) == '#')	//判断首字符是否为#	entityType entityAttr1 entityAttr2 entityAttr3 entityAttr4 
		{
			if (!currEntityType.isEmpty())	//标志着上一组数据处理完毕，需要将上一组数据添加进map
			{
				QMap<QString, QList<double>> lastGroupDataMap;

				//遍历数据行，并按列组装数据，Rows->Columns
				for (int k = 0; k < currEntityAttrValuesVec.size();k++)
				{
					for (int t = 0;t < currEntityAttrValuesVec[k].size();t++)
					{
						double currEntityAttrValue = currEntityAttrValuesVec.at(k).at(t);

						QString currEntityAttr;
						if(t < currEntityAttrs.size())
							currEntityAttr = currEntityAttrs.at(t);

						if (!currEntityAttr.isEmpty())
						{
							lastGroupDataMap[currEntityAttr].push_back(currEntityAttrValue);
						}
					}
				}
				currEntityAttrValuesVec.clear();
				m_entityDataMap.insert(currEntityType, lastGroupDataMap);
			}

			//开始处理新一组数据
			currEntityType = firstItem.remove('#');

			currEntityAttrs.clear();
			for (int j = 1;j < currLineItems.size();j++)
			{
				QString currEntityAttr = currLineItems.at(j);
				currEntityAttrs.push_back(currEntityAttr);
			}
		}
		else
		{
			for (int j = 1; j < currLineItems.size(); j++)
			{
				double currEntityAttrValue = currLineItems.at(j).toDouble();
				currEntityAttrValues.push_back(currEntityAttrValue);
			}
			currEntityAttrValuesVec.push_back(currEntityAttrValues);
		}

	}
	
	file.close();
	QMessageBox::information(NULL, QString::fromLocal8Bit("提示信息"), QString::fromLocal8Bit("已成功加载数据"));
}

void DataManager::loadCSV_stringTime(const QString& filePath)
{
	if (filePath.isEmpty())
	{
		qDebug() << "DataManager::loadCSV filePath is empty." << endl;
		return;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly))
	{
		qDebug() << "DataManager::loadCSV read file failure." << endl;
		return;
	}

	QTextStream source(&file);
	QStringList allLines = source.readAll().split("\n");

	QString currEntityType;
	QStringList currEntityAttrs;
	QVector<QStringList> currEntityAttrStringVec;

	//Time
	m_minTime = SECPERDAY * 365;
	m_maxTime = -1;
	m_refYear = -1;

	for (int i = 0; i < allLines.size(); i++)
	{
		QStringList currEntityAttrValues;

		QStringList currLineItems = allLines.at(i).split(",");

		if (currLineItems.size() <= 0)
			continue;
		if (currLineItems.size() == 1)
			continue;
		QString firstItem = currLineItems.at(0);

		if (firstItem.at(0) == '#')	//判断首字符是否为#	entityType entityAttr1 entityAttr2 entityAttr3 entityAttr4 
		{
			if (!currEntityType.isEmpty())	//标志着上一组数据处理完毕，需要将上一组数据添加进map
			{
				QMap<QString, QList<double>> lastGroupDataMap;

				//遍历数据行，并按列组装数据，Rows->Columns
				for (int k = 0; k < currEntityAttrStringVec.size(); k++)
				{
					for (int t = 0; t < currEntityAttrStringVec[k].size(); t++)
					{
						double currEntityAttrValue = 0.0;

						QString currEntityAttr;
						if (t < currEntityAttrs.size())
							currEntityAttr = currEntityAttrs.at(t);

						if (!currEntityAttr.isEmpty())
						{
							//Time时间转成double类型
							if (currEntityAttr.compare("Time") == 0)
							{
								QString timeString = currEntityAttrStringVec.at(k).at(t);
								timeString.remove(QChar('\"'));
								QStringList timeStringList;
								OrdinalTimeFormatter timeFormat;
								if (timeFormat.canConvert(timeString, timeStringList))
								{
									if (m_refYear == -1)
									{
										m_refYear = timeStringList.at(1).toInt();
									}
									currEntityAttrValue = timeFormat.convertToSeconds(timeString, m_refYear);
									m_minTime = (currEntityAttrValue < m_minTime) ? currEntityAttrValue : m_minTime;
									m_maxTime = (currEntityAttrValue > m_maxTime) ? currEntityAttrValue : m_maxTime;
								}
								else
									currEntityAttrValue = 0.0;

								m_timeDataVector.append(currEntityAttrValue);
							}
							else
							{
								currEntityAttrValue = currEntityAttrStringVec.at(k).at(t).toDouble();
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
			for (int j = 1; j < currLineItems.size(); j++)
			{
				QString currEntityAttr = currLineItems.at(j);
				currEntityAttrs.push_back(currEntityAttr);
			}
		}
		else
		{
			for (int j = 1; j < currLineItems.size(); j++)
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
	QMessageBox::information(NULL, QString::fromLocal8Bit("提示信息"), QString::fromLocal8Bit("已成功加载数据"));
}

QMap<QString, QMap<QString, QList<double>>>& DataManager::getDataMap()
{
	return m_entityDataMap;
}

void DataManager::getMinMaxTime(double & minTime, double & maxTime)
{
	minTime = m_minTime;
	maxTime = m_maxTime;
}

int DataManager::getRefYear()
{
	return m_refYear;
}

QList<double> DataManager::getEntityAttr_Value_List(QString entity, QString attr)
{
	QList<double> valueList;
	if (!m_entityDataMap.isEmpty())
	{
		if (m_entityDataMap.contains(entity))
		{
			if (m_entityDataMap.value(entity).contains(attr))
			{
				valueList = m_entityDataMap.value(entity).value(attr);
			}
		}
	}
	return valueList;
}

QList<double> DataManager::getEntityAttr_MaxPartValue_List(QString entity, QString attr, double secs)
{
	int index = getEntityAttr_MaxIndex_List(entity, attr, secs);
	QList<double> valueList;
	if (index >= 0)
	{
		index++;
		valueList = getEntityAttr_Value_List(entity, attr);
		if (!valueList.isEmpty())
		{
			if (index < valueList.size())
			{
				valueList = valueList.mid(0, index);
			}
		}
	}
	return valueList;
}

QList<double> DataManager::getEntityAttr_PartValue_List(QString entity, QString attr, int minIndex, int maxIndex)
{
	QList<double> valueList;
	if (minIndex < 0 || maxIndex < 0)
		return valueList;

	valueList = getEntityAttr_Value_List(entity, attr);
	if (minIndex >= valueList.size() || maxIndex >= valueList.size())
		return valueList;

	valueList = valueList.mid(minIndex, (maxIndex - minIndex + 1));
	return valueList;
}

QVector<double> DataManager::getTimeData_vector()
{
	return m_timeDataVector;
}

int DataManager::getEntityAttr_MaxIndex_List(QString entity, QString attr, double secs)
{
	int index = 0;
	QList<double> timeList = getEntityAttr_Value_List(entity);
	if (!timeList.isEmpty())
	{
		for (index = 0; index < timeList.size(); ++index)
		{
			if (timeList.at(index) > secs)
			{
				break;
			}
		}
	}
	return (index-1);
}
