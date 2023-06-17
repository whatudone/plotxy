
#pragma once
#include "GenericDefined.h"
/************************************************************************/
/*                                                                      */
/*                    MARS软件内部统一接口定义                          */
/*                                                                      */
/*    本接口由初阳、曾清、姜申、季蓓、韩海良等共同完成，包含了MARS后台  */
/*  与外部进行信息交互的所有数据结构。                                  */
/*  2015年1月12日，联合机动编队项目设计阶段，初阳进行了统一修订。       */
/************************************************************************/
/************************************************************************/
/* 说明  :
   本接口由“信息发送类接口”和“人工干预类接口"两部分组成，其中，
“信息发送类接口” 是指由MARS后台向外发送的数据; “人工干预类接口"
是指由外部向MARS后台发送的数据。                                                                
 */
/************************************************************************/
#define MACTABLE     5
#define GY8B         8
#define GY16B        16
#define GY32B		 32
#define GY40B		 40
#define GY64B        64
#define GY128B	     128
#define GY256B	     256
#define GY1024B	     1024
#define MESSAGENUM   1000
#define MAXVEC			20000
#define NLENTH			32
#define PORTFORMARS     20480       //端口      
/************************************************/
/*      2. 对外发送信息标识                                          
/************************************************/
#define MARS_MST_NONE                                  0//☆提示信息
#define MARS_AC_LAUNCHING                              1//飞机起飞*****************js
#define MARS_AC_RECOVERY                               2//飞机降落*****************js
#define MARS_AC_DECK_ABORT                             3//☆飞机甲板无效
#define MARS_AC_SUPPLIES                               4//☆飞机补给
#define MARS_AC_NO_AIRCRAFT                            5//飞机返回基地*****************js
#define MARS_AC_PROFILE                                7//☆飞机未储备
#define MARS_AC_NO_RESERVE                             8//☆飞机取消起飞
#define MARS_AMT_WPN_MISS                              10// 武器未击中*****************zq
#define MARS_AMT_WPN_SUCCESS_HIT                       11// 武器击中*****************zq
#define MARS_AMT_WPN_SUCCESS_DMG                       12// ☆武器成功毁伤目标
#define MARS_AMT_WPN_LOST_TRACK                        13//☆ 武器丢失目标
#define MARS_AMT_WPN_OUT_OF_RANGE                      14// 武器超出范围*****************zq
#define MARS_AMT_WPN_DESTRY_P_DEFENSE                  15// 武器被点防御系统毁伤
#define MARS_AMT_WPN_SURV_P_DEFENSE                    16// 武器突破点防御系统
#define MARS_AMT_WPN_WASTED                            17// 武器浪费*****************zq
#define MARS_AMT_CASUALTIES_TAKEN                      18// ☆ 人员伤亡
#define MARS_AMT_FUEL_EXHAUSTED                        196	// 燃料耗尽
#define MARS_AMT_WPN_INEFFECTIVE                       209//☆武器失效
#define MARS_COMMAND_STANDARD                          19// ☆指挥标准信息
#define MARS_COMMAND_HLA                               20// ☆指挥信息-HLA
#define MARS_COMMAND_EC                                22// ☆指挥信息-EC
#define MARS_COMMAND_EC_SELF                           23// ☆指挥信息-EC_SELF
#define MARS_COMMAND_TRANSFER                          208//☆ 指挥转移
#define MARS_COMMAND_PROCESSING_ASSIGNMENT             24// ☆指令处理分配信息
#define MARS_COMMAND_PROCESSING_UPDATE                 25// ☆指令处理更新信息
#define MARS_COMMAND_ISSUING_ALERT_LAUNCH_ASSIGNMENT   26// ☆发布发射分派警报类
#define MARS_COMMAND_CANTCO_ALERT_LAUNCH_ASSIGNMENT    27// ☆发射任务警报
#define MARS_COMMAND_ISSUING_ALERT_LAUNCH_CANCELATION  28// ☆发布发射取消警报
#define MARS_COMMAND_CANCEL_PROCESSING_ALERT_LAUNCH    29// ☆发射取消警报处理
#define MARS_COMMAND_ISSUING_TASK_ASSIGNMENT           30// 发布任务分派*****************zq
#define MARS_COMMAND_UNABLE_TO_DO_TASK                 31// 不能按分配的任务执行*****************zq
#define MARS_COMMAND_ISSUING_TASK_CANCELATION          32// ☆发布任务取消*****************zq
#define MARS_COMMAND_CANCEL_TASK_PROCESSING            33// ☆取消任务处理
#define MARS_COMMAND_ISSUING_ALERT_LAUNCH_UPDATE       34// ☆发布发射警报校正信息
#define MARS_COMMAND_COMPLYING_ALERT_LAUNCH_ORDER      35// ☆执行发射警报命令 
#define MARS_COMMAND_ORDERED_COMPLY_WITH_ASSIGNMENT    38// ☆按分配的任务执行*****************zq
#define MARS_COMMAND_ORDERED_ISSUING_UPDATE            39// ☆发布校正信息
#define MARS_COMMAND_ORDERED_COMPLY_WITH_UPDATE        41// ☆按任务校正命令执行
#define MARS_COMMAND_ORDERED_COMPLY_WITH_CANCEL        42// ☆按任务取消命令执行*****************zq
#define MARS_COMMAND_TASK_COMPLETED				       43// 任务完成*****************zq
#define MARS_COMMAND_TASK_COMPLETE_REPORT_ACK          44// ☆任务完成报告评定*****************zq
#define MARS_COMMAND_TASK_FAILED_REPORT_ACK            45// ☆任务失败报告评定*****************zq
#define MARS_COMMAND_TASK_RTB_FAILED_ACK               46// ☆目标不在队列-任务失败报告评定
#define MARS_CONTACT_START                             47	//☆关联开始信息
#define MARS_DETECT_START                              65//探测开始信息*****************cy
#define MARS_DETECT_PROCESSING                         73//探测过程信息
#define MARS_DETECT_DELAYED_START                      215//探测延迟开始信息*****************cy
#define MARS_MST_ASSET_TACTIC                          83//☆平台战术
#define MARS_GENERAL_INIT                              84//初始化信息*****************cy
#define MARS_GENERAL_TIMESTEP                          85//时间步长事件信息*****************cy
#define MARS_GENERAL_BAD_FORM                          87//☆编队错误提示信息
#define MARS_LOGISTIC_SIMPLE                           92//☆简单后勤信息
#define MARS_LOGISTIC_COMMANDER                        93//☆指挥类后勤信息
#define MARS_LOGISTIC_SUPPLY                           94//☆补给类后勤信息
#define MARS_LOGISTIC_TRANSFERING                      95//☆运输类后勤信息
#define MARS_LOGISTIC_PLATFORM                         96//☆平台类后勤信息
#define MARS_TRACK_NEW                                 97// 融合航迹新建信息*****************cy
#define MARS_TRACK_UPDATE                              98// 融合航迹更新信息*****************cy
#define MARS_TRACK_STOP                                99// 融合航迹停止信息*****************cy
#define MARS_TRACK_RESUME                              100// 融合航迹重建信息*****************cy
#define MARS_TRACK_DROP                                101// 融合航迹销毁信息*****************cy
#define MARS_GROUP_TRACK_NEW                           227// ☆群航迹新**暂不输出
#define MARS_GROUP_TRACK_UPDATE                        228// ☆群航迹更新**暂不输出
#define MARS_GROUP_TRACK_DROP                          229// ☆群航迹结束**暂不输出
#define MARS_TRANSMIT_NORMAL                           102//通信开始发送*****************cy
#define MARS_TRANSMIT_FAILED                           103//☆失败通讯信息**暂没找到
#define MARS_TRANSMIT_BROADCAST                        104//☆广播通讯信息**暂没找到
#define MARS_TRANSMIT_RECEIVE                          105//接收通讯信息*****************cy
#define MARS_STRIKE_SEARCH_PLAN                        106//☆打击搜索信息**暂没找到
#define MARS_STRIKE_MISSILE                            107//☆打击任务信息**暂没找到
#define MARS_STRIKE_GENERIC                            108//☆常规打击信息*****************cy
#define MARS_STRIKE_REGION_AREA                        109//☆打击区域信息**暂没找到
//以下是武器
#define MARS_MT_MUNITIONS_FAILED                       110//军需品失败*****************jb
#define MARS_MT_MUNITIONS_EXPENDED                     111//武器发射信息*****************jb
#define MARS_MT_WINCHESTER				               112//弹药耗尽信息*****************jb
#define MARS_MT_EVALUATION                             113//评估信息*****************jb
#define MARS_MT_SAM_MUNITIONS_EXPENDED                 114//地空导弹发射信息*****************jb
#define MARS_MT_OUT_OF_ENVELOPE                        115//目标在射程之外信息*****************jb
#define MARS_MT_TRACKING_LOST                          116//☆目标跟踪丢失信息*****************jb



//以下是地空导弹武器
#define MARS_MT_SAM_FCR                                117//SAM火控*****************jb
#define MARS_MT_SAM_FCR_LOCKED                         118//SAM火控锁定*****************jb
#define MARS_MT_SAM_FCR_LOST                           119//SAM火控丢失*****************jb
#define MARS_MT_SAM_ILLUMINATOR                        120//SAM照明*****************jb
#define MARS_MT_SAM_ILLUMINATOR_PAINTNG                121//SAM照明PAINTNG*****************jb
#define MARS_MT_SAM_ILLUMINATOR_AVAILABLE              122//SAM照明AVAILABLE*****************jb
#define MARS_MT_SAM_ILLUMINATOR_LOST                   123//☆
#define MARS_MT_SAM_LAUNCHER                           124//SAM发射*****************jb
#define MARS_MT_SAM_LOAD                               125//SAMLoad*****************jb
//以下是任务
#define MARS_TASK_LOITER_START                         126//☆
#define MARS_TASK_LOITER_END                           127//☆
#define MARS_TASK_TAT_START                            128//☆
#define MARS_TASK_TAT_END                              129//☆
#define MARS_MST_COMPONENT_DESTROYED                   130// 成员毁伤*****************lsl
#define MARS_MST_SUPPRESSED                            131// ☆取消
#define MARS_MST_SUPPRESSED_RECOVERY                   132// ☆取消恢复
#define MARS_MST_DAMAGED					           133// 损坏*****************lsl
#define MARS_MST_DESTROYED                             134// 摧毁*****************lsl
#define MARS_MST_DESTROYED_MINE                        135// ☆摧毁水雷
#define MARS_MST_CUED_ATTEMPT                          144// ☆攻击标识
#define MARS_MST_CUED_ATTEMPT_FAILURE                  145// ☆攻击失败标识
#define MARS_MST_CUED_OBSERVATION                      146// ☆观测标识
#define MARS_MST_CUED_OBSERVATION_FAILURE              147// ☆观测失败标识
#define MARS_PHASE_BEGIN                               148//☆阶段开始
#define MARS_PHASE_END                                 149//☆阶段结束
#define MARS_ASSET_INDEPENDENT_ISSUING_TASK_ASSIGNMENT  197// ☆平台独立任务指派
#define MARS_ASSET_INDEPENDENT_ORDERED_ISSUING_UPDATE   198// ☆平台独立任务更新
#define MARS_ASSET_INDEPENDENT_ISSUING_TASK_CANCELATION 199// ☆平台独立任务取消
#define MARS_ASSET_INDEPENDENT_TASK_COMPLETED           200// ☆平台独立任务完成
#define MARS_ASSET_INDEPENDENT_UNABLE_TO_DO_TASK        201// ☆平台独立无法指派任务
#define MARS_FLIGHT_OP_BEGIN                            210// ☆飞行运动开始*****************js
#define MARS_FLIGHT_OP_REPOSITION                       211// ☆飞行运动重新配置*****************js
#define MARS_FLIGHT_OP_LAUNCH                           212// ☆飞行运动起飞*****************js
#define MARS_FLIGHT_OP_RECOVERY                         213// ☆飞行运动回位*****************js
#define MARS_FLIGHT_OP_END                              214// ☆飞行运动结束*****************js
#define MARS_AIR_MISSION_FORMED                         218//☆空战任务成形
#define MARS_AIR_MISSION_REJOINED                       219//☆空战任务联合
#define MARS_USER_CUED_MSG_BEGIN                        220// ☆用户提示信息开始
#define MARS_USER_CUED_MSG_END                          221// ☆用户提示信息结束
#define MARS_TDMA_TIME_SLOT_REALLOCATION                226//☆通信时间空当分配
#define MARS_MST_MAX                                    230//☆
//自定义标识
#define MARS_CY_InitComplete_Message                    302//☆初始化结束信息（用于后台模型和dispose之间传递标志）
#define MARS_CY_UpdateTrack_Message                     303//☆真值航迹变化信息（引擎内部传递）
#define MARS_CY_AssetTacticSimMessage                   304//☆平台战术信息（未使用）
#define MARS_JS_SetUpTrack_Message                      306//想定中设置的航路信息
#define MARS_JS_SetUpArea_Message                       305//想定中设置的区域信息
#define MARS_JS_SENSORRANGE_Message                     308//☆传感器探测范围设置
#define MARS_JS_WEAPONRANGE_Message                     309//☆武器打击范围设置

