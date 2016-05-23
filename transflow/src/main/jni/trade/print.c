#include "appglobal.h"

char* printDataBuf = NULL;

#define TAG_IN_CARD_CHECK       "FF9015"
#define TAG_OUT_CARD_CHECK      "FF900B"

#define SIGN_BILL 1					//签购单
#define DETAILED_BILL 2				//明细单
#define COUNT_BILL 3				//统计单
#define REPLAY_SIGN_BILL 4			//重打签购单
#define TAG_PRINT_SCRIPT "FF9071"

struct _TradeName                                                                               //交易名称
{
	//u8 TradeId;                                                                                 //交易ID
	char TradeId[32];                                                                                 //交易ID
	char Name[32];                                                                                //交易名称
};

typedef struct _RecordDetail
{
    char TradeId[32];                                                                             //交易ID
    char Name[32];                                                                                //交易名称
    char CardNo[20];                                                                              //卡号
    char VoucherNo[20];                                                                           //流水号
    char Amount[20];                                                                              //交易金额
    char AuthCode[20];                                                                            //授权码
} stRecordDetail;

static const struct _TradeName TradeNameTab[] =
		{
				{TAG_TRADEID_PREAUTH,       {"预授权"}},
				{TAG_TRADEID_UNPREAUTH,     {"预授权撤销"}},
				{TAG_TRADEID_ADDPREAUTH, {"追加预授权"}},
				{TAG_TRADEID_COMPLETE,      {"预授权完成请求"}},
				{TAG_TRADEID_UNCOMPLETE,    {"预授权完成撤销"}},
				{TAG_TRADEID_SALE,                  {"消费"}},
				{TAG_TRADEID_UNSALE,                {"消费撤销"}},
				{TAG_TRADEID_OFFCOMPLETE,   {"预授权完成通知"}},
				{TAG_TRADEID_REFUND,                {"退货"}},
/*				
				{TAG_TRADEID_OFFSETTLE,     {"离线结算"}},

				{TAG_TRADEID_ADJUST,                {"结算调整"}},
				{TAG_TRADEID_TIPS,                  {"结算调整"}},
				{TAG_TRADEID_OFFSALE,       {"消费"}},
				{TAG_TRADEID_OFFUNSALE,     {"消费撤销"}},
				{TAG_TRADEID_QPBOC,             {"电子现金消费"}},
				{TAG_TRADEID_ECOFFSALE,     {"电子现金消费"}},
				{TAG_TRADEID_ECCASH,            {"电子现金充值"}},
				{TAG_TRADEID_ECCACC,            {"电子现金指定账户圈存"}},
				{TAG_TRADEID_ECCACCNO,      {"电子现金非指定账户圈存"}},
				{TAG_TRADEID_ECOFFREFUND,   {"电子现金退货"}},     //电子现金脱机退货

				{TAG_TRADEID_UNECCASH,         {"电子现金充值撤销"}},     //电子现金现金充值撤销
				{TAG_TRADEID_EPSALE,        {"电子钱包消费"}},     //电子钱包消费
				{TAG_TRADEID_EPLOAD_CASH,   {"电子钱包现金充值"}},     //电子钱包现金充值
				{TAG_TRADEID_EPLOAD_ACC,    {"电子钱包指定账户圈存"}},     //电子钱包指定账户圈存
				{TAG_TRADEID_EPLOAD_NOACC,  {"电子钱包非指定账户圈存"}},     //电子钱包非指定账户圈存
				{TAG_TRADEID_EPUNLOAD_CASH,         {"电子钱包现金充值撤销"}},     //电子钱包现金充值撤销
				{TAG_TRADEID_INSTALLMENTSALE,       {"分期付款交易"}},     //分期消费
				{TAG_TRADEID_INSTALLMENTUNSALE,     {"分期付款交易撤销"}},     //分期撤销
				{TAG_TRADEID_POINTSALE_ISSUER,      {"积分消费"}},     //发卡行积分消费
				{TAG_TRADEID_POINTUNSALE_ISSUER,    {"积分消费撤销"}},     //发卡行积分消费撤销

				{TAG_TRADEID_POINTSALE_UNION,       {"积分消费"}},     //联盟积分消费
				{TAG_TRADEID_POINTUNSALE_UNION,     {"积分消费撤销"}},     //联盟积分消费撤销
				{TAG_TRADEID_POINTREFUND_UNION,     {"积分退货"}},     //联盟积分退货
				{TAG_TRADEID_MOBILESALE,    {"手机消费"}},     //手机芯片消费
				{TAG_TRADEID_MOBILEUNSALE,  {"手机消费撤销"}},     //手机芯片消费撤销
				{TAG_TRADEID_RESERVATION_SALE,      {"预约消费"}},     //预约消费
				{TAG_TRADEID_RESERVATION_UNSALE,    {"预约消费撤销"}},     //预约消费撤销
				{TAG_TRADEID_RESERVATION_REFUND,    {"预约消费退货"}},    //预约退货
				{TAG_TRADEID_MOTOSALE,      {"订购消费"}},     //订购消费
				{TAG_TRADEID_MOTOUNSALE,    {"订购消费撤销"}},     //订购消费撤销
				{TAG_TRADEID_MOTOREFUND,    {"订购退货"}},     //订购退货
				{TAG_TRADEID_MOTOPREAUTH,   {"订购预授权"}},     //预授权
				{TAG_TRADEID_MOTOUNPREAUTH,         {"订购预授权撤销"}},     //预授权撤销
				{TAG_TRADEID_MOTOCOMPLETE,  {"订购预授权完成请求"}},     //预授权完成请求
				{TAG_TRADEID_MOTOOFFCOMPLETE,       {"订购预授权完成通知"}},     //预授权完成通知
				{TAG_TRADEID_MOTOUNCOMPLETE,        {"订购预授权完成撤销"}},     //预授权完成撤销
				{TAG_TRADEID_MAGLOAD_CASH,  {"磁条卡现金充值"}},     //磁卡现金充值
				{TAG_TRADEID_MAGLOAD_ACC,   {"磁条卡账户充值"}},     //磁卡账户充值
				{TAG_TRADEID_MAGLOAD_AFFIRM, {"磁条卡现金充值"}},
				{TAG_TRADEID_EPCACC,          {"指定账户圈存"}},
				{TAG_TRADEID_MOBILEREFUND,  {"退货"}},     //订购退货
				{TAG_TRADEID_MOBILEPREAUTH,         {"预授权"}},     //预授权
				{TAG_TRADEID_MOBILEUNPREAUTH,       {"预授权撤销"}},     //预授权撤销
				{TAG_TRADEID_MOBILECOMPLETE,        {"预授权完成请求"}},     //预授权完成请求
				{TAG_TRADEID_MOBILEOFFCOMPLETE,     {"预授权完成通知"}},     //预授权完成通知
				{TAG_TRADEID_MOBILEUNCOMPLETE,      {"预授权完成撤销"}},     //预授权完成撤销
#ifdef ELECSIGN
				{TAG_TRADEID_ELECSIGN,      {"电子签字上送"}},
#endif
*/
		};

