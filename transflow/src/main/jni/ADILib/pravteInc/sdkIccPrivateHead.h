/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : PrivateHead.h
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2012��6��15�� ������
   ����޸�   :
   ��������   :
                             ���ļ��������һЩsdk��˽��ȫ�ֺ���
                             �����ȣ�
                             ������ʱ���ǲ�������ļ���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��6��15�� ������
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/


/******************************************************************************

			 COPYRIGHT (C), 2001-2011, XGD

******************************************************************************
FILE NAME   : PrivateHead.h
VERSION   : First Draft
CREATED BY : shiweisong
CREATION DATE : Friday, June 15, 2012
RECENT MODIFICATION: Create files
FUNCTION   : This file is used for storing some SDK private global functions, variables etc. the release version without this file
FUNCTIONS LIST  :
MODIFICATION LOG   :
******************************************************************************/


#ifndef __ICCPRIVATEHEAD_H__
#define __ICCPRIVATEHEAD_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

/*=======BEGIN: ���ٲ� 2013.07.22  8:45 add===========*/
//��ֹ�ſ���IC���ײ������
#include "dllemvb.h"
/*====================== END======================== */

#define RF_INTFNC_MAG     0 //�ǽ��Ƿ�����ˢ��


/*=======BEGIN: ���ٲ� 2013.03.18  15:45 add===========*/
//��Ҫ����Ϊ���÷ǽӷ����һЩ�ṹ������
#define RECVBUFMAXLEN       1024
union _DataType {
    u8 Uni8[4];
    u16 Uni16[2];
    u32 Uni32;
};
/*****************************************************************************
** Descriptions:	        01Э�鷢�ͽṹ��
** Created By:		���ٲ�  2013.04.02
** Remarks: 		
*****************************************************************************/
typedef struct _SENDSTRUCT {
    u8 ucCmd;                                             // ���͵�����
    u8 pheSendBuf[1024];                         // ���͵�����
    u16 usLen;                                            // �������ݵĳ���
} SENDSTRUCT;

/*****************************************************************************
** Descriptions:	        Э����յĽṹ��
** Created By:		���ٲ�  2013.03.30
** Remarks: Ŀ����Ϊ���ۺ�01Э���02Э��		
*****************************************************************************/
typedef struct _PEDREVDATD {
    u8 ucStart;
    //����ͷ01��02
    //01Э�鲿��
    u8 ucFrameId;
    u16 usDataLen;
    //����������������ݳ���
    u8 ucCmd;               // ���յ��Ķ�������
    u8 ucRecvBuf[RECVBUFMAXLEN];            // ����
    u8 ucBCC;                      // У���
    //02Э�鲿��
    u8 ucSN;
    //���к�
    u8 ucRFUData[4];
    //�����ַ�
    //�����
    //���ȹ���
    //���ݹ���
    u16 usCRC;//���ֽ�CRCУ��
} PEDREVDATD;

/*=======BEGIN: ���ٲ� 2013.03.22  13:45 add===========*/
//G101�������02Э�鲿�ֶ���
#define LV2_STX                          0x02
#define LV2_ETX                          0x03
/*====================== END======================== */



#define PED_DATAOK                                                      0X20            /*��֡��ȷ*/
#define PED_DATAERR                                                     0X2F            /*��֡�����������·���*/

// ����λ��ͨѶ����
#define ICC_SOH                         0x01
#define ICC_ETX                          0x03

#define ICC_TEXT                0X12
#define ICC_ORDER            0X13
#define ICC_TEXTEX           0x15

#define ICC_SIM_TEXT                    0x16                        //Sim������ͨ��Э��
#define ICC_SIM_ORDER                   0x17                            //Sim������ͨ��Э��
#define ICC_SIM_TEXTEX                  0x24                            //Sim������ͨ����չЭ��


#define POS_DD_COMMAND                    0xDD        //����0xDDָ��
#define POS_DB_COMMAND                    0xDB        //����0xDBָ��
#define POS_DC_COMMAND                    0xDC        //����0xDCָ��
#define POS_DB_COMMAND                    0xDB        //����0xDBָ��
/*====================== END======================== */




//==================sdkmag.c===================
typedef struct {
    bool bIsMagData;
    bool bIsIccData;
    bool bIsCLData;
} SDK_ICC_READ_CARD;

