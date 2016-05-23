#include <android/log.h>

#define  LOG_TAG    "daemon-misc"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

//bool sBisTradeFlow = false;      //是否有交易
extern volatile u32 gScreenTimer;
//ljh2015-01-08
extern volatile bool gIsScreenOn;
extern bool gIsMachineK200P;

////////////////////////////////////

//测试==========================================================
const MenuItemStr MenuTab[] = {

        {0x1000, true,  "一级交易1",          "xxx"},
        {0x2000, true,  "一级交易2",          "xxx"},
        {0x3000, true,  "一级交易3",          "xxx"},
        {0x4000, false, "一级交易4",          "4000"},
        {0x5000, false, "一级交易5",          "5000"},
        {0x6000, false, "一级交易6",          "6000"},
        {0x7000, false, "一级交易7",          "7000"},
        {0x8000, false, "一级交易8",          "8000"},
        {0x9000, false, "一级交易9",          "9000"},

        {0x1100, false, " 1 二级交易1",       "1100"},
        {0x1200, true,  " 1 二级交易2",       "xxx"},

        {0x1210, false, " 1->2 三级交易1",    "1210"},
        {0x1220, false, " 1->2 三级交易2",    "1220"},
        {0x1230, true,  " 1->2 三级交易3",    "xxx"},

        {0x1231, false, " 1->2->3 四级交易1", "1231"},
        {0x1232, false, " 1->2->3 四级交易2", "1232"},
        {0x1233, false, " 1->2->3 四级交易2", "1233"},
        {0x1234, false, " 1->2->3 四级交易3", "1234"},

        {0x2100, true,  " 2 二级交易1",       "xxx"},
        {0x2200, false, " 2 二级交易2",       "2200"},

        {0x2110, false, " 2->1 三级交易1",    "2110"},

        {0x3100, false, " 3 二级交易1",       "3100"}

};

int getMenuItemCount() {
    return (sizeof(MenuTab) / sizeof(MenuTab[0]));
}


const unsigned char bmpBuf[1024] = {
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00,
        0X00,
        0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0X00, 0X00, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00,
        0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF, 0X00, 0XFF, 0XFF, 0XFF,
        0X00
};

//////////////////////////////////////////////////////////////////////////////


/*****************************************************************************
** Descriptions:	结束交易流程标志
** Parameters:    	void
** Returned value:
** Created By:		mai2014.08.07
** Remarks:
*****************************************************************************/
void FinishTradeFlow(void) {
    gScreenTimer = sdkTimerGetId();//ljh2015-01-08
    Trace("Lai", "FinishTradeFlow JudgeTradeFlow gScreenTimer = %d\n", gScreenTimer);
    gIsScreenOn = true;
    sBisTradeFlow = false;
}


/*****************************************************************************
** Descriptions:	进入交易流程标志
** Parameters:    	void
** Returned value:
** Created By:		lilin2014.06.25
** Remarks:
*****************************************************************************/
void GotoTradeFlow(void) {
    sBisTradeFlow = true;
}

/*****************************************************************************
** Descriptions:	判断是否交易流程
** Parameters:    	void
** Returned value:
** Created By:		lilin2014.06.25
** Remarks:
*****************************************************************************/
bool JudgeTradeFlow(void) {
    return sBisTradeFlow;
}

/*****************************************************************************
 函 数 名  : SetSystemTime
 功能描述  : 设置系统时钟
 输入参数  : VTable *pVTable
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月21日
    作    者   : zhangjun
    修改内容   : 新生成函数

*****************************************************************************/
static bool SetSystemTime(void) {
    u8 tmp[64] = {0}, lcdtemp[64] = {0};
    s32 key = 0;
    u8 keybuf[64] = {0}, dispbuf[64] = {0}, timeformat[64] = {0};
    u8 i, j;

    bool flag = true;
    strcpy(timeformat, "____-__-__ __:__:__");
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "设置时钟", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "当前系统时间", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "设置新的时间", SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, timeformat, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    while (1) {
        memset(tmp, 0, sizeof(tmp));
        sdkGetRtc(tmp);
        memset(lcdtemp, 0, sizeof(lcdtemp));
        if (tmp[0] < 0x50) //2000年之后
        {
            sprintf(lcdtemp, "20%02x-%02x-%02x %02x:%02x:%02x", tmp[0],    //显示的是系统时钟
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }
        else //2000年之前
        {
            sprintf(lcdtemp, "19%02x-%02x-%02x %02x:%02x:%02x", tmp[0],    //显示的是系统时钟
                    tmp[1], tmp[2], tmp[3], tmp[4], tmp[5]);
        }
        sdkDispRow(SDK_DISP_LINE3, 0, lcdtemp, SDK_DISP_DEFAULT);
        flag = false;
        key = sdkKbGetKey();
        if (key > 0) {
            if (key == SDK_KEY_ENTER) {
                memset(tmp, 0, sizeof(tmp));
                if (strlen(keybuf)) {
                    if (strlen(keybuf) == 14) {
                        memcpy(tmp, keybuf, 4);
                        if (atoi(tmp) > 1950 && atoi(tmp) < 2049) {
                            memset(tmp, 0, sizeof(tmp));
                            sdkAscToBcd(tmp, &keybuf[2], 12);
                            if (sdkJudgeTime(tmp, 6)) {
                                sdkSetRtc(tmp);
                                sdkSysBeep(SDK_SYS_BEEP_OK);
                                return true;
                            }
                        }
                    }
                    if (flag) {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                        sdkDispLabel(SDK_DISP_LINE5, "格式错误", SDK_DISP_RIGHT_DEFAULT);
                        sdkKbWaitKey(SDK_KEY_MASK_ALL, 2000);
                    }
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    return true;
                }

            }
            else if (key == SDK_KEY_ESC) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                return false;
            }
            else if ((key >= SDK_KEY_0 && key <= SDK_KEY_9) || key == SDK_KEY_BACKSPACE) {
                flag = true;
                if (key == SDK_KEY_BACKSPACE) {
                    if (strlen(keybuf) > 0) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        keybuf[strlen(keybuf) - 1] = '\0';
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                }
                else {
                    if (strlen(keybuf) < 14) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        keybuf[strlen(keybuf)] = key;
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                }

            }
        }
        if (flag) {
            strcpy(dispbuf, timeformat);
            for (i = 0, j = 0; i < strlen(keybuf); i++, j++) {
                if (i == 4 || i == 6 || i == 8 || i == 10 || i == 12) {
                    j++;
                }
                dispbuf[j] = keybuf[i];
            }
            sdkDispRow(SDK_DISP_LINE5, 0, dispbuf, SDK_DISP_DEFAULT);
        }
    }
}

/*****************************************************************************
 函 数 名  : CheckTime
 功能描述  : 检查系统时间
 输入参数  : void
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月21日
    作    者   : zhangjun
    修改内容   : 新生成函数

*****************************************************************************/
bool CheckTime(void) {
    u8 temp[64];
    bool flag = true;

    memset(temp, 0, sizeof(temp));
    sdkGetRtc(temp);
    flag = sdkJudgeTime(temp, 6);
    if (flag == false) {
        SetSystemTime();
    }

    return flag;
}

/*****************************************************************************
   函 数 名  : GetRtcSys
   功能描述  : 获取当前系统时间
   输入参数  : void
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月28日
    作    者   : zj
    修改内容   : 新生成函数

*****************************************************************************/
void GetRtcSys(void) {
    u8 temp[64];

    memset(temp, 0, sizeof(temp));

    if (sdkGetRtc(temp) == SDK_OK) {
        pgstSys->ucYear = temp[0];
        pgstSys->ucMonth = temp[1];
        pgstSys->ucDay = temp[2];
        pgstSys->ucHour = temp[3];
        pgstSys->ucMinute = temp[4];
        pgstSys->ucSecond = temp[5];
    }
}

/*****************************************************************************
   函 数 名  : PrintCurTime
   功能描述  : 打印当前系统时间
   输入参数  : void
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
*****************************************************************************/
void PrintCurTime(void) {
    u8 temp[6];

    memset(temp, 0, sizeof(temp));
    if (sdkGetRtc(temp) == SDK_OK) {
        Trace("Mpos", "%02x%02x%02x%02x%02x%02x\n", temp[0], temp[1], temp[2], temp[3], temp[4],
              temp[5]);
    }
}


/*****************************************************************************
   函 数 名  : GetRtcText
   功能描述  : 获得当前时间，格式YYYY-MM-DD HH:MM:SS
   输入参数  : u8 *lpOut
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月28日
    作    者   : zj
    修改内容   : 新生成函数

*****************************************************************************/
void GetRtcText(u8 *lpOut) {
    s32 i = 0;

    GetRtcSys();
    if (pgstSys->ucYear >= 0x50) {
        strcpy(lpOut, "19");
    }
    else {
        strcpy(lpOut, "20");
    }
    i = 2;
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucYear, 1);      //年
    i += 2;

    lpOut[i++] = '-';
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucMonth, 1);     //月
    i += 2;

    lpOut[i++] = '-';
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucDay, 1);       //日
    i += 2;

    lpOut[i++] = ' ';
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucHour, 1);          //时
    i += 2;

    lpOut[i++] = ':';
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucMinute, 1);        //分
    i += 2;

    lpOut[i++] = ':';
    sdkBcdToAsc(&lpOut[i], &pgstSys->ucSecond, 1);        //秒
    i += 2;
    lpOut[i++] = 0x00;
}

/*****************************************************************************
** Descriptions:	在制定行显示系统时间
** Parameters:          u8 RowNo显示行号
                               bool bDispFALSE,内存操作,TRUE:显示屏操作
** Returned value:
** Created By:
** Remarks:
*****************************************************************************/
void DispSysRtc(u8 RowNo, bool bDisp) {
    u8 temp[64];
//    u8 dispbuf[512];


    memset(temp, 0, sizeof(temp));
//    memset(dispbuf, 0, sizeof(dispbuf));
    GetRtcText(temp);

    if (bDisp) {
        sdkDispRow(RowNo, 0, temp,
                   gDisplayMode == 0 ? SDK_DISP_DEFAULT : SDK_DISP_NOFDISP | SDK_DISP_CDISP |
                                                          SDK_DISP_INCOL);//Xiaox 15-1-14 for test
    }
    else {
        sdkDispFillRowRam(RowNo, 0, temp,
                          gDisplayMode == 0 ? SDK_DISP_DEFAULT : SDK_DISP_NOFDISP | SDK_DISP_CDISP |
                                                                 SDK_DISP_INCOL);//Xiaox 15-1-14 for test
    }
}


