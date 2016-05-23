#include "sdkGlobal.h"
#include "SdkTest.h"

#define InitMem(dsc) memset(&dsc, 0, sizeof(dsc))


extern s32 Private_sdkPEDSetSN(const u8 *pDataIn); //huacong 2013.03.29 14:41
/*****************************************************************************
** Descriptions:	��ȡ������̰汾
** Parameters:    	char *pasOutInfo
                               char const *pTitle
** Returned value:	
** Created By:		fusuipu  2014.04.04
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkPEDGetPedVersion(char *pasOutInfo, char const *pTitle) {
    u8 temp[64] = {0};
    s32 ret = 0;
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P/*Modify by ���ƽ at 2014.06.16  17:33 */
    if(sdkPEDIsWithPinpad() == false) //shijianglong 2013.06.08 14:53
    {
        sprintf(pasOutInfo, "�˺���ֻ֧�������������");
        return SDK_OK;
    }
#endif /* if MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P */
    ret = sdkPEDGetPedVersion(temp);
    //Trace("zjp", "temp = %s\r\n", temp );
    sprintf(pasOutInfo, "%s\rreturn:%d", temp, ret);
    return 1;
}

static s32 Test_sdkPEDSetSN(char *pasOutInfo, char const *pTitle) {
    u8 temp[64] = {0};
    s32 ret = 0;

    if (sdkPEDIsWithPinpad() == false)//shijianglong 2013.06.08 14:53
    {
        sprintf(pasOutInfo, "�˺���ֻ֧�������������");
        return SDK_OK;
    }

    ret = sdkTestEditBox(pTitle, "������SN��", temp, 1, 11, SDK_MMI_NUMBER | SDK_MMI_LETTER);
    if (ret == SDK_OK) {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��[ȷ��]����", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();

        ret = Private_sdkPEDSetSN(&temp[1]);

        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
        sdkmSleep(500);
        sdkPEDCancel();
    }
    sprintf(pasOutInfo, "return:%d", ret);
    return 1;
}

static s32 Test_sdkPEDGetPedSn(char *pasOutInfo, char const *pTitle) {
    u8 temp[64] = {0};
    s32 ret = 0;

    if (sdkPEDIsWithPinpad() == false)//shijianglong 2013.06.08 14:53
    {
        sprintf(pasOutInfo, "�˺���ֻ֧�������������");
        return SDK_OK;
    }
    else {
        ret = sdkPEDGetPedSn(temp);
        sprintf(pasOutInfo, "SN:%s \rreturn:%d", temp, ret);
        return 1;
    }
}


static s32 Test_sdkPEDSetLanguage(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    u8 uc = sdkTestInputU8(pTitle, "����0 Ӣ��1:");

    if (sdkPEDIsWithPinpad() == false) {
        sprintf(pasOutInfo, "�˺���ֻ֧�������������");
        return SDK_OK;
    }
    sdkDispClearScreen();
    if ((SDK_PED_LANGUAGE) uc == SDK_PED_CH) {
        ret = sdkPEDSetLanguage(SDK_PED_CH);
    }
    else {
        ret = sdkPEDSetLanguage(SDK_PED_EN);
    }

    sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�鿴PED����", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "δ�ı���γ��ٲ���PED", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "�ٰ�[ȷ��]����", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

    sprintf(pasOutInfo, "input:%d \rreturn:%d", uc, ret);
    return 1;
}

