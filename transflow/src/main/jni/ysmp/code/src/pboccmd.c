const cmd_Str PbocCmdTab[] = {
        {PBOC_SET_PKEY,     OnSetPubKey,   NULL},
        {PBOC_SET_AID,      OnSetMposAid,  NULL},
        {PBOC_SET_TERATTR,  OnSetTerAttr,  NULL},
        {PBOC_GET_DEALDATA, OnGetTranData, NULL},
        {PBOC_EXE_FLOW,     OnExecStdFlow, NULL},
        {PBOC_SEC_AUTH,     OnPbocSecAuth, NULL},
        {PBOC_END_FLOW,     OnPbocFlowEnd, NULL},
        {PBOC_EXE_QPBOC,    OnQpbocFlow,   NULL},
        {PBOC_GET_LASTDEAL, OnGetLastTran, NULL},
        {PBOC_CMD_END,      NULL,          NULL}
};

//������ȫ�ֱ���

TagList TlvArray;
static u8 gucAmount[13] = {0};
//static u8 gucDf74Buf[32] = {0};
static u8 gucEcBalance[13] = {0};       //�����ֽ����: ʵ������6�ֽ�

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304
static u8 *gpTradeTmp;
#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304 
static SDK_ICC_PARAM gstIccParam;
static SDK_ICC_TRADE_PARAM gstIccTradeParam;

#define LAST_TRADE_FILE                  "/mtd0/res/last_trade.log"
#define LAST_TRADE_TC_FILE       "/mtd0/res/last_trade_tc.log"
#define MPOS_TERMINAL_PARAM_FILE "/mtd0/res/mpos_term_param.cfg"
#define TRANS_SEQ_COUNTER_FILE   "/mtd0/res/trans_seq_counter.log"





//20150330 ��Ӵ�����
#define ERRCODE_SUC             0x00        /*�ɹ�*/
#define ERRCODE_DEC             0x02        /*���׾ܾ�*/
#define ERRCODE_APPECNSF        0xf8        /*����ֻ֧�ֵ����ֽ��ֽ�����*/
#define ERRCODE_ONLYCARDECNSF   0xf9        /*�������ֽ𿨣�����*/
#define ERRCODE_NOSUPEC         0xfa        /*��֧�ֵ����ֽ�*/
#define ERRCODE_OVERLIMTAMT     0xfb        /*���׽������ն��޶�*/
#define ERRCODE_LOSTAID         0xfe        /*�ն�AID������ʧ*/
#define ERRCODE_CARDANOMALY     0xff        /*��Ƭ�����쳣*/

#define ERRCODE_ACCEPT          0x01        /*��������*/
#define ERRCODE_ONLINE          0x03        /*����*/
#define ERRCODE_GAT2AAC         0x04        /*�ڶ���Generate AC����AAC*/
#define ERRCODE_FALLBACK        0xFE        /*FALLBACK*/
#define ERRCODE_ESC             0xFC        /*����ȡ��*/
#define ERRCODE_OVERCARDLIMTAMT 0xF5        /*Ȧ��������Ƭ�������*/
#define ERRCODE_ERR             0xFF        /*����ʧ��*/

u8 gucErrCode;         //ȫ�ֵĽ��״�����

