#include "sdkGlobal.h"
#include "sdkPrivateIO.h"


#ifndef  DEBUG_IME
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


//extern void sdkMmiStrokeInput(u8 key, SDK_KEY_SCANFMT *pScan, SDK_KEY_STROKEPOSGROUP *posgroup, SDK_KEY_STROKEHZBUF *hzbuffer);
//extern void sdkMmiT9(u8 key, SDK_KEY_SCANFMT *pScan);
#define SDK_KEY_TAB_MAX_LEN 40

static u32 Sdk_timerID = 0;
static u16 gOldPwdLen = 0;
static u8 const Sdk_SpecSymbol[] = "~!@#$%^&*()_+|\\=-`<>?\":{}[];\'.,/";

/*****************************************************************************
** Descriptions:        ת����С�л���
** Parameters:          u8 KeyTab[MULKEYNUM][7]
                               u8 Bigif
** Returned value:
** Created By:        lilin2012.09.14
** Remarks:
*****************************************************************************/
static void sdkSysGetSysKeyPad(u8 KeyTab[MULKEYNUM][7], u8 Mode) {
    static const u8 sdkGBigLetterKeyPad[MULKEYNUM][7] =
            {
                    {"1QZ.,@"},
                    {"2ABC"},
                    {"3DEF"},
                    {"4GHI"},
                    {"5JKL"},
                    {"6MNO"},
                    {"7PRS"},
                    {"8TUV"},
                    {"9WXY"},
                    {"0*#- "},
            };
    //T9����Сд��ĸ���
    static const u8 sdkGMinLetterKeyPad[MULKEYNUM][7] =
            {
                    {"1qz.,@"},
                    {"2abc"},
                    {"3def"},
                    {"4ghi"},
                    {"5jkl"},
                    {"6mno"},
                    {"7prs"},
                    {"8tuv"},
                    {"9wxy"},
                    {"0*#- "},
            };
    //�绰�������
    static const u8 sdkTelLetterKeyPad[MULKEYNUM][7] =
            {
                    {"1,;@"},
                    {"2ABC"},
                    {"3D"},
                    {"4"},
                    {"5"},
                    {"6"},
                    {"7"},
                    {"8"},
                    {"9W"},
                    {"0*-"},
            };

    u8 i;

//    s32 machine_type = sdkSysGetMachineCode(NULL);

    for (i = 0; i < MULKEYNUM; i++) {
        if (MODE_BIG_LET == Mode) {       //fusuipu 2013.01.08 14:0
            strcpy(KeyTab[i], sdkGBigLetterKeyPad[i]);
        }
        else if (MODE_SMA_LET == Mode) {
            //fusuipu 2013.01.08 14:1
            strcpy(KeyTab[i], sdkGMinLetterKeyPad[i]);
        }
        else if (MODE_TEL_LET == Mode) {
            //fusuipu 2013.01.08 14:3
            strcpy(KeyTab[i], sdkTelLetterKeyPad[i]);
        }
        else {
            //fusuipu 2013.01.08 14:3
            Assert(0);
        }
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.06.13
** Remarks:
*****************************************************************************/
u8 sdksysGetT9keyNum(void) {
    s32 machine_type = sdkSysGetMachineCode(NULL);

    /*=======BEGIN: wangmingming 2014.06.07  10:1 modify===========*/
    if (machine_type == SDK_SYS_MACHINE_G810 ||
        machine_type == SDK_SYS_MACHINE_G870 ||
        machine_type == SDK_SYS_MACHINE_G870D ||
        machine_type == SDK_SYS_MACHINE_K508A ||
        machine_type == SDK_SYS_MACHINE_G2 ||
        machine_type == SDK_SYS_MACHINE_200P
            ) {
        //Trace("test", "\r\n>>> sdksysGetT9keyNum 9 <<<\r\n");
        return 9;
    }
    else {
        //Trace("test", "\r\n>>> sdksysGetT9keyNum 8 <<<\r\n");
        return 8;
    }
    /*====================== END======================== */
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 Tab[][5]
** Returned value:
** Created By:		fusuipu  2013.06.13
** Modified By:		huangkangkui 2015.02.12
** Remarks:
*****************************************************************************/
void SdkGetT9Tab(u8 Tab[][5]) {
    u8 i;
    static const u8 gfstlettertab[9][5] =
            {
                    {"qz"},
                    {"abc"},
                    {"def"},
                    {"ghi"},
                    {"jkl"},
                    {"mno"},
                    {"prs"},
                    {"tuv"},
                    {"wxy"},
            };

    for (i = 0; i < sdksysGetT9keyNum(); i++) {
        //Trace("test", "\r\n>>> gfstlettertab <<<\r\n");
        strcpy(Tab[i], gfstlettertab[i]);
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *Keyv
                               u8 *Letternum
** Returned value:
** Created By:		fusuipu  2013.06.13
** Remarks:
*****************************************************************************/
s32 sdksysGetT9key(u8 *Keyv, u8 *Letternum) {
    u8 i = sdksysGetT9keyNum();
    s32 machine_type = sdkSysGetMachineCode(NULL);

    if (Keyv == NULL || Letternum == NULL) //shiweisong 2013.01.30 16:20
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    /*=======BEGIN: wangmingming 2014.06.07  10:0 modify===========*/
    if (machine_type == SDK_SYS_MACHINE_G810 ||
        machine_type == SDK_SYS_MACHINE_G870 ||
        machine_type == SDK_SYS_MACHINE_G870D ||
        machine_type == SDK_SYS_MACHINE_K508A ||
        machine_type == SDK_SYS_MACHINE_G2 ||
        machine_type == SDK_SYS_MACHINE_200P
            ) {
        //Trace("test", "\r\n>>> 123456789 <<<\r\n");
        memcpy(Keyv, "123456789", i);
        memcpy(Letternum, "\x02\x03\x03\x03\x03\x03\x03\x03\x03", i);
    }
    else {
        //Trace("test", "\r\n>>> 23456789 <<<\r\n");
        memcpy(Keyv, "23456789", i);
        memcpy(Letternum, "\x03\x03\x03\x03\x03\x04\x03\x04", i);
    }
    /*====================== END======================== */
    return SDK_OK;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  �����ȡ���һ������
                            ����ѵ�ǰ�����ݸ���dispbuf.
   ��ڲ���:  ����ʾ�ַ�ָ��buf, ʵ����ʾ�ַ���ָ��dispbuf,
   �� �� ֵ:
   ��	  ע:  ���������֤ǰ����������ֵ�,����֤������������ֵ�
 ********************************************************************/
static u8 sdkMmiCountLineChar_L(const u8 *pStr) {
    u8 i = 0, len = 0;

    while (1)                                     //ÿһ��
    {
        if (pStr[i] == 0)                         //����ʾ�ַ���β
        {
            len = i;                             //ȡ�����еĳ���
            break;
        }

        if (pStr[i] > 0x80)                       //�Ǻ���
        {
            i += 2;                              //����һ�����ֵĳ���

            if (i > Private_sdkDispLineMaxLen())            //��β
            {
                len = i - 2;                     //ȡ�����еĳ���
                break;
            }
        }
        else                                     //���Ǻ���
        {
            i++;

            if (i > Private_sdkDispLineMaxLen())            //��β
            {
                len = i - 1;                     //ȡ�����еĳ���
                break;
            }
        }
    }

    return len;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ҷ���ȡ���һ������
                            ����ѵ�ǰ�����ݸ���dispbuf.
   ��ڲ���:  ����ʾ�ַ�ָ��buf, ʵ����ʾ�ַ���ָ��dispbuf,
   �� �� ֵ:
   ��	  ע:  ���������֤ǰ����������ֵ�,����֤������������ֵ�
 ********************************************************************/
static u8 sdkMmiCountLineChar_R(const u8 *pStr, const u8 *pEnd) {
    u8 i = 0;

    s32 len = 0, length = pEnd - pStr;

    if (length <= Private_sdkDispLineMaxLen())                 //����ʾ�ַ���β
    {
        if (pEnd < pStr) { len = 0; }
        else {
            len = pEnd - pStr;                      //ȡ�����еĳ���
        }
        return (u8) len;
    }

    while (1) {
        if (pStr[i] > 0x80)                          //�����ַ�
        {
            i += 2;

            if (i == (length - Private_sdkDispLineMaxLen())) {
                len = Private_sdkDispLineMaxLen();
                break;
            }

            if (i > (length - Private_sdkDispLineMaxLen()))    //���һ���Ǻ���
            {
                len = Private_sdkDispLineMaxLen() - 1;
                break;
            }
        }
        else {
            i++;

            if (i == (length - Private_sdkDispLineMaxLen())) {
                len = Private_sdkDispLineMaxLen();
                break;
            }
        }
    }

    return (u8) len;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���ݶ��뷽ʽ���㻻��,��dispbuf == NULL �����������,
                            ����ѵ�ǰ�����ݸ���dispbuf.
   ��ڲ���:  ����ʾ�ַ�ָ��buf, ʵ����ʾ�ַ���ָ��dispbuf,
   �� �� ֵ:
   ��	  ע:  ���������֤ǰ����������ֵ�,����֤������������ֵ�
 ********************************************************************/
static u8 sdkMmiCountLine(u8 *buf, u8 *dispbuf, u8 align, SDK_KEY_SCANMEMBER ScanMem) {
    u8 *p, *pEnd, len, MmiLineNum;

    MmiLineNum = 1;

    if ((s32) strlen(buf) <= Private_sdkDispLineMaxLen()) {
        MmiLineNum = 1;

        if (dispbuf != NULL) { memcpy(dispbuf, buf, strlen(buf)); }
    }
    else {
        if (align == SDK_DISP_RDISP)                     //�Ҷ��뷽ʽ:��ʾ����(�Ӻ���ǰ������)
        {
            p = buf + strlen(buf);

            while (1) {
                len = sdkMmiCountLineChar_R(buf, p);
                p -= len;

                if ((MmiLineNum == ScanMem.MmiCurrentRow) &&
                    (dispbuf != NULL))                //������м���ǰ��ʾ��
                {
                    memcpy(dispbuf, p, len);            //����Ҫ��ʾ���ַ���д��ָ���ڴ�
                }

                if (p <= buf) { break; }
                MmiLineNum++;
            }
        }
        else                                            //��������з�ʽ:��ʾ���к�����(��ǰ���������)
        {
            p = buf;                                    //ָ��ָ�����ʾ�ַ���ͷ
            pEnd = buf + strlen(buf);

            while (1) {
                len = sdkMmiCountLineChar_L(p);

                if ((MmiLineNum == ScanMem.MmiCurrentRow) &&
                    (dispbuf != NULL))                //������м���ǰ��ʾ��
                {
                    memcpy(dispbuf, p, len);            //����Ҫ��ʾ���ַ���д��ָ���ڴ�
                }
                p += len;

                if (p >= pEnd) {
                    break;                              //���������ʾ�ַ�β,����ֹѭ��
                }
                MmiLineNum++;                           //������1
            }
        }
    }
    return MmiLineNum;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ������ʾ
   ��ڲ���:  ��ҳ��־flag == 1 ���Ϸ�flag == -1 ���·�flag == 0 ��ʼ����ʾ
   �� �� ֵ:
   ��	  ע:  ���������֤ǰ����������ֵ�,����֤������������ֵ�
 ********************************************************************/
u8 sdkMmiChangeLine(s16 flag, SDK_KEY_SCANFMT *pScan) {
    u8 align, MmiLineNum;
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    if ((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT) {
        align = SDK_DISP_LDISP;
    }
    else {
        align = SDK_DISP_RDISP;
    }
    MmiLineNum = sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);

    if (flag < 0)                            //���Ϸ�
    {
        if (align == SDK_DISP_RDISP)         //�Ҷ��룬���Ϸ���������
        {
            if (pScanMem->MmiCurrentRow >= MmiLineNum) {
                pScanMem->MmiCurrentRow = MmiLineNum;
                return false;
            }
            else {
                pScanMem->MmiCurrentRow++;
                return true;
            }
        }
        else                                //���������У����Ϸ���������
        {
            if (pScanMem->MmiCurrentRow <= 1) {
                pScanMem->MmiCurrentRow = 1;
                return false;
            }
            else {
                pScanMem->MmiCurrentRow--;
                return true;
            }
        }
    }
    else if (flag > 0) {
        if (align == SDK_DISP_RDISP)         //�Ҷ��룬���·���������
        {
            if (pScanMem->MmiCurrentRow <= 1) {
                pScanMem->MmiCurrentRow = 1;
                return false;
            }
            else {
                pScanMem->MmiCurrentRow--;
                return true;
            }
        }
        else                                //���������У����Ϸ���������
        {
            if (pScanMem->MmiCurrentRow >= MmiLineNum) {
                pScanMem->MmiCurrentRow = MmiLineNum;
                return false;
            }
            else {
                pScanMem->MmiCurrentRow++;
                return true;
            }
        }
    }
    else {
        sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);

        if (!(align == SDK_DISP_RDISP))      //�Ҷ��룬���·���������
        {
            pScanMem->MmiCurrentRow = MmiLineNum;
        }
        else {
            pScanMem->MmiCurrentRow = 1;
        }
        return true;                        //��ʱ����ֵû��ʵ������
    }
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ������̻�����
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiClearKey(SDK_KEY_SCANMEMBER *pScanMem) {
    memset(pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
    pScanMem->MmiKeyLen = 0;
}

/*****************************************************************************
** Descriptions:    ��ʾ����
** Parameters:          SDK_KEY_SCANFMT *pScan
** Returned value:
** Created By:		wanggaodeng  2012.12.27
** Remarks:
*****************************************************************************/
static void sdkMmiDisplayPwdAgain(const SDK_KEY_SCANFMT *pScan) {
    u8 align, disp;
    u8 disbuf[128];

    u32 len = strlen((char *) pScan->Contents);

    memset(disbuf, 0, sizeof(disbuf));

    if ((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT) {
        align = SDK_DISP_LDISP;
    }
    else {
        align = SDK_DISP_RDISP;
    }

    if ((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP) {
        disp = SDK_DISP_NOFDISP;
    }
    else {
        disp = SDK_DISP_FDISP;
    }

    if (len > 0) {
        if (len <= 20) {
            memset(disbuf, '*', len);
        }
        else {
            memset(disbuf, '*', 20);
        }
    }
    else {
        strcpy(disbuf, STR_CURSOR);               //20090402
    }
    Private_sdkDispClearRowRam(pScan->RowNo);               //�����ض����ַ�
    Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
    sdkDispBrushScreen();
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ��������
   ��ڲ���:  ͨ������ṹ��
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
void sdkMmiDisplayScanf(SDK_KEY_SCANFMT *pScan) {
    u8 len, i, j, align, disp;
    //u8 disbuf[128], temp[128] = {0};
    u8 disbuf[64], temp[64] = {0};
    s32 ret = 0;

    len = (u8) strlen(pScan->Contents);
    memset(disbuf, 0, sizeof(disbuf));

    if (NULL != pScan->sdkMmiDispCallFun) {
        ret = pScan->sdkMmiDispFun(pScan->Contents, pScan->Mask, pScan->RowNo, pScan->pVar);

        if (SDK_OK == ret) {
            return;
        }
    }

    if ((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT) {
        align = SDK_DISP_LDISP;
    }
    else {
        align = SDK_DISP_RDISP;
    }

    if ((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP) {
        disp = SDK_DISP_NOFDISP;
    }
    else {
        disp = SDK_DISP_FDISP;
    }

    if ((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)      //���Խ��
    {
        if (strlen(pScan->Contents) == 0)            //ֻ������һλ
        {
            memcpy(temp, "0.00", 4);
        }

        if (strlen(pScan->Contents) == 1)            //ֻ������һλ
        {
            temp[0] = '0';
            temp[1] = '.';
            temp[2] = '0';
            temp[3] = pScan->Contents[0];
        }
        else if (strlen(pScan->Contents) == 2)       //ֻ��������λ
        {
            temp[0] = '0';
            temp[1] = '.';
            temp[2] = pScan->Contents[0];
            temp[3] = pScan->Contents[1];
        }
        else if (strlen(pScan->Contents) > 2)        //�������ĳ��ȴ���2
        {
            for (i = 0; i < (strlen(pScan->Contents) - 2); i++) {
                temp[i] = pScan->Contents[i];
            }

            temp[i] = '.';
            i++;
            temp[i] = pScan->Contents[i - 1];
            i++;
            temp[i] = pScan->Contents[i - 1];
        }                                           //������ʾ���е�λ��
        sdkMmiCountLine(temp, disbuf, align, pScan->ScanMem);
    }
    else if ((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME)   //����ʱ��
    {
        memcpy(disbuf, SYS_TIME_FORMAT, 19);
        j = 0;

        for (i = 0; i < len; i++) {
            if (i == 4 || i == 6 || i == 8 || i == 10 || i == 12) { j++; }
            disbuf[j] = pScan->Contents[i];
            j++;
        }

        align = SDK_DISP_CDISP;
    }
    else if ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD)     //��������
    {
        if (len > 0)                                           //wanggaodeng modify
        {
            if (pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET ||
                pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET) {
                if (len <= Private_sdkDispLineMaxLen()) {
                    memset(disbuf, '*', len - 1);
                    disbuf[len - 1] = pScan->Contents[len - 1];
                }
                else {
                    memset(disbuf, '*', (u32)(Private_sdkDispLineMaxLen() - 1));
                    disbuf[Private_sdkDispLineMaxLen() - 1] = pScan->Contents[len - 1];
                }
            }
            else {
                if (len <= Private_sdkDispLineMaxLen()) {
                    memset(disbuf, '*', len);
                }
                else {
                    memset(disbuf, '*', (u32) Private_sdkDispLineMaxLen());
                }
            }
        }
        else {
            strcpy(disbuf, STR_CURSOR);             //20090402
        }
    }
    else                                            //����
    {
        if (len == 0)                                // ��
        {
            Private_sdkDispClearRowRam(pScan->RowNo);           //�����ض����ַ�
            Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, STR_CURSOR, disp | align) ==
                   SDK_OK);
            sdkDispBrushScreen();

            return;
        }
        else {                                           //������ʾ���е�λ��
            sdkMmiCountLine(pScan->Contents, disbuf, align, pScan->ScanMem);
        }
    }
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P) /*Modify by wangmingming at 2014.06.27  16:9 */

    //�����������
    if ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD) {
        if (gOldPwdLen != len) {
            Private_sdkDispClearRowRam(pScan->RowNo);               //�����ض����ַ�
            Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
            sdkDispBrushScreen();
            gOldPwdLen = len;
        }
    }
    else {
        Private_sdkDispClearRowRam(pScan->RowNo);               //�����ض����ַ�
        Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
        sdkDispBrushScreen();
    }
#else
    Private_sdkDispClearRowRam(pScan->RowNo);                   //�����ض����ַ�
    Verify(Private_sdkDispFillRowRamEx(pScan->RowNo, 0, disbuf, disp | align) == SDK_OK);
    sdkDispBrushScreen();
#endif /* if 0 */
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ������Ϣ��ʼ��������
   ��ڲ���:  ͨ������ṹpInput
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/

static s32 sdkMmiScanfINIT(SDK_KEY_SCANFMT *pScan, SDK_KEY_STROKEPOSGROUP *posgroup,
                           SDK_KEY_STROKEHZBUF *hzbuffer) {
    u8 align, disp;
    u8 len;
    s32 ret = 0;
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;


    //��ʼ��Ĭ��ֵ
    if (pScan->Mask == 0) { pScan->Mask = SDK_MMI_NUMBER; }

    if (pScan->RowNo == 0) { pScan->RowNo = 3; }

    if (pScan->MaxLen == 0) { pScan->MaxLen = SDK_MAX_STR_LEN; }

    if (pScan->MaxLen < pScan->MinLen) { pScan->MinLen = 0; }

    if (((pScan->Mask & SDK_MMI_MULTTASK) == SDK_MMI_MULTTASK)   //�������ϵ�ǩ��״̬��ֻ����������
        || ((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)      //��С������ʾ��ֻ����������
        || ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD)
        || ((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME))               //�������룬ֻ����������
    {
        pScan->Mask = pScan->Mask & (~SDK_MMI_HEX);

        if ((pScan->Mask & SDK_MMI_PWD) != SDK_MMI_PWD) //wanggaodeng delete ���������������ĸ
        {
            pScan->Mask = pScan->Mask & (~SDK_MMI_LETTER);
        }
        pScan->Mask = pScan->Mask & (~SDK_MMI_HZ);
        pScan->Mask = pScan->Mask & (~SDK_MMI_SYMBOL);
    }

    if ((pScan->Mask & SDK_MMI_HEX) ==
        SDK_MMI_HEX)                      //����ʮ������ʱֻ���������д��ĸ״̬
    {
        pScan->Mask = pScan->Mask | SDK_MMI_LETTER;
        pScan->Mask = pScan->Mask | SDK_MMI_NUMBER;
        pScan->Mask = pScan->Mask & (~SDK_MMI_HZ);
        pScan->Mask = pScan->Mask & (~SDK_MMI_SYMBOL);
    }

    if ((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT) {
        pScan->Mask = pScan->Mask & (~SDK_MMI_LEFT);
    }
    /*=======BEGIN: fusuipu 2013.12.13  15:16 modify Ŀǰ���뷨����֧�ֱʻ����뺺�֣�����У�һ����Ϊƴ������===========*/

    if ((pScan->Mask & SDK_MMI_LEFT) == SDK_MMI_LEFT) { align = SDK_DISP_FDISP; }
    else { align = SDK_DISP_RDISP; }

    if ((pScan->Mask & SDK_MMI_TIME) == SDK_MMI_TIME) { align = SDK_DISP_CDISP; }

    if ((pScan->Mask & SDK_MMI_DISP) == SDK_MMI_DISP) { disp = SDK_DISP_NOFDISP; }
    else { disp = SDK_DISP_FDISP; }

//    /*=======BEGIN: fusuipu 2013.12.09  11:0 modify===========*/
//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        sdkMmiMultiScanfINIT(pScan, pScanMem);
//    }
//    /*====================== END======================== */
    //��ʼ�����뷽ʽ
    if (0 == pScan->uiFistMode) {
        pScan->uiFistMode = pScan->Mask; //fusuipu 2014.04.01 20:28 �����ѡ������û�����ã�����Ĭ�ϵĽ��д���
    }
    else if (0 == (pScan->uiFistMode &
                   pScan->Mask)) //fusuipu 2014.04.01 20:29 �����ѡ���벻�ڵ�Ȼ���뷶Χ֮�ڣ�˵����������ֱ�ӷ���
    {
        return SDK_PARA_ERR;
    }

/*   //huangkanghui 2015.02.27 10:50
        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ &&
       (SDK_MMI_HZ & pScan->uiFistMode))                     //�������뺺��
    {
        pScanMem->MmiCurrentMode = MODE_T9;             //T9ƴ��
        memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
        memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);       //��ʾ����T9ƴ��
    }

    else if((pScan->Mask & SDK_MMI_STROKE) == SDK_MMI_STROKE &&
            (SDK_MMI_STROKE & pScan->uiFistMode))    //START
    {
        memset (posgroup, 0, sizeof(SDK_KEY_STROKEPOSGROUP));
        memset (hzbuffer, 0, sizeof(SDK_KEY_STROKEHZBUF));
        memset ((u8 *)&posgroup, 0, sizeof(STROKEPOSGROUP));
    }
 */
    //END
    //else if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER &&
    if ((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER &&
        (SDK_MMI_NUMBER & pScan->uiFistMode))                                    //������������
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;

        if (((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ) ||
            ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
            || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX) ||
            ((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
            || ((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)) {
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "123",
                               SDK_DISP_FDISP | SDK_DISP_RDISP);     //��ʾ��������
        }
    }
    else if ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER &&
             (SDK_MMI_LETTER & pScan->uiFistMode))                           //����������ĸ
    {
        pScanMem->MmiCurrentMode = MODE_BIG_LET;
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "ABC",
                           SDK_DISP_FDISP | SDK_DISP_RDISP);      //��ʾ������ĸ
    }
    else if ((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL &&
             (SDK_MMI_SYMBOL & pScan->uiFistMode)) {
        pScanMem->MmiCurrentMode = MODE_SYMBOL;
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "1:~ 2:! 3:@ 4:# 5:$ ",
                           SDK_DISP_FDISP | SDK_DISP_RDISP |
                           SDK_DISP_INCOL);     //fusuipu 2013.06.07 9:30
    }

        /*====================== END======================== */
    else                                                //��������
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        //    Private_sdkDispRow(SDK_DISP_LINE5, 0, "123", SDK_DISP_FDISP | SDK_DISP_RDISP);  //fusuipu 2013.06.07 9:30
    }
    sdkKbKeyFlush();                                        //������̻���
//    dev_ts_flush();//shijianglong 2012.12.18 10:49 K390������ʱ����
    sdkMmiClearKey(pScanMem);
    pScanMem->MmiIsInput = 0;                           //�������뷨״̬
    pScanMem->MmiIsLetterEnd = 0;
    pScanMem->MmiCurrentRow = 1;

    if (NULL != pScan->sdkMmiDispCallFun)    //����лص�����������ʾ���ֵ�Ȩ�����׽����ص���ʾ����
    {
        if (pScanMem->MmiCurrentMode == MODE_T9) {
            memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        ret = pScan->sdkMmiDispFun(pScan->Contents, pScan->Mask, pScan->RowNo, pScan->pVar);

        if (SDK_OK ==
            ret)                   //����ص���ʾ����ֵΪSDK_OK,˵���ص���ʾ��������������ص���������ʾ��������������
        {
            return ret;
        }
    }

    if ((pScan->Contents[0] & 0x80) == 0x80)             //�����г�ʼ������ʾ����
    {
        Private_sdkDispRowEx(pScan->RowNo, 0, &pScan->Contents[1], disp | align);
        memset(pScan->Contents, 0, sizeof(pScan->Contents));
    }
    else {
        len = pScan->Contents[0];
        memcpy(pScan->Contents, &pScan->Contents[1], len);
        pScan->Contents[len] = 0;

        if (pScanMem->MmiCurrentMode == MODE_T9) {
            memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        sdkMmiCountLine(pScan->Contents, 0, align, *pScanMem);
        sdkMmiChangeLine(0, pScan);

        if (strlen(pScan->Contents) == 0) {
            if ((pScan->Mask & SDK_MMI_TIME) ==
                SDK_MMI_TIME)                            //�Ƿ��������� �� С��
            {
                Private_sdkDispRowEx(pScan->RowNo, 0, SYS_TIME_FORMAT,
                                     disp | align);     //�����涨�ĸ�ʽ��ʾ��ʾ
            }
            else if ((pScan->Mask & SDK_MMI_POINT) ==
                     SDK_MMI_POINT)                     //�Ƿ��������� �� С��
            {
                Private_sdkDispRowEx(pScan->RowNo, 0, "0.00",
                                     disp | align);              //�����涨�ĸ�ʽ��ʾ��ʾ
            }
            else {
                Private_sdkDispRowEx(pScan->RowNo, 0, STR_CURSOR, disp | align);          //��ʾ���
            }
        }
        else                                                                    //ԭ���������ĳ��Ȳ��� 0
        {
            sdkMmiDisplayScanf(pScan);
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ǰ���뷨�л�Ϊ�ʻ�
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
//static void sdkMmiChangeModeToSTROKE(SDK_KEY_SCANMEMBER *pScanMem, SDK_KEY_STROKEPOSGROUP *posgroup, SDK_KEY_STROKEHZBUF *hzbuffer)   //START
//{
//    memset (posgroup, 0, sizeof(SDK_KEY_STROKEPOSGROUP));
//    memset (hzbuffer, 0, sizeof(SDK_KEY_STROKEHZBUF));
//    pScanMem->MmiCurrentMode = MODE_STROKE;      //Ŀǰ�Ǳʻ�
//    Private_sdkDispRow(SDK_DISP_LINE5, 0, "�ʻ�", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ����ʻ�
//}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ǰ���뷨�л�Ϊ��д��ĸ
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiChangeModeToBigLet(SDK_KEY_SCANMEMBER *pScanMem) {
    pScanMem->MmiCurrentMode = MODE_BIG_LET;      //Ŀǰ��T9 �л�����д��ĸ
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "ABC", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ������ĸ
}

#if 0
/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ǰ���뷨�л�Ϊ��λ��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiChangeModeToCodeD(SDK_KEY_SCANMEMBER *pScanMem)
{
    //memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
    pScanMem->MmiCurrentMode = MODE_CODE_D;
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "��λ��", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ������λ��
}

#endif

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���������������
   ��ڲ���:  ͨ������ṹpInput, ����key
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiSymbol(u8 key, SDK_KEY_SCANFMT *pScan) {
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;
    //  u8 i, dispbuf[SDK_MAX_LINE_CHAR+1] = {0}, temp[2] = {0}, temp2[5] = {0};
    u8 i, temp[2] = {0}, temp2[5] = {0};
    u8 dispbuf[128] = {0}; //shiweisong 2012.12.26 10:35  �ƸߵƷ��������д���

    for (i = 0; i < 5; i++) {
        if (Sdk_SpecSymbol[pScanMem->MmiKeyLen + i] == 0) {
            break;
        }
        sprintf(temp2, "%i:%c ", i + 1, Sdk_SpecSymbol[pScanMem->MmiKeyLen + i]);
        strcat(dispbuf, temp2);
        Trace("fusuipu", "strlen = %d \r\nline len = %d\r\n", strlen(dispbuf),
              Private_sdkDispLineMaxLen());
    }

    Private_sdkDispRow(SDK_DISP_LINE5, 0, dispbuf,
                       SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);    //�����ض����ַ�

    if (key) {
        if (key <= (i + 0x30) && key >= '1') {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            temp[0] = key;
            i = (u8) atoi(temp) - 1;
            pScan->Contents[strlen(pScan->Contents)] = Sdk_SpecSymbol[pScanMem->MmiKeyLen + i];
        }
        else if (key == SDK_KEY_DOWN) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            i = pScanMem->MmiKeyLen + 5;

            if (i >= strlen(Sdk_SpecSymbol)) {
                pScanMem->MmiKeyLen = 0;
            }
            else {
                pScanMem->MmiKeyLen = i;
            }
            sdkMmiSymbol(0, pScan);
        }
        else if (key == SDK_KEY_UP) {
            if (pScanMem->MmiKeyLen == 0) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                pScanMem->MmiKeyLen -= 5;
                sdkMmiSymbol(0, pScan);
            }
        }
        else {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
        }
        sdkMmiChangeLine(0, pScan);
        sdkMmiDisplayScanf(pScan);
    }
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ǰ���뷨�л�Ϊ�����ַ�
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiChangeModeToSym(SDK_KEY_SCANFMT *pScan) {
    pScan->ScanMem.MmiCurrentMode = MODE_SYMBOL;
    Private_sdkDispRow(SDK_DISP_LINE5, 0, " ", SDK_DISP_FDISP | SDK_DISP_RDISP);
    sdkMmiSymbol(0, pScan);
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ǰ���뷨�л�Ϊ����
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiChangeModeToNum(SDK_KEY_SCANMEMBER *pScanMem) {
    pScanMem->MmiCurrentMode = MODE_NUMBER;      //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "123", SDK_DISP_FDISP | SDK_DISP_RDISP);    //��ʾ����T9ƴ��
}

#if 0
/*****************************************************************************
** Descriptions:	�л����뷨����дģʽ
** Parameters:          SDK_KEY_SCANMEMBER *pScanMem
** Returned value:
** Created By:		fusuipu  2013.06.04
** Remarks:             ��дģʽ���ⲿ����֧�֣�����sdkKbGetScanf�����������л��Ľӿ�
*****************************************************************************/
static void sdkMmiChangeModeToHand(SDK_KEY_SCANMEMBER *pScanMem)
{
    pScanMem->MmiCurrentMode = MODE_HAND;
    Private_sdkDispRow(SDK_DISP_LINE5, 0, "", SDK_DISP_FDISP | SDK_DISP_RDISP);
}

#endif

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  �л����뷨
   ��ڲ���:  ͨ������ṹ
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static u8 sdkMmiChangeMode(SDK_KEY_SCANFMT *pScan, const SDK_KEY_STROKEPOSGROUP *posgroup,
                           const SDK_KEY_STROKEHZBUF *hzbuffer) {
#if 0
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;


    sdkMmiClearKey(pScanMem);

//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        if(true == sdkMmiMultiChangeMode(pScan, pScanMem))
//        {
//            return true;
//        }
//    }

    if (pScanMem->MmiCurrentMode == MODE_CODE_D)                                                        //�������������
    {
        /*=======BEGIN: fusuipu 2013.06.04  16:55 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        else
        {
            pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
            //memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
            memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
            memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
            memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);   //��ʾ����T9ƴ��
            return true;
        }
    }
    else if(pScanMem->MmiCurrentMode == MODE_HAND)
    {
        pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
        //memset (pScanMem->MmiKeyBuf, 0, sizeof(pScanMem->MmiKeyBuf));
        memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
        memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);       //��ʾ����T9ƴ��
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_T9)
    {
        Private_sdkDispClearRow(SDK_DISP_LINE4);
#if 0               //lilin �����αʻ�
        sdkMmiChangeModeToSTROKE(pScanMem, posgroup, hzbuffer);
        return true;

#endif

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  16:59 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        else
        {
            pScanMem->MmiCurrentMode = MODE_T9;                                                     //T9ƴ��
            memcpy (pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
            memset ((u8 *)&pScanMem->MmiPinYinGroup, 0, sizeof(SDK_KEY_PINYINGROUP));
            memset ((u8 *)&pScanMem->MmiDispBuf, 0, sizeof(SDK_KEY_DISPBUF));
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "T9ƴ��", SDK_DISP_FDISP | SDK_DISP_RDISP);   //��ʾ����T9ƴ��
            return true;
        }
        /*====================== END======================== */
        //sdkMmiChangeModeToCodeD(pScanMem);//START
        //return TRUE;//END
    }
    else if(pScanMem->MmiCurrentMode == MODE_STROKE)
    {
        Private_sdkDispClearRow(SDK_DISP_LINE4);

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:0 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        sdkMmiChangeModeToCodeD(pScanMem);        //END
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_BIG_LET)
    {
        if((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)
        {
            /*=======BEGIN: fusuipu 2013.06.05  17:17 modify===========*/
            if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
            {
                sdkMmiChangeModeToHand(pScanMem);
                return true;
            }
            /*====================== END======================== */
            sdkMmiChangeModeToNum(pScanMem);
        }

        else if ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            pScanMem->MmiCurrentMode = MODE_SMA_LET;              //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
            Private_sdkDispRow(SDK_DISP_LINE5, 0, "abc", SDK_DISP_FDISP | SDK_DISP_RDISP);     //��ʾ����T9ƴ��
        }
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_SMA_LET)     //Сд��ĸ
    {
        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
        sdkMmiChangeModeToBigLet(pScanMem);
        return true;
    }
    else if(pScanMem->MmiCurrentMode == MODE_NUMBER)
    {
        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        if(((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
           || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX))
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)
        {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
    }
    else if(pScanMem->MmiCurrentMode == MODE_SYMBOL)     //�����ַ�
    {
        if((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER)
        {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_HZ) == SDK_MMI_HZ)
        {
            sdkMmiChangeModeToCodeD(pScanMem);
            return true;
        }

        if((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
        {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        /*=======BEGIN: fusuipu 2013.06.04  17:2 modify===========*/
        if((pScan->Mask & SDK_MMI_HAND) == SDK_MMI_HAND)
        {
            sdkMmiChangeModeToHand(pScanMem);
            return true;
        }
        /*====================== END======================== */
    }
    else                                                                                        //���������ƴ��
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        return true;
    }
    return false;

#endif

#if 1
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    sdkMmiClearKey(pScanMem);

    if (pScanMem->MmiCurrentMode == MODE_BIG_LET)         //��д��ĸ
    {
        pScanMem->MmiCurrentMode = MODE_SMA_LET;                  //Ŀǰ�Ǵ�д��ĸ �л���Сд��ĸ
        Private_sdkDispRow(SDK_DISP_LINE5, 0, "abc", SDK_DISP_FDISP | SDK_DISP_RDISP);

        return true;
    }
    else if (pScanMem->MmiCurrentMode == MODE_SMA_LET)            //Сд��ĸ
    {
        if ((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL) {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }

        if ((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER) {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }
        sdkMmiChangeModeToBigLet(pScanMem);
        return true;
    }
    else if (pScanMem->MmiCurrentMode == MODE_NUMBER)            //����
    {
        if (((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER)
            || ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)) {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }

        if ((pScan->Mask & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL) {
            sdkMmiChangeModeToSym(pScan);
            return true;
        }
    }
    else if (pScanMem->MmiCurrentMode == MODE_SYMBOL)         //�����ַ�
    {
        if ((pScan->Mask & SDK_MMI_NUMBER) == SDK_MMI_NUMBER) {
            sdkMmiChangeModeToNum(pScanMem);
            return true;
        }

        if ((pScan->Mask & SDK_MMI_LETTER) == SDK_MMI_LETTER) {
            sdkMmiChangeModeToBigLet(pScanMem);
            return true;
        }
    }
    else                                                                                            //���������ƴ��
    {
        pScanMem->MmiCurrentMode = MODE_NUMBER;
        return true;
    }
    return false;

#endif
}

#if 0
/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����"00"��
   ��ڲ���:  ͨ������ṹ
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static u8 sdkMmiDoubleZero(SDK_KEY_SCANFMT *pScan)
{
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    if((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT)    //������ǽ��
    {
        if(atoi(pScan->Contents) == 0)         //���Ϊ0
        {
            return false;
        }
        else if((u8)strlen(pScan->Contents) <= pScan->MaxLen - 2)
        {
            pScan->Contents[strlen(pScan->Contents)] = '0';
            pScan->Contents[strlen(pScan->Contents)] = '0';
            return true;
        }
        else        ////����˫0�󳤶Ȳ��Ϸ��� ��һ��0ʹ��
        {
            pScan->Contents[strlen(pScan->Contents)] = '0';
            return true;
        }
    }
    else if((pScanMem->MmiCurrentMode == MODE_BIG_LET) || (pScanMem->MmiCurrentMode == MODE_SMA_LET))
    {
        if((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)        //cwDebug 2009-06-23
        {
            return false;
        }
        else if(strlen(pScan->Contents) >= pScan->MaxLen)
        {
            pScanMem->MmiIsLetterEnd = 1;
            return false;
        }
        else
        {
            pScan->Contents[strlen(pScan->Contents)] = '#';
            return true;
        }
    }
    else     //�����������
    {
        return false;
    }
}

#endif

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ����ɾ�������ж����һ���Ǻ��ֻ��������ַ�
   ��ڲ���: ��ɾ���ַ���
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
void sdkMmiDelLastChar(u8 *str) {
    s32 len, i;

    len = strlen(str);
    i = 0;

    if (len > 0) {
        while (1) {
            if (str[i] > 0x80)                   //�����ַ�
            {
                i += 2;

                if (i >= len)                    //���һ���Ǻ���
                {
                    str[len - 1] = 0;
                    str[len - 2] = 0;
                    return;
                }
            }
            else {
                i++;

                if (i >= len)                    //���һ�����Ǻ���
                {
                    str[len - 1] = 0;
                    return;
                }
            }
        }
    }
    else { return; }
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����ɾ����
   ��ڲ���:  ͨ������ṹ
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
u8 sdkMmiDelete(SDK_KEY_SCANFMT *pScan) {
    u8 len = (u8) strlen(pScan->Contents);

    pScan->ScanMem.MmiLastKey = 0;
    pScan->ScanMem.MmiKeyTimes = 0;
    pScan->ScanMem.MmiIsLetterEnd = 0;

//    if(true == SDK_MMI_IS_MULTI(pScan->Mask))
//    {
//        return sdkMmiMultiDelete(pScan);
//    }
    if (len > 0) {
        if (len > 1) {
            sdkMmiDelLastChar(pScan->Contents);
        }
        else {
            pScan->Contents[0] = 0;
        }

        if (pScan->ScanMem.MmiCurrentMode == MODE_T9) {
            memset(pScan->ScanMem.MmiKeyBuf, 0, sizeof(pScan->ScanMem.MmiKeyBuf));
            memcpy(pScan->ScanMem.MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
        }
        pScan->ScanMem.MmiIsLetterEnd = 0;
        return true;
    }
    else {
        return false;
    }
}

/*******************************************************************
   ��	 ��: ������
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����������ּ��ŵ����̻������ȥ
   ��ڲ���: key	����ļ�ֵ
             keybuf	���̻�����
             keynum	�������Ч������
             flag	0 ��������Ѽ��̻���ȫ�����,
                    1 ֻ������һ��
                    2 �������ʱ�򲻽�
                    maxlen ������
   �� �� ֵ: ����һ���µ���ĸ����TRUE,���򷵻�FALSE
   ��	 ע:
 ********************************************************************/
void sdkMmiGetNumericKey(const u8 key, u8 *keybuf, u8 *keynum, u8 flag, u8 maxlen,
                         SDK_KEY_SCANFMT *pScan) {
    u8 i;

    if ((key < '0') || (key > '9')) {
        if (key ==
            SDK_KEY_BACKSPACE)                                                                   //�����
        {
            if ((*keynum == 0) && (flag < 2)) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_OK);

                if (flag ==
                    0)                                                                                  //�����������ȫ�����
                {
                    memset(keybuf, 0, sizeof(keybuf));
                    (*keynum) = 0;
                }
                else if (*keynum != 0) {
                    (*keynum)--;
                    keybuf[*keynum] = 0;
                }
            }
        }
        else if (key == SDK_KEY_CLEAR) {
            if (*keynum == 0) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                sdkMmiClearKey(&pScan->ScanMem);
            }
        }
        else if ((key != SDK_KEY_ENTER) && (key != SDK_KEY_ESC)) {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
            //������ʾ
        }
    }
    else {
        if ((*keynum) ==
            maxlen)                                                        //�Ѿ���������
        {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
        }
        else {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            i = *keynum;
            keybuf[i] = key;
            (*keynum)++;
        }
    }

    if (*keynum == 0) {
        memcpy(keybuf, STR_CURSOR, sizeof(u8));
    }
}

/*******************************************************************
   ��	 ��: ������
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����������ּ�ת����Ϊ��ĸ��
   ��ڲ���: key����ļ�ֵ
             bigif 0��д	1Сд
             newkey ת�������ĸ��������
   �� �� ֵ: ������ܸ����ǲ��Ǽ�1, 0 �� 1��
   ��	 ע: �����޸�
 ********************************************************************/
static s32 sdkMmiKeyConver(u8 keyvalue, u8 *newkey, u8 Mode, SDK_KEY_SCANFMT *pScan) {
    u8 temp[3] = {0};                   //fusuipu 2013.04.11 16:37
    u8 rlt = 0, i;
    u8 offset = 0;
    u8 key = keyvalue;
    u8 keytabletter[MULKEYNUM][7]; //fusuipu 2013.08.08 17:55 ��ĸ��ÿ���������7���ַ��͹���
    u8 keytab[MULKEYNUM][SDK_KEY_TAB_MAX_LEN];
    s32 tab_len = 0;

    memset(keytabletter, 0, sizeof(keytabletter));
    memset(keytab, 0, sizeof(keytab));  //fusuipu 2013.01.08 14:27

    sdkSysGetSysKeyPad(keytabletter, Mode);

    for (i = 0; i < MULKEYNUM; i++) {
        strcpy(keytab[i], keytabletter[i]);
    }


    if (key == '0') {
        key = 0x3a;
    }

    if (((key ==
          pScan->ScanMem.MmiLastKey))                                                            //���ϴ��ǰ���ͬһ����
        && (sdkTimerIsEnd(Sdk_timerID, 800) != 1)) {
        tab_len = strlen(keytab[key - 0x31]);


        if (0 == tab_len) {
            return SDK_ERR;
        }
        offset = (u8)(pScan->ScanMem.MmiKeyTimes % tab_len);       //ƫ����
        /*=======BEGIN: fusuipu 2013.04.11  16:37 modify===========*/
        temp[0] = keytab[key - 0x31][offset];
        //pVTable->OpenTimerAbc(TRUE);									        //���¿���ʱ��,�൱�ڶ�ʱ������
        Sdk_timerID = sdkTimerGetId();
        pScan->ScanMem.MmiKeyTimes++;
        rlt = 1;
    }
    else {
        pScan->ScanMem.MmiLastKey = key;
        temp[0] = keytab[key - 0x31][offset];
        pScan->ScanMem.MmiKeyTimes = 1;
        //pVTable->OpenTimerAbc(TRUE);					//���¿���ʱ��,�൱�ڶ�ʱ������
        Sdk_timerID = sdkTimerGetId();
        rlt = 0;
    }
    memcpy(newkey, temp, sizeof(temp));                 //fusuipu 2013.04.11 16:36
    //*newkey = temp;
    return rlt;
}

/*******************************************************************
   ��	 ��: ������
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����������ּ�ת����Ϊ��ĸ��
   ��ڲ���: key	����ļ�ֵ
             keybuf	���̻�����
             keynum	�������Ч������
             flag	0 ��������Ѽ��̻���ȫ�����, 1 ֻ������һ��
             maxlen ������
             bigif	��Сд���� 0��д 1 Сд, 2 �绰�����
             beep   0 ����, 1 ��
   �� �� ֵ: ����һ���µ���ĸ����TRUE,���򷵻�FALSE
   ��	 ע: �����޸�
 ********************************************************************/
static s32 sdkMmiGetLetterKey(const u8 key, u8 *keybuf, u8 *keynum, u8 flag, u8 maxlen, u8 Mode,
                              u8 beep, SDK_KEY_SCANFMT *pScan) {
    s32 m = 0;
    u8 rlt = false;
    u8 str_buf[3] = {0};

    memset(str_buf, 0, sizeof(str_buf));

    if (((key < '0') || (key > '9'))
        && (key != SDK_KEY_UP) && (key != SDK_KEY_DOWN)) {
        if (key ==
            SDK_KEY_BACKSPACE)                                                                                           //�����
        {
            if ((*keynum == 0) && (beep == 1)) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
            else if (*keynum > 0) {
                if (beep == 1) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                }

                if (flag ==
                    0)                                                                                  //�����������ȫ�����
                {
                    sdkMmiClearKey(&pScan->ScanMem);
                }
                else {
                    (*keynum)--;
                    keybuf[*keynum] = 0;
                }
                pScan->ScanMem.MmiLastKey = 0;
                pScan->ScanMem.MmiKeyTimes = 0;
                pScan->ScanMem.MmiIsLetterEnd = 0;
                //			pVTable->OpenTimerAbc(FALSE);
            }
        }
        else if ((key != SDK_KEY_ENTER) && (key != SDK_KEY_ESC) && (beep == 1)) {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
        }
    }
    else {
        if ((*keynum) > maxlen) {
            if (beep == 1) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
        else {
            if (beep == 1) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
            }
            m = sdkMmiKeyConver(key, str_buf, Mode, pScan);

            if (m == 0) {
                keybuf[*keynum] = str_buf[0];
                (*keynum)++;
                rlt = true;
            }
            else if (*keynum != 0) {
                keybuf[(*keynum) - 1] = str_buf[0];
            }
        }
    }

    if (*keynum == 0) {
        memset(keybuf, 0, SDK_KEY_BUF_SIZE);
        pScan->ScanMem.MmiLastKey = 0;
        pScan->ScanMem.MmiKeyTimes = 0;
        pScan->ScanMem.MmiIsLetterEnd = 0;
    }
    return rlt;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ����������ĸ
   ��ڲ���:  ͨ������ṹpInput, ����key
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiLetter(u8 key, SDK_KEY_SCANFMT *pScan) {
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    memset(pScanMem->MmiKeyBuf, 0, SDK_KEY_BUF_SIZE);
    memcpy(pScanMem->MmiKeyBuf, pScan->Contents, strlen(pScan->Contents));
    pScanMem->MmiKeyLen = (u8) strlen(pScan->Contents);


    sdkMmiGetLetterKey(key, pScanMem->MmiKeyBuf, &pScanMem->MmiKeyLen, 0, pScan->MaxLen,
                       pScanMem->MmiCurrentMode, 0, pScan);         //��˹��


    /*=======BEGIN: fusuipu 2013.04.14  19:51 modify===========*/
    if (pScanMem->MmiKeyLen <= pScan->MaxLen) {
        sdkSysBeep(SDK_SYS_BEEP_OK);
        memcpy(pScan->Contents, pScanMem->MmiKeyBuf, pScanMem->MmiKeyLen);
    }
        /*====================== END======================== */
    else {
        sdkSysBeep(SDK_SYS_BEEP_ERR);
        pScanMem->MmiIsLetterEnd = 1;
    }
    sdkMmiChangeLine(0, pScan);
    sdkMmiDisplayScanf(pScan);
}

#if 0
/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ������λ������״̬�����뺺��
   ��ڲ���:  ͨ������ṹSCANFMT, ����key
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static void sdkMmiCodeD(u8 key, SDK_KEY_SCANFMT *pScan)
{
    u8 buf[5], temp[3];
    SDK_KEY_SCANMEMBER *pScanMem = &pScan->ScanMem;

    sdkMmiGetNumericKey (key, pScanMem->MmiKeyBuf, &pScanMem->MmiKeyLen, 2, 4, pScan);
    Private_sdkDispClearRowRamAt(SDK_DISP_LINE5, 0, (sdkDispGetScreenPixel().siX / 2));
    Private_sdkDispRow(SDK_DISP_LINE5,  0, "��λ��", SDK_DISP_FDISP | SDK_DISP_RDISP);

    if (pScanMem->MmiKeyLen == 0)
    {
        pScanMem->MmiIsInput = 0;
        Verify(Private_sdkDispFillRowRam(SDK_DISP_LINE5, 0, "_",  SDK_DISP_FDISP | SDK_DISP_LDISP) == SDK_OK);
    }
    else
    {
        pScanMem->MmiIsInput = 1;
        TraceHex("fusuipu", "codeD:", pScanMem->MmiKeyBuf, strlen(pScanMem->MmiKeyBuf));
        Verify(Private_sdkDispFillRowRam(SDK_DISP_LINE5, 0, pScanMem->MmiKeyBuf, SDK_DISP_FDISP | SDK_DISP_LDISP) == SDK_OK);   // fusuipu added Ϊ�������д���룬��ԭ���ķ����ȸ�Ϊ����ʾ
    }

    if (key == SDK_KEY_ENTER)                                                                   //����ȷ����ƴ��
    {
        if (pScanMem->MmiKeyLen < 4)
        {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
            Private_sdkDispClearRowRamAt(SDK_DISP_LINE5, 0, (sdkDispGetScreenPixel().siX / 2));
            Private_sdkDispRow(SDK_DISP_LINE5,  0, "��������4λ", SDK_DISP_FDISP | SDK_DISP_RDISP);
            pScanMem->MmiIsInput = 0;
        }
        else if(strlen(pScan->Contents) + 1 >= pScan->MaxLen)      //�Է���һ
        {
            sdkSysBeep(SDK_SYS_BEEP_ERR);
            pScanMem->MmiIsInput = 0;
        }
        else
        {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            memset (temp, 0, sizeof(temp));
            memcpy (buf, pScanMem->MmiKeyBuf, 2);
            buf[2] = 0;
            temp[0] = (u8)atoi(buf) + 0xa0;
            memcpy (buf, pScanMem->MmiKeyBuf + 2, 2);
            buf[2] = 0;
            temp[1] = (u8)atoi(buf) + 0xa0;

            if ((temp[0] > 0xa0) && (temp[1] > 0xa0)
                && (temp[0] < 0xf8) && (temp[1] < 0xff))
            {
                /*=======BEGIN: fusuipu 2013.06.09  9:45 modify ���ɼ�����ֱ���˳�===========*/
                if(false == sdkIsVisibleStr(temp, 2))
                {
                    memset(temp, 0, 2);
                    return;
                }
                /*====================== END======================== */
                strcat (pScan->Contents, temp);
                sdkMmiChangeLine(0, pScan);
                sdkMmiDisplayScanf(pScan);
                Private_sdkDispClearRowRamAt(SDK_DISP_LINE5, 0, (sdkDispGetScreenPixel().siX  / 2));
                Verify(Private_sdkDispFillRowRam(SDK_DISP_LINE5,  0, "��λ��", SDK_DISP_FDISP | SDK_DISP_RDISP) == SDK_OK);
                Verify(Private_sdkDispFillRowRam(SDK_DISP_LINE5,  0, "_", SDK_DISP_FDISP | SDK_DISP_LDISP) == SDK_OK);
                pScanMem->MmiIsInput = 0;
            }
        }
        sdkMmiClearKey(pScanMem);
    }
    sdkDispBrushScreen();
}

#endif

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���봦��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static s32 sdkConverScanf(const u32 overtime, SDK_KEY_SCANFMT *pScan) {
    //static SDK_KEY_STROKEPOSGROUP posgroup;//huangkanghui 2015.02.27 10:48
    //static SDK_KEY_STROKEHZBUF hzbuffer;        //huangkanghui 2015.02.27 10:48
    u8 key, flag;
    u16 tempflag;
    u32 timerID;
    s32 ret = 0;

    //u8 i = 0;
    //u8 tmpkeytab[32] = {0};

    // ��������Ϸ����ж�
    if (NULL == pScan) {
        Assert(0);
        return SDK_ERR;
    }
    //ret = sdkMmiScanfINIT(pScan, &posgroup, &hzbuffer);
    ret = sdkMmiScanfINIT(pScan, NULL, NULL);     //huangkanghui 2015.02.27 10:51

    if (SDK_OK != ret) {
        return ret;
    }
    timerID = sdkTimerGetId();

    while (1) {
        tempflag = 0;
        key = (u8) sdkKbGetKey(); //huacong 2013.04.07 9:50 ��ʯγ��Ҫ�󽫰���ֵ�޸�ΪUpkey
        //key = Private_sdkGetKeyValue();

        if ((sdkTimerIsEnd(Sdk_timerID, 800) == 1) &&
            ((pScan->Mask & SDK_MMI_PWD) == SDK_MMI_PWD) &&
            (pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET ||
             pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET) &&
            (NULL ==
             pScan->sdkMmiDispCallFun)) //fusuipu 2014.04.01 20:59 ����лص���������ʾ��ʽ��ȫ�ɿ����߿���
        {
            sdkMmiDisplayPwdAgain(pScan);                           //wanggaodeng add
        }

        if (key > 0) {
            tempflag = 1;
#if 0 /*Modify by huacong at 2013.04.07  9:52 */
            Trace("sdkmmi", "tmpkey=%02x\r\n", key);

            for(i = 0; i < 32; i++)
            {
                if(key == tmpkeytab[i])
                {
                    tempflag = 1;
                    break;
                }
            }

#endif /* if 0 */
        }

        if (overtime != 0) //shiweisong 2012.12.21 10:35 ���ʱ��Ϊ0�Ͳ���ʱ
        {
            /*=======BEGIN: ����20121106 modify===========*/
            if (sdkTimerIsEnd(timerID, overtime))   //��ʱ���س�ʱ
            {
                sdkMmiClearKey(&pScan->ScanMem);
                memset(pScan->Contents, 0, sizeof(pScan->Contents));
                return SDK_TIME_OUT;
            }
            /*================ END================== */
        }

        if (key == SDK_KEY_ESC || //huacong 2013.04.07 9:53
            (SDK_SYS_MACHINE_K501AM == sdkSysGetMachineCode(NULL) &&
             key == SDK_KEY_F3)) //fusuipu 2013.04.15 11:2 ��K501AM����Ӧ�˳���
        {
            sdkSysBeep(SDK_SYS_BEEP_OK);
            sdkMmiClearKey(&pScan->ScanMem);
            memset(pScan->Contents, 0, sizeof(pScan->Contents));
            return key;
        }

        if (tempflag > 0) {
            timerID = sdkTimerGetId(); //���� 20121106

            if (((key == SDK_KEY_UP) || (key == SDK_KEY_DOWN)) && (!pScan->ScanMem.MmiIsInput)) {
                if (sdkSysIsMultiplexUpDown())               //�Ƿ������¼�
                {
                    if ((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) ||
                        (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET)) {
                        if ((strlen(pScan->Contents) >= pScan->MaxLen) ||
                            ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX)) {
                            continue;
                        }
                        else {
                            if (key == SDK_KEY_UP) {
                                pScan->Contents[strlen(pScan->Contents)] = '*';
                            }
                            else {
                                pScan->Contents[strlen(pScan->Contents)] = '#';
                            }
                            sdkSysBeep(SDK_SYS_BEEP_OK);
                            sdkMmiDisplayScanf(pScan);
                            continue;
                        }
                    }
                }

                if (pScan->ScanMem.MmiCurrentMode != MODE_SYMBOL) {
                    if (key == SDK_KEY_UP) {
                        flag = sdkMmiChangeLine(-1, pScan);
                    }
                    else {
                        flag = sdkMmiChangeLine(1, pScan);
                    }

//                    sdkSysBeep(flag);//shijianglong 2013.03.26 15:34
                    if (flag == true) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                    sdkMmiDisplayScanf(pScan);
                    continue;
                }
            }

            if (key == SDK_KEY_FUNCTION || key == SDK_KEY_ALPHA || key == SDK_KEY_MULTITASK) {
                if (((pScan->Mask & SDK_MMI_MULTTASK) == SDK_MMI_MULTTASK) &&
                    ((key == SDK_KEY_FUNCTION || key == SDK_KEY_MULTITASK))) {
                    //memset(pScan->Contents , 0 , sizeof(pScan->Contents));
                    sdkSysBeep(SDK_SYS_BEEP_OK);

                    if (key == SDK_KEY_FUNCTION) {
                        return SDK_KEY_FUNCTION;
                    }
                    else {
                        return SDK_KEY_MULTITASK;
                    }
                }
                else {
                    if (key == SDK_KEY_FUNCTION || key == SDK_KEY_ALPHA) {
                        pScan->ScanMem.MmiIsInput = 0;
                        //flag = sdkMmiChangeMode(pScan, &posgroup, &hzbuffer);
                        flag = sdkMmiChangeMode(pScan, NULL,
                                                NULL);                        //huangkanghui 2015.02.27 10:48

//                        sdkSysBeep(flag);//shijianglong 2013.03.26 15:36
                        if (flag == true) {
                            sdkSysBeep(SDK_SYS_BEEP_OK);
                        }
                        else {
                            sdkSysBeep(SDK_SYS_BEEP_ERR);
                        }
                        sdkMmiChangeLine(0, pScan);
                        sdkMmiDisplayScanf(pScan);

                        /*=======BEGIN: fusuipu 2013.06.04  17:5 modify===========*/
                        if (MODE_HAND == pScan->ScanMem.MmiCurrentMode) {
                            return MODE_HAND;
                        }
                        /*====================== END======================== */
                    }
                    else {
                        sdkSysBeep(SDK_SYS_BEEP_ERR);
                    }
                    continue;
                }
            }

            if ((key == SDK_KEY_CLEAR) && (!pScan->ScanMem.MmiIsInput)) {
//#if (MACHINEMODEL == SDK_SYS_MACHINE_G2  ||MACHINEMODEL == SDK_SYS_MACHINE_200P)  /*Modify by wangmingming at 2014.06.04  10:28 */
                //����������˸��ʹ��
                flag = sdkMmiDelete(pScan);

                if (flag == true) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
                sdkMmiChangeLine(0, pScan);
                sdkMmiDisplayScanf(pScan);
                continue;
            }

            if ((key == SDK_KEY_BACKSPACE) && (!pScan->ScanMem.MmiIsInput)) {
                flag = sdkMmiDelete(pScan);

                if (flag == true) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
                sdkMmiChangeLine(0, pScan);
                sdkMmiDisplayScanf(pScan);
                continue;
            }

            if ((key == SDK_KEY_ENTER) && (!pScan->ScanMem.MmiIsInput)) {
                if (strlen(pScan->Contents) == 0) {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);

                    if (pScan->MinLen == 0 ||
                        ((pScan->Mask & SDK_MMI_NOINPUT_QUIT) == SDK_MMI_NOINPUT_QUIT)) {
                        return SDK_KEY_ENTER;
                    }
                    continue;
                }
                else if (strlen(pScan->Contents) < pScan->MinLen) {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                    continue;
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_OK); //shijianglong 2013.03.26 15:43
                    return SDK_KEY_ENTER;
                }
            }

            if (strlen(pScan->Contents) >= pScan->MaxLen) {
                /*=======BEGIN: ����20120727 modify===========*/
                if (pScan->MaxLen == 1 && pScan->MinLen == 1 &&
                    pScan->ScanMem.MmiCurrentMode == MODE_NUMBER) {
                    if (key >= '0' && key <= '9') {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        sdkMmiDelete(pScan);
                        pScan->Contents[strlen(pScan->Contents)] = key;
                        sdkMmiChangeLine(0, pScan);
                        sdkMmiDisplayScanf(pScan);
                        continue;
                    }
                }
                /*================ END================== */

                if (!(((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) ||
                       (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET))
                      && (!pScan->ScanMem.MmiIsLetterEnd))) {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                    continue;
                }
            }

            if ((strlen(pScan->Contents) + 1 >= pScan->MaxLen)
                && ((pScan->ScanMem.MmiCurrentMode == MODE_CODE_D) ||
                    (pScan->ScanMem.MmiCurrentMode == MODE_T9))) {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
                continue;
            }

            if ((strlen(pScan->Contents) == 0) && (key == '0')) {
                if ((pScan->Mask & SDK_MMI_POINT) == SDK_MMI_POINT) {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                    continue;
                }
            }

            if (pScan->ScanMem.MmiCurrentMode == MODE_SYMBOL) {
                sdkMmiSymbol(key, pScan);
                continue;
            }
            else if ((pScan->ScanMem.MmiCurrentMode == MODE_BIG_LET) ||
                     (pScan->ScanMem.MmiCurrentMode == MODE_SMA_LET)) {
                if ((pScan->Mask & SDK_MMI_HEX) == SDK_MMI_HEX) {
                    if (((key >= '4') && (key <= '9')) || (key == '1') ||
                        (key == '0'))          //cwDebug 2009-06-23
                    {
                        if (strlen(pScan->Contents) >= pScan->MaxLen) {
                            pScan->ScanMem.MmiIsLetterEnd = 1;
                            sdkSysBeep(SDK_SYS_BEEP_ERR);
                        }
                        else {
                            sdkSysBeep(SDK_SYS_BEEP_OK);
                            pScan->Contents[strlen(pScan->Contents)] = key;
                            sdkMmiChangeLine(0, pScan);
                            sdkMmiDisplayScanf(pScan);
                        }
                        continue;
                    }
                }
                sdkMmiLetter(key, pScan);
            }
            else    // if(pScan->ScanMem.MmiCurrentMode == MODE_NUMBER)
            {
                if (key >= '0' && key <= '9') {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    pScan->Contents[strlen(pScan->Contents)] = key;
                    sdkMmiDisplayScanf(pScan);
                    continue;
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                    continue;
                }
            }
        }
    }
}

#if 0
/*****************************************************************************
** Descriptions:	��̬���ض�����Ժ������
** Parameters:          siOvertime
                               pScan
** Returned value:
** Created By:		fusuipu  2013.12.29
** Remarks:
*****************************************************************************/
static s32 sdkMmiMultiConver(const u32 overtime, SDK_KEY_SCANFMT *pScan)
{
    static s32 (*p_fun)(const u32 overtime, SDK_KEY_SCANFMT *pScan) = NULL;

#if 0 /*Modify by ���ƽ at 2015.01.19  15:32 */

    if(p_fun == NULL)
    {
        p_fun =  (s32 (*)(const u32 overtime, SDK_KEY_SCANFMT *pScan))sdk_dev_get_multilib_fun_hand("sdkMmiMultiConverScanf"); //lint !e611
    }
#endif /* if 0 */

    if(p_fun != NULL)
    {
        return p_fun(overtime, pScan);
    }
    else
    {
        Trace("fsp", "warming: sdkMmiMultiConverScanf");
        return SDK_ERR;
    }
}

#endif

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���봦��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkKbGetScanf(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen,
                  const u32 uiMode, const s32 siDispRow) {
    return sdkKbGetScanfEx(siOvertime, pheOut, siMinLen, siMaxLen, uiMode, siDispRow, NULL, NULL,
                           0);
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ���봦��
   ��ڲ���:
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkKbGetScanfEx(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen,
                    const u32 uiMode, const s32 siDispRow,
                    s32 (*psdkCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow,
                                       void *pVar), void *pVarible, u32 siFirstMode) {
    SDK_KEY_SCANFMT *pScan;

    s32 rslt = 0;

    //Ŀǰ��������ڳ�ʼ��ʱ��Ĭ�����ã�������뷨�Ժ���Ҫ֧���ڶ��ֶ������֮������л���
    //�����л�֮ǰ��Ҫ��������������Ϣ�����˳����뷨֮�󣬻ָ�֮ǰ������������Ϣ

    s32 lang, line;

    //��������ж� ʯ���� 2012.08.20 10:46
    if (pheOut == NULL || siMinLen > SDK_MAX_STR_LEN || siMaxLen > SDK_MAX_STR_LEN ||
        siMinLen > siMaxLen || (!(siDispRow < SDK_DISP_LINE_MAX)) || 0 == siMaxLen) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    /*=======BEGIN: wangmingming 2014.07.11  14:53 modify===========*/
    //�����뷨Ϊ��������ʱ������ʹ�����һ����Ϊ�����ʾ�У�ֱ�ӷ��ز�������
    //��Ϊ��������ʱ�����һ����Ϊ��ѡ������ʾ��
    if ((SDK_DISP_LINE5 == siDispRow)) {
        Trace("test", ">>> uiMode=%d, siFirstMode=%d\r\n", uiMode, siFirstMode);

        if (((uiMode & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL) ||
            ((siFirstMode & SDK_MMI_SYMBOL) == SDK_MMI_SYMBOL)) {
            Assert(0);
            return SDK_PARA_ERR;
        }
    }
    /*====================== END======================== */

    pScan = (SDK_KEY_SCANFMT *) sdkGetMem(sizeof(SDK_KEY_SCANFMT));         //�����ڴ�

    if (pScan == NULL) //ʯ���� 2012.08.20 10:46
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(pScan, 0, sizeof(SDK_KEY_SCANFMT));


    //ȥ������һ��0x80 �������������ʾ��Ϣ�Ķ��� shiweisong 2013.02.22 14:20
    if (pheOut[0] > siMaxLen) //shijianglong 2013.02.22 10:4�������ߵƽ������Ӵ��ж�
    {
        Assert(0);
        sdkFreeMem(pScan);
        return SDK_PARA_ERR;
    }
    //���ȫ�ֱ�����ʹ���Ƿ�ֹG2&K200P�������ּ�ʱ(��ˢ��)����ˢ������������
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P)

    if ((uiMode & SDK_MMI_PWD) == SDK_MMI_PWD) {
        gOldPwdLen = 0;
    }
#endif


    /*���pheoutΪ0����δ���*/
    pScan->Contents[0] = pheOut[0];
    memcpy(&pScan->Contents[1], &pheOut[1], pheOut[0]);

    pScan->MinLen = (u8) siMinLen;
    pScan->MaxLen = (u8) siMaxLen;
    pScan->Mask = uiMode;
    pScan->RowNo = (u8) siDispRow;
    pScan->uiFistMode = siFirstMode;
    pScan->sdkMmiDispCallFun = psdkCallFun;
    pScan->pVar = pVarible;
    /*=======BEGIN: fusuipu 2013.12.15  17:27 modify ����ϵͳ����������Ϣ===========*/
    lang = sdkSysGetLang();
    line = sdkSysGetSreenMaxLine();
    /*====================== END======================== */

    if (true == sdkIsMultiLang()) {
        Trace("fsp", "reay sdkMmiMultiConver");
        //rslt = sdkMmiMultiConver((u32)siOvertime, pScan);
        rslt = SDK_ERR;
    }
    else {
        rslt = sdkConverScanf((u32) siOvertime, pScan);
    }

    if (rslt == SDK_KEY_ENTER || rslt == SDK_KEY_FUNCTION || MODE_HAND == rslt) {
        memset(&pheOut[1], 0, pheOut[0]); //tjb 2014.06.04 9:29 ��pheOut�г�ֵʱ���������
        pheOut[0] = (u8) strlen(pScan->Contents);
        memcpy(&pheOut[1], pScan->Contents, pheOut[0]);
    }
    sdkFreeMem(pScan);

    /*=======BEGIN: fusuipu 2013.12.15  17:28 modify �ָ�ϵͳ����������Ϣ===========*/
    sdkSysSetLang(lang);
    sdkDispSetMaxRow(line);
    /*====================== END======================== */

    return rslt;
}

