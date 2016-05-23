#define MOBILEDETECTCARD 0x01
#define CARD_MOBILE    0x02                            //ICC��  huxiang add for mobilephone
#define MAXTAGNUM  20

#define APPNUM                                  10                                                              //maximum of applications(restricted by Daemon and system, up to 30)
#define TPK_STORENO                                     0x00                                                                    //store the serial number of TPK which in PIN pad


const AppDATAELEMENT TradeTagList[MAXTAGNUM] =
        {
                {/*"AppCrypt",0,*/         "\x9F\x26", 255, 8,  0, 0,    0, 0},
                {/*"CryptInfoData",0,*/    "\x9F\x27", 255, 1,  0, 1222, 0, 0},
                {/*"IssuAppData",0,*/      "\x9F\x10", 0,   32, 0, 2129, 0, 0},
                {/*"UnpredictNum",1,*/     "\x9F\x37", 255, 4,  0, 628,  0, 0},
                {/*"ATC",0,*/              "\x9F\x36", 255, 2,  0, 383,  0, 0},
                {/*"AmtAuthorNum",1,*/     "\x9F\x02", 255, 6,  1, 532,  0, 0},/* 45 */
                {/*"TransCurrencyCode",1,*/"\x5F\x2A", 255, 2,  1, 61,   0, 0},
                {/*"AmtOtherNum",1,*/      "\x9F\x03", 255, 6,  1, 542,  0, 0},
                //   {/*"AppVerNum",1,*/"\x9F\x09",255,2,0,59,0,0},/* 10 */
                {/*"TermCountryCode",1,*/  "\x9F\x1A", 255, 2,  1, 47,   0, 0},
                {/*"TermCapab",1,*/        "\x9F\x33", 255, 3,  0, 6,    0, 0},
//    {/*"TermType",1,*/"\x9F\x35",255,1,1,58,0,0},
//    {/*"CVR",1,*/"\x9F\x34",255,3,0,577,0,0},
                {/*"IFDSerNum",1,*/        "\x9F\x1E", 255, 8,  0, 14,   0, 0},
                {/*"TVR",1,*/              "\x95\x00", 255, 5,  0, 594,  0, 0},/* 55 */
                {/*"TransDate",1,*/        "\x9A\x00", 255, 3,  1, 602,  0, 0},
                {/*"TransTypeValue",1,*/   "\x9C\x00", 255, 1,  1, 83,   0, 0},
                {/*"AIP",0,*/              "\x82\x00", 255, 2,  0, 320,  0, 0},
                //   {/*"TSI",1,*/"\x9B\x00",255,2,0,599,0,0},
                //   {/*"AppLabel",0,*/"\x50\x00",1,16,0,323,0,0},
                {/*"Track2Equivalent",0,*/ "\x57\x00", 0,   19, 0, 3608, 0, 0},
                {/*"PANSeqNum",0,*/        "\x5F\x34", 255, 1,  1, 367,  0, 0},
//    {/*"AuthorRespCode",1,*/"\x8A\x00",255,2,0,575,0,0},
//    {/*"VLPIssuAuthorCode",0,*/"\x9F\x74",255,6,0,4016,0,0},
//    {/*"TransSeqCount",1,*/ "\x9F\x41", 255, 4, 1, 524, 0, 0},
                {/*"DFName",0,*/           "\x84\x00", 5,   16, 0, 1226, 0, 0},
                {/*CardIdentifyInfo*/      "\x9f\x63", 255, 16, 0, 4347, 0, 0},
                {/*"AppExpireDate",0,*/    "\x5F\x24", 255, 3,  1, 47,   1, 0},        //mandatory /* 5 */
                //   {/*"PAN",0,*/ "\x5A\x00", 1, 10, 2, 357, 1, 0},        //mandatory lechan 20070302 PAN ���Ȳ���Ϊ0
        };

