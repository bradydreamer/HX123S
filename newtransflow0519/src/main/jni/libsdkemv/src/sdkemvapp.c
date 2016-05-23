#include "sdkdevOption.h"
#include "sdkGlobal.h"

#include "dllemvb.h"

#include "sdkIccPrivateHead.h"

#ifndef  DEBUG_EMV
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


//#define emvb_free sdkFreeMem
//#define emvb_malloc sdkGetMem

#define emvb_free free
#define emvb_malloc malloc


#define SDK_EMV_MAX_PATH 40

typedef struct _CAPKDATAINDEX {
    u8 RID[5];
    u8 CAPKI;
    u8 SMCapkFlag;
    u8 ifusedflag;
} CAPKDATAINDEX;

typedef struct _AIDDATAINDEX {
    u8 tradetypevalue;
    u8 appID;                        //add by HHL: Ӧ�ñ�ʶ
    u8 AIDLen;
    u8 AID[16];              //5-16
    u8 ASI;                  //0-partial match up to the length;1-match exactly
    u8 KernelID;
    u8 ifusedflag;
} AIDDATAINDEX;

typedef struct _PAYDATA {
    u8 selectmode;
    u8 amount[6];
    u8 amountother[6];
    u8 cardtype;
    u8 EC_IsMulAccountCard;
    SDK_ICC_PARAM *pstIccParam;
    u16 nowtask;
    u8 ifreaddatacomplete;
    u8 tradetypevalue;
    u8 SoundPlayFlag;
    u8 CheckBlackCardFlag;
    u8 flag_COMMAND_MPREPAREPAY;
    u8 ResetAmountFlag;
    u8 ContactCardStatus;
    u8 *ppaydata;
    u16 paydatalen;
} PAYDATA;

typedef struct _EMVTRADEGLBVALUE {
    unsigned char POSEntryMode;
    unsigned char TVR[5];
    unsigned char TSI[2];
} SDK_ICC_EMVTRADEGLBVALUE;


#define EMVB_ICC_NOCARD   3

/*=======BEGIN: ���ٲ� 2013.07.17  16:29 add===========*/
#define STLMAXRINDEXNUM 1024                                                                                            //����������ױ���
u16 gusSdkIndexEnd;                                                                                   //��¼����
/*====================== END======================== */



/*=======BEGIN: ���ٲ� 2013.07.16  19:21 add===========*/
#define SDK_ICC_AID_NUM 40 //aid����//sujianzhong 20131108
#define SDK_ICC_CAPK_NUM 50 //capk���������ų��������󣬶�Ϊ50��//sujianzhong 20131108

unsigned int guiSdkTransNum;
unsigned long guiSdkTransIndex;
unsigned long guiSdkTransLogMaxNum;
unsigned char gucSdkHaveInputAmount;
unsigned char gucSdkPosSmsTempAid[1 + 16];    //sxl?�����Ż���



extern SDK_CARD_DATA gstSdkMagCardData; //�ſ���Ϣ
//u8 gucSdkCurAppDir[65]; //��ǰӦ�ó���Ŀ¼

extern u8 gucSdkIccRfCheckBattle;                               //�ǽ�ѵ����ʱ���Ƿ��жϵ�ص�ѹ��ֻ�е�һ��ȥѵ����ʱ����жϣ���������Ҫ�ж�

//u8 gucSdkCurAppId[33];//��ǰӦ�ñ�ʶ//���ٲ� 2013.09.09 14:46
u8 gucSdkCardMode; //����ģʽ
/*====================== END======================== */

static u8 gucSdkEC_IsMulAccountCard; //��˫�˻������ֽ𿨻��Ǵ������ֽ𿨱�־ 1:˫�˻� 0:�������ֽ�
static u8 gucSdkEcCurrencyCode[1 + 2];
static u8 gucSdkEcSecCurrencyCode[1 + 2];
static u8 gucSdkCardInfoSecVLPAvailableFund[6];
#define IPKRevokeListStoredNum 0 //1 ��ʱ��֧��IPK check
//static u8 const gucSdktransTypeValue[4] = {0x01, 0x00, 0x00, 0x09};
//static const u8 gucSdkPINDesKey[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
static const char gcSdkIdTypeTab[][16] =
        {
                "���֤", "����֤", "����", "�뾳֤", "��ʱ���֤", "����"
        };

static const T_DISPSTR gstSdkstrDispTable_En[] =
        {
                /*  0*/ {"K350 EMV L2",      0,  4, 0},                                   // VERSION_INFO
                /*  1*/
                        {"Amount:",          0,  2, 0},                                               // AMOUNT
                /*  2*/
                        {"Amount OK?",       0,  6, 0},                    // AMOUNT_OK
                /*  3*/
                        {"Trans Approved",   8,  2, 0},                        // APPROVED
                /*  4*/
                        {"Call Your Bank",   8,  0, 0},                             // CALL_YOUR_BANK
                /*  5*/
                        {"Cancel or Enter",  4,  2, 0},                             // CANCEL_OR_ENTER
                /*  6*/
                        {"Card Read Error",  4,  2, 0},                             // CARD_READ_ERROR
                /*  7*/
                        {"Trans Declined",   8,  2, 0},                              // DECLINED
                /*  8*/
                        {"Enter Amount:",    0,  2, 0},                               // ENTER_AMOUNT
                /*  9*/
                        {"Enter PIN:",       0,  2, 0},                                          // ENTER_PIN

                /* 10*/
                        {"Incorrect PIN!",   8,  1, 0},                              // INCORRECT_PIN
                /* 11*/
                        {"Pls Insert Card",  0,  3, 0},                             // INSERT_CARD
                /* 12*/
                        {"Not Accepted",     16, 2, 0},                               // NOT_ACCEPTED
                /* 13*/
                        {"PIN OK",           40, 3, 0},                                             // PIN_OK
                /* 14*/
                        {"Please Wait...",   8,  3, 0},                      // PLEASE_WAIT
                /* 15*/
                        {"Processing Error", 0,  2, 0},                            // PROCESSING_ERROR
                /* 16*/
                        {"Pls Remove Card",  4,  6, 0},                             // REMOVE_CARD
                /* 17*/
                        {"Use Chip Reader",  4,  3, 0},                            // USE_CHIP_READ
                /* 18*/
                        {"Use Mag Stripe",   8,  3, 0},                             // USE_MAG_STRIPE

                /* 19*/
                        {"        ",         0,  0, 0},                                      // RFU

                /* 20*/
                        {"1.BATCH UP",       0,  0, 0x10},                            // BATCH_UP
                /* 21*/
                        {"2.PRINT RECEIPT",  0,  2, 0x10},                          // PRINT_RECEIPT
                /* 22*/
                        {"3.UPDATE PARA",    0,  4, 0x10},                     // UPDATE_PARA

                /* 23*/
                        {"4.CHECK TVR",      0,  6, 0x10},                                // CHECK_TVR
                /* 24*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 25*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 26*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 27*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 28*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 29*/
                        {"        ",         0,  0, 0},                                      // RFU


                /* 30*/
                        {"1.SET DATE&TIME",  0,  0, 0x10},                           // SET_SYSTIME
                /* 31*/
                        {"2.SET TID&MID",    0,  2, 0x10},                      // SET_TIDMID
                /* 32*/
                        {"3.SET LANGUAGE",   0,  4, 0x10},                     // SELECT_LANGUAGE
                /* 33*/
                        {"4.SET TAC",        0,  6, 0x10},                   // SET_TAC
                /* 34*/
                        {"1.SET TRM PARA",   0,  0, 0x10},                            // SET_TRMPARA
                /* 35*/
                        {"2.SET TRANS TYPE", 0,  2, 0x10},                   // SET_TRANSTYPE

                /* 36*/
                        {"3.SET VLP PARA",   0,  4, 0x10},                     // SET_VLPPARA
                /* 37*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 38*/
                        {"        ",         0,  0, 0},                                      // RFU
                /* 39*/
                        {"        ",         0,  0, 0},                                      // RFU

                /* 40*/
                        {"Trans Refused",    0,  0, 0},                                 // TRANS_REFUSED
                /* 41*/
                        {"Referral",         32, 0, 0x0},                                   // BYPASS_REFERRAL

                /* 42*/
                        {"1.Accept Trans",   0,  2, 0},                                // ACCEPT_TRANS
                /* 43*/
                        {"2.Decline Trans",  0,  4, 0},                       // DECLINE_TRANS
                /* 44*/
                        {"3.Force Online",   0,  6, 0},                        // FORCE_ONLINE
                /* 45*/
                        {"App Select",       24, 0, 0},                                   // TITLE_APP_SELECT
                /* 46*/
                        {"Mag Card Error",   12, 6, 0},                       // MAGCARD_READ_ERROR
                /* 47*/
                        {"EMV APP TEST",     16, 0, 0},                                 // EMV_LEVEL2_TEST
                /* 48*/
                        {"XGD TECH LTD.",    0,  4, 0},                                // COMPANY_NAME
                /* 49*/
                        {"English",          45, 2, 0},                                      // EMV_ENGLISH
                /* 50*/
                        {"Chinese",          45, 6, 0},                                            // EMV_CHINESE
                /* 51*/
                        {"Setup Date",       24, 0, 0},                                         // SETUP_DATE

                /* 52*/
                        {"(YYMMDD)",         0,  2, 0},                                            // YYMMDD
                /* 53*/
                        {"Old:",             0,  2, 0},                                                // OLD_FL
                /* 54*/
                        {"New:",             0,  4, 0},                                                  // NEW_FL
                /* 55*/
                        {"Setup Time",       24, 0, 0},                                          // SETUP_TIME
                /* 56*/
                        {"(HHMM)",           40, 2, 0},                                               // HHMMSS
                /* 57*/
                        {"Terminal ID",      20, 0, 0},                                  // TERMINAL_ID
                /* 58*/
                        {"Old:",             0,  2, 1},                                                  // OLD_ID
                /* 59*/
                        {"New:",             0,  5, 1},                                                  // NEW_ID
                /* 60*/
                        {"Merchant ID",      20, 0, 0},                                  // MERCHANT_ID
                /* 61*/
                        {"TAC-Denial",       24, 0, 0},                                   // TAC_DENIAL

                /* 62*/
                        {"TAC-Online",       24, 0, 0},                                   // TAC_ONLINE
                /* 63*/
                        {"TAC-Default",      20, 0, 0},                                  // TAC_DEFAULT
                /* 64*/
                        {"Old:",             0,  4, 0},                                                  // OLD_TAC
                /* 65*/
                        {"New:",             0,  6, 0},                                                  // NEW_TAC
                /* 66*/
                        {"Floor Limit",      20, 0, 0},                                  // FLOOR_LIMIT
                /* 67*/
                        {"Threshold",        28, 0, 0},                                    // THRESHOLD
                /* 68*/
                        {"TP",               56, 0, 0},                                                   // TP
                /* 69*/
                        {"MTP",              52, 0, 0},                                                  // MTP
                /* 70*/
                        {"Need PIN?",        28, 2, 0},                    // NEED_PIN
                /* 71*/
                        {"Enter for YES",    16, 5, 1},                                // ENTER_FOR_YES

                /* 72*/
                        {"Cancel for NO",    16, 6, 1},                                // CANCEL_FOR_NO
                /* 73*/
                        {"No App Trans",     16, 3, 0},                                 // NO_APP_TRANS
                /* 74*/
                        {"No Paper",         32, 3, 0},                                     // PRN_NO_PAPER
                /* 75*/
                        {"Printing...",      20, 3, 0},                                  // PRN_PRINTING
                /* 76*/
                        {"Put On Base",      20, 3, 0},                                  // PUT_ON_BASE
                /* 77*/
                        {"Print Error",      20, 3, 0},                                  // PRN_ERROR
                /* 78*/
                        {"No Batch Data",    12, 3, 0},                                // NO_BATCH_DATA
                /* 79*/
                        {"Online Error",     16, 3, 0},                                 // ONLINE_ERROR
                /* 80*/
                        {"Read Track1 OK",   8,  0, 0},                                // READ_TRACK1_OK
                /* 81*/
                        {"Read Track2 OK",   8,  2, 0},                                // READ_TRACK2_OK
                /* 82*/
                        {"Read Track3 OK",   8,  4, 0},                                // READ_TRACK3_OK
                /* 83*/
                        {"Batch Up OK",      20, 3, 0},                                  // BATCH_UP_OK
                /* 84*/
                        {"Trans Completed",  4,  2, 0},                               // TRANS_COMPLETED
                /* 85*/
                        {"Trans Error",      20, 2, 0},                                  // TRANS_TERMINATED
                /* 86*/
                        {"Dialing...",       24, 2, 0},                                   // DIALING
                /* 87*/
                        {"Service Forbid",   0,  3, 0},                                // SERVICE_NOT_ALLOWED
                /* 88*/
                        {"Trans Type",       24, 0, 0},                                   // TITLE_TRANS_TYPE
                /* 89*/
                        {"Try Again",        28, 4, 0},                                    // TRY_AGAIN
                /* 90*/
                        {"Waiting for PIN",  4,  3, 0},                               // WAIT_FOR_PIN
                /* 91*/
                        {"( VLP )",          36, 2, 0},                                      // STR_VLP
                /* 92*/
                        {"VLP Trans Limit",  4,  0, 0},                               // VLP_TRANS_LIMIT
                /* 93*/
                        {"(Last PIN Try)",   8,  6, 0},                                // LAST_PIN_TRY
                /* 94*/
                        {"Trans Amount",     16, 0, 0},                                 // TRANS_AMOUNT
                /* 95*/
                        {"CashBack Amount",  4,  0, 0},                               // CASHBACK_AMOUNT
        };

static const T_DISPSTR gstSdkstrDispTable_Ch[] = {
        /*  0*/ {"   EMV Level2",     0,  4, 0},                                 // VERSION_INFO
        /*  1*/
                {"���:",              0,  2, 0},                                                 // AMOUNT
        /*  2*/
                {"ȷ�Ͻ��?",            0,  6, 0},                                     // AMOUNT_OK
        /*  3*/
                {"������׼",           32, 3, 0},                                     // APPROVED
        /*  4*/
                {"����������ϵ",       16, 0, 0},                               // CALL_YOUR_BANK
        /*  5*/
                {"ȷ�ϻ�ȡ��",           24, 3, 0},                                         // CANCEL_OR_ENTER
        /*  6*/
                {"��������",          32, 2, 0},                                           // CARD_READ_ERROR
        /*  7*/
                {"���׾ܾ�",            32, 2, 0},                                           // DECLINED
        /*  8*/
                {"������:",           0,  2, 0},                                           // ENTER_AMOUNT
        /*  9*/
                {"����PIN����:",      0,  2, 0},                                // ENTER_PIN

        /* 10*/
                {"PIN�������!",       16, 1, 0},                               // INCORRECT_PIN
        /* 11*/
                {"�����IC��",         24, 3, 0},                                         // INSERT_CARD
        /* 12*/
                {"������",            24, 2, 0},                                     // NOT_ACCEPTED
        /* 13*/
                {"������ȷ",           32, 3, 0},                                           // PIN_OK
        /* 14*/
                {"���Ժ�...",          28, 3, 0},                                  // PLEASE_WAIT
        /* 15*/
                {"�������",           32, 3, 0},                                           // PROCESSING_ERROR
        /* 16*/
                {"��γ�IC��",          24, 6, 0},                                         // REMOVE_CARD
        /* 17*/
                {"����IC��",          32, 3, 0},                                          // USE_CHIP_READ
        /* 18*/
                {"���ô�����",         24, 3, 0},                                // USE_MAG_STRIPE

        /* 19*/
                {"        ",          0,  0, 0},                                      // RFU

        /* 20*/
                {"1.������",          0,  0, 0x10},                              // BATCH_UP
        /* 21*/
                {"2.��ӡ���׵���",      0,  2, 0x10},                           // PRINT_RECEIPT
        /* 22*/
                {"3.��������",        0,  4, 0x10},                                // UPDATE_PARA

        /* 23*/
                {"4.�鿴TVR ",          0,  6, 0x10},                                 // CHECK_TVR
        /* 24*/
                {"5.������־",         0,  8, 0x10},                                         // RFU
        /* 25*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 26*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 27*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 28*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 29*/
                {"        ",          0,  0, 0},                                      // RFU

        /* 30*/
                {"1.��������ʱ��",     0,  0, 0x10},                            // SET_SYSTIME
        /* 31*/
                {"2.����TID&MID",     0,  2, 0x10},                      // SET_TIDMID
        /* 32 {"3.����ѡ��",0, 4, 0x10}, */                                 // SELECT_LANGUAGE
                {"3.Ӧ��ѡ��",          0,  4, 0x10},
        /* 33*/
                {"4.����TAC",         0,  6, 0x10},                               // SET_TAC
        /* 34*/
                {"1.����TRM����",     0,  0, 0x10},                             // SET_TRMPARA
        /* 35*/
                {"2.���ý�������",     0,  2, 0x10},                     // SET_TRANSTYPE
        /* 36*/
                {"3.����VLP����",     0,  4, 0x10},                              // SET_VLPPARA

        /* 37*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 38*/
                {"        ",          0,  0, 0},                                      // RFU
        /* 39*/
                {"        ",          0,  0, 0},                                      // RFU

        /* 40*/
                {"���ױ��ܾ�",          0,  0, 0x10},                                 // TRANS_REFUSED
        /* 41*/
                {"�ο�ѡ��",            0,  0, 0x10},                                   // BYPASS_REFERRAL

        /* 42*/
                {"1.���ܽ���",         0,  2, 0},                                    // ACCEPT_TRANS
        /* 43*/
                {"2.�ܾ�����",         0,  4, 0},                                    // DECLINE_TRANS
        /* 44*/
                {"3.ǿ����������",     0,  6, 0},                        // FORCE_ONLINE
        /* 45*/
                {"Ӧ��ѡ��",            32, 0, 0},                                     // TITLE_APP_SELECT
        /* 46*/
                {"���ſ�����",         12, 6, 0},                                   // MAGCARD_READ_ERROR
        /* 47*/
                {"EMV/VSDCӦ�ò���",    0,  0, 0},                      // EMV_LEVEL2_TEST
        /* 48*/
                {"�¹������޹�˾",       0,  4, 0},                               // COMPANY_NAME
        /* 49*/
                {"Ӣ��",               45, 2, 0},                                                 // EMV_ENGLISH
        /* 50*/
                {"����",              45, 6, 0},                                               // EMV_CHINESE
        /* 51*/
                {"��������",          32, 0, 0},                                           // SETUP_DATE

        /* 52*/
                {"(YYMMDD)",          32, 2, 0},                                           // YYMMDD
        /* 53*/
                {"�ɣ�",               0,  2, 0},                                                // OLD_FL
        /* 54*/
                {"�£�",               0,  4, 0},                                                  // NEW_FL
        /* 55*/
                {"����ʱ��",           32, 0, 0},                                            // SETUP_TIME
        /* 56*/
                {"(HHMM)",            40, 2, 0},                                               // HHMMSS
        /* 57*/
                {"�ն˺�",              40, 0, 0},                                               // TERMINAL_ID
        /* 58*/
                {"Old:",              0,  2, 1},                                                  // OLD_ID
        /* 59*/
                {"New:",              0,  5, 1},                                                  // NEW_ID
        /* 60*/
                {"�̻���",             40, 0, 0},                                               // MERCHANT_ID
        /* 61*/
                {"TAC-�ܾ�",           32, 0, 0},                                     // TAC_DENIAL

        /* 62*/
                {"TAC-����",          32, 0, 0},                                     // TAC_ONLINE
        /* 63*/
                {"TAC-Ĭ��",           32, 0, 0},                                     // TAC_DEFAULT
        /* 64*/
                {"�ɣ�",               0,  4, 0},                                                  // OLD_TAC
        /* 65*/
                {"�£�",               0,  6, 0},                                                  // NEW_TAC
        /* 66*/
                {"����޶�",            32, 0, 0},                                     // FLOOR_LIMIT
        /* 67*/
                {"��ֵ",               48, 0, 0},                                                 // THRESHOLD
        /* 68*/
                {"Ŀ��ٷ���",           24, 0, 0},                                   // TP
        /* 69*/
                {"���Ŀ��ٷ���",        8,  0, 0},                                // MTP
        /* 70*/
                {"��Ҫ���룿",           24, 0, 0},                   // NEED_PIN
        /* 71*/
                {"Enter for ��",      12, 4, 0},                                 // ENTER_FOR_YES

        /* 72*/
                {"Cancel for ��",     12, 6, 0},                                // CANCEL_FOR_NO
        /* 73*/
                {"��Ӧ�ý���",          24, 3, 0},                                   // NO_APP_TRANS
        /* 74*/
                {"��ӡ��ȱֽ",           24, 3, 0},                                   // PRN_NO_PAPER
        /* 75*/
                {"���ڴ�ӡ...",         20, 3, 0},                                  // PRN_PRINTING
        /* 76*/
                {"��Ż�����",          24, 3, 0},                                   // PUT_ON_BASE
        /* 77*/
                {"��ӡʧ��",            32, 3, 0},                                     // PRN_ERROR
        /* 78*/
                {"�������ݼ�¼",        16, 3, 0},                                 // NO_BATCH_DATA
        /* 79*/
                {"�����������",       16, 3, 0},                                 // ONLINE_ERROR
        /* 80*/
                {"���ŵ�1�ɹ�",         20, 0, 0},                                  // READ_TRACK1_OK
        /* 81*/
                {"���ŵ�2�ɹ�",         20, 2, 0},                                  // READ_TRACK2_OK
        /* 82*/
                {"���ŵ�3�ɹ�",         20, 4, 0},                                  // READ_TRACK3_OK
        /* 83*/
                {"�����ͳɹ�",          24, 3, 0},                                   // BATCH_UP_OK
        /* 84*/
                {"���׽���",           32, 2, 0},                                     // TRANS_COMPLETED
        /* 85*/
                {"���״���",           32, 2, 0},                                     // TRANS_TERMINATED
        /* 86*/
                {"���ڲ���...",        20, 2, 0},                                  // DIALING
        /* 87*/
                {"ҵ�񱻾ܾ�",            0,  3, 0},                                    // SERVICE_NOT_ALLOWED
        /* 88*/
                {"��������",          32, 0, 0},                                     // TITLE_TRANS_TYPE
        /* 89*/
                {"������",            40, 4, 0},                                       // EMV_TRY_AGAIN
        /* 90*/
                {"�ȴ���������...",    4,  3, 0},                               // WAIT_FOR_PIN
        /* 91*/
                {"( VLP )",           36, 2, 0},                                      // STR_VLP
        /* 92*/
                {"VLP�����޶�",        20, 0, 0},                                  // VLP_TRANS_LIMIT
        /* 93*/
                {"( ���һ�� )",        16, 6, 0},                                 // LAST_PIN_TRY
        /* 94*/
                {"���׽��",            32, 0, 0},                                     // TRANS_AMOUNT
        /* 95*/
                {"���ֽ��",            32, 0, 0},                                     // CASHBACK_AMOUNT
        /* 96*/
                {"PBOCӦ�ò���",        0,  0, 0},                                  // PBOC_VER
        /* 97*/
                {"3.��ʼ������",       0,  4, 0x10},                               // INITEMV_SYSDATA
        /*98*/
                {"Ӧ�ñ���!",           24, 3, 0},                                     //APP_BLOCK
        /*99*/
                {"������ʱ,��ȡ������", 0,  2, 0},     //CARD_READ_TIMEOUT
        /*100*/
                {"���ʾ��Ƭ",           24, 4, 0},                                   //CLess card
        /*101*/
                {"�ն˲�֧�ִ�CVM",       24, 3, 0},                              //Term_CVM no support
        /*102*/
                {"�ɽ���Ƭ�ƿ�",         24, 3, 0},                                 //remove Icc
        /*103*/
                {"��ʹ��MSD����",      24, 3, 0},                                //use MSD
        /*104*/
                {"��ʹ��PBOC����",     24, 3, 0},                               //use PBOC
};


#if 0
static const IPK_REVOKE gstSdkIPKRevokeListStored[] = {
    {{0xA0, 0x00, 0x00, 0x00, 0x03}, 0x96, {0x12, 0x34, 0x56}}
};

static const ISSUER_TAC gstSdkVisaVLPTAC = { {0x7C, 0x70, 0xB8, 0x08, 0x00}, {0x00, 0x00, 0x00, 0x00, 0x00}, {0x7C, 0x70, 0xB8, 0x08, 0x00}};
#endif

static const s32 giSdkEmv2SdkRlt[][2] = {
        {EMV_OK,                          SDK_OK},               // 0
        {EMV_ERR,                         SDK_ERR},             // 1
//{APPSEL_PSENOTFOUND ,},              //3,
        {OFFLINE_APPROVE,                 SDK_OfflineApprove},      //10
        {OFFLINE_DECLINE,                 SDK_OfflineDecline},          //11
        {ONLINE_APPROVE,                  SDK_OnlineApprove},        //12
        {ONLINE_DECLINE,                  SDK_OnlineDecline},        //13
        {ONLINE_WAIT,                     SDK_OnlineWait},                           //14

        {ERR_EMV_IccDataFormat,           SDK_IccDataFormatErr},       //34,
        {ERR_EMV_IccCommand,              SDK_IccCommandErr},          //35
        {ERR_EMV_IccReturn,               SDK_IccReturnErr},               //36,
        {ERR_EMV_NoAppSel,                SDK_NoAppSelErr},               //37,
        {ERR_EMV_IccDataRedund,           SDK_IccDataRedundErr},       //39,
        {ERR_EMV_NotAccept,               SDK_NotAcceptErr},              //48,
        {ERR_EMV_CancelTrans,             SDK_CancelTransErr},            //50,
        {ERR_EMV_CardBlock,               SDK_CardBlockErr},              //51,
        {ERR_APP_TIMEOUT,                 SDK_AppTimeOutErr},                //54
        {ERR_NEEDMAGCARD,                 SDK_NeedMagCardErr},                 //56
        {ERR_BLOCKORFORMATERR,            SDK_CardBlockErr},           //57//sujianzhong 2012.11.30 9:43
        {EMV_APPBLOCK,                    SDK_AppBlockErr},                       //59,
        {EMV_USERSELECT,                  SDK_UserSelect},                     //63,
        {ERR_EMVINITAPP_NOTACCEPED,       SDK_InitAppNotAcceptErr},   //64,
        {EMV_APPSELECTTRYAGAIN,           SDK_AppSelectTryAgainErr},      //72,
        {EMV_SHOWRANDNUM,                 SDK_ShowRandNum},                 //74,
        {EMV_NEEDREFER,                   SDK_NeedRefer},                      //76
        {EMV_NEEDREVERSAL,                SDK_NeedReversal},          //77
        {EMV_DISPEMVRECORD,               SDK_DispLog},               //83,
        {EMV_NORECORD,                    SDK_NoLogErr},                       //85,
        {EMV_READLOGEND,                  SDK_ReadLogEnd},                     //86,
        {ERR_EMV_CDAFORMAT,               SDK_CDAFormatErr},              //88,
        {TRANS_ONLINE_APPROVE,            SDK_TransOnlineApprove},        //100
        {TRANS_ONLINE_DECLINE,            SDK_TransOnlineDeclineErr},           //101
        {TRANS_OFFLINE_APPROVE,           SDK_TransOfflineApprove},        //103
        {TRANS_OFFLINE_DECLINE,           SDK_TransOfflineDeclineErr},          //104
        {TRANS_TERMINATE_TRANS_ERR,       SDK_TransTerminateErr},   //105
        {TRANS_TERMINATE_TRANS_NOTACCEPT, SDK_TransNotAcceptErr},   //106,
        {EMV_ICCGETPAN,                   SDK_IccEasyFlow},                          //108,
        {qPBOC_USEPBOC,                   SDK_UsePbocErr},                          //112,
        {qPBOC_APPEFFECT,                 SDK_AppEffectErr},                      //113,
        {qPBOC_APPEXPIRED,                SDK_AppExpiredErr},                    //114,
        {EC_NOTSUPPORT,                   SDK_ECNotSupportErr},                          //115,
        {EC_QUANCUNERR,                   SDK_ECLoadErr},                        //116,
//    {SDK_ICC_NOCARD, SDK_ICC_NOCARD},////sujianzhong 2013.02.24 12:50
};


#define CAPKSHIFTNUM   9
#define CAPKFLAGOFFSET 510
#define CAPKLEN     512     //sxl
#define CAPKENABLE  0x55
#define CAPKDISABLE 0xFF

#define MAXCAPKINDEX 100
CAPKDATAINDEX gCapkIndex[MAXCAPKINDEX];


#ifndef EMVBlockLen
#define EMVBlockLen 4096
#endif


#define AIDSHIFTNUM    9
#define AIDFLAGOFFSET  510
#define AIDLEN         512     //sxl
#define AIDENABLE      0x55
#define AIDDISABLE     0xFF


#define MAXAIDINDEX  50
AIDDATAINDEX gAidIndex[MAXAIDINDEX];
EMVBAppUnionStruct *GlbEMVBApp_UnionStruct = NULL;
TRADEPARAMETER *gEMVTradeParam;
EMVBERRORID gEmvErrID;
u8 gLanguage = EMV_CHINESE;
PAYDATA gPayData;


SDK_ICC_EMVTRADEGLBVALUE gEmvTradeGlbValue = {0};

#if 0 /*Modify by ���ٲ� at 2014.03.28  15:21 */
/*=======BEGIN: ���ٲ� 2013.07.22  8:37 add===========*/
//Ϊ����ſ���IC��������
bool sdkIccInitReadCard(void)  //sxl20140408
{
#if 0 /*Modify by ���ٲ� at 2013.02.27  18:21 */
    sdk_dev_clear_app_fifo();
#endif /* if 0 */
//    sdkIccClearDevComData(sdkSysGetComNo(SDK_SYS_COM_PIN_PAD));//sujianzhong 2013.04.12 8:42
//    memset(&gstSdkIccReadCard, 0, sizeof(SDK_ICC_READ_CARD));//���ٲ� 2013.07.22 8:26����ʹ��
//    memset(&gstSdkIccCardFifo, 0, sizeof(FIFO));//shiweisong 2013.06.19 14:54
    bIsSdkIccSendCLPoweron = 0;
    bIsSdkIccSendIccStatus = 0;
    guiSdkIccStatusTimeId = 0;
//    gucSdkIccQreaderStep = 0;//sujianzhong 2013.09.02 16:21
    return true;
}

#endif
/*====================== END======================== */

/*void emv_uart_printf(char *fmt,...)  //sxl?
   {
    int length = 0;
    va_list ap;
    char string[256];
    char *pt;

    va_start(ap,fmt);
    vsprintf((char *)string,(const char *)fmt,ap);
    pt = &string[0];
    while(*pt!='\0')
    {
        length++;
        pt++;
    }
    //core->CoreApi_sys_uart_printf("%s", string); //д���ڳɹ�
    ddi_sys_printf(string);
    va_end(ap);
   }*/


/*****************************************************************************
** Descriptions:    �ϵ粢Ѱ��(������ʽ)����Ҫ����sdkIccOpenRfDev���豸��
** Parameters:      u8 ucCLReaderType:����������:0->���ã�1->����
                    SDK_ICC_TRADE_PARAM  *pstIccTradeParam
                    SDK_ICC_PARAM *pstIccParam
                    u8 *pheDataOut: pheDataOut[0]Ϊ���ݳ���,pheDataOut[1~n]Ϊ��ֵ
** Returned value:  SDK_OK:Ѱ�������Ҹ�λ�ɹ�
                    SDK_ERR:Ѱ��ʧ��
                    SDK_PARA_ERR:��������
                    SDK_ICC_NOCARD:�޿�
                    SDK_ICC_MORECARD	��⵽���ſ�

** Created By:      sujianzhong   2012.09.27
** Remarks:  //sxl20140408
*****************************************************************************/
s32 sdkIccPowerOnAndSeek(const SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam,
                         u8 *pheDataOut) {
    //u8 *rbuf;  //[1024] = {0}
    s32 rlen = 0;
    //s32 len = 0;
    //s32 readcardrst = SDK_ICC_NOCARD;
    s32 ret;


    if (pstIccParam == NULL) {
        return SDK_PARA_ERR;
    }
    if (gucSdkIccRfCheckBattle == 0) {
        gucSdkIccRfCheckBattle = 1;

        //zcl 20141101
        if (SDK_OK != sdkRfBatteryEnough()) {
            return SDK_ICC_BAT_VOLTAGE_LOW;
        }
    }
    Trace("emv", "\r\n go->use inside rf \r\n");
    ret = ddi_rf_poweron(0x03);
        Trace("emv","ddi_rf_poweron = %d",ret);
    if (DDI_ERR == ret || DDI_EIO == ret) //Ѱ��ʧ��
    {
        return SDK_ICC_RESET_ERR;
    }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
        else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
    {
        return RF_INTERFRENCE_MAG;
    }
#endif /* if 0 */
    else if (DDI_EINVAL == ret) {
        return SDK_PARA_ERR;
    }

    rlen = ddi_rf_get_status();
    Trace("emv", "ddi_rf_get_status rlen = %d\r\n", rlen);

    if (rlen == 2) {
        return SDK_ICC_MORECARD;
    }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
        else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
    {
        return RF_INTERFRENCE_MAG;
    }
#endif /* if 0 */
    else if (rlen > 2) {
        pstIccParam->ucCardMode = SDK_ICC_RF;
        pstIccParam->ucSlotNo = 0;

        ret = ddi_rf_ioctl(DDI_RF_CTL_UID, 0, (u32) pheDataOut);//����ҵ��д����

        if (DDI_OK != ret) //������
        {
            pheDataOut[0] = 0;
        }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
        else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
        {
            return RF_INTERFRENCE_MAG;
        }
#endif /* if 0 */
        return SDK_OK;
    }

    return SDK_ICC_NOCARD;
}

/*====================== END======================== */

/*****************************************************************************
** Descriptions:   �ǽӶ������µ�
** Parameters:     void
** Returned value:
** Created By:     sujianzhong20130415
** Remarks:        //sxl20140408
*****************************************************************************/
s32 sdkIccPowerDown(void) {
    s32 ret;

    Trace("emv", "goto->Icc_CTLSPowerDown \r\n");

    gucSdkIccRfCheckBattle = 0;

    ret = ddi_rf_poweroff();

    if (DDI_OK == ret) {
        return SDK_OK;
    }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
        else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
    {
        return RF_INTERFRENCE_MAG;
    }
#endif /* if 0 */
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	        ��ѯICC��״̬
** Parameters:          SDK_ICC_PARAM *pstIccParam
                               s32 siTimerCnt
** Returned value:
** Created By:		���ٲ�  2013.07.12
** Remarks:             //sxl20140408
*****************************************************************************/
s32 sdkIccGetCardStatus(SDK_ICC_PARAM *pstIccParam, s32 siTimerCnt) {

    s32 soltNo = sdkIcGetIcdealCardNo();
    s32 rslt;
    u32 TimerID;

    if (pstIccParam == NULL || siTimerCnt < 0) {
        return SDK_PARA_ERR;
    }
    TimerID = sdkTimerGetId(); //�������볬ʱ

    while (1) {
        rslt = ddi_iccpsam_get_status((u32) soltNo);
        if (rslt == 2 || rslt == 3) //�п�
        {

            pstIccParam->ucCardMode = SDK_ICC_ICC;
            pstIccParam->ucSlotNo = (u8) soltNo; //����IC���Ŀ��ۺ��ڵײ��ȡ

            pstIccParam->eCardType = SDK_ICC_CPU;//liyang add. 2015-8-16

            return SDK_OK;
        }

        if (0 == siTimerCnt || sdkTimerIsEnd(TimerID, siTimerCnt)) {
            return SDK_ICC_NOCARD;
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:��Ҫ�ȸ�pIccParam->CardMode,pIccParam->CardType��pIccParam->SlotNo����������ֵ,Ȼ����Ϊ�������
                                u8 *pheOutBuf:���ص�������Ϣ
                                s32 *psiOutLen:���ص����ݳ���
** Returned value:	SDK_OK:�ɹ�
                                SDK_ERR:ʧ��
                                SDK_TIME_OUT:��ʱ
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccResetIcc(const SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf, s32 *psiOutLen) {
    s32 rslt = 0;
    u8 cardno = 0;

    if ((pstIccParam == NULL) || (pheOutBuf == NULL) || (psiOutLen == NULL)) {
        return SDK_PARA_ERR;
    }
    Trace("emv", "sdkIccResetIcc pstIccParam->ucCardMode = %d\r\n", pstIccParam->ucCardMode);

    if (SDK_ICC_ICC == pstIccParam->ucCardMode)                                         //�Ӵ�ʽ
    {
        cardno = pstIccParam->ucSlotNo; //PSAM����Ҫ����������ȥ
        rslt = sdk_dev_iccard_reset(pstIccParam->eCardType, &cardno, pheOutBuf,
                                    SDK_ICC_TIMER_RESET);
    }
    else if (SDK_ICC_RF == pstIccParam->ucCardMode)                                    //�ǽ�
    {
        rslt = ddi_rf_activate();
        Trace("emv", "ddi_rf_activate rslt = %d\r\n", rslt);

        if (DDI_OK == rslt) {
            rslt = SDK_OK;
        }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
            else if( DDI_READED_MAGCARD == rslt ) //�ǽӹ��̲������̶����ſ�����
        {
            return RF_INTERFRENCE_MAG;
        }
#endif /* if 0 */
        else {
            rslt = SDK_ERR;
        }
    }

    if (rslt > 0) {
        //zcl modify
        if (SDK_ICC_ICC == pstIccParam->ucCardMode) {
            *psiOutLen = rslt;
        }
        else {
            *psiOutLen = 0;
        }
        return SDK_OK;
    }
    else {
        return rslt;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkIccSendIccCardData
   ��������:	����IC������
   �������:
                    pIccParam:��Ҫ�ȸ�pIccParam->CardMode,pIccParam->CardType��pIccParam->SlotNo����������ֵ,Ȼ����Ϊ�������
                    pInBuf:��������,
                    InLen:���ͳ���,
   �������:
                    pOutBuf:��������(Memory/Mifare����Ӧ��ԭ������)
                    puiOutLen:�������ݵĳ���
   ��   ��  ֵ:
                    SDK_OK ���ճɹ�
                    SDK_TIME_OUT ���ճ�ʱ
                    SDK_ERR ���մ���
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD �޿�
   �޸ı�ע:
   ����ʱ��:2012.07.12 09:28:14  //sxl20140408
*******************************************************************/
s32 sdkIccSendIccCardData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheInBuf, s32 siInLen,
                          u8 *pheOutBuf, s32 *psiOutLen) {
    s32 rslt = 0;

    if ((NULL == pstIccParam) || (NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) ||
        siInLen < 0) {
        return SDK_PARA_ERR;
    }

    if (pstIccParam->ucCardMode & SDK_ICC_RF) //�ǽ�
    {
        s32 size = 256; //����ﶨ��Ϊ256��ĳ������´���һ��������

        TraceHex("emv", "rf apdu send", pheInBuf, siInLen);
        rslt = ddi_rf_exchange_apdu(pheInBuf, siInLen, pheOutBuf, psiOutLen, size);
        Trace("emv","ddi_rf_exchange_apdu return %d len %d",rslt,*psiOutLen);
        if (rslt == DDI_OK) {
            TraceHex("emv", "rf apdu rev", pheOutBuf, *psiOutLen);
            return SDK_OK;
        }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
            else if( DDI_READED_MAGCARD == rslt ) //�ǽӹ��̲������̶����ſ�����
        {
            return RF_INTERFRENCE_MAG;
        }
#endif /* if 0 */
        else if (rslt == DDI_EINVAL) {
            return SDK_PARA_ERR;
        }
        else if (rslt == DDI_ETIMEOUT) {
            return SDK_TIME_OUT;
        }
        else {
            return SDK_ERR;
        }
    }
    else if (pstIccParam->ucCardMode & SDK_ICC_ICC) //�Ӵ�
    {
        rslt = sdk_dev_iccard_command(pstIccParam->ucSlotNo, pheInBuf, siInLen, pheOutBuf,
                                      SDK_ICC_TIMER_AUTO, pstIccParam->eCardType);

        if (rslt > 0) {
            *psiOutLen = rslt;
            return SDK_OK;
        }
        else {
            return rslt;
        }
    }
    return SDK_ERR;
}

/*******************************************************************
   ��	  ��: �ս���
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ����IC��
   ��ڲ���: 1.iccParam:��Ҫ�ȸ�pIccParam->CardMode,pIccParam->CardType��pIccParam->SlotNo����������ֵ,Ȼ����Ϊ�������
                  2.pApduSend:���͵�����
                  3.pApduResp:���յ�����
   �� �� ֵ: SDK_OK:�ɹ�
                  SDK_ERR:ʧ��
                  SDK_TIME_OUT:��ʱ
                  SDK_ESC:��ȡ����
                  SDK_PARA_ERR
   ��	  ע:  //sxl20140331  //sxl?20140331 �ǽӵ�ʱ�����IC������δ����
                                              ��ʱ������
 ********************************************************************/
s32 sdkIccDealCardData(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_APDU_SEND *pstApduSend,
                       SDK_ICC_APDU_RESP *pstApduResp) {
    u8 iccommand[300];
    s32 len, sendLc, sendLe, OutLen = 0, templen = 0;
    s32 rslt;

    if ((NULL == pstIccParam) || (NULL == pstApduSend) || (NULL == pstApduResp)) {
        return SDK_PARA_ERR;
    }
    memset(iccommand, 0, sizeof(iccommand));

    len = 0;
    memcpy(&iccommand[len], pstApduSend->Command, 4);
    len = len + 4;
    sendLc = pstApduSend->Lc & 0x00ff;

    if (sendLc > 0)                     // �з�������
    {
        iccommand[len++] = (u8) sendLc;

#if 0 /*Modify by ���ƽ at 2015.01.23  10:54 */

        if(sendLc > 260)
        {
            sendLc = 260;
        }
#endif /* if 0 */
        memcpy(&iccommand[len], pstApduSend->DataIn, sendLc);

        len += sendLc;

        sendLe = pstApduSend->Le;

        if (sendLe > 0)                     // �з�������
        {
            sendLe &= 0x00ff;
            iccommand[len++] = (u8) sendLe;
        }
    }
    else                                                        // �޷�������
    {
        sendLe = pstApduSend->Le;

        if (sendLe > 0)                     // �з�������
        {
            sendLe &= 0x00ff;
            iccommand[len + sendLc] = (u8) sendLe;
            len++;
        }
        else                                                    // �޷�������
        {
            iccommand[len + sendLc] = 0;
            len++;
        }
    }
    memset(pstApduResp, 0, sizeof(SDK_ICC_APDU_RESP));

#ifdef XGD_SDK_DEBUG

    if(pstIccParam->ucCardMode & SDK_ICC_ICC)
    {
        TraceHex("emv", "EMVsend", iccommand, (u16)(len));
    }
    else
    {
        TraceHex("emv", "�ǽ�EMVsend 2", iccommand, (u16)(len));
    }
#endif

    sdkKbKeyFlush();

    while (1) {
        rslt = sdkIccSendIccCardData(pstIccParam, iccommand, len, pstApduResp->DataOut, &OutLen);

        if (rslt != SDK_OK) {
            Trace("emv", "sdkIccSendIccCardData rslt = %d\r\n", rslt);
            pstApduResp->ReadCardDataOk = COMMANDERR;
            return rslt;
        }
        TraceHex("emv", "EMVRx", pstApduResp->DataOut, OutLen);

        if (OutLen < 2 && (pstIccParam->ucCardMode &
                           SDK_ICC_ICC))     //̷����˵2���ֽ�ʱ����һ���ֽ��ǿ����ţ��ڶ����ֽ��Ǵ��󷵻���
        {
            SDK_ICC_PARAM tempIccParam;

            if (sdkIccGetCardStatus(&tempIccParam, 100) != SDK_OK)     //��⿨�Ƿ�γ�
            {
                pstApduResp->ReadCardDataOk = COMMANDERR;
                return SDK_ICC_NOCARD;
            }
        }
        templen = OutLen;

        if ((pstIccParam->ucCardMode & SDK_ICC_RF || pstIccParam->ucCardMode & SDK_ICC_ICC)) {
            pstApduResp->ReadCardDataOk = 1;

            if (templen > 2) {
                pstApduResp->LenOut = (u16)(templen - 2);
                pstApduResp->SWA = pstApduResp->DataOut[templen - 2];
                pstApduResp->SWB = pstApduResp->DataOut[templen - 1];
                pstApduResp->DataOut[templen - 2] = 0;
                pstApduResp->DataOut[templen - 1] = 0;
            }
            else if (templen == 2) {
                pstApduResp->LenOut = 0;
                pstApduResp->SWA = pstApduResp->DataOut[templen - 2];
                pstApduResp->SWB = pstApduResp->DataOut[templen - 1];
                pstApduResp->DataOut[templen - 2] = 0;
                pstApduResp->DataOut[templen - 1] = 0;
            }
            else {
                pstApduResp->ReadCardDataOk = COMMANDERR;
                return SDK_ERR;
            }
            return SDK_OK;
        }

        if (pstApduSend->EnableCancel)                                                             // ������ȡ��������
        {
            if (sdkKbGetKey() == SDK_KEY_ESC) {
                pstApduResp->ReadCardDataOk = COMMANDERR;
                return SDK_ESC;
            }
        }
        // ���г�ʱҲ�Ƿ���COMMANDERR   //sxl
    }
}

/*******************************************************************
   ��	  ��: �ս���
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡICCģ��汾��
   ��ڲ���: 1.pasVersion:��ȡ��ICCģ��汾��
   �� �� ֵ: ����:�汾���ֽڳ���
                    SDK_PARA_ERR:��������
   ��	  ע: \0��β����Ч�ַ��̶�����Ϊ17bytes
 ********************************************************************/
s32 sdkIccGetVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    s32 rtn = ddi_iccpsam_ioctl(DDI_ICCPSAM_CTL_VER, 0, (u32) pasVer);

    if (DDI_OK == rtn) {
        return strlen(pasVer);
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	        check whether  the RF module is exist
** Parameters:          u32 TimeOut:over time
                                u8 *pOutBuf:return data
** Returned value:	SDK_PARA_ERR
                                SDK_TIME_OUT
                                >0:length of returned data
** Created By:		sujianzhong2012.08.06
** Remarks:
*****************************************************************************/
s32 sdkIccIsInRfModule(s32 siTimeOut) {
    if (siTimeOut <= 0) {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucInBuf:
                                s32 *puiOutBuf:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
** Created By:		sujianzhong2012.08.06
** Remarks:  //sxl20140408
*****************************************************************************/
s32 sdkIccEmvToSdkRlt(u8 ucInBuf, s32 *puiOutBuf) {
    u32 i;
    u32 num;

    if (puiOutBuf == NULL) {
        return SDK_PARA_ERR;
    }
    num = sizeof(giSdkEmv2SdkRlt) / sizeof(giSdkEmv2SdkRlt[0]);
    Trace("emv", "*****num = %d, ucInBuf = %d\r\n", num, ucInBuf);

    for (i = 0; i < num; i++) {
        if ((s32) ucInBuf == giSdkEmv2SdkRlt[i][0]) {
            *puiOutBuf = giSdkEmv2SdkRlt[i][1];
            Trace("emv", "**********puiOutBuf = %d\r\n", *puiOutBuf);
            return SDK_OK;
        }
    }

    *puiOutBuf = SDK_ERR;
    Trace("emv", "**********puiOutBuf = %d\r\n", *puiOutBuf);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        get Card's data
** Parameters:          Input: None
** Returned value:
** Created By:		sujianzhong2012.08.06
** Remarks:   //sxl20140408
*****************************************************************************/
void sdkIccGetMagCardData()  //sxl?20140331����SDK��������Ҫ��Ӧ������һ��
{
    u16 i, j;
    u8 temp[128];
    EMVTAGCVLITEM *item = NULL;
    u8 tagexistflag;

    item = dllemvb_avl_gettagitempointer(TAG_Track2Equ);

    if (item != NULL) {
        if (item->len) {
            for (i = item->len - 1; i > 1; i--) {
                if (item->data[i] != 0xff) {
                    break;
                }
            }

            j = i + 1;

            if ((item->data[j - 1] & 0x0f) == 0x0f) {
                i = (u16)(j * 2 - 1);
            }
            else {
                i = (u16)(j * 2);
            }

            if (i > 37) {
                i = 37;                                                            // ��ֹ������� zjz20101201
            }
            gstSdkMagCardData.Track2Len = (u8) i;
            memcpy(gstSdkMagCardData.Track2Data, item->data, (i + 1) / 2);
        }
    }
    item = dllemvb_avl_gettagitempointer(TAG_AppExpireDate);

    if (item != NULL) {
        if (item->len >= 2) {
            gstSdkMagCardData.HaveExpired = true;
            memcpy(gstSdkMagCardData.ExpiredDate, item->data, 2);
        }
    }
    //item = dllemvb_avl_gettagitempointerandexiststatus(TAG_Track2Equ, &tagexistflag);
    dllemvb_avl_gettagitempointerandexiststatus(TAG_Track2Equ, &tagexistflag);
//xiaox 15/11/24
#ifndef SPI_DDI
	tagexistflag = false;
#endif
    if (tagexistflag) {
        memset(temp, 0, sizeof(temp));
        gstSdkMagCardData.HaveICC = sdkIccGetMagCardNo(&gstSdkMagCardData,
                                                       temp);  //sxl?20140331����ṹ����read record��ʱ��Ҫ����
        gstSdkMagCardData.HandLen = (u8) strlen(temp);
        memcpy(gstSdkMagCardData.HandData, temp, gstSdkMagCardData.HandLen);
        gstSdkMagCardData.HandInput = true;

        Trace("emv", "IN GPO RETURN 2TRACKEqu gstSdkMagCardData.HandLen = %d\r\n",
              gstSdkMagCardData.HandLen);
        TraceHex("emv", "IN GPO RETURN 2TRACKEqu", gstSdkMagCardData.HandData,
                 gstSdkMagCardData.HandLen);
    }
    else {
        memset(temp, 0, sizeof(temp));
        sdkIccGetIccMainAcc(temp);
        sdkBcdToU16(&i, temp, 1);
        gstSdkMagCardData.HandInput = true;
        gstSdkMagCardData.HandLen = (u8) i;
        sdkBcdToAsc(gstSdkMagCardData.HandData, &temp[1], (gstSdkMagCardData.HandLen + 1) / 2);
        gstSdkMagCardData.HandData[gstSdkMagCardData.HandLen] = 0;

        if (dllemvb_avl_checkiftagexist(TAG_PAN) == 0) {
            gstSdkMagCardData.HandInput = 0;
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pasOutPAN:Account Number
** Returned value:	SDK_ERR
                                SDK_PARA_ERR
                                SDK_OK
** Created By:		sujianzhong2012.08.06
** Remarks:    //sxl20140408
*****************************************************************************/
s32 sdkIccGetPAN(u8 *pasOutPAN) {
    u8 temp[128] = {0};
    u8 bcPAN[128] = {0};
//    u32 PANLen = 0, rlt;
    u32 i, j, len;
    EMVTAGCVLITEM *item = NULL, *panitem = NULL;
    u8 tagexistflag, pantagexistflag;


    if (NULL == pasOutPAN) {
        return SDK_PARA_ERR;
    }
    panitem = dllemvb_avl_gettagitempointerandexiststatus(TAG_PAN, &pantagexistflag);
    item = dllemvb_avl_gettagitempointerandexiststatus(TAG_Track2Equ, &tagexistflag);

    if (tagexistflag) {
        for (i = item->len - 1; i > 1; i--) {
            if (item->data[i] != 0xff) { break; }
        }

        j = i + 1;

        if ((item->data[j - 1] & 0x0f) == 0x0f) {
            i = j * 2 - 1;
        }
        else { i = j * 2; }

        if (i > 37) {
            i = 37;                //��ֹ������� zjz20101201
        }
        gstSdkMagCardData.Track2Len = (u8) i;
        memcpy(gstSdkMagCardData.Track2Data, item->data, (i + 1) / 2);

        if (gstSdkMagCardData.Track2Len)        //ˢ��
        {
            len = (gstSdkMagCardData.Track2Len + 1) / 2;

            if (len > 19) { len = 19; }
            memset(bcPAN, 0, sizeof(bcPAN));
            sdkBcdToAsc(temp, gstSdkMagCardData.Track2Data, len);

            len = strlen(temp);

            for (i = 0; i < len; i++) {
                if (temp[i] == 'D') {
                    break;
                }
            }

            len = i;

            if (len > 19) { len = 19; }
            memcpy(pasOutPAN, temp, len);
            return SDK_OK;
        }
    }
    else if (pantagexistflag) {
        memset(temp, 0, sizeof(temp));
        sdkBcdToAsc(temp, panitem->data, panitem->len);

        if ((panitem->data[panitem->len - 1] & 0x0F) == 0x0F) {
            temp[panitem->len * 2 - 1] = '\0';
        }
        memcpy(pasOutPAN, temp, strlen(temp));
        return SDK_OK;
    }
    return SDK_ERR;
}

typedef void *(*core_malloc)(size_t size);

typedef void (*core_free)(void *block);


typedef struct {
    core_malloc EMVBallocator_malloc;
    core_free EMVBallocator_free;
} EMVBallocator;


static void *sdkEmvMalloc(u32 nSize) {
    return sdkDebugGetMem(nSize, (u8 *) __FILE__, (s32) __LINE__);
}

static void sdkEmvFree(void *ap) {
    sdkFreeMem(ap);
}

//	��ʼ��emv���ڴ���亯��
void sdkInitEMV() {
    EMVBallocator mallocEmv;

    mallocEmv.EMVBallocator_free = (core_free) sdkEmvFree;
    mallocEmv.EMVBallocator_malloc = (core_malloc) sdkEmvMalloc;

    emvb_allocator_init(&mallocEmv);
}

/*****************************************************************************
** Descriptions:
** Parameters:
** Returned value:
** Created By:		sujianzhong2012.07.31
** Remarks:  //sxl20140408
*****************************************************************************/
s32 sdkIccPowerStartInit(void)   //Ӧ��������ʱ������������s
{
    Trace("emv", "---------------goto->sdkIccPowerStartInit-----------------\r\n");
/*=======BEGIN: ���ٲ� 2013.07.22  16:50 add===========*/
//    sdk_dev_iccard_open();
//    sdk_dev_cl_open();
//    sdkIccOpenMag();
    //sdkIccInitReadCard();
    //sdk_dev_cl_query_open();
/*====================== END======================== */

    sdkInitEMV();

    //CRYPTO_set_mem_functions(emvb_malloc,dev_realloc,emvb_free);  //sxl?


    sdkIccLoadAIDList();
    sdkIccLoadCAPKList();
    sdkIccPowerStartReader();
//        Verify(sdkIccGetEmvCardNo() >= 0);//���ٲ� 2013.07.22 15:26������
    sdkIccReadEmvTransLog();
//    sdkIccSetReaderParam();
    return SDK_OK;
}

/*=======BEGIN: sujianzhong 2013.03.08  9:14 modify===========*/
/*****************************************************************************
** Descriptions:   ֻ�����ļ�����ͬ������������
** Parameters:     void
** Returned value:
** Created By:     sujianzhong20130308
** Remarks:   //sxl20140408
*****************************************************************************/
static void SaveAIDFile(void) {
#if 0
    u8 *pData, fn[256];

    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
//    strcpy(fn, gucSdkCurAppDir);
    strcat(fn, "aid");
    TraceHex("emv", "aid path", fn, strlen(fn));
    pData = (u8 *)sdkGetMem(sizeof(gstSdkAidParam) + 2);   //sxl20140110
    memset(pData, 0, sizeof(gstSdkAidParam) + 2);
    pData[0] = gucSdkAidParamNum;
    memcpy(&pData[1], (u8 *)gstSdkAidParam, sizeof(gstSdkAidParam));
#if 0 //del by sujianzhong 2013.10.14
    Verify(sdkSaveOverlap(fn, pData, sizeof(gstSdkAidParam) + 1) >= 0);     //sjz
#else
    sdkWriteFile(fn, pData, sizeof(gstSdkAidParam) + 1);     //sjz
#endif
    sdkFreeMem(pData);
#endif
}

/*****************************************************************************
** Descriptions:   ֻ�����ļ�����ͬ������������
** Parameters:     void
** Returned value:
** Created By:     sujianzhong20130308
** Remarks:   sxl20140408
*****************************************************************************/
static void SaveCAPKFile(void) {
#if 0
    u8 *pData, fn[256];

    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
//    strcpy(fn, gucSdkCurAppDir);
    strcat(fn, "capk");
    TraceHex("emv", "capk path", fn, strlen(fn));

    pData = (u8 *)sdkGetMem(sizeof(gstSdkTermCAPK) + 2);   //sxl20140110
    memset(pData, 0, sizeof(gstSdkTermCAPK) + 2);
    pData[0] = gucSdkTermCAPKNum;                                                                     //��Կ����
    memcpy(&pData[1], (u8 *)gstSdkTermCAPK, sizeof(gstSdkTermCAPK));       //��Կ

#if 0 //del by sujianzhong 2013.10.14
    Verify(sdkSaveOverlap(fn, pData, sizeof(gstSdkTermCAPK) + 1) >= 0);     //sjz
#else
    sdkWriteFile(fn, pData, sizeof(gstSdkTermCAPK) + 1);     //sjz
#endif
    sdkFreeMem(pData);
#endif
}

/*====================== END======================== */
/*****************************************************************************
** Descriptions:	        delete one aid
** Parameters:          u8 *pheInAID:aid data
                                u32 uiAIDLen:aid data's len
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA: no data
** Created By:		sujianzhong2012.08.06
** Remarks:   //sxl20140408
*****************************************************************************/
s32 sdkIccDelOneAID(const u8 *pheInAID, s32 siAIDLen) {
    u8 i;
    u8 *tmp;
    u32 addr;
    u8 fn[SDK_EMV_MAX_PATH];
    s32 ret;

    Trace("emv", "goto->sdkIccDelOneAID\r\n");

    if (pheInAID == NULL || siAIDLen < 0)     //ʯ���� 2012.08.20 12:27
    {
        return SDK_PARA_ERR;
    }
    memset(fn, 0, SDK_EMV_MAX_PATH);

    TraceHex("emv", "pheInAID", pheInAID, siAIDLen);

    sdkSysGetCurAppDir(fn);  //sxl?����ļ����ƴ�С
    strcat(fn, "newaid");    //sxl20140408

    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE && gAidIndex[i].AIDLen == siAIDLen) {
            if (!memcmp(gAidIndex[i].AID, pheInAID, siAIDLen)) {
#if 0 /*Modify by ���ƽ at 2015.01.26  16:20 */
                EmvSysIni_STRUCT *aid;

                if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                                  || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
                {       //sujianzhong 2013.03.08 9:11
                    aid = (EmvSysIni_STRUCT *)emvb_malloc(sizeof(EmvSysIni_STRUCT));
                    aid->AidLen = gAidIndex[i].AIDLen;
                    memcpy(aid->Aid, gAidIndex[i].AID, aid->AidLen);

                    sdkIccDelOneAidToReader(aid);
                    emvb_free(aid);
                }
#endif /* if 0 */
                tmp = (u8 *) emvb_malloc(AIDLEN);
                memset(tmp, 0xFF, AIDLEN);
                addr = i;
                addr = addr << AIDSHIFTNUM;
                ret = sdkInsertFile(fn, tmp, addr,
                                    AIDLEN);              //sxl??  �����汸���ļ���û������
                emvb_free(tmp);

                if (ret == SDK_FILE_OK) {
#ifdef SXL_DEBUG
                    uart_printf("\r\ndelete AID suceess\r\n");
#endif
                    memset(&gAidIndex[i], 0xFF, sizeof(AIDDATAINDEX));
                    return SDK_OK;
                }
                else {
#ifdef SXL_DEBUG
                    uart_printf("\r\ndelete AID fail\r\n");
#endif
                    return SDK_ERR;
                }
            }
        }
    }

    return SDK_ICC_NODATA;
}

int kern_create_file(const char *filename, s32 len, u8 initialvalue);

/*****************************************************************************
** Descriptions:	        delete all AIDs
** Parameters:          void:
** Returned value:
** Created By:		sujianzhong2012.08.06
** Remarks:             //sxl20140408
*****************************************************************************/
void sdkIccDelAllAIDLists(void) {
    u8 i;
    u8 haveaid = 0;
    //u8 *tmp;
    //u32 addr;
    u8 fn[SDK_EMV_MAX_PATH];

    //s32 ret;


    Trace("emv", "goto->sdkIccDelAllAIDLists\r\n");


    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");    //sxl20140331

    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            haveaid = 1;
            break;
        }
    }

    if (haveaid == 0) {
        return;
    }
    sdkDelFile(fn);
    memset(&gAidIndex[0], 0xFF, sizeof(AIDDATAINDEX) * MAXAIDINDEX);
    kern_create_file(fn, (u32) MAXAIDINDEX * AIDLEN, 0xff);

#if 0 /*Modify by ���ƽ at 2015.01.26  16:29 */

    if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                      || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
    {         //sujianzhong 2013.03.08 9:12
        sdkIccDelAllAidToReader();         //G101ADD zjz20110620
    }
#endif /* if 0 */
    return;
}

/*****************************************************************************
** Descriptions:	        add any group of AIDs
** Parameters:          EmvSysIni_STRUCT *pstInAIDList:
                                u32 uiAIDNum:
** Returned value:	SDK_PARA_ERR
                                SDK_ICC_OVER_LIMIT: group count exceeds the limit
                                SDK_OK
** Created By:		sujianzhong2012.08.06
** Remarks:             //sxl20140408
*****************************************************************************/
s32 sdkIccAddAnyAIDList(const SDK_ICC_EmvSysIni_STRUCT *pstInAIDList,
                        s32 siAIDNum)  //sxl?�ṹ����Ҫ����
{
    u8 i, j; //, k = 0xff;
    //u8 flag = false;
    s32 ret = SDK_OK, ret1;
    u8 iffind = 0;
    u8 *tmp;
    s32 tmplen;
    u32 addr;
    u8 fn[SDK_EMV_MAX_PATH];
    u8 checksum;


    Trace("emv", "%s goto->sdkIccAddAnyAIDList\r\n",__FUNCTION__);


    if (pstInAIDList == NULL || siAIDNum > 100 || siAIDNum <= 0) {
        return SDK_PARA_ERR;
    }
    Trace("emv", "siAIDNum = %d\r\n", siAIDNum);


    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");    //sxl20140331

        Trace("emv", "aid fn = %s\r\n", fn);
    tmp = (u8 *) emvb_malloc(AIDLEN);
    for (j = 0; j < siAIDNum; j++) {
        iffind = 0;


        if ((pstInAIDList + j)->AidLen > 16) {
            emvb_free(tmp);
            Trace("emv", "Aid len err\r\n");
            return SDK_PARA_ERR;
        }
            for (i = 0; i < MAXAIDINDEX; i++)
        {
            //add kernel id,trade type value
            if ((gAidIndex[i].ifusedflag == AIDENABLE) &&
                (gAidIndex[i].AIDLen == (pstInAIDList + j)->AidLen) &&
                (!memcmp(gAidIndex[i].AID, (pstInAIDList + j)->Aid, (pstInAIDList + j)->AidLen))) {
                    iffind = 1;
                //compare aid data
                addr = ((u32) i) << AIDSHIFTNUM;
                tmplen = AIDLEN;

                    ret1 = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret1 != SDK_FILE_OK) {
                    break;
                }
                checksum = sdkCalcCrc16(tmp, AIDLEN - 1);

                if (checksum == tmp[AIDLEN - 1]) {
                    //memcpy((u8 *)&cmpaid,tmp,sizeof(EmvSysIni_STRUCT));
                    if (memcmp((u8 *) tmp, (pstInAIDList + j), sizeof(EmvSysIni_STRUCT)) == 0) {
                        iffind = 2;
                        break;
                    }
                }
                break;
            }
        }
        if (iffind == 0) {

            for (i = 0; i < MAXAIDINDEX; i++) {
                if (gAidIndex[i].ifusedflag != AIDENABLE) {
#ifdef SXL_DEBUG
                    uart_printf("\r\ngAidIndex[i].ifusedflag = %d\r\n", i);
#endif
                    break;
                }
            }
        }
        else if (iffind == 2) {
            continue;
        }

        if (i >= MAXAIDINDEX) {
            ret = SDK_ICC_OVER_LIMIT;
            break;
        }
        //save aid
        memset(tmp, 0xff, AIDLEN);
        memcpy(tmp, (pstInAIDList + j), sizeof(EmvSysIni_STRUCT));
        tmp[AIDFLAGOFFSET] = AIDENABLE;
        tmp[AIDLEN - 1] = sdkCalcCrc16(tmp, AIDLEN - 1);
        addr = i;
        addr = addr << AIDSHIFTNUM;
            Trace("emv","addr = %d\r\n",addr);
        ret1 = sdkInsertFile(fn, tmp, addr, AIDLEN);

        if (ret1 == SDK_FILE_OK) {
#if 0
            gAidIndex[i].tradetypevalue = (pstInAIDList + j)->tradetypevalue;
            gAidIndex[i].appID = (pstInAIDList + j)->appID;                     //modified by HHL:20110220 night
            gAidIndex[i].KernelID = gAidIndex[i].appID;
#endif
            //memcpy(&(gAidIndex[i].AIDLen), &((pstInAIDList + j)->AidLen), 18);
            gAidIndex[i].AIDLen = pstInAIDList[j].AidLen;
            memcpy((gAidIndex[i].AID), ((pstInAIDList + j)->Aid), 16);
            gAidIndex[i].ASI = pstInAIDList[j].Asi;
            gAidIndex[i].ifusedflag = AIDENABLE;
        }
        else {
            ret = SDK_ERR;
            break;
        }
    }


    if (ret == SDK_OK) {
#if 0 /*Modify by ���ƽ at 2015.01.26  16:29 */

        if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                          || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
        {
            //sujianzhong 2013.03.08 9:19

            if(siAIDNum == 1)                 //Ӧ�ò��ǲ���������ӵ�
            {
                sdkIccAddOneAidToReader((EmvSysIni_STRUCT*)pstInAIDList);
            }
            else     //Ӧ�ò����һ�����������е�aid
            {
                sdkIccAddAllAidToReader();
            }
        }
#endif /* if 0 */
        SaveAIDFile();     //ֻ�����ļ�����ͬ��aid����Ϊaid�Ѿ�������ͬ����//sujianzhong 2013.03.08 9:19
    }
    emvb_free(tmp);

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u32 uiStartIndex:get uiAIDNum AIDs start at uiStartIndex
                                u32 uiAIDNum:
                                EmvSysIni_STRUCT *pstOutAIDList:
                                u32 *puiOutAIDNum:the actual number of getting
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:     //sxl20140408   Ӧ��һ����ʾ���
*****************************************************************************/
s32 sdkIccGetAnyAIDList(s32 siStartIndex, s32 siAIDNum, SDK_ICC_EmvSysIni_STRUCT *pstOutAIDList,
                        s32 *psiOutAIDNum) {
    u32 i;
    u8 j;
    s32 ret;
    s32 addr, tmplen;
    u8 fn[SDK_EMV_MAX_PATH];
    u8 *tmp;

    *psiOutAIDNum = 0;

    if (siAIDNum == 0) {
        return SDK_OK;
    }
    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");

    Trace("emv", "goto->sdkIccGetAnyAIDList\r\n");

    j = 0;
    tmp = (u8 *) emvb_malloc(AIDLEN);

    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            if (j >= siStartIndex) {
                addr = ((u32) i) << AIDSHIFTNUM;
                tmplen = AIDLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret != SDK_FILE_OK) {
                    //���¶�ȡһ��AID����
                    sdkIccLoadAIDList();
                    break;
                }
                memcpy(&((pstOutAIDList + j - siStartIndex)->AidLen), tmp,
                       sizeof(SDK_ICC_EmvSysIni_STRUCT));
                j++;

                if (siStartIndex + siAIDNum == j) {
                    break;
                }
            }
            else {
                j++;
            }
        }
    }


    if (j >= siStartIndex) {
        *psiOutAIDNum = j - siStartIndex;
    }
    emvb_free(tmp);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u32 *puiInAIDListNum:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:               sxl20140408
*****************************************************************************/
s32 sdkIccGetAIDListNum(s32 *psiInAIDListNum) {
    u8 i, j;


    Trace("emv", "goto->sdkIccGetAIDListNum\r\n");

    if (psiInAIDListNum == NULL) {
        return SDK_PARA_ERR;
    }
    //*psiInAIDListNum = gucSdkAidParamNum;
    j = 0;

    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            j++;
        }
    }

    *psiInAIDListNum = j;
    Trace("emv", "goto->sdkIccGetAIDListNum = %d\r\n", j);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pheInAID:
                                u32 uiAIDLen:
                                EmvSysIni_STRUCT *pstOutAID:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:              sxl20140408
*****************************************************************************/
s32 sdkIccGetMatchAID(const u8 *pheInAID, s32 siAIDLen, SDK_ICC_EmvSysIni_STRUCT *pstOutAID) {
    u8 i;
    u8 fn[SDK_EMV_MAX_PATH];
    s32 addr;
    s32 tmplen;
    s32 ret;
    u8 checksum;
    u8 *tmp;

    Trace("emv", "goto->sdkIccGetMatchAID\r\n");

    if (pheInAID == NULL || pstOutAID == NULL || siAIDLen < 0 || siAIDLen > 16) {
        return SDK_PARA_ERR;
    }
    TraceHex("emv", "pheInAID", pheInAID, siAIDLen);


    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");    //sxl20140331

    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            if ((gAidIndex[i].AIDLen == siAIDLen) &&
                (!memcmp(gAidIndex[i].AID, pheInAID, siAIDLen))) {
                tmp = (u8 *) emvb_malloc(AIDLEN);
                addr = ((u32) i) << AIDSHIFTNUM;
                tmplen = AIDLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret == SDK_FILE_OK) {
                    checksum = sdkCalcCrc16(tmp, AIDLEN - 1);

                    if (checksum == tmp[AIDLEN - 1]) {
                        memcpy(pstOutAID, tmp, sizeof(SDK_ICC_EmvSysIni_STRUCT));
                        TraceHex("emv", "pstOutAID->Aid", pstOutAID->Aid, pstOutAID->AidLen);
                        ret = SDK_OK;
                    }
                    else {
                        ret = SDK_ERR;
                    }
                }
                else {
                    ret = SDK_ERR;
                }
                emvb_free(tmp);
                return ret;
            }
        }
    }

    return SDK_ICC_NODATA;
}

//sxl20140408
int kern_create_file(const char *filename, s32 len, u8 initialvalue) {
    s32 i;
    s32 ret = -1;
    u8 *tempbuf;
    //u32 pos;
    u32 buflen;
    FILE *fp;

    Trace("emv", "\r\nkern_create_file:%s\r\n", filename);
    //create file
    buflen = 512;

    ret = 0;

    fp = fopen((u8 *) filename, "wb+");

    if (fp == NULL) {
        return SDK_FILE_NONE;
    }

    if (len > 0) {
        tempbuf = (u8 *) sdkGetMem(buflen);
        memset(tempbuf, initialvalue, buflen);

        for (i = 0; i < len; i += (s32) buflen) {
            //pos = i;
            if ((len - i) > (s32) buflen) {
                //ret = sdkInsertFile((u8 *)filename, tempbuf, i, buflen);
                ret = fwrite(tempbuf, buflen, 1, fp);

                if (ret != 1) {
                    ret = -1;
                }
                else {
                    ret = 0;
                }
            }
            else {
                //ret = sdkInsertFile((u8 *)filename, tempbuf, i, len - i);
                ret = fwrite(tempbuf, len - i, 1, fp);

                if (ret != 1) {
                    ret = -1;
                }
                else {
                    ret = 0;
                }
            }

            if (ret < 0) {
                Trace("emv", "The file '%s' write fail!\r\n", filename);
                break;
            }
        }

        sdkFreeMem(tempbuf);
    }
    fclose(fp);
    return ret;
}

/*******************************************************************
   Author:   sxl
   Function Name:   void manage_aidindexread(void)
   Function Purpose:read aid index
   Input Paramters:
   Output Paramters:N/A
   Return Value:      N/A
   Remark: N/A
   Modify: N/A  1021  //sxl20140408
 ********************************************************************/
void manage_aidindexread(const u8 *aidfiledir) {
    s32 ret;
    u8 i;
    //u8 tmpreaddata[BlockLen];
    //u32 readnum;
    u32 tmpaddr;
    u32 readlen;
    u8 *tmp;           //[BlockLen]
    u32 addr;
    u8 checksum;
    u32 filelen;
    EmvSysIni_STRUCT *termaidparam;

#ifdef EMVB_DEBUG
    u8 j;
#endif

    memset(&gAidIndex[0], 0xFF, sizeof(AIDDATAINDEX) * MAXAIDINDEX);

    if (sdkGetFileSize(aidfiledir) != (u32) MAXAIDINDEX * AIDLEN) {
        ret = kern_create_file(aidfiledir, (u32) MAXAIDINDEX * AIDLEN, 0xff);

        if (ret != 0) {
            return;
        }
    }
    filelen = (u32) MAXAIDINDEX * AIDLEN;
    addr = 0;
    //tmpnum = BlockLen/AIDLEN;

    tmp = (u8 *) emvb_malloc(EMVBlockLen);


    for (i = 0; i < MAXAIDINDEX;) {
        if ((addr + EMVBlockLen) <= filelen) {
            readlen = EMVBlockLen;
        }
        else {
            readlen = filelen - addr;
        }
        ret = sdkReadFile(aidfiledir, tmp, addr, &readlen);

        if (ret != SDK_FILE_OK) {
            emvb_free(tmp);
            return;
        }

        //memset(tmp,0xff,sizeof(tmp));
        for (tmpaddr = 0; tmpaddr < readlen;) {
            checksum = sdkCalcCrc16(&tmp[tmpaddr], AIDLEN - 1);

            if (checksum == tmp[tmpaddr + AIDLEN - 1]) {
                termaidparam = (EmvSysIni_STRUCT * ) & tmp[tmpaddr];
                gAidIndex[i].tradetypevalue = termaidparam->tradetypevalue;
                gAidIndex[i].AIDLen = termaidparam->AidLen;
                memcpy(&(gAidIndex[i].AID[0]), termaidparam->Aid, 16);
                gAidIndex[i].ASI = termaidparam->Asi;
                gAidIndex[i].appID = termaidparam->appID;                               //HHL: 20110227 357 is the offset add of appID
                gAidIndex[i].ifusedflag = tmp[AIDFLAGOFFSET + tmpaddr];

#ifdef SXL_DEBUG
                uart_printf("\r\ngAidIndex[i].ifusedflag = %d %d\r\n", (addr + tmpaddr) >> 9, gAidIndex[i].ifusedflag);
#endif
                //termaidparam = (EmvSysIni_STRUCT *)tmp;


                gAidIndex[i].KernelID = gAidIndex[i].appID;
#ifdef EMVB_DEBUG
                uart_printf("\r\ngAidIndex[i].KernelID = %d %02x\r\n", gAidIndex[i].KernelID, gAidIndex[i].tradetypevalue);

                for(j = 0; j < gAidIndex[i].AIDLen; j++)
                {
                    uart_printf("%02x ", gAidIndex[i].AID[j]);
                }

                uart_printf("\r\n");
#endif
            }
            i++;

            if (i >= MAXAIDINDEX) {
                break;
            }
            tmpaddr += AIDLEN;
        }

        addr += readlen;
    }

    emvb_free(tmp);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:	SDK_OK
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:     //sxl20140408
*****************************************************************************/
s32 sdkIccLoadAIDList(void) {
    //u8 *pData;
    u8 fn[SDK_EMV_MAX_PATH];

    //s32 len; //sujianzhong 2013.10.14 15:58


    Trace("emv", "goto->sdkIccLoadAIDList\r\n");
    memset(fn, 0, SDK_EMV_MAX_PATH);

    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");    //sxl20140331
    Trace("emv", "aid fn:%s", fn);


    manage_aidindexread(fn);

    Trace("emv", "sdkIccLoadAIDList ret = %d\r\n", SDK_OK);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        �����ļ���ͬʱͬ�����е�aid����������
** Parameters:          void:
** Returned value:
** Created By:		sujianzhong2012.08.06
** Remarks:   //sxl20140331    //sxl?�ǽӶ�����û�д���
*****************************************************************************/
void sdkIccSaveAIDFile(void) {
    SaveAIDFile();     //sujianzhong 2013.03.08 14:45

    //����stm32ʱ������Ϊ���÷ǽӲŽ���aidͬ��
    //����stm32ʱ������Ϊ���÷ǽӲŽ���aidͬ��
#if 0

    if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                      || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
    {
        if((gucSdkAidParamNum != 0) && sdkIccAddAllAidToReader() == 0)     //ͬ��������AID //sjz 20120712
        {
            sdkIccDealSyncQreaderAidErr();     //ͬ��������ʧ��ֱ�ӽ��ն��ϵ�Ҳ���
        }
    }
#endif
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pheInRID:
                                u32 uiCAPKIndex:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccDelOneCAPK(const u8 *pheInRID, s32 siCAPKIndex) {
    u8 i;
    u8 *tmp;
    s32 ret;
    s32 addr;
    u8 fn[SDK_EMV_MAX_PATH];

    Trace("emv", "goto->sdkIccDelOneCAPK\r\n");

    if (pheInRID == NULL || siCAPKIndex < 0) {
        return SDK_PARA_ERR;
    }
    TraceHex("emv", "pheInRID", pheInRID, 5);
    Trace("emv", "siCAPKIndex=%x\r\n", siCAPKIndex);

    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");      //sxl20140331



    for (i = 0; i < MAXCAPKINDEX; i++) {
        if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
            if (!memcmp(gCapkIndex[i].RID, pheInRID, 5) && gCapkIndex[i].CAPKI == siCAPKIndex) {
                tmp = (u8 *) emvb_malloc(CAPKLEN);
                memset(tmp, 0xFF, CAPKLEN);
                addr = i;
                addr = addr << CAPKSHIFTNUM;
                ret = sdkInsertFile(fn, tmp, addr, CAPKLEN);
                emvb_free(tmp);

                if (ret == SDK_FILE_OK) {
#if 0 /*Modify by ���ƽ at 2015.01.26  16:29 */
                    memset(&gCapkIndex[i], 0xFF, sizeof(CAPKDATAINDEX));

                    if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                                      || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
                    {             //sujianzhong 2013.03.08 9:38
                                  //sdkIccDelOneCapkToReader(&gstSdkTermCAPK[i]);  //sxl?�������Ļ�û�д���
                    }
#endif /* if 0 */
                    return SDK_OK;
                }
                else {
                    return SDK_ERR;
                }
            }
        }
    }

    SaveCAPKFile();
    return SDK_ICC_NODATA;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:
** Created By:		sujianzhong2012.08.06
** Remarks:              //sxl20140408
*****************************************************************************/
void sdkIccDelAllCAPKLists(void) {
    u8 caplexistflag = 0;
    u8 i;
    //u8 *tmp;
    //s32 addr;
    //s32 ret;
    u8 fn[SDK_EMV_MAX_PATH];

    Trace("emv", "goto->sdkIccDelAllCAPKLists\r\n");

    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");

    for (i = 0; i < MAXCAPKINDEX; i++) {
        if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
            caplexistflag = 1;
            break;
        }
    }

    if (caplexistflag == 0) {
        return;
    }
    sdkDelFile(fn);
    memset(&gCapkIndex[0], 0xFF, MAXCAPKINDEX * sizeof(CAPKDATAINDEX));
    kern_create_file(fn, (u32) MAXCAPKINDEX * CAPKLEN, 0xff);

#if 0 /*Modify by ���ƽ at 2015.01.26  16:29 */

    if(caplexistflag)
    {
        if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                          || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
        {         //sujianzhong 2013.03.08 9:38
                  //sdkIccDelAllCapkToReader();                       //G101ADD zjz20110620  //sxl?�ǽӶ������Ļ�û�д���
        }
    }
#endif /* if 0 */
}

/*****************************************************************************
** Descriptions:	        this function is used for verify whether hash check sum of a capk is ok or not.
** Parameters:          const CAPK_STRUCT* capk:
** Returned value:	SDK_OK: CAPK verify ok
                                SDK_ERR: CAPK verify fail
** Created By:		sujianzhong 2012.08.17
** Remarks:             //sxl20140408
*****************************************************************************/
s32 sdkIccVerifyCAPK(const SDK_ICC_CAPK_STRUCT *capk) {
    u8 checkSum[20];
    u32 uiIndex;
    s32 retcode;
    u8 *checkData = NULL;

    s32 checkData_size = 5 + 1 + capk->ModulLen + capk->ExponentLen;

    checkData = (u8 *) sdkGetMem(checkData_size);

    if (NULL == checkData) {
        Assert(0);
        return SDK_ERR;
    }
    memset(checkData, 0, checkData_size);

    memcpy(checkData, capk->RID, 5);
    uiIndex = 5;
    checkData[5] = capk->CAPKI;
    uiIndex += 1;

    memcpy((u8 * ) & checkData[uiIndex], capk->Modul, capk->ModulLen);
    uiIndex += capk->ModulLen;

    memcpy((u8 * ) & checkData[uiIndex], capk->Exponent, capk->ExponentLen);
    uiIndex += capk->ExponentLen;


    Hash(checkData, uiIndex, checkSum);

    if (memcmp(capk->CheckSum, checkSum, 20)) {
        retcode = SDK_ERR;
    }
    else {
        retcode = SDK_OK;
    }
    Trace("emv", "sdkIccVerifyCAPK retcode = %d\r\n", retcode);

    sdkFreeMem(checkData);

    return retcode;
}

//zcl add
static void Privite_SDK_CAPK_TO_CAPK(CAPK_STRUCT *tempcapk,
                                     const SDK_ICC_CAPK_STRUCT *pstInCAPKList) {
    memcpy(tempcapk->RID, pstInCAPKList->RID, 5);
    tempcapk->CAPKI = pstInCAPKList->CAPKI;
    tempcapk->SMCapkFlag = 0;
    tempcapk->HashInd = pstInCAPKList->HashInd;
    tempcapk->ArithInd = pstInCAPKList->ArithInd;
    tempcapk->ModulLen = pstInCAPKList->ModulLen;

    if (tempcapk->ModulLen > sizeof(tempcapk->Modul)) {
        tempcapk->ModulLen = sizeof(tempcapk->Modul);
    }
    memset(tempcapk->Modul, 0, sizeof(tempcapk->Modul));
    memcpy(tempcapk->Modul, pstInCAPKList->Modul, tempcapk->ModulLen);
    tempcapk->ExponentLen = pstInCAPKList->ExponentLen;

    if (tempcapk->ExponentLen > sizeof(tempcapk->Exponent)) {
        tempcapk->ExponentLen = sizeof(tempcapk->Exponent);
    }
    memset(tempcapk->Exponent, 0, sizeof(tempcapk->Exponent));
    memcpy(tempcapk->Exponent, pstInCAPKList->Exponent, tempcapk->ExponentLen);
    tempcapk->CheckSumLen = pstInCAPKList->CheckSumLen;

    if (tempcapk->CheckSumLen > sizeof(tempcapk->CheckSum)) {
        tempcapk->CheckSumLen = sizeof(tempcapk->CheckSum);
    }
    memset(tempcapk->CheckSum, 0, sizeof(tempcapk->CheckSum));
    memcpy(tempcapk->CheckSum, pstInCAPKList->CheckSum, tempcapk->CheckSumLen);
    memcpy(tempcapk->ExpireDate, pstInCAPKList->ExpireDate, sizeof(tempcapk->ExpireDate));
#if 0           //del by zcl 20150318������
    tempcapk->ParamInd = pstInCAPKList->ParamInd;
#endif
    tempcapk->DGTLSGNTRlen = 0;
    memset(tempcapk->DGTLSGNTR, 0, sizeof(tempcapk->DGTLSGNTR));
}

static void Privite_CAPK_TO_SDK_CAPK(SDK_ICC_CAPK_STRUCT *pstInCAPKList,
                                     const CAPK_STRUCT *tempcapk) {
    memcpy(pstInCAPKList->RID, tempcapk->RID, 5);
    pstInCAPKList->CAPKI = tempcapk->CAPKI;
    pstInCAPKList->HashInd = tempcapk->HashInd;
    pstInCAPKList->ArithInd = tempcapk->ArithInd;
    pstInCAPKList->ModulLen = tempcapk->ModulLen;

    if (pstInCAPKList->ModulLen > sizeof(pstInCAPKList->Modul)) {
        pstInCAPKList->ModulLen = sizeof(pstInCAPKList->Modul);
    }
    memset(pstInCAPKList->Modul, 0, sizeof(pstInCAPKList->Modul));
    memcpy(pstInCAPKList->Modul, tempcapk->Modul, pstInCAPKList->ModulLen);
    pstInCAPKList->ExponentLen = tempcapk->ExponentLen;

    if (pstInCAPKList->ExponentLen > sizeof(pstInCAPKList->Exponent)) {
        pstInCAPKList->ExponentLen = sizeof(pstInCAPKList->Exponent);
    }
    memset(pstInCAPKList->Exponent, 0, sizeof(pstInCAPKList->Exponent));
    memcpy(pstInCAPKList->Exponent, tempcapk->Exponent, pstInCAPKList->ExponentLen);
    pstInCAPKList->CheckSumLen = tempcapk->CheckSumLen;

    if (pstInCAPKList->CheckSumLen > sizeof(pstInCAPKList->CheckSum)) {
        pstInCAPKList->CheckSumLen = sizeof(pstInCAPKList->CheckSum);
    }
    memset(pstInCAPKList->CheckSum, 0, sizeof(pstInCAPKList->CheckSum));
    memcpy(pstInCAPKList->CheckSum, tempcapk->CheckSum, pstInCAPKList->CheckSumLen);
    memcpy(pstInCAPKList->ExpireDate, tempcapk->ExpireDate, sizeof(pstInCAPKList->ExpireDate));
#if 0           //del by zcl 20150318������
    pstInCAPKList->ParamInd = tempcapk->ParamInd;
#endif
}

static bool Privite_CAPK_IS_EQUAL(const SDK_ICC_CAPK_STRUCT *pstInCAPKList,
                                  const CAPK_STRUCT *tempcapk) {
    if (!memcmp(pstInCAPKList->RID, tempcapk->RID, 5) &&
        pstInCAPKList->CAPKI == tempcapk->CAPKI &&
        pstInCAPKList->HashInd == tempcapk->HashInd &&
        pstInCAPKList->ArithInd == tempcapk->ArithInd &&
        pstInCAPKList->ModulLen == tempcapk->ModulLen &&
        !memcmp(pstInCAPKList->Modul, tempcapk->Modul, pstInCAPKList->ModulLen) &&
        pstInCAPKList->ExponentLen == tempcapk->ExponentLen &&
        !memcmp(pstInCAPKList->Exponent, tempcapk->Exponent, pstInCAPKList->ExponentLen) &&
        pstInCAPKList->CheckSumLen == tempcapk->CheckSumLen &&
        !memcmp(pstInCAPKList->CheckSum, tempcapk->CheckSum, pstInCAPKList->CheckSumLen) &&
        !memcmp(pstInCAPKList->ExpireDate, tempcapk->ExpireDate, sizeof(pstInCAPKList->ExpireDate))
#if 0  //del by zcl 20150318������
        &&
        pstInCAPKList->ParamInd == tempcapk->ParamInd
#endif
            ) {
        return true;
    }
    else {
        return false;
    }
}

//end


/*****************************************************************************
** Descriptions:
** Parameters:          CAPK_STRUCT *pstInCAPKList:
                                u32 uiCAPKNum:
** Returned value:	SDK_OK: CAPK add ok
                                SDK_PARA_ERR
                                SDK_ICC_OVER_LIMIT
                                SDK_ERR: CAPK verify fail
** Created By:		sujianzhong2012.08.06
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccAddAnyCAPKList(const SDK_ICC_CAPK_STRUCT *pstInCAPKList, s32 siCAPKNum) {
    u8 i, j;
    //u8 flag = false;
    s32 ret = SDK_OK, ret1;
    u8 fn[SDK_EMV_MAX_PATH];
    u8 *tmp;
    u8 iffind;
    s32 addr;
    s32 tmplen;
    u8 checksum;

    Trace("emv", "goto->sdkIccAddAnyCAPKList\r\n");

    if (pstInCAPKList == NULL || siCAPKNum > 100 || siCAPKNum <= 0) {
        return SDK_PARA_ERR;
    }
    Trace("emv", "siCAPKNum = %d\r\n", siCAPKNum);

    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");    //sxl20140331
        Trace("emv", "capk fn = %s\r\n", fn);

    tmp = (u8 *) emvb_malloc(CAPKLEN);

    for (j = 0; j < siCAPKNum; j++) {
        if (sdkIccVerifyCAPK((pstInCAPKList + j)) != SDK_OK) {
            emvb_free(tmp);
            return SDK_ERR;
        }
        iffind = 0;

        for (i = 0; i < MAXCAPKINDEX; i++) {
            //add kernel id,trade type value
            if ((gCapkIndex[i].ifusedflag == CAPKENABLE) &&
                (gCapkIndex[i].CAPKI == (pstInCAPKList + j)->CAPKI) &&
                (!memcmp(gCapkIndex[i].RID, (pstInCAPKList + j)->RID, 5))) {
                iffind = 1;
                //compare aid data
                addr = ((u32) i) << CAPKSHIFTNUM;
                tmplen = CAPKLEN;
                ret1 = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret1 != SDK_FILE_OK) {
                    break;
                }
                checksum = sdkCalcCrc16(tmp, CAPKLEN - 1);

                if (checksum == tmp[CAPKLEN - 1]) {
                    //zcl modify
                    if (Privite_CAPK_IS_EQUAL(pstInCAPKList + j, (CAPK_STRUCT *) tmp)) {
                        iffind = 2;
                        break;
                    }
                }
                break;
            }
        }

        if (iffind == 0) {
            for (i = 0; i < MAXCAPKINDEX; i++) {
                if (gCapkIndex[i].ifusedflag != CAPKENABLE) {
#ifdef SXL_DEBUG
                    uart_printf("\r\ngAidIndex[i].ifusedflag = %d\r\n", i);
#endif
                    break;
                }
            }
        }
        else if (iffind == 2) {
            continue;
        }

        if (i >= MAXCAPKINDEX) {
            ret = SDK_ICC_OVER_LIMIT;
            break;
        }
        //save aid
        memset(tmp, 0xff, CAPKLEN);
        //memcpy(tmp, (pstInCAPKList + j), sizeof(SDK_ICC_CAPK_STRUCT));
        //zcl modify
        Privite_SDK_CAPK_TO_CAPK((CAPK_STRUCT *) tmp, (pstInCAPKList + j));
        //end
        tmp[CAPKFLAGOFFSET] = CAPKENABLE;
        tmp[CAPKLEN - 1] = sdkCalcCrc16(tmp, CAPKLEN - 1);
        addr = i;
        addr = addr << CAPKSHIFTNUM;
        ret1 = sdkInsertFile(fn, tmp, addr, CAPKLEN);

        if (ret1 == SDK_FILE_OK) {
            memcpy(gCapkIndex[i].RID, (pstInCAPKList + j)->RID, 5);
            gCapkIndex[i].CAPKI = (pstInCAPKList + j)->CAPKI;
            gCapkIndex[i].ifusedflag = CAPKENABLE;
            gCapkIndex[i].SMCapkFlag = 0;       //zcl add
        }
        else {
            ret = SDK_ERR;
            break;
        }
    }


    //sxl?�ǽӶ�������û�д���
    if (ret == SDK_OK) {
#if 0 /*Modify by ���ƽ at 2015.01.26  16:29 */

        if(gucSdkIccEmvInQreader == 1 && ((gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
                                          || (gucSdkIccQreaderType == QREADER_TYPE_OUT && sdkSysIsRfIn() == 0)))
        {     //sujianzhong 2013.03.08 9:19
            if(siCAPKNum == 1)     //Ӧ�ò��ǲ���������ӵ�
            {
                //sdkIccAddOneCapkToReader((CAPK_STRUCT*)pstInCAPKList);  //�ǽӶ�������û�д���
                sdkIccAddOneCapkToReader((CAPK_STRUCT*)tmp);  //�ǽӶ�������û�д���//zcl modify
            }
            else     //Ӧ�ò����һ�����������е�aid
            {
                sdkIccAddAllCapkToReader();  // �ǽӶ�������û�д���
            }
        }
#endif /* if 0 */
        SaveCAPKFile();     //ֻ�����ļ�����ͬ��capk����Ϊ�����Ѿ�ͬ����//sujianzhong 2013.03.08 9:23
    }
    emvb_free(tmp);

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u32 uiStartIndex:
                                u32 uiCAPKNum:
                                CAPK_STRUCT *pstOutCAPKList:
                                u32 *puiOutCAPKNum:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccGetAnyCAPKList(s32 siStartIndex, s32 siCAPKNum, SDK_ICC_CAPK_STRUCT *pstOutCAPKList,
                         s32 *psiOutCAPKNum) {
    u32 i;
    u8 j;
    s32 ret;
    s32 addr, tmplen;
    u8 fn[SDK_EMV_MAX_PATH];
    u8 *tmp;


    *psiOutCAPKNum = 0;

    if (siCAPKNum == 0) {
        return SDK_OK;
    }
    memset(fn, 0, SDK_EMV_MAX_PATH);

    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");

    Trace("emv", "goto->sdkIccGetAnyCAPKList\r\n");

    j = 0;
    tmp = (u8 *) emvb_malloc(CAPKLEN);

    for (i = 0; i < MAXCAPKINDEX; i++) {
        if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
            if (j >= siStartIndex) {
                addr = ((u32) i) << CAPKSHIFTNUM;
                tmplen = CAPKLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret != SDK_FILE_OK) {
                    //���¶�ȡһ��AID����
                    sdkIccLoadCAPKList();
                    break;
                }
                //memcpy(&((pstOutCAPKList + j - siStartIndex)->RID), tmp, sizeof(SDK_ICC_CAPK_STRUCT));    //zcl modify
                Privite_CAPK_TO_SDK_CAPK(
                        (SDK_ICC_CAPK_STRUCT * ) & ((pstOutCAPKList + j - siStartIndex)->RID),
                        (CAPK_STRUCT *) tmp); //zcl modify

                j++;

                if (siStartIndex + siCAPKNum == j) {
                    break;
                }
            }
            else {
                j++;
            }
        }
    }


    if (j >= siStartIndex) {
        *psiOutCAPKNum = j - siStartIndex;
    }
    emvb_free(tmp);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u32 *puiInCAPKListNum:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccGetCAPKListNum(s32 *psiInCAPKListNum) {
    u8 i, j = 0;


    Trace("emv", "goto->sdkIccGetCAPKListNum\r\n");

    if (psiInCAPKListNum == NULL) {
        return SDK_PARA_ERR;
    }

    for (i = 0; i < MAXCAPKINDEX; i++) {
        if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
            j++;
        }
    }

    *psiInCAPKListNum = j;


#if 0
    *psiInCAPKListNum = gucSdkTermCAPKNum;
#endif


    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pheInRID:
                                u32 uiCAPKIndex:
                                CAPK_STRUCT *pstOutCAPK:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccGetMatchCAPK(const u8 *pheInRID, s32 siCAPKIndex, SDK_ICC_CAPK_STRUCT *pstOutCAPK) {
    u8 i;
    u8 fn[SDK_EMV_MAX_PATH];
    s32 addr;
    s32 tmplen;
    s32 ret;
    u8 checksum;
    u8 *tmp;

    Trace("emv", "goto->sdkIccGetMatchCAPK\r\n");

    if (pheInRID == NULL || pstOutCAPK == NULL || siCAPKIndex < 0) {
        return SDK_PARA_ERR;
    }
    Trace("emv", "siCAPKIndex=%x\r\n", siCAPKIndex);
    TraceHex("emv", "pheInRID", pheInRID, 5);

    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");    //sxl20140331


    for (i = 0; i < MAXCAPKINDEX; i++) {
        if ((gCapkIndex[i].ifusedflag == CAPKENABLE) && (!memcmp(pheInRID, gCapkIndex[i].RID, 5))
            && (siCAPKIndex == gCapkIndex[i].CAPKI)) {
            tmp = (u8 *) emvb_malloc(CAPKLEN);
            addr = ((u32) i) << CAPKSHIFTNUM;
            tmplen = CAPKLEN;
            ret = sdkReadFile(fn, tmp, addr, &tmplen);

            if (ret == SDK_FILE_OK) {
                checksum = sdkCalcCrc16(tmp, CAPKLEN - 1);

                if (checksum == tmp[CAPKLEN - 1]) {
                    //memcpy(pstOutCAPK, tmp, sizeof(SDK_ICC_CAPK_STRUCT));                         //����CAPK  //zcl modify
                    Privite_CAPK_TO_SDK_CAPK(pstOutCAPK, (CAPK_STRUCT *) tmp); //zcl modify
                    TraceHex("emv", "pstOutCAPK->RID", pstOutCAPK->RID, 5);
                    Trace("emv", "pstOutCAPK->CAPKI=%x\r\n", pstOutCAPK->CAPKI);
                    ret = SDK_OK;
                }
                else {
                    ret = SDK_ERR;
                }
            }
            else {
                ret = SDK_ERR;
            }
            emvb_free(tmp);
            return ret;
        }
    }

    return SDK_ICC_NODATA;
}

/*******************************************************************
   Author:   sxl
   Function Name:   void manage_capkindexread(void)
   Function Purpose:read CAPK index
   Input Paramters:
   Output Paramters:N/A
   Return Value:      N/A
   Remark: N/A
   Modify: N/A  1021   20140408
 ********************************************************************/
void manage_capkindexread(const u8 *capkfiledir) {
    s32 ret;
    u8 i;
    //u8 tmpreaddata[BlockLen];
    //u32 readnum;
    u32 tmpaddr;
    u32 readlen;
    u8 *tmp;       //[BlockLen]
    u32 addr;
    u8 checksum;
    u32 filelen;


    memset(&gCapkIndex[0], 0xFF, MAXCAPKINDEX * sizeof(CAPKDATAINDEX));


    if (sdkGetFileSize(capkfiledir) != (u32) MAXCAPKINDEX * CAPKLEN) {
        ret = kern_create_file(capkfiledir, (u32) MAXCAPKINDEX * CAPKLEN, 0xff);

        if (ret != 0) {
            return;
        }
    }
    filelen = (u32) MAXCAPKINDEX * CAPKLEN;
    addr = 0;
    //tmpnum = BlockLen/AIDLEN;

    tmp = (u8 *) emvb_malloc(EMVBlockLen);


    for (i = 0; i < MAXCAPKINDEX;) {
        if ((addr + EMVBlockLen) <= filelen) {
            readlen = EMVBlockLen;
        }
        else {
            readlen = filelen - addr;
        }
        ret = sdkReadFile(capkfiledir, tmp, addr, &readlen);

        if (ret != SDK_FILE_OK) {
            emvb_free(tmp);
            return;
        }

        //memset(tmp,0xff,sizeof(tmp));
        for (tmpaddr = 0; tmpaddr < readlen;) {
            checksum = sdkCalcCrc16(&tmp[tmpaddr], CAPKLEN - 1);

            if (checksum == tmp[tmpaddr + CAPKLEN - 1]) {
                memcpy(&gCapkIndex[i].RID, &tmp[tmpaddr], 7);
                gCapkIndex[i].ifusedflag = tmp[CAPKFLAGOFFSET + tmpaddr];
            }
            i++;

            if (i >= MAXCAPKINDEX) {
                break;
            }
            tmpaddr += CAPKLEN;
        }

        addr += readlen;
    }

    emvb_free(tmp);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:	SDK_OK
                                SDK_ICC_NODATA
** Created By:		sujianzhong2012.08.06
** Remarks:            //sxl20140408
*****************************************************************************/
s32 sdkIccLoadCAPKList(void) {
    //u8  *pData;
    //s32 ret;
    //s32 len; //sujianzhong 2013.10.14 15:59
    u8 fn[SDK_EMV_MAX_PATH];

    Trace("emv", "goto->sdkIccLoadCAPKList\r\n");
    memset(fn, 0, SDK_EMV_MAX_PATH);
    sdkSysGetCurAppDir(fn);
//    strcpy(fn, gucSdkCurAppDir);
    strcat(fn, "newcapk");      //sxl20140331
    Trace("emv", "capk fn:%s\r\n", fn);

    manage_capkindexread(fn);
    Trace("emv", "sdkIccLoadCAPKList ret = %d\r\n", SDK_OK);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:
** Created By:		sujianzhong2012.08.06
** Remarks:
*****************************************************************************/
void sdkIccSaveCAPKFile(void) {
    //s32 tempSdkTermCAPKNum;


    SaveCAPKFile();

//    if(gEmvInQreader)//sujianzhong 2013.01.28 9:17
//����stm32ʱ������Ϊ���÷ǽӲŽ���capkͬ��
//����stm32ʱ������Ϊ���÷ǽӲŽ���capkͬ��
#if 0 /*Modify by ���ƽ at 2015.01.26  16:30 */

    if((gucSdkIccEmvInQreader == 1 && gucSdkIccQreaderType == QREADER_TYPE_IN && sdkSysIsRfIn() == 1)
       || (gucSdkIccEmvInQreader == 1 && sdkSysIsRfIn() == 0))
    {
        sdkIccGetCAPKListNum(&tempSdkTermCAPKNum);

        if((tempSdkTermCAPKNum != 0) && sdkIccAddAllCapkToReader() == 0)        //ͬ����������Կ//sjz 20120712
        {
            sdkIccDealSyncQreaderCapkErr();   //sxl ���������Ȳ�����     //ͬ��������ʧ��ֱ�ӽ��ն��ϵ�Ҳ���
        }
    }
#endif /* if 0 */
}

//sxl20140331   //sxl?��Ҫȷ��������ʹ���������
s32 sdkIccCfgEmvParam(const SDK_ICC_CFG_EMVPARAM *pstCfgEmvParam) {
    u32 MerchNameLocateLen;

    if (pstCfgEmvParam == NULL) {
        return SDK_PARA_ERR;
    }
    dllemvb_avl_createsettagvalue(TAG_TermID, pstCfgEmvParam->asTerminalNo, 8);
    dllemvb_avl_createsettagvalue(TAG_MerchCateCode, pstCfgEmvParam->heMerchCateCode, 2);
    dllemvb_avl_createsettagvalue(TAG_MerchID, pstCfgEmvParam->asMerchantNo, 15);

    MerchNameLocateLen = sdkTruncateChar(pstCfgEmvParam->asMerchantName, 128);

    if (MerchNameLocateLen > 128) {
        MerchNameLocateLen = 128;
    }
    dllemvb_avl_createsettagvalue(TAG_paywavemerchantname, pstCfgEmvParam->asMerchantName,
                                  MerchNameLocateLen);
    dllemvb_avl_createsettagvalue(TAG_CountryCode, pstCfgEmvParam->bcCountryCode, 2);
    dllemvb_avl_createsettagvalue(TAG_TransCurcyCode, pstCfgEmvParam->bcTransCurcyCode, 2);
    dllemvb_avl_set(TAG_TransCurcyExp, (u8) pstCfgEmvParam->ucTransCurcyExp);
    dllemvb_avl_createsettagvalue(TAG_TermTransPredicable, pstCfgEmvParam->heTermTransPredicable,
                                  4);
    dllemvb_avl_set(TAG_TransTypeValue, (u8) pstCfgEmvParam->ucTransTypeValue);
    dllemvb_avl_createsettagvalue(TAG_TermCapab, pstCfgEmvParam->heTermCapab, 3);
    dllemvb_avl_createsettagvalue(TAG_TermAddCapab, pstCfgEmvParam->heTermAddCapab, 5);
    dllemvb_avl_set(TAG_TermType, (u8) pstCfgEmvParam->eTermType);

    if (gEMVTradeParam != NULL) {
        gEMVTradeParam->TransType = (u8) pstCfgEmvParam->eTransType;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        config trans-datas of the tag
** Parameters:          const u8 *pheTagLenValue:data in format of TagLenValue
** Returned value:	     SDK_OK
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:          //sxl20140408   ��������Ѿ����ó�����TAG����������,�鿴һ����������������ʹ��
                                                  ���������Ҫ�������������

*****************************************************************************/
s32 sdkIccConfigTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen)     //sjz 20120726
{
    //u8 i;
    u32 tag_len;
    u8 tag[4];

    if (NULL == pheTag || pheTag[0] == 0xFF || pheTag[0] == 0 || pheInData == NULL || siInLen < 0) {
        return SDK_PARA_ERR;
    }
    tag_len = 1;

    if ((pheTag[0] & 0x1F) == 0x1F) {
        tag_len = 2;
    }
    memset(tag, 0, sizeof(tag));
    memcpy(tag, pheTag, tag_len);

    if (dllemvb_avl_createsettagvalue(tag, pheInData, siInLen)) {
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const u8 *pheTag:
                                u32 uiAvailableLen:
                                u8 *pheOutData:
                                u32 *puiOutLen:
** Returned value:	SDK_OK:
                                SDK_PARA_ERR: parameter error
                                SDK_EQU: the card hasn't return the tag
** Created By:		sujianzhong2012.08.06
** Remarks:          //sxl20140408 �鿴��������������ʹ��
                                                 ���һ����Щ�������÷�  �������Ľ��װ���ʱ���ʹ��
*****************************************************************************/
s32 sdkIccReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) {
    u8 tag[4];
    u8 taglen = 0;
    u16 templen;

    //s32 ret = SDK_OK;


    if ((NULL == pheTag) || (NULL == pheOutData) || (NULL == psiOutLen)
        || (0 == pheTag[0]) || (0xFF == pheTag[0])) {
        return SDK_PARA_ERR;
    }
    memset(tag, 0, sizeof(tag));
    taglen = 1;

    if ((pheTag[0] & 0x1F) == 0x1F) {
        taglen = 2;
    }
    memcpy(tag, pheTag, taglen);
    *psiOutLen = 0;
    dllemvb_avl_gettagvalue_all(tag, pheOutData, &templen);

    if (templen) {
#if 0 /*Modify by zcl at 2014.08.07  18:45 */

        if(templen > *psiOutLen)
        {
            templen = *psiOutLen;
        }
#endif /* if 0 */
        *psiOutLen = templen;
        return SDK_OK;
    }
    return 0;
}

/*****************************************************************************
** Descriptions:    send command("GET DATA") to card and get corresponding data
** Parameters:      const SDK_ICC_PARAM *pstIccParam
                    const u8 *pheTag
                    u8 *pheOutData
                    s32 *puiOutLen
** Returned value:
** Created By:      sujianzhong   2012.09.25
** Remarks:     //sxl?ȷ����������������ʹ��
*****************************************************************************/
s32 sdkIccSendGetData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheTag, u8 *pheOutData,
                      s32 *psiOutLen) {
    u8 temp[3] = {0};
    u32 uiIndex; //, uilen, i;
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;

    Trace("emv", "goto->sdkIccSendGetData\r\n");

    if ((NULL == pstIccParam) || (NULL == pheTag) || (NULL == pheOutData) || (NULL == psiOutLen)) {
        return SDK_PARA_ERR;
    }
    uiIndex = 1;

    if ((pheTag[0] & 0x1F) == 0x1F) {
        uiIndex = 2;
    }
    TraceHex("emv", "pheTag", pheTag, uiIndex);
    memset(&apdu_s, 0, sizeof(apdu_s));
    memset(&apdu_r, 0, sizeof(apdu_r));
    memset(temp, 0, sizeof(temp));

    memcpy(apdu_s.Command, "\x80\xCA", 2);
    memcpy(temp, pheTag, uiIndex);
    memcpy((u8 *) apdu_s.Command + 2, temp, 2);
    apdu_s.Lc = 0;
    apdu_s.Le = 256;                                           //Le=0(256)

    if (sdkIccDealCardData(pstIccParam, &apdu_s, &apdu_r) == SDK_ICC_NOCARD) {
        return SDK_ICC_NOCARD;
    }

    if ((apdu_r.ReadCardDataOk == COMMANDERR) ||
        (apdu_r.ReadCardDataOk == COMMTIMEOUT))                                      //����ʧ��
    {
        return SDK_ERR;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) {
        *psiOutLen = apdu_r.DataOut[2];
        memcpy(pheOutData, apdu_r.DataOut + 3, *psiOutLen);
        TraceHex("emv", "pheOutData", pheOutData, *psiOutLen);
        return SDK_OK;
    }
    Trace("emv", "�Ҳ����ñ�ǩ!\r\n");
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pheOnlineRandNum:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:         //sxl20140408
*****************************************************************************/
s32 sdkIccGetOnlineRandNum(u8 *pheOnlineRandNum)     //sjz 20120726
{
    if (NULL == pheOnlineRandNum) {
        return SDK_PARA_ERR;
    }
    *pheOnlineRandNum = dllemvb_avl_gettagvalue(TAG_bShowRandNum);

    return SDK_OK;
}

u8 UnzipResponsePagData(u8 *DataIn, u16 DataLen)  //sxl20140408
{
    u16 id;
    u8 retCode = EMV_OK;
    //u8 bInTable;
    u8 tag[4], taglen;
    u16 len;
    u8 tmp71script[256], tmp72script[256];
    u16 tmp71scriptlen = 0, tmp72scriptlen = 0;
    EMVTAGCVLITEM retrunitem;
    u16 startpos;
    u8 parseret;

    id = 0;

    while (id < DataLen) {
        if (id >= DataLen) {
#ifdef EMVB_DEBUG
            //printf("\r\nindex = %d,DataLen = %d\r\n",index,DataLen);
#endif

            retCode = EMV_ERR;
            break;
        }

        if (DataIn[id] == 0x00) {
            id++;
            continue;
        }
        taglen = 0;
        memset(tag, 0, sizeof(tag));


        startpos = id;


        tag[taglen++] = DataIn[id];

        if ((tag[0] & 0x1F) == 0x1F) {
            tag[taglen++] = DataIn[id + 1];

            if (tag[1] & 0x80) {
                tag[taglen++] = DataIn[id + 2];
            }
        }
        //89      8A      91      71      72



        //bInTable = 0;

        id += taglen;
        parseret = ParseExtLen(DataIn, &id, &len);         //������̨���ص����ݵĳ���ΪTLV��ʽ

        if (parseret != 0) {
            return EMV_ERR;
        }

        if (tag[0] == 0x71 && taglen == 1) {
            if ((tmp71scriptlen + id - startpos + len) < 256) {
                memcpy(&tmp71script[tmp71scriptlen], DataIn + startpos, id - startpos + len);
                tmp71scriptlen += id - startpos + len;
            }
        }
        else if (tag[0] == 0x72 && taglen == 1) {
            if ((tmp72scriptlen + id - startpos + len) < 256) {
                memcpy(&tmp72script[tmp72scriptlen], DataIn + startpos, id - startpos + len);
                tmp72scriptlen += id - startpos + len;
            }
        }
        else if ((tag[0] == 0x89 || tag[0] == 0x8A || tag[0] == 0x91) && taglen == 1) {
            if (EMVB_TagBaseLib_read(tag, taglen, TAGTYPE_EMV, &retrunitem) == EMV_OK) {
                if (len >= retrunitem.minlen && len <= retrunitem.maxlen) {
                    dllemvb_avl_createsettagvalue(tag, DataIn + id, len);
                }
            }
        }
        id += len;
    }

    if (retCode == EMV_OK) {
        if (tmp71scriptlen) {
            dllemvb_avl_createsettagvalue(TAG_SCRIPT1, tmp71script, tmp71scriptlen);
        }

        if (tmp72scriptlen) {
            dllemvb_avl_createsettagvalue(TAG_SCRIPT2, tmp72script, tmp72scriptlen);
        }
    }
    return retCode;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucOnlineResult:SDK_OK:online success;SDK_ERR:online fail
                                const u8 *pheRspCode: issuer's response
                                const u8 *pheIssuerAuthData:
                                u32 uiIssuerAuthDataLen:
                                const u8 *pheScript:
                                u32 uiScriptLen:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
** Created By:		sujianzhong2012.08.08
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccGetOnlineData(u8 ucOnlineResult, const u8 *pheRspCode, const u8 *pheIssuerAuthData,
                        s32 siIssuerAuthDataLen, const u8 *pheScript,
                        s32 siScriptLen)      //sjz 20120726
{
    u8 retCode;

    if ((ucOnlineResult == SDK_OK && pheRspCode == NULL)
        || (siIssuerAuthDataLen != 0 && pheIssuerAuthData == NULL)
        || (siScriptLen != 0 && pheScript == NULL)) {
        return SDK_PARA_ERR;
    }

    if (ucOnlineResult == SDK_OK) {
        if (gEMVTradeParam != NULL) {
            gEMVTradeParam->onlinetradestatus = EMV_OK;
        }
        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *) pheRspCode, 2);
    }
    else {
        if (gEMVTradeParam != NULL) {
            gEMVTradeParam->onlinetradestatus = EMV_ERR;
        }
        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *) "00", 2);
    }

    if (siIssuerAuthDataLen > 0) {
        dllemvb_avl_createsettagvalue(TAG_IssuerAuthenData, pheIssuerAuthData, siIssuerAuthDataLen);
    }

    if (siScriptLen > 0) {
        retCode = UnzipResponsePagData(pheScript, siScriptLen);

        if (retCode != EMV_OK) {
            return SDK_ERR;
        }
    }
    Trace("emv", "onlineresult=%d\r\n", ucOnlineResult);
    TraceHex("emv", "pheRspCode", pheRspCode, 2);
    Trace("emv", "siIssuerAuthDataLen=%d\r\n", siIssuerAuthDataLen);
    TraceHex("emv", "pheIssuerAuthData", pheIssuerAuthData, siIssuerAuthDataLen);
    Trace("emv", "siScriptLen=%d\r\n", siScriptLen);
    TraceHex("emv", "pheScript", pheScript, siScriptLen);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        import online response
** Parameters:          u8 ucOnlineResult:
                                const u8 *pheRspCode:
** Returned value:
** Created By:		sjz modify2012.08.15
** Remarks:               //sxl20140408
*****************************************************************************/
s32 Private_sdkIccImportRespCode(u8 ucOnlineResult,
                                 const u8 *pheRspCode)     //sjz modify 2012.08.15 18:38
{
    if (ucOnlineResult == SDK_OK && pheRspCode == NULL) {
        return SDK_PARA_ERR;
    }

    if (ucOnlineResult == SDK_OK) {
        if (gEMVTradeParam != NULL) {
            gEMVTradeParam->onlinetradestatus = EMV_OK;
        }
        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *) pheRspCode, 2);
    }
    else {
        if (gEMVTradeParam != NULL) {
            gEMVTradeParam->onlinetradestatus = EMV_ERR;
        }
        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *) "00", 2);
    }
    Trace("emv", "onlineresult=%d\r\n", ucOnlineResult);
    TraceHex("emv", "pheRspCode", pheRspCode, 2);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u32 *puiScriptResultLen:
                                u8 *pheScriptResult:the result of deal script(per 5 bytes as a group of script result)
** Returned value:	SDK_PARA_ERR
                    SDK_OK
** Created By:		sujianzhong2012.08.08
** Remarks:                //sxl20140408
*****************************************************************************/
s32 sdkIccGetScriptResult(s32 *psiScriptResultLen, u8 *pheScriptResult)     //sjz 20120726
{
    EMVTAGCVLITEM *item;

    if ((pheScriptResult == NULL) || (psiScriptResultLen == NULL)) {
        return SDK_PARA_ERR;
    }
    *psiScriptResultLen = 0;
    item = dllemvb_avl_gettagitempointer(TAG_SCRIPT71RESULT);

    if (item != NULL) {
        if (item->len) {
            memcpy(&pheScriptResult[*psiScriptResultLen], item->data, item->len);
            *psiScriptResultLen += item->len;
        }
    }
    item = dllemvb_avl_gettagitempointer(TAG_SCRIPT72RESULT);

    if (item != NULL) {
        if (item->len) {
            memcpy(&pheScriptResult[*psiScriptResultLen], item->data, item->len);
            *psiScriptResultLen += item->len;
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pbcFirstCurrencyCode:3 Bytes:1st Byte:exist or not; 2nd~3rd Bytes:Tag9F51
                                u8 *pbcFirstBalance:6 Bytes;Tag9F79
                                u8 *pbcSecondCurrencyCode:3 Bytes:1st Byte:exist or not; 2nd~3rd Bytes:TagDF71
                                u8 *pbcSecondBalance:6 Bytes;TagDF79
** Returned value:	SDK_PARA_ERR
                                SDK_OK
** Created By:		sujianzhong2012.08.08
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccGetECQueryMsg(u8 *pbcFirstCurrencyCode, u8 *pbcFirstBalance, u8 *pbcSecondCurrencyCode,
                        u8 *pbcSecondBalance)     //sjz 20120726
{
    if ((pbcFirstCurrencyCode == NULL) || (pbcFirstBalance == NULL) ||
        (pbcSecondCurrencyCode == NULL)
        || (pbcSecondBalance == NULL)) {
        return SDK_PARA_ERR;
    }
    memcpy(pbcFirstCurrencyCode, &gucSdkEcCurrencyCode[0], 3);
    //memcpy(pbcFirstBalance, gstSdkCardInfo.VLPAvailableFund, 6);
    memset(pbcFirstBalance, 0, 6);
    dllemvb_avl_gettagvalue_spec(TAG_VLPAvailableFund, pbcFirstBalance, 0, 6);
    memcpy(pbcSecondCurrencyCode, &gucSdkEcSecCurrencyCode[0], 3);
    memcpy(pbcSecondBalance, gucSdkCardInfoSecVLPAvailableFund, 6);

    TraceHex("emv", "pbcFirstCurrencyCode", pbcFirstCurrencyCode, 3);
    TraceHex("emv", "pbcFirstBalance", pbcFirstBalance, 6);
    TraceHex("emv", "pbcSecondCurrencyCode", pbcSecondCurrencyCode, 3);
    TraceHex("emv", "pbcSecondBalance", pbcSecondBalance, 6);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
                                u8 ucLogNumIn:the number of log espected to read
                                u8 *pheLogFormat:the format of log
                                APDU_RESP *pstLogData:log data
                                u8 *pheLogNumOut:the number of log actually read
** Returned value:	SDK_PARA_ERR
                                SDK_ERR
                                SDK_NoLogErr:no log
                                SDK_ReadLogEnd:read log over
** Created By:		sujianzhong2012.08.08
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccReadLogDetail(const SDK_ICC_PARAM *pstIccParam, u8 ucLogNumIn, u8 *pheLogFormat,
                        SDK_ICC_APDU_RESP *pstLogData, u8 *pheLogNumOut)      //sjz 20120726
{
    u8 retCode;
    u8 TransRecordSFI, TransRecordNum, aucDOL[100], ucNowNum;
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;
    u8 logtype = 0;

    Trace("emv", "lognumin = %d\r\n", ucLogNumIn);

    if ((ucLogNumIn > 10) || (ucLogNumIn == 0) || (pheLogFormat == NULL) || (pstLogData == NULL) ||
        (pheLogNumOut == NULL)) {
        return SDK_PARA_ERR;
    }
    retCode = sdkIccCountTransRecord(&TransRecordSFI, &TransRecordNum, logtype);
    Trace("emv", "card record numbers = %d,retCode = %d\r\n", TransRecordNum, retCode);

    if (retCode == EMV_OTHER) {
        TransRecordSFI = 0x0B;
        TransRecordNum = 0x0A;
    }
    else if (retCode != EMV_OK) {
        return SDK_ERR;
    }

    if (TransRecordNum == 0) {
        return SDK_NoLogErr;
    }
    sdkIccPostReadLogCommand(&apdu_s, logtype);

    if (sdkIccDealCardData(pstIccParam, &apdu_s, &apdu_r) == SDK_ICC_NOCARD) {
        return SDK_ICC_NOCARD;
    }

    if (apdu_r.ReadCardDataOk == COMMANDERR || apdu_r.ReadCardDataOk == COMMTIMEOUT) {
        apdu_r.ReadCardDataOk = 0;
        return SDK_ERR;
    }
    //sxl  �������ص�����
    retCode = sdkIccDealICCParamRecord(&apdu_r, aucDOL, 100, logtype);

    if (retCode != EMV_OK) {
        return SDK_ERR;
    }

    if (ucLogNumIn > TransRecordNum) {
        ucLogNumIn = TransRecordNum;     //��Ҫ��ȡ�ļ�¼��������ʵ�ʴ��ڵļ�¼��������ʵ�ʼ�¼����Ϊ׼
    }
    ucNowNum = 1;
    (*pheLogNumOut) = 0;
    memset(pheLogFormat, 0, 100);
    memcpy(pheLogFormat, aucDOL, sizeof(aucDOL));

    while ((*pheLogNumOut) < ucLogNumIn) {
        memset(&apdu_s, 0, sizeof(apdu_s));
        memset(&apdu_r, 0, sizeof(apdu_r));

        PostReadReocrd((APDU_SEND * ) & apdu_s, (u8)(TransRecordSFI << 3), ucNowNum);

        if (sdkIccDealCardData(pstIccParam, &apdu_s, &apdu_r) == SDK_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }

        if (apdu_r.ReadCardDataOk == COMMANDERR || apdu_r.ReadCardDataOk == COMMTIMEOUT) {
            apdu_r.ReadCardDataOk = 0;
            return SDK_ERR;
        }

        if (!(apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00))     // ICC parameter Records found fail.
        {
            if ((*pheLogNumOut) == 0) {
                return SDK_ERR;
            }
            else {
                return SDK_ReadLogEnd;
            }
        }
        memcpy(pstLogData + (*pheLogNumOut), &apdu_r, sizeof(apdu_r));
        (*pheLogNumOut)++;
        ucNowNum++;
    }

    Trace("emv", "*pheLogNumOut = %d\r\n", *pheLogNumOut);
    return SDK_ReadLogEnd;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
                                u8 ucLogNumIn:the number of log espected to read
                                u8 *pheLogFormat:the format of log
                                APDU_RESP *pstLogData:log data
                                u8 *pheLogNumOut:the number of log actually read
** Returned value:	SDK_PARA_ERR
                                SDK_ERR
                                SDK_NoLogErr:no log
                                SDK_ReadLogEnd:read log over
** Created By:		sujianzhong2012.08.08
** Remarks:              sxl20140331
*****************************************************************************/
s32 sdkIccReadEcLoadDetail(const SDK_ICC_PARAM *pstIccParam, u8 ucLogNumIn, u8 *pheLogFormat,
                           SDK_ICC_APDU_RESP *pstLogData, u8 *pheLogNumOut)      //sjz 20120726
{
    u8 retCode;
    u8 TransRecordSFI, TransRecordNum, aucDOL[100], ucNowNum;
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;
    u8 logtype = 1;

    Trace("emv", "lognumin = %d\r\n", ucLogNumIn);

    if ((ucLogNumIn > 10) || (ucLogNumIn == 0) || (pheLogFormat == NULL) || (pstLogData == NULL) ||
        (pheLogNumOut == NULL)) {
        return SDK_PARA_ERR;
    }
    retCode = sdkIccCountTransRecord(&TransRecordSFI, &TransRecordNum, logtype);
    Trace("emv", "card record numbers = %d,retCode = %d\r\n", TransRecordNum, retCode);

    if (retCode == EMV_OTHER) {
//        TransRecordSFI = 0x0B;
        TransRecordSFI = 0x0C; //sujianzhong 2013.07.22 10:54
        TransRecordNum = 0x0A;
    }
    else if (retCode != EMV_OK) {
        return SDK_ERR;
    }

    if (TransRecordNum == 0) {
        return SDK_NoLogErr;
    }
    sdkIccPostReadLogCommand(&apdu_s, logtype);

    if (sdkIccDealCardData(pstIccParam, &apdu_s, &apdu_r) == SDK_ICC_NOCARD) {
        return SDK_ICC_NOCARD;
    }

    if (apdu_r.ReadCardDataOk == COMMANDERR || apdu_r.ReadCardDataOk == COMMTIMEOUT) {
        apdu_r.ReadCardDataOk = 0;
        return SDK_ERR;
    }
    //sxl  �������ص�����
    retCode = sdkIccDealICCParamRecord(&apdu_r, aucDOL, 100, logtype);

    if (retCode != EMV_OK) {
        return SDK_ERR;
    }

    if (ucLogNumIn > TransRecordNum) {
        ucLogNumIn = TransRecordNum;     //��Ҫ��ȡ�ļ�¼��������ʵ�ʴ��ڵļ�¼��������ʵ�ʼ�¼����Ϊ׼
    }
    ucNowNum = 1;
    (*pheLogNumOut) = 0;
    memset(pheLogFormat, 0, 100);
    memcpy(pheLogFormat, aucDOL, sizeof(aucDOL));

    while ((*pheLogNumOut) < ucLogNumIn) {
        memset(&apdu_s, 0, sizeof(apdu_s));
        memset(&apdu_r, 0, sizeof(apdu_r));

        PostReadReocrd((APDU_SEND * ) & apdu_s, (u8)(TransRecordSFI << 3), ucNowNum);

        if (sdkIccDealCardData(pstIccParam, &apdu_s, &apdu_r) == SDK_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }

        if (apdu_r.ReadCardDataOk == COMMANDERR || apdu_r.ReadCardDataOk == COMMTIMEOUT) {
            apdu_r.ReadCardDataOk = 0;
            return SDK_ERR;
        }

        if (!(apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00))     // ICC parameter Records found fail.
        {
            if ((*pheLogNumOut) == 0) {
                return SDK_ERR;
            }
            else {
                return SDK_ReadLogEnd;
            }
        }
        memcpy(pstLogData + (*pheLogNumOut), &apdu_r, sizeof(apdu_r));
        (*pheLogNumOut)++;
        ucNowNum++;
    }

    Trace("emv", "*pheLogNumOut = %d\r\n", *pheLogNumOut);
    return SDK_ReadLogEnd;
}

void Emvb_delspectag(void)  //20140408
{
    dllemvb_avl_delete(TAG_qPBOCOfflineSpendAmount);
    dllemvb_avl_delete(TAG_CardHoldIdNo);
    dllemvb_avl_delete(TAG_CardHoldIdType);
    dllemvb_avl_delete(TAG_CardIdentifyInfo);
    dllemvb_avl_delete(TAG_PaywaveCardAuthData);
    dllemvb_avl_delete(TAG_FormFactIndicator);
    dllemvb_avl_delete(TAG_VLPIssuAuthorCode);
    dllemvb_avl_delete(TAG_CustomerExclusiveData);

    //9F 6D�Ĵ���//sxl?20131024  �Ժ���
}

//sxl20140408
void EMVB_TransTermData_Init(void)  //sxl20131024  pinpad����ʱ���ܲ���ʱ��Ĳ�������
{


    unsigned char tmpvalue;
    u8 strBuf[32], transTime[9];
    u8 t[8];
    u16 seed;
    u32 k;


    dllemv_avl_settagtype(TAGTYPE_EMV);

    //Emvb_TermInfoInit_read();   //sxl?�����������Ҫ����




    tmpvalue = 0x00;      //sxl20131024  pinpad����ʱ������ �˻��Ƚ�������
    gEMVTradeParam->TransType = GOODS;
    gEMVTradeParam->bAbleOnline = 1;
    gEMVTradeParam->bReversal = 0;
    gEMVTradeParam->bAdvice = 0;

    gEMVTradeParam->bErrSDATL = 0;
    gEMVTradeParam->bErrAuthData = 0;
    gEMVTradeParam->bCDOL1HasNoUnpredictNum = 0;
    gEMVTradeParam->bCDOL2HasNoUnpredictNum = 0;
    gEMVTradeParam->bDDOLHasNoUnpredictNum = 0;
    gEMVTradeParam->bPrintReceipt = 0;
    gEMVTradeParam->bCombineDDA = 0;
    gEMVTradeParam->bCombineDDAFail = 0;
    gEMVTradeParam->bretforminit = 0;
    gEMVTradeParam->VLPIndicator = 0;
    gEMVTradeParam->bCardConfirmVLP = 0;
    gEMVTradeParam->bretforminit = 0;
    gEMVTradeParam->appblockflag = 0;
    gEMVTradeParam->SelectedAppNo = 0xff;


    dllemvb_avl_createsettagvalue(TAG_CVMResult, "\x3F\x00\x00", 3);


    Verify(sdkGetRtc(t) == SDK_OK);         //sjz

    seed = 0;
    k = t[5];
    seed += (((k & 0xF0) >> 4) * 10 + (k & 0x0F));
    k = t[4];
    seed += ((((k & 0xF0) >> 4) * 10 + (k & 0x0F)) * 60);
    k = t[3];
    seed += (u16)((((k & 0xF0) >> 4) * 10 + (k & 0x0F)) * 3600);
    srand(seed);


    sdkIccCreateUnpredictNum();       //sxl?�鿴ԭ������������ɺ���



    memset(transTime, 0, sizeof(transTime));

    Verify(sdkGetRtc(transTime) == SDK_OK);     //sjz
    dllemvb_avl_createsettagvalue(TAG_TransDate, transTime, 3);
    dllemvb_avl_createsettagvalue(TAG_TransTime, &transTime[3], 3);
	TraceHex("emv","transTime",transTime,6);

    dllemvb_avl_set(TAG_TermType, 0x22);


    dllemvb_avl_set(TAG_TransTypeValue, tmpvalue);


    dllemvb_avl_set(TAG_POSEntryMode, 0x05);


    dllemvb_avl_createsettagvalue(TAG_TVR, "\x00\x00\x00\x00\x00", 5);
    dllemvb_avl_createsettagvalue(TAG_TSI, "\x00\x00", 2);


    //dllemvb_avl_createsettagvalue(TAG_AmtAuthNum,"\x00\x00\x00\x00\x00\x00",6);
    //dllemvb_avl_createsettagvalue(TAG_AmtOtherNum,"\x00\x00\x00\x00\x00\x00",6);

    //dllemvb_avl_createsettagvalue(TAG_AmtAuthBin,"\x00\x00\x00\x00",4);
    //dllemvb_avl_createsettagvalue(TAG_AmtOtherBin,"\x00\x00\x00\x00",4);


    //sxl20131024 ����ʱ������

    if (dllemvb_avl_checkiftagexist(TAG_AppVerNum) == 0) {
        dllemvb_avl_createsettagvalue(TAG_AppVerNum, "\x00\x8C", 2);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TACDenial) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TACDenial, "\x00\x00\x00\x00\x00", 5);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TACDefault) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TACDefault, "\x00\x00\x00\x00\x00", 5);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TACOnline) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TACOnline, "\x00\x00\x00\x00\x00", 5);
    }

    if (dllemvb_avl_checkiftagexist(TAG_CountryCode) == 0) {
        dllemvb_avl_createsettagvalue(TAG_CountryCode, "\x01\x56", 2);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TransCurcyCode) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TransCurcyCode, "\x01\x56", 2);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TransReferCurcyCode) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TransReferCurcyCode, "\x01\x56", 2);
    }
#ifdef COM_DEBUG
    dllemvb_avl_createsettagvalue(TAG_CountryCode, "\x08\x40", 2);
    dllemvb_avl_createsettagvalue(TAG_TransCurcyCode, "\x08\x40", 2);
    dllemvb_avl_createsettagvalue(TAG_TransReferCurcyCode, "\x08\x40", 2);
#endif


    if (dllemvb_avl_checkiftagexist(TAG_TransReferCurcyExp) == 0) {
        dllemvb_avl_set(TAG_TransReferCurcyExp, 0x02);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TransCurcyExp) == 0) {
        dllemvb_avl_set(TAG_TransCurcyExp, 0x02);
    }

    if (dllemvb_avl_checkiftagexist(TAG_AcquireID) == 0) {
        dllemvb_avl_createsettagvalue(TAG_AcquireID, "\x00\x00\x00\x00\x00\x01", 6);
    }

    if (dllemvb_avl_checkiftagexist(TAG_MerchCateCode) == 0) {
        dllemvb_avl_createsettagvalue(TAG_MerchCateCode, "\x00\x01", 2);
    }

    if (dllemvb_avl_checkiftagexist(TAG_MCHIPTransCateCode) == 0) {
        dllemvb_avl_set(TAG_TransCurcyExp, 0x02);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TermCapab) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TermCapab, "\xE0\xB0\xC8", 3);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TermAddCapab) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TermAddCapab, "\x60\x00\xF0\xF0\x01", 5);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TermDDOL) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TermDDOL, "\x9F\x37\x04", 3);
    }

    if (dllemvb_avl_checkiftagexist(TAG_TermTDOL) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TermTDOL,
                                      "\x9F\x02\x06\x5F\x2A\x02\x9A\x03\x9C\x01\x95\x05\x9F\x37\x04",
                                      15);
    }
    gEMVTradeParam->VLPIndicator = 1; //20140619 add

    //sxl?bTermSupportVLP
    if (dllemvb_avl_checkiftagexist(TAG_VLPTransLimit) == 0) {
        dllemvb_avl_createsettagvalue(TAG_VLPTransLimit, "\x00\x00\x00\x00\x40\x00", 6);
    }

    if (dllemvb_avl_checkiftagexist(TAG_MCHIPTransCateCode) == 0) {
        dllemvb_avl_set(TAG_MCHIPTransCateCode, 'R');
    }

    if (dllemvb_avl_checkiftagexist(TAG_TermTransPredicable) == 0) {
        dllemvb_avl_createsettagvalue(TAG_TermTransPredicable, "\x36\x00\x00\x80", 4);
        Trace("emv", "EMVB_TransTermData_Init 9F66\r\n");
    }
    dllemvb_avl_createsettagvalue(TAG_AmtAuthBin, "\x00\x00\x00\x00", 4);
    dllemvb_avl_createsettagvalue(TAG_AmtOtherBin, "\x00\x00\x00\x00", 4);
    dllemvb_avl_createsettagvalue(TAG_AmtAuthNum, "\x00\x00\x00\x00\x00\x00", 6);
    dllemvb_avl_createsettagvalue(TAG_AmtOtherNum, "\x00\x00\x00\x00\x00\x00", 6);


    //ClearCreditPwd();


    memset(&gEmvErrID, 0, sizeof(EMVBERRORID));
    // IFN
    //termID merchID
    //dllemvb_avl_createsettagvalue(TAG_TermID,SimData.TerminalNo,8);
    //dllemvb_avl_createsettagvalue(TAG_MerchID,SimData.MerchantNo,15);


    memset(strBuf, 0, sizeof(strBuf));
    sdkIccReadSN(strBuf);
    dllemvb_avl_createsettagvalue(TAG_IFD_SN, strBuf, 8);    //sxl20131024 pinpad����ʱ������


    //if(strlen(SimData.MerchantCName))
    //{

    //	dllemvb_avl_createsettagvalue(TAG_paywavemerchantname,SimData.MerchantCName,strlen(SimData.MerchantCName));
    //dllemvb_avl_createsettagvalue(TAG_m,SimData.MerchantCName,strlen(SimData.MerchantCName));
    //sxl20131024 pinpad����ʱ������ merchant name locate

    //}


    //gPayData.cardtype = CARDTYPE_EMV;
    //gPayData.readcardmode = CARDMODE_CONTACT;


    Emvb_delspectag();
}

unsigned char qPBOC_checkifcancelcontactlesstrade(void)  //sxl  ��Щָ�����ȡ��
{
    unsigned char retCode = EMV_OK;

#ifdef SUPPORT_DUALINTERFACE       //sxl?�Ƿ�Ҫ���ǽӵ�ʱ�����IC����ſ�
    unsigned char insertcardstatus;
    unsigned char *tempCardData;   //[512]

    //����Ƿ��п����룬�������ӿ�
    if(gPayData.ifreaddatacomplete == 0 && SimData.AppType == PAYWAVEAPP)
    {
        gCardDataLen = 0;
        gCardDetectedMode = 0;
        tempCardData = (unsigned char *)emvb_malloc(512);
        insertcardstatus = Emv_CardReadReportData(tempCardData, &gCardDataLen);     //�п�����

        if(insertcardstatus == 0x76 || insertcardstatus == 0x31)        //��IC�����룬��ˢ�ſ�
        {
            if(insertcardstatus == 0x31)
            {
                if(gCardData == NULL)
                {
                    gCardData = (unsigned char *)emvb_malloc(1024);
                }
                memcpy(gCardData, tempCardData, gCardDataLen);
            }
            gCardDetectedMode = insertcardstatus;
            retCode = PAYWAVE_DETECTEDOTHERMODE;
        }
        emvb_free(tempCardData);
    }
#endif


    if (sdkKbGetKey() == 0x1B)  //sxl? �Ƿ���԰�ȡ����ȡ��
    {
        //EMV_Beep(0);
        sdkSysAudioBeepOk();
        //PAYPASS3.0

        //END
#ifdef EMVB_DEBUG
        printf("\r\nuser cancel\r\n");
#endif
        retCode = qPBOC_RESETCOMMAND;
    }
    return retCode;
}

unsigned char DealqPBOCCardData(unsigned char *tempsenddata, unsigned short tempdatalen,
                                APDU_RESP *apdu_r) {
    s32 templen = 0; //, EMVRxLen = 0;

    //s32 rslt;

    //unsigned short i;

    //i = 0;
    //u32 timeuse;
    //sxl?
    //sxl?�ǽӶ���������û�д���
    //DealqPBOCIcSim(tempsenddata,tempdatalen,EMVRxBuf,&EMVRxLen);
    //rslt = sdkIccSendIccCardData(gPayData.pstIccParam, tempsenddata, tempdatalen, apdu_r->DataOut, &templen);
#ifndef SPI_DDI
//xiaox 15/11/24
	memmove(&tempsenddata[2],tempsenddata,tempdatalen);
	tempsenddata[0] = GlbEMVBApp_UnionStruct->magdatalen;
	tempsenddata[1] = GlbEMVBApp_UnionStruct->magdatapos;
	tempdatalen += 2;
#endif

	sdkIccSendIccCardData(gPayData.pstIccParam, tempsenddata, tempdatalen, apdu_r->DataOut,
                          &templen);


    apdu_r->ReadCardDataOk = 0;
    templen = templen;

    if (templen > 2) {
        apdu_r->LenOut = templen - 2;
        apdu_r->SWA = apdu_r->DataOut[templen - 2];
        apdu_r->SWB = apdu_r->DataOut[templen - 1];
        apdu_r->DataOut[templen - 2] = 0;
        apdu_r->DataOut[templen - 1] = 0;
    }
    else if ((templen == 2) && (apdu_r->DataOut[templen - 2] > 0)) {
        apdu_r->LenOut = 0;
        apdu_r->SWA = apdu_r->DataOut[templen - 2];
        apdu_r->SWB = apdu_r->DataOut[templen - 1];
        apdu_r->DataOut[templen - 2] = 0;
        apdu_r->DataOut[templen - 1] = 0;
    }
    else {
        apdu_r->ReadCardDataOk = COMMANDERR;
    }
    return 0;
}

void ContactlessIccIsoCommand(APDU_SEND *ApduSend, APDU_RESP *apdu_r) {
    unsigned char *iccommand;   //[300]
    //unsigned short i;
    //unsigned char flag = 0;
    unsigned char retCode;
    int len = 0;

    //i = 0;


    if (ApduSend->EnableCancel) {
        retCode = qPBOC_checkifcancelcontactlesstrade();

        if (retCode == qPBOC_RESETCOMMAND || retCode == PAYWAVE_DETECTEDOTHERMODE ||
            retCode == qPBOC_RESTARTRANDE) {
            apdu_r->ReadCardDataOk = COMMANDERR;

            if (retCode == qPBOC_RESTARTRANDE) {
                gEmvErrID.L3 = EMVB_ERRID_L3_OK;
                gEmvErrID.L1 = EMVB_ERRID_L1_TIMEOUTERR;
                //retCode = qPBOC_RESETCOMMAND;
            }
            else if (retCode == qPBOC_RESETCOMMAND) {
                gEmvErrID.L3 = EMVB_ERRID_L3_STOP;
            }
            else {
                gEmvErrID.L3 = EMVB_ERRID_L3_CARDMAGINSERT;
            }
            return;
        }
    }
    iccommand = (unsigned char *) emvb_malloc(300);

    memset(iccommand, 0, 300);
    //iccommand[0] = EMVCARDNO;
    memcpy(&iccommand[1], ApduSend->Command, 4);
    len = len + 5;
    ApduSend->Lc &= 0x00ff;

    if (ApduSend->Lc > 0)  //�з�������
    {
        iccommand[5] = ApduSend->Lc;
        len = len + 1;

#if 0 /*Modify by ���ƽ at 2015.01.23  14:18 */

        if(ApduSend->Lc > 260) {ApduSend->Lc = 260; }
#endif /* if 0 */
        memcpy(&iccommand[6], ApduSend->DataIn, ApduSend->Lc);

        len += ApduSend->Lc;

        if (ApduSend->Le > 0)      //�з�������
        {
            ApduSend->Le &= 0x00ff;
            iccommand[6 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
    }
    else        //�޷�������
    {
        if (ApduSend->Le > 0)      //�з�������
        {
            ApduSend->Le &= 0x00ff;
            iccommand[5 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
        else    //�޷�������
        {
            len = len + 1;
            iccommand[5 + ApduSend->Lc] = 0;
        }
    }
    memset((unsigned char *) apdu_r, 0, sizeof(APDU_RESP));


    while (DealqPBOCCardData(&iccommand[1], (unsigned short) (len - 1), apdu_r)) {
    }


    //ccc command
#if 0           //sxl?? paypass����ʱ������

    if(memcmp(ApduSend->Command, "\x80\x2A\x8E\x80", 4) == 0)
    {
        paypassccctimer = PAYPASS_CCCTIMER;
        paypassccctimer = (1 << paypasscccovertimes) * paypassccctimer;
        paypassccctimer += 50;
        transflow_opentimer(&paypassccctimer);
    }
#endif


    gEmvErrID.L1 = EMVB_ERRID_L1_OK;
    gEmvErrID.L3 = EMVB_ERRID_L3_OK;


    if (apdu_r->ReadCardDataOk != EMV_OK) {
        gEmvErrID.L1 = EMVB_ERRID_L1_TIMEOUTERR;
    }
    else {
        gEmvErrID.SW12[0] = apdu_r->SWA;
        gEmvErrID.SW12[1] = apdu_r->SWB;
    }
#ifdef EMVB_DEBUG
    printf("\r\napdu_r->ReadCardDataOk1 = %d\r\n", apdu_r->ReadCardDataOk);
#endif

    if (ApduSend->EnableCancel) {
        retCode = qPBOC_checkifcancelcontactlesstrade();

        if (retCode == qPBOC_RESETCOMMAND || retCode == PAYWAVE_DETECTEDOTHERMODE ||
            retCode == qPBOC_RESTARTRANDE) {
            apdu_r->ReadCardDataOk = COMMANDERR;

            if (retCode == qPBOC_RESTARTRANDE) {
                gEmvErrID.L3 = EMVB_ERRID_L3_OK;
                gEmvErrID.L1 = EMVB_ERRID_L1_TIMEOUTERR;
                //retCode = qPBOC_RESETCOMMAND;
            }
            else if (retCode == qPBOC_RESETCOMMAND) {
                gEmvErrID.L3 = EMVB_ERRID_L3_STOP;
            }
            else {
                gEmvErrID.L3 = EMVB_ERRID_L3_CARDMAGINSERT;
            }
            return;
        }
    }
#ifdef EMVB_DEBUG
        printf("\r\napdu_r->ReadCardDataOk2 = %d\r\n", apdu_r->ReadCardDataOk);
#endif


    emvb_free(iccommand);
}

unsigned char DealEmvCardData(unsigned char *tempsenddata, unsigned short tempdatalen,
                              APDU_RESP *apdu_r) {
    s32 templen = 0; //, EMVRxLen = 0;
    SDK_ICC_PARAM tempIccParam;
    //s32 rslt;

#ifdef EMVB_DEBUG
    s32 i;
#endif

    //sxl?
#ifdef EMVB_DEBUG
    Trace("emv", "\r\nIccsend:%d\r\n", tempdatalen);

    for(i = 0; i < tempdatalen; i++)
    {
        Trace("emv", "%02x ", tempsenddata[i]);
    }

    Trace("emv", "\r\n");
#endif

    //��������       //sxl? ����Ҫ����
    //DealEMVIcSim(tempsenddata,tempdatalen,EMVRxBuf,&EMVRxLen);
    //rslt = sdkIccSendIccCardData(gPayData.pstIccParam, tempsenddata, tempdatalen, apdu_r->DataOut, &templen);
#ifndef SPI_DDI
//xiaox 15/11/24
	memmove(&tempsenddata[2],tempsenddata,tempdatalen);
	tempsenddata[0] = GlbEMVBApp_UnionStruct->magdatalen;
	tempsenddata[1] = GlbEMVBApp_UnionStruct->magdatapos;
	tempdatalen += 2;
#endif

    sdkIccSendIccCardData(gPayData.pstIccParam, tempsenddata, tempdatalen, apdu_r->DataOut,
                          &templen);
        Trace("emv","sdkIccSendIccCardData len = %d",templen);

#ifdef EMVB_DEBUG

    if(templen)
    {
        Trace("emv", "\r\nIccrec:%d\r\n", templen);

        for(i = 0; i < templen; i++)
        {
            Trace("emv", "%02x ", apdu_r->DataOut[i]);
        }

        Trace("emv", "\r\n");
    }
#endif

    apdu_r->ReadCardDataOk = 0;
    //templen = EMVRxLen ;
    //if(EMVRxBuf[0] == EMVCARDNO)
    {
        if (templen > 2) {
            apdu_r->LenOut = templen - 2;
            memcpy(apdu_r->DataOut, apdu_r->DataOut, apdu_r->LenOut);
            apdu_r->SWA = apdu_r->DataOut[templen - 2];
            apdu_r->SWB = apdu_r->DataOut[templen - 1];
            apdu_r->DataOut[templen - 2] = 0;
            apdu_r->DataOut[templen - 1] = 0;
        }
        else if ((templen == 2) && (apdu_r->DataOut[templen - 2] > 0)) {
            apdu_r->LenOut = 0;
            apdu_r->SWA = apdu_r->DataOut[templen - 2];
            apdu_r->SWB = apdu_r->DataOut[templen - 1];
            apdu_r->DataOut[templen - 2] = 0;
            apdu_r->DataOut[templen - 1] = 0;
        }
        else {
			//xiaox 15/12/14
            //if (sdkIccGetCardStatus(&tempIccParam, 100) != SDK_OK)             //��⿨�Ƿ�γ�
            {
                //��������
                gPayData.ContactCardStatus = EMVB_ICC_NOCARD;
            }
            apdu_r->ReadCardDataOk = COMMANDERR;
        }
        return 0;
    }
    //return 1;
}

void EmvIccIsoCommand(APDU_SEND *ApduSend, APDU_RESP *apdu_r)      //�������ݰ���ȴ�������
{
    unsigned char *iccommand;   //[300]
    //unsigned short i;
    unsigned char retCode;

    int len = 0;

    //i = 0;
    iccommand = (unsigned char *) emvb_malloc(300);
    memset(iccommand, 0, 300);
    //iccommand[0] = EMVCARDNO;
    memcpy(&iccommand[1], ApduSend->Command, 4);
    len = len + 5;
    ApduSend->Lc &= 0x00ff;

    if (ApduSend->Lc > 0)  //�з�������
    {
        iccommand[5] = ApduSend->Lc;
        len = len + 1;

#if 0 /*Modify by ���ƽ at 2015.01.23  14:42 */

        if(ApduSend->Lc > 260) {ApduSend->Lc = 260; }
#endif /* if 0 */
        memcpy(&iccommand[6], ApduSend->DataIn, ApduSend->Lc);

        len += ApduSend->Lc;

        if (ApduSend->Le > 0)      //�з�������
        {
            ApduSend->Le &= 0x00ff;
            iccommand[6 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
    }
    else        //�޷�������
    {
        if (ApduSend->Le > 0)      //�з�������
        {
            ApduSend->Le &= 0x00ff;
            iccommand[5 + ApduSend->Lc] = ApduSend->Le;
            len = len + 1;
        }
        else    //�޷�������
        {
            len = len + 1;
            iccommand[5 + ApduSend->Lc] = 0;
        }
    }
    memset((unsigned char *) apdu_r, 0, sizeof(APDU_RESP));


    while (DealEmvCardData(&iccommand[1], (unsigned short) (len - 1), apdu_r)) {
    }

    gEmvErrID.L1 = EMVB_ERRID_L1_OK;
    gEmvErrID.L3 = EMVB_ERRID_L3_OK;

    if (apdu_r->ReadCardDataOk != EMV_OK) {
        gEmvErrID.L1 = EMVB_ERRID_L1_TIMEOUTERR;
    }
    else {
        gEmvErrID.SW12[0] = apdu_r->SWA;
        gEmvErrID.SW12[1] = apdu_r->SWB;
    }
#ifdef EMVB_DEBUG
    printf("\r\napdu_r->ReadCardDataOk1 = %d\r\n", apdu_r->ReadCardDataOk);
#endif

    if (ApduSend->EnableCancel) {
        retCode = qPBOC_checkifcancelcontactlesstrade();

        if (retCode == qPBOC_RESETCOMMAND || retCode == PAYWAVE_DETECTEDOTHERMODE) {
            apdu_r->ReadCardDataOk = COMMANDERR;

            if (retCode == qPBOC_RESETCOMMAND) {
                gEmvErrID.L3 = EMVB_ERRID_L3_STOP;
            }
            else {
                gEmvErrID.L3 = EMVB_ERRID_L3_CARDMAGINSERT;
            }
        }
    }
    emvb_free(iccommand);
}

void IccIsoCommand(APDU_SEND *ApduSend, APDU_RESP *apdu_r) {
    u8 tempIccParamflag = 0;
    SDK_ICC_PARAM tempIccParam;

    Trace("IccCMD", "test1");

    if (gPayData.pstIccParam == NULL) {
        memset(&tempIccParam, 0, sizeof(SDK_ICC_PARAM));
        gPayData.pstIccParam = &tempIccParam;
        gPayData.pstIccParam->ucCardMode = SDK_ICC_RF;
        gPayData.pstIccParam->eCardType = SDK_ICC_CPU;
        tempIccParamflag = 1;
    }
    Trace("IccCMD", "\r\nucCardMode = %d\r\n", gPayData.pstIccParam->ucCardMode);

    if (gPayData.pstIccParam->ucCardMode & SDK_ICC_RF) {
        ContactlessIccIsoCommand(ApduSend, apdu_r);
    }
    else {
        EmvIccIsoCommand(ApduSend, apdu_r);
    }
    if (tempIccParamflag) {
        gPayData.pstIccParam = NULL;
    }
}

void EMVB_TradeParam_Create(unsigned int maxAppListCandidatenum, unsigned int maxaidnum) {
    if (gEMVTradeParam == NULL) {
        gEMVTradeParam = (TRADEPARAMETER *) emvb_malloc(sizeof(TRADEPARAMETER));
    }
    else {
        if (gEMVTradeParam->CAPK != NULL) {
            emvb_free(gEMVTradeParam->CAPK);
        }

        if (gEMVTradeParam->SelectedApp != NULL) {
            emvb_free(gEMVTradeParam->SelectedApp);
        }

        if (gEMVTradeParam->AppListCandidate != NULL) {
            emvb_free(gEMVTradeParam->AppListCandidate);
        }

        if (gEMVTradeParam->gPaypassProceFlag != NULL) {
            emvb_free(gEMVTradeParam->gPaypassProceFlag);
        }
    }
    memset(gEMVTradeParam, 0, sizeof(TRADEPARAMETER));
    gEMVTradeParam->CAPK = (CAPK_STRUCT *) emvb_malloc(sizeof(CAPK_STRUCT));
    memset((unsigned char *) gEMVTradeParam->CAPK, 0, sizeof(CAPK_STRUCT));

    gEMVTradeParam->SelectedApp = (LISTAPPDATA *) emvb_malloc(sizeof(LISTAPPDATA));
    memset((unsigned char *) gEMVTradeParam->SelectedApp, 0, sizeof(LISTAPPDATA));

    gEMVTradeParam->AppListCandidate = (LISTAPPDATA *) emvb_malloc(
            sizeof(LISTAPPDATA) * maxAppListCandidatenum);
    memset((unsigned char *) gEMVTradeParam->AppListCandidate, 0,
           sizeof(LISTAPPDATA) * maxAppListCandidatenum);

    gEMVTradeParam->gPaypassProceFlag = (PAYPASSPREPROCEFLAG *) emvb_malloc(
            sizeof(PAYPASSPREPROCEFLAG) * maxaidnum);
    memset((unsigned char *) gEMVTradeParam->gPaypassProceFlag, 0,
           sizeof(PAYPASSPREPROCEFLAG) * maxaidnum);
}

void EMVB_malloctradememory(void)   //20140320
{


    //��������ֽڵĿ����в���
    Trace("emv", "dllemvb_avl_create();\r\n");
    dllemvb_avl_create();      //��ÿ�ζ�Ҫ���½���,�����������Ա�Ӧ�ó���ÿ��ƣ����Ե������������ʱ����ɾ���ⲿ����Դ
    Trace("emv", "dllemvb_avl_create() done.\r\n");


    if (GlbEMVBApp_UnionStruct == NULL)      //�������̺�Ҫɾ����Щ
    {
        Trace("emv",
              "GlbEMVBApp_UnionStruct = (EMVBAppUnionStruct *)emvb_malloc(sizeof(EMVBAppUnionStruct));\r\n");
        GlbEMVBApp_UnionStruct = (EMVBAppUnionStruct *) emvb_malloc(sizeof(EMVBAppUnionStruct));
        memset(GlbEMVBApp_UnionStruct,0,sizeof(EMVBAppUnionStruct));
    }

    if (gEMVTradeParam == NULL)         //�������̺�Ҫɾ����Щ
    {
        Trace("emv", "EMVB_TradeParam_Create(16,MAXAIDINDEX);\r\n");
        EMVB_TradeParam_Create(16, MAXAIDINDEX);
    }
}

int Emvb_check_MatchTermAID(unsigned char kernelID, unsigned char *aid, unsigned char aidLen,
                            TERMAPP *Applist, unsigned char apptype) {
    unsigned char i;
    unsigned char TransTypeValue;
    unsigned char matchflag = 0;
    unsigned char capkmatchflag = 1;

    TransTypeValue = dllemvb_avl_gettagvalue(TAG_TransTypeValue);


    if (apptype == 3)     //qPBOC  //sxlȷ��
    {
        capkmatchflag = 0;

        for (i = 0; i < MAXCAPKINDEX; i++) {
            if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
                if (!memcmp(aid, gCapkIndex[i].RID, 5)) {
                    capkmatchflag = 1;
                    break;
                }
            }
        }
    }

    if (capkmatchflag == 0) {
        return -1;
    }
#ifdef EMVB_DEBUG
    TraceHex("emvcmpaid", "srcaid", aid, aidLen);
#endif


    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            if (apptype == PAYPASSAPP) {
                if (gAidIndex[i].tradetypevalue != TransTypeValue ||
                    kernelID != gAidIndex[i].KernelID) {
                    continue;
                }
            }

            //����ʱ�Ͷ�����෴20110426
            if (gAidIndex[i].ASI != PARTIAL_MATCH || apptype == PAYPASSAPP)              //����ƥ��
            {
                if (!memcmp(aid, gAidIndex[i].AID, gAidIndex[i].AIDLen)) {
                    matchflag = 1;
                }
            }
            else            //exact match   //ȷ��  //sxl?pos�ն˵ĺ�reader���෴
            {
                if (!memcmp(aid, gAidIndex[i].AID, aidLen))                      //��ȫƥ��
                {
                    matchflag = 1;
                }
            }
#ifdef EMVB_DEBUG
            TraceHex("emvcmpaid", "dstaid", gAidIndex[i].AID, gAidIndex[i].AIDLen);
            Trace("emvcmpaid1", "rslt:%d %d", gAidIndex[i].ASI, matchflag);
#endif

            if (matchflag) {
                Applist->ASI = gAidIndex[i].ASI;
                Applist->AIDLen = gAidIndex[i].AIDLen;
                memcpy(&Applist->AID[0], &gAidIndex[i].AID[0], 16);

                Applist->KernelID = gAidIndex[i].KernelID;
                return i;
            }
        }
    }


    if (apptype == 3)  //����qPBOC����ȷ��һ�²���ƥ���Ƿ����ѡ��
    {
        for (i = 0; i < MAXAIDINDEX; i++) {
            if (gAidIndex[i].ifusedflag == AIDENABLE) {
                if (!memcmp(aid, gAidIndex[i].AID, gAidIndex[i].AIDLen)) {
                    matchflag = 1;
                }

                if (matchflag) {
                    Applist->ASI = gAidIndex[i].ASI;
                    Applist->AIDLen = gAidIndex[i].AIDLen;
                    memcpy(&Applist->AID[0], &gAidIndex[i].AID[0], 16);

                    Applist->KernelID = gAidIndex[i].KernelID;
                    return i;
                }
            }
        }
    }
    return -1;
}

unsigned char manage_termipkrevokecheck(unsigned char *buf) {
    return 0;
}

void emvb_GetRandomNum(unsigned char *randpad, unsigned int randLen) {
    sdkGetRandom(randpad, randLen);
}

void Emvb_ReadTermAID(TERMAPP *Applist, unsigned char *TermApplistNum) {
    unsigned char i;
    unsigned char ApplistNum = 0;


    for (i = 0; i < MAXAIDINDEX; i++) {
        if (gAidIndex[i].ifusedflag == AIDENABLE) {
            (Applist + ApplistNum)->ASI = gAidIndex[i].ASI;
            (Applist + ApplistNum)->AIDLen = gAidIndex[i].AIDLen;
            memcpy((Applist + ApplistNum)->AID, &gAidIndex[i].AID[0], 16);

            (Applist + ApplistNum)->KernelID = gAidIndex[i].KernelID;

            ApplistNum++;

            if (ApplistNum >= 40)             //sxl����AID����
            {
                break;
            }
        }
    }

    *TermApplistNum = ApplistNum;
}

static void EMVB_TradeParam_Destory(void); //��������
unsigned char sdkIccInputCreditPwd(unsigned char pintype, unsigned char *ASCCreditPwd); //��������
/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM *pstIccTradeParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
** Created By:		sujianzhong2012.08.08
** Remarks:             //sxl?  �����   //sxl20140408
*****************************************************************************/
s32 sdkIccTransInit(const SDK_ICC_TRADE_PARAM *pstIccTradeParam)     //sjz 20120726
{


    if (NULL == pstIccTradeParam) {
        return SDK_PARA_ERR;
    }
    EMVB_TradeParam_Destory();                      //zcl 20140815


    Trace("emv", "EMVB_malloctradememory();\r\n");
    EMVB_malloctradememory();


    Trace("emv", "EMVB_TransTermData_Init();\r\n");
    EMVB_TransTermData_Init();


    // zcl
    memset(gucSdkEcCurrencyCode, 0, sizeof(gucSdkEcCurrencyCode));
    memset(gucSdkEcSecCurrencyCode, 0, sizeof(gucSdkEcSecCurrencyCode));
    memset(gucSdkCardInfoSecVLPAvailableFund, 0, sizeof(gucSdkCardInfoSecVLPAvailableFund));

    if (pstIccTradeParam->bIsForceOnline)                                                        // zcl 20120327
    {
        Trace("emv", "dllemvb_avl_setvalue_or(TAG_TVR,3,0x08);\r\n");
        dllemvb_avl_setvalue_or(TAG_TVR, 3, 0x08);
    }

    if (pstIccTradeParam->bIsSupportQPBOC) {
        gEMVTradeParam->qPBOCOrMSD = qPBOCTYPE;
    }

    if (pstIccTradeParam->bIsSupportCLPBOC) {
        dllemvb_avl_setvalue_or(TAG_TermTransPredicable, 0, 0x40);
    }
#if 0           //sxl?������Щ������û�д���
    //VLPTACDenial VLPTACOnline VLPTACDefault   bForceOnline Ӧ��û��ʹ�õ�
    gstSdkTermInfo.bTermSupportVLP = true;                      //0x00;��֧��VLP;//0x01;support VLP.
    gstSdkTermInfo.Language = EMV_CHINESE;
    gucSdkHaveInputAmount = 0;
#endif


    gEMVTradeParam->EmvNowTask = 0;

    switch (pstIccTradeParam->eFlowMode) {
        /*��Ϊemv�����涨����EMV_EC_CASH��EMV_READLOG����������ҲҪ����*/
        case SDK_ICC_EC_QUERY: {
            gEMVTradeParam->EmvNowTask = SDK_ICC_EC_QUERY;
        }
            break;                                                                //��ѯ����

        case SDK_ICC_EC_LOAD: {
            gEMVTradeParam->EmvNowTask = EMV_EC_CASH;
        }
            break;                                                                     //ָ���ʻ�Ȧ�桢��ָ���ʻ�Ȧ����ֽ��ֵ��ͬΪ�ֽ��ֵ

        case SDK_ICC_READLOG: {
            gEMVTradeParam->EmvNowTask = EMV_READLOG;
        }
            break;                                                                     //��������ϸ

        case SDK_ICC_ALL_FLOW:
        case SDK_ICC_EASY_FLOW: {
            gEMVTradeParam->EmvNowTask = 0;
        }
            break;

        default: {
            gEMVTradeParam->EmvNowTask = 0;
        }
            break;
    }


    if (GlbEMVBApp_UnionStruct == NULL) {
        GlbEMVBApp_UnionStruct = (EMVBAppUnionStruct *) emvb_malloc(sizeof(EMVBAppUnionStruct));
        memset(GlbEMVBApp_UnionStruct,0,sizeof(EMVBAppUnionStruct));
    }
    //ȡ�����ٶ�
    //manage_ipkrevokeread(&gIpkRevoke[0]);  //����ٶ����Ļ����������������
    memset(GlbEMVBApp_UnionStruct, 0, sizeof(EMVBAppUnionStruct));

    GlbEMVBApp_UnionStruct->EMVTradeParam = gEMVTradeParam;  //�����Ҫռ5K+2K TERMINFO + 3K CARDINFO ���ڴ�+6K��̬����Ĺ�16 K ,��16K���ܲ���ȫ����

    //sxl?��ָ��Ļ���Ҫ����
    GlbEMVBApp_UnionStruct->EmvErrID = &gEmvErrID;
    GlbEMVBApp_UnionStruct->EMVB_qPBOCIccIsoCommand = IccIsoCommand;     //sxl?��Щ��������Ҫ����

    //sxl?
    Trace("emvinit", "\r\nIccIsoCommand1 =%08x %08x %08x \r\n", GlbEMVBApp_UnionStruct,
          GlbEMVBApp_UnionStruct->EMVB_qPBOCIccIsoCommand, IccIsoCommand);


    GlbEMVBApp_UnionStruct->EMVB_InputCreditPwd = sdkIccInputCreditPwd;
    GlbEMVBApp_UnionStruct->EMVB_RandomNum = emvb_GetRandomNum;
    GlbEMVBApp_UnionStruct->termipkrevokecheck = manage_termipkrevokecheck;
    GlbEMVBApp_UnionStruct->check_MatchTermAID = Emvb_check_MatchTermAID;
    GlbEMVBApp_UnionStruct->ReadTermAID = Emvb_ReadTermAID;

    GlbEMVBApp_UnionStruct->pasTradeAmount = pstIccTradeParam->pasTradeAmount;      //sxl?�鿴����������Ӧ���Ƿ�ʹ����ȷ
    GlbEMVBApp_UnionStruct->InputPWD = pstIccTradeParam->InputPWD;
    GlbEMVBApp_UnionStruct->DispPromptData = (int (*)(
            unsigned char ePromptType)) pstIccTradeParam->DispPromptData;

    //sxl?
    Trace("emvinit", "\r\nIccIsoCommand2 =%08x %08x %08x \r\n", GlbEMVBApp_UnionStruct,
          GlbEMVBApp_UnionStruct->EMVB_qPBOCIccIsoCommand, IccIsoCommand);

    if (NULL != pstIccTradeParam->UserIccTransInit) {
        (*pstIccTradeParam->UserIccTransInit)();     //�˺����ṩ�Ĺ���:�û������³�ʼ��emv����
    }
    //sxl?
    Trace("emvinit", "\r\nIccIsoCommand3 =%08x %08x %08x \r\n", GlbEMVBApp_UnionStruct,
          GlbEMVBApp_UnionStruct->EMVB_qPBOCIccIsoCommand, IccIsoCommand);
    memset(&gPayData, 0, sizeof(PAYDATA));     //sxl?�鿴��������
	//zhouxiaoxin
	GlbEMVBApp_UnionStruct->getofflineencdata = sdkIccGetOfflineEncdata;
	//xiaox 15/11/14
	GlbEMVBApp_UnionStruct->getmagdata = sdkIccGetTrackData;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        get Application Candidate
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_IccCommandErr:
                                SDK_CardBlockErr:
                                SDK_IccDataFormatErr
                                SDK_ERR
** Created By:		sujianzhong2012.08.08
** Remarks:     //sxl20140408
*****************************************************************************/
s32 sdkIccGetAppCandidate(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }
    //sxl?
//    Trace("emvinit", "\r\nIccIsoCommand4 =%08x %08x %08x \r\n", GlbEMVBApp_UnionStruct,
//          GlbEMVBApp_UnionStruct->EMVB_qPBOCIccIsoCommand, IccIsoCommand);

        Trace("emv","pstIccParam->ucCardMode3 = %d",pstIccParam->ucCardMode);
    gPayData.pstIccParam = (SDK_ICC_PARAM *) pstIccParam;

    while (1) {
        if (SDK_ICC_RF == pstIccParam->ucCardMode)  //sxl? SDK_ICC_NOCARD ��Ҫ����SDK_ICC_NOCARD
        {
            Trace("emv", "qPBOC_GetAppCandidate\r\n");
            retCode = qPBOCb_GetAppCandidate(
                    GlbEMVBApp_UnionStruct);   //sxl? qPBOC���������Ҫ����������
            Trace("emv", "qPBOC_GetAppCandidate retCode = %02d\r\n", retCode);
        }
        else {
            Trace("emv", "EMV_AppSelect %08x\r\n", GlbEMVBApp_UnionStruct);
            retCode = EMVb_AppSelect(GlbEMVBApp_UnionStruct);
            Trace("emv", "EMV_AppSelect retCode = %02d\r\n", retCode);
        }

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (APPSEL_PSENOTFOUND == retCode) {
            retCode = ERR_EMV_NoAppSel;
        }
        else if ((EMV_OK != retCode) && (ERR_EMV_IccCommand != retCode) &&
                 (ERR_EMV_CardBlock != retCode) && (ERR_EMV_IccDataFormat != retCode)) {
            retCode = EMV_ERR;
        }
        Trace("emv", "sdkIccEmvToSdkRlt(%02d, &rlt);\r\n", retCode);
        sdkIccEmvToSdkRlt(retCode, &rlt);
        Trace("emv", "sdkIccEmvToSdkRlt(%02d, %d);\r\n", rlt);

        //zcl 20130702
        if (SDK_ICC_RF == pstIccParam->ucCardMode) {
            if (rlt != SDK_OK && rlt != SDK_NoAppSelErr)   //sxl? ��ȷ��
            {
                rlt = SDK_PSENotFoundErr;
            }
        }
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_FIXED_AID stFixedAid:
                                {   IsFixed: true:select fixed AID; false:no select fixed AID
                                    len: Aid Bytes(eg:Aid="A0 33 33 33 33",len=5 )
                                    Aid: Aid data
                                }
                                const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_UserSelect
                                SDK_OK
                                SDK_NoAppSelErr
                                SDK_CancelTransErr
                                SDK_FixedAidAbsentErr
                                SDK_ERR
                                SDK_AppBlockErr
** Created By:		sujianzhong2012.08.08
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccOrganizeAppList(SDK_ICC_FIXED_AID stFixedAid, const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR; //, i;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (true == stFixedAid.bIsFixed) {
        gucSdkPosSmsTempAid[0] = stFixedAid.uclen;

        if (gucSdkPosSmsTempAid[0] > 16) {
            gucSdkPosSmsTempAid[0] = 16;
        }
        memcpy(&gucSdkPosSmsTempAid[1], stFixedAid.heAid, gucSdkPosSmsTempAid[0]);
        retCode = Emvb_ChooseContactApp(GlbEMVBApp_UnionStruct, gucSdkPosSmsTempAid);
        Trace("emv", "Emv_ChooseContactApp retCode = %02d\r\n", retCode);
    }
    else {
        if (SDK_ICC_RF == pstIccParam->ucCardMode) {
            retCode = contactless_ChooseApp(GlbEMVBApp_UnionStruct);
            Trace("emv", "qPBOC_ChooseApp retCode = %02d\r\n", retCode);
        }
        else {
            retCode = Emvb_ChooseApp(GlbEMVBApp_UnionStruct);
            Trace("emv", "Emv_ChooseApp retCode = %02d\r\n", retCode);
        }
    }

// ===sjz 20120508 ѡ�񲻵�ָ����AID���ṩ��ѡ�б���û�ѡ��======
    if ((true == stFixedAid.bIsFixed) && (ERR_EMV_CancelTrans == retCode)) {
        return SDK_FixedAidAbsentErr;           // ��������ֵ:ѡ��Ĺ̶�AID������
    }
// ====================end====================================
    else if (ERR_EMV_NoAppSel == retCode) {
        if (GlbEMVBApp_UnionStruct->EMVTradeParam->appblockflag == 1) {
            return SDK_AppBlockErr;
        }
        retCode = ERR_EMV_NoAppSel;
    }
    else if ((EMV_OK != retCode) && (EMV_USERSELECT != retCode) &&
             (ERR_EMV_CancelTrans != retCode)) {
        retCode = EMV_ERR;
    }
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_CancelTransErr
                                SDK_IccReturnErr
                                SDK_ERR
** Created By:		sujianzhong2012.08.08
** Remarks:			//sxl20140408
*****************************************************************************/
s32 sdkIccUserSelectApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                        const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode;
    s32 rlt = SDK_ERR;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (SDK_ICC_RF == pstIccParam->ucCardMode)     // ���ûص�������ʾ???
    {
#if 0          //sxl �Ѿ���contactless_ChooseApp��ѡ��ã��������ṩ���û�ѡ��

        if(NULL == pstIccTradeParam->DispCandAppList)
        {
            sdkIccPostqPBOCDispAppList();  //sxl qPBOC��Ҳ����ʾ���û�ѡ��, ��select app��ʱ�����ȼ�ֱ��ѡ��
        }
        else
        {
            sdkIccDealqPBOCDispAppList(pstIccTradeParam);
        }
#endif
        retCode = EMV_OK;
    }
    else {
        if (NULL == pstIccTradeParam->DispCandAppList) {
            sdkIccPostDispAppList();
        }
        else {
            sdkIccDealDispAppList(pstIccTradeParam);
        }
    }
    Trace("emv", "-----------�˳�\r\n");

    if (SDK_ICC_RF != pstIccParam->ucCardMode) {
        retCode = Emvb_UserSelectResult(GlbEMVBApp_UnionStruct);
    }
    else {
        retCode = EMV_OK;
    }

    if ((EMV_OK != retCode) && (ERR_EMV_IccReturn != retCode) && (ERR_EMV_CancelTrans != retCode)) {
        retCode = EMV_ERR;
    }
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

void EmvbAidToTermInfo(unsigned char i, EmvSysIni_STRUCT *termaidparam);

void qPBOCb_AidToTermInfo(unsigned char i, EmvSysIni_STRUCT *termaidparam,
                          unsigned char onlinepinsup) {
    //unsigned int amt;
    //unsigned int j;

    EmvbAidToTermInfo(i, termaidparam);      //zcl 20140702
    dllemvb_avl_createsettagvalue(TAG_TermAID, (termaidparam + i)->Aid, (termaidparam + i)->AidLen);

    dllemvb_avl_createsettagvalue(TAG_AppVerNum, (termaidparam + i)->AppVerNum, 2);
    dllemvb_avl_createsettagvalue(TAG_TACDefault, (termaidparam + i)->TacDefault, 5);
    dllemvb_avl_createsettagvalue(TAG_TACOnline, (termaidparam + i)->TacOnline, 5);
    dllemvb_avl_createsettagvalue(TAG_TACDenial, (termaidparam + i)->TacDecline, 5);


    //dllemvb_avl_createsettagvalue(TAG_TermDDOL,(termaidparam+i)->TermDDOL,(termaidparam+i)->TermDDOLLen);
#if 0 /*Modify by zcl at 2014.07.01  18:46 */
    dllemvb_avl_createsettagvalue(TAG_TermTransPredicable, (termaidparam + i)->TermTransPredicable, 4);

    dllemvb_avl_createsettagvalue(TAG_TermCapab, (termaidparam + i)->terminalcapability, 3);


    dllemvb_avl_createsettagvalue(TAG_TermAddCapab, (termaidparam + i)->terminaladdcapability, 5);
#endif /* if 0 */


#if 0 /*Modify by zcl at 2014.07.01  19:1 */
    dllemvb_avl_set(TAG_TermType, (termaidparam + i)->terminaltype);
#endif /* if 0 */

#if 0 /*Modify by zcl at 2014.07.02  8:34 */
    dllemvb_avl_setvalue_and(TAG_TermTransPredicable, 1, 0x3f);
#endif /* if 0 */

    dllemvb_avl_createsettagvalue(TAG_VLPTransLimit, (termaidparam + i)->vlptranslimit, 6);

    dllemvb_avl_createsettagvalue(TAG_termcvm_limit, (termaidparam + i)->termcvm_limit, 6);
    dllemvb_avl_createsettagvalue(TAG_clessofflineamt, (termaidparam + i)->clessofflineamt, 6);
    dllemvb_avl_createsettagvalue(TAG_clessofflinelimitamt,
                                  (termaidparam + i)->clessofflinelimitamt, 6);

    if ((termaidparam + i)->TermPinCap) {
        dllemvb_avl_setvalue_or(TAG_TermCapab, 1, 0x40);
    }
    else {
        dllemvb_avl_setvalue_or(TAG_TermCapab, 1, 0xBF);
    }
}

s32 manage_aidread(u32 id, EmvSysIni_STRUCT *tmpaid) {
    u32 addr;
    u8 *tmp;   //[AIDLEN]
    s32 ret;
    u8 checksum;
    u8 fn[SDK_EMV_MAX_PATH];
    s32 readfilelen;

    memset(fn, 0, SDK_EMV_MAX_PATH);             //sxl? ����Щͳһ�ŵ�һ��������
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newaid");


    tmp = (u8 *) emvb_malloc(AIDLEN);
    addr = id << AIDSHIFTNUM;
    readfilelen = AIDLEN;
    ret = sdkReadFile(fn, tmp, addr, &readfilelen);

    if (ret != SDK_FILE_OK) {
        emvb_free(tmp);
        return 1;
    }
    checksum = sdkCalcCrc16(tmp, AIDLEN - 1);   //sxl? ����ЩҲͳһ�ŵ�һ��������

    if (checksum != tmp[AIDLEN - 1]) {
        emvb_free(tmp);
        return 1;
    }

    if (tmp[AIDFLAGOFFSET] != AIDENABLE) {
        emvb_free(tmp);
        return 1;
    }
    memcpy((u8 *) tmpaid, tmp, sizeof(EmvSysIni_STRUCT));

    emvb_free(tmp);

    return 0;
}

s32 qPBOCb_readaidparameters(EMVBAppUnionStruct *tempApp_UnionStruct) {
    u8 i;
    //u8 k;
    //s32 ret;
    EmvSysIni_STRUCT *tempaidparam;
    LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;


    tempappdata = tempApp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectedappno = tempApp_UnionStruct->EMVTradeParam->SelectedAppNo;

    tempaidparam = (EmvSysIni_STRUCT *) emvb_malloc(sizeof(EmvSysIni_STRUCT));

    for (i = 0; i < MAXAIDINDEX; i++) {
        if ((memcmp((tempappdata + tempselectedappno)->AidInTerm, gAidIndex[i].AID,
                    gAidIndex[i].AIDLen) == 0) &&
            ((tempappdata + tempselectedappno)->AidInTermLen == gAidIndex[i].AIDLen)) {
            if (gAidIndex[i].ifusedflag == AIDENABLE) {
                if (manage_aidread(i, tempaidparam)) {
                    emvb_free(tempaidparam);
                    return EMV_APPSELECTTRYAGAIN;
                }
#ifdef EMVB_DEBUG
                printf("\r\nqPBOC_readaidparameters index = %d \r\n", i);
#endif


                qPBOCb_AidToTermInfo(0, tempaidparam, 0);
                break;
            }
        }
    }


    emvb_free(tempaidparam);


    if (i >= MAXAIDINDEX) {
        return EMV_APPSELECTTRYAGAIN;
    }
    //appLabel DFname����Ӧ��ѡ���ʱ���У���Ҫע���Ƿ�����
#ifdef EMVB_DEBUG
    //printf("\r\nqPBOC_readaidparameters11 index = %d \r\n",*index);
#endif

//appLabel DFname����Ӧ��ѡ���ʱ���У���Ҫע���Ƿ�����
    //������ʱ����Ҫ����
    if (dllemvb_avl_checkiftagexist(TAG_CardAID) == 0)  //sxl? EMV��ҲҪ��������
    {
        if ((tempappdata + tempselectedappno)->AIDLen) {
            dllemvb_avl_createsettagvalue(TAG_CardAID, (tempappdata + tempselectedappno)->AID,
                                          (tempappdata + tempselectedappno)->AIDLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_AppLabel) == 0) {
        if ((tempappdata + tempselectedappno)->AppLabelLen) {
            dllemvb_avl_createsettagvalue(TAG_AppLabel, (tempappdata + tempselectedappno)->AppLabel,
                                          (tempappdata + tempselectedappno)->AppLabelLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_AppPreferName) == 0) {
        if ((tempappdata + tempselectedappno)->PreferNameLen) {
            dllemvb_avl_createsettagvalue(TAG_AppPreferName,
                                          (tempappdata + tempselectedappno)->PreferName,
                                          (tempappdata + tempselectedappno)->PreferNameLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_LangPrefer) == 0) {
        if ((tempappdata + tempselectedappno)->LangPreferLen) {
            dllemvb_avl_createsettagvalue(TAG_LangPrefer,
                                          (tempappdata + tempselectedappno)->LangPrefer,
                                          (tempappdata + tempselectedappno)->LangPreferLen);
        }
    }
    return EMV_OK;
}

void EmvbAidToTermInfo(unsigned char i, EmvSysIni_STRUCT *termaidparam) {
    //unsigned int amt;
    //unsigned int j;
    //unsigned char templimit[6];

    //sxl20131024  pinpad���Ȳ�������  �鿴qPBOC��paywave�Լ�paypass�Ƿ���Ҫ��������
    dllemvb_avl_createsettagvalue(TAG_TermAID, (termaidparam + i)->Aid, (termaidparam + i)->AidLen);

    dllemvb_avl_createsettagvalue(TAG_AppVerNum, (termaidparam + i)->AppVerNum, 2);

    dllemvb_avl_createsettagvalue(TAG_TACDefault, (termaidparam + i)->TacDefault, 5);

    dllemvb_avl_createsettagvalue(TAG_TACOnline, (termaidparam + i)->TacOnline, 5);

    dllemvb_avl_createsettagvalue(TAG_TACDenial, (termaidparam + i)->TacDecline, 5);


    dllemvb_avl_createsettagvalue(TAG_FloorLimit, (termaidparam + i)->FloorLimit, 4);
    dllemvb_avl_createsettagvalue(TAG_Threshold, (termaidparam + i)->Threshold, 4);
    dllemvb_avl_set(TAG_MaxTargetPercent, (termaidparam + i)->MaxTargetPercent);
    dllemvb_avl_set(TAG_TargetPercent, (termaidparam + i)->TargetPercent);
    dllemvb_avl_set(TAG_bShowRandNum, (termaidparam + i)->bShowRandNum);


    dllemvb_avl_createsettagvalue(TAG_TermDDOL, (termaidparam + i)->TermDDOL,
                                  (termaidparam + i)->TermDDOLLen);




    //ע�����²����������صĲ�����û��  //sxl?20131024 pinpad����ʱ����
#if 0 /*Modify by zcl at 2014.06.13  14:35 */
    dllemvb_avl_createsettagvalue(TAG_TermCapab, (termaidparam + i)->terminalcapability, 3);
#endif /* if 0 */
#ifdef EMVB_DEBUG
    dllemvb_avl_printtagallvalue("TAG_TermCapab", TAG_TermCapab);
#endif

    if ((termaidparam + i)->TermPinCap) {
        dllemvb_avl_setvalue_or(TAG_TermCapab, 1, 0x40);
    }
    else {
        dllemvb_avl_setvalue_or(TAG_TermCapab, 1, 0xBF);
    }
#if 0 /*Modify by zcl at 2014.06.13  14:36 */
    dllemvb_avl_createsettagvalue(TAG_TermAddCapab, (termaidparam + i)->terminaladdcapability, 5);



    dllemvb_avl_set(TAG_TermType, (termaidparam + i)->terminaltype);
#endif /* if 0 */
}

s32 Emvb_readaidparameters(EMVBAppUnionStruct *tempApp_UnionStruct) {
    u8 i;
    //u8 k;
    //s32 ret;


    EmvSysIni_STRUCT *tempaidparam;
    LISTAPPDATA *tempappdata;
    unsigned char tempselectedappno;

    tempappdata = tempApp_UnionStruct->EMVTradeParam->AppListCandidate;
    tempselectedappno = tempApp_UnionStruct->EMVTradeParam->SelectedAppNo;


    for (i = 0; i < MAXAIDINDEX; i++) {
        if ((memcmp((tempappdata + tempselectedappno)->AidInTerm, gAidIndex[i].AID,
                    gAidIndex[i].AIDLen) == 0) &&
            ((tempappdata + tempselectedappno)->AidInTermLen == gAidIndex[i].AIDLen)) {
            if (gAidIndex[i].ifusedflag == AIDENABLE) {
                tempaidparam = (EmvSysIni_STRUCT *) emvb_malloc(sizeof(EmvSysIni_STRUCT));

                if (manage_aidread(i, tempaidparam)) {
                    emvb_free(tempaidparam);
                    return EMV_APPSELECTTRYAGAIN;
                }
#ifdef EMVB_DEBUG
                printf("\r\nqPBOC_readaidparameters index = %d \r\n", i);
#endif


                EmvbAidToTermInfo(0, tempaidparam);

                emvb_free(tempaidparam);
#ifdef EMVB_DEBUG
                printf("\r\nPaywavebAidToTermInfo end\r\n");
#endif
                break;                 //huxiang add 091013 for EC and qPBOC
            }
        }
    }

    if (i >= MAXAIDINDEX) {
        return EMV_APPSELECTTRYAGAIN;
    }
#ifdef EMVB_DEBUG
    //printf("\r\nqPBOC_readaidparameters11 index = %d \r\n",*index);
#endif

    //appLabel DFname����Ӧ��ѡ���ʱ���У���Ҫע���Ƿ�����
    //������ʱ����Ҫ����
    if (dllemvb_avl_checkiftagexist(TAG_CardAID) == 0)  //sxl? EMV��ҲҪ��������
    {
        if ((tempappdata + tempselectedappno)->AIDLen) {
            dllemvb_avl_createsettagvalue(TAG_CardAID, (tempappdata + tempselectedappno)->AID,
                                          (tempappdata + tempselectedappno)->AIDLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_AppLabel) == 0) {
        if ((tempappdata + tempselectedappno)->AppLabelLen) {
            dllemvb_avl_createsettagvalue(TAG_AppLabel, (tempappdata + tempselectedappno)->AppLabel,
                                          (tempappdata + tempselectedappno)->AppLabelLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_AppPreferName) == 0) {
        if ((tempappdata + tempselectedappno)->PreferNameLen) {
            dllemvb_avl_createsettagvalue(TAG_AppPreferName,
                                          (tempappdata + tempselectedappno)->PreferName,
                                          (tempappdata + tempselectedappno)->PreferNameLen);
        }
    }

    if (dllemvb_avl_checkiftagexist(TAG_LangPrefer) == 0) {
        if ((tempappdata + tempselectedappno)->LangPreferLen) {
            dllemvb_avl_createsettagvalue(TAG_LangPrefer,
                                          (tempappdata + tempselectedappno)->LangPrefer,
                                          (tempappdata + tempselectedappno)->LangPreferLen);
        }
    }
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_AppSelectTryAgainErr
                                SDK_IccCommandErr
                                SDK_ERR
                                SDK_UsePbocErr
** Created By:		sujianzhong2012.08.08
** Remarks:            //sxl20140408
*****************************************************************************/
s32 sdkIccFinalSelectedApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                           const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    //u16 i;
    u8 AID[20];
    u8 TermCapab[3];

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (pstIccTradeParam->eFlowMode == SDK_ICC_READLOG) {
        gEMVTradeParam->ReadLogFlag = 1;
    }
    else {
        gEMVTradeParam->ReadLogFlag = 0;
    }

    while (1) {
        if (SDK_ICC_RF == pstIccParam->ucCardMode) {
            retCode = qPBOCb_FinalSelectedApp(GlbEMVBApp_UnionStruct);  //sxl?�鿴һ���ĵ�����
            Trace("evm", "qPBOC_FinalSelectedApp retCode = %02d\r\n", retCode);
        }
        else {
            retCode = Emvb_FinalSelectedApp(GlbEMVBApp_UnionStruct);
            Trace("evm", "Emv_FinalSelectedApp retCode = %02d\r\n", retCode);
        }

        if (retCode == qPBOC_READAIDPARAMETERS) {
            if (SDK_ICC_RF == pstIccParam->ucCardMode) {
                retCode = qPBOCb_readaidparameters(GlbEMVBApp_UnionStruct);
            }
            else {
                retCode = Emvb_readaidparameters(GlbEMVBApp_UnionStruct);
            }
        }

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            memset(AID, 0, sizeof(AID));   //TAG_CardAID�鿴�Ƿ�д�뵽����
            dllemvb_avl_gettagvalue_spec(TAG_CardAID, AID, 0, 8);

            if (memcmp(AID, "\xA0\x00\x00\x03\x33\x01\x01\x06", 8) == 0) {
                gucSdkEC_IsMulAccountCard = 0;     //�������ֽ�
            }
            else {
                gucSdkEC_IsMulAccountCard = 1;
            }
            memset(TermCapab, 0, sizeof(TermCapab));
            dllemvb_avl_gettagvalue_spec(TAG_TermCapab, TermCapab, 0, 3);


            memset(AID, 0, sizeof(AID));             //TAG_CardAID�鿴�Ƿ�д�뵽����
            dllemvb_avl_gettagvalue_spec(TAG_CardAID, AID, 0, 5);

            if (memcmp(AID, "\xA0\x00\x00\x03\x33", 5) == 0) {
                GlbEMVBApp_UnionStruct->EMVTradeParam->AppType = PBOCTYPE;
            }
            else {
                GlbEMVBApp_UnionStruct->EMVTradeParam->AppType = EMVTYPE;
            }

            if (GlbEMVBApp_UnionStruct->EMVTradeParam->AppType == EMVTYPE)     //EMV
            {
                //�����︳ֵ��ʹ������IccTransInit���渳ֵ��û��������
//                memcpy(pstTempterminfo->TermCapab , "\xE0\xF0\xC8" , 3); //IC������PIN
            }
            else     //PBOC
            {
//                memcpy(pstTempterminfo->TermCapab , "\xE0\xE1\xC8" , 3); //�ֿ���֤����֤
                TermCapab[1] &= 0xEF;     //��֧��IC������PIN
                TermCapab[1] |= 0x01;     //�ֿ���֤����֤
            }
#if 0 /*Modify by zcl at 2014.06.13  13:56 */

            if( (i & 0x40) == 0x40)     //����PIN������֤
            {
                TermCapab[1] |= 0x40;
            }
            else
            {
                TermCapab[1] &= 0xBF;
            }
#endif /* if 0 */

            dllemvb_avl_createsettagvalue(TAG_TermCapab, TermCapab, 3);

            retCode = EMV_OK;
        }
            //sxl? ����ȡ������������
        else if ((EMV_APPSELECTTRYAGAIN != retCode) && (ERR_EMV_IccCommand != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:	        reselect application
** Parameters:          void:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_NoAppSelErr
                                SDK_AppBlockErr
** Created By:		sujianzhong2012.08.08
** Remarks:            //sxl20140408
*****************************************************************************/
s32 sdkIccReSelectApp(void)     //sjz 20120726
{
    u8 retCode;
    s32 rlt = SDK_ERR;

    // sdkIccPostEmvSelectAgain();       //ʹ�ûص�������ʾ??? sjz 20120510
    retCode = Emvb_RebuildApplist(GlbEMVBApp_UnionStruct);                  // ���½���APP�б�

    if (ERR_EMV_NoAppSel == retCode) {
        if (GlbEMVBApp_UnionStruct->EMVTradeParam->appblockflag) {
            return SDK_AppBlockErr;
        }
        retCode = ERR_EMV_NoAppSel;
    }
    else if (EMV_OK != retCode) {
        retCode = EMV_ERR;
    }
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

bool Private_sdkIccMatchTagFromPDOL(const u8 *pheTag); //��������
/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_TRADE_PARAM *pstIccTradeParam:
                                const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_NeedMagCardErr: FALLBACK
                                SDK_AppSelectTryAgainErr
                                SDK_IccCommandErr
                                SDK_IccDataFormatErr
                                SDK_IccReturnErr
                                SDK_ERR
                                SDK_ReadECBalance
                                SDK_OnlineWait:qPBOC wait online transflow
                                SDK_OfflineApprove:qPBOC offline approve transflow
                                SDK_OfflineDecline:qPBOC offline decline transflow
                                SDK_UsePbocErr:please use PBOC transflow
                                SDK_ECNotSupportErr
** Created By:		sujianzhong2012.08.09
** Remarks:             //sxl20140408
*****************************************************************************/
s32 sdkIccInitialApp(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                     SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 bcAmount[6] = {0};     //sujianzhong 2013.01.10 9:17
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 AmtAuthNum[6] = {0};
    u8 VLPTransLimit[6] = {0};
    u8 qPBOCOfflineSpendAmount[6] = {0};
    u8 tagAIPflag, tagAFLflag;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (NULL != pstIccTradeParam->pasTradeAmount)     //sujianzhong 2013.01.10 9:17
    {
//        Verify(sdkU32ToBcd(bcAmount,(u32)atoi(pstIccTradeParam->pasTradeAmount), 6) >= 0);
        Verify(sdkAscToBcdR(bcAmount, pstIccTradeParam->pasTradeAmount, 6) >= 0);
        TraceHex("emv", "bcAmount", bcAmount, 6);
    }
    sdkIccImportAmount(bcAmount);     //sujianzhong 2013.01.10 9:17
    dllemvb_avl_gettagvalue_spec(TAG_AmtAuthNum, AmtAuthNum, 0, 6);
    dllemvb_avl_gettagvalue_spec(TAG_VLPTransLimit, VLPTransLimit, 0, 6);

    dllemvb_avl_delete(TAG_VLPIndicator);

    if (pstIccTradeParam->bIsSupportEc) {
        if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD)     //ת�뿨Ȧ��
        {
            dllemvb_avl_set(TAG_VLPIndicator, 0x01);
        }
        else {
            rlt = Private_sdkIccMatchTagFromPDOL("\x9F\x7A");             // �ж��Ƿ�Ҫ������EC�ն�ָʾ��
            Trace("emv", "rlt = %d\r\n", rlt);
            TraceHex("emv", "gstSdkTermInfo.AmtAuthNum", AmtAuthNum, 6);
            TraceHex("emv", "gstSdkTermInfo.VLPTransLimit", VLPTransLimit, 6);
            Trace("emv", "pstIccTradeParam->bIsDefaultEc = %d\r\n", pstIccTradeParam->bIsDefaultEc);

//            if((rlt == 1) && (memcmp(pstTempterminfo->AmtAuthNum, pstTempterminfo->VLPTransLimit, 6) < 0) && pstTempterminfo->bTermSupportVLP)
            if ((rlt == 1) && (memcmp(AmtAuthNum, VLPTransLimit, 6) < 0)) {
                if ((gucSdkEC_IsMulAccountCard == 1) && pstIccTradeParam->bIsPromptChooseEc &&
                    (NULL != pstIccTradeParam->DispChooseEC)) {
                    rlt = (*pstIccTradeParam->DispChooseEC)();     //��ʾ�Ƿ�ѡ������ֽ�
                    Trace("emv", "pstIccTradeParam->DispChooseEC return value=%d\r\n", rlt);

                    if (rlt == SDK_OK) {
                        dllemvb_avl_set(TAG_VLPIndicator, 0x01);
                    }
                    else if (rlt == SDK_TIME_OUT) {
                        dllemvb_avl_set(TAG_VLPIndicator, pstIccTradeParam->bIsDefaultEc);
                    }
                    else {
                        dllemvb_avl_delete(TAG_VLPIndicator);
                    }
                }
                else {
                    dllemvb_avl_set(TAG_VLPIndicator, pstIccTradeParam->bIsDefaultEc);
                }
            }
        }
    }
//    if(SDK_ICC_ICC == pstIccParam->ucCardMode && EC_IsMulAccountCard == 0 && pstTempterminfo->VLPIndicator == 0
//        && (pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW || pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD))
//    { //��֧�ִ������ֽ��ܣ������Ǵ������ֽ𿨣��ܾ�����sjz 20120719
//        InitLocalVarible(&gstSdkAppUnion);
//        return SDK_ECNotSupportErr;
//    }

    while (1) {
        if (SDK_ICC_RF == pstIccParam->ucCardMode) {
            if (pstIccTradeParam->bIsSupportQPBOC == true) {
                retCode = qPBOCb_PreTrans(GlbEMVBApp_UnionStruct);                 // �ն˽�������
                Trace("emv", "qPBOC_PreTrans retCode = %02d\r\n", retCode);

                if (retCode != EMV_OK) {
                    sdkIccEmvToSdkRlt(retCode, &rlt);
                    return rlt;
                }
            }
            retCode = qPBOCb_InitialApp(GlbEMVBApp_UnionStruct);
            Trace("emv", "qPBOC_InitialApp retCode = %02d\r\n", retCode);
        }
        else {
            retCode = Emvb_InitialApp(GlbEMVBApp_UnionStruct);
            Trace("emv", "Emv_InitialApp retCode = %02d\r\n", retCode);
        }
        tagAIPflag = dllemvb_avl_checkiftagexist(TAG_AIP);
        tagAFLflag = dllemvb_avl_checkiftagexist(TAG_AFL);

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            if (SDK_ICC_RF == pstIccParam->ucCardMode) {
                if (pstIccTradeParam->bIsSupportQPBOC ==
                    false) // �ǽ�PBOC����,����EMV_OKΪ����,����qPBOC_USEPBOC������ȷ��,�ݲ�����MSD
                {
                    pstIccParam->ucCLType = 1;

                    if (tagAIPflag && tagAFLflag) { //sujianzhong 2013.04.28 17:9
                        retCode = EMV_OK; //�ǽ�PBOC
                    }
                    else {
                        retCode = EMV_ERR;
                    }
                }
                else     // QPBOC����,����EMV_OK��������
                {
                    sdkIccGetMagCardData();

                    if (GlbEMVBApp_UnionStruct->EMVTradeParam->qPBOCOrMSD == MSDTYPE)     // MSD
                    {
                        pstIccParam->ucCLType = 3;
                        gstSdkMagCardData.HandInput = 0;

                        if (tagAFLflag) {
                            retCode = OFFLINE_APPROVE;
                        }
                        else {
                            if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult == ONLINE_WAIT) {
                                retCode = ONLINE_WAIT;
                            }
                            else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                     OFFLINE_APPROVE) {
                                retCode = OFFLINE_APPROVE;
                            }
                            else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                     OFFLINE_DECLINE) {
                                retCode = OFFLINE_DECLINE;
                            }
                            else {
                                retCode = EMV_ERR;
                            }
                        }
                    }
                    else {
                        pstIccParam->ucCLType = 2;

                        /*=======BEGIN: sujianzhong 2013.08.12  14:32 modify===========*/
                        //qpbocû�з�9f 27��G101���и�ֵ�ˣ�3306rҲ�����︳ֵ
                        if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult == ONLINE_WAIT) {
                            dllemvb_avl_set(TAG_CryptInfo, 0x80);
                        }
                        else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                 OFFLINE_APPROVE) {
                            dllemvb_avl_set(TAG_CryptInfo, 0x40);
                        }
                        else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                 OFFLINE_DECLINE) {
                            dllemvb_avl_set(TAG_CryptInfo, 0x00);
                        }

                        /*====================== END======================== */
                        if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                            ONLINE_WAIT)             // ARQC
                        {
                            if (gucSdkEC_IsMulAccountCard ==
                                0)        // zcl 20111223 �������ֽ𷵻�ARQC��ʱ��ȥ�ж����
                            {
                                //zcl 20130809 Q������λһ����Ӧ��
                                dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
                                //end
                                GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult = OFFLINE_DECLINE;
                                return SDK_ReadECBalance;
                            }
                            else {
                                retCode = ONLINE_WAIT;
                            }
                        }
                        else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                 OFFLINE_APPROVE)      // TC
                        {
                            retCode = OFFLINE_APPROVE;
                        }
                        else if (GlbEMVBApp_UnionStruct->EMVTradeParam->TransResult ==
                                 OFFLINE_DECLINE)     // ACC
                        {
                            //zcl 20130809 Q������λһ����Ӧ��
                            dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
                            //end
                            retCode = OFFLINE_DECLINE;
                        }
                        else {
                            retCode = EMV_ERR;
                        }
                    }
                }
            }
            else {
                retCode = EMV_OK;
            }
        }
        else if (retCode == qPBOC_USEPBOC)              // �߷ǽ�PBOC
        {
            if ((SDK_ICC_RF == pstIccParam->ucCardMode) && pstIccTradeParam->bIsSupportCLPBOC) {
                pstIccParam->ucCLType = 1;                //jiguang 20120521  �߷ǽ�PBOCʱ��Ҫ��λ

                if (tagAIPflag && tagAFLflag) {
                    retCode = EMV_OK;
                }
                else {
                    retCode = EMV_ERR;
                }
            }
            else                // �ն˽��������в�֧��qPBOC�ҽ��׽��Ϊ0 �����ն˽�������֧��qPBOC
                // GPO֮��Ƭ���ص�������AppCrypt  AIP  ATC  IssuAppData ��һ��������ʱ
                // ����GPO����֮�󷵻�6984
            {
                retCode = qPBOC_USEPBOC;
            }
        }
        else if (retCode == ERR_NEEDMAGCARD) {
            retCode = ERR_NEEDMAGCARD;
        }
        else if (retCode == ERR_EMVINITAPP_NOTACCEPED)                    // ��Ҫ��ʾ������sjz
        {
            retCode = EMV_APPSELECTTRYAGAIN;
        }
        else if ((ERR_EMV_IccCommand != retCode) && (ERR_APP_TIMEOUT != retCode) &&
                 (ERR_EMV_IccDataFormat != retCode)
                 && (ERR_EMV_IccReturn != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        dllemvb_avl_gettagvalue_spec(TAG_qPBOCOfflineSpendAmount, qPBOCOfflineSpendAmount, 0, 6);
        TraceHex("GPO", "qpboc", qPBOCOfflineSpendAmount, 6);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR :trans error
                                SDK_OfflineApprove:qPBOC offline approve transflow
                                SDK_OfflineDecline:qPBOC offline decline transflow
                                SDK_IccCommandErr :read card error
                                SDK_AppTimeOutErr :receive data overtime
                                SDK_IccDataRedundErr :data redund
                                SDK_IccDataFormatErr :data format error
                                SDK_IccReturnErr :returned data error
                                SDK_ReadECBalance: read EC balance
                                SDK_EcLackBalanceErr:EC Account lack balance
                                SDK_PARA_ERR: parameter_input error
                                SDK_OnlineWait:qPBOC wait online transflow
                                SDK_QpbocReadLastLogErr:qPBOC read last log error
** Created By:		sujianzhong2012.08.09
** Remarks:		     //sxl20140408
*****************************************************************************/
s32 sdkIccReadAppData(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                      const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    //bool hasVerifyPAN = false; //sujianzhong 2013.07.18  17:46
    //u8 asPAN[64] = {0}; //sujianzhong 2013.07.18  17:46
    u8 AmtAuthNum[6] = {0};
    u8 VLPAvailableFund[6] = {0};

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }
    GlbEMVBApp_UnionStruct->VerifyCardNo = NULL;

    //�����ֽ��ڷ����һ��Read Record֮ǰҪ�ж��Ƿ��Ǻ�������
    //֮��Ҳ���ж�
    if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
        (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
        GlbEMVBApp_UnionStruct->VerifyCardNo = pstIccTradeParam->VerifyCardNo;
    }

    while (1) {
        if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
            (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
			retCode = qPBOCb_ReadAppData(GlbEMVBApp_UnionStruct);
            Trace("emv", "qPBOC_ReadAppData retCode = %02d\r\n", retCode);
        }
        else {
            retCode = Emvb_ReadAppData(GlbEMVBApp_UnionStruct);
            Trace("emv", "Emv_ReadAppData retCode = %02d\r\n", retCode);
        }

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR && retCode != ERR_LASTRECORD) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            Trace("emv", "--------------------------------\r\n");
            Trace("emv", "�������ֽ�? == %d\r\n\r\n", gucSdkEC_IsMulAccountCard == 0 ? 1 : 0);
            Trace("emv", "9F7A = %d,   9F74 = %d\r\n\r\n",
                  dllemvb_avl_gettagvalue(TAG_VLPIndicator),
                  dllemvb_avl_checkiftagexist(TAG_VLPIssuAuthorCode));
            //sxl?? ����ϸ���һ��TAG_VLPIssuAuthorCode������

            dllemvb_avl_gettagvalue_spec(TAG_AmtAuthNum, AmtAuthNum, 0, 6);
            dllemvb_avl_gettagvalue_spec(TAG_VLPAvailableFund, VLPAvailableFund, 0, 6);

            if (!((SDK_ICC_RF == pstIccParam->ucCardMode) && (pstIccParam->ucCLType == 2 ||
                                                              pstIccParam->ucCLType ==
                                                              3)))     //����qpboc,Ҳ����MSD,��Ҫ�Դ������ֽ�����ж�
            {
                if (gucSdkEC_IsMulAccountCard ==
                    0)                               // huxiang add 091014
                {
                    if (dllemvb_avl_checkiftagexist(TAG_VLPIssuAuthorCode) == 0) {
//                        if(pstTempterminfo->EmvNowTask != EMV_EC_CASH)     // huxiang add 091207 ֻ����ͨ���Ѳ�������
                        if (pstIccTradeParam->eFlowMode ==
                            SDK_ICC_ALL_FLOW)     //sujianzhong 2013.02.24 12:50
                        {
                            return SDK_ReadECBalance;                               // zcl 20111223 �������ֽ�û�з�����Ȩ�������Ҫȥ�����
                        }
                    }
//                    else if(memcmp(pstTempterminfo->AmtAuthNum, pstTempcardinfo->VLPAvailableFund, 6) >= 0)                                 // zcl 20111028
                    else if (memcmp(AmtAuthNum, VLPAvailableFund, 6) >
                             0)  //zxx 20130717 10:24 �޸ĽӴ������ֽ�����ȫ��������
                    {
//                        if(pstTempterminfo->EmvNowTask != EMV_EC_CASH)
                        if (pstIccTradeParam->eFlowMode ==
                            SDK_ICC_ALL_FLOW)     //sujianzhong 2013.02.24 12:50
                        {
                            return SDK_EcLackBalanceErr;
                        }
                    }
                }
            }
            sdkIccGetMagCardData();

            if ((SDK_ICC_RF == pstIccParam->ucCardMode) && (pstIccParam->ucCLType == 2 ||
                                                            pstIccParam->ucCLType ==
                                                            3))                               // qpboc�����ݳɹ�
            {
                if (gEMVTradeParam->qPBOCOrMSD == MSDTYPE) {
                    gstSdkMagCardData.HandInput = 0;
                    retCode = MSDb_ConstraintParam(GlbEMVBApp_UnionStruct);

                    Trace("emv", "MSD_ConstraintParam retcode = %d\r\n", retCode);

                    if (retCode == EMV_OK) {
                        if (gEMVTradeParam->TransResult == ONLINE_WAIT) {
                            retCode = ONLINE_WAIT;
                        }
                        else if (gEMVTradeParam->TransResult == OFFLINE_APPROVE) {
                            retCode = OFFLINE_APPROVE;
                        }
                        else if (gEMVTradeParam->TransResult == OFFLINE_DECLINE) {
                            retCode = OFFLINE_DECLINE;
                        }
                        else {
                            retCode = EMV_ERR;
                        }
                    }
                    else {
                        retCode = EMV_ERR;
                    }
                }
                else {
                    retCode = OFFLINE_APPROVE;
                }
            }
        }

        else if (retCode == qPBOC_APPEFFECT || retCode == qPBOC_APPEXPIRED) {
            Trace("emv", "��Ƭδ��Ч������ʧЧ\r\n");
            gEMVTradeParam->TransResult = OFFLINE_DECLINE;
            retCode = OFFLINE_DECLINE;
            // qpboc�ѻ��ܾ�
            sdkIccEmvToSdkRlt(retCode, &rlt);
            return rlt;
        }

        else if (retCode == QPBOC_BLACKLISTCARD) {
            return SDK_ESC;
        }
        else if (retCode == ERR_LASTRECORD) {
            //zcl 20130809 Q������λһ����Ӧ��
            dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
            //end
            gEMVTradeParam->TransResult = OFFLINE_DECLINE;
            return SDK_QpbocReadLastLogErr;
        }
        else if ((ERR_EMV_IccCommand != retCode) && (ERR_APP_TIMEOUT != retCode) &&
                 (ERR_EMV_IccDataRedund != retCode)
                 && (ERR_EMV_IccDataFormat != retCode) && (ERR_EMV_IccReturn != retCode)) {
            retCode = EMV_ERR;
        }

        //zcl 20130809 Q������λһ����Ӧ��
        if (retCode != OFFLINE_APPROVE && (SDK_ICC_RF == pstIccParam->ucCardMode) &&
            (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
            dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
        }
        //end

        sdkIccEmvToSdkRlt(retCode, &rlt);

        //qpboc�ѻ�����ʱ������GPO����9F5D(���Ѻ�Ŀ������)
        if (dllemvb_avl_checkiftagexist(
                TAG_qPBOCOfflineSpendAmount)) // zcl 20111223 ����Ƭ������9F 5D������ȥȡ9F 79
        {
            dllemvb_avl_gettagvalue_spec(TAG_qPBOCOfflineSpendAmount, VLPAvailableFund, 0, 6);
            memset(VLPAvailableFund, 0, sizeof(VLPAvailableFund));
            dllemvb_avl_createsettagvalue(TAG_VLPAvailableFund, VLPAvailableFund, 6);
        }
        return rlt;
    }
}


s32 manage_capkread(u32 id, CAPK_STRUCT *tmpcapk) {
    u32 addr;
    s32 ret;
    u8 *tmp;       //[CAPKLEN]
    u8 checksum;
    u8 fn[SDK_EMV_MAX_PATH];
    s32 readfilelen;

    memset(fn, 0, SDK_EMV_MAX_PATH);             //sxl? ����Щͳһ�ŵ�һ��������
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");

    tmp = (u8 *) emvb_malloc(CAPKLEN);

    addr = id << CAPKSHIFTNUM;
    readfilelen = CAPKLEN;
    ret = sdkReadFile(fn, tmp, addr, &readfilelen);

    if (ret != SDK_FILE_OK) {
        emvb_free(tmp);
        return 1;
    }
    checksum = sdkCalcCrc16(tmp, CAPKLEN - 1);

    if (checksum == tmp[CAPKLEN - 1]) {
        if (tmp[CAPKFLAGOFFSET] == CAPKENABLE) {
            memcpy((u8 *) tmpcapk, tmp, sizeof(CAPK_STRUCT));
            emvb_free(tmp);
            return 0;
        }
    }
    emvb_free(tmp);
    return 1;
}


void Emvb_SMReadCAPK(EMVBAppUnionStruct *tempApp_UnionStruct) {
    CAPK_STRUCT *tempcapk;
    LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;


    tempcapk = gEMVTradeParam->CAPK;
    tempselectedapp = gEMVTradeParam->SelectedApp;


    tempApp_UnionStruct->IPKModulLen = 0;
    tempApp_UnionStruct->ICCPKModulLen = 0;
    tempcapk->ModulLen = 0;


    ret = dllemvb_avl_gettagvalue_spec(TAG_CAPKI, &CAPKI, 0, 1);

    if (ret != 0) {
        return;
    }

    if (tempcapk->ModulLen == 0) {
        memcpy(RID, (unsigned char *) tempselectedapp->AID, 5);

        for (i = 0; i < MAXCAPKINDEX; i++) {
            if ((!memcmp(RID, gCapkIndex[i].RID, 5)) && CAPKI == gCapkIndex[i].CAPKI) {
                if (!gCapkIndex[i].SMCapkFlag) {
                    continue;
                }

                if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
                    manage_capkread((u32) i, tempcapk);
                    return;
                }
            }
        }
    }
}

void Emvb_ReadCAPK(EMVBAppUnionStruct *tempApp_UnionStruct) {
    CAPK_STRUCT *tempcapk;
    LISTAPPDATA *tempselectedapp;
    unsigned char i, RID[5];
    unsigned char CAPKI;
    unsigned char ret;


    tempcapk = gEMVTradeParam->CAPK;
    tempselectedapp = gEMVTradeParam->SelectedApp;


    tempApp_UnionStruct->IPKModulLen = 0;
    tempApp_UnionStruct->ICCPKModulLen = 0;
    tempcapk->ModulLen = 0;


    ret = dllemvb_avl_gettagvalue_spec(TAG_CAPKI, &CAPKI, 0, 1);

    if (ret != 0) {
        return;
    }

    if (tempcapk->ModulLen == 0) {
        memcpy(RID, (unsigned char *) tempselectedapp->AID, 5);

        for (i = 0; i < MAXCAPKINDEX; i++) {
            if ((!memcmp(RID, gCapkIndex[i].RID, 5)) && CAPKI == gCapkIndex[i].CAPKI) {
                if (gCapkIndex[i].SMCapkFlag) {
                    continue;
                }

                if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
                    manage_capkread(i, tempcapk);
                    return;
                }
            }
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_OnlineWait:
                                SDK_OfflineDecline:
                                SDK_OfflineApprove
                                SDK_IccCommandErr
                                SDK_IccDataFormatErr
                                SDK_AppTimeOutErr
                                SDK_IccReturnErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccDataAuth(const SDK_ICC_TRADE_PARAM *pstIccTradeParam, const SDK_ICC_PARAM *pstIccParam) {
    u8 retCode = EMV_ERR, checkcapkexpire = true;
    s32 rlt = SDK_ERR;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        /*=======BEGIN: sujianzhong 2013.08.02  17:25 modify===========*/
        if (checkcapkexpire == true &&
            sdkIccCheckCapkExpire() == EMV_ERR) //������Ч�ڣ�����������֤ʧ��
        {
            retCode = Private_sdkIccDataAuth(pstIccParam);
        }
        else
            /*====================== END======================== */
        {
            // checkcapkexpire = false;

            if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
                (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
                //zhouxiaoxin�����汻���ε���仰
                Emvb_ReadCAPK(GlbEMVBApp_UnionStruct);
                retCode = qPBOCb_DataAuth(GlbEMVBApp_UnionStruct);
                Trace("emv", "qPBOC_DataAuth retCode = %02d\r\n", retCode);
                //TraceHex("emv", "qPBOC_DataAuth TVR", pstTempterminfo->TVR, 5);
            }
            else {
                //sxl??�����㷨��Ҫ����,ѯ��һ��Ӧ�����ڹ��������ʵ�ֵ�
#if 0 /*Modify by ���ƽ at 2015.03.23  16:46 */
                Trace("emv", "qPBOC_DataAuth GlbEMVBApp_UnionStruct->EMVTradeParam->ODASMstaus = %d\r\n", GlbEMVBApp_UnionStruct->EMVTradeParam->ODASMstaus);

                if(GlbEMVBApp_UnionStruct->EMVTradeParam->ODASMstaus) //�����㷨
                {
                    Emvb_SMReadCAPK(GlbEMVBApp_UnionStruct);
                    retCode = SM_DataAuth(GlbEMVBApp_UnionStruct);
                }
                else
#endif /* if 0 */
                {
                    Emvb_ReadCAPK(GlbEMVBApp_UnionStruct);
                    retCode = Emvb_DataAuth(GlbEMVBApp_UnionStruct);
                }
                Trace("emv", "Emv_DataAuth retCode = %02d\r\n", retCode);
                //TraceHex("emv", "Emv_DataAuth TVR", pstTempterminfo->TVR, 5);
            }
        }

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
                (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) { //qpboc
                if (gEMVTradeParam->TransResult == ONLINE_WAIT) {
                    if (gucSdkEC_IsMulAccountCard == 0) //qpboc�ѻ����׶�̬������֤ʧ�ܣ���������
                    {
                        //zcl 20130809 Q������λһ����Ӧ��
                        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
                        //end
                        return SDK_OfflineDecline; //sujianzhong 2013.03.22 ���������ж��Ƿ�ܾ�//zxx 20130705 14:30����ܾ���������
                    }
                    retCode = ONLINE_WAIT;
                }
                else if (gEMVTradeParam->TransResult == OFFLINE_DECLINE) {
                    //zcl 20130809 Q������λһ����Ӧ��
                    dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
                    //end
                    retCode = OFFLINE_DECLINE;  //sxl?? �鿴һ�»᲻���г�ͻ
                }
                else {
                    //zcl 20130809 Q������λһ����Ӧ��
                    dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Y1", 2);
                    //end
                    retCode = OFFLINE_APPROVE;
                }
            }
            else {
                retCode = EMV_OK;
            }
        }
        else if ((ERR_EMV_IccCommand != retCode) && (ERR_EMV_IccDataFormat != retCode) &&
                 (ERR_APP_TIMEOUT != retCode)
                 && (ERR_EMV_IccReturn != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_AppEffectErr:application hasn't effected
                                SDK_AppExpiredErr:application has expired
                                SDK_OfflineApprove:
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccProcessRestrict(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 TVR[5];

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
        (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
        retCode = qPBOCb_CheckExpDate(GlbEMVBApp_UnionStruct);

        if (EMV_OK == retCode) {
            retCode = OFFLINE_APPROVE;
        }
            //zcl 20130809 Q������λһ����Ӧ��
        else {
            dllemvb_avl_createsettagvalue(TAG_AuthRespCode, "Z1", 2);
        }
        //end
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
        Trace("emv", "qPBOC_ProcessRestrict retCode = %02d\r\n", retCode);
        TraceHex("emv", "qPBOC_ProcessRestrict TVR", TVR, 5);
    }
    else {
        retCode = Emvb_ProcessRestrict(GlbEMVBApp_UnionStruct);
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
        Trace("emv", "Emv_ProcessRestrict retCode = %02d\r\n", retCode);
        TraceHex("emv", "Emv_ProcessRestrict TVR", TVR, 5);
    }

    if ((qPBOC_APPEFFECT == retCode) || (qPBOC_APPEXPIRED == retCode)) {
        gEMVTradeParam->TransResult = OFFLINE_DECLINE;
    }
    else if ((EMV_OK != retCode) && (OFFLINE_APPROVE != retCode)) {
        retCode = EMV_ERR;
    }
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

//sxl20140408
unsigned char sdkIccInputCreditPwd(unsigned char pintype, unsigned char *ASCCreditPwd) {
    unsigned char rslt;
    unsigned char tCreditPwd[64], tICCEncryptWay;
    s32 rlt;
    u8 PINTryCount;
	u8 tmp[128]={0};
	s32 len = 0;
	u8 data[128] ={0};
	s32 offset = 0;

    if (pintype == EMVB_DATAAUTH_INPUTPINSTATUS) {
        if (ASCCreditPwd[0] == 0x00) {
            if (GlbEMVBApp_UnionStruct->DispPromptData == NULL) {
                sdkIccPostOfflinePinSuc();
            }
            else {
                (*GlbEMVBApp_UnionStruct->DispPromptData)(SDK_ICC_OFFLINE_PIN_SUC);
            }
        }
        else {
            if (ASCCreditPwd[1] == 0)              //û��PIN���Դ�����
            {
                if (GlbEMVBApp_UnionStruct->DispPromptData == NULL) {
                    sdkIccPostOfflinePinErr();
                }
                else {
                    (*GlbEMVBApp_UnionStruct->DispPromptData)(SDK_ICC_OFFLINE_PIN_ERR);
                }
            }
            else {
                if (ASCCreditPwd[1] == 1)                  //ֻ��һ�λ���
                {
                    if (GlbEMVBApp_UnionStruct->DispPromptData == NULL) {
                        sdkIccPostInputPinAgain(1);
                    }
                    else {
                        (*GlbEMVBApp_UnionStruct->DispPromptData)(SDK_ICC_INPUT_PIN_AGAIN);
                    }
                }
                else {
                    if (GlbEMVBApp_UnionStruct->DispPromptData == NULL) {
                        sdkIccPostInputPinAgain(0);
                    }
                    else {
                        (*GlbEMVBApp_UnionStruct->DispPromptData)(SDK_ICC_INPUT_PIN_AGAIN);
                    }
                }
            }
        }
        return EMV_OK;
    }
    else if (pintype == EMVB_DATAAUTH_VERIFYIDCARD) {
        return sdkIccVerifyIdCard();
    }
    else if (pintype == EMVB_NEEDREFER) {
        sdkIccProcessRefer(1);

        if (dllemvb_avl_gettagvalue(TAG_ReferenceControlParameter) == 0x00) {
            return EMV_ERR;
        }
        else {
            return EMV_OK;
        }
    }
    memset(tCreditPwd, 0, sizeof(tCreditPwd));


    if (pintype == EMVB_DATAAUTH_OFFLINEENCPIN || pintype == EMVB_DATAAUTH_OFFLINEPLAINTEXTPIN) {
        tICCEncryptWay = ICCOFFLINEPIN;
    }
    else {
        tICCEncryptWay = ICCONLINEPIN;
    }

    if (GlbEMVBApp_UnionStruct->InputPWD == NULL) {
        rlt = SDK_OK;
    }
    else {
        PINTryCount = dllemvb_avl_gettagvalue(TAG_PINTryCount);

        if (GlbEMVBApp_UnionStruct->pasTradeAmount == NULL)        //sujianzhong 2013.01.10 9:18
        {
            rlt = (*GlbEMVBApp_UnionStruct->InputPWD)("\x00", PINTryCount, tICCEncryptWay,
                                                      tCreditPwd);
        }
        else {
            rlt = (*GlbEMVBApp_UnionStruct->InputPWD)(GlbEMVBApp_UnionStruct->pasTradeAmount,
                                                      PINTryCount, tICCEncryptWay, tCreditPwd);
        }
    }

    if (rlt != SDK_OK)   //ȡ��������
    {
        memset(tCreditPwd, 0, sizeof(tCreditPwd));
        return ERR_EMV_CancelTrans;
    }
    else {
        if (tCreditPwd[0] > 0) {
            rslt = EMV_OK;
			//zhouxiaoxin
            if (tICCEncryptWay == ICCOFFLINEPIN) {
                memcpy(ASCCreditPwd, &tCreditPwd[1], tCreditPwd[0]);
            }
            else {
                //memcpy(ASCCreditPwd,tCreditPwd,8);
            }
        }
        else                                                                                                            //BYPASS
        {
            rslt = ERR_EMV_InputCancel;
        }
        memset(tCreditPwd, 0, sizeof(tCreditPwd));

        return rslt;
    }
}
//zhouxiaoxin
int sdkIccGetOfflineEncdata(unsigned char *encdata, unsigned int dataLen)
{
	TraceHex("emv","getofflineencpin",encdata,dataLen);
	return ddi_pin_getofflineencpin(encdata,dataLen);
}
//xiaox 15/11/24
//---------start--------//
static u8 gIcKey[16] = {0};//缓存 tek2
static u8 gIcEnTrack[128]={0};//缓存返给后台的35域数据
#include "javaInterface.h"
extern JAVA_INTERFACE *javaInterface;
void sdkIccSetFactorAndIcKey(u8 *key)
{
	memset(gIcKey,0,sizeof(gIcKey));
	memcpy(gIcKey,key,sizeof(gIcKey));
        memset(gIcEnTrack,0,sizeof(gIcEnTrack));
	TraceHex("emv","import tek",gIcKey,16);
}
int sdkIccGetTrackData(unsigned char *magdata,unsigned int magdatalen)
{
    int ret = -1;
	u8 temp[128] = {0};
	u16 len = 0;
	u8 offset = 0;
	u8 icTrackEnByTek[64]={0};
	Trace("emv","enter sdkIccGetTrackData");
    jstring result = (*(javaInterface->myEnv))->CallObjectMethod(javaInterface->myEnv,
                                                                        javaInterface->gJniObj,
                                                                        javaInterface->jniIcTrack
    );
    char *pIcTrack = (*(javaInterface->myEnv))->GetStringUTFChars((javaInterface->myEnv), result, NULL);
    if (!memcmp(pIcTrack, "-1", 2)) {
         ret = -1;
    }else{
    	Trace("emv","respone IcTrack %s",pIcTrack);
    	if(strlen(pIcTrack) > 4){//xiaox 15/12/21 ���ӳ����ж�
         ret = 0;
		 sdkAscToBcd(temp,pIcTrack,strlen(pIcTrack));
		 offset = 0;
		 
		 sdkBcdToU16(&len,temp,2);
		 memset(gIcEnTrack,0,sizeof(gIcEnTrack));
		 memcpy(gIcEnTrack,&temp[offset],len+2);
		 TraceHex("emv","IcEntrack",gIcEnTrack,len+2);
		 offset += (len+2);

		 sdkBcdToU16(&len,&temp[offset],2);
		 offset += 2;
		 memcpy(icTrackEnByTek,&temp[offset],len);
		 offset += (len+2);
		 
		 sdkDes3S(false,&icTrackEnByTek[len-9],gIcKey);
		 memcpy(magdata,icTrackEnByTek,len);
		 TraceHex("emv","plainIcTrack",magdata,len);
		 
		
         memcpy(gstSdkMagCardData.Track2Data, icTrackEnByTek, len);
		 gstSdkMagCardData.Track2Len = (u8) len*2;
		 if((gstSdkMagCardData.Track2Data[len-1]&0x0f)==0x0f){
		 	gstSdkMagCardData.Track2Len--;
		 }
		 memset(temp, 0, sizeof(temp));
         gstSdkMagCardData.HaveICC = sdkIccGetMagCardNo(&gstSdkMagCardData,
                                                       temp);
    		}
    }
    (*(javaInterface->myEnv))->ReleaseStringUTFChars((javaInterface->myEnv), result,pIcTrack);
        return ret;
}
void sdkIccGetIcEnTrack(u8 *pOut){
	u16 len = 0;
	u8 temp[64]={0};
	EMVTAGCVLITEM *item = NULL;
	u16 i=0,j=0;
	
	sdkBcdToU16(&len,gIcEnTrack,2);
        //xiaox 15/12/17
        #ifndef SPI_DDI
	//xiaox 15/12/15 为了给 k100/k200返回密文,如果libemv 数据认证不用磁道数据,则要自己调用一次获取加密磁道数据
	if(len == 0){
		sdkIccGetTrackData(temp,0);
		sdkBcdToU16(&len,gIcEnTrack,2);
	}
        #endif
	memcpy(pOut,gIcEnTrack,len+2);
}
//------end-------//
/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_OnlineWait:
                                SDK_OfflineApprove
                                SDK_OfflineDecline:
                                SDK_CancelTransErr
                                SDK_IccCommandErr
                                SDK_IccReturnErr ��
                                SDK_AppTimeOutErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccCardHolderVerf(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                         const SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    bool bHasQpbocOnlinePin = false;
    u8 retCode = EMV_ERR;
    u8 iccEncryptWay, creditPwd[64];
    s32 rlt = SDK_ERR;
    u8 TVR[5];
    u8 PINTryCount;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD) {
        return SDK_OK;
    }
    GlbEMVBApp_UnionStruct->pasTradeAmount = pstIccTradeParam->pasTradeAmount;
    GlbEMVBApp_UnionStruct->InputPWD = pstIccTradeParam->InputPWD;
    GlbEMVBApp_UnionStruct->DispPromptData = pstIccTradeParam->DispPromptData;

    while (1) {
        if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
            (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
            retCode = qPBOCb_CardHolderVerf(GlbEMVBApp_UnionStruct);
            Trace("emv", "qPBOC_CardHolderVerf retCode = %02d\r\n", retCode);
            dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
            TraceHex("emv", "qPBOC_CardHolderVerf TVR", TVR, 5);

            //if(TVR[2]&0x04)  //����PIN
            //{
            //    bHasQpbocOnlinePin = true;  //�ѻ�������������PINû������ sxl?���ﲻ��������PIN,����ǿ��Ҫ������PIN
            //}
        }
        else {
            retCode = Emvb_CardHolderVerf(GlbEMVBApp_UnionStruct);
            Trace("emv", "Emv_CardHolderVerf retCode = %02d\r\n", retCode);
            dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
            TraceHex("emv", "Emv_CardHolderVerf TVR", TVR, 5);
        }

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            if ((SDK_ICC_RF == pstIccParam->ucCardMode) && (pstIccParam->ucCLType == 2 ||
                                                            pstIccParam->ucCLType ==
                                                            3))                                           // qpboc
            {
                //Trace("emv", "qPBOC_CardHolderVerf  pstTempterminfo->TransResult= %02d\r\n", pstTempterminfo->TransResult);

                if (gEMVTradeParam->TransResult == ONLINE_WAIT) {
                    memset(TVR, 0, sizeof(TVR));
                    dllemvb_avl_createsettagvalue(TAG_TVR, TVR, 5);

                    if (bHasQpbocOnlinePin == false) {
                        iccEncryptWay = ICCONLINEPIN;
                        memset(creditPwd, 0, sizeof(creditPwd));

                        if (pstIccTradeParam->InputPWD != NULL) {
                            //PINTryCount = dllemvb_avl_gettagvalue(TAG_PINTryCount);  //sxl?�鿴������������δ����
                            PINTryCount = 5;

                            if (pstIccTradeParam->pasTradeAmount ==
                                NULL)                            //sujianzhong 2013.01.10 9:18
                            {
                                rlt = (*pstIccTradeParam->InputPWD)("\x00", PINTryCount,
                                                                    iccEncryptWay, creditPwd);
                            }
                            else {
                                rlt = (*pstIccTradeParam->InputPWD)(
                                        pstIccTradeParam->pasTradeAmount, PINTryCount,
                                        iccEncryptWay, creditPwd);
                            }

                            if (rlt != SDK_OK) {
                                return rlt;
                            }
                        }
                    }
                    retCode = ONLINE_WAIT;                                           // qpboc ��������
                }
                else if (gEMVTradeParam->TransResult == OFFLINE_APPROVE) {
                    //gEMVTradeParam->TransResult = 0;		//zcl delete
                    retCode = OFFLINE_APPROVE;                                           // qpboc �ѻ���׼,�����ߴ�����������
                }
                else if (gEMVTradeParam->TransResult == OFFLINE_DECLINE) {
                    retCode = OFFLINE_DECLINE;                                           // qpboc�ѻ��ܾ�,�߽��״�������
                }
                else {
                    retCode = EMV_ERR;
                }
            }
        }
        else if ((ERR_EMV_CancelTrans != retCode) && (ERR_EMV_IccCommand != retCode) &&
                 (ERR_EMV_IccReturn != retCode)
                 && (ERR_APP_TIMEOUT != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_ShowRandNum
                                SDK_IccCommandErr
                                SDK_AppTimeOutErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccTermRiskManage(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                         const SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    u8 retCode = EMV_ERR;
//    u32 m;
    s32 rlt = SDK_ERR;
//    unsigned int amt = 0, oldamount, MinLimit;
//    u8 buf[64] = {0}, temp[256];
    u8 AIP[2], TVR[5];

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (dllemvb_avl_checkiftagexist(TAG_VLPIssuAuthorCode))  //bCardConfirmVLP = 1
    {
        return SDK_OK;
    }
    memset(AIP, 0, sizeof(AIP));
    dllemvb_avl_gettagvalue_spec(TAG_AIP, AIP, 0, 2);

    if (AIP[0] & 0x08) {
        //if(gEMVTradeParam->IfAppCheckFloorLimit == 1)
        {
            //if(true == sdkIccCheckFloorLimit(&gstSdkAppUnion))  //sxl?��Ҫ���´���
            {
                //pstTempterminfo->IfTransOverFloorLimit = 1;   //sxl?
            }
        }
#if 0 //del by sujianzhong 20120926
        MinLimit = EMVByteArrayToInt(pstTempterminfo->FloorLimit, 4);
        memset(buf, 0, sizeof(buf));
        memset(temp, 0, sizeof(temp));
        sdkIccGetIccMainAcc(buf);
        sdkBcdToU32(&m, buf, 1);
        sdkBcdToAsc(temp, &buf[1], (m + 1) / 2);
        memset(buf, 0, sizeof(buf));

        if(NULL != pstIccTradeParam->GetPosTradeLogAmount)
        {
            rlt = (*pstIccTradeParam->GetPosTradeLogAmount)(buf, temp, m);

            if(SDK_OK != rlt)
            {
                return rlt;
            }
        }
//        GetPosFloorLimit(buf , temp , m);
        EMVBcdToU32(&oldamount, buf, 6);
        EMVBcdToU32(&amt, pstTempterminfo->AmtAuthNum, 6);
        amt += oldamount;                                                        // �ӵ�ǰ�Ľ��

        if(amt >= MinLimit)
        {
            if(pstTempterminfo->bCardConfirmVLP == 0)                                           // huxiang add 091023
            {
                pstTempterminfo->TVR[3] |= 0x80;                                           // set 'transaction exceeds floor limit' bit 1.
                // �ý��׽�����Ϊ'1;
            }
        }
#endif
    }

    while (1) {
        retCode = Emvb_TermRiskManage(GlbEMVBApp_UnionStruct);

        Trace("emv", "Emv_TermRiskManage retCode = %02d\r\n", retCode);
        memset(AIP, 0, sizeof(AIP));
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
        TraceHex("emv", "Emv_TermRiskManage TVR", TVR, 5);


        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            if (dllemvb_avl_gettagvalue(TAG_bShowRandNum)) {
                retCode = EMV_SHOWRANDNUM;
            }
        }

        else if ((ERR_EMV_IccCommand != retCode) && (ERR_APP_TIMEOUT != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_NeedMagCardErr
                                SDK_OK
                                SDK_IccCommandErr
                                SDK_IccReturnErr
                                SDK_IccDataFormatErr
                                SDK_AppTimeOutErr
                                SDK_NotAcceptErr
                                SDK_ERR
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:              sxl20140408
*****************************************************************************/
s32 sdkIccTermiAnalys(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                      const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 TVR[5] = {0};
    u8 TSI[2] = {0};

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        retCode = Emvb_TermiAnalys(GlbEMVBApp_UnionStruct);

        Trace("emv", "Emv_TermiAnalys retCode = %02d\r\n", retCode);
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
        dllemvb_avl_gettagvalue_spec(TAG_TSI, TSI, 0, 2);
        TraceHex("emv", "Emv_TermiAnalys TVR", TVR, 5);
        TraceHex("emv", "Emv_TermiAnalys TSI", TSI, 2);

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            retCode = EMV_OK;
        }
#ifdef EMV_LEVEL2TEST
            else if(retCode == ERR_NEEDMAGCARD)
        {
            retCode = ERR_EMV_IccCommand;
        }
#endif
// SWA��SWB ����6985 ������˼��ʹ�ô��� �ڳ��������ǰ�ʧ�ܴ����jiguang 20120430
        else if (retCode == ERR_NEEDMAGCARD) {
            retCode = ERR_NEEDMAGCARD;
        }
        else if ((ERR_EMV_IccCommand != retCode) && (ERR_EMV_IccReturn != retCode) &&
                 (ERR_EMV_IccDataFormat != retCode)
                 && (ERR_APP_TIMEOUT != retCode) && (ERR_EMV_NotAccept != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OnlineWait
                                SDK_OK
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_CDAFormatErr
                                SDK_AppTimeOutErr
                                SDK_IccReturnErr
                                SDK_IccDataFormatErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:               sxl20140408
*****************************************************************************/
s32 sdkIccTransAnalys(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                      const SDK_ICC_PARAM *pstIccParam)     //sjz 20120726
{
    u8 retCode = EMV_ERR;
    u8 iccEncryptWay, creditPwd[64];
    s32 rlt = SDK_ERR;
    //u8 Ec_LessThanResetAmount;
    u8 TVR[5] = {0};


    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        retCode = Emvb_TransProcess(GlbEMVBApp_UnionStruct);

        Trace("emv", "Emv_TransProcess retCode = %02d\r\n", retCode);
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
        TraceHex("emv", "Emv_TransProcess TVR", TVR, 5);
        Trace("emv", "gstSdkTermInfo.TransResult =%02d\r\n", gEMVTradeParam->TransResult);

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            if (Emvb_Ec_LessThanResetAmount() &&
                gEMVTradeParam->TransResult == ONLINE_WAIT) // huxiang modify for EC 091010
            {
//                return EMV_REQUESTONLINEENCPIN; // �˴��ǵ����ֽ����˺ŵ����� ӦΪ��������PIN
                if (!(gucSdkEC_IsMulAccountCard == 0 &&
                      pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW)) {
                    iccEncryptWay = ICCONLINEPIN;
                    memset(creditPwd, 0, sizeof(creditPwd));

                    if (pstIccTradeParam->InputPWD == NULL) {
                        rlt = SDK_OK;
                    }
                    else {
                        if (pstIccTradeParam->pasTradeAmount ==
                            NULL)    //sujianzhong 2013.01.10 9:19
                        {
                            rlt = (*pstIccTradeParam->InputPWD)("\x00", 5, iccEncryptWay,
                                                                creditPwd);     //sxl��������PIN������PIN���Դ���
                        }
                        else {
                            rlt = (*pstIccTradeParam->InputPWD)(pstIccTradeParam->pasTradeAmount, 5,
                                                                iccEncryptWay, creditPwd);
                        }
                    }

                    if (rlt == SDK_OK) {
#if 0

                        if(creditPwd[0] > 0)     //creditPwdΪASCII��
                        {
                            pstTempterminfo->inputpinstatus = EMV_OK;
                            pstTempterminfo->PINLen = 0;
                        }
                        else                                                                                                                            // BYPASS
                        {
                            pstTempterminfo->inputpinstatus = ERR_EMV_InputCancel;
                            //                        return EMV_ERR;
                        }
#endif
                    }
                    else {
                        return rlt;
                    }
                }
            }

            if (gEMVTradeParam->TransResult == ONLINE_WAIT) {
                //�ŵ�������ʾ
                /*=======BEGIN: sujianzhong 2013.08.12  15:27 modify===========*/
                if (gucSdkEC_IsMulAccountCard == 0 &&
                    pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW) //���Ѵ�����������ʱ���ܾ�������
                {
                    gEMVTradeParam->TransResult = OFFLINE_DECLINE;
                    retCode = EMV_OK;
                }
                else
                    /*====================== END======================== */
                {
                    retCode = ONLINE_WAIT;
                }
            }
            else {
                retCode = EMV_OK;
            }
//����������ֽ��ڿ�������ٶ�һ�����������ж��Ƿ��Ѿ���ȡ���//sjz 20121122
            //Trace("�����","pstIccTradeParam->eFlowMode=%d,pstTempterminfo->VLPIndicator=%d,pstTempterminfo->bCardConfirmVLP=%d\r\n",pstIccTradeParam->eFlowMode,pstTempterminfo->VLPIndicator,pstTempterminfo->bCardConfirmVLP);
#if 0 /*Modify by zxx at 2013.07.12  15:44 */

            if(pstTempterminfo->bCardConfirmVLP && pstTempcardinfo->IssuAppDataLen <= 9)
            { //��Ƭ��׼�����ֽ𣬲���9F10û�з�����Ҫ�ٶ�һ�����
                rlt = sdkIccReadEcData(pstIccTradeParam, pstIccParam);

                if((rlt != SDK_OK) && (rlt != SDK_EcLackBalanceErr))
                {
                    return SDK_ERR;
                }
            }
#endif /* if 0 */

//zxx 20130712 15:44
            if (dllemvb_avl_checkiftagexist(TAG_VLPIssuAuthorCode)) {
                if (dllemvb_avl_checkiftagexist(TAG_VLPAvailableFund) == 0)  //sxl?ȷ�Ϻ������Ƿ��д���
                {
                    rlt = sdkIccReadEcData(pstIccTradeParam, pstIccParam);

                    if ((rlt != SDK_OK) && (rlt != SDK_EcLackBalanceErr)) {
                        return SDK_ERR;
                    }
                }
            }
        }
        else if (retCode ==
                 ERR_NEEDMAGCARD)                                                          // �ڶ���GAC������ˢ������
        {
            retCode = ERR_EMV_IccCommand;
        }
        else if ((ERR_EMV_CDAFORMAT != retCode) && (ERR_EMV_IccCommand != retCode) &&
                 (ERR_APP_TIMEOUT != retCode)
                 && (ERR_EMV_IccReturn != retCode) && (ERR_EMV_IccDataFormat != retCode)) {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);

        return rlt;
    }
}

//20131023
unsigned char qPBOCb_OnLineDeal(EMVBAppUnionStruct *tempApp_UnionStruct) {
    unsigned char AuthRespCode[2];


    memset(AuthRespCode, 0, sizeof(AuthRespCode));
    dllemvb_avl_gettagvalue_spec(TAG_AuthRespCode, AuthRespCode, 0, 2);

#ifdef emvb_debug
    printf("This is OnOnLineDeal AuthRespCode=%02X %02X \r\n", AuthRespCode[0], AuthRespCode[1]);
#endif

    if (!memcmp(AuthRespCode, "00", 2) ||
        !memcmp(AuthRespCode, "10", 2) ||
        !memcmp(AuthRespCode, "11",
                2)) //Issuer approval,according to p172 of VIS140 terminal specification.
    {
        //�����������׼����(��Ȩ��Ӧ��Ϊ'00','10','11')ʱ,�ն�����TC
        tempApp_UnionStruct->EMVTradeParam->TransResult = ONLINE_APPROVE;
    }
    else if (!memcmp(AuthRespCode, "01", 2) ||
             !memcmp(AuthRespCode, "02", 2)) //Issuer referral
    {
        //�������������ο�(��Ȩ��Ӧ��Ϊ'01','02')ʱ,
        //�ն���ʾ����Ա��绰��Ȩ,���ݷ�������Ȩ���(��׼��ܾ�)
        //��������Ӧ������.����ն˲�֧�ֲο�,���ն�����AAC
        tempApp_UnionStruct->EMVTradeParam->IfNeedRefer = 1;
        return EMV_ERR;
    }
    else //Other codes are looked as Issuer decline.
    {   //�����Ȩ��δָ����׼��ο�,���ն�����AAC.
        tempApp_UnionStruct->EMVTradeParam->TransResult = ONLINE_DECLINE;
    }
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_NeedReversal
                                SDK_OK
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_CDAFormatErr:
                                SDK_AppTimeOutErr
                                SDK_ECLoadErr
                                SDK_PARA_ERR
                                SDK_Script72TimeOut
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccTransProcess(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                       const SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
	int i = 0;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam) || (GlbEMVBApp_UnionStruct == NULL) ||
        (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }
	
    while (1) {
        if (gEMVTradeParam->TransResult ==
            ONLINE_WAIT)                                                  // ���׵ȴ�
        {
            if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
                (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) {
                if (gEMVTradeParam->onlinetradestatus != EMV_OK) {
                    gEMVTradeParam->TransResult = ONLINE_DECLINE;
                    retCode = EMV_OK;
                }
                else {
                    retCode = qPBOCb_OnLineDeal(GlbEMVBApp_UnionStruct);
                }
            }
            else {
                Trace("emv", "sdkIccTransProcess gstSdkTermInfo.inputpinstatus = %02d\r\n",
                      gEMVTradeParam->onlinetradestatus);

                if (gEMVTradeParam->onlinetradestatus != EMV_OK) {
                    retCode = Emvb_UnableGoOnlineDeal(GlbEMVBApp_UnionStruct);

                    Trace("emv", "Emv_UnableGoOnlineDeal retCode = %02d\r\n", retCode);
                    Trace("emv", "Emv_UnableGoOnlineDeal transresult = %02d\r\n",
                          gEMVTradeParam->TransResult);
                }
                else {
                    retCode = Emvb_OnLineDeal(GlbEMVBApp_UnionStruct);

                    if (gEMVTradeParam->bReversal) {
                        //(temptermdatatable + MV_TVR - TermDataBase)->bExist = 1;  //sxl
                    }
                    Trace("emv", "Emv_OnLineDeal retCode = %02d\r\n", retCode);
                    Trace("emv", "Emv_OnLineDeal transresult = %02d\r\n",
                          gEMVTradeParam->TransResult);
                }

                if (retCode == EMV_OK) {
                    while (1) {
                        //���￨û�з������ݵĻ���ֻ�ǽű�����ʧ�ܣ����׻���û������

                        retCode = Emvb_PreTransComplete(GlbEMVBApp_UnionStruct);
                        Trace("emv", "Emv_PreTransComplete retCode = %02d\r\n", retCode);
                        //�ж�Ȧ�洦�������  71 72��Ҫ�ж�

                        if (EC_QUANCUNERR == retCode) {
                            //(temptermdatatable + MV_TVR - TermDataBase)->bExist = 1;
                            gEMVTradeParam->TransResult = ONLINE_DECLINE;
                        }
                        else if ((EMV_OK != retCode) && (EMV_NEEDREVERSAL !=
                                                         retCode))                                           // ֻ�е����ֽ�Ȧ��ʱ�ŷ���EMV_NEEDREVERSAL
                        {
                            retCode = EMV_ERR;
                        }
                        else {
                            Trace("�����",
                                  "pstIccTradeParam->eFlowMode=%d,pstTempterminfo->VLPIndicator=%d\r\n",
                                  pstIccTradeParam->eFlowMode,
                                  dllemvb_avl_gettagvalue(TAG_VLPIndicator));

                            if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD &&
                                dllemvb_avl_gettagvalue(TAG_VLPIndicator)) {
                                rlt = sdkIccReadEcData(pstIccTradeParam,
                                                       pstIccParam); //sujianzhong 2013.03.06 16:57

                                if ((rlt == SDK_OK) || (rlt == SDK_EcLackBalanceErr)) {
                                    retCode = EMV_OK;
                                }
                                else {
                                    retCode = EMV_ERR;
                                }
                            }
                        }
                        break;
                    }
                }
                else if (retCode ==
                         EMV_NEEDREVERSAL)                                                                 // huxiang modify for EC 091010
                {
                    retCode = EMV_NEEDREVERSAL;
                }
                else if (retCode ==
                         EC_QUANCUNERR)                                           // huxiang add 091127
                {
                    gEMVTradeParam->TransResult = ONLINE_DECLINE;
                    retCode = EC_QUANCUNERR;
                }
                else if (retCode == EMV_NEEDREFER)   // �����Ѿ����˴��������ٷ������ֵ
                {
                    // ��Ҫ��֤�Ƿ�������???
//                        sdkIccProcessRefer(ISSUER_REFER);
//                        continue;
                    return SDK_NeedRefer; //���ﲻ��Ҫ InitLocalVarible(&gstSdkAppUnion);
                }
                else if ((ERR_EMV_IccCommand != retCode) && (ERR_EMV_CDAFORMAT != retCode) &&
                         (ERR_APP_TIMEOUT != retCode)) {
                    retCode = EMV_ERR;
                }
            }
            sdkIccEmvToSdkRlt(retCode, &rlt);

#if 0                           //EMV���Ѿ����ű�������������
            //����ű����
            pstTempterminfo->ScriptResultDataLen = pstTempterminfo->IssuScriptNum * 5;

            if(pstTempterminfo->ScriptResultDataLen > 20)
            {
                pstTempterminfo->ScriptResultDataLen = 20;
            }
            memcpy(pstTempterminfo->ScriptResultData, pstTempterminfo->IssuScriptResult, pstTempterminfo->ScriptResultDataLen);
#endif

            return rlt;
        }
        else {
            return SDK_OK;
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:	SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr
                                SDK_TransOK_ARPCErr
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccTransComplete(void) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 TVR[5] = {0};


    if (gEMVTradeParam != NULL) {
        if (gEMVTradeParam->TransResult == ONLINE_APPROVE) {
            sdkIccSaveEmvTransLog();
            retCode = TRANS_ONLINE_APPROVE;
        }
        else if (gEMVTradeParam->TransResult == ONLINE_DECLINE) {
            retCode = TRANS_ONLINE_DECLINE;
        }
        else if (gEMVTradeParam->TransResult == OFFLINE_APPROVE) {
            sdkIccSaveEmvTransLog();
            retCode = TRANS_OFFLINE_APPROVE;
        }
        else if (gEMVTradeParam->TransResult == OFFLINE_DECLINE) {
            retCode = TRANS_OFFLINE_DECLINE;
        }
        else if (gEMVTradeParam->TransResult == TERMINATE_TRANS_NOTACCEPT) {
            retCode = TRANS_TERMINATE_TRANS_NOTACCEPT;
        }
        else {
            retCode = TRANS_TERMINATE_TRANS_ERR;
        }
        memset(TVR, 0, sizeof(TVR));
        dllemvb_avl_gettagvalue_spec(TAG_TVR, TVR, 0, 5);
    }
    sdkIccEmvToSdkRlt(retCode, &rlt);

    if (rlt == SDK_TransOnlineApprove) {
        if (TVR[4] & 0x40)   //��������֤����Ƭ�����׳ɹ�
        {
            rlt = SDK_TransOK_ARPCErr;
        }
    }
    return rlt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_EcLackBalanceErr
                                SDK_OfflineDecline
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_PARA_ERR
                                SDK_ECNotSupportErr
** Created By:		sujianzhong2012.08.09
** Remarks:             //sxl20140408     //sxl?��ȡEC���ݵ���ô�����ͷ��ڴ������
                                                                 �鿴Ӧ�ó����񵥶�����
*****************************************************************************/
s32 sdkIccReadEcData(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                     const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;
    u8 balancevalue[6] = {0}, balancevalueflag = 0;
    u8 AmtAuthNum[6] = {0};

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        //sxl? TAG_qPBOCOfflineSpendAmount�������ﷵ�ص�
        //zxx 20130712 13:34Ȧ��Ҫ�ٶ���������ԭ����
        balancevalueflag =
                1 - dllemvb_avl_gettagvalue_spec(TAG_qPBOCOfflineSpendAmount, balancevalue, 0, 6);

        if (balancevalueflag && pstIccTradeParam->eFlowMode !=
                                SDK_ICC_EC_LOAD)       // zcl 20111223 ����Ƭ������9F 5D������ȥȡ9F 79
        {
            dllemvb_avl_createsettagvalue(TAG_VLPAvailableFund, balancevalue, 6);
            retCode = EMV_OK;
        }
        else {
            retCode = Emvb_ReadEcData(GlbEMVBApp_UnionStruct);
        }
        Trace("emv", "Emv_ReadEcData retCode = %d\r\n", retCode);
        //TraceHex("emv", "gstSdkCardInfo.VLPAvailableFund", pstTempcardinfo->VLPAvailableFund, 6);


        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            dllemvb_avl_gettagvalue_spec(TAG_VLPAvailableFund, balancevalue, 0, 6);
            dllemvb_avl_gettagvalue_spec(TAG_AmtAuthNum, AmtAuthNum, 0, 6);

            if (gEMVTradeParam->EmvNowTask ==
                SDK_ICC_EC_QUERY)                                           //��ѯ���
            {
                retCode = EMV_OK;
            }
            else if (memcmp(AmtAuthNum, balancevalue, 6) >
                     0)                                             // zcl 20111223 >=��Ϊ>
            {
                return SDK_EcLackBalanceErr;
            }
            else if (gEMVTradeParam->TransResult == OFFLINE_DECLINE) {
                retCode = OFFLINE_DECLINE;
            }
            else {
                retCode = EMV_OK;
            }
        }
        else {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          APDU_SEND * apdu_s:
** Returned value:	EMV_PARA_ERR
                                EMV_OK
** Created By:		sujianzhong2012.08.09
** Remarks:    //sxl20140408
*****************************************************************************/
u8 sdkIccCmdReadECSecCurrencyCode(APDU_SEND *apdu_s) {
    memcpy(apdu_s->Command, "\x80\xCA\xDF\x71", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;
    apdu_s->EnableCancel = 1;    //Le=0(256)
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          AppUnionStruct * tempApp_UnionStruct:
** Returned value:	EMV_PARA_ERR
                                ERR_EMV_IccCommand
                                EMV_OK
                                ERR_APP_TIMEOUT
** Created By:		sujianzhong2012.08.09
** Remarks:        //sxl20140408
*****************************************************************************/
u8 sdkIccReadEcSecCurrencyCodeDetail(const EMVBAppUnionStruct *tempApp_UnionStruct) {
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;

    sdkIccCmdReadECSecCurrencyCode(&apdu_s);
    tempApp_UnionStruct->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

    if (apdu_r.ReadCardDataOk == COMMANDERR)                                          //����ʧ��
    {
        return ERR_EMV_IccCommand;
    }
    else if (apdu_r.ReadCardDataOk == COMMTIMEOUT)                                      //������ʱ
    {
        return ERR_APP_TIMEOUT;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) {
        if (!memcmp(apdu_r.DataOut, "\xDF\x71", 2)) {
            gucSdkEcSecCurrencyCode[0] = 1;
            memcpy(&gucSdkEcSecCurrencyCode[1], (u8 * ) & apdu_r.DataOut[3], 2);
        }
        return EMV_OK;
    }
    return EMV_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_EcLackBalanceErr
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccReadEcSecCurrencyCode(const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        retCode = sdkIccReadEcSecCurrencyCodeDetail(
                GlbEMVBApp_UnionStruct);                                           //zcl 20111028 ����һ���ֻ��Ҵ���

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            retCode = EMV_OK;
        }
        else                                           //zcl 20111028 ����һ���ֻ��Ҵ���ʧ��
        {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          APDU_SEND * pstApdu:
** Returned value:	EMV_PARA_ERR
                                EMV_OK
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl20140408
*****************************************************************************/
u8 sdkIccCmdReadECSecCashBalance(APDU_SEND *pstApdu) {
    memcpy(pstApdu->Command, "\x80\xCA\xDF\x79", 4);
    pstApdu->Lc = 0;
    pstApdu->Le = 256;                                           //Le=0(256)
    pstApdu->EnableCancel = 1;
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          AppUnionStruct * tempApp_UnionStruct:
** Returned value:	EMV_PARA_ERR
                                ERR_EMV_IccCommand
                                EMV_OK
                                ERR_APP_TIMEOUT
** Created By:		sujianzhong2012.08.09
** Remarks:            //sxl20140408
*****************************************************************************/
u8 sdkIccReadEcSecDataDetail(const EMVBAppUnionStruct *tempApp_UnionStruct) {
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;

    sdkIccCmdReadECSecCashBalance(&apdu_s);
    tempApp_UnionStruct->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

    if (apdu_r.ReadCardDataOk == COMMANDERR)                                          //����ʧ��
    {
        return ERR_EMV_IccCommand;
    }
    else if (apdu_r.ReadCardDataOk == COMMTIMEOUT)                                      //������ʱ
    {
        return ERR_APP_TIMEOUT;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) {
        if (!memcmp(apdu_r.DataOut, "\xDF\x79", 2)) {
            memcpy(gucSdkCardInfoSecVLPAvailableFund, (u8 * ) & apdu_r.DataOut[3],
                   6);  //sxl?��Щ�Ƿ��������TAG
        }
        return EMV_OK;
    }
    return EMV_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_EcLackBalanceErr
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccReadEcSecData(const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        retCode = sdkIccReadEcSecDataDetail(
                GlbEMVBApp_UnionStruct);                                           //zcl 20111028 ����һ���ֻ��Ҵ���

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            retCode = EMV_OK;
        }
        else                                           //zcl 20111028 ����һ���ֻ��Ҵ���ʧ��
        {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          APDU_SEND * apdu_s:
** Returned value:	EMV_PARA_ERR
                                EMV_OK
** Created By:		sujianzhong2012.08.09
** Remarks:                //sxl20140408
*****************************************************************************/
u8 sdkIccCmdReadECCurrencyCode(APDU_SEND *apdu_s) {
    memcpy(apdu_s->Command, "\x80\xCA\x9F\x51", 4);
    apdu_s->Lc = 0;
    apdu_s->Le = 256;                                           //Le=0(256)
    apdu_s->EnableCancel = 1;
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          AppUnionStruct * tempApp_UnionStruct:
** Returned value:	EMV_PARA_ERR
                                ERR_EMV_IccCommand
                                EMV_OK
                                ERR_APP_TIMEOUT
** Created By:		sujianzhong2012.08.09
** Remarks:		    //sxl20140408
*****************************************************************************/
u8 sdkIccReadEcFstCurrencyCodeDetail(const EMVBAppUnionStruct *tempApp_UnionStruct) {
        //zhouxiaoxin //need fix
    APDU_SEND apdu_s;
    APDU_RESP apdu_r;

    sdkIccCmdReadECCurrencyCode(&apdu_s);
    tempApp_UnionStruct->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);
    if (apdu_r.ReadCardDataOk == COMMANDERR)                                          //����ʧ��
    {
        return ERR_EMV_IccCommand;
    }
    else if (apdu_r.ReadCardDataOk == COMMTIMEOUT)                                      //������ʱ
    {
        return ERR_APP_TIMEOUT;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) {
        if (!memcmp(apdu_r.DataOut, "\x9F\x51", 2)) {
            gucSdkEcCurrencyCode[0] = 1;
            memcpy(&gucSdkEcCurrencyCode[1], (u8 * ) & apdu_r.DataOut[3], 2);
        }
        return EMV_OK;
    }
    return EMV_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_EcLackBalanceErr
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_PARA_ERR
** Created By:		sujianzhong2012.08.09
** Remarks:             //sxl20140408
*****************************************************************************/
s32 sdkIccReadEcFstCurrencyCode(const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }

    while (1) {
        retCode = sdkIccReadEcFstCurrencyCodeDetail(
                GlbEMVBApp_UnionStruct);                                           //zcl 20111028 ����һ���ֻ��Ҵ���

        if (gPayData.ContactCardStatus == EMVB_ICC_NOCARD) {
            return SDK_ICC_NOCARD;
        }
        else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
            retCode = ERR_EMV_IccCommand;
        }

        if (retCode == EMV_OK) {
            retCode = EMV_OK;
        }

        else                                           //zcl 20111028 ����һ���ֻ��Ҵ���ʧ��
        {
            retCode = EMV_ERR;
        }
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_OK
                                SDK_EcLackBalanceErr
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_ECNotSupportErr
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccReadECBalance(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                        const SDK_ICC_PARAM *pstIccParam) //sjz 20120726
{
    s32 ret;

    if (NULL == pstIccParam || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return SDK_PARA_ERR;
    }
    ret = sdkIccReadEcFstCurrencyCode(pstIccParam);
    Trace("emv", "sdkIccReadEcCurrencyCode  return value = %d\r\n", ret);

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccReadEcData(pstIccTradeParam, pstIccParam);
    Trace("emv", "sdkIccReadEcData  return value = %d\r\n", ret);

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccReadEcSecCurrencyCode(pstIccParam);
    Trace("emv", "sdkIccReadEcSecCurrencyCode  return value = %d\r\n", ret);

    if (ret != SDK_OK) {
        return SDK_OK;              //��һ��������ѯ���ˣ�����OK
    }
    sdkIccReadEcSecData(pstIccParam);
    Trace("emv", "sdkIccReadEcSecData  return value = %d\r\n", ret);
    return SDK_OK;                  //��һ��������ѯ���ˣ�����OK
}

bool sdkIccCheckFloorLimit(EMVBAppUnionStruct *tempApp_UnionStruct) {
#if 0       //sxl?��Ҫ�������
    unsigned int amt = 0, i, MinLimit;
    unsigned int tempTransNum;
    TERMINFO *tempterminfo;
    CARDINFO *tempcardinfo;
    TRANS_LOG *temptranslog;
    u8 *data;
    u8 fn[128] = {0};
    u32 datasize, index;
    Trace("emv", "goto->sdkIccCheckFloorLimit \r\n");
    Trace("emv", "-------------�¹���----------------- \r\n");
    tempterminfo = tempApp_UnionStruct->terminfo;
    tempcardinfo = tempApp_UnionStruct->cardinfo;
    tempTransNum = *(tempApp_UnionStruct->TempTransNum);
//    temptranslog = tempApp_UnionStruct->TempTermTransLog;

    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "EmvTransLog");
    datasize = sizeof(guiSdkTransLogMaxNum) + sizeof(guiSdkTransNum) + sizeof(guiSdkTransIndex) + sizeof(TRANS_LOG) * SDK_ICC_TRANS_MAXNUM;
    data = (u8*)sdkGetMem(datasize + 1);  //sxl20140110

    if(data == NULL) //ʯ���� 2012.08.30 17:4
    {
        Trace("emv", "�����ڴ�ʧ��!\r\n");
        Assert(0);
        return SDK_ERR;
    }
    memset(data, 0, datasize);
    sdkReadOverlap(fn, data, datasize);
    temptranslog = (TRANS_LOG *)(data + sizeof(guiSdkTransLogMaxNum) + sizeof(guiSdkTransNum) + sizeof(guiSdkTransIndex));

    MinLimit = EMVByteArrayToInt(tempterminfo->FloorLimit, 4);
    EMVBcdToU32(&amt, tempterminfo->AmtAuthNum, 6);        //sxl

    if(tempTransNum > 0)
    {
        for(i = 0; i < tempTransNum; i++)
        {
            if((temptranslog + i)->TransType != 0x01
               && memcmp(tempcardinfo->PAN, (temptranslog + i)->PAN, tempcardinfo->PANLen) == 0
               && (tempcardinfo->PANSeq == (temptranslog + i)->PANSeq))
            {
                amt += (temptranslog + i)->TransAmt;
            }
        }
    }
    Trace("emv", "---------------\r\n amt = %d, MinLimit = %d\r\n", amt, MinLimit);

    if(amt >= MinLimit)
    { //�ý��׽�����Ϊ'1;
        tempterminfo->TVR[3] |= 0x80;      //set 'transaction exceeds floor limit' bit 1.
        sdkFreeMem(data);
        return true;
    }
    sdkFreeMem(data);
#endif
    return false;
}

s32 sdkIccReadEmvTransLog(void)   //sxl?
{
#if 0       //sxl?��Ҫ�������
    u8 *data;
    u8 fn[128] = {0};
    u32 datasize, index;

    Trace("emv", "goto->sdkIccReadEmvTransLog\r\n");
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "EmvTransLog");
    datasize = sizeof(guiSdkTransLogMaxNum) + sizeof(guiSdkTransNum) + sizeof(guiSdkTransIndex) + sizeof(TRANS_LOG) * SDK_ICC_TRANS_MAXNUM;

    guiSdkTransLogMaxNum = 0;
    guiSdkTransNum = 0;
    guiSdkTransIndex = 0;

    data = (u8*)sdkGetMem(datasize + 1);   //sxl20140110

    if(data == NULL) //ʯ���� 2012.08.30 17:4
    {
        Trace("emv", "�����ڴ�ʧ��!\r\n");
        Assert(0);
        return SDK_ERR;
    }
    memset(data, 0, datasize);

    if(sdkReadOverlap(fn, data, datasize) == SDK_FILE_OK)
    {
        index = 0;
        memcpy((u8*)&guiSdkTransLogMaxNum, data + index, sizeof(guiSdkTransLogMaxNum));
        index += sizeof(guiSdkTransLogMaxNum);
        memcpy((u8*)&guiSdkTransNum, data + index, sizeof(guiSdkTransNum));
        index += sizeof(guiSdkTransNum);
        memcpy((u8*)&guiSdkTransIndex, data + index, sizeof(guiSdkTransIndex));
        index += sizeof(guiSdkTransIndex);
    }
    else //��ȡ���׼�¼ʧ�ܣ��Կռ�¼����
    {
        guiSdkTransLogMaxNum = SDK_ICC_TRANS_MAXNUM; //sujianzhong 2012.09.09 11:36
        memcpy(data, (u8*)&guiSdkTransLogMaxNum, sizeof(guiSdkTransLogMaxNum));
        sdkSaveOverlap(fn, data, datasize);
    }
    sdkFreeMem(data);
#endif
    return SDK_OK;
}

s32 sdkIccSaveEmvTransLog(void) {
#if 0       //sxl?��Ҫ�������
    TRANS_LOG transLog;
    u8 *data;
    u8 fn[128] = {0};
    u32 datasize, index, logoffset;

    Trace("emv", "goto->sdkIccSaveEmvTransLog\r\n");
    logoffset = sizeof(guiSdkTransLogMaxNum) + sizeof(guiSdkTransNum) + sizeof(guiSdkTransIndex);
    datasize = logoffset + sizeof(TRANS_LOG) * SDK_ICC_TRANS_MAXNUM;
    data = (u8*)sdkGetMem(datasize + 1);   //sxl20140110

    if(data == NULL)
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "EmvTransLog");
    sdkReadOverlap(fn, data, datasize);

    memset(&transLog, 0, sizeof(transLog));
    transLog.TransAmt = guiSdkAmtAuthBin;

    if(gstSdkICCDataTable[MV_VLPIssuAuthorCode].bExist) //sujianzhong 2013.03.26 17:28
    { //�����ֽ���
        transLog.TransType = 0x01;
    }
    else
    {
        transLog.TransType = 0x00;
    }
    memcpy(transLog.TSI, gstSdkTermInfo.TSI, 2); //sujianzhong 2013.03.26 17:28
    transLog.PANLen = gstSdkCardInfo.PANLen;
    memcpy(transLog.PAN, gstSdkCardInfo.PAN, gstSdkCardInfo.PANLen);
    transLog.PANSeq = gstSdkCardInfo.PANSeq;
    EMVBcdToU32(&transLog.TransCount, gstSdkTermInfo.TransSeqCount, 4);

    memcpy(transLog.TransDate, gstSdkTermInfo.TransDate, 3);
    memcpy(transLog.TransTime, gstSdkTermInfo.TransTime, 3);

    if(guiSdkTransNum < guiSdkTransLogMaxNum)
    {
//        memcpy((unsigned char*)&gstSdkEmvTransLog[guiSdkTransNum], (unsigned char*)&transLog, sizeof(TRANS_LOG));
        memcpy((unsigned char*)data + logoffset + (guiSdkTransNum * sizeof(TRANS_LOG)), (unsigned char*)&transLog, sizeof(TRANS_LOG));
        guiSdkTransNum++;
        guiSdkTransIndex++;
    }
    else
    {
        guiSdkTransIndex++;

        if(guiSdkTransIndex >= guiSdkTransLogMaxNum)
        {
            guiSdkTransIndex = 0;
        }
//        memcpy((unsigned char*)&gstSdkEmvTransLog[guiSdkTransIndex], (unsigned char*)&transLog, sizeof(TRANS_LOG));
        memcpy((unsigned char*)data + logoffset + (guiSdkTransIndex * sizeof(TRANS_LOG)), (unsigned char*)&transLog, sizeof(TRANS_LOG));
    }
    EMVIntToByteArray(guiSdkTransNum, gstSdkTermInfo.TransNum, 2);
    EMVIntToByteArray(guiSdkTransIndex, gstSdkTermInfo.TransIndex, 2);


    index = 0;
    memcpy(data + index, (u8 *)&guiSdkTransLogMaxNum, sizeof(guiSdkTransLogMaxNum));
    index += sizeof(guiSdkTransLogMaxNum);

    memcpy(data + index, (u8 *)&guiSdkTransNum, sizeof(guiSdkTransNum));
    index += sizeof(guiSdkTransNum);

    memcpy(data + index, (u8 *)&guiSdkTransIndex, sizeof(guiSdkTransIndex));
    index += sizeof(guiSdkTransIndex);

//    memcpy(data + index, (u8 *)&gstSdkEmvTransLog, sizeof(gstSdkEmvTransLog));
//    index += sizeof(gstSdkEmvTransLog);

    sdkSaveOverlap(fn, data, datasize);
    sdkFreeMem(data);


#endif
    return SDK_OK;
}

s32 sdkIccClearEmvWater(void) {
#if 0       //sxl?��Ҫ�������
    u8 *data;
    u8 fn[128] = {0};
    u32 datasize;

    Trace("emv", "goto->sdkIccClearWater\r\n");
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "EmvTransLog");

    guiSdkTransLogMaxNum = SDK_ICC_TRANS_MAXNUM; //sujianzhong 2012.09.09 11:36
    guiSdkTransNum = 0;
    guiSdkTransIndex = 0;
//    memset(gstSdkEmvTransLog, 0, sizeof(gstSdkEmvTransLog));

    datasize = sizeof(guiSdkTransLogMaxNum) + sizeof(guiSdkTransNum) + sizeof(guiSdkTransIndex) + sizeof(TRANS_LOG) * SDK_ICC_TRANS_MAXNUM;
    data = (u8*)sdkGetMem(datasize + 1);  //sxl20140110

    if(data == NULL) //ʯ���� 2012.08.30 17:4
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(data, 0, datasize);
    memcpy(data, (u8 *)&guiSdkTransLogMaxNum, sizeof(guiSdkTransLogMaxNum));
    sdkSaveOverlap(fn, data, datasize);

    sdkFreeMem(data);
#endif
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *DataIn:Tag+Len+Value......
                                u32 DataLen:
** Returned value:	SDK_PARA_ERR
                                SDK_IccDataRedundErr
                                SDK_IccDataFormatErr
                                SDK_OK
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl ֻ�ж�����������õ��������
                               //sxl?��Ҫ�鿴����������

*****************************************************************************/
s32 sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen) {
    u16 k; //, i, j, t;
    u16 uiIndex, len;
    //u8 bInTable;
    u8 tag[4], taglen;
    u16 tagdatalen;
    u8 present;
    s32 ret1;
    EMVTAGCVLITEM retrunitem;
    u32 tagtype;
    u16 tempdatalen;
    u16 startpos;
    u8 parseret;

    if (NULL == DataIn || siDataLen < 0) {
        return SDK_PARA_ERR;
    }
    TraceHex("emv", "dataIn", DataIn, siDataLen);

    //G101�ش����ݣ��������շ���CardInfo��TermInfo��,����sdkIccReadTLV��sdkIccConfigTLV��д����
    //memcpy((u8*)&gstSdkTermDataTable, (u8*)&gstSdkTermDataTable_Stored, TERMDATANUM * sizeof(DATAELEMENT));
    ///memcpy((u8*)&gstSdkICCDataTable, (u8*)&gstSdkICCDataTable_Stored, ICCDATANUM * sizeof(DATAELEMENT));

    uiIndex = 0;

    while (uiIndex < siDataLen) {
        if (DataIn[uiIndex] == 0xFF || DataIn[uiIndex] == 0x00) {
            uiIndex++;
            continue;
        }
        //bInTable = 0;
        k = DataIn[uiIndex];


        //Trace("emv", "k=%02X\r\n", k);

        //gucSdkIccUnzipQreaderData ��ʼ��
        //G101ADD zjz20110620==================================
#if 0 //del by sujianzhong 2015.06.05
        if(((k == 0x55) || (k == 0x01)) && (gucSdkIccUnzipQreaderData))                  //���ص�D4��������������TAG
        {
            uiIndex++;                            //TAG ����
            len = DataIn[uiIndex++];                     //����

            if(k == 0x55)                       //��Ҫ��ӡǩ����
            {
                if(DataIn[uiIndex] == 0)
                {
                    gstSdkIccQreaderTermInfo.PrintSign = 1;  //sxl?��������������ʱ������
                }
                else
                {
                    gstSdkIccQreaderTermInfo.PrintSign = 0;
                }
            }
            else                               //DDAʧ��Ҫת����
            {
                TraceHex("emv", "DataIn[uiIndex-3]=\r\n", DataIn + uiIndex - 3, 5);

                if(DataIn[uiIndex] == 1)
                {
                    gstSdkIccQreaderTermInfo.DDAFaileNeedOnline = 1;
                }
                else
                {
                    gstSdkIccQreaderTermInfo.DDAFaileNeedOnline = 0;
                }
            }
            uiIndex += len;
            continue;
        }
#endif
        //=======================================================
        //sxl?����һ���ٴ���
        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DataIn[uiIndex];

        if ((tag[0] & 0x1f) == 0x1f) {
            tag[taglen++] = DataIn[uiIndex + 1];

            if (tag[1] & 0x80) {
                tag[taglen++] = DataIn[uiIndex + 2];
            }
        }
        startpos = uiIndex;
        uiIndex += taglen;
        parseret = ParseExtLen(DataIn, &uiIndex, &tagdatalen);

        if (parseret != 0) {
            return SDK_IccDataFormatErr;
        }

        //����71  72 TAG
        if (tag[0] == 0x71 ||
            tag[0] == 0x72)          //tagdatalen��һ���ֽڣ�����DOL���͵�,�鿴������̴���
        {
            u8 *tempdata = (u8 *) sdkGetMem(300);

            if (NULL == tempdata) {
                Assert(0);
                return SDK_ERR;
            }
            memset(tempdata, 0, 300);
            dllemvb_avl_gettagvalue_all(tag, tempdata, &tempdatalen);

            if (tempdatalen + uiIndex - startpos + tagdatalen < 300) {
                memcpy(&tempdata[tempdatalen], &DataIn[startpos], uiIndex - startpos + tagdatalen);
                tempdatalen += uiIndex - startpos + tagdatalen;
                dllemvb_avl_createsettagvalue(tag, tempdata, tempdatalen);
            }
            sdkFreeMem(tempdata);
        }
        else {
            present = dllemvb_avl_checkiftagexist(tag);

            if (present)              //sxl?ȷ�ϴӶ�����������tagdatalen,���ֻ�Ǵ���Ӷ��������ص����ݵĻ����������ظ�
            {
                //return SDK_IccDataRedundErr;
            }
            tagtype = dllemv_avl_gettagtype();
            ret1 = EMVB_TagBaseLib_read(tag, taglen, tagtype, &retrunitem);

            if (ret1 == 0) {
                if (tagdatalen < retrunitem.minlen || tagdatalen > retrunitem.maxlen) {
                    return SDK_IccDataFormatErr;
                }
            }
            dllemvb_avl_createsettagvalue(tag, &DataIn[uiIndex], tagdatalen);
        }
        uiIndex += tagdatalen;
    }

    return SDK_OK;
}

#define FILE_55FIELD_NAME "sdk55field" //�ļ���
#define FILE_55FIELD_LEN 1024 //�ļ���С

/*****************************************************************************
** Descriptions:   ����55������
** Parameters:     const u8 *DataIn
                   s32 siDataLen
** Returned value: SDK_ERR
                   SDK_OK
** Created By:     sujianzhong20130822
** Remarks:        ֻУ��tlv���ݵĸ�ʽ���ܳ��ȣ���У��tag
   �ļ���С����Ϊ1024:��ʽ:2�ֽڵ��ܳ���(��λ��ǰ) + TLV����
                          //sxl20140408
*****************************************************************************/
static s32 Private_UnzipPagData(const u8 *DataIn, s32 siDataLen) {
    u8 *temp;  //[FILE_55FIELD_LEN]
    u16 id = 0;

    Trace("emv", "\r\n");
    u16 len = 0;
    u8 parseret;
    u8 fn[SDK_EMV_MAX_PATH];

    if (DataIn == NULL || siDataLen == 0 || (siDataLen > FILE_55FIELD_LEN - 2)) {
        Trace("emv", "���ݴ���,����=%d,����1022\r\n", siDataLen);
        return SDK_ERR;
    }
    id = 0;

    while (id < siDataLen) {
        if ((DataIn[id] & 0x1F) == 0x1F) {
            id++;
        }
        id++;

        parseret = ParseExtLen(DataIn, &id, &len);

        if (parseret != 0) {
            return SDK_ERR;
        }
        id += len;
    }

    if (id != siDataLen) {
        return SDK_ERR;
    }
    temp = (u8 *) emvb_malloc(FILE_55FIELD_LEN);
    memset(temp, 0, FILE_55FIELD_LEN);
    temp[0] = (u8)(siDataLen >> 8);
    temp[1] = (u8) siDataLen;
    memcpy(temp + 2, DataIn, siDataLen);

    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, FILE_55FIELD_NAME);

//    TraceHex("emv", "����", temp, siDataLen+18);
    if (SDK_FILE_OK != sdkWriteFile(fn, temp, FILE_55FIELD_LEN)) //����д�ļ�
    {
        emvb_free(temp);
        Trace("emv", "д�ļ�����\r\n");
        return SDK_ERR;
    }
    emvb_free(temp);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:   ��Private_UnzipPagData��ѹ������ݽ���tlv���ã����
                    ��ͬ����Tag�����������Ȼ��׷�ӵ���β
** Parameters:     const u8 *pheTag
                   const u8 *pheInData
                   s32 siInLen
** Returned value:
** Created By:     sujianzhong20130822
** Remarks:        //sxl20140408     //sxl?? ��Ҫ�޸�Ӧ�ó�����Ӧ�ó���һ����֯�����ݺ���
                                                            ���������и�д
*****************************************************************************/
static s32 Private_ConfigPagTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen) {
    u8 fn[SDK_EMV_MAX_PATH] = {0}, *temp;  //[FILE_55FIELD_LEN]={0}
    s32 totallen = 0, id = 0, taglen = 0, len = 0, tlvlen, templen = FILE_55FIELD_LEN;
    //u16 DOLLen = 0;
    u16 tagdatalen;

    Trace("emv", "\r\n");

    //if(NULL == pheTag || (NULL == pheInData && 0 != siInLen) || siInLen > 255)
    if (NULL == pheTag || siInLen > 255) {
        Trace("emv", "���ݴ���,����=%d\r\n", siInLen);
        return SDK_PARA_ERR;
    }
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, FILE_55FIELD_NAME);

    temp = (u8 *) emvb_malloc(FILE_55FIELD_LEN);

    if (!sdkAccessFile(fn)) {
        Trace("emv", "�ļ�������\r\n");
        memset(temp, 0, FILE_55FIELD_LEN);

        if (SDK_FILE_OK != sdkWriteFile(fn, temp, FILE_55FIELD_LEN)) //����д�ļ�
        {
            emvb_free(temp);
            return SDK_PARA_ERR;
        }
    }
    else {
        if (SDK_FILE_OK != sdkReadFile(fn, temp, 0, &templen)) {
            emvb_free(temp);
            return SDK_PARA_ERR;
        }
    }
    totallen = temp[0] * 256 + temp[1];

    if (totallen > FILE_55FIELD_LEN - 2) {
        emvb_free(temp);
        return SDK_PARA_ERR;
    }
    totallen += 2;

    //for(DOLLen = 0; DOLLen < siInLen; )
    {
        if ((pheTag[0] & 0x1F) == 0x1F) {
            taglen = 2;
        }
        else {
            taglen = 1;
        }
        id = 2;    //�ڶ����ֽڿ�ʼ����tlv����

        while (id < totallen) {
            tlvlen = 0;

            if ((temp[id] & 0x1F) == 0x1F)    //ƫ��Tag����
            {
                tlvlen = 2;
            }
            else {
                tlvlen = 1;
            }

            if (temp[id + tlvlen] > 127)     //sxl?��Ҫ��ϸ�������
            {
                tlvlen++;
            }
            len = temp[id + tlvlen];  //ƫ��len����
            tlvlen++;
            tlvlen += len;    //ƫ��data����

            if (memcmp(&pheTag[0], temp + id, taglen))  //��ǩ��ͬ
            {
                id += tlvlen;
                continue;
            }
            else    //�ҵ���ͬ�ı�ǩ
            {    //ɾ��ԭ��ǩ����
                memcpy(temp + id, temp + id + tlvlen, totallen - id - tlvlen);
                totallen -= tlvlen;
                break;
            }
        }


        //tagdatalen = pheTag[DOLLen + taglen];
        tagdatalen = siInLen;


        if ((totallen + tagdatalen + taglen + 1) > FILE_55FIELD_LEN - 2) {
            Trace("emv", "���ݳ���,����=%d,����1022\r\n", totallen + siInLen);
            emvb_free(temp);
            return SDK_PARA_ERR;
        }
        memcpy(temp + totallen, &pheTag[0], taglen);  //�ڽ�β׷���±�ǩ������
        totallen += taglen;
        //DOLLen += taglen;

        if (siInLen > 127) {
            temp[totallen++] = 0x81;
            //DOLLen++;
        }
        //temp[totallen++] = (u8)pheTag[DOLLen++];
        temp[totallen++] = (u8) tagdatalen;



        //memcpy(temp + totallen, &pheInData[DOLLen], tagdatalen);
        memcpy(temp + totallen, pheInData, (u32) tagdatalen);
        totallen += tagdatalen;
        //DOLLen += tagdatalen;
    }

    totallen -= 2;
    temp[0] = (u8)(totallen >> 8);
    temp[1] = (u8) totallen;

//    TraceHex("emv", "����", temp, totallen+18);
    if (SDK_FILE_OK != sdkWriteFile(fn, temp, FILE_55FIELD_LEN)) {
        Trace("emv", "д�ļ�����\r\n");
        emvb_free(temp);
        return SDK_ERR;
    }
    emvb_free(temp);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:   ��ȡPrivate_UnzipPagData��ѹ��Private_ConfigPagTLV����֮���TLV
                    ����
** Parameters:     const u8 *pheTag
                   u8 *pheOutData
                   s32 *psiOutLen
** Returned value:
** Created By:     sujianzhong20130822
** Remarks:        //sxl20140408
*****************************************************************************/
static s32 Private_ReadPagTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) {
    u8 fn[SDK_EMV_MAX_PATH] = {0}, *temp;  //[FILE_55FIELD_LEN]={0}
    s32 totallen = 0, id = 0, taglen = 0, len = 0, tlvlen = 0, templen = FILE_55FIELD_LEN;

    Trace("emv", "\r\n");

    if (NULL == pheTag || (NULL == pheOutData) || NULL == psiOutLen) {
        return SDK_PARA_ERR;
    }
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, FILE_55FIELD_NAME);

    if (!sdkAccessFile(fn)) {
        return SDK_PARA_ERR;
    }
    temp = (u8 *) emvb_malloc(FILE_55FIELD_LEN);

    if (SDK_FILE_OK != sdkReadFile(fn, temp, 0, &templen)) {
        emvb_free(temp);
        return SDK_PARA_ERR;
    }
//    TraceHex("emv", "����", temp, temp[0]*256+temp[1]+18);

    totallen = temp[0] * 256 + temp[1];

    if (totallen > FILE_55FIELD_LEN - 2) {
        emvb_free(temp);
        return SDK_PARA_ERR;
    }

    if ((pheTag[0] & 0x1F) == 0x1F) {
        taglen = 2;
    }
    else {
        taglen = 1;
    }
    id = 2; //�ڶ����ֽڿ�ʼ����tlv����
    totallen += 2;

    while (id < totallen) {
        tlvlen = 0;

        if ((temp[id] & 0x1F) == 0x1F) //ƫ��Tag����
        {
            tlvlen = 2;
        }
        else {
            tlvlen = 1;
        }

        if (temp[id + tlvlen] > 127) {
            tlvlen++;
        }
        len = temp[id + tlvlen]; //ƫ��len����
        tlvlen++;
        tlvlen += len; //ƫ��data����

        if (memcmp(pheTag, temp + id, taglen)) //��ǩ��ͬ
        {
            id += tlvlen;
            continue;
        }
        else //�ҵ���ͬ�ı�ǩ
        {
            memcpy(pheOutData, temp + id + tlvlen - len, len);
            *psiOutLen = len;
            emvb_free(temp);
            return SDK_OK;
        }
    }

    Trace("emv", "�Ҳ���ͬ����ǩ\r\n");
    emvb_free(temp);
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *DataIn:Tag+Len+Value......
                                u32 DataLen:
** Returned value:	SDK_PARA_ERR
                                SDK_IccDataRedundErr
                                SDK_IccDataFormatErr
                                SDK_OK
** Created By:		sujianzhong2012.08.09
** Remarks:                //sxl   �������Ͱ���ʱ����õ�  //sxl? ���������ʲô����?
                                 �������ͺͽ��հ���55������,��ʱ������

*****************************************************************************/
s32 Private_sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen) {
    //���ж����ݸ�ʽ�Ƿ���ȷ,��Ҫ�ж�̫�ϸ����ݳ�������Ҫ�󼴿�
    u16 uiIndex;
    u8 tag[4], taglen;
    u16 tagdatalen;
    //u8 present;
    s32 ret1;
    EMVTAGCVLITEM retrunitem;
    u32 tagtype;

    u16 tempdatalen;
    u16 startpos;
    u8 parseret;

    if (NULL == DataIn || siDataLen < 0) {
        return SDK_PARA_ERR;
    }
    uiIndex = 0;

    while (uiIndex < siDataLen) {
        if (DataIn[uiIndex] == 0x00) {
            uiIndex++;
            continue;
        }
        memset(tag, 0, sizeof(tag));
        taglen = 0;
        tag[taglen++] = DataIn[uiIndex];

        if ((tag[0] & 0x1f) == 0x1f) {
            tag[taglen++] = DataIn[uiIndex + 1];

            if (tag[1] & 0x80) {
                tag[taglen++] = DataIn[uiIndex + 2];
            }
        }
        //�й������ĵ�,55������ΪTLV��ʽ
        startpos = uiIndex;
        uiIndex += taglen;
        parseret = ParseExtLen(DataIn, &uiIndex, &tagdatalen);

        if (parseret != 0) {
            return SDK_IccDataFormatErr;
        }
        tagtype = dllemv_avl_gettagtype();
        ret1 = EMVB_TagBaseLib_read(tag, taglen, tagtype, &retrunitem);

        if (ret1 == 0) {
            if (tagdatalen < retrunitem.minlen || tagdatalen > retrunitem.maxlen) {
                return SDK_IccDataFormatErr;
            }
        }

        if (tag[0] == 0x71 || tag[0] == 0x72) {
            u8 *tempdata = (u8 *) sdkGetMem(300);

            if (NULL == tempdata) {
                Assert(0);
                return SDK_ERR;
            }
            memset(tempdata, 0, 300);         //sxl? ����Щͳһ�ŵ�һ��������
            dllemvb_avl_gettagvalue_all(tag, tempdata, &tempdatalen);

            if (tempdatalen + tagdatalen + uiIndex - startpos < 300) {
                memcpy(&tempdata[tempdatalen], &DataIn[startpos], tagdatalen + uiIndex - startpos);
                tempdatalen += tagdatalen + uiIndex - startpos;               //�鿴��֯�����Ƿ���ȷ
                dllemvb_avl_createsettagvalue(tag, tempdata, tempdatalen);
            }
            sdkFreeMem(tempdata);
        }
        else if (tag[0] == 0x91) {
            dllemvb_avl_createsettagvalue(tag, &DataIn[startpos], tagdatalen + uiIndex - startpos);
        }
        uiIndex += tagdatalen;
    }


    if (SDK_OK != Private_UnzipPagData(DataIn, siDataLen)) {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:   config trans-datas of the tag
** Parameters:     const u8 *pheTag
                   const u8 *pheInData
                   s32 siInLen
** Returned value:
** Created By:     sujianzhong20130308
** Remarks:    //sxl??  ReFormRecordTLV   ���������Ƚ���д�ļ��Ƚ϶�
                                ��Ҫ��Ӧ�ó���һ����֯�����ݴ�����
*****************************************************************************/
s32 Private_sdkIccConfigPagTLV(const u8 *pheTag, const u8 *pheInData,
                               s32 siInLen)     //sujianzhong 2013.03.08 14:35
{
    //u8 i;
    //u32 tag_len;

    if (NULL == pheTag || pheTag[0] == 0xFF || pheTag[0] == 0 || pheInData == NULL || siInLen < 0) {
        return SDK_PARA_ERR;
    }

    if (SDK_OK != Private_ConfigPagTLV(pheTag, pheInData, siInLen)) {
        Trace("emv", "����ʧ��\r\n");
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const u8 *pheTag:
                                u32 uiAvailableLen:
                                u8 *pheOutData:
                                u32 *puiOutLen:
** Returned value:	SDK_OK:
                                SDK_PARA_ERR: parameter error
                                SDK_EQU: the card hasn't return the tag
** Created By:		sujianzhong2012.08.06
** Remarks:            //sxl20140408  ����ʱ�ļ����ȡTAG����
*****************************************************************************/
s32 Private_sdkIccReadPagTLV(const u8 *pheTag, u8 *pheOutData,
                             s32 *psiOutLen)     //sujianzhong 2013.03.08 14:35
{
    if (SDK_OK != Private_ReadPagTLV(pheTag, pheOutData, psiOutLen)) {
        Trace("emv", "��ȡʧ��\r\n");
        return SDK_PARA_ERR;
    }
    TraceHex("emv", "Read Success", pheOutData, (*psiOutLen));
    return SDK_OK;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������:
   ��ڲ���:
   �� �� ֵ:
   ��    ע:    sxl20140408
 ********************************************************************/
void sdkIccCreateUnpredictNum(void) {
    u16 unPredict1, unPredict2;
    u8 tempkey[8], tempdata[8], tempdataOut[8];
    u8 UnpredictNum[4];

    memset(tempkey, 0, sizeof(tempkey));
    memset(tempdata, 0, sizeof(tempdata));
    memset(tempdataOut, 0, sizeof(tempdataOut));


//    sdkGetRtc(temp);
    Verify(sdkGetRandom(tempkey, sizeof(tempkey) - 2) == SDK_OK);      //sjz
//    tempkey[0] = Sys.Year;
//    tempkey[1] = Sys.Month;
//    tempkey[2] = Sys.Day;
//    tempkey[3] = Sys.Hour;
//    tempkey[4] = Sys.Minute;
//    tempkey[5] = Sys.Second;

    //tempkey[6] = gstSdkTermInfo.TransSeqCount[2];   //sxl?���￴��Ҫ��Ҫ����
    //tempkey[7] = gstSdkTermInfo.TransSeqCount[3];


    rand();
    unPredict1 = (u16) rand();
    unPredict2 = (u16) rand();

    memcpy((u8 *) tempdata, (u8 * ) & unPredict1, 2);
    memcpy((u8 * ) & tempdata[2], (u8 * ) & unPredict2, 2);

    Emvdes(tempdata, tempdataOut, tempkey, ENCRYPT);

    memcpy((u8 *) UnpredictNum, (u8 *) tempdataOut, 2);
    memcpy((u8 * ) & UnpredictNum[2], (u8 * ) & tempdataOut[2], 2);

//	memcpy((u8*)&TermInfo.UnpredictNum,(u8*)&unPredict1,2);
//	memcpy((u8*)&TermInfo.UnpredictNum[2],(u8*)&unPredict2,2);
    //gstSdkTermDataTable[MV_UnpredictNum - TermDataBase].bExist = 1;

    dllemvb_avl_createsettagvalue(TAG_UnpredictNum, UnpredictNum, 4);
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ������Կ
   ��ڲ���: 1:TPK,2:TAK
   �� �� ֵ: RSLT_OK: ���³ɹ�, RSLT_OVERTIME: ����ʧ��,RSLT_ESC:���ܴ�
   ��    ע:   //sxl  20140408
 ********************************************************************/
void sdkIccReadSN(u8 *SN) {
    u8 i;
    u8 temp[256];

    memset(temp, 0, sizeof(temp));
    sdkReadPosSn(temp);
    i = (u8) strlen(temp);

    if (i < 8) {
        memcpy(SN, temp, i);
        memcpy(SN + i, "        ", 8 - i);
    }
    else {
        memcpy(SN, &temp[i - 8], 8);
    }
        TraceHex("emv","sn",SN,8);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccProcessRefer(u8 source,AppUnionStruct *tempApp_UnionStruct)
   ��������: ����REFER
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ: sxl20140408
 ********************************************************************/
void sdkIccProcessRefer(u8 ucSource) {
#ifdef ADVT_DEBUG
    u8 i, len, pan[24];

    u8 rc, key, retCode;
    u8 PAN[20] = {0};
    u16 PANLen;

    if((GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL))
    {
        return;
    }
    dllemvb_avl_gettagvalue_all(TAG_PAN, PAN, &PANLen);
    memset(pan, 0, 24);

    len = 0;

    for(i = 0; i < PANLen; i++)
    {
        if((PAN[i] & 0xF0) == 0xF0)
        {
            break;
        }
        len++;

        if((PAN[i] & 0x0F) == 0x0F)
        {
            break;
        }
        len++;
    }

    EMVbcd_to_asc(pan, PAN, len);

    sdkDispClearScreen();
    sdkIccDispStr(CALL_YOUR_BANK);

    sdkIccScrDisplay(ASCII, 1, 0,  "����:   ");
    sdkIccScrDisplay(ASCII, 2, 0, (char*)pan);
    sdkIccScrDisplay(ASCII, 3, 0,  "�����������");

    EMV_WaitKeyValue(0, 0);


    sdkSysBeep(SDK_SYS_BEEP_OK);
    sdkDispClearScreen();
    sdkIccDispStr(BYPASS_REFERRAL);     //center: Bypass Referral
    sdkIccDispStr(ACCEPT_TRANS);     //1.Accept Trans
    sdkIccDispStr(DECLINE_TRANS);     //2.Decline Trans

    if(ucSource == ICC_REFER)
    {
        sdkIccDispStr(FORCE_ONLINE);               //3.Force Online
    }
    gEMVTradeParam->bAbleOnline = 0;

    Emv_KeyFlush();


    while(1)
    {
        key = sdkKbGetKey();

        if(key != 0)
        {
            if(key == '1')
            {
                //EMV_Beep(0);
                sdkSysAudioBeepOk();

                if(ucSource == ICC_REFER)
                {
                    dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *)"Y2", 2);
                }
                //gstSdkTermDataTable[MV_AuthorRespCode - TermDataBase].bExist = 1;
                //gstSdkTermInfo.reqCrpt = CryptType_TC;
                dllemvb_avl_set(TAG_ReferenceControlParameter, 0x40);  //sxl?�鿴
                break;
            }
            else if(key == '2')
            {
                //EMV_Beep(0);
                sdkSysAudioBeepOk();

                if(ucSource == ICC_REFER)
                {
                    dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *)"Z2", 2);
                }
                //gstSdkTermDataTable[MV_AuthorRespCode - TermDataBase].bExist = 1;
                //gstSdkTermInfo.reqCrpt = CryptType_AAC;
                dllemvb_avl_set(TAG_ReferenceControlParameter, 0x00);
                break;
            }

            else
            {
                //EMV_Beep(1);
                sdkSysAudioBeepErr();
            }
        }
    }

#else

    if (ucSource == ICC_REFER) {
        dllemvb_avl_createsettagvalue(TAG_AuthRespCode, (u8 *) "Z2", 2);
    }
    //gstSdkTermDataTable[MV_AuthorRespCode - TermDataBase].bExist = 1;
    //gstSdkTermInfo.reqCrpt = CryptType_AAC;
    dllemvb_avl_set(TAG_ReferenceControlParameter, 0x00);
#endif
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Okolle Ltd,Co.
   ��������: void sdkIccGetIccMainAcc(u8 *accu)
   ��������: ��CARDINFO�еõ�IC�����׵��ʺ�
   ��	  ��: CardInfo.PANLen;CardInfo.PAN
   ��   ���� �ʺ�1�ֽڵĳ���(ѹ��BCD)��10�ֽڵ�����
   ��ص���:
   ��    ע:  //sxl20140408
 ********************************************************************/
void sdkIccGetIccMainAcc(u8 *pAcc) {
    u8 temp[128];
    u16 i, j;
    EMVTAGCVLITEM *item = NULL;


    i = 0;
    memset(temp, 0, sizeof(temp));                                              //�建��



    item = dllemvb_avl_gettagitempointer(TAG_PAN);

    if (item != NULL) {
        if (item->len) {
            i = item->len;                                                                            //ȡIC���ʺŵĳ���

            if (i > 10) {
                i = 10;                                                                         //�޳�
            }
            sdkBcdToAsc(temp, item->data, i);                                            //ת����BCD

            for (j = 0;
                 j < i * 2; j++)                                                         //���˵�F
            {
                if (temp[j] == 'F' || temp[j] == 'f') {
                    temp[j] = 0;
                    break;
                }
            }

            i = (u8) strlen(
                    temp);                                                                           //ȡ����

            if (i > 19) {
                i = 19;                                                                                 //���Ƴ���
            }
        }
    }
    sdkU16ToBcd(pAcc, i,
                1);                                                               //ת����BCD
    sdkAscToBcd(&pAcc[1], temp, i);                                                //ת����BCD
}

/*******************************************************************
   ��    ��:       ��Т��
   ��������:       u8 sdkIccFilterChar(u8 *str)
   ��������:       �����ַ����в��ɼ��ַ�
   ��ڲ���:       str ������ַ���
   ���ڲ���:       str ���˺���ַ���
   �� �� ֵ:
   ��    ע:   //sxl20140408
 ********************************************************************/
void sdkIccFilterChar(u8 *str) {
    unsigned int i, j, len;
    u8 pBuf[21];

    len = strlen((char *) str);      //sxl arm_linux

    if (len > 20) {
        len = 20;
    }
    memset(pBuf, 0, 21);
    j = 0;

    for (i = 0; i < len; i++) {
        if (str[i] < 0xA0) {
            if (str[i] < 0x7f && str[i] > 0x1f) {
                pBuf[j++] = str[i];
            }
        }
        else {
            if (str[i] < 0xff) {
                i++;

                if (str[i] < 0xff && str[i] > 0x9f)                 //����
                {
                    pBuf[j++] = str[i - 1];
                    pBuf[j++] = str[i];
                }
                else if (str[i] < 0x7f && str[i] > 0x1f)                 //��0xf1 0x41
                {
                    pBuf[j++] = str[i];
                }
            }
        }
    }

    pBuf[j] = 0;     //lint !e850

    str[0] = 0;
    strcpy((char *) str, (char *) pBuf);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccScrDisplay(u8 x,u8 y,unsigned *Disdata)
   ��������: EMV��ʾ
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
void sdkIccScrDisplay(u8 mode, u8 x, u8 y, const char *Disdata)        //sxl arm_linux
{
    u8 strBuf[21];

    memset(strBuf, 0, sizeof(strBuf));
    strcpy((char *) strBuf, Disdata);
    sdkIccFilterChar(strBuf);
    Verify(sdkDispFillRowRam(x, y, strBuf, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) ==
           SDK_OK);
    sdkDispBrushScreen();       //��ʾ
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccDispStr(u8 ucIndex)
   ��������:
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
void sdkIccDispStr(u8 ucIndex) {
    u8 mode, x, y;
    T_DISPSTR strDisp;

    if (gLanguage == EMV_CHINESE)  //sxlLanguage����������
    {
        memcpy((u8 * ) & strDisp, (u8 * ) & gstSdkstrDispTable_Ch[ucIndex], sizeof(T_DISPSTR));
    }
    else {
        memcpy((u8 * ) & strDisp, (u8 * ) & gstSdkstrDispTable_En[ucIndex], sizeof(T_DISPSTR));
    }
    mode = ((strDisp.ucAttr & 0x01) == 0) ? CFONT : ASCII;

    if (mode == ASCII) {
        x = strDisp.ucRow;
        y = strDisp.ucCol / 6;
    }
    else {
        x = strDisp.ucRow / 2;
        y = strDisp.ucCol / 6;
    }
    sdkIccScrDisplay(mode, x, y, strDisp.asStr);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccVerifyIdCard(void)
   ��������: ��֤֤������
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
u8 sdkIccVerifyIdCard(void) {
    //u8 retCode ;   //sxl arm_linux
    u8 temp[22];
    u8 CardHolderType;
    EMVTAGCVLITEM *CardHolderIdNoitem;
    s32 key;

    memset(temp, 0, sizeof(temp));

    sdkDispClearScreen();

    strcpy((char *) temp, "֤��:");

    CardHolderType = dllemvb_avl_gettagvalue(TAG_CardHoldIdType);

    strcat((char *) temp, gcSdkIdTypeTab[CardHolderType]);
    sdkIccScrDisplay(CFONT, 0, 0, (char *) temp);
    memset(temp, 0, sizeof(temp));
    sdkIccScrDisplay(CFONT, 1, 0, (char *) "֤����:");

    CardHolderIdNoitem = dllemvb_avl_gettagitempointer(TAG_CardHoldIdNo);

    if (CardHolderIdNoitem->len > 20) {
        memcpy(temp, CardHolderIdNoitem->data, 20);
        sdkIccScrDisplay(CFONT, 2, 0, (char *) temp);

        memset(temp, 0, sizeof(temp));
        memcpy(temp, &CardHolderIdNoitem->data[20], CardHolderIdNoitem->len - 20);
        sdkIccScrDisplay(CFONT, 3, 0, (char *) temp);
    }
    else {
        memcpy(temp, CardHolderIdNoitem->data, CardHolderIdNoitem->len);
        sdkIccScrDisplay(CFONT, 2, 0, (char *) temp);
    }
    sdkIccScrDisplay(CFONT, 4, 0, "1.��ȷ  2.����");

    sdkKbKeyFlush();

    while (1) {
        key = sdkKbGetKey();

        if (key != 0) {
            switch (key) {
                case '1':
                case '2':
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    sdkDispClearScreen();
                    sdkIccDispStr(PLEASE_WAIT);

                    if (key == '1') {
                        return EMV_OK;
                    }
                    else {
                        return EMV_ERR;
                    }
                    //break;

                default:
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                    break;
            }
        }
    }
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostNotAccept(void)
   ��������: ��ʾ��������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: //sxl20140408
 ********************************************************************/
void sdkIccPostNotAccept(void) {
    sdkDispClearScreen();
    sdkIccDispStr(NOT_ACCEPTED);
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostInputPinAgain(void)
   ��������: ��ʾ��������������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug  //sxl20140408
 ********************************************************************/
s32 sdkIccPostInputPinAgain(u8 LastTry) {
    s32 key;

    sdkDispClearRow(SDK_DISP_LINE2);
    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispClearRow(SDK_DISP_LINE4);
    sdkDispClearRow(SDK_DISP_LINE5);

    Verify(sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PIN�������!",
                             SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    Verify(sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������",
                             SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);


    if (LastTry == 1) {
        Verify(sdkDispFillRowRam(SDK_DISP_LINE4, 0, "( ���һ�� )",
                                 SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    }
    sdkDispBrushScreen();
    key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 15 * 100);                      //�ȴ�����

    return key;
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostEmvSelectAgain(void)
   ��������: ��ʾ����ѡ��Ӧ��
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug  //sxl20140408
 ********************************************************************/
void sdkIccPostEmvSelectAgain(void) {
    sdkDispClearRow(SDK_DISP_LINE2);
    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispClearRow(SDK_DISP_LINE4);
    sdkDispClearRow(SDK_DISP_LINE5);

    Verify(sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������",
                             SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    Verify(sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������",
                             SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
    sdkDispBrushScreen();

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostDispAppList(void)
   ��������: �ṩӦ���б���û�ѡ��
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:   //sxl 20140408
 ********************************************************************/
void sdkIccPostDispAppList(void) {
    s32 key;
    u8 type;
    u8 curPage;
    u8 totalPage;

    /*=======BEGIN: ���ٲ� 2013.07.31  14:55 modify===========*/
//        dev_key_flush();
    sdkKbKeyFlush();
    /*====================== END======================== */
//    dev_ts_flush();//sjz??? 2013.01.05 18:13

    if (gEMVTradeParam == NULL) {
        return;
    }
    totalPage = (gEMVTradeParam->AppListCandidatenum - 1) / 4;
    curPage = 0;
    type = gEMVTradeParam->DispSeqListData.seqtype;
    sdkDispClearScreen();
    sdkIccDispStr(TITLE_APP_SELECT);
    sdkIccDispAppSelectPage(totalPage, curPage, type);
    sdkKbKeyFlush();

    while (1) {
        key = sdkKbGetKey();

        if (key != 0) {
            switch (key) {
                case SDK_KEY_1:
                case SDK_KEY_2:
                case SDK_KEY_3:
                case SDK_KEY_4: {
                    if ((curPage * 4 + key - '0') > gEMVTradeParam->AppListCandidatenum) {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        sdkDispClearScreen();
                        sdkIccDispStr(PLEASE_WAIT);
                        gEMVTradeParam->DispSeqListData.SelectedNum = (u8)(curPage * 4 + key - '1');
                        gEMVTradeParam->DispSeqListData.SelectedResult = EMV_OK;
                        return;
                    }
                }
                    break;

                case SDK_KEY_ESC: {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    sdkDispClearScreen();
                    sdkIccDispStr(PLEASE_WAIT);
                    gEMVTradeParam->DispSeqListData.SelectedResult = ERR_EMV_CancelTrans;
                    return;
                }

                case SDK_KEY_UP:                           //�Ϸ�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);

                    if (curPage > 0) {
                        curPage--;
                    }
                    sdkDispClearScreen();
                    sdkIccDispStr(TITLE_APP_SELECT);
                    sdkIccDispAppSelectPage(totalPage, curPage, type);
                }
                    break;

                case SDK_KEY_DOWN:                      //�·�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);

                    if (curPage < totalPage) {
                        curPage++;
                    }
                    sdkDispClearScreen();
                    sdkIccDispStr(TITLE_APP_SELECT);
                    sdkIccDispAppSelectPage(totalPage, curPage, type);
                }
                    break;

                default: {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
                    break;
            }     //lint !e788
        }
    }
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccDispAppSelectPage(u8 totalPage,u8 curPage,u8 type)
   ��������: ��ʾӦ���б�
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
void sdkIccDispAppSelectPage(u8 totalPage, u8 curPage, u8 type) {
    char scrBuf[32];
    u8 i, j, len;

    for (i = 0; i < 4; i++) {
        if (type == 2) {
            j = gEMVTradeParam->DispSeqListData.AppSeq[curPage * 4 + i];
        }
        else {
            j = (u8)(curPage * 4) + i;
        }

        if (curPage * 4 + i >= gEMVTradeParam->AppListCandidatenum) {
            break;
        }
        memset(scrBuf, 0, 32);
#if 0           //sxl?���������Ƿ���Ҫ����

        if(gstSdkAppListCandidate[j].bLocalName == 1)      //Merchant perferred name.
        {
            len = gstSdkAppListCandidate[j].AppLocalNameLen;

            if(len > (MAX_LINE_CHAR - 2))
            {
                len = MAX_LINE_CHAR - 2;
            }

            if(len > sizeof(gstSdkAppListCandidate[j].AppLocalName))
            {
                len = sizeof(gstSdkAppListCandidate[j].AppLocalName);
            }
            memcpy(scrBuf + 2, gstSdkAppListCandidate[j].AppLocalName, len);
        }
        else
#endif

        if (gEMVTradeParam->AppListCandidate[j].PreferNameLen != 0 &&
            gEMVTradeParam->AppListCandidate[j].ICTI ==
            0x01)              //Preferred name in card //huxiang 20090702
        {
            len = gEMVTradeParam->AppListCandidate[j].PreferNameLen;

            if (len > (MAX_LINE_CHAR - 2)) {
                len = MAX_LINE_CHAR - 2;
            }

            if (len > sizeof(gEMVTradeParam->AppListCandidate[j].PreferName)) {
                len = sizeof(gEMVTradeParam->AppListCandidate[j].PreferName);
            }
            memcpy(scrBuf + 2, gEMVTradeParam->AppListCandidate[j].PreferName, len);
        }
        else if (gEMVTradeParam->AppListCandidate[j].AppLabelLen != 0)      //Label in card
        {
            len = gEMVTradeParam->AppListCandidate[j].AppLabelLen;

            if (len > (MAX_LINE_CHAR - 2)) {
                len = MAX_LINE_CHAR - 2;
            }

            if (len > sizeof(gEMVTradeParam->AppListCandidate[j].AppLabel)) {
                len = sizeof(gEMVTradeParam->AppListCandidate[j].AppLabel);
            }
            memcpy(scrBuf + 2, gEMVTradeParam->AppListCandidate[j].AppLabel, len);
        }
        else        //others display AID
        {
            len = gEMVTradeParam->AppListCandidate[j].AIDLen;

            if (len > (MAX_LINE_CHAR - 2) / 2) {
                EMVbcd_to_asc((u8 *) scrBuf + 2, gEMVTradeParam->AppListCandidate[j].AID,
                              MAX_LINE_CHAR - 2);
            }
            else {
                EMVbcd_to_asc((u8 *) scrBuf + 2, gEMVTradeParam->AppListCandidate[j].AID, len * 2);
            }
        }
        scrBuf[0] = (u8)(i + 0x31);
        scrBuf[1] = '.';
        sdkIccScrDisplay(CFONT, i + 1, 0, scrBuf);
    }

    if (curPage < totalPage) {
        sdkDispRowAt(SDK_DISP_LINE5, 0, "��", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    }

    if (curPage > 0) {
        sdkDispRowAt(SDK_DISP_LINE2, 0, "��", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    }
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostUnselectApp(void)
   ��������: û��ѡ��Ӧ��
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug  //sxl20140408
 ********************************************************************/
void sdkIccPostUnselectApp(void) {
    sdkDispClearScreen();
    Verify(sdkDispFillRowRam(SDK_DISP_LINE3, 0, "û��ѡ��Ӧ��", SDK_DISP_FDISP | SDK_DISP_CDISP) ==
           SDK_OK);
    sdkDispBrushScreen();

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostOfflinePinErr(void)
   ��������: ��ʾ�������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug  //sxl20140408
 ********************************************************************/
void sdkIccPostOfflinePinErr(void) {
    sdkSysBeep(SDK_SYS_BEEP_ERR);
    sdkDispClearScreen();
    sdkIccDispStr(INCORRECT_PIN);

    sdkmSleep(1000);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: u8 sdkIccCountTransRecord(u8 *TransRecordSFI,u8 *TransRecordNum)
   ��������: ���㿨�еĽ��׼�¼����
   ��ڲ���:    RecordType: 0:������־��1:Ȧ����־
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:    //sxl20140408
 ********************************************************************/
u8 sdkIccCountTransRecord(u8 *TransRecordSFI, u8 *TransRecordNum, u8 RecordType) {
    unsigned short k, j, len, t;
    u32 uiIndex;
    u8 tag[2] = {0};
    EMVTAGCVLITEM *tagitem;
    u8 tempIssuerDiscret[256];
    u8 tempIssuerDiscretLen = 0;

    uiIndex = 0;

    if (RecordType == 0) {
        memcpy(tag, "\x9F\x4D", 2);
    }
    else if (RecordType == 1) {
        memcpy(tag, "\xDF\x4D", 2);
    }
    else {
        return EMV_ERR;
    }
    memset(tempIssuerDiscret, 0, sizeof(tempIssuerDiscret));

    tagitem = dllemvb_avl_gettagitempointer(TAG_FCIDisData);     //sxl?? ����

    if (tagitem != NULL) {
        if (tagitem->len) {
            tempIssuerDiscretLen = tagitem->len;

            if (tempIssuerDiscretLen > 250) {
                tempIssuerDiscretLen = 250;
            }
            memcpy(tempIssuerDiscret, tagitem->data, tempIssuerDiscretLen);
        }
    }

    while (uiIndex < tempIssuerDiscretLen) {
        if (uiIndex >= tempIssuerDiscretLen) {
            return EMV_ERR;
        }

        if (tempIssuerDiscret[uiIndex] == 0xFF || tempIssuerDiscret[uiIndex] == 0x00) {
            uiIndex++;
            continue;
        }
        else if (!memcmp(tempIssuerDiscret + uiIndex, tag, 2))       //
        {
            uiIndex += 2;

            if (tempIssuerDiscret[uiIndex] != 2) {
                return EMV_ERR;
            }
            uiIndex++;
            (*TransRecordSFI) = tempIssuerDiscret[uiIndex];
            uiIndex++;
            (*TransRecordNum) = tempIssuerDiscret[uiIndex];

            if ((*TransRecordNum) > 10) {
                return EMV_ERR;
            }
            return EMV_OK;
        }
        else        //other unknown TLV data
        {
            k = tempIssuerDiscret[uiIndex];

            if ((k & 0x1F) == 0x1F) {
                uiIndex++;
            }
            uiIndex++;

            if (tempIssuerDiscret[uiIndex] <= 127) {
                len = tempIssuerDiscret[uiIndex];
                uiIndex++;
            }
            else {
                len = 0;
                t = tempIssuerDiscret[uiIndex] & 0x7F;

                for (j = 1; j <= t; j++) {
                    len = (u16)(len * 256 + tempIssuerDiscret[uiIndex + j]);
                }

                uiIndex += t + 1;
            }
            uiIndex += len;
        }
    }

    return EMV_OTHER;
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: u8 sdkIccPostReadLogCommand(APDU_SEND *apdu_s)
   ��������: �����������־����
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:   //sxl20140408
 ********************************************************************/
u8 sdkIccPostReadLogCommand(SDK_ICC_APDU_SEND *apdu_s, u8 RecordType) {
    memcpy(apdu_s->Command, "\x80\xCA", 2);

    if (RecordType == 0) {
        memcpy((u8 * ) & apdu_s->Command[2], "\x9F\x4F", 2);     //������־
    }
    else {
        memcpy((u8 * ) & apdu_s->Command[2], "\xDF\x4F", 2);     //Ȧ����־
    }
    apdu_s->Lc = 0;
    apdu_s->Le = 256;     //Le=0(256)
    apdu_s->EnableCancel = 1;
    return 0;
}

/*******************************************************************
   ��    ��: ��Т��
   ��������:
   ��������: EMV����������
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
u8 sdkIccDealICCParamRecord(const SDK_ICC_APDU_RESP *apdu_r, u8 *aucDOL, s32 siAucLen,
                            u8 RecordType) {
    u8 ucDOLLen;
    u8 tag[2] = {0};

    if (RecordType == 0) {
        memcpy(tag, "\x9F\x4F", 2);
    }
    else if (RecordType == 1) {
        memcpy(tag, "\xDF\x4F", 2);
    }
    else {
        return EMV_ERR;
    }

    if (!(apdu_r->SWA == 0x90 && apdu_r->SWB == 0x00) ||
        siAucLen < 0)     // ICC parameter Records found fail.
    {
        return EMV_ERR;
    }
    else {
        if (memcmp(apdu_r->DataOut, tag, 2)) {
            return EMV_ERR;
        }
        else {
            if (apdu_r->DataOut[2] <= siAucLen) {
                ucDOLLen = apdu_r->DataOut[2];
            }
            else {
                ucDOLLen = siAucLen;
            }
            memset(aucDOL, 0x00, siAucLen);
            memcpy(aucDOL, &apdu_r->DataOut[3], ucDOLLen);
            return EMV_OK;
        }
    }
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostOfflinePinSuc(void)
   ��������: ��ʾ������ȷ
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug     //sxl20140408
 ********************************************************************/
void sdkIccPostOfflinePinSuc(void) {
    sdkDispClearScreen();
    sdkIccDispStr(PIN_OK);

    sdkmSleep(1000);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostTransApproved(void)
   ��������: ��ʾ������׼
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug   //sxl20140408
 ********************************************************************/
void sdkIccPostTransApproved(u8 CardType) {
    sdkDispClearScreen();
    sdkIccDispStr(APPROVED);

    if (!CardType) {
        sdkIccDispStr(REMOVE_CARD);           //��ʾ�ο�
    }
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��    ��: ��Т��
   ��������: void sdkIccPostTransComplete(void)
   ��������: ��ʾ�������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ: emv_debug   //sxl20140408
 ********************************************************************/
void sdkIccPostTransComplete(void) {
    sdkDispClearScreen();
    sdkIccDispStr(TRANS_COMPLETED);
    sdkIccDispStr(REMOVE_CARD);       //��ʾ�ο�

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: EMV����������
   ��ڲ���: 6λBCD���
   �� �� ֵ:
   ��	  ע:    //sxl20140408
 ********************************************************************/
void sdkIccImportAmount(const u8 *lpAmount) {
    u32 tmpsdkAmtAuthBin;
    u8 tmpAmtAuthBin[4];

    sdkBcdToU32(&tmpsdkAmtAuthBin, lpAmount, 6);
    EMVIntToByteArray(tmpsdkAmtAuthBin, tmpAmtAuthBin, 4);

    dllemvb_avl_createsettagvalue(TAG_AmtAuthNum, lpAmount, 6);
    dllemvb_avl_createsettagvalue(TAG_AmtAuthBin, tmpAmtAuthBin, 4);

    memcpy(gPayData.amount, lpAmount, 6);
}

//sxl20140408
void sdkIccDealqPBOCDispAppList(const SDK_ICC_TRADE_PARAM *pstIccTradeParam) {
#if 0

    if(1 > gucSdkAppNum)
    {
    }
    else
    {
        gstSdkDispSeqListData.SelectedNum = 0;
        gstSdkDispSeqListData.SelectedResult = EMV_OK;
    }
#endif
}

//sxl20140408
void sdkIccDealDispAppList(const SDK_ICC_TRADE_PARAM *pstIccTradeParam) {
    //u8 **pAppList=NULL;
    SDK_ICC_AIDLIST **pAppList = NULL;    //zcl 20130702
    u8 i = 0, j = 0, len = 0;
    s32 selectNO = 0;

    Trace("emv", "-------------\r\n goto->sdkIccDealDispAppList\r\n");

    //pAppList = (u8**)xgd_malloc(16);
    if (gEMVTradeParam == NULL) {
        return;
    }
    pAppList = (SDK_ICC_AIDLIST **) sdkGetMem(sizeof(SDK_ICC_AIDLIST * ) *
                                              gEMVTradeParam->AppListCandidatenum);        //zcl 20130702   //sxl20140110

    if (NULL == pAppList) {
        return;
    }

    for (i = 0; i < gEMVTradeParam->AppListCandidatenum; i++) {
        Trace("emv", "-------------i=%d,appnum=%d,address = %x\r\n", i,
              gEMVTradeParam->AppListCandidatenum, pAppList + i);
        //*(pAppList+i) = (u8*)xgd_malloc(32);
        *(pAppList + i) = (SDK_ICC_AIDLIST *) sdkGetMem(
                sizeof(SDK_ICC_AIDLIST)); //zcl 20130702  //sxl20140110

        if (*(pAppList + i) == NULL) {
            break;
        }
        //memset(*(pAppList+i), 0, 32);
        memset(*(pAppList + i), 0, sizeof(SDK_ICC_AIDLIST));       //zcl 20130702

        if (gEMVTradeParam->DispSeqListData.seqtype == 2) {
            j = gEMVTradeParam->DispSeqListData.AppSeq[i];
        }
        else {
            j = i;
        }
        (*(pAppList + i))->ucAidLen =
                gEMVTradeParam->AppListCandidate[j].AIDLen * 2; ///zxx 20130719
        len = gEMVTradeParam->AppListCandidate[j].AIDLen;
        EMVbcd_to_asc((*(pAppList + i))->asAid, gEMVTradeParam->AppListCandidate[j].AID, len * 2);

        if (len % 2) {
            (*(pAppList + i))->asAid[len * 2] = 0;
        }
        (*(pAppList + i))->ucAppLabelLen = gEMVTradeParam->AppListCandidate[j].AppLabelLen;
        len = gEMVTradeParam->AppListCandidate[j].AppLabelLen;
        memcpy((*(pAppList + i))->asAppLabel, gEMVTradeParam->AppListCandidate[j].AppLabel, len);
        (*(pAppList + i))->ucPreferNameLen = gEMVTradeParam->AppListCandidate[j].PreferNameLen;
        len = gEMVTradeParam->AppListCandidate[j].PreferNameLen;
        memcpy((*(pAppList + i))->asPreferName, gEMVTradeParam->AppListCandidate[j].PreferName,
               len);
        (*(pAppList + i))->ucPriority = gEMVTradeParam->AppListCandidate[j].Priority;
        (*(pAppList + i))->ucLangPreferLen = gEMVTradeParam->AppListCandidate[j].LangPreferLen;
        len = gEMVTradeParam->AppListCandidate[j].LangPreferLen;
        memcpy((*(pAppList + i))->asLangPrefer, gEMVTradeParam->AppListCandidate[j].LangPrefer,
               len);
        (*(pAppList + i))->ucIcti = gEMVTradeParam->AppListCandidate[j].ICTI;
        //sxl?AppLocalNameLen�Ƿ���ڣ���Ҫ����
        //(*(pAppList+i))->ucLocalName = gEMVTradeParam->AppListCandidate[j].bLocalName;
        // (*(pAppList+i))->ucAppLocalNameLen = gEMVTradeParam->AppListCandidate[j].AppLocalNameLen;
        //len = gstSdkAppListCandidate[j].AppLocalNameLen;
        //memcpy((*(pAppList+i))->asAppLocalName, gstSdkAppListCandidate[j].AppLocalName, len);
        //end
    }

    if (i != gEMVTradeParam->AppListCandidatenum) {
        for (j = 0; j < i; j++) {
            sdkFreeMem(*(pAppList + j));
        }

        sdkFreeMem(pAppList);
        return;
    }
    Trace("emv", "i=%d, papplist=%x\r\n", i, pAppList);
    selectNO = (*pstIccTradeParam->DispCandAppList)(i, (u8 **) pAppList);

    if (0 < selectNO && gEMVTradeParam->AppListCandidatenum >= selectNO) {
        gEMVTradeParam->DispSeqListData.SelectedNum = selectNO - 1;
        gEMVTradeParam->DispSeqListData.SelectedResult = EMV_OK;
    }
    else {
        gEMVTradeParam->DispSeqListData.SelectedResult = ERR_EMV_CancelTrans;
    }

    for (j = 0; j < i; j++) {
        sdkFreeMem(*(pAppList + j));
    }

    sdkFreeMem(pAppList);
    return;
}

/*******************************************************************
   ��    ��: ����
   ��    Ȩ: Okolle Ltd,Co.
   ��������: u8 sdkIccGetInterCode(u8 *InterCode)
   ��������: ȡ�������ÿ�����
   ��ڲ���:
   �� �� ֵ: OKȡ����,ERRûȡ��.ȡ����ŵ�InterCodeָ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:   //sxl20140408
 ********************************************************************/
static const u8 RidTab[4][5] =
        {
                {0xA0, 0x00, 0x00, 0x03, 0x33},
                {0xA0, 0x00, 0x00, 0x00, 0x03},
                {0xA0, 0x00, 0x00, 0x00, 0x04},
                {0xA0, 0x00, 0x00, 0x00, 0x65}
        };

static const u8 InterCodeTab[4][4] = {
        {"CUP"},
        {"VIS"},
        {"MCC"},
        {"JCB"},
};

//sxl20140408
u8 sdkIccGetInterCode(u8 *InterCode, const u8 *rid) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (!memcmp(RidTab[i], rid, 5)) {
            memcpy(InterCode, InterCodeTab[i], 3);
            return EMV_OK;
        }
    }

    return EMV_ERR;
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �������
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
void sdkIccClearEmvData(void) {
    //gstSdkTermInfo.POSEntryMode = IC_SUCCESS;   //sxl?���ӱ�����δ������ֵ
}

/*******************************************************************
   ��    ��: huxiang
   ��������: void sdkIccEmvFlushCom(u8 ComNo)
   ��������: ��ʾ��⵽���ſ�
   ��ڲ���: ��
   �� �� ֵ: 0 :ȡ�� 1:ȷ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
void sdkIccEmvFlushCom(u8 ComNo) {
//        sdk_dev_com_flush(ComNo, 10);
}

/*******************************************************************
   ��    ��: huxiang
   ��������: void sdkIccPostCVMNoSupport(void)
   ��������: ��ʾ��������
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
void sdkIccPostCVMNoSupport(void) {
    sdkDispClearScreen();
    sdkIccDispStr(TERM_CVMNOSUPPORT);
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
}

/*******************************************************************
   ��    ��: huxiang
   ��������: void sdkIccPostqPBOCDispAppList(void)
   ��������: �ṩӦ���б���û�ѡ��
   ��ڲ���:
   �� �� ֵ:
   ��ص���:
   ��    ע: huxiang add in beijing
   �޸���Ϣ:  //sxl20140408
 ********************************************************************/
void sdkIccPostqPBOCDispAppList(void) {
#if 0
    u8 key;
//    u8 type;
    u8 curPage;

//    u8 totalPage;

//    totalPage = (gucSdkAppNum - 1) / 4;
    curPage = 0;
//    type = DispSeqListData.seqtype;
    sdkDispClearScreen();
    sdkIccDispStr(TITLE_APP_SELECT);

    key = 0x31;


    if( (curPage * 4 + key - '0') > gucSdkAppNum )
    {
        //sdk_dev_beep_err();
    }
    else
    {
        //sdk_dev_beep_ok();
        sdkDispClearScreen();
        sdkIccDispStr(PLEASE_WAIT);
        gstSdkDispSeqListData.SelectedNum = (u8)(curPage * 4 + key - '1');
        gstSdkDispSeqListData.SelectedResult = EMV_OK;
        return;
    }
#endif
    DISPSEQLISTDATA *tempdispseqlistdata;

    tempdispseqlistdata = &(GlbEMVBApp_UnionStruct->EMVTradeParam->DispSeqListData);


    tempdispseqlistdata->SelectedResult = EMV_OK;
    tempdispseqlistdata->SelectedNum = 0;
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ��TermInfo�ṹ�е�����ת��AID�б�ṹ��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:    //sxl20140408  //sxl?Ӧ��������������
 ********************************************************************/
void sdkIccAidToAppList(void) {
#if 0
    u8 count, i;

    if(gucSdkAidParamNum < sizeof(gstSdkAppListTerm) / sizeof(gstSdkAppListTerm[0]))
    {
        count = gucSdkAidParamNum;
    }
    else
    {
        count = sizeof(gstSdkAppListTerm) / sizeof(gstSdkAppListTerm[0]);
    }

    for(i = 0; i < count; i++)
    {
        gstSdkAppListTerm[i].ASI = gstSdkAidParam[i].Asi;
        gstSdkAppListTerm[i].AIDLen = gstSdkAidParam[i].AidLen;
        memcpy(&gstSdkAppListTerm[i].AID[0], &gstSdkAidParam[i].Aid[0], 16);
    }

    gucSdkTermAppNum = count;
#endif
}

//void sdkIccDispNoAmount(void)
//{
//    sdkDispClearScreen();
//    Verify(sdkDispFillRowRam(SDK_DISP_LINE4, 0, "����ʧ��,δ������!!!", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL) == SDK_OK);
//    sdkDispBrushScreen();
//    sdkKbWaitKey(SDK_KEY_MASK_ALL, SDK_ICC_TIMER_AUTO);
//}

/*****************************************************************************
** Descriptions:    ��PDOL��ƥ���Ӧ��tag
** Parameters:      u8 *pheTag
** Returned value:  true:��  false:��
** Created By:      sujianzhong   2012.11.06
** Remarks:         //sxl20140408   //sxl ����������÷��Ըı䣬�鿴�����ط��Ƿ���õ�
*****************************************************************************/
bool Private_sdkIccMatchTagFromPDOL(const u8 *pheTag) {
    u8 PDOL[300] = {0};
    u16 PDOLLen;
    u8 taglen;
    u8 tag_len;
    u16 uiIndex;


    dllemvb_avl_gettagvalue_all(TAG_PDOL, PDOL, &PDOLLen);

    if (NULL == pheTag || PDOLLen == 0) {
        return false;
    }
    uiIndex = 0;
    tag_len = 1;

    if ((0x1F & pheTag[0]) == 0x1F) {
        tag_len = 2;
    }

    while (uiIndex < PDOLLen)     //Process PDOL
    {
        if (PDOL[uiIndex] == 0xFF || PDOL[uiIndex] == 0x00) {
            uiIndex++;
            continue;
        }
        taglen = 0;

        if ((PDOL[uiIndex] & 0x1f) == 0x1f) {
            taglen++;

            if (PDOL[uiIndex + 1] & 0x80) {
                taglen++;
            }
        }
        taglen++;

        if (taglen == tag_len) {
            if (!memcmp((u8 * ) & PDOL[uiIndex], pheTag,
                        tag_len))         //VLP terminal support indicator
            {
                return true;
            }
        }
        uiIndex += taglen + 1;        //DOL���ݳ��ȶ�ֻ��һ���ֽ�
    }

    return false;
}

//=================�ֻ�֧��======================

#if 0

typedef struct
{
    u8 MobileVerLen;     //�ƶ�֧���汾�ų���
    u8 MobileVer;     //�ƶ�֧���汾��
    u8 MobileVerbexit;     //�ƶ�֧���汾�Ŵ��ڱ�־
    u8 BCInfoFileEF04Len;     //���п���Ϣ�ļ�(EF04)��¼������
    u8 BCInfoFileEF04;     //���п���Ϣ�ļ�(EF04)��¼��
    u8 BCInfoFileEF04bexit;     //���п���Ϣ�ļ�(EF04)��¼�����ڱ�־
    u8 MobileCardInfoRecord[108];     //����Ϣ��¼����
    u8 MobileProcessKey[8];     //�ƶ�֧���Ĺ�����Կ
    u8 MobileCardSeq[10];     //�ƶ�֧����Ƭ���к�
    u8 MobileCardName[20];     //�ƶ�֧��������
    u8 MobileCardNo[10];     //�ƶ�֧������
    u8 Mobile2Equ[20];     //�ƶ�֧�����ŵ���Ч����
    u8 Mobile3Equ[54];     //�ƶ�֧�����ŵ���Ч����
    u8 MobileExpiredData[2];     //�ƶ�֧����Ч��
    u8 MobileGetTrackTime[8];     //�ƶ�֧���õ��ŵ����ݵ�ʱ��
}SDK_UPCARD_PARAM;

static SDK_UPCARD_PARAM gstSdkUpcardParam;  //sxl?���Ӧ�ã�����Щ����ʡ��
//extern AppUnionStruct gstSdkAppUnion;
//extern s32 sdkIccEmvToSdkRlt(u8 ucInBuf, s32 *puiOutBuf);
/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_ReadDFSelectRetData(SELECT_RET* selectRet,unsigned char* DataOut,unsigned short LenOut,CARDINFO *termpcardinfo)
   ��������: ����SELECT����صĿ�����
   ��ڲ���: selectRet - ���ļ�ѡ��Ӧ��ṹ��
          DataOut -  �����ص�����
          LenOut -   �����ص����ݳ���
   �� �� ֵ: EMV_OK��0��-�ɹ�
          ERR_EMV_IccDataFormat��34�� - IC�����ݸ�ʽ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardReadDFSelectRetData(SELECT_RET* selectRet, unsigned char* DataOut, unsigned short LenOut, AppUnionStruct *tempAppUnionStruct)
{
    unsigned short index, indexFCI, len, lenFCI;
    unsigned short k;

    index = 0;

    if(DataOut[index] != 0x70)          //�ƶ�֧��Ӧ�ñ�ʶ
    {
        return ERR_EMV_IccDataFormat;         //FCI template
    }
    index++;

    if(ParseExtLen(DataOut, &index, &lenFCI))
    {
        return ERR_EMV_IccDataFormat;
    }
    indexFCI = index;

    while(index < indexFCI + lenFCI)
    {
        if(index >= LenOut) { return ERR_EMV_IccDataFormat; }

        if(DataOut[index] == 0xFF || DataOut[index] == 0x00) { index++; continue; }
        else if(DataOut[index] == 0x51)         //DF��
        {
            index++;
            gstSdkUpcardParam.MobileVerLen = DataOut[index];

            if(gstSdkUpcardParam.MobileVerLen != 1) { return ERR_EMV_IccDataFormat; }
            index++;
            gstSdkUpcardParam.MobileVer = DataOut[index];
            index++;
            gstSdkUpcardParam.MobileVerbexit = 1;
        }
        else if(DataOut[index] == 0x52)         //�ƶ�֧�����п���Ϣ�ļ�(EF04)��¼��
        {
            gstSdkUpcardParam.BCInfoFileEF04Len = DataOut[index + 1];

            if(gstSdkUpcardParam.BCInfoFileEF04Len != 1)
            {
                return ERR_EMV_EmvDataFormat;                                   //with wrong len of SFI,select next app.//SFI�ļ����ȱ���ҪΪ1
            }
            index += 2;
            gstSdkUpcardParam.BCInfoFileEF04 = DataOut[index];
            index++;
            gstSdkUpcardParam.BCInfoFileEF04bexit = 1;
        }
        else
        {
            k = DataOut[index];

            if((k & 0x1F) == 0x1F)
            {
                index++;
            }
            index++;

            if(ParseExtLen(DataOut, &index, &len))
            {
                return ERR_EMV_IccDataFormat;
            }
            index += len;
        }
    }

    if(index != indexFCI + lenFCI)
    {
        return ERR_EMV_IccDataFormat;
    }
    return EMV_OK;
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_SelectDFRetData(unsigned char indexDF,SELECT_RET* selectRet,APDU_RESP *apdu_r,DFData *tempAppDfData)
   ��������: ����SELECT����صĿ�����
   ��ڲ���: indexDF - DF����
          SELECT_RET -  ѡ���ļ��ṹ��
          apdu_r -   �����ص�����
          DFData -   DF���ݽṹ
   �� �� ֵ: EMV_OK��0��-�ɹ�
          ERR_EMV_IccDataFormat��34�� - IC�����ݸ�ʽ��
          ERR_EMV_IccReturn -�������󷵻�
          ERR_BLOCKORFORMATERR -����
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardSelectDFRetData(unsigned char indexDF, SELECT_RET* selectRet, APDU_RESP *apdu_r, AppUnionStruct *tempAppUnionStruct)
{
    if(apdu_r->SWA == 0x90 && apdu_r->SWB == 0x00)
    {
        memset((unsigned char*)selectRet, 0, sizeof(SELECT_RET));

        if(sdkUpcardReadDFSelectRetData(selectRet, apdu_r->DataOut, apdu_r->LenOut, tempAppUnionStruct) != EMV_OK)
        {
            return ERR_EMV_IccDataFormat;
        }
        return EMV_OK;
    }
    else if(apdu_r->SWA == 0x6A && apdu_r->SWB == 0x81)     //card block or command not supported,terminate session.
    {
        return ERR_BLOCKORFORMATERR;                        //sxl ������Ҫע��
    }
    else
    {
        return ERR_EMV_IccReturn;
    }
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_SelectDF(unsigned char indexDF,AppUnionStruct *tempAppUnionStruct)
   ��������: ѡ��DF�ļ�
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardSelectDF(unsigned char indexDF, AppUnionStruct *tempAppUnionStruct)
{
    unsigned char retCode;
    unsigned char DFName[16], DFNameLen;
    SELECT_RET selectRet;

    APDU_SEND *apdu_s;
    APDU_RESP *apdu_r;
    DFData *tempAppDfData;
    EMVSTATE *tempemvstate;
    LOCALVARIBLE *templocalvar;

    tempAppDfData = tempAppUnionStruct->AppDfData;
    apdu_s = tempAppUnionStruct->Termapdu_s;
    apdu_r = tempAppUnionStruct->Termapdu_r;
    tempemvstate = tempAppUnionStruct->emvstate;
    templocalvar = tempAppUnionStruct->localvar;

    while(1)
    {
        switch(tempemvstate->stateB)
        {
             case EMVFIRST:
                {
                    DFNameLen = (tempAppDfData + indexDF)->DFNameLen;
                    memcpy(DFName, (tempAppDfData + indexDF)->DFName, DFNameLen);

                    COMMAND_SELECT(DFName, DFNameLen, 0, apdu_s);
                    tempemvstate->stateB = WAITCARDDATA;
                    return EMV_NEEDSENDCOMMAND;
                }
                break;

             case WAITCARDDATA:
                {
                    if(apdu_r->ReadCardDataOk == COMMANDERR)                        //����ʧ��
                    {
                        return ERR_EMV_IccCommand;
                    }
                    else if(apdu_r->ReadCardDataOk == COMMTIMEOUT)                  //������ʱ
                    {
                        return ERR_APP_TIMEOUT;
                    }
                    else
                    {
                        retCode = sdkUpcardSelectDFRetData(indexDF, &selectRet, apdu_r, tempAppUnionStruct);

                        if(retCode == EMV_OK)
                        {
                            templocalvar->bSelectPSE = 1;
                            return retCode;
                        }
                        else
                        {
                            return retCode;
                        }
                    }
                }
                break;

             default:
                {
                    return ERR_EMV_IccCommand;
                }
                break;
        }
    }
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_SelectApp(AppUnionStruct *tempApp_UnionStruct)
   ��������: �ֻ�оƬѡ��Ӧ��
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardSelectAppDetail(AppUnionStruct *tempApp_UnionStruct)
{
    unsigned char i;
    unsigned char retcode = 0;


    unsigned char *applistnum;
    DFData *tempdflisticc;
    unsigned char *tempdflistnum;
    DISPSEQLISTDATA *tempdispseqlistdata;
    EMVSTATE *tempemvstate;
    LOCALVARIBLE *templocalvar;


    tempdflisticc = tempApp_UnionStruct->AppDfData;
    tempdflistnum = tempApp_UnionStruct->dflistnum;

    applistnum = tempApp_UnionStruct->appnum;
    tempdispseqlistdata = tempApp_UnionStruct->dispseqlist;
    tempemvstate = tempApp_UnionStruct->emvstate;
    templocalvar = tempApp_UnionStruct->localvar;

    while(1)
    {
        switch(tempemvstate->stateA)
        {
             case EMVFIRST:
                {
                    templocalvar->bSelectPSE = 0;
                    templocalvar->SelectNextDDF = 0;
                    templocalvar->DFNum = 0;
                    *applistnum = 0;

                    *(tempApp_UnionStruct->bretforminit) = 0;

                    for(i = 0; i < 16; i++)
                    {
                        tempdispseqlistdata->AppPriority[i] = 0;
                        tempdispseqlistdata->AppSeq[i] = i;
                    }

                    templocalvar->curDFNum = 0;
                    tempdflisticc->DFNameLen = 15;
                    //DFListIcc[0].Recordnum=1;
                    tempdflisticc->Recordnum = 1;
                    memcpy(tempdflisticc->DFName, "\xA0\x00\x00\x03\x33", 5);
                    memcpy(&tempdflisticc->DFName[5], "CUP-MOBILE", 10);
                    tempdflisticc->InsertAppNo = 0;
                    *tempdflistnum = 1;
                    tempemvstate->stateA = WAITCARDDATA;
                }
                break;

             case WAITCARDDATA:
                {
                    retcode = sdkUpcardSelectDF((*tempdflistnum) - 1, tempApp_UnionStruct);
                    //if(retcode==EMV_OK || retcode==ERR_EMV_CardBlock || retcode==ERR_EMV_IccCommand || ERR_EMV_IccDataFormat)    //xule120307  Ӧ�ų����ͳ¹��ʼ�ȥ���ж�����������ѭ��
                    //{
                    return retcode;

                    //}
                }
                break;

             default:
                {
                    return ERR_EMV_IccCommand;
                }
                break;
        }
    }
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_ReadEF02(AppUnionStruct *tempApp_UnionStruct)
   ��������: �ֻ�оƬѡ��Ӧ��
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardReadEF02Detail(AppUnionStruct *tempApp_UnionStruct)
{
    EMVSTATE *tempemvstate;
    APDU_SEND *apdu_s;
    APDU_RESP *apdu_r;
    u8 len = 10, times = 0;     //zjz20120308  xule120309 ��Ϊ�ȶ�10�� ����

    tempemvstate = tempApp_UnionStruct->emvstate;
    apdu_s = tempApp_UnionStruct->Termapdu_s;
    apdu_r = tempApp_UnionStruct->Termapdu_r;

    while(1)
    {
        switch(tempemvstate->stateA)
        {
             case EMVFIRST:
                {
                    memcpy(apdu_s->Command, "\x00\xB0\x82\x00", 4);           //��ȡEF02�ļ�
                    apdu_s->Lc = 0;
                    //apdu_s->Le=10;     //xule
                    //apdu_s->Le=0;      //zjz20120308 ���� ���Ʋ���
                    apdu_s->Le = len;              //zjz20120308 ���� ���Ʋ���
                    apdu_s->EnableCancel = 1;                   //ʹ�ܰ�ȡ����
                    tempemvstate->stateA = WAITCARDDATA;
                    return EMV_NEEDSENDCOMMAND;
                }
                break;

             case WAITCARDDATA:
                {
                    if(apdu_r->ReadCardDataOk == COMMANDERR)                            //����ʧ��
                    {
                        return ERR_EMV_IccCommand;
                    }
                    else if(apdu_r->ReadCardDataOk == COMMTIMEOUT)                  //������ʱ
                    {
                        return ERR_APP_TIMEOUT;
                    }
                    else
                    {
                        if(apdu_r->SWA == 0x90 && apdu_r->SWB == 0x00)             //Select successful
                        {
                            memset(gstSdkUpcardParam.MobileCardSeq, 0, sizeof(gstSdkUpcardParam.MobileCardSeq));
                            memcpy(gstSdkUpcardParam.MobileCardSeq, apdu_r->DataOut, 10);
                            return EMV_OK;
                        }
                        else
                        {
                            //��һ�ζ����ɹ������ù̶�ֵ��һ�� zjz20120308
                            if(times > 0)
                            {
                                return ERR_EMV_IccCommand;                          //�ٶ�һ�λ���ʧ��
                            }
                            else
                            {
                                tempemvstate->stateA = EMVFIRST;
                                times = 1;
                                //len = 10;
                                len = 0;                        //xule120309 ��Ϊ���0������
                            }
                            //========================================
                        }
                    }
                }
                break;

             default:
                {
                    return ERR_EMV_IccCommand;
                }
                break;
        }
    }
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_ReadEF02(AppUnionStruct *tempApp_UnionStruct)
   ��������: ��ȡĬ��֧�����п���¼
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardReadEF03Detail(AppUnionStruct *tempApp_UnionStruct)
{
    EMVSTATE *tempemvstate;
    APDU_SEND *apdu_s;
    APDU_RESP *apdu_r;
    u8 len = 8, times = 0;     //zjz20120308   //xule120309 ��Ϊ�ȶ�8������

    tempemvstate = tempApp_UnionStruct->emvstate;
    apdu_s = tempApp_UnionStruct->Termapdu_s;
    apdu_r = tempApp_UnionStruct->Termapdu_r;

    while(1)
    {
        switch(tempemvstate->stateA)
        {
             case EMVFIRST:
                {
                    memcpy(apdu_s->Command, "\x00\xB0\x83\x00", 4);           //��ȡEF02�ļ�
                    apdu_s->Lc = 0;
                    //apdu_s->Le=10;
                    //apdu_s->Le=0;  //zjz20120308
                    apdu_s->Le = len;           //zjz20120308  ͨ������������
                    apdu_s->EnableCancel = 1;                   //ʹ�ܰ�ȡ����
                    tempemvstate->stateA = WAITCARDDATA;
                    return EMV_NEEDSENDCOMMAND;
                }
                break;

             case WAITCARDDATA:
                {
                    if(apdu_r->ReadCardDataOk == COMMANDERR)                            //����ʧ��
                    {
                        return ERR_EMV_IccCommand;
                    }
                    else if(apdu_r->ReadCardDataOk == COMMTIMEOUT)                  //������ʱ
                    {
                        return ERR_APP_TIMEOUT;
                    }
                    else
                    {
                        if(apdu_r->SWA == 0x90 && apdu_r->SWB == 0x00)             //Select successful
                        {
                            return EMV_OK;
                        }
                        else
                        {
                            //zjz20120308  ��һ�ζ����ɹ������ٶ�һ�ι̶�ֵ��
                            if(times > 0)
                            {
                                return ERR_EMV_IccCommand;
                            }
                            else                      //�ٶ�һ�ι̶�ֵ
                            {
                                tempemvstate->stateA = EMVFIRST;
                                //len = 8;
                                len = 0;                        //xule120309 ��Ϊ���0������
                                times = 1;
                            }
                            //===============================
                        }
                    }
                }
                break;

             default:
                {
                    return ERR_EMV_IccCommand;
                }
                break;
        }
    }
}

/*******************************************************************
   ��    ��: huxiang
   ��������: unsigned char Mobile_GetBCInfo(AppUnionStruct *tempApp_UnionStruct)
   ��������: �ֻ�оƬѡ��Ӧ��
   ��ڲ���: ��
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
unsigned char sdkUpcardGetBCInfoDetail(AppUnionStruct *tempApp_UnionStruct)
{
    EMVSTATE *tempemvstate;
    APDU_SEND *apdu_s;
    APDU_RESP *apdu_r;
    u8 transTime[9];

    tempemvstate = tempApp_UnionStruct->emvstate;
    apdu_s = tempApp_UnionStruct->Termapdu_s;
    apdu_r = tempApp_UnionStruct->Termapdu_r;

    while(1)
    {
        switch(tempemvstate->stateA)
        {
             case EMVFIRST:

               if(gstSdkUpcardParam.BCInfoFileEF04bexit == 0)
               {
                   return ERR_EMV_IccCommand;
               }
               memcpy(apdu_s->Command, "\x80\xF8\x02\x00", 4);                //Get bankCard Info ��ȡĬ���ֻ��˻�
               apdu_s->Lc = 8;
#if 0

               if(gstSdkTermInfo.TransDate[0] > 0x50) { apdu_s->DataIn[0] = 0x19; }
               else{ apdu_s->DataIn[0] = 0x20; }
               memcpy(&apdu_s->DataIn[1], gstSdkTermInfo.TransDate, 3);
               memcpy(&apdu_s->DataIn[4], gstSdkTermInfo.TransTime, 3);
#endif
               memset(transTime, 0, sizeof(transTime));
               Verify(sdkGetRtc(transTime) == SDK_OK);

               if(transTime[0] > 0x50) { apdu_s->DataIn[0] = 0x19; }
               else{ apdu_s->DataIn[0] = 0x20; }
               memcpy(&apdu_s->DataIn[1], transTime, 3);
               memcpy(&apdu_s->DataIn[4], &transTime[3], 3); //zxx 20130711 15:36����֮ǰ���ˣ�ǰ������ֵһ��
               apdu_s->DataIn[7] = 0x80;
               memcpy(gstSdkUpcardParam.MobileGetTrackTime, apdu_s->DataIn, 8);
               apdu_s->Le = 256;
               apdu_s->EnableCancel = 1;                        //ʹ�ܰ�ȡ����
               tempemvstate->stateA = WAITCARDDATA;
               return EMV_NEEDSENDCOMMAND;

               break;

             case WAITCARDDATA:

               if(apdu_r->ReadCardDataOk == COMMANDERR)                                 //����ʧ��
               {
                   return ERR_EMV_IccCommand;
               }
               else if(apdu_r->ReadCardDataOk == COMMTIMEOUT)                       //������ʱ
               {
                   return ERR_APP_TIMEOUT;
               }
               else
               {
                   if(apdu_r->SWA == 0x90 && apdu_r->SWB == 0x00)                  //Select successful
                   {
                       memset(gstSdkUpcardParam.MobileCardInfoRecord, 0, sizeof(gstSdkUpcardParam.MobileCardInfoRecord));
                       memset(gstSdkUpcardParam.MobileCardName, 0, sizeof(gstSdkUpcardParam.MobileCardName));
                       memset(gstSdkUpcardParam.MobileCardNo, 0, sizeof(gstSdkUpcardParam.MobileCardNo));
                       memset(gstSdkUpcardParam.Mobile2Equ, 0, sizeof(gstSdkUpcardParam.Mobile2Equ));
                       memset(gstSdkUpcardParam.Mobile3Equ, 0, sizeof(gstSdkUpcardParam.Mobile3Equ));
                       memset(gstSdkUpcardParam.MobileExpiredData, 0, sizeof(gstSdkUpcardParam.MobileExpiredData));
                       memset(gstSdkUpcardParam.MobileProcessKey, 0, sizeof(gstSdkUpcardParam.MobileProcessKey));

                       if(apdu_r->LenOut != 116) { return ERR_EMV_IccCommand; }
                       memcpy(gstSdkUpcardParam.MobileCardInfoRecord, apdu_r->DataOut, 108);
                       memcpy(gstSdkUpcardParam.MobileCardName, apdu_r->DataOut, 20);
                       memcpy(gstSdkUpcardParam.MobileCardNo, &apdu_r->DataOut[20], 10);
                       memcpy(gstSdkUpcardParam.Mobile2Equ, &apdu_r->DataOut[30], 20);
                       memcpy(gstSdkUpcardParam.Mobile3Equ, &apdu_r->DataOut[50], 54);
                       memcpy(gstSdkUpcardParam.MobileExpiredData, &apdu_r->DataOut[104], 2);
                       memcpy(gstSdkUpcardParam.MobileProcessKey, &apdu_r->DataOut[108], 8);
                       return EMV_OK;
                   }
                   else
                   {
                       return ERR_EMV_IccCommand;
                   }
               }
               break;

             default:
               return ERR_EMV_IccCommand;

               break;
        }
    }
}

/*****************************************************************************
** Descriptions:    ��ʼ������
** Parameters:      void
** Returned value:  SDK_OK
                    SDK_ERR
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpCardTransInit(void)
{
/*=======BEGIN: sujianzhong 2013.01.30  16:12 modify===========*/
//    DFData DFListIcc[8];
//    u8 DFNum;
/*====================== END======================== */

    memset(&gstSdkUpcardParam, 0, sizeof(gstSdkUpcardParam));
    memset(gstSdkUpcardParam.MobileCardNo, 0xFF, sizeof(gstSdkUpcardParam.MobileCardNo));
    gstSdkAppUnion.AppDfData = &gstSdkDFListIcc[0];
    gstSdkAppUnion.dflistnum = &gucSdkDFNum;
    gstSdkAppUnion.AppTermApp = &gstSdkAppListTerm[0];
    gstSdkAppUnion.termlistnum = &gucSdkTermAppNum;
    gstSdkAppUnion.AppAppData = &gstSdkAppListCandidate[0];
    gstSdkAppUnion.appnum = &gucSdkAppNum;
    gstSdkAppUnion.selectedapp = &gstSdkSelectedApp;
    gstSdkAppUnion.selectedappno = &gucSdkSelectedAppNo;
    gstSdkAppUnion.appblockflag = &gucSdkAppBlockFlag[0];
    gstSdkAppUnion.bretforminit = &gucSdkbRetFromInitApp;
    gstSdkAppUnion.iccdatatable = &gstSdkICCDataTable[0];
    gstSdkAppUnion.termdatatable = &gstSdkTermDataTable[0];
    gstSdkAppUnion.cardinfo = &gstSdkCardInfo;
    gstSdkAppUnion.terminfo = &gstSdkTermInfo;
    gstSdkAppUnion.aidparam = &gstSdkAidParam[0];
    gstSdkAppUnion.aidnum = &gucSdkAidParamNum;
    gstSdkAppUnion.emvcapk = &gstSdkCAPK;
    gstSdkAppUnion.termcapk = &gstSdkTermCAPK[0];
    gstSdkAppUnion.termcapknum = &gucSdkTermCAPKNum;
    gstSdkAppUnion.emvTermIPKRevoke = &gstSdkTermIPKRevoke[0];
    gstSdkAppUnion.emvTermIPKRevokeNum = &gucSdkTermIPKRevokeNum;
    gstSdkAppUnion.TempTransNum = &guiSdkTransNum;
    gstSdkAppUnion.TempIndexEnd = &gusSdkIndexEnd;
#if 0 //del by sujianzhong 2013.11.08
    gstSdkAppUnion.TempRecordIndex = &gpucSdkRecordIndex[0][0];
#else
    gstSdkAppUnion.TempRecordIndex = NULL;
#endif
    gstSdkAppUnion.SmsTempAid = &gucSdkPosSmsTempAid[0];
    gstSdkAppUnion.dispseqlist = &gstSdkDispSeqListData;
#if 0 //del by sujianzhong 2013.11.08
    gstSdkAppUnion.TempTermTransLog = &gstSdkTermTransLog[0];     ////sujianzhong  2012.10.11 8:30
#else
    gstSdkAppUnion.TempTermTransLog = NULL;     ////sujianzhong  2012.10.11 8:30
#endif
//    gstSdkAppUnion.TempTermTransLog = &gstSdkEmvTransLog[0];
    gstSdkAppUnion.Termapdu_r = &gstSdkAPDU_R;
    gstSdkAppUnion.Termapdu_s = &gstSdkAPDU_S;
    gstSdkAppUnion.emvstate = &gstSdkEmvState;
    gstSdkAppUnion.localvar = &gstSdkLocalVar;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ѡ��Ӧ��
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_PARA_ERR
                    SDK_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
                    SDK_IccDataFormatErr
                    SDK_CardBlockErr
                    SDK_IccReturnErr
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardSelectApp(const SDK_ICC_PARAM *pstIccParam)
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if(NULL == pstIccParam)
    {
        return SDK_PARA_ERR;
    }
    InitLocalVarible(&gstSdkAppUnion);

    while(1)
    {
        retCode = sdkUpcardSelectAppDetail(&gstSdkAppUnion);

        if(retCode == EMV_NEEDSENDCOMMAND)
        {
            if(sdkIccDealCardData(pstIccParam, (SDK_ICC_APDU_SEND*)&gstSdkAPDU_S, (SDK_ICC_APDU_RESP*)&gstSdkAPDU_R) == SDK_ICC_NOCARD)
            {
                return SDK_ICC_NOCARD;
            }

            if(gstSdkAPDU_R.ReadCardDataOk == COMMANDERR)
            {
                retCode = ERR_EMV_IccCommand;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }

    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��02�ļ�
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardReadEF02(const SDK_ICC_PARAM *pstIccParam)
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if(NULL == pstIccParam)
    {
        return SDK_PARA_ERR;
    }
    InitLocalVarible(&gstSdkAppUnion);

    while(1)
    {
        retCode = sdkUpcardReadEF02Detail(&gstSdkAppUnion);

        if(retCode == EMV_NEEDSENDCOMMAND)
        {
            if(sdkIccDealCardData(pstIccParam, (SDK_ICC_APDU_SEND*)&gstSdkAPDU_S, (SDK_ICC_APDU_RESP*)&gstSdkAPDU_R) == SDK_ICC_NOCARD)
            {
                return SDK_ICC_NOCARD;
            }

            if(gstSdkAPDU_R.ReadCardDataOk == COMMANDERR)
            {
                retCode = ERR_EMV_IccCommand;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }

    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��02�ļ�
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardReadEF03(const SDK_ICC_PARAM *pstIccParam)
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if(NULL == pstIccParam)
    {
        return SDK_PARA_ERR;
    }
    InitLocalVarible(&gstSdkAppUnion);

    while(1)
    {
        retCode = sdkUpcardReadEF03Detail(&gstSdkAppUnion);

        if(retCode == EMV_NEEDSENDCOMMAND)
        {
            if(sdkIccDealCardData(pstIccParam, (SDK_ICC_APDU_SEND*)&gstSdkAPDU_S, (SDK_ICC_APDU_RESP*)&gstSdkAPDU_R) == SDK_ICC_NOCARD)
            {
                return SDK_ICC_NOCARD;
            }

            if(gstSdkAPDU_R.ReadCardDataOk == COMMANDERR)
            {
                retCode = ERR_EMV_IccCommand;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }

    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��ȡBC��Ϣ
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardGetBCInfo(const SDK_ICC_PARAM *pstIccParam)
{
    u8 retCode = EMV_ERR;
    s32 rlt = SDK_ERR;

    if(NULL == pstIccParam)
    {
        return SDK_PARA_ERR;
    }
    InitLocalVarible(&gstSdkAppUnion);

    while(1)
    {
        retCode = sdkUpcardGetBCInfoDetail(&gstSdkAppUnion);

        if(retCode == EMV_NEEDSENDCOMMAND)
        {
            if(sdkIccDealCardData(pstIccParam, (SDK_ICC_APDU_SEND*)&gstSdkAPDU_S, (SDK_ICC_APDU_RESP*)&gstSdkAPDU_R) == SDK_ICC_NOCARD)
            {
                return SDK_ICC_NOCARD;
            }

            if(gstSdkAPDU_R.ReadCardDataOk == COMMANDERR)
            {
                retCode = ERR_EMV_IccCommand;
                break;
            }
            else
            {
                continue;
            }
        }
        else
        {
            break;
        }
    }

    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    �ֻ�֧����������
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_PARA_ERR
                    SDK_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
                    SDK_IccDataFormatErr
                    SDK_CardBlockErr
                    SDK_IccReturnErr
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardTransFlow(const SDK_ICC_PARAM *pstIccParam)
{
    u8 out[1024] = {0};
    s32 len = 0;
    s32 rlt = SDK_ERR;

    rlt = sdkUpCardTransInit();
    Trace("sdkUpCardTransInit", "rlt = %d\r\n", rlt);

    if(rlt != SDK_OK)
    {
        return rlt;
    }
    rlt = sdkIccResetIcc(pstIccParam, out, &len);
    Trace("sdkIccResetIcc", "rlt = %d\r\n", rlt);


    if(rlt != SDK_OK)
    {
        return rlt;
    }
    rlt = sdkUpcardSelectApp(pstIccParam);
    Trace("sdkUpcardSelectApp", "rlt = %d\r\n", rlt);

    if(rlt != SDK_OK)
    {
        return rlt;
    }
    rlt = sdkUpcardReadEF02(pstIccParam);
    Trace("sdkUpcardReadEF02", "rlt = %d\r\n", rlt);

    if(rlt != SDK_OK)
    {
        return rlt;
    }
    rlt = sdkUpcardReadEF03(pstIccParam);
    Trace("sdkUpcardReadEF03", "rlt = %d\r\n", rlt);

    if(rlt != SDK_OK)
    {
        return rlt;
    }
    rlt = sdkUpcardGetBCInfo(pstIccParam);
    Trace("sdkUpcardGetBCInfo", "rlt = %d\r\n", rlt);

    return rlt;
}

/*****************************************************************************
** Descriptions:    ��ȡ�ֻ�����TLV(tagֻ��"\xDF\x32"/"\xDF\x33"/"\xDF\x34"��Ч)
** Parameters:      const u8 *pheTag
                    u8 *pheOutData
                    s32 *psiOutLen
** Returned value:  SDK_OK
                    SDK_PARA_ERR
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen)
{
    if((NULL == pheTag) || (NULL == pheOutData) || (NULL == psiOutLen))
    {
        return SDK_PARA_ERR;
    }
    TraceHex("sdkUpcardReadTLV", "pheTag", pheTag, 2);

    //ֻ�����DF32 DF33 DF34 ��������
    if(pheTag[0] != 0xDF)
    {
        return SDK_PARA_ERR;
    }
    else if(pheTag[1] != 0x32 && pheTag[1] != 0x33 && pheTag[1] != 0x34)
    {
        return SDK_PARA_ERR;
    }
    else
    {
        if(pheTag[1] == 0x32)
        {
            *psiOutLen = 10;
            memcpy(pheOutData, gstSdkUpcardParam.MobileCardSeq, 10);
        }
        else if(pheTag[1] == 0x33)
        {
            *psiOutLen = 8;
            memcpy(pheOutData, gstSdkUpcardParam.MobileProcessKey, 8);
        }
        else if(pheTag[1] == 0x34)
        {
            *psiOutLen = 7;
            memcpy(pheOutData, gstSdkUpcardParam.MobileGetTrackTime, 7);
        }
        else
        {
            return SDK_PARA_ERR;
        }
    }
    TraceHex("sdkUpcardReadTLV", "outdata", pheOutData, *psiOutLen);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ��ȡ�ֻ�������
** Parameters:      SDK_ICC_CARDDATA *pstCardData
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
** Created By:      sujianzhong  2012.11.30
** Remarks:
*****************************************************************************/
s32 sdkUpcardGetMagData(SDK_ICC_CARDDATA *pstCardData)     //sujianzhong 2013.01.10 14:33
{
    u8 temp[128];
    u16 i, j;
    SDK_ICC_CARDDATA carddata;

    if(NULL == pstCardData)
    {
        return SDK_PARA_ERR;
    }
    memset(&carddata, 0, sizeof(SDK_ICC_CARDDATA));
    memset(temp, 0, sizeof(temp));                             //�建��
    i = 10;                                                                     //�޳�
    sdkBcdToAsc(temp, gstSdkUpcardParam.MobileCardNo, i);

    for( j = 0; j < 2 * i; j++)     //xule
    {
        if(temp[j] == 'F' || temp[j] == 'f')
        {
            temp[j] = 0;
            break;
        }
    }

    i = strlen(temp);     //ȡ����

    if(i == 0)
    {
        return SDK_ERR;                                                                             //���ػ�ȡ����
    }

    if(i > 19)
    {
        i = 19;                                                                                 //���Ƴ���
    }
    carddata.ucCardLen = i;
//    sdkBcdToAsc(carddata.asCardNO, temp, (i + 1) / 2);
    memcpy(carddata.asCardNO, temp, i);
    carddata.asCardNO[i] = 0;


    sdkBcdToU16(&i, gstSdkUpcardParam.Mobile2Equ, 1);
    TraceHex("", "gstSdkUpcardParam.Mobile2Equ", gstSdkUpcardParam.Mobile2Equ, 20);
    Trace("sdkUpcardGetMagData", "mobile2Equ len = %d\r\n", i);

    if(i > 0)
    {
        if(i > 37)
        {
            i = 37;
        }
        carddata.ucTrack2Len = i;
        sdkBcdToAsc(carddata.asTrack2Data, &gstSdkUpcardParam.Mobile2Equ[1], (i + 1) / 2);
        carddata.asTrack2Data[i] = 0;
    }
    sdkBcdToU16(&i, gstSdkUpcardParam.Mobile3Equ, 2);
    TraceHex("", "gstSdkUpcardParam.Mobile3Equ", gstSdkUpcardParam.Mobile3Equ, 54);

    Trace("sdkUpcardGetMagData", "mobile3Equ len = %d\r\n", i);

    if(i > 0)
    {
        if(i > 104)
        {
            i = 104;
        }
        carddata.ucTrack3Len = i;
        sdkBcdToAsc(carddata.asTrack3Data, &gstSdkUpcardParam.Mobile3Equ[2], (i + 1) / 2);
        carddata.asTrack3Data[i] = 0;
    }

    if(strlen(gstSdkUpcardParam.MobileExpiredData))
    {
        sdkBcdToAsc(carddata.asExpiredDate, gstSdkUpcardParam.MobileExpiredData, 2);
        carddata.asExpiredDate[4] = 0;
    }
    memcpy(pstCardData, &carddata, sizeof(SDK_ICC_CARDDATA));
    return SDK_OK;
}

//==================�ֻ�֧��end==================


/*=======BEGIN: sujianzhong 2013.04.10  14:24 modify Ѱupcash��upcard===========*/
#if 0
s32 Private_sdkIccQueryCLCardType(SDK_ICC_TRADE_PARAM  *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam, u8 *pheUID)
{
    u8 buf[1024] = {0};
    u8 bcAmount[6] = {0}; //sujianzhong 2013.01.10 9:16
    s32 len = 0;
    s32 readcardrst = SDK_ICC_NOCARD;
    s32 rslt = 0;
    SDK_FRAME_DATA FrameData;

    memset(buf, 0, sizeof(buf));

    if(pstIccParam == NULL)
    {
        return SDK_PARA_ERR;
    }

    if(pstIccTradeParam != NULL && pstIccTradeParam->pasTradeAmount != NULL) //sujianzhong 2013.01.10 9:16
    {
        Verify(sdkAscToBcdR(bcAmount, pstIccTradeParam->pasTradeAmount, 6) >= 0);
    }

    if(!bIsSdkIccSendCLPoweron)
    {
        sdkIccInitReadCard();
        bIsSdkIccSendCLPoweron = 1;

        if((pstIccTradeParam != NULL) &&
           ((pstIccTradeParam->bIsSupportQPBOC) && (pstIccTradeParam->bIsSupportCLPBOC == false)
            && (memcmp(bcAmount, "\x00\x00\x00\x00\x00\x00", 6)) //sujianzhong 2013.01.10 9:16
            && (pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW) && (sdkIccJudgeEnableQReaderTrade())))
        {
            Trace("emv", "go->use qreader\r\n");

            if(sdkIccSendQReaderSale(bcAmount) != SDK_OK) //sujianzhong 2013.01.10 14:22
            {
                return SDK_ERR;
            }
        }
        else
        {
            if(sdkSysIsRfIn() == false) //���÷ǽ�
            {
                Trace("emv", "��������Ѱ��ָ��\r\n");
            }
            else //���÷ǽ�
            {
                Trace("emv", "��������Ѱ��ָ��\r\n");
                sdk_dev_cl_query_01_send(0x03);
            }
        }
    }
    rslt = sdkIccQueryCardRecv(&FrameData);

    if(rslt > 0)
    {
        TraceHex("emv", "Ѱ����! receive data", buf, len);

        if(rbuf[0] == 0x01) //01Э���ʶ
        {
            if((rbuf[4] == 0x46) && (rbuf[5] == 0))
            {
                pstIccParam->ucCardMode = SDK_ICC_RF;
                pstIccParam->ucSlotNo = rbuf[6]; //13.56M����CID�̶�Ϊ0

                if(pheUID != NULL)
                {
                    if(rbuf[1] == 0x15) //��չ֡�����ֽڳ���
                    {
                        len = rbuf[3] * 256 + rbuf[4];
                        memcpy(pheUID, &rbuf[5], len);
                    }
                    else
                    {
                        len = rbuf[3];
                        memcpy(pheUID, &rbuf[4], len);
                    }
                    Trace("emv", "\r\readcardrst = %d \r\n", len);
                }
                else if((rbuf[4] == 0x46) && (rbuf[5] == 2)) //���ſ����
                {
                    readcardrst = SDK_ICC_MORECARD;
                }
            }
            else if(rbuf[0] == 0x02)
            {
                memset(&gstSdkIccQreaderDataRec, 0, sizeof(gstSdkIccQreaderDataRec));
                len = rbuf[7] * 256 + rbuf[8];

                if(len < QREADER_DATA_MAX)
                {
                    gstSdkIccQreaderDataRec.Sn = rbuf[1];
                    memcpy(gstSdkIccQreaderDataRec.Rfu, &rbuf[2], 4);
                    gstSdkIccQreaderDataRec.Cmd = rbuf[6];
                    gstSdkIccQreaderDataRec.Len = rbuf[7] * 256 + rbuf[8];
                    memcpy(gstSdkIccQreaderDataRec.Data, &rbuf[9], gstSdkIccQreaderDataRec.Len);

                    if(gstSdkIccQreaderDataRec.Sn > 0)
                    {
                        sdkIccIncReaderCurrentSn();
                    }

                    /*=======BEGIN: sujianzhong 2013.01.21  19:59 modify===========*/
                    if(gstSdkIccQreaderDataRec.Data[0] == RC_CARDNO)
                    {
                        if((NULL != pstIccTradeParam->VerifyCardNo)
                           && (SDK_OK == pstIccTradeParam->VerifyCardNo(&gstSdkIccQreaderDataRec.Data[1])))
                        {
                            sdkIccSendQReaderBlacklist(0xff);
                            return SDK_ERR;
                        }
                        else
                        {
                            sdkIccSendQReaderBlacklist(0);
                        }
                    }
                    /*====================== END======================== */
                    /*=======BEGIN: sujianzhong 2013.03.28  11:33 modify===========*/
                    else if(gstSdkIccQreaderDataRec.Data[0] == RC_CARDTYPE)
                    {
                        gucSdkIccQreaderStep = QREADER_STEP_SEL_PPSE;
                        pstIccParam->ucCardMode = SDK_ICC_RF;
                        readcardrst = SDK_OK;

                        if(gstSdkIccQreaderDataRec.Data[1] == QREADER_UPCASH) //upcash
                        {
                            pstIccParam->eCardType = SDK_ICC_CPU;
                        }
                        else if(gstSdkIccQreaderDataRec.Data[1] == QREADER_UPCASH) //upcard
                        {
                            pstIccParam->eCardType = SDK_ICC_MOBILE;
                        }
                        else
                        {
                            readcardrst = SDK_ERR;
                        }
                    }
                    /*====================== END======================== */
                    else if(gstSdkIccQreaderDataRec.Data[0] != RC_MORE_CARDS && gstSdkIccQreaderDataRec.Data[0] != RC_NO_CARD)
                    {
                        gucSdkIccQreaderStep = QREADER_STEP_COMPLETE;
                        pstIccParam->ucCLType = 0x02; //�ڶ�������������qpboc��
                        pstIccParam->ucCardMode = SDK_ICC_RF;
                        //                    readcardrst = sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
                        readcardrst = SDK_OK;
                    }
                    else // ���ſ�
                    {
                        readcardrst = SDK_ICC_MORECARD;
                    }
                }
            }
        }
    }
    else
    {
        readcardrst = SDK_ICC_NOCARD;
    }

    if(SDK_ICC_NOCARD != readcardrst)
    {
        sdkIccInitReadCard();
    }

    if(SDK_OK == readcardrst && 0 == gucSdkIccQreaderStep)
    {
        readcardrst = sdkIccResetIcc(pstIccParam, buf, &len);       //IC����λ
        Trace("emv", "sdkIccResetIcc==%d\r\n", readcardrst);

        if(SDK_OK != readcardrst)
        {
            if(pstIccTradeParam->bIsFallback)
            {
                return SDK_NeedMagCardErr;
            }
            else
            {
                return SDK_ICC_RESET_ERR;
            }
        }

        if(SDK_OK != sdkIccTransInit(pstIccTradeParam))                 //��ʼ������                          //jiguang 20120519
        {
            return SDK_ERR;
        }
        readcardrst = sdkIccGetAppCandidate(pstIccTradeParam, pstIccParam); //ѡ��upcash��֧��Ŀ¼
        Trace("emv", "sdkIccGetAppCandidate==%d\r\n", readcardrst);

        if(SDK_OK == readcardrst)
        {
            pstIccParam->eCardType = SDK_ICC_CPU; //��upcash
        }
        else
        {
            readcardrst = sdkUpCardTransInit();
            Trace("sdkUpCardTransInit", "readcardrst = %d\r\n", readcardrst);

            if(SDK_OK != readcardrst)
            {
                return SDK_ERR;
            }
            readcardrst = sdkUpcardSelectApp(pstIccParam); //ѡ��upcard��֧��Ŀ¼
            Trace("sdkUpcardSelectApp", "ret = %d\r\n", readcardrst);

            if(SDK_OK == readcardrst)
            {
                pstIccParam->eCardType = SDK_ICC_MOBILE; //��upcard
            }
            else
            {
                Trace("emv", "�Ȳ���upcash��Ҳ����upcard\r\n");
                readcardrst = SDK_ERR; //�Ȳ���upcash��Ҳ����upcard
            }
        }
    }
    return readcardrst;
}

s32 sdkIccDealQSaleQpboc(SDK_ICC_TRADE_PARAM * pstIccTradeParam, SDK_ICC_PARAM * pstIccParam)
{
    s32 ret = SDK_ERR, len = 0, timerID = 0;
//    FIFO fifo = {0};
    SDK_ICC_FIXED_AID FixedAid;
    u8 temp[1024] = {0};

    Trace("emv", "gucSdkIccQreaderStep = %d\r\n", gucSdkIccQreaderStep);

    if(QREADER_STEP_SEL_PPSE == gucSdkIccQreaderStep)
    {     //02Э�飬G101ѡ����ppse�󷵻ؿ�����
        sdkIccFillSendReaderData(gstSdkIccQreaderCurrentStatus.Sn, QREADER_CMD_QPBOC, "\x01", 1);     //"\x01"��ʾ֪ͨG101ѡ��upcash����
        sdkIccSendDataToReader();
        timerID = sdkTimerGetId();

        while(1)
        {
//            if(sdk_dev_qread_l2_return(temp) > 0)
            {
                memset(&gstSdkIccQreaderDataRec, 0, sizeof(gstSdkIccQreaderDataRec));
                len = temp[6] * 256 + temp[7];     // ��Э�������ݳ���

                if(len < QREADER_DATA_MAX)
                {
                    gstSdkIccQreaderDataRec.Sn = temp[0];
                    memcpy(gstSdkIccQreaderDataRec.Rfu, &temp[1], 4);
                    gstSdkIccQreaderDataRec.Cmd = temp[5];
                    gstSdkIccQreaderDataRec.Len = temp[6] * 256 + temp[7];
                    memcpy(gstSdkIccQreaderDataRec.Data, &temp[8], gstSdkIccQreaderDataRec.Len);

                    if(gstSdkIccQreaderDataRec.Sn > 0)
                    {
                        sdkIccIncReaderCurrentSn();
                    }

                    /*=======BEGIN: sujianzhong 2013.01.21  19:59 modify===========*/
                    if(gstSdkIccQreaderDataRec.Data[0] == RC_CARDNO)
                    {
                        if((NULL != pstIccTradeParam->VerifyCardNo)
                           && (SDK_OK == pstIccTradeParam->VerifyCardNo(&gstSdkIccQreaderDataRec.Data[1])))
                        {
                            sdkIccSendQReaderBlacklist(0xff);
                            return SDK_ERR;
                        }
                        else
                        {
                            sdkIccSendQReaderBlacklist(0);
                        }
                    }
                    /*====================== END======================== */
                    else if(gstSdkIccQreaderDataRec.Data[0] != RC_MORE_CARDS && gstSdkIccQreaderDataRec.Data[0] != RC_NO_CARD)
                    {
                        gucSdkIccQreaderStep = QREADER_STEP_COMPLETE;
                        pstIccParam->ucCLType = 0x02;     //�ڶ�������������qpboc��
                        pstIccParam->ucCardMode = SDK_ICC_RF;
                        return sdkIccDealQReaderTrade(pstIccTradeParam);     //���������״���
                    }
                    else     // ���ſ�
                    {
                        return SDK_ICC_MORECARD;
                    }
                }
            }
//            ret = sdkIccDealReadCard(&fifo);
//
//            if(ret == SDK_OK && gstSdkIccReadCard.bIsCLData == 1)
//            {
//                if(fifo.Cmd == FIFO_G101PLV2OK) //02Э��
//                {
//                    TraceHex("emv", "sdkIccReadCard G101  data", fifo.Data, fifo.Len);
//
//                    memset(&gstSdkIccQreaderDataRec, 0, sizeof(gstSdkIccQreaderDataRec));
//                    len = fifo.Data[6] * 256 + fifo.Data[7]; // ��Э�������ݳ���
//
//                    if(len < QREADER_DATA_MAX)
//                    {
//                        gstSdkIccQreaderDataRec.Sn = fifo.Data[0];
//                        memcpy(gstSdkIccQreaderDataRec.Rfu, &fifo.Data[1], 4);
//                        gstSdkIccQreaderDataRec.Cmd = fifo.Data[5];
//                        gstSdkIccQreaderDataRec.Len = fifo.Data[6] * 256 + fifo.Data[7];
//                        memcpy(gstSdkIccQreaderDataRec.Data, &fifo.Data[8], gstSdkIccQreaderDataRec.Len);
//
//                        if(gstSdkIccQreaderDataRec.Sn > 0)
//                        {
//                            sdkIccIncReaderCurrentSn();
//                        }
//
//                        /*=======BEGIN: sujianzhong 2013.01.21  19:59 modify===========*/
//                        if(gstSdkIccQreaderDataRec.Data[0] == RC_CARDNO)
//                        {
//                            if((NULL != pstIccTradeParam->VerifyCardNo)
//                               && (SDK_OK == pstIccTradeParam->VerifyCardNo(&gstSdkIccQreaderDataRec.Data[1])))
//                            {
//                                sdkIccSendQReaderBlacklist(0xff);
//                                return SDK_ERR;
//                            }
//                            else
//                            {
//                                sdkIccSendQReaderBlacklist(0);
//                            }
//                        }
//                        /*====================== END======================== */
//                        else if(gstSdkIccQreaderDataRec.Data[0] != RC_MORE_CARDS && gstSdkIccQreaderDataRec.Data[0] != RC_NO_CARD)
//                        {
//                            gucSdkIccQreaderStep = QREADER_STEP_COMPLETE;
//                            pstIccParam->ucCLType = 0x02; //�ڶ�������������qpboc��
//                            pstIccParam->ucCardMode = SDK_ICC_RF;
//                            return sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
//                        }
//                        else // ���ſ�
//                        {
//                            return SDK_ICC_MORECARD;
//                        }
//                    }
//                }
//                else
//                {
//                    return SDK_ERR;
//                }
//            }

            if(sdkTimerIsEnd(timerID, SDK_ICC_TIMER_AUTO))
            {
                return SDK_TIME_OUT;
            }
        }
    }
    else if(QREADER_STEP_COMPLETE == gucSdkIccQreaderStep)
    { //02Э�飬G101ֱ������˽���
        return sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
    }
    else if(0 == gucSdkIccQreaderStep)
    { //01Э��
        memset(&FixedAid, 0, sizeof(SDK_ICC_FIXED_AID));

        while(1)
        {
            ret = sdkIccGoToSelectApp(FixedAid, pstIccTradeParam, pstIccParam);      //ȥѡ��Ӧ��

            if(SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
            {
                if(pstIccTradeParam->bIsFallback)
                {
                    return SDK_NeedMagCardErr;
                }
            }
            else if(ret == SDK_OK)
            {
                Trace("emv", "pstIccTradeParam->eFlowMode = %d\r\n", pstIccTradeParam->eFlowMode);
                ret = sdkIccGoToInitialApp(pstIccTradeParam, pstIccParam);      //ȥ��ʼ��Ӧ��
            }

            if(ret == SDK_AppSelectTryAgainErr)
            {
                if(NULL != pstIccTradeParam->DispReselectApp)
                {
                    ret = (*pstIccTradeParam->DispReselectApp)();                                              // ��ʾ"����ѡ��"

                    if(ret != SDK_OK)
                    {
                        return ret;
                    }
                }
                ret = sdkIccReSelectApp(); //����ѡ��Ӧ��

                if(SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
                {
                    if(pstIccTradeParam->bIsFallback)
                    {
                        return SDK_NeedMagCardErr;
                    }
                }

                if(ret != SDK_OK)
                {
                    return ret;
                }
                continue;
            }
            //qPboc����
            else if(SDK_OnlineWait == ret)                                               // qpboc����(�Ǵ������ֽ��˻�)
            {
                return sdkIccDealQpbocOnline(pstIccTradeParam, pstIccParam);
            }
            //qPboc�ѻ���׼
            else if(SDK_OfflineApprove == ret)                                               //qPboc�ѻ���׼
            {
                return sdkIccDealQpbocOffline(pstIccTradeParam, pstIccParam);        //jiguang 20120602
            }
            else if(ret != SDK_OK)
            {
                return ret;
            }
            return SDK_ERR;
        }
    }
    else
    {
        return SDK_ERR;
    }
}

s32 sdkIccDealReaderUpcardRec()
{
    u8 buf[1024] = {0};
    s32 rlt = SDK_ERR, len = 0, i = 0;

    Trace("emv", "gstSdkIccQreaderDataRec.Cmd=%x\r\n", gstSdkIccQreaderDataRec.Cmd);
    Trace("emv", "***gstSdkIccQreaderDataRec.Len=%d\r\n", gstSdkIccQreaderDataRec.Len);

    if(!gucSdkIccEmvInQreader)
    {
        return rlt;
    }

    if(gstSdkIccQreaderDataSend.Cmd != gstSdkIccQreaderDataRec.Cmd)
    {
        return rlt;
    }

    if(gstSdkIccQreaderDataRec.Cmd != QREADER_CMD_MOBILEPAY)
    {
        return rlt;
    }

    if((gstSdkIccQreaderDataRec.Data[0] == RC_QPBOC_ERRINFO) && (gstSdkIccQreaderDataRec.Len > 1))                  //���س�����Ϣ  zjz20111108
    {
        return rlt;
    }

    if(gstSdkIccQreaderDataRec.Len > 1)
    {
        len = gstSdkIccQreaderDataRec.Len - 1;
        memcpy(buf, &gstSdkIccQreaderDataRec.Data[1], len);
        TraceHex("emv", "into sdkIccDealReaderQPBOCSuccess buf", buf, len);
        TraceHex("emv", "into sdkIccDealReaderQPBOCSuccess Msission", gstSdkIccQreaderCurrentStatus.Msission, 16);

        if(gucSdkIccQreaderType == QREADER_TYPE_OUT)       //���ö�������ȥ���� zjz20111108
        {
            for(i = 0; i < len; i += 8)
            {
                sdkDes3S(0, &buf[i], gstSdkIccQreaderCurrentStatus.Msission);
            }
        }
        TraceHex("emv", "After Decrypt buf", buf, len);
        memset(&gstSdkUpcardParam, 0, sizeof(gstSdkUpcardParam));
        i = 2;
        gstSdkUpcardParam.MobileVerLen = buf[i++];

        if(gstSdkUpcardParam.MobileVerLen)
        {
            gstSdkUpcardParam.MobileVer = buf[i++];
            gstSdkUpcardParam.MobileVerbexit = 1;
        }
        gstSdkUpcardParam.BCInfoFileEF04Len = buf[i++];

        if(gstSdkUpcardParam.BCInfoFileEF04Len)
        {
            gstSdkUpcardParam.BCInfoFileEF04 = buf[i++];
            gstSdkUpcardParam.MobileVerbexit = 1;
        }
        memcpy(gstSdkUpcardParam.MobileCardSeq, buf + i, 10);
        i += 10;
        memcpy(gstSdkUpcardParam.MobileCardInfoRecord, buf + i, 108);
        i += 108;
        memcpy(gstSdkUpcardParam.MobileCardName, buf + i, 20);
        i += 20;
        memcpy(gstSdkUpcardParam.MobileCardNo, buf + i, 10);
        i += 10;
        memcpy(gstSdkUpcardParam.Mobile2Equ, buf + i, 20);
        i += 20;
        memcpy(gstSdkUpcardParam.Mobile3Equ, buf + i, 54);
        i += 54;
        memcpy(gstSdkUpcardParam.MobileExpiredData, buf + i, 2);
        i += 2;
        memcpy(gstSdkUpcardParam.MobileProcessKey, buf + i, 8);
        i += 8;
        memcpy(gstSdkUpcardParam.MobileGetTrackTime, buf + i, 7);
        i += 7;

        if(i != (buf[0] * 256 + buf[1] + 2))
        {
            return rlt;
        }
    }
    return SDK_OK;
}

s32 sdkIccDealQSaleUpcard(SDK_ICC_PARAM *pstIccParam)
{
    s32 len = 0, timerID = 0;
    s32 rlt = SDK_ERR;
    u8 temp[1024] = {0};

//    FIFO fifo = {0};

    if(QREADER_STEP_SEL_PPSE == gucSdkIccQreaderStep)
    {     //02Э�飬G101ѡ����ppse�󷵻ؿ�����
        sdkIccFillSendReaderData(gstSdkIccQreaderCurrentStatus.Sn, QREADER_CMD_MOBILEPAY, "\x02", 1);     //"\x02"��ʾ֪ͨG101ѡ��upcard����
        sdkIccSendDataToReader();
        timerID = sdkTimerGetId();

        while(1)
        {
//            if(sdk_dev_qread_l2_return(temp) > 0)
            {
                memset(&gstSdkIccQreaderDataRec, 0, sizeof(gstSdkIccQreaderDataRec));
                len = temp[6] * 256 + temp[7];     // ��Э�������ݳ���

                if(len < QREADER_DATA_MAX)
                {
                    gstSdkIccQreaderDataRec.Sn = temp[0];
                    memcpy(gstSdkIccQreaderDataRec.Rfu, &temp[1], 4);
                    gstSdkIccQreaderDataRec.Cmd = temp[5];
                    gstSdkIccQreaderDataRec.Len = temp[6] * 256 + temp[7];
                    memcpy(gstSdkIccQreaderDataRec.Data, &temp[8], gstSdkIccQreaderDataRec.Len);

                    if(gstSdkIccQreaderDataRec.Sn > 0)
                    {
                        sdkIccIncReaderCurrentSn();
                    }

                    if(gstSdkIccQreaderDataRec.Data[0] != RC_MORE_CARDS && gstSdkIccQreaderDataRec.Data[0] != RC_NO_CARD)
                    {
                        gucSdkIccQreaderStep = QREADER_STEP_COMPLETE;

                        return sdkIccDealReaderUpcardRec();
                    }
                    else         // ���ſ�
                    {
                        return SDK_ICC_MORECARD;
                    }
                }
            }

            if(sdkTimerIsEnd(timerID, SDK_ICC_TIMER_AUTO))
            {
                return SDK_TIME_OUT;
            }
        }
    }
    else if(QREADER_STEP_COMPLETE == gucSdkIccQreaderStep)
    {
        return SDK_ERR;
    }
    else
    { //01Э��
        rlt = sdkUpcardSelectApp(pstIccParam);
        Trace("sdkUpcardSelectApp", "rlt = %d\r\n", rlt);

        if(rlt != SDK_OK)
        {
            return rlt;
        }
        rlt = sdkUpcardReadEF02(pstIccParam);
        Trace("sdkUpcardReadEF02", "rlt = %d\r\n", rlt);

        if(rlt != SDK_OK)
        {
            return rlt;
        }
        rlt = sdkUpcardReadEF03(pstIccParam);
        Trace("sdkUpcardReadEF03", "rlt = %d\r\n", rlt);

        if(rlt != SDK_OK)
        {
            return rlt;
        }
        rlt = sdkUpcardGetBCInfo(pstIccParam);
        Trace("sdkUpcardGetBCInfo", "rlt = %d\r\n", rlt);

        return rlt;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:     SDK_ICC_TRADE_PARAM * pstIccTradeParam
                   SDK_ICC_PARAM * pstIccParam
** Returned value: SDK_OK:����upcard�������ɹ�������upcash�������쳣
                    SDK_ERR ����ʧ��
                    SDK_IccDataFormatErr ���ݸ�ʽ��
                    SDK_IccCommandErr ����ʧ��
                    SDK_IccReturnErr ������ʧ��
                    SDK_NoAppSelErr û��Ӧ��
                    SDK_IccDataRedundErr:�������ݴ�
                    SDK_CardBlockErr ��������
                    SDK_AppTimeOutErr:��ʱ
                    SDK_NeedMagCardErr ʹ��fallback����
                    SDK_CDAFormatErr :CDA��ʽ��
                    SDK_OnlineWait:�����ȴ�(�ȴ�����)
                    SDK_TransOfflineApprove �ѻ���׼(�ѻ��ɹ�)
                    SDK_TransOfflineDeclineErr �ѻ��ܾ�(��������)
                    SDK_TransNotAcceptErr:���ײ�����
                    SDK_TransTerminateErr ��ֹ����
                    SDK_UsePbocErr:��ʹ��PBOC
                    SDK_PARA_ERR:��������
                    SDK_EcLackBalanceErr:�����ֽ�����
                    SDK_QpbocAuthNoPerformed:������δ����˫����֤
** Created By:     sujianzhong20130401
** Remarks:
*****************************************************************************/
s32 Private_sdkIccDealQSale(SDK_ICC_TRADE_PARAM * pstIccTradeParam, SDK_ICC_PARAM * pstIccParam)
{
    s32 ret = SDK_ERR;

    if((pstIccTradeParam == NULL) || (pstIccParam == NULL) || (SDK_ICC_RF != pstIccParam->ucCardMode))
    {
        return SDK_PARA_ERR;
    }

    if(SDK_ICC_CPU == pstIccParam->eCardType)
    {
        return sdkIccDealQSaleQpboc(pstIccTradeParam, pstIccParam);
    }
    else if(SDK_ICC_MOBILE == pstIccParam->eCardType)
    {
        ret = sdkUpcardReadEF02(pstIccParam);
        Trace("sdkUpcardReadEF02", "ret = %d\r\n", ret);

        if(ret != SDK_OK)
        {
            return ret;
        }
        ret = sdkUpcardReadEF03(pstIccParam);
        Trace("sdkUpcardReadEF03", "ret = %d\r\n", ret);

        if(ret != SDK_OK)
        {
            return ret;
        }
        ret = sdkUpcardGetBCInfo(pstIccParam);
        Trace("sdkUpcardGetBCInfo", "ret = %d\r\n", ret);

        return ret;
    }
    else
    {
        return SDK_ERR;
    }
}

#endif
/*====================== END======================== */

#endif
static MOBILECARDDATA *gMoblieData = NULL;


//20140320
void Emvb_MobileTransInit(void) {
    u8 transTime[9];


    if (gMoblieData == NULL) {
        gMoblieData = (MOBILECARDDATA *) emvb_malloc(sizeof(MOBILECARDDATA));
    }
    memset((unsigned char *) gMoblieData, 0, sizeof(MOBILECARDDATA));

    gMoblieData->EMVB_qPBOCIccIsoCommand = IccIsoCommand;


    memset(transTime, 0, sizeof(transTime));

    Verify(sdkGetRtc(&transTime[1]) == SDK_OK);     //sjz

    if (transTime[1] >= 0x49) {
        transTime[0] = 0x19;
    }
    else {
        transTime[0] = 0x20;
    }
    memcpy(gMoblieData->MobileGetTrackTime, transTime, 7);
    gMoblieData->MobileGetTrackTime[7] = 0x80;
}

//20140320
void Emvb_MobileTransData_free(void) {
    if (gMoblieData != NULL) {
        emvb_free(gMoblieData);
        gMoblieData = NULL;
    }
}

//20140320
unsigned char Emvb_Mobile_ReadDFSelectRetData(unsigned char *DataOut, unsigned short LenOut,
                                              MOBILECARDDATA *MoblieData) {
    unsigned short id, indexFCI, len, lenFCI;
    unsigned short k;

    id = 0;

    if (DataOut[id] != 0x70)          //�ƶ�֧��Ӧ�ñ�ʶ
    {
        return ERR_EMV_IccDataFormat;         //FCI template
    }
    id++;

    if (ParseExtLen(DataOut, &id, &lenFCI)) {
        return ERR_EMV_IccDataFormat;
    }
    indexFCI = id;

    while (id < indexFCI + lenFCI) {
        if (id >= LenOut) { return ERR_EMV_IccDataFormat; }

        if (DataOut[id] == 0xFF || DataOut[id] == 0x00) {
            id++;
            continue;
        }
        else if (DataOut[id] == 0x51)         //DF��
        {
            id++;
            MoblieData->MobileVerLen = DataOut[id];

            if (MoblieData->MobileVerLen != 1) { return ERR_EMV_IccDataFormat; }
            id++;
            MoblieData->MobileVer = DataOut[id];
            id++;
            MoblieData->MobileVerbexit = 1;
        }
        else if (DataOut[id] == 0x52)         //�ƶ�֧�����п���Ϣ�ļ�(EF04)��¼��
        {
            MoblieData->BCInfoFileEF04Len = DataOut[id + 1];

            if (MoblieData->BCInfoFileEF04Len != 1) {
                return ERR_EMV_EmvDataFormat;                                               //with wrong len of SFI,select next app.//SFI�ļ����ȱ���ҪΪ1
            }
            id += 2;
            MoblieData->BCInfoFileEF04 = DataOut[id];
            id++;
            MoblieData->BCInfoFileEF04bexit = 1;
        }
        else {
            k = DataOut[id];

            if ((k & 0x1F) == 0x1F) {
                id++;
            }
            id++;

            if (ParseExtLen(DataOut, &id, &len)) {
                return ERR_EMV_IccDataFormat;
            }
            id += len;
        }
    }

    if (id != indexFCI + lenFCI) {
        return ERR_EMV_IccDataFormat;
    }
    return EMV_OK;
}

//20140320
unsigned char Emvb_Mobile_SelectApp(MOBILECARDDATA *MoblieData) {
    unsigned char DfName[20];
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;


    memset(DfName, 0, sizeof(DfName));
    memcpy(DfName, "\xA0\x00\x00\x03\x33", 5);
    memcpy(&DfName[5], "CUP-MOBILE", 10);

    EMVB_COMMAND_SELECT(DfName, 15, 0, &apdu_s);

    MoblieData->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

    if (apdu_r.ReadCardDataOk != EMV_OK) {
        return ERR_APP_TIMEOUT;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) {
        if (Emvb_Mobile_ReadDFSelectRetData(apdu_r.DataOut, apdu_r.LenOut, MoblieData) != EMV_OK) {
            return ERR_EMV_IccDataFormat;
        }
        return EMV_OK;
    }
    else if (apdu_r.SWA == 0x6A &&
             apdu_r.SWB == 0x81)  //card block or command not supported,terminate session.
    {
        return ERR_BLOCKORFORMATERR;                        //sxl ������Ҫע��
    }
    else {
        return ERR_EMV_IccReturn;
    }
}

/*******************************************************************
   Author:   sxl
   Function Name: unsigned char Mobile_GetBCInfo(AppUnionStruct *tempApp_UnionStruct)
   Function Purpose:read mobile information
   Input Paramters:
   Output Paramters:N/A
   Return Value:       N/A
   Remark: N/A
   Modify: N/A        20140320
 ********************************************************************/
unsigned char Emvb_Mobile_GetBCInfo(MOBILECARDDATA *MoblieData) {
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;

    if (MoblieData->BCInfoFileEF04bexit == 0) {
        return ERR_EMV_IccCommand;
    }
    memcpy(apdu_s.Command, "\x80\xF8\x02\x00", 4); //Get bankCard Info ��ȡĬ���ֻ��˻�
    apdu_s.Lc = 8;

    memcpy(apdu_s.DataIn, MoblieData->MobileGetTrackTime, 8);
    apdu_s.Le = 256;
    apdu_s.EnableCancel = 1;           //ʹ�ܰ�ȡ����

    MoblieData->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

    if (apdu_r.ReadCardDataOk != EMV_OK) {
        return ERR_APP_TIMEOUT;
    }

    if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) //Select successful
    {
        memset(MoblieData->MobileCardInfoRecord, 0, sizeof(MoblieData->MobileCardInfoRecord));
        memset(MoblieData->MobileCardName, 0, sizeof(MoblieData->MobileCardName));
        memset(MoblieData->MobileCardNo, 0, sizeof(MoblieData->MobileCardNo));
        memset(MoblieData->Mobile2Equ, 0, sizeof(MoblieData->Mobile2Equ));
        memset(MoblieData->Mobile3Equ, 0, sizeof(MoblieData->Mobile3Equ));
        memset(MoblieData->MobileExpiredData, 0, sizeof(MoblieData->MobileExpiredData));
        memset(MoblieData->MobileProcessKey, 0, sizeof(MoblieData->MobileProcessKey));

        if (apdu_r.LenOut != 116) { return ERR_EMV_IccCommand; }
        memcpy(MoblieData->MobileCardInfoRecord, apdu_r.DataOut, 108);
        memcpy(MoblieData->MobileCardName, apdu_r.DataOut, 20);
        memcpy(MoblieData->MobileCardNo, &apdu_r.DataOut[20], 10);
        memcpy(MoblieData->Mobile2Equ, &apdu_r.DataOut[30], 20);
        memcpy(MoblieData->Mobile3Equ, &apdu_r.DataOut[50], 54);
        memcpy(MoblieData->MobileExpiredData, &apdu_r.DataOut[104], 2);
        memcpy(MoblieData->MobileProcessKey, &apdu_r.DataOut[108], 8);

        return EMV_OK;
    }
    else {
        return ERR_EMV_IccCommand;
    }
}

/*******************************************************************
   Author:   sxl
   Function Name: unsigned char Mobile_ReadEF02(AppUnionStruct *tempApp_UnionStruct)
   Function Purpose:read EF 02
   Input Paramters:
   Output Paramters:N/A
   Return Value:       N/A
   Remark: N/A
   Modify: N/A         20140320
 ********************************************************************/
unsigned char Emvb_Mobile_ReadEF02(MOBILECARDDATA *MoblieData) {
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;


#if 1   //20140716_lhd

    unsigned char cnt = 1;

    while (1) {
        memset(&apdu_s, 0, sizeof(SDK_ICC_APDU_SEND));
        memset(&apdu_r, 0, sizeof(SDK_ICC_APDU_RESP));

        memcpy(apdu_s.Command, "\x00\xB0\x82\x00", 4);         //Get bankCard Info ��ȡĬ���ֻ��˻�
        apdu_s.Lc = 0;

        if (0 == cnt) {
            apdu_s.Le = 0;
        }
        else {
            apdu_s.Le = 10;
        }
        apdu_s.EnableCancel = 1;                           //ʹ�ܰ�ȡ����

        MoblieData->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

        if (apdu_r.ReadCardDataOk != EMV_OK) {
            return ERR_APP_TIMEOUT;
        }

        if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00)         //Select successful
        {
            memset(MoblieData->MobileCardSeq, 0, sizeof(MoblieData->MobileCardSeq));
            memcpy(MoblieData->MobileCardSeq, apdu_r.DataOut, 10);

            return EMV_OK;
        }
        else {
            if (1 == cnt) {
                cnt = 0;
                continue;
            }
            return ERR_EMV_IccCommand;
        }
    }


#else
    memcpy(apdu_s.Command, "\x00\xB0\x82\x00", 4); //Get bankCard Info ��ȡĬ���ֻ��˻�
    apdu_s.Lc = 0;
    apdu_s.Le = 10;
    apdu_s.EnableCancel = 1;           //ʹ�ܰ�ȡ����

    MoblieData->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

    if(apdu_r.ReadCardDataOk != EMV_OK)
    {
        return ERR_APP_TIMEOUT;
    }

    if(apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00) //Select successful
    {
        memset(MoblieData->MobileCardSeq, 0, sizeof(MoblieData->MobileCardSeq));
        memcpy(MoblieData->MobileCardSeq, apdu_r.DataOut, 10);

        return EMV_OK;
    }
    else
    {
        return ERR_EMV_IccCommand;
    }
#endif
}

/*******************************************************************
   Author:   sxl
   Function Name: unsigned char Mobile_ReadEF02(AppUnionStruct *tempApp_UnionStruct)
   Function Purpose:read EF 02
   Input Paramters:
   Output Paramters:N/A
   Return Value:       N/A
   Remark: N/A
   Modify: N/A         20140320
 ********************************************************************/
unsigned char Emvb_Mobile_ReadEF03(MOBILECARDDATA *MoblieData) {
    SDK_ICC_APDU_SEND apdu_s;
    SDK_ICC_APDU_RESP apdu_r;
    unsigned char cnt = 1;

    while (1) {
        memset(&apdu_s, 0, sizeof(SDK_ICC_APDU_SEND));
        memset(&apdu_r, 0, sizeof(SDK_ICC_APDU_RESP));

        memcpy(apdu_s.Command, "\x00\xB0\x83\x00", 4);         //Get bankCard Info ��ȡĬ���ֻ��˻�
        apdu_s.Lc = 0;

        if (0 == cnt) {
            apdu_s.Le = 0;
        }
        else {
            apdu_s.Le = 8;
        }
        apdu_s.EnableCancel = 1;                           //ʹ�ܰ�ȡ����

        MoblieData->EMVB_qPBOCIccIsoCommand(&apdu_s, &apdu_r);

        if (apdu_r.ReadCardDataOk != EMV_OK) {
            return ERR_APP_TIMEOUT;
        }

        if (apdu_r.SWA == 0x90 && apdu_r.SWB == 0x00)         //Select successful
        {
            //memset(MoblieData->MobileCardSeq, 0, sizeof(MoblieData->MobileCardSeq));
            //memcpy(MoblieData->MobileCardSeq, apdu_r.DataOut, 10);

            return EMV_OK;
        }
        else {
            if (1 == cnt) {
                cnt = 0;
                continue;
            }
            return ERR_EMV_IccCommand;
        }
    }
}

//20140320
s32 Emvb_Mobile_TransFlow(unsigned char *emvtask, unsigned short *nowtask,
                          MOBILECARDDATA *MoblieData) {
    unsigned char retCode;
    s32 rlt;


    while (1) {
        retCode = qPBOC_checkifcancelcontactlesstrade();

        if (retCode == PAYWAVE_DETECTEDOTHERMODE) {
            retCode = ERR_EMV_CancelTrans;
            *emvtask = EMVERRDEAL;
        }
        else if (retCode == qPBOC_RESETCOMMAND) {
            retCode = ERR_EMV_CancelTrans;
            *emvtask = EMVERRDEAL;
        }

        if (*emvtask == qPBOCDETECTCARD) {
            *emvtask = qPBOCSELECTAPP;          //sxl ������Ӧ�ó��������д������ﲻ�ô���
            //retCode = contactless_detectedcard(0);
            //if(retCode == EMV_OK)
            //{
            //	*emvtask = qPBOCSELECTAPP;
            //}
            //else
            //{
            //    *emvtask = EMVERRDEAL;
            //}
        }

        if (*emvtask == qPBOCSELECTAPP) {
            retCode = Emvb_Mobile_SelectApp(MoblieData);
#ifdef EMVB_DEBUG
            printf("qPBOC_GetAppCandidate retCode = %d\r\n", retCode);
#endif

            if (retCode == EMV_OK) {
                *emvtask = EMVCHOOSEAPP;
            }
            else {
                *emvtask = EMVERRDEAL;
            }
        }

        if (*emvtask == EMVCHOOSEAPP) {
            retCode = Emvb_Mobile_GetBCInfo(MoblieData);

#ifdef EMVB_DEBUG
            printf("qPBOC_GetAppCandidate retCode = %d\r\n", retCode);
#endif

            if (retCode == EMV_OK) {
                *emvtask = EMVUSERSELECTAPP;
            }
            else {
                *emvtask = EMVERRDEAL;
            }
        }

        //��ȡEF02
        if (*emvtask == EMVUSERSELECTAPP) {
            retCode = Emvb_Mobile_ReadEF02(MoblieData);

            if (retCode == EMV_OK) {
#if 0                               //sxl?
                transflowb_readcardfinished(0);              //�����ɹ���Ҫ�ƿ���
                sdkIccPowerDown();
#endif
                retCode = EMV_OK;
            }
            else {
                *emvtask = EMVERRDEAL;
            }
        }

        if (*emvtask == EMVERRDEAL)      //���״�����
        {
            sdkIccPowerDown();

            if (gEmvErrID.L3 == EMVB_ERRID_L3_STOP) {
                //retCode = qPBOC_RESETCOMMAND;
                retCode = ERR_EMV_CancelTrans;
                *emvtask = EMVERRDEAL;
            }
            else if (gEmvErrID.L3 == EMVB_ERRID_L3_CARDMAGINSERT) {
                //return PAYWAVE_DETECTEDOTHERMODE;
                retCode = ERR_EMV_CancelTrans;
                *emvtask = EMVERRDEAL;
            }
            else if (gEmvErrID.L1 == EMVB_ERRID_L1_TIMEOUTERR) {
                //Emvb_MobileTransInit();    //sxl? �鿴�����Ƿ���ȷ
                //emvtask = qPBOCDETECTCARD;
                //continue;
                //sxl?Ӧ�ò��Ѽ���IC�������̴��������������½��н���
                retCode = ERR_APP_TIMEOUT;
                *emvtask = EMVERRDEAL;
            }
        }
        else {
            *emvtask = EMVERRDEAL;
            retCode = ERR_EMV_IccCommand;
        }
        //return PostOperateqPBOCErr(retCode);  ////sxl���ﲻ���������Ӧ�ý��д���
        sdkIccEmvToSdkRlt(retCode, &rlt);
        return retCode;
    }
}

//20140320       //sxl?  EMV��ҲŪһ���汾
s32 Emvb_DealMobilePosTransFlow(unsigned short nowtask) {
    s32 retCode;
    unsigned char emvtask;

    Emvb_MobileTransInit();
    emvtask = qPBOCDETECTCARD;

    while (1) {
        retCode = Emvb_Mobile_TransFlow(&emvtask, &nowtask, gMoblieData);

        sdkIccPowerDown();

        if (retCode != SDK_OK && gMoblieData != NULL) {
            emvb_free(gMoblieData);
            gMoblieData = NULL;
        }
        return retCode;
    }
}

s32 sdkUpcardReadTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) {
    if ((NULL == pheTag) || (NULL == pheOutData) || (NULL == psiOutLen) || (gMoblieData == NULL)) {
        return SDK_PARA_ERR;
    }
    TraceHex("sdkUpcardReadTLV", "pheTag", pheTag, 2);

    //ֻ�����DF32 DF33 DF34 ��������
    if (pheTag[0] != 0xDF) {
        return SDK_PARA_ERR;
    }
    else if (pheTag[1] != 0x32 && pheTag[1] != 0x33 && pheTag[1] != 0x34) {
        return SDK_PARA_ERR;
    }
    else {
        if (pheTag[1] == 0x32) {
            *psiOutLen = 10;
            memcpy(pheOutData, gMoblieData->MobileCardSeq, 10);
        }
        else if (pheTag[1] == 0x33) {
            *psiOutLen = 8;
            memcpy(pheOutData, gMoblieData->MobileProcessKey, 8);
        }
        else if (pheTag[1] == 0x34) {
            *psiOutLen = 7;
            memcpy(pheOutData, gMoblieData->MobileGetTrackTime, 7);
        }
        else {
            return SDK_PARA_ERR;
        }
    }
    TraceHex("sdkUpcardReadTLV", "outdata", pheOutData, *psiOutLen);
    return SDK_OK;
}

s32 sdkUpcardGetMagDataProcess(SDK_ICC_CARDDATA *pstCardData) {
    u8 temp[128];
    u16 i, j;
    SDK_ICC_CARDDATA *carddata;


    if (NULL == pstCardData || gMoblieData == NULL) {
        return SDK_PARA_ERR;
    }
    carddata = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));

    memset(carddata, 0, sizeof(SDK_ICC_CARDDATA));
    memset(temp, 0, sizeof(temp));                             //�建��
    i = 10;                                                                     //�޳�
    sdkBcdToAsc(temp, gMoblieData->MobileCardNo, i);

    for (j = 0; j < 2 * i; j++)     //xule
    {
        if (temp[j] == 'F' || temp[j] == 'f') {
            temp[j] = 0;
            break;
        }
    }

    i = strlen(temp);     //ȡ����

    if (i == 0) {
        emvb_free(carddata);
        return SDK_ERR;                                                                             //���ػ�ȡ����
    }

    if (i > 19) {
        i = 19;                                                                                 //���Ƴ���
    }
    carddata->ucCardLen = i;
//    sdkBcdToAsc(carddata.asCardNO, temp, (i + 1) / 2);
    memcpy(carddata->asCardNO, temp, i);
    carddata->asCardNO[i] = 0;


    sdkBcdToU16(&i, gMoblieData->Mobile2Equ, 1);
    TraceHex("", "gstSdkUpcardParam.Mobile2Equ", gMoblieData->Mobile2Equ, 20);
    Trace("sdkUpcardGetMagData", "mobile2Equ len = %d\r\n", i);

    if (i > 0) {
        if (i > 37) {
            i = 37;
        }
        carddata->ucTrack2Len = i;
        sdkBcdToAsc(carddata->asTrack2Data, &gMoblieData->Mobile2Equ[1], (i + 1) / 2);
        carddata->asTrack2Data[i] = 0;
    }
    sdkBcdToU16(&i, gMoblieData->Mobile3Equ, 2);
    TraceHex("", "gstSdkUpcardParam.Mobile3Equ", gMoblieData->Mobile3Equ, 54);

    Trace("sdkUpcardGetMagData", "mobile3Equ len = %d\r\n", i);

    if (i > 0) {
        if (i > 104) {
            i = 104;
        }
        carddata->ucTrack3Len = i;
        sdkBcdToAsc(carddata->asTrack3Data, &gMoblieData->Mobile3Equ[2], (i + 1) / 2);
        carddata->asTrack3Data[i] = 0;
    }

    if (strlen(gMoblieData->MobileExpiredData)) {
        sdkBcdToAsc(carddata->asExpiredDate, gMoblieData->MobileExpiredData, 2);
        carddata->asExpiredDate[4] = 0;
    }
    memcpy(pstCardData, carddata, sizeof(SDK_ICC_CARDDATA));
    emvb_free(carddata);
    return SDK_OK;
}

s32 sdkUpcardGetMagData(SDK_ICC_CARDDATA *pstCardData)     //sujianzhong 2013.01.10 14:33
{
    s32 retCode;

    retCode = sdkUpcardGetMagDataProcess(pstCardData);


    return retCode;
}

s32 Private_sdkIccDealUpCard(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam) {
    memset(&gPayData, 0, sizeof(PAYDATA));
    gPayData.pstIccParam = pstIccParam;


    return Emvb_DealMobilePosTransFlow(0);
}

/*******************************************************************
   Author:   sxl
   Function Name: void mobile_datazipbag(u8 *mobiledata,u32 *mobiledatalen)
   Function Purpose:mobile data zip bag
   Input Paramters:
   Output Paramters:N/A
   Return Value:       N/A
   Remark: N/A
   Modify: N/A         20140320
 ********************************************************************/
void Emvb_mobile_datazipbag(unsigned char *mobiledata, unsigned int *mobiledatalen) {
    unsigned int len = 0;

    if (gMoblieData == NULL) {
        mobiledata[0] = 0;
        mobiledata[1] = 0;
        *mobiledatalen = 0;
        return;
    }
    len = 2;

    mobiledata[len++] = gMoblieData->MobileVerLen;

    if (gMoblieData->MobileVerLen) {
        mobiledata[len++] = gMoblieData->MobileVer;
    }
    mobiledata[len++] = gMoblieData->BCInfoFileEF04Len;

    if (gMoblieData->BCInfoFileEF04Len) {
        mobiledata[len++] = gMoblieData->BCInfoFileEF04;
    }
    memcpy(&mobiledata[len], gMoblieData->MobileCardSeq, 10);
    len += 10;

    memcpy(&mobiledata[len], gMoblieData->MobileCardInfoRecord, 108);
    len += 108;
    memcpy(&mobiledata[len], gMoblieData->MobileCardName, 20);
    len += 20;
    memcpy(&mobiledata[len], gMoblieData->MobileCardNo, 10);
    len += 10;
    memcpy(&mobiledata[len], gMoblieData->Mobile2Equ, 20);
    len += 20;
    memcpy(&mobiledata[len], gMoblieData->Mobile3Equ, 54);
    len += 54;
    memcpy(&mobiledata[len], gMoblieData->MobileExpiredData, 2);
    len += 2;
    memcpy(&mobiledata[len], gMoblieData->MobileProcessKey, 8);
    len += 8;
    memcpy(&mobiledata[len], gMoblieData->MobileGetTrackTime, 7);
    len += 7;

    mobiledata[0] = (u8)(len >> 8);
    mobiledata[1] = (u8) len;

    *mobiledatalen = len;
}

//zcl add

/*****************************************************************************
** Descriptions:    ��ʼ������(���ô˺�����,������������ʱһ��Ҫ����sdkUpCardTransUnInit����)
** Parameters:      void
** Returned value:  SDK_OK
                    SDK_ERR
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpCardTransInit(void) {
    Emvb_MobileTransInit();

    memset(gMoblieData->MobileCardNo, 0xFF, sizeof(gMoblieData->MobileCardNo));

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    �ͷ���ز����ڴ�(����sdkUpCardTransInit��,������������ʱһ��Ҫ���ô˺���)
** Parameters:      void
** Returned value:  SDK_OK
                    SDK_ERR
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpCardTransUnInit(void) {
    Emvb_MobileTransData_free();

    if (NULL != gPayData.pstIccParam) {
        gPayData.pstIccParam = NULL;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ѡ��Ӧ��
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_PARA_ERR
                    SDK_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
                    SDK_IccDataFormatErr
                    SDK_CardBlockErr
                    SDK_IccReturnErr
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpcardSelectApp(const SDK_ICC_PARAM *pstIccParam) {
    unsigned char retCode;


    if (NULL == pstIccParam) {
        return SDK_PARA_ERR;
    }
    gPayData.pstIccParam = (SDK_ICC_PARAM *) pstIccParam;

    retCode = Emvb_Mobile_SelectApp(gMoblieData);


    s32 rlt = SDK_ERR;
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��02�ļ�
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpcardReadEF02(const SDK_ICC_PARAM *pstIccParam) {
    unsigned char retCode;

    if (NULL == pstIccParam) {
        return SDK_PARA_ERR;
    }

    if (NULL == gPayData.pstIccParam) {
        gPayData.pstIccParam = (SDK_ICC_PARAM *) pstIccParam;
    }
    retCode = Emvb_Mobile_ReadEF02(gMoblieData);

    s32 rlt = SDK_ERR;
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��03�ļ�
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpcardReadEF03(const SDK_ICC_PARAM *pstIccParam) {
    unsigned char retCode;

    if (NULL == pstIccParam) {
        return SDK_PARA_ERR;
    }

    if (NULL == gPayData.pstIccParam) {
        gPayData.pstIccParam = (SDK_ICC_PARAM *) pstIccParam;
    }
    retCode = Emvb_Mobile_ReadEF03(gMoblieData);

    s32 rlt = SDK_ERR;
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    ��ȡBC��Ϣ
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_ERR
                    SDK_PARA_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpcardGetBCInfo(const SDK_ICC_PARAM *pstIccParam) {
    unsigned char retCode;

    if (NULL == pstIccParam) {
        return SDK_PARA_ERR;
    }

    if (NULL == gPayData.pstIccParam) {
        gPayData.pstIccParam = (SDK_ICC_PARAM *) pstIccParam;
    }
    retCode = Emvb_Mobile_GetBCInfo(gMoblieData);

    s32 rlt = SDK_ERR;
    sdkIccEmvToSdkRlt(retCode, &rlt);
    return rlt;
}

/*****************************************************************************
** Descriptions:    �ֻ�֧����������
** Parameters:      const SDK_ICC_PARAM *pstIccParam
** Returned value:  SDK_OK
                    SDK_PARA_ERR
                    SDK_ERR
                    SDK_ICC_NOCARD
                    SDK_IccCommandErr
                    SDK_IccDataFormatErr
                    SDK_CardBlockErr
                    SDK_IccReturnErr
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkUpcardTransFlow(const SDK_ICC_PARAM *pstIccParam) {
    u8 out[64];
    s32 len = 0;
    s32 rlt = SDK_ERR;

    rlt = sdkUpCardTransInit();
    Trace("sdkUpCardTransInit", "rlt = %d\r\n", rlt);

    if (rlt != SDK_OK) {
        return rlt;
    }
    rlt = sdkIccResetIcc(pstIccParam, out, &len);

    Trace("sdkIccResetIcc", "rlt = %d\r\n", rlt);


    if (rlt != SDK_OK) {
        return rlt;
    }
    rlt = sdkUpcardSelectApp(pstIccParam);

    Trace("sdkUpcardSelectApp", "rlt = %d\r\n", rlt);

    if (rlt != SDK_OK) {
        return rlt;
    }
    rlt = sdkUpcardReadEF02(pstIccParam);
    Trace("sdkUpcardReadEF02", "rlt = %d\r\n", rlt);

    if (rlt != SDK_OK) {
        return rlt;
    }
    rlt = sdkUpcardReadEF03(pstIccParam);
    Trace("sdkUpcardReadEF03", "rlt = %d\r\n", rlt);

    if (rlt != SDK_OK) {
        return rlt;
    }
    rlt = sdkUpcardGetBCInfo(pstIccParam);
    Trace("sdkUpcardGetBCInfo", "rlt = %d\r\n", rlt);

    return rlt;
}

/*****************************************************************************
** Descriptions:   ���ͽű�����ʱ��û�н��յ���Ƭ�������ݣ���ʱ��Ҫ�Խű����������ش���
** Parameters:     void
** Returned value:
** Created By:     sujianzhong20130606
** Remarks:        ԭ���ű������һ�ֽ��Ǹ�ֵ0x00;�޸ĺ�Ӧ�ñ�Ϊ0x0n: n��ʾ�ű���������
*****************************************************************************/
void sdkIccDealScriptError(void) {
#if 0       //sxl?��Ҫ�������
    (gstSdkLocalVar.numScriptCom)++;
    gstSdkTermInfo.IssuerScriptStruct[gstSdkLocalVar.IssuScriptNowNum].ScriptResult[0] = 0x00; //�ű�ִ�н��δ֪

    if(gstSdkLocalVar.numScriptCom < 15)
    {
        gstSdkTermInfo.IssuerScriptStruct[gstSdkLocalVar.IssuScriptNowNum].ScriptResult[0] |= gstSdkLocalVar.numScriptCom; //first nibble:0-unknown
    }
    else
    {
        gstSdkTermInfo.IssuerScriptStruct[gstSdkLocalVar.IssuScriptNowNum].ScriptResult[0] |= 0x0F;
    }
    memcpy((u8*)&gstSdkTermInfo.IssuerScriptStruct[gstSdkLocalVar.IssuScriptNowNum].ScriptResult[1], gstSdkLocalVar.curScriptID, 4);
    memcpy(&gstSdkTermInfo.IssuScriptResult[(gstSdkTermInfo.IssuScriptResultNowNum) * 5], (unsigned char*)&gstSdkTermInfo.IssuerScriptStruct[gstSdkLocalVar.IssuScriptNowNum].ScriptResult, 5);
#endif
}

/*****************************************************************************
** Descriptions:   ���CAPK��Ч��
** Parameters:     Input: None
** Returned value:
** Created By:     sujianzhong20130802
** Remarks:        //sxl20140408
*****************************************************************************/
s32 sdkIccCheckCapkExpire() {
        /*
    s32 i;
    u8 AID[6] = {0};
    u8 CAPKI;
    u8 fn[SDK_EMV_MAX_PATH];
    //s32 fn_size = 256;
    u8 *tmp;
    s32 ret, addr, tmplen;
    u8 checksum;
    CAPK_STRUCT *capk;//zhouxiaoxin SDK_ICC_CAPK_STRUCT->CAPK_STRUCT
    u8 TransDate[3] = {0};


    dllemvb_avl_gettagvalue_spec(TAG_CardAID, AID, 0, 5);
    CAPKI = dllemvb_avl_gettagvalue(TAG_CAPKI);
    dllemvb_avl_gettagvalue_spec(TAG_TransDate, TransDate, 0, 3);

    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "newcapk");    //sxl20140331


    for (i = 0; i < MAXCAPKINDEX; i++) {
        if (gCapkIndex[i].ifusedflag == CAPKENABLE) {
            Trace("emv", "TermCapkIndex = %02x\r\n", gCapkIndex[i].CAPKI);

            if ((!memcmp(AID, gCapkIndex[i].RID, 5)) && CAPKI == gCapkIndex[i].CAPKI) {
                tmp = (u8 *) emvb_malloc(CAPKLEN);
                addr = ((u32) i) << CAPKSHIFTNUM;
                tmplen = CAPKLEN;
                ret = sdkReadFile(fn, tmp, addr, &tmplen);

                if (ret == SDK_FILE_OK) {
                    checksum = sdkCalcCrc16(tmp, CAPKLEN - 1);

                    if (checksum == tmp[CAPKLEN - 1]) {
                        capk = (CAPK_STRUCT *) tmp;//zhouxiaoxin SDK_ICC_CAPK_STRUCT->CAPK_STRUCT
                        if (memcmp(TransDate, &capk->ExpireDate[1], 3) > 0) {
                            emvb_free(tmp);
                            return EMV_ERR;
                        }
                    }
                }
                emvb_free(tmp);
                break;
            }
        }
    }

    //zcl add
    if (i >= MAXCAPKINDEX) {
        return EMV_ERR;
    }
         */
    return EMV_OK;
}

/*****************************************************************************
** Descriptions:   ����capkʧЧ�����
** Parameters:     SDK_ICC_PARAM *pstIccParam
** Returned value:
** Created By:     sujianzhong20130802
** Remarks:        //sxl20140408
*****************************************************************************/
s32 Private_sdkIccDataAuth(const SDK_ICC_PARAM *pstIccParam) {
    u8 AIP[2] = {0};
    u8 TermCapab[3] = {0};
    u8 CardTransPredicable;

    if ((pstIccParam == NULL) || (GlbEMVBApp_UnionStruct == NULL) || (gEMVTradeParam == NULL)) {
        return qPBOC_USEPBOC;
    }
    dllemvb_avl_gettagvalue_spec(TAG_AIP, AIP, 0, 2);
    dllemvb_avl_gettagvalue_spec(TAG_TermCapab, TermCapab, 0, 3);
    dllemvb_avl_gettagvalue_spec(TAG_CardTransPredicable, &CardTransPredicable, 0, 1);

    if (((AIP[0] & 0x01) && (TermCapab[2] & 0x08))) {
        dllemvb_avl_setvalue_or(TAG_TVR, 0,
                                0x04);                                   //Offline Combined DDA/AC Generation failed
        dllemvb_avl_setvalue_or(TAG_TSI, 0, 0x80);
    }
    else if ((AIP[0] & 0x20) && (TermCapab[2] & 0x40)) //ICC and terminal support dynamic data auth.
    {
        dllemvb_avl_setvalue_or(TAG_TVR, 0,
                                0x08);                                    //Offline dynamic Data Authentication failed
        dllemvb_avl_setvalue_or(TAG_TSI, 0, 0x80);
    }
    else if ((AIP[0] & 0x40) && (TermCapab[2] & 0x80)) //ICC and terminal support static data auth.
    {
        dllemvb_avl_setvalue_or(TAG_TVR, 0,
                                0x40);                                //Offline Static Data Authentication failed
        dllemvb_avl_setvalue_or(TAG_TSI, 0, 0x80);
    }
    else {
        dllemvb_avl_setvalue_or(TAG_TVR, 0,
                                0x80);                                    //Offline Data Authentication not performed
        dllemvb_avl_setvalue_and(TAG_TSI, 0, 0x7F);
    }

    if ((SDK_ICC_RF == pstIccParam->ucCardMode) &&
        (pstIccParam->ucCLType == 2 || pstIccParam->ucCLType == 3)) { //qpboc
        if ((CardTransPredicable & 0x20) == 0x20) {
            gEMVTradeParam->TransResult = ONLINE_WAIT;
            return EMV_OK;
        }
        else if ((CardTransPredicable & 0x10) == 0x10) {
            return qPBOC_USEPBOC;
        }
        else {
            gEMVTradeParam->TransResult = OFFLINE_DECLINE;
        }
    }
    return EMV_OK;
}

static void EMVB_TradeParam_Destory(void) {

    if (gEMVTradeParam != NULL) {
//        #ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory1\r\n");

//		#endif
        if (gEMVTradeParam->CAPK != NULL) {
            emvb_free(gEMVTradeParam->CAPK);
        }
//		#ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory2\r\n");
//		#endif

        if (gEMVTradeParam->SelectedApp != NULL) {
            emvb_free(gEMVTradeParam->SelectedApp);
        }
//		#ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory3\r\n");
//		#endif

        if (gEMVTradeParam->AppListCandidate != NULL) {
            emvb_free(gEMVTradeParam->AppListCandidate);
        }
//		#ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory4\r\n");
//		#endif

        if (gEMVTradeParam->gPaypassProceFlag != NULL) {
            emvb_free(gEMVTradeParam->gPaypassProceFlag);
        }
//		#ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory5\r\n");
//		#endif

        emvb_free(gEMVTradeParam);

        gEMVTradeParam = NULL;

//		#ifdef EMVB_DEBUG
        Trace("emvbtest", "\r\nEMVB_TradeParam_Destory6\r\n");
//		#endif
    }

    if (GlbEMVBApp_UnionStruct != NULL) {
        emvb_free(GlbEMVBApp_UnionStruct);
        GlbEMVBApp_UnionStruct = NULL;
    }

    if (gMoblieData != NULL)      //�ͷ���Դ  //sxl? gMoblieData��ԴҪ�ǵ��ͷ�
    {
        emvb_free(gMoblieData);
        gMoblieData = NULL;
    }
}

void sdkIccTransFreePartMem(s32 retCode) {
    //sxl?
    if (retCode != SDK_OnlineWait)       //ͬʱ�ǽ�IC�����ټ���������
    {
        EMVB_TradeParam_Destory();          //ֻ������Դ
    }
}

void sdkIccTransFreeAllMem(void) {
    Trace("ICcFreeAllMem", "EMVB_TradeParam_Destory");
    EMVB_TradeParam_Destory();
    Trace("ICcFreeAllMem", "dllemvb_avl_destory");
    dllemvb_avl_destory();
    Trace("ICcFreeAllMem", "sdkIccTransFreeAllMemexit");
}

void
EMVB_restarttrade_SelectNext(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                  SDK_ICC_PARAM *pstIccParam) {
    sdkIccTransInit(pstIccTradeParam);
    gPayData.pstIccParam = pstIccParam;
}

void sdkIccSaveEmvTradeGlbValue(void) {
    dllemvb_avl_gettagvalue_spec(TAG_TVR, gEmvTradeGlbValue.TVR, 0, 5);
    dllemvb_avl_gettagvalue_spec(TAG_TSI, gEmvTradeGlbValue.TSI, 0, 2);
}

void sdkIccGetTVRTSI(SDK_ICC_EMVTRADEGLBVALUE *pTradeGlbValue) {
    memcpy(pTradeGlbValue, &gEmvTradeGlbValue, sizeof(SDK_ICC_EMVTRADEGLBVALUE));
}

void dev_memset(void *dst, unsigned char data, unsigned int len) {
    unsigned int i;

    for (i = 0; i < len; i++) {
        *((char *) dst + i) = data;
    }
}

void dev_memcpy(void *dst, void *src, unsigned int len) {
    unsigned int i;

    for (i = 0; i < len; i++) {
        *((char *) dst + i) = *((char *) src + i);
    }
}

void uart_printf(char *fmt, ...) {
    Trace("emvb", fmt);
}

