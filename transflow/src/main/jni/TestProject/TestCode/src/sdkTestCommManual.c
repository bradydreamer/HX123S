#include "sdkGlobal.h"
#include "SdkTest.h"

/********************************************************************
   ��    ��: �����
   ��    Ȩ: �������¹��������ɷ����޹�˾
   ��������: ���ô��ڽṹ�����
   �������:
   �������:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
********************************************************************/

static void SetCommUartParm(SDK_COMM_STUARTPARAM *pstUartInfo, char *pasOutInfo,
                            char const *pTitle) {
    memset(pstUartInfo, 0, sizeof(SDK_COMM_STUARTPARAM));
    pstUartInfo->ucCom = sdkTestInputU8(pTitle, "port number:");
    pstUartInfo->uiBaudrate = sdkTestInputU32(pTitle, "baudrate:");
}

/********************************************************************
   ��    ��: �����
   ��    Ȩ: �������¹��������ɷ����޹�˾
   ��������: �򿪴��ڲ���
   �������:
   �������:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
********************************************************************/
static s32 sdkCommUartTest(char *pasOutInfo, char const *pTitle) {
    s32 ret, len, revlen = 100;
    u8 tempsend[2048 + 1] = {0};
    u8 temprec[512] = {0}, temp[1024 + 1] = {0};

    static SDK_COMM_STUARTPARAM pstUartInfo = {0};

    for (len = 0; len < (sizeof(tempsend) - 1); len++) {
        sprintf(&tempsend[len], "%d", (len % 10));
    }

    if (0 == pstUartInfo.uiBaudrate)    //��һ��Ҫ���ò���
    {
        SetCommUartParm(&pstUartInfo, pasOutInfo, pTitle);         //���ò���
    }
    else    //��ʾ�Ƿ��������
    {
        if (SDK_KEY_2 ==
            sdkDispMsgBox(pTitle, "1.��������� \r2.�������", 0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2)) {
            SetCommUartParm(&pstUartInfo, pasOutInfo, pTitle);             //���ò���
        }
    }
    TEST_IS_TRUE_WITH_INFO("sdkCommOpenUart ����", (ret = sdkCommOpenUart(&pstUartInfo) == SDK_OK));


    if (sdkDispMsgBox(pTitle, "1.����pos�������� \r2.����pos�������� ", 0,
                      SDK_KEY_MASK_1 | SDK_KEY_MASK_2) == SDK_KEY_1) {
        sdkDispMsgBox("sdkCommUartRecvData����", "��Ӵ��ڷ������ݵ�pos�� \r\n 10�볬ʱ", 1,
                      SDK_KEY_MASK_ALL);
        TEST_IS_TRUE_WITH_INFO("sdkCommUartRecvData ����",
                               ((ret = sdkCommUartRecvData(pstUartInfo.ucCom, temprec,
                                                           sizeof(temprec), 10000)) > 0));

        if (ret > 0) {
            memset(temp, 0, sizeof(temp));
            sdkBcdToAsc(temp, temprec, ret);
            sdkDispMsgBox("��ʾ�յ�������", temprec, 0, SDK_KEY_MASK_ALL);
        }
        else {
            sdkDispMsgBox("sdkCommUartRecvData����", "û���յ�����", 0, SDK_KEY_MASK_ALL);
        }
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "sdkCommUartRecvData����ֵ :\r%d", ret);
        strcat(pasOutInfo, temp);
    }
    else {
        sdkDispMsgBox("sdkCommUartSendData ����", "����ȷ�Ϻ��Ӵ��ڷ���2049�ֽ�������,�ӵ��Թ��߲鿴", 0,
                      SDK_KEY_MASK_ENTER);
        TEST_IS_TRUE_WITH_INFO("sdkCommUartSendData ����",
                               (ret = sdkCommUartSendData(pstUartInfo.ucCom, tempsend,
                                                          sizeof(tempsend)) == SDK_OK));
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "sdkCommUartSendData����ֵ : \r%d", ret);
        strcat(pasOutInfo, temp);
    }
    TEST_IS_TRUE_WITH_INFO("sdkCommCloseUart ����",
                           (sdkCommCloseUart(pstUartInfo.ucCom) == SDK_OK));


    return SDK_OK;
}

