#ifndef _DLLEMVB_H_
#define _DLLEMVB_H_




//#include <assert.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#pragma pack(push)   //�������״̬
#pragma pack(1)

//#include "../src/avl.h"


#include "../../dllemv/inc/dllemv.h"


#include "../src/dllemvbtagbaselib.h"
#include "../src/dllemvbavloperate.h"


#define PAYPASS_FUNC_SUPPORT
#define PAYWAVE_FUNC_SUPPORT
#define CONTACT_EMV

//#define SUPPORT_DUALINTERFACE
//#define EMVB_DEBUG   
/*
typedef struct _EMVTRADEGLBVALUE
{
	unsigned char POSEntryMode;
	unsigned char TVR[5];
	unsigned char TSI[2];
}EMVTRADEGLBVALUE;
*/



typedef struct _EMVBERRORID {

    unsigned int Step;
    unsigned char Status;
    unsigned char L1;
    unsigned char L2;
    unsigned char L3;
    unsigned char SW12[2];
    unsigned char MsgOnError;


} EMVBERRORID;


#define EMVB_DATAAUTH_ONLINEPIN               1
#define EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN     2
#define EMVB_DATAAUTH_OFFLINEENCPIN           3
#define EMVB_DATAAUTH_INPUTPINSTATUS          4
#define EMVB_DATAAUTH_VERIFYIDCARD            5
#define EMVB_NEEDREFER                        6


#define EMVB_ERRID_L1_OK            0
#define EMVB_ERRID_L1_TIMEOUTERR    1
#define EMVB_ERRID_L1_TRANSERR     2
#define EMVB_ERRID_L1_PROTOERR     3


#define EMVB_ERRID_L2_SUCCESS         0
#define EMVB_ERRID_L2_CARDDATAMISSING 1
#define EMVB_ERRID_L2_CAMFAILED       2
#define EMVB_ERRID_L2_STATUSBYTES     3
#define EMVB_ERRID_L2_PARSINGERROR    4
#define EMVB_ERRID_L2_MAXLIMITEXCEEDED 5
#define EMVB_ERRID_L2_CARDDATAERR     6
#define EMVB_ERRID_L2_MAGNOTSUP       7
#define EMVB_ERRID_L2_NOPPSE          8
#define EMVB_ERRID_L2_PPSEFAULT       9
#define EMVB_ERRID_L2_EMPTYCANLIST    10
#define EMVB_ERRID_L2_IDSREADERR      11
#define EMVB_ERRID_L2_IDSWRITEERR     12
#define EMVB_ERRID_L2_IDSDATAERR      13
#define EMVB_ERRID_L2_IDSNOMATCHAC    14
#define EMVB_ERRID_L2_TERMINALDATAERR 15


#define EMVB_ERRID_L3_OK               0x00
#define EMVB_ERRID_L3_TIMEOUT          0x01
#define EMVB_ERRID_L3_STOP             0x02
#define EMVB_ERRID_L3_AMOUNTNOTPRESENT 0x03
#define EMVB_ERRID_L3_CARDMAGINSERT    0x04



//........Analyse.c
#define TAA_OFFLINE 0x40
#define TAA_ONLINE  0x80
#define TAA_DENIAL  0x00



typedef struct _EMVEntryPoint
{
	LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;
	unsigned char SelectedAppNo;
	
}EMVEntryPoint;



