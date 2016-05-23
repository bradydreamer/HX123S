#include "appglobal.h"

//static SDK_8583_ST8583 gstSigSendBag;
//static SDK_8583_ST8583 gstSigRecvBag;
/*****************************************************************************
** Descriptions:	组签到报文
** Parameters:          struct _TradeData *pstSaleData
                               u8 *pheSendData
** Returned value:
** Created By:		lilin2012.08.01
** Remarks:
*****************************************************************************/
s32 FormSingBag(SDK_8583_ST8583 *pstSendBag8583)
{
	u8 ucTradeId;//消息ID
	u8 buf[64] = {0};
	int nRet;
	s32 len = 0,i=0;

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
     nRet = FormPublicSend(pstSendBag8583,pstTradeData, "0020000000C00012");
     if(nRet <= 0)
        return nRet;
     SetFieldData(pstSendBag8583, SDK_8583_FIELD_MSG, "0800", 4);

	//48域
	{
	 /*=============BEGIN: huangxin 2015.01.25  11:59 modify===========
	 10.3.22.6　用法四：参数版本信息
	 签到和参数传递中本域用于存放参数版本信息，如果终端没有原始参数信息，
	 参数版本号填充8个'0'。采用BCD码压缩表示，共占18个字节。
	 签到请求时为POS终端当前的版本信息，应答时为POS中心最新的版本信息。
	 参数传递时为要求的版本信息。
	 数据元长度	        N3
	 终端参数版本号	    N8
	 TMS参数版本号       N8
	 公钥参数版本号      N8
	 AID参数版本号       N8
	 POS中心年份	        N4：YYYY
	 ========================== END================================== */
	 memset(temp, 0, sizeof(temp));
	 i = 0;
	 memcpy(&temp[i], gstAppSetting.stTmsParam.TermParamVer, 8);
	 i += 8;
	 memcpy(&temp[i], gstAppSetting.stTmsParam.TMSParamVer, 8);
	 i += 8;
	 memcpy(&temp[i], gstAppSetting.stTmsParam.CapkParamVer, 8);
	 i += 8;
	 memcpy(&temp[i], gstAppSetting.stTmsParam.AidParamVer, 8);
	 i += 8;

	 nRet = GetSysTime();
	 if(nRet != SDK_OK){
		 Log("zbq","GetSysTime Failed");
		 return SDK_ERR;
	 }

	 if (gstSys.ucYear >= 0x50)
	 {
		 memcpy(&temp[i], "19", 2);
		 i += 2;
	 }
	 else
	 {
		 memcpy(&temp[i], "20", 2);
		 i += 2;
	 }
	 sprintf(&temp[i], "%02X", gstSys.ucYear);
	 i += 2;

	 memset(aucTempData,0,256);
	 sdkBcdToAsc(aucTempData,temp,i);
	 Log("zbq","48域  = %s ",aucTempData);

	 SetFieldData(pstSendBag8583, 48, temp, i);
	}

     //60域
    {
//        Log("wangb","pstTradeData->stSysParam.asBatchNo is %s ",pstTradeData->stSysParam.asBatchNo);
        memset(temp, 0, sizeof(temp));
        strcat(temp, "00");  //60.1
        strcat(temp,gstSys.asBatchNo);   //60.2批次号

        //60.3网络管理信息码
        if ( gstAppSetting.stKeySystemParam.bIsMk3Des )                            //是否3des
        {
            if (gstAppSetting.stKeySystemParam.bIsSupportTdk)                    //是否支持TDK
            {
                strcat(temp, "004");
            }
            else
            {
                strcat(temp, "003");
            }
        }
        else
        {
            strcat(temp, "001");
        }
        SetFieldData(pstSendBag8583, 60, temp, strlen(temp));
    }
     //62域                                                       //Fixme 待续
//        {
//            memset(temp, 0, sizeof(temp));
//            strcpy(temp, "Sequence No");                //tag
//            sdkReadPosSn(possn);                         //读机身号
//            i = strlen(possn);
//            sdkSysGetCupSn(cupsn);                       //读中国银联证书号
//            i += strlen(cupsn);
//            sprintf(&temp[strlen(temp)], "%02d", i);
//            strcat(temp, cupsn);
//            strcat(temp, possn);
//            SetFieldData(pstBag8583, 62, temp, strlen(temp));
//        }

    memset(temp, 0, sizeof(temp));

    //63域
	memset(temp, 0, sizeof(temp));
	sprintf(temp, "%02d", gstTradeData.usManNo);
	i = strlen(temp);
	if (i < 3)
	{
		temp[2] = 0x20;
	}

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
	memcpy(&temp[i], "  XGD|", 6);
	i += 6;
//        rlt = sdkSysGetMachineCode(NULL);
//        ConverMachineCode(rlt, &temp[i]);
	memcpy(&temp[i], "G870", 4);
	Log("zbq","63 Filed = %s",temp);
	SetFieldData(pstSendBag8583, 63, temp, strlen(temp));
	TraceHex("huangxin", "下载密钥63域上送\r\n", temp, strlen(temp));

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
** Descriptions:	初始化消签到的参数
** Parameters:    	struct _TradeData *pstTradeData
** Returned value:
                    修改 wangb  2016.03.08
** Created By:		huacong  2016.03.04
** Remarks
*****************************************************************************/
s32 InitSingTradeParam()
{
	u8 value[512];
	struct _KeySystemParam *pst_KeySystemParam = &gstAppSetting.stKeySystemParam;
	struct _SYS *pst_SYS = &gstSys;
	struct _TradeData *pstTradeData = &gstTradeData;
    
    if (pstTradeData == NULL)
    {
        return SDK_ERR;
    }
    //操作员代码
    memset(value, 0, sizeof(value));
    if(ProviderGetTagValue(TAG_8583_UsManNo, value))   // Fix me
   {
       pstTradeData->usManNo = atol(value);
    } 
    else
    {
        return SDK_ERR;
    }
    
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *indata
                               u32 len
                               u8 *outdata
                               u32 *rlen
                               签到入口
                               wangb 2016.03.01                            
** Returned value:	
** Created By:		huacong  2016.03.03
** Remarks: 		前面三个入参好似没有用，后面再干掉
*****************************************************************************/
s32 SignInstart(u8 *indata,u32 len,u8 *outdata,u32 *rlen)
{
	Log("zbq", "Jump into SignInstart");

	s32 nRet = 0;

	SDK_8583_ST8583 stSendBag8583;

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));

	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));

	gstTradeData.ucTradeId = TRADEID_MANLOGIN;

	//初始化系统参数
	nRet = InitSysParam();
    if(nRet < SDK_OK)
	{//异常提示后续修改
    	Log("wangb","InitSysParam is %d ",nRet);
    	return nRet;
	}

	//初始化签到参数
	InitSingTradeParam();

 //组消签到报文
    nRet = FormSingBag(&stSendBag8583);
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
** Descriptions:	更新工作密钥
** Parameters:          void
** Returned value:
** Created By:		lilin2012.08.07
                                修改wangb 2016 0305 
                                修改内容获取索引参数
** Remarks:
*****************************************************************************/
static s32 DecryptWK(SDK_8583_ST8583 *gstSigRecvBag) {
	Log("zbq", "Jump into DecryptWK");
    u32 TMK = 0;
    s32 slt = 0;
    u8 revbuf[128] = {0};
    u8 buf[64] = {0};
    u8 tempbuf[2048] ={0};
    s32 len = 0;
    s32 num = 0;

    memset(tempbuf,0,sizeof(tempbuf));
	sdkBcdToAsc(tempbuf,gstSigRecvBag->ucBagData,gstSigRecvBag->nBagLen);
	Log("zbq","Len = %d , RecvBag = %s", gstSigRecvBag->nBagLen, tempbuf);

    // Trace("DecryptWK", "CurFifoNo=[%d]\r\n", sdkSysGetCurFifoNo());

    /*=======BEGIN: huangxin 2015.01.28  18:22 modify===========*/
    len = GetFieldData(gstSigRecvBag, 62, revbuf, 128);

    memset(tempbuf,0,sizeof(tempbuf));
	sdkBcdToAsc(tempbuf,revbuf,len);
	Log("zbq","Len = %d , 62 Filed = %s", len, tempbuf);

	sdkAscToBcd(buf, revbuf, len);
	len = (len + 1)/2;
	/*============================= END======================== */
    if (len < 0)
    {
        return SDK_ERR;
    }
    if (len != 24 && len != 40 && len != 56 && len != 60 && len != 84)
    {
    	Log("zbq", "Package Len = %d Error!", len);
        return SDK_ERR;
    }

    memset(tempbuf,0,sizeof(tempbuf));
	sdkBcdToAsc(tempbuf,buf,len);
	Log("zbq","WKLen = %d , WK = %s", len, tempbuf);

    slt = ProviderUpdateWk(buf,len);
    if (slt != SDK_OK)
    {
        return slt;
    }
    return slt;
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
s32 SignParse8583(u8 *indata,u32 inlen,u8 * outdata,u32 *rlen)
{
	Log("zbq", "Jump into SignParse8583");

	s32 nRet = 0;
	struct _TradeData stTradeData;    //Fix me

	u8 recvbuf[2048]= {0};
	SDK_8583_ST8583 stRevBag8583;
	SDK_8583_ST8583 stSendBag8583;

	u8 aucTempBag[2048] = {0};

	bool flag = false;
	s32 ret = 0, IndexNum = 0, Mk3Des,i = 0,rlt,len,KLKIndexNum = 1;
	u8 TmpTMK[16],Downkek[41 + 1] = {0};
	u8 Kektype,calcheck[8 + 1] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	u8 checkvalue[8 + 1] = {0},calckek[16 + 1] = {0},CalData[16 + 1] = {0};

	memset(&gstTradeData, 0, sizeof(struct _TradeData));
	memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
	memset(&gstSys, 0, sizeof(struct _SYS));

	memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));
	memset(&stRevBag8583, 0, sizeof(SDK_8583_ST8583));

	gstTradeData.ucTradeId = TRADEID_MANLOGIN;

	//初始化系统参数
	nRet = InitSysParam();
    if(nRet < SDK_OK)
	{
    	//异常提示后续修改
    	Log("wangb","InitSysParam is %d ",nRet);
    	return nRet;
	}

	//初始化签到参数
	nRet = InitSingTradeParam();
	if(nRet < SDK_OK)
	{
		//异常提示后续修改
		Log("wangb","InitSingTradeParam is %d ",nRet);
		return nRet;
	}

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
    if(nRet == SDK_OK)
    {
        nRet = DecryptWK(&stRevBag8583);               //更新工作秘钥
        if(nRet != SDK_OK)
        {
            return nRet;
        }
       nRet = Set8583Tvl(&stRevBag8583);
       if (nRet == SDK_EQU)
	   {
		return SDK_ERR;
	   }
    }
    else {
        return nRet;
    }
    return SDK_OK;
}
