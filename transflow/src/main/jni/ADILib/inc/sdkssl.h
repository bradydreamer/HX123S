#ifndef SDK_SSL_H
#define SDK_SSL_H

#include "sdkGlobal.h"

//SSL���ֵ�Э��
typedef enum {
    SDK_SSL_TLSV1,                  //TLSv1
    SDK_SSL_V2,                     //SSLv2
    SDK_SSL_V3,                     //SSLv3
    SDK_SSL_V23                     //SSLv2/3
} SDK_SSL_PROTOCOL;

//SSL���ݽṹ
typedef struct _sdk_ext_ssl_st {
    SDK_SSL_PROTOCOL protocal;
    /* SSL���ֵ�Э�� */
    const u8 *pasCaFile;
    /* ����CA֤�飬����ȫ·�� */
    const u8 *pasSend;       /* ��Ҫ���͵����� */
    u8 szSerIP[16];    /* �������ĵ�ַ */
    u8 *pasRecv;
    /* �������ݵĻ����� */
    int uiLen;          /* �������ݻ������Ĵ�С */
    u32 uiTimeout;      /* ��ʱʱ��, ��λΪ���� */
    u32 uiPort;         /* �������˿ں� */
    s32 run_flag;       /* ����״̬�����Բ����� */
} SDK_EXT_SSL_ST;

//��չ��SSL���ݽṹ
typedef struct _sdk_ext_ssl_st_ex {
    SDK_SSL_PROTOCOL protocal;
    /* SSL���ֵ�Э�� */
    const u8 *pasCaFile;
    /* ����CA֤�飬����ȫ·�� */
    const u8 *pasSend;       /* ��Ҫ���͵����� */
    u8 szSerIP[16];    /* �������ĵ�ַ */
    u8 *pasRecv;
    /* �������ݵĻ����� */
    int uiLen;          /* �������ݻ������Ĵ�С */
    u32 uiTimeout;      /* ��ʱʱ��, ��λΪ���� */
    u32 uiPort;         /* �������˿ں� */
    s32 run_flag;
    /* ����״̬�����Բ����� */
    int uiSendLen;
    /* ��Ҫ���͵����ݵĳ���(���ݿ�����16���Ƶģ������Ҫ���볤��)*/
    void *pReserve;
    /* Ԥ������ʱ���� */
    void *pReserve2;     /* Ԥ������ʱ���� */
} SDK_EXT_SSL_ST_EX;

//֤���ʽ
#define SDK_CURL_PEM_TYPE       0   // PEM  ��ʽ��֤��
#define SDK_CURL_DER_TYPE       1   // DER  ��ʽ��֤��
#define SDK_CURL_P12_TYPE       3   // PKCS#12-encoded files ��ʽ��֤�� (OpenSSL versions 0.9.3 and later ֧��)

//�߳����н��ֵ
#define SDK_EXT_PTHREAD_RUNNING     0   //�߳���������
#define SDK_EXT_PTHREAD_EXIT        1   //�߳������˳���
#define SDK_EXT_PTHREAD_ERROR       2   //�̴߳����˳���
#define SDK_EXT_PTHREAD_TIMEOUT     3   //�̳߳�ʱ�˳���

#define URL_MAX_LEN            1024
#define CA_PATH_MAX_LEN        256

//HTTPS���ݽṹ
typedef struct _sdk_ext_curl_data {
    bool bMethod;
    /* ��ȡ��ʽ true:ʹ��GET��ʽ��false ʹ��POST��ʽ*/
    const u8 *psURL;             /* ��������ַ */
    u16 nPort;
    /* �������˵Ķ˿ں� */
    const u8 *psSend;            /* �������ݵĻ��� */
    u32 nSendLen;           /* �������ݵĴ�С*/
    u8 *psRecv;            /* �������ݵĻ����� */
    u32 nRecvLen;
    /* ���ջ����������ֵ, (����CURL���壬������ֵΪ16K) */
    const u8 *psCaFile;          /* CA֤����ļ�����������·����ΪNULLʱ�������з���������֤ */
    u32 nTimeout;
    /* ��ʱʱ�䣬��λ���� */

    const u8 *psCliCert;
    /* ��Ҫ˫����֤ʱ���ͻ��˵�֤�� */
    const u8 *psCliPwd;
    /* ��Ҫ˫����֤ʱ���ͻ��˵����� */
    int nCertType;          /* ֤��ĸ�ʽ, SDK_CURL_PEM_TYPE��SDK_CURL_DER_TYPE��SDK_CURL_P12_TYPE  */
} SDK_EXT_CURL_DATA;