static const struct _TradeName TradeNameTabE[] =
		{
				{TAG_TRADEID_PREAUTH,       {"AUTH"}},
				{TAG_TRADEID_UNPREAUTH,     {"CANCEL"}},
				{TAG_TRADEID_ADDPREAUTH, {"ADD-AUTH"}},
				{TAG_TRADEID_COMPLETE,      {"AUTH COMPLETE"}},
				{TAG_TRADEID_UNCOMPLETE,    {"COMPLETE VOID"}},
				{TAG_TRADEID_SALE,                  {"SALE"}},
				{TAG_TRADEID_UNSALE,                {"VOID"}},
				{TAG_TRADEID_OFFCOMPLETE,   {"AUTH SETTLEMENT"}},
				{TAG_TRADEID_REFUND,                {"REFUND"}},
/*				
				{TAG_TRADEID_OFFSETTLE,         {"OFFLINE"}},
				{TAG_TRADEID_ADJUST,                {"ADJUST"}},
				{TAG_TRADEID_TIPS,                  {"ADJUST"}},
				{TAG_TRADEID_OFFSALE,       {"SALE"}},
				{TAG_TRADEID_OFFUNSALE,     {"VOID"}},
				{TAG_TRADEID_QPBOC,             {"EC SALE"}},
				{TAG_TRADEID_ECOFFSALE,     {"EC SALE"}},
				{TAG_TRADEID_ECCASH,        {"EC LOAD"}},
				{TAG_TRADEID_ECCACC,        {"EC LOAD"}},
				{TAG_TRADEID_ECCACCNO,      {"EC LOAD"}},
				{TAG_TRADEID_ECOFFREFUND,   {"EC REFUND"}},     //电子现金脱机退货

				{TAG_TRADEID_UNECCASH,         {"EC LOAD VOID"}},     //电子现金现金充值撤销
				{TAG_TRADEID_EPSALE,        {"EP SALE"}},     //电子钱包消费
				{TAG_TRADEID_EPLOAD_CASH,   {"IC LOAD"}},     //电子钱包现金充值
				{TAG_TRADEID_EPLOAD_ACC,    {"IC LOAD"}},     //电子钱包指定账户圈存
				{TAG_TRADEID_EPLOAD_NOACC,  {"IC LOAD"}},     //电子钱包非指定账户圈存
				{TAG_TRADEID_EPUNLOAD_CASH,         {"VOID"}},     //电子钱包现金充值撤销
				{TAG_TRADEID_INSTALLMENTSALE,       {"INSTALLMENT"}},     //分期消费
				{TAG_TRADEID_INSTALLMENTUNSALE,     {"VOID"}},     //分期撤销
				{TAG_TRADEID_POINTSALE_ISSUER,      {"BONUS"}},     //发卡行积分消费
				{TAG_TRADEID_POINTUNSALE_ISSUER,    {"VOID"}},     //发卡行积分消费撤销

				{TAG_TRADEID_POINTSALE_UNION,       {"BONUS"}},     //联盟积分消费
				{TAG_TRADEID_POINTUNSALE_UNION,     {"VOID"}},     //联盟积分消费撤销
				{TAG_TRADEID_POINTREFUND_UNION,     {"REFUND"}},     //联盟积分退货
				{TAG_TRADEID_MOBILESALE,    {"SALE"}},     //手机芯片消费
				{TAG_TRADEID_MOBILEUNSALE,  {"VOID"}},     //手机芯片消费撤销
				{TAG_TRADEID_RESERVATION_SALE,      {"RESERVATION SALE"}},     //预约消费
				{TAG_TRADEID_RESERVATION_UNSALE,    {"VOID"}},     //预约消费撤销
				{TAG_TRADEID_RESERVATION_REFUND,    {"REFUND"}},     //预约消费退货
				{TAG_TRADEID_MOTOSALE,      {"MOTO SALE"}},     //订购消费
				{TAG_TRADEID_MOTOUNSALE,    {"VOID"}},     //订购消费撤销
				{TAG_TRADEID_MOTOREFUND,    {"REFUND"}},     //订购退货

				{TAG_TRADEID_MOTOPREAUTH,   {"MOTO AUTH"}},     //订购预授权
				{TAG_TRADEID_MOTOUNPREAUTH,         {"CANCEL"}},     //订购预授权撤销
				{TAG_TRADEID_MOTOCOMPLETE,  {"AUTH COMPLETE"}},     //订购预授权完成请求
				{TAG_TRADEID_MOTOOFFCOMPLETE,       {"AUTH SETTLEMENT"}},     //订购预授权完成通知
				{TAG_TRADEID_MOTOUNCOMPLETE,        {"COMPLETE VOID"}},     //预授权完成撤销
				{TAG_TRADEID_MAGLOAD_CASH,  {"ACCOUNT LOAD"}},     //磁卡现金充值
				{TAG_TRADEID_MAGLOAD_ACC,   {"ACCOUNT LOAD"}},     //磁卡账户充值
				{TAG_TRADEID_MAGLOAD_AFFIRM, {"ACCOUNT LOAD"}},
				{TAG_TRADEID_EPCACC,      {"EP LOAD"}},

				{TAG_TRADEID_MOBILESALE,    {"SALE"}},     //订购消费
				{TAG_TRADEID_MOBILEUNSALE,  {"VOID"}},     //订购消费撤销
				{TAG_TRADEID_MOBILEREFUND,  {"REFUND"}},     //订购退货

				{TAG_TRADEID_MOBILEPREAUTH,         {"AUTH"}},     //预授权
				{TAG_TRADEID_MOBILEUNPREAUTH,       {"CANCEL"}},     //预授权撤销
				{TAG_TRADEID_MOBILECOMPLETE,        {"AUTH COMPLETE"}},     //预授权完成请求
				{TAG_TRADEID_MOBILEOFFCOMPLETE,     {"AUTH SETTLEMENT"}},     //预授权完成通知
				{TAG_TRADEID_MOBILEUNCOMPLETE,      {"COMPLETE VOID"}},     //预授权完成撤销
#ifdef ELECSIGN
				{TAG_TRADEID_ELECSIGN,      {"ELEC SIGN UPLOAD"}},
#endif
*/
		};


