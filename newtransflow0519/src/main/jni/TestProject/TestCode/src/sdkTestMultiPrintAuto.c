#include "sdkGlobal.h"
#include "SdkTest.h"

#define MULKEYNUM    10
#define SDK_KEY_TAB_MAX_LEN    40

/*****************************************************************************
** Descriptions:	��һ���ַ���Unicode(UCS-2��UCS-4)����ת����UTF-8����
** Parameters:		uiUnic	�ַ���Unicode����ֵ
                    pOutput	ָ����������ڴ洢UTF8����ֵ�Ļ�������ָ��
                    outsize	pOutput����Ĵ�С
** Returned value:	����ת������ַ���UTF8������ռ���ֽ���, ��������򷵻� 0
** Created By:		fusuipu  2013.04.12
** Remarks:			1. UTF8û���ֽ�������, ����Unicode���ֽ���Ҫ��;
						�ֽ����Ϊ���(Big Endian)��С��(Little Endian)����;
						��Intel�������в���С�˷���ʾ, �ڴ˲���С�˷���ʾ. (�͵�ַ���λ)
					2. �뱣֤ pOutput �������������� 6 �ֽڵĿռ��С!
*****************************************************************************/
static s32 sdkMmiMultiUnicodeToutf8(u32 uiUnic, u8 *pOutput, s32 siOutSize) {
    Assert(pOutput != NULL);
    Assert(siOutSize >= 6);

    if (uiUnic <= 0x0000007F) {
        // * U-00000000 - U-0000007F : 0xxxxxxx
        *pOutput = (uiUnic & 0x7F);
        return 1;
    }
    else if (uiUnic >= 0x00000080 && uiUnic <= 0x000007FF) {
        // * U-00000080 - U-000007FF : 110xxxxx 10xxxxxx
        *(pOutput + 1) = (uiUnic & 0x3F) | 0x80;
        *pOutput = ((uiUnic >> 6) & 0x1F) | 0xC0;
        return 2;
    }
    else if (uiUnic >= 0x00000800 && uiUnic <= 0x0000FFFF) {
        // * U-00000800 - U-0000FFFF : 1110xxxx 10xxxxxx 10xxxxxx
        *(pOutput + 2) = (uiUnic & 0x3F) | 0x80;
        *(pOutput + 1) = ((uiUnic >> 6) & 0x3F) | 0x80;
        *pOutput = ((uiUnic >> 12) & 0x0F) | 0xE0;
        return 3;
    }
    else if (uiUnic >= 0x00010000 && uiUnic <= 0x001FFFFF) {
        // * U-00010000 - U-001FFFFF : 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 3) = (uiUnic & 0x3F) | 0x80;
        *(pOutput + 2) = ((uiUnic >> 6) & 0x3F) | 0x80;
        *(pOutput + 1) = ((uiUnic >> 12) & 0x3F) | 0x80;
        *pOutput = ((uiUnic >> 18) & 0x07) | 0xF0;
        return 4;
    }
    else if (uiUnic >= 0x00200000 && uiUnic <= 0x03FFFFFF) {
        // * U-00200000 - U-03FFFFFF : 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 4) = (uiUnic & 0x3F) | 0x80;
        *(pOutput + 3) = ((uiUnic >> 6) & 0x3F) | 0x80;
        *(pOutput + 2) = ((uiUnic >> 12) & 0x3F) | 0x80;
        *(pOutput + 1) = ((uiUnic >> 18) & 0x3F) | 0x80;
        *pOutput = ((uiUnic >> 24) & 0x03) | 0xF8;
        return 5;
    }
    else if (uiUnic >= 0x04000000 && uiUnic <= 0x7FFFFFFF) {
        // * U-04000000 - U-7FFFFFFF : 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
        *(pOutput + 5) = (uiUnic & 0x3F) | 0x80;
        *(pOutput + 4) = ((uiUnic >> 6) & 0x3F) | 0x80;
        *(pOutput + 3) = ((uiUnic >> 12) & 0x3F) | 0x80;
        *(pOutput + 2) = ((uiUnic >> 18) & 0x3F) | 0x80;
        *(pOutput + 1) = ((uiUnic >> 24) & 0x3F) | 0x80;
        *pOutput = ((uiUnic >> 30) & 0x01) | 0xFC;
        return 6;
    }
    return 0;
}

