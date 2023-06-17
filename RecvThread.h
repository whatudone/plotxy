#ifndef RECVTHREAD_H
#define RECVTHREAD_H

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

protected:
    void run() override;

    void onReadyRead();
    void onProtoBufReadyRead();

signals:
    void platInfoReceived(const MARS_PlatInfoDataExcect &plat);
    void genericReceived(const GenericData &generic);

    void protobufPlatInfoReceived(const USIM_PlatInfoMessage_Proto &plat);

private:
    QUdpSocket* m_udpSocket = nullptr;
};

#endif // RECVTHREAD_H
