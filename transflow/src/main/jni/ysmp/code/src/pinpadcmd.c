/*
        �����ĵ�: DDIӦ�ÿ���ָ��V1.07.pdf

        TMK		������: 0~101,  ˫����: 102~203

        WK		������: 0~127,   ˫����: 128~255

 */

/*
        ����TLV  �ṹ,  �����˽ṹ���ṹ��:  tlvStruct ������
 */

typedef struct TLV_STRUCT {
    u8 tag[4];
    u32 tagLen;
    u16 tagOffset;
    u16 tagMax;
} *tlvStruct;


#define WK_TRIPLE_DES_INDEX 128
#define TMK_TRIPLE_DES_INDEX 102


/*
        ע:  �ֽ׶����������̲���֧��,  ���ڽ��������ͬ����������
        Ψһ��������������������Ҫ��ʼ����ԿоƬ, �����ͳһ����
        ��������̺�����������.

 */

cmd_Str const PinpadCmdTab[] = {
        {PASSWD_INPUT_ENUM,       OnPasswdInput,     NULL},
        {LOAD_TMK_ENUM,           OnLoadTMK,         NULL},
        {DATA_ENCRYPT_ENUM,       OnDataEncrypt,     NULL},
        {MAC_CALC_ENUM,           OnCalcMAC,         NULL},
        {LOAD_WK_ENUM,            OnLoadWK,          NULL},
        {MAC_CALC_INNERINFO_ENUM, OnCalcMACStoreTag, NULL},
        {SETTING_PASSWD_ENUM,     OnSetPinpadOuter,  NULL},
        {PASSWD_INPUT_OUTER,      OnPasswdInput,     NULL},
        {DATA_ENCRYPT_OUTER,      OnDataEncrypt,     NULL},
        {MAC_CALC_OUTER,          OnCalcMAC,         NULL},
        {LOAD_WK_OUTER,           OnLoadWK,          NULL},
        {LOAD_DUKPT_OUTER,        OnLoadDUKPT,       NULL},
        {PINDPAD_END,             NULL,              NULL},
};


s32 loadDefaultTMK(void) {
    //Xiaox 15-1-8 for test
//	s32 ret = 0;
//
//	u8 TMKData[] = { 0xCF, 0xCC, 0xF0, 0xE8, 0xDB, 0x6C, 0x76, 0x37, \
// //						0x23, 0x61, 0x7A, 0xA3, 0x59, 0x46, 0x4F, 0x98 };
//
//	ret = sdkPEDUpdateTmk (99, SDK_PED_DES_TRIPLE, TMKData, 200);
//	if (ret != SDK_OK)
//		Trace("debug", "loadTMK: %d", ret);
//
//	return ret;

    s32 ret = 0;

    u8 TMKData[] = {0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32,
                    0x32, 0x32, 0x32};

    ret = sdkPEDUpdateTmk(00, SDK_PED_DES_TRIPLE, TMKData, 200);

    if (ret != SDK_OK) {
        Trace("debug", "loadTMK: %d", ret);
    }
    return ret;
}

/*
   �����õ���Կ
   TMK��
   CFCCF0E8DB6C763723617AA359464F98

   PIK��
   plaintext
   D48E289E4A80DF5CBC53207F52D321BA
   cipher
   733ADE6E6420CDC72BA8D075302F2731

   TDK��
   plaintext
   68AE72C957D5167DDF59FBA6BF4B8484
   cipher
   893F6F3953A522A91FA7E7A0E46F173F

   MAK��
   plaintext
   182B020EBD3D8E949686955ACEFEBEAE
   cipher
   7E2179708E1FE009403D487B98760BEC
 */
s32 loadDefaultWK(void) {
#if 0 //del by sujianzhong 2015.01.14
    s32 ret = 0;
    SDK_PED_WK_CFG pedWK1;
    SDK_PED_WK_CFG pedWK2;
    SDK_PED_WK_CFG pedWK3;

    memset(&pedWK1, 0, sizeof(pedWK1));

    ret = loadDefaultTMK();

    pedWK1.eTmkType = SDK_PED_DES_TRIPLE;
    pedWK1.eWkType = SDK_PED_DES_TRIPLE;
    pedWK1.ucWkIndex = 1;
    pedWK1.ucTmkIndex = 99;
    pedWK1.ucEnWkLen = 16;
    pedWK1.ucCheckDataLen = 0;
    pedWK1.ucCheckValueLen = 0;

    memcpy(&pedWK2, &pedWK1, sizeof(pedWK1));
    memcpy(&pedWK3, &pedWK1, sizeof(pedWK1));

    pedWK2.ucWkIndex = 2;
    pedWK3.ucWkIndex = 3;
    sdkAscToBcd(&pedWK1.heEnWk, "733ADE6E6420CDC72BA8D075302F2731", 32);
    sdkAscToBcd(&pedWK2.heEnWk, "893F6F3953A522A91FA7E7A0E46F173F", 32);
    sdkAscToBcd(&pedWK3.heEnWk, "7E2179708E1FE009403D487B98760BEC", 32);

    //PIK
    ret = sdkPEDUpdateWkEx(&pedWK1, SDK_PED_DES_ALL);

    if (ret != SDK_OK)
    {
        Trace("Mai", "$$$$ Update PIK err! ret:%d\r\n", ret);
        return INS_OTHER_ERR;
    }
    //TDK
    ret = sdkPEDUpdateWkEx(&pedWK2, SDK_PED_DES_ALL);

    if (ret != SDK_OK)
    {
        Trace("Mai", "$$$$ Update TDK err! ret:%d\r\n", ret);
        return INS_OTHER_ERR;
    }
    //MAK
    ret = sdkPEDUpdateWkEx(&pedWK3, SDK_PED_DES_ALL);

    if (ret != SDK_OK)
    {
        Trace("Mai", "$$$$ Update MAK err! ret:%d\r\n", ret);
        return INS_OTHER_ERR;
    }
    Trace("Mai", "Update wk OK!\r\n");
    return ret;

#endif
}

/*****************************************************************************
** Descriptions:		���������Կ
** Parameters:
** Returned value:
** Created By:		Maihaoqing  2014.12.30
** Remarks:
*****************************************************************************/
void LoadTestKey() {
    u8 ucMk[16] = {0};
    u8 ucAuthKey[16] = {0};
    u8 ucAppKey[16] = {0};
    SDK_PED_WK_CFG AuthKey;
    SDK_PED_WK_CFG AppKey;
    SDK_PED_DES_TYPE eKeyType = SDK_PED_DES_TRIPLE;

    memset(&AuthKey, 0, sizeof(AuthKey));
    memset(&AppKey, 0, sizeof(AppKey));

    sdkAscToBcd(ucMk, "F4E880F2427BDE8AF425015B5F96F84C", 32);
    sdkAscToBcd(ucAuthKey, "3B265A9FCB4607183FF880E810DFD48D", 32);             //����
    sdkAscToBcd(ucAppKey, "F4EFA2C7F930BFD42396E1BCE70DA71C", 32);              //����

    sdkPEDUpdateTmk(TEST_MK_INDEX, eKeyType, ucMk, 1000);

    sdkAscToBcd(AuthKey.heEnWk, "DD1F432490EA4FB8F08F81EF343CE0F8", 32);
    sdkAscToBcd(AppKey.heEnWk, "F2B9D0C99E07344FA1E3CE72536628ED", 32);

    AuthKey.ucEnWkLen = AppKey.ucEnWkLen = 16;
    AuthKey.ucTmkIndex = AppKey.ucTmkIndex = TEST_MK_INDEX;
    AuthKey.ucWkIndex = TEST_AUTH_KEY_INDEX;
    AppKey.ucWkIndex = TEST_APP_KEY_INDEX;

    sdkPEDUpdateWkEx(&AuthKey, SDK_PED_DES_ALL);
    sdkPEDUpdateWkEx(&AppKey, SDK_PED_DES_ALL);
}

