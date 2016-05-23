#ifndef _INSNEEDS_H_
#define _INSNEEDS_H_

#define  ECCASHITEM                    0X01//�ֽ�Ȧ��
#define  ECQCACCITEM                0X02//ָ���˻�Ȧ��
#define  ECQCACCNOITEM          0X03//��ָ���˻�Ȧ��

EXTERN SDK_ICC_TRADE_PARAM
IccEmvParam;
//SDK_ICC_PARAM IccParam;
//SDK_ICC_CARDDATA IccCardData;
EXTERN u8
gReadCardType;
//������ʽ
EXTERN u8
GLoadType;
EXTERN u8
ConfirmINS45;
//��ָ���˻�Ȧ����Ҫ��45Hָ��
EXTERN u8
CardExpDate;//���俨���ǣ��Ƿ���Ҫ������Ч�ڱ�־��1Ϊ��Ҫ

struct _CurrencyName                    //����
{
    u8 CurrencyCode[4];                         //���ִ���
    u8 Name[10];                                //��������
};

extern s32 sdkIccGetPAN(u8 * pasOutPAN);

extern void sdkIccProcessRefer(u8 ucSource);

extern s32 sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);

extern bool bIsUseQreader;

extern s32 sdkIccDealQReaderTrade(const SDK_ICC_TRADE_PARAM *pIccTradeParam);

//extern s32 sdkTimerGetId(void);
//extern bool sdkTimerIsEnd(s32 siId, s32 siMs);
extern s32 DisReaptSel();

extern s32 ConfirmCardNo(const u8 *pasPAN);

extern s32 InputExpired(void);

extern void InitTradeParam(SDK_ICC_TRADE_PARAM * pIccTradeParam, ST_TRANSFLOW * TransFlow);
extern s32 DealBlushCard(ST_TRANSFLOW * TransFlow);

extern u32 DealManualCard(SDK_ICC_CARDDATA *pIccCardData, u8 DateFlag, ST_TRANSFLOW *TransFlow);

extern s32 DealInsertIcc(ST_TRANSFLOW * TransFlow);

extern u8 GetUnsaleAid();

extern void DisIccNeedFallbcak();

extern s32 InputPsw(ST_TRANSFLOW * TransFlow);

extern void DisProcessing(void);

extern s32 DealRepeatSel(SDK_ICC_TRADE_PARAM * pIccTradeParam, SDK_ICC_PARAM * pIccParam);
extern s32 DealJumpToFallback(ST_TRANSFLOW * TransFlow);
extern s32 DealIntegralProcess(ST_TRANSFLOW * TransFlow);

extern s32 DealSimplePboc(ST_TRANSFLOW *TransFlow, u8 IfPutInPin);

//u8 DisResult(u8* DealMode,u8* ResponseCode,u8 *DisData,u16* Dlen,struct ST_TRANSFLOW *TransFlow);
extern s32 SaveTcScript();

#if 0
extern s32 FormTcScriptBag( ST_TCandScripts *TcScriptInfo,u8 *Des,u8 *DesLen );
#endif

extern void ParsePrintTag(u8 MerIndex, u8 *DesStr, u16 *DesLen, u8 *SrcStr, u16 *SrcLen);

extern u8 FindTagLen(u8 MerId, u8 *pTag, u8 *DesStr, u16 *deslen);
//void ParseMerchant(u8 *mdata,u8 *len,ST_MerchantInfo *MInfo);
#if 0
extern void FormOfflineBage(u8 *Sdata,ST_OffLineTrade *OfftradeInfo);
#endif

extern u8 RTlvfrmOffList(u8 Mindex, const u8 *pTag, u8 *pOutData, u32 *pOutLen);

extern void PrintOffTrade(ST_TRANSFLOW * TransFlow);

extern s32 ReadIccTradeLog();

extern s32 DealReadLogRecord(SDK_ICC_APDU_RESP *apdu_r, unsigned char *aucDOL,
                             unsigned char *ucRecordNum, unsigned char partno);

extern s32 DispReadCard(const u16 ucCardModeEn);

extern s32 DispIccErr(s32 ErrType);

extern s32 FormTLV(u8 *DesTag, u32 *DesTagLen, const u8 *pheTag, u8 *pheOutData, u32 *puiOutLen);

extern s32 SaveOfflineBag(u8 *TraCode, u32 OffMode);

extern void QpbocGetOnline(ST_TRANSFLOW * TransFlow);
extern void QpbocOfflineProve(ST_TRANSFLOW * TransFlow);

extern s32 DisMovCardAway();

extern s32 SposIccReadCard(u32 uiOverTime, u8 ucCardModeEn, SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                           SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData,
                           const u32 eDispRow);

extern u8 GetCurrncyName(u8 * lpIn, u8 * lpOut);

extern u8 RegCardNo(char *bcd_cardno, u32 *bcd_len, char *asc_cardno, u16 *asc_len);

extern s32 LoadInputpw(u8 type);

extern s32 SaveOfftradeDetail(u8 *Namechar, u8 Namlen);

extern void InitLoadTrade(void);

extern void SetIsICOnLineTrade(u8 Type);

extern u8 GetIsICOnLineTrade();

extern void TradeParmConfig(ST_TRANSFLOW * pstTransFlow);
extern s32 GetIccMagdata(SDK_ICC_CARDDATA * pstCardData);
extern void DispReadCardRet(s32 const
ucDispType);


#endif



