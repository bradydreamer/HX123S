#include "appglobal.h"

extern s32 Private_sdkIccConfigPagTLV(const u8 *pheTag, const u8 *pheInData,
s32 siInLen);
extern s32 Private_sdkIccImportRespCode(u8 ucOnlineResult, const u8 *pheRspCode);
extern s32 Private_sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);
extern s32 Private_sdkIccConfigPagTLV(const u8 *pheTag, const u8 *pheInData,
s32 siInLen);
extern s32 Private_sdkIccReadPagTLV(const u8 *pheTag, u8 *pheOutData,
s32 *psiOutLen);
TagList gstEmvTempTagList; //xiaox 用于临时存放emv的一些tag

static bool g_EncryptAllMsgflag = false;
static u8 aucEncryptKey[8];		// 加/解密密钥

/*
 SDK_8583_ST8583 gstSendBag;
 SDK_8583_ST8583 gstRecvBag;
 SDK_8583_ST8583 gstTempBag;
 */

SDK_8583_STFIELDDEF gstFieldTab[SDK_8583_FIELDMAX + 1] =
{
    /* FLD  00 */ {0, SDK_8583_LEN_BCD,  4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  01 */ {0, SDK_8583_LEN_BCD,  8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},     //此域不能更改
//lilin 20140710
    /* FLD  02 */ {2, SDK_8583_LEN_BCD, 19, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
//    /* FLD  02 */ {2, SDK_8583_LEN_BCD, 19, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD  03 */ {0, SDK_8583_LEN_BCD,  6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  04 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  05 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  06 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  07 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  08 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  09 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  10 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  11 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  12 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  13 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  14 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  15 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  16 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  17 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  18 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  19 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  20 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  21 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  22 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
#if 1 /*Modify by huangxin at 2015.03.17  14:16 */
    /* FLD  23 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
#else
    /* FLD  23 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
#endif /* if 0 */
    /* FLD  24 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD  25 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  26 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  27 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
#if 1 /*Modify by huangxin at 2015.03.05  18:29 */
    /* FLD  28 */ {0, SDK_8583_LEN_BCD, 9, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
#else
    /* FLD  28 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
#endif /* if 0 */
    /* FLD  29 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  30 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  31 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  32 */ {2, SDK_8583_LEN_BCD, 11, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  33 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  34 */ {0, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
//lilin 20140710
    /* FLD  35 */ {2, SDK_8583_LEN_BCD, 37, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  36 */ {3, SDK_8583_LEN_BCD, 104, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
//    /* FLD  35 */ {2, SDK_8583_LEN_BCD, 37, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
//    /* FLD  36 */ {3, SDK_8583_LEN_BCD, 104, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD  37 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD  38 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD  39 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD  40 */ {0, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  41 */ {0, SDK_8583_LEN_BCD, 8, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  42 */ {0, SDK_8583_LEN_BCD, 15, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  43 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  44 */ {2, SDK_8583_LEN_BCD, 25, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  45 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  46 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  47 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
#if 1 /*Modify by huangxin at 2015.03.11  21:45 */
    /* FLD  48 */ {3, SDK_8583_LEN_BCD, 512, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
#else
    /* FLD  48 */ {3, SDK_8583_LEN_BCD, 322, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
#endif /* if 0 */
    /* FLD  49 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  50 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  51 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  52 */ {0, SDK_8583_LEN_BCD, 8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
    /* FLD  53 */ {0, SDK_8583_LEN_BCD, 16, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  54 */ {3, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#ifdef ELECSIGN
    /* FLD  55 */ {3, SDK_8583_LEN_BCD, 1024, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
#else
	/* FLD	55 */ {3, SDK_8583_LEN_BCD, 255, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
#endif
    /* FLD  56 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  57 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  58 */ {3, SDK_8583_LEN_BCD, 100, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#if 1 /*Modify by huangxin at 2015.01.29  18:7 */
    /* FLD  59 */ {2, SDK_8583_LEN_BCD, 99, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, '0'},
    /* FLD  60 */ {3, SDK_8583_LEN_BCD, 13, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
#else
    /* FLD  59 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  60 */ {3, SDK_8583_LEN_BCD, 17, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},//zxx 20130711 20:12
#endif /* if 0 */
    /* FLD  61 */ {3, SDK_8583_LEN_BCD, 29, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},//zxx 20130711 20:12改为补0
#ifdef ELECSIGN
    /* FLD  62 */ {3, SDK_8583_LEN_BCD, 1160, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#else
	/* FLD  62 */ {3, SDK_8583_LEN_BCD, 1024, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#endif
#if 0 /*Modify by huangxin at 2015.04.09  17:21 */
    /* FLD  63 */ {3, SDK_8583_LEN_BCD, 163, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#else
    /* FLD  63 */ {3, SDK_8583_LEN_BCD, 512, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
#endif /* if 0 */
    /* FLD  64 */ {0, SDK_8583_LEN_BCD,  8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
    /* FLD  65 */ {0, SDK_8583_LEN_BCD,  8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},     //此域不能更改
    /* FLD  66 */ {2, SDK_8583_LEN_BCD, 19, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD  67 */ {0, SDK_8583_LEN_BCD,  6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  68 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  69 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  70 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  71 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  72 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  73 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  74 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  75 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  76 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  77 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  78 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  79 */ {0, SDK_8583_LEN_BCD, 4, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  80 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  81 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  82 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  83 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  84 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  85 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  86 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  87 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  88 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD  89 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  90 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  91 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  92 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  93 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  94 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  95 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  96 */ {2, SDK_8583_LEN_BCD, 11, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD  97 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD  98 */ {0, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD  99 */ {2, SDK_8583_LEN_BCD, 37, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD 100 */ {3, SDK_8583_LEN_BCD, 104, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD 101 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD 102 */ {0, SDK_8583_LEN_BCD, 6, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD 103 */ {0, SDK_8583_LEN_BCD, 2, SDK_8583_DATA_ASC, SDK_8583_ALIGN_R, '0'},
    /* FLD 104 */ {0, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 105 */ {0, SDK_8583_LEN_BCD, 8, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 106 */ {0, SDK_8583_LEN_BCD, 15, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 107 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 108 */ {2, SDK_8583_LEN_BCD, 25, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 109 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 110 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 111 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 112 */ {3, SDK_8583_LEN_BCD, 322, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD 113 */ {0, SDK_8583_LEN_BCD, 3, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 114 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 115 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 116 */ {0, SDK_8583_LEN_BCD, 8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
    /* FLD 117 */ {0, SDK_8583_LEN_BCD, 16, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, '0'},
    /* FLD 118 */ {3, SDK_8583_LEN_BCD, 40, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 119 */ {3, SDK_8583_LEN_BCD, 255, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0},
    /* FLD 120 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 121 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 122 */ {3, SDK_8583_LEN_BCD, 100, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 123 */ {0, SDK_8583_LEN_BCD, 12, SDK_8583_DATA_BCD, SDK_8583_ALIGN_R, '0'},
    /* FLD 124 */ {3, SDK_8583_LEN_BCD, 17, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD 125 */ {3, SDK_8583_LEN_BCD, 29, SDK_8583_DATA_BCD, SDK_8583_ALIGN_L, 'F'},
    /* FLD 126 */ {3, SDK_8583_LEN_BCD, 512, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 127 */ {3, SDK_8583_LEN_BCD, 163, SDK_8583_DATA_ASC, SDK_8583_ALIGN_L, ' '},
    /* FLD 128 */ {0, SDK_8583_LEN_BCD,  8, SDK_8583_DATA_BIT, SDK_8583_ALIGN_L, 0}
};

//锟斤拷息锟斤拷锟斤拷锟斤拷锟结构
struct _TradeFunCode {
	u8 ucTradeid;
	u8 ucTradeFunCode;
};

//交易消息类型码 根据文档
#define FUNCODE_MANAGE                      0x00                                //管理类交易，脚本通知交易#define FUNCODE_QUERY                       0x01                                //查询#define FUNCODE_POINTQUERY                  0x03                                //积分查询#define FUNCODE_PREAUTH                     0x10                                //预授权#define FUNCODE_UNPREAUTH           0x11                                        //预授权撤销#define FUNCODE_COMPLETE            0x20                                        //预授权完成#define FUNCODE_UNCOMPLETE          0x21                                        //预授权完成撤销#define FUNCODE_SALE                                0x22                                        //消费#define FUNCODE_UNSALE                      0x23                                        //消费撤销#define FUNCODE_OFFCOMPLETE         0x24                                        //预授权完成(离线)#define FUNCODE_REFUND                      0x25                                        //退货#define FUNCODE_ECOFFREFUND                      0x27                                        //电子现金退货#define FUNCODE_OFFSETTLE                   0x30                                        //离线结算#define FUNCODE_ADJUST                      0x32                                        //结算调整#define FUNCODE_TIPS                        0x34                                        //小费#define FUNCODE_OFFSALE              0x36                                       //IC卡脱机消费//#define FUNCODE_OFFUNSALE	            0x37					//IC卡脱机消费撤销(附加FUNCODE_UNSALE)//#define FUNCODE_ECOFFSALE               0x38                    //电子现金脱机消费,此类脱机消费不能被撤销
#define FUNCODE_EPCACC                     0x40                    //电子钱包指定账户圈存#define FUNCODE_EPCASH                     0x41                    //电子钱包现金圈存#define FUNCODE_EPCACCNO               0x42                    //电子钱包的非指定账户充值/冲正#define FUNCODE_ECCACC                      0x45                    //电子现金指定账户充值/冲正#define FUNCODE_ECCASH                      0x46                    //电子现金现金充值/冲正#define FUNCODE_ECCACCNO                  0x47                    //电子现金非指定账户充值/冲正#define FUNCODE_MAGLOADCASH            0x48                    //磁条卡现金充值/确认#define FUNCODE_MAGLOADACC            0x49                    //磁条卡账户充值/确认#define FUNCODE_UNECCASH                0x51                     //电子现金现金充值撤销/冲正#define FUNCODE_UNRESERVATION             0x53                     //预约消费撤销/冲正#define FUNCODE_RESERVATION                0x54                     //预约消费/冲正//消息类型码表 域60.1 //have_tradeid
static const struct _TradeFunCode const stTradeCode[] = {
		{ TRADEID_MANLOGIN, FUNCODE_MANAGE },                     //操作员签到
		{ TRADEID_POINTLOGIN, FUNCODE_MANAGE },                   //积分签到
		{ TRADEID_QUERY, FUNCODE_QUERY },                         //查询
		{ TRADEID_MAGLOAD_VERIFY, FUNCODE_QUERY },         //磁条卡现金充值帐户账户验证(查询一样)
		{ TRADEID_MOTOVERIFY, FUNCODE_QUERY },                    //持卡人身份验证
		{ TRADEID_POINTQUERY_UNION, FUNCODE_POINTQUERY },         //积分查询
		{ TRADEID_PREAUTH, FUNCODE_PREAUTH },                     //预授权
		{ TRADEID_MOTOPREAUTH, FUNCODE_PREAUTH },                 //订购预授权
		{ TRADEID_MOBILEPREAUTH, FUNCODE_PREAUTH },               //手机芯片预授权
		{ TRADEID_UNPREAUTH, FUNCODE_UNPREAUTH },                 //预授权撤销
		{ TRADEID_MOTOUNPREAUTH, FUNCODE_UNPREAUTH },             //订购预授权撤销
		{ TRADEID_MOBILEUNPREAUTH, FUNCODE_UNPREAUTH },             //手机芯片预授权撤销
		{ TRADEID_COMPLETE, FUNCODE_COMPLETE },                   //预授权完成
		{ TRADEID_MOTOCOMPLETE, FUNCODE_COMPLETE },               //订购预授权完成
		{ TRADEID_MOBILECOMPLETE, FUNCODE_COMPLETE },             //手机芯片预授权完成
		{ TRADEID_UNCOMPLETE, FUNCODE_UNCOMPLETE },              //预授权完成撤销
		{ TRADEID_MOTOUNCOMPLETE, FUNCODE_UNCOMPLETE },          //订购预授权完成撤销
		{ TRADEID_MOBILEUNCOMPLETE, FUNCODE_UNCOMPLETE },        //手机芯片预授权完成撤销

		{ TRADEID_SALE, FUNCODE_SALE },                           //消费
		{ TRADEID_MOTOSALE, FUNCODE_SALE },                       //订购消费
		{ TRADEID_INSTALLMENTSALE, FUNCODE_SALE },                //分期消费
		{ TRADEID_POINTSALE_ISSUER, FUNCODE_SALE },               //发卡行积分消费
		{ TRADEID_POINTSALE_UNION, FUNCODE_SALE },                //联盟积分消费
		{ TRADEID_MOBILESALE, FUNCODE_SALE },                     //手机消费
		{ TRADEID_RESERVATION_SALE, FUNCODE_RESERVATION },        //预约消费
		{ TRADEID_QPBOC, FUNCODE_OFFSALE },
		{ TRADEID_ECOFFSALE, FUNCODE_OFFSALE },
		//{TRADEID_OFFSALE, FUNCODE_SALE},//zxx 20130717 16:18 应该是36不是22
		{ TRADEID_UNSALE, FUNCODE_UNSALE },                     //消费撤销
		{ TRADEID_MOTOUNSALE, FUNCODE_UNSALE },                   //订购消费撤销
		{ TRADEID_INSTALLMENTUNSALE, FUNCODE_UNSALE },            //分期消费撤销
		{ TRADEID_POINTUNSALE_ISSUER, FUNCODE_UNSALE },           //发卡行积分消费撤销
		{ TRADEID_POINTUNSALE_UNION, FUNCODE_UNSALE },            //联盟积分消费撤销
		{ TRADEID_MOBILEUNSALE, FUNCODE_UNSALE },                 //手机消费撤销
		{ TRADEID_RESERVATION_UNSALE, FUNCODE_UNRESERVATION },    //预约消费撤销
		{ TRADEID_MOBILEQUERY, FUNCODE_QUERY },    //手机芯片查询
		{ TRADEID_MOBILEUNPREAUTH, FUNCODE_UNPREAUTH },    //手机芯片预授权撤销

		{ TRADEID_OFFCOMPLETE, FUNCODE_OFFCOMPLETE },           //预授权完成(离线)
		{ TRADEID_MOTOOFFCOMPLETE, FUNCODE_OFFCOMPLETE },       //订购预授权完成(离线)
		{ TRADEID_MOBILEOFFCOMPLETE, FUNCODE_OFFCOMPLETE },     //手机芯片预授权完成(离线)

		{ TRADEID_REFUND, FUNCODE_REFUND },                     //退货
		{ TRADEID_POINTREFUND_UNION, FUNCODE_REFUND },          //积分退货
		{ TRADEID_MOTOREFUND, FUNCODE_REFUND },                 //订购退货
		{ TRADEID_MOBILEREFUND, FUNCODE_REFUND },               //手机芯片退货
		{ TRADEID_ECOFFREFUND, FUNCODE_ECOFFREFUND },             //电子现金退货

		{ TRADEID_OFFSETTLE, FUNCODE_OFFSETTLE },                         //离线结算
		{ TRADEID_ADJUST, FUNCODE_ADJUST },                               //结算调整
		{ TRADEID_TIPS, FUNCODE_TIPS },                              //小费
		{ TRADEID_OFFSALE, FUNCODE_OFFSALE },                      //IC卡脱机消费
		{ TRADEID_EPCACC, FUNCODE_EPCACC },         //电子钱包指定账户圈存
		{ TRADEID_EPCASH, FUNCODE_EPCASH },         //电子钱包现金圈存
		{ TRADEID_EPCACCNO, FUNCODE_EPCACCNO },    //电子钱包的非指定账户充值/冲正
		{ TRADEID_ECCACC, FUNCODE_ECCACC },           //电子现金指定账户充值/冲正
		{ TRADEID_ECCASH, FUNCODE_ECCASH },         //电子现金现金充值/冲正
		{ TRADEID_ECCACCNO, FUNCODE_ECCACCNO },       //电子现金非指定账户充值/冲正
		{ TRADEID_MAGLOAD_CASH, FUNCODE_MAGLOADCASH },  //磁条卡现金充值/确认
		{ TRADEID_MAGLOAD_ACC, FUNCODE_MAGLOADACC },  //磁条卡账户充值/确认
		{ TRADEID_UNECCASH, FUNCODE_UNECCASH },    //电子现金现金充值撤销/冲正
		{ TRADEID_RESERVATION_UNSALE, FUNCODE_UNRESERVATION },    //预约消费撤销/冲正
		{ TRADEID_RESERVATION_SALE, FUNCODE_RESERVATION },       //预约消费/冲正
		};

//55域DOL结构
struct _Field55DoLTab {
	u8 heTag[3];
	u8 ucDatalen;
	bool bisM;
};

//55域消费DOL
static const struct _Field55DoLTab const stSaleTab[] = {
//余额查询,消费,预授权
		{ "\x9f\x26", 0x08, 1 },           //应用密文M
		{ "\x9f\x27", 0x01, 1 },           //应用数据信息M
		{ "\x9f\x10", 0x20, 1 },          //发卡行应用数据M
		{ "\x9f\x37", 0x04, 1 },           //不可预知数据M
		{ "\x9f\x36", 0x02, 1 },           //应用交易计数器M
		{ "\x95", 0x05, 1 },                       //终端验证结果M
		{ "\x9a", 0x03, 1 },                       //交易日期M
		{ "\x9c", 0x01, 1 },                       //交易类型M
		{ "\x9f\x02", 0x06, 1 },           //交易金额M
		{ "\x5f\x2a", 0x02, 1 },           //交易货币代码M
		{ "\x82", 0x02, 1 },              //应用交互特征M
		{ "\x9f\x1a", 0x02, 1 },           //终端国家代码M
		{ "\x9f\x03", 0x06, 1 },           //其他金额M
		{ "\x9f\x33", 0x03, 1 },           //终端性能M
		{ "\x9f\x34", 0x03, 0 },           //持卡人验证结果O
		{ "\x9f\x35", 0x01, 0 },           //终端类型O
		{ "\x9f\x1e", 0x08, 0 },           //接口设备序列号C
		{ "\x84", 0x10, 0 },                       //专用文件名称O
		{ "\x9f\x09", 0x02, 0 },           //应用版本号O
		{ "\x9f\x41", 0x04, 0 },           //交易序列计数器O
		{ "\x9f\x63", 0x10, 0 },           //卡产品标识信息O
		};

//55域脱机消费DOL
static const struct _Field55DoLTab const stOffSaleTab[] = {
//脱机消费
		{ "\x9f\x26", 0x08, 1 },           //应用密文M
		{ "\x9f\x27", 0x01, 1 },           //应用数据信息M
		{ "\x9f\x10", 0x20, 1 },           //发卡行应用数据M
		{ "\x9f\x37", 0x04, 1 },           //不可预知数据M
		{ "\x9f\x36", 0x02, 1 },           //应用交易计数器M
		{ "\x95", 0x05, 1 },                       //终端验证结果M
		{ "\x9a", 0x03, 1 },                       //交易日期M
		{ "\x9c", 0x01, 1 },                       //交易类型M
		{ "\x9f\x02", 0x06, 1 },           //交易金额M
		{ "\x5f\x2a", 0x02, 1 },           //交易货币代码M
		{ "\x82", 0x02, 1 },                       //应用交互特征M
		{ "\x9f\x1a", 0x02, 1 },           //终端国家代码M
		{ "\x9f\x03", 0x06, 1 },           //其他金额M
		{ "\x9f\x33", 0x03, 1 },           //终端性能M
		{ "\x9f\x1e", 0x08, 0 },           //接口设备序列号C
		{ "\x84", 0x10, 0 },                       //专用文件名称O
		{ "\x9f\x09", 0x02, 0 },           //应用版本号O
		{ "\x9f\x41", 0x04, 0 },           //交易序列计数器O
		{ "\x9f\x34", 0x03, 0 },           //持卡人验证结果O
		{ "\x9f\x35", 0x01, 0 },           //终端类型O
		{ "\x9f\x63", 0x10, 0 },           //卡产品标识信息O
		{ "\x9f\x74", 0x06, 0 },           //电子现金发卡行授权码O
		{ "\x8a", 0x02, 1 },                       //授权响应码M
		};

//55域脚本上送DOL
static const struct _Field55DoLTab const stScriptNoticeTab[] = { { "\x9f\x33",
		0x03, 1 },              //终端性能M
		{ "\x95", 0x05, 1 },                          //终端验证结果M
		{ "\x9f\x37", 0x04, 1 },              //不可预知数据M
		{ "\x9f\x1e", 0x08, 0 },              //接口设备序列号C
		{ "\x9f\x10", 0x20, 1 },              //发卡行应用数据M
		{ "\x9f\x26", 0x08, 1 },              //应用密文M
		{ "\x9f\x36", 0x02, 1 },              //应用交易计数器M
		{ "\x82", 0x02, 1 },                          //应用交互特征M
		{ "\xdf\x31", 0x15, 1 },              //脚本处理结果
		{ "\x9f\x1a", 0x02, 1 },              //终端国家代码M
		{ "\x9a", 0x03, 1 },                          //交易日期M
		};

//55域冲正DOL
static const struct _Field55DoLTab const stReversalTab[] =
//冲正
		{ { "\x95", 0x05, 1 },                          //终端验证结果M
				{ "\x9f\x1e", 0x08, 0 },              //接口设备序列号C
				{ "\x9f\x10", 0x20, 1 },              //发卡行应用数据M
				{ "\x9f\x36", 0x02, 1 },              //应用交易计数器M
				{ "\xdf\x31", 0x15, 0 },              //脚本处理结果
		};

//55域电子现金充值撤销DOL
static const struct _Field55DoLTab const stEcunloadCashTab[] = { { "\x9f\x1a",
		0x02, 1 },           //终端国家代码M
		{ "\x9f\x03", 0x06, 1 },           //其他金额M
		{ "\x9f\x33", 0x03, 1 },           //终端性能M
		{ "\x9f\x34", 0x03, 0 },           //持卡人验证结果O
		{ "\x9f\x35", 0x01, 0 },           //终端类型O
		{ "\x9f\x1e", 0x08, 0 },              //接口设备序列号C
		{ "\x84", 0x10, 0 },                       //专用文件名称O
		{ "\x9f\x09", 0x02, 0 },           //应用版本号O
		{ "\x9f\x41", 0x04, 0 },           //交易序列计数器O
		{ "\x9f\x63", 0x10, 0 },           //卡产品标识信息O
		{ "\x9f\x26", 0x08, 1 },           //应用密文M
		{ "\x9f\x27", 0x01, 1 },           //应用数据信息M
		{ "\x9f\x10", 0x20, 1 },           //发卡行应用数据M
		{ "\x9f\x37", 0x04, 1 },           //不可预知数据M
		{ "\x9f\x36", 0x02, 1 },           //应用交易计数器M
		{ "\x95", 0x05, 1 },                       //终端验证结果M
		{ "\x9a", 0x03, 1 },                       //交易日期M
		{ "\x9c", 0x01, 1 },                       //交易类型M
		{ "\x9f\x02", 0x06, 1 },           //交易金额M
		{ "\x5f\x2a", 0x02, 1 },           //交易货币代码M
		{ "\x82", 0x02, 1 },                       //应用交互特征M
		};

//55域手机芯片
static const struct _Field55DoLTab const stMobileTab[] = {
		{ "\xdf\x32", 0x10, 1 },           //芯片序列号
		{ "\xdf\x33", 0x08, 1 },           //过程密钥数据
		{ "\xdf\x34", 0x07, 1 },           //终端读取时间
		};
static const struct _Field55DoLTab const set55Tag[] = { { "\x9f\x10", 0x20, 1 }, //
		{ "\xdf\x31", 0x15, 1 },           //
		};
//extern s32 sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);

struct _RespCodeTab                 //应答码对照表
{
    u8 RespCode[3];                     //应答码
    u8 ShowInfo[50];                  //POS显示的内容
};

struct _RespTimeTab                 //应答码对照表
{
    s32 Resptime;                     //应答码
    u8 ShowInfo[50];                  //POS显示的内容
};
struct _BankName										//银行名称
{
    u8 BankCode[5];									//银行代码
    u8 BankName[20];								//银行名称
    u8 ShortName[10];                               //银行简称
};
static const struct _RespCodeTab RespTab[] =
{
    {{"01"}, {"请持卡人与发卡行联系"}},
    {{"03"}, {"无效商户"}},
    {{"04"}, {"此卡为无效卡（POS）"}},
    {{"05"}, {"持卡人认证失败"}},
    {{"11"}, {"成功,VIP客户"}},
    {{"12"}, {"无效交易"}},
    {{"13"}, {"无效金额"}},
    {{"14"}, {"无效卡号"}},
    {{"15"}, {"此卡无对应发卡方"}},
    {{"21"}, {"该卡未初始化或睡眠卡"}},
    {{"22"}, {"操作有误,或超出交易允许天数"}},
    {{"25"}, {"没有原始交易,请联系发卡方"}},
    {{"30"}, {"请重试"}},
    {{"34"}, {"作弊卡,吞卡"}},
    {{"38"}, {"密码错误次数超限,请与发卡方联系"}},
    {{"40"}, {"发卡方不支持的交易"}},
    {{"41"}, {"挂失卡(POS)"}},
    {{"43"}, {"被窃卡(POS)"}},
    //lilin 20140529
    {{"45"}, {"请使用芯片"}},
    {{"51"}, {"可用余额不足"}},
    {{"54"}, {"该卡已过期"}},
    {{"55"}, {"密码错"}},
    {{"57"}, {"不允许此卡交易"}},
    {{"58"}, {"发卡方不允许该卡在本终端进行此交易"}},
    {{"59"}, {"卡片校验错"}},
    {{"61"}, {"交易金额超限"}},
    {{"62"}, {"受限制的卡"}},
    {{"64"}, {"交易金额与原交易不匹配"}},
    {{"65"}, {"超出消费次数限制"}},
    {{"68"}, {"交易超时,请重试"}},
    {{"75"}, {"密码错误次数超限"}},
    {{"90"}, {"系统日切,请稍后重试"}},
    {{"91"}, {"发卡方状态不正常,请稍后重试"}},
    {{"92"}, {"发卡方线路异常,请稍后重试"}},
    {{"94"}, {"拒绝,重复交易,请稍后重试"}},
    {{"96"}, {"拒绝,交换中心异常,请稍后重试"}},
    {{"97"}, {"终端未登记"}},
    {{"98"}, {"发卡方超时"}},
    {{"99"}, {"PIN格式错,请重新签到"}},
    {{"A0"}, {"MAC校验错,请重新签到"}},
    /*=======BEGIN: lilin20140331 modify===========*/
    //最新文档更新响应码
    {{"A1"}, {"转账货币不一致"}},
    {{"A2"}, {"交易成功，请向资金转入行确认"}},
    {{"A3"}, {"资金到账行账号不正确"}},
    {{"A4"}, {"交易成功，请向资金到账行确认"}},
    {{"A5"}, {"交易成功，请向资金到账行确认"}},
    {{"A6"}, {"交易成功，请向资金到账行确认"}},
    {{"A7"}, {"安全处理失败"}},
    /*================ END================== */
    {{"77"}, {"批次号不一致,请重新签到"}},//lilin 20131224

    /*=======BEGIN: zhanbiqiang20160519 modify===========*/
    {{"P0"}, {"1下载密钥（已下载、不是999操作员）2其他金融交易（商户不支持此交易）"}},
    {{"P1"}, {"预授权完成离线，本行卡不支持"}},
    {{"P2"}, {"商户未开通"}},
    {{"P3"}, {"当批次不允许全额退货"}},
    {{"P4"}, {"IP地址不一致"}},
    {{"P5"}, {"错误的卡类型"}},
    {{"P6"}, {"未签到"}},
    {{"P7"}, {"交易日期超限"}},
    {{"P8"}, {"POS终端未开通"}},
    {{"P9"}, {"不支持信用卡交易"}},
    {{"E0"}, {"常汇户不存在"}},
    {{"E1"}, {"常汇户条数超限"}},
    {{"E2"}, {"未进行初始定位"}},
    {{"E3"}, {"终端超出可用地理范围"}},
    {{"E4"}, {"已初始定位  "}},
    {{"E5"}, {"收款账户涉案"}},
    {{"E6"}, {"收款账户可疑"}},
    {{"E7"}, {"付款账户涉案"}},
    {{"E8"}, {"付款账户可疑"}},
    {{"TI"}, {"服务器超时但未冲正"}},
    {{"WA"}, {"系统超时"}},
    {{"WB"}, {"参数错误"}},
    {{"WC"}, {"订单已支付"}},
    {{"WD"}, {"商户无权限"}},
    {{"WE"}, {"用户的条码已经过期"}},
    {{"WF"}, {"订单已关闭"}},
    {{"WG"}, {"外联网关错误"}},
    {{"WH"}, {"订单已撤销"}},
    {{"WI"}, {"用户支付中，需要输入密码"}},
    {{"WJ"}, {"授权码参数错误"}},
    {{"WK"}, {"授权码检验错误"}},
    {{"WL"}, {"XML格式错误 "}},
    {{"WM"}, {"关键域不匹配"}},
    {{"WN"}, {"未使用post传递参数"}},
    {{"WO"}, {"签名错误"}},
    {{"WP"}, {"缺少参数"}},
    {{"WQ"}, {"编码格式错误"}},
    {{"WR"}, {"支付帐号错误"}},
    {{"WS"}, {"APPID不存在 "}},
    {{"WT"}, {"MCHID不存在 "}},
    {{"WU"}, {"此交易订单号不存在"}},
    {{"WV"}, {"无效transaction_id"}},
    {{"WW"}, {"退款请求失败"}},
    {{"WX"}, {"appid和mch_id不匹配"}}
    /*================ END================== */
};
static const struct _RespTimeTab ErrTimeTab[] =
{
    {{0x01}, {"接收返回超时\n请重新交易"}},
    {{0x02}, {"通信中断\n请重新交易"}}
};
struct _BankName BankNameTab[] =
{
    {{"0100"}, {"邮储银行"}, {"邮储"}},
    {{"0102"}, {"工商银行"}, {"工行"}},
    {{"0103"}, {"农业银行"}, {"农行"}},
    {{"0104"}, {"中国银行"}, {"中行"}},
    {{"0105"}, {"建设银行"}, {"建行"}},
    {{"0301"}, {"交通银行"}, {"交行"}},
    {{"0302"}, {"中信银行"}, {"中信"}},
    {{"0303"}, {"光大银行"}, {"光大"}},
    {{"0304"}, {"华夏银行"}, {"华夏"}},
    {{"0305"}, {"民生银行"}, {"民生"}},
    {{"0306"}, {"广发银行"}, {"广发"}},
    {{"0307"}, {"平安银行"}, {"平安"}},
    {{"0308"}, {"招商银行"}, {"招行"}},
    {{"0309"}, {"兴业银行"}, {"兴业"}},
    {{"0310"}, {"浦发银行"}, {"浦发"}},
    {{"4802"}, {"银联商务"}, {"银商"}},
    {{"0401"}, {"上海银行"}, {"上海"}},
    {{"1438"}, {"湖南农信"}, {"湖南农信"}},
    {{"0570"}, {"华融湘江"}, {"华融湘江"}},
    {{"0461"}, {"长沙银行"}, {"长沙"}},
    {{"0425"}, {"东莞银行"}, {"东莞"}},
    {{"0489"}, {"南粤银行"}, {"南粤"}},
    {{"0403"}, {"北京银行"}, {"北京"}},
    {{"1418"}, {"北京农商"}, {"北京农商"}},
    {{"1401"}, {"上海农商"}, {"上海农商"}},
    {{"6403"}, {"北京银行"}, {"北京"}},	//新增加北京银行一个cl20131012
    {{"0410"}, {"平安银行"}, {"平安"}},//zxx 3.0 20130315 14:21增加平安北京上海
};


/*****************************************************************************
 ** Descriptions:	获得交易类型
 ** Parameters:          u8 tradeid
 ** Returned value:
 ** Created By:		lilin2012.08.15
 ** Remarks:
 *****************************************************************************/
u8 GetTradeFuncode(const u8 ucTradeId) {
	u8 rlt = 0;
	u16 i = 0;
	bool flag = false;
	u16 num = sizeof(stTradeCode) / sizeof(struct _TradeFunCode);

	for (i = 0; i < num; i++) {
		if (stTradeCode[i].ucTradeid == ucTradeId)        //匹配到相对的交易ID
				{
			rlt = stTradeCode[i].ucTradeFunCode;
			flag = true;
			break;
		}
	}

	if (!flag) {
//        Trace("GetTradeFuncode", "id = %d 未找到\r\n", ucTradeId);
		rlt = 0;
	}
	return rlt;
}
bool IsMobileTypeTrade(const u8 ucTradeId) {
	if (ucTradeId == TRADEID_MOBILEREFUND                              //手机芯片退货
	|| ucTradeId == TRADEID_MOBILEPREAUTH                //手机芯片预授权
	|| ucTradeId == TRADEID_MOBILEUNPREAUTH              //手机芯片预授权撤销
	|| ucTradeId == TRADEID_MOBILECOMPLETE            //手机芯片预授权完成请求
	|| ucTradeId == TRADEID_MOBILEOFFCOMPLETE           //手机芯片预授权完成通知
	|| ucTradeId == TRADEID_MOBILEUNCOMPLETE              //手机芯片预授权完成撤销
	|| ucTradeId == TRADEID_MOBILEQUERY                  //手机芯片手机芯片查询
	|| ucTradeId == TRADEID_MOBILESALE          //手机芯片消费
	|| ucTradeId == TRADEID_MOBILEUNSALE           //手机芯片消费撤销
			) {
		return true;
	}
	return false;
}

u32 ParseTlvPag(u8 *pTagStartPos, u16 size, u16 *pTag, u16 *pTagSize) {

	if (NULL == pTagStartPos) {
		return SDK_EQU;
	}
	if ((*pTagStartPos & 0x1F) == 0x1F) {
		if (size < 2) {
			return SDK_EQU;
		}
		if (*(pTagStartPos + 1) & 0x80) {
			return SDK_EQU;
		}
		*pTag = (u16) (((*pTagStartPos) << 8) + *(pTagStartPos + 1));

		*pTagSize = 2;
	} else {
		*pTag = *pTagStartPos;

		*pTagSize = 1;
	}
	return SDK_OK;

}

/*****************************************************************************
 ** Descriptions:	组55域
 ** Parameters:          u8 TradeType 组包类型
 u8 *DOLData 输出数据
 u8 *DataLen  输入长度
 ** Returned value:
 ** Created By:		lilin2012.08.14
 修改： wangb 0315
 ** Remarks:
 *****************************************************************************/

bool ZipPag55Data(SDK_8583_ST8583 *pstBag8583, u8 TradeType, u8 *DOLData,
u16 *DataLen, struct _TradeData *pstTradeData) {
	u32 dollen = 0;
	u8 dol[1024] = { 0 };
	u8 buf[1024] = { 0 };
	u32 buflen = 0;
	u8 tmp9f63[16] = { 0 };
	u8 i = 0, k = 0;
	s32 rslt;
	struct _Field55DoLTab stdoltab;
	u8 bctemp[8] = { 0 };
	u8 *p = NULL;
	u16 tag;

	//zxx 20130808 14:3 閿熸枻鎷蜂剑閿熺但ag 9f41閿熶茎闈╂嫹鍊?
	memset(buf, 0, sizeof(buf));

	dollen = 0;
	memset(buf, 0, sizeof(buf));
	if (GetFieldData(pstBag8583, 11, buf, sizeof(buf)) <= 0) {
		//  memcpy(buf, gstSys.asVoucherNo, 6);
		memcpy(buf, gstSys.asVoucherNo, 6);
		// sdkAscToBcdR(bctemp, buf, 4);
		// SetTagValue(0x9F41, bctemp, 4, &pTagList);
		ProviderSetEmvTagValue("\x9f\x41", bctemp, 4);
	}

	// sdkIccConfigTLV("\x9f\x41", bctemp, 4);
//    TradeType = SALE_MSG;
	switch (TradeType) {
	case QUERY_MSG:
	case SALE_MSG:             //消费

	case PREAUTH_MSG:
		i = sizeof(stSaleTab) / sizeof(struct _Field55DoLTab);
		break;

	case OFFLINE_SALE_MSG:              //脱机消费
		i = sizeof(stOffSaleTab) / sizeof(struct _Field55DoLTab);
		break;

	case SCRIPT_NOTICE_RLT:            //脚本上送通知
		i = sizeof(stScriptNoticeTab) / sizeof(struct _Field55DoLTab);
		break;

	case SPECIALTC_MSG:             //特殊tc
		i = sizeof(stSaleTab) / sizeof(struct _Field55DoLTab);
		break;

	case REVERSAL_MSG:             //冲正交易
		i = sizeof(stReversalTab) / sizeof(struct _Field55DoLTab);
		break;

	case ECUNLOAD_CASH_MSG:              //EC现金充值撤销报文
		i = sizeof(stEcunloadCashTab) / sizeof(struct _Field55DoLTab);
		break;

	case MOBILE_MSG:                   //手机芯片报文
		i = sizeof(stMobileTab) / sizeof(struct _Field55DoLTab);
		break;

	default:
		// Assert(0);
		break;
	}

	for (k = 0; k < i; k++) {
		switch (TradeType) {
		case QUERY_MSG:
		case SALE_MSG:             //消费
		case PREAUTH_MSG:
			memcpy(&stdoltab, &stSaleTab[k], sizeof(struct _Field55DoLTab));
			break;

		case OFFLINE_SALE_MSG:              //脱机消费
			memcpy(&stdoltab, &stOffSaleTab[k], sizeof(struct _Field55DoLTab));
			break;

		case SCRIPT_NOTICE_RLT:             //脚本上送通知
			memcpy(&stdoltab, &stScriptNoticeTab[k],
					sizeof(struct _Field55DoLTab));
			break;

		case SPECIALTC_MSG:                //特殊tc
			memcpy(&stdoltab, &stSaleTab[k], sizeof(struct _Field55DoLTab));
			break;

		case REVERSAL_MSG:                //冲正交易
			memcpy(&stdoltab, &stReversalTab[k], sizeof(struct _Field55DoLTab));
			break;

		case ECUNLOAD_CASH_MSG:              //EC现金充值撤销报文
			memcpy(&stdoltab, &stEcunloadCashTab[k],
					sizeof(struct _Field55DoLTab));
			break;

		case MOBILE_MSG:                     //手机芯片报文
			memcpy(&stdoltab, &stMobileTab[k], sizeof(struct _Field55DoLTab));
			break;

		default:
			//  Assert(0);
			break;
		}

		Log("wangb", "tag = %02x %02x len =%d ism=%d ", stdoltab.heTag[0],
				stdoltab.heTag[1], stdoltab.ucDatalen, stdoltab.bisM);
		memset(buf, 0, sizeof(buf));
		buflen = sizeof(buf);

		if (TradeType == SCRIPT_NOTICE_RLT || TradeType == REVERSAL_MSG
				|| TradeType == SPECIALTC_MSG) {
			if ((stdoltab.heTag[0] & 0x1F) == 0x1F) {
				sdkHexToU16((u8*) &tag, stdoltab.heTag, 2);
			} else {
				sdkHexToU16((u8*) &tag, stdoltab.heTag, 1);
			}
			buflen = GetTagValueSize(&gstEmvTempTagList, tag);
			if (buflen > 0) {
				p = GetTagValue(&gstEmvTempTagList, tag);
				if (p == NULL) {
					buflen = 0;
				} else {
					memcpy(buf, p, buflen);
					p = NULL;
				}
			}
            if (!memcmp(stdoltab.heTag, "\x9f\x36", 2) && TradeType == REVERSAL_MSG)
            {
                if (gstSys.bisOnlineDecline == false)    //Fixme 流程2的时候需要判断是否卡片拒绝
                {
                    buflen = 0;
                    memset(buf, 0, sizeof(buf));
                    stdoltab.bisM = 0;
                }
            }
		} else {
			buflen = ProviderGetEmvTagValue(stdoltab.heTag, 0, buf);
			int h;
			for (h = 0; h < buflen; h++) {
				Log("wangb", "ProviderGetEmvTagValue is %x ", buf[h]);
			}
		}
		if (buflen >= 0)
			rslt = SDK_OK;

		// Trace("ZipPag55Data", "rslt=[%d] TradeType=[%d]\r\n", rslt, TradeType);
//        Trace("ZipPag55Data", "stdoltab.heTag=[%02X%02X]\r\n", stdoltab.heTag[0], stdoltab.heTag[1]);
//        TraceHex("ZipPag55Data", "buf", buf, buflen);

		if (((rslt == SDK_OK) && (buflen > 0)) || stdoltab.bisM)   //取相应的emvtag值
				{
			if (memcmp(stdoltab.heTag, "\x9f\x63", 2)
					|| (memcmp(stdoltab.heTag, "\x9f\x63", 2) == 0
							&& memcmp(buf, tmp9f63, buflen)))
					//判断标签长度与银联文档文档不符  和emv 4.2
					//EMV v4.2 Book 3 Application Specification CR05_20090124020803 162页
					{
				if ((stdoltab.heTag[0] & 0x1F) == 0x1F) {
					memcpy(&dol[dollen], stdoltab.heTag, 2);
					dollen += 2;
				} else {
					dol[dollen++] = stdoltab.heTag[0];
				}
				if ((rslt == SDK_OK) && (buflen > 0))                  //有标签
						{
					dol[dollen++] = buflen;
					memcpy(&dol[dollen], buf, buflen);
					dollen += buflen;
				} else {
					if (stdoltab.bisM) {
						dol[dollen++] = 0;
					}
				}
			}
		}

	}
	*DataLen = dollen;
	memcpy(DOLData, dol, dollen);
	return true;
}

/*****************************************************************************
 ** Descriptions:	解55域
 ** Parameters:          SDK_8583_ST8583 *pstBag8583
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:
 *****************************************************************************/

bool UnZipPag55Data(const u8 ucTradeId, SDK_8583_ST8583 *pstBag8583) {
	u16 len = 0;
	u8 revtmp[512] = { 0 };
	s32 rslt;

	if (!IsMobileTypeTrade(ucTradeId)) {
		if (sdk8583IsDomainExist(pstBag8583, 55))           //如果存在解55域
				{
			memset(revtmp, 0, sizeof(revtmp));

			len = GetFieldData(pstBag8583, 55, revtmp, sizeof(revtmp));
			if (len > 0) {
//                Trace("UnZipPag55Data", "len=[%d]\r\n", len);
//                TraceHex("UnZipPag55Data", "revtmp", revtmp, len);
				rslt = Private_sdkIccUnzipPagData(revtmp, len);
//                Trace("UnZipPag55Data", "rslt=[%d]\r\n", rslt);
				if (rslt == SDK_OK) {
					return true;
				} else {
					return false;
				}
				return true;
			} else {
				return false;
			}
		}

	}
	return true;
}

/*****************************************************************************
 函 数 名  : CopyRecvField
 功能描述  : 从原交易接收包组织域数据
 输入参数  : u8 ucFieldNo
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
/*
 bool CopyRecvField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo)
 {
 if (sdk8583IsDomainExist(&gstRecvBag, ucFieldNo))
 {

 sdk8583CpyField(pstBag8583, &gstRecvBag, ucFieldNo);
 return true;
 }
 return false;
 }
 */
/*****************************************************************************
 函 数 名  : CopySendField
 功能描述  : 从原交易发送包组织域数据
 输入参数  : u8 ucFieldNo
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
/*
 bool CopySendField(SDK_8583_ST8583 *pstBag8583, u8 ucFieldNo)
 {
 if (sdk8583IsDomainExist(&gstTempBag, ucFieldNo))
 {
 sdk8583CpyField(pstBag8583, &gstTempBag, ucFieldNo);
 return true;
 }
 return false;
 }
 */
//extern s32 sdkIccUnzipPagData(const u8 *DataIn, s32 siDataLen);
/*****************************************************************************
 ** Descriptions:	取国际信用卡代码
 ** Parameters:          u8 *pasInterCode  国际信用卡代码
 u8 *pherid       所以AID
 ** Returned value:	 true 正确false错误
 ** Created By:		lilin2012.10.10
 ** Remarks:
 *****************************************************************************/
bool GetInterCode(u8 *pasInterCode, u8 *pherid) {
	u8 i;
	u8 RidTab[4][5] = { { 0xA0, 0x00, 0x00, 0x03, 0x33 }, { 0xA0, 0x00, 0x00,
			0x00, 0x03 }, { 0xA0, 0x00, 0x00, 0x00, 0x04 }, { 0xA0, 0x00, 0x00,
			0x00, 0x65 } };

	u8 const InterCodeTab[4][4] =
			{ { "CUP" }, { "VIS" }, { "MCC" }, { "JCB" }, };

	for (i = 0; i < 4; i++) {
		if (!memcmp(RidTab[i], pherid, 5)) {
			memcpy(pasInterCode, InterCodeTab[i], 3);
			return true;
		}
	}

	return false;
}

/*****************************************************************************
 ** Descriptions:	组61域原始交易内容
 包括批次号、流水号、交易日期
 ** Parameters:    	u8 const ucTradeId
 ** Returned value:
 ** Created By:		lilin2014.03.24
 ** Remarks:
 *****************************************************************************/

static void FormField61Origdata(u8 const ucTradeId, u8 *pasOrigdata) {
	u8 temp[512] = { 0 };
	u8 origdata[128] = { 0 };
#if 0 /*Modify by huacong at 2016.03.04  10:44 */
	//61域
	//从原交易接收包中取出批次号
	memset(temp, 0, sizeof(temp));
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(&gstRecvBag, 60, origdata, sizeof(origdata)) > 0)
	{
		strncat(temp, &origdata[2], 6);
	}
	else
	{
		if (GetFieldData(&gstTempBag, 60, origdata, sizeof(origdata)) > 0)
		{
			strncat(temp, &origdata[2], 6);
		}
		else
		{
			strncat(temp, "000000", 6);
		}
	}

	//从原交易接收包中取出流水号
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(&gstRecvBag, 11, origdata, sizeof(origdata)) > 0)
	{
		strncat(temp, origdata, 6);
	}
	else
	{
		if (GetFieldData(&gstTempBag, 11, origdata, sizeof(origdata)) > 0)
		{
			strncat(temp, origdata, 6);
		}
		else
		{
			strncat(temp, "000000", 6);
		}
	}
//    Trace("zxx", "pstTradeData->ucTradeId = %d\r\n", ucTradeId);
	if ((ucTradeId == TRADEID_UNPREAUTH) || (ucTradeId == TRADEID_COMPLETE) || (ucTradeId == TRADEID_UNCOMPLETE)
			|| (ucTradeId == TRADEID_MOTOUNPREAUTH) || (ucTradeId == TRADEID_MOTOCOMPLETE) || (ucTradeId == TRADEID_MOTOUNCOMPLETE)
			|| (ucTradeId == TRADEID_MOBILEUNPREAUTH) || (ucTradeId == TRADEID_MOBILECOMPLETE) || (ucTradeId == TRADEID_MOBILEUNCOMPLETE)
			|| ucTradeId == TRADEID_TIPS || ucTradeId == TRADEID_ADJUST || ucTradeId == TRADEID_OFFSETTLE)//zxx 20130723 14:23锟斤拷锟斤拷小锟窖和碉拷锟斤拷锟斤拷锟斤拷锟竭斤拷锟斤拷
	{
		//从原交易接收包中取出交易日期
		memset(origdata, 0, sizeof(origdata));
		if (GetFieldData(&gstRecvBag, 13, origdata, sizeof(origdata)) > 0)
		{
//            Trace("lilin","取锟斤拷锟斤拷锟秸帮拷\r\n");
			strncat(temp, origdata, 4);
		}
		else
		{
			if (GetFieldData(&gstTempBag, 13, origdata, sizeof(origdata)) > 0)
			{
				strncat(temp, origdata, 4);
			}
			else
			{
				strncat(temp, "0000", 4);
			}
		}
	}
	strcpy(pasOrigdata,temp);
#endif /* if 0 */
}

/*****************************************************************************
 函 数 名  : SetPublicField002
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 //have_tradeid
 *****************************************************************************/
bool SetPublicField002(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	if (pstTradeData->strCardInfo.stCardParam.ucCardMode != SDK_ICC_MAG) {
		SetFieldData(pstBag8583, 2,
				pstTradeData->strCardInfo.stCardData.asCardNO,
				pstTradeData->strCardInfo.stCardData.ucCardLen);
	}
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField003
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField003(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u8 temp[512] = { 0 };

	strcat(temp, "000000");
	SetFieldData(pstBag8583, 3, temp, strlen(temp));

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField004
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField004(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {

	SetFieldData(pstBag8583, 4, pstTradeData->asAmount,
			strlen(pstTradeData->asAmount));

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField011
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/

bool SetPublicField011(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 11, gstSys.asVoucherNo, 6);         //流水号 Fixme  存储
	ProviderSetTagValue(TAG_8583_VoucherNo, gstSys.asVoucherNo, 6);    //即时存储流水号
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField014
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField014(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SDK_ICC_CARDDATA *pstCardData = NULL; //卡数据

	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	pstCardData = &pstTradeData->strCardInfo.stCardData;

	if (pstCardData != NULL) {
		if (strlen(pstCardData->asExpiredDate) >= 4
				&& memcmp(pstCardData->asExpiredDate, "0000", 4) != 0) //有效期有14域
						{
			SetFieldData(pstBag8583, 14, pstCardData->asExpiredDate, 4);
		}
	}

	return true;
}

/*****************************************************************************
** Descriptions:	是否是预约交易
** Parameters:    	const u8 ucTradeId 交易ID
** Returned value:
** Created By:		lilin2013.01.31
** Remarks:
*****************************************************************************/
bool IsReservationTypeTrade(const u8 ucTradeId)
{
    if (ucTradeId == TRADEID_RESERVATION_SALE
            || ucTradeId == TRADEID_RESERVATION_UNSALE
            || ucTradeId == TRADEID_RESERVATION_REFUND)     //预约支付
    {
        return true;
    }
    return false;
}
/*****************************************************************************
 函 数 名  : SetPublicField022
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2012年12月17日
    作    者   : zhangjun
    修改内容   : 新生成函数

//have_tradeid
*****************************************************************************/
bool SetPublicField022(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData)
{
    u8 temp[512] = {0};
    u8 ucTradeId; //消息ID
    u8 ucCardMode; //卡模式
    u8 ucCLType; //实?蔍C卡流程
    bool bIsPwd; //是否输入密码

    if (pstTradeData == NULL)
    {
        return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
    }

    ucTradeId = pstTradeData->ucTradeId;
    ucCardMode = pstTradeData->strCardInfo.stCardParam.ucCardMode;
    ucCLType = pstTradeData->strCardInfo.stCardParam.ucCLType;
    bIsPwd = pstTradeData->heCreditPwd[0];

    if (IsReservationTypeTrade(ucTradeId))
    {
        strcpy(temp, "92");
    }
    else if (IsMobileTypeTrade(ucTradeId))
    {
        strcpy(temp, "96");
    }
    else
    {
        if (ucCardMode == SDK_ICC_MANUAL)      //手输
        {
            strcpy(temp, "01");
        }
        else if (ucCardMode == SDK_ICC_ICC || ucCardMode == SDK_ICC_RF)        //IC卡
        {
            if (ucCLType == ICCPBOCFLOW)               //PBOC流程
            {
                strcpy(temp, "98");
            }
            else if (ucCLType == ICCQPBOCFLOW)       //QPBOC流程
            {
                strcpy(temp, "07");
            }
            else if (ucCLType == ICCMSDFLOW)           //MSD流程
            {
                strcpy(temp, "91");
            }
            else
            {
                strcpy(temp, "05");
            }
        }
        else if (ucCardMode == SDK_ICC_MAG)             //磁卡
        {
            strcpy(temp, "02");
        }
        else
        {
            strcpy(temp, "00");
        }
    }

    if (bIsPwd)           //是否输入密码
    {
        strcat(temp, "1");
    }
    else
    {
        strcat(temp, "2");
    }
    SetFieldData(pstBag8583, 22, temp, strlen(temp));                //输入方式

    return true;

}

/*****************************************************************************
 函 数 名  : SetPublicField023
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField023(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	s32 j = 0, len;
	u8 temp[512] = { 0 };
	u8 buf[512] = { 0 };
	u8 ucCardMode; //卡模式

	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}
	ucCardMode = pstTradeData->strCardInfo.stCardParam.ucCardMode;
	if (pstTradeData->ucTradeId == TRADEID_ECCACCNO) {
		ucCardMode = pstTradeData->strInCardInfo.stCardParam.ucCardMode; //zxx 20130711 14:24这里应该是转入卡
	}
//    Trace("zxx","23域 ucCardMode = %d\r\n",ucCardMode);

	if (ucCardMode != SDK_ICC_MANUAL && ucCardMode != SDK_ICC_MAG
			&& ucCardMode != 0) {
//		memset(temp, 0, sizeof(temp));
//		j = sizeof(temp);
//        if (sdkIccReadTLV("\x5F\x34", temp, &j) > 0)
//		if ((len = ProviderGetTagValue(TAG_SYS_CardID, temp)) > 0) {
////            sdkBcdToAsc(buf, temp, j);
//			SetFieldData(pstBag8583, 23,
//					pstTradeData->strCardInfo.stCardData.ucUID, len);    //卡片序列号
//		} else {
//			//lilin 20140218 没有返回上送0
//			SetFieldData(pstBag8583, 23, "000", 3);             //卡片序列号
//		}
		if ((len = ProviderGetTagValue(TAG_SYS_CardID, temp)) > 0) {
			memset(temp, 0, sizeof(temp));
			temp[0] = '0';
			memcpy(temp + 1, pstTradeData->strCardInfo.stCardData.ucUID, len * 2);
			temp[3] = '0';
			SetFieldData(pstBag8583, 23,temp, 3);    //卡片序列号
		} else {
			//lilin 20140218 没有返回上送0
			SetFieldData(pstBag8583, 23, "000", 3);             //卡片序列号
		}
	}
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField025
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField025(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 25, "00", 2);                //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField026
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField026(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	if (pstTradeData->heCreditPwd != NULL)           //如果有密码
			{
		if (pstTradeData->heCreditPwd[0] > 0) {
			SetFieldData(pstBag8583, 26, "06", 2);

			if (pstTradeData->strCardInfo.stCardParam.ucCardMode != SDK_ICC_MAG
					&& pstTradeData->strCardInfo.stCardParam.ucCardMode
							!= SDK_ICC_MANUAL
					&& pstTradeData->strCardInfo.stCardParam.ucCardMode != 0) {

				SetFieldData(pstBag8583, 26, "12", 2);
			}

		}
	}

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField035
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField035(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	STCARDINFO *pstCardInfo = NULL; //卡数据
	u8 value[256];
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	pstCardInfo = &pstTradeData->strCardInfo;
	if (pstCardInfo != NULL) {
		if (!(pstCardInfo->stCardParam.ucCardMode == SDK_ICC_MANUAL))    //非手输卡号
		{
			if (pstCardInfo->stCardData.ucTrack2Len) {
				SetFieldData(pstBag8583, 35,
						pstCardInfo->stCardData.asTrack2Data,
						pstCardInfo->stCardData.ucTrack2Len);

			}
		}
	}

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField036
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField036(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	STCARDINFO *pstCardInfo = NULL; //卡数据

	pstCardInfo = &pstTradeData->strCardInfo;

	if (pstCardInfo != NULL) {
		if (!(pstCardInfo->stCardParam.ucCardMode == SDK_ICC_MANUAL))    //非手输卡号
		{
			if (pstCardInfo->stCardData.ucTrack3Len) {
				SetFieldData(pstBag8583, 36,
						pstCardInfo->stCardData.asTrack3Data,
						pstCardInfo->stCardData.ucTrack3Len);
			}
		}
	}
	return true;

}

/*****************************************************************************
 函 数 名  : SetPublicField037
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField037(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	if (strlen(pstTradeData->asSysRefNo)) {
		SetFieldData(pstBag8583, 37, pstTradeData->asSysRefNo, 12);       //原参考号
	}

	return true;
}
/*****************************************************************************
 函 数 名  : SetPublicField038
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField038(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	if (strlen(pstTradeData->asAuthCode)) {
		SetFieldData(pstBag8583, 38, pstTradeData->asAuthCode, 6);         //授权码
	}

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField041
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField041(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 41, gstAppSetting.stTerminalParam.asTerminalNo, 8); //终端号

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField042
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField042(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 42, gstAppSetting.stTerminalParam.asMerchantNo,
			15);     //商户号

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField048
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField048(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 48, "00", 2);  //默锟斤拷值

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField049
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField049(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	SetFieldData(pstBag8583, 49, gstAppSetting.stTerminalParam.asCurrencyCode,
			3);  //交易货币代码

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField052
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField052(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	if (pstTradeData->heCreditPwd != NULL)           //如果有密码
			{
		if (pstTradeData->heCreditPwd[0] > 0) {
			SetFieldData(pstBag8583, 52, (u8 *) &pstTradeData->heCreditPwd[1],
					8);
		}
	}

	return true;

}

/*****************************************************************************
 函 数 名  : SetPublicField053
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField053(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u8 temp[512] = { 0 };
	STCARDINFO *pstCardInfo = NULL; //卡数据

	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	pstCardInfo = &pstTradeData->strCardInfo;
	//zxx 20130712 15:34 35、36、52同时不存在不到有53域
	if (!sdk8583IsDomainExist(pstBag8583, 35)
			&& !sdk8583IsDomainExist(pstBag8583, 36)
			&& !sdk8583IsDomainExist(pstBag8583, 52)) {
		sdk8583ClrField(pstBag8583, 53);
		return true;
	}
	if (pstTradeData->heCreditPwd[0] > 0 || sdk8583IsDomainExist(pstBag8583, 35)
			|| sdk8583IsDomainExist(pstBag8583, 36)) {
		if (pstTradeData->heCreditPwd[0] > 0) {
			if (pstCardInfo->stCardParam.ucCardMode
					&& pstCardInfo->stCardParam.ucCLType != MOBILEMSDFLOW) {
				strcpy(temp, "200");   //带主账号加密
			} else {
				strcpy(temp, "100");   //不带主账号加密
			}
		} else {
			strcpy(temp, "000");   //zxx 20130711 15:56 35或36存在53就存在
		}
		if (gstAppSetting.stKeySystemParam.bIsMk3Des)        // 3des
		{
			temp[1] = '6';
		}
		temp[2] = '0'; //磁道加密标志在磁道加密函数里设置。
		SetFieldData(pstBag8583, 53, temp, strlen(temp));
	}
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField055
 功能描述  : 组织域数据
 输入参数  : struct _TradeData const *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 //have_tradeid
 *****************************************************************************/
bool SetPublicField055(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u16 i = 0;
	u8 temp[1024] = { 0 };
	u8 ucTradeId; //消息ID
	u8 ucCardMode; //卡模式
	u8 ucField55Type = SALE_MSG; //55域IC卡数据类型

	ucTradeId = pstTradeData->ucTradeId;
	if (ucTradeId == TRADEID_ECCACCNO) //非指定账户圈存是转入卡
			{
		ucCardMode = pstTradeData->strInCardInfo.stCardParam.ucCardMode;
	} else {
		ucCardMode = pstTradeData->strCardInfo.stCardParam.ucCardMode;
	}
//    ucTradeId = TRADEID_SALE;
	switch (ucTradeId) {
	case TRADEID_SALE:
	case TRADEID_INSTALLMENTSALE:
	case TRADEID_POINTSALE_ISSUER:
	case TRADEID_POINTSALE_UNION:
	case TRADEID_RESERVATION_SALE:
	case TRADEID_ECCASH:
	case TRADEID_ECCACC:
	case TRADEID_ECCACCNO:
		ucField55Type = SALE_MSG;
		break;

	case TRADEID_QUERY:
		ucField55Type = QUERY_MSG;
		break;

	case TRADEID_PREAUTH:
		ucField55Type = PREAUTH_MSG;
		break;

	case TRADEID_QPBOC:
	case TRADEID_ECOFFSALE:
	case TRADEID_OFFSALE:
		ucField55Type = OFFLINE_SALE_MSG;
		break;

	case TRADEID_UNECCASH:
		ucField55Type = ECUNLOAD_CASH_MSG;
		break;

	default:
		ucField55Type = SALE_MSG;
		break;
	}

	// Trace("SetPublicField055", "ucField55Type=[%d]\r\n", ucField55Type);

	if (IsMobileTypeTrade(ucTradeId)) {
		ucField55Type = MOBILE_MSG;
	}

	if (ucCardMode != SDK_ICC_MANUAL && ucCardMode != SDK_ICC_MAG
			&& ucCardMode != 0) {
		memset(temp, 0, sizeof(temp));
		Log("wangb", "5555nRet is %c ", ucCardMode);
		if (!ZipPag55Data(pstBag8583, ucField55Type, temp, &i, pstTradeData)) {
			//  sdkCommDestoryLink();
			//  sdkDispPromptBox(false, TV_AUTORET, "异常错误\n请重新交易");
			return false;
		}

		// Trace("SetPublicField055", "i=[%d]\r\n", i);
		//  TraceHex("SetPublicField055", "temp", temp, i);
		SetFieldData(pstBag8583, 55, temp, i);               //55域数据
	}

	return true;
}

bool SetPublicField060(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u8 i = 0;
	u8 temp[512] = { 0 };
	u8 buf[128] = { 0 };
	u8 ucTradeId; //消息ID
	u8 ucCLType; //实际IC卡流程
	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	ucTradeId = pstTradeData->ucTradeId;
	ucCLType = pstTradeData->strCardInfo.stCardParam.ucCLType;

	i = GetTradeFuncode(ucTradeId); //60.1交易类型码
	sprintf(buf, "%02x", i);

	strcat(buf, gstSys.asBatchNo);   //60.2批次号

	strcat(buf, "000");              //60.3网络管理信息码

	//60.4终端读取能力
	if (sdk8583IsDomainExist(pstBag8583, 22)) {
		if (GetFieldData(pstBag8583, 22, temp, sizeof(temp)) < 0)     //获取发送包22域
		{
			memset(temp, 0, sizeof(temp));
		}
	} else {
		memset(temp, 0, sizeof(temp));
	}

	if (1){//gstAppSetting.stTradeCardType.bIsSupportIcc){
#if 0 /*Modify by huangxin at 2015.02.06  16:43 */
		if (1)
		{
			//最新BCTC要求上送最高终端能力
			strcat(buf, "6");
		}
		else
		{
#endif /* if 0 */
		if (!memcmp(temp, "07", 2) || !memcmp(temp, "91", 2)               //非接触qPBOC   MSD
				|| !memcmp(temp, "96", 2) || !memcmp(temp, "98", 2))       //cupmobile qpboc  非接触PBOC
		{
			strcat(buf, "6");
		}
		else if (!memcmp(temp, "05", 2)                                    //接触式卡
				 || !memcmp(temp, "01", 2)                                 //手输
				 || !memcmp(temp, "02", 2))                                //磁条
		{
			strcat(buf, "5");
		}
		else                                                               //终端读取能力不可知
		{
			strcat(buf, "0");
		}
#if 0 /*Modify by huangxin at 2015.02.06  16:43 */
		}
#endif /* if 0 */
	}else{
		strcat(buf, "0");
	}

//	if (1) {
//		if (1) {
//			//最新BCTC要求上送最高终端能力
//			strcat(buf, "6");
//		} else {
//			if (!memcmp(temp, "07", 2) || !memcmp(temp, "91", 2) //非接触qPBOC   MSD
//					|| !memcmp(temp, "96", 2) || !memcmp(temp, "98", 2)) //cupmobile qpboc  非接触PBOC
//							{
//				strcat(buf, "6");
//			} else if (!memcmp(temp, "05", 2) //接触式卡
//			|| !memcmp(temp, "01", 2) //手输
//					|| !memcmp(temp, "02", 2)) //磁条
//							{
//				strcat(buf, "5");
//			} else                        //终端读取能力不可知
//			{
//				strcat(buf, "0");
//			}
//		}
//
//	} else {
//		strcat(buf, "0");
//	}

	//60.5基于PBOC借/贷记标准的IC卡条件代码
	//获取是否FALLBACK
	if (ucCLType == ICCFALLBACK
			&& (pstTradeData->strCardInfo.stCardData.bIsIccMagCard
					|| pstTradeData->strInCardInfo.stCardData.bIsIccMagCard)) //fallbcak
			{
		//60.4终端读取能力
		//60.5基于PBOC借/贷记标准的IC卡条件代码
		//strcat(buf, "52");
		//zxx 20130729 10:50
		strcat(buf, "2");
	} else {
		strcat(buf, "0");
	}
	//60.6支持部分扣款和返回余额标志
	//积分消费
//    if (ucTradeId == TRADEID_POINTSALE_UNION     //联盟积分消费
//            || ucTradeId == TRADEID_POINTSALE_ISSUER     //发卡行积分消费
//            || ucTradeId == TRADEID_SALE           //普通消费
//            || ucTradeId == TRADEID_RESERVATION_SALE     //预约消费
//            || ucTradeId == TRADEID_INSTALLMENTSALE
//            || ucTradeId == TRADEID_MOTOSALE
//            || ucTradeId == TRADEID_POINTQUERY_UNION
//            || ucTradeId == TRADEID_POINTUNSALE_ISSUER
//            || ucTradeId == TRADEID_POINTUNSALE_UNION
//            || ucTradeId == TRADEID_POINTREFUND_UNION
//       )
//zxx 20130729 15:42 无法确定哪一些交易要上送，只能是换规范中的报文来上送，有要求的上送1，无要求的不上送
	if (!(ucTradeId == TRADEID_QUERY                             //查询
	|| ucTradeId == TRADEID_PREAUTH                            //预授权
	|| ucTradeId == TRADEID_UNPREAUTH                          //预授权撤销
	|| ucTradeId == TRADEID_ADDPREAUTH                //追加预授权(附加TRADEID_PREAUTH)
	|| ucTradeId == TRADEID_COMPLETE                               //预授权完成
	|| ucTradeId == TRADEID_UNCOMPLETE                           //预授权完成撤销
	|| ucTradeId == TRADEID_OFFCOMPLETE                            //预授权完成通知
	|| ucTradeId == TRADEID_OFFSETTLE                                  //离线结算
	|| ucTradeId == TRADEID_ADJUST                                        //调整
	|| ucTradeId == TRADEID_TIPS                                           //小费
	|| ucTradeId == TRADEID_OFFSALE                                   //普通脱机消费
	|| ucTradeId == TRADEID_OFFUNSALE || ucTradeId == TRADEID_ECOFFSALE //电子现金脱机消费
	|| ucTradeId == TRADEID_ECCACC                                  //电子现金指定账户充值
	|| ucTradeId == TRADEID_ECCASH                                    //电子现金现金充值
	|| ucTradeId == TRADEID_ECCACCNO                               //电子现金非指定账户充值
	|| ucTradeId == TRADEID_QPBOC                                        //qPBOC
	|| ucTradeId == TRADEID_ECOFFREFUND               		//电子现金退货
	|| ucTradeId == TRADEID_UNECCASH           		//电子现金现金充值撤销
	|| ucTradeId == TRADEID_MOTOPREAUTH                                 //订购预授权
	|| ucTradeId == TRADEID_MOTOUNPREAUTH                          //预授权撤销
	|| ucTradeId == TRADEID_MOTOCOMPLETE                               //预授权完成请求
	|| ucTradeId == TRADEID_MOTOOFFCOMPLETE                            //预授权完成通知
	|| ucTradeId == TRADEID_MOTOUNCOMPLETE                          //预授权完成撤销
	|| ucTradeId == TRADEID_MOBILEPREAUTH                              //手机芯片预授权
	|| ucTradeId == TRADEID_MOBILEUNPREAUTH                        //手机芯片预授权撤销
	|| ucTradeId == TRADEID_MOBILECOMPLETE                        //手机芯片预授权完成请求
	|| ucTradeId == TRADEID_MOBILEOFFCOMPLETE                  //手机芯片预授权完成通知
	|| ucTradeId == TRADEID_MOBILEUNCOMPLETE                       //手机芯片预授权完成撤销
	|| ucTradeId == TRADEID_MOBILEQUERY                             //手机芯片手机芯片查询
	|| ucTradeId == TRADEID_SETTLE)                              //结算
	) {
		strcat(buf, "1");             //支持部分扣款和返回余额标志
	}

	//60.7账户类型
	if (ucTradeId == TRADEID_POINTSALE_UNION
			|| ucTradeId == TRADEID_POINTUNSALE_UNION     //联盟积分消费
			|| ucTradeId == TRADEID_POINTQUERY_UNION
			|| ucTradeId == TRADEID_POINTREFUND_UNION) //zxx 20130804 17:49 添加联盟积分退货
					{
		strcat(buf, "065");
	} else if (ucTradeId == TRADEID_POINTSALE_ISSUER
			|| ucTradeId == TRADEID_POINTUNSALE_ISSUER)     //发卡行积分消费
					{
		strcat(buf, "048");
	}

	if (SetFieldData(pstBag8583, 60, buf, strlen(buf)) < 0) {
		Assert(0);
	}

	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField061
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField061(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u8 temp[512] = { 0 };

	if (pstTradeData == NULL) {
		return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
	}

	FormField61Origdata(pstTradeData->ucTradeId, temp);

	SetFieldData(pstBag8583, 61, temp, strlen(temp));

	return true;
}
/*****************************************************************************
 函 数 名  : SetPublicField062
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField062(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
//    s32 len;
//    u8 temp[512] = {0};
//    u8 possn[512] = {0};
//    if((len = ProviderGetTagValue(TAG_8583_Field6201Type, possn)) > 0)
//    {
//        sdkBcdToAsc(temp, possn,len);
//    }

	//SetFieldData(pstBag8583, 62, temp, strlen(temp));
	return true;
}

/*****************************************************************************
 函 数 名  : SetPublicField063
 功能描述  : 组织域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField063(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	u8 temp[512] = { 0 };
	u8 aid[6] = { 0 };
	s32 len = 128;

	//国际信用卡公司代码
	memset(temp, 0, sizeof(temp));
	len = 128;
	//zxx 20130709 18:9只有icc卡才去读
	if (ProviderGetEmvTagValue("\x9F\x06", temp, &len) > 0 //Fixme ProviderGetEmvTagValue
			&& (pstTradeData->strCardInfo.stCardParam.ucCardMode == SDK_ICC_ICC
					|| pstTradeData->strCardInfo.stCardParam.ucCardMode
							== SDK_ICC_RF))      //aid字节长度
			{
		if (len >= 5) {
			memcpy(aid, temp, 5);
			if (!GetInterCode(temp, aid)) {
				strcpy(temp, "cup");
			}
		} else {
			strcpy(temp, "000");
		}

	} else {
		strcpy(temp, "000");
	}
	SetFieldData(pstBag8583, 63, temp, 3);

	return true;

}

/*****************************************************************************
 函 数 名  : SetPublicField064
 功能描述  : 组织域数据 主要目的把64bitmap置位
 输入参数  : SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月17日
 作    者   : zhangjun
 修改内容   : 新生成函数

 *****************************************************************************/
bool SetPublicField064(SDK_8583_ST8583 *pstBag8583,
		struct _TradeData *pstTradeData) {
	//主要目的把64bitmap置位
	SetFieldData(pstBag8583, 64, "\x00\x00\x00\x00\x00\x00\x00\x00", 8);

	return true;
}

/*****************************************************************************
 函 数 名  : SetFieldData
 功能描述  : 设置域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583
 u8 nFieldNo
 u8 *pFieldData
 u16 nFieldDataLen
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   :
 作    者   :
 修改内容   :

 *****************************************************************************/
s32 SetFieldData(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData,
u16 nFieldDataLen) {
	return sdk8583SetField(gstFieldTab[nFieldNo], pstBag8583, nFieldNo,
			pFieldData, nFieldDataLen);
}
/*****************************************************************************
 ** Descriptions:	设置域数据
 ** Parameters:    	SDK_8583_ST8583 *pstBag8583 8583结构体
 u8 nFieldNo      域号
 u8 *pFieldData   域数据
 u16 nFieldDataLen    域长度
 ** Returned value:
 ** Created By:		周晓鑫2013.07.23
 ** Remarks: 	//zxx 20130723 16:28 在原有的数据上追加，比较适用于复合域61.x 63.x
 *****************************************************************************/
s32 SetFieldDataAppend(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData,
u16 nFieldDataLen) {
	u8 temp[512] = { 0 };
	if (sdk8583IsDomainExist(pstBag8583, nFieldNo)) {
		GetFieldData(pstBag8583, nFieldNo, temp, sizeof(temp));
		strncat(temp, pFieldData, nFieldDataLen);
		return sdk8583SetField(gstFieldTab[nFieldNo], pstBag8583, nFieldNo,
				temp, strlen(temp));
	}
	return SDK_ERR;
}
/*
 s32 SetFieldDataGet(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData, u16 nFieldDataLen,TagList *pTagList,u32 tag)
 {
 u8 temp[512] = {0};
 printf("pFieldData.11 is %s\r\n",pFieldData);
 if(GetTagValue(pTagList, tag))
 {
 GetFieldData(pstBag8583, nFieldNo, temp, 256);
 printf("temp60.11 is %s\r\n",temp);
 strncat(temp, pFieldData, nFieldDataLen);
 printf("temp60.111 is %s\r\n",temp);
 return sdk8583SetField(gstFieldTab[nFieldNo], pstBag8583, nFieldNo, temp, strlen(temp));
 }
 return SDK_ERR;
 }
 */
/*****************************************************************************
 函 数 名  : GetFieldData
 功能描述  : 获取域数据
 输入参数  : SDK_8583_ST8583 *pstBag8583
 u8 nFieldNo
 u8 *pFieldData
 u16 nMaxSize
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   :
 作    者   :
 修改内容   :

 *****************************************************************************/
s32 GetFieldData(SDK_8583_ST8583 *pstBag8583, u8 nFieldNo, u8 *pFieldData,
u16 nMaxSize) {
	return sdk8583GetField(gstFieldTab[nFieldNo], pstBag8583, nFieldNo,
			pFieldData, nMaxSize);
}

/*****************************************************************************
 函 数 名  : ParseField8583
 功能描述  : 解析8583数据
 输入参数  : SDK_8583_ST8583 *pstBag8583
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2013年1月31日
 作    者   : zj
 修改内容   : 新生成函数

 *****************************************************************************/
s32 ParseField8583(SDK_8583_ST8583 *pstBag8583) {
	return sdk8583Parse8583(gstFieldTab, pstBag8583, 11);
}

/*****************************************************************************
 函 数 名  : InitFormSt8583
 功能描述  : 初始化8583报文,并增加报文头
 输入参数  : SDK_8583_ST8583 *pstBag8583
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   :
    作    者   :
    修改内容   :

*****************************************************************************/
s32 InitFormSt8583(SDK_8583_ST8583 *pstBag8583, struct _TradeData *pstTradeData) {
	int len;
	u8 temp[256] = { 0 };

	memset(pstBag8583, 0x00, sizeof(SDK_8583_ST8583));

	//TPDU
	u8 *asTpdu = pstTradeData->asTpdu;   //"6000630000";
	sdkAscToBcd(pstBag8583->ucBagData, asTpdu, 10);
	pstBag8583->nBagLen += 5;

	//应锟斤拷锟斤拷锟斤拷锟?
	/*
	if(stgAppSetting.stTradeCardType.bIsSupportIcc || stgAppSetting.stTradeCardType.bIsSupportRf)
	{
		pstBag8583->ucBagData[pstBag8583->nBagLen++] = 0x61;                                             //IC卡应用
	}
	else
	{
		pstBag8583->ucBagData[pstBag8583->nBagLen++] = 0x60;                                             //磁条卡应用
	}
	*/
    if(pstTradeData->strCardInfo.stCardParam.ucCardMode == SDK_ICC_ICC || pstTradeData->strCardInfo.stCardParam.ucCardMode == SDK_ICC_RF)
    {
        pstBag8583->ucBagData[pstBag8583->nBagLen++] = 0x61; //IC卡
    }
    else
    {
        pstBag8583->ucBagData[pstBag8583->nBagLen++] = 0x60; //默认60、需要发包前重新设置                                           //磁卡应用
    }
	//软件总版本号
	pstBag8583->ucBagData[pstBag8583->nBagLen++] = 0x02;

	//终端状态及处理要求
	pstBag8583->ucBagData[pstBag8583->nBagLen++] = ((0x04 & 0x0F)
			| (0x00 & 0x0F) << 4);
	//header[len++] = (byte) ((ProcessingDemand & 0x0F) | (terminalState & 0x0F)<<4);

//    //软件分版本号
//    sdkAscToBcd(&pstBag8583->ucBagData[pstBag8583->nBagLen], "123456", 6);
//    pstBag8583->nBagLen += 3;

	// 0 填充
	memcpy(&pstBag8583->ucBagData[8], "\x00\x00\x00", 3);
	pstBag8583->nBagLen += 3;

	pstBag8583->stFiled[SDK_8583_FIELD_MSG].nFieldHead = pstBag8583->nBagLen;

	len = SetFieldData(pstBag8583, SDK_8583_FIELD_MSG, "0000", 4);
	if (len < 0) {
		return len;
	}

	//位元表,此时实际只是占位,实际值有设置域时自动处理
	memset(temp, 0, sizeof(temp));
	len = SetFieldData(pstBag8583, SDK_8583_FIELD_MAP, temp, 0);
	if (len < 0) {
		return len;
	}

	return pstBag8583->nBagLen;

}

/*
 * DW.
 *
 * 参数：
 * bEncrypt				1加密	0解密
 * puc8583Packet		明文
 * pus8583PacketLen		明文长度
 *
 * 华夏 全报文加密
 * 当通讯方式为： TCPIP / GPRS / CDMA 要求全报文加密（公网接入）
 *
 */
s16 EncryptAllMsgByHX(bool bEncrypt, u8 * puc8583Packet, u16 * pus8583PacketLen) {
	u16 nRet = 0;
	u8 i = 0;
	s32 iMacKLen;
	u8 aucMacK[20] = { 0 };							// 工作密钥-MACKEY
	u8 aucEncryptData[SDK_8583_BAGMAXLEN] = { 0 };	// 密文数据
	u16 usEncryptDataLen = 0;						// 密文数据长度
	u8 aucTempData[SDK_8583_BAGMAXLEN] = { 0 };	// 暂存数据

	Log("zbq","jump into EncryptAllMsgByHX");
	if (bEncrypt) {
		// 全报文加密
//        g_EncryptAllMsgflag = true;
		// 5字节 TPDU + 7字节 报文头 + 2字节 报文长度
		memcpy(aucEncryptData, puc8583Packet, 6);		// TPDU + 应用类别
		aucEncryptData[6] = 0x04;						// 软件版本号 公网接入
		aucEncryptData[7] = 0x07;						// 处理要求 全报文加密
		// 终端号(BCD)
		sdkAscToBcd(&aucEncryptData[8],gstAppSetting.stTerminalParam.asTerminalNo, 8);
		// 2字节 报文长度(BCD)
		sdkU16ToBcd(&aucEncryptData[12], (*pus8583PacketLen - 11/*TPDU + 报文头*/),2);
		// 拷贝报文
		memcpy(&aucEncryptData[14], puc8583Packet + 11, *pus8583PacketLen - 11);
		usEncryptDataLen = *pus8583PacketLen + 3;

		// 更改报文头 应用类别 - 网络管理类应用
		if (TRADEID_MANLOGIN == gstTradeData.ucTradeId			// 签到
		|| TRADEID_MANLOGOUT == gstTradeData.ucTradeId			// 签退
		|| TRADEID_RINGDETECT == gstTradeData.ucTradeId			// 回响测试
		|| TRADEID_DOWNKEY == gstTradeData.ucTradeId			// 密钥下载
		|| TRADEID_SETTLE == gstTradeData.ucTradeId				// 结算
		|| TRADEID_SENDICCTC == gstTradeData.ucTradeId			// TC上送
		|| TRADEID_TMSDOWNINFO == gstTradeData.ucTradeId
		|| TRADEID_POSPARAMDL == gstTradeData.ucTradeId
		|| TRADEID_DOWNLOADPARAM == gstTradeData.ucTradeId		// 参数下载
		|| TRADEID_QUERYBANK == gstTradeData.ucTradeId			// 开户行查询
		|| TRADEID_ADDOFTENHOME == gstTradeData.ucTradeId		// 添加常汇户
		|| TRADEID_QUERYOFTENHOMES == gstTradeData.ucTradeId	// 查询常汇户
		|| TRADEID_ELECSIGN == gstTradeData.ucTradeId			// 签购单上送
		) {
			aucEncryptData[5] = 0x85;
		}

//		TraceHex("huangxin", "Send 加密前报文\r\n", aucEncryptData, usEncryptDataLen);
		Log("zbq","EncryptAllMsgByHX 111");
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,aucEncryptData,usEncryptDataLen);
		Log("zbq","Send 加密前报文= %s , Len = %d", aucTempData, usEncryptDataLen);


		/*
		 * DW.
		 * 计算 加密密钥
		 * 采用单倍长的DES密钥算法：
		 * 管理类交易加密密钥：使用8字节的华夏银行银联入网机构号(63040000)异或8字节的终端号作为加密密钥。
		 * 联机类交易加密密钥：使用mackey对终端号做3des运算，得到的8字节值作为加密密钥。
		 *
		 */
		memset(aucEncryptKey, 0, 8);
		if (aucEncryptData[5] == 0x85) {
			Log("zbq","管理类交易全报文加密");
			// 管理类交易
			u8 aucHXNumber[8] = { 0x36, 0x33, 0x30, 0x34, 0x30, 0x30, 0x30, 0x30 };
			for (i = 0; i < 8; i++) {
				aucEncryptKey[i] = gstAppSetting.stTerminalParam.asTerminalNo[i] ^ aucHXNumber[i];
			}
//			TraceHex("huangxin", "管理类交易-加密密钥\r\n", aucEncryptKey, 8);

			memset(aucTempData,0,SDK_8583_BAGMAXLEN);
			sdkBcdToAsc(aucTempData,aucEncryptKey,8);
			Log("zbq","管理类交易-加密密钥=  %s ", aucTempData);
		} else {
			// 联机类交易
//            iMacKLen = 16;
//            sdkReadFile("/mtd0/dll/allmsg", aucMacK, 0, &iMacKLen);
//            memcpy(aucEncryptKey, gstAppSetting.stTerminalParam.asTerminalNo, 8);
//            sdkDes3S(1, aucEncryptKey, aucMacK);
			Log("zbq","联机类交易全报文加密");
			u32 aucEncryptKeylen = 0;
			memcpy(aucMacK, gstAppSetting.stTerminalParam.asTerminalNo, 8);
			ProviderDesEncByWKey(aucMacK, 8, aucEncryptKey, &aucEncryptKeylen,1);
//			TraceHex("huangxin", "联机类交易-加密密钥\r\n", aucEncryptKey, 8);

			memset(aucTempData,0,SDK_8583_BAGMAXLEN);
			sdkBcdToAsc(aucTempData,aucEncryptKey,8);
			Log("zbq","联机类交易-加密密钥=  %s ", aucTempData);
		}

		if(ProviderSetTagValue(TAG_8583_AUCENCRYPTKEY,aucEncryptKey,8) != 1)
		{
			return SDK_ERR;
		}

		/*
		 * 加密报文:
		 * 1.去除TPDU+报文头
		 * 2.TPDU+报文头 不参与加密
		 * 3.增加 TPDU+报文头
		 */
		usEncryptDataLen -= 12;
		EncryptAllMsg(aucEncryptKey, aucEncryptData + 12, &usEncryptDataLen);
		usEncryptDataLen += 12;

		memcpy(puc8583Packet, aucEncryptData, usEncryptDataLen);
		*pus8583PacketLen = usEncryptDataLen;
//			TraceHex("huangxin", "Send 加密后报文\r\n", puc8583Packet, *pus8583PacketLen);

		Log("zbq","Send 加密后报文 Len = %d", *pus8583PacketLen);
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,puc8583Packet,*pus8583PacketLen);
		Log("zbq","Send 加密后报文=  %s ", aucTempData);
	} else {
		Log("zbq","DecryptAllMsg 111");
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,puc8583Packet,*pus8583PacketLen);
		Log("zbq","Recv 解密前报文= %s , Len = %d", aucTempData, *pus8583PacketLen);

		if(nRet = ProviderGetTagValue(TAG_8583_AUCENCRYPTKEY,aucEncryptKey) != 8)
		{
			Log("zbq","解密密钥 Len = %d", nRet);
			return SDK_ERR;
		}
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,aucEncryptKey,8);
		Log("zbq","原加密密钥=  %s ", aucTempData);

		memcpy(aucEncryptData, puc8583Packet, *pus8583PacketLen);
		usEncryptDataLen = *pus8583PacketLen;
		usEncryptDataLen -= 12;
		DecryptAllMsg(aucEncryptKey, aucEncryptData + 12 /*解密不包含TPDU+报文头*/,&usEncryptDataLen);
		// 解密完成后数据长度加上12
		usEncryptDataLen += 12;
//            TraceHex("huangxin", "Recv 解密后报文\r\n", aucEncryptData, usEncryptDataLen);

		Log("zbq","Recv 解密后报文 Len = %d", usEncryptDataLen);
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,aucEncryptData,usEncryptDataLen);
		Log("zbq","Recv 解密后报文 =  %s ", aucTempData);

		// 去掉因全报文加密而新增的3字节数据
		memcpy(puc8583Packet, aucEncryptData, 11);
		memcpy(puc8583Packet + 11, aucEncryptData + 14, usEncryptDataLen - 14);
		*pus8583PacketLen = usEncryptDataLen - 3;
//		TraceHex("huangxin", "Recv 还原后报文\r\n", puc8583Packet, *pus8583PacketLen);

		Log("zbq","Recv 还原后报文 Len = %d", *pus8583PacketLen);
		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,puc8583Packet,*pus8583PacketLen);
		Log("zbq","Recv 还原后报文=  %s ", aucTempData);
	}
	Log("zbq","jump out EncryptAllMsgByHX");
	return SDK_OK;
}

// 华夏.报文加密.单DES算法
void EncryptAllMsg(u8 * pucEncryptKey, u8 * pucMsgData, u16 * pusMsgDataLen) {
	u8 aucCleartext[SDK_8583_BAGMAXLEN];	// 明文
	u8 aucCiphertext[8];					// 密文
	u16 usCleartextLen;						// 明文长度
	u16 usCiphertextLen;					// 密文长度

	if (NULL == pucEncryptKey || NULL == pucMsgData) {
		return;
	}

	Log("zbq","jump into EncryptAllMsg");

	usCiphertextLen = 0;
	usCleartextLen = *pusMsgDataLen;
	memcpy(aucCleartext, pucMsgData, usCleartextLen);

	while (1) {
		if (usCleartextLen >= 8) {
			memcpy(aucCiphertext, aucCleartext + usCiphertextLen, 8);
			sdkDesS(1, aucCiphertext, pucEncryptKey);
			usCleartextLen -= 8;
			memcpy(pucMsgData + usCiphertextLen, aucCiphertext, 8);
			usCiphertextLen += 8;
			if (0 == usCleartextLen) {
				break;
			}
		} else {
			memcpy(aucCiphertext, aucCleartext + usCiphertextLen,
					usCleartextLen);
			memset((aucCiphertext + usCleartextLen), 0, (8 - usCleartextLen));
			sdkDesS(1, aucCiphertext, pucEncryptKey);
			memcpy(pucMsgData + usCiphertextLen, aucCiphertext, 8);
			usCiphertextLen += 8;
			break;
		}
	}
	*pusMsgDataLen = usCiphertextLen;

	Log("zbq","jump out EncryptAllMsg");
}

// 华夏.报文解密.单DES算法
void DecryptAllMsg(u8 * pucEncryptKey, u8 * pucMsgData, u16 * pusMsgDataLen) {
	u8 aucData[SDK_8583_BAGMAXLEN];
	u8 aucCleartext[8];						// 明文
	u16 usCleartextLen;						// 明文长度
	u16 usCiphertextLen;					// 密文长度
	u8 aucTempData[SDK_8583_BAGMAXLEN] = { 0 };	// 暂存数据

	if (NULL == pucEncryptKey || NULL == pucMsgData) {
		return;
	}

	Log("zbq","jump into DecryptAllMsg");

	usCleartextLen = 0;
	usCiphertextLen = *pusMsgDataLen;
	memset(aucData, 0, sizeof(aucData));

	Log("zbq","Recv 解密前报文 Len = %d", usCiphertextLen);
	memset(aucTempData,0,SDK_8583_BAGMAXLEN);
	sdkBcdToAsc(aucTempData,pucMsgData,usCiphertextLen);
	Log("zbq","Recv 解密前报文 =  %s ", aucTempData);


	while (usCiphertextLen) {
		memcpy(aucCleartext, pucMsgData + usCleartextLen, 8);

		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,aucCleartext,8);
		Log("zbq","解密前数据  =  %s ", aucTempData);

		sdkDesS(0, aucCleartext, pucEncryptKey);

		memset(aucTempData,0,SDK_8583_BAGMAXLEN);
		sdkBcdToAsc(aucTempData,aucCleartext,8);
		Log("zbq","解密后数据  =  %s ", aucTempData);

		usCiphertextLen -= 8;
		memcpy(aucData + usCleartextLen, aucCleartext, 8);
		usCleartextLen += 8;
	}

	sdkBcdToU16(pusMsgDataLen, aucData, 2);
	*pusMsgDataLen += 2;	// 加上报文长度2字节
	memcpy(pucMsgData, aucData, *pusMsgDataLen);

	memset(aucTempData,0,SDK_8583_BAGMAXLEN);
	sdkBcdToAsc(aucTempData,pucMsgData,*pusMsgDataLen);
	Log("zbq","解密后报文数据  =  %s,len = %d ", aucTempData, *pusMsgDataLen);


	Log("zbq","jump out DecryptAllMsg");
}

/*****************************************************************************
   函 数 名  : GetSysTime
   功能描述  : 获取当前系统时间
*****************************************************************************/
int GetSysTime(void)
{
	u8 sysTime[6] = {0};
	u16 sysTimeLen = 0;
	u8 aucTempData[256] = { 0 };	// 暂存数据

	ProviderGetSysTime(sysTime,&sysTimeLen);
	if(sysTimeLen != 6){
		Log("zbq","Error sysTimeLen = %d ",sysTimeLen);
		return -1;
	}
	else{
		memset(aucTempData,0,256);
		sdkBcdToAsc(aucTempData,sysTime,8);
		Log("zbq","sysTime BCD = %s ",aucTempData);

		gstSys.ucYear = sysTime[0];
		gstSys.ucMonth = sysTime[1];
		gstSys.ucDay = sysTime[2];
		gstSys.ucHour = sysTime[3];
		gstSys.ucMinute = sysTime[4];
		gstSys.ucSecond = sysTime[5];
		return SDK_OK;
	}
}


/*****************************************************************************
 ** Descriptions:        判断发卡行认证是否失败
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		lilin2014.05.07
 ** Remarks:
 *****************************************************************************/
bool JudgeIssuerAuteErr(void) {
	bool flag = false;
	u8 temp[512];
	s32 len;

	//通过TVR判断发卡行认证结果
	memset(temp, 0, sizeof(temp));
	if (sdkIccReadTLV("\x95", temp, &len) > 0) {
		if (0x40 == (temp[4] & 0x40))	//发卡行认证失败
				{
			flag = true;
		}
	}
	if (flag) {
		flag = false;
		//通过TSI判断发卡行认证是否进行
		memset(temp, 0, sizeof(temp));
		if (sdkIccReadTLV("\x9B", temp, &len) > 0) {
			if (0x10 == (temp[0] & 0x10))	//发卡行认证已经进行
					{
				flag = true;
			}
		}
	}
	return flag;
}
/*****************************************************************************
 ** Descriptions:	判断脚本执行结果是否OK
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		lilin2014.05.07
 ** Remarks:
 *****************************************************************************/
bool JudgeScriptResultOK(void) {
	bool flag = false;
	u8 temp[512];
	s32 len;
	s32 i = 0;
	//圈存脚本处理结果长度为0，置起冲正。
	//zxx 20130814 14:15 修改脚本结果判断
	memset(temp, 0, sizeof(temp));
	if (sdkIccReadTLV("\x95", temp, &len) > 0) //通过TVR判断脚本执行结果
			{
		if (0x10 != (temp[4] & 0x10)) //脚本执行成功
				{
			flag = true;
		}
	}
	if (flag) {
		flag = false;
		if (sdkIccReadTLV("\x9B", temp, &len) > 0) //通过TSI判断脚本执行结果
				{
			if (0x04 == (temp[0] & 0x04)) //脚本已执行
					{
				flag = true;
			}
		}
	}
	if (flag) {
		flag = false;
		len = 0;
		memset(temp, 0, sizeof(temp));
		sdkIccGetScriptResult(&len, temp);
		if (len) {
			for (i = 0; i < len / 5; i++) {
				if ((temp[i * 5] & 0xF0) == 0x20) //脚本结果执行成功
						{
					flag = true;
				} else {
					flag = false;
					break;
				}
			}
		}
	}
	return flag;
}

/*****************************************************************************
 函 数 名  : ReConfigBag8583
 功能描述  : 重新设置8583报文头
 输入参数  : SDK_8583_ST8583 *pstBag8583
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2013年3月12日
 作    者   : zhangjun
 修改内容   : 新生成函数
 备    注   : 报文头应用类别需要根据22域来设置
 *****************************************************************************/
void ReConfigBag8583(SDK_8583_ST8583 *pstBag8583) {
	u8 temp[32] = { 0 };

	//TPDU
	sdkAscToBcd(pstBag8583->ucBagData, gstAppSetting.stCommuParam.asTpdu, 10);

	//应用类型
	if (GetFieldData(pstBag8583, 22, temp, sizeof(temp)) > 0) {
		if (!memcmp(temp, "05", 2) || !memcmp(temp, "07", 2)) {
			pstBag8583->ucBagData[5] = 0x61;
		}
	}
}
/*****************************************************************************
 函 数 名  : CheckBitmap
 功能描述  : 判断Bitmap表是否将该域置位
 输入参数  : const u8* heBitmap
 u32 uiFieldNo
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2012年12月27日
 作    者   : zj
 修改内容   : 新生成函数

 *****************************************************************************/
bool CheckBitmap(const u8 *heBitmap, u32 uiFieldNo) {
	u8 Tmp;
	u32 i;
	u32 j;

	if (uiFieldNo == 0) {
		return 0;
	}

	i = (uiFieldNo - 1) / 8;
	j = (uiFieldNo - 1) % 8;

	Tmp = 0x80;
	Tmp = Tmp >> j;

	Tmp = Tmp & heBitmap[i];

	if (Tmp > 0) {
		return true;
	}

	return false;
}

/*****************************************************************************
 函 数 名  : FormPublicSend
 功能描述  : 按Bitmap组织公共域数据(会进行初始化)
 输入参数  : SDK_8583_ST8583 *pstBag8583
 struct _TradeData *pstTradeData
 u8 *pasBitMap
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   : 2013年1月31日
 作    者   : zj
 修改内容   : 新生成函数

 *****************************************************************************/
s32 FormPublicSend(SDK_8583_ST8583 *pstBag8583, struct _TradeData *pstTradeData,
u8 *pasBitMap)
//s32 FormPublicSend(SDK_8583_ST8583 *pstBag8583, u8 *pasBitMap)
{
	u32 len = 0;
	u32 i = 0;
	u32 uiMaxFieldNo = 64;
	bool bOk = true;
	u8 heBitmap[16] = { 0 };
	u8 temp[512] = { 0 };
	u8 possn[512] = { 0 };
	u8 proccode[32] = { 0 };
	// memset(temp, '0', sizeof(temp) - 1);
	if (pstBag8583 == NULL) {
//        Assert(0);
		return 0;
	}
	Log("wangb", "pasBitMap is %s", pasBitMap);
	InitFormSt8583(pstBag8583, pstTradeData);
	Log("wangb", "pstBag8583 is %s ", pstBag8583->ucBagData);
	len = strlen(pasBitMap);
	if (len > 32) {
		len = 32;
	}
	sdkAscToBcd(heBitmap, pasBitMap, len);
	if (CheckBitmap(heBitmap, 1)) {
		uiMaxFieldNo = 128;
	}
	for (i = 2; i <= uiMaxFieldNo; i++) {
		if (!CheckBitmap(heBitmap, i)) {
			continue;
		}
		Log("wangb", "i is %d ", i);
		switch (i) {
		case 2:
			bOk = SetPublicField002(pstBag8583, pstTradeData);
			break;

		case 3:
			bOk = SetPublicField003(pstBag8583, pstTradeData);
			break;

		case 4:
			bOk = SetPublicField004(pstBag8583, pstTradeData);
			break;

		case 11:
			bOk = SetPublicField011(pstBag8583, pstTradeData);
			break;

		case 14:
			bOk = SetPublicField014(pstBag8583, pstTradeData);
			break;

		case 22:
			bOk = SetPublicField022(pstBag8583, pstTradeData);
			break;

		case 23:
			bOk = SetPublicField023(pstBag8583, pstTradeData);
			break;

		case 25:
			bOk = SetPublicField025(pstBag8583, pstTradeData);
			break;

		case 26:
			bOk = SetPublicField026(pstBag8583, pstTradeData);
			break;

		case 35:
			bOk = SetPublicField035(pstBag8583, pstTradeData);
			break;

		case 36:
			bOk = SetPublicField036(pstBag8583, pstTradeData);
			break;

		case 37:
			bOk = SetPublicField037(pstBag8583, pstTradeData);
			break;

		case 38:
			bOk = SetPublicField038(pstBag8583, pstTradeData);
			break;

		case 41:
			bOk = SetPublicField041(pstBag8583, pstTradeData);
			break;

		case 42:
			bOk = SetPublicField042(pstBag8583, pstTradeData);
			break;

		case 49:
			bOk = SetPublicField049(pstBag8583, pstTradeData);
			break;

		case 52:
			bOk = SetPublicField052(pstBag8583, pstTradeData);
			break;

		case 53:
			bOk = SetPublicField053(pstBag8583, pstTradeData);
			break;

		case 55:
			bOk = SetPublicField055(pstBag8583, pstTradeData);
			break;

		case 60:
			bOk = SetPublicField060(pstBag8583, pstTradeData);
			break;

		case 61:
			bOk = SetPublicField061(pstBag8583, pstTradeData);
			break;

		case 62:
			bOk = SetPublicField062(pstBag8583, pstTradeData);
			break;

		case 63:
			bOk = SetPublicField063(pstBag8583, pstTradeData);
			break;

		case 64:
			bOk = SetPublicField064(pstBag8583, pstTradeData);
			break;

		default: //没有公共处理的填充默认值
			SetFieldData(pstBag8583, i, temp, 0);
			break;
		}

		if (!bOk) {
			//锟斤拷锟斤拷锟斤拷锟绞э拷锟?
//            Trace("锟斤拷锟斤拷锟斤拷", "锟斤拷锟?d锟斤拷失锟斤拷\r\n", i);
			return 0;
		}
	}

	return pstBag8583->nBagLen;
}
/*****************************************************************************
** Descriptions:	获得银行名称
** Parameters:    	u8 *lpOut   银行代码
                               u8 *lpBankCode   银行名称
                               u8 ucType 0 发卡行1收单行
** Returned value:
** Created By:		lilin2013.03.13
** Remarks:
*****************************************************************************/
void GetBankName(u8 *lpOut, u8 *lpBankCode, u8 ucType)
{
    u16 i;
    u16 k;
//Trace("lilin","获得银行名称!\r\n");

    k = sizeof(BankNameTab) / sizeof(BankNameTab[0]);
    for (i = 0; i < k; i++)
    {
        if (!memcmp(BankNameTab[i].BankCode, lpBankCode, 4))
        {
            strcpy(lpOut, BankNameTab[i].BankName);
            return;
        }
    }
//#if(PROGRAM_TYPE==UMS_PROGRAM)
//    if (strlen(gstAppSetting.stPrintSetParam.asDefaultIssuerBankName) && ucType == 0)
//    {
//        memcpy(lpOut, gstAppSetting.stPrintSetParam.asDefaultIssuerBankName, 8);
//    }
//        else
//#endif
//    {
//        memcpy(lpOut, lpBankCode, 8);
//    }

}
/*****************************************************************************
** Descriptions:	检查金额是否正确
** Parameters:    	u8 *amout
                               u8 len
** Returned value:
** Created By:		lilin2012.12.22
** Remarks:
*****************************************************************************/
u8 CheckAmoutFormat(u8 *amout, u8 len)
{
    u8 temp[256];
    u8 i;

    memset(temp, 0, sizeof(temp));
    memcpy(temp, amout, len);
    for (i = 0; i < len; i++)
    {
        if (temp[i] < 0x30 || temp[i] > 0x39) //不是数字
        {
            return false;    //金额格式不正确
        }
    }
    return true;
}

/*****************************************************************************
 ** Descriptions:	取8583包各个域置给相对应TAG
 ** Parameters:          void
 ** Returned value:
 ** Created By:		wangb 2016 0301
 ** Remarks:
 ***************************************************+**************************/

s32 Set8583Tvl(SDK_8583_ST8583 *pstBag8583) {
	u32 i = 0, slt = 1;
	bool bOk;
	u8 proccode[256] = { 0 };
	u32 uiMaxFieldNo = 64;
	if (pstBag8583 == NULL) {
		return SDK_ERR;
	}
	for (i = 2; i <= uiMaxFieldNo; i++) {
		memset(proccode, 0x00, 256);
		switch (i) {
		case 2:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CARDNO, proccode, slt);
			}
			break;

		case 3:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_POSVLU, proccode, slt);
			}
			break;

		case 4:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_AMOUNT, proccode, slt);
			}
			break;

		case 11:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
                    slt =ProviderSetTagValue(TAG_8583_VoucherNo,proccode,slt);
			}
			break;
		case 12:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_HMS, proccode, slt);
			}
                break;
		case 13:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
//				slt = GetFieldData(pstBag8583, i, proccode, 256);
//				ProviderSetTagValue(TAG_8583_MD, proccode, slt);
            	ProviderGetTagValue(TAG_8583_MD, proccode);
                slt = GetFieldData(pstBag8583, i, &proccode[4], 256);
                ProviderSetTagValue(TAG_8583_MD,proccode,slt+4);
			}
			break;
		case 14:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_ExpiredDate, proccode, slt);
			}
			break;

		case 22:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CardMode, proccode, slt);
			}
			break;

		case 23:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CardParam, proccode, slt);
			}
			break;

		case 25:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CardParam, proccode, slt);
			}
			break;
			/*
			 case 26:
			 bOk = SetPublicField026(pstBag8583,pTagList);
			 break;
			 */
		case 35:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_Track2Data, proccode, slt);
			}
			break;

		case 36:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_Track3Data, proccode, slt);
			}
			break;

		case 37:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_SysRefNo, proccode, slt);
			}
			break;

		case 38:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_AuthCode, proccode, slt);
			}
			break;
		case 39:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_BKRESULT, proccode, slt);
			}
			break;

		case 41:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_TerminalNo, proccode, slt);
			}
			break;

		case 42:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_MerchantNo, proccode, slt);

			}
			break;

        case 44:
            if (sdk8583IsDomainExist(pstBag8583, i))
            {
                u8 buf[512]={0},printbuf[128]={0};
                slt = GetFieldData(pstBag8583, i, proccode, 256);
                if (1)
                {
                    memset(printbuf, 0, sizeof(printbuf));
                    GetBankName(printbuf, proccode , 0);
                    strcpy(buf, printbuf);

                }
                else
                {
                    strncat(buf, proccode , 8);
                }
                ProviderSetTagValue(TAG_8583_IssuerBank,buf,slt);
                if (1)
                {
                    memset(printbuf, 0, sizeof(printbuf));
                    memset(buf, 0, sizeof(buf));
                    GetBankName(printbuf, proccode + 11, 1);
                    strcpy(buf, printbuf);
                }
                else
                {
                    strncat(buf, proccode + 11, 8);
                }
                ProviderSetTagValue(TAG_8583_AcquirerBank,buf,slt);
            }
            break;

		case 49:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CurrencyCode, proccode, slt);
			}
			break;

		case 52:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_CreditPwd, proccode, slt);
			}
			break;

		case 53:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_DomainExist, proccode, slt);
			}
			break;

		case 55:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_EMV55Type, proccode, slt);
			}
			break;

