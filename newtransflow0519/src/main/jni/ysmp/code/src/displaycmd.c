#define SDK_RGB(ired, igreen, iblue)    (  (((u8)ired)<<16) | (((u8)igreen)<<8) | ((u8)iblue)  )
//��ʾ���Ŀǰֻ�г�ʱ��صĲŵ���FinishTradeFlow();

static Point gCurPos = {0, 0};        //���λ��
u8 gDisplayMode = 0;                     //������ģʽ 00����,01����
static u8 *gpMenuData = NULL; //����˵�����
static MenuRecStr *gpMenuRecord = NULL; //��¼�˵������Ĳ��� �������Ϣ
static bool gbXYPosTwoBytes = false;            //��ʾxy������ֽ���

extern bool gIsMachineK200P;
//16λRGB 565
#define R(a) ((a >> 11) & 0x1F)
#define G(a) ((a >> 5) & 0x3F)
#define B(a) (a & 0x1F)

enum {
    CONFIRM_KEY = 0, CANCEL_KEY = 0x1B
}; //ȷ�ϼ�,ȡ����

cmd_Str const DipCmdTab[] = {
        {DIP_CLEAR,          OnClear,         NULL},     //
        {DIP_SETMODE,        OnSetMode,       NULL},     //
        {DIP_SET_CURPOS,     OnSetCurPos,     NULL},     //
        {DIP_GET_CURPOS,     OnGetCurPos,     NULL},     //
        {DIP_GET_SCREENATTR, OnGetScreenAttr, NULL},    //
        {DIP_DRAW_BMP,       OnDrawBmp,       NULL},     //δʵ��
        {DIP_DRAW_LINE,      OnDrawLine,      NULL},     //��ɫ��Ҫ��16λת��32λ
        {DIP_DRAW_RECT,      OnDrawRect,      NULL},     //��ɫ
        {DIP_SET_BACKLIGHT,  OnSetBackLight,  NULL},     //������
        {DIP_DRAW_TEXT,      OnDrawText,      NULL},     //�����ܷ���
        {DIP_LOAD_MENU,      OnLoadMenu,      NULL},     //
        {DIP_CMD_END,        NULL,            NULL}
};

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ���������õ�ǰ����,��ʾģʽ
 ********************************************************************/
