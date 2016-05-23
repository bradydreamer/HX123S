u8 gbIc1PowerOn = 0;
u8 gbIc2PowerOn = 0;
u8 gbIc3PowerOn = 0;
u8 gbSam1PowerOn = 0;
u8 gbSam2PowerOn = 0;
u8 gbSam3PowerOn = 0;
u8 gbClPowerOn = 0;

const cmd_Str IcCardCmdTab[] = {
        {IC_CHECK,     OnIccCheck,    NULL},//PSAM���޷���״̬,ֻ���ø�λ���жϲ忨���
        {IC_SET_TYPE,  OnIccSetType,  NULL},//
        {IC_POWER_ON,  OnIccPowerOn,  NULL},// �ǽ��ϵ�Ѱ������δ����,ATRֱ�Ӵ������
        {IC_POWER_OFF, OnIccPowerOff, NULL},//
        {IC_COMMUN,    OnIccCommum,   NULL},// ���˽�IC������,δ����
        {IC_CMD_END,   NULL,          NULL}
};

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: IC�����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnIccCheck(ST_TRANSFLOW *pstTransFlow) {
    u8 temp[256], IcStatus[8];
    s32 ret = 0;
    u16 len = 0;

#if 0
    Trace("Lai","%%% IC card OnCheck\r\n");
    sdkIccOpenIcDev();
    SDK_ICC_PARAM stIccParam;
    memset(&stIccParam,0,sizeof(stIccParam));

    ret = sdkIccGetCardStatus(&stIccParam,5);
    if(SDK_OK==ret)
    {
        Trace("Lai","11111111 IC1 in slot");
    }
    else
        Trace("Lai","11111111 IC1 Not in slot %d\r\n",ret);

    sdkIccCloseIcDev();
#endif
#if 1
    memset(IcStatus, 0, sizeof(IcStatus));
    memset(temp, 0, sizeof(temp));

    sdkIccOpenIcDev();
    ret = sdkIccIsIccCardIn(5, temp, &len);//IC��

    if (SDK_OK == ret) {
        TraceHex("Lai", "IC card in slot\r\n", temp, len);
        IcStatus[0] = 0x01;
    }
    else if (SDK_TIME_OUT == ret) {
        Trace("Lai", "$$$ IC card timeout\r\n");
    }


#if 0 //del by sujianzhong 2015.01.13
    SDK_ICC_PARAM stIccParam;

    memset(temp, 0, sizeof(temp));
    memset(&stIccParam, 0, sizeof(SDK_ICC_PARAM));
    stIccParam.ucCardMode = SDK_ICC_ICC;
    stIccParam.eCardType = SDK_ICC_PSAM;
    stIccParam.ucCLType=0x01;
    stIccParam.ucSlotNo = 1;//PSAM1
    ret = sdkIccReset(&stIccParam, temp, &len);
    if(SDK_OK==ret)
    {
        TraceHex("Lai","sdkIccReset psam1 ret ok  \r\n",temp,len);
        IcStatus[3] = 0x01;
    }
    else
    {
        Trace("Lai","sdkIccReset psam1 ret %d  \r\n",ret);
    }

    memset(temp, 0, sizeof(temp));
    stIccParam.ucSlotNo = 2;//PSAM2
    ret = sdkIccReset(&stIccParam, temp, &len);
    if(SDK_OK==ret)
    {
        TraceHex("Lai","psam2 ret ok  \r\n",temp,len);
        IcStatus[4] = 0x01;
    }
    else
    {
        Trace("Lai","psam2 not found ret %d  \r\n",ret);
    }
#endif


    sdkIccOpenRfDev();//�ǽ�
    u8 cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G;
    memset(temp, 0, sizeof(temp));
    ret = sdkIccRFQuery(cardtype, temp, 10);
    sdkIccCloseRfDev();

    if (ret > 0) {
        if (temp[0] == 0) {
            Trace("Lai", "@@@ sdkIccRFQuery found RF  ok, cardtype:%2x cardNum%d %d %d %d\r\n",
                  temp[2], temp[3], temp[4], temp[5], temp[6]);
            IcStatus[6] = 0x01;
        }
        else if (temp[0] == 2) {
            Trace("Lai", "@@@ sdkIccRFQuery no RF card  \r\n");
        }
    }
    else {
        Trace("Lai", "@@@ sdkIccRFQuery ret %d  \r\n", ret);
    }

    sdkIccCloseIcDev();

    pstTransFlow->usSendDataLen = 8;
    memcpy(pstTransFlow->heReceiveData, IcStatus, 8);        //0416Send==Recv
#endif
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����IC������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: ��֧��
 ********************************************************************/
s32 OnIccSetType(ST_TRANSFLOW *pstTransFlow) {
    //��֧��
    return INS_NOSUPPORT;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �ϵ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnIccPowerOn(ST_TRANSFLOW *pstTransFlow) {
    //Trace("Lai","OnPowerOn\r\n");
    u8 cardSlot, cardType;
    u8 temp[64];
    s32 ret;
    s32 len;
    bool isRFCard = false;
    SDK_ICC_PARAM *stIccParam;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    cardSlot = pstTransFlow->heReceiveData[0];
    cardType = pstTransFlow->heReceiveData[1];

    memset(temp, 0, sizeof(temp));
    stIccParam = sdkGetMem(sizeof(SDK_ICC_PARAM));
    if (stIccParam == NULL) {
        return INS_OTHER_ERR;
    }
    memset(stIccParam, 0, sizeof(SDK_ICC_PARAM));
    stIccParam->ucCardMode = SDK_ICC_ICC;
    stIccParam->ucCLType = 0x01;
    switch (cardSlot) {
        case 0:
            stIccParam->eCardType = SDK_ICC_CPU;// CPU���нӴ��ͷǽ�
            stIccParam->ucSlotNo = 0;
            break;
        case 3:
            stIccParam->eCardType = SDK_ICC_PSAM;
            stIccParam->ucSlotNo = 1;
            break;
        case 4:
            stIccParam->eCardType = SDK_ICC_PSAM;
            stIccParam->ucSlotNo = 2;
            break;
        case 6:
            isRFCard = true;
            stIccParam->ucSlotNo = 3;//���ڷǽ� ������
            stIccParam->ucCardMode = SDK_ICC_RF;
            if (cardType == 0)
                stIccParam->eCardType = SDK_ICC_CPU;
            else if (cardType == 1)
                stIccParam->eCardType = SDK_ICC_MIFARE;
            break;
        default:
            sdkFreeMem(stIccParam);
            return INS_PARAM_ERR;
    }


    if (!isRFCard) {
        sdkIccOpenIcDev();
        ret = sdkIccReset(stIccParam, temp, &len);

        if (SDK_OK == ret) {
            //TraceHex("Lai","sdkIccReset ret ok  \r\n",temp,len);
            if (len > 200)
                len = 200;

            sdkU16ToBcd(&pSend[0], len, 2);
            memcpy(&pSend[2], temp, len);
            pstTransFlow->usSendDataLen = len + 2;
        }
        else {
            Trace("Lai", "sdkIccReset fail:%d\r\n", ret);
        }
    }
    else {
        sdkIccOpenRfDev();//�ǽ�
        memset(temp, 0, sizeof(temp));
        //ret = sdkIccRFQuery (cardtype, temp, 5);
        memset(stIccParam, 0, sizeof(SDK_ICC_PARAM));
        SDK_ICC_TRADE_PARAM stIccTradeParam;

        if (sdkIccTransInit(&stIccTradeParam) != SDK_OK)//IC��������ʼ��
        {
            Trace("Lai", "$$$$$ sdkIccTransInit fail\r\n");
            sdkFreeMem(stIccParam);
            return INS_OTHER_ERR;
        }

        ret = sdkIccPowerOnAndSeek(&stIccTradeParam, stIccParam, temp);
        if (SDK_OK != ret) {
            Trace("Lai", "$$$$$ sdkIccPowerOnAndSeek fail:%d\r\n", ret);
            sdkFreeMem(stIccParam);
            return INS_OTHER_ERR;
        }

        ret = sdkIccReset(stIccParam, temp, &len);
        if (SDK_OK == ret) {
            len = temp[0];
            if (len > 0) {
                TraceHex("Lai", "sdkIccPowerOnAndSeek\r\n", temp, len + 1);
                sdkU16ToBcd(&pSend[0], len, 2);
                memcpy(&pSend[2], &temp[1], len);
                pstTransFlow->usSendDataLen = len + 2;
            }
            else {
                Trace("Lai", "rf reset len=0\r\n");
                sdkU16ToBcd(&pSend[0], 0, 2);
                pstTransFlow->usSendDataLen = 2;
                sdkFreeMem(stIccParam);
                return INS_SUC;
            }
        }
        else {
            Trace("Lai", "$$$$$ sdkIccReset fail:%d\r\n", ret);
            sdkFreeMem(stIccParam);
            return INS_OTHER_ERR;
        }
    }
    sdkFreeMem(stIccParam);
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �µ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnIccPowerOff(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "OnPowerOff\r\n");
    if (pstTransFlow->heReceiveData[0] == 0x06) {
        sdkIccOpenRfDev();
        sdkIccPowerDown();//�ǽ��µ�
        sdkIccCloseRfDev();
    }
    else if (pstTransFlow->heReceiveData[0] < 0x06) {
        //�����Ҫ�޸ĵ�
        sdkIccCloseIcDev();
    }

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: 
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����APDU
   ��ڲ���: �������� ����, ������� ����, IC������ز���
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
u32 TransferApdu(u8 *datain, u16 inlen, u8 *dataout, u16 *outlen, SDK_ICC_PARAM *pstIccParam) {
    TraceHex("Lai", "TransferApdu\r\n", datain, inlen);

    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;
    s32 rlt;
    u8 *temp;
    u16 templen;

    temp = (u8 *) sdkGetMem(512);

    if (temp == NULL) {
        return 0x8E03;
    }
    memset(temp, 0, 512);
    memset(&pstApduSend, 0, sizeof(pstApduSend));
    memset(&pstApduResp, 0, sizeof(pstApduResp));

    if (inlen < 5) {
        sdkFreeMem(temp);
        return 0x8E03;
    }
    else if (inlen == 5) {
        memcpy(pstApduSend.Command, datain, 4);
        pstApduSend.Le = datain[4];
    }
    else {
        memcpy(pstApduSend.Command, datain, 4);
        pstApduSend.Lc = datain[4];

        if (pstApduSend.Lc > inlen - 5) {
            sdkFreeMem(temp);
            return 0x8E03;
        }
        else if (pstApduSend.Lc == inlen - 5) {
            memcpy(pstApduSend.DataIn, datain + 5, pstApduSend.Lc);
        }
        else {
            memcpy(pstApduSend.DataIn, datain + 5, pstApduSend.Lc);
            pstApduSend.Le = datain[inlen - 1];
        }
    }
    rlt = sdkIccDealCardData(pstIccParam, &pstApduSend, &pstApduResp);

    if (rlt == SDK_OK) {
        memcpy(temp, pstApduResp.DataOut, pstApduResp.LenOut);
        temp[pstApduResp.LenOut] = pstApduResp.SWA;
        temp[pstApduResp.LenOut + 1] = pstApduResp.SWB;
        templen = pstApduResp.LenOut + 2;
        //templen = FormTlv(buf, "\xFF\x4D", 2, temp, templen); 

        if (*outlen < templen) {
            sdkFreeMem(temp);
            return 0x8E03;
        }
        memcpy(dataout, temp, templen);
        *outlen = templen;
        sdkFreeMem(temp);
        return 0x9000;
    }
    else {
        sdkFreeMem(temp);
        Trace("", "$$$$$$$$$ 6666666666 %d\r\n", rlt);
        return 0x8E03;
    }
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: IC��ͨѶ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnIccCommum(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "OnCommum\r\n");
    u8 cardType = pstTransFlow->heReceiveData[0];
    u16 len;
    u32 ret;
    //u8  tmpBuf[100]={0};
    //s32 outlen=0;
    SDK_ICC_PARAM stIccParam;
    u16 apdulen = 3;

    sdkBcdToU16(&len, &pstTransFlow->heReceiveData[1], 2);

    if (len + 3 != pstTransFlow->ReceiveDataLen) {
        sdkBcdToU16(&len, &pstTransFlow->heReceiveData[2], 2);
        apdulen = 4;
    }

    memset(&stIccParam, 0, sizeof(stIccParam));

    stIccParam.ucCardMode = SDK_ICC_ICC;
    stIccParam.ucCLType = 0x01;

    bool isRfDev = false;

    switch (cardType) {
        case 0:
            stIccParam.eCardType = SDK_ICC_CPU;// CPU���нӴ��ͷǽ�
            stIccParam.ucSlotNo = 0;
            break;
        case 3:
            stIccParam.eCardType = SDK_ICC_PSAM;
            stIccParam.ucSlotNo = 1;
            break;
        case 4:
            stIccParam.eCardType = SDK_ICC_PSAM;
            stIccParam.ucSlotNo = 2;
            break;
        case 6:
            isRfDev = true;
            stIccParam.ucCardMode = SDK_ICC_RF;
            stIccParam.eCardType = SDK_ICC_CPU;
            break;
        case 7:
            isRfDev = true;
            stIccParam.ucCardMode = SDK_ICC_RF;
            stIccParam.eCardType = SDK_ICC_MIFARE;
            break;
        default:
            return INS_PARAM_ERR;

    }
#if 0
    if(!isRfDev)
        sdkIccOpenIcDev();
    else
    {
        sdkIccOpenRfDev();
        u8 cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G;
        memset(tmpBuf, 0, sizeof(tmpBuf));
        ret = sdkIccRFQuery (cardtype, tmpBuf, 10 );

        if(ret>0)
        {
            TraceHex("Lai","sdkIccRFQuery ret data:\r\n",tmpBuf,ret);
        }
        else
            return INS_OTHER_ERR;
    }

    //��Ҫ��λ,������������������ж��ڵ�һ�βŸ�λ�?
    //sdkIccResetIcc

    ret = sdkIccResetIcc(&stIccParam, tmpBuf, &outlen);//sdkIccReset
    if(ret!=SDK_OK)
    {
        Trace("Lai","$$$ sdkIccReset ret:%d\r\n",ret);

//		if(!isRfDev)
//			sdkIccCloseIcDev();
//		else
//			sdkIccCloseRfDev();

        return INS_OTHER_ERR;
    }
    else
    {
        Trace("Lai","sdkIccReset ok..................\r\n");
    }
#endif

    u16 lenOut = 510;
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    ret = TransferApdu(&pstTransFlow->heReceiveData[apdulen], len, &pSend[2], &lenOut, &stIccParam);

//	if(!isRfDev)
//		sdkIccCloseIcDev();
//	else
//		sdkIccCloseRfDev();

    if (0x9000 == ret) {
        TraceHex("Lai", "apdu:\r\n", &pSend[2], lenOut);
    }
    else {
        Trace("Lai", "$$$ TransferApdu ret:%d\r\n", ret);
        return INS_OTHER_ERR;
    }

    sdkU16ToBcd(&pSend[0], lenOut, 2);

    pstTransFlow->usSendDataLen = lenOut + 2;
    return INS_SUC;
}