const AppDATAELEMENT TradeTagList2[MAXTAGNUM] =
        {
                {/*"PANSeqNum",0,*/       "\x5F\x34", 255, 1,  1, 367,  0, 0},
                {/*"AppExpireDate",0,*/   "\x5F\x24", 255, 3,  1, 47,   1, 0},        //mandatory /* 5 */
                {/*"PAN",0,*/             "\x5A\x00", 1,   10, 2, 357,  1, 0},        //mandatory lechan 20070302 PAN ���Ȳ���Ϊ0
                {/*"Track2Equivalent",0,*/"\x57\x00", 0,   19, 0, 3608, 0, 0},
        };

u8 RidTab[4][5] =
        {
                {0xA0, 0x00, 0x00, 0x03, 0x33},
                {0xA0, 0x00, 0x00, 0x00, 0x03},
                {0xA0, 0x00, 0x00, 0x00, 0x04},
                {0xA0, 0x00, 0x00, 0x00, 0x65}
        };

u8 const InterCodeTab[4][4] = {
        {"CUP"},
        {"VIS"},
        {"MCC"},
        {"JCB"},
};

u8 SysSelAid;                                        //ϵͳѡ��AID����


/*******************************************************************
��    ��: ����
��    Ȩ: Okolle Ltd,Co.
��������: u8 ZipPagData(u8 TradeType,u8 *DOLData,u8 *DataLen,u8 TradeOkButDecline,u8 SrciptResult)
��������: ��֯���ͱ��ĵ�55��
��ڲ���: u8 TradeType:��������
		  u8 TradeOkButDecline:���׳ɹ������ܾ��ĳ��� =1 =0��һ�����
		  u8 *DOLData:�õ�������
		  u8 *DataLen:�õ������ݳ���
		  u8 SrciptResult�Ƿ��нű����= 1�У�=0��
�� �� ֵ: u8 �ɹ��ڷ�OK�ɹ�.ERR��
��ص���:
��    ע: 
�޸���Ϣ:         
********************************************************************/
u8 ZipPagData(u8 TradeType, u8 *DOLData, u16 *DataLen) {
    u8 i, temp[5], tagdata[100], *ptr, destag[200];
    u32 taglen, destaglen = 0;
    s32 result;
    *DataLen = 0;
    if (NULL == DOLData || NULL == DataLen) {
        return UNSUCCESS;
    }
    for (i = 0; i < MAXTAGNUM; i++) {
        memset(temp, 0, sizeof(temp));
        if (TradeType)//�˴��޸�Ϊ�������̵�tag
        { strcpy(temp, &(TradeTagList2[i].Tag[0])); }
        else { strcpy(temp, &(TradeTagList[i].Tag[0])); }
        if (strlen(temp) == 0) break;

        memset(tagdata, 0, sizeof(tagdata));
        taglen = 0;

        result = sdkIccReadTLV(temp, tagdata, &taglen);
        Trace("wwzj", "ReadTLV RLT:%d\r\n", result);

        if (SDK_PARA_ERR == result) return UNSUCCESS;
        if (SDK_EQU == result) continue;
        destaglen = 0;
        result = FormTLV(destag, &destaglen, temp, tagdata, &taglen);
        if (SUCCESS != result) return UNSUCCESS;

        ptr = DOLData + (*DataLen);
        memcpy(ptr, destag, destaglen);
        (*DataLen) += destaglen;
        /*
        #ifdef AFX_DEBUG
        printf("\r\n-----------i=[%d]---------\r\n",i);
        printf("\r\n--��ȡ����ֵtagdata=");
        printfbuf(tagdata,taglen);
        
        printf("\r\n-----------DataLen=[%d]---------\r\n",*DataLen);
          printfbuf(ptr,*DataLen);
        #endif
        */
    }
    return SUCCESS;
}

