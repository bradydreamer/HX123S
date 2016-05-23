#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304
#define WELCOMELOGO "welcomelogo"


/*****************************************************************************
 �� �� ��  : ShowLogo
 ��������  : ��ʾLOGO
             LOGO�ļ��Ȳ���/mtd0/res/[aid��λӢ��]welcomelogo[��Ļ�ֱ���].bmp
             û������ /mtd0/res/welcomelogo[��Ļ�ֱ���].bmp
 �������  : u32 uiX  ��ʼ����
             u32 uiY  ��ʼ����
 �������  : ��
 �� �� ֵ  :
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��3��7��
    ��    ��   : zhangjun
    �޸�����   : �����ɺ���
*****************************************************************************/
static void ShowLogo(u32 uiX, u32 uiY) {
    SDK_DISP_PIXEL pixel;
//    u8 asAppId[64] = {0};
    u8 fn[256] = {0};
    u8 asPixel[64] = {0};
    s32 siXLen = 0;
    s32 siYLen = 0;

    memset(&pixel, 0, sizeof(SDK_DISP_PIXEL));
    memset(fn, 0, sizeof(fn));
    memset(asPixel, 0, sizeof(asPixel));

    pixel = sdkDispGetScreenPixel();
    Trace("pixel", "siX=[%d] siY=[%d]\r\n", pixel.siX, pixel.siY);

//    sdkSysGetCurAppId(asAppId);
//    Trace("FillNewLogo", "asAppId=[%s]\r\n", asAppId);

    if ((pixel.siX == 128 && pixel.siY == 64) //ʹ�õ�ͼΪ64��48
        || (pixel.siX == 320 && pixel.siY == 240) //ʹ�õ�ͼΪ160��120
        || (pixel.siX == 800 && pixel.siY == 480)) //ʹ�õ�ͼΪ320��240
    {
        if (pixel.siX == 128 && pixel.siY == 64) {
            siXLen = 64;
            siYLen = 48;
        }
        else if (pixel.siX == 320 && pixel.siY == 240) {
            siXLen = 160;
            siYLen = 120;
        }
        else if (pixel.siX == 800 && pixel.siY == 480) {
            siXLen = 320;
            siYLen = 240;
        }
        else {
            siXLen = pixel.siX / 2;
            siYLen = pixel.siY / 2;
        }

        sprintf(asPixel, "%d%d", pixel.siX, pixel.siY);

        strcpy(fn, "/mtd0/res/");
        //lilin 20140409
//        strcat(fn, gasCurAppAid);
        strcat(fn, WELCOMELOGO);
        strcat(fn, asPixel);
        strcat(fn, ".bmp");

        if (!sdkAccessFile(fn)) {
            memset(fn, 0, sizeof(fn));
            strcpy(fn, "/mtd0/res/");
            strcat(fn, WELCOMELOGO);
            strcat(fn, ".bmp");
            siXLen = pixel.siX;
            siYLen = pixel.siY;
        }

        Trace("FillNewLogo", "fn=[%s]\r\n", fn);

        if (sdkAccessFile(fn)) {
            sdkDispShowBmp(uiX, uiY, siXLen, siYLen, fn);   //��ʾlogo
        }
    }

}

