#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef SDK_PRINT_H
#define SDK_PRINT_H

//-------������ӡ��ASCII��������-----------
typedef enum {
    //����
            SDK_PRN_ASCII6X8 = 0,     //��6����8
    SDK_PRN_ASCII6X12 = 1,
    SDK_PRN_ASCII8X16 = 2,
    SDK_PRN_ASCII12X24 = 3,
    SDK_PRN_ASCII16X24 = 4,
    SDK_PRN_ASCII16X32 = 5,
    SDK_PRN_ASCII16X48 = 6,
    //����Ӻ�
            SDK_PRN_ASCII8X16B = 7,
    SDK_PRN_ASCII12X24B = 8,
    SDK_PRN_ASCII16X24B = 9,
    SDK_PRN_ASCII16X32B = 10,
    SDK_PRN_ASCII16X48B = 11,
    //��Բ
            SDK_PRN_ASCII12X24Y = 12,
    SDK_PRN_ASCII16X24Y = 13,
    SDK_PRN_ASCII16X32Y = 14,
    SDK_PRN_ASCII16X48Y = 15,
    //��Բ�Ӻ�
            SDK_PRN_ASCII12X24YB = 16,
    SDK_PRN_ASCII16X24YB = 17,
    SDK_PRN_ASCII16X32YB = 18,
    SDK_PRN_ASCII16X48YB = 19,
    //����
            SDK_PRN_ASCII20X32B = 20,
    SDK_PRN_ASCII28X48B = 21,
    SDK_PRN_ASCII24X48B = 22,    //�ײ㲻֧�ָ�����Ӻ�
    SDK_PRN_ASCII16X16B = 23,
    SDK_PRN_ASCII24X48 = 24,
    SDK_PRN_ASCII5X7 = 25,

    SDK_PRN_CHSONG12X12,                        //������������12*12
    SDK_PRN_CHSONG16X16,                        //������������16*16
    SDK_PRN_CHKAI20X20,                         //�������Ŀ���20*20
    SDK_PRN_CHSONG24X24,                        //������������16*16
    SDK_PRN_CHSONG32X32,                        //������������32*32
    SDK_PRN_CHSONG48X48,                        //������������48*48
    SDK_PRN_CHSONG20X20,                        //������������20*20
    SDK_PRN_SONG12X12_TRA,              //������������12*12
    SDK_PRN_SONG16X16_TRA,              //������������16*16
    SDK_PRN_SONG20X20_TRA,              //������������20*20
    SDK_PRN_SONG24X24_TRA,              //������������24*24
    SDK_PRN_SONG32X32_TRA,              //������������32*32
    SDK_PRN_SONG48X48_TRA,              //������������48*48
    SDK_PRN_FARSI12X12,                         //��˹���ֿ�12*12
    SDK_PRN_FARSI16X16,                         //��˹���ֿ�16*16
    SDK_PRN_FARSI20X20,                         //��˹���ֿ�20*20
    SDK_PRN_FARSI24X24,                         //��˹���ֿ�24*24
    SDK_PRN_FARSI32X32,                         //��˹���ֿ�32*32
    SDK_PRN_FARSI48X48,                         //��˹���ֿ�48*48
    SDK_PRN_ARABIC12X12,                        //���������ֿ�12*12
    SDK_PRN_ARABIC16X16,                        //���������ֿ�16*16
    SDK_PRN_ARABIC20X20,                        //���������ֿ�20*20
    SDK_PRN_ARABIC24X24,                        //���������ֿ�24*24
    SDK_PRN_ARABIC32X32,                        //���������ֿ�32*32
    SDK_PRN_ARABIC48X48,                        //���������ֿ�48*48
    SDK_PRN_SJIS12X12,                          //�����ֿ�12*12
    SDK_PRN_SJIS16X16,                          //�����ֿ�16*16
    SDK_PRN_SJIS20X20,                          //�����ֿ�20*20
    SDK_PRN_SJIS24X24,                          //�����ֿ�24*24
    SDK_PRN_SJIS32X32,                          //�����ֿ�32*32
    SDK_PRN_SJIS48X48,                          //�����ֿ�48*48
    SDK_PRN_HANGUL12X12,                        //�����ֿ�12*12
    SDK_PRN_HANGUL16X16,                        //�����ֿ�16*16
    SDK_PRN_HANGUL20X20,                        //�����ֿ�20*20
    SDK_PRN_HANGUL24X24,                        //�����ֿ�24*24
    SDK_PRN_HANGUL32X32,                        //�����ֿ�32*32
    SDK_PRN_HANGUL48X48,                        //�����ֿ�48*48
    SDK_PRN_RUSSIAN6X12,                   //�����ֿ�6*12                                      //   ���ֶ����ֿ���ʱû��
    SDK_PRN_RUSSIAN8X16,                   //�����ֿ�8*16
    SDK_PRN_RUSSIAN10X20,                   //�����ֿ�10*20                                  //   ���ֶ����ֿ���ʱû��
    SDK_PRN_RUSSIAN12X24,                         //�����ֿ�12*24
    SDK_PRN_RUSSIAN16X32,                           // �����ֿ�16*32
    SDK_PRN_RUSSIAN24X48,                   //�����ֿ�24*48                                 //  ���ֶ����ֿ���ʱû��
    SDK_PRN_GEORGIAN6X12,                              // ��³����6*12
    SDK_PRN_GEORGIAN8X16,                              // ��³����8*16
    SDK_PRN_GEORGIAN10X20,                              // ��³����10*20
    SDK_PRN_GEORGIAN12X24,                              // ��³����12*24
    SDK_PRN_GEORGIAN16X32,                              // ��³����16*32
    SDK_PRN_GEORGIAN24X48,                              // ��³����24*48

    SDK_PRN_ASCII_ERR,
} SDK_ASC_FONT;

