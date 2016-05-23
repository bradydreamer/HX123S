#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef XGDSDKCOMM_H
#define XGDSDKCOMM_H


#define SDK_COMMU_MASK_BASE                       0x0001
#define SDK_COMMU_MASK_MODEM                  (SDK_COMMU_MASK_BASE)
#define SDK_COMMU_MASK_LAN                       (SDK_COMMU_MASK_BASE<< 1)
#define SDK_COMMU_MASK_CDMA                    (SDK_COMMU_MASK_BASE<< 2)
#define SDK_COMMU_MASK_GPRS                     (SDK_COMMU_MASK_BASE<<3)
#define SDK_COMMU_MASK_WIFI                      (SDK_COMMU_MASK_BASE<<4)
#define SDK_COMMU_MASK_BLUETOOTH                (SDK_COMMU_MASK_BASE<<5)


#define SDK_COMMU_MASK_ALL                         0xFFFF


//ͨѶ��ʽ//communication mode
typedef enum {
    SDK_COMM_MODEM = 1,                                 //modem����//modem dial
    SDK_COMM_GPRS,                                      //GPRS����//GPRS wireless
    SDK_COMM_CDMA,                                      //CDMA����//CDMA wireless
    SDK_COMM_ETHERNET,                                  //��̫��//ethernet
    SDK_COMM_WIFI,                                      //WIFI �ݲ�֧��//don't support WIFI now
    SDK_COMM_MODEMPPP,                                  //Modem PPP �ݲ�֧��//don't support Modem ppp now
    SDK_COMM_BLUETOOTH,                                  //BlueTooth �� 2.0 ֧��
    SDK_COMM_AUDIO,                                     //AUDIOCOMM 
    SDK_COMM_USB,                                       //USB
} SDK_COMM_COMMMODE;
#define SDK_COMM_UART   0                               //����ͨѶ��ʽ//serial port communication mode


typedef struct {
    //������˽����²����������벻Ҫ���ã�ϵͳ��ʹ��Ĭ��ֵ
    //please don't set the following parameters if you don't know their function, system will use the default value
    u8 asDialNO[21];                                    //������� �20���ֽ�//dial number up to 20 bytes
    u8 asPin[51];                                       //asPin �50���ֽ�//asPin up to 50 bytes

    u32 uiMtu;                                          //Maximum Transmit Unit        ��С1280 //minimum 1280
    u32 uiMru;                                          //Maximum Receive Unit 128<<uiMtu<<16384 Ĭ��:1500 //default:1500

    u32 uiLcpEchoFailure;                               //
    u32 uiLcpEchoInterval;                              //If this option is given, pppd will send an LCP echo-request frame to the peer every n seconds.    (n is the value you set)
    u32 uiLcpMaxConfigure;                              //Set the maximum number of LCP configure-request transmissions
    u32 uiLcpMaxFailure;                                //Set the maximum number of LCP configure-NAKs returned before starting to send onfigure-Rejects      (default 10)
    u32 uiLcpMaxTerminate;                              //Set the maximum number of LCP terminate-request transmissions (default 3).
    u32 uiIpcpMaxConfigure;                             //Set the maximum number of IPCP configure-request transmissions
    u32 uiIpcpMaxFailure;                               //Set the maximum number of IPCP configure-NAKs returned before starting to send configure-Rejects      (default 10).
    u32 uiIpcpMaxTerminate;                             //Set the maximum number of IPCP terminate-request transmissions (default 3)
    u32 uiIdle;                                         //��λ ms �ڶ�����ģʽ��,���uiIdleʱ����û�����ݽ���,����Զ��Ͽ� PPP����//the units are seconds in the short connect mode, if there are no data transmit during the  uiIdle then disconnect PPP
    u8 asService[51];
    u32 uiSocketOvertime;
    bool bIsSendPing;                                   //�Ƿ���ping�� 0-��1-��//whether to send ping bag  0-no, 1-yes
    bool bIsMultiplexing;                               //�Ƿ��·���� 0-��1-��//whether multiplexing    0-no, 1-yes
} ATTRIBUTE
SDK_COMM_STEXPPPWIRELESS;

//����PPP����// wireless ppp parameters
typedef struct {
//    bool    IsPPPExt;                                 //�ⲿPPP֧�� 1: ֧��,0: ��֧��//external ppp support   1:yes, 0:no
    bool bIsNeedUser;                                   //�Ƿ���Ҫ�û�  0-����Ҫ��1��Ҫ//whether need user   0-no, 1-yes
    u8 asUsername[51];                                  //�û���//user name
    u8 asPassword[51];                                  //����//password
    u8 asGprsApn[51];                                   //�ƶ�APN����//mobile APN name
    u8 bIsNeedExParam;                                  //�Ƿ���Ҫ��չ����   0-����Ҫ��1��Ҫ//whether need expand parameters, 0-no, 1-yes
    SDK_COMM_STEXPPPWIRELESS stExPppWireless;           //������չ���� ������˽���չ�����������벻Ҫ���ã�ϵͳ��ʹ��Ĭ��ֵ//wireless expand parameters, please do not set the parameters if you do not know their functions, the system will use the default setting.
} ATTRIBUTE
SDK_COMM_STPPPWIRELESS;

