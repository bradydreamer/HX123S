#ifndef HX_DEBUG_H
#define HX_DEBUG_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#if(MACHINEMODEL == SDK_SYS_MACHINE_K508A)  //使用iar编译报没有声明的警告。只能先在这里添加声明了。

extern int sprintf(char *_Restrict, const char *_Restrict, ...);

extern int vprintf(const char *_Restrict, __Va_list);

extern int vsprintf(char *_Restrict, const char *_Restrict, __Va_list);

#endif


//调试信息输出模式//output debug message mode
enum SDK_DEBUG_OUT_TPYE {
    SDK_DEUBG_OUT_ALL = 0,                      //输出全部//output all
    SDK_DEUBG_BLOCK_ALL,                         //全部阻止//all block
    SDK_DEUBG_OUT_WHITE_LIST,                    //只出白名单//only output white list
    SDK_DEUBG_BLOCK_BLACK_LIST                   //只阻止黑名单//only block black list
};

//调试信息输出目标//target for outputting debug message 
enum SDK_DEBUG_OUT_TARGET {
    SDK_DEBUG_TARGET_SCREEN = 0,               //屏上输出//output on the screen
    SDK_DEBUG_TARGET_COM,                        //串口输出//port output
    SDK_DEBUG_TARGET_RPINT,                      //打印输出//print output
    SDK_DEBUG_TARGET_FILE,                      //文件输出//print output

    SDK_DEUBG_ERR                                //用于查错//used for checking error
};

//调试信息过滤器
typedef struct {
    bool bIsInTest;                              //是否在测试状太下，
    //如果在测试状下Assert不会断下来，//whether in the testing state, Assert won't break if in the testing state
    //只是输出一个断言信息后继续向后走//just output a assertion information then keep going
    bool bIsTraceFunName;                        //已经废弃不再使用
    enum SDK_DEBUG_OUT_TARGET eTarget;           //输出目标//output target
    enum SDK_DEBUG_OUT_TPYE eOutType;            //输出种类//output type
} SDK_DEBUG_FILTER;


extern void sdkFinalErr(u8 const
* pasMsn,
void const *pasFile,
unsigned int uiLine
);    // 致命错误//fatal error

#define FinalErr(Msn)   (sdkFinalErr((u8 const*)Msn, (u8 const*)__FILE__, __LINE__))


extern s32 sdkDebugSetFilter(SDK_DEBUG_FILTER const *pstFilter);

extern s32 sdkDebugInitalList(char const **pasWhiteList, u32 uiWhiteListLen,
                              char const **ppasBlackList, s32 siBlackListLen);

// 此函数不可以直接调用//this function can't be called directly
extern void sdkDebugAssert(const u8 *pasFile, u8 const *pasFun, int line);

// 此函数不可以直接调用//this function can't be called directly
s32 sdkDebugTraceHex(u8 const
* pasTag,
u8 const *pasInfo,
void const *pheDate, s32
siLen,
u8 const *pasfun, s32
line);

// 此函数不可以直接调用//this function can't be called directly
extern void SdkDebugTrace(u8 const
*pasTag,
u8 const *pasFile, u8 const
*pFun,
int siLine,
char const *pheFmt, ...
);

// 此函数不可以直接调用//this function can't be called directly
extern s32 sdkDebugOut(u8 const
*pasStr);

//调试信息管理函数
extern s32 sdkDebugControl(bool bIsSaveScreen);

#ifdef XGD_SDK_DEBUG
#ifdef __cplusplus
#define Trace(ptag, arg...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)__FUNCTION__, __LINE__, ##arg)
#else
	#if(MACHINEMODEL == SDK_SYS_MACHINE_K501AM)
		#define Trace(ptag,...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)"char", __LINE__, ##__VA_ARGS__)
    #else
        #define Trace(ptag,  ...)  SdkDebugTrace((u8 const*)ptag, (u8 const*)__FILE__, (u8 const*)__FUNCTION__, __LINE__, __VA_ARGS__)
	#endif
#endif



#if(MACHINEMODEL == SDK_SYS_MACHINE_K501AM)
    #define  TraceHex(pasTag, pasInfo, pheDate, uiLen)   sdkDebugTraceHex((u8 const*)pasTag, (u8 const*)pasInfo, pheDate, uiLen,(u8 const*)"fuction",__LINE__)
    #define Assert(exp) ((void)((exp) || (sdkDebugAssert((u8 const*)__FILE__,(u8 const*)"fuction",__LINE__), 0)))
#else
    #define  TraceHex(pasTag, pasInfo, pheDate, uiLen)   sdkDebugTraceHex((u8 const*)pasTag, (u8 const*)pasInfo, pheDate, uiLen,(u8 const*)__FUNCTION__,__LINE__)
    #define Assert(exp) ((void)((exp) || (sdkDebugAssert((u8 const*)__FILE__,(u8 const*)__FUNCTION__,__LINE__), 0)))
#endif
#define Verify(exp)   (Assert(exp))


#else
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
#define Assert(exp)
#define Verify(exp)    ((void)(exp))
extern s32 sdkDebugOut(u8 const
*pasStr);
#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif

