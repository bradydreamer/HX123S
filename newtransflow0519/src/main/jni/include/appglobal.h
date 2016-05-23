#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <android/log.h>
#include "devApi.h"
#include "sdkGlobal.h"

#include "tag.h"
#include "libs_tool.h"
#include "post8583.h"
#include "trade.h"
#include "print.h"
#include "signin.h"
#include "salesu.h"
#include "unsale.h"
#include "query.h"
#include "settle.h"
#include "refund.h"
#include "preauth.h"
#include "Complete.h"
#include "offcomplete.h"
#include "unpreauth.h"
#include "uncomplete.h"
#include "downloadPMK.h"
#include "switch.h"

//#include "Sdkiso8583.h"

struct _SYS gstSys;           //系统参数
struct _AppSetting gstAppSetting;    //pos参数
struct _TradeData gstTradeData; //交易处理数据
SDK_8583_ST8583 gstTempBag8583;  //内存暂存包

#define EncriptBagControl


#ifdef Trace
#undef Trace
#define Trace(tag, ...)  __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#endif
#define Log(tag, ...)  __android_log_print(ANDROID_LOG_DEBUG, tag, __VA_ARGS__)
#include "provider.h"

#ifndef SDK_TYPEDEF_H
#define SDK_TYPEDEF_H

//#ifndef XGD_SDK_DEBUG
//#define XGD_SDK_DEBUG

#ifndef __cplusplus

#undef true
#define true 1

#undef false
#define false 0


#define bool int
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0

#undef bool
//typedef   unsigned char bool;
//#define bool int

#endif  /* __cplusplus */


#undef TRUE
#undef FALSE

#undef BOOL
//typedef   unsigned int BOOL;
//#define bool int


//EC define, start
#define TAG_EC_BALANCE				0x9F79
#define TAG_EC_BALANCE_LIMIT		0x9F77
#define TAG_EC_ISSUER_AUTH_CODER	0x9F74
#define TAG_EC_SING_TRANS_LIMIT		0x9F78
#define TAG_EC_RESET_THRESHOLD 		0x9F6D
#define TAG_EC_BALANCE_FROM_GEN_AC	0x9F79

#define TAG_EC_TERMINAL_SUPPORT_IND	0x9F7A
#define TAG_EC_TERMINAL_TRANS_LIMIT	0x9F7B



#define BIT_0     (0x1ul << 0)
#define BIT_1     (0x1ul << 1)
#define BIT_2     (0x1ul << 2)
#define BIT_3     (0x1ul << 3)
#define BIT_4     (0x1ul << 4)
#define BIT_5     (0x1ul << 5)
#define BIT_6    (0x1ul << 6)
#define BIT_7     (0x1ul << 7)
#define BIT_8     (0x1ul << 8)
#define BIT_9     (0x1ul << 9)
#define BIT_10   (0x1ul << 10)
#define BIT_11     (0x1ul << 11)
#define BIT_12     (0x1ul << 12)
#define BIT_13     (0x1ul << 13)
#define BIT_14     (0x1ul << 14)
#define BIT_15     (0x1ul << 15)
#define BIT_16     (0x1ul << 16)
#define BIT_17     (0x1ul << 17)
#define BIT_18     (0x1ul << 18)
#define BIT_19     (0x1ul << 19)
#define BIT_20     (0x1ul << 20)
#define BIT_21     (0x1ul << 21)
#define BIT_22     (0x1ul << 22)
#define BIT_23     (0x1ul << 23)
#define BIT_24     (0x1ul << 24)
#define BIT_25     (0x1ul << 25)
#define BIT_26     (0x1ul << 26)
#define BIT_27     (0x1ul << 27)
#define BIT_28     (0x1ul << 28)
#define BIT_29     (0x1ul << 29)
#define BIT_30     (0x1ul << 30)
#define BIT_31     (0x1ul << 31)


#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

