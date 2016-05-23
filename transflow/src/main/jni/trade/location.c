#include "appglobal.h"

//static SDK_8583_ST8583 gstSigSendBag;
//static SDK_8583_ST8583 gstSigRecvBag;
/*****************************************************************************
** Descriptions:	组定位报文
** Parameters:          struct _TradeData *pstSaleData
                               u8 *pheSendData
** Returned value:
** Created By:		lilin2012.08.01
** Remarks:
*****************************************************************************/
s32 FormLocationBag(SDK_8583_ST8583 *pstSendBag8583)
{
	u8 ucTradeId;//消息ID
	u8 buf[64] = {0};
	int nRet;
	s32 len = 0,i=0;
	u16 length = 0;

	s32 tmplen = 0;
	u8 temp[256] = {0};
	u8 str[256] = {0};
	u8 aucTempData[256] = { 0 };	// 暂存数据

	struct _KeySystemParam *pst_KeySystemParam = &gstAppSetting.stKeySystemParam;
	struct _SYS *pst_SYS = &gstSys;
	struct _TradeData *pstTradeData = &gstTradeData;

	ucTradeId = pstTradeData->ucTradeId;

    if (pstSendBag8583 == NULL || pstTradeData == NULL)
    {
        return false;
    }
     nRet = FormPublicSend(pstSendBag8583,pstTradeData, "0020000000C00052");
     if(nRet <= 0)
        return nRet;
     SetFieldData(pstSendBag8583, SDK_8583_FIELD_MSG, "0800", 4);

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
	SetFieldData(pstSendBag8583, 58, str, length);

     //60域
    {
//        Log("wangb","pstTradeData->stSysParam.asBatchNo is %s ",pstTradeData->stSysParam.asBatchNo);
        memset(temp, 0, sizeof(temp));
        strcat(temp, "00");  //60.1
        strcat(temp,gstSys.asBatchNo);   //60.2批次号

        //60.3网络管理信息码
        strcat(temp, "998");
        SetFieldData(pstSendBag8583, 60, temp, strlen(temp));
    }

    //63域
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%03d", gstTradeData.usManNo);
	i = strlen(temp);
//	if (i < 3)
//	{
//		temp[2] = 0x20;
//	}

	/*=======BEGIN: huangxin 2015.01.25  13:7 modify===========
            在密钥下载交易中，上送终端型号（厂商名|型号）。
            该域由两个子域构成，具体描述如下：
            63.2.1   保留域   固定值"  "
            63.2.2   终端型号信息  ANS20
            命名规范
            厂商缩写+"|"+产品型号，总长度不超过20位，其中字符要求为英文
            和数字，不予许其他字符，以竖线分割。
            例如：00LANDI |EPT5630
	============================== END======================== */
	memcpy(temp+i, " XGD|", 5);
	i += 5;
//        rlt = sdkSysGetMachineCode(NULL);
//        ConverMachineCode(rlt, &temp[i]);
	memcpy(temp+i, "G870 ", 5);
	Log("zbq","63 Filed = %s",temp);
	SetFieldData(pstSendBag8583, 63, temp, strlen(temp));

	//重新改写报文头
	ReConfigBag8583(pstSendBag8583);

#ifdef EncriptBagControl
	nRet = WriteSendBag(TAG_8583_SENDBAG,pstSendBag8583);
	//全报文加密
	EncryptAllMsgByHX(1, pstSendBag8583->ucBagData, &pstSendBag8583->nBagLen);
#endif

	return pstSendBag8583->nBagLen;
}

/*****************************************************************************
** Descriptions:
** Parameters:    	u8 *indata
                               u32 len
                               u8 *outdata
                               u32 *rlen
                               初始定位入口
                               wangb 2016.03.01
** Returned value:
** Created By:		huacong  2016.03.03
** Remarks: 		前面三个入参好似没有用，后面再干掉
*****************************************************************************/
s32 Locationstart(u8 *indata,u32 len,u8 *outdata,u32 *rlen)
{
	Log("zbq", "Jump into Locationstart");

	s32 nRet = 0;

	SDK_8583_ST8583 stSendBag8583;

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));

	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));

	gstTradeData.ucTradeId = TRADEID_LOCATION;

	//初始化系统参数
	nRet = InitSysParam();
	/*     if(nRet < SDK_OK)
	{
	//异常提示后续修改
	return nRet;
	} */
	Log("wangb","InitSysParam is %d ",nRet);

 //组消初始定位报文
    nRet = FormLocationBag(&stSendBag8583);
    if(nRet <= SDK_EQU)
    {
       return nRet;
    }
    nRet =  stSendBag8583.nBagLen;

#ifdef EncriptBagControl
	nRet = WriteSendBag(TAG_8583_SENDECRIPTBAG,&stSendBag8583);
#else
	nRet = WriteSendBag(TAG_8583_SENDBAG,&stSendBag8583);
#endif

    if(nRet < SDK_OK)
    {
      //所有的异常应该都抛上去吧
       return nRet;
    }
     return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:    	u8 *indata
                               u32 len
                               u8 *outdata
                               u32 *rlen
                               签到解包入口
                               wangb 2016.03.01
** Returned value:
** Created By:		huacong  2016.03.03
** Remarks: 		前面二个入参好似没有用，后面再干掉
*****************************************************************************/
s32 LocationParse8583(u8 *indata,u32 inlen,u8 * outdata,u32 rlen)
{
	Log("zbq", "Jump into SignParse8583");

	s32 nRet = 0;
	struct _TradeData stTradeData;    //Fix me

	u8 recvbuf[2048]= {0};
	SDK_8583_ST8583 stRevBag8583;
	SDK_8583_ST8583 stSendBag8583;

	u8 aucTempBag[2048] = {0};

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));

	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));
	memset(&stRevBag8583, 0, sizeof(SDK_8583_ST8583));

	//初始化系统参数
	nRet = InitSysParam();
	/*     if(nRet < SDK_OK)
	{
	//异常提示后续修改
	return nRet;
	} */
	Log("wangb","InitSysParam is %d ",nRet);

	nRet = ReadSendBag(&stSendBag8583);
	if (!nRet)
	{
	return nRet;
	}
	nRet = ReadRevBag(&stRevBag8583);
	if (!nRet)
	{
	return nRet;
	}
	nRet = CheckRecBag(&stSendBag8583, &stRevBag8583);
    if(nRet != SDK_OK)
    {
        return nRet;
    }
    return SDK_OK;
}

