#define  MAXIDTAG   4
#define  GTAGLISTMUN 5//   6

struct _CurrencyName CurrencyNameTab[] =    //zcl 20111028 ���Ҵ����
        {
                {{"344"}, {"HKD"}},    //�۱�
                {{"156"}, {"RMB"}},    //�����
                {{"910"}, {"EUR"}},        //ŷԪ
                {{"826"}, {"GBP"}},    //Ӣ��
                {{"764"}, {"THP"}},    //̩��
                {{"392"}, {"JPY"}},    //��Ԫ
                {{"978"}, {"EUR"}},    //ŷԪ
                {{"840"}, {"USD"}},    //��Ԫ
                {{"280"}, {"DEM"}},    //�¹����
                {{"250"}, {"FRF"}},    //����
                {{"036"}, {"AUD"}},    //�Ĵ����ǰ�Ԫ
                {{"076"}, {"BRC"}},    //���� ��³����
                {{"810"}, {"SUR"}},    //����˹¬��
                {{"608"}, {"PHP"}},    //���ɱ� �Ʊ���
                {{"246"}, {"FIM"}},    //�������
                {{"528"}, {"NLG"}},    //������
                {{"124"}, {"CAD"}},    //���ô�Ԫ
                {{"414"}, {"KWD"}},    //�����ؿ����ص��ɶ�
                {{"724"}, {"ESP"}},    //������������������
                {{"620"}, {"PTE"}},    //��������������˹���
                {{"410"}, {"KPW"}},    //�ϳ���Ԫ
                {{"710"}, {"ZAR"}},    //�Ϸ� ����
                {{"578"}, {"NOK"}},    //Ų������
                {{"752"}, {"SEK"}},    //������
                {{"756"}, {"CHF"}},    //��ʿ����
                {{"702"}, {"SGD"}},    //�¼���Ԫ
                {{"554"}, {"NED"}},    //������Ԫ
                {{"528"}, {"NLG"}},    //������
                {{"056"}, {"BEF"}},    //����ʱ����
                {{"380"}, {"ITL"}},    //���������
                {{"356"}, {"INR"}},    //ӡ��¬��
                {{"360"}, {"IDR"}},    //ӡ��¬��
                {{"300"}, {"GRD"}},    //ϣ�� ��������
                {{"446"}, {"MOP"}},    //����Ԫ
                {{"901"}, {"TWD"}},    //��̨��Ԫ
                {{"040"}, {"ATS"}},    //�µ��� ����
                {{"208"}, {"DKK"}},    //�������
                {{"818"}, {"EGP"}},    //������
                {{"368"}, {"IQD"}},    //�����˵��ɶ�
                {{"364"}, {"IRR"}},    //�������Ƕ�
                {{"682"}, {"SAR"}},    //ɳ�����Ƕ�
                {{"784"}, {"AED"}},    //������ UAE����ķ
                {{"458"}, {"MYR"}},    //���������ּ���
                {{"949"}, {"TRY"}},     //����
                {{"376"}, {"ILS"}},        //��л
                {{"643"}, {"RUB"}}      //����˹ ¬��
        };

/**************************************Function Description *****************************************
** ��������:s32  DisReaptSel()
** ����:  ������ʾ����Ӧ��ѡ�񣬹��û�ѡ��
** �������:
** �������:
** ����ֵ:
                            SDK_OK  ȷ�����ɹ��Ƴ�
                            SDK_ESC ȡ���Ƴ�
                            SDK_TIME_OUT ��ʱ�Ƴ�
** ����:
** ��������:
**
*******************************************************************************************/
s32 DisReaptSel() {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "������ѡ��Ӧ��", SDK_DISP_FDISP | SDK_DISP_CDISP);
    sdkDispBrushScreen();
    return SDK_OK;
    //  return WaitEnterEsc();//��Ҫ����ȷ��
}

/**************************************Function Description *****************************************
** ��������:s32  LoadInputpw
** ����:  ��ʾ���ţ����ȴ��û�ȷ�Ͽ���
** �������:type=0 ָ���˻�Ȧ��
                            type=1����ָ���˻�Ȧ��
** �������:
** ����ֵ:
                            SDK_OK  ȷ�����ɹ��Ƴ�
                            SDK_CancelTransErr ȡ���Ƴ�
** ����:
** ��������:
**
*******************************************************************************************/
s32 LoadInputpw(u8 type) {
/*    u8 temp[20]={0},tempcvmr[20]={0},CVMResult[128]={0},tvr[6]={0},tsi[3]={0},cvmlist[128]={0};
    s32 result=0,tvr_len=0,tsi_len=0,cvmlist_len=0, i;
    memset(temp,0,sizeof(temp));
    result=InputCreditPwd(IccEmvParam.pasTradeAmount,0,ICCONLINEPIN,temp);
    if(SDK_OK!=result)  return SDK_CancelTransErr;

    if(!type)
    {
    //ָ���ʻ�Ȧ��ʱ����Ҫ����һ����Ϊ��������ж�����Ϊ����ָ���˻�Ȧ��&&����������&&EMV���������cmvlist&&cmvlist�����ݳ��ȴ��ڵ���10

        if(GLoadType == ECQCACCITEM && HaveCreditPwd == 1 && sdkIccReadTLV("\x8E",cvmlist,&cvmlist_len)==SDK_OK && cvmlist_len >=10)          //zcl 20111223 CVMlist
        {
            memcpy(tempcvmr, "\x42\x00\x00", 3);
            memcpy(CVMResult, tempcvmr, 3);
            if(sdkIccReadTLV("\x95", tvr, &tvr_len)==SDK_OK)
            {
                tvr[2] |= 0x04;        
                sdkIccConfigTLV("\x95", tvr, 5);
            }
            if(sdkIccReadTLV("\x9B", tsi, &tsi_len)==SDK_OK)
            {
                tsi[0] |= 0x40;
                sdkIccConfigTLV("\x9B", tsi, 2);
            }
            for(i = 8; i < cvmlist_len; i = i + 2)
            {
                memcpy(tempcvmr, &cvmlist[i], 2);

                if((tempcvmr[0] & 0x02) == 0x02)
                {
                    sdkIccConfigTLV("\x9F\x34", tempcvmr, 3);
                    break;
                }
            }
        }
    }
    return result;
    */
    return SDK_OK;
}

/**************************************Function Description *****************************************
** ��������:s32  ConfirmCardNo()
** ����:  ��ʾ���ţ����ȴ��û�ȷ�Ͽ���
** �������:
** �������:
** ����ֵ:
                            SDK_OK  ȷ�����ɹ��Ƴ�
                            SDK_ESC ȡ���Ƴ�
                            SDK_TIME_OUT ��ʱ�Ƴ�
** ����:
** ��������:
**
*******************************************************************************************/
s32 ConfirmCardNo(const u8 *pasPAN) {
    u8 cardno[50];
    s32 result;

    if (GLoadType == ECQCACCNOITEM && ConfirmINS45 != 0)//���Ƿ�ָ���˻�Ȧ�棬Ҫ����ת��������
    {
        result = LoadInputpw(1);
        if (SDK_OK != result) return result;//ȡ�����߳�ʱҪ�˳�
    }
    memset(cardno, 0, sizeof(cardno));
    strcpy(cardno, pasPAN);
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ȷ�Ͽ���:", SDK_DISP_FDISP | SDK_DISP_LDISP);
    sdkDispFillRowRam(SDK_DISP_LINE3, 3, cardno, SDK_DISP_FDISP | SDK_DISP_RDISP);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ�ϼ�����,ȡ��������", SDK_DISP_FDISP | SDK_DISP_RDISP);
    sdkDispBrushScreen();
    //result=WaitEnterEsc();   //  return WaitEnterEsc();//��Ҫ����ȷ��
    Trace("wlz", "==result %d ==\r\n", result);
    if (SDK_KEY_ENTER != result) return SDK_ERR;//ȡ�����߳�ʱҪ�˳�

    if (GLoadType == ECQCACCITEM)//����ָ���˻�Ȧ�棬Ҫ��������
    {
        result = LoadInputpw(0);
        if (SDK_OK != result) return SDK_ERR;//ȡ�����߳�ʱҪ�˳�
    }
    return SDK_OK;
}

/**************************************Function Description *****************************************
** ��������:s32  ConfirmCardNo()
** ����:  ��ʾ���ţ����ȴ��û�ȷ�Ͽ���
** �������:
** �������:
** ����ֵ:
                            SDK_OK  ȷ�����ɹ��Ƴ�
                            SDK_ESC ȡ���Ƴ�
                            SDK_TIME_OUT ��ʱ�Ƴ�
** ����:
** ��������:
**
*******************************************************************************************/
s32 DisMovCardAway() {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ƿ���Ƭ", SDK_DISP_FDISP | SDK_DISP_CDISP);
    sdkDispBrushScreen();
    return SDK_OK;
}

/**************************************Function Description *****************************************
** ��������:void TradeParmConfig(ST_TRANSFLOW *pstTransFlow)	
** ����:  ����IC���������ͳ�ʼ����������
** �������:  TransFlow���׽ṹ��
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
void TradeParmConfig(ST_TRANSFLOW *pstTransFlow) {
    pstTransFlow->InitIcTranStu.TransType = pstTransFlow->TradeType;
    Trace("wlz", "TradeParmConfig pstTransFlow->TradeType %d\r\n", pstTransFlow->TradeType);
    switch (pstTransFlow->TradeType) {
        case TRADE_QUERY:
            pstTransFlow->InitIcTranStu.EnableFallback = true;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = false;
            pstTransFlow->InitIcTranStu.BIsSupportEc = false;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = false;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_ALL_FLOW;
            break;
        case TRADE_SALE:
            pstTransFlow->InitIcTranStu.EnableFallback = true;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = false;
            pstTransFlow->InitIcTranStu.BIsSupportEc = false;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = false;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_ALL_FLOW;
            break;
        case TRADE_UNSALE:    // TRADE_REFUND TRADE_UNPREAUTH TRADE_UNCOMPLETE
            pstTransFlow->InitIcTranStu.EnableFallback = true;
            Trace("wlz", "TRADE_UNSALE EnableFallback %d\r\n",
                  pstTransFlow->InitIcTranStu.EnableFallback);
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = false;
            pstTransFlow->InitIcTranStu.BIsSupportEc = false;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = false;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_EASY_FLOW;
            break;
        case TRADE_DESGNLOAD:
            pstTransFlow->InitIcTranStu.EnableFallback = false;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = true;
            pstTransFlow->InitIcTranStu.BIsSupportEc = true;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = true;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_EC_LOAD;
            break;
        case TRADE_LOAD:
            pstTransFlow->InitIcTranStu.EnableFallback = true;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = false;
            pstTransFlow->InitIcTranStu.BIsSupportEc = true;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = true;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_EASY_FLOW;
            break;
        case TRADE_ECCASH:    //TRADE_MAGLOAD_AFFIRM
            pstTransFlow->InitIcTranStu.EnableFallback = false;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = true;
            pstTransFlow->InitIcTranStu.BIsSupportEc = true;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = true;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_EC_LOAD;

            break;
        case TRADE_UNECCASH:
            pstTransFlow->InitIcTranStu.EnableFallback = true;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = true;
            pstTransFlow->InitIcTranStu.BIsSupportEc = true;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = true;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_EC_LOAD;

            break;
        case TRADE_MAGLOAD_VERIFY:
            break;
        default:

            pstTransFlow->InitIcTranStu.EnableFallback = true;
            pstTransFlow->InitIcTranStu.ForceOnline = true;
            pstTransFlow->InitIcTranStu.EnableQPBOC = false;
            pstTransFlow->InitIcTranStu.EnableRFPBOC = false;
            pstTransFlow->InitIcTranStu.BIsSupportEc = false;
            pstTransFlow->InitIcTranStu.BIsPromptChooseEc = false;
            pstTransFlow->InitIcTranStu.BIsDefaultEc = false;
            pstTransFlow->InitIcTranStu.EcFlowType = SDK_ICC_ALL_FLOW;
            break;
    }
}


/**************************************Function Description *****************************************
** ��������:void InitTradeParam(SDK_ICC_TRADE_PARAM *IccEmvParam,struct ST_TRANSFLOW *TransFlow)
** ����:  ��ʼ��IC�����ײ���
** �������:
                            IccEmvParam  SDK_ICC_TRADE_PARAM�ṹ��
                            TransFlow���׽ṹ��
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
void InitTradeParam(SDK_ICC_TRADE_PARAM *pIccEmvParam, ST_TRANSFLOW *TransFlow) {
    pIccEmvParam->UserIccTransInit = NULL;
    pIccEmvParam->InputAmount = NULL;
    pIccEmvParam->DispReselectApp = DisReaptSel;
    pIccEmvParam->DispCardOut = DisMovCardAway;
    pIccEmvParam->VerifyCardNo = NULL;//ConfirmCardNo;
    pIccEmvParam->InputPWD = InputCreditPwd;
    pIccEmvParam->DispOnlineRandNum = NULL;
    pIccEmvParam->DispChooseEC = NULL;
    pIccEmvParam->VerifyIDCard = NULL;
    pIccEmvParam->DispCandAppList = NULL;
    pIccEmvParam->DispPromptData = NULL;

    TradeParmConfig(TransFlow);

    pIccEmvParam->bIsForceOnline = (bool) (TransFlow->InitIcTranStu.ForceOnline);
    pIccEmvParam->bIsFallback = (bool) (TransFlow->InitIcTranStu.EnableFallback);
    pIccEmvParam->bIsSupportQPBOC = (bool) (TransFlow->InitIcTranStu.EnableQPBOC);
    pIccEmvParam->bIsSupportCLPBOC = (bool) (TransFlow->InitIcTranStu.EnableRFPBOC);
    pIccEmvParam->bIsSupportEc = (bool) (TransFlow->InitIcTranStu.BIsSupportEc);
    pIccEmvParam->bIsPromptChooseEc = (bool) (TransFlow->InitIcTranStu.BIsPromptChooseEc);
    pIccEmvParam->bIsDefaultEc = (bool) (TransFlow->InitIcTranStu.BIsDefaultEc);

    pIccEmvParam->eFlowMode = TransFlow->InitIcTranStu.EcFlowType;
}

/**************************************Function Description *****************************************
** ��������:u8 DealBlushCard(struct ST_TRANSFLOW *TransFlow)
** ����:  ����ˢ������֮ǰ�Ѿ������������ݣ��ʲ��ٶ���ͷ

** �������:
                            TransFlow���׽ṹ��
** �������:
** ����ֵ:
                            E_NEED_ICC  תIC������
                            E_MAG_EXIT  �˳�
                            E_MAG_ERR   �ŵ����ݼ��ܴ���
** ����:
** ��������:
**
*******************************************************************************************/
s32 DealBlushCard(ST_TRANSFLOW *TransFlow) {
#if 0
    u8 result,CardData[300],cardno[50];
    u16 CardLen = 0,cardnoLen = 0;

    if((pstCardInfo.stCardData.bIsIccMagCard)&&(TransFlow->bFallback==0))
    {
  /*      if(gstAppSetting.BIsEnableIC ==1)//����֧�ּ��IC������
        {
            return E_NEED_ICC;//��IC���Ļ�ֱ�ӽ������IC������
        }*/
    }
    Trace("wlz","\r\n----���������--------\r\n");
    TransFlow->bOnline=1;//���������������ó�����

	AppGetMagCardNo(&pstCardInfo.stCardData, cardno);
	if(strlen(cardno)<13) 
	{
	    Trace("wlz","-���������淶E_MAG_ERR---\r\n");
	    return E_MAG_ERR;  //  ���ų���С��ʮ����ˢ������
	}
    memset(CardData,0,sizeof(CardData));
    cardnoLen = strlen(cardno);
    MagDataFormat(pstCardInfo.stCardData, 1,CardData,&CardLen);   
    TraceHex("AppDebug","======������=========\r\n",CardData,CardLen);
    memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));

    result = EncryptData(&TransFlow->heInsOutputData[3+ cardnoLen],CardData,CardLen);
    if ( result == SDK_OK)//�ŵ�������ȷ
    {  
       	result=ConfirmCardNo(cardno);
		if(SDK_OK!=result)  return E_MAG_EXIT;
         SavePan(cardno,cardnoLen);//���濨PAN
		if(TransFlow->bFallback)  
            TransFlow->heInsOutputData[1]=0x02;
		else 
            TransFlow->heInsOutputData[1]=0x00;
		TransFlow->heInsOutputData[2]= cardnoLen;
		memcpy(&TransFlow->heInsOutputData[3],cardno, cardnoLen );
		TransFlow->heInsOutputData[0]=CardLen+1+cardnoLen + 1;
		TransFlow->usInsOutputDataLen=TransFlow->heInsOutputData[0]+1; 
    }
    else
    {
        return E_MAG_ENP;
    }
