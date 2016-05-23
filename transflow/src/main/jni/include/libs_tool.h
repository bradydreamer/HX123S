#ifndef _LIBS_TOOL_H
#define _LIBS_TOOL_H
typedef struct _SCRIPT_TAG
{
    int uTag;  //标签
    unsigned char *pucDataAddr;//标签数据起始地址
    unsigned short usLen;//标签数据长度
    unsigned short usLtype;//长度属性 最高位表示是否定长，其他位表示最大长度
    unsigned char ucDtype;//标签定义类型
    unsigned char ucStype;//标签存储类型    Index 用在$9F66[Index] 下标
    unsigned short usIndex;//标签单字节引用  index为0没有被单字节引用,否则引用第index-1字节，表达式运算时使用。
    unsigned char *pucSpace;//目前仅供表达式使用
} SCRIPT_TAG;
/*************************************
 工具类函数模块头文件
功能:  各种工具类接口函数,TLV标签处理等管理。
**************************************/

/*****************************************************************************
** Descriptions:	   获取标签及其长度,最长为三个字节
** Parameters:    	unsigned char *pData     tlv数据串 
                               unsigned char *pTag       得到的第一个标签
** Returned value:	标签长度
** Created By:		王宏杨  2015.04.10
** Remarks: 		
*****************************************************************************/
extern int Lib_S_PubGetTagTLen(unsigned char *pData,unsigned char *pTag);
/*****************************************************************************
** Descriptions:	获取标签数据长度
** Parameters:    	unsigned char *pIn          TLV数据串
                               u32 *pOutLen  获取第一个标签的长度
** Returned value:	长度占数据串字节数
** Created By:		王宏杨  2015.03.09
** Remarks: 		
*****************************************************************************/
extern int Lib_S_PubGetTagLLen(const unsigned char *pheData, int *piOutLen);
/*****************************************************************************
** Descriptions:	获取tlv标签数据总长度
** Parameters:    	unsigned char *pucData
** Returned value:
** Created By:		lqq2013.07.03
** Remarks:
*****************************************************************************/
extern int Lib_S_PubGetTLVLen(unsigned char *pheData);
/*****************************************************************************
** Descriptions:	      获取tlv
** Parameters:    	unsigned char *pData   输入的tlv数据串 
                               unsigned char *pTag     得到的第一个标签
                               int *pLen                       得到的第一个标签数据长度
                               unsigned char *pValue   标签数据
** Returned value:	
** Created By:		王宏杨  2015.04.21 
** Remarks: 		
*****************************************************************************/
int Lib_S_PubGetTLV(unsigned char *pData,unsigned char *pTag,int *pLen,unsigned char *pValue);
/*****************************************************************************
** Descriptions:	      TLV数据转换成标签
** Parameters:    	unsigned char  *pData
                               SCRIPT_TAG *pstTag
** Returned value:	
** Created By:		王宏杨  2015.04.21
** Remarks: 		
*****************************************************************************/
int Lib_S_PubTLVToSTag(unsigned char  *pData,SCRIPT_TAG *pstTag);
/*****************************************************************************
** Descriptions:	      获取模块版本号
** Parameters:    	char *pszVer
** Returned value:	
** Created By:		王宏杨  2015.04.10
** Remarks: 		
*****************************************************************************/
extern int Lib_S_GetTLVVer(char *pszVer);


#endif