/*****************************************************************************
** Descriptions:	显示电池电量
** Parameters:    	void
** Returned value:
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
void DispIcon(void) {
    //enum BATTERYSTATE BatteryState = BATTERY_0;
    //static enum BATTERYSTATE NextBatteryState = BATTERY_0;
    //bool bisExpower = false;
    u8 offsetcoe = 1;
    //static bool haswarnflag = false;
    //static bool biscontrol = false;
    SDK_DISP_PIXEL pixel;
    u8 batteryOffset;
    u8 audioBtOffset;
    s32 ret;
    //bisExpower = sdkSysIsExPower();
    //BatteryState = (enum BATTERYSTATE)sdkSysGetBatteryStateAndControl();
    offsetcoe = (sdkSysIsColorScreen() ? 2 : 1);
    pixel = sdkDispGetScreenPixel();

    batteryOffset = (30) * offsetcoe;            //20150312 修改蓝牙和电池图标
    audioBtOffset = (50) * offsetcoe;            //20150312 修改蓝牙和电池图标
#if 0                                            //20150312 修改蓝牙和电池图标
    if( gIsMachineK200P )
	{
		batteryOffset = 30+2+30; //K200P 图标大小 30X12
		audioBtOffset = 30;		
	}
	else
	{
		batteryOffset = (16 + 8 + 16)* offsetcoe; //K200P 图标大小 39X16
		audioBtOffset = 16 * offsetcoe;
	}
#endif
    //Trace("mpos","bisExpower=%d \r\n BatteryState = %d\r\nNextBatteryState=%d\r\nj",bisExpower,BatteryState,NextBatteryState);
    //Trace("mpos","biscontrol=%d\r\n",biscontrol);
    //Trace("mpos","###### x=%d y=%d\r\n",pixel.siX,pixel.siY);
#if 0
    if (bisExpower)
    {
        haswarnflag = false;
		//Verify(sdkDispIcon(SDK_DISP_BATTERY, sdkSysGetBatteryStateAndControl(),pixel.siX - batteryOffset, 0) == SDK_OK);
		//Trace("mpos","##### BatteryState=%d\r\n",BatteryState);

        switch (BatteryState)
        {
            case BATTERY_LOW:
            case BATTERY_0:
            case BATTERY_1:
            case BATTERY_2:
            case BATTERY_3:
                if(BatteryState == BATTERY_2)    //判断第二格电需要显示正在充电
                {
                    if(biscontrol == true)
                    {
                        biscontrol = false;
                    }
                }
                if(biscontrol == false)
                {
                    NextBatteryState ++;
                    if(NextBatteryState > BATTERY_3)
                    {
                        NextBatteryState= BATTERY_0;
                    }
					Trace("mpos","111111111 %d\r\n",pixel.siX - batteryOffset);
                    Verify(sdkDispIcon(SDK_DISP_BATTERY, NextBatteryState, pixel.siX - batteryOffset, 0) == SDK_OK);
                }
                else
                {
                	Trace("mpos","22222 %d\r\n",pixel.siX - batteryOffset);
                    Verify(sdkDispIcon(SDK_DISP_BATTERY, BATTERY_MAX, pixel.siX - batteryOffset, 0) == SDK_OK);
                }
                break;
            case BATTERY_MAX:
                biscontrol = true;      //充满了
                Trace("mpos","33333 %d\r\n",pixel.siX - batteryOffset);
               Verify(sdkDispIcon(SDK_DISP_BATTERY, BATTERY_MAX, pixel.siX - batteryOffset, 0) == SDK_OK);
                break;
             case BATTERY_ERR: //外电还返回-2，说明是没电池
             default:
               NextBatteryState = BATTERY_HIDE;
			   Trace("mpos","444444 %d\r\n",pixel.siX - batteryOffset);
                Verify(sdkDispIcon(SDK_DISP_BATTERY, NextBatteryState, pixel.siX - batteryOffset, 0) == SDK_OK);
               break;
        }
    }
    else
    {
        biscontrol = false;
        switch (BatteryState)
        {
             case BATTERY_ERR:
             case BATTERY_LOW:
			   sprintf(tmpBuf,"电池故障或电量已空\n正在关机%d",BatteryState);
               sdkDispPromptBox(true, TV_SHOW,tmpBuf);// "电池故障或电量已空\n正在关机");
			   Trace("mpos","#### BatteryState:%d\r\n",BatteryState);
               if (!sdkSysIsExPower())
               {
                   sdkSysCmd("reboot",NULL);//sdkdealPoweroff
               }
               break;

             case BATTERY_0:

               if (!haswarnflag)
               {
                   sdkDispPromptBox(true, TV_AUTORET, "电池电量低\n请尽快充电");
                   haswarnflag = true;
                   break;
               }
             case BATTERY_1:
             case BATTERY_2:
             case BATTERY_3:
             case BATTERY_MAX:
             default:

                if (BatteryState != BATTERY_0)
                {
                    haswarnflag = false;
                }
				Trace("mpos","55555555 %d\r\n",pixel.siX - batteryOffset);
                Verify(sdkDispIcon(SDK_DISP_BATTERY, BatteryState, pixel.siX - batteryOffset, 0) == SDK_OK);
                break;
        }
    }
#endif
    //sdkDispIcon(SDK_DISP_BATTERY, BATTERY_MAX, pixel.siX - batteryOffset, 0);		//20150312 修改蓝牙和电池图标
    //Verify(sdkDispIcon(SDK_DISP_BATTERY, BATTERY_MAX, pixel.siX - batteryOffset, 0) == SDK_OK);        
    if (AUDIO == gucCurCommMode) {
        //Verify(sdkDispIcon(SDK_DISP_AUDIO, 5, pixel.siX - audioBtOffset, 0) == SDK_OK);
        sdkDispIcon(SDK_DISP_AUDIO, 5, pixel.siX - audioBtOffset, 0);
    }
    else if (BLUETOOTH == gucCurCommMode) {
        //Verify(sdkDispIcon(SDK_DISP_BT, 5, pixel.siX - audioBtOffset, 0) == SDK_OK);
        ret = sdkDispIcon(SDK_DISP_BT, 5, pixel.siX - audioBtOffset, 0);
        //这边有bug,比如 目前显示音频的图标 通信方式改为蓝牙 但蓝牙未打开 那么图标还是显示音频
        //Trace("Lai","***** BLUETOOTH gucCurCommMode:%d,ret:%d\n",gucCurCommMode,ret);
    }
    sdkDispIcon(SDK_DISP_BATTERY, BATTERY_MAX, pixel.siX - batteryOffset,
                0);        //20150312 修改蓝牙和电池图标

#ifdef WIRELESS
    s32 SignalLv = 0;

    if (GPRS ==gucCurCommMode ||CDMA == gucCurCommMode)
    {
        SignalLv = sdkCommGetSignalLevel();

        if (SignalLv >= 0x25)
        {
            Verify(sdkDispIcon(SDK_DISP_SIGNAL, 5, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }
        else if (SignalLv >= 0x20)
        {
            Verify(sdkDispIcon(SDK_DISP_SIGNAL, 4, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }
        else if (SignalLv >= 0x15)
        {
            Verify(sdkDispIcon(SDK_DISP_SIGNAL, 3, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }
        else if (SignalLv >= 0x08)
        {
           Verify(sdkDispIcon(SDK_DISP_SIGNAL, 2, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }
        else if (SignalLv == 0)
        {
            Verify(sdkDispIcon(SDK_DISP_SIGNAL, 0, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }
        else
        {
            Verify(sdkDispIcon(SDK_DISP_SIGNAL, 1, pixel.siX - 16 * offsetcoe, 0) == SDK_OK);
        }

        if (gucCurCommMode == GPRS)
        {
            Verify(sdkDispIcon(SDK_DISP_GPRS, true, pixel.siX - (16 + 8) * offsetcoe, 0) == SDK_OK);
        }
        else
        {
            Verify(sdkDispIcon(SDK_DISP_CDMA, true, pixel.siX - (16 + 8) * offsetcoe, 0) == SDK_OK);
        }
    }

#endif
}


/*****************************************************************************
** Descriptions:	清除全局变量
** Parameters:    	void
** Returned value:
** Created By:		lilin2013.01.09
** Remarks:
*****************************************************************************/
void ClearOthers(void) {
//    gucCurCommMode = pgstAppSetting->stCommuParam.ucCommMode;
    //gucCurCommMode = BLUETOOTH;
    giIdleClockTimer = 0;
    giAutoRetTimer = sdkTimerGetId();//sujianzhong 2015.01.14 1:13
    bisNeedBrushScreenWhenBack = false;
    bisNeedKeepScreenWhenBack = false;
}

/*******************************************************************
函数名称: RegulateMoney
函数功能: 将金额数据转化为标准的金额格式的字符串
相关调用:
输   入：money   6个字节12位BCD码的数组
输   出：string加上小数点后的标准金额格式字符串
返 回 值:
备    注:
创建信息:	 张建忠
修改信息:
********************************************************************/
u8 RegulateMoney(u8 *string, u8 *money) {
    u8 i, j, k;
    u8 temp[23] = {0};

    memset(temp, 0, 13);
    sdkBcdToAsc(temp, &money[0], 6);

    i = strspn(temp, "0");  //第一个不为0的位置
    if (i > 9) //增加小数点
    {
        string[0] = '0';
        string[1] = '.';
        string[2] = temp[10];
        string[3] = temp[11];
        string[4] = 0;
        return 4;
    }
    else {
        for (j = i, k = 0; j <= 9; j++, k++) {
            string[k] = temp[j];
        }
        string[k] = '.';
        string[k + 1] = temp[10];
        string[k + 2] = temp[11];
        string[k + 3] = 0;
        return (k + 3);
    }
}


