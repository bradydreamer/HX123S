/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD
 ******************************************************************************
  �� �� ��   : Queue.h
  �� �� ��   : ����
  ��    ��   : shiweisong
  ��������   : 2013��4��12��
  ����޸�   :
  ��������   : Queue.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��4��12��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __QUEUE_H__
#define __QUEUE_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef void *SDK_QUEUE_HAND;

extern SDK_QUEUE_HAND sdkQueueCreate(s32 sibufferSize);

extern s32 sdkQueueEmpey(SDK_QUEUE_HAND hand);

extern s32 sdkQueueGetData(SDK_QUEUE_HAND hand, u8 *const pOut, const s32 siMaxlen);

extern s32 sdkQueueGetDataLen(SDK_QUEUE_HAND hand);

extern s32 sdkQueueGetFreeSize(SDK_QUEUE_HAND hand);

extern s32 sdkQueueGetMaxSize(SDK_QUEUE_HAND hand);

extern u8 sdkQueueGetPosValue(SDK_QUEUE_HAND hand, s32 pos);

extern s32 sdkQueueInsertData(SDK_QUEUE_HAND hand, u8 const *const pData, const s32 len);

extern s32 sdkQueueIsEmpey(SDK_QUEUE_HAND hand);

extern s32 sdkQueueIsFull(SDK_QUEUE_HAND hand);

extern s32 sdkQueueRemoveRange(SDK_QUEUE_HAND hand, s32 Startpos, s32 EndPos);

extern s32 sdkQueueRelease(SDK_QUEUE_HAND hand);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __QUEUE_H__ */
