#include "appglobal.h"

//static SDK_8583_ST8583 gstConSendBag;

/*****************************************************************************
 ** Descriptions:	回写消费产生的其他特殊的信息
 ** Parameters:    	SDK_8583_ST8583 *pstBag8583
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteSaleMsg(SDK_8583_ST8583 *pstSendBag8583,
		SDK_8583_ST8583 *pstRevBag8583) {
	s32 nRet = SDK_ERR;

	if (pstSendBag8583 == NULL || pstRevBag8583 == NULL) {
		return SDK_ERR;
	}

	//写入借贷机，内外卡标志
	nRet = WriteDebitOrCreditType(TRADEID_SALE);
	if (nRet < SDK_OK) {
		return 0;
	}

	nRet = WriteCardInOrOut(pstSendBag8583, pstRevBag8583);
	if (nRet < SDK_OK) {
		return 0;
	}

	//回写接收包
	nRet = WriteRevBag(pstRevBag8583);
	if (nRet < SDK_OK) {
		return 0;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	初始化消费交易的参数
 ** Parameters:    	struct _TradeData *pstTradeData
 ** Returned value:
 修改 wangb  2016.03.08
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 InitSaleTradeParam(struct _TradeData *pstTradeData) {
	s32 ret = SDK_ERR, len;
	u8 value[512], temp[512] = { 0 };

	if (pstTradeData == NULL) {
		return SDK_ERR;
	}

	//交易id
	pstTradeData->ucTradeId = TRADEID_SALE;

	//金额
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_AMOUNT, value)) > 0) {
		memcpy(pstTradeData->asAmount, value, len);
	} else {
		return SDK_ERR;
	}

	//输入方式或者用卡模式,这个地方建议修改为两个标签分开，
	//一个是用卡方式，一个是有没有密码。
#if 1 /*Modify by huacong at 2016.03.04  10:7 */
	memset(value, 0, sizeof(value));
	if (ProviderGetTagValue(TAG_8583_CardMode, value)) {
//          Log("wangb","TAG_8583_CardMode is %s ",value);
		if (strstr(value, TAG_CardMAG)) {
			pstTradeData->strCardInfo.stCardParam.ucCardMode = SDK_ICC_MAG;
//			pstTradeData->strCardInfo.stCardParam.ucCLType = ICCPBOCNORMAL; //FIXME
		} else if (strstr(value, TAG_CardICC)) {
			pstTradeData->strCardInfo.stCardParam.ucCardMode = SDK_ICC_ICC;
			pstTradeData->strCardInfo.stCardParam.ucCLType = ICCPBOCNORMAL; //FIXME
		} else if (strstr(value, TAG_CardRF)) {
			pstTradeData->strCardInfo.stCardParam.ucCardMode = SDK_ICC_RF;
			pstTradeData->strCardInfo.stCardParam.ucCLType = ICCQPBOCFLOW; //FIXME
		} else if (strstr(value, TAG_IntpCard)) {
			pstTradeData->strCardInfo.stCardParam.ucCardMode = SDK_ICC_MANUAL;
//			pstTradeData->strCardInfo.stCardParam.ucCLType = ICCPBOCNORMAL; //FIXME
		}
	}

