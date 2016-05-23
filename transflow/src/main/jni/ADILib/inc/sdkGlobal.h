#ifndef AFX_SDK_GLOBAL_H
#define AFX_SDK_GLOBAL_H


#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

//#include <Winnls.h>
//#include <Wtypes.h>
//#include <Windows.h>




#define EXTEND_SDK                               //�������˿�����������չsdk���������ɻ���sdk  //if define this switch then generate extended sdk, or generate basic sdk
//#undef EXTEND_SDK                              

//���л��ͱ��붨�� 
#define SDK_SYS_MACHINE_K301FZ      0x04
#define SDK_SYS_MACHINE_K301FZE     0x14
#define SDK_SYS_MACHINE_K301P       0x24
#define SDK_SYS_MACHINE_K320        0x11
#define SDK_SYS_MACHINE_K320P       0x1A
#define SDK_SYS_MACHINE_K350        0x05
#define SDK_SYS_MACHINE_K350P       0x15
#define SDK_SYS_MACHINE_K360        0x06
#define SDK_SYS_MACHINE_K360P       0x16
#define SDK_SYS_MACHINE_K370        0x07
#define SDK_SYS_MACHINE_K370P       0x17
#define SDK_SYS_MACHINE_K390        0x09
#define SDK_SYS_MACHINE_K390P       0x19


#define SDK_SYS_MACHINE_K810        0x0D
#define SDK_SYS_MACHINE_K820        0x0C
#define SDK_SYS_MACHINE_K820P       0x1C
#define SDK_SYS_MACHINE_G810        0x42
#define SDK_SYS_MACHINE_G300        0x40
#define SDK_SYS_MACHINE_G860        0x46
#define SDK_SYS_MACHINE_G870        0x47
#define SDK_SYS_MACHINE_G870D       0x4B
#define SDK_SYS_MACHINE_V70D        0x4C
#define SDK_SYS_MACHINE_G891        0x49
#define SDK_SYS_MACHINE_K632        0xA0
#define SDK_SYS_MACHINE_K636        0xA1
#define SDK_SYS_MACHINE_K628        0xA2
#define SDK_SYS_MACHINE_K658        0xA3
#define SDK_SYS_MACHINE_G3          0x31

#define SDK_SYS_MACHINE_K501AM      0x23 //huacong 2013.03.25 10:57
#define SDK_SYS_MACHINE_K508A       0x70
#define SDK_SYS_MACHINE_G2          0x99
#define SDK_SYS_MACHINE_200P        0x98

#define SDK_IS_UCOS()    ((MACHINEMODEL == SDK_SYS_MACHINE_K501AM) || (MACHINEMODEL == SDK_SYS_MACHINE_K508A) || (MACHINEMODEL == SDK_SYS_MACHINE_K301FZ)||(MACHINEMODEL == SDK_SYS_MACHINE_K301FZE) || (MACHINEMODEL == SDK_SYS_MACHINE_G2) || (MACHINEMODEL == SDK_SYS_MACHINE_200P))
#define SDK_IS_K_PLAM() ((MACHINEMODEL == SDK_SYS_MACHINE_K320) || (MACHINEMODEL == SDK_SYS_MACHINE_K360) || (MACHINEMODEL == SDK_SYS_MACHINE_K370)  || (MACHINEMODEL == SDK_SYS_MACHINE_K390) || (MACHINEMODEL == SDK_SYS_MACHINE_K820))
#define SDK_IS_P_PLAM()  (!(SDK_IS_K_PLAM() || SDK_IS_UCOS()))
#define SDK_IS_DDI_PLAM() (MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P)

#define SDK_SYS_VER   "2.04"  //��ʱ�汾����ʽ����ʱ��Tempȥ��//temporary version, the Temp will be get rid of when the official version is released
//�������� ����Ĵ��� �����Լ�����//if parameters error, user can define specific error by themselves
//���û �б�Ҫ ��ֱ�������//if unnecessary then use the following 

