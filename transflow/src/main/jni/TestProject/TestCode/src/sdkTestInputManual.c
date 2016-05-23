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


/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkKbGetScanfManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    s32 param1;
    u8 param2[SDK_MAX_STR_LEN];
    s32 param3;
    s32 param4;
    s32 param5;
    s32 param6;
    u8 originaldata[SDK_MAX_STR_LEN];
    u8 temp[SDK_MAX_STR_LEN];

    memset(temp, 0, sizeof(temp));
    memset(param2, 0, sizeof(param2));
    memset(originaldata, 0, sizeof(originaldata));
    param1 = sdkTestInputs32(pTitle, "pls input overtime");
    sdkTestInputASC(&temp[1], pTitle, "pls input pout", 0, 100);
    param3 = sdkTestInputs32(pTitle, "pls input minlen");
    param4 = sdkTestInputs32(pTitle, "pls input maxlen");
    param5 = sdkTestInputs32(pTitle, "pls input inmode");
    param6 = sdkTestInputs32(pTitle, "pls input row");
    temp[0] = strlen(&temp[1]);
    memcpy(param2, temp, temp[0] + 1);
    memcpy(originaldata, &temp[1], SDK_MAX_STR_LEN);

    sdkDispClearScreen();
    ret = sdkKbGetScanf(param1, param2, param3, param4, param5, param6);
    /*=======BEGIN: wangmingming 2014.06.13  9:46 modify===========*/
    //����ʾ����(������ݵĵ�һ���ֽ�)
    sprintf(pasOutInfo,
            "input1:%d \rinput2:%s\rinput3:%d\rinput4:%d\rinput5:%d \rinput6:%d \routputdata:%s \rreturn:%d",
            param1, originaldata, param3, param4, param5, param6, &param2[1], ret);
    /*====================== END======================== */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 sdkCallFun(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow, void *pVar) {
    u8 temp[128] = {0};
    u8 buf[2048] = {0};
    u32 i = (*(s32 *) pVar);

    if (NULL == pStrDisp) {
        return SDK_ERR;
    }
    /*=======BEGIN: wangmingming 2014.06.13  9:15 modify===========*/
    //����ʾ����(��һ���ֽ�)
    sprintf(temp, "input:%s\r", &pStrDisp[1]);
    /*====================== END======================== */
    strcat(buf, temp);
    sprintf(temp, "uiMode:%d\r", uiMode);
    strcat(buf, temp);
    sprintf(temp, "siDispRow:%d\r", siDispRow);
    strcat(buf, temp);
    sprintf(temp, "input para:%d\r", i);
    strcat(buf, temp);
    sdkDispLabel(0, buf, 7);

    return SDK_OK;
}

static s32 Test_sdkKbGetScanfExManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    s32 param1;
    u8 param2[SDK_MAX_STR_LEN];
    s32 param3;
    s32 param4;
    s32 param5;
    s32 param6;
    s32 param7;
    s32 param8;
    s32 param9;
    u8 originaldata[SDK_MAX_STR_LEN];
    u8 temp[SDK_MAX_STR_LEN];

    s32(*psdkCallFun)(u8 * pStrDisp,
    const u32 uiMode,
    const s32 siDispRow,
    void *pVar) = NULL;

    memset(temp, 0, sizeof(temp));
    memset(param2, 0, sizeof(param2));
    memset(originaldata, 0, sizeof(originaldata));
    param1 = sdkTestInputs32(pTitle, "pls input overtime");
    sdkTestInputASC(&temp[1], pTitle, "pls input pout", 0, 100);
    param3 = sdkTestInputs32(pTitle, "pls input minlen");
    param4 = sdkTestInputs32(pTitle, "pls input maxlen");
    param5 = sdkTestInputs32(pTitle, "pls input inmode");
    param6 = sdkTestInputs32(pTitle, "pls input row");
    param7 = sdkTestInputs32(pTitle, "first scanf:");
    param8 = sdkTestInputs32(pTitle, "1-callfun 0-nocallfun");

    if (param8 != 0) {
        param9 = sdkTestInputs32(pTitle, "pls input test para");
        psdkCallFun = sdkCallFun;
    }
    temp[0] = strlen(&temp[1]);
    memcpy(param2, temp, temp[0] + 1);
    memcpy(originaldata, &temp[1], SDK_MAX_STR_LEN);

    sdkDispClearScreen();
    sdkDispBrushScreen();
    ret = sdkKbGetScanfEx(param1, param2, param3, param4, param5, param6, psdkCallFun, &param9,
                          param7);
    /*=======BEGIN: wangmingming 2014.06.13  9:47 modify===========*/
    if (param8 != 0) {//�лص�����
        //����ʾ����(������ݵĵ�һ���ֽڣ��лص�����ʱ���еڶ����ֽ�)
        sprintf(pasOutInfo,
                "input1:%d \rinput2:%s\rinput3:%d\rinput4:%d\rinput5:%d \rinput6:%d \routputdata:%s \rreturn:%d",
                param1, originaldata, param3, param4, param5, param6, &param2[2], ret);
    }
    else {//�޻ص�����
        //����ʾ����(������ݵĵ�һ���ֽ�)
        sprintf(pasOutInfo,
                "input1:%d \rinput2:%s\rinput3:%d\rinput4:%d\rinput5:%d \rinput6:%d \routputdata:%s \rreturn:%d",
                param1, originaldata, param3, param4, param5, param6, &param2[1], ret);
    }
    /*====================== END======================== */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkKbGetKeyManual(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�����������ȡ�İ���", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    while (ret == 0) {
        ret = sdkKbGetKey();
    }

    sprintf(pasOutInfo, "getkey:0x%x", ret);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����IP����
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2014.02.24
** Remarks:
*****************************************************************************/
static s32 Test_sdkInputIpManual(char *pasOutInfo, char const *pTitle) //17//
{
    u8 temp[128] = {0};
    s32 ret = 0;

    sdkDispClearScreen(); //shijianglong 2013.07.18 14:17
    sdkDispBrushScreen(); //shijianglong 2013.07.18 14:17

    ret = sdkInputIp(SDK_DISP_LINE1, "������ip:", SDK_DISP_LINE3, temp, 2, 10000);
    sprintf(pasOutInfo, "ip:%s \r����:%d", temp, ret);
    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2014.02.24
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP InputManuaMap[] =
        {
                {"sdkKbGetScanf",   Test_sdkKbGetScanfManual},
                {"sdkKbGetScanfEx", Test_sdkKbGetScanfExManual},
                {"sdkKbGetKey",     Test_sdkKbGetKeyManual},
                {"sdkInputIp",      Test_sdkInputIpManual},
        };

TEST_MAP_INFO GetInputManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(InputManuaMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = InputManuaMap;
    return ret;
}