/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��������
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
static s32 KbInputPin(const s32 siOvertime, u8 *pheOut, u8 ucPinMaxLen, const s32 siDispRow) {
    s32 i = strlen(pheOut);
    s32 key;
    s32 Overtime = siOvertime;
    u32 uiTimerId;
    u8 asDispBuf[21] = {0};

    if (pheOut == NULL) {
        return SDK_PARA_ERR;
    }

    if (0 == i) {
        strcpy(asDispBuf, "_");
        sdkDispClearRow(siDispRow);
        sdkDispFillRowRam(siDispRow, 0, asDispBuf,
                          SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
        sdkDispBrushScreen();
    }
    uiTimerId = sdkTimerGetId();

    while (i < 13) {
        //20150320 ��Ӹ�λ�����⸴λ����
        if (GetCmdResetExe() == TRUE || !JudgeTradeFlow()) {
            bisNeedBrushScreenWhenBack = true;
            return SDK_TIME_OUT;
        }
        if (sdkTimerIsEnd(uiTimerId, Overtime)) {
            return SDK_TIME_OUT;
        }
        key = sdkKbGetKey();

        if (key != SDK_KEY_ERR) {
            Overtime += 30;                             //�����°������ͽ��ŵȴ�
            sdkSysBeep(SDK_SYS_BEEP_OK);

            switch (key) {
                case SDK_KEY_0:
                case SDK_KEY_1:
                case SDK_KEY_2:
                case SDK_KEY_3:
                case SDK_KEY_4:
                case SDK_KEY_5:
                case SDK_KEY_6:
                case SDK_KEY_7:
                case SDK_KEY_8:
                case SDK_KEY_9: {
                    if (i < ucPinMaxLen) {
                        pheOut[i++] = (key & 0xFF);
                    }
                }
                    break;

                case SDK_KEY_ENTER: {
                    if ((i < 4) && (i != 0)) {
                        break;
                    }
                    else {
                        return SDK_KEY_ENTER;
                    }
                }

                case SDK_KEY_BACKSPACE: {
                    i = i == 0 ? 0 : i - 1;
                    pheOut[i] = 0x00;
                }
                    break;

                case SDK_KEY_ESC: {
                    memset(pheOut, 0x00, 12);
                    return SDK_KEY_ESC;
                }

                default: {
                }
                    break;
            }

            Trace("Mai", " pheOut = %s\r\n", pheOut);

            if (0 == i) {
                strcpy(asDispBuf, "_");                         //����ʲô������
            }
            else {
                memset(asDispBuf, 0x00, sizeof(asDispBuf));
                memset(asDispBuf, '*', strlen(pheOut));
            }
            sdkDispClearRow(siDispRow);
            sdkDispFillRowRam(siDispRow, 0, asDispBuf,
                              SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
            sdkDispBrushScreen();
        }
    }

    return SDK_OK;
}

static s32 initInputCFGInfo(struct_passwdInput *pPwdInput, u8 *inputBuf) {
    s32 count = 0;

    pPwdInput->cryptIndex = inputBuf[count++];
    pPwdInput->cryptType = inputBuf[count++];
    pPwdInput->mainAccountFlag = inputBuf[count++];
    pPwdInput->pMainAccount = &inputBuf[count];
    count += 20;
    sdkBcdToU16(&pPwdInput->cryptLen, &inputBuf[count], 2);
    count += 2;
    pPwdInput->pCryptData = &inputBuf[count];
    count += pPwdInput->cryptLen;
    pPwdInput->passwdLen = inputBuf[count++];
    pPwdInput->pCryptExtra = &inputBuf[count];
    count += 10;
    pPwdInput->enterClick = inputBuf[count++];
    pPwdInput->enterExpire = inputBuf[count++];
    sdkBcdToU16(&pPwdInput->displayDataLen, &inputBuf[count], 2);
    count += 2;
    pPwdInput->pDisplayData = &inputBuf[count];

}

/*****************************************************************************
** Descriptions:		��������
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

*****************************************************************************/

/*
        ID		������				����			��ע

        1		��Կ����			  1				����Կ����/ ������Կ����, ��Ϊ����Կ
                                                                                                        ������ID6 ��ָ��������Կ, ��Ϊ������Կ
                                                                                                        ������ID6 Ϊ0 �������ڹ�����Կ.

        2		��Կ����                          1				0x0:	MK/SK Ĭ��
                                                                                                        0x1:	DUKPT
                                                                                                        0x2:	FIXED (����һ�б����ø÷�ʽ)

        3		���˺�ָʾλ	  1				0:	���˺�, ָʾID4  Ϊ���˺�
                                                                                                        1:	��ϣֵ, ָʾID4  ����������˺ŵĹ�
                                                                                                        ϣֵ, ���ñ�ָ��֮ǰ�����һ��ˢ����
                                                                                                        ���˺ŵĹ�ϣֵ, MPOS ����Ҫ�������һ��ˢ
                                                                                                        �������˺Ź���ָ��ʹ��.

        4		���˺�/ ���˺Ź�ϣֵ	         20				���˺Ų���λ��F

        5		��Կ		                        LL...24                         PIN ������Կ(����)

        6		������Կ��󳤶�				LL...24			��󳤶�ֵ Ϊ0xC

        7		���ܸ�������	10				����PIN �����㷨�к��������

        8		�Ƿ����ûس���1				0:	������
                                                                                                        1:	����

        9		������ʱ			1				����ʱ, �������Ӧ����, ��ص�������
                                                                                                        ��, ���˳�.

        10		��ʾ����			LL...64			ָ������.

 */
//extern u8 gPlatformSerialNO[12];

//extern u8 gPlatformRandomNO[6];

s32 OnPasswdInput(ST_TRANSFLOW *pstTransFlow) {
    u8 sendCount = 0;
    u16 databuflen = 0;
    u8 passwdbuf[17] = {0};
    s32 i = 0, ret = 0;
    //u8 asPan[8] = {0}, pin[8] = {0};
    //u8 key[16] = {0};
    u8 fensan[16];
    SDK_PED_DES_TYPE tmkDesType, wkDesType;
    struct_passwdInput pwdInput;
    SDK_PED_PIN_CFG pincfg;
    static u8 inpin[48];      //0416Send==Recv


    // ����������
    if (pstTransFlow->cmdID == PASSWD_INPUT_OUTER) {
        ret = sdkPEDInitKeyIC();

        if (ret != SDK_OK) {
            memcpy(&inpin[1], "02", 2);
            FinishTradeFlow();
            bisNeedBrushScreenWhenBack = true;
            return INS_OTHER_ERR;
        }
    }

    initInputCFGInfo(&pwdInput, pstTransFlow->heReceiveData);

    if (pwdInput.cryptLen) {
        if (pwdInput.cryptLen == 8) {
            tmkDesType = SDK_PED_DES_SINGLE;
        }
        else if (pwdInput.cryptLen == 16 || pwdInput.cryptLen == 24) {
            tmkDesType = SDK_PED_DES_TRIPLE;
        }
        else {
            FinishTradeFlow();
            bisNeedBrushScreenWhenBack = true;
            return INS_OTHER_ERR;
        }
    }
    else {
        //		if (pwdInput.cryptIndex < WK_TRIPLE_DES_INDEX)//sujianzhong 2015.01.12 13:1
        //			wkDesType = SDK_PED_DES_SINGLE;
        //		else
        wkDesType = SDK_PED_DES_TRIPLE;
    }


    if (pstTransFlow->ucInsRunStep == 0) {
        memset(inpin, 0, sizeof(inpin));            //0416Send==Recv
        //memcpy(displayData, pwdInput.pDisplayData, pwdInput.displayDataLen);
        pwdInput.pDisplayData[pwdInput.displayDataLen] = '\0';

        Trace("Mai", "disp = %s \r\n", pwdInput.pDisplayData);
        sdkDispLabel(SDK_DISP_LINE2, pwdInput.pDisplayData, SDK_DISP_LDISP | SDK_DISP_FDISP);
        pstTransFlow->ucInsRunStep = 1;
        pstTransFlow->InsClockTimer = sdkTimerGetId();
        GotoTradeFlow();
    }
    //	if (!pwdInput.cryptLen)//sujianzhong 2015.01.12 12:57
    {
        pincfg.ucTpkIndex = pwdInput.cryptIndex;
        pincfg.eKeyType = wkDesType;
    }

    pincfg.ucRow = 4;
    pincfg.iTimeOut = pwdInput.enterExpire * 1000;

/*
        ƽ̨��ˮ��12  �ֽ�(asc) + �����6  �ֽ�+  SN  ��6  �ֽ� +  8  �ֽڼ���

        PINBLOCK

        �·���ƽ̨��ˮ��12  �ֽ�, ����ǰ6  �ֽ�ת��12  �ֽڵ�asc  ��, ��6
        �ֽ�ת�������

        PIN ���ܷ���:
        ����: 123456
        ��PINBLOCK: 0x06 0x12 0x34 0x56 0xFF 0xFF
 */    /*
      ��sdkPEDInputPIN �����еײ㲻�ܴ���ʹ����ʱ��Կ����������, �ʽ�
      ������PIN ��ʽ��ΪIC ���ѻ���ʽ, ����ȡ���û�����������.
 */

    //	pincfg.ePinMode = GetCurrentCardType();

    Trace("Mai", "cryptType = %02x \r\n", pwdInput.cryptType);
    if (pwdInput.cryptType == 0x00 || pwdInput.cryptType == 0x02)    //sujianzhong 2015.01.10 11:26
    {
        pincfg.ePinMode = SDK_PED_IC_OFFLINE_PIN;
    }
    else {
        pincfg.ePinMode = SDK_PED_IC_ONLINE_PIN;
#if 0
        memset(tmp, 0, sizeof(tmp));
        AppGetMagCardNo(&pstCardInfo.stCardData, tmp);

        if(strlen(tmp) < 13)
        {
            goto ERR_RETURN;
        }
        TraceHex("zcl", "pan:", tmp, sizeof(tmp));
        sdkAscToBcd(pincfg.hePan + 2, tmp + strlen(tmp) - 13, 12);
#endif
        FinishTradeFlow();
        bisNeedBrushScreenWhenBack = true;
        return INS_OTHER_ERR;          //zcl ����ģʽδʵ�֣��ȱ���
    }

    //Mai add 20150115 ,֧��������ʱ������Ӧ����ָ��
    if (!JudgeTradeFlow()) {
        Trace("Mai", "����\r\n");
        FinishTradeFlow();
        bisNeedBrushScreenWhenBack = true;
        return INS_NOT_RETURN;
    }
    else if (sdkTimerIsEnd(pstTransFlow->InsClockTimer, pincfg.iTimeOut)) {
        Trace("Mai", "����\r\n");
        FinishTradeFlow();
        gucPBOCTransFlg = SDK_TIME_OUT;
        Trace("Mai", "OnPasswdInput: �������볬ʱ\r\n");
        //DispTradeEnd(SDK_ESC);              //20150401
        return INS_OVERTIME;                //20150323 ���PBOC����ʱ
    }
    else {
        //�÷���Buf ���������ֵ
        ret = KbInputPin(100, inpin, pwdInput.passwdLen, SDK_DISP_LINE5);
        //20150318 ��Ӹ�λ�����⸴λ����
        if (GetCmdResetExe() == TRUE || !JudgeTradeFlow()) {
            FinishTradeFlow();
            bisNeedBrushScreenWhenBack = true;
            return INS_NOT_RETURN;
        }
        if (ret == SDK_TIME_OUT) {
            //FinishTradeFlow();
            //bisNeedBrushScreenWhenBack = true;
            return INS_KEEP;
        }
        else if (ret == SDK_KEY_ENTER) {
            FinishTradeFlow();                //20150313 �����������ý������׷���ˢ���Ͷ����� ������һ��
            passwdbuf[0] = strlen(inpin);
            Trace("Mai", "Pin: %s \r\n", inpin);
            memcpy(&passwdbuf[1], inpin, strlen(inpin));
            ret = (passwdbuf[0] == 0 ? SDK_PED_NOPIN : SDK_OK);

            //zcl ��ҵ����Ҫ����PIN
            if (pwdInput.cryptType == 0 && ret == SDK_OK)            //ǰ8�ֽڷż���pin,���油0
            {
                u8 tmp[20] = {0};
                u8 asPan[8] = {0};
                AppGetMagCardNo(&pstCardInfo.stCardData, tmp);

                if (strlen(tmp) < 13) {
                    FinishTradeFlow();
                    bisNeedBrushScreenWhenBack = true;
                    return INS_OTHER_ERR;
                }
                /*����pan*/
                memset(asPan, 0x00, sizeof(asPan));
                sdkAscToBcd(&asPan[2], tmp + (strlen(tmp) - 12 - 1), 12);
//                TraceHex("Mai", "Pan", &asPan[0], 8);

                /*����pin*/
                memset(fensan, 0xFF, sizeof(fensan));
                fensan[0] = strlen(
                        inpin);                                                      // ��һ���ֽڱ�ʾ���볤��
                sdkAscToBcd(fensan + 1, inpin,
                            fensan[0]);                                      // ��ASCII����ת����BCD

                if ((fensan[0] % 2) !=
                    0)                                                        // ����
                {
                    fensan[(fensan[0] / 2) +
                           1] |= 0x0F;                                            // ȷ��Ϊ0x0F
                }
                /*�������*/
                sdkXOR8(asPan, fensan, asPan);
//                TraceHex("Mai", "asPan", asPan, 8);

                sdkPEDWkDes(pincfg.ucTpkIndex, wkDesType, SDK_PED_ENCRYPT, asPan, fensan);
                passwdbuf[0] = 8;
                memcpy(passwdbuf + 1, fensan, 8);
            }
            //end
        }
        else {
            FinishTradeFlow();                //20150313 �����������ý������׷���ˢ���Ͷ����� ������һ��
            ret = SDK_ESC;
        }
    }
    //ret = sdkPEDInputPIN(&pincfg, NULL, passwdbuf);

//    TraceHex("Mai", "encrypt: ", passwdbuf, passwdbuf[0] + 1);
    sendCount = 0;

    switch (ret) {
        case SDK_OK:
        case SDK_PED_NOPIN:       //Xiaox 15-1-12
        {
            //DispWait();           //20150402    ˢ������ʱ��ʾ�ϱ�PBOC�еĽ��
            giAutoRetTimer = sdkTimerGetId();
            bisNeedBrushScreenWhenBack = false;
            inpin[0] = 0x00;
            sendCount += 1;
        }
            break;

        case SDK_ESC:
        default: {
            bisNeedBrushScreenWhenBack = true;        //Xiaox 15-1-13
            inpin[0] = 0x06;
            sendCount += 1;
        }
            break;
    }

    inpin[1] = passwdbuf[0];
    sendCount += 1;
    {
        u8 databuf[64];
        if (pwdInput.cryptType == 0) //ǰ8�ֽڷż���pin,���油0
        {
            databuflen = 32;
            memset(databuf, 0, sizeof(databuf));
            memcpy(databuf, passwdbuf + 1, passwdbuf[0]);
        }
        else {
            if (passwdbuf[0] == 0) {
                databuflen = 32;
                memset(databuf, 0, sizeof(databuf));
                memcpy(databuf, passwdbuf + 1, passwdbuf[0]);
            }
            else {
                memset(databuf, 'F', 14);
                memcpy(databuf, &passwdbuf[1], passwdbuf[0]);
                sdkAscToBcd(&passwdbuf[1], databuf, 14);
                //            TraceHex("zcl", "pinblock", passwdbuf, 8);

                sdkBcdToAsc(databuf, gPlatformSerialNO, 6);
                databuflen = 12;
                memcpy(databuf + databuflen, gPlatformSerialNO + 6, 6);
                databuflen += 6;
                sdkAscToBcd(databuf + databuflen, MposDeviceId + 8, 12);
                databuflen += 6;
                memcpy(databuf + databuflen, passwdbuf, 8);
                databuflen += 8;
//                TraceHex("zcl", "gPlatformSerialNO", gPlatformSerialNO, 12);

                memset(fensan, 0, sizeof(fensan));
                memcpy(fensan, MposPinKsn, 8);

                for (i = 8; i < 16; i++) {
                    fensan[i] = ~fensan[i - 8];
                }

                //fensan[i] = fensan[i - 8];
#if 0
                sdkAscToBcd(key, MposAppKey, 32);
                sdkDes3S(1, fensan, key);
                sdkDes3S(1, fensan + 8, key);
#endif

                sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan, fensan);
                sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, fensan + 8,
                             fensan + 8);


                //memcpy(key, fensan, 16);
                //            TraceHex("zcl", "key", key, 16);

                TraceHex("zcl", "databuf", databuf, databuflen);

                Des3sEcb(databuf, databuflen, databuf, &databuflen, fensan);

                TraceHex("zcl", "databuf", databuf, databuflen);
            }
        }
        memcpy(inpin + sendCount, databuf, databuflen);
        sendCount += databuflen;
    }
    memcpy(&inpin[34], MposPinKsn, 8);
    memset(&inpin[34 + 8], 0, 2);
    sendCount += 10;
    pstTransFlow->usSendDataLen = sendCount;
    memcpy(pstTransFlow->heReceiveData, inpin, sendCount);      //0416Send==Recv
    AddMposKSN(ADDPINKSN);
    return INS_SUC;

}

