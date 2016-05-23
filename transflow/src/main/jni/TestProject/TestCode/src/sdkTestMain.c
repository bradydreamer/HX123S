#include "sdkGlobal.h"
#include "SdkTest.h"
#include "sdkiso8583.h"
#include "sdkRecord.h" //shijianglong 2013.05.06 15:14
#include "sdkUI.h"
//#include "sdkIni.h"

void SdkAutoTestModul(void);

void Test_sdkIccAuto(void);
//void Test_sdkcommAtuo(void);
//extern void Test_sdkcommauto(void);





/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:
   ��������:
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.18 20:41:35
*******************************************************************/
void OnPress8(void) {
    Trace("test:%d", "OnPress8\r\n");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:  ʯ����
   ��������:	OnPress5
   ��������:
   �޸ı�ע:
   ����ʱ��:2012.05.15 16:39:38
*******************************************************************/
void OnPress5(void) {
    Trace("test", "OnPress5\r\n");
}

//������
static char const *pWhiteList[] =
        {
                "testResult",
                "bt_test",
                "rsa",
                "zjp",
                "test",
                "print",
                "icc",
                "dev",
                "devlcd",
                "emv",
                "bmp",
                "fsppro",
                "fsp"
        };


//������
static char const *pBlackst[] =
        {
                "d",
                //"dev"
        };

static const SDK_DEBUG_FILTER DebugFilter =
        {
                true,                                                   //�Ƿ��ڲ���״̬��
                false,
                SDK_DEBUG_TARGET_COM,                                // ������Ϣ���������
                //SDK_DEUBG_OUT_WHITE_LIST, // SDK_DEUBG_OUT_WHITE_LIST,  SDK_DEUBG_OUT_ALL                            //trace����ģʽ
                SDK_DEUBG_OUT_ALL,
                //SDK_DEUBG_BLOCK_ALL,
        };

extern void Test_sdkMathAuto(void);

extern void Test_SdkPedAuto(void);

s32 DisplayList(TEST_MAP const *pMap, s32 conter) {
    u8 *ptem;
    u8 *temp2;
    u32 ii;
    s32 rtn;
    ptem = (u8 *) sdkGetMem(1024);
    if (ptem == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(ptem, 0, 1024);
    temp2 = (u8 *) sdkGetMem(1024);
    if (temp2 == NULL) {
        sdkFreeMem(ptem);
        ptem = NULL;
        return SDK_ERR;
    }
    memset(temp2, 0, 1024);


    for (ii = 0; ii < conter; ii++) {
        int len = sprintf(temp2, "%d.%s\r", ii, pMap[ii].pAsInfo);
        strcat(ptem, temp2);

        if (len > 2048 || strlen(ptem) > 1024) {
            while (1) {
                sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            }
        }
    }

    rtn = sdkDispListBox("ȫ��������Ϣ", ptem, 0);
    if (ptem != NULL) {
        sdkFreeMem(ptem);
        ptem = NULL;
    }
    if (temp2 != NULL) {
        sdkFreeMem(temp2);
        temp2 = NULL;
    }
    return rtn;
}

/*****************************************************************************
** Descriptions:	�Զ�����
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.01.22
** Remarks:
*****************************************************************************/
static s32 test_sdkPrintResult(TEST_MAP *sAutoTestMap, s32 siIndex, s32 siDly) {
    u8 ptem[128] = {0};
    s32 total = 0;

    sdkTestClrStatisticsData();
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, sAutoTestMap[siIndex].pAsInfo, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�����Զ�����", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sAutoTestMap[siIndex].pAutoFun();

    s32 ret = sdkPrintInit();
    if (SDK_KEY_ESC == Test_sdkPrintShowErr(ret)) {
        //Test_sdkPrintTestInfo(sAutoTestMap[siIndex].pAsInfo, NULL);
        //total = sdkTestPrintStatisticsData();
        sprintf(ptem, "%s\r%d������δ��ͨ\r\n", sAutoTestMap[siIndex].pAsInfo,
                sdkTestPrintStatisticsData());
        //Test_sdkPrintTestInfo(NULL, ptem);
        /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
        //ret = sdkPrintStart();
        //Test_sdkPrintShowErr(ret);  
        /*====================== END======================== */

        sdkDispMsgBox("���Խ��", ptem, siDly, SDK_KEY_MASK_ALL);

        sdkTestClrStatisticsData();
        return sdkTestPrintStatisticsData();
    }
    else {
        sdkPrintInit();
    }

    Test_sdkPrintTestInfo(sAutoTestMap[siIndex].pAsInfo, NULL);
    total = sdkTestPrintStatisticsData();
    sprintf(ptem, "%s\r%d������δ��ͨ\r\n", sAutoTestMap[siIndex].pAsInfo, total);
    Test_sdkPrintTestInfo(NULL, ptem);
    /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
    ret = sdkPrintStart();
    //Test_sdkPrintShowErr(ret);  
    /*====================== END======================== */

    sdkDispMsgBox("���Խ��", ptem, siDly, SDK_KEY_MASK_ALL);

    sdkTestClrStatisticsData();
    return total;
}


void SdkAutoTestModul() {
    s32 Index = 0;
    char ptem[128] = {0};
    static const TEST_MAP sAutoTestMap[] =
            {
                    {"�Զ���������ģ��", NULL},
                    {"��ѧģ��", Test_sdkMathAuto},
                    {"����ģ��",
                     Test_SdkPedAuto},              //������,��������ȷ��,�ӿ��ڲ����Դ���δ�޸�
// duxd2015        {"����(�ļ�)����", Test_sdkToolsAuto},
#if MACHINEMODEL != SDK_SYS_MACHINE_200P
                    {"��ӡģ��", Test_sdkPrintAuto},            ////huacong 2013.03.28 16:44
#endif
                    {"��ʾģ��", Test_sdkDispAuto},
                    {"���뷨ģ��", Test_InputAuto},

                    {"IC ��ģ��", Test_sdkIccAuto},
                    {"Mifareģ��", TestSdkMifareMain},
                    {"MemCardģ��", TestSdkMemoryMain},
#if (MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P)
                    {"ͨ��ģ��", Test_sdkcommauto},
#endif
                    {"ϵͳģ��", Test_sdkSysauto},

#if (!SDK_IS_UCOS())
                    {"ɨ��ǹģ��", TestSdkScannerMain},
                    {"��ʷ��ģ��", TestsdkRecordLibMain},
                    {"8583ģ��", TestSdk8583Main},
#endif

                    {"����ģ��", Test_sdkListAuto},
                    {"����ģ��", Test_sdkQueueAuto}, //���ٲ� 2013.04.12 16:54 ADD
#if (!SDK_IS_UCOS())
                    {"XMLģ��", Test_sdkXMLAuto},
#endif
                    {"DUKPTģ��", Test_sdkDukptAuto},
            };
    while (1) {
        Index = DisplayList(sAutoTestMap, sizeof(sAutoTestMap) / sizeof(TEST_MAP));

        if (Index < 0) {
            return;
        }

        if ((sizeof(sAutoTestMap) / sizeof(TEST_MAP)) <= Index) {
            sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            return;
        }

        if (Index == 0) { //�Զ�ȫ������
            s32 ii = 0;
            s32 err_num = 0;
            sdkTestClrStatisticsData();

            for (ii = 1; ii < sizeof(sAutoTestMap) / sizeof(TEST_MAP); ii++) {
                err_num += test_sdkPrintResult(&sAutoTestMap, ii, 1);
            }

            sprintf(ptem, "%s\r%d������δ��ͨ\r��ȷ�ϼ���", "�Զ���������ģ��", err_num);
            sdkDispMsgBox("���Խ��", ptem, 0, SDK_KEY_MASK_ALL);
        }
        else {  //�����ֶ�����
            test_sdkPrintResult(&sAutoTestMap, Index, 0);
        }
    }
}

void Test_sdkSelectCase(u8 *pTital, TEST_MAP_INFO const *pInfo);

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkMathMaual() {
    TEST_MAP_INFO info = GetMathManualTestInfo();

    Test_sdkSelectCase("��ѧ�ⰸ��ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkMenuMaual() {
    TEST_MAP_INFO info = GetMenuManualTestInfo();

    Test_sdkSelectCase("�˵�����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.01.31
** Remarks:
*****************************************************************************/
static void Test_sdkMagMaual() {
    TEST_MAP_INFO info = GetMagManualTestInfo();

    Test_sdkSelectCase("�ſ�����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		������  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkPedMaual() {
    TEST_MAP_INFO info = GetMPedTestInfo();

    SDK_SYS_INITIAL_INFO stinitial;//shijianglong 2013.05.06 14:58
//
    stinitial.bIsHavePinPad = false;
    stinitial.bIsINRf = 1;
    sdkSysInitParam(&stinitial);
    Test_sdkSelectCase("���̰���ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng 2013.01.24 11:45
** Remarks:
*****************************************************************************/
void Test_sdkIccMaual() //yinpeng 2013.01.24 11:45
{
    SDK_SYS_INITIAL_INFO initial;

    TEST_MAP_INFO info = GetIccManualTestInfo();

#if MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P /*Modify by ���ƽ at 2014.06.04  19:28 */
    initial.bIsHavePinPad = false;
#else
    initial.bIsHavePinPad = true;
#endif /* if 0 */
    initial.bIsINRf = false;
    sdkSysReInitParam(&initial);

    sdkIccPowerStartInit();
    Test_sdkSelectCase("IC ��ģ�鰸��ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		huacong  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkPrintMaual() {
    TEST_MAP_INFO info = GetPrintManualTestInfo();

    Test_sdkSelectCase("��ӡ����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		yinpeng 2013.01.24 11:45
** Remarks:
*****************************************************************************/
void Test_sdkInputManual() {
    TEST_MAP_INFO info = GetInputManualTestInfo();

    Test_sdkSelectCase("���뷨����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		׿��  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkSysManual() {
    TEST_MAP_INFO info = GetSysManualTestInfo();

    Test_sdkSelectCase("ϵͳ����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:	disptest code
** Parameters:          Input: None
** Returned value:
** Created By:		wanggaodeng  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkDispMaual()                 //wanggaodeng 2013.01.28 11:12
{
    TEST_MAP_INFO info;

    info = GetDispManualTestInfo();

    Test_sdkSelectCase("��ʾ����ѡ��", &info);
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		��֪��  2013.01.28
** Remarks:
*****************************************************************************/
static void Test_sdkCommUartMaual() {
    //TEST_MAP_INFO info = GetCommUartManualTestInfo();

    //Test_sdkSelectCase("ͨ�Ű���ѡ��", &info);
}

static void Test_sdkIcdealDemon() {
    TEST_MAP_INFO info = GetIcdealDemonTestInfo();

    Test_sdkSelectCase("Demon���԰���ѡ��", &info);
}

static void Test_sdkIcdealFunction() {
    TEST_MAP_INFO info = GetIcdealFunctionTestInfo();

    Test_sdkSelectCase("Function���԰���ѡ��", &info);
}

static void Test_sdkMifareMaual() {
    TEST_MAP_INFO info = GetMifareManualTestInfo();

#if 0 /*Modify by ���ٲ� at 2013.08.21  15:44 */
    Private_sdkIccNonEmvCardInit  (); //���ٲ� 2013.08.05 9:10
#endif /* if 0 */

    Test_sdkSelectCase("Mifare����ѡ��", &info);
}

static void Test_sdkMemoryMaual() {
    if (sdkSysGetMachineCode(NULL) != SDK_SYS_MACHINE_K501AM) //huacong 2013.03.28 11:48
    {
        TEST_MAP_INFO info = GetMemoryManualTestInfo(); //shijianglong 2013.04.16 17:1

#if 0 /*Modify by ���ٲ� at 2013.08.21  15:44 */
        Private_sdkIccNonEmvCardInit  (); //���ٲ� 2013.08.05 9:10
#endif /* if 0 */

        Test_sdkSelectCase("Memory����ѡ��", &info);
    }
    else {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return;
    }
}

static void Test_sdkScannerMaual() {
    if (!sdkTestIsUcos()) //huacong 2013.03.28 11:48
    {
        TEST_MAP_INFO info = GetScannerManualTestInfo();

        Test_sdkSelectCase("Scanner����ѡ��", &info);
    }
    else {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return;
    }
}

extern void Test_sdkBtManual();

/*****************************************************************************
** Descriptions:	�ֶ�����
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.01.22
** Remarks:
*****************************************************************************/
void SdkManualTestModul() {
    s32 index = 0;
    u8 ptem[128] = {0};
    static const TEST_MAP sAutoTestMap[] =
            {
                    {"�ſ�ģ��", Test_sdkMagMaual},
                    {"����ģ��", Test_sdkPedMaual},
                    {"���뷨ģ��", Test_sdkInputManual},
                    {"ϵͳģ��", Test_sdkSysManual},
                    {"IC ��ģ��", Test_sdkIccMaual},
                    {"��ʾģ��", Test_sdkDispMaual},
#if (MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P)
                    {"ͨ��ģ��", Test_sdkCommUartMaual},
                    {"Scanner ģ��", Test_sdkScannerMaual},
#endif
                    {"IcDeal����ģ��", Test_sdkIcdealDemon},
                    {"IcDeal�ӿ�ģ��", Test_sdkIcdealFunction},
#if (MACHINEMODEL != SDK_SYS_MACHINE_200P)
                    {"��ӡģ��", Test_sdkPrintMaual},
#endif
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P)
                    {"����ģ��", Test_sdkBtManual},
#endif

            };

    while (1) {
        //�������ͳ������

        index = DisplayList(sAutoTestMap, sizeof(sAutoTestMap) / sizeof(TEST_MAP));
        if (index < 0) {
            return;
        }


        if ((sizeof(sAutoTestMap) / sizeof(TEST_MAP)) <= index) {
            sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            return;
        }
        sdkTestClrStatisticsData();
        sAutoTestMap[index].pAutoFun();
        s32 ret = sdkPrintInit();
        if (SDK_KEY_ESC == Test_sdkPrintShowErr(ret)) {
            //Test_sdkPrintTestInfo(sAutoTestMap[index].pAsInfo, NULL);
            sprintf(ptem, "%s\r%d������δ��ͨ\r��ȷ�ϼ�����һ����", sAutoTestMap[index].pAsInfo,
                    sdkTestPrintStatisticsData());
            //Test_sdkPrintTestInfo(NULL, ptem);
            /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
            //s32 ret = sdkPrintStart();
            //Test_sdkPrintShowErr(ret);
            /*====================== END======================== */
            sdkDispMsgBox("���Խ��", ptem, 0, SDK_KEY_MASK_ENTER);
            sdkTestClrStatisticsData();
            return;
        }
        else {
            sdkPrintInit();
        }

        Test_sdkPrintTestInfo(sAutoTestMap[index].pAsInfo, NULL);
        sprintf(ptem, "%s\r%d������δ��ͨ\r��ȷ�ϼ�����һ����", sAutoTestMap[index].pAsInfo,
                sdkTestPrintStatisticsData());
        Test_sdkPrintTestInfo(NULL, ptem);
        /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
        ret = sdkPrintStart();
        //Test_sdkPrintShowErr(ret);
        /*====================== END======================== */
        sdkDispMsgBox("���Խ��", ptem, 0, SDK_KEY_MASK_ENTER);
        sdkTestClrStatisticsData();
    }
}

/*****************************************************************************
** Descriptions:	ѡ�����
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.01.22
** Remarks:
*****************************************************************************/
void SdkSelfChoiceTestModul() {
    s32 index = 0;
    char *ptem;

    static const TEST_MAP sAutoTestMap[] =
            {
                    {"��ѧģ��", Test_sdkMathMaual},    //һ��,��������
                    {"�˵�ģ��", Test_sdkMenuMaual},    //������
                    //{"�ļ�ģ��", Test_ToolManual},      //������
                    {"��ӡģ��", Test_sdkPrintMaual},   //������
            };
    ptem = (char *) sdkGetMem(2048);
    if (ptem == NULL) {
        Assert(0);
        return;
    }
    memset(ptem, 0, 2048);
    while (1) {
        //�������ͳ������

        index = DisplayList(sAutoTestMap, sizeof(sAutoTestMap) / sizeof(TEST_MAP));

        if (index < 0) {
            if (ptem != NULL) {
                sdkFreeMem(ptem);
                ptem = NULL;
            }
            return;
        }

        if ((sizeof(sAutoTestMap) / sizeof(TEST_MAP)) <= index) {
            sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            if (ptem != NULL) {
                sdkFreeMem(ptem);
                ptem = NULL;
            }
            return;
        }
        sdkTestClrStatisticsData();
        sAutoTestMap[index].pAutoFun();
        sprintf(ptem, "%s\r%d������δ��ͨ\r��ȷ�ϼ�����һ����", sAutoTestMap[index].pAsInfo,
                sdkTestPrintStatisticsData());
        sdkDispMsgBox("���Խ��", ptem, 0, SDK_KEY_MASK_ENTER);
    }
    if (ptem != NULL) {
        sdkFreeMem(ptem);
        ptem = NULL;
    }
}

static SDK_PRINT_FONT GetPrintFont(s32 size) {
    s32 mach_type = sdkSysGetMachineCode(NULL);

    SDK_PRINT_FONT font_heat[] =
            {
                    {0, SDK_PRN_ASCII12X24, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X32, SDK_PRN_ZOOM_N, SDK_PRN_CH32X32, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X32, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X32, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X32, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X32, SDK_PRN_ZOOM_N, SDK_PRN_CH32X32, SDK_PRN_ZOOM_A, 0},
            };
    SDK_PRINT_FONT font_pin[] =
            {
                    {0, 0, 0, 0, 0, 0},
                    {1, 0, 0, 0, 0, 0},
                    {3, 0, 0, 0, 0, 0},
                    {5, 0, 0, 0, 0, 0},
            };

    if (mach_type == SDK_SYS_MACHINE_K350 || mach_type == SDK_SYS_MACHINE_K350P ||
        mach_type == SDK_SYS_MACHINE_K360 || mach_type == SDK_SYS_MACHINE_K360P) {
        size = (size > (sizeof(font_pin) / sizeof(SDK_PRINT_FONT) - 1)) ? (
                sizeof(font_pin) / sizeof(SDK_PRINT_FONT) - 1) : size;
        return font_pin[size];
    }
    else {
        size = (size > (sizeof(font_heat) / sizeof(SDK_PRINT_FONT) - 1)) ? (
                sizeof(font_heat) / sizeof(SDK_PRINT_FONT) - 1) : size;
        return font_heat[size];
    }
}

static s32 sdkTestSysGetFileAllVer(u8 *pFileName, u8 *lpOutVer) {
    u8 *fileTag[] =
            {
                    "ExOption",
                    "IsBaseSDK",
                    "Platform",
                    "OutVer",
                    "InnerVer",
                    "AppID",
                    "AppName",
                    "FileStyle",
                    "FileRoot",
                    "Company",
                    "SysEditDate",
                    "RelyFile",
                    "UserInfor",
            };

    u8 tem[256] = {0};
    s32 fileTagConter = sizeof(fileTag) / sizeof(u8 * );

    lpOutVer[0] = 0;    //����


    while (fileTagConter--) {
        memset(tem, 0, sizeof(tem));
        strcpy(tem, fileTag[fileTagConter]);
        strcat(tem, ":");
        sdkSysGetFileVer(pFileName, fileTag[fileTagConter], &tem[strlen(tem)]);
        strcat(tem, "\n");
        strcat(lpOutVer, tem);
    }

    return strlen(lpOutVer);
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *file_ver
                               u8 *pFileName
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
static void Test_sdkSysGetFileAllVer(u8 *pFileName, u8 *lpOutVer) {
    u8 *fileName[] =
            {
//                "/mtd0/dll/libsdk.so",
//                "/mtd0/dll/libsdkdev.so",
//                "/mtd0/dll/libsdkload.so",
//                "/mtd0/dll/libsdk8583.so",
//                "/mtd0/dll/libsdkemv.so",
//                "/mtd0/dll/libsdkrecord.so",
//                "/mtd0/dll/libsdkxml.so",
//                "/mtd0/dll/libsdkmenu.so"
                    "/mtd0/dll/libsdk8583.so",
                    "/mtd0/dll/libsdkcard.so",
                    "/mtd0/dll/libsdkcomm.so",
                    "/mtd0/dll/libsdkdebug.so",
                    "/mtd0/dll/libsdkdev.so",
                    "/mtd0/dll/libsdkdisp.so",
                    "/mtd0/dll/libsdkemv.so",
                    "/mtd0/dll/libsdkexdevice.so",
                    "/mtd0/dll/libsdkfile.so",
                    "/mtd0/dll/libsdkime.so",
                    "/mtd0/dll/libsdkkey.so",
                    "/mtd0/dll/libsdkload.so",
                    "/mtd0/dll/libsdkmath.so",
                    "/mtd0/dll/libsdkmathsm2.so",
                    "/mtd0/dll/libsdkmenu.so",
                    "/mtd0/dll/libsdkped.so",
                    "/mtd0/dll/libsdkprint.so",
                    "/mtd0/dll/libsdkrecord.so",
                    "/mtd0/dll/libsdkrecord.so",
                    "/mtd0/dll/libsdksys.so",
                    "/mtd0/dll/libsdktool.so",
            };

    s32 fileConter = sizeof(fileName) / sizeof(u8 * );
    u8 *tem;

    sdkDispMsgBox("��ʾ��Ϣ", "��׼���ô�ӡֽ,������ӡ����", 0, SDK_KEY_MASK_ALL);

    s32 ret = sdkPrintInit();
    if (SDK_KEY_ESC == Test_sdkPrintShowErr(ret)) {
        return;
    }
    else {
        sdkPrintInit();
    }

    tem = (u8 *) sdkGetMem(1024);
    if (tem == NULL) {
        Assert(0);
        return;
    }
    while (fileConter--) {
        memset(tem, 0, 1024);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 20);
        sdkPrintStr(fileName[fileConter], GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkTestSysGetFileAllVer(fileName[fileConter], tem);
        sdkPrintStr(tem, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 0);
    }

    sdkPrintStr(" ", GetPrintFont(1), SDK_PRINT_LEFTALIGN, 0, 50);
    /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
    ret = sdkPrintStart();
    //Test_sdkPrintShowErr(ret);
    /*====================== END======================== */

    if (tem != NULL) {
        sdkFreeMem(tem);
        tem = NULL;
    }
    return;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
static void SdkTestShowVer() {
    u8 buf[200] = {0};

    {
        s32 ret = 0;
#if (MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P)
        Test_sdkSysGetFileAllVer(NULL, NULL);
#endif

        ret = sdkPrintInit();
        if (SDK_KEY_ESC == Test_sdkPrintShowErr(ret)) {
            return;
        }
        else {
            sdkPrintInit();
        }

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "LibDev:");
        sdkSysGetLibdevVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);


        memset(buf, 0, sizeof(buf));
        strcpy(buf, "Panel:");
        sdkSysGetPanelVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);


        memset(buf, 0, sizeof(buf));
        strcpy(buf, "Daemon:");
        sdkSysGetDaemonVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);


        memset(buf, 0, sizeof(buf));
        strcpy(buf, "xgdemv:");
        sdkSysGetLibxgdemvVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);

#if (MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P)
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "LibMaths:");
        sdkSysGetLibMahtsVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);
#endif

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "LibModule:");
        sdkSysGetLibModuleVersion(&buf[strlen(buf)]);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);

#if 0 /*Modify by ���ƽ at 2015.03.23  17:51 */

        memset(buf, 0, sizeof(buf));
        strcpy(buf, "boot:");
        sdkSysGetFirmwareVer( &buf[strlen(buf)], FIRMWAREVER_BOOT1 );
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);
        
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "core:");
        sdkSysGetFirmwareVer( &buf[strlen(buf)], FIRMWAREVER_KERNEL );
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);
        
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "root fs:");
        sdkSysGetFirmwareVer( &buf[strlen(buf)], FIRMWAREVER_ROOTFS );
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);
        {
            u32 i = 0;
            u8 *titleInfo[] = {
                                "�ſ�:",
                                "IC��:",
                                "�ǽӿ�:",
                                "��ӡ��:",
                                "��Ƶ:",
                                "������Կ:",
                                "�����ļ�ϵͳ:",
                                "����:",
                                "��ƵͨѶ:",
                                "LED��:",
                                "USBͨѶ:",
                                "DUKPT����"
            };
            for( i = 0; i < MODULE_DUKPT + 1; i++ )
            {
            
                memset(buf, 0, sizeof(buf));
                strcpy(buf, titleInfo[i] );
                sdkSysGetModuleVer (&buf[strlen(buf)], i );
                strcat(buf, "\n");
                sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
                sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10); 
            }
        }
#endif /* if 0 */
        memset(buf, 0, sizeof(buf));
        strcpy(buf, "Ӧ�ñ���ʱ��:\n");
        strcat(buf, __DATE__ " "__TIME__);
        strcat(buf, "\n");
        sdkPrintStr(buf, GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr(" ", GetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 10);
        ret = sdkPrintStart();
        /*=======BEGIN: wangmingming 2014.06.16  17:19 modify===========*/
        //Test_sdkPrintShowErr(ret);
        /*====================== END======================== */
        if (ret != SDK_OK) {
            sprintf(buf, "��ӡʧ�ܷ���ֵ: %d", ret);
            sdkTestMsgBox(buf, SDK_KEY_MASK_ALL);
        }
    }
}

SDK_SYS_INITIAL_INFO gstInitial;

static void sdkTestSysCfg() {
    gstInitial.bIsHavePinPad = (SDK_KEY_1 == sdkDispMsgBox("�Ƿ�������������", "1.��  2. �� ", 0,
                                                           SDK_KEY_MASK_1 |
                                                           SDK_KEY_MASK_2));    //have PIN pad or not
    gstInitial.bIsINRf = (SDK_KEY_2 == sdkDispMsgBox("�Ƿ�����÷ǽ�", "1.��  2. �� ", 0,
                                                     SDK_KEY_MASK_1 |
                                                     SDK_KEY_MASK_2));   //have contactless or not

    if (gstInitial.bIsHavePinPad == true) {
        s32 mode = sdkSysGetMachineCode(NULL);

        if (mode == SDK_SYS_MACHINE_K350P || mode == SDK_SYS_MACHINE_K360 ||
            mode == SDK_SYS_MACHINE_G2 || mode == SDK_SYS_MACHINE_200P) {
            gstInitial.bIsHavePinPad = true;
            gstInitial.bIsINRf = true;
            sdkDispMsgBox("��ʾ", "�˻��Ͳ��ò�������������", 0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2);
        }
    }
    sdkSysInitParam(&gstInitial);
}

extern void sdkTestTempFun(void); //shijianglong 2013.05.06 15:1
//extern void Test_sdkU8ToBcd(void);
static SDK_SYS_HOOK_RET TestKeyHook(s32 *const siKey) {
    if (*siKey == SDK_KEY_F4) {
        *siKey = SDK_KEY_UP;
    }
    return SDK_SYS_NEXT_HOOK;
}

bool sdkTestIsUcos(void) {
    return (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K508A) ||
           (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM);
}

int appmain(int argc, const char **argv) {
    SDK_SYS_INITIAL_INFO stInitial;
    SDK_DISP_PIXEL pixel;
    u32 i = 0;

    sdkSysLoadDll(NULL, 0);
    Verify(sdkSysMain(argc, argv) == SDK_OK);
    Verify(sdkDebugInitalList(pWhiteList, sizeof(pWhiteList) / sizeof(char *), pBlackst,
                              sizeof(pBlackst) / sizeof(char *)) == SDK_OK);
    Verify(sdkDebugSetFilter(&DebugFilter) == SDK_OK);
    sdkTestTraceInit();
    sdkSysSetBlackLightCloseTime(0);

    stInitial.bIsHavePinPad = false;    //have PIN pad or not
    stInitial.bIsINRf = false;   //have contactless or not
    sdkSysInitParam(&stInitial);
    pixel = sdkDispGetScreenPixel();
    if (pixel.siX > 128) {
        sdkDispStateBar(true);
        i = 0;
        sdkDispIcon(SDK_DISP_BT, 0, 2 + ((pixel.siX - 4) / 4) * (i++), 1);
        sdkDispIcon(SDK_DISP_USB, 0, ((pixel.siX - 4) / 4) * (i++), 1);
        sdkDispIcon(SDK_DISP_BATTERY, 0, ((pixel.siX - 4) / 4) * (i++), 1);
        sdkDispIcon(SDK_DISP_AUDIO, 0, ((pixel.siX - 4) / 4) * (i++), 1);
    }
    while (1) {
        s32 key;

        key = sdkDispMsgBox("��ѡ�����ģ��",
                            "0.������Բ���\r\n1.�Զ����� \r2.�ֶ����� \r3.ϵͳ����\r4.�汾��Ϣ\r5.��ʱ����\r6.������Ϣ����\r7.ѡ��ģ��\r8.��չģ��",
                            0, SDK_KEY_MASK_ALL);
        Trace("fspmain", "key = %d\r\n", key);
        if (pixel.siX > 128) {
            sdkDispStateBar(false);
        }
        switch (key) {
            case SDK_KEY_0:
#if ((MACHINEMODEL != SDK_SYS_MACHINE_K501AM) && (MACHINEMODEL != SDK_SYS_MACHINE_K508A) && (MACHINEMODEL != SDK_SYS_MACHINE_G2) && (MACHINEMODEL != SDK_SYS_MACHINE_200P))
                sdkMultiLangTest();
#endif
                break;
            case SDK_KEY_1:
                SdkAutoTestModul();
                break;

            case SDK_KEY_2:
                if (0) {
                    u8 *p = NULL;
                    u32 k = 512, j = 1;
                    while (1) {
                        p = NULL;
                        p = sdkGetMem(k * j);
                        if (p != NULL) {
                            sdkFreeMem(p);
                            Trace("zjp", "malloc %d success!!!\r\n", k * j);
                            j++;
                        }
                        else {
                            Trace("zjp", "malloc %d fail!!!\r\n", k * j);
                            break;
                        }
                    }
                }
                SdkManualTestModul();
                break;

            case SDK_KEY_3:
                sdkTestSysCfg();
                break;

            case SDK_KEY_4:
                SdkTestShowVer();    //������//huacong 2013.03.30 17:38
                break;

            case SDK_KEY_5:
                sdkTestTempFun();
                break;

            case SDK_KEY_6:
                sdkDebugControl(true);
                break;

            case SDK_KEY_7:
                SdkSelfChoiceTestModul();
                break;
            case SDK_KEY_8:
#if (MACHINEMODEL != SDK_SYS_MACHINE_200P)
                sdkTestExttools();
#endif
                break;
            case SDK_KEY_9:
#if 0 /*Modify by ���ƽ at 2014.12.17  14:52 */
                {
                    static int i = 0;
                    u8 pinBuf[128] = {10};
                    int n = 0;
                    SDK_BT_PAIR_INFO stPairInfo;
                    
                    sdkBtOpen();
                    sdkBtGetPairDevInfo(&stPairInfo);
                    while( n < stPairInfo.m_num )
                    {
                        sdkBtDelPairDevInfo(n);
                        n++;
                    }

                    if( i == 0 )
                    {                      
                        sdkBtSetPinMode( E_SDK_BT_JUST_WORK );
                        i = 1;
                    }
                    else
                    {
                        sdkBtSetPinMode( E_SDK_BT_PIN_CODE );
                        i = 0;
                    }
                    sdkBtSetDiscoverable( 1 );
                    sdkDispClearScreen();
                    if( i == 1 )
                    {
                        sdkDispRowAt(SDK_DISP_LINE1, 0, "���������ģʽ����", SDK_DISP_DEFAULT);
                    }
                    else
                    {
                        sdkDispRowAt(SDK_DISP_LINE1, 0, "���������ģʽ����", SDK_DISP_DEFAULT);
                    }
                    sdkDispRowAt(SDK_DISP_LINE2, 0, "���ȶϿ��ֻ������", SDK_DISP_LEFT_DEFAULT);
                    memset(pinBuf, 0, 128);
                    memcpy( pinBuf, "����:", 5 );
                    sdkBtGetPin( pinBuf + 5, 128 );
                    sdkDispRowAt(SDK_DISP_LINE3, 0, pinBuf, SDK_DISP_LEFT_DEFAULT);
                    memset(pinBuf, 0, 128);
                    memcpy( pinBuf, "����:", 5 );
                    sdkBtGetName( pinBuf, 128 );
                    sdkDispRowAt(SDK_DISP_LINE4, 0, pinBuf, SDK_DISP_LEFT_DEFAULT);
                    while(SDK_BT_STATUS_WAITING != sdkBtGetStatus())
                    {
                        sdkmSleep(5);
                        if( SDK_KEY_ESC == sdkKbGetKey() )
                        {
                            break;
                        }
                    }
                    
                    sdkBtClose();
                }
#endif /* if 0 */

                break;
        }
        if (pixel.siX > 128) {
            sdkDispStateBar(true);
            i = 0;
            sdkDispIcon(SDK_DISP_BT, 0, 2 + ((pixel.siX - 4) / 4) * (i++), 1);
            sdkDispIcon(SDK_DISP_USB, 0, ((pixel.siX - 4) / 4) * (i++), 1);
            sdkDispIcon(SDK_DISP_BATTERY, 0, ((pixel.siX - 4) / 4) * (i++), 1);
            sdkDispIcon(SDK_DISP_AUDIO, 0, ((pixel.siX - 4) / 4) * (i++), 1);
        }
    }
    TEST_OUT:
    sdkSysAppQuit();
    sdkSysUnLoadDll();
#ifdef XGD_MEM_DEBUG
    sdkGetMemStatus();
#endif
    return 0;
}

