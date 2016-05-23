#ifndef SDKEMV_H
#define SDKEMV_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


typedef struct {
    unsigned char AidLen;
    unsigned char Aid[16];    //5-16
    unsigned char Asi;      //Application Selection Indicator.0-needn't match exactly(partial match up to the length);1-match exactly
    unsigned char AppVerNum[2];
    unsigned char TacDefault[5];
    unsigned char TacOnline[5];
    unsigned char TacDecline[5];
    unsigned char FloorLimit[4];
    unsigned char Threshold[4];
    unsigned char MaxTargetPercent;
    unsigned char TargetPercent;
    unsigned char TermDDOLLen;
    unsigned char TermDDOL[128];
    unsigned char TermPinCap;
    unsigned char vlptranslimit[6];     //huxiang add 091013 for Ec and qPBOC
    unsigned char termcvm_limit[6];     //huxiang add 091013 for Ec and qPBOC
    unsigned char clessofflineamt[6];     //huxiang add 091013 for Ec and qPBOC
    unsigned char clessofflinelimitamt[6];     //huxiang add 091013 for Ec and qPBOC
    unsigned char bShowRandNum;
    unsigned char bLocalName;    //If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char AppLocalNameLen;
    unsigned char AppLocalName[16];

    unsigned char bForceOnline;
} SDK_ICC_EmvSysIni_STRUCT;

typedef struct {
    unsigned char RID[5];
    unsigned char CAPKI;
    unsigned char HashInd;
    unsigned char ArithInd;
    unsigned char ModulLen;
    unsigned char Modul[248];
    unsigned char ExponentLen;
    unsigned char Exponent[3];
    unsigned char CheckSumLen;
    unsigned char CheckSum[20];
    unsigned char ExpireDate[4];        //ʧЧ��  //lechan 2006-1-9 02:20����
} SDK_ICC_CAPK_STRUCT;


#ifndef SDK_ICC_EMV_OK_BASE
#define SDK_ICC_EMV_OK_BASE (SDK_SYS_RET_OK_MIN + 100)     //(200)
#endif

#ifndef SDK_ICC_EMV_ERR_BASE
#define SDK_ICC_EMV_ERR_BASE (SDK_SYS_RET_ERR_MAX - 100)     //(-200)
#endif

