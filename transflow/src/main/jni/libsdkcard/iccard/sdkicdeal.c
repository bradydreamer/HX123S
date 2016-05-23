#include "sdkdevOption.h"
#include "sdkGlobal.h"
//#include "dllemvb.h"
#include "sdkIccPrivateHead.h"


#ifndef  DEBUG_ICC
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif



//DBָ��µ�mifare��������ʽ
#define MIFARE_DB_CHECK_KEY             0x77                    //check the key of a card
#define MIFARE_DB_READ_BLOCK            0x78                    //read a data block in a area of a card
#define MIFARE_DB_WRITE_BLOCK           0x79                    //write a data block in a area of a card
#define MIFARE_DB_VALUE_BLOCK           0x7A                    //block value-operated
#define MIFARE_DB_RECOVER_BLOCK         0X7B                    //recover the block of a card

//============================card max len====================================/
//#define CARDMEM_MAX_LEN                 249
#define CARD_4428_MAX_LEN               1024                    // the max length of card data
#define CARD_4442_MAX_LEN               256                    // the max length of card data/* End */
#define CARD_AT24C01_MAX_LEN            (1024 * 1)
#define CARD_AT24C02_MAX_LEN            (1024 * 2)
#define CARD_AT24C04_MAX_LEN            (1024 * 4)
#define CARD_AT24C08_MAX_LEN            (1024 * 8)
#define CARD_AT24C16_MAX_LEN            (1024 * 16)
#define CARD_AT24C32_MAX_LEN            (1024 * 32)
#define CARD_AT24C64_MAX_LEN            (1024 * 64)
#define CARD_1604_MAX_LEN               (1024 * 16)
#define CARD_1608_MAX_LEN                256
#define CARD_AT88SC101_MAX_LEN          (1024 * 1)
#define CARD_AT88SC153_MAX_LEN          (3 * 64 * 8)
#define CARD_AT88SC1608_MAX_LEN         (8 * 256 * 8)


#define READ_4428                       0x00                    //��4428����
#define CHECK_PWD_4428                  0x01                    //У��4428����
#define WRITE_4428                      0x02                    //д4428����
#define MODIFY_PWD_4428                 0x03                    //����4428����
#define PROBITREAD_4428                 0x1C                    //������λ4428������
#define PROBITWRITE_4428                0x1D                    //������λ4428д����
#define READ_4442                       0x04                    //��4442����
#define CHECK_PWD_4442                  0x05                    //У��4442����
#define WRITE_4442                      0x06                    //д4442����
#define MODIFY_PWD_4442                 0x07                    //����4442����
#define POWER_DOWN_4428_4442            0x08                    //4428��4442���µ�
#define READ_PROTECT_BIT                0x09                    //������λ
#define WRITE_PROTECT_BIT               0x0A                    //д����λ
#define READ_PWD_AND_PWDCOUNT           0x0B                    //��������������Ϳ�����
#define POWER_ON_4428_4442              0x0C                    //4428��4442���ϵ�
#define RESET_4428                      0x0D                    //��λ4428��
#define MEMCARD_DETECT                  0x0E                    //��ѯMemory ��,��Ӧ��1:�п�;5:�޿�
#define RESET_4442                      0x0F                    //��λ4442��

#define OUTSIDE_PINPAD_FALG     0 //�Ƿ��֧�������������
#undef OUTSIDE_PINPAD_FALG

typedef enum memCardCmd {
    MEM_CARD_POWER_ON, //�ϵ� mem�������ϵ磬������open ic ��
    MEM_CARD_SEARCH, //MEM ��Ѱ��
    MEM_CARD_RESET, //MEM ����λ
    MEM_CARD_READ, //������
    MEM_CARD_WRITE, //д����
    MEM_CARD_CHECK_PWD, //�˶�����
    MEM_CARD_MODIFY_PWD, //�޸�����
    MEM_CARD_READ_CHK_COUNT, //��ȡУ������ʣ�����
    MEM_CARD_POWER_DOWN, //�µ�
} memCardCmd;


u8 gucSdkIccRfCheckBattle;                              //�ǽ�Ѱ����ʱ���Ƿ��жϵ�ص�ѹ��ֻ�е�һ��ȥѰ����ʱ����жϣ���������Ҫ�ж�
//u8 gucSdkICCRandom[8]; //IC�����������
//struct _qReaderData gstSdkIccQreaderDataSend;    //����������͵�����
u8 gucSdkIccEmvInQreader;                                       //EMV L2�ڶ����������
u8 gucSdkIccQreaderType;                      //����������  zjz20111108


/*=======BEGIN: ���ٲ� 2013.09.03  9:8 add===========*/
//��֮ǰ�������ڵ�������������̵ĺ����Ƶ�����
/*====================== END======================== */

/*****************************************************************************
** Descriptions:	�жϵ�ǰ�����Ƿ���Դ򿪷ǽ�
** Parameters:          void
** Returned value:  SDK_OK:����
                    SDK_ICC_BAT_VOLTAGE_LOW:������
** Created By:		���ƽ  2014.11.28
** Remarks:
*****************************************************************************/
s32 sdkRfBatteryEnough(void) {
    //zhouxiaoxin
//    s32 batStatus = ddi_sys_bat_status();
//
//    if (batStatus < DDI_POWER_ONE || batStatus == DDI_POWER_AD_CHARGE_ONE ||
//        batStatus == DDI_POWER_AD_BATTERY_ERR) {
//        Assert(0);
//        return SDK_ICC_BAT_VOLTAGE_LOW;
//    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        ��λ�Ӵ�ʽIC��
** Parameters:          SDK_ICC_CARDTYPE cardtype:����λ�Ŀ�����
                               u8 *rbuf:��λ��������
                               s32 timeout:��ʱʱ��
** Returned value:
** Created By:		���ٲ�  2013.07.22
** Remarks:
*****************************************************************************/
s32 sdk_dev_iccard_reset(SDK_ICC_CARDTYPE cardtype, u8 *sbuf, u8 *rbuf, s32 timeout) {
    s32 len = 0;
    s32 rslt = 0;
    u32 cardno;

    //u8 aswData[64];

    Trace("zjp", "cardType = %d\r\n", cardtype);
    TraceHex("zjp", "sbuf", sbuf, 16);

    if (sbuf == NULL || rbuf == NULL || timeout < 0) {
        return SDK_PARA_ERR;
    }
    Trace("icc", "sdk_dev_iccard_reset:cardtype = %d\r\n", cardtype);

    if (cardtype == SDK_ICC_CPU || cardtype == SDK_ICC_SAM || cardtype == SDK_ICC_PSAM) {
        cardno = (u32) * sbuf;
        rslt = ddi_iccpsam_poweron(cardno, rbuf);
        Trace("icc", "rslt = %d\r\n", rslt);
        if (DDI_OK == rslt) {
            sdkBcdToU32(&len, rbuf, 1);
            memcpy(rbuf, rbuf + 1, len);
            return len;
        }
        else {
            return SDK_ERR;
        }
    }
    else {
        switch (cardtype) {
            case SDK_ICC_SLE4428:                            //MEMORY CARD SLE4428           //sjl20120724
            {
                rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_RESET, 0, 0);
                break;
            }

            case SDK_ICC_SLE4442:                            //MEMORY CARD SLE4442
            {
                rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_RESET, 0, 0);
                break;
            }

            case SDK_ICC_AT24C01:                            //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C02:                           //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C04:                          //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C08:                            //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C16:                            //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C32:                           //MEMORY CARD AT24Cxx
            case SDK_ICC_AT24C64:                          //MEMORY CARD AT24Cxx
            {
                rslt = DDI_OK;
                break;
            }

            case SDK_ICC_1604:                              //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
            {
                rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_RESET, 0, 0);
                break;
            }

            case SDK_ICC_AT88SC153:                         //MEMORY CARD AT88SC153
            case SDK_ICC_AT88SC1608:                        //MEMORY CARD AT88SC1608/
            {
                rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_RESET, 0, 0);
                break;
            }

            default: {
                rslt = DDI_ERR;
                break;
            }
        }

        Trace("icc", "rslt = %d\r\n", rslt);

        if (DDI_OK == rslt) {
            rbuf[0] = 0;
            return SDK_OK;
        }
        else {
            return SDK_ERR;
        }
    }
}

typedef struct _strAT88SCxxVerify {
    u32 m_type;         //��Ƭ����//0102-AT88SC153/AT88SC1608
    u32 m_mode;         //У��ģʽ 0:��У�� 1:дУ��
    u8 m_key[3];        //У������
    u8 m_index;         //��������(101��1������:������Χ0; 153��2������:������Χ0 ~ 1; 1608��8������:������Χ0 ~ 7 )
} strAT88SCxxVerify;
/*
   ���  ��Ƭ���ͣ�m_type��  ����
   1     AT88SC101           0
   2     AT88SC153           1
   3     AT88SC1608          2
 */

