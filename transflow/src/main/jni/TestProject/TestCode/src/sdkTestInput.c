#include "sdkGlobal.h"
#include "SdkTest.h"
#include "stdarg.h"


/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkKbGetKey
   ��������:	����API����sdkKbGetKey
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:������ȡ����ʱ�˳�
   ����ʱ��:2012.06.04 14:21:31
*******************************************************************/
/*void sdkTest_sdkKbGetKey(void)
{
    u8 key, inc = 0;

    Trace("Test input", " \r\n \r\nStart (sdkKbGetKey/ �����ʾ����  \r\n \r\n");

   
    while(1)
    {
        key = sdkKbGetKey();

        if(key != 0)
        {
            inc++;
            TEST_IS_TRUE_WITH_INFO("��������", (key != 0));

            if(inc >= 30 || key == SDK_KEY_ESC)
            {
                break;
            }
        }
    }
}*/

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkKbKeyFlush
   ��������:	����API����sdkKbKeyFlush
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 14:51:39
*******************************************************************/
/*
   void sdkTest_sdkKbKeyFlush(void)
   {
   u8 key;
   Trace("Test display" , " \r\n \r\nStart (sdkDispAt/ ��ָ��������ʾ��ʱ�������� \r\n \r\n");

   key = sdkKbGetKey();
   if(key != 0)
   {
    sdkKbKeyFlush();
    key = sdkKbGetKey();
   }
   TEST_IS_TRUE_WITH_INFO("��������" , (sdkDispAt(64 , 32 , "Normal Testing") == SDK_OK));
   NULL pointer testing


   }*/

