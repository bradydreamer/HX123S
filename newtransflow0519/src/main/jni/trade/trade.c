#include "appglobal.h"
#define STOREBAGLEN         512                                 //存储报文长度
#define MANUALMINCARDNUM 13          //最小手输卡长度

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	u8 *asTag
 u8 *pbuf
 u16 len
 ** Returned value:
 ** Created By:		huacong  2016.03.11
 ** Remarks:
 *****************************************************************************/
void DebugPrint(u8 *asTag, u8 *pbuf, u16 len) {
	u16 i = 0;
	u8 temp[512];

	if (sdkIsVisibleStr(pbuf, len)) {
		Log(asTag, "[%s]", pbuf);

	} else {
		memset(temp, 0, sizeof(temp));
		sdkBcdToAsc(temp, pbuf, len);
		Log(asTag, "[%s]", temp);
	}
}

/*****************************************************************************
 函 数 名  : Trace8583Bag
 功能描述  : 显示8583报文域信息
 输入参数  : u8 *asTitle
 SDK_8583_ST8583 *pstBag8583
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月25日
 作    者   : zj
 修改内容   : 新生成函数

 *****************************************************************************/
void Trace8583Bag(u8 *asTitle, SDK_8583_ST8583 *pstBag8583) {
#ifndef XGD_SDK_DEBUG
	return;
#else
	u32 i;
	u8 asBuf[STOREBAGLEN];
	u8 asBuf1[STOREBAGLEN * 2];
	s32 siLen = 0;

	for (i = 0; i <= 64; i++)
	{
		if (sdk8583IsDomainExist(pstBag8583, i))
		{
			memset(asBuf, 0, sizeof(asBuf));
			siLen = 0;

			siLen = GetFieldData(pstBag8583, i, asBuf, sizeof(asBuf));
			if (siLen > 0)
			{
				if (sdkIsVisibleStr(asBuf, siLen))
				{
//                    Trace(asTitle, "[%03u][%03d][%s]\r\n", i, siLen, asBuf);
				}
				else
				{
					//可能是不是ASCII内容，转换为HEX显示,最后标注*号区分
					memset(asBuf1, 0, sizeof(asBuf1));
					sdkBcdToAsc(asBuf1, asBuf, siLen);
//                    Trace(asTitle, "[%03u][%03d][%s]*\r\n", i, siLen, asBuf1);
				}
			}

		}
	}
#endif
}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	u8 *pasCardModeCode
 u16 *uCardMode
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
void GetUserCardMode(u8 *pasCardModeCode, u8 *ucCardMode) {
	if (strstr(pasCardModeCode, TAG_CardMAG)) {
		(*ucCardMode) = SDK_ICC_MAG;
	} else if (strstr(pasCardModeCode, TAG_CardICC)) {
		(*ucCardMode) = SDK_ICC_ICC;
	} else if (strstr(pasCardModeCode, TAG_CardRF)) {
		(*ucCardMode) = SDK_ICC_RF;
	} else {
		(*ucCardMode) = 0;
	}

}

/*****************************************************************************
 函 数 名  : GetMacData
 功能描述  : 计算MAC数据
 输入参数  : u8 *pheDataBuf
 s32 *iDataLen
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月26日
 作    者   : zj
 修改内容   : 新生成函数

 *****************************************************************************/
s32 GetMacData(struct _TradeData *pstTradeData, SDK_8583_ST8583 *pstSendBag,
u8 *pucMacBuf) {
	s32 slt = SDK_OK;
	u8 *p = NULL;
	u16 nLen;
	int outlen = 0;

	p =
			&(pstSendBag->ucBagData[pstSendBag->stFiled[SDK_8583_FIELD_MSG].nFieldHead]);
	nLen = pstSendBag->stFiled[64].nFieldHead
			- pstSendBag->stFiled[SDK_8583_FIELD_MSG].nFieldHead;

//    Log("wangb","nLen %d",nLen);
	slt = ProviderCalcMac(p, nLen, pucMacBuf, &outlen);
//    Log("wangb","pucMacBuf %s",pucMacBuf);
	if (slt != SDK_OK) {
		return SDK_ESC;
	}

	//TraceHex("GetMacData", "mac=",pucMacBuf,8);

	return SDK_OK;
}

/*****************************************************************************
 函 数 名  : EncryptMagData
 功能描述  : 磁道数据加密
 输入参数  : u8 *pheDataBuf
 s32 *iDataLen
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月26日
 作    者   : wuzhonglin
 修改内容   : 新生成函数

 *****************************************************************************/
