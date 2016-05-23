#ifndef _DLLEMVBCOM_H_
#define _DLLEMVBCOM_H_


//DF8129

#define PAYPASS_OPS_STATUS_APPROVED   1
#define PAYPASS_OPS_STATUS_DECLINED   2
#define PAYPASS_OPS_STATUS_ONLINE     3
#define PAYPASS_OPS_STATUS_ENDAPPLICATION 4
#define PAYPASS_OPS_STATUS_SELECTNEXT 5
#define PAYPASS_OPS_STATUS_TRYANOTHERINTERFACE     6
#define PAYPASS_OPS_STATUS_TRYAGAIN 7
#define PAYPASS_OPS_STATUS_NA 0x0F


#define PAYPASS_OPS_START_A  0x00
#define PAYPASS_OPS_START_B  0x01
#define PAYPASS_OPS_START_C  0x02
#define PAYPASS_OPS_START_D  0x03
#define PAYPASS_OPS_START_NA 0x0F


#define PAYPASS_OPS_CVM_NOCVMREQ        0
#define PAYPASS_OPS_CVM_OBTAINSIGNATURE 1
#define PAYPASS_OPS_CVM_ONLINEPIN       2
#define PAYPASS_OPS_CVM_CONFVERIFIED    3
#define PAYPASS_OPS_CVM_NA              0x0F

//#define PAYPASS_OPS_CVM_CVMREQ        3



#define PAYPASS_ERRID_L1_OK           0
#define PAYPASS_ERRID_L1_TIMEOUTERR   1
#define PAYPASS_ERRID_L1_TRANSERR     2
#define PAYPASS_ERRID_L1_PROTOERR     3


#define PAYPASS_ERRID_L2_SUCCESS         0
#define PAYPASS_ERRID_L2_CARDDATAMISSING 1
#define PAYPASS_ERRID_L2_CAMFAILED       2
#define PAYPASS_ERRID_L2_STATUSBYTES     3
#define PAYPASS_ERRID_L2_PARSINGERROR    4
#define PAYPASS_ERRID_L2_MAXLIMITEXCEEDED 5
#define PAYPASS_ERRID_L2_CARDDATAERR     6
#define PAYPASS_ERRID_L2_MAGNOTSUP       7
#define PAYPASS_ERRID_L2_NOPPSE          8
#define PAYPASS_ERRID_L2_PPSEFAULT       9
#define PAYPASS_ERRID_L2_EMPTYCANLIST    10
#define PAYPASS_ERRID_L2_IDSREADERR      11
#define PAYPASS_ERRID_L2_IDSWRITEERR     12
#define PAYPASS_ERRID_L2_IDSDATAERR      13
#define PAYPASS_ERRID_L2_IDSNOMATCHAC    14
#define PAYPASS_ERRID_L2_TERMINALDATAERR 15


#define PAYPASS_ERRID_L3_OK    0
#define PAYPASS_ERRID_L3_TIMEOUT 1
#define PAYPASS_ERRID_L3_STOP  2
#define PAYPASS_ERRID_L3_AMOUNTNOTPRESENT 3


#define PAYPASS_USERREQDATA_MSGID_CARDREADOK      0x17
#define PAYPASS_USERREQDATA_MSGID_TRYAGAIN        0x21   //33
#define PAYPASS_USERREQDATA_MSGID_APPROVED        0x03
#define PAYPASS_USERREQDATA_MSGID_APPROVEDSIGN    0x1A   //26
#define PAYPASS_USERREQDATA_MSGID_DECLINED        0x07
#define PAYPASS_USERREQDATA_MSGID_ERROR_OTHERCARD 0x1C   //28
#define PAYPASS_USERREQDATA_MSGID_INSERTCARD      0x1D   // 29
#define PAYPASS_USERREQDATA_MSGID_SEEPHONE        0x20   // 32
#define PAYPASS_USERREQDATA_MSGID_AUTHORISINGPLS  0x1B   // 27
#define PAYPASS_USERREQDATA_MSGID_CLEARDISP       0x1E   // 30