//综合态势
#define MARS_ADD_GROUP_TRACK_NEW_Message                406// ☆群航迹新**暂不输出*****************zq 2014.8.5
#define MARS_ADD_GROUP_TRACK_UPDATE_Message             407// ☆群航迹更新**暂不输出*****************zq 2014.8.5
#define MARS_ADD_GROUP_TRACK_DROP_Message               408// ☆群航迹结束**暂不输出*****************zq 2014.8.5
#define MARS_ADD_SensorList_Message					    411//传感器列表信息*****************js 2014.8.5
#define MARS_ADD_SensorState_Message				    412//传感器状态信息*****************js 2014.8.5
#define MARS_ADD_SensorTargetInfo_Message			    413//☆传感器探测到的目标点迹信息*****************js 2014.8.12
#define MARS_ADD_WeaponState_Message				    421 //武器信息 hhl 2014.8.7
#define MARS_ADD_NaviInfo_Message					    431 //☆实体导航信息  jb 2014.8.18
#define MARS_ADD_WorkSpaceInfo_Message				    432 //想定信息  cy 2014.8.18
#define MARS_ADD_ExtOderBackInfo_Message			    433 //人工干预回执信息 
#define MARS_ADD_CommsInfo_Message					    434 //通信关系信息
#define MARS_ADD_CommandStructInfo_Message			    435//指挥结构信息 
#define MARS_ADD_DestroyInfo						    438 //☆平台销毁, 船舶宾馆2014.9.22
#define MARS_ADD_TimeSysInfo						    439 //时间同步, 船舶宾馆2014.9.22
#define MARS_ADD_Environment						    440 //环境信息, 船舶宾馆2014.9.22
#define MARS_ADD_DQEnvironment						    441 //大气环境信息,2015.1.13 暂时不用
#define MARS_ADD_HYEnvironment						    442 //海洋环境信息,2015.1.13 暂时不用
#define MARS_AIRPORT_PLANEDATA					        443 //飞机机场信息
#define MARS_ADD_PLATINFODATA					        444 //平台信息
#define MARS_ALLPLATOVER							    445 //平台初始位置发送完毕
#define MARS_EVALUATIONDAATA					        446 //评估信息
#define MARS_JAMMINGDATA						        447 //电子干扰信息
#define MARS_FREEWIREDATA                               448 //☆监控台发送的自由文电信息 LPS添加20150720（未使用）
#define MARS_RECEIPTDATA                                449 //☆收据信息（未使用）
#define MARS_ZKDATA                                     450 //☆指控武器分配信息
#define MARS_DATALINK                                   451 //数据链专用通信关系信息（与实装通信）
#define MARS_EXUIDATA                                   499 //用户自定义对外信息发送标识（目前给电子战模型专用）
#define MARS_UmpireCtrl                                 452 //裁决控制
#define MARS_MultiRunStatus                             453 //人工裁决信息
#define MARS_LANDMODEL                                  454 //陆战模型输出信息 2017.9.23
#define MARS_REGIONTRACKUMPIRE                          455 //越线越区裁决信息 2017.9.23
#define MARS_SHIPMOTORUMPIRE                            456 //机动裁决信息 2017.9.27

#define MARS_MIW_MINE_DELTA_DETAIL                  457//,"水雷对抗数据更新",MS_MIWSimMessage);
#define MARS_MIW_MINE_MCM_ACTIVATED                458//,"水雷激活信息",MS_MIWSimMessage);
#define MARS_MIW_MINE_MCM_DEACTIVATED              459//,"水雷不激活信息",MS_MIWSimMessage);
#define MARS_MIW_MINE_LAY_START                     460//,"水雷开始布放",MS_MIWSimMessage);
#define MARS_MIW_MINE_LAY_END                       461//,"水雷结束布放",MS_MIWSimMessage);
#define MARS_MIW_MINE_LAY_DELTA                     462//,"水雷布放事件",MS_MIWSimMessage);
#define MARS_MIW_MINE_AREA_DATA                     463//,"水雷区域信息",MS_MineAreaInfo);
#define MARS_ADD_PLATSPCLDATA                     464//,"平台特殊动作信息
#define MARS_AMPHIBIOUSKEYEVENT                 465//两栖关键事件信息
#define MARS_DecoyInitData                                 466//,"诱饵信息",MS_WeaponInfo); 2019.7.16
/************************************************/
/*      2. 干预信息标识                                          
/************************************************/
//控制命令
#define GY_START				                        501 //开始
#define GY_PAUSE				                        502 //暂停
#define GY_SPEED				                        503 //速度
#define GY_TIME				                        504 //内部干预命令
#define GY_INIT				                            622 //外部发送-初始化
#define GY_INITREPLY	                            623 //外部接收-初始化回告
#define GY_END         		                        624	//外部发送-结束
#define GY_TIMEDATA         		                625	//初始化后发时间相关信息（持续时间、开始时间）
#define GY_INITSTATE	                            626 //外部接收-初始化状态
#define GY_REPLAY_READY                           627 //回放跳转准备完毕
//指挥干预
#define GY_FREE_ATTACK_ORDER                            505 //自由攻击
#define GY_TASK_CANCEL_ORDER                            506 //分配任务取消
#define GY_AIRVECTORCOMAND		                        507 //☆飞机指挥引导干预
#define GY_SHIPVECTORCOMAND		                        508 //☆对舰艇引导干预
#define GY_AIRCRAFTLAUNCH		                        509 //飞机起飞任务计划
//飞机运动干预
#define GY_AIRCRAFTTOAREA                               510 //飞机飞到指定区域
#define GY_AIRCRAFTTRACKTOAREA                          511 //飞机沿航线飞到另一区域
#define GY_AIRCRAFTTOTRACK                              512 //飞机航线运动
#define GY_SHIPTOAREA                                   513 //舰船到某区域运动
#define GY_AIRCRAFTRTB                                  514 //飞机返航
//武器干预
#define GY_LAUNCHWEAPON                                 515 //武器发射
//传感器干预
#define GY_SENSORCTRLONOFF                              516 //传感器控制开关

#define GY_ADDTARGET			                        517 //添加目标
#define GY_DELETETARGET			                        518 //删除目标

#define GY_ASSETUPDATA                                  519  //平台位置干预信息
#define GY_ENVIROUPDATA                                 520 //环境干预（未使用）
#define GY_ASSETVECTOR                                  521 //兵力引导干预
#define GY_RePortTextData                               522 //☆外部发送的提示信息
#define GY_DataRequest	                                523 //外部发送信息请求
#define GY_DZDKLD				                        524 //电子对抗干预
#define GY_AIRCRAFTTRACKRTB                             525 //飞机沿航线降落    
#define GY_AIRCRAFTAREAARRIVE                           526 //飞机到达指定区域
#define GY_LAUWEAPONTRACK                               527 //武器航路干预（武器沿航线发射） 2016.6
#define GY_UmprieCtrl                                   529 //人工裁决控制命令
#define GY_LAUNCHMission                                530 //飞机巡逻干预
#define GY_AirAttackMission								531 //飞机打击任务
#define GY_ShipAttackMission							532 //舰艇打击任务
#define GY_MISSILETARSEC                                533 //导弹截获状态字干预
#define GY_DecoySend                                    534 //诱饵发射干预
#define GY_PointDefense                                 535 //点防御干预
#define GY_AssetAtributeMotify                          536 //平台属性变更
#define GY_ADD_Environment						        537 //环境信息, 船舶宾馆2014.9.22
#define GY_FUSION						                538 //识别属性干预, 2017.3.30
#define GY_FUSIONADD                                    539//融合信息注入 2017.6.1（包括新建、更新、撤批）
#define GY_FUSIONCLEAR                                  540//外注融合信息清空 2017.6.9
#define GY_ENGAGEUMPIRE                                 541//人工还是自动裁决干预 2017.10 JS
#define GY_WEAPONNUMMOD                                 542//武器数量更改干预
#define GY_COMMSSET                                     543//通信通断设置 2017.11.3 JS
#define GY_MINE                                         597//水雷干预 2017.9.6
#define GY_LBATT                                        598 //陆战干预信息 2017.9.5
#define GY_EXTINFO                                      599 //其它干预命令标识，结构由用户自定义 2016.8
#define GY_QtStatusData                                 600 //☆监控设备状态信息
#define GY_StateRequest	                                601 //外部发送状态请求
#define GY_StateRevert	                                602 //外部发送状态回告
#define GY_FZYLLaunch                               544 //复杂鱼雷武器发射 2017.12.2 CY
#define GY_FZYELaunch                                   545 //复杂诱饵发射 2017.12.2 CY
#define GY_TACTICSSET                                    546//战术更改 2018.9.6 JS


//外部控制命令
#define WB_GY_START				                        701 //开始
#define WB_GY_PAUSE				                        702 //暂停
#define WB_GY_SPEED				                        703 //速度
#define WB_GY_TIME				                        704 //内部干预命令
#define WB_GY_INIT				                        705 //外部发送-初始化
#define WB_GY_INITREPLY									706 //外部接收-初始化回告
#define WB_GY_END         		                        707	//外部发送-结束
#define WB_GY_TIMEDATA         							708	//初始化后发时间相关信息（持续时间、开始时间）
//多分支推演外部控制命令