s32 OnClear(ST_TRANSFLOW *pstTransFlow) {
    GotoTradeFlow();

    pstTransFlow->usSendDataLen = 0;
    Trace("Lai", "enter OnClear=============\r\n");

    gCurPos.xPos = 0;
    gCurPos.yPos = 0;
    gDisplayMode = 0;

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150306
    //���״̬��
    sdkDispClearStateBar();
    sdkDispStateBar(TRUE);
#endif			//DEL_CODE_EXCESS	  ɾ���������: 20150306

    sdkDispClearScreen();
    sdkDispBrushScreen();
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ������ʾģʽ ����/����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetMode(ST_TRANSFLOW *pstTransFlow) {
    //��ָ�����ʱҪ������������з��Ե����ݣ��͸�ˢ����������
    GotoTradeFlow();
    Trace("Lai", "enter OnSetMode=============\r\n");

    if (pstTransFlow->heReceiveData[0] == 0x00)    //0x00 ����
    {
        gDisplayMode = 0x00;
    }
    else if (pstTransFlow->heReceiveData[0] == 0x01)    //0x01����
    {
        gDisplayMode = 0x01;
    }
    else {
        return INS_PARAM_ERR;
    }
    bisNeedBrushScreenWhenBack = true;    //Xiaox 15-1-14
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���õ�ǰ���λ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetCurPos(ST_TRANSFLOW *pstTransFlow) {
    u8 ucDispAtr = 0;
    u16 x, y;
    s32 width, high;
    SDK_DISP_MULT_DATA ctrl;

    GotoTradeFlow();
    Trace("Lai", "enter OnSetCurPos recvLen:%d=============\r\n", pstTransFlow->ReceiveDataLen);

    if (pstTransFlow->ReceiveDataLen == 4) {
        x = *(u16 * ) & pstTransFlow->heReceiveData[0];
        y = *(u16 * ) & pstTransFlow->heReceiveData[2];
        x = U16_ENDIAN(x);
        y = U16_ENDIAN(y);
        gbXYPosTwoBytes = true;
        sdkSysGetScreenWidthHigh(&width, &high);

        if (x > width - 1 || y > high - 1)    //����� 0,0��ʼ
        {
            return INS_PARAM_ERR;
        }
    }
    else {
        gbXYPosTwoBytes = false;
        x = pstTransFlow->heReceiveData[0];
        y = pstTransFlow->heReceiveData[1];
    }
    gCurPos.xPos = x;
    gCurPos.yPos = y;


    ctrl.siX = gCurPos.xPos;
    ctrl.siY = gCurPos.yPos;


    if (gDisplayMode == 0) {
        ucDispAtr = SDK_DISP_FDISP;         //����
    }
    else {
        ucDispAtr = SDK_DISP_NOFDISP;        //����
    }
    sdkSysGetScreenWidthHigh(&width, &high);
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150306
    if (width == 128 && high == 64) {
        //K200P
        ctrl.eAscFont = SDK_PRN_ASCII6X12;
        ctrl.eChFont = SDK_PRN_CH12X12;
    }
    else if (width == 320 && high == 240) {
        //G2
        ctrl.eAscFont = SDK_PRN_ASCII12X24;
        ctrl.eChFont = SDK_PRN_CH24X24;
    }
    else
#endif			//DEL_CODE_EXCESS	  ɾ���������: 20150306    
    {
        ctrl.eAscFont = SDK_PRN_ASCII6X12;
        ctrl.eChFont = SDK_PRN_CH12X12;
    }
    strcpy(ctrl.pasText, "_");

    sdkDispFormatFillRam(ucDispAtr, &ctrl, NULL);

    //sdkmSleep(5000);

    //�����
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ��ǰ���λ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnGetCurPos(ST_TRANSFLOW *pstTransFlow) {
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    GotoTradeFlow();

    Trace("Lai", "enter OnGetCurPos=============\r\n");
    u16 x = gCurPos.xPos;
    u16 y = gCurPos.yPos;

    if (gbXYPosTwoBytes) {
        pstTransFlow->usSendDataLen = 4;
        x = U16_ENDIAN(x);
        y = U16_ENDIAN(y);
        memcpy(&pSend[0], &x, 2);
        memcpy(&pSend[2], &y, 2);
    }
    else {
        pstTransFlow->usSendDataLen = 2;
        pSend[0] = x;
        pSend[1] = y;
    }
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ��Ļ��Ϣ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ����,�ڰ׌�
 ********************************************************************/
s32 OnGetScreenAttr(ST_TRANSFLOW *pstTransFlow) {
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    Trace("Lai", "enter OnGetScreenAttr=============\r\n");

    s32 width, high;
    u16 w, h;
    sdkSysGetScreenWidthHigh(&width, &high);

    w = width;
    h = high;

    w = U16_ENDIAN(w);
    h = U16_ENDIAN(h);

    pstTransFlow->usSendDataLen = 5;
    memcpy(&pSend[0], &w, 2);
    memcpy(&pSend[2], &h, 2);

    if (sdkSysIsColorScreen()) {
        pSend[4] = 0x01;
    }
    else {
        pSend[4] = 0x00;
    }
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��λͼ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnDrawBmp(ST_TRANSFLOW *pstTransFlow) {
    GotoTradeFlow();
    Trace("Lai", "enter OnDrawBmp recLen:%d=============\r\n", pstTransFlow->ReceiveDataLen);
    //xy����,ͼƬ���,�߶�,ͼƬ����LL1024����
    u16 uVal[4];
    u8 i;
    s32 ret;
    memcpy(uVal, pstTransFlow->heReceiveData, 8);

    for (i = 0; i < 4; i++) {
        uVal[i] = U16_ENDIAN(uVal[i]);
    }

    u32 bmpWidth = uVal[2];
    u32 bmpheight = uVal[3];
    u16 dataLen = 0;
    sdkBcdToU16(&dataLen, &pstTransFlow->heReceiveData[8], 2);
    SDK_DISP_SRECT dispRect = {uVal[0], uVal[1], uVal[0] + bmpWidth - 1, uVal[1] + bmpheight - 1};
    Trace("Lai", "xxxxxxxxxx %d %d %d %d\r\n", uVal[0], uVal[1], uVal[2], uVal[3]);

    if (SDK_OK != (ret = sdkDispShowBmpdots(dispRect, bmpWidth, bmpheight,
                                            &pstTransFlow->heReceiveData[10]))) {
        Trace("Lai", "$$$ DrawBmp fail ret:%d=============\r\n", ret);
    }
    //sdkmSleep(2000);
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ֱ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnDrawLine(ST_TRANSFLOW *pstTransFlow) {
    s32 ret;
    u32 color;
    u16 uVal[5];
    u8 i;

    GotoTradeFlow();

    Trace("Lai", "enter OnDrawLine=============\r\n");

    if (pstTransFlow->ReceiveDataLen < 10) {
        return INS_PARAM_ERR;
    }
    memset(&uVal[0], 0x00, sizeof(uVal));
    memcpy(uVal, pstTransFlow->heReceiveData, 10);

    for (i = 0; i < 5; i++) {
        sdkHexToU16(&uVal[i], &pstTransFlow->heReceiveData[2 * i], 2);
    }


    if (sdkSysIsColorScreen()) {
        color = SDK_RGB(R(uVal[4]), G(uVal[4]), B(uVal[4]));
    }
    else {
        if (uVal[4] == 0x00)      //��ɫ
        {
            color = SDK_RGB(255, 255, 255);
        }
        else {
            color = 0;
        }
    }
    //Trace("Lai","%d %d %d %d, %d\r\n",uVal[0],uVal[1],uVal[2],uVal[3],color);
    ret = sdkDispDrawLine(uVal[0], uVal[1], uVal[2], uVal[3], color);

    if (SDK_OK != ret) {
        Trace("Lai", "draw line fail ret:%d\r\n", ret);
        return INS_PARAM_ERR;
    }
    sdkDispBrushScreen();
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ������ ����߿�ģʽ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnDrawRect(ST_TRANSFLOW *pstTransFlow) {
    GotoTradeFlow();
    TraceHex("Lai", "enter OnDrawRect=============\r\n", pstTransFlow->heReceiveData, 11);

    u16 uVal[4];
    memcpy(uVal, pstTransFlow->heReceiveData, 8);
    u8 i;

    for (i = 0; i < 4; i++) {
        uVal[i] = U16_ENDIAN(uVal[i]);
    }

    //u8 mode = pstTransFlow->heReceiveData[8];
    u16 color16 = *(u16 *) pstTransFlow->heReceiveData[9];
    color16 = U16_ENDIAN(color16);

    u32 color;

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150309
    if (!gIsMachineK200P) {
        color = SDK_RGB(R(color16), G(color16), B(color16));
    }
    else {
        if (color16 == 0x00)      //��ɫ
        {
            color = SDK_RGB(255, 255, 255);
        }
        else {
            color = 0;
        }
    }
#else
    if(color16 == 0x00)      //��ɫ
    {
        color = SDK_RGB(255, 255, 255);
    }
    else
    {
        color = 0;
    }
#endif			//DEL_CODE_EXCESS	  ɾ���������: 20150309    
    //���xy����, ���εĿ��, ���/�߿�ģʽ, ��ɫֵ
    //�����xy����, �յ�xy����, ���/�߿�ģʽ, ��ɫֵ
    //SDK_DISP_SRECT stRect = {uVal[0], uVal[1], uVal[0] + uVal[2] - 1, uVal[1] + uVal[3] - 1};

#if 0 /*Modify by ���ƽ at 2015.03.23  16:30 */
    sdkDispDrawRect(&stRect, mode, color);  //mode
#endif /* if 0 */
    //sdkmSleep(1000);
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ñ���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetBackLight(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnSetBackLight=============\r\n");

    if (pstTransFlow->heReceiveData[0] == 0x00) {
        sdkSysBlackLightOff();
        sdkSysBlackLightCtro(TRUE);
    }
    else if (pstTransFlow->heReceiveData[0] == 0x01) {
        sdkSysBlackLightOn();
        Trace("Lai", "sdkSysBlackLightOnnnnnnnnnnn 2222222\n");
    }
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ʾ�ض�ʱ���ַ���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnDrawText(ST_TRANSFLOW *pstTransFlow) //�Ƿ������ʾ�����ֺ� ����һ����ʱ��������
{
    u8 ucDispAtr = 0;
    u8 ucTmp[] = {0x02, 0x00, 0x06, 0xA1, 0x0E, 0x2F, 0x03, 0x30, 0x30, 0x03, 0x86};
    u16 len = 0;
    static u8 OverTime = 0;  //ljh2015-01-12
    s32 iWidth, iHigh;
    SDK_DISP_MULT_DATA ctrl;

    GotoTradeFlow();
    //OverTime = pstTransFlow->heReceiveData[pstTransFlow->ReceiveDataLen];

    if (0 == pstTransFlow->flag1) {
        Trace("Lai", "enter OnDrawText=============\r\n");

        pstTransFlow->flag1 = 1;
        sdkBcdToU16(&len, pstTransFlow->heReceiveData, 2);

        if (len > 64) {
            len = 64;
            return INS_PARAM_ERR;
        }
        memset(ctrl.pasText, 0, sizeof(ctrl.pasText));
        memcpy(ctrl.pasText, &pstTransFlow->heReceiveData[2], len);

        OverTime = pstTransFlow->heReceiveData[len + 2];
        Trace("Lai", "drawtext timeout:%d,len:%d\r\n", OverTime, len);

        ctrl.siX = gCurPos.xPos;
        ctrl.siY = gCurPos.yPos;


        ucDispAtr |= SDK_DISP_LDISP | SDK_DISP_INCOL;

        if (gDisplayMode == 0) {
            ucDispAtr |= SDK_DISP_FDISP;
        }
        else {
            ucDispAtr |= SDK_DISP_NOFDISP;
        }
        sdkSysGetScreenWidthHigh(&iWidth, &iHigh);

        if (iWidth == 128 && iHigh == 64) {
            //K200P
            ctrl.eAscFont = SDK_PRN_ASCII6X12;
            ctrl.eChFont = SDK_PRN_CH12X12;
        }
        else if (iWidth == 320 && iHigh == 240) {
            //G2
            ctrl.eAscFont = SDK_PRN_ASCII12X24;
            ctrl.eChFont = SDK_PRN_CH24X24;
        }
        else {
            ctrl.eAscFont = SDK_PRN_ASCII6X12;
            ctrl.eChFont = SDK_PRN_CH12X12;
        }
        Trace("Mai", "ucDispAtr = %d \r\n", ucDispAtr);
        Trace("Mai", "ctrl.eAscFont = %d, ctrl.eChFont = %d \r\n", ctrl.eAscFont, ctrl.eChFont);

        //sdkDispClearScreen();
        sdkDispStateBar(false);        //�����õĻ���ʱ��ʾ��״̬���ϵ���Ϣˢ��ˢ����
        sdkDispFormatFillRam(ucDispAtr, &ctrl, NULL);
        pstTransFlow->InsClockTimer = sdkTimerGetId();

        SetCmdResetExe(FALSE);        //patch
        ucTmp[6] = pstTransFlow->ucNum;
        ucTmp[sizeof(ucTmp) - 1] = GetCRC(ucTmp + 1, sizeof(ucTmp) - 2);
        AppCommSendData(ucTmp, sizeof(ucTmp),
                        SENDDATA_TIMEOUT);        //�յ�ָ�����Ϸ��� //20150402 ԭ������ʱ��Ϊ2000
    }
    else {
        //�����Ҫ��ȷ��,�Ƿ���Ҫ���Ϸ��ػ��ǵ���ʱ��ŷ���
        //���Ϸ���,�����Ļ��ˢ����ʱʱ��͸�λ������,��ʱ������Ҫ��ʲô?
        //��ʱ����,�ֻ��˻᲻������Ϊָ��ִ�г�ʱû��ִ�гɹ�
        //OverTimeΪ0ʱ��һֱ��ʾ//sujianzhong 2015.01.14 0:58
        if ((SDK_KEY_ESC == sdkKbGetKey()) ||
            (OverTime && sdkTimerIsEnd(pstTransFlow->InsClockTimer, OverTime * 1000))) {
            bisNeedBrushScreenWhenBack = true;            //Xiaox 15-1-12

            FinishTradeFlow();
            sdkDispStateBar(true);
            sdkDispClearScreen();
            return INS_NOT_RETURN;
        }

        //�յ���λָ������Ҫ���ص�ǰָ���ִ�н�����Ǹ�λָ���ִ�н��?
        if (GetCmdResetExe() == TRUE) {
            Trace("wlz", "��Ӧ����λ  ��λ/ȡ��\r\n");
#if NEWCANCELCMD_MODE           //20150402 �µĸ�λ��ʽ
#else
            SetCmdResetExe(FALSE);
#endif
            FinishTradeFlow();
            sdkDispStateBar(true);
            sdkDispClearScreen();
            return INS_NOT_RETURN;                              //��Ӧȡ���˳�ʱ�ķ���ֵӰ�췵�������ȷ��
        }
    }
    return INS_KEEP;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�˵���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
char GetMenuItem(u16 usLvl, struct _MENUITEM *pstMenuItem, u8 *pucItemNum) {
    Trace("Lai", "enter GetMenuItem usLvl:%x=============\r\n", usLvl);

    u8 lvlnum = 0, index = 0;
    u16 i = 0, mask = 0xF000, nmask, maskpos, lvl;

    while (1) {
        if (!(usLvl & mask)) {
            break;
        }
        mask = mask >> 4;
        i++;
    }

    if (i > 3) {
        Trace("Lai", "$$$ enter GetMenuItem return -1=============\r\n");
        return -1;
    }
    maskpos = (3 - i) << 2;
    nmask = (~mask); //ȡ��

    i = 4;

    while (i < gpMenuRecord->MenuDataSize) {
        lvl = *(u16 * ) & gpMenuData[i]; //2byte�˵�����+1byte�¼��˵�����+20byte��ʾ����+5byte���״���
        i += 2;

        if ((lvl & mask) && ((lvl & nmask) == usLvl)) //�ҵ�ͬһ�����,usLv:0000,F000,0F00,00F0,����
        {
            index = (lvl >> maskpos) & 0x000f; //ȡ����ֵ

            if (index > 9) //���ֵΪ9
            {
                Trace("Lai", "$$$ enter GetMenuItem > 9 return -1 \r\n");
                return -1;
            }
            pstMenuItem[index - 1].usLvl = lvl;
            pstMenuItem[index - 1].bChildMenu = gpMenuData[i++];
            memcpy(pstMenuItem[index - 1].Disp, &gpMenuData[i], 20);
            //Trace("Lai","%s\r\n",pstMenuItem[index - 1].Disp);
            i += 20;
            memcpy(pstMenuItem[index - 1].TradeCode, &gpMenuData[i], 5);
            i += 5;

            lvlnum++;

            if (lvlnum >= 9) {
                //Trace("Lai","lvlnum >= 9 $$$$$$ \r\n");
                break;
            }
        }
        else {
            i += 26;
        }
    }

    *pucItemNum = lvlnum;
    return 0;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �˵������ص�,ȷ����,���ּ�,���·�ҳ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
SDK_UI_EXIT_VALUE MenuKeyCallBack(s32 siIndex) {
    Trace("Lai", "enter MenuKeyCallBack siIndex:%d MenuStep:%d\r\n",
          siIndex, gpMenuRecord->MenuStep);

    u8 ucMenuNo = (u8) siIndex;

    if (gpMenuRecord->stMenuItem[ucMenuNo].bChildMenu) {
        gpMenuRecord->MenuStep++; //������һ��˵�
        gpMenuRecord->MenuLvl[gpMenuRecord->MenuStep] = gpMenuRecord->stMenuItem[ucMenuNo].usLvl;
        Trace("Lai", "gpMenuRecord->MenuLvl[gpMenuRecord->MenuStep]:%d\r\n",
              gpMenuRecord->MenuLvl[gpMenuRecord->MenuStep]);

        gpMenuRecord->LevelChange = true;
        //gpMenuRecord->needReCalcTimeOut = true;
    }
    else {
        memcpy(gpMenuRecord->TradeCode, gpMenuRecord->stMenuItem[ucMenuNo].TradeCode, 5);
        Trace("Lai", "### tradeCode:%s\r\n", gpMenuRecord->stMenuItem[ucMenuNo].TradeCode);

        gpMenuRecord->keyValue = (ucMenuNo + 1);
        gpMenuRecord->isGetTradeCode = true;
    }
    Trace("Lai", "siIndex:%d MenuStep:%d\r\n", siIndex, gpMenuRecord->MenuStep);

    ShowMposMenu();

    return SDK_UI_EXIT_ENTER;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ȡ�����ص�,�˵�������һ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
SDK_UI_EXIT_VALUE EscKeyCb(s32 siIndex) {
    Trace("Lai", "enter EscKeyCb\r\n");

    if (gpMenuRecord->MenuStep > 0) {
        gpMenuRecord->MenuStep--; //������һ���˵�
        gpMenuRecord->LevelChange = true;
        ShowMposMenu();
    }
    else    //��һ���˵���ȡ��,�˳��˵��ص���������
    {
        gpMenuRecord->keyValue = CANCEL_KEY;
        gpMenuRecord->isGetTradeCode = true;        //�˳��˵�,��ʱ�Ľ��״���������
    }
    return SDK_UI_EXIT_ENTER;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ʾ�˵���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 DispMenuItem(struct _MENUITEM *pstMenuItem, u8 ucItemNum) {
    Trace("Lai", "enter DispMenuItem===========\r\n");

    u8 i = 0;
    pgstTradeData->ucTradeId = TRADEID_MPOS_MENU;
    gpMenuRecord->needReCalcTimeOut = true;

    sdkUIClearItems();
    sdkUISetShowMode(SDK_UI_SHOW_MODE_NOFDISP_1_ROW);

    while (i < ucItemNum) {
        sdkUIAddItem(pstMenuItem[i].Disp, MenuKeyCallBack);
        i++;
    }

    sdkUIAddHideItem((s32) SDK_KEY_ESC, EscKeyCb);
    sdkUIShowMenu();

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ʾ�˵�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void ShowMposMenu() {
    Trace("Lai", "enter ShowMposMenu =============\r\n");

    if (gpMenuRecord->isGetTradeCode) {
        //Trace("Lai","gpMenuRecord->isGetTradeCode return  $$$$$$$$$$$ \r\n");
        return;
    }

    if (NULL == gpMenuData) {
        //Trace("Lai","NULL == gpMenuData $$$$$$$$$$$ \r\n");
        return;
    }

    if (gpMenuRecord->LevelChange) {
        //Trace("Lai","gpMenuRecord->LevelChange true =============\r\n");

        gpMenuRecord->LevelChange = false;
        memset(gpMenuRecord->stMenuItem, 0, sizeof(gpMenuRecord->stMenuItem));

        signed char ret = GetMenuItem(gpMenuRecord->MenuLvl[gpMenuRecord->MenuStep], \
                                      gpMenuRecord->stMenuItem, &gpMenuRecord->CurLvlItems);

        //Trace("Lai","ret %d gpMenuRecord->stMenuItem[0].usLvl %d\r\n",ret,gpMenuRecord->stMenuItem[0].usLvl);

        if (ret < 0 || gpMenuRecord->stMenuItem[0].usLvl == 0) {
            return;
        }
    }
    //Trace("Lai","show menu Items:%d ##### \r\n",gpMenuRecord->CurLvlItems);
    DispMenuItem(gpMenuRecord->stMenuItem, gpMenuRecord->CurLvlItems);
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ղ˵���صĿռ���Դ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void FreeMenuResource() {
    if (NULL != gpMenuData) {
        sdkFreeMem(gpMenuData);
        gpMenuData = NULL;
    }

    if (NULL != gpMenuRecord) {
        sdkFreeMem(gpMenuRecord);
        gpMenuRecord = NULL;
    }
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ز˵�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnLoadMenu(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnLoadMenu=============\r\n");

    if (0 == pstTransFlow->ucInsRunStep) {
        GotoTradeFlow();
        pstTransFlow->ucInsRunStep = 1;

        if (NULL != gpMenuData) {
            sdkFreeMem(gpMenuData);
            gpMenuData = NULL;
        }

        if (NULL == gpMenuRecord) {
            gpMenuRecord = sdkGetMem(sizeof(MenuRecStr));
            memset(gpMenuRecord, 0, sizeof(MenuRecStr));
        }
        u16 len;
        gpMenuRecord->MenuTimeout = pstTransFlow->heReceiveData[0];
        gpMenuRecord->TotalMenuItems = pstTransFlow->heReceiveData[1];
        sdkBcdToU16(&len, &pstTransFlow->heReceiveData[2], 2);
        gpMenuRecord->MenuDataSize = len;
        gpMenuRecord->MenuDataSize += 4;        //timeout,items,LL
        gpMenuRecord->LevelChange = true;
        gpMenuRecord->isGetTradeCode = false;
        gpMenuRecord->needReCalcTimeOut = false;

        Trace("Lai",
              "timeout:%d TotalMenuItems %d MenuDataSize %d pstTransFlow->ReceiveDataLen %d\r\n",
              gpMenuRecord->MenuTimeout, gpMenuRecord->TotalMenuItems, gpMenuRecord->MenuDataSize,
              pstTransFlow->ReceiveDataLen);

        gpMenuData = (u8 *) sdkGetMem(gpMenuRecord->MenuDataSize);
        memcpy(gpMenuData, pstTransFlow->heReceiveData, gpMenuRecord->MenuDataSize);

        //TraceHex("Lai","Menu data:===========\r\n",\
        //              //			pstTransFlow->heReceiveData,pstTransFlow->ReceiveDataLen);

        pstTransFlow->InsClockTimer = sdkTimerGetId();
        ShowMposMenu();
    }
    else if (1 == pstTransFlow->ucInsRunStep) {
        if (gpMenuRecord->isGetTradeCode) {
            pstTransFlow->usSendDataLen = 6;
            pstTransFlow->heReceiveData[0] = gpMenuRecord->keyValue;               //0416Send==Recv
            memcpy(&pstTransFlow->heReceiveData[1], gpMenuRecord->TradeCode, 5);   //0416Send==Recv

            TraceHex("Lai", "### trade code\r\n", gpMenuRecord->TradeCode, 5);
            FreeMenuResource();
            FinishTradeFlow();
            return INS_SUC;
        }

        if (GetCmdResetExe() == TRUE) {
            Trace("Lai", "loadMenu reset\r\n");
#if NEWCANCELCMD_MODE           //20150402 �µĸ�λ��ʽ
#else
            SetCmdResetExe(FALSE);
#endif
            FinishTradeFlow();
            return INS_OVERTIME;
        }

        if (gpMenuRecord->needReCalcTimeOut) {
            gpMenuRecord->needReCalcTimeOut = false;
            pstTransFlow->InsClockTimer = sdkTimerGetId();
        }
        else {
            if (sdkTimerIsEnd(pstTransFlow->InsClockTimer, gpMenuRecord->MenuTimeout * 1000)) {
                FreeMenuResource();
                FinishTradeFlow();
                return INS_OVERTIME;
            }
        }
    }
    return INS_KEEP;
}

void DispWait() {
#if 0
    #if 0
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���׽�����...", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���Ժ�...", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    #else
    sdkDispClearScreen();    
    {     
        DispAmt("���:", SDK_DISP_LINE1, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        #if 0
        if (memcmp(gucAmount, "000000000000", 12))
        {
            u8  ucdispamount[32] = {0};
            u8  temp[12] = {0};
            strcpy(ucdispamount, "���:");
            sdkAscToBcd(temp, gucAmount, 12);
            RegulateMoney(&ucdispamount[5], temp);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucdispamount, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        }
        #endif
    }
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���״�����", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    if( SDK_ICC_ICC == GetCurrentCardType() )
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "...����ο�", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    #endif
    sdkDispBrushScreen();
#endif
}

