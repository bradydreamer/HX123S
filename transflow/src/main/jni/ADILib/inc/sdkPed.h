#ifndef _SDK_PED_H_
#define _SDK_PED_H_

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

typedef enum {
    SDK_LAMP_OFF = 0,        //��
    SDK_LAMP_ON,            //��
    SDK_LAMP_TWINK            //��˸
} SDK_PED_LAMP_ONOFF;       //led

typedef enum {
    SDK_PED_LAMP_1 = 1,        //led��1��G2 = eLampBlue
    SDK_PED_LAMP_2,            //led��2��G2 = eLampYellow
    SDK_PED_LAMP_3,            //led��3��G2 = eLampGreen
    SDK_PED_LAMP_4,            //led��4��G2 = eLampRed
} SDK_PED_LAMP;       //led //G2&200P��Q����

typedef struct {
    SDK_PED_LAMP_ONOFF eLamp1;        //eLampBlue;
    SDK_PED_LAMP_ONOFF eLamp2;        //eLampYellow
    SDK_PED_LAMP_ONOFF eLamp3;        //eLampGreen
    SDK_PED_LAMP_ONOFF eLamp4;        //eLampRed
    s32 siTimers;                     //zxx 20131029 11:24 ����ʱ�䣬��λms
} SDK_PED_LAMP_CTRL;


typedef struct {
    SDK_PED_LAMP lempID;            //�Ʊ��
    SDK_PED_LAMP_ONOFF lempState;
    s32 siOnTimers;                   //��˸ʱ��������ʱ��
    s32 siOffTimers;                  //��˸ʱ��Ϩ���ʱ��
    s32 siTimers;                     //��˸ʱ������ʱ�䣬��λms
} SDK_PED_LAMP_CTRL_EXT;//G2&200P��Q����


//���������Ӧ��ʱʱ��//the time keyboard response timeout
#define SDK_PED_TIMEOUT             (4000)                          //��λ: ms  //units : ms

//���󷵻���//error return code
typedef enum {
    SDK_PED_PIN_FORMAT_ERR = (SDK_SYS_RET_ERR_MAX -
                              5),                 //�����ʽ��//password format error
    SDK_PED_TPK_ERR = (SDK_SYS_RET_ERR_MAX - 6),        //PIN��Կ��//PIN error
    SDK_PED_TAK_ERR = (SDK_SYS_RET_ERR_MAX - 7),        //MAC��Կ��//MAC error
    SDK_PED_FLUSHWK_ERR = (SDK_SYS_RET_ERR_MAX - 8),               //д�빤����Կ//write working key
    SDK_PED_NOPIN = (SDK_SYS_RET_ERR_MAX - 9)                  //�����뷵��zhuoquan 20130204
}
        SDK_PED_RETURN;

//�����������//PIN pad language
typedef enum {
    SDK_PED_CH,                                                         //����//Chinese
    SDK_PED_EN                                                          //Ӣ��//English
} SDK_PED_LANGUAGE;

//���(��/��)   //balance(positive/negative)
typedef enum {
    SDK_PED_POSITIVE,                                                   //����//positive
    SDK_PED_NEGATIVE                                                    //����//negative
} SDK_PED_SYMBOL;


//DES ����//DES type
typedef enum {
    SDK_PED_DES_SINGLE,                                            //DES����//DES type
    SDK_PED_DES_TRIPLE,                                             // 3DES����//3DES type
    SDK_PED_DES_DUKPT,  //zhouzhihua 2013.08.15 15:3 ��ʱ��������
    SDK_PED_SM4                                                     //SM4
} SDK_PED_DES_TYPE;

//���ܻ����//encrypt or decrypt
typedef enum {
    SDK_PED_ENCRYPT,                                                    //����//encrypt
    SDK_PED_DECRYPT                                                     // ����//decrypt
} SDK_PED_CRYPT;