typedef struct _TRADEPARAMETER
{
    CAPK_STRUCT *CAPK;
    LISTAPPDATA *SelectedApp;
	LISTAPPDATA *AppListCandidate;
	unsigned char AppListCandidatenum;
    PAYPASSPREPROCEFLAG *gPaypassProceFlag;
    
	
	unsigned char bPrintReceipt:1;
	unsigned char bErrSDATL:1;
	unsigned char bErrAuthData:1;
	unsigned char bCombineDDA:1;
	unsigned char bCombineDDAFail:1;
	unsigned char bDDOLHasNoUnpredictNum:1;
	unsigned char bCDOL1HasNoUnpredictNum:1;
	unsigned char bCDOL2HasNoUnpredictNum:1;

	unsigned char VLPIndicator:1;
	unsigned char bCardConfirmVLP:1;
    unsigned char bAbleOnline:1;
	unsigned char bReversal:1;
	unsigned char bAdvice:1;
	unsigned char IfNeedRefer:1;
	unsigned char PaywaveCANAI:1;
    unsigned char PaywaveIUP:1;

	
    unsigned char PaywaveIUPperformed:1;
	unsigned char startdealiup:1;
    unsigned char canceldealiup:1;
	unsigned char HaveInputAmount:1;
	unsigned char BlkChkCardInBalckList:1;
	//unsigned char multicarddetected:1;
	
	unsigned short AuthDataLen;
    unsigned char AuthData[2048+300];  //SM 算法的时候这里可能缺资源,使用AuthData
	unsigned char TransResult;
	unsigned char qPBOCOrMSD;
    unsigned char TransType;
	unsigned char SelectedAppNo;
	unsigned short EmvNowTask;
	unsigned char MSDCVN17Support;
    unsigned char paywavetrackdataup;
	unsigned char paywavepdolfddaelement;
	unsigned char paywavecvntype;
	unsigned char PaywaveDRRI;
	unsigned char PINLen;
    unsigned char PIN[12];
	unsigned char onlinetradestatus;
	unsigned char TransResultBak;
	unsigned char PaypassTermEFLEF;
    unsigned char PaypassCVMRLEF;
	unsigned char PaypassOndevCTLEF;
	unsigned char SelectSuccessoutmsg;
	unsigned char PaypassSelectedPOSCIIID;
	unsigned char gSelectedaidindex;
	unsigned char ODAstaus;
	unsigned char ODASMstaus;
	unsigned char PaypassnUN;
	unsigned char paypassonlinepinreq;
	unsigned char GenerateACRetDataLen;
	unsigned char GenerateACRetData[255];
	unsigned char bretforminit;
	DISPSEQLISTDATA DispSeqListData;
	unsigned char appblockflag;
	unsigned char ReadLogFlag;
	unsigned char BlkChkCardNoSend;
	unsigned char AppType;
	unsigned char PaywaveAPLimitIndex;
	unsigned char DDATCForOnline;
	unsigned char VisaAPFlag;
	unsigned char VisaAPrandomdata[8];
	unsigned char VisaAPDDAFail;
	unsigned char VisaAPdoubledipflag;
	
	//unsigned char ;
	
}TRADEPARAMETER;






typedef void (*EMVBCORE_qPBOCIccIsoCommand)(APDU_SEND *ApduSend,APDU_RESP *apdu_r);
typedef unsigned char (*EMVBCORE_InputCreditPwd)(unsigned char pintype,unsigned char *ASCCreditPwd);
typedef void (*EMVBCORE_RandomNum)(unsigned char *RandomNum,unsigned int RandomNumLen);
typedef unsigned char (*core_termipkrevokecheck)(unsigned char *buf);
typedef int (*core_check_MatchTermAID)(unsigned char KernelID,unsigned char* aid,unsigned char aidLen,TERMAPP *Applist,unsigned char apptype);
typedef void (*core_ReadTermAID)(TERMAPP *Applist,unsigned char *TermApplistNum);
typedef void (*core_GetPANfromTrack2Equ)(unsigned char type);
typedef unsigned char (*core_sendverifysmmac)(unsigned char *veifydata,unsigned int veifydatalen);
typedef unsigned char (*core_SM_SM3Encrypt)(unsigned char *pInput,unsigned short usLen,unsigned char *pOutput);
typedef int  (*core_GetSummary)(const char *pPkeyX, const char *pPkeyY,const char *pSrcData, unsigned short pSrcDataLen,const char *pIDA, unsigned char *pSummary);


