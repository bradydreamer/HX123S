#ifndef _TAG_H_
#define _TAG_H_

//#define	MAX_TAG_LIST_SIZE	150

#undef u8
#define  u8   unsigned char

#undef U8

#undef s8
#define s8  signed char

#undef S8

#undef u16
#define  u16  unsigned short

#undef U16


#undef s16
#define  s16   signed short

#undef S16



#undef u32
#define  u32  unsigned int

#undef U32


#undef s32
#define  s32    signed int

#undef S32

#define TAG_TRADEID_DownLoadPMK	"trade_id_downloadPrimaryKey"
#define TAG_TRADEID_Locate	"trade_id_locate"
#define TAG_TRADEID_SIGNIN				"trade_id_signin"
#define TAG_TRADEID_SALE		               "trade_id_sale"
#define TAG_TRADEID_UNSALE				"trade_id_unsale"

#define TAG_TRADEID_REFUND              "trade_id_refund"
#define TAG_TRADEID_PREAUTH              "trade_id_preauth"
#define TAG_TRADEID_UNPREAUTH           "trade_id_unpreauth"
#define TAG_TRADEID_ADDPREAUTH           "trade_id_addpreauth"


#define TAG_TRADEID_COMPLETE            "trade_id_complete"
#define TAG_TRADEID_UNCOMPLETE            "trade_id_uncomplete"

#define TAG_TRADEID_OFFCOMPLETE            "trade_id_Offcomplete"

/*=======BEGIN: huacong 2016.03.11  11:9 modify===========*/
#define TAG_TRADEID_QUERY               "trade_id_query"
#define TAG_TRADEID_SETTLE              "trade_id_settle"
/*====================== END======================== */

#define TAG_JINGDU	  "FF8083" //定位 经度
#define TAG_WEIDU	  "FF8084" //定位 纬度

#define TAG_TPDU	  "FF8011"
#define TAG_BatchNo	  "FF9022"

#define TAG_MerchantName 	"FF8018"

#define TAG_TRADE_TYPE					"FF9000"
#define TAG_8583_CARDNO                                    "FF9001"
#define TAG_8583_AMOUNT                                  "FF9002"

#define TAG_8583_HMS                                   "FF9004"
#define TAG_8583_MD                                    "FF9005"

#define TAG_8583_POSVLU                                   "FF9014"

#define TAG_8583_VoucherNo                              "FF9003"

#define TAG_8583_ExpiredDate                            "FF9006"
#define TAG_8583_CardMode                                "FF9008"
#define TAG_8583_CardParam                              "FF9009"
#define TAG_8583_BKRESULT                             "FF9010"
#define TAG_SYS_CardID                                   "FF900A"
#define TAG_8583_Track2Data                              "FF900D"
#define TAG_8583_Track3Data                              "FF900E"
#define TAG_8583_SysRefNo                                 "FF900F"
#define TAG_8583_AuthCode                                "FF901F"
#define TAG_8583_TerminalNo                             "FF9012"

#define TAG_8583_IssuerBank                             "FF9013"
#define TAG_8583_AcquirerBank                             "FF9014"

#define TAG_8583_MerchantNo                           "FF9011"
#define TAG_8583_CurrencyCode                        "FF901B"
#define TAG_8583_CreditPwd                              "FF901C"

#define TAG_8583_DomainExist                          "FF901D"
#define TAG_8583_EMV55Type                         "FF902D"

#define TAG_8583_BALANCETYPE                         "FF9020"
#define TAG_8583_BALANCE                          "FF9021"

#define   TAG_8583_OLDVOUCHERNO                          "FF9029"

#define    TAG_8583_AsBatchNo                     "FF9022"
#define    TAG_8583_SignSupport                      "FF9026"
#define    TAG_8583_OldAsBatchNo                   "FF9028"
#define    TAG_8583_OldVoucherNo                     "FF9029"
#define    TAG_8583_AsTradeDate                     "FF902A"
#define   TAG_8583_CLTYPE                          "FF9070"

/*=======BEGIN: huacong 2016.03.03  16:21 modify===========*/
#define    TAG_8583_SENDECRIPTBAG              "FF908D"  //加密报文
#define    TAG_8583_RECVECRIPTBAG              "FF908E"
#define    TAG_8583_AUCENCRYPTKEY              "FF8082"  //全包文加密密钥
#define    TAG_8583_SENDBAG                     "FF9030" //未全报文加密的报文
#define    TAG_8583_RECVBAG                     "FF9031"
/*====================== END======================== */


