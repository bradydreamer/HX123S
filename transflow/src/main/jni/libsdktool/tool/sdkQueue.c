#include "sdkDevOption.h"
#include "sdkGlobal.h" 

#ifndef  DEBUG_QUEUE
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


/*****************************************************************************
** Descriptions:	        �������
** Parameters:          SDK_QUEUE * const pstQueue
** Returned value:
** Created By:		ʯ����2012.07.27
** Remarks:
*****************************************************************************/
s32 sdkClearQueue(SDK_QUEUE *const pstQueue) {
    if (pstQueue == NULL) {
        return SDK_PARA_ERR;
    }
    memset(pstQueue, 0, sizeof(*pstQueue));
    return SDK_OK;
}

/*******************************************************************
   ��	 �� : ������
   ��	 Ȩ : �����¹����ɷ����޹�˾
   ��������: �ж����Ƿ�Ϊ��
   ��ڲ���: Queue: ����ָ��
   �� �� ֵ: true: ���п�, false: ���зǿ�
   ��	  ע: ����֧�ֶ��߳�,�������ݲ�����,���Ա��봫��ָ��
 ********************************************************************/
bool sdkIsQueueEmpty(SDK_QUEUE const *const pstQueue) {
    if (pstQueue == NULL) {
        Assert(0);
        return true;
    }
    return (pstQueue->siHead == pstQueue->siTail);
}

/*******************************************************************
   ��	 �� : ������
   ��	 Ȩ : �����¹����ɷ����޹�˾
   ��������: �ж����Ƿ���
   ��ڲ���: Queue: ����ָ��
   �� �� ֵ: true: ������; false:���з���
   ��	  ע: ����֧�ֶ��߳�,�������ݲ�����,���Ա��봫��ָ��
 ********************************************************************/
bool sdkIsQueueFull(SDK_QUEUE const *const pstQueue) {
    if (pstQueue == NULL) {
        Assert(0);
        return true;
    }

    if (pstQueue->siHead == ((pstQueue->siTail + 1) % SDK_MAX_QUEUE_SIZE)) {
        return true;
    }
    else {
        return false;
    }
}

/*******************************************************************
   ��   �� : ������
   ��   Ȩ : �����¹����ɷ����޹�˾
   ��������: �����ݲ������
   ��ڲ���: Queue: ����ָ��,lpIn: ׼���������,nLen: ׼��������ݸ���
   �� �� ֵ: ʵ�ʲ������ݸ���
   ��    ע: ����֧�ֶ��߳�,�������ݲ�����,���Ա��봫��ָ��
 ********************************************************************/
s32 sdkInsertQueue(SDK_QUEUE *const pstQueue, u8 *phe, s32 siLen) {
    s32 uii;

    if (pstQueue == NULL || phe == NULL || siLen <= 0) {
        return SDK_PARA_ERR;
    }

    for (uii = 0; uii < siLen; uii++) {
        if (!sdkIsQueueFull(pstQueue)) {
            pstQueue->heData[pstQueue->siTail] = phe[uii];
            pstQueue->siTail = (pstQueue->siTail + 1) % SDK_MAX_QUEUE_SIZE;
        }
        else {
            Assert(0);
            break;
        }
    }

    return uii;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_QUEUE *pstQueue
                               u32 uiHead
** Returned value:
** Created By:		ʯ����2012.07.27
** Remarks:
*****************************************************************************/
s32 sdkSetQueueHead(SDK_QUEUE *const pstQueue, s32 siHead) {
    if (siHead >= SDK_MAX_QUEUE_SIZE || NULL == pstQueue || siHead < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    siHead = siHead % SDK_MAX_QUEUE_SIZE;  //lint !e539

    if (!sdkIsQueueEmpty(pstQueue)) {
        if (pstQueue->siHead < pstQueue->siTail) {
            if (siHead >= pstQueue->siHead && siHead <= pstQueue->siTail) {
                pstQueue->siHead = siHead;
            }
            else {
                //������ôŪһ����������ģ�������ǰ��ôŪ������ôŪ��
                //ʯ���� 20120727
                sdkClearQueue(pstQueue);
                return SDK_ERR;
            }
        }
        else {
            if (siHead >= pstQueue->siHead || siHead <= pstQueue->siTail) {
                pstQueue->siHead = siHead;
            }
            else {
                //������ôŪһ����������ģ�������ǰ��ôŪ������ôŪ��
                //ʯ���� 20120727
                sdkClearQueue(pstQueue);
                return SDK_ERR;
            }
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��   �� : ������
   ��   Ȩ : �����¹����ɷ����޹�˾
   ��������: ��ö���ͷ
   ��ڲ���: Queue: ����ָ��
   �� �� ֵ:
   ��    ע: ����֧�ֶ��߳�,�������ݲ�����,���Ա��봫��ָ��
 ********************************************************************/
s32 sdkGetQueueHead(SDK_QUEUE const *const pstQueue) {
    return pstQueue->siHead;
}

/*******************************************************************
   ��   �� : ������
   ��   Ȩ : �����¹����ɷ����޹�˾
   ��������: ���Ի�ȡ����ͷ����,����ɾ����������
   ��ڲ���: Queue: ����ָ��,nHead: ���д�ȡͷ,lpOut: ��ȡ����
   �� �� ֵ: true: ���Գɹ�, false: ��������Ч����
   ��    ע: ����֧�ֶ��߳�,�������ݲ�����,���Ա��봫��ָ��
 ********************************************************************/
bool sdkTryQueueData(SDK_QUEUE *pstQueue, s32 siHead, u8 *pucOut) {
    siHead = siHead % SDK_MAX_QUEUE_SIZE;

    if (!sdkIsQueueEmpty(pstQueue)) {
        if (pstQueue->siHead < pstQueue->siTail) {
            if (siHead >= pstQueue->siHead && siHead < pstQueue->siTail) {
                *pucOut = pstQueue->heData[siHead];
                return true;
            }
        }
        else {
            if (siHead >= pstQueue->siHead || siHead < pstQueue->siTail) {
                *pucOut = pstQueue->heData[siHead];
                return true;
            }
        }
    }
    return false;
}