#endif /* if 0 */
//    pstTradeData->strCardInfo.stCardParam.ucCardMode = SDK_ICC_MAG;

	//卡号
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_CARDNO, value)) > 0) {
		pstTradeData->strCardInfo.stCardData.ucCardLen = len;
		memset(temp, 0, sizeof(temp));
		// sdkBcdToAsc(temp, value,  (pstTradeData->strCardInfo.stCardData.ucCardLen + 1)/2);
		memcpy(pstTradeData->strCardInfo.stCardData.asCardNO, value,
				pstTradeData->strCardInfo.stCardData.ucCardLen);
	}

	//磁道
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_Track2Data, value)) > 0) {
		pstTradeData->strCardInfo.stCardData.ucTrack2Len = len;
		memcpy(pstTradeData->strCardInfo.stCardData.asTrack2Data, value,
				pstTradeData->strCardInfo.stCardData.ucTrack2Len);
	}

	//磁道
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_Track3Data, value)) > 0) {
		Log("wangb", "TAG_8583_Track3Data is %d ", len);
		pstTradeData->strCardInfo.stCardData.ucTrack3Len = len;
		memset(temp, 0, sizeof(temp));
		// sdkBcdToAsc(temp, value, (pstTradeData->strCardInfo.stCardData.ucTrack3Len + 1)/2);
		memcpy(pstTradeData->strCardInfo.stCardData.asTrack3Data, value,
				pstTradeData->strCardInfo.stCardData.ucTrack3Len);
	}

	//有效期
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_ExpiredDate, value)) > 0) {
		memcpy(pstTradeData->strCardInfo.stCardData.asExpiredDate, value, len);
	}

	//卡序列号
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_SYS_CardID, value)) > 0) {
		memcpy(pstTradeData->strCardInfo.stCardData.ucUID, value, len);
	}

	//密码
	memset(value, 0, sizeof(value));
	if ((len = ProviderGetTagValue(TAG_8583_CreditPwd, value)) > 0) {
		pstTradeData->heCreditPwd[0] = len;
		memcpy(&pstTradeData->heCreditPwd[1], value, len);
		//   sdkBcdToAsc(&pstTradeData->heCreditPwd[1], value, (len + 1)/2);
	}

}

/*****************************************************************************
 ** Descriptions:	组消费包
 ** Parameters:          struct _TradeData *pstSaleData
 u8 *pheSendData
 ** Returned value:
 ** Created By:		lilin2012.08.01
 ** Remarks:
 *****************************************************************************/
s32 FormSendSaleBag(struct _TradeData *pstTradeData,
		SDK_8583_ST8583 *pstBag8583) {
	u8 ucTradeId;    //消息ID
	u8 buf[64] = { 0 };
	int nRet;
	s32 len = 0;
	u16 length = 0;

	s32 tmplen = 0;
	u8 temp[256] = { 0 };
	u8 str[256] = { 0 };

	ucTradeId = pstTradeData->ucTradeId;

	if (pstBag8583 == NULL || pstTradeData == NULL) {
		return false;
	}

	nRet = FormPublicSend(pstBag8583, pstTradeData, "7024068030C09211");
	if (nRet <= 0) {
		return 0;
	}

	len = SetFieldData(pstBag8583, SDK_8583_FIELD_MSG, "0200", 4);
	if (len <= 0) {
		return 0;
	}

	// 3域
	len = SetFieldData(pstBag8583, 3, "000000", 6);
	if (len <= 0) {
		return 0;
	}

	//25域
	SetFieldData(pstBag8583, 25, "00", 2);

	//58域 定位
	memset(str, 0, sizeof(str));
	length = 0;
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_JINGDU, temp)) > 0) {
		memcpy(str, "GA", 2);
		Log("zbq","11111");
		length = length + 2;
		sprintf(str+length,"%03d", len);
		Log("zbq","22222");
		length = length + 3;
		memcpy(str+length, temp, len);
		Log("zbq","33333");
		length = length + len;
	} else {
		return SDK_ERR;
	}
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_WEIDU, temp)) > 0) {
		memcpy(str+length, "GB", 2);
		Log("zbq","44444");
		length = length + 2;
		sprintf(str+length,"%03d", len);
		Log("zbq","55555");
		length = length + 3;
		memcpy(str+length, temp, len);
		Log("zbq","666666");
		length = length + len;
	} else {
		return SDK_ERR;
	}
	memset(temp,0,256);
	sdkBcdToAsc(temp,str,length);
	Log("zbq","58域 =  %s ", temp);
	SetFieldData(pstBag8583, 58, str, length);

