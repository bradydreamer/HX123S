cmd_Str const MagCardCmdTab[] = {
        {MAG_OPEN_DEVICE,               OnOpenMagCardDevice,            NULL},
        {MAG_CLOSE_DEVICE,              OnCloseMagCardDevice,           NULL},
        {MAG_READ_CARDDATA_NEN,         OnReadMagCardNoEncrypt,         NULL},
        {MAG_READ_CARDDATA_EN,          OnReadMagCardEncrypt,           NULL},
        {MAG_READ_CARDDATA_INSIDE_SAVE, OnReadMagCardEncryptInsideSave, NULL},
        {MAG_READ_CARDDATA_AUDIO,       OnReadMagCardAudio,             NULL},
        {MAG_CMD_END,                   NULL,                           NULL}
};

#define TRACK_BUF_SIZE  150 //���ڼ��ܴŵ����ݵ���ʱ��������С

//ȫ�ֱ���,������������ݺ�״̬
static bool gIsMagDataValid = false; //�����Ƿ���Ч,����������Ϊ��Ч
////////////

bool gIsAlreadyReadCardRFFlg = false;   //20150330 ��ӷǽ��Ƿ��Ѷ�����ʶ
static u8 gCurrentCardType = SDK_ICC_MANUAL;


void Des3sEcb(u8 *input, u16 inputlen, u8 *output, u16 *outputlen, u8 *key) {
    u16 len = inputlen;
    u16 i;

    memcpy(output, input, inputlen);

    if (len % 8) {
        memcpy(output + len, "\x00\x00\x00\x00\x00\x00\x00\x00", ((len / 8) + 1) * 8 - len);
        len = ((len / 8) + 1) * 8;
    }

    for (i = 0; i < len; i += 8) {
        sdkDes3S(1, output + i, key);
    }

    *outputlen = len;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡƽ̨��ˮ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
u8 GetPlatformSerialNO(u8 *pOut) {
    memcpy(pOut, gPlatformSerialNO, 12);
    return 0;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ��ǰ��������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
u8 GetCurrentCardType() {
    return gCurrentCardType;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���õ�ǰ��������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static void SetCurrentCardType(u8 cardType) {
    gCurrentCardType = cardType;
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �򿪴�����������,ˢ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ljh�޸�
 ********************************************************************/
s32 OnOpenMagCardDevice(ST_TRANSFLOW *pstTransFlow) {
    static u8 OverTime;
    s32 ret;
    u8 cardTypeMask = 0;
    static u8 ReadCardType;
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    //Trace("Lai","====OnOpenMagCardDevice=====\r\n");

    if (0 == pstTransFlow->ucInsRunStep) {
        //20150402 ���ȡ������ʱ
        gsiDCSecond = 0;
        gsiDCKey = 0;
        gsiDCTimer = 0;
        gsiDCRow = 0;
        DispClearAmount();          //20150402 ����ϱʽ��׽��
        TraceHex("Lai", "==heReceiveData=\r\n", pstTransFlow->heReceiveData,
                 pstTransFlow->ReceiveDataLen);
        pstTransFlow->ucInsRunStep = 1;
        ReadCardType = pstTransFlow->heReceiveData[0];
        gIsAlreadyReadCardRFFlg = false;        //20150330 ��ӷǽ��Ѷ���ʶ

        if (!(0x07 & ReadCardType))         //��ȡ�ſ�
        {
            FinishTradeFlow();
            return INS_PARAM_ERR;            //INS_EXIT;
            //gReadCardType = SDK_ICC_MAG;
        }
#if 0
        else if(0x02 == pstTransFlow->heReceiveData[0])        //����Ƿ���IC������
        {
            gReadCardType = SDK_ICC_ICC;
            pstCardInfo.stCardParam.ucCardMode = SDK_ICC_ICC;
        }
        else if(0x04 == pstTransFlow->heReceiveData[0])        //����Ƿ��зǽӿ�Ƭ
        {
            gReadCardType = SDK_ICC_RF;
        }
        else
        {
        }
#endif
        //ReadCardType <<= 1;			//����һλ��궨������ mai 20140904
        OverTime = pstTransFlow->heReceiveData[1];
        Trace("Lai", "ˢ����ʱʱ��	%d\r\n", OverTime);
        u8 text[128 + 1] = {0};
        u16 len;
        sdkBcdToU16(&len, &pstTransFlow->heReceiveData[2], 2);

        if (len > 128) { len = 128; }
        memcpy(text, &pstTransFlow->heReceiveData[4], len);
        sdkDispLabel(SDK_DISP_LINE1, text, SDK_DISP_LDISP | SDK_DISP_FDISP);

        pstTransFlow->InsClockTimer = sdkTimerGetId();
        gIsMagDataValid = false;
        GotoTradeFlow();
        memset(gPlatformSerialNO, 0, 12);
        SetCmdResetExe(FALSE);        //ljh2015-01-08
        bisNeedBrushScreenWhenBack = false;        //Xiaox 15-1-14 ���⴦��
    }
    else if (1 == pstTransFlow->ucInsRunStep) {
        if (sdkTimerIsEnd(pstTransFlow->InsClockTimer, OverTime * 1000)) {
            Trace("Lai", "ˢ����ʱ����\r\n");
            FinishTradeFlow();

#if 1 /*Modify by ���ƽ at 2015.04.08  16:56 */
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
#endif /* if 0 */
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

            return INS_OVERTIME;
        }

        if (GetCmdResetExe() == TRUE) {
            Trace("Lai", "ˢ���յ���λ����\r\n");
#if NEWCANCELCMD_MODE           //20150402 �µĸ�λ��ʽ
#else
            SetCmdResetExe(FALSE);
#endif
            FinishTradeFlow();
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

            return INS_OVERTIME;                                //��Ӧȡ���˳�ʱ�ķ���ֵӰ�췵�������ȷ��
        }
        //Trace("Lai","******* ReadCardType = %x\n", ReadCardType);
        ret = SposIccReadCardEx(pstTransFlow, ReadCardType, &IccEmvParam,
                                &pstCardInfo.stCardParam, &pstCardInfo.stCardData, 3,
                                &cardTypeMask);

        //һ��ֻ�᷵��һ�����
        if (SDK_OK == ret) {
            gIsAlreadyReadCardRFFlg = false;        //20150330 ��ӷǽ��Ѷ���ʶ
            if (cardTypeMask & MAG_CARD_MSK) {
                pstTransFlow->ucInsRunStep = 2;
                //sdkDispClearScreen();             //20150402 ȥ��ˢ��ʱȷ�Ͽ��������һ������
                sdkDispClearRow(SDK_DISP_LINE2);
                sdkDispClearRow(SDK_DISP_LINE3);
                sdkDispClearRow(SDK_DISP_LINE4);
                sdkDispClearRow(SDK_DISP_LINE5);
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����:",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispFillRowRam(SDK_DISP_LINE3, 0, pstCardInfo.stCardData.asCardNO,
                                  SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
                sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��(ȷ��)", SDK_DISP_FDISP | SDK_DISP_RDISP |
                                                                SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
                sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��(ȡ��)",
                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                return INS_KEEP;
            }
            else if (cardTypeMask & IC_CARD_MSK) {
                pSend[0] = IC_CARD_MSK;                //IC���Ѳ���    //0416Send==Recv  ��ǰ�� |=
                SetCurrentCardType(SDK_ICC_ICC);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                //sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            }
            else {
                pSend[0] = RF_CARD_MSK;                //�ǽ��ѷ���    //0416Send==Recv  ��ǰ�� |=
                SetCurrentCardType(SDK_ICC_RF);
                //sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            }
            pstTransFlow->usSendDataLen = 1;
            FinishTradeFlow();
            return INS_SUC;
        }
        else if (SDK_ERR == ret || SDK_ESC == ret) {
            gIsAlreadyReadCardRFFlg = false;        //20150330 ��ӷǽ��Ѷ���ʶ
            Trace("Lai", "$$$$ SposIccReadCardEx ret	%d ==\r\n", ret);
            pSend[0] = 0x11;            //ʧ��
            pstTransFlow->usSendDataLen = 1;
            FinishTradeFlow();
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            return INS_OTHER_ERR;
        }
        else if (SDK_EQU == ret) //sujianzhong 2015.01.12 19:33
        {
            ReadCardType &= ~0x01;
        }
    }    //ȷ�Ͽ��Ų�����ѭ���ȴ�����
    else if (2 == pstTransFlow->ucInsRunStep) {
        gIsAlreadyReadCardRFFlg = false;        //20150330 ��ӷǽ��Ѷ���ʶ
        if (GetCmdResetExe() == TRUE) {
            pSend[0] = 0x11;            //ʧ��
            pstTransFlow->usSendDataLen = 1;
            FinishTradeFlow();
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            return INS_OTHER_ERR;
        }

        if (sdkTimerIsEnd(pstTransFlow->InsClockTimer, OverTime * 1000)) {
            Trace("Lai", "ˢ����ʱ����\r\n");
            FinishTradeFlow();
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            return INS_OVERTIME;
        }
        ret = sdkKbGetKey();

        if (ret != 0) {
            sdkSysBeep(SDK_SYS_BEEP_OK);
        }

        if (ret == SDK_KEY_ENTER) {
            //DispWait();               //20150402 ȥ��ȷ�Ͽ���ʱ��ʾ�ȴ���Ϣ
            pstTransFlow->usSendDataLen = 1;
            pSend[0] = MAG_CARD_MSK; //ˢ������            //0416Send==Recv  ��ǰ�� |=
            gIsMagDataValid = true;
            SetCurrentCardType(SDK_ICC_MAG);
            FinishTradeFlow();
//			bisNeedBrushScreenWhenBack = true;
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            return INS_SUC;
        }
        else if (ret == SDK_KEY_ESC) {
            pSend[0] = 0x11;            //ʧ��
            pstTransFlow->usSendDataLen = 1;
            FinishTradeFlow();
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            return INS_OTHER_ERR;
        }
    }
    return INS_KEEP;
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �رմ�����������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ljh�޸�
 ********************************************************************/
s32 OnCloseMagCardDevice(ST_TRANSFLOW *pstTransFlow) {
    gIsMagDataValid = false;

    if (GetCurrentCardType() == SDK_ICC_MAG) {
        sdkIccCloseMag();
    }
    else if (GetCurrentCardType() == SDK_ICC_ICC) {
        sdkIccCloseIcDev();
    }
    else if (GetCurrentCardType() == SDK_ICC_RF) {
        sdkIccCloseRfDev();
    }
    SetCurrentCardType(SDK_ICC_MANUAL);

    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ������������,����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ljh�޸�
 ********************************************************************/
s32 OnReadMagCardNoEncrypt(ST_TRANSFLOW *pstTransFlow) {
    static u8 MagDataMode;
    s32 ret;
    u8 cardno[25] = {0}, cardnobuf[25] = {0};
    u16 cardnoLen = 0, i = 0;
    s32 bcdLen = 0;
    u8 tembuf[200] = {0};
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    gReadCardType = SDK_ICC_MAG;
    MagDataMode = pstTransFlow->heReceiveData[0];    //��ȡ�ŵ���Ϣ

    if (!gIsMagDataValid) {
        Trace("Lai", "$$$ !gIsMagDataValid---\r\n");
        goto ERROR;
    }
    memset(cardnobuf, 'F', 20);
    AppGetMagCardNo(&pstCardInfo.stCardData, cardno);

    if (strlen(cardno) < 13) {
        Trace("Lai", "-���������淶E_MAG_ERR---\r\n");
        goto ERROR;
    }
    cardnoLen = strlen(cardno);

    pSend[i] = 0x00;    //ˢ������
    i++;
    memcpy(cardnobuf, cardno, cardnoLen);
    ret = sdkAscToBcd(&pSend[i], cardnobuf, 20);    //���˺�
    i += 10;

    memset(cardnobuf, 0x00, sizeof(cardnobuf));
    sdkSHA1(cardno, cardnoLen, cardnobuf);
    memcpy(&pSend[i], cardnobuf, 20);  //��ϣֵ
    i += 20;

    pSend[i] = MagDataMode;    //ָʾλ
    i++;

    //��Ҫ�����һ������,sdk��ȡ���������Ѿ��޸������� ����ԭ�ⲻ��
    if (MagDataMode & 0x01)    //1�ŵ� ��ת��
    {
        sdkU16ToBcd(&pSend[i], pstCardInfo.stCardData.ucTrack1Len, 2);
        i += 2;
        memcpy(&pSend[i], &pstCardInfo.stCardData.asTrack1Data, pstCardInfo.stCardData.ucTrack1Len);
        i += pstCardInfo.stCardData.ucTrack1Len;
        TraceHex("Lai", "***track1***", pstCardInfo.stCardData.asTrack1Data,
                 pstCardInfo.stCardData.ucTrack1Len);
    }

    if (MagDataMode & 0x02)    //2�ŵ� ת����bcd
    {
        bcdLen = sdkAscToBcd(tembuf, pstCardInfo.stCardData.asTrack2Data,
                             pstCardInfo.stCardData.ucTrack2Len);

        if (bcdLen > 0) {
            sdkU16ToBcd(&pSend[i], bcdLen, 2);
            i += 2;
            memcpy(&pSend[i], tembuf, bcdLen);
            i += bcdLen;
            TraceHex("Lai", "***track2***", tembuf, bcdLen);
        }
        else {
            Trace("Lai", "track2 ת��bcdʧ��!\r\n");
            goto ERROR;
        }
    }

    if (MagDataMode & 0x04)    //3�ŵ� ת��bcd
    {
        bcdLen = sdkAscToBcd(tembuf, pstCardInfo.stCardData.asTrack3Data,
                             pstCardInfo.stCardData.ucTrack3Len);

        if (bcdLen > 0) {
            sdkU16ToBcd(&pSend[i], bcdLen, 2);
            i += 2;
            memcpy(&pSend[i], tembuf, bcdLen);
            i += bcdLen;
            TraceHex("Lai", "***track3***", tembuf, bcdLen);
        }
        else {
            Trace("Lai", "track3 ת��bcdʧ��!\r\n");
            goto ERROR;
        }
    }
    pstTransFlow->usSendDataLen = i;
    gIsMagDataValid = false;    //����֮�����Ч
    return INS_SUC;

    ERROR:
    pSend[0] = 0xFF;    //ʧ��
    pstTransFlow->usSendDataLen = 1;
    return INS_OTHER_ERR;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ܴŵ�����,�������������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: pOutlen ������0�ĳ���,����ֵ�����������ݵĳ���
 ********************************************************************/
//�ŵ�����ǰ����������
//ƽ̨��ˮ�ţ�12���ֽڣ�+�������6���ֽڣ�+�豸���кţ�10���ֽڣ�+���ŵ����ݺϲ�
//���м��ܡ�ƽ̨���ܺ�������֤��������ٽ��н��ף�ƽ̨��ˮ�����ڱ�ʶ���ס�
//������������8�ֽڶ��룬���㲹 0x00��3DES����ECB ��ʽ��

s32 EncryptTrackData(u8 *pSerialNO, u8 *pRandomNO, u8 *pTrackData, u8 trackLen,
                     SDK_PED_WK_CFG *wkcfg, u8 *pOutBuf, u16 *pOutlen) {
    s32 ret;
    u8 posSN[16] = {0};
    u16 index = 0, realDataLen, encryptlen;

    u8 *pbuf = sdkGetMem(512);

    if (pbuf == NULL) {
        Trace("Lai", "$$$ pbuf==NULL\r\n");
        return -1;
    }
    memset(pbuf, 0, 512);

    realDataLen = sdkReadPosSn(posSN);
    TraceHex("Lai", "##### pos sn", posSN, realDataLen);

    memcpy(&pbuf[index], pSerialNO, 12);
    index += 12;
    memcpy(&pbuf[index], pRandomNO, 8);
    index += 8;  //Э���8,��ǰ���6...
    memcpy(&pbuf[index], posSN, 10);
    index += 10;
    memcpy(&pbuf[index], pTrackData, trackLen);
    index += trackLen;
    realDataLen = index;
    encryptlen = (realDataLen + 7) / 8 * 8;

    //TraceHex("Lai","### EncryptTrackData bef Encrypt buf:\r\n", pbuf, encryptlen);

    //������Կ����
    Trace("Lai", "wk encrypt wkcfg->ucWkIndex:%d,wkcfg->eWkType:%d  encryptlen:%d\r\n",
          wkcfg->ucWkIndex, wkcfg->eWkType, encryptlen);

    ret = sdkPEDDesEx(wkcfg->ucWkIndex, SDK_PED_WK, wkcfg->eWkType,
                      SDK_PED_ENCRYPT, SDK_PED_DES_ALL, pbuf, encryptlen, pOutBuf);

    sdkFreeMem(pbuf);

    if (ret != SDK_OK) {
        Trace("Lai", "$$$$$ pos sdkPEDDesEx fail ret:%d\r\n", ret);
        return -1;
    }
    TraceHex("Lai", "EncryptTrackData after Encrypt:\r\n", pOutBuf, encryptlen);

    *pOutlen = encryptlen;
    return realDataLen;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��������
   ��ڲ���:
   �� �� ֵ: ���ز�0��ĳ���
   ��	 ע: �Ѵ���EncryptData��������� �������ȡһ������
 ********************************************************************/
s32 EncryptHexData(u8 *pData, u8 dataLen, SDK_PED_WK_CFG *wkcfg, u8 *pOutBuf) {
    s32 ret;
    u8 *pTmpBuf = NULL;
    bool isMalloc = false;
    u16 encryptlen = (dataLen + 7) / 8 * 8;

    if (encryptlen == dataLen)  //���ȸպþͲ��÷������ռ䲹0��
    {
        pTmpBuf = pData;
    }
    else {
        pTmpBuf = sdkGetMem(encryptlen);

        if (pTmpBuf == NULL) {
            Trace("Lai", "$$$ pTmpBuf==NULL\r\n");
            return -1;
        }
        memset(pTmpBuf, 0, encryptlen);
        memcpy(pTmpBuf, pData, dataLen);
        isMalloc = true;
    }
    //TraceHex("Lai","### EncryptHexData bef Encrypt buf:\r\n", pTmpBuf, encryptlen);

    //������Կ����
    ret = sdkPEDDesEx(wkcfg->ucWkIndex, SDK_PED_WK, wkcfg->eWkType,
                      SDK_PED_ENCRYPT, SDK_PED_DES_ALL, pTmpBuf, encryptlen, pOutBuf);

    if (isMalloc) {
        sdkFreeMem(pTmpBuf);
    }

    if (ret != SDK_OK) {
        Trace("Lai", "$$$$$ pos sdkPEDDesEx fail ret:%d\r\n", ret);
        return -1;
    }
    else {
        TraceHex("Lai", "### EncryptHexData after Encrypt:\r\n", pOutBuf, encryptlen);
    }
    return encryptlen;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ2�ŵ�ʧЧ���ںͷ�����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void GetEDSCFromTrack2(u8 *pData, u8 *pEDSC) {
    u8 *p = pData;
    u8 i = 0;

    while (*p != 0x44 && i < 21)  //�ָ���,22��ֹ����ѭ��,���ڿ��ų��ȼ���
    {
        p++;
        i++;
    }

    p++;

    memcpy(pEDSC, p, 7);
    TraceHex("Lai", "EDSC:\r\n", pEDSC, 7);
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����������Ϣ,����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ljh�޸�
 ********************************************************************/
s32 OnReadMagCardEncrypt(ST_TRANSFLOW *pstTransFlow) {
    u8 ArithmMode = 0, MagDataMode = 0, EncryptMode = 0, keyindex;
    u8 cardnoMask[11] = {0}, KeyData[17] = {0}, KeyCheckValue[9] = {0};
    u8 cardnoMaskAsc[20] = {0};
    u8 randnum[9] = {0}, SerialNumber[12] = {0};
    SDK_PED_WK_CFG wkcfg;
    //s32 ret;
    u8 pkIndex;    //��Կ����
    u8 cardno[25], cardnobuf[25];//, buf[25] = {0}; //,heTmk[17],heTempdata[8];
    u16 cardnoLen = 0, i = 0, j = 0, k = 0, len = 0;
    u16 extInfoLen;//, encryptLen;
    u8 EDSC[7] = {0};
    u8 databuf[128] = {0};
    u16 databuflen = 0;
    //u8 ismaskd = false;
    u8 key[16] = {0};
    u8 fensan[16];

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    TraceHex("zcl", "asCardNO", pstCardInfo.stCardData.asCardNO, pstCardInfo.stCardData.ucCardLen);
    TraceHex("zcl", "asTrack1Data", pstCardInfo.stCardData.asTrack1Data,
             pstCardInfo.stCardData.ucTrack1Len);
    TraceHex("zcl", "asTrack2Data", pstCardInfo.stCardData.asTrack2Data,
             pstCardInfo.stCardData.ucTrack2Len);
    TraceHex("zcl", "asTrack3Data", pstCardInfo.stCardData.asTrack3Data,
             pstCardInfo.stCardData.ucTrack3Len);

    if (!gIsMagDataValid) {
        Trace("Lai", "$$$$$$ gIsMagDataValid\r\n");
        goto ERROR;
    }
    TraceHex("Lai", "==heReceiveData=\r\n", pstTransFlow->heReceiveData,
             pstTransFlow->ReceiveDataLen);
    gReadCardType = SDK_ICC_MAG;

    memset(&wkcfg, 0, sizeof(wkcfg));

    //��һ���ֽڲ����� ʹ��FIXED
    pkIndex = pstTransFlow->heReceiveData[i++];    //��Կ����
    MagDataMode = pstTransFlow->heReceiveData[i++];    //��ȡ�ŵ���Ϣ

    memcpy(cardnoMask, &pstTransFlow->heReceiveData[i], 10);
    i += 10;
    EncryptMode = pstTransFlow->heReceiveData[i++];    //�����㷨��ʶ  ��ʱ���ã�ֻʹ���������㷨

    //�Ƿ���������?
    /*
       //�Ȳ��ж����
       if(pkIndex!=0x02 || EncryptMode!=0x01)
       {
            Trace("Lai" ,"$$$$ pkIndex!=0x02 or EncryptMode!=0x01 :%d\r\n",EncryptMode);
            return INS_PARAM_ERR;
       }
     */
    keyindex = pstTransFlow->heReceiveData[i++];    //��Կ�������п����ǹ�������,Ҳ�п���������Կ����

    //����TmkIndex�Ƿ�Ϊ0,ʼ������wk����,ֻ��������0��ʱ����Ҫ����wk
    sdkBcdToU16(&len, &pstTransFlow->heReceiveData[i], 2);
    i += 2;

    if (len > 0)    //ID6������,Ϊ����Կ����
    {
        wkcfg.ucTmkIndex = keyindex;
        wkcfg.eTmkType = SDK_PED_DES_TRIPLE; //�̶�Ϊ˫des//sujianzhong 2015.01.10 10:25

        if (len == 24) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], 16);
            i += 16;
            memcpy(KeyCheckValue, &pstTransFlow->heReceiveData[i], 8);
            i += 8;
            wkcfg.eWkType = SDK_PED_DES_TRIPLE;
        }
        else if (len == 16) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], len);
            i += len;
            wkcfg.eWkType = SDK_PED_DES_TRIPLE;
        }
        else if (len == 8) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], len);
            memcpy(KeyData + 8, &pstTransFlow->heReceiveData[i],
                   len);         //����˫des����//sujianzhong 2015.01.10 15:17
            i += len;
            wkcfg.eWkType = SDK_PED_DES_TRIPLE;          //����˫des����
        }
        else {
            //Trace("Lai","$$$$$$$ ��Կ���� ������ %d\r\n",len);
            //goto ERROR;
            i += len;
        }

        if (SDK_OK !=
            sdkPEDDesEx(keyindex, SDK_PED_TMK, wkcfg.eWkType, SDK_PED_DECRYPT, SDK_PED_DES_ALL,
                        KeyData, len, key)) {
            goto ERROR;
        }
    }
    else {
        wkcfg.eWkType = SDK_PED_DES_TRIPLE;      //���֪��ԭ����ʲô���ܷ�ʽ?
        wkcfg.ucWkIndex = 0;      //������Կ����
    }

    if (pkIndex != 0xFF) {
        memset(fensan, 0, sizeof(fensan));
        memcpy(fensan, MposTrackKsn, 8);

        for (j = 8; j < 16; j++) {
            fensan[j] = ~fensan[j - 8];
        }

        if (len) //���·�������Կ��������������Կ���ܵĹ�����Կ���з�ɢ
        {
#if 0
            sdkAscToBcd(key, MposAppKey, 32); //�����Ժ�Ҫ��MposAppKey���������������
            sdkDes3S(1, fensan, key);
            sdkDes3S(1, fensan + 8, key);
#endif
            sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
            sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8, fensan + 8);

            memcpy(key, fensan, 16);
        }
        else {
            sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
            sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8, fensan + 8);
#if 0
            sdkAscToBcd(key, MposAppKey, 32); //�����Ժ�Ҫ��MposAppKey���������������
            sdkDes3S(1, fensan, key);
            sdkDes3S(1, fensan + 8, key);
#endif
            memcpy(key, fensan, 16);
        }
    }
    /**************************/
    memcpy(randnum, &pstTransFlow->heReceiveData[i], 8);
    i += 8;

    memcpy(gPlatformRandomNO, randnum, 6);
    memcpy(SerialNumber, &pstTransFlow->heReceiveData[i], 12);
    memcpy(gPlatformSerialNO, SerialNumber, 12);
    i += 12;
    ArithmMode = pstTransFlow->heReceiveData[i++];    //Ĭ����0

    if (EncryptMode == 7) //�㷨��ʶ7���и�����Ϣ//sujianzhong 2015.01.09 11:16
    {
        sdkBcdToU16(&extInfoLen, &pstTransFlow->heReceiveData[i], 2);
        i += 2;

        if (extInfoLen > 0) {
            TraceHex("Lai", "extinfo:\r\n", &pstTransFlow->heReceiveData[i], extInfoLen);
        }
    }
    //TraceHex("Lai","==cardnoMask=\r\n",cardnoMask, 10);
    //TraceHex("Lai","==KeyData=\r\n",KeyData, 16);
    //TraceHex("Lai","==KeyCheckValue=\r\n",KeyCheckValue, 8);
    //TraceHex("Lai","==randnum=\r\n",randnum, 8);
    //TraceHex("Lai","==SerialNumber=\r\n",SerialNumber, 12);


    memset(cardno, 0x00, sizeof(cardno));
    memset(cardnobuf, 0x00, sizeof(cardnobuf));
    memset(cardnobuf, 'F', 20);

    AppGetMagCardNo(&pstCardInfo.stCardData, cardno);

    if (strlen(cardno) < 13) {
        Trace("Lai", "-���������淶E_MAG_ERR---\r\n");
        goto ERROR;
    }
    cardnoLen = strlen(cardno);
    i = 0;
    pSend[i++] = 0x00;    //ˢ������


    sdkBcdToAsc(cardnoMaskAsc, cardnoMask, 10);

    for (j = 0; j < 20; j++) //���˺�
    {
        if (cardnoMaskAsc[j] == 'F') {
            break;
        }
    }

    for (k = 0; k < 20; k++) //���˺�
    {
        if (cardnoMaskAsc[19 - k] == 'F') {
            break;
        }
    }

    //ǰ6���� J = 6 K = 4��
    if (j + k > pstCardInfo.stCardData.ucCardLen) {
        len = 0;
    }
    else {
        len = pstCardInfo.stCardData.ucCardLen - j - k;
    }
    memset(cardnobuf, 0, sizeof(cardnobuf));
    memcpy(cardnobuf, pstCardInfo.stCardData.asCardNO, pstCardInfo.stCardData.ucCardLen);
    memcpy(cardnobuf + pstCardInfo.stCardData.ucCardLen, "FFFFFFFFFFFFFFFFFFFF",
           20 - pstCardInfo.stCardData.ucCardLen);

    if (len) {
        memcpy(cardnobuf + j, "EEEEEEEEEEEEEEEEEEEE", len);
    }
    sdkAscToBcd(&pSend[i], cardnobuf, 20);
    i += 10;
    TraceHex("Lai", "!!!!  after mask cardno", &pSend[i - 10], 10);

    memset(cardnobuf, 0x00, sizeof(cardnobuf));
    sdkSHA1(pstCardInfo.stCardData.asCardNO, pstCardInfo.stCardData.ucCardLen, cardnobuf);
    memcpy(&pSend[i], cardnobuf, 20);
    i += 20;

    pSend[i++] = MagDataMode;   //ģʽ


    /********����*************/
    if (MagDataMode & 0x01)    //1�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack1Len > 0) {
#if 1

            if (pkIndex != 0xFF) {
                memcpy(databuf, SerialNumber, 12);
                databuflen = 12;
                memcpy(databuf + databuflen, randnum, 6);
                databuflen += 6;
                sdkAscToBcd(databuf + databuflen, MposDeviceId, 20);
                databuflen += 10;
                memcpy(databuf + databuflen, pstCardInfo.stCardData.asTrack1Data,
                       pstCardInfo.stCardData.ucTrack1Len);
                databuflen += pstCardInfo.stCardData.ucTrack1Len;
                Des3sEcb(databuf, databuflen, databuf, &databuflen, key);
                sdkU16ToBcd(&pSend[i], databuflen, 2);
                i += 2;
                memcpy(&pSend[i], databuf, databuflen);
                i += databuflen;
            }
            else    //��ͳģʽ//sujianzhong 2015.01.12 11:28
            {
//                    memset(databuf, 0, sizeof(databuf));
//                    sdkAscToBcd(databuf, pstCardInfo.stCardData.asTrack1Data, pstCardInfo.stCardData.ucTrack1Len);
//                    databuflen = (pstCardInfo.stCardData.ucTrack1Len + 1) / 2;
//                    //����
//                    if(SDK_OK != sdkPEDWkDes(keyindex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, databuf+databuflen-9, databuf+databuflen-9))
//                    {
//                        goto ERROR;
//                    }
//                    sdkU16ToBcd(&pstTransFlow->heSendData[i], databuflen, 2);
//		            i+= 2;
//                    memcpy(&pstTransFlow->heSendData[i], databuf, databuflen);
//                    i += databuflen;
                //ֱ�Ӳ���1�ŵ�
                sdkU16ToBcd(&pSend[i], 0, 2);
                i += 2;
            }
#else
            Trace("Lai", "#### start to encrypt track1\r\n");

            if( EncryptTrackData(SerialNumber, randnum, pstCardInfo.stCardData.asTrack1Data,
                                 pstCardInfo.stCardData.ucTrack1Len, &wkcfg,
                                 &pSend[i + 2], &encryptLen) < 0 )
            {
                Trace("Lai", "$$$$$$ MagDataMode & 0x01 fail\r\n");
                goto ERROR;
            }
            sdkU16ToBcd(&pSend[i], encryptLen, 2);
            i += (2 + encryptLen);
#endif
        }
        else {
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
        }
    }

    if (pstCardInfo.stCardData.ucTrack2Len > 0) {
        GetEDSCFromTrack2(pstCardInfo.stCardData.asTrack2Data, EDSC);
    }

    if (MagDataMode & 0x02)    //2�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack2Len > 0) {