/*=======BEGIN: ���ٲ� 2013.02.27  11:41 add===========*/
//����ӵĽṹ��Ķ���
typedef struct _MagCardData {
    u8 HandInput;                                            //�����俨��
    u8 HaveExpired;                                          //����Ч��
    u8 HaveICC;                                                //��ICC�ſ�
    u8 ExpiredDate[2];                                        //����Ч����(YYMM)(1950-2049)
    u8 HandLen;                                              //���俨�ų���
    u8 HandData[20];                                         //���俨������
    u8 Track2Len;                                           // //2�ŵ�����
    u8 Track2Data[19];                                      // //2�ŵ�����
    u8 Track3Len;                                            ////3�ŵ�����
    u8 Track3Data[52];                                    //   //3�ŵ�����
    u8 Track1Len;                                          //  //1�ŵ�����
    u8 Track1Data[76];                                //       //1�ŵ���Ч����76��ASCII(7bits)
} SDK_CARD_DATA;
/*====================== END======================== */
//���Լ���Ӳ��ֵ�����
//#define STLMAXRINDEXNUM 1024	//����������ױ���
extern SDK_CARD_DATA gstSdkMagCardData;
//�ſ���Ϣ
extern u8 gucSdkCurAppDir[65];
//��ǰӦ�ó���Ŀ¼
extern u8 gucSdkICCRandom[8];
//IC�����������
//extern u16 IndexEnd;//��¼����
//extern u8 *RecordIndex[STLMAXRINDEXNUM];//����������
//extern SDK_SYS_INFO gstSysInfo;
//extern u8 gucSdkCurAppId[33];//��ǰӦ�ñ�ʶ//���ٲ� 2013.09.09 14:46
extern u8 gucSdkCardMode;
//����ģʽ
extern bool MagCardEnable;  //FALSE:��ֹˢ��,TRUE:����ˢ��
/*====================== END======================== */

//extern bool sdkIccInitReadCard(void);
//extern s32 sdkIccDealReadCard(FIFO *pstFifo);
extern u8 sdkIccGetMagCardNo(const SDK_CARD_DATA *pstMagCardData, u8 *lpOut);



//==================sdkmag.h end===============


//=================sdkemv.h==================
#define ASCII      0x01
#define CFONT      0x00
#define CAPKDATANUM 8  //lechan 2006-1-26 03:54���� 
#define INIDATANUM 16 //huxiang add 091013 for Ec and qPBOC 
/*=======BEGIN: ���ٲ� 2013.02.27  10:58 ADD===========*/
#define ICCDATANUM   71// huxiang add 2 tag for qPBOC//sujianzhong 2013.10.28 9:38 ����Ӧ����71�Ŷ�
#define TERMDATANUM  69// huxiang modify for qPBOC and EC

#define ICCONLINEPIN        0//IC����������PIN
#define ICCOFFLINEPIN        1//IC����������PIN

#define MAX_LINE_CHAR    20//ÿ�������ʾ�ַ�����

#define CARD_MAG                    0x00                                    //�ſ�����
#define CARD_ICC                    0x01                                    //ICC��

#define MULTITASKINI            "/mtd0/res/multitask.ini"    //�����������ļ�

//һЩȫ�ֱ���������


//�ս�����Ӳ��ֵ�����
#define SDK_ICC_AID_NUM 40//aid����//sujianzhong 20131108
#define SDK_ICC_CAPK_NUM 50//capk���������ų��������󣬶�Ϊ50��//sujianzhong 20131108

//extern u8 gEmvCardNo;//���ٲ� 2013.07.22 15:34����ʹ��
//extern u8 gEmvWithIccSocket;//���ٲ� 2013.07.22 15:28����ʹ��

//extern CARDINFO gstSdkCardInfo;
//extern u8 gucSdkAidParamNum;	
//extern IPK_REVOKE gstSdkTermIPKRevoke[8];
extern unsigned char TermIPKRevokeNum;
extern unsigned int guiSdkTransNum;
extern unsigned long guiSdkTransIndex;
extern unsigned long guiSdkTransLogMaxNum;
extern unsigned char gucSdkHaveInputAmount;
extern TRANS_LOG gstSdkTermTransLog[40];
extern unsigned char gucSdkPosSmsTempAid[1 + 16];


#define MV_CardIdentifyInfo 70          //zcl 20120116 ����ɺ궨���ӦICCDataTable_Stored��CardIdentifyInfo    //defined as macro definition corresponding CardIdentifyInfo of ICCDataTable_Stored

typedef struct {
    //unsigned int index;
    char asStr[21];
    u8 ucCol;        //0-127
    u8 ucRow;   //0-7
    u8 ucAttr;       //bit0: 0-CFONT,1-ASCII; bit4: 0-normal,1-Inverse show
} T_DISPSTR;