/*static void Tips(u8 *lpIn)
{
    u8 disp[210];

    memset(disp, 0, sizeof(disp));
    memcpy(disp, "������:", 7);
    strcat(disp, lpIn);
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��������", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, disp, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��������밴ȷ�ϼ�", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
}

static u8 ConfirmTips(u8 *ExpectResult, u8 *ActualResult)
{
    u8 expect[210], flag = 0;
    u8 actual[210];
    s32 Key;
    sdkDispClearScreen();

    memset(expect, 0, sizeof(expect));
    memset(actual, 0, sizeof(actual));
    memcpy(expect, "Ԥ��:", 5);
    memcpy(actual, "ʵ��:", 5);
    strcat(expect, ExpectResult);
    memcpy(&actual[5], &ActualResult[1], ActualResult[0]);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, expect, SDK_DISP_LEFT_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, actual, SDK_DISP_LEFT_DEFAULT);
    if(memcmp(ExpectResult, &ActualResult[1], (strlen(ExpectResult)>ActualResult[0])? strlen(ExpectResult):ActualResult[0]) == 0)
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "ʵ����Ԥ�ڽ����ͬ", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ�ϼ���һ���԰���", SDK_DISP_DEFAULT);
        flag =1;
        sdkDispBrushScreen();
        while(1)
        {
            Key = sdkKbGetKey();
            if(Key == SDK_KEY_ENTER)
            {          
                sdkSysBeep(SDK_SYS_BEEP_OK);
                break;
            }

        }
        Key = sdkKbWaitKey(SDK_KEY_ENTER, 0);
        
    }
    else
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "ʵ����Ԥ�ڽ������", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "ȷ�ϼ�������һ������", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȡ�����ظ���������", SDK_DISP_DEFAULT);
        sdkDispBrushScreen();
        while(1)
        {
            Key = sdkKbGetKey();
            if(Key == SDK_KEY_ENTER || Key == SDK_KEY_ESC)
            {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                break;
            }
                
        }
        //Key = sdkKbWaitKey(SDK_KEY_ENTER|SDK_KEY_ESC, 0);
         
    }
    
    if(Key == SDK_KEY_ENTER && flag == 1)
    {
        return SDK_TEST_OK;
    }
    else if(Key == SDK_KEY_ESC)
    {
        return SDK_TEST_REPEAT;
    }
    else
    {
        return SDK_TEST_FALSE;
    }
}

static void TestCase(u8 *TestType, u8 *ExpectInput, u8 *InitInput, u8 InputMode)
{
    u8 expect[210];
    u8 result[210], sdkkbgetscanf;
    
    memset(expect, 0, sizeof(expect));
    strcpy(expect, ExpectInput);
    memset(result, 0, sizeof(result));
    strcpy(result, InitInput);
    
    while(1)
    {
        Tips(expect);

        sdkKbGetScanf(0, result, 6, 10, InputMode, SDK_DISP_LINE3);
        {
            sdkkbgetscanf = ConfirmTips(expect, result);
            if( sdkkbgetscanf == SDK_TEST_REPEAT)
            {
                continue;
            }
            else
            {
                TEST_IS_TRUE_WITH_INFO(TestType, (sdkkbgetscanf == SDK_TEST_OK));
                break;
            }
        }
    }
}

static void BoundaryValueTestCase(u8 *TestType, u8 *ExpectInput, u8 *InitInput, u8 TestMode, ...)
{
    u8 expect[210];
    u8 result[210], sdkkbgetscanf;
    va_list ap;
    memset(expect, 0, sizeof(expect));
    strcpy(expect, ExpectInput);
    memset(result, 0, sizeof(result));
    strcpy(result, InitInput);

    while(1)
    {
        va_start(ap, TestMode);
        Tips(expect);
        switch(TestMode)
        {
            case 1:
                sdkKbGetScanf(va_arg(ap, s32), result, 6, 10, SDK_MMI_NUMBER, SDK_DISP_LINE3);
                break;

            case 2:
                memcpy(result,va_arg(ap, u8*), strlen(va_arg(ap, u8*)));
                sdkKbGetScanf(0, result, 6, 10, SDK_MMI_NUMBER, SDK_DISP_LINE3);
                break;

            case 3:
                sdkKbGetScanf(0, result, va_arg(ap, s32), 10, SDK_MMI_NUMBER, SDK_DISP_LINE3);
                break;

            case 4:
                sdkKbGetScanf(0, result, 0, va_arg(ap, s32), SDK_MMI_NUMBER, SDK_DISP_LINE3);
                break;

            case 5:
                sdkKbGetScanf(0, result, 6, 10, va_arg(ap, s32), SDK_DISP_LINE3);
                break;

            default :

                break;
        }
        va_end(ap);
        {
            sdkkbgetscanf = ConfirmTips(expect, result);
            if( sdkkbgetscanf == SDK_TEST_REPEAT)
            {
                continue;
            }
            else
            {
                TEST_IS_TRUE_WITH_INFO(TestType, (sdkkbgetscanf == SDK_TEST_OK));
                break;
            }
        }
    }
}

*/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkKbGetScanf
   ��������:	����API����sdkKbGetScanf
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 14:51:39
*******************************************************************/
//static void sdkTest_sdkKbGetScanf(void)
//{
/*u8 pbuf[128];
//u8 expect[21];

Trace("Test input", " \r\n \r\nStart (sdkKbGetScanf/ �������� \r\n \r\n");*/

/***************************�����������*******************************************/
/* TestCase("��������Сд��ĸ", "abcdefgh", "", SDK_MMI_LETTER);

 TestCase("�������Դ�д��ĸ", "ABCDEFGH", "", SDK_MMI_LETTER);

 TestCase("�������Ժ���", "������", "", SDK_MMI_HZ);

 TestCase("�������Է���", "(***_)", "", SDK_MMI_SYMBOL);

 TestCase("������������", "123456", "", SDK_MMI_NUMBER);

 TestCase("������������", "123456", "\x07wgdtest", SDK_MMI_PWD);

 TestCase("��������ʮ��������", "12AB34", "", SDK_MMI_HEX);

 TestCase("��������С��", "1234.56", "", SDK_MMI_POINT);

 TestCase("�������Ի������", "��234A", "", SDK_MMI_HZ|SDK_MMI_LETTER|SDK_MMI_SYMBOL|SDK_MMI_NUMBER);*/

