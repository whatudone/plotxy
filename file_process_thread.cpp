#include "file_process_thread.h"
#include "data_manager_data.h"
FileProcessThread::FileProcessThread(TaskType taskType, QObject* parent)
    : QThread(parent)
    , m_taskType(taskType)
{}

void FileProcessThread::run()
{
    if(m_taskType == TaskType::LoadFile)
    {
        DataManagerData::getInstance()->loadASIData(m_fileName);
    }
    else
    {
        DataManagerData::getInstance()->saveDataToASI(m_fileName);
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
