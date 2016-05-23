#include "sdkGlobal.h"
#include <stdarg.h>

#ifndef  DEBUG_DEBUG
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define SDK_DEBUG_TAG_LEN    (15 + 1)

#define SDK_DEBUG_FILE_MAXSIZE      20480       //文件可允许大小20K

static s32 sdkDebugCreatDebugFile(const u8 *pasfile);


static SDK_SYS_LIST_HAND mWhiteList = NULL;     // 白名单
static SDK_SYS_LIST_HAND mBlackList = NULL;     // 黑名单



typedef struct {
    u32 cmd1;
    u32 cmd2;
} SDK_PAIR;

static s32 sdkDebugFormFileName(u8 *pDebugFile) {
    strcpy(pDebugFile, "debug.txt");

    return strlen(pDebugFile);
}

static s32 sdkDebugFindPair(const SDK_PAIR *pstArray, s32 size, s32 cmd) {
    while (size-- > 0) {
        if (pstArray[size].cmd1 == (u32) cmd) {
            return pstArray[size].cmd2;
        }
    }

    Assert(0);
    return SDK_ERR;
}

static SDK_DEBUG_FILTER mDebugFilter =
        {
                false,                                        //非测试状态
                false,
                SDK_DEBUG_TARGET_COM,                         // 输出到屏上
                SDK_DEUBG_OUT_ALL,                            // 全部tag都打印
        };

/*****************************************************************************
** Descriptions:	设置调试过滤器
** Parameters:          SDK_DEBUG_FILTER const * pstFilter
** Returned value:
** Created By:		shiweisong  2013.11.08
** Remarks:
*****************************************************************************/
s32 sdkDebugSetFilter(SDK_DEBUG_FILTER const *pstFilter) {
    if (pstFilter != NULL) {
        {
            if (pstFilter->eTarget < SDK_DEUBG_ERR) {
                if (pstFilter->eOutType <= SDK_DEUBG_BLOCK_BLACK_LIST) {
                    memcpy(&mDebugFilter, pstFilter, sizeof(*pstFilter));
                    return SDK_OK;
                }
            }
        }
    }
    Assert(0);
    return SDK_PARA_ERR;
}

static s32 sdkDebugSysGetFileAllVer(const u8 *pFileName, u8 *lpOutVer) {
    u8 *fileTag[] =
            {
                    "OutVer",
                    "InnerVer",
                    "AppID",
                    "AppName",
                    "FileStyle",
                    "FileRoot",
                    "Company",
                    "SysEditDate",
                    "RelyFile",
                    "UserInfor",
                    "ExOption",
                    "Platform",
                    "IsBaseSDK",
            };
    u8 tem[256] = {0};
    s32 fileTagConter = sizeof(fileTag) / sizeof(u8 * );

    lpOutVer[0] = 0;     //防错

    while (fileTagConter--) {
        memset(tem, 0, sizeof(tem));
        strcpy(tem, fileTag[fileTagConter]);
        strcat(tem, ":");
        sdkSysGetFileVer(pFileName, fileTag[fileTagConter], &tem[strlen(tem)]);
        strcat(tem, "\n");
        strcat(lpOutVer, tem);
    }

    return strlen(lpOutVer);
}

