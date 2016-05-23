/********************************Copyright ( c ) ***************************
**                  Shenzhen Xinguodu Technology Co., LTD.
**                        http://www.xinguodu.com
**
**
**********Created By:     ���ƽ
**********Created Date:       2014-3-18
**********Version:            <v1.0 >
**********Description:
**
****************************************************************************/
#ifndef __DEV_H__
#define __DEV_H__


/******************************�������͵Ķ���********************************/

typedef signed int s32;
typedef signed short int s16;
typedef unsigned char u8;   //�޷�����
typedef unsigned short int u16;
typedef unsigned int u32;

typedef char s8;   //�з�����



/********************************ͨ�÷���ֵ�Ķ���**********************************/
#define     DDI_OK               0   //�ɹ�
#define     DDI_ERR              -1      //����
#define     DDI_ETIMEOUT         -2  //��ʱ
#define     DDI_EBUSY            -3  //�豸��æ
#define     DDI_ENODEV           -4  //�豸������
#define     DDI_EACCES           -5  //��Ȩ��
#define     DDI_EINVAL           -6  //������Ч
#define     DDI_EIO              -7      //�豸δ�򿪻��豸��������
#define     DDI_EDATA            -8  //���ݴ���
#define     DDI_EPROTOCOL        -9      //Э�����
#define     DDI_ETRANSPORT       -10     //�������
#define     DDI_READED_MAGCARD   -11     //�ǽӶ����ſ�

/********************************��ӡ��״̬����ֵ�Ķ���**********************************/
#define     DDI_PRINTER_WAITING        1//������
#define     DDI_PRINTER_PRINTING    2//����ӡ��
#define     DDI_PRINTER_NO_PAPER    3//��ȱֽ
#define     DDI_PRINTER_OVER_HEAT    4//������


/********************************�ṹ��Ķ���***********************************/
/*�������Խṹ��*/
typedef struct _strComAttr {
    signed int m_baud;         //�����ʣ�9600��38400��115200
    signed int m_databits;             //����λ��ȡֵ��5��6��7��8
    signed int m_parity;               //У�鷽ʽ��0-��У�飻1-��У�飻2-żУ��
    signed int m_stopbits;     //ֹͣλ��1��2
} strComAttr;

/*��������APDU�ṹ��*/
typedef struct _strCApdu {
    unsigned char m_cmd[4]; //�����֣���ʽ��CLA, INS,P1,P2
    signed int m_lc; // �������ݳ���
    unsigned char m_data[256]; //�������ݻ�����
    signed int m_le; //�������ص����ݳ���
} strCApdu;

/*��ӦAPDU�ṹ��*/
typedef struct _strRApdu {
    signed int m_len; //��Ӧ���ݳ���
    unsigned char m_data[512]; //��Ӧ���ݻ�����
    unsigned char m_sw1; //״̬��1
    unsigned char m_sw2; //״̬��2
} strRApdu;

/*�����������*/
typedef struct _strRect {
    unsigned int m_x0; //�������X����
    unsigned int m_y0; //�������Y����
    unsigned int m_x1; //�����յ�X����
    unsigned int m_y1; //�����յ�Y����
} strRect;

/*ͼ������*/
typedef struct _strPicture {
    unsigned int m_width; //ͼ�����ص���
    unsigned int m_height; //ͼ�����ص�߶�
    unsigned int *m_pic; //ͼ����������
} strPicture;

#define ONE_COLOR_TYPE          0 //��ɫ���
#define PICTURE_TYPE            1 //ͼƬ���
typedef struct _strBackGround {
    unsigned int m_mode; //�������÷�ʽ
    strRect m_rect;   //�������ھ�������
    unsigned int m_purecolor;      //��ɫ����ʽ��aRGB
    strPicture m_pic; //����ͼƬ
} strBackGround;


typedef struct _strRfChipParam {
    unsigned char m_modindex;     //�������
    unsigned char m_impedance;     //����迹
    unsigned char m_typeagain;     //TypeA���� unsigned char m_ typebgain; //TypeB����
    unsigned char m_felicagain;     //FeliCa����
    unsigned char m_rfu[28];     //����
} strRfChipParam;



/*nRGB:������ɫ����ɫ��ʽ��aRGB��ʽ��unsigned    int����8λ��ʾAlpha(�ݲ�֧��)���θ�8λ��ʾRed���δθ�8λ��ʾGreen,��8λ��ʾBlue��
   �ڴ����ݸ�ʽ���£�
   31      23      15        7      0
   Alpha	Red	    Green         Blue
   ����RGBֵ����0x00FF0000  ��0x00FF0000  ��0x00FF0000
   �� 0x00000000  ��0x00FFFFFF*/