/*****************************************************************************
** Descriptions:	ͨ��httpsЭ���ȡ����
** Parameters:    	SDK_EXT_CURL_DATA *pstData  ����ýṹ���˵��
** Returned value:	SDK_OK-�ɹ���SDK_ERR-ʧ�ܣ�SDK_TIME_OUT-��ʱ��SDK_PARA_ERR-��������
** Created By:		tjb  2014.06.23
** Remarks: 		
*****************************************************************************/
extern int sdkExtCurl(SDK_EXT_CURL_DATA *pstData);

/*****************************************************************************
** Descriptions:	ͨ��SSL��������������ݺͽ�������
** Parameters:    	SDK_EXT_SSL_ST *pSsl_st      SSLͨ�ŵĲ��������SDK_EXT_SSL_ST    
** Returned value:	SDK_PARA_ERR - ��������
                    SDK_TIME_OUT - ��ʱ����
                    SDK_ERR      - ʧ�ܣ�
                    SDK_OK       - �ɹ�
** Created By:		tjb  2014.03.21
** Remarks: 		
*****************************************************************************/
extern int sdkExtSslPerform(SDK_EXT_SSL_ST *pSsl_st);

/*****************************************************************************
** Descriptions:	ͨ��SSL��������������ݺͽ�������
** Parameters:    	SDK_EXT_SSL_ST_EX *pSsl_st      SSLͨ�ŵĲ��������SDK_EXT_SSL_ST    
** Returned value:	SDK_PARA_ERR - ��������
                    SDK_TIME_OUT - ��ʱ����
                    SDK_ERR      - ʧ�ܣ�
                    SDK_OK       - �ɹ�
** Created By:		tjb  2014.03.21
** Remarks: 		
*****************************************************************************/
extern int sdkExtSslPerformEx(SDK_EXT_SSL_ST_EX *pSsl_st);


#ifndef SDK_SSL_HANDLE
#define SDK_SSL_HANDLE      void *
#endif

/*SSLͨѶʱ���ص�ֵ*/
#define SDK_SSL_OK                 0       //�ɹ�
#define SDK_SSL_ERR                -1      //��������
#define SDK_SSL_PARAM_ERR          -2      //������
#define SDK_SSL_LOAD_CA_FAIL       -3      //����CAʧ��
#define SDK_SSL_LOAD_CERT_FAIL     -4      //����֤��ʧ��
#define SDK_SSL_LOAD_KEY_FAIL      -5      //����˽Կʧ��
#define SDK_SSL_SET_CIPHER_FAIL    -6      //�����㷨�׼�ʧ��
#define SDK_SSL_CONNECT_FAIL       -7      //SSL����ʧ��
#define SDK_SSL_NOCONNECT          -8      //SSL��δ����
#define SDK_SSL_SEND_FAIL          -9      //SSL��������ʧ��
#define SDK_SSL_RECV_FAIL          -10     //SSL��������ʧ��
#define SDK_SSL_TIMEOUT            -11     //SSLͨѶ��ʱ
#define SDK_SSL_CERT_HAS_EXPIRED   -12     //�Է�֤���ѹ���
#define SDK_SSL_CERT_NOT_YET_VALID -13     //�Է�֤�黹δ��Ч
#define SDK_SSL_NONSUPPORT         -14     //��֧�ֵ��ļ���ʽ

/* SSL��Э�� */
typedef enum {
    SDK_PROTOCOL_SSLv2,     //SSL�汾2.0
    SDK_PROTOCOL_SSLv3,     //SSL�汾3.0
    SDK_PROTOCOL_TLSv1,     //TLS�汾1.0
    SDK_PROTOCOL_TLSv1_1,   //TLS�汾1.1
    SDK_PROTOCOL_TLSv1_2,    //TLS�汾1.2
    SDK_PROTOCOL_SSLv23,    //SSL23
} SDK_SSL_ProtocolType;

/*֤���ʽ*/
typedef enum {
    SDK_SSL_FILE_DER,  //DER (ASN1) �ļ���ʽ 
    SDK_SSL_FILE_PEM   //PEM (BASE64) �ļ���ʽ 
} SDKSSLFileFormat;

