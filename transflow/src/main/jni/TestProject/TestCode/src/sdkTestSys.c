#include "sdkGlobal.h"
#include "SdkTest.h"


/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetCurAppId(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetCurAppId)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetCurAppId(NULL) == SDK_PARA_ERR));

    {
        u8 temp[256] = {0};
        u8 temp1[500] = {0};
        s32 len = 0;


        len = sdkSysGetCurAppId(temp);
        sprintf(temp1, "AppId:%s,len:%d\r\n", temp, len);
        Test_sdkPrintTestInfo("sdkSysGetCurAppId", temp1);

    }

    TestLog("TestSys", " \r\n \r\n(sdkSysGetCurAppId)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetCurAppDir(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetCurAppDir)测试  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetCurAppDir(NULL) == SDK_PARA_ERR));

    {
        u8 temp[256] = {0};
        u8 temp1[500] = {0};
        s32 len = 0;


        len = sdkSysGetCurAppDir(temp);
        sprintf(temp1, "AppDir:%s,len:%d\r\n", temp, len);
        Test_sdkPrintTestInfo("sdkSysGetCurAppDir", temp1);

    }


    TestLog("TestSys", " \r\n \r\n(sdkSysGetCurAppDir)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetLibdevVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetLibdevVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetLibdevVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetLibdevVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetPanelVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetPanelVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetPanelVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetPanelVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetDaemonVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetDaemonVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetDaemonVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetDaemonVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetLibxgdemvVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetLibxgdemvVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetLibxgdemvVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetLibxgdemvVersion)测试完成 \r\n \r\n");
}

