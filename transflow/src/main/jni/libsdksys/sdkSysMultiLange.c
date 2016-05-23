#include "sdkGlobal.h"


#ifndef  DEBUG_MULTI_LANGUAGE
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#if 0
/*****************************************************************************
** Descriptions:
** Parameters:          u8 *lpOut
                               u16 lpOutlen
** Returned value:
** Created By:		fusuipu  2013.08.04
** Remarks:
*****************************************************************************/ 
static s32 sdkGetLangFileName(u8 *lpOut, u16 lpOutlen)
{
    u8 buf[64] = {0};
    SDK_SYS_LANG eLang;

    if (lpOut == NULL)
    {
        return SDK_PARA_ERR;
    }
    strcpy(buf, "lang");
    eLang = sdkSysMultiGetLangMode();

    switch (eLang)
    {
         case SDK_SYS_LANG_CHN:
           strcat(buf, "CHN");
           break;

         case SDK_SYS_LANG_ENG:
           strcat(buf, "ENG");
           break;

         case SDK_SYS_LANG_FAR:
           strcat(buf, "FAR");
           break;

         case SDK_SYS_LANG_ARA:
           strcat(buf, "ARA");
           break;

         default:
           break;
    }

    strcat(buf, ".ini");

    if (strlen(buf) > lpOutlen)
    {
        return SDK_ERR;
    }
    strcpy(lpOut, buf);
    return SDK_OK;
}
#endif

/*****************************************************************************
** Descriptions:	������ʾ����
** Parameters:      SDK_SYS_LANG eLang
** Returned value:
** Created By:		fusuipu  2013.05.23
** Remarks:         �˺�������ϵͳ��ʼ��ʱ�趨ϵͳ������Ϣ
*****************************************************************************/
static s32 sdkSysSetDispLang(SDK_SYS_LANG eLang) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ʾ����ת��
** Parameters:      u8 *lpOut
                    u8*lpIn
                    SDK_SYS_LANG eLang