static s32 Test_sdkPEDDispAmt(char *pasOutInfo, char const *pTitle) {
    s32 ret1 = 0;
    u8 temp[64] = {0};
    u8 amt[8] = {0};
    u32 uiamt = 0.0;
    s32 disp_mode = 0;

    if (!sdkPEDIsWithPinpad() && (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)) {
        sdkTestMsgBox("�û����������������ģ��", SDK_KEY_MASK_ENTER);
        return -1;
    }
    ret1 = sdkTestEditBox(pTitle, "������:", temp, 0, 9, SDK_MMI_NUMBER | SDK_MMI_POINT);
    disp_mode = sdkTestInputU8(pTitle, "������ʾ-1 ������ʾ-0:");

    //Trace( "zjp", "&temp[1] = %s\r\n", &temp[1] );
    if (ret1 == SDK_OK) {
        sdkAscToBcdR(amt, &temp[1], 6);
        //TraceHex( "zjp", "amt", amt, 6 );
        if (0 != disp_mode) {
#if 0 /*Modify by ���ƽ at 2014.06.16  10:57 */
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������ʾ", SDK_DISP_DEFAULT);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "PED[ȷ��]/[ȡ��]", SDK_DISP_DEFAULT);
            sdkDispBrushScreen();
            sdkKbWaitKey(SDK_KEY_MASK_ENTER|SDK_KEY_MASK_ESC, 0);
#endif /* if 0 */

            ret1 = sdkPEDDispAmt(SDK_PED_POSITIVE, amt);
        }
        else {
#if 0 /*Modify by ���ƽ at 2014.06.16  10:57 */
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������ʾ", SDK_DISP_DEFAULT);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "PED[ȷ��]/[ȡ��]", SDK_DISP_DEFAULT);
            sdkDispBrushScreen();
            sdkKbWaitKey(SDK_KEY_MASK_ENTER|SDK_KEY_MASK_ESC, 0);
#endif /* if 0 */

            ret1 = sdkPEDDispAmt(SDK_PED_NEGATIVE, amt);
        }

        if (true == sdkPEDIsWithPinpad()) {
            sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);
            sdkmSleep(500);
            sdkPEDCancel();
        }
        //Trace( "zjp", "ret1 = %d\r\n", ret1 );
        TraceHex("zjp", "amt", amt, 6);
        sdkBcdToU32(&uiamt, amt, 6);
        //Trace( "zjp", "uiamt = %d\r\n", uiamt );
    }

    sprintf(pasOutInfo, "input:%d.%02d \rreturn:%d", uiamt / 100, uiamt % 100, ret1);
    return 1;
}

static s32 Test_sdkPEDDispBalance(char *pasOutInfo, char const *pTitle) {
    s32 ret1 = 0, ret2 = 0;
    u8 temp[64] = {0};
    u8 amt[8] = {0};
    u32 uiamt1, uiamt2, uiDecimal;

    if (!sdkPEDIsWithPinpad() && (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)) {
        sdkTestMsgBox("�û����������������ģ��", SDK_KEY_MASK_ENTER);
        return -1;
    }
    ret1 = sdkTestEditBox(pTitle, "�������:", temp, 0, 12, SDK_MMI_NUMBER | SDK_MMI_POINT);
    if (ret1 == SDK_OK) {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������ʾ", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��[ȷ��]����", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

        sdkAscToBcdR(amt, &temp[1], 6);
        ret1 = sdkPEDDispBalance(SDK_PED_POSITIVE, amt);

        sdkDispClearScreen();
        sdkDispClearRowRam(SDK_DISP_LINE2);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������ʾ", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        sdkKbWaitKey(SDK_KEY_MASK_ENTER, 0);

        ret2 = sdkPEDDispBalance(SDK_PED_NEGATIVE, amt);

        sdkPEDCancel();

        sdkBcdToU32(&uiamt1, &amt[0], 2);
        sdkBcdToU32(&uiamt2, &amt[2], 3);
        sdkBcdToU32(&uiDecimal, &amt[5], 1);
    }
    if (uiamt1 > 0) {
        sprintf(pasOutInfo, "input:%d%06d.%02d \rreturn:%d, %d", uiamt1, uiamt2, uiDecimal, ret1,
                ret2);
    }
    else {
        sprintf(pasOutInfo, "input:%d.%02d \rreturn:%d, %d", uiamt2, uiDecimal, ret1, ret2);
    }
    return 1;
}

