/******************************************************************************

                  ��Ȩ���� (C), 2001-2013, xgd

******************************************************************************
   �� �� ��   : sdkuart.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��8��1��
   ����޸�   :
   ��������   : uart����dev�ļ�

   �޸���ʷ   :
   1.��    ��   : 2013��8��1��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/
#include "sdkDevOption.h"
#include "sdkGlobal.h"
#include "sdkPrivateHead.h"

#ifndef  DEBUG_COMM
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


//ͨ�ŷ����������ֵ
#define SDK_COMM_MAXLEN                     2000        //ͨѶ�������ֵ
//���ں����ֵ
#define SDK_COMM_MAXUARTNUM                 9           //0-9 �ܹ�10�����ں�

typedef enum usbClass   //�豸����
{
    USB_CLASS_HID = 1, //Human Interface Device��������ѧ�豸 �������֮��Ķ���
    USB_CLASS_CDC = 2, //Communication Device Class ͨ�Žӿ���
    USB_CLASS_CCID = 3, //USB Chip/Smart Card Interface Devices-USBоƬ���ܿ��ӿ��豸
    USB_CLASS_MSD = 4, //�������洢�豸���������ݴ��� ����:U��
} eUsbClass;

/*****************************************************************************
** Descriptions:	��usb�豸
** Parameters:          u32 nClass:usb ����ʶ����
** Returned value:
** Created By:		���ƽ  2014.07.16
** Remarks:
*****************************************************************************/
s32 sdkUsbOpen(eUsbClass nClass) {
    //return sdk_dev_usb_open( (u32)nClass );
    s32 rslt;

    rslt = ddi_usb_open(nClass);

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else if (DDI_EBUSY == rslt) {
        return SDK_EBUSY;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	�ر�usb�豸
** Parameters:          u32 nClass:usb ����ʶ����
** Returned value:
** Created By:		���ƽ  2014.07.16
** Remarks:             ���ͻ�������������δ���꣬������������ɺ��ٹر�
                    �ر�δ��usbͨ�Ŷ˿ڣ�ֱ�ӷ���SDK_OK;
*****************************************************************************/
s32 sdkUsbClose(eUsbClass nClass) {
    //return sdk_dev_usb_close( (u32)nClass);
    s32 rslt;

    rslt = ddi_usb_close(nClass);

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

#if 0 /*Modify by ���ƽ at 2015.03.31  18:31 */
/*****************************************************************************
** Descriptions:	���usb��������
** Parameters:          u32 nClass:usb ����ʶ����
** Returned value:
** Created By:		���ƽ  2014.07.16
** Remarks:         ���ͻ�������������δ������ɣ������������ֹͣ����
*****************************************************************************/
static s32 sdkUsbClear(eUsbClass nClass)
{
    s32 rslt;

    rslt = ddi_usb_ioctl(DDI_USB_CTL_CLEAR, nClass, 0);

    if (DDI_OK == rslt)
    {
        return SDK_OK;
    }
    else if(DDI_EIO == rslt)
    {
        return SDK_EBUSY;
    }
    return SDK_ERR;
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:	��ȡusb������
** Parameters:          u32 nClass:usb  ����ʶ����
                    u8 *pucOutData  ��ȡ������
                    u16 uiExOutLen  ������ȡ���ݵĳ���
                    u32 timerout    ��ʱʱ�䣬��λ���롣��Ϊ0����ʱȡ��
** Returned value:	�ɹ�            ʵ�ʶ�ȡ���ݵĳ���
                    SDK_ERR         ʧ��
                    SDK_EIO         �豸δ��
                    SDK_PARA_ERR    ��������
                    SDK_TIME_OUT    ��ʱ����
** Created By:		tjb  2014.06.23
** Remarks:             ������Ҫ��ȡ��Ԥ��ָ����uiExOutLen���ȵ����ݣ�����Ҫ��
                    uiTimeout��Ϊ0��������һֱ��ȡ��ֱ��������
                    ��û��Ԥ�����ݳ��ȣ����������С��uiTimeoutֵ�����ٺ���
                    ��������
*****************************************************************************/
static s32 sdkUsbRead(eUsbClass nClass, u8 *pucOutData, u32 uiExOutLen, u32 uiTimeout) {
    s32 ret, rslt;
    s32 all_recv = uiExOutLen;
    s32 had_recv = 0;
    s32 now_recv;
    s32 timerid = sdkTimerGetId();
    //s8 *temp;
    //char status;
    s32 buf_len;

    if (pucOutData == NULL || uiExOutLen == 0) {
        Trace("bt_test", "sdkUsbRead: ���������������\n");
        return SDK_PARA_ERR;
    }

    if (uiExOutLen > MAX_BUFFER_LENGTH) {
        buf_len = MAX_BUFFER_LENGTH;
    }
    else {
        buf_len = uiExOutLen;
    }

/*
    temp = xgd_malloc(buf_len);
    if (temp == NULL)
    {
        Trace("bt_test", "xgd_malloc failed\n");
        return SDK_ERR;
    }
 */
    for (; ;) {
        //memset(temp, 0, buf_len);
        if (all_recv >= buf_len) //??�ж���������
        {
            ret = ddi_usb_read((u32) nClass, pucOutData + had_recv, buf_len);
            Trace("zjp", "ddi_usb_read:ret = %d, uiExOutLen= %d\r\n", ret, uiExOutLen);
        }
        else {
            ret = ddi_usb_read((u32) nClass, pucOutData + had_recv, all_recv);
            Trace("zjp", "ddi_usb_read:ret = %d, uiExOutLen= %d\r\n", ret, uiExOutLen);
        }

        if (ret < 0)        //error
        {
            Trace("bt_test", "sdkUsbRead: ��������ʧ��:ret\n", ret);

            if (ret == DDI_EINVAL)             //��������
            {
                rslt = SDK_PARA_ERR;
            }
            else if (ret == DDI_EIO)     //�豸δ�򿪻������
            {
                rslt = SDK_EIO;
            }
            else if (ret == DDI_EDATA)    //������������ݴ���
            {
                rslt = SDK_EDATA;
            }
            else {
                rslt = SDK_ERR;
            }
            memset(pucOutData, 0, uiExOutLen);
            break;
        }
        else {
            if (ret < all_recv) {
                now_recv = ret;
            }
            else {
                Trace("bt_test", "sdkUsbRead: ��ʧ����%d\n", ret - all_recv);
                now_recv = all_recv; //�����յ������ݳ�����Ԥ�ڣ���������ݵĶ�ʧ������δ�����???
            }
            //memcpy(pucOutData+had_recv, temp, now_recv);
            all_recv -= now_recv;
            had_recv += now_recv;

            if (all_recv <= 0)            /* ���յ������ݸ�Ԥ����ͬ */
            {
                rslt = had_recv;
                Trace("bt_test", "sdkUsbRead: had_recv =%d\n", had_recv);
                break;
            }

            if (uiTimeout != 0 && sdkTimerIsEnd(timerid, uiTimeout)) {
                Trace("bt_test", "sdkUsbRead: �豸æ��ʱ����\n");

                if (had_recv > 0)            /* ���Ƕ��������ݣ��򲻷��س�ʱ���� */
                {
                    rslt = had_recv;
                }
                else {
                    rslt = SDK_TIME_OUT;
                }
                break;
            }
        }
    }

    //Trace("bt_test", "sdkUsbRead: �������ݳɹ�����������%d�ֽ�\n", had_recv);

    //xgd_free(temp);
    return rslt;
}

/*****************************************************************************
** Descriptions:	��usbд������
** Parameters:          u32 nClass:usb ����ʶ����
                    const u8 *pucInData     ����д�������
                    int uiExInLen           ����д�����ݵĳ���
                    u32 timerout            ��ʱʱ�䣬��λ���롣��Ϊ0����ʱȡ��
** Returned value:	�ɹ�            ʵ��д�����ݵĳ���
                    SDK_ERR         ʧ��
                    SDK_EIO         �豸δ��
                    SDK_PARA_ERR    ��������
                    SDK_TIME_OUT    ��ʱ����
** Created By:		tjb  2014.06.23
** Remarks:
*****************************************************************************/
static s32 sdkUsbWrite(eUsbClass nClass, const u8 *pucInData, u32 uiExInLen, u32 uiTimeout) {
    s8 *p;
    s32 ret;
    s32 all_send = uiExInLen;
    s32 had_send = 0;
    s32 now_send;
    s32 timerid = sdkTimerGetId();

    //char status;

    if (pucInData == NULL || uiExInLen == 0) {
        Trace("bt_test", "sdkusbWrite: ���������������\n");
        return SDK_PARA_ERR;
    }

    for (; ;) {
        if (all_send > 512) {
            now_send = 512;
        }
        else {
            now_send = all_send;
        }
        p = (s8 * )(pucInData + had_send);

        ret = ddi_usb_write((u32) nClass, p, now_send);
        Trace("bt_test", "sdkusbWrite: ret = %d\n", ret);

        if (ret < 0) {
            switch (ret) {
                case DDI_EINVAL: {
                    ret = SDK_PARA_ERR;
                }
                    break;

                case DDI_EIO: {
                    ret = SDK_EIO;
                }
                    break;

                case DDI_EBUSY: {
                    ret = SDK_EBUSY;
                }
                    break;

                default: {
                    ret = SDK_ERR;
                }
                    break;
            }

            if (ret != SDK_EBUSY)        //error
            {
                Trace("bt_test", "sdkusbWrite: ��������ʧ��\n");
                return ret;
            }

            if (uiTimeout != 0 && sdkTimerIsEnd(timerid, uiTimeout)) {
                Trace("bt_test", "sdkusbWrite: �豸æ��ʱ����\n");
                return SDK_TIME_OUT;
            }
            continue;
        }
        else if (ret == now_send) {
            had_send += ret;
            all_send -= ret;

            if (all_send <= 0) {
                break;
            }

            if (uiTimeout != 0 && sdkTimerIsEnd(timerid, uiTimeout)) {
                Trace("bt_test", "sdkusbWrite: �豸æ��ʱ����\n");
                return SDK_TIME_OUT;
            }
        }
        else {
            Trace("bt_test", "sdkusbWrite: �������ݵ���0�򲻵���Ԥ��\n");
            return SDK_ERR;
        }
    }

    Trace("bt_test", "sdkUsbWrite: �������ݳɹ�����������%d�ֽ�\n", had_send);
    return had_send;
}

/*****************************************************************************
** Descriptions:	��ȡusbģ��汾��
** Parameters:          u32 nClass:usb ����ʶ����
                    Input: u8 * pucOutData      ����汾�ŵ��ַ���
** Returned value:
** Created By:		���ƽ  2014.07.16
** Remarks:
*****************************************************************************/
s32 sdkUsbVersion(u8 *pucOutData) {
    //return sdk_dev_usb_version(pucOutData);
    s32 rslt;

    rslt = ddi_usb_ioctl(DDI_USB_CTL_VER, 0, (u32) pucOutData);

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    else if (DDI_EIO == rslt) {
        return SDK_EIO;
    }
    return SDK_ERR;
}

#if 0 /*Modify by ���ƽ at 2015.03.31  18:32 */
/*******************************************************************************************
** Descriptions:    ��鴮�ڲ����Ƿ�Ϸ�
** Parameters:      stCommUartParam ���ڲ���
** Returned value:  SDK_PARA_ERR    �����Ƿ�
                    SDK_OK          �Ϸ�
** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
static s32 sdkCommCheckUartParam(const SDK_COMM_STUARTPARAM *pstCommUartParam)
{
    if(pstCommUartParam == NULL)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    switch(pstCommUartParam->uiBaudrate)
    {
         //������֧�� 9600 14400 19622 38400 57600 115200 ����Ϊ��������
         case 9600:
         case 14400:
         case 19200:
         case 38400:
         case 57600:
         case 115200:
            {
            }
            break;

         default:
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
    }

    switch(pstCommUartParam->ucByteLen)
    {
         //����λ֧�� 5,6,7,8λ������Ϊ�������
         case 0: //Ĭ��ֵ
         case '5':
         case '6':
         case '7':
         case '8':
            {
            }
            break;

         default:
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
    }

    switch(pstCommUartParam->ucCheckBit)
    {
         //����λ 'n'�޼���λ
         case 0: //Ĭ��ֵ
         case 'n':
         case 'o':
         case 'e':
         case 's':
            {
            }
            break;

         default:
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
    }

    switch(pstCommUartParam->ucStopBitLen)
    {
         //ֹͣλ֧�֣�����Ϊ�������
         case 0: //Ĭ��ֵ
         case '1':
         case '2':
            {
            }
            break;

         default:
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
    }

    switch(pstCommUartParam->ucFlowControl)
    {
         //���ؿ���ֻ֧�� ������
         case 0: //Ĭ��ֵ
         case '0':
            {
            }
            break;

         default:
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
    }

    return SDK_OK;
}

#endif /* if 0 */

/*******************************************************************************************
** Descriptions:    ��鴮��״̬
** Parameters:
** Returned value:  SDK_COMM_UART_CLOSE      ���ڹر�
                    SDK_COMM_UART_OPEN       �����Ѵ�
                    SDK_COMM_UART_MISS       ���ڲ�����
** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
s32 sdkCommGetUartState(const u8 nCom) {
    return SDK_COMM_UART_MISS;
}

/*******************************************************************************************
** Descriptions:    �򿪲���ʼ�������豸
** Parameters:      stUartInfo ��������
** Returned value:  SDK_OK �ɹ�
                    SDK_COMM_UART_USED      ���ڱ�ռ��
                    SDK_COMM_UART_NOESIST   ���ڲ�����
                    SDK_PARA_ERR            ��������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:
*******************************************************************************************/
s32 sdkCommOpenUart(const SDK_COMM_STUARTPARAM *pstUartInfo) {
    if (pstUartInfo == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (pstUartInfo->ucCom == SDK_COMM_UART_USB) {
        return sdkUsbOpen(USB_CLASS_CDC);
    }
    else if (pstUartInfo->ucCom == SDK_COMM_UART_USBHID) {
        return sdkUsbOpen(USB_CLASS_HID);
    }
    else {
        return SDK_COMM_UART_NOESIST;
    }
}

/*******************************************************************************************
** Descriptions:    �رմ��ڶ�ռ���ָ����ڲ�����Ϊ38400
** Parameters:
** Returned value:  SDK_OK �ɹ�
                    SDK_COMM_UART_NOESIST ���ڲ�����
** Created By:      �Ʊ���(2012-05-07)
** Remarks:
*******************************************************************************************/
s32 sdkCommCloseUart(const u8 ucCom) {
    if (ucCom == SDK_COMM_UART_USB) {
        return sdkUsbClose(USB_CLASS_CDC);
    }
    else if (ucCom == SDK_COMM_UART_USBHID) {
        return sdkUsbClose(USB_CLASS_HID);
    }
    else {
        return SDK_COMM_UART_NOESIST;
    }
}

/*******************************************************************************************
** Descriptions:    �򴮿ڷ�������
** Parameters:      nCom        ���ں�
                    pSendData   ���͵�����
                    len         ���ݳ���
** Returned value:  SDK_OK                  �ɹ�
                    SDK_PARA_ERR            ��������
                    SDK_COMM_UART_NOESIST   ���ڲ�����
                    SDK_COMM_UART_NOTOPEN   ����δ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         �շ���������ʱ�����ȶ�ռ����
                    //huangbx 20121102 ȥ���������ݳ������� �����������Ϊ 2000 byte
*******************************************************************************************/
s32 sdkCommUartSendData(const u8 ucCom, const u8 *pheSendData, const s32 usLen) {
    if (pheSendData == NULL || usLen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (ucCom == SDK_COMM_UART_USB) {
        return sdkUsbWrite(USB_CLASS_CDC, pheSendData, usLen, 0);
    }
    else if (ucCom == SDK_COMM_UART_USBHID) {
        return sdkUsbWrite(USB_CLASS_HID, pheSendData, usLen, 0);
    }
    else {
        return SDK_COMM_UART_NOESIST;
    }
}

/*******************************************************************************************
** Descriptions:    ���մ�������
** Parameters:      nCom                    ���ں�
                    pRecvData               ��������
                    nRecvBufMaxLen          ���������ݻ����С
                    nTimeOut                ��ʱʱ�� ms��λ //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
** Returned value:  �������ݳ���
                    SDK_PARA_ERR            ��������
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_UART_NOTOPEN   ����û��
                    SDK_COMM_UART_NOESIST   ���ڲ�����
                    SDK_COMM_BUFFER_ERROR   �������ݻ���������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         �շ���������ʱ�����ȶ�ռ����
                    �����յ��������ݺ��������ػ��߳�ʱʱ�䵽����
                    �����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommUartRecvData(const u8 ucCom, u8 *pheRecvData, const s32 usRecvBufMaxLen,
                        const s32 usRecvTimeOut) {
    if (pheRecvData == NULL || usRecvBufMaxLen <= 0 || usRecvTimeOut < 0) //ʯ���� 2012.08.20 11:51
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (ucCom == SDK_COMM_UART_USB) {
        return sdkUsbRead(USB_CLASS_CDC, pheRecvData, usRecvBufMaxLen, usRecvTimeOut);
    }
    else if (ucCom == SDK_COMM_UART_USBHID) {
        return sdkUsbRead(USB_CLASS_HID, pheRecvData, usRecvBufMaxLen, usRecvTimeOut);
    }
    else {
        return SDK_COMM_UART_NOESIST; //������������ʱûʹ��
    }
}

