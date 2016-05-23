#include "sdkGlobal.h"
#include "SdkTest.h"
#include "sdkPrivateHead.h"

#if 0

static void testmalloc(void)
{
#if 0
    u8 *temp = NULL;
    int conter = 0;

    while(1)
    {
        temp = sdkGetMem(1024 * 5);
        conter++;

        if(temp != NULL)
        {
            memset(temp, 0xFF, sizeof(temp));
            TestLog("sjl", "sdkGetMem is %d\r\n", conter);
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "okokokoko", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL); //fill ram
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "1.LOGON", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //fill ram
            sdkDispBrushScreen();         //display
        }
        else
        {
            TestLog("sjl", "sdkGetMem is null   ++++\r\n");
        }
    }

#endif
}

static void testSaveScreen(void)
{
#if 0
    // s32 LcdScreenSize;
    u8 *temp = NULL;

    TestLog("sjl", "begin disp    ++++\r\n");
    sdkDispClearScreen();
    sdkDispShowBmp(0, 0, 128, 64, "/mtd0/res/111.bmp");
    sdkDispBrushScreen ();  //brush screen
    temp = sdkGetMem(sdkDispGetScreenBufferSize());

    TestLog("sjl", "sdkDispGetScreenBufferSize %d \r\n", sdkDispGetScreenBufferSize());

    memset(temp, 0x99, sdkDispGetScreenBufferSize());
    sdkDispSaveScreen(temp, sdkDispGetScreenBufferSize());
    TestLogHex("kdkd", "buffer", temp, sdkDispGetScreenBufferSize());
    TestLog("sjl", "sdkDispGetScreenBufferSize %d \r\n", sdkDispGetScreenBufferSize());
#endif
}

static void TestMem1(void)
{
    u8*pmem = sdkGetMem(50);

    if(pmem != NULL)
    {
        memset(pmem, 0, 55);
        sdkFreeMem(pmem);
    }
}

static void TestMem2(void)
{
    u8*pmem = sdkGetMem(50);

    if(pmem != NULL)
    {
        memset(pmem, 0, 50);
        sdkFreeMem(pmem);
    }
}

SDK_SYS_HOOK_RET sdkTestHook(const u8 * const pData, s32 len)
{
    TestLogHex("sjl", "data is  ++++++=", pData, len);
    return SDK_SYS_END_HOOK;
}

void testhook(void)
{
    SDK_PRIVATE_FIFO stFifo;

    sdkSysSetComHook(sdkTestHook);

    while(1)
    {
        //  Private_sdkReadFifo(&stFifo,0);
        TestLogHex("sjl", "fifo data is +++++++", stFifo.heData, stFifo.usLen);
    }
}

#endif


const u8 *keyName[] =
        {
                "SDK_KEY_0",
                "SDK_KEY_1",
                "SDK_KEY_2",
                "SDK_KEY_3",
                "SDK_KEY_4",
                "SDK_KEY_5",
                "SDK_KEY_6",
                "SDK_KEY_7",
                "SDK_KEY_8",
                "SDK_KEY_9",
                "SDK_KEY_10",
                "SDK_KEY_11",
                "SDK_KEY_ENTER",
                "SDK_KEY_ESC",
                "SDK_KEY_UP",
                "SDK_KEY_DOWN",
                "SDK_KEY_F1",
                "SDK_KEY_F2",
                "SDK_KEY_F3",
                "SDK_KEY_F4",
                "SDK_KEY_CLEAR",
                "SDK_KEY_DOUBLEZERO",
                "SDK_KEY_ALPHA",
                "SDK_KEY_PRINT",
                "SDK_KEY_SIGN",
                "SDK_KEY_FUNCTION",
                "SDK_KEY_MULTITASK",
                "SDK_KEY_BACKSPACE",
                "SDK_KEY_LEFT",
                "SDK_KEY_RIGHT",
                "SDK_KEY_POWER_OFF"

        };