#endif
    return E_MAG_OK;
}


/**************************************Function Description *****************************************
** ��������:u8 InputExpired()
** ����:            �����뿨��
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
s32 InputExpired(void) {
    /*  u8 key,SysTemp[MAX_STR_LEN+2] = {0},temp[5]={0},len=0;
      bool result;

      OpenTimer(&TimerAutoRet,TV_AUTORET);
      dev_lcd_clear_screen();
      dev_lcd_fill_row_ram(SDK_DISP_LINE1,0,"�����뿨��Ч��",SDK_DISP_FDISP|SDK_DISP_LDISP|SDK_DISP_INCOL);
      dev_lcd_fill_row_ram(SDK_DISP_LINE2,0,"(��������):",SDK_DISP_FDISP|SDK_DISP_LDISP|SDK_DISP_INCOL);
      dev_lcd_brush_screen();
      while(1)
      {
          key=GetScanf(SysTemp, 0, 4, MMI_NUMBER, SDK_DISP_LINE5);
          if(key==ENTER)
          {
              len=SysTemp[0];
              if(len==0)
              {
                  MagCardData.HaveExpired=0;
                  memset(MagCardData.ExpiredDate,0,sizeof(MagCardData.ExpiredDate));
                  return TRUE;
              }
              else if(len==4)
              {
                  SysTemp[len+1] = 0;
                  memset(temp,0,sizeof(temp ));
                  AscToBcd(temp, &SysTemp[1], len);
                  result=JudgeTime(temp,2);
                  if(result)
                  {
                      MagCardData.HaveExpired=1;
                      memcpy(MagCardData.ExpiredDate,temp,2);
                      return TRUE;
                  }
                  else
                  {
                      memset(SysTemp, 0, sizeof(SysTemp));
                      SysTemp[0] = 0x80;
                      strcpy(&SysTemp[1], "����Ч�ڸ�ʽ����");
                  }
              }
              else
              {
                  memset(SysTemp, 0, sizeof(SysTemp));
                  SysTemp[0] = 0x80;
                  strcpy(&SysTemp[1], "����Ч�ڸ�ʽ����");
              }
          }
          else
          {return FALSE;}
      }
      return FALSE;
      */
    return FALSE;
}


/**************************************Function Description *****************************************
** ��������:u8 DealManualCard()
** ����:            �����뿨��
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
u32 DealManualCard(SDK_ICC_CARDDATA *pIccCardData, u8 DateFlag, ST_TRANSFLOW *TransFlow) {
/*    u8 cardno[50]={0},Tlen=0;
    s32 result,cardlen=0;
#ifdef AFX_DEBUG
    printf("\r\n----��������--------\r\n");
#endif
    cardlen=atoi(pIccCardData->asManualLen);
    memcpy(cardno,pIccCardData->asManualData,cardlen);

    MagCardData.HandInput=1;
    memset(MagCardData.HandData,0,sizeof(MagCardData.HandData));
    memcpy(MagCardData.HandData,cardno,cardlen);
    MagCardData.HandLen=cardlen;
    
    if(CardExpDate)
    {
        result=InputExpired();
        if(TRUE!= result)  return E_MAG_EXIT;
        if(MagCardData.HaveExpired) Tlen=2;
        else Tlen=0;//�û�����û��������Ч�ڣ���������������Ч�ڵ�ʱ�򣬳���Ϊ0
        pIccCardData->bIsHaveExpired= MagCardData.HaveExpired;
        memcpy(pIccCardData->bcExpiredDate,MagCardData.ExpiredDate,2);
    }
    else{Tlen=0;}

    TransFlow->heInsOutputData[0]=0x10;
    TransFlow->heInsOutputData[1]=0x00;
    
    TransFlow->heInsOutputData[2]=0x03;//03 ��ʾ���俨��
    
    TransFlow->heInsOutputData[3]=0x00;//��Ч���ݳ���
    TransFlow->heInsOutputData[4]=cardlen+Tlen+2;
     
    TransFlow->heInsOutputData[5]=cardlen;//���俨�ų���
    memcpy(&TransFlow->heInsOutputData[6],cardno,cardlen);
    
    TransFlow->heInsOutputData[6+cardlen]=Tlen;//��Ч�ڳ���
    memcpy(&TransFlow->heInsOutputData[7+cardlen],pIccCardData->bcExpiredDate,Tlen);
    TransFlow->usInsOutputDataLen=7+cardlen+Tlen;
   */
    return E_MAG_OK;
}

/*******************************************************************
��	  ��: �ų���
��	  Ȩ: �����¹��������ɷ����޹�˾
��������: �����ҵ�����
��ڲ���: 
�� �� ֵ:1 �ɹ� 0:ʧ��
��	  ע:
********************************************************************/
u8 GetCurrncyName(u8 *lpIn, u8 *lpOut) {
    u8 rslt = 0;
    u16 i = 0, k = 0;

    k = sizeof(CurrencyNameTab) / sizeof(CurrencyNameTab[0]);
    for (i = 0; i < k; i++) {
        if (!(memcmp(lpIn, CurrencyNameTab[i].CurrencyCode, 3))) {
            strcpy(lpOut, CurrencyNameTab[i].Name);
            rslt = 1;
            break;
        }
    }
    if (rslt == 0) {
        memcpy(lpOut, lpIn, 3);
    }
    lpOut[3] = 0;
    return rslt;
}

/**************************************Function Description *****************************************
** ��������:u8 DisEcBalance()
** ����:            ��ʾ�����ֽ����
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
s32 DisEcBalance() {
    u8 temp[64] = {0}, amt[64] = {0}, secamt[64] = {0}/*,pinpadamt[64]={0}*/, currencycode[10] = {
            0};
    u8 result;
    u8 pFirstCurrencyCode_Bcd[20],
            pFirstBalance_Bcd[20],
            pSecondCurrencyCode_Bcd[20],
            pSecondBalance_Bcd[20];

    memset(pFirstCurrencyCode_Bcd, 0, sizeof(pFirstCurrencyCode_Bcd));
    memset(pFirstBalance_Bcd, 0, sizeof(pFirstBalance_Bcd));
    memset(pSecondCurrencyCode_Bcd, 0, sizeof(pSecondCurrencyCode_Bcd));
    memset(pSecondBalance_Bcd, 0, sizeof(pSecondBalance_Bcd));
    result = sdkIccGetECQueryMsg(pFirstCurrencyCode_Bcd, pFirstBalance_Bcd, pSecondCurrencyCode_Bcd,
                                 pSecondBalance_Bcd);


    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "��ѯ�ɹ�",
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);

    memset(temp, 0, sizeof(temp));
    memset(currencycode, 0, sizeof(currencycode));
    sdkBcdToAsc(currencycode, &pFirstCurrencyCode_Bcd[1], 2);
    GetCurrncyName(&currencycode[1], temp);
    strcat(temp, "�������:");
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, temp, SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
    sdkRegulateAmount(amt, pFirstBalance_Bcd);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, amt, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    if (pSecondCurrencyCode_Bcd[0])//�ڶ����ִ���
    {
        memset(secamt, 0, sizeof(secamt));
        memset(temp, 0, sizeof(temp));
        memset(currencycode, 0, sizeof(currencycode));
        sdkBcdToAsc(currencycode, &pSecondCurrencyCode_Bcd[1], 2);
        GetCurrncyName(&currencycode[1], temp);
        strcat(temp, "�������");
        sdkDispFillRowRam(SDK_DISP_LINE4, 0, temp,
                          SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);
        sdkRegulateAmount(secamt, pSecondBalance_Bcd);
        sdkDispFillRowRam(SDK_DISP_LINE5, 0, secamt,
                          SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    }
    sdkDispBrushScreen();

    return SDK_OK;
    //return WaitEnterEsc();		//����ȷ�ϴ���
}