//extern s32 sdkIccGetEmvCardNo(void);//���ٲ� 2013.07.22 15:27������
//extern AppUnionStruct GlbApp_UnionStruct;
//extern s32 sdkIccCheckRFModule(s32 siTimeOut , u8 *pheOutBuf );//���ٲ� 2013.07.22 15:46�ս���˵�����������ȥ����
extern s32 sdkIccSendIccCardData(const SDK_ICC_PARAM *pstIccParam, const u8 *pInBuf, s32 siInLen,
                                 u8 *pOutBuf, s32 *psiOutLen);

extern void sdkIccGetMagCardData(void);

extern u8 sdkIccReadEcFstCurrencyCodeDetail(const EMVBAppUnionStruct *tempApp_UnionStruct);

extern u8 sdkIccCmdReadECCurrencyCode(APDU_SEND * apdu_s);

extern u8 sdkIccReadEcSecCurrencyCodeDetail(const EMVBAppUnionStruct *tempApp_UnionStruct);

extern u8 sdkIccCmdReadECSecCurrencyCode(APDU_SEND * apdu_s);

extern u8 sdkIccReadEcSecDataDetail(const EMVBAppUnionStruct *tempApp_UnionStruct);

extern u8 sdkIccCmdReadECSecCashBalance(APDU_SEND * apdu_s);


extern void sdkIccScrDisplay(u8 mode, u8 x, u8 y, const char *Disdata);

extern u8 sdkIccVerifyIdCard(void);

extern s32 sdkIccPostInputPinAgain(u8 LastTry);

extern void sdkIccPostEmvSelectAgain(void);

extern void sdkIccPostDispAppList(void);

extern void sdkIccDispAppSelectPage(u8 totalPage, u8 curPage, u8 type);

extern void sdkIccPostUnselectApp(void);

extern void sdkIccPostOfflinePinErr(void);
/*=======BEGIN: ���ٲ� 2013.07.17  16:19 modify===========*/
//extern u8 sdkIccPostReadLogCommand(SDK_ICC_APDU_SEND *apdu_s);
extern u8 sdkIccPostReadLogCommand(SDK_ICC_APDU_SEND *apdu_s, u8 RecordType);

/*====================== END======================== */
extern void sdkIccPostOfflinePinSuc(void);

extern void sdkIccImportAmount(const u8 *lpAmount);

extern u8 sdkIccGetInterCode(u8 *InterCode, const u8 *rid);

extern void sdkIccClearEmvData(void);

extern void sdkIccDispStr(u8 index);

extern void sdkIccGetIccMainAcc(u8 * pAcc);

extern void sdkIccPostqPBOCDispAppList(void);

/*=======BEGIN: sujianzhong 2013.04.24  15:35 modify===========*/
extern void sdkIccDealqPBOCDispAppList(const SDK_ICC_TRADE_PARAM *pstIccTradeParam);

extern void sdkIccDealDispAppList(const SDK_ICC_TRADE_PARAM *pstIccTradeParam);

/*====================== END======================== */

extern void sdkIccCreateUnpredictNum(void); //huxiang add for mobilephone
extern void sdkIccEmvGetTime(u8 * t); //huxiang add for mobilephone
extern void sdkIccReadSN(u8 * SN); //huxiang add for mobilephone
extern void sdkIccPostCVMNoSupport(void); //huxiang add for mobilephone
extern void PostqPBOCErrBack(u8 nowtask); //huxiang add for mobilephone

/*=======BEGIN: ���ٲ� 2013.07.17  16:17 modify===========*/
//extern u8 sdkIccCountTransRecord(u8 *TransRecordSFI , u8 *TransRecordNum);
extern u8 sdkIccCountTransRecord(u8 *TransRecordSFI, u8 *TransRecordNum, u8 RecordType);
/*====================== END======================== */
/*=======BEGIN: ���ٲ� 2013.07.17  16:20 modify===========*/
//extern u8 sdkIccDealICCParamRecord(const SDK_ICC_APDU_RESP *apdu_r , u8 *aucDOL , s32 SIAucLen);
extern u8 sdkIccDealICCParamRecord(const SDK_ICC_APDU_RESP *apdu_r, u8 *aucDOL, s32 siAucLen,
                                   u8 RecordType);

/*====================== END======================== */
extern void sdkIccAidToAppList(void);

//extern void sdkIccDispNoAmount(void);

