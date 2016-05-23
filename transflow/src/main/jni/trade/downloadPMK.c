#include "appglobal.h"
//
//extern struct _SYS gstSys;           //系统参数
//extern struct _AppSetting gstAppSetting;    //pos参数


/*****************************************************************************
** Descriptions:	组联机下载主密钥报文
** Parameters:          struct _TradeData *pstSaleData
                               u8 *pheSendData
** Returned value:
** Created By:		lilin2012.08.01
** Remarks:
*****************************************************************************/
s32 FormLoadPMKBag(SDK_8583_ST8583 *pstSendBag8583)
{
    u8 ucTradeId;//消息ID
    u8 buf[64] = {0};
    int nRet;
    s32 len = 0,i=0;

    s32 tmplen = 0;
    u8 temp[256] = {0};
    u8 str[256] = {0};

    struct _KeySystemParam *pst_KeySystemParam = &gstAppSetting.stKeySystemParam;
    struct _SYS *pst_SYS = &gstSys;
    struct _TradeData *pstTradeData = &gstTradeData;

    ucTradeId = pstTradeData->ucTradeId;

    if (pstSendBag8583 == NULL || pstTradeData == NULL)
    {
        return false;
    }
    nRet = FormPublicSend(pstSendBag8583, pstTradeData, "0020000000C00016");

    if (nRet <= 0)
    {
        return 0;
    }
    SetFieldData(pstSendBag8583, SDK_8583_FIELD_MSG, "0800", 4);

    //60域
    {
        memset(temp, 0, sizeof(temp));
        strcat(temp, "00");                 //60.1
        strcat(temp, pst_SYS->asBatchNo);     //60.2批次号
        strcat(temp, "999");                //60.3
        Log("zbq","60 Filed= %s",temp);
        SetFieldData(pstSendBag8583, 60, temp, strlen(temp));
    }

    //62域
    {
        /*=======BEGIN: huangxin 2015.01.26  11:5 modify===========
        主密钥下载时上送主密钥ID
        定义:
        数据元长度	N3
        主密钥ID：	N6
        在进行主密钥下载时上送；主密钥ID是唯一标识一条主密钥的标识号，
        该标识号是由密钥母POS灌入子POS的。
        ============================== END======================== */
        Log("zbq","uiTmkID is %s ",pst_KeySystemParam->uiTmkID);
        SetFieldData(pstSendBag8583, 62, pst_KeySystemParam->uiTmkID, 6);
    }
    //63域
    {
        memset(temp, 0, sizeof(temp));
        strcpy(temp,"999");
        i = 3;

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
    }

    //重新改写报文头
    ReConfigBag8583(pstSendBag8583);


#ifdef EncriptBagControl
    {
    nRet = WriteSendBag(TAG_8583_SENDBAG,pstSendBag8583);

    //全报文加密
    EncryptAllMsgByHX(1, pstSendBag8583->ucBagData, &pstSendBag8583->nBagLen);
    }
#endif

    return pstSendBag8583->nBagLen;
}

/*****************************************************************************
** Descriptions:	初始化下载主密钥参数
** Parameters:    	struct _TradeData *pstTradeData
** Returned value:
                    修改 wangb  2016.03.08
** Created By:		huacong  2016.03.04
** Remarks
*****************************************************************************/
s32 InitLoadPMKTradeParam()
{
    u8 value[512];
    struct _KeySystemParam *pst_KeySystemParam = &gstAppSetting.stKeySystemParam;
    struct _SYS *pst_SYS = &gstSys;
    struct _TradeData *pstTradeData = &gstTradeData;

    //主密钥ID
    memset(value, 0, sizeof(value));
    if(ProviderGetTagValue(TAG_TRADE_PMKID, value))   // Fix me
    {
        Log("zbq","TAG_TRADE_PMKID = %s",value);
        memcpy(pst_KeySystemParam->uiTmkID,value,6);
    }
    else
    {
        return SDK_ERR;
    }

//    //批次号
//    memset(value, 0, sizeof(value));
//    if(ProviderGetTagValue(TAG_BatchNo, value))   // Fix me
//    {
//        Log("zbq","TAG_BatchNo = %s",value);
//        memcpy(pst_SYS->asBatchNo,value,6);
//    }
//    else
//    {
//        return SDK_ERR;
//    }

//    //加密选择3DES、DES
//    memset(value, 0, sizeof(value));
//    if (ProviderGetTagValue(TAG_TRADE_DESTYPE, value))   // Fix me
//    {
//    	if(!memcmp(value,"0",1))
//		pst_KeySystemParam->bIsMk3Des = false ;
//	else if(!memcmp(value,"1",1))
//		pst_KeySystemParam->bIsMk3Des = true;
//
//    }
//    else
//    {
//        return SDK_ERR;
//    }
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *indata
                               u32 len
                               u8 *outdata
                               u32 *rlen
                               联机下载主密钥入口
                               wangb 2016.03.01                            
** Returned value:	
** Created By:		huacong  2016.03.03
** Remarks: 		前面三个入参好似没有用，后面再干掉
*****************************************************************************/
s32 LoadPMKStart(u8 *indata,u32 len,u8 *outdata,u32 *rlen)
{
    Log("zbq", "Jump into LoadPMKStart");

     s32 nRet = 0;
     
     SDK_8583_ST8583 stSendBag8583;

     memset(&gstTradeData, 0, sizeof(struct _TradeData));
     memset(&gstAppSetting, 0, sizeof(struct _AppSetting));
     memset(&gstSys, 0, sizeof(struct _SYS));

     memset(&stSendBag8583, 0, sizeof(SDK_8583_ST8583));
     
     gstTradeData.ucTradeId = TRADEID_DOWNKEY;

     //初始化系统参数
     nRet = InitSysParam();
/*     if(nRet < SDK_OK)
     {
      //异常提示后续修改
       return nRet;
     } */
     Log("wangb","InitSysParam is %d ",nRet);
     
     //初始化联机下载主密钥参数
     nRet = InitLoadPMKTradeParam();
/*     if(nRet < SDK_OK)
     {
      //异常提示后续修改
       return nRet;
     } */
 //组联机下载主密钥报文
    nRet = FormLoadPMKBag(&stSendBag8583);
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
** Parameters:    	u8 *uiCalData
** Returned value:	
** Created By:		huangxin  2015.01.28
** Remarks: 		
*****************************************************************************/
s32 GetCalData(u8 *uiCalData, SDK_8583_ST8583 *gstRecvBag)
{
    u8 caldata[32 + 1] = {0};
    s32 i = 0;
    u8 VouchNo[6 + 1] = {0}, SysRefNo[12 + 1] = {0},termNo[8 + 1] = {0};

    memset(caldata, 0x30, sizeof(caldata));
    //1����ȡ�ն˺�
    if(GetFieldData(gstRecvBag, 41, termNo, sizeof(termNo)) > 0)   
    {
        memcpy(caldata, termNo, 8);
        i += 8;
    }
    else
    {
        return SDK_ESC;
    }
    //2����ȡ��ˮ��
    if(GetFieldData(gstRecvBag, 11, VouchNo, sizeof(VouchNo)) > 0)
    {
        memcpy(&caldata[i], VouchNo, 6);
        i += 6;
    }
    else
    {
        return SDK_ESC;
    }
    //3����ȡ�����ο���
    if(GetFieldData(gstRecvBag, 37, SysRefNo, sizeof(SysRefNo)) > 0)
    {
        memcpy(&caldata[i], SysRefNo, 12);
        i += 12;
    }
    else
    {
        return SDK_ESC;
    }
    //4����0����32λ
    i += 6;                     
    //5�������������
    sdkAscToBcd(uiCalData, caldata, i);
    
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	用KLK解密下载的主密钥密文，保存主密钥明文到密码键盘
** Parameters:          void
** Returned value:
** Created By:		lilin2012.08.07
                                修改wangb 2016 0305
                                修改内容获取索引参数
** Remarks:
*****************************************************************************/
static s32 SaveTMK(SDK_8583_ST8583 *gstRecvBag) {
    Log("zbq", "jump into SaveTMK");
    bool flag = false;	
    u8 aucTempData[256] = {0};
    u8 aucTempBag[2048] = {0};
    s32 Mk3Des,i = 0,rlt,ret = 0,len;
    u8 TmpTMK[16],Downkek[41 + 1] = {0};
    u8 Kektype,calcheck[8 + 1] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    u8 checkvalue[8 + 1] = {0},calckek[16 + 1] = {0},CalData[16 + 1] = {0};
    struct _KeySystemParam *pst_KeySystemParam = &gstAppSetting.stKeySystemParam;

//    Mk3Des = pst_KeySystemParam->bIsMk3Des ;
    Mk3Des = 1;

//    memset(aucTempBag,0,2048);
//	sdkBcdToAsc(aucTempBag,gstRecvBag->ucBagData,gstRecvBag->nBagLen);
//	Log("zbq", "gstRecvBag = %s ",aucTempBag);

    len = GetFieldData(gstRecvBag, 62, Downkek, sizeof(Downkek));
    Log("zbq", "62Filedlen = %d", len);

    memset(aucTempData,0,256);
    sdkBcdToAsc(aucTempData,Downkek,len);
    Log("zbq", "62Filed = %s",aucTempData);

    if (len > 0)
    {
    	if(Downkek[0] == 'A' || Mk3Des == 0) 
        {
            //不支持单倍密钥算法

    		Log("zbq", "不支持单倍密钥算法,Mk3Des = %d", gstAppSetting.stKeySystemParam.bIsMk3Des);
            return SDK_ESC;
        }
        Kektype = Downkek[0];   //首字节是后台下发密钥时给出的密钥类型
        i += 1;
        if(len != 41) {return SDK_ESC;}  //数据长度有误
        sdkAscToBcd(calckek, &Downkek[i], 32);
        i += 32;

        memset(aucTempData,0,256);
        sdkBcdToAsc(aucTempData,calckek,16);
        Log("huangxin", "下发的密钥密文 = %s ", aucTempData);

        sdkAscToBcd(checkvalue, &Downkek[i], 8);
        i += 8;
        memset(aucTempData,0,256);
        sdkBcdToAsc(aucTempData,checkvalue,4);
        Log("huangxin", "下发的密钥校验值 = %s", aucTempData);
        switch(Kektype)
        {
            case 'B':                               //三DES
                if(Mk3Des == 1)    //判断终端设置的加密类型是不是三DES
                {
                    //1、调用预装的KLK对下发的密钥进行解密
//                    rlt = sdkPEDTmkDes(KLKIndexNum, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, calckek, TmpTMK);
//                    if(rlt != SDK_OK) {break;}
                    rlt = ProviderDesByTmsKey(calckek,8,TmpTMK,8,1);
                    if(rlt != SDK_OK) {break;}

//                    rlt = sdkPEDTmkDes(KLKIndexNum, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, &calckek[8], &TmpTMK[8]);
//                    if(rlt != SDK_OK) {break;}
                    rlt = ProviderDesByTmsKey(&calckek[8],8,&TmpTMK[8],8,1);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
                    sdkBcdToAsc(aucTempData,TmpTMK,16);
                    Log("huangxin", "调用预装的KLK对下发的密钥进行解密 = %s", aucTempData);

                    //2、与0进行加密获取校验值
                    rlt = sdkDes3S(true, calcheck, TmpTMK);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
                    sdkBcdToAsc(aucTempData,calcheck,8);
                    Log("huangxin", "获取校验值进行比对 = %s", aucTempData);

                    //3、与下发的校验值进行比对
                    if(memcmp(calcheck, checkvalue, 4) == 0) {flag = true;}
                }
                break;

            case 'C':                               //三DES自定义算法
                if(Mk3Des == 1)    //判断终端设置的加密类型是不是三DES
                {
                    //1、组织运算密钥
                    rlt = GetCalData(CalData, gstRecvBag);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
                    sdkBcdToAsc(aucTempData,CalData,16);
                    Log("huangxin", "组装的运算数据 = %s", aucTempData);

                    //2、用运算密钥对下发密文进行解密
                    rlt = sdkDes3S(false, calckek, CalData);
                    if(rlt != SDK_OK) {break;}

                    rlt = sdkDes3S(false, &calckek[8], CalData);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
                    sdkBcdToAsc(aucTempData,calckek,16);
                    Log("huangxin", "用运算密钥对下发密文进行解密 = %s", aucTempData);

                    //3、调用预装的KLK对下发的密钥进行解密
//                    rlt = sdkPEDTmkDes(KLKIndexNum, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, calckek, TmpTMK);
                    u32 TmpTMKlen = 0;
                    rlt = ProviderDesByTmsKey(calckek,8,TmpTMK,&TmpTMKlen,1);
                    if(rlt != SDK_OK) {break;}

//                    rlt = sdkPEDTmkDes(KLKIndexNum, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, &calckek[8], &TmpTMK[8]);
                    rlt = ProviderDesByTmsKey(&calckek[8],8,&TmpTMK[8],&TmpTMKlen,1);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
					sdkBcdToAsc(aucTempData,TmpTMK,16);
					Log("huangxin", "调用预装的KLK对下发的密钥进行解密 = %s", aucTempData);

                    //4、与0进行加密获取校验值与下发的校验值进行比对
                    rlt = sdkDes3S(true, calcheck, TmpTMK);
                    if(rlt != SDK_OK) {break;}

                    memset(aucTempData,0,256);
                    sdkBcdToAsc(aucTempData,calcheck,8);
                    Log("huangxin", "获取校验值进行比对 = %s", aucTempData);

                    //5、与下发的校验值进行比对
                    memset(aucTempData,0,256);
					sdkBcdToAsc(aucTempData,calcheck,8);
                    Log("huangxin", "计算的校验值 = %s", calcheck);

                    memset(aucTempData,0,256);
					sdkBcdToAsc(aucTempData,checkvalue,8);
                    Log("huangxin", "待比较校验值 = %s", checkvalue);
                    if(memcmp(calcheck, checkvalue, 4) == 0) {flag = true;}
                }
                break;
        }
	}

    if(flag == true)
    {
        ret = ProviderUpdatePMK(TmpTMK, 16);//加载主密钥
    }
    else
    {
        ret = SDK_ESC;
    }
    return ret;
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
s32 LoadPMKParse8583(u8 *indata,u32 inlen,u8 * outdata,u32 outlen)
{
     Log("zbq", "Jump into LoadPMKParse8583");
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
     
     gstTradeData.ucTradeId = TRADEID_DOWNKEY;

     //初始化系统参数
     nRet = InitSysParam();
/*     if(nRet < SDK_OK)
     {
      //异常提示后续修改
       return nRet;
     } */
     Log("wangb","InitSysParam is %d ",nRet);
     
     //初始化联机下载主密钥参数
     nRet = InitLoadPMKTradeParam();
/*     if(nRet < SDK_OK)
     {
      //异常提示后续修改
       return nRet;
     } */

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
//        nRet = DecryptWK(&stRevBag8583);               //更新工作秘钥

//    	Log("zbq", "stRevBag8583.nBagLen = %d ",stRevBag8583.nBagLen);
//    	memset(aucTempBag,0,2048);
//		sdkBcdToAsc(aucTempBag,stRevBag8583.ucBagData,stRevBag8583.nBagLen);
//		Log("zbq", "gstRecvBag = %s ",aucTempBag);
        nRet = SaveTMK(&stRevBag8583); //保存主密钥
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