/*�����*/
typedef struct _strDot {
    unsigned int m_x; //��X����
    unsigned int m_y; //��Y����
} strDot, point;
typedef struct _strFont {
    unsigned int m_font;             //���壬����ASCII�ַ�
    unsigned int m_ascsize;          //ASCII�ַ��ֺ�
    unsigned int m_nativesize;       //�����ֺ�
} strFont;

typedef struct _strLine {
    unsigned int m_x0; //�����X����
    unsigned int m_y0; //�����Y����
    unsigned int m_x1; //���յ�X����
    unsigned int m_y1; //���յ�Y����
} strLine;


/*��Ч���ص�*/
typedef struct _strTpPoint {
    unsigned int m_x; //��������ĺ�����
    unsigned int m_y; //���������������
    unsigned int m_pressure; //����ѹ��(��������Ч)
} strTpPoint;

/*��Ч���ص㼯��*/
typedef struct _strTpPixels {
    unsigned int m_num;            //��Ч���ص����
    strTpPoint m_point[5];   //��Ч���ص����
    unsigned int m_gesture;           //���ƣ�
} strTpPixels;

/*��ӡ�ı����ƽṹ��*/
typedef struct _strPrnTextCtrl {
    unsigned int m_align;            //���뷽ʽ
    unsigned int m_offset;           //��ƫ��
    unsigned int m_font;             //���壬���� ASCII �ַ�
    unsigned int m_ascsize;          // ASCII �ַ��ֺ�
    unsigned int m_asczoom;          // ASCII �ַ��Ŵ�ʽ
    unsigned int m_nativesize;       //�����ֺ�
    unsigned int m_nativezoom;       //����Ŵ�ʽ
} strPrnTextCtrl;


/*��ӡ�ı����ƽṹ��*/
typedef struct _strPrnCombTextCtrl {
    unsigned int m_x0;                           //����ԭ��ĺ�����
    unsigned int m_y0;                           //����ԭ���������
    unsigned int m_font;                 //���壬����ASCII�ַ�
    unsigned int m_ascsize;              //ASCII�ַ��ֺ�
    unsigned int m_asczoom;              //ASCII�ַ��Ŵ�ʽ
    unsigned int m_nativesize;   //�����ֺ�
    unsigned int m_nativezoom;   //����Ŵ�ʽ
    unsigned char *m_text;                     //��ӡ�ı�����
} strPrnCombTextCtrl;


/*�ź�ǿ�Ⱥ�������*/
typedef struct _strWirelessCsq {
    unsigned char m_rssi; //�ź�ǿ�ȣ���Χ��0~31,99��δ֪��
    unsigned char m_ber; //�����ʣ���Χ��0~7,99��δ֪��
} strWirelessCsq;

/*������վ��Ϣ*/
typedef struct _strWirelessBsE {
    unsigned int m_lac; //С����
    unsigned int m_cellid; //��վ��
    unsigned int m_csq;        //�ź�ǿ��
} strWirelessBsE;

/*��վȺ��Ϣ*/
typedef struct _strWirelessBS {
    unsigned char m_num; //��վ��Ϣ����
    strWirelessBsE m_bse[10]; //������վ��Ϣ
} strWirelessBS;

/*modem���Ų���*/
typedef struct _strModemDialPara {
    signed int m_mode;         //���ŷ�ʽ
    unsigned char m_telnum[3][32]; //���ź��룬��ʽΪ��'\0'�������ַ���
    signed int m_timeout;              //�����ܳ�ʱʱ�䣬��λΪ��
} strModemDialPara;

/*DUKPT ��ʼ������*/
typedef struct _strDukptInitInfo {
    unsigned char m_groupindex;    //��Կ���0~1
    unsigned char m_keyindex;      //��Կ������0~9
    unsigned char m_initkey[24];   //��ʼ��Կ
    unsigned char m_keylen;        //��ʼ��Կ���ȣ��̶�16λ
    unsigned char m_ksnindex;      //ksn�����ţ�Ŀǰ����Կ������Ҫ����һ��
    unsigned char m_initksn[20];   //��ʼksn
    unsigned char m_ksnlen;        //��ʼksn���ȣ��̶�Ϊ10λ
} strDukptInitInfo;

/********************************��Ķ���***********************************/
#define     DDI_POWER_NULL               0//��ص������㣬�ػ�
#define     DDI_POWER_ZERO               1//����磬���0��
#define     DDI_POWER_ONE                2//����磬���1��
#define     DDI_POWER_TWO                3//����磬���2��
#define     DDI_POWER_THREE              4//����磬���3��
#define     DDI_POWER_AD_CHARGE          5//����磬�����
#define     DDI_POWER_AD_NO_CHARGE       6//����磬δ���
#define     DDI_POWER_AD_CHARGE_ONE      7//����磬�����, ���1 ������
#define     DDI_POWER_AD_BATTERY_ERR     8//����磬��ع���

/*���ڿ�������*/
#define     DDI_COM_CTL_BAUD            (0) //���ò�����
#define     DDI_COM_CTL_FLWCTL          (1) //��������

#define     DDI_MAG_CTL_VER             (0) //��ȡ�ſ��Ķ����汾


/*ICC/PSAM��������*/
#define        DDI_ICCPSAM_CTL_VER                        0//��ȡICC/PSAM�豸�汾
#define        DDI_ICCPSAM_CTL_ETUD                    1//ָ����λ��ʼETU
#define        DDI_MEMORY_CTL_POWEROFF                    2//MEMORY���µ����
#define        DDI_SLE4428_CTL_RESET                    3//SLE4428����λ����
#define        DDI_SLE4428_CTL_READ                    4//SLE4428��������
#define        DDI_SLE4428_CTL_WRITE                    5//SLE4428��д����
#define        DDI_SLE4428_CTL_READEC                    6//SLE4428����ʣ������У�����
#define        DDI_SLE4428_CTL_VERIFY                    7//SLE4428������У��
#define        DDI_SLE4428_CTL_UPDATESC                8//SLE4428�������޸�
#define        DDI_SLE4442_CTL_RESET                    9//SLE4442����λ����
#define        DDI_SLE4442_CTL_READ                    10//SLE4442��������
#define        DDI_SLE4442_CTL_WRITE                    11//SLE4442��д����
#define        DDI_SLE4442_CTL_READEC                    12//SLE4442����ʣ������У�����
#define        DDI_SLE4442_CTL_VERIFY                    13//SLE4442������У��
#define        DDI_SLE4442_CTL_UPDATESC                14//SLE4442�������޸�
#define        DDI_AT24CXX_CTL_READ                    15//AT24Cϵ�п�������
#define        DDI_AT24CXX_CTL_WRITE                    16//AT24Cϵ�п�д����
#define        DDI_AT88SCXX_CTL_RESET                    17//AT88SCϵ�п���λ����
#define        DDI_AT88SCXX_CTL_READ                    18//AT88SCϵ�п�������
#define        DDI_AT88SCXX_CTL_WRITE                    19//AT88SCϵ�п�д����
#define        DDI_AT88SCXX_CTL_READEC                    20//AT88SCϵ�п���ʣ������У�����
#define        DDI_AT88SCXX_CTL_VERIFY                    21//AT88SCϵ�п�����У��
#define        DDI_AT88SCXX_CTL_UPDATEEC                22//AT88SCϵ�п������޸�
#define        DDI_IS23SC1604_CTL_RESET                23//IS23SC1604����λ����
#define        DDI_IS23SC1604_CTL_READ                    24//IS23SC1604��������
#define        DDI_IS23SC1604_CTL_WRITE                25//IS23SC1604��д����
#define        DDI_IS23SC1604_CTL_ERASE                26//IS23SC1604����������
#define        DDI_IS23SC1604_CTL_READEC                27//IS23SC1604����ʣ������У�����
#define        DDI_IS23SC1604_CTL_VERIFY                28//IS23SC1604������У��
#define        DDI_IS23SC1604_CTL_UPDATEEC                29//IS23SC1604���޸�����


/*�ǽӿ��Ķ�����������*/
#define     DDI_RF_CTL_VER              (0) //��ȡ�ǽӶ������豸�汾
#define     DDI_RF_CTL_SAK              (1) //��ȡѡ��Ӧ��SAKֵ
#define     DDI_RF_CTL_UID              (2) //��ƬID
#define     DDI_RF_CTL_MF_AUTH          (3) //Mifare����֤
#define     DDI_RF_CTL_MF_READ_RAW      (4) //��ȡԭʼ����������
#define     DDI_RF_CTL_MF_WRITE_RAW     (5) //дԭʼ����������
#define     DDI_RF_CTL_MF_READ_VALUE    (6) //��ȡ��ֵ
#define     DDI_RF_CTL_MF_WRITE_VALUE   (7) //д��ֵ
#define     DDI_RF_CTL_MF_INC_VALUE     (8) //��ֵ����
#define     DDI_RF_CTL_MF_DEC_VALUE     (9) //��ֵ����
#define     DDI_RF_CTL_MF_BACKUP_VALUE  (10) //��ֵ����
#define     DDI_RF_CTL_SET_PARAM        (11) //���÷ǽӿ���оƬ����
#define     DDI_RF_CTL_GET_PARAM        (13) //���ǽӿ���оƬ����

/*��ӡ����������*/
#define     DDI_THMPRN_CTL_VER          (0) //��ȡ������ӡ�汾
#define     DDI_THMPRN_CTL_GRAY         (1) //���ô�ӡ�Ҷ�
#define     DDI_THMPRN_CTL_LINEGAP      (2) //���ô�ӡ�ı��м��
#define     DDI_THMPRN_CTL_COLGAP       (3) //���ô�ӡ�ı��м��

/*������������*/
#define     DDI_KEY_CTL_LONGPRESS           (0) //���ó�����
#define     DDI_KEY_CTL_BKLIGHT             (1) //���ư�������
#define     DDI_KEY_CTL_BKLIGHTTIME         (2) //���ñ���ʱ��
#define     DDI_KEY_CTL_BKLIGHT_CTRL         (3) //���Ƽ��̱�������


/*��ʾ��������*/
#define     DDI_LCD_CTL_SET_BACKGROUND      (0) //����ָ�����򱳾�ɫ
#define     DDI_LCD_CTL_SET_COLOR           (1) //����ǰ��ɫ
#define     DDI_LCD_CTL_SET_FONT            (2) //���õ�ǰ����
#define     DDI_LCD_CTL_SET_LINEWIDTH       (3) //�����߿�
#define     DDI_LCD_CTL_RESOLUTION          (4) //��ȡ��Ļ�ֱ���
#define     DDI_LCD_CTL_INVERT_RECT         (5) //����ָ������
#define     DDI_LCD_CTL_BRIGHT              (6) //������Ļ��������,�ڰ������ڻҶ�
#define     DDI_LCD_CTL_ICON                (7) //����ͼ��
#define     DDI_LCD_CTL_GET_DOTCOLOR        (8) //��ȡָ������ɫ
#define     DDI_LCD_CTL_BKLIGHT             (9) //ʹ��LCD����
#define     DDI_LCD_CTL_BKLIGHTTIME         (10) //����LCD����ʱ��
#define     DDI_LCD_CTL_GET_COLOR           (11) //��ȡ��ǰ��ǰ����ɫ
#define     DDI_LCD_CTL_BKLIGHT_CTRL        (12) //����LCD��������

/*��Ƶ��������*/
/* BEGIN: Modified by wangmingming, 2014/5/20   PN:1 */
#define     DDI_AUDIO_CTL_VER               (0)  //��ȡ��Ƶ�豸�汾
#define     DDI_AUDIO_CTL_VOLUME            (1)  //��������
#define     DDI_AUDIO_CTL_BUZZER            (2)  //���Ʒ���������
#define     DDI_AUDIO_CTL_COMB_PLAY         (3)  //��ϲ���
#define     DDI_AUDIO_CTL_GET_STATUS        (4)  //��ȡ����״̬

//����״̬
#define AUDIO_STATUS_IDLE   (0) //����
#define AUDIO_STATUS_PLAYING   (1) //���ڲ���
#define AUDIO_STATUS_PAUSE  (3)  //��ͣ
/* END:   Modified by wangmingming, 2014/5/20   PN:1 */
/*��������������*/
#define     DDI_TP_CTL_VER                  (0) //��ȡ�������豸�汾
#define     DDI_TP_CTL_MODE                 (1) //���ƴ�����ģʽ

/*����ͨ�ſ�������*/
#define     DDI_WIRELESS_CTL_VER            (0) //��ȡ���߿��Ƴ���汾
#define     DDI_WIRELESS_CTL_MVER           (1) //��ȡ����ģ��̼��汾
#define     DDI_WIRELESS_CTL_SIM            (2) //��ȡSIM����״̬
#define     DDI_WIRELESS_CTL_IMEI           (3) //��ȡģ��IMEI
#define     DDI_WIRELESS_CTL_IMSI           (4) //��ȡSIM��IMSI
#define     DDI_WIRELESS_CTL_CSQ            (5) //��ȡ�ź�ǿ��
#define     DDI_WIRELESS_CTL_BS             (6) //��ȡ��վ��Ϣ
#define     DDI_WIRELESS_CTL_DIALNUM        (7) //���ò��ź���
#define     DDI_WIRELESS_CTL_DUALSIM        (8) //˫��ѡ��
#define     DDI_WIRELESS_CTL_CALL           (9) //���������绰
#define     DDI_WIRELESS_CTL_HUNGUP         (10) //�Ҷ���������

/*MODEMͨ�ſ�������*/
#define     DDI_MODEM_CTL_VER                   (0) //��ȡMODEM���Ƴ���汾
#define     DDI_MODEM_CTL_MVER                  (1) //��ȡMODEM�̼��汾
#define     DDI_MODEM_CTL_LINEINUSE             (2) //���ò������
#define     DDI_MODEM_CTL_DIALTONE              (3) //���ò��������
#define     DDI_MODEM_CTL_ATTLEVEL              (4) //���÷��͵�ƽ
#define     DDI_MODEM_CTL_DTMFTIME              (5) //����DTMF ON/OFFʱ��
#define     DDI_MODEM_CTL_RATE                  (6) //��ȡͨ������
#define     DDI_MODEM_CTL_DIALMODE              (7) //���ò��ŷ�ʽ
#define     DDI_MODEM_CTL_ANSMODE               (8) //���ñ���Ӧ��ʽ
#define     DDI_MODEM_CTL_NOCARRIER             (9) //�����ز���ʧ���һ���ʱ��
/*������Կ��������*/
#define     DDI_INNERKEY_CTL_VER                (0) //��ȡ������Կ�豸�汾
#define     DDI_INNERKEY_CTL_TKEY_INJECT        (1) //����ʱ��Կ���浽ָ����Կ��
#define     DDI_INNERKEY_CTL_TKEY_ENCRYPT       (2) //��ʱ��Կ����
#define     DDI_INNERKEY_CTL_TKEY_DECRYPT       (3) //��ʱ��Կ����
#define     DDI_INNERKEY_CTL_HKEY_ENCRYPT       (4) //˫������8�ֽڼ���
#define     DDI_INNERKEY_CTL_HKEY_DECRYPT       (5) //˫������8�ֽڽ���
#define     DDI_INNERKEY_CTL_KEY_CHECK          (6) //����Ƿ������Կ
#define     DDI_INNERKEY_CTL_KEY_TAK_ENCRYPT_TAK    (7) //ʹ�ù�����Կ���ܹ�����Կ


/*�����ļ�ϵͳ��������*/
#define     DDI_VFS_CTL_VER                     (0) //��ȡ�����ļ�ϵͳ����汾
#define     DDI_VFS_CTL_FLUSH                   (1) //�����ļ�����

/*��Ƶͨ�ſ�������*/
#define     DDI_AUDIOCOM_CTL_VER                (0) //��ȡ��Ƶͨ�Ű汾��
#define     DDI_AUDIOCOM_CTL_GET_STA            (1) //��ѯ��ƵͨѶ״̬
/*Led�ȿ���ָ��*/
#define     DDI_LED_CTL_VER                     (0) //��ȡledģ��汾
#define     DDI_LED_CTL_GLEAM                   (1) //����led����˸

/*=======BEGIN: tjb 2014.05.26  17:41 modify===========*/
#define     DDI_BT_CTL_VER                      (0)           //��ȡ�������Ƴ���汾
#define     DDI_BT_CTL_MVER                     (1)           //��ȡ����ģ��̼��汾
#define     DDI_BT_CTL_ADDR                     (2)           //��ȡ������ַ
#define     DDI_BT_CTL_RPIN                     (3)           //ȡPIN CODE
#define     DDI_BT_CTL_RNAME                    (4)           //��ȡ�����豸����
#define     DDI_BT_CTL_PAIRED_LIST              (5)           //��ȡ������б�
#define     DDI_BT_CTL_PAIRED_DEL               (6)           //ɾ��������豸��Ϣ
#define     DDI_BT_CTL_REMOTE_INFO              (7)           //��ȡԶ���豸����Ϣ
#define     DDI_BT_CTL_WPIN                     (8)           //����PIN CODE
#define     DDI_BT_CTL_WNAME                    (9)           //���������豸����
#define     DDI_BT_CTL_DISCOVERABLE             (10)          //�����Ƿ�ɱ�ɨ��
#define     DDI_BT_CTL_PAIRED_MODE_SET            (11)          //�������ģʽ
#define     DDI_BT_CTL_PAIRED_MODE_GET            (12)          //��ȡ���ģʽ

/*USB�ȿ���ָ��*/
#define     DDI_USB_CTL_VER                     (0)//�汾
#define     DDI_USB_CTL_CLEAR                   (1)//�建��

/*DUKPT�ȿ���ָ��*/
#define     DDI_DUKPT_CTL_VER                   (0)//��ȡDUKPT�豸�汾
#define     DDI_DUKPT_CTL_KSN_NEXT              (1)//DUKPT��Կ����

typedef enum eFont {
    E_PRN_FONT_SONGTI = 0,     //����
    E_PRN_FONT_HEITI = 1,     //����
    E_PRN_FONT_YOUYUAN = 2,     //��Բ
    E_PRN_FONT_YOUHEI = 3,     //�׺�
} eFont;

typedef enum {
    SDK_WORD_ZOOM_N = 0,           //�����ֺ�
    SDK_WORD_ZOOM_H = 1,           //���򱶸�
    SDK_WORD_ZOOM_W = 2,             //���򱶿�
    SDK_WORD_ZOOM_WH = 3,            //�����Ŵ�
} eZoom;


typedef enum eAscSize {
    E_WORD_ASCII_6_12 = 0,     //6*12
    E_WORD_ASCII_8_16 = 1,     //8*16
    E_WORD_ASCII_12_24 = 2,     //12*24
    E_WORD_ASCII_16_32 = 3,     //16*32
    E_WORD_ASCII_24_48 = 4,     //24*48  G2��ӡ��֧��
    E_WORD_ASCII_ERR = 5,     //������
} eAscSize;