const u8 keyValue[] =
        {
                SDK_KEY_0,
                SDK_KEY_1,
                SDK_KEY_2,
                SDK_KEY_3,
                SDK_KEY_4,
                SDK_KEY_5,
                SDK_KEY_6,
                SDK_KEY_7,
                SDK_KEY_8,
                SDK_KEY_9,
                SDK_KEY_10,
                SDK_KEY_11,
                SDK_KEY_ENTER,
                SDK_KEY_ESC,
                SDK_KEY_UP,
                SDK_KEY_DOWN,
                SDK_KEY_F1,
                SDK_KEY_F2,
                SDK_KEY_F3,
                SDK_KEY_F4,
                SDK_KEY_CLEAR,
                SDK_KEY_DOUBLEZERO,
                SDK_KEY_ALPHA,
                SDK_KEY_PRINT,
                SDK_KEY_SIGN,
                SDK_KEY_FUNCTION,
                SDK_KEY_MULTITASK,
                SDK_KEY_BACKSPACE,
                SDK_KEY_LEFT,
                SDK_KEY_RIGHT,
                SDK_KEY_POWER_OFF,

        };

/*********************************************************************************************************
** Descriotions:      A test for getting combined keys
** parameters:
** Returned value:
** Created By:        chenbilun 20111216
** Remarks:           for ddi plam only
*********************************************************************************************************/
void Test_sdkKbGetCombinedKey(void) {
    u32 i;
    u32 combKey = 0;
    u8 key1 = 0;
    u8 key2 = 0;
    u8 temp[128] = {0};

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "TEST COMBINED KEY",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PRESS <F + ȡ��> QUIT",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    while (1) {
        combKey = (u32) sdkKbGetCombinedKey();

        if (combKey) {
            if ((combKey >> 24) == SDK_KEY_FUNCTION) {
                key1 = combKey >> 24;
                key2 = combKey & 0x000000FF;

                if (key2 == SDK_KEY_ESC) {
                    break;
                }
            }
            else {
                key1 = (combKey & 0x0000FF00) >> 8;
                key2 = combKey & 0x000000FF;
            }

            i = 0;
            memset(temp, 0, sizeof(temp));
            memcpy(temp, "����1 = ��", 10);
            while (i < sizeof(keyValue)) {
                if (key1 == keyValue[i]) {
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "����1 = %s", keyName[i]);
                }
                i++;
            }
            sdkDispClearRow(SDK_DISP_LINE3);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

            i = 0;
            memset(temp, 0, sizeof(temp));
            memcpy(temp, "����2 = ��", 10);
            while (i < sizeof(keyValue)) {
                if (key2 == keyValue[i]) {
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "����2 = %s", keyName[i]);
                }
                i++;
            }
            sdkDispClearRow(SDK_DISP_LINE4);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
        }
    }
}

/*********************************************************************************************************
** Descriotions:      A test for getting combined keys press long
** parameters:
** Returned value:
** Created By:        chenbilun 20111216
** Remarks:           for ddi plam only
*********************************************************************************************************/
void Test_CombKeyLongPress(void) {
    u32 i;
    u32 getLongKeyTimes = 0;
    s32 combKey = 0;
    u8 key1 = 0;
    u8 key2 = 0;
    u8 keyold[2] = {0};
    u8 temp[128] = {0};

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "TEST PRESS COMB_KEY LONG",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PRESS <F + ȡ��> QUIT",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    sdkKbkeySetLongPress(1);
    while (1) {
        combKey = sdkKbGetCombinedKey();

        if (combKey) {
            if ((combKey >> 24) == SDK_KEY_FUNCTION) {
                key1 = SDK_KEY_FUNCTION;
                key2 = combKey & 0x000000FF;

                if (key2 == SDK_KEY_ESC) {
                    break;
                }
            }
            else {
                key1 = (combKey & 0x0000FF00) >> 8;
                key2 = combKey & 0x000000FF;
            }

            if (keyold[0] == 0 || (keyold[0] != key1 || keyold[1] != key2)) {
                keyold[0] = key1;
                keyold[1] = key2;
                getLongKeyTimes = 1;
            }
            else {
                getLongKeyTimes++;
            }

            Trace("zjp", "combKey = 0x%08x, key1 = 0x%02x; key2 = 0x%02x\r\n", combKey, key1, key2);
            i = 0;
            memset(temp, 0, sizeof(temp));
            memcpy(temp, "����1 = ��", 10);
            while (i < sizeof(keyValue)) {
                if (key1 == keyValue[i]) {
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "����1 = %s", keyName[i]);
                }
                i++;
            }
            sdkDispClearRow(SDK_DISP_LINE3);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

            i = 0;
            memset(temp, 0, sizeof(temp));
            sprintf(temp, "����2 = ��", 10);
            while (i < sizeof(keyValue)) {
                if (key2 == keyValue[i]) {
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "����2 = %s", keyName[i]);
                }
                i++;
            }
            sdkDispClearRow(SDK_DISP_LINE4);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

            memset(temp, 0, sizeof(temp));
            sprintf(temp, "�������� = %d", getLongKeyTimes);
            sdkDispClearRow(SDK_DISP_LINE5);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);


            sdkDispBrushScreen();
        }
    }

    sdkKbkeySetLongPress(0);
}


