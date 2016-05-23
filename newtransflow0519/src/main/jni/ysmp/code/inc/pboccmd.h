#ifndef MPOS_PBOC_CMD_H
#define MPOS_PBOC_CMD_H

EXTERN u8
gstPbocKey[16];
EXTERN u8
gstEcBalance;
//PBOC

typedef enum {
    PBOC_SET_PKEY = 0x01,        //PBOC ���ù�Կ
    PBOC_SET_AID,            //PBOC ���� AID
    PBOC_SET_TERATTR,        //PBOC �����ն�����
    PBOC_GET_DEALDATA,        //PBOC ȡ��������
    PBOC_EXE_FLOW,            //PBOC ִ�б�׼����
    PBOC_SEC_AUTH,            //PBOC ������Ȩ
    PBOC_END_FLOW,            //PBOC ���̽���
    PBOC_EXE_QPBOC,        //qPBOC ����
    PBOC_GET_LASTDEAL,        //PBOC ȡĩ����Ϣ
    PBOC_CMD_END = 0xFF
} PBOC_MPOS_CMD;

typedef struct {
    u8 ICS[7];
    u8 termType;
    u8 termCap[3];
    u8 termAddCap[5];
    u8 posEntry;
    u8 acquirerId[6];
    u8 merchantTypeCode[2];
    u8 merchantNo[15];
    u8 currencyCode[2];
    u8 currencyExp;
    u8 refCurrencyCode[2];
    u8 refCurrencyExp;
    u8 termCountryCode[2];
    u8 ifdSN[8];
    u8 termID[8];
    u8 ddolLen;
    u8 ddol[64];
    u8 tdolLen;
    u8 tdol[64];
    u8 isSuppCompareAid;
    u8 isFallback;
    u8 termTradeAttri[4];

} stMposTradeParam;



//CAPK
#define CLEAN_ALL_PUBKEY        0x01
#define ADD_ONE_PUBKEY            0x02
#define DEL_ONE_PUBKEY            0x03
#define READ_PUBKEY_LIST        0x04
#define READ_SPECIFIC_PUBKEY    0x05

//AID
#define CLEAN_ALL_AID        0x01
#define ADD_ONE_AID            0x02
#define DEL_ONE_AID            0x03
#define READ_AID_LIST        0x04
#define READ_SPECIFIC_AID    0x05

#define LL_LENGTH            2    //LL...1024��LL�������ֽڱ�ʾ


extern s32 OnSetPubKey(ST_TRANSFLOW * pstTransFlow);
extern s32 OnSetMposAid(ST_TRANSFLOW * pstTransFlow);
extern s32 OnSetTerAttr(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetTranData(ST_TRANSFLOW * pstTransFlow);
extern s32 OnExecStdFlow(ST_TRANSFLOW * pstTransFlow);
extern s32 OnPbocSecAuth(ST_TRANSFLOW * pstTransFlow);
extern s32 OnPbocFlowEnd(ST_TRANSFLOW * pstTransFlow);
extern s32 OnGetLastTran(ST_TRANSFLOW * pstTransFlow);
extern s32 OnQpbocFlow(ST_TRANSFLOW * pstTransFlow);

extern s32 sdkIccSendIccCardData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheInBuf, s32 siInLen,
                                 u8 *pheOutBuf, s32 *psiOutLen);

extern bool IsPureCard();

/*************************������ʽ*********************/
#define HANDONLY            SDK_ICC_MANUAL                        //ֻ�������俨
#define MAGONLY                SDK_ICC_MAG                            //ֻ����ˢ��
#define ICCONLY                SDK_ICC_ICC                            //ֻ����IC������
#define RFONLY              SDK_ICC_RF                          //ֻ����RF
#define MAGHAND                (SDK_ICC_MANUAL|SDK_ICC_MAG)        //�ſ���������
#define MAGICC                (SDK_ICC_ICC|SDK_ICC_MAG)            //�ſ���ICC���
#define MAGICCHAND            (SDK_ICC_ICC|SDK_ICC_MAG|SDK_ICC_MANUAL)    //�ſ���ICC��������
#define ICCRF            (SDK_ICC_ICC|SDK_ICC_RF)                    //ICC��RF���
#define MAGALL        (SDK_ICC_ICC|SDK_ICC_MAG|SDK_ICC_MANUAL|SDK_ICC_RF)    //���ж�֧��
#define MAGICCRF    (SDK_ICC_ICC|SDK_ICC_MAG|SDK_ICC_RF) //�ſ�ICC��RF

enum EMV_CLFLOW {
    ICCPBOCNORMAL = 0x00,                   //��ͨ����
    ICCPBOCFLOW = 0x01,                    //�ǽ�PBOC
    ICCQPBOCFLOW = 0x02,                 //�ǽ�QPBOC
    ICCMSDFLOW = 0x03,                     //�ǽ�MSD
    ICCFALLBACK = 0x04,                          //�Ӵ�ʽFALLback
    MOBILEMSDFLOW = 0x05,                          //�ֻ�оƬ wuzhonglin_20130106
};


extern void DispClearAmount(void);            //20150402 ����ϱʽ��׽��
extern void DispAmt(u8 *pasStr, s32 siRow, u32 ucAtr);

//�ر����еĶ������豸
extern void CloseAllReadCardDev(void);

//��DOLTab��֯TLV����
extern s32 FormTLVFromDOLTab(const u8 (*pDOLTab)[2], u8 usTagNum, u8 *ucpOutBuf, u16 *usipOutLen);

//����ʱ��ص�ȫ�ֱ���
extern s32 gsiDCSecond;                //����ʱ����
extern s32 gsiDCRow;                   //����ʱ��ʾ������
extern s32 gsiDCTimer;                 //����ʱ��ʼʱ��
extern s32 gsiDCKey;                   //����ʱ��ȡ�İ���ֵ

#define PBOCCLEARSCREEN_MODE    0        //20150403 PBOC���Ƿ��������������Ļ�������ʾ�ӵڶ��п�ʼ��ʾ

#endif