/*****************************************************************************
** Descriptions:		װ�ع�����Կ
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

*****************************************************************************/

/*
        ID		������				����			��ע

        1		KEK ģʽ				  1				0x01:	ʹ��TR31 �ĸ�ʽ
                                                                                                        0x02:	ʹ�ô�����Կ��������Կ�ĸ�ʽ
                                                                                                        0x03:	����Կ���÷��з�˽Կ���ܵĸ�ʽ
                                                                                                        0x04:	ʹ������Կ��������Կ�ĸ�ʽ.

        2		����Կ����                1				1~255	��װ�ص�����Կ����

        3		װ������Կ����  LL...1024		��ID1  ֵΪ0x01, TR31 ��ʽ������
                                                                                                        ��ID1  ֵΪ0x02, ������Կ���ܵ�����
                                                                                                        ��ID1  ֵΪ0x03, Ϊ�������з�˽Կ����
                                                                                                        �������Կ,��16  �ֽ�����Կ+  У��ֵ
                                                                                                        8  �ֽ�
                                                                                                        ��ID1  ֵΪ0x04, ʹ��ID4 ָ��������Կ����
                                                                                                        ������Կ����.

        4		����Կ����		   1				��ID  ֵΪ0x04  ʱ,ָ���Ѱ�װ������Կ����.

 */