/*********************************************************************************************************
** Descriotions:      A test for getting key press long
** parameters:
** Returned value:
** Created By:        chenbilun 20111216
** Remarks:           for ddi plam only
*********************************************************************************************************/
void Test_KeyLongPress(void) {
    u32 i;
    u32 getLongKeyTimes = 0;
    s32 combKey = 0;
    u8 keyold = 0;
    u8 temp[128] = {0};

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "TEST PRESS KEY LONG",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "PRESS <ȡ��> QUIT",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    sdkKbkeySetLongPress(1);
    while (1) {
        combKey = sdkKbGetKey();

        if (combKey) {
            if (combKey == SDK_KEY_ESC) {
                break;
            }
            if (keyold == 0 || keyold != combKey) {
                keyold = combKey;
                getLongKeyTimes = 1;
            }
            else {
                getLongKeyTimes++;
            }

            i = 0;
            memset(temp, 0, sizeof(temp));
            sprintf(temp, "���� = ��", 10);
            while (i < sizeof(keyValue)) {
                if (keyold == keyValue[i]) {
                    memset(temp, 0, sizeof(temp));
                    sprintf(temp, "���� = %s", keyName[i]);
                }
                i++;
            }
            sdkDispClearRow(SDK_DISP_LINE3);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

            memset(temp, 0, sizeof(temp));
            sprintf(temp, "�������� = %d", getLongKeyTimes);
            sdkDispClearRow(SDK_DISP_LINE4);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

            sdkDispBrushScreen();
        }
    }

    sdkKbkeySetLongPress(0);
}


#if 0 /*Modify by shijianglong at 2013.05.21  15:42 */


void TestPrinttemp(void)
{
    s32 i;
    SDK_PRINT_FONT font;

    sdkDispMsgBox("��ӡ����", "������", 1, 0);

    Verify(sdkPrintInit() >= 0);
//    for(i = 0;i<SDK_FONT_BMP;i++)
    memset(&font, 0, sizeof(font));

    if(sdkSysIsPrintPin())
    {
        u32 tempFont = 0;
        tempFont = SDK_PRN_PIN_SMALL;
        memcpy(&font, &tempFont, sizeof(font));
    }
    else
    {
        font.uiAscFont  = SDK_PRN_ASCII8X16;
        font.uiAscZoom =  SDK_PRN_ZOOM_N;
        font.uiChFont =  E_WORD_NATIVE_16_16;
        font.uiChZoom = SDK_PRN_ZOOM_N;
    }

    for(i = 0;i<1;i++)
    {
        sdkPrintStr(".��,����������`�p?��^������", font, SDK_PRINT_LEFTALIGN, 0, 1);
        sdkPrintStr("����~\\�M||��-��()�x�y�����z", font, SDK_PRINT_LEFTALIGN, 0, 1);//lk 2013.08.22 16:25
//        sdkPrintStr("�{����??����[]{}����������", i, SDK_PRINT_LEFTALIGN, 0, 1);
//        sdkPrintStr("��??�����o�s�r�����t�n���@", i, SDK_PRINT_LEFTALIGN, 0, 1);
//        sdkPrintStr("-����_��?�U;??�k�j��A�D@����", i, SDK_PRINT_LEFTALIGN, 0, 1) ;
//        sdkPrintStr("��������-?�|���}���~���l�m", i, SDK_PRINT_LEFTALIGN, 0, 1);
//        sdkPrintStr("?+=<�\~#$&%*\�h�i�������", i, SDK_PRINT_LEFTALIGN, 0, 1);
//        sdkPrintStr("��_��������硥����", i, SDK_PRINT_LEFTALIGN, 0, 1);

//        sdkPrintStr("����", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr("����", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr(".��,������", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr("����`�p?��^��''����", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr("?��^��''����", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr(".��,����������`�p?��^��''����", i, SDK_PRINT_LEFTALIGN, 0, 0);


//        sdkPrintStr(".��,����������`�p?��^��''����", i, SDK_PRINT_LEFTALIGN, 0, 0);
//        sdkPrintStr("����~\�M||��-��()�x�y�����z", i, SDK_PRINT_LEFTALIGN, 0, 0);
      }


    Verify(sdkPrintStart() >= 0);

    
}