/*�����㷨*/
typedef enum {
    SDK_CIPHER_RSA_NULL_MD5, /*�����׼�-RSA_NULL_MD5*/
            SDK_CIPHER_RSA_NULL_SHA, /*�����׼�-RSA_NULL_SHA*/
            SDK_CIPHER_RSA_RC4_40_MD5, /*�����׼�-RSA_RC4_40_MD5*/
            SDK_CIPHER_RSA_RC4_128_MD5, /*�����׼�-RSA_RC4_128_MD5*/
            SDK_CIPHER_RSA_RC4_128_SHA, /*�����׼�-RSA_RC4_128_SHA*/
            SDK_CIPHER_RSA_RC2_40_MD5, /*�����׼�-RSA_RC2_40_MD5*/
            SDK_CIPHER_RSA_IDEA_128_SHA, /*�����׼�-IDEA_128_SHA*/
            SDK_CIPHER_RSA_DES_40_CBC_SHA, /*�����׼�-DES_40_CBC_SHA*/
            SDK_CIPHER_RSA_DES_64_CBC_SHA, /*�����׼�-DES_64_CBC_SHA*/
            SDK_CIPHER_RSA_DES_192_CBC3_SHA, /*�����׼�-DES_192_CBC3_SHA*/
            SDK_CIPHER_DH_RSA_DES_192_CBC3_SHA, /*�����׼�-DH_DSS_DES_192_CBC3_SHA*/
            SDK_CIPHER_DH_DSS_DES_40_CBC_SHA, /*�����׼�-DH_DSS_DES_40_CBC_SHA*/
            SDK_CIPHER_DH_DSS_DES_64_CBC_SHA, /*�����׼�-DH_DSS_DES_64_CBC_SHA*/
            SDK_CIPHER_DH_DSS_DES_192_CBC3_SHA, /*�����׼�-DH_DSS_DES_192_CBC3_SHA*/
            SDK_CIPHER_DH_RSA_DES_40_CBC_SHA, /*�����׼�-DH_RSA_DES_40_CBC_SHA*/
            SDK_CIPHER_DH_RSA_DES_64_CBC_SHA, /*�����׼�-DH_RSA_DES_64_CBC_SHA*/
            SDK_CIPHER_EDH_DSS_DES_40_CBC_SHA, /*�����׼�-EDH_DSS_DES_40_CBC_SHA*/
            SDK_CIPHER_EDH_DSS_DES_64_CBC_SHA, /*�����׼�-EDH_DSS_DES_64_CBC_SHA*/
            SDK_CIPHER_EDH_DSS_DES_192_CBC3_SHA, /*�����׼�-EDH_DSS_DES_192_CBC3_SHA*/
            SDK_CIPHER_EDH_RSA_DES_40_CBC_SHA, /*�����׼�-EDH_RSA_DES_40_CBC_SHA*/
            SDK_CIPHER_EDH_RSA_DES_64_CBC_SHA, /*�����׼�-EDH_RSA_DES_64_CBC_SHA*/
            SDK_CIPHER_EDH_RSA_DES_192_CBC3_SHA, /*�����׼�-EDH_RSA_DES_192_CBC3_SHA*/
            SDK_CIPHER_ADH_RC4_40_MD5, /*�����׼�-ADH_RC4_40_MD5*/
            SDK_CIPHER_ADH_RC4_128_MD5, /*�����׼�-ADH_RC4_128_MD5*/
            SDK_CIPHER_ADH_DES_40_CBC_SHA, /*�����׼�-ADH_DES_40_CBC_SHA*/
            SDK_CIPHER_FZA_DMS_NULL_SHA, /*�����׼�-FZA_DMS_NULL_SHA*/
            SDK_CIPHER_CK_FZA_DMS_FZA_SHA, /*�����׼�-CK_FZA_DMS_FZA_SHA*/
            SDK_CIPHER_CK_FZA_DMS_RC4_SHA, /*�����׼�-CK_FZA_DMS_RC4_SHA*/
            SDK_CIPHER_CK_ADH_DES_64_CBC_SHA, /*�����׼�-CK_ADH_DES_64_CBC_SHA*/
            SDK_CIPHER_CK_ADH_DES_192_CBC_SHA     /*�����׼�-CK_ADH_DES_192_CBC_SHA*/
} SDKCipherSuite;

