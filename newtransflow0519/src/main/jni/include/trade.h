#ifndef _TRADE_H_
#define _TRADE_H_

enum EMV_CLFLOW
{
    ICCPBOCNORMAL = 0x00,                   //普通流程
    ICCPBOCFLOW      = 0x01,                    //非接PBOC
    ICCQPBOCFLOW  = 0x02    ,                 //非接QPBOC
    ICCMSDFLOW    = 0x03  ,                     //非接MSD
    ICCFALLBACK  = 0x04,                          //接触式FALLback
    MOBILEMSDFLOW  = 0x05,                          //手机芯片 wuzhonglin_20130106
};

typedef enum
{
    WORK_KEY_TPK,
    WORK_KEY_TAK,
    WORK_KEY_TDK
} WORK_KEY_TYPE;

enum _SysParamError {
    SYS_VOUNO = SYS_ERR_MAX+(-1),
    SYS_FILEd6002Type = SYS_ERR_MAX+(-2),
    SYS_MerchantNo  = SYS_ERR_MAX+(-3),
    SYS_TerminalNo  = SYS_ERR_MAX+(-4),
    SYS_CurrencyCode  = SYS_ERR_MAX+(-5),
    SYS_TMKINDEX  = SYS_ERR_MAX+(-6),
    SYS_KeySystemParam = SYS_ERR_MAX+(-7),
    SYS_SUPPORTTDK  = SYS_ERR_MAX+(-8),
    SYS_KEY_TPK  = SYS_ERR_MAX+(-9),
    SYSKEY_TAK  = SYS_ERR_MAX+(-10),
    SYS_KEY_TDK  = SYS_ERR_MAX+(-11),
    SYS_MacType= SYS_ERR_MAX+(-12),
    SYS_SysRefNo= SYS_ERR_MAX+(-13),
    SYS_TPDU= SYS_ERR_MAX+(-14),
};
//???彻??????????????壬??????????????????
//struct _KeySystemParam
//{
//    BOOL bIsMk3Des;                                          //??des????des
//    BOOL bIsSupportTdk;                                         //??????TDK
//    u32 uiTMkIndex;                                                  //???????????
//    u32 uiTakIndex;
//    u32 uiTpkIndex;
//    u32 uiTdkIndex;
//    u32 uiMacType;                          //mac??????
//};

//struct _TerminalParam
//{
//    u8 asMerchantNo[15 + 1];                                                                                        //????? ans15
//    u8 asTerminalNo[8 + 1];                                                                                         //???? ans8
//    u8 asMerchantName[40 + 1];                                                                                      //??????????? ans40
//    u8 asMerchantEnName[40 + 1];                                                                                      //?????????? ans40
//
//    u8 asCurrencyCode[3 + 1];                                                                                     //???????
//
//};



//struct _SYS                                                                         //?????????????
//{
//    u8 ucYear;                                                                      //??Χ(1949??2050),>=0x50??????19xx,<0x50 ???20xx
//    u8 ucMonth;                                                                     //??Χ(1??12)
//    u8 ucDay;                                                                       //??Χ(1??31)
//    u8 ucHour;                                                                      //??Χ(0??23)
//    u8 ucMinute;                                                                    //??Χ(0??59)
//    u8 ucSecond;                                                                    //??Χ(0??59)
//    u8 asVoucherNo[6 + 1];                                                          //???????
//    u8 asBatchNo[6 + 1];                                                            //?????κ?
//    u16 usLoginMan;                                                                 //?????????
//    bool isPosLogin;                                                                //Pos??????,FALSE:δ??? TRUE:?????//?????????????ò???????
//
//    bool isLogined;                                                               //???????
//};




//typedef struct
//{
//    SDK_ICC_PARAM stCardParam;
//    SDK_ICC_CARDDATA stCardData;       //??????
//} STCARDINFO;
//



extern void GetUserCardMode(u8 *pasCardModeCode, u8 *ucCardMode);


extern s32 InitSysParam();
extern s32 GetMagCardNo(SDK_ICC_CARDDATA const *pstCardData, u8 *pasOut);
extern s32 ReadSendBag(SDK_8583_ST8583 *pstSendBag);
extern s32 ReadRevBag(SDK_8583_ST8583 *pstRevBag);
extern s32 WriteSendBag(char *strTag,SDK_8583_ST8583 *pstBag8583);
extern s32 WriteRevBag(SDK_8583_ST8583 *pstBag8583);

extern s32 EncryptSendData(struct _TradeData *pstTradeData, SDK_8583_ST8583 *pstSendBag);
extern s32 CheckRecBag(SDK_8583_ST8583 *pstSendBag, SDK_8583_ST8583 *pstRevBag);
extern s32 ReadReversalBag(SDK_8583_ST8583 *pstReversalBag);

extern s32 WriteTradeTimes(const u32 uiTradeTimes);
extern bool JudgeIsNeedReversal(s32 CheckBagStatue);
extern s32 GetCurCommuStatue(u8 ucCommuStatue);
extern s32 WriteUndoFlag(bool bisReversal);

extern s32 WriteCardInOrOut(SDK_8583_ST8583 *pstSendBag, SDK_8583_ST8583 *pstRevBag);
extern s32 WriteDebitOrCreditType(u8 ucTradeid);
extern s32 BcdAmount2AscAmount(u8 *heBcdAmount, u8 *asAmount);
extern s32 magdata(u8 *src,u8 *data ,u8 * tag);
#endif