typedef enum eNativeSize {
    E_WORD_NATIVE_12_12 = 0,     //12*12
    E_WORD_NATIVE_16_16 = 1,     //16*16
    E_WORD_NATIVE_20_20 = 2,     //20*20
    E_WORD_NATIVE_24_24 = 3,     //24*24
    E_WORD_NATIVE_32_32 = 4,     //32*32
    E_WORD_NATIVE_48_48 = 5,     //48*48  G2��ӡ��֧��
    E_WORD_NATIVE_ERR = 6,     //������
} eNativeSize;

typedef enum eGesture {
    E_NO_MOTION = 0, //������
    E_NOVE_MOTION = 1, //��������
    E_CLICK_MOTION = 2, //�������
    E_DOUBULE_CLICK_MOTION = 3, //˫�������
    E_PRESS_MOTION = 4, //��ѹ����
    E_ZOOM_OUT_MOTION = 5, //��С����
    E_ZOOM_IN_MOTION = 6, //�Ŵ�����
    E_TURN_MOTION = 7, //ת������
    E_UP_MOTION = 8, //��������
    E_DOWN_MOTION = 9, //��������
    E_LEFT_MOTION = 10, //��������
    E_RIGHT_MOTION = 11, //��������
} eGesture;

/*����ͨ��״̬����*/
typedef enum {
    DEV_WIRELESS_SEARCHING = 1, //Ѱ����
    DEV_WIRELESS_IN_NET, //������
    DEV_WIRELESS_NO_SIM, //��SIM��
    DEV_WIRELESS_DIALING, //������
    DEV_WIRELESS_CONNECTED, //������
    DEV_WIRELESS_HANGUPING //�Ҷ���
} DEV_WIRELESS_RETURN;

/*����ͨ��״̬����*/
typedef enum {
    DEV_MODEM_HANGUPED = 1, //�ѹһ�
    DEV_MODEM_IN_DIALING, //������
    DEV_MODEM_NO_CONNECTED, //������
    DEV_MODEM_HANGUPING, //�Ҷ���
    DEV_MODEM_NOLINE, //δ�ӵ绰��
    DEV_MODEM_ENGAGED //ռ��
} DEV_MODEM_RETURN;

/*====================== END======================== */

/**###############################xgd ddi�ӿں�������#############################**/

/********************************2.1������������**********************************/
#define printf(fmt, arg ...) ddi_core_printf(fmt, ## arg)

extern signed int ddi_sys_printf(const char *string);

extern signed int ddi_sys_msleep(unsigned int nMs);

extern signed int ddi_sys_set_time(const unsigned char *lpTime);

extern signed int ddi_sys_get_time(unsigned char *lpTime);

extern signed int ddi_sys_get_tick(unsigned int *nTick);

extern signed int ddi_sys_read_dsn(unsigned char *lpOut);

extern signed int ddi_sys_sleep(signed int status);

extern signed int ddi_sys_get_firmwarever(unsigned char *lpOut, unsigned int nType);

extern signed int ddi_sys_bat_status(void);

extern signed int ddi_sys_poweroff(void);

extern void *ddi_k_malloc(unsigned nbytes);

extern void ddi_k_free(void *ap);

extern signed int ddi_sys_sleep_with_alarm(signed int sec, signed int type, signed int status);

extern signed int ddi_sys_reboot(void);

extern signed int ddi_sys_download(unsigned int type);

/********************************2.2���ں�������**********************************/
extern signed int ddi_com_open(unsigned int nCom, const strComAttr *lpstrAttr);

extern signed int ddi_com_close(unsigned int nCom);

extern signed int ddi_com_clear(unsigned int nCom);

extern signed int ddi_com_read(unsigned int nCom, unsigned char *lpOut, signed int nLe);

extern signed int ddi_com_write(unsigned int nCom, unsigned char *lpIn, signed int nLe);

extern signed int ddi_com_ioctl(unsigned int nCom, unsigned int nCmd, unsigned int lParam,
                                unsigned int wParam);

/********************************2.3�ſ���������************************************/
extern signed int ddi_mag_open(void);

extern signed int ddi_mag_close(void);

extern signed int ddi_mag_clear(void);

extern signed int ddi_mag_read(unsigned char *lpTrack1, unsigned char *lpTrack2,
                               unsigned char *lpTrack3);

extern signed int ddi_mag_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.4ICC/PSAM��������********************************/
extern signed int ddi_iccpsam_open(unsigned int nslot);

extern signed int ddi_iccpsam_close(unsigned int nslot);

extern signed int ddi_iccpsam_poweron(unsigned int nSlot, unsigned char *lpAtr);

extern signed int ddi_iccpsam_poweroff(unsigned int nslot);