#if 1

            if (pkIndex != 0xFF) {
                memcpy(databuf, SerialNumber, 12);
                databuflen = 12;
                memcpy(databuf + databuflen, randnum, 6);
                databuflen += 6;
                sdkAscToBcd(databuf + databuflen, MposDeviceId, 20);
                databuflen += 10;
                memcpy(databuf + databuflen, pstCardInfo.stCardData.asTrack2Data,
                       pstCardInfo.stCardData.ucTrack2Len);
                databuflen += pstCardInfo.stCardData.ucTrack2Len;
                Des3sEcb(databuf, databuflen, databuf, &databuflen, key);
                sdkU16ToBcd(&pSend[i], databuflen, 2);
                i += 2;
                memcpy(&pSend[i], databuf, databuflen);
                i += databuflen;
            }
            else    //��ͳģʽ//sujianzhong 2015.01.12 11:28
            {
                memset(databuf, 0, sizeof(databuf));
                sdkAscToBcd(databuf, pstCardInfo.stCardData.asTrack2Data,
                            pstCardInfo.stCardData.ucTrack2Len);
                databuflen = (pstCardInfo.stCardData.ucTrack2Len + 1) / 2;
                TraceHex("zcl", "track2:SRC", databuf, databuflen);

                //����
                if (SDK_OK != sdkPEDWkDes(keyindex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                          databuf + databuflen - 9, databuf + databuflen - 9)) {
                    goto ERROR;
                }
                sdkU16ToBcd(&pSend[i], pstCardInfo.stCardData.ucTrack2Len, 2);
                i += 2;
                sdkBcdToAsc(&pSend[i], databuf, databuflen);
                i += pstCardInfo.stCardData.ucTrack2Len;
//                    memcpy(&pstTransFlow->heSendData[i], databuf, databuflen);
//                    i += databuflen;
                TraceHex("zcl", "track2:encrypt", databuf, databuflen);
            }
