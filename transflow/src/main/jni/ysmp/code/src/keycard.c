#if 1
/*

��Կ����	����У��ֵ	����Ч��	��Կ��¼��ʽ	����	    У��λ
    2�ֽ�	4�ֽ�	                4�ֽ�	            1�ֽ�	                15�ֽ�	    1�ֽ�
��Կ����:16�����ֽ����磺00 FFΪ255��	
����У��ֵ:����2.1	
����Ч��:YYYYMMDDת��BCD��洢	
��Կ��¼��ʽ:0x01-��ͳ��Կ���̻����ն�ASCII�洢��0x55-������Կ��0xFF-��ͳ��Կ���̻����ն�ѹ��BCD�洢	
����:0xFF���	
У��λ:����2.2
*/
struct _CARDHEADER {
    u16 KeyNum;
    u8 chCheckvalue[4];
    u8 chExpireDate[4];
    u8 chKeyType;
    //lilin 2014.11.14 8:56��Կ�洢��ʽ
    u8 chReserved[15];
    u8 chLRCValue;
    //��ȡ��У��ֵ
    u8 CalcLRCValue;//�����У��ֵ
};
static struct _CARDHEADER CardHeader;    //��Ƭ��Ϣ

struct _KEYREC {
    u8 kyMechID[8];  //�̻���
    u8 kyTermID[4];  //�ն˺�
    u8 kyEncryptMKey[16];  //��Կ����
    u8 kyIndex;   //��Կ����
    u8 kyLRCValue;  //У��λ
    u8 kyNum;  //����Կ���ô���
    u8 CalcLRCValue;//�����У��ֵ
};
static struct _KEYREC KeyRec;    //��Կ��Ϣ


/*******************************************************************
��	  ��: wuzhonglin
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��ʾIC������ʧ����Ϣ
��ڲ���: 1:IC����������ֵ,2.��ʾ��Ϣ
�� �� ֵ:
��	  ע:
********************************************************************/
s32 ShowIcCardErr(s32 rslt, u8 *strDlg) {
    u8 strTmp[256];

    memset(strTmp, 0, sizeof(strTmp));
    strcpy(strTmp, "\n");
    strcat(strTmp, strDlg);

    if (rslt == SDK_TIME_OUT) {
        strcat(strTmp, "��ʱ!");
    }
    else {
        strcat(strTmp, "ʧ��!");
    }

    sdkDispClearScreen();
    return sdkDispPromptBox(false, TV_AUTORET, strTmp);
}

/*******************************************************************
��	  ��: wuzhonglin
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ICC����
��ڲ���: void
�� �� ֵ: RSLT_ESC: ��������,�ײ����ʾ,ֱ�ӷ���
		   RSLT_OVERTIME:��λ��ʱ,�Ѿ�����ʾ
��	  ע:
			mode 0: + Le
			mode 1: + Lc + lpIn
			mode 2: + Lc + lpIn + Le
********************************************************************/
u8 SendIcData(const SDK_ICC_PARAM *pstIccParam, u8 mode, u8 CLA, u8 INS, u8 P1, u8 P2, u8 *DataIn,
              u16 Lc, u8 *DataOut, s32 *Le) {

    u8 rlt;
    s32 i;
    u8 lpIn[260];

    i = 0;
    memset(lpIn, 0x00, sizeof(lpIn));
    /*=======BEGIN: huangbx20130809 modify===========*/
    lpIn[i++] = pstIccParam->ucSlotNo;//sujianzhong 2015.01.09 17:31����ucos��sdkû�п�����
    /*==================== END ======================*/
    lpIn[i++] = CLA;
    lpIn[i++] = INS;
    lpIn[i++] = P1;
    lpIn[i++] = P2;

    if (mode == 0) {
        lpIn[i++] = Lc;
//        lpIn[i++] = *Le;
    }
    else if (mode == 1) {
        lpIn[i++] = Lc;
        memcpy(&lpIn[i], DataIn, Lc);
        i += Lc;
    }
    else if (mode == 2) {
        lpIn[i++] = Lc;
        memcpy(&lpIn[i], DataIn, Lc);
        i += Lc;
        lpIn[i++] = *Le;
    }
    else {
        lpIn[i++] = Lc;
        memcpy(&lpIn[i], DataIn, Lc);
        i += Lc;
        lpIn[i++] = *Le;
    }

    TraceHex("SendIcData", "lpIn", lpIn, i);
    rlt = sdkIccSendData(pstIccParam, lpIn, i, DataOut, Le);
    TraceHex("SendIcData", "DataOut", DataOut, *Le);
    if (rlt == SDK_OK) {
        if (DataOut[*Le - 2] != 0x90 || DataOut[*Le - 1] != 0x00) rlt = SDK_ERR;
    }

    return rlt;
}


/*******************************************************************
��	  ��: wuzhonglin
��	  Ȩ: �����¹����ɷ����޹�˾
��������:��������Ϣ��¼��
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
u8 ParseCardHeader(u8 *DataIn) {
    u16 len = 0;

    //BcdToU16(&CardHeader.KeyNum,DataIn,2);//��Կ����
    sdkHexToU16(&CardHeader.KeyNum, DataIn, 2);                //xule120229
    len += 2;
    memcpy(CardHeader.chCheckvalue, DataIn + len, 4); //����У��
    len += 4;
    memcpy(CardHeader.chExpireDate, DataIn + len, 4); //����Ч��
    len += 4;
    /*=======BEGIN: lilin 2014.11.19  16:48 modify===========*/
    CardHeader.chKeyType = DataIn[len++];
    memcpy(CardHeader.chReserved, DataIn + len, 15); //����
    len += 15;
    /*====================== END======================== */
    CardHeader.chLRCValue = *(DataIn + len);
    CardHeader.CalcLRCValue = *DataIn;
    for (len = 1; len < 26; len++) {
        CardHeader.CalcLRCValue ^= *(DataIn + len);
    }
    if (CardHeader.CalcLRCValue == CardHeader.chLRCValue) //У��ֵ���
        return SDK_OK;
    else
        return SDK_ESC;
}

/*******************************************************************
��	  ��: wuzhonglin
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��������Կ����
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
void ParseTmkInfo(u8 *DataIn) {
    u16 len = 0;
    u16 i = 0;

    memset(&KeyRec, 0, sizeof(KeyRec));
    if (CardHeader.chKeyType == 0x01) {
//    memcpy(KeyRec.kyMechID, DataIn, 8); //�̻���
        len += 15;
//    memcpy(KeyRec.kyTermID, DataIn + len, 4); //�ն˺�
        len += 8;
    }
    else {
        len += 8;
        len += 4;
    }
    memcpy(KeyRec.kyEncryptMKey, DataIn + len, 16); //����Կ����
    len += 16;
    KeyRec.kyIndex = *(DataIn + len);
    len++;
    KeyRec.kyLRCValue = *(DataIn + len);
    len++;
    KeyRec.kyNum = *(DataIn + len);
    len++;
    KeyRec.CalcLRCValue = *(DataIn);
    for (i = 1; i < len - 2; i++) {
//        Trace("lilin","%02x\r\n",KeyRec.CalcLRCValue);
        KeyRec.CalcLRCValue ^= *(DataIn + i);
    }
//    Trace("lilin","%02x\r\n",KeyRec.CalcLRCValue);

}

extern s32 Private_sdkSysGetSlotNO(u8 *ucSlotNO);

/*****************************************************************************
** Descriptions:	����IC�����������
** Parameters:    	void
** Returned value:	
** Created By:		sunxiangze 20150108
** Remarks: 		
*****************************************************************************/
s32 KeyCardImportTmkDetails(void) {
    u8 lpOut[512], buf[512], TermNoTemp[9], MerchNoTemp[16], pwd[9], TMK1[16], TMK[17];
    s32 lpLe = 0;
    SDK_ICC_PARAM stIccParam;
    s32 rlt;
    u8 temp[256] = {0};
    u8 out[256] = {0};
    s32 outlen = 0;
    s32 IndexNum = 0;
    s32 key;
    bool bflag = false;
    s32 i;
    stMposTermpara stTermPara;

    memset(&stTermPara, 0x00, sizeof(stTermPara));
    struct _INPUTMAP InputMap = {"\x80\xFF 1cfIC��������Կ\n2lf����Կ������(0-99):", false, 1, 2,
                                 SDK_MMI_NUMBER, SDK_DISP_LINE3};
//lilin 20140611
    struct _INPUTMAP InputMap2 = {"\x80\xFF 1cfIC��������Կ\n2lf����������:", false, 4, 8,
                                  SDK_MMI_NUMBER | SDK_MMI_PWD, SDK_DISP_LINE3};
//add by Sun.xz 20150108
    //struct _INPUTMAP InputMap1 = {"\x80\xFF 1cfIC��������Կ\n2lf����Կ����(0-DES,1-3DES):",  false, 1,  1,  SDK_MMI_NUMBER, SDK_DISP_LINE3};

    SDK_PED_DES_TYPE ekeytype = SDK_PED_DES_TRIPLE;
//��IC���豸
    sdkIccOpenIcDev();
//IC��������ʼ��
    stIccParam.ucCardMode = SDK_ICC_ICC;
    stIccParam.ucSlotNo = sdkIcGetIcdealCardNo();
    stIccParam.eCardType = SDK_ICC_CPU;
    TraceHex("Mai", "������ucSlotNo \r\n", &stIccParam.ucSlotNo, 1);
    memset(temp, 0, sizeof(temp));
    //lilin 20140516
    bflag = InputAny(&InputMap, temp, NULL, NULL);
    if (bflag) {
        IndexNum = atoi(temp);
    }
    else {
        return SDK_ESC;
    }
    if (SDK_OK != ReadPagTLV(MPOS_PARA_FILE, "\x5F\x0D", stTermPara.merchantNo, &rlt)) {
        DispClearContentRam();
        sdkDispPromptBox(false, TV_AUTORET, "\n���̻���\n�������̻���!");
        return SDK_ESC;
    }
    if (SDK_OK != ReadPagTLV(MPOS_PARA_FILE, "\x5F\x0E", stTermPara.terminalNo, &rlt)) {
        DispClearContentRam();
        sdkDispPromptBox(false, TV_AUTORET, "\n���ն˺�\n�������ն˺�!");
        return SDK_ESC;
    }

    sdkDispLabel(SDK_DISP_LINE1, "\n�������Կ��...", SDK_DISP_DEFAULT);

    while (1) {
        rlt = sdkIccIsIccCardIn(100, out, &outlen);
        if (rlt == SDK_OK) {
            break;
        }

        key = sdkKbGetKey();
        if (key != SDK_KEY_ERR) {
            if (key == SDK_KEY_ESC) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                return SDK_ESC;
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }
    sdkmSleep(300);
    Trace("Mai", "KeyCardImportTmkDetails ��λIC��\r\n");
    rlt = sdkIccReset(&stIccParam, lpOut, &lpLe);
    if (rlt != SDK_OK) {
        ShowIcCardErr(rlt, "IC����λ");
        return SDK_ESC;
    }
    TraceHex("Mai", "sdkIccReset", lpOut, lpLe);
    //ѡ����Ŀ¼
    memset(lpOut, 0, sizeof(lpOut));
    rlt = SendIcData(&stIccParam, 1, 0x00, 0xA4, 0x00, 0x00, "\x3F\x00", 2, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "ѡ����Ŀ¼");
        return SDK_ESC;
    }
    //��ȡ����Ϣ��¼��
    memset(lpOut, 0, sizeof(lpOut));
    rlt = SendIcData(&stIccParam, 1, 0x00, 0xA4, 0x00, 0x00, "\x00\x03", 2, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "������Ϣ��¼��");
        return SDK_ESC;
    }
    //	---��ȡ����Ϣ��¼��--------
    memset(&CardHeader, 0, sizeof(CardHeader));
    memset(lpOut, 0, sizeof(lpOut));
    rlt = SendIcData(&stIccParam, 0, 0x00, 0xb0, 0x00, 0x00, 0x00, 0x1f, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "������Ϣ��¼��");
        return SDK_ESC;
    }
//lilin 20140313
//    rlt = ParseCardHeader(&lpOut[1]); //��������Ϣ��¼���ȶ�У��λ
    rlt = ParseCardHeader(lpOut); //��������Ϣ��¼���ȶ�У��λ
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "У��λУ��");
        return SDK_ESC;
    }
    if (CardHeader.chKeyType == 0x55)       //������Կ
    {
        sdkDispClearScreen();
        return sdkDispPromptBox(false, TV_AUTORET, "\n��֤����!\n�ÿ�Ϊ������Կ��!");
    }
    memset(buf, 0, sizeof(buf));
#if 0 /*Modify by lilin at 2014.11.14  9:0 */
    buf[0] = 0x20;
    buf[1] = gstSys.ucYear;
    buf[2] = gstSys.ucMonth;
    buf[3] = gstSys.ucDay;
    
    �����жϿ���Ч�� IC��������Կ�޸�
    if (memcmp(buf, CardHeader.chExpireDate, 4) > 0) //������Ч��
    {
        DispClearScreenRam();
        sdkDispPromptBox(false, TV_AUTORET, "\n��Կ���ѹ���!");
        return SDK_ESC;
    }
#endif /* if 0 */
    //��ȡ��Կ��Ϣ��¼��
    rlt = SendIcData(&stIccParam, 1, 0x00, 0xA4, 0x00, 0x00, "\x00\x02", 2, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "����Կ��Ϣ��¼��");
        return SDK_ESC;
    }

    i = 0;  //��Կ����
    while (i < CardHeader.KeyNum) //�����̻����ն˺Ŷ�Ӧ������Կ
    {
        /*=======BEGIN: lilin 2014.11.20  18:54 modify===========*/
        if (CardHeader.chKeyType == 0x01) {
            rlt = SendIcData(&stIccParam, 0, 0x00, 0xb0, (i * 42) / 256, (i * 42) % 256, 0x00, 42,
                             lpOut, &lpLe);
            if (SDK_OK != rlt) {
                ShowIcCardErr(rlt, "��ȡ����Ϣ��¼��");
                return SDK_ESC;
            }
            memcpy(MerchNoTemp, lpOut, 15);
            memcpy(TermNoTemp, &lpOut[15], 8);
        }
        else {
            rlt = SendIcData(&stIccParam, 0, 0x00, 0xb0, (i * 31) / 256, (i * 31) % 256, 0x00, 31,
                             lpOut, &lpLe);
            if (SDK_OK != rlt) {
                ShowIcCardErr(rlt, "��ȡ����Ϣ��¼��");
                return SDK_ESC;
            }
            memset(buf, 0, sizeof(buf));
            sdkBcdToAsc(buf, lpOut, 8);
            memset(MerchNoTemp, 0, sizeof(MerchNoTemp));
            memcpy(MerchNoTemp, buf, 15);
            memset(TermNoTemp, 0, sizeof(TermNoTemp));
            sdkBcdToAsc(TermNoTemp, &lpOut[8], 4);
        }
        /*====================== END======================== */
        TraceHex("KeyCardImportTmkDetails", "MerchNoTemp", MerchNoTemp, strlen(MerchNoTemp));
        TraceHex("KeyCardImportTmkDetails", "TermNoTemp", TermNoTemp, strlen(TermNoTemp));
        TraceHex("KeyCardImportTmkDetails", "MerchNo", stTermPara.merchantNo, strlen(MerchNoTemp));
        TraceHex("KeyCardImportTmkDetails", "TermNo", stTermPara.terminalNo, strlen(TermNoTemp));
        if (!memcmp(MerchNoTemp, stTermPara.merchantNo, 15) &&
            !memcmp(TermNoTemp, stTermPara.terminalNo, 8)) //�ҵ���Ӧ����Կ������Կ
        {
//            ParseTmkInfo(&lpOut[1]);//����TMK
//lilin 20140313
            ParseTmkInfo(lpOut);//����TMK
            break;
        }
        i++;
    }
    if (i >= CardHeader.KeyNum) //û���ҵ���Ӧ���̻��ź��ն˺�
    {
        sdkDispClearScreen();
        sdkDispPromptBox(false, TV_AUTORET, "δ�ҵ���Ӧ��Կ\n�����̻����ն˺�!");
        return SDK_ESC;
    }
    /*if(!KeyRec.kyNum)//�Ѿ�����������ȷ�ϣ������и�ֵ
    {
    	dev_lcd_clear_screen();
    	dev_lcd_fill_row_ram(LINE2,0,"��Կ���ô���Ϊ0!",FDISP|CDISP|INCOL);
    	dev_lcd_brush_screen();
    	WaitEnterEsc();
    	return FALSE;
    }*/
    if (KeyRec.kyLRCValue != KeyRec.CalcLRCValue) //У��ֵ����ȷ
    {
        ShowIcCardErr(rlt, "У��λУ��");
        return SDK_ESC;
    }

    memset(buf, 0, sizeof(buf));
    memset(pwd, 0, sizeof(pwd));
    bflag = InputAny(&InputMap2, buf, NULL, NULL);
    if (!bflag) {
        return SDK_ESC;
    }

    i = strlen(buf);
    if (i > 8) i = 8;
    memcpy(pwd, &buf[0], i);
    memset(&pwd[i], 0xFF, 8 - i);
    memset(buf, 0, sizeof(buf));
    sdkDesS(true, buf, pwd);

    TraceHex("KeyCardImportTmkDetails", "pwd", pwd, 8);
    TraceHex("KeyCardImportTmkDetails", "buf", buf, 8);
    if (memcmp(buf, CardHeader.chCheckvalue, 4)) {
        sdkDispClearScreen();
        sdkDispPromptBox(false, TV_AUTORET, "\n�������!");
        return SDK_ESC;
    }

    memset(TMK1, 0, sizeof(TMK1));
    memcpy(TMK1, KeyRec.kyEncryptMKey, 16);
    sdkDesS(0, TMK1, pwd);
    sdkDesS(0, &TMK1[8], pwd); //һ�����ܻ�ȡTMK1

    rlt = SendIcData(&stIccParam, 0, 0x80, 0x1A, 0x08, 0x03, 0, 0, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "������Կ����");
        return SDK_ESC;
    }
    rlt = SendIcData(&stIccParam, 1, 0x80, 0xFA, 0x80, 0x00, TMK1, 0x10, lpOut, &lpLe);
    if (SDK_OK != rlt) {
        ShowIcCardErr(rlt, "������Կ����");
        return SDK_ESC;
    }
    memset(TMK, 0, sizeof(TMK));

    TraceHex("Mai", "MerchNo", stTermPara.merchantNo, strlen(MerchNoTemp));
    TraceHex("Mai", "TermNo", stTermPara.terminalNo, strlen(TermNoTemp));
    TraceHex("Mai", "IC������Կ", lpOut, 16);
//    memcpy(TMK, &lpOut[1], 16);
//lilin 20140313
    memcpy(TMK, lpOut, 16);
    //if(Setting.PosParam.DesType == DES_SINGLE) index = KeyRec.kyIndex;  //xule111228
    //else index = KeyRec.kyIndex+MK_3DESTHRESHOLD;
    rlt = sdkPEDUpdateTmk(IndexNum, ekeytype, TMK, TV_AUTORET);
    if (rlt == SDK_OK) {
        sdkDispPromptBox(false, TV_AUTORET, "����Կ���³ɹ�");
    }
    else {
        sdkDispPromptBox(false, TV_AUTORET, "����Կ����ʧ��");
    }

    return rlt;
}

s32 SetKeyCard(void) {
    KeyCardImportTmkDetails();
    MainFace();

    return SDK_OK;
}


#endif
