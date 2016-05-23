#include "sdkGlobal.h"
#include "sdkExGlobal.h"
#include "sdkTest.h"

#define BUF_LEN 128
#define BUF_LEN_256 256

//#define XXX_YYY

#ifdef XXX_YYY
static int test_getchar(char *src, char *dst, int dstlen)
{
    char rslt[BUF_LEN_256];
    char tmp[128];
    int i=0;
    u32 len=0;
    u32 j;
    int buf_len;
    
    if(src == NULL || dst == NULL)
        return SDK_ERR;
    
    Trace("bt_test", "getchar begin: i=%d\n", i);
    //Э��ͷ��3�ֽ�
    while(i<3)
    {
        #if 0
        if(src[i++] != 0x04)
        {
            sprintf(rslt, "Э��ͷ���ԣ�%x%x%x\n", src[0], src[1], src[2]);
            goto end;
        }
        #else
        i++;
        #endif
    }

    //Len, 2 byte
    Trace("bt_test", "getchar len: i=%d\n", i);
    Trace("bt_test", "getchar src[%d]=%x, src[%d]=%x\n", i, src[i], i+1, src[i+1]);
    Trace("bt_test", "getchar %x\n", (src[i]<<8) | src[i+1]);    
    len = (src[i]*256) + src[i+1];
    Trace("bt_test", "getchar len=%d\n", len);
    i +=2;
    
    //SN , 1 byte
    i++;
    //MODE, 1 byte
//    if(src[i] != 0x01 || src[i] != 0x00)
//        return SDK_ERR;
    i++;
    
    Trace("bt_test", "getchar data: i=%d\n", i);
    //data
    if (len-2 > dstlen)
        buf_len = dstlen;
    else
        buf_len = len -2;
    
    for(j=0;j<buf_len;j++)
    {
        dst[j]=src[i++];
    }
    Trace("bt_test", "getchar end: dst=%s\n", dst);
    //END, 1 byte
    return SDK_OK;
    //CRC, 2 byte
end:
    sdkDispMsgBox("���ݷ���", rslt, 0, SDK_KEY_MASK_ALL);
    return SDK_ERR;
}
#endif

static void inline get_error_str(char *str, int ret, char *funname) {
    switch (ret) {
        case SDK_EDATA:
            sprintf(str, "������������ݴ���, ret=%d\n", ret);
            break;
        case SDK_EIO:
            sprintf(str, "�豸δ�򿪻������, ret=%d\n", ret);
            break;
        case SDK_EBUSY:
            sprintf(str, "�豸æ, ret=%d\n", ret);
            break;
        case SDK_PARA_ERR:
            sprintf(str, "��������, ret=%d\n", ret);
            break;
        case SDK_TIME_OUT:
            sprintf(str, "��ʱ�˳�, ret=%d\n", ret);
            break;
        default:
            sprintf(str, "%s����ʧ��, ret=%d\n", funname, ret);
            break;
    }
}