typedef struct {

    TRADEPARAMETER *EMVTradeParam;
    unsigned char IPKModul[2048];
    unsigned char IPKModulLen;
    unsigned char ICCPKModul[248];
    unsigned char ICCPKModulLen;
	unsigned char *paypassccctimes;
	unsigned int  *paypassccctimer;
	EMVBERRORID *EmvErrID;
	OUTPARAMETERSET *pOutParameterSet;
	USERINTERFACEREQDATA *pUserInterfaceReqData;
	ERRORID              *pErrorID;
	PHONEMESSAGETABLEINFO *pphonemsgtable;
	unsigned char *pasTradeAmount;
	EMVBCORE_qPBOCIccIsoCommand EMVB_qPBOCIccIsoCommand;
	EMVBCORE_InputCreditPwd EMVB_InputCreditPwd;
	EMVBCORE_RandomNum EMVB_RandomNum;
	core_senduserinterfacerequestdata senduserinterfacerequestdata;
	core_sendoutparamset sendoutparamset;
	core_sendDisData sendDisData;
	core_sendendapplicationdatarecord sendendapplicationdatarecord;
	core_promptremovecardsoundled promptremovecardsoundled;
	core_checkifcardintornlog checkifcardintornlog;
	core_RECOVERAC       RECOVERAC;
	core_freetornlogtempdata freetornlogtempdata;
	core_deletetornlog   deletetornlog;
	core_copytornrecorddata copytornrecorddata;
	core_addtornlog      addtornlog;
	core_termipkrevokecheck termipkrevokecheck;
	core_check_MatchTermAID check_MatchTermAID;
	core_ReadTermAID ReadTermAID;
	core_GetPANfromTrack2Equ CheckPANfromTrack2Equ;
	core_sendverifysmmac     sendverifysmmac;
    core_SM_SM3Encrypt       SM_SM3Encrypt;
	core_GetSummary          GetSummary;
	int (*InputPWD)(const unsigned char *pasTradeAmount, unsigned char ucPINTryCount, unsigned char ucIccEncryptWay, unsigned char *pheCreditPwd);
	int (*DispPromptData)(unsigned char ePromptType);
	int (*VerifyCardNo)(const unsigned char *pasPAN);
	int (*getofflineencdata)(unsigned char *encdata,unsigned int encdatalen);
	int (*getmagdata)(unsigned char *magdata,unsigned int magdatalen);
	int (*getmagdatahash)(unsigned char* bb,unsigned int *len);
	unsigned int magauthdatapos;
	unsigned char magauthdatalen;
	unsigned char magdatamaskflag;  //sxl20151113
	unsigned char magdatalen;
	unsigned char magdatapos;
	unsigned char magauthdataflag;
	
	
	
}EMVBAppUnionStruct;


typedef struct {
    unsigned char method;
    unsigned char condition;
}ATTRIBUTE CVR;


typedef struct {
    CVR CVRList[120];
    unsigned char CVRListLen;
    unsigned char CVM_X[6];
    unsigned char CVM_Y[6];
}ATTRIBUTE CVMSTRCUT;


typedef struct {
    unsigned char MobileVerLen;             //�ƶ�֧���汾�ų���
    unsigned char MobileVer;                //�ƶ�֧���汾��
    unsigned char MobileVerbexit;           //�ƶ�֧���汾�Ŵ��ڱ�־
    unsigned char BCInfoFileEF04Len;        //���п���Ϣ�ļ�(EF04)��¼������
    unsigned char BCInfoFileEF04;           //���п���Ϣ�ļ�(EF04)��¼��
    unsigned char BCInfoFileEF04bexit;      //���п���Ϣ�ļ�(EF04)��¼�����ڱ�־
    unsigned char MobileCardInfoRecord[108];
    //����Ϣ��¼����
    unsigned char MobileProcessKey[8];      //�ƶ�֧���Ĺ�����Կ
    unsigned char MobileCardSeq[10];        //�ƶ�֧����Ƭ���к�
    unsigned char MobileCardName[20];       //�ƶ�֧��������
    unsigned char MobileCardNo[10];         //�ƶ�֧������
    unsigned char Mobile2Equ[20];           //�ƶ�֧�����ŵ���Ч����
    unsigned char Mobile3Equ[54];           //�ƶ�֧�����ŵ���Ч����
    unsigned char MobileExpiredData[2];     //�ƶ�֧����Ч��
    unsigned char MobileGetTrackTime[8];    //�ƶ�֧���õ��ŵ����ݵ�ʱ��
    EMVBCORE_qPBOCIccIsoCommand EMVB_qPBOCIccIsoCommand;
}ATTRIBUTE MOBILECARDDATA;


#include "../src/dllemvbcom.h"


extern TRADEPARAMETER *gEMVTradeParam;
extern EMVEntryPoint *gEmvEntryPoint;