extern signed int ddi_iccpsam_get_status(unsigned int nslot);

extern signed int ddi_iccpsam_exchange_apdu(unsigned int nSlot, const unsigned char *lpCApdu,
                                            unsigned int nCApduLen, unsigned char *lpRApdu,
                                            unsigned int *lpRApduLen, unsigned int nRApduSize);

extern signed int ddi_iccpsam_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.5�ǽӿ���������**********************************/
extern signed int ddi_rf_open(void);

extern signed int ddi_rf_close(void);

extern signed int ddi_rf_poweron(unsigned int ntype);

extern signed int ddi_rf_poweroff(void);

extern signed int ddi_rf_get_status(void);

extern signed int ddi_rf_activate(void);

extern signed int ddi_rf_exchange_apdu(const unsigned char *lpCApdu, unsigned int nCApduLen,
                                       unsigned char *lpRApdu, unsigned int *lpRApduLen,
                                       unsigned int nRApduSize);

extern signed int ddi_rf_remove(void);

extern signed int ddi_rf_ioctl(unsigned int ncmd, unsigned int lparam, unsigned int wparam);

/********************************2.6��ӡ����������**********************************/

extern signed int ddi_thmprn_open(void);

extern signed int ddi_thmprn_close(void);

extern signed int ddi_thmprn_feed_paper(unsigned int nPixels);

extern signed int ddi_thmprn_print_image(unsigned int nOrgLeft, unsigned int nImageWidth,
                                         unsigned int nImageHeight, const unsigned char *lpImage);

extern signed int ddi_thmprn_print_image_file(unsigned int nOrgLeft, unsigned int nImageWidth,
                                              unsigned int nImageHeight,
                                              const unsigned char *lpImageName);

extern signed int ddi_thmprn_print_text(strPrnTextCtrl *lpPrnCtrl, const unsigned char *lpText);

extern signed int ddi_thmprn_print_comb_text(unsigned int nNum,
                                             const strPrnCombTextCtrl *lpPrnCombTextCtrl[]);

extern signed int ddi_thmprn_get_status(void);

extern signed int ddi_thmprn_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.7������������************************************/
extern signed int ddi_key_open(void);

extern signed int ddi_key_close(void);

extern signed int ddi_key_clear(void);

extern signed int ddi_key_read(unsigned int *lpKey);

extern signed int ddi_key_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.8��ʾ��������************************************/
extern signed int ddi_lcd_open(void);

extern signed int ddi_lcd_close(void);

extern signed int ddi_lcd_fill_rect(const strRect *lpstrRect, unsigned int nRGB);

extern signed int ddi_lcd_clear_rect(const strRect *lpstrRect);

extern signed int ddi_lcd_show_text(unsigned int nX, unsigned int nY, const unsigned char *lpText);

extern signed int ddi_lcd_show_picture(const strRect *lpstrRect, const strPicture *lpstrPic);

extern signed int ddi_lcd_show_picture_file(const strRect *lpstrRect,
                                            const unsigned char *lpBmpName);

extern signed int ddi_lcd_show_pixel(unsigned int nX, unsigned int nY);

extern signed int ddi_lcd_show_line(const strLine *lpstrLine);

extern signed int ddi_lcd_show_rect(const strRect *lpstrRect);

extern signed int ddi_lcd_extract_rect(const strRect *lpstrRect, strPicture *lpstrPic);

extern signed int ddi_lcd_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.9��Ƶ��������************************************/
extern signed int ddi_audio_open(void);

extern signed int ddi_audio_close(void);

extern signed int ddi_audio_play(const unsigned char *lpFile);

extern signed int ddi_audio_stop(void);

extern signed int ddi_audio_pause(void);

extern signed int ddi_audio_replay(void);

extern signed int ddi_audio_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.10��������������*********************************/
extern signed int ddi_tp_open(void);

extern signed int ddi_tp_close(void);

extern signed int ddi_tp_read_pixels(strTpPixels *lpTpPixels);

extern signed int ddi_tp_clear(void);

extern signed int ddi_tp_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);


/********************************2.11����������̺�������***************************/
extern signed int ddi_innerkey_open(void);

extern signed int ddi_innerkey_close(void);

extern signed int ddi_innerkey_inject(unsigned int nKeyArea, unsigned int nIndex,
                                      const unsigned char *lpKeyData);

extern signed int ddi_innerkey_encrypt(unsigned int nKeyArea, unsigned int nIndex,
                                       unsigned int nLen, const unsigned char *lpIn,
                                       unsigned char *lpOut);

extern signed int ddi_innerkey_decrypt(unsigned int nKeyArea, unsigned int nIndex,
                                       unsigned int nLen, const unsigned char *lpIn,
                                       unsigned char *lpOut);

