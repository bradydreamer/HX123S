#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "sdkIccPrivateHead.h"

#ifndef  DEBUG_MAG
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define TRACK1  (79 + 2)
#define TRACK2  (39 + 2)
#define TRACK3  (104 + 2)

static s32 magcard_read(u8 *rbuf);

SDK_CARD_DATA gstSdkMagCardData;  //�ſ���Ϣ

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: �����ŵ���Ϣ
   ��ڲ���: 1: �ſ���Ϣ�ṹ��,2: �ŵ�������
   �� �� ֵ: true:��Ч�ſ�����,false: ����ſ�����
   ��    ע:lgp��ʱ���ú������޸�
 ********************************************************************/
bool sdkIccAdjustTrackData(SDK_CARD_DATA *pstMagCardData, const u8 *lpIn, SDK_ICC_CARDDATA *lpbuf) {
    u16 i;
    u8 len = 0;
    //u8 tempbuf[128] = {0};
    u8 *tempbuf = (u8 *) lpbuf;

    i = 0;

    if (NULL == lpbuf) {
        return false;
    }
#if 1 //SDK_IS_DDI_PLAM() /*Modify by ���ƽ at 2014.08.14  18:4 ddiƽ̨2,3�ŵ�Ϊ��ѹ�����ݣ����Ƚ���ѹ��*/
    len = lpIn[i++];

    if (len > 37) {
        Assert(0);
        len = 37;
    }
    //memset( tempbuf, 0 ,sizeof(tempbuf ));
    memset(tempbuf, 0, sizeof(SDK_ICC_CARDDATA));
    memcpy(tempbuf, lpIn + i, len);
    i += len;

    if (len % 2 != 0) //����
    {
        tempbuf[len] = 'f';
        len++;
    }
    TraceHex("mag", "tempbuf", tempbuf, len);
    sdkAscToBcd(pstMagCardData->Track2Data, tempbuf, len);
    pstMagCardData->Track2Len = len >> 1;
    Trace("mag", "2����is %d\r\n", pstMagCardData->Track2Len);
    TraceHex("mag", "2����", pstMagCardData->Track2Data, pstMagCardData->Track2Len);

    len = lpIn[i++];

    if (len > 104) {
        Assert(0);
        len = 104;
    }
    memset(tempbuf, 0, sizeof(SDK_ICC_CARDDATA));
    memcpy(tempbuf, lpIn + i, len);
    i += len;

    if (len % 2 != 0) //����
    {
        tempbuf[len] = 'f';
        len++;
    }
    TraceHex("mag", "tempbuf", tempbuf, len);
    sdkAscToBcd(pstMagCardData->Track3Data, tempbuf, len);
    pstMagCardData->Track3Len = len >> 1;
    Trace("mag", "3����is %d\r\n", pstMagCardData->Track3Len);
    TraceHex("mag", "3����", pstMagCardData->Track3Data, pstMagCardData->Track3Len);


    pstMagCardData->Track1Len = lpIn[i++];                     //1�ŵ�,ͬʱ�����ڹܵ������Ƿ�����

    if (pstMagCardData->Track1Len > 76) {
        pstMagCardData->Track1Len = 76;
    }
    memcpy(pstMagCardData->Track1Data, &lpIn[i], pstMagCardData->Track1Len);
    Trace("mag", "�ŵ�һ����is %d\r\n", pstMagCardData->Track1Len);
    TraceHex("mag", "1����", pstMagCardData->Track1Data, pstMagCardData->Track1Len);
#else
    pstMagCardData->Track2Len = lpIn[i++];                       //2�ŵ�

    if(pstMagCardData->Track2Len > 19)
    {
        pstMagCardData->Track2Len = 19;
    }
    memcpy(pstMagCardData->Track2Data, &lpIn[i], pstMagCardData->Track2Len);
    i += pstMagCardData->Track2Len;

    pstMagCardData->Track3Len = lpIn[i++];                       //3�ŵ�

    if(pstMagCardData->Track3Len > 52)
    {
        pstMagCardData->Track3Len = 52;
    }
    memcpy(pstMagCardData->Track3Data, &lpIn[i], pstMagCardData->Track3Len);
    i += pstMagCardData->Track3Len;

    pstMagCardData->Track1Len = lpIn[i++];                       //1�ŵ�,ͬʱ�����ڹܵ������Ƿ�����

    if(pstMagCardData->Track1Len > 76)
    {
        pstMagCardData->Track1Len = 76;
    }
    memcpy(pstMagCardData->Track1Data, &lpIn[i], pstMagCardData->Track1Len);
    Trace("mag", "�ŵ�һ����is %d\r\n", pstMagCardData->Track1Len);
    TraceHex("mag", "1����", pstMagCardData->Track1Data, pstMagCardData->Track1Len);
#endif /* if 0 */

    if (pstMagCardData->Track2Len || pstMagCardData->Track3Len) {
        if (pstMagCardData->Track3Len) {
            if ((pstMagCardData->Track3Data[pstMagCardData->Track3Len - 1] & 0x0F) == 0x0F) {
                pstMagCardData->Track3Data[pstMagCardData->Track3Len - 1] =
                        pstMagCardData->Track3Data[pstMagCardData->Track3Len - 1] & 0xF0;
                pstMagCardData->Track3Len = (u8)(2 * pstMagCardData->Track3Len) - 1;
            }
            else {
                pstMagCardData->Track3Len = (u8)(2 * pstMagCardData->Track3Len);
            }
        }

        if (pstMagCardData->Track2Len) {
            if ((pstMagCardData->Track2Data[pstMagCardData->Track2Len - 1] & 0x0F) == 0x0F) {
                pstMagCardData->Track2Data[pstMagCardData->Track2Len - 1] =
                        pstMagCardData->Track2Data[pstMagCardData->Track2Len - 1] & 0xF0;
                pstMagCardData->Track2Len = (u8)(2 * pstMagCardData->Track2Len) - 1;
            }
            else {
                pstMagCardData->Track2Len = (u8)(2 * pstMagCardData->Track2Len);
            }
        }
    }
    return true;   //fusuipu modefied at 2013-3-7 ���SDK��Եײ�����Ĵſ����ݵ����ơ���ȡ���ݺ�ֱ���ϴ���Ӧ�ò㡣
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ôſ������俨��
   ��ڲ���: 1: �ſ���Ϣ�ṹ��,2: ����
   �� �� ֵ: true:'D'����'2','6'����,��ICC��,false: 'D'����'2','6'����,��IC��
   ��    ע:lgp��ʱ���ú����޸�
 ********************************************************************/
u8 sdkIccGetMagCardNo(const SDK_CARD_DATA *pstMagCardData, u8 *lpOut) {
    //u8 temp[256], flag = false;
    u8 flag = false;
    u16 i;
    u32 len;
    u8 *temp = lpOut;

    if (pstMagCardData->Track2Len)                                                                                //2�ŵ�����
    {
        len = (pstMagCardData->Track2Len + 1) / 2;

        if (len > 19) {
            len = 19;
        }
        sdkBcdToAsc(temp, pstMagCardData->Track2Data, len);
    }
    else if (pstMagCardData->Track3Len)                                                                   //���ŵ�����
    {
        len = (pstMagCardData->Track3Len + 1) / 2;

        if (len > 52) {
            len = 52;
        }
        sdkBcdToAsc(temp, pstMagCardData->Track3Data + 1, len);
    }

    if (pstMagCardData->Track2Len || pstMagCardData->Track3Len) {
        len = strlen(temp);

        for (i = 0; i < len; i++) {
            if (temp[i] == 'D') {
                if (temp[i + 5] == '2' ||
                    temp[i + 5] == '6')                            //���D��ߵĵ�5λ��2��6��ʾ�ÿ�����IC��
                {
                    flag = true;
                }
                break;
            }
        }

        len = i;

        if (len > 19) {
            len = 19;
        }
        temp[len++] = 0;
    }
    else {
        //memset(temp, 0, sizeof(temp));
        memset(temp, 0, 80);
        len = pstMagCardData->HandLen;

        if (len > 19) {
            len = 19;
        }
        memcpy(temp, pstMagCardData->HandData, len);
        temp[len++] = 0;
    }
    //memcpy(lpOut, temp, len);
    return flag;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_CARDDATA *pstCardData:
** Returned value:	SDK_OK
                                SDK_PARA_ERR
                                SDK_ERR
** Created By:		sujianzhong2012.08.06
** Remarks:
*****************************************************************************/
s32 sdkIccGetCardData(SDK_ICC_CARDDATA *pstCardData) //sujianzhong 2013.01.10 14:33
{
    if (NULL == pstCardData) {
        return SDK_PARA_ERR;
    }
    memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));
    pstCardData->bIsIccMagCard = gstSdkMagCardData.HaveICC;

    if (gstSdkMagCardData.HaveExpired) //sujianzhong 2013.07.16 10:58
    {
        sdkBcdToAsc(pstCardData->asExpiredDate, gstSdkMagCardData.ExpiredDate, 2);
        pstCardData->asExpiredDate[4] = 0;
    }

    if (gstSdkMagCardData.HandLen > 19) {
        gstSdkMagCardData.HandLen = 19;
    }

    if (gstSdkMagCardData.HandLen > 0) {
        pstCardData->ucCardLen = gstSdkMagCardData.HandLen;
        memcpy(pstCardData->asCardNO, gstSdkMagCardData.HandData, gstSdkMagCardData.HandLen);
        pstCardData->asCardNO[gstSdkMagCardData.HandLen] = 0;
    }

    if (gstSdkMagCardData.Track1Len > 76) {
        /*=======BEGIN: fusuipu 2013.02.01  16:36 modify===========*/
        //return SDK_ERR;
        gstSdkMagCardData.Track1Len = 76;
        /*====================== END======================== */
    }

    if (gstSdkMagCardData.Track1Len > 0) {
        pstCardData->ucTrack1Len = gstSdkMagCardData.Track1Len;
        /*=======BEGIN: fusuipu 2013.01.31  16:51 modify===========*/
        //Verify(sdkBcdToAsc(pstCardData->asTrack1Data, MagCardData.Track1Data, (MagCardData.Track1Len + 1) >> 1) >= 0);
        memcpy(pstCardData->asTrack1Data, gstSdkMagCardData.Track1Data,
               gstSdkMagCardData.Track1Len);
        /*====================== END======================== */
        pstCardData->asTrack1Data[gstSdkMagCardData.Track1Len] = 0;
    }

    if (gstSdkMagCardData.Track2Len > 37) {
        /*=======BEGIN: fusuipu 2013.02.01  16:37 modify===========*/
        //return SDK_ERR;
        gstSdkMagCardData.Track2Len = 37;
        /*====================== END======================== */
    }

    if (gstSdkMagCardData.Track2Len > 0) {
        pstCardData->ucTrack2Len = gstSdkMagCardData.Track2Len;
        Verify(sdkBcdToAsc(pstCardData->asTrack2Data, gstSdkMagCardData.Track2Data,
                           (gstSdkMagCardData.Track2Len + 1) >> 1) >= 0);
        pstCardData->asTrack2Data[gstSdkMagCardData.Track2Len] = 0;
    }

    if (gstSdkMagCardData.Track3Len > 104) {
        /*=======BEGIN: fusuipu 2013.02.01  16:37 modify===========*/
        //return SDK_ERR;
        gstSdkMagCardData.Track3Len = 104;
        /*====================== END======================== */
    }

    if (gstSdkMagCardData.Track3Len > 0) {
        pstCardData->ucTrack3Len = gstSdkMagCardData.Track3Len;
        Verify(sdkBcdToAsc(pstCardData->asTrack3Data, gstSdkMagCardData.Track3Data,
                           (gstSdkMagCardData.Track3Len + 1) >> 1) >= 0);
        pstCardData->asTrack3Data[gstSdkMagCardData.Track3Len] = 0;
    }
    return SDK_OK;
}

