extern volatile u32 gScreenTimer;
//ljh2015-01-07
extern volatile bool gIsScreenOn;

enum Commustate {
    COMMIDLEING = 1,                 //����״̬
    COMMLINKING,                         //����������
    COMMLINKSUC,                         //���ӳɹ�
};


/*****************************************************************************
** Descriptions:	Ӧ�ø��豸д����
** Parameters:    	const char *pszData
                               int nBufLen
** Returned value:	>0 ���ͳɹ�DK_ERR ����ʧ��
** Created By:		lilin2014.06.26
** Remarks: 		
*****************************************************************************/
static s32 AppCommuWrite(const char *pheData, u32 nBufLen, u32 uiTimeout) {
    s32 rslt = SDK_ERR;

    if (AUDIO == gucCurCommMode)        //�ж���Ƶ�豸
    {
        rslt = sdkCommUartSendData(SDK_COMM_UART_AUDIO, pheData, nBufLen);
    }
    else if (BLUETOOTH == gucCurCommMode) {
        rslt = sdkBtWrite(pheData, nBufLen, uiTimeout);
    }
    else if (USB_HID == gucCurCommMode) {
        rslt = sdkCommUartSendData(SDK_COMM_UART_USBHID, pheData, nBufLen);
    }
    else if (USB_CDC == gucCurCommMode) {
        rslt = sdkCommUartSendData(SDK_COMM_UART_USB, pheData, nBufLen);
    }
    else {
        Assert(0);
        return SDK_ERR;
    }
    return rslt;
}


