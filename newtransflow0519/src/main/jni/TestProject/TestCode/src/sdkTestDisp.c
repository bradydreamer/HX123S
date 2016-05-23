#include "sdkGlobal.h"
#include "SdkTest.h"

#define SDK_DISP_NEWATR (SDK_DISP_OVERLINE|SDK_DISP_DOWNLINE|SDK_DISP_SIDELINE)

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *pFile
** Returned value:	
** Created By:		huacong  2013.04.10
** Remarks: 		
*****************************************************************************/
void IN_sdkTest_sdkDispSetBackground(u8 *pFile) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();

    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispSetBackground(0, 0, pixel.siX, pixel.siY, pFile) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkDispSetBackground(0, 0, pixel.siX + 5, pixel.siY + 5, NULL) ==
                            SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispSetBackground(0, 0, pixel.siX + 5, pixel.siY + 5, pFile) ==
                            SDK_PARA_ERR));
    /*
       NULL pointer testing
     */
    //���һ���ǿ���null��
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkDispSetBackground(0, 0, pixel.siX, pixel.siY, NULL) == SDK_OK));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 0)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 1)",
                           (sdkDispSetBackground(1, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 60)",
                           (sdkDispSetBackground(60, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0= pixel.siX-1)",
                           (sdkDispSetBackground(pixel.siX - 1, 0, pixel.siX - 1, pixel.siY - 1,
                                                 pFile) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 0)",
                           (sdkDispSetBackground(0, 0, 0, pixel.siY - 1, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 1)",
                           (sdkDispSetBackground(0, 0, 1, pixel.siY - 1, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 60)",
                           (sdkDispSetBackground(0, 0, 60, pixel.siY - 1, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = pixel.siX)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));


    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 0)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 1)",
                           (sdkDispSetBackground(0, 1, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 32)",
                           (sdkDispSetBackground(0, 32, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = pixel.siY-1)",
                           (sdkDispSetBackground(0, pixel.siY - 1, pixel.siX - 1, pixel.siY - 1,
                                                 pFile) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 0)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, 0, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 1)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, 1, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 32)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, 32, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = pixel.siY)",
                           (sdkDispSetBackground(0, 0, pixel.siX - 1, pixel.siY - 1, pFile) ==
                            SDK_OK));
}


void IN_sdkTest_sdkDispShowBmp(u8 *pFile) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispShowBmp/ ��ʾͼƬ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispShowBmp(0, 0, 60, 60, pFile) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(x0  = pixel.siX)",
                           (sdkDispShowBmp(pixel.siX, 0, 129, 65, pFile) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(Y0 = pixel.siY)",
                           (sdkDispShowBmp(0, pixel.siY, 129, 65, pFile) == SDK_PARA_ERR));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkDispShowBmp(0, 0, 129, 65, NULL) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 0)", (sdkDispShowBmp(0, 0, 127, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 1)", (sdkDispShowBmp(1, 0, 126, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0 = 60)", (sdkDispShowBmp(60, 0, 67, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x0= pixel.siX-1)",
                           (sdkDispShowBmp(pixel.siX - 1, 0, 0, 63, pFile) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 0)", (sdkDispShowBmp(0, 0, 0, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 1)", (sdkDispShowBmp(0, 0, 1, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = 60)", (sdkDispShowBmp(0, 0, 60, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(xlen = pixel.siX)",
                           (sdkDispShowBmp(0, 0, pixel.siX - 1, 63, pFile) == SDK_OK));


    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 0)", (sdkDispShowBmp(0, 0, 127, 63, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 1)", (sdkDispShowBmp(0, 1, 127, 62, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 32)", (sdkDispShowBmp(0, 32, 127, 31, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = pixel.siY-1)",
                           (sdkDispShowBmp(0, pixel.siY - 1, 127, 0, pFile) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 0)", (sdkDispShowBmp(0, 0, 127, 0, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 1)", (sdkDispShowBmp(0, 0, 127, 1, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = 32)",
                           (sdkDispShowBmp(0, 0, 127, 32, pFile) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(ylen  = pixel.siY)",
                           (sdkDispShowBmp(0, 0, 127, pixel.siY - 1, pFile) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispFillRowRam
   ��������:	����API����sdkDispFillRowRam
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 14:21:31
*******************************************************************/
void sdkTest_sdkDispFillRowRam(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispFillRowRam/ �����ʾ����  \r\n \r\n");


    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Normal Testing",
                                                          SDK_DISP_DEFAULT) == SDK_OK));
    /*
       NULL pointer testing
     */
    TestLog("NULL pointer testing start", "NULL pointer testing start");
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, 0, NULL, SDK_DISP_DEFAULT) ==
                            SDK_PARA_ERR));
    TestLog("NULL pointer testing end", "NULL pointer testing end");
    /*
       not visible character tesing
     */
