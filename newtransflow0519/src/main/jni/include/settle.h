
#define CARD_MAG                                        0x02       //磁卡交易//magcard trade
#define CARD_ICC                                        0x04         //ICC卡//ICC card


/********************************批上送模式***********************************/
enum _UPMODETYPE
{
//结算之前
    UPMODE_MAGNOOFFLINE = 0x00,		//未上送磁卡离线类
    UPMODE_ICCNOOFFLINE ,				//未上送ICC(脱机)
    UPMODE_ICCEPNOOFFLINE ,			//未上送电子钱包ICC(脱机)       暂不实现
    UPMODE_ICCSCRIPT,			//未上送脚本的ICC卡交易
//对账之后
    UPMODE_MAGALLOFFLINE ,	    //批上送全部磁卡离线
    UPMODE_ICCALLOFFLINE,			//批上送全部ICC(脱机)
    UPMODE_ICCEPOFFLINE,				//批上送电子钱包ICC(脱机)      暂不实现
    UPMODE_MAGONLINE,			//批上送全部磁卡在线
    //zxx 20130721 22:45一分为二，先磁条再上IC的，不要混在一起
    //UPMODE_MAGICCNOTIFY = 0x08,			//批上送磁卡和ICC通知类(退货/预授权完成(离线))
    UPMODE_MAGNOTIFY,               //批上送磁卡通知类(退货/预授权完成(离线))
    UPMODE_ICCNOTIFY,               //批上送IC卡通知类(退货/预授权完成(离线))
    UPMODE_ICCONLINE,			//批上送ICC(在线)
    UPMODE_ICCSPECIAL,				//批上送ICC(两类特殊)拒绝
    UPMODE_ICCARPC,				//批上送ICC(ARPC错)
    UPMODE_ICCEPLOAD ,			//批上送电子钱包圈存        暂不实现

    //UPMODE_OVER,
};

/************************************脱机交易是否上送状态*******************************************/
//0x00:未上送 0x01:上送成功0x2 失败 0x03:上送被拒
enum _UPRESPONESMODE
{
    TRADE_NOUP = 0x00,         //未上送
    TRADE_UPSUCCESS = 0x01,    //上送成功
    TRADE_UPERROR = 0x02,      // 失败
    TRADE_UPREFUSE = 0x03,  //上送被拒

    // 以下为批上送标志//zxx 20130502 11:48
    TRADE_SETTLE_UPSUCCESS = 0x10,     //已经批上送成功
    TRADE_SETTLE_UPSUCCESS_UPERROR = 0x11,     //已经批上送成功但是之前离线上送失败
    TRADE_SETTLE_UPSUCCESS_UPREFUSE = 0x12,    //已经批上送成功但是之前离线上送被拒
};



enum _SETTLERESTYPE
{
    SETTLE_EQU = 1,			//平锟斤拷
    SETTLE_NOTEQU = 2,  	//锟斤拷平
    SETTLE_ERROR = 3,		//锟斤拷平,锟斤拷锟斤拷
};

//解结算接收包的类型，做不同处理
enum _SETTLE_REVBAG_TYPE
{
    NONE_REV = 0x00,            //错误包
    SETTLE_REV = 0x01,         //结算接收返回包
    UPLOAD_REV = 0x02,         //披上送返回
    UPLOADOVER_REV = 0X03,      //披上送结束
};



struct _UPLOAD
{
    u16 usUpAdd;				  								//批上送断点,0xFFFF不批上送
    u16 usTotalUpNum;                                          //披上送总个数
    u8 ucUpMode;				  								//批上送模式
    u8 ucSettleInState;                                                                     //内卡结算是否平
    u8 ucSettleOutState;                                                                    //外卡结算是否平
//zxx 20130502
    bool bNeedReUp;                                                                     //是否用重发
    u8 ucReUpTimes;                                                                     //重发次数

    u16 usIndexEnd;                 //总的交易笔数
};

struct _UPLOAD gstUpLoad;                //披上送

#define UP_NONE 0xFFFF       //无需批上送

struct _stIndexData
{
    u8 ucTradeId;               //交易ID  //trade ID
    u8 ucMainAccLen;      //主账号长度//main account length
    u8 bcMainAcc[10];           //交易卡号 BCD格式//trade card number, BCD format
    u8 bcAmount[6];             //交易金额    BCD格式//trade amount, BCD format
    u8 bcTraceNo[3];            //交易流水号  BCD格式//trade trace number, BCD format
    bool bIsForeignCard;        //是否是外卡//whether is foreign card
    bool bIsOfflineTrade;       //是否是离线交易//whether is offline trade
    bool bIsOrgSaleOrComplete;          //原始交易FALSE:消费 TRUE:预授权完成(针对小费)  //original trade, FALSE:sale, TRUE: preauthtrade complete(aim at tip)
    u8 ucBankcode[9];           //银行卡代码//bank card code
    u8 ucCardMode;              //是否是IC卡交易//whether is IC card trade
    u8 ucIccSN;                 //卡片序列//card serial number
//以上数据保存后不允许修改否则会得不到索引//above data can not be changed after being saved, or will not get index
//以下数据保存之后可以修改//following data can be change after being saved
    bool bIsUndo;               //交易是否被撤销//the trade whether undo
    u8 ucAdjust;                //0:未调整 0x01:已调整,不统计, 0x02 已调整,需要统计统计//0:not adjust, 0x01:adjusted, not do statistic, 0x02 adjusted, need do statistic
    u8 ucUpload;                //0x00:未上送 0x01:上送成功0x2 失败 0xFF:上送被拒//0x00:not upload, 0x01:update success, 0x2 failed, 0xFF:upload is rejected
    bool bIsArpcErr;            //是否是ARPC错的交易//whether is APRC error trade
    bool bIsPrint;              //打印标志 1 已打印  0 未打印//print flag, 1 printed, 0 not print
    u8 ucOrgTraceNo[3];       //原交易流水号 BCD格式//original trade trace number, BCD format

    u8 asBatchNo[6];            //批次号
};



extern s32 Settlestart(u8 *indata,u32 len,u8 *outdata,u32 *rlen);
extern s32 SettleParse8583(u8 *indata,u32 len,u8 * outdata,u32 rlen);

extern s32 WriteUploadFlag(SDK_8583_ST8583 *pstSendBag8583,bool bisUpload);
extern s32 SaveMagUploadFlag(void);
//extern s32 GetCurCommuStatue(u8 ucCommuStatue);
extern bool ParseField60_new(struct _Field60 *pstField60, SDK_8583_ST8583 *pstBag8583);
extern void FillRight(u8 *pOut, u32 uiOutLen, u8 *pIn, u32 uiInLen, u8 ucChar);
extern void FillLeft(u8 *pOut, u32 uiOutLen, u8 *pIn, u32 uiInlen, u8 ucChar);



