/******************************************************************************

                  ��Ȩ���� (C), 2010-2014, �������¹��������ɷ����޹�˾

******************************************************************************
   �� �� ��   : sdkdevaudio.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2013��8��1��
   ����޸�   : 2014��5��16��
   ��������   : ��ƵSDK�豸��ӿ�ʵ��Դ���ļ�
   �����б�   :
              sdkSysSetVolume
              sdk_dev_audio_get_volume
              sdk_dev_audio_close
              sdkSysAudioOpen
              sdk_dev_audio_pause
              sdk_dev_audio_play
              sdk_dev_audio_replay
              sdk_dev_audio_stop
              sdk_dev_beep_err
              sdk_dev_beep_ok
              sdk_dev_beep_play
              sdkSysPlaySoundFile
   �޸���ʷ   :
   1.��    ��   : 2013��8��1��
    ��    ��   : shiweisong
    �޸�����   : �����ļ�
   2.��    ��   : 2014��5��16��
    ��    ��   : wangmingming
    �޸�����   : �޸��ļ�

******************************************************************************/
#include "sdkdevOption.h"
#include "sdkGlobal.h"

#ifndef  DEBUG_AUDIO
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define DRIVER_BUZZER_IS_EXIST                      1 //������ �Ƿ����  0:�����ڣ�1:����
#define DRIVER_AUDIO_IS_EXIST                       0 //��Ƶ������ �Ƿ����  0:�����ڣ�1:����


