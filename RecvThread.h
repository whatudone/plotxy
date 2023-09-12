#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QDateTime>
#include <QThread>
#include <QUdpSocket>

#include "IO_CommStructer.h"
#include "constdef.h"
#include "protobuf/USimDataStructm.pb.h"

class recvThread : public QThread
{
    Q_OBJECT
public:
    explicit recvThread(QObject* parent = nullptr);
    ~recvThread() override;

    void stop();
    void startThread();

protected:
    void run() override;
    void onProtoBufReadyRead();

signals:
    void updateRealTime();

private:
    QUdpSocket* m_udpSocket = nullptr;
    bool m_startFlag = false;
    // 上次图表刷新时间
    int64_t m_lastUpdateTime = QDateTime::currentMSecsSinceEpoch();
};

#endif // RECVTHREAD_H