s32 OnLoadTMK(ST_TRANSFLOW *pstTransFlow) {
    u8 keymode = 0, newTMKIndex = 0, oldTMKIndex = 0;
    s32 ret = 0;
    u8 *pRecv = NULL, *pSend = NULL, *pData = NULL, encryptbuf[8];
    u16 count = 0, cryptlen = 0;

    SDK_PED_DES_TYPE newDesType, oldDesType;

    pRecv = pstTransFlow->heReceiveData;
    pSend = pstTransFlow->heReceiveData;       //0416Send==Recv
    pstTransFlow->usSendDataLen = 0;

    keymode = pRecv[count];
    count += 1;

#ifdef DEBUG_CODE_TEST

    ret = loadDefaultTMK();

    if (ret != SDK_OK)
    {
        return INS_OTHER_ERR;
    }
#endif

    if (keymode == 2 || keymode == 4) {
        newTMKIndex = pRecv[count];
        count += 1;

        sdkBcdToU16(&cryptlen, &pRecv[count], 2);
        count += 2;

        if (cryptlen == 8) {
            newDesType = SDK_PED_DES_SINGLE;
        }
        else if (cryptlen == 16 || cryptlen == 24) {
            newDesType = SDK_PED_DES_TRIPLE;
        }
        else {
            memcpy(pSend, "45", 2);
            pstTransFlow->usSendDataLen += 2;
            goto ERR_RETURN;
        }
        pData = &pRecv[count];
        count += cryptlen;

        oldTMKIndex = pRecv[count];
        //Xiaox 15-1-8 for test
#if 1
        oldDesType = SDK_PED_DES_TRIPLE;
#else

        if (oldTMKIndex < TMK_TRIPLE_DES_INDEX)
        {
            oldDesType = SDK_PED_DES_SINGLE;
        }
        else
        {
            oldDesType = SDK_PED_DES_TRIPLE;
        }
#endif
        ret = sdkPEDUpdateTmkEx(oldTMKIndex, oldDesType, newTMKIndex, newDesType, pData, 300);

        if (ret != SDK_OK) {
            memcpy(pSend, "02", 2);
            pstTransFlow->usSendDataLen += 2;
            goto ERR_RETURN;
        }
    }
    memset(encryptbuf, 0, sizeof(encryptbuf));
    sdkPEDDesEx(newTMKIndex, SDK_PED_TMK, newDesType, SDK_PED_ENCRYPT, SDK_PED_DES_ALL, encryptbuf,
                sizeof(encryptbuf), &pSend[2]);
    pstTransFlow->usSendDataLen += 8;

    memcpy(pSend, "00", 2);
    pstTransFlow->usSendDataLen += 2;

    return INS_SUC;

    ERR_RETURN:

    memset(&pSend[2], 0, 8);
    pstTransFlow->usSendDataLen += 8;

    Trace("dcq", "err return");

    return INS_OTHER_ERR;
}

/*****************************************************************************
** Descriptions:		װ�ع�����Կ
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

*****************************************************************************/

/*
        ID		������				����			��ע

        1		��Կ����			  1				0x01:	���ݼ��ܹ�����Կ
                                                                                                        0x02:	PIN ������Կ
                                                                                                        0x03:	MAC ������Կ

        2		����Կ����                1				0~255	ָ��������Կ����

        3		������Կ����	  1				0~255	Ҫװ�صĹ�����Կ����

        4		��Կ		                        LL...24

 */

s32 OnLoadWK(ST_TRANSFLOW *pstTransFlow) {
    u8 cryptLen = 0;
    u8 *pRecv, *pSend, temp[8];
    u16 recvCount = 0, sendCount = 0;
    s32 ret = 0;
    SDK_PED_WK_CFG pedWK;

    pSend = pstTransFlow->heReceiveData;           //0416Send==Recv
    pRecv = pstTransFlow->heReceiveData;

    recvCount += 1;

#ifdef DEBUG_CODE_TEST

    ret = loadDefaultTMK();

    if (ret != SDK_OK)
    {
        return INS_OTHER_ERR;
    }
#endif

    IsQmfMode = 0;

    //  ����������
    if (pstTransFlow->cmdID == LOAD_WK_OUTER) {
        ret = sdkPEDInitKeyIC();

        if (ret != SDK_OK) {
            memcpy(&pSend[1], "02", 2);
            goto ERR_RETURN;
        }
    }
    pedWK.ucTmkIndex = pRecv[recvCount];
//	pedWK.ucTmkIndex = 99;

    recvCount += 1;

#if 1
    pedWK.eTmkType = SDK_PED_DES_TRIPLE;
#else

    if (pedWK.ucTmkIndex <= TMK_TRIPLE_DES_INDEX)
    {
        pedWK.eTmkType = SDK_PED_DES_SINGLE;
    }
    else
    {
        pedWK.eTmkType = SDK_PED_DES_TRIPLE;
    }
#endif

    pedWK.ucWkIndex = pRecv[recvCount];
    recvCount += 1;

    sdkBcdToU16(&cryptLen, &pRecv[recvCount], 2);
    recvCount += 2;

    if (cryptLen == 8) {
        pedWK.eWkType = SDK_PED_DES_SINGLE;
    }
    else if (cryptLen == 16 || cryptLen == 24) {
        pedWK.eWkType = SDK_PED_DES_TRIPLE;
    }
    else {
        memcpy(pSend, "45", 2);
        sendCount += 2;
        goto ERR_RETURN;
    }
    pedWK.ucEnWkLen = cryptLen;
    memcpy(pedWK.heEnWk, &pRecv[recvCount], cryptLen);

    pedWK.ucCheckDataLen = 0;
    pedWK.ucCheckValueLen = 0;
    memset(pedWK.heCheckData, 0, sizeof(pedWK.heCheckData));
    memset(pedWK.heCheckValue, 0, sizeof(pedWK.heCheckValue));

    ret = sdkPEDUpdateWkEx(&pedWK, SDK_PED_DES_ALL);

    if (ret != SDK_OK) {
        memcpy(pSend, "02", 2);
        sendCount += 2;
        goto ERR_RETURN;
    }
    memset(temp, 0, sizeof(temp));
    ret = sdkPEDWkDes(pedWK.ucWkIndex, pedWK.eWkType, SDK_PED_ENCRYPT, temp, &pSend[2]);
    sendCount += 8;

    if (ret != SDK_OK) {
        memcpy(pSend, "02", 2);
        sendCount += 2;
        goto ERR_RETURN;
    }
    memcpy(pSend, "00", 2);
    sendCount += 2;
    pstTransFlow->usSendDataLen = sendCount;

    return INS_SUC;

    ERR_RETURN:

    memset(&pSend[2], 0, 8);
    sendCount += 8;
    pstTransFlow->usSendDataLen = sendCount;

    return INS_OTHER_ERR;
}

/*****************************************************************************
** Descriptions:		���ݼ���/ ����
** Parameters:                  T_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

*****************************************************************************/

