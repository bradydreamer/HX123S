#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */
#ifndef _RECORDLIB_H_
#define _RECORDLIB_H_

/*************************�����ױ�־Card trade flag******************************************/
#define CARD_MAG                                        0x00       //�ſ�����//magcard trade
#define CARD_ICC                                        0x01         //ICC��//ICC card

//�����޸���ʷ���е�����//used for modifying the data in the history library
enum RECORDFLAG {
//��ʷ�ⷢ���������� ��ر�־//the flag of sending area in history library
            RF_SEND_CARDHOLD = 0, //�ֿ���25b  //cardholder 25b
    RF_SEND_AID, //AID���� 16b  //AID data  16b
    RF_SEND_APPLABEL, //Ӧ�ñ�ǩ���� 16b  //application label data  16b
    RF_SEND_APPNAME, //Ӧ���� 16b  //application name  16b
    RF_SEND_TVR, //TVR 5bytes
    RF_SEND_TSI, //TSI 2bytes
    RF_SEND_ATC, //ATC2b
    RF_SEND_TC, //TC8b
    RF_SEND_IOCFOROFFSALE, //IC���ѻ����ѳ���������֯����//International organization code for IC card offline trade offsale
    RF_SEND_TERMCAPAB, //9f33 3�ֽ�//9f33 3 bytes
    RF_SEND_UNPREDICTNUM, //9f37 4�ֽڲ���Ԥ֪��//9f37 4 bytes, unpredictable number
    RF_SEND_CVM, // 3�ֽ�//3 bytes
    RF_SEND_AIP, //82 2�ֽ�//82 2 bytes
    RF_SEND_ARQC, // 8�ֽ�//8 bytes
    RF_SEND_EPTAC, //  4b
    RF_SEND_OFFBALANCE, //6b
    RF_SEND_USER, //�û��Զ��巢��������//user-defined sending data area
//��ʷ������������ ��ر�־//the flag of history library index area data
            RF_INDEX_UNDO, //���׳��� ��־//undo trade flag
    RF_INDEX_ADJUST, //0:δ���� 0x01:�ѵ���,��ͳ��, 0x02 �ѵ���,��Ҫͳ��ͳ��//0:not adjust, 0x01: adjusted, 0x02 adjusted, need do statistic
    RF_INDEX_UPLOAD, //0x00:δ���� 0x01:���ͳɹ� 0xFF:����δ�ɹ�.//0x00: not upload, 0x01: upload success, 0xFF: failed upload
    RF_INDEX_APPCERROR, //�Ƿ���ARPC��Ľ���//is ARPC error trade or not
    RF_INDEX_PRINT, //��ӡ ��־//print flag
    RF_INDEX_ORGTRACENO, //ԭ������ˮ//original trade trace number
    RF_INDEX_USER, //�û��Զ�������������//user-defined index area data
//��ʷ������� ������ݱ�־//the flag of history library receive data area
            RF_RECV_REVREASON, //����ԭ��//reversal reason
    RF_RECV_TVR, //���հ�֮��tvr  //tvr after receiving bag
    RF_RECV_TSI,
    RF_RECV_ATC,
    RF_RECV_TC,
    RF_RECV_ISSUEAPPDATA, //������Ӧ������//issuer application data
    RF_RECV_SCRIPT, //�Ǳ�����//script data
    RF_RECV_UNIONPAYID,
    RF_RECV_USER //�����������û��Զ���//user-defined receive data area
};