/*****************************************************************************
** Descriptions:	��ʾ����
** Parameters:    	const u8 *pasBmpFileName
** Returned value:	
** Created By:		������2013.12.09
** Remarks: 		//lilin 20131214
*****************************************************************************/
s32 ShowBackground(const u8 *pasBmpFileName) {
    SDK_DISP_PIXEL pixel;

    //�ж��ļ��Ƿ���
    if (sdkAccessFile((u8 *) pasBmpFileName)) {
        memset(&pixel, 0, sizeof(SDK_DISP_PIXEL));
        pixel = sdkDispGetScreenPixel();
        return sdkDispSetBackground(0, 0, pixel.siX, pixel.siY, pasBmpFileName);
    }
    return SDK_PARA_ERR;
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

/*****************************************************************************
** Descriptions:	������
** Parameters:          void
** Returned value:	void
** Created By:		����2012.07.27
** Remarks:
*****************************************************************************/
s32 MainMenu(void) {
//    SDK_DISP_PIXEL pixel;

    u8 btName[68] = {"������: "};
    u8 tmpBtname[68] = {0};
    int retVal = 0;


    Trace("mpos", "go into MainMenu\r\n");
//    memset(&pixel, 0, sizeof(SDK_DISP_PIXEL));
    pgstTradeData->ucTradeId = TRADEID_MAINMENU;

    sdkUIClearItems();
    sdkUISetShowMode(SDK_UI_SHOW_MODE_1_ROW);

    Verify(sdkUIAddHideItem((s32) SDK_KEY_FUNCTION, OnSysSetting) >= 0);

#if 0
    Verify(sdkUIAddHideItem((s32)SDK_KEY_1, TestOnOpenMagCardDevice) >= 0);
    Verify(sdkUIAddHideItem((s32)SDK_KEY_2, TestOnCloseMagCardDevice) >= 0);
    Verify(sdkUIAddHideItem((s32)SDK_KEY_3, TestOnReadMagCard) >= 0);
    Verify(sdkUIAddHideItem((s32)SDK_KEY_4, TestOnReadMagCardEncrypt) >= 0);
    //Verify(sdkUIAddHideItem((s32)SDK_KEY_5, DrawText) >= 0);

    Verify(sdkUIAddHideItem((s32)SDK_KEY_6, TestOnDataEncrypt) >= 0);
    Verify(sdkUIAddHideItem((s32)SDK_KEY_7, TestOnMAC) >= 0);
    //Verify(sdkUIAddHideItem((s32)SDK_KEY_8, TestOnGetDateTime) >= 0);
    //Verify(sdkUIAddHideItem((s32)SDK_KEY_9, TestOnSetDateTime) >= 0);
#endif
//	sdkUIShowMenu();

    //-----------------------------------------------------
    sdkDispClearScreen();
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304
    ShowBackground(BACKGROUNDBMP);

//    giAutoRetTimer = sdkTimerGetId();//Xiaox 15-1-12 ���ڽ��յ�ָ��ǰ
    if (ShowBackground(WELCOMBMP) != SDK_OK) ShowLogo(0, 0);
//    pixel = sdkDispGetScreenPixel();
//	sdkDispClearScreen();
#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

    //sdkDispFillRowRam(SDK_DISP_LINE2-ROW_OFFSET, 0, "�й���������", gDisplayMode == 0?SDK_DISP_DEFAULT: SDK_DISP_NOFDISP| SDK_DISP_CDISP|SDK_DISP_INCOL);//Xiaox 15-1-14 for test
    sdkDispFillRowRam(SDK_DISP_LINE2 - ROW_OFFSET, 0, "��ӭʹ��ȫ����POS",
                      gDisplayMode == 0 ? SDK_DISP_DEFAULT : SDK_DISP_NOFDISP | SDK_DISP_CDISP |
                                                             SDK_DISP_INCOL);//Xiaox 15-1-14 for test        //20150324 �����滻

    if (BLUETOOTH == gucCurCommMode) {
        retVal = sdkBtGetName(tmpBtname, sizeof(tmpBtname) - 1);

        if (retVal == SDK_OK)//2015-01-08
        {
            strcat(btName, tmpBtname);
            sdkDispFillRowRam(SDK_DISP_LINE3 - ROW_OFFSET, 0, btName,
                              gDisplayMode == 0 ? SDK_DISP_DEFAULT : SDK_DISP_NOFDISP |
                                                                     SDK_DISP_CDISP |
                                                                     SDK_DISP_INCOL);//Xiaox 15-1-14 for test
        }
        else {
            //Trace("Lai","sdkBtGetName error:%d\r\n",retVal);
        }
    }


#ifdef XGD_SDK_DEBUG
    sdkDispFillRowRam(SDK_DISP_LINE4-ROW_OFFSET, 0, "���԰汾,��������", gDisplayMode == 0?SDK_DISP_DEFAULT: SDK_DISP_NOFDISP| SDK_DISP_CDISP|SDK_DISP_INCOL);//Xiaox 15-1-14 for test
#else
    //20150402 ��ʾCSN�ͻ����к�, ���Է��ڰ汾��Ϣ��
    //sdkDispFillRowRam(SDK_DISP_LINE4-ROW_OFFSET, 0, MposDeviceId, gDisplayMode == 0?SDK_DISP_DEFAULT: SDK_DISP_NOFDISP| SDK_DISP_CDISP|SDK_DISP_INCOL);//Xiaox 15-1-14 for test
#endif
    DispIcon();
    DispSysRtc(SDK_DISP_LINE5 - ROW_OFFSET, false);
    sdkDispBrushScreen();
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������
** Parameters:          void
** Returned value:	void
** Created By:		����2012.07.27
** Remarks:
*****************************************************************************/
s32 MainFace(void) {
    //Trace("","go into MainFace\r\n");

    ClearOthers();
    if (pgstAppSetting->stComsetting.bisEL) {
        sdkSysBlackLightCtro(true);
    }
//    sdkPEDCancel();
    if (0 == gsiDCTimer)      //20150401
        MainMenu();

    return SDK_OK;
}

#if 0 /*del by lilin at 20140624 */
/*****************************************************************************
** Descriptions:	��ӭ����
** Parameters:    	s32 index
** Returned value:	
** Created By:		������2013.12.09
** Remarks: 		
*****************************************************************************/
SDK_UI_EXIT_VALUE PostShowWelcome(s32 index)
{
    SDK_DISP_PIXEL pixel;
    u8 temp[64] = {0};
    memset(&pixel, 0, sizeof(SDK_DISP_PIXEL));
    
    pgstTradeData->ucTradeId = TRADEID_WELCOME;
    if (pgstAppSetting->stComsetting.uiIdleCntDown > 0 && !sdkSysIsExPower())
    {
        giIdleClockTimer = sdkTimerGetId();
    }

    sdkUISetShowMode(SDK_UI_SHOW_MODE_1_ROW);
    sdkUIClearItems();
    sdkUIAddHideItem((s32)SDK_KEY_ENTER, MainFace);
 
    if (ShowBackground(WELCOMBMP) != SDK_OK) ShowLogo(0, 0);
    pixel = sdkDispGetScreenPixel();
    memset(temp,0,sizeof(temp));
    sdkDispFillRowRam(SDK_DISP_LINE2, pixel.siX / 2, "�й�����", SDK_DISP_LEFT_DEFAULT);
    memset(temp, 0, sizeof(temp));

#ifdef XGD_SDK_DEBUG
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, "���԰汾,��������", SDK_DISP_DEFAULT);
#endif
    DispIcon();
    DispSysRtc(SDK_DISP_LINE5, false);
    sdkDispBrushScreen();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
** Descriptions:	ѡ����뻶ӭ����
** Parameters:    	s32 index
** Returned value:	
** Created By:		������2013.12.09
** Remarks: 		
*****************************************************************************/
SDK_UI_EXIT_VALUE SelectPostShowWelcome(s32 index)
{
    if (pgstAppSetting->stComsetting.bisEL)
    {
        sdkSysBlackLightCtro(true);
    }
    return PostShowWelcome(index);
}
#endif /* if 0 */