#define MHQ_DEBUG  0
#if 0
static s32 closeReadCard_IccRfMag( u8 ucCardMode )
{
	/*����Ϊ���ƽ��ӣ����׽����ر�����*/
	if( ucCardMode & SDK_ICC_MAG )
	{
        sdkIccCloseMag();
    }
    else if( ucCardMode & SDK_ICC_ICC )
    {
        sdkIccCloseIcDev();
    }
    else if( ucCardMode & SDK_ICC_RF )
    {
        sdkIccCloseRfDev();//����������طǽӣ��طǽӵ��¿�Ƭ����ʧ�ܣ�����Ӧ�ò��ǷǽӵĽ���λ�á�
    }
    Trace("zcl", "*****************************�ر�����*****************************\r\n");
}
#endif

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:����һ�ʽ�����־
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 ReadLastTrans(u8 *pTransLogBuf) {
    s32 iRet;
    s32 iLen = 2;

    if (pTransLogBuf == NULL) {
        return SDK_PARA_ERR;
    }
    iRet = sdkReadFile(LAST_TRADE_FILE, pTransLogBuf, 0, &iLen);
#if MHQ_DEBUG
    TraceHex("Mpos", "Param :\n", pTransLogBuf, 2);
    Trace("Mpos", "LAST_TRADE_FILE iRet = %d \n", iRet);
#endif

    if (iRet != SDK_FILE_OK) {
        pTransLogBuf[0] = pTransLogBuf[1] = 0x00;
        return 0;
    }
    sdkBcdToU32(&iLen, pTransLogBuf, LL_LENGTH);

    iRet = sdkReadFile(LAST_TRADE_FILE, &pTransLogBuf[2], LL_LENGTH, &iLen);
#if MHQ_DEBUG
    TraceHex("Mpos", "Param :\n", pTransLogBuf, iLen);
    Trace("Mpos", "LAST_TRADE_FILE iRet = %d \n", iRet);
#endif

    if (iRet != SDK_FILE_OK) {
        pTransLogBuf[0] = pTransLogBuf[1] = 0x00;
        return 0;
    }
    return iLen + LL_LENGTH;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:д�ϱʽ�����־
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 WriteLastTrans(u8 *pTransLogBuf, s32 iBufLen) {
    s32 iRet;

    if (pTransLogBuf == NULL) {
        return SDK_PARA_ERR;
    }
    iRet = sdkSaveOverlap(LAST_TRADE_FILE, pTransLogBuf, iBufLen);
#if MHQ_DEBUG
    TraceHex("Mpos", "Param :\n", pTransLogBuf, iBufLen);
    Trace("Mpos", "LAST_TRADE_FILE iRet = %d \n", iRet);
#endif

    if (iRet != SDK_FILE_OK) {
        return 0;
    }
    return SDK_FILE_OK;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:����һ������TC������־
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 ReadLastTcFile(u8 *pTcLogBuf) {
    s32 iRet;
    s32 iLen;

    if (pTcLogBuf == NULL) {
        return SDK_PARA_ERR;
    }
    iRet = sdkReadOverlap(LAST_TRADE_FILE, pTcLogBuf, LL_LENGTH);

    if (iRet != SDK_FILE_OK) {
        pTcLogBuf[0] = pTcLogBuf[1] = 0x00;
        return 0;
    }
    sdkBcdToU32(&iLen, pTcLogBuf, LL_LENGTH);

    iRet = sdkReadFile(LAST_TRADE_FILE, &pTcLogBuf[2], LL_LENGTH, &iLen);

    if (iRet != SDK_FILE_OK) {
        pTcLogBuf[0] = pTcLogBuf[1] = 0x00;
        return 0;
    }
    return iLen + LL_LENGTH;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯��׼EMV����TC ��TLV ��,�����ļ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 WriteLastTcFile(void) {
    s32 iRet;
    s32 iValueLen = 0;
    u16 i;
    u16 usOutDataLen = 0;
    u8 *pTmpBuf = NULL;
    u8 ucValue[20] = {0};
    u8 ucStdTagArray[11][3] = {
            {0x08, 0x9F, 0x26},
            {0x02, 0x9F, 0x36},
            {0x04, 0x9F, 0x37},
            {0x05, 0x95},
            {0x03, 0x9A},
            {0x01, 0x9C},
            {0x02, 0xDF, 0x31},
            {0x01, 0xDF, 0x75},
            {0x04, 0xDF, 0x76},
            {0x01, 0xDF, 0x78},
            {0x01, 0xDF, 0x79}
    };

    pTmpBuf = sdkGetMem(256);

    if (pTmpBuf == NULL) {
        return SDK_ERR;
    }
    i = sizeof(ucStdTagArray) / sizeof(ucStdTagArray[0]);

    while (i--) {
        //Tag
        if ((ucStdTagArray[i][1] & 0x1F) == 0x1F) {
            pTmpBuf[usOutDataLen++] = ucStdTagArray[i][1];
            pTmpBuf[usOutDataLen++] = ucStdTagArray[i][2];
        }
        else {
            pTmpBuf[usOutDataLen++] = ucStdTagArray[i][1];
        }
        iRet = sdkIccReadTLV(&ucStdTagArray[i][1], ucValue, &iValueLen);

        if (iRet == SDK_OK) {
            pTmpBuf[usOutDataLen++] = iValueLen;
            memcpy(&pTmpBuf[usOutDataLen], ucValue, iValueLen);
            usOutDataLen += iValueLen;
        }
        else {
            pTmpBuf[usOutDataLen++] = ucStdTagArray[i][0];              //��ȱʡֵ
            memset(&pTmpBuf[usOutDataLen], 0x00,
                   ucStdTagArray[i][0]);            //��������value����
            usOutDataLen += ucStdTagArray[i][0];
        }
    }

    iRet = sdkSaveOverlap(LAST_TRADE_TC_FILE, pTmpBuf, usOutDataLen);
    sdkFreeMem(pTmpBuf);
    return iRet;
}

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:��������ˮ��(3���ֽ�)
   ��ڲ���:
   �� �� ֵ:�ɹ��ͷ���SDK_OK
   ��	 ע: ���������ֽ�BCD ����ˮ��
 ********************************************************************/
static s32 ReadTransSeqCounter(u8 *pTransSeqNo) {
    s32 iRet;
    s32 iLen = 7;
    u8 ucTmp[8] = {0};

    if (pTransSeqNo == NULL) {
        return SDK_PARA_ERR;
    }
    iRet = sdkReadOverlap(TRANS_SEQ_COUNTER_FILE, ucTmp, iLen);

    if (iRet != SDK_FILE_OK) {
        return iRet;
    }
    sdkAscToBcd(pTransSeqNo, ucTmp, 6);

    return SDK_OK;
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
void DispTradeTitle(u8 const *pasTradehead)
{
    u32 displine = SDK_DISP_LINE1;  //�̶���ʾ��һ��

    if (pasTradehead == NULL)
    {
        Assert(0);
        return;
    }
    sdkDispClearScreen(); //zxx 20130723 18:23 ��ʱ��ô����ɣ�390��һ�еĻ����岻�˼�϶
    sdkDispClearRowRam(displine);              //�������
    sdkDispFillRowRam(displine, 0, pasTradehead, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
}
#endif

s32 DispCandAppList(u8 ucCandAppNum, u8 **pheCandApp) {
    SDK_ICC_AIDLIST *pstAidlist = NULL;
    u8 allpage = 0;
    u8 currpage = 0;
    s32 key = 0, iRet = 0;
    u8 dispbuf[256] = {0};
    u8 temp[64] = {0};
    u8 len = 0;
    u8 i = 0, j = 0, selectindex = 0;
    s32 timer = sdkTimerGetId();            //20150322 ��ӳ�ʱ����

    allpage = (ucCandAppNum + 3) / 4;

    for (i = 0, j = 0; i < ucCandAppNum; i++) {
        pstAidlist = (SDK_ICC_AIDLIST * )(*(pheCandApp + i));
        memset(temp, 0, sizeof(temp));

        if (pstAidlist->ucAidLen != 0) {
            sdkAscToBcd(temp, pstAidlist->asAid, pstAidlist->ucAidLen);

            if (!memcmp(temp, "\xA0\x00\x00\x03\x33", 5)) {
                j++;
                selectindex = i + 1;
            }
        }
    }

    if (j == 1) {
        return selectindex;
    }
    //DispSaveScreen();
    //DispClearScreenRam();
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
    DispTradeTitle(" Ӧ��ѡ��");
#endif          //20150403 PBOC���Ƿ�����

    memset(dispbuf, 0, sizeof(dispbuf));
    memset(temp, 0, sizeof(temp));

    for (i = 0; i < 4; i++) {
        if (currpage * 4 + i + 1 > ucCandAppNum) {
            break;
        }
        temp[0] = i + '1';
        temp[1] = '.';

        strcat(dispbuf, temp);
        pstAidlist = (SDK_ICC_AIDLIST * )(*(pheCandApp + currpage * 4 + i));

        if (pstAidlist->ucLocalName == 1)     //Merchant perferred name.
        {
            len = pstAidlist->ucAppLocalNameLen;

            if (len > sizeof(pstAidlist->asAppLocalName)) {
                len = sizeof(pstAidlist->asAppLocalName);
            }
            strncat(dispbuf, pstAidlist->asAppLocalName, len);
        }
        else if (pstAidlist->ucPreferNameLen != 0 &&
                 pstAidlist->ucIcti == 0x01)     //Preferred name in card //huxiang 20090702
        {
            len = pstAidlist->ucPreferNameLen;

            if (len > sizeof(pstAidlist->asPreferName)) {
                len = sizeof(pstAidlist->asPreferName);
            }
            strncat(dispbuf, pstAidlist->asPreferName, len);
        }
        else if (pstAidlist->ucAppLabelLen != 0)     //Label in card
        {
            len = pstAidlist->ucAppLabelLen;

            if (len > sizeof(pstAidlist->asAppLabel)) {
                len = sizeof(pstAidlist->asAppLabel);
            }
            strncat(dispbuf, pstAidlist->asAppLabel, len);
        }
        else        //others display AID
        {
            len = pstAidlist->ucAidLen;
            strncat(temp, pstAidlist->asAid, len);
        }
        dispbuf[strlen(dispbuf)] = '\n';
    }

    sdkDispLabel(SDK_DISP_LINE2, dispbuf, SDK_DISP_LEFT_DEFAULT);


    memset(temp, 0, sizeof(temp));

    while (1) {
        if (sdkTimerIsEnd(timer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
        {
            gucPBOCTransFlg = SDK_TIME_OUT;
            return SDK_TIME_OUT;
        }
        iRet = AppCommuRead(temp, sizeof(temp), 10);

        if (iRet > 0) {
            Trace("Mai", "iRet = %d\n", iRet);
            TraceHex("Mai", "temp \r\n", temp, iRet);

            if (temp[3] == 0x1D && temp[4] == 0x08) {
                PostTransFlow(temp, iRet);
                return SDK_ESC;
            }
        }
        key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                           SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_ENTER |
                           SDK_KEY_MASK_ESC, 100);

        switch (key) {
            case SDK_KEY_ENTER:
            case SDK_KEY_DOWN:

                if (currpage + 1 < allpage) { currpage++; }
                break;

            case SDK_KEY_UP:

                if (currpage > 0) { currpage--; }
                break;

            case SDK_KEY_1:
            case SDK_KEY_2:
            case SDK_KEY_3:
            case SDK_KEY_4:
                //DispRestoreScreen();
                DispClearContentRam();
                //DispTradeLoad("���Ժ�...");
                sdkDispLabel(SDK_DISP_LINE3, "���Ժ�...", SDK_DISP_DEFAULT);
                return (key - '0') + currpage * 4;

                break;

            case SDK_KEY_ESC:
                return SDK_ESC;

            default:
                break;
        }
    }
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:д������ˮ��,��ˮ������һ
   ��ڲ���:
   �� �� ֵ:�ɹ��ͷ���SDK_OK
   ��	 ע:
 ********************************************************************/
static s32 WriteTransSeqCounter(void) {
    s32 iRet;
    s32 iLen = 7;
    u8 ucTmp[8] = "000000";

    if (!sdkAccessFile(TRANS_SEQ_COUNTER_FILE)) {
        iRet = sdkWriteFile(TRANS_SEQ_COUNTER_FILE, ucTmp, iLen);

        if (iRet != SDK_FILE_OK) {
            return iRet;
        }
    }
    iRet = sdkReadOverlap(TRANS_SEQ_COUNTER_FILE, ucTmp, iLen);

    if (iRet != SDK_FILE_OK) {
        return iRet;
    }
    iRet = atoi(ucTmp);

    if ((iRet < 1) || (iRet > 999999)) {
        iRet = 1;
    }
    else {
        iRet++;
    }
    sprintf(ucTmp, "%06d", iRet);

    iRet = sdkSaveOverlap(TRANS_SEQ_COUNTER_FILE, ucTmp, iLen);

    if (iRet != SDK_FILE_OK) {
        return iRet;
    }
    return SDK_OK;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:���ն˽��ײ����ļ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 ReadMposParam(stMposTradeParam *pstTradeParam) {
    s32 iRet;
    s32 iLen;
    u8 *pTmpBuf = NULL;

    pTmpBuf = sdkGetMem(sizeof(stMposTradeParam));
    //memset(pTmpBuf, 0x00, sizeof(stMposTradeParam));    //20150113 modify by yc sdkFreeMem

    if ((pstTradeParam == NULL) || (pTmpBuf == NULL)) {
        sdkFreeMem(pTmpBuf);
        return SDK_PARA_ERR;
    }
    memset(pTmpBuf, 0x00, sizeof(stMposTradeParam));    //20150113 modify by yc sdkFreeMem

    if (!sdkAccessFile(MPOS_TERMINAL_PARAM_FILE)) {
        iRet = sdkWriteFile(MPOS_TERMINAL_PARAM_FILE, pTmpBuf, sizeof(stMposTradeParam));
#if MHQ_DEBUG
        Trace("Mpos", "1��Read: %d\n", iRet);
#endif

        if (iRet != SDK_FILE_OK) {
            sdkFreeMem(pTmpBuf);
            return iRet;
        }
    }
    iLen = sizeof(stMposTradeParam);
    iRet = sdkReadFile(MPOS_TERMINAL_PARAM_FILE, pTmpBuf, 0, &iLen);
#if MHQ_DEBUG
    Trace("Mpos", "2��Read: %d\n", iRet);
#endif

    if (iRet == SDK_FILE_OK) {
        memcpy(pstTradeParam, pTmpBuf, sizeof(stMposTradeParam));
    }
#if MHQ_DEBUG
    TraceHex("Mpos", "Param :\n", pTmpBuf, sizeof(stMposTradeParam));
    TraceHex("Mpos", "Param :\n", pstTradeParam, sizeof(stMposTradeParam));
#endif
    sdkFreeMem(pTmpBuf);
    return iRet;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:д�ն˽��ײ����ļ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 WriteMposParam(stMposTradeParam *pstTradeParam) {
    s32 iRet;
    u8 *pTmpBuf = NULL;

    pTmpBuf = sdkGetMem(sizeof(stMposTradeParam));
    //memcpy(pTmpBuf, pstTradeParam, sizeof(stMposTradeParam));   //20150113 modify by yc sdkFreeMem

    if ((pstTradeParam == NULL) || (pTmpBuf == NULL)) {
        sdkFreeMem(pTmpBuf);
        return SDK_PARA_ERR;
    }
    memset(pTmpBuf, 0x00, sizeof(stMposTradeParam));    //20150113 modify by yc sdkFreeMem

    //TraceHex("Mpos", "Param :\n", pTmpBuf, sizeof(stMposTradeParam));
    iRet = sdkWriteFile(MPOS_TERMINAL_PARAM_FILE, pTmpBuf, sizeof(stMposTradeParam));

#if MHQ_DEBUG
    Trace("Mpos", "2��Write: %d\n", iRet);
    memset(pTmpBuf, 0x00, sizeof(stMposTradeParam));
    sdkReadOverlap(MPOS_TERMINAL_PARAM_FILE, pTmpBuf, sizeof(stMposTradeParam));
    TraceHex("Mpos", "1��Param :\n", pTmpBuf, sizeof(stMposTradeParam));

    s32 iLen = sizeof(stMposTradeParam);
    memset(pTmpBuf, 0x00, sizeof(stMposTradeParam));
    sdkReadFile(MPOS_TERMINAL_PARAM_FILE, pTmpBuf, 0, &iLen);
    TraceHex("Mpos", "2��Param :\n", pTmpBuf, iLen);
#endif
    sdkFreeMem(pTmpBuf);
    return iRet;
}

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ļ������ն˽��ײ���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static void LoadMposParam(void) {
    stMposTradeParam stTradeParam;

    memset(&stTradeParam, 0x00, sizeof(stMposTradeParam));
    ReadMposParam(&stTradeParam);

    TraceHex("Mai", "Mpos Param", &stTradeParam, sizeof(stMposTradeParam));
    TraceHex("Mai", "Mpos cap", stTradeParam.termCap, 3);
    return;

    //��ʱ��֧��

    sdkIccConfigTLV("\x9F\x35", &stTradeParam.termType, 1);
    sdkIccConfigTLV("\x9F\x33", stTradeParam.termCap, 3);
    sdkIccConfigTLV("\x9F\x40", stTradeParam.termAddCap, 5);
    sdkIccConfigTLV("\x9F\x39", &stTradeParam.posEntry, 1);
    sdkIccConfigTLV("\x9F\x01", stTradeParam.acquirerId, 6);
    sdkIccConfigTLV("\x9F\x15", stTradeParam.merchantTypeCode, 2);
    sdkIccConfigTLV("\x9F\x16", stTradeParam.merchantNo, 15);
    sdkIccConfigTLV("\x5F\x2A", stTradeParam.currencyCode, 2);
    sdkIccConfigTLV("\x5F\x36", &stTradeParam.currencyExp, 1);
    sdkIccConfigTLV("\x9F\x3C", stTradeParam.refCurrencyCode, 2);
    sdkIccConfigTLV("\x9F\x3D", &stTradeParam.refCurrencyExp, 1);
    sdkIccConfigTLV("\x9F\x1A", stTradeParam.termCountryCode, 2);
    sdkIccConfigTLV("\x5F\x1E", stTradeParam.ifdSN, 8);
    sdkIccConfigTLV("\x9F\x1C", stTradeParam.termID, 8);
    sdkIccConfigTLV("\x9F\x66", &stTradeParam.termTradeAttri, 4);
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304


/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:��ʼ����׼����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void InitStdTransFlow(void) {
#if 0
    u8 ucTransSeq[3];
    u8 *pTmp = gpTradeTmp;
    u16 usPos = 0;
    s32 iRet;

    memset(gucAmount, 0x00, sizeof(gucAmount));

    usPos = 3;
    TraceHex("Mai", "���׽��\n", pTmp + usPos, 6);
    sdkIccConfigTLV("\x9F\x02", pTmp + usPos, 3);

    sdkBcdToAsc(&gucAmount[0], pTmp + usPos, 6);
    Trace("Mai", "1. >>>>>>>>>>>>>>>>>> ��� gucAmount = %s \r\n", gucAmount);

    usPos += 9;
    sdkIccConfigTLV("\x9F\x03", pTmp + usPos, 6);

    usPos += 8;
    sdkIccConfigTLV("\x9c", pTmp + usPos, 1);

    iRet = ReadTransSeqCounter(ucTransSeq);

    if(iRet != SDK_OK)
    {
        Trace("Mai", "��������ˮ��ʧ�� rlst =%d\n", iRet);
    }
    TraceHex("Mai", "������ˮ��\n", ucTransSeq, 3);

    sdkIccConfigTLV("\x9F\x41", ucTransSeq, 3);
    //LoadMposParam();
#endif
    TagList *pTagList = &TlvArray;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;

    sdkIccConfigTLV("\x9F\x41", "\x00\x00\x00\x00", 4);

    //sdkIccConfigTLV("\x9F\x02", GetTagValue(pTagList, TAG_AMOUNT),  GetTagValueSize(pTagList, TAG_AMOUNT));
    //zcl ��ΪSDK���ƣ����һ��Ҫ��pstIccTradeParam->pasTradeAmount
    sdkIccConfigTLV("\x9F\x03", GetTagValue(pTagList, TAG_AMOUNT_OTHER),
                    GetTagValueSize(pTagList, TAG_AMOUNT_OTHER));

    if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD) {
        sdkIccConfigTLV("\x9C", "\x60", 1);
    }
    else {
        sdkIccConfigTLV("\x9C", GetTagValue(pTagList, TAG_TRANSACTION_TYPE),
                        GetTagValueSize(pTagList, TAG_TRANSACTION_TYPE));
    }
    sdkIccConfigTLV("\x9F\x4E", "CHINAUMS MPOS", strlen("CHINAUMS MPOS"));
}

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:��ʼ��QPBOC ����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
void InitQpbocTransFlow(void) {
    u8 ucTransSeq[4];
    u8 *pTmp = gpTradeTmp;
    u16 usPos = 0;
    s32 iRet;


    usPos = 3;
    sdkBcdToAsc(gstIccTradeParam.pasTradeAmount, pTmp + usPos, 6);

    usPos = 8;
    sdkIccConfigTLV("\x9c", pTmp + usPos, 1);

    iRet = ReadTransSeqCounter(ucTransSeq);

    if (iRet != SDK_OK) {
        Trace("Mai", "���ò��� rlst =%d\n", iRet);
    }
    sdkIccConfigTLV("\x9F\x41", ucTransSeq, 4);
    LoadMposParam();
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �������յ��Ĺ�ԿTLV ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static u8 ParseRecvCapkTlv(SDK_ICC_CAPK_STRUCT *tempCapk, u8 *pData, u16 usDataLen) {
    u16 usTag;
    u16 usValueLen;
    u16 usOffset = 0;

    while (usDataLen > usOffset) {
        sdkHexToU16(&usTag, &pData[usOffset], 2);
        usOffset += 2;

        switch (usTag) {
            case 0x9F06:
                memcpy(tempCapk->RID, &pData[++usOffset], 5);
                usOffset += 5;
                break;

            case 0x9F22:
                tempCapk->CAPKI = pData[++usOffset];
                usOffset++;
                break;

            case 0xDF05:
                memcpy(tempCapk->ExpireDate, &pData[++usOffset], 4);
                usOffset += 4;
                break;

            case 0xDF06:
                tempCapk->HashInd = pData[++usOffset];
                usOffset++;
                break;

            case 0xDF07:
                tempCapk->ArithInd = pData[++usOffset];
                usOffset++;
                break;

            case 0xDF02:
                usValueLen = pData[usOffset] > 0x80 ? pData[++usOffset] : pData[usOffset];
                tempCapk->ModulLen = usValueLen;
                memcpy(tempCapk->Modul, &pData[++usOffset], usValueLen);
                usOffset += 4;
                break;

            case 0xDF04:
                usValueLen = pData[usOffset];
                tempCapk->ExponentLen = usValueLen;
                memcpy(tempCapk->Exponent, &pData[++usOffset], usValueLen);
                usOffset += usValueLen;
                break;

            case 0xDF03:
                usValueLen = pData[usOffset];
                tempCapk->CheckSumLen = usValueLen;
                memcpy(tempCapk->CheckSum, &pData[++usOffset], usValueLen);
                usOffset += usValueLen;
                break;

            default:
                usOffset--;                              //��ʶ�������һ���ֽ�
                break;
        }
    }

#if 0

    TraceHex("Mai", "PubKey->RID", tempCapk->RID, 5);
    TraceHex("Mai", "PubKey->CAPKI", &tempCapk->CAPKI, 1);
    TraceHex("Mai", "PubKey->ExpireDate", tempCapk->ExpireDate, 4);
    TraceHex("Mai", "PubKey->HashInd", &tempCapk->HashInd, 1);
    TraceHex("Mai", "PubKey->ArithInd", &tempCapk->ArithInd, 1);
    Trace("Mai", "PubKey->ModulLen = %d\n", tempCapk->ModulLen);
    TraceHex("Mai", "PubKey->Modul", tempCapk->Modul, tempCapk->ModulLen);
    Trace("Mai", "PubKey->ExponentLen = %d\n", tempCapk->ExponentLen);
    TraceHex("Mai", "PubKey->Exponent", tempCapk->Exponent, tempCapk->ExponentLen);
    Trace("Mai", "PubKey->CheckSumLen = %d\n", tempCapk->CheckSumLen);
    TraceHex("Mai", "PubKey->CheckSum", tempCapk->CheckSum, tempCapk->CheckSumLen);
#endif
    return OK;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �������յ���AID TLV ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static u8 ParseRecvAidTlv(SDK_ICC_EmvSysIni_STRUCT *tempAidInfo, u8 *pData, u16 usDataLen) {
    u16 usTag;
    u16 usOffset = 0;

    while (usDataLen > usOffset) {
        sdkHexToU16(&usTag, &pData[usOffset], 2);
        usOffset += 2;

        switch (usTag) {
            case 0x9F06:
                tempAidInfo->AidLen = pData[usOffset];
                memcpy(tempAidInfo->Aid, &pData[++usOffset], tempAidInfo->AidLen);
                usOffset += tempAidInfo->AidLen;
                break;

            case 0xDF01:
                tempAidInfo->Asi = pData[++usOffset];
                usOffset++;
                break;

            case 0x9F08:
            case 0x9F09:
                memcpy(tempAidInfo->AppVerNum, &pData[++usOffset], 2);
                usOffset += 2;
                break;

            case 0xDF11:
                memcpy(tempAidInfo->TacDefault, &pData[++usOffset], 5);
                usOffset += 5;
                break;

            case 0xDF12:
                memcpy(tempAidInfo->TacOnline, &pData[++usOffset], 5);
                usOffset += 5;
                break;

            case 0xDF13:
                memcpy(tempAidInfo->TacDecline, &pData[++usOffset], 5);
                usOffset += 5;
                break;

            case 0x9F1B:
                memcpy(tempAidInfo->FloorLimit, &pData[++usOffset], 4);
                usOffset += 4;
                break;

            case 0xDF15:
                memcpy(tempAidInfo->Threshold, &pData[++usOffset], 4);
                usOffset += 4;
                break;

            case 0xDF16:
                tempAidInfo->MaxTargetPercent = pData[++usOffset];
                usOffset++;
                break;

            case 0xDF17:
                tempAidInfo->TargetPercent = pData[++usOffset];
                usOffset++;
                break;

            case 0xDF14:
                tempAidInfo->TermDDOLLen = pData[usOffset];
                memcpy(tempAidInfo->TermDDOL, &pData[++usOffset], tempAidInfo->TermDDOLLen);
                usOffset += tempAidInfo->TermDDOLLen;
                break;

            case 0xDF18:
                tempAidInfo->TermPinCap = pData[++usOffset];
                usOffset++;
                break;

            case 0x9F7B:
                memcpy(tempAidInfo->vlptranslimit, &pData[++usOffset], 6);
                usOffset += 6;
                break;

            case 0xDF21:
                memcpy(tempAidInfo->termcvm_limit, &pData[++usOffset], 6);
                usOffset += 6;
                break;

            case 0xDF20:
                memcpy(tempAidInfo->clessofflineamt, &pData[++usOffset], 6);
                usOffset += 6;
                break;

            case 0xDF19:
                memcpy(tempAidInfo->clessofflinelimitamt, &pData[++usOffset], 6);
                usOffset += 6;
                break;

            default:
                usOffset--;                              //��ʶ�������һ���ֽ�
                break;
        }
    }

    return OK;
}

#if 0
/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �������յ��ĵڶ�����֤TLV ��������
   ��ڲ���:
   �� �� ֵ:	-1Ϊ��ʶ�����ݣ�0Ϊ��������
   ��	 ע:
 ********************************************************************/
static u8 ParseSecAuthTlvAndSet(u8 *pData, u16 usDataLen)
{
    u16 usPos = 0;
    u16 usTmp;
    u16 usValueLen;
    u8 *pValue;
    u8 ucTag[2] = {0};

#ifdef zcl_debug

    sdkIccGetOnlineData(SDK_OK, "00", NULL, 0, NULL, 0);
    return SDK_OK;

#else

    while(usPos < usDataLen)
    {
#if 0

        if(pData[usPos] != 0x8A && pData[usPos] != 0x89 && pData[usPos] != 0x71 &&
           pData[usPos] != 0x72 && pData[usPos] != 0x91)
        {
            return SDK_ERR;
        }
#endif
        ucTag[0] = pData[usPos];                        //�ݲ�����TagΪ�����ֽ�

        if(pData[usPos + 1] > 0x80)
        {
            usTmp = (pData[usPos + 1] & 0x7F);
            sdkHexToU16(&usValueLen, &pData[usPos + 2], usTmp);
            usPos += (1 + usTmp);
            pValue = &pData[usPos];
        }
        else
        {
            usValueLen = pData[1];
            pValue = &pData[2];
        }

        if(pData[usPos] != 0x8A && pData[usPos] != 0x89 && pData[usPos] != 0x71 &&
           pData[usPos] != 0x72 && pData[usPos] != 0x91)
        {
            continue;
        }
        else
        {
            sdkIccConfigTLV(ucTag, pValue, usValueLen);
        }
        usPos += usValueLen;
    }

    sdkIccGetOnlineData(SDK_OK, "00", NULL, 0, NULL, 0);
    return SDK_OK;

#endif
}

#endif

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯��ԿTLV ��
   ��ڲ���:
   �� �� ֵ:	������֯�����ݵĳ���
   ��	 ע:
 ********************************************************************/
static u16 FormCapkTlv(SDK_ICC_CAPK_STRUCT *tempCapk, u8 *pOutData) {
    u16 usValueLen = 0;

    memcpy(pOutData, "\x9F\x06\x05", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempCapk->RID, 5);
    usValueLen += 5;

    memcpy(pOutData + usValueLen, "\x9F\x22\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempCapk->CAPKI;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\xDF\x05\x04", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempCapk->ExpireDate, 4);
    usValueLen += 4;

    memcpy(pOutData + usValueLen, "\xDF\x06\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempCapk->HashInd;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\xDF\x07\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempCapk->ArithInd;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\xDF\x02", 2);
    usValueLen += 2;

    if (tempCapk->ModulLen > 0x81) {
        pOutData[usValueLen] = 0x81;
        usValueLen++;
    }
    pOutData[usValueLen] = tempCapk->ModulLen;
    usValueLen++;
    memcpy(pOutData + usValueLen, tempCapk->Modul, tempCapk->ModulLen);
    usValueLen += tempCapk->ModulLen;

    memcpy(pOutData + usValueLen, "\xDF\x04", 2);
    usValueLen += 2;
    pOutData[usValueLen] = tempCapk->ExponentLen;
    usValueLen++;
    memcpy(pOutData + usValueLen, tempCapk->Exponent, tempCapk->ExponentLen);

    memcpy(pOutData + usValueLen, "\xDF\x03", 2);
    usValueLen += 2;
    pOutData[usValueLen] = tempCapk->CheckSumLen;
    usValueLen++;
    memcpy(pOutData + usValueLen, tempCapk->CheckSum, tempCapk->CheckSumLen);
    usValueLen += tempCapk->CheckSumLen;

    return usValueLen;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯AID TLV ��
   ��ڲ���:
   �� �� ֵ:	������֯�����ݵĳ���
   ��	 ע:
 ********************************************************************/
static u16 FormAidTlv(SDK_ICC_EmvSysIni_STRUCT *tempAid, u8 *pOutData) {
    u16 usValueLen = 0;

    memcpy(pOutData, "\x9F\x06", 2);
    usValueLen += 2;
    pOutData[usValueLen] = tempAid->AidLen;
    usValueLen++;
    memcpy(pOutData + usValueLen, tempAid->Aid, tempAid->AidLen);
    usValueLen += tempAid->AidLen;

    memcpy(pOutData + usValueLen, "\xDF\x01\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempAid->Asi;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\x9F\x09\x02", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->AppVerNum, 2);
    usValueLen += 4;

    memcpy(pOutData + usValueLen, "\xDF\x11\x05", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->TacDefault, 5);
    usValueLen += 5;

    memcpy(pOutData + usValueLen, "\xDF\x12\x05", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->TacOnline, 5);
    usValueLen += 5;

    memcpy(pOutData + usValueLen, "\xDF\x13\x05", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->TacDecline, 5);
    usValueLen += 5;

    memcpy(pOutData + usValueLen, "\x9F\x1B\x04", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->FloorLimit, 4);
    usValueLen += 4;

    memcpy(pOutData + usValueLen, "\xDF\x15\x04", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->Threshold, 4);
    usValueLen += 4;

    memcpy(pOutData + usValueLen, "\xDF\x16\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempAid->MaxTargetPercent;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\xDF\x17\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempAid->TargetPercent;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\xDF\x14", 2);
    usValueLen += 2;
    pOutData[usValueLen] = tempAid->TermDDOLLen;
    usValueLen++;
    memcpy(pOutData + usValueLen, tempAid->TermDDOL, tempAid->TermDDOLLen);

    memcpy(pOutData + usValueLen, "\xDF\x18\x01", 3);
    usValueLen += 3;
    pOutData[usValueLen] = tempAid->TermPinCap;
    usValueLen++;

    memcpy(pOutData + usValueLen, "\x9F\x7B\x06", 3);
    usValueLen += 3;
    memcpy(pOutData + usValueLen, tempAid->vlptranslimit, 6);
    usValueLen += 6;

    return usValueLen;
}

//�������̷�ʽ����
//�����8�ı�������8��0��
//������ǣ���0x80,����֮���ٲ�0x00��ֱ��Ϊ8�ı���
//����src,���dst������:len;��Կ:key
//type=1,��Ҫ��0��type=0,����Ҫ��0
unsigned int TDES_UMS(unsigned char *outdata, unsigned char *indata, int inlen,
                      const unsigned char *key, unsigned char type) {
    unsigned int i;
    int datalen;

    //int outlen=0;

    memset(outdata, 0, inlen + 8);
    datalen = inlen;

    memcpy(outdata, indata, inlen);

    if ((inlen % 8 != 0) && (type != 2) && (type != 3)) //��Ϊ8�ı���
    {
        outdata[datalen] = 0x80;
        datalen++;

        if (datalen % 8 != 0) {
            memset(outdata + datalen, 0, 8 - datalen % 8);
            datalen += 8 - datalen % 8;
        }
        else if (type == 1)  //8�ı���֮���Ƿ���Ҫ��0
        {
            memset(outdata + datalen, 0, 8);
            datalen += 8;
        }
    }
    else if (type == 1) {
        memset(outdata + datalen, 0, 8);
        datalen += 8;
    }
    else if (type == 2) {
        datalen = pkcs5Padding(indata, inlen, outdata);
    }
    else if (type == 3) {
        memset(outdata + datalen, 0, 8 - datalen % 8);
        datalen += 8 - datalen % 8;
    }
    TraceHex("zcl", "TDES_UMS", outdata, datalen);

    for (i = 0; i < datalen / 8; i++) {
        sdkDes3S(1, outdata + i * 8, key);
    }

    return datalen;
}

void FormPbocOnlineData(u16 scriptlen) {
    s32 iRet, iValueLen;
    TagList *pTagList = &TlvArray;
    u8 fensan[16] = {0};
    u8 ucTmpTag[2] = {0};
    u8 *ucTmpBuf;
    u16 i, j, count, taglen;
    u8 *pTagStr;
    u32 pTagStrLen;

#if 0
    u16 DF35Dol[] = {TAG_CRYPTOGRAM, TAG_CRYPTOGRAM_INFO_DATA, TAG_ISSUER_APP_DATA, TAG_UNPREDICTABLE_NUMBER,
                     TAG_ATC,        TAG_TVR,                  TAG_TRANSACTION_DATE, TAG_TRANSACTION_TYPE,
                     TAG_AMOUNT,     TAG_TRANS_CURRENCY_CODE, TAG_AIP,               TAG_TERM_COUNTRY_CODE,
                     TAG_AMOUNT_OTHER, TAG_CVM_RESULTS, TAG_TERMINAL_CAPABILITIES, TAG_TERMINAL_TYPE,
                     TAG_IFD_SERIAL_NUMBER, TAG_DF_NAME,        TAG_APP_VERSION_NUMBER, TAG_TRANS_SEQ_COUNTER,
                     TAG_RESPONSE_CODE, TAG_AMOUNT_OTHER};
#endif

    u16 DF37Dol[] = {TAG_CRYPTOGRAM, TAG_CRYPTOGRAM_INFO_DATA, TAG_ISSUER_APP_DATA,
                     TAG_UNPREDICTABLE_NUMBER,
                     TAG_ATC, TAG_TVR, TAG_TRANSACTION_DATE, TAG_TRANSACTION_TYPE,
                     TAG_AMOUNT, TAG_TRANS_CURRENCY_CODE, TAG_AIP, TAG_TERM_COUNTRY_CODE,
                     TAG_AMOUNT_OTHER, TAG_CVM_RESULTS, TAG_TERMINAL_CAPABILITIES,
                     TAG_TERMINAL_TYPE,
                     TAG_IFD_SERIAL_NUMBER, TAG_DF_NAME, TAG_APP_VERSION_NUMBER,
                     TAG_TRANS_SEQ_COUNTER,
                     TAG_RESPONSE_CODE,};

    //u8 key[16] = {0};

    ucTmpBuf = (u8 *) sdkGetMem(512);

    if (ucTmpBuf == NULL) {
        return;
    }
    pTagStr = (u8 *) sdkGetMem(512);

    if (pTagStr == NULL) {
        sdkFreeMem(ucTmpBuf);
        return;
    }
    memset(ucTmpBuf, 0, 512);
    memset(pTagStr, 0, 512);

#if 0
    i = 0;
    pTagStrLen = 0;
    count = sizeof(DF35Dol) / sizeof(DF35Dol[0]);
//	Trace("Mai"," count = %d \r\n", count);

    for(j = 0; j < count; j++)
    {
//		Trace("Mai"," j = %d \r\n", j);

        if(DF35Dol[j] > 0xFF)
        {
            taglen = 2;
        }
        else
        {
            taglen = 1;
        }
        memset(ucTmpTag, 0, sizeof(ucTmpTag));
        sdkU16ToHex(ucTmpTag, DF35Dol[j], taglen);

        iRet = sdkIccReadTLV(ucTmpTag, ucTmpBuf, &iValueLen);

//		TraceHex("Mai","Value: \n", ucTmpTag, 2);
//		TraceHex("Mai","Value: \n", ucTmpBuf, iValueLen);

        if(iRet == SDK_OK && iValueLen != 0)
        {
            memcpy(pTagStr + pTagStrLen,  ucTmpTag, taglen);
            pTagStrLen += taglen;

            if(iValueLen > 0xFF)
            {
                pTagStr[pTagStrLen++] = 0x82;
                sdkU16ToHex(pTagStr + pTagStrLen, iValueLen, 2);
                pTagStrLen += 2;
            }
            else if(iValueLen > 0x7F)
            {
                pTagStr[pTagStrLen++] = 0x81;
                pTagStr[pTagStrLen++] = iValueLen;
            }
            else
            {
                pTagStr[pTagStrLen++] = iValueLen;
            }
            memcpy(pTagStr + pTagStrLen, ucTmpBuf, iValueLen);
            pTagStrLen += iValueLen;
        }
    }

    TraceHex("Mai", "55������DF35", pTagStr, pTagStrLen);

    if(pTagStrLen)
    {
        SetTagValue(0xDF35, pTagStr, pTagStrLen, pTagList);
    }
#endif

    i = 0;
    pTagStrLen = 0;
    count = sizeof(DF37Dol) / sizeof(DF37Dol[0]);
//	Trace("Mai"," count = %d \r\n", count);

    for (j = 0; j < count; j++) {
//		Trace("Mai"," j = %d \r\n", j);

        if (DF37Dol[j] > 0xFF) {
            taglen = 2;
        }
        else {
            taglen = 1;
        }
        memset(ucTmpTag, 0, sizeof(ucTmpTag));
        sdkU16ToHex(ucTmpTag, DF37Dol[j], taglen);

        iRet = sdkIccReadTLV(ucTmpTag, ucTmpBuf, &iValueLen);

//		TraceHex("Mai","Value: \n", ucTmpTag, 2);
//		TraceHex("Mai","Value: \n", ucTmpBuf, iValueLen);

        if (iRet == SDK_OK && iValueLen != 0) {
            memcpy(pTagStr + pTagStrLen, ucTmpTag, taglen);
            pTagStrLen += taglen;

            if (iValueLen > 0xFF) {
                pTagStr[pTagStrLen++] = 0x82;
                sdkU16ToHex(pTagStr + pTagStrLen, iValueLen, 2);
                pTagStrLen += 2;
            }
            else if (iValueLen > 0x7F) {
                pTagStr[pTagStrLen++] = 0x81;
                pTagStr[pTagStrLen++] = iValueLen;
            }
            else {
                pTagStr[pTagStrLen++] = iValueLen;
            }
            memcpy(pTagStr + pTagStrLen, ucTmpBuf, iValueLen);
            pTagStrLen += iValueLen;
        }
    }

    TraceHex("Mai", "�����ܵ�55������DF78", pTagStr, pTagStrLen);

    if (pTagStrLen) {
        //ʹ��KSN����
        //��Կ��ɢ
        memset(fensan, 0x00, sizeof(fensan));
        memcpy(fensan, MposPbocKsn, sizeof(MposPbocKsn));

        for (i = 8; i < 16; i++) {
            fensan[i] = ~fensan[i - 8];
        }

#if 0
        sdkAscToBcd(key, MposAppKey, 32);
        sdkDes3S(1, fensan, key);
        sdkDes3S(1, fensan + 8, key);
#endif

        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8, fensan + 8);

        memcpy(gstPbocKey, fensan, 16);
        TraceHex("Mai", "PBOC KEY", fensan, 16);
        //20150413 ��ӽű��������
        //20150413 ������Ȩʱ���ص�DF78�����ݸ�ʽΪ: 1�ֽڵ�icdata���ܺ�ĳ���+icdata���ܺ������+1�ֽ�scriptdata���ܺ�ĳ���+scriptdata���ܺ������
        //sciptlen ���ڸĳ���Ϊ�Ƿ�Ϊ������Ȩʱ��DF78������־�������������Ľű�����
        if (scriptlen) {
            //����icdata
            count = TDES_UMS(ucTmpBuf + 1, pTagStr, pTagStrLen, fensan, 2); //��Ҫ��λ
            TraceHex("Mai", "���ܺ�55������", ucTmpBuf + 1, count);
            ucTmpBuf[0] = count % 0xFF;     //icdata�ļ��ܳ���

            //����scriptdata            
            iValueLen = (count % 0xFF) + 1;    //���ܺ�scriptdata����ʼƫ��
            memset(pTagStr, 0, 512);
            pTagStrLen = 0;
            sdkIccGetScriptResult(&pTagStrLen, pTagStr + 3);
            Trace("zcl", "�ű����len = %d\r\n", pTagStrLen);
            TraceHex("zcl", "�ű����", pTagStr, pTagStrLen);

            if (pTagStrLen > 0) {
                memcpy(pTagStr, "\xDF\x31", 2);         //tag
                pTagStr[2] = pTagStrLen % 0xFF;         //len
                count = 0;
                pTagStrLen += 3;
                count = TDES_UMS(ucTmpBuf + iValueLen + 1, pTagStr, pTagStrLen, fensan, 2); //��Ҫ��λ
                TraceHex("Mai", "���ܺ�ű����", ucTmpBuf + iValueLen + 1, count);
                ucTmpBuf[iValueLen] = count % 0xFF;     //scriptdata�ļ��ܳ���
                count += iValueLen +
                         1;                   //DF78�����ݳ���: icdata����+scriptdata����+2�ֽ�
            }
            else {
                ucTmpBuf[iValueLen] = 0;                //û�нű����������Ϊ0
                count = iValueLen % 0xFF +
                        1;           //DF78�����ݳ���: icdata����+scriptdata����+2�ֽ�
            }
            TraceHex("Mai", "���ܺ�����DF78", ucTmpBuf, count);
            SetTagValue(0xDF78, ucTmpBuf, count, pTagList);
        }
        else {
            count = TDES_UMS(ucTmpBuf, pTagStr, pTagStrLen, fensan, 2); //��Ҫ��λ
            TraceHex("Mai", "���ܺ�55������DF78", ucTmpBuf, count);
            SetTagValue(0xDF78, ucTmpBuf, count, pTagList);
        }

    }
    memset(ucTmpBuf, 0, 128);
    memcpy(ucTmpBuf, MposPbocKsn, 8);
    memcpy(ucTmpBuf + 10, MposTrackKsn, 8);

    SetTagValue(0xDF79, ucTmpBuf, 20, pTagList);
    AddMposKSN(ADDPBOCKSN);
    sdkFreeMem(ucTmpBuf);
    sdkFreeMem(pTagStr);
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯��׼EMV����TLV ��
   ��ڲ���:
   �� �� ֵ:	������֯�����ݵĳ���
   ��	 ע:    ��ȡ������������0x00
 ********************************************************************/
static u16 FormStdTransTlv(u8 *pOutData, s32 ret) {
    u16 i = 0;
    u16 usTagNum;
    u16 usOutDataLen = 0;
    u16 len = 0;
    s32 iRet;
    s32 iValueLen = 0;
    TagList *pTagList = &TlvArray;
    u8 *ucValue = (u8 *) sdkGetMem(128);           //20150417 ���ֵӦ�ò��ᳬ��128�ֽ� ʵ�ʲ��Իᳬ��64
    u8 fensan[16] = {0}; //, key[16] = {0};
    SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;
    u8 tagvalue;        //20150420 �ñ���ֻ��DF75���õ�
    u8 trackdata[20];
    u8 ucStdTagArray[][2] = {
            {0x9F, 0x41},
            {0x9F, 0x63},
            {0x9F, 0x02},
            {0x9F, 0x74},
            {0x84},
            {0x9F, 0x26},
            {0x5F, 0x24},
            {0x5F, 0x34},
            {0x9F, 0x10},
            {0x9F, 0x37},
            {0x9F, 0x36},
            {0x95},
            {0x9B},
            {0x9C},
            {0x5F, 0x2A},
            {0x82},
            {0x9F, 0x1A},
            {0x9F, 0x33},
            {0x9F, 0x35},
            {0x9F, 0x1E},
            {0x5A},
            {0x9F, 0x06},
            {0x9A},
            {0x9F, 0x21},
            {0x9F, 0x27},
            {0x9F, 0x09},
            {0x8A},
            {0x9F, 0x03},
            /*=======BEGIN: sujianzhong 2015.01.13  22:54 modify===========*/
            {0x50},    //appable
            {0x9F, 0x12},   //appname
            {0x4F},    //AID,��̨����84ȡaid
            /*====================== END======================== */
    };


    u8 ucStdOfflineTagArray[][2] = {
            {0x9F, 0x41},
            {0x9F, 0x63},
            {0x9F, 0x02},
            {0x9F, 0x74},
            {0x84},
            {0x9F, 0x26},
            {0x5F, 0x24},
            {0x57},
            {0x5F, 0x34},
            {0x9F, 0x10},
            {0x9F, 0x37},
            {0x9F, 0x36},
            {0x95},
            {0x9B},
            {0x9C},
            {0x5F, 0x2A},
            {0x82},
            {0x9F, 0x1A},
            {0x9F, 0x33},
            {0x9F, 0x35},
            {0x9F, 0x1E},
            {0x5A},
            {0x9F, 0x06},
            {0x9A},
            {0x9F, 0x21},
            {0x9F, 0x27},
            {0x9F, 0x09},
            {0x8A},
            {0x9F, 0x03},
            {0x9F, 0x79},
            /*=======BEGIN: sujianzhong 2015.01.13  22:54 modify===========*/
            {0x50},    //appable
            {0x9F, 0x12},   //appname
            {0x4F},    //AID,��̨����84ȡaid
            /*====================== END======================== */
    };

    const u8 Field55DOLTab[][2] =        //huxiang add 091127
            {
                    {0x9F, 0x26},           //Ӧ������M
                    {0x9F, 0x27},           //Ӧ��������ϢM
                    {0x9F, 0x10},           //������Ӧ������M
                    {0x9F, 0x37},           //����Ԥ֪����M
                    {0x9F, 0x36},           //Ӧ�ý��׼�����M
                    {0x95},                         //�ն���֤���M
                    {0x9a},                         //��������M
                    {0x9c},                         //��������M
                    {0x9f, 0x02},           //���׽��M
                    {0x5f, 0x2a},           //���׻��Ҵ���M
                    {0x82},                         //Ӧ�ý�������M
                    {0x9f, 0x1a},           //�ն˹��Ҵ���M
                    {0x9f, 0x03},           //�������M
                    {0x9f, 0x33},           //�ն�����M
                    {0x9f, 0x34},           //�ֿ�����֤���O
                    {0x9f, 0x35},           //�ն�����O
                    {0x9f, 0x1e},           //�ӿ��豸���к�C
                    {0x84},                         //ר���ļ�����O
                    {0x9f, 0x09},           //Ӧ�ð汾��O
                    {0x9f, 0x41},           //�������м�����O
                    {0x57},
                    {0x5A},
                    {0x5F, 0x34},
                    /*=======BEGIN: sujianzhong 2015.01.13  22:54 modify===========*/
                    {0x50},    //appable
                    {0x9F, 0x12},   //appname
                    {0x9B},    //tsi
                    {0x4F},    //AID,��̨����84ȡaid
                    {0x9F, 0x06},
                    /*====================== END======================== */
            };

    const u8 TrackDol[][2] =        //huxiang add 091127
            {
                    {0x57},
                    {0x5A},
            };

    if (ucValue == NULL) {
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\xFF", 4);
        usOutDataLen += 4;
        return usOutDataLen;
    }
    memset(ucValue, 0, 64);      //512+128

    Trace("zcl", "IsQmfMode = %d\r\n", IsQmfMode);
    //��ҵ�汾��ȫ�񸶰汾ͨ��(ȫ���人����Ҫ������ֽ�����ѯҲҪ�Ϳ���,��ҵ����ʱ������ǰ�Ĵ���)
    if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_QUERY) {
        if (ret != SDK_OK) {
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\xFF", 4);
            usOutDataLen += 4;
            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
        else {
            if (IsQmfMode == 1)          //��ҵ����ʱ����֮ǰ�Ĵ���,ȫ�񸶰����ӿ��ŵ�һЩtag
            {
                if (pstIccParam->ucCardMode == SDK_ICC_RF) {
                    iValueLen = 0;
                    sdkIccReadTLV("\x9F\x79", ucValue, &iValueLen);
                    if (iValueLen <= 0) {
                        iValueLen = 0;
                        sdkIccSendGetData(pstIccParam, "\x9F\x79", ucValue, &iValueLen);
                        if (iValueLen) {
                            sdkIccConfigTLV("\x9F\x79", ucValue, 6);
                        }
                    }

                    iValueLen = strlen(&pstCardInfo.stCardData.asCardNO);
                    strcpy(ucValue, &pstCardInfo.stCardData.asCardNO);
                    strcat(ucValue, "F");
                    iValueLen = (iValueLen + 1) / 2 * 2;
                    TraceHex("zcl", "ucValue", ucValue, iValueLen);
                    sdkAscToBcd(trackdata, ucValue, iValueLen);
                    TraceHex("zcl", "trackdata", trackdata, iValueLen / 2);
                    sdkIccConfigTLV("\x5A", trackdata, iValueLen / 2);
                }
                else {
                    iValueLen = 0;
                    sdkIccSendGetData(pstIccParam, "\x9F\x79", ucValue, &iValueLen);

                    if (iValueLen) {
                        sdkIccConfigTLV("\x9F\x79", ucValue, 6);
                    }
                }

                {
                    i = 0;
                    usTagNum = sizeof(Field55DOLTab) / sizeof(Field55DOLTab[0]);

                    FormTLVFromDOLTab(Field55DOLTab, usTagNum, pOutData + usOutDataLen, &i);
                    usOutDataLen += i;

                    memcpy(pOutData + usOutDataLen, "\xDF\x35", 2);
                    len = usOutDataLen;
                    usOutDataLen += 2;

                    if (len > 0xFF) {
                        pOutData[usOutDataLen++] = 0x82;
                        sdkU16ToHex(pOutData + usOutDataLen, len, 2);
                        usOutDataLen += 2;
                    }
                    else if (len > 0x7F) {
                        pOutData[usOutDataLen++] = 0x81;
                        pOutData[usOutDataLen++] = len;
                    }
                    else {
                        pOutData[usOutDataLen++] = len;
                    }
                    memcpy(pOutData + usOutDataLen, pOutData, len);
                    usOutDataLen += len;
                }
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\x00", 4);
            usOutDataLen += 4;
            iValueLen = 0;
            memcpy(pOutData + usOutDataLen, "\x9F\x79\x06", 3);
            usOutDataLen += 3;
            sdkIccReadTLV("\x9F\x79", ucValue, &iValueLen);

            if (iValueLen) {
                memcpy(pOutData + usOutDataLen, ucValue, 6);
                usOutDataLen += 6;

                memcpy(pOutData + usOutDataLen, "\x9F\x5D\x06", 3);
                usOutDataLen += 3;
                memcpy(pOutData + usOutDataLen, ucValue, 6);
                usOutDataLen += 6;
            }
            else {
                memcpy(pOutData + usOutDataLen, "\x00\x00\x00\x00\x00\x00", 6);
                usOutDataLen += 6;
            }
            //9F77 9F78
            iValueLen = 0;
            memcpy(pOutData + usOutDataLen, "\x9F\x77\x06", 3);
            usOutDataLen += 3;
            sdkIccSendGetData(pstIccParam, "\x9F\x77", ucValue, &iValueLen);

            if (iValueLen) {
                memcpy(pOutData + usOutDataLen, ucValue, 6);
                usOutDataLen += 6;
            }
            else {
                memcpy(pOutData + usOutDataLen, "\x00\x00\x00\x00\x00\x00", 6);
                usOutDataLen += 6;
            }
            iValueLen = 0;
            memcpy(pOutData + usOutDataLen, "\x9F\x78\x06", 3);
            usOutDataLen += 3;
            sdkIccSendGetData(pstIccParam, "\x9F\x78", ucValue, &iValueLen);

            if (iValueLen) {
                memcpy(pOutData + usOutDataLen, ucValue, 6);
                usOutDataLen += 6;
            }
            else {
                memcpy(pOutData + usOutDataLen, "\x00\x00\x00\x00\x00\x00", 6);
                usOutDataLen += 6;
            }
            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
    }
    else if (pstIccTradeParam->eFlowMode == SDK_ICC_READLOG) {
        usOutDataLen = 0;
        iRet = QueryTradeLog(pstIccParam, ucValue, &len);

        if (iRet == SDK_ReadLogEnd) {
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\x00", 4);
            usOutDataLen += 4;
            memcpy(pOutData + usOutDataLen, "\xDF\x77", 2);
            usOutDataLen += 2;

            if (len > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, len, 2);
                usOutDataLen += 2;
            }
            else if (len > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = len;
            }
            else {
                pOutData[usOutDataLen++] = len;
            }
            memcpy(pOutData + usOutDataLen, ucValue, len);
            usOutDataLen += len;
        }
        else {
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\xFF", 4);
            usOutDataLen += 4;
        }
        sdkFreeMem(ucValue);
        return usOutDataLen;
    }
    //������������ҵ���ȫ�񸶰汾������

    //��ҵ��
    if (IsQmfMode == 0) {
        //zcl ��ͳ55
        if (ret == SDK_TransOfflineApprove || ret == SDK_OnlineWait) {
            i = 0;
            usTagNum = sizeof(Field55DOLTab) / sizeof(Field55DOLTab[0]);

            FormTLVFromDOLTab(Field55DOLTab, usTagNum, pOutData + usOutDataLen, &i);
            usOutDataLen += i;

            memcpy(pOutData + usOutDataLen, "\xDF\x35", 2);
            len = usOutDataLen;
            usOutDataLen += 2;

            if (len > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, len, 2);
                usOutDataLen += 2;
            }
            else if (len > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = len;
            }
            else {
                pOutData[usOutDataLen++] = len;
            }
            memcpy(pOutData + usOutDataLen, pOutData, len);
            usOutDataLen += len;

            if (ret == SDK_TransOfflineApprove) {
                tagvalue = 0x01;
            }
            else {
                tagvalue = 0x03;
            }

            if (GetTagValueSize(pTagList, 0xDF75) == 1) {
                memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
            usOutDataLen += 3;
            pOutData[usOutDataLen++] = tagvalue;

            if (ret == SDK_TransOfflineApprove) {
                iValueLen = 0;
                iRet = sdkIccReadTLV("\x9F\x79", ucValue, &iValueLen);

                if (iRet == SDK_OK && iValueLen != 0) {
                    memcpy(pOutData + usOutDataLen, "\x9F\x79\x06", 3);
                    usOutDataLen += 3;
                    memcpy(pOutData + usOutDataLen, ucValue, 6);
                    usOutDataLen += 6;
                }
            }
            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
        else if (ret == SDK_IccEasyFlow) {
            i = 0;
            usTagNum = sizeof(TrackDol) / sizeof(TrackDol[0]);

            FormTLVFromDOLTab(TrackDol, usTagNum, pOutData + usOutDataLen, &i);
            usOutDataLen += i;

            tagvalue = 0;

            if (GetTagValueSize(pTagList, 0xDF75) == 1) {
                memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
            usOutDataLen += 3;
            pOutData[usOutDataLen++] = tagvalue;

            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
        else {
            tagvalue = 0xFF;

            if (GetTagValueSize(pTagList, 0xDF75) == 1) {
                memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
            usOutDataLen += 3;
            pOutData[usOutDataLen++] = tagvalue;

            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
    }
    //ȫ�񸶰汾
    if (ret == SDK_TransOfflineApprove) {
        i = 0;
        usTagNum = sizeof(ucStdOfflineTagArray) / sizeof(ucStdOfflineTagArray[0]);

        FormTLVFromDOLTab(ucStdOfflineTagArray, usTagNum, pOutData + usOutDataLen, &i);
        usOutDataLen += i;

#if 0
        memcpy(pOutData + usOutDataLen, "\xDF\x76\x04\x00\x00\x00\x00", 7);
        usOutDataLen += 7;
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\x00\x00\x00\x00", 4);
        usOutDataLen += 4;
#endif

        tagvalue = 0x01;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;
        pOutData[usOutDataLen++] = 0x57;
        iRet = sdkIccReadTLV("\x57", ucValue, &iValueLen);
        pOutData[usOutDataLen++] = iValueLen;
        memcpy(pOutData + usOutDataLen, ucValue, iValueLen);
        usOutDataLen += iValueLen;

        //zcl 20150409 �ѻ�ҲҪ��DF78��DF79
        FormPbocOnlineData(0);

        iRet = GetTagValueSize(pTagList, 0xDF78);

        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x78;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF78), iRet);
            usOutDataLen += iRet;
        }
        iRet = GetTagValueSize(pTagList, 0xDF79);

        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x79;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF79), iRet);
            usOutDataLen += iRet;
        }
        //end
        sdkFreeMem(ucValue);
        return usOutDataLen;
    }
    else if (ret == SDK_OnlineWait) {
        /*=======BEGIN: zcl 2015.01.27  17:11 modify ������Ҫ��5A,����APP��û����Ӧ===========*/
        if (pstIccParam->ucCardMode == SDK_ICC_RF) {
            iValueLen = strlen(&pstCardInfo.stCardData.asCardNO);
            strcpy(ucValue, &pstCardInfo.stCardData.asCardNO);
            strcat(ucValue, "F");
            iValueLen = (iValueLen + 1) / 2 * 2;
            TraceHex("zcl", "ucValue", ucValue, iValueLen);
            sdkAscToBcd(trackdata, ucValue, iValueLen);
            TraceHex("zcl", "trackdata", trackdata, iValueLen / 2);
            sdkIccConfigTLV("\x5A", trackdata, iValueLen / 2);
        }
        /*====================== END======================== */

        FormPbocOnlineData(0);


#if 0
        iRet = GetTagValueSize(pTagList, 0xDF35);
        Trace("Mai", "DF35 len = %d\r\n", iRet);

        if(0 != iRet)
        {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x35;

            if(iRet > 0xFF)
            {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if(iRet > 0x7F)
            {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else
            {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF35), iRet);
            usOutDataLen += iRet;

//                TraceHex("Mai", "DF35",  GetTagValue(pTagList, 0xDF35), iRet);
        }
#endif

        iRet = GetTagValueSize(pTagList, 0xDF78);
        Trace("Mai", "ȡDF78 ICdata: iRet:%d\r\n", iRet);
        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x78;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF78), iRet);
            usOutDataLen += iRet;
        }
        iRet = GetTagValueSize(pTagList, 0xDF79);

        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x79;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF79), iRet);
            usOutDataLen += iRet;
        }
        tagvalue = 0x03;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;

        i = 0;
        usTagNum = sizeof(ucStdTagArray) / sizeof(ucStdTagArray[0]);

        FormTLVFromDOLTab(ucStdTagArray, usTagNum, pOutData + usOutDataLen, &i);
        usOutDataLen += i;

#if 0
        memcpy(pOutData + usOutDataLen, "\x9F\x34\x03\x42\x00\x00", 6);
        usOutDataLen += 6;
#endif

        memset(fensan, 0x00, sizeof(fensan));
        memcpy(fensan, MposTrackKsn, 8);

        for (i = 8; i < 16; i++) {
            fensan[i] = ~fensan[i - 8];
        }

#if 0
        sdkAscToBcd(key, MposAppKey, 32);
        sdkDes3S(1, fensan, key);
        sdkDes3S(1, fensan + 8, key);
#endif

        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8, fensan + 8);

        //memcpy(key, fensan, 16);

        sdkBcdToAsc(ucValue, gPlatformSerialNO, 6);
        memcpy(ucValue + 12, gPlatformSerialNO + 6, 6);
        sdkAscToBcd(ucValue + 18, MposDeviceId, 20);
        TraceHex("Mai", "gPlatformSerialNO", gPlatformSerialNO, 12);

        iRet = sdkIccReadTLV("\x57", trackdata, &iValueLen);

        if (iRet == SDK_OK && iValueLen != 0) {
            sdkBcdToAsc(ucValue + 28, trackdata, iValueLen);
            iValueLen *= 2;
            iValueLen += 28;

            if (ucValue[iValueLen - 1] == 'F' || ucValue[iValueLen - 1] == 'f') {
                iValueLen--;
            }

            for (i = 28; i < iValueLen; i++) {
                if ((ucValue[i] == 'D') || (ucValue[i] == 'd')) {
                    ucValue[i] = '=';
                    break;
                }
            }

            TraceHex("Mai", "ucValue", ucValue, iValueLen);
            pOutData[usOutDataLen++] = 0x57;
            Des3sEcb(ucValue, iValueLen, &pOutData[usOutDataLen + 1], &iValueLen, fensan);
            //iValueLen = TDES_UMS(&pOutData[usOutDataLen+1], ucValue,iValueLen,fensan,2);//��Ҫ��λ
            pOutData[usOutDataLen] = iValueLen;
            usOutDataLen += (iValueLen + 1);
        }
        sdkFreeMem(ucValue);
        AddMposKSN(ADDTRACEKSN);
        return usOutDataLen;
    }
    else if (ret == SDK_IccEasyFlow) {
        memset(fensan, 0x00, sizeof(fensan));
        memcpy(fensan, MposTrackKsn, 8);

        for (i = 8; i < 16; i++) {
            fensan[i] = ~fensan[i - 8];
        }

#if 0
        sdkAscToBcd(key, MposAppKey, 32);
        sdkDes3S(1, fensan, key);
        sdkDes3S(1, fensan + 8, key);
#endif

        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
        sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8, fensan + 8);


        //memcpy(key, fensan, 16);

        sdkBcdToAsc(ucValue, gPlatformSerialNO, 6);
        memcpy(ucValue + 12, gPlatformSerialNO + 6, 6);
        sdkAscToBcd(ucValue + 18, MposDeviceId, 20);
        TraceHex("Mai", "gPlatformSerialNO", gPlatformSerialNO, 12);

        iRet = sdkIccReadTLV("\x57", trackdata, &iValueLen);

        if (iRet == SDK_OK && iValueLen != 0) {
            sdkBcdToAsc(ucValue + 28, trackdata, iValueLen);
            iValueLen *= 2;
            iValueLen += 28;

            if (ucValue[iValueLen - 1] == 'F' || ucValue[iValueLen - 1] == 'f') {
                iValueLen--;
            }

            for (i = 28; i < iValueLen; i++) {
                if ((ucValue[i] == 'D') || (ucValue[i] == 'd')) {
                    ucValue[i] = '=';
                    break;
                }
            }

            TraceHex("Mai", "ucValue", ucValue, iValueLen);
            pOutData[usOutDataLen++] = 0x57;
            Des3sEcb(ucValue, iValueLen, &pOutData[usOutDataLen + 1], &iValueLen, fensan);
            //iValueLen = TDES_UMS(&pOutData[usOutDataLen+1], ucValue,iValueLen,fensan,2);//��Ҫ��λ
            pOutData[usOutDataLen] = iValueLen;
            usOutDataLen += (iValueLen + 1);
        }
        tagvalue = 0;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;

        sdkFreeMem(ucValue);
        AddMposKSN(ADDTRACEKSN);
        return usOutDataLen;
    }
    else {
        tagvalue = 0xFF;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;

        sdkFreeMem(ucValue);
        return usOutDataLen;
    }
    sdkFreeMem(ucValue);
    return usOutDataLen;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯�ڶ�����֤����TLV ��
   ��ڲ���:
   �� �� ֵ:	������֯�����ݵĳ���
   ��	 ע: ��ȡ������������0x00
 ********************************************************************/
static u16 FormSecAuthTlv(u8 *pOutData, u8 ret, u16 scriptlen) {
    u16 i = 0;
    u16 usTagNum;
    u16 usOutDataLen = 0;
    s32 iRet;
    s32 iValueLen = 0;
    TagList *pTagList = &TlvArray;
    u8 *ucValue = (u8 *) sdkGetMem(512);       //512+128 �ĳ�512
    //u8 fensan[16] = {0}, key[16] = {0};

#if 0
    u8 ucStdTagArray[][2] = {
        {0x9F, 0x41},
        {0x9F, 0x40}, {0x82}, {0x84}, {0x9A},
        {0x9b},       {0x9C},               {0x95},           {0x9F, 0x77},
        {0x9F, 0x78}, {0x9F, 0x79}, {0x9F, 0x06}, /*{0x06, 0x9F, 0x03},
                                                     {0x06, 0x9F, 0x02}, */{0x4F}, {0x9F, 0x09}, /*{, 0xDF, 0x76},{, 0xDF, 0x75},*/
        { 0x5F, 0x34}, {0x5A, }, {0x5F, 0x24},
        /*=======BEGIN: sujianzhong 2015.01.13  23:17 modify===========*/
        {0x9B},
        {0x4F},    //AID,��̨����84ȡaid
        {0x9F, 0x06},
        /*====================== END======================== */
        {0x9F, 0x36},
    };
#endif
    u8 ucStdTagArray[][2] = {
            {0x9F, 0x41},
            {0x9F, 0x63},
            {0x9F, 0x02},
            {0x9F, 0x74},
            {0x84},
            {0x9F, 0x26},
            {0x5F, 0x24},
            {0x5F, 0x34},
            {0x9F, 0x10},
            {0x9F, 0x37},
            {0x9F, 0x36},
            {0x95},
            {0x9B},
            {0x9C},
            {0x5F, 0x1A},
            {0x82},
            {0x9F, 0x1A},
            {0x9F, 0x33},
            {0x9F, 0x35},
            {0x9F, 0x1E},
            {0x5A},
            {0x9F, 0x06},
            {0x9A},
            {0x9F, 0x21},
            {0x9F, 0x27},
            {0x9F, 0x09},
            {0x8A},
            {0x9F, 0x03},
            /*=======BEGIN: sujianzhong 2015.01.13  22:54 modify===========*/
            {0x50},    //appable
            {0x9F, 0x12},   //appname
            {0x9B},    //tsi
            {0x4F},    //AID,��̨����84ȡaid
            {0x9F, 0x06},
            /*====================== END======================== */
    };
    u8 tagvalue;
    //SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;
    u16 len;
    u8 Field55DOLTab[][2] =         //huxiang add 091127
            {
                    {0x9F, 0x26},           //Ӧ������M
                    {0x9F, 0x27},           //Ӧ��������ϢM
                    {0x9F, 0x10},           //������Ӧ������M
                    {0x9F, 0x37},           //����Ԥ֪����M
                    {0x9F, 0x36},           //Ӧ�ý��׼�����M
                    {0x95},                         //�ն���֤���M
                    {0x9a},                         //��������M
                    {0x9c},                         //��������M
                    {0x9f, 0x02},           //���׽��M
                    {0x5f, 0x2a},           //���׻��Ҵ���M
                    {0x82},                         //Ӧ�ý�������M
                    {0x9f, 0x1a},           //�ն˹��Ҵ���M
                    {0x9f, 0x03},           //�������M
                    {0x9f, 0x33},           //�ն�����M
                    {0x9f, 0x34},           //�ֿ�����֤���O
                    {0x9f, 0x35},           //�ն�����O
                    {0x9f, 0x1e},           //�ӿ��豸���к�C
                    {0x84},                         //ר���ļ�����O
                    {0x9f, 0x09},           //Ӧ�ð汾��O
                    {0x9f, 0x41},           //�������м�����O
                    {0x57},
                    {0x5A},
                    {0x5F, 0x34},
                    /*=======BEGIN: sujianzhong 2015.01.13  23:17 modify===========*/
                    {0x9B},
                    {0x4F},    //AID,��̨����84ȡaid
                    {0x9F, 0x06},
                    /*====================== END======================== */
            };


    if (ucValue == NULL) {
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\xFF", 4);
        usOutDataLen += 4;
        return usOutDataLen;
    }

    //��ҵ��
    if (IsQmfMode == 0) {
        //zcl ��ͳ55
        if (ret == SDK_TransOnlineApprove || ret == SDK_TransOK_ARPCErr) {
            i = 0;
            usTagNum = sizeof(Field55DOLTab) / sizeof(Field55DOLTab[0]);

            while (i < usTagNum) {
                iRet = sdkIccReadTLV(&Field55DOLTab[i][0], ucValue, &iValueLen);

                if (iRet == SDK_OK && iValueLen != 0) {
                    //Tag
                    if ((Field55DOLTab[i][0] & 0x1F) == 0x1F) {
                        pOutData[usOutDataLen++] = Field55DOLTab[i][0];
                        pOutData[usOutDataLen++] = Field55DOLTab[i][1];
                    }
                    else {
                        pOutData[usOutDataLen++] = Field55DOLTab[i][0];
                    }

                    if (iValueLen > 0xFF) {
                        pOutData[usOutDataLen++] = 0x82;
                        sdkU16ToHex(pOutData + usOutDataLen, iValueLen, 2);
                        usOutDataLen += 2;
                    }
                    else if (iValueLen > 0x7F) {
                        pOutData[usOutDataLen++] = 0x81;
                        pOutData[usOutDataLen++] = iValueLen;
                    }
                    else {
                        pOutData[usOutDataLen++] = iValueLen;
                    }
                    memcpy(&pOutData[usOutDataLen], ucValue, iValueLen);
                    usOutDataLen += iValueLen;
                }
                else {
                    //û�оͲ���
                    //pOutData[usOutDataLen++] = ucStdTagArray[i][0];	//��ȱʡֵ
                    //memset(&pOutData[usOutDataLen], 0x00, ucStdTagArray[i][0]);//��������value����
                    //usOutDataLen += ucStdTagArray[i][0];
#if 0

                    if(!memcmp(&Field55DOLTab[i][0], "\x9F\x41", 2))
                    {
                        memcpy(pOutData + usOutDataLen, "\x9F\x41\x04\x00\x00\x01\x48", 7);
                        usOutDataLen += 7;
                    }
                    else if(!memcmp(&Field55DOLTab[i][0], "\x9F\x79", 2))
                    {
                        memcpy(pOutData + usOutDataLen, "\x9F\x79\x06\x00\x00\x00\x00\x00\x00", 9);
                        usOutDataLen += 9;
                    }
                    else if(!memcmp(&Field55DOLTab[i][0], "\xDF\x76", 2))
                    {
                        memcpy(pOutData + usOutDataLen, "\xDF\x76\x04\x00\x00\x00\x00", 7);
                        usOutDataLen += 7;
                    }
#if 0
                    else if(!memcmp(&ucStdTagArray[i][1], "\xDF\x75", 2))
                    {
                        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01\x03", 4);
                        usOutDataLen += 4;
                    }
#endif
                    else if(!memcmp(&Field55DOLTab[i][0], "\xDF\x37", 2))
                    {
                        memcpy(pOutData + usOutDataLen, "\xDF\x37\x01\x03", 4);
                        usOutDataLen += 4;
                    }
#endif
                }
                i++;
            }

            memcpy(pOutData + usOutDataLen, "\xDF\x35", 2);
            len = usOutDataLen;
            usOutDataLen += 2;

            if (len > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, len, 2);
                usOutDataLen += 2;
            }
            else if (len > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = len;
            }
            else {
                pOutData[usOutDataLen++] = len;
            }
            memcpy(pOutData + usOutDataLen, pOutData, len);
            usOutDataLen += len;

            tagvalue = 0x01;

            if (GetTagValueSize(pTagList, 0xDF75) == 1) {
                memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
            usOutDataLen += 3;
            pOutData[usOutDataLen++] = tagvalue;

            if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD) {
                iValueLen = 0;
                iRet = sdkIccReadTLV("\x9F\x79", ucValue, &iValueLen);

                if (iRet == SDK_OK && iValueLen != 0) {
                    memcpy(pOutData + usOutDataLen, "\x9F\x79\x06", 3);
                    usOutDataLen += 3;
                    memcpy(pOutData + usOutDataLen, ucValue, 6);
                    usOutDataLen += 6;
                }
            }
            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
        else {
            tagvalue = 0xFF;

            if (GetTagValueSize(pTagList, 0xDF75) == 1) {
                memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
            }
            memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
            usOutDataLen += 3;
            pOutData[usOutDataLen++] = tagvalue;

            sdkFreeMem(ucValue);
            return usOutDataLen;
        }
    }
    IsQmfMode = 0;
    usTagNum = sizeof(ucStdTagArray) / sizeof(ucStdTagArray[0]);

#if 0
    memset(&stApduinfo, 0x00, sizeof(SDK_ICC_APDU_SEND));
    memcpy(stApduinfo.Command, "\x80\xCA\x9F\x36\x00", 4);
    stApduinfo.Lc = 0;

    //������1��û�л�ȡatc, ���������ﵥ����apdu��ȡ mai 20140808
    //iRet = sdkIccSendIccCardData(&gstIccParam, "\x80\xCA\x9F\x36\x00", 5, ucValue, &iValueLen);
    iRet = sdkIccDealCardData(&gstIccParam, &stApduinfo, &stApduResp);

    if(iRet == SDK_OK)
    {
        TraceHex("Mai", "Value: \n", stApduResp.DataOut, stApduResp.LenOut);
        sdkIccConfigTLV("\x9F\x36", stApduResp.DataOut, 2);
    }
#endif

    if (ret == SDK_TransOnlineApprove || ret == SDK_TransOK_ARPCErr) {
        //20150413 ��ӽű��������
        //20150413 ������Ȩʱ���ص�DF78�����ݸ�ʽΪ: 1�ֽڵ�icdata���ܺ�ĳ���+icdata���ܺ������+1�ֽ�scriptdata���ܺ�ĳ���+scriptdata���ܺ������
        //sciptlen ���ڸĳ���Ϊ�Ƿ�Ϊ������Ȩʱ��DF78������־�������������Ľű�����
        //FormPbocOnlineData(scriptlen);
        FormPbocOnlineData(8);      //20150413 scriptlen > 0

#if 0
        iRet = GetTagValueSize(pTagList, 0xDF35);
        Trace("Mai", "DF35 len = %d\r\n", iRet);

        if(0 != iRet)
        {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x35;

            if(iRet > 0xFF)
            {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if(iRet > 0x7F)
            {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else
            {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF35), iRet);
            usOutDataLen += iRet;

            //                TraceHex("Mai", "DF35",  GetTagValue(pTagList, 0xDF35), iRet);
        }
#endif

        iRet = GetTagValueSize(pTagList, 0xDF78);

        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x78;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF78), iRet);
            usOutDataLen += iRet;
        }
        iRet = GetTagValueSize(pTagList, 0xDF79);

        if (0 != iRet) {
            pOutData[usOutDataLen++] = 0xDF;
            pOutData[usOutDataLen++] = 0x79;

            if (iRet > 0xFF) {
                pOutData[usOutDataLen++] = 0x82;
                sdkU16ToHex(pOutData + usOutDataLen, iRet, 2);
                usOutDataLen += 2;
            }
            else if (iRet > 0x7F) {
                pOutData[usOutDataLen++] = 0x81;
                pOutData[usOutDataLen++] = iRet;
            }
            else {
                pOutData[usOutDataLen++] = iRet;
            }
            memcpy(&pOutData[usOutDataLen], GetTagValue(pTagList, 0xDF79), iRet);
            usOutDataLen += iRet;
        }
        tagvalue = 0x01;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;

        i = 0;
        usTagNum = sizeof(ucStdTagArray) / sizeof(ucStdTagArray[0]);

        while (i < usTagNum) {
            iRet = sdkIccReadTLV(&ucStdTagArray[i][0], ucValue, &iValueLen);
            Trace("Mai", "TAG: %02x%02x��nValueLen = %d\n", ucStdTagArray[i][0],
                  ucStdTagArray[i][1], iValueLen);
            TraceHex("Mai", "Value: \n", ucValue, iValueLen);

            if (iRet == SDK_OK && iValueLen != 0) {
                //Tag
                if ((ucStdTagArray[i][0] & 0x1F) == 0x1F) {
                    pOutData[usOutDataLen++] = ucStdTagArray[i][0];
                    pOutData[usOutDataLen++] = ucStdTagArray[i][1];
                }
                else {
                    pOutData[usOutDataLen++] = ucStdTagArray[i][0];
                }

                if (iValueLen > 0xFF) {
                    pOutData[usOutDataLen++] = 0x82;
                    sdkU16ToHex(pOutData + usOutDataLen, iValueLen, 2);
                    usOutDataLen += 2;
                }
                else if (iValueLen > 0x7F) {
                    pOutData[usOutDataLen++] = 0x81;
                    pOutData[usOutDataLen++] = iValueLen;
                }
                else {
                    pOutData[usOutDataLen++] = iValueLen;
                }
                memcpy(&pOutData[usOutDataLen], ucValue, iValueLen);
                usOutDataLen += iValueLen;
            }
            else {
                //û�оͲ���
                //pOutData[usOutDataLen++] = ucStdTagArray[i][0];	//��ȱʡֵ
                //memset(&pOutData[usOutDataLen], 0x00, ucStdTagArray[i][0]);//��������value����
                //usOutDataLen += ucStdTagArray[i][0];
#if 0

                if(!memcmp(&ucStdTagArray[i][0], "\x9F\x41", 2))
                {
                    memcpy(pOutData + usOutDataLen, "\x9F\x41\x04\x00\x00\x01\x48", 7);
                    usOutDataLen += 7;
                }
                else if(!memcmp(&ucStdTagArray[i][0], "\x9F\x79", 2))
                {
                    memcpy(pOutData + usOutDataLen, "\x9F\x79\x06\x00\x00\x00\x00\x00\x00", 9);
                    usOutDataLen += 9;
                }
                else if(!memcmp(&ucStdTagArray[i][0], "\xDF\x76", 2))
                {
                    memcpy(pOutData + usOutDataLen, "\xDF\x76\x04\x00\x00\x00\x00", 7);
                    usOutDataLen += 7;
                }
                else if(!memcmp(&ucStdTagArray[i][0], "\xDF\x37", 2))
                {
                    memcpy(pOutData + usOutDataLen, "\xDF\x37\x01\x03", 4);
                    usOutDataLen += 4;
                }
#endif
            }
            i++;
        }

        if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD) {
            memcpy(pOutData + usOutDataLen, "\x9F\x79\x06", 3);
            usOutDataLen += 3;
            iValueLen = 0;
            iRet = sdkIccReadTLV("\x9F\x79", ucValue, &iValueLen);

            if (iRet == SDK_OK && iValueLen != 0) {
                memcpy(pOutData + usOutDataLen, ucValue, 6);
            }
            else {
                memcpy(pOutData + usOutDataLen, "\x00\x00\x00\x00\x00\x00", 6);
            }
            usOutDataLen += 6;
        }
        sdkFreeMem(ucValue);
        return usOutDataLen;
    }
    else {
        tagvalue = 0xFF;

        if (GetTagValueSize(pTagList, 0xDF75) == 1) {
            memcpy(&tagvalue, GetTagValue(pTagList, 0xDF75), 1);
        }
        memcpy(pOutData + usOutDataLen, "\xDF\x75\x01", 3);
        usOutDataLen += 3;
        pOutData[usOutDataLen++] = tagvalue;

        sdkFreeMem(ucValue);
        return usOutDataLen;
    }
}

#if 0
/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯QPBOC����TLV ��
   ��ڲ���:
   �� �� ֵ:	������֯�����ݵĳ���
   ��	 ע:
 ********************************************************************/
static u16 FormQpbocFlowTlv(u8 *pOutData, u8 *pTagList, u16 usTagListLen)
{
    u16 usOutDataLen = 0;
    u16 usPos = 0;
    s32 iRet;
    s32 iValueLen = 0;
    u8 ucValue[20] = {0};

    while(usTagListLen > usPos)
    {
        iRet = sdkIccReadTLV(&pTagList[usPos], ucValue, &iValueLen);
        Trace("Mai", "TAG: %02x%02x��nValueLen = %d\n", pTagList[usPos], pTagList[usPos + 1], iValueLen);
        TraceHex("Mai", "Value: \n", ucValue, iValueLen);

        if(iRet != SDK_OK)
        {
            iValueLen = 0;
        }

        //Tag
        if((pTagList[usPos] & 0x1F) == 0x1F)
        {
            pOutData[usOutDataLen++] = pTagList[usPos++];
            pOutData[usOutDataLen++] = pTagList[usPos++];
        }
        else
        {
            pOutData[usOutDataLen++] = pTagList[usPos++];
        }
        //�˴��и�bug ,����ֻ����һ���ֽڱ�ʾ
        pOutData[usOutDataLen++] = iValueLen;
        memcpy(&pOutData[usOutDataLen], ucValue, iValueLen);
        usOutDataLen += iValueLen;
    }

    return usOutDataLen;
}
#endif

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ù�Կ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetPubKey(ST_TRANSFLOW *pstTransFlow) {
    s32 rslt;
    u8 ucOperaCode;
    u8 *pTlvData;
    s32 iPkNum, i;
    u16 usSendPos = 0;
    u16 usTlvDataLen;
    SDK_ICC_CAPK_STRUCT tempCapk;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    GotoTradeFlow();
    memset(&tempCapk, 0x00, sizeof(tempCapk));
    ucOperaCode = pstTransFlow->heReceiveData[0];
    pTlvData = &pstTransFlow->heReceiveData[3];

    sdkIccLoadCAPKList();
    sdkBcdToU16(&usTlvDataLen, &pstTransFlow->heReceiveData[1], LL_LENGTH);

    if (usTlvDataLen > 1024) {
        return INS_PARAM_ERR;
    }
    ParseRecvCapkTlv(&tempCapk, pTlvData, usTlvDataLen);
    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 10);

    switch (ucOperaCode) {
        case CLEAN_ALL_PUBKEY:
            sdkIccDelAllCAPKLists();
            rslt = SDK_OK;
            break;

        case ADD_ONE_PUBKEY:
            ParseRecvCapkTlv(&tempCapk, pTlvData, usTlvDataLen);
            rslt = sdkIccAddAnyCAPKList(&tempCapk, 1);
            break;

        case DEL_ONE_PUBKEY:
            rslt = sdkIccDelOneCAPK(tempCapk.RID, tempCapk.CAPKI);
            break;

        case READ_PUBKEY_LIST:
            rslt = sdkIccGetCAPKListNum(&iPkNum);
            i = iPkNum;
            usSendPos = 2;

            while (i--) {
                rslt = sdkIccGetAnyCAPKList(i, 1, &tempCapk, &rslt);

                if (rslt != SDK_OK) {
                    return INS_OTHER_ERR;
                }
                memcpy(&pSend[usSendPos], "\x9F\x06\x05", 3);
                memcpy(&pSend[usSendPos + 3], tempCapk.RID, 5);
                memcpy(&pSend[usSendPos + 8], "\x9F\x22\x01", 3);
                pSend[usSendPos + 11] = tempCapk.CAPKI;
                usSendPos += 12;
            }

            sdkU16ToBcd(&pSend[0], usSendPos - LL_LENGTH, LL_LENGTH);
            pstTransFlow->usSendDataLen = usSendPos;
            break;

        case READ_SPECIFIC_PUBKEY:

            rslt = sdkIccGetMatchCAPK(tempCapk.RID, tempCapk.CAPKI, &tempCapk);

            if (rslt != SDK_OK) {
                return INS_OTHER_ERR;
            }
            usSendPos = FormCapkTlv(&tempCapk, &pSend[2]);
            sdkU16ToBcd(&pSend[0], usSendPos, LL_LENGTH);
            pstTransFlow->usSendDataLen = usSendPos + LL_LENGTH;
            break;

        default:
            FinishTradeFlow();
            return INS_PARAM_ERR;
    }

    if (rslt != SDK_OK) {
        Trace("Mai", "=================== ERR = %d ====================\n", rslt);
        FinishTradeFlow();
        return INS_OTHER_ERR;
    }
    else {
        FinishTradeFlow();
    }
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����AID
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetMposAid(ST_TRANSFLOW *pstTransFlow) {
    s32 rslt = SDK_OK;
    u8 ucOperaCode;
    u8 *pTlvData;
    s32 iAidNum, i;
    u16 usOffset = 0;
    u16 uspTlvDataLen;
    SDK_ICC_EmvSysIni_STRUCT stInAIDList;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    ucOperaCode = pstTransFlow->heReceiveData[0];
    pTlvData = &pstTransFlow->heReceiveData[3];

    GotoTradeFlow();
    sdkBcdToU16(&uspTlvDataLen, &pstTransFlow->heReceiveData[1], LL_LENGTH);

    if (uspTlvDataLen > 1024) {
        return INS_PARAM_ERR;
    }
    memset(&stInAIDList, 0,
           sizeof(stInAIDList));       //20150409 ��ʼ��AID�ṹ�壬��Ȼû���·���tag��ֵ����������¸���������һ��
    ParseRecvAidTlv(&stInAIDList, pTlvData, uspTlvDataLen);

    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], uspTlvDataLen);

    switch (ucOperaCode) {
        case CLEAN_ALL_AID:
            sdkIccDelAllAIDLists();
            break;

        case ADD_ONE_AID:
            rslt = sdkIccAddAnyAIDList(&stInAIDList, 1);
            break;

        case DEL_ONE_AID:
            rslt = sdkIccDelOneAID(stInAIDList.Aid, stInAIDList.AidLen);
            break;

        case READ_AID_LIST:
            sdkIccGetAIDListNum(&iAidNum);
            i = iAidNum;
            usOffset = LL_LENGTH;                //LL

            while (i--) {
                rslt = sdkIccGetAnyAIDList(i, 1, &stInAIDList, &rslt);

                if (rslt != 1) {
                    return INS_OTHER_ERR;
                }
                memcpy(&pSend[usOffset], "\x9F\x06", 2);
                pSend[usOffset + 2] = stInAIDList.AidLen;
                memcpy(&pSend[usOffset + 3], stInAIDList.Aid, stInAIDList.AidLen);
                usOffset += (stInAIDList.AidLen + 3);
            }

            sdkU16ToBcd(&pSend[0], usOffset - LL_LENGTH, LL_LENGTH);
            pstTransFlow->usSendDataLen = usOffset;
            break;

        case READ_SPECIFIC_AID:

            rslt = sdkIccGetMatchAID(stInAIDList.Aid, stInAIDList.AidLen, &stInAIDList);

            if (rslt != SDK_OK) {
                return INS_OTHER_ERR;
            }
            pstTransFlow->usSendDataLen = FormAidTlv(&stInAIDList, &pSend[2]);
            sdkU16ToBcd(&pSend[0], pstTransFlow->usSendDataLen, LL_LENGTH);
            pstTransFlow->usSendDataLen += LL_LENGTH;
            break;

        default:
            FinishTradeFlow();
            return INS_PARAM_ERR;
    }

    FinishTradeFlow();

    if (rslt != SDK_OK) {
        Trace("Mai", "=================== ERR = %d ====================\n", rslt);
        return INS_OTHER_ERR;
    }
    else {
        return INS_SUC;
    }
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �����ն�����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnSetTerAttr(ST_TRANSFLOW *pstTransFlow) {
    s32 rslt = SDK_OK;
    u16 usPos = 0;
    u16 usTag;
    u16 usTlvLen;
    u8 ucValueLen;
    u8 *pValue;
    u8 *pData = &pstTransFlow->heReceiveData[0];
    bool bFind = true;
    stMposTradeParam stTradeparam;

    GotoTradeFlow();
    sdkBcdToU16(&usTlvLen, pData, LL_LENGTH);

    if (usTlvLen > 1024) {
        return INS_PARAM_ERR;
    }
    usPos += LL_LENGTH;

    rslt = ReadMposParam(&stTradeparam);

    //Trace("Mai","Data len = %d\n", pstTransFlow->ReceiveDataLen);
    //TraceHex("Mai","Data hex\n", &pstTransFlow->heReceiveData[0], usTlvLen);
    while (usTlvLen > usPos) {
        sdkHexToU16(&usTag, &pData[usPos], 2);
        ucValueLen = pData[usPos + 2];                          //Ĭ��Tagռ2��byte,Lenռ1��byte
        pValue = &pData[usPos + 3];

        switch (usTag) {
            case 0xDF7A:
                memcpy(stTradeparam.ICS, pValue, ucValueLen);
                break;

            case 0x9F35:
                stTradeparam.termType = pValue[0];
                break;

            case 0x9F33:
                memcpy(stTradeparam.termCap, pValue, ucValueLen);
                break;

            case 0x9F40:
                memcpy(stTradeparam.termAddCap, pValue, ucValueLen);
                break;

            case 0x9F39:
                stTradeparam.posEntry = pValue[0];
                break;

            case 0x9F01:
                memcpy(stTradeparam.acquirerId, pValue, ucValueLen);
                break;

            case 0x9F15:
                memcpy(stTradeparam.merchantTypeCode, pValue, ucValueLen);
                break;

            case 0x9F16:
                memcpy(stTradeparam.merchantNo, pValue, ucValueLen);
                break;

            case 0x5F2A:
                memcpy(stTradeparam.currencyCode, pValue, ucValueLen);
                break;

            case 0x5F36:
                stTradeparam.currencyExp = pValue[0];
                break;

            case 0x9F3C:
                memcpy(stTradeparam.refCurrencyCode, pValue, ucValueLen);
                break;

            case 0x9F3D:
                stTradeparam.refCurrencyExp = pValue[0];
                break;

            case 0x9F1A:
                memcpy(stTradeparam.termCountryCode, pValue, ucValueLen);
                break;

            case 0x9F1E:
                memcpy(stTradeparam.ifdSN, pValue, ucValueLen);
                break;

            case 0x9F1C:
                memcpy(stTradeparam.termID, pValue, ucValueLen);
                break;

            case 0xDF44:
                memcpy(stTradeparam.ddol, pValue, ucValueLen);
                stTradeparam.ddolLen = ucValueLen;
                break;

            case 0xDF45:
                memcpy(stTradeparam.tdol, pValue, ucValueLen);
                stTradeparam.tdolLen = ucValueLen;
                break;

            case 0xDF01:
                stTradeparam.isSuppCompareAid = pValue[0];
                break;

            case 0xDF40:
                stTradeparam.isFallback = pValue[0];
                break;

            case 0x9F66:
                memcpy(stTradeparam.termTradeAttri, pValue, 4);
                break;

            default:
                usPos++;
                bFind = false;
                break;
        }

        //Trace("Mai","Tag = %04X\n", usTag);
        //TraceHex("Mai"," value", pValue, ucValueLen);
        if (bFind) {
            usPos += (ucValueLen + 3);                          //Ĭ��Tagռ2��byte,Lenռ1��byte
        }
        else {
            bFind = true;
        }
    }

    rslt = WriteMposParam(&stTradeparam);

    FinishTradeFlow();

    if (rslt != SDK_FILE_OK) {
        return INS_OTHER_ERR;
    }
    else {
        return INS_SUC;
    }
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ȡ��������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnGetTranData(ST_TRANSFLOW *pstTransFlow) {
    s32 rslt;
    s32 nValueLen;
    u16 usRecvPos = 0;
    u16 usSendPos = LL_LENGTH;
    u16 usTag;
    u16 usTlvLen;
    u8 ucTmpBuf[10];
    u8 *pRecvData = &pstTransFlow->heReceiveData[0];

    u8 *pSend = NULL;       //0416Send==Recv
    sdkBcdToU16(&usTlvLen, &pRecvData[0], LL_LENGTH);

    if (usTlvLen > 1024) {
        return INS_PARAM_ERR;
    }

    pSend = sdkGetMem(1024);    //0416Send==Recv
    if (!pSend) {
        return INS_OTHER_ERR;
    }
    memset(pSend, 0, 1024);
    usRecvPos += LL_LENGTH;

    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], usTlvLen);

    while (usTlvLen > usRecvPos) {
        if ((pRecvData[usRecvPos] & 0x1F) == 0x1F) {
            sdkHexToU16(&usTag, &pRecvData[usRecvPos], 2);
            pSend[usSendPos] = pRecvData[usRecvPos];
            pSend[usSendPos + 1] = pRecvData[usRecvPos + 1];
            usSendPos += 2;
            usRecvPos += 2;
        }
        else {
            usTag = pRecvData[usRecvPos];
            pSend[usSendPos] = pRecvData[usRecvPos];
            usSendPos++;
            usRecvPos++;
        }

        switch (usTag) {
            case 0x5A:
            case 0x9F1E:
            case 0x5F24:
            case 0xDF75:
                rslt = sdkIccReadTLV(&pRecvData[usRecvPos], ucTmpBuf, &nValueLen);

                if (rslt != SDK_OK) {
                    Trace("Mai", "= ERR = %d =\n", rslt);
                    sdkFreeMem(pSend);   //0416Send==Recv
                    return INS_OTHER_ERR;
                }
                pSend[usSendPos] = nValueLen;
                usSendPos++;
                memcpy(&pSend[usSendPos], ucTmpBuf, nValueLen);
                usSendPos += nValueLen;
                break;

            default:
                sdkFreeMem(pSend);   //0416Send==Recv
                return INS_PARAM_ERR;
        }
    }

    sdkU16ToBcd(&pSend[0], usSendPos - LL_LENGTH, LL_LENGTH);
    memcpy(&pstTransFlow->heReceiveData[0], pSend, usSendPos);   //0416Send==Recv
    pstTransFlow->usSendDataLen = usSendPos;

    sdkFreeMem(pSend);   //0416Send==Recv
    return INS_SUC;
}

bool MatchTagPDOL(const u8 *pheTag, const u8 *dol, s32 dollen) {
    s32 tag_len;
    u32 uiIndex;
    u8 k, bInTable;//i,

    uiIndex = 0;

    if (NULL == pheTag || dol == NULL || dollen <= 0) {
        return false;
    }
    tag_len = 1;

    if ((0x1F & pheTag[0]) == 0x1F) {
        tag_len = 2;
    }

    while (uiIndex < dollen)     //Process PDOL
    {
        if (dol[uiIndex] == 0xFF || dol[uiIndex] == 0x00) {
            uiIndex++;
            continue;
        }
        k = dol[uiIndex];

        bInTable = 0;

        if (k != pheTag[0]) {
            uiIndex++;
            if ((k & 0x1F) == 0x1F) {
                uiIndex++;
            }
            continue;
        }

        if ((k & 0x1F) == 0x1F) {
            if (dol[uiIndex + 1] != pheTag[1]) {
                uiIndex += 2;
                continue;
            }
        }

        return true;
    }

    return false;
}

void ForceOnline() {
    u8 temp[8] = {0};
    s32 iRet, len = 0;

    iRet = sdkIccReadTLV("\x95", temp, &len);
    if (iRet == SDK_OK && len == 5) {
        if ((temp[3] & 0xF7) == 0) {
            temp[3] |= 0x08;
            sdkIccConfigTLV("\x95", temp, len);
        }
    }
}

s32 DispCardNo(const u8 *pasPAN) {
    s32 key, iRet;
    SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;
    u8 temp[10] = {0}, tradeamt[8] = {0};
    s32 len;
    s32 timer = sdkTimerGetId();            //20150322 ��ӳ�ʱ����
    u8 ecerr = 0;
    //u8 ucDispAmount[32] = {0};

    Trace("zcl",
          "ȷ�Ͽ���: gucPBOCTransFlg:%d ERRCODE:%d gstEcBalance:%d ucCardMode:%02x eFlowMode:%d bIsSupportEc:%d\r\n",
          gucPBOCTransFlg, gucErrCode, gstEcBalance, pstIccParam->ucCardMode,
          pstIccTradeParam->eFlowMode, pstIccTradeParam->bIsSupportEc);

    if (SDK_ESC == gucPBOCTransFlg) {
        return SDK_ERR;
    }


    if ((pstIccParam->ucCardMode == SDK_ICC_RF) || gstEcBalance ||
        IsPureCard())      //20150327 ��Ӵ�������ȷ�Ͽ���
    {
        if (ERRCODE_ESC !=
            gucErrCode)     //�ǽӵ�ȡ�������ֽ�ʱ������Ҫȷ�Ͽ��ŵģ�ĿǰgucErrCode==ERRCODE_ESCʱ��ʾ�û�û��ѡ������ֽ�
            return SDK_OK;
    }

#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
    //syh 20150326 modify ��һ����ʾ���
   if (memcmp(gucAmount, "000000000000", 12))
   {
       strcpy(ucDispAmount, "���:");
       sdkAscToBcd(temp, gucAmount, 12);
       RegulateMoney(&ucDispAmount[5], temp);
       memset(temp, 0, sizeof(temp));
   }
#endif          //20150403 PBOC���Ƿ�����

    //zcl 20150325 ���ӵ����ֽ������ж�
    if (pstIccParam->ucCardMode == SDK_ICC_ICC && pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW &&
        pstIccTradeParam->bIsSupportEc == true)      //�Ӵ�ʽ�����ֽ���
    {
        //�жϿ�Ƭ֧�ֵ����ֽ���
        len = 0;
        memset(temp, 0, sizeof(temp));
        iRet = sdkIccReadTLV("\x9F\x38", temp, &len);       //����ƬPDOL
//        iRet = sdkIccReadTLV("\x9F\x7A", temp, &len);       //��9F7A
        if (iRet == SDK_OK && len > 0) {
            TraceHex("zcl", "PDOL", temp, len);
//            TraceHex("zcl","9F7A",temp,len);
            if (MatchTagPDOL("\x9F\x7A", temp, len))  //��Ƭ֧�ֵ����ֽ���
//            if(temp[0])  //�ն�ʹ���˵����ֽ�
            {
                //Trace("zcl","��Ƭ֧�ֵ����ֽ�\r\n");
                len = 0;
                memset(temp, 0, sizeof(temp));
                iRet = sdkIccReadTLV("\x9F\x74", temp, &len);
                if (iRet != SDK_OK || len != 6)      //û��ʹ�õ����ֽ�
                {
                    ForceOnline();      //û��ʹ�õ����ֽ��ǿ������
                    Trace("zcl", "��Ƭû��ʹ�õ����ֽ�\r\n");
                    if (ERRCODE_ESC == gucErrCode)     //�û�û��ѡ������ֽ�
                    {
                        Trace("zcl", "�û�û��ѡ������ֽ�\r\n");
                        goto VERIFYCARDNO;      //�����û�û��ѡ������ֽ���Ҫ��ʾת����
                    }

                    sdkIccReadTLV("\x9F\x02", tradeamt, &len);
                    TraceHex("zcl", "���׽��:", tradeamt, len);

                    //zcl add �����޶�ıȽ�
                    len = 0;
                    memset(temp, 0, sizeof(temp));
                    iRet = sdkIccReadTLV("\x9F\x7B", temp, &len);
                    if (iRet != SDK_OK || len != 6)      //û��ʹ�õ����ֽ�
                        goto VERIFYCARDNO;
                    else if (memcmp(tradeamt, temp, 6) >= 0)     //���׽����ڵ��ڵ����ֽ��޶�
                    {
                        goto VERIFYCARDNO;
                    }
                    //end
                    len = 0;
                    memset(temp, 0, sizeof(temp));
                    iRet = sdkIccSendGetData(pstIccParam, "\x9F\x79", temp, &len);        //����Ƭ���
                    TraceHex("zcl", "��Ƭ���:", temp, len);
                    if (iRet != SDK_OK || len != 6)      //�����ʧ��
                    {
                        Trace("zcl", "��Ƭ��ȡ�����ֽ����ʧ�ܣ���Ҫת����\r\n");
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
                        sdkDispClearScreen();
                        sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucDispAmount, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);   //syh 20150326 add ��ʾ���
#else
                        sdkDispClearRow(SDK_DISP_LINE2);
                        sdkDispClearRow(SDK_DISP_LINE3);
                        sdkDispClearRow(SDK_DISP_LINE4);
                        sdkDispClearRow(SDK_DISP_LINE5);
#endif
                        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ҫת��������",
                                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȷ��)",
                                          SDK_DISP_FDISP | SDK_DISP_RDISP |
                                          SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
                        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȡ��)",
                                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                        sdkDispBrushScreen();
                        ecerr = 1;
                    }
                    else            //�����ɹ�,��Ҫ�ͽ��׽�����Ա�
                    {
                        Trace("zcl", "��Ƭ��ȡ�����ֽ����ɹ�\r\n");
//                        sdkIccReadTLV("\x9F\x02", tradeamt, &len);
//                        TraceHex("zcl","���׽��:", tradeamt, len);
                        if (memcmp(tradeamt, temp, 6) > 0)     //���׽����ڵ����ֽ����
                        {
                            Trace("zcl", "���׽����ڵ����ֽ����\r\n");
                            if (IsPureCard() == false)               //���Ǵ������ֽ�
                            {
                                Trace("zcl", "���׽����ڵ����ֽ����Ҳ��Ǵ�������ת����\r\n");
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
                                sdkDispClearScreen();
                                sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucDispAmount, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);   //syh 20150326 add ��ʾ���
#else
                                sdkDispClearRow(SDK_DISP_LINE2);
                                sdkDispClearRow(SDK_DISP_LINE3);
                                sdkDispClearRow(SDK_DISP_LINE4);
                                sdkDispClearRow(SDK_DISP_LINE5);
#endif
                                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�����ֽ�����",
                                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                                sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��������������",
                                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                                sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȷ��)",
                                                  SDK_DISP_FDISP | SDK_DISP_RDISP |
                                                  SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
                                sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȡ��)",
                                                  SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                                sdkDispBrushScreen();
                                ecerr = 1;
                            }
                            else                        //�������ֽ𿨾ܾ�����
                            {
                                Trace("zcl", "���׽����ڵ����ֽ������Ǵ�����ʾ����\r\n");
#if 1
                                sdkDispClearScreen();
                                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����ʧ��",
                                                  SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                                DispAmt("���׽��:", SDK_DISP_LINE3,
                                        SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                                sdkDispFillRowRam(SDK_DISP_LINE4, 0, "�����ֽ�����",
                                                  SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                                gsiDCRow = SDK_DISP_LINE4;
                                gsiDCTimer = sdkTimerGetId();
                                gsiDCSecond = 10;
                                sdkDispBrushScreen();
                                //�ݳ����� ��Ҫ����ʱ
                                //ecerr = 1;                                       
#endif
                                gucErrCode = ERRCODE_ONLYCARDECNSF;       //������
                                return SDK_ERR;
                            }
                        }
                        else //����ԭ���µ�û���ߵ����ֽ���                       
                        {
                            Trace("zcl", "���׽��С�ڵ����ֽ�������ԭ����ת����\r\n");
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
                            sdkDispClearScreen();
                            sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucDispAmount, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);   //syh 20150326 add ��ʾ���
#else
                            sdkDispClearRow(SDK_DISP_LINE2);
                            sdkDispClearRow(SDK_DISP_LINE3);
                            sdkDispClearRow(SDK_DISP_LINE4);
                            sdkDispClearRow(SDK_DISP_LINE5);
#endif
                            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ҫת��������",
                                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȷ��)",
                                              SDK_DISP_FDISP | SDK_DISP_RDISP |
                                              SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
                            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȡ��)",
                                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                            sdkDispBrushScreen();
                            ecerr = 1;
                        }
                    }
                }
                else {
                    Trace("zcl", "��Ƭʹ�õ����ֽ�\r\n");
                    //20150403 IC��ʹ�õ����ֽ��ֱ�ӷ��ز��ü�����ʾȷ�Ͽ��ţ�
                    //����Ȧ�潻����Ҫȷ�Ͽ��ţ�pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD                    
                    return SDK_OK;
                }
            }
            else {
                Trace("zcl", "��Ƭ��֧�ֵ����ֽ�\r\n");
//                Trace("zcl","�ն�û��ʹ�õ����ֽ�\r\n");
                ForceOnline();      //û��ʹ�õ����ֽ��ǿ������
            }
        }
        else {
            Trace("zcl", "����ƬPDOLʧ��\r\n");
//            Trace("zcl","��9F7Aʧ��\r\n");
            ForceOnline();      //û��ʹ�õ����ֽ��ǿ������
        }
    }
    else if (pstIccParam->ucCardMode == SDK_ICC_ICC) {
        ForceOnline();      //û��ʹ�õ����ֽ��ǿ������
    }

    if (ecerr) {
        while (1) {
            if (sdkTimerIsEnd(timer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
            {
                gucPBOCTransFlg = SDK_TIME_OUT;
                return SDK_TIME_OUT;
            }
            iRet = AppCommuRead(temp, sizeof(temp), 10);

            if (iRet > 0) {
                Trace("Mai", "iRet = %d\n", iRet);
                TraceHex("Mai", "temp \r\n", temp, iRet);

                if (temp[3] == 0x1D && temp[4] == 0x08) {
                    PostTransFlow(temp, iRet);
                    return SDK_ERR;
                }
            }
            key = sdkKbGetKey();

            if (key != 0) {
                if (key == SDK_KEY_ENTER) {
                    //Xiaox 15-1-14 ���¶�ʱ����Ȼȷ�Ϻ�ָ�������oncirle��ȫ��5���ѹ����ͻ��mainfaceˢ����
                    bisNeedBrushScreenWhenBack = false;
                    giAutoRetTimer = sdkTimerGetId();
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    DispWait();
                    break;
                }
                else if (key == SDK_KEY_ESC) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    //return SDK_ERR;         //20150323 ȷ�Ͽ���ȡ��ʱ�����û�ȡ��
                    gucErrCode = ERRCODE_ESC;
                    return SDK_ESC;
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
            }
        }
    }

    VERIFYCARDNO:
    //end
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucDispAmount, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);   //syh 20150326 add ��ʾ���
#else
    sdkDispClearRow(SDK_DISP_LINE2);
    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispClearRow(SDK_DISP_LINE4);
    sdkDispClearRow(SDK_DISP_LINE5);
#endif
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����:", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, pasPAN, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    //sdkDispFillRowRam(SDK_DISP_LINE5, 0, "��ȷ��...", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȡ��", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ��", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();

    while (1) {
        if (sdkTimerIsEnd(timer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
        {
            gucPBOCTransFlg = SDK_TIME_OUT;
            return SDK_TIME_OUT;
        }
        iRet = AppCommuRead(temp, sizeof(temp), 10);

        if (iRet > 0) {
            Trace("Mai", "iRet = %d\n", iRet);
            TraceHex("Mai", "temp \r\n", temp, iRet);

            if (temp[3] == 0x1D && temp[4] == 0x08) {
                PostTransFlow(temp, iRet);
                return SDK_ERR;
            }
        }
        key = sdkKbGetKey();

        if (key != 0) {
            if (key == SDK_KEY_ENTER) {
                //Xiaox 15-1-14 ���¶�ʱ����Ȼȷ�Ϻ�ָ�������oncirle��ȫ��5���ѹ����ͻ��mainfaceˢ����
                bisNeedBrushScreenWhenBack = false;
                giAutoRetTimer = sdkTimerGetId();
                sdkSysBeep(SDK_SYS_BEEP_OK);
                DispWait();
                return SDK_OK;
            }
            else if (key == SDK_KEY_ESC) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                //return SDK_ERR;         //20150323 ȷ�Ͽ���ȡ��ʱ�����û�ȡ��
                gucErrCode = ERRCODE_ESC;
                return SDK_ESC;
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }

    Assert(0); // �����ܵ�������
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�Ƿ��Ǵ������ֽ�
** Parameters:          void
** Returned value:
** Created By:		lilin2012.12.09
** Remarks:
*****************************************************************************/
bool IsPureCard(void) {
    u8 temp[256] = {0};
    s32 len = 0;

    if (sdkIccReadTLV("\x4F\x00", temp, &len) > 0) {
        if (!memcmp(temp, "\xA0\x00\x00\x03\x33\x01\x01\x06", 8))  //����aid
        {
            return true;
        }
    }
    return false;
}

/*****************************************************************************
** Descriptions:	��ʾѡ������ֽ���
** Parameters:          void
** Returned value:
** Created By:		lilin2013.12.09
** Remarks:
*****************************************************************************/
s32 DispChooseEc(void) {
    s32 key = 0;
    u32 time_ID;
    u8 temp[10] = {0};                          //20150323 ���PBOC�����м�⸴λ����
    //u8 ucdispamount[32] = {0};


    if (gstIccTradeParam.bIsForceOnline) {
        return SDK_ESC;                                //sujianzhong 2015.01.13 15:36
    }

    //zxx 20131016 7:26 �к�̨�ʻ��߽����
    if (IsPureCard()) {
        return SDK_OK;
    }
    else {
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
        //syh 20150326 modify ��һ����ʾ���
        if (memcmp(gucAmount, "000000000000", 12))
        {
            strcpy(ucdispamount, "���:");
            sdkAscToBcd(temp, gucAmount, 12);
            RegulateMoney(&ucdispamount[5], temp);
            memset(temp, 0, sizeof(temp));
        }
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, ucdispamount, SDK_DISP_LDISP | SDK_DISP_FDISP);
#else
        sdkDispClearRow(SDK_DISP_LINE2);
        sdkDispClearRow(SDK_DISP_LINE3);
        sdkDispClearRow(SDK_DISP_LINE4);
        sdkDispClearRow(SDK_DISP_LINE5);
#endif
        //sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ÿ��߱������ֽ���", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        //sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�Ƿ�ʹ�õ����ֽ���:", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ƭ֧�ֵ����ֽ���",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�Ƿ����?",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȷ��)", SDK_DISP_FDISP | SDK_DISP_RDISP |
                                                        SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȡ��)",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispBrushScreen();
        time_ID = sdkTimerGetId();

        while (1) {
            if (sdkTimerIsEnd(time_ID, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
            {
                gucPBOCTransFlg = SDK_TIME_OUT;
                return SDK_TIME_OUT;
            }
            key = AppCommuRead(temp, sizeof(temp), 10);

            if (key > 0) {
                Trace("Mai", "iRet = %d\n", key);
                TraceHex("Mai", "temp \r\n", temp, key);

                if (temp[3] == 0x1D && temp[4] == 0x08) {
                    PostTransFlow(temp, key);
                    gucPBOCTransFlg = SDK_ESC;                  //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
                    return SDK_ERR;
                }
            }
            key = sdkKbGetKey();

            if (key != 0) {
                if (key == SDK_KEY_ENTER)       //syh 20150326 modify  ȷ�ϰ����޸�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    return SDK_OK;
                }
                else if (key == SDK_KEY_ESC)    //syh 20150326  modify  ȡ�������޸�
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    gucErrCode = ERRCODE_ESC;           //20150331 ��ס�û��Ƿ�ѡ������ֽ�
                    return SDK_ESC;
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
            }


        }
    }
}

/*****************************************************************************
** Descriptions:	��ѯ�����ֽ�����ϸ
** Parameters:          SDK_ICC_PARAM *IccParam ������
** Returned value:	void
** Created By:		lilin2013.12.06
** Remarks:
*****************************************************************************/
s32 QueryTradeLog(SDK_ICC_PARAM *IccParam, u8 *output, u16 *outlen) {
    u8 pLogFormat[100] = {0}, pLogNumOut;
    SDK_ICC_APDU_RESP pLogData[10];
    u8 rslt;
    u8 i = 0;
    u16 len = 0;

    memset(pLogData, 0, sizeof(pLogData));
    rslt = sdkIccReadLogDetail(IccParam, 10, pLogFormat, pLogData, &pLogNumOut);

    if (rslt == SDK_ReadLogEnd) {
        for (i = 0; i < pLogNumOut; i++) {
            memcpy(output + len, pLogData[i].DataOut, pLogData[i].LenOut);
            len += pLogData[i].LenOut;
            output[len++] = 0x3B;
        }
    }
    *outlen = len;
    return rslt;
//    if (rslt == SDK_ReadLogEnd)
//    {
//        if (pLogNumOut > 0)
//        {
//            DealEcLog(pLogData, pLogFormat, pLogNumOut);
//        }
//    }
//    else
//    {
//        sdkSysBeep(SDK_SYS_BEEP_OK);
//        sdkDispPromptBox(false, TV_AUTORET, "��ȡ������ϸʧ��\n��[ȡ��]�˳�");
//    }
}

/*
   //20141104100401|20141104100400|1|99|0|6214835510559261|868C61FE6AE214CD12F5AFA343B74A828EADB7819034697A99ECDB7B902C4D54E3CF7E9CAC1ADF65EBD90FB2E7B96ACE452DACB802D0CC2AD3C41D88A05705B00CE76DC7277CA2ABD5E9C0307B2BE1158CBC39BB58FB268B4AEEA9C67BA402C208B261B53142C9E1F4B952A200F73A0B8DAC6515676A81115BE93DD40EC37BB3DFBCFD81760C12E3|2000000000000002|622014110404633443|2305|
   �ѻ���ˮ�洢�ļ�����offlineFile
   �ѻ���ˮ�洢��ʽ��transactionId | transactionTime | transactionAmount | cardBalance | cardSeqNum | PAN | icData | pbocKsn | orderId | cardExpiredDate | identifier
   �����ֶεĺ��弰��ʽ��
   transactionId                         �ѻ�����ID  yyyyMMddHHmmss
   transactionTime	 ����ʱ�䣬   yyyyMMddHHmmss
   transactionAmount	 ���׽���λ��
   cardBalance   ���ʽ��׺�����ֽ����
   cardSeqNum    �����к�
   PAN   ���ţ����������Ρ�
   icData        55�����ݣ�pbocKsn���ܹ��ģ��ѻ����ױش�
   pbocKsn
   orderId                               ������
   cardExpiredDate       ����Ч�ڣ�YYMM
   identifier    ���ױ�ʶ��checkcard��ʱ����

   ��Ҫ��pboc��qpboc�����ӵ�tag���ֶΣ�orderId identifier
   ֱ���ڹ̼������ɣ�transactionId

   DF38 ������
   DF39 ���ױ�ʶ
 */

void SaveOffLineFile() {
    u8 temp[32], buf[64];
    s32 len; //, iRet;
    u8 *filebuf = NULL, *pboctag = NULL, *aspboctag = NULL;
    ST_TRANSFLOW *pstTransFlow = NULL;
    u16 pboclen = 0, filebuflen = 0;
    u8 i, usTagNum;
    //u8 fensan[16] = {0};//, key[16] = {0};

    const u8 Field55DOLTab[][2] =    //huxiang add 091127
            {
                    {0x9F, 0x26},    //Ӧ������M
                    {0x9F, 0x27},    //Ӧ��������ϢM
                    {0x9F, 0x10},    //������Ӧ������M
                    {0x9F, 0x37},    //����Ԥ֪����M
                    {0x9F, 0x36},    //Ӧ�ý��׼�����M
                    {0x95},                 //�ն���֤���M
                    {0x9a},                 //��������M
                    {0x9c},                 //��������M
                    {0x9f, 0x02},    //���׽��M
                    {0x5f, 0x2a},    //���׻��Ҵ���M
                    {0x82},                 //Ӧ�ý�������M
                    {0x9f, 0x1a},    //�ն˹��Ҵ���M
                    {0x9f, 0x33},    //�ն�����M
//        {0x9f, 0x34},    //�ֿ�����֤���O
                    {0x9f, 0x35},    //�ն�����O
                    {0x9f, 0x1e},    //�ӿ��豸���к�C
                    {0x84},                 //ר���ļ�����O
                    {0x9f, 0x09},    //Ӧ�ð汾��O
                    {0x9f, 0x41},    //�������м�����O
//        {0x57},
//        {0x5A},
//        {0x5F, 0x34},
//        {0x9F, 0x74},
//        {0x9F, 0x63},
                    {0x8A},
#if 0
                    /*=======BEGIN: sujianzhong 2015.01.13  22:54 modify===========*/
                    {0x50}, //appable
                    {0x9F, 0x12}, //appname
                    {0x9B}, //tsi
                    {0x4F}, //AID,��̨����84ȡaid
                    {0x9F, 0x06},
                    /*====================== END======================== */
                    {0x9F, 0x79},
                    {0x5F, 0x24},
#endif
//        {0x9F, 0x21},
                    {0x9f, 0x03},    //�������M
            };

    if (IsQmfMode == 0) {
        return;
    }

    //zcl add
    sdkIccGetMagCardData();
    sdkIccGetCardData(&pstCardInfo.stCardData);
    Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
    //end

    pstTransFlow = (u8 *) sdkGetMem(sizeof(*pstTransFlow));
    if (pstTransFlow == NULL) {
        return;
    }

    pboctag = (u8 *) sdkGetMem(256);
    if (pboctag == NULL) {
        sdkFreeMem(pstTransFlow);
        return;
    }

    aspboctag = (u8 *) sdkGetMem(512);
    if (aspboctag == NULL) {
        sdkFreeMem(pstTransFlow);
        sdkFreeMem(pboctag);
        return;
    }

    memset(pstTransFlow, 0, sizeof(*pstTransFlow));
    memset(pboctag, 0, 256);
    memset(aspboctag, 0, 512);

    pboclen = strlen(OFFLINEFILE);
    sdkU16ToBcd(pstTransFlow->heReceiveData, pboclen, 2);
    pstTransFlow->ReceiveDataLen = 2;
    memcpy(pstTransFlow->heReceiveData + pstTransFlow->ReceiveDataLen, OFFLINEFILE, pboclen);
    pstTransFlow->ReceiveDataLen += pboclen;

    filebuf = pstTransFlow->heReceiveData + pstTransFlow->ReceiveDataLen + 2;

    pboclen = 0;
    filebuflen = 0;

    //transactionId
    memcpy(filebuf, "20", 2);
    filebuflen = 2;
    len = 0;
    sdkIccReadTLV("\x9A", temp, &len);
    sdkBcdToAsc(filebuf + filebuflen, temp, len);
    filebuflen += len * 2;
    sdkIccReadTLV("\x9F\x21", temp, &len);
    sdkBcdToAsc(filebuf + filebuflen, temp, len);
    filebuflen += len * 2;
    filebuf[filebuflen++] = '|';

    //transactionTime
    memcpy(filebuf + filebuflen, filebuf, filebuflen);      //Ϊʲô?
    filebuflen *= 2;

    //transactionAmount
    memset(temp, 0, sizeof(temp));
    memset(buf, 0, sizeof(buf));
    sdkIccReadTLV("\x9F\x02", temp, &len);
    sdkBcdToAsc(buf, temp, 6);

    for (i = 0; i < 11; i++) {
        if (buf[i] != '0') {
            break;
        }
    }

    memcpy(filebuf + filebuflen, buf + i, 12 - i);
    filebuflen += (12 - i);
    filebuf[filebuflen++] = '|';


    //cardBalance
    memset(temp, 0, sizeof(temp));
    memset(buf, 0, sizeof(buf));
    sdkIccReadTLV("\x9F\x79", temp, &len);
    sdkBcdToAsc(buf, temp, 6);

    for (i = 0; i < 11; i++) {
        if (buf[i] != '0') {
            break;
        }
    }

    memcpy(filebuf + filebuflen, buf + i, 12 - i);
    filebuflen += (12 - i);
    filebuf[filebuflen++] = '|';

    //cardSeqNum
    memset(temp, 0, sizeof(temp));
    memset(buf, 0, sizeof(buf));
    sdkIccReadTLV("\x5F\x34", temp, &len);
    sdkBcdToAsc(buf, temp, 1);
    i = 0;

    if (buf[0] == '0') {
        i = 1;
    }
    memcpy(filebuf + filebuflen, buf + i, 2 - i);
    filebuflen += (2 - i);
    filebuf[filebuflen++] = '|';

    //PAN
    memcpy(filebuf + filebuflen, pstCardInfo.stCardData.asCardNO, pstCardInfo.stCardData.ucCardLen);
    filebuflen += pstCardInfo.stCardData.ucCardLen;
    filebuf[filebuflen++] = '|';

    //icData     55�����ݣ�pbocKsn���ܹ��ģ��ѻ����ױش�

    i = 0;
    usTagNum = sizeof(Field55DOLTab) / sizeof(Field55DOLTab[0]);
    pboclen = 0;

    FormTLVFromDOLTab(Field55DOLTab, usTagNum, pboctag, &pboclen);
#if 0
    while(i < usTagNum)
    {
        iRet = sdkIccReadTLV(&Field55DOLTab[i][0], temp, &len);

        if(iRet == SDK_OK && len != 0)
        {
            //Tag
            if((Field55DOLTab[i][0] & 0x1F) == 0x1F)
            {
                pboctag[pboclen++] = Field55DOLTab[i][0];
                pboctag[pboclen++] = Field55DOLTab[i][1];
            }
            else
            {
                pboctag[pboclen++] = Field55DOLTab[i][0];
            }

            if(len > 0xFF)
            {
                pboctag[pboclen++] = 0x82;
                sdkU16ToHex(pboctag + pboclen, len, 2);
                pboclen += 2;
            }
            else if(len > 0x7F)
            {
                pboctag[pboclen++] = 0x81;
                pboctag[pboclen++] = len;
            }
            else
            {
                pboctag[pboclen++] = len;
            }
            memcpy(&pboctag[pboclen], temp, len);
            pboclen += len;
        }
        i++;
    }
#endif

#if 0
    memcpy(pboctag + pboclen, "\xDF\x75\x01\x01", 4);
    pboclen += 4;
#endif

    //ʹ��KSN����
    //��Կ��ɢ

    memset(temp, 0, sizeof(temp));
    memcpy(temp, MposPbocKsn, sizeof(MposPbocKsn));

    for (i = 8; i < 16; i++) {
        temp[i] = ~temp[i - 8];
    }

#if 0
    sdkAscToBcd(key, MposAppKey, 32);
    sdkDes3S(1, fensan, key);
    sdkDes3S(1, fensan + 8, key);
#endif

    sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, temp, temp);
    sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, temp + 8, temp + 8);


    memcpy(gstPbocKey, temp, 16);
    TraceHex("zcl", "pboctag", pboctag, pboclen);
    len = TDES_UMS(aspboctag, pboctag, pboclen, temp, 2); //��Ҫ��λ
    TraceHex("zcl", "aspboctag", aspboctag, len);
    sdkBcdToAsc(filebuf + filebuflen, aspboctag, len);
    filebuflen += len * 2;
    filebuf[filebuflen++] = '|';

    sdkFreeMem(aspboctag);
    sdkFreeMem(pboctag);

    //pbocKsn
    sdkBcdToAsc(filebuf + filebuflen, MposPbocKsn, 8);
    filebuflen += 8 * 2;
    filebuf[filebuflen++] = '|';


    //orderId   DF38 ������
    if (GetTagValue(&TlvArray, 0xDF38)) {
        len = GetTagValueSize(&TlvArray, 0xDF38);
        memcpy(filebuf + filebuflen, GetTagValue(&TlvArray, 0xDF38), len);
        filebuflen += len;
    }
    filebuf[filebuflen++] = '|';


    //cardExpiredDate
    memcpy(filebuf + filebuflen, pstCardInfo.stCardData.asExpiredDate, 4);
    filebuflen += 4;
    filebuf[filebuflen++] = '|';

    //identifier         ���ױ�ʶ
    if (GetTagValue(&TlvArray, 0xDF39)) {
        len = GetTagValueSize(&TlvArray, 0xDF39);
        memcpy(filebuf + filebuflen, GetTagValue(&TlvArray, 0xDF39), len);
        filebuflen += len;
    }
    filebuf[filebuflen++] = '|';

    TraceHex("zcl", "offlineFile", filebuf, filebuflen);
    //sdkAppendFile("offlineFile", filebuf, filebuflen);


    sdkU16ToBcd(pstTransFlow->heReceiveData + pstTransFlow->ReceiveDataLen, filebuflen, 2);
    pstTransFlow->ReceiveDataLen += (2 + filebuflen);

    InitPbocOfflineFile();
    OnAddRecord(pstTransFlow);
    sdkFreeMem(pstTransFlow);
    AddMposKSN(ADDPBOCKSN);
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ִ��PBOC ��׼��������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnExecStdFlow(ST_TRANSFLOW *pstTransFlow) {
    u8 ucResult;
    //u8 ucCardInterfaceType;				//20150312 ʵ�ʶ�������
    u8 ucCustomTradeType = 0;            //20150312 ��������: ��׼����Ȩ���� ת�� PBOC��־ �����ֽ����� �����ֽ�ָ���˻�Ȧ�� ...
    //u8 ucPbocFlowIndicator;				//20150312 PBOC����ָʾ: 0x01 ��Ӧ������   0x06 ��һ����������
    //u16 usTlvLen;//, usOffset, usTag;
    u16 usSendPos = 0;
    //u16 usRecvPos = 0;
    s32 rslt;
    TagList *pTagList = &TlvArray;
    SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv


#if PBOCCLEARSCREEN_MODE            //20150408 PBOCִ�е�ʱ����ʾ���ڶ���
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ڶ���,���Ժ�", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);    
    sdkDispBrushScreen();
#else
    DispClearContentRam();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ڶ���,���Ժ�",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
#endif          //20150408 PBOCִ�е�ʱ����ʾ���ڶ���

    gucPBOCTransFlg = SDK_OK;       //20150323  ���PBOC����ȫ�ֱ�ʶ
    gucErrCode = ERRCODE_SUC;       //20150331  ��Ӵ�����
    memset(gucEcBalance, 0, sizeof(gucEcBalance));          //20150331 �����ֽ����

    memset(&gstIccParam, 0x00, sizeof(gstIccParam));
    memset(&gstIccTradeParam, 0x00, sizeof(gstIccTradeParam));

    sdkBcdToU16(&usSendPos, &pstTransFlow->heReceiveData[0], LL_LENGTH);

    if (usSendPos > 1024) {
        CloseAllReadCardDev();
        //DispTradeEnd(SDK_ERR);              //20150401
        return INS_PARAM_ERR;
    }
    InitTagList(pTagList);
    GotoTradeFlow();
    WriteTransSeqCounter();


    Trace("Mai", "\n\n OnExecStdFlow Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 10);

    // ��������TLV ��
    BuildTagListOneLevel(&pstTransFlow->heReceiveData[LL_LENGTH], usSendPos, pTagList);

    //zcl Ӧ�÷���userinit��
#if 0
    sdkIccConfigTLV("\x9F\x02", GetTagValue(pTagList, TAG_AMOUNT),  GetTagValueSize(pTagList, TAG_AMOUNT));
    sdkIccConfigTLV("\x9F\x03", GetTagValue(pTagList, TAG_AMOUNT_OTHER),  GetTagValueSize(pTagList, TAG_AMOUNT_OTHER));
    sdkIccConfigTLV("\x9C", GetTagValue(pTagList, TAG_TRANSACTION_TYPE),  GetTagValueSize(pTagList, TAG_TRANSACTION_TYPE));
#endif

    //zcl ��ΪSDK���ƣ����һ��Ҫ��pstIccTradeParam->pasTradeAmount
    memset(gucAmount, 0, sizeof(gucAmount));
    strcpy(gucAmount, "000000000000");

    if (GetTagValue(pTagList, TAG_AMOUNT)) {
        sdkBcdToAsc(gucAmount, GetTagValue(pTagList, TAG_AMOUNT), 6);
    }
    memcpy(&ucCustomTradeType, GetTagValue(pTagList, 0xDF7C), 1);
    ucResult = 0;
    memcpy(&ucResult, GetTagValue(pTagList, 0xDF72), 1);
    pstIccTradeParam->bIsForceOnline = (ucResult == 0x01 ? true : false);
    //ƽ̨��ˮ��
    memcpy(gPlatformSerialNO, GetTagValue(pTagList, 0xDF74), GetTagValueSize(pTagList, 0xDF74));

    //���ý��׽ṹ��
    pstIccTradeParam->pasTradeAmount = &gucAmount[0];
    pstIccTradeParam->bIsFallback = false;

    //�ж��Ƿ������ѽ��ף�ֻ������֧�ֵ����ֽ�
    ucResult = 0xFF;

    if (GetTagValueSize(pTagList, TAG_TRANSACTION_TYPE) == 1) {
        memcpy(&ucResult, GetTagValue(pTagList, TAG_TRANSACTION_TYPE), 1);
    }

    if (memcmp(gucAmount, "000000000000", 12) &&
        (ucCustomTradeType == 0x01 || ucCustomTradeType == 0x0B) && ucResult == 0x00) {
        pstIccTradeParam->bIsSupportEc = true;
        pstIccTradeParam->bIsPromptChooseEc = true;
        pstIccTradeParam->bIsDefaultEc = true;
    }
    else {
        pstIccTradeParam->bIsSupportEc = false;
        pstIccTradeParam->bIsPromptChooseEc = false;
        pstIccTradeParam->bIsDefaultEc = false;
    }
    pstIccTradeParam->bIsSupportQPBOC = false;
    pstIccTradeParam->bIsSupportCLPBOC = true;
    pstIccTradeParam->stFixedAid.bIsFixed = false;

    pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
    pstIccTradeParam->InputPWD = InputCreditPwd;
    pstIccTradeParam->UserIccTransInit = InitStdTransFlow;
    pstIccTradeParam->DispChooseEC = DispChooseEc;
    pstIccTradeParam->VerifyCardNo = DispCardNo;
    pstIccTradeParam->DispCandAppList = DispCandAppList;

    ucResult = 0;
    memcpy(&ucResult, GetTagValue(pTagList, 0xDF71), 1);
    Trace("Mai", "\n\n\n 111111111 ucCustomTradeType = %d \n\n\n", ucCustomTradeType);
    Trace("Mai", "\n\n\n 22222222 ucPbocFlowIndicator = %d\n", ucResult);
    gstEcBalance = 0;
    switch (ucCustomTradeType) {
        case 0x01:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            break;

        case 0x0B:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            pstIccTradeParam->bIsSupportEc = true;
            break;

        case 0x21:
        case 0x22:
        case 0x23:
            pstIccTradeParam->eFlowMode = SDK_ICC_EC_LOAD;
            pstIccTradeParam->bIsSupportEc = true;
            break;

        case 0x25:
            if (IsQmfMode == 0)      //��ҵ�治ȷ���Ƿ���Ҫ����,�Ȱ���֮ǰ�Ĵ���
            {
                pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
            }
            else {
                //pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;        //�人���ԣ�Ҫ������ֽ��������Ҫ���ؿ���
                pstIccTradeParam->eFlowMode = SDK_ICC_EASY_FLOW;
                gstEcBalance = 1;
            }
            break;

        case 0x0A:
        case 0x0E:
            pstIccTradeParam->eFlowMode = SDK_ICC_READLOG;
            break;

        default:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            break;
    }

    pstIccParam->ucCardMode = GetCurrentCardType();

    if ((SDK_ICC_ICC != pstIccParam->ucCardMode) && (SDK_ICC_RF != pstIccParam->ucCardMode)) {
        FinishTradeFlow();
        FreeTagList(pTagList);
        CloseAllReadCardDev();
        DispTradeEnd(SDK_ERR);              //20150401
        return INS_OTHER_ERR;
    }
    Trace("Mai", "\n\n\n 22222222 ucCardInterfaceType = %d\n", pstIccParam->ucCardMode);

    pstIccParam->ucSlotNo = 0;
    pstIccParam->ucSlotNo = 0;
    pstIccParam->eCardType = SDK_ICC_CPU;

    if (ucResult == 0x01) {
        pstIccTradeParam->eFlowMode = SDK_ICC_EASY_FLOW;
    }
    /*
        else if(ucPbocFlowIndicator == 0x06)
        {
                pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
        }
        else
        {
                FinishTradeFlow();
                FreeTagList(pTagList);
                return INS_PARAM_ERR;
        }
     */
    Trace("Mai", "\n\n\n >>>>>>>>>>> Amount = %s \n\n\n", pstIccTradeParam->pasTradeAmount);


    //sdkIccOpenIcDev();
    rslt = sdkIccTransFlow1(pstIccTradeParam, pstIccParam);
    Trace("Mai", "\n\n\n 333333333 rslt = %d \n\n\n", rslt);

#if 0           //20150409 ����ȡ 9F7B DF19 DF20 DF21   ���Թص�
    u8     TAG_clessofflineamt[4] = {0};
    u8      ttbuf[12] = {0};
    s32     ttlen = 0;

    memset(ttbuf, 0, sizeof(ttbuf));
    sdkIccReadTLV("\x9F\x7B", ttbuf, &ttlen);
    TraceHex("Mai", "9F7B:", ttbuf, 6);    
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x24\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812400:", ttbuf, 6);
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x23\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812300:", ttbuf, 6);
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x26\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812600:", ttbuf, 6);

#endif
    //20150323 ��ʱֱ�ӷ��س�ʱ ��ȡ��ĳ��ҵķ�����Ϣ����ʱֱ�ӷ�����Ӧ��37��ʱ
    if (SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg) {
        FinishTradeFlow();
        FreeTagList(pTagList);
        CloseAllReadCardDev();
        DispTradeEnd(SDK_ESC);              //20150401
        return INS_OVERTIME;
    }
    if (SDK_ESC == gucPBOCTransFlg)            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
    {
        FinishTradeFlow();
        FreeTagList(pTagList);
        CloseAllReadCardDev();
        return INS_NOT_RETURN;
    }

    switch (rslt) {
        case SDK_OK:
        case SDK_IccEasyFlow:
            ucResult = 0x00;
            break;

        case SDK_TransOfflineApprove: {
            s32 taglen = 0;
            sdkIccReadTLV("\x9F\x5D", gucEcBalance, &taglen);    //ȡ��Ƭ���: 9F79 ?
        }
            sdkIccConfigTLV("\x8A", "Y1", 2);
            WriteLastTcFile();
            SaveOffLineFile();
            ucResult = 0x01;
            break;

        case SDK_TransOfflineDeclineErr:
            ucResult = 0x02;
            break;

        case SDK_OnlineWait:
            if (pstIccParam->ucCardMode == SDK_ICC_ICC &&
                pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW &&
                pstIccTradeParam->bIsSupportEc == true)      //�Ӵ�ʽ�����ֽ���
            {
                u8 temp[32] = {0};
                s32 taglen = 0;
                s32 iRet, key;
                taglen = 0;
                TraceHex("Mai", "�����ȴ���ʼ�����ֽ����: ", gucEcBalance, 12);
                sdkIccReadTLV("\x9F\x5D", gucEcBalance, &taglen);    //ȡ��Ƭ���: 9F79 ?
                TraceHex("Mai", "�����ȴ�����ȡ���ֽ����: ", gucEcBalance, taglen);
                iRet = sdkIccReadTLV("\x9F\x74", temp, &taglen);
                Trace("Mai", "�����ȴ�����ȡ�õ����ֽ𷢿�����Ȩ����: iRet:%d len:%d\r\n", iRet, taglen);
                TraceHex("Mai", "�����ȴ�����ȡ�õ����ֽ𷢿�����Ȩ��: ", temp, taglen);
                if (iRet == SDK_OK && taglen == 6) {
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
                    sdkDispClearScreen();
                    DispAmt("���:", SDK_DISP_LINE1, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
#else
                    sdkDispClearRow(SDK_DISP_LINE2);
                    sdkDispClearRow(SDK_DISP_LINE3);
                    sdkDispClearRow(SDK_DISP_LINE4);
                    sdkDispClearRow(SDK_DISP_LINE5);
#endif
                    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��Ҫת��������",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȷ��)",
                                      SDK_DISP_FDISP | SDK_DISP_RDISP |
                                      SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
                    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��(ȡ��)",
                                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
                    sdkDispBrushScreen();

                    s32 ttimer = sdkTimerGetId();
                    while (1) {
                        if (sdkTimerIsEnd(ttimer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
                        {
                            gucPBOCTransFlg = SDK_TIME_OUT;
                            //return SDK_TIME_OUT;
                            rslt = SDK_ESC;
                            DispTradeEnd(SDK_ERR);               //20150401
                            ucResult = 0xFF;
                            break;
                        }
                        iRet = AppCommuRead(temp, sizeof(temp), 10);

                        if (iRet > 0) {
                            Trace("Mai", "iRet = %d\n", iRet);
                            TraceHex("Mai", "temp \r\n", temp, iRet);

                            if (temp[3] == 0x1D && temp[4] == 0x08) {
                                PostTransFlow(temp, iRet);
                                //return SDK_ERR;
                                rslt = SDK_ESC;
                                //DispTradeEnd(SDK_ERR);               //20150401
                                ucResult = 0xFF;
                                break;
                            }
                        }
                        key = sdkKbGetKey();

                        if (key != 0) {
                            if (key == SDK_KEY_ENTER) {
                                //Xiaox 15-1-14 ���¶�ʱ����Ȼȷ�Ϻ�ָ�������oncirle��ȫ��5���ѹ����ͻ��mainfaceˢ����
                                bisNeedBrushScreenWhenBack = false;
                                giAutoRetTimer = sdkTimerGetId();
                                sdkSysBeep(SDK_SYS_BEEP_OK);
                                DispWait();
                                ucResult = ERRCODE_ONLINE;          //20150409 ȱ�ٸ�ֵĬ��Ϊ0�ɹ������³��ָ������ʱ���ǳɹ�����

                                //20150331 ���ȷ�Ͽ�������
                                Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
                                gucErrCode = ERRCODE_ESC;       //��ʾû�ߵ����ֽ��߷ǽ�PBOC����
                                rslt = DispCardNo(pstCardInfo.stCardData.asCardNO);
                                if (SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg) {
                                    FinishTradeFlow();
                                    FreeTagList(pTagList);
                                    CloseAllReadCardDev();
                                    DispTradeEnd(SDK_ESC);               //20150401
                                    return INS_OVERTIME;
                                }
                                if (SDK_ESC ==
                                    gucPBOCTransFlg)            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
                                {
                                    FinishTradeFlow();
                                    FreeTagList(pTagList);
                                    CloseAllReadCardDev();
                                    return INS_NOT_RETURN;
                                }
                                if (SDK_OK != rslt)   //�û�ȡ��
                                {
                                    ucResult = 0xFC;            //ȡ��
                                    SetTagValue(0xDf75, &ucResult, 1, pTagList);
                                    FinishTradeFlow();
                                    FreeTagList(pTagList);
                                    CloseAllReadCardDev();
                                    DispTradeEnd(SDK_ESC);               //20150401
                                    //return INS_SUC;
                                }
                                rslt = SDK_OnlineWait;
                                gucErrCode = ERRCODE_SUC;

                                break;
                            }
                            else if (key == SDK_KEY_ESC) {
                                sdkSysBeep(SDK_SYS_BEEP_OK);
                                //return SDK_ERR;         //20150323 ȷ�Ͽ���ȡ��ʱ�����û�ȡ��
                                gucErrCode = ERRCODE_ESC;
                                //return SDK_ESC;
                                rslt = SDK_ESC;
                                DispTradeEnd(SDK_ESC);               //20150401
                                ucResult = 0xFF;
                                break;
                            }
                            else {
                                sdkSysBeep(SDK_SYS_BEEP_ERR);
                            }
                        }
                    }
                    break;
                }
            }
            ucResult = 0x03;
            break;

        case SDK_OfflineDecline:
            ucResult = 0x02;
            break;

        case SDK_ICC_RESET_ERR:        //20150324 ���fallback: ��IC�����ظ�λʧ��ʱ��Ҳ��fallback
        case SDK_NeedMagCardErr:
            DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFE;
            break;

        case SDK_ESC:                  //20150323 ����ȷ�ϵȰ�ȡ��ʱ����ȡ��: Ϊ�˷�ֹ�е�ʱ�򷵻�SDK_ESC ��ʱ����SDK_CancelTransErr
        case SDK_CancelTransErr:       //20150320 Ӧ��ѡ��ʱӦ�÷��ؽ���ȡ��
            ucResult = 0xFC;
            //DispTradeEnd(SDK_ESC);               //20150401   20150402 ȥ����ʾ����Ȼ��������ˢ��
            break;

        case SDK_EcLackBalanceErr:             //�����ֽ�����
            ucResult = ERRCODE_ONLYCARDECNSF;
            break;

        case SDK_ERR:
            DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFF;
            break;

        case SDK_NoAppSelErr:
            rslt = 0;
            sdkIccGetAIDListNum(&rslt);
            if (rslt > 0) {
                ucResult = 0xFA;        //20150402 ��֧�ֵ����ֽ�
            }
            else {
                ucResult = ERRCODE_LOSTAID;     //20150331 �ն�AID��ʧ
            }
            rslt = SDK_NoAppSelErr;
            break;

        default:
            //DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFF;
            break;
    }

    //if(memcmp(AppCreditPwd, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF", 8) != 0)
    //{
    //	sdkIccConfigTLV("\xDF\x36", AppCreditPwd, 8);
    //}
    if (gstEcBalance && pstIccTradeParam->eFlowMode == SDK_ICC_EASY_FLOW) {
        pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
        if (rslt == SDK_IccEasyFlow) {
            rslt = SDK_OK;
            DispTradeEnd(SDK_OK);               //20150401
        }
        gstEcBalance = 0;
    }
#if 0
    //if(gucErrCode == ERRCODE_ONLYCARDECNSF)         //20150331 
    if(SDK_EcLackBalanceErr == rslt && IsPureCard() )                  //20150401
    {
        Trace("Mai", "���������ֽ�����: rslt:%d\r\n", rslt);
        //ucResult = ERRCODE_ONLYCARDECNSF;
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "����ʧ��", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        DispAmt("���׽��:", SDK_DISP_LINE3, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "�����ֽ�����", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        gsiDCRow = SDK_DISP_LINE4;
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 10;
        sdkDispBrushScreen();        
    }
#endif
    if (SDK_TransOfflineApprove == rslt) {
        DispTradeEnd(SDK_OK);               //20150401
    }
    else if (SDK_DispLog == rslt) {
        DispTradeEnd(SDK_EQU);               //20150401
    }
    SetTagValue(0xDf75, &ucResult, 1, pTagList);


    /*=======BEGIN: sujianzhong 2015.01.13  11:13 modify===========*/
    sdkIccGetMagCardData();
    sdkIccGetCardData(&pstCardInfo.stCardData);
    Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
    //Trace("zcl","cardno:%s\r\n",pstCardInfo.stCardData.);
    /*====================== END======================== */


    //��֯��Ӧ����
    usSendPos = LL_LENGTH;
    //DF37,DF76,DF77,DF78,DF79
    //��6������Ԫδ����
    usSendPos += FormStdTransTlv(&pSend[2], rslt);

    sdkU16ToBcd(&pSend[0], usSendPos - LL_LENGTH, LL_LENGTH);
    pstTransFlow->usSendDataLen = usSendPos;

    FreeTagList(pTagList);

    if (rslt != SDK_OnlineWait && rslt != SDK_IccEasyFlow) {
        FinishTradeFlow();
    }

#if 0       //20150327 �˴�����
    if( SDK_KEY_ENTER != DispIccErr(rslt) )
    {
        ucResult = 0xFC;            //ȡ��
        SetTagValue(0xDf75, &ucResult, 1, pTagList);
        FinishTradeFlow();
        FreeTagList(pTagList);
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        return INS_SUC;
    }
#endif


    rslt = WriteLastTrans(&pSend[0], usSendPos);
    Trace("Mai", "\n\n\n 444444444 rslt = %d \n\n\n", rslt);
    //FreeTagList(pTagList);        �ͷ���ǰ��

    /*=======BEGIN: sujianzhong 2015.01.13  11:13 modify===========*/
    //sdkIccGetMagCardData();
    //sdkIccGetCardData(&pstCardInfo.stCardData);
    //Trace("zcl", "cardno2:%s\r\n", pstCardInfo.stCardData.asCardNO);
    /*====================== END======================== */


    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ������֤
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnPbocSecAuth(ST_TRANSFLOW *pstTransFlow) {
    SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;
    s32 rslt, ilen;//, i, iMax;
    u8 ucResult;
    //u16 usTagValueArray[] = {0x8A, 0x89, 0x71, 0x72, 0x91, 0xDF78};
    u16 usSendPos = 0;
    //u16 usTlvDataLen = 0;
    //u8 ucRet;
    u32 taglen, j;
    //u8 ucTag[2] = {0};
    u8 *pRecvData;
    TagList *pTagList = &TlvArray;
    u8 *ucValue = (u8 *) sdkGetMem(512);        //20150422 1024==>512
    u8 rescode[2];
    u8 scriptlen = 0;
    u8 IssuerAuthData[64];
    u16 IssuerAuthDataLen;
    u16 usTlvLen;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 10);

    if (ucValue == NULL) {

#if 1 /*Modify by ���ƽ at 2015.04.08  16:55 */
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
#endif /* if 0 */
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        return SDK_ERR;
    }
    memset(ucValue, 0, 512);        //20150422 1024==>512


#if 0
    iMax = sizeof(usTagValueArray) / sizeof(usTagValueArray[0]);

    for(i = 0; i < iMax; i++)
    {
        rslt = GetTagValueSize(pTagList, usTagValueArray[i]);

        if(rslt != 0)
        {
            if(usTagValueArray[i] > 0xFF)
            {
                taglen = 2;
            }
            else
            {
                taglen = 1;
            }
            sdkU16ToHex(ucTag, usTagValueArray[i], taglen);
            sdkIccConfigTLV(ucTag, GetTagValue(pTagList, usTagValueArray[i]), rslt);
        }
    }

#endif

    InitTagList(pTagList);
    GotoTradeFlow();

    // ��������TLV ��
    sdkBcdToU16(&usTlvLen, &pstTransFlow->heReceiveData[0], LL_LENGTH);
    pRecvData = &pstTransFlow->heReceiveData[LL_LENGTH];
    BuildTagListOneLevel(pRecvData, usTlvLen, pTagList);

    taglen = GetTagValueSize(pTagList, 0xDF78);

    if (taglen) {
        memcpy(ucValue, GetTagValue(pTagList, 0xDF78), taglen);

        for (j = 0; j < taglen; j += 8) {
            sdkDes3S(0, ucValue + j, gstPbocKey);
        }

        TraceHex("zcl", "DF78mingwen all", ucValue, taglen);
        taglen = pkcs5UnPadding(ucValue, taglen);
        TraceHex("zcl", "DF78mingwen nopadding", ucValue, taglen);
        BuildTagListOneLevel(ucValue, taglen, pTagList);
    }
    taglen = GetTagValueSize(pTagList, 0x8A);

    if (taglen) {
        memcpy(rescode, GetTagValue(pTagList, 0x8A), 2);
    }
    scriptlen = 0;
    taglen = GetTagValueSize(pTagList, 0x71);
    Trace("zcl", "�ű�71 len = %d\r\n", taglen);

    if (taglen) {
        ucValue[scriptlen++] = 0x71;

        if (taglen > 0x79) {
            ucValue[scriptlen++] = 0x81;
        }
        ucValue[scriptlen++] = taglen;
        memcpy(ucValue + scriptlen, GetTagValue(pTagList, 0x71), taglen);
        scriptlen += taglen;
    }
    taglen = GetTagValueSize(pTagList, 0x72);
    Trace("zcl", "�ű�72 len = %d\r\n", taglen);

    if (taglen) {
        ucValue[scriptlen++] = 0x72;

        if (taglen > 0x79) {
            ucValue[scriptlen++] = 0x81;
        }
        ucValue[scriptlen++] = taglen;
        memcpy(ucValue + scriptlen, GetTagValue(pTagList, 0x72), taglen);
        scriptlen += taglen;
    }
    IssuerAuthDataLen = GetTagValueSize(pTagList, 0x91);

    if (IssuerAuthDataLen) {
        memcpy(IssuerAuthData, GetTagValue(pTagList, 0x91), IssuerAuthDataLen);
    }
    TraceHex("zcl", "�ű�", ucValue, scriptlen);
    sdkIccGetOnlineData(SDK_OK, rescode, IssuerAuthData, IssuerAuthDataLen, ucValue, scriptlen);
    /*
       ucRet = ParseSecAuthTlvAndSet(pRecvData, usTlvDataLen);
       if(ucRet != SDK_OK)
       {
            FinishTradeFlow();
            return INS_PARAM_ERR;
       }
     */

    rslt = sdkIccTransFlow2(pstIccTradeParam, pstIccParam);
    Trace("zcl", "sdkIccTransFlow2 = %d\r\n", rslt);

    if (rslt == SDK_TransOnlineApprove || rslt == SDK_TransOK_ARPCErr) {
        DispTradeEnd(SDK_OK);               //20150401
        ucResult = 0x01;
    }
    else if (rslt == SDK_TransOnlineDeclineErr) {
        DispTradeEnd(SDK_ERR);               //20150401
        ucResult = 0x02;
    }
    else if (rslt == SDK_TransOfflineDeclineErr) {
        DispTradeEnd(SDK_ERR);               //20150401
        ucResult = 0x04;
    }
    else {
        DispTradeEnd(SDK_ERR);               //20150401
        ucResult = 0x02;
    }
    SetTagValue(0xDf75, &ucResult, 1, pTagList);

    if (rslt == SDK_TransOnlineApprove || rslt == SDK_TransOK_ARPCErr) {
        WriteLastTcFile();
    }
    //��֯��Ӧ����
    usSendPos = LL_LENGTH;
    usSendPos += FormSecAuthTlv(&pSend[2], rslt, scriptlen);
    //DF31,DF76,DF78,DF79
    //��4������Ԫû����

    if (scriptlen) {
        memset(ucValue, 0, 512);
        ilen = 0;
        sdkIccGetScriptResult(&ilen, ucValue);
        Trace("zcl", "�ű����len = %d\r\n", ilen);
        TraceHex("zcl", "�ű����", ucValue, ilen);

        if (ilen > 0) {
            memcpy(&pSend[usSendPos], "\xDF\x31", 2);
            usSendPos += 2;
            pSend[usSendPos++] = ilen;
            memcpy(&pSend[usSendPos], ucValue, ilen);
            usSendPos += ilen;
        }
    }
    sdkU16ToBcd(&pSend[0], usSendPos - LL_LENGTH, LL_LENGTH);
    pstTransFlow->usSendDataLen = usSendPos;

    FreeTagList(pTagList);
    FinishTradeFlow();

    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

    sdkIccTransFreeAllMem();
    sdkFreeMem(ucValue);

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���̽���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnPbocFlowEnd(ST_TRANSFLOW *pstTransFlow) {
    memset(&gstIccParam, 0x00, sizeof(gstIccParam));
    memset(&gstIccTradeParam, 0x00, sizeof(gstIccTradeParam));

    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 2);

    IsQmfMode = 0;

    GotoTradeFlow();
//    sdkDispClearScreen();  //syh 20150325 del  pboc���������Ȳ�����,�����о���淶�ٸ���

#if 1 /*Modify by ���ƽ at 2015.04.08  16:54 */
    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
#endif /* if 0 */
    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

    if (pstTransFlow->heReceiveData[0] == 0x00) {
//        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���׳ɹ�", SDK_DISP_DEFAULT);      //syh 20150325 del �����ε�������ʾ��Ϣ,�����о���淶�ٸ���
        DispTradeEnd(SDK_OK);              //20150401
    }
    else if (pstTransFlow->heReceiveData[0] == 0x01) {
//        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "����ʧ��", SDK_DISP_DEFAULT);      //syh 200150325 del �����ε�������ʾ��Ϣ,�����о���淶�ٸ���
        DispTradeEnd(SDK_ERR);              //20150401
    }
    else {
        DispTradeEnd(SDK_EQU);              //20150401
        FinishTradeFlow();
        return INS_PARAM_ERR;
    }
    //sdkDispBrushScreen();
    FinishTradeFlow();
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ���һ�ʽ�����Ϣ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnGetLastTran(ST_TRANSFLOW *pstTransFlow) {
    u8 *pRecvData;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    Trace("Mai", "Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 3);

    GotoTradeFlow();
    pRecvData = &pstTransFlow->heReceiveData[0];

    if (pRecvData[0] == 0x01) {
        pstTransFlow->usSendDataLen = ReadLastTrans(&pSend[0]);
    }
    else if (pRecvData[0] == 0x02) {
        pstTransFlow->usSendDataLen = ReadLastTcFile(&pSend[0]);
        //	DF31,DF75,DF76,DF78,DF79
        //	��������Դδ����
    }
    else {
        FinishTradeFlow();
        return INS_PARAM_ERR;
    }
    FinishTradeFlow();
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: QPBOC����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnQpbocFlow(ST_TRANSFLOW *pstTransFlow) {
    u8 *pRecvData;
    u8 ucResult;
    //u8 ucCardInterfaceType;
    u8 ucCustomTradeType;
    u16 usTlvLen;//, usOffset, usTag;
    u16 usSendPos = 0;
    //u16 usRecvPos = 0;
    s32 rslt;
    TagList *pTagList = &TlvArray;
    SDK_ICC_PARAM *pstIccParam = &gstIccParam;
    SDK_ICC_TRADE_PARAM *pstIccTradeParam = &gstIccTradeParam;
    u8 balance[8] = {0};

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

#if PBOCCLEARSCREEN_MODE            //20150408 PBOCִ�е�ʱ����ʾ���ڶ���
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ڶ���,���Ժ�", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);    
    sdkDispBrushScreen();
#else
    DispClearContentRam();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ڶ���,���Ժ�",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
#endif          //20150408 PBOCִ�е�ʱ����ʾ���ڶ���

    gucPBOCTransFlg = SDK_OK;       //20150323  ���PBOC����ȫ�ֱ�ʶ
    gucErrCode = ERRCODE_SUC;       //20150331  ��Ӵ�����

    memset(gucEcBalance, 0, sizeof(gucEcBalance));          //20150331 �����ֽ����

    InitTagList(pTagList);
    memset(&gstIccParam, 0x00, sizeof(gstIccParam));
    memset(&gstIccTradeParam, 0x00, sizeof(gstIccTradeParam));

    sdkBcdToU16(&usTlvLen, &pstTransFlow->heReceiveData[0], LL_LENGTH);

    if (usTlvLen > 1024) {
        FreeTagList(pTagList);
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        DispTradeEnd(SDK_ERR);               //20150401
        return INS_PARAM_ERR;
    }
    GotoTradeFlow();
    WriteTransSeqCounter();


    Trace("Mai", "\n\n OnExecStdFlow Data len = %d\n", pstTransFlow->ReceiveDataLen);
    TraceHex("Mai", "Data hex\n", &pstTransFlow->heReceiveData[0], 10);

    // ��������TLV ��
    pRecvData = &pstTransFlow->heReceiveData[LL_LENGTH];
    BuildTagListOneLevel(pRecvData, usTlvLen, pTagList);

    memset(gucAmount, 0, sizeof(gucAmount));
    strcpy(gucAmount, "000000000000");

    if (GetTagValue(pTagList, TAG_AMOUNT)) {
        sdkBcdToAsc(gucAmount, GetTagValue(pTagList, TAG_AMOUNT), 6);
    }
    memcpy(&ucCustomTradeType, GetTagValue(pTagList, 0xDF7C), 1);
    //ƽ̨��ˮ��
    memcpy(gPlatformSerialNO, GetTagValue(pTagList, 0xDF74), GetTagValueSize(pTagList, 0xDF74));

    //���ý��׽ṹ��
    pstIccTradeParam->pasTradeAmount = &gucAmount[0];
    pstIccTradeParam->bIsFallback = false;
    pstIccTradeParam->bIsSupportEc = false;
    pstIccTradeParam->bIsSupportQPBOC = true;
    pstIccTradeParam->bIsSupportCLPBOC = false;
    pstIccTradeParam->stFixedAid.bIsFixed = false;

    pstIccTradeParam->InputPWD = InputCreditPwd;
    pstIccTradeParam->UserIccTransInit = InitStdTransFlow;
    pstIccTradeParam->DispChooseEC = DispChooseEc;
    pstIccTradeParam->VerifyCardNo = DispCardNo;


    Trace("Mai", "\n\n\n 111111111 ucCustomTradeType = %d \n\n\n", ucCustomTradeType);
    gstEcBalance = 0;
    switch (ucCustomTradeType) {
        case 0x01:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            break;

        case 0x0B:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            pstIccTradeParam->bIsSupportEc = true;
            break;

        case 0x21:
        case 0x22:
        case 0x23:
            pstIccTradeParam->eFlowMode = SDK_ICC_EC_LOAD;
            break;

        case 0x25:
            if (IsQmfMode == 0)      //��ҵ�治ȷ���Ƿ���Ҫ����,�Ȱ���֮ǰ�Ĵ���
            {
                pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
            }
            else {
                //pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;        //�人���ԣ�Ҫ������ֽ��������Ҫ���ؿ���
                pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
                memset(gucAmount, 0, sizeof(gucAmount));
                strcpy(gucAmount, "000000000000");
                gstEcBalance = 1;
            }
            break;

        case 0x0E:
        case 0x0A:
            pstIccTradeParam->eFlowMode = SDK_ICC_READLOG;
            break;

        default:
            pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
            break;
    }

    pstIccParam->ucSlotNo = 0;
    //pstIccParam->ucCLType = 0x02;	 //Qpboc
    pstIccParam->eCardType = SDK_ICC_CPU;
    pstIccParam->ucCardMode = SDK_ICC_RF;

    Trace("Mai", "\n\n\n >>>>>>>>>>> Amount = %s \n\n\n", pstIccTradeParam->pasTradeAmount);

    rslt = sdkIccTransFlow1(pstIccTradeParam, pstIccParam);
    Trace("Mai", "\n\n\n 333333333 rslt = %d \n\n\n", rslt);

#if 0           //20150409 ����ȡ 9F7B DF19 DF20 DF21   ���Թص�
    u8     TAG_clessofflineamt[4] = {0};
    u8      ttbuf[12] = {0};
    s32     ttlen = 0;

    memset(ttbuf, 0, sizeof(ttbuf));
    sdkIccReadTLV("\x9F\x7B", ttbuf, &ttlen);
    TraceHex("Mai", "9F7B:", ttbuf, 6);    
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x24\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812400:", ttbuf, 6);
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x23\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812300:", ttbuf, 6);
    
    memcpy(TAG_clessofflineamt, "\xDF\x81\x26\x00", 4);
    memset(ttbuf, 0, sizeof(ttbuf));
    dllemvb_avl_gettagvalue_spec(TAG_clessofflineamt,ttbuf,0,6);
    TraceHex("Mai", "DF812600:", ttbuf, 6);

#endif
    //20150323 ��ʱֱ�ӷ��س�ʱ ��ȡ��ĳ��ҵķ�����Ϣ����ʱֱ�ӷ�����Ӧ��37��ʱ
    if (SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg) {
        FinishTradeFlow();
        FreeTagList(pTagList);
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        DispTradeEnd(SDK_ESC);               //20150401
        return INS_OVERTIME;
    }
    if (SDK_ESC == gucPBOCTransFlg)            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
    {
        FinishTradeFlow();
        FreeTagList(pTagList);
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        return INS_NOT_RETURN;
    }

    switch (rslt) {
        case SDK_OK:
        case SDK_IccEasyFlow:
            ucResult = 0x00;
            break;

        case SDK_TransOfflineApprove:
            /*=======BEGIN: sujianzhong 2015.01.13  11:13 modify===========*/
            sdkIccGetMagCardData();
            sdkIccGetCardData(&pstCardInfo.stCardData);
            Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);

            //Trace("zcl","cardno:%s\r\n",pstCardInfo.stCardData.);
            /*====================== END======================== */
            if (sdkIccReadTLV("\x9F\x5D", balance, &rslt) > 0) {
                TraceHex("zcl", "balance", balance, rslt);
                sdkIccConfigTLV("\x9F\x79", balance, rslt);
                memcpy(gucEcBalance, balance, rslt);
                //rslt = SDK_TransOfflineApprove;
            }
            rslt = SDK_TransOfflineApprove;
            sdkIccConfigTLV("\x8A", "Y1", 2);
            WriteLastTcFile();
            SaveOffLineFile();
            ucResult = 0x01;
            break;

        case SDK_TransOfflineDeclineErr:
            /*=======BEGIN: sujianzhong 2015.01.13  11:13 modify===========*/
            sdkIccGetMagCardData();
            sdkIccGetCardData(&pstCardInfo.stCardData);
#if 0        //20150324 ��ӷǽ���������ʾ����ȷ�Ͽ���

#endif
            Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
            //Trace("zcl","cardno:%s\r\n",pstCardInfo.stCardData.);
            /*====================== END======================== */
            ucResult = 0x02;
            break;

        case SDK_OnlineWait:
            /*=======BEGIN: sujianzhong 2015.01.13  11:13 modify===========*/
            sdkIccGetMagCardData();
            sdkIccGetCardData(&pstCardInfo.stCardData);
            Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
            //Trace("zcl","cardno:%s\r\n",pstCardInfo.stCardData.);
            /*====================== END======================== */
            ucResult = 0x0F;
            break;

        case SDK_OfflineDecline:
            //ucResult = 0x03;           //20150401 �ѻ��ܾ�Ӧ�÷���02 ���׾ܾ�
            ucResult = 0x02;
            break;

            //case SDK_ICC_RESET_ERR:        //20150324 ���fallback: QPBOC��ʱ�򷵻�-106SDK_ICC_RESET_ERR��ʱ�򷵻ؿ�Ƭ�����쳣DF75Ϊ255
        case SDK_NeedMagCardErr:
            DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFE;
            break;

        case SDK_ESC:                  //20150323 ����ȷ�ϵȰ�ȡ��ʱ����ȡ��: Ϊ�˷�ֹ�е�ʱ�򷵻�SDK_ESC ��ʱ����SDK_CancelTransErr
        case SDK_CancelTransErr:       //20150320 Ӧ��ѡ��ʱӦ�÷��ؽ���ȡ��
            ucResult = 0xFC;
            DispTradeEnd(SDK_ESC);               //20150401
            break;

        case SDK_ERR:
            DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFF;
            break;

        case SDK_EcLackBalanceErr:      //�����ֽ�����
            ucResult = 0xF9;
            break;

        case SDK_NoAppSelErr:
            rslt = 0;
            sdkIccGetAIDListNum(&rslt);
            if (rslt > 0) {
                ucResult = 0xFA;        //20150402 ��֧�ֵ����ֽ�
            }
            else {
                ucResult = ERRCODE_LOSTAID;     //20150331 �ն�AID��ʧ
            }
            rslt = SDK_NoAppSelErr;
            break;

        case SDK_UsePbocErr:                   //20150402 �����ն��޶�
            ucResult = ERRCODE_OVERLIMTAMT;
            break;

        default:
            //DispTradeEnd(SDK_ERR);               //20150401
            ucResult = 0xFF;
            break;
    }

    if (gstEcBalance && pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW) {
        pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
        gstEcBalance = 0;
        if (rslt == SDK_OnlineWait || rslt == SDK_TransOfflineDeclineErr) {
            rslt = SDK_OK;
            ucResult = 0x00;
            DispTradeEnd(SDK_OK);               //20150401
        }
    }

    SetTagValue(0xDf75, &ucResult, 1, pTagList);

#if 0
    if( SDK_KEY_ENTER != DispIccErr(rslt) )
    {
        ucResult = 0xFC;            //ȡ��
        SetTagValue(0xDf75, &ucResult, 1, pTagList);
        FinishTradeFlow();
        FreeTagList(pTagList);
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
        return INS_SUC;
    }
#endif

    if (SDK_EcLackBalanceErr == rslt) {
        if (IsPureCard()) {
            //�ݳ����� ��ʾ�������ֽ�
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����ʧ��",
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            DispAmt("���׽��:", SDK_DISP_LINE2, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�����ֽ�����",
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            gsiDCRow = SDK_DISP_LINE4;
            gsiDCTimer = sdkTimerGetId();
            gsiDCSecond = 10;
            sdkDispBrushScreen();
        }
#if 0
        else  //20150401 ��ӷǽӵ����ֽ�������ʾ      
        {
            Trace("zcl","���׽����ڵ����ֽ����Ҳ��Ǵ�������ת����\r\n");
            sdkDispClearScreen();
            DispAmt("���:", SDK_DISP_LINE1, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);           
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�����ֽ�����", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��������������", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȷ��)", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȡ��)", SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();

            s32 ttimer = sdkTimerGetId();
            s32 key = SDK_KEY_ERR;
            while(1)
            {
                if (sdkTimerIsEnd(ttimer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
                {
                    gucPBOCTransFlg = SDK_TIME_OUT;
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    DispTradeEnd(SDK_ESC);               //20150401
                    return INS_OVERTIME;
                }
                if( SDK_ESC == gucPBOCTransFlg )            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
                {
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    return INS_NOT_RETURN;
                }
                key = sdkKbGetKey();
                if( SDK_KEY_ESC == key )   //�û�ȡ��
                {
                    ucResult = 0xFC;            //ȡ��
                    SetTagValue(0xDf75, &ucResult, 1, pTagList);
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    DispTradeEnd(SDK_ESC);               //20150401
                    break;
                }
                else if( SDK_KEY_ENTER == key )
                {
                    break;
                }
            }
            if( SDK_KEY_ENTER == key )
            {
                gucErrCode = ERRCODE_ESC;       //��ʾû�ߵ����ֽ��߷ǽ�PBOC����
                key = DispCardNo(pstCardInfo.stCardData.asCardNO);
                if( SDK_TIME_OUT == key || SDK_TIME_OUT == gucPBOCTransFlg )
                {
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    DispTradeEnd(SDK_ESC);               //20150401
                    return INS_OVERTIME;
                }
                if( SDK_ESC == gucPBOCTransFlg )            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
                {
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    return INS_NOT_RETURN;
                }
                if( SDK_OK != key )   //�û�ȡ��
                {
                    ucResult = 0xFC;            //ȡ��
                    SetTagValue(0xDf75, &ucResult, 1, pTagList);
                    FinishTradeFlow();
                    FreeTagList(pTagList);
                    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                    DispTradeEnd(SDK_ESC);               //20150401
                    //return INS_SUC;
                }
                gucErrCode = ERRCODE_SUC; 
            }
        }
#endif
    }
    else if (SDK_OnlineWait == rslt) {
        //�ݳ����� ��ʾת����        
        Trace("zcl", "���׽����ڵ����ֽ����Ҳ��Ǵ�������ת����\r\n");
        //20150409 ת�����п����ǳ����ն��޶�,���������Ӧ�������ǳ����ն��޶�ǵ����ֽ�����
#if 1       //20150409 �жϳ����ն��޶� �����ת
        rslt = 0;
        memset(balance, 0, sizeof(balance));
        sdkIccReadTLV("\x9F\x5D", balance, &rslt);
        if (rslt != 6) {
            rslt = 0;
            memset(balance, 0, sizeof(balance));
            sdkIccReadTLV("\x9F\x79", balance, &rslt);
        }
        if (rslt != 6)             //û�ж������
        {
            Trace("zcl", "�ǽ�ת����: û�ж��������ֽ����: rslt:%d\r\n", rslt);
            rslt = SDK_OnlineWait;
            goto ONLINEDISPCARDNO;
        }
        rslt = SDK_OnlineWait;
        if (!GetTagValue(pTagList, TAG_AMOUNT))    //û�н��׽��
        {
            Trace("zcl", "�ǽ�ת����: û�ж������׽��\r\n");
            goto ONLINEDISPCARDNO;
        }
        if (memcmp(GetTagValue(pTagList, TAG_AMOUNT), balance, 6) <= 0) {
            //���׽��С�ڵ������
            Trace("zcl", "�ǽ�ת����: ���׽��С�ڵ����ֽ����\r\n");
            Trace("zcl", "�ǽ�ת����: ���׽��", GetTagValue(pTagList, TAG_AMOUNT), 6);
            Trace("zcl", "�ǽ�ת����: �����ֽ����", balance, 6);
            goto ONLINEDISPCARDNO;
        }
        Trace("zcl", "�ǽ�ת����: ���׽����ڵ����ֽ����\r\n");
#endif      //
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
        sdkDispClearScreen();
        DispAmt("���:", SDK_DISP_LINE1, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
#else
        DispClearContentRam();
#endif          //20150403 PBOC���Ƿ�����
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�����ֽ�����",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��������������",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȷ��)", SDK_DISP_FDISP | SDK_DISP_RDISP |
                                                        SDK_DISP_INCOL);      //syh 20150326 modify  �޸�Ϊ��ȷ��ȡ����
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��(ȡ��)",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkDispBrushScreen();

        s32 ttimer = sdkTimerGetId();
        s32 key = SDK_KEY_ERR;
        while (1) {
            if (sdkTimerIsEnd(ttimer, SDK_ICC_TIMER_AUTO))        //20150322 ��ӳ�ʱ����
            {
                gucPBOCTransFlg = SDK_TIME_OUT;
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                DispTradeEnd(SDK_ESC);               //20150401
                return INS_OVERTIME;
            }
            if (SDK_ESC == gucPBOCTransFlg)            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
            {
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                return INS_NOT_RETURN;
            }
            key = sdkKbGetKey();
            if (SDK_KEY_ESC == key)   //�û�ȡ��
            {
                break;
            }
            else if (SDK_KEY_ENTER == key) {
                break;
            }
        }
        if (SDK_KEY_ENTER != key) {
            ucResult = 0xFC;            //ȡ��
            SetTagValue(0xDf75, &ucResult, 1, pTagList);
            FinishTradeFlow();
            FreeTagList(pTagList);
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            DispTradeEnd(SDK_ESC);               //20150401
            goto qPBOCFORMTLV;              //20150409 �жϳ����ն��޶� �����ת
        }
#if     0                           //20150409 �жϳ����ն��޶� �����ת
        else
        {
            //20150331 ���ȷ�Ͽ�������
            Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
            gucErrCode = ERRCODE_ESC;       //��ʾû�ߵ����ֽ��߷ǽ�PBOC����
            rslt = DispCardNo(pstCardInfo.stCardData.asCardNO);
            if( SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg )
            {
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                DispTradeEnd(SDK_ESC);               //20150401
                return INS_OVERTIME;
            }
            if( SDK_ESC == gucPBOCTransFlg )            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
            {
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                return INS_NOT_RETURN;
            }
            if( SDK_OK != rslt )   //�û�ȡ��
            {
                ucResult = 0xFC;            //ȡ��
                SetTagValue(0xDf75, &ucResult, 1, pTagList);
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                DispTradeEnd(SDK_ESC);               //20150401
                //return INS_SUC;
            }
            rslt = SDK_OnlineWait;
            gucErrCode = ERRCODE_SUC;
        }
#endif          //20150409 �жϳ����ն��޶� �����ת
        ONLINEDISPCARDNO:           //20150409 �жϳ����ն��޶� �����ת
        //20150331 ���ȷ�Ͽ�������
        Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
        gucErrCode = ERRCODE_ESC;       //��ʾû�ߵ����ֽ��߷ǽ�PBOC����
        rslt = DispCardNo(pstCardInfo.stCardData.asCardNO);
        if (SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg) {
            FinishTradeFlow();
            FreeTagList(pTagList);
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            DispTradeEnd(SDK_ESC);               //20150401
            return INS_OVERTIME;
        }
        if (SDK_ESC == gucPBOCTransFlg)            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
        {
            FinishTradeFlow();
            FreeTagList(pTagList);
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            return INS_NOT_RETURN;
        }
        if (SDK_OK != rslt)   //�û�ȡ��
        {
            ucResult = 0xFC;            //ȡ��
            SetTagValue(0xDf75, &ucResult, 1, pTagList);
            FinishTradeFlow();
            FreeTagList(pTagList);
            sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
            sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
            //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
            DispTradeEnd(SDK_ESC);               //20150401
            //return INS_SUC;
        }
        rslt = SDK_OnlineWait;
        gucErrCode = ERRCODE_SUC;

    }
    else if (SDK_NoAppSelErr == rslt) {
        //�ݳ����� ��ʾ��֧�ֵ����ֽ�
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "����ʧ��",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        DispAmt("���׽��:", SDK_DISP_LINE2, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Ӧ�ò�֧��",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        gsiDCRow = SDK_DISP_LINE4;
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 10;
        sdkDispBrushScreen();
    }
    else if (SDK_TransOfflineApprove == rslt) {
        DispTradeEnd(SDK_OK);               //20150401
    }
    else if (SDK_DispLog == rslt) {
        DispTradeEnd(SDK_EQU);               //20150401
    }
#if     0     //20150409 ��Ӧ�ÿ��ǽӽ��׽����ն��޶�ת��������
    else if( SDK_UsePbocErr == rslt )        //20150408 ��ӳ����ն��޶�ʱ���Ϊ��Ӧ�ÿ�ʱ����������
    {
        if(false == IsPureCard())           //20150408 ��Ӧ�ÿ�ʱ�������ն��޶�����������
        {
            //20150331 ���ȷ�Ͽ�������
            sdkIccGetMagCardData();
            sdkIccGetCardData(&pstCardInfo.stCardData);
            Trace("zcl", "cardno:%s\r\n", pstCardInfo.stCardData.asCardNO);
            gucErrCode = ERRCODE_ESC;       //��ʾû�ߵ����ֽ��߷ǽ�PBOC����
            rslt = DispCardNo(pstCardInfo.stCardData.asCardNO);
            if( SDK_TIME_OUT == rslt || SDK_TIME_OUT == gucPBOCTransFlg )
            {
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                DispTradeEnd(SDK_ESC);               //20150401
                return INS_OVERTIME;
            }
            if( SDK_ESC == gucPBOCTransFlg )            //20150323 ��ӵ����ֽ�ȷ���ǽ��ո�λ�����
            {
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                return INS_NOT_RETURN;
            }
            if( SDK_OK != rslt )   //�û�ȡ��
            {
                ucResult = 0xFC;            //ȡ��
                SetTagValue(0xDf75, &ucResult, 1, pTagList);
                FinishTradeFlow();
                FreeTagList(pTagList);
                sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
                sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
                //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ
                DispTradeEnd(SDK_ESC);               //20150401
                //return INS_SUC;
            }
            else
            {
                ucResult = ERRCODE_ONLINE; 
                SetTagValue(0xDf75, &ucResult, 1, pTagList); 
            }
            rslt = SDK_OnlineWait;
            gucErrCode = ERRCODE_SUC;
        }
    }
#endif      //20150409 ��Ӧ�ÿ��ǽӽ��׽����ն��޶�ת��������

    qPBOCFORMTLV:   //20150409 �����תλ��

    //��֯��Ӧ����
    usSendPos = LL_LENGTH;
    //DF37,DF76,DF77,DF78,DF79
    //��6������Ԫδ����
    usSendPos += FormStdTransTlv(&pSend[2], rslt);

    sdkU16ToBcd(&pSend[0], usSendPos - LL_LENGTH, LL_LENGTH);
    pstTransFlow->usSendDataLen = usSendPos;

    if (rslt != SDK_OnlineWait && rslt != SDK_IccEasyFlow) {
        FinishTradeFlow();
    }

    rslt = WriteLastTrans(&pSend[0], usSendPos);
    Trace("Mai", "\n\n\n 444444444 rslt = %d \n\n\n", rslt);

    FreeTagList(pTagList);
    return INS_SUC;
}


void DispAmt(u8 *pasStr, s32 siRow, u32 ucAtr) {
    if (memcmp(gucAmount, "000000000000", 12) && pasStr) {
        u8 ucdispamount[32] = {0};
        u8 temp[12] = {0};
        if (strlen(pasStr) > 18) {
            return;
        }
        strcpy(ucdispamount, pasStr);
        sdkAscToBcd(temp, gucAmount, 12);
        RegulateMoney(ucdispamount + strlen(ucdispamount), temp);
        sdkDispFillRowRam(siRow, 0, ucdispamount, ucAtr);
    }
}


//ȡ������
s32 GetErrCode() {
#if 0
    #define ERRCODE_APPECNSF        0xf8        /*����ֻ֧�ֵ����ֽ��ֽ�����*/
#define ERRCODE_ONLYCARDECNSF   0xf9        /*�������ֽ𿨣�����*/
#define ERRCODE_NOSUPEC         0xfa        /*��֧�ֵ����ֽ�*/
#define ERRCODE_OVERLIMTAMT     0xfb        /*���׽������ն��޶�*/
#define ERRCODE_LOSTAID         0xfe        /*�ն�AID������ʧ*/
#define ERRCODE_CARDANOMALY     0xff        /*��Ƭ�����쳣*/

#define ERRCODE_ACCEPT          0x01        /*��������*/
#define ERRCODE_ONLINE          0x03        /*����*/
#define ERRCODE_GAT2AAC         0x04        /*�ڶ���Generate AC����AAC*/
#define ERRCODE_FALLBACK        0xFE        /*FALLBACK*/
#define ERRCODE_ESC             0xFC        /*����ȡ��*/
#define ERRCODE_OVERCARDLIMTAMT 0xF5        /*Ȧ��������Ƭ�������*/
#define ERRCODE_ERR             0xFF        /*����ʧ��*/

    /*����ֻ֧�ֵ����ֽ��ֽ�����*/
#endif
    return SDK_OK;
}

//����ʱ��ص�ȫ�ֱ���
s32 gsiDCSecond = 0;                //����ʱ����:
s32 gsiDCRow = 0;                   //����ʱ��ʾ������: �����Ϊ0����1-5�򣬲���ʵ����ʱ���ǻ�ȴ���ʱ�����ڳ�ʱ����Ҫ����ʱ�Ľ���
s32 gsiDCTimer = 0;                 //����ʱ��ʼʱ��
s32 gsiDCKey = 0;                   //����ʱ��ȡ�İ���ֵ

s32 DispTradeEnd(s32 siRslt) {
    s32 rslt = siRslt;
    u8 tbuf[32] = {0};


    //���׳ɹ�
    if (SDK_OK == rslt) {
        /*
            ���׳ɹ�
            ���ѽ�1.00
            �����ֽ���519.00
            <����ʱ2s>
        */
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���׳ɹ�",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        DispAmt("���׽��:", SDK_DISP_LINE3, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        if (memcmp(gucEcBalance, "\x00\x00\x00\x00\x00\x00", 6)) {
            memset(tbuf, 0, sizeof(tbuf));
            strcpy(tbuf, "�����ֽ����:");
            RegulateMoney(tbuf + strlen(tbuf), gucEcBalance);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, tbuf,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            gsiDCRow = SDK_DISP_LINE5;
        }
        else {
            gsiDCRow = SDK_DISP_LINE4;
        }
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 5;
        sdkDispBrushScreen();
    }
    else if (SDK_ESC == rslt) {
        /*
            ����ȡ������ο�
            ��ȷ�Ϸ���������
            <����ʱ10s>
        */
        sdkDispClearScreen();
        memset(tbuf, 0, sizeof(tbuf));
        strcpy(tbuf, "����ȡ��");
        if (SDK_ICC_ICC == GetCurrentCardType()) {
            strcat(tbuf, ",��ο�");
        }
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, tbuf,
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ȷ�Ϸ���������",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        gsiDCRow = SDK_DISP_LINE4;
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 10;
        sdkDispBrushScreen();
    }
    else if (SDK_ERR == rslt) {
        /*
            �����쳣����ο�
            ��ȷ�Ϸ���������
            <����ʱ6s>
        */
        sdkDispClearScreen();
        memset(tbuf, 0, sizeof(tbuf));
        strcpy(tbuf, "�����쳣");
        if (SDK_ICC_ICC == GetCurrentCardType()) {
            strcat(tbuf, ",��ο�");
        }
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, tbuf,
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ȷ�Ϸ���������",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        gsiDCRow = SDK_DISP_LINE4;
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 10;
        sdkDispBrushScreen();
    }
    else {
        sdkDispClearScreen();
        memset(tbuf, 0, sizeof(tbuf));
        strcpy(tbuf, "���׽���");
        if (SDK_ICC_ICC == GetCurrentCardType()) {
            strcat(tbuf, ",��ο�");
        }
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, tbuf,
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ȷ�Ϸ���������",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
        gsiDCRow = SDK_DISP_LINE4;
        gsiDCTimer = sdkTimerGetId();
        gsiDCSecond = 10;
        sdkDispBrushScreen();
    }

    Trace("zcl", "rslt:%d second:%d Timer:%d Row:%d\r\n",
          rslt, gsiDCSecond, gsiDCTimer, gsiDCRow);

    return SDK_OK;
}


void DispClearAmount() {
    memset(gucAmount, 0, sizeof(gucAmount));
}

//�ر����еĶ������豸
void CloseAllReadCardDev() {
    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
}


//��DOLTab��֯TLV����
s32 FormTLVFromDOLTab(const u8 (*pDOLTab)[2], u8 usTagNum, u8 *ucpOutBuf, u16 *usipOutLen) {
    s32 len = 0, iRet = SDK_OK;
    u8 temp[64] = {0};
    u16 pboclen = 0;
    u8 i = 0;

    if (!pDOLTab || !ucpOutBuf || !usipOutLen || usTagNum == 0) {
        return SDK_PARA_ERR;
    }

    while (i < usTagNum) {
        iRet = sdkIccReadTLV(&pDOLTab[i][0], temp, &len);

        if (iRet == SDK_OK && len != 0) {
            //Tag
            if ((pDOLTab[i][0] & 0x1F) == 0x1F) {
                ucpOutBuf[pboclen++] = pDOLTab[i][0];
                ucpOutBuf[pboclen++] = pDOLTab[i][1];
            }
            else {
                ucpOutBuf[pboclen++] = pDOLTab[i][0];
            }

            if (len > 0xFF) {
                ucpOutBuf[pboclen++] = 0x82;
                sdkU16ToHex(ucpOutBuf + pboclen, len, 2);
                pboclen += 2;
            }
            else if (len > 0x7F) {
                ucpOutBuf[pboclen++] = 0x81;
                ucpOutBuf[pboclen++] = len;
            }
            else {
                ucpOutBuf[pboclen++] = len;
            }
            memcpy(&ucpOutBuf[pboclen], temp, len);
            pboclen += len;
        }
        i++;
    }
    *usipOutLen = pboclen;

    return SDK_OK;
}