//PPPOE ����//PPPOE parameters

typedef struct {
    u8 asPPPoeUser[51];                                 //PPPoe�û����� �50���ֽ� ASCII     //pppoe user name up to 50 bytes ASCII
    u8 asPPPoeUserPwd[51];                              //PPPoe�û����� �50���ֽ� ASCII     //pppoe user password up to 50 bytes ASCII
} ATTRIBUTE
SDK_COMM_STPPPOEPARAM;

//��̫������//Ethernet parameters
typedef struct {
    u8 asNetClientIP[16];                               //����IP��ַ//local IP
    u8 asNetGate[16];                                   //��������//local gateway
    u8 asNetMask[16];                                   //������������//local subnet mask
    u8 asDnsIP[16];                                     //DNS������IP  �ݲ�֧�֣�����ʹ�øò���//DNS server IP, not support now, this parameter will not be used
    bool bIsDhcp;                                       //�Ƿ�ʹ��DHCP //whether use DHCP
    bool bIsPPPoe;                                      //�Ƿ���PPPoe��ʽ 0-��1-��//is pppoe or not, 0-no, 1-yes
    SDK_COMM_STPPPOEPARAM stPPPoeParam;                 //PPPoe����//pppoe parameter
} ATTRIBUTE
SDK_COMM_STLANPARAM;

//MODEM��չ����//MODEM expanded parameters
typedef struct {
    bool bIsCheckPhone;                                 //�Ƿ񲢻����  0-����Ҫ��1��Ҫ  Ĭ����Ҫ//whether need detection when starting up, 0-no, 1-yes, default yes
    u8 ucDtmfOnTime;                                    //DTMF On ʱ��(��λΪms Ĭ��100 ��ʾ100ms)  //DTMF On time(units ms, default 100 means 100ms)
    u8 ucDtmfOffTime;                                   //DTMF OFF ʱ��(��λΪms Ĭ��100 ��ʾ100ms)  //DTMF OFF time(units ms, default 100 means 100ms)
    bool bIsCheckDialTone;                              //�Ƿ��Ⲧ���� 0-����Ҫ��1��Ҫ  Ĭ����Ҫ//whether check dial tone, 0-no, 1-yes, default yes
    u8 ucDelayBeforeDial;                               //����Ⲧ����ʱ ����ǰ����ʱʱ�� (��λΪs Ĭ��1 ��ʾ1s)  //the delay time before dialing if don't check dial tone(units s, default 1 means 1s)
    u16 usCheckToneTime;                                //���������ʱ��(��λΪms Ĭ��500 ��ʾ500ms)  //dial tone testing time(units ms, default 500 means 500ms)
    u16 usWaveLostTime;                                 //�ز���ʧʱ��(��λΪms Ĭ��20 ��ʾ20ms)  //wave lost time(units ms, default 20 means 20ms)
    u8 ucSendLevel;                                     //���͵�ƽ ( Ĭ��Ϊ 3))  //send level(default 3)
    u8 ucFsk01Num;                                      //0��1�ĸ���
    u8 ucFsk1Num;                                       //1��0�ĸ���
    u8 ucCidFormat;                                     //����ģʽ
    u8 ucModemSpeedLevel;                               //modem��չ����0~5 �����ٶȵȼ�1-5�� //huangbx 20130117
} ATTRIBUTE
SDK_COMM_STMODEMEXPARAM;

typedef enum {
    SDK_COMM_LINK_MODE_SYN = 0,    ////0 - ͬ��
    SDK_COMM_LINK_MODE_AYN = 1,    //1 - �첽
    SDK_COMM_LINK_MODE_FSK = 2    //FSK
} SDK_COMM_LINK_MODE;

typedef enum {
    SDK_COMM_HARD_MODEM_MODE_1 = 0,    ////���ӿ죬ͨѶ��
    SDK_COMM_HARD_MODEM_MODE_2 = 1    //��������ͨѶ��
} SDK_COMM_HARD_MODEM_MODE;

//MODEM����//MODEM parameters
typedef struct {
    SDK_COMM_LINK_MODE eLinkMode;                       //0 - ͬ�� 1 - �첽  2 - FSK//0-synchronous, 1-asynchronous
    u8 asOutLineNo[8];                                  //���ߺ��� �7λ//out line number up to 7 digit
    u8 asPhone[3][28];                                  //�绰���� �25λ//phone number up to 25 digit
    bool bIsNeedExParam;                                //�Ƿ���Ҫ��չ����  0-����Ҫ��1 - ��Ҫ//whether need expand parameters, 0-no, 1-yes
    SDK_COMM_STMODEMEXPARAM stModemExParam;             //��չ����//expand parameter
} ATTRIBUTE
SDK_COMM_STMODEMPARAM;

//MODEMPPP ����//MODEMPPP parameters
typedef struct {
    u8 asPPPNetTel[32];
    u8 asPPPUser[32];                                   //ppp�����û���// ppp dialing user name
    u8 asPPPPwd[32];                                    //ppp��������//ppp dialing password
    u8 asPPPExParam[32];                                //Ԥ��//reserved
} SDK_COMM_STMODEMPPPPARAM;