/*=======BEGIN: ���������ݽṹIndex area data struct=======================*/
#define MAXRINDEXNUM 2048                                               //����������ױ���//the maximun trade number of index area
#define MAXRINDEXSIZE  (512) //�������������//maximum index area buffer
#define MINRINDEXSIZE  (128) //��С����������//minimum index area buffer
#define MAXRECORDSIZE (4096) //����¼��С//maximun record size
struct _RECORDINDEX   //Ӧ���������ݽṹ//application index data struct
{
    u8 ucTradeId;               //����ID  //trade ID
    u8 ucMainAccLen;      //���˺ų���//main account length
    u8 bcMainAcc[10];           //���׿��� BCD��ʽ//trade card number, BCD format
    u8 bcAmount[6];             //���׽��    BCD��ʽ//trade amount, BCD format
    u8 bcTraceNo[3];            //������ˮ��  BCD��ʽ//trade trace number, BCD format
    bool bIsForeignCard;        //�Ƿ����⿨//whether is foreign card
    bool bIsOfflineTrade;       //�Ƿ������߽���//whether is offline trade
    bool bIsOrgSaleOrComplete;          //ԭʼ����FALSE:���� TRUE:Ԥ��Ȩ���(���С��)  //original trade, FALSE:sale, TRUE: preauthtrade complete(aim at tip)
    u8 ucBankcode[9];           //���п�����//bank card code
    u8 ucCardMode;              //�Ƿ���IC������//whether is IC card trade
    u8 ucIccSN;                 //��Ƭ����//card serial number
//�������ݱ���������޸ķ����ò�������//above data can not be changed after being saved, or will not get index
//�������ݱ���֮������޸�//following data can be change after being saved
    bool bIsUndo;               //�����Ƿ񱻳���//the trade whether undo
    u8 ucAdjust;                //0:δ���� 0x01:�ѵ���,��ͳ��, 0x02 �ѵ���,��Ҫͳ��ͳ��//0:not adjust, 0x01:adjusted, not do statistic, 0x02 adjusted, need do statistic
    u8 ucUpload;                //0x00:δ���� 0x01:���ͳɹ�0x2 ʧ�� 0xFF:���ͱ���//0x00:not upload, 0x01:update success, 0x2 failed, 0xFF:upload is rejected
    bool bIsArpcErr;            //�Ƿ���ARPC��Ľ���//whether is APRC error trade
    bool bIsPrint;              //��ӡ��־ 1 �Ѵ�ӡ  0 δ��ӡ//print flag, 1 printed, 0 not print
    u8 ucOrgTraceNo[3];       //ԭ������ˮ�� BCD��ʽ//original trade trace number, BCD format

    //�û��Զ�����������������sizeof(struct _RECORDINDEX)���ֽڿ�ʼ��//From the sizeof(struct _RECORDINDEX) bytes of index area is user-defined area
    //�û��Զ�����������󳤶�ΪRindexSize-sizeof(struct _RECORDINDEX)   //user-defined area maximum length is RindexSize-sizeof(struct _RECORDINDEX)
    u16 usUserLen;    //�û��������ݳ���//user afferent data length
    u8 *pUserVoid;    //�û��������ݵ�ַ����ʹ�ýṹ��//user afferent data address, suggest to use struct
};
/*====================== END======================== */

/*=======BEGIN: ��ʷ�ⷢ����history library sending area===============================*/

struct _RECORDSENDDATA      //������һЩ������Ҫ���������//some special data need to be saved of the trade
{
/******************ԭ���ͱ�־��Original sending area flag******************************/
    //������һЩ������Ϣ//basic trade message===============
    u16 usMan;          //����Ա��//operator number
    u8 bcTradeDate[4];  //BCD��������  YYYYMMDD  //BCD trade date YYYYMMDD
    u8 bcTradeTime[3];  //BCD����ʱ��  HHMMSS      //BCD trade time HHMMSS
    u8 ucTradeId;       //����ID  //trade ID
    u8 ucIccTrade;  //�Ƿ�ΪIC������//whether is IC card trade
    bool bReversal; //�Ƿ���Ҫ����//whether need reversal
    u8 ucIntalMode;    //���ڸ��ʽ//installment mode

