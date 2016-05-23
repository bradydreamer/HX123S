//���ڴ��һЩ����,SDK�Ѿ�����
//����Ӧ���޷�����ʹ��
COMM_SETTING_STR gCommSetting;


/*****************************************************************************
 ��������: ϵͳ���ò˵�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnSysSetting(s32 ix) {
    pgstTradeData->ucTradeId = TRADEID_SYS_SETTING_MENU;

    sdkUIClearItems();
    sdkUISetShowMode(SDK_UI_SHOW_MODE_1_ROW);
    sdkUISetTile("ϵͳ����");
    Verify(sdkUIAddItem("��������", BtSettingMenu) >= 0);
    Verify(sdkUIAddItem("ͨѶ��ʽ����", CommuSettingMenu) >= 0);
    Verify(sdkUIAddItem("IC����Կ", SetKeyCard) >= 0);
    Verify(sdkUIAddItem("�汾��Ϣ", VersionMenu) >= 0);    //20150317 ��Ӱ汾��ѯ

    Verify(sdkUIAddHideItem((s32) SDK_KEY_ESC, MainMenu) >= 0);
    Verify(sdkUIAddHideItem((s32) SDK_KEY_ENTER, MainMenu) >= 0);

    sdkUIShowMenu();
    giAutoRetTimer = sdkTimerGetId();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: �������ò˵�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
s32 BtSettingMenu() {
    pgstTradeData->ucTradeId = TRADEID_BT_SETTING_MENU;

    DispIcon();
    sdkUIClearItems();
    sdkUISetShowMode(SDK_UI_SHOW_MODE_1_ROW);
    sdkUISetTile("��������");

    Verify(sdkUIAddItem("�Ͽ�����", OnDisconnectBt) >= 0);
    Verify(sdkUIAddItem("������", OnOpenBt) >= 0);
    Verify(sdkUIAddItem("�ر�����", OnCloseBt) >= 0);
    Verify(sdkUIAddItem("�޸�����", OnSetBtNameAndPwd) >= 0);
    Verify(sdkUIAddItem("���ÿɼ�", OnSetBtDiscoverable) >= 0);

    Verify(sdkUIAddHideItem((s32) SDK_KEY_ESC, OnSysSetting) >= 0);

    sdkUIShowMenu();

    giAutoRetTimer = sdkTimerGetId();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: ������,�Ѵ����ٴ�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnDisconnectBt(s32 siIndex) {
    int ret = 0;
    u8 add[64] = {0};
    sdkDispClearScreen();
    ret = sdkBtGetStatus();
    Trace("Lai", "*** sdkBtGetStatus ret %d\n", ret);

    if (ret == SDK_BT_STATUS_WAITING) {
        sdkBtGetMacAddr(add, 64);
        sdkBtDisconnect(add);
        sdkDispPromptLabel(500, SDK_DISP_LINE3, SDK_DISP_LINE3, "�����ѶϿ�",
                           SDK_DISP_CDISP | SDK_DISP_FDISP);
    }
    else if (ret == SDK_EIO) {
        sdkDispPromptLabel(500, SDK_DISP_LINE3, SDK_DISP_LINE3, "����δ��",
                           SDK_DISP_CDISP | SDK_DISP_FDISP);
    }
    else {
        sdkDispPromptLabel(500, SDK_DISP_LINE3, SDK_DISP_LINE3, "����δ����",
                           SDK_DISP_CDISP | SDK_DISP_FDISP);
    }
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
    BtSettingMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: ������,�Ѵ����ٴ�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnOpenBt(s32 siIndex) {
    int ret = 0;

    sdkDispClearScreen();
    ret = sdkBtGetStatus();
    Trace("Lai", "*** sdkBtGetStatus ret %d\n", ret);

    if (!(ret == SDK_EIO || ret == SDK_ERR))//�Ѵ򿪾Ͳ��ٴ�
    {
        sdkDispPromptLabel(500, SDK_DISP_LINE3, SDK_DISP_LINE3, "�����Ѵ�",
                           SDK_DISP_CDISP | SDK_DISP_FDISP);
    }
    else {
        ret = sdkBtOpen();

        if (ret == SDK_OK) {
            sdkDispLabel(SDK_DISP_LINE3, "�����Ѵ�", SDK_DISP_CDISP | SDK_DISP_FDISP);
            sdkBtSetDiscoverable(true);
        }
        else {
            if (ret == SDK_EBUSY)
                sdkDispLabel(SDK_DISP_LINE3, "�豸æ", SDK_DISP_CDISP | SDK_DISP_FDISP);
            else
                sdkDispLabel(SDK_DISP_LINE3, "��ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
        }
    }
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
    BtSettingMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: �ر�����
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnCloseBt(s32 siIndex) {
    int ret = sdkBtClose();
    sdkDispClearScreen();
    if (ret == SDK_OK) {
        sdkDispLabel(SDK_DISP_LINE3, "�����ѹر�", SDK_DISP_CDISP | SDK_DISP_FDISP);
        //sdkBtSetDiscoverable(false);
    }
    else
        sdkDispLabel(SDK_DISP_LINE3, "�����ر�ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
    if (gucCurCommMode == BLUETOOTH)
        BtSettingMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: �����������ƺ�����
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnSetBtNameAndPwd(s32 siIndex) {
    s32 rslt = 0;
    s32 key = 0;
    SDK_BT_REMOTE_INFO btInfo;
    memset(&btInfo, 0, sizeof(btInfo));
    //sdkDispClearScreen();
    rslt = sdkBtGetStatus();
    Trace("Lai", "*** sdkBtGetStatus ret %d\n", rslt);

    if (rslt == SDK_EIO || rslt == SDK_ERR) {
        key = sdkDispMsgBox("����δ��,�Ƿ��?", "[ȷ��] ��\n[ȡ��] ��", 0,
                            SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
        if (key == SDK_KEY_ENTER) {
            if (SDK_OK == sdkBtOpen()) {
                if (SDK_OK == sdkBtSetDiscoverable(true))
                    sdkDispLabel(SDK_DISP_LINE3, "���óɹ�", SDK_DISP_CDISP | SDK_DISP_FDISP);
                else
                    sdkDispLabel(SDK_DISP_LINE3, "����ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
            }
            else
                sdkDispLabel(SDK_DISP_LINE3, "��ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
        }
        else {
            BtSettingMenu();
            return SDK_UI_EXIT_ENTER;
        }
    }
    else if (SDK_BT_STATUS_WAITING == rslt) {
        sdkDispLabel(SDK_DISP_LINE1, "������������״̬\n��Ͽ������޸�\n�Ƿ��ȶϿ�?\n[ȷ��] ��\n[ȡ��] ��",
                     SDK_DISP_LDISP | SDK_DISP_FDISP);

        key = sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 20000);

        if (key == SDK_KEY_ENTER) {
            sdkBtGetRemoteInfo(&btInfo);
            sdkBtDisconnect(btInfo.m_addr);
        }
        else {
            BtSettingMenu();
            return SDK_UI_EXIT_ENTER;
        }
    }

    //rslt = SetBtName();
    rslt = SDK_OK;
    if (rslt == SDK_OK) {
        rslt = SetBtPwd();
        if (rslt == SDK_OK)//ljh
            rslt = sdkBtSetDiscoverable(true);
    }
    if (rslt == SDK_EIO) {
        sdkDispPromptBox(false, TV_AUTORET, "��ȡ�����豸��Ϣʧ��\n��ȷ�������豸�Ƿ���δ����״̬");
        BtSettingMenu();
        return SDK_UI_EXIT_INTERNAL_ERR;
    }
    else if (rslt == SDK_ERR) {
        sdkBtClose();//ljh
        sdkDispPromptBox(false, TV_AUTORET, "��ȡ�����豸��Ϣʧ��\n���������豸�Ƿ�����");
        BtSettingMenu();
        return SDK_UI_EXIT_INTERNAL_ERR;
    }
    else {
        BtSettingMenu();
    }

    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: ���������ɱ����ҵ�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
SDK_UI_EXIT_VALUE OnSetBtDiscoverable(s32 siIndex) {
    int ret = 0;
    s32 key = 0;
    sdkDispClearScreen();
    ret = sdkBtGetStatus();
    Trace("Lai", "*** sdkBtGetStatus ret %d\n", ret);

    if (ret == SDK_EIO || ret == SDK_ERR) {

        key = sdkDispMsgBox("����δ��,�Ƿ��?", "[ȷ��] ��\n[ȡ��] ��", 0,
                            SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
        if (key == SDK_KEY_ENTER) {
            if (SDK_OK == sdkBtOpen()) {
                if (SDK_OK == sdkBtSetDiscoverable(true))
                    sdkDispLabel(SDK_DISP_LINE3, "���óɹ�", SDK_DISP_CDISP | SDK_DISP_FDISP);
                else
                    sdkDispLabel(SDK_DISP_LINE3, "����ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
            }
            else
                sdkDispLabel(SDK_DISP_LINE3, "��ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
        }
        else {
            BtSettingMenu();
            return SDK_UI_EXIT_ENTER;
        }
    }
    else {
        if (ret == SDK_BT_STATUS_IDLE) {
            ret = sdkBtSetDiscoverable(true);

            if (ret == SDK_OK)
                sdkDispLabel(SDK_DISP_LINE3, "���óɹ�", SDK_DISP_CDISP | SDK_DISP_FDISP);
            else
                sdkDispLabel(SDK_DISP_LINE3, "����ʧ��", SDK_DISP_CDISP | SDK_DISP_FDISP);
        }
        else if (ret == SDK_EBUSY)//��ʱ�ر�ok�� sdkBtGetStatusһֱ���豸æ
            sdkDispLabel(SDK_DISP_LINE3, "�豸æ", SDK_DISP_CDISP | SDK_DISP_FDISP);
        else
            sdkDispLabel(SDK_DISP_LINE3, "����״̬�޷�����", SDK_DISP_CDISP | SDK_DISP_FDISP);
    }

    sdkKbWaitKey(SDK_KEY_MASK_ALL, 500);
    BtSettingMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: ͨѶ��ʽ�����ò˵�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : ljh 2015-01-09
*****************************************************************************/
s32 CommuSettingMenu() {
    u8 currentMode[10] = {0};
    s32 key = 0;
    u8 preMode = gucCurCommMode;
    sdkDispClearScreen();

    switch (gucCurCommMode) {
        case BLUETOOTH:
            currentMode[0] = '1';
            break;
        case USB_HID:
            currentMode[0] = '2';
            break;
        case USB_CDC:
            currentMode[0] = '3';
            break;
        case AUDIO:
            currentMode[0] = '4';
            break;
    }

    sdkDispLabel(SDK_DISP_LINE1, "��ѡ��ͨѶ��ʽ:\n1.���� 2.HID \n3.CDC 4.��Ƶ",
                 SDK_DISP_LDISP | SDK_DISP_FDISP);

    sdkDispLabel(SDK_DISP_LINE5, currentMode, SDK_DISP_RDISP | SDK_DISP_FDISP);

    key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 |
                       SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                       SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 10000);
    switch (key) {
        case SDK_KEY_1:
            gucCurCommMode = BLUETOOTH;
            break;
        case SDK_KEY_2:
            gucCurCommMode = USB_HID;
            break;
        case SDK_KEY_3:
            gucCurCommMode = USB_CDC;
            break;
        case SDK_KEY_4:
            gucCurCommMode = AUDIO;
            break;
    }
    //д�ļ�

    if (preMode != gucCurCommMode) {
        Trace("Lai", "*** gucCurCommMode:%d\n", gucCurCommMode);
        currentMode[0] = key;
        sdkDispLabel(SDK_DISP_LINE5, currentMode, SDK_DISP_RDISP | SDK_DISP_FDISP);

        gCommSetting.commuType[0] = gucCurCommMode;
        sdkWriteFile(CommuSettingFile, &gCommSetting, sizeof(gCommSetting));

        if (preMode == BLUETOOTH)
            OnCloseBt(0);

        //syh 20150325 add ������ͨ��ģʽ�л�������ͨ��ģʽʱ��������������Ϊ��̽Ѱ
        if (preMode != BLUETOOTH && gucCurCommMode == BLUETOOTH) {
            sdkBtOpen();
            sdkBtSetDiscoverable(true);
        }
    }

    Trace("Lai", "*** CommuSettingMenu gucCurCommMode:%d\n", gucCurCommMode);
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 600);
    OnSysSetting(0);
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
 ��������: �汾�鿴�˵�
 �������: 
 �������: 
 �� �� ֵ: 
 ����	 : 