//����PIN �ķ�ʽ//the mode of requesting PIN
typedef enum {
    SDK_PED_IC_ONLINE_PIN,                                              //IC������PIN   //IC card online PIN
    SDK_PED_IC_OFFLINE_PIN,                                             //IC���ѻ�PIN (���ķ�ʽ)  //IC card offline PIN(plaintext)
    SDK_PED_MAG_PIN                                                     //������PIN  //magnetic stripe card PIN
} SDK_PED_PIN_MODE;

//�ӽ����㷨//encryption and decryption algorithm
typedef enum {
    SDK_PED_ECB,                                                        //�й�������׼����MAC�㷨//China UnionPay standard calcuate MAC algorithm
    SDK_PED_CBC,                                                        //ANSI X9.9�����㷨//ANSI X9.9 encryption algorithm
    SDK_PED_X919,                                                        //ANSI X9.19 �����㷨//ANSI X9.19 encryption algorithm
    SDK_PED_MAC_SM4                                                        //SM4�㷨����MAC
} SDK_PED_MAC_MODE;

typedef enum {
    SDK_PED_TMK,
    SDK_PED_WK
} SDK_PED_KEY_TYPE;
//��������
typedef enum {
    SDK_PED_DES_ALL,                                            //ȫ����Կ
    SDK_PED_DES_TOP8,                                              // ǰ8λ
    SDK_PED_DES_MIDDLE8,                                        //��8λ,��8B��Կ��Ч����16B��ԿΪ��8B
    SDK_PED_DES_LAST8                                              //��8λ����24B��Կ��Ч
} SDK_PED_CRYPT_WAY;
/*=======BEGIN: zhouzhihua 2013.08.21  10:19 modify===========*/
//ͬʱ����Key and Ksn��1:ֻ����key��2:ֻ����ksn
typedef enum {
    SDK_PED_DUKPT_UPDATE_ALL = 0,
    SDK_PED_DUKPT_UPDATE_DKEY = 1,
    SDK_PED_DUKPT_UPDATE_KSN = 2

} SDK_PED_DUKPT_INIT_KEY_MODE;
//��ʼkey������0:mackey ,1: pinkey ,2: dkey
typedef enum {
    SDK_PED_DUKPT_MAC_KEY = 0,
    SDK_PED_DUKPT_PIN_KEY = 1,
    SDK_PED_DUKPT_DB_KEY = 2

} SDK_PED_DUKPT_INIT_KEY_TYPE;
typedef enum {
    SDK_PED_DUKPT_GET_PIN_RELEASE = 0, //����ģʽ
            SDK_PED_DUKPT_GET_PIN_DEBUG = 1//����ģʽֻҪ����һ������
} SDK_PED_DUKPT_GET_PIN_MODE;

typedef enum {
    SDK_PED_DUKPT_GET_MAC_OLD = 0, //ʹ�þɵ�key
            SDK_PED_DUKPT_GET_MAC_NEW = 1//ʹ���µ�key
} SDK_PED_DUKPT_GET_MAC_MODE;

//����PIN ���ò����ṹ��//request PIN configuration parameters struct
typedef struct {
    SDK_PED_PIN_MODE ePinMode;                                          //����PIN�ķ�ʽ//the mode of requesting PIN
    SDK_PED_DES_TYPE eKeyType;                                          //��Կ���ͣ�DES��3DES  //key type, DES of 3DES
    u8 ucTpkIndex;                                                      //����PIN ��TPK ����0-124   //the TPK index for encrypting PIN, 0-124
    u8 hePan[8];                                                        //�����˺ż��ܣ�8�ֽ�PAN Format���������˺ż��ܣ���Ϊȫ0  //encryption with primary account number: 8 bytes PAN Format; encryption without primary account number: all zero
    u8 ucRow;                                                           //���������������ʱ����������кţ������������ʱ�˲�����������//echo password line number when inlay PIN pad inputting, this parameter is no use when outlay PIN pad
    s32 iTimeOut;                                                       //��ʱʱ�䵥λms   //timeout units: ms
    u8 uiBuf[32];                                                       //SM4�㷨��PAN��Ҫ16λ��ǰ�ߵ�����̫С��Ϊ�˼��ݣ��ڽṹ������ӱ���
    void *pVar;                                                         //Ԥ��һ����ָ��
} SDK_PED_PIN_CFG;