#define    TAG_8583_OldSysRefNo                       "FF9054"
#define    TAG_8583_UsManNo                    "FF9041"
#define    TAG_8583_MAC                     "FF9045"

#define    TAG_8583_FieldALL                          "FF9030"

/*=======BEGIN: huacong 2016.03.03  15:47 modify===========*/
#define    TAG_TRADE_MACTYPE                         "FF8010"

#define    TAG_TRADE_SUPPORTTDK                         "FF8015"
#define    TAG_TRADE_TMKINDEX                          "FF8016" 
#define    TAG_TRADE_KLKINDEX                          "FF8080" 
#define    TAG_TRADE_DESTYPE                         "FF8017"
#define    TAG_TRADE_PMKID                         "FF8081"
/*====================== END======================== */

#define TAG_TRADE_REVERSAL                          "FF907E"
#define TAG_TRADE_REVERSALBAG                       "FF907F"

#define TAG_TRADE_UNDOFLAG                          "FF907B"

#define TAG_SCIRTP_SCRIPT                          "FF9072"
#define TAG_SCIRTP_SENDFLAG                          "FF9073"
#define TAG_SCIRTP_RCVEFLAG                          "FF9074"

/*=======BEGIN: huacong 2016.03.17  17:11 modify===========*/
#define TAG_TRADE_DEBITCREDIT                       "FF9075"
#define TAG_TRADE_CARDINOUT                         "FF9076"
#define TAG_TRADE_TIME                         "FF9079"


#define TAG_TOTAL_IN_DEBIT_NUM                      "FF9039"   //内卡交易借记总笔数
#define TAG_TOTAL_IN_DEBIT_AMOUNT                   "FF9037"     //内卡结算借记总额
#define TAG_TOTAL_IN_CREDIT_NUM                     "FF9019"    //内卡交易贷记总笔数
#define TAG_TOTAL_IN_CREDIT_AMOUNT                  "FF9017"       //内卡结算贷记总额

#define TAG_TOTAL_OUT_DEBIT_NUM                     "FF901A"    //外卡交易借记总笔数
#define TAG_TOTAL_OUT_DEBIT_AMOUNT                  "FF9036"       //外卡结算借记总额
#define TAG_TOTAL_OUT_CREDIT_NUM                    "FF903A"     //外卡交易贷记总笔数
#define TAG_TOTAL_OUT_CREDIT_AMOUNT                 "FF9016"        //外卡结算贷记总额

#define TAG_TRADE_SETTLEEQU                         "FF9015"        //内卡对账结果
#define TAG_TRADE_OUTSETTLEEQU                         "FF900B"        //外卡对账结果


#define TAG_8583_OLDTYPE                         "FF902B"        //原交易ID

#define  TAG_TRADE_UPADD                "FF906E"
#define  TAG_TRADE_TOTALUPNUM           "FF906F"
#define  TAG_TRADE_UPMODE               "FF906D"

#define TAG_TRADE_NEEDREUP              "FF906C"        //?????????
/*====================== END======================== */

#define TAG_TRADE_UPLOAD                          "FF907C"

#define TAG_IntpCard                                "03"
#define TAG_CardRF                                "02"
#define TAG_CardICC                                "01"
#define TAG_CardMAG                               "00"

#define TAG_NUBSIX                                   "6"
#define TAG_NUBFOR                                "4"
#define TAG_NUBSTR                                "3"
#define TAG_NUBTWO                                 "2"
#define TAG_NUBONE                                "1"
#define TAG_NUBZER                              "0"

#define NET_UNCONNECT                         0x03  //网络未连接
#define NET_INTERRUT                          0x02  //通信中断
#define NET_TIMEOUT                           0x01  //超时
#define NET_OK                                0x00  //正常通行
                            
#define TYRDE_REVERRSAL                            1  //冲正
#define  TYRDE_SCRIPTL                               2  //脚本