#else
            Trace("Lai", "#### start to encrypt track2\r\n");

            if( EncryptTrackData(SerialNumber, randnum, pstCardInfo.stCardData.asTrack2Data,
                                 pstCardInfo.stCardData.ucTrack2Len, &wkcfg,
                                 &pSend[i + 2], &encryptLen) < 0 )
            {
                Trace("Lai", "$$$$$$ MagDataMode & 0x02 fail\r\n");
                goto ERROR;
            }
            sdkU16ToBcd(&pSend[i], encryptLen, 2);
            i += (2 + encryptLen);
#endif
        }
        else {
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
        }
    }

    if (MagDataMode & 0x04)    //3�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack3Len > 0) {
#if 1

            if (pkIndex != 0xFF) {
                memcpy(databuf, SerialNumber, 12);
                databuflen = 12;
                memcpy(databuf + databuflen, randnum, 6);
                databuflen += 6;
                sdkAscToBcd(databuf + databuflen, MposDeviceId, 20);
                databuflen += 10;
                memcpy(databuf + databuflen, pstCardInfo.stCardData.asTrack3Data,
                       pstCardInfo.stCardData.ucTrack3Len);
                databuflen += pstCardInfo.stCardData.ucTrack3Len;
                Des3sEcb(databuf, databuflen, databuf, &databuflen, key);
                sdkU16ToBcd(&pSend[i], databuflen, 2);
                i += 2;
                memcpy(&pSend[i], databuf, databuflen);
                i += databuflen;
            }
            else    //��ͳģʽ//sujianzhong 2015.01.12 11:28
            {
                memset(databuf, 0, sizeof(databuf));
                sdkAscToBcd(databuf, pstCardInfo.stCardData.asTrack3Data,
                            pstCardInfo.stCardData.ucTrack3Len);
                databuflen = (pstCardInfo.stCardData.ucTrack3Len + 1) / 2;
                TraceHex("zcl", "track3:SRC", databuf, databuflen);

                //����
                if (SDK_OK != sdkPEDWkDes(keyindex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                          databuf + databuflen - 9, databuf + databuflen - 9)) {
                    goto ERROR;
                }
                sdkU16ToBcd(&pSend[i], pstCardInfo.stCardData.ucTrack3Len, 2);
                i += 2;
                sdkBcdToAsc(&pSend[i], databuf, databuflen);
                i += pstCardInfo.stCardData.ucTrack3Len;
//                    memcpy(&pstTransFlow->heSendData[i], databuf, databuflen);
//                    i += databuflen;
                TraceHex("zcl", "track2:encrypt", databuf, databuflen);
            }