/**************************************Function Description *****************************************
** ��������:u32 DealInsertIcc(struct ST_TRANSFLOW *TransFlow)
** ����:  ����Ӵ�ʽ��Ƭ
** �������:
                            TransFlow
** �������:
** ����ֵ:

    EMV_ERR
    ERR_NEEDMAGCARD:Ҫ��FALLBACK����
    
    2.ERR_EMV_IccCommand:����ʧ��
    3.ERR_EMV_CardBloc:��������
    4.ERR_EMV_IccDataFormat:���ݸ�ʽ��
    5.EMV_ERR:����ʧ��
    EMV_PARA_ERR
    EMV_DISPEMVRECORD
** ����:
** ��������:
**
*******************************************************************************************/
s32 DealInsertIcc(ST_TRANSFLOW *TransFlow) {
    return sdkIccTransFlow1(&IccEmvParam, &pstCardInfo.stCardParam);    //zcl 20140507

#if 0
    u8  OutBuf[128]={0};
    u32 OutLen=0;
    s32 result;
    SDK_ICC_FIXED_AID FixedAid;

    Trace("wlz","\r\n----����IC��--------\r\n");


    result = sdkIccConfigTLV("\x9C\x00",&TransFlow->InitIcTranStu.TransType,1);
    if(SDK_OK!=result)  return SDK_PARA_ERR;

    if(SDK_ICC_ICC == pstCardInfo.stCardParam.ucCardMode)
    {
        sdkmSleep(800); //�Ӵ�ʽIC����λҪͣ��800ms,��Ȼ�п��ܻḴλ���ɹ�
    }
    result = sdkIccResetIcc(&pstCardInfo.stCardParam , OutBuf , &OutLen);

    Trace("wlz","\r\n----��λIC��result=%d--------\r\n",result);

    if(SDK_OK != result)
    {
        Trace("wlz","\r\n-IccEmvParam.bIsFallback=%d-\r\n",IccEmvParam.bIsFallback);
        if(IccEmvParam.bIsFallback)
        {
            return SDK_NeedMagCardErr;
        }
        else
        {
            return SDK_ICC_RESET_ERR;
        }
    }
    result=sdkIccGetAppCandidate(&IccEmvParam,&pstCardInfo.stCardParam); 

   Trace("wlz","\r\n----��ȡӦ���б� result=%d--------\r\n",result);

    if(result!=SDK_OK)  return  result;
    memcpy(&FixedAid , &IccEmvParam.stFixedAid , sizeof(SDK_ICC_FIXED_AID));
    while(1)
    {
        result = sdkIccGoToSelectApp(FixedAid ,&IccEmvParam, &pstCardInfo.stCardParam);

        Trace("wlz","\r\n--GoToSelectApp  result %d\r\n",result);

        if(result == SDK_FixedAidAbsentErr)       //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
        {
            FixedAid.bIsFixed = FALSE;           //ѡ��һ��֮�������־ sjz 20120508
            continue;
        }
        else if(result == SDK_OK)
        {

            if(SDK_ICC_READLOG == IccEmvParam.eFlowMode)//��������ϸ
            {
                return SDK_DispLog;                                           //��ʾ������ϸ
            }
            else if(SDK_ICC_EC_QUERY == IccEmvParam.eFlowMode)//��ѯ���
            {
                result= sdkIccReadECBalance(&IccEmvParam,&pstCardInfo.stCardParam);  //jiguang  20120602

                Trace("wlz","\r\nReadECBalance  result= %d-\r\n",result);

                if(SDK_OK==result) 
                {
                    DisEcBalance();
                    sdkIccPowerDown();
                }
               return result;
            }
            memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));
   		   break;
        }
        if(result == SDK_AppSelectTryAgainErr)
        {
            result = (IccEmvParam.DispReselectApp)();                                                  // ��ʾ"����ѡ��"
            if(result != SDK_OK)
            {
                return result;
            }
            result = sdkIccReSelectApp();                                           //����ѡ��Ӧ��

            Trace("wlz","\r\n ReSelectApp  result=%d-\r\n",result);

            if(result != SDK_OK)
            {
                return result;
            }
            continue;
        }
        else if(result != SDK_OK)
        {
            return result;
        }
        break;
    }
    TransFlow->ucInsRunStep = 0;

	while (1)
	{
	    if(TransFlow->ucInsRunStep == 0) 
	    {
		    result= sdkIccGoToInitialApp(&IccEmvParam,&pstCardInfo.stCardParam);

		    Trace("wlz","\r\n-sdkIccGoToInitialApp  result=%d\r\n",result);

		    switch(result)
		    {
		        case   SDK_OK :TransFlow->ucInsRunStep = 1;
                        continue;
                        break;
		        case   SDK_NeedMagCardErr://�ߴ���������
                        return SDK_NeedMagCardErr;
                        break;
		        case   SDK_AppSelectTryAgainErr://    2.��ѡ��AID
                        result=DealRepeatSel(&IccEmvParam,&pstCardInfo.stCardParam);
                        if(result!=SDK_OK)  return INS_EXIT;//    ����ѡ��ʧ��
                        else     continue;//����ѡ��Ӧ�ú��������д���ָ�����Ӧ�ó�ʼ��
                        break;
		        case    SDK_OfflineDecline://	�ѻ��ܾ�
		        case    SDK_TransOfflineDeclineErr:
		                SaveOfflineBag(TransFlow->asTransCode,SDK_OfflineDecline);
		                DispIccErr(result);
		                return INS_EXIT;
		                break;  
		        case    SDK_TransOfflineApprove://	�ѻ���׼��ת��47�Ŵ���
		                TransFlow->QpbocOnlineFlag=2;
		                return INS_SUC;;
		                break;  
		        case    SDK_TransOnlineApprove://	������׼
                        TransFlow->ucInsRunStep=8;//��ΪQpboc��������ֱ�ӻ�ȡ55������
                        continue;
                        break;
		        default:DispIccErr(result);
                        return INS_EXIT;//   ����ʧ��
                        break;
		    }
	    }
	    else if(TransFlow->ucInsRunStep == 1)
	    {
	        result=sdkIccReadAppData(&IccEmvParam,&pstCardInfo.stCardParam);
	        Trace("wlz","\r\n-sdkIccReadAppData  result=%d--\r\n",result);
	        switch(result)
	        {
	            case   SDK_OK :
                        return SDK_OK;
                        break;
	             case    SDK_OfflineApprove://	�ѻ���׼��ת��47�Ŵ���
                        TransFlow->QpbocOnlineFlag=2;
                        return SDK_OK;
	            break;
	            case    SDK_OfflineDecline://	qPBOC�ѻ��ܾ�����
                        SaveOfflineBag(TransFlow->asTransCode,SDK_OfflineDecline);
                        return result;
	            break;
	            default:
	        //        DispIccErr(result);
	                    return result;//   ����ʧ��
	                    break;
	        }
	    }
	}
    return result;
#endif
}

/**************************************Function Description *****************************************
** ��������:void GetUnsaleAid()
** ����:  ��ȡ�����ཻ�׵�ԭ��AID
** �������:
** �������:
** ����ֵ:
                        SUCCESS  �ҵ�ԭ����AID
                        UNSUCCESS  �Ҳ���ԭ����AID
** ����:
** ��������:
**
*******************************************************************************************/
u8 GetUnsaleAid() {
    IccEmvParam.stFixedAid.bIsFixed = TRUE;
    memcpy(IccEmvParam.stFixedAid.heAid, "A000000333010101", 16);
    IccEmvParam.stFixedAid.uclen = 0;
    return SUCCESS;
}

/**************************************Function Description *****************************************
** ��������:u8 DisIccNeedFallbcak()
** ����:            ��ʾ��Ҫ���н��ʹ���
** �������:
** �������:
** ����ֵ:
                            SDK_OK  ȷ�����ɹ��˳�
                            SDK_ESC ȡ���˳�
                            SDK_TIME_OUT ��ʱ�˳�
** ����:
** ��������:
**
*******************************************************************************************/
void DisIccNeedFallbcak() {
    sdkDispClearScreen();
//	sdkDispFillRowRam(SDK_DISP_LINE2,  0, "��Ҫ��fallback����," , SDK_DISP_LDISP|SDK_DISP_FDISP);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "IC������ʧ��,��ˢ�ſ�", SDK_DISP_LDISP | SDK_DISP_FDISP);
    sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ�ϼ�����...", SDK_DISP_RDISP | SDK_DISP_FDISP);
    sdkDispBrushScreen();
    WaitEnterEsc();
//	 WaitFallBack();	
}

/**************************************Function Description *****************************************
** ��������:u8 InputPsw(struct ST_TRANSFLOW *TransFlow)
** ����:  ���������ʱ����������
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
s32 InputPsw(ST_TRANSFLOW *TransFlow) {
    //   return ReadEncryptPin(TransFlow, IcTranType);
    return SDK_OK;
}

/**************************************Function Description *****************************************
** ��������:void DisProcessing(void)
** ����:  ��ʾ"���Ժ�"
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
void DisProcessing(void) {
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "���Ժ�...", SDK_DISP_FDISP | SDK_DISP_CDISP);
    sdkDispBrushScreen();
}

/**************************************Function Description *****************************************
** ��������:u8 DealRepeatSel(SDK_ICC_TRADE_PARAM * pIccTradeParam,SDK_ICC_PARAM*pIccParam )
** ����:            Ӧ�ó�ʼ��ʧ�ܺ���Ҫ����Ӧ��ѡ��
** �������:
** �������:
** ����ֵ:
                            1.EMV_OK:�ɹ�;
                            2.ERR_EMV_NoAppSel:û��Ӧ�ÿɹ�ѡ��
                            3.EMV_ERR:����ʧ��
** ����:
** ��������:
**
*******************************************************************************************/
s32 DealRepeatSel(SDK_ICC_TRADE_PARAM *pIccTradeParam, SDK_ICC_PARAM *pIccParam) {
    s32 ret;
    SDK_ICC_FIXED_AID FixedAid;
    memset(&FixedAid, 0, sizeof(FixedAid));

    if (NULL != pIccTradeParam->DispReselectApp) {

        ret = (*pIccTradeParam->DispReselectApp)();   // ��ʾ"����ѡ��"
        if (ret != SDK_OK) {
            return ret;
        }
    }
    ret = sdkIccReSelectApp();  //����ѡ��Ӧ��

    while (1) {
        ret = sdkIccGoToSelectApp(FixedAid, pIccTradeParam, pIccParam);//ȥѡ��Ӧ��

        if (ret == SDK_FixedAidAbsentErr)       //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
        {
            FixedAid.bIsFixed = FALSE;           //ѡ��һ��֮�������־ sjz 20120508
            continue;
        }
        else if (ret == SDK_OK) {
            ret = sdkIccGoToInitialApp(pIccTradeParam, pIccParam);//ȥ��ʼ��Ӧ��
        }

        if (ret == SDK_AppSelectTryAgainErr) {
            if (NULL != pIccTradeParam->DispReselectApp) {
                ret = (*pIccTradeParam->DispReselectApp)();                                              // ��ʾ"����ѡ��"

                if (ret != SDK_OK) {
                    return ret;
                }
            }
            ret = sdkIccReSelectApp();                                           //����ѡ��Ӧ��

            if (ret != SDK_OK) {
                return ret;
            }
            continue;
        }
        else if (ret != SDK_OK) {
            return ret;
        }
        break;
    }

    return ret;

}

