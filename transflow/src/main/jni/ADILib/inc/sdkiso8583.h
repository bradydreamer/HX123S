#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#define SDK_8583_BAGMAXLEN 		2048     //存储报文长度
#define SDK_8583_FIELDMAX        128



#define SDK_8583_FIELD_MSG       0
#define SDK_8583_FIELD_MAP       1

typedef enum
{
    SDK_8583_LEN_ASC =  (0x01),   //ASCII
    SDK_8583_LEN_BCD =  (0x02),   //BCD 组包时需要压缩处理
    SDK_8583_LEN_HEXL = (0x03),  //hex 高位在前
    SDK_8583_LEN_HEXR = (0x04),  //hex 低位在前
}SDK_8583_LEN_TYPE;

typedef enum
{
    SDK_8583_DATA_ASC = (0x01),   //ASCII
    SDK_8583_DATA_BCD = (0x02),   //BCD 组包时需要压缩处理
    SDK_8583_DATA_BIT = (0x03),   //bit 组包时原样copy
}SDK_8583_DATA_TYPE;

typedef enum
{
    SDK_8583_ALIGN_L = (0x01),    //左对齐 对于DO_TYPE_HEX类数据表示高位在前
    SDK_8583_ALIGN_R = (0x02),    //右对齐 对于DO_TYPE_HEX类数据表示低位在前
}SDK_8583_DATA_ALIGN;

typedef struct
{
    u8 nFLenLen;                  //域长度指示位长度
    u8 ucFLenT;                   //域长度数据类型

    u16 nFDataLen;                //域内容最大长度
    u8 ucFDataT;                  //域内容数据类型
    u8 ucFDataA;                  //域内容对齐方式
    u8 ucFDataS;                  //域内容不够时补充字符
}SDK_8583_STFIELDDEF;


//某个域在报文中的位置属性
typedef struct
{
    u16 nFieldHead;               //域数据在ucBagData结构中的起始位置
    u16 nFieldLen;                //实际长度
}SDK_8583_STFIELDATA;

typedef struct
{
    u8 ucBagData[SDK_8583_BAGMAXLEN];      //报文数据
    u16 nBagLen;                  //报文长度
    u8 nEndFiled;                 //报文数据中最大的域编号
    SDK_8583_STFIELDATA stFiled[SDK_8583_FIELDMAX + 1]; //每个域在报文中的位置属性
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

