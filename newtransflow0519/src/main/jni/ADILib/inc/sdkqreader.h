#ifndef _QREADER_H_
#define _QREADER_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef struct {
    u8 heTermTransPredicable[4];    //"9F66"
    u8 heMerchCateCode[2];          //"9F15"
    u8 heAcquireID[6];              //"9F01"
    u8 bcCountryCode[2];            //"9F1A"
    u8 bcTransCurcyCode[2];         //"5F2A"
    u8 ucTransCurcyExp;             //"5F36"
    u8 ucTransTypeValue;            //"9C"
    u8 asMerchID[16];               //"9F16"
    u8 asMerchantName[129];         //"9F4E"
    u8 asTermID[9];                 //"9F1C"
} SDK_ICC_SET_READER_PARAM;


extern void sdkIccPowerStartReader(void);

//�޸�"9F66/9F15/9F01/9F1A/5F2A/5F36/9C/9F16/9F1C/9F4E"��Ҫ����sdkIccSyncReaderParamͬ��������  //modify "9F66/9F15/9F01/9F1A/5F2A/5F36/9C/9F16/9F1C/9F4E" need call sdkIccSyncReaderParam to synchronize card reader
//extern s32 sdkIccSyncReaderParam(SDK_ICC_SET_READER_PARAM *pstSetReaderParam); //sujianzhong 2012.07.31 15:17
extern s32 sdkIccDealQReaderTrade(const SDK_ICC_TRADE_PARAM *pIccTradeParam); //sjz

/*=======BEGIN: sujianzhong 2013.02.24  12:51 modify===========*/
extern void sdkIccSaveQreaderCurrentStatusRamdisk(void);

//��Ӧ���л�ǰ����ô˺����������������
extern void sdkIccSwitchAppResumeQreader(void); //��Ӧ���л�������ô˺����ָ�����������
/*====================== END======================== */



#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

