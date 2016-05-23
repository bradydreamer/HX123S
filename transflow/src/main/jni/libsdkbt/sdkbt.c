#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "sdkExGlobal.h"


typedef struct _strBtPairList {
    u8 m_num;           //������豸����
    u8 m_addr[4][6];    //�豸��ַ��6�ֽ�HEX���յĵ�ַΪȫ0
    u8 m_name[4][64];   //�豸����
} strBtPairList;

typedef struct _strBtRemote {
    u8 m_rssi;      //�����豸�źų�ǿֵ
    u8 m_addr[6];   //�豸��ַ��6�ֽ�HEX
    u8 m_name[64];  //�豸����
} strBtRemote;

#ifndef  DEBUG_BT
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

/************************************** ***************************************
** Descriptions:	�������豸
** Parameters:          Input: None
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸æ              SDK_EBUSY
** Created By:		tjb  2014.05.26
** Remarks:             �豸�򿪺�Ĭ��Ϊ���ɼ�״̬����Ҫ����sdkBtSetDiscoverable����Ϊ�ɼ�
*****************************************************************************/
s32 sdkBtOpen(void) {
    //return sdk_dev_bt_open();
    s32 ret = ddi_bt_open();

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_ERR) {
        return SDK_ERR;
    }
    else {
        return SDK_EBUSY;
    }
}

/*****************************************************************************
** Descriptions:	�ر������豸
** Parameters:          Input: None
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
** Created By:		tjb  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkBtClose() {
    //return sdk_dev_bt_close();
    s32 ret = ddi_bt_close();

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	�Ͽ���ǰ������
** Parameters:          const u8 *pszMac   �����豸MAC��ַ
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkBtDisconnect(const u8 *pszMac) {
    //return sdk_dev_bt_disconnect(pszMac);
    s32 ret;

    if (pszMac == NULL) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_disconnect(pszMac);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	������������
** Parameters:          const u8 *pszData     ����д�������
                    u32 nBufLen             ����д�����ݵĳ���
                    u32 nTimeout            �������ݳ�ʱʱ�䣬��λ���롣��Ϊ0����
                                            ��ʱȡ��
** Returned value:	�ɹ�����ʵ�ʷ��͵����ݴ�С
                    ʧ��                SDK_ERR
                    ��������            SDK_PARAERR
                    �豸δ�򿪻��д��  SDK_EIO
                    ��ʱ����            SDK_TIME_OUT
** Created By:		tjb  2014.05.26
** Remarks:             ����������ʽ�ģ�ֱ�����������ݷ������
*****************************************************************************/
int sdkBtWrite(const u8 *pszData, u32 nBufLen, u32 nTimeout) {
    s8 *p;
    s32 ret;
    s32 all_send = nBufLen;
    s32 had_send = 0;
    s32 now_send;
    s32 timerid = sdkTimerGetId();

    if (pszData == NULL || nBufLen == 0) {
        Trace("bt_test", "sdkBtWrite: ���������������\n");
        return SDK_PARA_ERR;
    }

    //while (all_send > 0)
    for (; ;) {
        ret = sdkBtGetStatus();

        if (ret == SDK_ERR) {
            Trace("bt_test", "sdkBtWrite: ��ȡ����״̬����\n");
            return SDK_ERR;
        }
        else if (ret == SDK_BT_STATUS_IDLE || ret == SDK_EIO) {
            Trace("bt_test", "sdkBtWrite: ����δ��\n");
            return SDK_EIO;
        }
        else if (ret == SDK_BT_STATUS_WORKING) {
            if (nTimeout != 0 && sdkTimerIsEnd(timerid, nTimeout)) {
                Trace("bt_test", "sdkBtWrite: ��ʱ����\n");
                return SDK_TIME_OUT;
            }
            continue;
        }

        if (all_send > 512) {
            now_send = 512;
        }
        else {
            now_send = all_send;
        }
        p = (s8 * )(pszData + had_send);

        //ret = bt_write(p, now_send);
        ret = ddi_bt_write(p, now_send);

        if (ret < 0)        //error
        {
            Trace("bt_test", "sdkBtWrite: ��������ʧ��\n");
            return ret;
        }
        else if (ret < now_send) {
            Trace("bt_test", "sdkBtWrite: ��������С��Ԥ��\n");
            return SDK_ERR;
        }
        else {
            had_send += ret;
            all_send -= ret;

            if (all_send <= 0) {
                break;
            }
            timerid = sdkTimerGetId();
        }
    }

    Trace("bt_test", "sdkBtWrite: �������ݳɹ�����������%d�ֽ�\n", had_send);
    return had_send;
//    return sdk_dev_bt_write(pszData, nBufLen);
}

