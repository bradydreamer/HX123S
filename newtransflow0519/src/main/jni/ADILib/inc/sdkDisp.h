#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#ifndef SDK_IO_H
#define SDK_IO_H


#define SDK_DISP_LINE1              0                                              //��ʾ�к�1  //display line number 1
#define SDK_DISP_LINE2              1                                              //��ʾ�к�2  //display line number 2
#define SDK_DISP_LINE3              2                                              //��ʾ�к�3  //display line number 3
#define SDK_DISP_LINE4              3                                              //��ʾ�к�4  //display line number 4
#define SDK_DISP_LINE5              4                                              //��ʾ�к�5  //display line number 5
#define SDK_DISP_LINE_MAX           5                                              //��ʾ�к������//the max line number of display


#define SDKDISPMAXLINENO            5
#define SDKDISPMAXLINELEN           20


/**************************** ��ʾ���Զ���Display Attribute Definition********************************/
#define SDK_DISP_FDISP              BIT_0                                          //����//normal display
#define SDK_DISP_NOFDISP            0x00                                           //����//revert display
#define SDK_DISP_INCOL              BIT_1                                           //����һ��//insert a row
#define SDK_DISP_LDISP              BIT_2                                           //�����//flush left
#define SDK_DISP_CDISP              BIT_3                                           //����//center
#define SDK_DISP_RDISP              BIT_4                                           //�Ҷ���//flush right
#define SDK_DISP_NOFDISPLINE        BIT_5                                           //����//revert display
#define SDK_DISP_CTRLA              BIT_7                                           //�������ַ���һ��ʱ������л���'\xa'��س�'\xd'�ַ�,CTRLAȡ����,����λΪ0ʱ��������//when a line is full of characters, behind with new line character '\xa' or enter character '\xd' and CTRLA is usefull, if this bit is 0 then useless
#define SDK_DISP_DEFAULT            (SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL)   //���Ծ���//normal display, center
#define SDK_DISP_LEFT_DEFAULT       (SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL)   //�����//flush left
#define SDK_DISP_RIGHT_DEFAULT      (SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL)   //�Ҷ���//flush right

#define SDK_DISP_OVERLINE BIT_8   //�����б�Ǻ���
#define SDK_DISP_DOWNLINE BIT_9   //�����б�Ǻ���
#define SDK_DISP_SIDELINE BIT_10   //�����п���

/**************************************������Error Code*************************************/
#define SDK_DISP_SIZEERRO           -101                                           //�����С�������(��Ļ��x ��Ļ��x4)  //the buffer must be bigger than (screen wide multiply screen high multiply 4)
#define SDK_DISP_MULTI_OVER_DISP    (SDK_DISP_ERR_BASE - 1) //???????????????                        //�����ʾ���ݳ���һ��
/**************************��ɫ(��ɫ��)Color(monochrome screen)****************************************/

typedef struct {
    s32 siX;                                                                       //��//wide
    s32 siY;                                                                       //��//high
} SDK_DISP_PIXEL;
/* ��Ļ���� screen pixel*/

typedef enum {
    SDK_DISP_BATTERY = 1,                               //���ͼ��
    SDK_DISP_PLUG,                                      //��Դͼ��
    SDK_DISP_SIGNAL,                                    //�ź�ͼ��
    SDK_DISP_GPRS,                                      //GPRSͼ��
    SDK_DISP_CDMA,                                      //CDMAͼ��
    SDK_DISP_WIFI,                                      //WIFIͼ��
    SDK_DISP_BT,                                 //����ͼ�� 
    SDK_DISP_AUDIO,                                     //??????
    SDK_DISP_USB,                                     //USB????       
} SDK_DISP_ICON;

typedef struct {
    SDK_ASC_FONT eAscFont;                                   //asc��?��??�꨺?
    SDK_CH_FONT eChFont;                                   //oo��?��?��??�꨺?
    s32 siX;                                            //o����?����
    s32 siY;                                            //��Y��?����
    u8 pasText[64];
} SDK_DISP_MULT_DATA;

typedef struct {
    s32 siOverNode;                                    //����һ����ʾ���ַ���
    s32 siStartByte;                                   //������ʾ���ַ�����ʼ�ڵ�
    void *pVar;                                        //��������ָ��
} SDK_DISP_MULT_RET_INFO;

typedef struct {
    u8 fileName[50];
    u32 Offset;
    u32 len;
    u32 high;
} SDK_FONT;//zhouzhihua 2013.06.19 11:4


typedef struct                          //������ʽ
{
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
} SDK_DISP_SRECT;


typedef enum {
    SDK_DISP_FONT_SMALL = 12,
    SDK_DISP_FONT_NORMAL = 16,
    SDK_DISP_FONT_BIG = 24,
    SDK_DISP_FONT_LARGE = 32,
    SDK_DISP_FONT_HUGE = 48
} SDK_DISP_FONT;

typedef enum {
    SDK_BLACK_COLOR_ID = 0,
    SDK_YELLOW_COLOR_ID,
    SDK_LGREEN_COLOR_ID,
    SDK_BLUE_COLOR_ID,
    SDK_GREEN_COLOR_ID,
    SDK_RED_COLOR_ID,
    SDK_WINDOWS_BLUE_ID,
    SDK_WHITE_COLOR_ID
} SDK_COLOR_ID;


