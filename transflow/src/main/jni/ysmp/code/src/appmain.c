#define AFX_APP_VARIABLES
#undef AFX_APP_VARIABLES
//������//white list

volatile u32 gScreenTimer; //ljh2015-01-07 ������Ļ������ж�
volatile bool gIsScreenOn; //��Ļ�Ƿ�����

bool gIsMachineK200P; //�ж���G2����K200P

static char const *pWhiteList[] =
        {
                //"lilin",
                //"commu",
                //"mpos",
                //"emv",
                //"zcl",
                //"dcq",
                "Mai",
                //"Lai",
//    "print",	
                //"YC",
        };


//������//black list
static char const *pBlackst[] =
        {
                "sdkmmi",
                "bt_test",
        };

static const SDK_DEBUG_FILTER DebugFilter =
        {
                true,                            //�Ƿ��ڲ���״̬��//whether in testing state
                true,                             // �Ƿ����������//whether output function name
                SDK_DEBUG_TARGET_COM,             // ������Ϣ���������//debug massage output to serial port
                SDK_DEUBG_OUT_ALL,
                //SDK_DEUBG_OUT_WHITE_LIST,       //trace����ģʽ//trace filtering mode
                //SDK_DEUBG_BLOCK_ALL,
        };


#define         SDK_MSG_OUT             0                       //����Ϣ������Ϻ�Ӷ������
#define         SDK_MSG_KEEP            1                       //����Ϣ������Ϻ��ƶ�������β��


/*****************************************************************************
** Descriptions:	��ȫ��ָ������ڴ�
** Parameters:          void
** Returned value:
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
static bool AppInitGlobal(void) {
    //ϵͳ����
    pgstSys = (struct _SYS *) sdkGetMem(sizeof(struct _SYS));

    if (NULL == pgstSys) {
        Assert(0);
        return false;
    }
    memset(pgstSys, 0, sizeof(struct _SYS));
    //���ò���
    pgstAppSetting = (struct _AppSetting *) sdkGetMem(sizeof(struct _AppSetting));

    if (NULL == pgstAppSetting) {
        Assert(0);

        if (pgstSys)                     //20150112 modify by yc sdkFreeMem
        {
            sdkFreeMem(pgstSys);
            pgstSys = NULL;
        }
        return false;
    }
    memset(pgstAppSetting, 0, sizeof(struct _AppSetting));

    pgstTradeData = (struct _TradeData *) sdkGetMem(sizeof(struct _TradeData));

    if (NULL == pgstTradeData) {
        Assert(0);

        if (pgstSys)                     //20150112 modify by yc sdkFreeMem
        {
            sdkFreeMem(pgstSys);
            pgstSys = NULL;
        }

        if (pgstAppSetting)              //20150112 modify by yc sdkFreeMem
        {
            sdkFreeMem(pgstAppSetting);
            pgstAppSetting = NULL;
        }
        return false;
    }
    memset(pgstTradeData, 0, sizeof(struct _TradeData));
    return true;
}

/*****************************************************************************
** Descriptions:	�ͷ�ȫ��ָ���ڴ�
** Parameters:          void
** Returned value:
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
static bool AppFreeGlobal(void) {
    if (NULL != pgstSys) {
        sdkFreeMem(pgstSys);
        pgstSys = NULL;
    }

    if (NULL != pgstAppSetting) {
        sdkFreeMem(pgstAppSetting);
        pgstAppSetting = NULL;
    }

    if (NULL != pgstTradeData) {
        sdkFreeMem(pgstTradeData);
        pgstTradeData = NULL;
    }
    return true;
}

/*****************************************************************************
   ��������: Ӧ�õ�һЩͨ�����õĳ�ʼ��
   �������:
   �������:
   �� �� ֵ:
   ����	 : ljh 2015-01-09
*****************************************************************************/
void AppCommSettingInit() {
    s32 len;
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150309
    u8 MachineModel[20] = {0};
#endif			//DEL_CODE_EXCESS	  ɾ���������: 20150309

    memset(&gCommSetting, 0, sizeof(gCommSetting));

    len = sizeof(gCommSetting);
    sdkReadFile(CommuSettingFile, &gCommSetting, 0, &len);

    Trace("Lai", "gCommSetting.commuType %d\n", gCommSetting.commuType[0]);

    //Ŀǰֻ֧��������,�����������Ĭ��������
    if (gCommSetting.commuType[0] >= BLUETOOTH && gCommSetting.commuType[0] <=
                                                  USB_HID)                                                           //����ͨѶ��ʽ
    {
        gucCurCommMode = gCommSetting.commuType[0];
    }
    else {
        gucCurCommMode = BLUETOOTH;
    }
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150309
    sdkSysGetMachineModel(MachineModel);

    if (strcmp(MachineModel, "200Q") == 0) {
        gIsMachineK200P = true;
    }
#else
    gIsMachineK200P = true;
#endif			//DEL_CODE_EXCESS	  ɾ���������: 20150309

#if    PRINT_ABLE                //��ӡ����: �������ε���ӡ��صĴ���
    InitFont();
#endif	//PRINT_ABLE	��ӡ����: �������ε���ӡ��صĴ���    
}

#define  COUNTDOWN_FLG      1           //����ʱ�꿪��

/*****************************************************************************
** Descriptions:	ɨ������
** Parameters:          SDK_MSG *lpMsg
** Returned value:	������  SDK_MSG_KEEP RETURN: must be SDK_MSG_KEEP
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
static u8 LoopScanf(SDK_MSG *lpMsg) {
    s32 key = 0;

    if (sBisTradeFlow) {
        return SDK_MSG_KEEP;              //����н�����������ִ�У�����İ�������Ӧ
    }
    key = sdkKbGetKey();

#if COUNTDOWN_FLG
    if ((gsiDCSecond) && (SDK_KEY_ENTER == key || SDK_KEY_ESC == key)) {
        gsiDCSecond = 0;
        gsiDCKey = key;
        gsiDCTimer = 0;
        //gsiDCRow = 0;
    }
#endif

    //Trace("Mpos","LoopScanf 1\n");
    //PrintCurTime();
    if (key != SDK_KEY_ERR) {
        if (gIsMachineK200P) {
            sdkSysBlackLightOn();
        }
        sdkSysBeep(SDK_SYS_BEEP_OK);

//�����ε���,�������þͰ�F���� ljh2015-01-09
#if 0

        if(key == SDK_KEY_5)
        {
            if(!JudgeTradeFlow())
            {
                sdkSysPackMessage(SysetDevice, 0, 0);
            }
        }
#endif
//		else if(key==SDK_KEY_0)
//		{
//			//SDK_SYS_MACHINE_K820
//			//u8 pstr[50]={0};
//			//int ret = sdkSysGetMachineModel(pstr);
//			//Trace("Lai","&&&&& sdkSysGetMachineModel:%d ,%s\n",ret,pstr);
//		}
//		else
        sdkUIRevKey(key);

        gScreenTimer = sdkTimerGetId();        //ljh2015-01-07
        gIsScreenOn = true;
    }

#if SDK_MEM_DEBUG /*����ڴ��Ƿ����й©�Ĵ���:Modify by ���ƽ at 2015.04.07  9:54 */
    if( key == SDK_KEY_F1 )
    return SDK_MSG_KEEP + 100;
#endif /* if 0 */

    //Trace("Mpos","LoopScanf 2\n");
    //PrintCurTime();
    return SDK_MSG_KEEP;
}

/*****************************************************************************
** Descriptions:	ʱ��ѭ��
** Parameters:          SDK_MSG *lpMsg
** Returned value:	������  SDK_MSG_KEEP	RETURN: must be SDK_MSG_KEEP
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
static u8 OnCircle(SDK_MSG *lpMsg) {
    static u32 siSoftClockTimer = 0; //siStatusBarTimer=0;
    static u32 uiTradeAutoRetTimer = 0; //syh 20150327 �������̳�ʱ��ʱ��,���ڽ����ѷ��������������


    //Trace("Mpos","OnCircle 1\n");
    //PrintCurTime();
    if (siSoftClockTimer == 0) {
        siSoftClockTimer = sdkTimerGetId();
    }

    if (sdkTimerIsEnd(siSoftClockTimer, 900) &&
        siSoftClockTimer != 0) //zxx 20130819 14:1����Ӧ����1000ms�ģ�����������
    {
        //Trace("zcl", "siSoftClockTimer JudgeTradeFlow  = %d pgstTradeData->ucTradeId = %d\r\n", sBisTradeFlow, pgstTradeData->ucTradeId);


#if COUNTDOWN_FLG       //����ʱ��ͳһ�ĺú����ã������Ȳ���
        s32 now = sdkTimerGetId();
        //Trace("zcl", "second:%d Timer:%d Now:%d Row:%d\r\n", 
        //            gsiDCSecond, gsiDCTimer, now, gsiDCRow);
        //if( gsiDCSecond && gsiDCTimer )
        if (gsiDCSecond) {
            s32 sec = (now - gsiDCTimer) / 1000;
            //Trace("zcl", "second:%d Timer:%d Now:%d Row:%d sec:%d\r\n",
            //    gsiDCSecond, gsiDCTimer, now, gsiDCRow, sec);

            if (sec >= gsiDCSecond) {
                MainFace();
                gsiDCSecond = 0;
                gsiDCKey = 0;
                gsiDCTimer = 0;
                gsiDCRow = 0;
            }
            else if ((gsiDCRow >= SDK_DISP_LINE1) && (gsiDCRow <= SDK_DISP_LINE5)) {
#if 0       //����ʱ����λ��������ˣ���ʱ������ʾ����ʱ��
                u8  tbuf[32] = {0};
                sdkDispClearRow(gsiDCRow);
                sprintf(tbuf, "<����ʱ%ds>", gsiDCSecond-sec);
                sdkDispFillRowRam(gsiDCRow, 0, tbuf, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
#endif
            }
        }
        else if ((gsiDCRow >= SDK_DISP_LINE1) && (gsiDCRow <= SDK_DISP_LINE5)
                 && ((SDK_KEY_ENTER == gsiDCKey) ||
                     (SDK_KEY_ESC == gsiDCKey)))     //���ⰴ����û�л�������
        {
            MainFace();
            gsiDCKey = 0;
            gsiDCRow = 0;
        }
        else
#endif
            //if(!JudgeTradeFlow())       //���ǽ�������		//20150313 ע�͵�
            //{
        if (pgstTradeData->ucTradeId == TRADEID_MAINMENU) {
            DispIcon();
            DispSysRtc(SDK_DISP_LINE5 - ROW_OFFSET, false);

            if (uiTradeAutoRetTimer == 0 &&
                JudgeTradeFlow())    //syh 20150327 add �Դ��ڽ���״̬ʱ������ʱ��
            {
                uiTradeAutoRetTimer = sdkTimerGetId();
            }

            if (uiTradeAutoRetTimer != 0 && sdkTimerIsEnd(uiTradeAutoRetTimer, 30 *
                                                                               1000))  //syh 20150327 add 30s��ʱ��,�رս���״̬���ö�ʱ��
            {
                sBisTradeFlow = false;
                uiTradeAutoRetTimer = 0;
                MainFace();
            }
        }
        else  //syh 20150327 add  ��������������ʱ,��ʱ��������
        {
            uiTradeAutoRetTimer = 0;
        }
        //}
        siSoftClockTimer = sdkTimerGetId();
    }

    AppCommPollQueryAndExe();

    //ljh2015-01-07 ��ӹر���Ļ��ʱ��
    if (sdkTimerIsEnd(gScreenTimer, 60000) && gScreenTimer != 0) {
        //Trace("zcl", "gScreenTimer JudgeTradeFlow = %d pgstTradeData->ucTradeId = %d\r\n", sBisTradeFlow, pgstTradeData->ucTradeId);

        gScreenTimer = 0;
        if (!JudgeTradeFlow())       //���ǽ�������
        {
            if (pgstTradeData->ucTradeId == TRADEID_MAINMENU) {
                sdkSysBlackLightOff();
                gIsScreenOn = false;
                Trace("Lai", "LCD....................offffffffff\r\n");
                if (!sdkSysIsExPower())                //20150324 �������ж�
                {
                    //20150309 add by yc ����͹���	
                    //��Ļ��ʾ"����ģʽ"���ޱ��⣬����ʱ���ѣ�������
                    //��Ļ��ʾ"����ģʽ"���ޱ��⣬����ʱ���ѣ�������
                    sdkDispClearScreen();
                    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "����ģʽ", SDK_DISP_DEFAULT);
                    sdkDispBrushScreen();

                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

                    sdkSysEnterIdle(1, 1);
                    MainFace();
                    //gScreenTimer= sdkTimerGetId();		//20150309  �����˳��͹��ĺ����κβ��������ٴν���͹���
                }
                //gScreenTimer�����������Ϊ����ʱ�䵽����ӵ�Դʱ����ʱ�ε��������û�а������׾Ͳ����ٴν���͹���
                gScreenTimer = sdkTimerGetId();        //20150309  �����˳��͹��ĺ����κβ��������ٴν���͹���
            }
        }
    }

    if (sdkTimerIsEnd(giAutoRetTimer, 5000) && giAutoRetTimer != 0) {
        Trace("zcl", "giAutoRetTimer JudgeTradeFlow  = %d pgstTradeData->ucTradeId = %d\r\n",
              sBisTradeFlow, pgstTradeData->ucTradeId);

        if (!JudgeTradeFlow())       //���ǽ�������
        {
#if COUNTDOWN_FLG
            if (0 == gsiDCSecond)  //����ʱ��Чʱ������
#endif
                MainFace();
        }
        giAutoRetTimer = 0;
    }
    //Trace("Mpos","OnCircle 3\n");
    //PrintCurTime();
    return SDK_MSG_KEEP;
}

/*****************************************************************************
** Descriptions:	��Ϣ����
** Parameters:          void
** Returned value:
** Created By:		lilin2014.06.23
** Remarks:
*****************************************************************************/
static void AppSysmain(void) {
    u8 rslt;
    SDK_MSG *lpMsg = NULL;

    while (sdkSysQueryMessage(&lpMsg)) {
        //Trace("zjp", "######################  lpMsg->uiLParam = %d ######################\r\n", lpMsg->uiLParam );
        rslt = lpMsg->pFun(lpMsg);

        if (rslt == SDK_MSG_KEEP) {
            Verify(sdkSysPostMessage(lpMsg) == SDK_OK);
        }
        else if (rslt == SDK_MSG_OUT) {
            Verify(sdkFreeMem(lpMsg) == SDK_OK);
        }
#ifdef SDK_MEM_DEBUG /*����ڴ��Ƿ����й©�Ĵ���:Modify by ���ƽ at 2015.04.07  9:53 */
        else if( rslt == SDK_MSG_KEEP + 100 )
        {
            break;
        }
#endif /* if 0 */
    }
}