/*****************************************************************************
** Descriptions:	������ȡ����
** Parameters:          u8 *pszOut        ������ȡ���ݵĳ���
                    u32 nBufLen         ��ȡ������
                    u32 nTimeout        �������ݳ�ʱʱ�䣬��λ���롣��Ϊ0����
                                        ��ʱȡ��
** Returned value:	�ɹ�����ʵ�ʶ��������ݴ�С
                    ʧ��                SDK_ERR
                    ��������            SDK_PARAERR
                    �豸δ�򿪻��д��  SDK_EIO
                    ��ʱ����            SDK_TIME_OUT
** Created By:		tjb  2014.05.26
** Remarks:             ������Ҫ��ȡ��Ԥ��ָ����uiExOutLen���ȵ����ݣ�����Ҫ��
                    uiTimeout��Ϊ0��������һֱ��ȡ��ֱ��������
                    ��û��Ԥ�����ݳ��ȣ����������С��uiTimeoutֵ�����ٺ���
                    ��������
*****************************************************************************/
int sdkBtRead(u8 *pszOut, u32 nBufLen, u32 nTimeout) {
    s32 ret, rslt;
    s32 all_recv = nBufLen;
    s32 had_recv = 0;
    s32 now_recv;
    s32 timerid = sdkTimerGetId();
    //char *temp;
    s32 buf_len;


    if (pszOut == NULL || nBufLen == 0) {
        Trace("bt_test", "sdkBtRead: ���������������\n");
        return SDK_PARA_ERR;
    }

    if (nBufLen > MAX_BUFFER_LENGTH) {
        buf_len = MAX_BUFFER_LENGTH;
    }
    else {
        buf_len = nBufLen;
    }

    for (; ;) {
        ret = sdkBtGetStatus();

        if (ret == SDK_ERR) {
            Trace("bt_test", "sdkBtRead: ��ȡ����״̬����\n");
            rslt = SDK_ERR;
            memset(pszOut, 0, nBufLen);
            goto end;
        }
        else if (ret == SDK_BT_STATUS_IDLE || ret == SDK_EIO) {
            Trace("bt_test", "sdkBtRead: ����δ�򿪻�δ����\n");
            rslt = SDK_EIO;
            memset(pszOut, 0, nBufLen);
            goto end;
        }
        else if (ret == SDK_BT_STATUS_WORKING) {
            if (nTimeout != 0 && sdkTimerIsEnd(timerid, nTimeout)) {
                Trace("bt_test", "sdkBtRead: �豸æ��ʱ����\n");

                if (had_recv > 0)            /* ���Ƕ��������ݣ��򲻷��س�ʱ���� */
                {
                    rslt = had_recv;
                }
                else {
                    rslt = SDK_TIME_OUT;
                }
                goto end;
            }
            continue;
        }

        //memset(temp, 0, buf_len);
        if (all_recv >= buf_len) //�ְ���
        {
            ret = ddi_bt_read(pszOut + had_recv, buf_len);
            Trace("bt_test", "ddi_bt_read:ret = %d\r\n", ret);
        }
        else //��ʣ�ಿ��
        {
            ret = ddi_bt_read(pszOut + had_recv, all_recv);
            Trace("bt_test", "ddi_bt_read:ret = %d\r\n", ret);
        }

        if (ret < 0)            //error
        {
            Trace("bt_test", "sdkBtRead: ��������ʧ��\n");
            rslt = ret;
            goto end;
        }
        else if (ret == 0) {
            if (nTimeout != 0 && sdkTimerIsEnd(timerid, nTimeout)) {
                Trace("bt_test", "sdkBtRead: �����ݶ�ȡ��ʱ����\n");

                if (had_recv > 0)                /* ���Ƕ��������ݣ��򲻷��س�ʱ���� */
                {
                    rslt = had_recv;
                }
                else {
                    rslt = SDK_TIME_OUT;
                }
                goto end;
            }
        }
        else {
            if (ret < all_recv) {
                now_recv = ret;
            }
            else {
                Trace("bt_test", "sdkBtRead: ��ʧ����%d\n", ret - all_recv);
                now_recv = all_recv;
            }
            //memcpy(pszOut+had_recv, temp, now_recv);
            all_recv -= now_recv;
            had_recv += now_recv;

            if (all_recv <= 0)                /* ���յ������ݸ�Ԥ����ͬ */
            {
                rslt = had_recv;
                break;
            }
            //���¼�ʱ
            timerid = sdkTimerGetId();
        }
    }

    //Trace("bt_test", "sdkBtRead: �������ݳɹ�����������%d�ֽ�\n", had_recv);
    end:
    return rslt;
}