char getTypeCode(char *tradeId)
{
    char code = 0x00;

    if(strcmp(tradeId, "trade_id_sale") == 0){
        code = 'S';
    }
    else if(strcmp(tradeId, "trade_id_refund") == 0){
        code = 'R';
    }
    else if(strcmp(tradeId, "trade_id_complete") == 0){
            code = 'P';
    }
    else{
        code = 0x00;
    }

    return code;
}

int getAllRecord(stRecordDetail *pstRecord, int total, int index)
{
    int i;
	s32 siSrclen;
    char amount[20] = {0};
	char ucAmount[10] = {0};

    if((pstRecord == NULL) || (index > total))
        return -4;

    Log("getAllRecord","111111111111111111111111111111111   recordTotal = %d 111111111111111111111111111111111", total);
    for(i = index; i < total; i++)
    {
        memset(amount, 0x00, sizeof(amount));
        ProviderSetRecordToTagList(i);
        Log("getAllRecord","111111111111111111111111111111111   recordTotal = %d 111111111111111111111111111111111", i);
        ProviderGetTagValue(TAG_8583_OLDTYPE, pstRecord[i].TradeId);
        ProviderGetTagValue(TAG_8583_CARDNO, pstRecord[i].CardNo);
        ProviderGetTagValue(TAG_8583_OldVoucherNo, pstRecord[i].VoucherNo);
        siSrclen = ProviderGetTagValue(TAG_8583_AMOUNT, amount);
		sdkAscToBcd(ucAmount, amount, siSrclen);
        BcdAmount2AscAmount(ucAmount, pstRecord[i].Amount);
        ProviderGetTagValue(TAG_8583_AuthCode, pstRecord[i].AuthCode);

        Log("printDetailBill","pstRecord[i].TradeId  = %s 111111111111111111111111111111111", pstRecord[i].TradeId);
        Log("printDetailBill","pstRecord[i].CardNo  = %s 111111111111111111111111111111111", pstRecord[i].CardNo);
        Log("printDetailBill","pstRecord[i].VoucherNo  = %s 111111111111111111111111111111111", pstRecord[i].VoucherNo);
        Log("printDetailBill","pstRecord[i].Amount  = %s 111111111111111111111111111111111", pstRecord[i].Amount);
        Log("printDetailBill","pstRecord[i].AuthCode  = %s 111111111111111111111111111111111", pstRecord[i].AuthCode);
    }

    return 1;
}

/*****************************************************************************
** Descriptions:	获得交易类型名称
** Parameters:          u8 *lpOut交易名称
                               u8 nTradeId交易ID
** Returned value:
** Created By:		李琳2012.10.15
** Remarks:
*****************************************************************************/
void GetTradeName(char *asOut, char *asTradeId)
{
	u8 i;
	u16 k;

	k = sizeof(TradeNameTab) / sizeof(struct _TradeName);

	for (i = 0; i < k; i++)
	{
		if (strcmp(TradeNameTab[i].TradeId,asTradeId) == 0)
		{
			strcpy(asOut, TradeNameTab[i].Name);
			break;
		}
	}
}