//            case 60:
//                if (sdk8583IsDomainExist(pstBag8583, i))
//                {
//                    slt = GetFieldData(pstBag8583, i, proccode, 256);
//                    ProviderSetTagValue(TAG_8583_Field6002Type,proccode,slt);
//                }
//                break;

		case 61:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_OldAsBatchNo, proccode, slt);
			}
			break;

        case 62:
            if (sdk8583IsDomainExist(pstBag8583, i))
            {
                 slt = GetFieldData(pstBag8583, i, proccode, 256);
                 u8 tmp[64]={0};
                 if (gstTradeData.ucTradeId == TRADEID_INSTALLMENTSALE )
                 {
                        //FF42      分期付款首期金额        Cn,6        分期付款消费交易应答  C，原交易应答返回时出现，包含12位数字
                        memset(tmp, 0, sizeof(tmp));
                        memcpy(tmp, &proccode[0], 12);
                        ProviderSetTagValue(TAG_TRADE_INSPAY,tmp,slt);
                        //FF43      分期付款还款币种        Cn,2        分期付款消费交易应答  C，原交易应答返回时出现，包含3位数字
                        memset(tmp, 0, sizeof(tmp));
                        tmp[0] = 0x30;
                        memcpy(&tmp[1], &proccode[12], 3);
                        ProviderSetTagValue(TAG_TRADE_REPAYMENT,tmp,slt);

                        //FF44      持卡人手续费      Cn,6        分期付款消费交易应答  C，原交易应答返回时出现,包含12位数字
                        memset(tmp, 0, sizeof(tmp));

                        if (gstTradeData.asPayMode[0] == '1') //分期支付
                        {
                            if (CheckAmoutFormat(proccode + 52, 12) == true)   //xule120309 如果格式正确打印金额,否则不打
                            {
                                memcpy(tmp, proccode + 52, 12);
                            }
                        }
                        else //一次性支付手续费 xule 0927
                        {
                            if (CheckAmoutFormat(proccode + 15, 12) == true)
                            {
                                memcpy(tmp, proccode + 15, 12);
                            }
                        }
                        ProviderSetTagValue(TAG_TRADE_CardHOLDERFEE,tmp,slt);

                    ////////mark
                }
                if (gstTradeData.ucTradeId == TRADEID_POINTSALE_ISSUER || gstTradeData.ucTradeId == TRADEID_POINTSALE_UNION)
                {
                    bool flag = false;
                    int ret = 0;
                    //FF46      兑换积分数	    Cn,5        积分消费交易应答    C，原交易应答返回时出现，包含10位数字
                    memcpy(tmp, &proccode[0], 10);
                    ProviderSetTagValue(TAG_TRADE_EXPROSCORE,tmp,slt);

                    //FF47      积分余额数	    Cn,5        积分消费交易应答    C，原交易应答返回时出现，包含10位数字
                    memset(tmp, 0, sizeof(tmp));
                    memset(proccode, 0, sizeof(proccode));
                    if(GetFieldData(pstBag8583, 54, proccode, 256))
                    {
                        memcpy(tmp, proccode+8, 12);
                        ProviderSetTagValue(TAG_TRADE_EXPROQUERY,tmp,slt);
                    }
                    memset(tmp, 0, sizeof(tmp));
                    memset(proccode, 0, sizeof(proccode));

                    if(GetFieldData(pstBag8583, 60, proccode, 256))
                    {
                        //TraceHex("lhd_debug","judge2自付费金额是否上送 get 60 feild",temp,strlen(temp));
                        if(1 == (proccode[13] & 0x0F))//20141021_lhd
                        {
                            flag=true;
                        }
                    }
                    if(flag)
                    {
                        //FF48      自付金额        Cn,6        积分消费交易应答    C，部分承兑时出现包含12位数字
                        memset(proccode,0,sizeof(proccode));
                        if( GetFieldData(pstBag8583, 62, proccode, 256))
                        {
                            memcpy(tmp, proccode+10, 12);
                            ProviderSetTagValue(TAG_TRADE_SELFAMOUNT,tmp,slt);
                        }
                    }
                }
             //   ProviderSetTagValue(TAG_8583_Field6201Type,proccode,slt);
            }
            if(gstTradeData.ucTradeId == TRADEID_ECCACCNO || gstTradeData.ucTradeId == TRADEID_EPLOAD_NOACC || gstTradeData.ucTradeId == TRADEID_MAGLOAD_ACC)
            {
                //FF25      转入卡卡号	cn,var. up to 10        62域	C，当IC卡非指定帐户圈存时出现（不需要卡号屏蔽）
                 ProviderSetTagValue(TAG_TRADE_TRCARDNO,proccode,slt);
            }
            break;


