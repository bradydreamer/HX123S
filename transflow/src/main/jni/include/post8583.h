#include "sdkiso8583.h"
#include "switch.h"

#ifndef AFX_POST8583_H
#define AFX_POST8583_H
#define bool int
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
SDK_8583_ST8583 gstSigSendBag;
SDK_8583_ST8583 gstSigRecvBag;
extern TagList gstEmvTempTagList; //xiaox 用于临时存放emv的一些tag
#define    SDK_ICC_MANUAL       0x01                // 支持手输入//support manual input
#define    SDK_ICC_MAG          0x02                // 支持刷卡//support swiping card
#define    SDK_ICC_ICC          0x04                // 支持接触式//support contact
#define    SDK_ICC_RF           0x08                // 支持非接触//support contactless

enum _TradeId
{
	TRADEID_UNKNOWN = 0x00,
	    TRADEID_WELCOME = 0x01,                     //欢迎界面
	    TRADEID_MAINMENU,                           //主界面
	    TRADEID_MANLOGIN    ,                       //操作员签到
	    TRADEID_POINTLOGIN,                         //积分签到
	    TRADEID_POSPARAMDL,                         //引发参数下载
	    TRADEID_TMSDOWNINFO,                        //TMS参数下载
	    TRADEID_QUERY,                              //查询
	    TRADEID_PREAUTH ,                           //预授权
	    TRADEID_UNPREAUTH ,                         //预授权撤销
	    TRADEID_ADDPREAUTH ,                        //追加预授权(附加TRADEID_PREAUTH)
	    TRADEID_COMPLETE ,                          //预授权完成
	    TRADEID_UNCOMPLETE ,                        //预授权完成撤销
	    TRADEID_SALE ,                              //消费
	    TRADEID_UNSALE ,                            //消费撤销
	    TRADEID_OFFCOMPLETE ,                       //预授权完成通知
	    TRADEID_REFUND ,                            //退货
	    TRADEID_OFFSETTLE ,                         //离线结算
	    TRADEID_ADJUST ,                            //调整
	    TRADEID_TIPS ,                              //小费
	    TRADEID_OFFSALE ,                           //普通脱机消费
	    TRADEID_OFFUNSALE ,
	    TRADEID_ECOFFSALE ,                         //电子现金脱机消费
	    TRADEID_ECCACC ,                            //电子现金指定账户充值
	    TRADEID_ECCASH ,                            //电子现金现金充值
	    TRADEID_ECCACCNO ,                          //电子现金非指定账户充值
	    TRADEID_QPBOC ,                             //qPBOC
	    TRADEID_ECOFFREFUND               ,		       //电子现金退货
	    TRADEID_UNECCASH           ,		              //电子现金现金充值撤销
	    TRADEID_ECTRADELOG,                         //电子现金读交易记录
	    TRADEID_ECBALANCE,                          //电子现金读余额
	    TRADEID_ECLOADLOG,                          //圈存日志
	    //电子钱包
	    TRADEID_EPSALE                          ,		 //电子钱包消费
	    TRADEID_EPQUERY                       ,		   //电子钱包余额查询
	    TRADEID_EPLOAD_CASH               ,		       //电子钱包现金充值
	    TRADEID_EPLOAD_ACC                ,		       //电子钱包指定账户充值
	    TRADEID_EPLOAD_NOACC             ,		        //电子钱包非指定账户充值
	    TRADEID_EPUNLOAD_CASH           ,		         //电子钱包现金充值撤销
	    TRADEID_EPCACC ,
	    TRADEID_EPCASH ,
	    TRADEID_EPCACCNO,
	    TRADEID_EPOFFSALE ,
	    //预约类
	    TRADEID_RESERVATION_SALE ,                  //预约消费
	    TRADEID_RESERVATION_UNSALE ,                //预约消费撤销
	    TRADEID_RESERVATION_REFUND,                 //预约消费脱货
	    //积分类
	    TRADEID_POINTSALE_ISSUER ,                  //发卡行积分消费
	    TRADEID_POINTUNSALE_ISSUER ,                //发卡行积分消费撤销
	    TRADEID_POINTSALE_UNION ,                   //联盟积分消费
	    TRADEID_POINTUNSALE_UNION ,                 //联盟积分消费撤销
	    TRADEID_POINTQUERY_UNION ,                  //联盟积分查询
	    TRADEID_POINTREFUND_UNION ,                 //联盟积分退货
	    //订购类
	    TRADEID_MOTOSALE ,                          //订购消费
	    TRADEID_MOTOUNSALE ,                        //订购消费撤销
	    TRADEID_MOTOREFUND ,                        //订购退货
	    TRADEID_MOTOPREAUTH ,                       //订购预授权
	    TRADEID_MOTOUNPREAUTH ,                     //预授权撤销
	    TRADEID_MOTOCOMPLETE ,                      //预授权完成请求
	    TRADEID_MOTOOFFCOMPLETE ,                   //预授权完成通知
	    TRADEID_MOTOUNCOMPLETE ,                    //预授权完成撤销
	    TRADEID_MOTOVERIFY,                         //持卡人身份验证
	    //手机芯片
	    TRADEID_MOBILEREFUND ,                      //手机芯片退货
	    TRADEID_MOBILEPREAUTH ,                     //手机芯片预授权
	    TRADEID_MOBILEUNPREAUTH ,                   //手机芯片预授权撤销
	    TRADEID_MOBILECOMPLETE ,                    //手机芯片预授权完成请求
	    TRADEID_MOBILEOFFCOMPLETE   ,               //手机芯片预授权完成通知
	    TRADEID_MOBILEUNCOMPLETE ,                  //手机芯片预授权完成撤销
	    TRADEID_MOBILEQUERY      ,                  //手机芯片手机芯片查询
	    TRADEID_MOBILESALE                  ,       //手机芯片消费
	    TRADEID_MOBILEUNSALE             ,          //手机芯片消费撤销
	    TRADEID_MAGLOAD_VERIFY                ,		   //磁卡现金充值账户验证
	    TRADEID_MAGLOAD_CASH               ,		      //磁卡现金充值
	    TRADEID_MAGLOAD_AFFIRM                ,		   //磁卡现金充值确认
	    TRADEID_MAGLOAD_ACC                ,		      //磁卡账户充值
	    TRADEID_INSTALLMENTSALE ,                   //分期消费
	    TRADEID_INSTALLMENTUNSALE ,                 //分期撤销
	    TRADEID_SETTLE ,                            //结算
	    //以下非真正意义上的交易类型
	    TRADEID_ECMIXSALE,                          //可挥卡插卡的电子现金，根据刷卡方式最后转化为TRADEID_QPBOC和TRADEID_ECOFFSALE
	    //huangxin 2015.01.09 16:24
	    TRADEID_VERYFACC      ,                     //账户验证
	    TRADEID_UNIONTRANSFER ,                     //银联转账-卡卡转账
	    TRADEID_UNIONPUBRANSFER,                    //银联转账-对公转账
	    TRADEID_ADDOFTENHOME,                       //添加常汇户
	    TRADEID_QUERYOFTENHOMES,                    //查询常汇户
	    TRADEID_ONLINETRANFER,                      //跨行大小额转账
	    TRADEID_SELFBANKTRANFER,                    //行内大小额转账
	    TRADEID_HNPUBRANSFER,                       //行内大小额-对公转账
	    TRADEID_DOWNKEY,                            //下载主密钥
	    TRADEID_QUERYBANK,                          //开户行查询
	    #ifdef ELECSIGN
		TRADEID_ELECSIGN,						    //电子签名上送交易
	    #endif

