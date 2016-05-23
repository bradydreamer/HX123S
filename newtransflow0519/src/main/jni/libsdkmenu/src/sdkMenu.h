/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : Sysmemu.h
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2012��7��17�� ���ڶ�
   ����޸�   :
   ��������   : Sysmemu.c ��ͷ�ļ�
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��7��17�� ���ڶ�
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/


#ifndef __SDK_MENU_H__
#define __SDK_MENU_H__

#include "sdkGlobal.h"
#include "sdkUI.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SDK_MENU_MAX_ITEM   10              //��ǰ�˵�����0~9
typedef struct _ITEM {
    u8 *pText;                              //��ʾ���ַ���
    s32 Key;                                //�󶨵�key
    s32 ID;                                 //�˵�ID
    MenuCallBackFun pFun;

} ITEM;

typedef struct _MENU {
    u8 pTitle[256];                  //����
    SDK_UI_SHOW_MODE Mode;          //��ʾģʽ

    s32 siPageNo;                   //��ǰҳ��
    s32 siID;                       //��ǰ�˵��ڵ���
    s32 siNoDisp;                   //��ǰ�����±�
    s32 siMaxTile;                  //ÿ��ģʽ�µ����ѭ���˵�����

} Menu;


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SYSMEMU_H__ */