/*******************************************************************
��    ��: ������
��    Ȩ: �����¹����ɷ����޹�˾
��������: ��ʾ��������
��ڲ���: 
�� �� ֵ: 
��    ע: 
********************************************************************/
void DispInputCreditPwd(u8 PINTryCount, u8 IccEncryptWay) {
    u8 row, TradeAmountTemp[32];

    sdkDispClearScreen();
    row = SDK_DISP_LINE2;

    if (IccEncryptWay == ICCOFFLINEPIN) {
        sdkDispFillRowRam(row, 0, "�������ѻ�����",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        if (PINTryCount == 1) {
            sdkDispFillRowRam(row + 1, 0, "��ֻ�����һ�λ���",
                              SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
        }
    }
    else {
        if (memcmp(gucTradeAmount, "\x00\x00\x00\x00\x00\x00", 6)) {
            memset(TradeAmountTemp, 0, sizeof(TradeAmountTemp));
            RegulateMoney(TradeAmountTemp, gucTradeAmount);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���:", SDK_DISP_FDISP | SDK_DISP_LDISP);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, TradeAmountTemp, SDK_DISP_FDISP | SDK_DISP_RDISP);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, "��ֿ�����������:",
                              SDK_DISP_FDISP | SDK_DISP_LDISP);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ֿ�����������:",
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ�ϼ�����...",
                              SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
        }
    }
    sdkDispBrushScreen();
}

/*******************************************************************
��    ��: ������
��    Ȩ: �����¹����ɷ����޹�˾
��������: ������������
��ڲ���: 
�� �� ֵ: RSLT_OK: ȷ������(����������),���������IC���ѻ�����,��Ϊ��������.
		   RSLT_ESC: �û�ȡ����������,�����������볬ʱ
��    ע: ��Ϊ�ѻ���������ʱ,HaveCreditPwdͬ��Ϊ1.
********************************************************************/
s32 InputCreditPwd(const u8 *pTradeAmount, u8 PINTryCount, u8 IccEncryptWay, u8 *pCreditPwd) {
    if (ICCONLINEPIN == IccEncryptWay) {
        pCreditPwd[0] = 0x06;
        return SDK_OK;
    }

    SDK_PED_PIN_CFG PinCfg;
    u8 bcAmount[6] = {0};
    u8 ucCardNo[8] = {0};
    //u8 ucTmp[32] = {0};
    u8 CardNoLen;
    u8 row;
    s32 ret;
    s32 timer = sdkTimerGetId();            //20150323 ��ӳ�ʱ����

    sdkAscToBcd(bcAmount, pTradeAmount, 12);
    Trace("Mai", "\n\n pTradeAmount = %s \r\n", pTradeAmount);
    TraceHex("Mai", "bcAmount", bcAmount, 6);
    memset(AppCreditPwd, 0xFF, 16);

#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
    sdkDispClearScreen();
#endif
    row = SDK_DISP_LINE2;


    if (IccEncryptWay == ICCOFFLINEPIN) {
        sdkDispFillRowRam(row, 0, "�������ѻ�����",
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        if (PINTryCount == 1) {
            sdkDispFillRowRam(row + 1, 0, "��ֻ�����һ�λ���",
                              SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
        }
    }
    else {
#if PBOCCLEARSCREEN_MODE            //20150403 PBOC���Ƿ�����
        if(memcmp(bcAmount, "\x00\x00\x00\x00\x00\x00",6))
        {
           memset(ucTmp,0,sizeof(ucTmp));
           strcpy(ucTmp, "���:");
           RegulateMoney(&ucTmp[5],bcAmount);
		   Trace("Mai", "ucTmp = %s\n", ucTmp);
		   sdkDispFillRowRam(SDK_DISP_LINE2, 0, ucTmp, SDK_DISP_FDISP|SDK_DISP_RDISP);
           //sdkDispFillRowRam(SDK_DISP_LINE1, 0, "���:", SDK_DISP_FDISP|SDK_DISP_LDISP);
           //sdkDispFillRowRam(SDK_DISP_LINE2, 0, ucTmp, SDK_DISP_FDISP|SDK_DISP_RDISP);
           //sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ֿ�����������:", SDK_DISP_FDISP|SDK_DISP_LDISP);
        }
        else
        {
           //sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ֿ�����������:", SDK_DISP_FDISP|SDK_DISP_LDISP|SDK_DISP_INCOL);
           //sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ�ϼ�����...", SDK_DISP_FDISP|SDK_DISP_RDISP|SDK_DISP_INCOL);
        }
#else
        sdkDispClearRow(SDK_DISP_LINE2);
        sdkDispClearRow(SDK_DISP_LINE3);
        sdkDispClearRow(SDK_DISP_LINE4);
        sdkDispClearRow(SDK_DISP_LINE5);
#endif          //20150403 PBOC���Ƿ�����
        sdkDispFillRowRam(SDK_DISP_LINE3, 0, "����������:", SDK_DISP_FDISP | SDK_DISP_LDISP);
        if (SDK_ICC_ICC == GetCurrentCardType())  //20150330 IC��ʱ��ʾ��ο���������ʾ���ƿ�
        {
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "��ο�", SDK_DISP_FDISP | SDK_DISP_RDISP);
        }
        else {
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ƿ�", SDK_DISP_FDISP | SDK_DISP_RDISP);
        }
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, "�������밴ȷ�ϼ�", SDK_DISP_FDISP | SDK_DISP_LDISP);
    }
    sdkDispBrushScreen();
    memset(&PinCfg, 0, sizeof(SDK_PED_PIN_CFG));
    PinCfg.ePinMode = IccEncryptWay;

    PinCfg.eKeyType = SDK_PED_DES_TRIPLE;

    PinCfg.ucTpkIndex = 1;                //Mai test index
    sdkIccReadTLV("5A", ucCardNo, &ret);
    TraceHex("Mai", "Pan", ucCardNo, ret);
    CardNoLen = ret;
    SavePan(ucCardNo, CardNoLen);
    GetPan(PinCfg.hePan);
    PinCfg.ucRow = SDK_DISP_LINE4;
    PinCfg.iTimeOut = SDK_ICC_TIMER_AUTO;

    while (1) {
        ret = sdkPEDInputPIN(&PinCfg, bcAmount, pCreditPwd);
        if (sdkTimerIsEnd(timer, SDK_ICC_TIMER_AUTO))        //20150323 ��ӳ�ʱ����
        {
            gucPBOCTransFlg = SDK_TIME_OUT;
            return SDK_TIME_OUT;
        }
        if (ret == SDK_PED_NOPIN) {
            continue;
        }
        else {
            break;
        }
    }

    Trace("Mai", "sdkPEDInputPIN: ret:%d\r\n", ret);

    if (ret == SDK_OK) {
        memset(AppCreditPwd, 0xFF, 16);
        memcpy(AppCreditPwd, &pCreditPwd[1], 8);

    }
    else if (ret == SDK_PED_NOPIN) {
        memset(AppCreditPwd, 0x00, sizeof(AppCreditPwd));
        memset(AppCreditPwd, 0xFF, 16);
        ret = SDK_OK;
    }
    else {
        memset(AppCreditPwd, 0xFF, 16);
    }

    //DisProcessing();      //20150403 ȥ����ʾ
    return ret;
}

/*******************************************************************
��	  ��: 
��	  Ȩ: �����¹����ɷ����޹�˾
��������: 
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
void UnionMangeMenu()  //zjz20091207
{
    s32 key;
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "1.��ѯ��Կ",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "2.��ѯAID",
                      SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
    key = sdkKbWaitKey(SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_ESC, GetTimerAutoRetValue());
    switch (key) {
        case SDK_KEY_1:        //��ѯ��Կ
            if (AppTermCAPKNum == 0) {
                sdkIccGetCAPKListNum(&AppTermCAPKNum);
                sdkIccGetAnyCAPKList(0, AppTermCAPKNum, AppTermCAPK, &AppTermCAPKNum);
            }
            if (AppTermCAPKNum > 0) {
                UnionMangeQueryCapk();
            }
            else {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ն���û�й�Կ",
                                  SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkmSleep(600);
            }
            UnionMangeMenu();
            break;
        case SDK_KEY_2:        //��ѯAID
            if (AppAidParamNum == 0) {
                sdkIccGetAIDListNum(&AppAidParamNum);
                sdkIccGetAnyAIDList(0, AppAidParamNum, AppAidParam, &AppAidParamNum);
            }
            if (AppAidParamNum > 0) {
                EmvSelAid(0);                  //ֻ�ܿ�AID
            }
            else {
                sdkDispClearScreen();
                sdkDispFillRowRam(SDK_DISP_LINE2, 0, "�ն���û��AID",
                                  SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
                sdkDispBrushScreen();
                sdkmSleep(600);
            }
            UnionMangeMenu();
            break;
        case SDK_KEY_ESC:
            //   MainFace();
            break;
        default:
            MainFace();
            break;
    }
}

/*******************************************************************
��	  ��: 
��	  Ȩ: �����¹����ɷ����޹�˾
��������: 
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
void UnionMangeQueryCapk() //������������ʾ��Կzjz20091207
{
    u16 i = 0;
    u8 key = 0;

    DispCapk(i, 0);
    while (1) {
        if (i == AppTermCAPKNum) {
            break;
        }
        else {

            key = sdkKbGetKey();
            if (key > 0) {
                if (key == SDK_KEY_ENTER || key == SDK_KEY_DOWN) {
                    i++;
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                }
                else if (key == SDK_KEY_UP) {
                    if (i > 0) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        i--;
                    }
                    else {
                        sdkSysBeep(FALSE);
                    }
                }
                else if (key == SDK_KEY_ESC) {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    break;
                }
                else if (key > 0) {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
                DispCapk(i, 0);
            }

        }
    }
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��ʾ��Կ
��ڲ���: index ��Կ����
                       isDown = 1 ���� isDown = 0 ֻ�ǲ�ѯ
�� �� ֵ: 
��	  ע:   zjz20091207
********************************************************************/
void DispCapk(u8 index, u8 isDown) {
    u8 temp[256], buf[256];
    u8 *pX;

    sdkDispClearScreen();
    memset(temp, 0, sizeof(temp));
    if (isDown) {
        sprintf(temp, "���µ�%d�鹫Կ�ɹ�", index + 1);
    }
    else {
        sprintf(temp, "��%d�鹫Կ", index + 1);
    }
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, temp, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    memset(temp, 0, sizeof(temp));
    strcpy(temp, "RID:");
    memset(buf, 0, sizeof(buf));
    pX = AppTermCAPK[index].RID;
    if (GetInterCode(buf, pX) == ERR) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%02X,%02X,%02X,%02X,%02X",
                pX[0], pX[1], pX[2], pX[3], pX[4]);
    }
    strcat(temp, buf);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

    memset(temp, 0, sizeof(temp));
    sprintf(temp, "����: %02X", AppTermCAPK[index].CAPKI);
    sdkDispFillRowRam(SDK_DISP_LINE4, 0, temp, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

    memset(temp, 0, sizeof(temp));
    pX = AppTermCAPK[index].ExpireDate;
    sprintf(temp, "��Ч��: %02X%02X/%02X/%02X", pX[0], pX[1], pX[2], pX[3]);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, temp, SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    sdkDispBrushScreen();
}