s32 OnDataEncrypt(ST_TRANSFLOW *pstTransFlow) {
    u8 *pRecv = NULL, *pdata = NULL, temp[8], temp1[8], encryptbuf[24];
    u8 cryptIndex = 0, cryptMode = 0;
    u32 count = 0;
    u16 dataLen = 0, cryptLen = 0, sendlen;
    //s32 ret = 0;

    ENUM_CRYPTINDEX CRYPTINDEX = NULL_INDEX;
    SDK_PED_MAC_MODE desMode;
    SDK_PED_CRYPT encryptMode;
    SDK_PED_DES_TYPE desType;

    pRecv = pstTransFlow->heReceiveData;
    sendlen = 0;
    u8 *pSend = sdkGetMem(1024);       //0416Send==Recv

    if (pSend == NULL) {
        memcpy(&pSend[1], "02", 2);
        goto ERR_RETURN;
    }
    memset(pSend, 0, 1024);

#if 0 /*Modify by ���ƽ at 2015.04.18  15:42 */
    //  ����������
    if (pstTransFlow->cmdID == DATA_ENCRYPT_OUTER)
    {
        ret = sdkPEDInitKeyIC();

        if (ret != SDK_OK)
        {
            memcpy(&pSend[1], "02", 2);
            goto ERR_RETURN;
        }
    }
#endif /* if 0 */
    cryptIndex = pRecv[count];
    count += 1;
    pSend[0] = cryptIndex;

    cryptMode = pRecv[count];
    count += 1;

    switch (cryptMode) {
        case 1: {
            encryptMode = SDK_PED_ENCRYPT;
            desMode = SDK_PED_CBC;
        }
            break;

        case 2: {
            encryptMode = SDK_PED_ENCRYPT;
            desMode = SDK_PED_ECB;
        }
            break;

        case 3: {
            encryptMode = SDK_PED_DECRYPT;
            desMode = SDK_PED_CBC;
        }
            break;

        case 4: {
            encryptMode = SDK_PED_DECRYPT;
            desMode = SDK_PED_ECB;
        }
            break;

        default: {
        }
            break;
    }

    sdkBcdToU16(&dataLen, &pRecv[count], 2);
    count += 2;

    pdata = &pRecv[count];
    count += dataLen;

    TraceHex("dcq", "data: \n", pdata, dataLen);

    sdkBcdToU16(&cryptLen, &pRecv[count], 2);
    count += 2;
#if 1
    desType = SDK_PED_DES_TRIPLE;
#else

    if (cryptLen == 8)
    {
        desType = SDK_PED_DES_SINGLE;
    }
    else if (cryptLen == 16 || cryptLen == 24)
    {
        desType = SDK_PED_DES_TRIPLE;
    }
    else
    {
        memcpy(&pSend[1], "45", 2);
        goto ERR_RETURN;
    }
#endif
//	memcpy(encryptbuf, &pRecv[count], cryptLen);
    count += cryptLen;

    if (cryptLen == 0) {
        CRYPTINDEX = WK_INDEX;
    }
    else {
        CRYPTINDEX = TMK_INDEX;
        sdkPEDDesEx(cryptIndex, SDK_PED_TMK, desType, SDK_PED_DECRYPT, SDK_PED_DES_ALL,
                    &pRecv[count], cryptLen, encryptbuf);
    }

    if (desMode == SDK_PED_CBC) {
        memcpy(temp1, &pRecv[count], 8);
    }
    {
        u16 i = 0, m = 0, n = 0, k = 0, len;
        m = dataLen % 8;
//		n = dataLen / 8;//sujianzhong 2015.01.07 22:57
        n = (dataLen + 7) / 8;

        for (i = 0; i < n; i++) {
//			if (m && i==n-1)//sujianzhong 2015.01.07 22:57
            if ((i + 1) * 8 > dataLen) {
                memcpy(temp, &pdata[8 * i], m);
                k = m;

                while (k++ < 8) {
                    temp[k - 1] = 0x00;
                }
            }
            else {
                memcpy(temp, &pdata[8 * i], 8);
            }

            if (desMode == SDK_PED_CBC) {
                k = 0;

                while (k++ < 8) {
                    temp[k - 1] = temp[k - 1] ^ temp1[k - 1];
                }
            }

            if (CRYPTINDEX == WK_INDEX) {
                TraceHex("dcq", "before descryptdata", temp, 8);
                sdkPEDWkDes(cryptIndex, desType, encryptMode, temp, temp1);
                TraceHex("dcq", "after decryptdata\n", temp1, 8);
            }
            else {
                sdkPEDTemporaryKeyDes(SDK_PED_ENCRYPT, temp, 8, temp1);
            }
            memcpy(&pSend[5 + 8 * i], temp1, sizeof(temp1));
            memcpy(temp1, temp, 8);
        }

        if (m == 0) {
            len = 8 * n;
        }
        else {
            len = 8 * (n + 1);
        }
        sdkU16ToBcd(&pSend[3], len, 2);
        pstTransFlow->usSendDataLen = len;
    }

    memcpy(&pSend[1], "00", 2);
    pstTransFlow->usSendDataLen += 5;
    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
    sdkFreeMem(pSend);
    return INS_SUC;

#if 1
    ERR_RETURN:

    memcpy(&pSend[3], 0, 2);
    pstTransFlow->usSendDataLen = 5;
    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
    sdkFreeMem(pSend);
    return INS_OTHER_ERR;

#endif
}

/*****************************************************************************
** Descriptions:		MAC ����(������Ϣ���豸�ڲ���ȡ)
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

** Notice: MAC 9606 SDK û��ʵ��.

*****************************************************************************/

/*
        ID		������				����			��ע

        1		��Կ����			  1				����Կ����/ ������Կ����, ��Ϊ����Կ
                                                                                                        ������ID6 ��ָ��������Կ, ��Ϊ������Կ
                                                                                                        ������ID6 Ϊ0 �������ڹ�����Կ.

        2		��Կ��ϵ����      1				0:	MK/SK Ĭ��
                                                                                                        1:	DUKPT

        3		MAC ģʽ				  1				0x00:	MAC X99
                                                                                                        0x01:	MAC X919
                                                                                                        0x02:	ECB �����淶ECB �㷨
                                                                                                        0x03:	MAC 9606

        4		���־	                          1				0:	First block
                                                                                                        1:	Next block
                                                                                                        2:	Last block
                                                                                                        3:	Only block

        5		��MAC ������		LL...1024               �������ݽ���DES ��������

        6		������Կ����	LL...24			�ŵ����ܹ�����Կ����

 */


s32 OnCalcMAC(ST_TRANSFLOW *pstTransFlow) {
    u8 *pRecv = NULL, *pdata = NULL, macbuf[8], cryptbuf[24];
    u32 len = 0, count = 0, sendCount = 0;
    s32 fileLen = 0, ret = 0, returnValue = 0;

    maccal_struct maccal;
    maccal_struct *pmaccal = &maccal;

    SDK_PED_MAC_MODE macMode;
    SDK_PED_DES_TYPE desType;
    SDK_ENCRYPT_MODE encryptType;

    u8 pSend[24] = {0};       //0416Send==Recv
    pRecv = pstTransFlow->heReceiveData;
    sendCount = 0;


    //  ����������
    if (pstTransFlow->cmdID == MAC_CALC_OUTER) {
        ret = sdkPEDInitKeyIC();

        if (ret != SDK_OK) {
            memcpy(&pSend[1], "02", 2);
            goto ERR_RETURN;
        }
    }
    pmaccal->cryptIndex = pRecv[count];
    count += 1;

    pSend[0] = pmaccal->cryptIndex;
    sendCount += 1;

    pmaccal->cryptMode = pRecv[count];
    count += 1;

    pmaccal->macMode = pRecv[count];
    count += 1;

    switch (pmaccal->macMode) {
        case 0x0: {
            macMode = SDK_PED_CBC;
        }
            break;

        case 0x1: {
            macMode = SDK_PED_X919;
        }
            break;

        case 0x2:
        case 0x3: {
            macMode = SDK_PED_ECB;
        }
            break;
    }

    pmaccal->blockFlag = pRecv[count];
    count += 1;

    sdkBcdToU16(&(pmaccal->macLen), &pRecv[count], 2);
    count += 2;

    pmaccal->pMac = &pRecv[count];
    count += pmaccal->macLen;

    sdkBcdToU16(&(pmaccal->cryptLen), &pRecv[count], 2);
    count += pmaccal->cryptLen;

#if 1
    desType = SDK_PED_DES_SINGLE;
    encryptType = SDK_DES;
#else

    if (pmaccal->cryptLen == 8)
    {
        desType = SDK_PED_DES_SINGLE;
        encryptType = SDK_DES;
    }
    else
    {
        desType = SDK_PED_DES_TRIPLE;
        encryptType = SDK_3DES;
    }
#endif
    pmaccal->encryptData = &pRecv[count];

    if (pmaccal->blockFlag == BLOCK_FIRST) {
        ret = sdkWriteFile(MACCALC_FILENAME, pmaccal->pMac, pmaccal->macLen);

        if (ret != SDK_OK) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        return INS_SUC;
    }
    else if (pmaccal->blockFlag == BLOCK_NEXT) {
        ret = sdkAppendFile(MACCALC_FILENAME, pmaccal->pMac, pmaccal->macLen);

        if (ret != SDK_FILE_OK) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        return INS_SUC;
    }
    else if (pmaccal->blockFlag == BLOCK_LAST) {
        fileLen = sdkGetFileSize(MACCALC_FILENAME);

        if (fileLen < 0) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        pdata = sdkGetMem(fileLen + pmaccal->macLen);

        if (pdata == NULL) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        ret = sdkReadFile(MACCALC_FILENAME, pdata, 0, &fileLen);

        if (ret != SDK_FILE_OK) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        memcpy(&pdata[fileLen], pmaccal->pMac, pmaccal->macLen);

        sdkDelFile(MACCALC_FILENAME);
    }

    if (pmaccal->blockFlag == BLOCK_ONLY) {
        pdata = pmaccal->pMac;
        len = pmaccal->macLen;
    }

    if (pmaccal->cryptLen != 0) {
        sdkPEDTmkDes(pmaccal->cryptIndex, desType, SDK_PED_DECRYPT, pmaccal->encryptData, cryptbuf);

        if (pmaccal->macMode == SDK_PED_ECB) {
            sdkE90S(macbuf, pdata, len, cryptbuf, encryptType);
        }
        else if (pmaccal->macMode == SDK_PED_CBC) {
            sdkE99S(macbuf, pdata, len, cryptbuf, encryptType);
        }
        else {
            sdkE919S(macbuf, pdata, len, cryptbuf, encryptType);
        }
    }
    else {
        sdkPEDCalcMac(pmaccal->cryptIndex, macMode, desType, pdata, len,
                      macbuf);        //sujianzhong 2015.01.12 14:36
    }

    if (pmaccal->blockFlag == BLOCK_LAST) {
        sdkFreeMem(pdata);
        pdata = NULL;                       //20150113 modify by yc sdkFreeMem
    }
    memcpy(&pSend[1], "00", 2);
    sendCount += 2;

    TraceHex("dcq", "macbuf: \n", macbuf, sizeof(macbuf));

    memcpy(&pSend[3], macbuf, sizeof(macbuf));
    sendCount += sizeof(macbuf);

    pstTransFlow->usSendDataLen = sendCount;
    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
    return INS_SUC;

    ERR_RETURN:

    if (pmaccal->blockFlag == BLOCK_LAST) {
        if (pdata)                             //20150113 modify by yc sdkFreeMem
        {
            sdkFreeMem(pdata);
        }
        pdata = NULL;                           //20150113 modify by yc sdkFreeMem
    }
    memcpy(&pSend[3], 0, sizeof(macbuf));
    sendCount += sizeof(macbuf);

    pstTransFlow->usSendDataLen = sendCount;
    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);    //0416Send==Recv
    return returnValue;
}