		// DW.新增回响测试、签退ID 便于全报文加密交易判断
		TRADEID_DOWNLOADPARAM,						//参数下载
		TRADEID_RINGDETECT,							//回响测试
		TRADEID_MANLOGOUT,							//签退
		TRADEID_REVERSAL,							//冲正
		TRADEID_SENDECCTRADE,						//离线交易上送
		TRADEID_SENDTRADESCRIPT,					//脚本上送
		TRADEID_SENDICCTC,							//TC上送

		// DW.16/03/16 新增微信支付四个交易
		TRADEID_WECHATSALE,							//微信 消费
	    TRADEID_WECHATQUERY,						//微信 订单查询
	    TRADEID_WECHATUNSALE,						//微信 撤销
	    TRADEID_WECHATREFUND,						//微信 退货

	    //TMS
	    TRADEID_QUERYPROG,                          //TMS请求版本 linhz_ccb 120611//huangxin 2015.03.06 15:27
	    TRADEID_TMSDLREQ,                           //TMS下载请求 linhz_ccb 120611//huangxin 2015.03.06 15:27
    
	    // MPOS定位
	    TRADEID_LOCATION,
};
#define SYS_ERR_MAX  -100


#define  SDK_APP_RET_OK_MIN          (10000)    //用户自定义必大于这个//user-defined must be bigger than this
//Icc卡返回结果
enum ICCRETURN
{
    ICCOK              =    (SDK_APP_RET_OK_MIN +0) ,        //返回继续走ic交易
    ICCONWAIT          = (SDK_APP_RET_OK_MIN +1) ,        //返回直接联机
    ICCOFFLINE         =   (SDK_APP_RET_OK_MIN +2) ,        //返回脱机交易成功
    ICCOFFLINEERR      =    (SDK_APP_RET_OK_MIN +3) ,        //脱机拒绝需要保存错误包
    ICCONLINE          =    (SDK_APP_RET_OK_MIN +4) ,        //返回联机交易成功
    ICCONLINEDECLINE       = (SDK_APP_RET_OK_MIN +5) ,        //返回联机交易拒绝
    ICCESC             =    (SDK_APP_RET_OK_MIN +6) ,        //退出
    ICCREPEAT          =    (SDK_APP_RET_OK_MIN +7) ,        //返回fallback
    ICCERROR           =    (SDK_APP_RET_OK_MIN +8) ,        //返回错误
    ICCREVERSAL        =    (SDK_APP_RET_OK_MIN +9) ,        //冲正
    ICCEASYFLOW        =    (SDK_APP_RET_OK_MIN +10) ,       //简化流程
    ICCNOTQPBOC        =    (SDK_APP_RET_OK_MIN +11) ,       //非接返回重复刷卡，继续走手机芯片流程不是QPBOC卡
    //lilin 2015.06.04 9:40
            ICCEXISTMAG        =    (SDK_APP_RET_OK_MIN +12) ,       //非接过程中有刷卡
    ICCEXISTQPROVE        =    (SDK_APP_RET_OK_MIN +13) ,       //非接过程中有闪卡SDK_ICCQProve
//    ICCEQPROVEOK        =    (SDK_APP_RET_OK_MIN +14) ,       //闪卡还原成功
            ICCEQPROVEERROR       =    (SDK_APP_RET_OK_MIN +14) ,       //需要换卡

};