//***Venus**系统需要的消息头//记录回放文件使用
#define VNR_GY_ASSETVECTOR                     1100   //兵力引导干预
#define VNR_GY_MOVETOPOINT                     1101   //到达指定点
#define VNR_GY_MoveToPointByTrack_Venus        1102   //沿航线到达指定点
#define VNR_GY_MoveToAreaByTrack_Venus         1103   //沿航线到达指定区域
#define VNR_GY_GroupChange_Venus               1104   //编队队形变换
#define VNR_GY_WEAPON_LAUNCH_Venus             1105   //对单目标的攻击
#define VNR_GY_SENSORCTRLONOFF                 1106   //传感器控制开关
#define VNR_GY_WEAPON_LAUNCH_TRACK             1107   //武器沿航路打击
#define VNR_GY_AIRCRAFTRTB                     1108   //飞机返航
#define VNR_GY_TACTICSSET                      1109   //战术干预
#define VNR_GY_AirAttackMission				  1110   //飞机打击任务
#define VNR_GY_AIRCRAFTLAUNCH		          1111   //飞机起飞
#define VNR_GY_ACCOMPANYSHIPCTRL_VENUS		  1112   //伴舰飞行干预_已起飞飞机
#define VNR_GY_ACCOMPANYSHIPMISSION_VENUS	  1113   //伴舰飞行干预_未起飞飞机
#define VNR_GY_ACCOMPANYAIRCTRL_VENUS		  1114   //伴机飞行干预_已起飞飞机
#define VNR_GY_ACCOMPANYAIRMISSION_VENUS		  1115   //伴机飞行干预_未起飞飞机
		
#define VNR_GY_LAUWEAPONTRACK                  1116   //武器航路干预（导弹打击）
#define VNR_GY_PatrolToPoint                   1117   //定点巡逻
#define VNR_GY_SearchPlan_Region		          1118   //飞机警巡
#define VNR_GY_JAMMING	                      1119   //电子干扰
#define VNR_GY_WEAPON_ATTACK_TOMULTI           1120   //对多目标的攻击
#define VNR_GY_FLIGHT_REFUEL                   1121   //空中加油
#define VNR_GY_LAUNCH_DECOY                    1122   //释放诱饵
#define VNR_GY_LAUNCH_SONBUOY                  1123   //释放声呐浮标
#define VNR_GY_VECTOR_COMMAND                  1124   //批量指令
#define VNR_GY_WEAPON_LAUNCH                   1125   //手动开火

//********************************************************************
//the size of "unsigned long" is 4 on windows and 8 on linux,
// we replace all the unsigned long with MARS_Uint32
//and did something bellow
//wzh changed on 2020.10.10 in Peking
//********************************************************************
#ifndef WIN32

#define MARS_Uint32 unsigned int
#define MARS_Uint64 unsigned long
#else
#define MARS_Uint32 unsigned int
#define MARS_Uint64 unsigned long
#endif



/************************************************/
/*      3. 其它信息标识                                   
/************************************************/

#define MultitudeRunPercent                             1001	//☆多次运行仿真时间标识
#define RequestENV                                      1002	//环境请求
#define ENVMode                                         1003	//环境模式


//平行仿真需要
#define VN_GY_FIRE_AUTO                                1200//对目标主动开火，打死为止-wzh add on 2020.12.4 平行仿真课题需要


/************************************************/
/*      4. 多分支推演所需的干预与消息头
/************************************************/
#define MULTI_ENGINECTRL_GY                            2001//多分支引擎运行控制
#define MULTI_ENGINE_INFO                              2002//多分支引擎信息
#define MULTI_CHANGE_CURENGINE						   2003//切换当前引擎
#define MULTI_ENGCTRL_JUMP						       2004//引擎运行时间跳转
#define MULTI_REPLAY_READY						       2005//回放准备完毕
#define MULTI_ENGINE_READY                             2006 //多分支引擎准备好
#pragma pack (push ,1) 

/************************************************************************/
/*                     一、信息发送类接口                                
/************************************************************************/
/**************************************/
/*            0.初始发送信息        */
/*                               */
/*   仿真开始前对外发送的初始信息，如 */
/* 指挥结构、通信关系、机场飞机等。   */
/**************************************/
/********************/
/* 0.总报文结构
/********************/
struct Z_SendHeader{
	unsigned short	iMessageType;	//信息标识
	double  dfTime;				//时间
	int		ilength;				//本包信息长度
};
//0.1
struct Lng_Lat{
	double    dfLon;				//经度
	double    dfLat;				//纬度
	double	  dfAlt;
};

struct TrackPoint{
	double   dfLng;				    //经度
	double   dfLat;				   //纬度
	float    fAlt;					       //高度
	float    fSpeed;				   //速度
	float    fPause;				       //暂停时间
};
/*****************/
/*0.1 通信组网信息
/*****************/

struct MARS_CommsNetInfo									//信息标识：MARS_ADD_CommsInfo_Message
{
	unsigned short      Flag;								//0-红方,1-蓝方 作为干预命令时，为命令号
	char				Rcvbuff[NLENTH];					//接收方
	char			    Sndbuff[NLENTH];					//发送方
	unsigned short      Value;								//是否连通 0-未连通1-连通
};	

/*****************/
/*0.2 指挥结构信息
/*****************/

struct MARS_CommdStructInfo						//信息标识：MARS_ADD_CommandStructInfo_Message
{
	int     flag;								//0-红1蓝
	unsigned short	Type;						 //1-指挥所2-平台3-任务指挥
	char	name[NLENTH];						//节点名称
	char	parentName[NLENTH];					//父节点名称 
	char    centerPlatName[NLENTH];				//指挥所的指挥中心
};

/*****************/
/*0.3 战术表信息
/*****************/

struct MARS_WorkSpaceInfo						//信息标识：MARS_ADD_WorkSpaceInfo_Message
{												//________________________________________________
	int  iCIU;									//0-机场    |  1-飞机    | 2-战术表    | 3-呼号
	char name[NLENTH];							//机场名称  |   飞机名称 |  战术表名称 |  呼号名称
	char subname[NLENTH];						//飞机名称  |   挂载方案 |  无效       |  无效
};												//————————————————————————
/*****************/
/*0.4 区域航线信息
/*****************/

struct MARS_TrackAreaInfo						//信息标识：MARS_JS_SetUpTrack_Message，MARS_JS_SetUpArea_Message
{
	char		name[NLENTH];					//航线（区域）名称
	int			iPointNum;						//点数
	Lng_Lat 	sPoint[GY64B];					//边界点
	int        iAreaType;                       //区域类型 0蓝1红2绿
};
/*****************/
/*0.5 水雷区域信息
/*****************/
struct MineInfo 
{
	char   mineName[NLENTH];
	int    count;
};
struct MARS_MineAreaInfo						//信息标识：
{
    char		name[NLENTH];				//水雷区域名称
    Mars_Alliance      alliance;			//联盟名称
    int	    mines_deployed_at_start;       //仿真开始时部署
    int      mineListNum;				//水雷种类
    MineInfo  mineList[GY8B];				//水雷列表
};
struct MARS_MIWSimMessage
{
    char MineFieldName[NLENTH];				//水雷区域名称
    char PlanName[NLENTH];				//计划名称
    Mars_Alliance Alliance;				//实体联盟名称
    char EntityName[NLENTH];				//实体名称
    char MCMSystem[NLENTH];				//水雷对抗系统名称
    double mineContacts;
    double mineIdentifications;
    double mineNeutralizations;
    MineInfo mineContactsHash[GY8B];		//用mineName为“”空来判断有效数量
    MineInfo mineIdentificationsHash[GY8B];
    MineInfo mineNeutralizationsHash[GY8B];
    double searchCompletion;
    double identifyCompletion;
    double neutralizationCompletion;
    MineInfo groundTruthTotals[GY8B];
    MineInfo minesLaid[GY8B];
};
/*****************/
/*0.6 传感器探测范围
/*****************/

struct MARS_SensorRange_ADD
{
	char           cSensorname[NLENTH];			//传感器名称
	double       	  dfVecRange;			        //最大探测范围  单位：海里
	double          dfMinRange;				    //最小探测范围  单位：海里
	double          dfCenterB;				    //中心线方位
	int             iRecativeFlag;				//是否是相对方位1-是0-不是
	double          dfWithB;					//视线宽度，单位：度
	int             iType;						//类型：1-柱状2-楔形3-双楔形4-球形
	int             iAlive;						//是否激活标志 0关机 1开机 2 探测范围降为0.3倍 4 探测范围恢复  5 传感器搜索开始  6 传感器搜索结束
};
struct MARS_SensorList_ADD
{
	char            cPlatname[NLENTH];			//传感器所在平台名称
    MARS_Uint32   uID;						//平台ID
	char            cSensorType[GY16B];			//传感器类型 “低分辨率” “中分辨率”
	MARS_SensorRange_ADD  cSensor[GY16B];		//传感器列表
};

/******************/
/*0.7 武器状态信息
/******************/
struct MARS_Weapon
{
	char     Weaponname[NLENTH];					//武器名称 
	int      WeaponNum;							//某武器数量  
	float    WeaponSpeed;                       //武器速度
	float    fShotRange;						//武器射程（海里）
};
struct MARS_WeaponInfo
{
	double     dfTime;   //时间
	char       platname[NLENTH];					//武器所在平台
#ifndef WIN32
    unsigned int uID;
#else
    MARS_Uint32   uID;						//平台ID
#endif
	char       WS_name[NLENTH];					//武器系统名称
	unsigned short   iWTNum;	
	MARS_Weapon vecWeapon[MACTABLE];	
};

/**************************************/
/*            1.定时发送信息          */
/*                                    */
/*   仿真开始后，每个仿真步长对外发送 */
/* 信息。							  */
/**************************************/

/*****************/
/*1.1 时间步长信息
/*****************/
struct TimeStepMessage							//信息标识：MARS_GENERAL_TIMESTEP
{
	double			dfTime;						//时间
#ifndef WIN32
    unsigned int dfStep;
#else
    MARS_Uint32   dfStep;						//步长（秒）
#endif
	unsigned short			wYear;
	unsigned short			wMonth;
	unsigned short			wDay;
	unsigned short			wHour;
	unsigned short			wMinite;
	unsigned short			wSencond;
};
/*****************/
/*1.2 机场飞机信息
/*****************/
struct AirPlaneData								//信息标识：MARS_AIRPORT_PLANEDATA
{
	double    TIME;								// 时戳	 
	char      airportName[NLENTH];				// 机场名称	
	int       PlanTypeNum;						// 飞机种类数量	
};
struct AirPlaneData_Sub							//与上面的结构动态链接在一起，PlanTypeNum为下面数组个数
{
	char      PlaneName[NLENTH];				// 飞机名称  拥有多种飞机时，此2字段重复出现
    int       PlaneNum;							// 飞机数量  2字节
	int        PlaneInitNo;							//飞机的起始编码  三位数字,cy 2016.11.21
};	