#define SDK_8583_FIELD_MSG				0
#define SDK_OK                       (1)
#define SDK_EQU                      (0)        //比较大小相等时用这个//use this if it's equal when compare size
#define SDK_ERR                      (-1)
#define SDK_TIME_OUT                 (-2)       //超时//timeout
#define SDK_ESC                      (-3)
#define SDK_PARA_ERR                 (-4)
#define SDK_FUN_NULL                 (-5)       //函数体为空
/*=======BEGIN: tjb 2014.06.03  14:52 modify===========*/
#define SDK_EBUSY                    (-6)       /* 设备忙 */
#define SDK_EIO                      (-7)       /* 设备未打开 */
#define SDK_EDATA                    (-8)       /* 数据溢出或数据错误 */
#define SDK_LOD                       (100)    //继续调用接口
#define SDK_MAKE                      (99)
enum COMMUERROR_RETURN
{
    ERR_TPK        = (SDK_APP_RET_ERR_MAX + (-1)),                     //TPK错误
    ERR_TAK        = (SDK_APP_RET_ERR_MAX + (-2)),       //TAK错误
    ERR_TDK        = (SDK_APP_RET_ERR_MAX + (-3)),      //TDK错误
    ERR_MSGTYPE = (SDK_APP_RET_ERR_MAX + (-4)),       //消息类型不符
    ERR_PROCESSCODE = (SDK_APP_RET_ERR_MAX + (-5)),               //处理码不符
    ERR_VOUCHERNO     = (SDK_APP_RET_ERR_MAX + (-6)),          //凭证号不符
    ERR_TRADEAMOUNT  = (SDK_APP_RET_ERR_MAX + (-7)),          //交易金额不符
    ERR_BATCHNO  = (SDK_APP_RET_ERR_MAX + (-8)),      //批次号不符
    ERR_TERMINALNO    = (SDK_APP_RET_ERR_MAX + (-9)),        //终端号不符
    ERR_MERCHANTNO = (SDK_APP_RET_ERR_MAX + (-10)),       //商户号不符
    ERR_CONDITIONCODE  = (SDK_APP_RET_ERR_MAX + (-11)),        //条件码不符
    ERR_CURRENCYCODE = (SDK_APP_RET_ERR_MAX + (-12)),  //货币代码不符
    ERR_UNIONPAYID    = (SDK_APP_RET_ERR_MAX + (-13)),    //32域格式错
    ERR_MISSMAC         = (SDK_APP_RET_ERR_MAX + (-14)),                                       //缺少MAC域
    ERR_MISSBALANCE  = (SDK_APP_RET_ERR_MAX + (-15)),                                          //缺少查询余额域
    ERR_MISSACC           = (SDK_APP_RET_ERR_MAX + (-16)),                                     //缺少主帐号
    ERR_MISSAUTHCODE = (SDK_APP_RET_ERR_MAX + (-17)),                                          //缺少授权码
    ERR_CARDDES            = (SDK_APP_RET_ERR_MAX + (-18)),                                            //卡加密错
    ERR_RECZERO            = (SDK_APP_RET_ERR_MAX + (-19)),                                    //接收长度为零
    ERR_FLUSHWK             = (SDK_APP_RET_ERR_MAX + (-20)),                                                   //更新密钥失败
    ERR_PIN                       = (SDK_APP_RET_ERR_MAX + (-21)),                                   //PIN错误  zjz20100423
    ERR_CALCMAC              = (SDK_APP_RET_ERR_MAX + (-22)),                     //计算MAC错误
    ERR_NEEDREVERSAL     = (SDK_APP_RET_ERR_MAX + (-23)),                             //需要冲正
    ERR_RESPONSE             = (SDK_APP_RET_ERR_MAX + (-24)),                     //错误响应码
    ERR_RESPONSE10         = (SDK_APP_RET_ERR_MAX + (-25)),                         //响应码10
    ERR_SCRIPT      = (SDK_APP_RET_ERR_MAX + (-26)), //脚本失败//zxx 20130802 11:18 只有圈存交易才有可以返回这个值
};


#define TAG_TYPE_N 		0
#define TAG_TYPE_CN 		1
#define TAG_TYPE_OTHER	2

#define	MAX_TAG_LIST_SIZE	128

typedef struct _TAG_TYPE
{
	u16 Tag;
	u8   Type;
} TAG_TYPE;
typedef struct _TAG_INFO
{
	u16 Tag;
	u32 Length;
	u8 *Value;
} TagItem;

