/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, XGD

******************************************************************************
   �� �� ��   : sdkSys.h
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2012��6��15�� ������
   ����޸�   :
   ��������   : sdkSys.h ��ͷ�ļ�
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��6��15�� ������
    ��    ��   : shiweisong
    �޸�����   : �����ļ�

******************************************************************************/

/******************************************************************************

			 COPYRIGHT (C), 2001-2011, XGD

******************************************************************************
FILE NAME   : sdkSys.h
VERSION   : First Draft
CREATED BY : shiweisong
CREATION DATE : Friday, June 15, 2012
RECENT MODIFICATION: Create files
FUNCTION   : the head file of sdkSys.h
FUNCTIONS LIST  :
MODIFICATION LOG   :
******************************************************************************/



#ifndef __SDKSYS_H__
#define __SDKSYS_H__


#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#define         SDK_SYS_MSG_OUT             0                       //����Ϣ������Ϻ�Ӷ������//clear this message from queue if it has been processed
#define         SDK_SYS_MSG_KEEP            1                        //����Ϣ������Ϻ��ƶ�������β��//move this message to the queue end if it has been processed
#define         SDK_SYS_MSG_DISCARD         2                       //��������//don't handle


typedef struct TagSDK_MSG                                           //��Ϣ���ƽڵ�//message mechanism nodes
{
    u8 ucPriority;                                                  //��Ϣ���ȼ�//message priority
    u8 ucStatus;                                                    //��Ϣ״̬//message status
    u8 (*pFun)(
            struct TagSDK_MSG *pstMsg);                          //��Ϣ��ں���//message entrance function
    u32 uiWParam;                                                   //��Ϣ�߲���//message high parameters
    u32 uiLParam;                                                   //��Ϣ�Ͳ���//message low parameters
} SDK_MSG;


typedef u8 (*pMSGFun)(struct TagSDK_MSG *pstMsg);           //��Ϣ��ں���//massage entrance function

typedef enum {
    SDK_SYS_SHELL_RUN_SCRIPT = (-4),      //���нű�ʧ��//run script failed
    SDK_SYS_SHELL_FAILLED_IN_RUN_SCRIPT = (-3),      //�ű����й�����ʧ��//failed while script running
    SDK_SYS_SHELL_PARA_NULL = (-2),      //�մ���//null pointer
    SDK_SYS_SHELL_SYS_ERR = (-1),      //ϵͳ����//system error
    SDK_SYS_SHELL_OK = (1)        //�ɹ�//success
} SDK_SYS_SHELL_RET;

typedef enum {
    SDK_SYS_BEEP_OK,                                                //��������//beep normal
    SDK_SYS_BEEP_ERR,                                              //��������//beep error
    SDK_SYS_BEEP_INPUT_PIN,                                    //��������������
    SDK_SYS_BEEP_RING_1,
    SDK_SYS_BEEP_RING_2,
    SDK_SYS_BEEP_RING_3,
    SDK_SYS_BEEP_USER_1,
    SDK_SYS_BEEP_USER_2,
    SDK_SYS_BEEP_USER_3,
    SDK_SYS_BEEP_USER_4,
    SDK_SYS_BEEP_USER_5,
    SDK_SYS_BEEP_USER_6,
    SDK_SYS_BEEP_READ_CARD,                             //��ˢ��                 
    SDK_SYS_BEEP_READ_CARD_AGAIN,                 //���ٴ�ˢ��        
    SDK_SYS_BEEP_INPUT_AMOUNT,                       //��������
    SDK_SYS_BEEP_INPUT_AMOUNT_AGAIN,           //���ٴ�������        
    SDK_SYS_BEEP_INPUT_OR_READ_CARD,           //��������߶���
    SDK_SYS_BEEP_INPUT,                                      //������
    SDK_SYS_BEEP_CONFIRM,                                 //��ȷ��
    SDK_SYS_BEEP_SELECT,                                    //��ѡ��
    SDK_SYS_BEEP_INSERT_CARD,                         //��忨
    SDK_SYS_BEEP_INSERT_OR_RD_CARD           // ��忨����ˢ��
} SDK_SYS_BEEP_TYPE;

typedef struct {
    bool bIsHavePinPad; //�Ƿ���������//whether with PIN pad
    bool bIsINRf;       //�Ƿ���ǽ�//whether with contactless
} SDK_SYS_INITIAL_INFO;

/*  */
#define SDK_DOWN_MODE_MEM           0x00000000      //ͨ�������ڴ������̼乲����Ϣ��Ĭ�ϣ�
#define SDK_DOWN_MODE_FILE          0x00000001      //ͨ���ļ������̼乲����Ϣ
/* ���ز��� */
#define SDK_DOWN_MODE_LINK_OK       0x00000010      //����wifi\����ͨѶʱ��ָʾ��·�Ѿ������ɹ�������ֱ��ʹ��
/*���ؿ��Ʋ���*/
#define SDK_DOWN_MODE_NETANTS       0x00000100      //������Ҫ�ϵ�����
#define SDK_DOWN_MODE_VERCOMP       0x00000200      //������Ҫ�Ƚϰ汾, ���°汾������
#define SDK_DOWN_MODE_BACKUPAPP     0x00000400      //�Ƿ񱸷�Ӧ�ó���
#define SDK_DOWN_MODE_DELAPPS       0x00000800      //�����ļ�ǰ�Ƿ���Ҫɾ��ȫ��Ӧ��


typedef struct {
    s32 siStatus;
    /* ���ص�״̬: 1-��ʼ���أ�2-���سɹ���3-����ʧ��...*/
    u8 uiType;
    /* ���صķ�ʽ: 1-����(����cdc usb)��2-socket(����wifi��GPRS������)��3-����...*/
    u32 uiFlags;
    /* ���صı�־λ���Ա���չ */
    u8 uiComNo;
    /* ���ں� */
    u32 uiComBaud;
    /* ���ڵĲ����� */
    u8 asServerIP[16];
    /* ������IP��ַ */
    u8 asPort[6];
    /* ����˿ں� */
    s32 siAntTime;
    /* �ϵ�������ʱЧ��������ʱ��ֵ����Ҫ�������ء���λΪ���� */
    s32 siTimeOut;
    /* ���ع����У�ͨѶ��ʱʱ�� */
    u8 uiReserve[128];
    /* Ԥ�� */
    void *pVar;               /* Ԥ��ָ��*/
} SDK_DOWNLOAD_MODE;

enum SDK_SYS_COM_TYPE {
    SDK_SYS_COM_PC = 0,                   // pc��//PC com port
    SDK_SYS_COM_PIN_PAD = 1,               // ���̽ӿ�//PIN pad interface
    SDK_SYS_COM_SCANNER = 2,                // ɨ��ǹ��
    SDK_SYS_COM_USB = 3,                    // usb��
    SDK_SYS_COM_ERR                         // ����������//used for checking error
};

#define SDK_SYS_LOWPOWER                    SDK_SYS_RET_OK_MIN+1    //��ص�ѹ��

enum SDK_SYS_COM_NET_TYPE {
    SDK_SYS_COMMU_ALL = 0,                // ���ж�֧��//support all
    SDK_SYS_COMMU_LAN = 1,                 // ���̽ӿ�//keyboard interface
    SDK_SYS_SOMMU_WIRELESS = 2,             // ֧������//support wireless
    SDK_SYS_SOMMU_ERR                      // ����������//used for checking error
};


enum SDK_SYS_SCREEN_TYPE {
    SDK_SYS_SCREEN_128H64 = 0,            // 128x64����//128x64 pixels
    SDK_SYS_SCREEN_320H240 = 1,             // 320*240����        K360ϵ��  //320*240 pixels    terminal K360 series
    SDK_SYS_SCREEN_800H480 = 2,             // 800*480����        K390ϵ��  //800*480 pixels   terminal K390 series
    SDK_SYS_SCREEN_CHECK_ERR = 3,           // ����������//used for checking error
    SDK_SYS_SCREEN_240H320 = 4              // 240*320����        G870ϵ��  //240*320 pixels    terminal G870 series//shijianglong 2013.01.10 15:50
};

typedef enum {
    SDK_MACHINE_PLAM_UC = 0,
    SDK_MACHINE_PLAM_K = 1,
    SDK_MACHINE_PLAM_P = 2
} SDK_MACHINE_PLAM;


/**********************************************************************
  ��׼����Standard Function
 ***********************************************************************/
extern s32 sdkSysMain(int argc, const char **argv);

extern s32 sdkSysInitParam(const SDK_SYS_INITIAL_INFO *pstInital);

extern void sdkSysAppQuit(void);

extern bool sdkSysIsInSimulator(void);

extern void sdkSysEnableBeep(bool bIsBeep);

extern bool sdkSysGetBeepState(void);

extern bool sdkSysBeep(SDK_SYS_BEEP_TYPE eType);

extern s32 sdkSysGetFileVer(const u8 *fpFile, const u8 *pTag, u8 *lpOut);

/*****************************�����ӿں�������͸��Other functions call these functions****************************/
extern s32 sdkSysGetCurFifoNo(void);

extern SDK_SYS_SHELL_RET sdkSysCmd(const u8 *pasCmd, s32 *puiRetCode);

extern s32 sdkSysDestroyMsgList(void);

extern bool sdkSysIsRfIn(void);

extern s32 sdkSysSwitchApp(const u8 *pasAppId, const u8 *pasSrc, s32 siSrcLen, u8 *pasDest);

//shijianglong 2013.02.20 10:22
extern s32 sdkSysTravelManage(s32 siPosPTrigger, const u8 *pasSrc, s32 siSrcLen, u8 *pasDest);

//shijianglong 2013.02.25 9:57
extern s32 sdkSysPackMessage(pMSGFun pfun, u32 uiWParam, u32 uiLParam);

extern s32 sdkSysQueryMessage(SDK_MSG **ppstMsg);

extern s32 sdkSysPostMessage(SDK_MSG *pstMsg);

extern s32 sdkSysGetCurAppId(char *pasData);

extern s32 sdkSysGetCurAppDir(u8 * pasData);