/*****************/
/*1.3 平台状态信息
/*****************/
struct MARS_PlatInfoDataExcect   	//信息标识：MARS_ADD_PLATINFODATA
{
	double  time;								    //时间  单位：时
	char	PlatName[NLENTH];				//平台名称
    MARS_Uint32   uID;						//平台ID
	double  fuel;								    //油料  单位：磅
	double  damageper;					    //生命值
	Mars_OpsStatus      OpsStatus;	//状态
	Mars_Alliance	    Alliance;				//联盟名称 
	Mars_Operating_medium  	Operating_medium;	//运动介质
	Mars_IconType	    Icon_type;		//类型 
	float	Speed;								        //速度  单位：米/秒
	float	Bearing;							        //航向  单位： 度
	double	Lat;								    //纬度
	double	Lng;								    //经度
	double	Alt;								    //高度
	double	Cw;									//艏向角(原航向)
	double	pitch;								//纵摇角,艏高为正,艏低为负
	double	roll;								    //横摇角,左舷高为正,右舷高为负
	char	    BasePlatName[NLENTH];	    //数据准备时的平台名称
	char      HullName[NLENTH];		//飞机舷号**必填项,默认值为“None”;如果是武器的话，该字段为武器所在平台舷号
	char      cStandBy[NLENTH];         //备用字节，岸导火力单元号 2016.7.29
	int         iIconNum;                        //军标号 2016.8.25 陆战需要
	int         iIconLib;                           //军标库号
	int         iVisible;                            //是否显示（海图上）
	double  maxSpeed;						//平台最大速度   -1为无效值   单位：节
};
/*****************/
/*1.4 平台特殊状态信息
/*****************/
struct MARS_PlatSpecialStatusInfo//信息标识：MARS_ADD_PLATINFODATA
{
	double			time;						    //时间
	int              iDex;							//流水号 从1开始
	char				PlatName[NLENTH];				        //平台名称
    MARS_Uint32		uID;						    //平台ID
	char            equpName[NLENTH];						//设备名称 可以用该名称来匹配该事件的“开始-结束”
	char            targetName[NLENTH];					//目标名称
	int              iStatusType;				    //状态类型  1-舰炮发射 2-实施有源电子干扰 3-无源干扰弹
	double			dfMaxRange;					//最大范围（海里）
	double			dfMinRange;					//最小范围（海里）
	int             iRecativeFlag;					//是否是相对方位1-是0-不是
	double			dfCenterB;						//中心线方位
	double			dfWithB;						//宽度，单位：度
	int              iExeStatus;						//执行表示  1-开始  2-结束
};
/*****************/
/*1.4 大气环境信息
/*****************/

struct  MARS_AtmosphereEnviroment
{
    MARS_Uint32   dwMonth;				// 月份  1-12月
	double  dfLat;							// 点经度
	double  dfLon;							// 点纬度
	float   fAtmoTemp;						// 温度  摄氏度
	float   fAtmoDensity;					// 密度  kg/m3
	float   fAirpress;						// 气压  帕斯卡
	float   fHumidity;						// 相对湿度
	float   fVisibility;					// 能见度  公里
    MARS_Uint32   dwCloudcover;			// 云量  成数
    float   fWindCourse;					// 风向  度
    float   fWindSpeed;						// 风速  米/秒
    MARS_Uint32   dwIllum;				// 光照  0：白天 1：黑夜
};
/*****************/
/*1.5 海洋环境信息
/*****************/

struct  MARS_OceanEnviroment 
{
    MARS_Uint32   dwMonth;				// 月份  1-12月
	double  dfLat;							// 点经度
	double  dfLon;							// 点纬度
    float   fDeepth;						// 水深  米
	float   fMaxTide;						// 高潮潮高  米
	float   fMaxTideTime;					// 高潮时刻  时
	float   fMinTide;						// 低潮潮高  米
	float   fMinTideTime;					// 低潮时刻  时
	float   fCurrentCourse;					// 海流方向  度
	float   fCurrentSpeed;					// 海流速度  米/秒
	float   fOceanWaveHeight;				// 海浪高度  米
	int     nOceanButtom;			        // 海底底质  1：淤泥；2：细沙；3：中沙；4：粗沙；  
	float   FOceanTemp;						// 海水温度  摄氏度
	float   fSeaSalinity;					// 盐度  
	float   fSeaDensity;					// 密度  kg/m3
	int     SeaState;                       // 海况  1：1级；2：2级；3：3级；4：4级；5：5级；
};

//地理数据
struct  SGeoData
{
	double          dfLat;			// 点经度
	double          dfLon;		    // 点纬度
	int             iDuliFang;			             // 是否有独立房等点状障碍物，0否，1是  
	int             iYabiDiba;			             // 是否有崖壁\堤坝等线装障碍物   0否，1是 
	double     dfAlt;                              //点高程
	double     dfGrade;                        //坡度
	int             iJuMinqu;                      //区民区类型  130204 普通街区 ;0 无
	int             iDiZhi;                            //底质类型	161205  干出滩/161206  沙泥滩/161207  沙滩 /161208  泥滩                  
	                                                         // /161209  沙砾滩 /161210  砾滩/161211  磊石滩 /161212  岩石、珊瑚滩                                    
		                                                     // /161213  岩石滩 /161214  珊瑚滩/161221  植物滩/160401  常年湖                
	                                                         // /160402  时令湖/160403  池塘/160405  水库/161102  大陆陆地/161103  岛屿、明礁            
	                                                         // /161105  洲/160601  盐田/160602  沼泽地 /0 没有居民地/-1 错误
	int              iZhiBei;  	                     //植被类型   /	220101  森林 /220102  矮林\幼林\苗圃 /	220103  疏林/	220111  灌木林/	220112  密集灌木林
	                                                         // /	220113  稀疏灌木林 /	220121  竹林/	220125  经济林/	220201  稻田/	220205  经济作物地 
	                                                         // /	220206  旱生作物地/	220207  水生作物地/	220210  高草地/	220211  草地/	220215  空地
	                                                         // /0 没有居民地/-1 错误
};
//水声跃变层信息
struct EnvSDeepAndSpeed 
{
	float	fSeaDeep;	//深度
	float	fSeaSpeed;	//声速
};

//环境数据结构，-999为空值,除地理数据
struct  MARS_Enviroment
{
	unsigned short             ID;
	unsigned short             nYear;			 // 年
	unsigned short             nMonth;		     // 月  1-12月
	unsigned short             nDay;		     // 日  1-31
	unsigned short             nHour;			 // 时  1-24
	double                     dfLat;			 // 点经度
	double                     dfLon;		     // 点纬度
	//大气环境参数
	float                      fAtmoTemp;	     // 温度  摄氏度
	float                      fAirpress;		 // 气压  帕斯卡
	float                      fHumidity;		 // 湿度
	float                      fWindSpeed;		 // 风速  米/秒
	float                      fWindCourse;		 // 风向  度
	float                      fCloudcover;		 // 云厚（云量  成数）
	float                      fCloudHigh;		 // 云高
	float                      fVisibility;		 // 能见度  米
	char			           chWeatherPheno[NLENTH];//天气现象	用字符串形式描述天气基本状况，例如“大风天气”、“暴雨天气”、“扬沙天气”。包含结束符‘\0’
	int                        nGZ;              // 光照 0白天 1晚上
	//海洋环境参数
	int                        SeaState;         // 海况  1：1级；2：2级；3：3级；4：4级；5：5级；☆
	float                      fOceanTemp;		 // 海水温度  摄氏度
	float                      fSeaSalinity;	 // 海水盐度 ☆ 
	float                      fSeaDensity;		 // 海水密度  kg/m3☆
	float                      fSeaAirpress;	 // 海面气压  帕斯卡☆
	float                      fSeaWindSpeed;	 // 风速  米/秒
	float                      fSeaWindCourse;	 // 风向  度
	float                      fSeaVisibility;   // 海面能见度  米
	float                      fSeaHumidity;	 // 相对湿度
	double	                   dfTideHigh;		 // 潮汐值
	int                        iSSYBC;           // 水声跃变层数
	EnvSDeepAndSpeed           sDeepSpeed[GY40B];// 水声跃变数据
	int                        nOceanButtom;	 // 海底底质  1：淤泥；2：细沙；3：中沙；4：粗沙；  
	float                      fDeepth;			 // 水深（深度）  米
	float                      fMaxTide;		 // 高潮潮高  米
	float                      fMaxTideTime;	 // 高潮时刻  时
	float                      fMinTide;		 // 低潮潮高  米
	float                      fMinTideTime;	 // 低潮时刻  时
	float                      fCurrentCourse;	 // 海流方向  度    0~360
	float                      fCurrentSpeed;	 // 海流速度  米/秒
	float                      fTideCurSpeed;    // 潮流流速  米/秒
	float                      fOceanWaveHeight; // 海浪高度  米
	unsigned char               EnvFlag;          // 环境标识  (0,区域环境；1，平台环境；2，城市环境；-1，无效值）
    MARS_Uint32               EnviID;           // 平台ID或区域号或城市ID
	char                      cPlatName[NLENTH];// 平台名称，用于环境编辑，在模型中未使用
};

//环境数据请求信息
struct  SRequestEvn
{
	unsigned short          EnvFlag;// 环境标识  (0,区域环境；1，平台环境；2，城市环境；-1，无效值）
    MARS_Uint32          EnvID;              // 平台ID或区域号或城市ID
	char                 cPlatName[NLENTH];  // 平台名称
	char                 cReName[NLENTH];    // 信息请求方
    Lng_Lat               sPoint;             // 请求点
    MARS_Uint32	        dwDate;			    // 日期 20041020
	unsigned short          nHour;				// 时   1-24
};

/*****************/
/*1.6 电子干扰信息
/*****************/
struct pair_point 
{
	double    dis;//距离，单位：海里
	double    rag;//角度，单位：度
};
struct  MARS_JamingData     //MARS_JAMMINGDATA
{
	int iJamType;//干扰类型  1：雷达干扰  2：通信干扰  3：导弹干扰
	int iJamModel;//干扰模式   如果是雷达和通信 1：噪声压制 2：假目标 3：欺骗  如果是导弹 1：有源干扰 2：冲淡箔条 3：质心箔条 4：曳光弹 5：遮蔽弹	
	double  time;
	char	AttackName[NLENTH];					//攻击平台名称
	char	TargetName[NLENTH];					//干扰目标平台名称
	char	SystemName[NLENTH];					//系统平台名称
	int     iFlag;								//干扰标识， 1-开始；0-结束
	int		iNum;								//如果是雷达通信：代表不规则图形点的个数    如果是导弹：无意义
	pair_point point[GY64B];					//点
};
/*****************/
/*1.8 人工裁决控制信息
/*****************/
struct  MARS_SUmpireCtrl
{
	int          iNo;				//流水号
	int          umpiretype;	//裁决类型                               0-交战裁决
	char       PlatName[NLENTH];//平台名称
	char       WeaponName[NLENTH];//目标名称
	char       TargetName[NLENTH];//目标名称
	int		  Resault;		//裁决结果                                   1-命中0-未命中2-命中有毁伤
	double   value;			//值
	double   fwaittime;	//等待时间
	char       DestroyName[NLENTH];//目标名称
};
/**************************************/
/*            2.随机发送信息          */
/*                                    */
/*   仿真开始后，不定时的对外发送信息 */
/* 通常为事件信息。					  */
/**************************************/

