#include "sdkdevOption.h"
#include "sdkGlobal.h"

#ifndef  DEBUG_KEY
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif



//ϵͳ������ʵֵ
#define SYS_POWER_OFF   (0xEF) //ϵͳ�ػ�
/****************************k200P��ϵͳ����ֵ����ת����*****************************/
#if MACHINEMODEL == SDK_SYS_MACHINE_200P
#define POWER           (0x01) //ϵͳ�ػ�
#define F1              (POWER + 1)
#define F2              (F1 + 1)
#define F3              (F2 + 1)
#define FUNCTION        (F2 + 1)
#define DIGITAL1        (F3 + 1)
#define DIGITAL2        (DIGITAL1 + 1)
#define DIGITAL3        (DIGITAL2 + 1)
#define ALPHA           (DIGITAL3 + 1)
#define DIGITAL4        (ALPHA + 1)
#define DIGITAL5        (DIGITAL4 + 1)
#define DIGITAL6        (DIGITAL5 + 1)
#define ESC             (DIGITAL6 + 1)
#define DIGITAL7        (ESC + 1)
#define DIGITAL8        (DIGITAL7 + 1)
#define DIGITAL9        (DIGITAL8 + 1)
#define CLEAR           (DIGITAL9 + 1)
#define UP_OR_10        (CLEAR + 1) //���ϣ��Ǻ�
#define DIGITAL0        (UP_OR_10 + 1)
#define DOWN_OR_11      (DIGITAL0 + 1) //���£�����
#define ENTER           (DOWN_OR_11 + 1)


