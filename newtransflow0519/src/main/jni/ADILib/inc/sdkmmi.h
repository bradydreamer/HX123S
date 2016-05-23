#ifndef AFX_SDK_SDKMMI_H
#define AFX_SDK_SDKMMI_H

#define SDK_MAX_STR_LEN             127                 //����������ݳ���//the max input length
#define SDK_MAX_LINE_CHAR                               20                              //ÿ�������ʾ�ַ�����//the maximum number of display characters per line
#define SDK_KEY_BUF_SIZE                        128                                     //�������뻺��ĳ���//the length of buffer of keyboard entry

/**************************** scanf ���붨��scanf mask definition********************************/
#define SDK_MMI_NUMBER              0x00000001              //������������//allow enter number
#define SDK_MMI_SYMBOL              0x00000002              //��������ɼ��ַ�//enter special visible character
#define SDK_MMI_HEX                 0x00000004              //����16����//allow hex
#define SDK_MMI_LETTER              0x00000008              //����������ĸ//allow enter letter
#define SDK_MMI_HZ                  0x00000010              //�������뺺��//allow enter Chinese characters
#define SDK_MMI_LEFT                0x00000020              //�����(Ϊ����Ϊ�Ҷ���)  //left-aligned(if 0 then right-aligned)
#define SDK_MMI_POINT               0x00000040              //�����С��������(���)  //enter numbers with a decimal point(amount)
#define SDK_MMI_PWD                 0x00000080              //��������(����Ϊ*, ���������������ֺ���ĸ-ä��(*^__^*) )//����(Ϊ����Ϊ����)  //enter password(display *, now allow enter number and letter - touch type ) //reverse display(if 0 then normal display)
#define SDK_MMI_TIME                0x00000100              //����ʱ��//enter time
#define SDK_MMI_MULTTASK            0x00000200              //Function����Ӧ���л���-���ǩ��//Function is used to switch application - aim at login
#define SDK_MMI_DISP                0x00000400              //����(Ϊ����Ϊ����)  //reverse display(if 0 then normal display)
#define SDK_MMI_STROKE              0x00000800              //�ʻ������ַ�//STARTEND   //enter special visible characters
#define SDK_MMI_HAND                0x00001000              //��д����
#define SDK_MMI_NOINPUT_QUIT        0x00002000              //�����ڽ������뷨֮��û�������ַ�ǰ��ֱ��ȷ���˳�
#define SDK_MMI_FARSI               0X00004000              //���벨˹��
#define SDK_MMI_RUSSIAN             0x00008000              //�������˹��
#define SDK_MMI_GEORGIA             0x00010000              //���������³������
#define SDK_MMI_DEFAULT_STR   (SDK_MMI_NUMBER|SDK_MMI_LETTER|SDK_MMI_HZ)
//enum SDK_DISP_LINE;

/*********************************************************************
��׼��Standard Version
**********************************************************************/
extern s32 sdkKbGetScanf(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen,
                         const u32 uiMode, const s32 siDispRow);

extern s32 sdkKbGetScanfEx(const s32 siOvertime, u8 *pheOut, const s32 siMinLen, const s32 siMaxLen,
                           const u32 uiMode, const s32 siDispRow,
                           s32 (*psdkCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow,
                                              void *pVar), void *pVarible, u32 siFirstMode);

#endif