/*****************************************************************************
** Descriptions:	��ȡ������״̬
** Parameters:          Input: None
** Returned value:	�ɹ�����������״̬:
                        SDK_BT_STATUS_IDLE      1   //Idle ģʽ(δ����)
                        SDK_BT_STATUS_WORKING   2   //����������
                        SDK_BT_STATUS_WAITING   3   //������
                    ʧ��                SDK_ERR
                    �豸δ��          SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkBtGetStatus() {
    //return sdk_dev_bt_get_status();
    s32 ret = ddi_bt_get_status();

    if (ret > 0) {
        return ret;
    }
    else if (ret == DDI_EIO) {
        Trace("bt_test", "ddi_bt_get_status:ret = %d\r\n", ret);
        return SDK_EIO;
    }
    else {
        Trace("bt_test", "ddi_bt_get_status:ret = %d\r\n", ret);
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ�������Ƴ���汾��Ϣ(��'\0'������ASCII�ַ�������Ч�ַ����ȹ̶�Ϊ17Bytes)
** Parameters:          char *pszVer        ����汾��Ϣ���ַ���
                    int nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             ��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
s32 sdkBtGetVersion(u8 *pszVer, u32 nLen) {
    //return sdk_dev_bt_get_version(pszVer, nLen);
    s32 ret;

    if (pszVer == NULL || nLen < 17) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_VER, 0, (unsigned int) pszVer);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ����ģ��̼��汾(��'\0'������ASCII�ַ�������Ч�ַ����ȹ̶�Ϊ17Bytes)
** Parameters:          char *pszMVer       ����汾��Ϣ���ַ���
                    int nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             ��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
s32 sdkBtGetMVersion(u8 *pszMVer, u32 nLen) {
    //return sdk_dev_bt_get_mversion(pszMVer, nLen);
    s32 ret;

    if (pszMVer == NULL || nLen < 17) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_MVER, 0, (unsigned int) pszMVer);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ����MAC��ַ, ���ȹ̶�Ϊ6�ֽ�
** Parameters:          u8 *pszAddr       �����ַ���ַ���
                    u32 nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             ��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
s32 sdkBtGetMacAddr(u8 *pszAddr, u32 nLen) {
    //return sdk_dev_bt_get_mac_addr(pszAddr, nLen);
    s32 ret;

    if (pszAddr == NULL || nLen < 6) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_ADDR, 0, (unsigned int) pszAddr);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ�������������(PIN)����'\0'������ASCII�ַ�����4��6λ����
** Parameters:          char *pszPin        ����PIN���ַ���
                    int nLen            �ַ������ȣ�����С��6
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             PINֵΪ4��6λ�����ַ���(G2\K200P����Ϊ6λ)
*****************************************************************************/
s32 sdkBtGetPin(u8 *pszPin, u32 nLen) {
    //return sdk_dev_bt_get_pin(pszPin, nLen);
    s32 ret;

    if (pszPin == NULL || nLen < 6) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_RPIN, 0, (unsigned int) pszPin);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	�����������������(PIN)
** Parameters:          u8 *pszPin
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             PINֵΪ4��6λ�����ַ���(G2\K200P����Ϊ6λ)
*****************************************************************************/
s32 sdkBtSetPin(u8 *pszPin) {
    //return sdk_dev_bt_set_pin(pszPin);
    s32 ret;

    if (pszPin == NULL || strlen(pszPin) != 6) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_WPIN, (unsigned int) pszPin, 0);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ�������豸����
** Parameters:          u8 *pszName
                    u32 nLen
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             �豸����Ϊ17λ���ڵ��ַ���
*****************************************************************************/
s32 sdkBtGetName(u8 *pszName, u32 nLen) {
    //return sdk_dev_bt_get_name(pszName, nLen);
    s32 ret;

    if (pszName == NULL || nLen < 64) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_RNAME, 0, (unsigned int) pszName);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	�����������豸����
** Parameters:          u8 *pszName
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkBtSetName(u8 *pszName) {
    //return sdk_dev_bt_set_name(pszName);
    s32 ret;

    if (pszName == NULL || strlen(pszName) > 64) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_WNAME, (unsigned int) pszName, 0);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ����豸����Ϣ