#define    SDK_DISP_BLACK_COLOR     sdkSysGetColorValue(SDK_BLACK_COLOR_ID)
#define    SDK_DISP_YELLOW_COLOR    sdkSysGetColorValue(SDK_YELLOW_COLOR_ID)
#define    SDK_DISP_LGREEN_COLOR    sdkSysGetColorValue(SDK_LGREEN_COLOR_ID)
#define    SDK_DISP_BLUE_COLOR      sdkSysGetColorValue(SDK_BLUE_COLOR_ID)
#define    SDK_DISP_GREEN_COLOR     sdkSysGetColorValue(SDK_GREEN_COLOR_ID)
#define    SDK_DISP_RED_COLOR       sdkSysGetColorValue(SDK_RED_COLOR_ID)
#define    SDK_DISP_WINDOWS_BLUE    sdkSysGetColorValue(SDK_WINDOWS_BLUE_ID)
#define    SDK_DISP_WHITE_COLOR     sdkSysGetColorValue(SDK_WHITE_COLOR_ID)

/**************************************��������Function Definition***********************************/
/*******************************************************************************
   ��׼�溯�� Standard Functions
 ********************************************************************************/
extern SDK_DISP_PIXEL sdkDispGetScreenPixel(void);

extern s32 sdkDispFillRowRam(s32 siRow, s32 usColid, const u8 *pasStr, u32 ucAtr);

extern s32 sdkDispAt(s32 siX, s32 siy, const u8 *pasStr);

extern void sdkDispClearScreen(void);

extern s32 sdkDispClearRow(s32 siRow);

extern s32 sdkDispClearAt(s32 siX0, s32 siY0, s32 siX1, s32 siY1);

extern void sdkDispBrushScreen(void);

extern s32 sdkDispSetBackground(s32 siX0, s32 siY0, s32 siXlen, s32 siYlen,
                                const u8 *pasBmpFileName);

extern s32 sdkDispShowBmp(s32 siX0, s32 siY0, s32 siLen, s32 siYlen, const u8 *pasBmpFileName);

extern s32 sdkDispDrawProgbar(s32 siRow, s32 siX1, s32 siX2, u8 ucValue);

/*****************************�����ӿں�������͸��Other Functions Call These Functions****************************/
extern s32 sdkDispRow(s32 siRow, s32 col, const u8 *pasStr, u32 ucAtr);

extern s32 sdkDispClearRowRam(s32 siRow);

extern s32 sdkDispClearRowRamAt(s32 siRow, s32 siStartCol, s32 siEndCol);

extern s32 sdkDispRestoreScreen(const u16 *pusLcdmem, s32 siLen);

extern u32 sdkDispGetScreenBufferSize(void);

extern s32 sdkDispRowAt(s32 siRow, s32 sicol, const u8 *pasStr, u32 ucAtr);

extern s32 sdkDispSaveScreen(const u16 *pusLcdmem, s32 siLen);

extern s32 sdkDispDrawProgbarRam(s32 siRow, s32 siX1, s32 siX2, u8 ucValue);

extern s32 sdkDispDrawLine(s32 siX0, s32 siY0, s32 siX1, s32 siY1,
                           u32 siColor); //shijianglong 2013.01.23 16:28
extern s32 sdkDispPutPixel(s32 siX, s32 siY, u32 siColor); //shijianglong 2013.01.10 15:57
extern s32 sdkDispGetPixel(s32 siX, s32 siY); //shijianglong 2013.01.10 15:57
extern s32 sdkDispIcon(const SDK_DISP_ICON eIcon, u8 flag, s32 siX,
                       s32 siY); //shijianglong 2013.06.25 11:43
extern void sdkDispSetBackColor(u32 usColor);

extern s32 sdkDispDrawDotRam(s32 siX, s32 siY, u32 ucColor);

extern s32 sdkDispDrawDot(s32 siX, s32 siY, u32 ucColor);

extern s32 sdkDispFormatFillRam(u8 ucAtr, const SDK_DISP_MULT_DATA *pData, ...);

//shijianglong 2013.06.26 15:37
extern s32 sdkDispFormatFillRamEx(u8 ucAtr, SDK_DISP_MULT_RET_INFO *pstRetInfor,
                                  const SDK_DISP_MULT_DATA *pData, ...);

extern void sdkDispSetFontColor(u32 usColor);

extern s32 sdkDispGetFontColor(void);

extern s32 sdkDispSetFontSize(const SDK_DISP_FONT siFontSize);

extern s32 sdkDispGetFontSize(void);

extern s32 sdkDispSetRect(SDK_DISP_SRECT *pRect, const s32 siX0, const s32 siY0, const s32 siX1,
                          const s32 siY1);

extern s32 sdkDispSetRectColor(const SDK_DISP_SRECT *pstRect, const u32 usColor);

extern s32 sdkDispNofdispAt(s32 siStartX, s32 siStartY, s32 siEndX, s32 siEndY);

/*******************************************************************
   ��    Ȩ:    �¹���
   ��    ��:    ���ƽ
   ��������:    sdkDispShowBmpdots
   ��������:	��ʾͼƬ(��������)
   �������:
                dispRect,ͼƬҪ��ʾ������
                bmpWidth,bmpheight,ͼƬ�����
                pBmpDots ͼƬ����ָ�룬��ɫ����Һ����ģ��
                        ����ȡģ���ֽڵ���
   �������:    ��
   �� �� ֵ:    SDK_PARA_ERR:��������SDK_OK:�������óɹ���SDK_ERR:ʧ��
   �޸ı�ע:
   ����ʱ��:    012.11.02 10:43:59
*******************************************************************/
extern s32 sdkDispShowBmpdots(const SDK_DISP_SRECT dispRect, u32 bmpWidth, u32 bmpheight,
                              const u8 *pBmpDots);

extern s32 sdkDispStateBar(bool statBarFlag);

extern s32 sdkDispClearStateBar(void);

extern s32 sdkSysGetColorValue(SDK_COLOR_ID eColorId);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

