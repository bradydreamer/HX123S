#include "sdkGlobal.h"
#include "SdkTest.h"
//#include "sdkRecord.h"
#include "sdkiso8583.h"

//һ�����Թ�����ʧ�ܵ��ܴ���
static s32 mFailedConter = 0;

//һ�����Թ����гɹ����ܴ���
static s32 mOkConter = 0;


static SDK_SYS_LIST_HAND g_TestTraceListHand = NULL;  //Test Trace ��������


/*****************************************************************************
** Descriptions:	    TestTrace�ĳ�ʼ��
** Parameters:    	void
** Returned value:	
** Created By:		������  2013.04.07
** Remarks: 		�ϵ��ʼ���ú���,��ֻ�ܵ���һ��
*****************************************************************************/
void sdkTestTraceInit(void) {
    if (g_TestTraceListHand == NULL) {
        g_TestTraceListHand = sdkSysListCreate();

        if (g_TestTraceListHand == NULL) {
            Assert(0);
            TestLog("sdkTestTraceInit", "��������");
        }
        else {
            TestLog("sdkTestTraceInit", "sdkTestTraceInit success!\r\n");
        }
    }
    else {
        Assert(0);
    }
    mFailedConter = 0;
    mOkConter = 0;
}

/*****************************************************************************
** Descriptions:	    ������Ƿ��ʼ��
** Parameters:    	void
** Returned value:	
** Created By:		������  2013.04.07
** Remarks: 		
*****************************************************************************/
static bool sdkCheckTestTraceIsInit(void) {
    return (g_TestTraceListHand != NULL) ? true : false;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	sdkTestClrStatisticsData
   ��������:	�������ͳ������
                         һ�㶼����һ���������̿�ʼǰ����

   ����ʱ��:2012.05.19 14:11:50
*******************************************************************/
void sdkTestClrStatisticsData(void) {
    char *pTestTace = NULL;

    mFailedConter = 0;
    mOkConter = 0;

    Assert(sdkCheckTestTraceIsInit() == true);
    if (g_TestTraceListHand != NULL) {  //ɾ����ǰ�Ĵ�����Ϣ
        int conter = 0, i = 0;

        conter = sdkSysListGetCnt(g_TestTraceListHand);
        for (i = 0; i < conter; i++) {
            Verify((pTestTace = sdkSysListGetAt(g_TestTraceListHand, 0)) != NULL);
            sdkFreeMem(pTestTace);  //�ͷŵ�������Ϣ
            Verify(sdkSysListRemoveAt(g_TestTraceListHand, 0) == SDK_OK);
        }
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	sdkTestPrintStatisticsData
   ��������:	��ӡ����ͳ��������
                         һ���ڲ�����ɺ��ӡ���Խ��
   ����ʱ��:2012.05.19 14:13:47
*******************************************************************/
int sdkTestPrintStatisticsData(void) {
    char *pTestTace = NULL;
    int conter = 0;
    char *buf;
    Assert(sdkCheckTestTraceIsInit() == true);

    TestLog("test", "\r\n���Խ��:\r\n ʧ����: %d \r\n �ɹ���:%d \r\n", mFailedConter, mOkConter);

    conter = sdkSysListGetCnt(g_TestTraceListHand);

    buf = (char *) sdkGetMem(1024);
    if (buf == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(buf, 0, 1024);
    if (conter > 0) {
        int index = 0;
        sprintf(buf, "��%d������", conter);
        Test_sdkPrintTestInfo(buf, NULL);
        memset(buf, 0, strlen(buf));
        while (index < conter) {
            Verify((pTestTace = sdkSysListGetAt(g_TestTraceListHand, index++)) != NULL);
            sprintf(buf, "\r\n�� %d ������\r\n%s \r\n", index, pTestTace);
            Test_sdkPrintTestInfo(NULL, buf);
            TestLog("test", buf);
            memset(buf, 0, strlen(buf));
        }
    }
    if (buf != NULL) {
        sdkFreeMem(buf);
        buf = NULL;
    }
    Assert(conter == mFailedConter); //����ĸ���Ӧ��������ĸ������
    return conter;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	skdTestIncFailed
   ��������:	����ʧ�ܴ���
                        ����ڵ��Ժ����Զ�����
   ����ʱ��:2012.05.19 14:17:28
*******************************************************************/
void skdTestIncFailed(const u8 *pfile, int line, const u8 *pInfo, const u8 *pstr) {
    char *pTestTace = NULL;
    u8 *temp;
    u32 len;

#if 0 /*Modify by ���ƽ at 2014.05.27  19:57 */
    mFailedConter++;
#else
    Assert(sdkCheckTestTraceIsInit() == true);

    //��֯�ڵ����ݲ�������
    Assert(sdkIsAscii(pfile) == true);  //ȷ����assic
    temp = (char *) sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    memset(temp, 0, 1024);
    len = (u32) sprintf((char *) temp, "INFO:%s  \r\n ���ʽ:%s  \r\n ,FILE:%s,LINE: %d", pInfo, pstr,
                        pfile, line);


    pTestTace = (u8 *) sdkGetMem(len + 1);

    if ((len < 1024)) {
        memset(pTestTace, 0, len + 1);
        strcpy(pTestTace, temp);

        if (sdkSysListAddAtEnd(g_TestTraceListHand, pTestTace) != SDK_OK) { //���벻�ɹ���Ҫ�ͷŽڵ��ڴ�ռ�
            TestLog("skdTestIncFailed", "Insert   tracenode err");
            sdkFreeMem(pTestTace);
        }
        else {
            mFailedConter++;
            TestLogHex("skdTestIncFailed", "mFailedConter Data:", (u8 * ) & mFailedConter,
                       4); //�������
        }
    }
    else  //������
    {
        TestLog("skdTestIncFailed", "exception ");
        if (pTestTace != NULL) {
            sdkFreeMem(pTestTace);
        }
    }
    if (temp != NULL) {
        sdkFreeMem(temp);
        temp = NULL;
    }
#endif /* if 0 */
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	skdTestIncOk
   ��������:	�����ɹ�����
                        ����ڵ��Ժ����Զ�����
   ����ʱ��:2012.05.19 14:17:28
*******************************************************************/
void skdTestIncOk() {
    mOkConter++;
}

s32 ShowTestResulstBox(u8 *pasfun, u8 *pasInfo) {
    char *temp;
    s32 rtn;

    temp = (char *) sdkGetMem(2048);
    if (temp == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(temp, 0, 2048);


    sprintf(temp, "%s%s", pasInfo, "\r1.��ȷ  2.���� \r3.����  4.�˳�");
    rtn = sdkDispMsgBox(pasfun, temp, 0,
                        SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4);
    if (temp != NULL) {
        sdkFreeMem(temp);
        temp = NULL;
    }
    return rtn;
}

s32 InputTestIndex(u8 *pTital, int size) {
    char *temp;
    int ii;

    temp = (char *) sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(temp, 0, 1024);

    while (1) {
        sprintf(temp, "����ģ���0~%d �˳�99", size);

        ii = sdkTestInputU16(pTital, temp);

        if (ii == 99) {
            break;
        }

        if ((size - 1) < ii) {
            sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
        }
        else {
            break;
        }
    }
    if (temp != NULL) {
        sdkFreeMem(temp);
        temp = NULL;
    }
    return ii;
}

s32 DisplayManualList(TEST_MANUAL_MAP const *pMap, s32 conter) {
    char *ptem;
    char temp2[256];
    int ii;
    s32 rtn;

    ptem = (char *) sdkGetMem(2048);
    if (ptem == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(ptem, 0, 2048);
    memset(temp2, 0, sizeof(temp2));


    for (ii = 0; ii < conter; ii++) {
        int len = sprintf(temp2, "%d.%s\r", ii, pMap[ii].pAsInfo);
        strcat(ptem, temp2);

        if (len > sizeof(temp2) || strlen(ptem) > 2048) {
            while (1) {
                sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
            }
        }
    }

    rtn = sdkDispListBox("ȫ���ֶ�������Ϣ", ptem, 0);
    if (ptem != NULL) {
        sdkFreeMem(ptem);
        ptem = NULL;
    }
    return rtn;
}

void Test_sdkSelectCase(u8 *pTital, TEST_MAP_INFO const *pInfo) {
    char *temp;
    int ii = 0;
//    s32 key  = 0;
    TEST_MANUAL_MAP const *pMap = pInfo->pMap;
    int size = pInfo->size;

    if (size == 0) {
        sdkDispMsgBox("��ʾ��Ϣ", "��ģ��û����Ϣ", 0, SDK_KEY_MASK_ALL);
        return;
    }

    temp = (char *) sdkGetMem(2048);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    memset(temp, 0, 2048);

    while (1) {
        ii = DisplayManualList(pMap, size);

        if (ii < 0) {
            if (temp != NULL) {
                sdkFreeMem(temp);
                temp = NULL;
            }

            return;
        }

        if (ii > (size - 1)) {
            sdkDispMsgBox("��ʾ��Ϣ", "�������", 0, SDK_KEY_MASK_ALL);
            continue;
        }
        RetTry: //����
        memset(temp, 0, 2048);
        Trace("zjp", "pInfo->pMap = 0x%x\r\n", pInfo->pMap);
        Trace("zjp", "GetIcdealDemonTestInfo().pMap = 0x%x\r\n", GetIcdealDemonTestInfo().pMap);
        if (pInfo->pMap == GetIcdealDemonTestInfo().pMap)//���Ե���mem��
        {
            Test_sdkSetMemCardIndex(ii);
        }
        pMap[ii].pAutoFun(temp, pMap[ii].pAsInfo);

        switch (ShowTestResulstBox(pMap[ii].pAsInfo, temp)) {
            case SDK_KEY_1: //��ȷ
            {
                break;
            }

            case SDK_KEY_2: //�����¼�´�����Ϣ
            {
                skdTestIncFailed(__FILE__, __LINE__, temp, pMap[ii].pAsInfo);
                break;
            }

            case SDK_KEY_3:  //����
            {
                goto RetTry;
                break;
            }

            case SDK_KEY_4:  //�˳�
            {
                /*=======BEGIN: wangmingming 2014.05.29  16:1 modify===========*/
                if (temp != NULL) {
                    sdkFreeMem(temp);
                    temp = NULL;
                }
                /*====================== END======================== */
                return;

                break;
            }
        }
    }


    if (temp != NULL) {
        sdkFreeMem(temp);
        temp = NULL;
    }
}

/*****************************************************************************
** Descriptions:	��ʾ������Ϣ
** Parameters:          u8 const *pasInfo  Ҫ��ʾ����Ϣ
                    s32 keyMask        �������룬�����Ӧ��Щ������
** Returned value:	�û��������ĸ�����
** Created By:		shiweisong  2013.01.06
** Remarks:
*****************************************************************************/
s32 sdkTestMsgBox(u8 const *pasInfo, s32 keyMask) {
    return sdkDispMsgBox("������Ϣ", pasInfo, 0, keyMask);
}

/*****************************************************************************
** Descriptions:	����һ�������
** Parameters:          u8 const *pasInfo      ��ʾ����ʾ��Ϣ
                    u8 *pheOut
                    const s32 siMinLen
                    const s32 siMaxLen
                    const u32 uiMode    �� sdkKbGetScanf  ������ģʽ��һ����
** Returned value:
** Created By:		shiweisong  2013.01.06
** Remarks:
*****************************************************************************/
s32 sdkTestEditBox(u8 const *pasTitle, u8 const *pasInfo, u8 *pheOut, const s32 siMinLen,
                   const s32 siMaxLen, const u32 uiMode) {
    s32 Key = 0;

    //  u16 *ptemp = (u16 *)sdkFree(sdkDispGetScreenBufferSize()); //����������

    //   if(ptemp != NULL)
    {
        u8 Temp[128];
        memset(Temp, 0, sizeof(Temp));
        //     sdkDispSaveScreen(ptemp, sdkDispGetScreenBufferSize());
        sdkDispClearScreen();
        Verify(sdkDispFillRowRam(SDK_DISP_LINE1, 0, pasTitle, SDK_DISP_DEFAULT) >= 0);

        if (strlen(pasInfo) > 20) {
            s32 le = sdkSplitStr(Temp, pasInfo, 0, 20);
            Assert(le > 0);
            Verify(sdkDispFillRowRam(SDK_DISP_LINE2, 0, Temp, SDK_DISP_LEFT_DEFAULT) >= 0);
            Verify(sdkDispFillRowRam(SDK_DISP_LINE3, 0, &pasInfo[le], SDK_DISP_LEFT_DEFAULT) >= 0);
            /*=======BEGIN: wangmingming 2014.07.09  10:30 modify===========*/
            sdkDispBrushScreen();
            Key = sdkKbGetScanf(0, (u8 *) pheOut, siMinLen, siMaxLen, uiMode, SDK_DISP_LINE4);
            /*====================== END======================== */
        }
        else {
            Verify(sdkDispFillRowRam(SDK_DISP_LINE2, 0, pasInfo, SDK_DISP_LEFT_DEFAULT) >= 0);
            /*=======BEGIN: wangmingming 2014.07.09  10:31 modify===========*/
            //����ʹ�õ�������Ϊ��ѡ������ʾ
            sdkDispBrushScreen();
            Key = sdkKbGetScanf(0, (u8 *) pheOut, siMinLen, siMaxLen, uiMode, SDK_DISP_LINE3);
            /*====================== END======================== */
        }

        //  Verify(sdkDispRestoreScreen(ptemp, sdkDispGetScreenBufferSize()) >= 0); //�ָ�������
        //  sdkFree(ptemp);
        return Key;
    }

}

/*****************************************************************************
** Descriptions:	InputNO ��������
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
static int InputNO(u8 const *pasTitle, u8 const *pasInfo, s32 Minlen, s32 MaxLen) {
    u8 tem[100] = {0};
    int ret;

    while (1) {
        memset(tem, 0, sizeof(tem));

        if ((ret = sdkTestEditBox(pasTitle, pasInfo, tem, Minlen, MaxLen, SDK_MMI_NUMBER)) ==
            SDK_KEY_ENTER) {
            //if(sdkIsAsciiNum(tem) == true)
            {
                ret = atoi(&tem[1]);
                break;
            }
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    return ret;
}


/*****************************************************************************
** Descriptions:	Ԥ����ʾ���ݵ��������뺯��
** Parameters:    	u8 const *pasTitle
                               u8 const *pasInfo
                               u8 *pasContent
                               s32 Minlen
                               s32 MaxLen
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
static int PresetInputNO(u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent, s32 Minlen,
                         s32 MaxLen) {
    int ret;
    u8 tem[100] = {0};

    tem[0] = strlen(pasContent);
    strcpy(&tem[1], pasContent);
    while (1) {
        if ((ret = sdkTestEditBox(pasTitle, pasInfo, tem, Minlen, MaxLen, SDK_MMI_NUMBER)) ==
            SDK_KEY_ENTER) {
            //if(sdkIsAsciiNum(tem) == true)
            {
                ret = atoi(&tem[1]);
                break;
            }
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    return ret;
}


/*****************************************************************************
** Descriptions:	����u8
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
u8 sdkTestInputU8(u8 const *pasTitle, u8 const *pasInfo) {
    return (u8) InputNO(pasTitle, pasInfo, 1, 3);
}


/*****************************************************************************
** Descriptions:	����u8,Ԥ������ʾ����
** Parameters:    	u8 const *pasTitle
                               u8 const *pasInfo
                               u8 *pasContent
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
u8 sdkTestPresetInputU8(u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent) {
    return (u8) PresetInputNO(pasTitle, pasInfo, pasContent, 1, 3);
}


/*****************************************************************************
** Descriptions:	����u8
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
char sdkTestInputChar(u8 const *pasTitle, u8 const *pasInfo) {
    return (char) InputNO(pasTitle, pasInfo, 1, 3);
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
u16 sdkTestInputU16(u8 const *pasTitle, u8 const *pasInfo) {
    return (u16) InputNO(pasTitle, pasInfo, 1, 5);
}


/*****************************************************************************
** Descriptions:	����u16��Ԥ������ʾ����
** Parameters:    	u8 const *pasTitle
                               u8 const *pasInfo
                               u8 *pasContent
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
u16 sdkTestPresetInputU16(u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent) {
    return (u16) PresetInputNO(pasTitle, pasInfo, pasContent, 1, 5);
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
s16 sdkTestInputS16(u8 const *pasTitle, u8 const *pasInfo) {
    return (s16) InputNO(pasTitle, pasInfo, 1, 5);
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
u32 sdkTestInputU32(u8 const *pasTitle, u8 const *pasInfo) {
    return (u32) InputNO(pasTitle, pasInfo, 1, 10);
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 const *pasTitle
                               u8 const *pasInfo
                               u8 *pasContent
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
u32 sdkTestPresetInputU32(u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent) {
    return (u32) PresetInputNO(pasTitle, pasInfo, pasContent, 1, 10);
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.19
** Remarks:
*****************************************************************************/
s32 sdkTestInputs32(u8 const *pasTitle, u8 const *pasInfo) {
    return InputNO(pasTitle, pasInfo, 1, 10);
}

/*****************************************************************************
** Descriptions:	  ѡ���������к� ������ȡ��
** Parameters:          const *pasTitle
                               u8 const *pasInfo
** Returned value:
** Created By:		shiweisong  2013.01.26
** Remarks:
*****************************************************************************/
s32 sdkTestInputTestIndex(u8 const *pasTitle, u8 const *pasInfo) {
    u8 tem[10] = {0};
    int ret = SDK_ESC;

    while (1) {
        memset(tem, 0, sizeof(tem));

        if ((ret = sdkTestEditBox(pasTitle, pasInfo, tem, 0, 5, SDK_MMI_NUMBER)) == SDK_KEY_ENTER) {
            //if(sdkIsAsciiNum(tem) == true)
            {
                ret = atoi(&tem[1]);
                break;
            }
        }

        if (ret == SDK_KEY_ESC) {
            return SDK_ESC;
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8*pOut
                               u8 const *pasInfo
** Returned value:
** Created By:		fusuipu  2013.01.25
** Remarks:
*****************************************************************************/
s32 sdkTestInputBCD(u8 const *pTitle, u8 *pOut, u8 const *pasInfo) {
    u8 *tem;
    u8 *src;
    int len = 0;

    tem = (u8 *) sdkGetMem(1024);
    if (tem == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(tem, 0, 1024);

    src = (u8 *) sdkGetMem(1024);
    if (src == NULL) {
        Assert(0);
        if (tem != NULL) {
            sdkFreeMem(tem);
            tem = NULL;
        }
        return SDK_ERR;
    }
    memset(src, 0, 1024);


    while (1) {
        memset(tem, 0, 1024);

        if (sdkTestEditBox(pTitle, pasInfo, tem, 1, 50, SDK_MMI_NUMBER | SDK_MMI_LETTER) ==
            SDK_KEY_ENTER) {
            len = strlen(&tem[1]);
            strcpy(src, &tem[1]);

            if (sdkAscToBcdR(pOut, src, (len + 1) / 2) >= 0) {
                if (tem != NULL) {
                    sdkFreeMem(tem);
                    tem = NULL;
                }
                if (src != NULL) {
                    sdkFreeMem(src);
                    src = NULL;
                }
                return (len + 1) / 2;
            }
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }
    if (tem != NULL) {
        sdkFreeMem(tem);
        tem = NULL;
    }
    if (src != NULL) {
        sdkFreeMem(src);
        src = NULL;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8*pOut
                               u8 const *pasInfo
** Returned value:
** Created By:		fusuipu  2013.01.25
** Remarks:
*****************************************************************************/
s32 sdkTestPintHex(u8 *pDst, u8 *pSrc, s32 siLen) {
    s32 i = 0;
    u8 buf[256] = {0};

    for (i = 0; i < siLen; i++) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "0x%02X ", pSrc[i]);
        strcat(pDst, buf);
    }

    return 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pOut
                               u8 const *pasTitle
                               u8 const *pasInfo
                               s32 siMin
                               s32 siMax
** Returned value:
** Created By:		huacong  2013.01.25
** Remarks:
*****************************************************************************/
s32 sdkTestInputASC(u8 *pOut, u8 const *pasTitle, u8 const *pasInfo, s32 Minlen, s32 MaxLen) {
    u8 temp[512];

    while (1) {
        memset(temp, 0, sizeof(temp));

        if (sdkTestEditBox(pasTitle, pasInfo, temp, Minlen, MaxLen,
                           SDK_MMI_NUMBER | SDK_MMI_LETTER | SDK_MMI_HZ | SDK_MMI_SYMBOL) ==
            SDK_KEY_ENTER) {
            break;
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    memcpy(pOut, &temp[1], temp[0]);
    return (s32) temp[0];            //���س���
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *pOut
                               u8 const *pasTitle
                               u8 const *pasInfo
                               s32 Minlen
                               s32 MaxLen
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
s32 sdkTestPresetInputASC(u8 *pOut, u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent,
                          s32 Minlen, s32 MaxLen) {
    u8 temp[512] = {0};

    temp[0] = strlen(pasContent);
    strcpy(&temp[1], pasContent);
    while (1) {
        // memset(temp, 0, sizeof(temp));

        if (sdkTestEditBox(pasTitle, pasInfo, temp, Minlen, MaxLen,
                           SDK_MMI_NUMBER | SDK_MMI_LETTER | SDK_MMI_HZ | SDK_MMI_SYMBOL) ==
            SDK_KEY_ENTER) {
            break;
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    memcpy(pOut, &temp[1], temp[0]);
    return (s32) temp[0];            //���س���
}

/*****************************************************************************
** Descriptions:		�������ֻ���ĸ
** Parameters:         u8 *pOut
                       u8 const *pasTitle
                       u8 const *pasInfo
                       s32 siMin
                       s32 siMax
                       
** Returned value:
** Created By:		wangyanjie  2013.09.22
** Remarks:
*****************************************************************************/
s32 sdkTestInputASCNum(u8 *pOut, u8 const *pasTitle, u8 const *pasInfo, s32 Minlen, s32 MaxLen) {
    u8 temp[512];

    while (1) {
        memset(temp, 0, sizeof(temp));

        if (sdkTestEditBox(pasTitle, pasInfo, temp, Minlen, MaxLen, SDK_MMI_LETTER) ==
            SDK_KEY_ENTER) {
            break;
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    memcpy(pOut, &temp[1], temp[0]);
    return (s32) temp[0];            //���س���
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *pOut
                               u8 const *pasTitle
                               u8 const *pasInfo
                               s32 Minlen
                               s32 MaxLen
** Returned value:	
** Created By:		zhongjiahong  2013.11.27
** Remarks: 		
*****************************************************************************/
s32 sdkTestPresetInputASCNum(u8 *pOut, u8 const *pasTitle, u8 const *pasInfo, u8 *pasContent,
                             s32 Minlen, s32 MaxLen) {
    u8 temp[512] = {0};

    temp[0] = strlen(pasContent);
    strcpy(&temp[1], pasContent);
    while (1) {
        // memset(temp, 0, sizeof(temp));

        if (sdkTestEditBox(pasTitle, pasInfo, temp, Minlen, MaxLen, SDK_MMI_LETTER) ==
            SDK_KEY_ENTER) {
            break;
        }
        sdkTestMsgBox("��������,������", SDK_KEY_MASK_ALL);
    }

    memcpy(pOut, &temp[1], temp[0]);
    return (s32) temp[0];            //���س���
}

void test_UpDataProce() {
    static u32 conter;
    s32 ret = ((conter++) % 10);
    if (ret == 5) {
        sdkDispRow(SDK_DISP_LINE4, 0, "...              ", SDK_DISP_LEFT_DEFAULT);
    }
    else if (ret == 0) {
        sdkDispRow(SDK_DISP_LINE4, 0, ".......         ", SDK_DISP_LEFT_DEFAULT);
    }
}


void sdkTestShowProcessing(const u8 *pastr) {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���Խ���", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, pastr, SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	s32 index
** Returned value:	
** Created By:		shiweisong  2013.10.11
** Remarks: 		
*****************************************************************************/
static SDK_PRINT_FONT sdkTestGetPrintFont(s32 index) {
    SDK_PRINT_FONT font[] =
            {
                    {0, SDK_PRN_ASCII12X24, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII12X24, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII12X24, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
            };
    s32 size = sizeof(font) / sizeof(font[0]) - 1;
    index = (index > size) ? size : index;

    return font[index];
}

static s32 Test_sdkPrintTitle(u8 *pTitle) {
    static s32 count = 0;
    s32 rtn;
    u8 *temp;

    if (NULL == pTitle) {
        return SDK_PARA_ERR;
    }

    temp = (u8 *) sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    memset(temp, 0, 1024);

    sprintf(temp, "%d:", count++);
    strcat(temp, pTitle);

    rtn = sdkPrintStr(temp, sdkTestGetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 0);
    if (temp != NULL) {
        sdkFreeMem(temp);
        temp = NULL;
    }
    return rtn;

}

static s32 Test_sdkPrintInfo(u8 *pas) {
    s32 rtn;
    if (NULL == pas) {
        return SDK_PARA_ERR;
    }

    sdkPrintStr(pas, sdkTestGetPrintFont(1), SDK_PRINT_LEFTALIGN, 0, 0);
    rtn = sdkPrintStr(" ", sdkTestGetPrintFont(1), SDK_PRINT_LEFTALIGN, 0, 20);

    return rtn;

}

/*****************************************************************************
** Descriptions:	�ڴ�ӡֽ�ϴ�ӡ����ǣ����������Ա�ԱȲ���
** Parameters:          u8 *pTitle
** Returned value:
** Created By:		fusuipu  2013.08.21
** Remarks:
*****************************************************************************/
void Test_sdkPrintTestInfo(u8 *pTitle, u8 *pInfo) {
    Test_sdkPrintTitle(pTitle);
    Test_sdkPrintInfo(pInfo);
}

