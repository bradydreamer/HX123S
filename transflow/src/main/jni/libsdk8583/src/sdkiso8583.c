#include "sdkGlobal.h"
//#include "sdkdevGlobal.h"
#include "sdkiso8583.h"

/*****************************************************************************
** Descriptions:	判断某一域表的某一域的数据是否存在
** Parameters:          SDK_8583_ST8583 *pstBag8583:  8583报文结构
                    u8 ucFieldNo:                 域号
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
u32 sdk8583IsDomainExist(SDK_8583_ST8583 const *pstBag8583, u8 ucFieldNo)
{
    u32 ret = 0;
    u16 nHead = 0;

    if((pstBag8583 == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
       // Assert(0);
        return 0;
    }

    if(ucFieldNo > SDK_8583_FIELDMAX || (ucFieldNo > 64 && pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen != 16)) {return false; }

    if(ucFieldNo == SDK_8583_FIELD_MSG || ucFieldNo == SDK_8583_FIELD_MAP)
    {   //消息类型和位元表通过数据长度来判断
        if(pstBag8583->stFiled[ucFieldNo].nFieldLen) {ret = 1; }
    }
    else
    {
        nHead = pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldHead + ((ucFieldNo - 1) / 8);

        if(((pstBag8583->ucBagData[nHead]) & (0x80 >> ((ucFieldNo - 1) % 8))) != 0) {ret = true; }
    }
    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_8583_ST8583 *stBag8583
                               u8 nFieldNo
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
static bool sdk8583SetFieldBit(SDK_8583_ST8583 *stBag8583, u8 ucFieldNo)
{
    u16 nHead = 0, nTmpHead = 0, n = 0;

    if((stBag8583 == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
       // Assert(0);
        return false;
    }

    if(ucFieldNo > SDK_8583_FIELDMAX) {return false; }

    if(ucFieldNo > SDK_8583_FIELD_MAP)
    {
        if(stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen == 0)
        {
            //Assert(0);
            return false;
        }
        nHead = stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldHead;

        if((ucFieldNo > 64) && (stBag8583->nEndFiled <= 64))
        {
            if((stBag8583->nBagLen + 8) > SDK_8583_BAGMAXLEN) {return false; }
            nTmpHead = nHead + 8;
            stBag8583->nBagLen += 8;

            for(n = stBag8583->nBagLen - 1; n >= nTmpHead; n--)
            {
                stBag8583->ucBagData[n] = stBag8583->ucBagData[n - 8];
            }

            memset(&(stBag8583->ucBagData[nTmpHead]), 0x00, 8);
            stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen = 16;
            stBag8583->ucBagData[nHead] |= 0x80;

            for(n = SDK_8583_FIELD_MAP + 1; n <= ucFieldNo; n++)
            {
                if(stBag8583->stFiled[n].nFieldLen > 0)
                {
                    stBag8583->stFiled[n].nFieldHead += 8;
                }
            }
        }
        stBag8583->ucBagData[nHead + ((ucFieldNo - 1) / 8)] |= (0x80 >> ((ucFieldNo - 1) % 8));
    }

    if( ucFieldNo > stBag8583->nEndFiled) {stBag8583->nEndFiled = ucFieldNo; }
    return true;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_8583_ST8583 *stBag8583
                               u8 nFieldNo
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
static bool sdlk8583ClrFieldBit(SDK_8583_ST8583 *stBag8583, u8 ucFieldNo)
{
    u16 nHead;
    u8 n;

    if((stBag8583 == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
        //Assert(0);
        return false;
    }

    if(ucFieldNo > stBag8583->nEndFiled) {return true; }

    if(ucFieldNo > SDK_8583_FIELD_MSG && sdk8583IsDomainExist(stBag8583, ucFieldNo))
    {
        if(stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen == 0)
        {
            //Assert(0);
            return false;
        }
        nHead = stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldHead;
        stBag8583->ucBagData[nHead + ((ucFieldNo - 1) / 8)] &= ~(0x80 >> ((ucFieldNo - 1) % 8));

        if(ucFieldNo == stBag8583->nEndFiled)
        {
            stBag8583->nEndFiled = 0;

            for(; ucFieldNo > 0; ucFieldNo--)
            {
                if(sdk8583IsDomainExist(stBag8583, ucFieldNo))
                {
                    stBag8583->nEndFiled = ucFieldNo;
                    break; //shiweisong 2013.03.14 17:51 张珺提出 设置最后一域时没有break会设置成最小域
                }
            }
        }

        if(stBag8583->nEndFiled <= 64 && ucFieldNo > 64)
        {
            stBag8583->ucBagData[nHead] &= 0x7F;
            memcpy(&(stBag8583->ucBagData[nHead + 8]), &(stBag8583->ucBagData[nHead + 16]), (stBag8583->nBagLen - nHead) - 16);
            stBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen = 8;
            stBag8583->nEndFiled -= 8;

            for(n = SDK_8583_FIELD_MAP + 1; SDK_8583_FIELD_MAP <= stBag8583->nEndFiled; n++) //lint !e441
            {
                if(stBag8583->stFiled[n].nFieldLen > 0)
                {
                    stBag8583->stFiled[n].nFieldHead -= 8;
                }
            }
        }
    }
    return true;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_8583_STFIELDDEF stField
                               u8 *pucOutData
                               u16 nMaxOut
                               u8 *pFieldData
                               u16 nFieldDataLen
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
static s32 sdk8583FormFieldData(SDK_8583_STFIELDDEF stField, u8 *pucOutData,  u16 nMaxOut, const u8 *pFieldData,  u16 nFieldDataLen)
{
    u16 nFLen, nDataLen, nTmp;
    u8 *pucTmp = NULL;
    u8 ucTmpLen[5];
    u8 ucCtrLen[5];
    u16 i;

    if(pucOutData == NULL || pFieldData == NULL)
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }

    if(nFieldDataLen > stField.nFDataLen) {nFieldDataLen = stField.nFDataLen; }
    nFLen = stField.nFLenLen;
    memset(ucTmpLen, 0x00, sizeof(ucTmpLen));

    if(nFLen != 0) //变长
    {
        switch(stField.ucFLenT)
        {
             case SDK_8583_LEN_ASC:
               memset(ucCtrLen, 0x00, sizeof(ucCtrLen));
               sprintf((char*)ucCtrLen, "%%0%dd", nFLen); //shijianglong 2013.05.02 16:23
               sprintf((char*)ucTmpLen, (char*)ucCtrLen, nFieldDataLen); //shijianglong 2013.05.02 16:23
               break;

             case SDK_8583_LEN_BCD:
               nFLen = (nFLen + 1) / 2;
               sdkU16ToBcd(ucTmpLen, nFieldDataLen, nFLen);
               break;

             case SDK_8583_LEN_HEXL:
               nTmp = nFieldDataLen;

               for(i = 0; i < nFLen; i++)
               {
                   ucTmpLen[i] = nTmp % 256;
                   nTmp /= 256;
               }

               break;

             case SDK_8583_LEN_HEXR:
               nTmp = nFieldDataLen;

               for(i = nFLen; i > 0; i--)
               {
                   ucTmpLen[i] = nTmp % 256;
                   nTmp /= 256;
               }

               break;

             default:
               /*=======BEGIN: fusuipu 2014.03.19  19:53 modify 根据253行的逻辑，此处直接返回===========*/
               //nFLen = 0;
               //nDataLen = 0;
               return -3;
               /*====================== END======================== */
        }

        nDataLen = nFieldDataLen;
    }
    else
    {
        nDataLen = stField.nFDataLen; //定长
    }

    if(stField.ucFDataT == SDK_8583_DATA_BCD)
    {
        nDataLen += nDataLen % 2;
    }
    nTmp = nDataLen + nFLen;

    if(nTmp > nMaxOut || nTmp == 0)
    {
        return -3;
    }

    if(nFLen > 0)
    {
        memcpy(pucOutData, ucTmpLen, nFLen);
    }

    if(nDataLen > 0)
    {
        pucTmp = (u8 *)malloc(nDataLen);

        if(pucTmp == NULL)
        {
            //Assert(0);
            return -2;
        }
        memset(pucTmp, stField.ucFDataS, nDataLen);

        if(stField.ucFDataA == SDK_8583_ALIGN_R) //补齐
        {
            memcpy(&pucTmp[nDataLen - nFieldDataLen], pFieldData, nFieldDataLen);
        }
        else
        {
            memcpy(pucTmp, pFieldData, nFieldDataLen);
        }

        if(stField.ucFDataT == SDK_8583_DATA_BCD)
        {
            sdkAscToBcd(&pucOutData[nFLen], pucTmp, nDataLen) >= 0;
            nDataLen /= 2;
        }
        else
        {
            memcpy(&pucOutData[nFLen], pucTmp, nDataLen);
        }

        free(pucTmp) ;
    }
    return nFLen + nDataLen;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_8583_STFIELDDEF stField
                               SDK_8583_ST8583 *stBag8583
                               u8 nFieldNo
                               u8 *pFieldData
                               u16 nFieldDataLen
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
static s32 sdk8583ReplaceField(SDK_8583_STFIELDDEF stField, SDK_8583_ST8583 *stBag8583, u8 ucFieldNo, const u8 *pFieldData, u16 nFieldDataLen)
{
    u8 ucTmpData[SDK_8583_BAGMAXLEN];
    u16 nHead = 0, nOrgLen = 0, nTmpLen;
    u8 nCurrField;
    s32 nLen;

    if((stBag8583 == NULL) || (pFieldData == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }
    nHead = stBag8583->nBagLen; //lint !e838


    for(nCurrField = ucFieldNo; nCurrField <= stBag8583->nEndFiled; nCurrField++)
    {
        if(sdk8583IsDomainExist(stBag8583, nCurrField))
        {
            nHead = stBag8583->stFiled[nCurrField].nFieldHead;

            if(nCurrField == ucFieldNo)
            {
                nOrgLen = stBag8583->stFiled[nCurrField].nFieldLen;
            }
            break;
        }
    }

    nTmpLen = SDK_8583_BAGMAXLEN - (stBag8583->nBagLen - nOrgLen);
    nLen = sdk8583FormFieldData(stField, ucTmpData, nTmpLen, pFieldData, nFieldDataLen);

    if(nLen > 0)
    {
        if(nLen == nOrgLen)
        {
            memcpy(&(stBag8583->ucBagData[nHead]), ucTmpData, nLen);
        }
        else
        {
            stBag8583->stFiled[ucFieldNo].nFieldHead = nHead;
            stBag8583->stFiled[ucFieldNo].nFieldLen = (u16)nLen;

            if(ucFieldNo == nCurrField) {nCurrField++; }

            if(nCurrField >= SDK_8583_FIELDMAX + 1) //fusuipu 2014.03.19 19:25
            {
                return SDK_ERR;
            }

            if(nLen < nOrgLen)
            {
                memcpy(&(stBag8583->ucBagData[nHead]), ucTmpData, nLen); //lint !e671
                memcpy(&(stBag8583->ucBagData[nHead + nLen]), &(stBag8583->ucBagData[nHead + nOrgLen]), (stBag8583->nBagLen - nHead) - nOrgLen);
                nTmpLen = nOrgLen - (u16)nLen;
                stBag8583->nBagLen -= nTmpLen;

                for(; nCurrField <= stBag8583->nEndFiled; nCurrField++)
                {
                    if(stBag8583->stFiled[nCurrField].nFieldLen > 0)
                    {
                        stBag8583->stFiled[nCurrField].nFieldHead -= nTmpLen;
                    }
                }
            }
            else
            {
                memcpy(&ucTmpData[nLen], &(stBag8583->ucBagData[nHead + nOrgLen]), (stBag8583->nBagLen - nHead) - nOrgLen);
                memcpy(&(stBag8583->ucBagData[nHead]), ucTmpData, stBag8583->nBagLen + (nLen - nHead) - nOrgLen);
                nTmpLen = (u16)nLen - nOrgLen;
                stBag8583->nBagLen += nTmpLen;

                for(; nCurrField <= stBag8583->nEndFiled; nCurrField++)
                {
                    if(stBag8583->stFiled[nCurrField].nFieldLen > 0)
                    {
                        stBag8583->stFiled[nCurrField].nFieldHead += nTmpLen;
                    }
                }

                if(nOrgLen == 0)
                { //此时原域不存在，需要重新置位元表
                    sdk8583SetFieldBit(stBag8583, ucFieldNo);
                }
            }
        }
    }
  //  Trace("sdk8583ReplaceField", "ucFieldNo=%d stBag8583->nEndField=%d\r\n", ucFieldNo, stBag8583->nEndFiled);
  //  TraceHex("sdk8583ReplaceField", "data", &stBag8583->ucBagData[nHead], nLen);

    return nLen;
}

/*****************************************************************************
** Descriptions:	设置某一域表的某一域的数据
** Parameters:          SDK_8583_STFIELDDEF stField : 域表
                    SDK_8583_ST8583 *pstBag8583 : 8583报文结构
                    u8 ucFieldNo                : 域号
                    u8 *pFieldData              : 输入数据
                    u16 usFieldDataLen          : 输入数据长度
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
s32 sdk8583SetField(SDK_8583_STFIELDDEF stField, SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo, const u8 *pFieldData, u16 usFieldDataLen)
{
    u16 head;
    s32 len;

    if((pstBag8583 == NULL) || (pFieldData == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
       // Assert(0);
        return SDK_PARA_ERR;
    }

    if(pstBag8583->nBagLen >= SDK_8583_BAGMAXLEN) {return -1; }

    if( ucFieldNo > SDK_8583_FIELD_MAP && pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen == 0)
    {
        memset(&(pstBag8583->ucBagData[pstBag8583->nBagLen]), 0x00, 8);
        pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldHead = pstBag8583->nBagLen;
        pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldLen = 8;
        pstBag8583->nBagLen += 8;
    }

    if((ucFieldNo == pstBag8583->nEndFiled && pstBag8583->stFiled[ucFieldNo].nFieldLen > 0) || ucFieldNo < pstBag8583->nEndFiled)
    {
        return sdk8583ReplaceField(stField, pstBag8583, ucFieldNo, pFieldData, usFieldDataLen);
    }
    head = pstBag8583->nBagLen;
    len = SDK_8583_BAGMAXLEN - pstBag8583->nBagLen;

    if(len <= 0)
    {
        //Assert(0);
        return SDK_ERR;
    }
    len = sdk8583FormFieldData(stField, &pstBag8583->ucBagData[head], (u16)len, pFieldData, usFieldDataLen);

    if(len > 0)
    {
        pstBag8583->stFiled[ucFieldNo].nFieldHead = head;
        pstBag8583->stFiled[ucFieldNo].nFieldLen = (u16)len;

        pstBag8583->nBagLen += (u16)len;

        sdk8583SetFieldBit(pstBag8583, ucFieldNo);
    }
    //Trace("sdk8583SetField","ucFieldNo=%d pstBag8583->nEndField=%d\r\n",ucFieldNo,pstBag8583->nEndFiled);
    //TraceHex("sdk8583SetField","data",&pstBag8583->ucBagData[head],len);

    return len;
}

/*****************************************************************************
** Descriptions:	将报文某一域表中的数据复制到另一报文
** Parameters:          SDK_8583_ST8583 *pstBag8583Dest : 目标8583报文结构
                    SDK_8583_ST8583 *pstBag8583Src  : 源8583报文结构
                    u8 ucFieldNo                    : 域号
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
s32 sdk8583CpyField(SDK_8583_ST8583 *pstBag8583Dest, SDK_8583_ST8583 const *pstBag8583Src, u8 ucFieldNo)
{
    s32 nHead = 0, nOrgHead = 0, nOrgLen = 0, nTmpHead, nTmpLen = 0;
    u8 nCurrField;
    s32 n; //u16 n;//zhangjun 2013.02.26 10:1
    s32 nLen = 0;

    if((pstBag8583Dest == NULL) || (pstBag8583Src == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }

    if(sdk8583IsDomainExist(pstBag8583Src, ucFieldNo))
    {
        nHead = pstBag8583Src->stFiled[ucFieldNo].nFieldHead;
        nLen = pstBag8583Src->stFiled[ucFieldNo].nFieldLen;

        if(nLen > 0)
        {
            nOrgHead = pstBag8583Dest->nBagLen;
            nOrgLen = 0;

            for(nCurrField = ucFieldNo; nCurrField <= pstBag8583Dest->nEndFiled; nCurrField++)
            {
                if(sdk8583IsDomainExist(pstBag8583Dest, nCurrField))
                {
                    nOrgHead = pstBag8583Dest->stFiled[nCurrField].nFieldHead;

                    if(nCurrField == ucFieldNo)
                    {
                        nOrgLen = pstBag8583Dest->stFiled[ucFieldNo].nFieldLen;
                    }
                    break;
                }
            }

            if(nLen == nOrgLen)
            {
                memcpy(&(pstBag8583Dest->ucBagData[nOrgHead]), &(pstBag8583Src->ucBagData[nHead]), nLen);
            }
            else
            {
                if(nLen <= nOrgLen) //lint !e831
                {
                    memcpy(&(pstBag8583Dest->ucBagData[nOrgHead]), &(pstBag8583Src->ucBagData[nHead]), nLen);   //lint !e671
                    memcpy(&(pstBag8583Dest->ucBagData[nOrgHead + nLen]), &(pstBag8583Dest->ucBagData[nOrgHead + nOrgLen]), (pstBag8583Dest->nBagLen - nOrgHead) - nOrgLen);
                    nTmpLen = nOrgLen - nLen;
                    pstBag8583Dest->nBagLen -= (u16)nTmpLen;

                    if(ucFieldNo == nCurrField) {nCurrField++; }

                    if(nCurrField >= SDK_8583_FIELDMAX + 1)
                    {
                        return SDK_ERR;
                    }

                    for(; nCurrField <= pstBag8583Dest->nEndFiled; nCurrField++)
                    {
                        if(pstBag8583Dest->stFiled[nCurrField].nFieldHead >= nOrgHead)
                        {
                            pstBag8583Dest->stFiled[nCurrField].nFieldHead -= (u16)nTmpLen;
                        }
                    }
                }
                else
                {
                    nTmpLen = nLen - nOrgLen;

                    if((pstBag8583Dest->nBagLen + nTmpLen) > SDK_8583_BAGMAXLEN) {return -1; }
                    pstBag8583Dest->nBagLen += (u16)nTmpLen;

                    nTmpHead = nOrgHead + nOrgLen;

                    for(n = pstBag8583Dest->nBagLen - 1; n >= nTmpHead; n--)
                    {
                        pstBag8583Dest->ucBagData[n] = pstBag8583Dest->ucBagData[n - nTmpLen];
                    }

                    memcpy(&(pstBag8583Dest->ucBagData[nOrgHead]), &(pstBag8583Src->ucBagData[nHead]), nLen);

                    if(ucFieldNo == nCurrField) {nCurrField++; }

                    if(nCurrField >= SDK_8583_FIELDMAX + 1)
                    {
                        return SDK_ERR;
                    }

                    for(; nCurrField <= pstBag8583Dest->nEndFiled; nCurrField++)
                    {
                        if(pstBag8583Dest->stFiled[nCurrField].nFieldHead >= nOrgHead)
                        {
                            pstBag8583Dest->stFiled[nCurrField].nFieldHead += (u16)nTmpLen;
                        }
                    }
                }

                if(ucFieldNo >= SDK_8583_FIELDMAX + 1)
                {
                    //Assert(0);
                    return SDK_ERR;
                }
                pstBag8583Dest->stFiled[ucFieldNo].nFieldHead = (u16)nOrgHead;
                pstBag8583Dest->stFiled[ucFieldNo].nFieldLen = (u16)nLen;


                if(nOrgLen == 0)
                {   //此时原域不存在，需要重新置位元表
                    sdk8583SetFieldBit(pstBag8583Dest, ucFieldNo);
                }
            }
        }
    }
    return nLen;
}

/*****************************************************************************
** Descriptions:	清除报文结构中的某一域数据
** Parameters:          SDK_8583_ST8583 *pstBag8583 :  8583报文结构
                    u8 ucFieldNo                :  域号
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
s32 sdk8583ClrField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo)
{
    s32 nHead = 0, nOrgLen = 0;
    u8 i;

    if((pstBag8583 == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }

    if(sdk8583IsDomainExist(pstBag8583, ucFieldNo))
    {
        nHead = pstBag8583->stFiled[ucFieldNo].nFieldHead;
        nOrgLen = pstBag8583->stFiled[ucFieldNo].nFieldLen;

        memcpy(&(pstBag8583->ucBagData[nHead]), &(pstBag8583->ucBagData[nHead + nOrgLen]), (pstBag8583->nBagLen - nHead) - nOrgLen);
        pstBag8583->nBagLen -= (u16)nOrgLen;

        for(i = ucFieldNo; i <= pstBag8583->nEndFiled; i++)
        {
            if(pstBag8583->stFiled[i].nFieldLen > 0)
            {
                pstBag8583->stFiled[i].nFieldHead -= (u16)nOrgLen;
            }
        }

        pstBag8583->stFiled[ucFieldNo].nFieldHead = 0;
        pstBag8583->stFiled[ucFieldNo].nFieldLen = 0;
        sdlk8583ClrFieldBit(pstBag8583, ucFieldNo);
    }
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_8583_ST8583 *stBag8583
                    SDK_8583_STFIELDDEF stField
                    u8 ucFieldNo
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
static s32 sdk8583ParseField(SDK_8583_ST8583 *stBag8583, SDK_8583_STFIELDDEF stField, u8 ucFieldNo)
{
    u16 nFLen = 0, nDataLen = 0;
    u8 ucTmpLen[5];
    u16 i;
    u8 *p = NULL;
    u16 nHead;

    if((stBag8583 == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }
    nHead = stBag8583->stFiled[stBag8583->nEndFiled].nFieldHead + stBag8583->stFiled[stBag8583->nEndFiled].nFieldLen;
    p = &(stBag8583->ucBagData[nHead]); //lint !e838

    if(ucFieldNo == SDK_8583_FIELD_MAP)
    {
        nFLen = 0;

        if((p[0] & 0x80) == 0x80 && SDK_8583_FIELDMAX == 128)   //lint !e774 !e506
        {
            nDataLen = 16;
        }
        else{nDataLen = 8; }
    }
    else
    {
        nFLen = stField.nFLenLen;

        if(nFLen != 0) //变长
        {
            memset(ucTmpLen, 0x00, sizeof(ucTmpLen));

            switch(stField.ucFLenT)
            {
                 case SDK_8583_LEN_ASC:
                   memcpy(ucTmpLen, p, nFLen);
                   nDataLen = (u16)atoi((const char *)ucTmpLen); //shijianglong 2013.05.02 16:25
                   break;

                 case SDK_8583_LEN_BCD:
                   nFLen = (nFLen + 1) / 2;
                   sdkBcdToU16(&nDataLen, p, nFLen);
                   break;

                 case SDK_8583_LEN_HEXL:

                   for(i = 0; i < nFLen; i++)
                   {
                       nDataLen *= 256;
                       nDataLen += p[i];
                   }

                   break;

                 case SDK_8583_LEN_HEXR:

                   for(i = nFLen; i > 0; i--)
                   {
                       nDataLen *= 256;
                       nDataLen += p[i - 1];
                   }

                   break;

                 default:
                   nDataLen = stField.nFDataLen;
                   break;
            }
        }
        else
        {
            nDataLen = stField.nFDataLen;  //定长
        }

        if(nDataLen > stField.nFDataLen) {nDataLen = stField.nFDataLen; }

        if(stField.ucFDataT == SDK_8583_DATA_BCD)
        {
            nDataLen = (nDataLen + 1) / 2;
        }
    }

    if((nHead + nFLen + nDataLen) > SDK_8583_BAGMAXLEN) {return 0; }
    stBag8583->stFiled[ucFieldNo].nFieldHead = nHead;
    stBag8583->stFiled[ucFieldNo].nFieldLen = nFLen + nDataLen;
    stBag8583->nEndFiled = ucFieldNo;

    //Trace("sdk8583ParseField","nFieldNo=%d\r\n",stBag8583->nEndFiled);
    //TraceHex("sdk8583ParseField","data",&stBag8583->ucBagData[nHead],nFLen + nDataLen);

    return stBag8583->stFiled[ucFieldNo].nFieldLen;
}

/*****************************************************************************
** Descriptions:   获取某一域表的某一域的数据是否存在
** Parameters:     SDK_8583_STFIELDDEF stField  :域表
                   SDK_8583_ST8583 *pstBag8583  :8583报文结构
                   u8 ucFieldNo                 :域号
                   u8 *pFieldData               :输出数据最大长度
                   u16 usMaxSize                :输出域数据
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
s32 sdk8583GetField(SDK_8583_STFIELDDEF stField, SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo, u8 *pFieldData, u16 usMaxSize)
{
    s32 nFLen = 0, nDataLen = 0;
    u8 ucTmpLen[5];
    u8 i;
    u8 *p = NULL;

    if((&stField == NULL) || (pstBag8583 == NULL) || (pFieldData == NULL) || (ucFieldNo > SDK_8583_FIELDMAX))
    {
      //  Assert(0);
        return SDK_PARA_ERR;
    }
    p = &(pstBag8583->ucBagData[pstBag8583->stFiled[ucFieldNo].nFieldHead]);
    nDataLen = pstBag8583->stFiled[ucFieldNo].nFieldLen;

    if(nDataLen == 0)
    {
        return 0;
    }
    nFLen = stField.nFLenLen;

    if(nFLen != 0) //变长
    {
        memset(ucTmpLen, 0x00, sizeof(ucTmpLen));

        switch(stField.ucFLenT)
        {
             case SDK_8583_LEN_ASC:
               memcpy(ucTmpLen, p, nFLen);
               nDataLen = atoi((const char *)ucTmpLen); //shijianglong 2013.05.02 16:25
               break;

             case SDK_8583_LEN_BCD:
               nFLen = (nFLen + 1) / 2;
               sdkBcdToU32((u32 *)(&nDataLen), p, nFLen); //shijianglong 2013.05.02 16:29
               break;

             case SDK_8583_LEN_HEXL:
               nDataLen = 0;
               for(i = 0; i < nFLen; i++)
               {
                   nDataLen *= 256;
                   nDataLen += p[i];
               }

               break;

             case SDK_8583_LEN_HEXR:
               nDataLen = 0;
               for(i = nFLen; i > 0; i--)
               {
                   nDataLen *= 256;
                   nDataLen += p[i - 1];
               }

               break;

             default:
               nDataLen = stField.nFDataLen;
               break;
        }
    }
    else
    {
        nDataLen = stField.nFDataLen;  //定长
    }

    if(nDataLen > stField.nFDataLen) {nDataLen = stField.nFDataLen; }

    //Trace("sdk8583GetField","ucFieldNo=%d\r\n",ucFieldNo);
    if(stField.ucFDataT == SDK_8583_DATA_BCD)
    {
        if(((nDataLen + 1) / 2) > usMaxSize) {nDataLen = 2 * usMaxSize; }
        sdkBcdToAsc(pFieldData, p + nFLen, (nDataLen + 1) / 2);

        if((stField.ucFDataA == SDK_8583_ALIGN_R) && (nDataLen % 2)) //补齐//zhangjun 2013.02.26 10:2
        {
            memcpy(pFieldData, &pFieldData[1], nDataLen);
            pFieldData[nDataLen] = 0;
        }

        if(nDataLen % 2) {pFieldData[nDataLen] = 0x00; }
        //Trace("sdk8583GetField","data-bcd(len=%d)%s\r\n",nDataLen,pFieldData);
    }
    else
    {
        if(nDataLen > usMaxSize) {nDataLen = usMaxSize; }
        memcpy(pFieldData, p + nFLen, nDataLen);

        if(stField.ucFDataT == SDK_8583_DATA_ASC)
        {
            //Trace("sdk8583GetField","data-str(len=%d)%s\r\n",nDataLen,pFieldData);
        }
        else
        {
            ; //TraceHex("sdk8583GetField","data-hex",pFieldData,nDataLen);
        }
    }
    return nDataLen;
}

/*****************************************************************************
** Descriptions:	解析报文数据
** Parameters:          SDK_8583_STFIELDDEF *pstField : 域属性表
                    SDK_8583_ST8583 *pstBag8583   : 解析前的报文结构 解析后的报文结构
                    u16 usMsgHead                 : 消息类型码的起始位置
** Returned value:
** Created By:		zhangjun  2013.02.22
** Remarks:
*****************************************************************************/
s32 sdk8583Parse8583(SDK_8583_STFIELDDEF const *pstField, SDK_8583_ST8583 *pstBag8583, u16 usMsgHead)
{
    int nFieldId, nFieldNum = SDK_8583_FIELDMAX;
    int nLen = 0;
    int nBagLen = 0;

    if(pstBag8583 == NULL || pstField == NULL)
    {
        //Assert(0);
        return SDK_PARA_ERR;
    }
    //TraceHex("sdk8583Parse8583", "收到数据\r\n", pstBag8583->ucBagData, pstBag8583->nBagLen);

    //从报文头开始解析
    pstBag8583->stFiled[SDK_8583_FIELD_MSG].nFieldHead = usMsgHead;
    nBagLen += usMsgHead;

    //消息类型
    nBagLen += sdk8583ParseField(pstBag8583, pstField[SDK_8583_FIELD_MSG], SDK_8583_FIELD_MSG);

    //位元表
    nBagLen += sdk8583ParseField(pstBag8583, pstField[SDK_8583_FIELD_MAP], SDK_8583_FIELD_MAP);

    if((pstBag8583->ucBagData[pstBag8583->stFiled[SDK_8583_FIELD_MAP].nFieldHead] & 0x80) == 0) {nFieldNum = 64; }

    for(nFieldId = 2; nFieldId <= nFieldNum; nFieldId++)
    {
        if(sdk8583IsDomainExist(pstBag8583, (u8)nFieldId))
        {
            nLen = sdk8583ParseField(pstBag8583, pstField[nFieldId], (u8)nFieldId);

            if(nLen > 0)
            {
                nBagLen += nLen;
            }
            else
            {
                return 0;
            }
        }
    }

    pstBag8583->nBagLen = (u16)nBagLen;

    return pstBag8583->nBagLen;
}
/*****************************************************************************
** Descriptions: 此函数从2.0不再支持，为了兼容1.06，在此提供空函数
** Parameters: u8 *pasVer
** Returned value:
** Created By: shiweisong 2012.12.12
** Remarks:
*****************************************************************************/
s32 sdk8583GetVersion(u8 *pasVer)
{
    return SDK_ERR;
}