typedef struct {
    SDK_PED_DES_TYPE eTmkType;                                          //����Կ���ͣ�DES��3DES   //master key type, DES or 3DES
    u8 ucTmkIndex;                                                      //����Կ����(0-100)  //master key index (1-100)
    SDK_PED_DES_TYPE eWkType;                                           //������Կ���ͣ�DES��3DES   //working key type, DES or 3DES
    /*
        bOnlyCalcCheckValue trueʱ�������κ�����      the index is no use when bOnlyCalcCheckValue is true
     */
    /*ԭ�г�������ϵ�ȱ�ݵ���
       ���������Կ����Ϊ3des,wk���ĳ�����Ч�����ݳ��ȱ���Ϊ16
       ���������Կ����Ϊ3des,wk���ĳ�����Ч�����ݳ��ȱ���Ϊ8 */
    /*Defects in the original program design lead to
       if working key type is 3des, then working key ciphertext length and check data length must be 16
       if working key type is des,   then working key ciphertext length and check data length must be 8 */
    u8 ucWkIndex;                                                       //�洢wk������(0-124)  //the index of storing wk (0-124)
    u8 ucEnWkLen;                                                       //wk���ĳ���//wk ciphertext length
    u8 heEnWk[24];                                                      //wk����//wk ciphertext
    u8 ucCheckDataLen;                                                  //wkУ�����ݳ���//wk check data length
    u8 heCheckData[24];                                                 //wkУ������//wk check data
    u8 ucCheckValueLen;                                                 //Ч��ֵ����   //check value length
    /*
        bOnlyCalcCheckValue trueʱΪ������̷��ص�Ч��ֵ   when bOnlyCalcCheckValue is true, it's check value returned from PIN pad
        falseʱΪ������Ҫ���бȶԵĽ���ֵ                       when false, it's afferent need contrast check value
     */
    u8 heCheckValue[24];                                                //У��ֵ//check value
    bool bOnlyCalcCheckValue;
    /*����ͨ������Կ�⹤����Կ,Ȼ�󽫹�����Կ�����Ч��ֵ�洢.
       ������Կͬʱ����ʱ,����bOnlyCalcCheckValue ĳ��Ϊtrueĳ��Ϊfalse,
       ����һ���ܵõ�����Ҫ�Ľ��*/
    /*just accroding to master key to decrypt working key, then store the checkvalue calcuated by working key.
       when configure multigroup keys at the same time, bOnlyCalcCheckValue cannot be different in different groups,
       or you may not get the result you want	*/
} SDK_PED_WK_CFG;

