#define DEBUG_IGNORE_BATTERY (0)            //20150324 �ص��������� ��Ȼһֱ�������粻���

const cmd_Str TsCmdTab[] = {
        {TM_SET_BEEP,       OnSetBeep,      NULL},
        {TM_SET_LED,        OnSetLED,       NULL},
        {TM_SET_DATETIME,   OnSetDateTime,  NULL},
        {TM_GET_DATETIME,   OnGetDateTime,  NULL},
        {TM_SET_TER_PARAM,  OnSetTerParam,  NULL},
        {TM_GET_TER_PARAM,  OnGetTerParam,  NULL},
        {TM_CANCEL_OPER,    OnCancelOper,   NULL},
        {TM_UPDATE,         OnUpdate,       NULL},
        {TM_ECHO,           OnEcho,         NULL},
        {TM_CTRL_DEV,       OnCtrlDev,      NULL},
        {TM_GET_STATUS_DEV, OnGetDevStatus, NULL},
        {TM_CMD_END,        NULL,           NULL}
};

static u32 guiCurFileOffset = 0;

void SaveAppUpDateFlg();

#if 0//û�õ��ĺ���
/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������:���Ƴɹ����ص��ļ�
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
static s32 CopyTempFile()
{

	s32 rslt;
#if 0	
	s32 iCount = 0;
	s32 iBlockSize = 0;
	u8 ucData[1024] = {0};

	sdkDelFile(MPOS_APP_FILE);
	while(iCount < guiCurFileOffset)
	{
		iBlockSize = (guiCurFileOffset - iCount) > 1024? 1024: (guiCurFileOffset - iCount);
		rslt = sdkReadFile(MPOS_TMP_APP_FILE, ucData, iCount, &iBlockSize);
		if(rslt != SDK_FILE_OK)
			return SDK_ERR;
		
		rslt = sdkInsertFile(MPOS_APP_FILE, ucData, iCount, iBlockSize);
		if(rslt != SDK_FILE_OK)
			return SDK_ERR;

		iCount += iBlockSize;
	}
#endif
	
	rslt = sdkRenameFile(MPOS_TMP_APP_FILE, MPOS_APP_FILE);
	Trace("Mai", "rslt  = %d\r\n", rslt);
	return SDK_OK;
}
#endif
static u32 timer = 0;

