#ifndef FILEPROCESSTHREAD_H
#define FILEPROCESSTHREAD_H

#include <QThread>

class FileProcessThread : public QThread
{
public:
    enum class TaskType
    {
        LoadFile = 0,
        SaveFile
    };

public:
    FileProcessThread(TaskType taskType, QObject* parent = nullptr);

    QString fileName() const;
    void setFileName(const QString& fileName);

protected:
    void run() override;

private:
    TaskType m_taskType = TaskType::LoadFile;
    QString m_fileName;
};

#endif // FILEPROCESSTHREAD_H