typedef enum {
    SDK_Script72TimeOut = (SDK_ICC_EMV_ERR_BASE - 35),    //72�ű���ʱ
    SDK_QpbocAuthNoPerformed = (SDK_ICC_EMV_ERR_BASE -
                                34),    //qpboc������Ϊ����˫����֤//qpboc reader is not mutual authentication
    SDK_QpbocMoreCards = (SDK_ICC_EMV_ERR_BASE - 33),    //qpboc���࿨//qpboc detect multiple cards
    SDK_QpbocNoCard = (SDK_ICC_EMV_ERR_BASE - 32),    //qpboc����޿�//qpboc detect no card
    SDK_QpbocDDAAuthFail = (SDK_ICC_EMV_ERR_BASE -
                            31),    //qpboc������֤ʧ��//qpboc data authentication error
    SDK_EcLackBalanceErr = (SDK_ICC_EMV_ERR_BASE - 30),    //�����ֽ�����// e-cash balance lack
    SDK_QpbocReadLastLogErr = (SDK_ICC_EMV_ERR_BASE -
                               29),    //qpboc�����һ����¼ʧ��//qpboc failed to read last record
    SDK_FixedAidAbsentErr = (SDK_ICC_EMV_ERR_BASE -
                             28),    //ָ����AID������//the specificed AID not exist
    SDK_ECLoadErr = (SDK_ICC_EMV_ERR_BASE - 27),    //�����ֽ�Ȧ�����//e-cash load error
    SDK_ECNotSupportErr = (SDK_ICC_EMV_ERR_BASE - 26),    //��֧�ֵ����ֽ���//not support e-cash trade
    SDK_AppExpiredErr = (SDK_ICC_EMV_ERR_BASE - 25),    //Ӧ��ʧЧ//application expired
    SDK_AppEffectErr = (SDK_ICC_EMV_ERR_BASE - 24),    //Ӧ��δ��Ч//application is not effective
    SDK_UsePbocErr = (SDK_ICC_EMV_ERR_BASE - 23),    //ʹ��PBOC����//use PBOC to handle
    SDK_TransNotAcceptErr = (SDK_ICC_EMV_ERR_BASE - 22),    //���ײ�������//transaction is not accept
    SDK_TransTerminateErr = (SDK_ICC_EMV_ERR_BASE - 21),    //������ֹ//transaction terminated
    SDK_TransOfflineDeclineErr = (SDK_ICC_EMV_ERR_BASE -
                                  20),    //�����ѻ��ܾ�//transaction offline decline
    SDK_TransOnlineDeclineErr = (SDK_ICC_EMV_ERR_BASE -
                                 19),    //���������ܾ�//transaction online decline
    SDK_CDAFormatErr = (SDK_ICC_EMV_ERR_BASE - 18),    //CDA���ݸ�ʽ��//CDA data format error
    SDK_NoLogErr = (SDK_ICC_EMV_ERR_BASE - 17),    //�޼�¼//no record
    SDK_AppSelectTryAgainErr = (SDK_ICC_EMV_ERR_BASE - 16),    //����ѡ��Ӧ��//reselect application
    SDK_InitAppNotAcceptErr = (SDK_ICC_EMV_ERR_BASE -
                               15),    //��ʼ��Ӧ�ò�����//application initialization is not accepted
    SDK_AppBlockErr = (SDK_ICC_EMV_ERR_BASE - 14),    //Ӧ�ñ���//application is locked
    SDK_NeedMagCardErr = (SDK_ICC_EMV_ERR_BASE -
                          13),    //��Ҫ�ſ���fallback����//need magnetic card, fallback workflow
    SDK_AppTimeOutErr = (SDK_ICC_EMV_ERR_BASE - 12),    //��Ӧ�ó�ʱ//read card overtime
    SDK_CardBlockErr = (SDK_ICC_EMV_ERR_BASE - 11),    //����//card lock
    SDK_CancelTransErr = (SDK_ICC_EMV_ERR_BASE - 10),    //����ȡ��//cancel transaction
    SDK_NotAcceptErr = (SDK_ICC_EMV_ERR_BASE - 9),     //���񲻽���//not accept service
    SDK_IccDataRedundErr = (SDK_ICC_EMV_ERR_BASE - 8),     //���������ظ�//returned data reduplicated
    SDK_NoAppSelErr = (SDK_ICC_EMV_ERR_BASE - 7),     //û�п�ѡӦ��//no application to choose
    SDK_IccReturnErr = (SDK_ICC_EMV_ERR_BASE - 6),     //�������ݴ�//return data error
    SDK_IccCommandErr = (SDK_ICC_EMV_ERR_BASE - 5),     //���������//return command error
    SDK_IccDataFormatErr = (SDK_ICC_EMV_ERR_BASE - 4),     //���ݸ�ʽ��//data format error
    SDK_OnlineDecline = (SDK_ICC_EMV_ERR_BASE - 3),     //�����ܾ�//online reject
    SDK_OfflineDecline = (SDK_ICC_EMV_ERR_BASE - 2),     //�ѻ��ܾ�//offline reject
    SDK_PSENotFoundErr = (SDK_ICC_EMV_ERR_BASE - 1),     //û��PSEĿ¼//no PSE directory

    SDK_OfflineApprove = (SDK_ICC_EMV_OK_BASE + 1),      //�ѻ���׼//offline approve
    SDK_OnlineApprove = (SDK_ICC_EMV_OK_BASE + 2),      //������׼//online approve
    SDK_OnlineWait = (SDK_ICC_EMV_OK_BASE + 3),      //�����ȴ�//online wait
    SDK_UserSelect = (SDK_ICC_EMV_OK_BASE + 4),      //�û�ѡ��//select user
    SDK_ShowRandNum = (SDK_ICC_EMV_OK_BASE + 5),      //��ʾ�����//display random number
    SDK_NeedRefer = (SDK_ICC_EMV_OK_BASE + 6),      //��Ҫ�ο�//need reference
    SDK_NeedReversal = (SDK_ICC_EMV_OK_BASE + 7),      //��Ҫ����//need reversal
    SDK_DispLog = (SDK_ICC_EMV_OK_BASE + 8),      //��ʾ��¼//display log
    SDK_ReadLogEnd = (SDK_ICC_EMV_OK_BASE + 9),      //������¼β//read the end of log
    SDK_TransOnlineApprove = (SDK_ICC_EMV_OK_BASE +
                              10),     //����������׼//transaction online approved
    SDK_TransOfflineApprove = (SDK_ICC_EMV_OK_BASE +
                               11),     //�����ѻ���׼//transaction offline approved
    SDK_IccEasyFlow = (SDK_ICC_EMV_OK_BASE + 12),     //������//simplify flow
    SDK_ReadECBalance = (SDK_ICC_EMV_OK_BASE + 13),     //�������ֽ����//read e-cash balance
    SDK_TransOK_ARPCErr = (SDK_ICC_EMV_OK_BASE +
                           14),     //APRC����Ƭ�����׳ɹ�//APRC error, but the card allow transaction success
    SDK_ICCNoCapk = (SDK_ICC_EMV_OK_BASE + 15),     //zcl 20141117 qpboc����,û�й�Կ��ʱ�򷵻ش˷���ֵ
    SDK_ICCQProveOffline = (SDK_ICC_EMV_OK_BASE + 16)  //liyang

} SDK_EMV_RLT;