/********************************************************************
 作    者:
 版    权: 深圳市新国都技术股份有限公司
 函数功能:判断磁道信息格式
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
bool AdjustTrackData(SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData) {
    u16 i;
    bool flag = false;
    i = 0;
    Trace("wlz", "=pstCardData->ucTrack1Len %d=\r\n", pstCardData->ucTrack1Len);
    Trace("wlz", "=pstCardData->ucTrack2Len %d=\r\n", pstCardData->ucTrack2Len);
    Trace("wlz", "=pstCardData->ucTrack3Len %d=\r\n", pstCardData->ucTrack3Len);
    TraceHex("wlz", " ucTrack1 \r\n", pstCardData->asTrack1Data, pstCardData->ucTrack1Len);
    TraceHex("wlz", " ucTrack2 \r\n", pstCardData->asTrack2Data, pstCardData->ucTrack2Len);
    TraceHex("wlz", " ucTrack3 \r\n", pstCardData->asTrack3Data, pstCardData->ucTrack3Len);
    if ((pstCardData->ucTrack2Len > 0 && pstCardData->ucTrack2Len <= 12 * 2) ||
        (pstCardData->ucTrack2Len > 19 * 2) || (0 == pstCardData->ucTrack2Len)) {
        memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));
        pstIccParam->ucCardMode = 0;
        //sdkIccOpenMag();
        Trace("wlz", "=AdjustTrackData=1=\r\n");
        return false;
    }

    if (pstCardData->ucTrack2Len > 19 * 2) {
        pstCardData->ucTrack2Len = 19 * 2;
    }

    if (pstCardData->ucTrack3Len > 52 * 2) {
        pstCardData->ucTrack3Len = 52 * 2;
    }

    if (pstCardData->ucTrack1Len > 76 * 2) {
        pstCardData->ucTrack1Len = 76 * 2;
    }
    //zxx 20130821 9:46加入对二磁道数据合法性的判断
    //sdk层在二磁道无D又是奇数长度时，报上来的asCardNO取值 不正确
    flag = true;
    if (pstCardData->ucTrack2Len > 12) {
        flag = false;
        for (i = 0; i < pstCardData->ucTrack2Len; i++) {
            if (pstCardData->asTrack2Data[i] == 'D' || pstCardData->asTrack2Data[i] == '=') {
                flag = true;
                break;
            }
        }
    }
    if (!flag) {
        memset(pstCardData, 0, sizeof(SDK_ICC_CARDDATA));
        pstIccParam->ucCardMode = 0;
        //sdkIccOpenMag();
    }
    Trace("wlz", "=AdjustTrackData=flag %d=\r\n", flag);
    return flag;
}

/*****************************************************************************
** Descriptions:	 获得磁卡或手输卡号
** Parameters:          SDK_ICC_CARDDATA *pstCardData磁卡信息结构体
                               u8 *lpOut卡号
** Returned value:	SDK_EQU:'D'后有'2','6'数据,是ICC卡,
                                SDK_OK: 'D'后无'2','6'数据,非IC卡
                                SDK_ERR:手输卡号长度太短
** Created By:		lilin2012.08.18
** Remarks:
*****************************************************************************/
s32 GetMagCardNo(SDK_ICC_CARDDATA const *pstCardData, u8 *pasOut) {
    u8 temp[256] = {0};
    s32 rslt = SDK_OK;
    u16 i = 0;
    u8 len = 0;
    u8 track2len = 0;
    u8 track3len = 0;
    bool track2flag = false; //是否有二磁道数据或手输

    memset(temp, 0, sizeof(temp));
    track2len = pstCardData->ucTrack2Len;

    if (track2len)                                                                               //2磁道数据
    {
        if (track2len > 37) {
            track2len = 37;
        }
        memcpy(temp, pstCardData->asTrack2Data, track2len);
        track2flag = true; //有二磁道数据
    }
    else {
        track3len = pstCardData->ucTrack3Len;

        if (track3len)                                                                  //三磁道数据
        {
            if (track3len > 104) {
                track3len = 104;
            }
            memcpy(temp, pstCardData->asTrack3Data, track3len);
        }
    }

    if (track2len || track3len) {
        len = strlen(temp);

        for (i = 0; i < len; i++) {
            if (temp[i] == 'D') {
                //1，武汉银商确定刷卡时不判断磁卡是否为IC，由他们后台判断，在GetMagCardNo注释掉判断62
#if 0
                if (temp[i + 5] == '2' || temp[i + 5] == '6')                           //如果D后边的第5位是2或6表示该卡上有IC卡
                {
                    rslt = SDK_EQU;
                }
#endif
                break;
            }
        }

        len = i;

        if (len > 19) {
            len = 19;
        }
        temp[len++] = 0;
    }
    else {
        memset(temp, 0, sizeof(temp));
        len = pstCardData->ucCardLen;

        if (len > 19) {
            len = 19;
        }

        if (len < MANUALMINCARDNUM)         //手输卡号小于13
        {
            rslt = SDK_ERR;
        }
        memcpy(temp, pstCardData->asCardNO, len);
        temp[len++] = 0;
        track2flag = true;
    }

    if (strlen(temp) == 0) //未获取到卡号
    {
        return SDK_PARA_ERR;
    }
    memcpy(pasOut, temp, len);

    if (track2flag && strlen(temp) >= 13) //必须有二磁道数据或有手输数据
    {
        return rslt;
    }
    else {
        return SDK_PARA_ERR;
    }
}

/*********************************************************************************************************
** Descriotions:        |显示ICC卡
** parameters:          |none
** Returned value:    |SDK_ESC SDK_OK
** Created By:          |李琳
** Remarks:              |
*********************************************************************************************************/
static bool DispICCMag(u8 *pusCardType) {
    u32 displine = SDK_DISP_LINE2;  //固定显示第第二行
    u8 temp[64] = {0};
    //u8 bmpbuf[64] = {0};
    //bool bRfLogoFile = false;
    //u8 asTradeName[21];

    if ((*pusCardType) & SDK_ICC_ICC ||
        (*pusCardType) & SDK_ICC_RF) //当前支持IC卡//zxx 20130729 11:33添加rfmask判断
    {
        *pusCardType &= ~MAGONLY;//zxx 20130729 11:33 非掉磁条
        strcpy(temp, "本卡为IC卡\n请");
        if (*pusCardType & SDK_ICC_ICC && *pusCardType & SDK_ICC_RF) {
            strcat(temp, "插入IC卡或挥卡");
        }
        else if (*pusCardType & SDK_ICC_ICC) {
            strcat(temp, "插入IC卡");
        }
        else if (*pusCardType & SDK_ICC_RF) {
            strcat(temp, "挥卡");
        }
        sdkDispLabel(displine, temp, SDK_DISP_DEFAULT);
        return true;
    }
    return false;
}

/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:支持刷卡和按键
 输入参数:uiOverTime  	:超时退出时间

 			eDispRow		:显示行
 输出参数:
  			pstIccParam	:磁卡输出数据
 			pstCardData
 			mode		:Slotcard �m ManualInput

 返 回 值
 备    注:
 修改信息:
********************************************************************/
extern bool gIsAlreadyReadCardRFFlg;   //20150330 添加非接是否已读到标识
s32 SposIccReadCardEx(ST_TRANSFLOW *pstTransFlow, u8 CardMode,
                      SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam,
                      SDK_ICC_CARDDATA *pstCardData, const u32 eDispRow, u8 *pcardTypeMask) {
    u8 heUID[64] = {0}; //heUID[0]:UID的长度；heUID[1~x]:为UID的数据
    u8 temp[64] = {0};
    u8 handLen = 0, handData[128] = {0};
    s32 key;
    s32 iRslt;
    u8 mode = CardMode << 1;//MPOS文档跟SDK定义的有差别 这边转换下 ljh2015-01-08
    static s32 timer = 0;       //20150330 计时非接读卡超时时间
    static SDK_ICC_PARAM stTParam;
    static s32 trslt = 0;
    //Trace("wlz","==SposIccReadCardEx==\r\n");
    if (FALSE == pstTransFlow->flag1) {
        pstTransFlow->flag1 = TRUE;
        sdkKbKeyFlush();
        Trace("Lai", "*** *** SposIccReadCardEx mode %#x\r\n", mode);
        //emv参数初始化放在这里，是因为参数有可能被改变，导致与读卡器参数不一致，如果不一致，
        //在上电前重新对读卡器赋值
        if (mode & SDK_ICC_MANUAL) {
            sdkDispClearRowRam(eDispRow);
            sdkDispFillRowRam(eDispRow, 0, "_", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
        }
        if (mode & SDK_ICC_RF) {
            iRslt = sdkIccOpenRfDev();
            Trace("Lai", "sdkIccOpenRfDev iRslt %d\r\n", iRslt);
        }
        if (mode & SDK_ICC_MAG) {
            iRslt = sdkIccOpenMag();
            Trace("Lai", "sdkIccOpenMag iRslt %d\r\n", iRslt);
        }
        if (mode & SDK_ICC_ICC) {
            iRslt = sdkIccOpenIcDev();
            Trace("Lai", "sdkIccOpenIcDev iRslt %d\r\n", iRslt);
        }
    }

    if (mode & SDK_ICC_MAG) {
        iRslt = sdkIccGetMagData(pstIccParam, pstCardData);

        //TraceHex("","@@@ expiredate\r\n",pstCardData->asExpiredDate,4);
        //Trace("wlz","\r\n===sdkIccGetMagData==%d==\r\n",iRslt);
        if (iRslt == SDK_OK) {
            iRslt = (AdjustTrackData(pstIccParam, pstCardData)) ? SDK_OK : SDK_ICC_BRUSH_ERR;
        }
        //Trace("wlz","\r\n===AdjustTrackData==%d==\r\n",iRslt);

        if (iRslt == SDK_OK) //成功返回
        {
            sdkIccCloseIcDev();
            sdkIccCloseRfDev();
#if 1
            memset(temp, 0, sizeof(temp));
            iRslt = GetMagCardNo(pstCardData, temp);
            Trace("readcard", "是否62 %d\r\n", iRslt);
            if (iRslt == SDK_EQU) //判断6,2卡号
            {
                if (DispICCMag(&mode)) {
                    return SDK_EQU;//采用ic卡
                }
            }
            else if (iRslt == SDK_ERR)   //刷卡错误
            {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
                return SDK_ERR;
            }
            else if (iRslt == SDK_PARA_ERR)  //无二磁道，可继续刷卡
            {
                memset(pstCardData, 0, sizeof(STCARDINFO));
                sdkIccOpenMag();
                sdkDispLabel(SDK_DISP_LINE2, "刷卡失败,请重新刷卡!", SDK_DISP_LDISP | SDK_DISP_FDISP);
                sdkSysBeep(SDK_SYS_BEEP_ERR);
                return SDK_ICC_NOCARD;
            }
#endif
            Trace("Lai", "==SposIccReadCardEx=Ok=\r\n");
            sdkSysBeep(SDK_SYS_BEEP_OK);
            *pcardTypeMask |= MAG_CARD_MSK;
            return iRslt;
        }
        else if (iRslt == SDK_ICC_BRUSH_ERR) //可继续读卡
        {
            Trace("Lai", "==??????????==\r\n");
            //刷卡错
            sdkSysBeep(SDK_SYS_BEEP_ERR);
            sdkDispLabel(SDK_DISP_LINE2, "刷卡失败,请重新刷卡!", SDK_DISP_LDISP | SDK_DISP_FDISP);
        }
        else if (iRslt != SDK_ICC_NOCARD) //失败退出
        {
            Trace("Lai", "==SDK_ICC_NOCARD error=\r\n");
            return SDK_ERR;
        }
    }

    if (mode & SDK_ICC_ICC) {
        iRslt = sdkIccGetCardStatus(pstIccParam, 100);
        if (iRslt == SDK_OK) //成功返回
        {
            sdkIccCloseMag();
            sdkIccCloseRfDev();
            if (sdkIccTransInit(pstIccTradeParam) != SDK_OK)//IC卡参数初始化
            {
                Trace("Lai", "==sdkIccTransInit error=\r\n");
                return SDK_ERR;
            }
            Trace("Lai", "sdkIccGetCardStatus ok..........\r\n");
            *pcardTypeMask |= IC_CARD_MSK;
            return iRslt;
        }
        else if (iRslt != SDK_ICC_NOCARD) //失败退出
        {
            Trace("Lai", "sdkIccGetCardStatus fail..........\r\n");
            return SDK_ERR;
        }
        //Trace("","sdkIccGetCardStatus SDK_ICC_NOCARD..........\r\n");
    }

    if (mode & SDK_ICC_RF) //非接读卡
    {
#if 1           //20150403 使用非接延时判断磁条卡是否	
        Trace("Lai", "RF FLG:%d timer:%d\r\n", gIsAlreadyReadCardRFFlg, timer);
        //2015030 添加读到非接时，延时检测磁条卡，避免刷磁条卡时总是读到非接
        if (!gIsAlreadyReadCardRFFlg)   //没有读到过非接
        {
            timer = 0;

            iRslt = sdkIccPowerOnAndSeek(pstIccTradeParam, pstIccParam, heUID);
            Trace("Lai", "RF FLG:%d rslt:%d\r\n", gIsAlreadyReadCardRFFlg, iRslt);
            if (iRslt == SDK_OK || (iRslt != SDK_ICC_NOCARD && iRslt != SDK_ICC_MORECARD &&
                                    iRslt != SDK_ICC_BAT_VOLTAGE_LOW)) {
                sdkIccCloseIcDev();
                sdkIccCloseMag();
                if (sdkIccTransInit(pstIccTradeParam) != SDK_OK)//IC卡参数初始化
                {
                    Trace("Lai", "==sdkIccTransInit error=\r\n");
                    return SDK_ERR;
                }
                timer = sdkTimerGetId();
                gIsAlreadyReadCardRFFlg = true;
                trslt = iRslt;
                memcpy(&stTParam, pstIccParam, sizeof(stTParam));
            }
        }
        else if (sdkTimerIsEnd(timer, 500) && timer != 0)          //已经读到非接，且等待磁条卡数据超时，则继续读非接
        {
            timer = 0;
            gIsAlreadyReadCardRFFlg = false;
            memcpy(pstIccParam, &stTParam, sizeof(stTParam));
            //iRslt = sdkIccPowerOnAndSeek(pstIccTradeParam, pstIccParam, heUID);
            iRslt = trslt;
            Trace("Lai", "RF TimeOut FLG:%d rslt:%d\r\n", gIsAlreadyReadCardRFFlg, iRslt);
            if (iRslt == SDK_OK) {
                /*                if(bIsUseQreader) //qpboc在读卡器上面处理完成(外置G101或内置stm32)
    	            {
    	                return sdkIccDealQReaderTrade(pstIccTradeParam); //读卡器交易处理
    	            }
    	            else */
                if ((pstIccTradeParam->bIsSupportQPBOC) &&
                    (pstIccTradeParam->bIsSupportCLPBOC == FALSE) &&
                    (pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW)) {
                    Trace("Lai", "ret sdkIccTransFlow1..........\r\n");
                    return sdkIccTransFlow1(pstIccTradeParam, pstIccParam);
                }
                *pcardTypeMask |= RF_CARD_MSK;
                Trace("Lai", "ret RF_CARD_MSK ok..........\r\n");
                return iRslt;
            }
            else if (iRslt != SDK_ICC_NOCARD && iRslt != SDK_ICC_MORECARD &&
                     iRslt != SDK_ICC_BAT_VOLTAGE_LOW)    //无卡或多卡，继续寻卡
            {
                //ljh2015-01-08 添加SDK_ICC_BAT_VOLTAGE_LOW
                //电压低时候非接无法打开,这时候如果返回错误磁条卡和IC卡会用不了
                Trace("Lai", "ret RF_CARD_MSK   iRslt:%d.......\r\n", iRslt);
                return SDK_ERR;
            }
        }
#else
        iRslt = sdkIccPowerOnAndSeek(pstIccTradeParam, pstIccParam, heUID);
	        Trace("Lai","RF FLG:%d rslt:%d\r\n", gIsAlreadyReadCardRFFlg, iRslt);
	        if(iRslt == SDK_OK)
    	    {
		        sdkIccCloseIcDev();
		        sdkIccCloseMag();	    
			    if(sdkIccTransInit(pstIccTradeParam) != SDK_OK)//IC卡参数初始化
			    {
			    	Trace("Lai","==sdkIccTransInit error=\r\n");
			        return SDK_ERR;
			    }			
    	/*                if(bIsUseQreader) //qpboc在读卡器上面处理完成(外置G101或内置stm32)
    	            {
    	                return sdkIccDealQReaderTrade(pstIccTradeParam); //读卡器交易处理
    	            }
    	            else */
    			if((pstIccTradeParam->bIsSupportQPBOC) && (pstIccTradeParam->bIsSupportCLPBOC == FALSE)&& (pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW))
    			{
    				Trace("Lai","ret sdkIccTransFlow1..........\r\n");
    				return sdkIccTransFlow1(pstIccTradeParam, pstIccParam);
    			}
    			*pcardTypeMask |= RF_CARD_MSK;
    			Trace("Lai","ret RF_CARD_MSK ok..........\r\n");
    		 	return iRslt;
    	    }
    	    else if(iRslt != SDK_ICC_NOCARD && iRslt != SDK_ICC_MORECARD && iRslt != SDK_ICC_BAT_VOLTAGE_LOW)    //无卡或多卡，继续寻卡
    	    {
    	    	//ljh2015-01-08 添加SDK_ICC_BAT_VOLTAGE_LOW
    	    	//电压低时候非接无法打开,这时候如果返回错误磁条卡和IC卡会用不了
    	    	Trace("Lai","ret RF_CARD_MSK   iRslt:%d.......\r\n",iRslt);
    	        return SDK_ERR;
    	    }
#endif
    }

    key = sdkKbGetKey();

    if (mode & SDK_ICC_MANUAL) //手输卡号
    {
        if (key > 0) {
            if (TRUE == pstTransFlow->flag2) {
                if (key == SDK_KEY_ESC) {
                    Trace("Lai", "ret SDK_KEY_ESC..........\r\n");
                    return SDK_ESC;
                }
                handLen = 0;
                memset(handData, 0, sizeof(handData));
                memset(temp, 0, sizeof(temp));
                sdkDispRow(eDispRow, 0, temp, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
            }
            else if (key == SDK_KEY_ENTER) {
                if (handLen >= 13) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    pstIccParam->ucCardMode = SDK_ICC_MANUAL;
                    pstCardData->ucCardLen = handLen;
                    memcpy(pstCardData->asCardNO, handData, handLen);
                    return SDK_OK;
                }
                else if (handLen < 13) {
                    sdkDispRow(eDispRow, 0, "卡号不能小于13位",
                               SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
                    pstTransFlow->flag2 = TRUE;
                }
            }
            else if (key == SDK_KEY_CLEAR || key == SDK_KEY_BACKSPACE ||
                     (key >= SDK_KEY_0 && key <= SDK_KEY_9)) {
                if (key == SDK_KEY_CLEAR) {
                    handLen = 0;
                    memset(handData, 0, sizeof(handData));
                }
                else if (key == SDK_KEY_BACKSPACE) {
                    if (handLen > 0) {
                        handData[handLen] = 0;
                        handLen--;
                    }
                }
                else if (key >= SDK_KEY_0 && key <= SDK_KEY_9) {
                    if (handLen < 19) {
                        handData[handLen++] = (u8) key;
                    }
                }
                memset(temp, 0, sizeof(temp));

                if (handLen > 0) {
                    memcpy(temp, handData, handLen);
                }
                else {
                    temp[0] = '_';
                }
                sdkDispRow(eDispRow, 0, temp, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
            }
            sdkSysBeep(SDK_SYS_BEEP_OK);
        }
    }
    if (key == SDK_KEY_ESC) {
        bisNeedBrushScreenWhenBack = true;
        sdkSysBeep(SDK_SYS_BEEP_OK);
        return SDK_ESC;
    }
    return SDK_TIME_OUT;
}


/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:格式化磁道信息
 输入参数:
 输出参数:
 返 回 值
          		SDK_OK
 备    注:
 修改信息:
********************************************************************/
void MagDataFormat(SDK_ICC_CARDDATA pstCardData, u8 FormatMode, u8 *lpOut, u16 *nLen) {
    u8 tempmagdata[512];
    u8 tempmagdata1[512];
    u16 tempmagdatalen = 0;
    u16 tempmodlen;
    u16 Track2Len = 0, Track3Len = 0;

    memset(tempmagdata, 0, sizeof(tempmagdata));
    memset(tempmagdata, 'F', sizeof(tempmagdata) - 1);

    Track2Len = pstCardData.ucTrack2Len;
    Track3Len = pstCardData.ucTrack3Len;


    if (Track2Len > 0) {
        if (FormatMode == 0) {
            memset(tempmagdata1, 0, sizeof(tempmagdata1));
            memcpy((char *) tempmagdata1, (char *) pstCardData.asTrack2Data, Track2Len);
            memcpy(tempmagdata, tempmagdata1, Track2Len);
            tempmagdatalen = 48;
        }
        else {
            memcpy((char *) (tempmagdata), (char *) pstCardData.asTrack2Data, Track2Len);
            tempmagdatalen = Track2Len;
            tempmodlen = (tempmagdatalen % 16);              // 取余补F
            if (tempmodlen == 0) {
                memset(&tempmagdata[tempmagdatalen], 'F', 16);
                tempmagdatalen += 16;
            }
            else {
                memset(&tempmagdata[tempmagdatalen], 'F', 16 - tempmodlen);
                tempmagdatalen += 16 - tempmodlen;
            }
        }
    }
    if (Track3Len > 0) {
        memset(tempmagdata1, 0, sizeof(tempmagdata1));
        memcpy((char *) tempmagdata1, (char *) pstCardData.asTrack3Data, Track3Len);
        memcpy(&tempmagdata[tempmagdatalen], tempmagdata1, Track3Len);
        tempmagdatalen = tempmagdatalen + Track3Len;
        tempmodlen = (tempmagdatalen % 16);              // 取余补F
        if (tempmodlen == 0) {
            memset(&tempmagdata[tempmagdatalen], 'F', 16);
            tempmagdatalen += 16;
        }
        else {
            memset(&tempmagdata[tempmagdatalen], 'F', 16 - tempmodlen);
            tempmagdatalen += 16 - tempmodlen;
        }
    }
    sdkAscToBcd(lpOut, tempmagdata, tempmagdatalen);
    (*nLen) = (tempmagdatalen) >> 1;
}


/********************************************************************
 作    者: 张文晖
 版    权: 深圳市新国都技术股份有限公司
 函数功能: 获得磁卡或手输卡号
 输入参数:SDK_ICC_CARDDATA *pstCardData磁卡信息结构体
 输出参数:u8 *lpOut卡号
 返 回 值:SDK_EQU:'D'后有'2','6'数据,是ICC卡,
          SDK_OK: 'D'后无'2','6'数据,非IC卡
          SDK_ERR:手输卡号长度太短
 备    注:
 修改信息:
********************************************************************/
s32 AppGetMagCardNo(SDK_ICC_CARDDATA const *pstCardData, u8 *pasOut) {
    u8 temp[256] = {0};
    s32 flag = SDK_OK;
    u16 i = 0;
    u8 len = 0;
    u8 track2len = 0;
    u8 track3len = 0;
    TraceHex("wlz", "磁道1 \r\n", pstCardData->asTrack1Data, pstCardData->ucTrack1Len);
    TraceHex("wlz", "磁道2 \r\n", pstCardData->asTrack2Data, pstCardData->ucTrack2Len);
    TraceHex("wlz", "磁道3 \r\n", pstCardData->asTrack3Data, pstCardData->ucTrack3Len);
    memset(temp, 0, sizeof(temp));
    track2len = pstCardData->ucTrack2Len;

    if (track2len)                                                                                //2磁道数据
    {
        if (track2len > 37) {
            track2len = 37;
        }
        memcpy(temp, pstCardData->asTrack2Data, track2len);
    }
    else {
        track3len = pstCardData->ucTrack3Len;

        if (track3len)                                                                   //三磁道数据
        {
            if (track3len > 104) {
                track3len = 104;
            }
            memcpy(temp, &pstCardData->asTrack3Data[2], track3len - 2);
        }
    }

    if (track2len || track3len) {
        len = strlen(temp);

        for (i = 0; i < len; i++) {
            if (temp[i] == 'D' || temp[i] == 'E' || temp[i] == '=') {
                if (temp[i + 5] == '2' ||
                    temp[i + 5] == '6')                            //如果D后边的第5位是2或6表示该卡上有IC卡
                {
                    flag = SDK_EQU;
                }
                break;
            }
        }

        len = i;

        if (len > 19) {
            len = 19;
        }
        temp[len++] = 0;
    }
    else {
        memset(temp, 0, sizeof(temp));
        len = pstCardData->ucCardLen;

        if (len > 19) {
            len = 19;
        }

        if (len < MANUALMINCARDNUM)          //手输卡号小于13
        {
            flag = SDK_ERR;
        }
        memcpy(temp, pstCardData->asCardNO, len);
        temp[len++] = 0;
    }
    memcpy(pasOut, temp, len);
    return flag;
}


/********************************************************************
 作    者: 张文晖
 版    权: 深圳市新国都技术股份有限公司
 函数功能:
 输入参数:
 输出参数:
 返 回 值:
 备    注:
 修改信息:
********************************************************************/
u8 CalcBcc(u8 const *pucsrc, u32 uinum) {
    u32 i;                                            //临时个数
    u8 bcc = 0;                                        //累加和

    for (i = 0; i < uinum; i++) {
        bcc += pucsrc[i];                                //计算校验和
    }
    return bcc;                                        //返回校验和
}

/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:
 输入参数:
 输出参数:
 返 回 值:
 备    注:
 修改信息:
********************************************************************/
u8 CalcLrc(u8 const *pucsrc, u32 uinum) {
    u8 ucLrc = 0x00;

    while (uinum--)
        ucLrc += pucsrc[uinum];

    return ~(ucLrc);            //返回LRC
}


/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:保存PAN
 输入参数:
 输出参数:PAN
 返 回 值
 备    注:
 修改信息:
********************************************************************/
void SavePan(u8 *lpCardNo, u8 CardNoLen) {
    u8 PAN[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
    memset(gPan, 0x00, 8);
    if (CardNoLen >= 13) {
        sdkAscToBcd(PAN + 2, lpCardNo + CardNoLen - 13, 12);
    }
    memcpy(gPan, PAN, 8);
}

/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:取PAN
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
void GetPan(u8 *lpPan) {
    memset(lpPan, 0x00, 8);
    memcpy(lpPan, gPan, 8);
}

/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:设置指令取消复位
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
void SetCmdResetExe(bool bCmdReset) {
//    if(gbSetCmdResetExe){//Xiaox 15-1-12
//        bisNeedBrushScreenWhenBack = true;
//    }
    gbSetCmdResetExe = bCmdReset;
}

/********************************************************************
 作    者: 王录庄
 版    权: 深圳市新国都技术股份有限公司
 函数功能:获取指令取消复位
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
bool GetCmdResetExe(void) {
    return gbSetCmdResetExe;
}


/******************************************************
*功能:计算CRC的值
*
*
*******************************************************/
u8 GetCRC(u8 *pBuf, u16 iLen) {
    u16 i = 0;
    u8 crc = 0;

    for (i = 0; i < iLen; ++i)
        crc ^= pBuf[i];

    return crc;
}

/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:PKCS #5 填充
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
s32 pkcs5Padding(u8 *ucInData, u32 uiInDataLen, u8 *ucOutData) {
    u8 ucPadByte = 0x00;
    u32 uiTmpLen = 0;

    if (ucInData == NULL || ucOutData == NULL)
        return SDK_PARA_ERR;

    uiTmpLen = (uiInDataLen / 8) * 8 + 8;
    ucPadByte = uiTmpLen - uiInDataLen;
#if 0
    ucPadByte = 0;
#endif
    memcpy(ucOutData, ucInData, uiInDataLen);
    memset(ucOutData + uiInDataLen, ucPadByte, ucPadByte);

    return uiTmpLen;

}


s32 pkcs5UnPadding(u8 *ucInData, u32 uiInDataLen) {
    //u8 ucPadByte = 0x00;
    u8 temp[8];
    u8 lastdata;

    if (ucInData == NULL)
        return uiInDataLen;
    if (uiInDataLen % 8 || uiInDataLen < 8)
        return uiInDataLen;

    lastdata = ucInData[uiInDataLen - 1];
    memset(temp, lastdata, sizeof(temp));
    Trace("zcl", "lastdata = %d\r\n", lastdata);
    if (lastdata > 0x08) {
        return uiInDataLen;
    }
    TraceHex("zcl", "temp\r\n", temp, lastdata);
    TraceHex("zcl", "ucInData + uiInDataLen - lastdata\r\n", ucInData + (uiInDataLen - lastdata),
             lastdata);
    if (!memcmp(temp, ucInData + (uiInDataLen - lastdata), lastdata)) {
        return uiInDataLen - lastdata;
    }
    else {
        return uiInDataLen;
    }
}

#ifndef DEL_CODE_EXCESS                //删除多余代码: 20150304


// 测试命令 -------------------------------------------------
//==========存储类===========================
#define CONTEN_LEN    100
#define OFFSET1    2
#define OFFSETLEN1    8
#define OFFSET2    20
#define OFFSETLEN2    10


SDK_UI_EXIT_VALUE InitStoreRec(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;
    u8 *pRecName = "LIST";
    u8 len = strlen(pRecName);

    u16 val[5] = {CONTEN_LEN, OFFSET1, OFFSETLEN1, OFFSET2, OFFSETLEN2};

    u8 i = 0;
    for (i = 0; i < 5; i++)
        val[i] = U16_ENDIAN(val[i]);

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], len + 12 + 4, 2);
    index += 2;
    pOutBuf[index++] = STORE_REC_CLASS;
    pOutBuf[index++] = STORE_INIT_REC;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], len, 2);
    index += 2;
    memcpy(&pOutBuf[index], pRecName, len);
    index += len;
    memcpy(&pOutBuf[index], val, sizeof(val));
    index += sizeof(val);
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE GetRecCount(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;
    u8 *pRecName = "WATER";
    u8 len = strlen(pRecName);

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], len + 2 + 4, 2);
    index += 2;
    pOutBuf[index++] = STORE_REC_CLASS;
    pOutBuf[index++] = STORE_GET_RECCOUNT;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], len, 2);
    index += 2;
    memcpy(&pOutBuf[index], pRecName, len);
    index += len;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE AddRec(s32 ix) {
    u8 pOutBuf[200] = {0};
    u8 index = 0;
    u8 *pRecName = "WATER";
    u8 len = strlen(pRecName);

//============LL..1024======================
    u8 idxdata = 0;
    u8 data[102] = {0};
    u8 index1[] = "1";
    u8 index2[] = "w";
    u8 conten[] = "water record1";
    sdkU16ToBcd(&data[idxdata], CONTEN_LEN, 2);
    idxdata += 2;
    memcpy(&data[idxdata + OFFSET1], index1, strlen(index1));
    memcpy(&data[idxdata + OFFSET2], index2, strlen(index2));
    memcpy(&data[idxdata + OFFSET2 + OFFSETLEN2], conten, strlen(conten));
//==============================================

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 2 + len + sizeof(data) + 4, 2);
    index += 2;
    pOutBuf[index++] = STORE_REC_CLASS;
    pOutBuf[index++] = STORE_ADD_REC;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], len, 2);
    index += 2;
    memcpy(&pOutBuf[index], pRecName, len);
    index += len;
    memcpy(&pOutBuf[index], data, sizeof(data));
    index += sizeof(data);
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE UpdateRec(s32 ix) {
    u8 pOutBuf[200] = {0};
    u8 index = 0;
    u8 *pRecName = "WATER";
    u32 recNo = 1;
    u8 index1[] = "1";
    u8 index2[] = "w";
    u8 len = strlen(pRecName);
//============LL..1024======================
    u8 idxdata = 0;
    u8 data[102] = {0};
    u8 newindex1[] = "new idx1";
    u8 newindex2[] = "new index2";
    u8 conten[] = "new water record1";
    //u16 val[5]={CONTEN_LEN,OFFSET1,OFFSETLEN1,OFFSET2,OFFSETLEN2};
    sdkU16ToBcd(&data[idxdata], CONTEN_LEN, 2);
    idxdata += 2;
    memcpy(&data[idxdata + OFFSET1], newindex1, strlen(newindex1));
    memcpy(&data[idxdata + OFFSET2], newindex2, strlen(newindex2));
    memcpy(&data[idxdata + OFFSET2 + OFFSETLEN2], conten, strlen(conten));
//==============================================
    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], len + 10 + strlen(index1) + strlen(index2) + sizeof(data) + 4, 2);
    index += 2;
    pOutBuf[index++] = STORE_REC_CLASS;
    pOutBuf[index++] = STORE_UPDATE_REC;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], len, 2);
    index += 2;
    memcpy(&pOutBuf[index], pRecName, len);
    index += len;

    recNo = U32_ENDIAN(recNo);
    memcpy(&pOutBuf[index], &recNo, 4);
    index += 4;

    sdkU16ToBcd(&pOutBuf[index], strlen(index1), 2);
    index += 2;
    memcpy(&pOutBuf[index], index1, strlen(index1));
    index += strlen(index1);

    sdkU16ToBcd(&pOutBuf[index], strlen(index2), 2);
    index += 2;
    memcpy(&pOutBuf[index], index2, strlen(index2));
    index += strlen(index2);

    memcpy(&pOutBuf[index], data, sizeof(data));
    index += sizeof(data);

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE GetRec(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;
    u8 *pRecName = "WATER";
    u32 recNo = 0;
    u8 index1[] = "1";
    u8 index2[] = "w";
    u8 len = strlen(pRecName);

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], len + 10 + strlen(index1) + strlen(index2) + 4, 2);
    index += 2;
    pOutBuf[index++] = STORE_REC_CLASS;
    pOutBuf[index++] = STORE_GET_REC;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], len, 2);
    index += 2;
    memcpy(&pOutBuf[index], pRecName, len);
    index += len;

    recNo = U32_ENDIAN(recNo);
    memcpy(&pOutBuf[index], &recNo, 4);
    index += 4;

    sdkU16ToBcd(&pOutBuf[index], strlen(index1), 2);
    index += 2;
    memcpy(&pOutBuf[index], index1, strlen(index1));
    index += strlen(index1);

    sdkU16ToBcd(&pOutBuf[index], strlen(index2), 2);
    index += 2;
    memcpy(&pOutBuf[index], index2, strlen(index2));
    index += strlen(index2);

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