struct _Field60
{
    // u8 heReserved60[2];                                                         //#60 自定义60域长度n...13
    u8 asFunCode[2];                                                       //#60.1 交易功能码 n2
    u8 asBatchNo[6];                                                                    //#60.2 批次号 n6
    u8 asNetCode[3];                                                            //#60.3 网络信息码 n3
    u8 ucPosReadCap;                                                                    //#60.4 终端读取能力 n1
    u8 ucIccCondition;                                                          //#60.5 ICC条件码 n1
    u8 ucPortion;                                                       //#60.6 部分扣款和返回余额标志n1
    u8 asAccType[3];                                                            //#60.7 账户类型标志 n3
};

struct _TmsParam
{
    BOOL bisUpdated;
    BOOL bisTms12000027Param;

    u8 asTpdu[11];                              //TPDU
    u8 Phone[4][27];                            //电话号码最大20个数字，逗号,W n26
    u32 CommMode;								//通信模式
    //GPRS无线参数
    u8 GprsApn1[51];							//Apn
    u8 GprsIp1[16];								//15位IP地址
    u8 GprsPort1[6];							//5位端口号
    //CDMA无线参数
    u8 CdmaApn1[51];							//Apn
    u8 CdmaIp1[16];								//15位IP地址
    u8 CdmaPort1[6];							//5位端口号
    //网络参数
    u8 NetServerIp[16];							//15位IP地址
    u8 NetServerPort[6];						//5位端口号
    u8 NetClientIp[16];							//15位Tcp/Ip
    u8 NetGate[16];								//15位Tcp/Ip
    u8 NetMask[16];								//15位Tcp/Ip
    u8 NetServerIp2[16];						//15位IP地址
    u8 NetServerPort2[6];						//5位端口号
    u8 NetGate2[16];							//15位Tcp/Ip
    u8 NetMask2[16];							//15位Tcp/Ip