//            case 63:
//                if (sdk8583IsDomainExist(pstBag8583, i))
//                {
//                    slt = GetFieldData(pstBag8583, i, proccode, 256);
//                    ProviderSetTagValue(TAG_8583_Field6301Type,proccode,slt);
////                    ProviderSetTagValue("FF9076","0",1);
////                    ProviderSetTagValue("FF9077",proccode,slt);
//                }
//                break;

		case 64:
			if (sdk8583IsDomainExist(pstBag8583, i)) {
				slt = GetFieldData(pstBag8583, i, proccode, 256);
				ProviderSetTagValue(TAG_8583_MAC, proccode, slt);
			}
			break;

		default: //没锟叫癸拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷默锟斤拷值
			SetFieldData(pstBag8583, i, proccode, 0);
			break;
		}
//        if(slt == 0)
//        {
//            return 0;
//        }

	}

	return slt;
}

/*****************************************************************************
 ** Descriptions:	解析60域
 ** Parameters:          struct _Field60 *pstField60
 ** Returned value:
 ** Created By:		lilin2012.08.06
 ** Remarks:
 *****************************************************************************/
bool ParseField60(struct _Field60 *pstField60) {
	u8 temp[256] = { 0 };
	s32 revlen;
	u16 j = 0;

	revlen = GetFieldData(&gstSigRecvBag, 60, temp, 256);     //取60域ASC值
	if (revlen) {
		Trace("ParseField60", "len=[%d],[%s]\r\n", revlen, temp);

		if (revlen >= 2) {
			memcpy(pstField60->asFunCode, &temp[j], 2);                   //交易类型
		}
		j += 2;

		if (revlen >= 8)                                                 //交易批次号
				{
			memcpy(pstField60->asBatchNo, &temp[j], 6);
		}
		j += 6;

		if (revlen >= 11)                                               //网络管理信息
				{
			memcpy(pstField60->asNetCode, &temp[j], 3);
		}
		j += 3;

		if (revlen >= 12) {
			pstField60->ucPosReadCap = temp[j];
		}
		j += 1;

		if (revlen >= 13)                                               //长度大于12
				{
			pstField60->ucIccCondition = temp[j];
		}
		j += 1;

		if (revlen >= 14)                                               //长度大于12
				{
			pstField60->ucPortion = temp[j];
			Trace("ParseField60", "pstField60->ucPortion=[%02X]\r\n",
					pstField60->ucPortion);
		}
		j += 1;

		if (revlen >= 15)                                               //长度大于12
				{
			memcpy(pstField60->asAccType, &temp[j], 3);
		}
		j += 1;

		return true; //lint !e438
	}
	return false;
}

