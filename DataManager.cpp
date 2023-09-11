/* * @filename: DataManager.cpp
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include "data_manager_data.h"

#include <QFileInfo>
#include <QMessageBox>
DataManager::DataManager()
{
    m_recvThread = new recvThread;
}

DataManager::~DataManager() {}

bool DataManager::saveDataToASI(const QString& asiFileName)
{
    return DataManagerData::getInstance()->saveDataToASI(asiFileName);
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
        DataManagerData::getInstance()->loadCSVData(filename);
    }
    else if(suffix == "asi")
    {
        DataManagerData::getInstance()->loadASIData(filename);
    }
    else
    {
        QMessageBox::warning(nullptr, "警告", "不支持的文件类型");
        return;
    }
    if(DataManagerData::getInstance()->entityDataIsEmpty())
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

recvThread* DataManager::getRecvThread() const
{
    return m_recvThread;
}