#if 0 /*Modify by ���ٲ� at 2014.03.21  8:52 */
void sdkIccClearDevComData(u8 comid)
{
    Verify(sdk_dev_com_clear_buffer(comid) >= 0);
}

#endif /* if 0 */


//bool MagCardEnable;  //FALSE:��ֹˢ��,TRUE:����ˢ��

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.01.13
** Remarks:             �ѵײ�򿪷ŵ�ϵͳ��ʼ������
*****************************************************************************/
s32 sdkIccOpenMag(void) {
    s32 ret;

    //MagCardEnable = true;
    memset(&gstSdkMagCardData, 0, sizeof(SDK_CARD_DATA));
    //sdk_dev_magcard_open(); //�򿪴ſ�
    //sdk_dev_magcard_clear_fifo();
    //sdk_dev_read_fifo_pthread_open();

    ret = ddi_mag_open();
    ddi_mag_clear();

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_ERR) {
        return SDK_ERR;
    }
    else {
        return SDK_EBUSY;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:      SDK_ICC_CARDDATA *pstCardData
** Returned value:
** Created By:      sujianzhong   2012.09.26
** Remarks:         ������ʵֻ�дſ�����
*****************************************************************************/
s32 sdkIccGetMagData(SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData) {
    u8 *fifo;
    s32 Len;                                                                                            //����

    //u8 temp[256] = {0};
//    int readcardrst;
// s32 rslt = 0;

    if (pstCardData == NULL || pstIccParam == NULL) //fusuipu modefied at 2013-1-31
    {
        return SDK_PARA_ERR;
    }
/*
    if(MagCardEnable == false)
    {
        Trace("emv", "������ˢ�ſ�!");
        return SDK_ERR;
    }
 */
    fifo = (u8 *) sdkGetMem(256); //һ�����ŵ�79 + 2 + 39 + 2 + 104 + 2 = 226 < 256

    if (NULL == fifo) {
        Assert(0);
        return SDK_ERR;
    }
    memset(fifo, 0, 256);
    Len = magcard_read(fifo);

    if (Len > 3) //zhouzhihua 2013.06.24 15:51  ��50x��0x31��������ȷ��0x37���ݴ���
    {
        TraceHex("mag", "fifodata\r\n", fifo, Len);
        memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));

        if (sdkIccAdjustTrackData(&gstSdkMagCardData, fifo, pstCardData)) {
            memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));
            memset(fifo, 0, 256);
            gstSdkMagCardData.HaveICC = sdkIccGetMagCardNo(&gstSdkMagCardData, fifo);
            gstSdkMagCardData.HandLen = (u8) strlen(fifo);
            memcpy(gstSdkMagCardData.HandData, fifo, gstSdkMagCardData.HandLen);

            if (sdkIccGetCardData(pstCardData) == SDK_OK) {
                //sdkSysBeep(SDK_SYS_BEEP_OK);
                pstIccParam->ucCardMode = SDK_ICC_MAG;
                TraceHex("emv", "mag data", pstCardData, sizeof(SDK_ICC_CARDDATA));

                if (NULL != fifo) {
                    sdkFreeMem(fifo);
                    //fifo = NULL;
                }
                return SDK_OK;
            }
        }
        //sdkSysBeep(SDK_SYS_BEEP_ERR);
        memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));
        memset(&gstSdkMagCardData, 0, sizeof(SDK_CARD_DATA));

        if (NULL != fifo) {
            sdkFreeMem(fifo);
            //fifo = NULL;
        }
        return SDK_ICC_BRUSH_ERR;
    }

    if (NULL != fifo) {
        sdkFreeMem(fifo);
        //fifo = NULL;
    }
    return SDK_ICC_NOCARD;
}

