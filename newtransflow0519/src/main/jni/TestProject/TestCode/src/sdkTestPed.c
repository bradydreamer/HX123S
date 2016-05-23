#include "sdkGlobal.h"
#include "SdkTest.h"

extern SDK_SYS_INITIAL_INFO gstInitial;
//extern s32 Private_sdkPEDSetSN(const u8 *pDataIn);


//=====================================================���������������===================================================

/**************************************sdkPEDGetVersion���Կ�ʼ**********************************/
#if 0 /*Modify by zxx at 2013.01.30  17:49 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPedGetVersion(void)
   ��������:	��ӡ�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
static void TestIllPar_sdkPedGetVersion(void)
{
    TestLog("TestPedGetVersion", " \r\n \r\n��ʼTestPedGetVersion�Ƿ���������  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDGetVersion(NULL) == SDK_PARA_ERR));

    TestLog("TestPedGetVersion", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPedGetVersion
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
static void  TestNormal_sdkPedGetVersion()
{
    u8 tem[30] = {0};
    s32 i, len;

    TestLog("TestsdkPedGetVersion", "\r\n������Կ�ʼ\r\n");

    i = sdkPEDGetVersion(tem);
    len = strlen(tem);
    TEST_IS_TRUE_WITH_INFO("���Է��ذ汾��Ϣ����", i == len);

    TEST_IS_TRUE_WITH_INFO("���Է��ذ汾��Ϣ�Ƿ�Ϊ�ɼ��ַ�", sdkIsVisibleStr(tem, (u32)len) == true);
    TestLog("TestsdkPedGetVersion", "\r\n������Խ���\r\n ");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPedGetVersion(void)
   ��������: ����sdkPedGetVersion
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
static void Test_sdkPedGetVersion(void)
{
    TestLog("TestPedGetVersion", "\r\n��ʼ����s32 sdkPEDGetVersion(u8 *pDataOut)\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPedGetVersion();
    //�����������
    TestNormal_sdkPedGetVersion();

    TestLog("TestPedGetVersion", "\r\n s32 sdkPEDGetVersion(u8 *pDataOut)���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDGetVersion�������******************************************************************/


/**************************************sdkPEDInitKeyIC���Կ�ʼ**********************************/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPEDInitKeyIC(void)
   ��������:	��ӡ�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:12 */
static void TestIllPar_sdkPEDInitKeyIC(void)
{
    TestLog("TestsdkPEDInitKeyIC", " \r\n \r\n��ʼTestsdkPEDInitKeyIC�Ƿ���������  \r\n \r\n");
    sdkTestMsgBox("ped��ʱ����,��γ��������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("����������̲���", (sdkPEDInitKeyIC() == SDK_TIME_OUT));

    TestLog("TestsdkPEDInitKeyIC", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif /* if 0 */

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPEDInitKeyIC
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
static void TestNormal_sdkPEDInitKeyIC() {
    TestLog("TestsdkPEDInitKeyIC", "\r\n������Կ�ʼ\r\n");
//    sdkTestMsgBox("�����������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("��������ʹ��", (sdkPEDInitKeyIC() == SDK_OK));

    TestLog("TestsdkPEDInitKeyIC", "\r\n������Խ���\r\n ");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPEDInitKeyIC(void)
   ��������: ����sdkPEDInitKeyIC
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
static void Test_sdkPEDInitKeyIC(void) {
    TestLog("TestsdkPEDInitKeyIC", "\r\n��ʼ����s32 sdkPEDInitKeyIC(u8 *pDataOut)\r\n ");

    //�Ƿ���������
//    TestIllPar_sdkPEDInitKeyIC();
    //�����������
    TestNormal_sdkPEDInitKeyIC();

    TestLog("TestsdkPEDInitKeyIC", "\r\n s32 sdkPEDInitKeyIC(u8 *pDataOut)���Խ���\r\n ");
}

/********************************************************************sdkPEDInitKeyIC�������******************************************************************/



/**************************************sdkPEDGetPedVersion���Կ�ʼ**********************************/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPEDGetPedVersion
   ��������:	�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
static void TestIllPar_sdkPEDGetPedVersion(void) {
    TestLog("TestsdkPEDGetPedVersion", " \r\n \r\n��ʼsdkPEDGetPedVersion�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDGetPedVersion(NULL) == SDK_PARA_ERR));
    {
        u8 temp[64] = {0};
        u8 buf[500] = {0};
        s32 ret = 0;

        if (sdkPEDIsWithPinpad() == false) //shijianglong 2013.06.08 14:53
        {
            sprintf(buf, "�˺���ֻ֧�������������");
            return;
        }
        ret = sdkPEDGetPedVersion(temp);
        sprintf(buf, "PedVersion:%s\r fun ret:%d", temp, ret);
        Test_sdkPrintTestInfo("sdkPEDGetPedVersion", buf);
    }
    TestLog("TestsdkPEDGetPedVersion", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPEDGetPedVersion
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
static void TestNormal_sdkPEDGetPedVersion() {
//    SDK_PED_PIN_CFG key[12];
    u8 temp[30];
    s32 ret = 0;

    TestLog("TestsdkPEDDispAmt", "\r\n������Կ�ʼ\r\n");
    memset(temp, 0, sizeof(temp));
    TEST_IS_TRUE_WITH_INFO("������������", ((ret = sdkPEDGetPedVersion(temp) >= 0)));
    TestLog("test", "sdkPEDGetPedVersion ret %d", ret);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkIsVisibleStr(temp, strlen(temp)) == true));

    TestLog("TestsdkPEDDispAmt", "\r\n������Խ���\r\n ");
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPEDGetPedVersion
   ��������: ����sdkPEDGetPedVersion
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
static void Test_sdkPEDGetPedVersion(void) {
    TestLog("TestsdkPEDGetPedVersion", "\r\n��ʼ����s32 sdkPEDGetPedVersion\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDGetPedVersion();

    //�������
    TestNormal_sdkPEDGetPedVersion();


    TestLog("TestsdkPEDGetPedVersion", "\r\n s32 sdkPEDGetPedVersion���Խ���\r\n ");
}

/********************************************************************sdkPEDGetPedVersion�������******************************************************************/




/**************************************sdkPEDGetSN���Կ�ʼ**********************************/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPEDGetSN
   ��������:	�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:13 */
static void TestIllPar_sdkPEDGetSN(void)
{
    TestLog("TestsdkPEDGetSN", " \r\n \r\n��ʼsdkPEDGetSN�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDGetPedSn(NULL) == SDK_PARA_ERR));


//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (sdkPEDGetPedSn(lpout) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);

    TestLog("TestsdkPEDGetSN", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPEDGetSN
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:13 */
static void  TestNormal_sdkPEDGetSN()
{
    u8 temp[30];

    TestLog("TestsdkPEDGetSN", "\r\n������Կ�ʼ\r\n");
    memset(temp, 0, sizeof(temp));
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDGetPedSn(temp) == SDK_OK));

//	TraceHex("ped","PedSn", temp,30);
    TestLog("", "sdkIsVisibleStr(temp , strlen(temp) = %d\r\n", sdkIsVisibleStr(temp, strlen(temp)));

    TEST_IS_TRUE_WITH_INFO("������������", (sdkIsVisibleStr(temp, strlen(temp)) == true));
    TestLog("TestsdkPEDGetSN", "\r\n������Խ���\r\n ");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPEDGetSN
   ��������: ����sdkPEDGetSN
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:13 */
static void Test_sdkPEDGetSN(void)
{
    TestLog("TestsdkPEDGetSN", "\r\n��ʼ����s32 sdkPEDGetSN\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDGetSN();

    //�������
    TestNormal_sdkPEDGetSN();


    TestLog("TestsdkPEDGetSN", "\r\n s32 sdkPEDGetSN���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDGetSN�������******************************************************************/





/**************************************sdkPEDSetSN���Կ�ʼ**********************************/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPEDSetSN
   ��������:	�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:14 */
static void TestIllPar_sdkPEDSetSN(void)
{
    u8 tem[6];

    TestLog("TestsdkPEDSetSN", " \r\n \r\n��ʼsdkPEDSetSN�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (Private_sdkPEDSetSN(NULL) == SDK_PARA_ERR));


    memset(tem, 0x32, sizeof(tem));
//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (Private_sdkPEDSetSN(tem) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);
    TestLog("TestsdkPEDSetSN", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPEDSetSN
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:14 */
static void  TestNormal_sdkPEDSetSN()
{
    u8 temp[20];

    TestLog("TestsdkPEDSetSN", "\r\n������Կ�ʼ\r\n");
    memset(temp, 0, sizeof(temp));
    strcpy(temp, "V1234567890");
    TEST_IS_TRUE_WITH_INFO("������������", (Private_sdkPEDSetSN(temp) == SDK_OK));
    sdkPEDCancel();

    TestLog("TestsdkPEDSetSN", "\r\n������Խ���\r\n ");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPEDGetSN
   ��������: ����sdkPEDGetSN
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:14 */
static void Test_sdkPEDSetSN(void)
{
    TestLog("TestsdkPEDSetSN", "\r\n��ʼ����s32 sdkPEDSetSN\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDSetSN();

    //�������
    TestNormal_sdkPEDSetSN();


    TestLog("TestsdkPEDSetSN", "\r\n s32 sdkPEDSetSN���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDSetSN�������******************************************************************/



/**************************************sdkPEDSetLanguage���Կ�ʼ**********************************/
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:void TestIllPar_sdkPEDSetLanguage
   ��������:	�Ƿ���������
   ����ʱ��:2012.05.19 15:49:15
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:14 */
static void TestIllPar_sdkPEDSetLanguage(void)
{
    TestLog("TestsdkPEDSetLanguage", " \r\n \r\n��ʼsdkPEDSetLanguage�Ƿ���������  \r\n \r\n");

//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (sdkPEDSetLanguage(SDK_PED_CH) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);
    TestLog("TestsdkPEDSetLanguage", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	TestNormal_sdkPEDSetLanguage
   ��������:	���Գ������
   ����ʱ��:2012.05.19 15:49:44
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:14 */
static void  TestNormal_sdkPEDSetLanguage()
{
    TestLog("TestsdkPEDSetLanguage", "\r\n������Կ�ʼ\r\n");

    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDSetLanguage(SDK_PED_EN) == SDK_OK));
    sdkTestMsgBox("��鿴���������ӦΪӢ��", SDK_KEY_MASK_ALL);

    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDSetLanguage(SDK_PED_CH) == SDK_OK));
    sdkTestMsgBox("��鿴���������ӦΪ����", SDK_KEY_MASK_ALL);

    TestLog("TestsdkPEDSetLanguage", "\r\n������Խ���\r\n ");
}

#endif /* if 0 */
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void Test_sdkPEDSetLanguage
   ��������: ����sdkPEDSetLanguage
   �޸ı�ע:
   ����ʱ��: 2012.05.22 20:18:00
*******************************************************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:13 */
static void Test_sdkPEDSetLanguage(void)
{
    TestLog("TestsdkPEDSetLanguage", "\r\n��ʼ����s32 sdkPEDSetLanguage\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDSetLanguage();

    //�������
    TestNormal_sdkPEDSetLanguage();


    TestLog("TestsdkPEDSetLanguage", "\r\n s32 sdkPEDSetLanguage���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDSetLanguage�������******************************************************************/


/**************************************sdkPEDDispAmt���Կ�ʼ**********************************/

#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:16 */
static void TestIllPar_sdkPEDDispAmt(void)
{
    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDDispAmte", " \r\n \r\n��ʼsdkPEDDispAmt�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDispAmt(SDK_PED_NEGATIVE, NULL) == SDK_PARA_ERR));

//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (sdkPEDDispAmt(temp) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);
    TestLog("TestsdkPEDDispAmt", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif /* if 0 */

#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:16 */
static void  TestNormal_sdkPEDDispAmt()
{
    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDDispAmt", "\r\n������Կ�ʼ\r\n");
    sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDDispAmt(SDK_PED_NEGATIVE, temp) == SDK_OK));
    sdkTestMsgBox("�����������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDDispAmt(SDK_PED_POSITIVE, temp) == SDK_OK));
    sdkPEDCancel();

    TestLog("TestsdkPEDDispAmt", "\r\n������Խ���\r\n ");
}

#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:16 */
#endif /* if 0 */
static void Test_sdkPEDDispAmt(void)
{
    TestLog("TestsdkPEDDispAmt", "\r\n��ʼ����s32 sdkPEDDispAmt\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDDispAmt();

    //�������
    TestNormal_sdkPEDDispAmt();


    TestLog("TestsdkPEDDispAmt", "\r\n s32 sdkPEDDispAmt���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDDispAmt�������******************************************************************/

/**************************************sdkPEDDispBalance���Կ�ʼ**********************************/

#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:16 */
static void TestIllPar_sdkPEDDispBalance(void)
{
    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDDispBalance", " \r\n \r\n��ʼsdkPEDDispBalance�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDDispBalance(SDK_PED_NEGATIVE, NULL) == SDK_PARA_ERR));

//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (sdkPEDDispBalance(SDK_PED_NEGATIVE,temp) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);
    TestLog("TestdkPEDDispBalance", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void  TestNormal_sdkPEDDispBalance()
{
    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestdkPEDDispBalance", "\r\n������Կ�ʼ\r\n");

    sdkTestMsgBox("��������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDDispBalance(SDK_PED_NEGATIVE, temp) == SDK_OK));
    sdkTestMsgBox("�����������", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDDispBalance(SDK_PED_POSITIVE, temp) == SDK_OK));
    sdkPEDCancel();
    TestLog("TestsdkPEDDispBalance", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDDispBalance(void)
{
    TestLog("TestsdkPEDDispBalance", "\r\n��ʼ����s32 sdkPEDDispBalance\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDDispBalance();

    //�������
    TestNormal_sdkPEDDispBalance();


    TestLog("TestsdkPEDDispBalance", "\r\n s32 sdkPEDDispBalance���Խ���\r\n ");
}

#endif /* if 0 */

/********************************************************************sdkPEDDispBalance�������******************************************************************/

/**************************************sdkPEDUpdateTmk���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDUpdateTmk(void) {
//s32 sdkPEDUpdateTmk(s32 iTmkIndex, SDK_PED_DES_TYPE eKeyType, const u8 *pheKey, s32 iTimeout)

    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDUpdateTmk", " \r\n \r\n��ʼsdkPEDUpdateTmk�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkPEDUpdateTmk(0, SDK_PED_DES_SINGLE, NULL, SDK_PED_TIMEOUT) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateTmk(101, SDK_PED_DES_SINGLE, temp, SDK_PED_TIMEOUT) ==
                            SDK_PARA_ERR));

//    sdkTestMsgBox("��γ��������",SDK_KEY_MASK_ALL);
//    TEST_IS_TRUE_WITH_INFO("���Գ�ʱ" , (sdkPEDDispBalance(SDK_PED_NEGATIVE,temp) == SDK_TIME_OUT));
//    sdkTestMsgBox("������������",SDK_KEY_MASK_ALL);
    TestLog("TestsdkPEDUpdateTmk", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDUpdateTmk() {
    u8 temp[30] = {0};

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDUpdateTmk", "\r\n������Կ�ʼ\r\n");

    memset(temp, 0x31, sizeof(temp));
    TestLog("TestsdkPEDUpdateTmk", "SDK_PED_DES_SINGLE Tmkindex = 0  Tmkdata = all 0x31\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDUpdateTmk(0, SDK_PED_DES_SINGLE, temp, 3000) == SDK_OK));


    TestLog("TestsdkPEDUpdateTmk",
            "SDK_PED_DES_TRIPLE Tmkindex = 1  Tmkdata = 0x31 x 6 0x32 x 6\r\n");
    memset(&temp[8], 0x32, 16);
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDUpdateTmk(1, SDK_PED_DES_TRIPLE, temp, 3000) == SDK_OK));


    TestLog("TestsdkPEDUpdateTmk", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDUpdateTmkLoop() {
    u8 temp[30] = {0};
    u8 i = 0;

    memset(temp, 0x31, sizeof(temp));

    for (; i < 100; i++) {
        memset(temp, 0x31, sizeof(temp));
        TestLog("TestsdkPEDUpdateTmk", "SDK_PED_DES_SINGLE Tmkindex = %d  Tmkdata = all 0x31\r\n",
                i);
        TEST_IS_TRUE_WITH_INFO("Tmk ѭ��",
                               (sdkPEDUpdateTmk(i, SDK_PED_DES_SINGLE, temp, SDK_PED_TIMEOUT * 2) ==
                                SDK_OK));
    }
}

static void Test_sdkPEDUpdateTmk() {
    TestLog("TestsdkPEDUpdateTmk", "\r\n��ʼ����s32 sdkPEDUpdateTmk\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDUpdateTmk();

    //�������
    TestNormal_sdkPEDUpdateTmk();


    TestLog("TestsdkPEDUpdateTmk", "\r\n s32 sdkPEDUpdateTmk���Խ���\r\n ");
}

/**************************************sdkPEDUpdateWkEx���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDUpdateWkEx(void) {
    SDK_PED_WK_CFG stPedWkCfg[2];

    memset(&stPedWkCfg[0], 0, sizeof(stPedWkCfg));

    TestLog("sdkPEDUpdateWkEx", " \r\n \r\n��ʼsdkPEDUpdateWkEx�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDUpdateWkEx(NULL, 2) == SDK_PARA_ERR));

    TestLog("sdkPEDUpdateWkEx", " ����Wkʱ��Ӧ��Tmkindex < 100\r\n");
    stPedWkCfg[0].ucTmkIndex = 101;
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_PARA_ERR));

    TestLog("sdkPEDUpdateWkEx", " ����Wkʱ��Ӧ��wkindex < 125\r\n");
    stPedWkCfg[0].ucWkIndex = 125;
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_PARA_ERR));

    TestLog("sdkPEDUpdateWkEx", "����Wkʱ��Ӧ��EnWkLen > 0 \r\n");
    stPedWkCfg[0].ucWkIndex = 0;
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_PARA_ERR));

    TestLog("sdkPEDUpdateWkEx", "����Wkʱ��Ӧ��EnWkLen > 0 && <= 24\r\n");
    stPedWkCfg[0].ucWkIndex = 25;
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_PARA_ERR));

    TestLog("sdkPEDUpdateWkEx", "����Wkʱ��Ӧ��EnWkLen > 0 && <= 24 && %8 != 0 \r\n");
    stPedWkCfg[0].ucWkIndex = 17;
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_PARA_ERR));
    TestLog("sdkPEDUpdateWkEx", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDUpdateWkEx() {
    SDK_PED_WK_CFG stPedWkCfg[1];

    memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG));

    stPedWkCfg[0].bOnlyCalcCheckValue = false;
    stPedWkCfg[0].eTmkType = SDK_PED_DES_TRIPLE;
    stPedWkCfg[0].eWkType = SDK_PED_DES_TRIPLE;
    stPedWkCfg[0].ucTmkIndex = 1;
    stPedWkCfg[0].ucWkIndex = 2;
    stPedWkCfg[0].ucEnWkLen = 16;

    memset(stPedWkCfg[0].heCheckData, 0, 24);

    //31313131313131313232323232323232//����Կ
    //F792CC988ED7E3DF8A14E394C2B48DB1 //A83E7705//���������Լ�checkvalue
    //34313138383639353439313730383939//���ܺ�����
    memcpy(stPedWkCfg[0].heEnWk, "\xF7\x92\xCC\x98\x8E\xD7\xE3\xDF\x8A\x14\xE3\x94\xC2\xB4\x8D\xB1",
           16);
    stPedWkCfg[0].ucCheckDataLen = 16;
    stPedWkCfg[0].ucCheckValueLen = 4;

    memcpy(stPedWkCfg[0].heCheckValue, "\xA8\x3E\x77\x05", 4);

    TestLog("TestsdkPEDUpdateWkEx", "SDK_PED_DES_TRIPLE Tmkindex = 1 WkIndex = 2&3\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDUpdateWkEx(&stPedWkCfg[0], SDK_PED_DES_ALL) == SDK_OK));

    TestLog("TestsdkPEDUpdateWkEx", "\r\n������Խ���\r\n ");
}


static void Test_sdkPEDUpdateWkEx() {
    TestLog("Test_sdkPEDUpdateWkEx", "\r\n��ʼ����void Test_sdkPEDUpdateWkEx\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDUpdateWkEx();

    //�������
    TestNormal_sdkPEDUpdateWkEx();

    TestLog("Test_sdkPEDUpdateWkEx", "\r\n void Test_sdkPEDUpdateWkEx���Խ���\r\n ");
}
/********************************************************************Test_sdkPEDUpdateWkEx�������******************************************************************/



/**************************************sdkPEDUpdateTmkExt���Կ�ʼ**********************************/
static void Test_sdkPEDUpdateTmkEx() {
    u8 temp[32] = {0};
    u8 dataIn[64] = {1};
    u8 dataOut1[64] = {0};
    u8 dataOut2[64] = {0};

    /*ȫ��ΪDES����*/
    //��һ������Կ 0x31
    memset(temp, 0x31, sizeof(temp));
    sdkPEDUpdateTmk(0, SDK_PED_DES_SINGLE, temp, 3000);
    memset(temp, 0x32, sizeof(temp));
    sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, SDK_PED_DES_ALL, temp, 8,
                dataOut1);//�õ�һ������Կ�Եĵڶ�������Կ����
    sdkPEDUpdateTmkEx(0, SDK_PED_DES_SINGLE, 2, SDK_PED_DES_SINGLE, dataOut1, 3000);//�޸ĵ�������Կ
    //�ڶ�������Կ 0x32
    memset(temp, 0x32, sizeof(temp));
    sdkPEDUpdateTmk(1, SDK_PED_DES_SINGLE, temp, 3000);

    memset(dataOut1, 0, 64);
    memset(dataOut1, 0, 64);
    sdkPEDTmkDes(1, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, dataIn,
                 dataOut1);//�õ�2������Կ�Եĵڶ�������Կ����
    sdkPEDTmkDes(2, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, dataIn,
                 dataOut2);//�õ�3������Կ�Եĵڶ�������Կ����
    if (memcmp(dataOut1, dataOut2, 8) == 0) {
        TestLog("sdkPEDUpdateTmkEx", "SDK_PED_DES_SINGLE update Tmk Succ\r\n");
    }
    else {
        TestLog("sdkPEDUpdateTmkEx", "SDK_PED_DES_SINGLE update Tmk  Fail\r\n");
    }


    /*ȫ��Ϊ3DES����*/
    //��һ������Կ 0x31
    memset(temp, 0x31, sizeof(temp));
    sdkPEDUpdateTmk(0, SDK_PED_DES_TRIPLE, temp, 3000);
    memset(temp, 0x32, sizeof(temp));
    TEST_IS_TRUE_WITH_INFO("������������0",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 16, dataOut1) ==
                            SDK_OK));//�õ�һ������Կ�Եĵڶ�������Կ����
    TEST_IS_TRUE_WITH_INFO("������������0-0",
                           (sdkPEDUpdateTmkEx(0, SDK_PED_DES_TRIPLE, 2, SDK_PED_DES_TRIPLE,
                                              dataOut1, 3000) == SDK_OK));//�޸ĵ�������Կ
    //�ڶ�������Կ 0x32
    memset(temp, 0x32, sizeof(temp));
    sdkPEDUpdateTmk(1, SDK_PED_DES_TRIPLE, temp, 3000);

    memset(dataOut1, 0, 64);
    memset(dataOut1, 0, 64);
    TEST_IS_TRUE_WITH_INFO("������������1",
                           (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, dataIn, 64, dataOut1) ==
                            SDK_OK));//�õ�2������Կ�ԵĲ������ݼ���
    TEST_IS_TRUE_WITH_INFO("������������2",
                           (sdkPEDDesEx(2, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, dataIn, 64, dataOut2) ==
                            SDK_OK));//�õ�3������Կ�ԵĲ������ݼ���

    if (memcmp(dataOut1, dataOut2, 64) == 0) {
        TestLog("sdkPEDUpdateTmkEx", "SDK_PED_DES_TRIPLE update Tmk Succ\r\n");
    }
    else {
        TestLog("sdkPEDUpdateTmkEx", "SDK_PED_DES_TRIPLE update Tmk  Fail\r\n");
    }

    TestLog("TestsdkPEDUpdateWkEx", "\r\n������Խ���\r\n ");
}
/**************************************sdkPEDUpdateTmkExt���Խ���**********************************/


/**************************************TestIllPar_sdkPEDDesEx���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDDesEx(void) {
    u8 temp[32] = {0};

    TEST_IS_TRUE_WITH_INFO("�������ݿ�ָ�����",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, NULL, 8, temp) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�������ݿ�ָ�����",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 8, NULL) == SDK_PARA_ERR));

    TestLog("sdkPEDDesEx", "�ӽ���ʱ��Ӧ��usInLen != 0 \r\n");
    TEST_IS_TRUE_WITH_INFO("�������ݳ��Ȳ���",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 0, temp) == SDK_PARA_ERR));
    TestLog("sdkPEDDesEx", "�ӽ���ʱ��Ӧ��usInLen <= 512 \r\n");
    TEST_IS_TRUE_WITH_INFO("�������ݳ��Ȳ���",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 513, temp) == SDK_PARA_ERR));
    TestLog("sdkPEDDesEx", "�ӽ���ʱ��Ӧ��usInLen%8 != 0 \r\n");
    TEST_IS_TRUE_WITH_INFO("�������ݳ��Ȳ���",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 7, temp) == SDK_PARA_ERR));

    TestLog("sdkPEDDesEx", "DES�ӽ���ʱ��Ӧ��eCryptWay != SDK_PED_DES_TOP8\r\n");
    TEST_IS_TRUE_WITH_INFO("��Կλ����",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_TOP8, temp, 8, temp) == SDK_PARA_ERR));
    TestLog("sdkPEDDesEx", "DES�ӽ���ʱ��Ӧ��eCryptWay != SDK_PED_DES_MIDDLE8\r\n");
    TEST_IS_TRUE_WITH_INFO("��Կλ����",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_MIDDLE8, temp, 8, temp) == SDK_PARA_ERR));
    TestLog("sdkPEDDesEx", "DES�ӽ���ʱ��Ӧ��eCryptWay != SDK_PED_DES_LAST8\r\n");
    TEST_IS_TRUE_WITH_INFO("��Կλ����",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_LAST8, temp, 8, temp) == SDK_PARA_ERR));

    TestLog("sdkPEDDesEx", "������Կ�ӽ���ʱ��Ӧ��iIndex < 125 \r\n");
    TEST_IS_TRUE_WITH_INFO("��Կλ����",
                           (sdkPEDDesEx(125, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 8, temp) == SDK_PARA_ERR));

    TestLog("sdkPEDDesEx", "����Կ�ӽ���ʱ��Ӧ��iIndex < 100 \r\n");
    TEST_IS_TRUE_WITH_INFO("��Կλ����",
                           (sdkPEDDesEx(100, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp, 8, temp) == SDK_PARA_ERR));
    TestLog("TestsdkPEDWkDes", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}


static void TestNormal_sdkPEDDesEx() {
    u8 temp1[128] = {0};
    u8 temp2[128] = {0};

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("����ԿDESȫ��Կ������������",
                           (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("����ԿDESȫ��Կ������������",
                               (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_ALL, temp2, 8, temp1) == SDK_OK));
    }
#if 0 /*Modify by ���ƽ at 2014.06.26  19:22 */

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("����ԿDESǰ8λ������������", (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, SDK_PED_DES_TOP8, temp1, 8, temp2 ) == SDK_OK));
    if(sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("����ԿDESǰ8λ������������", (sdkPEDDesEx(0, SDK_PED_TMK, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT, SDK_PED_DES_TOP8, temp2, 8, temp1 ) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8))
        {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else
        {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }
#endif /* if 0 */


    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("����Կ3DESȫ��Կ������������",
                           (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("����Կ3DESȫ��Կ������������",
                               (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_ALL, temp2, 8, temp1) == SDK_OK));
    }

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("����Կ3DESǰ8λ������������",
                           (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_TOP8, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("����Կ3DESǰ8λ������������",
                               (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_TOP8, temp2, 8, temp1) == SDK_OK));
    }

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("����Կ3DE��8λ������������",
                           (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_MIDDLE8, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("����Կ3DE��8λ������������",
                               (sdkPEDDesEx(1, SDK_PED_TMK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_MIDDLE8, temp2, 8, temp1) == SDK_OK));
    }

    /*������Կ���Բ��ֿ�ʼ*/
    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������ԿDESȫ��Կ������������",
                           (sdkPEDDesEx(0, SDK_PED_WK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp1, 128, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������ԿDESȫ��Կ������������",
                               (sdkPEDDesEx(0, SDK_PED_WK, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_ALL, temp2, 128, temp1) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 128) == 0));

        if (!memcmp(temp1, temp2, 128)) {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }

#if 0 /*Modify by ���ƽ at 2014.06.26  19:23 */
    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������ԿDESǰ8λ������������", (sdkPEDDesEx(0, SDK_PED_WK, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, SDK_PED_DES_TOP8, temp1, 8, temp2 ) == SDK_OK));
    if(sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������ԿDESǰ8λ������������", (sdkPEDDesEx(0, SDK_PED_WK, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT, SDK_PED_DES_TOP8, temp2, 8, temp1 ) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8))
        {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else
        {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }
#endif /* if 0 */


    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������Կ3DESȫ��Կ������������",
                           (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_ALL, temp1, 128, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������Կ3DESȫ��Կ������������",
                               (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_ALL, temp2, 128, temp1) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 128) == 0));

        if (!memcmp(temp1, temp2, 128)) {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������Կ3DESǰ8λ������������",
                           (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_TOP8, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������Կ3DESǰ8λ������������",
                               (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_TOP8, temp2, 8, temp1) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������Կ3DES��8λ������������",
                           (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT,
                                        SDK_PED_DES_MIDDLE8, temp1, 8, temp2) == SDK_OK));
    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������Կ3DE��8λ������������",
                               (sdkPEDDesEx(2, SDK_PED_WK, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT,
                                            SDK_PED_DES_MIDDLE8, temp2, 8, temp1) == SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des TRIPLE��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDDesEx", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }

    TestLog("TestsdkPEDDesEx", "\r\n������Խ���\r\n ");
}


static void Test_sdkPEDDesEx() {
    TestLog("Test_sdkPEDDesEx", "\r\n��ʼ����s32 Test_sdkPEDDesEx\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDDesEx();

    //�������
    TestNormal_sdkPEDDesEx();


    TestLog("Test_sdkPEDDesEx", "\r\n s32 Test_sdkPEDDesEx���Խ���\r\n ");
}

/********************************************************************Test_sdkPEDDesEx�������******************************************************************/











/**************************************sdkPEDUpdateWk���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDUpdateWk(void) {
//s32 sdkPEDUpdateWk(SDK_PED_WK_CFG *pstWkCfg, const s32 iWknum, s32 iTimeout)

    SDK_PED_WK_CFG stPedWkCfg[2];

    memset(&stPedWkCfg[0], 0, sizeof(stPedWkCfg));
    stPedWkCfg[0].ucTmkIndex = 101;
    stPedWkCfg[1].ucTmkIndex = 101;

    TestLog("TestsdkPEDUpdateWk", " \r\n \r\n��ʼsdkPEDUpdateWk�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDUpdateWk(NULL, 2, SDK_PED_TIMEOUT) == SDK_PARA_ERR));
    TestLog("TestsdkPEDUpdateWk", " ����Wkʱ��Ӧ��Tmkindex > 100\r\n");
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDUpdateWk(&stPedWkCfg[0], 2, SDK_PED_TIMEOUT) == SDK_PARA_ERR));
    TestLog("TessdkPEDUpdateWk", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDUpdateWk() {
    SDK_PED_WK_CFG stPedWkCfg[2];

    memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG) * 2);
    stPedWkCfg[0].bOnlyCalcCheckValue = false;

    stPedWkCfg[0].eTmkType = SDK_PED_DES_SINGLE;
    stPedWkCfg[1].eTmkType = SDK_PED_DES_SINGLE;

    stPedWkCfg[0].eWkType = SDK_PED_DES_SINGLE;
    stPedWkCfg[1].eWkType = SDK_PED_DES_SINGLE;

    stPedWkCfg[0].ucTmkIndex = 0;
    stPedWkCfg[1].ucTmkIndex = 0;

    stPedWkCfg[0].ucWkIndex = 0;
    stPedWkCfg[1].ucWkIndex = 1;

    stPedWkCfg[0].ucEnWkLen = 8;
    stPedWkCfg[1].ucEnWkLen = 8;

    memset(stPedWkCfg[0].heCheckData, 0, 24);
    memset(stPedWkCfg[1].heCheckData, 0, 24);

    //3131313131313131//����Կ
    //F92714E80E832EDC//ADC67D84//4BF6E91B1E3A9D81//ADC67D84//���������Լ�checkvalue
    //3232323232323232//3333333333333333//���ܺ�����
    memcpy(stPedWkCfg[0].heEnWk, "\xF9\x27\x14\xE8\x0E\x83\x2E\xDC", 8);
    memcpy(stPedWkCfg[1].heEnWk, "\x4B\xF6\xE9\x1B\x1E\x3A\x9D\x81", 8);

    stPedWkCfg[0].ucCheckDataLen = 8;
    stPedWkCfg[1].ucCheckDataLen = 8;


    stPedWkCfg[0].ucCheckValueLen = 4;
    stPedWkCfg[1].ucCheckValueLen = 4;

    memcpy(stPedWkCfg[0].heCheckValue, "\xAD\xC6\x7D\x84", 4);
    memcpy(stPedWkCfg[1].heCheckValue, "\xAD\xC6\x7D\x84", 4);

    TestLog("TestsdkPEDUpdateWk", "\r\n������Կ�ʼ\r\n");
    TestLog("TestsdkPEDUpdateWk", "SDK_PED_DES_SINGLE Tmkindex = 0\r WkIndex = 0&1\r\n");
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDUpdateWk(&stPedWkCfg[0], 2, 3000) == SDK_OK));
    memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG) * 2);
    stPedWkCfg[0].bOnlyCalcCheckValue = false;

    stPedWkCfg[0].eTmkType = SDK_PED_DES_TRIPLE;
    stPedWkCfg[1].eTmkType = SDK_PED_DES_TRIPLE;

    stPedWkCfg[0].eWkType = SDK_PED_DES_TRIPLE;
    stPedWkCfg[1].eWkType = SDK_PED_DES_TRIPLE;

    stPedWkCfg[0].ucTmkIndex = 1;
    stPedWkCfg[1].ucTmkIndex = 1;

    stPedWkCfg[0].ucWkIndex = 2;
    stPedWkCfg[1].ucWkIndex = 3;

    stPedWkCfg[0].ucEnWkLen = 16;
    stPedWkCfg[1].ucEnWkLen = 16;

    memset(stPedWkCfg[0].heCheckData, 0, 24);
    memset(stPedWkCfg[1].heCheckData, 0, 24);

    //31313131313131313232323232323232//����Կ
    //F792CC988ED7E3DF8A14E394C2B48DB1 //A83E7705//���������Լ�checkvalue
    //34313138383639353439313730383939//���ܺ�����
    memcpy(stPedWkCfg[0].heEnWk, "\xF7\x92\xCC\x98\x8E\xD7\xE3\xDF\x8A\x14\xE3\x94\xC2\xB4\x8D\xB1",
           16);
    memcpy(stPedWkCfg[1].heEnWk, "\xF7\x92\xCC\x98\x8E\xD7\xE3\xDF\x8A\x14\xE3\x94\xC2\xB4\x8D\xB1",
           16);

    stPedWkCfg[0].ucCheckDataLen = 16;
    stPedWkCfg[1].ucCheckDataLen = 16;


    stPedWkCfg[0].ucCheckValueLen = 4;
    stPedWkCfg[1].ucCheckValueLen = 4;

    memcpy(stPedWkCfg[0].heCheckValue, "\xA8\x3E\x77\x05", 4);
    memcpy(stPedWkCfg[1].heCheckValue, "\xA8\x3E\x77\x05", 4);

    TestLog("TestsdkPEDUpdateWk", "SDK_PED_DES_TRIPLE Tmkindex = 1 WkIndex = 2&3\r\n");
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDUpdateWk(&stPedWkCfg[0], 2, 3000) == SDK_OK));

    TestLog("TestsdkPEDUpdateWk", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDUpdateWk() {
    TestLog("TestsdkPEDUpdateWk", "\r\n��ʼ����s32 sdkPEDUpdateWk\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDUpdateWk();

    //�������
    TestNormal_sdkPEDUpdateWk();


    TestLog("TestsdkPEDUpdateWk", "\r\n s32 sdkPEDUpdateWk���Խ���\r\n ");
}

/********************************************************************sdkPEDUpdateTmk�������******************************************************************/



static void Test_sdkPEDUpdateWkLoop() {
    SDK_PED_WK_CFG stPedWkCfg[2];
    u8 i = 0;
    u8 temp[64]; //huacong 2013.03.29 14:12
    s32 count = 0; //shijianglong 2013.04.18 17:54

    memset(&stPedWkCfg[0], 0, sizeof(SDK_PED_WK_CFG) * 2);
    stPedWkCfg[0].bOnlyCalcCheckValue = false;

    stPedWkCfg[0].eTmkType = SDK_PED_DES_SINGLE;
    stPedWkCfg[1].eTmkType = SDK_PED_DES_SINGLE;

    stPedWkCfg[0].eWkType = SDK_PED_DES_SINGLE;
    stPedWkCfg[1].eWkType = SDK_PED_DES_SINGLE;


    stPedWkCfg[0].ucEnWkLen = 8;
    stPedWkCfg[1].ucEnWkLen = 8;

    memset(stPedWkCfg[0].heCheckData, 0, 24);
    memset(stPedWkCfg[1].heCheckData, 0, 24);

    //3131313131313131//����Կ
    //F92714E80E832EDC//ADC67D84//4BF6E91B1E3A9D81//ADC67D84//���������Լ�checkvalue
    //3232323232323232//3333333333333333//���ܺ�����
    memcpy(stPedWkCfg[0].heEnWk, "\xF9\x27\x14\xE8\x0E\x83\x2E\xDC", 8);
    memcpy(stPedWkCfg[1].heEnWk, "\x4B\xF6\xE9\x1B\x1E\x3A\x9D\x81", 8);

    stPedWkCfg[0].ucCheckDataLen = 8;
    stPedWkCfg[1].ucCheckDataLen = 8;


    stPedWkCfg[0].ucCheckValueLen = 4;
    stPedWkCfg[1].ucCheckValueLen = 4;

    memcpy(stPedWkCfg[0].heCheckValue, "\xAD\xC6\x7D\x84", 4);
    memcpy(stPedWkCfg[1].heCheckValue, "\xAD\xC6\x7D\x84", 4);

    if (sdkIsSupportPCI() == true) {
        count = 124;
    }
    else {
        count = 20;
    }

    for (; i < count; i += 2) {
        if (i < 100) {
            stPedWkCfg[0].ucTmkIndex = i;
            stPedWkCfg[1].ucTmkIndex = i;
        }
        else {
            stPedWkCfg[0].ucTmkIndex = 0;
            stPedWkCfg[1].ucTmkIndex = 0;
        }
        stPedWkCfg[0].ucWkIndex = i;
        stPedWkCfg[1].ucWkIndex = i + 1;

        TestLog("TestsdkPEDUpdateWk", "SDK_PED_DES_SINGLE  WkIndex = %d&%d\r\n", i, i + 1);
        memset(temp, 0, sizeof(temp));
        sprintf(temp, "Wkѭ��WkIndex = %d&%d", i, i + 1);
        TEST_IS_TRUE_WITH_INFO(temp,
                               (sdkPEDUpdateWk(&stPedWkCfg[0], 2, SDK_PED_TIMEOUT * 5) == SDK_OK));
    }
}

/*****************************************************************************
** Descriptions:	�����������к�����
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.04.09
** Remarks:
*****************************************************************************/
//static void Test_Out_Private_sdkPEDSetSN()
//{
//    u8 temp[64];
//
//    Trace("TestsdkPEDUpdateWk", "\r\n��ʼ����s32 Test_Out_Private_sdkPEDSetSN\r\n ");
//    //�Ƿ���������
//     TEST_IS_TRUE_WITH_INFO("���ü����������кŲ���",Private_sdkPEDSetSN("12345678901")== SDK_OK);
//     sdkPEDGetPedSn(temp);
//     TEST_IS_TRUE_WITH_INFO("��ȡ���кűȽ�",memcpy(temp,"12345678901",11) == 0);
//
//     TEST_IS_TRUE_WITH_INFO("���ü����������кŲ���",Private_sdkPEDSetSN("1234567890")== SDK_PARA_ERR);
//     TEST_IS_TRUE_WITH_INFO("���ü����������кŲ���",Private_sdkPEDSetSN(NULL)== SDK_PARA_ERR);
//
//    Trace("TestsdkPEDUpdateWk", "\r\n s32 Test_Out_Private_sdkPEDSetSN���Խ���\r\n ");
//}

/********************************************************************sdkPEDUpdateWk�������******************************************************************/

/**************************************sdkPEDTmkDes���Կ�ʼ**********************************/

static void TestIllPar_sdkPEDTmkDes(void) {
//s32 sdkPEDTmkDes(s32 iTmkIndex, SDK_PED_DES_TYPE eTmkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn, u8 *pheOut)

    u8 temp[32] = {0};

    TestLog("TestsdkPEDTmkDes", " \r\n \r\n��ʼsdkPEDTmkDes�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkPEDTmkDes(0, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, NULL, NULL) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDTmkDes(101, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, temp, temp) ==
                            SDK_PARA_ERR));
    TestLog("TestsdkPEDTmkDes", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDTmkDes() {
    u8 temp1[17] = {0};
    u8 temp2[17] = {0};

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDTmkDes", "\r\n������Կ�ʼ\r\n");
    TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_SINGLE\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDTmkDes(0, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                            SDK_OK));
    memset(temp1, 0, sizeof(temp1));
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDTmkDes(0, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT, temp2, temp1) ==
                            SDK_OK));
#if !SDK_IS_DDI_PLAM()/*Modify by ���ƽ at 2014.06.26  12:20 ddi ƽ̨ ��������ܲ����ؽ��ܽ��*/
    memset(temp2, 0x31, sizeof(temp2));
    TEST_IS_TRUE_WITH_INFO("Tmk des single��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));
#endif /* if 0 */

    if (!memcmp(temp1, temp2, 8)) {
        TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_SINGLE En/Decrypt Succ\r\n");
    }
    else {
        TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_SINGLE En/Decrypt fail\r\n");
    }
    TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_TRIPLE\r\n");
    memset(temp1, 0x31, sizeof(temp1));
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                            SDK_OK));
    memset(temp1, 0, sizeof(temp1));
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDTmkDes(1, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, temp2, temp1) ==
                            SDK_OK));
#if !SDK_IS_DDI_PLAM()/*Modify by ���ƽ at 2014.06.26  12:20 ddi ƽ̨ ��������ܲ����ؽ��ܽ��*/
    memset(temp2, 0x31, sizeof(temp2));
    TEST_IS_TRUE_WITH_INFO("Tmk triple single��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));
#endif /* if 0 */    


    if (!memcmp(temp1, temp2, 8)) {
        TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
    }
    else {
        TestLog("TestsdkPEDTmkDes", "SDK_PED_DES_TRIPLE En/Decrypt fail\r\n");
    }
    TestLog("TestsdkPEDTmkDes", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDTmkDes() {
    TestLog("TestsdkPEDTmkDes", "\r\n��ʼ����s32 sdkPEDTmkDes\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDTmkDes();

    //�������
    TestNormal_sdkPEDTmkDes();


    TestLog("TestsdkPEDTmkDes", "\r\n s32 sdkPEDTmkDes���Խ���\r\n ");
}

/********************************************************************sdkPEDTmkDes�������******************************************************************/

/**************************************sdkPEDWkDes���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDWkDes(void) {
//s32 sdkPEDWkDes(s32 iWkIndex, SDK_PED_DES_TYPE eWkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn, u8 *pheOut)

    u8 temp[32] = {0};

    TestLog("TestsdkPEDWkDes", " \r\n \r\n��ʼsdkPEDWkDes�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkPEDWkDes(0, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, NULL, NULL) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������",
                           (sdkPEDWkDes(126, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, temp, temp) ==
                            SDK_PARA_ERR));
    TestLog("TestsdkPEDWkDes", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDWkDes() {
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};

    memset(temp1, 0x31, sizeof(temp1));
    TestLog("TestsdkPEDWkDes", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDWkDes(0, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                            SDK_OK));


    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(0, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT, temp2, temp1) ==
                                SDK_OK));

        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des single��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPEDWkDes", "SDK_PED_DES_SINGLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDWkDes", "SDK_PED_DES_SINGLE En/Decrypt Fail\r\n");
        }
    }
    TestLog("TestsdkPEDWkDes", "SDK_PED_DES_TRIPLE\r\n");
    memset(temp1, 0x31, sizeof(temp1));
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDWkDes(2, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                            SDK_OK));

    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(2, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, temp2, temp1) ==
                                SDK_OK));
        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des triple��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPEDWkDes", "SDK_PED_DES_TRIPLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPEDWkDes", "SDK_PED_DES_TRIPLE En/Decrypt Fail\r\n");
        }
    }
    TestLog("TestsdkPEDWkDes", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDWkDes() {
    TestLog("TestsdkPEDWkDes", "\r\n��ʼ����s32 sdkPEDWkDes\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDWkDes();

    //�������
    TestNormal_sdkPEDWkDes();


    TestLog("TestsdkPEDWkDes", "\r\n s32 sdkPEDWkDes���Խ���\r\n ");
}

/********************************************************************sdkPEDWkDes�������******************************************************************/


/**************************************sdkPedWkEncryptWk���Կ�ʼ**********************************/
#if 0 /*Modify by ���ƽ at 2014.09.24  14:49 */
static void TestIllPar_sdkPedWkEncryptWk(void)
{
    u8 temp[32] = {0};

    TestLog("TestsdkPedWkEncryptWk", " \r\n \r\n��ʼsdkPedWkEncryptWk�Ƿ���������  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("�Ƿ���������", (sdkPedWkEncryptWk( SDK_PED_WK, SDK_PED_DES_SINGLE, 0, SDK_PED_TMK,SDK_PED_DES_SINGLE, 2, NULL ) == SDK_PARA_ERR));
    TestLog("TestsdkPedWkEncryptWk", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}
#endif /* if 0 */


static void TestNormal_sdkPedWkEncryptWk() {
    u8 temp1[30] = {0};
    u8 temp2[30] = {0};

    TestLog("TestsdkPedWkEncryptWk", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPedWkEncryptWk(SDK_PED_DES_SINGLE, 0, SDK_PED_DES_SINGLE, 1, 4) ==
                            SDK_OK));


    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0x31, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(4, SDK_PED_DES_SINGLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                                SDK_OK));
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(4, SDK_PED_DES_SINGLE, SDK_PED_DECRYPT, temp2, temp1) ==
                                SDK_OK));
        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des single��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPedWkEncryptWk", "SDK_PED_DES_SINGLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPedWkEncryptWk", "SDK_PED_DES_SINGLE En/Decrypt Fail\r\n");
        }
    }

    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPedWkEncryptWk(SDK_PED_DES_TRIPLE, 2, SDK_PED_DES_TRIPLE, 3, 5) ==
                            SDK_OK));


    if (sdkIsSupportPCI() == true) //shijianglong 2013.04.18 18:10
    {
        memset(temp1, 0x31, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(5, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, temp1, temp2) ==
                                SDK_OK));
        memset(temp1, 0, sizeof(temp1));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDWkDes(5, SDK_PED_DES_TRIPLE, SDK_PED_DECRYPT, temp2, temp1) ==
                                SDK_OK));
        memset(temp2, 0x31, sizeof(temp2));
        TEST_IS_TRUE_WITH_INFO("Wk des triple��������ܺ����ݱȽ�", (memcmp(temp1, temp2, 8) == 0));

        if (!memcmp(temp1, temp2, 8)) {
            TestLog("TestsdkPedWkEncryptWk", "SDK_PED_DES_SINGLE En/Decrypt Succ\r\n");
        }
        else {
            TestLog("TestsdkPedWkEncryptWk", "SDK_PED_DES_SINGLE En/Decrypt Fail\r\n");
        }
    }


    TestLog("TestsdkPedWkEncryptWk", "\r\n������Խ���\r\n ");
}

static void Test_sdkPedWkEncryptWk() {
    TestLog("TestsdkPEDWkDes", "\r\n��ʼ����s32 sdkPEDWkDes\r\n ");

#if 0 /*Modify by ���ƽ at 2014.09.24  14:49 �������Ƿ���������*/
    //�Ƿ���������
    TestIllPar_sdkPedWkEncryptWk();
#endif /* if 0 */

    //�������
    TestNormal_sdkPedWkEncryptWk();


    TestLog("TestsdkPedWkEncryptWk", "\r\n s32 sdkPedWkEncryptWk���Խ���\r\n ");
}
/********************************************************************sdkPedWkEncryptWk�������******************************************************************/




/**************************************sdkPEDInputPIN���Կ�ʼ**********************************/
#if 0 /*Modify by zxx 3.0 at 2013.02.28  10:17 */
static void TestIllPar_sdkPEDInputPIN(void)
{
//s32 sdkPEDInputPIN(const SDK_PED_PIN_CFG *pstPinCfg, u8 *pheOut)

    TestLog("TestsdkPEDInputPIN", " \r\n \r\n��ʼsdkPEDInputPIN�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkPEDInputPIN(NULL, NULL, NULL) == SDK_PARA_ERR));
    TestLog("TestsdkPEDInputPIN", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void  TestNormal_sdkPEDInputPIN()
{
    u8 temp[30] = {0};
    SDK_PED_PIN_CFG stPinCfg;

    memset(&stPinCfg, 0, sizeof(SDK_PED_PIN_CFG));
    stPinCfg.ePinMode = SDK_PED_MAG_PIN;
    stPinCfg.eKeyType = SDK_PED_DES_SINGLE;
    //memset(stPinCfg.hePan,0,8);//�������ʺ�
    memcpy(stPinCfg.hePan, "\x00\x00\x67\x89\x01\x23\x45\x67", 8);
    stPinCfg.iTimeOut = SDK_PED_TIMEOUT * 8;
    stPinCfg.ucTpkIndex = 0;
    TestLog("TestsdkPEDInputPIN", "\r\n������Կ�ʼ\r\n");
    sdkTestMsgBox("��ped����123456,��ȷ�ϼ���ʼ����", SDK_KEY_MASK_ALL);
    TEST_IS_TRUE_WITH_INFO("������������", (sdkPEDInputPIN(&stPinCfg, NULL, temp) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("����������ݱȽ�", (memcmp(&temp[1], "\x11\x09\x03\xaf\xea\xde\x72\xa3", 8)));

    if (!memcmp(&temp[1], "\x11\x09\x03\xaf\xea\xde\x72\xa3", 8))
    {
        TestLog("TestsdkPEDInputPIN", "Succ!!\r\n ");
    }
    else
    {
        TestLog("TestsdkPEDInputPIN", "Fail!!\r\n ");
        TestLogHex("TestsdkPEDInputPIN", "retdata", temp, 8);
    }
    TestLog("TestsdkPEDInputPIN", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDInputPIN()
{
    TestLog("TestsdkPEDInputPIN", "\r\n��ʼ����s32 sdkPEDInputPIN\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDInputPIN();

    //�������
    TestNormal_sdkPEDInputPIN();


    TestLog("TestsdkPEDInputPIN", "\r\n s32 sdkPEDInputPIN���Խ���\r\n ");
}

#endif /* if 0 */
/********************************************************************sdkPEDInputPIN�������******************************************************************/

/**************************************sdkPEDCalcMac���Կ�ʼ**********************************/
static void TestIllPar_sdkPEDCalcMac(void) {
//s32 sdkPEDCalcMac(s32 iKeyIndex, SDK_PED_MAC_MODE eMode, SDK_PED_DES_TYPE eKeyType, const u8 *pheIn, s32 iInLen, u8 *pheOut)
    u8 temp[128] = {0}, tmp[128] = {0};

    TestLog("TestsdkPEDCalcMac", " \r\n \r\n��ʼsdkPEDCalcMac�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkPEDCalcMac(0, SDK_PED_ECB, SDK_PED_DES_SINGLE, temp, 0, tmp) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������",
                           (sdkPEDCalcMac(126, SDK_PED_ECB, SDK_PED_DES_SINGLE, temp, 0, tmp) ==
                            SDK_PARA_ERR));
    TestLog("TestsdkPEDCalcMac", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

static void TestNormal_sdkPEDCalcMac() {
    u8 temp[1024] =
            {
                    0x02, 0x00, 0x30, 0x20, 0x04, 0x80, 0x30, 0xC0,
                    0x88, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                    0x00, 0x00, 0x01, 0x00, 0x00, 0x04, 0x02, 0x20,
                    0x00, 0x31, 0x23, 0x26, 0x88, 0x88, 0x00, 0x00
            };

    u8 ecbrslt[9] = {0x32, 0x30, 0x35, 0x44, 0x32, 0x34, 0x35, 0x45};
    u8 cbcrslt[9] = {0xCF, 0xEE, 0x38, 0x2C, 0xEB, 0xCD, 0x84, 0x1D};
    u8 x919rslt[9] = {0};

    u8 tmp[64] = {0};

    memset(&temp[32], 0x31, 500);
    memset(&temp[532], 0x35, 300);
    TestLog("TestsdkPEDCalcMac", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDCalcMac(1, SDK_PED_ECB, SDK_PED_DES_SINGLE, temp, 32, tmp) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("ecb calcmac test reslut", memcmp(tmp, ecbrslt, 8) == 0);

    strcpy(ecbrslt, "\x43\x46\x36\x32\x35\x33\x41\x41");
    TestLog("TestsdkPEDCalcMac", "\r\n������Կ�ʼ\r\n");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDCalcMac(1, SDK_PED_ECB, SDK_PED_DES_SINGLE, temp, 532, tmp) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("ecb calcmac test reslut", memcmp(tmp, ecbrslt, 8) == 0);

    strcpy(ecbrslt, "\x39\x43\x30\x35\x46\x35\x33\x33");
    TEST_IS_TRUE_WITH_INFO("������������",
                           (sdkPEDCalcMac(1, SDK_PED_ECB, SDK_PED_DES_SINGLE, temp, 832, tmp) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("ecb calcmac test reslut", memcmp(tmp, ecbrslt, 8) == 0);

    if (sdkIsSupportPCI() == true) {
        memset(tmp, 0, sizeof(tmp));
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDCalcMac(1, SDK_PED_CBC, SDK_PED_DES_SINGLE, temp, 32, tmp) !=
                                SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("cbc calcmac test reslut", memcmp(tmp, cbcrslt, 8) == 0);
    }
    //���ݲ�Ʒ�������ѧ��ָʾ��K370ʹ����ԿоƬ��������������֧��X919�㷨
    if (SDK_SYS_MACHINE_K370 != sdkSysGetMachineCode(NULL)) {
        memset(tmp, 0, sizeof(tmp));

        strcpy(x919rslt, "\x4A\xD8\x2D\xCB\xF4\x2A\x4F\x77");
        memset(temp, 0, sizeof(temp));
        memcpy(temp,
               "\x23\x00\x00\x10\x20\xc0\x00\x51\x90\x00\x00\x10\x31\x21\x54\x51\x31\x10\x00\x08\x30\x31\x30\x31\x30\x31\x30\x30\x30\x32\x30\x30",
               32);
        memcpy(&temp[32],
               "\x30\x31\x31\x37\x30\x30\x30\x31\x30\x30\x38\x30\x32\x30\x30\x30\x31\x31\x37\x30\x30\x30\x31\x00\x14\x46\x2d\x50\x4f\x53\x43\x20",
               32);
        memcpy(&temp[64], "\x33\x2e\x37\x2e\x30\x2e\x30\x35\x30\x30\x30\x30\x30\x30\x30\x00", 16);
        memset(&temp[80], 0x31, 500);
        memset(&temp[580], 0x35, 300);
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDCalcMac(1, SDK_PED_X919, SDK_PED_DES_SINGLE, temp, 80, tmp) !=
                                SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("X919 calcmac test reslut", memcmp(tmp, x919rslt, 8) == 0);

        strcpy(x919rslt, "\x0C\xFC\x01\x9C\x7C\x7F\xA5\x08");
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDCalcMac(1, SDK_PED_X919, SDK_PED_DES_SINGLE, temp, 832,
                                              tmp) != SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("X919 calcmac test reslut", memcmp(tmp, x919rslt, 8) == 0);

/*=======BEGIN: taowz 2014.05.23  10:52 modify===========*/
        memcpy(x919rslt, "\xAB\x0E\x2B\x05\x00\x8D\x26\x91", 8);
        TEST_IS_TRUE_WITH_INFO("������������",
                               (sdkPEDCalcMac(2, SDK_PED_X919, SDK_PED_DES_TRIPLE, temp, 32, tmp) !=
                                SDK_PARA_ERR));
        TEST_IS_TRUE_WITH_INFO("X919 calcmac test reslut", memcmp(tmp, x919rslt, 8) == 0);
/*====================== END======================== */

    }
    TestLog("TestsdkPEDCalcMac", "\r\n������Խ���\r\n ");
}

static void Test_sdkPEDCalcMac() {
    TestLog("TestsdkPEDCalcMac", "\r\n��ʼ����s32 sdkPEDCalcMac\r\n ");

    //�Ƿ���������
    TestIllPar_sdkPEDCalcMac();

    //�������
    TestNormal_sdkPEDCalcMac();


    TestLog("TestsdkPEDCalcMac", "\r\n s32 sdkPEDCalcMac���Խ���\r\n ");
}

extern SDK_SYS_INITIAL_INFO gstInitial;
/********************************************************************sdkPEDCalcMac�������******************************************************************/



/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.10.11
** Remarks:
*****************************************************************************/
static s32 Test_sdkOutPinPadTest(void) {
    SDK_SYS_INITIAL_INFO stinitial;
    s32 key = 0;
    char tem[256];
    s32 machina = sdkSysGetMachineCode(NULL);

    stinitial.bIsHavePinPad = true;
    stinitial.bIsINRf = 1;
    sdkSysInitParam(&stinitial);

    while (sdkPEDGetPedVersion(tem) <= 0) {
        key = sdkDispMsgBox("��ʾ��Ϣ", "�����������̿�ʼ����������̲���\nȡ���˳�����", 0,
                            SDK_KEY_MASK_ALL);

        if (key == SDK_KEY_ESC) {
            Verify(sdkSysInitParam(&gstInitial) >= 0);
            return SDK_ESC;
        }
    }

    //���ò���
    Test_sdkPEDInitKeyIC();
    sdkPrintInit();
    Test_sdkPrintTestInfo("��������Զ�����ģ��", "NULL");
    Test_sdkPEDGetPedVersion();
    sdkPrintStart();
    // Test_Out_Private_sdkPEDSetSN();
    sdkTestShowProcessing("�����10%...");
    Test_sdkPEDUpdateTmk();
    Test_sdkPEDUpdateWk();
    sdkTestShowProcessing("�����30%...");
    Test_sdkPEDTmkDes();
    sdkTestShowProcessing("�����40%...");
    Test_sdkPEDWkDes();
    sdkTestShowProcessing("�����60%...");
    Test_sdkPEDCalcMac();
    key = sdkDispMsgBox("��ʾ��Ϣ", "�����Ƿ��������Կ�Լ�������Կ��ѭ������? ���ѵ�ʱ��Ƚϳ� 5����Զ�ѭ������",
                        5000, SDK_KEY_MASK_ALL);

    if (key == SDK_KEY_ENTER || key == SDK_TIME_OUT) {
        sdkTestShowProcessing("�����70%...");
        Test_sdkPEDUpdateTmkLoop();
        sdkTestShowProcessing("�����80%...");
        Test_sdkPEDUpdateWkLoop();
    }
}

//=====================================================���������������===================================================


void Test_SdkPedAuto(void) {
    SDK_SYS_INITIAL_INFO stinitial;
    s32 key = 0;
    char tem[256];
    s32 machina = sdkSysGetMachineCode(NULL);

    if (machina != SDK_SYS_MACHINE_K350 &&
        machina != SDK_SYS_MACHINE_K301FZ &&
        machina != SDK_SYS_MACHINE_K301FZE &&
        machina != SDK_SYS_MACHINE_K501AM) //huacong 2013.03.28 11:48//K501����Ҫ����Ĵ���
    {
        //���ò���  //wuweiwei 20130221
        stinitial.bIsHavePinPad = false;
        stinitial.bIsINRf = 1;
        sdkSysInitParam(&stinitial);
        Test_sdkPEDUpdateTmk();
        sdkTestShowProcessing("�����80%...");
        Test_sdkPEDUpdateWk();
        Test_sdkPEDTmkDes();
        Test_sdkPEDWkDes();
        Test_sdkPedWkEncryptWk();
        Test_sdkPEDCalcMac();
    }

    Test_sdkPEDDesEx();
    Test_sdkPEDUpdateWkEx();
    Test_sdkPEDUpdateTmkEx();
    sdkTestShowProcessing("�����100%...");
    Trace("zjp", "________________________________________________\r\n");
    //K370,G2,200P����ֻ��������
    if (!((machina == SDK_SYS_MACHINE_K370) || (machina == SDK_SYS_MACHINE_K370P)
          || (machina == SDK_SYS_MACHINE_G2) || (machina == SDK_SYS_MACHINE_200P))) {
        Test_sdkOutPinPadTest();
    }
    Verify(sdkSysInitParam(&gstInitial) >= 0);  //�ָ�Ϊ����
    /*���Ҫ�鿴������Ϣ�����Դ������һ��*/
    //sdkDispMsgBox("��ʾ��Ϣ", "�����������̲鿴������Ϣ����ȷ�ϼ�����", 0, SDK_KEY_MASK_ENTER); //huacong 2013.04.11 9:32

    return;
}