//ͨѶ����������//communication parameters union
typedef union {
    SDK_COMM_STMODEMPARAM stModemInfo;                   //����MODEM����//dialing MODEM parameter
    SDK_COMM_STPPPWIRELESS stPppWireLessInfo;            //PPP���߲���//ppp wireless parameter
    SDK_COMM_STLANPARAM stLanInfo;                       //LAN����//LAN parameter
    SDK_COMM_STMODEMPPPPARAM stModemPppInfo;             //MODEMPPP���� �ݲ�ʹ��//MODEMPPP parameter, not use now
} ATTRIBUTE
SDK_COMM_COMMINFO;

//��������������//server parameters type
typedef struct {
    u8 asServerIP[16];                                   //������IP��ַ//server IP
    u8 asPort[6];                                        //����˿ں�//server port number
} ATTRIBUTE
SDK_COMM_STSERVERADDRESS;

//ͨѶ����//communication parameters
typedef struct {
    SDK_COMM_COMMMODE eMode;                              //ͨѶ��ʽ//communication mode
    u8 ucReDialTimes;                                     //�ز�����//re-dial times
    u32 ucDialTime;                                        //����ʱ��  ��Ϊ��λ ΪmsĬ��60000ms  //dialing time, units ms, default 60000ms
    bool bSocketConnectMode;                              //Socket���ӷ�ʽ 1:������ 0:������//Socket connection type, 1: short connect, 0:long connect
    SDK_COMM_COMMINFO stCommInfo;
    SDK_COMM_STSERVERADDRESS stServerInfo;                //����������//server parameters
} ATTRIBUTE
SDK_COMM_STCOMMPARAM;


//���ڲ���//serial port parameters
typedef struct {
    u8 ucCom;                                             //SDK_COMM_UART_RS232 RS232���ں�//SDK_COMM_UART_RS232 RS232 port number
    //SDK_COMM_UART_PINPAD ������̴��ں�//SDK_COMM_UART_PINPAD   PIN pad port number
    //0 - 9 ��ʾ��Ӧ�Ĵ��ں�//0-9 means corresponding port number

    u32 uiBaudrate;                                       //���ڲ����� ֧�� 9600 14400 19622 38400 57600 115200 ����Ϊ��������//serial port baudrate supports 9600 14400 19622 38400 57600 115200, other is wrong
    u8 ucByteLen;                                         //����λ����:֧��5,6,7,8λ, //data glag:support 5,6,7,8 bit
    u8 ucCheckBit;                                         //����λ:֧�� 'n'-�޼��飬'o'-odd���飬'e'-evenУ�飬's'-spaceУ��//check flag:support  'n'-no check, 'o'-odd check, 'e'-even check, 's'-space check
    u8 ucStopBitLen;                                      //ֹͣλ:֧��1,2λ//stop flag: support 1,2 bit
    u8 ucFlowControl;                                     //����  :��֧������ ��0  //flow control: is 0 if not support flow control

    //ֹͣλ:֧��1,2λ//stop flag: support 1,2 bit

    //Ĭ��Ϊ"8n10"��ʾ8������λ,none û��У��λ��1��ֹͣλ��0 ������//default is "8n10" means 8 bits data flag, none check flag, one stop flag, 0 no flow control
} ATTRIBUTE
SDK_COMM_STUARTPARAM;

typedef struct {
    u8 ucUsrName[65];       //VNP �û���
    u8 uiPassWord[65];      //VNP ����
    u8 uiHostIP[16];        //VNP ������IP
    u8 uiGetWay[16];        //VNP ����  
    u16 uiEncryMode;        //����ģʽ����ʱ����
    u8 uiReserve[128];      //Ԥ��buf
    void *pVar;             //Ԥ������
} ATTRIBUTE
SDK_COMM_VPN_PARA;

//   ͨ��Э��//communication protocol

#define SDK_COMM_SEND8583           0                     //8583���ݴ���Э��//8583 data transport protocols
#define SDK_COMM_TRANSPARENT        1                     //͸��Э��//transparent transport protocols