//==========IC卡===========================
SDK_UI_EXIT_VALUE CheckICCard(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4, 2);
    index += 2;
    pOutBuf[index++] = IC_CARD_CLASS;
    pOutBuf[index++] = IC_CHECK;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    //pOutBuf[index++]=cmd;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE PowerOnICCard(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;
    u8 cardSlot = 0x06, cardType = 0x00;
    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x02 + 4, 2);
    index += 2;
    pOutBuf[index++] = IC_CARD_CLASS;
    pOutBuf[index++] = IC_POWER_ON;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = cardSlot;
    pOutBuf[index++] = cardType;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE PowerOffICCard(s32 ix) {
    u8 pOutBuf[100] = {0};
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4, 2);
    index += 2;
    pOutBuf[index++] = IC_CARD_CLASS;
    pOutBuf[index++] = IC_POWER_OFF;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    //pOutBuf[index++]=cmd;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE ICCardCommun(s32 ix) {
    Trace("", "^^^^^^^^^^^^ix:%d\r\n", ix);

    u8 pOutBuf[100] = {0};
    u8 index = 0;
    u8 cardtype = 0;
    ix++;

    switch (ix) {
        case 1:
            cardtype = 0;
            break;
        case 2:
            cardtype = 3;
            break;
        case 3:
            cardtype = 4;
            break;
        case 4:
            cardtype = 6;
            break;
        case 6:
            cardtype = 7;
            break;
    }

    //SDK_ICC_APDU_SEND pstApduSend;
    //memcpy(pstApduSend.Command,"",4);

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4 + 8, 2);
    index += 2;
    pOutBuf[index++] = IC_CARD_CLASS;
    pOutBuf[index++] = IC_COMMUN;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = cardtype;//card type IC1
    sdkU16ToBcd(&pOutBuf[index], 5, 2);
    index += 2;
    memcpy(&pOutBuf[index], "\x00\x84\x00\x00\x04", 5);// 4cmd,1 le
