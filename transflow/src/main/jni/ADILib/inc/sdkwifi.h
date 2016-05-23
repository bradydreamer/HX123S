#ifndef _SDK_WIFI_H_
#define _SDK_WIFI_H_

#include "sdkGlobal.h"


#define SDK_WIFI_MAX_SSID_LENGTH        63
#define SDK_WIFI_MAX_PWD_LENGTH         63
#define SDK_WIFI_MAX_NAME_LENGTH        63

/* WIFI ��֤���ͣ�ĿǰWIFIģ�鲻֧��WPA��WPA2��ҵ��֤ģʽ */
enum SDK_EXT_WIFI_PSK {
    SDK_WIFI_PSK_OPEN = 0,
    SDK_WIFI_PSK_WEP,            //WEP��֤��ʽ
    SDK_WIFI_PSK_WPAPSK,         //WPA_PSK��֤��ʽ
    SDK_WIFI_PSK_WPA2PSK,        //WPA2_PSK
    SDK_WIFI_PSK_WPACCKM,        //WPA_CCKM(Cisco Centralized Key Management)����EAPϵ�е���ҵ��֤
    SDK_WIFI_PSK_DEFAULT,        //Ĭ����֤��ʽ
};

/* WIFI ���ܷ�ʽ */
enum SDK_EXT_WIFI_CIPHER {
    SDK_WIFI_CIPHER_NONE = 0x00,
    SDK_WIFI_CIPHER_WEP = 0x01,
    SDK_WIFI_CIPHER_TKIP = 0x02,
    SDK_WIFI_CIPHER_WEP128 = 0x04,
    SDK_WIFI_CIPHER_AES = 0x08,
    SDK_WIFI_CIPHER_DEFAULT =
    SDK_WIFI_CIPHER_WEP | SDK_WIFI_CIPHER_TKIP | SDK_WIFI_CIPHER_WEP128 | SDK_WIFI_CIPHER_AES,
};

typedef enum {
    SDK_WIFI_DEVICE_UNAVAILABLE = -8,       //�豸������
    SDK_WIFI_PARA_ERR = -7,       //������
    SDK_WIFI_CONNECT_ERR = -6,       //�޷�����
    SDK_WIFI_CONNECTING = -5,       //ͨѶ������,PPP������
    SDK_WIFI_LOGOFF = -4,       //ͨѶ��·�Ͽ�
    SDK_WIFI_CANCEL = -3,       //����ȡ��
    SDK_WIFI_TIMEOUT = -2,       //��ʱ
    SDK_WIFI_FAIL = -1,       //ʧ��
    SDK_WIFI_SUCCESS = 0,       //�ɹ�
    SDK_WIFI_LINK_OK = 1        //ͨѶ��·OK
} SDK_WIFI_STATUS;


/*WIFI��ȫ����ģʽ*/
typedef enum {
    SDK_WIFI_SEC_NONE, /**��ʹ�ð�ȫ����ģʽ*/
            SDK_WIFI_SEC_WEP_OPEN, /**ʹ�ÿ�����Կ��WEPģʽ*/
            SDK_WIFI_SEC_WEP_SHARED, /**ʹ�ù�����Կ��WEPģʽ*/
            SDK_WIFI_SEC_WPA, /**ʹ��WPAģʽ*/
            SDK_WIFI_SEC_WPA2, /**ʹ��WPA2ģʽ*/
            SDK_WIFI_SEC_WPA_PSK, /**ʹ��WPA-PSKģʽ*/
            SDK_WIFI_SEC_WPA2_PSK            /**ʹ��WPA2-PSKģʽ*/
} SDKWIFISECTYPE;

/**WIFI�ȵ���Ϣ�ṹ��:*/
typedef struct {
    unsigned char SSID[64];
    /*WIFI�ȵ�����*/
    int SecMode;            /*��ȫ����ģʽ���μ�enum SDKWIFISECTYPE*/
} SDKWIFIINFO;

#if 0
#define WIFI_MAX_SSID_LENGTH        SDK_WIFI_MAX_SSID_LENGTH
#define WIFI_MAX_PWD_LENGTH         SDK_WIFI_MAX_PWD_LENGTH

#define WIFI_PSK_OPEN       SDK_WIFI_PSK_OPEN
#define WIFI_PSK_WEP        SDK_WIFI_PSK_WEP
#define WIFI_PSK_WPAPSK     SDK_WIFI_PSK_WPAPSK
#define WIFI_PSK_WPA2PSK    SDK_WIFI_PSK_WPA2PSK
#define WIFI_PSK_WPACCKM    SDK_WIFI_PSK_WPACCKM
#define WIFI_PSK_DEFAULT    SDK_WIFI_PSK_DEFAULT

#define WIFI_CIPHER_NONE        SDK_WIFI_CIPHER_NONE
#define WIFI_CIPHER_WEP         SDK_WIFI_CIPHER_WEP
#define WIFI_CIPHER_TKIP        SDK_WIFI_CIPHER_TKIP
#define WIFI_CIPHER_WEP128      SDK_WIFI_CIPHER_WEP128
#define WIFI_CIPHER_AES         SDK_WIFI_CIPHER_AES
#define WIFI_CIPHER_DEFAULT     SDK_WIFI_CIPHER_DEFAULT
#endif


//IP��WIFIͨѶ����IP��Ϣ
typedef struct {
    char IP[16];
    /*����IP��ַ*/
    char Gateway[16];
    /*��������*/
    char Mask[16];
    /*������������*/
    char Dns1[16];
    /*DNS*/
    char Dns2[16];
    /*DNS*/
    char MAC[18];      /*MAC��ַ,��ʽ��00-23-5A-15-99-42*/
} SDKTERMINALIPINFO;

typedef struct _sdk_ext_wifi_info {
    char ssid[SDK_WIFI_MAX_SSID_LENGTH + 1];
    /* ssid�� */
    char pwd[SDK_WIFI_MAX_PWD_LENGTH + 1];
    /* ��ȫ���� */
    int iAuth;
    /* ��֤����: ��� enum SDK_EXT_WIFI_PSK */
    int iCipher;
    /* �����㷨: ��� enum SDK_EXT_WIFI_CIPHER */
    int iDhcp;
    /*�Ƿ��Զ���ȡ��ַ*/
    char cLocalIp[16];
    /*����IP*/
    char cGateWay[16];
    /*��������*/
    char cMask[16];
    /*��������*/
    char cDns1[16];
    /*Ĭ��dns*/
    char cDns2[16];             /*����dns*/
} SDK_EXT_WIFI_INFO;

/*****************************************************************************
** Descriptions:	��WIFI�豸����ʼ����ص�����
** Parameters:    	void
** Returned value:	SDK_WIFI_DEVICE_UNAVAILABLE = -8, //�豸������
                    SDK_WIFI_FAIL     = -1,       //ʧ��
                    SDK_WIFI_SUCCESS  =  0        //�ɹ�

** Created By:		tjb  2014.07.02
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIOpen(void);

/*****************************************************************************
** Descriptions:	�ر�WIFI�豸
** Parameters:    	void
** Returned value:	SDK_WIFI_DEVICE_UNAVAILABLE  //�豸������
                    SDK_WIFI_FAIL                //ʧ��
                    SDK_WIFI_SUCCESS             //�ɹ�
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIClose(void);

/*****************************************************************************
** Descriptions:	��ȡ����WIFI�ȵ��б�
** Parameters:    	char **ppszSSIDList     WIFI�ȵ�SSID�б�
                    int *pnNumList          ��������ȡ��WIFI�ȵ����
** Returned value:	ʵ�ʻ�ȡ����WIFI�ȵ����
                    SDK_WIFI_DEVICE_UNAVAILABLE    //�豸������
                    SDK_WIFI_FAIL                  //ʧ��
                    SDK_WIFI_SUCCESS               //�ɹ�
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIGetSSIDList(SDKWIFIINFO WIFIList[], int *pnNumList);

/*****************************************************************************
** Descriptions:	���ӵ�WIFI�ȵ�
** Parameters:    	const WIFICONFIG *st_wificonfig WIFI���Ӳ���
** Returned value:	SDK_WIFI_DEVICE_UNAVAILABLE    //�豸������
                    SDK_WIFI_PARA_ERR              //������
                    SDK_WIFI_CONNECT_ERR           //�޷�����
                    SDK_WIFI_FAIL                  //ʧ��
                    SDK_WIFI_SUCCESS               //�ɹ�
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIConnect(const SDK_EXT_WIFI_INFO *st_wificonfig);

/*****************************************************************************
** Descriptions:	�Ͽ�WIFI�ȵ�����
** Parameters:    	void
** Returned value:	SDK_WIFI_DEVICE_UNAVAILABLE    //�豸������
                    SDK_WIFI_FAIL                  //ʧ��
                    SDK_WIFI_SUCCESS               //�ɹ�
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIDisConnect(void);

/*****************************************************************************
** Descriptions:	���WIFI����״̬
** Parameters:    	void
** Returned value:	SDK_WIFI_CONNECTING        //ͨѶ������
                    SDK_WIFI_LOGOFF            //ͨѶ��·�Ͽ�
                    SDK_WIFI_FAIL              //ʧ��
                    SDK_WIFI_LINK_OK           //ͨѶ��·OK
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFICheck(void);

/*****************************************************************************
** Descriptions:	ʵʱ��ʾWIFI�����ź�ǿ��
** Parameters:    	
** Returned value:	�����ź�ǿ��0-100(100Ϊ��ǿ�ź�)
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFISignal();

/*****************************************************************************
** Descriptions:	WIFIͨѶ����IP��Ϣ
** Parameters:    	SDKTERMINALIPINFO *st_terminalipinfo   IP��Ϣ
** Returned value:	SDK_WIFI_DEVICE_UNAVAILABLE    //�豸������
                    SDK_WIFI_PARA_ERR              //������
                    SDK_WIFI_FAIL                  //ʧ��
                    SDK_WIFI_SUCCESS               //�ɹ�
** Created By:		tjb  2014.07.03
** Remarks: 		
*****************************************************************************/
extern int sdkWIFIIPInfo(SDKTERMINALIPINFO *st_terminalipinfo);

/*****************************************************************************
** Descriptions:	��������
** Parameters:          void
** Returned value:
** Created By:		lqq2013.01.15
** Remarks:
*****************************************************************************/
extern int sdkWIFIReConnect();

#endif
