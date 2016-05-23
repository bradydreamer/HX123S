#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef AFX_SDK_OPTION_H
#define AFX_SDK_OPTION_H

#ifdef __IAR_SYSTEMS_ICC__
#define _STDIO
#endif


//#include "common.h"
//#include "osApiDeclare.h"
#include "../src/devApi.h"

//通讯方式//communication mode
//#define FIFO_STX                                0x02                                             //FIFO数据同步头
//#define FIFO_SIZE                       1024                                                     //FIFO数据最大长度

#endif  //AFX_SDK_OPTION_H#

//蓝牙 usb 和 音频通信缓冲最大值
#define MAX_BUFFER_LENGTH                   512


//#define DEBUG_BT                            1//打开蓝牙的调试信息
//#define DEBUG_LCD                           1//打开LCD的调试信息
//#define DEBUG_MAG                           1//打开MAG的调试信息  
//#define DEBUG_ICC                           1//打开ICC的调试信息  
//#define DEBUG_COMM                          1//打开COMM的调试信息  
//#define DEBUG_DEBUG                         1//打开DEBUG的调试信息
//#define DEBUG_EMV                           1//打开EMV的调试信息
//#define DEBUG_FILE                          1//打开FILE的调试信息
//#define DEBUG_IME                           1//打开IME的调试信息
//#define DEBUG_KEY                           1//打开KEY的调试信息
//#define DEBUG_MATH                          1//打开MATH的调试信息
//#define DEBUG_MENU                          1//打开MENU的调试信息
//#define DEBUG_PED                           1//打开PED的调试信息
//#define DEBUG_PRINT                         1//打开PRINT的调试信息
//#define DEBUG_SYS                           1//打开SYS的调试信息
//#define DEBUG_AUDIO                         1//打开AUDIO的调试信息
//#define DEBUG_MESSAGE                       1//打开MESSAGE的调试信息
//#define DEBUG_MULTI_LANGUAGE                1//打开MULTI_LANGUAGE的调试信息
//#define DEBUG_PCI                           1//打开PCI的调试信息
//#define DEBUG_HOOK                          1//打开HOOK的调试信息
//#define DEBUG_TOOL                          1//打开TOOL的调试信息
//#define DEBUG_QUEUE                         1//打开QUEUE的调试信息
//#define DEBUG_LIST                          1//打开LIST的调试信息
//#define DEBUG_MEM                           1//打开MEM的调试信息



#define RF_INTERFRENCE_MAG                    -60000


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

