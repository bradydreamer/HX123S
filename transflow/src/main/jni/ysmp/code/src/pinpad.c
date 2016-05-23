/**************************************Copyright( c ) *****************************************
**                      Shenzhen Xinguodu Technology Co., LTD.
**
**                                 http://www.xinguodu.com
**
**---------File Info--------------------------------------------------------------------------
** Created By:
** Created Date: (YYYY-MM-DD)
** Version :
** Description:
**
**------------------------------------------------------------------------------------------
** Modified by:
** Modified date: (YYYY-MM-DD)
** Version :
** Description:
**
*******************************************************************************************/

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304


/*****************************************************************************
** Descriptions:	��õ�ǰ��Կ����
** Parameters:    	void
** Returned value:

** Created By:		wanggaodeng  2013.03.11
** Remarks:

*****************************************************************************/
u8 GetCurWkIndex(u8 ucIndex) {
    u8 index;

    //   index = sdkSysGetCurFifoNo()*12 + ucIndex;
    index = ucIndex;
    return index;
}

/*****************************************************************************
** Descriptions:	���¹�����Կ
** Parameters:    	void
** Returned value:

** Created By:		wanggaodeng  2013.03.11
** Remarks:

*****************************************************************************/
s32 DecryptWK(ST_KEYDATA *pstEncryptKeyData) {
#if 0
    s32 slt = 0;
    s32 num = 0;
    SDK_PED_WK_CFG stwkcfg;
    SDK_PED_WK_CFG stAllWKCfg[3];

    memset(&stwkcfg, 0, sizeof(stwkcfg));
    memset(stAllWKCfg, 0, sizeof(stAllWKCfg));
    //TPK�������

    stwkcfg.eTmkType = SDK_PED_DES_TRIPLE;
    stwkcfg.eWkType = SDK_PED_DES_TRIPLE;
    stwkcfg.ucTmkIndex = gstAppSetting.uiTMkIndex;
    stwkcfg.ucEnWkLen = 16;
    memcpy(stwkcfg.heEnWk, pstEncryptKeyData->heTpk, 16);                                                                                   //TPK
    stwkcfg.ucCheckDataLen = 16;
    memset(stwkcfg.heCheckData, 0, 16);
    stwkcfg.ucWkIndex = GetCurWkIndex(gstAppSetting.uiTpkIndex);
    memcpy(stwkcfg.heCheckValue, pstEncryptKeyData->heTpkcheckvalue, 8);
    stwkcfg.ucCheckValueLen = 8;
    stwkcfg.bOnlyCalcCheckValue = false;
   	stAllWKCfg[num] = stwkcfg;
   	num++;
	Trace("updateWk","ucTmkIndex %d==PIK WK %d==\r\n",stwkcfg.ucTmkIndex,stwkcfg.ucWkIndex);
	TraceHex("updateWk","PIK WK \r\n",stwkcfg.heEnWk,16);

    //TAK�������
    memset(&stwkcfg, 0, sizeof(stwkcfg));
    stwkcfg.eTmkType = SDK_PED_DES_TRIPLE;
    stwkcfg.eWkType = SDK_PED_DES_TRIPLE;
    stwkcfg.ucTmkIndex = gstAppSetting.uiTMkIndex;
    stwkcfg.ucEnWkLen = 16;
    memcpy(stwkcfg.heEnWk, pstEncryptKeyData->heTak, 16);    //TPK

    stwkcfg.ucCheckDataLen = 16;
    memset(stwkcfg.heCheckData, 0, 16);
    stwkcfg.ucWkIndex = GetCurWkIndex(gstAppSetting.uiTakIndex);

    memcpy(stwkcfg.heCheckValue, pstEncryptKeyData->heTakcheckvalue, 8);
    stwkcfg.ucCheckValueLen = 8;
    stwkcfg.bOnlyCalcCheckValue = false;
    stAllWKCfg[num] = stwkcfg;
    num++;
	Trace("updateWk","ucTmkIndex %d==heTak %d==\r\n",stwkcfg.ucTmkIndex,stwkcfg.ucWkIndex);
	TraceHex("updateWk","heTak Wk \r\n",stwkcfg.heEnWk,16);
    //TdK�������
    if(gstAppSetting.bIsSupportTdk)
    {
        memset(&stwkcfg, 0, sizeof(stwkcfg));
        stwkcfg.eTmkType = SDK_PED_DES_TRIPLE;
        stwkcfg.eWkType = SDK_PED_DES_TRIPLE;
        stwkcfg.ucTmkIndex = gstAppSetting.uiTMkIndex;
        stwkcfg.ucEnWkLen = 16;
        memcpy(stwkcfg.heEnWk, pstEncryptKeyData->heTdk, 16);                                                                                  //TPK
        stwkcfg.ucCheckDataLen = 16;
        memset(stwkcfg.heCheckData, 0, 16);
        stwkcfg.ucWkIndex = GetCurWkIndex(gstAppSetting.uiTdkIndex);
        memcpy(stwkcfg.heCheckValue, pstEncryptKeyData->heTdkcheckvalue, 8);
        stwkcfg.ucCheckValueLen = 8;
        stwkcfg.bOnlyCalcCheckValue = false;
        stAllWKCfg[num] = stwkcfg;
        num++;
        Trace("updateWk","ucTmkIndex %d==heTdk WkIndex%d==\r\n",stwkcfg.ucTmkIndex,stwkcfg.ucWkIndex);
        TraceHex("updateWk","heTdk Wk \r\n",stwkcfg.heEnWk,16);
    }
	
	Trace("updateWk","sdkPEDUpdateWk num %d====\r\n",num); 
	Trace("updateWk","PIKINdex  %d====\r\n",stAllWKCfg[0].ucWkIndex); 
	TraceHex("updateWk","PIK.heEnWk \r\n",stAllWKCfg[0].heEnWk,16);

	slt = sdkPEDUpdateWk(stAllWKCfg, num, num*SDK_PED_TIMEOUT);
    Trace("updateWk","sdkPEDUpdateWk slt=[%d]\r\n", slt);
    if(slt != SDK_OK)
    {
        slt = SDK_ERR;
        return slt;
    }
    return slt;
#endif
    return SDK_OK;
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 PinPad_EncryptPwd(u8 *lpOut, u8 *lpIn, u16 nLen) {
#if 0
    u8 Index = 0;
    Index = GetCurWkIndex(gstAppSetting.uiTpkIndex);
    if(gstAppSetting.bIsMk3Des)
    {
        return  sdkPEDWkDes(Index, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT , lpIn, lpOut);
    }
    else
    {
        return sdkPEDWkDes(Index, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT , lpIn, lpOut);
    }
#endif
    return SDK_OK;
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 PinPad_EncryptMac(u8 *lpOut, u8 *lpIn, u16 nLen) {
#if 0
    u8 Index = 0;
    Index = GetCurWkIndex(gstAppSetting.uiTakIndex);

    if(gstAppSetting.bIsMk3Des)
    {
        return	sdkPEDWkDes(Index, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT , lpIn, lpOut);
    }
    else
    {
        return	sdkPEDWkDes(Index, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT , lpIn, lpOut);
    }
#endif
    return SDK_OK;
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 PinPad_EncryptData(u8 *lpOut, u8 *lpIn, u16 nLen) {
#if 0
    s32 iWkIndex;

    iWkIndex = GetCurWkIndex(gstAppSetting.uiTdkIndex);

    if(gstAppSetting.bIsMk3Des)
    {
        return	sdkPEDWkDes(iWkIndex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT , lpIn, lpOut);

    }
    else
    {
        return	sdkPEDWkDes(iWkIndex, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT , lpIn, lpOut);
    }
#endif
    return SDK_OK;

}

/********************************************************************************************
**��������:   u8 PinPad_WritePinPadNo(u8 *lpOut, u16 *nLen)
**��������:  д�����к�
**�������:   1:���кŴ洢λ�� 2:���ݳ���һ��Ϊ16
**�������:
**
**����     ֵ:   ture �ɹ�  false ʧ��
*******************************************************************************************/
s32 WriteSerialNo(u8 *lpIn, u16 nLen) {

#if(KEYPAD_LOCATION == ON)
    //   memcpy(gstAppSetting.asPinPadNo,lpIn,nLen);       wlz modifi
    return SDK_OK;
#else
    s32 Rslt;
    Rslt = sdkPEDSaveUserData(lpIn,nLen);
    if(Rslt == SDK_OK)
    {return SDK_OK;}
    else
    {return SDK_ESC;}
#endif
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 ReadSerialNo(u8 *lpOut, u16 *nLen) {

#if(KEYPAD_LOCATION == ON)
//    memcpy(lpOut,gstAppSetting.asPinPadNo,16);        wlz modifi
    *nLen = 16;
    return SDK_OK;
#else
    s32 Rslt;
    Rslt = sdkPEDReadUserData(lpOut,(s32)*nLen);
    if(Rslt > 0)
        {return SDK_OK;}
    else
        {return SDK_ESC;}

#endif
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 EncryptPwd(u8 *lpOut, u16 *outLen, u8 *lpIn, u16 nLen) {
    return PinPad_EncryptPwd(lpOut, lpIn, nLen);
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 EncryptMac(u8 *lpOut, u8 *lpIn, u16 nLen) {
#if 0
    u8 iKeyIndex = 0;
    s32 Ret;
    u16 templen=0;

//	lpIn[nLen]=0x80;
//    templen=nLen+1;
     templen=nLen;
    iKeyIndex = GetCurWkIndex(gstAppSetting.uiTakIndex);
    Ret = sdkPEDCalcMac(iKeyIndex, SDK_PED_X919, SDK_PED_DES_TRIPLE, lpIn, templen, lpOut);
    Trace("wlz","sdkPEDCalcMac %d \r\n",Ret);
    return Ret;
#endif
    return SDK_OK;
}

/*******************************************************************
��    ��: ����ҫ
��    Ȩ: �¹���
��������:u8 EncryptData(u8 *lpOut,u8 *lpIn,u16 nLen)
��������: ��TPK������Ϣ��������
��ڲ���:
�� �� ֵ:
��ص���:
��    ע: ��Ӧ�������
�޸���Ϣ:
********************************************************************/
s32 EncryptData(u8 *lpOut, const u8 *lpIn, u16 nLen) {
    u8 temp[9], A0[9];
    u16 i, j, len = 0;
    s32 iRslt;
    memset(A0, 0, sizeof(A0));
    for (i = 0; i < ((nLen + 7) / 8); i++) {
        memset(temp, 0, sizeof(temp));
        if (((i + 1) * 8) > nLen) {
            j = nLen - ((nLen / 8) * 8);
        }
        else { j = 8; }
        memcpy(temp, &lpIn[i * 8], j);

        TraceHex("EncryptData", "==PinPad_EncryptData=����ǰ����==\r\n", temp, 8);
        iRslt = PinPad_EncryptData(A0, temp, 8);
        if (iRslt != SDK_OK) { return SDK_ESC; }
        memcpy(&lpOut[len], A0, 8);
        len += 8;
        TraceHex("EncryptData", "==PinPad_EncryptData=���ܺ�����===\r\n", A0, 8);
    }
    return SDK_OK;
}

/********************************************************************************************
**��������:
**��������: У����ܰ�
**�������:
**�������:
**
**����     ֵ:
*******************************************************************************************/
s32 CheckMac(u8 *lpIn, u16 nLen, u8 *Mac) {
    u8 MacTemp[16];
    s32 result = 0;
    //MACΪ0  ����ҪУ��
    if (memcmp(Mac, "\x00\x00\x00\x00\x00\x00\x00\x00", 8) == 0) { return SDK_OK; }

    //��ҪУ��MAC
    memset(MacTemp, 0, sizeof(MacTemp));
    result = EncryptMac(MacTemp, lpIn, nLen);

    TraceHex("wlz", "MacTemp", MacTemp, 8);
    TraceHex("wlz", "Mac", Mac, 8);

    if ((result == SDK_OK) && (memcmp(MacTemp, Mac, 4) == 0)) { return SDK_OK; }
    else { return SDK_ESC; }

}

/*******************************************************************
��    ��: ������
��    Ȩ: Okolle Ltd,Co.
��������: void GetMacSource(u8 *MSource,u8 *Buf,u16 lLength)
��������: Macԭ�������
��ڲ���: 1,MacSouce��ŵĵ�ַ;2.��Mac����ָ��;3.��Mac���ݳ���
�� �� ֵ: ��
��ص���:
��    ע:
�޸���Ϣ:
********************************************************************/
u16 MacDataFormat(u8 *lpOut, u8 *lpIn, u16 nLen) {
#if 1
    u8 MACSrc[1024], A1[8], Source[8];
    u16 i;

    memset(MACSrc, 0, sizeof(MACSrc));                        //������������
    memcpy(MACSrc, lpIn, nLen);
    if ((nLen % 8) != 0) {
        for (i = 0; i < (8 - nLen % 8); i++) {
            MACSrc[nLen + i] = 0;                        //��ԭʼ���ݱ��8����������������
        }
        nLen += i;
    }

    memcpy(Source, MACSrc, 8);                    //ȡ�ĵ�һ��8λ����
    for (i = 1; i < (nLen / 8); i++)    //��ԭʼ���ݰ�ÿ8���ֽ������
    {
        sdkXOR8(A1, Source, &MACSrc[i * 8]);
        memcpy(Source, A1, 8);
    }
    memcpy(lpOut, Source, 8);               //���м�����ת����Ascii
    return 8;
#else
    u8 i;

    memset(MSource,0,8);
    memcpy(&MSource[8],Buf,lLength);
    lLength+=8;
    memcpy(&MSource[lLength++],"\x80",1);
    if ((lLength % 8) != 0)
    {
        for (i = 0; i < (8 - lLength % 8); i++)
        {
            MSource[lLength + i] = 0;
        }
        lLength += i;
    }
    return lLength;
#endif
}


/*******************************************************************
��	  ��: ���ߵ�
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����PC����
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
u8 SendDataToPcsoft(u8 *lpin, u16 len) {
    return SDK_OK;

/*
	SendPinPad(FIFO_SENDCOMTRANSP, lpin, len);
	return 1;
*/
}

/*******************************************************************
��	  ��: ���ߵ�
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����PC����
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
void SendComData_dll(u8 cmd, u8 *Dest, u16 len) {
    u8 TxBuf_Pad[50];
    u16 TxLen_Pad;
    u8 i;

    i = 0;

    memset(TxBuf_Pad, 0x01, 5);
    i += 5;
    TxBuf_Pad[i++] = 0x10;
    TxBuf_Pad[i++] = cmd;
    TxBuf_Pad[i++] = len;
    memcpy(&TxBuf_Pad[i], Dest, len);
    i += len;
    TxBuf_Pad[i++] = 0x03;
    TxBuf_Pad[i] = CalcBcc((char *) &TxBuf_Pad[3], i - 3);
    i++;
    TxLen_Pad = i;
//	SendDataToPcsoft(TxBuf_Pad, TxLen_Pad);
    sdkCommUartSendData(sdkSysGetComNo(SDK_SYS_COM_PIN_PAD), TxBuf_Pad, TxLen_Pad);

}

/********************************************************************
 ��    ��: ��¼ׯ
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������:��PAN ��PIN����
 �������: 	lpPan   ���˺�(PAN),
 				����Ľ��,
 				Row 		��ʾ���к�
 �������:	��������
 �� �� ֵ
				 SDK_OK  �ɹ�
				 SDK_PARA_ERR	 ��������
				 SDK_PED_PIN_FORMAT_ERR  �����ʽ����
				 SDK_ESC �û�ȡ������
				 SDK_TIME_OUT	 ���������Ӧ��ʱ(��λ: ms)
 ��    ע:
 �޸���Ϣ:
********************************************************************/
s32 E98(u8 *lpOut, u8 *lpPan, const u8 *pbcAmt, u8 Row) {
#if 0
    SDK_PED_PIN_CFG pstPinCfg;
    u8 tpksno;

    tpksno = GetCurWkIndex(gstAppSetting.uiTpkIndex);

    memset(&pstPinCfg, 0 , sizeof(pstPinCfg));

    pstPinCfg. ePinMode= SDK_PED_MAG_PIN;
    if(gstAppSetting.bIsMk3Des)
    {
        pstPinCfg. eKeyType = SDK_PED_DES_TRIPLE;
    }
    else
    {
        pstPinCfg. eKeyType = SDK_PED_DES_SINGLE;
    }

    pstPinCfg. ucTpkIndex = tpksno;
    memcpy(pstPinCfg. hePan, lpPan, 8);
    pstPinCfg. ucRow = Row;
    pstPinCfg. iTimeOut = 60*1000;//60s

    return sdkPEDInputPIN(&pstPinCfg, pbcAmt,lpOut);
#endif
    return SDK_OK;
}

/**************************************Function Description ****************************
** ��������:void InitSys(void)
** ����:��ʼ��ϵͳ
** �������:N/A
** �������:N/A
** ����ֵ:N/A
** ����: sxl
** ��������: 2009-05-13
** ����: ��ʼ��Ӳ�����ļ�ϵͳ
**
*******************************************************************************************/
void ReadPinPadInfo(void) {
    u8 temp[64];
    s32 iOutLen = 16;
    s32 rslt = TRUE;

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���ڼ���������", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���Ժ�...", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    memset(temp, 0x00, sizeof(temp));
//    rslt = IFPedNeedUpadte();         wlz modifi
#if(KEYPAD_LOCATION == ON)
    if (rslt > 0) {
        return;
    }
#else
    if(rslt>0 && sdkPEDReadUserData(temp, iOutLen))
    {      
#if(KEYPAD_LOCATION==OFF)//wwzj
        memcpy(gstAppSetting.asPinPadNo,temp,16);
        SaveSetting();
#endif
        return;
    }
#endif
}

/**************************************Function Description ****************************
** ��������:void ReadPedParam(void)
** ����:��ȡ�������ͨѶ��������Ҫ������ͨѶ�ӿ�ǰ����
** �������:N/A
** �������:N/A
** ����ֵ:N/A
** ����: sxl
**
*******************************************************************************************/
void ReadPedParam(void) {
#if 0
    u8 temp[120],buf[30];
    s32 iOutLen = 36; 
    s32 ret;
    if((gstAppSetting.BIsPedParam == 1) &&(gstAppComSetting.bisPinPad==1))
    {
        memset(temp,0x00,sizeof(temp));
        memset(buf,0x00,sizeof(buf));
        ret = sdkPEDReadUserData(temp, iOutLen);
        Trace("wlz","sdkPEDReadUserData %d iOutLen%d\r\n",ret,iOutLen);
        if((ret>0) && (temp[16] == 0xFF))
        {
            iOutLen = 120;
            memset(temp,0x00,sizeof(temp));
            ret = sdkPEDReadUserData(temp, iOutLen);
        }
	    if(ret>0)
	    { 
            TraceHex("wlz","sdkPEDReadUserData\r\n",temp,iOutLen);
            if(memcmp(buf,temp,16) != 0)
            {memcpy(gstAppSetting.asPinPadNo,temp,16);}
            if(memcmp(buf,&temp[16],20) != 0)
            {memcpy(gstAppSetting.stCommuParam.asPhone[0],&temp[16],27);}
            if(memcmp(buf,&temp[56],20) != 0)
            {memcpy(gstAppSetting.stCommuParam.asPhone[1],&temp[56],27);}
			else
			{memcpy(gstAppSetting.stCommuParam.asPhone[1],gstAppSetting.stCommuParam.asPhone[0],27);}
            if(memcmp(buf,&temp[96],5) != 0)
            {sdkBcdToAsc(gstAppSetting.stCommuParam.asTpdu,&temp[96],5);}
            SaveSetting();
        }    
		else if((ret != -101) && (gstAppSetting.BIsPedParam == 1))
		{
			sdkDispClearScreen();
		    DispManyLines("�������û��¼��ͨѶ����,�����·ַ��������!", SDK_DISP_LINE1, SDK_DISP_LEFT_DEFAULT);
		    DispManyLines("�����������...", SDK_DISP_LINE5, SDK_DISP_RIGHT_DEFAULT);
		    sdkKbWaitKey(SDK_KEY_MASK_ALL,GetTimerAutoRetValue());
		}
    }
#endif
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304
