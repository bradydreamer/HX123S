#include "sdkGlobal.h"
#include "SdkTest.h"

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: �汾��Ϣ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
#if 0 /*Modify by fusuipu at 2013.01.30  11:50 */
static void Test_sdkCommGetVersion(void)
{
    u8 temp[30] = {0};

    TestLog("TestMath" , " \r\n \r\n(sdkMathsGetVersion()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����" , (sdkCommGetVersion(NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("��������" , (sdkCommGetVersion(temp) == strlen(temp)));
    TestLog("TestComm" , "ͨѶģ��汾=  %s\r\n" , temp);
    TestLog("TestComm" , " \r\n \r\n Test_sdkCommGetVersion������� \r\n \r\n");
}
#endif /* if 0 */

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ������չ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommSendModemCfgEx(void) {
    SDK_COMM_STMODEMEXPARAM pstModemExParam;
    SDK_COMM_STCOMMPARAM pstCommUartParam;

    memset(&pstModemExParam, 0x00, sizeof(SDK_COMM_STMODEMEXPARAM));
    TestLog("TestComm", " \r\n \r\n(sdkCommSendModemCfgEx()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommSendModemCfgEx(0, NULL) == SDK_PARA_ERR));
    TestLog("TestComm", "start test normal fun!\n");

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    sdkCommInit(&pstCommUartParam);

    TEST_IS_TRUE_WITH_INFO("����ֵ����", (sdkCommSendModemCfgEx(1, &pstModemExParam) == SDK_OK));
    TestLog("TestComm", " \r\n \r\n Test_sdkCommSendModemCfgEx������� \r\n \r\n");

}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: �Ͽ���·����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommDestoryLink(void) {
    ;//��
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ����ͨѶ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommConfig(void) {
    SDK_COMM_STCOMMPARAM pstCommUartParam;

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));

    TestLog("TestComm", " \r\n \r\n(sdkCommConfig()) starts  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommConfig(NULL) == SDK_PARA_ERR));

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    pstCommUartParam.eMode = 8;
    TEST_IS_TRUE_WITH_INFO("�����������", (sdkCommConfig(&pstCommUartParam) == SDK_PARA_ERR));

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    strcpy(pstCommUartParam.stCommInfo.stModemInfo.asPhone[0], "1234567890123456789012345678");
    TEST_IS_TRUE_WITH_INFO("�����������", (sdkCommConfig(&pstCommUartParam) == SDK_PARA_ERR));

    TestLog("TestComm", " \r\n \r\n Test_sdkCommConfig ������� \r\n \r\n");
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ��ʼ������
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommInit(void) {
    SDK_COMM_STCOMMPARAM pstCommUartParam;

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));

    TestLog("TestComm", " \r\n \r\n(sdkCommInit()) starts  \r\n \r\n");

    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommInit(NULL) == SDK_PARA_ERR));


    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    pstCommUartParam.eMode = 8;
    TEST_IS_TRUE_WITH_INFO("�����������", (sdkCommInit(&pstCommUartParam) == SDK_PARA_ERR));

    memset(&pstCommUartParam, 0x00, sizeof(pstCommUartParam));
    strcpy(pstCommUartParam.stCommInfo.stModemInfo.asPhone[0], "1234567890123456789012345678");
    TEST_IS_TRUE_WITH_INFO("�����������", (sdkCommInit(&pstCommUartParam) == SDK_PARA_ERR));

    TestLog("TestComm", " \r\n \r\n Test_sdkCommConfig ������� \r\n \r\n");
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: Ԥ���Ų���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommPre(void) {
    ;//��
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommCreateLink(void) {
    ;//��
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: MODEM��������
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommSendData(void) {
    ;//��

}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������:
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommTmsRecvData(void) {
    ;//��
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: �򿪴��ڲ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommOpenUart(void) {
    TestLog("TestComm", " \r\n \r\n(sdkCommOpenUart()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommOpenUart(NULL) == SDK_PARA_ERR));
    TestLog("TestComm", " \r\n \r\n Test_sdkCommCheckUartState������� \r\n \r\n");
}


/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: �򿪴��ڲ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommGetPppIP(void) {
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommGetPppIP(NULL) == SDK_PARA_ERR));
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: �رմ��ڲ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommCloseUart(void) {
    ;//��
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ���ڷ������ݲ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommUartSendData(void) {
    u8 uccom;
    uccom = 0;
    TestLog("TestComm", " \r\n \r\n(sdkCommUartSendData()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommUartSendData(uccom, NULL, 10) == SDK_PARA_ERR));
    TestLog("TestComm", " \r\n \r\n Test_sdkCommUartSendData������� \r\n \r\n");

}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ���ڽ������ݲ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkCommUartRecvData(void) {
    u8 uccom;
    uccom = 0;

    TestLog("TestComm", " \r\n \r\n(sdkCommUartRecvData()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommUartRecvData(uccom, NULL, 10, 10) == SDK_PARA_ERR));
    TestLog("TestComm", " \r\n \r\n Test_sdkCommUartRecvData������� \r\n \r\n");

}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ����ͨѶ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkUart(void) {
    TestLog("TestComm", " \r\n ���ڲ��ݲ���  starts\r\n \r\n");

    T_sdkCommOpenUart();       //�򿪴��ڲ���
    T_sdkCommCloseUart();      //�رմ��ڲ���
    T_sdkCommUartSendData();   //���ڷ������ݲ���
    T_sdkCommUartRecvData();   //���ڽ������ݲ���

    TestLog("TestComm", " \r\n ���ڲ��ݲ������  \r\n \r\n");
}

/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ����ͨѶ����
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkModem(void) {
    T_sdkCommSendModemCfgEx();//��չ��������
    T_sdkCommGetPppIP();
    T_sdkCommConfig();
    T_sdkCommInit();
    sdkTestShowProcessing("�����50%...");
    T_sdkCommPre();           //Ԥ����
    T_sdkCommCreateLink();   //��������
    T_sdkCommDestoryLink();  //��������
    T_sdkCommSendData();     //��������
    T_sdkCommTmsRecvData();  //��������

}


/********************************************************************
 ��    ��: �����
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: IP��ַ���
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
static void T_sdkIsRightIP(void) {

    Trace("TestIsRightIP", " \r\n \r\n(T_sdkIsRightIP()) starts  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommOpenUart(NULL) == SDK_PARA_ERR));


    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkCommIsRightIP(NULL) == false));
    TEST_IS_TRUE_WITH_INFO("��ȷIP����", (sdkCommIsRightIP("192.168.1.5") == true));
    //TEST_IS_TRUE_WITH_INFO("����IP����" , (sdkCommIsRightIP("255.168.1.5") == false));
    //TEST_IS_TRUE_WITH_INFO("����IP����" , (sdkCommIsRightIP("255.255.255.255") == false));
    //TEST_IS_TRUE_WITH_INFO("����IP����" , (sdkCommIsRightIP("192.168.0.0") == false));
    //TEST_IS_TRUE_WITH_INFO("����IP����" , (sdkCommIsRightIP("0.168.1.5") == false));
    TEST_IS_TRUE_WITH_INFO("����IP����", (sdkCommIsRightIP("256.168.1.5") == false));
    //TEST_IS_TRUE_WITH_INFO("����IP����" , (sdkCommIsRightIP("0.0.0.0") == false));
    TEST_IS_TRUE_WITH_INFO("����IP����", (sdkCommIsRightIP("a.a.a.a") == false));
    TEST_IS_TRUE_WITH_INFO("����IP����", (sdkCommIsRightIP("a.1.a.a") == false));
    TEST_IS_TRUE_WITH_INFO("����IP����", (sdkCommIsRightIP("a.a.a.a") == false));


    Trace("TestIsRightIP", " \r\n \r\n T_sdkIsRightIP������� \r\n \r\n");

}


/********************************************************************
 ��    ��: ������
 ��    Ȩ: �������¹��������ɷ����޹�˾
 ��������: ���Գ���������
 �������:
 �������:
 �� �� ֵ:
 ��    ע:
 �޸���Ϣ:
********************************************************************/
void Test_sdkcommauto(void) {

    TestLog("TestComm", "\r\n��ʼͨѶ����\r\n ");


//    Test_sdkCommGetVersion(); //�汾����      //fusuipu modefied at 2013-1-30
    T_sdkUart();           //����ͨѶ����
    T_sdkModem();          //modemͨѶ����
    T_sdkIsRightIP();        //IP���


    TestLog("TestComm", "\r\n����ͨѶ����\r\n ");

}

