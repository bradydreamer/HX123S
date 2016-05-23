#ifndef AFX_PINPAD_H
#define AFX_PINPAD_H


typedef struct {
    u8 heTpk[16];
    u8 heTpkcheckvalue[8];
    u8 heTak[16];
    u8 heTakcheckvalue[8];
    u8 heTdk[16];
    u8 heTdkcheckvalue[8];
} ST_KEYDATA;


/***********************����Կ����************************/
//0-99 ��DES����,100Ϊ������Կ�� 101Ϊ����Կ��ʱ��
//102-201 3DES���� 202Ϊ3DES��ʱ��

/***********************������Կ����************************/
//2009/12/31��ʼ,�µ���Կ��֧�����100�鹤����Կ.

//0-19 150-254��DES���� ���128��, 20,21Ϊ��ʱ������,
//22-41 44-149 Ϊ3DES����,42,43Ϊ3DES��ʱ����
//��ʱ��������������ڹ�������ķ�Χ,������������������ѡ��15,16
/*******************************3DES������Կ�洢���****************************/
#define MK_DESTHRESHOLD                0                                        //DES����Կ��ƫ��λ�� 0-99 (100)
#define MK_3DESTHRESHOLD                102                                        //3DES����Կ��ƫ��λ�� 102-201 (100)

#define WK_DESTHRESHOLD                0                                        //DES������Կ��ʼλ�� 0-19 150-254  (128)
#define WK_3DESTHRESHOLD                22                                        //3DES������Կ��ʼλ�� 22-41 44-148 (128)

#define WK_DESTHRESHOLDEX                150                                        //DES������Կ��չ��ʼλ�� 0-19 150-254  (128)
#define WK_3DESTHRESHOLDEX            44                                        //3DES������Կ��չ��ʼλ�� 22-41 44-148 (128)

/*******************************������Կ�洢���****************************/
#define TPK_STORENO                    0x00                                    //�洢TPK��������̵ı��
#define TAK_STORENO                    0x01                                    //�洢TAK��������̵ı��
#define TDK_STORENO                    0x13                                    //�洢TDK��������̵ı��

#define TPK_3DESSTORENO                (TPK_STORENO+WK_3DESTHRESHOLD)        //�洢TPK��������̵ı��,ƫ����22
#define TAK_3DESSTORENO                (TAK_STORENO+WK_3DESTHRESHOLD)        //�洢TPK��������̵ı��,ƫ����22
#define TDK_3DESSTORENO                 (TDK_STORENO+WK_3DESTHRESHOLD)        //�洢TPK��������̵ı��,ƫ����22

#define TPK_STORENOEX                    (TPK_STORENO+WK_DESTHRESHOLDEX)        //�洢TPK��������̵ı��
#define TAK_STORENOEX                    (TAK_STORENO+WK_DESTHRESHOLDEX)        //�洢TAK��������̵ı��
#define TPK_3DESSTORENOEX            (TPK_STORENO+WK_3DESTHRESHOLDEX)    //�洢TPK��������̵ı��,ƫ����22
#define TAK_3DESSTORENOEX            (TAK_STORENO+WK_3DESTHRESHOLDEX)    //�洢TPK��������̵ı��,ƫ����22

/********************************�ݴ湤����Կ�洢���***********************/
#define TPK_TEMPSTORENO                20                                        //�ݴ�TPK��������̵ı��(С������ָ��)
#define TAK_TEMPSTORENO                21                                        //�ݴ�TAK��������̵ı��
#define TPK_3DESTEMPSTORENO            (TPK_TEMPSTORENO+WK_3DESTHRESHOLD)    //�ݴ�TPK��������̵ı��(С¬ָ��)
#define TAK_3DESTEMPSTORENO            (TAK_TEMPSTORENO+WK_3DESTHRESHOLD)    //�ݴ�TPK��������̵ı��(С¬ָ��)

#define PCI_DESTEMPSTORENO            0                                   //��������ʱ������ 0-15 ֧������Կ�͹�����Կ
#define PCI_3DESTEMPSTORENO            16                                  //��������ʱ������ 16-31֧������Կ�͹�����Կ


extern s32 PinPad_EncryptPwd(u8 *lpOut, u8 *lpIn, u16 nLen);

extern s32 PinPad_EncryptMac(u8 *lpOut, u8 *lpIn, u16 nLen);

extern s32 PinPad_EncryptData(u8 *lpOut, u8 *lpIn, u16 nLen);

extern s32 WriteSerialNo(u8 *lpIn, u16 nLen);

extern s32 ReadSerialNo(u8 * lpOut, u16 * nLen);

extern s32 EncryptPwd(u8 *lpOut, u16 *outLen, u8 *lpIn, u16 nLen);

extern s32 EncryptMac(u8 *lpOut, u8 *lpIn, u16 nLen);

extern s32 EncryptData(u8 *lpOut, const u8 *lpIn, u16 nLen);

extern s32 CheckMac(u8 *lpIn, u16 nLen, u8 *Mac);

extern u16 MacDataFormat(u8 *lpOut, u8 *lpIn, u16 nLen);

u8 SendDataToPcsoft(u8 *lpin, u16 len);

void SendComData_dll(u8 cmd, u8 *Dest, u16 len);

extern s32 E98(u8 *lpOut, u8 *lpPan, const u8 *pbcAmt, u8 Row);

extern void ReadPinPadInfo(void);

extern s32 DecryptWK(ST_KEYDATA *pstEncryptKeyData);

extern u8 GetCurWkIndex(u8 ucIndex);

extern void ReadPedParam(void);

#endif