/**************************************Function Description *****************************************
** ��������:u8 DealIntegralProcess(struct ST_TRANSFLOW *TransFlow)
** ����:  PBOC��������
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
s32 DealIntegralProcess(ST_TRANSFLOW *TransFlow) {
#if 0
    u32 result;
    u8 temp[512];
    u16 tLen=0;
	
	Trace("wlz","INS44_DealIntegralPBOC--�������̴���ָ��!\r\n");
	memset(temp,0x00,sizeof(temp));
	IccEmvParam.eFlowMode = SDK_ICC_ALL_FLOW;      //zcl 20140507
	if(gucNeedIntegralProcess== FALSE)
	{
		result = sdkIccTransFlow1(&IccEmvParam,&pstCardInfo.stCardParam);
	}
	else
	{
		result = sdkIccTransFlow1_5(&IccEmvParam,&pstCardInfo.stCardParam); 	
	}
	switch(result)
	{
		case	SDK_OnlineWait:
				result=ZipPagData(0,temp,&tLen);

				Trace("wlz","\r\n----ZipPagData  result--------\r\n");
				TraceHex("wlz","\r\n----��ȡ55����=",temp,tLen);

				memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));

				TransFlow->heInsOutputData[2] = 0x35;//�ɽӴ�ʽ�����ȡIC��
				TransFlow->heInsOutputData[3] = 0x01;//IC��
				memcpy(&TransFlow->heInsOutputData[4],AppCreditPwd,8);
				TransFlow->heInsOutputData[20]=tLen;
				memcpy(&(TransFlow->heInsOutputData[21]),temp,tLen);
				sdkU16ToHex(TransFlow->heInsOutputData,  tLen + 19, 2);
				TransFlow->usInsOutputDataLen = tLen+21;
				SetIsICOnLineTrade(INTEGRAL);         //���õ�ǰ����ΪIC����������
				return INS_SUC;
				break;
		case	SDK_NeedMagCardErr:
			 	if(gstAppSetting.BIsEnableFallback==1)
    			{
    				
               		return DealJumpToFallback(TransFlow);
    			}
				else
				{
					DispIccErr(SDK_ICC_RESET_ERR);
            		return INS_EXIT;//�����˳�
				}
				break;
		case	SDK_OfflineDecline://	qPBOC�ѻ��ܾ�
		//		SaveOfflineBag(TransFlow->asTransCode,SDK_OfflineDecline);
				DispIccErr(result);
				return INS_EXIT;
		case	SDK_OfflineApprove: //qPBOC�ѻ���׼
		//		TransFlow->QpbocOnlineFlag=2;
				DispIccErr(result);
				return INS_SUC;
		default:
				DispIccErr(result);
		        return INS_EXIT;//   ����ʧ��
				break;
	}
#endif
#if 0
    while(1)
    {
        if(TransFlow->ucInsRunStep == 0)
        {
            result=sdkIccDataAuth(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\nsdkIccDataAuth  result=%d-\r\n",result);

            switch(result)
            {
                case   SDK_OK ://
                TransFlow->ucInsRunStep = 3;
                break;
                case    SDK_OnlineWait://	��qPBOC��������
                            TransFlow->ucInsRunStep=8;//��ΪQpboc��������ֱ�ӻ�ȡ55������
                break;
                case    SDK_OfflineDecline://	qPBOC�ѻ��ܾ�
                            SaveOfflineBag(TransFlow->asTransCode,SDK_OfflineDecline);
                            return INS_EXIT;
                break;
                case    SDK_OfflineApprove:	//qPBOC�ѻ���׼
                            TransFlow->QpbocOnlineFlag=2;
                             return INS_SUC;
                break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }
            continue;
        }
        else if(TransFlow->ucInsRunStep == 3)
        {
            result=sdkIccProcessRestrict(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\n-sdkIccProcessRestrict  result=%d-\r\n",result);

            switch(result)
            {
                case   SDK_OK ://
                            TransFlow->ucInsRunStep = 4;
                break;
                case    SDK_OfflineApprove://	qPBOC�ѻ���׼
                            TransFlow->QpbocOnlineFlag=2;
                            return INS_SUC;
                break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }
            continue;
        }
        else if(TransFlow->ucInsRunStep == 4)
        {
            result=sdkIccCardHolderVerf(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\n-sdkIccCardHolderVerf  result=%d-\r\n",result);

            switch(result)
            {
                case   SDK_OK ://
                        TransFlow->ucInsRunStep =5;
                break;
                case    SDK_OfflineApprove://	qPBOC�ѻ���׼
                            TransFlow->QpbocOnlineFlag=2;
                            return INS_SUC;
                break;
                case    SDK_OfflineDecline://	qPBOC�ѻ��ܾ�
                            SaveOfflineBag(TransFlow->asTransCode,SDK_OfflineDecline);
                            return INS_EXIT;
                break;
                 case    SDK_OnlineWait	://qPBOC��������
                             TransFlow->ucInsRunStep=8;//��ΪQpboc��������ֱ�ӻ�ȡ55������
                 break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }
            continue;
        }
        else if(TransFlow->ucInsRunStep == 5)
        {
            result=sdkIccTermRiskManage(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\n--sdkIccTermRiskManage  result=%d-\r\n",result);
            switch(result)
            {
                case   SDK_OK ://
                    TransFlow->ucInsRunStep = 6;
                    break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }
                continue;
        }
        else if(TransFlow->ucInsRunStep == 6)
        {
            result=sdkIccTermiAnalys(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\n-sdkIccTermiAnalys  result=%d\r\n",result);

            switch(result)
            {
                case   SDK_OK ://
                        TransFlow->ucInsRunStep = 7;
                        continue;
                        break;
                case   SDK_NeedMagCardErr ://�ߴ���������
                        return DealJumpToFallback(TransFlow);
                        break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }

        }
        else if(TransFlow->ucInsRunStep == 7)
        {
            result=sdkIccTransAnalys(&IccEmvParam,&pstCardInfo.stCardParam);

            Trace("wlz","\r\nsdkIccTransAnalys  result=%d\r\n",result);

            switch(result)
            {
                case    SDK_OK://	�ѻ����׳ɹ�

                    TransFlow->bOnline=0;
                    return INS_SUC;
                    break;
               case  SDK_OnlineWait:
                    TransFlow->bOnline=1;
                    TransFlow->ucInsRunStep = 8;
                    continue;
                    break;
                default:
                    DispIccErr(result);
                    return INS_EXIT;//   ����ʧ��
                break;
            }

        }
        else if(TransFlow->ucInsRunStep == 8)//��֯����tagֵ��������
        {
            result=ZipPagData(0,temp,&tLen);

            Trace("wlz","\r\n----ZipPagData  result--------\r\n");
            TraceHex("wlz","\r\n----��ȡ55����=",temp,tLen);

            memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));

            TransFlow->heInsOutputData[2] = 0x35;//�ɽӴ�ʽ�����ȡIC��
            TransFlow->heInsOutputData[3] = 0x01;//IC��
            memcpy(&TransFlow->heInsOutputData[4],AppCreditPwd,8);
            TransFlow->heInsOutputData[20]=tLen;
            memcpy(&(TransFlow->heInsOutputData[21]),temp,tLen);
            sdkU16ToHex(TransFlow->heInsOutputData,  tLen + 19, 2);
            TransFlow->usInsOutputDataLen = tLen+21;
            SetIsICOnLineTrade(INTEGRAL);         //���õ�ǰ����ΪIC����������

            return INS_SUC;
        }
    }
#endif
}

/**************************************Function Description *****************************************
** ��������:u8 DealSimplePboc(struct ST_TRANSFLOW *TransFlow)
** ����:  PBOC������
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
s32 DealSimplePboc(ST_TRANSFLOW *TransFlow, u8 IfPutInPin) {
#if 0
    u32 result;
    u8 tvr[128]={0},CreditPwdBuf[13];
    u8 temp[512];
    u16 tLen=0;
    u32 tvrlen=0;

    if(IfPutInPin)
    {
        if(NULL != IccEmvParam.InputPWD) 
        {
            result = (IccEmvParam.InputPWD)(IccEmvParam.pasTradeAmount , 3, SDK_PED_IC_ONLINE_PIN, CreditPwdBuf); // ȷ�Ͽ���
            if(result != SDK_OK)  return INS_EXIT;   //û��ȷ�Ͽ��ž��˳�
        }
    }

    memset(temp,0,sizeof(temp));
    result=ZipPagData(1,temp,&tLen);


    Trace("wlz","\r\n----ZipPagData  result=%d--------\r\n",result);
    TraceHex("wlz","\r\n----��ȡ55����=",temp,tLen);

    memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));

    TransFlow->heInsOutputData[2] = 0x35;//�ɽӴ�ʽ�����ȡIC��
    TransFlow->heInsOutputData[3] = 0x01;//IC��
    memcpy(&TransFlow->heInsOutputData[4], AppCreditPwd,8);
    TransFlow->heInsOutputData[20]=tLen;
    memcpy(&(TransFlow->heInsOutputData[21]),temp,tLen);
    sdkU16ToHex(TransFlow->heInsOutputData,  tLen + 19, 2);
    TransFlow->usInsOutputDataLen = tLen+21;
    SetIsICOnLineTrade(SIMPLE);         //���õ�ǰ����ΪIC����������
    return INS_SUC;
#endif
}


/**************************************Function Description *****************************************
** ��������:u8 SaveTcScript()
** ����:  ����TC�ͽű�ֵ��_TCandScripts�ṹ���У�����ϵͳ�ļ�
** �������:
** �������:
** ����ֵ:     0 -�ɹ�  1 -ʧ�� 
** ����:
** ��������:
**
*******************************************************************************************/
s32 SaveTcScript() {
#if 0
    u8 tempdata[50],
    Voucher[4],
       destag[200],
    len=0,
    i=0;
    u32   temptaglen=0,destaglen=0;
    s32 result;
    char Gtaglist[GTAGLISTMUN][3]=
    {

        {"\x9f\x26"},//Ӧ������
        {"\x9f\x27"},//CryptInfoData
    //	{"\x9f\x34"},//CVR
        { "\xDF\x31"},//�ű�֪ͨ
        { "\x95\x00"},//TVR
        { "\x9F\x10"},//������Ӧ������
    };
    ST_TCandScripts TempTCScripts;
    memset(&TempTCScripts,0,sizeof(ST_TCandScripts));
    for(i=0;i<GTAGLISTMUN;i++)
    {
        memset(tempdata,0,sizeof(tempdata));
        memset(destag,0,sizeof(destag));
        result= sdkIccReadTLV(Gtaglist[i],tempdata,&temptaglen);
        if( (result==SDK_OK)&&	(temptaglen>0))
        {
            destaglen=0;
            result= FormTLV(destag,&destaglen,Gtaglist[i],tempdata,&temptaglen);
            if(SUCCESS!=result) return UNSUCCESS;
            memcpy(&(TempTCScripts.Tlv[len]),destag,destaglen);
            len=len+destaglen;
            if(i == 1)
            {
                TempTCScripts.TcLen = len;
            }
        }
    }

    TempTCScripts.Tlen=len;
    TempTCScripts.ScrLen = TempTCScripts.Tlen - TempTCScripts.TcLen;
    if(gstSys.ucYear >= 0x50)
        TempTCScripts.bcSysDate[0] = 0x19;
    else
        TempTCScripts.bcSysDate[0] = 0x20;
    TempTCScripts.bcSysDate[1] = gstSys.ucYear;
    TempTCScripts.bcSysDate[2] = gstSys.ucMonth;
    TempTCScripts.bcSysDate[3] = gstSys.ucDay;

    //ԭ������ˮ
    memcpy(Voucher,gstSys.asVoucherNo,3);
    sdkBcdSub(Voucher,Voucher, 3,"\x00\x00\01",3);
    memcpy(TempTCScripts.VoucherNo,Voucher,3);
    //ԭ����MAC
    memcpy(TempTCScripts.lastMac,LastMac,8);
    result=SaveTcScriptsBag(&TempTCScripts);

    Trace("wlz","\r\n----SaveTcScriptsBag resultt=%d--------\r\n",result);
    TraceHex("wlz","TCScripts",TempTCScripts.Tlv,TempTCScripts.Tlen);

    return result;
#endif
    return 0;

}

/**************************************Function Description *****************************************
** ��������:u8 FormTcScriptBag( ST_TCandScripts *TcScriptInfo,u8 *Des,u8 *DesLen )
** ����:  ��TcScriptInfo�ж�ȡֵ�� ��֯TC�ͽű�֪ͨ�����
** �������:
                            TcScriptInfo  �ӸĽṹ���ж�����Ա
                            Des  ����   TC�ͽű�֪ͨ���
                            DesLen :DES���ݳ���
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
#if 0
s32 FormTcScriptBag( ST_TCandScripts *TcScriptInfo,u8 *Des,u8 *DesLen )
{
    u8  *ptr,len;
    ptr=Des;
    len=0;
    
     //ԭ��������
    memcpy(&ptr[len],TcScriptInfo->bcSysDate,4);
    len+=4;
    //ԭ������ˮ
    memcpy(&ptr[len],TcScriptInfo->VoucherNo,3);
    len+=3;
    //ԭ����MAC
    memcpy(&ptr[len],TcScriptInfo->lastMac,8);
    len+=8;
    //TC����/TC����
    ptr[len] = TcScriptInfo->TcLen;
    len+=1;
    memcpy(&ptr[len],TcScriptInfo->Tlv,TcScriptInfo->TcLen);
    len+=TcScriptInfo->TcLen;
    //�ű�����/�ű�����
    ptr[len] = TcScriptInfo->ScrLen;
    len+=1;
    memcpy(&ptr[len],&TcScriptInfo->Tlv[TcScriptInfo->TcLen],TcScriptInfo->ScrLen);
    len+=TcScriptInfo->ScrLen;
    *DesLen=len;
    return 0;
}
#endif

/**************************************Function Description *****************************************
** ��������:u8 FindTagLen(u8 *pTag)
** ����:  ����TAG���ر�ʾTAG�ֽڵĳ���
** �������:
                            Mindex:     �̻�����
                            pTag :      Tagֵ
                            pOutData:�����V
                            pOutLen:    V���ܳ���
** �������:
                        UNSUCCESS:δ�ҵ�TAG
                        SUCCESS     :�ҵ�TAG
                        
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
u8 RTlvfrmOffList(u8 Mindex, const u8 *pTag, u8 *pOutData, u32 *pOutLen) {
/*    u8 result,temp[50] = {0},len=0,TradeVou[5];
    _MerchantInfo TempInfo;
    if(NULL==pTag||NULL==pOutData) return UNSUCCESS;
    memset(&TempInfo,0,sizeof(TempInfo));

    if(memcmp(pTag,"\x5F\x06",2)==0)//ԭ������ˮ
    {
        memset(TradeVou,0,sizeof(TradeVou));
        memcpy(TradeVou,Sys.VoucherNo,3);//��ˮ
        sdkBcdSub(TradeVou, "\x00\x00\01",3);				//��ˮ�ż�1
        len=3;
        memcpy(temp,TradeVou,len);
    }
  else
    {
        if((memcmp(pTag,"\x5f\x01",2)!=0)&&
        (memcmp(pTag,"\x1F\x07",2)!=0)&&
        (memcmp(pTag,"\x1F\x08",2)!=0)&&
        (memcmp(pTag,"\x1F\x09",2)!=0))
        return UNSUCCESS;
        result=ReadMerchantInfo(Mindex,&TempInfo);
        if(RSLT_OK!=result) return UNSUCCESS;

        //memcpy(pOutData,pTag,2);
        if(memcmp(pTag,"\x5F\x01",2)==0)  
        {
            len=strlen(TempInfo.Channel);
            memcpy(temp,TempInfo.Channel,len);
        }
         else   if(memcmp(pTag,"\x1F\x08",2)==0)  
        {
            len=strlen(TempInfo.MerchantNo);
            memcpy(temp,TempInfo.MerchantNo,len);
        }
         else   if(memcmp(pTag,"\x1F\x07",2)==0)  
        {
            len=strlen(TempInfo.MerchantName);
            memcpy(temp,TempInfo.MerchantName,len);
        }
         else   if(memcmp(pTag,"\x1F\x09",2)==0)  
        {
            len=strlen(TempInfo.TerminalNo);
            memcpy(temp,TempInfo.TerminalNo,len);
        }
    }
    memcpy(pOutData,temp,len);
    *pOutLen=len;
    return SUCCESS;
    */
    return SUCCESS;

}