//extern s32 Private_sdkIccUnzipPagData(const u8 *DataIn , s32 siDataLen);//sjz modify 2012.08.15 14:55
//extern s32 Private_sdkIccImportRespCode(u8 ucOnlineResult, const u8 *pheRspCode);    //sjz modify 2012.08.15 18:38

extern bool sdkIccCheckFloorLimit(EMVBAppUnionStruct * tempApp_UnionStruct);

extern s32 sdkIccReadEmvTransLog(void);

extern s32 sdkIccSaveEmvTransLog(void);

extern s32 sdkIccCheckCapkExpire(void);

extern s32 Private_sdkIccDataAuth(const SDK_ICC_PARAM *pstIccParam);//sujianzhong 2013.08.02 17:26

//===================sdkemv.h end==============


//===========sdkQreader.h================
//#define QREADER_RESEND_T0    1  //100ms
//#define QREADER_RESEND_T1	 5  //500MS  ��ʱ�ط�//overtime 500ms resend
//#define QREADER_RESEND_T2    10  //1s  ��ʱ�ط�//overtime 1s resend
//#define QREADER_RESEND_T3    20  //2s  ��ʱ�ط�//overtime 2s resend
//#define QREADER_RESEND_T4    50  //5s  ��ʱ�ط�//overtime 5s resend
//xule120111 ��������ʱʱ������һ��//double the reader timeout
#define QREADER_RESEND_T0    200  //200ms
#define QREADER_RESEND_T1   1000  //1000MS  ��ʱ�ط�//overtime 1000ms resend
#define QREADER_RESEND_T2    2000  //2s  ��ʱ�ط�//overtime 2s resend
#define QREADER_RESEND_T3    4000  //4s  ��ʱ�ط�//overtime 4s resend
#define QREADER_RESEND_T4    10000  //10s  ��ʱ�ط�//overtime 10s resend


#define QREADER_DATA_MAX    1024  //Э���е�������󳤶�//the max length of protocol data
//���������ص���Ӧ��//the response code returned from reader
#define RC_SUCCESS                          0x00
#define RC_DATA                             0x01
#define RC_POLL_A                           0x02
#define RC_POLL_P                           0x03
#define RC_SCHEME_SUPPORTED                 0x04
#define RC_SIGNATURE                        0x05
#define RC_ONLINE_PIN                       0x06
#define RC_OFFLINE_PIN                      0x07
#define RC_SECOND_APPLICATION               0x08
#define RC_QPBOC_ERRINFO                    0x0C   //����֧��ʱ����������صĴ�����ʾ��Ϣ zjz20111108  //the error prompt message returned from reader when quick payment
#define RC_PAYPASSDECLINE                   0x50
#define RC_NO_SIG                           0xEB
#define RC_NO_PIN                           0xEC
#define RC_US_CARDS                         0xED
#define RC_Other_AP_CARDS                   0xEE
#define RC_POLL_N                           0xEF
#define RC_NO_PARAMETER                     0xF0
#define RC_NO_PBOC_TAGS                     0xF1
#define RC_NO_CARD                          0xF2
#define RC_MORE_CARDS                       0xF3
#define RC_INVALID_SCHEME                   0xF5
#define RC_INVALID_KEYINDEX                 0xF6
#define RC_INVALID_PARAM                    0xF7
#define RC_INVALID_DATA                     0xF8
#define RC_INVALID_COMMAND                  0xF9
#define RC_DDA_AUTH_FAILURE                 0xFA
#define RC_AUTH_NOT_PERFORMED               0xFB
#define RC_AUTH_FAILURE                     0xFC
#define RC_ACCESS_FAILURE                   0xFD
#define RC_ACCESS_NOT_PERFORMED             0xFE
#define RC_FAILURE                          0xFF

#define RC_CARDNO 0x0B //hhl 2012-08-07 ���ؿ��ţ�����POS���к������ȶ�//sjz modify 2012.08.16 10:39
#if 0
/*=======BEGIN: sujianzhong 2013.04.10  14:25 modify Ѱupcash��upcard===========*/
#define RC_CARDTYPE 0x10    //���ؿ�����01:QREADER_UPCASH;02:QREADER_UPCARD
#endif
#define RC_NO_PSE 0x09      //zcl 20130702

#define QREADER_UPCASH 0x01   //�����ֽ�
#define QREADER_UPCARD 0x02   //�ֻ�оƬ��
#define QREADER_STEP_COMPLETE 1//�������Ѿ����qpboc����
#define QREADER_STEP_SEL_PPSE 2//���������ppse
/*====================== END======================== */