typedef enum {
    SDK_PED_INPUT_PHONE = 0,        //�����ֻ���,ģʽ��������󳤶Ƚ��������ƣ�ֻҪ�û����������λ��С�ڵ���11λ�����������
    SDK_PED_INPUT_TITLE_NUM,        //��Data�����ʾ��������ʾ��LCD��һ�У�Ȼ����밴������״̬���ȴ��û����룬�6λ
} SDK_PED_INPUT_STR;
/*****************************************************************************
** Descriptions:	��ȡPED ��汾��  Get PED library version number
** Parameters:          u8 *pasVer
** Returned value:
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
//s32 sdkPEDGetVersion(u8 *pasVer);  //fusuipu modefied at 2013-1-29
/*****************************************************************************
** Descriptions:	�ж����Ƿ�����������
** Parameters:
** Returned value:true:������������
                            false:������������
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
bool sdkPEDIsWithPinpad(void);

/*****************************************************************************
** Descriptions:
** Parameters:          1. iTmkIndex: �洢����Կ������ //the index of storing the master key
                               0-99
                        2. eKeyType: ��Կ����  // key type
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]     //DES key [8 bytes]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]  //3DES key [16 bytes]
                        3. pheKey: ��Կֵ  //key value
                        4. iTimeout: ��ʱʱ�� ������100ms    //timeout is not less than 100ms
** Returned value:
                        SDK_OK: �ɹ�//success
                        SDK_PARA_ERR: ��������//parameters error
                        SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)   //PIN pad response timeout(units: ms)
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateTmk(s32 iTmkIndex, SDK_PED_DES_TYPE eKeyType, const u8 *pheKey, s32 iTimeout);

/*****************************************************************************
** Descriptions:	���¹�����Կ//update working key
** Parameters:          const SDK_WK_CFG *pstWkCfg ������Կ���ýṹ//working key configuration struct
                               const u8 siWknum������Կ����//set key number
                               u32 siTimeout��ʱʱ�䲻����100ms��λms  //timeout is not less than 100ms (units: ms)
** Returned value:
** Created By:		//lqq 2012.11.23 10:57
** Remarks:
   ����Կ 0-99
   ������Կ 0-124
   ԭ�г�������ϵ�ȱ�ݵ���
    ���������Կ����Ϊ3des,wk���ĳ�����Ч�����ݳ��ȱ���Ϊ16
    ���������Կ����Ϊdes,wk���ĳ�����Ч�����ݳ��ȱ���Ϊ8
    ���򷵻ز�������
   master key 0-99
   working key 0-124
   Defects in the original program design lead to
    if working key type is 3des, then working key ciphertext length and check data length must be 16
    if working key type is des,   then working key ciphertext length and check data length must be 8
    or return parameters error
*****************************************************************************/
s32 sdkPEDUpdateWk(SDK_PED_WK_CFG *pstWkCfg, const s32 iWknum, s32 iTimeout);