s32 EncryptMagData(struct _TradeData *pstTradeData, SDK_8583_ST8583 *pstSendBag) {
	u8 tmp[256], buf[128], outbuf[128] = { 0 };
	s32 iLen, pOutLen;
	s32 slt = SDK_OK;
	u8 *pBuf;
	bool bIsEncryp = false;
// && pstTradeData->stKeySystemParam.bIsMk3Des
//    iLen = GetFieldData(pstSendBag, 35, tmp, sizeof(tmp));
//    && pstTradeData->stKeySystemParam.bIsMk3Des
	if (gstAppSetting.stKeySystemParam.bIsSupportTdk
			&& (sdk8583IsDomainExist(pstSendBag, 35)
					|| sdk8583IsDomainExist(pstSendBag, 36))) {
		memset(tmp, 0, sizeof(tmp));
		iLen = GetFieldData(pstSendBag, 35, tmp, sizeof(tmp));
		Log("wangb", "Field35_1=%s", tmp);
		if (iLen > 16) {
//            sdkAscToBcd(buf, tmp, iLen);
//            pBuf = &buf[(iLen + 1) / 2 - 9];
			//加密

			//   slt = sdkPEDWkDes(pstTradeData->stKeySystemParam.uiTdkIndex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, pBuf, pBuf);
			slt = ProviderTrackDataEncrypt(tmp, outbuf, &pOutLen);   //Fixme
			if (slt != SDK_OK) {
				return SDK_ERR;
			}
//          sdkBcdToAsc(tmp, outbuf, (pOutLen + 1) / 2);
			sdkBcdToAsc(tmp, outbuf, pOutLen);
			Log("wangb", "Field35_2 %s", tmp);
			sdk8583ClrField(pstSendBag, 35);
			SetFieldData(pstSendBag, 35, tmp, iLen);

			bIsEncryp = true;
		}
		memset(tmp, 0, sizeof(tmp));
		memset(outbuf, 0, sizeof(outbuf));
		iLen = GetFieldData(pstSendBag, 36, tmp, sizeof(tmp));
		if (iLen > 16) {
//
//            sdkAscToBcd(buf, tmp, iLen);
//            pBuf = &buf[(iLen + 1) / 2 - 9];
			//加密
//            slt = sdkPEDWkDes(pstTradeData->stKeySystemParam.uiTdkIndex, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, pBuf, pBuf);
			slt = ProviderTrackDataEncrypt(tmp, outbuf, &pOutLen);
			if (slt != SDK_OK) {
				return SDK_ERR;
			}
//          sdkBcdToAsc(tmp, buf, (iLen + 1) / 2);
			sdkBcdToAsc(tmp, outbuf, pOutLen);
			sdk8583ClrField(pstSendBag, 36);
			SetFieldData(pstSendBag, 36, tmp, iLen);
			bIsEncryp = true;
		}
	}
	if (bIsEncryp) {
		//重新设置53域TRACK-ENCRYPTION-USED
		iLen = 0;
		memset(tmp, 0, sizeof(tmp));
		iLen = GetFieldData(pstSendBag, 53, tmp, sizeof(tmp));
		if (iLen >= 3) {
			tmp[2] = '1';
			sdk8583ClrField(pstSendBag, 53);
			SetFieldData(pstSendBag, 53, tmp, iLen);
		} else {
			strcpy(tmp, "001");
			if (gstAppSetting.stKeySystemParam.bIsMk3Des)        // 3des
			{
				tmp[1] = '6';
			}
			SetFieldData(pstSendBag, 53, tmp, strlen(tmp));
		}
	}
	Log("wangb", "pucMacBuf3 is %s", tmp);
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	 获得磁卡或手输卡号
 ** Parameters:          SDK_ICC_CARDDATA *pstCardData磁卡信息结构体
 u8 *lpOut卡号
 ** Returned value:	SDK_EQU:'D'后有'2','6'数据,是ICC卡,
 SDK_OK: 'D'后无'2','6'数据,非IC卡
 SDK_ERR:手输卡号长度太短
 ** Created By:		lilin2012.08.18
 ** Remarks:
 *****************************************************************************/
s32 GetMagCardNo(SDK_ICC_CARDDATA const *pstCardData, u8 *pasOut) {
	u8 temp[256] = { 0 };
	s32 rslt = SDK_OK;
	u16 i = 0;
	u8 len = 0;
	u8 track2len = 0;
	u8 track3len = 0;
	bool track2flag = false; //是否有二磁道数据或手输

	memset(temp, 0, sizeof(temp));
	track2len = pstCardData->ucTrack2Len;

	if (track2len)                                                       //2磁道数据
	{
		if (track2len > 37) {
			track2len = 37;
		}
		memcpy(temp, pstCardData->asTrack2Data, track2len);
		track2flag = true;  //有二磁道数据
	} else {
		track3len = pstCardData->ucTrack3Len;

		if (track3len)                                                   //三磁道数据
		{
			if (track3len > 104) {
				track3len = 104;
			}
			memcpy(temp, pstCardData->asTrack3Data, track3len);
		}
	}

	if (track2len || track3len) {
		len = strlen(temp);

		for (i = 0; i < len; i++) {
			if (temp[i] == 'D') {
				if (temp[i + 5] == '2' || temp[i + 5] == '6') //如果D后边的第5位是2或6表示该卡上有IC卡
						{
					rslt = SDK_EQU;
				}
				break;
			}
		}

		len = i;

		if (len > 19) {
			len = 19;
		}
		temp[len++] = 0;
	} else {
		memset(temp, 0, sizeof(temp));
		len = pstCardData->ucCardLen;

		if (len > 19) {
			len = 19;
		}

		if (len < MANUALMINCARDNUM)          //手输卡号小于13
		{
			rslt = SDK_ERR;
		}
		memcpy(temp, pstCardData->asCardNO, len);
		temp[len++] = 0;
		track2flag = true;
	}

	if (strlen(temp) == 0)  //未获取到卡号
			{
		return SDK_PARA_ERR;
	}
	memcpy(pasOut, temp, len);

	if (track2flag && strlen(temp) >= 13) //必须有二磁道数据或有手输数据
			{
		return rslt;
	} else {
		return SDK_PARA_ERR;
	}
}

/*****************************************************************************
 ** Descriptions:	计算MAC类型
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		huacong  2016.03.03
 ** Remarks:
 *****************************************************************************/
s32 GetMacType(u32 *uiMacType) {
	u8 tmp[128] = { 0 };
	s32 mactype = 0;
	s32 rlen;
	bool falg = true;

	memset(tmp, 0, sizeof(tmp));
	if (ProviderGetTagValue(TAG_TRADE_MACTYPE, tmp)) {
		Log("wangb", "TAG_TRADE_MACTYPE= %s", tmp);
		rlen = atoi(&tmp[0]);
		Log("zbq", "rlen = %d", rlen);
		switch (rlen) {
		case 1:
			mactype = SDK_PED_ECB;
			break;

		case 2:
			mactype = SDK_PED_X919;
			break;

		case 3:
			mactype = SDK_PED_ECB;     //1锟斤拷3 锟截革拷锟剿帮拷//huacong 2016.03.04 9:46
			break;

		default:
			falg = false;
			break;
		}
	} else {
		falg = false;
	}

	if (!falg) {
		return SDK_ERR;
	}
	(*uiMacType) = mactype;

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	更新工作秘钥
 ** Parameters:    	u8 ucWorkKeyType
 ** Returned value:
 ** Created By:		huacong  2016.03.03
 ** Remarks:
 *****************************************************************************/
s32 GetWorkKeyIndexByTmk(WORK_KEY_TYPE ucWorkKeyType, u32 uiTmkIndex) {
	u8 tmp[128], tmkindex = 0, wkindex = 0;
	bool falg = true;

	memset(tmp, 0, sizeof(tmp));
	//if(ProviderGetTagValue(TAG_TRADE_TMKINDEX, tmp))
	{
		tmkindex = uiTmkIndex;
		switch (ucWorkKeyType) {
		case 0:
			wkindex = tmkindex + 1;
			break;

		case 1:
			wkindex = tmkindex + 2;
			break;

		case 2:
			wkindex = tmkindex + 3;
			break;

		default:
			falg = false;
			break;
		}
	}

	if (!falg) {
		return SDK_ERR;
	}
	return wkindex;
}

/*****************************************************************************
 ** Descriptions:	锟斤拷取锟斤拷锟斤拷钥锟斤拷锟斤拷
 ** Parameters:    	u32 uiTmkType
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 GetTmkDesType(u32 *uiTmkType) {
	u8 temp[512] = { 0 };

	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_TRADE_DESTYPE, temp)) {
		if (temp[0] == 0) {
			(*uiTmkType) = 0;
		} else if (temp[0] == 6) {
			(*uiTmkType) = 1;
		} else      //huacong 2016.03.04 9:40 锟斤拷锟斤拷锟斤拷锟揭凤拷锟?
		{
			(*uiTmkType) = 1;
		}
	} else {
		return SDK_ERR;
	}

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	整数转字符串
 ** Returned value:
 ** Created By:		wangb  2016.03.22
 ** Remarks:
 *****************************************************************************/
void itoa(u32 num, u8 *str) {
	u32 sign = num;
	u32 i = 0;
	u32 j = 0;
	u8 temp[100];
	//如果是负数就去掉符号,将-1234转成1234
	if (sign < 0) {
		num = -num;
	}

	//转成字符串，1234转成"4321"
	do {
		temp[i] = num % 10 + '0';
		num /= 10;
		i++;
	} while (num > 0);
	//如果是负数的话，加个符号在末尾，如："4321-"
	if (sign < 0) {
		temp[i++] = '-';
	}
	temp[i] = '\0';
	//  i--;
	//将temp数组中逆序输入到str数组中
	//将"4321-" ====> "-1234"
	while (i > 0) {
		i--;
		str[j] = temp[i];
		j++;

	}
	//字符串结束标识
	str[j] = '\0';
}

/*****************************************************************************
 ** Descriptions:	获取系统参数，如果没有获取到直接跑错
 ** Parameters:    	struct _TradeData *pstTradeData
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 InitSysParam()     //Fix me 参数判断
{
	u8 temp[512] = { 0 };
	u8 src[12] = { 0 };
	s32 ret = SDK_ERR, len;

	//TMS 参数初始化 fixme (暂时写死，以后看交易是否跑通)
	memcpy(gstAppSetting.stTmsParam.TermParamVer, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.TMSParamVer, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.CapkParamVer, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.AidParamVer, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.TermParamVerBak, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.TMSParamVerBak, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.CapkParamVerBak, "00000000", 8);
	memcpy(gstAppSetting.stTmsParam.AidParamVerBak, "00000000", 8);

	//脚本标志
	if (ReadReverScriptFlag(&gstTradeData.bisHaveReverScriptBag) != SDK_OK) {
		return SDK_ERR;
	}
	//冲正标志
	if (ReadReversalFlag(&gstTradeData.bisHaveReversalBag) != SDK_OK) {
		return SDK_ERR;
	}
	Log("wangb", "pstTradeData->bisHaveReversalBag is %d ",
			gstTradeData.bisHaveReversalBag);
	//这个地方先顺序执行，后续看是否可以优化为建立一个常用表，依次自动配置
	//流水号
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_VoucherNo, temp)) > 0) {
		u32 numb = atol(temp) + 1;
		memset(temp, 0, sizeof(temp));
		itoa(numb, temp);
		sprintf(src, "%06s", temp);
		memcpy(gstSys.asVoucherNo, src, len);
	} else {
		return SYS_VOUNO;
	}

	//DES
	gstAppSetting.stKeySystemParam.bIsMk3Des = 1;
	Log("zbq", "bIsMk3Des = %d", gstAppSetting.stKeySystemParam.bIsMk3Des);

	//批次号
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_AsBatchNo, temp)) > 0) //Fix me len 更改
			{
		Log("wangb", "TAG_8583_Field6002Type is %s", temp);
		memcpy(gstSys.asBatchNo, temp, len);
	} else {
		return SYS_FILEd6002Type;
	}

	//参考号
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_SysRefNo, temp) > 0)   // Fix me
			{
		memcpy(gstTradeData.asSysRefNo, temp, 12);
	} else {
		return SDK_ERR;
	}

	//操作员代码
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_UsManNo, temp))   // Fix me
			{
		gstTradeData.usManNo = atol(temp);
		Log("zbq", "usManNo = %d", gstTradeData.usManNo);
	} else {
		return SDK_ERR;
	}
	//商户号
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_MerchantNo, temp)) > 0)  //Fix me
			{
		Log("wangb", "TAG_8583_MerchantNo is %s", temp);
		memcpy(gstAppSetting.stTerminalParam.asMerchantNo, temp, len);
	} else {
		return SYS_MerchantNo;
	}

	//终端号
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_TerminalNo, temp)) > 0) {
		Log("wangb", "TAG_8583_TerminalNo is %s", temp);
		memcpy(gstAppSetting.stTerminalParam.asTerminalNo, temp, len);
	} else {
		return SYS_TerminalNo;
	}

	//TPDU
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_TPDU, temp)) {
		Log("wangb", "TAG_TPDU is %s", temp);
		memcpy(gstTradeData.asTpdu, temp, 11);
	} else {
		return SYS_TPDU;
	}

	//货币代码
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_CurrencyCode, temp)) > 0) {
		Log("wangb", "TAG_8583_CurrencyCode is %s", temp);
		//  memcpy(pstTradeData->stTerminalParam.asCurrencyCode, temp, len);
		memcpy(gstAppSetting.stTerminalParam.asCurrencyCode, "156", 3);
	} else {
		return SYS_CurrencyCode;
	}

	//主密钥索引
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_TRADE_TMKINDEX, temp)) {
		Log("wangb", "TAG_TRADE_TMKINDEX is %s", temp);
		gstAppSetting.stKeySystemParam.uiTMkIndex = temp[0];
	} else {
		return SYS_TMKINDEX;
	}

	//  主密钥类型
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_TRADE_DESTYPE, temp))   //Fix me
			{
		Log("wangb", "TAG_TRADE_DESTYPE is %s", temp);
		if (strcmp(temp, TAG_NUBSIX) == 0) {
			gstAppSetting.stKeySystemParam.bIsMk3Des = true;
		} else if (strcmp(temp, TAG_NUBZER) == 0) {
			gstAppSetting.stKeySystemParam.bIsMk3Des = false;
		}

	} else {
		return SYS_KeySystemParam;
	}

	//是否支持磁道加密
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_TRADE_SUPPORTTDK, temp))  //Fix me
			{
		Log("wangb", "TAG_TRADE_SUPPORTTDK is %s ", temp);
		if (strcmp(temp, TAG_NUBONE) == 0)   //Fix me
				{
			gstAppSetting.stKeySystemParam.bIsSupportTdk = true;
		} else if (strcmp(temp, TAG_NUBZER) == 0) {
			gstAppSetting.stKeySystemParam.bIsSupportTdk = false;
		}

	} else {
		return SYS_SUPPORTTDK;
	}

	//mac加密方式
	if ((ret = GetMacType(&gstAppSetting.stKeySystemParam.uiMacType)) != SDK_OK) {
		Log("zbq", "GetMacType Error,code = %d", ret);
		return SYS_MacType;
	}

	Log("wangb", "Out InitSysParam");

	//TPK索引暂保留
	/*    ret = GetWorkKeyIndexByTmk(WORK_KEY_TPK, pstTradeData->stKeySystemParam.uiTMkIndex);
	 if(ret != SDK_ERR)
	 {
	 pstTradeData->stKeySystemParam.uiTpkIndex = ret;
	 }
	 else
	 {
	 return SYS_KEY_TPK;
	 }

	 //TaK索引 暂保留
	 ret = GetWorkKeyIndexByTmk(WORK_KEY_TAK, pstTradeData->stKeySystemParam.uiTMkIndex);
	 if(ret != SDK_ERR)
	 {
	 pstTradeData->stKeySystemParam.uiTakIndex = ret;
	 }
	 else
	 {
	 return SYSKEY_TAK;
	 }

	 //TdK索引 暂保留
	 if(pstTradeData->stKeySystemParam.bIsMk3Des &&  pstTradeData->stKeySystemParam.bIsSupportTdk)
	 {
	 ret = GetWorkKeyIndexByTmk(WORK_KEY_TDK, pstTradeData->stKeySystemParam.uiTMkIndex);
	 if(ret != SDK_ERR)
	 {
	 pstTradeData->stKeySystemParam.uiTakIndex = ret;
	 }
	 else
	 {
	 return SYS_KEY_TDK;
	 }
	 } */

	return SDK_OK;

}

s32 InitIccTradeParam(SDK_ICC_TRADE_PARAM *pstIccTradeParam) {
	u8 temp[512] = { 0 };
	s32 ret = SDK_ERR, len;

	// 是否强制联机//whether force online
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsForceOnline = temp[0];
	} else {
		return SYS_VOUNO;
	}

	// 是否支持fallback  //whether support fallback
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsFallback = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}
	//是否支持QPBOC  //whether support QPBOC
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsSupportQPBOC = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}

	//是否支持非接PBOC  //whether support contactless PBOC
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsSupportCLPBOC = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}

	// 0:不支持电子现金; 1:支持电子现金//0:not support e-cash, 1:support e-cash
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsSupportEc = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}

	// 是否提示选择电子现金//whether prompt to select e-cash
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsPromptChooseEc = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}

	// 不提示选择电子现金或是选择电子现金超时,使用缺省值1:使用电子现金;0:不使用电子现金
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->bIsDefaultEc = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}
	//01完整流程;02简化流程;03查余额;04圈存;05读日志//01 intact flow; 02 simplified flow; 03 query balancd; 04:load; 05:read log
	memset(temp, 0, sizeof(temp));
	if (ProviderGetTagValue(TAG_8583_VoucherNo, temp)) {
		pstIccTradeParam->eFlowMode = temp[0];
	} else {
		return SYS_FILEd6002Type;
	}

	// 是否选择固定的AID  //whether to choose fixed AID
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_8583_VoucherNo, temp)) > 0) {
		memcpy(pstIccTradeParam->stFixedAid.heAid, temp, len);
	} else {
		return SYS_FILEd6002Type;
	}

	return SDK_OK;

}