#define SDK_ICC_TRANS_MAXNUM    300                 //emv���׼�¼������//the maximum record of emv transaction
typedef struct {
    bool bIsFixed;                // TRUE:ѡ��̶���AID; FALSE:��ѡ��̶���AID  //TRUE: select the fixed AID; FALSE: do not select the fixed AID
    u8 uclen;                     // Aid���ֽڳ���//byte length of Aid
    u8 heAid[16];                 // Aid����//Aid data
} SDK_ICC_FIXED_AID;

typedef enum {
    SDK_ICC_ALL_FLOW = 1,               //��������//intact flow
    SDK_ICC_EASY_FLOW = 2,               //������//simplified flow
    SDK_ICC_EC_QUERY = 3,               //�����ֽ�����ѯ//query e-cash balance
    SDK_ICC_EC_LOAD = 4,               //Ȧ��//load
    SDK_ICC_READLOG = 5                //����־//read log
} SDK_ICC_FLOW_MODE;


typedef enum {
    SDK_ICC_WAIT,           //��ʾ"���Ժ�..."
    SDK_ICC_OFFLINE_PIN_ERR, //��ʾ"PIN�������!"
            SDK_ICC_OFFLINE_PIN_SUC, //��ʾ"������ȷ"
            SDK_ICC_INPUT_PIN_AGAIN//��ʾ"PIN�������!\r\n������",��ȡ"9F17",�ж�ֵ�Ƿ�Ϊ1������ǣ���ʾ"(���һ��)"
} SDK_ICC_PROMPT_TYPE;

