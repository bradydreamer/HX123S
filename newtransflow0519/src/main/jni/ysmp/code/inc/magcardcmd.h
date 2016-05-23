#ifndef MPOS_MAGCARD_CMD_H
#define MPOS_MAGCARD_CMD_H


EXTERN u8
gPlatformSerialNO[12];

EXTERN u8
gPlatformRandomNO[6];


typedef enum {
    MAG_OPEN_DEVICE = 0x01,        //�򿪶�����
    MAG_CLOSE_DEVICE,            //������������
    MAG_READ_CARDDATA_NEN = 0x04,    //�����������ݣ����ģ�
    MAG_READ_CARDDATA_EN,    //�����������ݣ����ģ�
    MAG_READ_CARDDATA_INSIDE_SAVE,    //�������������ڲ�����
    MAG_READ_CARDDATA_AUDIO,
    MAG_CMD_END = 0xFF
} MAGCARD_CMD;


#define MAG_CARD_MSK    0x01
#define IC_CARD_MSK        0x02
#define RF_CARD_MSK    0x04


typedef struct _TAG_PARSE {
    unsigned char ucTag[3]; //��ǩ
    unsigned short usMinLen; //������С����
    unsigned short usMaxLen; //������󳤶�
    bool *bExist; //��ǩ�Ƿ����
    unsigned short int *pucLen; //��ǩ��Ӧ���ݳ���
    unsigned char *pucData; //��ǩ����
} TAG_PARSE;


#define MAG_TLV_FILENAME "/mtd0/mpos/MagTlv.dat"

typedef struct MAG_TLV {
    u8 tag[3];
    u16 realLen;
    //val��ʵ����
    u16 dataLen;
    u8 data[300];//T+L+���ܵ�VAL
} MagTlvStr;

//u16 FormTlv(u8 *lpOut, u8 *tag, u8 taglen, u8 *value, u16 valuelen);
u16 FormTlv(u8 *lpOut, u8 *tag, u8 *value, u16 valuelen);

u8 ParseExtLenT(u8 * ParseData, u16 * index, u16 * DataLen);

u16 ParseTlv(u8 *pucDataIn, u16 uiDataLen, TAG_PARSE *pstTagParse, u8 ucTagNum);

u16 ParseCommTlv(u8 *pucDataIn, u16 uiDataLen, u8 *outtag, u8 *outtaglen, u16 *outdatalen,
                 u8 *outtagdata);

u8 ParseFixedTlv(u8 *pucDataIn, u16 uiDataLen, u8 *intag, u16 *outdatalen, u8 *outtagdata);

s32 OnOpenMagCardDevice(ST_TRANSFLOW * pstTransFlow);
s32 OnCloseMagCardDevice(ST_TRANSFLOW * pstTransFlow);
s32 OnReadMagCardNoEncrypt(ST_TRANSFLOW * pstTransFlow);
s32 OnReadMagCardEncrypt(ST_TRANSFLOW * pstTransFlow);
s32 OnReadMagCardEncryptInsideSave(ST_TRANSFLOW * pstTransFlow);
s32 OnReadMagCardAudio(ST_TRANSFLOW * pstTransFlow);

u8 GetCurrentCardType();

u8 GetPlatformSerialNO(u8 * pOut);//�̶�����12�ֽڵ�pOut

#endif