/*****************************************************************************
** Descriptions:	����mac    //calculate mac
** Parameters:          s32 iKeyIndex tak���� :0-124   //index:0-124
                               SDK_PED_MAC_MODE eMode: ecb ,cbc,x919
                               SDK_PED_DES_TYPE eKeyType:des����      //des type
                               const u8 *pheIn:���������ݳ��Ȳ���8λ��8λ  //the length of data need to be calculated, make up to 8 bit if it's less than 8 bit
                               s32 iInLen:�������ݳ���  //calculate data length
                               u8 *pheOut:8�ֽ�mac��  //8 bytes mac
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDCalcMac(s32 iKeyIndex, SDK_PED_MAC_MODE eMode, SDK_PED_DES_TYPE eKeyType, const u8 *pheIn,
                  s32 iInLen, u8 *pheOut);

/*****************************************************************************
** Descriptions:	��������  //enter password
** Parameters:          const SDK_PED_PIN_CFG *pstPinCfg ������������   //password attribute configuration
                               u8 *pheOut  ����������ݣ���һ���ֽڳ��ȣ���߸�ʵ����Ҫ����//password related data, first byte is length, following actual data
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDInputPIN(const SDK_PED_PIN_CFG *pstPinCfg, const u8 *pbcAmt, u8 *pheOut);

/*****************************************************************************
** Descriptions:	��ʾ���//display amount
** Parameters:          const u8 *pbcAmt 8�ֽ�bcd    //6 bytes bcd
** Returned value:	SDK_OK�����������쳣  //SDK_Ok is normal, other value is abnormal
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDDispAmt(SDK_PED_SYMBOL eSymbol, const u8 *pbcAmt);

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_PED_BALANCE_SYMBOL eSymbol:
                                SDK_PED_POSITIVE: ���Ϊ��ֵ//balance is positive value
                                SDK_PED_NEGATIVE: ���Ϊ��ֵ//balance is negative value

                               const u8 *pbcBalance:��8 �ֽ��Ҷ���ѹ��BCD�룩//balance(8 bytes right-aligned compressed BCD)
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDDispBalance(SDK_PED_SYMBOL eSymbol, const u8 *pbcBalance);

/*****************************************************************************
** Descriptions: �������ȡ�� //cancel PIN pad
** Parameters:          void
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
void sdkPEDCancel(void);

/*****************************************************************************
** Descriptions:	����Կ�ӽ�������  //master key encrypt and decrypt data
** Parameters:          s32 iTmkIndex
                               SDK_PED_DES_TYPE eTmkType
                               SDK_PED_CRYPT eCrypt
                               const u8 *pheIn
                               u8 *pheOut
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDTmkDes(s32 iTmkIndex, SDK_PED_DES_TYPE eTmkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn,
                 u8 *pheOut);

/*****************************************************************************
** Descriptions:        �������滻����Կ( �¾���Կ����һ��Ϊ�滻)
** Parameters:          1. iOldTmkIndex: ������Կ������(��������Կ)0-99
                        2. eOldKeyType: ������Կ����(��������Կ)
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]
** Parameters:          3. iNewTmkIndex: �洢������Կ������(��������Կ)0-99
                        4. eNewKeyType: ������Կ����(��������Կ)
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]  
                        5. pheKey: ����Կ��������
                        6. iTimeout: ��ʱʱ�� ������100ms
** Returned value:
                        SDK_OK: �ɹ�
                        SDK_PARA_ERR: ��������
                        SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)
** Created By:	        ���ƽ 2014-08-08
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateTmkEx(s32 iOldTmkIndex, SDK_PED_DES_TYPE eOldKeyType, s32 iNewTmkIndex,
                      SDK_PED_DES_TYPE eNewKeyType, const u8 *pheKey, s32 iTimeout);//G2&200P��Q����


/*****************************************************************************
** Descriptions:	��չ���¹�����Կ�����ô�У�����ݣ������ܾ͹���
** Parameters:          pstWkCfg->eTmkType
                        pstWkCfg->ucTmkIndex
                        pstWkCfg->eWkType
                        pstWkCfg->ucWkIndex
                        pstWkCfg->ucEnWkLen;
                        pstWkCfg->heEnWk;
                        //�ṹ�������ɲ��ô���
                        eCryptWay:���ڽ��ܵ�����Կ�������ͣ���16�ֽڣ���ֻ��ǰ8�ֽ�
** Returned value:
** Created By:		zxx 20130513 17:43
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateWkEx(const SDK_PED_WK_CFG *pstWkCfg, SDK_PED_CRYPT_WAY eCryptWay);

/*****************************************************************************
** Descriptions:	������Կ�ӽ���  //working key encryption and decryption
** Parameters:          s32 iWkIndex
                               SDK_PED_DES_TYPE eWkType
                               SDK_PED_CRYPT eCrypt
                               const u8 *pheIn
                               u8 *pheOut
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDWkDes(s32 iWkIndex, SDK_PED_DES_TYPE eWkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn,
                u8 *pheOut);

/*****************************************************************************
** Descriptions:	��չ�������des����
** Parameters:          s32 iIndex:����
                               SDK_PED_KEY_TYPE eKeyType:����Կ���߹�����Կ
                               SDK_PED_DES_TYPE eKeyDesType:����Կ���߹�����Կdes����
                               SDK_PED_CRYPT eCrypt:���ܻ��߽���
                               SDK_PED_CRYPT_WAY eCryptWay:���ڼ��ܵ���Կ��������
                               const u8 *pheIn:����������
                               u16 usInLen:���ݳ��ȣ�����8�ı����������������
                               u8 *pheOut:��ȡ��������
** Returned value:
** Created By:		shijianglong  2013.05.10
** Remarks:
*****************************************************************************/
s32 sdkPEDDesEx(s32 iIndex, SDK_PED_KEY_TYPE eKeyType, SDK_PED_DES_TYPE eKeyDesType,
                SDK_PED_CRYPT eCrypt, SDK_PED_CRYPT_WAY eCryptWay, const u8 *pheIn, u16 usInLen,
                u8 *pheOut);