/*****************************************************************************
** Descriptions:	     ������ֱ�ӷ���FIFOָ���ѯ״̬
** Parameters:          void
** Returned value:
** Created By:		������  2013.04.02
** Remarks:
*****************************************************************************/
s32 testPrint(void)
{
//    u8 Buf[16] = {0};
    s32 TimerID = 0, rslt = 0; //,machine_type = 0
//    FIFO fifo = {0};
    SDK_PRIVATE_FIFO fifo;

//    Buf[0] = 0x01;
    fifo.usCmd = 0x00B4;
    fifo.usLen = 1;
    fifo.heData[0] = 0x01;
    Private_sdkWriteFifo(&fifo);
    TimerID = sdkTimerGetId();
    TestLog("sdkPrintPinInit", "begin!");

    while (1)
    {
        if (sdkTimerIsEnd(TimerID, 200))
        {
            rslt = SDK_TIME_OUT;
            break;
        }
        TestLog("sdkPrintPinInit", "Waiting!");

        if(Private_sdkReadFifo(&fifo, 50))
        {
            TestLogHex("sdkPrintPinInit", "fifo.Cmd:", (u8 *)&fifo.usCmd, 2);     //�������
            TestLogHex("sdkPrintPinInit", "fifo.Data:", (u8 *)fifo.heData, 1);     //�������

            if (fifo.usCmd == 0x00B4)
            {
                if (fifo.heData[0] == 1)                         //��ֽ
                {
                    rslt = SDK_OK;
                    break;
                }
                else if (fifo.heData[0] == 0)                    //�޷���ȡ
                {
                    rslt = SDK_PRINT_LOSE_COMMAND;
                    break;
                }
                else if (fifo.heData[0] == 2)                    //ȱֽ
                {
                    rslt = SDK_PRINT_OUTOF_PAPER;
                    break;
                }
                else if (fifo.heData[0] == 3)                    //��ӡ������
                {
                    rslt = SDK_PRINT_DEV_FAIL;
                    break;
                }
                else    // ��ʶ������ shiweisong 2013.04.03 14:28
                {
                    Assert(0);
                }
            }
        }
    }

    return rslt;
}

#endif /* if 0 */