static int sdkTest_BtTestManual2(char *pasOutInfo, char const *pTitle) {
    char *temp = NULL, *temp1 = NULL;
    char rslt[256];
    int ret, i;
    int key;
    char *szAll = "1.������Ϣ\r2.����״̬\r3.������豸��Ϣ\r4.ƥ���豸��Ϣ\r5.ɾ��ƥ���豸ϵ��Ϣ";
    char connectInfo[64] = {0};

    ret = sdkBtGetStatus();
    memset(connectInfo, 0, sizeof(connectInfo));
    if (ret == SDK_BT_STATUS_WAITING || ret == SDK_BT_STATUS_WAITING)
        sprintf(connectInfo, "�豸�������޷�ִ��:");

    temp = (char *) sdkGetMem(BUF_LEN);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp1 = (char *) sdkGetMem(BUF_LEN);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }

    memset(temp, 0, BUF_LEN);
    memset(temp1, 0, BUF_LEN);

    for (; ;) {
        memset(temp, 0, BUF_LEN);
        key = sdkDispMsgBox("��ѡ�����ģ��", szAll, 0, SDK_KEY_MASK_ALL);

        switch (key) {
            case SDK_KEY_1: {
                memset(temp, 0, BUF_LEN);
                memset(temp1, 0, BUF_LEN);
                memset(rslt, 0, sizeof(rslt));
                ret = sdkBtGetVersion(temp, BUF_LEN);
                if (ret == SDK_OK) {
                    ret = sdkBtGetMVersion(temp1, BUF_LEN);
                    if (ret == SDK_OK) {
                        sprintf(rslt, "����汾=%s\r�̼��汾=%s\n", temp, temp1);
                    }
                    else {
                        get_error_str(rslt, ret, "sdkBtGetMVersion");
                    }

                    memset(temp, 0, BUF_LEN);
                    memset(temp1, 0, BUF_LEN);

                    ret = sdkBtGetName(temp, BUF_LEN);
                    if (ret == SDK_OK) {
                        sprintf(temp1, "Name=%s, ", temp);
                        strcat(rslt, temp1);

                        memset(temp, 0, BUF_LEN);
                        memset(temp1, 0, BUF_LEN);

                        ret = sdkBtGetPin(temp, BUF_LEN);
                        if (ret == SDK_OK) {
                            sprintf(temp1, "Pin=%s\r", temp);
                            strcat(rslt, temp1);

                            memset(temp, 0, BUF_LEN);
                            memset(temp1, 0, BUF_LEN);

                            ret = sdkBtGetMacAddr(temp, BUF_LEN);
                            if (ret == SDK_OK) {
                                sprintf(temp1, "MAC:  %2x:%2x:%2x:%2x:%2x:%2x\r", temp[0], temp[1],
                                        temp[2], temp[3], temp[4], temp[5]);
                            }
                            else {
                                sprintf(temp1, "%ssdkBtGetMacAddr\rfailed, ret=%d\r", connectInfo,
                                        ret);
                            }
                            strcat(rslt, temp1);
                        }
                        else {
                            memset(temp1, 0, BUF_LEN);
                            sprintf(temp1, "%ssdkBtGetPin\rfailed, ret=%d\r", connectInfo, ret);
                            strcat(rslt, temp1);
                        }
                    }
                    else {
                        sprintf(temp1, "%ssdkBtGetName\rfailed=%d\r", connectInfo, ret);
                        strcat(rslt, temp1);
                    }
                }
                else {
                    sprintf(rslt, "sdkBtGetVersion failed=%d\r", ret);
                }

                sdkDispMsgBox("������Ϣ", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_2: {
//            memset(temp, 0, BUF_LEN);
//            memset(temp1, 0, BUF_LEN);
                memset(rslt, 0, sizeof(rslt));

                ret = sdkBtGetStatus();
                if (ret == SDK_BT_STATUS_IDLE)
                    sprintf(rslt, "�豸δ����");
                else if (ret == SDK_BT_STATUS_WAITING || ret == SDK_BT_STATUS_WAITING)
                    sprintf(rslt, "�豸��������");
                else
                    sprintf(rslt, "��ȡ�豸״̬ʧ��");

                sdkDispMsgBox("����״̬", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_3: {
                SDK_BT_REMOTE_INFO stRemote;
                unsigned char *p;

                memset(&stRemote, 0, sizeof(SDK_BT_REMOTE_INFO));
                memset(rslt, 0, sizeof(rslt));
                ret = sdkBtGetRemoteInfo(&stRemote);
                if (ret == SDK_OK) {
                    p = stRemote.m_addr;
                    sprintf(rslt, "Զ���豸��Ϣrssi=%d,\r addr=%2x:%2x:%2x:%2x:%2x:%2x\n",
                            stRemote.m_rssi, *p,
                            *(p + 1), *(p + 2), *(p + 3), *(p + 4), *(p + 5));
                }
                else {
                    sprintf(rslt, "%s��ȡԶ���豸��Ϣ\rʧ��=%d\n", connectInfo, ret);
                }
                sdkDispMsgBox("Զ���豸��Ϣ", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_4: {
                SDK_BT_PAIR_INFO stPairInfo;
                unsigned char *p;

                memset(&stPairInfo, 0, sizeof(SDK_BT_PAIR_INFO));
                memset(rslt, 0, sizeof(rslt));
                memset(temp, 0, BUF_LEN);

                ret = sdkBtGetPairDevInfo(&stPairInfo);
                if (ret == SDK_OK) {
                    Trace("bt_test", "PairInfo num=%d\n", stPairInfo.m_num);
#if 1
                    sprintf(rslt, "���%d���豸��Ϣ:\r", stPairInfo.m_num);
                    for (i = 0; i < stPairInfo.m_num; i++) {
                        p = stPairInfo.m_addr[i];
                        sprintf(temp, "%d:%s\rdev:%2X:%2X:%2X:%2X:%2X:%2X\r", i,
                                stPairInfo.m_name[i], p[0], p[1], p[2], p[3], p[4], p[5]);
                        strcat(rslt, temp);
                    }
#else
                    sprintf(rslt, "���%d���豸:\raddr:%2X:%2X:%2X:%2X:%2X:%2X\r", stPairInfo.m_num, stPairInfo.m_addr[0][0],
                        stPairInfo.m_addr[0][1], stPairInfo.m_addr[0][2], stPairInfo.m_addr[0][3], stPairInfo.m_addr[0][4],
                        stPairInfo.m_addr[0][5]);
#endif
                }
                else {
                    sprintf(rslt, "%ssdkBtGetPairDevInfo/rfailed=%d\r", connectInfo, ret);
                }

                Trace("bt_test", rslt);
                sdkDispMsgBox("�����Ϣ", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_5: {
                u8 iidx;
                SDK_BT_PAIR_INFO stPairInfo;
                memset(&stPairInfo, 0, sizeof(SDK_BT_PAIR_INFO));
                memset(temp, 0, BUF_LEN);
                memset(rslt, 0, sizeof(rslt));

                sdkBtGetPairDevInfo(&stPairInfo);
                sprintf(temp, "����%d���豸����������Ҫɾ�����豸��:", stPairInfo.m_num);
                iidx = sdkTestInputU8(pTitle, temp);
                Trace("zjp", "sdkBtDelPairDevInfo = %d\r\n", iidx);
                ret = sdkBtDelPairDevInfo(iidx);
                if (ret == SDK_OK)
                    sprintf(rslt, "��ɾ��%d�����Ϣ\n", iidx);
                else
                    sprintf(rslt, "%sɾ��%d�����Ϣ\rʧ��=%d\r", connectInfo, iidx, ret);

                sdkDispMsgBox("ɾ�������Ϣ", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_ESC:
            case SDK_KEY_CLEAR:
                goto end;
            default:
                break;
        }
    }

    end:
    if (temp) sdkFreeMem(temp);
    if (temp1) sdkFreeMem(temp1);

    return SDK_OK;
}

static int sdkTest_BtTestManual(char *pasOutInfo, char const *pTitle) {
    static int iopen = 0;
    static int ivisable = 0;
    static char szMac[6];
    s32 ret;
//    u32 timerid = 0;
    char *temp = NULL, *temp1 = NULL;
    char rslt[256];
//    bool bwrite=false;
//    int nCount=0;
    int key, key2;


    char *szopen[] = {"1.�������豸", "1.�ر������豸"};
    char *szvisable[] = {"3.�����豸�ɼ�", "3.�����豸���ɼ�"};
    char *szAll = "%s\r2.�����������ƺ�����\r%s\r4.��������\r5.��������\r6.�Ͽ�����\r7.����1023���ַ�\r0.����������Ϣ";
    temp = (char *) sdkGetMem(BUF_LEN);
    if (temp == NULL) {
        Assert(0);
        return -1;
    }
    temp1 = (char *) sdkGetMem(BUF_LEN);
    if (temp1 == NULL) {
        Assert(0);
        sdkFreeMem(temp);
        return -1;
    }
    memset(temp, 0, BUF_LEN);
    memset(temp1, 0, BUF_LEN);

    for (; ;) {
        memset(temp, 0, BUF_LEN);
        sprintf(temp, szAll, szopen[iopen], szvisable[ivisable]);
        key = sdkDispMsgBox("��ѡ�����ģ��", temp, 0, SDK_KEY_MASK_ALL);

        switch (key) {
            case SDK_KEY_1: {
                memset(rslt, 0, sizeof(rslt));
                if (iopen == 0) {
                    ret = sdkBtOpen();
                    if (ret != SDK_OK)
                        sprintf(rslt, "sdkBtOpen failed=%d\n", ret);
                    else {
                        iopen = 1;
                        sprintf(rslt, "�����豸�Ѵ�\n");

                        memset(szMac, 0, sizeof(szMac));
                        sdkBtGetMacAddr(szMac, sizeof(szMac));
                    }
                }
                else {
                    ret = sdkBtClose();
                    if (ret == SDK_OK) {
                        iopen = 0;
                        if (ivisable == 1)
                            ivisable = 0;
                        sprintf(rslt, "�����豸�ѹر�\n");
                    }
                    else {
                        sprintf(rslt, "sdkBtClose failed=%d\n", ret);
                    }
                }

                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_2: {
                memset(temp, 0, BUF_LEN);
                memset(rslt, 0, sizeof(rslt));

                sdkTestPresetInputASC(temp, pTitle, "�豸����:", "bt123", 2, 64);
                ret = sdkBtSetName(temp);
                if (ret == SDK_OK) {
                    memset(temp, 0, BUF_LEN);
                    sdkTestPresetInputASC(temp, pTitle, "����PIN:", "123456", 6, 6);
                    ret = sdkBtSetPin(temp);
                    if (ret == SDK_OK) {
                        sprintf(rslt, "�豸��Ϣ���óɹ�\n");
                    }
                    else {
                        get_error_str(rslt, ret, "sdkBtSetPin");
                    }
                }
                else {
                    sprintf(rslt, "sdkBtSetName failed=%d\n", ret);
                }
                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_3: {
                memset(rslt, 0, sizeof(rslt));
                if (ivisable == 0) {
                    ret = sdkBtSetDiscoverable(true);
                    if (ret == SDK_OK) {
                        ivisable = 1;
                        sprintf(rslt, "�����������豸�ɼ�");
                    }
                    else {
                        get_error_str(rslt, ret, "sdkBtSetDiscoverable");
                    }
                }
                else {
                    ret = sdkBtSetDiscoverable(false);
                    if (ret == SDK_OK) {
                        ivisable = 0;
                        sprintf(rslt, "�����������豸���ɼ�");
                    }
                    else {
                        get_error_str(rslt, ret, "sdkBtSetDiscoverable");
                    }

                }
                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_4: {
                memset(rslt, 0, sizeof(rslt));

                for (; ;) {
                    ret = sdkBtGetStatus();
                    switch (ret) {
                        case SDK_BT_STATUS_IDLE:
                            sprintf(rslt, "�豸��δ����", temp);
                            break;
                        case SDK_BT_STATUS_WORKING:
                        case SDK_BT_STATUS_WAITING:
                            sprintf(rslt, "�ȴ���������", temp);
                            break;
                        case SDK_ERR:
                        case SDK_EIO: {
                            memset(rslt, 0, sizeof(rslt));
                            sprintf(rslt, "sdkBtGetStatus failed=%d\n", ret);
                            strcpy(pasOutInfo, rslt);
                            goto end;
                        }
                        default:
                            break;
                    }

                    sdkDispClearScreen();
                    sdkDispAt(0, 0, rslt);

                    memset(temp1, 0, BUF_LEN);
                    memset(rslt, 0, sizeof(rslt));
                    ret = sdkBtRead(temp1, BUF_LEN, 1000);
                    if (ret < 0) {
                        get_error_str(rslt, ret, "sdkBtRead");
                    }
                    else if (ret > 0) {
                        TraceHex("bt_test", "receive data", temp1, ret);
                        sprintf(rslt, "��������len=%d, %s\r\n", ret, temp1);
                        sdkBtWrite(temp1, ret, 0);
                    }

                    if (strlen(rslt) > 0) {
                        sdkDispClearScreen();
                        sdkDispLabel(SDK_DISP_LINE2, rslt, SDK_DISP_LEFT_DEFAULT);
                    }
                    key2 = sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
                    if (key2 == SDK_KEY_CLEAR || key2 == SDK_KEY_ESC) {
                        break;
                    }
                }
                break;
            }
            case SDK_KEY_5: {
                memset(rslt, 0, sizeof(rslt));
                memset(temp1, 0, BUF_LEN);

                sdkTestPresetInputASC(temp1, pTitle, "�����뷢�͵�����", "hello", 1, 127);
                ret = sdkBtWrite(temp1, strlen(temp1), 0);
                if (ret > 0) {
                    sdkDispClearScreen();
                    sprintf(rslt, "��������=%s, len=%d\n", temp1, strlen(temp1));
                }
                else {
                    get_error_str(rslt, ret, "sdkBtWrite");
                }

                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                break;
            }
            case SDK_KEY_6: {
                SDK_BT_REMOTE_INFO stRemote;
                memset(rslt, 0, sizeof(rslt));
                memset(&stRemote, 0, sizeof(SDK_BT_REMOTE_INFO));
#if 0
                ret = sdkBtGetRemoteInfo(&stRemote);
                if (ret == SDK_OK)
                {
                    ret = sdkBtDisconnect(stRemote.m_addr);
                    if(ret == SDK_OK)
                    {
                        sprintf(rslt, "�ѶϿ���%s������\n", stRemote.m_addr);
                    }
                    else
                    {
                        sprintf(rslt, "����sdkBtDisconnect failed, ret=%s\n", ret);
                    }
                }
                else
                {
                    sprintf(rslt, "����sdkBtGetRemoteInfoʧ�ܣ����ܻ�ȡ�����豸��MAC��ַ\n");
                }
#else
                ret = sdkBtDisconnect(szMac);
                if (ret == SDK_OK) {
                    sprintf(rslt, "�ѶϿ�����\n");
                    if (ivisable == 1)
                        ivisable = 0;
                }
                else {
                    get_error_str(rslt, ret, "sdkBtDisconnect");
                }
#endif
                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);

                break;
            }
            case SDK_KEY_7: {
                int ret;
                char *tmp_big = sdkGetMem(1024);
                if (tmp_big == NULL)
                    break;

                memset(tmp_big, 0x61, 1023);
                tmp_big[1023] = '\0';
                ret = sdkBtWrite(tmp_big, 1023, 0);
                if (ret < 0) {
                    switch (ret) {
                        case SDK_ERR:
                            sprintf(rslt, "sdkBtWrite ����ʧ��\n");
                            break;
                        case SDK_EIO:
                            sprintf(rslt, "sdkBtWrite ����: �豸δ��\n");
                            break;
                        case SDK_PARA_ERR:
                            sprintf(rslt, "sdkBtWrite ����: ��������\n");
                            break;
                    }
                }
                else {
                    sprintf(rslt, "sdkBtWrite ����: ���ͳɹ���ret=%d\n", ret);
                }

                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                sdkFreeMem(tmp_big);
                break;
            }
            case SDK_KEY_8: {
                int ret;
                char *tmp_big = sdkGetMem(3072);
                if (tmp_big == NULL)
                    break;

                memset(tmp_big, 0, 3072);
                ret = sdkBtRead(tmp_big, 3071, 30000);
                if (ret < 0) {
                    switch (ret) {
                        case SDK_ERR:
                            sprintf(rslt, "sdkBtRead ����ʧ��\n");
                            break;
                        case SDK_EIO:
                            sprintf(rslt, "sdkBtRead ����: �豸δ��\n");
                            break;
                        case SDK_PARA_ERR:
                            sprintf(rslt, "sdkBtRead ����: ��������\n");
                            break;
                        case SDK_TIME_OUT:
                            sprintf(rslt, "sdkBtRead ����: ��ʱ����\n");
                            break;
                    }
                }
                else {
                    sprintf(rslt, "sdkBtRead ����: ���ճɹ���ret=%d\n", ret);
                }

                sdkDispMsgBox("��������", rslt, 0, SDK_KEY_MASK_ALL);
                sdkFreeMem(tmp_big);
                break;
            }
            case SDK_KEY_9: {
                int ret;
                sdkDispMsgBox("��������", "�����������͹���", 0, SDK_KEY_MASK_ALL);
                sdkSysEnterIdle(1, 1);

                memset(rslt, 0, sizeof(rslt));
                for (; ;) {
                    ret = sdkBtGetStatus();
                    switch (ret) {
                        case SDK_BT_STATUS_IDLE:
                            sprintf(rslt, "�豸��δ����", temp);
                            break;
                        case SDK_BT_STATUS_WORKING:
                        case SDK_BT_STATUS_WAITING:
                            sprintf(rslt, "�ȴ���������", temp);
                            break;
                        case SDK_ERR:
                        case SDK_EIO: {
                            memset(rslt, 0, sizeof(rslt));
                            sprintf(rslt, "sdkBtGetStatus failed=%d\n", ret);
                            strcpy(pasOutInfo, rslt);
                            goto end;
                        }
                        default:
                            break;
                    }

                    sdkDispClearScreen();
                    sdkDispAt(0, 0, rslt);

                    memset(temp1, 0, BUF_LEN);
                    memset(rslt, 0, sizeof(rslt));
                    ret = sdkBtRead(temp1, BUF_LEN, 1000);
                    if (ret < 0) {
                        get_error_str(rslt, ret, "sdkBtRead");
                    }
                    else if (ret > 0) {
                        TraceHex("bt_test", "receive data", temp1, ret);
                        sprintf(rslt, "��������len=%d, %s\r\n", ret, temp1);
                        sdkBtWrite(temp1, ret, 0);
                    }

                    if (strlen(rslt) > 0) {
                        sdkDispClearScreen();
                        sdkDispLabel(SDK_DISP_LINE2, rslt, SDK_DISP_LEFT_DEFAULT);
                    }
                    key2 = sdkKbWaitKey(SDK_KEY_MASK_ALL, 1000);
                    if (key2 == SDK_KEY_CLEAR || key2 == SDK_KEY_ESC) {
                        break;
                    }
                }
                break;
            }
            case SDK_KEY_0:
                sdkTest_BtTestManual2(pasOutInfo, pTitle);
                break;
            case SDK_KEY_ESC:
            case SDK_KEY_CLEAR:
                goto end;
            default:
                break;
        }
    }

    end:
    memset(temp, 0, BUF_LEN);
//    sdkBtGetMacAddr(temp, BUF_LEN);
//    sdkBtDisconnect(temp);
    if (iopen) {
        sdkBtClose();
    }
    iopen = 0;
    ivisable = 0;

    if (temp) sdkFreeMem(temp);
    if (temp1) sdkFreeMem(temp1);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	Input: None 
** Returned value:	
** Created By:		fusuipu  2013.09.01
** Remarks: 		
*****************************************************************************/
static const TEST_MANUAL_MAP BtManaulMap[] =
        {
                {"BlueThooth", sdkTest_BtTestManual},
                //{"AudioCommTest", sdkTest_AudoTestManual},
                //{"UsbTest", sdkTest_UsbTestManual},

        };

static TEST_MAP_INFO GetBtManualTestInfo() {
    TEST_MAP_INFO ret;

    ret.size = sizeof(BtManaulMap) / sizeof(TEST_MANUAL_MAP);
    ret.pMap = BtManaulMap;
    return ret;
}


void Test_sdkBtManual() {
    TEST_MAP_INFO info = GetBtManualTestInfo();

    Test_sdkSelectCase("��������ѡ��", &info);
}