/*****************************************************************************
** Descriptions:	��˹�ı�
** Parameters:		u8 KeyTab[MULKEYNUM][7]
					u8 Bigif
** Returned value:
** Created By:		lilin2012.09.14
** Remarks:
*****************************************************************************/
static void sdkMmiMultiGetFarKeyPad(u8 KeyTab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN]) {
    u8 parsi_buf[20] = {0};
    u8 i = 0;
    s16 j = 0, z = 0, len = 0;
    s32 ret = 0;
    static const u16 sdkParsiKeyPadLen[10] = {3, 5, 5, 5, 6, 5, 5, 7, 5, 1};
    static const u16 sdkPareLetterKeyPad[MULKEYNUM][7] =
            {
                    {0x0661, 0x06d4, 0x066b},
                    {0x0662, 0x062b, 0x062a, 0x067e, 0x0628},
                    {0x0663, 0x064a, 0x0621, 0x0622, 0x0627},
                    {0x0664, 0x0636, 0x0635, 0x0634, 0x0633},
                    {0x0665, 0x0698, 0x0632, 0x0631, 0x0630, 0x062f},
                    {0x0666, 0x062e, 0x062d, 0x0686, 0x062c},
                    {0x0667, 0x06cc, 0x0647, 0x0648, 0x0646},
                    {0x0668, 0x0645, 0x0644, 0x06af, 0x06a9, 0x0642, 0x0641},
                    {0x0669, 0x063a, 0x0639, 0x0638, 0x0637},
                    {0x0660}
            };

    for (i = 0; i < MULKEYNUM; i++) {
        j = sdkParsiKeyPadLen[i];
        memset(KeyTab[i], 0, 20);
        for (z = 0; z < j; z++) {
            memset(parsi_buf, 0, sizeof(parsi_buf));
            len = sdkMmiMultiUnicodeToutf8(sdkPareLetterKeyPad[i][z], parsi_buf, 10);
            strncat(KeyTab[i], parsi_buf, len);
            ret += len;
        }
        //Trace("fsp","KeyTab[%d]\r\n",i);
        //TraceHex("fsp","Russian KeyTab:",KeyTab[i],ret);
        KeyTab[i][SDK_KEY_TAB_MAX_LEN - 1] = ret;
        ret = 0;
    }
}

/*****************************************************************************
** Descriptions:	��˹�ı�
** Parameters:		u8 KeyTab[MULKEYNUM][7]
					u8 Bigif
** Returned value:
** Created By:		lilin2012.09.14
** Remarks:
*****************************************************************************/
static void sdkMmiMultiGetRussianKeyPad(u8 KeyTab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN]) {
    u8 parsi_buf[20] = {0};
    u8 i = 0;
    s16 j = 0, z = 0, len = 0;
    s32 ret = 0;

    static const u16 sdkRuaasinKeyPadLen[10] = {0, 8, 10, 8, 8, 8, 10, 8, 10, 0};
    static const u16 sdkRuaasinKeyPad[MULKEYNUM][10] =
            {
                    {0},
                    {0x0410, 0x0411, 0x0412, 0x0413, 0x0430, 0x0431, 0x0432, 0x0433},
                    {0x0414, 0x0415, 0x0401, 0x0416, 0x0417, 0x0434, 0x0435, 0x0451, 0x0436, 0x0437},
                    {0x0418, 0x0419, 0x041A, 0x041B, 0x0438, 0x0439, 0x043A, 0x043B},
                    {0x041C, 0x041D, 0x041E, 0x041F, 0x043C, 0x043D, 0x043E, 0x043F},
                    {0x0420, 0x0421, 0x0422, 0x0423, 0x0440, 0x0441, 0x0442, 0x0443},
                    {0x0424, 0x0425, 0x0426, 0x0427, 0x0444, 0x0445, 0x0446, 0x0447},
                    {0x0428, 0x0429, 0x042A, 0x042B, 0x0448, 0x0449, 0x044A, 0x044B},
                    {0x042C, 0x042D, 0x042E, 0x042F, 0x044C, 0x044D, 0x044E, 0x044F},
                    {0}
            };

    for (i = 0; i < MULKEYNUM; i++) {
        j = sdkRuaasinKeyPadLen[i];
        memset(KeyTab[i], 0, 20);
        for (z = 0; z < j; z++) {
            memset(parsi_buf, 0, sizeof(parsi_buf));
            len = sdkMmiMultiUnicodeToutf8(sdkRuaasinKeyPad[i][z], parsi_buf, 10);
            strncat(KeyTab[i], parsi_buf, len);
            ret += len;
        }
        KeyTab[i][SDK_KEY_TAB_MAX_LEN - 1] = ret;
        ret = 0;
    }
}

