
#pragma once

/************************************************************************/
/*                                                                      */
/*                   USIM软件内部统一接口定义                          */
/*                                                                      */
/*    本接口由初阳、贾贞、吴章华共同商定，吴章华编辑。       */
/************************************************************************/
/************************************************************************/
/* 说明  :
   本接口由“信息发送类接口”和“人工干预类接口"两部分组成，其中，
“信息发送类接口” 是指由MARS后台向外发送的数据; “人工干预类接口"
是指由外部向MARS后台发送的数据。                                                                
 */
/************************************************************************/
#define	USIM_SIM_MESSAGE	1000	//仿真信息
#define	USIM_COMMANDSTRUCTURE_MESSAGE	1001	//指挥结构信息
#define	USIM_PLATLOADING_MESSAGE	1002	//装备挂载信息
#define	USIM_TIME_MESSAGE	1003	//时间信息
#define	USIM_PLATFORM_MESSAGE	1004	//平台信息
#define	USIM_DETECT_MESSAGE	1005	//探测信息
#define	USIM_FUSION_MESSAGE	1006	//融合信息
#define	USIM_DETECTEVENT_MESSAGE	1007	//探测事件信息
#define	USIM_FUSIONEVENT_MESSAGE	1008	//融合事件信息
#define	USIM_COMMUNICATIONEVENT_MESSAGE	1009	//通信事件信息
#define	USIM_COMMANDEVENT_MESSAGE	1010	//指挥控制事件信息
#define	USIM_WEAPONEVENT_MESSAGE	1011	//武器系统事件信息
#define	USIM_WEAPONLAUNCHEVENT_MESSAGE	1012	//武器发射事件信息
#define	USIM_BAITLAUNCHEVENT_MESSAGE	1013	//诱饵发射事件信息
#define	USIM_EWEVENT_MESSAGE	1014	//电子战事件信息
#define	USIM_ACOUSTICJAMMINGEVENT_MESSAGE	1015	//水声干扰事件信息
#define	USIM_SENSORSTATEEVENT_MESSAGE	1016	//传感器开关机事件信息
#define	USIM_ENGAGEMENTRESULTEVENT_MESSAGE	1017	//交战结果事件信息
#define	USIM_DAMAGEEVENT_MESSAGE	1018	//毁伤事件信息
#define	USIM_LOGISTICSUPPLYEVENT_MESSAGE	1019	//后勤补给事件信息
#define	USIM_AIRCRAFTLAUNCHEVENT_MESSAGE	1020	//飞机起飞事件信息
#define	USIM_AIRCRAFTLANDINGEVENT_MESSAGE	1021	//飞机降落事件信息
#define	USIM_RTBEVENT_MESSAGE	1022	//飞机返回基地事件信息
#define	USIM_THROWBUOYEVENT_MESSAGE	1023	//撒浮标事件信息
#define	USIM_SONARBUOYREUSINGEVENT_MESSAGE	1024	//声纳吊放回收事件信息
#define	USIM_FORCEOPROJECTIONEVENT_MESSAGE	1025	//兵力投送事件信息
#define	USIM_FORCEREUSINGEVENT_MESSAGE	1026	//兵力回收事件信息
#define	USIM_DYNAMICRADIATIONCIRCLE_MESSAGE	1027	//动态放射圈信息
#define	USIM_DRAWING_WEDGE_MESSAGE	1028	//楔形波束体信息
#define	USIM_DRAWING_BALL_MESSAGE	1029	//球形波束体信息
#define	USIM_DRAWING_CIRCULARRING_MESSAGE	1030	//圆环形波束体信息
#define	USIM_DRAWING_CIRCULARCONE_MESSAGE	1031	//圆锥形波束体信息
#define	USIM_DRAWING_PYRAMID_MESSAGE	1032	//方锥形波束体信息
#define	USIM_DRAWING_CYLINDER_MESSAGE	1033	//圆柱形波束体信息
#define	USIM_LINETYPE_MESSAGE	1034	//线型信息
#define	USIM_DISABLEDISPLAY_MESSAGE	1035	//图形显示关闭信息
#define	USIM_SONARDETECTINFO_MESSAGE	1036//声纳探测详情
#define	USIM_SONOBOUYDETECTINFO_MESSAGE	1037//浮标阵探测详情
#define	USIM_AirDeckAbort_MESSAGE	1038	//飞机甲板中止事件信息
#define	USIM_IssuingTaskAssignment_MESSAGE	1039	//指挥任务分派信息
#define	USIM_CommandTaskCompleted_MESSAGE	1040	//指挥任务完成信息 
#define	USIM_SetUpTrack_MESSAGE	1041	//航线信息 
#define	USIM_SetUpArea_MESSAGE	1042	//区域信息 

#define	USIM_AmpHeliLaunch_Message	1043	//直升机垂直登陆起飞
#define	USIM_AmpHeliRecovery_Message	1044	//直升机垂直登陆回位
#define	USIM_AmpHeliDamage_Message	1045	//直升机垂直登陆过程中损毁

#define	USIM_AmpHeliUnloadBegin_Message	1046	//直升机卸载开始
#define	USIM_AmpHeliUnloadEnd_Message	1047	//直升机卸载结束
#define	USIM_AmpHeliUnloadDamage_Message	1048	//直升机卸载过程中损毁

#define	USIM_AmpCraftFlashBegin_Message	1049	//气垫艇泛水开始
#define	USIM_AmpCraftFlashEnd_Message	1050	//气垫艇泛水结束
#define	USIM_AmpCraftFlashDamage_Message	1051	//气垫艇泛水损毁

#define	USIM_AmpVehicleFlashBegin_Message	1052	//两栖装甲泛水开始
#define	USIM_AmpVehicleFlashEnd_Message	1053	//两栖装甲泛水结束
#define	USIM_AmpVehicleFlashDamage_Message	1054	//两栖装甲泛水损毁

#define	USIM_AmpCraftTransBegin_Message	1055	//气垫艇换乘开始
#define	USIM_AmpCraftTransEnd_Message	1056	//气垫艇换乘结束
#define	USIM_AmpCraftTransDamage_Message	1057	//气垫艇换乘损毁

#define	USIM_AmpHeliTransBegin_Message	1058	//直升机换乘开始
#define	USIM_AmpHeliTransEnd_Message	1059	//直升机换乘结束
#define	USIM_AmpHeliTransDamage_Message	1060	//直升机换乘损毁

#define USIM_AmpCraftUnloadBegin_Message 1061    //气垫船卸载开始
#define USIM_AmpCraftUnloadEnd_Message 1062    //气垫船卸载结束
#define USIM_AmpCraftUnloadDamage_Message 1063    //气垫船卸载损毁

#define USIM_AmpSteamBoatUnloadBegin_Message 1064  //冲锋舟卸载开始
#define USIM_AmpSteamBoatUnloadBegin_Message 1065  //冲锋舟卸载结束
#define USIM_AmpSteamBoatUnloadBegin_Message 1066  //冲锋舟卸载损毁

#define USIM_MineSweepStart_Message 1067  //扫雷开始 
#define USIM_MineSweepEnd_Message 1068  //扫雷结束
#define USIM_MineSweepSuccess_Message 1069  //扫雷成功
#define USIM_MineSweepExploded_Message 1070  //水雷爆炸

#pragma pack (push ,1) 

//struct Z_SendHeader{
//	unsigned short	iMessageType;	//信息标识
//	double  dfTime;				   //时间
//	int		ilength;			  //本包信息长度
//};


#pragma pack (pop)
