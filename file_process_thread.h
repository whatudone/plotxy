#ifndef FILEPROCESSTHREAD_H
#define FILEPROCESSTHREAD_H

#include <QThread>

class FileProcessThread : public QThread
{
    Q_OBJECT
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
    // 加载ASI格式的数据（二维类型）
    void loadASIData(const QString& asiFileName);
    // 使用正则表达式加载ASI中特定数据格式
    QStringList parsePlatformData(const QString& data);
signals:
    // 文件读取进度从0-100
    void fileReadProgress(int progress);

private:
    TaskType m_taskType = TaskType::LoadFile;
    QString m_fileName;
};

#endif // FILEPROCESSTHREAD_H