#define FIFO_COMMU_QREADER    0xB000




//==========������Command Word=============================
#define QREADER_CMD_POLL                0x07   //POLL
#define QREADER_CMD_ECHO                0x08   //ECHO
#define QREADER_CMD_SETDEBUG            0x10   //����ΪDEBUGģʽ//set to debug mode
#define QREADER_CMD_PARAM                0x12   //���ö������ڲ�����//set the interior parameters of reader
#define QREADER_CMD_INIT                        0x20   //ͨѶ��ʼ��//initialize communication
#define QREADER_CMD_DOUBLEAUTH            0x21   //˫����֤//double authentication
#define QREADER_CMD_CREATEKEY            0x22   //������Կ//create key
#define QREADER_CMD_DISABLEREADER        0x23   //���ζ�����//disable reader
#define QREADER_CMD_SETPOLLP                       0x24   //�ı�˫����֤��״̬��ʹPOLLʱ���Ƿ���û��˫����֤  //zjz20111108  //change the double authentication state, make it return no double authentication when POLL
#define QREADER_CMD_QPBOC                0x30   //�ǽӽ���//contactless trade
#define QREADER_CMD_RESET                0x31   //��λ//reset
#define QREADER_CMD_DISPSTATUS            0x32   //��ʾ״̬//display state
#define QREADER_CMD_QUERY                0x3F   //����ѯby skz 2011-05-25  //query balance

#define QREADER_CMD_MANAGE                0x40   //�������ģʽ//enter manage mode
#define QREADER_CMD_GETCAPABILITY        0x41   //��ȡ����//get capability
#define QREADER_CMD_SETCAPABILITY        0x42   //��������//set capability
#define QREADER_CMD_GETTIME                0x43   //��ȡʱ��//get time
#define QREADER_CMD_SETTIME                0x44   //����ʱ��//set time
#define QREADER_CMD_RESETACQKEY            0x54   //�����յ�����Կ//reset the acquirer key
#define QREADER_CMD_RESUMEREADER        0x55   //�ָ�������//reset reader

#define QREADER_CMD_SETPBOCTAG            0x57   //����PBOC��ǩֵby skz 2011-05-13   //set the PBOC tag value

#define QREADER_CMD_GETCVM                0x5A   //���CVM  //get CVM
#define QREADER_CMD_SETCVM                0x5B   //����CVM	//set CVM
#define QREADER_CMD_SETCAPK                0x61   //���ù�Կ//set public key
#define QREADER_CMD_SETPBOCPARAM        0x67   //����PBOC�̶�����//set PBOC fixed parameters
#define QREADER_CMD_SETAID                0x68   //����AID  //set AID
#define QREADER_CMD_AFTERONLINE            0x6A   //������Ĵ���//the processing after online

/*=======BEGIN: sujianzhong 2013.04.10  14:25 modify Ѱupcash��upcard===========*/
#define QREADER_CMD_MOBILEPAY                   0x6B
/*====================== END======================== */

//===�Զ�������User-defined Command=======
#define QREADER_CMD_GETBAUT                0xf1   //��ȡ������//get baud rate
#define QREADER_CMD_SETBAUT                0xf2   //���ò�����//set baud rate
#define QREADER_CMD_DELALLAID            0xF3   //ɾ������AID  //delete all AID
#define QREADER_CMD_DELALLCAPK            0xF4   //ɾ������AID  //delete all AID
#define QREADER_CMD_INITSYS                0xFB   //������ϵͳ�ļ���ʼ��//initialize system files of reader
#define QREADER_CMD_GETID                0xFC   //��ȡ�����//get machine ID
#define QREADER_CMD_CHECKSTATASET        0x3E   //״̬�������by skz 2011-05-17  //check state setting

//======���ù�Կ/AID��ʽSet Public Key/AID Mode==================
#define QREADER_CAPKAID_ADD                0x11    //���//add
#define QREADER_CAPKAID_DELONE            0x21    //ɾ������//delete one
#define QREADER_CAPKAID_DELALL            0x22    //ɾ��ͬһ����������ȫ������//delete all data of same issuer

//=========֧�ֵķ�����ʶThe Supported Program Identifying================
#define QREADER_ID_PBOC            0x90
#define QREADER_ID_QPBOC        0x91
#define QREADER_ID_MSD            0x92
#define QREADER_ID_UPCARD        0x93

