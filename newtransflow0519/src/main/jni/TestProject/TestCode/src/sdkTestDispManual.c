/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkMathMaualTest.c
   �� �� ��   : ����
   ��    ��   : ���ߵ�
   ��������   : 2013��1��25��
   ����޸�   :
   ��������   : �ֶ�������ʾ��
   �����б�   :
   �޸���ʷ   :


******************************************************************************/

#include "sdkGlobal.h"
#include "SdkTest.h"


typedef struct Para_Info_FillRowRam {
    s32 siRow;
    s32 siColid;
    const u8 *pasStr;
    u32 ucAtr;
} Disp_Para;

typedef struct Para_Info_At        //zhongjiahong 2013.11.11 17:44
{
    s32 siX1;
    s32 siY1;
    s32 siX2;
    s32 siY2;
} Disp_At;

typedef struct Para_Info_XY   //zhongjiahong 2013.11.12 10:2
{
    s32 siX;
    s32 siY;
    s32 XLen;
    s32 YLen;
} Disp_XY;

typedef struct Para_Info_ProgBar    //zhongjiahong 2013.11.12 10:7
{
    s32 siRow;
    s32 siX1;
    s32 siX2;
    u8 Value;
} Disp_ProgBar;

typedef struct Para_Info_Box      //zhongjiahong 2013.11.12 11:28
{
    bool Flag;
    s32 OverTime;
    u8 *Content;
} Disp_Box;

typedef struct Para_Info_Label     //zhongjiahong 2013.11.12 11:41
{
    s32 StartLine;
    u8 *pasStr;
    u8 Format;
} Disp_Label;

typedef struct Para_Info_Msg {
    u8 *pasTitle;
    u8 *pasContent;
    s32 OverTime;
    s32 KeyMask;
} Disp_Msg;

typedef struct Para_Info_Prompt {
    s32 OverTime;
    s32 StartLine;
    s32 EndLine;
    u8 *pasStr;
    s32 Format;
} Disp_Prompt;

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siRow
                               s32 siColid
                               const u8 *pasStr
                               u32 ucAtr
                               char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.11.11
** Remarks:
*****************************************************************************/
static s32 sdkDispFillRowRam_Para_Info(s32 siRow, s32 siColid, const u8 *pasStr, u32 ucAtr,
                                       char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;

    sdkDispClearScreen();
    ret = sdkDispFillRowRam(siRow, siColid, pasStr, ucAtr);
    sdkDispBrushScreen();

    if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
        return SDK_KEY_ESC;
    }
    sprintf(pasOutInfo, "sdkDispFillRowRam(%d, %d, %s, %d)\r\nret = %d\r\n", siRow, siColid, pasStr,
            ucAtr, ret);

    ret = sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
    return ret;
}