//   ״̬����ֵ//state return value
typedef enum {
    SDK_COMM_DIALING = (SDK_SYS_RET_OK_MIN + 1),    // ���ڲ���
    SDK_COMM_ENGAGED = (SDK_SYS_RET_ERR_MAX - 1),    //�绰��ռ��//telephone line is engaged
    SDK_COMM_NO_SIM = (SDK_SYS_RET_ERR_MAX - 2),    //ĩ�����ֻ���//not insert SIM card
    SDK_COMM_LINKFAIL = (SDK_SYS_RET_ERR_MAX - 3),    //��������ʧ��//net connect failed
    SDK_COMM_WAVELOST = (SDK_SYS_RET_ERR_MAX - 4),    //�ز���ʧ//wave lost
    SDK_COMM_NOLINE = (SDK_SYS_RET_ERR_MAX - 5),    //ĩ�ӵ绰��//not link telephone line
    SDK_COMM_NOSTAKE = (SDK_SYS_RET_ERR_MAX - 6),    //�ֻ��Żص���//put the phone back to underplate
    SDK_COMM_LRC_ERROR = (SDK_SYS_RET_ERR_MAX - 7),    //Э��LRC����//protocol LRC wrong
    SDK_COMM_BUFFER_ERROR = (SDK_SYS_RET_ERR_MAX -
                             8),    //�������ݳ��ȹ���//the received data length is too long
    SDK_COMM_UART_USED = (SDK_SYS_RET_ERR_MAX - 9),    //���ڱ�ռ��//port is used
    SDK_COMM_UART_NOESIST = (SDK_SYS_RET_ERR_MAX - 10),   //���ڲ�����//port isn't exit
    SDK_COMM_UART_NOTOPEN = (SDK_SYS_RET_ERR_MAX - 11),   //����û��//port not open
    SDK_COMM_DEVERR = (SDK_SYS_RET_ERR_MAX - 12),   //�豸����
    SDK_COMM_LINEBUSY = (SDK_SYS_RET_ERR_MAX - 13),   //������·æ
    SDK_COMM_NOANSWER = (SDK_SYS_RET_ERR_MAX - 14),   //������Ӧ��
    SDK_COMM_NODIALTONE = (SDK_SYS_RET_ERR_MAX - 15),   //�޲�����
    SDK_COMM_UNKNOWN = (SDK_SYS_RET_ERR_MAX - 16),   //δ֪�쳣
    SDK_COMM_POWERFAIL = (SDK_SYS_RET_ERR_MAX - 17),   //����ģ������ʧ��
    SDK_COMM_CREGFAIL = (SDK_SYS_RET_ERR_MAX - 18),    //����ģ��δע��
    SDK_COMM_PPP_CREAT_FAIL = (SDK_SYS_RET_ERR_MAX - 19),    //PPP����ʧ��
    SDK_COMM_UART_USB_DISCONNET = (SDK_SYS_RET_ERR_MAX - 20), //USB���ӶϿ�

/*=======BEGIN: fusuipu 2014.10.31  9:42 modify ��վ��Ϣ�ﾭ��ʹ�õķ���ֵ===========*/
            SDK_COMM_WIRLE_NOTSUPPOT = (SDK_SYS_RET_ERR_MAX - 21),   //����ģ�鲻֧��
    SDK_COMM_MODULE_VERSION_ERROR = (SDK_SYS_RET_ERR_MAX - 22), //G610�̼��汾����
            SDK_COMM_NO_MUX = (SDK_SYS_RET_ERR_MAX - 23),  //û�ж�·����
    SDK_COMM_NOT_UP = (SDK_SYS_RET_ERR_MAX - 24),  //��·����δ����
    SDK_COMM_NO_ANSWER = (SDK_SYS_RET_ERR_MAX - 25), //ָ��û�з���
    SDK_COMM_NOT_SUPPORT_VNP = (SDK_SYS_RET_ERR_MAX - 26),  //��֧��VNPͨ��ģʽ
    SDK_COMM_LINEWAIT = (SDK_SYS_RET_ERR_MAX - 27) //δ�ӵ绰�ߵȴ�
/*====================== END======================== */
} SDK_COMM_RETURN;

typedef enum {
    SDK_COMM_LINK_POWER_ON = (SDK_COMM_OK_BASE + 1),                //�����ϵ�
    SDK_COMM_LINK_INITIALIZE,                         //���ڳ�ʼ��
    SDK_COMM_LINK_CMUX_OPEN,                          //��ͨ�Žӿ� < open CMUX
    SDK_COMM_LINK_DIAL_INITIALIZE,                    //��ʼ������
    SDK_COMM_LINK_SIM_CHECK,                          //���SIM��
    SDK_COMM_LINK_MODE_CHECK,                         //< mode check
    SDK_COMM_LINK_SIGNAL_CHECK,                       //�ź�ǿ�ȼ�� < check Signal
    SDK_COMM_LINK_NETWORK_ATTACH,                     //���總��
    SDK_COMM_LINK_DIAL_START,                         //��ʼ����
    SDK_COMM_LINK_GET_IP,                             //��ȡIP< got IP
    SDK_COMM_LINK_DIAL_STOP,                          //ֹͣ���� < stop to dial
    SDK_COMM_LINK_CMUX_CLOSE,                         //�ر�ͨ�Žӿ� < close CMUX
    SDK_COMM_LINK_RESET,                              //��λ< reset module
    SDK_COMM_LINK_POWER_OFF,                          //�����µ� < module power off
    SDK_COMM_LINK_WIFI_AP_DISCONNECT,                 //�Ͽ�AP����
    SDK_COMM_LINK_WIFI_FIND_SSID,                     //����AP SSID
    SDK_COMM_LINK_WIFI_AP_CONNECTING,                 //��������AP
    SDK_COMM_LINK_WIFI_AP_CONNECTED,                  //AP���ӳɹ�
    SDK_COMM_PPP_CREAT_SUCCESS                        //����ppp�ɹ�  
} SDK_COMM_LINK_STATE;

