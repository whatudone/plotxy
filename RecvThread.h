#ifndef RECVTHREAD_H
#define RECVTHREAD_H

#include <QThread>
#include <QUdpSocket>

#include "constdef.h"

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

signals:
    void platInfoReceived(PlatInfoDataExcect plat);
    void genericReceived(GenericDataExcect generic);

private:
    QUdpSocket* m_udpSocket = nullptr;
};

#endif // RECVTHREAD_H
