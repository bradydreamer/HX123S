/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkMathMaualTest.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��1��22��
   ����޸�   :
   ��������   : �ֶ�������ѧ��
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2013��1��22��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

#include "sdkGlobal.h"
#include "SdkTest.h"

#define InitMem(dsc) memset(dsc, 0, sizeof(dsc))

//#define GetLen(Len1,Len2) ((Len1) > (Len2) ? (Len2):(Len1))
#define GetLen(Len1, Len2)

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.01.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkU8ToBcdManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp = NULL, *temp2 = NULL;
    u8 uc = sdkTestInputU8(pTitle, "u8:");
    s32 len = sdkTestInputs32(pTitle, "len:");
    s32 ret = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp2, 0, 1024);

    ret = sdkU8ToBcd(temp, uc, len);
    sdkTestPintHex(temp2, temp, len);

    sprintf(pasOutInfo, "u8:%d \rlen:%d \rBcd:%s\rreturn:%d", uc, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp2);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
#if 0 /*Modify by fusuipu at 2013.01.30  9:48 */
static s32 Test_sdkMathsGetVersionManual(char *pasOutInfo, char const*pTitle)   //  1
{
    u8 temp[200] = {0};

    InitMem(temp);
    sdkMathsGetVersion(temp);
    strcpy(pasOutInfo, temp);
    return 0;
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkU16ToBcdManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024];
//    u8 temp2[1024];
    u8 *temp = NULL, *temp2 = NULL;
    s32 ret = 0;
    u16 uc = sdkTestInputU16(pTitle, "U16:");
    s32 len = sdkTestInputs32(pTitle, "len:");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp2, 0, 1024);

    ret = sdkU16ToBcd(temp, uc, len);
    sdkTestPintHex(temp2, temp, len);
    sprintf(pasOutInfo, "U16:%d \rlen:%d \rBcd:%s\rreturn:%d", uc, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp2);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkU32ToBcdManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp = NULL, *temp2 = NULL;
    s32 ret = 0;
    u32 uc = sdkTestInputU32(pTitle, "U32:");
    s32 len = sdkTestInputs32(pTitle, "len:");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp2, 0, 1024);

    ret = sdkU32ToBcd(temp, uc, len);
    sdkTestPintHex(temp2, temp, len);
    sprintf(pasOutInfo, "U32:%d \rlen:%d \rBcd:%s\rreturn:%d", uc, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp2);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdToU8Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    u8 rst = 0;
    u32 bcdlen = 0;
    s32 ret = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("BcdToU8����", dst, "BCD:");
    len = sdkTestInputs32(pTitle, "len:");
    //   len = GetLen(len, bcdlen);

    ret = sdkBcdToU8(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);

    sprintf(pasOutInfo, "Bcd:%s \rlen:%d \rU8:%d\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdToU16Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024];
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    s32 ret = 0;
    u16 rst = 0;
    s32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("BcdToU16����", dst, "BCD:");
    len = sdkTestInputs32(pTitle, "len:");
    //   len = GetLen(len, bcdlen);
    ret = sdkBcdToU16(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);

    sprintf(pasOutInfo, "Bcd:%s \rlen:%d \rU16:%d\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdToU32Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    s32 ret = 0;
    u32 rst = 0;
    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("BcdToU32����", dst, "BCD:");
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, bcdlen);
    ret = sdkBcdToU32(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);

    sprintf(pasOutInfo, "Bcd:%s \rlen:%d \rU32:%d\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkHexToU16Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024];
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    s32 ret = 0;
    u16 rst = 0;

    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("HexToU16����", dst, "HEX:");
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, bcdlen);
    ret = sdkHexToU16(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);
    sprintf(pasOutInfo, "Hex:%s \rlen:%d \rU16:%d\rreturn:%d", temp, len, rst, ret);
    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkHexToU32Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    s32 ret = 0;
    u32 rst = 0;

    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("HexToU32����", dst, "HEX:");
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, bcdlen);
    memset(temp, 0, sizeof(temp));
    ret = sdkHexToU32(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);
    sprintf(pasOutInfo, "Hex:%s \rlen:%d \rU32:%d\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkU16ToHexManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp = NULL, *temp2 = NULL;
    s32 ret = 0;
    u16 uc = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp2, 0, 1024);
    uc = sdkTestInputU16(pTitle, "U16:");
    len = sdkTestInputs32(pTitle, "len:");
    ret = sdkU16ToHex(temp, uc, len);
    sdkTestPintHex(temp2, temp, len);
    sprintf(pasOutInfo, "U16:%d \rlen:%d \rHex:%s\rreturn:%d", uc, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp2);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkU32ToHexManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp = NULL, *temp2 = NULL;
    s32 ret = 0;
    u32 uc = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp2, 0, 1024);
    uc = sdkTestInputU32(pTitle, "U32:");
    len = sdkTestInputs32(pTitle, "len:");
    ret = sdkU32ToHex(temp, uc, len);
    sdkTestPintHex(temp2, temp, len);
    sprintf(pasOutInfo, "U32:%d \rlen:%d \rHex:%s\rreturn:%d", uc, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp2);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdToFloatManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *dst = NULL;
    s32 ret = 0;
    double rst = 0;
    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("BcdToFloat����", dst, "BCD:");
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, bcdlen);
    ret = sdkBcdToFloat(&rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);

    sprintf(pasOutInfo, "BCD:%s \rlen:%d \rFloat:%f\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdToAscManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 rst[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *rst = NULL, *dst = NULL;
    s32 ret = 0;
    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(dst);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(rst, 0, 1024);
    memset(dst, 0, 1024);

    bcdlen = sdkTestInputBCD("sdkBcdToAsc����", dst, "BCD:");
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, bcdlen);
    ret = sdkBcdToAsc(rst, dst, len);

    sdkTestPintHex(temp, dst, bcdlen);

    sprintf(pasOutInfo, "Bcd:%s \rlen:%d \rAsc:%s\rreturn:%d", temp, len, rst, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);
    sdkFreeMem(rst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkAscToBcdManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024];
//    u8 rst[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *temp = NULL, *rst = NULL, *dst = NULL;
    s32 ret = 0;

    u32 asclen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(dst);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(rst, 0, 1024);
    memset(dst, 0, 1024);

    asclen = sdkTestInputASC(dst, "sdkAscToBcd", "ASC:", 1, 50);
    len = sdkTestInputs32(pTitle, "len:");
//    len = GetLen(len, asclen);
    ret = sdkAscToBcd(rst, dst, len);
    sdkTestPintHex(temp, rst, ret);

    sprintf(pasOutInfo, "Asc:%s \rlen:%d \rBcd:%s\rreturn:%d", dst, len, temp, ret);

    sdkFreeMem(temp);
    sdkFreeMem(dst);
    sdkFreeMem(rst);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkXOR8Manual(char *pasOutInfo, char const *pTitle) {
//    u8 src1[1024] = {0};
//    u8 src2[1024] = {0};
//    u8 rst[1024] = {0};
//    u8 temp[1024] = {0};
    u8 *src1 = NULL, *src2 = NULL, *rst = NULL, *temp = NULL;
    s32 ret = 0;
    u16 src1len = 0;
    u16 src2len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    src1 = sdkGetMem(1024);
    if (src1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    src2 = sdkGetMem(1024);
    if (src2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        sdkFreeMem(src2);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(src1, 0, 1024);
    memset(src2, 0, 1024);
    memset(rst, 0, 1024);

    src1len = sdkTestInputBCD("sdkXOR8����", src1, "Hex1:");
    src2len = sdkTestInputBCD("sdkXOR8����", src2, "Hex2:");

    ret = sdkXOR8(rst, src1, src2);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, rst, 8);
    memset(rst, 0, 1024);
    strcpy(rst, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src1, src1len);
    memset(src1, 0, 1024);
    strcpy(src1, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src2, src2len);
    memset(src2, 0, 1024);
    strcpy(src2, temp);

    sprintf(pasOutInfo, "Hex1:%s \rHex2:%s \rHex:%s\rreturn:%d", src1, src2, rst, ret);

    sdkFreeMem(src1);
    sdkFreeMem(src2);
    sdkFreeMem(rst);
    sdkFreeMem(temp);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdAddManual(char *pasOutInfo, char const *pTitle) {
//    u8 src1[1024] = {0};
//    u8 src2[1024] = {0};
//    u8 rst[1024] = {0};
//    u8 temp[1024] = {0};
    u8 *src1 = NULL, *src2 = NULL, *rst = NULL, *temp = NULL;

    s32 ret = 0;
    u16 src1len = 0;
    u16 src2len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    src1 = sdkGetMem(1024);
    if (src1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    src2 = sdkGetMem(1024);
    if (src2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        sdkFreeMem(src2);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(src1, 0, 1024);
    memset(src2, 0, 1024);
    memset(rst, 0, 1024);

    sdkTestInputBCD("sdkBcdAdd����", src1, "Bcd1:");
    src1len = sdkTestInputs32(pTitle, "Bcd1 len:");
    sdkTestInputBCD("sdkBcdAdd����", src2, "Bcd2:");
    src2len = sdkTestInputs32(pTitle, "Bcd2 len:");

    ret = sdkBcdAdd(rst, src1, src1len, src2, src2len);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, rst, ret);
    memset(rst, 0, 1024);
    strcpy(rst, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src1, src1len);
    memset(src1, 0, 1024);
    strcpy(src1, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src2, src2len);
    memset(src2, 0, 1024);
    strcpy(src2, temp);

    sprintf(pasOutInfo, "Bcd1:%s \rBcd2:%s \rBcdSum:%s\rreturn:%d", src1, src2, rst, ret);

    sdkFreeMem(src1);
    sdkFreeMem(src2);
    sdkFreeMem(rst);
    sdkFreeMem(temp);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkBcdSubManual(char *pasOutInfo, char const *pTitle) {
//    u8 src1[1024] = {0};
//    u8 src2[1024] = {0};
//    u8 rst[1024] = {0};
//    u8 temp[1024] = {0};
    u8 *src1 = NULL, *src2 = NULL, *rst = NULL, *temp = NULL;
    s32 ret = 0;
    u16 src1len = 0;
    u16 src2len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    src1 = sdkGetMem(1024);
    if (src1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    src2 = sdkGetMem(1024);
    if (src2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(src1);
        sdkFreeMem(src2);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(src1, 0, 1024);
    memset(src2, 0, 1024);
    memset(rst, 0, 1024);

    sdkTestInputBCD("sdkBcdSub����", src1, "Bcd1:");
    src1len = sdkTestInputs32(pTitle, "Bcd1 len:");

    sdkTestInputBCD("sdkBcdSub����", src2, "Bcd2:");
    src2len = sdkTestInputs32(pTitle, "Bcd2 len:");

    ret = sdkBcdSub(rst, src1, src1len, src2, src2len);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, rst, ret);
    memset(rst, 0, 1024);
    strcpy(rst, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src1, src1len);
    memset(src1, 0, 1024);
    strcpy(src1, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, src2, src2len);
    memset(src2, 0, 1024);
    strcpy(src2, temp);

    sprintf(pasOutInfo, "Bcd1:%s \rBcd2:%s \rBcdRst:%s\rreturn:%d", src1, src2, rst, ret);

    sdkFreeMem(src1);
    sdkFreeMem(src2);
    sdkFreeMem(rst);
    sdkFreeMem(temp);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkAscToBcdRManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024];
//    u8 rst[1024] = {0};
//    u8 dst[1024] = {0};
    u8 *rst = NULL, *dst = NULL, *temp = NULL;
    s32 ret = 0;
    u32 asclen = 0;
    s32 len = 0;
    u8 style = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    rst = sdkGetMem(1024);
    if (rst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    dst = sdkGetMem(1024);
    if (dst == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(rst);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(dst, 0, 1024);
    memset(rst, 0, 1024);

    asclen = sdkTestInputASC(dst, "sdkAscToBcd", "ASC:", 1, 50);
    style = sdkTestInputs32(pTitle, "0-AscToBcd 1-AscToBcdR");
    len = sdkTestInputs32(pTitle, "len:");

    if (0 == style) {
        ret = sdkAscToBcd(rst, dst, len);
    }
    else {
        ret = sdkAscToBcdR(rst, dst, len);
    }
    sdkTestPintHex(temp, rst, ret);

    sprintf(pasOutInfo, "Asc:%s \rlen:%d \rBcd:%s\rreturn:%d", dst, len, temp, ret);

    sdkFreeMem(dst);
    sdkFreeMem(rst);
    sdkFreeMem(temp);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkCalcCUSUMManual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 rstbcd[1024] = {0};
//    u8 rstasc[1024] = {0};
    u8 *temp = NULL, *rstbcd = NULL, *rstasc = NULL;
    s32 ret = 0;

    u32 bcdlen = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    rstbcd = sdkGetMem(1024);
    if (rstbcd == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    rstasc = sdkGetMem(1024);
    if (rstasc == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(rstbcd);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(rstbcd, 0, 1024);
    memset(rstasc, 0, 1024);

    bcdlen = sdkTestInputBCD("CalcCUSUM����", rstbcd, "Hex:");
    len = sdkTestInputs32(pTitle, "len:");
    sdkTestPintHex(rstasc, rstbcd, bcdlen);
    ret = sdkCalcCUSUM(rstbcd, len);

    sprintf(pasOutInfo, "Hex:%s \rsrc len:%d \rU32rst:%d\rreturn:%d", rstasc, len, ret, ret);

    sdkFreeMem(temp);
    sdkFreeMem(rstbcd);
    sdkFreeMem(rstasc);

    return 0;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdksdkCalcCrc16Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp[1024] = {0};
//    u8 rstbcd[1024] = {0};
//    u8 rstasc[1024] = {0};
    u8 *temp = NULL, *rstbcd = NULL, *rstasc = NULL;
    s32 ret = 0;
    u32 bcdlen;
    s32 len;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    rstbcd = sdkGetMem(1024);
    if (rstbcd == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    rstasc = sdkGetMem(1024);
    if (rstasc == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(rstbcd);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(rstbcd, 0, 1024);
    memset(rstasc, 0, 1024);

    bcdlen = sdkTestInputBCD("CalcCrc16����", rstbcd, "Hex:");
    len = sdkTestInputs32(pTitle, "len:");

//    len = GetLen(len, bcdlen);
    sdkBcdToAsc(rstasc, rstbcd, bcdlen);
    ret = sdkCalcCrc16(rstbcd, len);

    sprintf(pasOutInfo, "Hex:%s \rlen:%d \rCrc:%02x\rreturn:%d", rstasc, len, ret, ret);

    sdkFreeMem(temp);
    sdkFreeMem(rstbcd);
    sdkFreeMem(rstasc);

    return 0;
}

#if 0
/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkEcbManual(char *pasOutInfo, char const*pTitle)
{
    u8 rsc[50] = {0};
    u8 dsc[50] = {0};
    u8 temp[50] = {0};

    InitMem(rsc);
    InitMem(dsc);
    InitMem(temp);

    u16 src1len = sdkTestInputBCD("sdkEcb����", rsc, "Input src1:");
    s32 len = sdkTestInputs32(pTitle, "Input len:");
    u16 ret = 0;

    len = GetLen(len, src1len);
    ret = sdkEcb(dsc, rsc, len);
    sdkBcdToAsc(temp, rsc, src1len);

    sprintf(pasOutInfo, "input:%s \rlen:%d \rreslust:%s\rreturn:%d", temp, src1len, dsc, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE90SDesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;


    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E90S��DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E90S��DES����", temp2, "Input src2:");

    ret = sdkE90S(temp3, temp2, 8, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 8);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE90S3DesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;

    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E90S3DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E90S3DES����", temp2, "Input src2:");

    ret = sdkE90S(temp3, temp2, 16, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 16);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE98SDesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;


    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E98S��DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E98S��DES����", temp2, "Input src2:");

    ret = sdkE98S(temp3, temp2, 8, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 8);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE98S3DesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;

    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E98S3DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E98S3DES����", temp2, "Input src2:");

    ret = sdkE98S(temp3, temp2, 16, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 16);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE99SDesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;


    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E99S��DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E99S��DES����", temp2, "Input src2:");

    ret = sdkE99S(temp3, temp2, 8, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 8);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE99S3DesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;

    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E99S3DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E99S3DES����", temp2, "Input src2:");

    ret = sdkE99S(temp3, temp2, 16, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 16);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE919SDesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;


    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E919S��DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E919S��DES����", temp2, "Input src2:");

    ret = sdkE919S(temp3, temp2, 8, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 8);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkE919S3DesManual(char *pasOutInfo, char const*pTitle)
{
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};
    u8 temp3[30] = {0};
    u8 temp[50] = {0};
    s32 ret = 0;
    u8 temp1len = 0;
    u8 temp2len = 0;

    InitMem(temp1);
    InitMem(temp2);
    InitMem(temp3);
    InitMem(temp);
    temp1len = sdkTestInputBCD("E919S3DES����", temp1, "Input src1:");
    temp2len = sdkTestInputBCD("E919S3DES����", temp2, "Input src2:");

    ret = sdkE919S(temp3, temp2, 16, temp1, SDK_DES);

    InitMem(temp);
    sdkTestPintHex(temp, temp3, 16);
    strcpy(temp3, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp2, temp1len);
    strcpy(temp2, temp);

    InitMem(temp);
    sdkTestPintHex(temp, temp1, temp2len);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "src data:%s \rlen:%s \rreslust:%s\rreturn:%d", temp1, temp2, temp3, ret);
}

#endif //endif 0

/*****************************************************************************
** Descriptions:
** Parameters:          u8*pOut
                               u8 const *pasInfo
** Returned value:
** Created By:		fusuipu  2013.01.25
** Remarks:
*****************************************************************************/
static s32 sdkPintHex(u8 *pDst, u8 *pSrc, s32 siLen) {
    u16 i = 0;
    u8 buf[256] = {0};

    for (i = 0; i < siLen; i++) {
        memset(buf, 0, sizeof(buf));
        sprintf(buf, "%02X ", pSrc[i]);
        strcat(pDst, buf);
    }

    return 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMD5Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL, *temp2 = NULL;
    s32 ret = 0;
    u8 temp1len = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(temp1);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    temp1len = sdkTestInputASC(temp1, "sdkMD5����", "Asc:", 1, 50);

    len = sdkTestInputs32(pTitle, "Asc len:");

    ret = sdkMD5(temp2, temp1, len);


    memset(temp, 0, 1024);
    sdkPintHex(temp, temp2, 30);
    memset(temp2, 0, 1024);
    strcpy(temp2, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, temp1, temp1len);
    memset(temp1, 0, 1024);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "Asc:%s \rAsc len:%d \rHex:%s\rreturn:%d", temp1, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp1);
    sdkFreeMem(temp2);

    return 0;
}

#if 1

/*****************************************************************************
** Descriptions:
** Parameters:          s32
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkSHA1Manual(char *pasOutInfo, char const *pTitle) {
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL, *temp2 = NULL;
    s32 ret = 0;
    u8 temp1len = 0;
    s32 len = 0;

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        sdkFreeMem(temp1);
        return -1;
    }

    memset(temp, 0, 1024);
    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);

    temp1len = sdkTestInputBCD("sdkSHA1����", temp1, "Hex:");
    len = sdkTestInputs32(pTitle, "Hex len:");


    ret = sdkSHA1(temp1, len, temp2);

    memset(temp, 0, 1024);
    sdkPintHex(temp, temp2, 30);
    memset(temp2, 0, 1024);
    strcpy(temp2, temp);

    memset(temp, 0, 1024);
    sdkTestPintHex(temp, temp1, temp1len);
    memset(temp1, 0, 1024);
    strcpy(temp1, temp);

    sprintf(pasOutInfo, "Hex:%s \rHexlen:%d \rHexRst:%s\rreturn:%d", temp1, len, temp2, ret);

    sdkFreeMem(temp);
    sdkFreeMem(temp1);
    sdkFreeMem(temp2);

    return 0;
}

#endif

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.01.31
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP MathManaulMap[] =
        {
                {"sdkU8ToBcd",      Test_sdkU8ToBcdManual},
//    {"sdkMathsGetVersion",Test_sdkMathsGetVersionManual}, //fusuipu modefied at 2013-1-30
                {"sdkSHA1",         Test_sdkSHA1Manual},
                {"sdkMD5",          Test_sdkMD5Manual},
//    {"sdkE919S3Des", Test_sdkE98S3DesManual},
//    {"sdkE919SDes", Test_sdkE98SDesManual},
//    {"sdkE99S3Des", Test_sdkE98S3DesManual},
//    {"sdkE99SDes", Test_sdkE98SDesManual},
//    {"sdkE98S3Des", Test_sdkE98S3DesManual},
//    {"sdkE98SDes", Test_sdkE98SDesManual},
//    {"sdkE90S3Des", Test_sdkE90S3DesManual},
//    {"sdkE90SDes", Test_sdkE90SDesManual},
                {"sdksdkCalcCrc16", Test_sdksdkCalcCrc16Manual},
                {"sdkCalcCUSUM",    Test_sdkCalcCUSUMManual},
                {"sdkAscToBcdR",    Test_sdkAscToBcdRManual},
                {"sdkBcdSub",       Test_sdkBcdSubManual},
                {"sdkBcdAdd",       Test_sdkBcdAddManual},
                {"sdkXOR8",         Test_sdkXOR8Manual},
                {"sdkAscToBcd",     Test_sdkAscToBcdManual},
                {"sdkBcdToAsc",     Test_sdkBcdToAscManual},
                {"sdkBcdToFloat",   Test_sdkBcdToFloatManual},
                {"sdkU32ToHex",     Test_sdkU32ToHexManual},
                {"sdkU16ToHex",     Test_sdkU16ToHexManual},
                {"sdkHexToU32",     Test_sdkHexToU32Manual},
                {"sdkHexToU16",     Test_sdkHexToU16Manual},
                {"sdkBcdToU32",     Test_sdkBcdToU32Manual},
                {"sdkBcdToU16",     Test_sdkBcdToU16Manual},
                {"sdkBcdToU8",      Test_sdkBcdToU8Manual},
                {"sdkU32ToBcd",     Test_sdkU32ToBcdManual},
                {"sdkU16ToBcd",     Test_sdkU16ToBcdManual}
        };

TEST_MAP_INFO GetMathManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(MathManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = MathManaulMap;
    return ret;
}