u8 parseTag(u8 *pTag, u16 tagLen, u8 *tagbuf, u8 tagFlag[]) {
    u8 tagCount = 0, len = 0, count = 0, i = 0, j = 0;
    u16 tagOffsetBuf[20];

    tlvStruct pTLV = (tlvStruct) tagbuf;

    tagFlag[0] = 0;
    memset(tagOffsetBuf, 0, sizeof(tagOffsetBuf));

    while (count <= tagLen) {
        memset(pTLV, 0, sizeof(struct TLV_STRUCT));

        if ((pTag[count] & 0x1f) != 0x1f) {
            len = 1;
        }
        else if ((pTag[count + 1] & 0x80) != 0x80) {
            len = 2;
        }
        else {
            len = 3;
        }
        memcpy(pTLV->tag, &pTag[count], len);
        count += len;

        sdkBcdToU32(&pTLV->tagLen, &pTag[count], 4);
        count += 4;

        sdkBcdToU16(&pTLV->tagOffset, &pTag[count], 2);
        count += 2;

/*
        ����ƫ��λ�õĴ�С����, ��λ�����������tagFlag  ��.
 */

        {
            for (i = 0; i < tagCount; i++) {
                if (pTLV->tagOffset > tagOffsetBuf[i]) {
                    continue;
                }
                break;
            }

            for (j = tagCount; j > i; j--) {
                tagOffsetBuf[j] = tagOffsetBuf[j - 1];
                tagFlag[j] = tagFlag[j - 1];
            }

            tagOffsetBuf[i] = pTLV->tagOffset;
            tagFlag[i] = tagCount;
        }

        sdkBcdToU16(&pTLV->tagMax, &pTag[count], 2);
        count += 2;

        pTLV++;
        tagCount++;

        if (tagLen - count < 9) {
            break;
        }
    }

    return tagCount;
}

s32 findTag(u8 *pTag, u8 buf[]) {
    u8 count = 0, i = 0, tagBuf[4];
    s32 fileLen = 0, fileOffset = 0, ret = 0, readLen = 0;

    fileLen = sdkGetFileSize(STORE_TAG_FILENAME);

    if (fileLen < 0) {
        return INS_OTHER_ERR;
    }
    count = fileLen / (4 + 128);

    for (i = 0; i < count; i++) {
        memset(tagBuf, 0, sizeof(tagBuf));
        fileOffset = i * (4 + 128);
        readLen = 4;
        ret = sdkReadFile(STORE_TAG_FILENAME, tagBuf, fileOffset, &readLen);

        if (ret != SDK_FILE_OK) {
            return INS_OTHER_ERR;
        }

        if (!memcpy(pTag, tagBuf, sizeof(tagBuf))) {
            readLen = 128;
            ret = sdkReadFile(STORE_TAG_FILENAME, buf, fileOffset + 4, &readLen);

            if (ret != SDK_FILE_OK) {
                return INS_OTHER_ERR;
            }
            return INS_SUC;
        }
    }

    return INS_OTHER_ERR;
}

s32 storeTagData(u8 *pTag, u16 tagLen, u8 *pMac, u16 macLen, u8 *tagbuf) {
    u8 tagCount = 0, tagFlag[20], i = 0;//, count = 0;
    u16 lenPriv = 0, len = 0, offset = 0, offset1 = 0, tagMax = 0;

#if 0
    u8 buf[] = { 0x13, 0x00, 0x00, 0x00, 0x04, 0x13, 0xf0, 0x00, 0x10, \
                 0x14, 0x00, 0x00, 0x00, 0x04, 0x11, 0x23, 0x00, 0x10, \
                 0x18, 0x00, 0x00, 0x00, 0x04, 0x12, 0x60, 0x00, 0x10, \
                 0x17, 0x00, 0x00, 0x00, 0x04, 0x11, 0x02, 0x00, 0x10, \
                 0x15, 0x00, 0x00, 0x00, 0x04, 0x15, 0xe0, 0x00, 0x10, \
                 0x11, 0x30, 0x55 };
#endif

    if (tagLen > 9) {
        tagCount = parseTag(pTag, tagLen, tagbuf, tagFlag);
    }

    for (i = 0; i < tagCount; i++) {
        offset = tagFlag[i] * sizeof(struct TLV_STRUCT);
        memcpy(&offset1, &tagbuf[offset + 8], 2);
        len = offset1 - lenPriv;
        sdkAppendFile(MACCALC_FILENAME, pMac, len);
        lenPriv = offset1;

        memcpy(&tagMax, &tagbuf[offset + 10], 2);
        findTag(&tagbuf[offset], &tagbuf[200]);
        tagMax > tagbuf[200] ? tagbuf[200] : tagMax;
        sdkAppendFile(MACCALC_FILENAME, &tagbuf[201], tagMax);
    }

    return 0;
}

/*****************************************************************************
** Descriptions:		MAC ����(������Ϣ���豸�ڲ���ȡ)
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

** Notice: MAC 9606 SDK û��ʵ��.

*****************************************************************************/

/*
        ID		������				����			��ע

        1		��Կ����			  1				����Կ����/ ������Կ����, ��Ϊ����Կ
                                                                                                        ������ID6 ��ָ��������Կ, ��Ϊ������Կ
                                                                                                        ������ID6 Ϊ0 �������ڹ�����Կ.

        2		��Կ��ϵ����      1				0:	MK/SK Ĭ��
                                                                                                        1:	DUKPT

        3		MAC ģʽ				  1				0x00:	MAC X99
                                                                                                        0x01:	MAC X919
                                                                                                        0x02:	ECB �����淶ECB �㷨
                                                                                                        0x03:	MAC 9606

        4		���־	                          1				0:	First block
                                                                                                        1:	Next block
                                                                                                        2:	Last block
                                                                                                        3:	Only block

        5		��MAC ������		LL...1024               �������ݽ���DES ��������

        6		������Կ����	LL...24			�ŵ����ܹ�����Կ����

        7		�ڲ�ָ����ȡ��Ϣָʾ���� LL...128

 */


