#include "appglobal.h"


#define LIBS_TOOL_VER "20150915_1"    //编译时日期加序号，当天修改时序号加1
/*****************************************************************************
** Descriptions:	      获取模块版本号
** Parameters:          char *pszVer
** Returned value:	成功返回版本号长度
                                S_RET_PARA_ERR  输入参数错
                                S_RET_ERR失败
** Created By:		王宏杨  2015.04.23
** Remarks:
*****************************************************************************/
/*
int Lib_S_GetTLVVer(char *pszVer)
{
    if(NULL == pszVer)
    {
        return S_RET_PARA_ERR;
    }
    else
    {
        int len = strlen(LIBS_TOOL_VER);

        if(len > 0)
        {
            memcpy(pszVer, LIBS_TOOL_VER, len);
            return len;
        }
        else
        {
            return S_RET_ERR;
        }
    }
}
*/
/*****************************************************************************
** Descriptions:	   获取标签及其长度,最长为三个字节
** Parameters:          unsigned char *pData     tlv数据串
                               unsigned char *pTag       得到的第一个标签
** Returned value:	标签长度     占1～3个字节长度
** Created By:		王宏杨  2015.04.10
** Remarks:
*****************************************************************************/
int Lib_S_PubGetTagTLen(unsigned char *pData, unsigned char *pTag)
{
//	printf("into S_PubGetTagTLen\r\n");
    unsigned char taglen = 0;

    if((NULL == pData) || (0 == pData[0]) || (NULL == pTag))
    {
        return 0;                               //不能返回出错，返回0
    }

    if(0x1F == (pData[0] & 0x1F))
    {
        if(0x80 == (pData[1] & 0x80))
        {
            taglen = 3;
        }
        else
        {
            taglen = 2;
        }
    }
    else
    {
        taglen = 1;
    }
    memcpy(pTag, pData, taglen);
    return taglen;
}

/*****************************************************************************
** Descriptions:	获取标签数据长度
** Parameters:          unsigned char *pIn          TLV数据串
                                int *pOutLen  获取第一个标签的长度
** Returned value:	长度占数据串字节数
** Created By:		王宏杨  2015.03.09
** Remarks:
*****************************************************************************/
int Lib_S_PubGetTagLLen(const unsigned char *pheData, int *piOutLen)
{
    unsigned char tmp = 0;
    unsigned char offset = 0;
    unsigned char i = 0;

    if((NULL == pheData) || (pheData[0] == 0) || (NULL == piOutLen))
    {
        return 0;                               //不能返回出错，返回0
    }

    if(0x80 == (pheData[offset] & 0x80))
    {
        tmp =  pheData[offset++] & 0x7f;            //长度域最高字节后跟随的长度字节数
//        sdkHexToU32(piOutLen, &pheData[offset], tmp);
        tmp = tmp > 4 ? 4 : tmp;  //防止超长
        *piOutLen = 0;

        for(i = 0; i < tmp; i++)
        {
            *piOutLen *= 256;
            *piOutLen += pheData[offset + i];
        }

        return tmp + 1;
    }
    else
    {
        *piOutLen = pheData[offset];
        return 1;
    }
}

/*****************************************************************************
** Descriptions:	获取tlv标签数据总长度
** Parameters:          unsigned char *pucData
** Returned value:    TLV的总长度
** Created By:		lqq2013.07.03
** Remarks:
*****************************************************************************/
int Lib_S_PubGetTLVLen(unsigned char *pheData)
{
    int i = 0, t = 0, l = 0;
    unsigned char tag[10] = {0};

    if((NULL == pheData) || (0 == pheData[0]))
    {
        return 0;                               //不能返回出错，返回0
    }
    i = Lib_S_PubGetTagTLen(pheData, tag);
    l = Lib_S_PubGetTagLLen(&pheData[i], &t);
    i += (l + t);
    return i;
}

/*****************************************************************************
** Descriptions:	      获取tlv
** Parameters:          unsigned char *pData   输入的tlv数据串
                               unsigned char *pTag     得到的第一个标签
                               int *pLen                       得到的第一个标签数据长度
                               unsigned char *pValue   标签数据
** Returned value:	第一个标签tlv的总字节数
** Created By:		王宏杨  2015.04.21
** Remarks:
*****************************************************************************/
int Lib_S_PubGetTLV(unsigned char *pData, unsigned char *pTag, int *pLen, unsigned char *pValue)
{
    int i = 0, t = 0, l = 0;

    if((NULL == pData) || (NULL == pTag) || (NULL == pLen) || (NULL == pValue))
    {
        return 0;                               //不能返回出错，返回0
    }
    i = Lib_S_PubGetTagTLen(pData, pTag);          //取tag
    l = Lib_S_PubGetTagLLen(&pData[i], &t);
    *pLen = t;                                     //标签的数据长度
    memcpy(pValue, &pData[i + l], t);  //取数据值
    i += (l + t);
    return i;
}

/*****************************************************************************
** Descriptions:	      TLV数据转换成标签
** Parameters:          unsigned char  *pData
                               SCRIPT_TAG *pstTag
** Returned value:
** Created By:		王宏杨  2015.04.21
** Remarks:
*****************************************************************************/
int Lib_S_PubTLVToSTag(unsigned char  *pData, SCRIPT_TAG *pstTag)
{
    unsigned char temp[10] = {0};
    unsigned char tag[10] = {0};
    int i = 0, t = 0, l = 0, j = 0;

    if((NULL == pData) || (NULL == pstTag))
    {
        return 0;                               //不能返回出错，返回0
    }
    i = Lib_S_PubGetTagTLen(pData, &tag[0]);          //取tag

//    pstTag->uTag = atoi(&tag[0]);
    for(j = 0; j < i; j++)
    {
        temp[i - j - 1] = tag[j];
    }

    memcpy(&pstTag->uTag, temp, i);
    l = Lib_S_PubGetTagLLen(&pData[i], &t);
    pstTag->usLen = t;                           //标签的数据长度
    pstTag->pucDataAddr = &pData[i + l]; //取数据值
    i += (l + t);
    return i;
}

