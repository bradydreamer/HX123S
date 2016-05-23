/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

 ******************************************************************************
  �� �� ��   : sdkSysHook.h
  �� �� ��   : ����
  ��    ��   : shiweisong
  ��������   : 2013��5��16��
  ����޸�   :
  ��������   : sdkSysHook.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��5��16��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

#ifndef __SDKSYSHOOK_H__
#define __SDKSYSHOOK_H__


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

typedef enum {
    SDK_SYS_NEXT_HOOK,  //�Ÿ���һ��hook   //give next hook
    SDK_SYS_END_HOOK,   //hook �����治��Ҫ�ٴ�����//don't handle if hook
    SDK_SYS_FORCE_OUT   //ǿ���˳���
} SDK_SYS_HOOK_RET;

typedef enum {
    SDK_SYS_HOOK_FIFO,     //ϵͳ�ڲ�hook Ӧ���޲����˽�
    SDK_SYS_HOOK_COM_REV,  //���ڽ���hook
    SDK_SYS_HOOK_COM_SEND, //���ڽ��� ���� hook
    SDK_SYS_HOOK_KEY,       //����hook
    SDK_SYS_HOOK_ERR         //hook �����治��Ҫ�ٴ�����//don't handle if hook
} SDK_SYS_HOOK_ID;

typedef SDK_SYS_HOOK_RET (*SDK_SYS_COM_FIFO_HOOK)(void *const pFifo);

typedef SDK_SYS_HOOK_RET (*SDK_SYS_COM_REV_HOOK)(s32 siComID, u8 *const pheData, s32 *const siLen);

typedef SDK_SYS_HOOK_RET (*SDK_SYS_COM_SEND_HOOK)(s32 siComID, u8 *const pheData, s32 *const siLen);

typedef SDK_SYS_HOOK_RET (*SDK_SYS_KEY_HOOK)(s32 * const
siKey);

extern s32 sdkSysSetHook(SDK_SYS_HOOK_ID eID, void *pFun);

extern s32 sdkSysUnHook(SDK_SYS_HOOK_ID eID);


extern void *sdkSysGetHookFun(SDK_SYS_HOOK_ID eID);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SDKSYSHOOK_H__ */