s32 OnCalcMACStoreTag(ST_TRANSFLOW *pstTransFlow) {
    u8 *pRecv = NULL, *pdata = NULL, macbuf[8], cryptbuf[24];
    u32 len = 0, count = 0, sendCount = 0;
    s32 fileLen = 0, ret = 0, returnValue = 0;

    maccal_struct maccal;
    maccal_struct *pmaccal = &maccal;

    SDK_PED_MAC_MODE macMode;
    SDK_PED_DES_TYPE desType;
    SDK_ENCRYPT_MODE encryptType;

    u8 pSend[24] = {0};                         //0416Send==Recv
    u8 pTagData[512] = {0};                     //0416Send==Recv
    pRecv = pstTransFlow->heReceiveData;
    sendCount = 0;

    loadDefaultWK();

    pmaccal->cryptIndex = pRecv[count];
    count += 1;

    pSend[0] = pmaccal->cryptIndex;
    sendCount += 1;

    pmaccal->cryptMode = pRecv[count];
    count += 1;

    pmaccal->macMode = pRecv[count];
    count += 1;

    switch (pmaccal->macMode) {
        case 0x0: {
            macMode = SDK_PED_CBC;
        }
            break;

        case 0x1: {
            macMode = SDK_PED_X919;
        }
            break;

        case 0x2:
        case 0x3: {
            macMode = SDK_PED_ECB;
        }
            break;
    }

    pmaccal->blockFlag = pRecv[count];
    count += 1;

    sdkBcdToU16(&(pmaccal->macLen), &pRecv[count], 2);
    count += 2;

    pmaccal->pMac = &pRecv[count];
    count += pmaccal->macLen;

    sdkBcdToU16(&(pmaccal->cryptLen), &pRecv[count], 2);
    count += pmaccal->cryptLen;

    if (pmaccal->cryptLen == 8) {
        desType = SDK_PED_DES_SINGLE;
        encryptType = SDK_DES;
    }
    else {
        desType = SDK_PED_DES_TRIPLE;
        encryptType = SDK_3DES;
    }
    pmaccal->encryptData = &pRecv[count];
    count += pmaccal->cryptLen;

    sdkBcdToU16(&(pmaccal->tagLen), &pRecv[count], 2);
    count += 2;

    pmaccal->pTag = &pRecv[count];

    if (pmaccal->blockFlag == BLOCK_FIRST) {
        if (pmaccal->tagLen) {
            // use pSend: 1~100: save return data. 100~300: save TLV data. 300~512: save tag data.
            storeTagData(pmaccal->pTag, pmaccal->tagLen, pmaccal->pMac, pmaccal->macLen,
                         pTagData);     //0416Send==Recv
        }
        else {
            ret = sdkWriteFile(MACCALC_FILENAME, pmaccal->pMac, pmaccal->macLen);

            if (ret != SDK_OK) {
                memcpy(&pSend[1], "02", 2);
                returnValue = INS_OTHER_ERR;
                goto ERR_RETURN;
            }
        }
        memcpy(pstTransFlow->heReceiveData, pSend, 3);      //0416Send==Recv
        pstTransFlow->usSendDataLen = 3;                    //0416Send==Recv 
        return INS_SUC;
    }
    else if (pmaccal->blockFlag == BLOCK_NEXT) {
        ret = sdkAppendFile(MACCALC_FILENAME, pmaccal->pMac, pmaccal->macLen);

        if (ret != SDK_FILE_OK) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        memcpy(pstTransFlow->heReceiveData, pSend, 3);      //0416Send==Recv
        pstTransFlow->usSendDataLen = 3;                    //0416Send==Recv
        return INS_SUC;
    }
    else if (pmaccal->blockFlag == BLOCK_LAST) {
        fileLen = sdkGetFileSize(MACCALC_FILENAME);

        if (fileLen < 0) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        pdata = sdkGetMem(fileLen + pmaccal->macLen);

        if (pdata == NULL) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        ret = sdkReadFile(MACCALC_FILENAME, pdata, 0, &fileLen);

        if (ret != SDK_FILE_OK) {
            memcpy(&pSend[1], "02", 2);
            returnValue = INS_OTHER_ERR;
            goto ERR_RETURN;
        }
        memcpy(&pdata[fileLen], pmaccal->pMac, pmaccal->macLen);

        sdkDelFile(MACCALC_FILENAME);
    }

    if (pmaccal->blockFlag == BLOCK_ONLY) {
        pdata = pmaccal->pMac;
        len = pmaccal->macLen;
    }

    if (pmaccal->cryptLen != 0) {
        sdkPEDTmkDes(pmaccal->cryptIndex, desType, SDK_PED_DECRYPT, pmaccal->encryptData, cryptbuf);

        if (pmaccal->macMode == SDK_PED_ECB) {
            sdkE90S(macbuf, pdata, len, cryptbuf, encryptType);
        }
        else if (pmaccal->macMode == SDK_PED_CBC) {
            sdkE99S(macbuf, pdata, len, cryptbuf, encryptType);
        }
        else {
            sdkE919S(macbuf, pdata, len, cryptbuf, encryptType);
        }
    }
    else {
        sdkPEDCalcMac(pmaccal->cryptIndex, macMode, SDK_PED_DES_TRIPLE, pdata, len, macbuf);
    }

    if (pmaccal->blockFlag == BLOCK_LAST) {
        if (pdata)                             //20150113 modify by yc sdkFreeMem
        {
            sdkFreeMem(pdata);
        }
        pdata = NULL;                           //20150113 modify by yc sdkFreeMem
    }
    memcpy(&pSend[1], "00", 2);
    sendCount += 2;

    TraceHex("dcq", "macbuf: \n", macbuf, sizeof(macbuf));

    memcpy(&pSend[3], macbuf, sizeof(macbuf));
    sendCount += sizeof(macbuf);

    pstTransFlow->usSendDataLen = sendCount;

    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);      //0416Send==Recv
    return INS_SUC;

    ERR_RETURN:

    if (pmaccal->blockFlag == BLOCK_LAST) {
        if (pdata)                             //20150113 modify by yc sdkFreeMem
        {
            sdkFreeMem(pdata);
        }
        pdata = NULL;                           //20150113 modify by yc sdkFreeMem
    }
    memcpy(&pSend[3], 0, sizeof(macbuf));
    sendCount += sizeof(macbuf);

    pstTransFlow->usSendDataLen = sendCount;
    memcpy(pstTransFlow->heReceiveData, pSend, pstTransFlow->usSendDataLen);      //0416Send==Recv
    return returnValue;
}

/*****************************************************************************
** Descriptions:		װ��DUKPT ��KSN �ͳ�ʼ��Կ
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

** Notice:              sdkPedDukptGetMac  �˽ӿ���δʵ��

*****************************************************************************/

/*
        ID		������				����			��ע

        1		KEK ģʽ				  1                             0x01:	ʹ��TR31 �ĸ�ʽ
                                                                                                        0x02:	ʹ�ô�����Կ����DUKPT ��ʼ��Կ�ĸ�ʽ
                                                                                                        0x03:	DUKPT ��ʼ��Կ���÷��з�˽Կ���ܵĸ�ʽ
                                                                                                        0x04:	ʹ������Կ����DUKPT ��ʼ��Կ�ĸ�ʽ

        2		KSN ����				  1                             ��Կ��ID:	1~10

        3		KSN						  10                    ����

        4		װ�صĳ�ʼ��Կ����LL...32	��ID1 ֵΪ0x01, TR31 ��ʽ������
                                                                                                        ��ID1 ֵΪ0x02, ������Կ���ܵ�����
                                                                                                        ��ID1 ֵΪ0x03, Ϊ�������з�˽Կ���ܺ�
                                                                                                        ��DUKPT ��ʼ��Կ, ��16/24 �ֽ�DUKPT ��ʼ��Կ
 + У��ֵ8 �ֽ�

        5		����Կ����(��Ϊ������Կ)		1		��ID ֵΪ0x04ʱ,ָ���Ѱ�װ������
                                                                                                                                Կ����ֵ 1...255
 */


