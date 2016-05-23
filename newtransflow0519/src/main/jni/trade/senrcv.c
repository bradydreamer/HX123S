#include "appglobal.h"

//#ifdef XGD_SDK_DEBUG
////白名单//white list
//static char const   *pWhiteList[] =
//{
//		"zbq",
//    "lilin",
//    "huangxin",
//    "dw",
//    "hbx",
////	"sdkExtLib",
////    "UnzipCapkPagData",
//    "VerifyCAPK",
//	"lhd_debug",
////    "CommConfig",
////    "LBS",
////    "Send8583"
////    "DecryptWK"
////    "DecryptWK-TPK"
////    "DecryptWK-TAK"
////    "NotifyCapkTrade"
////    "DealNotifyCapk"
////       "fifosws",
//    "CheckRecBag",
////        "fifosws",
//};
//
//
////黑名单//black list
//static char const   *pBlackst[] =
//{
//
//    "sdkmmi",
////    "sdk8583ReplaceField",
////    "sdk8583SetField",
////    "sdk8583ParseField",
////    "sdk8583GetField",
////    "sdk8583Parse8583",
////    "ReleasePrintData",
////    "InsertPrintData",
////    "UPrint_Start",
//    "fsp",
//    "Print",
//    "szg",
//    "hbx",
//};
//
//static const SDK_DEBUG_FILTER DebugFilter =
//{
//    false,                            //是否在测试状态下//whether in testing state
//    false,                             // 是否输出函数名//whether output function name
//    SDK_DEBUG_TARGET_COM,             // 调试信息输出到串口//debug massage output to serial port
//   SDK_DEUBG_OUT_WHITE_LIST,       //trace过滤模式//trace filtering mode
////  SDK_DEUBG_BLOCK_BLACK_LIST,
//};
//#endif


int Package(char *indata,int len,char *outdata,int *rlen)
{
    s32 slt = 0;
    u8 utrade[256] = {0};
    
    if(ProviderGetTagValue(TAG_TRADE_TYPE, utrade))    //Fix me 2
    {
        Log("wangb", "Package is %s",utrade);
    }
    else
    {
         return SDK_ERR;
    }

//#ifdef XGD_SDK_DEBUG
//    Verify(sdkDebugInitalList(pWhiteList, sizeof(pWhiteList) / sizeof(char *), pBlackst, sizeof(pBlackst) / sizeof(char *)) == SDK_OK);
//    Verify(sdkDebugSetFilter(&DebugFilter) == SDK_OK);
//#endif

    if(strcmp(utrade,TAG_TRADEID_DownLoadPMK) == 0)    
    {
       slt = LoadPMKStart(indata,len,outdata,rlen);
    }
    else if(strcmp(utrade,TAG_TRADEID_Locate) == 0)
	{
	   slt = Locationstart(indata,len,outdata,rlen);
	}
    else if(strcmp(utrade,TAG_TRADEID_SIGNIN) == 0)
    {
       slt = SignInstart(indata,len,outdata,rlen);
    }
    else if(strcmp(utrade,TAG_TRADEID_SALE) == 0)
    {
       slt = Salestart(indata,len,outdata,rlen);
    }
//    else  if(strcmp(utrade,TAG_TRADEID_UNSALE) ==0)
//    {
//       slt = UnSalestart(indata,len,outdata,rlen);
//
//    }
//    else if(strcmp(utrade,TAG_TRADEID_QUERY) == 0)
//    {
//        slt = Querystart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_SETTLE) == 0)
//    {
//        slt = Settlestart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_REFUND) == 0)
//    {
//        slt = Refundstart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_PREAUTH) == 0)
//    {
//        slt = Preauthstart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_COMPLETE) == 0)
//    {
//        slt = Completestart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_OFFCOMPLETE) == 0)
//    {
//        slt = Offcompletestart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_UNPREAUTH) == 0)
//    {
//        slt = Unpreauthstart(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_UNCOMPLETE) == 0)
//    {
//        slt = UnCompletestart(indata,len,outdata,rlen);
//    }

    Log("wangb","len3 is %d ",slt);
    return slt;

}

int UnPackage(char *indata,int len,char *outdata,int *rlen)
{
    s32 slt = 0;
    u8 utrade[256] = {0};
    
    if(ProviderGetTagValue(TAG_TRADE_TYPE, utrade))
    {
        Log("wangb", "UnPackage is %s",utrade);
    }
    else
    {
         return SDK_ERR;
    }

    if(strcmp(utrade,TAG_TRADEID_DownLoadPMK) == 0)
    {
        slt = LoadPMKParse8583(indata,len,outdata,rlen);
    }
    else if(strcmp(utrade,TAG_TRADEID_Locate) == 0)
	{
	   slt = LocationParse8583(indata,len,outdata,rlen);
	}
    else if(strcmp(utrade,TAG_TRADEID_SIGNIN) == 0)
    {
        slt = SignParse8583(indata,len,outdata,rlen);
    }
    else if(strcmp(utrade,TAG_TRADEID_SALE) == 0)
    {
       slt = SaleParse8583(indata,len,outdata,rlen);
    }
//    else  if(strcmp(utrade,TAG_TRADEID_UNSALE) == 0)
//    {
//       slt = UnSaleParse8583(indata,len,outdata,rlen);
//
//    }
//    else if(strcmp(utrade,TAG_TRADEID_QUERY) == 0)
//    {
//        slt = QueryParse8583(indata,len,outdata,rlen);
//    }
//    if(strcmp(utrade,TAG_TRADEID_SETTLE) == 0)
//    {
//       slt = SettleParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_REFUND) == 0)
//    {
//        slt = RefundParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_PREAUTH) == 0)
//    {
//        slt = PreauthParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_COMPLETE) == 0)
//    {
//        slt = CompleteParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_OFFCOMPLETE) == 0)
//    {
//        slt = OffcompleteParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_UNPREAUTH) == 0)
//    {
//        slt = UnpreauthParse8583(indata,len,outdata,rlen);
//    }
//    else if(strcmp(utrade,TAG_TRADEID_UNCOMPLETE) == 0)
//    {
//        slt = UnCompleteParse8583(indata,len,outdata,rlen);
//    }

    Log("wangb", "UnPackage out is %d",slt);
    return slt;

}