//    TEST_IS_TRUE_WITH_INFO ("���ɼ��ַ�����", (sdkDispFillRowRam(SDK_DISP_LINE3, 0, "\x80\xFF", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    /*
       parameter error testing (example model K320)
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row = 5)",
                           (sdkDispFillRowRam(5, 0, "Testing", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(row = -1)",
                           (sdkDispFillRowRam(-1, 0, "Testing", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(colid = pixel.siX)",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, pixel.siX, "Testing",
                                              SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(colid = 0)",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, 0, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(colid = 1)",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, 1, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(colid = 60)",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(colid = pixel.siX - 1)",
                           (sdkDispFillRowRam(SDK_DISP_LINE3, pixel.siX - 1, "Testing",
                                              SDK_DISP_DEFAULT) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 0)",
                           (sdkDispFillRowRam(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row= 1)",
                           (sdkDispFillRowRam(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 2)",
                           (sdkDispFillRowRam(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 3)",
                           (sdkDispFillRowRam(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 4)",
                           (sdkDispFillRowRam(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));

    /*���������µ����ԵĲ��Դ���*/
    TEST_IS_TRUE_WITH_INFO("�������Բ���", (sdkDispFillRowRam(SDK_DISP_LINE1, 0, "Testing",
                                                             SDK_DISP_OVERLINE | SDK_DISP_LDISP) ==
                                           SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�������Բ���", (sdkDispFillRowRam(SDK_DISP_LINE2, 2, "Testing",
                                                             SDK_DISP_DOWNLINE | SDK_DISP_RDISP) ==
                                           SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�������Բ���", (sdkDispFillRowRam(SDK_DISP_LINE3, 2, "Testing",
                                                             SDK_DISP_SIDELINE | SDK_DISP_CDISP) ==
                                           SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�������Բ���", (sdkDispFillRowRam(SDK_DISP_LINE4, 2, "Testing",
                                                             SDK_DISP_NEWATR | SDK_DISP_CDISP) ==
                                           SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�������Բ���", (sdkDispFillRowRam(SDK_DISP_LINE5, 64, "Testing",
                                                             SDK_DISP_NEWATR | SDK_DISP_CDISP) ==
                                           SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispAt
   ��������:	����API����sdkDispAt
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 14:51:39
*******************************************************************/
void sdkTest_sdkDispAt(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispAt/ ��ָ��������ʾ��ʱ�������� \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispAt(64, 32, "Normal Testing") == SDK_OK));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkDispAt(64, 32, NULL) == SDK_PARA_ERR));
    /*
       not visible character tesing
     */
    TEST_IS_TRUE_WITH_INFO("���ɼ��ַ�����", (sdkDispAt(64, 32, "\x80\xF5") == SDK_PARA_ERR));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(x  = 128)",
                           (sdkDispAt(pixel.siX, 32, "Testing") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(y = 64)",
                           (sdkDispAt(64, pixel.siY, "Testing") == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 0)", (sdkDispAt(0, 32, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 1)", (sdkDispAt(1, 32, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 60)", (sdkDispAt(60, 32, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = pixel.siX-1)",
                           (sdkDispAt(pixel.siX - 1, 32, "Testing") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 0)", (sdkDispAt(64, 0, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 1)", (sdkDispAt(64, 1, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 32)", (sdkDispAt(64, 32, "Testing") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = pixel.siY-1)",
                           (sdkDispAt(64, pixel.siY - 1, "Testing") == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispClearAt
   ��������:	����API ����sdkDispClearAt
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 15:44:45
*******************************************************************/
void sdkTest_sdkDispClearAt(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display",
            " \r\n \r\nStart (sdkDispClearAt/ ͨ��ָ�����������ָ���������ʾ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispClearAt(64, 32, 100, 50) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(X0  = pixel.siX)",
                           (sdkDispClearAt(pixel.siX, 32, 100, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X1 = pixel.siX)",
                           (sdkDispClearAt(64, 32, pixel.siX, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(Y0 = pixel.siY)",
                           (sdkDispClearAt(64, pixel.siY, 100, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(Y1 = pixel.siY)",
                           (sdkDispClearAt(64, 32, 100, pixel.siY) == SDK_PARA_ERR));
    /*
       parameter logic error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����߼��������(X0 > X1)",
                           (sdkDispClearAt(100, 32, 64, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(Y0 > Y1)",
                           (sdkDispClearAt(64, 50, 120, 32) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X0 = 0)", (sdkDispClearAt(0, 32, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X0 = 1)", (sdkDispClearAt(1, 32, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X0 = 60)", (sdkDispClearAt(60, 32, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X0 = pixel.siX-1)",
                           (sdkDispClearAt(pixel.siX - 1, 32, pixel.siX - 1, 50) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 0)", (sdkDispClearAt(0, 32, 0, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 1)", (sdkDispClearAt(0, 32, 1, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 60)", (sdkDispClearAt(0, 32, 60, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = pixel.siX-1)",
                           (sdkDispClearAt(0, 32, pixel.siX - 1, 50) == SDK_OK));


    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 0)", (sdkDispClearAt(64, 0, 100, 63) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 1)", (sdkDispClearAt(64, 1, 100, 63) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = 32)", (sdkDispClearAt(64, 32, 100, 63) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y0  = pixel.siY-1)",
                           (sdkDispClearAt(64, pixel.siY - 1, 100, pixel.siY - 1) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y1  = 0)", (sdkDispClearAt(64, 0, 100, 0) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y1  = 1)", (sdkDispClearAt(64, 0, 100, 1) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y1  = 32)", (sdkDispClearAt(64, 0, 100, 32) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y1  = 62)", (sdkDispClearAt(64, 0, 100, 62) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(Y1  = pixel.siY-1)",
                           (sdkDispClearAt(64, 0, 100, pixel.siY - 1) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispSetBackground
   ��������:����API����sdkDispSetBackground
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 17:20:11
*******************************************************************/
void sdkTest_sdkDispSetBackground(void) {

//shiweisong 2013.04.11 9:39
//�˺�����ʱ������
    if (sdkTestIsUcos())//huacong 2013.04.10 13:42
    {
#if 0 /*Modify by huacong at 2013.04.11  10:37 */
        if(sdkAccessFile("LOGO0.BMP") != true)
        {
            sdkDispMsgBox("��ʾ��Ϣ", "LOGO0.BMP  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }
#endif /* if 0 */
        // IN_sdkTest_sdkDispSetBackground("LOGO0.BMP");
    }
    else {
#if MACHINEMODEL == SDK_SYS_MACHINE_G2
        if (sdkAccessFile("/mtd1/logo2.bmp") != true) {
            sdkDispMsgBox("��ʾ��Ϣ", "/mtd1/logo2.bmp  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }
#else
        if(sdkAccessFile("/mtd0/res/background.bmp") != true)
        {
            sdkDispMsgBox("��ʾ��Ϣ", "/mtd0/res/background.bmp  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }
#endif
        //  IN_sdkTest_sdkDispSetBackground("/mtd0/res/background.bmp");
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:sdkTest_sdkDispClearRowRamAt
   ��������:����API����sdkDispClearRowRamAt
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 17:35:30
*******************************************************************/
void sdkTest_sdkDispClearRowRamAt(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display",
            " \r\n \r\nStart (sdkDispClearRowRamAt/ ���ָ���е���ʼ�е������еĻ������� \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispClearRowRamAt(SDK_DISP_LINE3, 64, 80) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row  = 5)",
                           (sdkDispClearRowRamAt(5, 64, 80) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(startCol = pixel.siX)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, pixel.siX, 80) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(endCol = pixel.siX)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 64, pixel.siX) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(startCol > endCol)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 100, 90) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(startCol = 0)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 0, 127) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(startCol = 1)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 1, 127) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(startCol = 60)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 60, 127) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(startCol = pixel.siX-1)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, pixel.siX - 1, pixel.siX - 1) ==
                            SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(endCol = 0)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 0, 0) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(endCol = 1)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 0, 1) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(endCol = 60)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 0, 60) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(endCol = pixel.siX-1)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 0, pixel.siX - 1) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 0)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE1, 64, 80) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 1)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE2, 64, 80) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 2)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE3, 64, 80) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 3)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE4, 64, 80) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 4)",
                           (sdkDispClearRowRamAt(SDK_DISP_LINE5, 64, 80) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:sdkTest_sdkDispClearRow
   ��������:����API����sdkDispClearRow
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 17:49:44
*******************************************************************/
void sdkTest_sdkDispClearRow(void) {

    TestLog("Test display", " \r\n \r\nStart (sdkDispClearRow/ ���ָ������ʾ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispClearRow(SDK_DISP_LINE3) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row  = 5)", (sdkDispClearRow(5) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 0)", (sdkDispClearRow(SDK_DISP_LINE1) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 1)", (sdkDispClearRow(SDK_DISP_LINE2) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 2)", (sdkDispClearRow(SDK_DISP_LINE3) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 3)", (sdkDispClearRow(SDK_DISP_LINE4) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 4)", (sdkDispClearRow(SDK_DISP_LINE5) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispRowAt
   ��������: ����API ����sdkDispRowAt
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 17:57:21
*******************************************************************/
void sdkTest_sdkDispRowAt(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display",
            " \r\n \r\nStart (sdkDispRowAt/��LCD��ĳһ��,ĳһ��(col����Ϊ��λ)��ʾ����, ����������ԭ������  \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispRowAt(SDK_DISP_LINE3, 0, "Normal Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkDispRowAt(SDK_DISP_LINE3, 0, NULL, SDK_DISP_DEFAULT) ==
                                        SDK_PARA_ERR));
    /*
       not visible character tesing
     */
    TEST_IS_TRUE_WITH_INFO("���ɼ��ַ�����",
                           (sdkDispRowAt(SDK_DISP_LINE3, 0, "\x80\xFF", SDK_DISP_DEFAULT) ==
                            SDK_PARA_ERR));
    /*
       parameter error testing (example model K320)
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row = 5)",
                           (sdkDispRowAt(5, 0, "Testing", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(col  = pixel.siX)",
                           (sdkDispRowAt(SDK_DISP_LINE3, pixel.siX, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 0)",
                           (sdkDispRowAt(SDK_DISP_LINE3, 0, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 1)",
                           (sdkDispRowAt(SDK_DISP_LINE3, 1, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 60)",
                           (sdkDispRowAt(SDK_DISP_LINE3, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = pixel.siX-1)",
                           (sdkDispRowAt(SDK_DISP_LINE3, pixel.siX - 1, "Testing",
                                         SDK_DISP_DEFAULT) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 0)",
                           (sdkDispRowAt(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 1)",
                           (sdkDispRowAt(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 2)",
                           (sdkDispRowAt(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 3)",
                           (sdkDispRowAt(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 4)",
                           (sdkDispRowAt(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispRow
   ��������:	����API����sdkDispRow
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 17:59:32
*******************************************************************/
void sdkTest_sdkDispRow(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display",
            " \r\n \r\nStart (sdkDispRow/�Ȳ�������ԭ�����ݣ���LCD��ĳһ��,ĳһ��(col����Ϊ��λ)��ʾ����  \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispRow(SDK_DISP_LINE3, 0, "Normal Testing", SDK_DISP_DEFAULT) ==
                            SDK_OK));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkDispRow(SDK_DISP_LINE3, 0, NULL, SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    /*
       not visible character tesing
     */
    TEST_IS_TRUE_WITH_INFO("���ɼ��ַ�����",
                           (sdkDispRow(SDK_DISP_LINE3, 0, "\x80\xFF", SDK_DISP_DEFAULT) ==
                            SDK_PARA_ERR));
    /*
       parameter error testing (example model K320)
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row = 5)",
                           (sdkDispRow(5, 0, "Testing", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(col  = pixel.SIX)",
                           (sdkDispRow(SDK_DISP_LINE3, pixel.siX, "Testing", SDK_DISP_DEFAULT) ==
                            SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 0)",
                           (sdkDispRow(SDK_DISP_LINE3, 0, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 1)",
                           (sdkDispRow(SDK_DISP_LINE3, 1, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = 60)",
                           (sdkDispRow(SDK_DISP_LINE3, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(col = pixel.siX-1)",
                           (sdkDispRow(SDK_DISP_LINE3, pixel.siX - 1, "Testing",
                                       SDK_DISP_DEFAULT) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 0)",
                           (sdkDispRow(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 1)",
                           (sdkDispRow(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 2)",
                           (sdkDispRow(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 3)",
                           (sdkDispRow(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row = 4)",
                           (sdkDispRow(SDK_DISP_LINE1, 64, "Testing", SDK_DISP_DEFAULT) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispRestoreScreen
   ��������:	����API����sdkDispRestoreScreen
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 18:53:26
*******************************************************************/
void sdkTest_sdkDispRestoreScreen(void) {
    u16 *buf;

    buf = (u16 *) sdkGetMem(sizeof(u16) * (sdkDispGetScreenBufferSize()));
    TestLog("Test display", " \r\n \r\nStart (sdkDispRestoreScreen/ �ָ���Ļ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispRestoreScreen(buf, sdkDispGetScreenBufferSize()) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(len   = sdkDispGetScreenBufferSize()/2)",
                           (sdkDispRestoreScreen(buf, sdkDispGetScreenBufferSize() / 2) ==
                            SDK_DISP_SIZEERRO));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkDispRestoreScreen(NULL, sdkDispGetScreenBufferSize()) ==
                                        SDK_PARA_ERR));
    sdkFreeMem(buf); //shiweisong 2013.04.02 18:21
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispSaveScreen
   ��������:	����API����sdkDispSaveScreen
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 18:55:37
*******************************************************************/
void sdkTest_sdkDispSaveScreen(void) {
    u16 *buf;

    buf = (u16 *) sdkGetMem(sizeof(u16) * (sdkDispGetScreenBufferSize()));
    TestLog("Test display", " \r\n \r\nStart (sdkDispRestoreScreen/ �ָ���Ļ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispSaveScreen(buf, sdkDispGetScreenBufferSize()) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(len   = sdkDispGetScreenBufferSize()/2)",
                           (sdkDispSaveScreen(buf, sdkDispGetScreenBufferSize() / 2) ==
                            SDK_DISP_SIZEERRO));
    /*
       NULL pointer testing
     */
    TEST_IS_TRUE_WITH_INFO("��ָ�����",
                           (sdkDispSaveScreen(NULL, sdkDispGetScreenBufferSize()) == SDK_PARA_ERR));
    sdkFreeMem(buf); // shiweisong 2013.04.02 18:21
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispDrawProgbarRam
   ��������:	����API sdkDispDrawProgbarRam
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 11:32:09
*******************************************************************/
void sdkTest_sdkDispDrawProgbarRam(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispDrawProgbarRam/����ʾ�����л�һ������ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 64, 80, 50) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row  = 5)",
                           (sdkDispDrawProgbarRam(5, 64, 80, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X1 = pixel.SIX)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, pixel.siX, 80, 50) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X2 = pixel.siX)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 64, pixel.siX, 50) ==
                            SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X1 > X2)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 100, 90, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(ucValue > 100)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 100, 90, 101) == SDK_PARA_ERR));

    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 0)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 0, pixel.siY - 1, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 1)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 1, pixel.siY - 1, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 60)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 60, pixel.siY - 1, 50) ==
                            SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = pixel.SIX-1)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, pixel.siX - 1, 64, 50) ==
                            SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 0)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 0, 0, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 1)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 0, 1, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 60)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 0, 60, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = pixel.siX-1)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 0, pixel.siX - 1, 50) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 0)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE1, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 1)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE2, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 2)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE3, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 3)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE4, 64, 80, 50) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 4)",
                           (sdkDispDrawProgbarRam(SDK_DISP_LINE5, 64, 80, 50) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:sdkTest_sdkDispDrawProgbar
   ��������:	����API sdkDispDrawProgbar
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 11:51:39
*******************************************************************/
void sdkTest_sdkDispDrawProgbar(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispDrawProgbar/����ʾ�����л�һ������ \r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispDrawProgbar(SDK_DISP_LINE3, 64, 80, 50) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(row  = 5)",
                           (sdkDispDrawProgbar(5, 64, 80, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X1 = pixel.siX)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, pixel.siX, 80, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X2 = pixel.siX)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 64, pixel.siX, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(X1 > X2)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 100, 90, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(ucValue > 100)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 100, 90, 101) == SDK_PARA_ERR));

    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 0)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 0, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 1)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 1, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = 60)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 60, 127, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X1 = pixel.siX)-1",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, pixel.siX - 1, 127, 50) ==
                            SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 0)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 0, 0, 50) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 1)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 0, 1, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = 60)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 0, 60, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(X2 = pixel.siX-1)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 0, pixel.siX - 1, 50) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 0)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE1, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 1)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE2, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 2)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE3, 64, 80, 50) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 3)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE4, 64, 80, 50) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(row  = 4)",
                           (sdkDispDrawProgbar(SDK_DISP_LINE5, 64, 80, 50) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkTest_sdkDispSetBackground
   ��������:	����API sdkDispShowBmp
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 09:41:56
*******************************************************************/
void sdkTest_sdkDispShowBmp(void) {
    if (sdkTestIsUcos())//huacong 2013.04.10 13:42
    {

#if 0 /*Modify by huacong at 2013.04.11  10:38 */
        if(sdkAccessFile("logo0.bmp") != true)
        {
            sdkDispMsgBox("��ʾ��Ϣ", "LOGO0.BMP ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        } 
        IN_sdkTest_sdkDispShowBmp("logo0.bmp");
#endif /* if 0 */


    }
    else {
#if MACHINEMODEL == SDK_SYS_MACHINE_G2
        if (sdkAccessFile("/mtd1/logo1.bmp") != true) {
            sdkDispMsgBox("��ʾ��Ϣ", "/mtd1/logo1.bmp  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }
        IN_sdkTest_sdkDispShowBmp("/mtd1/logo1.bmp");
#elif MACHINEMODEL == SDK_SYS_MACHINE_200P
        if(sdkAccessFile("/mtd1/logo0.bmp") != true)
        {
            sdkDispMsgBox("��ʾ��Ϣ", "/mtd1/logo0.bmp  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }    
        IN_sdkTest_sdkDispShowBmp("/mtd1/logo1.bmp");
#else        
        if(sdkAccessFile("/mtd0/res/background.bmp") != true)
        {
            sdkDispMsgBox("��ʾ��Ϣ", "/mtd0/res/background.bmp  ������ ���Խ�����ܻ����", 0, SDK_KEY_MASK_ALL);
        }
        IN_sdkTest_sdkDispShowBmp("/mtd0/res/background.bmp");
#endif
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:sdkDispDrawLine
   ��������:����API sdkDispDrawLine
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 10:53:32
*******************************************************************/
void sdkTest_sdkDispDrawLine(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispDrawLine/����ʾ�����л�һֱ�� \r\n \r\n");

//    /*
//       Normal testing(Take K320 as an example 128*64)
//     */
//    TEST_IS_TRUE_WITH_INFO("��������" , (sdkDispDrawLine(64 , 32 , 100 , 50) == SDK_OK));
//    /*
//       parameter error testing
//     */
//    TEST_IS_TRUE_WITH_INFO ("�����������(X0  = 128)" , (sdkDispDrawLine(128 , 32 , 100 , 50) == SDK_PARA_ERR));
//    TEST_IS_TRUE_WITH_INFO ("�����������(X1 = 128)" , (sdkDispDrawLine(64 , 32 , 128 , 50) == SDK_PARA_ERR));
//    TEST_IS_TRUE_WITH_INFO ("�����������(Y0 = 64)" , (sdkDispDrawLine(64 , 64 , 100 , 50)  == SDK_PARA_ERR));
//    TEST_IS_TRUE_WITH_INFO ("�����������(Y1 = 64)" , (sdkDispDrawLine(64 , 32 , 100 , 64) == SDK_PARA_ERR));
//    /*
//       parameter logic error testing
//     */
//    TEST_IS_TRUE_WITH_INFO ("�����߼��������(X0 > X1)" , (sdkDispDrawLine(100 , 32 , 64 , 50) == SDK_PARA_ERR));
//    TEST_IS_TRUE_WITH_INFO ("�����������(Y0 > Y1)" , (sdkDispDrawLine(64 , 50 , 128 , 32) == SDK_PARA_ERR));
//
//    /*
//       Boundary value testing
//     */
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X0 = 0)" , (sdkDispDrawLine(0 , 32 , 127 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X0 = 1)" , (sdkDispDrawLine(1 , 32 , 127 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X0 = 60)" , (sdkDispDrawLine(60 , 32 , 127 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X0 = 126)" , (sdkDispDrawLine(126 , 32 , 127 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X0 = 127)" , (sdkDispDrawLine(127 , 32 , 127 , 50) == SDK_OK));
//
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X1 = 0)" , (sdkDispDrawLine(0 , 32 , 0 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X1 = 1)" , (sdkDispDrawLine(0 , 32 , 1 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X1 = 60)" , (sdkDispDrawLine(0 , 32 , 60 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X1 = 126)" , (sdkDispDrawLine(0 , 32 , 126 , 50) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(X1 = 127)" , (sdkDispDrawLine(0 , 32 , 127 , 50) == SDK_OK));
//
//
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y0  = 0)" , (sdkDispDrawLine(64 , 0 , 100 , 63) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y0  = 1)" , (sdkDispDrawLine(64 , 1 , 100 , 63) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y0  = 32)" , (sdkDispDrawLine(64 , 32 , 100 , 63) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y0  = 62)" , (sdkDispDrawLine(64 , 62 , 100 , 63) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y0  = 63)" , (sdkDispDrawLine(64 , 63 , 100 , 63) == SDK_OK));
//
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y1  = 0)" , (sdkDispDrawLine(64 , 0 , 100 , 0) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y1  = 1)" , (sdkDispDrawLine(64 , 0 , 100 , 1) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y1  = 32)" , (sdkDispDrawLine(64 , 0 , 100 , 32) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y1  = 62)" , (sdkDispDrawLine(64 , 0 , 100 , 62) == SDK_OK));
//    TEST_IS_TRUE_WITH_INFO ("�߽�ֵ����(Y1  = 63)" , (sdkDispDrawLine(64 , 0 , 100 , 63) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkDispPutPixel
   ��������:	����API sdkDispPutPixel
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 11:11:37
*******************************************************************/
void sdkTest_sdkDispDrawPixel(void) {
    SDK_DISP_PIXEL pixel;

    pixel = sdkDispGetScreenPixel();
    TestLog("Test display", " \r\n \r\nStart (sdkDispPutPixel/ ����ʾ�����л�һ����\r\n \r\n");

    /*
       Normal testing(Take K320 as an example 128*64)
     */
    TEST_IS_TRUE_WITH_INFO("��������", (sdkDispPutPixel(64, 32, SDK_DISP_BLACK_COLOR) == SDK_OK));
    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(x  = pixel.six)",
                           (sdkDispPutPixel(pixel.siX, 32, SDK_DISP_BLACK_COLOR) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(y = pixel.siY)",
                           (sdkDispPutPixel(64, pixel.siY, SDK_DISP_WHITE_COLOR) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 0)",
                           (sdkDispPutPixel(0, 32, SDK_DISP_BLACK_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 1)",
                           (sdkDispPutPixel(1, 32, SDK_DISP_WHITE_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = 60)",
                           (sdkDispPutPixel(60, 32, SDK_DISP_BLACK_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(x = pixel.siX-1)",
                           (sdkDispPutPixel(pixel.siX - 1, 32, SDK_DISP_BLACK_COLOR) == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 0)",
                           (sdkDispPutPixel(64, 0, SDK_DISP_WHITE_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 1)",
                           (sdkDispPutPixel(64, 1, SDK_DISP_BLACK_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = 32)",
                           (sdkDispPutPixel(64, 32, SDK_DISP_WHITE_COLOR) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�߽�ֵ����(y  = pixel.siY-1)",
                           (sdkDispPutPixel(64, pixel.siY - 1, SDK_DISP_WHITE_COLOR) == SDK_OK));
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkDispPutPixel
   ��������:	����API sdkDispPutPixel
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 11:11:37
*******************************************************************/
void sdkTest_sdkDispPromptBox(void) {
    TestLog("Test display", " \r\n \r\nStart (sdkDispPromptBox/ ��ʾ��Ϣ\r\n \r\n");

    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(bIsSaveScreen  = 5)",
                           (sdkDispPromptBox(5, 100, "���ߵ�") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(overtime = -1)",
                           (sdkDispPromptBox(1, -1, "���ߵ�") == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(asDispData = NULL)",
                           (sdkDispPromptBox(1, 0, NULL) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	sdkDispPutPixel
   ��������:	����API sdkDispPutPixel
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.11.02 11:11:37
*******************************************************************/
void sdkTest_sdkDispLabel(void) {
    TestLog("Test display", " \r\n \r\nStart (sdkDispLabel/ ��ʾ��Ϣ\r\n \r\n");

    /*
       parameter error testing
     */
    TEST_IS_TRUE_WITH_INFO("�����������(startline  = 5)",
                           (sdkDispLabel(5, "���ߵ�", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(asDispData = NULL)",
                           (sdkDispLabel(SDK_DISP_LINE3, NULL, SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�����������(startline = -1)",
                           (sdkDispLabel(-1, "���ߵ�", SDK_DISP_DEFAULT) == SDK_PARA_ERR));
    /*
       Boundary value testing
     */
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ��ΰΰ
   ��������:void TestIllPar_sdkUIGetVersion
   ��������:	�Ƿ���������
   ����ʱ��:2013.02.22 09:30:00
*******************************************************************/
#if 0
void TestIllPar_sdkUIGetVersion(void)
{
    TestLog("Test_sdkUIGetVersion", " \r\n \r\n��ʼsdkUIGetVersion�Ƿ���������  \r\n \r\n");
    TEST_IS_TRUE_WITH_INFO("��ָ�����", (sdkUIGetVersion(NULL) == SDK_PARA_ERR));

    TestLog("Test_sdkUIGetVersion", " \r\n \r\n�Ƿ������������ \r\n \r\n");
}

#endif


/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2014.02.18
** Remarks:
*****************************************************************************/
static void Test_sdkMultDisp2(void) {
#if 0
    //�����ʾ����
    SDK_DISP_MULT_CTRL textInfo[6] = {0};
    u8 i;
    u8 *str1[] = {"ԭ��ĺ�����fdsafdsafdsf","����ԭ���ytjykuk","����ASCII��fdsf��",
                    "fgdsshgfj�����ֺ�","Ԥ���ĸ��ֽڹ����ֺ�","fdsfdshgjkjhlfgfhf"};

    u8 randNmb[4] = {0};                
    sdkDispMsgBox("�����ʾ����", "�������ʼ����\r\n�ٴ�������˳�����\r\n", 0, SDK_KEY_MASK_ALL); 
    sdkDispClearScreen();
    for( i = 0; i < 6; i++ )
    {
        textInfo[i].m_x0 = 10;
        textInfo[i].m_y0 = 40 * i;
        textInfo[i].m_font = 0;
        sdkGetRandom( randNmb, 4);
        textInfo[i].m_ascsize = randNmb[0]%5;
        textInfo[i].m_nativesize = randNmb[1]%4;
        textInfo[i].m_attrbution = (randNmb[2]%2) | (0x01 << (randNmb[4]%3+2));
        textInfo[i].m_text = str1[i];
        Trace( "devlcdtest", "str1[%d] = %s\r\n", i, str1[i] );
    }
    sdkDispCombText( 6, textInfo );   
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
#else
    SDK_DISP_MULT_DATA stDispData[3];
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
    sdkDispMsgBox("�����ʾ����2", "�������ʼ����\r\n�ٴ�������˳�����\r\n", 0, SDK_KEY_MASK_ALL);
    sdkDispClearScreen();
    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
    stDispData[0].eChFont = SDK_PRN_CH12X12;
    stDispData[0].siX = 0;
    stDispData[0].siY = 0;
    strcpy(stDispData[0].pasText, "����:");

    //memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
    stDispData[1].eAscFont = SDK_PRN_ASCII5X7;
    stDispData[1].eChFont = SDK_PRN_CH12X12;
    stDispData[1].siX = 64;
    stDispData[1].siY = 0;
    strcpy(stDispData[1].pasText, "622500******0113");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);

    memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
    stDispData[0].eAscFont = SDK_PRN_ASCII6X12;
    stDispData[0].eChFont = SDK_PRN_CH12X12;
    stDispData[0].siX = 0;
    stDispData[0].siY = 32;
    strcpy(stDispData[0].pasText, "�����ʾ");

    //memset(stDispData, 0, sizeof(SDK_DISP_MULT_DATA) * 3);
    stDispData[1].eAscFont = SDK_PRN_ASCII5X7;
    stDispData[1].eChFont = SDK_PRN_CH16X16;
    stDispData[1].siX = 64;
    stDispData[1].siY = 32;
    strcpy(stDispData[1].pasText, "���������");

    sdkDispFormatFillRam(SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL, &stDispData[0],
                         &stDispData[1], NULL);

    sdkDispBrushScreen();
    sdkKbWaitKey(SDK_KEY_MASK_ALL, 0);
#endif

}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ���ߵ�
   ��������:	TestIOmain
   ��������:	�����������main����
   �������:��
   �������:��
   ��   ��  ֵ:��
   �޸ı�ע:
   ����ʱ��:2012.06.04 09:45:21
*******************************************************************/
void Test_sdkDispAuto(void) {
#if 1
    sdkTest_sdkDispFillRowRam();

    sdkTest_sdkDispAt();
    sdkTest_sdkDispClearAt();
    sdkTest_sdkDispSetBackground();
    sdkTest_sdkDispClearRowRamAt();

    sdkTest_sdkDispClearRow();
    sdkTest_sdkDispRowAt();
    sdkTest_sdkDispRow();
#endif

    sdkTestShowProcessing("�����50%...");
    sdkTest_sdkDispShowBmp();
    sdkTest_sdkDispDrawProgbarRam();
    sdkTest_sdkDispDrawProgbar();
    sdkTest_sdkDispDrawLine();
    sdkTest_sdkDispDrawPixel();
    sdkTest_sdkDispLabel();

    if (SDK_SYS_MACHINE_K301FZ != sdkSysGetMachineCode(NULL) &&
        SDK_SYS_MACHINE_K301FZE != sdkSysGetMachineCode(NULL) &&
        SDK_SYS_MACHINE_K350 != sdkSysGetMachineCode(NULL) &&
        SDK_SYS_MACHINE_G2 != sdkSysGetMachineCode(NULL) &&
        SDK_SYS_MACHINE_200P != sdkSysGetMachineCode(NULL))//200P ��ʱ������
    {
        sdkTest_sdkDispPromptBox();
        sdkTest_sdkDispRestoreScreen();
        sdkTest_sdkDispSaveScreen();
    }

    Test_sdkMultDisp2();
}

