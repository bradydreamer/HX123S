#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <android/log.h>

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef SDKMAG_H
#define SDKMAG_H

#define    SDK_ICC_MANUAL       0x01                // ֧��������//support manual input
#define    SDK_ICC_MAG          0x02                // ֧��ˢ��//support swiping card
#define    SDK_ICC_ICC          0x04                // ֧�ֽӴ�ʽ//support contact
#define    SDK_ICC_RF           0x08                // ֧�ַǽӴ�//support contactless


typedef enum {
    SDK_ICC_NOCARD = SDK_SYS_RET_ERR_MAX + (-1), // ����޿�//detected no card
    SDK_ICC_MORECARD = SDK_SYS_RET_ERR_MAX + (-2), // ��⵽���ſ�//detected multiple cards
    SDK_ICC_BRUSH_ERR = SDK_SYS_RET_ERR_MAX + (-3), // ˢ������//swiping card error
    SDK_ICC_NODATA = SDK_SYS_RET_ERR_MAX + (-4), //������//no data
    SDK_ICC_OVER_LIMIT = SDK_SYS_RET_ERR_MAX + (-5), //����//over limit
    SDK_ICC_RESET_ERR = SDK_SYS_RET_ERR_MAX + (-6),  //��λʧ��//reset error
    SDK_ICC_BAT_VOLTAGE_LOW = SDK_SYS_RET_ERR_MAX + (-7)  //��ص�ѹ����
} SDK_ICC_RETURN;

typedef enum {
    SDK_ICC_CPU,                                     // CPU��//CPU card//Ĭ��13.56
    SDK_ICC_MOBILE,                                  //�ֻ���//mobile phone card
    SDK_ICC_SAM,                                     // ATR������ΪISO/IEC 7816��׼��SAM��//ATR baud rate is ISO/IEC 7816 srandard SAM card
    SDK_ICC_PSAM,                                    //ATR������Ϊ�й����貿��׼PSAM��//ATR baud rate is the Ministry of Construction of China standard PSAM card
    SDK_ICC_MEMORY,                                  // Memory��//Memory card
    SDK_ICC_SLE4428,                                 //MEMORY CARD SLE4428           //sjl20120724
    SDK_ICC_SLE4442,                                 //MEMORY CARD SLE4442
    SDK_ICC_AT24Cxx,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C01,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C02,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C04,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C08,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C16,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C32,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_AT24C64,                                 //MEMORY CARD AT24Cxx
    SDK_ICC_1604,                                    //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
    SDK_ICC_1608,                                    //MEMORY CARD AT88SC1608/AT88SC153
    SDK_ICC_AT88SC153,                               //MEMORY CARD AT88SC153
    SDK_ICC_AT88SC1608,                              //MEMORY CARD AT88SC1608/
    SDK_ICC_MIFARE,                                  //MIFARE��//MIFARE card
    /*=======BEGIN: sujianzhong 2013.08.15  17:40 modify===========*/
            SDK_ICC_CPU24,                                  //2.4G CPU��
    /*====================== END======================== */
            SDK_ICC_PSAM_4, //PSAM4��Ƶ��
    SDK_ICC_AT88SC101,                               //MEMORY CARD AT88SC101  
    SDK_ICC_AT88SC102,                               //MEMORY CARD AT88SC102   
    SDK_ICC_AT88SC1604,                              //MEMORY CARD AT88SC1604
} SDK_ICC_CARDTYPE;                                   // ������//card type

typedef struct {
    u8 ucCardMode;                                  // ʵ�ʵĶ���ģʽ��01�����롢02ˢ����04�Ӵ���08�ǽ�//actual reading card mode: contactless, contact, swiping card, manual input
    u8 ucCLType;                                    // 01:ʵ���߷ǽ�PBOC����;02:ʵ����QPBOC;03ʵ���߷ǽ�MSD
    u8 ucSlotNo;                                    // ������//slot number
    SDK_ICC_CARDTYPE eCardType;                     // ������:��CPU��/Sam��/Psam��/memory ��/Mifare��  //card type: such as CPU card / Sam card / Psam card / memory card / Mifare card
} SDK_ICC_PARAM;                                     // ������//card parameters

typedef struct {

    bool bIsIccMagCard;                              //��ICC�ſ�//there is ICC magcard
    u8 ucUIDLen;                                     //�����ų���//UID number length
    u8 ucUID[20];                                    //оƬ��UID//UID number data
    u8 ucCardLen;                                 //���ų���//card number length
    u8 asCardNO[20];                                 //��������//card number data
    u8 asExpiredDate[5];                             //����Ч����(YYMM)(1950-2049)//card expired date(YYMM)(1950-2049)
    u8 ucTrack1Len;                                  //1�ŵ�����//track 1 length
    u8 asTrack1Data[128];                             //1�ŵ���Ч����76��ASCII(7bits)   //track 1 valid data are 76 ASCII(7 bits)
    u8 ucTrack2Len;                                  //2�ŵ�����//track 2 length
    u8 asTrack2Data[128];                             //2�ŵ�����,���������"="Ҳ��0x30  //track 2 data, the "=" in data also need add 0x37
    u8 ucTrack3Len;                                  //3�ŵ�����//track 3 length
    u8 asTrack3Data[128];                            //3�ŵ�����//track 3 data
    u8 heReserved[256];                              //Ԥ��������չ��Ŀ���ʹ��//reserved for future expansion of other cards
} ATTRIBUTE
SDK_ICC_CARDDATA;                                   // ������//card data


#define     SDK_ICC_CARDNO_MIN_NUM  13          //��������λ��//the minimum number of card number
#define     SDK_ICC_TIMER_AUTO    30000         //�Զ����ض�ʱ��30s   //automatically return timer 30s
#define     SDK_ICC_TIMER_RESET   2000          // �ǽӸ�λʱ��2s  //contactless reset time 2s


extern s32 sdkIccOpenMag(void);

extern s32 sdkIccGetMagData(SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData);

extern void sdkIccCloseMag(void);


extern s32 sdkIccGetCardData(SDK_ICC_CARDDATA * pstCardData);

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