/*****************************************************************************
** Descriptions:	获得交易类型英文名称
** Parameters:          u8 *lpOut交易名称
                               u8 nTradeId交易ID
** Returned value:
** Created By:		李琳2012.10.15
** Remarks:
*****************************************************************************/
void GetTradeEName(char *asOut, char *asTradeId)
{
	u8 i;
	u16 k;

	k = sizeof(TradeNameTabE) / sizeof(struct _TradeName);

	for (i = 0; i < k; i++)
	{
		if (strcmp(TradeNameTabE[i].TradeId, asTradeId) == 0)
		{
			strcpy(asOut, TradeNameTabE[i].Name);
			break;
		}
	}
}


s32 CoverCardNo(u8 *pasCardNo, u8 *pasCoverNo)
{
	u16 usLen = 0;

	if(pasCardNo == NULL || pasCoverNo == NULL || strlen(pasCardNo) < 10)
		return SDK_PARA_ERR;

	usLen = strlen(pasCardNo);
	//显示前六后四
	memset(pasCoverNo, '*', usLen);
	memcpy(pasCoverNo, pasCardNo, 6);
	memcpy(&pasCoverNo[usLen-4], &pasCardNo[usLen-4], 4);

	return SDK_OK;
}

void getTradeTypeStr(char *tradeStr, int type)
{
	char buf[128] = {0};
	char tmp[64] = {0};
	char tradeID[32] = {0};

	if(type == 0)
		ProviderGetTagValue(TAG_TRADE_TYPE, tradeID);
	else
		ProviderGetTagValue(TAG_8583_OLDTYPE, tradeID);
	Log("PRINT", "111111111111111111tradeID = %s", tradeID);

	GetTradeName(buf, tradeID);
	strcat(buf, "/");
	GetTradeEName(tmp, tradeID);
	strcat(buf, tmp);

	Log("PRINT", "111111111111111111tradeID = %s", buf);
	strcpy(tradeStr, buf);
	Log("PRINT", "111111111111111111tradeID = %s", tradeStr);

}

void getTimeStr(char *timeStr)
{
	char buf[128] = {0};
	char tmp[64] = {0};

	ProviderGetTagValue("FF9005", buf);
	Log("PRINT", "date  = %s", buf);
	strcat(buf, " ");
	ProviderGetTagValue("FF9004", tmp);
	strcat(buf, tmp);
	Log("PRINT", "date + time  = %s", buf);

	strcpy(timeStr, buf);

}


/**
* 初始化打印
*
*/
int initPrint(){

	if(printDataBuf != NULL){
		free(printDataBuf);
	}
	printDataBuf = malloc(50*1024);
	memset(printDataBuf, 0x00, 50*1024);

	setChnFontSize("n");
	setAscFontSize("n");
	setPrintGray(5);
	setPrintLineSpace(6);
	setBarCodeConfig(2, 64);
	setQrCodeConfig(100, 100, 2);

	return 1;
}

/**
* 设置中文字体大小
*(s,n,l,sn,sl,nl)
*s（小字体）：一行打印24个(16x16)；
*n（标准字体）：一行打印16个(24x24)；
*l（大字体）：一行打印12个；(32x32)
*sn：小字体宽度，标准字体高度；(16x24)
*sl：小字体宽度，大字体高度；(16x32)
*nl：标准字体宽度，大字体高度；(24x32)
*默认为 n
*/
int setChnFontSize(char* fontSize){
	int len;

	if(strcmp(fontSize, "s")==0 || strcmp(fontSize, "n")==0 || strcmp(fontSize, "l")==0||
	   strcmp(fontSize, "sn")==0 || strcmp(fontSize, "sl")==0|| strcmp(fontSize, "nl")==0){

		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len], "!hz %s\n",fontSize);

	return 1;
}

/**
* 设置ascll字体大小
*s,n,l,sn,sl,nl)
*s（小字体）：一行打印48个(8x16)；
*n（标准字体）：一行打印32个(12x24)；
*l（大字体）：一行打印24个；(16x32)
*sn：小字体宽度，标准字体高度(8x24)
*sl：小字体宽度，大字体高度(8x32)
*nl：标准字体宽度，大字体高度(12x32)
*默认为 n
*/
int setAscFontSize(char* fontSize){
	int len;

	if(strcmp(fontSize, "s")==0 || strcmp(fontSize, "n")==0 || strcmp(fontSize, "l")==0||
	   strcmp(fontSize, "sn")==0 || strcmp(fontSize, "sl")==0 || strcmp(fontSize, "nl")==0){

		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len], "!asc %s\n",fontSize);

	return 1;

}

/**
* 设置打印灰度
*灰度/浓度，文字粗细（对于只支持加粗的
*打印机进行如下操作：1~5为标准，6~10为加粗）
*[1，10]
*默认5  
*/
int setPrintGray(u32 grayLevel){
	int len;

	if(grayLevel< 1 || grayLevel > 10){
		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"!gray %d\n", grayLevel);

	return 1;
}

/**                                          
* 设置行间距                            
*
*@param lineSpace
*行间距大小
*[0, 60]
*默认6                                    
*/
int setPrintLineSpace(u32 lineSpace){
	int len;

	if(lineSpace > 60){
		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"!yspace %d\n", lineSpace);

	return 1;
}