/**************************************Function Description *****************************************
** ��������:u8 RegCardNo(char *bcd_cardno,u8 *bcd_len,char *asc_cardno,u8 *asc_len,)
** ����: ��BCD����ת��ΪASC��
** �������:
                            bcd_cardno:     BCD�����ַ���
                            bcd_len :      bcd_cardno�ĳ���
                            asc_cardno:ASC�����ַ���
                            asc_len:    asc_cardno����
** �������:
                        SUCCESS     :�ɹ�ת��
                        
** ����ֵ:
** ����:
** ��������:

*******************************************************************************************/
u8 RegCardNo(char *bcd_cardno, u32 *bcd_len, char *asc_cardno, u16 *asc_len) {
    u32 len, i, j;
    u8 Tcardno[101] = {0};
    if (*bcd_len > 100) *bcd_len = 100;
    len = *bcd_len;
    memcpy(Tcardno, bcd_cardno, len);
    for (i = len - 1; i > 1; i--) {
        if (Tcardno[i] != 0xff) {
            break;
        }
    }
    j = i + 1;
    if ((Tcardno[j - 1] & 0x0f) == 0x0f) {
        i = (u16)(j * 2 - 1);
    }
    else {
        i = (u16)(j * 2);
    }

    *asc_len = i;
    sdkBcdToAsc(asc_cardno, bcd_cardno, *bcd_len);
    asc_cardno[i] = 0x00;
    return SUCCESS;

}


/**************************************Function Description *****************************************
** ��������:u8 FindTagLen(u8 *pTag)
** ����:  ����TAG���ر�ʾTAG�ֽڵĳ���
** �������:
                            pTag:     TAGֵ
                            MerId:    �̻�����
                            DesStr  :ֵ����Ϊ���ת��Ϊ���ĸ�ʽ
                            deslen  :DesStr�ĳ���
** �������:
** ����ֵ:TLV��T��ռ���ֽ���

** ����:
** ��������:
**
*******************************************************************************************/
u8 FindTagLen(u8 MerId, u8 *pTag, u8 *DesStr, u16 *deslen) {
    u8 OutData[130], gtag[3], Llen, taglen, temp[200], TimeTmp[10];
    u32 OutLen;
    s32 result;
    if (((*pTag) & 0x1F) == 0x1F) taglen = 2;//��ǩTΪ2���ֽ�
    else taglen = 1;//����TΪһ���ֽ�

    memset(gtag, 0, sizeof(gtag));
    memcpy(gtag, pTag, taglen);
#ifdef AFX_DEBUG
    //   printf("Ҫ������TAG:");
   //    printfbuf(gtag,taglen);
#endif
    memset(OutData, 0, sizeof(OutData));
    OutLen = 0;

    result = sdkIccReadTLV(gtag, OutData, &OutLen);
#ifdef AFX_DEBUG
    //   printf("--------sdkIccReadTLV result=%d\r\n",result);
#endif
    if (SDK_PARA_ERR == result)//û�ҵ���TAG ���ٵ��ѻ��̻���Ϣ��ȥ��
    {
        result = RTlvfrmOffList(MerId, gtag, OutData, &OutLen);
#ifdef AFX_DEBUG
        //   printf("--------sdkIccReadTLV result=%d\r\n",result);
#endif
        if (SUCCESS != result) return taglen;
    }
    else if (SDK_EQU == result) return taglen;

    if (OutLen < 128) Llen = 1;
    else Llen = 2;

    if ((memcmp("\x9f\x02", gtag, 2) == 0) || (memcmp("\x9f\x79", gtag, 2) == 0)) {
        sdkRegulateAmount(temp, OutData);
        *deslen = strlen(temp);
        memcpy(DesStr, temp, *deslen);//��V���Ƶ�DesStr
    }
    else if (memcmp("\x9f\x21", gtag, 2) == 0)//����ʱ��
    {
        memset(TimeTmp, 0, sizeof(TimeTmp));
        sdkBcdToAsc(TimeTmp, OutData, OutLen);
        memcpy(temp, TimeTmp, 2);
        temp[2] = ':';
        memcpy(&temp[3], &TimeTmp[2], 2);
        temp[5] = ':';
        memcpy(&temp[6], &TimeTmp[4], 2);
        *deslen = OutLen * 2 + 2;//TLV��V�ĳ���
        memcpy(DesStr, temp, *deslen);//��V���Ƶ�DesStr
    }
    else if ((memcmp("\x9A\x00", gtag, 2) == 0) ||
             (memcmp("\x5F\x24", gtag, 2) == 0)) //�������ں���Ч��
    {
        memset(TimeTmp, 0, sizeof(TimeTmp));
        sdkBcdToAsc(TimeTmp, OutData, OutLen);
        memcpy(temp, "20", 2);
        memcpy(&temp[2], TimeTmp, 2);
        temp[4] = '/';
        memcpy(&temp[5], &TimeTmp[2], 2);
        temp[7] = '/';
        memcpy(&temp[8], &TimeTmp[4], 2);
        *deslen = OutLen * 2 + 4;//TLV��V�ĳ���
        memcpy(DesStr, temp, *deslen);//��V���Ƶ�DesStr
    }
    else if (memcmp("\x50\x00", gtag, 2) == 0 || memcmp("\x1f\x07", gtag, 2) == 0
             || memcmp("\x1f\x08", gtag, 2) == 0 ||
             memcmp("\x1f\x09", gtag, 2) == 0)//applabel ��asc ֱ�Ӵ�ӡ����������Ҫת
    {
        *deslen = OutLen;//TLV��V�ĳ���
        memcpy(DesStr, OutData, *deslen);//��V���Ƶ�DesStr
    }
    else if (memcmp("\x5a\x00", gtag, 2) == 0)//����
    {
        RegCardNo(OutData, &OutLen, temp, deslen);
        memcpy(DesStr, temp, *deslen);//��V���Ƶ�DesStr
    }
    else {
        sdkBcdToAsc(temp, OutData, OutLen);
        *deslen = OutLen * 2;//TLV��V�ĳ���
        memcpy(DesStr, temp, *deslen);//��V���Ƶ�DesStr
    }

#ifdef AFX_DEBUG
    //   printf("��������TAG=:");
   //    printfbuf(DesStr,*deslen);
#endif
    //DesStr=DesStr+(*deslen);
    return taglen;
}

/**************************************Function Description *****************************************
** ��������:u8 FindTagLen(u8 *pTag)
** ����:  
                


** �������:
                            MerIndex:�̻�����
                            DesStr:����ҪTAGֵ���ַ������������DesStr
                            DesLen:����ĳ���
                            SrcStr:����TAGֵ���ַ���
                            SrcLen:SrcStr�ĳ���
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
void PrasePrintTag(u8 MerIndex, u8 *DesStr, u16 *DesLen, u8 *SrcStr, u16 *SrcLen) {
    u8 *sptr, *dptr, tlen;
    u16 slen, Dlen;
    dptr = DesStr;
    sptr = SrcStr;
    slen = (*SrcLen);
    *DesLen = 0;
    while (slen) {
        if (memcmp(sptr, "\xEE\xEE\xEE\xEE", 4) == 0) {
            sptr += 4;
            Dlen = 0;
            tlen = FindTagLen(MerIndex, sptr, dptr, &Dlen);
            dptr += Dlen;
            sptr += tlen;
            slen = slen - 4 - tlen;
            (*DesLen) += Dlen;
        }
        else {
            *dptr = *sptr;
            dptr++;
            sptr++;
            (*DesLen)++;
            slen--;
        }
        continue;
    }
}
/**************************************Function Description *****************************************
** ��������:
** ����           : 
** �������:
                            
** �������:
** ����ֵ      :
** ����           :
** ��������:
**
*******************************************************************************************/
/*
void ParseMerchant(u8 *mdata,u8 *len,_MerchantInfo *MInfo)
{
    u8 *sdata,mlen=*len,tlen,i;
    
    u8 temptag[MAXIDTAG][3]=
        {
            "\x5F\x01",
            "\x1F\x07",
            "\x1F\x08",
            "\x1F\x09"
        };
    
    sdata=mdata;
    while(mlen)
    {
        for(i=0;i<MAXIDTAG;i++)//��ͷ�ַ���temptag�Ƚ�
        {
           if(memcmp(sdata,&temptag[i][0],2)==0)//�ҵ��̻���Ϣ
            {
#ifdef AFX_DEBUG
    printf("�������̻���ϢTAG=:");
    printfbuf(&temptag[i][0],2);
#endif
                sdata+=2;
                tlen=*sdata;
                sdata++;
                switch(i)
                {
                    case 0:memcpy(MInfo->Channel,sdata,tlen);break;//����ͨ����
                    case 1:memcpy(MInfo->MerchantName,sdata,tlen);break;//�̻�����
                    case 2:memcpy(MInfo->MerchantNo,sdata,tlen);break;//�̻���
                    case 3:memcpy(MInfo->TerminalNo,sdata,tlen);break;//�ն˺�
                    default:break;
                }
                 sdata+=tlen;
                mlen=mlen-tlen-3;
                break;
            }
        }
        if(i==MAXIDTAG) //  ͷ�ַ���temptag��ƥ�䣬��ͷ�ַ��Ƶ���һ�ַ�
        {
            #ifdef AFX_DEBUG
    printf("----δ�������̻���Ϣsdata=%d\r\n",*sdata);
#endif
            mlen--;
            sdata++;
        }
    }


}*/
/**************************************Function Description *****************************************
** ��������:
** ����           : 
** �������:
                            
** �������:
** ����ֵ      :
** ����           :
** ��������:
**
*******************************************************************************************/
/*
void FormOfflineBage(u8 *Sdata,_OffLineTrade *OfftradeInfo)
{
    u8 *ptr;
    ptr=Sdata;
    sdkU16ToHex(ptr, (OfftradeInfo->TagLen)+8, 2);//    ���⽻�����ݳ���
    ptr+=2;
    *ptr=OfftradeInfo->OffType;//���⽻������
    ptr++;

    *ptr=OfftradeInfo->RcardMode;//���׶�����ʽ
    ptr++;
    
    memcpy(ptr,OfftradeInfo->TradeVou,3);//������ˮ��
    ptr+=3;

    memcpy(ptr,OfftradeInfo->TradeCode,3);//���״���
    ptr+=3;
    
    memcpy(ptr,OfftradeInfo->TagLenVal,OfftradeInfo->TagLen);//���⽻������

}
*/
/**************************************Function Description *****************************************
** ��������:void PrintOffTrade(ST_TRANSFLOW* TransFlow)
** ����           : ��ӡ�ѻ���
** �������:
                            
** �������:
** ����ֵ      :
** ����           :
** ��������:
**
*******************************************************************************************/
void PrintOffTrade(ST_TRANSFLOW *TransFlow) {
/*    u8 reslut;
    u8 PrintData[1024];				//��ӡ��Ϣ
    u16 PrintDataLen=0,PLen;
	u8 flag=0,n;
    
    _PRINTINFO *PrintInfo=NULL;
    _OFFLINEPRINTINFO OfflinePrintInfo;
	
	u8 Listindex;
	
	memset(PrintData, 0x00,sizeof(PrintData));
	PrintInfo = (_PRINTINFO *)GetMem(sizeof(_PRINTINFO));
	Listindex = TransFlow->InitIcTranStu.OffPrintIndex;

    memset(&OfflinePrintInfo,0,sizeof(_OFFLINEPRINTINFO));

    LcdClearMulRow(SDK_DISP_LINE1, SDK_DISP_LINE5);
    sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���ڴ�ӡ...", SDK_DISP_FDISP|SDK_DISP_CDISP|OVERLINE|DOWNLINE|SIDELINE);
    sdkDispBrushScreen();

    reslut= ReadOfflinePrintBag(Listindex, &OfflinePrintInfo);
    if(reslut)  
	{
		FreeMem(PrintInfo);
		PrintInfo = NULL;
		return;
	}
    HexToU16(&PLen,OfflinePrintInfo.OLPRINT_InfoLen,2);
    PrintInfo->PRINT_Flag = 0;					//��ӡ��־
  
#ifdef AFX_DEBUG
    printf("Ҫ����������(%d):",PLen);
    printfbuf(OfflinePrintInfo.OLPRINT_Info,PLen);
#endif

    PrintData[0] = OfflinePrintInfo.OLPRINT_Copies;//��ӡ����

    PrasePrintTag(Listindex,&PrintData[1],&PrintDataLen,OfflinePrintInfo.OLPRINT_Info,&PLen);
   
#ifdef AFX_DEBUG
    printf("���������Ĵ�ӡ����(%d):",PrintDataLen);
    printfbuf(PrintData,PrintDataLen);
#endif
   
   if(DealPrintData(PrintData,PrintDataLen + 1, TransFlow->TransName, PrintInfo,FALSE)==RSLT_OK)
  	{
  		if (Setting.PrintSwitch== 0)
  		{
  			sdkDispClearScreen();
  			sdkDispFillRowRam(SDK_DISP_LINE2, 0, "��ӡ�ر�", SDK_DISP_FDISP|SDK_DISP_CDISP|SDK_DISP_INCOL);
  			sdkDispFillRowRam(SDK_DISP_LINE3, 0, "���������", SDK_DISP_FDISP|SDK_DISP_CDISP|SDK_DISP_INCOL);
  			sdkDispBrushScreen();
  			WaitAnyKey(MASK_ALL,GetTimerAutoRetValue());
  			flag=1;
  		}
  		else
  		{
#ifdef AFX_DEBUG
    		printf("==============PrintInfo->PRINT_Copies(%d):\r\n",PrintInfo->PRINT_Copies);
#endif
  			for(n=0;n<PrintInfo->PRINT_Copies;n++)
  			{
  				if(PostPrint(PrintInfo,n,0)!=RSLT_OK)
  				{
  					flag=1;
  					break;
  				}
  			}
  		}
  	}
  
  	if(flag==0)  //�ɹ�
  	{
  		PrintInfo->PRINT_Flag = 1;//δ��ӡ��־
  		SavePrintInfo(PrintInfo);
  	}
  */
}