/********************************************************************
   ��    ��: �����
   ��    Ȩ: �������¹��������ɷ����޹�˾
   ��������: ��ȡ����ѡ���ͨ��ģʽ
   �������:
   �������:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
********************************************************************/
static s32 sdkGetInputCommMode() {
    s32 ret, inputflag = 1;

    while (inputflag) {
        s32 key = 0;
        key = sdkDispMsgBox("ѡ��ͨ�ŷ�ʽ(1-4)", "1.modem���� \r2.GPRS����\r3.CDMA����\r4.��̫��\r", 0,
                            SDK_KEY_MASK_ALL);

        switch (key) {
            case SDK_KEY_1:
                ret = 1;
                inputflag = 0;
                break;

            case SDK_KEY_2:
                ret = 2;
                inputflag = 0;
                break;

            case SDK_KEY_3:
                ret = 3;
                inputflag = 0;
                break;

            case SDK_KEY_4:
                ret = 4;
                inputflag = 0;
                break;

            default:
                sdkDispMsgBox("tips", "������ѡ��ͨ�ŷ�ʽ", 0, SDK_KEY_MASK_ALL);
                break;
        }
    }

    return ret;
}


static s32 sdkGetModemMode() {
    s32 ret;

    while (1) {
        s32 key = 0;
        key = sdkDispMsgBox("ѡ��ͨ�ŷ�ʽ(1-4)", "1.FSK\r2.HDLC ͬ��\r3.HDLC �첽\r", 0, SDK_KEY_MASK_ALL);

        switch (key) {
            case SDK_KEY_1:
                return SDK_COMM_LINK_MODE_FSK;

            case SDK_KEY_2:
                return SDK_COMM_LINK_MODE_SYN;

            case SDK_KEY_3:
                return SDK_COMM_LINK_MODE_AYN;

            default:
                sdkDispMsgBox("tips", "������ѡ��ͨ�ŷ�ʽ", 0, SDK_KEY_MASK_ALL);
                break;
        }
    }

}