/**                                          
* 设置条码配置   
*@param width                      
*单条条码宽度（从效果来看，建议宽度>=2）
*[1，8],默认2                                  
*@param height
*期望高度               
*[1，320]且必须是8的倍数,默认64 
*                                         
*/
int setBarCodeConfig(u32 width, u32 height){

	char cmd[36] = {0};

	if(width < 1 || width > 8 || height < 1 || height > 320){
		return -4;
	}

	sprintf(cmd,"!barcode %d %d\n", width, height);
	if(strlen(printDataBuf)){
		strcat(printDataBuf, cmd);
	}else{
		strcpy(printDataBuf, cmd);
	}
	return 1;
}

/**                                          
* 设置二维码配置                            
*@param width
*不超过打印边界(384)
*@param height
*(int)
*默认100
*@param correctionLevel 纠错级别
*[0,3]
*一般为2                               
*/
int setQrCodeConfig(u32 width, u32 height, u32 correctionLevel){
	int len = 0;

	if(width < 1 || width > 8 || height < 1 || height > 320){
		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"!qrcode %d %d %d\n", width, height, correctionLevel);
	return 1;

}


/**                                          
* 打印文本                        
* @param dispSite
* 显示位置
* (l,c,r)l:居左c:居中r:居右   
* @param text
* 待打印的路径和文件名                
*/
int writePrintText(char dispSite, char* text){
	int len = 0;

	if((dispSite != 'l'&& dispSite != 'c' && dispSite != 'r' ) || text == NULL){
		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"*text %c %s\n", dispSite, text);
	return 1;
}

/**                                          
* 打印图片                        
* 用于打印logo、电子签名、条码和二维码等
* @param dispSite
* 显示位置
* (l,c,r)l:居左 c:居中 r:居右
* @param imagePath
* 待打印的路径和文件名                
*/
int writePrintImage(char dispSite, char* imagePath){
	int len = 0;

	if((dispSite != 'l'&& dispSite != 'c' && dispSite != 'r' ) || imagePath == NULL){
		return -4;
	}

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"*image %c %s\n", dispSite, imagePath);
	return 1;
}

/**                                          
* 暂停时间                        
* 用于多联打印
* @param times
*（单位秒）                            
*/
int writePauseTime(u32 times){
	int len = 0;

	len = strlen(printDataBuf);
	sprintf(&printDataBuf[len],"*pause %d\n", times);
	return 1;

}

/**
* 开始打印
* 打印出来缓存的内容
*
*/
int startPrint(){

	ProviderPrintScript(printDataBuf);
	free(printDataBuf);
	printDataBuf = NULL;

	return 1;
}

/*****************************************************************************
** Descriptions:		写入打印脚本到java的tag内
** Parameters:

                               wangb 2016.03.15
** Returned value: 成功返回 1，失败返回 -1 、-4

*****************************************************************************/
int writePrintScript()
{
	int iRet;

	iRet = ProviderSetTagValue(TAG_PRINT_SCRIPT, printDataBuf, strlen(printDataBuf));
	free(printDataBuf);
	printDataBuf = NULL;

	return iRet;
}

/*****************************************************************************
** Descriptions:		打印脚本的组织分发
** Parameters:


** Returned value: 成功返回 1，失败返回 -1 、-4

*****************************************************************************/
int printMain(int printType)
{
	if(printType  == SIGN_BILL){
		printSignBill(printType);
	}
	else if(printType  == DETAILED_BILL){
        printDetailBill();
	}
	else if(printType  == COUNT_BILL){
		printSettleBill();
	}
	else if(printType  == REPLAY_SIGN_BILL){
		reprintSignBill();
	}else{

		return -4;
	}
	return 1;
}