typedef struct {
//    u8 bcTradeAmount[6];      //���׽��,bcd��,ǰ�油0x00  //trade amount, bcd code, add 0x00 from the head 
    bool bIsForceOnline;      // �Ƿ�ǿ������//whether force online
    bool bIsFallback;         // �Ƿ�֧��fallback  //whether support fallback
    bool bIsSupportQPBOC;     //�Ƿ�֧��QPBOC  //whether support QPBOC
    bool bIsSupportCLPBOC;    //�Ƿ�֧�ַǽ�PBOC  //whether support contactless PBOC
    bool bIsSupportEc;        // 0:��֧�ֵ����ֽ�; 1:֧�ֵ����ֽ�//0:not support e-cash, 1:support e-cash
    bool bIsPromptChooseEc;   // �Ƿ���ʾѡ������ֽ�//whether prompt to select e-cash
    bool bIsDefaultEc;        // ����ʾѡ������ֽ����ѡ������ֽ�ʱ,ʹ��ȱʡֵ1:ʹ�õ����ֽ�;0:��ʹ�õ����ֽ�
    //when not prompt select e-cash or select e-cash overtime, use default value 1:use e-cash; 0: not use e-cash
    SDK_ICC_FLOW_MODE eFlowMode;              //01��������;02������;03�����;04Ȧ��;05����־//01 intact flow; 02 simplified flow; 03 query balancd; 04:load; 05:read log
    SDK_ICC_FIXED_AID stFixedAid;             // �Ƿ�ѡ��̶���AID  //whether to choose fixed AID

    u8 *pasTradeAmount;//trade amount, ascii code

    void *pheReserved1;
    //Ԥ��ָ��
    void *pheReserved2;
    //Ԥ��ָ��
    void *pheReserved3;
    //Ԥ��ָ��
    void *pheReserved4;
    //Ԥ��ָ��
    void *pheReserved5;//Ԥ��ָ��

    void (*UserIccTransInit)(
            void);               //��IccTransInit���棬�����Զ����ʼ������//in Icc TransInit, user can custom initialization parameters
    s32 (*InputAmount)(
            u8 *pasTradeAmount);   // λ��InitialApp��һ��,���Խ�����:������Ȳ���//before InitialApp can proceed operations like enter amount etc.
    s32 (*DispReselectApp)(void);                 // ��ʾ����ѡ��Ӧ��;  //display reselect application
    s32 (*DispCardOut)(
            void);                     //��ʾ����Ƭ�ƿ�,InitialApp֮��;qpboc���õ�//display take away the card, after InitialApp the qpboc just be used
    s32 (*VerifyCardNo)(
            const u8 *pasPAN);    //λ��ReadAppData֮��,DataAuth֮ǰ,�ɽ�����:�ȽϺ�����/����ȷ�ϵȲ���//sjz modify 2012.08.15 19:3
    //located after ReadAppData, before DataAuth, operations like check black list/confirm card number etc. can be proceed
    s32 (*InputPWD)(const u8 *pasTradeAmount, u8 ucPINTryCount, u8 ucIccEncryptWay,
                    u8 *pheCreditPwd);    //��������//enter password
    s32 (*VerifyIDCard)(void);

    //֤����֤������ֵ:1->��֤ͨ��; ��1->��֤ʧ��//verify ID card, return value: 1->pass verification; not 1->failed to verify
    s32 (*DispOnlineRandNum)(
            u8 ucOnlineRandNum); // ��ʾ�����, �ն˷��չ���֮��,�ն���Ϊ����֮ǰ;  //display random number, after terminal risk management, before termianl behavior analysis
    s32 (*DispChooseEC)(
            void);                    //��ʾ�Ƿ�ѡ������ֽ�,��InitialApp�������;SDK_OK:��;SDK_TIME_OUT:Ĭ��;����:��    											  //display whether to select e-cash, be called in InitalApp, SDK_OK: yes; SDK_TIME_OUT: default; other: no
    s32 (*DispCandAppList)(u8 ucCandAppNum, u8 **pheCandApp);

    //sujianzhong 2013.04.23 15:4��ʾ��ѡӦ���б�
    s32 (*DispPromptData)(SDK_ICC_PROMPT_TYPE ePromptType);//����ePromptType��ʾ��ʾ��Ϣ

    void (*fun1)(void *param1, void *param2, void *param3, void *param4, void *param5);

    //Ԥ���ص�����1
    void (*fun2)(void *param1, void *param2, void *param3, void *param4, void *param5);

    //Ԥ���ص�����2
    void (*fun3)(void *param1, void *param2, void *param3, void *param4, void *param5);

    //Ԥ���ص�����3
    void (*fun4)(void *param1, void *param2, void *param3, void *param4, void *param5);

    //Ԥ���ص�����4
    void (*fun5)(void *param1, void *param2, void *param3, void *param4, void *param5);//Ԥ���ص�����5
} SDK_ICC_TRADE_PARAM;


