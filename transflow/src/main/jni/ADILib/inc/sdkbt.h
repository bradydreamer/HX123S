#ifndef _SDK_BT_H_
#define _SDK_BT_H_

#define SDK_BT_NAME_LEN     16

/* ������״̬ */
#define SDK_BT_STATUS_IDLE      1       /* Idle ģʽ(δ����) */
#define SDK_BT_STATUS_WORKING   2       /* �����շ����� */
#define SDK_BT_STATUS_WAITING   3       /* ����״̬(�����ӣ��������շ�) */

/* Զ���豸��Ϣ */
typedef struct _sdk_st_BtRemote {
    unsigned char m_rssi;          //�����豸�źų�ǿֵ
    unsigned char m_addr[6];       //�豸��ַ��6�ֽ�HEX
    unsigned char m_name[64];      //�豸����
} SDK_BT_REMOTE_INFO;

/* ����豸��Ϣ */
typedef struct _sdk_st_BtPairList {
    unsigned char m_num;             //������豸���� 
    unsigned char m_addr[4][6];      //�豸��ַ��6�ֽ�HEX���յĵ�ַΪȫ0
    unsigned char m_name[4][64];     //�豸����
} SDK_BT_PAIR_INFO;

typedef enum _sdk_bt_pin_mode {
    SDK_BT_PIN_CODE = 0,        //�������������
    SDK_BT_JUST_WORK = 1,       //���������������
    SDK_BT_PIN_MODE_ERR = 2,    //���������������
} SDK_BT_PIN_MODE;

/*****************************************************************************
** Descriptions:	�������豸
** Parameters:    	Input: None 
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸æ              SDK_EBUSY	
** Created By:		tjb  2014.05.26
** Remarks: 		�豸�򿪺�Ĭ��Ϊ���ɼ�״̬����Ҫ����sdkBtSetDiscoverable����Ϊ�ɼ�
*****************************************************************************/
int sdkBtOpen(void);

/*****************************************************************************
** Descriptions:	�ر������豸
** Parameters:    	Input: None 
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtClose();

/*****************************************************************************
** Descriptions:	�Ͽ���ǰ������
** Parameters:    	const char *pszMac  ��Ҫ�Ͽ���Զ�������豸MAC��ַ
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO	
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtDisconnect(const u8 *pszMac);

/*****************************************************************************
** Descriptions:	������������
** Parameters:    	const u8 *pszData       ����д�������
                    u32 nBufLen             ����д�����ݵĳ���
                    u32 nTimeout            �������ݳ�ʱʱ�䣬��λ���롣��Ϊ0����
                                            ��ʱȡ��
** Returned value:	�ɹ�����ʵ�ʷ��͵����ݴ�С
                    ʧ��                SDK_ERR
                    ��������            SDK_PARAERR
                    �豸δ�򿪻��д��  SDK_EIO
                    ��ʱ����            SDK_TIME_OUT
** Created By:		tjb  2014.05.26
** Remarks: 		����������ʽ�ģ�ֱ�����������ݷ������
*****************************************************************************/
int sdkBtWrite(const u8 *pszData, u32 nBufLen, u32 nTimeout);

/*****************************************************************************
** Descriptions:	������ȡ����
** Parameters:    	u8 *pszOut          ������ȡ���ݵĳ���
                    u32 nBufLen         ��ȡ������
                    u32 nTimeout        �������ݳ�ʱʱ�䣬��λ���롣��Ϊ0����
                                        ��ʱȡ��
** Returned value:	�ɹ�����ʵ�ʶ��������ݴ�С
                    ʧ��                SDK_ERR
                    ��������            SDK_PARAERR
                    �豸δ�򿪻��д��  SDK_EIO	
                    ��ʱ����            SDK_TIME_OUT
** Created By:		tjb  2014.05.26
** Remarks: 		������Ҫ��ȡ��Ԥ��ָ����uiExOutLen���ȵ����ݣ�����Ҫ��
                    uiTimeout��Ϊ0��������һֱ��ȡ��ֱ��������
                    ��û��Ԥ�����ݳ��ȣ����������С��uiTimeoutֵ�����ٺ���
                    ��������
*****************************************************************************/
int sdkBtRead(u8 *pszOut, u32 nBufLen, u32 nTimeout);