/*****************************************************************************
** Descriptions:		打印签购单 （持卡人联 和 商户联）
** Parameters:

                               wangb 2016.03.15
** Returned value:

*****************************************************************************/
s32 printSignBill(){
	u8 buf[32] = {0};
	u8 cmd[128] = {0};
	u8 Amount[16] = {0};
	s32 result;
	initPrint();


	writePrintText('c',"商户名称：");

	memset(cmd,0,128);
	magdata(cmd,"","FF8018");
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"商户编号:            ",TAG_8583_MerchantNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"终端编号:               ",TAG_8583_TerminalNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"操作员号:                  ",TAG_8583_UsManNo);
	writePrintText('c',cmd);


	memset(cmd,0,128);
	magdata(cmd,"发卡行:               ",TAG_8583_IssuerBank);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"收单行:               ",TAG_8583_AcquirerBank);
	writePrintText('c',cmd);

	memset(cmd,0,sizeof(cmd));
	memset(buf,0, sizeof(buf));
	result = ProviderGetTagValue(TAG_8583_CARDNO, cmd);
	if(result > 0){
		CoverCardNo(cmd, buf);
		memset(cmd,0,sizeof(cmd));
	}
	sprintf(cmd,"卡号(CARD):  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	writePrintText('c',"交易类型(TRAN TYPE):          \n");

	memset(cmd,0,sizeof(cmd));
	memset(buf,0,sizeof(buf));
	getTradeTypeStr(buf, 0);
	Log("PRINT", "22222222222222222  tradeID = %s", buf);
	sprintf(cmd, "      %s\n", buf);
	Log("PRINT", "22222222222222222  tradeID = %s", cmd);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"批次号:                ",TAG_8583_AsBatchNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"凭证号:              ",TAG_8583_VoucherNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"授权号:                ",TAG_8583_AuthCode);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"参考号:          ",TAG_8583_SysRefNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	memset(buf,0,sizeof(buf));
	getTimeStr(buf);
	sprintf(cmd,"日期/时间:  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);

	result = ProviderGetTagValue(TAG_8583_AMOUNT, cmd);
	if(result > 0){
		sdkAscToBcd(buf, cmd, result);
		memset(cmd,0,128);
		strcpy(cmd,"金额(AMOUNT):   RMB  ");
		BcdAmount2AscAmount(buf, &cmd[strlen(cmd)]);
		strcat(cmd, "\n");
		writePrintText('c', cmd);
	}

	writePrintText('c',"-------------------------------------\n");
	writePrintText('c', "备注/REFERENCE    \n");
	writePrintText('c', "持卡人签名/CARD HOLDER SIGNATURE:  \n");
	writePrintText('c', "\n");
	writePrintText('c', "\n");
	writePrintText('c',"商户存根     CARDHOLDER COPY\n");


	writePauseTime(2);

	writePrintText('c',"商户名称：");

	memset(cmd,0,128);
	magdata(cmd,"",TAG_MerchantName);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"商户编号:            ",TAG_8583_MerchantNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"终端编号:               ",TAG_8583_TerminalNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"操作员号:                  ",TAG_8583_UsManNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
    magdata(cmd,"发卡行:               ",TAG_8583_IssuerBank);
    writePrintText('c',cmd);
    memset(cmd,0,128);
    magdata(cmd,"收单行:               ",TAG_8583_AcquirerBank);
    writePrintText('c',cmd);

	memset(cmd,0,sizeof(cmd));
	memset(buf,0, sizeof(buf));
	result = ProviderGetTagValue(TAG_8583_CARDNO, cmd);
	if(result > 0){
		CoverCardNo(cmd, buf);
		memset(cmd,0,sizeof(cmd));
	}
	sprintf(cmd,"卡号(CARD):  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	writePrintText('c',"交易类型(TRAN TYPE):          \n");
	memset(cmd,0,sizeof(cmd));
	memset(buf,0,sizeof(buf));
	getTradeTypeStr(buf, 0);
	sprintf(cmd, "      %s\n", buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"批次号:                ",TAG_8583_AsBatchNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
    magdata(cmd,"凭证号:              ",TAG_8583_VoucherNo);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"授权号:                ",TAG_8583_AuthCode);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"参考号:          ",TAG_8583_SysRefNo);
    	writePrintText('c',cmd);

	memset(cmd,0,128);
	memset(buf,0,sizeof(buf));
	getTimeStr(buf);
	sprintf(cmd,"日期/时间:  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	result = ProviderGetTagValue(TAG_8583_AMOUNT, cmd);
	if(result > 0){
		sdkAscToBcd(buf, cmd, result);
		memset(cmd,0,128);
		strcpy(cmd,"金额(AMOUNT):   RMB  ");
		BcdAmount2AscAmount(buf, &cmd[strlen(cmd)]);
		strcat(cmd, "\n");
		writePrintText('c', cmd);
	}


	writePrintText('c', "备注/REFERENCE    \n");
	writePrintText('c',"----------------------------------\n");
	writePrintText('c',"持卡人存根     CARDHOLDER COPY\n");



	result = writePrintScript();

	return result;
}

/*****************************************************************************
** Descriptions:  重打印 （持卡人联）
** Parameters:

** Returned value:

*****************************************************************************/
s32 reprintSignBill(){
	u8 buf[32] = {0};
	u8 cmd[128] = {0};
	u8 Amount[16] = {0};
	s32 result;

	initPrint();

	writePrintText('c',"商户名称：");
	memset(cmd,0,128);
	magdata(cmd,"",TAG_MerchantName);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"商户编号:            ",TAG_8583_MerchantNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"终端编号:               ",TAG_8583_TerminalNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"操作员号:                  ",TAG_8583_UsManNo);
	writePrintText('c',cmd);

    memset(cmd,0,128);
	magdata(cmd,"发卡行:               ",TAG_8583_IssuerBank);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"收单行:               ",TAG_8583_AcquirerBank);
	writePrintText('c',cmd);

	memset(cmd,0,sizeof(cmd));
	memset(buf,0, sizeof(buf));
	result = ProviderGetTagValue(TAG_8583_CARDNO, cmd);
	if(result > 0){
		CoverCardNo(cmd, buf);
		memset(cmd,0,sizeof(cmd));
	}
	sprintf(cmd,"卡号(CARD):  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	writePrintText('c',"交易类型(TRAN TYPE):          \n");

	memset(cmd,0,sizeof(cmd));
	memset(buf,0,sizeof(buf));
	getTradeTypeStr(buf, 1);
	sprintf(cmd, "      %s\n", buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"批次号:                ",TAG_8583_AsBatchNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
    magdata(cmd,"凭证号:              ",TAG_8583_OldVoucherNo);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"授权号:                ",TAG_8583_AuthCode);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"参考号:          ",TAG_8583_SysRefNo);
    writePrintText('c',cmd);

	memset(cmd,0,128);
	memset(buf,0,sizeof(buf));
	getTimeStr(buf);
	sprintf(cmd,"日期/时间:  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	result = ProviderGetTagValue(TAG_8583_AMOUNT, cmd);
	if(result > 0){
		sdkAscToBcd(buf, cmd, result);
		memset(cmd,0,128);
		strcpy(cmd,"金额(AMOUNT):   RMB  ");
		BcdAmount2AscAmount(buf, &cmd[strlen(cmd)]);
		strcat(cmd, "\n");
		writePrintText('c', cmd);
	}

	writePrintText('c',"-------------------------------------\n");
	writePrintText('c', "备注/REFERENCE    \n");
	writePrintText('c', "重打印凭证/DUPLICATED");
	writePrintText('c', "持卡人签名/CARD HOLDER SIGNATURE:  \n");
	writePrintText('c', "     \n");
	writePrintText('c', "     \n");
	writePrintText('c',"商户存根     CARDHOLDER COPY\n");


	writePauseTime(2);

	writePrintText('c',"商户名称：");

	memset(cmd,0,128);
	magdata(cmd,"",TAG_MerchantName);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"商户编号:            ",TAG_8583_MerchantNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"终端编号:               ",TAG_8583_TerminalNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"操作员号:                  ",TAG_8583_UsManNo);
	writePrintText('c',cmd);

	memset(cmd,0,128);
    magdata(cmd,"发卡行:               ",TAG_8583_IssuerBank);
    writePrintText('c',cmd);
    memset(cmd,0,128);
    magdata(cmd,"收单行:               ",TAG_8583_AcquirerBank);
    writePrintText('c',cmd);

	memset(cmd,0,sizeof(cmd));
	memset(buf,0, sizeof(buf));
	result = ProviderGetTagValue(TAG_8583_CARDNO, cmd);
	if(result > 0){
		CoverCardNo(cmd, buf);
		memset(cmd,0,sizeof(cmd));
	}
	sprintf(cmd,"卡号(CARD):  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	writePrintText('c',"交易类型(TRAN TYPE):          \n");

	memset(cmd,0,sizeof(cmd));
	memset(buf,0,sizeof(buf));
	getTradeTypeStr(buf, 1);
	sprintf(cmd, "      %s\n", buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	magdata(cmd,"批次号:                ",TAG_8583_AsBatchNo);
	writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"凭证号:              ",TAG_8583_OldVoucherNo);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"授权号:                ",TAG_8583_AuthCode);
    writePrintText('c',cmd);

    memset(cmd,0,128);
    magdata(cmd,"参考号:          ",TAG_8583_SysRefNo);
    writePrintText('c',cmd);

	memset(cmd,0,128);
	memset(buf,0,sizeof(buf));
	getTimeStr(buf);
	sprintf(cmd,"日期/时间:  %s",buf);
	writePrintText('c',cmd);

	memset(cmd,0,128);
	result = ProviderGetTagValue(TAG_8583_AMOUNT, cmd);
	if(result > 0){
		sdkAscToBcd(buf, cmd, result);
		memset(cmd,0,128);
		strcpy(cmd,"金额(AMOUNT):   RMB  ");
		BcdAmount2AscAmount(buf, &cmd[strlen(cmd)]);
		strcat(cmd, "\n");
		writePrintText('c', cmd);
	}

	writePrintText('c',"----------------------------------\n");
	writePrintText('c', "备注/REFERENCE    \n");
	writePrintText('c', "重打印凭证/DUPLICATED");
	writePrintText('c',"持卡人存根     CARDHOLDER COPY\n");



	result = writePrintScript();

}