/*****************************************************************************
** Descriptions:文件超过一定大小后删除
** Parameters:          Input: None
** Returned value:
** Created By:		zhongjiahong  2013.11.08
** Remarks:
*****************************************************************************/
static s32 DeleteLargeFile(const u8 *pFile) {
    s32 FileSize;

    if (NULL == pFile) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    FileSize = sdkGetFileSize(pFile);        //获取文件大小  //Get the size of the file

    if (FileSize > SDK_DEBUG_FILE_MAXSIZE) {
        sdkDelFile(pFile);
        return SDK_OK;
    }
    return SDK_ERR;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: IsInBackList
   函数功能: 黑白盒初始化
   输入参数: ppasWhiteList，白名单数组
             uiWhiteListLen，白名单点个数
             ppasBlackList，黑名单数组
             siBlackListLen，黑名单节点个数
   输出参数:
   返 回 值:
   修改备注:
   日期时间: 2012.05.14 14:57:09
*******************************************************************/
s32 sdkDebugInitalList(char const **ppasWhiteList, u32 uiWhiteListLen, char const **ppasBlackList,
                       s32 siBlackListLen) {
    if (mWhiteList == NULL) {
        mWhiteList = sdkSysListCreate();
    }
    Assert(mWhiteList != NULL);

    if (ppasWhiteList != NULL && uiWhiteListLen > 0) {
        while (uiWhiteListLen-- > 0) {
            void *p = (void *) sdkGetMem(SDK_DEBUG_TAG_LEN);

//            if(strlen(ppasWhiteList[uiWhiteListLen]) > SDK_DEBUG_TAG_LEN )
//            {
//                xgd_free(p);
//                return SDK_PARA_ERR;
//            }
            if (NULL == p) {
                Assert(0);
                return SDK_ERR;
            }
            memset(p, 0, SDK_DEBUG_TAG_LEN);
            strncpy(p, ppasWhiteList[uiWhiteListLen], SDK_DEBUG_TAG_LEN);
            Verify(sdkSysListAddAtFront(mWhiteList, p) >= 0);
        } //lint !e429
    }

    if (mBlackList == NULL) {
        mBlackList = sdkSysListCreate();
    }
    Assert(mBlackList != NULL);

    if (ppasBlackList != NULL && siBlackListLen > 0) {
        while (siBlackListLen-- > 0) {
            void *p = (void *) sdkGetMem(SDK_DEBUG_TAG_LEN);

//            if(strlen(ppasBlackList[siBlackListLen]) > SDK_DEBUG_TAG_LEN )
//            {
//                xgd_free(p);
//                return SDK_PARA_ERR;
//            }
            if (NULL == p) {
                Assert(0);
                return SDK_ERR;
            }
            memset(p, 0, SDK_DEBUG_TAG_LEN);
            strncpy(p, ppasBlackList[siBlackListLen], SDK_DEBUG_TAG_LEN);
            Verify(sdkSysListAddAtFront(mBlackList, p) >= 0);
        } //lint !e429
    }
    return SDK_OK;
} //lint !e818

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: IsInBackList
   函数功能: 检查是否在黑名单之中
   输入参数: pBacKList
   输出参数:
   返 回 值:
   修改备注:
   日期时间: 2012.05.14 14:57:09
*******************************************************************/
static bool IsInBlackList(u8 const *pasTag) {
    u32 len = strlen(pasTag);
    u32 listlen;
    u8 *pData = NULL;
    int conter = sdkSysListGetCnt(mBlackList);

    len = (len == 0) ? 1 : len;
    len = len < SDK_DEBUG_TAG_LEN ? len : SDK_DEBUG_TAG_LEN;

    while (conter-- > 0) {
        pData = sdkSysListGetAt(mBlackList, conter);                     //取出节点

        listlen = strlen(pData) < SDK_DEBUG_TAG_LEN ? strlen(pData) : SDK_DEBUG_TAG_LEN;

        if (listlen == len)                 //zhongjiahong 2013.12.19 15:30,长度一样才去比较
        {
            if (memcmp(pData, pasTag, len) == 0) {
                return true;
            }
        }
    }

    return false;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: IsInWhiteList
   函数功能: 查找是否是在白名单内
   输入参数:
   输出参数:
   返 回 值:
   修改备注:
   日期时间: 2012.05.14 15:05:57
*******************************************************************/
static bool IsInWhiteList(u8 const *pasTag) {
    u32 len = strlen(pasTag);
    u32 listlen;
    u8 *pData = NULL;
    int conter = sdkSysListGetCnt(mWhiteList);

    len = (len == 0) ? 1 : len;
    len = len < SDK_DEBUG_TAG_LEN ? len : SDK_DEBUG_TAG_LEN;

    while (conter-- > 0) {
        pData = sdkSysListGetAt(mWhiteList, conter);                      //取出节点

        listlen = strlen(pData) < SDK_DEBUG_TAG_LEN ? strlen(pData) : SDK_DEBUG_TAG_LEN;

        if (listlen == len)          //zhongjiahong 2013.12.19 15:30,长度一样才去比较
        {
            if (memcmp(pData, pasTag, len) == 0) {
                return true;
            }
        }
    }

    return false;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: sdkIsNeedOut
   函数功能: 根据配置查找相应的tag是否要输出
   输入参数: ptag
   日期时间: 2012.05.16 14:09:03
*******************************************************************/
bool sdkIsNeedOut(u8 const *pasTag) //lint !e765
{
    if (mDebugFilter.eOutType == SDK_DEUBG_BLOCK_ALL) {
        return false;
    }

    if (mDebugFilter.eOutType == SDK_DEUBG_OUT_ALL) {
        return true;
    }
    else if (mDebugFilter.eOutType == SDK_DEUBG_OUT_WHITE_LIST) {
        return IsInWhiteList(pasTag);
    }
    else if (mDebugFilter.eOutType == SDK_DEUBG_BLOCK_BLACK_LIST) {
        return !IsInBlackList(pasTag);
    }
    Assert(0);   // 是不应当来这里的
    return true;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: sdkDebugGetTarget
   函数功能: 得到调试作息输了目标
   返 回 值:
             SDK_DEBUG_TARGET_SCREEN  =  0 , // 屏上输出
             SDK_DEBUG_TARGET_COM ,          // 串口输出
             SDK_DEBUG_TARGET_RPINT          // 打印输出
   修改备注:
   日期时间: 2012.05.19 17:14:50
*******************************************************************/
static enum SDK_DEBUG_OUT_TARGET sdkDebugGetTarget() {
    Assert(SDK_DEUBG_ERR > mDebugFilter.eTarget);
    return mDebugFilter.eTarget;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石玮松
   函数名称: sdkFinalErr
   函数功能: 发生致命错误时调用这函数
   返 回 值:
   修改备注:
   日期时间: 2012.05.22 14:21:30
*******************************************************************/
void sdkFinalErr(u8 const *pasMsn, void const *pasFile, unsigned int uiLine) {
    char const *pfile = pasFile;
    u8 temp[256] = {0};

    if (strlen(pfile) > 13) {
        pfile = &pfile[strlen(pfile) - 13];
    }
    sprintf(temp, "%s \r%s \rline :%d\r请重启机器", pfile, pasMsn, (s32) uiLine);


    sdkDispMsgBox("致命错误", temp, 0, SDK_KEY_MASK_9);
}

/*******************************************************************
   版    权: 新国都
   作    者: 石伟松
   函数名称: sdkDebugTraceHex
   函数功能: 输出16制数替代PrintFormat
   输入参数:
   输出参数:
   返 回 值:
   修改备注:
   日期时间: 2012.05.16 14:39:51
*******************************************************************/
s32 sdkDebugTraceHex(u8 const *pasTag, u8 const *pasInfo, void const *pheDate, s32 siLen,
                     u8 const *pasfun, s32 line) //lint !e765
{
    u8 const *src = (u8 const *) pheDate;

    if (pasTag == NULL || pasInfo == NULL || pheDate == NULL || siLen < 0) //lilin 20120818
    {
        Assert(0);
        //printf("pasTag :%s pasInfo: %s pheDate:%s siLen:%d  pasfun:%s  line:%d \r\n", pasTag, pasInfo, (char *)pheDate, siLen, pasfun, line);
        return SDK_PARA_ERR;
    }

    /*=======BEGIN: lilin20120818 modify===========*/
    if (siLen == 0) {
        return SDK_OK;
    }
    /*================ END================== */

    if (sdkIsNeedOut(pasTag) == true) {
        s32 i;
        u8 temp[128 + 50], buf[50];
        s32 len;
        memset(temp, 0, sizeof(temp));
        len = strlen(pasTag);
        (len > 10) ? pasTag = &pasTag[len - 10] : 0;
        len = strlen(pasInfo);
        (len > 30) ? pasInfo = &pasInfo[len - 30] : 0;     //一次打出防止太长打出来出问题

        Verify(sprintf((char *) temp, "\r\n Fun:%s LINE:%d  \r\n", pasfun, line) < (s32)
        sizeof(temp));

        Verify(sdkDebugOut((u8 *) temp) == SDK_OK);

        memset(temp, 0, sizeof(temp));

        Verify((u32) sprintf((char *) temp, "\r\n TAG:%s  INFO:%s LEN:%d  \r\n", pasTag, pasInfo,
                             siLen) < sizeof(temp));

        Verify(sdkDebugOut((u8 *) temp) == SDK_OK);
        memset(temp, 0, sizeof(temp));


        for (i = 0; i < siLen; i++) {
            if (i % 16 == 0) {
                strcat(temp, "\r\n");

                if (strlen(temp) >= sizeof(temp) - 50) {
                    Verify(sdkDebugOut((u8 *) temp) == SDK_OK);
                    memset(temp, 0, sizeof(temp));
                }
            }
            memset(buf, 0, sizeof(buf));
            sprintf((char *) buf, "%02X ", src[i]);
            strcat(temp, buf);
        }

        strcat(temp, "\r\n");
        sdkDebugOut(temp);
    }
    return SDK_OK;
}

/*inline*/ static bool IsInDebug() {
    return mDebugFilter.bIsInTest == false;
}

/*******************************************************************
   版    权: 新国都
   作    者: 石伟松
   函数名称: sdkDebugAssert
   函数功能: 输出调试信息
   输入参数:
   输出参数:
   返 回 值:
   修改备注:
   日期时间: 2012.05.16 14:39:51
*******************************************************************/
void sdkDebugAssert(const u8 *pasFile, u8 const *pasFun, int line) {
    u8 temp[256];
    int len = strlen((char *) pasFile);

    len > 13 ? pasFile = &pasFile[len - 13] : 0;
    memset(temp, 0, sizeof(temp));

    sprintf((char *) temp, "assert failed: %s,line: %d FUN:%s \r\n ", (char *) pasFile, line,
            (char *) pasFun);

    sdkDebugOut(temp);

/* duxd2015
    while (IsInDebug())
    {
        if(sdkKbGetKey() == SDK_KEY_ESC)
        {
            break;
        }
    }
*/
}

/*****************************************************************************
   版    权: 新国都
   函数名称: sdkDebugOut
   参    数: u8 const *pasStr
   返 回 值:
   说    明: 输出调试信息

   创 建 者: 石玮松2012.07.25
   注    意:
*****************************************************************************/
s32 sdkDebugOut(u8 const *pasStr) {
    int size;

    if (pasStr == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkDebugGetTarget() == SDK_DEBUG_TARGET_COM) {
        ddi_sys_printf((s8 *) pasStr);
    }
    else if (sdkDebugGetTarget() == SDK_DEBUG_TARGET_FILE) {
        char FileName[50], FilePath[60];
        size = sdkDebugFormFileName(FileName);         //zhongjiahong 2013.11.08 10:3

        if (size < 0) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        sprintf(FilePath, "/mtd0/dll/%s", FileName);

        DeleteLargeFile(FilePath);                    //文件超过一定大小后删除//zhongjiahong 2013.11.08 14:54

        if (sdkAccessFile(FilePath) == false) //如果文件不存在就创建一个带库版本的文件
        {
            sdkDebugCreatDebugFile(FilePath);
        }
        sdkAppendFile(FilePath, pasStr, strlen(pasStr));
    }
    else if (sdkDebugGetTarget() == SDK_DEBUG_TARGET_SCREEN) {
        sdkDispMsgBox("调试信息", pasStr, 1000, SDK_KEY_MASK_ESC);
        return SDK_OK;
    }
    else {
        FinalErr("不支持的输出方式");
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
   版    权: 新国都
   函数名称: SdkDebugTrace
   参    数: u8 const *pasTag   :Tag过滤器
             u8 const*  pasFile :调试信息的文件归属
             u8 const *pasFun   :调试信息的函数归属
             int line           :调试信息的行数归属
             char const *pheFmt :其他的参数
   返 回 值:
   说    明: 输出调试信息

   创 建 者: 石玮松2012.07.25
   注    意:
*****************************************************************************/
void SdkDebugTrace(u8 const *pasTag, u8 const *pasFile, u8 const *pasFun, int line,
                   char const *pheFmt, ...) {
                   
    u8 *temp;
    int len = strlen((char *) pasFile);

    len > 13 ? pasFile = &pasFile[len - 13] : 0;
    len = strlen((char *) pasFun);
    len > 25 ? pasFun = &pasFun[len - 25] : 0;

    temp = (u8 *) sdkGetMem(1024 + 128);

    if (NULL == temp) {
        return;
    }
    memset(temp, 0, 1024 + 128);

    if (sdkIsNeedOut(pasTag) == true) {
        va_list varg;
        sprintf((char *) temp, "TAG:%s file:%s FUN:%s  LINE:%d  \r\n", pasTag, pasFile, pasFun,
                line);
        va_start(varg, pheFmt); //lint !e530  转换不定参数
        vsprintf(&temp[strlen(temp)], pheFmt, varg);
        sdkDebugOut(temp);
        va_end(varg);
    }

    if (NULL != temp) {
        sdkFreeMem(temp);
    }
    
}

/*****************************************************************************
** Descriptions:	删除log文件
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.10.29
** Remarks:
*****************************************************************************/
static s32 sdkDebugDelLogFile(void) {
    char FileName[50], FilePath[60];
    s32 size;

    size = sdkDebugFormFileName(FileName);         //zhongjiahong 2013.11.08 10:3

    if (size < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sprintf(FilePath, "/mtd0/dll/%s", FileName);

    sdkDispMsgBox("调试文件管理", sdkDelFile(FilePath) >= 0 ? "删除文件成功" : "删除文件失败", 2000,
                  SDK_KEY_MASK_ESC);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	把文件拷贝到Upan
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.10.29
** Remarks:
*****************************************************************************/
static s32 sdkDebugCopLogToUpan(void) {
    s32 pRetCode;
    s32 ret;
    char FileName[50], Cmd[60];
    s32 size;

    size = sdkDebugFormFileName(FileName);         //zhongjiahong 2013.11.08 10:3

    if (size < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sprintf(Cmd, "cp /mtd0/dll/%s /mnt/usb/", FileName);
    ret = sdkSysCmd(Cmd, &pRetCode);
    sdkSysCmd("sync", &pRetCode);
    sdkDispMsgBox("调试文件管理", ret == SDK_SYS_SHELL_OK ? "copy OK" : "copy err", 1000,
                  SDK_KEY_MASK_ESC);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	打印调试文件时字体的选择
** Parameters:          void
** Returned value:
** Created By:		zhongjiahong  2013.11.01
** Remarks:
*****************************************************************************/
static SDK_PRINT_FONT sdkGetPrintFont(s32 size) {
    SDK_PRINT_FONT font[] =
            {
                    {0, SDK_PRN_ASCII12X24,   SDK_PRN_ZOOM_N, SDK_PRN_CH12X12, SDK_PRN_ZOOM_N, 0},
                    {0, SDK_PRN_ASCII16X48,   SDK_PRN_ZOOM_N, SDK_PRN_CH32X32, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X48B,  SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X48Y,  SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X48YB, SDK_PRN_ZOOM_N, SDK_PRN_CH24X24, SDK_PRN_ZOOM_A, 0},
                    {0, SDK_PRN_ASCII16X16B,  SDK_PRN_ZOOM_N, SDK_PRN_CH32X32, SDK_PRN_ZOOM_A, 0},
            };

    size = ((u32) size > (sizeof(font) / sizeof(SDK_PRINT_FONT) - 1)) ? (
            sizeof(font) / sizeof(SDK_PRINT_FONT) - 1) : (u32) size;
    return font[size];
}

/*****************************************************************************
** Descriptions:	读取并打印调试文件
** Parameters:          u8 const *pasFile
** Returned value:
** Created By:		zhongjiahong  2013.11.07
** Remarks:
*****************************************************************************/
static s32 sdkFilePrinting(u8 const *pasFile) {
    u8 *buf;
    s32 pos, Len, FileSize;

    if (NULL == pasFile) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    FileSize = sdkGetFileSize(pasFile);        //获取文件大小  //Get the size of the file

    if (0 >= FileSize) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    buf = (u8 *) sdkGetMem(2048);

    if (NULL == buf) {
        Assert(0);
        return SDK_ERR;
    }
    memset(buf, 0, 2048);

    pos = 0;
    Len = 2048 + 1 - 1;

    if (FileSize < Len) {
        Len = FileSize;
    }
    sdkPrintInit();

    while (0 < FileSize) {
        sdkReadFile(pasFile, buf, pos, &Len);

        sdkPrintStr(buf, sdkGetPrintFont(0), SDK_PRINT_LEFTALIGN, 0, 5);
        sdkPrintStart();                            //start printing

        memset(buf, 0, 2048 + 1);

        pos += Len;
        FileSize -= Len;

        if (FileSize < Len) {
            Len = FileSize;
        }
    }

    if (NULL != buf) {
        sdkFreeMem(buf);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	打印调试文件时的相关显示
** Parameters:          void
** Returned value:
** Created By:		zhongjiahong  2013.11.07
** Remarks:
*****************************************************************************/
static s32 sdkDebugPrintLog(void) {
    s32 rslt;
    char FileName[50], FilePath[60];
    s32 size;

    size = sdkDebugFormFileName(FileName);         //zhongjiahong 2013.11.08 10:3

    if (size < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sprintf(FilePath, "/mtd0/dll/%s", FileName);

    if (sdkAccessFile(FilePath) == false) {
        sdkDispMsgBox("调试文件管理", "File Not Exist!", 3000, SDK_KEY_MASK_ESC);
        return SDK_ERR;
    }
    else {
        s32 temp = mDebugFilter.eOutType;
        sdkDispMsgBox("调试文件管理", "正在打印......", 50, SDK_KEY_MASK_ENTER);

        mDebugFilter.eOutType = SDK_DEUBG_BLOCK_ALL;          //防止读文件打印的过程中调试信息写入文件

        rslt = sdkFilePrinting(
                FilePath);                        //打印文件内容  //printing the content of file

        mDebugFilter.eOutType = temp;            //恢复输出模式  //Recover output mode

        if (rslt == SDK_PARA_ERR) {
            sdkDispMsgBox("调试文件管理", "文件有误,请确认退出", 0, SDK_KEY_MASK_ENTER);
        }
        sdkDispMsgBox("调试文件管理", "打印完毕,请确认退出", 0, SDK_KEY_MASK_ENTER);

        return SDK_OK;
    }
}

/*****************************************************************************
** Descriptions:	组织名单列表
** Parameters:          void
** Returned value:
** Created By:		zhongjiahong  2013.11.04
** Remarks:
*****************************************************************************/
static s32 sdkDisplayList(const u8 *pTitle, SDK_SYS_LIST_HAND hand) {
    int rtn;
    char *ptem;
    int ii;
    int conter = sdkSysListGetCnt(hand);
    u8 *pData = NULL;

    ptem = (char *) sdkGetMem(2048);

    if (NULL == ptem) {
        return SDK_ERR;
    }
    memset(ptem, 0, 2048);

    for (ii = 0; ii < conter; ii++) {
        pData = sdkSysListGetAt(hand, ii);
        sprintf(&ptem[strlen(ptem)], "%d.%s\r", ii, pData);

        if (strlen(ptem) > 2048) {
            while (1) {
                sdkDispMsgBox("测试信息", "致命错误", 0, SDK_KEY_MASK_ALL);
            }
        }
    }

    rtn = sdkDispListBox(pTitle, ptem, 0);

    sdkFreeMem(ptem);
    return rtn;
}

/*****************************************************************************
** Descriptions:	黑白名单管理的名单显示
** Parameters:          u8 * pDelList
                    SDK_SYS_LIST_HAND Delhand
** Returned value:
** Created By:		zhongjiahong  2013.11.07
** Remarks:
*****************************************************************************/
static s32 sdkShowWhiteBlack(const u8 *pDelList, SDK_SYS_LIST_HAND Delhand) {
    if (sdkSysListGetCnt(Delhand) <= 0) {
        sdkDispMsgBox(pDelList, "列表无数据...... ", 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);
        return -1;
    }
    return sdkDisplayList(pDelList, Delhand);
}

/*****************************************************************************
** Descriptions:	黑白名单管理的相关操作
** Parameters:          SDK_SYS_LIST_HAND Delhand
                   SDK_SYS_LIST_HAND Addhand
                   u8 *DelList
                   u8 *AddList
** Returned value:
** Created By:		zhongjiahong  2013.11.07
** Remarks:
*****************************************************************************/
static void sdkDealList(SDK_SYS_LIST_HAND hDelhand, SDK_SYS_LIST_HAND Addhand, u8 *pasDelList,
                        u8 *pasAddList) {
    u8 *pData = NULL;
    char temp[100];
    s32 indx;

    while (1) {
        indx = sdkShowWhiteBlack(pasDelList, hDelhand);

        if (0 <= indx) {
            pData = sdkSysListGetAt(hDelhand, indx);             //取出节点

            memset(temp, 0, sizeof(temp));
            sprintf(temp, "将标签%s置入%s ?\r是--请按确认\r否--请按取消", pData, pasAddList);

            while (1) {
                s32 ret = sdkDispMsgBox(pasDelList, temp, 0, SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC);

                if (SDK_KEY_ESC != ret) {
                    sdkSysListAddAtFront(Addhand, pData);       //将节点插入另一个列表
                    sdkSysListRemoveData(hDelhand, pData);       //删去节点
                    ret = sdkDispMsgBox(pasDelList, "标签置入成功，请按确认返回", 0, SDK_KEY_MASK_ENTER);
                    break;
                }
                else {
                    return;
                }
            }
        }
        else {
            break;
        }
    }
}

/*****************************************************************************
** Descriptions:	判断名单是否已经存在
** Parameters:          SDK_SYS_LIST_HAND ListHand
                               u8 *ucStr
** Returned value:
** Created By:		zhongjiahong  2013.12.05
** Remarks:
*****************************************************************************/
static s32 sdkListIsExist(SDK_SYS_LIST_HAND ListHand, u8 *ucStr) {
    int i, count;

    if (NULL == ucStr || SDK_DEBUG_TAG_LEN < strlen(ucStr)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == ListHand) {
        return SDK_ERR;                                 //名单不存在
    }
    count = sdkSysListGetCnt(ListHand);        //已存在的名单数量

    for (i = 0; i < count; i++) {
        u8 *pData = sdkSysListGetAt(ListHand, i);                 //取出节点

        if (strlen(pData) == strlen(ucStr))          //zhongjiahong 2013.12.19 15:30,长度一样才去比较
        {
            if (0 == memcmp(pData, ucStr, strlen(pData))) {
                return i;                                                 //名单存在
            }
        }
    }

    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	添加名单
** Parameters:          SDK_SYS_LIST_HAND ListHand
                               u8 *ucStr
** Returned value:
** Created By:		zhongjiahong  2013.12.04
** Remarks:
*****************************************************************************/
static s32 sdkAddList(SDK_SYS_LIST_HAND AddList, SDK_SYS_LIST_HAND OppositeList, u8 *ucStr) {
    s32 AddIndex, OppositeIndex;

    if (NULL == ucStr || SDK_DEBUG_TAG_LEN < strlen(ucStr)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (NULL == AddList) {
        AddList = sdkSysListCreate();
    }
    Assert(AddList != NULL);

    AddIndex = sdkListIsExist(AddList, ucStr);                          //该名单是否已经存在于要添加的列表
    OppositeIndex = sdkListIsExist(OppositeList, ucStr);          //判断相反列表是否存在该名单

    if (0 <= AddIndex) {
        sdkDispMsgBox("添加名单", "名单已经存在", 1000, SDK_KEY_MASK_ENTER);                     //显示存在错误
        return SDK_ERR;
    }
    else if (0 <= OppositeIndex) {
        u8 *pData = sdkSysListGetAt(OppositeList, OppositeIndex);             //取出相反列表的节点
        Verify(sdkSysListAddAtFront(AddList, pData) >= 0);         //将节点插入要添加的列表
        sdkSysListRemoveData(OppositeList, pData);                   //删去相反列表的节点
        sdkDispMsgBox("添加名单", "添加名单成功", 1000, SDK_KEY_MASK_ENTER);
        return SDK_OK;
    }
    else {
        void *p = (void *) sdkGetMem(SDK_DEBUG_TAG_LEN);

        if (NULL == p) {
            Assert(0);
            return SDK_ERR;
        }
        memset(p, 0, SDK_DEBUG_TAG_LEN);
        strncpy(p, ucStr, SDK_DEBUG_TAG_LEN);
        Verify(sdkSysListAddAtFront(AddList, p) >= 0);
        sdkDispMsgBox("添加名单", "添加名单成功", 1000, SDK_KEY_MASK_ENTER);
        return SDK_OK;
    }
}

/*****************************************************************************
** Descriptions:	输入名单
** Parameters:          u8 *pheOut
** Returned value:
** Created By:		zhongjiahong  2013.12.05
** Remarks:
*****************************************************************************/
static s32 sdkInputList(u8 *pheOut) {
    if (NULL == pheOut) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE1, 0, "添加名单", SDK_DISP_DEFAULT);
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "名单:", SDK_DISP_LEFT_DEFAULT);
    sdkDispBrushScreen();
    return sdkKbGetScanf(0, pheOut, 1, SDK_DEBUG_TAG_LEN - 1, SDK_MMI_LETTER, SDK_DISP_LINE4);
}

/*****************************************************************************
** Descriptions:	判断并添加名单
** Parameters:          u8 *pheOut
** Returned value:
** Created By:		zhongjiahong  2013.12.05
** Remarks:
*****************************************************************************/
static s32 sdkJudgeAddList(u8 *pheOut) {
    s32 ret;

    if (NULL == pheOut) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = sdkDispMsgBox("名单添加到", "1.白名单，按1键\r2.黑名单，按2键", 0,
                        SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_ESC);

    if (SDK_KEY_1 == ret) {
        return sdkAddList(mWhiteList, mBlackList, pheOut);                     //添加白名单
    }
    else if (SDK_KEY_2 == ret) {
        return sdkAddList(mBlackList, mWhiteList, pheOut);                      //添加黑名单
    }
    else {
        return SDK_ERR;                              //不添加名单
    }
}

/*****************************************************************************
** Descriptions:	名单添加管理
** Parameters:          Input: None
** Returned value:
** Created By:		zhongjiahong  2013.12.05
** Remarks:
*****************************************************************************/
static void sdkAddListManage() {
    s32 Key;
    u8 pheOut[SDK_DEBUG_TAG_LEN + 1];

    while (1) {
        //输入名单
        memset(pheOut, 0, sizeof(pheOut));
        Key = sdkInputList(pheOut);

        if (SDK_KEY_ENTER == Key) {
            //判断并添加名单
            sdkJudgeAddList(&pheOut[1]);
        }
        else {
            break;
        }
    }
}

/*****************************************************************************
** Descriptions:	黑白名单管理主界面
** Parameters:          void
** Returned value:
** Created By:		zhongjiahong  2013.11.07
** Remarks:
*****************************************************************************/
static s32 sdkBlackWhiteManage(void) {
    while (1) {
        s32 ret = sdkDispMsgBox("黑白名单管理", "1.添加名单\r2.白名单管理\r3.黑名单管理\r4.退出ESC", 0,
                                SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 |
                                SDK_KEY_MASK_ESC);

        if (SDK_KEY_1 == ret) {
            sdkAddListManage();
        }
        else if (SDK_KEY_2 == ret) {
            sdkDealList(mWhiteList, mBlackList, "白名单列表", "黑名单");
        }
        else if (SDK_KEY_3 == ret) {
            sdkDealList(mBlackList, mWhiteList, "黑名单列表", "白名单");
        }
        else {
            break;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:输出模式管理主界面
** Parameters:          Input: None
** Returned value:
** Created By:		zhongjiahong  2013.11.06
** Remarks:
*****************************************************************************/
static s32 sdkOutTypeManage() {
    while (1) {
        s32 ret = sdkDispMsgBox("输出模式管理", "1.全部输出\r2.全部阻止\r3.输出白名单\r4.阻止黑名单\r5.退出ESC", 0,
                                SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                                SDK_KEY_MASK_ESC);

        if (SDK_KEY_ESC != ret) {
            SDK_PAIR pair[] =
                    {
                            {SDK_KEY_1, SDK_DEUBG_OUT_ALL},
                            {SDK_KEY_2, SDK_DEUBG_BLOCK_ALL},
                            {SDK_KEY_3, SDK_DEUBG_OUT_WHITE_LIST},
                            {SDK_KEY_4, SDK_DEUBG_BLOCK_BLACK_LIST}
                    };

            mDebugFilter.eOutType = sdkDebugFindPair(pair, sizeof(pair) / sizeof(SDK_PAIR), ret);
            sdkDispMsgBox("输出模式管理", "模式设置完成,确认退出", 4000, SDK_KEY_MASK_ENTER);
        }
        else {
            break;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    调试文件控制
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.10.29
** Remarks:
*****************************************************************************/
static s32 sdkDebugFileControl() {
    while (1) {
        s32 ret = sdkDispMsgBox("调试文件管理", "1.删除文件\r2.copy到U盘\r3.打印文件\r4.退出ESC", 0,
                                SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                                SDK_KEY_MASK_ESC);

        if (SDK_KEY_ESC != ret) {
            switch (ret) {
                case SDK_KEY_1:
                    sdkDebugDelLogFile();
                    break;

                case SDK_KEY_2:
                    sdkDebugCopLogToUpan();
                    break;

                case SDK_KEY_3:
                    sdkDebugPrintLog();
                    break;

                default:
                    break;
            }
        }
        else {
            break;
        }
    }

    return SDK_OK;
}

static s32 sdkDebugCreatDebugFile(const u8 *pasfile) {
    u8 buf[512];
    s32 temp = mDebugFilter.eOutType;

    mDebugFilter.eOutType = SDK_DEUBG_BLOCK_ALL;     //把调试信息全部block 防止创建文件头时出问题


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "File Crate Time (pos clock):\r\n");
    sprintf(&buf[strlen(buf)], "%s\r\n\r\n", sdkTimerGetAsc());
    sdkAppendFile(pasfile, buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "LibDev:");
    sdkSysGetLibdevVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "Panel:");
    sdkSysGetPanelVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "Daemon:");
    sdkSysGetDaemonVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "xgdemv:");
    sdkSysGetLibxgdemvVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "LibMahts:");
    sdkSysGetLibMahtsVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));


    memset(buf, 0, sizeof(buf));
    strcpy(buf, "LibModule:");
    sdkSysGetLibModuleVersion(&buf[strlen(buf)]);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "libsdk.so版本信息:\r\n");
    sdkDebugSysGetFileAllVer("/mtd0/dll/libsdk.so", buf);
    sdkAppendFile(pasfile, buf, strlen(buf));

    memset(buf, 0, sizeof(buf));
    strcpy(buf, "应用编译时间:\r\n");
    strcat(buf, __DATE__ " "__TIME__);
    strcat(buf, "\r\n");
    sdkAppendFile(pasfile, buf, strlen(buf));

    mDebugFilter.eOutType = temp;     //恢复输出模式

    return SDK_OK;
}

static s32 sdkDebugTraceOutSet() {
    s32 MachineCode;

    MachineCode = sdkSysGetMachineCode(NULL);

    while (1) {
        s32 ret = sdkDispMsgBox("输出目标管理", "1.串口输出\r2.屏上输出\r3.打印输出\r4.文件输出\r5.退出ESC", 0,
                                SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                                SDK_KEY_MASK_ESC);

        if (SDK_KEY_ESC != ret) {
            SDK_PAIR pair[] =
                    {
                            {SDK_KEY_1, SDK_DEBUG_TARGET_COM},
                            {SDK_KEY_2, SDK_DEBUG_TARGET_SCREEN},
                            {SDK_KEY_3, SDK_DEBUG_TARGET_RPINT},
                            {SDK_KEY_4, SDK_DEBUG_TARGET_FILE}
                    };

            if ((SDK_SYS_MACHINE_K508A == MachineCode || SDK_SYS_MACHINE_K501AM == MachineCode) &&
                SDK_KEY_4 == ret) {
                sdkDispMsgBox("", "本机型不支持该操作", 1000, SDK_KEY_MASK_ESC);
            }
            else {
                mDebugFilter.eTarget = sdkDebugFindPair(pair, sizeof(pair) / sizeof(SDK_PAIR), ret);
                break;
            }
        }
        else {
            break;
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	打印未释放的内存节点
** Parameters:          Input: None
** Returned value:
** Created By:		zhongjiahong  2013.12.09
** Remarks:
*****************************************************************************/
static void sdkPrintMemoryNode() {
    //sdkDebugMemPrintNotFree();
}

/*****************************************************************************
** Descriptions:	调试信息管理函数
** Parameters:          bool bIsSaveScreen
** Returned value:
** Created By:		shiweisong  2013.10.29
** Remarks:
*****************************************************************************/
s32 sdkDebugControl(bool bIsSaveScreen) {
    //保存屏幕
    u16 *p = NULL;
    s32 key;
    s32 MachineCode;

    Trace("sws", "key :%d \r\n", 56);

    if (bIsSaveScreen == true) {
        p = sdkGetMem(sdkDispGetScreenBufferSize());
        sdkDispSaveScreen(p, sdkDispGetScreenBufferSize());
    }
    MachineCode = sdkSysGetMachineCode(NULL);

    while (1) {
        key = sdkDispMsgBox("调试控制",
                            "1.调试文件管理\r2.黑白名单管理\r3.输出目标管理\r4.输出模式管理\r5.打印未释放内存节点\r6.查看so文件版本信息\r7.退出ESC",
                            0, SDK_KEY_MASK_1 | SDK_KEY_MASK_2 | SDK_KEY_MASK_3 | SDK_KEY_MASK_4 |
                               SDK_KEY_MASK_5 | SDK_KEY_MASK_6 | SDK_KEY_MASK_ESC);

        if (SDK_KEY_ESC != key) {
            switch (key) {
                case SDK_KEY_1:

                    if (SDK_SYS_MACHINE_K508A == MachineCode ||
                        SDK_SYS_MACHINE_K501AM == MachineCode) {
                        sdkDispMsgBox("", "本机型不支持该操作", 1000, SDK_KEY_MASK_ESC);
                    }
                    else {
                        sdkDebugFileControl();
                    }
                    break;

                case SDK_KEY_2:
                    sdkBlackWhiteManage();
                    break;

                case SDK_KEY_3:
                    sdkDebugTraceOutSet();
                    break;

                case SDK_KEY_4:
                    sdkOutTypeManage();
                    break;

                case SDK_KEY_5:
                    sdkPrintMemoryNode();
                    break;

                case SDK_KEY_6:
                    //sdk_dev_traverse_dll_dir();
                    break;

                default:
                    break;
            }
        }
        else {
            break;
        }
    }

    if (bIsSaveScreen == true) {
        Trace("sws", "ret %d \r\n", key);

        //恢复屏幕
        sdkDispRestoreScreen(p, sdkDispGetScreenBufferSize());
        sdkFreeMem(p);
    }
    return SDK_OK;
}