/*****************************************************************************
 ** Descriptions:	加密发送包
 ** Parameters:    	SDK_8583_ST8583 *pstSendBag 发送包
 ** Returned value:
 ** Created By:		lilin2012.12.22
 ** Remarks:
 *****************************************************************************/
s32 EncryptSendData(struct _TradeData *pstTradeData,
		SDK_8583_ST8583 *pstSendBag) {
	s32 slt = SDK_OK;
	u8 ucMacBuf[32] = { 0 };
//lilin 20140319
//    //重新改写报文头
//    ReConfigBag8583(pstSendBag);
	//磁道信息加密
	if (pstSendBag == NULL || pstTradeData == NULL) {
		return 0;
	}
	slt = EncryptMagData(pstTradeData, pstSendBag);
	Log("wangb", "EncryptMagData is %d ", slt);
	if (slt != SDK_OK) {
		return SDK_ESC;
	}

	//获得MAC
	if (sdk8583IsDomainExist(pstSendBag, 64)) {
		slt = GetMacData(pstTradeData, pstSendBag, ucMacBuf);
		if (slt != SDK_OK) {
			return SDK_ESC;
		}
		slt = SetFieldData(pstSendBag, 64, ucMacBuf, 8);
		//   slt = SetFieldData(pstSendBag, 64, "\x02\x00\x30\x20\x04\xC0\x20\xC0", 8);
		//    slt = SetFieldData(pstSendBag, 64, "\x32\x42\x35\x39\x36\x33\x39\x37", 8);
		if (slt <= 0) {
			return SDK_ESC;
		}
	}
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	存储内外卡标志
 ** Parameters:    	SDK_8583_ST8583 *pstBag8583
 ** Returned value:
 ** Created By:		huacong  2016.03.17
 ** Remarks:
 *****************************************************************************/
s32 WriteCardInOrOut(SDK_8583_ST8583 *pstSendBag, SDK_8583_ST8583 *pstRevBag) {
	u8 temp[512];
	u8 bIsForeignCard[2] = { 0 };
	memset(temp, 0, sizeof(temp));
	if (GetFieldData(pstRevBag, 63, temp, sizeof(temp)) > 0) {
		if (((!memcmp(temp, "CUP", 3))
				|| //中国银联
				(!memcmp(temp, "cup", 3)) || (!memcmp(temp, "000", 3))
				|| (!memcmp(temp, "\x00\x00\x00", 3)))) {
			bIsForeignCard[0] = '0';
		} else {
			bIsForeignCard[0] = '1';
		}
	} else if (GetFieldData(pstSendBag, 63, temp, sizeof(temp)) > 0) {
		if (((!memcmp(temp, "CUP", 3))
				|| //中国银联
				(!memcmp(temp, "cup", 3)) || (!memcmp(temp, "000", 3))
				|| (!memcmp(temp, "\x00\x00\x00", 3)))) {
			bIsForeignCard[0] = '0';
		} else {
			bIsForeignCard[0] = '1';
		}
	}
//    Log("wangb","WriteCardInOrOut is %s ",bIsForeignCard);
	if (ProviderSetTagValue(TAG_TRADE_CARDINOUT, bIsForeignCard, 1) != 1) {
		return SDK_ERR;
	}
//    Log("wangb","WriteCardInOrOut is %s ",bIsForeignCard);
	return SDK_OK;

}

/*****************************************************************************
 ** Descriptions:	存储借贷机标志
 ** Parameters:    	u8 ucTradeid
 ** Returned value:
 ** Created By:		huacong  2016.03.17
 ** Remarks: 		通过交易类型
 *****************************************************************************/
s32 WriteDebitOrCreditType(u8 ucTradeid) {
	//  u8 bisCredit = false;      //是否是借记
	u8 bisCredit[2] = { 0 };      //是否是借记
	switch (ucTradeid) {
	case TRADEID_SALE:
		bisCredit[0] = '0';
		break;

	case TRADEID_UNSALE:
		bisCredit[0] = '1';
		break;

	default:

		break;
	}
	if (ProviderSetTagValue(TAG_TRADE_DEBITCREDIT, bisCredit, 1) != 1) {
		return SDK_ERR;
	}
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	回写脚本发送次数
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteScriptTimes(const u32 uiTradeTimes) {
	u8 temp[8] = { 0 };
	itoa(uiTradeTimes, temp);
	if (ProviderSetTagValue(TAG_TRADE_TIME, temp, 1) != 1) {
		return SDK_ERR;
	}

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	回写冲正发送次数
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteTradeTimes(const u32 uiTradeTimes) {
	u8 temp[8] = { 0 };
	itoa(uiTradeTimes, temp);
	Log("wangb", "WriteTradeTimes is %s", temp);
	if (ProviderSetTagValue(TAG_TRADE_TIME, temp, 1) != 1) {
		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	回写脚本发送包
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteSendScript(SDK_8583_ST8583 *pstBag8583) {
	u8 outdata[2048] = { 0 };
	sdkU16ToBcd(outdata, pstBag8583->nBagLen, 2);
	memcpy(&outdata[0] + 2, pstBag8583->ucBagData, pstBag8583->nBagLen);
//    ProviderSetTagValue(TAG_8583_SENDBAG,outdata,pstBag8583->nBagLen+2);
	if (ProviderSetTagValue(TAG_8583_SENDBAG, outdata, pstBag8583->nBagLen + 2)
			!= 1) {
		return SDK_ERR;
	}

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	回写发送包
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteSendBag(char *strTag, SDK_8583_ST8583 *pstBag8583) {
	u8 outdata[2048] = { 0 };
	u8 aucTempData[8] = { 0 };
	if (pstBag8583 == NULL) {
		return false;
	}

	Log("zbq", "pstBag8583->nBagLen = %d", pstBag8583->nBagLen);

//    sdkU16ToBcd(&outdata, pstBag8583->nBagLen, 2);
	sdkU16ToHex(outdata, pstBag8583->nBagLen, 2);
	memcpy(&outdata[0] + 2, pstBag8583->ucBagData, pstBag8583->nBagLen);

	memset(aucTempData, 0, 8);
	sdkBcdToAsc(aucTempData, outdata, 2);
	Log("zbq", "BcdToAsc len = %s", aucTempData);

//    ProviderSetTagValue(TAG_8583_SENDBAG,outdata,pstBag8583->nBagLen+2);
	if (ProviderSetTagValue(strTag, outdata, pstBag8583->nBagLen + 2) != 1) {
		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	网络通信状态
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		huacong  2016.04.06
 ** Remarks:
 *****************************************************************************/
s32 GetCurCommuStatue(u8 ucCommuStatue) {
	s32 ret = SDK_ERR;

	switch (ucCommuStatue) {
	case 0x00:
		ret = SDK_OK;
		break;

	case 0x01:
		ret = SDK_TIME_OUT;
		break;

	case 0x02:
		ret = SDK_COMM_WAVELOST;
		break;

	case 0x03:
		ret = SDK_COMM_LINKFAIL;
		break;

	default:

		break;

	}

	return ret;
}

/*****************************************************************************
 ** Descriptions:	回写接收包
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteRevBag(SDK_8583_ST8583 *pstBag8583) {
	u8 outdata[2048] = { 0 };
	if (pstBag8583 == NULL) {
		return false;
	}
	sdkU16ToBcd(outdata, pstBag8583->nBagLen, 2);
	memcpy(outdata + 2, pstBag8583->ucBagData, pstBag8583->nBagLen);

	if (ProviderSetTagValue(TAG_8583_RECVBAG, outdata, pstBag8583->nBagLen + 2)
			!= 1) {
		return SDK_ERR;
	}
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	读取发送包
 ** Parameters:    	SDK_8583_ST8583 *pstSendBag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 修改  wangb 2016 0315
 ** Remarks:
 *****************************************************************************/
s32 ReadSendBag(SDK_8583_ST8583 *pstSendBag) {
	u8 temp[2048];
	s32 len;
	memset(temp, 0, sizeof(temp));

	if (pstSendBag == NULL) {
		return SDK_ERR;
	}
	if ((len = ProviderGetTagValue(TAG_8583_SENDBAG, temp)) > 2) {
		memcpy(pstSendBag->ucBagData, temp + 2, len - 2);
		pstSendBag->nBagLen = len - 2;
		if (ParseField8583(pstSendBag) < SDK_EQU) {
			return SDK_ERR;
		} else {
			//xiaox 有55域则写入 emvtag 缓存链表
			if (sdk8583IsDomainExist(pstSendBag, 55)) {
				if (gstEmvTempTagList.ItemCount > 0) {
					FreeTagList(&gstEmvTempTagList);
				}
				memset(temp, 0, sizeof(temp));
				InitTagList(&gstEmvTempTagList);
				len = GetFieldData(pstSendBag, 55, temp, sizeof(temp));
				BuildTagListOneLevel(temp, len, &gstEmvTempTagList);
				TraceHex("xiaox","55field",temp,len);
			}
		}
	} else {
		return SDK_ERR;
	}
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	读取接收包
 ** Parameters:    	SDK_8583_ST8583 *pstSendBag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 修改  wangb 2016 0315
 ** Remarks:
 *****************************************************************************/
s32 ReadRevBag(SDK_8583_ST8583 *pstRevBag) {
	u8 temp[2048];
	u16 len;
	s32 nRet = 0;
	memset(temp, 0, sizeof(temp));
	if (pstRevBag == NULL) {
		return SDK_ERR;
	}
#ifdef EncriptBagControl
	if ((len = ProviderGetTagValue(TAG_8583_RECVECRIPTBAG, temp)) > 2) {
		//全报文解密
		len = len - 2; //减去报文全长2字节
		EncryptAllMsgByHX(0, temp + 2, &len);

		memcpy(pstRevBag->ucBagData, temp + 2, len);
		pstRevBag->nBagLen = len;
		if (ParseField8583(pstRevBag) < SDK_EQU) {
			return SDK_ERR;
		}

		//保存解密的接收报文
		nRet = WriteSendBag(TAG_8583_RECVBAG, pstRevBag);
		if (nRet < SDK_OK) {
			return nRet;
		}
	} else {
		return SDK_ERR;
	}
#else
	if((len = ProviderGetTagValue(TAG_8583_RECVBAG, temp)) > 2)
	{
		memcpy(pstRevBag->ucBagData, temp+2, len-2);
		pstRevBag->nBagLen = len-2;
		if(ParseField8583(pstRevBag) < SDK_EQU)
		{
			return SDK_ERR;
		}
	}
	else
	{
		return SDK_ERR;
	}
#endif

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	回写脚本包
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		wangb  2016.03.21
 ** Remarks:
 *****************************************************************************/
s32 WriteScriptBag(SDK_8583_ST8583 *pstBag8583) {
	if (ProviderSetTagValue(TAG_SCIRTP_SENDFLAG, pstBag8583->ucBagData,
			pstBag8583->nBagLen) != 0) {
		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	回写冲正包
 ** Parameters:    	SDK_8583_ST8583 *pst8583Bag
 ** Returned value:
 ** Created By:		huacong  2016.03.04
 ** Remarks:
 *****************************************************************************/
s32 WriteReversalBag(SDK_8583_ST8583 *pstBag8583) {
	if (ProviderSetTagValue(TAG_TRADE_REVERSALBAG, pstBag8583->ucBagData,
			pstBag8583->nBagLen) != 0) {
		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	读取脚本
 ** Parameters:    	SDK_8583_ST8583 *pstReversalBag
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 ReadReverScriptBag(SDK_8583_ST8583 *pstReversalBag) {
	s32 ret = SDK_ESC, len;
	u8 temp[SDK_8583_BAGMAXLEN];

	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_SCIRTP_SENDFLAG, temp)) > 0) {
		memcpy(pstReversalBag->ucBagData, temp, len);
		pstReversalBag->nBagLen = len;
		if (ParseField8583(pstReversalBag) < SDK_EQU) {
			return SDK_ERR;
		} else {
			//xiaox 有55域则写入 emvtag 缓存链表
			if (sdk8583IsDomainExist(pstReversalBag, 55)) {
				if (gstEmvTempTagList.ItemCount > 0) {
					FreeTagList(&gstEmvTempTagList);
				}
				memset(temp, 0, sizeof(temp));
				InitTagList(&gstEmvTempTagList);
				len = GetFieldData(pstReversalBag, 55, temp, sizeof(temp));
				BuildTagListOneLevel(temp, len, &gstEmvTempTagList);
				TraceHex("xiaox","55field",temp,len);
			}
		}
	} else {
		return SDK_ERR;
	}

}

/*****************************************************************************
 ** Descriptions:	读取冲正包
 ** Parameters:    	SDK_8583_ST8583 *pstReversalBag
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 ReadReversalBag(SDK_8583_ST8583 *pstReversalBag) {
	s32 ret = SDK_ESC, len;
	u8 temp[SDK_8583_BAGMAXLEN];
	Log("zbq", "jump into ReadReversalBag");
	memset(temp, 0, sizeof(temp));
	if ((len = ProviderGetTagValue(TAG_TRADE_REVERSALBAG, temp)) > 0) {
		memcpy(pstReversalBag->ucBagData, temp, len);
		pstReversalBag->nBagLen = len;
		if (ParseField8583(pstReversalBag) < SDK_EQU) {
			return SDK_ERR;
		} else {
			//xiaox 有55域则写入 emvtag 缓存链表
			if (sdk8583IsDomainExist(pstReversalBag, 55)) {
				if (gstEmvTempTagList.ItemCount > 0) {
					FreeTagList(&gstEmvTempTagList);
				}
				memset(temp, 0, sizeof(temp));
				InitTagList(&gstEmvTempTagList);
				len = GetFieldData(pstReversalBag, 55, temp, sizeof(temp));
				BuildTagListOneLevel(temp, len, &gstEmvTempTagList);
				TraceHex("xiaox","55field",temp,len);
			}
		}
	} else {
		return SDK_ERR;
	}

}
/*****************************************************************************
 ** Descriptions:	写脚本标志
 ** Parameters:    	bisReversal
 ** Returned value:
 ** Created By:		wangb  2016.03.20
 ** Remarks:
 *****************************************************************************/
s32 WriteScriptlFlag(bool bisReversal) {
	u8 astemp[2 + 1];

	memset(astemp, 0, sizeof(astemp));
	if (bisReversal) {
		strcpy(astemp, "1");
	} else {
		strcpy(astemp, "0");
	}

	if (ProviderSetTagValue(TAG_SCIRTP_SCRIPT, astemp, 1) != 0) {
		return SDK_ERR;
	}
}

/*****************************************************************************
 ** Descriptions:	?????????
 ** Parameters:    	SDK_8583_ST8583 *pstReversalBag
 ** Returned value:
 ** Created By:		wangb  2016.03.21
 ** Remarks:
 *****************************************************************************/
s32 ReadReverScriptFlag(bool *bisReversal) {
	u8 astemp[2 + 1];

	memset(astemp, 0, sizeof(astemp));
	if (ProviderGetTagValue(TAG_SCIRTP_SCRIPT, astemp)) {
		if (memcmp(astemp, "1", 2) == 0) {
			(*bisReversal) = true;
		} else {
			(*bisReversal) = false;
		}
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	?????????
 ** Parameters:    	bool bisReversal
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 ReadReversalFlag(bool *bisReversal) {
	u8 astemp[2 + 1];

	memset(astemp, 0, sizeof(astemp));
	if (ProviderGetTagValue(TAG_TRADE_REVERSAL, astemp)) {
		if (memcmp(astemp, "1", 2) == 0) {
			(*bisReversal) = true;
		} else {
			(*bisReversal) = false;
		}
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	写冲正包标志
 ** Parameters:    	SDK_8583_ST8583 *pstReversalBag
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 WriteReversalFlag(bool bisReversal) {
	u8 astemp[2 + 1];

	memset(astemp, 0, sizeof(astemp));
	if (bisReversal) {
		strcpy(astemp, "1");
	} else {
		strcpy(astemp, "0");
	}

	if (ProviderSetTagValue(TAG_TRADE_REVERSAL, astemp, 1) != 1) {
		return SDK_ERR;
	}
}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	Input: None
 ** Returned value:
 ** Created By:		huacong  2016.03.11
 ** Remarks: 		39域处理
 *****************************************************************************/
void SetResversal39Field(s32 CheckBagStatue, u8 *pasResCode) {
	u8 asResCode[2 + 1];

	memset(asResCode, 0, sizeof(asResCode));
	switch (CheckBagStatue) {
	case ERR_CALCMAC:       //????mac????
	case ERR_MISSMAC:       //???MAC
		strcpy(asResCode, "A0");
		break;

	case SDK_TIME_OUT:      //???
		strcpy(asResCode, "98");
		break;

	default:
		strcpy(asResCode, "06");
		break;
	}

	memcpy(pasResCode, asResCode, 2);

}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	s32 CheckBagStatue
 ** Returned value:
 ** Created By:		huacong  2016.03.11
 ** Remarks:
 *****************************************************************************/
bool JudgeIsNeedReversal(s32 CheckBagStatue) {
	bool flag = true;

	switch (CheckBagStatue) {
	case ERR_RESPONSE:       //????mac????
	case ERR_RESPONSE10:       //???MAC
	case SDK_OK:
		flag = false;
		break;
	default:
		break;
	}

	return flag;
}

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	SDK_8583_ST8583 *pstReversalBag
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 WriteUndoFlag(bool bisUndo) {
	u8 astemp[2 + 1];
	if (bisUndo) {
		strcpy(astemp, "1");
	} else {
		strcpy(astemp, "0");
	}
	if (ProviderSetTagValue(TAG_TRADE_UNDOFLAG, astemp, 1) != 1) {
		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	效验接收包
 ** Parameters:          void
 ** Returned value:
 ** Created By:		lilin2012.08.06
 ** Remarks:
 ***************************************************+**************************/
s32 CheckRecBag(SDK_8583_ST8583 *pstSendBag, SDK_8583_ST8583 *pstRevBag) {
	//SDK_PED_DES_TYPE type;
	u8 ucMacBuf[32];
	u8 sendbuf[64] = { 0 };
	u8 proccode[6] = { 0 };
	s32 slen;
	s32 rlen;
	u8 revbuf[64] = { 0 };
	u8 res[3] = { 0 };
	s32 rlt;
	u8 i;
	u8 num = 0;
	struct _Field60 stfield60;

	struct _TradeData *stTradeData = &gstTradeData;

#ifdef ELECSIGN
	u8 maxCheckNum = 6;
#endif
	u8 he_checkdata[6] = {
	SDK_8583_FIELD_MSG,           //消息类型
			3,                              //交易处理码
			11,                             //流水号
			41,                             //终端号
			42,                             //商户号
			4,                              //交易金额
			};

	//获取处理吗
	Log("wangb", "CheckRecBag start");
	if (sdk8583IsDomainExist(pstSendBag, 3)) {
		GetFieldData(pstSendBag, 3, proccode, 6);
	}
	memset(sendbuf, 0, sizeof(sendbuf));
	rlt = GetFieldData(pstSendBag, SDK_8583_FIELD_MSG, sendbuf, 4);   //获取消息类型
	if (rlt < 0) {
		Log("CheckRecBag", "取RECV0错误");
		return ERR_RECZERO;
	} else {
		if (memcmp(sendbuf, "05", 2))                        //结算报文没有39域
				{
			memset(res, 0, sizeof(res));
			rlt = GetFieldData(pstRevBag, 39, res, 3);
			if (rlt < 0) {
				Log("CheckRecBag", "取RECV39错误\r\n");
				return ERR_RECZERO;
			} else {
				ProviderSetTagValue(TAG_8583_BKRESULT, res, rlt);
			}
		}
	}
	//是否缺失64域
	if (sdk8583IsDomainExist(pstSendBag, 64))          //先判断发送包是否有64
			{
		if (strcmp(res, "00") == 0)       //39域返回正确
				{
			if (!sdk8583IsDomainExist(pstRevBag, 64))         //判断接收64域
					{
				rlt = ERR_MISSMAC;
				return rlt;
			}
		}
	}

	//判断64域是否正确
	if (sdk8583IsDomainExist(pstRevBag, 64)) {
		rlt = GetMacData(stTradeData, pstRevBag, ucMacBuf);
		Log("wangb", "ucMacBuf64 %s", ucMacBuf);
		if (rlt != SDK_OK) {
			Log("CheckRecBag", "ECB算法错误");
			return ERR_CALCMAC;
		}
		memset(revbuf, 0, sizeof(revbuf));
		rlt = GetFieldData(pstRevBag, 64, revbuf, 8);
		Log("wangb", "revbuf64 %s", revbuf);
		if (rlt < 0) {
			Log("CheckRecBag", "缺少64域");
			return ERR_RECZERO;
		}
		// TraceHex("CheckRecBag", "cal=", ucMacBuf, 8);
		// TraceHex("CheckRecBag", "mac=", revbuf, 8);

		if (memcmp(ucMacBuf, revbuf, 8)) {
			Log("CheckRecBag", "MAC效验错误");
			rlt = ERR_CALCMAC;
			return rlt;
		}
	}

	u8 gElecSignIsSendingEsTrade = false; //本次交易为电子签名上送交易(用于解包8583不校验4域--打包8583)
#ifdef ELECSIGN
	if (gElecSignIsSendingEsTrade) {
		maxCheckNum = 5;
	} else {
		maxCheckNum = 6;
	}
	for (i = 0; i < maxCheckNum; i++)
#else
			for (i = 0; i < 6; i++)
#endif
			{
		num = he_checkdata[i];

		if (sdk8583IsDomainExist(pstRevBag, num)
				&& sdk8583IsDomainExist(pstSendBag, num)) {
			memset(revbuf, 0, sizeof(revbuf));
			rlen = GetFieldData(pstRevBag, num, revbuf, 64);

			if (rlen < 0) {
				Log("CheckRecBag", "取RECV%d错误\r\n", num);
				return ERR_RECZERO;
			}
			memset(sendbuf, 0, sizeof(sendbuf));
			slen = GetFieldData(pstSendBag, num, sendbuf, 64);

			if (slen < 0) {
				Log("CheckRecBag", "取SEND%d错误\r\n", num);
				return ERR_RECZERO;
			}
			Log("wangb", "sendbuf is %s", sendbuf);
			Log("wangb", "revbuf is %s", revbuf);
			switch (num) {
			case SDK_8583_FIELD_MSG:   //判断消息类型

				if ((memcmp(sendbuf, revbuf, 2))
						|| (revbuf[2] != (sendbuf[2] + 1))) {
					rlt = ERR_MSGTYPE;
					return rlt;
				}
				break;

			case 3:      //比较处理码(银行卡检测中心说一定要比较第1个字节交易类型)

				if (memcmp(sendbuf, revbuf, 2)) {
					rlt = ERR_PROCESSCODE;
					return rlt;
				}
				break;

			case 11:

				//比较POS流水号
				if (strcmp(sendbuf, revbuf)) {
					rlt = ERR_VOUCHERNO;
					return rlt;
				}
				break;

			case 41:

				//比较终端号
				if (strcmp(sendbuf, revbuf)) {
					rlt = ERR_TERMINALNO;
					return rlt;
				}
				break;

			case 42:

				//比较商户号
				if (strcmp(sendbuf, revbuf)) {
					rlt = ERR_MERCHANTNO;
					return rlt;
				}
				break;

			case 4:

				//比较交易金额
				if (strcmp(sendbuf, revbuf)) {
					Log("CheckRecBag", "%s  %s\r\n", sendbuf, revbuf);
					memset(&stfield60, 0, sizeof(stfield60));

					if (ParseField60(&stfield60)) {
						if (strcmp(res, "10") || stfield60.ucPortion != '1') //不为部分扣款
								{
							rlt = ERR_TRADEAMOUNT;
							return rlt;
						}
					}
				}
				break;

			default:
				break;
			}
		} else {
			if (num == 41) {
				rlt = ERR_TERMINALNO;
				return rlt;
			}

			if (num == 42) {
				rlt = ERR_MERCHANTNO;
				return rlt;
			}
		}
	}

	//响应码处理
	memset(sendbuf, 0, sizeof(sendbuf));
	rlt = GetFieldData(pstSendBag, SDK_8583_FIELD_MSG, sendbuf, 4); //获取消息类型码

	if (rlt < 0) {
		Log("CheckRecBag", "取RECV0错误");
		return ERR_RECZERO;
	}

	if (memcmp(sendbuf, "04", 2) == 0)                    //冲正包判断
			{
		if (memcmp(res, "00", 2) && memcmp(res, "12", 2)
				&& memcmp(res, "25", 2)) {
			return ERR_RESPONSE;
		}
	}
	//磁条卡现金充值确认包判断
	else if (memcmp(sendbuf, "0220", 4) == 0
			&& memcmp(proccode, "63", 2) == 0) {
		if (memcmp(res, "00", 2) && memcmp(res, "A6", 2)) {
			return ERR_RESPONSE;
		}
	} else {
		if (memcmp(sendbuf, "05", 2))                        //结算报文没有39域
				{
			if (!CheckResponseCode(res))    //响应码错误
					{
				Log("CheckRecBag", "响应码error  %s", res);
//			  Log("CheckRecBag", "FunCode", stfield60.asFunCode, 2);
				//磁条卡现金充值回98当做超时
				memset(&stfield60, 0, sizeof(struct _Field60));
				ParseField60(&stfield60);

				//磁条卡现金充值包判断
				if (memcmp(sendbuf, "0200", 4) == 0
						&& memcmp(proccode, "63", 2) == 0
						&& memcmp(stfield60.asFunCode, "48", 2) == 0) {
					if (memcmp(res, "98", 2) == 0) {
						//lilin 20140214磁条卡现金充值不显示交易失败要进行磁条卡现金充值确认
//                        GetStrResponseError(dispbuf);
//                        sdkDispPromptBox(false, 500, dispbuf);
						return SDK_TIME_OUT;
					}
				}
				/*=======BEGIN: lilin 2015.01.12  15:31 modify===========*/
				if ((memcmp(sendbuf, "0200", 4) == 0)
						&& (memcmp(proccode, "00", 2) == 0)
						&& (memcmp(stfield60.asFunCode, "22", 2) == 0))     //消费
						{
					if ((strcmp(res, "10") == 0)
							&& (stfield60.ucPortion != '1')) //如果不是部分扣费还返回10则需要冲正
							{
						return ERR_NEEDREVERSAL;
					}
					if ((strcmp(res, "10") == 0)
							&& (stfield60.ucPortion == '1'))           //部分扣款
							{
						return ERR_RESPONSE10;
					}
				}
				/*====================== END======================== */
				return ERR_RESPONSE;
			}
		}
	}

	//都正确处理更新时间和日期
	if (sdk8583IsDomainExist(pstRevBag, 12))                         //更新交易时间
			{
		memset(revbuf, 0, sizeof(revbuf));
		rlt = GetFieldData(pstRevBag, 12, revbuf, 64);
		/*
		 if (rlt)
		 {
		 sdkAscToBcd(&gstSys.ucHour, &revbuf[0], 2);
		 sdkAscToBcd(&gstSys.ucMinute, &revbuf[2], 2);
		 sdkAscToBcd(&gstSys.ucSecond, &revbuf[4], 2);
		 }
		 */
	}

	if (sdk8583IsDomainExist(pstRevBag, 13))                     //更新交易日期
			{
		memset(revbuf, 0, sizeof(revbuf));
		rlt = GetFieldData(pstRevBag, 13, revbuf, 64);
		/*
		 if (rlt)
		 {
		 sdkAscToBcd(&gstSys.ucMonth, &revbuf[0], 2);
		 sdkAscToBcd(&gstSys.ucDay, &revbuf[2], 2);
		 }
		 */
	}
	// Trace("CheckRecBag", "OK\r\n");
	Log("wangb", "CheckRecBag OK");
	return SDK_OK;
}
/*******************************************************************
 作	 者: 麦浩卿
 版	 权: 深圳新国都股份有限公司
 函数功能:bcd金额转成字符串金额
 入口参数:
 返 回 值:
 备	 注:
 ********************************************************************/
s32 BcdAmount2AscAmount(u8 *heBcdAmount, u8 *asAmount) {
	s32 i = 0;
	u8 asTmp[14] = { 0 };
	u8 asPoint[4] = ".";
	memset(asAmount, 0x00, 14);
	while (i < 5) {
		if (heBcdAmount[i] != 0x00)
			break;
		i++;
	}
	//Trace("Mai","1、金额: %s \r\n", asAmount);
	if (i != 5) {
		sdkBcdToAsc(asTmp, &heBcdAmount[i], 5 - i);
		if (asTmp[0] == '0')
			strcpy(asAmount, &asTmp[1]);
		else
			strcpy(asAmount, asTmp);
	} else {
		asAmount[0] = '0';
	}
	//Trace("Mai","2、金额: %s \r\n", asTmp);
	//Trace("Mai","2、金额: %s \r\n", asAmount);
	//if(heBcdAmount[5] != 0x00)
	//{
	sdkBcdToAsc(&asPoint[1], &heBcdAmount[5], 1);
	strcat(asAmount, asPoint);
	//}
	//Trace("Mai","3、金额: %s \r\n", asAmount);
	return SDK_OK;

}
/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	u8 *src  要打印的数据
 u8 *data
 u8 * tag  要打印的TAG
 打印数据
 wangb 2016.03.15
 ** Returned value:
 *****************************************************************************/

s32 magdata(u8 *src, u8 *data, u8 * tag) {
	u8 getbuf[128] = { 0 };
	strcat(src, data);
	if (ProviderGetTagValue(tag, getbuf)) {
		strcat(src, getbuf);
	}
	strcat(src, "\n");
	return SDK_OK;
}
