#include "sdkGlobal.h"
#include "SdkTest.h"

static s32 Test_sdkPrintInitManual(char *pasOutInfo, char const *pTitle) {
    if (sdkSysIsHaveBase())//�ж��Ƿ������
    {
        sdkTestMsgBox("��ע��˻����е�����ע������޵������������ȷ�ϼ���ʼ����", SDK_KEY_MASK_ENTER);
    }//huacong 2013.01.31 14:10 ���ӵ������ж�
    sprintf(pasOutInfo, "return(ret):%d\r\n", sdkPrintInit());
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:		char *pasOutInfo
					char const*pTitle
** Returned value:
** Created By:		huacong  2013.01.28
** Remarks:
*****************************************************************************/
static s32 Test_sdkPrintNeedleFeedPaperManual(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    s32 derection = sdkTestInputs32(pTitle, "���뷽��(0 ǰ�� 1����):");

    /*=======BEGIN: wangmingming 2014.06.07  8:45 modify===========*/
    if (1 == derection) {
        sdkDispMsgBox("������Ϣ", "�˻���֧����ֽ", 2000, 0xFFFF);
        //sprintf(pasOutInfo,"input1(RollDirection):%d \rinput2(uDot):%d \rreturn:%d",derection,udots,ret);
        return SDK_OK;
    }
    else {
        u8 udots = sdkTestInputU8(pTitle, "Input 2(����):");

        ret = sdkPrintInit();
        if (SDK_OK == ret) {
            ret = sdkPrintFeedPaper((SDK_PRINT_ROLL_DIRECTION) derection, udots);
        }

        sprintf(pasOutInfo, "input1(RollDirection):%d \rinput2(uDot):%d \rreturn:%d", derection,
                udots, ret);

        return ret;
    }
    /*====================== END======================== */
}

/*****************************************************************************
** Descriptions:
** Parameters:		char *pasOutInfo
					char const*pTitle
** Returned value:
** Created By:		huacong  2013.01.28
** Remarks:
*****************************************************************************/
static s32 Test_sdkPrintBitMapManual(char *pasOutInfo, char const *pTitle) {
    s32 ret2 = 0;
    u8 picture[128] = {0};
    s32 eAlign = sdkTestInputs32(pTitle, "0:�� 1:��  2:�� Align:");
    u32 Offset = sdkTestInputU32(pTitle, "Input(ƫ��):");

    if (sdkTestIsUcos())//huacong 2013.03.28 11:48
    {
        strcpy(picture, "LOGO0.BMP");
    }
    else {
        strcpy(picture, "/mtd1/logo0.bmp");
    }

    if (sdkDispMsgBox("1.�� 2. �� ʹ��Ĭ��ͼƬ��ӡ", picture, 0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2) ==
        SDK_KEY_2) {
        sdkTestInputASC(picture, "�����ӡͼƬ����·��", "����·��", 1, sizeof(picture) - 1);
    }

    ret2 = sdkPrintInit();
    if (SDK_OK == ret2) {
        sdkPrintBitMap(picture, (SDK_PRINT_ALIGNMODE) eAlign, Offset);
        ret2 = sdkPrintStart();
    }

    //Trace("print","sdkPrintStart ret2=%d\r\n", ret2);
    sprintf(pasOutInfo, "picture: %s \rinput1(eAlign):%d \rinput2(uiOffset):%d \rret:%d", picture,
            eAlign, Offset, ret2);
    return ret2;
}

static s32 Test_sdkPrintStartNoRoolPaperManual(char *pasOutInfo, char const *pTitle) {
    SDK_PRINT_FONT font;

    s32 ret2 = sdkPrintInit();
    if (SDK_OK == ret2) {
        font.uiAscFont = SDK_PRN_ASCII12X24;
        font.uiAscZoom = SDK_PRN_ZOOM_N;
        font.uiChFont = SDK_PRN_CH24X24;
        font.uiChZoom = SDK_PRN_ZOOM_N;
        sdkPrintStr("��ӡ���Զ���ֽ����", font, SDK_PRINT_LEFTALIGN, 0, 0);
        sdkPrintStartNoRollPaper();
        ret2 = sdkPrintStart();
    }

    sprintf(pasOutInfo, "return :%d\r", ret2);
    return 1;
}

static s32 Test_sdkPrintStartManual(char *pasOutInfo, char const *pTitle) {
    SDK_PRINT_FONT font;

    s32 ret2 = sdkPrintInit();
    Trace("print", "ret2 = %d\r\n", ret2);
    if (SDK_OK == ret2) {
        font.uiAscFont = SDK_PRN_ASCII12X24;
        font.uiAscZoom = SDK_PRN_ZOOM_N;
        font.uiChFont = SDK_PRN_CH24X24;
        font.uiChZoom = SDK_PRN_ZOOM_N;
        ret2 = sdkPrintStr("��ӡ���Զ���һ��ֽ����", font, SDK_PRINT_LEFTALIGN, 0, 0);
        Trace("print", "ret2 = %d\r\n", ret2);
        ret2 = sdkPrintStart();
        Trace("print", "ret2 = %d\r\n", ret2);
    }

    sprintf(pasOutInfo, "return :%d\r", ret2);
    return 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:		char *pasOutInfo
					char const*pTitle
** Returned value:
** Created By:		huacong  2013.03.01
** Remarks:
*****************************************************************************/
static s32 Test_sdkPrintSetLastRowGapManual(char *pasOutInfo, char const *pTitle) {
    SDK_PRINT_FONT font;

    /*=======BEGIN: wangmingming 2014.06.17  16:1 modify===========*/
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.06.26  16:14 */
    sdkDispMsgBox("������Ϣ", "�ȴ�֧�ִ�ӡ�������һ�е�ʵ���м������", 2000, 0xFFFF);
    return 1;
#else
    s32 ret2 = sdkPrintInit();
    if ( SDK_OK == ret2 )
    {
        font.uiAscFont = E_WORD_ASCII_12_24;
        font.uiAscZoom = E_PRN_ZOOM_N;
        font.uiChFont = E_WORD_NATIVE_24_24;
        font.uiChZoom = E_PRN_ZOOM_N;
        sdkPrintStr("tSetLastRowGap1",font,SDK_PRINT_LEFTALIGN,0,0);
        sdkPrintSetLastRowGap(sdkTestInputU32(pTitle,"Input LastRowGap:"));
        sdkPrintStr("tSetLastRowGap2",font,SDK_PRINT_LEFTALIGN,0,0);
        sdkPrintStart();
        sprintf(pasOutInfo,"��鿴��ӡЧ��");
    }
    
    return ret2;

#endif /* if 0 */
    /*====================== END======================== */
}

static const TEST_MANUAL_MAP PrintManaulMap[] =
        {
                {"Init",             Test_sdkPrintInitManual},
                {"NeedleFeedPaper",  Test_sdkPrintNeedleFeedPaperManual},
                {"BitMap",           Test_sdkPrintBitMapManual},
                {"StartNoRoolPaper", Test_sdkPrintStartNoRoolPaperManual},
                {"Start",            Test_sdkPrintStartManual},
                {"SetLastRowGap",    Test_sdkPrintSetLastRowGapManual},
        };

/*****************************************************************************
** Descriptions:
** Parameters:		Input: None
** Returned value:
** Created By:		huacong  2013.01.28
** Remarks:
*****************************************************************************/
TEST_MAP_INFO GetPrintManualTestInfo(void) {
    TEST_MAP_INFO ret;

    ret.size = sizeof(PrintManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = PrintManaulMap;
    return ret;
}