//	memcpy(&pOutBuf[index],"\x7E\x10\x00\x01\x04",5);// 4cmd,1 le
    index += 5;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

//==========显示类===========================
SDK_UI_EXIT_VALUE ClearScreen(s32 ix) {
    //STX		数据长度	指令号	指示位		序列号	可变数据	ETX		LRC
    // 1		2			2		1			1		可变		1		1

    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_CLEAR;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    //pOutBuf[index++]=cmd;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE LoadMenu(s32 ix) {
    u8 *pOutBuf = sdkGetMem(1024);

    if (pOutBuf == NULL)
        return SDK_UI_EXIT_ERR_VALUE;

    u16 index = 0;
    u32 filesize = 4 + getMenuItemCount() * 28;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], filesize + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_LOAD_MENU;
    pOutBuf[index++] = 0x11;
    pOutBuf[index++] = 0x22;

    TraceHex("mpos", "$$$$$$$", pOutBuf, 7);

    u8 *p = (u8 *) sdkGetMem(filesize);

    if (p == NULL) {
        Trace("mpos", "p NULL =============\n");
        sdkFreeMem(pOutBuf);
        return SDK_UI_EXIT_ERR_VALUE;
    }

    memset(p, 0, filesize);

    u8 timeout = 0x05;
    u8 items = getMenuItemCount();
    u16 len = items * 28;

    u32 idx = 0;
    p[idx++] = timeout;
    p[idx++] = items;

    sdkU16ToBcd(&p[idx], len, 2);

    idx += 2;

    int i;
    for (i = 0; i < items; i++) {
        len = MenuTab[i].usLvl;
        memcpy(&p[idx], &len, 2);
        idx += 2;

        if (MenuTab[i].bChildMenu)
            p[idx++] = 0x01;
        else
            p[idx++] = 0x00;

        memcpy(&p[idx], MenuTab[i].Disp, strlen((char *) MenuTab[i].Disp));
        idx += 20;
        memcpy(&p[idx], MenuTab[i].TradeCode, strlen((char *) MenuTab[i].TradeCode));
        idx += 5;
    }

    memcpy(&pOutBuf[index], p, filesize);

    index += filesize;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    sdkFreeMem(pOutBuf);
    sdkFreeMem(p);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE DrawText(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    u8 text[] = "hello,测试文字";
    u16 txtLen = strlen(text);

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], txtLen + 3 + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_DRAW_TEXT;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    sdkU16ToBcd(&pOutBuf[index], txtLen, 2);
    index += 2;
    memcpy(&pOutBuf[index], text, txtLen);
    index += txtLen;
    pOutBuf[index++] = 0x05;//timeout
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE SetBackLight(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;
    static bool flag = true;
    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x01 + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_SET_BACKLIGHT;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    if (flag) {
        pOutBuf[index++] = 1;
        flag = false;
    }
    else {
        pOutBuf[index++] = 0;
        flag = true;
    }
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE DrawLine(s32 ix) {
    u8 pOutBuf[100];
    u16 x1 = U16_ENDIAN(50), y1 = U16_ENDIAN(50), x2 = U16_ENDIAN(100), y2 = U16_ENDIAN(
            100), color = U16_ENDIAN(0);
    u8 index = 0;


    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 10 + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_DRAW_LINE;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], &x1, 2);
    index += 2;
    memcpy(&pOutBuf[index], &y1, 2);
    index += 2;
    memcpy(&pOutBuf[index], &x2, 2);
    index += 2;
    memcpy(&pOutBuf[index], &y2, 2);
    index += 2;
    memcpy(&pOutBuf[index], &color, 2);
    index += 2;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE DrawRect(s32 ix) {
    u8 pOutBuf[100];
    u16 x1 = U16_ENDIAN(150), y1 = U16_ENDIAN(150), x2 = U16_ENDIAN(100), y2 = U16_ENDIAN(
            100), color = U16_ENDIAN(0xff);
    u8 index = 0;
    static u8 mode = 0x00;
    if (mode == 0x01) {
        mode = 0x00;
        Trace("mpos", "########### mode=0\r\n");
    }
    else {
        mode = 0x01;
        Trace("mpos", "########### mode=1\r\n");
    }

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 11 + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_DRAW_RECT;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], &x1, 2);
    index += 2;
    memcpy(&pOutBuf[index], &y1, 2);
    index += 2;
    memcpy(&pOutBuf[index], &x2, 2);
    index += 2;
    memcpy(&pOutBuf[index], &y2, 2);
    index += 2;
    pOutBuf[index++] = mode;
    memcpy(&pOutBuf[index], &color, 2);
    index += 2;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE DrawBmp(s32 ix) {
    u8 *pOutBuf = sdkGetMem(1124);

    if (pOutBuf == NULL)
        return SDK_UI_EXIT_ERR_VALUE;

    //u8 pOutBuf[500];
    u16 index = 0;
    u16 val[] = {10, 10, 16, 16};
    u8 i;
    for (i = 0; i < 4; i++)
        val[i] = U16_ENDIAN(val[i]);
    //xy坐标,图片宽度,高度,图片数据LL1024点阵
    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 10 + sizeof(bmpBuf) + 4, 2);
    index += 2;
    pOutBuf[index++] = DISPLAY_CLASS;
    pOutBuf[index++] = DIP_DRAW_BMP;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], val, 8);
    index += 8;
    sdkU16ToBcd(&pOutBuf[index], sizeof(bmpBuf), 2);
    index += 2;
    memcpy(&pOutBuf[index], bmpBuf, sizeof(bmpBuf));
    index += sizeof(bmpBuf);
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    sdkFreeMem(pOutBuf);

    return SDK_UI_EXIT_ENTER;
}