/*****************************************************************************
** Descriptions:	mem��У������
** Parameters:          SDK_ICC_CARDTYPE cardType ������
                    const u8 *phePwd bcd����
                    const u8 *pAT88SCxxVerify  ֻ��AT88SCxxϵ�п���Ч����������Ч��
                    u32 *remainCheckTime ����ʣ��У���������
** Returned value:
** Created By:		���ƽ  2014.06.20
** Remarks:             ��������ΪAT88SCxxϵ��ʱ phePwd ��Ч
*****************************************************************************/
static s32 sdk_dev_iccard_mem_check_pwd(SDK_ICC_CARDTYPE cardType, const u8 *phePwd,
                                        const u8 *pAT88SCxxVerify, u32 *remainCheckTime) {
    //s32 len = 0;
    s32 rslt = 0;

    //u32 cardno;
    //u8 aswData[64];

    if (remainCheckTime == NULL) {
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2014.11.11  14:41 */
    Trace("zjp", "cardType = %d\r\n", cardType);
    TraceHex("zjp", "phePwd", phePwd, 6);

    if( pAT88SCxxVerify != NULL )
    {
        TraceHex("zjp", "pAT88SCxxVerify", pAT88SCxxVerify, 14);
    }
#endif /* if 0 */

    *remainCheckTime = 0;

    switch (cardType) {
        case SDK_ICC_SLE4428:                          //MEMORY CARD SLE4428           //sjl20120724
        {
            if (phePwd == NULL) {
                return SDK_PARA_ERR;
            }
            TraceHex("zjp", "phePwd", phePwd, 6);
            rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_VERIFY, (u32) phePwd, (u32) remainCheckTime);
            Trace("zjp", "rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_SLE4442:                            //MEMORY CARD SLE4442
        {
            if (phePwd == NULL) {
                return SDK_PARA_ERR;
            }
            rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_VERIFY, (u32) phePwd, (u32) remainCheckTime);
            break;
        }

        case SDK_ICC_AT24C01:                            //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C02:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C04:                          //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C08:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C16:                            //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C32:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C64:                           //MEMORY CARD AT24Cxx
        { //AT24Cϵ������У������
            *remainCheckTime = 0xffffffff;
            rslt = DDI_OK;
            break;
        }

        case SDK_ICC_1604:                               //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
        {
            if (phePwd == NULL) {
                return SDK_PARA_ERR;
            }
            rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_VERIFY, (u32) phePwd,
                                     (u32) remainCheckTime);
            break;
        }

        case SDK_ICC_AT88SC153:                         //MEMORY CARD AT88SC153
        case SDK_ICC_AT88SC1608:                         //MEMORY CARD AT88SC1608/
        {
            if (pAT88SCxxVerify == NULL) {
                return SDK_PARA_ERR;
            }
            rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_VERIFY, (u32)(pAT88SCxxVerify),
                                     (u32) remainCheckTime);
            break;
        }

        default: {
            *remainCheckTime = 0;
            rslt = DDI_ERR;
            break;
        }
    }

    Trace("icc", "sdk_dev_iccard_mem_check_pwd :rslt = %d remainCheckTime =%d\r\n", rslt,
          *remainCheckTime);

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

typedef struct _strAT88SCxxReadEC {
    u32 m_type; //��Ƭ����
    u32 m_mode; //У��ģʽ 0:��У�� 1:дУ��
    u32 m_index; //��������
} strAT88SCxxReadEC;
typedef struct _strIs23SC1604ReadEC {
    u32 m_zone; //����� 0:�����洢�� 1~4:Ӧ����1~4
    u32 m_mode; //У������ 0:��ȫ���� 1:��������
} strIs23SC1604ReadEC;

/*****************************************************************************
** Descriptions:	mem����ȡʣ��У�����
** Parameters:          SDK_ICC_CARDTYPE cardType ������
                    const u8 *phePwd bcd����
                    const u8 *pAT88SCxxVerify  ֻ��AT88SCxxϵ�п���Ч����������Ч��
                    u32 *remainCheckTime ����ʣ��У���������
** Returned value:
** Created By:		���ƽ  2014.06.20
** Remarks:             ��������ΪAT88SCxxϵ��ʱ phePwd ��Ч
*****************************************************************************/
static s32 sdk_dev_iccard_mem_readDec(SDK_ICC_CARDTYPE cardType, const void *extData,
                                      u32 *remainCheckTime, s32 reserve) {
    //s32 len = 0;
    s32 rslt = 0;

    //u32 cardno;
    //u8 aswData[64];

    if (remainCheckTime == NULL) {
        return SDK_PARA_ERR;
    }
    Trace("zjp", "cardType = %d\r\n", cardType);

    *remainCheckTime = 0;

    switch (cardType) {
        case SDK_ICC_SLE4428:                          //MEMORY CARD SLE4428           //sjl20120724
        {
            rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_READEC, 0, (u32) remainCheckTime);
            Trace("zjp", "rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_SLE4442:                            //MEMORY CARD SLE4442
        {
            *remainCheckTime = 0xff;
            rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_READEC, 0, (u32) remainCheckTime);
            Trace("zjp", "rslt = %d\r\n", rslt);

            switch (*remainCheckTime) {
                case 0: {
                    *remainCheckTime = 0x00;
                    break;
                }

                case 1: {
                    *remainCheckTime = 0x01;
                    break;
                }

                case 2: {
                    *remainCheckTime = 0x03;
                    break;
                }

                case 3: {
                    *remainCheckTime = 0x07;
                    break;
                }

                default: {
                    rslt = DDI_ERR;
                    break;
                }
            }

            break;
        }

        case SDK_ICC_AT24C01:                            //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C02:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C04:                          //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C08:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C16:                            //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C32:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C64:                           //MEMORY CARD AT24Cxx
        { //AT24Cϵ������У������
            *remainCheckTime = 0xffffffff;
            rslt = DDI_OK;
            break;
        }

        case SDK_ICC_1604:                               //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
        {
            strIs23SC1604ReadEC *pSC1604ReadEC;

            if (extData == NULL) {
                return SDK_PARA_ERR;
            }
            else {
                pSC1604ReadEC = (strIs23SC1604ReadEC *) extData;
            }
            rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_READEC, (u32) pSC1604ReadEC,
                                     (u32) remainCheckTime);
            break;
        }

        case SDK_ICC_AT88SC153:                         //MEMORY CARD AT88SC153
        case SDK_ICC_AT88SC1608:                         //MEMORY CARD AT88SC1608/
        {
            strAT88SCxxReadEC *pAT88SCReadEC;

            if (extData == NULL) {
                return SDK_PARA_ERR;
            }
            else {
                pAT88SCReadEC = (strAT88SCxxReadEC *) extData;
            }
            rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_READEC, (u32)(pAT88SCReadEC),
                                     (u32) remainCheckTime);
            break;
        }

        default: {
            *remainCheckTime = 0;
            rslt = DDI_ERR;
            break;
        }
    }

    Trace("icc", "sdk_dev_iccard_mem_check_pwd :rslt = %d remainCheckTime =%d\r\n", rslt,
          *remainCheckTime);

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡmem������
** Parameters:          SDK_ICC_CARDTYPE cardType������
                    u32 addr��ʼ��ַ
                    u32 usReadLen����
                    u8* rbuf���ض�ȡ�����ݻ�����
** Returned value:
** Created By:		���ƽ  2014.06.20
** Remarks:
*****************************************************************************/
static s32 sdk_dev_iccard_mem_read(SDK_ICC_CARDTYPE cardType, u32 addr, u32 sLen, u8 *rbuf) {
    //s32 len = 0;
    s32 rslt = DDI_ERR;

    //u32 cardno;
    //u8 aswData[64];

    Trace("zjp", "cardType = %d\r\n", cardType);
    TraceHex("zjp", "rbuf", rbuf, 16);
    Trace("zjp", "addr = %d\r\n", addr);
    Trace("zjp", "sLen = %d\r\n", sLen);

    switch (cardType) {
        case SDK_ICC_SLE4428:                          //MEMORY CARD SLE4428           //sjl20120724
        {
            struct _strSle4428 {
                u32 m_protect; //����λ��ʽ 0:��������λ�� 1:������λ
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } Sle4428AddrInfo;

            if (addr + sLen > CARD_4428_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_4428_MAX_LEN) {
                    sLen = CARD_4428_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            Sle4428AddrInfo.m_protect = 0; //����λ��ʽ 0:��������λ�� 1:������λ
            Sle4428AddrInfo.m_addr = addr; //��ʼ��ַ
            Sle4428AddrInfo.m_le = sLen; //���ݳ���

            rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_READ, (u32)(&Sle4428AddrInfo),
                                     (u32)(rbuf + 1));
            Trace("zjp", "DDI_SLE4428_CTL_READ :rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_SLE4442:                           //MEMORY CARD SLE4442
        {
            struct _strSle4442 {
                u32 m_area; //�洢����0�����洢����1��������
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } SLE4442AddrInfo;

            if (addr + sLen > CARD_4442_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_4442_MAX_LEN) {
                    sLen = CARD_4442_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            SLE4442AddrInfo.m_area = 0; //�洢����
            SLE4442AddrInfo.m_addr = addr; //��ʼ��ַ
            SLE4442AddrInfo.m_le = sLen; //���ݳ���
            Trace("zjp", "SLE4442AddrInfo.m_addr = %d\r\n", SLE4442AddrInfo.m_addr);
            Trace("zjp", "SLE4442AddrInfo.m_le = %d\r\n", SLE4442AddrInfo.m_le);
            rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_READ, (u32)(&SLE4442AddrInfo),
                                     (u32)(rbuf + 1));
            Trace("zjp", "DDI_SLE4442_CTL_READ :rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_AT24C01:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C02:                          //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C04:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C08:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C16:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C32:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C64:                            //MEMORY CARD AT24Cxx
        {
            const u32 at24CXX_max_len[] = {
                    CARD_AT24C01_MAX_LEN,
                    CARD_AT24C02_MAX_LEN,
                    CARD_AT24C04_MAX_LEN,
                    CARD_AT24C08_MAX_LEN,
                    CARD_AT24C16_MAX_LEN,
                    CARD_AT24C32_MAX_LEN,
                    CARD_AT24C64_MAX_LEN
            };

            struct _strAT24Cxx {
                u32 m_type; //AT24Cϵ�п�Ƭ����
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } AT24CAddrInfo;

            AT24CAddrInfo.m_type =
                    cardType - SDK_ICC_AT24C01; //�����ͱ�Ŵ�0~6,�ֱ��ӦSDK_ICC_AT24C01~SDK_ICC_AT24C64

            if (addr + sLen > at24CXX_max_len[AT24CAddrInfo.m_type]) //��ȡ�����ݳ�����
            {
                if (addr < at24CXX_max_len[AT24CAddrInfo.m_type]) {
                    sLen = at24CXX_max_len[AT24CAddrInfo.m_type] - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            AT24CAddrInfo.m_addr = addr; //��ʼ��ַ
            AT24CAddrInfo.m_le = sLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_AT24CXX_CTL_READ, (u32)(&AT24CAddrInfo), (u32)(rbuf + 1));
            Trace("zjp", "DDI_AT24CXX_CTL_READ :rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_1604:                              //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
        {
            struct _strIs23sc1604 {
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } sc1604AddrInfo;

            if (addr + sLen > CARD_1604_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_1604_MAX_LEN) {
                    sLen = CARD_1604_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            sc1604AddrInfo.m_addr = addr; //��ʼ��ַ
            sc1604AddrInfo.m_le = sLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_READ, (u32)(&sc1604AddrInfo),
                                     (u32)(rbuf + 1));
            Trace("zjp", "DDI_IS23SC102_1604_CTL_READ :rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_AT88SC153:                         //MEMORY CARD AT88SC153
        case SDK_ICC_AT88SC1608:                        //MEMORY CARD AT88SC1608/
        {
            const u32 AT88SC_max_len[] = {
                    CARD_AT88SC153_MAX_LEN,
                    CARD_AT88SC1608_MAX_LEN,
            };
            struct _strAT88SCxx {
                u32 m_type; //��Ƭ����
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } AT88SCAddrInfo;


            AT88SCAddrInfo.m_type =
                    cardType - SDK_ICC_AT88SC153 + 1; //�����ͱ�Ŵ�0,1,2,�ֱ��Ӧ101, 153, 1608

            if (addr + sLen > AT88SC_max_len[AT88SCAddrInfo.m_type]) //��ȡ�����ݳ�����
            {
                if (addr < AT88SC_max_len[AT88SCAddrInfo.m_type]) {
                    sLen = AT88SC_max_len[AT88SCAddrInfo.m_type] - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            AT88SCAddrInfo.m_addr = addr; //��ʼ��ַ
            AT88SCAddrInfo.m_le = sLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_READ, (u32)(&AT88SCAddrInfo),
                                     (u32)(rbuf + 1));
            Trace("zjp", "DDI_AT88SC153_1608_CTL_READ :rslt = %d\r\n", rslt);
            break;
        }

        default: {
            rslt = DDI_ERR;
            break;
        }
    }

    Trace("zjp", "sdk_dev_iccard_mem_read :rslt = %d\r\n", rslt);

    if (DDI_OK == rslt) {
        rbuf[0] = 0;
        Trace("zjp", "read mem card data", rbuf, sLen);
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	дmem������
** Parameters:          SDK_ICC_CARDTYPE cardType������
                    u32 addr��ʼ��ַ
                    u32 usReadLen����
                    u8* sbufд�����ݻ�����
** Returned value:
** Created By:		���ƽ  2014.06.20
** Remarks:
*****************************************************************************/
static s32 sdk_dev_iccard_mem_write(SDK_ICC_CARDTYPE cardType, u32 addr, u32 rLen, u8 *sbuf) {
    //s32 len = 0;
    s32 rslt = 0;

    //u32 cardno;
    //u8 aswData[64];

    Trace("zjp", "cardType = %d\r\n", cardType);
    TraceHex("zjp", "sbuf", sbuf, 16);
    Trace("zjp", "addr = %d\r\n", addr);
    Trace("zjp", "rLen = %d\r\n", rLen);


    switch (cardType) {
        case SDK_ICC_SLE4428:                          //MEMORY CARD SLE4428           //sjl20120724
        {
            struct _strSle4428 {
                u32 m_protect; //����λ��ʽ 0:��������λ�� 1:������λ
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } Sle4428AddrInfo;

            if (addr + rLen > CARD_4428_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_4428_MAX_LEN) {
                    rLen = CARD_4428_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            Sle4428AddrInfo.m_protect = 0; //����λ��ʽ 0:��������λ�� 1:������λ
            Sle4428AddrInfo.m_addr = addr; //��ʼ��ַ
            Sle4428AddrInfo.m_le = rLen; //���ݳ���

            rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_WRITE, (u32)(&Sle4428AddrInfo), (u32) sbuf);
            Trace("zjp", "rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_SLE4442:                           //MEMORY CARD SLE4442
        {
            struct _strSle4442 {
                u32 m_area; //�洢����0�����洢����1��������
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } Sle4442AddrInfo;

            if (addr + rLen > CARD_4442_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_4442_MAX_LEN) {
                    rLen = CARD_4442_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            Sle4442AddrInfo.m_area = 0;
            Sle4442AddrInfo.m_addr = addr; //��ʼ��ַ
            Sle4442AddrInfo.m_le = rLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_WRITE, (u32)(&Sle4442AddrInfo), (u32) sbuf);
            Trace("zjp", "rslt = %d\r\n", rslt);
            break;
        }

        case SDK_ICC_AT24C01:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C02:                          //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C04:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C08:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C16:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C32:                           //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C64:                            //MEMORY CARD AT24Cxx
        {
            const u32 at24CXX_max_len[] = {
                    CARD_AT24C01_MAX_LEN,
                    CARD_AT24C02_MAX_LEN,
                    CARD_AT24C04_MAX_LEN,
                    CARD_AT24C08_MAX_LEN,
                    CARD_AT24C16_MAX_LEN,
                    CARD_AT24C32_MAX_LEN,
                    CARD_AT24C64_MAX_LEN
            };

            struct _strAT24Cxx {
                u32 m_type; //AT24Cϵ�п�Ƭ����
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } AT24CAddrInfo;

            AT24CAddrInfo.m_type =
                    cardType - SDK_ICC_AT24C01; //�����ͱ�Ŵ�0~6,�ֱ��ӦSDK_ICC_AT24C01~SDK_ICC_AT24C64

            if (addr + rLen > at24CXX_max_len[AT24CAddrInfo.m_type]) //��ȡ�����ݳ�����
            {
                if (addr < at24CXX_max_len[AT24CAddrInfo.m_type]) {
                    rLen = at24CXX_max_len[AT24CAddrInfo.m_type] - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            AT24CAddrInfo.m_addr = addr; //��ʼ��ַ
            AT24CAddrInfo.m_le = rLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_AT24CXX_CTL_WRITE, (u32)(&AT24CAddrInfo), (u32) sbuf);
            break;
        }

        case SDK_ICC_1604:                              //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
        {
            struct _strIs23sc1604 {
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } sc1604AddrInfo;

            if (addr + rLen > CARD_1604_MAX_LEN) //��ȡ�����ݳ�����
            {
                if (addr < CARD_1604_MAX_LEN) {
                    rLen = CARD_1604_MAX_LEN - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            sc1604AddrInfo.m_addr = addr; //��ʼ��ַ
            sc1604AddrInfo.m_le = rLen; //���ݳ���
            rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_WRITE, (u32)(&sc1604AddrInfo), (u32) sbuf);
            break;
        }

        case SDK_ICC_AT88SC153:                         //MEMORY CARD AT88SC153
        case SDK_ICC_AT88SC1608:                        //MEMORY CARD AT88SC1608/
        {
            const u32 AT88SC_max_len[] = {
                    CARD_AT88SC153_MAX_LEN,
                    CARD_AT88SC1608_MAX_LEN,
            };
            struct _strAT88SCxx {
                u32 m_type; //��Ƭ����
                u32 m_addr; //��ʼ��ַ
                u32 m_le; //���ݳ���
            } AT88SCAddrInfo;


            AT88SCAddrInfo.m_type =
                    cardType - SDK_ICC_AT88SC153 + 1; //�����ͱ�Ŵ�0,1,2,�ֱ��Ӧ101, 153, 1608

            if (addr + rLen > AT88SC_max_len[cardType - SDK_ICC_AT88SC153]) //��ȡ�����ݳ�����
            {
                if (addr < AT88SC_max_len[cardType - SDK_ICC_AT88SC153]) {
                    rLen = AT88SC_max_len[cardType - SDK_ICC_AT88SC153] - addr;
                }
                else {
                    return SDK_PARA_ERR;
                }
            }
            AT88SCAddrInfo.m_addr = addr; //��ʼ��ַ
            AT88SCAddrInfo.m_le = rLen; //���ݳ���
            Trace("zjp", "AT88SCAddrInfo.m_type = %d\r\n", AT88SCAddrInfo.m_type);
            Trace("zjp", "AT88SCAddrInfo.m_addr = %d\r\n", AT88SCAddrInfo.m_addr);
            Trace("zjp", "AT88SCAddrInfo.m_le = %d\r\n", AT88SCAddrInfo.m_le);
            Trace("zjp", "sbuf", sbuf, rLen);
            rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_WRITE, (u32)(&AT88SCAddrInfo), (u32) sbuf);
            Trace("zjp", "DDI_AT88SC153_1608_CTL_WRITE rslt = %d\r\n", rslt);

            break;
        }

        default: {
            rslt = DDI_ERR;
            break;
        }
    }

    Trace("icc", "sdk_dev_iccard_mem_write :rslt = %d\r\n", rslt);

    if (DDI_OK == rslt) {
        Trace("icc", "read mem card data", sbuf, rLen);
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

typedef struct {
    s32 AppCmd;
    s32 DevCmd;
} CMD_CONVERT;

/*****************************************************************************
** Descriptions:	        �Ӵ�ʽIC�������
** Parameters:          const u8 *sbuf:���͸���������
                               s32 slen:�������ݳ���
                               u8 *rbuf:�����ص�����
                               s32 timeout:��ʱʱ��
** Returned value:  ���buf�ĳ���
** Created By:		���ٲ�  2013.07.22
** Remarks:
*****************************************************************************/
s32 sdk_dev_iccard_command(u8 ucSlotNo, const u8 *sbuf, s32 slen, u8 *rbuf, s32 timeout,
                           u8 cardtype) {
    s32 rslt = SDK_ERR;
    s32 len = 0;
    u16 size = 256;

    if (NULL == sbuf || NULL == rbuf || timeout < 0 || slen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    TraceHex("zjp", "sbuf", sbuf, slen);

//    if (cardtype == SDK_ICC_CPU) {
        rslt = ddi_iccpsam_exchange_apdu(ucSlotNo, sbuf, slen, rbuf, (u32*)&len, size);
        Trace("zjp", "rslt = %d len = %d", rslt,len);
        if(rslt == DDI_OK) {
            TraceHex("zjp", "rbuf", rbuf, len);
        }
        if (rslt == DDI_OK) {
            return len;
        }
        else if (rslt == DDI_ERR) {
            return SDK_ERR;
        }
        else if (rslt == DDI_ETIMEOUT) {
            return SDK_ERR;
        }
        else if (rslt == DDI_EINVAL) {
            return SDK_TIME_OUT;
        }
        else {
            return SDK_EIO;
        }
//    }
//    else //������ʱȫ������cpu�������ˡ�
//    {
//        rslt = ddi_iccpsam_exchange_apdu(ucSlotNo, sbuf, slen, rbuf, (u32 * ) & len, size);
//        Trace("zjp", "rslt = %d\r\n", rslt);
//        TraceHex("zjp", "rbuf", rbuf, len);
//        if(rslt == DDI_OK) {
//            TraceHex("zjp", "rbuf", rbuf, len);
//        }
//        if (rslt == DDI_OK) {
//            return len;
//        }
//        else if (rslt == DDI_ERR) {
//            return SDK_ERR;
//        }
//        else if (rslt == DDI_ETIMEOUT) {
//            return SDK_ERR;
//        }
//        else if (rslt == DDI_EINVAL) {
//            return SDK_TIME_OUT;
//        }
//        else {
//            return SDK_EIO;
//        }
//    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:	s32 sdkIccRFQuery(u8 ucCardType)
   ��������:	�ǽ�Ѱ��
   �������:u8 eCardType:������,A��,B��,2.4G��
                           s32 siTimerCnt:��ʱʱ��(����)
   �������:u8*pheOutBuf:����Ѱ����Ϣ
   ��   ��  ֵ:Ѱ���ɹ��򷵻ػ�ȡ��Ѱ����Ϣ����
                          Ѱ����ʱ�򷵻�SDK_TIME_OUT
                          Ѱ��ʱȡ���򷵻�SDK_ESC
                          �������󷵻�SDK_PARA_ERR
   �޸ı�ע:��֧��CPU��������Ѱ����Ϣ�ṹΪ:
   ��һ�ֽ�0��ʾѰ���ɹ���2��ʾʧ�ܣ�
   �ڶ��ֽڹ̶�Ϊ����00��
   �����ֽ�'A'��ʾѰ������type A����'B'��ʾѰ������typb B����
   �����ֽڵ������ֽ�Ϊ���Ŀ���UID/PUPI�ţ������ţ�
   ���һ�ֽ�ΪSAK��	���ֽڽ���CardtypeΪ"A"ʱ���ڡ�
   ����ʱ��:2012.07.23 15:16:52
*******************************************************************/
s32 sdkIccRFQuery(u8 ucCardType, u8 *pheOutBuf, s32 siTimerCnt) {
    s32 key = 0;
    u32 timerid;
    s32 ret;
    s32 len = 0;

    if (NULL == pheOutBuf || siTimerCnt < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    //zcl 20141101
    if (gucSdkIccRfCheckBattle == 0) {
        gucSdkIccRfCheckBattle = 1;

        if (SDK_OK != sdkRfBatteryEnough()) {
            Assert(0);
            return SDK_ICC_BAT_VOLTAGE_LOW;
        }
        //end
    }
    ucCardType = ucCardType;

#if 0 /*Modify by ���ƽ at 2015.01.20  11:24 */

    if(ucCardType & SDK_ICC_RFCARD_24G) //sujianzhong 2013.08.15 9:16
    { //���� 2.4G��
        if(0 == sdkIccJudgeHave24GModule())
        {
            Trace("IC", "û��2.4Gģ��!\r\n");
            ucCardType &= (~SDK_ICC_RFCARD_24G);
        }
    }
#endif /* if 0 */

    if (0 == (ucCardType &
              (SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G))) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = ddi_rf_poweron(ucCardType & (SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B |
                                       SDK_ICC_RFCARD_FELICA)); //�ײ�ֻ֧��3�ֿ�
    Trace("rf", "ddi_rf_poweron:ret = %d, ucCardType = %d\r\n\r\n", ret, ucCardType);
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */

    if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
    {
        return RF_INTERFRENCE_MAG;
    }
    else
#endif /* if 0 */

    if (DDI_OK != ret) //�ϵ�ʧ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    timerid = sdkTimerGetId();

    pheOutBuf[len++] = 0; //Ѱ���ɹ�
    pheOutBuf[len++] = 0; //Ĭ��Ϊ0

    while (1) {
        if (siTimerCnt >= 1000)   //�����ʱʱ�����һ�����Ӧ����//shijianglong 2013.05.10 14:53
        {
            key = sdkKbGetKey(); //shijianglong 2012.12.27 14:48�޸�ʹ�÷�ʽ

            if (key == SDK_KEY_ESC) {
                return SDK_ESC;
            }
        }

        switch (ddi_rf_get_status()) {
            case 1: //û��Ѱ����
            {
                break;
            }

            case 2: //���ſ�
            {
                return SDK_ICC_MORECARD;
            }

            case 3: //����Type A CPU
            case 5: //����Type A MEM
            {
                ret = ddi_rf_ioctl(DDI_RF_CTL_UID, 0, (u32)(&pheOutBuf[len]));

                if (DDI_OK == ret) //������
                {
                    if (pheOutBuf[len] != 4) //��4�ֽ���ȫ������
                    {
                        pheOutBuf[len++] = 'A';
                        return len;
                    }
                    else //ֻ����4�ֽڿ���
                    {
                        pheOutBuf[len++] = 'A';
                        len += 4;
                    }
                }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
                    else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
                    {
                        return RF_INTERFRENCE_MAG;
                    }
#endif /* if 0 */
                else {
                    pheOutBuf[len++] = 'A';
                    return len;
                }
                ret = ddi_rf_ioctl(DDI_RF_CTL_SAK, 0, (u32)(&pheOutBuf[len]));

                if (DDI_OK == ret) {
                    len++;
                }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
                else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
                {
                    return RF_INTERFRENCE_MAG;
                }
#endif /* if 0 */
                return len;
            }

            case 4: //����Type B CPU
            case 6: //����Type B MEM
            {
                ret = ddi_rf_ioctl(DDI_RF_CTL_UID, 0, (u32)(&pheOutBuf[len]));

                if (DDI_OK == ret) //������
                {
                    if (pheOutBuf[len] != 4) //��4�ֽ���ȫ������
                    {
                        pheOutBuf[len++] = 'B';
                    }
                    else //ֻ����4�ֽڿ���
                    {
                        pheOutBuf[len++] = 'B';
                        len += 4;
                    }
                }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
                    else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
                    {
                        return RF_INTERFRENCE_MAG;
                    }
#endif /* if 0 */
                else {
                    pheOutBuf[len++] = 'B';
                }
                return len;
            }

            case 7: //����FeliCa��
            {
                ret = ddi_rf_ioctl(DDI_RF_CTL_UID, 0, (u32)(&pheOutBuf[len]));

                if (DDI_OK == ret) //������
                {
                    if (pheOutBuf[len] != 4) //��4�ֽ���ȫ������
                    {
                        pheOutBuf[len++] = 'F';
                    }
                    else //ֻ����4�ֽڿ���
                    {
                        pheOutBuf[len++] = 'F';
                        len += 4;
                    }
                }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
                    else if( DDI_READED_MAGCARD == ret ) //�ǽӹ��̲������̶����ſ�����
                    {
                        return RF_INTERFRENCE_MAG;
                    }
#endif /* if 0 */
                else {
                    pheOutBuf[len++] = 'F';
                }
                return len;
            }

#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
                case DDI_READED_MAGCARD:
                   {
                       return RF_INTERFRENCE_MAG;
                   }
                  //break;
#endif /* if 0 */

            default: {
                Assert(0);
                return SDK_ICC_NOCARD;
            }
        }

        if (1 == sdkTimerIsEnd(timerid, (u32) siTimerCnt)) {
            Assert(0);
            return SDK_TIME_OUT;
        }
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	s32 sdkIccReset(const SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf , s32 *psiOutLen)
   ��������:	��Ƭ��λ
   �������:const SDK_ICC_PARAM *pstIccParam:ʵ�ʵĶ�����ʽ���������ͺͿ�����
   �������:u8 *pheOutBuf :�������
            s32 *psiOutLen:������ݳ���
   ��   ��  ֵ:SDK_OK	�ɹ�
               SDK_ERR	ʧ��
               SDK_PARA_ERR	��������
               SDK_TIME_OUT	��ʱ
   �޸ı�ע:
   ����ʱ��:2012.07.24 14:38:01
*******************************************************************/
s32 sdkIccReset(const SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf, s32 *psiOutLen) {
    s32 rslt = 0;
    u8 cardno = 0;

    if ((pstIccParam == NULL) || (pheOutBuf == NULL) || (psiOutLen == NULL)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *psiOutLen = 0;
    Trace("emv", "ResetIcc   GO->Begin\r\n");

    if (SDK_ICC_ICC == pstIccParam->ucCardMode)              //�Ӵ�
    {
        cardno = pstIccParam->ucSlotNo; //psam����Ҫ�����洫������
        Trace("emv", "ResetIcc   GO-> SDK_ICC_ICC SDK_ICC_SLE4428 = %d eCardType = %d\r\n",
              SDK_ICC_SLE4428, pstIccParam->eCardType);
        rslt = sdk_dev_iccard_reset(pstIccParam->eCardType, &cardno, pheOutBuf, SDK_ICC_TIMER_AUTO);
        Trace("emv", "*psiOutLen = %d \r\n", *psiOutLen);

        if (rslt > 0) {
            *psiOutLen = rslt;
            return SDK_OK;
        }
        else {
            return rslt;
        }
    }
    else if (SDK_ICC_RF == pstIccParam->ucCardMode)         //�ǽ�
    {
        rslt = ddi_rf_activate();

        if (DDI_OK == rslt) {
            *psiOutLen = 0;
            return SDK_OK;
        }
#if RF_INTFNC_MAG /*����Ƿ��Ǹ����˴ſ�ˢ�� Modify by ���ƽ at 2015.04.13  10:10 */
            else if( DDI_READED_MAGCARD == rslt ) //�ǽӹ��̲������̶����ſ�����
            {
                return RF_INTERFRENCE_MAG;
            }
#endif /* if 0 */
        else {
            return SDK_ERR;
        }
    }
    return rslt;
}

/*****************************************************************************
   ��          Ȩ: �¹���
   ��������: sdkIccPowerDown
   ��������:�������µ�
   ��          ��: Input: const SDK_ICC_PARAM *pstIccParam :ʵ�ʵĶ�����ʽ���������ͺͿ�����
   ��  ��   ֵ:    SDK_OK	�ɹ�
                         SDK_ERR	ʧ��
                         SDK_PARA_ERR	��������
                         SDK_TIME_OUT	��ʱ

   ˵          ��:

   ��  ��   ��:sjl2012.07.26
   ע          ��:
*****************************************************************************/
s32 sdkIccMemMifarePowerDown(const SDK_ICC_PARAM *pstIccParam) {
    s32 ret;

    if (pstIccParam == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Trace("emv", "Powerdown   GO->Begin\r\n");

    if (SDK_ICC_ICC == pstIccParam->ucCardMode)               //�Ӵ�
    {
        u8 statee = 0;

        if (pstIccParam->eCardType >= SDK_ICC_MEMORY &&
            pstIccParam->eCardType <= SDK_ICC_AT88SC1608) //��mem��
        {
            statee = ddi_iccpsam_ioctl(DDI_MEMORY_CTL_POWEROFF, 0, 0);
        }
        else {
            statee = ddi_iccpsam_poweroff(0);
        }

        if (statee == DDI_OK) {
            return SDK_OK;
        }
        return SDK_ERR;
    }
    else if (SDK_ICC_RF == pstIccParam->ucCardMode)         //�ǽ�
    {
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
    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:	sdkIccSendData
   ��������:	����IC������
   �������:
                    pstIccParam:��Ҫ�ȸ�pIccParam->CardMode,pstIccParam->CardType��pIccParam->SlotNo����������ֵ,Ȼ����Ϊ�������
                    pheInBuf:��������,
                    siInLen:���ͳ���,
   �������:
                    pheOutBuf:��������(Memory/Mifare����Ӧ��ԭ������)
                    psiOutLen:�������ݵĳ���
   ��   ��  ֵ:
                    SDK_OK ���ճɹ�
                    SDK_TIME_OUT ���ճ�ʱ
                    SDK_ERR ���մ���
                    SDK_PARA_ERR ��������
   �޸ı�ע:
   ����ʱ��:2012.07.12 09:28:14
   �˽ӿڲ��ص������ԣ���Ĳ��Խӿ��Ѿ������ýӿ�
*******************************************************************/
s32 sdkIccSendData(const SDK_ICC_PARAM *pstIccParam, const u8 *pheInBuf, s32 siInLen, u8 *pheOutBuf,
                   s32 *psiOutLen) {
    s32 rslt = 0;

    if ((NULL == pstIccParam) || (NULL == pheInBuf) || (NULL == pheOutBuf) || (NULL == psiOutLen) ||
        siInLen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *psiOutLen = 0;

    if (pstIccParam->ucCardMode == SDK_ICC_RF)             //�ǽ�
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
    else if (pstIccParam->ucCardMode == SDK_ICC_ICC)       //�Ӵ�
    {
        if (siInLen < 1) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        rslt = sdk_dev_iccard_command(pheInBuf[0], pheInBuf + 1, siInLen - 1, pheOutBuf,
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

/*****************************************************************************
** Descriptions:
** Parameters:     const SDK_ICC_PARAM *pstIccParam
                   const SDK_ICC_APDU_SEND *pstApduSend
                   SDK_ICC_APDU_RESP *pstApduResp
** Returned value:
** Created By:     sujianzhong20130416
** Remarks:
*****************************************************************************/
s32 sdkIccSendAPDU(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_APDU_SEND *pstApduSend,
                   SDK_ICC_APDU_RESP *pstApduResp) {
    u8 iccommand[300];
    s32 len, sendLc, sendLe, OutLen = 0, templen = 0;
    s32 rslt;

    if ((NULL == pstIccParam) || (NULL == pstApduSend) || (NULL == pstApduResp)) {
        return SDK_PARA_ERR;
    }
    memset(iccommand, 0, sizeof(iccommand));

    len = 0;

    if (pstIccParam->ucCardMode == SDK_ICC_ICC) //�Ӵ�IC����Ҫ��������
    {
        iccommand[len++] = pstIccParam->ucSlotNo; //���ֽڷſ�����
    }
    memcpy(&iccommand[len], pstApduSend->Command, 4);
    len = len + 4;
    sendLc = pstApduSend->Lc & 0x00ff;

    if (sendLc > 0)                     // �з�������
    {
        iccommand[len++] = (u8) sendLc;

#if 0 /*Modify by ���ƽ at 2015.01.23  10:27 */

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

    sdkKbKeyFlush();

    while (1) {
        rslt = sdkIccSendData(pstIccParam, iccommand, len, pstApduResp->DataOut, &OutLen);

        if (rslt != SDK_OK) {
            Trace("emv", "EMVRx Timeout rslt = %x\r\n", rslt);

            pstApduResp->ReadCardDataOk = COMMANDERR;
            return rslt;
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

/*****************************************************************************
** Descriptions:   ���ǽӿ�Ƭ�Ƿ��ƿ�
** Parameters:     void
** Returned value: 1:�����ƿ� 0:��δ�ƿ�
** Created By:     sujianzhong20130416
** Remarks:         Ĭ�ϼ��֮ǰ��Ƭ�Ѿ��ڸ�Ӧ����
*****************************************************************************/
bool sdkIccDetectMove(void) {
    s32 rslt = 0;

    Trace("emv", "goto->sdkIccDetectCLCardOut\r\n");

    rslt = ddi_rf_get_status();

    if (rslt <= 1) {
        return true;
    }
    else {
        return false;
    }
}

/*=================================================
   memorycard
   ==================================================*/

/*****************************************************************************
   ��          Ȩ: �¹���
   ��������: sdkIccMemCheck
   ��������:��ѯMemory��������������Memory��
   ��          ��: Input: ��
                      Output: u8 *pheOutBuf ��ѯ���ص�������Ϣ
                     s32 psiOutLen	��ѯ���ص����ݳ���
   ��  ��   ֵ: SDK_OK	�ɹ�
                    SDK_ERR     ʧ��
                    SDK_TIME_OUT	��ʱ
                    SDK_PARA_ERR	��������
   ˵          ��:

   ��  ��   ��:sjl2012.07.27
   ע          ��:
*****************************************************************************/
s32 sdkIccMemCheck(u8 *pheOutBuf, s32 *psiOutLen) {
    //u8 temp[32];
    //u16 len = 0;
    //s32 rslt = 0;
    //SDK_ICC_PARAM stIccParam;

    if (NULL == pheOutBuf || NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    pheOutBuf[0] = 0x1;
    *psiOutLen = 1;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	        Memory��������
** Parameters:   Input: const SDK_ICC_PARAM *pstIccParam
                               const SDK_ICC_RMEMIN *pstRMemInData
                         Output: u8 *pheOutBuf
                                    s32 *psiOutLen
** Returned value:	SDK_OK	�ɹ�
                                SDK_ERR         ʧ��
                                SDK_TIME_OUT	��ʱ
                                SDK_PARA_ERR	��������
** Created By:		sjl 2012.07.27
** Remarks:����������memory��
*****************************************************************************/
s32 sdkIccMemReadData(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_RMEMIN *pstRMemInData,
                      u8 *pheOutBuf, s32 *psiOutLen) {
    if (NULL == pstIccParam || NULL == pstRMemInData || NULL == pheOutBuf ||
        NULL == psiOutLen) //shijianglong 2013.01.31 13:59
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdk_dev_iccard_mem_read(pstIccParam->eCardType, pstRMemInData->ucAddress,
                                   pstRMemInData->ucReadLen, pheOutBuf);
}

/*****************************************************************************
** Descriptions:	    Memory��д����
** Parameters:          SDK_ICC_PARAM *pstIccParam
                        const SDK_ICC_WMEMIN *pstWMemInData     д������
                        u8 *pheOutBuf   �������
                        s32 *psiOutLen  ������ݳ���
** Returned value:	SDK_OK	�ɹ�
                                SDK_ERR         ʧ��
                                SDK_TIME_OUT	��ʱ
                                SDK_PARA_ERR	��������
** Created By:		sjl 2012.07.31
** Remarks:
*****************************************************************************/
s32 sdkIccMemWriteData(const SDK_ICC_PARAM *pstIccParam, const SDK_ICC_WMEMIN *pstWMemInData,
                       u8 *pheOutBuf, s32 *psiOutLen) {
    s32 rslt = 0;

    if (NULL == pstIccParam || NULL == pstWMemInData || NULL == pheOutBuf || NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rslt = sdk_dev_iccard_mem_write(pstIccParam->eCardType, pstWMemInData->ucAddress,
                                    pstWMemInData->ucWriteLen, pstWMemInData->ucWriteData);
    Trace("zjp", "rslt = %d\r\n", rslt);

    if (rslt == SDK_OK) {
        pheOutBuf[0] = 0; //д��ɹ���־
        *psiOutLen = 1;
        return SDK_OK;    //����д�����ݵĳ���
    }
    else {
        return rslt;
    }
}

/*****************************************************************************
** Descriptions:	        Memory��У������
** Parameters:           SDK_ICC_PARAM *pstIccParam
                               const u8 *phePwd
                               u8 *pheOutBuf
                               s32 *psiOutLen
** Returned value:SDK_OK	�ɹ�
                          SDK_ERR         ʧ��
                          SDK_TIME_OUT	��ʱ
                          SDK_PARA_ERR	��������
** Created By:		sjl 2012.08.06
** Remarks:��Ӧ��4442��4428��
*****************************************************************************/
s32 sdkIccMemCheckPwd(const SDK_ICC_PARAM *pstIccParam, const u8 *phePwd, u8 *pheOutBuf,
                      s32 *psiOutLen) {
    u8 pwd[32];
    u32 remainCheckTime;
    strAT88SCxxVerify AT88SCxxVerify;
    s32 rslt = 0;

    if (NULL == pstIccParam || NULL == phePwd || NULL == pheOutBuf || NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(pwd, 0, sizeof(pwd));
    memset((void *) &AT88SCxxVerify, 0, sizeof(AT88SCxxVerify));
    switch (pstIccParam->eCardType) {
        case SDK_ICC_SLE4428:                                //MEMORY CARD SLE4428           //sjl20120724
        {
            sdkAscToBcd(pwd, phePwd, 4); //��4���ֽ��Ժ�������(����)
            break;
        }

        case SDK_ICC_SLE4442:                                //MEMORY CARD SLE4442
        {
            sdkAscToBcd(pwd, phePwd, 6); //��4���ֽ��Ժ�������(����)
            break;
        }

        case SDK_ICC_1604:                                     //MEMORY CARD AT88SC1604/IS23SC1604/AT88SC101/102
        {
            sdkAscToBcd(pwd, phePwd, 8); //��4���ֽ��Ժ�������(����)
            break;
        }

        case SDK_ICC_AT88SC153:                              //MEMORY CARD AT88SC153
        case SDK_ICC_AT88SC1608:                             //MEMORY CARD AT88SC1608/
        {
            sdkAscToBcd(pwd, phePwd, 6); //��4���ֽ��Ժ�������(����)
            break;
        }

        case SDK_ICC_AT24C01:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C02:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C04:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C08:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C16:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C32:                                //MEMORY CARD AT24Cxx
        case SDK_ICC_AT24C64:                                //MEMORY CARD AT24Cxx
        default: //����Ĭ�����볤��Ϊ6λ BCD��Ϊ3λ
        {
            sdkAscToBcd(pwd, phePwd, 6);
            break;
        }
    }

    rslt = sdk_dev_iccard_mem_check_pwd(pstIccParam->eCardType, pwd, (u8 * ) & AT88SCxxVerify,
                                        &remainCheckTime);

    if (rslt == SDK_OK) {
        pheOutBuf[0] = 0;
        *psiOutLen = 1;
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:	    Memory����������
** Parameters:          const SDK_ICC_PARAM *pstIccParam
                               const u8 *pheOldPwd
                               const u8 *pheNewPwd
                               u8 *pheOutBuf
                               s32 *psiOutLen
** Returned value:SDK_OK	�ɹ�
                          SDK_ERR         ʧ��
                          SDK_TIME_OUT	��ʱ
                          SDK_PARA_ERR	��������
** Created By:		sjl2012.08.10
** Remarks:
*****************************************************************************/
s32 sdkIccMemModifyPwd(const SDK_ICC_PARAM *pstIccParam, const u8 *pheOldPwd, const u8 *pheNewPwd,
                       u8 *pheOutBuf, s32 *psiOutLen) {
    u8 oldPwd[16];
    u8 newPwd[16];
    s32 rslt = DDI_ERR;
    u32 checkTimes = 0;

    if (NULL == pstIccParam || NULL == pheOldPwd || NULL == pheNewPwd || NULL == pheOutBuf ||
        NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(oldPwd, 0, sizeof(oldPwd));
    memset(newPwd, 0, sizeof(newPwd));

    if (pstIccParam->eCardType == SDK_ICC_SLE4428) {
        sdkAscToBcd(oldPwd, pheOldPwd, 4);
        sdkAscToBcd(newPwd, pheNewPwd, 4);
        TraceHex("zjp", "oldPwd", oldPwd, 6);

        if (SDK_OK == sdk_dev_iccard_mem_check_pwd(SDK_ICC_SLE4428, oldPwd, NULL,
                                                   &checkTimes))  //�޸�����ǰ��У�������
        {
            TraceHex("zjp", "newPwd", newPwd, 6);
            rslt = ddi_iccpsam_ioctl(DDI_SLE4428_CTL_UPDATESC, (u32)(newPwd), (u32)(newPwd));
        }
    }
    else if (pstIccParam->eCardType == SDK_ICC_SLE4442) {
        sdkAscToBcd(oldPwd, pheOldPwd, 6);
        sdkAscToBcd(newPwd, pheNewPwd, 6);

        if (SDK_OK == sdk_dev_iccard_mem_check_pwd(SDK_ICC_SLE4428, oldPwd, NULL,
                                                   &checkTimes))  //�޸�����ǰ��У�������
        {
            rslt = ddi_iccpsam_ioctl(DDI_SLE4442_CTL_UPDATESC, 0, (u32)(newPwd));
            Trace("zjp", "rslt = %d\r\n", rslt);
        }
    }
    else if (pstIccParam->eCardType == SDK_ICC_AT88SC153 ||
             pstIccParam->eCardType == SDK_ICC_AT88SC1608
             || pstIccParam->eCardType ==
                SDK_ICC_AT88SC101) //SDK_ICC_AT88SC102, SDK_ICC_AT88SC1604��֧��
    {
        strAT88SCxxVerify AT88SCxxVerify;

        if (pstIccParam->eCardType == SDK_ICC_AT88SC101) {
            AT88SCxxVerify.m_type = 0;
        }
        else if (pstIccParam->eCardType == SDK_ICC_AT88SC153) {
            AT88SCxxVerify.m_type = 1;
        }
        else if (pstIccParam->eCardType == SDK_ICC_AT88SC1608) {
            AT88SCxxVerify.m_type = 2;
        }
        else {
            return SDK_ERR;
        }
        AT88SCxxVerify.m_mode = 1;
        memcpy(AT88SCxxVerify.m_key, oldPwd, 3);
        AT88SCxxVerify.m_index = 0;
        sdkAscToBcd(oldPwd, pheOldPwd, 6);

        if (SDK_OK ==
            sdk_dev_iccard_mem_check_pwd(pstIccParam->eCardType, oldPwd, (u8 * ) & AT88SCxxVerify,
                                         &checkTimes))  //�޸�����ǰ��У�������
        {
            sdkAscToBcd(newPwd, pheNewPwd, 6);
            memcpy(AT88SCxxVerify.m_key, newPwd, 3);
            rslt = ddi_iccpsam_ioctl(DDI_AT88SCXX_CTL_UPDATEEC, (u32) & AT88SCxxVerify,
                                     (u32)(newPwd));
            Trace("zjp", "rslt = %d\r\n", rslt);
        }
    }
    else if (pstIccParam->eCardType == SDK_ICC_1604) //����ָֻIS23SC1604
    {
        sdkAscToBcd(oldPwd, pheOldPwd, 4);
        sdkAscToBcd(newPwd, pheNewPwd, 4);

        if (SDK_OK == sdk_dev_iccard_mem_check_pwd(pstIccParam->eCardType, oldPwd, NULL,
                                                   &checkTimes))  //�޸�����ǰ��У�������
        {
            rslt = ddi_iccpsam_ioctl(DDI_IS23SC1604_CTL_UPDATEEC, 0, (u32)(newPwd));
            Trace("zjp", "rslt = %d\r\n", rslt);
        }
    }
    else //�������Ϳ������޸�����
    {
        Assert(0);
        return SDK_OK;
    }
    Trace("zjp", "rslt = %d\r\n", rslt);

    if (rslt == DDI_OK) {
        pheOutBuf[0] = 0; //�޸�����ɹ���־
        *psiOutLen = 1;
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:    Memory4442����������������Ϳ�����
** Parameters:          u8 *pheOutBuf
                               s32 *psiOutLen
** Returned value:
** Created By:		sjl2012.08.13
** Remarks:
*****************************************************************************/
s32 sdkIccMem4442ReadPwdAndPwdCount(u8 *pheOutBuf, s32 *psiOutLen) {
    u8 temp[32];
    //u16 templen = 0;
    SDK_ICC_PARAM stIccParam;
    s32 rslt = 0;

    if (NULL == pheOutBuf || NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(stIccParam));

    u32 remainCheckTime;
    s32 reserve = 0;

    rslt = sdk_dev_iccard_mem_readDec(SDK_ICC_SLE4442, NULL, &remainCheckTime, reserve);
    Trace("zjp", "rslt = %d\r\n", rslt);

    if (rslt == SDK_OK) {
        pheOutBuf[0] = 0; //��ȡУ������ʣ������ɹ���־
        pheOutBuf[1] = (u8) remainCheckTime;
        *psiOutLen = 2;
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:	        ��Mifare����
** Parameters:          u8 *pheDest
** Returned value:
** Created By:		shijianglong  2012.11.08
** Remarks:             ���п�����ʱ����ȷ���ؿ��ţ�
   ��������ʱ�ڳ�ʱʱ�������ڿ��ܻ᷵��0.
   ����޿������»�ȡ���ſ���Ѱ��������ȡ����
*****************************************************************************/
s32 sdkIccMifareReadCardNum(u8 *pheDest) {
    u8 uID[32] = {0};

    if (NULL == pheDest) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(uID, 0, sizeof(uID));

    if (DDI_OK == ddi_rf_ioctl(DDI_RF_CTL_UID, 0, (u32)(uID))) {
        TraceHex("mifare", "uID data", uID, uID[0] + 1);
        memcpy(pheDest, &uID[1], uID[0]);
        return uID[0];
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	        Mifare��KEY��֤
** Parameters:          u8 ucBlock
                               u8 ucKeyType
                               const u8 *pheKey
                               s32 siKeyLen
                               u8 *pheDest
                               s32 *psiDestLen
** Returned value:
** Created By:		shijianglong  2012.11.08
** Remarks:
*****************************************************************************/
s32 sdkIccMifareVerifyKey(u8 ucBlock, u8 ucKeyType, const u8 *pheKey, s32 siKeyLen, u8 *pheDest,
                          s32 *psiDestLen) {
    u8 temp[64];
    s32 templen = 0;
    s32 rslt = 0;
    SDK_ICC_PARAM stIccParam;

    if (NULL == pheDest || NULL == psiDestLen || NULL == pheKey || siKeyLen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));

    temp[templen++] = MIFARE_DB_CHECK_KEY;
    temp[templen++] = 0x00;
    temp[templen++] = ucBlock;
    temp[templen++] = ucKeyType;

    memcpy(&temp[templen], pheKey, siKeyLen);
    templen += siKeyLen;

    stIccParam.ucCardMode = SDK_ICC_RF;
    stIccParam.eCardType = SDK_ICC_MIFARE;
    stIccParam.ucSlotNo = (u8) sdkIcGetIcdealCardNo();
    TraceHex("mifare", "���������Ƕ���", temp, templen);
    rslt = sdkIccSendData(&stIccParam, temp, templen, pheDest, psiDestLen);

    return rslt;
}

/*****************************************************************************
** Descriptions:	        Mifare������
** Parameters:          u8 ucBlock
                               u8 *pheDest
                               s32 *psiDestLen
** Returned value:
** Created By:		shijianglong  2012.11.08
** Remarks:
*****************************************************************************/
s32 sdkIccMifareReadBlock(u8 ucBlock, u8 *pheDest, s32 *psiDestLen) {
    u8 temp[32];
    s32 rslt = 0;
    u16 templen = 0;
//    FIFO fifo;
    SDK_ICC_PARAM stIccParam;

    if (NULL == pheDest || NULL == psiDestLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));

    temp[templen++] = MIFARE_DB_READ_BLOCK;                                                        //Ext_cmd
    temp[templen++] = 0x00;                                                        //RFU
    temp[templen++] = ucBlock;                                                       //block NO.

    stIccParam.ucCardMode = SDK_ICC_RF;
    stIccParam.eCardType = SDK_ICC_MIFARE;
    stIccParam.ucSlotNo = (u8) sdkIcGetIcdealCardNo();
    rslt = sdkIccSendData(&stIccParam, temp, templen, pheDest, psiDestLen);

    return rslt;
}

/*****************************************************************************
** Descriptions:	        Mifare��д��
** Parameters:          u8 ucBlock
                               const u8 *pheScr
                               s32 siSrcLen
                               u8 *pheDest
                               s32 *pDestLen
** Returned value:
** Created By:		shijianglong  2012.11.08
** Remarks:
*****************************************************************************/
s32 sdkIccMifareWriteBlock(u8 ucBlock, const u8 *pheScr, s32 siSrcLen, u8 *pheDest,
                           s32 *psiDestLen) {
    u8 temp[32];
    s32 rslt = 0;
    u8 data[128];
    u16 templen = 0;
//    FIFO fifo;
    SDK_ICC_PARAM stIccParam;

    if (NULL == pheDest || NULL == psiDestLen || NULL == pheScr ||
        siSrcLen <= 0) //shijianglong 2013.01.31 14:2
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (siSrcLen > 16)           //�������ݳ��ȳ���16�������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(data, 0xff, sizeof(data));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));

    temp[templen++] = MIFARE_DB_WRITE_BLOCK;                                                        //Ext_cmd
    temp[templen++] = 0x00;                                                        //RFU
    temp[templen++] = ucBlock;                                                       //block NO.

    memcpy(data, pheScr, siSrcLen); //shijianglong 2013.02.28 16:44
    memcpy(temp + templen, data, 16);                                        //data(4 Bytes)
    templen += 16;
    stIccParam.ucCardMode = SDK_ICC_RF;
    stIccParam.eCardType = SDK_ICC_MIFARE;
    stIccParam.ucSlotNo = (u8) sdkIcGetIcdealCardNo();
    rslt = sdkIccSendData(&stIccParam, temp, templen, pheDest, psiDestLen);

    return rslt;
}

/*****************************************************************************
** Descriptions:	����mifare��(��ʽ������ֵ����ֵ��)
** Parameters:          u8 ucOptType
                               u8 ucBlock
                               const u8 *pheSrc
                               u8 *pheDest
                               s32 *psiDestLen
** Returned value:
** Created By:		shijianglong  2012.11.08
** Remarks:
*****************************************************************************/
s32 sdkIccMifareOperateBlock(u8 ucOptType, u8 ucBlock, const u8 *pheSrc, u8 *pheDest,
                             s32 *psiDestLen) {
    u8 temp[32];
    s32 rslt = 0;
    u16 templen = 0;
    SDK_ICC_PARAM stIccParam;

    if (NULL == pheSrc || NULL == pheDest || NULL == psiDestLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));


    temp[templen++] = MIFARE_DB_VALUE_BLOCK;                                                        //Ext_cmd
    temp[templen++] = 0x00;                                                        //RFU
    temp[templen++] = ucOptType;                              //option type
    temp[templen++] = ucBlock;                                                       //block NO.
    memcpy(temp + templen, pheSrc, 4);                                         //data(4 Bytes)
    templen += 4;

    stIccParam.ucCardMode = SDK_ICC_RF;
    stIccParam.eCardType = SDK_ICC_MIFARE;
    stIccParam.ucSlotNo = (u8) sdkIcGetIcdealCardNo();
    rslt = sdkIccSendData(&stIccParam, temp, templen, pheDest, psiDestLen);

    return rslt;
}

/*****************************************************************************
** Descriptions:	        Mifare���鱸��
** Parameters:          u8 ucS_block
                               u8 ucD_block
                               u8 *pheOutBuf
                               s32 *psiDestLen
** Returned value:
** Created By:		sjl  2012.08.17
** Remarks:
*****************************************************************************/
s32 sdkIccMifareBackUpBlock(u8 ucS_block, u8 ucD_block, u8 *pheDest, s32 *psiDestLen) {
    u8 temp[32];
    s32 rslt = 0;
    u16 templen = 0;
    SDK_ICC_PARAM stIccParam;

    if (NULL == pheDest || NULL == psiDestLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));

    temp[templen++] = MIFARE_DB_RECOVER_BLOCK;                                                        //Ext_cmd
    temp[templen++] = 0x00;                                                        //RFU
    temp[templen++] = ucS_block;                              //option type
    temp[templen++] = ucD_block;                                                       //block NO.

    stIccParam.ucCardMode = SDK_ICC_RF;
    stIccParam.eCardType = SDK_ICC_MIFARE;
    stIccParam.ucSlotNo = (u8) sdkIcGetIcdealCardNo();
    rslt = sdkIccSendData(&stIccParam, temp, templen, pheDest, psiDestLen);

    return rslt;
}

/*****************************************************************************
** Descriptions:	       ��ѯIC���Ƿ����
** Parameters:                 s32 siTimerCnt ��ʱʱ��
                               u8 *pheOutBuf    ��������
                               s32 *psiOutLen  �������ݳ���
** Returned value:
** Created By:		sjl  2012.08.17
** Remarks:
*****************************************************************************/
s32 sdkIccIsIccCardIn(s32 siTimerCnt, u8 *pheOutBuf, s32 *psiOutLen) {
    s32 rslt = SDK_ERR;
    u32 timeid;

    if (NULL == pheOutBuf || NULL == psiOutLen || siTimerCnt < 0) //shijianglong 2013.01.31 13:56
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    timeid = sdkTimerGetId();

    while (1) {
        if (ddi_iccpsam_get_status(0) > 1) {
            rslt = SDK_OK;
            break;
        }

        if (true == sdkTimerIsEnd(timeid, (u32) siTimerCnt)) {
            rslt = SDK_TIME_OUT;
            break;
        }
    }

    if (rslt > 0) {
        *psiOutLen = rslt;
        return SDK_OK;
    }
    else if (rslt == SDK_TIME_OUT) {
        return SDK_TIME_OUT;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:        ��ѯ�Ӵ�ʽIC�Ƿ��ڿ�����
** Parameters:          void
** Returned value:
** Created By:		sjl  2012.08.17
** Remarks:
*****************************************************************************/
s32 sdkIccIsInSlot(void) {
    s32 rslt;
    u32 timeid;

    timeid = sdkTimerGetId();

    while (1) {
        rslt = ddi_iccpsam_get_status(0); //��Ҫ���Ƽ��һ��ʱ�䷢��Ѱ��ָ��

        if (rslt > 1) //�п�
        {
            return SDK_OK;
        }

        if (true == sdkTimerIsEnd(timeid, 100)) //��ʱ100MS�˳�
        {
            rslt = SDK_ERR;
            break;
        }
    }

    return rslt;
}

/*****************************************************************************
** Descriptions:	��ѯIC���Ƿ�γ�
** Parameters:          s32 siTimerCnt
** Returned value:
** Created By:		����2012.10.24
** Remarks:
*****************************************************************************/
s32 sdkIccIsCardOut(s32 siTimerCnt) {
    u32 timeid;
    s32 rslt;

    if (siTimerCnt < 0) //shijianglong 2013.01.31 14:4
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    timeid = sdkTimerGetId();

    while (1) {
        if (1 == ddi_iccpsam_get_status(0)) //�޿�
        {
            rslt = SDK_OK;
            break;
        }

        if (true == sdkTimerIsEnd(timeid, (u32) siTimerCnt)) {
            rslt = SDK_TIME_OUT;
            break;
        }
    }

    return rslt;
}

/*****************************************************************************
   ��          Ȩ: �¹���
   ��������: sdkIccMemCardPowerOn
   ��������:Memory���ϵ�
   ��          ��: Input:const SDK_ICC_PARAM *pstIccParam ʵ�ʵĶ�����ʽ���������ͺͿ�����
                      Output:u8 *pheOutBuf      �ϵ緵�ص�������Ϣ
                                s32 psiOutLen	�ϵ緵�ص����ݳ���
   ��  ��   ֵ:SDK_OK	�ϵ�ɹ�
                        SDK_ERR	�ϵ�ʧ��
                        SDK_TIME_OUT	��ʱ
                        SDK_PARA_ERR	��������
   ˵          ��:Memory���г���AT24Cxx�������඼��Ҫ�ϵ硣
   ��  ��   ��:sjl2012.07.27
   ע          ��:
*****************************************************************************/
s32 sdkIccMemPowerOn(SDK_ICC_PARAM *pstIccParam, u8 *pheOutBuf, s32 *psiOutLen) {
    //u16 len = 0;
    //u8 temp[32];
    //s32 rslt = 0;

    if (NULL == pstIccParam || NULL == pheOutBuf || NULL == psiOutLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(pheOutBuf, 0, sizeof(pheOutBuf));
    *psiOutLen = 1;
    return SDK_OK;
}

//===========================emv Card=====================================/

/*******************************************************************
   ��	  ��: �ս���
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ�EMV���ۺ�
   ��ڲ���:
   �� �� ֵ:
                    ���ڵ���0: �Ӵ�ʽ���ۺ�
                    С��0:ʧ��
   ��	  ע:
 ********************************************************************/
s32 sdkIcGetIcdealCardNo(void) { //�Ӵ�ʽic�������Ź̶�Ϊ0.
    return 0;
}

//===========================emv Card=====================================/


/*=======BEGIN: sujianzhong 2013.08.20  10:5 modify===========*/
/*****************************************************************************
** Descriptions:   �ж��Ƿ���2.4Gģ��
** Parameters:     void
** Returned value: 0:û��  1:����  2:ֱͨ
** Created By:     sujianzhong20130820
** Remarks:        �������ϵ�Ѱ��֮ǰ �ȿ����Ƿ���ģ��
*****************************************************************************/
s32 sdkIccJudgeHave24GModule(void) {
    return 0;
}

/*====================== END======================== */
/*=======BEGIN: sujianzhong 2013.09.02  16:20 modify===========*/
s32 sdkIccOpenIcDev(void) {
    return (ddi_iccpsam_open(0) == DDI_OK) ? (SDK_OK) : (SDK_ERR); //0ΪIC �����ţ�>=1Ϊpsam������
}

s32 sdkIccCloseIcDev(void) {
    return (ddi_iccpsam_close(0) == DDI_OK) ? (SDK_OK) : (SDK_ERR);
}

s32 sdkIccOpenRfDev(void) {
    gucSdkIccRfCheckBattle = 0;

    //zcl 20141101
    //zhouxiaoxin
//    if (SDK_OK != sdkRfBatteryEnough()) {
//        return SDK_ICC_BAT_VOLTAGE_LOW;
//    }

    if (ddi_rf_open() != DDI_OK) {
        return SDK_ERR;
    }
    return SDK_OK;
}

s32 sdkIccCloseRfDev(void) {
    gucSdkIccRfCheckBattle = 0;

    if (ddi_rf_close() != DDI_OK) {
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ�ǽӿ��Ķ����汾
** Parameters:          u8 *pVerOut ���شſ��Ķ����汾��Ϣ \0��β
** Returned value:	�ɹ�:SDK_OK  ʧ��:SDK_ERR
** Created By:		���ƽ  2014.07.25
** Remarks:             \0��β����Ч�ַ��̶�����Ϊ17bytes
*****************************************************************************/
s32 sdkIccRfGetVersion(u8 *pVerOut) {
    s32 rtn = ddi_rf_ioctl(DDI_RF_CTL_VER, 0, (u32) pVerOut);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*====================== END======================== */