//Ϊ�˼���Ӧ�ô���ĸĶ������Բ������¶��壬����һ��ת��
#if 0
#define USSL_HANDLE         SDK_SSL_HANDLE

#define USSL_OK                 SDK_SSL_OK       
#define USSL_ERR                SDK_SSL_ERR     
#define USSL_PARAM_ERR          SDK_SSL_PARAM_ERR     
#define USSL_LOAD_CA_FAIL       SDK_SSL_LOAD_CA_FAIL      
#define USSL_LOAD_CERT_FAIL     SDK_SSL_LOAD_CERT_FAIL     
#define USSL_LOAD_KEY_FAIL      SDK_SSL_LOAD_KEY_FAIL    
#define USSL_SET_CIPHER_FAIL    SDK_SSL_SET_CIPHER_FAIL
#define USSL_CONNECT_FAIL       SDK_SSL_CONNECT_FAIL
#define USSL_NOCONNECT          SDK_SSL_NOCONNECT
#define USSL_SEND_FAIL          SDK_SSL_SEND_FAIL
#define USSL_RECV_FAIL          SDK_SSL_RECV_FAIL
#define USSL_TIMEOUT            SDK_SSL_TIMEOUT
#define USSL_CERT_HAS_EXPIRED   SDK_SSL_CERT_HAS_EXPIRED
#define USSL_CERT_NOT_YET_VALID SDK_SSL_CERT_NOT_YET_VALID
#define USSL_NONSUPPORT         SDK_SSL_NONSUPPORT

#define USSL_FILE_DER       SDK_SSL_FILE_DER
#define USSL_FILE_PEM       SDK_SSL_FILE_PEM
#define USSLFileFormat      SDKSSLFileFormat

#define UPROTOCOL_SSLv2     SDK_PROTOCOL_SSLv2
#define UPROTOCOL_SSLv3     SDK_PROTOCOL_SSLv3
#define UPROTOCOL_TLSv1     SDK_PROTOCOL_TLSv1
#define UPROTOCOL_TLSv1_1   SDK_PROTOCOL_TLSv1_1
#define UPROTOCOL_TLSv1_2   SDK_PROTOCOL_TLSv1_2
#define UProtocolType       SDK_SSL_ProtocolType