extern signed int ddi_innerkey_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.12����ͨ�ź�������*******************************/
extern signed int ddi_wireless_open(void);

extern signed int ddi_wireless_close(void);

extern signed int ddi_wireless_destroy(void);

extern signed int ddi_wireless_dial(const unsigned char *lpApn, const unsigned char *lpUser,
                                    const unsigned char *lpPwd);

extern signed int ddi_wireless_hangup(void);

extern signed int ddi_wireless_get_status(void);

extern signed int ddi_wireless_sleep(void);

extern signed int ddi_wireless_wakeup(void);

extern signed int ddi_wireless_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.13MODEMͨ�ź�������******************************/
extern signed int ddi_modem_open(void);

extern signed int ddi_modem_close(void);

extern signed int ddi_modem_dial(const strModemDialPara *lpstrDialPara);

extern signed int ddi_modem_hangup(void);

extern signed int ddi_modem_get_status(void);

extern signed int ddi_modem_read(unsigned char *lpOut, signed int nLe);

extern signed int ddi_modem_write(unsigned char *lpIn, signed int nLe);

extern signed int ddi_modem_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.14PPPOE��������**********************************/
//��
/********************************2.15PPPOM��������**********************************/
//��
/********************************2.16WIFI��������***********************************/
//��
/********************************2.17������������***********************************/
extern signed int ddi_bt_open(void);

extern signed int ddi_bt_close(void);

extern signed int ddi_bt_disconnect(const unsigned char *lpMac);

extern signed int ddi_bt_write(unsigned char *lpIn, signed int nLe);

extern signed int ddi_bt_read(unsigned char *lpOut, signed int nLe);

extern signed int ddi_bt_get_status(void);

extern signed int ddi_bt_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

extern signed int ddi_audiocom_open(void);

extern signed int ddi_audiocom_close(void);

extern signed int ddi_audiocom_clear(void);

extern signed int ddi_audiocom_read(unsigned char *lpOut, signed int nLe);

extern signed int ddi_audiocom_write(unsigned char *lpIn, signed int nLe);

extern signed int ddi_audiocom_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.18ɨ��ǹ��������*********************************/
//��
/********************************2.19ָ�ƺ�������***********************************/
//��
/********************************2.20GPS��������************************************/
//��
/********************************2.21 xgd�ļ�ϵͳ��������******************************/
extern signed int ddi_vfs_open(const unsigned char *lpPath, const unsigned char *lpMode);

extern signed int ddi_vfs_close(signed int nHandle);

extern signed int ddi_vfs_write(const unsigned char *lpData, signed int nLen, signed int nHandle);

extern signed int ddi_vfs_read(unsigned char *lpData, signed int nLen, signed int nHandle);

extern signed int ddi_vfs_seek(signed int nHandle, signed int nOffset, signed int nWhence);

extern signed int ddi_vfs_tell(signed int nHandle);

extern signed int ddi_vfs_remove(const unsigned char *lpFileName);

extern signed int ddi_vfs_rename(const unsigned char *lpOldName, const unsigned char *lpNewName);

extern signed int ddi_vfs_access(const unsigned char *lpName);

extern signed int ddi_vfs_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.22 LED��������************************************/
extern signed int ddi_led_open(void);

extern signed int ddi_led_close(void);

extern signed int ddi_led_sta_set(unsigned int nLed, unsigned int nSta);

extern signed int ddi_led_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

/********************************2.23 USB��������************************************/
extern signed int ddi_usb_open(unsigned int nClass);

extern signed int ddi_usb_close(unsigned int nClass);

extern signed int ddi_usb_read(unsigned int nClass, unsigned char *lpOut, unsigned int nLe);

extern signed int ddi_usb_write(unsigned int nClass, unsigned char *lpIn, unsigned int nLe);

extern signed int ddi_usb_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

//

/********************************2.23 dukpt��������************************************/
extern signed int ddi_dukpt_open(void);

extern signed int ddi_dukpt_close(void);

extern signed int ddi_dukpt_inject(strDukptInitInfo *lpstrDukptInitInfo);

extern signed int ddi_dukpt_encrypt(unsigned int nKeyGroup, unsigned int nKeyIndex,
                                    unsigned int nLen, const unsigned char *lpIn,
                                    unsigned char *lpOut, unsigned int nMode);

extern signed int ddi_dukpt_decrypt(unsigned int nKeyGroup, unsigned int nKeyIndex,
                                    unsigned int nLen, const unsigned char *lpIn,
                                    unsigned char *lpOut, unsigned int nMode);

extern signed int ddi_dukpt_getksn(unsigned int nKeyGroup, unsigned int nKeyIndex,
                                   unsigned int *lpLen, unsigned char *lpOut);

extern signed int ddi_dukpt_ioctl(unsigned int nCmd, unsigned int lParam, unsigned int wParam);

#endif