** Parameters:          SDK_BT_PAIR_INFO *pstPariInfo
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             �����Ϣ��Զ���豸��Ϣ�У�"�豸����"��G2��G3ʹ�õ�ģ����
                    ��֧��;
                    ��ȡ��������������������δ����״̬��ɡ����������״̬
                    �������᷵�����ʧ��
*****************************************************************************/
s32 sdkBtGetPairDevInfo(SDK_BT_PAIR_INFO *pstPariInfo) {
    //return sdk_dev_bt_get_pairdevInfo((strBtPairList *)pstPariInfo);
    strBtPairList list;
    s32 ret;

    if (pstPariInfo == NULL) {
        return SDK_PARA_ERR;
    }
    memset(&list, 0, sizeof(list));
    ret = ddi_bt_ioctl(DDI_BT_CTL_PAIRED_LIST, 0, (unsigned int) &list);

    if (ret == DDI_OK) {
        memcpy(pstPariInfo, &list, sizeof(strBtPairList));
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	ɾ������豸����Ϣ
** Parameters:          u32 nIndex
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             ����ֵ�ķ�Χ0~3
                    ��ȡ��������������������δ����״̬��ɡ����������״̬
                    �������᷵�����ʧ��
*****************************************************************************/
s32 sdkBtDelPairDevInfo(u32 nIndex) {
    //return sdk_dev_bt_del_pairdevInfo(nIndex);
    s32 ret;

    if (nIndex > 3) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_PAIRED_DEL, nIndex, 0);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	���������豸�ܷ񱻷���
** Parameters:          bool bDiscover   true ���Ա����֣�fasle ���ɷ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkBtSetDiscoverable(bool bDiscover) {
    //return sdk_dev_bt_set_discoverable(bDiscover);
    s32 ret;

    ret = ddi_bt_ioctl(DDI_BT_CTL_DISCOVERABLE, (unsigned int) bDiscover, 0);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡԶ���豸����Ϣ
** Parameters:          SDK_BT_REMOTE_INFO *pstRemote Զ���豸����Ϣ
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks:             ���������ӵ�״̬�»�ȡ
*****************************************************************************/
s32 sdkBtGetRemoteInfo(SDK_BT_REMOTE_INFO *pstRemote) {
    //return sdk_dev_bt_get_remoteInfo((strBtRemote *)pstRemote);
    strBtRemote bt_info;
    s32 ret;

    if (pstRemote == NULL) {
        return SDK_PARA_ERR;
    }
    ret = ddi_bt_ioctl(DDI_BT_CTL_REMOTE_INFO, 0, (unsigned int) &bt_info);

    if (ret == DDI_OK) {
        memcpy(pstRemote, &bt_info, sizeof(strBtRemote));
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ��������ģʽ
** Parameters:
** Returned value:	�ɹ�����            0 �� 1
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		zjp  2014.12.11
** Remarks:             E_SDK_BT_PIN_CODE:Ϊ PIN CODEPIN CODEģʽ��������������룩
                    E_SDK_BT_JUST_WORK:Ϊ JUST WORKģʽ������������룩
*****************************************************************************/
SDK_BT_PIN_MODE sdkBtGetPinMode() {
    s32 pinMode;      // = bt_get_pin_mode();
    s32 ret;

    ret = ddi_bt_ioctl(DDI_BT_CTL_PAIRED_MODE_GET, 0, (u32)(&pinMode));

    if (ret == DDI_OK) {
        return (SDK_BT_PIN_MODE) pinMode;
    }
    else if (ret == DDI_EIO) {
        return SDK_BT_PIN_MODE_ERR;
    }
    else {
        return SDK_BT_PIN_MODE_ERR;
    }
}

/*****************************************************************************
** Descriptions:	������������ģʽ
** Parameters:          E_SDK_BT_PIN_MODE ePinMode:
                    E_SDK_BT_PIN_CODE:Ϊ PIN CODEPIN CODEģʽ��������������룩
                    E_SDK_BT_JUST_WORK:Ϊ JUST WORKģʽ������������룩
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		zjp  2014.12.11
** Remarks:
*****************************************************************************/
s32 sdkBtSetPinMode(SDK_BT_PIN_MODE ePinMode) {
    s32 ret;

    if (ePinMode >= SDK_BT_PIN_MODE_ERR) {
        return SDK_PARA_ERR;
    }
    //return sdk_dev_bt_set_pin_mode( ePinMode );
    ret = ddi_bt_ioctl(DDI_BT_CTL_PAIRED_MODE_SET, (unsigned int) ePinMode, 0);

    if (ret == DDI_OK) {
        return SDK_OK;
    }
    else if (ret == DDI_EIO) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

