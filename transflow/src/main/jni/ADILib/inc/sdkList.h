/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, xgd

 ******************************************************************************
  �� �� ��   : sdkList.h
  �� �� ��   : ����
  ��    ��   : shiweisong
  ��������   : 2012��12��27��
  ����޸�   :
  ��������   : sdkList.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��12��27��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __SDKLIST_H__
#define __SDKLIST_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef void *SDK_SYS_LIST_HAND; //shiweisong 2012.12.27 18:3 �б���

extern s32 sdkSysListAddAtFront(SDK_SYS_LIST_HAND pHand, void *const pData);

extern s32 sdkSysListAddAtEnd(SDK_SYS_LIST_HAND pHand, void *const pData);

extern SDK_SYS_LIST_HAND sdkSysListCreate(void);

extern SDK_SYS_LIST_HAND sdkSysListDestroy(SDK_SYS_LIST_HAND pHand);

extern void *sdkSysListGetAt(SDK_SYS_LIST_HAND pHand, s32 siIndex);

extern s32 sdkSysListGetCnt(SDK_SYS_LIST_HAND pHand);

extern s32 sdkSysListRemoveAt(SDK_SYS_LIST_HAND pHand, s32 siIndex);

extern s32 sdkSysListRemoveData(SDK_SYS_LIST_HAND pHand, void const *pData);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SDKLIST_H__ */