/*****************************************************************************
** Descriptions:	��ȡ������̰汾  //get PIN pad version
** Parameters:          u8 *pasVer ��ȡ���İ汾��//version number
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDGetPedVersion(u8 * pasPedVer);
/*****************************************************************************
** Descriptions:	��ȡ������̻����//get PIN pad number
** Parameters:          u8 *pasSn
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDGetPedSn(u8 * pasSn);

/*****************************************************************************
** Descriptions:	���������������//set PIN pad language
** Parameters:          SDK_PED_LANGUAGE eLanguage
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDSetLanguage(SDK_PED_LANGUAGE eLanguage);

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pasStr ����ʾ�ַ���//character string to be displayed
                               u8 ucRow ����ʾ���ص�����ƫ��//pixels to be displayed longitudinal offset
                               u8 ucCol����ʾ���ص����ƫ��//pixels to be displayed lateral offset
                               u8 ucAtr���뷽ʽ,���·�ʽ���������//alignment, following type can be xor processed
                                0x00   //����(Ĭ������)  //normal display(default)
                                0x01   //����//reverse display
                                0x02   //����һ��//insert a column
                                0x04   //�����//left-aligned
                                0x08   //����//center
                                0x10   //�Ҷ���//right-aligned
** Returned value:
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
s32 sdkPEDDisplayStr(const u8 *pasStr, s32 ucRow, s32 ucCol, u32 ucAtr);

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pbcAmtOut ����Ľ��(8�ֽ��Ҷ���BCD)    //the entered amount(8 bytes right-aligned BCD)
** Returned value:
                          SDK_OK: �ɹ�//success
                          SDK_PARA_ERR: ��������//parameters error
                          SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)    //PIN pad response timeout (units: ms)
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
s32 sdkPEDInputAmt(u8 * pbcAmtOut);

/*****************************************************************************
** Descriptions:	��ʼ����ԿоƬ//initialize the key chip
** Parameters:          void
** Returned value:
                        SDK_OK: �ɹ�//success
                        SDK_ERR: ʧ��//failed
                        SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)    //PIN pad response timeout (units: ms)
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
s32 sdkPEDInitKeyIC(void);

s32 sdkPEDSaveUserData(const u8 *pDataIn, const s32 ucDataLen);

s32 sdkPEDReadUserData(u8 *pDataOut, const s32 ucOutLen);

/*****************************************************************************
** Descriptions:   �����ʼ����
** Parameters:
                   u8 ucIndex ����
                   u8 *pucKey ��ʼkey DK
                   u16 uiKeyLen ���Գ���
                   u8 *pucKsn ksn
                   u16 uiKsnLen ksn�ĳ���
                   s32 siTimer  ��ʱʱ��
** Returned value:
                    ����0	�����ɹ�
                    SDK_ESC	�û�ȡ��
                    SDK_PARA_ERR	��������
                    SDK_ESC	�û�ȡ��
                    SDK_TIME_OUT	��ʱ�˳�
                    SDK_ERR	����ʧ��
** Created By:		zhouzhihua  2013.07.24
** Remarks:
*****************************************************************************/
s32 sdkPEDDukptLoadAndInit(u8 ucIndex, const u8 *pucKey, u16 uiKeyLen, const u8 *pucKsn,
                           u16 uiKsnLen, s32 siTimer);

/*****************************************************************************
** Descriptions:        DUKPT��ȡPIN����

** Parameters:          u8 ucMode 00 ����ģʽ��0x01 ����ģʽ
                        u8 ucIndex ����
                        u8 *pucPan 6Bytes BCD �����ʺż���
                        u16 uiMinLen ������С����
                        u16 uiMaxLen ������󳤶�
                        u8 *pucPinBlock ���ܺ������
                        s32 siTimer ��ʱ��
** Returned value:	����0 ksn�ĳ��ȣ�С�ڵ���0 ʧ��
** Created By:		zhouzhihua  2013.07.29
** Remarks:             Ŀǰ��������ʺż���
*****************************************************************************/
s32 sdkPEDDukptGetPin(u8 ucMode, u8 ucIndex, const SDK_PED_PIN_CFG *pstPinCfg, u8 uiMinLen,
                      u8 uiMaxLen, u8 *pucPinBlock, u8 *pucKsn, u8 *pucKsnLen, s32 siTimer);

