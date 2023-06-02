#include "RecvThread.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <string>
#include <thread>

#pragma comment(lib, "ws2_32.lib")

recvThread::recvThread(QObject* parent)
    : QThread(parent)
{}

recvThread::~recvThread() {}

void recvThread::stop()
{
    quit();
    wait();
}

void recvThread::run()
{
    m_udpSocket = new QUdpSocket();
    QString iniFileName = QCoreApplication::applicationDirPath() + "/PlotXY.ini";
    int port;
    QString address;
    QSettings setting(QCoreApplication::applicationDirPath() + "/PlotXY.ini", QSettings::IniFormat);
    if(!QFile::exists(iniFileName))
    {
        port = 8888;
        address = "127.0.0.1";
        setting.setValue("netConfig/address", "127.0.0.1");
        setting.setValue("netConfig/port", 8888);
    }
    else
    {
        address = setting.value("netConfig/address").toString();
        port = setting.value("netConfig/port").toInt();
    }

    QHostAddress localAddress(address);
    m_udpSocket->bind(localAddress, quint16(port));
    connect(
        m_udpSocket, &QUdpSocket::readyRead, this, &recvThread::onReadyRead, Qt::DirectConnection);
    exec();
    m_udpSocket->deleteLater();
}

void recvThread::onReadyRead()
{
    while(m_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(m_udpSocket->pendingDatagramSize());
        QHostAddress senderAddress;

        m_udpSocket->readDatagram(datagram.data(), datagram.size(), &senderAddress);
        QByteArray headerData = datagram.mid(0, sizeof(Z_SendHeader));
        // 处理接收到的数据
        Z_SendHeader* header = reinterpret_cast<Z_SendHeader*>(headerData.data());
        QByteArray platData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
        PlatInfoDataExcect* plat = reinterpret_cast<PlatInfoDataExcect*>(platData.data());

        emit dataReceived(*plat);
    }
}
