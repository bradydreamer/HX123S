#include "sdkdevOption.h"
#include "sdkGlobal.h"
//#include<unistd.h>

#ifndef  DEBUG_SYS
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


typedef enum {
    FIRMWAREVER_BOOT1 = 0, //��boot1(��Ƭ��boot)��һ��boot
    FIRMWAREVER_BOOT2 = 1, //��boot2������boot
    FIRMWAREVER_KERNEL = 2, //��kernel(��Ƭ��core)
    FIRMWAREVER_ROOTFS = 3, //: rootfs
    FIRMWAREVER_FSIMING = 4, //��fsiming
    FIRMWAREVER_LIBDEV = 5, //: libdev
    FIRMWAREVER_PANNEL = 6, //: panel.so
    FIRMWAREVER_HL = 7, //: hl.bin
    FIRMWAREVER_TE = 8, //: te.so
} SDK_SYS_FIRMWAREVER_TYPE;

typedef enum {
    MODULE_MAGCARD = 0, //���ſ�
    MODULE_ICCCARD = 1, //��IC��
    MODULE_RFCARD = 2, //���ǽӿ�
    MODULE_PRINTER = 3, //: ��ӡ��
    MODULE_AUDIO = 4, //����Ƶ
    MODULE_INSIDEPED = 5, //: ������Կ
    MODULE_FILESYS = 6, //: �����ļ�ϵͳ
    MODULE_BT = 7, //: ����
    MODULE_AUDIOCOMM = 8, //: ��ƵͨѶ
    MODULE_LED = 9, //: LED��
    MODULE_USB = 10, //: usbͨѶ
    MODULE_DUKPT = 11, //: DUKPT
} SDK_SYS_MODULE_TYPE;

typedef struct {
    bool bIsForceOut;
    bool bIsHavePinPad; //�Ƿ���������//whether with PIN pad
    bool bIsBeep;
    bool bIsINRf;                 //�Ƿ����÷ǽ�//whether inlay contactless
    SDK_SYS_LANG uiLangMode;     //����ģʽ
    SDK_SYS_LINE_MODE uiLineMode;        //��ʾ����ģʽ
    SDK_SYS_LINE_CHOICE uiDispMode;
} SDK_SYS_INFO;

#define APPSUPPORTALL                           0                               // ���ж�֧��
#define APPSUPPORTLAN                           1                               // ֻ֧������
#define APPSUPPORTWIRELESS    2                                         // ֧������

// ���ӻ���֧��ͨѶ��ʽ
#define APPCOMMUBASE                    0x0001
#define APPCOMMUMODEM                   (APPCOMMUBASE)
#define APPCOMMULAN                     (APPCOMMUBASE << 1)
#define APPCOMMUCDMA                    (APPCOMMUBASE << 2)
#define APPCOMMUGPRS                    (APPCOMMUBASE << 3)
#define APPCOMMUWIFI                    (APPCOMMUBASE << 4)
#define APPCOMMUBLUETOOTH               (APPCOMMUBASE << 5)
#define APPCOMMUAUDIO                   (APPCOMMUBASE << 6)
#define APPCOMMUUSB                     (APPCOMMUBASE << 7)

#define APPCOMMUALL                             0xFFFF

SDK_SYS_INFO gstSysInfo =
        {
                false,
                false, //�Ƿ�������������ture
                true,
                false, //�Ƿ�����÷ǽ�false
                SDK_SYS_LANG_CHN, //Ĭ��Ϊ����ģʽ��֧��Ӣ��
                SDK_SYS_LINE5,   //Ĭ��5��ģʽ
                SDK_SYS_LINE2_LINE5, //4��ģʽʱ����ʾģʽ��5��ģʽ��������
        };