//	memset(&gstTempBag8583,0,sizeof(SDK_8583_ST8583));
//	memset(&gstTempBag8583,pstBag8583,sizeof(SDK_8583_ST8583));  //保存未加密的报文到全局暂存报文中

#ifdef EncriptBagControl  //如果要全报文加密，则需要提前保存发送报文
	nRet = WriteSendBag(TAG_8583_SENDBAG, pstBag8583);
#endif
	//MAC
	nRet = EncryptSendData(pstTradeData, pstBag8583);
	if (nRet == SDK_ESC) {
		return SDK_ESC;
	}
	//重新改写报文头
	ReConfigBag8583(pstBag8583);
#ifdef EncriptBagControl
	//全报文加密
	EncryptAllMsgByHX(1, pstBag8583->ucBagData, &pstBag8583->nBagLen);
#endif

	return pstBag8583->nBagLen;
}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	u8 *indata
 u32 len
 u8 *outdata
 u32 *rlen
 消费入口
 wangb 2016.03.01
 ** Returned value:
 ** Created By:		修改 huacong  2016.03.04
 ** Remarks: 		参数会进行修改
 *****************************************************************************/
s32 Salestart(u8 *indata, u32 len, u8 *outdata, u32 *rlen) {
	Log("zbq", "Jump into Salestart");

	s32 nRet = 0;
	u16 BagLen = 0;

	SDK_8583_ST8583 stSendBag8583;

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));
	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));
	u8 temp[SDK_8583_BAGMAXLEN];

	gstTradeData.ucTradeId = TRADEID_SALE;

	//初始化系统参数
	nRet = InitSysParam();
	/*     if(nRet < SDK_OK)
	 {
	 //异常提示后续修改
	 return nRet;
	 } */
	Log("wangb", "InitSysParam is %d ", nRet);

	//判断是否有脚本 是否有冲正
	nRet = DealSendBagAhead(&gstTradeData);
	if (nRet == SDK_EQU) {
		return SDK_OK;
	} else if (nRet == SDK_ESC) {
		return SDK_ERR;
	}

	//初始化消费的交易参数
	nRet = InitSaleTradeParam(&gstTradeData);
//    if(nRet != SDK_OK)
//    {
	//异常提示后续修改
//        return nRet;
//    }

	//组消费报文
	nRet = FormSendSaleBag(&gstTradeData, &stSendBag8583);
	if (nRet <= SDK_EQU) {
		return nRet;
	}
	nRet = stSendBag8583.nBagLen;
//    if(nRet <= 0)
//    {
//       //所有的异常应该都抛上去吧
//        return nRet;
//    }
	//回写报文
#ifdef EncriptBagControl
	nRet = WriteSendBag(TAG_8583_SENDECRIPTBAG, &stSendBag8583);
#else
	nRet = WriteSendBag(TAG_8583_SENDBAG,&stSendBag8583);
#endif

//    nRet = WriteSendBag(&stSendBag8583);
//    if(nRet <= SDK_OK)
//    {
//       //所有的异常应该都抛上去吧
//        return nRet;
//    }
	//获取未加密的发送包，作为冲正包保存
	memset(temp, 0, sizeof(temp));
	memset(&gstTempBag8583, 0, sizeof(SDK_8583_ST8583));
	BagLen = ProviderGetTagValue(TAG_8583_SENDBAG, temp);
	memcpy(gstTempBag8583.ucBagData, temp+2, BagLen-2);
	gstTempBag8583.nBagLen = BagLen-2;
	if(ParseField8583(&gstTempBag8583) < SDK_EQU)
   {
	   return SDK_ERR;
   }
	//从全局暂存报文中获取报文
	nRet = SaveReversalBag(true, &gstTempBag8583, &gstTradeData);
	if (nRet != SDK_OK) {
		return SDK_ESC;
	}
	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	把响应码,发卡行认证数据,脚本导入emv库
 ** Parameters:          bool BisOK报文是否正确
 ** Returned value:
 ** Created By:		lilin2012.08.20
 ** Remarks:     响应码错才会走联机交易，报文错直接走脱机交易
 *****************************************************************************/