bool CheckResponseCode(u8 const *resp) {
	if (memcmp(resp, "00", 2) == 0 || memcmp(resp, "11", 2) == 0
			|| memcmp(resp, "A2", 2) == 0 || memcmp(resp, "A4", 2) == 0
			|| memcmp(resp, "A5", 2) == 0 || memcmp(resp, "A6", 2) == 0) {
		return true;
	}
	return false;
}

/*****************************************************************************
 ** Descriptions:	拷贝
 ** Parameters:    	SDK_8583_ST8583 *pstDestBag8583
 u8 ucFieldNo
 SDK_8583_ST8583 *pstResBag8583
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
bool CopyBag8583Field(SDK_8583_ST8583 *pstDestBag8583, u8 ucFieldNo,
		SDK_8583_ST8583 *pstResBag8583) {
	Log("CopyBag8583Field", "CopyField : %d", ucFieldNo);
	if (sdk8583IsDomainExist(pstResBag8583, ucFieldNo)) {
		sdk8583CpyField(pstDestBag8583, pstResBag8583, ucFieldNo);
		return true;
	}
	Log("CopyBag8583Field", "CopyField Failure : %d !!!", ucFieldNo);
	return false;
}
///*****************************************************************************
//** Descriptions:	写脚本包标志
//** Parameters:    	SDK_8583_ST8583 *pstReversalBag
//** Returned value:
//** Created By:		huacong  2016.03.10
//** Remarks:
//*****************************************************************************/
//s32 WriteReverScriptFlag(bool bisReversal)
//{
//    u8 astemp[2 + 1];
//
//    memset(astemp, 0, sizeof(astemp));
//    if(bisReversal)
//    {
//        strcpy(astemp, "01");
//    }
//    else
//    {
//        strcpy(astemp, "00");
//    }
//
//    if(ProviderSetTagValue(TAG_SCIRTP_REVERSAL, astemp, 2) != 0)
//    {
//        return SDK_ERR;
//    }
//}
//
//
///*****************************************************************************
//** Descriptions:	读脚本标志
//** Parameters:    	bool bisReversal
//** Returned value:
//** Created By:		wangb  2016.03.21
//** Remarks:
//*****************************************************************************/
//s32 ReadReverScriptFlag(bool *bisReversal)
//{
//    u8 astemp[2 + 1];
//
//    memset(astemp, 0, sizeof(astemp));
//    if(ProviderGetTagValue(TAG_SCIRTP_REVERSAL, astemp))
//    {
//        if(memcmp(astemp, "01", 2) == 0)
//        {
//            (*bisReversal) = true;
//        }
//        else
//        {
//            (*bisReversal) = false;
//        }
//    }
//
//    return SDK_OK;
//}