typedef struct _TAG_LIST
{
	u32 ItemCount;
	TagItem tagItem[MAX_TAG_LIST_SIZE]; //FIXME:this is not satisfied, should be fix later.
	u32 duplicateItemCount;
	u16 duplicateItem[MAX_TAG_LIST_SIZE];
} TagList;
/*
#define bool char
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
*/
#define bool int
#define true 1
#define false 0
#define TRUE 1
#define FALSE 0
#define	TAGMODE_DUPLICATE_ITEM_ALLOWED	 	0x01

#define	TAGMODE_RESERVE_ALL_ITEM			0x02

/*

#define 	TAG_ADF_NAME						0x4F

#define 	TAG_APP_LABEL 						0x50

#define	TAG_TRACK2							0x57

#define	TAG_PAN							0x5A

#define	TAG_ENTRY_TEMPLATE				0x61

#define	TAG_FCI_TEMPLATE					0x6F

#define	TAG_TEMPERATE_70					0x70

#define	TAG_TEMPERATE_71					0x71

#define	TAG_TEMPERATE_72					0x72

#define	TAG_DIRECTORY_TEMPLATE			0x73

#define	TAG_AMOUNT_BINARY				0x81

#define 	TAG_AIP 							0x82

#define 	TAG_DF_NAME 						0x84

#define	TAG_SCRIPT							0x86

#define	TAG_APP_PRIOR						0x87

#define 	TAG_SFI							0x88

#define	TAG_RESPONSE_CODE				0x8A

#define	TAG_CDOL1							0x8C

#define	TAG_CDOL2							0x8D

#define	TAG_CVM							0x8E

#define	TAG_CA_PUBLIC_KEY_INDEX			0x8F

#define	TAG_ISSUER_PK_CERTIFICATE		0x90

#define	TAG_ISSUER_AUTH_DATA				0x91

#define	TAG_ISSUER_PK_REMAINDER			0x92

#define	TAG_SDA_SSAD						0x93

#define 	TAG_AFL 							0x94

#define	TAG_TVR							0x95

#define	TAG_TDOL							0x97

#define	TAG_TDOL_HASH_VALUE				0x98

#define	TAG_TRANSACTION_DATE				0x9A

#define	TAG_TSI							0x9B

#define	TAG_TRANSACTION_TYPE				0x9C

#define	TAG_DDF_NAME						0x9D

#define	TAG_FCI_PROPRIETARY_TEMPLATE	0xA5

#define	TAG_CARDHOLDER_NAME				0x5F20

#define	TAG_APP_EXPIRATION_DATE			0x5F24

#define	TAG_APP_EFFECTIVE_DATE			0x5F25

#define	TAG_ISSUER_COUNTRY_CODE			0x5F28

#define	TAG_TRANS_CURRENCY_CODE			0x5F2A

#define	TAG_PREFERRED_LANGUAGE			0x5F2D

#define	TAG_SERVICE_CODE					0x5F30

#define 	TAG_PAN_SEQU_NUMBER				0x5F34

#define	TAG_TRANS_CURRENCY_EXP			0x5F36

#define	TAG_ACCOUNT_TYPE					0x5F57

#define	TAG_ACQUIRER_ID					0x9F01

#define 	TAG_AMOUNT						0x9F02

#define	TAG_AMOUNT_OTHER					0x9F03

#define	TAG_AMOUNT_OTHER_BINARY			0x9F04

#define	TAG_TERMINAL_AID					0x9F06

#define	TAG_AUC							0x9F07

#define	TAG_ICC_APP_VERSION_NUMBER		0x9F08

#define	TAG_APP_VERSION_NUMBER			0x9F09

#define 	TAG_IAC_DEFAULT					0x9F0D

#define 	TAG_IAC_DENIAL					0x9F0E

#define 	TAG_IAC_ONLINE					0x9F0F

#define	TAG_ISSUER_APP_DATA				0x9F10

#define	TAG_ISSUER_CODE_INDEX			0x9F11

#define	TAG_PREFERRED_NAME				0x9F12

#define	TAG_LAST_ATC						0x9F13

#define	TAG_LOWER_OFFLINE_LIMIT			0x9F14

#define	TAG_MERCHANT_CATEGORY_CODE		0x9F15

#define	TAG_MERCHANT_ID					0x9F16

#define 	TAG_PIN_TRY_COUNTER 				0x9F17

#define	TAG_SCRIPT_ID						0x9F18

#define	TAG_TERM_COUNTRY_CODE			0x9F1A

#define	TAG_FLOOR_LIMIT					0x9F1B

#define	TAG_TERMINAL_ID					0x9F1C

#define	TAG_TERMINAL_RISK_DATA			0x9F1D

#define	TAG_IFD_SERIAL_NUMBER			0x9F1E

#define	TAG_TRANSACTION_TIME				0x9F21

#define	TAG_UPPER_OFFLINE_LIMIT			0x9F23

#define	TAG_CRYPTOGRAM					0x9F26

#define	TAG_CRYPTOGRAM_INFO_DATA		0x9F27

#define 	TAG_PIN_CERTIFICATE				0x9F2D

#define 	TAG_PIN_EXPONENT					0x9F2E

#define 	TAG_PIN_PK_REMAINDER				0x9F2F

#define	TAG_ISSUER_PK_EXPONENT			0x9F32

#define	TAG_TERMINAL_CAPABILITIES		0x9F33

#define	TAG_CVM_RESULTS					0x9F34

#define	TAG_TERMINAL_TYPE					0x9F35

#define 	TAG_ATC							0x9F36

#define	TAG_UNPREDICTABLE_NUMBER		0x9F37

#define	TAG_PDOL							0x9F38

#define	TAG_POS_ENTRY_MODE				0x9F39

#define	TAG_TRANS_REF_CURRENCY_CODE	0x9F3C

#define	TAG_TRANS_REF_CURRENCY_EXP		0x9F3D

#define	TAG_ADDITIONAL_TERM_CAPABILITY	0x9F40

#define	TAG_TRANS_SEQ_COUNTER			0x9F41

#define	TAG_APP_CURRENCY_CODE			0x9F42

#define	TAG_DATA_AUTH_CODE				0x9F45

#define	TAG_ICC_PK_CERTIFICATE			0x9F46

#define	TAG_ICC_PK_EXPONENT				0x9F47

#define	TAG_ICC_PK_REMAINDER				0x9F48

#define	TAG_DDOL							0x9F49

#define	TAG_SDA_TAGLIST					0x9F4A

#define	TAG_DDA_SDAD						0x9F4B

#define	TAG_DYNAMIC_DATA					0x9F4C

#define	TAG_LOG_ENTRY						0x9F4D

#define	TAG_MERCHANT_LOCATION			0x9F4E

#define 	TAG_LOG_FORMAT 					0x9F4F

#define	TAG_VLP							0x9F7A

#define	TAG_FCI_ISSUER_DATA				0xBF0C

#define TAG_CREDENTIAL_NUMBER				0x9F61

#define TAG_CREDENTIAL_TYPE					0x9F62

*/