s32 OnLoadDUKPT(ST_TRANSFLOW *pstTransFlow) {
    u8 keyMode = 0, TMKIndex = 0, KSNIndex = 0;
    u8 *pRecv = NULL, *pData = NULL, descryptbuf[16], encryptbuf[8], *pKSN = NULL;
    u16 count = 0, cryptlen = 0;
    //u8 ucKSN[8], ucMac[8];
    s32 ret = 0;

    u8 pSend[24] = {0};                         //0416Send==Recv
    pRecv = pstTransFlow->heReceiveData;


    keyMode = pRecv[count];
    count += 1;

    KSNIndex = pRecv[count];
    count += 1;

    pKSN = &pRecv[count];
    count += 10;

    sdkBcdToU16(&cryptlen, &pRecv[count], 2);
    count += 2;

    pData = &pRecv[count];
    count += cryptlen;

    if (keyMode == 4) {
        TMKIndex = pRecv[count];
    }

    if (keyMode == 2 || keyMode == 4) {
        if (cryptlen % 8 == 0) {
            if (keyMode == 4) {
                ret = sdkPEDDesEx(TMKIndex, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                  SDK_PED_DES_ALL, pData, cryptlen, descryptbuf);
            }
            else {
                ret = sdkPEDDesEx(TRANSPORT_INDEX, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                  SDK_PED_DES_ALL, pData, cryptlen, descryptbuf);
            }

            if (ret != SDK_OK) {
                memcpy(pSend, "02", 2);
                goto ERR_RETURN;
            }
            ret = sdkPEDDukptLoadAndInit(KSNIndex, descryptbuf, sizeof(descryptbuf), pKSN, 10, 500);

            if (ret != SDK_OK) {
                memcpy(pSend, "02", 2);
                goto ERR_RETURN;
            }
            memset(encryptbuf, '0', sizeof(encryptbuf));

            //	ret = sdkPedDukptGetMac(1, KSNIndex, encryptbuf, sizeof(encryptbuf), ucMac, ucKSN, 100);
            if (ret != SDK_OK) {
                memcpy(&pSend[2], "02", 2);
                goto ERR_RETURN;
            }
            memcpy(pSend, "00", 2);
        }
        else {
            memcpy(pSend, "45", 2);
            goto ERR_RETURN;
        }
    }
    else {
        memcpy(pSend, "02", 2);
        goto ERR_RETURN;
    }
    memcpy(pstTransFlow->heReceiveData, pSend, 2);          //0416Send==Recv
    memcpy(pstTransFlow->heReceiveData + 2, encryptbuf,
           8);   //0416Send==Recv    �ĺ���Ӧ��û��ʵ���߼�����
    pstTransFlow->usSendDataLen = 10;                       //0416Send==Recv
    return INS_SUC;

    ERR_RETURN:

    memset(&pSend[2], 0, sizeof(encryptbuf));
    return INS_OTHER_ERR;
}

/*****************************************************************************
** Descriptions:		����������͸��
** Parameters:                  ST_TRANSFLOW
** Returned value:	INS_SUC	:
                                        INS_OTHER_ERR:
** Created By:		dongChangQiang  2014.07.22

   ��G2 ��Ŀû������������, ��SDK Ҳû���ṩ���������̵����ýӿ�

*****************************************************************************/

s32 OnSetPinpadOuter(ST_TRANSFLOW *pstTransFlow) {
    return INS_SUC;
}

#if 0
void test()
{
    pan_flag = RecvCmd->strData[2]; //���˺�ָʾλ

    if(pan_flag == 0)
    {
        //id4 Ϊ���˺�
        memcpy(pan, RecvCmd->strData + 3, 20);
        //dubuf(pan,20,"Get  pan1:");
        memset(pan, 'F', sizeof(pan)); //û��PA
    }
    else
    {
        //id4  Ϊ���˺Ź�ϣֵ У���ϣֵ
        SHA1(gSysPara.Card_No, gSysPara.Card_Len, pan);

        if(memcmp(pan, RecvCmd->strData + 3, 20) == 0)
        {
            du("���˺Ź�ϣ�ɹ�");
            memset(pan, 'F', sizeof(pan));
            memcpy(pan, gSysPara.Card_No, gSysPara.Card_Len);
            //���´�½һ����
            memset(pan, 'F', sizeof(pan)); //û��PA
        }
        else
        {
            du("���˺Ź�ϣʧ��");
            //Sys_Uart_Debug("pan hash err",NULL,0);
            //YSSetRsposeRuslt(RecvCmd,SendCmd,YS_PARA_ERR,0);
            //return ;
            memset(pan, 'F', sizeof(pan)); //û��PAN
        }
        //dubuf(pan,20,"Get  pan2:");
    }
    offset = 3 + 20;

    if(id5_present)
    {
        disp_len = RecvCmd->strData[offset++];
        disp_len = (RecvCmd->strData[offset] >> 4) * 10 + (RecvCmd->strData[offset] & 0x0f);
        offset++;
        offset += disp_len;
    }
    else
    {
        disp_len = RecvCmd->strData[offset++];
        disp_len = (RecvCmd->strData[offset] >> 4) * 10 + (RecvCmd->strData[offset] & 0x0f);
        offset++;
        offset += disp_len;
    }
    pin_maxlen = RecvCmd->strData[offset++];
    du("Get  pin_maxlen:%d", pin_maxlen);
    memcpy(pin_add, RecvCmd->strData + offset, 10);
    offset += 10;
    enter_flag = RecvCmd->strData[offset++];
    du("Get  enter_flag:%d", enter_flag);
    timeout = RecvCmd->strData[offset++];
    du("Get  timeout:%d", timeout);
    //��ʾ��Ļ����
    disp_len = RecvCmd->strData[offset++];
    disp_len = (RecvCmd->strData[offset] >> 4) * 10 + (RecvCmd->strData[offset] & 0x0f);
    offset++;
    du("disp len:%d", disp_len);
    //Sys_DisplayLineA(3,"���:<>Ԫ",0,0);
    //Sys_DisplayLineA(4,"����������:",0,0);
    //Sys_DisplayMsgAt(128-33,3*13,"��ο�",strlen("��ο�"),0,0);
    memcpy(dispbuf, RecvCmd->strData + offset, disp_len);
    dubuf((char*)dispbuf, disp_len, "Disp");
    du((char*)dispbuf);
    //Sys_DisplayLineA(4,dispbuf,0,ALIGN_LEFT);
    //Sys_ClearLineA(2);
    //Sys_ClearScreen();
    Syd_DispAutoLineMode(2, NULL, 0, 0);
    Syd_DispAutoLine35(RecvCmd->strData + offset, disp_len);

    if(pin_maxlen > 0x0c)
    {
        pin_maxlen = 0x0c;
    }
    //if(Sys_ICCardPresent() == IC_CARD_IN)

    //Sys_DisplayLineA(5,"�������밴ȷ�ϼ�",0,0);
    ret  = Sys_GetInputPin(20, 4, (char*)pin_buf, pin_maxlen, enter_flag, timeout);


    //����PINBLOCK
    //pan
    pan_len = 0;

    for(i = 0; i < sizeof(pan); i++)
    {
        if(pan[i] == 'F')
        {
            break;
        }
        pan_len++;
    }

    du("panlen[%d]", pan_len);

    if(pan_len > 13)
    {
        memcpy(temp_buf, pan + pan_len - 1 - 12, 12);
    }
    else
    {
        memset(temp_buf, '0', sizeof(temp_buf));
    }
    //dubuf(temp_buf,16,"pan in");
    memset(pan, 0, sizeof(pan));
    Syd_DspToHex((char *)(temp_buf), (char *)(pan + 2), 6);
    //dubuf(pan,8,"pan out");
    memset(temp_buf, 0, sizeof(temp_buf));
    temp_buf[0] = pin_len;
    Syd_DspToHex((char*)pin_buf, (char*)temp_buf + 1, 7);
    //dubuf(temp_buf,8,"pin out");
    //ȡ10�ֽ��豸���к�   Ŀǰ�������̶�����Ϊ0x01
    ret = TLV6Bit_GetDev(&devFlag, 1, devInfo, 0);

    if(ret < 0 )
    {
        YSSetRsposeRuslt(RecvCmd, SendCmd, YS_OTHER_ERR, 0);
        return;
    }
    dubuf((char*)devInfo, 12, "devInfo:");
    //yihuo
    do_xor(temp_buf, pan, 8);
    dubuf((char*)temp_buf, 8, "xorpinpan:");
    memcpy(SendCmd->strData + 2, PINDerData, 18);
    memcpy(SendCmd->strData + 18 + 2, devInfo + 6, 6);
    memcpy(SendCmd->strData + 24 + 2, temp_buf, 8);
    dubuf((char*)SendCmd->strData + 2, 32, "mingwen:");
    dubuf((char*)SendCmd->strData + 2, 32, "mingwen:");

    for(i = 0; i < 4; i++)
    {
        TDES(SendCmd->strData + 2 + i * 8, SendCmd->strData + 2 + i * 8, PinKey);
    }

    offset = 32 + 2;
    memcpy(SendCmd->strData + offset, KSN_PIN, 10);
    SendCmd->strData[offset + 8] = 0x00;
    SendCmd->strData[offset + 9] = 0x00;
    IncKSN_PIN();
    offset += 10;
}

#endif