#else
            Trace("Lai", "#### start to encrypt track3\r\n");

            if( EncryptTrackData(SerialNumber, randnum, pstCardInfo.stCardData.asTrack3Data,
                                 pstCardInfo.stCardData.ucTrack3Len, &wkcfg,
                                 &pSend[i + 2], &encryptLen) < 0 )
            {
                Trace("Lai", "$$$$$$ MagDataMode & 0x04 fail\r\n");
                goto ERROR;
            }
            sdkU16ToBcd(&pSend[i], encryptLen, 2);
            i += (2 + encryptLen);
#endif
        }
        else {
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
        }
    }
    memcpy(&pSend[i], EDSC, 7);
    i += 7;

    memcpy(&pSend[i], MposTrackKsn, 8);  //KSN 10���ֽ�
    i += 8;
    memcpy(&pSend[i], "\x00\x00", 2);  //KSN 10���ֽ�
    i += 2;


    sdkU16ToBcd(&pSend[i], 0, 2);    //��������LL
    i += 2;

    pstTransFlow->usSendDataLen = i;
    gIsMagDataValid = false;    //����֮�����Ч
    AddMposKSN(ADDTRACEKSN);
    return INS_SUC;

    ERROR:
    pSend[0] = 0xFF;    //ʧ��
    pstTransFlow->usSendDataLen = 1;
    return INS_OTHER_ERR;
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����������Ϣ,�ڲ�����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ljh�޸�
 ********************************************************************/