typedef enum {
//������					            �������� 					   //Environment:                     Operating Control
//					        ���ڻ���(F)	�̻�(M)     �ֿ���(C)        //						Financial Institution(F)  Merchant(M)  Cardholder(C)
//�з���Ա��(SERVER)��											   //All Server (SERVER):
//		��������(ON)		  11         21             --					   //Only online(ON)                        11		21			--
//�������������ѻ�(ON_OFF)	  12	     22		        --			   //The offline have the ability to online(ON_OFF) 12    22		--
//		�����ѻ�(OFF)		  13         23             --					   //Only offline(OFF)			     13		23			--
//
//	������(SELF)��													   //Self-service(SELF):
//		��������(ON)		  14         24             34					   //Only online(ON)			     14		24		34
//�������������ѻ�(ON_OFF)	  15	     25             35				   //The offline have the ability to online(ON_OFF) 15	25		35
//		�����ѻ�(OFF)		  16         26             36					   //Only offline(OFF)			      16		26		36
//��ע���ն�����Ϊ'14','15','16'�Ҿ����ֽ�֧������(�����ն����ܵ��ֽ�1�����ֽ�λ=��1��)
//	���ն˱���Ϊ��ATM���������͵��ն˶���������ATM]
//Remarks: The terminals which type is '14','15','16' and have cash payment ability(subjoin terminal performance byte 1, 'cash' bit = '1') can be regard as ATM 
//other type terminals cannot be regard as ATM
            SDK_ICC_F_SERVER_ON = 11,
    SDK_ICC_F_SERVER_ON_OFF = 12,
    SDK_ICC_F_SERVER_OFF = 13,
    SDK_ICC_F_SELF_ON = 14,
    SDK_ICC_F_SELF_ON_OFF = 15,
    SDK_ICC_F_SELF_OFF = 16,
    SDK_ICC_M_SERVER_ON = 21,
    SDK_ICC_M_SERVER_ON_OFF = 22,
    SDK_ICC_M_SERVER_OFF = 23,
    SDK_ICC_M_SELF_ON = 24,
    SDK_ICC_M_SELF_ON_OFF = 25,
    SDK_ICC_M_SELF_OFF = 26,
    SDK_ICC_C_SELF_ON = 34,
    SDK_ICC_C_SELF_ON_OFF = 35,
    SDK_ICC_C_SELF_OFF = 36
} SDK_ICC_TERM_TYPE;

typedef enum {
    SDK_ICC_TRANS_TYPE_CASH = 0,                 //�ֽ�//cash
    SDK_ICC_TRANS_TYPE_GOODS = 1,               //����//goods
    SDK_ICC_TRANS_TYPE_SERVICE = 2,           //����//service
    SDK_ICC_TRANS_TYPE_CASHBACK = 3        //����//cashback
} SDK_ICC_TRANS_TYPE;


typedef struct {
    u8 asTerminalNo[9];         //�ն˺�"9F1C"  			//terminal number "9F1C"
    u8 heMerchCateCode[2];      //�̻�����"9F15"  			//merchant code "9F15"
    u8 asMerchantNo[16];        //�̻���"9F16" 				//merchant number "9F16"
    u8 asMerchantName[129];      //�̻�����"9F4E"  		//merchant name "9F4E"
    u8 bcCountryCode[2];        //���Ҵ���"9F1A"			//country code "9F1A"
    u8 bcTransCurcyCode[2];     //���׻��Ҵ���"5F2A"	//transaction currency code "5F2A"
    u8 ucTransCurcyExp;         //���׻���ָ��"5F36"	//transaction currency exponent "5F36"
    u8 heTermTransPredicable[4];
    //�ն˽�������"9F66"	//terminal transaction predicable "9F66"
    u8 ucTransTypeValue;        //��������ֵ"9C"			//transaction type value "9C"
    u8 heTermCapab[3];          //�ն�����"9F33"			//terminal capability "9F33"
    u8 heTermAddCapab[5];       //�ն˸�������"9F40"	//terminal additional capability "9F40"
    SDK_ICC_TERM_TYPE eTermType;
    //�ն�����"9F35"			//termianl type "9F35"
    SDK_ICC_TRANS_TYPE eTransType;//�ֽ�CASH=0;//����GOODS=1;//����SERVICE=2;//����CASHBACK=3;

} SDK_ICC_CFG_EMVPARAM;


