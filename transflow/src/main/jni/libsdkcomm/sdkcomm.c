/******************************************************************************

                  ��Ȩ���� (C), 2001-2012, �¹��������ɷ����޹�˾

 ******************************************************************************
   �� �� ��   : sdkcomm.c
   �� �� ��   : ����
   ��    ��   : huangbx
   ��������   : 2012��5��7��
   ����޸�   :
   ��������   :
                ��ģ�齫�ṩ�Ľӿں�����
                ��ʼ��ͨѶ:sdkCommInit ��		Ԥ���� sdkPreComm
                ������·:sdkCommCreateLink��	�Ͽ���·:sdkCommDestoryLink��
                ��������:sdkCommSendData��		��������:sdkCommRecvData ��
                ����ͨѶ����:sdkCommConfig��    ���PPP����ͨѶ�ı���IP: sdkCommGetPppIP
                ���ڣ�
                �򿪴��� sdkCommOpenUart  ���رմ��� sdkCommCloseUart
                �򴮿ڷ����� sdkCommUartSendData �����մ������� sdkCommUartRecvData
                �汾��
                ���ģ��汾�ţ� sdkCommGetVersion
                ��ģ��ͨѶ�������ֵΪ2000Byte
                ��ģ�鴮�ں�ȡֵ��ΧΪ0-9,�ܹ�10�����ں�
                ��ģ���ͷ�ļ� sdkComm.h

    �޸���ʷ   :
    ��    ��   : 2012��5��7��
    ��    ��   : huangbx
    �޸�����   : �����ļ�
 *****************************************************************************/
#include "sdkGlobal.h" //zhouzhihua 2013.06.14 10:35

#ifndef  DEBUG_COMM
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