static s32 DispDownStatus(s32 iSize, u8 status) {


    if (status == START_DOWNLOAD) {
        sdkDispClearScreen();
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�̼�����", SDK_DISP_DEFAULT);
        //sprintf(disp,"������: % 3d KB", (iSize/1024));
        //sdkDispFillRowRam(SDK_DISP_LINE3, 0, disp, SDK_DISP_DEFAULT);

        //memset(disp, 0, 32);
        //sprintf(disp,"���غ�ʱ: %d ms", sdkTimerGetId() - timer);
        //sdkDispFillRowRam(SDK_DISP_LINE4, 0, disp, SDK_DISP_DEFAULT);
        //sdk_dev_sys_printf(disp);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���Ե�...", SDK_DISP_DEFAULT);

    }
    else if (status == DOWNLOADING) {
        //sdkDispFillRowRam(SDK_DISP_LINE3, 0, "�̼�����", SDK_DISP_DEFAULT);
        //sdkDispFillRowRam(SDK_DISP_LINE4, 0, "���Ե�...", SDK_DISP_DEFAULT);

    }
    else {
        sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�̼�����...", SDK_DISP_DEFAULT);
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���½���,�Զ�����!", SDK_DISP_DEFAULT);
    }
    sdkDispBrushScreen();
    return SDK_OK;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���÷���������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnSetBeep(ST_TRANSFLOW *pstTransFlow) {
    u16 usCount = 0;
    u16 usFreq = 0;
    u16 usTimer = 0;
    u16 usSpaceTime = 0;
    s32 iTimerID;

    TraceHex("Mai", "����:", &pstTransFlow->heReceiveData[0], 8);
    sdkHexToU16(&usCount, &pstTransFlow->heReceiveData[0], 2);
    sdkHexToU16(&usFreq, &pstTransFlow->heReceiveData[2], 2);
    sdkHexToU16(&usTimer, &pstTransFlow->heReceiveData[4], 2);
    sdkHexToU16(&usSpaceTime, &pstTransFlow->heReceiveData[6], 2);
    Trace("Mai", "���� nCount = %d,nFreq = %d,usTimer = %d,nSpaceTime = %d:\n", usCount, usFreq,
          usTimer, usSpaceTime);

    if (usFreq == 0)
        usFreq = 1500;        //ȱʡΪ1500hz

    sdkSysEnableBeep(true);    //ʹ�ܷ�����
    sdkSysSetBeepTune(usFreq, usTimer);
    while (usCount--) {
        iTimerID = sdkTimerGetId();
        while (sdkTimerIsEnd(iTimerID, usSpaceTime) == FALSE);
        Trace("Mai", "�������� = %d\n", usCount);

        sdkSysBeep(SDK_SYS_BEEP_OK);
        iTimerID = sdkTimerGetId();
        while (sdkTimerIsEnd(iTimerID, usTimer) == FALSE);
    }

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ָʾ����˸ָ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnSetLED(ST_TRANSFLOW *pstTransFlow) {
    u8 i = 4;
    u8 ucColor = 0;
    u16 usCount = 0;
    u16 usSpaceTime = 0;
    s32 iLedNum = 0;
    s32 iRet;
    SDK_PED_LAMP_CTRL_EXT *pstLampCtrlEx;
    SDK_PED_LAMP_CTRL_EXT stLampCtrlEx[4];

    Trace("Mai", "Enter set LED!\n");
    memset(stLampCtrlEx, 0x00, sizeof(stLampCtrlEx));

    ucColor = pstTransFlow->heReceiveData[2];
    sdkHexToU16(&usCount, &pstTransFlow->heReceiveData[0], 2);
    sdkHexToU16(&usSpaceTime, &pstTransFlow->heReceiveData[3], 2);
    Trace("Mai", "LED nCount = %d, usSpaceTime = %d\n", usCount, usSpaceTime);

    if (!(ucColor & 0x0F))
        return INS_PARAM_ERR;
    Trace("Mai", " ===============LED==================== \n");
    while (i--) {
        pstLampCtrlEx = &stLampCtrlEx[iLedNum];
        switch (ucColor & (0x01 << i)) {
            case 0x01: //blue
                stLampCtrlEx[iLedNum++].lempID = SDK_PED_LAMP_1;
                break;

            case 0x02: //green
                stLampCtrlEx[iLedNum++].lempID = SDK_PED_LAMP_3;
                break;

            case 0x04: //yellow
                stLampCtrlEx[iLedNum++].lempID = SDK_PED_LAMP_2;
                break;

            case 0x08: //red
                stLampCtrlEx[iLedNum++].lempID = SDK_PED_LAMP_4;
                break;
        }
        pstLampCtrlEx->lempState = SDK_LAMP_TWINK;
        pstLampCtrlEx->siOnTimers = 50;                            //��50 ms
        pstLampCtrlEx->siOffTimers = usSpaceTime;                    //��
        pstLampCtrlEx->siTimers = (usSpaceTime + 50) * usCount;        //����ʱ��

    }

    iRet = sdkPEDLampControlExt(iLedNum, stLampCtrlEx);
    Trace("Mai", "LED iRet = %d, LED NUM = %d\n", iRet, iLedNum);
    if (iRet != SDK_OK)
        return INS_OTHER_ERR;

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����ʱ������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnSetDateTime(ST_TRANSFLOW *pstTransFlow) {
    s32 rslt;
    u8 ucTime[6] = {0};

    sdkAscToBcd(ucTime, &pstTransFlow->heReceiveData[2], 12);
    TraceHex("Mai", "ʱ������:", ucTime, 6);

    rslt = sdkSetRtc(ucTime);
    if (rslt == SDK_OK)
        return INS_SUC;
    else if (rslt == SDK_PARA_ERR)
        return INS_PARAM_ERR;
    else
        return INS_OTHER_ERR;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡʱ������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnGetDateTime(ST_TRANSFLOW *pstTransFlow) {
    s32 rlst;
    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    u8 ucTime[6] = {0};

    rlst = sdkGetRtc(ucTime);
    if (rlst != SDK_OK)
        return INS_OTHER_ERR;

    strcpy(pSend, "20");
    sdkBcdToAsc(&pSend[2], ucTime, 6);
    pstTransFlow->usSendDataLen = 14;

    Trace("Mai", "ʱ��:%14s", pSend);

    return INS_SUC;
}

/*****************************************************************************
** Descriptions:   �����ͬ����Tag�����������Ȼ��׷�ӵ���β
** Parameters:     const u8 *pheTag
                   const u8 *pheInData
                   s32 siInLen
** Returned value: 
** Created By:     //sujianzhong 2015.01.08 20:18
** Remarks:        
*****************************************************************************/
s32 ConfigPagTLV(u8 *fileName, const u8 *pheTag, const u8 *pheInData, s32 siInLen) {
    u8 fn[128] = {0}, *temp = NULL;
    s32 totallen = 0, index = 0, taglen = 0, len = 0, tlvlen;

    if (fileName == NULL || NULL == pheTag || (NULL == pheInData && 0 != siInLen) ||
        siInLen > 255) {
        return SDK_PARA_ERR;
    }
    memset(fn, 0, sizeof(fn));
    strcat(fn, fileName);
    if (!sdkAccessFile(fn)) {
        return SDK_PARA_ERR;
    }

    totallen = sdkGetFileSize(fn);
    if (totallen < 0) return SDK_PARA_ERR;

    temp = sdkGetMem(totallen + 2 + 2 + siInLen);
    if (temp == NULL) return SDK_PARA_ERR;
    if (SDK_FILE_OK != sdkReadFile(fn, temp, 0, &totallen)) {
        sdkFreeMem(temp);          //20150113 modify by yc sdkFreeMem
        return SDK_PARA_ERR;
    }

    if ((pheTag[0] & 0x1F) == 0x1F) {
        taglen = 2;
    }
    else {
        taglen = 1;
    }
    index = 0;//�ڶ����ֽڿ�ʼ����tlv����
    while (index < totallen) {
        tlvlen = 0;
        if ((temp[index] & 0x1F) == 0x1F)//ƫ��Tag����
        {
            tlvlen = 2;
        }
        else {
            tlvlen = 1;
        }

        if (temp[index + tlvlen] > 127) {
            tlvlen++;
        }
        len = temp[index + tlvlen];//ƫ��len����
        tlvlen++;
        tlvlen += len;//ƫ��data����

        if (memcmp(pheTag, temp + index, taglen))//��ǩ��ͬ
        {
            index += tlvlen;
            continue;
        }
        else//�ҵ���ͬ�ı�ǩ
        {//ɾ��ԭ��ǩ����
            memcpy(temp + index, temp + index + tlvlen, totallen - index - tlvlen);
            totallen -= tlvlen;
            break;
        }
    }

    memcpy(temp + totallen, pheTag, taglen);//�ڽ�β׷���±�ǩ������
    totallen += taglen;
    if (siInLen > 127) {
        temp[totallen++] = 0x81;
    }
    temp[totallen++] = (u8) siInLen;
    memcpy(temp + totallen, pheInData, siInLen);
    totallen += siInLen;
//    TraceHex(__FUNCTION__, "����", temp, totallen+18);
    if (SDK_FILE_OK != sdkWriteFile(fn, temp, totallen)) {
        sdkFreeMem(temp);          //20150113 modify by yc sdkFreeMem
        return SDK_ERR;
    }
    sdkFreeMem(temp);          //20150113 modify by yc sdkFreeMem
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:   ��ȡ�ConfigPagTLV����֮���TLV����
** Parameters:     const u8 *pheTag
                   u8 *pheOutData
                   s32 *psiOutLen
** Returned value: 
** Created By:     //sujianzhong 2015.01.08 20:18
** Remarks:        
*****************************************************************************/
s32 ReadPagTLV(u8 *fileName, const u8 *pheTag, u8 *pheOutData, s32 *psiOutLen) {
    u8 fn[128] = {0}, *temp = NULL;
    s32 totallen = 0, index = 0, taglen = 0, len = 0, tlvlen = 0;

    if (fileName == NULL || NULL == pheTag || (NULL == pheOutData) || NULL == psiOutLen) {
        return SDK_PARA_ERR;
    }

//    TraceHex("zcl", "tag:", pheTag, 2);
    memset(fn, 0, sizeof(fn));
    strcat(fn, fileName);

    if (!sdkAccessFile(fn)) {
        return SDK_PARA_ERR;
    }

    totallen = sdkGetFileSize(fn);
    if (totallen < 0) return SDK_PARA_ERR;

    temp = sdkGetMem(totallen);
    if (temp == NULL) return SDK_PARA_ERR;

    if (SDK_FILE_OK != sdkReadFile(fn, temp, 0, &totallen)) {
        sdkFreeMem(temp);      //20150113 modify by yc sdkFreeMem
        return SDK_PARA_ERR;
    }
//    TraceHex("zcl", "����", temp, totallen);

    if ((pheTag[0] & 0x1F) == 0x1F) {
        taglen = 2;
    }
    else {
        taglen = 1;
    }
    index = 0;//�ڶ����ֽڿ�ʼ����tlv����
    while (index < totallen) {
        tlvlen = 0;
        if ((temp[index] & 0x1F) == 0x1F)//ƫ��Tag����
        {
            tlvlen = 2;
        }
        else {
            tlvlen = 1;
        }

        if (temp[index + tlvlen] > 127) {
            tlvlen++;
        }
        len = temp[index + tlvlen];//ƫ��len����
        tlvlen++;
        tlvlen += len;//ƫ��data����

        if (memcmp(pheTag, temp + index, taglen))//��ǩ��ͬ
        {
            index += tlvlen;
            continue;
        }
        else//�ҵ���ͬ�ı�ǩ
        {
            memcpy(pheOutData, temp + index + tlvlen - len, len);
            *psiOutLen = len;
            sdkFreeMem(temp);      //20150113 modify by yc sdkFreeMem
            return SDK_OK;
        }
    }

    sdkFreeMem(temp);      //20150113 modify by yc sdkFreeMem
    return SDK_ERR;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �����ն˲���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnSetTerParam(ST_TRANSFLOW *pstTransFlow) {
    //u32 nTag;
    //s32 rslt;
    u16 usRecvLen;
    u16 usValueLen;
    u16 usRecvPos = 2;
    bool bIsExist = false;
    //stMposTermpara stTermPara;
    u8 *pRecvData = pstTransFlow->heReceiveData;
    u8 tag[2 + 1] = {0};

    Trace("Mai", "Enter set parameter\n");

    bIsExist = sdkAccessFile(MPOS_PARA_FILE);
    if (!bIsExist)
        sdkWriteFile(MPOS_PARA_FILE, pRecvData, 0);

    sdkBcdToU16(&usRecvLen, &pstTransFlow->heReceiveData[0], 2);
    if (usRecvLen > 1024)
        return INS_PARAM_ERR;

    usRecvLen += 2;//LL.1024
    while (usRecvLen > usRecvPos) {
        if (pRecvData[usRecvPos] == 0) usRecvPos++;
        memset(tag, 0, sizeof(tag));
        if ((pRecvData[usRecvPos] & 0x1F) == 0x1F) {
            memcpy(tag, pRecvData + usRecvPos, 2);
            usRecvPos += 2;
        }
        else {
            tag[0] = pRecvData[usRecvPos++];
        }
        if (pRecvData[usRecvPos] > 127) {
            usRecvPos++;
        }
        usValueLen = pRecvData[usRecvPos++];

        if (SDK_OK != ConfigPagTLV(MPOS_PARA_FILE, tag, pRecvData + usRecvPos, usValueLen))
            return INS_OTHER_ERR;

        TraceHex("Mai", "����\n", &pRecvData[usRecvPos], 5);
        usRecvPos += usValueLen;

    }

    Trace("Mai", "Set parameter success!\n");
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�ն˲���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnGetTerParam(ST_TRANSFLOW *pstTransFlow) {
    //s32 rslt;
    //u32 nTag;

    u32 uiValueLen;
    u8 *pRecvData = pstTransFlow->heReceiveData;
    u8 pSendData[1024] = {0};   //0416Send==Recv
    u16 usRecvLen;
    u16 usRecvOffset = 2;
    u16 usSendOffset = 2;
    //bool bIsExist = false;

    u8 ucValue[256];
    //stMposTermpara stTermPara;
    u8 tag[2 + 1];


    Trace("Mai", "Enter Get Parameter!");
    sdkBcdToU16(&usRecvLen, &pstTransFlow->heReceiveData[0], 2);
    if (usRecvLen > 1024)
        return INS_PARAM_ERR;

    usRecvLen += 2;//LL.1024
    TraceHex("Mai", "Get Parameter:\n", pRecvData, usRecvLen);
    while (usRecvLen > usRecvOffset) {
        if (pRecvData[usRecvOffset] == 0) {
            pSendData[usSendOffset++] = pRecvData[usRecvOffset++];
        }
        memset(tag, 0, sizeof(tag));
        if ((pRecvData[usRecvOffset] & 0x1F) == 0x1F) {
            memcpy(tag, pRecvData + usRecvOffset, 2);
            memcpy(pSendData + usSendOffset, pRecvData + usRecvOffset, 2);
            usRecvOffset += 2;
            usSendOffset += 2;
        }
        else {
            tag[0] = pRecvData[usRecvOffset];
            pSendData[usSendOffset++] = pRecvData[usRecvOffset++];
        }

        memset(ucValue, 0, sizeof(ucValue));
        uiValueLen = 0;
        if (SDK_OK != ReadPagTLV(MPOS_PARA_FILE, tag, ucValue,
                                 &uiValueLen)) {//�ų���˵��������tag��"tag"+"01 30"
            uiValueLen = 1;
            ucValue[0] = 0x30;
        }
        if (uiValueLen > 127) pSendData[usSendOffset++] = 0x81;
        pSendData[usSendOffset++] = uiValueLen;

        memcpy(&pSendData[usSendOffset], ucValue, uiValueLen);
        usSendOffset += uiValueLen;
    }
    TraceHex("Mai", "Get Parameter:\n", pSendData, usSendOffset);
    sdkU16ToBcd(&pSendData[0], usSendOffset - 2, 2);
    pstTransFlow->usSendDataLen = usSendOffset;
    memcpy(pstTransFlow->heReceiveData, pSendData, usSendOffset);   //0416Send==Recv

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ȡ��/��λ����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnCancelOper(ST_TRANSFLOW *pstTransFlow) {
    Trace("Mai", "���븴λ����\n");

#if NEWCANCELCMD_MODE                   //20150402 ���µĸ�λ��ʽ
    //20150402 �µĸ�λ��ʽ: ���յ���λ����ĵ�һ��ʱ���ø�λ���ڶ�����ʱ1���Ӻ��ٻָ���λ��ʶ��
    //��������⵽��λʱ���ָ���λ��ʶ������ֻ�е�һ�������˳�����û����Ϣʱ�յ���λ����󣬺����ĵ�һ�����ִ��
	static s32 timeout = 0;

    if(0 == pstTransFlow->ucInsRunStep)
    {           
        pstTransFlow->ucInsRunStep = 1;
        timeout = sdkTimerGetId();
        SetCmdResetExe(NEED_RESET);        

	    FinishTradeFlow();              //20150320 ��ӽ��׽���������ʱ�������û�н��׽������� ����POS�˼���(û���˳����ף������ܰ�����ˢ��)������: ��������ˢ�ſ��������볬ʱǰ�ֻ��˰�ȡ��
    	pgstTradeData->ucTradeId = TRADEID_MAINMENU;    //20150320 ���⸴λ������г�ʱ����ִ��ʱ�Ῠ������
        sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
        sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
        //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

        gsiDCSecond = 0;
        gsiDCKey = 0;
        gsiDCTimer = 0;
        gsiDCRow = 0;

        return INS_KEEP;
    }
    else if(1 == pstTransFlow->ucInsRunStep)
    {
        if( sdkTimerIsEnd(timeout, 1000) )
        {
            return INS_KEEP;
        }

        SetCmdResetExe(NO_NEED_RESET);
        sdkDispClearStateBar();
	    sdkDispStateBar(TRUE);

        //20150402 ���ȡ������ʱ
        gsiDCSecond = 0;
        gsiDCKey = 0;
        gsiDCTimer = 0;
        gsiDCRow = 0;
    
        pstTransFlow->heReceiveData[0] = 0x00;          //0416Send==Recv
    	pstTransFlow->usSendDataLen = 1;

    	FinishTradeFlow();              //20150320 ��ӽ��׽���������ʱ�������û�н��׽������� ����POS�˼���(û���˳����ף������ܰ�����ˢ��)������: ��������ˢ�ſ��������볬ʱǰ�ֻ��˰�ȡ��
	    pgstTradeData->ucTradeId = TRADEID_MAINMENU;    //20150320 ���⸴λ������г�ʱ����ִ��ʱ�Ῠ������
    
        return INS_SUC;
    }
	//
#else
    SetCmdResetExe(NEED_RESET);
    //���״̬��
    sdkDispClearStateBar();
    sdkDispStateBar(TRUE);

    pstTransFlow->heReceiveData[0] = 0x00;             //0416Send==Recv
    pstTransFlow->usSendDataLen = 1;

    FinishTradeFlow();              //20150320 ��ӽ��׽���������ʱ�������û�н��׽������� ����POS�˼���(û���˳����ף������ܰ�����ˢ��)������: ��������ˢ�ſ��������볬ʱǰ�ֻ��˰�ȡ��
    pgstTradeData->ucTradeId = TRADEID_MAINMENU;    //20150320 ���⸴λ������г�ʱ����ִ��ʱ�Ῠ������
    sdkIccCloseRfDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseIcDev();             //20150324 ��λ��ӹض����豸
    sdkIccCloseMag();               //20150324 ��λ��ӹض����豸
    //sdkIccClearEmvData();           //20150324 ��λ����ͷ�EMV��Դ

    gsiDCSecond = 0;
    gsiDCKey = 0;
    gsiDCTimer = 0;
    gsiDCRow = 0;

    return INS_SUC;
#endif
}


/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �����̼�/Ӧ��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 	������ʱ����������������,ÿ��
   				����������ʱ(K200P)��100ms����
 ********************************************************************/
s32 OnUpdate(ST_TRANSFLOW *pstTransFlow) {
    u8 ucStatus;
    u16 usContentLen;
    u8 *pContent = NULL;
    s32 rslt;
    u8 pSend[24] = {0};          //0416Send==Recv

    ucStatus = pstTransFlow->heReceiveData[0];
    sdkBcdToU16(&usContentLen, pstTransFlow->heReceiveData + 1, 2);
    pContent = pstTransFlow->heReceiveData + 3;

    if ((usContentLen > 1024) || (pstTransFlow->ReceiveDataLen < 3)) {
        rslt = SDK_ERR;
        goto RE_STATE;
    }

    if (pstTransFlow->ucInsRunStep == 1) {
        DispDownStatus(guiCurFileOffset, END_DOWNLOAD);
        SaveAppUpDateFlg();
        Trace("Mai", "���������ļ�������");
        sdkSysCmd("reboot", NULL);
        return INS_SUC;
    }

    /*
    u8 tmp[16];
    sprintf(tmp, "����%d\n", sdkTimerGetId());
    sdk_dev_sys_printf(tmp);
    */
    GotoTradeFlow();

    switch (ucStatus) {
        case START_DOWNLOAD:
            timer = sdkTimerGetId();
            sdkDelFile(MPOS_APP_FILE);
            rslt = Private_sdkCreatFastFile(MPOS_APP_FILE);
            Trace("Mai", "Private_sdkCreatFastFile rslt = %d \r\n", rslt);
            Trace("Mai", "��ʼ����ʱ�� timer:%d\r\n", sdkTimerGetId());
            rslt = sdkWriteFile(MPOS_APP_FILE, pContent, usContentLen);
            //rslt = SDK_FILE_OK;
            break;

        case DOWNLOADING:
            rslt = SDK_PARA_ERR;
            if (guiCurFileOffset == 0)
                goto RE_STATE;

            //sprintf(tmp, "�ļ�1 %d\n", sdkTimerGetId());
            //sdk_dev_sys_printf(tmp);
            //Trace("Mai", "�������� timer:%d\r\n", sdkTimerGetId());
            rslt = sdkInsertFile(MPOS_APP_FILE, pContent, guiCurFileOffset, usContentLen);
            //rslt = SDK_FILE_OK;
            //sprintf(tmp, "�ļ�2 %d\n", sdkTimerGetId());
            //sdk_dev_sys_printf(tmp);

            break;

        case END_DOWNLOAD:
            Trace("Mai", "���ؽ��� timer:%d\r\n", sdkTimerGetId());
            rslt = sdkInsertFile(MPOS_APP_FILE, pContent, guiCurFileOffset, usContentLen);
            //rslt = SDK_FILE_OK;
            Private_sdkCloseFastFile(MPOS_APP_FILE);
            break;

        default:
            rslt = SDK_PARA_ERR;
    }


    RE_STATE:

    pstTransFlow->usSendDataLen = 23;
    pSend[0] = pstTransFlow->heReceiveData[0];

    if (rslt == SDK_FILE_OK) {
        memcpy(pSend + 1, "00", 2);
        //sprintf(tmp, "��ϣ1 %d\n", sdkTimerGetId());
        //sdk_dev_sys_printf(tmp);
        sdkSHA1(pContent, usContentLen, pSend + 3);
        //sprintf(tmp, "��ϣ2 %d\n", sdkTimerGetId());
        //sdk_dev_sys_printf(tmp);

        if (ucStatus == START_DOWNLOAD) {
            guiCurFileOffset = usContentLen;
            DispDownStatus(guiCurFileOffset, START_DOWNLOAD);
        }
        else if (ucStatus == DOWNLOADING) {
            guiCurFileOffset += usContentLen;
            DispDownStatus(guiCurFileOffset, DOWNLOADING);
        }
        else {
            DispDownStatus(guiCurFileOffset, DOWNLOADING);
            pstTransFlow->ucInsRunStep = 1;
            guiCurFileOffset = 0;
            return INS_SEND_NOT_OUT;
        }

        memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
        Trace("Mai", "Size = %d\n", guiCurFileOffset);
        return INS_SUC;
    }
    else if (rslt == SDK_PARA_ERR) {
        memcpy(pSend + 1, "01", 2);
        memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
        return INS_PARAM_ERR;
    }
    else if (rslt == SDK_ERR) {
        memcpy(pSend + 1, "04", 2);
        memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
        return INS_OTHER_ERR;
    }
    else {
        memcpy(pSend + 1, "02", 2);
        memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
        return INS_OTHER_ERR;
    }
}


/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnEcho(ST_TRANSFLOW *pstTransFlow) {

    //memcpy(pstTransFlow->heSendData, pstTransFlow->heReceiveData, pstTransFlow->ReceiveDataLen);  //0416Send==Recv
    pstTransFlow->usSendDataLen = pstTransFlow->ReceiveDataLen;

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: �����豸
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: �ڷ�����Ӧ���ĺ����߻�ػ�
 ********************************************************************/
s32 OnCtrlDev(ST_TRANSFLOW *pstTransFlow) {
    u8 ucCmd;

    ucCmd = pstTransFlow->heReceiveData[0];
    if (pstTransFlow->flag1 == 0) {
        pstTransFlow->flag1 = 1;
        return INS_SEND_NOT_OUT;
    }

    if (ucCmd == 0x01) //�ػ�
    {
        //sdkDealPowerOff();
        sdkSysCmd("poweroff", NULL);
    }
    else if (ucCmd == 0x02) {
        sdkSysEnterIdle(DEV_SLEEP, 0);        //����2�ֽ׶�������
    }

    return INS_NOT_RETURN;    //�����쳣�����ñ��Ĵ�����ٷ�����Ӧ����
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�豸״̬
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 OnGetDevStatus(ST_TRANSFLOW *pstTransFlow) {
    s32 iRet;
    bool bIsExpower;
    static u32 timer = 0;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

//����ʱ����Щ���ױ��͵�ѹ�޷�����
#if DEBUG_IGNORE_BATTERY

    memcpy(pSend, "99", 2);

    pSend[2] = 0x00;
    pSend[3] = 0x00;

    pstTransFlow->usSendDataLen = 4;
    return INS_SUC;

#endif

    bIsExpower = sdkSysIsExPower();
    iRet = sdkSysGetBatteryStateAndControl();
//#ifdef DEBUG_CODE_TEST
#if 0
    iRet = FULL_POWER;
#endif
    Trace("Mai", "��� = %d\n", iRet);
    switch (iRet) {
        case BATTERY_ERR:
            timer = 0;
            if (bIsExpower) {
                memcpy(pSend, "00", 2);
                break;
            }
            else
                return INS_OTHER_ERR;

        case NO_POWER:
        case LOW_POWER:
            timer = 0;
            memcpy(pSend, "00", 2);
            break;

        case ONE_BAR:
#if 1
            memcpy(pSend, "14",
                   2);       //20150326 ��������ʱ��1��緵�ص���Ϊ14% ��Ϊ�人����SDKĿǰ��13-15%Ϊ�͵���״̬ �и�����Ϊ�͵�������
#else
            memcpy(pSend, "25", 2);
            //20150325 Ϊ���Ե͵����������װ���������������: һ���ʱ����ӵ�Դ5���Ӻ���ʾΪ�͵�13-15%
            if( (!bIsExpower) && (timer != 0) && sdkTimerIsEnd(timer, 300000) )
            {
                //20150325 ���Ե������ʱ����Ҫ�����Ϊ�ͼ���ˢ��: ȫ��SDK����POS���ص���13-15%ʱ�Ż��߸İ���
                memcpy(pSend, "14", 2);
            }
            else if( bIsExpower )
            {
                timer = 0;
            }
            else if( 0 == timer )
            {
                timer = sdkTimerGetId();
            }
#endif
            break;

        case TWO_BAR:
            timer = 0;
            memcpy(pSend, "50", 2);
            break;

        case THREE_BAR:
            timer = 0;
            memcpy(pSend, "75", 2);
            break;

        case FULL_POWER:
            timer = 0;
            memcpy(pSend, "99", 2);
            break;

        default:
            timer = 0;
            return INS_OTHER_ERR;

    }

    if (bIsExpower) {
        //20150327 ���´�½���ݷ��ֳ���ʱ��������ص�00   ���ǳ���ʱ��֪������ʼ�շ���1��绹�Ǹ����ģ��ֻ���������ʾ�͵���
        if (iRet == BATTERY_ERR || iRet == FULL_POWER) {
            pSend[2] = 0x00;
        }
        else {
            memcpy(pSend, "00", 2);
            pSend[2] = 0x01;
        }
        pSend[3] = 0x01;            //G2ֻ֧��USB ����

    }
    else
        pSend[3] = 0x00;

    pstTransFlow->usSendDataLen = 4;

    Trace("Mai", "��� = %d\n", iRet);
    return INS_SUC;
}

/*******************************************************************
Function Nam :u32 GetHash(u32 crc, u8 * szSrc, u32 dwSrcLen)
Function Purpose : Get Hash value
Input Paramters : 1:crc - initial crc
2zSrc - source data
3wSrcLen -source data len
Return Value:crc value
Remark: 0722
********************************************************************/
u32 GetHash(u32 crc, u8 *szSrc, u32 dwSrcLen) {
    u32 len = dwSrcLen;
    u8 *buf = (unsigned char *) szSrc;
    u32 mtbCRC32[256];
    u32 i;
    const u32 tbCRC32[256] = { //sxl Ҫ�鿴�Ƿ���flash��
            0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F, 0xE963A535,
            0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988, 0x09B64C2B, 0x7EB17CBD,
            0xE7B82D07, 0x90BF1D91,
            0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551,
            0x83D385C7, 0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
            0xFA0F3D63, 0x8D080DF5,
            0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172, 0x3C03E4D1, 0x4B04D447, 0xD20D85FD,
            0xA50AB56B, 0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75,
            0xDCD60DCF, 0xABD13D59,
            0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423, 0xCFBA9599,
            0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924, 0x2F6F7C87, 0x58684C11,
            0xC1611DAB, 0xB6662D3D,
            0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5,
            0xE8B8D433, 0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
            0x91646C97, 0xE6635C01,
            0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E, 0x6C0695ED, 0x1B01A57B, 0x8208F4C1,
            0xF50FC457, 0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49,
            0x8CD37CF3, 0xFBD44C65,
            0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7, 0xA4D1C46D,
            0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0, 0x44042D73, 0x33031DE5,
            0xAA0A4C5F, 0xDD0D7CC9,
            0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409,
            0xCE61E49F, 0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
            0xB7BD5C3B, 0xC0BA6CAD,
            0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A, 0xEAD54739, 0x9DD277AF, 0x04DB2615,
            0x73DC1683, 0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D,
            0x0A00AE27, 0x7D079EB1,
            0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB, 0x196C3671,
            0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC, 0xF9B9DF6F, 0x8EBEEFF9,
            0x17B7BE43, 0x60B08ED5,
            0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD,
            0x48B2364B, 0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
            0x316E8EEF, 0x4669BE79,
            0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236, 0xCC0C7795, 0xBB0B4703, 0x220216B9,
            0x5505262F, 0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31,
            0x2CD99E8B, 0x5BDEAE1D,
            0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F, 0x72076785,
            0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38, 0x92D28E9B, 0xE5D5BE0D,
            0x7CDCEFB7, 0x0BDBDF21,
            0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1,
            0x18B74777, 0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
            0x616BFFD3, 0x166CCF45,
            0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2, 0xA7672661, 0xD06016F7, 0x4969474D,
            0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5,
            0x47B2CF7F, 0x30B5FFE9,
            0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693, 0x54DE5729,
            0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94, 0xB40BBE37, 0xC30C8EA1,
            0x5A05DF1B, 0x2D02EF8D
    };

    for (i = 0; i < 256; i++) {
        mtbCRC32[i] = tbCRC32[i];
    }

    while (len--) {
        crc = (crc >> 8) ^ mtbCRC32[(crc & 0xFF) ^ *buf++];
    }
    return crc;
}


#define READSIZE 512

u32 GetAppBinCrc(u32 *crc) {
    u32 file_crc = 0xffffffff;
    u32 file_len = 0;
    s32 total_len = 0;
    s32 onebaglen = 0;
    s32 ret = 0;
    u8 *data = NULL;

    total_len = sdkGetFileSize(MPOS_APP_FILE);
    if (total_len <= 0) {
        return 0;
    }

    data = (u8 *) sdkGetMem(READSIZE);
    if (data == NULL) {
        return 0;
    }

    while (1) {
        if ((total_len - file_len) > READSIZE) {
            onebaglen = READSIZE;
            ret = sdkReadFile(MPOS_APP_FILE, data, file_len, &onebaglen);
            if (ret != SDK_FILE_OK || onebaglen != READSIZE) {
                ret = -1;
                break;
            }

            file_len += onebaglen;
            file_crc = GetHash(file_crc, data, onebaglen);
        }
        else {
            onebaglen = total_len - file_len;
            if (onebaglen == 0) {
                break;
            }
            ret = sdkReadFile(MPOS_APP_FILE, data, file_len, &onebaglen);
            if (ret != SDK_FILE_OK || onebaglen != (total_len - file_len)) {
                ret = -1;
                break;
            }
            file_len += onebaglen;
            file_crc = GetHash(file_crc, data, onebaglen);
            break;
        }
    }
    file_crc ^= 0xffffffff;
    *crc = file_crc;
    sdkFreeMem(data);      //20150113 modify by yc sdkFreeMem
    if (ret < 0)
        return 0;
    else
        return total_len;
}

void SaveAppUpDateFlg() {
    SDK_INI_HAND pHand = NULL;
    u8 temp[64] = {0};
    u32 crc;
    u32 filelen = 0;

    filelen = GetAppBinCrc(&crc);
    Trace("Mai", "GetAppBinCrc: len:%d crc:%02x\r\n", filelen, crc);
    if (filelen == 0) {
        return;
    }

    sdkDelFile(MPOS_APP_UPDATEFLG);

    pHand = sdkCreatIni();
    Trace("Mai", "sdkCreatIni: pHand:%d\r\n", pHand);
    if (pHand == NULL)
        return;

    sdkIniAddSection(pHand, "dirlist", "");
    sdkIniAddKey(pHand, "dirlist", "dir0", "");
    sdkIniAddKeyValue(pHand, "dirlist", "dir0", "/mtd0", "");

    sdkIniAddSection(pHand, "namelist", "");
    sdkIniAddKey(pHand, "namelist", "file0", "");
    sdkIniAddKeyValue(pHand, "namelist", "file0", "K21App.bin", "");

    sprintf(temp, "%d", filelen);

    sdkIniAddSection(pHand, "lenlist", "");
    sdkIniAddKey(pHand, "lenlist", "len0", "");
    sdkIniAddKeyValue(pHand, "lenlist", "len0", temp, "");

    sprintf(temp, "0x%08X", crc);

    /*
    Trace("Mai","CRC32 = %s\n", temp);
    sdkmSleep(500);
    */

    sdkIniAddSection(pHand, "crclist", "");
    sdkIniAddKey(pHand, "crclist", "crc0", "");
    sdkIniAddKeyValue(pHand, "crclist", "crc0", temp, "");

    sdkIniAddSection(pHand, "adrlist", "");
    sdkIniAddKey(pHand, "adrlist", "adr0", "");
    sdkIniAddKeyValue(pHand, "adrlist", "adr0", "0x00070000", "");

    sdkIniSave(pHand, MPOS_APP_UPDATEFLG);


    Trace("Mai", "APP File = %d\n", sdkAccessFile(MPOS_APP_FILE));
    Trace("Mai", "Conf File = %d\n", sdkAccessFile(MPOS_APP_UPDATEFLG));
    sdkmSleep(500);

    sdkIniClose(pHand);

    //20150312 �������file.adr�ļ�
    u8 fadr[] = "/mtd0/file.adr";
    //u8	fdata[] = "[ADDR]\r\nName=K21App.bin\r\nStartAddr=0x00070000\r\nEndAddr=0x00070000\r\n";

    sdkDelFile(fadr);
    //sdkSaveOverlap(fadr, fdata, 59+8);
    pHand = sdkCreatIni();
    Trace("Mai", "sdkCreatIni: pHand:%d\r\n", pHand);
    if (pHand == NULL)
        return;

    sdkIniAddSection(pHand, "ADDR", "");
    sdkIniAddKey(pHand, "ADDR", "Name", "");
    sdkIniAddKeyValue(pHand, "ADDR", "Name", "K21App.bin", "");

    sdkIniAddKey(pHand, "ADDR", "StartAddr", "");
    sdkIniAddKeyValue(pHand, "ADDR", "StartAddr", "0x00070000", "");

    sdkIniAddKey(pHand, "ADDR", "EndAddr", "");
    sdkIniAddKeyValue(pHand, "ADDR", "EndAddr", "0x00070000", "");

    sdkIniSave(pHand, fadr);
    Trace("Mai", "file.adr File = %d\n", sdkAccessFile(fadr));
    sdkmSleep(500);

    sdkIniClose(pHand);

}

/*******************************************************************
   ��	 ��: ������
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�ն˺ź��̻���
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: 
 ********************************************************************/
s32 GetTerParam(stMposTermpara *stTermPara) {
    s32 rslt;
    s32 nFileLen = 0;
    u32 nTag;
    u16 usValueLen = 0;
    u16 usOffset = 0;
    bool bIsExist = false;
    u8 *pBuff = NULL;

    Trace("Mai", "Enter Get Parameter!");
    bIsExist = sdkAccessFile(MPOS_PARA_FILE);
    if (!bIsExist) {
        Trace("Mai", "");
        return SDK_ESC;
    }
    nFileLen = sdkGetFileSize(MPOS_PARA_FILE);
    Trace("Mai", "=%d\r\n", nFileLen);
    if (nFileLen <= 0)
        return SDK_ESC;
    pBuff = (u8 *) sdkGetMem(nFileLen + 1);
    if (NULL == pBuff) {
        return SDK_ESC;
    }
    memset(pBuff, 0, nFileLen + 1);
    rslt = sdkReadOverlap(MPOS_PARA_FILE, pBuff, nFileLen + 1);
    if (rslt != SDK_FILE_OK) {
        Trace("Mai", " ���ļ�ʧ��= %d\n", rslt);
        sdkFreeMem(pBuff);
        return SDK_ESC;
    }
    TraceHex("Mai", "Get Parameter:\n", pBuff, nFileLen);
    while (nFileLen > usOffset) {
        sdkHexToU32(&nTag, &pBuff[usOffset], 3);
        usOffset += 3;
        sdkHexToU16(&usValueLen, &pBuff[usOffset - 1], 1);
        switch (nTag) {
            case 0x5F0D0F:
                //�̻���
                memcpy(stTermPara->merchantNo, &pBuff[usOffset], usValueLen);
                break;

            case 0x5F0E08:
                //�ն˺�
                memcpy(stTermPara->terminalNo, &pBuff[usOffset], usValueLen);
                break;

            default:
                Trace("Mai", "Tag = 0x%X", nTag);
                sdkFreeMem(pBuff);
                return SDK_ESC;
        }
        usOffset += usValueLen;
    }
    sdkFreeMem(pBuff);
    return SDK_OK;
}