//zcl 20130702
typedef struct {
    unsigned char ucAidLen;
    unsigned char asAid[32];
    //5-16
    unsigned char ucAppLabelLen;
    unsigned char asAppLabel[32];
    unsigned char ucPreferNameLen;
    unsigned char asPreferName[32];
    unsigned char ucPriority; //tag'87'
    unsigned char ucLangPreferLen;
    unsigned char asLangPrefer[16];
    //2-8
    unsigned char ucIcti; //Issuer Code Table Index.lang used for display app list according to ISO8859.but not include Chinese,Korea,etc.
    unsigned char ucLocalName;
    //If display app list using local language considerless of info in card.0-use card info;1-use local language.
    unsigned char ucAppLocalNameLen;
    unsigned char asAppLocalName[32];
} SDK_ICC_AIDLIST;        //Ӧ��������Ϣ

extern s32 sdkIccPowerOnAndSeek(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                SDK_ICC_PARAM *pstIccParam, u8 *pheUID);

extern s32 sdkIccPowerDown(void);

extern s32 sdkIccGetCardStatus(SDK_ICC_PARAM *pstIccParam, s32 siTimerCnt);

extern s32 sdkIccResetIcc(const SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccDealCardData(const SDK_ICC_PARAM *pstIccParam,
                              const SDK_ICC_APDU_SEND *pstApduSend, SDK_ICC_APDU_RESP *pstApduResp);


extern s32 sdkIccDelOneAID(const u8 *pheInAID, s32 siAIDLen);

extern void sdkIccDelAllAIDLists(void);

extern s32 sdkIccAddAnyAIDList(const SDK_ICC_EmvSysIni_STRUCT *pstInAIDList, s32 siAIDNum);

extern s32 sdkIccGetAnyAIDList(s32 siStartIndex, s32 siAIDNum,
                               SDK_ICC_EmvSysIni_STRUCT *pstOutAIDList, s32 *psiOutAIDNum);

extern s32 sdkIccGetAIDListNum(s32 * psiInAIDListNum);

extern s32 sdkIccGetMatchAID(const u8 *pheInAID, s32 siAIDLen, SDK_ICC_EmvSysIni_STRUCT *pstOutAID);

extern s32 sdkIccLoadAIDList(void);

extern void sdkIccSaveAIDFile(void);

extern s32 sdkIccDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex);

extern void sdkIccDelAllCAPKLists(void);

extern s32 sdkIccAddAnyCAPKList(const SDK_ICC_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum);

extern s32 sdkIccGetAnyCAPKList(s32 siStartIndex, s32 siCAPKNum,
                                SDK_ICC_CAPK_STRUCT *pstOutCAPKList, s32 *psiOutCAPKNum);

extern s32 sdkIccGetCAPKListNum(s32 * psiInCAPKListNum);

extern s32 sdkIccGetMatchCAPK(const u8 *pheInRID, s32 uiCAPKIndex, SDK_ICC_CAPK_STRUCT *pstOutCAPK);

extern s32 sdkIccLoadCAPKList(void);

extern void sdkIccSaveCAPKFile(void);

//extern s32 sdkIccCfgTerminalMsg(const u8 *pasTerminalNo, const u8 *pasMerchantNo, const u8 *pasMerchantName);
//extern s32 sdkIccCfgTermCapab(const u8 *pheLpIn);
//extern s32 sdkIccCfgTermAddCapab(const u8 *pheLpIn);
//extern s32 sdkIccCfgTermType(SDK_ICC_TERM_TYPE eTermType);
//extern s32 sdkIccCfgCountryCode(SDK_ICC_COUNTRY_NAME eCountryName);
//extern s32 sdkIccCfgTransCurcyCode(SDK_ICC_TRANS_CURCY_NAME eTransCurcyName);
//extern void sdkIccCfgTransCurcyExp(u8 ucTransCurcyExp);
//extern s32 sdkIccCfgTermTransPredicable(const u8 *pheLpIn);
//extern s32 sdkIccCfgTransType(SDK_ICC_TRANS_TYPE eTransType);
//extern void sdkIccCfgTransTypeValue(u8 ucTransTypeValue);


extern s32 sdkIccCfgEmvParam(const SDK_ICC_CFG_EMVPARAM *pstCfgEmvParam);