/*****************************************************************************
** Descriptions:	Ӧ�ø��豸������
** Parameters:    	const char *pszData ���������ݳ���
                               int nBufLen ������ȡ���ݳ���
** Returned value:	>0 �����ݳ���SDK_ERR ������ʧ��
** Created By:		lilin2014.06.26
** Remarks: 		
*****************************************************************************/
s32 AppCommuRead(u8 *pheRecvBuf, const u32 iRecvBufMaxLen, const u32 iRecvTimeOut) {
    s32 rslt = SDK_ERR;

    if (AUDIO == gucCurCommMode)        //�ж���Ƶ�豸
    {
        rslt = sdkCommUartRecvData(SDK_COMM_UART_AUDIO, pheRecvBuf, iRecvBufMaxLen, iRecvTimeOut);
    }
    else if (BLUETOOTH == gucCurCommMode) {
        rslt = sdkBtRead(pheRecvBuf, iRecvBufMaxLen, iRecvTimeOut);
    }
    else if (USB_HID == gucCurCommMode) {
        rslt = sdkCommUartRecvData(SDK_COMM_UART_USBHID, pheRecvBuf, iRecvBufMaxLen, iRecvTimeOut);
    }
    else if (USB_CDC == gucCurCommMode) {
        rslt = sdkCommUartRecvData(SDK_COMM_UART_USB, pheRecvBuf, iRecvBufMaxLen, iRecvTimeOut);
    }
    else {
        Assert(0);
        return SDK_ERR;
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:	��ѯͨѶ״̬
** Parameters:    	void
** Returned value:	SDK_OK ���ͳɹ�SDK_ERR ����ʧ��
** Created By:		lilin2014.06.26
** Remarks: 		ֻ�з���SDK_OK ���ܽ��з�������
*****************************************************************************/
s32 AppGetCommuStatus(void) {
    u8 temp[32] = {0};
    s32 rslt = COMMIDLEING;


    if (AUDIO == gucCurCommMode)        //�ж���Ƶ�豸
    {
        rslt = sdkAudioCommState(temp);
        if (rslt == SDK_OK) {
            if (temp[0] == 0)        //��Ƶ����״̬����ʹ��
            {
                rslt = SDK_OK;
            }
            else {
                rslt = SDK_ERR;
            }
        }
    }
    else if (BLUETOOTH == gucCurCommMode) {
        rslt = sdkBtGetStatus();
        if (COMMLINKSUC == rslt)      //��������COMMLINKSUC 2����ʹ��
        {
            rslt = SDK_OK;
        }
        else {
            rslt = SDK_ERR;
        }
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:	�ж�ͨѶ��·�Ƿ�OK
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.06.25
** Remarks: 		
*****************************************************************************/
bool JudgeCommuDevIsok(void) {
    bool bisok = false;

    if (AUDIO == gucCurCommMode)        //�ж���Ƶ�豸
    {
        if (sdkAudioCommOpen() == SDK_OK) {
            bisok = true;
        }
    }
    else if (BLUETOOTH == gucCurCommMode)    //�ж������豸
    {
        if (sdkBtOpen() == SDK_OK) {
            bisok = true;
        }
    }
    return bisok;
}

/*****************************************************************************
** Descriptions:	��������
** Parameters:    	const u8 *pSendData  ���͵�����
                               const s32 iSendDataLen �������͵����ݳ���
                               const s32 iTimerOver    ��ʱʱ��
** Returned value:	>0���ͳɹ���������
** Created By:		lilin2014.06.25
** Remarks: 		���iTimerOver����0 ���Ƿ�����
                                ���iTimerOver>0 �������ʱ���ڻ�ȴ��豸���к���
*****************************************************************************/
s32 AppCommSendData(const u8 *pSendData, const u32 iSendDataLen, const u32 iTimerOver) {
    s32 rslt = SDK_ERR;
    //s32 time = sdkTimerGetId();


    TraceHex("commu", "����\r\n", pSendData, iSendDataLen);
    Trace("Mai", "���Ϳ�ʼʱ��:%d iTimerOver:%d\r\n", sdkTimerGetId(), iTimerOver);
#if 1


    while (1) {
        rslt = AppCommuWrite(pSendData, iSendDataLen, iTimerOver);
        if (rslt == SDK_ERR || rslt == 0)     //ʧ��
        {
            bisNeedBrushScreenWhenBack = true;//Xiaox 15-1-14
            sdkDispPromptBox(false, TV_AUTORET, "��������ʧ��\n");
            break;
        }
        else if (rslt == SDK_EIO)        //�豸δ����
        {
            bisNeedBrushScreenWhenBack = true;//Xiaox 15-1-14
            sdkDispPromptBox(false, TV_AUTORET, "�豸����ʧ��\n");
            break;
        }
        else if (rslt == SDK_TIME_OUT)   //��ʱ
        {
            bisNeedBrushScreenWhenBack = true;//Xiaox 15-1-14
            sdkDispPromptBox(false, TV_AUTORET, "�������ݳ�ʱ\n");
            break;
        }
        else //rslt > 0
        {
            //sdkDispLabel(SDK_DISP_LINE2,"�������ݳɹ�",SDK_DISP_CDISP|SDK_DISP_FDISP);
            //sdkmSleep(300);
            Trace("commu", "�������ݳɹ�");
            Trace("Mai", "���ͽ���ʱ��:%d iTimerOver:%d rslt:%d\r\n", sdkTimerGetId(), iTimerOver,
                  rslt);
            return SDK_OK;
        }

    }
#endif
    Trace("Mai", "���ͽ���ʱ��:%d iTimerOver:%d rslt:%d\r\n", sdkTimerGetId(), iTimerOver, rslt);
    return SDK_ERR;
}


/*****************************************************************************
** Descriptions:	��������
** Parameters:    	const u8 *pRecvData
                               const s32 *piRecvDataLen
                               const s32 iTimerOver
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 AppCommRecvData(const u8 *pRecvData, const u32 iRecvDataLen, const u32 iTimerOver) {
    s32 rslt = SDK_ERR;
    //s32 time = sdkTimerGetId();



    while (1) {
        rslt = AppCommuRead(pRecvData, iRecvDataLen, iTimerOver);
        if (rslt == SDK_ERR || rslt == 0)     //ʧ��
        {
            sdkDispPromptBox(false, TV_AUTORET, "��������ʧ��\n");
            break;
        }
        else if (rslt == SDK_EIO)        //�豸δ����
        {
            sdkDispPromptBox(false, TV_AUTORET, "�豸����ʧ��\n");
            break;
        }
        else if (rslt == SDK_TIME_OUT)   //��ʱ
        {
            sdkDispPromptBox(false, TV_AUTORET, "���ͳ�ʱ\n");
            break;
        }
        else //rslt > 0
        {
            Trace("Send", "�������ݳɹ�");
            return SDK_OK;
        }

    }
    return SDK_ERR;
}

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*****************************************************************************
** Descriptions:	����������ѯ��ѯ����
** Parameters:    	const u8 *pQueryData����
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		100ms��ѯ��ѯ1���ֽڸ�����
                                ���Ը���ʵ������������趨
*****************************************************************************/
s32 AppCommPollQuery(const u8 *pQueryData) {
    u8 data = 0;        // 1���ֽ�
    s32 len = 0;

    while (1) {
        len = AppCommuRead(&data, sizeof(data), 100);
        if (len > 0) {
            memcpy(pQueryData, &data, sizeof(data));
            len = sizeof(data);
            return len;
        }
        else {
            return 0;
        }
    }
    //return len;
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

#if 0
/*****************************************************************************
** Descriptions:	����������ѯ��ȡ��λ���·����ݲ�����
** Parameters:    	const u8 *pQueryData����
** Returned value:	
** Created By:		wlz2014.07.08
** Remarks: 		100ms��ѯ��ѯ1048���ֽڵ�����
*************************************************************/
void AppCommPollQueryAndExe(void)
{
    s32 rslt = 0;
	u32 iRecvDataLen = 1048;
	u8 *pRecvData = NULL;

	pRecvData = sdkGetMem(iRecvDataLen + 1);

	if(pRecvData != NULL)
	{
		memset(pRecvData, 0x00, iRecvDataLen + 1);
		
	   	rslt = AppCommuRead(pRecvData, iRecvDataLen, 100);
		if(rslt > 0)
		{
			//TraceHex("wlz","AppCommuRead \r\n",pRecvData,iRecvDataLen);
			iRecvDataLen = rslt; 
			PostTransFlow(pRecvData,(u16)iRecvDataLen);
	    }

		sdkFreeMem(pRecvData);
	}
	else
	{
		Trace("", "****************** pRecvData== NULL ********** \n");
	}
}
#endif

/*****************************************************************************
** Descriptions:	����������ѯ��ȡ��λ���·����ݲ�����
** Parameters:    	const u8 *pQueryData����
** Returned value:	
** Created By:		wlz2014.07.08
** Remarks: 		100ms��ѯ��ѯ1048���ֽڵ�����
** Modify: 		5 ms��ѯ��ѯ3072 ���ֽڵ�����,mai
*************************************************************/
void AppCommPollQueryAndExe(void) {
    s32 rslt = 0;
    u32 iRecvDataLen = 1088;    //3072 �ĳ� 1024+64
    u32 iTmpLen = 0;
    u32 iTimeOut = 5;
    u16 usDataLen = 1024;        //����Ϊ��
    u8 *pRecvData = NULL;

    pRecvData = sdkGetMem(iRecvDataLen);

    if (pRecvData != NULL) {
        memset(pRecvData, 0x00, iRecvDataLen);

        while (1) {
            rslt = AppCommuRead(pRecvData + iTmpLen, usDataLen - iTmpLen, iTimeOut);
            if (rslt <= 0) {
                //if(usDataLen > 20)
                //	Trace("Mai", "rslt = %d\n", rslt);
                break;
            }


            iTmpLen += rslt;
            sdkBcdToU16(&usDataLen, &pRecvData[1], 2);
            usDataLen += 5;
            Trace("Mai", "usDataLen = %d, iTmpLen = %d\n", usDataLen, iTmpLen);
            //if((usDataLen <= iTmpLen) || (usDataLen > 3072) || (pRecvData[0] != 0x02))
            if ((usDataLen <= iTmpLen) || (usDataLen > 1088) || (pRecvData[0] != 0x02)) {
                break;
            }



            //if((usDataLen-iTmpLen) < 50)
            //	iTimeOut = 5;
            //else
            //iTimeOut = (usDataLen-iTmpLen);
            iTimeOut = 1000;

            //Trace("Mai", "iTimeOut = %d \n", iTimeOut);

        }

        if (iTmpLen > 0) {
            //TraceHex("zcl","AppCommuRead \r\n",pRecvData,iTmpLen);


            gIsScreenOn = true;
            sdkSysBlackLightOn();//ljh2015-01-07 �յ����ݾ͵�����Ļ ���¼�ʱ
            Trace("Lai", "sdkSysBlackLightOnnnnnnnnnnn 11111\n");
            gScreenTimer = sdkTimerGetId();

            iRecvDataLen = iTmpLen;
            TraceHex("Mai", "===���յ�������=====\r\n", pRecvData, iRecvDataLen);
#if 0
            if(iRecvDataLen > 500)
            {
                sdkmSleep(800);
            }
            else if(iRecvDataLen > 250)
            {
                sdkmSleep(300);
            }
            else if(iRecvDataLen > 100)
            {
                sdkmSleep(100);
            }
#endif
            PostTransFlow(pRecvData, (u16) iRecvDataLen);
        }

        sdkFreeMem(pRecvData);
    }
    else {
        Trace("", "****************** pRecvData== NULL ********** \n");
    }
}

/*****************************************************************************
** Descriptions:	��֯Ӧ���ķ��͸���λ��
** Parameters:    	const u8 *pQueryData����
** Returned value:	
** Created By:		wlz2014.07.08
** Remarks: 		
*************************************************************/
s32 AppCommMakeBagAndSend(ST_TRANSFLOW *Transflow) {
    s32 rslt = 0;
    u32 i = 0, iSendDataLen, uiTimeOut;
    u8 *pSendData = NULL;

    //sdkDispLabel(SDK_DISP_LINE2,"AppCommMakeBagAndSend",SDK_DISP_CDISP|SDK_DISP_FDISP);
    Trace("zcl", " goto AppCommMakeBagAndSend \r\n");
    if (Transflow == NULL) {
        return SDK_ERR;
    }

    pSendData = sdkGetMem(Transflow->usSendDataLen + 20);//������һЩ�ռ��������
    if (pSendData == NULL) {
        Trace("zcl", "�����ڴ�ʧ��: size:%d\r\n", Transflow->usSendDataLen + 20);
        return SDK_ERR;
    }
    Trace("zcl", "�����ڴ�ɹ�: size:%d\r\n", Transflow->usSendDataLen + 20);
    memset(pSendData, 0x00, Transflow->usSendDataLen + 20);


    pSendData[i] = SOP_DATA;
    i++;
    iSendDataLen = Transflow->usSendDataLen + 6; // ��ָ���+ָʾ��+���
    sdkU32ToBcd(&pSendData[i], iSendDataLen, 2);
    i += 2;
    pSendData[i] = Transflow->cmdClass;
    i++;
    pSendData[i] = Transflow->cmdID;
    i++;
    pSendData[i] = Transflow->heIndicatbit;
    i++;
    pSendData[i] = Transflow->ucNum;
    i++;
    memcpy(&pSendData[i], Transflow->asCmdAck, 2);
    i += 2;
    memcpy(&pSendData[i], Transflow->heReceiveData, Transflow->usSendDataLen);    //0416Send==Recv
    i += Transflow->usSendDataLen;
    pSendData[i] = EOP_DATA;
    i++;
    pSendData[i] = GetCRC(&pSendData[1], i);
    i++;

    iSendDataLen = i;


    TraceHex("zcl", "��������:", pSendData, iSendDataLen);
    uiTimeOut = iSendDataLen > 2 ? iSendDataLen / 2 : 1;

    rslt = AppCommSendData(pSendData, iSendDataLen, SENDDATA_TIMEOUT);   //20150402 ԭ����ʱΪ1000
    Trace("zcl", " AppCommSendData rslt %d\r\n", rslt);

    sdkFreeMem(pSendData);
    return rslt;
}


/*****************************************************************************
** Descriptions:	��ȡ���޸���������
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 SetBtName(void) {
    s32 rslt = SDK_OK;
    u8 temp[64 + 1] = {0};
    SDK_BT_REMOTE_INFO stinfo;

    rslt = sdkBtGetStatus();
    if (rslt < 0)return rslt;

    if (rslt > COMMIDLEING)        //��æ
    {
        memset(&stinfo, 0, sizeof(stinfo));
        rslt = sdkBtGetRemoteInfo(&stinfo);     //��ȡԶ���豸
        if (rslt < 0)return rslt;
        rslt = sdkBtDisconnect(stinfo.m_addr);
        if (rslt < 0)return rslt;
    }

    rslt = sdkBtGetName(temp, 65);      //��ȡ��������
    if (rslt < 0)return rslt;

    rslt = InputBtName(temp);      //������������
    if (rslt < 0)return rslt;

    rslt = sdkBtSetName(temp);      //������������
    if (rslt < 0)return rslt;

    return rslt;
}

/*****************************************************************************
** Descriptions:	��ȡ��������������
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 SetBtPwd(void) {
    s32 rslt = SDK_OK;
    u8 pwd[6 + 1] = {0};
    SDK_BT_REMOTE_INFO stinfo;

    rslt = sdkBtGetStatus();
    if (rslt < 0)return rslt;

    if (rslt > COMMIDLEING)        //��æ
    {
        memset(&stinfo, 0, sizeof(stinfo));
        rslt = sdkBtGetRemoteInfo(&stinfo);     //��ȡԶ���豸
        if (rslt < 0)return rslt;
        rslt = sdkBtDisconnect(stinfo.m_addr);      //�ο���������
        if (rslt < 0)return rslt;
    }

    rslt = sdkBtGetPin(pwd, 7);      //��ȡ��������
    if (rslt < 0)return rslt;

    //TraceHex("Lai", "*** sdkBtGetPin pwd", pwd, 7);
    rslt = InputBtPwd(pwd);      //�޸���������
    if (rslt < 0)return rslt;

    //TraceHex("Lai", "*** InputBtPwd pwd", pwd, 7);
    rslt = sdkBtSetPin(pwd);      //������������
    if (rslt < 0) {
        Trace("Lai", "*** sdkBtSetPin error:%d\n", rslt);
        return rslt;
    }
    //Trace("Lai", "*** sdkBtSetPin ok\n");

    return rslt;
}

/*****************************************************************************
** Descriptions:        ���ò���
** Parameters:    	SDK_MSG *lpMsg
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
u8 SysetDevice(SDK_MSG *lpMsg) {
    s32 rslt = SDK_ERR;

    if (BLUETOOTH == gucCurCommMode)        //�ж���Ƶ�豸
    {
        rslt = SetBtName();
        if (rslt == SDK_OK) {
            rslt = SetBtPwd();
            if (rslt == SDK_OK)//ljh
                rslt = sdkBtSetDiscoverable(true);
        }
        if (rslt == SDK_EIO) {
            sdkDispPromptBox(false, TV_AUTORET, "��ȡ�����豸��Ϣʧ��\n��ȷ�������豸�Ƿ���δ����״̬");
            MainFace();
            return SDK_MSG_OUT;
        }
        else if (rslt == SDK_ERR) {
            sdkBtClose();//ljh
            sdkDispPromptBox(false, TV_AUTORET, "��ȡ�����豸��Ϣʧ��\n���������豸�Ƿ�����");
            MainFace();
            return SDK_MSG_OUT;
        }
        else {
            MainFace();
        }
    }
    return SDK_MSG_OUT;
}