    u8 TmsDlLimtDate[9];                        //参数下载限制日期
    u8 TmsDlReDial[5];                          //自动重拨间隔时间

    u8 ProgVer[4 + 1];                          //前2位为大版本，后2位为小版本
    u8 ProgVerBak[4 + 1];                       //前2位为大版本，后2位为小版本(备份版本，只有更新新程序成功后才正式赋值到ProgVer)
    u8 TermParamVer[8 + 1];                     //终端参数版本号    N8
    u8 TermParamVerBak[8 + 1];                  //终端参数版本号    N8(备份版本，只有更新新程序成功后才正式赋值到TermParamVer)
    u8 TMSParamVer[8 + 1];                      //TMS参数版本号     N8
    u8 TMSParamVerBak[8 + 1];                   //TMS参数版本号     N8(备份版本，只有更新新程序成功后才正式赋值到TMSParamVer)
    u8 CapkParamVer[8 + 1];                     //公钥参数版本号    N8
    u8 CapkParamVerBak[8 + 1];                  //公钥参数版本号    N8(备份版本，只有更新新程序成功后才正式赋值到CapkParamVer)
    u8 AidParamVer[8 + 1];                      //AID参数版本号     N8
    u8 AidParamVerBak[8 + 1];                   //AID参数版本号     N8(备份版本，只有更新新程序成功后才正式赋值到AidParamVer)

    u8 Reserve0[100];                           //预留100个字节，用多少，这块减去多少 保证恢复结构体不错位
};


struct _KeySystemParam
{
    BOOL bIsMk3Des;                                          //单des，三des
    BOOL bIsSupportTdk;                                         //是否支持TDK
    u32 uiTMkIndex;                                                  //主密钥索引号
    u32 uiTakIndex;
    u32 uiTpkIndex;
    u32 uiTdkIndex;
    u32 uiMacType;
    /*=======BEGIN: huangxin 2015.01.28  9:48 modify===========*/
    u8 uiTmkID[6 + 1];                             //�������ػ��
    u32 uiKLKIndex;                                //������Կ������
    /*====================== END======================== */
};
struct _TerminalParam
{
    u8 asMerchantNo[15 + 1];                                                                                        //商户号 ans15
    u8 asTerminalNo[8 + 1];                                                                                         //终端号 ans8
    u8 asMerchantName[40 + 1];                                                                                      //商户中文名称 ans40
    u8 asMerchantEnName[40 + 1];                                                                                      //商户英文名称 ans40

    u8 asCurrencyCode[3 + 1];                                                                                    //货币代码

};
//报文类型 组织55域IC卡数据的时候要用
enum FIELD55TYPE
{
    SALE_MSG = 0x01,                //消费报文
    QUERY_MSG,                          //查询报文
    PREAUTH_MSG,                    //预授权报文
    OFFLINE_SALE_MSG,           //脱机消费报文
    SCRIPT_NOTICE_RLT,       //IC卡交易脚本通知处理结果报文
    REVERSAL_MSG,                       //冲正交易报文
    SPECIALTC_MSG ,                       //上送TC
    ECUNLOAD_CASH_MSG ,          //EC现金充值撤销报文
    MOBILE_MSG,             //手机芯片报文
};


struct _SYS                                                                          //系统依赖重要参数
{
    u8 ucYear;                                                                      //范围(1949－2050),>=0x50时，表示19xx,<0x50 表示20xx
    u8 ucMonth;                                                                     //范围(1－12)
    u8 ucDay;                                                                       //范围(1－31)
    u8 ucHour;                                                                      //范围(0－23)
    u8 ucMinute;                                                                    //范围(0－59)
    u8 ucSecond;                                                                    //范围(0－59)
    u8 asVoucherNo[6 + 1];                                                          //系统流水号
    u8 asBatchNo[6 + 1];                                                            //系统批次号
    u16 usLoginMan;                                                                 //签到的操作员
    bool isPosLogin;                                                                //Pos签到标志,FALSE:未签到 TRUE:已签到//主要作用是签到用不用连机