static s32 Test_sdkPEDInputPIN1(char *pasOutInfo, char const *pTitle) {
    u8 pinbuf[30] = {0};
    s32 ret = 0;
    SDK_PED_PIN_CFG stPinCfg;
    memset(&stPinCfg, 0, sizeof(SDK_PED_PIN_CFG));
    stPinCfg.ePinMode = SDK_PED_IC_OFFLINE_PIN;
    stPinCfg.eKeyType = SDK_PED_DES_SINGLE;
    memset(stPinCfg.hePan, 0, 8);
    stPinCfg.iTimeOut = SDK_PED_TIMEOUT * 15;
    stPinCfg.ucTpkIndex = 0;


    if (!sdkPEDIsWithPinpad() && (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)) {
        sdkTestMsgBox("�û����������������ģ��", SDK_KEY_MASK_ENTER);
        return -1;
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��PED����IC�ѻ�����", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();

    ret = sdkPEDInputPIN(&stPinCfg, "\x00\x00\x00\x12\x34\x56", pinbuf);
    sdkmSleep(500);
    sdkPEDCancel();
    sprintf(pasOutInfo, "input:%s \rreturn:%d", &pinbuf[1], ret);
    return 1;
}

static s32 Test_sdkPEDInputPIN2(char *pasOutInfo, char const *pTitle) {
    s32 ret1 = SDK_ERR, ret2 = SDK_ERR, ret3 = SDK_ERR, ret4 = SDK_ERR, ret = 0;
    SDK_PED_PIN_CFG stPinCfg;
    SDK_PED_WK_CFG stPedWkCfg[2];
    u8 temp[30] = {0}, buf[30] = {0};
    u8 *p = NULL;
    memset(temp, 0x31, sizeof(temp));
    memset(&stPinCfg, 0, sizeof(SDK_PED_PIN_CFG));

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "������Կ��...", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();
    //������Կ
    ret1 = sdkPEDUpdateTmk(0, SDK_PED_DES_SINGLE, temp, SDK_PED_TIMEOUT * 2);
    sdkmSleep(200);
    if (ret1 == SDK_OK) {

        memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG) * 2);
        stPedWkCfg[0].bOnlyCalcCheckValue = false;

        stPedWkCfg[0].eTmkType = SDK_PED_DES_SINGLE;
        stPedWkCfg[1].eTmkType = SDK_PED_DES_SINGLE;

        stPedWkCfg[0].eWkType = SDK_PED_DES_SINGLE;
        stPedWkCfg[1].eWkType = SDK_PED_DES_SINGLE;

        stPedWkCfg[0].ucTmkIndex = 0;
        stPedWkCfg[1].ucTmkIndex = 0;

        stPedWkCfg[0].ucWkIndex = 0;
        stPedWkCfg[1].ucWkIndex = 1;

        stPedWkCfg[0].ucEnWkLen = 8;
        stPedWkCfg[1].ucEnWkLen = 8;

        memset(stPedWkCfg[0].heCheckData, 0, 24);
        memset(stPedWkCfg[1].heCheckData, 0, 24);

        //3131313131313131//����Կ
        //F92714E80E832EDC//ADC67D84//4BF6E91B1E3A9D81//ADC67D84//���������Լ�checkvalue
        //3232323232323232//3333333333333333//���ܺ�����
        memcpy(stPedWkCfg[0].heEnWk, "\xF9\x27\x14\xE8\x0E\x83\x2E\xDC", 8);
        memcpy(stPedWkCfg[1].heEnWk, "\x4B\xF6\xE9\x1B\x1E\x3A\x9D\x81", 8);

        stPedWkCfg[0].ucCheckDataLen = 8;
        stPedWkCfg[1].ucCheckDataLen = 8;


        stPedWkCfg[0].ucCheckValueLen = 4;
        stPedWkCfg[1].ucCheckValueLen = 4;

        memcpy(stPedWkCfg[0].heCheckValue, "\xAD\xC6\x7D\x84", 4);
        memcpy(stPedWkCfg[1].heCheckValue, "\xAD\xC6\x7D\x84", 4);

        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�๤����Կ��...", SDK_DISP_LEFT_DEFAULT);
        sdkDispBrushScreen();
        //�๤����Կ
        ret2 = sdkPEDUpdateWk(&stPedWkCfg[0], 2, SDK_PED_TIMEOUT * 5);
        sdkmSleep(200);
        if (ret2 == SDK_OK) {
            memset(temp, 0, sizeof(temp));
            memset(buf, 0, sizeof(buf));
            ret = sdkTestEditBox(pTitle, "PED��Ҫ��ʾ�Ľ��:", temp, 0, 12,
                                 SDK_MMI_NUMBER | SDK_MMI_POINT);
            if (ret == SDK_KEY_ENTER) {
                sdkAscToBcdR(buf, &temp[1], 6);
                p = buf;
            }
            memset(temp, 0, sizeof(temp));
            while (1) {
                ret = sdkTestEditBox(pTitle, "���볤��:0-6λ1-12λ", temp, 1, 1, SDK_MMI_NUMBER);
                if (ret == SDK_KEY_ENTER) {
                    if (temp[1] > '1') { continue; }
                    if (temp[1] == '0') { stPinCfg.ePinMode = SDK_PED_MAG_PIN; }
                    else { stPinCfg.ePinMode = SDK_PED_IC_ONLINE_PIN; }

                }
                else {
                    stPinCfg.ePinMode = SDK_PED_MAG_PIN;
                }
                break;
            }
            stPinCfg.eKeyType = SDK_PED_DES_SINGLE;
            memcpy(stPinCfg.hePan, "\x00\x00\x67\x89\x01\x23\x45\x67", 8);
            stPinCfg.iTimeOut = SDK_PED_TIMEOUT * 8;
            stPinCfg.ucTpkIndex = 0;
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
            if (stPinCfg.ePinMode == SDK_PED_MAG_PIN) {
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��PED����123456", SDK_DISP_LEFT_DEFAULT);
            }
            else {
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��PED����123456123456",
                                  SDK_DISP_LEFT_DEFAULT);
            }
            sdkDispBrushScreen();
            memset(temp, 0, sizeof(temp));
            ret3 = sdkPEDInputPIN(&stPinCfg, p, temp);
            if (ret3 == SDK_OK) {
                TestLogHex("ped", "Pin", temp, 8);
                if (stPinCfg.ePinMode == SDK_PED_MAG_PIN) {
                    if (!memcmp(&temp[1], "\x11\x09\x03\xaf\xea\xde\x72\xa3", 8)) { ret4 = SDK_OK; }
                }
                else {
                    if (!memcmp(&temp[1], "\xfe\x94\xf4\x68\xa5\x9a\x90\xd5", 8)) { ret4 = SDK_OK; }
                }
            }
            sdkmSleep(500);
        }
    }
    sdkPEDCancel();
    sprintf(pasOutInfo, "FlushTmk:%d FlushWk:%d\rInputPin:%d Result:%d", ret1, ret2, ret3, ret4);
    return 1;
}

static s32 Test_sdkPEDInputAmt(char *pasOutInfo, char const *pTitle) {
    u8 temp[30] = {0};
    s32 ret = 0;
    u32 uiamt1, uiamt2, uiDecimal;

    if (!sdkPEDIsWithPinpad() && (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)) {
        sdkTestMsgBox("�û����������������ģ��", SDK_KEY_MASK_ENTER);
        return -1;
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTitle, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��PED������", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();

    ret = sdkPEDInputAmt(temp);
    sdkmSleep(500);
    sdkPEDCancel();

    sdkBcdToU32(&uiamt1, &temp[0], 4);
    sdkBcdToU32(&uiamt2, &temp[4], 3);
    sdkBcdToU32(&uiDecimal, &temp[7], 1);
    if (uiamt1 > 0) {
        sprintf(pasOutInfo, "input:%d%06d.%02d \rreturn:%d", uiamt1, uiamt2, uiDecimal, ret);
    }
    else {
        sprintf(pasOutInfo, "input:%d.%02d \rreturn:%d", uiamt2, uiDecimal, ret);
    }

    return 1;
}

static s32 Test_sdkPEDDisplayStr(char *pasOutInfo, char const *pTitle) {

    if (sdkTestIsUcos()) //huacong 2013.03.28 11:48
    {
        sdkTestMsgBox("�û����ݲ�֧�ָ�ģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
    }
    else {
        s32 ret = 0;
        u8 temp[64] = {0};
        u8 temp2[64] = {0};
        //    u8 temp3[64] ={0};
        u8 ucCol = 0, ucAtr = 0;
        ret = sdkTestEditBox(pTitle, "������PED��ʾ����:", temp, 1, 15,
                             SDK_MMI_NUMBER | SDK_MMI_HZ | SDK_MMI_SYMBOL | SDK_MMI_LETTER);
        if (ret == SDK_OK) {
            while (1) {
                ret = sdkTestEditBox(pTitle, "0-��һ�� 1-�ڶ���:", temp2, 1, 1, SDK_MMI_NUMBER);
                if (ret == SDK_OK) {
                    if (temp2[1] > '1') continue;
                }
                else {
                    temp2[1] = '0';
                }
                break;
            }
            ucCol = sdkTestInputU8(pTitle, "ucCol:");
            ucAtr = sdkTestInputU8(pTitle, "ucAtr:");
            ret = sdkPEDDisplayStr(&temp[1], atoi(&temp2[1]), ucCol, ucAtr);
        }
        sprintf(pasOutInfo, "input:%s\rrow:%d\rucCol:%d\rucAtr:%d\rreturn:%d", &temp[1],
                atoi(&temp2[1]), ucCol, ucAtr, ret);
        return 1;
    }
    return 1;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	char *pasOutInfo
                               char const *pTitle
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkPEDLampControl(char *pasOutInfo, char const *pTitle) {
    s32 light_state = 0;
    SDK_PED_LAMP_CTRL ped_lcd = {0};
    s32 ret = 0;
    u8 *ledState[3] = {"��", "��", "��˸"};

    sdkDispMsgBox("��ʾ��Ϣ", "���������ó����÷ǽӾͿ��Ƶ������õƷ��������õƣ���ʱֻ֧��3102", 0,
                  SDK_KEY_MASK_ALL);

    light_state = sdkTestInputU8(pTitle, "LED1: 0-off 1-on 2-flash");
    ped_lcd.eLamp1 = light_state;
    light_state = sdkTestInputU8(pTitle, "LED2: 0-off 1-on 2-flash");
    ped_lcd.eLamp2 = light_state;
    light_state = sdkTestInputU8(pTitle, "LED3: 0-off 1-on 2-flash");
    ped_lcd.eLamp3 = light_state;
    light_state = sdkTestInputU8(pTitle, "LED4: 0-off 1-on 2-flash");
    ped_lcd.eLamp4 = light_state;

    if (ped_lcd.eLamp1 == SDK_LAMP_TWINK || ped_lcd.eLamp2 == SDK_LAMP_TWINK ||
        ped_lcd.eLamp3 == SDK_LAMP_TWINK || ped_lcd.eLamp4 == SDK_LAMP_TWINK) {
        light_state = sdkTestInputU32(pTitle, "FLASH TOTAL TIME:(ms)");
        ped_lcd.siTimers = light_state;
    }
    ret = sdkPEDLampControl(&ped_lcd);

    sprintf(pasOutInfo, "led1:%s\rled2:%s\rled3:%s\rled4:%s\rreturn:%d", ledState[ped_lcd.eLamp1],
            ledState[ped_lcd.eLamp2], ledState[ped_lcd.eLamp3], ledState[ped_lcd.eLamp4], ret);
    return 1;
}


/*****************************************************************************
** Descriptions:	led ���� ��չ��������
** Parameters:    	char *pasOutInfo
                               char const *pTitle
** Returned value:	
** Created By:		���ƽ  2014.07.16
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkPEDLampControlExt(char *pasOutInfo, char const *pTitle) {
    s32 light_state = 0;
    SDK_PED_LAMP_CTRL_EXT ped_lcd[4];
    s32 ret = 0;
    u8 *ledState[3] = {"��", "��", "��˸"};
    sdkDispMsgBox("��ʾ��Ϣ",
                  "���������ó����÷ǽӾͿ��Ƶ������õƷ��������õ�(��ʱֻ֧��3102)��(G2&200P ֻ֧������)", 0,
                  SDK_KEY_MASK_ALL);

    /*ped lamp1*/
    light_state = sdkTestInputU8(pTitle, "LED1: 0-off 1-on 2-flash");
    ped_lcd[0].lempState = light_state;
    ped_lcd[0].lempID = SDK_PED_LAMP_1;
    if (ped_lcd[0].lempState == SDK_LAMP_TWINK) {
        light_state = sdkTestInputU32(pTitle, "LED1:light:(ms)");
        ped_lcd[0].siOnTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED1:off:(ms)");
        ped_lcd[0].siOffTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED1:FLASH TOTAL TIME:(ms)");
        ped_lcd[0].siTimers = light_state;
    }
    /*ped lamp2*/
    light_state = sdkTestInputU8(pTitle, "LED2: 0-off 1-on 2-flash");
    ped_lcd[1].lempState = light_state;
    ped_lcd[1].lempID = SDK_PED_LAMP_2;
    if (ped_lcd[1].lempState == SDK_LAMP_TWINK) {
        light_state = sdkTestInputU32(pTitle, "LED2:light:(ms)");
        ped_lcd[1].siOnTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED2:off:(ms)");
        ped_lcd[1].siOffTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED2:FLASH TOTAL TIME:(ms)");
        ped_lcd[1].siTimers = light_state;
    }
    /*ped lamp3*/
    light_state = sdkTestInputU8(pTitle, "LED3: 0-off 1-on 2-flash");
    ped_lcd[2].lempState = light_state;
    ped_lcd[2].lempID = SDK_PED_LAMP_3;
    if (ped_lcd[2].lempState == SDK_LAMP_TWINK) {
        light_state = sdkTestInputU32(pTitle, "LED3:light:(ms)");
        ped_lcd[2].siOnTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED3:off:(ms)");
        ped_lcd[2].siOffTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED3:FLASH TOTAL TIME:(ms)");
        ped_lcd[2].siTimers = light_state;
    }
    /*ped lamp4*/
    light_state = sdkTestInputU8(pTitle, "LED4: 0-off 1-on 2-flash");
    ped_lcd[3].lempState = light_state;
    ped_lcd[3].lempID = SDK_PED_LAMP_4;
    if (ped_lcd[3].lempState == SDK_LAMP_TWINK) {
        light_state = sdkTestInputU32(pTitle, "LED4:light:(ms)");
        ped_lcd[3].siOnTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED4:off:(ms)");
        ped_lcd[3].siOffTimers = light_state;
        light_state = sdkTestInputU32(pTitle, "LED4:FLASH TOTAL TIME:(ms)");
        ped_lcd[3].siTimers = light_state;
    }

    ret = sdkPEDLampControlExt(4, ped_lcd);

    sprintf(pasOutInfo, "led1:%s\rled2:%s\rled3:%s\rled4:%s\rreturn:%d",
            ledState[ped_lcd[0].lempState],
            ledState[ped_lcd[1].lempState], ledState[ped_lcd[2].lempState],
            ledState[ped_lcd[3].lempState], ret);
    return 1;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
static const TEST_MANUAL_MAP PedManaulMap[] =
        {
                {"sdkPEDGetPedVersion", Test_sdkPEDGetPedVersion},
                {"sdkPEDLampControl", Test_sdkPEDLampControl},
                {"sdkPEDLampControlExt", Test_sdkPEDLampControlExt},
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P/*Modify by ���ƽ at 2014.06.16  17:33 */
                {"sdkPEDSetSN", Test_sdkPEDSetSN},
                {"sdkPEDGetPedSn", Test_sdkPEDGetPedSn},
                {"sdkPEDSetLanguage", Test_sdkPEDSetLanguage},
#endif /* if MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P */
                {"sdkPEDDispAmt", Test_sdkPEDDispAmt},
                {"sdkPEDDispBalance", Test_sdkPEDDispBalance},
                {"sdkPEDInputPIN(�ѻ�)", Test_sdkPEDInputPIN1},
                {"sdkPEDInputPIN(����)", Test_sdkPEDInputPIN2},
                {"sdkPEDInputAmt", Test_sdkPEDInputAmt},
                {"sdkPEDDisplayStr", Test_sdkPEDDisplayStr},
        };

TEST_MAP_INFO GetMPedTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(PedManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = PedManaulMap;
    return ret;
}