static s32 Test_sdkDispFillRowRamManual(char *pasOutInfo, char const *pTitle) {
    int i = 0;

    Disp_Para para[] = {
            {0, 0,  "��һ�в���test!",  SDK_DISP_FDISP},
            {1, 16, "�ڶ��в���test!",  SDK_DISP_FDISP},
            {2, 0,  "�����в���test!", SDK_DISP_NOFDISP | SDK_DISP_CDISP},
            {3, 0,  "�����в���test!", SDK_DISP_RDISP},
            {4, 0,  "�����в���test!", SDK_DISP_RDISP}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        if (SDK_KEY_ESC ==
            sdkDispFillRowRam_Para_Info(para[i].siRow, para[i].siColid, para[i].pasStr,
                                        para[i].ucAtr, pasOutInfo, pTitle)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siX
                               s32 siY
                               const u8 *pasStr
                               char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.11.11
** Remarks:
*****************************************************************************/
static s32 sdkDispAt_Para_Info(s32 siX, s32 siY, const u8 *pasStr, char *pasOutInfo,
                               char const *pTitle) {
    s32 ret = 0;

    sdkDispClearScreen();
    ret = sdkDispAt(siX, siY, pasStr);
    sdkDispBrushScreen();

    if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
        return SDK_KEY_ESC;
    }
    sprintf(pasOutInfo, "sdkDispAt(%d,%d,%s)\r\nret=%d\r\n", siX, siY, pasStr, ret);

    ret = sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
    return ret;
}

static s32 Test_sdkDispAtManual(char *pasOutInfo, char const *pTitle) {
    int i = 0;

    Disp_Para para[] = {
            {0,  0,  "disp test!", 7},
            {64, 16, "disp test!", 7},
            {32, 16, "disp test!", 7},
            {0,  32, "disp test!", 7},
            {64, 52, "disp test!", 7}
    };

    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        if (SDK_KEY_ESC ==
            sdkDispAt_Para_Info(para[i].siRow, para[i].siColid, para[i].pasStr, pasOutInfo,
                                pTitle)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

void FillAllScreen(void) //shijianglong 2013.04.09 15:3
{

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "                       ",
                      SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "                       ",
                      SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "                       ",
                      SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "                       ",
                      SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "                       ",
                      SDK_DISP_NOFDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);

    sdkDispBrushScreen();
}

static s32 IN_Test_sdkDispSaveScreen(u8 *pbuf) {
    s32 ret = 0;

    sdkDispClearScreen();

    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ȷ�ϼ�������Ļ", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ȷ�ϼ�������Ļ", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�˰���˵�����ڱ�", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��֮ǰ��Члл!", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "�ڻָ�ʱ����ȷ��", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sdkDispSaveScreen((const u16 *) pbuf,
                      sdkDispGetScreenBufferSize()); //shijianglong 2013.05.06 13:56
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ļ�ѱ���", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ȷ�ϼ��ָ���Ļ", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sdkDispClearScreen();
    ret = sdkDispRestoreScreen((const u16 *) pbuf,
                               sdkDispGetScreenBufferSize()); //shijianglong 2013.05.06 13:58
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    return ret;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdksdkDispClearScreenManual(char *pasOutInfo, char const *pTitle) {
    FillAllScreen();
    sdkDispClearRowRam(SDK_DISP_LINE1);
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��������", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sdkDispClearScreen();
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sprintf(pasOutInfo, "���Գɹ�"); //shijianglong 2013.05.17 17:13
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siRow
                               char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		zhongjiahong  2013.11.11
** Remarks:
*****************************************************************************/
static s32 sdkDispClearRow_Para_Info(s32 siRow, char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;

    ret = sdkDispClearRow(siRow);

    if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
        return SDK_KEY_ESC;
    }
    sprintf(pasOutInfo, "sdkDispClearRow(%d)\r\nret = %d\r\n", siRow, ret);

    ret = sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
    return ret;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispClearRowManual(char *pasOutInfo, char const *pTitle) {
    int i;
    s32 siRow[] = {0, 1, 2, 3, 4};

    for (i = 0; i < sizeof(siRow) / sizeof(siRow[0]); i++) {
        FillAllScreen();

        if (siRow[i] != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

        if (siRow[i] != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }

        if (SDK_KEY_ESC == sdkDispClearRow_Para_Info(siRow[i], pasOutInfo, pTitle)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispClearAtManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_At para[] = {
            {0,  0, 58,  28},
            {0,  0, 58,  60},
            {58, 0, 122, 28},
            {58, 0, 122, 60},
            {0,  0, 122, 60}
    };

    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        FillAllScreen();

        if (para[i].siY1 > 13) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�������ο����", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "�������ο����", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
        ret = sdkDispClearAt(para[i].siX1, para[i].siY1, para[i].siX2, para[i].siY2);

        if (para[i].siY1 > 13) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�������ο���ȷ��", SDK_DISP_DEFAULT);
        }
        else if (para[i].siY2 < 52) {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "�������ο���ȷ��", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispClearAt(%d,%d,%d,%d)\r\nret = %d\r\n", para[i].siX1,
                para[i].siY1, para[i].siX2, para[i].siY2, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

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
static s32 Test_sdkDispSetBackgroundManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_At para[] = {
            {0, 0, 58, 28}
    };
#if SDK_IS_DDI_PLAM()
    u8 param[5][100] = {"/mtd1/logo1.bmp"};
#else
    u8 param[5][100] = {"/mtd0/res/background.bmp"};
#endif


    if (!sdkSysIsColorScreen()) {
        sprintf(pasOutInfo, "�˺���ֻ֧�ֲ�������֧�ִ˻���");
        return SDK_OK;
    }

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        TestLogHex("sjl", "-=-=-=-=-=-=--=-=-=-=is", param, strlen(param));

        sdkDispClearScreen();

        if (para[i].siY1 > 13) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "���ͼƬ����", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "���ͼƬ����", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
        sdkDispClearScreen(); //shijianglong 2013.05.17 15:22
        ret = sdkDispSetBackground(para[i].siX1, para[i].siY1, para[i].siX2, para[i].siY2,
                                   param[i]);

        if (para[i].siY1 > 13) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "���ͼƬ��ȷ��", SDK_DISP_DEFAULT);
        }
        else if (para[i].siY2 < 52) {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "���ͼƬ��ȷ��", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();

        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
        sdkDispClearScreen();
        sdkDispSetBackground(para[i].siX1, para[i].siY1, para[i].siX2, para[i].siY2,
                             NULL); //shijianglong 2013.05.17 15:46
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ͼƬ�Ѿ����", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispSetBackground(%d,%d,%d,%d,%s)\r\nret = %d\r\n", para[i].siX1,
                para[i].siY1, para[i].siX2, para[i].siY2, param[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

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
static s32 Test_sdkDispShowBmpManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    SDK_DISP_PIXEL ScreenPixel = sdkDispGetScreenPixel();
    Disp_XY para[] = {
            {0, 0, 58, 28},
            {0, 0, 58, 28}
    };
#if MACHINEMODEL == SDK_SYS_MACHINE_G2
    u8 param[][100] = {"/mtd1/logo1.bmp", "/mtd1/logo2.bmp"};
#elif MACHINEMODEL == SDK_SYS_MACHINE_200P
    u8 param[][100] = {"/mtd1/logo0.bmp", "/mtd1/logo0.bmp"};
#else       
    u8 param[][100] = {"/mtd0/res/background.bmp", "/mtd0/res/cncupprintlogo.bmp"};
#endif


#if 0 /*Modify by zhouzhihua at 2013.07.16  17:20 */

    if(sdkDispMsgBox("1.�� 2. �� ʹ��Ĭ��ͼƬ��ʾ", param6, 0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2) == SDK_KEY_1)
    {
        sdkTestInputASC(param6, "�����ӡͼƬ����·��", "����·��", 1, sizeof(param6) - 1);
    }
    else
    {
        sdkTestInputASC(param5, pTitle, "������ͼƬ����(·��/mtd0/res/��·����������)", 1, 100);
        strcat(param6, "/mtd0/res/");
        strcat(param6, param5); //shijianglong 2013.05.17 15:3
    }
#endif /* if 0 */
    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        TestLogHex("sjl", "-=-=-=-=-=-=--=-=-=-=is", param[i], strlen(param[i]));

        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�̶�λ�����ͼƬȷ��", SDK_DISP_DEFAULT);
        ret = sdkDispShowBmp(para[i].siX, para[i].siY, para[i].XLen, para[i].YLen,
                             param[i]); //shijianglong 2013.05.31 10:27
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispShowBmp(%d,%d,%d,%d,%s)\r\nret = %d\r\n", para[i].siX,
                para[i].siY, para[i].XLen, para[i].YLen, param[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

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
static s32 Test_sdkDispDrawProgbarManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    SDK_DISP_PIXEL ScreenPixel = sdkDispGetScreenPixel();
    Disp_ProgBar para[] = {
            {0, 0,  58,  50},
            {1, 58, 122, 50},
            {2, 25, 102, 25},
            {3, 0,  122, 50},
            {4, 0,  122, 100}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����������ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����������ȷ��", SDK_DISP_DEFAULT);
        }
        ret = sdkDispDrawProgbar(para[i].siRow, para[i].siX1, para[i].siX2, para[i].Value);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispDrawProgbar(%d,%d,%d,%d)\r\nret = %d\r\n", para[i].siRow,
                para[i].siX1, para[i].siX2, para[i].Value, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

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
static s32 Test_sdkDisGetScreenPixelManual(char *pasOutInfo, char const *pTitle) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();

    sprintf(pasOutInfo, "return1:%d \rreturn2:%d", pixel.siX, pixel.siY);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispRowManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_Para para[] = {
            {0, 0,  "��һ�в���test!",  SDK_DISP_FDISP},
            {1, 16, "�ڶ��в���test!",  SDK_DISP_FDISP},
            {2, 0,  "�����в���test!", SDK_DISP_NOFDISP | SDK_DISP_CDISP},
            {3, 0,  "�����в���test!", SDK_DISP_RDISP},
            {4, 0,  "�����в���test!", SDK_DISP_RDISP}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();
        //FillAllScreen();

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ʾ��ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��ʾ��ȷ��", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        ret = sdkDispRow(para[i].siRow, para[i].siColid, para[i].pasStr, para[i].ucAtr);

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispRow(%d,%d,%s,%d)\r\nret = %d\r\n", para[i].siRow,
                para[i].siColid, para[i].pasStr, para[i].ucAtr, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispClearRowRamManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    s32 siRow[] = {0, 1, 2, 3, 4};

    for (i = 0; i < sizeof(siRow) / sizeof(siRow[0]); i++) {
        sdkDispClearScreen();

        FillAllScreen();

        if (siRow[i] != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ�в���", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

        if (siRow[i] != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ��ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ��ȷ��", SDK_DISP_DEFAULT);
        }
        ret = sdkDispClearRowRam(siRow[i]);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispClearRowRam(%d)\r\nret = %d\r\n", siRow[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispClearRowRamAtManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_ProgBar para[] = {
            {0, 0, 25,  0},
            {1, 0, 50,  0},
            {2, 0, 76,  0},
            {3, 0, 102, 0},
            {4, 0, 122, 0}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        FillAllScreen();

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ�в��ֲ���", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ�в��ֲ���", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispClearRowRam(SDK_DISP_LINE1);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����һ�в���ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispClearRowRam(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����һ�в���ȷ��", SDK_DISP_DEFAULT);
        }
        ret = sdkDispClearRowRamAt(para[i].siRow, para[i].siX1, para[i].siX2);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispClearRowRamAt(%d,%d,%d)\r\nret = %d\r\n", para[i].siRow,
                para[i].siX1, para[i].siX2, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispSaveScreen(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;

    u16 *buf = (u16 *) sdkGetMem(
            (s32) sdkDispGetScreenBufferSize()); //shijianglong 2013.05.06 14:53

    if (buf == NULL) {
        printf("1023����ʧ��\r\n");
    }
    ret = IN_Test_sdkDispSaveScreen((u8 *) buf); //shijianglong 2013.05.06 14:50


    sdkFreeMem(buf);
    buf = NULL;

#if 0 /*Modify by huacong at 2013.03.30  16:10 */
    sdkDispClearScreen();

    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ȷ�ϼ�������Ļ", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ȷ�ϼ�������Ļ", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�˰���˵�����ڱ�", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��֮ǰ��Члл!", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "�ڻָ�ʱ����ȷ��", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sdkDispSaveScreen(buf, sdkDispGetScreenBufferSize());
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ļ�ѱ���", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ȷ�ϼ��ָ���Ļ", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
    sdkDispClearScreen();
    ret = sdkDispRestoreScreen(buf, sdkDispGetScreenBufferSize());
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

    if(sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM) //huacong 2013.03.28 11:48
    {
        sdkFreeMem(buf);
    }
#endif /* if 0 */

    sprintf(pasOutInfo, "\rreturn:%d", ret);
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
static s32 Test_sdkDispGetScreenBufferSizeManual(char *pasOutInfo, char const *pTitle) {
    u32 LcdScreenSize;

    LcdScreenSize = sdkDispGetScreenBufferSize();

    sprintf(pasOutInfo, "return1:%d ", LcdScreenSize);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispRowAtManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_Para para[] = {

            {0, 0,  "��һ�в���test!",  SDK_DISP_FDISP},
            {1, 16, "�ڶ��в���test!",  SDK_DISP_FDISP},
            {2, 0,  "�����в���test!", SDK_DISP_NOFDISP | SDK_DISP_CDISP},
            {3, 0,  "�����в���test!", SDK_DISP_RDISP},
            {4, 0,  "�����в���test!", SDK_DISP_RDISP}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ʾ��ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��ʾ��ȷ��", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();

        //sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
        ret = sdkDispRowAt(para[i].siRow, para[i].siColid, para[i].pasStr, para[i].ucAtr);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispRowAt(%d,%d,%s,%d)\r\nret = %d\r\n", para[i].siRow,
                para[i].siColid, para[i].pasStr, para[i].ucAtr, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

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
static s32 Test_sdkDispDrawProgbarRamManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_ProgBar para[] = {
            {0, 0,  58,  50},
            {1, 58, 122, 50},
            {2, 25, 102, 25},
            {3, 0,  122, 50},
            {4, 0,  122, 100}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();
        //    FillAllScreen();

        if (para[i].siRow != SDK_DISP_LINE1) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����������ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "����������ȷ��", SDK_DISP_DEFAULT);
        }
        ret = sdkDispDrawProgbarRam(para[i].siRow, para[i].siX1, para[i].siX2, para[i].Value);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispDrawProgbarRam(%d,%d,%d,%d)\r\n ret = %d\r\n", para[i].siRow,
                para[i].siX1, para[i].siX2, para[i].Value, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	Test_sdkDispFillRowRamManual
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispDrawLineManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_At para[] = {

            {0,  28, 122, 28},
            {64, 0,  64,  60},
            {0,  0,  122, 60},
            {0,  60, 122, 0}
    };
    s32 Colour[] = {1, 1, 1, 1};

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();

        if (Colour[i] == 0) //shijianglong 2013.05.17 18:6
        {
            FillAllScreen();
        }
        ret = sdkDispDrawLine(para[i].siX1, para[i].siY1, para[i].siX2, para[i].siY2, Colour[i]);

        if (para[i].siY2 > 14) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��һ������ȷ��", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��һ������ȷ��", SDK_DISP_DEFAULT);
        }
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispDrawLine(%d,%d,%d,%d,%d)\r\nret = %d\r\n", para[i].siX1,
                para[i].siY1, para[i].siX2, para[i].siY2, Colour[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

static s32 Test_sdkDispDrawPixelManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;

    Disp_At para[] = {
            {25,  13, 0, 0},
            {50,  25, 0, 0},
            {64,  32, 0, 0},
            {100, 50, 0, 0},
            {115, 57, 0, 0}
    };
    s32 Colour[] = {1, 1, 1, 1, 1};

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();

        if (para[i].siY1 > 14) {
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ȷ��������λ����ɫ", SDK_DISP_DEFAULT);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��ȷ��������λ����ɫ", SDK_DISP_DEFAULT);
        }
        ret = sdkDispPutPixel(para[i].siX1, para[i].siY1, Colour[i]);
        sdkDispBrushScreen();

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispPutPixel(%d,%d,%d)\r\nret = %d\r\n", para[i].siX1, para[i].siY1,
                Colour[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}


#define SDK_TEST_RGB(ired, igreen, iblue)    (  (((u8)ired)<<16) | (((u8)igreen)<<8) | ((u8)iblue)  )
#if 0
static s32 Test_sdkDispDrawRect(char *pasOutInfo, char const*pTitle)
{
    s32 ret;
    int i;
    u32 isfullColor = 0;
    u8 tmpstr[128] = {0};
    SDK_DISP_SRECT para[] = {
        {0, 28, 122, 40},
        {64, 25, 120, 60},
        {0, 27, 122, 60},
        {0, 40, 122, 60}
    };
    u32 Color[] = {SDK_TEST_RGB(255, 0, 0), SDK_TEST_RGB(0, 255, 0), SDK_TEST_RGB(0, 0, 150), SDK_TEST_RGB(200, 200, 200) };//�죬�̣�������ǳ��
    u8 colorName[4][5] = {"��","��","����","ǳ��"};
    

    for(i = 0; i < sizeof(para) / sizeof(para[0]); i++)
    {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "������", SDK_DISP_DEFAULT);
        memset( tmpstr, 0, sizeof(tmpstr) );
        sprintf(tmpstr, "����:(%d,%d,%d,%d)\r\n",  para[i].x0, para[i].y0, para[i].x1, para[i].y1 );
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, tmpstr, SDK_DISP_LEFT_DEFAULT);
        if( isfullColor < 4 )
        {
            memset( tmpstr, 0, sizeof(tmpstr) );
            sprintf(tmpstr, "�����ɫ:(%s)\r\n",  colorName[i]);        
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, tmpstr, SDK_DISP_LEFT_DEFAULT);
        }
        else
        {  
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�������ɫ", SDK_DISP_LEFT_DEFAULT);        
        }
        isfullColor++;
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "���������ʼ��", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        if(SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ALL, 0))
        {
            return SDK_OK;
        }
        sdkDispClearScreen();
        ret = sdkDispDrawRect( para + i, isfullColor < 5 ? true:false, Color[i] );
        if(SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0))
        {
            return SDK_OK;
        }        
        sprintf(pasOutInfo, "sdkDispDrawRect(%d,%d,%d,%d,%s,%s)\r\nret = %d\r\n",  
        para[i].x0, para[i].y0, para[i].x1, para[i].y1,isfullColor < 5 ? "true":"false", colorName[i],  ret);

        if ( SDK_KEY_ESC == sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC))
        {
            return SDK_OK;
        }
        if( isfullColor == 4 )
        {
            i = 0;
        }
    }

    return SDK_OK;
}
#endif

static s32 Test_sdkDispSetRectColor(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    u8 tmpstr[128] = {0};
    SDK_DISP_SRECT para[] = {
            {0,  28, 122, 40},
            {64, 25, 120, 60},
            {0,  27, 122, 60},
            {0,  40, 122, 60}
    };
    u32 Color[] = {SDK_TEST_RGB(255, 0, 0), SDK_TEST_RGB(0, 255, 0), SDK_TEST_RGB(0, 0, 150),
                   SDK_TEST_RGB(200, 200, 200)};//�죬�̣�������ǳ��
    u8 colorName[4][5] = {"��", "��", "����", "ǳ��"};


    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        sdkDispClearScreen();

        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "������", SDK_DISP_DEFAULT);
        memset(tmpstr, 0, sizeof(tmpstr));
        sprintf(tmpstr, "����:(%d,%d,%d,%d)\r\n", para[i].x0, para[i].y0, para[i].x1, para[i].y1);

        sdkDispFillRowRam(SDK_DISP_LINE2, 0, tmpstr, SDK_DISP_LEFT_DEFAULT);
        memset(tmpstr, 0, sizeof(tmpstr));
        sprintf(tmpstr, "�����ɫ:(%s)\r\n", colorName[i]);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, tmpstr, SDK_DISP_LEFT_DEFAULT);

        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "���������ʼ���", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ALL, 0)) {
            return SDK_OK;
        }
        sdkDispClearScreen();
        ret = sdkDispSetRectColor(para + i, Color[i]);
        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispSetRectColor(%d,%d,%d,%d,%s)\r\nret = %d\r\n",
                para[i].x0, para[i].y0, para[i].x1, para[i].y1, colorName[i], ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}


/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispPromptBoxManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Box para[] = {
            {0, 0,    "��һ�β��Բ�������������ʱ��DispTest!+++DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++��һ�β��Բ�������������ʱ��DispTest!"},
            {0, 3000, "�ڶ��β��Գ�ʱʱ��3�룬DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++�����β��Գ�ʱʱ��3�룬DispTest!"},
            {0, 0,    "���Ĵβ��Բ���ʱ���м��л��У�DispTest+++DispTest+++DispTest+++DispTest+++DispTest+++DispTest�˴�����\n+++DispTest+++�����β��Գ�ʱʱ��3�룬l have a dream that one day! this nation will rise up, and live out the true meaning of its creates,we hold this truth to be self avdients, that all man are created equels!l have a dream that one day! this nation will rise up, and live out the true meaning of its creates,we hold this truth to be self avdients, that all man are created equels!l have a dream that one day! this nation will rise up, and live out the true meaning of its creates,we hold this truth to be self avdients, that all man are created equels!l have a dream that one day! this nation will rise up, and live out the true meaning of its creates,we hold this truth to be self avdients, that all man are created equels!"}

    };
    sdkDispMsgBox("����Ļ�ָ���Ļ����",
                  "1.�Զ����� \r2.�ֶ����� \r3.ϵͳ����\r4.�汾��Ϣ\r5.��ʱ����\r6������Ϣ����", 0,
                  SDK_KEY_MASK_ALL);
    sdkDispPromptBox(1, 0, "��һ������Ϊ֮ǰ�Ľ���");
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //   ConfirmStatTest("?????????");
        //sdkDispClearScreen();
        ret = sdkDispPromptBox(para[i].Flag, para[i].OverTime, para[i].Content);

        sprintf(pasOutInfo, "sdkDispPromptBox(%d,%d,%s)\r\nret = %d\r\n", para[i].Flag,
                para[i].OverTime, para[i].Content, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispLabelManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Label para[] = {
            {0, "DispLable,����",                                                     0},
            {1, "DispLable���ԣ�����",                                                 0},
            {2, "DispLable���ԣ�����",                                                 0},
            {3, "DispLable���ԣ����ԣ����в���ҳ+++++++++���Ĵ�,�ټ�һ��",                   1},
            {4, "DispLable���ԣ����ԣ����в���ҳ+++++++++����Σ������ӳ�һ������",           1},
            {1, "DispLable���ԣ����ԣ��м��л���+++++++++�����Σ��˴�����\n+++DispTest+++", 1}

    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //   ConfirmStatTest("����֤��ʾ���");
        sdkDispClearScreen();
        ret = sdkDispLabel(para[i].StartLine, para[i].pasStr, para[i].Format);

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispLabel(%d,%s,%d)\r\nret = %d\r\n", para[i].StartLine,
                para[i].pasStr, para[i].Format, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispMsgBoxManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Msg para[] = {
            {"��������", "�밴0��,Ȼ��ȷ��", 0, SDK_KEY_MASK_0},
            {"��������", "�밴1��,Ȼ��ȷ��", 0, SDK_KEY_MASK_1},
            {"��������", "�밴2��,Ȼ��ȷ��", 0, SDK_KEY_MASK_2},
            {"��������", "�밴3��,Ȼ��ȷ��", 0, SDK_KEY_MASK_3},
            {"��������", "�밴4��,Ȼ��ȷ��", 0, SDK_KEY_MASK_4},
            {"��������", "�밴5��,Ȼ��ȷ��", 0, SDK_KEY_MASK_5},
            {"��������", "�밴6��,Ȼ��ȷ��", 0, SDK_KEY_MASK_6},
            {"��������", "�밴7��,Ȼ��ȷ��", 0, SDK_KEY_MASK_7},
            {"��������", "�밴8��,Ȼ��ȷ��", 0, SDK_KEY_MASK_8},
            {"��������", "�밴9��,Ȼ��ȷ�ϣ����ݳ������м��з��У���������+++�������ݣ��˴�����\n+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������,�˴��ٷ���\r��+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������+++��������", 0, SDK_KEY_MASK_9}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //     ConfirmStatTest("��ȷ����ʾ����");
        sdkDispClearScreen();
        ret = sdkDispMsgBox(para[i].pasTitle, para[i].pasContent, para[i].OverTime,
                            para[i].KeyMask);

        if (SDK_KEY_ESC == sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0)) {
            return SDK_OK;
        }
        sprintf(pasOutInfo, "sdkDispMsgBox(%s,%s,%d,%d)\r\nret = %d\r\n", para[i].pasTitle,
                para[i].pasContent, para[i].OverTime, para[i].KeyMask, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispPromptLabelManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Prompt para[] = {
            {0,    0, 0, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    1, 1, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    2, 2, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    3, 3, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    4, 4, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    0, 1, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    0, 2, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    0, 3, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    0, 4, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    1, 2, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    2, 3, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    3, 4, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    2, 3, "l have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,we hold \r\nthis truth to be avdients\r\nthat all man\r\n are created equels!", 0},
            {0,    3, 4, "\r\nl have a dream\r\nthat one day!\r\nthis nation will rise up,\r\nand live out the\r\n true meaning of its\r\n creates,that all man\r\n are created equels!", 0},
            {3000, 0, 1, "??3??+++++++++++++++++++++++++++++++++++++++", 0}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //     ConfirmStatTest("����ȷ��ȡ������Ч");
        sdkDispClearScreen();
        ret = sdkDispPromptLabel(para[i].OverTime, para[i].StartLine, para[i].EndLine,
                                 para[i].pasStr, para[i].Format);

        sprintf(pasOutInfo, "sdkDispPromptLabel(%d,%d,%d,%s,%d)\r\nret = %d\r\n", para[i].OverTime,
                para[i].StartLine, para[i].EndLine, para[i].pasStr, para[i].Format, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispDrawDotManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Prompt para[] = {
            {0,  0,  1},
            {10, 10, 1},
            {30, 30, 1},
            {40, 40, 1},
            {50, 50, 1}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //     ConfirmStatTest("����ȷ��ȡ������Ч");
        sdkDispClearScreen();
        ret = sdkDispDrawDot(para[i].OverTime, para[i].StartLine, para[i].EndLine);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
        sprintf(pasOutInfo, "sdkDispDrawDot(%d, %d, %d)\r\nret = %d\r\n", para[i].OverTime,
                para[i].StartLine, para[i].EndLine, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(NULL, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            return SDK_OK;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		wanggaodeng  2013.01.31
** Remarks:
*****************************************************************************/
static s32 Test_sdkDispDrawDotRamManual(char *pasOutInfo, char const *pTitle) {
    s32 ret;
    int i;
    Disp_Prompt para[] = {
            {0,  0,  1},
            {10, 10, 1},
            {30, 30, 1},
            {40, 40, 1},
            {50, 50, 1}
    };

    for (i = 0; i < sizeof(para) / sizeof(para[0]); i++) {
        //     ConfirmStatTest("����ȷ��ȡ������Ч");
        sdkDispClearScreen();
        ret = sdkDispDrawDotRam(para[i].OverTime, para[i].StartLine, para[i].EndLine);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
        sprintf(pasOutInfo, "sdkDispDrawDot(%d, %d, %d)\r\nret = %d\r\n", para[i].OverTime,
                para[i].StartLine, para[i].EndLine, ret);

        if (SDK_KEY_ESC ==
            sdkDispMsgBox(pTitle, pasOutInfo, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC)) {
            break;
        }
    }
    sprintf(pasOutInfo, "sdkDispDrawDotRam����");

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2013.09.03
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkDispFormatFillRamManual(char *pasOutInfo, char const *pTitle) {
    u8 i = 0;
    SDK_DISP_MULT_DATA stDispData[3];
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);


    sdkDispClearScreen();

    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
    stDispData[0].eChFont = SDK_PRN_CH16X16;
    stDispData[0].siX = 0;
    stDispData[0].siY = 0;
    strcpy(stDispData[0].pasText, "ԭ����:");


    stDispData[1].eAscFont = SDK_PRN_ASCII6X12;
    stDispData[1].eChFont = SDK_PRN_CH16X16;
    stDispData[1].siX = 54;
    stDispData[1].siY = 0;
    strcpy(stDispData[1].pasText, "���ѳ���");
    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);
//____________________________________________________________
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[0].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[0].eChFont = SDK_PRN_CH16X16;
    stDispData[0].siX = 0;
    stDispData[0].siY = 16;
    strcpy(stDispData[0].pasText, "622500******0113");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0], NULL);
//____________________________________________________________
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[0].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[0].eChFont = SDK_PRN_CH16X16;
    stDispData[0].siX = 0;
    stDispData[0].siY = 28;
    strcpy(stDispData[0].pasText, "VOUCHER:");

#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[1].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[1].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[1].eChFont = SDK_PRN_CH16X16;
    stDispData[1].siX = 40;
    stDispData[1].siY = 28;
    strcpy(stDispData[1].pasText, "660198");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);
//____________________________________________________________
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[0].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[0].eChFont = SDK_PRN_CH16X16;
    stDispData[0].siX = 0;
    stDispData[0].siY = 40;
    strcpy(stDispData[0].pasText, "AMOUNT:");

#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[1].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[1].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[1].eChFont = SDK_PRN_CH16X16;
    stDispData[1].siX = 35;
    stDispData[1].siY = 40;
    strcpy(stDispData[1].pasText, "1.11");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);
//____________________________________________________________
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[0].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[0].eChFont = SDK_PRN_CH16X16;
    stDispData[0].siX = 0;
    stDispData[0].siY = 52;
    strcpy(stDispData[0].pasText, "REF. NO:");

#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.07.04  15:42 */
    stDispData[1].eAscFont = SDK_PRN_ASCII6X12;
#else
    stDispData[1].eAscFont = SDK_PRN_ASCII5X7;
#endif /* if 0 */
    stDispData[1].eChFont = SDK_PRN_CH16X16;
    stDispData[1].siX = 40;
    stDispData[1].siY = 52;
    strcpy(stDispData[1].pasText, "100505070023");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);

    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    sprintf(pasOutInfo, "sdkDispDrawDotRam����");

    return SDK_OK;
}

static s32 Test_sdkDispIcon(char *pasOutInfo, char const *pTitle) {
    u32 screenWidht = 0;
    u32 i = 0;
    u32 iconWidth = 0;
    u32 timerID = sdkTimerGetId();
    sdkDispClearScreen();
    sdkSysGetScreenWidthHigh(&screenWidht, NULL);
    sdkDispStateBar(true);
    sdkBtOpen();
    sdkBtSetDiscoverable(true);
    while (1) {
        if (sdkTimerIsEnd(timerID, 700)) {
            i = 0;
            iconWidth = (screenWidht - 4) / 4;
            sdkDispClearStateBar();
            sdkDispIcon(SDK_DISP_BT, 0, 2 + iconWidth * i++, 1);
            sdkDispIcon(SDK_DISP_USB, 0, 2 + iconWidth * i++, 1);
            sdkDispIcon(SDK_DISP_BATTERY, 0, 2 + iconWidth * i++, 1);
            sdkDispIcon(SDK_DISP_AUDIO, 0, 2 + iconWidth * i++, 1);
            timerID = sdkTimerGetId();
        }
        if (SDK_KEY_ESC == sdkKbGetKey()) {
            sprintf(pasOutInfo, "sdkDispIcon����");
            sdkBtClose();
            return SDK_OK;
        }
    }
}

static const TEST_MANUAL_MAP DispManuaMap[] =
        {
                {"sdkDispFillRowRam", Test_sdkDispFillRowRamManual},
                {"sdkDispAt", Test_sdkDispAtManual},
                {"sdksdkDispClearScreen", Test_sdksdkDispClearScreenManual},
                {"sdkDispClearRow", Test_sdkDispClearRowManual},
                {"sdkDispClearAt", Test_sdkDispClearAtManual},
                {"sdkDispSetBackground", Test_sdkDispSetBackgroundManual},
                {"sdkDispShowBmp", Test_sdkDispShowBmpManual},
                {"sdkDispDrawProgbar", Test_sdkDispDrawProgbarManual},
                {"sdkDisGetScreenPixel", Test_sdkDisGetScreenPixelManual},
                {"sdkDispRow", Test_sdkDispRowManual},
                {"sdkDispClearRowRam", Test_sdkDispClearRowRamManual},
#if (MACHINEMODEL != SDK_SYS_MACHINE_G2)
                {"sdkDispSaveScreen", Test_sdkDispSaveScreen},     //������
#endif
                {"sdkDispGetScreenBufferSize", Test_sdkDispGetScreenBufferSizeManual},
                {"sdkDispRowAt", Test_sdkDispRowAtManual},
                {"sdkDispDrawProgbarRam", Test_sdkDispDrawProgbarRamManual},
                {"sdkDispDrawLine", Test_sdkDispDrawLineManual},
                {"sdkDispDrawPixel", Test_sdkDispDrawPixelManual},
                //{"sdkDispDrawRect", Test_sdkDispDrawRect},
                {"sdkDispSetRectColor", Test_sdkDispSetRectColor},
                {"sdkDispClearRowRamAt", Test_sdkDispClearRowRamAtManual},
#if (SDK_IS_DDI_PLAM())
                //{"sdkDispCombText", Test_sdkDispCombText},
#endif
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P
                {"sdkDispDrawDot", Test_sdkDispDrawDotManual},
                {"sdkDispDrawDotRam", Test_sdkDispDrawDotRamManual},
#endif
                {"sdkDispFormatFillRam", Test_sdkDispFormatFillRamManual},
                {"sdkDispPromptBox", Test_sdkDispPromptBoxManual},
                {"sdkDispLabel", Test_sdkDispLabelManual},
                {"sdkDispMsgBox", Test_sdkDispMsgBoxManual},

                {"sdkDispPromptLabel", Test_sdkDispPromptLabelManual},
                {"sdkDispIcon", Test_sdkDispIcon},

        };


TEST_MAP_INFO GetDispManualTestInfo(void) {
    TEST_MAP_INFO ret;

    ret.size = sizeof(DispManuaMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = DispManuaMap;
    return ret;
}

