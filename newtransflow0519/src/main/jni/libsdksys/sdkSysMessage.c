/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkSysMessage.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2012��6��13�� ������
   ����޸�   :
   ��������   : ��Ϣ���й�����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��6��13�� ������
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/
#include "sdkGlobal.h"
#include "sdkList.h"

#ifndef  DEBUG_MESSAGE
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


static SDK_SYS_LIST_HAND AppMsgLink = NULL;

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: �ʼ���Ϣ
   ��ڲ���: 1:lpMsg: ��Ϣ�ڵ�ָ��
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
s32 sdkSysPostMessage(SDK_MSG *ppstMsg) {
    if (AppMsgLink == NULL) {
        AppMsgLink = sdkSysListCreate();
        Assert(AppMsgLink != NULL);
    }

    //if(ppstMsg != NULL || (AppMsgLink == NULL))
    if (ppstMsg != NULL && (AppMsgLink != NULL)) {
#if 0         //hhh

        if(ppstMsg->ucPriority ==  SDK_SYS_MSG_HIGH)
        {
            return sdkSysListAddAtFront(AppMsgLink, ppstMsg);
        }
        else
#endif
        {
            return sdkSysListAddAtEnd(AppMsgLink, ppstMsg);
        }
    }
    return SDK_PARA_ERR;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ѯ��Ϣ
   ��ڲ���: 1: lpMsg: ��Ϣ�ڵ�ָ��
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
s32 sdkSysQueryMessage(SDK_MSG **pstMsg) {
    SDK_MSG *pRet = (SDK_MSG *) sdkSysListGetAt(AppMsgLink, 0);

    if (pRet != NULL) {
        *pstMsg = pRet;
        Verify(sdkSysListRemoveData(AppMsgLink, pRet) >= 0);
        return SDK_OK;
    }
    return SDK_ERR;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ɾ����Ϣ������Ϣ
   ��ڲ���: 1: lpFun: ��Ϣ��Ӧ����ָ��
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
#if 0 /*Modify by ���ٲ� at 2014.03.04  17:9 */
s32 sdkSysTerminateMessage(const void *lpFun)
{
    s32 conter =    sdkSysListGetCnt(AppMsgLink);

    Assert(conter >= 0);

    while(conter--)
    {
        SDK_MSG  * p = (SDK_MSG  *)sdkSysListGetAt(AppMsgLink, conter);

        if((void*)(p->pFun) == lpFun ) //lint !e611
        {
#if 0             //hhh
            p->ucStatus = SDK_SYS_MSG_TERMINAL;
            p->ucPriority = SDK_SYS_MSG_HIGH;
#endif
            Verify(sdkSysListRemoveData(AppMsgLink, p) >= 0);
            Verify(sdkSysListAddAtFront(AppMsgLink, p) >= 0);
            return SDK_OK;
        }
    }

    return SDK_ERR;
}
#endif /* if 0 */

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ʾ��Ϣ����
   ��ڲ���: ��
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
#if 0 /*Modify by ���ٲ� at 2014.03.04  17:7 */
s32  SdkDispMsgList(void)
{
    //XgdDispMsgList(&AppMsgLink);
    return SDK_OK;
}
#endif /* if 0 */

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ֱ�Ӵݻ������Ϣ����
   ��ڲ���: 1: pzHead: �ڴ�����ͷָ��,
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
s32 sdkSysDestroyMsgList(void) {
    SDK_MSG *p;

    while (sdkSysQueryMessage(&p) == SDK_OK) {
        sdkFreeMem(p);
    }

    AppMsgLink = sdkSysListDestroy(AppMsgLink);
    return SDK_OK;
}

#if 0 /*Modify by ���ٲ� at 2014.03.04  17:7 */
s32 sdkDispMemList(void)
{
    return SDK_OK;
}
#endif /* if 0 */

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��֯һ����Ϣ,������
   ��ڲ���: ��Ϣ��Ӧ����
   �� �� ֵ:
   ��    ע:
 ********************************************************************/
s32 sdkSysPackMessage(pMSGFun pFun, u32 uiWParam, u32 uiLParam) {
    typedef u8 (*ptempFun)(SDK_MSG *lpMsg); //Ϊ�˽�����������
    SDK_MSG *pMsg;

    if (pFun == NULL) {
        return SDK_PARA_ERR;
    }
    pMsg = (SDK_MSG *) sdkGetMem(sizeof(SDK_MSG));
    Assert(pMsg != NULL);
    memset(pMsg, 0, sizeof(SDK_MSG));
    pMsg->pFun = (ptempFun) pFun;
    pMsg->uiWParam = uiWParam;
    pMsg->uiLParam = uiLParam;
    return sdkSysPostMessage((SDK_MSG *) pMsg);
}

