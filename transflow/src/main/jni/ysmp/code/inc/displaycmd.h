#ifndef MPOS_DISPLAY_CMD_H
#define MPOS_DISPLAY_CMD_H


//LCD��ʾ

#define U16_ENDIAN(a) ( (a & 0xFF00) >> 8 | (a & 0x00FF) << 8 ) //���
#define U32_ENDIAN(a) ( (a & 0x00FF) << 24 | (a & 0xFF00) << 8 | (a & 0x00FF0000) >> 8 | (a & 0xFF000000) >> 24 ) //���

typedef enum {
    DIP_CLEAR = 0x01,        //����
    DIP_SETMODE,            //������ʾģʽ
    DIP_SET_CURPOS = 0x04,    //���ù��λ��
    DIP_GET_CURPOS,            //��ȡ���λ��
    DIP_GET_SCREENATTR,    //��ȡҺ��������
    DIP_DRAW_BMP,            //��ʾλͼ
    DIP_DRAW_LINE,            //��ֱ��
    DIP_DRAW_RECT,            //������
    DIP_SET_BACKLIGHT = 0x0B, //���ñ���
            DIP_DRAW_TEXT = 0x0E,    //��ʾ�ַ���
    DIP_LOAD_MENU,            //װ�ز˵�
    DIP_CMD_END = 0xFF
} DISPLAY_CMD;

typedef struct {
    u16 xPos;
    u16 yPos;
} Point;

typedef struct _MENUITEM {
    u16 usLvl;        //�˵��ȼ�
    bool bChildMenu;
    //�����Ӳ˵�
    u8 Disp[21];    //��ʾ����
    u8 TradeCode[5];//���״���
} MenuItemStr;

//#define MENU_ITEM_SIZE (28) // 2+1+20+5

typedef struct MENU_RECORD {
    u16 MenuLvl[4];
    u8 MenuStep;        //�˵��Ĳ��
    u8 MenuTimeout;    //��ʱʱ��,��
    u16 TotalMenuItems;
    //���в˵�������
    u16 MenuDataSize;    //�˵����ݴ�С
    u8 LevelChange;    //��Ҫ�л��˵�����
    u8 CurLvlItems;    //��ǰ�����ж��ٸ��˵���
    u8 TradeCode[5];    //������
    u8 keyValue;        //��Ӧ��Ҫ�󷵻ذ���ֵ
    bool isGetTradeCode;
    //��ȡ��������,�˵��ڵ�
    bool needReCalcTimeOut;
    //��Ҫ���¼�ʱ,������ҳ��������
    MenuItemStr stMenuItem[9];//�˵������9��
} MenuRecStr;


s32 OnClear(ST_TRANSFLOW * pstTransFlow);
s32 OnSetMode(ST_TRANSFLOW * pstTransFlow);
s32 OnSetCurPos(ST_TRANSFLOW * pstTransFlow);
s32 OnGetCurPos(ST_TRANSFLOW * pstTransFlow);
s32 OnGetScreenAttr(ST_TRANSFLOW * pstTransFlow);
s32 OnDrawBmp(ST_TRANSFLOW * pstTransFlow);
s32 OnDrawLine(ST_TRANSFLOW * pstTransFlow);
s32 OnDrawRect(ST_TRANSFLOW * pstTransFlow);
s32 OnSetBackLight(ST_TRANSFLOW * pstTransFlow);
s32 OnDrawText(ST_TRANSFLOW * pstTransFlow);
s32 OnLoadMenu(ST_TRANSFLOW * pstTransFlow);

void ShowMposMenu();

extern u8 gDisplayMode;

//Xiaox 15-1-14
extern void DispWait();

#endif