s32 OnReadMagCardEncryptInsideSave(ST_TRANSFLOW *pstTransFlow) {
    u8 ArithmMode = 0, MagDataMode = 0, EncryptMode = 0, TmkIndex;
    u8 cardnoMask[11] = {0}, KeyData[17] = {0}, KeyCheckValue[9] = {0};
    u8 randnum[9] = {0}, SerialNumber[12] = {0};
    SDK_PED_WK_CFG wkcfg;
    s32 ret;
    u8 pkIndex;     //��Կ����
    u8 cardno[25], cardnobuf[25], buf[25] = {0}; //,heTmk[17],heTempdata[8];
    u16 cardnoLen = 0, i = 0, len = 0, j = 0;
    u16 encryptLen;
    u8 EDSC[7] = {0};   //��ƬʧЧ����,�������
    u8 *pEncryptData = NULL;  //�洢�ŵ���������

    u16 tagLenAcc, tagLenTrack1, tagLenTrack2, tagLenTrack3;
    u8 tagAcc[3] = {0}, tagTrack1[3] = {0}, tagTrack2[3] = {0}, tagTrack3[3] = {0};

    u16 trackEncryptLen[3] = {0};  //���ܺ�ĳ���,��0
    u8 trackRealLen[3] = {0};  //���ܺ�ĳ���,δ��0

    u8 cardEncryptData[30] = {0};
    u16 cardEncryptLen;
    MagTlvStr *pTlvStr;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    if (!gIsMagDataValid) {
        Trace("Lai", "$$$$$$ gIsMagDataValid\r\n");
        goto ERROR;
    }
    TraceHex("Lai", "==heReceiveData=\r\n", pstTransFlow->heReceiveData,
             pstTransFlow->ReceiveDataLen);
    gReadCardType = SDK_ICC_MAG;

    memset(&wkcfg, 0, sizeof(wkcfg));

    //��һ���ֽڲ�����ʹ��FIXED
    pkIndex = pstTransFlow->heReceiveData[i++];    //��Կ����
    MagDataMode = pstTransFlow->heReceiveData[i++];    //��ȡ�ŵ���Ϣ

    memcpy(cardnoMask, &pstTransFlow->heReceiveData[i], 10);
    i += 10;
    EncryptMode = pstTransFlow->heReceiveData[i++];    //�����㷨��ʶ ��ʱֻʹ���������㷨

    if (pkIndex != 0x02 && EncryptMode != 0x01) //��ʱֻ֧�����ַ�ʽ
    {
        Trace("Lai", "$$$$$$ pkIndex!=0x02 && EncryptMode!=0x01\r\n");
        goto ERROR;
    }
    TmkIndex = pstTransFlow->heReceiveData[i++];    //��Կ����

    if (TmkIndex > 0) {
        sdkBcdToU16(&len, &pstTransFlow->heReceiveData[i], 2);
        i += 2;

        if (TmkIndex >= MK_3DESTHRESHOLD)        //3des ����Կ������sdk�ײ�������ת��
        {
            TmkIndex -= MK_3DESTHRESHOLD;
            wkcfg.eTmkType = SDK_PED_DES_TRIPLE;
            Trace("Lai", "TMK SDK_PED_DES_TRIPLE\n");
        }
        else {
            wkcfg.eTmkType = SDK_PED_DES_SINGLE;
            Trace("Lai", "TMK SDK_PED_DES_SINGLE\n");
        }

        if (len == 24) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], 16);
            i += 16;
            memcpy(KeyCheckValue, &pstTransFlow->heReceiveData[i], 8);
            i += 8;
            wkcfg.eWkType = SDK_PED_DES_TRIPLE;
            Trace("Lai", "TWK SDK_PED_DES_TRIPLE\n");
        }
        else if (len == 16) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], len);
            i += len;
            wkcfg.eWkType = SDK_PED_DES_TRIPLE;
            Trace("Lai", "TWK SDK_PED_DES_TRIPLE\n");
        }
        else if (len == 8) {
            memcpy(KeyData, &pstTransFlow->heReceiveData[i], len);
            i += len;
            wkcfg.eWkType = SDK_PED_DES_SINGLE;
            Trace("Lai", "TWK SDK_PED_DES_SINGLE\n");
        }
        else {
            Trace("Lai", "$$$$$$$ ��Կ���� ������ %d\r\n", len);
            goto ERROR;
        }
        wkcfg.ucTmkIndex = TmkIndex;      //����Կ����
        wkcfg.ucWkIndex = 0;      //������Կ����
        wkcfg.ucEnWkLen = len;      //������Կ����
        memcpy(wkcfg.heEnWk, KeyData, len);      //������Կ����
        TraceHex("Lai", "KeyData", KeyData, len);
        wkcfg.ucCheckDataLen = 0;
        wkcfg.ucCheckValueLen = 0;
        wkcfg.bOnlyCalcCheckValue = false;

        ret = sdkPEDUpdateWkEx(&wkcfg, SDK_PED_DES_ALL);

        if (ret != SDK_OK) {
            Trace("Lai", "$$$$$$$ sdkPEDUpdateWkEx fail:%d\r\n", ret);
            Trace("Lai",
                  "wkcfg.eTmkType %d wkcfg.eWkType %d wkcfg.ucTmkIndex:%d wkcfg.ucWkIndex:%d wkcfg.ucEnWkLen:%d\r\n",
                  wkcfg.eTmkType, wkcfg.eWkType, wkcfg.ucTmkIndex, wkcfg.ucWkIndex,
                  wkcfg.ucEnWkLen);
            goto ERROR;
        }
    }
    else {
        wkcfg.ucWkIndex = 0;
        wkcfg.eWkType = SDK_PED_DES_TRIPLE;
    }
    //=============================================���������,��ˮ��,Tagֵ
    memcpy(randnum, &pstTransFlow->heReceiveData[i], 8);
    i += 8;
    memcpy(SerialNumber, &pstTransFlow->heReceiveData[i], 12);
    memcpy(gPlatformSerialNO, SerialNumber, 12);
    i += 12;
    ArithmMode = pstTransFlow->heReceiveData[i++];    //Ĭ����0


    sdkBcdToU16(&tagLenAcc, &pstTransFlow->heReceiveData[i], 2);
    i += 2;
    memcpy(tagAcc, &pstTransFlow->heReceiveData[i], tagLenAcc);
    i += tagLenAcc;

    sdkBcdToU16(&tagLenTrack1, &pstTransFlow->heReceiveData[i], 2);
    i += 2;
    memcpy(tagTrack1, &pstTransFlow->heReceiveData[i], tagLenTrack1);
    i += tagLenTrack1;

    sdkBcdToU16(&tagLenTrack2, &pstTransFlow->heReceiveData[i], 2);
    i += 2;
    memcpy(tagTrack2, &pstTransFlow->heReceiveData[i], tagLenTrack2);
    i += tagLenTrack2;

    sdkBcdToU16(&tagLenTrack3, &pstTransFlow->heReceiveData[i], 2);
    i += 2;
    memcpy(tagTrack3, &pstTransFlow->heReceiveData[i], tagLenTrack3);
    i += tagLenTrack3;

#if 0
    TraceHex("Lai", "==cardnoMask=\r\n", cardnoMask, 10);
    TraceHex("Lai", "==KeyData=\r\n", KeyData, 16);
    TraceHex("Lai", "==KeyCheckValue=\r\n", KeyCheckValue, 8);
    TraceHex("Lai", "==randnum=\r\n", randnum, 8);
    TraceHex("Lai", "==SerialNumber=\r\n", SerialNumber, 12);
#endif
    //ɾ���ļ�
    sdkDelFile(MAG_TLV_FILENAME);

    //=========================================***** ���� ******
    //����ÿռ�洢���ܵĴŵ�����,��3���ŵ� �ŵ����104 ������ʱ�������������
    //����һ������ pEncryptData[0] pEncryptData[150] pEncryptData[300]
    pEncryptData = sdkGetMem(TRACK_BUF_SIZE * 3);  //


    if (pEncryptData == NULL) {
        Trace("Lai", "$$$$$$ pEncryptData==NULL\r\n");
        goto ERROR;
    }
    memset(pEncryptData, 0, TRACK_BUF_SIZE * 3);
    memset(cardno, 0x00, sizeof(cardno));
    memset(cardnobuf, 0x00, sizeof(cardnobuf));
    memset(cardnobuf, 'F', 20);

    AppGetMagCardNo(&pstCardInfo.stCardData, cardno);

    if (strlen(cardno) < 13) {
        Trace("Lai", "$$$$ -���������淶E_MAG_ERR---\r\n");
        goto ERROR;
    }
    cardnoLen = strlen(cardno);

    TraceHex("Lai", "## AppGetMagCardNo\r\n", cardno, cardnoLen);

    if ((ret = EncryptHexData(cardno, cardnoLen, &wkcfg, cardEncryptData)) < 0) //�������˺�
    {
        Trace("Lai", "$$$$$$ EncryptHexData fail\r\n");
        goto ERROR;
    }
    else {
        cardEncryptLen = ret;
    }

    if (pstCardInfo.stCardData.ucTrack1Len > 0) {
        if ((ret = EncryptTrackData(SerialNumber, randnum,
                                    pstCardInfo.stCardData.asTrack1Data,
                                    pstCardInfo.stCardData.ucTrack1Len,
                                    &wkcfg, &pEncryptData[0], &encryptLen)) < 0) {
            Trace("Lai", "$$$$$$ MagDataMode & 0x01 fail\r\n");
            goto ERROR;
        }
        else {
            trackRealLen[0] = ret;
            trackEncryptLen[0] = encryptLen;
        }
    }

    if (pstCardInfo.stCardData.ucTrack2Len > 0) {
        GetEDSCFromTrack2(pstCardInfo.stCardData.asTrack2Data, EDSC);

        if ((ret = EncryptTrackData(SerialNumber, randnum,
                                    pstCardInfo.stCardData.asTrack2Data,
                                    pstCardInfo.stCardData.ucTrack2Len,
                                    &wkcfg, &pEncryptData[TRACK_BUF_SIZE], &encryptLen)) < 0) {
            Trace("Lai", "$$$$$$ MagDataMode & 0x02 fail\r\n");
            goto ERROR;
        }
        else {
            trackRealLen[1] = ret;
            trackEncryptLen[1] = encryptLen;
        }
    }

    if (pstCardInfo.stCardData.ucTrack3Len > 0) {
        if ((ret = EncryptTrackData(SerialNumber, randnum,
                                    pstCardInfo.stCardData.asTrack3Data,
                                    pstCardInfo.stCardData.ucTrack3Len,
                                    &wkcfg, &pEncryptData[TRACK_BUF_SIZE * 2], &encryptLen)) < 0) {
            Trace("Lai", "$$$$$$ MagDataMode & 0x04 fail\r\n");
            goto ERROR;
        }
        else {
            trackRealLen[2] = ret;
            trackEncryptLen[2] = encryptLen;
        }
    }
    //==================================================����TLV,д�ļ�

    pTlvStr = sdkGetMem(sizeof(MagTlvStr));

    if (pTlvStr == NULL) {
        Trace("Lai", "$$$$$$ pTlvStr==NULL\r\n");
        goto ERROR;
    }
    memset(pTlvStr, 0, sizeof(MagTlvStr));
    memcpy(pTlvStr->tag, tagAcc, 3);
    pTlvStr->realLen = cardnoLen;
    pTlvStr->dataLen = FormTlv(pTlvStr->data, tagAcc, cardEncryptData, cardEncryptLen);
    sdkAppendFile(MAG_TLV_FILENAME, pTlvStr, sizeof(MagTlvStr));


    memset(pTlvStr, 0, sizeof(MagTlvStr));  //1�ŵ�
    memcpy(pTlvStr->tag, tagTrack1, 3);
    pTlvStr->realLen = trackRealLen[0];
    pTlvStr->dataLen = FormTlv(pTlvStr->data, tagTrack1, &pEncryptData[0], trackEncryptLen[0]);
    sdkAppendFile(MAG_TLV_FILENAME, pTlvStr, sizeof(MagTlvStr));

    memset(pTlvStr, 0, sizeof(MagTlvStr));  //2�ŵ�
    memcpy(pTlvStr->tag, tagTrack2, 3);
    pTlvStr->realLen = trackRealLen[1];
    pTlvStr->dataLen = FormTlv(pTlvStr->data, tagTrack2, &pEncryptData[TRACK_BUF_SIZE],
                               trackEncryptLen[1]);
    sdkAppendFile(MAG_TLV_FILENAME, pTlvStr, sizeof(MagTlvStr));

    memset(pTlvStr, 0, sizeof(MagTlvStr));  //3�ŵ�
    memcpy(pTlvStr->tag, tagTrack3, 3);
    pTlvStr->realLen = trackRealLen[2];
    pTlvStr->dataLen = FormTlv(pTlvStr->data, tagTrack3, &pEncryptData[TRACK_BUF_SIZE * 2],
                               trackEncryptLen[2]);
    sdkAppendFile(MAG_TLV_FILENAME, pTlvStr, sizeof(MagTlvStr));

    sdkFreeMem(pTlvStr);
    //==================================================
    i = 0;
    pSend[i++] = 0x00;    //ˢ������

    memcpy(cardnobuf, cardno, cardnoLen);
    sdkAscToBcd(buf, cardnobuf, 20);

    for (j = 0; j < 10; j++) {
        u8 ismaskd = false;

        if ((cardnoMask[j] & 0xF0) == 0xF0) {
            pSend[i] |= 0xE0;            //���˺�
            ismaskd = true;
        }

        if ((cardnoMask[j] & 0x0F) == 0x0F) {
            pSend[i] |= 0x0E;
            ismaskd = true;
        }

        if (!ismaskd) {
            pSend[i] = buf[j];
        }
        i++;
    }

    memset(cardnobuf, 0x00, sizeof(cardnobuf));
    sdkSHA1(cardno, cardnoLen, cardnobuf);
    memcpy(&pSend[i], cardnobuf, 20);  //��ϣֵ
    i += 20;

    pSend[i] = MagDataMode;    //ģʽ
    i++;

    //=============================================
    if (MagDataMode & 0x01)    //1�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack1Len > 0) {
            sdkU16ToBcd(&pSend[i], trackEncryptLen[0], 2);
            i += 2;
            memcpy(&pSend[i], &pEncryptData[0], trackEncryptLen[0]);
            i += trackEncryptLen[0];

            Trace("Lai", "^^^^^^ track1 ^^^^^^^^^\r\n");
        }
        else {
            Trace("Lai", "^^^^^^ track1 00 ^^^^^^\r\n");
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
        }
    }

    if (MagDataMode & 0x02)    //2�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack2Len > 0) {
            sdkU16ToBcd(&pSend[i], trackEncryptLen[1], 2);
            i += 2;
            memcpy(&pSend[i], &pEncryptData[TRACK_BUF_SIZE], trackEncryptLen[1]);
            i += trackEncryptLen[1];
            Trace("Lai", "^^^^^^ track2 ^^^^^^^^^\r\n");
        }
        else {
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
            Trace("Lai", "^^^^^^ track2 00 ^^^^^^\r\n");
        }
    }

    if (MagDataMode & 0x04)    //3�ŵ�
    {
        if (pstCardInfo.stCardData.ucTrack3Len > 0) {
            sdkU16ToBcd(&pSend[i], trackEncryptLen[2], 2);
            i += 2;
            memcpy(&pSend[i], &pEncryptData[TRACK_BUF_SIZE * 2], trackEncryptLen[2]);
            i += trackEncryptLen[2];
            Trace("Lai", "^^^^^^ track3 ^^^^^^^^^\r\n");
        }
        else {
            sdkU16ToBcd(&pSend[i], 0, 2);
            i += 2;
            Trace("Lai", "^^^^^^ track3 00 ^^^^^^\r\n");
        }
    }
    memcpy(&pSend[i], EDSC, 7);
    i += 7;

    pstTransFlow->usSendDataLen = i;
    gIsMagDataValid = false;    //����֮�����Ч

    sdkFreeMem(pEncryptData);
    return INS_SUC;


    ERROR:
    pSend[0] = 0xFF;    //ʧ��
    pstTransFlow->usSendDataLen = 1;
    sdkFreeMem(pEncryptData);
    return INS_OTHER_ERR;
}