    bool isLogined;                                                                 //发起签到
};


typedef struct
{
    SDK_ICC_PARAM stCardParam;
    SDK_ICC_CARDDATA stCardData;       //卡数据
}STCARDINFO;

struct _IccSystemParam
{
    BOOL bisSupportIcc;             //??????ic??
    BOOL bisDownAidParam;           //???????aid
    BOOL bisDownCapkParam;          //capk

};

struct _TRADETOTAL_FOR_ALL
{
    u16 TotalDebitNum;          //总的借计笔数
    u8 TotalDebitAmount[6];        //总的借计金额
    u16 TotalCreditNum;         //总的贷计笔数
    u8 TotalCreditAmount[6];       //总的贷计金额
};


struct  _TRADETOTAL_ALL
{
    struct _TRADETOTAL_FOR_ALL stTotalIn;
    struct _TRADETOTAL_FOR_ALL stTotalOut;
};

struct _OtherTradeParam                            //部分交易参数
{
    //刷卡控制参数
    BOOL bisUnSaleBrush;                           //消费撤销是否刷卡
    BOOL bisUnCompleteBrush;                       //预授权完成撤销是否刷卡

    //密码控制参数
    BOOL bisUnSaleUndoPwd;                         //消费撤销是否输入密码
    BOOL bisUnPreAuthUndoPwd;                      //预授权撤销是否输入密码
    BOOL bisUnCompleteUndoPwd;                     //预授权完成撤销是否输入密码
    BOOL bisCompleteUndoPwd;                       //预授权完成是否输入密码

    BOOL bisAutoLogout;                            //结算后自动签退
    BOOL bisSupportOffSettleSmallMode;             //是否支持小额代付方式
    BOOL bisCheckAdminPwd;                         //是否输入主管密码
    BOOL bisDefaultTrade;                          //缺省交易选择
    BOOL bisTestWorkMode;                          //工作状态
    BOOL bisShieldPAN;                             //是否屏蔽卡号
    BOOL bisNeedCmpCardBin;                        //是否需要比较卡bin
    BOOL bisSupport12Pin;                          //是否借记卡支持12位密码
    u8 asMaxRefundAmount[12 + 1];                  //最大退货金额
    u8 asMaxSaleAmount[12 + 1];                    //信用卡交易最大金额
    u32 uiOffTradeUpTimes;                         //离线上送次数
    u32 uiOffSaleAutoUpNum;                        //自动上送的累计笔数
    u32 uiReversalTimes;                           //冲正重发次数
    u32 uiMaxTradeNum;                             //最大交易笔数
    u32 uiTipsRate;                                //小费比例
    u32 uiCompleteMode;                            //预授权完成方式

    BOOL bisForceOnline;                           //强制联机
    BOOL bisPromptICC;                             //提示IC卡
    BOOL bisEnableTrack1;
    BOOL bisShieldPreauthPAN;                      //预授权屏蔽卡号
    BOOL bisTradeLightOn;                          //是否交易时背光灯常亮//lilin 20140107

    /*=======BEGIN: huangxin 2015.02.04  18:19 modify===========*/
    u8 asMaxOfflineSaleAmount[12 + 1];             //离线交易单笔最大金额
    u8 asMaxNotUpTradeNum[2 + 1];                  //POS中允许保存的未上送交易笔数
    BOOL uibIsFallback;                              //0：不允许，1：允许，2：只允许普通磁条卡交易
    BOOL uibIsInstall;                               //0：不允许，1：允许 是否支持程序下装
    BOOL IsUpTrade;                                  //是否批上送0:不上送 1:批结不平上送 2:不管批结结果都上送
    u8 asICPerTradeMaxAmount[12 + 1];              //IC卡单笔限额
    u8 asNotPerTradeMaxAmount[12 + 1];             //非IC卡单笔限额
    u8 uiMaxUpTradeNum[2 + 1];                     //自动上送的累计笔数
    /*====================== END======================== */
};