/*******************************************************************
��    ��: ��Т��
��������:unsigned char DealReadLogRecord(SDK_ICC_APDU_RESP *apdu_r)
��������: ������־����
��ڲ���: 
�� �� ֵ: 
��ص���: 
��    ע: 
�޸���Ϣ:         
********************************************************************/
s32 DealReadLogRecord(SDK_ICC_APDU_RESP *apdu_r, unsigned char *aucDOL, unsigned char *ucRecordNum,
                      unsigned char partno) {
    unsigned char *pDOL, *pDataOut;
    unsigned char aucTransDate[3], aucTransTime[3], aucAuthAmount[6], aucOtherAmount[6],
            aucCountryCode[2], aucTransCurcyCode[2], aucMerchantName[20], aucATC[4], ucTransType;
    unsigned char ucTagLen;
    unsigned short i;
    unsigned short recorddatalen = 0;
    unsigned char aucBuff[30], temp[13];
    unsigned char tempnum;

    pDOL = aucDOL;
    pDataOut = (unsigned char *) apdu_r->DataOut;
    memset(aucTransDate, 0x00, sizeof(aucTransDate));
    memset(aucTransTime, 0x00, sizeof(aucTransTime));
    memset(aucAuthAmount, 0x01, sizeof(aucAuthAmount));
    memset(aucOtherAmount, 0x01, sizeof(aucOtherAmount));
    memset(aucCountryCode, 0x00, sizeof(aucCountryCode));
    memset(aucTransCurcyCode, 0x00, sizeof(aucTransCurcyCode));
    memset(aucMerchantName, 0x00, sizeof(aucMerchantName));
    memset(aucATC, 0x00, sizeof(aucATC));
    ucTransType = SDK_ICC_TRANS_TYPE_GOODS;

    while (recorddatalen < apdu_r->LenOut) //parse data
    {
        if (((*pDOL) & 0x1F) == 0x1F) { ucTagLen = 2; }
        else { ucTagLen = 1; }

        if (!memcmp(pDOL, "\x9A", ucTagLen)) {
            memcpy(aucTransDate, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x21", ucTagLen)) {
            memcpy(aucTransTime, pDataOut, *(pDOL + ucTagLen));
        }

        else if (!memcmp(pDOL, "\x9F\x02", ucTagLen)) {
            memcpy(aucAuthAmount, pDataOut, *(pDOL + ucTagLen));
        }

        else if (!memcmp(pDOL, "\x9F\x03", ucTagLen)) {
            memcpy(aucOtherAmount, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x1A", ucTagLen)) {
            memcpy(aucCountryCode, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x5F\x2A", ucTagLen)) {
            memcpy(aucTransCurcyCode, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x4E", ucTagLen)) {
            memcpy(aucMerchantName, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9C", ucTagLen)) {
            memcpy(&ucTransType, pDataOut, *(pDOL + ucTagLen));
        }
        else if (!memcmp(pDOL, "\x9F\x36", ucTagLen)) {
            memcpy(aucATC, pDataOut, *(pDOL + ucTagLen));
        }
        pDataOut += (*(pDOL + ucTagLen));
        recorddatalen += (*(pDOL + ucTagLen));
        pDOL += ucTagLen + 1;
    }
    for (i = 0; i < 6; i++) {
        if (aucAuthAmount[i] != 0XFF)break;
        if (aucOtherAmount[i] != 0XFF) break;
    }

    if (i == 6) {
        return SDK_ERR;
    }
    else     //sxl ��ʾ������־
    {
        if (partno == 0) {
            sdkDispClearScreen();
            memset(aucBuff, 0x00, sizeof(aucBuff));
            tempnum = *ucRecordNum + 1;
            if ((tempnum) == 0x0a) (tempnum) = 0x10;
            sdkBcdToAsc(aucBuff, &tempnum, 1);
            strcpy((char *) (&aucBuff[2]), ":��������:");//sxl arm_linux
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransDate, 3);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);//sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "����ʱ��:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransTime, 3);
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memset(temp, 0x00, sizeof(temp));
            strcpy((char *) aucBuff, "���:");  //sxl arm_linux
            sdkRegulateAmount(temp, aucAuthAmount);
            strcat((char *) aucBuff, (char *) temp);//sxl arm_linux
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memset(temp, 0x00, sizeof(temp));
            strcpy((char *) aucBuff, "�����:");//sxl arm_linux
            sdkRegulateAmount(temp, aucOtherAmount);
            strcat((char *) aucBuff, (char *) temp);    //sxl arm_linux
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "�ն˹��Ҵ���:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucCountryCode, 2);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);//sxl arm_linux

            sdkDispBrushScreen();
        }
        else {
            sdkDispClearScreen();
            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "���׻��Ҵ���:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucTransCurcyCode, 2);
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL); //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "�̻�����:");
            sdkDispFillRowRam(SDK_DISP_LINE2, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);//sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            memcpy(aucBuff + strlen((char *) aucBuff), aucMerchantName, sizeof(aucMerchantName));
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);  //sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "��������:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), &ucTransType, 1);
            sdkDispFillRowRam(SDK_DISP_LINE4, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);//sxl arm_linux

            memset(aucBuff, 0x00, sizeof(aucBuff));
            strcpy((char *) aucBuff, "ATC:");
            sdkBcdToAsc(aucBuff + strlen((char *) aucBuff), aucATC, 2);
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, (char *) aucBuff,
                              SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL);//sxl arm_linux

            sdkDispBrushScreen();
        }
        return SDK_OK;
    }
}