/*******************************************************************************************
** Descriptions:    ���PPP����ͨѶ�ı���IP
** Parameters:      pDest:IP
** Returned value:  IP����
                    SDK_PARA_ERR	�������󷵻�
                    SDK_ERR         ��ȡIPʧ��
** Created By:      �Ʊ���(2012-05-08)
** Remarks:         ���������ص�IP�����Ϊ16���ֽڳ��ȣ��������������������ڻ����16���ֽڵ��ڴ�ռ�
*******************************************************************************************/
s32 sdkCommGetPppIP(u8 *pasDest) {
    if (NULL == pasDest) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    �ж�IP��ʽ�Ƿ�Ϸ�
** Parameters:      pIPstr IP�ַ���
** Returned value:  true �Ϸ� false �Ƿ�
** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
bool sdkCommIsRightIP(const u8 *pasIPstr) {
    return false;
}

/*******************************************************************************************
** Descriptions:    ����MODEMͨѶ��չ����(�첽 ����MODEM���ʲ���)
** Parameters:      IsDefaultValue 0-����Ĭ��ֵ 1-Ĭ��ֵ
                    SDK_COMM_STMODEMEXPARAM *stModemExParam MODEM��չ����
** Returned value:  SDK_OK                  �ɹ�
                    SDK_PARA_ERR            ��������

** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
s32 sdkCommSendModemCfgEx_Asyn(bool IsDefaultValue,
                               const SDK_COMM_STMODEMEXPARAM *pstModemExParam) {
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ����MODEMͨѶ��չ����
** Parameters:      IsDefaultValue 0-����Ĭ��ֵ 1-Ĭ��ֵ
                    SDK_COMM_STMODEMEXPARAM *stModemExParam MODEM��չ����
** Returned value:  SDK_OK                  �ɹ�
                    SDK_PARA_ERR            ��������

** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
s32 sdkCommSendModemCfgEx(bool IsDefaultValue, const SDK_COMM_STMODEMEXPARAM *pstModemExParam) {
    if (pstModemExParam == NULL && !IsDefaultValue) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ����ͨѶ����
** Parameters:
** Returned value:  SDK_OK          �ɹ�
                    SDK_PARA_ERR    �����Ƿ�
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ����
                    ���ͨѶ�����иĶ��������ô˺����������ò���
*******************************************************************************************/
s32 sdkCommConfig(const SDK_COMM_STCOMMPARAM *pstCommParam) {
    if (NULL == pstCommParam) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ��ʼ��ͨѶ
** Parameters:      stCommParam:ͨѶ�����ṹ��
** Returned value:  SDK_OK                  ��ʼ���ɹ�
                    SDK_PARA_ERR            ��������
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_NO_SIM         û�в����ֻ���//huangbx 20130108
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����Ĭ��MODEM��չ��������ʼ�������������������IP���������������IP
                    ��������ͨѶ��������PPP��·,9s�����û�ܳɹ�����PPP��·�򷵻س�ʱ(SDK_TIME_OUT)
*******************************************************************************************/
s32 sdkCommInit(const SDK_COMM_STCOMMPARAM *pstCommParam) {
    if (pstCommParam == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    Ԥ����
** Parameters:
** Returned value:  SDK_OK   Ԥ���ųɹ�
                    SDK_ERR  ʧ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����Ԥ���Ź��ܣ����Ͳ��������ֱ���˳����ȴ����Ž��
                    1.LANͨѶ��ʽ�²�����
                    2.���ñ�����ǰ�����ȵ��� ��ʼ��ͨ��

*******************************************************************************************/
s32 sdkCommPre() {
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    �Ͽ���·
** Parameters:
** Returned value:  SDK_OK   �ɹ�
** Created By:      �Ʊ���(2012-05-07)
** Remarks:
*******************************************************************************************/
s32 sdkCommDestoryLink() {
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ������·
** Parameters:
** Returned value:  SDK_OK   ����ͨѶ
                    SDK_ESC             �ֶ�ȡ��
                    SDK_TIME_OUT        ���ӳ�ʱ
                    SDK_COMM_NOLINE     δ�ӵ绰��
                    SDK_COMM_NOSTAKE    �ֻ��Żص���
                    SDK_COMM_ENGAGED    �绰��ռ��
                    SDK_COMM_NO_SIM     δ�����ֻ���
                    SDK_COMM_LINKFAIL   ��������ʧ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ��ʼ�����õĲ�����������
   //zhouzhihua 20130614
*******************************************************************************************/
s32 sdkCommCreateLinkEx(s32 (*pDisFunc)(const s32 usRecvTimeOut, const s32 usCurrentTime)) {
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ������·
** Parameters:
** Returned value:  SDK_OK   ����ͨѶ
                    SDK_ESC             �ֶ�ȡ��
                    SDK_TIME_OUT        ���ӳ�ʱ
                    SDK_COMM_NOLINE     δ�ӵ绰��
                    SDK_COMM_NOSTAKE    �ֻ��Żص���
                    SDK_COMM_ENGAGED    �绰��ռ��
                    SDK_COMM_NO_SIM     δ�����ֻ���
                    SDK_COMM_LINKFAIL   ��������ʧ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ��ʼ�����õĲ�����������

*******************************************************************************************/
s32 sdkCommCreateLink() {
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pSendData   Ҫ���͵�����
                    Len         ���ݵĳ���
                    nProTocol   Э��
** Returned value:  SDK_OK      �ɹ�
                    SDK_PARA_ERR ��������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����nProtocolָ����Э������������
                    1.	����������󳤶�Ϊ2000 byte
                    2.	��SDK_COMM_SEND8583Э�鷢�����ݣ�ϵͳ���Զ�����ǰ�����������ֽڵĳ���
                        �����ڽ���ʱϵͳ�����һ��������8583���ٷ�������
                    3.	��SDK_COMM_TRANSPARENTЭ�鷢�����ݣ��ڽ�������ʱϵͳ���յ����ݺ���������أ�
                        ���յ����ݿ��ܲ���һ�����������ݰ�����Ҫ�û���֤���յ������Ƿ�����

*******************************************************************************************/
s32 sdkCommSendData(const u8 *pheSendData, const u16 usLen, bool bMode) {
    if (pheSendData == NULL) //ʯ���� 2012.08.20 11:47
    {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pRecvBuf        ���յ�������
                    nRecvBufMaxLen  ���յ���󳤶�
                    RecvTimeOut     ���ճ�ʱʱ�� ����Ϊ��λ 5 <= RecvTimeOut <= 120
** Returned value:  ���ؽ��յ������ݳ���
                    SDK_COMM_WAVELOST   ͨѶ�ж�
                    SDK_PARA_ERR        ��������
                    SDK_TIME_OUT         ͨѶ��ʱ
                    SDK_COMM_BUFFER_ERROR      �������ݻ���������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         1.	�����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    2.	pDisFuncΪNULLʱ����Ĭ����ʾ�������û����Զ�����ʾ���ݣ������ο�DisFuncDemo
*******************************************************************************************/
s32 sdkCommRecvData(u8 *pheRecvBuf, const s32 usRecvBufMaxLen, const s32 usRecvTimeOut,
                    s32 (*pDisFunc)(const s32 usRecvTimeOut, const s32 usCurrentTime)) {
    if (pheRecvBuf == NULL || usRecvBufMaxLen <= 0 ||
        (usRecvTimeOut < 5 * 1000 || usRecvTimeOut > 120 * 1000)) {
        Assert(0); //ʯ���� 2012.08.20 11:49
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pRecvBuf        ���յ�������
                    nRecvBufMaxLen  ���յ���󳤶�
                    RecvTimeOut     ���ճ�ʱʱ�� �Ժ���Ϊ��λ 5000 <= RecvTimeOut <= 120000
** Returned value:  ���ؽ��յ������ݳ���
                    SDK_COMM_WAVELOST   ͨѶ�ж�
                    SDK_PARA_ERR        ��������
                    SDK_TIME_OUT         ͨѶ��ʱ
                    SDK_ESC             �û�ȡ��
                    SDK_COMM_BUFFER_ERROR      �������ݻ���������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         1.	�����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    2.	pDisFuncΪNULLʱ����Ĭ����ʾ�������û����Զ�����ʾ���ݣ������ο�DisFuncDemo
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommTmsRecvData(u8 *pheRecvBuf, const s32 usRecvBufMaxLen, const s32 usRecvTimeOut,
                       s32 (*pDisFunc)(const s32 usRecvTimeOut, const s32 usCurrentTime),
                       bool bBreakMode) {
    if (pheRecvBuf == NULL || usRecvBufMaxLen <= 0 ||
        (usRecvTimeOut < 5 * 1000 || usRecvTimeOut > 120 * 1000)) {
        Assert(0); //ʯ���� 2012.08.20 11:49
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************************************
** Descriptions:    ����PPP��·
** Parameters:      uiTimeOut:��ʱʱ�� ��λms
** Returned value:  SDK_OK                  ����PPP�ɹ�
                    SDK_ERR                 ʧ��
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_NO_SIM         û�в����ֻ���//huangbx 20130108
                    SDK_ESC                     �ֶ�ȡ��
** Created By:      �Ʊ���(2013-01-08)
** Remarks:         uiTimeOut Ϊ0���ͽ���PPP�����ֱ�ӷ���,���ж��Ƿ�ɹ�
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommCreatePPP(u32 uiTimeOut) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	Ϊ���modem ͨ����������һ������
** Parameters:          SDK_COMM_HARD_MODEM_MODE eMode
** Returned value:
** Created By:		shiweisong  2013.11.18
** Remarks:
*****************************************************************************/
s32 sdkCommSetHardModemMode(SDK_COMM_HARD_MODEM_MODE eMode) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ����ģ���IMSI
** Parameters:          u8 *pasDest:��ȡ������ģ���
** Returned value:	�ɹ�����ģ��ų���
                                �������󷵻�SDK_PARA_ERR
                                ʧ�ܷ��س�ʱ
** Created By:		���ƽ  2013.12.19
** Remarks:
*****************************************************************************/
s32 sdkCommGetWirelessModuleIMSI(u8 *pasDest) {
    if (pasDest == NULL) {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ����ģ���id
** Parameters:          u8 *pasDest:��ȡ������ģ���
** Returned value:	�ɹ�����ģ��ų���
                                �������󷵻�SDK_PARA_ERR
                                ʧ�ܷ��س�ʱ
** Created By:		shijianglong  2013.03.18
** Remarks:
*****************************************************************************/
s32 sdkCommGetWirelessModuleID(u8 *pasDest) {
    if (pasDest == NULL) {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û�����֧�ֵ�ͨѶ��ʽ
   ��ڲ���:
   �� �� ֵ:���ظ���������֧�ֵ�ͨѶ��ʽ
   ��      ע:
 ********************************************************************/
s32 sdkCommGetCommMask(void) {
    return SDK_COMMU_MASK_BLUETOOTH;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û���Ĭ����֧�ֵ�ͨѶ��ʽ
   ��ڲ���:
   �� �� ֵ:���ظ�������Ĭ����֧�ֵ�ͨѶ��ʽ
   ��      ע:
 ********************************************************************/
SDK_COMM_COMMMODE sdkComGetDefaultCommuType(void) {
    return SDK_COMM_BLUETOOTH;
}

/*****************************************************************************
** Descriptions:	��ȡ�����ź�ǿ�ȵȼ�
** Parameters:
** Returned value:	��ʱ����SDK_TIME_OUT
                    �ɹ����������ź�ǿ�ȵȼ���ʮ�����Ʒ�Χ0x00~0x31
** Created By:		shijianglong  2013.08.01
** Remarks:
*****************************************************************************/
s32 sdkCommGetSignalLevel(void) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡС����Ϣ
** Parameters:
** Returned value:
** Created By:
** Remarks: ����508���ͣ�siNodeNum = 10,siTimeOut��������Ϊ2000 ��pVar = NULL
*****************************************************************************/
s32 sdkCommGetBaseStationInfor(SDK_COMM_STATION_INFO *pstStationInfo, s32 siNodeNum, s32 siTimeOut,
                               void *pVar) {
    if (NULL == pstStationInfo) {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