typedef struct {
    u16 uiIdx;              //��ǰ��վ�ڵ�����
    u32 uiDateLen;          //��ǰ��վ�����ڵ����ݳ���
    u16 uiLac;              //localization area code
    u16 uiCi;               //cell identifier
    u16 uiMcc;              //country code
    u8 uiMnc;              //network operator code
    u16 uiBsic;             //base station identification code
    u16 uiBcch;             //BroadcastControlChannel
    s32 uiCsq;               //received signal strength //ͳһʹ��dbmֵ��ʾ
    u8 ucOtherDate[256];    //һ����վ��Ϣ��󳤶�Ϊ256����ȥLAC�����ݣ�����������256��buf����
    void *pVar;             //Ԥ��һ����ָ�룬�Ա���������չʹ��
} SDK_COMM_STATION_INFO;

/**
����K501��MODEMͨѶ����Ҫ�������������չ������֮ǰ�Ǽ��ڽṹ��SDK_COMM_STMODEMEXPARAM�����ǻ��г�ͻ�����Խ������ⶨ��ṹ����������������������һ���ӿ�����������������
**/
typedef struct {
    bool bIsCheckAnswerTone;    //�Ƿ���Ӧ���� 0-����Ҫ��1��Ҫ  Ĭ����Ҫ//lzl 20141118
    u8 ucCheckAnswerToneTime;   //Ӧ�������ʱ��(0-5)//(0:ʹ��Ĭ�ϵ�Ӧ�������ʱ��  1��300ms   2:500ms  3:700ms  4:900ms   5:1100ms)//lzl20141117
    u8 ucOtherDate[100];        //Ԥ��100byte�Ŀռ�
    void *pVar;                //Ԥ��һ����ָ�����
} SDK_COMM_EXT_PARA;

typedef struct {
    u8 uPhoneSwitch; //�绰����/ʹ�ܿ���
    u8 uiSim; //�Ƿ��⵽SIM��
    u8 ucCarrierInfo; //��Ӫ��
    s8 ssignLevel; //�ź�ǿ��
} SDK_COMM_SIM_INFO;


//      ����״̬//serial port state
#define SDK_COMM_UART_CLOSE         0                     //���ڹر�//port close
#define SDK_COMM_UART_OPEN          1                     //���ڴ�//port open
#define SDK_COMM_UART_MISS          2                     //���ڲ�����//port not exit

#define SDK_COMM_UART_RS232         0xFF                  //RS232���ں�//RS232 serial port number
#define SDK_COMM_UART_PINPAD        0xFE                  //������̴��ں�//PIN pad port number
#define SDK_COMM_UART_USB           0xFD                  //USB�� (cdc)
#define SDK_COMM_UART_USBHID        0xFC                  //USB�� (hid)
#define SDK_COMM_UART_AUDIO         0xFB                  //AUDIO�� 

s32 sdkPhoneEnable(u8 ucEnable); //����ӵ�sdkDownload���ȥ
/*******************************************************************************************
** Descriptions:    ���PPP����ͨѶ�ı���IP
** Parameters:      pDest:IP
** Returned value:  IP����
                    SDK_PARA_ERR	�������󷵻�
                    SDK_ERR         ��ȡIPʧ��
** Created By:      �Ʊ���(2012-05-08)
** Remarks:         ���������ص�IP�����Ϊ16���ֽڳ��ȣ��������������������ڻ����16���ֽڵ��ڴ�ռ�
*******************************************************************************************/
s32 sdkCommGetPppIP(u8 * pasDest);

/*******************************************************************************************
** Descriptions:    ��ʼ��ͨѶ
** Parameters:      stCommParam:ͨѶ�����ṹ��
** Returned value:  SDK_OK                  ��ʼ���ɹ�
                    SDK_PARA_ERR            ��������
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_NO_SIM         û�в����ֻ���//huangbx 20130108
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����Ĭ��MODEM��չ��������ʼ�������������������IP���������������IP
                    ��������ͨѶ��������PPP��·,9s�����û�ܳɹ�����PPP��·�򷵻س�ʱ(SDK_TIME_OUT)
*******************************************************************************************/
s32 sdkCommInit(const SDK_COMM_STCOMMPARAM *pstCommParam);

/*******************************************************************************************
** Descriptions:    ����ͨѶ����
** Parameters:
** Returned value:  SDK_OK          �ɹ�
                    SDK_PARA_ERR    �����Ƿ�
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ����
                    ���ͨѶ�����иĶ��������ô˺����������ò���
*******************************************************************************************/
s32 sdkCommConfig(const SDK_COMM_STCOMMPARAM *pstCommParam);

/*******************************************************************************************
** Descriptions:    ����MODEMͨѶ��չ����
** Parameters:      IsDefaultValue 0-����Ĭ��ֵ 1-Ĭ��ֵ:pstModemExParam��Ч
                    SDK_COMM_STMODEMEXPARAM *stModemExParam MODEM��չ����
** Returned value:  SDK_OK                  �ɹ�
                    SDK_PARA_ERR            ��������

** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
s32 sdkCommSendModemCfgEx(bool IsDefaultValue, const SDK_COMM_STMODEMEXPARAM *stModemExParam);

/*******************************************************************************************
** Descriptions:    Ԥ����
** Parameters:
** Returned value:  SDK_OK   Ԥ���ųɹ�
                    SDK_ERR  ʧ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����Ԥ���Ź��ܣ����Ͳ��������ֱ���˳����ȴ����Ž��
                    1.LANͨѶ��ʽ�²�����
                    2.���ñ�����ǰ�����ȵ��� ��ʼ��ͨ��
*******************************************************************************************/
s32 sdkCommPre(void);

/*******************************************************************************************
** Descriptions:    ������·
** Parameters:
** Returned value:  SDK_OK   ����ͨѶ
                    SDK_ESC             �ֶ�ȡ��
                    SDK_TIME_OUT        ���ӳ�ʱ
                    SDK_COMM_NOLINE     δ�ӵ绰��
                    SDK_COMM_NOSTAKE    �ֻ��Żص���
                    SDK_COMM_ENGAGED    �绰��ռ��
                    SDK_COMM_NO_SIM     δ�����ֻ���
                    SDK_COMM_LINKFAIL   ��������ʧ��

                 ��modemģʽ�£�״̬��ѯ���ش���
                    SDK_COMM_DEVERR;    //�豸����
                    SDK_COMM_LINEBUSY;  //������·æ
                    SDK_COMM_NOANSWER;  //������Ӧ��
                    SDK_COMM_NODIALTONE;//�޲�����
                    SDK_COMM_WAVELOST;  //�ز���ʧ
                    SDK_COMM_UNKNOWN;   //δ֪�쳣
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ��ʼ�����õĲ�����������
   //zhouzhihua 20130614
*******************************************************************************************/
s32 sdkCommCreateLink(void);

/*******************************************************************************************
** Descriptions:    ������·
** Parameters:
** Returned value:  SDK_OK   ����ͨѶ
                    SDK_ESC             �ֶ�ȡ��
                    SDK_TIME_OUT        ���ӳ�ʱ
                    SDK_COMM_NOLINE     δ�ӵ绰��
                    SDK_COMM_NOSTAKE    �ֻ��Żص���
                    SDK_COMM_ENGAGED    �绰��ռ��
                    SDK_COMM_NO_SIM     δ�����ֻ���
                    SDK_COMM_LINKFAIL   ��������ʧ��

                 ��modemģʽ�£�״̬��ѯ���ش���
                    SDK_COMM_DEVERR;    //�豸����
                    SDK_COMM_LINEBUSY;  //������·æ
                    SDK_COMM_NOANSWER;  //������Ӧ��
                    SDK_COMM_NODIALTONE;//�޲�����
                    SDK_COMM_WAVELOST;  //�ز���ʧ
                    SDK_COMM_UNKNOWN;   //δ֪�쳣
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����ͨѶ��ʼ�����õĲ�����������
   //zhouzhihua 20130614
*******************************************************************************************/
s32 sdkCommCreateLinkEx(s32(*pDisFunc)( const
s32 usRecvTimeOut,
const s32 usCurrentTime
));

/*******************************************************************************************
** Descriptions:    �Ͽ���·
** Parameters:
** Returned value:  SDK_OK   �ɹ�
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         1.	SDK_COMM_MODEMͨѶ��ʽ�����йһ�����
                    2.	SDK_COMM_GPRS�� SDK_COMM_CDMAͨѶ��ʽ���ȶϿ�socket�������Ͽ�ppp����
                        ���ڶ����ӵ����ӷ�ʽ�£������߸߼�����SDK_COMM_STEXPPPWIRELESS���uiIdle
                        �����೤ʱ�������ݽ�����Ͽ�PPP���ӣ�
                    3.	SDK_COMM_ETHERNETͨѶ��ʽ��Ͽ�socket

*******************************************************************************************/
s32 sdkCommDestoryLink(void);

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pSendData   Ҫ���͵�����
                    usLen       ���ݵĳ���
                    nProTocol   Э��
** Returned value:  SDK_OK      �ɹ�
                    SDK_PARA_ERR ��������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         ����nProtocolָ����Э������������
                    1.	����������󳤶�Ϊ2000 byte
                    2.	��SDK_COMM_SEND8583Э�鷢�����ݣ�ϵͳ���Զ�����ǰ�����������ֽڵĳ���
                        �����ڽ���ʱϵͳ�����һ��������8583���ٷ�������
                    3.	��SDK_COMM_TRANSPARENTЭ�鷢�����ݣ��ڽ�������ʱϵͳ���յ����ݺ���������أ�
                        ���յ����ݿ��ܲ���һ�����������ݰ�����Ҫ�û���֤���յ������Ƿ�����
*******************************************************************************************/
s32 sdkCommSendData(const u8 *pheSendData, const u16 usLen, bool bMode);

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pRecvBuf        ���յ�������
                    nRecvBufMaxLen  ���յ���󳤶�
                    RecvTimeOut     ���ճ�ʱʱ�� �Ժ���Ϊ��λ 5000 <= RecvTimeOut <= 120000
** Returned value:  ���ؽ��յ������ݳ���
                    SDK_COMM_WAVELOST   ͨѶ�ж�
                    SDK_PARA_ERR        ��������
                    SDK_TIME_OUT         ͨѶ��ʱ
                    SDK_ESC             �û�ȡ��
                    SDK_COMM_BUFFER_ERROR      �������ݻ���������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         1.	�����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    2.	pDisFuncΪNULLʱ����Ĭ����ʾ�������û����Զ�����ʾ���ݣ������ο�DisFuncDemo
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommRecvData(u8 *pheRecvBuf, const s32 usRecvBufMaxLen, const s32 usRecvTimeOut,
                    s32 (*pDisFunc)(const s32 usRecvTimeOut, const s32 usCurrentTime));