/**************************************Function Description *****************************************
** ��������:
** ����           : 
** �������:
                            
** �������:
** ����ֵ      :
** ����           :
** ��������:
**
*******************************************************************************************/
s32 ReadIccTradeLog() {
    SDK_ICC_APDU_RESP LogStu[10];
    //SDK_ICC_APDU_RESP *Plog;
    u8 RelLogNum = 0, Logfmt[100], ucNowNum, key, RecordHalf;
    s32 retCode;

    memset(&LogStu, 0, sizeof(SDK_ICC_APDU_RESP) * 10);
    memset(Logfmt, 0, sizeof(Logfmt));
    // Plog=LogStu;

    Trace("wlz", "\r\n---------------��ȡIC��������ϸ--------\r\n");

    retCode = sdkIccReadLogDetail(&pstCardInfo.stCardParam, 10, Logfmt, LogStu, &RelLogNum);

    if (SDK_ReadLogEnd != retCode) return retCode;
    else {
        ucNowNum = 0;
        while (1) {
            RecordHalf = 0;
            retCode = DealReadLogRecord(&LogStu[ucNowNum], Logfmt, &ucNowNum, 0);
            if (retCode == SDK_ERR) {
                return SDK_ERR;
            }
            else                   //�Ѿ���ʾ�˼�¼
            {
                RecordHalf = 1;    //�Ѿ���ʾ��ǰ�벿��
                while (1) {
                    key = sdkKbGetKey();
                    if (key == SDK_KEY_ENTER)//ȷ�ϼ��鿴��һ��
                    {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        ucNowNum++;
                        if (ucNowNum == 10) ucNowNum = 0;
                        break;
                    }
                    else if (key == SDK_KEY_DOWN)//xule111215
                    {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        if (RecordHalf != 0) {
                            RecordHalf = 0;
                            DealReadLogRecord(&LogStu[ucNowNum], Logfmt, &ucNowNum,
                                              1);   //��ʾ�ڶ�����
                        }
                    }
                    else if (key == SDK_KEY_UP) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);

                        if (RecordHalf == 0) {
                            RecordHalf = 1;
                            DealReadLogRecord(&LogStu[ucNowNum], Logfmt, &ucNowNum,
                                              0);   //��ʾ�ڶ�����
                        }
                    }
                    else if (key == SDK_KEY_ESC) {
                        sdkSysBeep(SDK_SYS_BEEP_OK);
                        return SDK_OK;
                    }
                }
            }
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ʾ������Ľ��
** Parameters:          const u8 ucDispType ״̬
** Returned value:	void
** Created By:		����2012.07.27
** Remarks:
*****************************************************************************/
void DispReadCardRet(s32 const ucDispType) {
    u8 tmpdisp[64] = {0};
//lilin 20131219
    //LampTradeError();		mai del for debug 20140723
    switch (ucDispType) {
        case SDK_PARA_ERR:
            //strcpy(tmpdisp , "��������\n");
            strcpy(tmpdisp, "����ʧ��");
            break;

        case SDK_ERR:
            //strcpy(tmpdisp, "������ˢ��");
            break;

        case SDK_ICC_RESET_ERR:
            strcpy(tmpdisp, "��λʧ��");
            break;

        case SDK_QpbocDDAAuthFail:
            strcpy(tmpdisp, "DDA��֤ʧ��");
            break;

        case SDK_QpbocNoCard:
            strcpy(tmpdisp, "δ��ʾIC��");
            break;

        case SDK_QpbocMoreCards:
            strcpy(tmpdisp, "���ſ�");
            break;

        case SDK_QpbocAuthNoPerformed:
            strcpy(tmpdisp, "δ����˫����֤");
            break;

        case SDK_ECNotSupportErr:
            strcpy(tmpdisp, "�������ֽ𿨲�֧��");
            break;

        case SDK_EcLackBalanceErr:
            strcpy(tmpdisp, "����\n�������ֽ�");
            break;

        case SDK_UsePbocErr:
            strcpy(tmpdisp, "��ʹ��PBOC����");
            break;

        case SDK_CardBlockErr:
            strcpy(tmpdisp, "����");
            break;

        case SDK_NoAppSelErr:
            strcpy(tmpdisp, "��Ӧ�ÿɹ�ѡ��");
            break;

        case SDK_IccDataFormatErr:
            strcpy(tmpdisp, "���ݸ�ʽ��");
            break;

        case SDK_AppBlockErr:
            strcpy(tmpdisp, "Ӧ������");
            break;

        default:
            break;
    }

    strcat(tmpdisp, "\n����ʧ��\n");
    strcat(tmpdisp, "��[ȡ��]�˳�");
    sdkDispPromptBox(false, TV_AUTORET, tmpdisp);
}


/*********************************************************************************************************
** Descriotions: |��ʾˢ��
** parameters: |none
** Returned value: |none
** Created By: |����
** Remarks: |
*********************************************************************************************************/
s32 DispReadCard(const u16 ucCardModeEn) {
    u8 as_temp[64] = {0};
    u32 displine = SDK_DISP_LINE2; //�̶���ʾ�ڵڶ���
    u8 disptype = SDK_DISP_LDISP | SDK_DISP_FDISP | SDK_DISP_INCOL; //�̶���ʾ��������
    u8 num = 0;
    if ((ucCardModeEn & (SDK_ICC_MAG | SDK_ICC_ICC | SDK_ICC_RF | SDK_ICC_MANUAL)) == 0) {
        return SDK_PARA_ERR;
    }

    if (ucCardModeEn & SDK_ICC_MAG) { num++; }
    if (ucCardModeEn & SDK_ICC_ICC) { num++; }
    if (ucCardModeEn & SDK_ICC_RF) { num++; }
    if (ucCardModeEn & SDK_ICC_MANUAL) { num++; }

    strcpy(as_temp, "��");

    if (ucCardModeEn & SDK_ICC_ICC) {
        if (num == 1) { strcat(as_temp, "����IC��"); }
        else { strcat(as_temp, "����IC����"); }
        num--;
    }

    if (ucCardModeEn & SDK_ICC_MAG) {
        if (num == 1) { strcat(as_temp, "ˢ��"); }
        else { strcat(as_temp, "ˢ����"); }
        num--;
    }

    if (ucCardModeEn & SDK_ICC_RF) {
        if (num == 1) { strcat(as_temp, "�ӿ�"); }
        else { strcat(as_temp, "�ӿ���"); }
        num--;
    }

    if (ucCardModeEn & SDK_ICC_MANUAL) {
        strcat(as_temp, "���俨��");
    }
    strcat(as_temp, ":");
    sdkDispClearScreen();
    sdkDispFillRowRam(displine, 0, as_temp, disptype);
    sdkDispBrushScreen();

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		  2012.08.14
** Remarks: 		
*****************************************************************************/
s32 DispIccErr(s32 ErrType) {
    u32 displine = SDK_DISP_LINE2; //�̶���ʾ�ڵڶ���
    u8 disptype = SDK_DISP_LDISP | SDK_DISP_FDISP | SDK_DISP_INCOL; //�̶���ʾ��������
    u8 tmpdisp[50];
    bool flag = false;
    memset(tmpdisp, 0, sizeof(tmpdisp));
    s32 key = SDK_KEY_ENTER;
    switch (ErrType) {
        case    SDK_ERR:
            strcpy(tmpdisp, "����ʧ��,SDK ʧ��\n�����²���!");
            break;
        case    SDK_IccReturnErr:
            strcpy(tmpdisp, "����ʧ��,�������ݴ���\n�����²���!");
            break;
        case    SDK_NoAppSelErr:
            strcpy(tmpdisp, "����ʧ��,�޿�ѡӦ��");
            break;
        case    SDK_CancelTransErr:
            strcpy(tmpdisp, "����ʧ��,���ױ�ȡ��\n�����²���!");
            break;
        case    SDK_AppBlockErr:
            strcpy(tmpdisp, "����ʧ��,Ӧ�ñ���");
            break;
        case    SDK_PARA_ERR:
            strcpy(tmpdisp, "����ʧ��,��������\n���������ز˵�!");
            break;
            // case    SDK_TIME_OUT:                                                 strcpy(tmpdisp,"������ʱ");break;
        case    SDK_ESC:
            strcpy(tmpdisp, "����ȡ��\n�����²���!");
            break;
        case    SDK_ICC_BRUSH_ERR:
            strcpy(tmpdisp, "����ʧ��,ˢ������\n�����²���!");
            break;
        case    SDK_QpbocAuthNoPerformed:
            strcpy(tmpdisp, "����ʧ��,δ����˫����֤\n�����²���!");
            break;
        case    SDK_QpbocNoCard:
            strcpy(tmpdisp, "����ʧ��,��ⲻ����\n�����²���!");
            break;
        case    SDK_QpbocMoreCards:
            strcpy(tmpdisp, "����ʧ��,��⵽���ſ�\n�����²���!");
            break;
        case    SDK_ECNotSupportErr:
            strcpy(tmpdisp, "����ʧ��,�������ֽ�\n��֧�ָù���");
            break;
        case    SDK_CardBlockErr:
            strcpy(tmpdisp, "����ʧ��,IC������");
            break;
        case    SDK_TransOfflineDeclineErr:
            strcpy(tmpdisp, "����ʧ��,�����ѻ�\n�ܾ�");
            break;
        case    SDK_OnlineDecline:
            strcpy(tmpdisp, "����ʧ��\n�����²���!");
            break;
        case    SDK_QpbocDDAAuthFail:
            strcpy(tmpdisp, "����ʧ��,qpboc����\n��֤ʧ��");
            break;
        case    SDK_EcLackBalanceErr:
            strcpy(tmpdisp, "����ʧ��,�����ֽ�\n����");
            break;
        case    SDK_QpbocReadLastLogErr:
            strcpy(tmpdisp, "����ʧ��,qpboc����\n��һ����¼ʧ��");
            break;
        case    SDK_ECLoadErr:
            strcpy(tmpdisp, "����ʧ��,�����ֽ�\nȦ�����");
            break;
        case    SDK_AppExpiredErr:
            strcpy(tmpdisp, "����ʧ��,Ӧ��ʧЧ    ");
            break;
        case    SDK_AppEffectErr:
            strcpy(tmpdisp, "����ʧ��,Ӧ��δ��Ч");
            break;
        case    SDK_UsePbocErr:
            strcpy(tmpdisp, "����ʧ��,ʹ��PBOC\n�������");
            break;
        case    SDK_TransNotAcceptErr:
            strcpy(tmpdisp, "����ʧ�ܣ����ײ���\n����");
            break;
        case    SDK_TransTerminateErr:
            strcpy(tmpdisp, "����ʧ��,������ֹ\n�����²���!");
            break;
        case    SDK_TransOnlineDeclineErr:
            strcpy(tmpdisp, "����ʧ��,��������\n�ܾ�");
            break;
        case    SDK_InitAppNotAcceptErr:
            strcpy(tmpdisp, "����ʧ��,��ʼ��Ӧ\n�ò�����");
            break;
        case    SDK_AppTimeOutErr:
            strcpy(tmpdisp, "����ʧ��,��Ӧ�ó�ʱ\n�����²���!");
            break;
        case    SDK_NotAcceptErr:
            strcpy(tmpdisp, "����ʧ��,���񲻽���");
            break;
        case    SDK_IccCommandErr:
            strcpy(tmpdisp, "����ʧ��,���������\n�����²���!");
            break;
        case    SDK_IccDataFormatErr:
            strcpy(tmpdisp, "����ʧ��,���ݸ�ʽ��");
            break;
        case    SDK_PSENotFoundErr:
            strcpy(tmpdisp, "����ʧ��,û��PSEĿ¼");
            break;
        case    SDK_NoLogErr:
            strcpy(tmpdisp, "����ʧ��,�޼�¼");
            break;
        case    SDK_CDAFormatErr:
            strcpy(tmpdisp, "����ʧ��,CDA���ݸ�\nʽ��");
            break;
        case    SDK_IccDataRedundErr:
            strcpy(tmpdisp, "����ʧ��,���������ظ�");
            break;
        case    SDK_OfflineDecline:
            strcpy(tmpdisp, "����ʧ��,�ѻ��ܾ�");
            break;
//        case    SDK_OfflineApprove:                 strcpy(tmpdisp,"�ѻ���׼");break;
//        case    SDK_OnlineApprove:                  strcpy(tmpdisp,"������׼");break;
//        case    SDK_TransOnlineApprove:             strcpy(tmpdisp,"����������׼");break;
//        case    SDK_TransOfflineApprove:            strcpy(tmpdisp,"�����ѻ���׼");break;   
        case    SDK_OnlineWait:
            strcpy(tmpdisp, "��������������");
            break;
            //case	SDK_ICC_RESET_ERR:                  strcpy(tmpdisp,"����ʧ��,��λʧ��");break;
        case    SDK_ICC_RESET_ERR:
            strcpy(tmpdisp, "����ȷ����IC��!");
            break;
        default:
//                strcpy(tmpdisp,"����ʧ��,δ֪����");
            flag = true;
            break;
    }

    //syh 20150326 modify ��ʾIC��������Ϣ
    if (!flag) {
        strcat(tmpdisp, "\n��[ȷ��]����");
        sdkDispClearScreen();
        sdkDispLabel(displine, tmpdisp, disptype);
        //    DispManyLines( tmpdisp ,displine , disptype);
        sdkDispBrushScreen();
        //    ErrorLog("000", tmpdisp);

        Trace("wlz", "\r\n---��ʾ������Ϣ=%s---\r\n", tmpdisp);
        key = sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, SDK_ICC_TIMER_AUTO);
        //��ȷ�Ϻ�POS���ڴ������̣���ʱ����ʾ�����Ժ�...�������о���淶�ٸ���
        sdkDispClearScreen();
        sdkDispLabel(SDK_DISP_LINE3, "���Ժ�...", SDK_DISP_CDISP | SDK_DISP_FDISP);
        sdkDispBrushScreen();
    }
    return key;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		88888888888888  2012.08.21
** Remarks: 		
*****************************************************************************/
s32 FormTLV(u8 *DesTag, u32 *DesTagLen, const u8 *pheTag, u8 *pheOutData, u32 *puiOutLen) {
    u8 *Dptr = DesTag;
    if (DesTag == NULL || pheTag == NULL || pheOutData == NULL || puiOutLen == NULL ||
        DesTagLen == NULL)
        return UNSUCCESS;

    while ((*pheTag) != '\0') {
        *Dptr++ = *pheTag++;
        (*DesTagLen) = (*DesTagLen) + 1;
    }

    //DesTagLen=strlen(DesTag);
    if ((*puiOutLen) < 128) {
        (*Dptr++) = *puiOutLen;
        (*DesTagLen) = (*DesTagLen) + 1;
    }
    else {
        sdkU32ToHex(Dptr, *puiOutLen, 2);
        Dptr += 2;
        (*DesTagLen) = (*DesTagLen) + 2;
    }
    memcpy(Dptr, pheOutData, *puiOutLen);
    (*DesTagLen) = (*DesTagLen) + (*puiOutLen);
    return SUCCESS;
}

/*****************************************************************************
** Descriptions:	SaveOfflineBag
** Parameters:    	Input: None 
** Returned value:	
** Created By:		
** Remarks: 		
                    �����ѻ����װ�
*****************************************************************************/
s32 SaveOfflineBag(u8 *TraCode, u32 OffMode) {
/*
    u32 result;
    ST_OffLineTrade  OffTradeinfo;
    u8 temp[256],RMode;
    u16 tLen;
    result=ZipPagData(0,temp,&tLen);

    Trace("wlz","\r\n----ZipPagData  result--------\r\n");
    Trace("wlz","\r\n----��ȡ55����=");
    TraceHex("wlz",temp,tLen);

    if(SDK_ICC_ICC==pstCardInfo.stCardParam.ucCardMode)    RMode=0x01;
    else   RMode=0x02;

    if(SDK_OfflineDecline==OffMode)     OffMode=0x03;
    else if(SDK_TransOfflineApprove==OffMode)     OffMode=0x02;
	else if(SDK_TransOK_ARPCErr==OffMode)     OffMode=0x01;
    memset(&OffTradeinfo,0,sizeof(OffTradeinfo));
    OffTradeinfo.OffType=OffMode;//�ѻ�ʧ��
    OffTradeinfo.RcardMode=RMode;//����ģʽ
    OffTradeinfo.UpFlag=0;//���ͱ�־ 1 �����ͣ�0 δ����
    memcpy(OffTradeinfo.TradeVou,gstSys.asVoucherNo,3);//��ˮ
 	if(OffMode!=0x01) 
    {
        sdkBcdAdd(gstSys.asVoucherNo, "\x00\x00\01",3); //����APPC���ʱ����ˮ�ż�1 ,���ѻ���ʱ��Ҫ�ۼ���ˮ
        SavePosCfg();
    }
    else
    {
        sdkBcdSub(OffTradeinfo.TradeVou, "\x00\x00\01",3); //ARPC��Ϊ��������������ˮҪ��1
    }
	memcpy(OffTradeinfo.TradeCode,TraCode,3);//���״���
	OffTradeinfo.TagLen=tLen;//TLV���ݳ���
	memcpy(OffTradeinfo.TagLenVal,temp,tLen);//TLV����
	result=SaveOfflineTrade(&OffTradeinfo);
	if(result!=RSLT_OK)
	{
	        Trace("wlz","\r\n----�����ѻ���ʧ��--------\r\n");
	}
   return  result;*/
    return SDK_OK;    //û���ѻ����ף���ʱע�͵�
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		88888888888888  2012.09.11
** Remarks: 		
*****************************************************************************/
void QpbocGetOnline(ST_TRANSFLOW *TransFlow) {
#if 0
    TransFlow->bOnline=1;
    TransFlow->ucInsRunStep = 9;
    TransFlow->QpbocOnlineFlag=1;
    memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));
    //��ǰΪIC������IC����
    if(SDK_ICC_ICC==(pstCardInfo.stCardParam.ucCardMode))
    memcpy(TransFlow->heInsOutputData,"\x40\x00\x01\x00\x00",5);//�Ӵ�ʽ
    else  memcpy(TransFlow->heInsOutputData,"\x20\x00\x01\x00\x00",5);//�ǽ�
    TransFlow->usInsOutputDataLen = 5;
#endif
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		88888888888888  2012.09.11
** Remarks: 		
*****************************************************************************/
void QpbocOfflineProve(ST_TRANSFLOW *TransFlow) {
#if 0
    TransFlow->QpbocOnlineFlag=2;
    memset(TransFlow->heInsOutputData,0,sizeof(TransFlow->heInsOutputData));
    //��ǰΪIC������IC����
     if(SDK_ICC_ICC==(pstCardInfo.stCardParam.ucCardMode))
    memcpy(TransFlow->heInsOutputData,"\x40\x00\x01\x00\x00",5);//�Ӵ�ʽ
     else  memcpy(TransFlow->heInsOutputData,"\x20\x00\x01\x00\x00",5);//�ǽ�
    TransFlow->usInsOutputDataLen = 5;
#endif
}

/*****************************************************************************
** Descriptions:	SposIccReadCard
** Parameters:    	
** Returned value:	
** Created By:		
** Remarks: 
                    ���������������������룬�忨
                    �ǽӣ���������
                    ����֧��ʱ�� ����G101��Qpboc������G101������
                    ��ΪK3306R���������������ʱ����
                    �������QPBOC
*****************************************************************************/
s32 SposIccReadCard(u32 uiOverTime, u8 ucCardModeEn, SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                    SDK_ICC_PARAM *pstIccParam, SDK_ICC_CARDDATA *pstCardData, const u32 eDispRow) {
/*
    u8 heUID[64] = {0}; //heUID[0]:UID�ĳ��ȣ�heUID[1~x]:ΪUID������
    u8 temp[64] = {0};
    u8 handLen = 0, handData[128] = {0};
    u8 flag;
    u8  key;
    u32 timerID;
    s32 rslt;

//emv������ʼ�������������Ϊ�����п��ܱ��ı䣬�����������������һ�£������һ�£�
//���ϵ�ǰ���¶Զ�������ֵ
    if((ucCardModeEn & SDK_ICC_ICC) || (ucCardModeEn & SDK_ICC_RF))
    {
        if(sdkIccTransInit(pstIccTradeParam) != SDK_OK)//IC��������ʼ��
        {
            return SDK_ERR;
        }
    }
    if(ucCardModeEn & SDK_ICC_MAG)
    {
        sdkIccOpenMag();
    }

    if(ucCardModeEn & SDK_ICC_MANUAL)
    {
        LcdDisplayRow(eDispRow, 0, "_", SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
    }
    timerID = sdkTimerGetId();
	DefaultTradeCardInfo();
	
    while(1)
    {
        if(ucCardModeEn & SDK_ICC_MAG)
        {
            rslt = sdkIccGetMagData(pstIccParam, pstCardData);
            if(rslt == SDK_OK) //�ɹ�����
            {
                return rslt;
            }
            else if(rslt == SDK_ICC_BRUSH_ERR) //�ɼ�������
            {
               return SDK_ICC_BRUSH_ERR;
            }
            else if(rslt != SDK_ICC_NOCARD) //ʧ���˳�
            {
                return SDK_ERR;
            }
        }

        if(ucCardModeEn & SDK_ICC_ICC)
        {
            rslt = sdkIccGetCardStatus(pstIccParam);

            if(rslt == SDK_OK) //�ɹ�����
            {
                return rslt;
            }
            else if(rslt != SDK_ICC_NOCARD) //ʧ���˳�
            {
                return SDK_ERR;
            }
        }

        if(ucCardModeEn & SDK_ICC_RF) //�ǽӶ���
        {
            rslt = sdkIccPowerOnAndSeek(pstIccTradeParam, pstIccParam, heUID);

            if(rslt == SDK_OK)
            {
                if(bIsUseQreader) //qpboc�ڶ��������洦�����(����G101������stm32)
                {
                    return sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
                }
                else if((pstIccTradeParam->bIsSupportQPBOC) && (pstIccTradeParam->bIsSupportCLPBOC == FALSE)
                        && (memcmp(pstIccTradeParam->pasTradeAmount, "\x00\x00\x00\x00\x00\x00", 6))
                        && (pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW))
                {
                    return sdkIccTransFlow1(pstIccTradeParam, pstIccParam);
                }
                return rslt;
            }
            else if(rslt != SDK_ICC_NOCARD && rslt != SDK_ICC_MORECARD)    //�޿���࿨������Ѱ��
            {
                return SDK_ERR;
            }
        }
        
         key = sdkKbGetKey();
        if(ucCardModeEn & SDK_ICC_MANUAL) //���俨��
        {
           
            if(key != 0)
            {
                flag = FALSE;

                if(key == ENTER)
                {
                    if(handLen >= 13)
                    {
                        Beep(SDK_SYS_BEEP_OK);
                        pstIccParam->ucCardMode = SDK_ICC_MANUAL;
                        sprintf(pstCardData->asManualLen, "%d", handLen);
                        memcpy(pstCardData->asManualData, handData, handLen);
                        pstCardData->bIsManualInput =1;
                        return SDK_OK;
                    }
                    else if(handLen<13)
                    {
                        LcdDisplayRow(eDispRow,0,"���Ų���С��13λ",SDK_DISP_FDISP|SDK_DISP_RDISP|SDK_DISP_INCOL);
                    }
                }
                else if(key == CLEAR || key == BACKSPACE || (key >= DIGITAL0 && key <= DIGITAL9))
                {
                    if(key == CLEAR)
                    {
                        flag = TRUE;
                        handLen = 0;
                        memset(handData, 0, sizeof(handData));
                    }
                    else if(key == BACKSPACE)
                    {
                        if(handLen > 0)
                        {
                            flag = TRUE;
                            handData[handLen] = 0;
                            handLen--;
                        }
                    }
                    else if(key >= DIGITAL0 && key <= DIGITAL9)
                    {
                        if(handLen < 19)
                        {
                            flag = TRUE;
                            handData[handLen++] = (u8)key;
                        }
                    }
                    memset(temp, 0, sizeof(temp));

                    if(handLen > 0)
                    {
                        memcpy(temp, handData, handLen);
                    }
                    else
                    {
                        temp[0] = '_';
                    }

                    LcdDisplayRow(eDispRow, 0, temp, SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL);
                }
                Beep(flag);
            }

            
        }
        if(key == ESC)
        {
            pstIccParam->ucCardMode = 0;
            return SDK_ESC;
        }
        if(1 == sdkTimerIsEnd(timerID, uiOverTime))
        {
            return SDK_TIME_OUT;
        }
    }
    */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		88888888888888  2012.11.25
** Remarks: 		
*****************************************************************************/

s32 SaveOfftradeDetail(u8 *Namechar, u8 Namlen) {
/*    _TRADEDETAIL TradeDetail;
    u8 VoucherNotmp[4]={0},tempdata[128];
    u8 tmptag[4][3]={
                                    "\x5a\x00",
                                    "\x9f\x02",
                                    "\x9a\x00",
                                    "\x9f\x21",
                                    };
    s32 result=0,temptaglen=0;
    memset(&TradeDetail,0,sizeof(_TRADEDETAIL));
    TradeDetail.Debitflag=0x01;//������
    TradeDetail.Cardflag=0x00;//���⿨���
    TradeDetail.Namelen=Namlen;//���Ƴ���
    if(TradeDetail.Namelen>=60)
    {Namlen=60;}
    else
    {Namlen=TradeDetail.Namelen;}

    memcpy(TradeDetail.Name,Namechar,Namlen);//��������

    memset(tempdata,0,sizeof(tempdata));
    temptaglen=0;
    result= sdkIccReadTLV(tmptag[0],tempdata,&temptaglen);
    memset(TradeDetail.CardNo,0xff,sizeof(TradeDetail.CardNo));//��Ҫ��ʽ��Ϊ0xff
    memcpy(TradeDetail.CardNo,tempdata,temptaglen);//���˺�

    memcpy(VoucherNotmp,Sys.VoucherNo,3);
    sdkBcdSub(VoucherNotmp, "\x00\x00\01",3);
    memcpy(TradeDetail.VoucherNo,VoucherNotmp,3);//��ˮ��
    memcpy(TradeDetail.AuthCode,"000000",6);//��Ȩ��
    memcpy(TradeDetail.SysRefNo,"000000",12);//ϵͳ�ο���

    memset(tempdata,0,sizeof(tempdata));
    temptaglen=0;
    result= sdkIccReadTLV(tmptag[1],tempdata,&temptaglen);
    memcpy(TradeDetail.TradeAmount,tempdata,6);//���

    memset(tempdata,0,sizeof(tempdata));
    temptaglen=0;
    TradeDetail.Time[0]=0x20;//Ĭ��Ϊ20_ _��
    result= sdkIccReadTLV(tmptag[2],tempdata,&temptaglen);
    memcpy(&TradeDetail.Time[1],tempdata,3);//����
    memset(tempdata,0,sizeof(tempdata));
    temptaglen=0;
    result= sdkIccReadTLV(tmptag[3],tempdata,&temptaglen);
    memcpy(&TradeDetail.Time[4],tempdata,3);//ʱ��

    return  SaveTradeDetail(&TradeDetail);
*/
    return SDK_OK;
}

/**************************************Function Description *****************************************
** ��������:InitLoadTrade
** ����:  ��ʼ��Ȧ�潻��
** �������:
** �������:
** ����ֵ:
** ����:
** ��������:
**
*******************************************************************************************/
void InitLoadTrade(void) {
    GLoadType = 0;//Ȧ���־λ��ʼ��0��
    ConfirmINS45 = 0;
}

/******************************************************************************
**��������:  void SetIsICOnLineTrade(u8 Type)
**��������:  ���ý�������ΪIC  ����������
**�������:     
                            .type: FALSE ��  ��TRUE= ��
**�������: ��
**          
**����     ֵ:   ��
*******************************************************************************/
void SetIsICOnLineTrade(u8 Type) {
    gIsICOnLineTrade = Type;
}

/******************************************************************************
**��������: u8 GetetIsICOnLineTrade()
**��������:  ��ý������ͣ�ȷ���Ƿ���IC����������
**�������: ��
**�������: ��
**          
**����     ֵ: FALSE = ��  ��TRUE= ��
*******************************************************************************/
u8 GetIsICOnLineTrade() {
    Trace("comm", "gIsICOnLineTrade %d\r\n", gIsICOnLineTrade);
    return gIsICOnLineTrade;
}

/*****************************************************************************
** Descriptions:        ��ICC����֯ICC�ŵ�����
** Parameters:          SDK_ICC_CARDDATA *pstCardData
** Returned value:
** Created By:		lilin2012.08.18
** Remarks:
*****************************************************************************/
s32 GetIccMagdata(SDK_ICC_CARDDATA *pstCardData) {
    u16 i = 0, j = 0;
    u32 len = 0;
    u8 temp[256] = {0};

    len = 256;
    if (sdkIccReadTLV("\x5F\x24", temp, &len) > 0) //������Ч��//����YYMMDD
    {
        TraceHex("GetIccMagdata", "tag 5f24 ", temp, len);
        if (len > 0) {
            sdkBcdToAsc(pstCardData->asExpiredDate, &temp[0], 2);
        }
    }

    memset(temp, 0, sizeof(temp));
    len = 256;
    if (sdkIccReadTLV("\x5A", temp, &len) > 0) //��Pan��Ϣ
    {
        TraceHex("GetIccMagdata", "tag 5A ", temp, len);
        if (len) {
            pstCardData->ucCardLen = len * 2;
            sdkBcdToAsc(pstCardData->asCardNO, temp, len);
            //zxx 20130730 14:45 �����жϣ�����λ�Ŀ��ŲŲ���ȡ��
            if ((temp[len - 1] & 0x0f) == 0x0f) {
                pstCardData->asCardNO[pstCardData->ucCardLen - 1] = '\0';
            }
            Trace("zxx", "pstCardData->asCardNO = %s\r\n", pstCardData->asCardNO);
        }
        else {
            Trace("GetIccMagdata", "tag 5Alenerror\r\n");
            Assert(0);
        }
    }
    else {
        Trace("GetIccMagdata", "get tag 5Aerror\r\n");
    }

    memset(temp, 0, sizeof(temp));
    len = 256;
    if (sdkIccReadTLV("\x57", temp, &len) > 0) //��IC2����Ϣ
    {
        TraceHex("GetIccMagdata", "tag 57 ", temp, len);
        if (len > 0) {
            for (i = len - 1; i > 1; i--) {
                if (temp[i] != 0xff) {
                    break;
                }
            }

            j = i + 1;

            if ((temp[j - 1] & 0x0f) == 0x0f) {
                i = (j * 2 - 1);
            }
            else {
                i = (j * 2);
            }

            if (i > 37) {
                i = 37;                                                        // ��ֹ������� zjz20101201
            }
            pstCardData->ucTrack2Len = i;
            sdkBcdToAsc(pstCardData->asTrack2Data, temp, len);
            pstCardData->bIsIccMagCard = true;
            Trace("GetIccMagdata", "tag 57  len=%d  data=%s\r\n", pstCardData->ucTrack2Len,
                  pstCardData->asTrack2Data);
        }
    }
    return SDK_OK;
}