struct _TradeCardType
{
    BOOL bIsSupportMag;                                  //刷卡
    BOOL bIsSupportManual;                               //手输
    BOOL bIsSupportIcc;                                  //ICC卡
    BOOL bIsSupportRf;                                   //rf卡
};

struct _PrintSetParam                              //打印相关参数
{
    BOOL bisPrintEnglish;                          //热敏签购单是否打印英文  0 否 1 是 默认 1
    BOOL bisPrintForm;                             //针打的打印格式 0  新  1空白单
    BOOL bisPrintDetail;                           //结算单打印完后是否打印交易明细单	1 打印 0 不打印
    BOOL bisPrintErrReport;                        //是否打印故障报告单
    BOOL bisIssuerBankChinese;                     //发卡行是否打印中文
    BOOL bisAcquirerBankChinese;                   //收单行是否打印中文
    BOOL bisPrintMinussign;                        //撤消类交易打印负号
    BOOL bisUseDefaultPrintTitle;                  //使用默认打印抬头
    u8 asPrintTitle[21];                           //抬头输入内容
    BOOL bisPrintLogo;                             //是否打印LOGO
    u32 uiPrintTimes;                              //打印张数N1
    u8 asPrintServiceHotline[21];                  //打印热线号码
    u32 uiPrintFontSize;                           //打印签购单字体
    BOOL bisPrintECLoad;                           //是否打印圈存日志
    BOOL bisPrintAllDetail;                        //是否打印所有的明细
    u8 asDefaultIssuerBankName[8 + 1];             //默认未知发卡行
    u8 asUmsAdvertisement[60 + 1];                 //银商广告
};

struct _TradeData
{
    STCARDINFO strCardInfo;        //交易卡片信息息
    STCARDINFO strInCardInfo;       //转入卡片信息
//    struct _SYS stSysParam;         //系统依赖重要参数
//    struct _KeySystemParam stKeySystemParam;        //秘钥参数
//    struct _TerminalParam stTerminalParam;          //商户参数
    struct _IccSystemParam IccSystemParam;
    struct  _TRADETOTAL_ALL TRADETOTAL_ALL;

    u8 asTpdu[11];

    u16 usManNo;                      //操作员代码
    u8 ucTradeId;                    //消息ID
    u8 bcVoucherNo[6+1];              //原流水号
    u8 asAmount[12 + 1];            //金额
    u8 asTipsAmount[12 + 1];           //小费金额
    u8 heCreditPwd[13];              //密码
    u8 asSysRefNo[12 + 1];           //授权号
    u8 asTradeDate[4 + 1];           //原交易日期
    u8 asAuthCode[6 + 1];           //预授权


    BOOL bisHaveReversalBag;        //是否有冲正
    u32 uiReversalTimes ;            //冲正次数
    u32 uiCurrTimes;                //记录当前冲正次数
    BOOL bisHaveReverScriptBag;        //是否有脚本
    u32 uiReverScriptTimes ;            //脚本次数
    u32 uiScriptTimes;                //记录当前的次数
    s32 netstate;                //记录当前的次数
};

struct _CommuParam
{
    u8 asTpdu[11];                                       //TPDU
};

struct _AppSetting                                 //设置
{
	struct _PrintSetParam stPrintSetParam;         //打印相关参数
	struct _OtherTradeParam stOtherTradeParam;     //部分交易参数
    struct _KeySystemParam stKeySystemParam;      //秘钥参数
    struct _TerminalParam stTerminalParam;        //商户参数
    struct _CommuParam stCommuParam;              //POS通讯设置
    struct _TmsParam stTmsParam;                  //huangxin 2015.03.05 14:48 tms通讯参数通过tms参数下载获取
    struct _TradeCardType stTradeCardType;         //刷卡类型
//    struct _OperatorParam stOperatorParam;         //操作员密码
//    struct _TradeCardType stTradeCardType;         //刷卡类型
//    struct _TradeShield stTradeShield;             //交易屏蔽
//    struct _AutoRetTime stAutoRetTime;             //超时时间的设定

//    struct _ModemExCfg stModemExCfg;               //MODEM扩展参数
//    struct _PPPWirelessExCfg stPPPWirelessExCfg;
//    struct _TmsParam stTmsParam;                    //huangxin 2015.03.05 14:48 tms通讯参数通过tms参数下载获取

//    u8 asSSLPemName[30+1];                         //ssl pem 文件名 默认放在res路径下
//    u32 uiSettleNum;
    /*=======BEGIN: huangxin 2015.02.11  11:14 modify===========*/
//    struct _ElecSignParam stElecSignParam;
    /*====================== END======================== */
//    BOOL bisSaleOffline;                 //是否消费优先走脱机
//    BOOL bisSalePreAuth;                 //快捷受理交易开关“1”消费；“2”预授权。
//    BOOL bisSupportGM;                  //是否支持国密
//    struct _QpbocParam stQpbocParam;            //闪卡参数设置
};

