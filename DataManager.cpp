/* * @filename: DataManager.cpp
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#include "DataManager.h"
#include "data_manager_data.h"
#include "file_process_thread.h"

#include <QDialog>
#include <QFileInfo>
#include <QMessageBox>
DataManager::DataManager()
{
    m_recvThread = new recvThread;
}

DataManager::~DataManager() {}

bool DataManager::saveDataToASI(const QString& asiFileName)
{
    FileProcessThread thread(FileProcessThread::TaskType::SaveFile);
    thread.setFileName(asiFileName);
    QMessageBox message("保存",
                        "正在保存数据，请稍后！",
                        QMessageBox::Information,
                        QMessageBox::NoButton,
                        QMessageBox::NoButton,
                        QMessageBox::NoButton);
    message.setStandardButtons(QMessageBox::NoButton);
    connect(&thread, &QThread::finished, &message, &QDialog::accept);
    thread.start();
    message.exec();
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
    if(suffix == "asi")
    {
        FileProcessThread thread(FileProcessThread::TaskType::LoadFile);
        thread.setFileName(filename);
        QMessageBox message("导入",
                            "正在导入数据，请稍后！",
                            QMessageBox::Information,
                            QMessageBox::NoButton,
                            QMessageBox::NoButton,
                            QMessageBox::NoButton);
        message.setStandardButtons(QMessageBox::NoButton);
        connect(&thread, &QThread::finished, &message, &QDialog::accept);
        thread.start();
        message.exec();
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