void EmvImportRespCode(bool BisOK, SDK_8583_ST8583 *pstRevBag) {
	u8 code[3] = { 0 };

	if (BisOK) {
		GetFieldData(pstRevBag, 39, code, 3);
		//zxx 20131014 如果是有缺陷的成功，转成00给sdkemv，不然下面会做联机拒绝处理
		if (CheckResponseCode(code)) {
			memset(code, 0, sizeof(code));
			strcpy(code, "00");
		}
	}
//    Trace("EmvImportRespCode", "code=%s  BisOK=%d\r\n", code, BisOK);
	Private_sdkIccImportRespCode(BisOK, code);
}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:
 打印入口
 wangb 2016.03.15
 ** Returned value:

 *****************************************************************************/
s32 printmag() {
	u8 buf[32] = { 0 };
	u8 cmd[128] = { 0 };
	u8 src[32] = { 0 };
	u8 Amount[16] = { 0 };
	s32 result;
	initPrint();
	writePrintText('c', "------------------          \n");
	writePrintText('c', "持卡人存根     CARDHOLDER COPY\n");

	magdata(cmd, "商户名:               ", TAG_8583_MerchantNo);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);
	magdata(cmd, "终端号:               ", TAG_8583_TerminalNo);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);
	magdata(cmd, "操作员:                  ", TAG_8583_UsManNo);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);
	writePrintText('c', "发卡行:               90880038\n");
	writePrintText('c', "收单行:             交通银行\n");

	magdata(cmd, "卡号(CARD):", TAG_8583_CARDNO);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);

	writePrintText('c', "交易类型(TRAN TYPE):          \n");
	writePrintText('c', "                   消费(SALE)\n");

	memset(cmd, 0, 128);
	magdata(cmd, "批次号:                ", TAG_8583_AsBatchNo);
	writePrintText('c', cmd);

	writePrintText('c', "凭证号:              000065\n");
	writePrintText('c', "授权号:                123456\n");
//    writePrintText('c', "参考号:          210031895667\n");
	memset(cmd, 0, 128);
	magdata(cmd, "参考号: ", TAG_8583_SysRefNo);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);

	strcat(cmd, "日期/时间: ");
	magdata(buf, "", TAG_8583_MD);
	if (strlen(buf) >= 4)
		sprintf(src, "%c%c-%c%c", buf[0], buf[1], buf[2], buf[3]);
	strcat(cmd, src);
	memset(buf, 0, 32);
	memset(src, 0, 32);

	magdata(buf, "", TAG_8583_HMS);
	if (strlen(buf) >= 6)
		sprintf(src, "  %c%c:%c%c:%c%c", buf[0], buf[1], buf[2], buf[3], buf[4],
				buf[5]);
	strcat(cmd, src);
	writePrintText('c', cmd);
	memset(cmd, 0, 128);
	memset(buf, 0, 32);

	result = ProviderGetTagValue(TAG_8583_AMOUNT, cmd);
	if (result > 0) {
		sdkAscToBcd(buf, cmd, result);
		memset(cmd, 0, 128);
		strcpy(cmd, "金额(AMOUNT):   RMB  ");
		BcdAmount2AscAmount(buf, &cmd[strlen(cmd)]);
		strcat(cmd, "\n");
		writePrintText('c', cmd);
	}

	writePrintText('c', "备注/REFERENCE    \n");
	writePrintText('c', "中国银联银联钱包签购            \n");
//    result = startPrint();

	return result;

}
/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	u8 *indata
 u32 len
 u8 *outdata
 u32 *rlen
 消费入口
 wangb 2016.03.08
 ** Returned value:
 ** Created By:		修改 huacong  2016.03.04
 ** Remarks: 		参数会进行修改
 *****************************************************************************/
