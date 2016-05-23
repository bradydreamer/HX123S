/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkTestUI.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��2��23��
   ����޸�   :
   ��������   : �˵�UI���Դ���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2013��2��23��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/
#include "sdkGlobal.h"
#include "SdkTest.h"

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *str
** Returned value:
** Created By:		fusuipu  2014.01.03
** Remarks:
*****************************************************************************/
void Test_sdkPrintStr(u8 *str, s32 siLangMode) {
    SDK_PRINT_FONT font;
    s32 i = 0, k = 0;
    static u8 Not_Pin_Farsi_Font[3][10] =
            {
                    {SDK_SYS_LANG_FAR, 5, SDK_PRN_FARI_12X12, SDK_PRN_FARI_16X16, SDK_PRN_FARI_24X24, SDK_PRN_FARI_32X32, SDK_PRN_FARI_48X48},
                    {SDK_SYS_LANG_RUS, 3, SDK_PRN_RUSS_8X16,  SDK_PRN_RUSS_12X24, SDK_PRN_RUSS_16X32},
                    {SDK_SYS_LANG_GER, 6, SDK_PRN_GEOR_6X12,  SDK_PRN_GEOR_8X16,  SDK_PRN_GEOR_10X20, SDK_PRN_GEOR_12X24, SDK_PRN_GEOR_16X32, SDK_PRN_GEOR_24X48},
            };


    font.uiAscFont = SDK_PRN_ASCII8X16;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = SDK_PRN_CH24X24;
    font.uiChZoom = SDK_PRN_ZOOM_N;

    sdkPrintInit();
    Trace("fspmul", "enter multi ziti,ready print\r\n");

    for (k = 0; k < 3; k++) {
        if (siLangMode == Not_Pin_Farsi_Font[k][0]) {
            memset(&font, 0, sizeof(font));

            for (i = 0; i < Not_Pin_Farsi_Font[k][1]; i++) {
                font.uiAscFont = Not_Pin_Farsi_Font[k][i + 2];
                sdkPrintStr(str, font, SDK_PRINT_LEFTALIGN, 0, 1);
            }

            sdkPrintStart();
            return;
        }
    }

    sdkPrintStr("use default ziti print", font, SDK_PRINT_LEFTALIGN, 0, 1);
    sdkPrintStr(str, font, SDK_PRINT_LEFTALIGN, 0, 1);
    sdkPrintStart();

    return;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siKey
** Returned value:
** Created By:		fusuipu  2013.12.24
** Remarks:
*****************************************************************************/
static s32 sdkChoiceLang(s32 *Mask) {
    s32 key = 0;
    s32 lang_mode = 0;
    s32 line_mode = 0;

    key = sdkDispMsgBox("Multi Lang Test",
                        "1.Farsi\r\n2.Russian\r\n3.Georaia\r\n4.All multi print\r\n5.T9pinyin", 0,
                        SDK_KEY_MASK_ALL);

    if (SDK_KEY_1 == key || SDK_KEY_3 == key) {
        line_mode = sdkDispMsgBox("Farsi line mode", "1.max line:4\r\n2.max line:5\r\n", 0,
                                  SDK_KEY_MASK_ALL);

        if (SDK_KEY_1 == line_mode) {
            line_mode = 4;
        }
        else {
            line_mode = 5;
        }
    }

    switch (key) {
        case SDK_KEY_1: {
            sdkSysMultiLangInit(SDK_SYS_LANG_FAR, line_mode);

            *Mask = SDK_MMI_FARSI;
            lang_mode = SDK_SYS_LANG_FAR;
        }
            break;

        case SDK_KEY_2: {
            sdkSysMultiLangInit(SDK_SYS_LANG_RUS, line_mode);
            *Mask = SDK_MMI_RUSSIAN;
            lang_mode = SDK_SYS_LANG_RUS;
        }
            break;

        case SDK_KEY_3: {
            sdkSysMultiLangInit(SDK_SYS_LANG_GER, line_mode);
            *Mask = SDK_MMI_GEORGIA;
            lang_mode = SDK_SYS_LANG_GER;
        }
            break;

        case SDK_KEY_4: {
            Test_sdkPrintStrAuto();
            return SDK_ESC;
        }
            break;

        default: {
            sdkSysMultiLangInit(SDK_SYS_LANG_CHN, line_mode);
            *Mask = SDK_MMI_DEFAULT_STR;
            lang_mode = SDK_SYS_LANG_CHN;
        }
            break;
    }

    sdkDispClearScreen();
    sdkDispBrushScreen();
    return lang_mode;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2014.01.02
** Remarks:
*****************************************************************************/
s32 sdkMultiLangTest(void) {
    s32 mask = 0;
    u8 buf[512] = {0};
    s32 lang = 0;

    lang = sdkChoiceLang(&mask);
    sdkDispClearScreen();
    sdkDispBrushScreen();

    if (SDK_ESC == lang) {
        return SDK_OK;
    }
    memset(buf, 0, sizeof(buf));
    sdkKbGetScanf(0, buf, 1, 100, mask, SDK_DISP_LINE1);
    Test_sdkPrintStr(&buf[1], lang);
    sdkSysMultiLangInit(SDK_SYS_LANG_CHN, 5);
    return SDK_OK;
}

