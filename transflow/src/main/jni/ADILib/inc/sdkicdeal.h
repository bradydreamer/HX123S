#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef SDKICDEAL_H
#define SDKICDEAL_H

#define    SDK_ICC_RFCARD_A              0x01            //�ǽ�A��//contactless A card
#define    SDK_ICC_RFCARD_B              0x02            //�ǽ�B��//contactless B card
#define    SDK_ICC_RFCARD_FELICA         0x04           //�ǽ�FeLiCa�� 13.56M//sujianzhong 2013.08.15 16:13
#define    SDK_ICC_RFCARD_24G            0x08            //�й��ƶ�(��ͨ)2.4G��//China Mobile(China Unicom) 2.4G card
#define    SDK_ICC_TIMER_QUERYRF         10000           //�ǽ�Ѱ��ʱ��10s  //contactless query card time 10s


//===========================Mifare Card=====================================/
#define    SDK_RF_KEYA                   0x60            //RF Key A
#define    SDK_RF_KEYB                   0x61            //RF Key B
#define    SDK_FORMAT_BLOCK              0x00            //mifare���ʽ��//format mifare block
#define    SDK_INC_BLOCK                 0x01            //mifare����ֵ//increase in value mifare block
#define    SDK_DEC_BLOCK                 0x02            //mifare���ֵ//decrease in value mifare block

#define CARDMEM_MAX_LEN                 249
typedef struct {
    u16 ucAddress;                                       //����ʼλ��//read the start address
    u16 ucReadLen;                                       //�����ݳ���//read data length
    u16 ucZone;
} ATTRIBUTE
SDK_ICC_RMEMIN;                                         //��Memory���������//read the memory card incoming parameters

typedef struct {
    u16 ucAddress;                                       //д��ʼλ��//wirte the start position
    u8 ucWriteData[CARDMEM_MAX_LEN];                                      //��д����//data to be written
    u16 ucWriteLen;                                      //д���ݳ���//write data length
    u16 ucZone;
} ATTRIBUTE
SDK_ICC_WMEMIN;                                         // дMemory���������//write memory card coming parameters

/*=======BEGIN: sujianzhong 2013.04.16  9:27 modify===========*/
// 1 ��1.1 ����APDU��ʽ//1.    1.1 command  APDU format
typedef struct {
    u8 Command[4];
    u16 Lc;
    u8 DataIn[300];
    u16 Le;
    u8 EnableCancel;
} SDK_ICC_APDU_SEND;


// 1.2 Ӧ��APDU��ʽ//1.2 response  APDU format
typedef struct {
    u8 ReadCardDataOk;
    u16 LenOut;
    u8 DataOut[300];
    u8 SWA;
    u8 SWB;
} SDK_ICC_APDU_RESP;

//��ӵ���չ����
typedef struct {
    u8 uiLen;
    u8 ucSysCode[256];
    void *pVar;
} SDK_ICC_SYS_CODE;
/*====================== END======================== */
typedef enum {
    SDK_IC_MEM_CHECK_KEY = 1,
    SDK_IC_MEM_CHECK_WRITE_KEY,
    SDK_IC_MEM_CHECK_READ_KEY,
    SDK_IC_MEM_MODIFY_KEY,
    SDK_IC_MEM_MODIFY_WRITE_KEY,
    SDK_IC_MEM_MODIFY_READ_KEY
} SDK_IC_MEM_OPER;
typedef enum {
    SDK_MEM_ZONE_0 = 0X00,
    SDK_MEM_ZONE_1 = 0X01,
    SDK_MEM_ZONE_2 = 0X02,
    SDK_MEM_ZONE_3 = 0X03,
    SDK_MEM_ZONE_4 = 0X04,
    SDK_MEM_ZONE_5 = 0X05,
    SDK_MEM_ZONE_6 = 0X06,
    SDK_MEM_ZONE_7 = 0X07
} SDK_IC_MEM_ZONE;

extern s32 sdkIccRFQuery(u8 ucCardType, u8 *pheOutBuf, s32 siTimerCnt);

extern s32 sdkIccReset(const SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccMemMifarePowerDown(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccSendData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheInBuf, s32 siInLen,
                          u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccSendAPDU(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_APDU_SEND *pstApduSend,
                          SDK_ICC_APDU_RESP *pstApduResp);

//sujianzhong 2013.04.16 9:27
extern bool sdkIccDetectMove(void);

//=========================Memory Card=====================================/
extern s32 sdkIccMemCheck(u8 * pheOutBuf, s32 * psiOutLen);

extern s32 sdkIccMemReadData(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_RMEMIN *pstRMemInData,
                             u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccMemWriteData(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_WMEMIN *pstWMemInData,
                              u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccMemCheckPwd(const SDK_ICC_PARAM *pstIccParam, const u8 *phePwd, u8 *pheOutBuf,
                             s32 *psiOutLen);

extern s32 sdkIccMemModifyPwd(const SDK_ICC_PARAM *pstIccParam, const u8 *pheOldPwd,
                              const u8 *pheNewPwd, u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccMem4442ReadPwdAndPwdCount(u8 * pheOutBuf, s32 * psiOutLen);
//=========================Mifare Card=====================================/
extern s32 sdkIccMifareReadCardNum(u8 * pheDest);

extern s32 sdkIccMifareVerifyKey(u8 ucBlock, u8 ucKeyType, const u8 *pheKey, s32 siKeyLen,
                                 u8 *pheDest, s32 *psiDestLen);

extern s32 sdkIccMifareReadBlock(u8 ucBlock, u8 *pheDest, s32 *psiDestLen);

extern s32 sdkIccMifareWriteBlock(u8 ucBlock, const u8 *pheScr, s32 siSrcLen, u8 *pheDest,
                                  s32 *psiDestLen);

extern s32 sdkIccMifareOperateBlock(u8 ucOptType, u8 ucBlock, const u8 *pheSrc, u8 *pheDest,
                                    s32 *psiDestLen);

extern s32 sdkIccMifareBackUpBlock(u8 ucS_block, u8 ucD_block, u8 *pheDest, s32 *psiDestLen);

//=========================CPU Card========================================/
extern s32 sdkIccIsIccCardIn(s32 siTimerCnt, u8 *pheOutBuf, s32 *psiOutLen);

extern s32 sdkIccIsInSlot(void);

extern s32 sdkIccIsCardOut(s32 siTimerCnt);

extern s32 sdkIccMemPowerOn(SDK_ICC_PARAM * pstIccParam, u8 * pheOutBuf, s32 * psiOutLen);

extern s32 sdkIcGetIcdealCardNo(void);

//sujianzhong 2013.04.17 9:54
extern s32 sdkIccJudgeHave24GModule(void);

//sujianzhong 2013.08.15 8:59
extern s32 sdkIccOpenIcDev(void);

extern s32 sdkIccCloseIcDev(void);

extern s32 sdkIccOpenRfDev(void);

extern s32 sdkIccCloseRfDev(void);

extern s32 sdkIccIsInRfModule(s32 siTimeOut);

extern s32 sdkIccSlotPowerDown(const SDK_ICC_PARAM *pstIccParam);

extern s32 sdkIccRFQueryEx(u8 ucCardType, u8 *pheOutBuf, s32 siTimerCnt,
                           SDK_ICC_SYS_CODE *pstSysCode);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

