#ifndef AFX_CONST_H
#define AFX_CONST_H

/*****************�Զ����ض�ʱ****************************/
#define TV_SHOW 3*1000
#define TV_BALANCE 5*1000
#define TV_AUTORET 60*1000

#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*****************ͼƬ·��****************************/
#define WELCOMBMP         "/mtd0/res/welcome.bmp"       //��ӭ����LOGO��ɫ����ͼ
#define BACKGROUNDBMP "/mtd0/res/background.bmp"    //��ɫ����ͼ

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

/*****************��Ϣ����****************************/
#define         SDK_MSG_OUT             0                       //����Ϣ������Ϻ�Ӷ������
#define         SDK_MSG_KEEP            1                       //����Ϣ������Ϻ��ƶ�������β��

/*****************���״̬****************************/
enum BATTERYSTATE {
    BATTERY_ERR = -2,               //�޵��
    BATTERY_LOW = -1,          //��ص�����
    BATTERY_0 = 0,                  //�޵���
    BATTERY_1 = 1,                  //1�����
    BATTERY_2 = 2,                  //2�����
    BATTERY_3 = 3,                  //3�����
    BATTERY_HIDE = 98,          //����ʾ���ͼ��
    BATTERY_MAX = 99,           //��س���
};

/***********************ͨ�ŷ�ʽ����*************************/
enum COMMUTYPE {
    MODEM = 1,                                                  //�绰ͨ��
    GPRS,                                                                                          //GPRS�ƶ�ͨ��
    CDMA,                                                                                          //CDMA�ƶ�ͨ��
    COM,                                                                          //COMͨ�ŷ�ʽ
    LAN,                                                                          //LAN
    WIFI,                                                                          //wifi communication mode
    BLUETOOTH,                                                              //����ͨѶ��ʽ
    AUDIO,                                                                      //��ƵͨѶ
    USB_CDC,                                    //usbת����
    USB_HID,                                       //usb HID
};

//����ID �������ֽ���
enum _TradeId {
    TRADEID_UNKNOWN = 0x00,
    TRADEID_WELCOME = 0x01,                     //��ӭ����
    TRADEID_MAINMENU,                           //������
    TRADEID_MPOS_MENU,
    TRADEID_SYS_SETTING_MENU, //ljh2015-01-09
            TRADEID_BT_SETTING_MENU,
    TRADEID_FIRMWARE_UPDATE,            //���¹̼�
};

#define MANUALMINCARDNUM        13

enum RLST {
    SUCCESS,
    UNSUCCESS
};


#endif


