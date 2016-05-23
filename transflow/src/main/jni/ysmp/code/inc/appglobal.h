#ifndef AFX_GLOBAL_H
#define AFX_GLOBAL_H


#ifdef AFX_APP_VARIABLES
#define EXTERN
#else
#define EXTERN extern
#endif


// duxd2015 #define DEL_CODE_EXCESS				//删除多余代码: 20150304
#define PRINT_ABLE            0        //打印能力: 用于屏蔽掉打印相关的代码
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#include "sdkGlobal.h"
#include "sdkExGlobal.h"
#include "sdkUI.h"

#include "appconfig.h"
#include "appdef.h"
#include "Transflow.h"
#include "const.h"
#include "Pinpad.h"

#include "misc.h"
#include "appflie.h"
#include "menu.h"
#include "commu.h"
#include "postio.h"
#include "orderdata.h"


#include "MposPhoneModeCmdDefine.h"
#include "displaycmd.h"
#include "iccardcmd.h"
#include "keyboardcmd.h"
#include "magcardcmd.h"
#include "printercmd.h"
#include "pinpadcmd.h"
#include "pboccmd.h"
#include "printercmd.h"
#include "magneticcardcmd.h"
#include "storerecordcmd.h"
#include "datatranscmd.h"


#include "terminalmngcmd.h"
#include "capkaid.h"
#include "EnableIcc.h"
#include "Insneeds.h"
#include "Pinpad.h"
#include "tag.h"

// add by dongCQ
#include "printercmd.h"
#include "securityauthcmd.h"
#include "keyboardcmd.h"
//#include "sdkdevprint.h"
#include "sdkprint.h"

// add by Sun.xz
#include "keycard.h"

#include "syssetting.h"//ljh2015-01-09

#include "mock.h" //duxd2015

//debug 开关
//#define DEBUG_CODE_TEST
//end



EXTERN struct _SYS *pgstSys;           //系统参数

EXTERN struct _AppSetting *pgstAppSetting;   //参数全局指针
EXTERN struct _TradeData *pgstTradeData;          //交易处理数据

EXTERN u8 gReadCardType;//读卡方式
EXTERN STCARDINFO pstCardInfo;

EXTERN u8 gIsICOnLineTrade;                //IC 卡联机交易

EXTERN u8 gucCurCommMode;    //通讯方式
EXTERN u32 giIdleClockTimer;    //低功耗定时
EXTERN u32 giAutoRetTimer;       //自动返回定时

EXTERN u8 gPan[8];                          //卡PAN
EXTERN u8 gucTradeAmount[6];                //交易金额
EXTERN u8 gasTradeAmount[13];                //交易金额
EXTERN u8 AppCreditPwd[17];                    //IC卡交易持卡人验证密码密文

EXTERN bool gbSetCmdResetExe;                //指令执行取消/复位标识


//zcl add 设备信息
EXTERN u8 MposDeviceId[20];
EXTERN u8 MposMainKey[32];//索引为MPOSMAINKEYINDEX
EXTERN u8 MposAuthKey[32];//索引为MPOSAUTHKEYINDEX
EXTERN u8 MposAppKey[32];//索引为MPOSAPPKEYINDEX
EXTERN u8 MposPinKsn[8];
EXTERN u8 MposTrackKsn[8];
EXTERN u8 MposPbocKsn[8];

//所有指令交易完成后，默认只刷新第五行钟
EXTERN bool bisNeedBrushScreenWhenBack; //指令通讯完，是否要重新刷屏，解决闪屏，交易完要马上刷整屏的请置此值为true		
EXTERN bool bisNeedKeepScreenWhenBack;//指令通讯完，第五行时间也不刷新		

EXTERN bool IsQmfMode;          //zcl add 是否是全民付模式，全民付模式1C 05回复的值和行业版不一致，目前判断是不是全民付模式是根据是否有设备认证的过程，每次做完1C 05该标志清零
/*=======BEGIN: sujianzhong 2015.01.14  0:47 modify===========*/
EXTERN bool sBisTradeFlow;      //是否有交易				
/*====================== END======================== */

EXTERN s32 gucPBOCTransFlg;                //20150323 添加全局的PBOC处理标识，主要记录确认卡号，应用选择，输入密码时的超时标志

#define MAINKEYINDEX 5
#define AUTHKEYINDES 6
#define APPKEYINDES 7
//#define IDFILE "mpos/xgddeciveid"
//#define KSNFILE "mpos/xgksn"
#define IDFILE "/mtd0/xgddevid"                //20150320  以前的文件写不成功
#define KSNFILE "/mtd0/xgksn"
#define ADDPINKSN 0x01
#define ADDTRACEKSN 0x02
#define ADDPBOCKSN 0x04

#define OFFLINEFILE "offlineFile"
#endif