/*******************************************************************
��	  ��: 
��	  Ȩ: �����¹����ɷ����޹�˾
��������: 
��ڲ���:
�� �� ֵ:
��	  ע:
********************************************************************/
u8 GetInterCode(u8 *InterCode, u8 *rid) {
    u8 i;

    for (i = 0; i < 4; i++) {
        if (!memcmp(RidTab[i], rid, 5)) {
            memcpy(InterCode, InterCodeTab[i], 3);
            return OK;
        }
    }
    return ERR;
}

/*******************************************************************
��	  ��: �Ž���
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��ʾAID�б�
��ڲ���: 	start �ӵڼ��鿪ʼ��ʾ  ��Сֵ��0
				count ��ҳ��Ҫ��ʾ��AID����  0 < count < 5
�� �� ֵ: 
��	  ע: 	AidParamNum�����е�AID������ȫ�ֱ�����xzs����  
			AidParam��   AID�б�ṹ���飬ȫ�ֱ�����xzs����  
			SysSelAid�� ��ѡ�е�AID�б�(��Сֵ��0),ѡ�е�AID�б����AidParam[SysSelAid]       
********************************************************************/
void DisplayAid(u8 start, u8 count) //xule120306 ȥ�����¼�ͷ,ʹ��ԭ����ʾAID����
{
    u8 buf[40] = {0};
    u8 i = 0;
    u8 aidLen;

    sdkDispClearRow(SDK_DISP_LINE2);
    sdkDispClearRow(SDK_DISP_LINE3);
    sdkDispClearRow(SDK_DISP_LINE4);
    sdkDispClearRow(SDK_DISP_LINE5);
    //LcdClearMulRow(SDK_DISP_LINE2, SDK_DISP_LINE5);
    for (i = 0; i < count; i++) {
        memset(buf, 0, sizeof(buf));
        buf[0] = 0x31 + i;
        buf[1] = '.';
        aidLen = AppAidParam[start + i].AidLen; //����Aid�ж��ٸ��ֽ�
        if (aidLen > 16)aidLen = 16;
        sdkBcdToAsc(&buf[2], AppAidParam[start + i].Aid, aidLen); //��AIDת����ASC����ʾ
        if (strlen(buf) > 21) //һ����ʾ����
        {
            memcpy(&buf[18], "...", 3);  //��ʾʡ�Ժ�
            buf[21] = 0; //�ַ�������
        }
        sdkDispFillRowRam(SDK_DISP_LINE2 + i, 0, buf, SDK_DISP_FDISP | SDK_DISP_LDISP);  //��ʾ
    }
    sdkDispBrushScreen();   //��ʾ
}