/*****************/
/*2.1 文字提示信息
/*****************/
struct InitReport						//信息标识：MARS_CY_InitComplete_Message
{
	char		vectorData[GY128B];		//内容
	int  		iLenth;					//报文长度
	int         iFLag;					//备用 //0-正在初始化中的报文 1-初始化完成标识 2-其他提示信息3-结束信息
	double      simtime;				//仿真时间
};

/*****************/
/*2.3 通信事件信息
/*****************/
struct MARS_MsgSimMessage					//信息标识：MARS_TRANSMIT_NORMAL
{
	char OriginSender[NLENTH];				//通讯发送源名称
	char Receiver[NLENTH];					//接收节点名称
	float SenderLat;						//发送者纬度
	float SenderLng;						//发送者经度
	float SenderAlt;						//发送者高度  单位：英尺
};
/*****************/
/*2.4 平台战术信息
/*****************/	

struct MARS_AssetTacticSimMessage				//信息标识：MARS_CY_AssetTacticSimMessage
{
	char Asset[NLENTH];							//平台名称
	char CallSign[NLENTH];						//呼号
	char ActualTarget[NLENTH];					//目标
	char TargetMedium[NLENTH];					//目标介质
	char PerceivedClassification[NLENTH];		//分类
	int  TrackId;								//航迹ID
	char TacticsTable[NLENTH];					//战术表
	char Tactic[NLENTH];						//战术
	char Response[NLENTH];
};
/*****************/
/*2.5 任务执行信息
/*****************/	
struct MARS_StrikeSimMessage					//信息标识：MARS_STRIKE_GENERIC
{
	char StrikeTask[NLENTH];					//打击任务
	char StrikingSite[NLENTH];					//打击点
	char Target[NLENTH];						//打击目标
	int  iRow;									//表行数
	char table_message[MACTABLE][GY128B];		//列表		
};
/*****************/
/*2.6 探测信息
/*****************/	
enum MARS_BDA_SUMMARY { 
	MARS_BDA_Operational,			//可用
	MARS_BDA_Damaged,			//损毁
	MARS_BDA_Destroyed			//消失
};
struct MARS_DetectSimMessage					//信息标识：MARS_DETECT_START,MARS_DETECT_PROCESSING,MARS_DETECT_DELAYED_START
{
	float	        fTime;						                //点迹生成时间,单位:时
	char            cSensorname[NLENTH];		//传感器名称
	char            cPlatname[NLENTH];			//传感器所在平台名称
    MARS_Uint32   uID;						       //平台ID
	char            cSensorType[GY16B];		   //传感器类型 “低分辨率” “中分辨率”
	char            cTargetname[NLENTH];	   //目标名称
    MARS_Uint32   uTargetID;				   //目标平台ID
	Mars_Classification            classification;		   //目标属性 敌、中立、友、未知
	Mars_Identification            identification;	   //目标种类
	Mars_Operating_medium            operating_medium;	   //目标
	double		    dfLon;						       //目标经度     单位: 度   范围: -180~180  , 东经为正
	double		    dfLat;						       //目标纬度     单位: 度   范围: -90~90°  , 北纬为正
	float				fAlt;						                   //目标海拔高度,范围正负均可, 单位: 米  
	float				fCm;						                   //目标航向	   单位: 度    范围: 0~360        
	float				fVm;						                  //目标航速     单位: 米/秒 范围: >0
	float				fElveAngle;					      //目标高度角   单位: 度
	float				fBearing;					          //目标方位     单位: 度    范围: 0~360 
	float				fDm;						              //目标距离     单位: 米    范围: >0 
	double			BdaTime;					      //毁伤时间
	MARS_BDA_SUMMARY Summary;	  //毁伤类型
	int				Confidence;					 //毁伤条件
	double			PercentDamaged;		 //毁伤百分比
    char			Processor[NLENTH];			//探测主体
	int				TrackQuality;				    //航迹质量
	int             iUID;						            //航迹号
	//////////////////2016.8新增/////////////////////////////////////////////////////////
	unsigned char cDetecterType;                         //探测源类型      1-雷达、2-光电、3-声纳、4-潜望镜、5-指控
	unsigned char cEventType;                               //事件类型   1开机、2加高压、3关高压、4关机、5捕获目标、6捕获目标消失、7目标识别
};
  
/*****************/
/*2.7 融合事件信息
/*****************/	
struct MARS_ExtTrackSimMessage						//信息标识：MARS_TRACK_NEW,MARS_TRACK_STOP,MARS_TRACK_RESUME,MARS_TRACK_UPDATE,MARS_TRACK_DROP
{
	double time;								//时间
	int  TrackNumber;							//融合批号
	int  PlatID;								//平台ID
	int  TargetID;								//实际目标ID					
	Mars_Classification Classifict;				//识别敌我属性
	Mars_Identification identification;			//目标类型
	double dfLon;
	double dfLat;
	double dfALt;        //高度  单位：米
	double dfCource;  //方向  单位：度
	double dfSpeed;   //速度  单位：米/秒
	Mars_Operating_medium  	Operating_medium;		//运动介质
	int  iStBy;//备用  融合干预时 0为无效，1为新建 2为更新 3为撤批
	int iFalse;//是否是假目标0否 1是
	unsigned short      wCommand;                  //命令号
};
/*****************/
/*2.9 飞机起飞信息
/*****************/	

struct ACPublicSimMessage
{
	char Airbase[NLENTH];		 				//空军基地
	char Aircraft[NLENTH]; 						//飞机名称
	char SortieType[NLENTH];					//出击类型	
	char WarfareCommanderName[NLENTH];			//作战任务指挥
	char MissionType[NLENTH];					//任务类型
	char MissionPlanName[NLENTH];				//任务方案名称
	char CallSign[NLENTH];						//呼号	
	char OrderId[NLENTH];						//作战命令编号
};
struct MARS_AircraftLaunchSimMessage			//信息标识：MARS_AC_LAUNCHING
{
	ACPublicSimMessage sACPublicSimMessage;		//作战基本信息
	char DliId[NLENTH];							//数据链编号
	Lng_Lat LaunchPoint; 						//起飞位置（*）界面显示为 纬度、经度
	Lng_Lat OnStationPoint;						//预计待战位置（*）
	Lng_Lat OffStationPoint;					//预计离开待战区位置（*）	
	Lng_Lat LandPoint;							//预计着陆位置（*）	
	double InternalFuel; 						//总的内部油量（磅）（*）
	double ExternalFuel; 						//总的外部油量（磅）（*）
	double AdditionalFuel; 						//飞行中隐含补充油量（磅）（*）
    double dfTotalFuel;							//需要计算总的可获得油量（磅），新增，上述三个变量和；（*）
	double LaunchFuel; 		  					//起飞耗油（磅）（*）
	double RecoverFuel; 						//着陆耗油（磅）（*）
	double MissionReserveFuelFactor;			//安全保留（%20）的总油量
	double dfTotalConsumeFuel;					//要求保留的油量（磅），新增，上述三个变量之和（*）
	double dfPlanUsedFuel;						//任务可用油量（磅），新增，上述两个和的差（*）
	
	double IngressDistance;						//预计出航距离（海里）（*）
	double IngressSpeed;						//出航航行速度（节）（*）
	double OnStationTime;						//估计出航时间（秒）（*）需要转化为 时：分：秒
	double IngressTransitBurn; 					//出航航行平均耗油率（磅/小时）（*）
	double dfLaunchFuel;						//出航耗油（磅），负值，新增，计算结果加上负号，预计出航距离/航行速度*平均耗油率（*）
	
	double EgressDistance;						//预计返航距离（海里）（*）
	double EgressSpeed; 						//返航航行速度（节）（*）
	double OffStationTime; 						//估计返航时间（时）（*）需要转化为 时：分：秒
	double EgressTransitBurn; 					//返航航行平均耗油率（磅/小时）（*）
	double dfEgressFuel;						//返航耗油（磅），负值，新增，计算结果加上负号，预计返航距离/航行速度*平均耗油率（*）
	double StationSpeed; 						 //空域待战速度（节）（*）
	double StationBurn; 						 //空域待战初始耗油率（磅/小时）（*）
	double AdditionalStationReserveFuelFactor;  //待战保留（%10）的总油量（不包括安全保留的）（*）
	double dfStayTime;						    //估计在空域时间（时），新增，（任务可用油量-出航耗油-返航耗油）/空域待战耗油率（*）,需要转换为（时：分：秒）	
	double TransitSpeed; 						//运输速度（节）
	int NoFuel_p; 								//没有燃料标志
	int IsSearchPlan; 							//搜索计划
};

/*****************/
/*2.10 飞机降落信息
/*****************/	

struct MARS_AircraftLandSimMessage				//信息标识：MARS_AC_RECOVERY
{
	ACPublicSimMessage sACPublicSimMessage;		//作战基本信息（*）
	int Repair_p; 								//维修尝试要求（*），需要显示字符串"有""无"
	double TurnaroundTime; 						//等级转进时间（天）（*），显示需要转化为天：时：分：秒
	double AvailableTime; 						//可用时间（天）（*），显示需要转化为天：时：分：秒
	int Major_p; 								//
	double MaintainDelay; 						//维修延迟时间
	double MaintainTime;						//维修时间
};

/*****************/
/*2.11 飞机返回基地
/*****************/	

struct MARS_AircraftRTBSimMessage				//信息标识：MARS_AC_NO_AIRCRAFT
{
	ACPublicSimMessage sACPublicSimMessage;		//作战基本信息（*）
	char UserDefinedRTBReason[NLENTH];			//用户定义原因类型（*）
	MS_AIRCRAFT_RTB_REASON Reason;			//返回基地原因
};

/*****************/
/*2.13 飞机随风运动
/*****************/	
struct MARS_FlightOpsSimMessage					//信息标识：MARS_FLIGHT_OP_BEGIN
{
	char AssetName[NLENTH];						//设备名称
	char FlightOperationArea[NLENTH];			//飞行运动区域（区域名称）
	double WindDirection;						//风向（度）
	double ScheduleDuration;					//时间周期（时）
};

/*****************/
/*2.14 交战事件信息
/*****************/	

struct MARS_AttritionSimMessage						//信息标识：MARS_AMT_FUEL_EXHAUSTED,MARS_AMT_WPN_SUCCESS_HIT,MARS_AMT_WPN_MISS,MARS_AMT_WPN_WASTED,MARS_AMT_WPN_OUT_OF_RANGE
{
	double	Addl_damage; 							//附加损害
	double	Range; 									   //射失距离（海里）
	double	Att_range; 								  //攻击机与目标距离（海里）
	double	Pdh;		 							          //相关毁伤概率
	double	Cum_damage; 						  //附带损害
	int	    Missile_p; 								     //导弹标志
	char	Attacker[NLENTH];						     //攻击方
	char    attackerFlag[NLENTH];					//攻击平台联盟
	char    attackerType[NLENTH];				//攻击平台类型
	char	Weapon[NLENTH];							//武器/弹药
	char	Target[NLENTH];							//目标
	char	FieldSix[NLENTH]; 						   //
	char	Int_table[NLENTH];						   //使用的交互表
	char	Component[NLENTH]; 				   //成员/部件
	char    targetFlag[NLENTH];				   //被攻击平台联盟
	char    targetType[NLENTH];				   //被攻击平台类型
	/////////////////////////////////////////////////////////
	unsigned char cEngageResult	;                           //	交战结果  1-武器未击中、2-武器击中、3-武器成功毁伤目标、4-武器丢失目标、5-武器超出范围、6-武器被点防御系统毁伤、7-武器突破点防御系统、8-武器浪费、9-燃料耗尽、10-武器失效
	char chEngageResultDisb[GY40B]	;   //	交战结果描述
	char WeaponType[NLENTH];//武器型号
	int  ResNo;	//备用，如使用的武器编号
};