typedef enum {
    SDK_PRN_FARI_12X12 = 0,
    SDK_PRN_FARI_16X16 = 1,
    SDK_PRN_FARI_24X24 = 2,
    SDK_PRN_FARI_32X32 = 3,
    SDK_PRN_FARI_48X48 = 4,
    SDK_PRN_FARI_ERR
} SDK_FARSI_FONT;

typedef enum {
    SDK_PRN_RUSS_8X16 = 0,
    SDK_PRN_RUSS_12X24 = 1,
    SDK_PRN_RUSS_16X32 = 2,
    SDK_PRN_RUSS_ERR
} SDK_RUSS_FONT;

typedef enum {
    SDK_PRN_GEOR_6X12 = 0,
    SDK_PRN_GEOR_8X16 = 1,
    SDK_PRN_GEOR_10X20 = 2,
    SDK_PRN_GEOR_12X24 = 3,
    SDK_PRN_GEOR_16X32 = 4,
    SDK_PRN_GEOR_24X48 = 5,
    SDK_PRN_GEOR_ERR
} SDK_GEOR_FONT;


//-------������ӡ�� ���ֵ�������------------
typedef enum {
    SDK_PRN_CH12X12 = 0,
    SDK_PRN_CH16X16 = 1,
    SDK_PRN_CH20X20 = 2,
    SDK_PRN_CH24X24 = 3,
    SDK_PRN_CH32X32 = 4,
    SDK_PRN_CH48X48 = 5,            //�ײ㲻֧��

    SDK_PRN_CH_ERR,
} SDK_CH_FONT;
//-------������ӡ�� ASCII  ������������-----------
typedef enum {
    SDK_PRN_ZOOM_N = 0,            //ԭ��
    SDK_PRN_ZOOM_A = 1,            //����Ŵ�
    SDK_PRN_ZOOM_B = 2,              //����Ŵ�
    SDK_PRN_ZOOM_AB = 3,             //˫��Ŵ�
    //��ײ�δ����С��������
//     SDK_PRN_LESS_A   =  (4+1),     //������С(Ԥ���ײ�δ��)
//     SDK_PRN_LESS_B   =  (4+2),      //������С(Ԥ���ײ�δ��)
//     SDK_PRN_LESS_AB =  (4+3),      //˫����С(Ԥ���ײ�δ��)
            SDK_PRN_ZOOM_LESS_ERR
} SDK_ZOOM_LESS;