s32 SaleParse8583(u8 *indata, u32 len, u8 * outdata, u32 *rlen) {
	Log("zbq", "Jump into SignParse8583");

	s32 netstate = SDK_EQU;  //网络状态及返回状态
	bool bisResversal = false;
	s32 nRet = 0;
	u8 recvbuf[2048] = { 0 };
	SDK_8583_ST8583 stRevBag8583;
	SDK_8583_ST8583 stSendBag8583;

	u8 aucTempBag[2048] = { 0 };

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));

	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));
	memset(&stRevBag8583, 0, sizeof(SDK_8583_ST8583));

	gstTradeData.ucTradeId = TRADEID_SALE;

	//初始化系统参数
	nRet = InitSysParam();
	/*     if(nRet < SDK_OK)
	 {
	 //异常提示后续修改
	 return nRet;
	 } */
	//初始化消费的交易参数
	nRet = InitSaleTradeParam(&gstTradeData);
//    if(nRet != SDK_OK)
//    {
	//异常提示后续修改
//        return SDK_ERR;
//    }
//先检查是否有提前数据
	//读取发送包，这个地方要考虑是用全局还是局部，还有会不会存在读取内存
	//发送包的时候有串包的情况
	nRet = ReadSendBag(&stSendBag8583);
	if (!nRet) {
		return nRet;
	}
	nRet = ReadRevBag(&stRevBag8583);

	if (indata != NULL) {
		netstate = indata[0];
		gstTradeData.netstate = GetCurCommuStatue(indata[0]);
	} else {
		netstate = NET_UNCONNECT;
	}

	//判断是否是脚本 是否是冲正
	nRet = DealReversalBag(&stRevBag8583, &stSendBag8583, &gstTradeData,
			&netstate);
	if (nRet != SDK_OK) {                        //解析失败返回状态(netstate = 100 继续发包)
		return netstate;
	} else if (netstate == SDK_LOD) {
		return netstate;                                     //解析正确判断是否脚本或者冲
	}                                                                //脚本或者冲继续发包

	//校验包
	nRet = CheckRecBag(&stSendBag8583, &stRevBag8583);
	if(nRet == ERR_RESPONSE)
	{
		  if (gstTradeData.netstate == SDK_TIME_OUT)
		  {
			 GetTimeResponseError(outdata,NET_TIMEOUT);
		  }
		  else if (gstTradeData.netstate == SDK_COMM_WAVELOST)
		  {
			  GetTimeResponseError(outdata,NET_INTERRUT);
		  }
		  else
		  {
			   GetStrResponseError(outdata,&stRevBag8583);
		  }
		  WriteReversalFlag(false);
		  *rlen = strlen(outdata);
		  return SDK_ERR;
	}
	if (JudgeIsNeedReversal(nRet)) {
		bisResversal = true;
	}
	//IC卡全流程后续处理
	if (gstTradeData.strCardInfo.stCardParam.ucCardMode == SDK_ICC_ICC) {
		nRet = PostPag55Data(&stRevBag8583, &gstTradeData);
		if (nRet == SDK_ERR) {
			bisResversal = true;
		}
	}
	//存储冲正
	if (bisResversal) {
		nRet = SaveReversalBag(false, &stRevBag8583, &gstTradeData);
		return SDK_ERR;
	} else {
		nRet = WriteReversalFlag(false);
	}
	if (!nRet) {
		return SDK_ERR;
	}

	//        ProviderSetTagValue("FF9075", "0", 1);
	// 这里还要回写消费交易产生的一些特殊的信息tag
	nRet = WriteSaleMsg(&stSendBag8583, &stRevBag8583);
	if (nRet != SDK_OK) {
		return SDK_ERR;
	}
	nRet = Set8583Tvl(&stRevBag8583);
	if (nRet == SDK_EQU) {
		return SDK_ERR;
	}
	nRet = ProviderUpdateValueToDb();
	if (nRet != SDK_OK) {
		return SDK_ERR;
	}
	//nRet = printmag();
	return SDK_OK;

}