//==========֧�ֵ�CVM��ʽThe Supported CVM Mode=================
#define QREADER_CVM_NO            0x00    //��CVm    //there is no CVm
#define QREADER_CVM_SIGN        0x10    //ǩ��//sign
#define QREADER_CVM_ONLINEPIN    0x11    //ONLINEPIN


#define QREADER_KEY_IMEKmdk        0
#define QREADER_KEY_IMEK        1
#define QREADER_KEY_MEK            2
#define QREADER_KEY_MSESSION    3
#define QREADER_KEY_IAEKmdk        4
#define QREADER_KEY_IAEK        5
#define QREADER_KEY_AEK            6
#define QREADER_KEY_ASESSION    7

#define QREADER_XGD_RFU            "\x0B\x01\x0E\x01"

//=============================================
#define QREDER_RLT_FAIL                0        //����ʧ��//trade failed
#define QREDER_RLT_OFFLINE            1       //�ѻ���׼//offline approve
#define QREDER_RLT_ONLINE            2       //��������//online request
#define QREDER_RLT_REFUSE            3       //���׾ܾ�//trade refuse
#define QREDER_RLT_NOTSUPPORTAPP    4        //Ӧ�÷�����֧��//not supprot the application
#define QREDER_RLT_OVERTIME            5       //���ճ�ʱ//receive overtime
#define QREDER_BLACKLISTCARDNO        6       //��������//black list card
#define RC_DECLINE                  0x0F                   //HHL2011023 add for sending declined transaction information
#define RC_ERR_LASTRECORD           0x0E                            //HHL20110824 add for err in handling last record

//===========����������Reader Type ================================
#define QREADER_TYPE_OUT            0     //���ö����� zjz20111108  //outlay reader
#define QREADER_TYPE_IN                1      //���ö����� zjz20111108  //inlay reader


#define QREADER_PAR_DIR            "/mtd0/appcom/"


//sjz add
#define COM_MISPOS          sdkSysGetComNo(SDK_SYS_COM_PC)
#define COM_PINPAD        sdkSysGetComNo(SDK_SYS_COM_PIN_PAD)

struct _qReaderData  //���������ݽṹ//reader data structure
{
    u8 Sn;            //���к�//serial number
    u8 Rfu[4];    //����ʹ��//reserved for use
    u8 Cmd;         //������//command
    u16 Len;        //ʵ�����ݳ���//actual data length
    u8 Data[QREADER_DATA_MAX];
};//__attribute__((packed)); 

struct _qReaderCapability //�ն����ܽṹ//capability structure
{
    u8 Id;      //Ӧ��ID  //application ID
    u8 Enable;  //֧��//support
};

struct _qReaderCvm        //������CVM  //reader CVM
{
    u8 Id;        //00 ��CVM  10 ǩ��  11 ����PIN  //00 no CVM, 10 sign, 11 online PIN
    u8 Enable;  //֧��			//support
};


struct _qReaderKey //��Կϵͳ//key system
{
    u8 IMEKmdkLen;
    u8 IMEKmdk[16];

    u8 IAEKmdkLen;
    u8 IAEKmdk[16];

    u8 IMEKLen;
    u8 IMEK[16];

    u8 IAEKLen;
    u8 IAEK[16];

    u8 MEKLen;
    u8 MEK[16];

    u8 AEKLen;
    u8 AEK[16];

    u8 MsessionLen;
    u8 Msession[16];

    u8 AsessionLen;
    u8 Asession[16];
};

struct _qReaderCurrentStatus {
    u8 KeyType;        //��Կ����//key type
    u8 Index;        //��Կ����//key index
    u8 KeyLen;        //��Կ����//key length
    u8 Key[16];        //ʹ�õ���Կ//key in using
    u8 RND_B[8];    //�ն������//terminal random number
    u8 RND_R[8];    //�����������//reader random number
    u8 TmpKey[16];    //��֤�Ự��Կ //authenticate session key
    u8 Sn;            //���к�//serial number
    u8 Msission[16];
    u8 Asission[16];
    u8 CurAppId[33];//��ǰӦ�õ�ID zjz20111108  //current application ID
};