//-------����ӡ��ֻ�й̶���������,��֧���������-----------
typedef enum {
    SDK_PRN_PIN_FULL = 0x00000000,      //���� 32*16,ASC 12*8
    SDK_PRN_PIN_HALF = 0x00000001,      //����18*16,ASC 12*8
    SDK_PRN_PIN_SMALL = 0x00000003,      //���� 24*12,ASC 12*8
    SDK_PRN_PIN_DOUBLE = 0x00000005,      //����32*16,ASC 16*16��Ϊ����
} SDK_PRINT_PIN;
typedef struct {
    u32 uiLowUndef : 8; //��λ�ֽ�δ����
    u32 uiAscFont : 5;    //ASCII��������
    u32 uiAscZoom : 3;  //ASCII��������
    u32 uiChFont : 5;     //���ֵ�������
    u32 uiChZoom : 3;   //������������
    u32 uiHighUndef : 8; //��λ�ֽ�δ����
} SDK_PRINT_FONT; //�����S32 ��λ��ֵ����ָ���

typedef enum {
    SDK_PRINT_LEFTALIGN,
    SDK_PRINT_RIGHTALIGN,
    SDK_PRINT_MIDDLEALIGN
} SDK_PRINT_ALIGNMODE;

typedef enum {
    SDK_PRINT_HIGHSPEED,
    SDK_PRINT_MIDDLESPEED,
    SDK_PRINT_LOWSPEED
} SDK_PRINT_SPEED;

typedef enum                                                // needle printer has double directions
{
    SDK_PRINT_ROLL_FORWARD,
    SDK_PRINT_ROLL_BACK
} SDK_PRINT_ROLL_DIRECTION;

typedef enum {
    SDK_PRINT_PRINTING = SDK_SYS_RET_ERR_MAX - 10,   //��ӡ��
    SDK_PRINT_BAT_VOLTAGE_LOW = SDK_SYS_RET_ERR_MAX - 9,   //��ѹ̫�ͣ����ܴ�ӡ
    SDK_PRINT_PARA_ERR = SDK_SYS_RET_ERR_MAX - 8,   //��������
    SDK_PRINT_OVER_HEAT = SDK_SYS_RET_ERR_MAX - 7,   //��ӡ������//printer equipment failure
    SDK_PRINT_NO_INIT = SDK_SYS_RET_ERR_MAX - 6,   //��ӡ��δ��ʼ��
    SDK_PRINT_HANDLE_BACK = SDK_SYS_RET_ERR_MAX - 5,   //�ֱ�δ�Żص���
    SDK_PRINT_CACHE_ERR = SDK_SYS_RET_ERR_MAX - 4,   //�滺��ʧ��//store cache failed
    SDK_PRINT_LOSE_COMMAND = SDK_SYS_RET_ERR_MAX - 3,   //δ��ô�ӡ���//not get print command
    SDK_PRINT_OUTOF_PAPER = SDK_SYS_RET_ERR_MAX - 2,   //��ӡ��ȱֽ//printer is out of paper
    SDK_PRINT_DEV_FAIL = SDK_SYS_RET_ERR_MAX - 1    //��ӡ���豸����//printer equipment failure
} SDK_PRINT_STATUS;

typedef struct {
    SDK_PRINT_FONT uiFont;                                   //��ӡ����ģʽ
    s32 siOffset;                                            //��ƫ��//row offsets
    SDK_PRINT_ALIGNMODE eAlign;
    u8 pasText[512];                                         //��ӡ�ı�(���/����)  //print text(stylus print / thermal print)
    s32 siRowGap;                                           //�м��//row gap
} SDK_PRINT_DATA;


extern s32 sdkPrintInit(void);

extern s32 sdkPrintFeedPaper(SDK_PRINT_ROLL_DIRECTION eRollDirection, u8 ucDot);

extern s32 sdkPrintStr(u8 const
*pAsStr,
SDK_PRINT_FONT uiFont, SDK_PRINT_ALIGNMODE
eAlign,
s32 siOffset, s32
siRowGap);

extern s32 sdkPrintBitMap(const u8 *pAsBmpFile, SDK_PRINT_ALIGNMODE eAlign, s32 siOffset);

extern s32 sdkPrintStart(void);

extern s32 sdkPrintStartNoRollPaper(void);

extern s32 sdkPrintStartNoRollBack(void);

extern s32 sdkPrintSetLastRowGap(s32 siDoit);

extern s32 sdkPrintMultStr(s32 siRowGap, const SDK_PRINT_DATA *pData, ...);

extern s32 sdkPrintStartWithMultFont(void);

extern s32 sdkMultiPrintBitMap(const u8 *pAsBmpFile, SDK_PRINT_ALIGNMODE eAlign, s32 siOffset);

extern s32 sdkPrintGetState(s32 siTimeOut);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