extern s32 sdkSysGetCurAppState(void);

extern s32 sdkSysGetAppMaxNum(void);

extern bool sdkSysIsFroceOut(void);

extern s32 sdkSysReInitParam(const SDK_SYS_INITIAL_INFO *pstInital);

extern s32 sdkSysGetLibdevVersion(u8 * pasVer);
extern s32 sdkSysGetPanelVersion(u8 * pasVer);
extern s32 sdkSysGetDaemonVersion(u8 * pasVer);
extern s32 sdkSysGetLibxgdemvVersion(u8 * pasVer);
extern s32 sdkSysGetLibappdeviceVersion(u8 * pasVer);
extern s32 sdkSysGetLibMahtsVersion(u8 * pasVer);
extern s32 sdkSysGetLibModuleVersion(u8 * pasVer);
extern s32 sdkSysGetManageVersion(u8 * pasVer);
extern s32 sdkSysGetLibKernelVersion(u8 * pasVer);
extern s32 sdkSysGetHandsetVersion(u8 * pasVer);
extern s32 sdkSysGetBaseVersion(u8 * pasVer);
extern s32 sdkSysGetDynaloadVer(u8 * lpOut);

extern s32 sdkSysPlaySoundFile(const u8 *pFile, s32 level);

extern int SdkSysDownloadMode(SDK_DOWNLOAD_MODE *pstDown);


extern s32 sdkSysGetComNo(const enum SDK_SYS_COM_TYPE eComtype);

// �õ�mis���ں�//get mis com port number
extern s32 sdkSysGetMisComNo(bool bIsSupportPinPad);

// �Ƿ���Ĭ���������//whether default have PIN pad
extern bool sdkSysIsHavePinPad(void);

// �õ������ַ���    �����ַ�������-1��ʾ��������//get starting up character string, return character string length, -1 means error
extern s32 sdkSysGetMachineModel(u8 * pasStr);

extern s32 sdkSysStlGetMachinemodelVersion(u8 * pasMachineversion);

extern s32 sdkSysGetMachineCode(u8 * pasCode);

// �Ƿ������//whether have base
extern bool sdkSysIsHaveBase(void);

// �Ƿ��� uclinxƽ̨//is uclinux platform or not
extern bool sdkSysIsUclinuxPlat(void);

// �Ƿ����//is color screen or not
extern bool sdkSysIsColorScreen(void);

extern bool sdkSysIsHaveSpeaker(void);

// �Ƿ����Ӵ�//is stylus printer or not
extern bool sdkSysIsPrintPin(void);

// �õ�֤��//get certificate
extern s32 sdkSysGetCupSn(u8 * pheSn);

// �õ�����֧��ͨ����������//get communication mask supported by machine

extern enum SDK_SYS_COM_NET_TYPE sdkSysGetSupportCommu(void);

extern bool sdkIsSupportPCI(void);

extern bool sdkSysIsSupportTSlib(void);

extern enum SDK_SYS_SCREEN_TYPE sdkSysGetScreenPixel(void);

extern s32 sdkSysGetSwitchAppId(const u8 *pasIdIn, u8 *pasIdOut);

extern bool sdkSysIsMultiplexUpDown(void);

extern s32 sdkSysGetAppReturnWelcome(const s32 siDeflautTimer);

extern s32 sdkSysGetSreenMaxLine(void);

extern s32 sdkSysGetSreenCharCntPerLine(void);

/*=======BEGIN: huangbx20130117 modify===========*/
extern u8 sdkSysGetModemType(void);

/*==================== END ======================*/
extern s32 sdkSysGetPowerState(void);

extern s32 sdkSysEnterIdle(bool bFlag, u8 uOption);

extern bool sdkSysIsExPower(void);

extern s32 sdkSysSetBlackLightCloseTime(s32 siDlyTime);

extern s32 sdkSysSetBeepTune(s32 siKeyFreq, s32 siKeyAtio);

extern s32 sdkSysSetLcdContrast(s32 siLightLev);

extern void sdkSysBlackLightOn(void);

extern void sdkSysBlackLightOff(void);

extern void sdkSysBlackLightCtro(bool isEnCtro);

extern s32 sdkSysGetBatteryStateAndControl(void);

extern s32 sdkSysGetVolume(void);

extern s32 sdkSysSetVolume(u32 uiVolume);

extern s32 sdkSysGetMaxLetterPerLine(void);

extern s32 sdkSysGetMachinePlam(void);

extern s32 sdkSysSetCurAppId(char *pasData);//zhouxiaoxin

//����������������������ʼ����̬���صĲ�Ҫ���뵽��//the following two functions are used to initialize dynamic loading, don't add them to dynamic loading list
//̬�����б���//shiweisong 2012.11.14 16:32
s32 sdkSysUnLoadDll(void);

s32 sdkSysLoadDll(u8 *pasErr, s32 siSize);

s32 sdkExtLoadDll(u8 *pasErr, s32 siSize);

s32 sdkExtUnLoadDll(void);
/**********************************************************************
   hook ��ش���  hook related code
 ***********************************************************************/


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif /* __SDKSYS_H__ */