*****************************************************************************/
SDK_UI_EXIT_VALUE VersionMenu(s32 siIndex)        //20150317 ��Ӱ汾��ѯ
{
    s32 key = SDK_ERR;
    u8 core[64] = {0};
    u8 boot[64] = {0};
    u8 bt[64] = {0};
    u8 step = 0;

    giAutoRetTimer = sdkTimerGetId();
    sdkDispClearScreen();

    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "�汾��Ϣ", SDK_DISP_DEFAULT);

    sdkSysGetFirmwareVer(core, 2);
    sdkSysGetFirmwareVer(boot, 0);
    sdkBtGetVersion(bt, 17);

    while (1) {
        DispClearContentRam();
        switch (step) {
            case 0:             //��ʾӦ�ð汾
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "Ӧ�ð汾:", SDK_DISP_LEFT_DEFAULT);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, VERSION_R, SDK_DISP_LEFT_DEFAULT);
                break;
            case 1:             //��ʾ�ͻ����к�CSN
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ͻ����к�:", SDK_DISP_LEFT_DEFAULT);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, MposDeviceId, SDK_DISP_LEFT_DEFAULT);
                break;
            case 2:             //��ʾcore�汾
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "CORE�汾:", SDK_DISP_LEFT_DEFAULT);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, core, SDK_DISP_LEFT_DEFAULT);
                break;
            case 3:             //��ʾboot�汾
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "BOOT�汾:", SDK_DISP_LEFT_DEFAULT);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, boot, SDK_DISP_LEFT_DEFAULT);
                break;
            case 4:             //��ʾ�����汾
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�����汾:", SDK_DISP_LEFT_DEFAULT);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, bt, SDK_DISP_LEFT_DEFAULT);
                break;
        }
        key = sdkKbWaitKey(
                SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC | SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN,
                5000);           //4��û�����˳�
        if ((SDK_KEY_ENTER == key) || (SDK_KEY_DOWN == key))       //�¼�
        {
            step++;
            if (step > 4) step = 0;
        }
        else if (SDK_KEY_UP == key) {
            step--;
            if (step > 4) step = 4;
        }
        else {
            break;
        }
        sdkDispBrushScreen();
    }

    OnSysSetting(0);
    return SDK_UI_EXIT_ENTER;

#if 0
    //APP
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "A:");
    strcat(buf, VERSION_R);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, buf, SDK_DISP_LEFT_DEFAULT);
    Trace("Yc","%s\r\n", buf);

    //core
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "C:");
    sdkSysGetFirmwareVer(buf+strlen(buf), 2);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, buf, SDK_DISP_LEFT_DEFAULT);
    Trace("Yc","%s\r\n", buf);

    //boot
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "B:");
    sdkSysGetFirmwareVer(buf+strlen(buf), 0);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, buf, SDK_DISP_LEFT_DEFAULT);
    Trace("Yc","%s\r\n", buf);

    //bluetooth
    memset(buf, 0, sizeof(buf));
    strcpy(buf, "BT:");
    sdkBtGetVersion(buf+strlen(buf), 17);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, buf, SDK_DISP_LEFT_DEFAULT);
    Trace("Yc","%s\r\n", buf);

    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 5000);
    OnSysSetting(0);
    return SDK_UI_EXIT_ENTER;
#endif
}