    u8 ucCardHolder[25]; //�ֿ�������(���ÿ�һ�ŵ�����)  //cardholder name(credit card ---magnetic track data )
    //IC����ص�һЩ����//IC card data==============
    u8 ucAIDLen;      //aid���ֽڳ���//the type length of aid
    u8 ucAID[16];     //aid  
    u8 ucAppLabelLen; //Ӧ�ñ�ǩ����//application label length
    u8 ucAppLabel[16]; //Ӧ�ñ�ǩ//application lable
    u8 ucAppNameLen;  //Ӧ����ѡ���Ƴ���//application first choice name length
    u8 ucAppName[16]; //Ӧ����ѡ����//application first choice name
    u8 ucTVR[5];      //TVR
    u8 ucTSI[2];      //TSI
    u8 ucATC[2];      //ATC
    u8 ucTC[8];       //TC
    u8 ucIOCForOffSale[3]; //IC���ѻ����ѳ���������֯����//the International Organization Code for IC card offline undo trade
    u8 ucTermCapab[3];     //�ն�����9F33 (3B)  //terminal capability 9F33(3B)
    u8 ucUnpredictNum[4];  //����Ԥ֪��9f37(4B)  //unpredictable number 9f37(4B)
    u8 ucCVM[3];           //CVM 9F34 (3B)
    u8 ucAIP[2];          //AIP TAG:82  (2B)
    u8 ucARQC[8];          //Ӧ������ //application ciphertext TAG:9F23 (8B)
    u8 ucEPTAC[4];         //����Ǯ����TAC (4B)  //the TAC of electronic wallet (4B)
    u8 ucOfflineBalance[6]; //�ѻ����//offline balance
    //�û��Զ���������StoreBagLen+32+RindexSize+sizeof(struct _RECORDSENDDATA)��ʼ��RecordSize  
    //user-defined area from StoreBagLen+32+RindexSize+sizeof(struct _RECORDSENDDATA) to RecordSize  
    //�û��Զ������������ֵΪRecordSize-(StoreBagLen+32+RindexSize+sizeof(struct _RECORDSENDDATA))
    //user-defined area maximum is RecordSize-(StoreBagLen+32+RindexSize+sizeof(struct _RECORDSENDDATA))
    u16 usUserLen; //�û��������ݳ���//user afferent data length
    void *pUserVoid; //�û��������ݵ�ַ����ʹ�ýṹ��//user afferent data address, suggest use struct
};
/*====================== END======================== */

//��ʷ�������//history library receiving area
struct _RECORDRECVDATA      //������һЩ������Ҫ���������//some special data need to be saved of the trade
{
/******************ԭ���ͱ�־��Original sending area flag******************************/
    //������һЩ������Ϣ//some basic trade message===============
    u8 bcTradeDate[4];  //BCD��������  YYMMDD  //BCD trade date  YYMMDD
    u8 bcTradeTime[3];  //BCD����ʱ��  HHMMSS  //BCD trade time   HHMMSS
    u8 asReversalReason[2]; //����ԭ��//reversal reason
    //IC����ص�һЩ����//IC card data==============
    u8 ucTVR[5];      //TVR
    u8 ucTSI[2];      //TSI
    u8 ucATC[2];      //ATC
    u8 ucTC[8];       //TC
    u8 ucIssueAppDataLen; //����//length
    u8 ucIssueAppData[32]; //��������//data
    u8 ucScriptLen; //�ű�����//script length
    u8 ucScript[21]; //�ű�����//script data
    //ò����2��ûʲô�ô����Դӽ��հ�����ȡ//can get the two following value from receiving bag, seem like no use
    u8 ucUnionPayIdForScriptLen; //����//length
    u8 ucUnionPayIdForScript[6]; //

    //�û��Զ���������StoreBagLen+32+sizeof(struct _RECORDRECVDATA)��ʼ��RecordSize
    //user-defined area from StoreBagLen+32+sizeof(struct _RECORDRECVDATA) to RecordSize
    //�û��Զ������������ֵΪRecordSize-(StoreBagLen+32+sizeof(struct _RECORDRECVDATA))
    //user-defined area maximum is RecordSize-(StoreBagLen+32+sizeof(struct _RECORDRECVDATA))
    u16 usUserLen; //�û��������ݳ���//user afferent data length
    void *pUserVoid; //�û��������ݵ�ַ����ʹ�ýṹ��//user afferent data address, suggest to use struct
};
/*====================== END======================== */

enum _BAGTYPE {
    /*���Ͱ�����*/  /*sending bag type*/
            SENDBAG = 0x55, //���Ͱ�//sending bag

    /*���հ�����*/  /*receiving bag type*/
            OKBAG = 0x66, //��ȷ���հ�//receive data OK
    OFFLINEBAG = 0x77, //���߰�//offline bag
    SPECBAG = 0x88, //�����,ָʾIC���ѻ����ײ��ɹ��ı���,(��Ҫ����������)  //special bag, the bag indicating IC card offline trade error,(need special upload)
    REOKBAG = 0xAA, //��Ҫ�����ı���//the bag need reversal
    ERRORBAG = 0xBB, //����Ľ��հ�//error receiving bag
};

//���ڶ���ʷ��ĺ���//used for multiple history library function
//��һ��ʷ������ʹ��//need not use if single history 
extern s32 StlSetCurRecordLibNo(u8 ucCurRecordLibNo);

extern s32 StlSetRecordLibNum(u8 ucRecordLibNum);

//����//other
extern s32 StlInitRecordLib(u16 usStoreBagLen, u16 usRIndexSize, u16 usRIndexNum, u16 usRecordSize,
                            u8 *pucDir, bool bOnlySaveSendBag);

extern s32 StlFreeRecordLib(void);

extern s32 StlIsRecordFull(u16 usSetMaxTradeNum);

extern s32 StlSaveSendRecord(u8 *pucBagIn, u16 usBagInLen,
                             struct _RECORDSENDDATA *pstRecordSendData);

extern s32 StlSaveRecvRecord(u8 *pucBagIn, u16 usBagInLen, enum _BAGTYPE eBagType,
                             struct _RECORDRECVDATA *pstRecordRecvData);


enum _RECORDINDEXTYPE {
    NORMAL_INDEX = 0, //��������//normal index
    SPEC_INDEX = 1, //��������//special index
    REVERSAL_INDEX = 2, //��������//reversal index
    SCRIPT_INDEX = 3 //�ű�����//script index
};

extern s32 StlSaveRecordIndex(struct _RECORDINDEX *pstRecordIndex,
                              enum _RECORDINDEXTYPE eIndexType);

extern s32 StlReadRecordIndex(struct _RECORDINDEX *pstRecordIndex, u16 usIndex,
                              enum _RECORDINDEXTYPE eIndexType);

extern s32 StlGetRecordIndexNum(enum _RECORDINDEXTYPE eIndexType);

extern s32 StlFindRecordIndex(u16 * pusIndexOut, u8 * pucTraceNo);

extern s32 StlReadSendRecord(u8 *pucBagOut, u16 *pusBagOutLen,
                             struct _RECORDSENDDATA *pstRecordSendData, u16 usIndex,
                             enum _RECORDINDEXTYPE eIndexType);

extern s32 StlReadRecvRecord(u8 *pucBagOut, u16 *pusBagOutLen,
                             struct _RECORDRECVDATA *pstRecordRecvData, u16 usIndex,
                             enum _RECORDINDEXTYPE eIndexType);

extern s32 StlReplaceRecvRecord(u8 *pucBagIn, u16 usBagInLen, enum _BAGTYPE eBagType,
                                struct _RECORDRECVDATA *pstRecordRecvData, u16 usIndex,
                                enum _RECORDINDEXTYPE eIndexType);

extern s32 StlInsertRecordData(u8 *pucDataIn, u16 usDataLen, enum RECORDFLAG eRf, u16 usIndex,
                               enum _RECORDINDEXTYPE eRit);

extern s32 StlSaveUploadFlag(u8 ucUploadFlag, u16 usIndex);

extern s32 StlSavePrintFlag(bool bPrint, u16 usIndex);

extern s32 StlSaveUndoFlag(bool bUndo, u16 usIndex);

extern s32 StlSaveAdjustFlag(u8 ucAdjustFlag, u16 usIndex);

extern s32 StlDelScript(u16 usScriptIndex);

extern s32 StlDelReversalBag(u16 usReIndex);

extern s32 StlDelNormalBag(u16 usNormalIndex);

extern s32 StlDelSpecBag(u16 usSpecIndex);

extern s32 StlDelRecord(
        void); //����ʱʹ��,��ɾ����ʷ��������ʷ�����к�++   //will be used when settle, will delete the histoty library and serial number ++

extern s32 StlGetRecordVersion(u8 * pasVer);


#endif //_RECORDLIB_H_

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

