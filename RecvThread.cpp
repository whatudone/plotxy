#include "RecvThread.h"
#include "DataManager.h"
#include "IO_USIM_MessageType.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <string>
#include <thread>

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
    //    m_udpSocket->setSocketOption(QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint,true);
    m_udpSocket->bind(QHostAddress::AnyIPv4,port,QUdpSocket::ShareAddress);
    bool flag= m_udpSocket->joinMulticastGroup(localAddress);
    if(flag){
//        connect(m_udpSocket, &QUdpSocket::readyRead, this, &recvThread::onReadyRead, Qt::DirectConnection);
        connect(m_udpSocket, &QUdpSocket::readyRead, this, &recvThread::onProtoBufReadyRead, Qt::DirectConnection);
        exec();
    }else{
        qDebug()<<"join udp multi failed";
    }
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
        if(!header){
            continue;
        }
        if(header->iMessageType == MARS_ADD_PLATINFODATA)
        {
            QByteArray platData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            MARS_PlatInfoDataExcect* plat = reinterpret_cast<MARS_PlatInfoDataExcect*>(platData.data());

            if(plat){
                emit platInfoReceived(*plat);
                //                DataManagerInstance->onRecvPlatinfoData(*plat);

            }
        }
        else
        {
            GenericData data;
            data.m_relativeTime = header->dfTime*3600;
            QByteArray genericData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            if(header->iMessageType == MARS_TRANSMIT_NORMAL){
                MARS_MsgSimMessage *generic = reinterpret_cast<MARS_MsgSimMessage*>(genericData.data());
                data.m_platName = QString::fromLocal8Bit(generic->OriginSender);
            }
            else if(header->iMessageType == VNR_GY_MOVETOPOINT){
                MOVE_CTRL_POINT* generic = reinterpret_cast<MOVE_CTRL_POINT*>(genericData.data());
                data.m_platName = QString::fromLocal8Bit(generic->host_name);
            }

            emit genericReceived(data);
        }
    }
}

void recvThread::onProtoBufReadyRead()
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
        if(!header){
            continue;
        }
        if(header->iMessageType == USIM_PLATFORM_MESSAGE)
        {
            QByteArray platData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            USIM_PlatInfoMessage_Proto plat;
            std::string str(platData.data(),header->ilength);
            plat.ParseFromString(str);

            QString name = QString::fromLocal8Bit(plat.splatname().data());
            emit protobufPlatInfoReceived(plat);

        }

        else
        {
            GenericData data;
            data.m_relativeTime = header->dfTime*3600;
            QByteArray genericData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            std::string str(genericData.data(),header->ilength);
            if(header->iMessageType == USIM_SIM_MESSAGE){
                USIM_SIMINFO_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sscinarioname().data());
            }
            else if(header->iMessageType == USIM_COMMANDSTRUCTURE_MESSAGE){
                USIM_CommandStructInfo_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.snodename().data());
            }
            else if(header->iMessageType == USIM_PLATLOADING_MESSAGE){
            }
            else if(header->iMessageType == USIM_TIME_MESSAGE){
            }
            else if(header->iMessageType == USIM_DETECT_MESSAGE){
                USIM_Detection_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.csensorname().data());
            }
            else if(header->iMessageType == USIM_FUSION_MESSAGE){
                USIM_Fusion_Proto generic;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DETECTEVENT_MESSAGE){
                USIM_DetectionEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.csensorname().data());
            }
            else if(header->iMessageType == USIM_FUSIONEVENT_MESSAGE){
                USIM_FusionEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_COMMUNICATIONEVENT_MESSAGE){
                USIM_CommunicationEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_COMMANDEVENT_MESSAGE){
                USIM_CommandControlEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sslaveequipmentname().data());
            }
            else if(header->iMessageType == USIM_WEAPONEVENT_MESSAGE){
                USIM_WeaponSystemEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cweaponname().data());
            }
            else if(header->iMessageType == USIM_WEAPONLAUNCHEVENT_MESSAGE){
                USIM_WeaponLaunchEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cweaponname().data());
            }
            else if(header->iMessageType == USIM_BAITLAUNCHEVENT_MESSAGE){
                USIM_DecoyLaunchEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cbaitname().data());
            }
            else if(header->iMessageType == USIM_EWEVENT_MESSAGE){
                USIM_ElectronicWarEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cjammingsystemname().data());
            }
            else if(header->iMessageType == USIM_ACOUSTICJAMMINGEVENT_MESSAGE){
            }
            else if(header->iMessageType == USIM_SENSORSTATEEVENT_MESSAGE){
                USIM_SensorStateEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.ssensorname().data());
            }
            else if(header->iMessageType == USIM_ENGAGEMENTRESULTEVENT_MESSAGE){
                USIM_BattleResultEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sweaponname().data());
            }
            else if(header->iMessageType == USIM_DAMAGEEVENT_MESSAGE){
                USIM_DamageEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sweaponname().data());
            }
            else if(header->iMessageType == USIM_LOGISTICSUPPLYEVENT_MESSAGE){
                USIM_LodisticsEvent_Protp generic;
                generic.ParseFromString(str);
                data.m_ID = generic.usenderid();
            }
            else if(header->iMessageType == USIM_AIRCRAFTLAUNCHEVENT_MESSAGE){
                USIM_AircraftTakeoffEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.splanename().data());
            }
            else if(header->iMessageType == USIM_AIRCRAFTLANDINGEVENT_MESSAGE){
                USIM_AircraftLandEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_ID = generic.uplaneid();
            }
            else if(header->iMessageType == USIM_RTBEVENT_MESSAGE){
                USIM_AircraftReturnEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_ID = generic.uplaneid();
            }
            else if(header->iMessageType == USIM_THROWBUOYEVENT_MESSAGE){
                USIM_BuoyReleaseEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sbuoy().data());
            }
            else if(header->iMessageType == USIM_SONARBUOYREUSINGEVENT_MESSAGE){
                USIM_SonarDropReuseEvent_Proto generic;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.ssonarname().data());
            }


            emit genericReceived(data);
        }
    }
}
