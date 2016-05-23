#include "sdkGlobal.h"
#include "SdkTest.h"

#if 1

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
#if 0 /*Modify by fusuipu at 2013.01.29  20:11 */
static void Test_sdkMathsGetVersion(void)   //  1
{
    u8 temp[30] = {0};

    TestLog("TestMath", " \r\n \r\n(sdkMathsGetVersion()) starts  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkMathsGetVersion(NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��������", (sdkMathsGetVersion(temp) == strlen(temp)));
    TEST_IS_TRUE_WITH_INFO("�����֤", (0 == memcmp("V120515101", temp, strlen(temp))));

    TestLog("TestMath", "��ѧ��汾=  %s\r\n", temp);

    TestLog("TestMath", " \r\n \r\n sdkMathsGetVersion������� \r\n \r\n");
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkU8ToBcd(void)   //  2
{
//    u8 temp[1024];
    u8 *temp = NULL;
    u16 u = 0;
    u32 uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n ---------Test_sdkU8ToBcd-----------  \r\n");

    TestLog("TestMath", "\r\n s32 sdkU8ToBcd() starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    u = 56;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU8ToBcd(NULL, u, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU8ToBcd(temp, u, -1) == SDK_PARA_ERR));

    u = 12;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��������", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x12", 2) == 0);
    TestLogHex("TestMath", "����� \r\n", temp, 2);


    u = 255;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x02\x55", 2) == 0);

    u = 0;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00", 2) == 0);

    u = 256;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("�������", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00", 2) == 0);

    u = 257;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("�������", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x01", 2) == 0);

    u = 260;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("�������", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x04", 2) == 0);

    u = 123;
    uLen = 0;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00", 2) == 0);

    u = 123;
    uLen = 100;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("����Ϊ100����", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(&temp[98], "\x01\x23", 2) == 0);

    u = 123;
    uLen = 1;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("����С����ʵ���ݳ��Ȳ���", (sdkU8ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "����� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x23", 1) == 0);

    sdkFreeMem(temp);
    TestLog("TestMath", " \r\n sdkU8ToBcd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkU16ToBcd(void)   //  3
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u16 u = 0;
    u32 uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n s32 sdkU16ToBcd() starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

//    Assert(0);
    u = 56;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU16ToBcd(NULL, u, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����С��0����", (sdkU16ToBcd(temp, u, -1) == SDK_PARA_ERR));


    u = 56;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��������", (sdkU16ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ����  \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x56", 2) == 0);

    u = 65535;
    uLen = 3;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO(" ���ֵ����", (sdkU16ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 3);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x06\x55\x35", 3) == 0);


    u = 0;
    uLen = 2;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkU16ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ����  \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00", 2) == 0);

    sdkFreeMem(temp);
    TestLog("TestMath", " \r\n sdkU16ToBcd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkU32ToBcd(void)   //  4
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u32 u = 0, uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n s32 sdkU32ToBcd() starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    u = 56;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU32ToBcd(NULL, u, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU32ToBcd(temp, u, -1) == SDK_PARA_ERR));

    u = 59567;
    uLen = 3;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��������", (sdkU32ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 3);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x05\x95\x67", 3) == 0);


    u = 2147483647; // 4294967295
    uLen = 5;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkU32ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, uLen);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x21\x47\x48\x36\x47", uLen) == 0);

    u = 0;
    uLen = 3;
    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkU32ToBcd(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 3);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00\x00", 3) == 0);

    sdkFreeMem(temp);
    TestLog("TestMath", "\r\n ---------Test_sdkU32ToBcd-----------  \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdToU8(void)   //  5
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;
    u8 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n s32 sdkBcdToU8() starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdToU8(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToU8(temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToU8(temp, temp1, -1) == SDK_PARA_ERR));


    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x02;
    temp[1] = 0x55;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkBcdToU8(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 255);


    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkBcdToU8(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 0);

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    temp[1] = 0x57;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToU8(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 57);

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x34;
    temp[1] = 0x56;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToU8(&u, temp, uLen) == SDK_PARA_ERR));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u < 255);

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n sdkBcdToU8()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdToU16(void)   //  6
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;
    u16 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdToU16 starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdToU16(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToU16((u16 *) temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", (sdkBcdToU16((u16 *) temp, temp1, 0) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("���ȹ�������", (sdkBcdToU16((u16 *) temp, temp1, 10) == SDK_PARA_ERR));


    uLen = 3;
    memset(temp, 0, 1024);
    temp[0] = 0x06;
    temp[1] = 0x55;
    temp[2] = 0x35;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkBcdToU16(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", (u == 65535));

    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkBcdToU16(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", (u == 0));

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x12;
    temp[1] = 0x34;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToU16(&u, temp, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�����֤", (u == 1234));

    uLen = 3;
    memset(temp, 0, 1024);
    temp[0] = 0x12;
    temp[1] = 0x34;
    temp[2] = 0x56;
    TEST_IS_TRUE_WITH_INFO("�������", (sdkBcdToU16(&u, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����֤", (u < 65536));

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", "����� %d\r\n", u);
    TestLog("TestMath", " \r\n sdkBcdToU16()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdToU32(void)   //  7
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 u = 0, uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdToU32 starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdToU32(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToU32((u32 *) temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToU32((u32 *) temp, temp1, -1) == SDK_PARA_ERR));


    uLen = 5;
    memset(temp, 0, 1024);
    temp[0] = 0x21;
    temp[1] = 0x47;
    temp[2] = 0x48;
    temp[3] = 0x36;
    temp[4] = 0x47;

    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkBcdToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 2147483647);

    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkBcdToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 0);

    uLen = 3;
    memset(temp, 0, 1024);
    temp[0] = 0x01;
    temp[1] = 0x23;
    temp[2] = 0x45;

    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 12345);

#if 0 /*Modify by fusuipu at 2013.08.21  11:30 */
    uLen = 5;
    memset(temp, 0, 1024);
    temp[0] = 0x21;
    temp[1] = 0x47;
    temp[2] = 0x48;
    temp[3] = 0x36;
    temp[4] = 0x48;

    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkBcdToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %d\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", (u != 2147483648));
#endif /* if 0 */

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n sdkBcdToU32()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkHexToU16(void)   //  8
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u32 uLen = 0;
    u16 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n Test_sdkHexToU16 starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkHexToU16(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkHexToU16((u16 *) temp, NULL, uLen) == SDK_PARA_ERR));

    uLen = 3;
    memset(temp, 0, 1024);
    temp[0] = 0x98; // Դ ���ݴ���65535 , �쳣
    temp[1] = 0x76;
    temp[2] = 0x54;

//    TEST_IS_TRUE_WITH_INFO("��������Ĳ���", (sdkHexToU16(&u, temp, uLen) == SDK_PARA_ERR));

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0xFF;
    temp[1] = 0xFF;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkHexToU16(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "�����%x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 65535);


    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkHexToU16(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "�����%x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 0);

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x01;
    temp[1] = 0x17;

    TEST_IS_TRUE_WITH_INFO("��������", (sdkHexToU16(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 279);

    sdkFreeMem(temp);
    TestLog("TestMath", " \r\n Test_sdkHexToU16()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkHexToU32(void)   //  9
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;
    u32 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n Test_sdkHexToU32 starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkHexToU32(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkHexToU32((u32 *) temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����С��0����", (sdkHexToU32((u32 *) temp, temp1, -1) == SDK_PARA_ERR));


    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0xFF;
    temp[1] = 0xFF;
    temp[2] = 0xFF;
    temp[3] = 0xFF;
    TEST_IS_TRUE_WITH_INFO("�������", (sdkHexToU32(&u, temp, uLen) == SDK_OK));

    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkHexToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "�����%x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 0);

    uLen = 4;
    memset(temp, 0, 1024);
    temp[0] = 0x7F;
    temp[1] = 0xFF;
    temp[2] = 0xFF;
    temp[3] = 0xFF;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkHexToU32(&u, temp, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�����֤", (u == 2147483647));


    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x01;
    temp[1] = 0x18;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkHexToU32(&u, temp, uLen) == SDK_OK));
    TestLog("TestMath", "����� %x\r\n", u);
    TEST_IS_TRUE_WITH_INFO("�����֤", u == 280);

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n Test_sdkHexToU32()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkU16ToHex(void)   //  10
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u32 uLen = 0;
    u32 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkU16ToHex starts  \r\n");
    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    uLen = 2;
    u = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU16ToHex(NULL, u, uLen) == SDK_PARA_ERR));

    u = 65535;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkU16ToHex(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\xFF\xFF", uLen) == 0);

    u = 0;
    uLen = 1;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkU16ToHex(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 1);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00", 1) == 0);

    u = 65536;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("�������", (sdkU16ToHex(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ����  \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00\x00", 2) == 0);

    sdkFreeMem(temp);
    temp = NULL;

    TestLog("TestMath", " \r\n sdkU16ToHex()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkU32ToHex(void)   //  11
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u32 uLen = 0;
    u32 u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkU32ToHex starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    uLen = 2;
    u = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkU32ToHex(NULL, u, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����С��0����", (sdkU32ToHex(temp, u, -1) == SDK_PARA_ERR));

    u = 65535;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkU32ToHex(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\xFF\xFF", 2) == 0);

    u = 0;
    uLen = 1;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkU32ToHex(temp, u, uLen) == uLen));
    TestLogHex("TestMath", "���Խ���� \r\n", temp, 1);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x00", 1) == 0);

    u = 279;
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkU32ToHex(temp, u, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x01\x17", 2) == 0);

    u = 12345;
    uLen = 1;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkU32ToHex(temp, u, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp, "\x39", 1) == 0);

    sdkFreeMem(temp);
    temp = NULL;

    TestLog("TestMath", " \r\n sdkU32ToHex()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdToFloat(void)   //  12
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;
    double u = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n Test_sdkBcdToFloat starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdToFloat(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           (sdkBcdToFloat((double *) temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����",
                           (sdkBcdToFloat((double *) temp, temp1, -1) == SDK_PARA_ERR));

    uLen = 3;
    memset(temp, 0, 1024);
    temp[0] = 0x02;
    temp[1] = 0x34;
    temp[2] = 0x56;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToFloat(&u, temp, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("��������", (abs(u - 23456.000000) < 0.0001));

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x02;
    temp[1] = 0x34;
    temp[2] = 0x56;
    TEST_IS_TRUE_WITH_INFO("���ȹ��̲���", (sdkBcdToFloat(&u, temp, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("��������", (abs(u - 234.000000) < 0.0001));

    uLen = 4;
    memset(temp, 0, 1024);
    temp[0] = 0x02;
    temp[1] = 0x34;
    temp[2] = 0x56;
    TEST_IS_TRUE_WITH_INFO("���ȹ�������", (sdkBcdToFloat(&u, temp, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("��������", (abs(u - 2345600) < 0.0001));


    TestLog("TestMath", "���Խ����  %6.5d\r\n", u);

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n Test_sdkBcdToFloat()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdToAsc(void)   //  13
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdToAsc starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdToAsc(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdToAsc(temp, NULL, uLen) == SDK_PARA_ERR));

    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0xFF;

    TEST_IS_TRUE_WITH_INFO("���ֵ����", (sdkBcdToAsc(temp1, temp, uLen) == 2 * uLen));
    TestLogHex("TestMath", "���Խ����\r\n", temp1, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp1, "FF", 2) == 0);

    uLen = 1;
    memset(temp, 0, 1024);
    temp[0] = 0x00;

    TEST_IS_TRUE_WITH_INFO("��Сֵ����", (sdkBcdToAsc(temp1, temp, uLen) == 2 * uLen));
    TestLogHex("TestMath", "���Խ����\r\n", temp1, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp1, "00", 2) == 0);

    uLen = 2;
    memset(temp, 0, 1024);
    temp[0] = 0x23;
    temp[1] = 0xaf;

    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdToAsc(temp1, temp, uLen) == 2 * uLen));
    TestLogHex("TestMath", "���Խ����  \r\n", temp1, 4);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp1, "23AF", 4) == 0);

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n sdkBcdToAsc()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdCardNotoAsc(void)   //  14
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdCardNotoAsc starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkBcdCardNumToAsc(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkBcdCardNumToAsc(temp, NULL, uLen) == SDK_PARA_ERR));

    uLen = 8;  // ��������
    memset(temp, 0, 1024);
    temp[0] = 0x12;
    temp[1] = 0x34;
    temp[2] = 0x56;
    temp[3] = 0x78;
    temp[4] = 0x90;
    temp[5] = 0XD1;
    temp[6] = 0x23;
    temp[7] = 0x45;

    TEST_IS_TRUE_WITH_INFO("��������", (sdkBcdCardNumToAsc(temp1, temp, uLen) == 2 * uLen));
    TEST_IS_TRUE_WITH_INFO("�������", (memcmp(temp1, "1234567890=12345", 11) == 0));

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkAscToBcd(void)   //  15
{
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
    u8 *temp = NULL, *temp1 = NULL;
    u32 uLen = 0;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkAscToBcd starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkAscToBcd(NULL, temp, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkAscToBcd(temp, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("0����", (sdkAscToBcd(temp, temp1, -1) == SDK_PARA_ERR));

    uLen = 2;
    memset(temp, 0, 1024);
    memcpy(temp, "wrt", sizeof("wrt"));
    memset(temp1, 0, 1024);

    TEST_IS_TRUE_WITH_INFO("��BCD�����",
                           (sdkAscToBcd(temp1, temp, (uLen + 1) / 2) == SDK_PARA_ERR));

    uLen = 3;
    memset(temp1, 0, 1024);
    memset(temp, 0, 1024);
    strcpy(temp, "ab1");
    TEST_IS_TRUE_WITH_INFO("��������", (sdkAscToBcd(temp1, temp, uLen) == ((uLen + 1) / 2)));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp1, "\xab\x10", 2) == 0);

    uLen = 3;
    memset(temp1, 0, 1024);
    memset(temp, 0, 1024);
    strcpy(temp, "ab11122");
    TEST_IS_TRUE_WITH_INFO("���ȹ��̲���", (sdkAscToBcd(temp1, temp, uLen) == ((uLen + 1) / 2)));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp1, "\xab\x10", 2) == 0);

    uLen = 9;
    memset(temp1, 0, 1024);
    memset(temp, 0, 1024);
    strcpy(temp, "ab11122");
    TEST_IS_TRUE_WITH_INFO("���ȹ�������", (sdkAscToBcd(temp1, temp, uLen) == SDK_PARA_ERR));

    sdkFreeMem(temp);
    temp = NULL;

    sdkFreeMem(temp1);
    temp1 = NULL;

    TestLog("TestMath", " \r\n sdkAscToBcd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkIsAscii(void)   //  16
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkIsAscii starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkIsAscii(NULL) == false));

    memset(temp, 0, 1024);
    TEST_IS_TRUE_WITH_INFO("ȫ0����", (sdkIsAscii(temp) == false));

    memset(temp, 0, 1024);
    memcpy(temp, "hello", sizeof("hello"));
    TEST_IS_TRUE_WITH_INFO("��������", (sdkIsAscii(temp) == true));

    sdkFreeMem(temp);
    temp = NULL;

    TestLog("TestMath", " \r\n sdkIsAscii()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}


/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkXOR8(void)   //  17
{
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkXOR8 starts  \r\n");

    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;

        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkXOR8(NULL, temp2, temp3) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkXOR8(temp1, NULL, temp3) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3", (sdkXOR8(temp1, temp2, NULL) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "ABCDE");
    strcpy(temp2, "4567");
    TEST_IS_TRUE_WITH_INFO("��������", (sdkXOR8(temp3, temp1, temp2) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("������Բ���", (memcmp(temp3, "\x45\x47\x45\x43\x45", 5)));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    memcpy(temp1, "\x12\x23\x45\x78\x12\x23\x56\x78\x12", 9);
    memcpy(temp2, "\x45\x78\x12\x23\x56\x78\x12\x12\x23", 9);

    TEST_IS_TRUE_WITH_INFO("���ȳ���8�ֽڲ���", (sdkXOR8(temp3, temp1, temp2) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("������Բ���",
                           (memcmp(temp3, "\x57\x5B\x57\x5B\x44\x5B\x44\x6A", 8) == 0));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    memcpy(temp1, "\x12\x23\x45\x78\x12\x23", 6);
    memcpy(temp2, "\x45\x78\x12\x23\x56", 5);

    Trace("bt_test", "1024=%d\n", 1024);

    TEST_IS_TRUE_WITH_INFO("����С��8�ֽڲ���", (sdkXOR8(temp3, temp1, temp2) == SDK_OK));
    TraceHex("bt_test", "temp3=\n", temp3, 8);
    TEST_IS_TRUE_WITH_INFO("������Բ���",
                           (memcmp(temp3, "\x57\x5B\x57\x5B\x44\x23\x00\x00", 8) == 0));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdAdd(void)   //  18
{
    u32 uLen1;
    u32 uLen2;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdAdd starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    uLen1 = 2;
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           (sdkBcdAdd(NULL, temp1, uLen1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           (sdkBcdAdd(temp3, NULL, uLen1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����1<0����",
                           (sdkBcdAdd(temp3, temp1, -1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           (sdkBcdAdd(temp3, temp1, uLen1, NULL, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����2<0����",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, -3) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x99\x99");
    uLen1 = 2;
    strcpy(temp2, "\x12\x34");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("���ֵ����",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TestLogHex("TestMath", " \r\n ���is  \r\n", temp3, 2);
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x12\x33", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x34");
    uLen1 = 2;
    strcpy(temp2, "\x56");
    uLen2 = 1;
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x12\x90", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x99\x99");
    uLen1 = 2;
    strcpy(temp2, "\x01");
    uLen2 = 1;
    TEST_IS_TRUE_WITH_INFO("�������",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x00\x00", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\xFA\x99");
    uLen1 = 2;
    strcpy(temp2, "\x01");
    uLen2 = 1;
    TEST_IS_TRUE_WITH_INFO("��չBCD����",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x23");
    uLen1 = 2;
    strcpy(temp2, "\x34\x45");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("�������",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x46\x68", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x23\x12");
    uLen1 = 5;
    strcpy(temp2, "\x34\x45");
    uLen2 = 6;
    TEST_IS_TRUE_WITH_INFO("���ȴ���ʵ�����ݳ���",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x34\x57\x23\x12\x00\x00", 6) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x23\x12\x14\x56");
    uLen1 = 1;
    strcpy(temp2, "\x34\x45\x12\x23");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("����С��ʵ�����ݳ���",
                           (sdkBcdAdd(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x34\x57", 2) == 0);

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;

    TestLog("TestMath", " \r\n sdkBcdAdd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkBcdSub(void)   //  19
{
    u32 uLen1;
    u32 uLen2;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkBcdSub starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    uLen1 = 2;
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           (sdkBcdSub(NULL, temp1, uLen1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           (sdkBcdSub(temp3, NULL, uLen1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����1<0����",
                           (sdkBcdSub(temp3, temp1, -1, temp2, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           (sdkBcdSub(temp3, temp1, uLen1, NULL, uLen2) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����2<0����",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, -2) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x00\x00");
    uLen1 = 2;
    strcpy(temp1, "\x12\x34");
    uLen1 = 2;
    TEST_IS_TRUE_WITH_INFO("��Сֵ����",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TestLogHex("TestMath", " \r\n ����� \r\n", temp3, 2);


    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x34");
    uLen1 = 2;
    strcpy(temp2, "\x56");
    uLen2 = 1;
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x11\x78", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x34");
    uLen1 = 2;
    strcpy(temp2, "\x56\x78");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("�������",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x55\x56", 2) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x12\x34");
    uLen1 = 2;
    strcpy(temp2, "\x56\xFF");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("A-F��BCD�����",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x55\x66\x77");
    uLen1 = 3;
    strcpy(temp2, "\x11\x22");
    uLen2 = 2;
    TEST_IS_TRUE_WITH_INFO("���Ȳ��Ȳ���",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x55\x55\x55", 3) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x55\x66\x77");
    uLen1 = 4;
    strcpy(temp2, "\x11\x22");
    uLen2 = 3;
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x55\x55\x55\x00", 4) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "\x55\x66\x77");
    uLen1 = 2;
    strcpy(temp2, "\x11\x22");
    uLen2 = 1;
    TEST_IS_TRUE_WITH_INFO("����С�����ݳ��Ȳ���",
                           (sdkBcdSub(temp3, temp1, uLen1, temp2, uLen2) == (uLen1 > uLen2) ? uLen1
                                                                                            : uLen2));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp3, "\x55\x55", 2) == 0);

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;

    TestLog("TestMath", " \r\n sdkBcdAdd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkAscToBcdR(void)   //  20
{
    u8 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkAscToBcdR starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����1", (sdkAscToBcdR(NULL, temp1, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (sdkAscToBcdR(temp2, NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("���ݳ���С��0����", (sdkAscToBcdR(temp2, temp1, -1) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "\x80\xff");
    uLen = 2; //sjl 2012.08.09 15:2
    TEST_IS_TRUE_WITH_INFO("��ASCII��", (sdkAscToBcdR(temp2, temp1, uLen) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "abcd1");
    uLen = 3; //sjl 2012.08.09 14:59
    TEST_IS_TRUE_WITH_INFO("��������1", (sdkAscToBcdR(temp2, temp1, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp2, "\x0a\xbc\xd1", 3) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "abcd12");
    uLen = 0;
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", (sdkAscToBcdR(temp2, temp1, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp2, "\x00\x00\x00", 3) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "abcd12");
    uLen = 10;
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���", (sdkAscToBcdR(temp2, temp1, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤",
                           memcmp(temp2, "\x00\x00\x00\x00\x00\x00\x00\xAB\xCD\x12", 10) == 0);

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "abcd12");
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���", (sdkAscToBcdR(temp2, temp1, uLen) == uLen));
    TEST_IS_TRUE_WITH_INFO("�����֤", memcmp(temp2, "\xCD\x12", 2) == 0);

    sdkFreeMem(temp1);
    temp1 = NULL;

    sdkFreeMem(temp2);
    temp2 = NULL;

    TestLog("TestMath", " \r\n sdkAscToBcdR()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkCalcBcc(void)   //  21
{
    u32 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkCalcBcc starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCalcCUSUM(NULL, uLen) == SDK_PARA_ERR));
    uLen = -3;
    TEST_IS_TRUE_WITH_INFO("����Ϊ0�Ĳ���", (sdkCalcCUSUM(temp2, uLen) == SDK_PARA_ERR));


    memset(temp1, 0, 1024);
    strcpy(temp1, "\x08\x02\x10");
    uLen = 4;
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���", ((26 == sdkCalcCUSUM(temp1, uLen))));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
}


/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkCalcAppCrc(void)   //  22
{
    u32 uLen = 0;
    s16 sRslt;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkCalcCrc16 starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCalcCrc16(NULL, uLen) == SDK_PARA_ERR));
    uLen = 0;
    TEST_IS_TRUE_WITH_INFO("����Ϊ0�Ĳ���", (sdkCalcCrc16(temp2, uLen) == SDK_PARA_ERR));

    memset(temp1, 0, 1024);
    memcpy(temp1, "\xA\x1B\x23", 3);
    uLen = 3;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkCalcCrc16(temp1, uLen) == 55873));

    memset(temp1, 0, 1024);
    memcpy(temp1, "\xA\x1B\x23", 3);
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("����С�����ݳ��Ȳ���", ((sRslt = sdkCalcCrc16(temp1, uLen)) == 15450));

    memset(temp1, 0, 1024);
    memcpy(temp1, "\xA\x1B\x23", 3);
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("����С�����ݳ��Ȳ���", ((sRslt = sdkCalcCrc16(temp1, uLen)) == 15450));

    memset(temp1, 0, 1024);
    memcpy(temp1, "\x12\x34", 2);
    uLen = 5;
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���", ((sRslt = sdkCalcCrc16(temp1, uLen)) == 6009));


    TestLog("TestMath", " \r\n �����  %d \r\n", sRslt);
    TestLog("TestMath", " \r\n sdkCalcCrc16()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkEcb(void)   //  23
{
    u32 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkEcb starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    memset(temp1, 0, 1024);
    strcpy(temp1, "16");
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��������", (sdkEcb(temp2, temp1, uLen) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("���Խ��", (memcmp(temp2, "31360000", 8) == 0));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkE90S(void)   //  24
{
    u16 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkE90S starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           ((sdkE90S(NULL, temp2, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           ((sdkE90S(temp3, NULL, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           ((sdkE90S(temp3, temp2, uLen, NULL, SDK_3DES) == SDK_PARA_ERR)));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "12345678");
    strcpy(temp2, "12345678");
    uLen = 8;
    TEST_IS_TRUE_WITH_INFO("��DES����", ((sdkE90S(temp3, temp2, uLen, temp1, SDK_DES) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("��DES�������",
                           (memcmp(temp3, "\x42\x30\x36\x43\x34\x34\x35\x45", 8) == 0));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memset(temp3, 0, 1024);
    strcpy(temp1, "1234567812345678");
    strcpy(temp2, "1234567812345678");
    uLen = 16;
    TEST_IS_TRUE_WITH_INFO("3DES����", ((sdkE90S(temp3, temp2, uLen, temp1, SDK_3DES) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("3DES�������",
                           (memcmp(temp3, "\x38\x36\x36\x44\x43\x37\x38\x38", 8) == 0));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkE98S(void)   //  25
{
//    u8 temp1[1024] = {0x31};
//    u8 temp2[1024] = {2};
//    u8 temp3[1024] = {0};
//    u8 temp4[1024] = {4};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL, *temp4 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkE98S starts  \r\n");

    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }
    temp4 = sdkGetMem(1024);
    if (temp4 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        sdkFreeMem(temp3);
        temp3 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           ((sdkE98S(NULL, temp3, temp2, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           ((sdkE98S(temp4, NULL, temp2, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           ((sdkE98S(temp4, temp3, NULL, temp1, SDK_3DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����4",
                           ((sdkE98S(temp4, temp3, temp2, NULL, SDK_3DES) == SDK_PARA_ERR)));

    memset(temp1, 0x31, 30);
    memset(temp2, 0, 1024);
    strcpy(temp2, "\x11\x11\x11\x11\x11\x11\x11\x11");

    memset(temp3, 0, 1024);
    strcpy(temp3, "000000");
    //TEST_IS_TRUE_WITH_INFO("��������", (((rslt = sdkE98S(temp4, temp3, temp2, temp1, SDK_DES)) == SDK_OK)));
    sdkE98S(temp4, temp3, temp2, temp1, SDK_DES);
    TEST_IS_TRUE_WITH_INFO("�������", (memcmp(temp4, "\xd2\xab\x38\x00\x8b\xdc\x3b\xea", 8) == 0));

    memset(temp2, 0x11, 8);
    memset(&temp1[8], 0x32, 8);
    strcpy(temp3, "00000000");
    //TEST_IS_TRUE_WITH_INFO("��������", (((rslt = sdkE98S(temp4, temp3, temp2, temp1, SDK_3DES)) == SDK_OK)));
    sdkE98S(temp4, temp3, temp2, temp1, SDK_3DES);
    //TEST_IS_TRUE_WITH_INFO("�������", (memcmp(temp4, "\x7c\x05\xdd\xa7\x2e\x7f\xa0\x18", 8) == 0));
    TEST_IS_TRUE_WITH_INFO("�������", (memcmp(temp4, "\x7b\xf0\xf9\x29\x51\x71\xaa\x30", 8) == 0));
    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;
    sdkFreeMem(temp4);
    temp4 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkE99S(void)   //  26
{
    u16 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkE99S starts  \r\n");
    temp1 = sdkGetMem(1024);//liuguangpeng
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           ((sdkE99S(NULL, temp2, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           ((sdkE99S(temp3, NULL, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           ((sdkE99S(temp3, temp2, uLen, NULL, SDK_3DES) == SDK_PARA_ERR)));


    memset(temp1, 0, 1024);
    strcpy(temp1, "1234567812345678");
    uLen = 8;

    memset(temp2, 0x30, 30);
    memset(temp3, 0, 30);
    TEST_IS_TRUE_WITH_INFO("��DES����", ((sdkE99S(temp3, temp2, uLen, temp1, SDK_DES) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("��DES�������",
                           (memcmp(temp3, "\x5F\x6A\x7D\x52\x8E\x39\x4F\x39", 8) == 0));

    memset(temp2, 0x30, 30);
    memset(temp3, 0, 30);
    TEST_IS_TRUE_WITH_INFO("3DES����", ((sdkE99S(temp3, temp2, uLen, temp1, SDK_3DES) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("3DES�������",
                           (memcmp(temp3, "\x5F\x6A\x7D\x52\x8E\x39\x4F\x39", 8) == 0));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkE919S(void)   //  27
{
    u16 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
//    u8 temp3[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp3 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkE919S starts  \r\n");

    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }
    temp3 = sdkGetMem(1024);
    if (temp3 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        sdkFreeMem(temp2);
        temp2 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1",
                           ((sdkE919S(NULL, temp2, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2",
                           ((sdkE919S(temp3, NULL, uLen, temp1, SDK_DES) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3",
                           ((sdkE919S(temp3, temp2, uLen, NULL, SDK_3DES) == SDK_PARA_ERR)));


    memset(temp1, 0, 1024);
    strcpy(temp1, "\x01\x06");
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��DES����", ((sdkE919S(temp3, temp2, uLen, temp1, SDK_DES) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("3DES����", ((sdkE919S(temp3, temp2, uLen, temp1, SDK_3DES) == SDK_OK)));
    TestLogHex("TestMath", " \r\n����� \r\n", temp3, 1);
    TestLog("TestMath", " \r\n sdkE919S()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
    sdkFreeMem(temp3);
    temp3 = NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkMD5(void)   //  28
{
    u16 uLen = 0;
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL;

    TestLog("TestMath", "\r\n\r\n");
    TestLog("TestMath", "\r\n sdkE919S starts  \r\n");
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����1", ((sdkMD5(temp1, NULL, uLen) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", ((sdkMD5(NULL, temp2, uLen) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", ((sdkMD5(temp1, temp2, 0) == SDK_PARA_ERR)));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "12");
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��������", ((sdkMD5(temp2, temp1, uLen) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("�������", (memcmp(temp2,
                                              "\xC2\x0A\xD4\xD7\x6F\xE9\x77\x59\xAA\x27\xA0\xC9\x9B\xFF\x67\x10",
                                              16) == 0));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "GHjib");
    uLen = 2;
    TEST_IS_TRUE_WITH_INFO("��������", ((sdkMD5(temp2, temp1, uLen) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("���㳤��С������ʱ�䳤�Ȳ���", (memcmp(temp2,
                                                            "\x68\x48\xAE\x6F\x8E\x78\x60\x62\xF1\xB2\x34\x76\xC9\xEC\xD2\x58",
                                                            16) == 0));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, "1234");
    uLen = 8;
    TEST_IS_TRUE_WITH_INFO("��������", ((sdkMD5(temp2, temp1, uLen) == SDK_OK)));
    TestLogHex("fsp", "temp2:", temp2, 16);
    TEST_IS_TRUE_WITH_INFO("���㳤�ȴ�������ʱ�䳤�Ȳ���", (memcmp(temp2,
                                                            "\xE4\x5D\xC5\x9A\x69\x68\xA0\xC9\xDC\xFB\x94\x4B\x72\x00\x56\x7D",
                                                            16) == 0));

    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    strcpy(temp1, ".,@*#");
    uLen = 5;
    TEST_IS_TRUE_WITH_INFO("��������", ((sdkMD5(temp2, temp1, uLen) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("�����ַ�����", (memcmp(temp2,
                                                  "\x64\x23\x21\xD7\xBA\x2A\x52\xA6\x73\xF5\x1A\x06\x91\x00\x1B\x70",
                                                  16) == 0));

    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
}

static void Test_sdkCalcCUSUM(void)   //  2
{
//    u8 temp[1024] = {0};
    u8 *temp = NULL;
    u32 uLen = 0;

    if (temp == NULL) {
        Assert(0);
        return;
    }

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCalcCUSUM(NULL, uLen) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", (sdkCalcCUSUM(temp, 0) == SDK_PARA_ERR));

    uLen = 2;
    memset(temp, 0, 1024);
    memcpy(temp, "\x12\x34\x56\x78", 1024);
    TEST_IS_TRUE_WITH_INFO("����С�����ݳ��Ȳ���", (sdkCalcCUSUM(temp, uLen) == 70));

    uLen = 2;
    memset(temp, 0, 1024);
    memcpy(temp, "\x12\xD0", 1024);
    TEST_IS_TRUE_WITH_INFO("��������", (sdkCalcCUSUM(temp, uLen) == 226));

    uLen = 5;
    memset(temp, 0, 1024);
    memcpy(temp, "\x12\x34\x56", 1024);
    TEST_IS_TRUE_WITH_INFO("���ȴ������ݳ��Ȳ���", (sdkCalcCUSUM(temp, uLen) == 156));

    sdkFreeMem(temp);

    TestLog("TestMath", " \r\n sdkU8ToBcd()������� \r\n");
    TestLog("TestMath", "\r\n\r\n");
}

#if 1

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkRSAPublicEncrypt(void)   //  29
{
//    u8 temp[512] = {0}, input[512] = {0}, output[512] = {0};
    u8 *temp = NULL, *input = NULL, *output = NULL;
    s32 ret = 0, len = 0, outlen = 0;
    SDK_RSA_PUBLIC_KEY pk;
    SDK_RANDOM_STRUCT rand;
    SDK_RSA_PRIVATE_KEY sk;

    TestLog("rsa", "begin sdkRSAPublicEncrypt\r\n");
    temp = sdkGetMem(512);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    input = sdkGetMem(512);
    if (input == NULL) {
        Assert(0);
        return;
    }
    output = sdkGetMem(512);
    if (output == NULL) {
        Assert(0);
        return;
    }

    memset(input, '1', 6);
    memset(&pk, 0, sizeof(pk));
    memset(&rand, 0, sizeof(rand));
    pk.bits = 512;

    //public key modules
    memcpy(&pk.modulus[sizeof(pk.modulus) - 64],
           "\xB4\xBF\xEE\x4B\xA9\xBE\xCB\x68\xF7\xBB\xF1\x1F\x16\x87\x25\xE7\x2C\xBF\x2B\x11\x43\x3C\x0F\xDF\x29\x64\xD7\xCF\xF5\xAB\xCE\xCE\xA1\xE3\x9A\xF4\x05\xAA\x95\xCD\xEF\x65\x34\x31\x64\xFB\x84\x38\x88\xF2\x4B\x6D\x65\xA4\xEB\x0F\xEF\x47\x65\x16\xCF\x04\x2D\x9F",
           64);
    //public key exponent
    memcpy(&pk.exponent[sizeof(pk.exponent) - 3], "\x01\x00\x01", 3);
    memset(temp, 0, 512);
    ret = sdkRSAPublicEncrypt(temp, &len, input, 6, &pk, &rand); //public key encrypt
    TestLogHex("rsa", "calc rsa ", temp, len);
    TestLog("rsa", "begin sdkRSAPrivateDecrypt\r\n");

    memset(&sk, 0, sizeof(sk));
    ret = 0;
    sk.bits = 512;
    //private key modules
    memcpy(&sk.modulus[sizeof(sk.modulus) - 64],
           "\xB4\xBF\xEE\x4B\xA9\xBE\xCB\x68\xF7\xBB\xF1\x1F\x16\x87\x25\xE7\x2C\xBF\x2B\x11\x43\x3C\x0F\xDF\x29\x64\xD7\xCF\xF5\xAB\xCE\xCE\xA1\xE3\x9A\xF4\x05\xAA\x95\xCD\xEF\x65\x34\x31\x64\xFB\x84\x38\x88\xF2\x4B\x6D\x65\xA4\xEB\x0F\xEF\x47\x65\x16\xCF\x04\x2D\x9F",
           64);
    //private key exponent
    memcpy(&sk.publicExponent[sizeof(sk.exponent) - 3], "\x01\x00\x01", 3);
    memcpy(&sk.exponent[sizeof(sk.exponent) - 64],
           "\xA5\x36\xCA\x9A\x5E\x8E\x07\x5F\xEB\x11\xDF\x54\x8B\x42\xFD\x62\x51\x84\x6B\xD9\x5B\x9B\x8A\xE6\x70\xEF\xBC\x54\x5A\x3A\xB0\xCC\xA3\xD6\xB2\xF7\x4B\x4C\x27\xAA\x6F\x50\xBF\x8E\x8A\xD1\xB2\xD4\x75\x19\x81\x02\xAB\xC9\x57\x02\xB5\x1D\xC1\x49\x3F\x9A\x20\x01",
           64);
    memcpy(&sk.prime[0][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xEE\x79\x23\xAF\xEA\x05\x47\xC6\x38\x82\x9C\xE2\xFF\x57\xF1\xAA\xB9\xB0\x4F\xD4\x1C\x4F\x83\xD6\x41\x9D\x79\xB3\x3E\x99\x2B\xF9",
           32);
    memcpy(&sk.prime[1][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xC2\x08\xBA\x36\xFC\xB9\x99\x8E\xBA\x30\x10\xE1\xC5\x80\x4C\xEB\xB9\x74\x59\x90\x57\xC7\x2B\x46\xC1\x37\xA2\xBE\x5D\x1F\x1C\x57",
           32);
    memcpy(&sk.primeExponent[0][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xE1\x95\xB0\x7F\x99\x45\xB3\x76\xBC\x40\x39\xFE\xB5\x83\x50\xB5\xAA\x80\x31\x93\xB9\xD0\x32\x2B\x7C\xD8\x65\xA6\x7C\xA1\xD2\xA9",
           32);
    memcpy(&sk.primeExponent[1][SDK_MAX_RSA_PRIME_LEN - 32],
           "\x62\xC3\xEC\x7D\x76\xD0\xC6\x70\x17\x27\xC6\x40\x15\xB7\x03\xF0\x6C\xFA\xE3\x68\x0C\xB6\x2D\xBA\x1B\x39\xC5\x96\x1E\x72\x6A\x8B",
           32);
    memcpy(&sk.coefficient[sizeof(sk.coefficient) - 32],
           "\xA4\x7C\xE2\x7C\x7D\x59\xC9\x4A\xB9\x5D\x9A\x66\x3B\xB1\x33\x78\xA5\xD7\x15\xB4\x0C\x04\xB2\x1D\xE4\xE8\x32\xD4\x12\xF4\xE9\x04",
           32);

    ret = sdkRSAPrivateDecrypt(output, &outlen, temp, 64, &sk);

    TestLog("rsa", "ret is %d outlen is %d\r\n", ret, outlen);
    TestLogHex("rsa", "Decrypt rsa ", output, outlen);

    TEST_IS_TRUE_WITH_INFO("��Կ����˽Կ���ܲ���", (memcmp(output, input, 6) == 0));

    TestLog("rsa", "begin sdkRSAPrivateEncrypt\r\n");
    memset(output, 0, 512);
    memset(&sk, 0, sizeof(sk));
    ret = 0;
    sk.bits = 512;
    //private key modules
    memcpy(&sk.modulus[sizeof(sk.modulus) - 64],
           "\xB4\xBF\xEE\x4B\xA9\xBE\xCB\x68\xF7\xBB\xF1\x1F\x16\x87\x25\xE7\x2C\xBF\x2B\x11\x43\x3C\x0F\xDF\x29\x64\xD7\xCF\xF5\xAB\xCE\xCE\xA1\xE3\x9A\xF4\x05\xAA\x95\xCD\xEF\x65\x34\x31\x64\xFB\x84\x38\x88\xF2\x4B\x6D\x65\xA4\xEB\x0F\xEF\x47\x65\x16\xCF\x04\x2D\x9F",
           64);
    //private key exponent
    memcpy(&sk.publicExponent[sizeof(sk.exponent) - 3], "\x01\x00\x01", 3);
    memcpy(&sk.exponent[sizeof(sk.exponent) - 64],
           "\xA5\x36\xCA\x9A\x5E\x8E\x07\x5F\xEB\x11\xDF\x54\x8B\x42\xFD\x62\x51\x84\x6B\xD9\x5B\x9B\x8A\xE6\x70\xEF\xBC\x54\x5A\x3A\xB0\xCC\xA3\xD6\xB2\xF7\x4B\x4C\x27\xAA\x6F\x50\xBF\x8E\x8A\xD1\xB2\xD4\x75\x19\x81\x02\xAB\xC9\x57\x02\xB5\x1D\xC1\x49\x3F\x9A\x20\x01",
           64);
    memcpy(&sk.prime[0][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xEE\x79\x23\xAF\xEA\x05\x47\xC6\x38\x82\x9C\xE2\xFF\x57\xF1\xAA\xB9\xB0\x4F\xD4\x1C\x4F\x83\xD6\x41\x9D\x79\xB3\x3E\x99\x2B\xF9",
           32);
    memcpy(&sk.prime[1][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xC2\x08\xBA\x36\xFC\xB9\x99\x8E\xBA\x30\x10\xE1\xC5\x80\x4C\xEB\xB9\x74\x59\x90\x57\xC7\x2B\x46\xC1\x37\xA2\xBE\x5D\x1F\x1C\x57",
           32);
    memcpy(&sk.primeExponent[0][SDK_MAX_RSA_PRIME_LEN - 32],
           "\xE1\x95\xB0\x7F\x99\x45\xB3\x76\xBC\x40\x39\xFE\xB5\x83\x50\xB5\xAA\x80\x31\x93\xB9\xD0\x32\x2B\x7C\xD8\x65\xA6\x7C\xA1\xD2\xA9",
           32);
    memcpy(&sk.primeExponent[1][SDK_MAX_RSA_PRIME_LEN - 32],
           "\x62\xC3\xEC\x7D\x76\xD0\xC6\x70\x17\x27\xC6\x40\x15\xB7\x03\xF0\x6C\xFA\xE3\x68\x0C\xB6\x2D\xBA\x1B\x39\xC5\x96\x1E\x72\x6A\x8B",
           32);
    memcpy(&sk.coefficient[sizeof(sk.coefficient) - 32],
           "\xA4\x7C\xE2\x7C\x7D\x59\xC9\x4A\xB9\x5D\x9A\x66\x3B\xB1\x33\x78\xA5\xD7\x15\xB4\x0C\x04\xB2\x1D\xE4\xE8\x32\xD4\x12\xF4\xE9\x04",
           32);

    ret = sdkRSAPrivateEncrypt(output, &outlen, input, 6, &sk);
    TestLog("rsa", "sdkRSAPrivateEncrypt ret is %d outlen is %d\r\n", ret, outlen);
    TestLogHex("rsa", "sdkRSAPrivateEncrypt rsa ", output, outlen);

    TestLog("rsa", "begin sdkRSAPrivateDecrypt\r\n");

    memset(temp, 0, 512);
    memset(&pk, 0, sizeof(pk));
    pk.bits = 512;

    //public key modules
    memcpy(&pk.modulus[sizeof(pk.modulus) - 64],
           "\xB4\xBF\xEE\x4B\xA9\xBE\xCB\x68\xF7\xBB\xF1\x1F\x16\x87\x25\xE7\x2C\xBF\x2B\x11\x43\x3C\x0F\xDF\x29\x64\xD7\xCF\xF5\xAB\xCE\xCE\xA1\xE3\x9A\xF4\x05\xAA\x95\xCD\xEF\x65\x34\x31\x64\xFB\x84\x38\x88\xF2\x4B\x6D\x65\xA4\xEB\x0F\xEF\x47\x65\x16\xCF\x04\x2D\x9F",
           64);
    //public key exponent
    memcpy(&pk.exponent[sizeof(pk.exponent) - 3], "\x01\x00\x01", 3);

    ret = sdkRSAPublicDecrypt(temp, &len, output, outlen, &pk);
    TestLog("rsa", "sdkRSAPublicDecrypt ret is %d outlen is %d\r\n", ret, len);
    TestLogHex("rsa", "sdkRSAPublicDecrypt rsa ", temp, len);

    TEST_IS_TRUE_WITH_INFO("˽Կ���ܹ�Կ���ܲ���", (memcmp(temp, input, 6) == 0));

    sdkFreeMem(temp);
    sdkFreeMem(output);
    sdkFreeMem(input);
}

#if 0
/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkRSAPublicDecrypt(void)   //  31
{
    s32 rslt = 0;
    u8 Dest[64] = {0}; s32 Destlen = 64;
    u8 Src[64] = {1, 2, 3}; u32 Srclen = 64;
    SDK_RSA_PUBLIC_KEY PublicKey;

    memset(&PublicKey, 0, sizeof(PublicKey));

    TestLog("TestMath", "\r\n sdkRSAPublicEncrypt starts  \r\n");

    // TEST_IS_TRUE_WITH_INFO("��ָ�����1" , (SDK_PARA_ERR == sdkRSAPublicDecrypt(NULL , &Destlen , Src , Srclen , &PublicKey)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (SDK_PARA_ERR == sdkRSAPublicDecrypt(Dest, &Destlen, NULL, Srclen, &PublicKey)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3", (SDK_PARA_ERR == sdkRSAPublicDecrypt(Dest, &Destlen, Src, Srclen, NULL)));

    TEST_IS_TRUE_WITH_INFO("��������", (rslt = sdkRSAPublicDecrypt(Dest, &Destlen, Src, Srclen, &PublicKey)));

    TestLogHex("TestMath", " \r\n�����\r\n", Dest, Destlen);
    TestLog("TestMath", " \r\n sdkRSAPublicDecrypt()������� \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkRSAPrivateDecrypt(void)   //  32
{
    s32 rslt = 0;
    u8 Dest[64] = {0}; s32 Destlen = 64;
    u8 Src[64] = {1, 2, 3}; u32 Srclen = 64;
    SDK_RSA_PRIVATE_KEY PrivateKey;

    memset(&PrivateKey, 0, sizeof(PrivateKey));

    TestLog("TestMath", "\r\n sdkRSAPrivateDecrypt starts  \r\n");

    //TEST_IS_TRUE_WITH_INFO("��ָ�����1" , (SDK_PARA_ERR == sdkRSAPrivateDecrypt(NULL , &Destlen , Src , Srclen , &PrivateKey)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", (SDK_PARA_ERR == sdkRSAPrivateDecrypt(Dest, &Destlen, NULL, Srclen, &PrivateKey)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����3", (SDK_PARA_ERR == sdkRSAPrivateDecrypt(Dest, &Destlen, Src, Srclen, NULL)));

    TEST_IS_TRUE_WITH_INFO("��������", (rslt = sdkRSAPrivateDecrypt(Dest, &Destlen, Src, Srclen, &PrivateKey)));

    TestLogHex("TestMath", " \r\n ����� \r\n", Dest, Destlen);
    TestLog("TestMath", " \r\n sdkRSAPrivateDecrypt()������� \r\n");
}

#endif //endif 0

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
static void Test_sdkSHA1(void)   //  33
{
    u16 uLen = 0;
//    u8 temp[1024] = {0};
//    u8 temp1[1024] = {0};
//    u8 temp2[1024] = {0};
    u8 *temp1 = NULL, *temp2 = NULL, *temp = NULL;

    TestLog("TestMath", "\r\n sdkSHA1 starts  \r\n");

    temp = sdkGetMem(1024);
    if (temp == NULL) {
        Assert(0);
        return;
    }
    temp1 = sdkGetMem(1024);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        return;
    }
    temp2 = sdkGetMem(1024);
    if (temp2 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        temp = NULL;
        sdkFreeMem(temp1);
        temp1 = NULL;
        return;
    }

    memset(temp1, 0, 1024);
    strcpy(temp1, "12");
    uLen = 2;

    TEST_IS_TRUE_WITH_INFO("��ָ�����1", ((sdkSHA1(NULL, uLen, temp2) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", ((sdkSHA1(temp1, uLen, NULL) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("��ָ�����2", ((sdkSHA1(temp1, 0, temp2) == SDK_PARA_ERR)));

    TEST_IS_TRUE_WITH_INFO("��������", ((sdkSHA1(temp1, uLen, temp2) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("�������", ((memcmp(temp2,
                                               "\x7B\x52\x00\x9B\x64\xFD\x0A\x2A\x49\xE6\xD8\xA9\x39\x75\x30\x77\x79\x2B\x05\x54",
                                               20) == 0)));


    uLen = 0;
    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memcpy(temp1, "\x12\x34\x56", 3);
    TEST_IS_TRUE_WITH_INFO("����Ϊ0����", ((sdkSHA1(temp1, uLen, temp2) == SDK_PARA_ERR)));
    TEST_IS_TRUE_WITH_INFO("�����֤", ((memcmp(temp2,
                                              "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00",
                                              20) == 0)));

    uLen = 2;
    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memcpy(temp1, "\x12\x34\x56\x78", 4);
    TEST_IS_TRUE_WITH_INFO("���ȶ������ݳ��Ȳ���", ((sdkSHA1(temp1, uLen, temp2) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("�����֤", ((memcmp(temp2,
                                              "\xFF\xA7\x6D\x85\x4A\x29\x69\xE7\xB9\xD8\x38\x68\xD4\x55\x51\x2F\xCE\x0F\xD7\x4D",
                                              20) == 0)));

    uLen = 6;
    memset(temp1, 0, 1024);
    memset(temp2, 0, 1024);
    memcpy(temp1, "\x12\xAC\xD0", 3);
    TEST_IS_TRUE_WITH_INFO("���ȳ������ݳ��Ȳ���", ((sdkSHA1(temp1, uLen, temp2) == SDK_OK)));
    TEST_IS_TRUE_WITH_INFO("�����֤", ((memcmp(temp2,
                                              "\x52\xAB\xED\x05\x82\xED\x6B\x8E\x10\x60\xE3\xAD\x21\x2C\x5D\x25\x0B\x03\x11\xF8",
                                              20) == 0)));


    TestLogHex("TestMath", "����� \r\n", temp, 2);

    sdkFreeMem(temp);
    temp = NULL;
    sdkFreeMem(temp1);
    temp1 = NULL;
    sdkFreeMem(temp2);
    temp2 = NULL;
}

#endif

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		fusuipu  2013.01.30
** Remarks:
*****************************************************************************/

static void Test_sdkIccGetMagData(void) {
#ifndef IS_UCPlat
    if (sdkSysGetMachineCode(NULL) != SDK_SYS_MACHINE_K501AM) //huacong 2013.03.28 11:48
    {
        SDK_ICC_PARAM sticcparam;
        SDK_ICC_CARDDATA stSaleData;

        memset(&sticcparam, 0, sizeof(sticcparam));
        memset(&stSaleData, 0, sizeof(stSaleData));

        TEST_IS_TRUE_WITH_INFO("�򿪴ſ���ȡ����", (sdkIccOpenMag() == SDK_OK));
        TEST_IS_TRUE_WITH_INFO("���ſ���ָ�����",
                               ((sdkIccGetMagData(NULL, &stSaleData)) == SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("���ſ���ָ�����",
                               ((sdkIccGetMagData(&sticcparam, NULL)) == SDK_PARA_ERR));
        sdkIccCloseMag();
        TEST_IS_TRUE_WITH_INFO("��ֹ���ſ�����", ((sdkIccGetMagData(&sticcparam, NULL)) != SDK_ERR));
    }
#endif
    return;
}

static void Test_sdkSM2InitElliptic(void) {
#ifndef IS_UCPlat
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2InitElliptic(NULL, "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", NULL, "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00", NULL,
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", NULL,
                                                           "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           NULL, "\x31\x32\x33\x00") ==
                                        SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", NULL) ==
                                        SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x01\x02", "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x01\x02", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00", "\x01\x02",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x01\x02", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00", "\x01\x02",
                                               "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2InitElliptic("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                               "\x31\x32\x33\x00", "\x01\x02") == SDK_ERR));
#endif
    return;
}

static void Test_sdkSM3Encrypt(void) {
#ifndef IS_UCPlat
    u8 temp[128] = {0};
    u8 temp1[128] = {0};

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM3Encrypt(NULL, 10, temp) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM3Encrypt(temp1, 10, NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������", (sdkSM3Encrypt(temp1, -1, temp) == SDK_PARA_ERR));
    memset(temp1, 0x31, 1024);
    TEST_IS_TRUE_WITH_INFO("��ȷ���Բ���", (sdkSM3Encrypt(temp1, 1024, temp) == 32));
#endif
    return;
}

static void Test_sdkSM2CheckSign(void) {
#ifndef IS_UCPlat
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2CheckSign(NULL, "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2CheckSign("\x31\x32\x33\x00", NULL, "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00", NULL,
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                        "\x31\x32\x33\x00", NULL,
                                                        "\x31\x32\x33\x00", "\x31\x32\x33\x00") ==
                                        SDK_PARA_ERR));
//    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00", "\x31\x32\x33\x00", "\x31\x32\x33\x00", NULL, "\x31\x32\x33\x00") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                        "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                        "\x31\x32\x33\x00", NULL) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2CheckSign("\x01\x02", "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2CheckSign("\x31\x32\x33\x00", "\x01\x02", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���",
                           (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00", "\x01\x02",
                                            "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                            "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x01\x02",
                                                           "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00") == SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x01\x02", "\x31\x32\x33\x00") ==
                                           SDK_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݲ���", (sdkSM2CheckSign("\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x31\x32\x33\x00",
                                                           "\x31\x32\x33\x00", "\x01\x02") ==
                                           SDK_ERR));
#endif
    return;
}

static void Test_sdkSM2(void) {
#ifndef IS_UCPlat
//    int nRet = 0;

    const char GroupP[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
    const char GroupA[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
    const char GroupB[] = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
    const char GroupX[] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
    const char GroupY[] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";
    const char GroupN[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";
    char cKeyX[65] = "E8105E77861FD2EB727C84E36D3D4A5666BD0ADCE8781F0145D3D82D72B92748";
    char cKeyY[65] = "E22D5404C6C41F3EC8B790DE2F61CF29FAECB168C79F5C8666762D53CC26A460";
    char cSrcData[256] = "12622800011220000001040011409F483BF2CC71C5093728318061E3F768EA7C170F82DD8C4B979FBD8C76A129F93FB5746E96F5E49B987FFB521E473B25E1B017C30BE3FC638BA14D5FA4AADC16";
    char cSignR[65] = "B1E1A02ED62D811268C4A2123CD8B26690BCE14BB7A01713C01C2D2FF2892D15";
    char cSignS[65] = "96A7D9D131C73813C3A3D6F851DCDA917E986923F98DD2444ADC8EF0D0CE1E2A";
    char cIDA[65] = "31323334353637383132333435363738";

    //��ʼ��
//    nRet = sdkSM2InitElliptic(GroupP, GroupA, GroupB, GroupX, GroupY, GroupN);
    TEST_IS_TRUE_WITH_INFO("��ȷ���Բ���",
                           (sdkSM2InitElliptic(GroupP, GroupA, GroupB, GroupX, GroupY, GroupN) ==
                            SDK_OK));
    //��ǩ
//    nRet = sdkSM2CheckSign(cKeyX, cKeyY, cSignR, cSignS, cSrcData, cIDA);
    TEST_IS_TRUE_WITH_INFO("��ȷ���Բ���",
                           (sdkSM2CheckSign(cKeyX, cKeyY, cSignR, cSignS, cSrcData, cIDA) ==
                            SDK_OK));
    //�ͷ��ڴ�
    sdkSM2ClearAll();
#endif
    return;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.01.17
** Remarks:
*****************************************************************************/
void Test_sdkMathAuto(void) {
    TestLog("TestMath", "\r\n��ʼ��ѧ�����\r\n ");

    //   Test_sdkMathsGetVersion();    //  1 fusuipu modefied at 2013-1-29
    Test_sdkIccGetMagData();
    Test_sdkU8ToBcd();            //  2
    Test_sdkU16ToBcd();           //  3
    Test_sdkU32ToBcd();           //  4
    Test_sdkBcdToU8();            //  5
    sdkTestShowProcessing("�����20%...");
    Test_sdkBcdToU16();           //  6
    Test_sdkBcdToU32();           //  7
    Test_sdkHexToU16();           //  8
    Test_sdkHexToU32();           //  9
    Test_sdkU16ToHex();           //  10
    sdkTestShowProcessing("�����40%...");
    Test_sdkU32ToHex();           //  11
    Test_sdkBcdToFloat();         //  12
    Test_sdkBcdToAsc();           //  13
    Test_sdkBcdCardNotoAsc();     //  14
    Test_sdkAscToBcd();           //  15
    sdkTestShowProcessing("�����60%...");
    Test_sdkIsAscii();            //  16
    Test_sdkXOR8();               //  17
    Test_sdkBcdAdd();             //  18
    Test_sdkBcdSub();             //  19
    Test_sdkAscToBcdR();          //  20
    Test_sdkCalcBcc();            //21
    Test_sdkCalcAppCrc();         //22
    Test_sdkEcb();                //23
    Test_sdkE90S(); //24
    sdkTestShowProcessing("�����80%...");
    Test_sdkE99S(); //26
    Test_sdkE919S(); //27
    Test_sdkMD5(); //28


    Test_sdkE98S(); //25

    Test_sdkRSAPublicEncrypt(); //29   liuguangpeng�ײ���ýӿ�������ʱע��


    Test_sdkSHA1(); //33

    /*=======BEGIN: shijianglong 2013.06.03  16:16 modify===========*/
#if((MACHINEMODEL != SDK_SYS_MACHINE_K501AM) && (MACHINEMODEL != SDK_SYS_MACHINE_K508A))//&& !(MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P)
    Test_sdkSM2InitElliptic();
    Test_sdkSM3Encrypt();
    Test_sdkSM2CheckSign();
    Test_sdkSM2();
#endif
    /*====================== END======================== */

    return;
}


#endif

