/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkICdealMaualTest.c
   �� �� ��   : ����
   ��    ��   : ʯ����
   ��������   : 2013��1��25��
   ����޸�   :
   ��������   : �ֶ�����icdeal��
   �����б�   :
   �޸���ʷ   :


******************************************************************************/

#include "sdkGlobal.h"
#include "SdkTest.h"

const static u8 *gCardName[] =
        {
                0,
                "4428",                                 //MEMORY CARD SLE4428           //sjl20120724
                "4442",                                 //MEMORY CARD SLE4442
                "AT24C01",                                 //MEMORY CARD AT24Cxx
                "AT24C02",                                 //MEMORY CARD AT24Cxx
                "AT24C04",                                 //MEMORY CARD AT24Cxx
                "AT24C08",                                 //MEMORY CARD AT24Cxx
                "AT24C16",                                 //MEMORY CARD AT24Cxx
                "AT24C32",                                 //MEMORY CARD AT24Cxx
                "AT24C64",                                 //MEMORY CARD AT24Cxx
                "IS23SC1604",                                    //MEMORY CARD AT88SC1608/AT88SC153
                "AT88SC153",                               //MEMORY CARD AT88SC153
                "AT88SC1608",                              //MEMORY CARD AT88SC1608/
                0,
        };


const static u8 gCardType[] =
        {
                0,
                SDK_ICC_SLE4428,                                 //MEMORY CARD SLE4428           //sjl20120724
                SDK_ICC_SLE4442,                                 //MEMORY CARD SLE4442
                SDK_ICC_AT24C01,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C02,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C04,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C08,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C16,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C32,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_AT24C64,                                 //MEMORY CARD AT24Cxx
                SDK_ICC_1604,                                    //MEMORY CARD AT88SC1608/AT88SC153
                SDK_ICC_AT88SC153,                               //MEMORY CARD AT88SC153
                SDK_ICC_AT88SC1608,                              //MEMORY CARD AT88SC1608/
                0,
        };
static u8 gCardTypeID = 0;

void Test_sdkSetMemCardIndex(u8 index) {
    if (index < 14) {
        gCardTypeID = index;
    }
    else {
        sdkDispMsgBox("Test_sdkSetMemCardIndex", "�����mem��������������", 0, SDK_KEY_MASK_ALL);
    }
}


/*=======BEGIN: ���ٲ� 2013.10.16  9:44 add mifare mem card test demon===========*/
/*****************************************************************************
** Descriptions:	
** Parameters:    	void
** Returned value:	
** Created By:		���ٲ�  2014.03.13
** Remarks: 		����mifare������
                    ��Ҫ����
                    ������
                    ���ݿ�
                    ��Կ����
                    ��Կ����
*****************************************************************************/
static s32 Test_sdkMifareSetPara(u8 *psblock, u8 *pdblock) {

    if (psblock == NULL || pdblock == NULL) {
        return SDK_PARA_ERR;
    }
    *psblock = sdkTestInputU8("����mfiare�����Բ���", "�������:");
    *pdblock = sdkTestInputU8("����mfiare�����Բ���", "���ݿ��:");

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:
*****************************************************************************/
static s32 Test_sdkMifareQueryCard(void) {
    s32 rslt = 0, timeout = 0;
    u8 *outbuf; //[1024] = {0};
    u8 cardtype = 0;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    timeout = 10000; //��ʱʱ��10s
    cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B;

//2����ʾ��ʾ
    sdkDispMsgBox("����Ѱ��", "������:AB��\r��ʱʱ��:10s\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    sdkIccOpenRfDev();
    TEST_IS_TRUE_WITH_INFO("����Ѱ��", (rslt = sdkIccRFQuery(cardtype, outbuf, timeout) > 0));

//4�������ʾ
    p = (rslt > 0) ? ("Ѱ���ɹ�") : ("Ѱ��ʧ��");

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:
*****************************************************************************/
static s32 Test_sdkMifareReadCard(void) {
    s32 rslt = 0;
    u8 *outbuf;//[1024] = {0};
    u8 temp[64] = {0};
    u8 *dispbuf;//[1024] = {0};
    u8 *p = NULL;

//1������׼��
    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//2����ʾ��ʾ
    sdkDispMsgBox("���Զ�����", "����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Զ�����", ((rslt = sdkIccMifareReadCardNum(outbuf)) > 0));

//4�������ʾ
    if (rslt > 0) {
        TraceHex("emv", "sdkIccMifareReadCardNum ok \r\n", outbuf, rslt);
        sdkBcdToAsc(temp, outbuf, rslt);
        sprintf(dispbuf, "����:%s\r����ȷ�ϡ����Ժ���\r����ȡ�����˳�", temp);
        p = "�����ųɹ�";
    }
    else {
        sprintf(dispbuf, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�");
        p = "������ʧ��";
    }

    rslt = sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:
*****************************************************************************/
static s32 Test_sdkMifareVerifyKey(u8 block) {
    s32 rslt = 0, outlen = 0, keylen = 0;
    u8 *outbuf;//[1024] = {0};
    u8 keydata[64] = {0}, keytype = 0;
    u8 *dispbuf;//[1024] = {0};
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    memcpy(keydata, "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
    keylen = 6;
    keytype = SDK_RF_KEYA;

//2����ʾ��ʾ
    rslt = sdkBcdToAsc(outbuf, keydata, 6);
    sprintf(dispbuf, "��Կ:%s\r���:%d\r��Կ����:%d\r����ȷ�ϡ�����", outbuf, block, keytype);

    sdkDispMsgBox("������ԿУ��", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    memset(outbuf, 0, sizeof(outbuf));
    TEST_IS_TRUE_WITH_INFO("������ԿУ��",
                           ((rslt = sdkIccMifareVerifyKey(block, keytype, keydata, keylen, outbuf,
                                                          &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("У��ɹ�") : ("У��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *inbuf
                               s32 inlen
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkMifareReadBlock(u8 block, u8 *inbuf, s32 inlen) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    u8 *p = NULL;

//1������׼��
    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);
//2����ʾ��ʾ
    sprintf(dispbuf, "���:%d\r����ȷ�ϡ�����", block);

    sdkDispMsgBox("���Կ��ȡ", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ��ȡ",
                           ((rslt = sdkIccMifareReadBlock(block, outbuf, &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));
    TraceHex("emv", "mifare card read out", outbuf, outlen);

//4���Ƚ���ʾ���
    if (rslt == SDK_OK && outbuf[0] == 0x00) {
        if ((inbuf == NULL) || (inlen == 0)) //�״ζ��������ж�
        {
            p = "��ȡ�ɹ�";
        }
        else if ((!memcmp(inbuf, &outbuf[1], inlen))) {
            p = "��ȡ��У��ɹ�";
        }
        else {
            p = "��ȡʧ��";
        }
    }
    else {
        p = "��ȡʧ��";
    }
    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:	        ����д�����ݴ����Ƚ�
** Parameters:          u8 *wbuf_out
                               s32 *wlen_out
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:             ���ǲ���д�������
*****************************************************************************/
static s32 Test_sdkMifareWriteBlock(u8 block, u8 *wbuf_out, s32 *wlen_out) {
    s32 rslt = 0, inlen = 0, outlen = 0;
    u8 inbuf[64] = {0};
    u8 *outbuf, *dispbuf;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    sdkGetRandom(inbuf, 4); //һK��ֻ��д4���ֽڣ�4k����16�ֽ�
    inlen = 4;

    memcpy(wbuf_out, inbuf, inlen);

//2����ʾ��ʾ
    rslt = sdkBcdToAsc(outbuf, inbuf, inlen);
    sprintf(dispbuf, "���:%d\rд������:%s\r����ȷ�ϡ�����", block, outbuf);

    sdkDispMsgBox("���Կ�д��", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    memset(outbuf, 0, sizeof(outbuf));
    TEST_IS_TRUE_WITH_INFO("���Կ�д��",
                           ((rslt = sdkIccMifareWriteBlock(block, inbuf, inlen, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    *wlen_out = (rslt == SDK_OK && outbuf[0] == 0x00) ? (inlen) : (0); //�����Ǵ���д��������

    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("д��ɹ�") : ("д��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:             opttype                 ����
                                    0                  ��ʽ����
                                    1                  ��ֵ����
                                    2                  ��ֵ����
*****************************************************************************/
static s32 Test_sdkMifareOperateBlock(u8 block, u8 opttype) {
    s32 rslt = 0, outlen = 0;
    u8 inbuf[64] = {0};
    u8 *outbuf, *dispbuf;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    memcpy(inbuf, "\x00\x00\x00\x01", 4);

//2����ʾ��ʾ
    p = (opttype == 0) ? ("���Կ��ʽ��") : ((opttype == 1) ? ("���Կ���ֵ") : ((opttype == 2)
                                                                           ? ("���Կ��ֵ")
                                                                           : ("�����������")));

    rslt = sdkBcdToAsc(outbuf, inbuf, 4);
    sprintf(dispbuf, "���:%d\rд������:%s\r����ȷ�ϡ�����", block, outbuf);

    sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ�д��",
                           ((rslt = sdkIccMifareOperateBlock(opttype, block, inbuf, outbuf,
                                                             &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("�����ɹ�") : ("����ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:             Դ����뾭����ʽ���������ܱ���
*****************************************************************************/
static s32 Test_sdkMifareBackUpBlock(u8 sblock, u8 dblock) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    //sblock -Դ�飬dblock-���ݿ�
    u8 *p = NULL;

//1������׼��
    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);
//2����ʾ��ʾ
    sprintf(dispbuf, "Դ���:%d\r���ݿ��:%d\r����ȷ�ϡ�����", sblock, dblock);

    sdkDispMsgBox("���Կ鱸��", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ鱸��",
                           ((rslt = sdkIccMifareBackUpBlock(sblock, dblock, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("�鱸�ݳɹ�") : ("�鱸��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:
*****************************************************************************/
static s32 Test_sdkMifarePowerDown(void) {
    s32 rslt = 0;
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;

//1������׼��
    psticcparam.eCardType = SDK_ICC_MIFARE;
    psticcparam.ucCardMode = SDK_ICC_RF;
    psticcparam.ucSlotNo = 0;

//2����ʾ��ʾ
    sdkDispMsgBox("�����µ�", "����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);


//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("�����µ�", (rslt = sdkIccMemMifarePowerDown(&psticcparam) > 0));

//4�������ʾ
    p = (rslt > 0) ? ("�µ�ɹ�") : ("�µ�ʧ��");

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:	        mifare������ʾ��
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.16
** Remarks:            
*****************************************************************************/
s32 Test_sdkIccMifareTestDemon(char *pasOutInfo, char const *pTitle) {
    u8 *temp;
    u8 sblock = 0, dblock = 0;
    s32 templen = 0;

    temp = (u8 *) sdkGetMem(1024);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 1024);

//0�����ò��Բ���
    Test_sdkMifareSetPara(&sblock, &dblock);

    do {
//1��Ѱ������
        if (Test_sdkMifareQueryCard() == SDK_KEY_ESC) { break; }

//2�������Ų���
        if (Test_sdkMifareReadCard() == SDK_KEY_ESC) { break; }

//3��У����Կ
        if (Test_sdkMifareVerifyKey(sblock) == SDK_KEY_ESC) { break; }

//4���������
        if (Test_sdkMifareReadBlock(sblock, NULL, 0) == SDK_KEY_ESC) { break; }

//5��д�����
        if (Test_sdkMifareWriteBlock(sblock, temp, &templen) == SDK_KEY_ESC) { break; }

//6���ٴζ�������
        if (Test_sdkMifareReadBlock(sblock, temp, 4) == SDK_KEY_ESC) { break; }

//7�����ʽ������
        if (Test_sdkMifareOperateBlock(sblock, 0) == SDK_KEY_ESC) { break; }

//8������ֵ����
        if (Test_sdkMifareOperateBlock(sblock, 1) == SDK_KEY_ESC) { break; }

//9�����ֵ����
        if (Test_sdkMifareOperateBlock(sblock, 2) == SDK_KEY_ESC) { break; }

//10���鱸��
        if (Test_sdkMifareBackUpBlock(sblock, dblock) == SDK_KEY_ESC) { break; }
    }
    while (0);

    sdkFreeMem(temp);
    temp = NULL;

//11���µ����
    Test_sdkMifarePowerDown();

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442PowerOn(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    psticcparam.eCardType = gCardType[gCardTypeID];
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    sdkDispMsgBox("�����ϵ�", "������:4442\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("�����ϵ�",
                           (rslt = sdkIccMemPowerOn(&psticcparam, outbuf, &outlen) == SDK_OK));

//4�������ʾ
    p = (rslt == SDK_OK) ? ("�ϵ�ɹ�") : ("�ϵ�ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442QueryCard(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;//[1024] = {0};
    u8 *p = NULL;

//1������׼��
    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);
//2����ʾ��ʾ
    sdkDispMsgBox("����Ѱ��", "������:4442\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("����Ѱ��", ((rslt = sdkIccMemCheck(outbuf, &outlen) == SDK_OK) &&
                                       (outbuf[0] == 0x01)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x01) ? ("Ѱ���ɹ�") : ("Ѱ��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442Reset(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;


    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    psticcparam.eCardType = gCardType[gCardTypeID];
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    sdkDispMsgBox("���Ը�λ", "������:4442\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Ը�λ",
                           ((rslt = sdkIccReset(&psticcparam, outbuf, &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("��λ�ɹ�") : ("��λʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442VerifyKey(u8 *pkey, s32 len) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    psticcparam.eCardType = gCardType[gCardTypeID];
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//    memcpy(inbuf, "\x12\x34\x56", 3); //6��F��Կ

//2����ʾ��ʾ
//    rslt = sdkBcdToAsc(outbuf, pkey, len);
    sprintf(dispbuf, "������:4442\r��Կ:%s\r����ȷ�ϡ�����", pkey);

    sdkDispMsgBox("������ԿУ��", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    memset(outbuf, 0, sizeof(outbuf));
    TEST_IS_TRUE_WITH_INFO("������ԿУ��",
                           ((rslt = sdkIccMemCheckPwd(&psticcparam, pkey, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("У��ɹ�") : ("У��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *inbuf
                               s32 inlen
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442ReadBlock(u8 *inbuf, s32 inlen) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM stparam;
    SDK_ICC_RMEMIN stdatain;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

    stdatain.ucAddress = 40; //32ǰ��Ϊ��Կ���������32�Ժ��ȡ
    stdatain.ucReadLen = 8;

//2����ʾ��ʾ
    sprintf(dispbuf, "������:4442\r��ʼ��ַ:%d\r�����ݳ���:%d\r����ȷ�ϡ�����", stdatain.ucAddress,
            stdatain.ucReadLen);

    sdkDispMsgBox("���Կ��ȡ", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ��ȡ",
                           ((rslt = sdkIccMemReadData(&stparam, &stdatain, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));
    TraceHex("emv", "mem4442 read out", outbuf, outlen);

//4�������ʾ
    if (rslt == SDK_OK && outbuf[0] == 0x00) {
        if ((inbuf == NULL) || (inlen == 0)) //�״ζ��������ж�
        {
            p = "��ȡ�ɹ�";
        }
        else if ((inlen == (outlen - 1)) && (!memcmp(inbuf, &outbuf[1], inlen))) {
            p = "��ȡ��У��ɹ�";
        }
    }
    else {
        p = "��ȡʧ��";
    }
    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *wbuf_out
                               s32 *wlen_out
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442WriteBlock(u8 *wbuf_out, s32 *wlen_out) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM stparam;
    SDK_ICC_WMEMIN stdatain;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

    stdatain.ucAddress = 40; //32ǰ��Ϊ��Կ���������32�Ժ����
    sdkGetRandom(stdatain.ucWriteData, 8);
    stdatain.ucWriteLen = 8;

    memcpy(wbuf_out, stdatain.ucWriteData, stdatain.ucWriteLen);

//2����ʾ��ʾ
    rslt = sdkBcdToAsc(outbuf, stdatain.ucWriteData, stdatain.ucWriteLen);
    sprintf(dispbuf, "������:4442\rд���ַ:%d\rд������:%s\r����ȷ�ϡ�����", stdatain.ucAddress, outbuf);

    sdkDispMsgBox("���Կ�д��", dispbuf, 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ�д��",
                           ((rslt = sdkIccMemWriteData(&stparam, &stdatain, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    *wlen_out = (rslt == SDK_OK && outbuf[0] == 0x00) ? (stdatain.ucWriteLen) : (0);

    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("д��ɹ�") : ("д��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:	        ������Կ
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442UpdateKey(u8 *pkey, s32 keylen) {
    s32 rslt = 0, outlen = 0;
    u8 newkey[64] = {0}, temp[64] = {0};
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM stparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//    memcpy(oldkey, "\xFF\xFF\xFF\xFF\xFF\xFF", 6);
    memcpy(newkey, "\x46\x45\x46\x45\x46\x45", 6);



//2����ʾ��ʾ
//    rslt = sdkBcdToAsc(outbuf, oldkey, 6);
//    rslt = sdkBcdToAsc(temp, newkey, 6);
    sprintf(dispbuf, "������:4442\rԭʼ��Կ:%s\r������Կ:%s\r����ȷ�ϡ�����\r����ȡ�����˳�", pkey, newkey);

    /*=======BEGIN: ���ٲ� 2013.11.18  8:34 modify===========*/
    //ȥ�����������goto���
    do {
        if (sdkDispMsgBox("������Կ����", dispbuf, 0, SDK_KEY_MASK_ALL) == SDK_KEY_ESC) { break; }
//3�����Բ���
        TEST_IS_TRUE_WITH_INFO("������Կ����",
                               ((rslt = sdkIccMemModifyPwd(&stparam, pkey, newkey, outbuf,
                                                           &outlen) == SDK_OK) &&
                                (outbuf[0] == 0x00)));

//4������ж�
        if (rslt != SDK_OK || outbuf[0] != 0x00) { break; }
//5�����ԸĻ�
        TEST_IS_TRUE_WITH_INFO("������Կ�Ļ�",
                               ((rslt = sdkIccMemModifyPwd(&stparam, newkey, pkey, outbuf,
                                                           &outlen) == SDK_OK) &&
                                (outbuf[0] == 0x00)));
    }
    while (0);

    /*====================== END======================== */

//6�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("���²��Գɹ�") : ("���²���ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}


/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 data
** Returned value:	
** Created By:		���ٲ�  2014.03.24
** Remarks: 		
*****************************************************************************/
static s32 Test_sdkU8DataBit1NumGet(u8 data) {
    s32 ii = 0, cnt = 0;

    for (ii = 0; ii < 8; ii++) {
        Trace("hsb", "data = %x, data&0x01 = %x", data, data & 0x01);
        cnt = cnt + (data & 0x01);
        data = data >> 1;
    }
    return cnt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442ReadPwdAndPwdCount(void) {
    s32 rslt = 0, outlen = 0, cnt = 0;
    u8 inbuf[64] = {0};
    u8 *outbuf, *dispbuf;
    u8 *p = NULL;

//1������׼��
    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//2����ʾ��ʾ
    sdkDispMsgBox("���Զ�������������", "������:4442\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Զ�������������",
                           ((rslt = sdkIccMem4442ReadPwdAndPwdCount(outbuf, &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));

//4�������ʾ
    if (rslt == SDK_OK && outbuf[0] == 0x00) {
        TraceHex("emv", "mem4442 read Pwd And PwdCount out", outbuf, outlen);

        cnt = Test_sdkU8DataBit1NumGet(outbuf[1]);

        sprintf(dispbuf, "�������:%d,ʣ�����:%d\r����ȷ�ϡ����Ժ���\r����ȡ�����˳�", (3 - cnt), cnt);
        p = "��ȡ�ɹ�";
    }
    else {
        sprintf(dispbuf, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�");
        p = "��ȡʧ��";
    }
    rslt = sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4442PowerDown(void) {
    s32 rslt = 0;
    SDK_ICC_PARAM stparam;
    u8 *p = NULL;

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    sdkDispMsgBox("�����µ�", "������:4442\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("�����µ�", (rslt = sdkIccMemMifarePowerDown(&stparam) == SDK_OK));

//4�������ʾ
    p = (rslt == SDK_OK) ? ("�µ�ɹ�") : ("�µ�ʧ��");

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);
}


/*****************************************************************************
** Descriptions:	����mem4442����Կ
** Parameters:    	u8 *pkey
** Returned value:	
** Created By:		���ٲ�  2014.03.31
** Remarks: 		������Կ����
*****************************************************************************/
static s32 Test_sdkMem4442SetPara(u8 *pkey) {
    s32 rslt = 0;

    if (pkey == NULL) {
        return SDK_PARA_ERR;
    }

    rslt = sdkTestInputASC(pkey, "���ò��Կ�mem4442��Կ", "����������:", 1, 12);

    TraceHex("hsb", "key value", pkey, rslt);

    return rslt;
}


/*****************************************************************************
** Descriptions:	        4442������ʾ��
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:             �������޸���Կ����
*****************************************************************************/
s32 Test_sdkIccMem4442TestDemon(char *pasOutInfo, char const *pTitle) {
    u8 *temp;//[1024] = {0};
    u8 keybuf[64] = {0};
    s32 templen = 0, keylen = 0;

    temp = (u8 *) sdkGetMem(1024);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 1024);

//0�����ò�����Կ
    keylen = Test_sdkMem4442SetPara(keybuf);

    do {
//1���ϵ����
        if (Test_sdkMem4442PowerOn() == SDK_KEY_ESC) { break; }

//2��Ѱ������
        if (Test_sdkMem4442QueryCard() == SDK_KEY_ESC) { break; }

//3����λ����
        if (Test_sdkMem4442Reset() == SDK_KEY_ESC) { break; }

//4��У����Կ
        if (Test_sdkMem4442VerifyKey(keybuf, keylen) == SDK_KEY_ESC) { break; }

//5���������
        if (Test_sdkMem4442ReadBlock(NULL, 0) == SDK_KEY_ESC) { break; }

//6��д�����
        if (Test_sdkMem4442WriteBlock(temp, &templen) == SDK_KEY_ESC) { break; }

//7���ٴζ������
        if (Test_sdkMem4442ReadBlock(temp, templen) == SDK_KEY_ESC) { break; }

//8��������Կ����
        if (Test_sdkMem4442UpdateKey(keybuf, keylen) == SDK_KEY_ESC) { break; }

//9���������������
        if (Test_sdkMem4442ReadPwdAndPwdCount() == SDK_KEY_ESC) { break; }

//10���µ����
        if (Test_sdkMem4442PowerDown() == SDK_KEY_ESC) { break; }
    }
    while (0);

    sdkFreeMem(temp);
    temp = NULL;

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428PowerOn(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;//[1024] = {0};
    SDK_ICC_PARAM stparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID]);
        sdkDispMsgBox("�����ϵ�", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("�����ϵ�",
                           (rslt = sdkIccMemPowerOn(&stparam, outbuf, &outlen) == SDK_OK));

//4�������ʾ
    p = (rslt == SDK_OK) ? ("�ϵ�ɹ�") : ("�ϵ�ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428QueryCard(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;//[1024] = {0};
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);
//1������׼��

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID]);
        sdkDispMsgBox("����Ѱ��", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("����Ѱ��", ((rslt = sdkIccMemCheck(outbuf, &outlen) == SDK_OK) &&
                                       (outbuf[0] == 0x01)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x01) ? ("Ѱ���ɹ�") : ("Ѱ��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428Reset(void) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf;//[1024] = {0};
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    psticcparam.eCardType = gCardType[gCardTypeID];
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID]);
        sdkDispMsgBox("���Ը�λ", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Ը�λ",
                           ((rslt = sdkIccReset(&psticcparam, outbuf, &outlen) == SDK_OK) &&
                            (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("��λ�ɹ�") : ("��λʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428VerifyKey(u8 *pkey, s32 keylen) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM psticcparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    psticcparam.eCardType = gCardType[gCardTypeID];
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//    memcpy(inbuf, "\xFF\xFF\xFF\xFF", 4); //4��F��Կ

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r��Կ:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID], pkey);

        sdkDispMsgBox("������ԿУ��", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    memset(outbuf, 0, sizeof(outbuf));
    TEST_IS_TRUE_WITH_INFO("������ԿУ��",
                           ((rslt = sdkIccMemCheckPwd(&psticcparam, pkey, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("У��ɹ�") : ("У��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *inbuf
                               s32 inlen
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428ReadBlock(u8 *inbuf, s32 inlen) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    u8 temp[64] = {0};
    SDK_ICC_PARAM stparam;
    SDK_ICC_RMEMIN stdatain;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

    stdatain.ucAddress = 40; //32ǰ��Ϊ��Կ���������32�Ժ��ȡ
    stdatain.ucReadLen = 8;

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r��ʼ��ַ:%d\r�����ݳ���:%d\r����ȷ�ϡ�����",
                gCardName[gCardTypeID], stdatain.ucAddress, stdatain.ucReadLen);

        sdkDispMsgBox("���Կ��ȡ", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ��ȡ",
                           ((rslt = sdkIccMemReadData(&stparam, &stdatain, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));
    TraceHex("emv", "mem4428 read out", outbuf, outlen);

//4�������ʾ
    if (rslt == SDK_OK && outbuf[0] == 0x00) {
        if ((inbuf == NULL) || (inlen == 0)) //�״ζ��������ж�
        {
            p = "��ȡ�ɹ�";
        }
        else if ((inlen == (outlen - 1)) && (!memcmp(inbuf, &outbuf[1], inlen))) {
            p = "��ȡ��У��ɹ�";
        }
    }
    else {
        p = "��ȡʧ��";
    }

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *wbuf_out
                               s32 *wlen_out
** Returned value:
** Created By:		���ٲ�  2013.10.25
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428WriteBlock(u8 *wbuf_out, s32 *wlen_out) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    SDK_ICC_PARAM stparam;
    SDK_ICC_WMEMIN stdatain;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

    stdatain.ucAddress = 40; //32ǰ��Ϊ��Կ���������32�Ժ����
    sdkGetRandom(stdatain.ucWriteData, 8);
    stdatain.ucWriteLen = 8;

    memcpy(wbuf_out, stdatain.ucWriteData, stdatain.ucWriteLen);

//2����ʾ��ʾ
    rslt = sdkBcdToAsc(outbuf, stdatain.ucWriteData, stdatain.ucWriteLen);
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\rд���ַ:%d\rд������:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID],
                stdatain.ucAddress, outbuf);

        sdkDispMsgBox("���Կ�д��", showInfo, 0, SDK_KEY_MASK_ALL);
    }

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("���Կ�д��",
                           ((rslt = sdkIccMemWriteData(&stparam, &stdatain, outbuf, &outlen) ==
                                    SDK_OK) && (outbuf[0] == 0x00)));

//4�������ʾ
    *wlen_out = (rslt == SDK_OK && outbuf[0] == 0x00) ? (stdatain.ucWriteLen) : (0);

    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("д��ɹ�") : ("д��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428UpdateKey(u8 *pkey, s32 keylen) {
    s32 rslt = 0, outlen = 0;
    u8 *outbuf, *dispbuf;
    u8 newkey[64] = {0}, temp[64] = {0};
    SDK_ICC_PARAM stparam;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(1024);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 1024);

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//    memcpy(oldkey, "\xFF\xFF\xFF\xFF", 4);
    memcpy(newkey, "\x46\x45\x46\x45", 4);

//2����ʾ��ʾ
//    rslt = sdkBcdToAsc(outbuf, oldkey, 4);
//    rslt = sdkBcdToAsc(temp, newkey, 4);
    sprintf(dispbuf, "������:%s\rԭʼ��Կ:%s\r������Կ:%s\r����ȷ�ϡ�����\r����ȡ�����˳�",
            gCardName[gCardTypeID], pkey, newkey);

    do {
        if (sdkDispMsgBox("������Կ����", dispbuf, 0, SDK_KEY_MASK_ALL) == SDK_KEY_ESC) { break; }

        //3�����Բ���
        TEST_IS_TRUE_WITH_INFO("������Կ����",
                               ((rslt = sdkIccMemModifyPwd(&stparam, pkey, newkey, outbuf,
                                                           &outlen) == SDK_OK) &&
                                (outbuf[0] == 0x00)));

        //4������ж�
        if (rslt != SDK_OK || outbuf[0] != 0x00) { break; }

        //5�����ԸĻز���
        TEST_IS_TRUE_WITH_INFO("������Կ�Ļ�",
                               ((rslt = sdkIccMemModifyPwd(&stparam, newkey, pkey, outbuf,
                                                           &outlen) == SDK_OK) &&
                                (outbuf[0] == 0x00)));
    }
    while (0);


//6�������ʾ
    p = (rslt == SDK_OK && outbuf[0] == 0x00) ? ("���²��Գɹ�") : ("���²���ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:
*****************************************************************************/
static s32 Test_sdkMem4428PowerDown(void) {
    s32 rslt = 0;
    SDK_ICC_PARAM stparam;
    u8 *p = NULL;

//1������׼��
    stparam.eCardType = gCardType[gCardTypeID];
    stparam.ucCardMode = SDK_ICC_ICC;
    stparam.ucSlotNo = 0;//������ʹ�õײ��ȡ

//2����ʾ��ʾ
    {
        u8 showInfo[128];
        sprintf(showInfo, "������:%s\r����ȷ�ϡ�����", gCardName[gCardTypeID], 0, SDK_KEY_MASK_ALL);
        sdkDispMsgBox("�����µ�", showInfo, 0, SDK_KEY_MASK_ALL);
    }
    sdkDispMsgBox("�����µ�", "������:4428\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    TEST_IS_TRUE_WITH_INFO("�����µ�", (rslt = sdkIccMemMifarePowerDown(&stparam) == SDK_OK));

//4�������ʾ
    p = (rslt == SDK_OK) ? ("�µ�ɹ�") : ("�µ�ʧ��");

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);
}


/*****************************************************************************
** Descriptions:	����mem4428����Կ����
** Parameters:    	u8 *pkey
** Returned value:	
** Created By:		���ٲ�  2014.03.31
** Remarks: 		������Կ����
*****************************************************************************/
static s32 Test_sdkMem4428SetPara(u8 *pkey) {
    s32 rslt = 0;
    u8 *ptitle = NULL;

    if (pkey == NULL) {
        return SDK_PARA_ERR;
    }

    switch (gCardType[gCardTypeID]) {
        case SDK_ICC_SLE4428://MEMORYCARDSLE4428//sjl20120724
        {
            ptitle = "���ò��Կ�mem4428��Կ";
            break;
        }
        case SDK_ICC_SLE4442://MEMORYCARDSLE4442
        {
            ptitle = "���ò��Կ�mem4442��Կ";
            break;
        }
        case SDK_ICC_AT24C01://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C02://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C04://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C08://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C16://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C32://MEMORYCARDAT24Cxx
        case SDK_ICC_AT24C64://MEMORYCARDAT24Cxx
        {
            return 0;
        }
        case SDK_ICC_1604://MEMORYCARDAT88SC1608/AT88SC153
        {
            ptitle = "���ò��Կ�mem1604��Կ";
            break;
        }
        case SDK_ICC_AT88SC153://MEMORYCARDAT88SC153
        case SDK_ICC_AT88SC1608://MEMORYCARDAT88SC1608/
        {
            ptitle = "���ò��Կ�memAT88SC��Կ";
            break;
        }
        default:
            return 0;
    }
    rslt = sdkTestInputASC(pkey, ptitle, "����������:", 1, 12);

    TraceHex("hsb", "key value", pkey, rslt);

    return rslt;
}


/*****************************************************************************
** Descriptions:	        4428������ʾ��
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.10.17
** Remarks:             �������޸���Կ����
*****************************************************************************/
s32 Test_sdkIccMem4428TestDemon(char *pasOutInfo, char const *pTitle) {
    u8 *temp;//[1024] = {0};
    u8 keybuf[64] = {0};
    s32 templen = 0, keylen = 0;

    temp = (u8 *) sdkGetMem(1024);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 1024);

//0�����ò���
    keylen = Test_sdkMem4428SetPara(keybuf);

    do {
//1���ϵ����
        if (Test_sdkMem4428PowerOn() == SDK_KEY_ESC) { break; }

//2��Ѱ������
        if (Test_sdkMem4428QueryCard() == SDK_KEY_ESC) { break; }

//3����λ����
        if (Test_sdkMem4428Reset() == SDK_KEY_ESC) { break; }

//4��У����Կ
        if (Test_sdkMem4428VerifyKey(keybuf, keylen) == SDK_KEY_ESC) { break; }

//5���������
        if (Test_sdkMem4428ReadBlock(NULL, 0) == SDK_KEY_ESC) { break; }

//6��д�����
        if (Test_sdkMem4428WriteBlock(temp, &templen) == SDK_KEY_ESC) { break; }

//7���ٴζ������
        if (Test_sdkMem4428ReadBlock(temp, templen) == SDK_KEY_ESC) { break; }

//8��������Կ����
        if (Test_sdkMem4428UpdateKey(keybuf, keylen) == SDK_KEY_ESC) { break; }

//9���µ����
        if (Test_sdkMem4428PowerDown() == SDK_KEY_ESC) { break; }
    }
    while (0);

    sdkFreeMem(temp);
    temp = NULL;

    return SDK_OK;
}

/*====================== END======================== */




/*=======BEGIN: ���ٲ� 2013.11.06  9:43 add for 2.4G card===========*/

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccTest24GCardQuery(void) {
    u8 cardtype = 0, *temp = NULL;
    s32 rslt = 0;
    u8 *p = NULL;

    cardtype = SDK_ICC_RFCARD_24G;

    temp = (u8 *) sdkGetMem(512);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 512);

    sdkDispMsgBox("����Ѱ��", "������:2.4G��\r��ʱʱ��:10s\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

    TEST_IS_TRUE_WITH_INFO("����Ѱ��", (rslt = sdkIccRFQuery(cardtype, temp, 10000) > 0));

    p = (rslt > 0) ? ("Ѱ���ɹ�") : ("Ѱ��ʧ��");

    rslt = sdkDispMsgBox(p, "����ȷ�ϡ�����APDU\r����ȡ�����˳�", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(temp);
    temp = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *inbuf
                               s32 inlen
                               SDK_ICC_APDU_SEND *apduout
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 sdkIccFormApdu(u8 *inbuf, s32 inlen, SDK_ICC_APDU_SEND *apduout) {
    if (inlen < 4 || apduout == NULL || inbuf == NULL) { return SDK_PARA_ERR; }
    memcpy(apduout->Command, inbuf, 4);

    if (inlen == 5)            //ֻ��5�ֽڴ�����Ҫ��������
    {
        apduout->Le = inbuf[4];
    }
    else if (inlen > 5) {
        if (inbuf[4] == 0x00) {
            return SDK_PARA_ERR;
        }
        else {
            apduout->Lc = inbuf[4];
        }

        if (inlen == apduout->Lc + 5)              //����Ӧ����
        {
            memcpy(apduout->DataIn, inbuf + 5, apduout->Lc);                //��������
        }
        else if (inlen == apduout->Lc + 6)              //����Ӧ����
        {
            memcpy(apduout->DataIn, inbuf + 5, apduout->Lc);                //��������
            apduout->Le = (inbuf[apduout->Lc + 5] == 0x00) ? (256) : (inbuf[apduout->Lc + 5]);
        }
        else {
            return SDK_PARA_ERR;
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_APDU_RESP apduin
                               u8 *outbuf
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 sdkIccApduToBuf(SDK_ICC_APDU_RESP apduin, u8 *outbuf) {
    s32 len = 0;

    if (outbuf == NULL) { return SDK_PARA_ERR; }

    if (apduin.LenOut > 0) {
        memcpy(outbuf, apduin.DataOut, apduin.LenOut);
        len += apduin.LenOut;
    }
    outbuf[len++] = apduin.SWA;
    outbuf[len++] = apduin.SWB;
    TraceHex("emv", "APDU to buf����", outbuf, len);
    return len;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccTest24GCardSelectFile(void) {
    u8 cardtype = 0, *temp = NULL, inbuf[32] = {0};
    u8 *dispbuf, *outbuf;
    s32 rslt = 0, inlen = 0, outlen = 0;
    u8 *p = NULL;
    SDK_ICC_PARAM psticcparam;
    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(512);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 512);

    temp = (u8 *) sdkGetMem(256);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 256);

    memset(&psticcparam, 0, sizeof(SDK_ICC_PARAM));
    memset(&pstApduSend, 0, sizeof(SDK_ICC_APDU_SEND));
    memset(&pstApduResp, 0, sizeof(SDK_ICC_APDU_RESP));


    psticcparam.eCardType = SDK_ICC_CPU24;
    psticcparam.ucCardMode = SDK_ICC_RF;
    cardtype = SDK_ICC_RFCARD_24G;
    memcpy(inbuf, "\x00\xA4\x04\x00\x02\x10\x01", 7);
    inlen = 7;
    sdkIccFormApdu(inbuf, inlen, &pstApduSend);

    sdkBcdToAsc(outbuf, inbuf, inlen);
    sprintf(dispbuf, "������:2.4G��\rAPDU����:%s\r����ȷ�ϡ�����", outbuf);
    sdkDispMsgBox("����APDU", dispbuf, 0, SDK_KEY_MASK_ALL);


    TEST_IS_TRUE_WITH_INFO("����APDU",
                           ((rslt = sdkIccSendAPDU(&psticcparam, &pstApduSend, &pstApduResp) ==
                                    SDK_OK)));

    p = (rslt == SDK_OK) ? ("����APDU�ɹ�") : ("����APDUʧ��");

    memset(temp, 0, 256);

    if (rslt == SDK_OK) {
        outlen = sdkIccApduToBuf(pstApduResp, outbuf);
        sdkBcdToAsc(temp, outbuf, outlen);
        memset(dispbuf, 0, sizeof(dispbuf));
        sprintf(dispbuf, "APDU��������:%s\r����ȷ�ϡ�������һ��ָ��\r����ȡ�����˳�", temp);
    }
    else {
        sprintf(dispbuf, "����ȷ�ϡ�������һ��ָ��\r����ȡ�����˳�");
    }
    rslt = sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;

    sdkFreeMem(temp);
    temp = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccTest24GCardReadRecord(void) {
    u8 cardtype = 0, temp[512] = {0}, dispbuf[256] = {0}, inbuf[32] = {0}, outbuf[512] = {0};
    s32 rslt = 0, inlen = 0, outlen = 0;
    u8 *p = NULL;
    SDK_ICC_PARAM psticcparam;
    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;

    memset(&psticcparam, 0, sizeof(SDK_ICC_PARAM));
    memset(&pstApduSend, 0, sizeof(SDK_ICC_APDU_SEND));
    memset(&pstApduResp, 0, sizeof(SDK_ICC_APDU_RESP));

    psticcparam.eCardType = SDK_ICC_CPU24;
    psticcparam.ucCardMode = SDK_ICC_RF;
    cardtype = SDK_ICC_RFCARD_24G;
    memcpy(inbuf, "\x00\xB2\x01\xC4\x23", 5);
    inlen = 5;
    sdkIccFormApdu(inbuf, inlen, &pstApduSend);

    sdkBcdToAsc(outbuf, inbuf, inlen);
    sprintf(dispbuf, "������:2.4G��\rAPDU����:%s\r����ȷ�ϡ�����", outbuf);
    sdkDispMsgBox("����APDU", dispbuf, 0, SDK_KEY_MASK_ALL);


    TEST_IS_TRUE_WITH_INFO("����APDU",
                           ((rslt = sdkIccSendAPDU(&psticcparam, &pstApduSend, &pstApduResp) ==
                                    SDK_OK)));

    p = (rslt == SDK_OK) ? ("����APDU�ɹ�") : ("����APDUʧ��");

    memset(temp, 0, sizeof(temp));

    if (rslt == SDK_OK) {
        outlen = sdkIccApduToBuf(pstApduResp, outbuf);
        sdkBcdToAsc(temp, outbuf, outlen);
        memset(dispbuf, 0, sizeof(dispbuf));
        sprintf(dispbuf, "APDU��������:%s\r����ȷ�ϡ�������һ��ָ��\r����ȡ�����˳�", temp);
    }
    else {
        sprintf(dispbuf, "����ȷ�ϡ�������һ��ָ��\r����ȡ�����˳�");
    }
    return sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccTest24GCardGetData(void) {
    u8 cardtype = 0, inbuf[32] = {0};
    u8 *dispbuf, *outbuf, *temp;
    s32 rslt = 0, inlen = 0, outlen = 0;
    u8 *p = NULL;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

    dispbuf = (u8 *) sdkGetMem(512);
    if (dispbuf == NULL) {
        TestLog("", "Allocate for dispbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(dispbuf, 0, 512);

    temp = (u8 *) sdkGetMem(512);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, 512);

    SDK_ICC_PARAM psticcparam;
    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;

    memset(&psticcparam, 0, sizeof(SDK_ICC_PARAM));
    memset(&pstApduSend, 0, sizeof(SDK_ICC_APDU_SEND));
    memset(&pstApduResp, 0, sizeof(SDK_ICC_APDU_RESP));

    psticcparam.eCardType = SDK_ICC_CPU24;
    psticcparam.ucCardMode = SDK_ICC_RF;
    cardtype = SDK_ICC_RFCARD_24G;
    memcpy(inbuf, "\x80\x5C\x00\x02\x04", 5);
    inlen = 5;
    sdkIccFormApdu(inbuf, inlen, &pstApduSend);

    sdkBcdToAsc(outbuf, inbuf, inlen);
    sprintf(dispbuf, "������:2.4G��\rAPDU����:%s\r����ȷ�ϡ�����", outbuf);
    sdkDispMsgBox("����APDU", dispbuf, 0, SDK_KEY_MASK_ALL);

    TEST_IS_TRUE_WITH_INFO("����APDU",
                           ((rslt = sdkIccSendAPDU(&psticcparam, &pstApduSend, &pstApduResp) ==
                                    SDK_OK)));

    p = (rslt == SDK_OK) ? ("����APDU�ɹ�") : ("����APDUʧ��");

    memset(temp, 0, sizeof(temp));

    if (rslt == SDK_OK) {
        outlen = sdkIccApduToBuf(pstApduResp, outbuf);
        sdkBcdToAsc(temp, outbuf, outlen);
        memset(dispbuf, 0, sizeof(dispbuf));
        sprintf(dispbuf, "APDU��������:%s\r����ȷ�ϡ����Խ���", temp);
    }
    else {
        sprintf(dispbuf, "����ȷ�ϡ����Խ���");
    }
    rslt = sdkDispMsgBox(p, dispbuf, 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    sdkFreeMem(dispbuf);
    dispbuf = NULL;

    sdkFreeMem(temp);
    temp = NULL;

    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          char *pasOutInfo
                               char const*pTitle
** Returned value:
** Created By:		���ٲ�  2013.11.05
** Remarks:
*****************************************************************************/
static s32 Test_sdkIccTest24GCardDemon(char *pasOutInfo, char const *pTitle) {
    sdkIccOpenRfDev();

    /*=======BEGIN: ���ٲ� 2013.11.18  8:28 modify===========*/
    //ȥ�����������goto���
    do {
        if (Test_sdkIccTest24GCardQuery() == SDK_KEY_ESC) { break; }

        if (Test_sdkIccTest24GCardSelectFile() == SDK_KEY_ESC) { break; }

        if (Test_sdkIccTest24GCardReadRecord() == SDK_KEY_ESC) { break; }

        if (Test_sdkIccTest24GCardGetData() == SDK_KEY_ESC) { break; }
    }
    while (0);

    /*====================== END======================== */
    sdkIccCloseRfDev();
    return SDK_OK;
}

/*====================== END======================== */




/*=======BEGIN: ���ٲ� 2014.02.11  16:6 add for �ǽ�IC�ӿڲ���===========*/
/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.11
** Remarks:
*****************************************************************************/
s32 Test_sdkIccRFModuleJudge(char *pasOutInfo, char const *pTitle) {
    s32 rslt = 0;
    u8 *p = NULL;

    //2����ʾ��ʾ
    sdkDispMsgBox("�����жϷǽ�ģ��", "��ʱʱ��:500ms\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);


//3�����Բ���
#if 0 /*Modify by ���ƽ at 2014.08.21  18:08*/
    rslt = sdkIccIsInRfModule(500);
#endif /* if 0 */
//4�������ʾ
    switch (rslt) {
        case 0: {
            p = "�޷ǽ�ģ��";
        }
            break;

        case 1: {
            p = "����PIN512ģ��";
        }
            break;

        case 2: {
            p = "����STM32ģ��";
        }
            break;

        default: {
            p = "���Դ���";
        }
            break;
    }

    return sdkDispMsgBox(p, "����ȷ�ϡ��˳�", 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ���Էǽ�Ѱ�������ӷǽ��µ紦��
*****************************************************************************/
s32 Test_sdkIcDealRFQuery(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0, time = 0, temp_size = 256;
    u8 *p = NULL;
    u8 cardtype = 0, *temp = NULL;
    SDK_ICC_PARAM psticcparam;

    temp = (u8 *) sdkGetMem(temp_size);
    if (temp == NULL) {
        TestLog("", "Allocate for temp[] Error!\r\n");
        return SDK_ERR;
    }
    memset(temp, 0, temp_size);

//1������׼��
    memset(&psticcparam, 0, sizeof(SDK_ICC_PARAM));
    psticcparam.ucCardMode = SDK_ICC_RF;
    time = 10000; //10����Ѱ��
    cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G;

//2����ʾ��ʾ
    sdkDispMsgBox("���Էǽ�Ѱ��", "������:�ǽӴ�ʽ��Ƭ\r����˵��:��ȷ�Ϻ�10������ӿ�\r����ȷ�ϡ�����", 0,
                  SDK_KEY_MASK_ALL);

//3�����Բ���
    sdkIccOpenRfDev();
    ret = sdkIccRFQuery(cardtype, temp, time);

//4�������ʾ
    p = (ret >= 0 && temp[0] == 0) ? ("Ѱ���ǽӿ�") : ("δѰ���ǽӿ�");

    sdkIccMemMifarePowerDown(&psticcparam);//���ӷǽ��µ紦��   
    sdkIccCloseRfDev();

    ret = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(temp);
    temp = NULL;

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ���Կ�Ƭ��λ(�������ǽ�)
*****************************************************************************/
s32 Test_sdkIcDealIccReset(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0, time = 0, outlen = 0;
    u8 *p = NULL, *outbuf;//[1024] = {0};
    SDK_ICC_PARAM psticcparam;

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��

//2����ʾ��ʾ
    ret = sdkDispMsgBox("���Կ�Ƭ��λ", "������:�Ӵ�ʽCPU��\r����˵��:��忨\r����ȷ�ϡ�����", 0,
                        SDK_KEY_MASK_ALL);
    psticcparam.ucSlotNo = 0; //Ĭ��0��slot
    psticcparam.ucCardMode = SDK_ICC_ICC;
    psticcparam.eCardType = SDK_ICC_CPU;

//3�����Բ���
    sdkIccOpenIcDev();
    TEST_IS_TRUE_WITH_INFO("���Կ�Ƭ��λ",
                           (ret = sdkIccReset(&psticcparam, outbuf, &outlen) == SDK_OK));
    sdkIccCloseIcDev();
//4�������ʾ
    p = (ret == SDK_OK) ? ("��λ�ɹ�") : ("��λʧ��");

    ret = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ����APDU
*****************************************************************************/
s32 Test_sdkIcDealIccSendAPDU(char *pasOutInfo, char const *pTitle) {
    u8 temp[200] = {0};
    s32 ret = 0, len = 0;
    SDK_ICC_PARAM psticcparam;
    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;
    u8 cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G;
    u8 *p = NULL;
    u32 timerid = 0;

//    sdkDispLabel(SDK_DISP_LINE1, pTitle, SDK_DISP_DEFAULT);
//    sdkDispLabel(SDK_DISP_LINE2, "��忨/�ӿ�", SDK_DISP_LEFT_DEFAULT);
    sdkKbKeyFlush();
    sdkIccOpenRfDev();
    sdkIccOpenIcDev();

    sdkDispMsgBox("����APDU", "������:�Ӵ�ʽ�ͷǽӴ�ʽ��Ƭ\r����˵��:��ȷ�Ϻ�10������ӿ���忨\r����ȷ�ϡ�����", 0,
                  SDK_KEY_MASK_ALL);

    timerid = sdkTimerGetId();

    while (1) {
        ret = sdkIccRFQuery(cardtype, temp, 100);
        TraceHex("emv", "the rf query temp:", temp, ret);
        if (ret >= 0 && temp[0] == 0) {
            psticcparam.eCardType = SDK_ICC_CPU;
            psticcparam.ucCardMode = SDK_ICC_RF;
            psticcparam.ucSlotNo = temp[1];
            ret = SDK_OK;
            break;
        }
        else if (ret == SDK_ESC) {
//            sprintf(pasOutInfo, "ȡ���˳�");
            ret = SDK_ESC;
            break;
        }
        else if (ret != SDK_TIME_OUT && ret != SDK_ICC_BAT_VOLTAGE_LOW) {
//            sprintf(pasOutInfo, "Ѱ�ǽӿ��쳣�˳�\r�����sdkIccRFQuery");
            ret = SDK_ERR;
            break;
        }

        ret = sdkIccIsInSlot();

        if (ret == SDK_OK) {
            psticcparam.eCardType = SDK_ICC_CPU;
            psticcparam.ucCardMode = SDK_ICC_ICC;
            psticcparam.ucSlotNo = sdkIcGetIcdealCardNo();
            ret = SDK_OK;
            break;
        }

        if (sdkKbGetKey() == SDK_KEY_ESC) {
//            sprintf(pasOutInfo, "ȡ���˳�");
            ret = SDK_ESC;
            break;
        }

        if (1 == sdkTimerIsEnd(timerid, 10000)) {
            ret = SDK_TIME_OUT;
            break;
        }
    }

    if (SDK_OK == ret) {
        if (SDK_OK != sdkIccReset(&psticcparam, temp, &len)) {
//            sprintf(pasOutInfo, "��λʧ��\r�����sdkIccReset");
            p = "��λʧ��";
        }
        else {
            memset(&pstApduSend, 0, sizeof(pstApduSend));
            memset(&pstApduResp, 0, sizeof(pstApduResp));

            memcpy(pstApduSend.Command, "\x00\xA4\x04\x00", 4);
            pstApduSend.Lc = 14;
            pstApduSend.Le = 256;
            pstApduSend.EnableCancel = 1;           //ʹ�ܰ�ȡ����

            if (psticcparam.ucCardMode == SDK_ICC_ICC) {
                memcpy(pstApduSend.DataIn, "1PAY.SYS.DDF01", pstApduSend.Lc);
            }
            else {
                memcpy(pstApduSend.DataIn, "2PAY.SYS.DDF01", pstApduSend.Lc);
            }

            if (sdkIccSendAPDU(&psticcparam, &pstApduSend, &pstApduResp) != SDK_OK) {
                p = "APDU����ʧ��";
            }
            else {
                p = (pstApduResp.SWA == 0x90 && pstApduResp.SWB == 0x00 &&
                     pstApduResp.DataOut[0] == 0x6F) ? ("APDU���Գɹ�")
                                                     : ("APDU����ʧ��"); //�ж�APDU�ķ���
            }
        }
    }
    else {
        p = (ret == SDK_ESC) ? ("��ȡ���˳�") : ("Ѱ��ʧ��");
    }
    sdkIccCloseIcDev();
    sdkIccCloseRfDev();
    psticcparam.ucCardMode = SDK_ICC_RF;
    sdkIccMemMifarePowerDown(&psticcparam);//���ӷǽ��µ紦��  
    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ���Բ�ѯIC���Ƿ���뿨����
*****************************************************************************/
s32 Test_sdkIcDealIsIccCardIn(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0, time = 0, outlen = 0;
    u8 *p = NULL, *outbuf;//[1024] = {0};

    outbuf = (u8 *) sdkGetMem(1024);
    if (outbuf == NULL) {
        TestLog("", "Allocate for outbuf[] Error!\r\n");
        return SDK_ERR;
    }
    memset(outbuf, 0, 1024);

//1������׼��
    time = 5000; //5���ڲ忨

//2����ʾ��ʾ
    sdkDispMsgBox("���Կ��Ƿ���뿨��", "������:�Ӵ�ʽ��Ƭ\r����˵��:��ȷ�Ϻ�5������忨\r����ȷ�ϡ�����", 0,
                  SDK_KEY_MASK_ALL);

//3�����Բ���
    sdkIccOpenIcDev();
    TEST_IS_TRUE_WITH_INFO("���Կ��Ƿ���뿨��",
                           (ret = sdkIccIsIccCardIn(time, outbuf, &outlen) == SDK_OK));
    sdkIccCloseIcDev();
//4�������ʾ
    p = (ret == SDK_OK) ? ("�п�����") : ("�޿�����");

    ret = sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);

    sdkFreeMem(outbuf);
    outbuf = NULL;

    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ���Բ�ѯIC���Ƿ��ڿ�����
*****************************************************************************/
s32 Test_sdkIcDealIccIsInSlot(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    u8 *p = NULL;

//1������׼��

//2����ʾ��ʾ
    sdkDispMsgBox("���Կ��Ƿ��ڿ���", "������:�Ӵ�ʽ��Ƭ\r����˵��:��忨\r����ȷ�ϡ�����", 0,
                  SDK_KEY_MASK_ALL);

//3�����Բ���
    sdkIccOpenIcDev();
    TEST_IS_TRUE_WITH_INFO("���Կ��Ƿ��ڿ���", (ret = sdkIccIsInSlot() == SDK_OK));
    sdkIccCloseIcDev();

//4�������ʾ
    p = (ret == SDK_OK) ? ("�п��ڿ���") : ("�޿��ڿ���");

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		���ٲ�  2014.02.27
** Remarks:             ���Բ�ѯIC���Ƿ�γ�
*****************************************************************************/
s32 Test_sdkIcDealIsCardOut(char *pasOutInfo, char const *pTitle) {
    s32 ret = 0;
    s32 timer = 0;
    u8 *p = NULL;

//1������׼��
    timer = 5000; //5�����Ƿ��п��γ�

//2����ʾ��ʾ
    sdkDispMsgBox("���Կ��Ƿ�γ�", "������:�Ӵ�ʽ��Ƭ\r��ʱʱ��:5��\r����ȷ�ϡ�����", 0, SDK_KEY_MASK_ALL);

//3�����Բ���
    sdkIccOpenIcDev();
    TEST_IS_TRUE_WITH_INFO("���Կ��Ƿ�γ�", (ret = sdkIccIsCardOut(timer) == SDK_OK));
    sdkIccCloseIcDev();

//4�������ʾ
    p = (ret == SDK_OK) ? ("�п��γ�") : ("�޿��γ�");

    return sdkDispMsgBox(p, "����ȷ�ϡ����Ժ���", 0, SDK_KEY_MASK_ALL);
}


//���̲���
/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		���ٲ�  2014.03.06
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP IcdealDemonTestMap[] =
        {
                {"MifareTestDemon", Test_sdkIccMifareTestDemon},
                {"Mem4428TestDemon", Test_sdkIccMem4428TestDemon},
                {"Mem4442TestDemon", Test_sdkIccMem4442TestDemon},
                {"MemAT24C01TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C02TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C04TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C08TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C16TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C32TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT24C64TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemIS23SC1604TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT88SC153TestDemon", Test_sdkIccMem4428TestDemon},
                {"MemAT88SC1608TestDemon", Test_sdkIccMem4428TestDemon},
#if MACHINEMODEL != SDK_SYS_MACHINE_G2 && MACHINEMODEL != SDK_SYS_MACHINE_200P /*Modify by ���ƽ at 2014.07.03  10:20 */
                {"2.4GCardTestDemon", Test_sdkIccTest24GCardDemon}
#endif /* if 0 */
        };

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		���ٲ�  2014.03.06
** Remarks:
*****************************************************************************/
TEST_MAP_INFO GetIcdealDemonTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(IcdealDemonTestMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = IcdealDemonTestMap;
    return ret;
}

//�����ӿڲ���
/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		���ٲ�  2014.03.06
** Remarks:
*****************************************************************************/
static const TEST_MANUAL_MAP IcdealFunctionTestMap[] =
        {
                {"RFModuleJudge",     Test_sdkIccRFModuleJudge},
                {"IcDealRFQuery",     Test_sdkIcDealRFQuery},
                {"IcDealIccReset",    Test_sdkIcDealIccReset},
                {"IcDealIccSendAPDU", Test_sdkIcDealIccSendAPDU},
                {"IcDealIsIccCardIn", Test_sdkIcDealIsIccCardIn},
                {"IcDealIccIsInSlot", Test_sdkIcDealIccIsInSlot},
                {"IcDealIsCardOut",   Test_sdkIcDealIsCardOut}
        };


/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		���ٲ�  2014.03.06
** Remarks:
*****************************************************************************/
TEST_MAP_INFO GetIcdealFunctionTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(IcdealFunctionTestMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = IcdealFunctionTestMap;
    return ret;
}

#if 0 /*Modify by ���ٲ� at 2014.02.28  8:18 */

#endif /* if 0 */

/*====================== END======================== */




static s32 Test_sdkIccRFQueryManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[200] = {0};
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    u8 uc = sdkTestInputU8(pTitle, "������:");
    s32 overtime = sdkTestInputs32(pTitle, "��ʱʱ��:");
    s32 ret = 0, len = 0;

    memset(temp, 0, sizeof(temp));
    memset(temp2, 0, sizeof(temp2));
    TestLog("mifare", "������:%x\r\n", uc);
    sdkDispMsgBox("mifareѰ��", "����Ѱ��...", 5, SDK_KEY_MASK_ENTER);
    sdkIccOpenRfDev();
    ret = sdkIccRFQuery(uc, temp, overtime);
    sdkIccCloseRfDev();
    TestLogHex("mifare", "sdkIccRFQuery temp is ", temp, ret);
    len = sdkBcdToAsc(temp2, temp, ret);
    Verify(sdkU8ToAsc(uc, temp3) >= 0);
    sprintf(pasOutInfo, "������:%s \r��ʱʱ��:%d \r��������:%s\rreturn:%d",
            temp3, overtime, temp2, ret);
    return SDK_OK;
}

static s32 Test_sdkIccResetManual(char *pasOutInfo, char const *pTitle, s32 cardtype, u8 cardmode) {
    u8 temp[200] = {0};
    u8 temp2[200] = {0};
    u8 cttemp[200] = {0};
    u8 cmtemp[200] = {0};
    u8 sntemp[200] = {0};
    s32 ret = 0, len = 0;
    SDK_ICC_PARAM psticcparam;

    memset(temp, 0, sizeof(temp));
    memset(temp2, 0, sizeof(temp2));
    memset(&psticcparam, 0, sizeof(psticcparam));

    // psticcparam.eCardType =  sdkTestInputU8(pTitle, "������:");
    // psticcparam.ucCardMode = sdkTestInputU8(pTitle, "����ģʽ:");
    //psticcparam.ucSlotNo = sdkTestInputU8(pTitle, "������:");

    psticcparam.eCardType = cardtype;
    psticcparam.ucCardMode = cardmode;
    psticcparam.ucSlotNo = 0; //Ĭ��0��slot

    TestLog("emv", "\r\n eCardType = %d ucCardMode = %d \r\n", psticcparam.eCardType,
            psticcparam.ucCardMode);

    ret = sdkIccReset(&psticcparam, temp, &len);
    TestLogHex("mifare", "sdkIccReset temp is ", temp, len);
    Verify(sdkBcdToAsc(temp2, temp, len) >= 0);
    Verify(sdkU8ToAsc(psticcparam.eCardType, cttemp) >= 0);
    Verify(sdkU8ToAsc(psticcparam.ucCardMode, cmtemp) >= 0);
    Verify(sdkU8ToAsc(psticcparam.ucSlotNo, sntemp) >= 0);
    sprintf(pasOutInfo, "������:%s \r����ģʽ:%s \r������:%s \r��������:%s\rreturn len:%d ret :%d \r",
            cttemp, cmtemp, sntemp, temp2, len, ret);
    return SDK_OK;
}

static s32 Test_sdkIccSendAPDUManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[200] = {0};
    u8 temp2[200] = {0};
    u8 cttemp[200] = {0};
    u8 cmtemp[200] = {0};
    u8 sntemp[200] = {0};
    s32 ret = 0, len = 0, i = 0, j = 0;
    SDK_ICC_PARAM psticcparam;
    SDK_ICC_APDU_SEND pstApduSend;
    SDK_ICC_APDU_RESP pstApduResp;
    u8 cardtype = SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B | SDK_ICC_RFCARD_FELICA | SDK_ICC_RFCARD_24G;

    sdkDispLabel(SDK_DISP_LINE1, pTitle, SDK_DISP_DEFAULT);
    sdkDispLabel(SDK_DISP_LINE2, "��忨/�ӿ�", SDK_DISP_LEFT_DEFAULT);
    sdkKbKeyFlush();
    sdkIccOpenRfDev();
    sdkIccOpenIcDev();

    while (1) {
        ret = sdkIccRFQuery(cardtype, temp, 0);

        if (ret >= 0 && temp[0] == 0) {
            psticcparam.eCardType = SDK_ICC_CPU;
            psticcparam.ucCardMode = SDK_ICC_RF;
            psticcparam.ucSlotNo = temp[1];
            ret = SDK_OK;
            break;
        }
        else if (ret == SDK_ESC) {
            sprintf(pasOutInfo, "ȡ���˳�");
            ret = SDK_ESC;
            break;
        }
        else if (ret != SDK_TIME_OUT) {
            sprintf(pasOutInfo, "Ѱ�ǽӿ��쳣�˳�\r�����sdkIccRFQuery");
            ret = SDK_ERR;
            break;
        }
        ret = sdkIccIsInSlot();

        if (ret == SDK_OK) {
            psticcparam.eCardType = SDK_ICC_CPU;
            psticcparam.ucCardMode = SDK_ICC_ICC;
            psticcparam.ucSlotNo = sdkIcGetIcdealCardNo();
            ret = SDK_OK;
            break;
        }

        if (sdkKbGetKey() == SDK_KEY_ESC) {
            sprintf(pasOutInfo, "ȡ���˳�");
            ret = SDK_ESC;
            break;
        }
    }

    if (SDK_OK == ret) {
        if (SDK_OK != sdkIccReset(&psticcparam, temp, &len)) {
            sprintf(pasOutInfo, "��λʧ��\r�����sdkIccReset");
        }
        else {
            memset(&pstApduSend, 0, sizeof(pstApduSend));
            memset(&pstApduResp, 0, sizeof(pstApduResp));

            memcpy(pstApduSend.Command, "\x00\xA4\x04\x00", 4);
            pstApduSend.Lc = 14;
            pstApduSend.Le = 256;
            pstApduSend.EnableCancel = 1;           //ʹ�ܰ�ȡ����

            if (psticcparam.ucCardMode == SDK_ICC_ICC) {
                memcpy(pstApduSend.DataIn, "1PAY.SYS.DDF01", pstApduSend.Lc);
            }
            else {
                memcpy(pstApduSend.DataIn, "2PAY.SYS.DDF01", pstApduSend.Lc);
            }

            if (sdkIccSendAPDU(&psticcparam, &pstApduSend, &pstApduResp) != SDK_OK) {
                sprintf(pasOutInfo, "sdkIccSendAPDUʧ��");
            }
            else {
                strcpy(pasOutInfo, "DATA:");
                sdkBcdToAsc(pasOutInfo + strlen(pasOutInfo), pstApduResp.DataOut,
                            pstApduResp.LenOut);
                sdkBcdToAsc(pasOutInfo + strlen(pasOutInfo), &pstApduResp.SWA, 1);
                sdkBcdToAsc(pasOutInfo + strlen(pasOutInfo), &pstApduResp.SWB, 1);
            }
        }
    }
    sdkIccCloseIcDev();
    sdkIccCloseRfDev();
    return SDK_OK;
}

static s32 Test_sdkIccMemMifarePowerDownManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[128] = {0};
    u8 cttemp[200] = {0};
    u8 cmtemp[200] = {0};
    u8 sntemp[200] = {0};
    s32 ret = 0;
    SDK_ICC_PARAM psticcparam;

    memset(&psticcparam, 0, sizeof(psticcparam));

    psticcparam.eCardType = sdkTestInputU8(pTitle, "������:");
    psticcparam.ucCardMode = sdkTestInputU8(pTitle, "����ģʽ:");
    psticcparam.ucSlotNo = sdkTestInputU8(pTitle, "������:");

    ret = sdkIccRFQuery(SDK_ICC_RFCARD_A | SDK_ICC_RFCARD_B, temp, 3000);
    Verify(sdkU8ToAsc(psticcparam.eCardType, cttemp) >= 0);
    Verify(sdkU8ToAsc(psticcparam.ucCardMode, cmtemp) >= 0);
    Verify(sdkU8ToAsc(psticcparam.ucSlotNo, sntemp) >= 0);
    ret = sdkIccMemMifarePowerDown(&psticcparam);
    sprintf(pasOutInfo, "������:%s \r����ģʽ:%s \r������:%s \r ret :%d \r",
            cttemp, cmtemp, sntemp, ret);
    return SDK_OK;
}

static s32 Test_sdkIccMem4442ReadPwdAndPwdCountManual(char *pasOutInfo, char const *pTitle,
                                                      s32 cardtype, u8 cardmode) {
    u8 temp[128] = {0};
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    s32 len = 0, ret = 0;

    ret = sdkIccMem4442ReadPwdAndPwdCount(temp, &len);
    Verify(sdkBcdToAsc(temp2, temp, len) >= 0);

    if ('7' == temp2[3]) {
        strcpy(temp3, "0 times err, 3 times left");
    }
    else if ('6' == temp2[3]) {
        strcpy(temp3, "1 times err, 2 times left");
    }
    else if ('4' == temp2[3]) {
        strcpy(temp3, "2 times err, 1 times left");
    }
    else if ('0' == temp2[3]) {
        strcpy(temp3, "3 times err, 0 times left");
    }
    sprintf(pasOutInfo, "��������:%s \r���ݳ���:%d \r��������¼:%s\r return:%d", temp2, len, temp3,
            ret);
    return SDK_OK;
}

static s32 Test_sdkIccMifareReadCardNumManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[128] = {0};
    u8 temp2[200] = {0};
    s32 ret = 0;

    ret = sdkIccMifareReadCardNum(temp);
    Verify(sdkBcdToAsc(temp2, temp, ret) >= 0);
    sprintf(pasOutInfo, "��������:%s \r���ݳ���:%d \r", temp2, ret);
    return SDK_OK;
}

static s32 Test_sdkIccMifareVerifyKeyManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[128] = {0};
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    u8 temp5[200] = {0};
    u8 blocktmp[200] = {0};
    u8 keytmp[200] = {0};
    s32 ret = 0, bcdlen = 0, len = 0, templen = 0;

    u8 block = sdkTestInputU8(pTitle, "���:");
    u8 keytype = sdkTestInputU8(pTitle, "��Կ����:");

    bcdlen = sdkTestInputBCD(pTitle, temp, "��Կ:");
//    memcpy(temp,"\xFF\xFF\xFF\xFF\xFF\xFF",6);
    len = sdkTestInputs32(pTitle, "��Կ����:");

    TestLog("mifare", "���:%x\r\n", block);
    TestLog("mifare", "��Կ����:%x\r\n", keytype);
    Verify(sdkU8ToAsc(block, blocktmp) >= 0);
    Verify(sdkU8ToAsc(keytype, keytmp) >= 0);

    if (keytype == 1) {
        ret = sdkIccMifareVerifyKey(block, SDK_RF_KEYA, temp, len, temp3, &templen);
    }
    else if (keytype == 2) {
        ret = sdkIccMifareVerifyKey(block, SDK_RF_KEYB, temp, len, temp3, &templen);
    }
    else {
        ret = sdkIccMifareVerifyKey(block, keytype, temp, len, temp3, &templen);
    }
    Verify(sdkBcdToAsc(temp2, temp3, ret) >= 0);
    Verify(sdkBcdToAsc(temp5, temp, bcdlen) >= 0);
    sprintf(pasOutInfo,
            "���:%s \r ��Կ����:%c \r ��Կ:%s \r ��Կ����:%d \r��������:%s \r���ݳ���:%d \r retrun %d \r",
            blocktmp, keytmp[0], temp5, len, temp2, templen, ret);
    return 0;
}

static s32 Test_sdklccMifareReadBlockManual(char *pasOutInfo, char const *pTitle) {
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    u8 blocktmp[200] = {0};
    s32 ret = 0, templen = 0;

    u8 block = sdkTestInputU8(pTitle, "���:");

    TestLog("mifare", "���:%x\r\n", block);
    //  ret = sdkIccMifareWriteBlock(block,temp3,templen);

    Verify(sdkU8ToAsc(block, blocktmp) >= 0);
    Verify(sdkBcdToAsc(temp2, temp3, templen) >= 0);
    TestLogHex("card", "read card data", temp2, templen * 2);
    sprintf(pasOutInfo, "���:%s \r ��������:%s \r���ݳ���:%d \r retrun %d \r",
            blocktmp, temp2, templen, ret);
    return SDK_OK;
}

static s32 Test_sdklccMifareWriteBlockManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[128] = {0};
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    u8 temp4[200] = {0};
    u8 blocktmp[200] = {0};
    s32 ret = 0, bcdlen = 0, len = 0, templen = 0;

    u8 block = sdkTestInputU8(pTitle, "���:");

    TestLog("mifare", "���:%x\r\n", block);

    bcdlen = sdkTestInputBCD(pTitle, temp, "д������:");
    len = sdkTestInputs32(pTitle, "д�����ݳ���:");
    ret = sdkIccMifareWriteBlock(block, temp, len, temp2, &templen);
    Verify(sdkU8ToAsc(block, blocktmp) >= 0);
    Verify(sdkBcdToAsc(temp3, temp, ret) >= 0);
    Verify(sdkBcdToAsc(temp4, temp2, ret) >= 0);
    sprintf(pasOutInfo,
            "���:%s \r д������:%s \rд�����ݳ���:%d \r��������:%s \r���ݳ���:%d \r retrun %d \r",
            blocktmp, temp3, len, temp4, templen, ret);
    return 0;
}

static s32 Test_sdkIccMifareOperateBlockManual(char *pasOutInfo, char const *pTitle) {
    u8 temp[128] = {0};
    u8 temp2[200] = {0};
    u8 temp3[200] = {0};
    u8 temp4[200] = {0};
    u8 blocktmp[200] = {0};
    u8 opttypetmp[200] = {0};
    s32 ret = 0, bcdlen = 0, templen = 0;
    u8 opttype = sdkTestInputU8(pTitle, "��������:");
    u8 block = sdkTestInputU8(pTitle, "���:");

    TestLog("mifare", "���:%x\r\n", block);
    TestLog("mifare", "��������:%x\r\n", opttype);
    bcdlen = sdkTestInputBCD(pTitle, temp, "д������:");
    ret = sdkIccMifareOperateBlock(opttype, block, temp, temp2, &templen);
    Verify(sdkU8ToAsc(block, blocktmp) >= 0);
    Verify(sdkU8ToAsc(opttype, opttypetmp) >= 0);
    Verify(sdkBcdToAsc(temp3, temp, bcdlen) >= 0);
    Verify(sdkBcdToAsc(temp4, temp2, templen) >= 0);
    sprintf(pasOutInfo,
            "��������:%c\r���:%s \r д������:%s \r��������:%s \r���ݳ���:%d \r retrun %d \r",
            opttypetmp[0], blocktmp, temp3, temp4, templen, ret);
    return SDK_OK;
}

static s32 Test_sdkIccMifareBackUpBlockManual(char *pasOutInfo, char const *pTitle) {
    u8 temp2[200] = {0};
    u8 temp4[200] = {0};
    u8 sblocktmp[200] = {0};
    u8 blocktmp[200] = {0};
    s32 ret = 0, templen = 0;
    u8 sblock = sdkTestInputU8(pTitle, "Դ���:");
    u8 block = sdkTestInputU8(pTitle, "���ݿ��:");

    TestLog("mifare", "s���:%x\r\n", sblock);
    TestLog("mifare", "���:%x\r\n", block);
    ret = sdkIccMifareBackUpBlock(sblock, block, temp2, &templen);
    Verify(sdkU8ToAsc(block, blocktmp) >= 0);
    Verify(sdkU8ToAsc(sblock, sblocktmp) >= 0);
    Verify(sdkBcdToAsc(temp4, temp2, ret) >= 0);
    sprintf(pasOutInfo, "Դ���:%c\r���:%s \r��������:%s \r���ݳ���:%d \r retrun %d \r",
            sblocktmp[0], blocktmp, temp4, templen, ret);
    return SDK_OK;
}

static s32 Test_sdkIccIsInSlotManual(char *pasOutInfo, char const *pTitle, s32 cardtype,
                                     u8 cardmode) {
    s32 ret = 0;

    sdkIccOpenIcDev();
    ret = sdkIccIsInSlot();
    sdkIccCloseIcDev();
    TEST_IS_TRUE_WITH_INFO("sdkIccIsInSlot ����", (sdkIccIsInSlot() == SDK_OK));

    sprintf(pasOutInfo, "retrun %d \r", ret);

    if (ret != SDK_OK) { Test_sdkPrintTestInfo(NULL, pasOutInfo); }
    return SDK_OK;
}

static s32 Test_sdkIccIsCardOutManual(char *pasOutInfo, char const *pTitle, s32 cardtype,
                                      u8 cardmode) {
    s32 ret = 0;
    s32 timer = sdkTestInputs32(pTitle, "��ʱʱ��(����):");

    sdkDispClearScreen();
    sdkDispFillRowRam(SDK_DISP_LINE2, 0, "mem������У����Ժ�..", SDK_DISP_DEFAULT);
    sdkDispBrushScreen();
    sdkIccPowerDown();
    sdkIccOpenIcDev();
    ret = sdkIccIsCardOut(timer);
    sprintf(pasOutInfo, "��ʱʱ��:%d \r retrun %d \r", timer, ret);

    if (ret != SDK_OK) { Test_sdkPrintTestInfo(NULL, pasOutInfo); }
    return SDK_OK;
}

static s32 Test_sdkIccJudgeHave24GModule(char *pasOutInfo, char const *pTitle) {
    if (0 == sdkIccJudgeHave24GModule()) {
        sprintf(pasOutInfo, "û��2.4Gģ��");
    }
    else {
        sprintf(pasOutInfo, "��2.4Gģ��");
    }
    return SDK_OK;
}

static const TEST_MANUAL_MAP MifareManaulMap[] =
        {
                /*=======BEGIN: ���ٲ� 2013.10.23  9:29 modify===========*/
                {"Test_sdkIccMifareTestDemon", Test_sdkIccMifareTestDemon},
                {"Test_sdkIcc2.4GCardTestDemon",
                 Test_sdkIccTest24GCardDemon} //���ٲ� 2013.11.06 10:7
#if 0 /*Modify by ���ٲ� at 2013.11.05  10:6 */
                {"sdkIccRFQuery", Test_sdkIccRFQueryManual},
                {"sdkIccMifareReadCardNum", Test_sdkIccMifareReadCardNumManual},
                {"sdkIccMifareVerifyKey", Test_sdkIccMifareVerifyKeyManual},
                {"sdkIccMifareWriteBlock", Test_sdklccMifareReadBlockManual},
                {"sdkIccMifareWriteBlock", Test_sdklccMifareWriteBlockManual},
                {"sdkIccMifareOperateBlock", Test_sdkIccMifareOperateBlockManual},
                {"sdkIccMifareBackUpBlock", Test_sdkIccMifareBackUpBlockManual},
                {"sdkIccMemMifarePowerDown", Test_sdkIccMemMifarePowerDownManual},
                {"sdkIccJudgeHave24GModule", Test_sdkIccJudgeHave24GModule}
#endif /* if 0 */
                /*====================== END======================== */
        };

TEST_MAP_INFO GetMifareManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(MifareManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = MifareManaulMap;
    return ret;
}

static const TEST_MANUAL_MAP MemoryManaulMap[] =
        {
                /*=======BEGIN: ���ٲ� 2013.10.23  9:27 modify===========*/
                {"Test_sdkIccMem4442TestDemon", Test_sdkIccMem4442TestDemon},
                {"Test_sdkIccMem4428TestDemon", Test_sdkIccMem4428TestDemon},

                //{"sdkIccReset", Test_sdkIccResetManual},
                {"sdkIccSendAPDU",              Test_sdkIccSendAPDUManual},
                //{"sdkIccSendDataManual", Test_sdkIccSendDataManual},
                //{"sdkIccMemCheck", Test_sdkIccMemCheckManual},
                //{"sdkIccMemCheckPwd", Test_sdkIccMemCheckPwdManual},
                //{"sdkIccMemReadData", Test_sdkIccMemReadDataManual},
                //{"sdkIccMemWriteData", Test_sdkIccMemWriteDataManual},
                //{"sdkIccMemModifyPwd", Test_sdkIccMemModifyPwdManual},
                //{"sdkIccMem4442ReadPwdAndPwdCount", Test_sdkIccMem4442ReadPwdAndPwdCountManual},
                //{"sdkIccIsIccCardIn", Test_sdkIccIsIccCardInManual},
                //{"sdkIccIsInSlot", Test_sdkIccIsInSlotManual},
                //{"sdkIccIsCardOut", Test_sdkIccIsCardOutManual},
                //{"sdkIccMemPowerOn", Test_sdkIccMemPowerOnManual},
                /*====================== END======================== */
        };


TEST_MAP_INFO GetMemoryManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(MemoryManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = MemoryManaulMap;
    return ret;
}