//#define PAYPASS_ERRID_MSGONERR_OTHERCARD 128   
//#define PAYPASS_ERRID_MSGONERR_TRYAGAIN  129


#define PAYPASS_USERREQDATA_STATUS_NOTREADY    0
#define PAYPASS_USERREQDATA_STATUS_IDLE        1
#define PAYPASS_USERREQDATA_STATUS_READYTOREAD 2
#define PAYPASS_USERREQDATA_STATUS_PROCESSING  3
#define PAYPASS_USERREQDATA_STATUS_CARDREADSUCCESS 4
#define PAYPASS_USERREQDATA_STATUS_PROCESSINGERR   5


#define PAYPASS_USERREQDATA_VQ_NONE     0
#define PAYPASS_USERREQDATA_VQ_AMOUNT   1
#define PAYPASS_USERREQDATA_VQ_BALANCE  2


#define PAYPASS_USERREQDATA_MSGHOLDTIME_DEFAULT "\x00\x00\x13"
#define PAYPASS_TORNLOG_MAXLIFETIME_DEFAULT     "\x00\x01\x2C"

#define PAYPASS_CCCTIMER 300


typedef struct {
    unsigned char CertFormat;        //'12'֤���ʽ
    unsigned char IssuID[4];        //���ʺ��������3-8����(�Ҳ�F)�����б�ʶ
    unsigned char ExpireDate[2];    //֤��ʧЧ����MMYY
    unsigned char CertSerial[3];    //֤�����к�
    unsigned char IPKSIGNAlgoInd;       // �����й�Կǩ���㷨��ʶ  SM2-'04'
    unsigned char IPKEncrptAlgoInd;        //�����й�Կ�����㷨��ʶ, �ݲ�ʹ�ã�ȡֵ'00'
    unsigned char IPKParamInd;       //�����й�Կ������ʶ
    unsigned char IPKLen;            //�����й�Կ���ֽڳ���
    unsigned char IPK[248];    //�����й�Կ NI
    unsigned char DGTLSGNTR[64];        //����ǩ��NCA
} SM_IPK_CRTFCT;


typedef struct {
    unsigned char CertFormat;      //'14'֤���ʽ
    unsigned char AppPAN[10];    //���ʺ�(�Ҳ�F)
    unsigned char ExpireDate[2];   //֤��ʧЧ����MMYY
    unsigned char CertSerial[3];    //֤�����к�
    unsigned char ICCPKSIGNAlgoInd;       // IC����Կǩ���㷨��ʶ  SM2-'04'
    unsigned char ICCPKEncrptAlgoInd;        //IC����Կ�����㷨��ʶ, �ݲ�ʹ�ã�ȡֵ'00'
    unsigned char ICCPKParamInd;       //IC����Կ������ʶ
    unsigned char ICCPKLen;
    unsigned char ICCPK[248];           //IC����Կ  NICC
    unsigned char DGTLSGNTR[64];        //����ǩ��NI
} SM_ICCPK_CRTFCT;


typedef struct {
    unsigned char DataFormat;
    //'15'֤���ʽ
    unsigned char ICCDynDataLen;        //LDD
    unsigned char ICCDynData[248];
    unsigned char DGTLSGNTR[64];        //����ǩ��NI
} SM_SIGN_DYN_APPDATA;


typedef struct {
    unsigned char DataFormat;
    //'13'
    unsigned char DataAuthCode[2];
    unsigned char DGTLSGNTR[64];        //����ǩ��NI
} SM_SIGN_STAT_APPDATA;


typedef struct {
    unsigned char ICCDynNumLen;
    unsigned char ICCDynNum[8];
    unsigned char CryptInfo;
    unsigned char AppCrypt[8];
    unsigned char HashResult[32];
} SM_ICC_DYN_DATA;


extern unsigned char Emvb_CheckDataMissDDA(EMVBAppUnionStruct * tempApp_UnionStruct);
extern unsigned char Emvb_GetCAPK(EMVBAppUnionStruct * tempApp_UnionStruct);
extern unsigned char Emvb_GetICCPK(EMVBAppUnionStruct * tempApp_UnionStruct);
extern unsigned char Emvb_initDynamicAuth(EMVBAppUnionStruct * tempApp_UnionStruct);

extern unsigned char Emvb_CombineDDA(unsigned char type, EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char Emvb_CommandIssuerAuthen(APDU_SEND *apdu_s,unsigned char *IssuAuthenData,unsigned char IssuAuthenDataLen);
extern unsigned char Emvb_IssuerAuthen(APDU_RESP *apdu_r);
extern unsigned char Emvb_SendScriptCommand(unsigned char* Script,unsigned char ScriptLen,APDU_SEND *apdu_s);

extern void EMVB_COMMAND_SELECT(unsigned char *DFFileName,unsigned char DFFileNameLen,unsigned char nextflag,APDU_SEND *apdu_s);

//extern void EmvbAidToTermInfo(unsigned char i,EmvSysIni_STRUCT *termaidparam,unsigned char onlinepinsup);

extern unsigned char EMVB_CheckReadAFL(EMVTAGCVLITEM *item);
extern void Emvb_FormReadAflData(APDU_SEND *apdu_s,unsigned char tempAFL_RecordNum,unsigned char SFI);
extern unsigned char Emvb_DOLProcessFunc(unsigned char type,unsigned char* DOL,unsigned short DOLLen,unsigned char* DOLData,unsigned short* DOLDataLen, EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Emv_FillSDA(unsigned char* SDAData,SIGN_DYN_APPDATA_RECOVER* recovSDA,unsigned char tempICCPKModulLen);

extern unsigned char contactless_checkifRedundantData(unsigned char *tag,unsigned char *rundatabuf,unsigned int rundatalen);
extern int Emvb_MatchTermAID(unsigned char* aid,unsigned char aidLen,TERMAPP *Applist,unsigned char listnum);
extern void EmvBcdSub(unsigned char *Dest,unsigned char *Src,unsigned char Len);
extern unsigned char qPBOCb_CheckExpDate(EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Paywaveb_ParseAndStoreCardResponse(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char *DataOut,unsigned short *DataOutIndex,unsigned char *parsebInTable,unsigned char readstep);
extern unsigned char Emvb_GetIPK(EMVBAppUnionStruct *tempApp_UnionStruct);
extern unsigned char Paywaveb_GPORetData(EMVBAppUnionStruct *tempApp_UnionStruct,APDU_RESP *apdu_r);
extern unsigned char Paypassb_ParseAndStoreCardResponse(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char *DataOut,unsigned short *DataOutIndex,unsigned char *parsebInTable,unsigned char readstep);
extern unsigned char Paypassb_DOLProcess(unsigned char type, unsigned char * DOL,unsigned short DOLLen,unsigned char* DOLData,unsigned short* DOLDataLen,EMVBAppUnionStruct *tempApp_UnionStruct);
extern void EMVB_restartqPBOCtrade_SelectNext(void);



extern unsigned char SM_CombineDDASignVerify(unsigned char type,EMVBAppUnionStruct *tempApp_UnionStruct);


extern unsigned char Emvb_SM_SignatureVerf(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char* dgtsgnt, unsigned char* Datatbsign, unsigned short DatatbsignLen, unsigned char *pkdata, unsigned short pkdatalen);

extern void EmvPrintFormat(unsigned char *src, unsigned short Len);



extern unsigned char SM_initDynamicAuth(EMVBAppUnionStruct *tempApp_UnionStruct,unsigned char type);
extern int  CheckSM2Sign(const char *pPkeyX, const char *pPkeyY, const char *pSignR, const char *pSignS,const char *pSrcData, unsigned short DatatbsignLen, const char *pIDA);
extern unsigned char Emvb_ExpireDateVerify(unsigned char* ExpireDate);
extern unsigned char Emvb_FormDynSignGenerate(EMVBAppUnionStruct *tempApp_UnionStruct,APDU_SEND *apdu_s);
extern unsigned char Emvb_DynSignGenerate(EMVBAppUnionStruct *tempApp_UnionStruct,APDU_RESP *apdu_r);
extern int SM3Encrypt(unsigned char *pInput, unsigned short usLen, unsigned char *pOutput);

#endif