//EC define, end
u32 GetTagCount(TagList *pTagList);
void InitTagList(TagList *pTagList);
void FreeTagList(TagList *pTagList);
void appendTagList(TagList *sourceTagList, TagList *descTagList);
u32 BuildTagList(u8 *buf, u32 size, TagList *pTagList);
u32 BuildTagListInMode(u8 *buf, u32 size, u32 mode, TagList *pTagList);
void PrintOutTagList(TagList *pTagList, char* promoptMsg);
bool TagIsExisted(TagList *pTagList, u16 tag);
bool TagDataIsMissing(TagList *pTagList, u16 tag);
bool TagIsDuplicate(TagList *pTagList, u16 tag);
u32 AppendTagValue(u16 tag, u8* pValue, u32 length, TagList *pTagList);
u32 SetTagValue(u16 tag, u8* pValue, u32 length, TagList *pTagList);
u8* GetTagValue(TagList *pTagList, u16 tag);
u32 GetTagValueSize(TagList *pTagList, u16 tag);
bool tagIsInDOL(TagList *tagList, u16 dolType, u16 targetTag);
u32 BuildDOLToStream(TagList *tagList, u16 dolType, u8 *resultStream, u8 *streamLength);
u32 ParseTlvLengthInfo(u8 *pLenStartPos, u16 size, u16 *pLength, u16 *pLengthSize);
u32 BuildTagListOneLevel(u8 *buf, u32 size, TagList *pTagList);
void ResetDuplicateStatus(TagList *pTagList);
void removeTag(TagList *pTagList, u16 tag);

#endif