/*******************************************************************
��	  ��: �Ž���
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ѡ��AID�б�
��ڲ���: enSet = 1 �ܽ�ȥ�޸�AID��Ӧ�Ĳ���
			             = 0 ֻ�ܲ鿴AID�����ܽ����޸ĺ���
�� �� ֵ: 
��	  ע: �����޸�
********************************************************************/
void EmvSelAid(u8 enSet) {
    u8 key, MaxKey;  //��ֵ
    u8 count, start;
    u8 LcdFlag;

    sdkKbKeyFlush(); //������̻���
    sdkDispClearScreen();
    if (enSet) //ϵͳ������
    {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ѡ��AID�б�",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    }
    else {
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, "AID�б�",
                          SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    }
    sdkDispBrushScreen();   //��ʾ
    sdkKbKeyFlush(); //������̻���

    LcdFlag = 1; //��1ҳ��ʼ������ע��AID�б�ĵ�0�鿪ʼ
    SysSelAid = 0; //ѡ�е��ǵڼ���AID
    if (AppAidParamNum < 5) count = AppAidParamNum; //AID�б�ĸ���С�� 5  ��һҳ�Ϳ�����ʾ��
    else count = 4; //Ҫ���Ƿ�ҳ
    MaxKey = 0x30 + count; //ÿһҳ�пɰ���������1��4��
    DisplayAid(0, count); //��0�鿪ʼ��ʾAID

    while (1) {
        key = sdkKbGetKey();
        if (key != 0) {
            if (key == SDK_KEY_ESC) {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                /*			if(enSet) //ϵͳ����								wlz modifi 2014-5-30
                            {
                                //PostTraFunMenu(UP);��ֲ��SPOS�ϣ��������ϵͳ����
                                ;
                            }
                            else     //���������в�AID
                            {
                                UnionMangeMenu();
                            }*/
                break;
            }
            else if (key == SDK_KEY_UP) {
                if (LcdFlag > 1)//���Է�ҳ
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    LcdFlag--;
                    start = (LcdFlag * 4) - 4; //�õ���һҳ�ӵڼ��鿪ʼ��ʾ��AID�б�
                    DisplayAid(start, 4); //��start�鿪ʼ��ʾ4��AID
                    MaxKey = 0x30 + 4;  //ÿһҳ�пɰ���������1��4��
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
            }
            else if (key == SDK_KEY_DOWN) {
                if (LcdFlag * 4 < AppAidParamNum) //���Է�ҳ
                {
                    sdkSysBeep(SDK_SYS_BEEP_OK);
                    start = (LcdFlag * 4); //�õ���һҳ�ӵڼ��鿪ʼ��ʾ��AID�б�
                    if ((AppAidParamNum - 1 - start) >
                        4) //AidParamNum����Сֵ��1, start����Сֵ��0,ÿҳֻ����ʾ4��
                        count = 4;
                    else
                        count = AppAidParamNum - start;
                    MaxKey = 0x30 + count;  //ÿһҳ�пɰ���������1��4��
                    LcdFlag++;
                    DisplayAid(start, count); //��start�鿪ʼ��ʾcount��AID
                }
                else {
                    sdkSysBeep(SDK_SYS_BEEP_ERR);
                }
            }
            else if ((key >= SDK_KEY_1) && (key <= MaxKey)) //ѡ����AID�б�
            {
                sdkSysBeep(SDK_SYS_BEEP_OK);
                /*��ֲ��SPOS�ϣ��������ϵͳ����
        if(enSet)  //ϵͳ����
        {
            if(LcdFlag == 1) //�ڵ�һҳ��ѡ����AID
            {
                SysSelAid = (key - 0x30) -1;
            }
            else  //���ǵ�һҳ��ѡ����AID
            {
                SysSelAid = ((LcdFlag - 1) * 4) + (key - 0x30) - 1;
            }
            AidToPosTermInfo(SysSelAid); //��AID�ṹ�����еĲ���ת��TermInfo�ṹ��
            PostEmvSetMenu();
            break;
        }
        */
            }
            else {
                sdkSysBeep(SDK_SYS_BEEP_ERR);
            }
        }
    }
}


/**************************************Function Description *****************************************
** ��������:void InitPadRfPara(u8 type)
** ����:    ��ʼ��������̺����÷ǽ�
** �������:
                type=1������ϵͳ������ʱ�����³�ʼ����
                type=0      �ϵ��ʱ�򣬵�һ�γ�ʼ��
** �������:N/A
** ����ֵ:N/A
** ����: jane
** ��������: 2012-11-06
** ����: 
**
*******************************************************************************************/
void InitPadRfPara(u8 type) {
    SDK_SYS_INITIAL_INFO GpadRfinfo;
    //��������������̵�ʱ��������÷ǽӵ�ʱ����Ҫ���²�����SDK
    GpadRfinfo.bIsHavePinPad = TRUE;
    GpadRfinfo.bIsINRf = TRUE;        //Ĭ�ϲ������÷ǽ�

    Trace("wlz", "GpadRfinfo.bIsHavePinPad %d===\r\n", GpadRfinfo.bIsHavePinPad);
    Trace("wlz", "GpadRfinfo.bIsINRf %d===\r\n", GpadRfinfo.bIsINRf);

    if (type) {
        sdkSysReInitParam(&GpadRfinfo);
        sdkIccPowerStartReader();
    }
    else {
        sdkSysInitParam(&GpadRfinfo);
        sdkIccPowerStartInit();
    }
}