/********************************************************************
   ��    ��: hzl
   ��    Ȩ: �������¹��������ɷ����޹�˾
   ��������:
   �������:
   �������:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
********************************************************************/
static s32 SetCommParmManual(SDK_COMM_STCOMMPARAM *pstCommUartParam, char *pasOutInfo,
                             char const *pTitle) {
    s32 ret;
    SDK_COMM_STMODEMEXPARAM *pExParam = &pstCommUartParam->stCommInfo.stModemInfo.stModemExParam;

    memset(pstCommUartParam, 0, sizeof(SDK_COMM_STCOMMPARAM));


    //2013-09-09 �޸� by wangyanjie	 ��ȡͨ��ģʽ
    pstCommUartParam->eMode = sdkGetInputCommMode();

    if (pstCommUartParam->eMode == SDK_COMM_MODEM)  //modem
    {
        pstCommUartParam->stCommInfo.stModemInfo.eLinkMode = sdkGetModemMode();
        pstCommUartParam->ucReDialTimes = sdkTestPresetInputU8(pTitle, "�ز�����:", "3");
        pstCommUartParam->ucDialTime = sdkTestPresetInputU32(pTitle, "����ʱ��(ms):", "60000");

        if (sdkTestPresetInputU8(pTitle, "-�Ƿ������ߺ���:0-�ޣ�1-��", "1")) {
            sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stModemInfo.asOutLineNo, pTitle,
                                     "���ߺ���:", "1234", 1, 7);
        }
        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stModemInfo.asPhone[0], pTitle,
                                 "�绰����0:", "#1", 1, 25);
        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stModemInfo.asPhone[1], pTitle,
                                 "�绰����1:", "#1", 1, 25);
        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stModemInfo.asPhone[2], pTitle,
                                 "�绰����2:", "#1", 1, 25);

        if (sdkTestPresetInputU8(pTitle, "�Ƿ�����modem��չ����:0-��1-��", "1")) {
            pstCommUartParam->stCommInfo.stModemInfo.bIsNeedExParam = 1;   // ʹ�����õ���չ����
            pExParam->usCheckToneTime = sdkTestPresetInputU16(pTitle, "���������ʱ��(500):", "500");
            pExParam->usWaveLostTime = sdkTestPresetInputU16(pTitle, "�ز���ʧʱ��(20):", "20");
            pExParam->bIsCheckPhone = sdkTestPresetInputU8(pTitle, "�Ƿ񲢻����(1):", "1");
            pExParam->ucDtmfOnTime = sdkTestPresetInputU8(pTitle, "DTMF on ʱ��(100):", "100");
            pExParam->ucDtmfOffTime = sdkTestPresetInputU8(pTitle, "DTMF offʱ��(100):", "100");
            pExParam->bIsCheckDialTone = sdkTestPresetInputU8(pTitle, "�Ƿ��Ⲧ����(1):", "1");
            pExParam->ucDelayBeforeDial = sdkTestPresetInputU8(pTitle,
                                                               "����Ⲧ����ʱ ����ǰ����ʱʱ��(1):", "1");
            pExParam->ucSendLevel = sdkTestPresetInputU8(pTitle, "���͵�ƽ(12):", "12");
            pExParam->ucFsk01Num = sdkTestPresetInputU8(pTitle, "0��1�ĸ���(120):", "120");
            pExParam->ucFsk1Num = sdkTestPresetInputU8(pTitle, "1��0�ĸ���(60):", "60");
            pExParam->ucCidFormat = sdkTestPresetInputU8(pTitle, "����ģʽ(0):", "0");
        }
    }
    else if (pstCommUartParam->eMode == SDK_COMM_GPRS || pstCommUartParam->eMode == SDK_COMM_CDMA) {
        pstCommUartParam->ucReDialTimes = sdkTestPresetInputU8(pTitle, "�ز�����:", "3");
        pstCommUartParam->ucDialTime = sdkTestPresetInputU32(pTitle, "����ʱ��:", "60000");


        pstCommUartParam->bSocketConnectMode = sdkTestPresetInputU8(pTitle,
                                                                    "SocketConnectMode(0��1):",
                                                                    "0");

        //     strcpy(pstCommUartParam->stCommInfo.stPppWireLessInfo.asUsername, "card");
        //       strcpy(pstCommUartParam->stCommInfo.stPppWireLessInfo.asPassword, "card");
        //      strcpy(pstCommUartParam->stCommInfo.stPppWireLessInfo.asGprsApn, "cmnet");

        sdkTestPresetInputASC(pstCommUartParam->stCommInfo.stPppWireLessInfo.asUsername, pTitle,
                              "username:", "card", 1, 50);
        sdkTestPresetInputASC(pstCommUartParam->stCommInfo.stPppWireLessInfo.asPassword, pTitle,
                              "asPassword:", "card", 1, 50);
        sdkTestPresetInputASC(pstCommUartParam->stCommInfo.stPppWireLessInfo.asGprsApn, pTitle,
                              "asGprsApn:", "cmnet", 1, 50);

        //sdkTestInputASC(pstCommUartParam->stCommInfo.stLanInfo.asNetClientIP, pTitle, "ClinetIP:", 1, 15);
        //sdkTestInputASC(pstCommUartParam->stCommInfo.stLanInfo.asNetGate, pTitle, "NetGate:", 1, 15);
        //sdkTestInputASC(pstCommUartParam->stCommInfo.stLanInfo.asNetMask, pTitle, "NetMask:", 1, 15);


        //pstCommUartParam->stServerInfo.asServerIP = sdkTestInputU8(pTitle, "server ip:");
        //pstCommUartParam->stServerInfo.asPort = sdkTestInputU8(pTitle, "server port:");

        //   strcpy(pstCommUartParam->stServerInfo.asServerIP,"219.133.170.86");
        // strcpy(pstCommUartParam->stServerInfo.asPort, "8324");

        sdkTestPresetInputASCNum(pstCommUartParam->stServerInfo.asServerIP, pTitle, "server ip:",
                                 "219.133.170.86", 1, 15);
        sdkTestPresetInputASCNum(pstCommUartParam->stServerInfo.asPort, pTitle, "server port:",
                                 "8821", 1, 5);
    }
    else if (pstCommUartParam->eMode == SDK_COMM_ETHERNET) {
#if 1
        pstCommUartParam->ucReDialTimes = sdkTestPresetInputU8(pTitle, "�ز�����:", "3");
        pstCommUartParam->ucDialTime = sdkTestPresetInputU32(pTitle, "����ʱ��:", "20000");

        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stLanInfo.asNetClientIP, pTitle,
                                 "ClinetIP:", "172.23.10.253", 1, 15);
        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stLanInfo.asNetGate, pTitle,
                                 "NetGate:", "172.23.0.254", 1, 15);
        sdkTestPresetInputASCNum(pstCommUartParam->stCommInfo.stLanInfo.asNetMask, pTitle,
                                 "NetMask:", "255.255.0.0", 1, 15);


        sdkTestPresetInputASCNum(pstCommUartParam->stServerInfo.asServerIP, pTitle, "server ip:",
                                 "172.23.0.34", 1, 15);
        sdkTestPresetInputASCNum(pstCommUartParam->stServerInfo.asPort, pTitle, "server port:",
                                 "8821", 1, 5);