//-------------------------------------------------
SDK_UI_EXIT_VALUE updateTMK_Test(s32 ix) {

    u8 key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08};

    //u8 data[]= { 0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88 };
    //u8 data_e[8]={0};
    //u8 data_ue[8]={0};
    s32 ret;
    if ((ret = sdkPEDUpdateTmk(110 - 102, SDK_PED_DES_TRIPLE, key, 200)) != SDK_OK) {
        Trace("", "$$$$$$ sdkPEDUpdateTmk fail\r\n");
    }
    else {
        Trace("", "###### sdkPEDUpdateTmk ok\r\n");
#if 0
        //测试思路
		//用主密钥加密一个数据当作加密的wk
		//然后将这个wk导入
		//再用wk加解密磁道数据 首先这边加解密的结果要能对上
		//最后用跟软件计算的结果对比下是否正确

		u8 wkey[]={0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,
				   0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x08};
		u8 wkey_e[16]={0};

		sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE,
			SDK_PED_ENCRYPT, SDK_PED_DES_ALL,wkey, 16, wkey_e);//加密16位wkey



		ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, data, data_e);
		if(ret != SDK_OK)
			Trace("","$$$$$$ sdkPEDUpdateTmk fail\r\n");
		else
		{

			TraceHex("", "bef encrypt\r\n", data, 8);
			ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, data, data_e);
			TraceHex("", "after encrypt\r\n", data_e, 8);
			ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, data_e, data_ue);
			Trace("", "after unencrypt ret %d\r\n", ret);
			TraceHex("", "after unencrypt\r\n", data_ue, 8);
		}
#endif
    }
    //sdkPEDUpdateWk(SDK_PED_WK_CFG * pstWkCfg, const s32 iWknum, s32 iTimeout)

    //更新密钥
    //加解密,对比


#if 0
    u8 val[]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08};
	u8 * pheTag = "\x9F\x99";
	s32 ret = sdkIccConfigTLV(pheTag, val, sizeof(val));

	if(ret != SDK_OK)
		Trace("","$$$$$$$ testSaveTLV1 sdkIccConfigTLV error :%d\r\n",ret);
	else
		Trace("","testSaveTLV1 sdkIccConfigTLV ok\r\n");
#endif
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE testSaveTLV2(s32 ix) {
    u8 key[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x00,
                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x08};
    u8 data[] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88};
    u8 data_e[8] = {0};
    u8 data_ue[8] = {0};
    s32 ret;
    if ((ret = sdkPEDUpdateTmk(1, SDK_PED_DES_TRIPLE, key, 200)) != SDK_OK) {
        Trace("", "$$$$$$ sdkPEDUpdateTmk fail\r\n");
    }
    else {
        ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, data, data_e);
        if (ret != SDK_OK)
            Trace("", "$$$$$$ sdkPEDUpdateTmk fail\r\n");
        else {

            TraceHex("", "bef encrypt\r\n", data, 8);
            ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, data, data_e);
            TraceHex("", "after encrypt\r\n", data_e, 8);
            ret = sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, data_e, data_ue);
            TraceHex("", "after unencrypt\r\n", data_ue, 8);

        }
    }

#if 0
    u8 val[]={0x11,0x22,0x33,0x44,0x55,0x66,0x99};
	u8 * pheTag = "\x9F\x99";
	s32 ret = sdkIccConfigTLV(pheTag, val, sizeof(val));

	if(ret != SDK_OK)
		Trace("","$$$$$$$ testSaveTLV2 sdkIccConfigTLV error :%d\r\n",ret);
	else
		Trace("","testSaveTLV2 sdkIccConfigTLV ok\r\n");
#endif
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE testReadTLV(s32 ix) {
    u8 val[50] = {0};
    u8 *pheTag = "\x9F\x99";
    s32 len;
    s32 ret = sdkIccReadTLV(pheTag, val, &len);

    if (ret != SDK_OK)
        Trace("", "$$$$$$$ sdkIccReadTLV error :%d\r\n", ret);
    else
        Trace("", "sdkIccReadTLV ok len=%d\r\n", len);

    TraceHex("", "pheTag:\r\n", val, len);

    return SDK_UI_EXIT_ENTER;
}

//===========================================================
SDK_UI_EXIT_VALUE TestOnSetBeep(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 14 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_SET_BEEP;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], "\x00\x05", 2);//蜂鸣次数
    index += 2;
    memcpy(&pOutBuf[index], "\x00\x00", 2);//蜂鸣频率
    index += 2;
    memcpy(&pOutBuf[index], "\x01\x00", 2);//蜂鸣时间
    index += 2;
    memcpy(&pOutBuf[index], "\x06\x00", 2);//蜂鸣间隔
    index += 2;

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnSetLED(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 5 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_SET_LED;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], "\x00\x08", 2);//闪烁次数
    index += 2;
    pOutBuf[index++] = 0x07;               //配置颜色
    memcpy(&pOutBuf[index], "\x00\x80", 2);//时间间隔
    index += 2;

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnMAC(s32 ix) {

//	return writeTag();

    u8 pOutBuf[100];
    u8 index = 0;

    u8 buf[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                 0x55, 0x44, 0x33};

    pOutBuf[index++] = SOP_DATA;

    index += 2;

    pOutBuf[index++] = PIN_PAD_CLASS;
    pOutBuf[index++] = LOAD_WK_ENUM;

    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;

    pOutBuf[index++] = 0x01;
    pOutBuf[index++] = 0x02;
    pOutBuf[index++] = 0x02;
    pOutBuf[index++] = 0x03;

    sdkU16ToBcd(&pOutBuf[index], sizeof(buf), 2);
    index += 2;

    memcpy(&pOutBuf[index], buf, sizeof(buf));
    index += sizeof(buf);

    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;

    pOutBuf[index++] = EOP_DATA;

    sdkU16ToBcd(&pOutBuf[1], index - 4, 2);

    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}


SDK_UI_EXIT_VALUE TestOnDataEncrypt(s32 ix) {

    return readTag();

    u8 pOutBuf[100];
    u8 index = 0;

    u8 buf[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, \
                 0x55, 0x44, 0x33};

    pOutBuf[index++] = SOP_DATA;
//	sdkU16ToBcd(&pOutBuf[index], 14 + 4 ,2);
    index += 2;

    pOutBuf[index++] = PIN_PAD_CLASS;
    pOutBuf[index++] = DATA_ENCRYPT_ENUM;

    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;

    pOutBuf[index++] = 0x01;
    pOutBuf[index++] = 0x02;

    sdkU16ToBcd(&pOutBuf[index], sizeof(buf), 2);
    index += 2;

    memcpy(&pOutBuf[index], buf, sizeof(buf));
    index += sizeof(buf);

    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;

    pOutBuf[index++] = EOP_DATA;

    sdkU16ToBcd(&pOutBuf[1], index - 4, 2);

    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}


