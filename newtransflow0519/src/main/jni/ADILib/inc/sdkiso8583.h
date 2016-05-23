#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SDK_8583_BAGMAXLEN 		2048     //�洢���ĳ���
#define SDK_8583_FIELDMAX        128



#define SDK_8583_FIELD_MSG       0
#define SDK_8583_FIELD_MAP       1

typedef enum
{
    SDK_8583_LEN_ASC =  (0x01),   //ASCII
    SDK_8583_LEN_BCD =  (0x02),   //BCD ���ʱ��Ҫѹ������
    SDK_8583_LEN_HEXL = (0x03),  //hex ��λ��ǰ
    SDK_8583_LEN_HEXR = (0x04),  //hex ��λ��ǰ
}SDK_8583_LEN_TYPE;

typedef enum
{
    SDK_8583_DATA_ASC = (0x01),   //ASCII
    SDK_8583_DATA_BCD = (0x02),   //BCD ���ʱ��Ҫѹ������
    SDK_8583_DATA_BIT = (0x03),   //bit ���ʱԭ��copy
}SDK_8583_DATA_TYPE;

typedef enum
{
    SDK_8583_ALIGN_L = (0x01),    //����� ����DO_TYPE_HEX�����ݱ�ʾ��λ��ǰ
    SDK_8583_ALIGN_R = (0x02),    //�Ҷ��� ����DO_TYPE_HEX�����ݱ�ʾ��λ��ǰ
}SDK_8583_DATA_ALIGN;

typedef struct
{
    u8 nFLenLen;                  //�򳤶�ָʾλ����
    u8 ucFLenT;                   //�򳤶���������

    u16 nFDataLen;                //��������󳤶�
    u8 ucFDataT;                  //��������������
    u8 ucFDataA;                  //�����ݶ��뷽ʽ
    u8 ucFDataS;                  //�����ݲ���ʱ�����ַ�
}SDK_8583_STFIELDDEF;


//ĳ�����ڱ����е�λ������
typedef struct
{
    u16 nFieldHead;               //��������ucBagData�ṹ�е���ʼλ��
    u16 nFieldLen;                //ʵ�ʳ���
}SDK_8583_STFIELDATA;

typedef struct
{
    u8 ucBagData[SDK_8583_BAGMAXLEN];      //��������
    u16 nBagLen;                  //���ĳ���
    u8 nEndFiled;                 //������������������
    SDK_8583_STFIELDATA stFiled[SDK_8583_FIELDMAX + 1]; //ÿ�����ڱ����е�λ������
}SDK_8583_ST8583;

extern u32 sdk8583IsDomainExist(SDK_8583_ST8583 const *stBag8583, u8 ucFieldNo);
extern s32 sdk8583SetField(SDK_8583_STFIELDDEF stField, SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo, const u8 *pFieldData, u16 usFieldDataLen);
extern s32 sdk8583GetField(SDK_8583_STFIELDDEF stField, SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo, u8 *pFieldData, u16 usMaxSize);
extern s32 sdk8583CpyField(SDK_8583_ST8583 *pstBag8583Dest, SDK_8583_ST8583 const *pstBag8583Src, u8 ucFieldNo);
extern s32 sdk8583ClrField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo);
extern s32 sdk8583Parse8583(SDK_8583_STFIELDDEF const *pstField, SDK_8583_ST8583 *pstBag8583, u16 usMsgHead);
extern s32 sdk8583GetVersion(u8 *pasVer);




#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