/*****************/
/*2.15 指挥命令信息
/*****************/	

struct MARS_CommandSimMessage
{
	char Task[NLENTH];								//任务
	char AssigningCommanderORAsset[NLENTH];			//被分配的指挥官或平台
	char AssignedCommander[NLENTH]; 				//被分配的指挥官
	char ID[NLENTH];  								//ID号
	char AssignedAsset[NLENTH]; 					//被分配的平台
	char Target[NLENTH]; 							//目标
	char Type[NLENTH]; 								//类型
	char SelfTarget[NLENTH]; 						//

};
struct MARS_ZKDataMessage
{
	double  time;								//时间
	int     iFlag;								//标识，1-信息上报，2-目标识别，3-数据融合，4-威胁判断，5-火力分配，6-下发目标指示
												//      7-收到目指，8-加电   9-加载诸元
	char    AssetName[NLENTH];					//本平台名称
	char    TargetName[NLENTH];					//目标名称
	char    WeaponSystem[NLENTH];					//武器系统名称
	double  intactTime;			//拦截时间
	double  dfLon;				//拦截经度
	double  dfLat;				//拦截纬度
	double  flyTime;			//飞行时间
	double  dfC;				//发射方位
	double  dfsendTime;			//发射时间
	int     iKillType;			//1-有软杀伤；0-没有软杀伤
	
};
/*****************/
/*2.16 指挥处理信息
/*****************/	

struct MARS_BaseCommandSimMessage 
{
	int GroupTrack_p;								//组航线
	char TheWarfareCommander[NLENTH];				//作战任务指挥
	int  CommanderTrackID;							//任务指挥所航迹ID
	char CommanderOrderID[NLENTH];					//任务指挥所命令ID
	char TaskedAsset[NLENTH];						//被分配的平台
	char TargetOperatingMedium[NLENTH];				//目标运动介质
	char TargetID[NLENTH];							//目标的感知ID
	char TargetClassification[NLENTH];				//目标识别分类
	int	 GroupTrackComponentCount; 					//组航线数
};

struct MARS_CommandProcessingSimMessage
{
	MARS_BaseCommandSimMessage BaseCommand;
	char m_ActualTarget[NLENTH];					//目标真实ID
	char m_AircraftType[NLENTH]; 					//飞机类型
	char m_CallSign[NLENTH]; 						//呼号
	char m_DLINumber[NLENTH]; 						//数据链编号
};

/*****************/
/*2.17 指挥任务完成
/*****************/	

struct MARS_CommandTaskCompletedSimMessage
{
	MARS_BaseCommandSimMessage BaseCommand;
	char OrderType[NLENTH];									//被命令的响应行动, hard kill, salvo, etc
	char ActualTarget[NLENTH];								//实际目标
};

/*****************/
/*2.18 指挥任务结束
/*****************/	
enum MARS_CommandTaskEndingReportIntention {
	NONE,
	SHORT_TERM,
	LONG_TERM,
	TRACK
};
struct MARS_CommandTaskEndingReportAckSimMessage 
{
	MARS_BaseCommandSimMessage BaseCommand;
	MARS_CommandTaskEndingReportIntention Intention;		// 指挥任务结束报告意图
	double Delay; 											//延迟
};

/*****************/
/*2.19 指挥命令信息
/*****************/	

struct MARS_OrderedCommandSimMessage 
{
	MARS_BaseCommandSimMessage BaseCommand;
	char OrderType[NLENTH];									//被命令的响应行动, hard kill, salvo, etc
	char MotionType[NLENTH];								//被命令的响应运动类型, none, simple, t&t etc.
	char ActualTarget[NLENTH];								//实际目标
};

/*****************/
/*2.20 指挥任务结束
/*****************/	

struct MARS_CommandTaskEndingSimMessage
{
	MARS_OrderedCommandSimMessage OrderedCommand;
	char m_Reason[NLENTH];									//结束原因
};

/*****************/
/*2.21 发布任务分派
/*****************/	

struct MARS_IssuingTaskAssignmentSimMessage {
	MARS_OrderedCommandSimMessage OrderedCommand;
	char CommanderTacticsTable[NLENTH];						//指挥战术表
	char TriggeredTactic[NLENTH];							//被触发的战术
};

/******************/
/*2.22 传感器状态改变
/******************/	
struct MARS_SensorState_ADD
{
	char            cPlatname[NLENTH];						//传感器所在平台名称
	char            cSensorname[NLENTH];					//传感器名称，有两种特殊的，如果是名称是“低（中）分辨率”，则该平台上的该类型全开机
	int       	    iTurnOn;								//开关机标志 0关机 1开机 2 探测范围变更 4 备用  5 传感器搜索开始  6 传感器搜索结束
	double          mrange;                                  //当iTurnOn==2有效
};
/******************/
/*2.23 干预回执信息
/******************/

struct MARS_ExteriorOderBackInfo
{
	double             currentTime;							// 当前时刻
	unsigned char      GYFLAG;								// 干预命令标识	1字节	同导调评估子系统发往配训子系统信息的"信息单元标识"
    MARS_Uint32      SENDTIME;							// 命令发送时刻	4字节	同导调评估子系统发往配训子系统信息的"时戳"
	unsigned short     FSFID;								// 发送方
    MARS_Uint32      ORDERNO;								// 命令编号	4字节	从0001开始，累计单个发送方的命令序号
	unsigned char      RESAULT;								// 执行结果	1字节	0：命令有误，1执行成功，2执行失败
	unsigned short     wCommand;							// 预留
	char	           buff[GY128B];						// 回执信息
};

/******************/
/*2.24 武器系统信息
/******************/

struct MARS_WeaponSystemSimMessage		//信息标识： MARS_MT_MUNITIONS_EXPENDED(武器发射信息)
															                        //           MARS_MT_MUNITIONS_FAILED(发射失败事件)
															                        //           MARS_MT_WINCHESTER(弹药耗尽事件)
															                         //           MARS_MT_EVALUATION(武器发射后评估事件)
															                         //           MARS_MT_OUT_OF_ENVELOPE(超出武器射程)
{
	char Attacker[NLENTH];									//攻击者
	char attackerFlag[NLENTH];							//攻击平台联盟
	char attackerType[NLENTH];							//攻击平台类型
	char Weapon[NLENTH];									//武器
	char WeaponType[NLENTH];						//武器类型
	char Target[NLENTH];									//目标
 	float InterceptTime;									   //拦截时间  单位：时
	double dfReachLng;
	double dfReachLat;
	double dfReachAlt;
	double RangeToTarget;							 //武器射程   单位：海里
	double WeaponRange;								 //武器到目标的距离  单位：海里
	char TrackId[NLENTH];								//航迹批号
	int TargetTrackId;										//目标航迹批号
	char WpType[NLENTH];								//武器类型，这里指武器模板类型
	/////////////////////////////////////////////////////////////////
    char WpSyName[NLENTH];						//武器系统名称
    char chEventDescribe[GY40B];				    //事件描述
	unsigned char cEventType;                                       //事件类型1-发射失败、-2武器发射、-3弹药耗尽、-4超出武器射程、-5目标跟踪丢失
};

/******************/
/*2.25 SAM武器信息
/******************/

struct MARS_SamSystemSimMessage
{
	char Asset[NLENTH];										 //平台
	char Callsign[NLENTH];									 //平台通信呼号
	char WeaponSystem[NLENTH];								 //武器系统
	char Weapon[NLENTH];									 //武器
	char systemName[NLENTH];								 //系统名称
	double delay;											 //延迟/拦截时间
	int	 rails;												 //航迹拐点的数量
	char Target[NLENTH];									 //目标
	char TrackId[NLENTH];									 //航迹批号
	int  TargetTrackId;										 //目标航迹批号
	float InterceptTime;									//拦截时间
	double dfReachLng;
	double dfReachLat;
	double dfReachAlt;
};

/******************/
/*2.26 毁伤信息
/******************/
struct DamageSimData
{
	char chComponentName[GY40B]	;   //	部件名称
	int  iAlivecomponentCount;				//	存活的部件数量
	int  iTotalcomponentCount;				//	所有部件数量

};

struct MARS_DamageSimMessage 							//信息标识： MARS_MST_COMPONENT_DESTROYED、MARS_MST_DAMAGED、MARS_MST_DESTROYED
{
	char      Asset[NLENTH];								//毁伤平台
	char      AssetType[NLENTH];					        //平台类型
	char      AssetFlag[NLENTH];							//平台联盟
	char      Attacker[NLENTH];								//攻击者
	char	  attackerFlag[NLENTH];								  //攻击平台联盟
	char	  attackerType[NLENTH];								  //攻击平台类型
	char	  AttackerCallsign[NLENTH];							 //攻击者通信呼号
	char      weaponName[NLENTH];							//武器名称
	char      Component[NLENTH];							//成员
	int       ComponentCount;								//成员数量
    MARS_Uint32 AssetID;									 //毁伤平台ID
	////////////////////////////////////////
	unsigned char cDamageResult	;                //	毁伤结果   1-平台部件毁伤、2-受压制、3-受压制恢复、4-平台损伤、5-平台损毁、6-平台被水雷损毁
	char chDamageResultDisb[GY40B];      //	毁伤结果描述
	char chAssetID[GY40B];              //	发射平台机舷号
	char chWeaponSysName[GY40B];       //	武器系统名称
	double fDamageLevel	;                //	目标生命值
	DamageSimData  sComponetData[GY16B];         //  部件毁伤数据
	int  ResNo;	//备用，如使用的武器编号
};

/******************/
/*2.27 文电信息
/******************/

struct MARS_TextMessage 									
{
    MARS_Uint32 CMNo;				                       //台位号
    MARS_Uint32 lenth;			                               //长度
	char          buff[1024];
};
/******************/
/*2.27 大样本运行监控信息
/******************/
struct MARS_DATA_RUNSTATUS
{
	char				scinarioName[256];			//想定名称
	int 				repNum;				//样本数;		
    MARS_Uint32 		seeds;				//种子数;
	double			pers;					//运行百分比;	
};
/************************************************************************/
/*                     二、人工干预类接口                                
/************************************************************************/

/**************************************/
/*            0.仿真进程控制信息      */
/*                                    */
/*   对仿真过程进行控制的信息。       */
/**************************************/

/******************/
/*0.1 仿真初始化命令
/******************/