/*******************************************************************
   ��	 ��: wlz
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ˢ��������,������Ƶ�ӿ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: �ݲ�֧��
 ********************************************************************/
s32 OnReadMagCardAudio(ST_TRANSFLOW *pstTransFlow) {
#if 0 //���Զ�ȡ�����TLV
    s32 filesize = 0;
    u16 TlvStrSize = sizeof(MagTlvStr);
    u8 i, tag[2] = {0};
    MagTlvStr *pTlvStr = NULL;

    //sdkAppendFile(MAG_TLV_FILENAME, pTlvStr,sizeof(MagTlvStr));

    filesize = sdkGetFileSize(MAG_TLV_FILENAME);

    if(filesize < 0)
    {
        Trace("mpos", "$$$ sdkGetFileSize filesize < 0\r\n");
        return INS_PARAM_ERR;
    }
    u8 count = filesize / TlvStrSize;
    Trace("Lai", "##### TlvCount:%d TlvStrSize:%d\r\n", count, TlvStrSize);

    if( (pTlvStr = sdkGetMem(TlvStrSize)) == NULL )
    {
        Trace("mpos", "$$$ pBuf==null \r\n");
        return INS_PARAM_ERR;
    }

    for(i = 0; i < count; i++)
    {
        memset(pTlvStr, 0, TlvStrSize);
        filesize = TlvStrSize;
        sdkReadFile(MAG_TLV_FILENAME, pTlvStr, i * TlvStrSize, &filesize);

        if(filesize != TlvStrSize)
        {
            Trace("Lai", "$$$ sdkReadFile fail\r\n");
            sdkFreeMem(pTlvStr);
            return INS_PARAM_ERR;
        }
        TraceHex("Lai", "tag:\r\n", pTlvStr->tag, 2);
        TraceHex("Lai", "TLV:\r\n", pTlvStr->data, pTlvStr->dataLen);
    }

    sdkFreeMem(pTlvStr);
    return INS_SUC;    //

#endif
    return INS_NOSUPPORT;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *lpOut
                               u8 *tag
                               u8 taglen
                               u8 *value
                               u16 valuelen
** Returned value:
** Created By:		zcl  2014.07.03
** Remarks:
*****************************************************************************/
u16 FormTlv(u8 *lpOut, u8 *tag, u8 *value, u16 valuelen) {
    u16 len = 0;

    if ((tag[0] & 0x0F) == 0x0F) {
        memcpy(lpOut, tag, 2);
        len += 2;
    }
    else {
        lpOut[0] = tag[0];
        len++;
    }

    if (valuelen > 0) {
        if (valuelen > 255) {
            lpOut[len++] = 0x82;
            lpOut[len++] = valuelen >> 8;
        }
        else if (valuelen > 127) {
            lpOut[len++] = 0x81;
        }
        lpOut[len++] = valuelen;
        memcpy(&lpOut[len], value, valuelen);
        len += valuelen;
    }
    else {
        lpOut[len++] = 0x00;
    }
    TraceHex("Lai", "TLV:\r\n", lpOut, len);
    return len;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����TLV����,ֻ����һ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void AnalysicsTLV(u8 *pTLVbuf, u16 uiDataLen, u8 *pTag, u8 *lpOutValue, u16 *valuelen) {
    u16 i = 0;
    u8 taglen = 0, Llen;

    u16 L;

    while (i < uiDataLen)    //���˿�ʼ����Ч����,L��value�������򲻹�
    {
        if (pTLVbuf[i] == 0x00)         //invalid data
        {
            i++;
            continue;
        }
        taglen = 0;

        if ((pTLVbuf[i] & 0x0f) == 0x0f)          //�������ֽڵĳ��ȱ�ǩ
        {
            taglen++;
        }
        taglen++;
        i += taglen;

        memcpy(pTag, &pTLVbuf[0], taglen);

        if (pTLVbuf[i] & 0x80) {
            Llen = pTLVbuf[i++] & (~0x80);            //L���ֽ���
            sdkHexToU16(&L, &pTLVbuf[i], Llen);          //��ȡvalue�ĳ���L
            i += Llen;
        }
        else {
            L = pTLVbuf[i++];
        }
        memcpy(lpOutValue, &pTLVbuf[i], L);
        *valuelen = L;

        break;
    }
}