SDK_UI_EXIT_VALUE TestOnSetDateTime(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 14 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_SET_DATETIME;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], "20141122111100", 14);
    index += 14;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnGetDateTime(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 14 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_GET_DATETIME;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnSetTerParam(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 19 + 12
                                 + 24 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_SET_TER_PARAM;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], "\xFF\x9F\x11\x0F", 4);
    index += 4;
    memcpy(&pOutBuf[index], "012345678912345", 15);
    index += 15;
    memcpy(&pOutBuf[index], "\xFF\x9F\x12\x08", 4);
    index += 4;
    memcpy(&pOutBuf[index], "12345678", 8);
    index += 8;
    memcpy(&pOutBuf[index], "\xFF\x9F\x13\x14", 4);
    index += 4;
    memcpy(&pOutBuf[index], "01234567890123456789", 20);
    index += 20;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnGetTerParam(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 11 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_GET_TER_PARAM;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x02;
    memcpy(&pOutBuf[index], "\xFF\x9F\x11", 3);
    index += 3;
    memcpy(&pOutBuf[index], "\xFF\x9F\x12", 3);
    index += 3;
    memcpy(&pOutBuf[index], "\xFF\x9F\x13", 3);
    index += 3;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnCtrlDev(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 1 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_CTRL_DEV;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x02;        //休眠指令
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnEcho(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 9 + 4, 2);
    index += 2;
    pOutBuf[index++] = TER_MANAGE_CLASS;
    pOutBuf[index++] = TM_ECHO;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    memcpy(&pOutBuf[index], "123456789", 9);
    index += 9;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);
    return SDK_UI_EXIT_ENTER;
}


//================================================================
SDK_UI_EXIT_VALUE TestOnOpenMagCardDevice(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    u8 *pText = "请插入IC卡!";
    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4 + 4 + strlen(pText), 2);
    index += 2;
    pOutBuf[index++] = MAGNETIC_CARD_CLASS;
    pOutBuf[index++] = MAG_OPEN_DEVICE;
    pOutBuf[index++] = 0x2F;
    pOutBuf[index++] = 0x01;
    pOutBuf[index++] = 0x07;//读磁道
    pOutBuf[index++] = 0x0A;//超时
    sdkU16ToBcd(&pOutBuf[index], strlen(pText), 2);
    index += 2;
    memcpy(&pOutBuf[index], pText, strlen(pText));
    index += strlen(pText);

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnCloseMagCardDevice(s32 ix) {
    u8 buf[128];
    u16 len = 0;

    memset(buf, 0x00, sizeof(buf));
    memcpy(buf, "\x02\x00\x04\xD1\x02\x2F\x01\x03\xFA", 0x09);
    len = 0x09;

    PostTransFlow(buf, len);
    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnReadMagCard(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4 + 1, 2);
    index += 2;
    pOutBuf[index++] = MAGNETIC_CARD_CLASS;
    pOutBuf[index++] = MAG_READ_CARDDATA_NEN;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x07;//2磁道 1 2 4

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnReadMagCardEncrypt(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;
    u8 mask[] = {0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00};
    u8 wkey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

    u8 wkey_e[16] = {0};
    u8 randN0[] = {0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02};
    u8 serNO[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03};

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 4 + 55 + 18, 2);
    index += 2;
    pOutBuf[index++] = MAGNETIC_CARD_CLASS;
    pOutBuf[index++] = MAG_READ_CARDDATA_EN;
    pOutBuf[index++] = 0x2F;
    pOutBuf[index++] = 0x01;
    pOutBuf[index++] = 0x02;//公钥索引 fixed
    pOutBuf[index++] = 0x07;//磁道
    memcpy(&pOutBuf[index], mask, 10);//屏蔽掩码
    index += 10;
    pOutBuf[index++] = 0x01;//加密算法
    pOutBuf[index++] = 110;//密钥索引

#if 1
    sdkPEDDesEx(110 - 102, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                SDK_PED_DES_ALL, wkey, 16, wkey_e);//加密16位wkey
    TraceHex("", "&&&&&& encryptd wkey\r\n", wkey_e, 16);

    sdkU16ToBcd(&pOutBuf[index], 16, 2);
    index += 2;
    memcpy(&pOutBuf[index], wkey_e, 16);//密钥密文....
    index += 16;
#endif

    memcpy(&pOutBuf[index], randN0, 8);//随机数
    index += 8;

    memcpy(&pOutBuf[index], serNO, 12);//流水号
    index += 12;

    pOutBuf[index++] = 0x00;//算法模式

    sdkU16ToBcd(&pOutBuf[index], 0x00, 2);
    index += 2;

    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnReadMagCardSave(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;
    u8 mask[] = {0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0xF0, 0x00, 0x00, 0x00};

    u8 wkey[] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08};

    u8 wkey_e[16] = {0};
    u8 randN0[] = {0x01, 0x01, 0x02, 0x02, 0x01, 0x01, 0x02, 0x02};
    u8 serNO[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03};

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 4 + 67 + 18, 2);
    index += 2;
    pOutBuf[index++] = MAGNETIC_CARD_CLASS;
    pOutBuf[index++] = MAG_READ_CARDDATA_INSIDE_SAVE;
    pOutBuf[index++] = 0x2F;
    pOutBuf[index++] = 0x01;
    pOutBuf[index++] = 0x02;//公钥索引 fixed
    pOutBuf[index++] = 0x07;//磁道
    memcpy(&pOutBuf[index], mask, 10);//屏蔽掩码
    index += 10;
    pOutBuf[index++] = 0x01;//加密算法
    pOutBuf[index++] = 110;//密钥索引

#if 1
    sdkPEDDesEx(110 - 102, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                SDK_PED_DES_ALL, wkey, 16, wkey_e);//加密16位wkey
    TraceHex("", "&&&&&& encryptd wkey\r\n", wkey_e, 16);

    sdkU16ToBcd(&pOutBuf[index], 16, 2);
    index += 2;
    memcpy(&pOutBuf[index], wkey_e, 16);//密钥密文....
    index += 16;
#endif

    memcpy(&pOutBuf[index], randN0, 8);//随机数
    index += 8;

    memcpy(&pOutBuf[index], serNO, 12);//流水号
    index += 12;

    pOutBuf[index++] = 0x00;//算法模式

    sdkU16ToBcd(&pOutBuf[index], 0x01, 2);
    index += 2;
    pOutBuf[index++] = 0x97;

    sdkU16ToBcd(&pOutBuf[index], 0x02, 2);
    index += 2;
    memcpy(&pOutBuf[index], "\x9F\x10", 2);
    index += 2;

    sdkU16ToBcd(&pOutBuf[index], 0x01, 2);
    index += 2;
    pOutBuf[index++] = 0x98;

    sdkU16ToBcd(&pOutBuf[index], 0x02, 2);
    index += 2;
    memcpy(&pOutBuf[index], "\x9F\x20", 2);
    index += 2;


    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnReadMagCardAudio(s32 ix) {
    u8 pOutBuf[100];
    u8 index = 0;

    pOutBuf[index++] = SOP_DATA;
    sdkU16ToBcd(&pOutBuf[index], 0x00 + 4, 2);
    index += 2;
    pOutBuf[index++] = MAGNETIC_CARD_CLASS;
    pOutBuf[index++] = MAG_READ_CARDDATA_AUDIO;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = 0x00;
    pOutBuf[index++] = EOP_DATA;
    pOutBuf[index] = GetCRC(&pOutBuf[1], index - 1);
    index++;

    PostTransFlow(pOutBuf, index);

    return SDK_UI_EXIT_ENTER;
}

SDK_UI_EXIT_VALUE TestOnCancelOper(s32 ix) {
    u8 buf[128];
    u16 len = 0;

    memset(buf, 0x00, sizeof(buf));

    memcpy(buf, "\x02\x00\x04\x1D\x08\x2F\x01\x03\xF6", 9);
    len = 9;

    PostTransFlow(buf, len);
    return SDK_UI_EXIT_ENTER;
}

#endif	//DEL_CODE_EXCESS	删除多余代码: 20150304

//Xiaox 15-1-12 需要指令结束时刷屏的指令
void SetScreenStatusWhenBack(u8 cmdClass, u8 cmdID) {
    int i = 0;
    struct _CMD {
        u8 cmdClass;
        u8 cmdID;
    };

    struct _CMD needBrushScreenCmd[] = {
//            {PBOC_CLASS,PBOC_EXE_FLOW},
            {TER_MANAGE_CLASS, TM_CANCEL_OPER},
            /*=======BEGIN: sujianzhong 2015.01.14  14:54 modify===========*/
//            {PIN_PAD_CLASS,PASSWD_INPUT_OUTER},
//            {PIN_PAD_CLASS,PASSWD_INPUT_ENUM},
            /*====================== END======================== */
    };
    struct _CMD needKeepScreenCmd[] = {
/*=======BEGIN: sujianzhong 2015.01.14  14:49 modify===========*/
            {PIN_PAD_CLASS,       PASSWD_INPUT_OUTER},
            {PIN_PAD_CLASS,       PASSWD_INPUT_ENUM},
/*====================== END======================== */
//            {DISPLAY_CLASS,DIP_DRAW_TEXT},
            {MAGNETIC_CARD_CLASS, MAG_OPEN_DEVICE},
            {PBOC_CLASS,          0},
            {DISPLAY_CLASS,       0},
            {PIN_PAD_CLASS,       0},      //zcl
            {STORE_REC_CLASS,     0},      //zcl
    };
    //不只这里会置起bisNeedBrushScreenWhenBack，交易过程中也会
    for (i = 0; i < sizeof(needBrushScreenCmd) / sizeof(struct _CMD); i++) {
        if (needBrushScreenCmd[i].cmdClass == cmdClass &&
            (needBrushScreenCmd[i].cmdID == cmdID || needBrushScreenCmd[i].cmdID == 0)) {
            bisNeedBrushScreenWhenBack = true;
            break;
        }
    }
    //只这里会置起bisNeedKeepScreenWhenBack
    for (i = 0; i < sizeof(needKeepScreenCmd) / sizeof(struct _CMD); i++) {
        if (needKeepScreenCmd[i].cmdClass == cmdClass &&
            (needKeepScreenCmd[i].cmdID == cmdID || needKeepScreenCmd[i].cmdID == 0)) {
            bisNeedKeepScreenWhenBack = true;
            break;
        }
    }
}