#define CIPHER_RSA_NULL_MD5                 SDK_CIPHER_RSA_NULL_MD5
#define CIPHER_RSA_NULL_SHA                 SDK_CIPHER_RSA_NULL_SHA
#define CIPHER_RSA_RC4_40_MD5               SDK_CIPHER_RSA_RC4_40_MD5
#define CIPHER_RSA_RC4_128_MD5              SDK_CIPHER_RSA_RC4_128_MD5
#define CIPHER_RSA_RC4_128_SHA              SDK_CIPHER_RSA_RC4_128_SHA
#define CIPHER_RSA_RC2_40_MD5               SDK_CIPHER_RSA_RC2_40_MD5
#define CIPHER_RSA_IDEA_128_SHA             SDK_CIPHER_RSA_IDEA_128_SHA
#define CIPHER_RSA_DES_40_CBC_SHA           SDK_CIPHER_RSA_DES_40_CBC_SHA
#define CIPHER_RSA_DES_64_CBC_SHA           SDK_CIPHER_RSA_DES_64_CBC_SHA
#define CIPHER_RSA_DES_192_CBC3_SHA         SDK_CIPHER_RSA_DES_192_CBC3_SHA
#define CIPHER_DH_RSA_DES_192_CBC3_SHA      SDK_CIPHER_DH_RSA_DES_192_CBC3_SHA
#define CIPHER_DH_DSS_DES_40_CBC_SHA        SDK_CIPHER_DH_DSS_DES_40_CBC_SHA
#define CIPHER_DH_DSS_DES_64_CBC_SHA        SDK_CIPHER_DH_DSS_DES_64_CBC_SHA
#define CIPHER_DH_DSS_DES_192_CBC3_SHA      SDK_CIPHER_DH_DSS_DES_192_CBC3_SHA
#define CIPHER_DH_RSA_DES_40_CBC_SHA        SDK_CIPHER_DH_RSA_DES_40_CBC_SHA
#define CIPHER_DH_RSA_DES_64_CBC_SHA        SDK_CIPHER_DH_RSA_DES_64_CBC_SHA
#define CIPHER_EDH_DSS_DES_40_CBC_SHA       SDK_CIPHER_EDH_DSS_DES_40_CBC_SHA
#define CIPHER_EDH_DSS_DES_64_CBC_SHA       SDK_CIPHER_EDH_DSS_DES_64_CBC_SHA
#define CIPHER_EDH_DSS_DES_192_CBC3_SHA     SDK_CIPHER_EDH_DSS_DES_192_CBC3_SHA
#define CIPHER_EDH_RSA_DES_40_CBC_SHA       SDK_CIPHER_EDH_RSA_DES_40_CBC_SHA
#define CIPHER_EDH_RSA_DES_64_CBC_SHA       SDK_CIPHER_EDH_RSA_DES_64_CBC_SHA
#define CIPHER_EDH_RSA_DES_192_CBC3_SHA     SDK_CIPHER_EDH_RSA_DES_192_CBC3_SHA
#define CIPHER_ADH_RC4_40_MD5               SDK_CIPHER_ADH_RC4_40_MD5
#define CIPHER_ADH_RC4_128_MD5              SDK_CIPHER_ADH_RC4_128_MD5
#define CIPHER_ADH_DES_40_CBC_SHA           SDK_CIPHER_ADH_DES_40_CBC_SHA
#define CIPHER_FZA_DMS_NULL_SHA             SDK_CIPHER_FZA_DMS_NULL_SHA
#define CIPHER_CK_FZA_DMS_FZA_SHA           SDK_CIPHER_CK_FZA_DMS_FZA_SHA
#define CIPHER_CK_FZA_DMS_RC4_SHA           SDK_CIPHER_CK_FZA_DMS_RC4_SHA
#define CIPHER_CK_ADH_DES_64_CBC_SHA        SDK_CIPHER_CK_ADH_DES_64_CBC_SHA
#define CIPHER_CK_ADH_DES_192_CBC_SHA       SDK_CIPHER_CK_ADH_DES_192_CBC_SHA

#define UCipherSuite                        SDKCipherSuite
#endif


/*****************************************************************************
** Descriptions:	����һ���µ�SSL���
** Parameters:    	SDK_SSL_HANDLE *pnSSL  SSL���
** Returned value:	SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�

** Created By:		tjb  2014.05.09
** Remarks: 		ͨ��sdkSslNew����һ��SSL�����ͨ��sdkSslFree�ͷ�һ��SSL�����
                    SSL����֮ǰ��Ҫ�ȴ��������ʹ�����ע���ͷž����Դ
*****************************************************************************/
extern int sdkSslNew(SDK_SSL_HANDLE*pnSSL);

/*****************************************************************************
** Descriptions:	�ͷ�SSL�����Դ
** Parameters:    	SDK_SSL_HANDLE nSSL    SSL���
** Returned value:	SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		ͨ��sdkSslNew����һ��SSL�����ͨ��sdkSslFree�ͷ�һ��SSL�����
                    SSL����֮ǰ��Ҫ�ȴ��������ʹ�����ע���ͷž����Դ
*****************************************************************************/
extern int sdkSslFree(SDK_SSL_HANDLE nSSL);

/*****************************************************************************
** Descriptions:	����CA��֤�顢˽Կ
** Parameters:    	SDK_SSL_HANDLE nSSL SSL���
                    const char *pszCA   CA�ļ���
                    const char *pszCert ֤���ļ������������˲���֤pos��������NULL
                    const char *pszKey  ˽Կ�ļ������������˲���֤pos��������NULL
                    int nFormat         CA��֤�顢˽Կ�ļ���ʽ���μ�enum SDKSSLFileFormat
** Returned value:	SDK_SSL_NONSUPPORT        = -14��//��֧�ֵ��ļ���ʽ
                    USSL_LOAD_KEY_FAIL        = -5,  //����˽Կʧ��
                    USSL_LOAD_CERT_FAIL       = -4,  //����֤��ʧ��
                    USSL_LOAD_CA_FAIL         = -3,  //����CAʧ��
                    SDK_SSL_PARAM_ERR         = -2,  //������
                    SDK_SSL_ERR               = -1,  //��������
                    SDK_SSL_OK                =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		
*****************************************************************************/
extern int sdkSslLoadCACertKey(SDK_SSL_HANDLE nSSL, const char *pszCA, const char *pszCert,
                               const char *pszKey, int nFormat);