/*******************************************************************************************
** Descriptions:    �򿪲���ʼ�������豸
** Parameters:      stUartInfo ��������
** Returned value:  SDK_OK �ɹ�
                    SDK_ERR ʧ��
                    SDK_PARA_ERR            ��������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:
*******************************************************************************************/
s32 sdkCommOpenUart(const SDK_COMM_STUARTPARAM *pstUartInfo);

/*******************************************************************************************
** Descriptions:    �رմ��ڶ�ռ���ָ����ڲ�����Ϊ38400
** Parameters:
** Returned value:  SDK_OK �ɹ�
                    SDK_COMM_UART_NOESIST ���ڲ�����
** Created By:      �Ʊ���(2012-05-07)
** Remarks:
*******************************************************************************************/
s32 sdkCommCloseUart(const u8 ucCom);

/*******************************************************************************************
** Descriptions:    �򴮿ڷ�������
** Parameters:      ucCom        ���ں�
                    pSendData   ���͵�����
                    usLen         ���ݳ���
** Returned value:  SDK_OK                  �ɹ�
                    SDK_PARA_ERR            ��������
                    SDK_COMM_UART_NOESIST   ���ڲ�����
                    SDK_COMM_UART_NOTOPEN   ����δ��
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         �շ���������ʱ�����ȶ�ռ����
                    //huangbx 20121102 ȥ���������ݳ������� �����������Ϊ 2000 byte
*******************************************************************************************/
s32 sdkCommUartSendData(const u8 ucCom, const u8 *pheSendData, const s32 usLen);

/*******************************************************************************************
** Descriptions:    ���մ�������
** Parameters:      ucCom                   ���ں�
                    pRecvData               ��������
                    nRecvBufMaxLen          ���������ݻ����С
                    usRecvTimeOut           ��ʱʱ�� ms��λ //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
** Returned value:  �������ݳ���
                    SDK_PARA_ERR            ��������
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_UART_NOTOPEN   ����û��
                    SDK_COMM_UART_NOESIST   ���ڲ�����
                    SDK_COMM_BUFFER_ERROR   �������ݻ���������
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         �շ���������ʱ�����ȶ�ռ����
                    �����յ��������ݺ��������ػ��߳�ʱʱ�䵽����
                    �����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommUartRecvData(const u8 ucCom, u8 *pheRecvData, const s32 usRecvBufMaxLen,
                        const s32 usRecvTimeOut);

/*******************************************************************************************
** Descriptions:    �ж�IP��ʽ�Ƿ�Ϸ�
** Parameters:      pIPstr IP�ַ���
** Returned value:  true �Ϸ� false �Ƿ�
** Created By:      �Ʊ���(2012-05-08)
** Remarks:
*******************************************************************************************/
bool sdkCommIsRightIP(const u8 *pasIPstr);

/*******************************************************************************************
** Descriptions:    ��������
** Parameters:      pRecvBuf        ���յ�������
                    nRecvBufMaxLen  ���յ���󳤶�
                    RecvTimeOut     ���ճ�ʱʱ�� �Ժ���Ϊ��λ 5000 <= RecvTimeOut <= 120000
** Returned value:  ���ؽ��յ������ݳ���
                    SDK_COMM_WAVELOST   ͨѶ�ж�
                    SDK_PARA_ERR        ��������
                    SDK_TIME_OUT         ͨѶ��ʱ
                    SDK_ESC             �û�ȡ��
                    SDK_COMM_BUFFER_ERROR      �������ݻ���������
                    ����                ͨѶ�����쳣
** Created By:      �Ʊ���(2012-05-07)
** Remarks:         1.	�����յ����ݳ��ȴ����û�ָ���Ľ������ݻ�����󳤶�nRecvMaxLenʱ����������SDK_COMM_BUFFERERR
                    2.	pDisFuncΪNULLʱ����Ĭ����ʾ�������û����Զ�����ʾ���ݣ������ο�DisFuncDemo
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommTmsRecvData(u8 *pheRecvBuf, const s32 usRecvBufMaxLen, const s32 usRecvTimeOut,
                       s32 (*pDisFunc)(const s32 usRecvTimeOut, const s32 usCurrentTime),
                       bool bBreakMode);

/*******************************************************************************************
** Descriptions:    ����PPP��·
** Parameters:      uiTimeOut:��ʱʱ�� ��λms
** Returned value:  SDK_OK                  ����PPP�ɹ�
                    SDK_ERR                 ʧ��
                    SDK_TIME_OUT            ��ʱ
                    SDK_COMM_NO_SIM         û�в����ֻ���//huangbx 20130108
                    SDK_ESC                     �ֶ�ȡ��
** Created By:      �Ʊ���(2013-01-08)
** Remarks:         uiTimeOut Ϊ0���ͽ���PPP�����ֱ�ӷ���,���ж��Ƿ�ɹ�
                    //huangbx 20130128 ����ʯ����Ҫ�� ȫ��ʱ�䵥λ����Ϊ����.
*******************************************************************************************/
s32 sdkCommCreatePPP(u32 uiTimeOut);

