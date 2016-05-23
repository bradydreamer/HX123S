#ifndef SDK_TOUCHSCREEN_H
#define SDK_TOUCHSCREEN_H

#define MESSAGEBMPDIR   "/mtd0/res/bmp/messagebox.bmp"
typedef struct                          //�������ʽ
{
    s32 x, y;
    u8 Pressed;
} SDK_TS_POINT;

typedef struct {
    s32 row;
    s32 col0;
    s32 col1;
    s32 val;
} SDK_TS_TABLE;

typedef struct mapnode {
    SDK_DISP_SRECT stRect;
    u8 siKey;
} SDK_TS_MAPNODE;

extern void sdkTsOpen(void);                                                  //����������
extern void sdkTsClose(void);                                                 //�رմ�����
extern void sdkTsFlush(
        void);                                                   //�����ǰ������������
extern s32 sdkTsAddKeyMap(const SDK_DISP_SRECT *pstRect,
                          const s32 siKey);                      //���Ӿ���-��ֵӳ��ڵ�
extern s32 sdkTsDelKeyMap(
        const s32 siKey);                                           //ɾ������-��ֵӳ��ڵ�
extern s32 sdkTsFreeKeyMap(void);

extern s32 sdkTsInit(void);                                          //��ʼ��K390����������
extern s32 sdkTsAdjust(void);                                                   //У׼������
extern s32 sdkTsGetScanf(const s32 siOvertime, u8 *pOut, s32 siMinLen, s32 siMaxLen,
                         const s32 siMode, const SDK_DISP_SRECT *pstRect);  //��ȡ��������д����
extern s32 sdkTsGetStrokesRect(
        SDK_DISP_SRECT * pRect);                            //��ȡ�Ȼ�����������
extern s32 sdkTsSetStrokesRect(
        const SDK_DISP_SRECT *pstRect);                            //���ñȻ�����������
extern s32 sdkTsGetListRect(
        SDK_DISP_SRECT * pRect);                               //��ȡ��ѡ������������
extern s32 sdkTsSetListRect(
        const SDK_DISP_SRECT *pstRect);                               //���ú�ѡ������������
extern s32 sdkTsGetKeyValue(void);                                              //��ȡ�������ֵ
extern s32 sdkTsGetPollState(SDK_TS_POINT *pPoint);

extern s32 sdkTsWarning(u8 *pInfo, u8 uiRow);

extern s32 sdkTsMessageBox(u8 *pMsg, u8 *pFileName, SDK_TS_MAPNODE *pBmpButton, s32 siKeyNum);

#endif