#define UP              UP_OR_10
#define DOWN            DOWN_OR_11
static const u8 gsdksyskeytab[36] = {POWER, F1, F2, F3, DIGITAL1, DIGITAL2, DIGITAL3, ALPHA,
                                     DIGITAL4,
                                     DIGITAL5, DIGITAL6, ESC, DIGITAL7, DIGITAL8, DIGITAL9, CLEAR,
                                     UP_OR_10, DIGITAL0, DOWN_OR_11,
                                     ENTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const SDK_KEY_TAB gsdkuserkeytab[] =
        {
                {POWER,    SDK_KEY_POWER_OFF},
                {F1,       SDK_KEY_F1},
                {F2,       SDK_KEY_F2},
                {F3,       SDK_KEY_FUNCTION},
                {DIGITAL1, SDK_KEY_1},
                {DIGITAL2, SDK_KEY_2},
                {DIGITAL3, SDK_KEY_3},
                {ALPHA,    SDK_KEY_ALPHA},
                {DIGITAL4, SDK_KEY_4},
                {DIGITAL5, SDK_KEY_5},
                {DIGITAL6, SDK_KEY_6},
                {ESC,      SDK_KEY_ESC},
                {DIGITAL7, SDK_KEY_7},
                {DIGITAL8, SDK_KEY_8},
                {DIGITAL9, SDK_KEY_9},
                {CLEAR,    SDK_KEY_BACKSPACE},
                {UP,       SDK_KEY_UP},
                {DIGITAL0, SDK_KEY_0},
                {DOWN,     SDK_KEY_DOWN},
                {ENTER,    SDK_KEY_ENTER},
        };
/****************************G2��ϵͳ����ֵ����ת����*****************************/
#elif MACHINEMODEL == SDK_SYS_MACHINE_G2
#define POWER   (0x01) //ϵͳ�ػ�
#define F1              (POWER + 1)
#define F2              (F1 + 1)
#define FUNCTION        (F2 + 1)
#define DIGITAL1        (FUNCTION + 1)
#define DIGITAL2        (DIGITAL1 + 1)
#define DIGITAL3        (DIGITAL2 + 1)
#define DIGITAL4        (DIGITAL3 + 1)
#define DIGITAL5        (DIGITAL4 + 1)
#define DIGITAL6        (DIGITAL5 + 1)
#define DIGITAL7        (DIGITAL6 + 1)
#define DIGITAL8        (DIGITAL7 + 1)
#define DIGITAL9        (DIGITAL8 + 1)
#define UP_OR_10        (DIGITAL9 + 1) //���ϣ��Ǻ�
#define DIGITAL0        (UP_OR_10 + 1)
#define DOWN_OR_11      (DIGITAL0 + 1) //���£�����
#define ESC             (DOWN_OR_11 + 1)
#define CLEAR           (ESC + 1)
#define ENTER           (CLEAR + 1)

#define UP              UP_OR_10
#define DOWN            DOWN_OR_11

static const u8 gsdksyskeytab[36] = { POWER, F1, F2, FUNCTION, DIGITAL1, DIGITAL2, DIGITAL3, DIGITAL4,
                                      DIGITAL5, DIGITAL6, DIGITAL7, DIGITAL8, DIGITAL9, UP_OR_10, DIGITAL0, DOWN_OR_11,
                                      ESC, CLEAR, ENTER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

static const SDK_KEY_TAB gsdkuserkeytab[] =
{
    {POWER, SDK_KEY_POWER_OFF},
    {F1, SDK_KEY_F1},
    {F2, SDK_KEY_F2},
    {FUNCTION, SDK_KEY_FUNCTION},
    {DIGITAL1, SDK_KEY_1},
    {DIGITAL2, SDK_KEY_2},
    {DIGITAL3, SDK_KEY_3},
    {DIGITAL4, SDK_KEY_4},
    {DIGITAL5, SDK_KEY_5},
    {DIGITAL6, SDK_KEY_6},
    {DIGITAL7, SDK_KEY_7},
    {DIGITAL8, SDK_KEY_8},
    {DIGITAL9, SDK_KEY_9},
    {UP, SDK_KEY_UP},
    {DIGITAL0, SDK_KEY_0},
    {DOWN, SDK_KEY_DOWN},

    {ESC, SDK_KEY_ESC},
    {CLEAR, SDK_KEY_BACKSPACE},
    {ENTER, SDK_KEY_ENTER},
};

#endif


static const u8 gsdkuserkeynum = ENTER;           //�û���������
static bool gPressKeyOpenLcd = false;           //�û���������



s32 sdkKeyOpen(void) {
    s32 rtn = ddi_key_open();

    if (DDI_OK == rtn) {
        sdkKbSetBgLightEnableIdle(false);
        sdkKbSetBgLightOnOrOff(false);
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

s32 sdkKeyClose(void) {
    s32 rtn = ddi_key_close();

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ��ϼ�ֵ
** Parameters:          void
** Returned value:  ������ϰ���(SDK_KEY_FUNCTION+other)ֵ0x0F0000xx
   0x0F�����ܼ�ֵ��xx����ڶ�����ϼ�ֵ
** Created By:		shijianglong  2013.05.10
** Remarks:
*****************************************************************************/
s32 sdkKbGetCombinedKey(void) //shijianglong 2013.05.09 14:17
{
    u32 key = 0;
    u32 combKey = 0;
    u16 numb = 0;
    u8 tempKey = 0;

    if (DDI_OK != ddi_key_open()) {
        return SDK_ERR;
    }
    numb = ddi_key_read(&key);

    //Trace( "zjp", "key = 0x%08x\r\n", key );
    if (numb) {
        if (gPressKeyOpenLcd == true) {
            gPressKeyOpenLcd = false;
            sdk_dev_misc_ex_control_lcdlamp(true);
        }

        if (numb == 2) //G2 200P ֻ֧��ͬʱ����2����ֵ
        {
            tempKey = key & 0x000000FF;

            if (tempKey == FUNCTION) //���ܼ���0~7λ��
            {
                combKey += gsdkuserkeytab[tempKey - POWER].UpKey << 24;
                combKey += gsdkuserkeytab[((key >> 8) & 0x000000FF) - POWER].UpKey;
            }
            else if (((key >> 8) & 0x000000FF) == FUNCTION) //���ܼ���8~15λ��
            {
                combKey += gsdkuserkeytab[tempKey - POWER].UpKey;
                combKey += gsdkuserkeytab[((key >> 8) & 0x000000FF) - POWER].UpKey << 24;
            }
            else //ԭ���ݷ���
            {
                combKey += gsdkuserkeytab[tempKey - POWER].UpKey;
                combKey += gsdkuserkeytab[((key >> 8) & 0x000000FF) - POWER].UpKey << 8;
            }
            //Trace( "zjp", "combKey = 0x%08x\r\n", combKey );
            //Trace( "zjp", "combKey[0] = 0x%02x\r\n", (combKey & 0x000000FF) );
            //Trace( "zjp", "combKey[1] = 0x%02x\r\n", ((combKey & 0x0000FF00) >> 8) );
            //Trace( "zjp", "combKey[2] = 0x%02x\r\n", ((combKey & 0x00FF0000) >> 16) );
            //Trace( "zjp", "combKey[3] = 0x%02x\r\n", ((combKey & 0xFF000000) >> 24) );
        }
        else if (key == SYS_POWER_OFF) {
            sdkDealPowerOff();
        }
    }
    return (s32) combKey;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ����ֵ
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkKbGetKey(void) {
    s32 tmpkey = 0;
    u32 key;
    u32 num = 0;

#if 0 /*Modify by ���ƽ at 2014.07.02  14:38 */

    if (0 != ddi_key_open())
    {
        return -1;
    }
#endif /* if 0 */

    num = ddi_key_read(&key);

    if (num != 0) {
        Trace("lgp", "�ײ㷵��%d\r\n", key);
    }

    if (key) {
        if (gPressKeyOpenLcd == true) {
            gPressKeyOpenLcd = false;
            sdk_dev_misc_ex_control_lcdlamp(true);
        }

        if (key <= gsdkuserkeynum) {
            tmpkey = gsdkuserkeytab[key - POWER].UpKey;
        }
        else if (key == SYS_POWER_OFF) {
            sdkDealPowerOff();
        }
        //else//�õ�������ϼ����û�ͬʱ���¶����
        //{
        //	  ;
        //}
    }
    return tmpkey;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �����ǰ���̻������е�����δ��ȡ�İ���
   ��ڲ���:��
   �� �� ֵ:��
   ��	  ע:
 ********************************************************************/
void sdkKbKeyFlush(void) {
    ddi_key_clear();
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:
   ��������:
   �������:
   �������:
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��:2012.06.28 16:43:25
*******************************************************************/
s32 sdkKbWaitKey(u32 uiMask, s32 siMs) {
    u32 timerID;
    bool flag = false;
    s32 Key;

    timerID = sdkTimerGetId();

    //���ʱ�� Ϊ0�Ǿ;�˵��ʱ�����޵ȴ�

    sdkKbKeyFlush();

// duxd2015
#ifdef ANDROID
    return sdkKbGetKey();
#endif
    while (1) {
        if ((siMs != 0) && sdkTimerIsEnd(timerID, (u32) siMs) == 1) {
            return SDK_TIME_OUT;
        }
        Key = sdkKbGetKey();


        if (SDK_KEY_ERR != Key) {
            if (0x7FFFFFFF == uiMask) {
                flag = true;
            }
            else if (
                    (Key == SDK_KEY_0 && (uiMask & SDK_KEY_MASK_0))
                    || (Key == SDK_KEY_1 && (uiMask & SDK_KEY_MASK_1))
                    || (Key == SDK_KEY_2 && (uiMask & SDK_KEY_MASK_2))
                    || (Key == SDK_KEY_3 && (uiMask & SDK_KEY_MASK_3))
                    || (Key == SDK_KEY_4 && (uiMask & SDK_KEY_MASK_4))
                    || (Key == SDK_KEY_5 && (uiMask & SDK_KEY_MASK_5))
                    || (Key == SDK_KEY_6 && (uiMask & SDK_KEY_MASK_6))
                    || (Key == SDK_KEY_7 && (uiMask & SDK_KEY_MASK_7))
                    || (Key == SDK_KEY_8 && (uiMask & SDK_KEY_MASK_8))
                    || (Key == SDK_KEY_9 && (uiMask & SDK_KEY_MASK_9))
                    || (Key == SDK_KEY_ENTER && (uiMask & SDK_KEY_MASK_ENTER))
                    || (Key == SDK_KEY_ESC && (uiMask & SDK_KEY_MASK_ESC))
                    || (Key == SDK_KEY_UP && (uiMask & SDK_KEY_MASK_UP))
                    || (Key == SDK_KEY_DOWN && (uiMask & SDK_KEY_MASK_DOWN))
                    || (Key == SDK_KEY_F1 && (uiMask & SDK_KEY_MASK_F1))
                    || (Key == SDK_KEY_F2 && (uiMask & SDK_KEY_MASK_F2))
                    || (Key == SDK_KEY_F3 && (uiMask & SDK_KEY_MASK_F3))
                    || (Key == SDK_KEY_F4 && (uiMask & SDK_KEY_MASK_F4))
                    || (Key == SDK_KEY_PRINT && (uiMask & SDK_KEY_MASK_PRINT))
                    || (Key == SDK_KEY_FUNCTION && (uiMask & SDK_KEY_MASK_FUNCTION))
                    || (Key == SDK_KEY_MULTITASK && (uiMask & SDK_KEY_MASK_MULTTASK))
                    || (Key == SDK_KEY_ALPHA && (uiMask & SDK_KEY_MASK_ALPHA))
                    || (Key == SDK_KEY_LEFT && (uiMask & SDK_KEY_MASK_LEFT))
                    || (Key == SDK_KEY_DOUBLEZERO && (uiMask & SDK_KEY_MASK_00))
                    || (Key == SDK_KEY_RIGHT && (uiMask & SDK_KEY_MASK_RIGHT))
                    || (Key == SDK_KEY_CLEAR && (uiMask & SDK_KEY_MASK_CLEAR))
                    || (Key == SDK_KEY_BACKSPACE && (uiMask & SDK_KEY_MASK_BACKSPACE))
                    || (Key == SDK_KEY_SIGN && (uiMask & SDK_KEY_MASK_LOGIN))
                    || (Key == SDK_KEY_10 && (uiMask & SDK_KEY_MASK_10))
                    || (Key == SDK_KEY_11 && (uiMask & SDK_KEY_MASK_11))
                    || (Key == SDK_KEY_F5 && (uiMask & SDK_KEY_MASK_F5))
                    || (Key == SDK_KEY_SET && (uiMask & SDK_KEY_MASK_SET)) //ֻ�������ü�
                    || (Key == SDK_KEY_BILL && (uiMask & SDK_KEY_MASK_BILL)) //ֻ�����˵���
                    || (Key == SDK_KEY_MENU && (uiMask & SDK_KEY_MASK_MENU)) //ֻ����˵���
                    || (Key == SDK_KEY_PAY && (uiMask & SDK_KEY_MASK_PAY)) //ֻ����֧����
                    || (Key == SDK_KEY_FLASH &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����
                    || (Key == SDK_KEY_SMS &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����
                    || (Key == SDK_KEY_NOHOOK &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����
                    || (Key == SDK_KEY_REDIAL &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����
                    || (Key == SDK_KEY_DIALIN &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����
                    || (Key == SDK_KEY_DIALOUT &&
                        (uiMask & SDK_KEY_MASK_PHONE)) //�������ϡ����š����ᡢ�ز������硢ȥ��6����

                    ) {
                flag = true;
            }

            if (flag) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                return Key;
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *poutkeytab
                               s32 siTabSize
** Returned value:
** Created By:		fusuipu  2013.11.14
** Remarks:             ͬ��sdk1.04��ߵĽӿ�
*****************************************************************************/
s32 sdkSysGetSysKeytab(u8 *poutkeytab, s32 siTabSize) {
    //return sdk_dev_get_sys_key_tab(poutkeytab, siTabSize);
    if (NULL == poutkeytab || siTabSize <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(gsdksyskeytab) > (u32) siTabSize) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memcpy(poutkeytab, gsdksyskeytab, sizeof(gsdksyskeytab));

    return SDK_OK;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ø��������û���ֵ
   ��ڲ���:���ؼ�ֵӳ��tab��
   �� �� ֵ: ����
   ��      ע:
 ********************************************************************/
s32 sdkSysGetUserKeytab(SDK_KEY_TAB userkeytab[], u8 *userkeynum) {
    memcpy(userkeytab, gsdkuserkeytab, sizeof(gsdkuserkeytab));
    //*userkeynum = sizeof(gsdkuserkeytab) / sizeof(gsdkuserkeytab[0]);
    *userkeynum = gsdkuserkeynum;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���ó�������
** Parameters:          bool enableLongPress  1:�����й��� 0����
** Returned value:
** Created By:		���ƽ  2014.07.08
** Remarks:             Ĭ�ϳ����޹���
*****************************************************************************/
s32 sdkKbkeySetLongPress(bool enableLongPress) {
    s32 rtn = ddi_key_ioctl(DDI_KEY_CTL_LONGPRESS, enableLongPress, 0);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���ü��̱�����Ƿ��Զ�Ϩ��
** Parameters:          bool ctrl
** Returned value:
** Created By:		���ƽ  2014.07.31
** Remarks:             1.ctrl = 1��sdkKbSetLedDelayOffTimes�趨��ʱ��δ�����Զ��ر�;
                    2.ctrl = 0���趨ʱ��δ�������Զ��ر�;
*****************************************************************************/
s32 sdkKbSetBgLightEnableIdle(bool ctrl) {
    s32 rtn = ddi_key_ioctl(DDI_KEY_CTL_BKLIGHT, ctrl, 0);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���ü����޲�������ر�ʱ��
** Parameters:          u8 uiTime(ms)
** Returned value:
** Created By:		���ƽ  2014.07.31
** Remarks:
*****************************************************************************/

s32 sdkKbSetBgLightIdleTimes(u32 uiTime) {
    s32 rtn = ddi_key_ioctl(DDI_KEY_CTL_BKLIGHTTIME, uiTime, 0);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/****************************************************************************
  **Description:       ���Ƽ��̱��������
  **Input parameters:  1--the led on  0--the led off
  **Output parameters:
  ********************Returned value:
  ********************Created by:            ��ѧ��(2013-4-15)

****************************************************************************/
s32 sdkKbSetBgLightOnOrOff(bool ctrl) {
    s32 rtn = ddi_key_ioctl(DDI_KEY_CTL_BKLIGHT_CTRL, ctrl, 0);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.11.14
** Remarks:             sdk1.04���������ӿڣ����Ա���ͬ������
*****************************************************************************/
s32 Private_sdkGetKeyValue(void) {
    s32 key = 0;
    u32 num = 0;

    num = ddi_key_read(&key);

    if (num != 0) {
        Trace("lgp", "�ײ㷵��%d\r\n", key);
    }

    if (key > 0) {
        Trace("wlz", "===========dev_get_key %d \r\n", key);
        //dev_key_flush(); wlz modifi
    }
    return key;
}