#endif

#if 0
        pstCommUartParam->ucReDialTimes = 3;
        pstCommUartParam->ucDialTime = 60 * 1000;

        strcpy(pstCommUartParam->stCommInfo.stLanInfo.asNetClientIP, "172.23.114.66");
        strcpy(pstCommUartParam->stCommInfo.stLanInfo.asNetGate, "172.23.0.254");
        strcpy(pstCommUartParam->stCommInfo.stLanInfo.asNetMask, "255.255.0.0");


        strcpy(pstCommUartParam->stServerInfo.asServerIP, "172.23.1.45");
        strcpy(pstCommUartParam->stServerInfo.asPort, "8000");
#endif
    }
    else if (pstCommUartParam->eMode == SDK_COMM_WIFI) {
    }
    else if (pstCommUartParam->eMode == SDK_COMM_MODEMPPP) {
    }
    else {
    }
    return SDK_OK;
}

/********************************************************************
   ��    ��: hzl
   ��    Ȩ: �������¹��������ɷ����޹�˾
   ��������: ����
   �������:
   �������:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
********************************************************************/
static s32 sdkCommTestManual(char *pasOutInfo, char const *pTitle) {
    s32 ret = SDK_OK;
    u8 info[128] = {0};
    u8 temp[2048] = {0}, temp1[2048] = {0};
    //2013-09-17 wangyanjie �޸Ŀ��Բ��ظ��������, ���Կ�����ֻ����һ�β���
    //SDK_COMM_STCOMMPARAM pstCommUartParam;
    static SDK_COMM_STCOMMPARAM pstCommUartParam;
    static s32 initflag = 0;
    static bool HardMode = 3;
    s32 TimerCommuStateID;

    u8 fn[512] = {0};
    s32 len = 0;

    //sdkCommDestoryLink();

    /*=======BEGIN: ���ٲ� 2014.03.04  16:16 add===========*/
    memset(fn, 0, sizeof(fn));
    sdkSysGetCurAppDir(fn);
    strcat(fn, "testpara");

    len = sizeof(SDK_COMM_STCOMMPARAM);


    if (SDK_FILE_OK != sdkReadFile(fn, &pstCommUartParam, 0, &len)) {
        memset(&pstCommUartParam, 0, sizeof(SDK_COMM_STCOMMPARAM));
    }
    /*====================== END======================== */



    memset(temp, 0, sizeof(temp));
    //memcpy(temp, "\x60\x10\x00\x20\x00\x60\x31\x00\x31\x00\x17\x08\x00\x00\x20\x00\x00\x00\xC0\x00\x16\x00\x00\x01\x35\x35\x35\x35\x35\x35\x35\x35\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x33\x00\x11\x00\x00\x00\x01\x00\x10\x00\x17\x53\x65\x71\x75\x65\x6E\x63\x65\x20\x4E\x6F\x30\x34\x33\x31\x30\x30\x00\x03\x30\x31\x20", 79);
    //memcpy(temp,"\x60\x00\x08\x00\x00\x87\x67\xc6\x69\x73\x01\x31\x35\x30\x33\x36\x30\x31\x31\x30\x33\x30\x31\x35\x36\x38\x31\x00\x00\x00\x00\x00\x00\x00\x00\x00\x02\x90\x28\x00\x00\xea\x00\x2a\x02\x00\x30\x01\x20\x12\x12\x27\x00\x00\x01\x30\x35\x31\x01\x82\x00\x18\x31\x35\x30\x33\x36\x30\x31\x31\x30\x33\x30\x31\x35\x36\x38\x31\x00\x00\x00\x00\x00\x00\x00\x00\x7f",87);  //���ɲ��Բ���
    memcpy(temp,
           "\x00\x3E\x60\x00\x03\x00\x21\x60\x22\x00\x00\x00\x00\x08\x00\x00\x20\x00\x00\x00\xC0\x00\x12\x00\x10\x30\x32\x32\x32\x32\x32\x32\x32\x32\x39\x38\x37\x36\x35\x34\x33\x32\x31\x30\x39\x38\x37\x36\x35\x00\x11\x00\x00\x00\x01\x00\x10\x00\x03\x30\x31\x20",
           62);
    //memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    //2013-09-17 wangyanjie �޸Ŀ��Բ��ظ��������
    if (0 == pstCommUartParam.eMode)            //��һ��Ҫ���ò���
    {
        SetCommParmManual(&pstCommUartParam, pasOutInfo, pTitle);                 //���ò���
        if (pstCommUartParam.eMode == SDK_COMM_MODEM) {
            HardMode = sdkDispMsgBox("Ӳèģʽ(501֧��)", "\r0.���ӿ죬ͨѶ�� \r1.��������ͨѶ��", 0,
                                     SDK_KEY_MASK_01) == SDK_KEY_1;
        }
    }
    else if (sdkDispMsgBox(pTitle, "\r1.���������  2.�������", 0, SDK_KEY_MASK_12) ==
             SDK_KEY_2)                  //��ʾ�Ƿ��������
    {
        memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
        SetCommParmManual(&pstCommUartParam, pasOutInfo, pTitle);                     //���ò���
        if (pstCommUartParam.eMode == SDK_COMM_MODEM) {
            HardMode = sdkDispMsgBox("Ӳèģʽ(501֧��)", "\r0.���ӿ죬ͨѶ�� \r1.��������ͨѶ��", 0,
                                     SDK_KEY_MASK_01) == SDK_KEY_1;
        }
        initflag = 0;
    }


    if (sdkCommConfig(&pstCommUartParam) != SDK_OK) {
        memset(info, 0, sizeof(info));
        strcpy(info, "�������ô���");
        goto OUT;
    }

    /*=======BEGIN: ���ٲ� 2014.03.04  16:26 add===========*/
    //�����ļ�
    sdkWriteFile(fn, &pstCommUartParam, sizeof(SDK_COMM_STCOMMPARAM));
    /*====================== END======================== */

    sdkDispMsgBox("��ʼ��", "��ʼ����...", 1, SDK_KEY_MASK_ALL);

    if (initflag == 0)//��ʼ��ֻҪһ��//shiweisong 2013.11.18 16:0
    {
        initflag = 1;
        Trace("likun", "��ʼ��ͨ�Ų���\r\n");
        sdkCommInit(&pstCommUartParam);    //(const SDK_COMM_STCOMMPARAM * pstCommParam)
        sdkCommSetHardModemMode(HardMode);
    }
    if (sdkDispMsgBox(pTitle, "\r1.Ԥ����  2.��Ԥ����", 0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2) ==
        SDK_KEY_1) {
        TimerCommuStateID = sdkTimerGetId();
        sdkCommPre();//lk 2013.10.15 11:21  Ԥ����
        while (sdkTimerIsEnd(TimerCommuStateID, 20 * 1000) ==
               false)                                                                            // ��ѯͨ��״̬��ʱ��
        {
            u8 tem[50];
            sprintf(tem, "ʣ��ʱ��%d ms", (TimerCommuStateID + 20 * 1000 - sdkTimerGetId())),
                    sdkDispMsgBox("Ԥ����", tem, 1, SDK_KEY_MASK_1 | SDK_KEY_MASK_2);
        }
    }

    sdkDispMsgBox("����", "��������....", 1, SDK_KEY_MASK_1 | SDK_KEY_MASK_2);
    TESTAGAIN:
    ret = sdkCommCreateLink();
    /*==================== END ======================*/

    if (ret != SDK_OK) {
        memset(info, 0, sizeof(info));
        strcpy(info, "���Ӳ��ɹ�");
        goto OUT;
    }

    sdkDispMsgBox("����", "���ڷ�������", 1, SDK_KEY_MASK_1);
    ret = sdkCommSendData(temp, 62, 0);
    TestLog("sjl", "ret is +++++++++%d \r\n", ret);
    if (ret != SDK_OK) {
        memset(info, 0, sizeof(info));
        strcpy(info, "�������ݲ��ɹ�");
        goto OUT;
    }

    sdkDispMsgBox("����", "���ڽ�������", 1, SDK_KEY_MASK_1);

    ret = 0;
    ret = sdkCommTmsRecvData(temp, 2048, 60, NULL, 1);
    TestLog("sjl", "ret is +++++++++%d \r\n", ret);

    if (ret >= 0)     //shijianglong 2013.04.10 11:42
    {
        TestLogHex("TestComm", "���յ�����", temp, ret);
        sdkBcdToAsc(temp1, temp, ret);
        sdkDispMsgBox("�յ�������", temp1, 0, SDK_KEY_MASK_ESC);
        memset(info, 0, sizeof(info));
        strcpy(info, "ͨ�Ų��Գɹ�");

        if (SDK_KEY_1 ==
            sdkDispMsgBox("tips", "ͨ�ųɹ����Ƿ񲻹Ҷ�ͨ�ż�������\r\n0.�˳�����\r\n1.��������\r\n", 0,
                          SDK_KEY_MASK_0 | SDK_KEY_MASK_1)) {
            goto TESTAGAIN;
        }
    }
    else {
        strcpy(info, "δ���յ�����");
    }
    OUT:

    if (SDK_OK != sdkCommDestoryLink()) {
        strcat(info, "\r�Ͽ����Ӳ��ɹ�");
    }

    if (pstCommUartParam.eMode == SDK_COMM_MODEM) {
        sprintf(pasOutInfo,
                "%s\r ret=%d\r �ز����� %d \r����ʱ�� %d\r ���ߺ��� %s\r �绰����0 %s\r�绰����1 %s \r�绰����2 %s  ",
                info, ret, pstCommUartParam.ucReDialTimes, pstCommUartParam.ucDialTime,
                pstCommUartParam.stCommInfo.stModemInfo.asOutLineNo,
                pstCommUartParam.stCommInfo.stModemInfo.asPhone[0],
                pstCommUartParam.stCommInfo.stModemInfo.asPhone[1],
                pstCommUartParam.stCommInfo.stModemInfo.asPhone[2]
        );
    }
    else if (pstCommUartParam.eMode == SDK_COMM_ETHERNET) {
        sprintf(pasOutInfo,
                "%s\r ret=%d\r �ز����� %d \r����ʱ�� %d\rClinetIP:%s\r   NetGate:%s\r   NetMask:%s\r  serverIP:%s\r serverport:%s",
                info, ret,
                pstCommUartParam.ucReDialTimes,
                pstCommUartParam.ucDialTime,
                pstCommUartParam.stCommInfo.stLanInfo.asNetClientIP,
                pstCommUartParam.stCommInfo.stLanInfo.asNetGate,
                pstCommUartParam.stCommInfo.stLanInfo.asNetMask,
                pstCommUartParam.stServerInfo.asServerIP,
                pstCommUartParam.stServerInfo.asPort);
    }
    else if (pstCommUartParam.eMode == SDK_COMM_GPRS || pstCommUartParam.eMode == SDK_COMM_CDMA) {
        sprintf(pasOutInfo,
                "%s\r ret=%d \r dialtime=%d\r redialtimes=%d\r bSocketConnectMode=%d\r  username=%s \r password=%s\r gprsapn=%s\r  serverIP:%s\r serverport:%s",
                info, ret, pstCommUartParam.ucDialTime,
                pstCommUartParam.ucReDialTimes,
                pstCommUartParam.bSocketConnectMode,
                pstCommUartParam.stCommInfo.stPppWireLessInfo.asUsername,
                pstCommUartParam.stCommInfo.stPppWireLessInfo.asPassword,
                pstCommUartParam.stCommInfo.stPppWireLessInfo.asGprsApn,
                pstCommUartParam.stServerInfo.asServerIP,
                pstCommUartParam.stServerInfo.asPort);
    }
    return SDK_OK;
}