struct _qReaderParam {
    u8 TDes;
    u16 MsgTimeOut;        //Ĭ��500ms  //default 500ms
    u16 SaleTimeOut;    //������1000ms  //up to 1000ms
    u16 PollMsg;        //Ĭ��30S  //default 30s
    u16 BufTimeOut;        //Ĭ��5000ms  //default 5000ms
    u8 Encryption;
    u8 Display;
    u16 MaxBufSize;
    u16 DoubleDip;
    u16 ReaderIndex;
    u8 LanguageLen;
    u8 Language[128];
    u16 DisplaySMsg;
    u16 DisplayLMsg;
    u16 DisplaySSMsg;
    u16 DisplaySRMsg;
    u16 DisplayPinMsg;
    u16 DisplayEMsg;
    struct _qReaderCapability ReaderCapability[4];
    struct _qReaderCvm ReaderCvm[3];
    u8 ReaderId[16];
};//__attribute__((packed));

struct _qReaderTermInfo   //G101 zjz20110511
{
    u8 PrintSign;   //�Ƿ���Ҫ��ӡǩ����//whether need to print sign
    u8 DDAFaileNeedOnline; //DDAʧ�� ��Ҫȥ����//DDA failed, need online
};


/*=======BEGIN: ���ٲ� 2013.07.31  14:36 modify===========*/

#if 0 /*Modify by ���ٲ� at 2013.09.03  10:1 */
extern s32 sdkIccForm01FrameData(u8 cmd, const u8 *inbuf, s32 inlen, u8 *outbuf);
extern s32 sdkIccOpenCom(void);
extern s32 sdkIccSendComData(const u8 *sbuf, s32 slen);
#endif /* if 0 */

extern s32 sdkIccQreaderQueryCardSend(u8 cardtype);

extern s32 sdkIccQueryCardRecv(u8 * buf);

extern s32 sdkIccQreaderPowerDown(u8 *buffer, s32 timeout);

extern s32 sdkIccQreaderReset(u8 *rbuf, s32 timeout);

extern s32 sdkIccQreader01Command(const u8 *sbuf, s32 slen, u8 *rbuf, s32 timeout, u8 cardtype,
                                  u8 cardcid);

extern s32 sdkIccQreaderExist(u8 *rbuf, s32 timeout);

#if 0 /*Modify by ���ٲ� at 2013.09.03  9:58 */
extern s32 sdkIccGet02FrameSN(void);
#endif /* if 0 */
#if 0 /*Modify by ���ٲ� at 2013.09.03  9:58 */
extern s32 sdkIccForm02FrameData(u8 cmd, const u8 *inbuf, s32 inlen, u8 *outbuf);
#endif /* if 0 */

extern s32 sdkIccQreader02CommandSend(u8 cmd, const u8 *sbuf, s32 slen);

extern s32 sdkIccQreader02CommandRecv(u8 * rbuf);
#if 0 /*Modify by ���ٲ� at 2013.09.03  9:56 */
extern s32 sdkIccQreader02Command(u8 cmd, const u8 *sbuf, s32 slen, u8 *rbuf, s32 timeout);
#endif /* if 0 */

extern s32 sdkIccQreaderQuery24GModule(s32 timeout);
/*====================== END======================== */


//extern u8 gucSdkIccQreaderStep;  //ʹ��L2������//use L2 reader
extern struct _qReaderData gstSdkIccQreaderDataRec;     //�������������ص�����//get the returned data from reader
extern struct _qReaderData gstSdkIccQreaderDataSend;    //����������͵�����//data sended to reader
extern struct _qReaderKey gstSdkIccQreaderKey[10];      //ʮ����Կ//ten group key
extern struct _qReaderParam gstSdkIccQreaderParam;        //����������//reader parameters
extern struct _qReaderCurrentStatus gstSdkIccQreaderCurrentStatus; //��ǰʹ�õ���Կ//the current key
//EXTERN_QREADER bool TimerReaderPoll;				//��������ʱPOSָ��//reader fixed time POS order
extern u32 guiSdkIccTimerReaderPollID;                            //��������ʱPOSָ��   //reader fixed time POS order
extern u8 gucSdkIccEmvInQreader;                    //EMV L2�ڶ���������� //EMV L2 complete in reader
extern struct _qReaderTermInfo gstSdkIccQreaderTermInfo;    //G101 zjz20110511
extern u8 gucSdkIccQreaderType;                      //����������  zjz20111108   //reader type
extern u8 gucSdkIccUnzipQreaderData;         //G101ADD


extern s32 sdkIccReadPanelVersion(u8 * pVersion);

//extern void sdkIccPostSetReaderMenu(u8 nPage);
//extern u8 sdkIccOnSetReaderMenuCb(SDK_MSG *msg);
//extern void sdkIccPostReaderXgdRfuToPanel(void);
extern s32 sdkIccSendDataToReader(void);