/*
extern SDK_8583_ST8583 gstSendBag;
extern SDK_8583_ST8583 gstRecvBag;
extern SDK_8583_ST8583 gstTempBag;
*/
//EXTERN SDK_8583_STFIELDDEF gstFieldTab[SDK_8583_FIELDMAX + 1];

//extern bool ReplaceRecvRecord(u8 *const pucBagIn, const u16 usBagInLen, enum _BAGTYPE eBagType, u16 usIndex, enum _RECORDINDEXTYPE eIndexType);


/*=======BEGIN: huacong 2016.03.03  15:59 modify===========*/
/*
typedef enum
{
    WORK_KEY_TPK,
    WORK_KEY_TAK,
    WORK_KEY_TDK
} WORK_KEY_TYPE;
 */
/*====================== END======================== */


extern bool SaveAdjustFlag(const u8 ucAdjustFlag, u8 *const pucTraceNo);
//extern bool SaveRecordIndex(const u8 ucTradeId, enum _RECORDINDEXTYPE eIndexType);
//extern bool SaveRecvRecord(const u8 ucReReason, const u8 ucTradeId, u8 *const pheBagIn, const u16 usBagInLen, enum _BAGTYPE eBagType);
//extern bool SaveSendRecord(bool bReversal, struct _TradeData *stTradeData, u8 *const pheBagIn, const u16 usBagInLen);
extern bool SaveUndoFlag(const bool bUndo, u8 *const pucTraceNo);
extern bool SavePrintFlag(const bool bPrint, u8 *const pucTraceNo);
extern void DealMagRetRepose(const s32 ucReturn, const u8 ucTradeId, u8 *const heBagIn, const u16 usBagInLen);
//extern bool ParseRecordIndex(struct _RECORDINDEX *pstRecordIndex, u16 usIndex, enum _RECORDINDEXTYPE eIndexType);
//extern s32 GetRecordIndexNum(enum _RECORDINDEXTYPE eIndexType);
//extern bool ParseRecvRecord(struct _RECORDRECVDATA *pstRecordRecvData, u16 usIndex, enum _RECORDINDEXTYPE eIndexType);
//extern bool ParseSendRecord(struct _RECORDSENDDATA *pstRecordSendData, u16 usIndex, enum _RECORDINDEXTYPE eIndexType);
extern void GetField03Data(const u8 ucTradeId);
extern void GetField11Data(SDK_8583_ST8583 *pstBag8583);
extern void GetField22Mode(u8 const ucTradeId, u8 const ucCardMode, u8 const ucCLType, bool const bIsPwd);
extern void GetField25Data(u8 const ucTradeId);
extern void GetField55Data(SDK_8583_ST8583 *pstBag8583, u8 const ucCardMode, u8 const ucField55Type);
extern void GetField60Data(u8 const ucTradeId, u8 const ucCLType);
extern void GetField61Data(u8 *as61Data);
//extern void GetField62Data(SDK_8583_ST8583 *pstBag8583, const u8 ucTradeId, struct _OtherTradeUnionData *pstOtherTradeUnionData);
extern void GetField63Data(void);
extern bool GetField64Data(const u8 *pFormData, const s32 FormDataLen, u8 *pOutMac);
extern void GetFieldTermMer(void);
extern bool SetPublicField002(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField003(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField004(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField011(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField014(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField022(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField023(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField025(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField026(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField035(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField036(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField037(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField038(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField041(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField042(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField048(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField049(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField052(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField053(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField055(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField060(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField061(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField062(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField063(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);
extern bool SetPublicField064(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData);


extern bool CopyRecvField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo);
extern bool CopySendField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo);

extern u8 GetTradeFuncode(const u8 ucTradeId);
extern s32 DealSendBagAhead(struct _TradeData *pstTradeData);
extern s32 SendReversalBag(bool bAfterReversal);
extern s32 SendScriptBag(SDK_8583_ST8583 *pstRevBag8583,struct _TradeData *stTradeData);
extern bool UnZipPag55Data(const u8 ucTradeId, SDK_8583_ST8583 *pstBag8583);
extern bool ZipPag55Data(SDK_8583_ST8583 *pstBag8583,u8 TradeType, u8 *DOLData, u16 *DataLen,struct _TradeData *pstTradeData);

//extern s32 FindRecord(bool bReInput, u8 *const pbcTraceNo, struct _RECORDDATA *pstRecordData, s32(*pFun)(struct _RECORDDATA *pstData, u8 *pasDispInfo));
//extern bool ParseRecord(struct _RECORDSENDDATA *pstRecordSendData, struct _RECORDRECVDATA *pstRecordRecvData, const u16 usIndex, const enum _RECORDINDEXTYPE eIndexType);

extern bool DelNomarlBag(u8 *const pucTraceNo);
extern void ReConfigBag8583(SDK_8583_ST8583 *pstBag8583);


//extern s32 Send8583Bag(bool bSendHead, enum SENDBAG_MODE eSendMode, SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam, s32(*pFun)(struct _TradeData *pstTradeData, SDK_8583_ST8583 *pstBag8583), u8 const ucRevDisp);
//extern enum ICCRETURN EmvProcessOnlineData(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam);
//xtern s32 FormPublicSend(SDK_8583_ST8583 *pstBag8583, u8 *pasBitMap);
extern s32 FormPublicSend(SDK_8583_ST8583 *pstBag8583, struct _TradeData *pstTradeData, u8 *pasBitMap);
extern s32 DealPortionTrade(void);
extern s32 DealTradeReponse(const s32 siRet);


extern s32 InitFormSt8583(SDK_8583_ST8583 *pstBag8583, struct _TradeData *pstTradeData);
extern s32 SetFieldData(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData, u16 nFieldDataLen);
extern s32 GetFieldData(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData, u16 nMaxSize);
extern s32 ParseField8583(SDK_8583_ST8583 *pstBag8583);
extern s32 SetFieldDataAppend(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData, u16 nFieldDataLen);
//extern s32 SetFieldDataGet(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData, u16 nFieldDataLen,TagList *pTagList,u32 tag);
extern s32 DealTradeErrReponse(const s32 siRet);
extern bool ParseField60(struct _Field60 *pstField60);
extern u8 ChangeLastTradePrintFlag(void);
extern void CheckQpProveTrade(void);

extern bool CheckResponseCode(u8 const *resp);
extern bool GetInterCode(u8 *pasInterCode, u8 *pherid);
extern  bool JudgeIssuerAuteErr(void);
extern  bool JudgeScriptResultOK(void);
//extern s32 EncryptSendData(SDK_8583_ST8583 *pstSendBag);


extern bool CopyBag8583Field(SDK_8583_ST8583 *pstDestBag8583, u8 ucFieldNo, SDK_8583_ST8583 *pstResBag8583);

extern void SetResversal39Field(s32 CheckBagStatue, u8 *pasResCode);
extern s32 SaveReversalBag(bool bisSendBag, SDK_8583_ST8583 *pst8583Bag, struct _TradeData *pstTradeData);
extern s32 PostPag55Data(SDK_8583_ST8583 * pstRevBag8583, struct _TradeData *stTradeData);
extern s32 EmvProcessOnlineData(SDK_8583_ST8583 *pstRevBag8583,struct _TradeData *stTradeData,u8*pOutData);
extern void unsaleField61Origdata(u8 ucTradeId,u8 *pasOrigdata, SDK_8583_ST8583 *pstSendBag,SDK_8583_ST8583 *pstRevBag);
extern void GetTimeResponseError(u8 *pasOut,s32 res);

extern int GetSysTime(void);
#endif