/*****************************************************************************
** Descriptions:	��˹�ı�
** Parameters:		u8 KeyTab[MULKEYNUM][7]
					u8 Bigif
** Returned value:
** Created By:		lilin2012.09.14
** Remarks:
*****************************************************************************/
static void sdkMmiMultiGetGeorgiaKeyPad(u8 KeyTab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN]) {
    u8 i = 0;

    static const u8 sdkGeorgiaKeyPadLen[10] = {0, 12, 12, 12, 12, 12, 12, 12, 15, 0};
    static const u8 sdkGeorgiaKeyPad[MULKEYNUM][20] =
            {
                    {0},
                    {0xe1, 0x83, 0x90, 0xe1, 0x83, 0x91, 0xe1, 0x83, 0x92, 0xe1, 0x83, 0x93},
                    {0xe1, 0x83, 0x94, 0xe1, 0x83, 0x95, 0xe1, 0x83, 0x96, 0xe1, 0x83, 0x97},
                    {0xe1, 0x83, 0x98, 0xe1, 0x83, 0x99, 0xe1, 0x83, 0x9a, 0xe1, 0x83, 0x9b},
                    {0xe1, 0x83, 0x9c, 0xe1, 0x83, 0x9d, 0xe1, 0x83, 0x9e, 0xe1, 0x83, 0x9f},
                    {0xe1, 0x83, 0xa0, 0xe1, 0x83, 0xa1, 0xe1, 0x83, 0xa2, 0xe1, 0x83, 0xa3},
                    {0xe1, 0x83, 0xa4, 0xe1, 0x83, 0xa5, 0xe1, 0x83, 0xa6, 0xe1, 0x83, 0xa7},
                    {0xe1, 0x83, 0xa8, 0xe1, 0x83, 0xa9, 0xe1, 0x83, 0xaa, 0xe1, 0x83, 0xab},
                    {0xe1, 0x83, 0xac, 0xe1, 0x83, 0xad, 0xe1, 0x83, 0xae, 0xe1, 0x83, 0xaf, 0xe1, 0x83, 0xb0},
                    {0}
            };

    for (i = 0; i < MULKEYNUM; i++) {
        memset(KeyTab[i], 0, 20);
        memcpy(KeyTab[i], sdkGeorgiaKeyPad[i], sdkGeorgiaKeyPadLen[i]);
        KeyTab[i][SDK_KEY_TAB_MAX_LEN - 1] = sdkGeorgiaKeyPadLen[i];
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:		u8 *str
** Returned value:
** Created By:		fusuipu  2014.01.03
** Remarks:
*****************************************************************************/
void Test_sdkPrintStrAuto(void) {
    u8 KeyTab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN] = {{0}};
    SDK_PRINT_FONT font;
    s32 i = 0, k = 0;
    u8 buf[128] = {0};
    static u8 Not_Pin_Farsi_Font[3][10] =
            {
                    {5, SDK_PRN_FARI_12X12, SDK_PRN_FARI_16X16, SDK_PRN_FARI_24X24, SDK_PRN_FARI_32X32, SDK_PRN_FARI_48X48},
                    {3, SDK_PRN_RUSS_8X16,  SDK_PRN_RUSS_12X24, SDK_PRN_RUSS_16X32},
                    {6, SDK_PRN_GEOR_6X12,  SDK_PRN_GEOR_8X16,  SDK_PRN_GEOR_10X20, SDK_PRN_GEOR_12X24, SDK_PRN_GEOR_16X32, SDK_PRN_GEOR_24X48},
            };

    font.uiAscFont = SDK_PRN_ASCII8X16;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = SDK_PRN_CH24X24;
    font.uiChZoom = SDK_PRN_ZOOM_N;

    sdkPrintInit();
    sdkSysMultiLangInit(SDK_SYS_LANG_FAR, 5);
    memset(KeyTab, 0, sizeof(KeyTab));
    sdkMmiMultiGetFarKeyPad(KeyTab);
    sdkPrintStr("Farsi print:5 kind style", font, SDK_PRINT_LEFTALIGN, 0, 1);
    for (i = 0; i < Not_Pin_Farsi_Font[0][0]; i++) {
        font.uiAscFont = Not_Pin_Farsi_Font[0][i + 1];
        for (k = 0; k < MULKEYNUM; k++) {
            memset(buf, 0, sizeof(buf));
            memcpy(buf, KeyTab[k], KeyTab[k][SDK_KEY_TAB_MAX_LEN - 1]);
            sdkPrintStr(buf, font, SDK_PRINT_LEFTALIGN, 0, 1);
        }
        sdkPrintStr("                     ", font, SDK_PRINT_LEFTALIGN, 0, 1);
    }
    sdkPrintStart();

    sdkPrintInit();
    font.uiAscFont = SDK_PRN_ASCII8X16;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = SDK_PRN_CH24X24;
    font.uiChZoom = SDK_PRN_ZOOM_N;
    memset(KeyTab, 0, sizeof(KeyTab));
    sdkSysMultiLangInit(SDK_SYS_LANG_RUS, 5);
    sdkMmiMultiGetRussianKeyPad(KeyTab);
    sdkPrintStr("Russian print:3 kind style", font, SDK_PRINT_LEFTALIGN, 0, 1);
    for (i = 0; i < Not_Pin_Farsi_Font[1][0]; i++) {
        font.uiAscFont = Not_Pin_Farsi_Font[1][i + 1];
        for (k = 0; k < MULKEYNUM; k++) {
            memset(buf, 0, sizeof(buf));
            memcpy(buf, KeyTab[k], KeyTab[k][SDK_KEY_TAB_MAX_LEN - 1]);
            sdkPrintStr(buf, font, SDK_PRINT_LEFTALIGN, 0, 1);
        }
        sdkPrintStr("                     ", font, SDK_PRINT_LEFTALIGN, 0, 1);
    }
    sdkPrintStart();

    sdkPrintInit();
    font.uiAscFont = SDK_PRN_ASCII8X16;
    font.uiAscZoom = SDK_PRN_ZOOM_N;
    font.uiChFont = SDK_PRN_CH24X24;
    font.uiChZoom = SDK_PRN_ZOOM_N;
    memset(KeyTab, 0, sizeof(KeyTab));
    sdkSysMultiLangInit(SDK_SYS_LANG_GER, 5);
    sdkMmiMultiGetGeorgiaKeyPad(KeyTab);
    sdkPrintStr("Georgia print:6 kind style", font, SDK_PRINT_LEFTALIGN, 0, 1);
    for (i = 0; i < Not_Pin_Farsi_Font[2][0]; i++) {
        font.uiAscFont = Not_Pin_Farsi_Font[2][i + 1];
        for (k = 0; k < MULKEYNUM; k++) {
            memset(buf, 0, sizeof(buf));
            memcpy(buf, KeyTab[k], KeyTab[k][SDK_KEY_TAB_MAX_LEN - 1]);
            sdkPrintStr(buf, font, SDK_PRINT_LEFTALIGN, 0, 1);
        }
        sdkPrintStr("                     ", font, SDK_PRINT_LEFTALIGN, 0, 1);
    }
    sdkPrintStart();
    sdkSysMultiLangInit(SDK_SYS_LANG_CHN, 5);

    return;
}