extern unsigned char Emvb_ProcessRestrict(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_TermRiskManage(EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char Paywaveb_GetAppCandidate(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char contactless_ChooseApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_FinalSelectedApp(EMVBAppUnionStruct *App_UnionStruct);

extern unsigned char Paywaveb_InitialApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_ReadAppData(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_ProcessRestrict(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_UnableGoOnlineDeal(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paywaveb_OnLineDeal(EMVBAppUnionStruct *tempApp_UnionStruct);


extern void Paypassb_packoutsignal(EMVBAppUnionStruct *tempApp_UnionStruct, unsigned int step);

extern void Paypassb_SetErrPar(EMVBAppUnionStruct *tempApp_UnionStruct, unsigned int step);

extern unsigned char Paypassb_GetAppCandidate(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_ChooseApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_FinalSelectedApp(EMVBAppUnionStruct *App_UnionStruct);
extern void Paypassb_initEMVobjBeforeGPO(EMVBAppUnionStruct *App_UnionStruct,unsigned char aidindex);
extern unsigned char Paypassb_InitialApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_ReadAppData(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_MSCheckCVM(EMVBAppUnionStruct *tempApp_UnionStruct);

extern void Paypassb_checkavn(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_computecryptochecksum(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_preGenACBalanceRead(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_CardHolderVerf(EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Paypassb_TermiAnalys(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char TermAnaResult);
extern unsigned char Paypassb_GenACBalanceRead(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_TransProcess(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Paypassb_OnLineDeal(EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char qPBOCb_GetAppCandidate(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char qPBOCb_FinalSelectedApp(EMVBAppUnionStruct *App_UnionStruct);

extern unsigned char qPBOCb_PreTrans(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char qPBOCb_InitialApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char qPBOCb_ReadAppData(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char qPBOCb_CardHolderVerf(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char qPBOCb_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char MSDb_ConstraintParam(EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char EMVb_AppSelect(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_ChooseApp(EMVBAppUnionStruct *App_UnionStruct);
extern unsigned char Emvb_ChooseContactApp(EMVBAppUnionStruct *App_UnionStruct,unsigned char *tempsmstempaid);
extern unsigned char Emvb_UserSelectResult(EMVBAppUnionStruct *App_UnionStruct);

extern unsigned char Emvb_RebuildApplist(EMVBAppUnionStruct *App_UnionStruct);

extern unsigned char Emvb_FinalSelectedApp(EMVBAppUnionStruct *App_UnionStruct);

extern unsigned char Emvb_InitialApp(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_ReadAppData(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_CardHolderVerf(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_TermRiskManage(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_TermiAnalys(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_TransProcess(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_UnableGoOnlineDeal(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_OnLineDeal(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_PreTransComplete(EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char Emvb_ReadEcData(EMVBAppUnionStruct *tempApp_UnionStruct);

extern unsigned char Emvb_ReadEcResetThreshold(EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char emvb_InputCreditPwd(unsigned char pintype, unsigned char *ASCCreditPwd);

extern void EMVBcdAdd(unsigned char *Dest, unsigned char *Src, unsigned char Len);

extern unsigned char PaywaveAP_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char EMVStrPos(unsigned char *Src,unsigned char chr );
extern unsigned char Paypassb_TermActAnaly(EMVBAppUnionStruct *tempApp_UnionStruct);

//SM2
extern unsigned char SM_DataAuth(EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char PaypassbAnalyCvmlist_cmpamount(unsigned char *CVMamount,unsigned char type);
extern unsigned char Emvb_Paywave_ScriptProcess(unsigned char tag,EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Emvb_DOLProcessFunc(unsigned char type,unsigned char* DOL,unsigned short DOLLen,unsigned char* DOLData,unsigned short* DOLDataLen, EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Emvb_ParseAndStoreCardResponse(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char *DataOut,unsigned short *DataOutIndex,unsigned char *parsebInTable,unsigned char readstep);
extern unsigned char Hash_withmagmask(unsigned char* bb,unsigned int len,unsigned char* Message_Digest,unsigned int pos,unsigned int maglen,EMVBAppUnionStruct *tempApp_UnionStruct);

#pragma pack(pop)


#endif







