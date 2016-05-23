#include "sdkGlobal.h"
#include "SdkTest.h"

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.07.05
** Remarks:
*****************************************************************************/
static s32 sdkTest_sdkSysSetBlackLightCloseTimeManual(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    s32 sec = 0;
    u8 buf[128] = {0};

    sec = sdkTestInputs32(pTitle, "set light time(ms):");
    sprintf(buf, "����ʱ��:%d", sec);
    sdkDispClearScreen();
    sdkDispFillRowRam(0, 0, "��ʼ�������", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(1, 0, buf, SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(2, 0, "�������������", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();
    ret = sdkSysSetBlackLightCloseTime(sec);
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);       //�ȴ�����

    sprintf(pasOutInfo, "set light time:%d \rret:%d \r", sec, ret);
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.07.05
** Remarks:
*****************************************************************************/
static s32 sdkTest_sdkSysBlackLightOnOrOffManual(char *pasOutInfo, char const *pTitle) {
    s32 state = 0;
    u8 buf[128] = {0};

    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    state = sdkTestInputs32(pTitle, "1-light on 0-light off");
    if (state == 1) {
        sdkSysBlackLightOn();
        strcpy(buf, "on");
    }
    else if (state == 0) {
        sdkSysBlackLightOff();
        strcpy(buf, "off");
    }
    sprintf(pasOutInfo, "set light state:%s,please check! \r", buf);
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.07.05
** Remarks:
*****************************************************************************/
static s32 sdkTest_sdkSysBlackLightCtroManual(char *pasOutInfo, char const *pTitle) {
    s32 state = 0;
    u8 buf[128] = {0};

    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }
    state = sdkTestInputs32(pTitle, "1-Enbale ctr 0-unEnable ctr");
    if (state == 1) {
        sdkSysBlackLightCtro(true);
        strcpy(buf, "Enable");
    }
    else if (state == 0) {
        sdkSysBlackLightCtro(false);
        strcpy(buf, "unEnable");
    }
    sprintf(pasOutInfo, "ctr light state:%s,please check! \r", buf);
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.07.05
** Remarks:
*****************************************************************************/
static s32 sdkTest_sdkSysSetBeepTuneManual(char *pasOutInfo, char const *pTitle) {
    s32 freq = 0;
    s32 atio = 0;
    s32 ret = 0;
    s32 mach_type = 0;
    u8 buf[128] = {0};
    mach_type = sdkSysGetMachineCode(NULL);

    //if (mach_type == SDK_SYS_MACHINE_K350 || mach_type == SDK_SYS_MACHINE_K301FZ || mach_type == SDK_SYS_MACHINE_K370 || mach_type == SDK_SYS_MACHINE_K320)
#if  ((MACHINEMODEL == SDK_SYS_MACHINE_G2) || (MACHINEMODEL == SDK_SYS_MACHINE_200P) || (MACHINEMODEL == SDK_SYS_MACHINE_K301FZ) || (MACHINEMODEL == SDK_SYS_MACHINE_K350) || (MACHINEMODEL == SDK_SYS_MACHINE_K370) || (MACHINEMODEL == SDK_SYS_MACHINE_K320))
    {
        freq = sdkTestInputs32(pTitle, "set freq(hz):");
        atio = sdkTestInputs32(pTitle, "set atio(ms):");

        ret = sdkSysSetBeepTune(freq, atio);

        sprintf(pasOutInfo, "freq(hz):%d \atio(ms):%d \rret:%d \r", freq, atio, ret);
    }
    //else
#else
    {
        sprintf(buf, "%d��֧�ִ˲���!", mach_type);
        strcat(pasOutInfo, buf);
    }
#endif
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.07.05
** Remarks:
*****************************************************************************/
static s32 sdkTest_sdkSysSetLcdContrastManual(char *pasOutInfo, char const *pTitle) {
    s32 light_lev = 0;
    s32 ret = 0;

    light_lev = sdkTestInputs32(pTitle, "set LightLev(20-60):");
    ret = sdkSysSetLcdContrast(light_lev);

    sprintf(pasOutInfo, "set light time:%d \rret:%d \r", light_lev, ret);
    return 0;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.07.05
** Remarks: 		
*****************************************************************************/
static s32 sdkTest_sdkSysMultiLangInitManual(char *pasOutInfo, char const *pTitle) {

    s32 ret = 0;
    s32 lang_style = 0;
//    u8 str_buf[1024] = {0};
    u8 *str_buf = NULL;

    if (sdkTestIsUcos() || SDK_SYS_MACHINE_G2 || SDK_SYS_MACHINE_200P) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return -1;
    }

    str_buf = sdkGetMem(1024);
    if (str_buf == NULL) {
        Assert(0);
        return -1;
    }
    memset(str_buf, 0, sizeof(str_buf));

    lang_style = sdkTestInputs32(pTitle, "set lang:0-CHN 1-ENG");
    sdkTestInputASC(str_buf, "����ת������", "input string:", 1, 100);
    ret = sdkSysMultiLangInit(lang_style, 5);

    sdkDispClearScreen();
    sdkDispFillRowRam(0, 0, str_buf, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    sprintf(pasOutInfo, "input string:%s\rret:%d", str_buf, ret);
    sdkSysMultiLangInit(0, 5);
    sdkFreeMem(str_buf);
    return 0;
}


static s32 sdkTest_sdkSet_GetVolume(char *pasOutInfo, char const *pTitle) {
    s32 oldLevel = 0;
    s32 newLevel = 0;
    s32 level = sdkTestInputs32(pTitle, "�����������ֵ����Χ:0-7");
    s32 ret = 0;

    oldLevel = sdkSysGetVolume();
    ret = sdkSysSetVolume(level);
    newLevel = sdkSysGetVolume();
    sprintf(pasOutInfo, "ϵͳ����ǰ����:%d\r�����õ�����:%d\r��ǰϵͳ����:%d\r����ֵ:%d", oldLevel, level,
            newLevel, ret);
    return 0;
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
extern s32 DisplayList(TEST_MAP const *pMap, s32 conter);

static s32 sdkTest_sdkSysPlaySound(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    s32 voice_style = 0;
    s32 level = 0;

    static const TEST_MAP sAutoTestMap[] =
            {
                    {"SOUND_OK",                NULL},
                    {"SOUND_ERR",               NULL},
                    {"SOUND_INPUTPIN",          NULL},
                    {"SOUND_RING_1",            NULL},
                    {"SOUND_RING_2",            NULL},
                    {"SOUND_RING_3",            NULL},
                    {"SOUND_USER_1",            NULL},
                    {"SOUND_USER_2",            NULL},
                    {"SOUND_USER_3",            NULL},
                    {"SOUND_USER_4",            NULL},
                    {"SOUND_USER_5",            NULL},
                    {"SOUND_USER_6",            NULL},
                    {"SOUND_RD_CRD",            NULL},
                    {"RD_CARD_AGAIN",           NULL},
                    {"INPUT_AMT",               NULL},
                    {"INPUT_AMT_AGAIN",         NULL},
                    {"INPUT_OR_READ_CARD",      NULL},
                    {"SOUND_INPUT",             NULL},
                    {"SOUND_CONFIRM",           NULL},
                    {"SOUND_SELECT",            NULL},
                    {"SOUND_INSERT_CARD",       NULL},
                    {"SOUND_INSERT_OR_RD_CARD", NULL},
            };

    while (1) {
        //�������ͳ������

        voice_style = DisplayList(sAutoTestMap, sizeof(sAutoTestMap) / sizeof(TEST_MAP));
        if ((sizeof(sAutoTestMap) / sizeof(TEST_MAP)) <= voice_style) {
            sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            return -1;
        }
        if (voice_style < 0) {
            return -1;
        } else {
            break;
        }
    }
    Trace("likun", "voice type == %d\r\n", voice_style);
    level = sdkTestInputs32(pTitle, "��Χ:0-7");
    if ((MACHINEMODEL == SDK_SYS_MACHINE_G2 && voice_style >= SDK_SYS_BEEP_USER_1 &&
         voice_style <= SDK_SYS_BEEP_USER_6) ||
        (sdkTestIsUcos() && voice_style >= SDK_SYS_BEEP_USER_1 &&
         voice_style <= SDK_SYS_BEEP_USER_6)) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return 0;
    } else {
        sdkDispMsgBox(pTitle, "���Ժ�....", 1500, SDK_KEY_MASK_9);
    }
    ret = sdkSysPlaySound(voice_style, level);
    sprintf(pasOutInfo, "voice_style:%d\rlevel:%d\rret:%d", voice_style, level, ret);
    return 0;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const*pTitle
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
static s32 sdkTest_sdkSysPlaySoundFile(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    s32 voice_style = 0;
    s32 level = 0;
    u8 buf[128] = {0};

    if (sdkTestIsUcos()) {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return 0;
    }
    voice_style = sdkTestInputs32(pTitle, "voice file:0-ok file 1-err file 3-pin voice file");
    level = sdkTestInputs32(pTitle, "voice lever:1-low 4-mid 7-high");
#if 0 /*Modify by wangmingming at 2014.05.28  10:47 */
    if(0 == voice_style)
    {
        strcpy(buf, "/mtd0/res/beepok.wav");
    }
    else if(1 == voice_style)
    {
        strcpy(buf, "/mtd0/res/beeperr.wav");
    }
    else
    {
        strcpy(buf, "/mtd0/res/inputpwd.wav");
    }
#else
    if (0 == voice_style) {
        voice_style = SDK_SYS_BEEP_OK;
    }
    else if (1 == voice_style) {
        voice_style = SDK_SYS_BEEP_ERR;
    }
    else {
        voice_style = SDK_SYS_BEEP_INPUT_PIN;
    }
    //Trace("test", "\r\n>>> buf=%s voice_style=%d <<<\r\n", buf, voice_style);
#endif /* if 0 */
    //ret = sdkSysPlaySoundFile(buf, level);
    ret = sdkSysPlaySound(voice_style, level);
    sprintf(pasOutInfo, "voice_file:%d\rlevel:%d\rret:%d", buf, level, ret);
    return 0;
}

static s32 Test_sdkSysEnterIdleManual(char *pasOutInfo, char const *pTitle) {
    u8 flag1, flag2;
    s32 ret;

    //�͹��Ĳ���
    flag1 = sdkTestInputU8(pTitle, "��һ������");
    flag2 = sdkTestInputU8(pTitle, "�ڶ�������");
    ret = sdkSysEnterIdle(flag1, flag2);
    sprintf(pasOutInfo, "bFlag:%d uOption:%d return:%d", flag1, flag2, ret);
    return 0;
}

#if 0
static s32 Test_sdkSysIdleWithAlarm(char *pasOutInfo, char const*pTitle)
{
    u8 sec, type, status;
    s32 ret;

    //����ʱ���ѹ��ܵ�ϵͳ���߲���
    sec = sdkTestInputU8(pTitle, "��һ������");
    type = sdkTestInputU8(pTitle, "�ڶ�������");
    status = sdkTestInputU8(pTitle, "����������");
    ret = sdkSysIdleWithAlarm( sec, type, status );
    sprintf(pasOutInfo, "sec:%d\rtype:%d\rstatus:%d\rreturn:%d", sec, type, status, ret);
    return 0;
}
#endif

static s32 Test_sdkSysCmd_Reboot(char *pasOutInfo, char const *pTitle) {
    s32 ret;

    //�͹��Ĳ���
    ret = sdkTestMsgBox("�����ڽ�����������,ϵͳ���Զ���������ȡ�����˳�,���������������!!",
                        SDK_KEY_MASK_ALL);
    if (ret == SDK_KEY_ESC) {
        return 0;
    }
    ret = sdkSysCmd("reboot", NULL);
    sprintf(pasOutInfo, " return:%d", ret);
    return 0;
}

static s32 Test_sdkSysDownloadMode(char *pasOutInfo, char const *pTitle) {
    SDK_DOWNLOAD_MODE downloadPara;
    s32 ret;

    //Ӧ�����ؽӿڲ���
    ret = sdkTestMsgBox("�����ڽ���Ӧ�����ز���,ϵͳ���Զ��������أ���ȡ�����˳�,���������������!!",
                        SDK_KEY_MASK_ALL);
    if (ret == SDK_KEY_ESC) {
        return 0;
    }
    memset(&downloadPara, 0, sizeof(SDK_DOWNLOAD_MODE));
    downloadPara.uiType = sdkTestInputU8(pTitle, "����������ģʽ:1:����,3:����");
    ret = sdkSysDownloadMode(&downloadPara);
    sprintf(pasOutInfo, "return:%d", ret);
    return 0;
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
static const TEST_MANUAL_MAP SysManaulMap[] =
        {
                {"sdkSysSet_GetVolume", sdkTest_sdkSet_GetVolume},
#if MACHINEMODEL != SDK_SYS_MACHINE_200P
                {"sdkSysPlaySound", sdkTest_sdkSysPlaySound},
#endif
                {"sdkSysPlaySoundFile", sdkTest_sdkSysPlaySoundFile},
                {"sdkSysMultiLangInit", sdkTest_sdkSysMultiLangInitManual},
                {"sdkSysSetBlackLightCloseTime", sdkTest_sdkSysSetBlackLightCloseTimeManual},
                {"sdkSysBlackLightOnOrOff", sdkTest_sdkSysBlackLightOnOrOffManual},
                {"sdkSysBlackLightCtro", sdkTest_sdkSysBlackLightCtroManual},
                {"sdkSysSetBeepTune", sdkTest_sdkSysSetBeepTuneManual},
                {"sdkSysSetLcdContrast", sdkTest_sdkSysSetLcdContrastManual},
                {"sdkSysEnterIdle", Test_sdkSysEnterIdleManual},
                //{"sdkSysIdleWithAlarm", Test_sdkSysIdleWithAlarm},
                {"sdkSysCmd_Reboot", Test_sdkSysCmd_Reboot},
                {"sdkSysDownloadMode", Test_sdkSysDownloadMode},
        };

TEST_MAP_INFO GetSysManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(SysManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = SysManaulMap;
    return ret;
}