/*****************************************************************************
** Descriptions:	����ϵͳ����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
s32 sdkSysSetLang(s32 siLang) {
    gstSysInfo.uiLangMode = siLang;
    return (s32) gstSysInfo.uiLangMode;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰϵͳ����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
s32 sdkSysGetLang(void) {
    return (s32) gstSysInfo.uiLangMode;
}

/*****************************************************************************
** Descriptions:	����LCD��ʾ�ļ���
** Parameters:      void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
s32 sdkSysSetDispLineMode(SDK_SYS_LINE_CHOICE siDispMode) {
    gstSysInfo.uiDispMode = siDispMode;
    return (s32) gstSysInfo.uiDispMode;
}

/*****************************************************************************
** Descriptions:	��ȡLCD��ʾ�ļ���
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
SDK_SYS_LINE_CHOICE sdkSysGetDispLineMode(void) {
    return gstSysInfo.uiDispMode;
}

/*****************************************************************************
** Descriptions:	����ϵͳ�Ƿ�������������
** Parameters:      void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
s32 sdkSysSetPinPadState(bool bIsHavePinPad) {
    return gstSysInfo.bIsINRf = false;
}

/*****************************************************************************
** Descriptions:	��ȡϵͳ�Ƿ�������������
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
bool sdkSysGetPinPadState(void) {
    return gstSysInfo.bIsHavePinPad;
}

/*****************************************************************************
** Descriptions:	����ϵͳ�Ƿ�Ϊ���÷ǽ�
** Parameters:      void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
s32 sdkSysSetInRf(bool bIsInfr) {
    return gstSysInfo.bIsINRf = true;
}

/*****************************************************************************
** Descriptions:	��ȡϵͳ���÷ǽ�״̬
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2014.03.10
** Remarks:
*****************************************************************************/
bool sdkSysGetInRf(void) {
    return gstSysInfo.bIsINRf;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��Ӧ���̺߳���
   ��ڲ���: �ܵ���,Ӧ�ñ�ʶ
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
s32 sdkSysMain(int argc, const char **argv) {
#ifdef XGD_SDK_CDC
    ddi_sys_printf("");
#endif
    sdkInitEMV(); //��emv���ʼ��malloc��free

    sdkKeyOpen();
    sdkDispLcdOpen();


    sdkSysAudioOpen();
    sdkPedInsideOpen();
    sdkLedOpen();

    sdkSysMultiLangInit(SDK_SYS_LANG_CHN, 5);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:�ս���
   ��������:	sdkSysInitParam
   ��������:	��ʼ��sdkϵͳ����
   �������:
   �������:
   ��   ��  ֵ:
                    SDK_ERR
                    SDK_OK
   �޸ı�ע:
   ����ʱ��:2012.07.16 16:41:03
*******************************************************************/
s32 sdkSysInitParam(const SDK_SYS_INITIAL_INFO *pstInital) //sjz 201210716
{
    s32 rslt = SDK_OK;

    //s32 machine = sdkSysGetMachineCode(NULL);

    if (pstInital == NULL) {
        return SDK_PARA_ERR;
    }
    sdkSysSetPinPadState(pstInital->bIsHavePinPad);
    sdkSysSetInRf(pstInital->bIsINRf);
#if 0 /*Modify by ���ƽ at 2015.01.22  14:39 δʹ�ùܵ�*/

    if(SDK_SYS_MACHINE_K301FZ == machine ||
       SDK_SYS_MACHINE_K301FZE == machine ||
       SDK_SYS_MACHINE_K350 == machine )
    {
        if(false == sdkSysGetPinPadState())
        {
            sdkSysSetPinPadState(true);
            sdkDispMsgBox("������̲�������", "\r\n�˻����������������\r\n�Զ���Ϊ�����������\r\n", 0, SDK_KEY_MASK_ALL);
        }
    }
    Verify((rslt = Private_sdkInitFifo(sdkSysGetCurFifoNo())) == SDK_OK);
#endif /* if 0 */
    return rslt;
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������: sdkSysIsInSimulator
   ��������: ���غ����Ƿ�����ģ����������

   ����ʱ��: 2012.05.29 09:44:09
*******************************************************************/
bool sdkSysIsInSimulator() {
#ifdef IS_IN_simulator
    return true;

#else
    return false;

#endif
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������: sdkSysEnableBeep
   ��������:
   �������: beep
             true ʹ��beep
             flase ͣ��beep
   ����ʱ��: 2012.06.01 16:40:05
*******************************************************************/
void sdkSysEnableBeep(bool bIsBeep) {
    gstSysInfo.bIsBeep = bIsBeep;
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������:
   ��������: sdkSysGetBeepState
   �������:
   �������:
   �� �� ֵ: �õ�����״̬
   �޸ı�ע:
   ����ʱ��:2012.06.01 16:42:25
*******************************************************************/
bool sdkSysGetBeepState(void) {
    return gstSysInfo.bIsBeep;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_SYS_BEEP_TYPE eType
** Returned value:
** Created By:		shiweisong  2013.06.21
** Remarks:
*****************************************************************************/
bool sdkSysBeep(SDK_SYS_BEEP_TYPE eType) {
    if (gstSysInfo.bIsBeep) {
        if (eType == SDK_SYS_BEEP_OK) {
            sdkSysAudioBeepOk();
        }
        else {
            sdkSysAudioBeepErr();
        }
    }
    return gstSysInfo.bIsBeep;
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������: sdkSysCmd
   ��������: ����shell����
   �������: cmd shell�ű��ַ���

   �������: pRetCode
             ���������
   �� �� ֵ:
             SDK_SYS_SHELL_RUN_SCRIPT            = (-4),    //���нű�ʧ��
             SDK_SYS_SHELL_FAILLED_IN_RUN_SCRIPT = (-3) ,   //�ű����й�����ʧ��
             SDK_SYS_SHELL_PARA_NULL             = (-2) ,   //�մ���
             SDK_SYS_SHELL_SYS_ERR               = (-1) ,   //ϵͳ����
             SDK_SYS_SHELL_OK                    = (1) ,    //�ɹ�
   �޸ı�ע:
   ��           ע:  ����㲻Ҫ�õ�psiRetCode ���Դ�NULL����
   ����ʱ��:2012.05.21 15:34:45
*******************************************************************/
SDK_SYS_SHELL_RET sdkSysCmd(const u8 *pasCmd, s32 *psiRetCode) {
    if (pasCmd == NULL) {
        return SDK_SYS_SHELL_PARA_NULL;
    }

    if (strcmp(pasCmd, "reboot") == 0) {
        if (DDI_OK == ddi_sys_reboot()) {
            return SDK_OK;
        }
    }
    else if (strcmp(pasCmd, "poweroff") == 0) {
        if (DDI_OK == ddi_sys_poweroff()) {
            return SDK_OK;
        }
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions: �л�������ģ��, ��param������д�빲���ڴ�(�����ļ�)
** Parameters:   void *param �����ṹ��
                 int len �����ṹ��Ĵ�С
** Returned value:
** Created By:   tjb 2014.12.09
** Remarks:      ����ģ��ʵ�ֿ�����һ�����̻��߿����档
                 ���ط�ʽΪ socket ʱ��Ӧ�����л�ǰ����Ҫ����·���á�����ģ��
                 ֻ������ܡ��������ݡ�
                 ���ط�ʽΪ����ʱ��
*****************************************************************************/
int sdkSysDownloadMode(SDK_DOWNLOAD_MODE *pstDown) {
    if (pstDown == NULL) {
        return SDK_PARA_ERR;
    }

    switch (pstDown->uiType) {
        case 1: {
            if (DDI_OK == ddi_sys_download(0)) {
                return SDK_OK;
            }
            return SDK_ERR;
        }

        case 2: {
            break;
        }

        case 3: {
            if (DDI_OK == ddi_sys_download(1)) {
                return SDK_OK;
            }
            return SDK_ERR;
        }

        default:
            break;
    }

    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�жϵ�ǰ���߷ǽ��Ƿ������õ�
** Parameters:      void
** Returned value:
** Created By:		shiweisong  2012.10.30
** Remarks:
*****************************************************************************/
bool sdkSysIsRfIn(void) {
    return sdkSysGetInRf();
}

//增加给外部设置应用 id 接口//zhouxiaoxin
u8 gCurAppId[128]={0};
/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ׿��
   ��������: sdkGetCurAppId
   ��������: �õ���ǰӦ�ñ�ʶ
   �������:
   �������: pdata
             ��ǰӦ�ñ�ʶ
   �� �� ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.31
*******************************************************************/
s32 sdkSysGetCurAppId(char *pasData) {
    if (pasData == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    strcpy(pasData, gCurAppId);
    return strlen(pasData);
}
s32 sdkSysSetCurAppId(char *pasData){
    if (pasData == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    strcpy(gCurAppId,pasData);
    return strlen(gCurAppId);
}
/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ׿��
   ��������: sdkGetCurAppDir
   ��������: �õ���ǰӦ�ó���Ŀ¼
   �������:
   �������: pdata
             ��ǰӦ��Ŀ¼
   �� �� ֵ:
   �޸ı�ע:
   ����ʱ��:2012.05.31
*******************************************************************/
s32 sdkSysGetCurAppDir(u8 *pasData) {
    u8 bffer[128];

    if (NULL == pasData) //shijianglong 2013.01.30 15:58
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(bffer, 0, 128);

    if(sdkSysGetCurAppId(bffer)>0) {
        return sprintf(pasData, "/sdcard/emv/%s/", bffer);
    }else{
        return sprintf(pasData, "/sdcard/emv/");
    }
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ׿��
   ��������: sdkGetCurAppState
   ��������: �õ���ǰӦ������״̬
   �������:
   �������:
   �� �� ֵ: ��ǰӦ������״̬
   �޸ı�ע:
   ����ʱ��:2012.06.01
*******************************************************************/
s32 sdkSysGetCurAppState(void) {
    return SDK_ERR;
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ׿��
   ��������: sdkGetAppNum
   ��������: �õ�Ӧ�õ�������
   �������:
   �������:
   �� �� ֵ:���Ӧ�ø���
   �޸ı�ע:
   ����ʱ��:2012.06.01
*******************************************************************/
s32 sdkSysGetAppMaxNum(void) {
    return 1; //G2 P200 ֻ��1��
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������:
   ��������: ��ѯϵͳ�Ƿ�Ҫǿ���˳�
   �������:
   �� �� ֵ:
             true ǿ���˳�
             false û��ǿ���˳�Ҫ��
   �޸ı�ע:
   ����ʱ��:2012.06.21 10:57:47
*******************************************************************/
bool sdkSysIsFroceOut(void) {
    bool ret = gstSysInfo.bIsForceOut;

    //ÿһ��ǿ���˳���ǿ���˳���־�ͻ����
    gstSysInfo.bIsForceOut = false;
    return ret;
}

/*****************************************************************************
** Descriptions:	��ȡlibdev�汾��
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetLibdevVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memcpy(pasVer, "δʹ��libdev", 12);
    return 12;
}

/*****************************************************************************
** Descriptions:	��ȡpanel�汾
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetPanelVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memcpy(pasVer, "δʹ��panel", 11);

    return 11;
}

/*****************************************************************************
** Descriptions:	��ȡdaemon�汾
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetDaemonVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memcpy(pasVer, "δʹ��daemon", 12);
    return 12;
}

/*****************************************************************************
** Descriptions:	��ȡemv��汾��
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetLibxgdemvVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    GetLibEmvVersion(pasVer);

    return strlen(pasVer);
}

/*****************************************************************************
** Descriptions:	��ȡ
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetLibappdeviceVersion(u8 *pasVer) {
    u8 temp[32] = {0};

    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    pasVer[0] = 'V';
    Verify(sdkSysGetMachineCode(&pasVer[1]) >= 0);
    Verify(sdkSysStlGetMachinemodelVersion(temp) >= 0);
    strcat(pasVer, temp);
    return strlen(pasVer);
}

/*****************************************************************************
** Descriptions:	��ȡ��ѧ��汾��
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetLibMahtsVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    strcat(pasVer, SDK_SYS_VER);
#ifdef XGD_SDK_DEBUG
    return strlen(strcat(pasVer, "d"));

#else
    return strlen(strcat(pasVer, "r"));

#endif
}

/*****************************************************************************
** Descriptions:	��ȡmodule��汾
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		shijianglong  2013.01.10
** Remarks:             ��V+����
*****************************************************************************/
s32 sdkSysGetLibModuleVersion(u8 *pasVer) {
    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memcpy(pasVer, "δʹ��libmodule", 15);
    return 15;
}

/*****************************************************************************
** Descriptions:    �����ļ����͹ؼ��ֻ�ȡ�ļ��汾��Ϣ
** Parameters:      u8 *fpFile  ����·��
                    u8 *pTag
                    u8* lpOut  ���
** Returned value:  ȡ�����ݳ��� û��ȡ���򷵻� -1
** Created By:		fusuipu  2013.03.29
** Remarks:
*****************************************************************************/
s32 sdkSysGetFileVer(const u8 *fpFile, const u8 *pTag, u8 *lpOut) {
    if (strcmp(pTag, "OutVer") == 0 || strcmp(pTag, "InnerVer") == 0) {
        strcat(lpOut, SDK_SYS_VER);
#ifdef XGD_SDK_DEBUG
        return strlen(strcat(lpOut, "d"));

#else
        return strlen(strcat(lpOut, "r"));

#endif
    }
    return -1;
}

/*****************************************************************************
** Descriptions:	��ȡ�ײ�̼��汾
** Parameters:          u8 *lpOut :����̼��汾��Ϣ( ����32�ֽ����� )
                    u32 nType :�̼�����ID
** Returned value:
** Created By:		���ƽ  2014.07.22
** Remarks:
*****************************************************************************/
s32 sdkSysGetFirmwareVer(u8 *lpOut, SDK_SYS_FIRMWAREVER_TYPE nType) {
    s32 rtn;

    if (lpOut == NULL) {
        return SDK_PARA_ERR;
    }
    rtn = ddi_sys_get_firmwarever(lpOut, nType);

    if (rtn == DDI_OK) {
        return SDK_OK;
    }
    else if (rtn == DDI_EINVAL) {
        return SDK_PARA_ERR;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ�ײ�ģ��汾
** Parameters:          u8 *lpOut :����ײ�ģ��汾��Ϣ
                    u32 nType :ģ��ID
** Returned value:
** Created By:		���ƽ  2014.07.22
** Remarks:
*****************************************************************************/
s32 sdkSysGetModuleVer(u8 *lpOut, SDK_SYS_MODULE_TYPE nType) {
    s32 ret;

    switch (nType) {
        case MODULE_MAGCARD: //�ſ�
        {
            ret = sdkMagcardGetVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkIccOpenMag();
                ret = sdkMagcardGetVersion(lpOut);
                sdkIccCloseMag();
            }
            break;
        }

        case MODULE_ICCCARD: //IC��
        {
            ret = sdkIccGetVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkIccOpenIcDev();
                ret = sdkIccGetVersion(lpOut);
                sdkIccCloseIcDev();
            }
            break;
        }

        case MODULE_RFCARD: //�ǽӿ�
        {
            ret = sdkIccRfGetVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkIccOpenRfDev();
                ret = sdkIccRfGetVersion(lpOut);
                sdkIccCloseRfDev();
            }
            break;
        }

        case MODULE_PRINTER: //��ӡ��
        {
            memcpy(lpOut, "û�д�ӡ��", 10);
            ret = SDK_OK;
            break;
        }

        case MODULE_AUDIO: //��Ƶ
        {
            ret = sdkSysAudioVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkSysAudioOpen();
                ret = sdkSysAudioVersion(lpOut);
                sdkSysAudioClose();
            }
            break;
        }

        case MODULE_INSIDEPED: //������Կ
        {
            ret = sdkPedInsideVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkPedInsideOpen();
                ret = sdkPedInsideVersion(lpOut);
                sdkPedInsideClose();
            }
            break;
        }

        case MODULE_FILESYS: //�����ļ�ϵͳ
        {
#if 0 //del by sujianzhong 2015.06.05
            ret = fversion(lpOut);
#else
            if (DDI_OK == ddi_vfs_ioctl(DDI_VFS_CTL_VER, 0, (u32) lpOut)) {
                return SDK_OK;
            }
            else {
                return SDK_ERR;
            }
#endif
            break;
        }

        case MODULE_BT:    //����
        {
            ret = sdkBtGetVersion(lpOut, 17);

            if (ret == SDK_EIO) {
                sdkBtOpen();
                ret = sdkBtGetVersion(lpOut, 17);
                sdkBtClose();
            }
            break;
        }

        case MODULE_AUDIOCOMM: //��ƵͨѶ
        {
            memcpy(lpOut, "��֧����ƵͨѶ", 10);
            ret = SDK_OK;
            break;
        }

        case MODULE_LED:   //LED��
        {
            ret = sdkLedVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkLedOpen();
                ret = sdkLedVersion(lpOut);
                sdkLedClose();
            }
            break;
        }

        case MODULE_USB:   //USBͨѶ
        {
            ret = sdkUsbVersion(lpOut);

#if 0 /*Modify by ���ƽ at 2015.03.27  16:46 */

            if( ret == SDK_EIO )
            {
                sdkUsbOpen(1);
                ret = sdkUsbVersion(lpOut);
                sdkUsbClose(1);
            }
#endif /* if 0 */
            break;
        }

        case MODULE_DUKPT:   //dukpt
        {
            ret = sdkPedDukptGetVersion(lpOut);

            if (ret == SDK_EIO) {
                sdkPedDukptOpen();
                ret = sdkPedDukptGetVersion(lpOut);
                sdkPedDukptClose();
            }
            break;
        }

        default: {
            return SDK_ERR;
        }
    }

    return ret;
}

/*****************************************************************************
** Descriptions:	���³�ʼ��sdk���� ����������Զ�ε���
** Parameters:          const SDK_SYS_INITIAL_INFO *pstInital
** Returned value:
** Created By:		shiweisong  2012.11.06
** Remarks:
*****************************************************************************/
s32 sdkSysReInitParam(const SDK_SYS_INITIAL_INFO *pstInital) {
    if (pstInital == NULL) {
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2015.01.19  16:11 */
    sdkSysSetPinPadState(pstInital->bIsHavePinPad);
    sdkSysSetInRf(pstInital->bIsINRf);
#endif /* if 0 */
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    ���ñ���ʱ��
** Parameters:      s32 siDlyTime
** Returned value:
** Created By:		fusuipu  2013.05.09
** Remarks:         ��λ:ms
*****************************************************************************/
s32 sdkSysSetBlackLightCloseTime(s32 siDlyTime) {
    return sdkDispExSetLcdlampDelayTime(siDlyTime);
}

/*****************************************************************************
** Descriptions:
** Parameters:      s32 siKeyFreq  ����������Ƶ��
                    s32 siKeyAtio  ������ÿ������ʱ�䳤��
** Returned value:
** Created By:		fusuipu  2013.05.09
** Remarks:
*****************************************************************************/
s32 sdkSysSetBeepTune(s32 siKeyFreq, s32 siKeyAtio) {
    if (siKeyFreq < 0 || siKeyAtio < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�����Աȶ�
** Parameters:          s32 siLightLev
** Returned value:
** Created By:		fusuipu  2013.05.10
** Remarks:
*****************************************************************************/
s32 sdkSysSetLcdContrast(s32 siLightLev) {
    if (siLightLev < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (siLightLev < 20) {
        siLightLev = 20;
    }
    else if (siLightLev > 60) {
        siLightLev = 60;
    }
    sdkSysSetConTrast(siLightLev);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�������
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.12
** Remarks:
*****************************************************************************/
void sdkSysBlackLightOn(void) {
    sdk_dev_misc_ex_enable_lcdlamp(true);           //������Ʊ����
    sdk_dev_misc_ex_control_lcdlamp(true);          //������
    sdk_dev_misc_ex_enable_lcdlamp(false);          //��ֹ����ƿ���
}

/*****************************************************************************
** Descriptions:	�ر����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.12
** Remarks:
*****************************************************************************/
void sdkSysBlackLightOff(void) {
    sdk_dev_misc_ex_enable_lcdlamp(true);           //������Ʊ����
    sdk_dev_misc_ex_control_lcdlamp(false);          //�ر���
    sdk_dev_misc_ex_enable_lcdlamp(false);          //��ֹ����ƿ���
}

/*****************************************************************************
** Descriptions:
** Parameters:          bool isEnCtro
** Returned value:
** Created By:		fusuipu  2013.07.16
** Remarks:
*****************************************************************************/
void sdkSysBlackLightCtro(bool isEnCtro) {
    sdk_dev_misc_ex_enable_lcdlamp(isEnCtro);
}

typedef enum {
    SDK_POWER_AD_BATTERY_ERR = -2, //����磬��ع���, �����Դ�ӡ��,�����Կ��ǽ�
    SDK_POWER_NEED_DOWN = -1, //��ص������㣬�ػ�, �����Դ�ӡ��,�����Կ��ǽ�
    SDK_POWER_ZERO = 0, //���0��, ��͵���ʾ�������Դ�ӡ��,�����Կ��ǽ�
    SDK_POWER_ONE = 1, //���1��
    SDK_POWER_TWO = 2, //���2��
    SDK_POWER_THREE = 3, //���3��
    SDK_POWER_FULL = 99, //��س���
} SDK_POWER_STATE;

/*****************************************************************************
** Descriptions:��ȡ��ص��������Ƴ�ŵ�
** Parameters:          void
** Returned value:
                        -2  ��ع���
                        -1	����޵���,��Ҫ�����ػ�
                        0	���0�����,��Ҫ����
                        1	���1�����
                        2	���2�����
                        3	���3�����
                        99	��س���
** Created By:		shijianglong  2013.06.24
** Remarks:������粢�ҵ�ص������ǳ���״̬��ʱ�����г�硣
*****************************************************************************/
s32 sdkSysGetBatteryStateAndControl(void) {
    s32 power_state = ddi_sys_bat_status();

    Trace("devapi", "ddi_sys_bat_status = %d\r\n", power_state);

    switch (power_state) {
        case DDI_POWER_NULL:              //��ص������㣬�ػ�
        {
            return SDK_POWER_NEED_DOWN;
        }

        case DDI_POWER_ZERO:              //����磬���0��
        {
            return SDK_POWER_ZERO;
        }

        case DDI_POWER_ONE:               //����磬���1��
        {
            return SDK_POWER_ONE;
        }

        case DDI_POWER_TWO:               //����磬���2��
        {
            return SDK_POWER_TWO;
        }

        case DDI_POWER_THREE:             //����磬���3��
        {
            return SDK_POWER_THREE;
        }

        case DDI_POWER_AD_CHARGE:         //����磬�����//Ӧ�ô���1�� �͵���1����
        {
            return SDK_POWER_ONE;
        }

        case DDI_POWER_AD_CHARGE_ONE:     //����磬�����, ���1 ������: ����0��
        {
            return SDK_POWER_ZERO;
        }

        case DDI_POWER_AD_NO_CHARGE:      //����磬δ���(��ʱ����Ϊ�������)
        {
            return SDK_POWER_FULL;
        }

        case DDI_POWER_AD_BATTERY_ERR:    //����磬��ع���
        {
            return SDK_POWER_AD_BATTERY_ERR;
        }

        default:
            return SDK_POWER_AD_BATTERY_ERR;
    }
}

/*******************************************************************
   ��    Ȩ: �¹���
   ��    ��: ʯ����
   ��������: Private_sdkSysSetFroceOut
   ��������: ����ǿ���˳���־
   �������:
   �� �� ֵ:
   �޸ı�ע:
   ����ʱ��:2012.06.21 11:03:26
*******************************************************************/
bool Private_sdkSysSetFroceOut(void) {
    gstSysInfo.bIsForceOut = true;
    return true;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.08.02
** Remarks:
*****************************************************************************/
s32 sdkSysGetCurFifoNo(void) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�л�Ӧ�ó���
** Parameters:          const u8 *pasAppId:Ŀ��Ӧ�ñ�ʶ. ���Ϊ�վ��л�������
                               const u8 *pData
                               s32 siLen
** Returned value:
** Created By:		shijianglong  2013.02.19
** Remarks:
*****************************************************************************/
s32 sdkSysSwitchApp(const u8 *pasAppId, const u8 *pasSrc, s32 siSrcLen, u8 *pasDest) {
    if (NULL == pasAppId || NULL == pasSrc || NULL == pasDest) {
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�����ط�������
** Parameters:          s32 siPosPTrigger:
                                0:�������أ���������Ҫ��ʲô
                                1:ʹ�ô��������(��������һ��Ϊ62������)
                                3:ǿ����������
                               const u8 *pasSrc:��������һ��Ϊ62������
                               s32 siSrcLen:�������ݳ���
                               u8 *pasDest:��ȡ������
** Returned value:	�ɹ����ػ�ȡ�����ݳ���
                                ʧ�ܷ���SDK_ERR
                                ��������SDK_PARA_ERR.
** Created By:		shijianglong  2013.02.25
** Remarks:
*****************************************************************************/
s32 sdkSysTravelManage(s32 siPosPTrigger, const u8 *pasSrc, s32 siSrcLen, u8 *pasDest) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ���ذ汾��
** Parameters:          u8 *pasVer:�汾��
** Returned value:  ���ذ汾�ų���
** Created By:		shijianglong  2013.02.27
** Remarks:
*****************************************************************************/
s32 sdkSysGetManageVersion(u8 *pasVer) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡģ���汾��
** Parameters:          u8 *pasVer
** Returned value:	�汾�ų���
** Created By:		shijianglong  2013.02.27
** Remarks:
*****************************************************************************/
s32 sdkSysGetLibKernelVersion(u8 *pasVer) {
    u8 temp[32];

    if (pasVer == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    sdkSysGetMachineCode(temp);
    sprintf(pasVer, "V%s100201104", temp);
    return strlen(pasVer);
}

/*****************************************************************************
** Descriptions:	����ֻ��汾��Ϣ
** Parameters:          u8 *pasVer:�ֻ��汾��Ϣ
** Returned value:	�ֻ��汾��Ϣ����
** Created By:		shijianglong  2013.02.27
** Remarks:
*****************************************************************************/
s32 sdkSysGetHandsetVersion(u8 *pasVer) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��õ����汾��Ϣ
** Parameters:          u8 *pasVer:�汾��
** Returned value:	�汾�ų���
** Created By:		shijianglong  2013.02.27
** Remarks:             ���Ϊ390��汾�źͳ��Ⱦ�����0
*****************************************************************************/
s32 sdkSysGetBaseVersion(u8 *pasVer) {
    return SDK_ERR;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û�����֧�ֵ�ͨѶ��ʽ
   ��ڲ���:
   �� �� ֵ:���ظ���������֧�ֵ�ͨѶ��ʽ
   ��      ע:
 ********************************************************************/
u16 sdkSysGetCommMask(void) {
    u16 type = SDK_COMMU_MASK_ALL;             // Ĭ��֧��ȫ��ͨѶ��ʽ

    return type;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �����֧�ֵ�ͨѶ��ʽ
   ��ڲ���: ��
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
enum SDK_SYS_COM_NET_TYPE sdkSysGetSupportCommu(void) //lint !e714
{
    u16 Mask;
    u32 ret = SDK_SYS_COMMU_ALL;


    Mask = sdkSysGetCommMask();

    if (((Mask & APPCOMMUCDMA) != APPCOMMUCDMA)
        && ((Mask & APPCOMMUGPRS) != APPCOMMUGPRS)
        && ((Mask & APPCOMMULAN) == APPCOMMULAN) //֧������
            ) {
        ret = SDK_SYS_COMMU_LAN;                            // ֻ֧������
    }
    else if (((Mask & APPCOMMUCDMA) == APPCOMMUCDMA)
             && ((Mask & APPCOMMUGPRS) ==
                 APPCOMMUGPRS)                       // ��֧��CDMA��GPRS ֧������
             && ((Mask & APPCOMMULAN) != APPCOMMULAN)
            ) {
        ret = SDK_SYS_SOMMU_WIRELESS;                                                       // ֻ֧������
    }
    // ��ֹ���ִ���
    Assert(APPSUPPORTALL == SDK_SYS_COMMU_ALL);
    Assert(APPSUPPORTALL == SDK_SYS_COMMU_LAN);
    Assert(APPSUPPORTALL == SDK_SYS_SOMMU_WIRELESS);
    return (enum SDK_SYS_COM_NET_TYPE) ret;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û���Ĭ����֧�ֵ�ͨѶ��ʽ
   ��ڲ���:
   �� �� ֵ:���ظ�������Ĭ����֧�ֵ�ͨѶ��ʽ
   ��      ע:
 ********************************************************************/
SDK_COMM_COMMMODE sdkSysGetDefaultCommuType(void) //lint -e714
{
    return (SDK_COMM_COMMMODE) SDK_COMMU_MASK_BLUETOOTH;
}

/*****************************************************************************
** Descriptions:    ��ȡmodem����
** Parameters:      void
** Returned value:
** Created By:      huangbx(20130117)
** Remarks:
*****************************************************************************/
u8 sdkSysGetModemType(void) {
    return 2;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const u8 *pasFile
** Returned value:
** Created By:		shijianglong  2013.01.24
** Remarks:
*****************************************************************************/
s32 Private_sdkSysLicenseVerify(const u8 *pasFile) {
    if ((pasFile == NULL) || (access(pasFile, 0) != 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return 0;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ�Ӧ�ô��ں�
   ��ڲ���: comtype 0ΪPC����
                                comtype 1Ϊpin����
   �� �� ֵ: ���ں���
   ��      ע:
 ********************************************************************/
s32 sdkSysGetComNo(const enum SDK_SYS_COM_TYPE eComtype) {
    s32 comno = 0;

    if (!(eComtype < SDK_SYS_COM_ERR)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return comno;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ�mis���ں�
   ��ڲ���: �Ƿ���������
   �� �� ֵ: ���ں���
   ��      ע:
 ********************************************************************/
s32 sdkSysGetMisComNo(bool IsSupportPinPad) {
    return SDK_SYS_COM_PC;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const u8 *fpFile
                   const u8 *pFlag Flag Ϊ�汾��Ϣ=ǰ��Ĳ���

                    [dgxrevofni]
                    OutVer=1.02
                    InnerVer=1.02
                    AppID=
                    AppName=
                    FileStyle=dll
                    FileRoot=/mtd0/dll/
                    Company=XGD
                    RelyFile=libsdk.so -libsdkdev.so -libsdkload.so
                    UserInfor= made by XGD sdk group
                    SysEditDate=
                    [end]

                   u8 * lpOut ��Ӧflag =���������
** Returned value:	������ݳ���
** Created By:		shiweisong  2013.07.03
** Remarks:
*****************************************************************************/
s32 sdkSysGetFileVersion(const u8 *fpFile, const u8 *pFlag, u8 *lpOut) {
    if (strcmp(pFlag, "OutVer") == 0 || strcmp(pFlag, "InnerVer") == 0) {
        {
            strcat(lpOut, SDK_SYS_VER);
#ifdef XGD_SDK_DEBUG
            return strlen(strcat(lpOut, "d"));

#else
            return strlen(strcat(lpOut, "r"));

#endif
        }
    }
    return -1;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�汾��Ϣ
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
s32 sdkSysStlGetMachinemodelVersion(u8 *pasMachineversion) {
    if (NULL == pasMachineversion) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdkSysGetFileVersion("libsdkdev.so", "OutVer", pasMachineversion);
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û��ͷ���
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
s32 sdkSysGetMachineModel(u8 *pStr) {
    if (pStr == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    //strcpy(pStr, "K200P");
    strcpy(pStr, "200Q");
    return strlen(pStr);
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ�Ӧ�õĻ����ͺ�
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
s32 sdkSysGetMachineCode(u8 *pcode) {
    u8 temp[64] = {0};

    if (pcode != NULL) {
        sprintf(temp, "%02X", (u8) MACHINEMODEL);
        strcpy(pcode, temp);
    }
    return MACHINEMODEL;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ��Ƿ���POS
   ��ڲ���:
   �� �� ֵ:TRUE �� FALSE��
   ��      ע:
 ********************************************************************/
bool sdkSysIsHaveBase(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ���unlinuxƽ̨����
   ��ڲ���:
   �� �� ֵ:TRUE �� FALSE��
   ��      ע:
 ********************************************************************/
bool sdkSysIsUclinuxPlat(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ��в���?
   ��ڲ���:
   �� �� ֵ:��
   ��      ע:
 ********************************************************************/
bool sdkSysIsColorScreen(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������:
   ��ڲ���:
   �� �� ֵ:��������
   ��      ע:
 ********************************************************************/
enum SDK_SYS_SCREEN_TYPE sdkSysGetScreenPixel(void) {
    return SDK_SYS_SCREEN_128H64;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ�������
   ��ڲ���:
   �� �� ֵ:TRUE �� FALSE��
   ��      ע:
 ********************************************************************/
bool sdkSysIsHaveSpeaker(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ����
   ��ڲ���:
   �� �� ֵ:TRUE �� FALSE��
   ��      ע:
 ********************************************************************/
bool sdkSysIsPrintPin(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ���֤��
   ��ڲ���:
   �� �� ֵ:֤��ų��� ʧ��Ϊ-1
   ��      ע:
 ********************************************************************/
s32 sdkSysGetCupSn(u8 *pSn) {
    if (pSn == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    strcpy(pSn, "0000");

    return strlen(pSn);
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��û�����������Ĭ���������
   ��ڲ���:
   �� �� ֵ:���ظ�������Ĭ���������
   ��      ע:
 ********************************************************************/
bool sdkSysIsHavePinPad(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ��÷��ػ�ӭ����ʱ��
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
s32 sdkSysGetAppReturnWelcome(const s32 siDeflautTimer) {
    return (s32) siDeflautTimer;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ�֧��PCI����ȫ��
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
bool sdkIsSupportPCI(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ�֧��TSlib��
   ��ڲ���:
   �� �� ֵ:
   ��      ע:
 ********************************************************************/
bool sdkSysIsSupportTSlib(void) {
    return false;
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: ���Ӧ���л�ID
   ��ڲ���:const u8 *IdIn ���벻�����͵�ID,
                        u8 *IdOut ���������͵�ID,
   �� �� ֵ: ����
   ��      ע:
 ********************************************************************/
s32 sdkSysGetSwitchAppId(const u8 *IdIn, u8 *IdOut) {
    //u8 temp[64] = {0};

    if (IdIn == NULL || IdOut == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by ���ƽ at 2015.05.05  18:55 */

    if(strlen(IdIn) + 4 > sizeof(temp))     // ��ֹ���������ݾ�̫�����
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
#endif /* if 0 */
    strcpy(IdOut, "K200P");
    strcat(IdOut, IdIn);
    return strlen(IdOut);
}

/*******************************************************************
   ��      ��: ����
   ��      Ȩ: �����¹����ɷ����޹�˾
   ��������: �Ƿ������¼���
   ���¼��ᱻ���ó�*#
   ��ڲ���:��
   �� �� ֵ: ��
   ��      ע:
 ********************************************************************/
bool sdkSysIsMultiplexUpDown(void) {
    return true;
}

/*****************************************************************************
** Descriptions:      �õ��������ʾ������
** Parameters:        void
** Returned value:
** Created By:        ʯ����  2012.08.03
** Remarks:           ��˹�����֧��4��ģʽ���������Ĭ��5��ģʽ
*****************************************************************************/
s32 sdkSysGetSreenMaxLine(void) {
    return gstSysInfo.uiLineMode;
}

/*****************************************************************************
** Descriptions:      �����������ʾ������
** Parameters:        void
** Returned value:
** Created By:        ʯ����  2012.08.03
** Remarks:           ��˹�����֧��4��ģʽ���������Ĭ��5��ģʽ
*****************************************************************************/
s32 sdkSysSetSreenMaxLine(SDK_SYS_LINE_MODE uiLineMode) {
    gstSysInfo.uiLineMode = uiLineMode;
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ��Ļ����д��������ĸ����(��λ:byte)
** Parameters:      void
** Returned value:
** Created By:		fusuipu  2013.03.26
** Remarks:
*****************************************************************************/
s32 sdkSysGetSreenCharCntPerLine(void) {
    return SDKDISPMAXLINELEN;
}

/*****************************************************************************
** Descriptions:    ����Ƿ�����
** Parameters:      Input: None
** Returned value:
** Created By:      huangbx(20130307)
** Remarks:         ����ǲ����Դ�򱣴��ǳ��״̬
*****************************************************************************/
bool sdkSysIsExPower() {
    s32 power_state = ddi_sys_bat_status();

    if (power_state >= DDI_POWER_AD_CHARGE) {
        return true;
    }
    else {
        return false;
    }
}

/*****************************************************************************
** Descriptions:    �͹��Ŀ���
** Parameters:
                    bOption == 0����Ѿ�����,���һ�,����ģ���51����͹���
                    bOption == 1����Ѿ�����,���һ�,����ģ�����͹���,51������
** Returned value:
** Created By:      shiweisong
** Remarks:         ����������µ��ô˺��� �᷵��SDK_ERR
                    �����߹�������� ��������� �������Զ��˳��͹���
                    ��������SDK_OK
                    ��������£�Щ����������ֱ���˳����ġ�
                    �������ͽ��������û����˳������ģ�������

*****************************************************************************/
s32 sdkSysEnterIdle(bool bFlag, u8 uOption) {
    if (DDI_OK != ddi_sys_sleep(0))  //�����˻��Ѱ���
    {
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����ʱ���ѹ��ܵ�ϵͳ����
** Parameters:          s32 sec:�Զ�������ʱʱ��( ��λ:�� )
                    s32 type:   0����ʱʱ�䵽�ػ�
                                1����ʱʱ�䵽����ϵͳ
                                -1:ȡ����ʱ����
                    s32 status  0�������˻��Ѱ���
                                1�����˻��Ѱ���
** Returned value:	SDK_PARA_ERR,SDK_ERR,SDK_OK
** Created By:		���ƽ  2014.08.22
** Remarks:
******************************************************************************/
s32 sdkSysIdleWithAlarm(s32 sec, s32 type, bool status) {
    if (sec < 0 && type < -1 && type > 1) {
        return SDK_PARA_ERR;
    }

    if (type == -1) {
        ddi_sys_sleep(status);
    }
    else if (DDI_ERR == ddi_sys_sleep_with_alarm(sec, type, status)) {
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    Ѱ��ָ����ǩ��λ��
** Parameters:          u8 *fpFile
                               u8 *pHead
** Returned value:
** Created By:		fusuipu  2013.03.29
** Remarks:         �˺������ڻ�ȡָ����ǩ�汾��Ϣ
*****************************************************************************/
s32 Private_sdkSysGetSignVer(const u8 *fpFile, const u8 *pFlag, u8 *lpOut) {
    if (strcmp(pFlag, "OutVer") == 0 || strcmp(pFlag, "InnerVer") == 0) {
        strcat(lpOut, SDK_SYS_VER);
#ifdef XGD_SDK_DEBUG
        return strlen(strcat(lpOut, "d"));

#else
        return strlen(strcat(lpOut, "r"));

#endif
    }
    return -1;
}

/*****************************************************************************
** Descriptions:	��ȡsdk�洢���ļ�Ŀ¼
** Parameters:          u8 *lpOut
** Returned value:
** Created By:		fusuipu  2013.04.01
** Remarks:         �˺�����ȡ�û��ļ��Ĵ洢Ŀ¼
*****************************************************************************/
s32 Private_sdkGetPosRoot(u8 *lpOut) {
    if (NULL == lpOut) {
        return SDK_PARA_ERR;
    }
    return 0;
}

/*****************************************************************************
** Descriptions:	        �����豸
** Parameters:          void
** Returned value:	SDK_OK �ɹ�
** Created By:		ʯ����  2012.08.09
** Remarks:
*****************************************************************************/
s32 Private_OpenDevices(void) {
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:   �ú���ֻ�ǻ�ȡlinuxƽ̨�Ŀ����ţ�
** Parameters:     ucSlotNO:���ۺ�
** Returned value: SDK_PARA_ERR:��������
                    0:����IC��ģ��
                    1:��IC��ģ��
** Created By:     sujianzhong20130222
** Remarks:
*****************************************************************************/
s32 Private_sdkSysGetSlotNO(u8 *pUcSlotNO) {
    return false;
}

/*****************************************************************************
** Descriptions:	        ��ɨ��ǹ�µ�
** Parameters:          void
** Returned value:
** Created By:		shijianglong  2012.09.21
** Remarks:
*****************************************************************************/
void Private_sdkPowerDownScanner(void) {
}

/*****************************************************************************
** Descriptions:	����ɨ��ǹ��ɨ������
** Parameters:          void
** Returned value:
** Created By:		shijianglong  2012.09.21
** Remarks:
*****************************************************************************/
void Private_sdkWakeUpScanner(void) {
}

/*****************************************************************************
** Descriptions:	��ʼ���ܵ���Ϣ
** Parameters:          s32 CurFifoNo
** Returned value:
** Created By:		fusuipu  2014.02.14
** Remarks:
*****************************************************************************/
s32 Private_sdkInitFifo(s32 curfifono) {
    return SDK_ERR;
}

#if 0
/*****************************************************************************
** Descriptions:
** Parameters:          bool bState
** Returned value:
** Created By:		fusuipu  2013.11.14
** Remarks:             ͬ��1.04
*****************************************************************************/
static void sdkSaveChargeState(bool bState)
{
    return;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.11.14
** Remarks:             ͬ��1.04
*****************************************************************************/
static u8 sdkReadChargeState()
{
    return SDK_OK;
}

#endif

/*****************************************************************************
** Descriptions:    ����Դ״̬
** Parameters:      Input: None
** Returned value:
** Created By:      huangbx(20130307)
** Remarks:         ����ǲ����Դ�򱣴��ǳ��״̬
*****************************************************************************/
s32 sdkSysGetPowerState(void) {
    if (!sdkSysIsExPower()) //�������
    {
        if (sdkSysGetBatteryStateAndControl() < 0)                                     //�͵�ѹ״̬
        {
            return SDK_SYS_LOWPOWER;
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ��Ӧ���߳��˳�
   ��ڲ���:
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
void sdkSysAppQuit(void) {
    sdkSysDestroyMsgList();                                                 //�ݻ���Ϣ����

    sdkKeyClose();
    sdkDispLcdClose();
    sdkIccCloseMag();
    sdkIccCloseIcDev();
    sdkIccCloseRfDev();
    sdkSysAudioClose();

    sdkBtClose();
    //sdkCommCloseUart(SDK_COMM_UART_AUDIO);
    sdkCommCloseUart(SDK_COMM_UART_USB);
    sdkCommCloseUart(SDK_COMM_UART_USBHID);
    sdkLedClose();
    sdkPedInsideClose();
    sdkPedDukptClose();
}

/*******************************************************************
   ��	  ��: ������
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: ����ػ�
   ��ڲ���:
   ��	  ע: �˺���Ϊ����ײ���ú���,���������ܵ�����,����ᵼ�¹ܵ���ʧ
 ********************************************************************/
void sdkDealPowerOff(void) {
    sdkDispStateBar(false);
    sdkDispClearScreen();
    sdkDispRow(SDK_DISP_LINE3, 0, (u8 *) "���ڹػ�...",
               SDK_DISP_DEFAULT); //shijianglong 2013.01.07 13:57
    {
//        OS_ERR tem; OSTimeDly(1000, 0, &tem);
    }

    sdkSysAppQuit();
    ddi_sys_poweroff();
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8*pasErr
                     s32 siSize
** Returned value:
** Created By:		shiweisong  2013.04.01
** Remarks:
*****************************************************************************/
s32 sdkSysLoadDll(u8 *pasErr, s32 siSize) {
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.04.01
** Remarks:
*****************************************************************************/
s32 sdkSysUnLoadDll(void) {
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: �辢��
   ��������:	sdkDispGetScreenPixel
   ��������:	�����Ļ���ش�С
   �������:��
   �������:��
   ��   ��  ֵ:���ش�С
   �޸ı�ע:
   ����ʱ��:2012.05.28 18:48:01
*******************************************************************/
s32 sdkSysGetScreenWidthHigh(s32 *siX, s32 *siY) {
    SDK_DISP_PIXEL pixel = sdkDispGetScreenPixel();

    if (siX) {
        *siX = pixel.siX;
    }

    if (siY) {
        *siY = pixel.siY;
    }
    return SDK_OK;
}

