/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

 ******************************************************************************
  �� �� ��   : Queue.h
  �� �� ��   : ����
  ��    ��   : ʯ����
  ��������   : 2012��7��27�� ������
  ����޸�   :
  ��������   : sdkQueue.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��7��27�� ������
    ��    ��   : ʯ����
    �޸�����   : �����ļ�

******************************************************************************/

/******************************************************************************

			 COPYRIGHT (C), 2001-2011, XGD

******************************************************************************
FILE NAME   : Queue.h
VERSION   : First Draft
CREATED BY : shiweisong
CREATION DATE : Friday, July 27, 2012
RECENT MODIFICATION: Create files
FUNCTION   : the head file of sdkQueue.c
FUNCTIONS LIST  :
MODIFICATION LOG   :
******************************************************************************/


#ifndef __sdkQUEUE_H__
#define __sdkQUEUE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#define SDK_MAX_QUEUE_SIZE              (4096)                                //���г���//queue size

typedef struct                                                                                    //ͨ���Ͷ���//popular type queue
{
    s32 siHead;
    s32 siTail;
    u8 heData[SDK_MAX_QUEUE_SIZE];
} SDK_QUEUE;

extern s32 sdkGetQueueHead(SDK_QUEUE const *const pstQueue);

extern s32 sdkClearQueue(SDK_QUEUE *const pstQueue);

extern s32 sdkInsertQueue(SDK_QUEUE *const pstQueue, u8 *phe, s32 siLen);

extern bool sdkIsQueueEmpty(SDK_QUEUE const *const pstQueue);

extern bool sdkIsQueueFull(SDK_QUEUE const *const pstQueue);

extern bool sdkTryQueueData(SDK_QUEUE *pstQueue, s32 siHead, u8 *pucOut);

extern s32 sdkSetQueueHead(SDK_QUEUE *const pstQueue, s32 siHead);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __QUEUE_H__ */
