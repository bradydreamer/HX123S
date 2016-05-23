#include "sdkDevOption.h"
#include "sdkGlobal.h"

#ifndef  DEBUG_HOOK
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

//�������ݻ�����нṹ
typedef struct {
    s32 siPos;
    s32 siMaxSize;
    u8 Data[1];
} SDK_QUEUE_STRUCT;


bool IsQueueOK(SDK_QUEUE_HAND hand) {
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    return hand != NULL && pSt->siPos >= 0 && pSt->siMaxSize >= 20 && pSt->siPos <= pSt->siMaxSize;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 sibufferSize
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
SDK_QUEUE_HAND sdkQueueCreate(s32 sibufferSize) {
    SDK_QUEUE_STRUCT *pSt;

    if (sibufferSize < 20) {
        Assert(0);

        return NULL;
    }
    pSt = (SDK_QUEUE_STRUCT *) sdkGetMem((s32)
    sizeof(SDK_QUEUE_STRUCT) + sibufferSize +
    2); //������������������������//shiweisong 2013.04.12 15:6

    if (pSt != NULL) {
        memset(pSt, 0, (sizeof(SDK_QUEUE_STRUCT) + (u32)(sibufferSize + 2))); //���ϳ�ʼ��Ϊ��
        pSt->siMaxSize = sibufferSize;
        pSt->siPos = 0;
    }
    Assert(IsQueueOK(pSt));

    return pSt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueGetMaxSize(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);

        return SDK_PARA_ERR;
    }
    Assert(IsQueueOK(hand));
    return ((SDK_QUEUE_STRUCT *) hand)->siMaxSize;
}   //lint !e818

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueGetDataLen(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);

        return SDK_PARA_ERR;
    }
    Assert(IsQueueOK(hand));
    return ((SDK_QUEUE_STRUCT *) hand)->siPos;
}//lint !e818

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueIsEmpey(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Assert(IsQueueOK(hand));
    return ((SDK_QUEUE_STRUCT *) hand)->siPos == 0 ? SDK_OK : SDK_ERR;
}//lint !e818

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueEmpey(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Assert(IsQueueOK(hand));
    ((SDK_QUEUE_STRUCT *) hand)->siPos = 0;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueIsFull(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return ((SDK_QUEUE_STRUCT *) hand)->siPos == ((SDK_QUEUE_STRUCT *) hand)->siMaxSize ? SDK_OK
                                                                                        : SDK_ERR;
}//lint !e818

s32 sdkQueueGetFreeSize(SDK_QUEUE_HAND hand) {
    if (hand == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return ((SDK_QUEUE_STRUCT *) hand)->siMaxSize - ((SDK_QUEUE_STRUCT *) hand)->siPos;
}//lint !e818

/***********************************************************
* Created on:2012-3-7
* Author: shiweisong
* description���Ƴ��б��������� Startpos���������� EndPos����������֮�������Ԫ��
* Input enclosing_method_arguments
* return < 0 ʧ��
* return >= 0 ������ʣ���������
***********************************************************/
s32 sdkQueueRemoveRange(SDK_QUEUE_HAND hand, s32 Startpos, s32 EndPos) {
    s32 dataLen;

#if 0 /*Modify by ���ٲ� at 2013.04.14  19:31 */
    u8 buffer[1024] = {0};
#endif /* if 0 */
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    //�����Ŵ�

    if (IsQueueOK(hand) == false) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (Startpos >= EndPos) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    dataLen = sdkQueueGetDataLen(hand);

    if ((dataLen <= Startpos) || (dataLen < EndPos)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    {
        s32 len = sdkQueueGetDataLen(hand);
#if 1 /*Modify by ���ٲ� at 2013.04.14  15:11 */
        u8 *buffer = (u8 *) sdkGetMem(len);

        if (buffer == NULL) {
            FinalErr("�ڴ����");
            return SDK_ERR;
        }
#endif /* if 0 */

        memcpy(buffer, pSt->Data, (u32) len); //lint !e670   //ȫ�����ݰ���
        memcpy(pSt->Data, buffer, (u32) Startpos);//lint !e669    //����ǰ�������
        memcpy(&pSt->Data[Startpos], &buffer[EndPos],
               (u32)(pSt->siPos - EndPos));//lint !e676 //���˺��������
        pSt->siPos -= (EndPos - Startpos);
#if 1 /*Modify by ���ٲ� at 2013.04.14  15:11 */
        //Verify(xgd_free(buffer) >= 0);
        sdkFreeMem(buffer);
#endif /* if 0 */
    }
    return pSt->siPos;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
                     s32 pos   λ ��  0��ʼ
** Returned value:	����������·��ص��� �������ֵ���ڲ������� ����·��ص���0
                     �������������ʱ��Ҫ�ر�С�ģ�
                     �������� ���������޷����ص�
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
u8 sdkQueueGetPosValue(SDK_QUEUE_HAND hand, s32 pos) {
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    //�����Ŵ�
    if (IsQueueOK(hand) == false || pos >= sdkQueueGetDataLen(hand)) {
        Assert(0);
        return 0;
    }
    return pSt->Data[pos];//lint !e676
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
                               u8 const * const pData
                               const s32 len
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueInsertData(SDK_QUEUE_HAND hand, u8 const *const pData, const s32 len) {
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    if (len < 0 || IsQueueOK(hand) == false) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    // ����һ�¿����Ƿ����ѹ�ý�ȥ shiweisong 2013.04.12 15:46
    if (sdkQueueGetMaxSize(hand) - sdkQueueGetDataLen(hand) >= len) {
        memcpy(&pSt->Data[pSt->siPos], pData, len);
        pSt->siPos += len;
        //��ֹ��������
        return pSt->siPos;
    }
    return SDK_PARA_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
                    u8 * const pOut
                    const s32 Maxlen  �ϲ����bufferֵ ��ֹ��������̫������ϲ��ڴ����
** Returned value:
** Created By:		shiweisong  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueGetData(SDK_QUEUE_HAND hand, u8 *const pOut, const s32 siMaxlen) {
    s32 Retlen;
    s32 Qlen;
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    //�����Ŵ�

    if (IsQueueOK(hand) == false || pOut == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkQueueIsEmpey(hand) == SDK_OK) {
        return 0;
    }
    Qlen = sdkQueueGetDataLen(hand);
    Retlen = siMaxlen > Qlen ? Qlen : siMaxlen;
    memcpy(pOut, pSt->Data, Retlen);//lint !e671 !e670
    return Retlen;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE_HAND hand
** Returned value:
** Created By:		���ٲ�  2013.04.12
** Remarks:
*****************************************************************************/
s32 sdkQueueRelease(SDK_QUEUE_HAND hand) {
    SDK_QUEUE_STRUCT *pSt = (SDK_QUEUE_STRUCT *) hand;

    if (IsQueueOK(hand) == false) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    pSt->siMaxSize = -20; //��ֹ�ͷ������ʹ��
    sdkFreeMem(hand);
    return SDK_OK;
}