#define  SDK_APP_RET_OK_MIN          (10000)    //�û��Զ���ش������//user-defined must be bigger than this

#define  SDK_SYS_RET_OK_MIN          (100)      //sdk�ڲ��Զ���ش������//sdk interior user-defined must be bigger than this
#define  SDK_SYS_CONTINUE            (SDK_SYS_RET_OK_MIN + 1)
#define  SDK_SYS_NEXT                (SDK_SYS_CONTINUE + 1)

#define  SDK_ICC_EMV_OK_BASE         (SDK_SYS_RET_OK_MIN + 100)
#define  SDK_COMM_OK_BASE            (SDK_ICC_EMV_OK_BASE + 200)
#define  SDK_PRINT_OK_BASE           (SDK_COMM_OK_BASE + 200)


#define SDK_OK                       (1)
#define SDK_EQU                      (0)        //�Ƚϴ�С���ʱ�����//use this if it's equal when compare size
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)       //��ʱ//timeout
#define SDK_ESC                      (-3)
#define SDK_PARA_ERR                 (-4)
#define SDK_FUN_NULL                 (-5)       //������Ϊ��
/*=======BEGIN: tjb 2014.06.03  14:52 modify===========*/
#define SDK_EBUSY                    (-6)       /* �豸æ */
#define SDK_EIO                      (-7)       /* �豸δ�� */
#define SDK_EDATA                    (-8)       /* ������������ݴ��� */
/*====================== END======================== */

#define SDK_SYS_RET_ERR_MAX          (-100)     //sdk�ڲ��Զ���������С�����//sdk interior user-defined error must be smaller than this
#define SDK_ICC_EMV_ERR_BASE         (SDK_SYS_RET_ERR_MAX - 100)     //(-200)
#define SDK_DISP_ERR_BASE            (SDK_ICC_EMV_ERR_BASE - 200)
#define SDK_APP_RET_ERR_MAX          (-10000)   //�û��Զ����С�����//user-defined must be smaller than this

#ifdef __GNUC__
#define ATTRIBUTE __attribute__((packed))
#endif

#ifdef __IAR_SYSTEMS_ICC__
#pragma pack(1)
#define ATTRIBUTE
#ifdef __SDK__
#include "filebase.h"
#endif
#endif

#ifdef __ADS__
#define ATTRIBUTE
//#include "filebase.h"//lk 2014.05.09 9:11
#endif


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


#if(!SDK_IS_UCOS())

#define ATTRIBUTE  __attribute__((packed))

#include <unistd.h>

#ifdef  IS_BASE_SDK
#include <time.h> 
#include "libdev.h"            
#include "libxgdmaths.h"
#include "libxgdmodule.h"
#include "libxgdkernel.h"
#include "dllemv.h"
#endif //endif IS_BASE_SDK
#endif

#include "sdkTypeDefine.h"

#include "sdkSysLanguage.h"
#include "sdkDebug.h"

#include "sdkPed.h"

#include "sdkmaths.h"
#include "sdkfile.h" //zhouzhihua 2013.06.13 15:53


#include "sdktools.h"
#include "sdkSysMem.h"

#include "sdkPrint.h"
#include "sdkSys.h"
#include "sdkComm.h"

#include "sdkDisp.h"
#include "sdkkey.h"
#include "sdkmmi.h"

#include "sdkmag.h"
#include "sdkicdeal.h"
#include "sdkemvapp.h"
#include "sdkqreader.h"

#include "sdkpromptbox.h"

#include "sdkQueue.h"

#include "sdkScanner.h"
#include "sdkIni.h"
#include "sdkList.h"
#include "sdkQueue2.h"

#include "sdkPrivateHead.h"
#include "sdkSysHook.h"


#define MACHINEMODEL SDK_SYS_MACHINE_200P//sujianzhong 2015.05.20 11:3

//��ֹ�е�ͬ���ô���assert   //avoid some workmates use assert incorrectly
#ifdef assert
#undef  assert
#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif


