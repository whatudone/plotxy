#include "RecvThread.h"
#include "DataManager.h"
#include "IO_USIM_MessageType.h"
#include "Utils.h"

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
    m_udpSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress);
    bool flag = m_udpSocket->joinMulticastGroup(localAddress);
    if(flag)
    {
        //        connect(m_udpSocket, &QUdpSocket::readyRead, this, &recvThread::onReadyRead, Qt::DirectConnection);
        connect(m_udpSocket,
                &QUdpSocket::readyRead,
                this,
                &recvThread::onProtoBufReadyRead,
                Qt::DirectConnection);
        exec();
    }
    else
    {
        qDebug() << "join udp multi failed";
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
        if(!header)
        {
            continue;
        }
        if(header->iMessageType == MARS_ADD_PLATINFODATA)
        {
            QByteArray platData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            MARS_PlatInfoDataExcect* plat =
                reinterpret_cast<MARS_PlatInfoDataExcect*>(platData.data());

            if(plat)
            {
                emit platInfoReceived(*plat);
                //                DataManagerInstance->onRecvPlatinfoData(*plat);
            }
        }
        else
        {
            GenericData data;
            data.m_relativeTime = header->dfTime * 3600;
            QByteArray genericData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            if(header->iMessageType == MARS_TRANSMIT_NORMAL)
            {
                MARS_MsgSimMessage* generic =
                    reinterpret_cast<MARS_MsgSimMessage*>(genericData.data());
                data.m_platName = QString::fromLocal8Bit(generic->OriginSender);
            }
            else if(header->iMessageType == VNR_GY_MOVETOPOINT)
            {
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
        if(!header)
        {
            continue;
        }
        if(header->iMessageType == USIM_PLATFORM_MESSAGE)
        {
            QByteArray platData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            USIM_PlatInfoMessage_Proto plat;
            std::string str(platData.data(), header->ilength);
            plat.ParseFromString(str);

            emit protobufPlatInfoReceived(plat);
        }
        else
        {
            // TODO,等待后续客户提供分类依据，然后开始分类
            GenericData data;
            data.m_relativeTime = header->dfTime * 3600;
            QByteArray genericData = datagram.mid(sizeof(Z_SendHeader), header->ilength);
            std::string str(genericData.data(), header->ilength);
            if(header->iMessageType == USIM_SIM_MESSAGE)
            {
                USIM_SIMINFO_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_SIM_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sscinarioname().data());
                data.m_eventName = QString("仿真信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_COMMANDSTRUCTURE_MESSAGE)
            {
                USIM_CommandStructInfo_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_COMMANDSTRUCTURE_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.snodename().data());
                data.m_eventName = QString("指挥结构信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_PLATLOADING_MESSAGE)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_PLATLOADING_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("装备挂载信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_TIME_MESSAGE)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_TIME_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("时间信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_DETECT_MESSAGE)
            {
                USIM_Detection_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DETECT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.csensorname().data());
                data.m_eventName = QString("探测信息");
                data.m_eventType = data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_FUSION_MESSAGE)
            {
                USIM_Fusion_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_FUSION_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
                data.m_eventName = QString("融合信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_DETECTEVENT_MESSAGE)
            {
                USIM_DetectionEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DETECTEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.csensorname().data());
                data.m_eventName = QString("探测事件信息");
                data.m_eventType = data.m_ID = generic.uplatid();
            }
            else if(header->iMessageType == USIM_FUSIONEVENT_MESSAGE)
            {
                USIM_FusionEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_FUSIONEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
                data.m_eventName = QString("融合事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_COMMUNICATIONEVENT_MESSAGE)
            {
                USIM_CommunicationEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_COMMUNICATIONEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.uid();
                data.m_eventName = QString("通信事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_COMMANDEVENT_MESSAGE)
            {
                USIM_CommandControlEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_COMMANDEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sslaveequipmentname().data());
                data.m_eventName = QString("指挥控制事件信息");
                data.m_eventType = data.m_ID = generic.uhostid();
            }
            else if(header->iMessageType == USIM_WEAPONEVENT_MESSAGE)
            {
                USIM_WeaponSystemEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_WEAPONEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cweaponname().data());
                data.m_eventName = QString("武器系统事件信息");
                data.m_eventType = data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_WEAPONLAUNCHEVENT_MESSAGE)
            {
                USIM_WeaponLaunchEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_WEAPONLAUNCHEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cweaponname().data());
                data.m_eventName = QString("武器发射事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_BAITLAUNCHEVENT_MESSAGE)
            {
                USIM_DecoyLaunchEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_BAITLAUNCHEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cbaitname().data());
                data.m_eventName = QString("诱饵发射事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_EWEVENT_MESSAGE)
            {
                USIM_ElectronicWarEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_EWEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cjammingsystemname().data());
                data.m_eventName = QString("电子战事件信息");
                data.m_eventType = data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_ACOUSTICJAMMINGEVENT_MESSAGE)
            {
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_ACOUSTICJAMMINGEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("水声干扰事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_SENSORSTATEEVENT_MESSAGE)
            {
                USIM_SensorStateEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_SENSORSTATEEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.ssensorname().data());
                data.m_eventName = QString("传感器开关机事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_ENGAGEMENTRESULTEVENT_MESSAGE)
            {
                USIM_BattleResultEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_ENGAGEMENTRESULTEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sweaponname().data());
                data.m_eventName = QString("交战结果事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DAMAGEEVENT_MESSAGE)
            {
                USIM_DamageEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DAMAGEEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sweaponname().data());
                data.m_eventName = QString("毁伤事件信息");
                data.m_eventType = data.m_ID = generic.uattackerid();
            }
            else if(header->iMessageType == USIM_LOGISTICSUPPLYEVENT_MESSAGE)
            {
                USIM_LodisticsEvent_Protp generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_LOGISTICSUPPLYEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.usenderid();
                data.m_eventName = QString("后勤补给事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AIRCRAFTLAUNCHEVENT_MESSAGE)
            {
                USIM_AircraftTakeoffEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AIRCRAFTLAUNCHEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.splanename().data());
                data.m_eventName = QString("飞机起飞事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uplaneid();
            }
            else if(header->iMessageType == USIM_AIRCRAFTLANDINGEVENT_MESSAGE)
            {
                USIM_AircraftLandEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AIRCRAFTLANDINGEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.uplaneid();
                data.m_eventName = QString("飞机降落事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_RTBEVENT_MESSAGE)
            {
                USIM_AircraftReturnEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_RTBEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_ID = generic.uplaneid();
                data.m_eventName = QString("飞机返回基地事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_THROWBUOYEVENT_MESSAGE)
            {
                USIM_BuoyReleaseEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_THROWBUOYEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.sbuoy().data());
                data.m_eventName = QString("撒浮标事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_SONARBUOYREUSINGEVENT_MESSAGE)
            {
                USIM_SonarDropReuseEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_SONARBUOYREUSINGEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.ssonarname().data());
                data.m_eventName = QString("声纳吊放回收事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_FORCEOPROJECTIONEVENT_MESSAGE)
            {
                USIM_ForceDeliveryEvent_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_FORCEOPROJECTIONEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.cforcename().data());
                data.m_eventName = QString("兵力投送事件信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_FORCEREUSINGEVENT_MESSAGE)
            {
                USIM_ForceReusingEvent_Proto generic;
                generic.ParseFromString(str);
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_FORCEREUSINGEVENT_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_ID = generic.uid();
                data.m_eventName = QString("兵力回收事件信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_DYNAMICRADIATIONCIRCLE_MESSAGE)
            {
                USIM_DyamicCycle_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_DYNAMICRADIATIONCIRCLE_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("动态放射圈信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_WEDGE_MESSAGE)
            {
                USIM_WedgeBeam_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DRAWING_WEDGE_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("楔形波束体信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_BALL_MESSAGE)
            {
                USIN_SphereBeam_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DRAWING_BALL_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("球形波束体信息");
                data.m_eventType = data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_CIRCULARRING_MESSAGE)
            {
                USIM_CircleBeam_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_DRAWING_CIRCULARRING_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("圆环形波束体信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_CIRCULARCONE_MESSAGE)
            {
                USIM_ConeBeam_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_DRAWING_CIRCULARCONE_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("圆锥形波束体信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_PYRAMID_MESSAGE)
            {
                USIM_PyramidBeam_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DRAWING_PYRAMID_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("方锥形波束体信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DRAWING_CYLINDER_MESSAGE)
            {
                USIM_CylinderBeam_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DRAWING_CYLINDER_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("圆柱形波束体信息");
                data.m_eventType = type;
                data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_LINETYPE_MESSAGE)
            {
                USIM_Line_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_LINETYPE_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("线型信息");
                data.m_eventType = data.m_ID = generic.uid();
            }
            else if(header->iMessageType == USIM_DISABLEDISPLAY_MESSAGE)
            {
                USIM_GraphOnoff_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_DISABLEDISPLAY_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.suuid().data());
                data.m_eventName = QString("图形显示关闭信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_SONARDETECTINFO_MESSAGE)
            {
                USIM_SonarDetectInfo_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_SONARDETECTINFO_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.detector().data());
                data.m_eventName = QString("声纳探测详情");
                data.m_eventType = type;
                data.m_ID = generic.plat_id();
            }
            else if(header->iMessageType == USIM_SONOBOUYDETECTINFO_MESSAGE)
            {
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_SONOBOUYDETECTINFO_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("浮标阵探测详情");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AirDeckAbort_MESSAGE)
            {
                USIM_AircraftDeckAbortEvent_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AirDeckAbort_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.aircraft().data());
                data.m_eventName = QString("飞机甲板中止事件信息");
                data.m_eventType = data.m_ID = generic.major_p();
            }
            else if(header->iMessageType == USIM_IssuingTaskAssignment_MESSAGE)
            {
                USIM_IssuingTaskAssignment_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_IssuingTaskAssignment_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.thewarfarecommander().data());
                data.m_eventName = QString("指挥任务分派信息");
                data.m_eventType = type;
                data.m_ID = generic.commandertrackid();
            }
            else if(header->iMessageType == USIM_CommandTaskCompleted_MESSAGE)
            {
                USIM_CommandTaskCompleted_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_CommandTaskCompleted_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.thewarfarecommander().data());
                data.m_eventName = QString("指挥任务完成信息");
                data.m_eventType = type;
                data.m_ID = generic.commandertrackid();
            }
            else if(header->iMessageType == USIM_SetUpTrack_MESSAGE)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_SetUpTrack_MESSAGE);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("航线信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_SetUpArea_MESSAGE)
            {
                USIM_TrackAreaInfo_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_SetUpArea_MESSAGE);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.name().data());
                data.m_eventName = QString("区域信息");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliLaunch_Message)
            {
                USIM_AMPHELITRANSFER_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpHeliLaunch_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.transportasset().data());
                data.m_eventName = QString("直升机垂直登陆起飞");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliRecovery_Message)
            {
                USIM_AMPHELITRANSFER_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpHeliRecovery_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.heliasset().data());
                data.m_eventName = QString("直升机垂直登陆回位");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliDamage_Message)
            {
                USIM_AMPHELITRANSFER_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpHeliDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.heliasset().data());
                data.m_eventName = QString("直升机垂直登陆过程中损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliUnloadBegin_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpHeliUnloadBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("直升机卸载开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliUnloadEnd_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpHeliUnloadEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("直升机卸载结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliUnloadDamage_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpHeliUnloadDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("直升机卸载过程中损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftFlashBegin_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftFlashBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.transportasset().data());
                data.m_eventName = QString("气垫艇泛水开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftFlashEnd_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpCraftFlashEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.flashingasset().data());
                data.m_eventName = QString("气垫艇泛水结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftFlashDamage_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftFlashDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.flashingasset().data());
                data.m_eventName = QString("气垫艇泛水损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpVehicleFlashBegin_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpVehicleFlashBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.transportasset().data());
                data.m_eventName = QString("两栖装甲泛水开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpVehicleFlashEnd_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpVehicleFlashEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.flashingasset().data());
                data.m_eventName = QString("两栖装甲泛水结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpVehicleFlashDamage_Message)
            {
                USIM_AMPFLASHING_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpVehicleFlashDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.flashingasset().data());
                data.m_eventName = QString("两栖装甲泛水损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftTransBegin_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftTransBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.transportasset().data());
                data.m_eventName = QString("气垫艇换乘开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftTransEnd_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpCraftTransEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.subasset().data());
                data.m_eventName = QString("气垫艇换乘结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftTransDamage_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftTransDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.subasset().data());
                data.m_eventName = QString("气垫艇换乘损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliTransBegin_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpHeliTransBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.transportasset().data());
                data.m_eventName = QString("直升机换乘开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliTransEnd_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type = DataManagerInstance->getGroupNameByID(USIM_AmpHeliTransEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.subasset().data());
                data.m_eventName = QString("直升机换乘结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpHeliTransDamage_Message)
            {
                USIM_AMPTRANSFER_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpHeliTransDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.subasset().data());
                data.m_eventName = QString("直升机换乘损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftUnloadBegin_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftUnloadBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("气垫船卸载开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftUnloadEnd_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftUnloadEnd_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("气垫船卸载结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpCraftUnloadDamage_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpCraftUnloadDamage_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("气垫船卸载损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpSteamBoatUnloadBegin_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpSteamBoatUnloadBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("冲锋舟卸载开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpSteamBoatUnloadBegin_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpSteamBoatUnloadBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("冲锋舟卸载结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_AmpSteamBoatUnloadBegin_Message)
            {
                USIM_AMPLANDINGUNLOAD_Proto generic;
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_AmpSteamBoatUnloadBegin_Message);
                if(type.isEmpty())
                    continue;
                generic.ParseFromString(str);
                data.m_platName = QString::fromLocal8Bit(generic.unloadasset().data());
                data.m_eventName = QString("冲锋舟卸载损毁");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_MineSweepStart_Message)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_MineSweepStart_Message);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("扫雷开始");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_MineSweepEnd_Message)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_MineSweepEnd_Message);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("扫雷结束");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_MineSweepSuccess_Message)
            {
                QString type = DataManagerInstance->getGroupNameByID(USIM_MineSweepSuccess_Message);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("扫雷成功");
                data.m_eventType = type;
            }
            else if(header->iMessageType == USIM_MineSweepExploded_Message)
            {
                QString type =
                    DataManagerInstance->getGroupNameByID(USIM_MineSweepExploded_Message);
                if(type.isEmpty())
                    continue;
                data.m_eventName = QString("水雷爆炸");
                data.m_eventType = type;
            }
            emit genericReceived(data);
        }
    }
}