//extern u8 sdkIccOnReaderKeyMenuCb(SDK_MSG *msg);
//extern void sdkIccPostReaderKeyMenu(void);
extern u8 sdkIccSetReaderDesType(void);

extern void sdkIccInitReaderParam(void);

extern void sdkIccSaveQReaderKey(void);

extern void sdkIccReadQReaderKey(void);

extern u8 sdkIccReaderKeyInit(void);

//extern void sdkIccPostSetReaderKeyMenu(void);
extern u8 sdkIccNormalPowerOnReader(void);

//extern u8 sdkIccOnSetReaderKeyMenuCb(SDK_MSG *msg);
extern u8 sdkIccSetReaderDefaultParam(void);

extern u8 sdkIccSetReaderWorkMode(u8 cmd, u8 mode);

extern u8 sdkIccAddOneAidToReader(EmvSysIni_STRUCT * aid);
extern u8 sdkIccDelOneAidToReader(EmvSysIni_STRUCT * aid);

extern u8 sdkIccDelAllAidToReader(void);

extern u8 sdkIccAddOneCapkToReader(const CAPK_STRUCT *capk);

extern u8 sdkIccDelOneCapkToReader(const CAPK_STRUCT *capk);

extern u8 sdkIccDelAllCapkToReader(void);

extern u8 sdkIccGetReaderId(u8 * id);

extern u8 sdkIccAddAllCapkToReader(void);

extern u8 sdkIccAddAllAidToReader(void);

extern void sdkIccInitReaderSys(void);

extern u8 sdkIccPollReader(void);

extern void sdkIccResetQReader(void);

extern u8 sdkIccSyncReaderTime(void);

extern void sdkIccSaveQReaderId(void);

extern void sdkIccReadQReaderId(void);

extern u8 sdkIccIfHaveQReader(void);

extern u8 sdkIccJudgeEnableQReaderTrade(void);

//extern void DealQReaderTrade(void);
//extern void SendQReaderSale(void);
extern void sdkIccIncReaderCurrentSn(void);

extern void sdkIccQReaderDispTradeStatus(u8 success);  //G101 zjz20110511
extern void sdkIccInitqReaderTermInfo(void);             //G101 zjz20110511
extern u8 sdkIccSetReaderPBOCTag(void);                     //by skz 20111-05-13
extern bool sdkIccCheckStateSet(void);

extern void sdkIccSendQReaderQuery(void);

extern void sdkIccDealQReaderQuery(void);

extern u8 sdkIccFirstPowerOnreader(void);

extern u8 sdkIccSyncReaderTermNo(void);

extern void sdkIccDispBlackCardError(void);


extern void sdkIccDealSyncQreaderCapkErr(void); //zjz20111108
extern void sdkIccDealSyncQreaderAidErr(void);  //zjz20111108
extern void sdkIccDealQreaderBackErr(const u8 *info, s32 len); //zjz20111108
extern u8 sdkIccCheckQreaderType(void);          //zjz20111108
//extern void sdkIccSwitchAppResumeQreader(void); //zjz20111108
extern u8 SdkIccReadqReaderCurrentStatusRamdisk(void);       //zjz20111108
//extern void SaveQreaderCurrentStatusRamdisk(void); //zjz20111108
extern u8 sdkIccCheckCurAppNeedStm32(void);     //zjz20111108
extern u8 sdkIccDisableQreader(void);           //zjz20111108
extern u8 sdkIccSetQreaderPollP(void);//zjz20111108

extern u8 sdkSetReaderDefaultParamDetail(void);

extern void sdkIccFillSendReaderData(u8 sn, u8 cmd, const u8 *buf, u16 len);

extern s32 sdkIccDealQReaderTrade(const SDK_ICC_TRADE_PARAM *pIccTradeParam);

extern s32 sdkIccSendQReaderSale(const u8 *pAmount_Bcd);

//extern s32 sdkIccSyncReaderParam(SDK_ICC_SET_READER_PARAM *pstSetReaderParam) ;
extern bool sdkIccCheckReaderParamOK(void);

//extern s32 sdkIccSetReaderParam() ;
extern void sdkIccSendQReaderBlacklist(u8 ucCode);
//==================sdkQreader.h end====================================




extern void sdkIccDealScriptError(void);

extern bool sdkIccQreaderDetectMove(s32 timeout);

extern s32 sdkIccMifareCardRead(u8 *rbuf, s32 timeout);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __PRIVATEHEAD_H__ */