** Returned value:
** Created By:		fusuipu  2013.05.23
** Remarks:         �˺�����ʵ�ֽ��û�������ʾ��Ϣת����ϵͳ��ʼ��ʱ�趨��������Ϣ
*****************************************************************************/
s32 Private_sdkSysLangChange(u8 *lpOut, const u8 *lpIn) {
    if (NULL == lpOut || NULL == lpIn) {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

typedef struct       //JLS added for display
{
    s32 ascii_font_type;
    s32 main_font_type;
} SDK_MULTI_FONT;

/*****************************************************************************
** Descriptions:
** Parameters:          s32 uiLangMode
                               u8 uiMaxLine
** Returned value:
** Created By:		fusuipu  2013.12.29
** Remarks:
*****************************************************************************/
static s32 sdkInitMultiLangMode(s32 uiLangMode, u8 uiMaxLine, s32 *eLang,
                                SDK_MULTI_FONT *pMultiFont) {
    static void (*p_fun)(s32 uiLangMode, u8 uiMaxLine, s32 *eLang,
                         SDK_MULTI_FONT *pMultiFont) = NULL;

#if 0 /*Modify by ���ƽ at 2015.01.19  15:32 */

    if(p_fun == NULL)
    {
        p_fun =  (void (*)(s32 uiLangMode, u8 uiMaxLine, s32 *eLang, SDK_MULTI_FONT *pMultiFont))sdk_dev_get_multilib_fun_hand("sdk_dev_set_multiLang_mode");//lint !e611
    }
#endif /* if 0 */

    if (p_fun != NULL) {
        p_fun(uiLangMode, uiMaxLine, eLang, pMultiFont);
        return SDK_OK;
    }
    else {
        Trace("fsp", "warming: sdk_dev_set_multiLang_mode load fail");
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	ϵͳ��������
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.08.05
** Remarks:             ���ڶ���������
*****************************************************************************/
typedef s32 (*P_LANGUAGESET)(s32 eLang);

static s32 sdkMultiLanguageSet(s32 eLang) {
    //static s32 (*p_fun)(s32 eLang) = NULL;
    P_LANGUAGESET p_fun = NULL;
#if 0 /*Modify by ���ƽ at 2015.01.19  15:32 */
    if(p_fun == NULL)
    {
        p_fun =  (P_LANGUAGESET)sdk_dev_get_multilib_fun_hand("sdk_dev_language_set");
    }
#endif /* if 0 */

    if (p_fun != NULL) {
        return p_fun(eLang);
    }
    else {
        Trace("dev", "warming: dev_language_set");
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���������С
** Parameters:          &sysFont
** Returned value:
** Created By:		fusuipu  2013.08.05
** Remarks:             ���ڶ���������
*****************************************************************************/
typedef s32 (*P_FONTSET)(SDK_MULTI_FONT *currlcdfont);

static s32 sdkMultiLcdSetFontExt(SDK_MULTI_FONT *currlcdfont) {
    //static s32 (*p_fun)(SDK_MULTI_FONT *currlcdfont) = NULL;
    P_FONTSET p_fun = NULL;

#if 0 /*Modify by ���ƽ at 2015.01.19  15:32 */
    if(p_fun == NULL)
    {
        p_fun =  (P_FONTSET)sdk_dev_get_multilib_fun_hand("sdk_dev_lcd_set_font_ext");
    }
#endif /* if 0 */

    if (p_fun != NULL) {
        p_fun(currlcdfont);
    }
    else {
        Trace("dev", "warming: sdk_dev_lcd_set_font_ext");
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�ж��Ƿ�Ϊ�����������
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.12.29
** Remarks:
*****************************************************************************/
s32 sdkIsMultiLang(void) {
    s32 lang = sdkSysGetLang();
    //Trace("fsp", "enter s32 sdkIsMultiLang(void)");
    if (SDK_SYS_LANG_CHN != lang &&
        SDK_SYS_LANG_ENG != lang) {
        return true;
    }
    return false;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_SYS_LANG uiLangMode
                               u8 uiMaxLine
** Returned value:
** Created By:		fusuipu  2013.08.04
** Remarks:
*****************************************************************************/
s32 sdkSysMultiLangInit(SDK_SYS_LANG uiLangMode, u8 uiMaxLine) {
//    u8 lang_file[64] = {0};
    s32 ret;
    s32 dev_lang = 0;                           //����ģʽ�������޸�
    s32 line_mode = 5;
    s32 lang_mode = SDK_SYS_LANG_CHN;
    SDK_MULTI_FONT multi_lcd_font = {1, 26};    //����ģʽ��Ĭ��ֵ�������޸�

    if (uiLangMode < SDK_SYS_LANG_CHN || uiLangMode > SDK_SYS_LANG_DEF) {
        return SDK_PARA_ERR;
    }


    if (4 == uiMaxLine) {
        line_mode = uiMaxLine;
    }

    if (SDK_SYS_LANG_RUS == uiLangMode) {
        line_mode = 4;                  //����˹ֻ֧��4��ģʽ
    }

    if (SDK_SYS_LANG_ENG == uiLangMode)              //���������Ӣ�ģ�ϵͳĬ������Ϊ֧��Ӣ�ĵ�����ģʽ
    {
        lang_mode = SDK_SYS_LANG_CHN;
    }
    else {
        lang_mode = uiLangMode;
    }
    sdkSysSetLang(lang_mode);
    sdkDispSetMaxRow(line_mode);
    sdkSysSetDispLang(uiLangMode);                  //�û�������ini�ļ��н���ʾ������ת��Ϊ��ʾӢ��
    ret = sdkInitMultiLangMode(uiLangMode, uiMaxLine, &dev_lang, &multi_lcd_font);

    if (ret < 0) {
        Trace("fsp", "sdkInitMultiLangMode failed");
    }
    sdkMultiLanguageSet(dev_lang);
    sdkMultiLcdSetFontExt(&multi_lcd_font);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ��ʾ����ģʽ
** Parameters:          SDK_SYS_LINE_INFOR *pstDipsLineInfo
** Returned value:
** Created By:		fusuipu  2014.11.19
** Remarks:
*****************************************************************************/
s32 sdkSysMultiGetLineMode(SDK_SYS_LINE_INFOR *pstDipsLineInfo) {
    if (NULL == pstDipsLineInfo) {
        return SDK_PARA_ERR;
    }
    pstDipsLineInfo->eLine = (SDK_SYS_LINE_MODE) sdkSysGetSreenMaxLine();
    pstDipsLineInfo->eLineMode = sdkSysGetDispLineMode();
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������ʾ����ģʽ
** Parameters:          SDK_SYS_LINE_INFOR *pstDipsLineInfo
** Returned value:
** Created By:		fusuipu  2014.11.19
** Remarks:
*****************************************************************************/
s32 sdkSysMultiSetLineMode(const SDK_SYS_LINE_INFOR *pstDipsLineInfo) {
    if (NULL == pstDipsLineInfo) {
        return SDK_PARA_ERR;
    }
    sdkSysSetSreenMaxLine(pstDipsLineInfo->eLine);
    sdkSysSetDispLineMode(pstDipsLineInfo->eLineMode);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ��ʾģʽ
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.01.10
** Remarks:
*****************************************************************************/
SDK_SYS_LANG sdkSysMultiGetLangMode(void) {
    return sdkSysGetLang();
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ�ַ������������ֽ����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
s32 Private_sdkSysMultiGetLetterLen(const u8 *pStr) {
    if (0xe1 == *pStr)                                //��³����
    {
        return 3;
    }

    else if (0xD9 == *pStr || 0xD0 == *pStr ||
             (Private_sdkJudgeChn(pStr[0], pStr[1], 0, 0) == true))   //��˹�����뷨����ĸutf-8��0xD9��ͷ
    {
        return 2;
    }
    else if (*pStr < 0x80 && *pStr > 0)                            //ASC
    {
        return 1;
    }
    else {
        return 1;
        //return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���㵱ǰ�ַ�������ĸ����/�ַ�������
** Parameters:          u8 *pStr
                    s32 bIsByteNum
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
static s32 sdkMultiCalueStrAriNumOrLen(const u8 *pStr, bool bIsGetNum) {
    s32 i = 0;
    s32 count = 0;
    s32 len = 0;

    if (NULL == pStr) {
        return SDK_PARA_ERR;
    }

    while (0 != pStr[i]) {
        if ((len = Private_sdkSysMultiGetLetterLen(&pStr[i])) > 0) {
            count++;
            i += len;
        }
        else {
            return SDK_ERR;
        }
    }

    if (true == bIsGetNum)               //�����ַ�������������ĸ����
    {
        return count;
    }
    else                                //�����ַ����ĳ���
    {
        return i;
    }
}

/*****************************************************************************
** Descriptions:	���㵱ǰ�ַ�������ĸ����
** Parameters:          u8 *pStr
                    s32 bIsByteNum
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
s32 Private_sdkMultiStrAriNum(const u8 *pStr) {
    return sdkMultiCalueStrAriNumOrLen(pStr, true);
}

/*****************************************************************************
** Descriptions:	�����������ַ�������
** Parameters:          u8 *pStr
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
s32 Private_sdkMultiGetStrLen(const u8 *pStr) {
    return sdkMultiCalueStrAriNumOrLen(pStr, false);
}

/*****************************************************************************
** Descriptions:	�ж϶�������ַ������Asc�Ķ��ٱ�
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2014.01.14
** Remarks:             �ڶ�����ģʽ�£���������ַ�����п�����Asc���ַ���ȵ�2��
*****************************************************************************/
static bool sdkMultiIsAscEquelMultiLangWidth() {
    s32 lang = sdkSysMultiGetLangMode();

    if (SDK_SYS_LANG_CHN == lang) {
        return false;
    }
    return true;
}

/*****************************************************************************
** Descriptions:	��ȡָ���ַ��������ַ�������
** Parameters:          u8 *pStr
                               s32 siLetterNum
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:         ��ascΪ��λ��һ��ȫ���ַ���2��asc
*****************************************************************************/
s32 Private_sdkMulitGetLcdOneLineLetterMaxLen(const u8 *pStr, s32 siLetterNum) {
    s32 i = 0;
    s32 count = 0;
    s32 len = 0; //lint !e818

    TraceHex("fspdispmulti", "sdkMmiCalueStrAriNumAndByte date:", pStr, strlen(pStr));

    if (NULL == pStr || 0 == siLetterNum) {
        return SDK_PARA_ERR;
    }

    //Ŀǰ��ʱʹ��strlen������ȡ�ַ������ȣ����Ƕ��ڶ�����ԵĿ��ַ����Ǵ���bug��
    //�Ժ��ڴ˴�ͳһ������ַ����ַ�����������


    while (0 != pStr[i] && '\r' != pStr[i] && '\n' != pStr[i]) {
        if ((len = Private_sdkSysMultiGetLetterLen(&pStr[i])) > 0) {
            i += len;
        }

        /*�ڶ�������У�����siLetterNum��ָ������Ե��ַ�������ʵ��LCD��Ļ��ʾ��ʱ��
           ��ʱ��һ�����������ĸ��ռ�Ŀ�ȿ�����ʾ����Asc�������ڴ˴�Ҫ�����жϴ��� */
        if (1 == len && false == sdkMultiIsAscEquelMultiLangWidth())                //ASC
        {
            count++;
        }
        else {
            count += 2;
        }

        if (siLetterNum == count) {
            break;
        }
            //������һ��ֻ������һ��Acs����ʱ�����һ��ȫ���ַ�����������һ�����������ĸ
        else if (siLetterNum < count) {
            i -= len;
            break;
        }
    }

    return i;
}

/*****************************************************************************
** Descriptions:	��ȡ�ַ������һ����ĸ��ռ�ֽڸ���
** Parameters:          u8 *pStr
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
s32 Private_sdkMultiGetLastLetterLen(const u8 *pStr) {
    s32 byte_len = 0;
    u8 len = (u8) Private_sdkMultiGetStrLen(pStr);

#if 0 /*Modify by ���ٲ� at 2014.03.04  17:23 */
    if(len < 0)
    {
        return SDK_PARA_ERR;
    }
#endif /* if 0 */

    if (0xe1 == pStr[len - 3])        //��ǰLCD��ʾ��������ͷ�ϵ��ַ�Ϊ��³������
    {
        byte_len = 3;
    }
    else if (0xD9 == pStr[len - 2] || 0xD0 == pStr[len - 2] ||
             (Private_sdkJudgeChn(pStr[len - 2], pStr[len - 1], 0, 0) == true)) {
        byte_len = 2;
    }
    else if (pStr[len - 1] < 0x80 && pStr[len - 1] > 0) {
        byte_len = 1;
    }
    else {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return byte_len;
}

/*****************************************************************************
** Descriptions:	�ж��ַ����ǲ���ȫasc
** Parameters:          u8 *pStr
** Returned value:
** Created By:		fusuipu  2014.01.23
** Remarks:
*****************************************************************************/
bool Private_sdkMmiMultiIsAscStr(const u8 *pStr) {
    s32 len = 0;

    while (0 != pStr[len]) {
        if (1 != Private_sdkSysMultiGetLetterLen(&pStr[len])) {
            return false;
        }
        len++;
    }

    return true;
}

/*****************************************************************************
** Descriptions:	asc�ַ��������㷨
** Parameters:          char *pStr
** Returned value:
** Created By:		fusuipu  2014.01.23
** Remarks:
*****************************************************************************/
s32 Private_sdkMmiMultiReverse(u8 *pStr) {
    s32 len;
    s32 i;
    u8 tmp = 0;

    if ((len = strlen(pStr)) == 0) {
        return SDK_PARA_ERR;
    }
    else if (len == 1) {
        return SDK_OK;
    }

    for (i = 0; i < len / 2; i++) {
        tmp = pStr[i];
        pStr[i] = pStr[len - (i + 1)];
        pStr[len - (i + 1)] = tmp;
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���㵱ǰ�ַ�������ɰ��/ȫ��ʱ���ַ�����
** Parameters:          u8 *pStr
                               s32 siLetterNum
** Returned value:
** Created By:		fusuipu  2014.01.13
** Remarks:
*****************************************************************************/
s32 sdkMultiGetStrNumAsHalfAgle(const u8 *pStr, bool bIsHalfAgle) {
    s32 i = 0;
    s32 count = 0;
    s32 len = 0;

    TraceHex("fspdispmulti", "sdkMmiCalueStrAriNumAndByte date:", pStr, strlen(pStr));

    if (NULL == pStr) {
        return SDK_PARA_ERR;
    }

    while (0 != pStr[i] && '\r' != pStr[i] && '\n' != pStr[i]) {
        len = Private_sdkSysMultiGetLetterLen(&pStr[i]);

        if (len > 0) {
            if (1 == len) {
                count += 1;             //���
            }
            else {
                count += 2;
            }
            i += len;
        }
        else {
            Assert(0);
            return SDK_ERR;
        }
    }

    if (true == bIsHalfAgle) {
        return count;
    }
    else {
        return count / 2;
    }
}