/*****************************************************************************
** Descriptions:	��ȡ������״̬
** Parameters:    	Input: None 
** Returned value:	�ɹ�����������״̬: 
                        SDK_BT_STATUS_IDLE      1   //Idle ģʽ(δ����)
                        SDK_BT_STATUS_WORKING   2   //�����շ�����
                        SDK_BT_STATUS_WAITING   3   //����״̬(�����ӣ��������շ�)
                    ʧ��                SDK_ERR
                    �豸δ��          SDK_EIO	
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtGetStatus();

/*****************************************************************************
** Descriptions:	��ȡ�������Ƴ���汾��Ϣ(��'\0'������ASCII�ַ�������Ч�ַ����ȹ̶�Ϊ17Bytes)
** Parameters:    	u8 *pszVer        ����汾��Ϣ���ַ���
                    int nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO	
** Created By:		tjb  2014.05.26
** Remarks: 		��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
int sdkBtGetVersion(u8 *pszVer, u32 nLen);

/*****************************************************************************
** Descriptions:	��ȡ����ģ��̼��汾(��'\0'������ASCII�ַ�������Ч�ַ����ȹ̶�Ϊ17Bytes)
** Parameters:    	u8 *pszMVer       ����汾��Ϣ���ַ���
                    u32 nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
int sdkBtGetMVersion(u8 *pszMVer, u32 nLen);

/*****************************************************************************
** Descriptions:	��ȡ����MAC��ַ, ���ȹ̶�Ϊ6�ֽ�
** Parameters:    	u8 *pszAddr       �����ַ���ַ���
                    u32 nLen            �ַ����ĳ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		��ȡ��������������������δ����״̬��ɡ����������״̬��
                    �����᷵�����ʧ��
*****************************************************************************/
int sdkBtGetMacAddr(u8 *pszAddr, u32 nLen);

/*****************************************************************************
** Descriptions:	��ȡ�������������(PIN)����'\0'������ASCII�ַ�����4��6λ����
** Parameters:    	u8 *pszPin          ����PIN���ַ���
                    int nLen            �ַ������ȣ�����С��6
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		PINֵΪ4��6λ�����ַ���
*****************************************************************************/
int sdkBtGetPin(u8 *pszPin, u32 nLen);

/*****************************************************************************
** Descriptions:	�����������������(PIN)
** Parameters:    	u8 *pszPin        
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		PINֵΪ4��6λ�����ַ���
*****************************************************************************/
int sdkBtSetPin(u8 * pszPin);

/*****************************************************************************
** Descriptions:	��ȡ�������豸����
** Parameters:    	u8 *pszName
                    u32 nLen
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		�豸����Ϊ64λ���ڵ��ַ���
*****************************************************************************/
int sdkBtGetName(u8 *pszName, u32 nLen);

/*****************************************************************************
** Descriptions:	�����������豸����
** Parameters:    	u8 *pszName
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO	
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtSetName(u8 * pszName);

/*****************************************************************************
** Descriptions:	��ȡ����豸����Ϣ
** Parameters:    	SDK_BT_PAIR_INFO *pstPariInfo
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		�����Ϣ��Զ���豸��Ϣ�У�"�豸����"��G2��G3ʹ�õ�ģ����
                    ��֧��
*****************************************************************************/
int sdkBtGetPairDevInfo(SDK_BT_PAIR_INFO *pstPariInfo);

/*****************************************************************************
** Descriptions:	ɾ������豸����Ϣ
** Parameters:    	u32 nIndex
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		����ֵ�ķ�Χ0~3
*****************************************************************************/
int sdkBtDelPairDevInfo(u32 nIndex);

/*****************************************************************************
** Descriptions:	���������豸�ܷ񱻷���
** Parameters:    	bool bDiscover   true ���Ա����֣�fasle ���ɷ���
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtSetDiscoverable(bool bDiscover);

/*****************************************************************************
** Descriptions:	��ȡԶ���豸����Ϣ
** Parameters:    	SDK_BT_REMOTE_INFO *pstRemote Զ���豸����Ϣ
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		tjb  2014.05.26
** Remarks: 		
*****************************************************************************/
int sdkBtGetRemoteInfo(SDK_BT_REMOTE_INFO *pstRemote);

/*****************************************************************************
** Descriptions:	��ȡ��������ģʽ
** Parameters:
** Returned value:	�ɹ�����            0 �� 1
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		zjp  2014.12.11
** Remarks: 		E_SDK_BT_PIN_CODE:Ϊ PIN CODEPIN CODEģʽ��������������룩
                    E_SDK_BT_JUST_WORK:Ϊ JUST WORKģʽ������������룩
*****************************************************************************/
SDK_BT_PIN_MODE sdkBtGetPinMode();

/*****************************************************************************
** Descriptions:	������������ģʽ
** Parameters:    	E_SDK_BT_PIN_MODE ePinMode:
                    E_SDK_BT_PIN_CODE:Ϊ PIN CODEPIN CODEģʽ��������������룩
                    E_SDK_BT_JUST_WORK:Ϊ JUST WORKģʽ������������룩
** Returned value:	�ɹ�����            SDK_OK
                    ʧ��                SDK_ERR
                    �豸δ�򿪻��д��  SDK_EIO
** Created By:		zjp  2014.12.11
** Remarks:
*****************************************************************************/
int sdkBtSetPinMode(SDK_BT_PIN_MODE ePinMode);

#endif