struct SIM_CTRL_INIT
{
	unsigned short      wYear;
	unsigned short      wMonth;
	unsigned short      wDay;
	unsigned short      wHour;
	unsigned short      wMinite;
	unsigned short      wSencond;
	char      cScinarioName[GY128B];		//想定名称
};
/******************/
/*0.2 仿真过程命令
/******************/

struct SIM_CTRL								//包含仿真初始化回告
{
	unsigned short      wInitStatus;		//初始化回告时有效，0-成功1-失败
	double			    simtime;			//仿真总时间，单位：小时
};

/******************/
/*0.3 加速命令信息
/******************/ 

struct CTRLCOMMAND                          //二维界面的控制命令
{
	float     speed;
};
/**************************************/
/*            1.运动控制信息		  */
/*                                    */
/*   对仿真过程中，实体运动进行控制的 */
/* 命令。                             */
/**************************************/

/******************/
/*1.1 到达指定点
/******************/

struct MOVE_CTRL_POINT
{
	char      host_name[NLENTH];				//平台名称
	double    arrivetime;					//到达时间
	float     fLng;							//经度
	float     fLat;							//纬度
	float     fAlt;							//高度
	float     fSpeed;						//速度
};

/******************/
/*1.2 到达指定目标
/******************/

struct MOVE_CTRL_PLAT						//注：包含飞机返回基地
{
	char      host_name[NLENTH];				//平台名称
	char      arrive_name[NLENTH];			//到达平台目标
};

/******************/
/*1.3 到达指定区域
/******************/

struct MOVE_CTRL_AREA
{
	char      host_name[NLENTH];				//平台名称
	int       point_count;					//区域点个数
	Lng_Lat   area_point[GY8B];				//区域点数组
};

/******************/
/*1.4 更改仿真时间
/******************/

struct SIM_CTRL_TIME
{
	unsigned short      wYear;
	unsigned short      wMonth;
	unsigned short      wDay;
	unsigned short      wHour;
	unsigned short      wMinite;
	unsigned short      wSencond;
};

/******************/
/*1.5 新增加目标
/******************/

struct SIMPLE_TARGET
{
	unsigned short	  wCommand ;				//命令标识		0x07,该值为517时，添加的目标为实时态势目标，既需要不断的更新速度、航向、经纬度等
	char      name[NLENTH];						//平台名称，这是用户自己起的平台名称**必填项
	char      Alliance[NLENTH];					//联盟**必填项
	double    dfLng;							           //经度
	double    dfLat;							          //纬度
	double	  dCource;							 //航向  单位：度
	double	  trasSpeed;						 //速度  单位：米/秒
	double	  SearchAltitude;				//高度  单位：米
	double	  searchTime;						//航向时间  单位：时
	char      tacticname[NLENTH];				//战术名称
	char      PrePlatName[NLENTH];				//平台类型，这是数据准备中的平台名称**必填项
	char      HullName[NLENTH];					//平台舷号
};

/******************/
/*1.6 删除已有目标
/******************/

struct DELETE_TARGET
{
	unsigned short	  wCommand ;				//命令标识		0x10
	char      name[NLENTH];						//平台名称
};

/******************/
/*1.7 对兵力引导
/******************/

struct ALL_COMMAND_VECTOR
{
	int         PlatID;
	unsigned short	wCommand ;					    //命令标识		0x0100
	char		dwRecPlatName[NLENTH];				//接收平台名称
	double		dfDegree	;						//方向		 -1表示不控制    0 ~359.95  单位：度
	double		dfSpeed	;							//速度   -1表示不控制  单位：米/秒
	double		dfHigh	;							//高度   -1表示不控制  0~81900，仅飞机、潜艇有效  单位：米
	double		dfTime	;							//在当前给定的航向上继续航行时间 0~4094秒，-1表示不控制  单位：秒
	double      dfLon;								//当前经度
	double      dfLat;								//当前纬度
};

/******************/
/*1.7 对飞机引导
/******************/

struct AIR_COMMAND_VECTOR
{
	int         PlatID;
	unsigned short	wCommand ;					//命令标识		0x0100
	char		dwRecPlatName[NLENTH];				//接收平台名称
	double		dfDegree	;						//方向		 -1表示不控制    0 ~359.95
	double		dfSpeed	;							//应飞航速   -1表示不控制
	double		dfHigh	;							//应飞高度   -1表示不控制  0~81900
	double		dfGrade	;							//应飞坡度   -1表示不控制  0 ~ 90
	double		fTurning;							//坡度转向控制   0 右转  1 左转  2 改平  全1表示不控制
	double		dfTime	;							//在当前给定的航向上继续飞行时间 0~4094秒，-1表示不控制
};

/******************/
/*1.8 对舰艇引导
/******************/

struct SURFACE_COMMAND_VECTOR
{
	int         PlatID;
	unsigned short		wCommand    ;				//命令标识		0x0301
	char		dwRecPlatName[NLENTH] ;				//接收平台编号
	double		dwDegree	;						//方向 有效位0.05	   0 ~359.95
	double		dwSpeed     ;						//航速 最低有效位 0.5 节    0~127 节 全1表示不控制
	double      dfAlt;								//高度/深度,无效值-9999
	double		dwTime	    ;						//在当前给定的航向上继续航行时间 有效位1分 0~24，全1表示不控制
};

/******************/
/*1.9 飞机起飞
/******************/

struct AIECRAFTLAUNCHPLAN
{
	int        PlatID;
	unsigned short      wCommand ;					//命令标识		519
	char      dwRecPlatName[NLENTH];					//接收平台**必填项
	unsigned short	  wPlaneNum;					//起飞架数**必填项
	char      cAirCraftName[NLENTH];					//飞机名称**必填项
	char      cSortieType[NLENTH];					//挂载方案
	char      cCallSigns[NLENTH];					//呼号
	char      cTacticsTable[NLENTH];					//战术表
    MARS_Uint32  dwRegionNum;						//区域编号
	int       pointNum;								//航线点数**必填项
	TrackPoint pointarray[GY16B];					//飞机航线**必填项
	char      HullName[NLENTH];						//飞机舷号**必填项**
};

/******************/
/*1.10 实体位置更新
/******************/

struct POSITION_UPDATA_VECTOR
{
	int         PlatID;
	unsigned short		wCommand;					//命令标识		0x0100
	char		dwPlatName[NLENTH];					//接收平台名称
	double		dfLon;								//期望经度		 -1表示不控制    0 ~359.
	double		dfLat	;							//期望纬度       -1表示不控制
	double		dfAlt	;							//期望高度       -1表示不控制  0~81900 单位：米
	double      dfSpeed ;							//期望速度    单位：米/秒		 
	double      dfCource;							//期望航向	  单位：度	 
    MARS_Uint32		dfTime	;					//当前点时间,	 -1表示不控制
};

/******************/
/*1.10 自由攻击命令
/******************/

struct FREE_ATTACK_ORDER
{
	int       PlatID;
	int       TargetID;
	unsigned short      wCommand;					//命令标识		0x0008
	char      RecPlatName[NLENTH];					//接收平台名称
	char      RecPlatAlliance[NLENTH];				//接收平台联盟
	char      TargetName[NLENTH];					//目标平台名称
	char      TargetAlliance[NLENTH];				//目标平台联盟
};

/******************/
/*1.11 取消分配关系
/******************/

struct TASK_CANCEL_ORDER
{
	int       PlatID;
	int       TargetID;
	unsigned short      wCommand;					//命令标识		0x0009
	char      RecPlatName[NLENTH];					//攻击平台名称
	char      RecPlatAlliance[NLENTH];				//攻击平台联盟
	char      TargetName[NLENTH];					//被攻击平台名称
	char      TargetAlliance[NLENTH];				//被攻击平台联盟
};
/******************/
/*1.12 传感器开关机控制
/******************/

struct SENSOR_CTRL
{
	int      PlatID;
	unsigned short     wCommand;					//命令标识		0x0009
	int		 iFLag;									//0-关 1-开 2-缩小 3-恢复
	int      All_One;								//0-针对某个平台全关或全开，1-针对某平台上某一个传感器
	char     host_name[NLENTH];						//平台名称
	char     sensor_name[NLENTH];
};

/******************/
/*1.13 出航XX空域/海域
/******************/

struct AIRSHIP_MOVE_CTRL_AREA
{
	int         PlatID;
	unsigned short      wCommand;					//命令标识		0x0015
	char      host_name[NLENTH];						//平台名称
	int       point_count;							//区域点个数
	Lng_Lat   area_point[GY8B];						//区域点数组
};

/******************/
/*1.14 经过XX航线，出航XX空域/海域
/******************/

struct AIR_MOVE_TRACK_CTRL_AREA
{
	int         PlatID;
	unsigned short      wCommand;					//命令标识		0x0018
	char      host_name[NLENTH];						//平台名称
	int       point_count;							//区域点个数
	Lng_Lat   area_point[GY8B];						//区域点数组
	int       Tpoint_count;							//航线点个数
	TrackPoint   Track_point[GY16B];				//航线数组
};
/******************/
/*1.15 沿XX航线出航
/******************/

struct AIR_MOVE_CTRL_TRACK
{
	int       PlatID;
	unsigned short      wCommand;					//命令标识		0x0018
	char      host_name[NLENTH];					//平台名称
	int       Tpoint_count;							//航线点个数
	TrackPoint   Track_point[GY16B];				//航线数组
};

/******************/
/*1.16 飞机返回基地
/******************/

struct AIR_RTB
{
	int       PlatID;
	unsigned short      wCommand;           //命令标识		0x0112
	char      host_name[NLENTH];			//平台名称
	char      base_name[NLENTH];			//基地名称
};
/******************/
/*1.17 飞机沿航线返回基地   武汉航管
/******************/
struct AIRTrack_RTB
{
	unsigned short      wCommand;           //命令标识		0x0112
	char      host_name[NLENTH];			//平台名称
	char      base_name[NLENTH];			//基地名称
	int       Tpoint_count;					//航线点个数，后面跟Lng_Lat结构
};
struct AIRAreaArive
{
	unsigned short      wCommand;           //命令标识		0x0112
	char      host_name[NLENTH];			//平台名称
	int       Tpoint_count;					//区域点个数，后面跟Lng_Lat结构
};
/******************/
/*1.18 武器发射/点防御
/******************/

struct WEAPON_CTRL_LAUNCH
{
	short    PlatID;                    //兵力实体代号
	short    TgtID;                  //目标实体代号
	unsigned short      wCommand;          
	char      host_name[NLENTH];//平台名称
	char      weapon_name[NLENTH];//武器名称（点防御时为防御系统名称）
	char      arrive_name[NLENTH];//目标名称（点防御时为来袭武器名）
	int         weapon_num;			//发射数量
};

/******************/
/*1.19 武器发射，平台主动攻击--2020.12.04--平行仿真添加
/******************/
struct WEAPON_LAUNCH_Auto
{
	unsigned short      wCommand;                  //命令号
	short    PlatID;                                               //兵力实体代号
	short    TgtID;                                               //目标实体代号     
	char      host_name[NLENTH];                        //平台名称
	char      arrive_name[NLENTH];                        //目标名称
	int    fireType;						//发射类型  0-发射一次  1-击毁为止或无弹药为止
};