///*****************************************************************************
//** Descriptions:	读取冲正包
//** Parameters:    	SDK_8583_ST8583 *pstReversalBag
//** Returned value:
//** Created By:		huacong  2016.03.10
//** Remarks:
//*****************************************************************************/
//s32 WriteReversalFlag(bool bisReversal)
//{
//    u8 astemp[2 + 1];
//
//    memset(astemp, 0, sizeof(astemp));
//    if(bisReversal)
//    {
//        strcpy(astemp, "01");
//    }
//    else
//    {
//        strcpy(astemp, "00");
//    }
//
//    if(ProviderSetTagValue(TAG_TRADE_REVERSAL, astemp, 2) != 0)
//    {
//        return SDK_ERR;
//    }
//}

///*****************************************************************************
//** Descriptions:	读冲正标志
//** Parameters:    	bool bisReversal
//** Returned value:
//** Created By:		huacong  2016.03.10
//** Remarks:
//*****************************************************************************/
//s32 ReadReversalFlag(bool *bisReversal)
//{
//    u8 astemp[2 + 1];
//
//    memset(astemp, 0, sizeof(astemp));
//    if(ProviderGetTagValue(TAG_TRADE_REVERSAL, astemp))
//    {
//        if(memcmp(astemp, "01", 2) == 0)
//        {
//            (*bisReversal) = true;
//        }
//        else
//        {
//            (*bisReversal) = false;
//        }
//    }
//
//    return SDK_OK;
//}
//

/*****************************************************************************
 ** Descriptions:    发送脚本包
 ** Parameters:          void
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:
 *****************************************************************************/
s32 SendScriptBag(SDK_8583_ST8583 *pstRevBag8583,
		struct _TradeData *stTradeData) {
	s32 num;
	s32 rslt;
	u8 temp[255] = { 0 };
	u16 len = 0;
	s32 i, j;
	bool bIsOk = false;
	u8 origdata[64] = { 0 };
	//  struct _RECORDDATA stRecordData;
	SDK_8583_ST8583 pstBag8583;
	memset(&pstBag8583, 0, sizeof(SDK_8583_ST8583));
	//   pstBag8583 = &gstSendBag;
	/*
	 num = GetRecordIndexNum(SCRIPT_INDEX);
	 if (num == 0)
	 {
	 return SDK_OK;
	 }
	 */

	InitFormSt8583(&pstBag8583, stTradeData);

	SetFieldData(&pstBag8583, SDK_8583_FIELD_MSG, "0620", 4); //消息类型
	CopyBag8583Field(&pstBag8583, 2, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 3, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 4, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 11, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 22, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 23, pstRevBag8583);
	CopyBag8583Field(&pstBag8583, 38, pstRevBag8583);
	SetPublicField041(&pstBag8583, stTradeData);
	SetPublicField042(&pstBag8583, stTradeData);
	CopyBag8583Field(&pstBag8583, 49, pstRevBag8583);

	//55域
	//55域
	if (sdk8583IsDomainExist(pstRevBag8583, 55)) {
		memset(temp, 0, sizeof(temp));
		len = 255;

		//需要知道原交易类型
#if 0 /*Modify by huacong at 2016.03.11  9:7 */
		if (IsMobileTypeTrade(stRecordData.stSendData.ucTradeId))
		{
			CopySendField(pstBag8583, 55);
		}
		else
#endif /* if 0 */
		{
			bIsOk = ZipPag55Data(pstRevBag8583, REVERSAL_MSG, temp, &len,
					stTradeData);
			if (!bIsOk)               //组织脚本上送55域内容
			{
				return SDK_ESC;
			}
			SetFieldData(&pstBag8583, 55, temp, len);               //设置55域
		}
	}

	//60域
	memset(temp, 0, sizeof(temp));
	len = GetFieldData(pstRevBag8583, 60, temp, sizeof(temp));
	if (len)           //发送包60域
	{
		memcpy(temp, "00", 2);
		memcpy(&temp[2], gstSys.asBatchNo, 6);           //批次号
		memcpy(&temp[8], "951", 3);
		if (len > 13) {
			len = 13;   //脚本不上送60.6
		}
		SetFieldData(&pstBag8583, 60, temp, len);
	}
	//61域
	//从原交易接收包中取出批次号
	memset(temp, 0, sizeof(temp));
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(pstRevBag8583, 60, origdata, sizeof(origdata)) > 0) {
		strncat(temp, &origdata[2], 6);
	} else {
		strncat(temp, "000000", 6);
	}

	//从原交易接收包中取出流水号
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(pstRevBag8583, 11, origdata, sizeof(origdata)) > 0) {
		strncat(temp, origdata, 6);
	} else {
		strncat(temp, "000000", 6);
	}

	//从原交易接收包中取出日期
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(pstRevBag8583, 13, origdata, sizeof(origdata)) > 0) {
		strncat(temp, origdata, 4);
	} else {
		strncat(temp, "000000", 4);
	}
	SetFieldData(&pstBag8583, 61, temp, strlen(temp));

//64域
	SetPublicField064(&pstBag8583, NULL);
	rslt = EncryptSendData(stTradeData, &pstBag8583);
	if (rslt == SDK_ESC) {
		return SDK_ESC;
	}
	rslt = WriteScriptBag(&pstBag8583);
	if (rslt == SDK_ESC) {
		return SDK_ESC;
	}
	WriteScriptlFlag(true);
	//zxx 20131021 9:48
	//zxx 20131021 9:48 脚本重发上限后要有提示并删除脚本
	//删除脚本
	//  StlDelScript(0);

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:   覆盖原来的数据
 ** Parameters:
 ** Returned value:
 ** Created By:	   wangb  2016.03.31
 ** Remarks:
 *****************************************************************************/
int replacerawdata(char *data, int datalen, int* maxlen, char * oldkeyend,
		int oldlen, char *newval, int newlen) {
	if (data && datalen > 0 && maxlen && *maxlen > 0 && oldkeyend && oldlen >= 0
			&& newlen >= 0)
		;
	{
		char *oldval = 0;
		char *ptmp = 0;
		int tmplen = 0;
		oldval = oldkeyend + 1;
		memmove(oldval + newlen, oldval + oldlen,
				datalen - (oldval - data) - oldlen);
		if (newlen > 0)
			memcpy(oldval, newval, newlen);
		*maxlen = datalen + newlen - oldlen;
	}
	return 0;
}
/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	查找16进制数组
 ** Returned value:
 ** Created By:	   wangb  2016.03.31
 ** Remarks:
 *****************************************************************************/
char *my_strhex(const char *str, u32 srclen, const char *sub_str, u32 len) {
	int i = 0;
	for (i = 0; i < srclen; i++) {
		int tem = i; //tem保留主串中的起始判断下标位置
		int j = 0;
		while (str[i++] == sub_str[j++]) {
			if (sub_str[j] == '\0' || j >= len) {
				return &str[tem];
			}
		}
		i = tem;
	}

	return 0;
}
//char *strncpyHex(char *dest, const char *src, size_t count)
//{
//    char *tmp = dest;
//    while (count) {
//        *tmp = *src;
//         src++;
//         tmp++;
//        count--;
//    }
//    return dest;
//}
/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	替换55域数据
 ** Returned value:
 ** Created By:	   wangb  2016.03.31
 ** Remarks:
 *****************************************************************************/
s32 Replace55bag(SDK_8583_ST8583 *send8583Bag, SDK_8583_ST8583 *recv8583Bag) {
	s32 slt;
	u32 recvlen = 0, buflen = 0, sendbuflen, maxlen = 512;
	u16 pTag, pTagSize = 2;
	u8 buf[128] = { 0 };
	u8 temp[512] = { 0 };
	u8 i = 0, k = 0;
	u8 *keyinoldfile = 0;
	struct _Field55DoLTab stdoltab;
	TagList recvTagList;
	TagList sendTagList;
	InitTagList(&recvTagList);
	InitTagList(&sendTagList);

	if ((recvlen = GetFieldData(recv8583Bag, 55, temp, 512))) {
		BuildTagListOneLevel(temp, recvlen, &recvTagList);
	}
//    if((catlen = ProviderGetTagValue(TAG_8583_Field55Type, recvbuf)))
//    {
//        BuildTagListOneLevel(recvbuf, catlen, &recvTagList);
//    }
	memset(temp, 0, 512);
	if ((recvlen = GetFieldData(send8583Bag, 55, temp, 512))) {
		BuildTagListOneLevel(temp, recvlen, &sendTagList);
	}
	i = sizeof(set55Tag) / sizeof(struct _Field55DoLTab);
	for (k = 0; k < i; k++) {
		memset(buf, 0, sizeof(buf));
		memset(&stdoltab, 0, sizeof(struct _Field55DoLTab));
		memcpy(&stdoltab, &set55Tag[k], sizeof(struct _Field55DoLTab));
		if ((slt = ParseTlvPag(stdoltab.heTag, 3, &pTag, &pTagSize))) {
			if ((buflen = GetTagValueSize(&recvTagList, pTag))) {
//                Log("wangb","stdoltab.heTag is %x ",pTag);
				memcpy(buf, GetTagValue(&recvTagList, pTag), buflen);
				keyinoldfile = my_strhex(temp, recvlen, stdoltab.heTag, 2);
				if (keyinoldfile) {
					if ((slt = ParseTlvPag(stdoltab.heTag, 3, &pTag, &pTagSize))) {
						if ((sendbuflen = GetTagValueSize(&sendTagList, pTag))) {
							replacerawdata(temp, recvlen, &maxlen,
									keyinoldfile + 2, sendbuflen, buf, buflen); //注意 目前所以的判断都是两个字节的TAG +2，一个字节请在数组中判断+1
							recvlen = maxlen;
						}
					}
				} else if (buflen > 0) {
//                      Log("wangb","stdoltab.heTag is %x ",pTag);
					memcpy(temp + recvlen, stdoltab.heTag, 2);
					recvlen = recvlen + 2;
					temp[recvlen++] = buflen;
					memcpy(temp + recvlen, buf, buflen);
					recvlen = recvlen + buflen;
				}
			}
		}

	}
	if (ProviderSetTagValue(TAG_8583_EMV55Type, temp, recvlen) != 1)
		return SDK_ERR;
	return SDK_OK;
//    i = sizeof(stReversalTab) / sizeof(struct _Field55DoLTab);
//    for (k = 0; k < i; k++)
//    {
//     memset(buf, 0, sizeof(buf));
//     buflen = sizeof(buf);
//     memcpy(&stdoltab, &stReversalTab[k], sizeof(struct _Field55DoLTab));
//     if ((stdoltab.heTag[0] & 0x1F) == 0x1F)
//     {
//        memcpy(&dol[dollen], stdoltab.heTag, 2);
//        dollen += 2;
//     }
//     else
//     {
//        dol[dollen++] = stdoltab.heTag[0];
//     }
//
//     if((slt = ParseTlvPag(stdoltab.heTag, 3, &pTag,&pTagSize))) {
//            if ((buflen = GetTagValueSize(&sendTagList, pTag))) {
//            memcpy(buf, GetTagValue(&sendTagList, pTag), buflen);
//        }
//     }
//
//     if ((slt == SDK_OK) && (buflen > 0))                  //
//     {
//        dol[dollen++] = buflen;
//        memcpy(&dol[dollen], buf, buflen);
//        dollen += buflen;
//    }
//    else
//    {
//        if (stdoltab.bisM)
//        {
//            dol[dollen++] = 0;
//        }
//     }
//    }
//    Log("wangb","dollen is %d ",dollen);
//    if(ProviderSetTagValue(TAG_8583_Field55Type, dol, dollen) != 1)
//     return SDK_ERR;
//    return SDK_OK;

}
/*****************************************************************************
 ** Descriptions:	写冲正包
 ** Parameters:
 ** Returned value:	修改 wangb 2016 0331
 ** Created By:		huacong  2016.03.11
 ** Remarks:
 *****************************************************************************/