extern s32 sdkIccConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen);

extern s32 sdkIccReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen);

extern s32 sdkIccSendGetData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheTag, u8 *pheOutData,
                             s32 *psiOutLen);

extern s32 sdkIccGetOnlineRandNum(u8 * pheOnlineRandNum);

extern s32 sdkIccGetOnlineData(u8 ucOnlineResult, const u8 *pasRspCode, const u8 *pheIssuerAuthData,
                               s32 siIssuerAuthDataLen, const u8 *pucScript, s32 siScriptLen);

extern s32 sdkIccGetScriptResult(s32 * psiScriptResultLen, u8 * pheScriptResult);
extern s32 sdkIccGetECQueryMsg(u8 * pbcFirstCurrencyCode, u8 * pbcFirstBalance,
                               u8 * pbcSecondCurrencyCode, u8 * pbcSecondBalance);

extern s32 sdkIccReadLogDetail(const SDK_ICC_PARAM *pstIccParam, u8 ucLogNumIn, u8 *pheLogFormat,
                               SDK_ICC_APDU_RESP *pstLogData, u8 *pheLogNumOut);

extern s32 sdkIccReadEcLoadDetail(const SDK_ICC_PARAM *pstIccParam, u8 ucLogNumIn, u8 *pheLogFormat,
                                  SDK_ICC_APDU_RESP *pstLogData, u8 *pheLogNumOut);

extern s32 sdkIccTransInit(const SDK_ICC_TRADE_PARAM *pstIccTradeParam);

extern s32 sdkIccGetAppCandidate(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                 const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccOrganizeAppList(SDK_ICC_FIXED_AID stfixedAid,
                                 const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                 const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccUserSelectApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                               const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccFinalSelectedApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                  const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccReSelectApp(void);

extern s32 sdkIccInitialApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                            SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccReadAppData(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                             const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccDataAuth(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccProcessRestrict(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                 const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccCardHolderVerf(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTermRiskManage(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTermiAnalys(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                             const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTransAnalys(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                             const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTransProcess(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                              const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTransComplete(void);

extern s32 sdkIccReadEcData(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                            const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccReadECBalance(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                               const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccGoToSelectApp(SDK_ICC_FIXED_AID stFixedAid,
                               const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                               const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccGoToInitialApp(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccDealQpbocOnline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                 const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccDealQpbocOffline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                  const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccDealEmvFlow(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                             const SDK_ICC_PARAM *pstIccParam);

extern void sdkIccProcessRefer(u8 ucSource);

//����ο�������ĿǰĬ��Ϊ�ο�ʧ�ܣ������׾ܾ�//process reference, but now default is refer failed, that is trade rejected
extern s32 sdkIccClearEmvWater(void);

#define sdkIccClearEmvLog sdkIccClearEmvWater            //zcl 20140527

extern s32 sdkIccTransFlow1(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccTransFlow2(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                            const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccPowerStartInit(void); //modefied by fusuipu 2012-12-4

extern int sdkIccGetOfflineEncdata(unsigned char *encdata, unsigned int dataLen);//zhouxiaoxin


//============�ֻ�֧��Mobile Phone Payment========================
extern s32 sdkUpCardTransInit(void);

extern s32 sdkUpcardSelectApp(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkUpcardReadEF02(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkUpcardReadEF03(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkUpcardGetBCInfo(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkUpcardTransFlow(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkUpcardReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen);

extern s32 sdkUpcardGetMagData(SDK_ICC_CARDDATA * pstCardData);
//=============end============================

//���ƽ��ͷ�ļ�ͬ����ȷ����Ҫ������������
extern void sdkIccTransFreePartMem(s32 retCode);

extern void sdkIccTransFreeAllMem(void);

extern void sdkIccSetFactorAndIcKey(u8 *key);//xiaox 15/11/24
extern void sdkIccGetIcEnTrack(u8 *pOut);//xiaox 15/11/24
extern int sdkIccGetTrackData(unsigned char *magdata,unsigned int magdatalen);//xiaox 15/11/24
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