/*****************************************************************************
** Descriptions:	��ȡ����ģ���id
** Parameters:          u8 *pasDest:��ȡ������ģ���
** Returned value:	�ɹ�����ģ��ų���
                                �������󷵻�SDK_PARA_ERR
                                ʧ�ܷ��س�ʱ
** Created By:		shijianglong  2013.03.18
** Remarks:
*****************************************************************************/
s32 sdkCommGetWirelessModuleID(u8 * pasDest);


/***********************************###########################*****************************************/
/*����ĺ���Ϊ2.0�汾����*/
/***********************************###########################*****************************************/
/*****************************************************************************
** Descriptions:	��ȡ�����ź�ǿ�ȵȼ�
** Parameters:
** Returned value:	��ʱ����SDK_TIME_OUT
                    �ɹ����������ź�ǿ�ȵȼ���ʮ�����Ʒ�Χ0x00~0x31
** Created By:		shijianglong  2013.08.01
** Remarks:
*****************************************************************************/
s32 sdkCommGetSignalLevel(void);

/*****************************************************************************
** Descriptions:	��ѯͨ��״̬
** Parameters:
** Returned value:	��ʱ����SDK_TIME_OUT
                    �ɹ����������ź�ǿ�ȵȼ���ʮ�����Ʒ�Χ0x00~0x31
** Created By:		shijianglong  2013.08.01
** Remarks:
*****************************************************************************/
s32 sdkCommCheckCommState(u32 timerOut);

/*****************************************************************************
** Descriptions:	��ȡ��վ��Ϣ
** Parameters:          s32 siTimeOut
** Returned value:
** Created By:		fusuipu  2014.07.02
** Remarks:
*****************************************************************************/
s32 sdkCommGetBaseStationInfor(SDK_COMM_STATION_INFO *pstStationInfo, s32 siNodeNum, s32 siTimeOut,
                               void *pVar);

/*****************************************************************************
** Descriptions:	��ȡSIM��ID
** Parameters:          u8 *lpOut
                    s32 uiTimeOut
** Returned value:
** Created By:		fusuipu  2014.08.21
** Remarks:
*****************************************************************************/
s32 sdkCommGetSimID(u8 *lpOut, s32 siTimeOut);

/*****************************************************************************
** Descriptions:	��ȡSIM��IMSI
** Parameters:          u8 *lpOut
                    s32 uiTimeOut
** Returned value:
** Created By:		fusuipu  2014.08.21
** Remarks:
*****************************************************************************/
s32 sdkCommGetSimIMSI(u8 *lpOut, s32 siTimeOut);

/*****************************************************************************
** Descriptions:	����ǰģʽ����ΪVPNͨ��ģʽ
** Parameters:          SDK_COMM_VPN_PARA *pstVpnPara
** Returned value:
** Created By:		fusuipu  2015.01.07
** Remarks:
*****************************************************************************/
s32 sdkCommVPNOpen(SDK_COMM_VPN_PARA * pstVpnPara);

/*****************************************************************************
** Descriptions:	����ǰģʽ����ΪVPNͨ��ģʽ
** Parameters:          SDK_COMM_VPN_PARA *pstVpnPara
** Returned value:
** Created By:		fusuipu  2015.01.07
** Remarks:         VPN��������5S����
*****************************************************************************/
s32 sdkCommVPNClose(s32 siTimeOut);

/*******************************************************************************************
** Descriptions:    ��������ͨ�Ų�����Ϣ
** Parameters:
** Returned value:  SDK_OK          �ɹ�
                    SDK_PARA_ERR    �����Ƿ�
                    SDK_ERR         ����ʧ��
** Created By:      ��־��(2015-03-09)
** Remarks:         ������չͨ�Ų�����Ϣ
                    ��Ҫ�������ú������ͨ�ſ��ܵ���չ������Ϣ
*******************************************************************************************/
s32 sdkSetCommExtPara(const SDK_COMM_EXT_PARA *pstCommOtherInfo);

/*******************************************************************************************
** Descriptions:    ��ȡSIM����Ϣ
** Parameters:
** Returned value:  SDK_OK          �ɹ�
                    SDK_PARA_ERR    �����Ƿ�
                    SDK_ERR         ��������		
** Created By:      ��־��(2015-03-09)
** Remarks:         Ŀǰ��Ҫ���ڵ绰pos
*******************************************************************************************/
s32 sdkCommGetSimInfo(SDK_COMM_SIM_INFO *pstSimInfo);

/*******************************************************************************************
** Descriptions:   ����SIM����Ϣ
** Parameters:
** Returned value:  SDK_OK          �ɹ�
                    SDK_PARA_ERR    �����Ƿ�
                    SDK_ERR         ��������
** Created By:      ��־��(2015-03-09)
** Remarks:         ����SIM����Ϣ                 
*******************************************************************************************/
s32 sdkCommSetSimInfo(SDK_COMM_SIM_INFO *pstSimInfo);

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