void InitBt() {
    u8 btname[64] = {0};
    u8 buf[64] = {0};
    u8 sn[7] = {0};

    sdkReadPosSn(buf);

    //syh 20150413 modify �������Ƹ�ʽ: B-�豸�ͺ�-SN����λ
    memset(sn, '0', 7);
    if (strlen(buf) <= 0) {
        strcpy(sn, "000000");
    }
    else if (strlen(buf) < 6) {
        memcpy(&sn[6 - strlen(buf)], buf, strlen(buf));
    }
    else {
        memcpy(sn, &buf[strlen(buf) - 6], 6);
    }

    if (gIsMachineK200P == true) {
        sprintf(btname, "B-K200-%s", sn);
    }
    else {
        sprintf(btname, "B-G2-%s", sn);
    }

    sdkBtOpen();
//    sdkBtGetName(btname, sizeof(btname) - 1);
    sdkBtSetName(btname);

//    if(strcmp(sn, btname))
//    {
//        sdkBtSetName(sn);
//    }
    sdkBtSetDiscoverable(true);
}

/*****************************************************************************
** Descriptions:	���������
** Parameters:          s32 argc
                               char const *argv[]
** Returned value:
** Created By:		lilin2014.06.18
** Remarks:
*****************************************************************************/
s32 appmain(s32 argc, char const *argv[]) {
    SDK_SYS_INITIAL_INFO stInitial;
    sdkmSleep(4000);
    while (1) { }
    Verify(sdkSysLoadDll(NULL, 0) == SDK_OK);
    Verify(sdkSysMain(argc, argv) == SDK_OK);       //�Ѿ��������豸

    Verify(sdkDebugInitalList(pWhiteList, sizeof(pWhiteList) / sizeof(char *), pBlackst,
                              sizeof(pBlackst) / sizeof(char *)) == SDK_OK);
    Verify(sdkDebugSetFilter(&DebugFilter) == SDK_OK);

    stInitial.bIsHavePinPad = false;    //have PIN pad or not
    stInitial.bIsINRf = false;   //have contactless or not
    sdkSysInitParam(&stInitial);

    Trace("lilin", "state log....\r\n");
    Verify(AppInitGlobal() == true);

    //sdkDispStateBar(true, SDK_SYS_LINE4);
#if 0 /*Modify by ���ƽ at 2015.04.08  17:12 */
    sdkDispStateBar(true);
#endif /* if 0 */
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ʼ��", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    //���ʱ��
    Trace("lilin", "���ʱ��...\r\n");
    CheckTime();
    sdkIccPowerStartInit();
    //��ʼ��aid �� capk ��ע�����ܱ���debug�棬�ռ�
    //#ifndef DEL_CODE_EXCESS				//ɾ���������: 20150304
    //InitMposAid();
    //InitMposCapk();
    //#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

    loadDefaultWK();                    //Mai add for debug 2014-12-19

    //����ϵͳ����
    Trace("lilin", "����ϵͳ����...\r\n");
#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304
    ReadAppPosCfg();
#else
    GetRtcSys();
#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304    


    SetCmdResetExe(FALSE);

    IsQmfMode = 0;
    ClearOthers();
    //2����פ��Ϣ

    gScreenTimer = sdkTimerGetId();    //ljh2015-01-07
    gIsScreenOn = true;
    AppCommSettingInit();


    ReadMposDeviceId();


#ifdef DEBUG_CODE_TEST

    //zcl add ��ʼ���豸��Ϣ
#if 1
    memcpy(MposDeviceId, "00011003180000579227", 20);
    memcpy(MposMainKey, "08B230226CEB196EA0D969A418E0D064", 32);
    memcpy(MposAuthKey, "3B9E64FCDA07DD1DB68F4DCF40A869FA", 32);
    memcpy(MposAppKey, "151F3867B8AEFC29B8E6BD8D772892B5", 32);
#else
    memcpy(MposDeviceId, "00011006180001299068", 20);
    memcpy(MposMainKey, "338E32E8D1FE7A39EE0AE72968EC1D34", 32);
    memcpy(MposAuthKey, "3D03C1B9EAD0ECC4B107FB15FDCB7EF7", 32);
    memcpy(MposAppKey, "EB0DC71237FA25A972AEB7E301A1443A", 32);
#endif


    u8 ucAuthKey[16];
    sdkAscToBcd(ucAuthKey, MposMainKey, 32);
    sdkPEDUpdateTmk (MAINKEYINDEX, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);
    sdkAscToBcd(ucAuthKey, MposAuthKey, 32);
    sdkPEDUpdateTmk (AUTHKEYINDES, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);
    sdkAscToBcd(ucAuthKey, MposAppKey, 32);
    sdkPEDUpdateTmk (APPKEYINDES, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);

#endif

    {
        u8 file[64] = {0};
        sprintf(file, "%s%s", FileNamePrefix, OFFLINEFILE);
        sdkDelFile(file);
    }

#if 0
    memcpy(MposPinKsn, "\x00\x00\x00\x00\x00\x00\x00\x88", 8);
    memcpy(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x88", 8);
    memcpy(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x88", 8);
#endif
    ReadMposKSN();
    //end

    Trace("lilin", "����פ��Ϣ...\r\n");
    sdkSysPackMessage(LoopScanf, 0, 0);
    sdkSysPackMessage(OnCircle, 0, 0);

    InitBt();

    MainFace();


    AppSysmain();

    sdkUIClearItems();//�ͷŲ˵������ƽ����
    AppFreeGlobal();
    Verify(sdkSysUnLoadDll() == SDK_OK);
    sdkSysAppQuit(); // main�����˳�ǰһ��Ҫ�����������
#ifdef SDK_MEM_DEBUG
    sdkIccTransFreeAllMem();
    sdkGetMemStatus();
#endif
    return 0;
}

