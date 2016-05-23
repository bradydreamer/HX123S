#include "sdkGlobal.h"
#include "SdkTest.h"

#if 1 //del by sujianzhong 2013.09.27

static void TestIllPar_sdkIccCloseRfDev() {

    TestLog("mem", "��ʼsdkIccCloseRfDev�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("return value test", sdkIccCloseRfDev() == SDK_OK);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccCloseRfDev() {
    TestLog("mem", "test start :  sdkIccCloseRfDev\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccCloseRfDev();

    TestLog("mem", "sdkIccCloseRfDev\r\n ");
}

#endif /* if 0 */

#if 1 //del by sujianzhong 2013.09.27

static void TestIllPar_sdkIccOpenRfDev() {

    TestLog("mem", "��ʼsdkIccOpenRfDev�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("return value test", sdkIccOpenRfDev() == SDK_OK);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccOpenRfDev() {
    TestLog("mem", "test start :  sdkIccOpenRfDev\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccOpenRfDev();

    TestLog("mem", "sdkIccOpenRfDev\r\n ");
}

#endif /* if 0 */

#if 1 //del by sujianzhong 2013.09.27

static void TestIllPar_sdkIccCloseIcDev() {

    TestLog("mem", "��ʼsdkIccCloseIcDev�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("return value test", sdkIccCloseIcDev() == SDK_OK);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccCloseIcDev() {
    TestLog("mem", "test start :  sdkIccCloseIcDev\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccCloseIcDev();

    TestLog("mem", "sdkIccCloseIcDev\r\n ");
}

#endif

#if 1 //del by sujianzhong 2013.09.27

static void TestIllPar_sdkIccOpenIcDev() {

    TestLog("mem", "��ʼsdkIccOpenIcDev�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("return value test", sdkIccOpenIcDev() == SDK_OK);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccOpenIcDev() {
    TestLog("mem", "test start :  sdkIccOpenIcDev\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccOpenIcDev();

    TestLog("mem", "sdkIccOpenIcDev\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemPowerOn() {

    u8 temp[128] = {0};
    s32 len = 0;
    SDK_ICC_PARAM psticcparam;
    TestLog("mem", "��ʼsdkIccMemPowerOn�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMemPowerOn(NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemPowerOn(&psticcparam, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemPowerOn(&psticcparam, temp, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemPowerOn() {
    TestLog("mem", "test start :  sdkIccMemPowerOn\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemPowerOn();

    TestLog("mem", "sdkIccMemPowerOn\r\n ");
}

#endif /* if 0 */

#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccIsIccCardIn() {

    u8 temp[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccIsIccCardIn�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccIsIccCardIn(3000, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccIsIccCardIn(3000, temp, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccIsIccCardIn() {
    TestLog("mem", "test start :  sdkIccIsIccCardIn\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccIsIccCardIn();

    TestLog("mem", "sdkIccIsIccCardIn\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMifareBackUpBlock() {

    u8 temp[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccMifareBackUpBlock�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareBackUpBlock(1, 3, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareBackUpBlock(1, 3, temp, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMifareBackUpBlock() {
    TestLog("mem", "test start :  sdkIccMifareBackUpBlock\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMifareBackUpBlock();

    TestLog("mem", "sdkIccMifareBackUpBlock\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMifareOperateBlock() {

    u8 temp[128] = {0}, buf[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccMifareOperateBlock�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareOperateBlock(1, 1, NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareOperateBlock(1, 1, temp, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareOperateBlock(1, 1, temp, buf, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMifareOperateBlock() {
    TestLog("mem", "test start :  sdkIccMifareOperateBlock\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMifareOperateBlock();

    TestLog("mem", "sdkIccMifareOperateBlock\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdklccMifareWriteBlock() {

    u8 temp[128] = {0}, buf[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdklccMifareWriteBlock�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareWriteBlock(1, NULL, 5, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("len = 0 test",
                           sdkIccMifareWriteBlock(1, temp, 0, buf, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareWriteBlock(1, temp, 5, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareWriteBlock(1, temp, 5, buf, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdklccMifareWriteBlock() {
    TestLog("mem", "test start :  sdkIccMifareWriteBlock\r\n ");

    //�Ƿ���������
    TestIllPar_sdklccMifareWriteBlock();

    TestLog("mem", "sdkIccMifareWriteBlock\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdklccMifareReadBlock() {

//    u8 temp[128] = {0};
//    s32 len = 0;
    TestLog("mem", "��ʼsdklccMifareReadBlock�Ƿ��������� \r\n");

    //��ʱŪ��//shiweisong 2013.08.15 11:41
    //  TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMifareWriteBlock(1,NULL,len) == SDK_PARA_ERR);
    // TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMifareWriteBlock(1,temp,NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdklccMifareReadBlock() {
    TestLog("mem", "test start :  sdkIccMifareWriteBlock\r\n ");

    //�Ƿ���������
    TestIllPar_sdklccMifareReadBlock();

    TestLog("mem", "sdkIccMifareWriteBlock\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMifareVerifyKey() {

    u8 temp[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccMifareVerifyKey�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareVerifyKey(1, 2, NULL, 3, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("len = 0 test",
                           sdkIccMifareVerifyKey(1, 2, "\xFF\xFF\xFF\xFF\xFF\xFF", 0, temp, &len) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareVerifyKey(1, 2, "\xFF\xFF\xFF\xFF\xFF\xFF", 6, NULL, &len) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMifareVerifyKey(1, 2, "\xFF\xFF\xFF\xFF\xFF\xFF", 6, temp, NULL) ==
                           SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMifareVerifyKey() {
    TestLog("mem", "test start :  sdkIccMifareVerifyKey\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMifareVerifyKey();

    TestLog("mem", "sdkIccMifareVerifyKey\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMifareReadCardNum() {
    TestLog("mem", "��ʼsdkIccMifareReadCardNum�Ƿ��������� \r\n");

    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMifareReadCardNum(NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMifareReadCardNum() {
    TestLog("mem", "test start :  sdkIccMifareReadCardNum\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMifareReadCardNum();

    TestLog("mem", "sdkIccMifareReadCardNum\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMem4442ReadPwdAndPwdCount() {

    u8 temp[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccMem4442ReadPwdAndPwdCount�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMem4442ReadPwdAndPwdCount(NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMem4442ReadPwdAndPwdCount(temp, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMem4442ReadPwdAndPwdCount() {
    TestLog("mem", "test start :  sdkIccMem4442ReadPwdAndPwdCount\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMem4442ReadPwdAndPwdCount();

    TestLog("mem", "sdkIccMem4442ReadPwdAndPwdCount\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemModifyPwd() {

    u8 temp[128] = {0};
    s32 len = 0;
    SDK_ICC_PARAM psticcparam;
    TestLog("mem", "��ʼsdkIccMemModifyPwd�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMemModifyPwd(NULL, "\xFF\xFF\xFF\xFF\xFF\xFF",
                                                                   "\xFF\xFF\xFF\xFF\xFF\x99", temp,
                                                                   &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemModifyPwd(&psticcparam, NULL, "\xFF\xFF\xFF\xFF\xFF\x99", temp,
                                              &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemModifyPwd(&psticcparam, "\xFF\xFF\xFF\xFF\xFF\xFF", NULL, temp,
                                              &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemModifyPwd(&psticcparam, "\xFF\xFF\xFF\xFF\xFF\xFF",
                                              "\xFF\xFF\xFF\xFF\xFF\x99", NULL, &len) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemModifyPwd(&psticcparam, "\xFF\xFF\xFF\xFF\xFF\xFF",
                                              "\xFF\xFF\xFF\xFF\xFF\x99", temp, NULL) ==
                           SDK_PARA_ERR);


    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemModifyPwd() {
    TestLog("mem", "test start :  sdkIccMemModifyPwd\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemModifyPwd();

    TestLog("mem", "sdkIccMemModifyPwd\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemCheckPwd() {

    u8 temp[128] = {0};
    s32 len = 0;
    SDK_ICC_PARAM psticcparam;
    TestLog("mem", "��ʼsdkIccMemCheckPwd�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemCheckPwd(NULL, "\xFF\xFF\xFF\xFF\xFF\xFF", temp, &len) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemCheckPwd(&psticcparam, NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemCheckPwd(&psticcparam, "\xFF\xFF\xFF\xFF\xFF\xFF", NULL,
                                             &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemCheckPwd(&psticcparam, "\xFF\xFF\xFF\xFF\xFF\xFF", temp,
                                             NULL) == SDK_PARA_ERR);


    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemCheckPwd() {
    TestLog("mem", "test start :  sdkIccMemCheckPwd\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemCheckPwd();

    TestLog("mem", "sdkIccMemCheckPwd\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemWriteData() {

    u8 temp[128] = {0};
    s32 len = 0;
    SDK_ICC_WMEMIN pstwmemin;
    SDK_ICC_PARAM psticcparam;
    TestLog("mem", "��ʼsdkIccMemWriteData�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    memset(&pstwmemin, 0, sizeof(pstwmemin));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemWriteData(NULL, &pstwmemin, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemWriteData(&psticcparam, NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemWriteData(&psticcparam, &pstwmemin, NULL, &len) ==
                           SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemWriteData(&psticcparam, &pstwmemin, temp, NULL) ==
                           SDK_PARA_ERR);


    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemWriteData() {
    TestLog("mem", "test start :  sdkIccMemWriteData\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemWriteData();

    TestLog("mem", "sdkIccMemWriteData\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemReadData() {

    u8 temp[128] = {0};
    s32 len = 0;
    SDK_ICC_RMEMIN pstmemin;
    SDK_ICC_PARAM psticcparam;
    TestLog("mem", "��ʼsdkIccMemReadData�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    memset(&pstmemin, 0, sizeof(pstmemin));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemReadData(NULL, &pstmemin, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemReadData(&psticcparam, NULL, temp, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemReadData(&psticcparam, &pstmemin, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccMemReadData(&psticcparam, &pstmemin, temp, NULL) == SDK_PARA_ERR);


    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemReadData() {
    TestLog("mem", "test start :  sdkIccMemReadData\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemReadData();

    TestLog("mem", "sdkIccMemReadData\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccMemCheck() {

    u8 temp[128] = {0};
    s32 len = 0;
    TestLog("mem", "��ʼsdkIccMemCheck�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMemCheck(NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMemCheck(temp, NULL) == SDK_PARA_ERR);

    TestLog("mem", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemCheck() {
    TestLog("mem", "test start :  sdkIccMemCheck\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemCheck();

    TestLog("mem", "sdkIccMemCheck\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccSendData() {

    u8 buf[128] = {0};
    s32 len = 0;
    SDK_ICC_PARAM psticcparam;
    TestLog("mifare", "��ʼsdkIccSendData�Ƿ��������� \r\n");
    memset(&psticcparam, 0, sizeof(psticcparam));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccSendData(NULL, "123456", 6, buf, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccSendData(&psticcparam, NULL, 6, buf, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("len < 0 test",
                           sdkIccSendData(&psticcparam, "123456", 0, buf, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccSendData(&psticcparam, "123456", 6, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccSendData(&psticcparam, "123456", 6, buf, NULL) == SDK_PARA_ERR);
    TestLog("mifare", "�Ƿ������������\r\n");
}

static void Test_sdkIccSendData() {
    TestLog("mifare", "test start :  sdkIccSendData\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccSendData();

    TestLog("mifare", "sdkIccSendData\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.29  10:42 */

static void TestIllPar_sdkIccMemMifarePowerDown() {
    TestLog("mifare", "��ʼsdkIccMemMifarePowerDown�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccMemMifarePowerDown(NULL) == SDK_PARA_ERR);
    TestLog("mifare", "�Ƿ������������\r\n");
}

static void Test_sdkIccMemMifarePowerDown() {
    TestLog("mifare", "test start :  sdkIccMemMifarePowerDown\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccMemMifarePowerDown();

    TestLog("mifare", "sdkIccMemMifarePowerDown\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:59 */

static void TestIllPar_sdkIccReset() {
    u8 temp[20] = {0};
    s32 len = 0;
    SDK_ICC_PARAM psticcparam;

    TestLog("mifare", "��ʼsdkIccReset�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccReset(NULL, temp, &len) == SDK_PARA_ERR);
    memset(&psticcparam, 0, sizeof(psticcparam));
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccReset(&psticcparam, NULL, &len) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("timer fault test",
                           sdkIccReset(&psticcparam, temp, NULL) == SDK_PARA_ERR);
    TestLog("mifare", "�Ƿ������������\r\n");
}

static void Test_sdkIccReset() {
    TestLog("mifare", "test start :  sdkIccReset\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccReset();

    TestLog("mifare", "sdkIccReset\r\n ");
}

#endif /* if 0 */


#if 1 /*Modify by shijianglong at 2013.01.28  14:49 */

static void TestIllPar_sdkIccRFQuery() {
    u8 temp[20] = {0};

    u8 *p = NULL;                  //zhongjiahong 2013.11.20 14:32

    TestLog("mifare", "��ʼsdkIccRFQuery�Ƿ��������� \r\n");
    TEST_IS_TRUE_WITH_INFO("NULL pointer test", sdkIccRFQuery(9, p, 3000) ==
                                                SDK_PARA_ERR);           //zhongjiahong 2013.11.20 14:33
    TEST_IS_TRUE_WITH_INFO("NULL pointer test",
                           sdkIccRFQuery(SDK_ICC_RFCARD_A, NULL, 3000) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("timer fault test",
                           sdkIccRFQuery(SDK_ICC_RFCARD_A, temp, -1) == SDK_PARA_ERR);
    TestLog("mifare", "�Ƿ������������\r\n");
}

static void Test_sdkIccRFQuery() {
    TestLog("mifare", "test start :  sdkIccRFQuery\r\n ");

    //�Ƿ���������
    TestIllPar_sdkIccRFQuery();

    TestLog("mifare", "sdkIccRFQuery\r\n ");
}

#endif /* if 0 */


void TestSdkMifareMain(void) {
    if (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)//huacong 2013.03.28 11:48
    {
        sdkTestMsgBox("�û����ݲ�֧��Mifareģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return;
    }
    else {
        TestLog("TestMifare", " ��ʼMifare��������  \r\n");

        Test_sdkIccRFQuery();
        Test_sdkIccReset();
        Test_sdkIccMemMifarePowerDown();
        Test_sdkIccSendData();
        Test_sdkIccMifareReadCardNum();
        Test_sdkIccMifareVerifyKey();
        Test_sdklccMifareReadBlock();
        Test_sdklccMifareWriteBlock();
        Test_sdkIccMifareOperateBlock();
        Test_sdkIccMifareBackUpBlock();
        Test_sdkIccIsIccCardIn();
        Test_sdkIccOpenRfDev();
        Test_sdkIccCloseRfDev();
        TestLog("TestMifare", " Mifare  �������Խ��� \r\n");
        return;
    }
}

void TestSdkMemoryMain(void) {
    if (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K501AM)//huacong 2013.03.28 11:48
    {
        sdkTestMsgBox("�û����ݲ�֧��Memoryģ��,��ȷ�ϼ���������", SDK_KEY_MASK_ENTER);
        return;
    }
    else {
        TestLog("TestMem", " ��ʼMem��������  \r\n");
        Test_sdkIccMemCheck();
        Test_sdkIccMemReadData();
        Test_sdkIccMemWriteData();
        Test_sdkIccMemCheckPwd();
        Test_sdkIccMemModifyPwd();
        Test_sdkIccMem4442ReadPwdAndPwdCount();
        Test_sdkIccMemPowerOn();
        Test_sdkIccOpenIcDev();
        Test_sdkIccCloseIcDev();
        TestLog("TestMem", " Mem  �������Խ��� \r\n");
        return;
    }
}