/******************/
/*1.18 武器按航路发射
/******************/

struct WEAPON_LAUNCH_TRACK
{
	unsigned short      wCommand;                  //命令号
	short    PlatID;                                               //兵力实体代号
	short    TgtID;                                               //目标实体代号     
	char      host_name[NLENTH];                        //平台名称
	char      weapon_name[NLENTH];                 //武器名称
	char      arrive_name[NLENTH];                     //目标名称
	int       weapon_num;			                         //发射数量
	int       Tpoint_count;							         //航线点个数
	TrackPoint   Track_point[GY16B];				//航线数组
	short      GuiMode;                       //制导方式：0为无效 1雷达 2红外 3光学 4主动声定位 5电子情报 6通信情报 7磁 8其他
	short    WeaponID;                             //武器ID 实兵交战系统用
	int    SearchType;						//1-最大目标、2-RCS最大目标、3-发现目标中最近目标、4-最先发现目标、5-任意目标
};

/******************/
/*1.18 导弹截获状态字
/******************/
struct MISSILE_TARGET_SECMED
{
	unsigned short      wCommand;             //命令号      0导弹截获字 1制导方式
	char      weapon_name[NLENTH];           //武器名称
	int         iTarSeleM;                   //截获状态方式：0选航控电压最小；1选中间；2选左边；3选右边；4选最近；5选最远；6选最大；
	                                         //7选最小；8选随机；9选最大
};
/******************/
/*1.18 诱饵干预信息
/******************/
struct OPER_DECOY_DATA
{
	unsigned short      wCommand;    //命令号      
	char      cname[NLENTH];              //诱饵名称
	char      cPlatName[NLENTH];      //母舰名称
	int         iNum;                                //发射的诱饵数量
};
/*****************/
/*1.8 人工裁决干预命令信息
/*****************/
struct  EXT_SUmpire_Ctrl
{
	int          iNo;   //流水号
	int          Ctrl;	//	1-接受裁决命令0-拒绝裁决命令
	int		  Resault;		//裁决结果
	double   fvalue;			//值
};
/*****************/
/*1.9 任务干预信息
/*****************/
//1.91 飞机巡逻任务
struct  AIR_LAUNCHMission
{
	unsigned short      wCommand;                  //命令号   
	char      dwBaseName[NLENTH];					//起飞基地**必填项
	unsigned short	  wPlaneNum;					//起飞架数**必填项
	char      cAirCraftName[NLENTH];					//飞机名称**必填项
	int       pointNum;								//区域点数**必填项
	Lng_Lat pointarray[GY16B];					//飞机巡逻空域**必填项
	double  dfWorkTime;							//巡逻时间  单位：时
	double  dfSpeed;									//巡逻速度  单位：米/秒
	double  dfAlt;										//巡逻高度      单位：米
};
//1.9.2 飞机打击任务
struct  AIR_AttackMission
{
	unsigned short      wCommand;                  //命令号   
	char      dwBaseName[NLENTH];					//起飞基地**必填项
	unsigned short	  wPlaneNum;					//起飞架数**必填项
	char      cAirCraftName[NLENTH];					//飞机名称**必填项
	int       pointNum;								//航线点数**必填项
	TrackPoint pointarray[GY16B];					//飞机航线**必填项
	int       FirePoint;										//发射点，第i个点，i从0开始
	char      weapon_name[NLENTH];                 //武器名称
	char      arrive_name[NLENTH];                     //目标名称
	int       weapon_num;			                         //发射数量
};
//1.9.3 舰艇打击任务
struct  Ship_AttackMission
{
	unsigned short      wCommand;                  //命令号   
	char      dwShipName[NLENTH];					//起飞基地**必填项
	int       pointNum;								//航线点数**必填项
	TrackPoint pointarray[GY16B];					//舰艇航线**必填项
	int       FirePoint;										//发射点，第i个点，i从0开始
	char      weapon_name[NLENTH];                 //武器名称
	char      arrive_name[NLENTH];                     //目标名称
	int       weapon_num;			                         //发射数量
};
/******************/
/*信息变更
	更改平台属性信息
/******************/
struct  AssetAttributeMotify
{
	unsigned short      wCommand;                  //命令号   
	char      PlatName[NLENTH];					//平台名称
	short     Type;											//平台信息类型   0-敌我属性 1-军标类型 2-运动介质 3-生命值
	char       PlatValue[GY16B];			             //信息值 
};
/*****************/
/* 融合清空干预信息
/*****************/	
struct FUSION_ClearMessage						//信息标识：GY_FUSIONCLEAR
{
	double time;							     // 时间
	int        PlatID;							     // 融合中心平台ID
	char     plat_name[NLENTH];    // 平台名称
	int        iStBy;                              // 备用
};

/*****************/
/* 战术变更干预信息
/*****************/	
struct TACTICS_ChangeSet						    //信息标识：GY_TACTICSSET
{
	unsigned short	  wCommand ;				//命令标识
	char     cName[NLENTH];					       //平台名称
	char     cTacticname[NLENTH];			       //战术名称
	char     cOther[NLENTH];						   //备用
};

/*****************/
/* 越线越区裁决信息*/
/*****************/	
struct SResultMonitor
{
    MARS_Uint32	dwAssetID;              // 平台编号 无
	unsigned char	ucDisobeyType;	        // 裁决结果违反类型 1-兵力越线 2-兵力越区 3-速度越界 4-速度未越界
	char	        chAssetName[NLENTH];	// 平台名称	无
	char            regionname[NLENTH];    // 区域/航线名称
	unsigned char	ucDisobeyReason;        // 违反原因	1-兵力越线 2-兵力越区 3-速度越界 4-速度未越界
};

/******************/
/*信息变更
	更改平台武器数量信息
/******************/
struct  MARS_WeaponNumMotify
{
	unsigned short      wCommand;           //命令号   
	char     PlatName[NLENTH];					//平台名称
	char     WeaponSystem[NLENTH];			    //武器系统名称
	char      WeaponName[NLENTH];			        //武器名称
	int        iNum;						//武器数量
};

/******************/
/*1.19 信息请求
	监控台项DIspose软件请求信息
/******************/

struct DATA_REQUEST
{
	unsigned short      wCommand;				//监控台号 
	char				ip_add[NLENTH];			//请求方的IP地址
	int                 iPort;					//请求方端口号
	int					iTYPE;					//请求的数据类型 ,-1,全部信息；0-	区域航线;1-武器状态;2-传感器状态;3-指挥结构;4-想定基本信息;5-飞机机场信息;6-通信组网关系
};

///////////////////////////////////////////////////////////////////
/************************************************************************/
/* 状态请求信息                                                           */
/************************************************************************/
struct MARS_StateRequest
{
	unsigned short      PlatID;				//台号 
	unsigned short      wCommand;			//命令号  交战裁决干预命令时 0为自动裁决 1为人工裁决
};

/************************************************************************/
/* 状态回复信息                                                           */
/************************************************************************/
struct MARS_StateRevert
{
	unsigned short      PlatID;				//台号 
	unsigned short      wCommand;			//命令号 
	char      cScinarioName[GY128B];		//想定名称
};

struct WEAPON_LAUNCH_TRACK_QT				//鱼雷武器发控信息  与后台对接
{
    MARS_Uint32		ulTTime;				//相对于当前演习时间的相对事件	单位 ms
	unsigned short      wCommand;                  //命令号
	short    PlatID;                                               //兵力实体代号
	short    TgtID;                                               //目标实体代号     
	char      host_name[NLENTH];                        //平台名称
	char      weapon_name[NLENTH];                 //武器名称
	char      arrive_name[NLENTH];                     //目标名称
	double	dwgtDistance;  //目标距离 
	float		wTgtBear;  //目标方位 
	int       weapon_num;			                         //发射数量
	float  fCorner1;                       //一次转角     单位：度
	float  fCorner2;                       //二次转角     单位：度
	float  fDepthLimitMax;             //上限深度   单位：米
	float  fDepthLimitMin;              //下限深度   单位：米
	float  fSearchDepth;                //搜索深度   单位：米
	float  fBootDistance;               //自导开机距离  单位：米
	float  fBoatDistance;        //二次转角前直航距    单位：米
	float  fActiveBootDis;		//	主动自导开机距离	最大值为13000 最小值为600    单位：米  注：仅鱼3有效，其他填无效值0	
	unsigned char	ucUserType;  //	使用方式	1：线导，2：自导，3：直航，4：齐射
	unsigned char	ucGuideType;  //	自导方式	1：被动声自导2：主动声自导   3：主被动联合声自导4：尾流自导（水面尾流自导）5：水下声自导6：高速水面声自导7：低速水面声自导注：鱼6填无效值0
	unsigned char	ucGuideSpeedMod;  //	反潜自导鱼雷速制	反潜自导鱼雷速制1：高速   2：低速注：仅鱼6、鱼9有效，其他填无效值0
};

struct DECOY_LAUNCH_INFO				//声抗器材发控信息
{
    MARS_Uint32		ulTTime;				//相对于当前演习时间的相对事件	单位 ms
	unsigned short      wCommand;                  //命令号
	short    PlatID;                               //兵力实体代号
	char      host_name[NLENTH];                   //平台名称
	char      Decoy_name[NLENTH];                  //声抗器材名称

	float	nTgtCw	;    //	目标舷角	； 	参见HXB 03.2 - 2015 4.68
	unsigned char	ucDecoyType;    //	对抗器材类型	； 	1-声诱饵，2-噪声干扰器，3-气幕弹
	float	fShootBm;    //	发射方位	； 	参见HXB 03.2 - 2015 4.10
	float	fShootDis;    //	发射距离	； 	参见HXB 03.2 - 2015 4.70
	unsigned short	dShootNum;    //	发射数量	；
	//发射声诱饵时有效
	float	fBaitCorner;    //	诱饵初始转角	； 	范围：-100度~100度，单位：0.01度（°）
	float	fBaitDept;    //	诱饵初始航深	； 	参见HXB 03.2 - 2015 4.8
	unsigned char	ucBaitVm;    //	诱饵工作航速	； 	1-低速，2高速
};

/************************************************************************/
/* 多分支推演数据                                                          */
/************************************************************************/
struct MULTI_ENGINE_Ctrl			//多分支引擎控制
{
	int TreeNodeID;				    //分支ID -1,对所有分支控制
	int EngineCtrl;                //引擎控制 1，开始 2，暂停3，结束
};

struct MULTI_ENGINE_Info			//多分支引擎信息
{
	int TreeNodeID;				    //分支ID -1,对所有分支控制
	int FEngineID;					//父节点分支ID
	double  EngineTime;                //分支ID运行时间
};

struct MULTI_DecionPoint_request			//决策点请求
{
	int TreeNodeID;				    //分支ID 
	double  DecionPointTime;         //决策点时间
};

struct MULTI_DecionPoint_rply			//决策点回告
{
	int DecionPointID;				    //决策点ID
	double  DecionPointTime;                //决策点时间
};

struct MULTI_change_curEngineID			//切换当前引擎id
{
	int TreeNodeID;				    //分支ID
	int EngineID;					//引擎id
};

#pragma pack (pop)