static s32 sdkCommGetSignalLevelM(char *pasOutInfo, char const *pTitle) {
    sdkDispMsgBox("tips", "ע��ֻ��������ģ��Ļ�����֧�ִ˺���", 0, SDK_KEY_MASK_ALL);
    sprintf(pasOutInfo, "�ź�ֵ%d\r", sdkCommGetSignalLevel());
    return SDK_OK;
}

static s32 sdkCommGetWirelessModuleIMSIM(char *pasOutInfo, char const *pTitle) {
    s32 len = 0;
    u8 imsi[1024] = {0};

    len = sdkCommGetWirelessModuleIMSI(imsi);
    sprintf(pasOutInfo, "IMSIΪ:%s ����:%d", imsi, len);
    return SDK_OK;
}

static s32 sdkCommGetWirelessModuleIDM(char *pasOutInfo, char const *pTitle) {
    s32 len = 0;
    u8 imei[1024] = {0};

    len = sdkCommGetWirelessModuleID(imei);
    sprintf(pasOutInfo, "ModuleIDΪ:%s ����:%d", imei, len);

    return SDK_OK;
}


//wangyanjie �޸� 2013-09-17
static const TEST_MANUAL_MAP CommUartManaulMap[] =
        {
                {"sdkCommUartTest", sdkCommUartTest},
#if (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P)
                {"sdkCommGetSignalLevel", sdkCommGetSignalLevelM},
                {"sdkCommTestManual", sdkCommTestManual},
                {"sdkCommGetWirelessModuleID", sdkCommGetWirelessModuleIDM},
                {"sdkCommGetWirelessModuleIMSI", sdkCommGetWirelessModuleIMSIM}
                //{"sdkCommCloseUart", sdkCommCloseUartM}
#endif
        };


TEST_MAP_INFO GetCommUartManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(CommUartManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = CommUartManaulMap;
    return ret;
}

