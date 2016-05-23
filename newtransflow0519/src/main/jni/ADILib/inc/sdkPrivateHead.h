/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD
 ******************************************************************************
  �� �� ��   : Queue.h
  �� �� ��   : ����
  ��    ��   : shiweisong
  ��������   : 2013��4��12��
  ����޸�   :
  ��������   : Queue.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��4��12��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __PRIVATEHEAD_H__
#define __PRIVATEHEAD_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
#define SDK_PRIVATE_FIFO_SIZE                   2048                      //FIFO������󳤶�//maximun length of FIFO data
#define SDK_UART_USB_PORT                        14                        //USB����
typedef struct                                                            //�ܵ�����//fifo command
{
    u16 usCmd;                                                            //������//command
    u16 usLen;                                                            //����//length
    u8 heData[SDK_PRIVATE_FIFO_SIZE];                                     //��������//data content
} SDK_PRIVATE_FIFO;


extern s32 sdkCommGetWirelessModuleIMSI(u8 * pasDest);

extern s32 sdkCommSetHardModemMode(SDK_COMM_HARD_MODEM_MODE eMode);

extern s32 sdkSysPlaySound(SDK_SYS_BEEP_TYPE type, s32 level);


extern s32 Private_sdkCheckFileSys(void);

extern s32 Private_sdkDeleteFileSys(void);

extern s32 Private_sdkCreatFastFile(const u8 *pas);

extern s32 Private_sdkCloseFastFile(const u8 *pas);

extern s32 Private_sdkPEDSetUartBaudRate(s32 siRate);

extern s32 Private_sdkInitFifo(s32 CurFifoNo);

extern s32 Priavte_GetDaemonFifoName(u8 * pasName);

extern bool Private_sdkJudgeChn(const u8 heC1, const u8 heC2, const u8 heC3, const u8 heC4);

extern s32 Private_sdkReadFifo(SDK_PRIVATE_FIFO *pstFifo, s32 siMs);

extern s32 Private_sdkWriteFifo(const SDK_PRIVATE_FIFO *pstFifo);

extern s32 Private_sdkWriteSelfFifo(const SDK_PRIVATE_FIFO *pstFifo);

//?????
extern s32 Private_sdkPEDSetSN(const u8 *pDataIn);

extern s32 Private_sdkGetKeyValue(void);


//����ϵͳǿ���˳���־//set system force out flag
extern bool Private_sdkSysSetFroceOut(void);

//extern void dev_ts_flush(void);//shijianglong 2012.12.18 10:49 K390������ʱ����//used for touch screen
extern s32 Private_sdkPciClose(void);

extern s32 Private_sdkPciFileauthentication(u8 filetype, u32 flagtailpos, const u8 *srcfilepath,
                                            const u8 *dstfilepath);

extern s32 Private_sdkPciGeneratemmk(void);

extern s32 Private_sdkPciMiscCheckExtsupplyVoltage(void);

extern s32 Private_sdkPciMmkelrckcheck(void);

extern s32 Private_sdkPciOpen(void);

extern s32 Private_sdkSysLicenseVerify(const u8 *pasFile);

/*=======BEGIN: sujianzhong 2013.02.24  12:51 modify===========*/
extern s32 Private_sdkSysGetSlotNO(u8 * ucSlotNO);

extern s32 Private_sdkIccLoadAccNo(SDK_ICC_TRADE_PARAM * pstIccTradeParam,
                                   SDK_ICC_PARAM * pstIccParam);

extern bool Private_sdkIccMatchTagFromPDOL(const u8 *pheTag);

extern s32 sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);

//sjz modify 2012.08.15 14:55
//���������ʱ����������emv���׿ռ���
extern s32 Private_sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);

//sjz modify 2012.08.15 14:55
//����Private_sdkIccUnzipPagData����������
extern s32 Private_sdkIccConfigPagTLV(const u8 *pheTag, const u8 *pheInData,
                                      s32 siInLen); //sujianzhong 2013.03.08 14:35
//��ȡPrivate_sdkIccUnzipPagData����������
extern s32 Private_sdkIccReadPagTLV(const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen);

//sujianzhong 2013.03.08 14:35
extern s32 Private_sdkIccImportRespCode(u8 ucOnlineResult,
                                        const u8 *pheRspCode);    //sjz modify 2012.08.15 18:38



extern void Private_sdkSysListTrace(SDK_SYS_LIST_HAND hand);

extern s32 Private_sdkGetPosRoot(u8 * lpOut);

extern s32 Private_sdkSysGetSignVer(const u8 *fpFile, const u8 *pFlag, u8 *lpOut);

extern s32 Private_sdkSysLangChange(u8 *lpOut, const u8 *lpIn);

extern s32 Private_sdkDispGetHandInputAreaPixel(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY);

extern s32 Private_sdkRSAPublicBlock(u8 *pucDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen,
                                     const SDK_RSA_PUBLIC_KEY *pstPublicKey);

extern s32 Private_sdkRSAPrivateBlock(u8 *pucDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen,
                                      const SDK_RSA_PRIVATE_KEY *pstPrivateKey);

extern s32 Private_CloseDevices(void);

extern s32 Private_sdkDispGetHandInputAreaPixel(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY);

extern s32 Private_sdkSysSetNetCard(bool bIsNetCardOpen);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __PRIVATEHEAD_H__ */







