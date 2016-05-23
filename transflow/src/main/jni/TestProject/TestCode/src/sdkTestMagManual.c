/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkMathMaualTest.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��1��22��
   ����޸�   :
   ��������   : �ֶ�������ѧ��
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2013��1��22��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

#include "sdkGlobal.h"
#include "SdkTest.h"


#define InitMem(dsc) memset(&dsc, 0, sizeof(dsc))
#define GetLen(Len1, Len2) ((Len1) > (Len2) ? (Len2) : (Len1))

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                              char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.01.30
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccGetMagDataManual(char *pasOutInfo, char const *pTitle) {
    s32 rslt = 10000;
    s32 rslt1 = 0, rslt2 = 0;
    u32 timer = 0;
    s32 key = 0;

    SDK_ICC_PARAM sticcparam;
    SDK_ICC_CARDDATA stSaleData;

    memset(&sticcparam, 0, sizeof(sticcparam));
    memset(&stSaleData, 0, sizeof(stSaleData));


    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "GetMagData��������",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);     //װ�Դ�
    //sdkDispFillRowRam(SDK_DISP_LINE3, 0, "10s����ˢ��!", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);     //װ�Դ�
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "ȷ�ϼ���ʼ��ʱ!",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);     //װ�Դ�
    sdkDispBrushScreen();             //��ʾ

    while (1) {
        key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);

        if (SDK_KEY_ENTER == key) {
            break;
        }
        else if (SDK_KEY_ESC == key) {
            return 0;
        }
        else { ;
        }
    }

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ʱ��ʼ",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);     //װ�Դ�
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "10s����ˢ��!",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);     //װ�Դ�
    sdkDispBrushScreen();             //��ʾ
    timer = sdkTimerGetId();
    Trace("zhouzhihua", "cfg-----\r\n");
    rslt1 = sdkIccOpenMag();
    while ((rslt = sdkIccGetMagData(&sticcparam, &stSaleData)) != SDK_OK) {
        if (sdkTimerIsEnd(timer, 10000) == true || rslt != SDK_ICC_NOCARD) {
            break;
        }
    }
    Trace("zhouzhihua", "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^-----\r\n");
    sdkIccCloseMag();


    sprintf(pasOutInfo,
            "OpenMagRet:%d\r\nbIsIccMagCard:%d\r\nucUIDLen:%d\r\nuucUID:%s\r\nuucCardLen:%d\r\nuasCardNO:%s\r\nasExpiredDate:%d\r\nTrack1len:%d\r\nTrack1data:%s\r\nTrack2len:%d\r\nTrack2data:%s\r\nTrack3len:%d\r\nTrack3data:%s\r\nreturn:%d\r\n",
            rslt1,
            stSaleData.bIsIccMagCard,
            stSaleData.ucUIDLen, stSaleData.ucUID,
            stSaleData.ucCardLen, stSaleData.asCardNO,
            stSaleData.asExpiredDate,
            stSaleData.ucTrack1Len, stSaleData.asTrack1Data,
            stSaleData.ucTrack2Len, stSaleData.asTrack2Data,
            stSaleData.ucTrack3Len, stSaleData.asTrack3Data,
            rslt);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.03
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP MagManaulMap[] =
        {
                {"sdkIccGetMagData", Test_sdkIccGetMagDataManual}
        };

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.03
** Remarks:
*****************************************************************************/
TEST_MAP_INFO GetMagManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(MagManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = MagManaulMap;
    return ret;
}

