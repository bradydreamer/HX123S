#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef AFX_SDK_IO_H
#define AFX_SDK_IO_H

#define STR_CURSOR                      "_"
#define SYS_TIME_FORMAT         "____-__-__ __:__:__"
#define SDKMAXT9KEYNUM      9

/************************���뷽ʽ����**********************************/
#define MODE_NONE                       0x00                    //���뷨��ʼ״̬
#define MODE_CODE_D                     0x01                    //����
#define MODE_T9                         0x02                    //��д��ĸ
#define MODE_BIG_LET                    0x03                    //��д��ĸ
#define MODE_SMA_LET                    0x04                    //Сд��ĸ
#define MODE_NUMBER                     0x05                    //����
#define MODE_SYMBOL                     0x06                    //�����ַ�
#define MODE_STROKE                     0x07                    //STARTEND
#define MODE_HAND                       0x08                    //��д����ģʽ
#define MODE_FARSI                      0x09                    //��˹��
#define MODE_TEL_LET                    0x10                    //�绰������ĸ��
#define MODE_RUSSIAN                    0x11                    //����˹
#define MODE_GEORGIA                    0x12                    //��³����
/*********************T9ƴ����һЩ�ṹ�嶨��*****************************/
#ifndef F_OK
#define F_OK                    0
#endif

typedef struct                                  //������ִ�ŵĻ���.
{
    u8 *Point;                                  //������ͷ��ַ
    u8 Len;                                     //����������
} SDK_KEY_HZMEMERY;

typedef struct {
    u32 Mask;
    u8 GetCharRslt;                             //GETCHAR�������Ϣ״̬
} SDK_KEY_GETCHARFMT;

typedef struct                                  //������ִ�ŵĻ���.
{
    u8 curr;
    u8 buf[30];                                 //����������
} SDK_KEY_STROKEHZBUF;

typedef struct                                  //ƴ���ṹ
{
    u8 Stroke[50];
    u8 StrokeNum;                               //�ʻ���
    u16 position;                               //��ǰλ��
} SDK_KEY_STROKEPOSGROUP;


typedef struct                                  //�����ṹ
{
    u16 MaxAddr;
    u16 MinAddr;
} SDK_KEY_ADDR;

typedef struct                                  //ƴ���ṹ
{
    u8 Num;                                     //���ܵ�ƴ����ϸ���
    u8 Index;                                   //��ѡ�е�ƴ��(��ƴ���ַ�������)������
    u8 String[10][10];                          //ƴ���ַ�������
    SDK_KEY_ADDR FstIndex[10];                  //һ������(���ֿ��е�,������)
} SDK_KEY_PINYINGROUP;

typedef struct                                  //��ʾ���ַ��ṹ
{
    u8 Update;                                  //�ǲ���Ҫ���¶��ֿ� 0����Ҫ,1��Ҫ
    u16 CurrentAddr;                            //��ǰҳ�Ŀ�ʼ��ַ
    u16 NextAddr;                               //��һҳ���ֵĿ�ʼ��ַ
    u16 PreAddr;                                //��һҳ���ֵĿ�ʼ��ַ
    u8 col;                                     //�����ַ�����Ҫ���Ե���
    u8 Chinese[SDK_MAX_LINE_CHAR + 1];          //��Ӧ�ĺ����ַ���
} SDK_KEY_DISPBUF;

typedef struct {
    //Modify by huangkanghui at 2015.03.25 19:01
    //SDK_KEY_PINYINGROUP MmiPinYinGroup;         //ƴ�����뷨��ƴ���ṹ
    //SDK_KEY_DISPBUF MmiDispBuf;                 //ƴ�����뷨����ʾ�ַ��ṹ
    u8 MmiCurrentMode;                          //��ǰ�����뷽ʽ
    u8 MmiKeyBuf[SDK_KEY_BUF_SIZE + 1];         // ���ռ�������Ļ�����
    u8 MmiKeyLen;                               // �����������
    u8 MmiCurrentRow;                           //��ǰ������
    u8 MmiIsInput;                              //�Ƿ���������λ��or ƴ��
    u8 MmiIsLetterEnd;                          //������ĸ����
    u8 MmiLastKey;                              //�������һ������
    u8 MmiKeyTimes;                             //ͬһ�����ּ�����Ĵ���

} SDK_KEY_SCANMEMBER;

#define psdkMmiDisp  s32 (*sdkMmiDispCallFun)(u8 *pStrDisp, const u32 uiMode, const s32 siDispRow, void *pVar)
#define sdkMmiDispFun(pStrDisp, uiMode, siDispRow, pVar) sdkMmiDispCallFun(pStrDisp, uiMode, siDispRow, pVar)

typedef struct {
    u8 Contents[SDK_MAX_STR_LEN + 1 + 1];       //�������� ������ǰ���һ�����Ⱥ�������һ�� ������
    u8 RowNo;                                   //�����к�(0-4)
    u8 MaxLen;                                  //��󳤶�����
    u8 MinLen;                                  //��С��������
    u32 Mask;                                   //������
    SDK_KEY_SCANMEMBER ScanMem;
    u32 uiFistMode;                               //��ʼ�����뷨
    psdkMmiDisp;                                //�ص���ʾ����ָ��
    void *pVar;                                 //��չָ�����
} SDK_KEY_SCANFMT;                               //������Ϣ�ṹ��

u8 sdkMmiDelete(SDK_KEY_SCANFMT *pScan);

void sdkMmiDelLastChar(u8 * str);

void sdkMmiDisplayScanf(SDK_KEY_SCANFMT *pScan);

u8 sdkMmiChangeLine(s16 flag, SDK_KEY_SCANFMT *pScan);


extern u8 sdksysGetT9keyNum(void);

extern s32 sdksysGetT9key(u8 * Keyv, u8 * Letternum);

extern void SdkGetT9Tab(u8 Tab[][5]);

extern u8 const T9pinyinTable[];

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