/*****************************************************************************
** Descriptions:    DUKPT��ȡmac
** Parameters:          u8 ucMode 0x00 ��key �� 0x01��key
                               u8 ucIndex  ����
                               u8 *pucInData ���ݱ���Ϊ ASCII
                               u16 uiDataLen ���ݳ���
                               u8 *pucMac �����mac
                               u8 *pucKsn
                               s32 siTimer
** Returned value:	����0 ksn�ĳ��� ��ȷ��С��0:ʧ�ܣ�ʧ�ܣ���ʱ���û�ȡ������������
** Created By:		zhouzhihua  2013.07.30
** Remarks:
*****************************************************************************/
s32 sdkPEDDukptGetMac(u8 ucMode, u8 ucIndex, const u8 *puascInData, u16 uiDataLen, u8 *pucMac,
                      u8 *pucKsn, u8 *pucKsnLen, s32 siTimer);

/*****************************************************************************
** Descriptions:	    POS�ǽ�led��
** Parameters:          const SDK_PED_LAMP_CTRL *pstLampCtrl �Ը��ƵĿ��Ʒ���
** Returned value:
                        SDK_PARA_ERR	��������
                        SDK_ERR	����ʧ��
                        SDK_COMM_UART_NOTOPEN	������̴���δ�򿪣�����������̲��У�
                        SDK_COMM_UART_NOESIST	������̴��ںŲ�����
                        SDK_OK	�򿪳ɹ�
** Created By:		    shiweisong  2013.08.28
** Remarks:
*****************************************************************************/
s32 sdkPEDLampControl(const SDK_PED_LAMP_CTRL *pstLampCtrl);

/*****************************************************************************
** Descriptions:	����LCD��������������չ�����������û�������˸Ƶ��
** Parameters:    	u32 ledNmb Ҫ���ƵƵĸ���
                    const SDK_PED_LAMP_CTRL_EXT pstLampCtr[] ������Ϣ����
** Returned value:	SDK_PARA_ERR	��������
                    SDK_ERR	����ʧ��
                    SDK_COMM_UART_NOTOPEN	������̴���δ�򿪣�����������̲��У�
                    SDK_COMM_UART_NOESIST	������̴��ںŲ�����
                    SDK_OK	�򿪳ɹ�
** Created By:		���ƽ  2014.07.16
** Remarks: 		
*****************************************************************************/
s32 sdkPEDLampControlExt(u32 ledNmb, const SDK_PED_LAMP_CTRL_EXT pstLampCtr[]);//G2&200P��Q����
/*****************************************************************************
** Descriptions:	��ָ���Ĺ���A��Կ����B��Կ�������Ϊ�µĹ�����Կ���������
                    newKeyIndexλ��(���������Կ)
** Parameters:    	SDK_PED_DES_TYPE eSrKeyType:A��Կ��������
                    s32 siSrKeyIndex:A��Կ����
                    SDK_PED_DES_TYPE eTgKeyType:B��������
                    s32 siTgKeyIndex:B��Կ����
                    u8 newKeyIndex:����Կ����
** Returned value:	
** Created By:		���ƽ  2014.09.24
** Remarks: 		����Կ����Կ������B��Կ����һ��
*****************************************************************************/
s32 sdkPedWkEncryptWk(SDK_PED_DES_TYPE eAWKDesType, u8 siAWKIndex,
                      SDK_PED_DES_TYPE eBWKDesType, u8 siBWKIndex, u8 newWKIndex);//G2&200P��Q����
/*****************************************************************************
** Descriptions:	��ȡ������������ַ���
** Parameters:    	u8 *pTitle      ��Ҫ�������������ʾ�ı���
                    u8 *pOut        ����������̷��ص��û�������ַ���
                    s32 siTimeOut   ��ʱʱ�䣬0��ʾ������ʱ
                    void *pVar      Ԥ������
** Returned value:	
** Created By:		fusuipu  2014.11.13
** Remarks: 		��ȡ�����˳�
                    ��ʱʱ��Ϊ0��ʾ������ʱ
*****************************************************************************/
s32 sdkPEDGetInputStr(SDK_PED_INPUT_STR eInputMode, u8 *pOut, s32 siTimeOut, void *pVar);


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif //_SDK_PED_H_