/*****************************************************************************
** Descriptions:	��ȡ�ſ��Ķ����汾
** Parameters:          u8 *pVerOut ���شſ��Ķ����汾��Ϣ \0��β
** Returned value:	�ɹ�:SDK_OK  ʧ��:SDK_ERR
** Created By:		���ƽ  2014.07.25
** Remarks:             \0��β����Ч�ַ��̶�����Ϊ17bytes
*****************************************************************************/
s32 sdkMagcardGetVersion(u8 *pVerOut) {
    s32 rtn = ddi_mag_ioctl(DDI_MAG_CTL_VER, 0, (u32) pVerOut);

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

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.01.13
** Remarks:
*****************************************************************************/
void sdkIccCloseMag(void) {
    //MagCardEnable = false;
    //sdk_dev_magcard_close();
    //sdk_dev_read_fifo_pthread_close();

    ddi_mag_close();
}

/*=======================================================*/



/*****************************************************************************
** Descriptions:     ��ȡ�ſ�����
** Parameters:       u8 *rbuf
** Returned value:	 ���ݳ��� ��SDK_ERR Ϊˢ������  0Ϊ��ʾû��ˢ������
** Created By:		shiweisong  2013.07.10
** Remarks:        ���ݸ�ʽΪ
                   rbuf[0]                                   �ڶ��ŵ����ݳ���   rbuf[XX] �ڶ��ŵ�����
                   rbuf[rbuf[0] + 1]                         �����ŵ����ݳ���   rbuf[XX] �ڶ��ŵ�����
                   rbuf[rbuf[0]+rbuf[rbuf[0]] + 2 ]          ��һ�ŵ����ݳ���   rbuf[XX] ��1�ŵ�����

*****************************************************************************/
static s32 magcard_read(u8 *rbuf) {
    s32 i;
    s32 rslt;
    u16 rlen;
    //u8 sTrack1[79 + 2];
    //u8 sTrack2[39 + 2];
    //u8 sTrack3[104 + 2];
    u8 *sTrack1 = rbuf + TRACK2 + TRACK3 + 1;
    u8 *sTrack2 = rbuf + 1;
    u8 *sTrack3 = rbuf + TRACK2 + 1;
    s32 loop = 0;

    //memset( sTrack1, 0, sizeof(sTrack1) );
    //memset( sTrack2, 0, sizeof(sTrack2) );
    //memset( sTrack3, 0, sizeof(sTrack3) );
    memset(sTrack1 - 1, 0, TRACK1);
    memset(sTrack2 - 1, 0, TRACK2);
    memset(sTrack3 - 1, 0, TRACK3);
    rslt = ddi_mag_read(sTrack1, sTrack2, sTrack3);

//	memset(rbuf, 0, TRACK1 + TRACK2 + TRACK3);
//	rslt = ddi_mag_read ( rbuf, rbuf + TRACK1, rbuf + TRACK2);
    Trace("mag", "rslt is %d\r\n", rslt);
    TraceHex("mag", "strack1 is:", sTrack1, strlen(sTrack1));
    TraceHex("mag", "strack2 is:", sTrack2, strlen(sTrack2));
    TraceHex("mag", "strack3 is:", sTrack3, strlen(sTrack3));

    if (DDI_OK == rslt) {
        //sdkBcdToU16( &rlen, sTrack2, 2 );
        rlen = strlen(sTrack2);
        Trace("mag", "2 rlen is %d\r\n", rlen);

        for (loop = 0; loop < rlen; loop++) {
            if (sTrack2[loop] == 0x3d) //�Ⱥ�
            {
                sTrack2[loop] = 0x44; //��дD
            }
        }

        i = 0;
        rbuf[i++] = rlen;
        memcpy(rbuf + i, sTrack2, rlen);  //lpTrack2��2�ŵ����ݣ���ʽ��z.. LLVAR����󳤶�ΪN=37
        i += rlen;

        //sdkBcdToU16( &rlen, sTrack3, 2 );
        rlen = strlen(sTrack3);
        Trace("mag", "3rlen is %d\r\n", rlen);

        for (loop = 0; loop < rlen; loop++) {
            if (sTrack3[loop] == 0x3d) //�Ⱥ�
            {
                sTrack3[loop] = 0x44; //��дD
            }
        }

        rbuf[i++] = rlen;
        memcpy(rbuf + i, sTrack3, rlen);  //lpTrack3��3�ŵ����ݣ���ʽ��z... LLLVAR����󳤶�N=104
        i += rlen;


        //sdkBcdToU16( &rlen, sTrack1, 2 );
        rlen = strlen(sTrack1);
        Trace("mag", "1rlen is %d\r\n", rlen);
        rbuf[i++] = rlen;
        memcpy(rbuf + i, sTrack1, rlen);  //lpTrack1��1�ŵ����ݣ���ʽ��ans.. LLVAR����󳤶�N=79
        i += rlen;

        TraceHex("mag", "magic", rbuf, i);
        return i;
    }
    return SDK_ERR;
}

