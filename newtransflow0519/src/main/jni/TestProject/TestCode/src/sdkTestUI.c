/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkTestUI.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��2��23��
   ����޸�   :
   ��������   : �˵�UI���Դ���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2013��2��23��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/
#include "sdkGlobal.h"
#include "SdkTest.h"
#include "sdkUI.h"


#define         SDK_MSG_OUT             0                       //����Ϣ������Ϻ�Ӷ������//clear this message from queue after being handled
#define         SDK_MSG_KEEP            1                       //����Ϣ������Ϻ��ƶ�������β��//move this message to the end of queue after being handled

SDK_UI_EXIT_VALUE ShowMainFace(s32 Index);

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
SDK_UI_EXIT_VALUE Eclipse(s32 Index) {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���ز˵�ִ�гɹ�!", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������˳�", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    return SDK_ESC;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 Index
** Returned value:
** Created By:		fusuipu  2013.04.16
** Remarks:
*****************************************************************************/
SDK_UI_EXIT_VALUE UITest2(s32 Index) {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�����˵�ִ�гɹ�!", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������˳�", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
    return SDK_ESC;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
SDK_UI_EXIT_VALUE UITest1(s32 Index) {
    u8 str_buf[21] = {0};

    sprintf(str_buf, "No%d�˵�ִ�гɹ�", Index + 1);

    sdkUIClearItems();
    sdkUISetShowMode(SDK_UI_SHOW_MODE_NOFDISP_1_ROW);
    sdkUISetTile("ִ�н��");
    sdkUIAddItem(str_buf, NULL);
    sdkUIAddItem("�Ӳ˵�����", UITest2);
    sdkUIShowMenu();

    return SDK_MSG_KEEP;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
u8 MenuBug[40][30] = {
        {"No1�˵��������!!!!!!"},
        {"No2�˵��������!!!!!!"},
        {"No3�˵��������!!!!!!"},
        {"No4�˵��������!!!!!!"},
        {"No5�˵��������!!!!!!"},
        {"No6�˵��������!!!!!!"},
        {"No7�˵��������!!!!!!"},
        {"No8�˵��������!!!!!!"},
        {"No9�˵��������!!!!!!"},
        {"No10�˵��������!!!!!!"},
        {"No11�˵��������!!!!!!"},
        {"No12�˵��������!!!!!!"},
        {"No13�˵��������!!!!!!"},
        {"No14�˵��������!!!!!!"},
        {"No15�˵��������!!!!!!"},
        {"No16�˵��������!!!!!!"},
        {"No17�˵��������!!!!!!"},
        {"No18�˵��������!!!!!!"},
        {"No19�˵��������!!!!!!"},
        {"No20�˵��������!!!!!!"},
        {"No21�˵��������!!!!!!"},
        {"No22�˵��������!!!!!!"},
        {"No23�˵��������!!!!!!"},
        {"No24�˵��������!!!!!!"},
        {"No25�˵��������!!!!!!"},
        {"No26�˵��������!!!!!!"},
        {"No27�˵��������!!!!!!"},
        {"No28�˵��������!!!!!!"},
        {"No29�˵��������!!!!!!"},
        {"No30�˵��������!!!!!!"},
        {"No31�˵��������!!!!!!"},
        {"No32�˵��������!!!!!!"},
        {"No33�˵��������!!!!!!"},
};

/*****************************************************************************
** Descriptions:    ��ʾ�˵�����
** Parameters:      Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 AviMenuShowMainFace(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    u8 title[25] = {0};
    s32 i = 0;
    s32 mode = sdkTestInputs32("menu test", "�˵�ģʽ:");
    s32 itemnum = sdkTestInputs32("menu test", "�˵�����(0~30):");
    s32 itemcircle = sdkTestInputs32("menu test", "�˵�������к�:");
    s32 ishavetitile = sdkTestInputs32("menu test", "�Ƿ���ӱ���:1-s�� 0-��");

    s32 locateitem = sdkTestInputs32("menu test", "ѡ��ǰ���Բ˵�:");   //zhongjiahong 2013.11.29 17:2

    Verify(sdkUISetModeitemNum(itemcircle) >= 0);
    Verify(sdkUISetShowMode(mode) >= 0);
    sdkUIClearItems();

    if (ishavetitile != 0) {
        strcpy(title, "�˵����ԣ�������!");
        sdkUISetTile(title);
    }

    if (itemnum > 40) {
        itemnum = 40;
    }

    for (i = 0; i < itemnum; i++) {
        sdkUIAddItem(MenuBug[i], UITest1);
    }
    sdkUISetSelectPosition(locateitem, itemnum);      //zhongjiahong 2013.12.02 9:44
    sdkUIShowMenu();
    sprintf(pasOutInfo, "�˵�ģʽ:%d \r�˵�����:%d \r�˵�������к�:%d\r�Ƿ��б���:%d", mode, itemnum,
            itemcircle, ishavetitile);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ��ʽ�˵�����
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 HidenMenuShowMainFace(char *pasOutInfo, char const *pTitle) {
    u8 temp[25] = {0};
    u8 temp1[25] = {0};
    s32 key = 0;

    s32 mode = 0;//sdkTestInputs32("menu test", "�˵�ģʽ:");


    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "������ѡ�����ؼ�", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    key = sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);

    Verify(sdkUISetShowMode(mode) >= 0);
    Verify(sdkUIClearItems() >= 0);
    sdkUIAddHideItem(key, Eclipse);

    sprintf(temp, "%dkey����Ϊ���ؼ�", key);
    sprintf(temp1, "press %dkey ִ�����ز˵�", key);
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, temp, SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp1, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    sprintf(pasOutInfo, "���ز˵���:%d", key);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���                                               COPYRIGHT:XGD
   ��          ��:ʯ����							CREATED BY:shiweisong
   ��������:	OnCircle								FUNCTION MANE: OnCircle

   ��   ��  ֵ: ������  SDK_MSG_KEEP			RETURN: must be SDK_MSG_KEEP
   �޸ı�ע:										REMARKS:
   ����ʱ��:2012.07.16 17:53:57					DATE:2012.07.16 17:53:57
*******************************************************************/
u8 OnCircle(SDK_MSG *lpMsg) {
    static s32 usTimer = 0;

//��һ100MS���ǵ��������//call following content if reach 100ms
    if (sdkTimerIsEnd(usTimer, 100) == true) {
        usTimer = sdkTimerGetId();
        // Trace("ExSdk", "usTimer:%d\r\n", usTimer);
    }
    return SDK_MSG_KEEP;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_MSG *lpMsg
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
u8 LoopSanf(SDK_MSG *lpMsg) {
    s32 key = sdkKbGetKey();

    if (key == SDK_KEY_ESC) {
        return SDK_MSG_OUT;
    }
    else if (key != SDK_KEY_ERR) {
        sdkSysBeep(SDK_SYS_BEEP_OK);
        Trace("Menu", "key = %d", key);

        if (SDK_ESC == sdkUIRevKey(key)) {
            return SDK_MSG_OUT;
        }
    }
    //  Trace("ExSdk", "LoopSanf");


    return SDK_MSG_KEEP;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 Test_sdkAviMenuTest(char *pasOutInfo, char const *pTitle) {
    u8 rslt;
    SDK_MSG *lpMsg = NULL;

    AviMenuShowMainFace(pasOutInfo, pTitle);

    Verify(sdkSysPackMessage(OnCircle, 0, 0) == SDK_OK);
    Verify(sdkSysPackMessage(LoopSanf, 0, 0) == SDK_OK);


    //Trace("fsp", "mgs keep \r\n");
    while (sdkSysQueryMessage(&lpMsg)) {
        rslt = lpMsg->pFun(lpMsg);

        if (rslt == SDK_MSG_KEEP) {
            Verify(sdkSysPostMessage(lpMsg) == SDK_OK);
        }
        else if (rslt == SDK_MSG_OUT) {
            sdkFreeMem(lpMsg);
            lpMsg = NULL;
            return SDK_ERR;
        }
    }

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 Test_sdkHidenMenuTest(char *pasOutInfo, char const *pTitle) {
    u8 rslt;
    SDK_MSG *lpMsg = NULL;

    HidenMenuShowMainFace(pasOutInfo, pTitle);

    Verify(sdkSysPackMessage(OnCircle, 0, 0) == SDK_OK);
    Verify(sdkSysPackMessage(LoopSanf, 0, 0) == SDK_OK);

    while (sdkSysQueryMessage(&lpMsg)) {
        rslt = lpMsg->pFun(lpMsg);

        if (rslt == SDK_MSG_KEEP) {
            Verify(sdkSysPostMessage(lpMsg) == SDK_OK);
        }
        else if (rslt == SDK_MSG_OUT) {
            sdkFreeMem(lpMsg);
            return SDK_ERR;
        }
    }

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.01.31
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP MenuManaulMap[] =
        {
                {"��ʽ�˵�����", Test_sdkAviMenuTest},
                {"��ʽ�˵�����", Test_sdkHidenMenuTest},
        };

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.13
** Remarks:
*****************************************************************************/
TEST_MAP_INFO GetMenuManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(MenuManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = MenuManaulMap;
    return ret;
}