s32 SaveReversalBag(bool bisSendBag, SDK_8583_ST8583 *pst8583Bag,
		struct _TradeData *pstTradeData) {

	Log("zbq","jump into SaveReversalBag");
//该冲正交易不能改变gstTradeData这个全局的值，实际上也用不到
	u8 temp[512] = { 0 }, buf[64] = { 0 };
	s32 slt;
	u16 len = 0;
	s32 times = 0;
	u32 uiTotalTimes;
	u8 response[3] = { 0 }; //?????
	u8 res[3] = { 0 };
	bool bIsOk = false;
	SDK_8583_ST8583 stReversalBag;
	u8 origdata[128] = { 0 };

	if (pst8583Bag == NULL || pstTradeData == NULL) {
		return SDK_ESC;
	}

	memset(&stReversalBag, 0, sizeof(SDK_8583_ST8583));

	memset(response, 0, sizeof(response));
	if (bisSendBag) {
		Log("zbq","copy Field start");
		InitFormSt8583(&stReversalBag, pstTradeData);

		SetFieldData(&stReversalBag, SDK_8583_FIELD_MSG, "0400", 4); //???????

		//和原来发送包一致
		CopyBag8583Field(&stReversalBag, 2, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 3, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 4, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 11, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 14, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 22, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 23, pst8583Bag);
		CopyBag8583Field(&stReversalBag, 25, pst8583Bag);

		CopyBag8583Field(&stReversalBag, 38, pst8583Bag);

		SetPublicField041(&stReversalBag, pstTradeData);
		SetPublicField042(&stReversalBag, pstTradeData);
		CopyBag8583Field(&stReversalBag, 49, pst8583Bag);

		//55域
		if (sdk8583IsDomainExist(pst8583Bag, 55)) {
			memset(temp, 0, sizeof(temp));
			len = 255;

			//???????????????
#if 0 /*Modify by huacong at 2016.03.11  9:7 */
			if (IsMobileTypeTrade(stRecordData.stSendData.ucTradeId))
			{
				CopySendField(pstBag8583, 55);
			}
			else
#endif /* if 0 */
			{
				//xiaox 有55域则写入 emvtag 缓存链表

				if (gstEmvTempTagList.ItemCount > 0) {
					FreeTagList(&gstEmvTempTagList);
				}
				memset(temp, 0, sizeof(temp));
				InitTagList(&gstEmvTempTagList);
				len = GetFieldData(pst8583Bag, 55, temp, sizeof(temp));
				BuildTagListOneLevel(temp, len, &gstEmvTempTagList);
				TraceHex("xiaox","55field",temp,len);

				//xiaox 这种冲正不上送9f36，只有联机拒绝才要
				removeTag(&gstEmvTempTagList, 0x9f36);
				bIsOk = ZipPag55Data(&stReversalBag, REVERSAL_MSG, temp, &len,
						pstTradeData);
				if (!bIsOk)                //组织脚本上送55域内容
				{
					return SDK_ESC;
				}
				SetFieldData(&stReversalBag, 55, temp, len);          //????55??
			}
		}

		CopyBag8583Field(&stReversalBag, 60, pst8583Bag);

		//先把原交易发送包复制过来
		CopyBag8583Field(&stReversalBag, 61, pst8583Bag);

		memset(temp, 0, sizeof(temp));
		memset(buf, 0, sizeof(buf));
		GetFieldData(pst8583Bag, SDK_8583_FIELD_MSG, temp, 4);
		GetFieldData(pst8583Bag, 3, buf, 6);

		if (((!memcmp(temp, "02", 2)) && (!memcmp(buf, "00", 2)))
				|| ((!memcmp(temp, "01", 2)) && (!memcmp(buf, "03", 2)))) {
			sdk8583ClrField(&stReversalBag, 62);
		} else {
			CopyBag8583Field(&stReversalBag, 62, pst8583Bag);
		}

	} else {
		//先读取之前存储的冲正包
		slt = ReadReversalBag(&stReversalBag);
		if (slt == SDK_ESC) {
			return SDK_ESC;
		}

		//39域需要重置，要知道接收包的状态
		SetResversal39Field(pstTradeData->netstate, response);  //  Fixme 网络状态判断
		SetFieldData(&stReversalBag, 39, response, 2);

		//61域用请求的交易
		memset(temp, 0, sizeof(temp));
		if (sdk8583IsDomainExist(&stReversalBag, 61)) {
			sdk8583ClrField(&stReversalBag, 61);

			memset(temp, 0, sizeof(temp));
			memset(origdata, 0, sizeof(origdata));
			if (GetFieldData(pst8583Bag, 60, origdata, sizeof(origdata)) > 0) {
				strncat(temp, &origdata[2], 6);
			} else {
				if (GetFieldData(&stReversalBag, 60, origdata, sizeof(origdata))
						> 0) {
					strncat(temp, &origdata[2], 6);
				} else {
					strncat(temp, "000000", 6);
				}
			}

			memset(origdata, 0, sizeof(origdata));
			if (GetFieldData(pst8583Bag, 11, origdata, sizeof(origdata)) > 0) {
				strncat(temp, origdata, 6);
			} else {
				if (GetFieldData(&stReversalBag, 11, origdata, sizeof(origdata))
						> 0) {
					strncat(temp, origdata, 6);
				} else {
					strncat(temp, "000000", 6);
				}
			}
           if ((pstTradeData->ucTradeId == TRADEID_UNPREAUTH) || (pstTradeData->ucTradeId == TRADEID_COMPLETE) || (pstTradeData->ucTradeId == TRADEID_UNCOMPLETE)
                      || (pstTradeData->ucTradeId == TRADEID_MOTOUNPREAUTH) || (pstTradeData->ucTradeId == TRADEID_MOTOCOMPLETE) || (pstTradeData->ucTradeId == TRADEID_MOTOUNCOMPLETE)
                      || (pstTradeData->ucTradeId == TRADEID_MOBILEUNPREAUTH) || (pstTradeData->ucTradeId == TRADEID_MOBILECOMPLETE) || (pstTradeData->ucTradeId == TRADEID_MOBILEUNCOMPLETE)
                      || pstTradeData->ucTradeId == TRADEID_TIPS || pstTradeData->ucTradeId == TRADEID_ADJUST || pstTradeData->ucTradeId == TRADEID_OFFSETTLE) //zxx 20130723 14:23加上小费和调整和离线结算
              {
                  //从原交易接收包中取出交易日期
                  memset(origdata, 0, sizeof(origdata));
                  if (GetFieldData(&stReversalBag, 13, origdata, sizeof(origdata)) > 0)
                  {
//                      Trace("lilin","取到接收包\r\n");
                      strncat(temp, origdata, 4);
                  }
                  else
                  {
                      if (GetFieldData(pst8583Bag, 13, origdata, sizeof(origdata)) > 0)
                      {
                          strncat(temp, origdata, 4);
                      }
                      else
                      {
                          strncat(temp, "0000", 4);
                      }
                  }
              }
			SetFieldData(&stReversalBag, 61, temp, strlen(temp));
		}
		if (sdk8583IsDomainExist(pst8583Bag, 55)) {
			memset(temp, 0, sizeof(temp));
			len = 255;

			SDK_8583_ST8583 pstSendBag8583;
			memset(&pstSendBag8583, 0, sizeof(SDK_8583_ST8583));
			bIsOk = ReadSendBag(&pstSendBag8583);
			if (!bIsOk) {
//                Log("wangb", "ReadSendBag is %d", nRet);
				return SDK_ESC;
			}
			bIsOk = ZipPag55Data(&stReversalBag, REVERSAL_MSG, temp, &len,
					pstTradeData);
			if (!bIsOk)                //解析55域失败
			{
				return SDK_ESC;
			}
			SetFieldData(&stReversalBag, 55, temp, len);         //设置55域
		}
	}
	SetPublicField064(&stReversalBag, NULL);
	slt = EncryptSendData(pstTradeData, &stReversalBag);
	if (slt == SDK_ESC) {
		return SDK_ESC;
	}

	slt = WriteReversalBag(&stReversalBag);
	if (slt == SDK_ESC) {
		return SDK_ESC;
	}

	WriteReversalFlag(true);

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	处理脚本
 ** Parameters:    	struct _TradeData *pstTradeData
 ** Returned value:
 ** Created By:		wangb  2016.03.21
 ** Remarks:
 *****************************************************************************/
s32 DealSendReverScriptNew(struct _TradeData *pstTradeData) {
	s32 slt;
	u8 astemp[8] = { 0 };
	SDK_8583_ST8583 stReversalBag;
	//读取冲脚本
	if (ProviderSetTagValue(TAG_SCIRTP_SCRIPT, astemp, 1) != 1) {
		pstTradeData->uiCurrTimes = 0;
	} else {
		pstTradeData->uiCurrTimes = atoi(astemp);
	}
	memset(&stReversalBag, 0, sizeof(SDK_8583_ST8583));
	slt = ReadReverScriptBag(&stReversalBag);                 //Fixme 脚本流水号加1
	if (slt == SDK_ERR) {
		return SDK_ESC;
	}
	SetFieldData(&stReversalBag, 11, gstSys.asVoucherNo, 6);   //添加流水号 Fixme  存储
	if (pstTradeData->uiScriptTimes < 3) {
		//加密后直接发送
		SetPublicField064(&stReversalBag, NULL);
		slt = EncryptSendData(pstTradeData, &stReversalBag);
		if (slt == SDK_ESC) {
			return SDK_ESC;
		}

#if 1 /*Modify by huacong at 2016.03.11  10:45 */
		//写发送脚本包
		//重新改写报文头
	ReConfigBag8583(&stReversalBag);

	#ifdef EncriptBagControl
		slt = WriteSendBag(TAG_8583_SENDBAG,&stReversalBag);
		//全报文加密
		EncryptAllMsgByHX(1, stReversalBag.ucBagData, &stReversalBag.nBagLen);
	#endif

	#ifdef EncriptBagControl
		slt = WriteSendBag(TAG_8583_SENDECRIPTBAG,&stReversalBag);
		if(slt != SDK_OK){
			return SDK_ESC;
		}
	#else
		slt = WriteSendBag(TAG_8583_SENDBAG,&stReversalBag);
		if(slt != SDK_OK){
			return SDK_ESC;
		}
	#endif

//		if (WriteSendScript(&stReversalBag) != SDK_OK) {
//			return SDK_ESC;
//		}
#endif /* if 0 */

		//写成功之后，更新脚本次数
		pstTradeData->uiScriptTimes++;
		if (WriteTradeTimes(pstTradeData->uiScriptTimes) != SDK_OK) {
			return SDK_ESC;
		}
		return SDK_EQU;
	} else {
		//脚本次数到，请除脚本
		if (WriteScriptlFlag(false) != SDK_OK) {
			return SDK_ESC;
		}
		if (WriteScriptTimes(0) != SDK_OK) {
			return SDK_ESC;
		}
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	处理冲正
 ** Parameters:    	struct _TradeData *pstTradeData
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 DealSendReversalBagNew(struct _TradeData *pstTradeData) {
	s32 slt;
	u8 astemp[8] = { 0 };
	SDK_8583_ST8583 stReversalBag;
	//读取冲正包
	memset(&stReversalBag, 0, sizeof(SDK_8583_ST8583));
	slt = ReadReversalBag(&stReversalBag);
	if (ProviderGetTagValue(TAG_TRADE_TIME, astemp) != 1) {
		pstTradeData->uiCurrTimes = 0;
	} else {
		pstTradeData->uiCurrTimes = atoi(astemp);
	}

	if (pstTradeData->uiCurrTimes < 3) {
		//加密后直接发送
		SetPublicField064(&stReversalBag, NULL);
		slt = EncryptSendData(pstTradeData, &stReversalBag);
		if (slt == SDK_ESC) {
			return SDK_ESC;
		}

#if 1 /*Modify by huacong at 2016.03.11  10:45 */
		//写发送冲正包

		//重新改写报文头
		ReConfigBag8583(&stReversalBag);

	#ifdef EncriptBagControl
		slt = WriteSendBag(TAG_8583_SENDBAG,&stReversalBag);
		//全报文加密
		EncryptAllMsgByHX(1, stReversalBag.ucBagData, &stReversalBag.nBagLen);
	#endif

	#ifdef EncriptBagControl
		slt = WriteSendBag(TAG_8583_SENDECRIPTBAG,&stReversalBag);
		if(slt != SDK_OK){
			return SDK_ESC;
		}
	#else
		slt = WriteSendBag(TAG_8583_SENDBAG,&stReversalBag);
		if(slt != SDK_OK){
			return SDK_ESC;
		}
	#endif

//		if (WriteSendBag(TAG_8583_SENDBAG,&stReversalBag) != SDK_OK) {
//			return SDK_ESC;
//		}
#endif /* if 0 */

		//写成功之后，更新冲正次数
		pstTradeData->uiCurrTimes++;
		if (WriteTradeTimes(pstTradeData->uiCurrTimes) != SDK_OK) {
			return SDK_ESC;
		}
		return SDK_EQU;

	} else {
		//冲正次数到，请除冲正
		if (WriteReversalFlag(false) != SDK_OK) {
			return SDK_ESC;
		}
		if (WriteTradeTimes(0) != SDK_OK) {
			return SDK_ESC;
		}
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	冲正脚本通讯状态处理
 ** Parameters:    	struct _TradeData *pstTradeData
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 修改 wangb 2016 04.06
 ** Remarks:
 *****************************************************************************/
s32 DealReversalBag(SDK_8583_ST8583 *pstRevBag, SDK_8583_ST8583 *pstSendBag,
		struct _TradeData *pstTradeData, s32 *pNetstate) {
	s32 slt;
	s32 sendtype = SDK_EQU;         //发送交易类型是否是冲正或者脚本 1 冲正2 脚本
	s32 recvtype = SDK_EQU;         //接收交易类型是否是冲正或者脚本 1 冲正2 脚本
	u8 response[3] = { 0 }; //
	u8 res[3] = { 0 };
	slt = DealParseFieldhead(pstRevBag, pstSendBag, pNetstate, &sendtype,
			&recvtype);     //判断网络状态和包类型
	if (slt != SDK_OK) {
		if (*pNetstate == NET_INTERRUT)                             //通信中断直接返回错误
		{
			*pNetstate = SDK_ERR;
			return SDK_ERR;
		} else if (*pNetstate == NET_TIMEOUT) {
			if (sendtype == TYRDE_REVERRSAL)                        //冲正包超时修改39域
			{
				sdk8583ClrField(pstSendBag, 39);
				SetFieldData(pstSendBag, 39, "98", 2);
				*pNetstate = SDK_LOD;                                     //继续冲正
				if (WriteReversalBag(pstSendBag) != SDK_OK) {
					return SDK_ERR;
				}
			} else {
				*pNetstate = SDK_ERR;                             //脚本包或者正常包返回错误
			}
		} else if (*pNetstate == NET_UNCONNECT)             //正常包网络未连接
		{
			*pNetstate = SDK_ERR;
			if (sendtype != TYRDE_REVERRSAL && sendtype != TYRDE_SCRIPTL) //判断是否是正常包
			{
				if (WriteReversalFlag(false) == SDK_ERR)     //     去掉冲正标志
				{
					return SDK_ERR;
				}
			}
		}
		return slt;
	} else if (*pNetstate != SDK_LOD) {
		return SDK_OK;                                //正常包直接返回
	}
	slt = CheckRecBag(pstSendBag, pstRevBag);             //冲正包及脚本包验证
	if (slt == SDK_OK) {
		if (recvtype == TYRDE_REVERRSAL)                //判断接收包是否是冲正包
		{
			slt = WriteReversalFlag(false);                //     去掉冲正标志
		} else if (recvtype == TYRDE_SCRIPTL)                   //判断接收包是否是脚本包
		{
			slt = WriteScriptlFlag(false);                        //     去掉冲正标志
		}
		if (slt != SDK_ERR) {
			return SDK_OK;
		}
	} else {
		//判断是否是冲正包
		if (sendtype == TYRDE_REVERRSAL)            //冲正包处理
		{
			if (slt == ERR_RESPONSE) {
				SetFieldData(pstSendBag, 39, "06", 2);
				if (GetFieldData(pstRevBag, 39, res, 2)) //
						{
					if (!memcmp(res, "A0", 2)) {
						memcpy(response, "A0", 2);
						SetFieldData(pstSendBag, 39, "06", 2);
						return SDK_ERR;
					}
				}
			} else {
				SetFieldData(pstSendBag, 39, "06", 2);
			}

			if (WriteReversalBag(pstSendBag) != SDK_OK) {
				return SDK_ERR;
			}
		}

		return SDK_ERR;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	提前包检查
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		wangb  2016.03.21
 ** Remarks:           //判断网络状态和包类型
 *****************************************************************************/
s32 DealParseFieldhead(SDK_8583_ST8583 *pstRevBag8583,
		SDK_8583_ST8583 *pstSendBag8583, s32 *pNetstate, s32 *sendtype,
		s32 *recvtype) {
	u8 revbuf[16] = { 0 };
	s32 nRet = 0;
	//  s32 nsend = 0;

	nRet = GetFieldData(pstSendBag8583, SDK_8583_FIELD_MSG, revbuf, 16); //读取发送包判断是否是脚本或者冲正
	if (nRet <= 0) {
		return SDK_ERR;
	}
	if (strstr(revbuf, "0400"))               //判断是否是冲正
			{
		*sendtype = TYRDE_REVERRSAL;
	} else if (strstr(revbuf, "0620"))     //判断是否是脚本
			{
		*sendtype = TYRDE_SCRIPTL;
	}
	memset(revbuf, 0, 16);
	//nRet = ParseField8583(pstRevBag8583);
	if (*pNetstate == NET_TIMEOUT || *pNetstate == NET_INTERRUT
			|| *pNetstate == NET_UNCONNECT)   //判断网络是否正常
	{
		return SDK_ERR;
	} else {
		nRet = GetFieldData(pstRevBag8583, SDK_8583_FIELD_MSG, revbuf, 16);
		if (nRet <= 0) {
			if (*sendtype == 1) {
				*pNetstate = SDK_LOD;
			}
			return SDK_ERR;
		}
		if (strstr(revbuf, "0410"))          //判断接收包是否为冲正包
				{
			*recvtype = TYRDE_REVERRSAL;
		} else if (strstr(revbuf, "0630"))         //判断接收包是否为脚本
				{
			*recvtype = TYRDE_SCRIPTL;
		}
		if (*sendtype == TYRDE_REVERRSAL)            //如果发送包为冲正
		{
			if (*recvtype == TYRDE_REVERRSAL)         //如果接收包为冲正
			{
				*pNetstate = SDK_LOD;                //继续发包
			} else {
				*pNetstate = SDK_LOD;                    //继续发包
				return SDK_ERR;
			}
		}
	}
	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	提前包检查
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 DealSendBagAhead(struct _TradeData *pstTradeData) {
	s32 rslt = 0;

	if (pstTradeData == NULL) {
		return SDK_ERR;
	}
	//先检查capk和aid，要增加一个标签表示是否支持ic卡，还有ic卡参数是否已经下载

	//脚本是否有上送

	if (pstTradeData->bisHaveReverScriptBag) {
		rslt = DealSendReverScriptNew(pstTradeData);
		return rslt;
	}
	//再检查是否有冲正
	if (pstTradeData->bisHaveReversalBag) {
		rslt = DealSendReversalBagNew(pstTradeData);
		return rslt;
	}

	return SDK_OK;
}

/*****************************************************************************
 ** Descriptions:	????????
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
s32 DealRevBagAhead(struct _TradeData *pstTradeData) {
	s32 rslt = 0;
	if (pstTradeData->bisHaveReversalBag) {
//        rslt = DealReversalBag(pstTradeData);
		if (rslt != SDK_EQU) {
			if (pstTradeData->uiCurrTimes < pstTradeData->uiReversalTimes) {
				pstTradeData->uiCurrTimes++;
				WriteTradeTimes(pstTradeData->uiCurrTimes);
			} else {
				WriteReversalFlag(false);
				return SDK_EQU;
			}
		}
		return SDK_EQU;
	}

	return SDK_OK;
}
/*****************************************************************************
 ** Descriptions:	解析61域
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		huacong  2016.03.10
 ** Remarks:
 *****************************************************************************/
void unsaleField61Origdata(u8 ucTradeId, u8 *pasOrigdata,
		SDK_8583_ST8583 *pstSendBag, SDK_8583_ST8583 *pstRevBag)     //Fixme
{
	u8 temp[512] = { 0 };
	u8 origdata[128] = { 0 };
#if 1 /*Modify by huacong at 2016.03.04  10:44 */
	//61锟斤拷
	//锟斤拷原锟斤拷锟阶斤拷锟秸帮拷锟斤拷取锟斤拷锟斤拷锟轿猴拷
	memset(temp, 0, sizeof(temp));
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(pstRevBag, 60, origdata, sizeof(origdata)) > 0) {
		strncat(temp, &origdata[2], 6);
	} else {
		if (GetFieldData(pstSendBag, 60, origdata, sizeof(origdata)) > 0) {
			strncat(temp, &origdata[2], 6);
		} else {
			strncat(temp, "000000", 6);
		}
	}

	//获取原流水号
	memset(origdata, 0, sizeof(origdata));
	if (GetFieldData(pstRevBag, 11, origdata, sizeof(origdata)) > 0) {
		strncat(temp, origdata, 6);
	} else {
		if (GetFieldData(pstSendBag, 11, origdata, sizeof(origdata)) > 0) {
			strncat(temp, origdata, 6);
		} else {
			strncat(temp, "000000", 6);
		}
	}
	if ((ucTradeId == TRADEID_UNPREAUTH) || (ucTradeId == TRADEID_COMPLETE)
			|| (ucTradeId == TRADEID_UNCOMPLETE)
			|| (ucTradeId == TRADEID_MOTOUNPREAUTH)
			|| (ucTradeId == TRADEID_MOTOCOMPLETE)
			|| (ucTradeId == TRADEID_MOTOUNCOMPLETE)
			|| (ucTradeId == TRADEID_MOBILEUNPREAUTH)
			|| (ucTradeId == TRADEID_MOBILECOMPLETE)
			|| (ucTradeId == TRADEID_MOBILEUNCOMPLETE)
			|| ucTradeId == TRADEID_TIPS || ucTradeId == TRADEID_ADJUST
			|| ucTradeId == TRADEID_OFFSETTLE) {
		//锟获取原交易日期
		memset(origdata, 0, sizeof(origdata));
		if (GetFieldData(pstRevBag, 13, origdata, sizeof(origdata)) > 0) {
			strncat(temp, origdata, 4);
		} else {
			if (GetFieldData(pstSendBag, 13, origdata, sizeof(origdata)) > 0) {
				strncat(temp, origdata, 4);
			} else {
				strncat(temp, "0000", 4);
			}
		}
	}
	strcpy(pasOrigdata, temp);
#endif /* if 0 */
}

/*****************************************************************************
   函 数 名  : DispResponseError
   功能描述  : 根据报文39域返回错误代码显示错误提示信息
   输入参数  : void
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年12月17日
    作    者   : wuzhonglin
    修改内容   : 新生成函数

*****************************************************************************/
void GetStrResponseError(u8 *pasOut,SDK_8583_ST8583 *pstRevBag)
{
    s32 len  = 3;
    u16 i = 0;
    u8 res[3] = {0};
    s32 resperrornum;

    memset(res, 0, sizeof(res));
    len = GetFieldData(pstRevBag, 39, res, 3);
    resperrornum = sizeof(RespTab) / sizeof(struct _RespCodeTab);
    if (len)
    {
        for (i = 0; i < resperrornum; i++)
        {
            if (!memcmp(res, RespTab[i].RespCode, 2))
            {
                break;
            }
        }

        if (i < resperrornum)
        {
            strcpy(pasOut, RespTab[i].ShowInfo);
        }
        else
        {
            strcpy(pasOut, "交易失败");
        }

//        if (memcmp(res, "00", 2))
//        {
//            strcat(pasOut, "\n响应码:");
//            strcat(pasOut, res);
//        }
    }
    else
    {
        strcpy(pasOut, "交易失败");
    }
}
/*****************************************************************************
   函 数 名  : DispResponseError
   功能描述  : 根据报文39域返回网络状态错误
   输入参数  : void
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2016.04.29
    作    者   : wangb
    修改内容   : 新生成函数

*****************************************************************************/
void GetTimeResponseError(u8 *pasOut,s32 res)
{
    u16 i = 0;
    s32 resperrornum;

    resperrornum = sizeof(ErrTimeTab) / sizeof(struct _RespTimeTab);

    for (i = 0; i < resperrornum; i++)
    {
        if(res == ErrTimeTab[i].Resptime)
        {
            break;
        }
    }

    if (i < resperrornum)
    {
        strcpy(pasOut, ErrTimeTab[i].ShowInfo);
    }
    else
    {
        strcpy(pasOut, "交易失败");
    }
}
/*****************************************************************************
 ** Descriptions:	组织索引数据
 ** Parameters:          const u8 ucTradeId  交易ID
 struct _RECORDINDEX *pstRecordIndex 索引结构体
 ** Returned value:
 ** Created By:		李琳2012.09.04
 ** Remarks:
 *****************************************************************************/
/*
 static void FormRecordIndex(const u8 ucTradeId, struct _RECORDINDEX *pstRecordIndex)
 {
 u8 temp[64] = {0};
 s32 i = 0;
 //    struct _RecordIndexUserData *pstuser = NULL;


 pstRecordIndex->ucTradeId = ucTradeId;

 if (!GetMainAcc(&gstRecvBag, temp))
 {
 GetMainAcc(&gstSendBag, temp);
 }

 //主账号
 pstRecordIndex->ucMainAccLen = strlen(temp);
 sdkAscToBcd(pstRecordIndex->bcMainAcc, temp, pstRecordIndex->ucMainAccLen);
 //金额
 memset(temp, 0, sizeof(temp));
 if (GetFieldData(&gstRecvBag, 4, temp, sizeof(temp)) > 0)
 {
 sdkAscToBcdR(pstRecordIndex->bcAmount, temp, 6);
 }
 else if (GetFieldData(&gstSendBag, 4, temp, sizeof(temp)) > 0)
 {
 sdkAscToBcdR(pstRecordIndex->bcAmount, temp, 6);
 }

 //发送包11域流水号
 memset(temp, 0, sizeof(temp));
 if (GetFieldData(&gstSendBag, 11, temp, sizeof(temp)) > 0)
 {
 i = 0;
 i = atoi(temp);
 if ((i < 1) || (i > 999999))
 {
 i = 1;
 }
 sprintf(temp, "%06d", i);
 sdkAscToBcd(pstRecordIndex->bcTraceNo, temp, 6);
 }

 //内外卡标志处理
 memset(temp, 0, sizeof(temp));
 if (GetFieldData(&gstRecvBag, 63, temp, sizeof(temp)) > 0)
 {
 #if(PROGRAM_TYPE == UMS_PROGRAM)
 if (memcmp(temp, "CUP", 3))
 #else
 if (!((!memcmp(temp, "CUP", 3)) ||                                       //锟叫癸拷锟斤拷锟斤拷
 (!memcmp(temp, "cup", 3)) ||
 (!memcmp(temp, "000", 3)) ||
 (!memcmp(temp, "\x00\x00\x00", 3))))
 #endif
 {
 pstRecordIndex->bIsForeignCard = true;
 }
 }
 else if (GetFieldData(&gstSendBag, 63, temp, sizeof(temp)) > 0)
 {
 #if(PROGRAM_TYPE == UMS_PROGRAM)
 if (memcmp(temp, "CUP", 3))
 #else
 if (!((!memcmp(temp, "CUP", 3)) ||                                        //中国银联
 (!memcmp(temp, "cup", 3)) ||
 (!memcmp(temp, "000", 3)) ||
 (!memcmp(temp, "\x00\x00\x00", 3))))
 #endif
 {
 pstRecordIndex->bIsForeignCard = true;
 }
 }

 if (ucTradeId == TRADEID_ADJUST || ucTradeId == TRADEID_OFFSALE         //离线调整,脱机消费
 || ucTradeId == TRADEID_OFFUNSALE || ucTradeId == TRADEID_OFFSETTLE  //脱机消费撤销,离线结算
 || ucTradeId == TRADEID_ECOFFSALE || ucTradeId == TRADEID_QPBOC//zxx 20130505 19:24加上电子现金
 || ucTradeId == TRADEID_TIPS//zxx 20130814 10:26加上小费
 )
 {
 pstRecordIndex->bIsOfflineTrade = true;
 }

 if (ucTradeId == TRADEID_TIPS)
 {
 //从原交易接收包60.1域判断是何种交易类型
 memset(temp, 0, sizeof(temp));
 GetFieldData(&gstRecvBag, 60, temp, sizeof(temp));
 if (memcmp(temp, "22", 2) == 0)
 {
 pstRecordIndex->bIsOrgSaleOrComplete = false;
 }
 else
 {
 pstRecordIndex->bIsOrgSaleOrComplete = true;
 }
 }

 if (sdk8583IsDomainExist(&gstSendBag, 55))
 {
 pstRecordIndex->ucCardMode = CARD_ICC;
 memset(temp, 0, sizeof(temp));

 if (GetFieldData(&gstSendBag, 23, temp, sizeof(temp)) > 0)
 {
 sdkAscToBcd(&pstRecordIndex->ucIccSN, &temp[1], 2);
 }

 if (ucTradeId == TRADEID_SALE || ucTradeId == TRADEID_PREAUTH)
 {
 memset(temp, 0, sizeof(temp));
 i = 64;

 if (sdkIccReadTLV("\x95", temp, &i) > 0)        //TVR
 {
 if ((temp[4] & 0x40) == 0x40)
 {
 pstRecordIndex->bIsArpcErr  = true;
 }
 }
 }
 }
 else
 {
 pstRecordIndex->ucCardMode = CARD_MAG;
 }
 if (sdk8583IsDomainExist(&gstSendBag, 61))
 {
 memset(temp, 0, sizeof(temp));
 if (GetFieldData(&gstSendBag, 61, temp, sizeof(temp)) > 0)
 {
 sdkAscToBcd(pstRecordIndex->ucOrgTraceNo, &temp[6], 6);
 }
 }
 //    if (pstRecordIndex->usUserLen > 0
 //            && pstRecordIndex->pUserVoid != NULL)
 //    {
 //        //锟皆讹拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
 //        pstuser = (struct _RecordIndexUserData *)pstRecordIndex->pUserVoid;
 //    }
 }

 /*****************************************************************************
 ** Descriptions:	锟斤拷锟斤拷锟斤拷锟斤拷
 ** Parameters:          const u8 ucTradeId 锟斤拷锟斤拷ID
 ** Returned value:
 ** Created By:		锟斤拷锟斤拷2012.09.04
 ** Remarks:         锟斤拷锟街帮拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟酵拷锟剿拷牛锟斤拷锟斤拷锟缴撅拷锟街帮拷锟斤拷锟斤拷锟?zhangjun 20130310
 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:	获得一磁道数据
 ** Parameters:          u8 *name
 SDK_ICC_CARDDATA *pstCardData
 ** Returned value:
 ** Created By:		lilin2012.08.15
 ** Remarks:
 *****************************************************************************/
/*
 static bool GetCardHolder(SDK_ICC_CARDDATA const *pstCardData, u8 *pasName)
 {
 u8 i = 0, j = 0, flg = 0, rlt = false;
 u8 tmp[128] = {0};
 u8 len = 0;

 len = pstCardData->ucTrack1Len;

 if (len)
 {
 for (i = 0, j = 0, flg = 0; i < len; i++)
 {
 if (pstCardData->asTrack1Data[i] == '^')
 {
 if (!flg)                //找到第一个'^',姓名开始
 {
 flg = 1;
 }
 else                    //找到第二个'^',姓名结束
 {
 rlt = true;
 flg = 0;
 break;
 }
 }
 else
 {
 if (flg)
 {
 tmp[j++] = pstCardData->asTrack1Data[i];
 }
 }
 }
 }

 if (rlt)
 {
 sdkTrim(tmp, SDK_BOTH);
 strcpy(pasName, tmp);
 }
 return rlt;
 }
 */
/*****************************************************************************
 ** Descriptions:        组织record数据
 ** Parameters:          struct _RECORDSENDDATA *pstRecordSendData
 SDK_ICC_CARDDATA *pstCardData
 ** Returned value:
 ** Created By:		lilin2012.08.15
 ** Remarks:
 *****************************************************************************/
/*
 static void FormRecordSendData(bool bReversal, struct _TradeData *stTradeData, struct _RECORDSENDDATA *pstRecordSendData)
 {
 u8 temp[128] = {0};
 s32 len = 128;
 u8 ucTradeId;
 //    struct _SendUserData *pstuser = NULL;

 ucTradeId = stTradeData->ucTradeId;

 GetRtcSys(); //获取系统时间

 pstRecordSendData->ucTradeId = ucTradeId;
 pstRecordSendData->usMan = gstSys.usLoginMan;             //签到操作员
 pstRecordSendData->bcTradeDate[0] = GetBcdCentury(gstSys.ucYear);
 pstRecordSendData->bcTradeDate[1] = gstSys.ucYear;
 pstRecordSendData->bcTradeDate[2] = gstSys.ucMonth;
 pstRecordSendData->bcTradeDate[3] = gstSys.ucDay;
 pstRecordSendData->bcTradeTime[0] = gstSys.ucHour;
 pstRecordSendData->bcTradeTime[1] = gstSys.ucMinute;
 pstRecordSendData->bcTradeTime[2] = gstSys.ucSecond;

 pstRecordSendData->bReversal = bReversal;

 if (GetCardHolder(&stTradeData->strCardInfo.stCardData, temp))        //获得一磁道
 {
 strcpy(pstRecordSendData->ucCardHolder, temp);
 }

 Trace("lilin","stTradeData->strCardInfo.stCardParam.ucCardMode=%d\n",stTradeData->strCardInfo.stCardParam.ucCardMode);

 //20141101_lhd 电子签名 磁条卡帐户充值 IC卡信息在第二次读卡之前存储好 不然会被二次读卡时清除掉EMV TAG值
 if (sdk8583IsDomainExist(&gstSendBag, 55)
 || ( (SDK_ICC_ICC == stTradeData->strCardInfo.stCardParam.ucCardMode) && (TRADEID_MAGLOAD_ACC != ucTradeId) ) )//20141101_lhd IC卡交易无55域时候也要存储相关信息
 {
 pstRecordSendData->ucIccTrade = true;             //是否IC

 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9F\x06", temp, &len) > 0)       //aid字节长度
 {
 if (len > 16)
 {
 len = 16;
 }
 pstRecordSendData->ucAIDLen = len;
 memcpy(pstRecordSendData->ucAID, temp, len);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x50", temp, &len) > 0)       //应用标签
 {
 if (len > 16)
 {
 len = 16;
 }
 pstRecordSendData->ucAppLabelLen = len;
 memcpy(pstRecordSendData->ucAppLabel, temp, len);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x12", temp, &len) > 0)       //应用首选
 {
 if (len > 16)
 {
 len = 16;
 }
 pstRecordSendData->ucAppNameLen = len;
 memcpy(pstRecordSendData->ucAppName, temp, len);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x95", temp, &len) > 0)        //TVR
 {
 memcpy(pstRecordSendData->ucTVR, temp, 5);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9B", temp, &len) > 0)     //TSI
 {
 memcpy(pstRecordSendData->ucTSI, temp, 2);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x36", temp, &len) > 0)      //ATC
 {
 memcpy(pstRecordSendData->ucATC, temp, 2);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x26", temp, &len) > 0)       //TC
 {
 memcpy(pstRecordSendData->ucTC, temp, 8);
 memcpy(pstRecordSendData->ucARQC, temp, 8);
 }

 memset(temp, 0, sizeof(temp));
 len = 128;

 if (pstRecordSendData->ucTradeId == TRADEID_OFFUNSALE)  //脱机消费撤销
 {
 if (GetFieldData(&gstSendBag, 63, temp, sizeof(temp)) > 0)       //获取发送包63域
 {
 memcpy(pstRecordSendData->ucIOCForOffSale, temp, 3);
 }
 }

 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x33", temp, &len) > 0)       //终端性能9F33 (3B)
 {
 memcpy(pstRecordSendData->ucTermCapab, temp, 3);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x37", temp, &len) > 0)      //不可预知数9f37(4B)
 {
 memcpy(pstRecordSendData->ucUnpredictNum, temp, 4);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x9f\x34", temp, &len) > 0)     //CVM 9F34 (3B)
 {
 memcpy(pstRecordSendData->ucCVM, temp, 3);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 if (sdkIccReadTLV("\x82", temp, &len) > 0)       //AIP TAG:82  (2B)
 {
 memcpy(pstRecordSendData->ucAIP, temp, 2);
 }
 memset(temp, 0, sizeof(temp));
 len = 128;

 memset(temp, 0, sizeof(temp));
 len = 128;

 //        if (ucTradeId == TRADEID_ECOFFSALE
 //                || ucTradeId == TRADEID_ECCACCNO
 //                || ucTradeId == TRADEID_ECCACC
 //                || ucTradeId == TRADEID_ECCASH)
 if (ucTradeId == TRADEID_ECOFFSALE)//zxx 20130712 11:28圈存这里读不到的
 {
 if (sdkIccReadTLV("\x9f\x79", temp, &len) > 0)       //电子现金余额:9F79(6B)
 {
 memcpy(pstRecordSendData->ucOfflineBalance, temp, 6);
 }
 }
 else if (ucTradeId == TRADEID_QPBOC)
 {
 if (sdkIccReadTLV("\x9f\x5d", temp, &len) > 0)        //脱机余额:9F5d(6B)
 {
 memcpy(pstRecordSendData->ucOfflineBalance, temp, 6);
 }
 }
 }

 #ifdef ELECSIGN
 if( (SDK_ICC_ICC == stTradeData->strCardInfo.stCardParam.ucCardMode) && (TRADEID_MAGLOAD_ACC == ucTradeId) )//20141101_lhd
 {
 pstRecordSendData->ucIccTrade = true;             //是否IC

 if(gstEsNeedUploadEmvParam.ucAIDLen > 0)
 {
 pstRecordSendData->ucAIDLen = gstEsNeedUploadEmvParam.ucAIDLen;
 memcpy(pstRecordSendData->ucAID, gstEsNeedUploadEmvParam.ucAID, gstEsNeedUploadEmvParam.ucAIDLen);
 }

 if(gstEsNeedUploadEmvParam.ucAppLabelLen > 0)
 {
 pstRecordSendData->ucAppLabelLen = gstEsNeedUploadEmvParam.ucAppLabelLen;
 memcpy(pstRecordSendData->ucAppLabel, gstEsNeedUploadEmvParam.ucAppLabel, gstEsNeedUploadEmvParam.ucAppLabelLen);
 }

 if(gstEsNeedUploadEmvParam.ucAppNameLen > 0)
 {
 pstRecordSendData->ucAppNameLen = gstEsNeedUploadEmvParam.ucAppNameLen;
 memcpy(pstRecordSendData->ucAppName, gstEsNeedUploadEmvParam.ucAppName, gstEsNeedUploadEmvParam.ucAppNameLen);
 }

 //            Trace("lhd_debug","ucAppLabelLen=%d\n",pstRecordSendData->ucAppLabelLen);
 //            Trace("lhd_debug","ucAppNameLen=%d\n",pstRecordSendData->ucAppNameLen);
 //            Trace("lhd_debug","ucAIDLen=%d\n",pstRecordSendData->ucAIDLen);
 }
 #endif


 //    if (pstRecordSendData->usUserLen > 0
 //            && pstRecordSendData->pUserVoid != NULL)
 //    {
 //        pstuser = (struct _SendUserData *)pstRecordSendData->pUserVoid;
 //        //自定义数据在下面添加...
 //    }
 }

 */
/*****************************************************************************
 ** Descriptions:	保存发送包
 ** Parameters:          const u8 ucTradeId交易类型
 const u8 * pheBagIn发送包
 const u16 usBagInLen接收包
 SDK_ICC_CARDDATA const *pstCardData卡结构体
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:
 *****************************************************************************/
/*
 bool SaveSendRecord(bool bReversal, struct _TradeData *stTradeData, u8 *const pheBagIn, const u16 usBagInLen)
 {
 struct _RECORDSENDDATA stSend;        //发送特殊数据
 struct _APPINFO stAppInfo;
 u8 temp[STOREBAGLEN] = {0};
 u16 len;

 len = usBagInLen;
 memcpy(temp, pheBagIn, usBagInLen);

 memset(&stSend, 0, sizeof(stSend));
 stSend.usUserLen = sizeof(gstSendUserData);
 stSend.pUserVoid = (void *)&gstSendUserData;
 FormRecordSendData(bReversal, stTradeData, &stSend);        //组织发送区特殊数据

 if (StlSaveSendRecord(temp, len, &stSend) < 0)
 {
 DispRecordLibError(-1);
 return false;
 }
 //zxx 20130826 16:33 保存标志，告诉主控应用有交易记录
 if(bReversal)   //冲正时候需要标明有交易
 {
 memset(&stAppInfo, 0, sizeof(struct _APPINFO));
 ReadAppInfo(&stAppInfo);
 if (stAppInfo.asNeedSettle[0] != '1')
 {
 stAppInfo.asNeedSettle[0] = '1';
 SaveAppInfo(&stAppInfo);
 }
 }
 return true;
 }

 */
/*****************************************************************************
 ** Descriptions:
 ** Parameters:          struct _RECORDRECVDATA *pstRecordRecvData
 ** Returned value:
 ** Created By:		锟斤拷锟斤拷2012.10.31
 ** Remarks:
 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:	保存接收包数据
 ** Parameters:          const u8 ucReReason冲正原因如果不是冲正包写0
 const u8 ucTradeId交易ID
 const u8 * pheBagIn 接收包
 const u16 usBagInLen接收包长度
 enum _BAGTYPE eBagType包类型
 ** Returned value:
 ** Created By:		李琳2012.09.04
 ** Remarks:
 *****************************************************************************/
/*
 bool SaveRecvRecord(const u8 ucReReason, const u8 ucTradeId, u8 *const pheBagIn, const u16 usBagInLen, enum _BAGTYPE eBagType)
 {
 s32 slt;
 struct _RECORDRECVDATA stRecv;
 //    u8 temp[3] = {0};

 memset(&stRecv, 0, sizeof(stRecv));
 stRecv.usUserLen = sizeof(gstRecvUserData);
 stRecv.pUserVoid = (void *)&gstRecvUserData;

 if (eBagType == REOKBAG)
 {
 //sprintf(temp, "%02d", ucReReason);//zxx 20130410 11:16进来的是bcd码不是hex，这也是冲正码错误的原因
 //        memcpy(stRecv.asReversalReason, temp, 2);
 sdkBcdToAsc(stRecv.asReversalReason, &ucReReason, 1);
 }
 FormRecordRecvData(&stRecv);

 slt = StlSaveRecvRecord(pheBagIn, usBagInLen, eBagType, &stRecv);
 if (slt < 0)
 {
 DispRecordLibError(-1);
 return false;
 }

 return true;
 }
 */
/*****************************************************************************
 函 数 名  : ReplaceRecvRecord
 功能描述  : 替换接收包数据
 输入参数  : u8* const pucBagIn
 const u16 usBagInLen
 FormRecordRecvData(&stRecv);
 enum _BAGTYPE eBagType
 u16 usIndex
 enum _RECORDINDEXTYPE eIndexType
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
 1.日    期   :
 作    者   :
 修改内容   :

 *****************************************************************************/
/*
 bool ReplaceRecvRecord(u8 *const pucBagIn, const u16 usBagInLen, enum _BAGTYPE eBagType, u16 usIndex, enum _RECORDINDEXTYPE eIndexType)
 {
 struct _RECORDRECVDATA stRecv, stOriRecv;
 SDK_8583_ST8583 gstRecvBagTemp;
 struct _RecvUserData stUser;

 memset(&stRecv, 0, sizeof(stRecv));
 memset(&stUser, 0, sizeof(stUser));
 stRecv.usUserLen = sizeof(stUser);
 stRecv.pUserVoid = (void *)&stUser;
 FormRecordRecvData(&stRecv);

 //zxx 20130718 11:5 脱机ic上送，重写接收包，会导致之前脱机交易存在接收包中的数据被覆盖
 //所以如果新的接收包无55域，说明数据不用更新，写回脱机交易原来的数据
 //lilin 2015.02.09 14:3脱机上送不会返回55域，返回则错误
 //    if (!sdk8583IsDomainExist(&gstRecvBag, 55))
 {
 memcpy(&gstRecvBagTemp, &gstRecvBag, sizeof(SDK_8583_ST8583));

 memset(&stOriRecv, 0, sizeof(stOriRecv));
 ParseRecvRecord(&stOriRecv, usIndex, eIndexType);

 memcpy(stRecv.ucTVR, stOriRecv.ucTVR, 5); //TVR
 memcpy(stRecv.ucTSI, stOriRecv.ucTSI, 2); //TSI
 memcpy(stRecv.ucATC, stOriRecv.ucATC, 2); //ucATC
 memcpy(stRecv.ucTC, stOriRecv.ucTC, 8); //TC
 stRecv.ucIssueAppDataLen = stOriRecv.ucIssueAppDataLen;
 memcpy(stRecv.ucIssueAppData, stOriRecv.ucIssueAppData, stOriRecv.ucIssueAppDataLen); //发卡行认证数据
 stRecv.ucScriptLen = stOriRecv.ucScriptLen;
 memcpy(stRecv.ucScript, stOriRecv.ucScript, stOriRecv.ucScriptLen); //脚本结果

 memcpy(&gstRecvBag, &gstRecvBagTemp, sizeof(SDK_8583_ST8583));
 }
 if (StlReplaceRecvRecord(pucBagIn, usBagInLen, eBagType, &stRecv, usIndex, eIndexType) < 0)
 {
 DispRecordLibError(-1);
 return false;
 }
 return true;
 }
 */
/*****************************************************************************
 ** Descriptions:	从历史库中读取发送包并解析
 ** Parameters:          struct _RECORDSENDDATA *pstRecordSendData发送特殊数据
 u16 usIndex 索引号
 enum _RECORDINDEXTYPE eIndexType索引类型
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:             取出发送数据放在临时域中
 *****************************************************************************/
/*
 bool ParseSendRecord(struct _RECORDSENDDATA *pstRecordSendData, u16 usIndex, enum _RECORDINDEXTYPE eIndexType)
 {
 s32 rslt;

 //自定义数据处理
 if (pstRecordSendData->pUserVoid == NULL)
 {
 pstRecordSendData->usUserLen = sizeof(gstSendUserDataParse);
 pstRecordSendData->pUserVoid = (struct _SendUserData *)&gstSendUserDataParse;
 }

 memset(&gstTempBag, 0x00, sizeof(SDK_8583_ST8583));
 rslt = StlReadSendRecord(gstTempBag.ucBagData, &gstTempBag.nBagLen, pstRecordSendData, usIndex, eIndexType); //取发送包
 if (DispRecordLibError(rslt))       //判断-1文件系统故障
 {
 sdkDispPromptBox(false, TV_AUTORET, "读取发送包错误");
 return false;
 }

 //解包
 if (ParseField8583(&gstTempBag) < 0)   //解包失败
 {
 sdkDispPromptBox(false, TV_AUTORET, "解析发送包错误");
 return false;
 }

 if (!UnZipPag55Data(pstRecordSendData->ucTradeId, &gstTempBag))
 {
 //lilin 2015.07.01 16:10
 //        sdkDispPromptBox(false, TV_AUTORET, "解析发送包55域错误");
 //        return false;
 }

 return true;
 }

 */
/*****************************************************************************
 ** Descriptions:	从历史库中读取接收包并解析
 ** Parameters:          struct _RECORDRECVDATA *pstRecordRecvData接收特殊数据
 u16 usIndex 索引号
 enum _RECORDINDEXTYPE eIndexType索引类型
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:
 *****************************************************************************/
/*
 bool ParseRecvRecord(struct _RECORDRECVDATA *pstRecordRecvData, u16 usIndex, enum _RECORDINDEXTYPE eIndexType)
 {
 s32 rslt;

 //自定义数据处理
 if (pstRecordRecvData->pUserVoid == NULL)
 {
 pstRecordRecvData->usUserLen = sizeof(gstRecvUserDataParse);
 pstRecordRecvData->pUserVoid = (struct _RecvUserData *)&gstRecvUserDataParse;
 }

 memset(&gstRecvBag, 0x00, sizeof(SDK_8583_ST8583));
 rslt = StlReadRecvRecord(gstRecvBag.ucBagData, &gstRecvBag.nBagLen, pstRecordRecvData, usIndex, eIndexType); //取发送包
 if (DispRecordLibError(rslt))
 {
 sdkDispPromptBox(false, TV_AUTORET, "读取接收包错误");
 return false;   //判断-1文件系统故障
 }

 //解包
 if (ParseField8583(&gstRecvBag) < 0)   //解包失败
 {
 sdkDispPromptBox(false, TV_AUTORET, "解析接收包错误");
 return false;
 }

 //Trace8583Bag("历史库接收包", &stRecvBag);
 //zxx 20130730 17:2 不管后台有没有加55域，都解吧，无55时，只会导致相应的值为0而已，不然在"后台不回55时，批上送就没有数据了"
 //if (sdk8583IsDomainExist(&gstRecvBag, 55))          //判断是否55域
 //zxx 20130808 15:47 再次修改为，当冲正并无冲正码，说明是断电，接收包是没有东西了，那么不解55域，不然会覆盖发送包的
 if (!(eIndexType == REVERSAL_INDEX && !memcmp(pstRecordRecvData->asReversalReason, "\x00\x00", 2)))
 {
 ReFormRecordTLV(pstRecordRecvData);          //重写SDKicc数据
 }
 return true;
 }
 */

/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : ParseRecord
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟斤拷史锟斤拷锟叫讹拷取锟斤拷锟酵★拷锟斤拷锟秸帮拷锟斤拷锟斤拷锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : struct _RECORDSENDDATA *pstRecordSendData
 struct _RECORDRECVDATA *pstRecordRecvData
 const u16 usIndex
 const enum _RECORDINDEXTYPE eIndexType
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  :
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   :
 锟斤拷    锟斤拷   :
 锟睫革拷锟斤拷锟斤拷   :

 *****************************************************************************/
/*
 bool ParseRecord(struct _RECORDSENDDATA *pstRecordSendData, struct _RECORDRECVDATA *pstRecordRecvData, const u16 usIndex, const enum _RECORDINDEXTYPE eIndexType)
 {
 bool isok = false;

 if (ParseSendRecord(pstRecordSendData, usIndex, eIndexType)) //取锟斤拷锟酵帮拷
 {
 isok = ParseRecvRecord(pstRecordRecvData, usIndex, eIndexType);
 }
 return isok;
 }

 */
/*****************************************************************************
 ** Descriptions:	锟斤拷锟斤拷锟斤拷锟斤拷
 ** Parameters:          struct _RECORDINDEX *pstRecordIndex 锟斤拷录锟斤拷锟斤拷
 u16 usIndex 锟斤拷录锟斤拷锟斤拷锟斤拷
 enum _RECORDINDEXTYPE eIndexType 锟斤拷锟斤拷锟斤拷锟斤拷
 ** Returned value:
 ** Created By:		锟斤拷锟斤拷2012.10.15
 ** Remarks:
 *****************************************************************************/
/*
 bool ParseRecordIndex(struct _RECORDINDEX *pstRecordIndex, u16 usIndex, enum _RECORDINDEXTYPE eIndexType)
 {
 //锟皆讹拷锟斤拷锟斤拷锟捷达拷锟斤拷
 if (pstRecordIndex->pUserVoid == NULL)
 {
 pstRecordIndex->usUserLen = sizeof(gstRecordIndexUserDataParse);
 pstRecordIndex->pUserVoid = (u8 *)(struct _RecordIndexUserData *)&gstRecordIndexUserDataParse;
 }
 if (StlReadRecordIndex(pstRecordIndex, usIndex, eIndexType) < 0)
 {
 DispRecordLibError(-1);
 return false;
 }
 return true;
 }
 */
/*****************************************************************************
 ** Descriptions:    锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
 ** Parameters:
 enum _RECORDINDEXTYPE eIndexType 锟斤拷锟斤拷锟斤拷锟斤拷
 ** Returned value:
 ** Created By:
 ** Remarks:
 *****************************************************************************/
/*
 s32 GetRecordIndexNum(enum _RECORDINDEXTYPE eIndexType)
 {
 s32 num = 0;
 num = StlGetRecordIndexNum(eIndexType);   //取锟斤拷史锟斤拷锟斤拷锟斤拷
 Trace("GetRecordIndexNum", "num=[%d]\r\n", num);
 if (DispRecordLibError(num))
 {
 return 0;      //锟叫讹拷-1锟侥硷拷系统锟斤拷锟斤拷
 }

 return num;
 }
 */

/*****************************************************************************
 ** Descriptions:	锟斤拷锟斤拷锟斤拷水锟脚憋拷锟芥撤锟斤拷锟斤拷志
 ** Parameters:          const bool bUndo锟斤拷锟斤拷锟斤拷志
 const u8 *pucTraceNo锟斤拷水锟斤拷
 ** Returned value:
 ** Created By:		锟斤拷锟斤拷2012.10.18
 ** Remarks:
 *****************************************************************************/
/*
 bool SaveUndoFlag(const bool bUndo, u8 *const pucTraceNo)
 {
 bool isok = false;
 u16 index = 0;

 if (pucTraceNo == NULL)
 {
 Assert(0);
 return isok;
 }

 if (StlFindRecordIndex(&index, pucTraceNo) > 0)      //锟揭碉拷锟矫笔斤拷锟斤拷
 {
 if (StlSaveUndoFlag(bUndo, index) > 0)
 {
 isok = true;
 }
 else
 {
 Assert(0);
 }
 }
 else
 {
 Assert(0);
 }

 if (!isok)
 {
 DispRecordLibError(-1);
 }
 return isok;
 }
 */
/*****************************************************************************
 ** Descriptions:	删锟斤拷锟斤拷锟斤拷锟斤拷
 ** Parameters:    	u8 *const pucTraceNo 锟斤拷水锟斤拷
 ** Returned value:
 ** Created By:		lilin2012.12.09
 ** Remarks:
 *****************************************************************************/
/*
 bool DelNomarlBag(u8 *const pucTraceNo)
 {
 bool isok = false;
 u16 index = 0;
 s32 rslt;

 if (pucTraceNo == NULL)
 {
 Assert(0);
 return isok;
 }

 rslt = StlFindRecordIndex(&index, pucTraceNo);
 Trace("DelNomarlBag", "rslt=[%d] index[%d]\r\n", rslt, index);
 if (rslt > 0)      //锟揭碉拷锟矫笔斤拷锟斤拷
 {
 rslt = StlDelNormalBag(index);
 Trace("DelNomarlBag", "rslt=[%d]\r\n", rslt);
 if (rslt > 0)
 {
 isok = true;
 }
 else
 {
 Assert(0);
 }
 }
 else
 {
 isok = true;
 }

 if (!isok)
 {
 DispRecordLibError(-1);
 }
 return isok;
 }

 */
/*****************************************************************************
 ** Descriptions:	锟斤拷锟斤拷锟斤拷水锟脚憋拷锟斤拷锟斤拷锟斤拷锟街?
 ** Parameters:          const bool bUndo锟斤拷锟斤拷锟斤拷志
 const u8 *pucTraceNo锟斤拷水锟斤拷
 ** Returned value:
 ** Created By:		锟斤拷锟斤拷2012.10.18
 ** Remarks:
 *****************************************************************************/
/*
 bool SaveAdjustFlag(const u8 ucAdjustFlag, u8 *pucTraceNo)
 {
 bool isok = false;
 u16 index = 0;

 if (pucTraceNo == NULL)
 {
 Assert(0);
 return isok;
 }

 if (StlFindRecordIndex(&index, pucTraceNo) > 0)      //锟揭碉拷锟矫笔斤拷锟斤拷
 {
 if (StlSaveAdjustFlag(ucAdjustFlag, index) > 0)
 {
 isok = true;
 }
 else
 {
 Assert(0);
 }
 }
 else
 {
 Assert(0);
 }

 if (!isok)
 {
 DispRecordLibError(-1);
 }
 return isok;
 }
 */
/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : SavePrintFlag
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟斤拷锟接★拷锟街?
 锟斤拷锟斤拷锟斤拷锟? : const bool bPrint //true:锟窖达拷印 false:未锟斤拷印
 u8* const pucTraceNo //锟斤拷锟斤拷锟斤拷水锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  :
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   : 2013锟斤拷3锟斤拷7锟斤拷
 锟斤拷    锟斤拷   : zhangjun
 锟睫革拷锟斤拷锟斤拷   : 锟斤拷锟斤拷锟缴猴拷锟斤拷

 *****************************************************************************/
/*
 bool SavePrintFlag(const bool bPrint, u8 *const pucTraceNo)
 {
 bool isok = false;
 u16 index = 0;

 if (pucTraceNo == NULL)
 {
 Assert(0);
 return isok;
 }

 if (StlFindRecordIndex(&index, pucTraceNo) > 0)      //锟揭碉拷锟矫笔斤拷锟斤拷
 {
 if (StlSavePrintFlag(bPrint, index) > 0)
 {
 isok = true;
 }
 else
 {
 Assert(0);
 }
 }
 else
 {
 Assert(0);
 }

 if (!isok)
 {
 DispRecordLibError(-1);
 }
 return isok;
 }

 */

/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : DealSendMagLoadConfirmBag
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟酵达拷锟斤拷锟斤拷锟街斤拷锟街等凤拷习锟?
 锟斤拷锟斤拷锟斤拷锟? : bool bAfterReversal
 s32 iIndex
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  :
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   :
 锟斤拷    锟斤拷   :
 锟睫革拷锟斤拷锟斤拷   :

 *****************************************************************************/
/*
 s32 DealSendMagLoadConfirmBag(bool bAfterReversal, s32 iIndex)
 {
 //锟矫达拷锟斤拷锟斤拷锟街斤拷锟街等凤拷习锟斤拷锟斤拷撞锟斤拷芨谋锟絞stTradeData锟斤拷锟饺拷值锟街碉拷锟?
 //实锟斤拷锟斤拷也锟矫诧拷锟斤拷

 s32 slt;
 s32 times = 0;
 u32 uiTotalTimes;
 u8 res[3] = {0};
 bool bisok = false;
 struct _TradeData stTradeData;
 struct _RECORDDATA stRecordData;
 SDK_8583_ST8583 *pstBag8583;

 pstBag8583 = &gstSendBag;

 uiTotalTimes = gstAppSetting.stOtherTradeParam.uiReversalTimes;
 if (bAfterReversal)
 {
 uiTotalTimes = 0;
 }

 for (times = 0; times <= uiTotalTimes; times++)
 {
 slt = QueryCommu();
 if (slt != SDK_OK)
 {
 return SDK_ESC;
 }

 memset((u8 *)&stRecordData, 0, sizeof(stRecordData));
 if (ParseRecord(&stRecordData.stSendData, &stRecordData.stRecvData, iIndex, REVERSAL_INDEX) == false)
 {
 return SDK_ESC;
 }

 InitFormSt8583(pstBag8583);

 SetFieldData(pstBag8583, SDK_8583_FIELD_MSG, "0220", 4); //锟斤拷息锟斤拷锟斤拷

 //锟斤拷原锟斤拷锟斤拷锟酵帮拷一锟斤拷
 CopySendField(pstBag8583, 2);
 CopySendField(pstBag8583, 3);
 CopySendField(pstBag8583, 4);
 CopySendField(pstBag8583, 11);
 CopySendField(pstBag8583, 14);
 CopySendField(pstBag8583, 22);
 CopySendField(pstBag8583, 25);
 SetPublicField041(pstBag8583, NULL);
 SetPublicField042(pstBag8583, NULL);
 CopySendField(pstBag8583, 49);

 //60锟斤拷
 CopySendField(pstBag8583, 60);

 SetPublicField064(pstBag8583, NULL);
 slt = EncryptSendData(&gstSendBag);
 if (slt == SDK_ESC)
 {
 //            continue;
 return SDK_ESC;
 }
 slt = Send8583(&gstSendBag, &gstRecvBag, DISP_APPAFFIRM);
 if (slt == SDK_OK)
 {
 StlDelReversalBag(iIndex); //锟斤拷锟斤拷锟斤拷锟?

 memset((u8 *)&stTradeData, 0, sizeof(stTradeData));
 stTradeData.ucTradeId = TRADEID_MAGLOAD_CASH;

 memcpy(&gstSendBag, &gstTempBag, sizeof(SDK_8583_ST8583));
 bisok = SaveSendRecord(false, &stTradeData, gstSendBag.ucBagData, gstSendBag.nBagLen);
 if (!bisok)
 {
 return SDK_ESC;
 }

 bisok = SaveRecordIndex(TRADEID_MAGLOAD_CASH, NORMAL_INDEX);
 if (!bisok)
 {
 return SDK_ESC;
 }

 bisok = SaveRecvRecord(0x00, TRADEID_MAGLOAD_CASH, gstRecvBag.ucBagData, gstRecvBag.nBagLen, OKBAG);
 if (!bisok)
 {
 return SDK_ESC;
 }

 DealTradeReponse(SDK_OK);

 if (bAfterReversal)                                     //锟铰猴拷锟斤拷锟?
 {
 sdkCommDestoryLink();
 }
 return SDK_OK;
 }
 else //if (slt < 0)    //锟斤拷锟斤拷SDK_COMM_WAVELOST  SDK_TIME_OUT  ERR_UNKOWREVBAG
 {
 if (slt == SDK_COMM_WAVELOST)
 {
 //lilin 20140725
 AppDestorySSL();

 sdkCommDestoryLink();
 DispWaveLost();
 return slt;
 }
 else
 {
 memset(res, 0, sizeof(res));
 if (GetFieldData(&gstRecvBag, 39, res, 2))
 {
 if (!memcmp(res, "A0", 2)) //锟斤拷锟斤拷锟斤拷锟斤拷锟接︼拷锟斤拷锟斤拷锟紸0 MAC锟斤拷锟斤拷锟斤拷示锟斤拷锟斤拷签锟斤拷
 {
 sdkCommDestoryLink();
 DispResponseError();
 return SDK_ESC;
 }

 }
 }
 }
 }

 //锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷,锟斤拷锟斤拷失锟斤拷,锟斤拷锟斤拷锟斤拷锟?
 if (bAfterReversal) //锟铰猴拷锟斤拷锟斤拷锟斤拷晒锟?
 {
 sdkCommDestoryLink();
 sdkDispPromptBox(false, TV_AUTORET, "锟斤拷锟斤拷失锟斤拷");
 return SDK_ESC;
 }

 StlDelReversalBag(iIndex); //锟斤拷锟斤拷锟斤拷锟?
 return SDK_OK;
 }

 */

/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : SendReversalBag
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟街斤拷锟街等凤拷锟?
 锟斤拷锟斤拷锟斤拷锟? : bool bAfterReversal
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  :
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   : 2013锟斤拷1锟斤拷5锟斤拷
 锟斤拷    锟斤拷   : zj
 锟睫革拷锟斤拷锟斤拷   : 锟斤拷锟斤拷锟缴猴拷锟斤拷

 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:    锟斤拷锟酵脚憋拷锟斤拷
 ** Parameters:          void
 ** Returned value:
 ** Created By:		lilin2012.10.11
 ** Remarks:
 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:	锟斤拷锟斤拷锟酵斤拷锟阶帮拷之前
 ** Parameters:          void
 ** Returned value:	SDK_ESC;SDK_OK
 ** Created By:		lilin2012.10.11
 ** Remarks:             锟斤拷锟酵脚憋拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
 *****************************************************************************/

/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : FindRecord
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟斤拷锟斤拷史锟斤拷锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : bool bReInput 锟角凤拷支锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
 u8* const pbcTraceNo 锟斤拷水锟斤拷
 struct _RECORDINDEX *pstRecordIndex, 锟斤拷锟斤拷锟斤拷锟?
 s32 (*pFun)(struct _RECORDINDEX *pstRecordIndex, u8 *pasDispInfo) 锟斤拷锟斤拷校锟介函锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  : SDK_ESC:未锟揭碉拷锟斤拷锟竭诧拷锟斤拷锟较癸拷锟斤拷 SDK_OK:锟揭碉拷 SDK_EQU:锟斤拷要锟斤拷锟铰诧拷锟斤拷
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   : 2013锟斤拷1锟斤拷9锟斤拷
 锟斤拷    锟斤拷   : zj
 锟睫革拷锟斤拷锟斤拷   : 锟斤拷锟斤拷锟缴猴拷锟斤拷

 *****************************************************************************/

#if 0 /*del by lilin at 20140507 */

/*****************************************************************************
 ** Descriptions:
 ** Parameters:    	const s32 siReturn 锟斤拷锟秸憋拷锟侥凤拷锟斤拷值
 SDK_ICC_TRADE_PARAM *pstIccTradeParam //IC锟斤拷锟斤拷锟阶诧拷锟斤拷锟斤拷
 SDK_ICC_PARAM *pstIccParam //IC锟斤拷锟斤拷锟斤拷锟斤拷
 ** Returned value:
 ** Created By:		lilin2014.03.21
 ** Remarks: 		//lilin 20140327
 *****************************************************************************/
static s32 DealRecvICCFlow(const s32 siReturn,const bool bReversal,SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam)
{
	SDK_EMV_RLT eEmvRet = 0;
	u8 temp[256] = {0};
	s32 len = 0;
	u8 i = 0;
	s32 ret=siReturn;
	bool flag = false;

	UnZipPag55Data(gstTradeData.ucTradeId, &gstRecvBag);
	EmvImportRespCode(true);
	eEmvRet = EmvProcessOnlineData(pstIccTradeParam, pstIccParam);

	if (eEmvRet != ICCONLINE)
	{
		gstRecvUserData.bisOnlineDecline = true;
		if (siReturn == SDK_OK)    //zxx 20130729 17:9锟斤拷台锟叫兑匡拷片锟杰撅拷要锟斤拷示
		{
			//锟斤拷锟窖猴拷预锟斤拷权要锟斤拷示//zxx 20130929 11:1
			if (gstTradeData.ucTradeId == TRADEID_SALE || gstTradeData.ucTradeId == TRADEID_PREAUTH)
			{
				sdkDispPromptBox(false, 500, "锟斤拷锟斤拷系锟斤拷锟斤拷锟斤拷");
			}
			//zxx 20131018 13:44 锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷芫锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷植锟斤拷贸锟斤拷锟斤拷模锟斤拷锟矫匆拷艿锟?
//            if (bReversal)
//            {
//                ret = ERR_NEEDREVERSAL;
//            }
//            else
//            {
			ret = SDK_ERR;
//            }
		}
	}
	else
	{
		if (siReturn == SDK_OK)
		{
			if (gstTradeData.ucTradeId == TRADEID_SALE || gstTradeData.ucTradeId == TRADEID_PREAUTH)
			{
				flag = false;
				//通锟斤拷TVR锟叫断凤拷锟斤拷锟斤拷锟斤拷证锟斤拷锟?
				memset(temp,0,sizeof(temp));
				if (sdkIccReadTLV("\x95", temp, &len) > 0)
				{
					if (0x40 == (temp[4] & 0x40))    //锟斤拷锟斤拷锟斤拷锟斤拷证失锟斤拷
					{
						flag = true;
					}
				}
				if (flag)
				{
					flag = false;
					//通锟斤拷TSI锟叫断凤拷锟斤拷锟斤拷锟斤拷证锟角凤拷锟斤拷锟?
					memset(temp,0,sizeof(temp));
					if (sdkIccReadTLV("\x9B", temp, &len) > 0)
					{
						if (0x10 == (temp[0] & 0x10)) //锟斤拷锟斤拷锟斤拷锟斤拷证锟窖撅拷锟斤拷锟斤拷
						{
							flag = true;
						}
					}
				}
			}
			if(flag)        //锟斤拷锟斤拷锟斤拷锟斤拷证失锟杰斤拷锟阶硷拷锟斤拷锟斤拷示锟斤拷锟斤拷系锟斤拷锟斤拷锟斤拷
			{
				sdkDispPromptBox(false, 500, "锟斤拷锟斤拷系锟斤拷锟斤拷锟斤拷");
			}
		}
	}
	if (pstIccTradeParam->eFlowMode == SDK_ICC_EC_LOAD
			&&(siReturn == SDK_OK))
	{
		//圈锟斤拷疟锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟轿?锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
		//zxx 20130814 14:15 锟睫改脚憋拷锟斤拷锟斤拷卸锟?
		flag = false;
		if (sdkIccReadTLV("\x95", temp, &len) > 0)//通锟斤拷TVR锟叫断脚憋拷执锟叫斤拷锟?
		{
			if (0x10 != (temp[4] & 0x10)) //锟脚憋拷执锟叫成癸拷
			{
				flag = true;
			}
		}
		if (flag)
		{
			flag = false;
			if (sdkIccReadTLV("\x9B", temp, &len) > 0) //通锟斤拷TSI锟叫断脚憋拷执锟叫斤拷锟?
			{
				if (0x04 == (temp[0] & 0x04)) //锟脚憋拷锟斤拷执锟斤拷
				{
					flag = true;
				}
			}
		}
		if (flag)
		{
			flag = false;
			len = 0;
			memset(temp, 0, sizeof(temp));
			sdkIccGetScriptResult(&len, temp);
			if (len)
			{
				for (i = 0; i < len / 5; i++)
				{
					if ((temp[i * 5] & 0xF0) == 0x20) //锟脚憋拷锟斤拷锟街达拷谐晒锟?
					{
						flag = true;
					}
					else
					{
						flag = false;
						break;
					}
				}
			}
		}
		if(!flag)
		{
			ret = SDK_ERR;
		}
		Trace("zxx", "slt = %d\r\n", ret);
	}
	return ret;
}
#endif /* if 0 */
/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : SaveRecvData
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟秸凤拷锟斤拷锟斤拷锟捷猴拷拇锟斤拷锟斤拷锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : u8 eBagType
 bool bReversal
 bool bSaveIndex
 s32 iRet
 u8* const pheBagIn
 const u16 usBagInLen
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  :
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   : 2012锟斤拷12锟斤拷20锟斤拷
 锟斤拷    锟斤拷   : wuzhonglin
 锟睫革拷锟斤拷锟斤拷   : 锟斤拷锟斤拷锟缴猴拷锟斤拷
 //lilin 20140327
 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:        锟叫断凤拷锟斤拷锟斤拷锟斤拷证锟角凤拷失锟斤拷
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		lilin2014.05.07
 ** Remarks:
 *****************************************************************************/

/*****************************************************************************
 ** Descriptions:	锟叫断脚憋拷执锟叫斤拷锟斤拷欠锟絆K
 ** Parameters:    	void
 ** Returned value:
 ** Created By:		lilin2014.05.07
 ** Remarks:
 *****************************************************************************/

/*****************************************************************************
 锟斤拷 锟斤拷 锟斤拷  : Send8583Bag
 锟斤拷锟斤拷锟斤拷锟斤拷  : 锟斤拷锟斤拷8583锟斤拷锟侥和达拷锟斤拷锟斤拷锟?
 锟斤拷锟斤拷锟斤拷锟? : enum SENDHEAD_FLAG eSendHeadFlag //锟角凤拷锟斤拷前锟斤拷锟酵癸拷锟斤拷锟斤拷锟斤拷
 enum SENDBAG_MODE eSendMode //锟斤拷锟斤拷锟斤拷锟侥Ｊ?
 SDK_ICC_TRADE_PARAM *pstIccTradeParam //IC锟斤拷锟斤拷锟阶诧拷锟斤拷锟斤拷
 SDK_ICC_PARAM *pstIccParam //IC锟斤拷锟斤拷锟斤拷锟斤拷
 s32 (*pFun)(struct _TradeData *pstTradeData
 u8 *pheDataBuf) //锟斤拷织锟斤拷锟侥猴拷锟斤拷
 锟斤拷锟斤拷锟斤拷锟? : 锟斤拷
 锟斤拷 锟斤拷 值  : SDK_OK:锟斤拷锟阶成癸拷 sdk_equ:锟解部锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟?
 锟斤拷锟矫猴拷锟斤拷  :
 锟斤拷锟斤拷锟斤拷锟斤拷  :

 锟睫革拷锟斤拷史      :
 1.锟斤拷    锟斤拷   : 2012锟斤拷12锟斤拷26锟斤拷
 锟斤拷    锟斤拷   : wuzhonglin
 锟睫革拷锟斤拷锟斤拷   : 锟斤拷锟斤拷锟缴猴拷锟斤拷
 锟斤拷注使锟斤拷要小锟斤拷pstIccTradeParam锟斤拷pstIccParam 锟结传锟斤拷锟街革拷锟?
 *****************************************************************************/
/*
 s32 Send8583Bag(
 bool bSendHead, //锟角凤拷锟斤拷前锟斤拷锟酵癸拷锟斤拷锟斤拷锟斤拷
 enum SENDBAG_MODE eSendMode, //锟斤拷锟斤拷锟斤拷锟侥Ｊ?
 s32(*pFun)(SDK_8583_ST8583 *pstBag8583,TlvArray),  //锟斤拷织锟斤拷锟侥猴拷锟斤拷
 u8 const ucRevDisp
 )
 {
 s32 slt = SDK_OK;
 bool isok = false;
 bool bSaveRecord, bSaveIndex, bReversal;
 u8 temp[256] = {0};
 u8 i = 0;
 SDK_8583_ST8583 stunencryptsendbag;
 u8 ResponseCode[8] = {0};
 s32 rslt;
 // enum ICCRETURN eEmvRet;

 if (pFun == NULL)
 {
 return SDK_ESC;
 }
 //lilin 20131214  锟角斤拷指示锟斤拷
 //   LampConnecting();      //锟饺达拷锟角接碉拷
 bSaveRecord = SEND_SAVERECORD & eSendMode;
 bSaveIndex = SEND_SAVEINDEX & eSendMode;
 bReversal = SEND_REVERSAL & eSendMode;
 if (!bSaveIndex && !bSaveRecord) bReversal = false; //zxx 20130804 16:42 锟斤拷锟斤拷锟叫讹拷
 //lilin 2015.04.01 14:1 锟睫革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟揭拷锟斤拷虏锟斤拷锟斤拷锟揭拷锟街?
 for (i = 0; i < 3; i++) //zxx 20130808 17:15循锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
 {
 //------------------------------------------//
 // 锟介发锟酵帮拷
 slt = pFun(&TlvArray, &stunencryptsendbag);
 if (!slt)
 {
 return SDK_ESC;
 }
 //lilin 20140324 锟斤拷未锟斤拷锟斤拷锟斤拷锟捷革拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷
 memcpy(&gstSendBag,&stunencryptsendbag,sizeof(stunencryptsendbag));
 if (slt == SDK_ESC)
 {
 return SDK_ESC;
 }
 //锟斤拷锟芥发锟酵帮拷
 if (bSaveRecord)
 {
 //锟斤拷锟芥发锟酵帮拷
 //lilin 20140324 锟斤拷锟斤拷未锟斤拷锟斤拷之前锟侥憋拷锟斤拷
 isok = SaveSendRecord(bReversal, &gstTradeData, stunencryptsendbag.ucBagData, stunencryptsendbag.nBagLen);
 if (!isok)
 {
 return SDK_ESC;
 }
 }
 return slt;
 }
 return SDK_ESC;//锟斤拷锟斤拷锟竭碉拷锟斤拷锟斤拷锟斤拷
 }
 /*****************************************************************************
 ** Parameters:          SDK_ICC_TRADE_PARAM *pstIccTradeParam
 SDK_ICC_PARAM *pstIccParam
 ** Returned value:
 ** Created By:		wangb 2016 0314
 ** Remarks:
 包含的处理流程：
 ,55 域数据获取处理，二次授权，脚本
 *****************************************************************************/
s32 EmvProcessOnlineData(SDK_8583_ST8583 *pstRevBag8583,
		struct _TradeData *stTradeData, u8*pOutData) {
	s32 eEmvRet = 0;
	s32 pOutDatalen = 0;
	u8 revtmp[512] = { 0 };
	u8 asRespCode[16] = { 0 };
	u8 asAuthCode[32] = { 0 };

	s32 rslt;
	u16 len = 0;
	if (sdk8583IsDomainExist(pstRevBag8583, 39))          //鍒ゆ柇39鍩熸槸鍚︽暟鎹?
			{
		len = GetFieldData(pstRevBag8583, 39, asRespCode, sizeof(revtmp)); //鑾峰彇39鍩熸暟鎹?
		Log("wangb", "asRespCode is %s ", asRespCode);
		if (len < 0) {
			return SDK_ERR;
		}
	}
	memset(revtmp, 0, sizeof(revtmp));
	if (sdk8583IsDomainExist(pstRevBag8583, 38))          //鍒ゆ柇38鍩熸槸鍚︽暟鎹?
			{
		len = GetFieldData(pstRevBag8583, 38, asAuthCode, sizeof(revtmp)); //鑾峰彇38鍩熸暟鎹?
		Log("wangb", "asAuthCode is %s ", asAuthCode);
	}

	memset(revtmp, 0, sizeof(revtmp));
//    Log("wangb","asAuthCode is %s ",asAuthCode);
//    u8 aaa[] = "\x71\x1B\x86\x19\x84\x24\x00\x01\x14\x64\xAB\xC0\xD8\xCD\x46\xA7\x3A\x3D\x52\x9E\xFA\x63\x78\x05\xFE\x39\xD7\x25\xB2";
//    SetFieldData(pstRevBag8583, 55, aaa,29 );
	if (!IsMobileTypeTrade(stTradeData->ucTradeId)) {
		if (sdk8583IsDomainExist(pstRevBag8583, 55))          //鍒ゆ柇55鍩熸槸鍚︽暟鎹?
		{
			len = GetFieldData(pstRevBag8583, 55, revtmp, sizeof(revtmp)); //鑾峰彇55鍩熸暟鎹?
			if (len > 0) {
				ProviderSetTagValue(TAG_8583_EMV55Type, revtmp, len); //Fixme 是否
				eEmvRet = ProviderSecAuth(asRespCode, asAuthCode, revtmp, len, pOutData, &pOutDatalen);      //Fixme 二次授权  返回数据
				if (eEmvRet < 0) {
					Log("wangb", "ProviderSecAuth eEmvRet : %d ", eEmvRet);
					return SDK_ERR;
				} else {
					if (strstr(pOutData, "DF31")) {
						ProviderSetTagValue(TAG_TRADE_UNDOFLAG, "1", 1);   //宏定义
						eEmvRet = SendScriptBag(pstRevBag8583, stTradeData);
						Log("wangb", "SendScriptBag is %d ", eEmvRet);
					}
				}
			} else {
				Log("wangb", "GetFieldData 55 len is %d ", len);
				return SDK_ERR;
			}
		}

	}

	if (eEmvRet == SDK_ERR) {
		eEmvRet = SDK_ERR;
	}
	return eEmvRet;
}
/*****************************************************************************
 ** Descriptions:
 ** Parameters:         struct _TradeData

 wangb 2016.03.08
 ** Returned value:
 流程2处理
 ** Remarks: 		参数会进行修改
 *****************************************************************************/
s32 PostPag55Data(SDK_8583_ST8583 * pstRevBag8583,struct _TradeData *stTradeData) {
	s32 nRet = 0;
	s32 eEmvRet = 0;
	u8 pOutData[512] = { 0 };

	eEmvRet = EmvProcessOnlineData(pstRevBag8583, stTradeData, pOutData); //执行流程2
	if (eEmvRet == SDK_ERR)        //联机批准
	{
		eEmvRet = SDK_ERR;        //联机交易失败
	} else {

	}
	return eEmvRet;
}
/*****************************************************************************
** Descriptions:	电子现金非指定账户充值组48域
** Parameters:    	SDK_8583_ST8583 *pstBag8583 8583报文结构体
                               struct _TradeData *pstTradeData 交易参数结构体
** Returned value:	true 正确false失败
** Created By:		lilin2013.12.06
** Remarks:
*****************************************************************************/
bool SetEcCaccNoField048(SDK_8583_ST8583 *pstBag8583,  struct _TradeData *pstTradeData)
{
    u8 temp[512] = {0};
    u8 ucCardMode; //卡模式
    u8 ucCLType; //实际IC卡流程
    bool bIsPwd; //是否输入密码

    if (pstTradeData == NULL)
    {
        return false;    //该域需要struct _TradeData内的数据，为NULL时直接返回失败。
    }

    ucCardMode = pstTradeData->strInCardInfo.stCardParam.ucCardMode;
    ucCLType = pstTradeData->strInCardInfo.stCardParam.ucCLType;
    bIsPwd = false;


    if (ucCardMode == SDK_ICC_ICC || ucCardMode == SDK_ICC_RF)        //IC卡
    {
        if (ucCLType == ICCPBOCFLOW)               //PBOC流程
        {
            strcpy(temp, "98");
        }
        else if (ucCLType == ICCQPBOCFLOW)       //QPBOC流程
        {
            strcpy(temp, "07");
        }
        else if (ucCLType == ICCMSDFLOW)           //MSD流程
        {
            strcpy(temp, "91");
        }
        else
        {
            strcpy(temp, "05");
        }
    }
    else
    {
        strcpy(temp, "00");
    }

    if (bIsPwd)           //是否输入密码
    {
        strcat(temp, "1");
    }
    else
    {
        strcat(temp, "2");
    }

    strcat(temp, "0");


    SetFieldData(pstBag8583, 48, temp, strlen(temp));                //输入方式

    return true;

}