//
///*****************************************************************************
//** Descriptions:
//** Parameters:        Input: None
//** Returned value:
//** Created By:		卓铨  2013.01.05
//** Remarks:
//*****************************************************************************/
//void sdkTest_sdkSysGetLibappdeviceVersion(void)
//{
//    Trace("TestSys", " \r\n \r\n开始(sdkSysGetLibappdeviceVersion)测试  \r\n \r\n");
//
//    TEST_IS_TRUE_WITH_INFO("空指针测试" , (sdkSysGetLibappdeviceVersion(NULL) == SDK_PARA_ERR));
//
//    Trace("TestSys", " \r\n \r\n(sdkSysGetLibappdeviceVersion)测试完成 \r\n \r\n");
//}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetLibMathsVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetLibMahtsVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetLibMahtsVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetLibMahtsVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetLibModuleVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetLibModuleVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetLibModuleVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysGetLibModuleVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetComNo(void) {
    u8 comno = 0;
    u16 i = 2;

    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetComNo)测试  \r\n \r\n");

    for (i = 2; i < 10; i++) {
        TEST_IS_TRUE_WITH_INFO("错误入参测试", (sdkSysGetComNo(i) == SDK_PARA_ERR));
    }

    i = SDK_SYS_COM_PC;
    comno = 0;

    if (i == SDK_SYS_COM_PC)                       // PC
    {
        switch (sdkSysGetMachineCode(NULL)) {
            case SDK_SYS_MACHINE_K320: {
                comno = 1;
            }
                break;

            case SDK_SYS_MACHINE_K370: {
                comno = 0;          //zhuoquan 2013.03.22 11:11
            }
                break;

            case SDK_SYS_MACHINE_K390: //shijianglong 2013.01.14 17:39修改返回串口号为1
            {
                comno = 1;
            }
                break;

            case SDK_SYS_MACHINE_K301P:
            case SDK_SYS_MACHINE_K320P:
            case SDK_SYS_MACHINE_K360P:
            case SDK_SYS_MACHINE_K370P:
            case SDK_SYS_MACHINE_K350P:
                //case SDK_SYS_MACHINE_G2:
                //case SDK_SYS_MACHINE_200P:
            {
                comno = 0;
            }
                break;

            case SDK_SYS_MACHINE_K501AM: //huacong 2013.03.25 11:11
            {
                comno = 0;
            }
                break;

            default: {
                comno = 0;
            }
                break;
        }

        TEST_IS_TRUE_WITH_INFO("正常测试", (sdkSysGetComNo(i) == comno));
        TestLog("TestSys", "PC comno  = %d\r\n", comno);
    }
    i = SDK_SYS_COM_PIN_PAD;
    comno = 0;

    if (i == SDK_SYS_COM_PIN_PAD)                   // PINCOM
    {
        switch (sdkSysGetMachineCode(NULL)) {
            case SDK_SYS_MACHINE_K320:
            case SDK_SYS_MACHINE_K390:
            case SDK_SYS_MACHINE_K390P:
            case SDK_SYS_MACHINE_K820: {
                comno = 2;
            }
                break;

                /*=======BEGIN: lilin20120825 modify===========*/
            case SDK_SYS_MACHINE_K370:
            case SDK_SYS_MACHINE_K350:
            case SDK_SYS_MACHINE_K301FZ:
            case SDK_SYS_MACHINE_K360:
            case SDK_SYS_MACHINE_K301FZE: {
                comno = 1;
            }
                break;

            case SDK_SYS_MACHINE_K501AM: //huacong 2013.03.25 11:13
            {
                comno = 0;
            }
                break;

            case SDK_SYS_MACHINE_K508A:    //zhongjiahong 2013.11.20 16:18,根据王宏杨在函数sdkSysGetComNo的修改而定
            {
                comno = 3;
            }
                break;

            default: {
                //lilin 20120825
                //     comno = 1;
                comno = 2;
            }
                break;
        }

        TEST_IS_TRUE_WITH_INFO("正常测试", (sdkSysGetComNo(i) == comno));
        TestLog("TestSys", "PIN_PAD comno  = %d\r\n", comno);
    }
    TestLog("TestSys", " \r\n \r\n(sdkSysGetComNo)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysStlGetMachinemodelVersion(void) {
    TestLog("TestSys", " \r\n \r\n开始(sdkSysStlGetMachinemodelVersion)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysStlGetMachinemodelVersion(NULL) == SDK_PARA_ERR));
    //TEST_IS_TRUE_WITH_INFO("空指针测试" , (sdkSysStlGetMachinemodelVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestSys", " \r\n \r\n(sdkSysStlGetMachinemodelVersion)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetMachineModel(void) {
    u8 str[128] = {0};
    u8 pStr[128] = {0};


    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetMachineModel)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetMachineModel(NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("正常测试", (sdkSysGetMachineModel(str) == strlen(str)));

    {
        switch (sdkSysGetMachineCode(NULL)) {
            case SDK_SYS_MACHINE_K301FZ:
            case SDK_SYS_MACHINE_K301P:
            case SDK_SYS_MACHINE_K301FZE: {
                strcpy(pStr, "K301FZ");
            }
                break;

            case SDK_SYS_MACHINE_K320:
            case SDK_SYS_MACHINE_K320P: {
                strcpy(pStr, "K320");
            }
                break;

            case SDK_SYS_MACHINE_K350:
            case SDK_SYS_MACHINE_K350P: {
                strcpy(pStr, "K350");
            }
                break;

            case SDK_SYS_MACHINE_K360:
            case SDK_SYS_MACHINE_K360P: {
                strcpy(pStr, "K360");
            }
                break;

            case SDK_SYS_MACHINE_K370:
            case SDK_SYS_MACHINE_K370P: {
                strcpy(pStr, "K370");
            }
                break;

            case SDK_SYS_MACHINE_K390:
            case SDK_SYS_MACHINE_K390P: {
                strcpy(pStr, "K390");
            }
                break;

            case SDK_SYS_MACHINE_K820:
            case SDK_SYS_MACHINE_K820P: {
                strcpy(pStr, "K820");
            }
                break;

                /*=======BEGIN: lilin20120825 modify===========*/
            case SDK_SYS_MACHINE_G810: {
                strcpy(pStr, "G810");
            }
                break;
                /*================ END================== */

            case SDK_SYS_MACHINE_G870:
            case SDK_SYS_MACHINE_G870D: {
                strcpy(pStr, "G870");
            }
                break;

            case SDK_SYS_MACHINE_V70D: {
                strcpy(pStr, "V70D");
            }
                break;

            case SDK_SYS_MACHINE_K501AM: //huacong 2013.03.25 11:24
            {
                strcpy(pStr, "K501");
            }
                break;

            case SDK_SYS_MACHINE_K508A:     //zhongjiahong 2013.11.20 16:24增加型号k508
            {
                strcpy(pStr, "K508A");
            }
                break;
            case SDK_SYS_MACHINE_G2:     //zhujinping 2013.11.20 16:24增加型号G2
            {
                strcpy(pStr, "G2");
            }
                break;
            case SDK_SYS_MACHINE_200P: //zhuoquan 2013.04.19 15:24
            {
                strcpy(pStr, "K200P");
            }
                break;
            default:
                break;
        }
    }
    TEST_IS_TRUE_WITH_INFO("正常测试", (0 == memcmp(pStr, str, strlen(str))));
    TestLog("TestSys", "MachineModel  = %s\r\n", str);
    TestLog("TestSys", " \r\n \r\n(sdkSysGetMachineModel)测试完成 \r\n \r\n");

}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetMachineCode(void) {
    u8 temp[256] = {0};
//    u8 temp1[500] = {0};


    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetMachineCode)测试  \r\n \r\n");
    //sdkSysGetMachineCode  这个函数现在改了//shiweisong 2013.04.02 11:29

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetMachineCode(NULL) == sdkSysGetMachineCode(temp)));
    TEST_IS_TRUE_WITH_INFO("正常测试", (2 == strlen(temp)));

    {
        u8 temp1[500] = {0};
        s32 len = 0;
        len = sdkSysGetMachineCode(temp);
        sprintf(temp1, "MachineCode:%s, len:%d\r\n", temp, len);
        Test_sdkPrintTestInfo("sdkSysGetMachineCode", temp1);
    }

    TestLog("TestSys", " \r\n \r\n(sdkSysGetMachineCode)测试完成 \r\n \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetCupSn(void) {
    u8 str[128] = {0};
    u8 pSn[128] = {0};


    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetCupSn)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetCupSn(NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("正常测试", (sdkSysGetCupSn(str) == strlen(str)));
    {
        switch (sdkSysGetMachineCode(NULL)) {
            case SDK_SYS_MACHINE_K301FZ:
            case SDK_SYS_MACHINE_K301P: {
                strcpy(pSn, "3061");
            }
                break;

            case SDK_SYS_MACHINE_K320:
            case SDK_SYS_MACHINE_K320P: {
                strcpy(pSn, "3106");
            }
                break;

            case SDK_SYS_MACHINE_K350:
            case SDK_SYS_MACHINE_K350P: {
                strcpy(pSn, "3063");
            }
                break;

            case SDK_SYS_MACHINE_K360:
            case SDK_SYS_MACHINE_K360P: {
                strcpy(pSn, "3085");
            }
                break;

            case SDK_SYS_MACHINE_K370:
            case SDK_SYS_MACHINE_K370P: {
                strcpy(pSn, "3089");
            }
                break;

            case SDK_SYS_MACHINE_K390:
            case SDK_SYS_MACHINE_K390P: {
                strcpy(pSn, "3100");
            }
                break;

            case SDK_SYS_MACHINE_G870:                 // lilin 20120428
            case SDK_SYS_MACHINE_G870D: {
                strcpy(pSn, "3158");
            }
                break;

            case SDK_SYS_MACHINE_G810: {
                strcpy(pSn, "3195");
            }
                break;

            case SDK_SYS_MACHINE_K501AM: //huacong 2013.03.25 11:28 暂时写为501
            {
                strcpy(pSn, "5001");
            }
                break;

            case SDK_SYS_MACHINE_K508A:     //zhongjiahong 2013.11.20 16:24增加型号k508
            {
                strcpy(pSn, "5008");
            }
                break;
            default: {
                Assert(0);
                strcpy(pSn, "0000");
                // return SDK_PARA_ERR;
            }
                break;    //lint  !e527 屏蔽掉pclint警告
        }
    }
    TEST_IS_TRUE_WITH_INFO("正常测试", (0 == memcmp(pSn, str, strlen(str))));
    TestLog("TestSys", "CupSn  = %s\r\n", str);
    TestLog("TestSys", " \r\n \r\n(sdkSysGetCupSn)测试完成 \r\n \r\n");

}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		卓铨  2013.01.05
** Remarks:
*****************************************************************************/
void sdkTest_sdkSysGetSwitchAppId(void) {
    u8 datain[128] = {0};
    u8 dataout[128] = {0};
    u8 temp[64] = {0};


    TestLog("TestSys", " \r\n \r\n开始(sdkSysGetSwitchAppId)测试  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetSwitchAppId(NULL, dataout) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("空指针测试", (sdkSysGetSwitchAppId(datain, NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("正常测试", (sdkSysGetSwitchAppId(datain, dataout) == strlen(dataout)));
    {
        switch (sdkSysGetMachineCode(NULL)) {
            case SDK_SYS_MACHINE_K301FZ:
            case SDK_SYS_MACHINE_K301P:
            case SDK_SYS_MACHINE_K301FZE: {
                strcpy(temp, "301");
            }
                break;

            case SDK_SYS_MACHINE_K320:
            case SDK_SYS_MACHINE_K320P: {
                strcpy(temp, "320");
            }
                break;

            case SDK_SYS_MACHINE_K350:
            case SDK_SYS_MACHINE_K350P: {
                strcpy(temp, "350");
            }
                break;

            case SDK_SYS_MACHINE_K360:
            case SDK_SYS_MACHINE_K360P: {
                strcpy(temp, "360");
            }
                break;

            case SDK_SYS_MACHINE_K370:
            case SDK_SYS_MACHINE_K370P: {
                strcpy(temp, "370");
            }
                break;

            case SDK_SYS_MACHINE_K390:
            case SDK_SYS_MACHINE_K390P: {
                strcpy(temp, "390");
            }
                break;

            case SDK_SYS_MACHINE_G870:
            case SDK_SYS_MACHINE_G870D: {
                strcpy(temp, "870");
            }
                break;

            case SDK_SYS_MACHINE_V70D: {
                strcpy(temp, "V70");
            }
                break;

            case SDK_SYS_MACHINE_K820:
            case SDK_SYS_MACHINE_K820P: {
                strcpy(temp, "820");
            }
                break;

            case SDK_SYS_MACHINE_G810: {
                strcpy(temp, "810");
            }
                break;

            case SDK_SYS_MACHINE_K501AM: {
                strcpy(temp, "501");
            }
                break;
            case SDK_SYS_MACHINE_K508A:     //zhongjiahong 2013.11.20 16:24增加型号k508
            {
                strcpy(temp, "508");
            }
            case SDK_SYS_MACHINE_G2:     //zhongjiahong 2013.11.20 16:24增加型号k508
            {
                strcpy(temp, "G2");
            }
                break;
            case SDK_SYS_MACHINE_200P: //zhujinping 2013.04.19 15:39
            {
                strcpy(temp, "K200P");
            }
                break;
            default:
                break;
        }

        strcat(temp, datain);
        // strcpy(IdOut, temp);
    }
    TestLog("TestSys", "dataout  = %s\r\n", dataout);
    TestLog("TestSys", "temp  = %s\r\n", temp);
    TestLog("TestSys", "strlen(dataout)  = %d\r\n", strlen(dataout));
    TEST_IS_TRUE_WITH_INFO("正常测试", (0 == memcmp(temp, dataout, strlen(dataout))));
    TestLog("TestSys", "SwitchAppId  = %s\r\n", dataout);
    TestLog("TestSys", " \r\n \r\n(sdkSysGetSwitchAppId)测试完成 \r\n \r\n");

}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
static void Test_sdkSysGetCurAppState(void) {
    u8 temp[256] = {0};
    s32 len = 0;

    len = sdkSysGetCurAppState();
    sprintf(temp, "State:%d\r\n", len);
    Test_sdkPrintTestInfo("sdkSysGetCurAppState", temp);
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
static void Test_sdkSysGetMisComNo(void) {
    u8 temp1[500] = {0};
    s32 withpinpad_comno = 0;
    s32 nopinpad_comno = 0;


    nopinpad_comno = sdkSysGetMisComNo(0);
    withpinpad_comno = sdkSysGetMisComNo(1);
    sprintf(temp1, "withpinpad_comno:%d\r\nnopinpad_comno:%d\r\n", withpinpad_comno,
            nopinpad_comno);
    Test_sdkPrintTestInfo("sdkSysGetMisComNo", temp1);

}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
static void Test_sdkSysGetComNo(void) {
    u8 temp1[500] = {0};
    s32 pinpad = 0, pcno = 0;


    pcno = sdkSysGetComNo(SDK_SYS_COM_PC);
    pinpad = sdkSysGetComNo(SDK_SYS_COM_PIN_PAD);
    sprintf(temp1, "pinpadno:%d \rpcno:%d\r\n", pinpad, pcno);
    Test_sdkPrintTestInfo("sdkSysGetComNo", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsHaveBase(void) {
    u8 temp1[500] = {0};


    sprintf(temp1, "IsHaveBase:%d\r\n", sdkSysIsHaveBase());
    Test_sdkPrintTestInfo("sdkSysIsHaveBase", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsUclinuxPlat(void) {
    u8 temp1[500] = {0};


    sprintf(temp1, "IsUclinux:%d\r\n", sdkSysIsUclinuxPlat());
    Test_sdkPrintTestInfo("sdkSysIsUclinuxPlat", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsColorScreen(void) {
    u8 temp1[500] = {0};


    sprintf(temp1, "IsColorScreen:%d\r\n", sdkSysIsColorScreen());
    Test_sdkPrintTestInfo("sdkSysIsColorScreen", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetScreenPixel(void) {
    enum SDK_SYS_SCREEN_TYPE ret;
    u8 temp1[128] = {0};
    s32 siX;
    s32 siY;


    ret = sdkSysGetScreenPixel(); //shijianglong 2013.06.17 16:50
    sdkSysGetScreenWidthHigh(&siX, &siY);
    sprintf(temp1, "ScreenPixel:\r长width = %d\r宽height = %d\nreturn = %d\r\n", siX, siY, ret);
    Test_sdkPrintTestInfo("sdkSysGetScreenPixel", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsHaveSpeaker(void) {
    bool ret;
    u8 temp1[500] = {0};


    ret = sdkSysIsHaveSpeaker();
    sprintf(temp1, "IsHaveSpeaker:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsHaveSpeaker", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsPrintPin(void) {
    bool ret;
    u8 temp1[500] = {0};


    ret = sdkSysIsPrintPin();
    sprintf(temp1, "IsPrintPin:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsPrintPin", temp1);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetCupSn(void) {
    u8 temp[256] = {0};
    u8 temp1[256] = {0};
    s32 len = 0;

    len = sdkSysGetCupSn(temp);

    strcpy(temp1, "CupSn:");
    strcat(temp1, temp);
    strcat(temp1, "\r\n");
    Test_sdkPrintTestInfo("sdkSysGetCupSn", temp1);

}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetCommMask(void) {
    s32 ret;
    u8 temp[256] = {0};

    ret = sdkCommGetCommMask();
    sprintf(temp, "return:0x%x\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysGetCommMask", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetDefaultCommuType(void) {
    SDK_COMM_COMMMODE ret;
    u8 temp[256] = {0};

    ret = sdkComGetDefaultCommuType();
    sprintf(temp, "DefaultCommuType:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysGetDefaultCommuType", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsHavePinPad(void) {
    bool ret;
    u8 temp[256] = {0};

    ret = sdkSysIsHavePinPad();
    sprintf(temp, "IsHavePinPad:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsHavePinPad", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetAppReturnWelcome(void) {
    s32 ret;
    s32 time = 10000;
    u8 temp[256] = {0};

    ret = sdkSysGetAppReturnWelcome(time);

    sprintf(temp, "time:%d \rreturn:%d\r\n", time, ret);
    Test_sdkPrintTestInfo("sdkSysGetAppReturnWelcome", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkIsSupportPCI(void) {
    bool ret;
    u8 temp[256] = {0};

    ret = sdkIsSupportPCI();
    sprintf(temp, "IsSupportPCI:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkIsSupportPCI", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsSupportTSlib(void) {
    bool ret;
    u8 temp[256] = {0};

    ret = sdkSysIsSupportTSlib();
    sprintf(temp, "IsSupportTS:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsSupportTSlib", temp);

}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetSwitchAppId(void) {
    u8 temp[256] = {0};
    u8 buf[128] = {0};
    s32 len = 0;


    len = sdkSysGetSwitchAppId("123456", buf);
    sprintf(temp, "IN_ID:%s \rGetAid:%s\rreturn:%d\r\n", "123456", buf, len);
    Test_sdkPrintTestInfo("dkSysGetSwitchAppId", temp);

}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsMultiplexUpDown(void) {
    bool ret;
    u8 temp[256] = {0};

    ret = sdkSysIsMultiplexUpDown();
    sprintf(temp, "IsMultiplexUpDown:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsMultiplexUpDown", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetBatteryStateAndControl(void) {
    s32 ret;
    u8 temp[256] = {0};

    //检查电源状态
    ret = sdkSysGetBatteryStateAndControl();
    sprintf(temp, "BatteryState:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysGetBatteryStateAndControl", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.08.22
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysIsExPower(void) {
    bool ret;
    u8 temp[256] = {0};

    //外接电源测试
    ret = sdkSysIsExPower();
    sprintf(temp, "IsExPower:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysIsExPower", temp);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.11.15
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetPowerStateManual(void) {
    s32 ret;
    u8 temp[256] = {0};

    //检查电源状态
    ret = sdkSysGetPowerState();
    sprintf(temp, "return:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysGetPowerState", temp);
    return;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.11.15
** Remarks: 		
*****************************************************************************/
static void Test_Private_sdkSysGetSlotNO(void) {
    s32 ret;
    u8 temp[256] = {0};
    //检查电源状态
    u8 slotno = 0;

    ret = Private_sdkSysGetSlotNO(&slotno);
    sprintf(temp, "slotno = %d, return:%d\r\n", slotno, ret);
    Test_sdkPrintTestInfo("Private_sdkSysGetSlotNO", temp);
    return;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2014.02.21
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysGetMaxLetterPerLine(void) {
    s32 ret = sdkSysGetMaxLetterPerLine();
    u8 temp[256] = {0};

    sprintf(temp, "max letter one line:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysGetMaxLetterPerLine", temp);
    return;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2014.02.21
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysMultiGetLangMode(void) {
    s32 ret = sdkSysMultiGetLangMode();
    u8 temp[256] = {0};

    sprintf(temp, "lang mode:%d\r\n", ret);
    Test_sdkPrintTestInfo("sdkSysMultiGetLangMode", temp);
    return;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2014.02.21
** Remarks: 		
*****************************************************************************/
static void Test_sdkSysMultiGetLineMode(void) {
    SDK_SYS_LINE_INFOR pstDipsLineInfo;
    s32 ret = sdkSysMultiGetLineMode(&pstDipsLineInfo);
    u8 temp[256] = {0};

    sprintf(temp, "line nmb:%d,line mode:%d, rtn:\r\n", pstDipsLineInfo.eLine,
            pstDipsLineInfo.eLineMode, ret);
    Test_sdkPrintTestInfo("sdkSysMultiGetLineMode", temp);
    return;
}

/*****************************************************************************
** Descriptions:	获取图片大小测试
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2014.04.16
** Remarks: 		
*****************************************************************************/
static void sdkTest_sdkGetBmpSize(void) {
    s32 width = 0, height = 0;
    u8 buf[512] = {0};
    if (false == sdkAccessFile("/mtd1/logo1.bmp")) {
        sdkDispMsgBox("sdkTest_sdkGetBmpSize", "/mtd1/logo1.bmp not exist\r\nany key continue\r\n",
                      0, SDK_KEY_MASK_ALL);
        sdkDispClearScreen();
        sdkDispBrushScreen();
    }
    sdkGetBmpSize(&width, &height, "/mtd1/logo1.bmp");
    sprintf(buf, "bmp:%s\r\nwidth:%d\r\nheight:%d\r\n", "/mtd1/logo1.bmp", width, height);
    Test_sdkPrintTestInfo("sdkGetBmpSize", buf);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.08.22
** Remarks:
*****************************************************************************/
void Test_sdkSysauto(void) {
    TestLog("TestSys", "\r\n开始sys测试\r\n ");
    s32 ret = sdkPrintInit();
    if (SDK_KEY_ESC == Test_sdkPrintShowErr(ret)) {
        return;
    }
    else {
        sdkPrintInit();
    }

    Test_sdkPrintTestInfo("系统自动测试模块", "NULL");

    sdkTest_sdkSysGetCurAppId();
    sdkTest_sdkSysGetCurAppDir();
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P
    sdkTest_sdkSysGetLibdevVersion();
    sdkTest_sdkSysGetPanelVersion();
    sdkTest_sdkSysGetDaemonVersion();
#endif
    sdkTest_sdkSysGetLibxgdemvVersion();

//    sdkTest_sdkSysGetLibappdeviceVersion();//shijianglong 2013.05.06 16:10
    sdkTestShowProcessing("已完成50%...");

    sdkTest_sdkSysGetLibMathsVersion();
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P
    sdkTest_sdkSysGetLibModuleVersion();
#endif
    sdkTest_sdkSysGetComNo();
    sdkTest_sdkSysStlGetMachinemodelVersion();
    sdkTest_sdkSysGetMachineModel();
    sdkTest_sdkSysGetMachineCode();
    sdkTest_sdkSysGetCupSn();
    sdkTest_sdkSysGetSwitchAppId();
    Test_sdkSysGetCurAppState();
    Test_sdkSysGetMisComNo();
    Test_sdkSysIsHaveBase();
    Test_sdkSysIsUclinuxPlat();
    Test_sdkSysIsColorScreen();
    Test_sdkSysGetScreenPixel();
    Test_sdkSysIsHaveSpeaker();
    Test_sdkSysIsPrintPin();
    Test_sdkSysGetCupSn();
#if (MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P)
    Test_sdkSysGetCommMask();
    Test_sdkSysGetDefaultCommuType();
#endif
    Test_sdkIsSupportPCI();
    Test_sdkSysIsSupportTSlib();
    Test_sdkSysGetSwitchAppId();
    Test_sdkSysIsMultiplexUpDown();
    Test_sdkSysGetBatteryStateAndControl();
    Test_sdkSysIsExPower();
    Test_sdkSysGetComNo();
    Test_sdkSysIsHavePinPad();
    Test_sdkSysGetAppReturnWelcome();
    Test_sdkSysGetPowerStateManual();
    Test_Private_sdkSysGetSlotNO();
    Test_sdkSysGetMaxLetterPerLine();
    Test_sdkSysMultiGetLangMode();
    Test_sdkSysMultiGetLineMode();
    sdkTest_sdkGetBmpSize();
    sdkPrintStart();

    return;
}

