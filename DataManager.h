/* * @filename: DataManager.h
* @brief: DataManager实现数据加载功能
* @author: zhameng
* @create time: 2022-09-27
*  */

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include "RecvThread.h"

class DataManager : public QObject
{
    Q_OBJECT
public:
    static DataManager* getInstance()
    {
        static DataManager mgr;
        return &mgr;
    }

    recvThread* getRecvThread() const;
    void loadFileData(const QString& filename);
    bool saveDataToASI(const QString& file);

private:
    DataManager();
    DataManager(const DataManager&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    ~DataManager();

signals:
    void loadDataFinished();

private:
    recvThread* m_recvThread = nullptr;
};

#define DataManagerInstance DataManager::getInstance()
#endif // !