#if 0 /*Modify by shiweisong at 2013.12.10  10:58 */
SDK_PRINT_FONT sdkGetPrintFront(s32 index)
{

   static const SDK_PRINT_FONT font[] =
        {
        {0,SDK_PRN_ASCII12X24,SDK_PRN_ZOOM_N,SDK_PRN_CH12X12,SDK_PRN_ZOOM_N,0},
        {0,SDK_PRN_ASCII6X12,SDK_PRN_ZOOM_A,SDK_PRN_CH16X16,SDK_PRN_ZOOM_A,0},
        {0,SDK_PRN_ASCII8X16,SDK_PRN_ZOOM_B,SDK_PRN_CH20X20,SDK_PRN_ZOOM_B,0},
        {0,SDK_PRN_ASCII16X24,SDK_PRN_ZOOM_AB,SDK_PRN_CH24X24,SDK_PRN_ZOOM_A,0},
        {0,SDK_PRN_ASCII16X32,SDK_PRN_ZOOM_AB,SDK_PRN_CH24X24,SDK_PRN_ZOOM_AB,0},
        };

    index =  index >= sizeof(font)/sizeof(SDK_PRINT_FONT) ? (sizeof(font)/sizeof(SDK_PRINT_FONT)-1): index;
    return font[index];
 
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2013.09.03
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkMulPrint(void)
{
    SDK_PRINT_DATA stPrintData[6];
    int i = 0 ;
    u32 offset = 0 ;
    u8 temp[2];   
    
    TEST_IS_TRUE_WITH_INFO("��ӡ����ʼ��", sdkPrintInit() == SDK_OK);
    
    for(i = 0 ;  i < 1 ; i++)
        {
            memset(stPrintData, 0, sizeof(stPrintData));
            
            sdkGetRandom(temp,2);
            stPrintData[0].uiFont = sdkGetPrintFront(temp[0]%5);
            sprintf(stPrintData[0].pasText,"%d.",i);

            sdkGetRandom(temp,2);
            stPrintData[1].uiFont = sdkGetPrintFront(temp[0]%5);
            strcpy(stPrintData[1].pasText,"��ƫ��:");

            sdkGetRandom((u8*)&offset,sizeof(offset));
            offset = offset % 50;
            stPrintData[2].uiFont = sdkGetPrintFront(temp[0]%5);
            sprintf(stPrintData[2].pasText,"%d ",offset);

            sdkGetRandom(temp,2);
            stPrintData[3].uiFont = sdkGetPrintFront(temp[0]%5);
            strcpy(stPrintData[3].pasText,"������");

            sdkGetRandom(temp,2);
            stPrintData[4].uiFont = sdkGetPrintFront(temp[0]%5);
            strcpy(stPrintData[4].pasText,"����");
            
            sdkGetRandom(temp,2);
            stPrintData[5].uiFont = sdkGetPrintFront(temp[0]%5);
            strcpy(stPrintData[5].pasText,"test!");

            TEST_IS_TRUE_WITH_INFO("������print",sdkPrintMultStr(offset, &stPrintData[0],&stPrintData[1],&stPrintData[2],&stPrintData[3],&stPrintData[4],&stPrintData[5], NULL) == SDK_OK);
        }

    
    TEST_IS_TRUE_WITH_INFO("������print",sdkPrintStartWithMultFont()== SDK_OK);
}
#endif /* if 0 */

void testmultprint() {

    SDK_PRINT_DATA stPrintData[6];

    s32 ret = sdkPrintInit();
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�̻����                        MERCHANT COPY");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�̻���:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "�¹���");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�̻���:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "937320293990033");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);

    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�ն˺�:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "14370035          ");
    stPrintData[2].siOffset = 200;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "����Ա:");
    stPrintData[3].siOffset = 256;
    stPrintData[3].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[3].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[3].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[3].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[3].pasText, "01");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    &stPrintData[3],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "������:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "��������          ");
    stPrintData[2].siOffset = 200;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "�յ���:");
    stPrintData[3].siOffset = 256;
    stPrintData[3].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[3].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[3].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[3].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[3].pasText, "��������");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    &stPrintData[3],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "����(CARD NO):");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "4122 66** **** 1165");
    stPrintData[1].siOffset = 304;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[1].pasText, " S");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "��������(TRANS TYPE):");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "123           ����(SALE)");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "123           (SALE)����");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "(SALE)����           123");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "(SALE)����              ");
    stPrintData[1].siOffset = 0;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII6X12;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[1].pasText, "                     123");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);


    //=======================
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "��������(TRANS TYPE):");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "123           ����(SALE)");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "123           (SALE)����");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "(SALE)����           123");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[0].pasText, "(SALE)����              ");
    stPrintData[1].siOffset = 0;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH20X20;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[1].pasText, "                     123");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);


    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "���κ�:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "000005            ");
    stPrintData[2].siOffset = 200;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "ƾ֤��:");
    stPrintData[3].siOffset = 256;
    stPrintData[3].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[3].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[3].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[3].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[3].pasText, "000236");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    &stPrintData[3],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "��Ȩ��:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "��Ч��:");
    stPrintData[2].siOffset = 112;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "2014/02");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�ο���:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "104920057280");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "����/ʱ��:");
    stPrintData[1].siOffset = 80;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "2014");
    stPrintData[2].siOffset = 112;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "/02/12");
    stPrintData[3].siOffset = 160;
    stPrintData[3].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[3].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[3].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[3].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[3].pasText, " 10492020");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    &stPrintData[3],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "���(AMOUNT):");
    stPrintData[1].siOffset = 104;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[1].pasText, " RMB");
    stPrintData[2].siOffset = 168;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[2].pasText, "         0.01");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "���:");
    stPrintData[1].siOffset = 40;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, " RMB                         ");
    stPrintData[2].siOffset = 272;
    stPrintData[2].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[2].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[2].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[2].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[2].pasText, "-989990377.57");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    &stPrintData[2],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�ֻ���:");
    stPrintData[1].siOffset = 56;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_AB;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_AB;
    strcpy(stPrintData[1].pasText, "0");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "��ע/REFERENCE");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�����ֽ����:     ");
    stPrintData[1].siOffset = 144;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "                          000");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "ԭ�ο���(OLD REF. NO):");
    stPrintData[1].siOffset = 176;
    stPrintData[1].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[1].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[1].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[1].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[1].pasText, "0");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    &stPrintData[1],
                    NULL);