/*****************************************************************************
** Descriptions:	����Ƶ�豸
** Parameters:          ��
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1����Ƶ�豸�ǿ������豸�������δ򿪡�
*****************************************************************************/
s32 sdkSysAudioOpen(void) {
    s32 rtn = ddi_audio_open();

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	    ��ʼ��ϵͳ����
** Parameters:          void:
** Returned value:	�ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR
** Created By:		wangmingming  2014.06.12
** Modified By:
** Remarks:                 SDKĬ������ΪDEFAULT_AUDIO_VOLUME(5)
*****************************************************************************/
s32 sdkSysSetVolume(u32 Vol) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	    ��ȡϵͳ��ǰ����
** Parameters:          void:
** Returned value:	���ص�ǰϵͳʹ�õ�����
** Created By:		wangmingming  2014.06.12
** Modified By:
** Remarks:                 SDK�ڳ�ʼ��ʱ������Ĭ������ΪDEFAULT_AUDIO_VOLUME(5)
*****************************************************************************/
s32 sdkSysGetVolume(void) {
    return 0;
}

/*****************************************************************************
** Descriptions:	�ر���Ƶ�豸
** Parameters:          ��
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1.  �ر�δ����Ƶ�豸��DDI�ӿ�ֱ�ӷ��� DDI_OK��
*****************************************************************************/
s32 sdkSysAudioClose(void) {
    s32 rtn = ddi_audio_close();

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	����ָ������Ƶ�ļ�
** Parameters:          const u8 *lpfile:����·������Ƶ�ļ���
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1���������󷵻�-4��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1.  ʹ�÷�������ʽ���š�
                    2.  ��ʱֻ֧�� wav ��ʽ����Ƶ�ļ���
                    3.  ��ʹ�÷� wav ��ʽ����Ƶ�ļ�ʱDDI���� DDI_EINVAL ��
                    4.  wav ��ʽ��8k��16bit��˫ͨ����
*****************************************************************************/
s32 sdkSysAudioPlay(const u8 *lpfile) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	ֹͣ��Ƶ����
** Parameters:          ��
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1.  �������ֹͣ����/��ͣ����״̬��DDI�ӿ�ֱ�ӷ��� DDI_OK��
*****************************************************************************/
s32 sdkSysAudioStop(void) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ͣ��Ƶ����
** Parameters:          ��
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1.  �������ֹͣ����/��ͣ����״̬��DDI�ӿ�ֱ�ӷ��� DDI_OK��
*****************************************************************************/
s32 sdkSysAudioPause(void) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�ָ���Ƶ����
** Parameters:          ��
** Returned value:
                    �ɹ�����1��ʧ�ܷ���-1��
** Created By:		wangmingming  2014.05.12
** Remarks:
                    1.  �������ֹͣ����״̬��DDI�ӿ�ֱ�ӷ��� DDI_OK��
*****************************************************************************/
s32 sdkSysAudioRepaly(void) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	���Ʒ���������
** Parameters:
                    u32 frequency:����������Ƶ��
                    u32 times:����������ʱ��
** Returned value:
                    �ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR���������󷵻�SDK_PARA_ERR��
** Created By:		���ƽ  2014.07.28
** Remarks:

*****************************************************************************/
s32 sdkSysAudioBuzzer(u32 frequency, u32 times) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ��Ƶ�豸״̬
** Parameters:
                    u32 playState:
** Returned value:
                    �ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR���������󷵻�SDK_PARA_ERR��
** Created By:		���ƽ  2014.07.28
** Remarks:
*****************************************************************************/
s32 sdkSysAudioState(u32 *playState) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡ��Ƶ�豸�汾
** Parameters:          u8 *pVerOut ������Ƶ�豸�汾��Ϣ \0��β
** Returned value:	�ɹ�:SDK_OK  ʧ��:SDK_ERR
** Created By:		���ƽ  2014.07.25
** Remarks:             \0��β����Ч�ַ��̶�����Ϊ17bytes
*****************************************************************************/
s32 sdkSysAudioVersion(u8 *pVerOut) {
    s32 rtn = ddi_audio_ioctl(DDI_AUDIO_CTL_VER, 0, (u32) pVerOut);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else {
        Assert(0);
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ϲ���
** Parameters:
                    u32 soundNmb: Ҫ���ŵ���������
                    u8 soundPath[]: Ҫ���ŵ�����·������(����)
** Returned value:
                    �ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR���������󷵻�SDK_PARA_ERR��
** Created By:		���ƽ  2014.05.12
** Remarks:
*****************************************************************************/
s32 sdkSysAudioPlayComb(u32 soundNmb, const u8 *soundPath[]) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void:
** Returned value:
** Created By:		wangmingming  2014.05.12
** Remarks:
*****************************************************************************/
s32 sdkSysAudioBeepOk(void) {
    s32 rtn = DDI_OK;

    rtn = ddi_audio_ioctl(DDI_AUDIO_CTL_BUZZER, 2, 20); //��������  Ƶ��3000
    //ddi_sys_msleep(30);
    Trace("zjp", "sdkSysAudioBeepOk rtn = %d\r\n", rtn);

    if (rtn == DDI_ERR) {
        return SDK_ERR;
    }
    else if (rtn == DDI_EINVAL) {
        return SDK_PARA_ERR;
    }
    else if (rtn == DDI_EIO) {
        return SDK_EIO;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.07.29
** Remarks:
*****************************************************************************/
s32 sdkSysAudioBeepErr(void) {
    s32 rtn = SDK_OK;

    rtn = ddi_audio_ioctl(DDI_AUDIO_CTL_BUZZER, 2, 30); //�������� Ƶ��3000
    ddi_sys_msleep(30 + 190);

    if (rtn == DDI_OK) {
        rtn = ddi_audio_ioctl(DDI_AUDIO_CTL_BUZZER, 2, 30); //��������  Ƶ��3000
        ddi_sys_msleep(30 + 30);
    }

    if (rtn == DDI_OK) {
        rtn = ddi_audio_ioctl(DDI_AUDIO_CTL_BUZZER, 2, 30); //��������  Ƶ��3000
        //ddi_sys_msleep(30);
    }
    Trace("zjp", "sdkSysAudioBeepErr rtn = %d\r\n", rtn);

    if (rtn == DDI_ERR) {
        return SDK_ERR;
    }
    else if (rtn == DDI_EINVAL) {
        return SDK_PARA_ERR;
    }
    else if (rtn == DDI_EIO) {
        return SDK_EIO;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���������ļ�  ֻ֧��linuxϵͳ  ucϵͳ��beepһ��
** Parameters:       u8 const * file ������Ŀ¼ ������Ӧ�ò�ͬ���ͼ���ֲ
                     s32 leve   0~7
** Returned value:
** Created By:		shiweisong  2013.06.14
** Remarks:
           Linux ϵͳ�Ļ����ļ��Ƿ���Ĭ���ļ����� /mtd0/res/
           ��Ҫ���ŵ��ļ����������Ӧ����Ĭ�ϵ�Ŀ¼�·�ͻ�ʧ��
*****************************************************************************/
s32 sdkSysPlaySoundFile(const u8 *pfile, s32 leve) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	���Ÿ������͵����� ֧�����еĻ���
** Parameters:          SDK_SYS_SOUND_TYPE type
                    s32 level
** Returned value:
** Created By:		shiweisong  2013.06.14
** Remarks:
*****************************************************************************/
s32 sdkSysPlaySound(SDK_SYS_BEEP_TYPE type, s32 level) {
    return SDK_ERR;
}