/*****************************************************************************
** Descriptions:		结算单
** Parameters:

** Returned value:

*****************************************************************************/
s32 printSettleBill(){
	u8 buf[32] = {0};
	u8 cmd[128] = {0};
	u8 Amount[16] = {0};
	char asInDebitCardNum[10] = {0};
	char asInDebitCardAmount[20] = {0};
	char asOutDebitCardNum[10] = {0};
	char asOutDebitCardAmount[20] = {0};
	char asInCreditCardNum[10] = {0};
    char asInCreditCardAmount[20] = {0};
    char asOutCreditCardNum[10] = {0};
    char asOutCreditCardAmount[20] = {0};
	s32 result;

    ProviderGetTagValue(TAG_TOTAL_IN_DEBIT_NUM, asInDebitCardNum);
	ProviderGetTagValue(TAG_TOTAL_IN_DEBIT_AMOUNT, asInDebitCardAmount);
	ProviderGetTagValue(TAG_TOTAL_OUT_DEBIT_NUM, asOutDebitCardNum);
	ProviderGetTagValue(TAG_TOTAL_OUT_DEBIT_AMOUNT, asOutDebitCardAmount);
	ProviderGetTagValue(TAG_TOTAL_IN_CREDIT_NUM, asInCreditCardNum);
    ProviderGetTagValue(TAG_TOTAL_IN_CREDIT_AMOUNT, asInCreditCardAmount);
    ProviderGetTagValue(TAG_TOTAL_OUT_CREDIT_NUM, asOutCreditCardNum);
    ProviderGetTagValue(TAG_TOTAL_OUT_CREDIT_AMOUNT, asOutCreditCardAmount);


	memset(buf,0,sizeof(buf));
	BcdAmount2AscAmount(asInDebitCardAmount, buf);
	memset(asInDebitCardAmount,0,sizeof(asInDebitCardAmount));
	strcpy(asInDebitCardAmount, buf);

	memset(buf,0,sizeof(buf));
    BcdAmount2AscAmount(asOutDebitCardAmount, buf);
    memset(asOutDebitCardAmount,0,sizeof(asOutDebitCardAmount));
    strcpy(asOutDebitCardAmount, buf);

	memset(buf,0,sizeof(buf));
	BcdAmount2AscAmount(asInCreditCardAmount, buf);
	memset(asInCreditCardAmount,0,sizeof(asInCreditCardAmount));
	strcpy(asInCreditCardAmount, buf);

	memset(buf,0,sizeof(buf));
    BcdAmount2AscAmount(asOutCreditCardAmount, buf);
    memset(asOutCreditCardAmount,0,sizeof(asOutCreditCardAmount));
    strcpy(asOutCreditCardAmount, buf);


	initPrint();
	writePrintText('c',"                中国银联签购单\n");
    writePrintText('c',"    结算总计单/SETTLEMENT REPORT\n");
    writePrintText('c',"--------------------------------------\n");
	writePrintText('c',"商户名称：");

	memset(cmd,0,128);
	magdata(cmd,"",TAG_MerchantName);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"商户编号:            ",TAG_8583_MerchantNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"终端编号:               ",TAG_8583_TerminalNo);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	magdata(cmd,"操作员号:                  ",TAG_8583_UsManNo);
	writePrintText('c',cmd);

	writePrintText('c',"--------------------------------------\n");
	writePrintText('c',"类型/TYPE     笔数/SUM     金额/AMT\n");
	writePrintText('c',"--------------------------------------\n");
	writePrintText('c',"内卡\n");
	memset(cmd,0,128);
	sprintf(cmd,"借记        %s            %s",asInDebitCardNum, asInDebitCardAmount);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	sprintf(cmd,"借记        %s             %s",asInCreditCardNum, asInCreditCardAmount);
	writePrintText('c',cmd);

	writePrintText('c',"--------------------------------------\n");
	writePrintText('c',"外卡\n");
	memset(cmd,0,128);
	sprintf(cmd,"借记        %s             %s",asOutDebitCardNum, asOutDebitCardAmount);
	writePrintText('c',cmd);
	memset(cmd,0,128);
	sprintf(cmd,"贷记        %s             %s",asOutCreditCardNum, asOutCreditCardAmount);
	writePrintText('c',cmd);

    writePrintText('c',"--------------------------------------\n");
    memset(buf,0,sizeof(buf));
	result = ProviderGetTagValue(TAG_IN_CARD_CHECK, buf);
	if(result > 0){
	    result = atoi(buf);
	}

	if(result == SETTLE_EQU)
	{
	    writePrintText('c',"内卡对账平\n");
	}
	else if(result == SETTLE_NOTEQU)
	{
	     writePrintText('c',"内卡对账不平\n");
	}

	memset(buf,0,sizeof(buf));
    result =ProviderGetTagValue(TAG_OUT_CARD_CHECK, buf);
    if(result > 0){
        result = atoi(buf);
    }

    if(result == SETTLE_EQU)
    {
        writePrintText('c',"外卡对账平\n");
    }
    else if(result == SETTLE_NOTEQU)
    {
         writePrintText('c',"外卡对账不平\n");
    }


	writePrintText('c',"--------------------------------------\n");
    writePrintText('c',"商户存根     CARDHOLDER COPY\n");
	result = writePrintScript();

	return result;

}