/***************************�����������*******************************************/
/* TEST_IS_TRUE_WITH_INFO ("�����������(minlen  = 260)", (sdkKbGetScanf(0, pbuf, 260, 50, SDK_MMI_HEX | SDK_MMI_HZ, SDK_DISP_LINE3) == SDK_PARA_ERR));
 TEST_IS_TRUE_WITH_INFO ("��ָ�����", (sdkKbGetScanf(0, NULL, 10, 20, SDK_MMI_HEX | SDK_MMI_HZ, SDK_DISP_LINE3) == SDK_PARA_ERR));
 TEST_IS_TRUE_WITH_INFO ("�����������(maxlen  = 260)", (sdkKbGetScanf(0, pbuf, 4, 260, SDK_MMI_HEX | SDK_MMI_HZ, SDK_DISP_LINE3) == SDK_PARA_ERR));
 TEST_IS_TRUE_WITH_INFO ("�����������(line  = 6)", (sdkKbGetScanf(0, pbuf, 4, 50, SDK_MMI_HEX | SDK_MMI_HZ, 6) == SDK_PARA_ERR));
 BoundaryValueTestCase("�����������(��ʼ����\x82)", "123456", "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa", 2, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
 BoundaryValueTestCase("�����������(��ʼ����\x82  )", "123456", "\x82", 2, "\x82");*/


/***************************�߽�ֵ����*******************************************/

/*BoundaryValueTestCase("�߽�ֵ����(overtime = 0)", "123456", "\x07wgdtest", 1, 0);
BoundaryValueTestCase("�߽�ֵ����(��ʼ����\x00", "123456", "\x7Fwgdtestdjhkhkhd", 2, "\x00");
BoundaryValueTestCase("�߽�ֵ����(maxlen = SDK_MAX_STR_LEN)", "1111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111", "\x07wgdtest", 4, SDK_MAX_STR_LEN);
BoundaryValueTestCase("�߽�ֵ����(minlen = 0)", "", "\x02wg", 3, 0);
BoundaryValueTestCase("�߽�ֵ����(maxlen = 0)", "", "\x02wg", 4, 0);
BoundaryValueTestCase("�߽�ֵ����(uimode = 0)", "123456", "\x07wgdtest", 5, 0);
BoundaryValueTestCase("�߽�ֵ����(uimode = 0xffffffff)", "AB12��", "\x07wgdtest", 5, 0xffffffff);*/
//}

static void sdkAUTOTest_sdkKbGetScanf(void) {
    u8 pbuf[128];
    TEST_IS_TRUE_WITH_INFO("�����������(minlen  = 260)",
                           (sdkKbGetScanf(0, pbuf, 260, 50, SDK_MMI_HEX | SDK_MMI_HZ,
                                          SDK_DISP_LINE3) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkKbGetScanf(0, NULL, 10, 20, SDK_MMI_HEX | SDK_MMI_HZ,
                                                      SDK_DISP_LINE3) == SDK_PARA_ERR));
    sdkTestShowProcessing("�����50%...");
    TEST_IS_TRUE_WITH_INFO("�����������(maxlen  = 260)",
                           (sdkKbGetScanf(0, pbuf, 4, 260, SDK_MMI_HEX | SDK_MMI_HZ,
                                          SDK_DISP_LINE3) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(line  = 6)",
                           (sdkKbGetScanf(0, pbuf, 4, 50, SDK_MMI_HEX | SDK_MMI_HZ, 6) ==
                            SDK_PARA_ERR));
}


/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	Test_InputAuto
   ��������:	�����������main����
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 09:45:21
*******************************************************************/
void Test_InputAuto(void) {
    //sdkTest_sdkKbGetKey();
    sdkAUTOTest_sdkKbGetScanf();
}