/*****************************************************************************
** Descriptions:	����SSL���Ӳ���
** Parameters:    	SDK_SSL_HANDLE nSSL    SSL���
                    int nProtocol       SSLЭ��汾���μ�enum SDKProtocolType
                    int nCipher         SSL�����׼����μ�enum SDKCipherSuite
** Returned value:	SDK_SSL_NONSUPPORT           = -14��//��֧�ֵ�Э��汾�������׼�
                    SSL_CTX_set_cipher_list      = -6,  //�����㷨�׼�ʧ��
                    SDK_SSL_PARAM_ERR            = -2,  //������
                    SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		
*****************************************************************************/
extern int sdkSslSetOption(SDK_SSL_HANDLE nSSL, int nProtocol, int nCipher);

/*****************************************************************************
** Descriptions:	��������
** Parameters:    	SDK_SSL_HANDLE nSSL     SSL���
                    const char *pszIPAddr   IP��ַ
                    int nPort               �˿ں�
                    int nTimeout            ��ʱʱ�䣬��λ����ms
** Returned value:	SDK_SSL_CERT_NOT_YET_VALID   = -13��//�Է�֤�黹δ��Ч
                    SDK_SSL_CERT_HAS_EXPIRED     = -12��//�Է�֤���ѹ���
                    SDK_SSL_TIMEOUT              = -11, //SSLͨѶ��ʱ
                    SDK_SSL_NOCONNECT            = -8,  //SSL��δ����
                    SDK_SSL_CONNECT_FAIL         = -7,  //SSL����ʧ��
                    SDK_SSL_PARAM_ERR            = -2,  //������
                    SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		���ӳɹ���������sdkSslDisconnect֮���ٽ����µ�����
*****************************************************************************/
extern int sdkSslConnect(SDK_SSL_HANDLE nSSL, const char *pszIPAddr, int nPort, int nTimeout);

/*****************************************************************************
** Descriptions:	�Ͽ�����
** Parameters:    	SDK_SSL_HANDLE nSSL        SSL���
** Returned value:	SDK_SSL_NOCONNECT            = -8, //SSL��δ����
                    SDK_SSL_PARAM_ERR            = -2,  //������
                    SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		
*****************************************************************************/
extern int sdkSslDisconnect(SDK_SSL_HANDLE nSSL);

/*****************************************************************************
** Descriptions:	��������
** Parameters:    	SDK_SSL_HANDLE nSSL         SSL���
                    const unsigned char *psBuf           �������ݻ���
                    unsigned int  nBufLen       �������ݳ���
** Returned value:	�ɹ�����ʵ�ʷ������ݳ���
                    SDK_SSL_SEND_FAIL            = -9, //SSL��������ʧ��
                    SDK_SSL_NOCONNECT            = -8, //SSL��δ����
                    SDK_SSL_PARAM_ERR            = -2,  //������
                    SDK_SSL_ERR                  = -1,  //��������

** Created By:		tjb  2014.05.09
** Remarks: 		
*****************************************************************************/
extern int sdkSslSend(SDK_SSL_HANDLE nSSL, const unsigned char *psBuf, unsigned int nBufLen);

/*****************************************************************************
** Descriptions:	��������
** Parameters:    	SDK_SSL_HANDLE nSSL        SSL���
                    unsigned char *psBuf             �������ݻ���
                    unsigned int *pnBufLen��        �������ݻ����С
                    int nTimeout            ���ճ�ʱʱ��
** Returned value:	SDK_SSL_TIMEOUT              = -11, //SSLͨѶ��ʱ
                    SDK_SSL_RECV_FAIL            = -10, //SSL��������ʧ��
                    SDK_SSL_NOCONNECT            = -8,  //SSL��δ����
                    SDK_SSL_PARAM_ERR            = -2,  //������
                    SDK_SSL_ERR                  = -1,  //��������
                    SDK_SSL_OK                   =  0   //�ɹ�
** Created By:		tjb  2014.05.09
** Remarks: 		
*****************************************************************************/
extern int sdkSslReceive(SDK_SSL_HANDLE nSSL, unsigned char *psBuf, unsigned int *pnBufLen,
                         int nTimeout);

#endif