/*****************************************************************************
** Descriptions:		明细单
** Parameters:

** Returned value:

*****************************************************************************/
s32 printDetailBill(){

	u8 buf[32] = {0};
	u8 cmd[128] = {0};
	u8 Amount[16] = {0};
	u8 cardNo[20] = {0};
	u8 type[4] = {0};
	s32 recordTotal = 0;
	s32 iRet, i;
	stRecordDetail *pstRecord;

	initPrint();
    writePrintText('c',"             交易明细单\n");
    writePrintText('c',"卡号                   类型    授权码\n");
    writePrintText('c',"凭证号                           金额\n");


	recordTotal = ProviderGetRecordTotal();
	pstRecord = (stRecordDetail*)malloc(recordTotal * sizeof(stRecordDetail));
	memset(pstRecord, 0x00, recordTotal * sizeof(stRecordDetail));
	iRet = getAllRecord(pstRecord, recordTotal, 0);

	Log("printDetailBill","111111111111111111111111111111111   recordTotal = %d 111111111111111111111111111111111", recordTotal);
	writePrintText('c',"  \n");
	for(i = 0; i < recordTotal; i++)
	{
	    memset(cmd, 0x00, sizeof(cmd));
	    memset(cardNo, 0x00, sizeof(cardNo));
	    CoverCardNo(pstRecord[i].CardNo, cardNo);

	    type[0] = getTypeCode(pstRecord[i].TradeId);
        sprintf(cmd, "%s   %s   %s\n", cardNo, type, pstRecord[i].AuthCode);
		writePrintText('c',cmd);

		memset(cmd, 0x00, sizeof(cmd));
		sprintf(cmd, "%s                    RMB %s\n", pstRecord[i].VoucherNo, pstRecord[i].Amount);
		writePrintText('c',cmd);
		writePrintText('c',"  \n");
	}

    Log("printDetailBill","111111111111111111111111111111111   recordTotal = %d 111111111111111111111111111111111", recordTotal);
    writePrintText('c',"商户存根     CARDHOLDER COPY\n");
	iRet = writePrintScript();

	free(pstRecord);
	Log("printDetailBill","22222222222222222222222222222222   recordTotal = %d 111111111111111111111111111111111", recordTotal);
	return iRet;

}
