//    //=======================
//    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
//    stPrintData[0].siOffset = 0;
//    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
//    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
//    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
//    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
//    strcpy(stPrintData[0].pasText, "0");
//    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
//    				&stPrintData[0],
//    				NULL);
//    //=======================
//    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
//    stPrintData[0].siOffset = 0;
//    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
//    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
//    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
//    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
//    strcpy(stPrintData[0].pasText, "0");
//    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
//    				&stPrintData[0],
//    				NULL);
//    //=======================
//    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
//    stPrintData[0].siOffset = 0;
//    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
//    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
//    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
//    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
//    strcpy(stPrintData[0].pasText, "0");
//    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
//    				&stPrintData[0],
//    				NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "�ֿ���ǩ��(CARD HOLDER SIGNATURE):");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII12X24;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH24X24;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_N;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_N;
    strcpy(stPrintData[0].pasText, "------------------------");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    //=======================
    memset(stPrintData, 0, sizeof(SDK_PRINT_DATA) * 6);
    stPrintData[0].siOffset = 0;
    stPrintData[0].uiFont.uiAscFont = SDK_PRN_ASCII8X16;
    stPrintData[0].uiFont.uiChFont = SDK_PRN_CH16X16;
    stPrintData[0].uiFont.uiAscZoom = SDK_PRN_ZOOM_B;
    stPrintData[0].uiFont.uiChZoom = SDK_PRN_ZOOM_B;
    strcpy(stPrintData[0].pasText, "����ȷ�����Ͻ���,ͬ�⽫����뱾���˻�");
    sdkPrintMultStr(SDK_PRINT_RIGHTALIGN,
                    &stPrintData[0],
                    NULL);
    sdkPrintStartWithMultFont();
}


void sdkTestTempFun(void) {
    while (1) {
        s32 key = 0;
        key = sdkDispMsgBox("��ѡ�����ģ��",
                            "0.ǩ������ϴ�ӡ����\r1.��ϼ�����\r2.��ϼ���������\r3.������������\r", 0,
                            SDK_KEY_MASK_ALL);
        switch (key) {
            case SDK_KEY_0:
                testmultprint();
                break;
            case SDK_KEY_1:
                Test_sdkKbGetCombinedKey();
                break;
            case SDK_KEY_2:
                Test_CombKeyLongPress();
                break;
            case SDK_KEY_3:
                Test_KeyLongPress();
                break;
#if 0 /*Modify by ���ƽ at 2014.08.06  18:7 */
                case SDK_KEY_4:
                    sdkKbSetBgLightEnableIdle( sdkTestInputU8("Set key idle enable", "1=enable, 0=disable") );
                   break;
                case SDK_KEY_4:
                    sdkKbSetBgLightOnOrOff( sdkTestInputU8("Set key led on off", "idle on: 1, of:0") );
                   break;
                case SDK_KEY_6:
                   sdkKbSetBgLightIdleTimes( SDK_SECOND_TO_MS(sdkTestInputU8("Set key idle time", "Idle time: 1 ~ 255")));
                   break;
#endif /* if 0 */
            default:
                return;
        }
    }
}

