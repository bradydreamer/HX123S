#include "sdkdevOption.h"
#include "sdkGlobal.h"

#ifndef  DEBUG_PED
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

typedef struct _strLedGleamPara {
    u32 m_led; //LED���
    u32 m_ontime; //������������ʱ�䣬��λ����
    u32 m_offtime; //�������𱣳�ʱ�䣬��λ����
    u32 m_duration; //��˸����ʱ�䣬��λ����
} strLedGleamPara;

/*****************************************************************************
** Descriptions:	��led�豸
** Parameters:          Input: None
** Returned value:
** Created By:		taowz  2014.05.29
** Remarks:
*****************************************************************************/
s32 sdkLedOpen(void) {
    s32 rslt;

    rslt = ddi_led_open();

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�ر�led�豸
** Parameters:          Input: None
** Returned value:
** Created By:		taowz  2014.05.29
** Remarks:
*****************************************************************************/
s32 sdkLedClose(void) {
    s32 rslt;

    rslt = ddi_led_close();

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	����led�Ʒ�ʽ
** Parameters:          Input:  SDK_PED_LAMP_ONOFF eLamp,
                            u32 uiLed, //LED���   ��-0x01����-0x02����-0x03����-0x04
                            s32 siTimer
** Returned value:
** Created By:		taowz  2014.05.29
** Remarks:
*****************************************************************************/
static s32 sdkLedTypeCtrl(SDK_PED_LAMP_ONOFF eLamp, u32 uiLed, s32 siOnTimer, s32 siOffTimer,
                          s32 siTimer) {
    s32 rslt = SDK_ERR;
    strLedGleamPara stledcfg;

    stledcfg.m_led = uiLed;
    stledcfg.m_ontime = siOnTimer;
    stledcfg.m_offtime = siOffTimer;
    stledcfg.m_duration = siTimer;

    switch (eLamp) {
        case SDK_LAMP_OFF:
            rslt = ddi_led_sta_set(uiLed, SDK_LAMP_OFF);
            break;

        case SDK_LAMP_ON:
            rslt = ddi_led_sta_set(uiLed, SDK_LAMP_ON);
            break;

        case SDK_LAMP_TWINK:
            Trace("zjp", "sdk_dev_led_gleam:stledcfg.m_led = %d\r\n", stledcfg.m_led);
            Trace("zjp", "sdk_dev_led_gleam:stledcfg.m_ontime = %d\r\n", stledcfg.m_ontime);
            Trace("zjp", "sdk_dev_led_gleam:stledcfg.m_offtime = %d\r\n", stledcfg.m_offtime);
            Trace("zjp", "sdk_dev_led_gleam:stledcfg.m_duration = %d\r\n", stledcfg.m_duration);
            rslt = ddi_led_ioctl(DDI_LED_CTL_GLEAM, (u32) & stledcfg, 0);
            Trace("zjp", "sdk_dev_led_gleam:rslt = &d\r\n", rslt);
            break;

        default:
            break;
    }

    if (DDI_OK == rslt) {
        rslt = SDK_OK;
    }
    else {
        rslt = SDK_ERR;
    }
    Trace("zjp", "sdk_dev_led_type_ctrl:rslt = %d\r\n", rslt);
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_PED_LAMP_CTRL *pstLampCtrl
                               s32 timeout
** Returned value:
** Created By:		���ٲ�  2013.08.27
** Remarks:
*****************************************************************************/
static s32 sdkLedCtrl(const SDK_PED_LAMP_CTRL *pstLampCtr) {
    s32 rslt;
    s32 siOnTimers = 50;
    s32 siOffTimers = 50;

    rslt = sdkLedTypeCtrl(pstLampCtr->eLamp1, SDK_PED_LAMP_1, siOnTimers,
                          siOffTimers, pstLampCtr->siTimers);

    if (rslt != SDK_OK) {
        Assert(0);
        return rslt;
    }
    rslt = sdkLedTypeCtrl(pstLampCtr->eLamp2, SDK_PED_LAMP_2, siOnTimers,
                          siOffTimers, pstLampCtr->siTimers);

    if (rslt != SDK_OK) {
        Assert(0);
        return rslt;
    }
    rslt = sdkLedTypeCtrl(pstLampCtr->eLamp3, SDK_PED_LAMP_3, siOnTimers,
                          siOffTimers, pstLampCtr->siTimers);

    if (rslt != SDK_OK) {
        Assert(0);
        return rslt;
    }
    rslt = sdkLedTypeCtrl(pstLampCtr->eLamp4, SDK_PED_LAMP_4, siOnTimers,
                          siOffTimers, pstLampCtr->siTimers);

    if (rslt != SDK_OK) {
        Assert(0);
        return rslt;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	    POS�ǽ�led��
** Parameters:          const SDK_PED_LAMP_CTRL *pstLampCtrl �Ը��ƵĿ��Ʒ���
** Returned value:
                        SDK_PARA_ERR	��������
                        SDK_ERR	����ʧ��
                        SDK_COMM_UART_NOTOPEN	������̴���δ�򿪣�����������̲��У�
                        SDK_COMM_UART_NOESIST	������̴��ںŲ�����
                        SDK_OK	�򿪳ɹ�
** Created By:		    shiweisong  2013.08.28
** Remarks:
*****************************************************************************/
s32 sdkPEDLampControl(const SDK_PED_LAMP_CTRL *pstLampCtrl) {
    //s32 mode = 0;
    s32 rtn = 0;

    if (pstLampCtrl == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rtn = sdkLedCtrl(pstLampCtrl);                                    //���÷ǽ�
    return rtn;
}

/*****************************************************************************
** Descriptions:	����LCD��������������չ�����������û�������˸Ƶ��
** Parameters:          u32 ledNmb Ҫ���ƵƵĸ���
                    const SDK_PED_LAMP_CTRL_EXT pstLampCtr[] ������Ϣ����
** Returned value:	SDK_OK
                    SDK_ERR
** Created By:		���ƽ  2014.07.16
** Remarks:
*****************************************************************************/
static s32 sdkLedCtrlExt(u32 ledNmb, const SDK_PED_LAMP_CTRL_EXT pstLampCtr[]) {
    s32 rslt;
    s32 rtn = SDK_OK;
    u32 i = 0;

    while (i < ledNmb) {
        rslt = sdkLedTypeCtrl(pstLampCtr[i].lempState, pstLampCtr[i].lempID,
                              pstLampCtr[i].siOnTimers,
                              pstLampCtr[i].siOffTimers, pstLampCtr[i].siTimers);
        Trace("zjp", "sdk_dev_sdkLedCtrl_ext:rslt = %d\r\n", rslt);

        if (rslt != SDK_OK) {
            Assert(0);
            rtn = SDK_ERR;
        }
        i++;
    }

    Trace("zjp", "____________________sdk_dev_sdkLedCtrl_ext:rslt = %d\r\n", rslt);
    return rtn;
}

/*****************************************************************************
** Descriptions:	����LCD��������������չ�����������û�������˸Ƶ��
** Parameters:          u32 ledNmb Ҫ���ƵƵĸ���
                    const SDK_PED_LAMP_CTRL_EXT pstLampCtr[] ������Ϣ����
** Returned value:	SDK_PARA_ERR	��������
                    SDK_ERR	����ʧ��
                    SDK_COMM_UART_NOTOPEN	������̴���δ�򿪣�����������̲��У�
                    SDK_COMM_UART_NOESIST	������̴��ںŲ�����
                    SDK_OK	�򿪳ɹ�
** Created By:		���ƽ  2014.07.16
** Remarks:
*****************************************************************************/
s32 sdkPEDLampControlExt(u32 ledNmb, const SDK_PED_LAMP_CTRL_EXT pstLampCtr[]) {
    //s32 mode = 0;
    s32 rtn = 0;

    if (pstLampCtr == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rtn = sdkLedCtrlExt(ledNmb, pstLampCtr);                                      //���÷ǽ�
    return rtn;
}

/*****************************************************************************
** Descriptions:	��led�豸�汾
** Parameters:          Input: u8 * pucOutData
** Returned value:
** Created By:		taowz  2014.05.29
** Remarks:
*****************************************************************************/
s32 sdkLedVersion(u8 *pucOutData) {
    s32 rslt;

    if (NULL == pucOutData) {
        return SDK_PARA_ERR;
    }
    rslt = ddi_led_ioctl(DDI_LED_CTL_VER, 0, (u32) pucOutData);

    if (rslt == DDI_OK) {
        return SDK_OK;
    }
    else if (DDI_EIO == rslt) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

