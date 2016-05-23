/*****************************************Copyright(c)***************************************************
**                              Shenshen Xinguodu Technology Co.,Ltd.
**
**                                     http://www.xinguodu.com
**
**------File Info----------------------------------------------------------------------------------------
**Created By:             caoliang
**Created Data:           20150818 
**Version:                V1.0
**Description:            
**
*********************************************************************************************************/
#ifndef __DEV_SPI_H__
#define __DEV_SPI_H__

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;

typedef signed char		s8;
typedef short			s16;
typedef int				s32;

#define     DDI_OK               0   //�ɹ�
#define     DDI_ERR              -1      //����
#define     DDI_ETIMEOUT         -2  //��ʱ
#define     DDI_EBUSY            -3  //�豸��æ
#define     DDI_ENODEV           -4  //�豸������
#define     DDI_EACCES           -5  //��Ȩ��
#define     DDI_EINVAL           -6  //������Ч
#define     DDI_EIO              -7      //�豸δ����豸��������
#define     DDI_EDATA            -8  //���ݴ���
#define     DDI_EPROTOCOL        -9      //Э�����
#define     DDI_ETRANSPORT       -10     //�������
#define     DDI_READED_MAGCARD   -11     //�ǽӶ����ſ�

/********************************��ӡ��״̬����ֵ�Ķ���**********************************/
#define     DDI_PRINTER_WAITING        1//������
#define     DDI_PRINTER_PRINTING    2//����ӡ��
#define     DDI_PRINTER_NO_PAPER    3//��ȱֽ
#define     DDI_PRINTER_OVER_HEAT    4//������


/************************ *********************************************************
***
***   define function pointers as follow
***
**********************************************************************************/

typedef void (*XGDTAG)(void);

//系统
void (sys_uart_printf)(const char *fmt,...);
s32 (spi_ddi_sys_msleep)(u32 nMs);
s32 (spi_ddi_sys_set_time)(const u8 *lpTime);
s32 (spi_ddi_sys_get_time)(u8 *lpTime);
s32 (spi_ddi_sys_get_tick)(u32 *nTick);  //系统滴答
s32 (spi_ddi_sys_read_dsn)(u8 *lpOut);
s32 (spi_ddi_sys_sleep) (s32 status);
s32 (spi_ddi_sys_get_firmwarever) (u8 *lpOut, u32 nType );
s32 (spi_ddi_sys_bat_status) (void );
s32 (spi_ddi_sys_poweroff)(void);
void (spi_ddi_k_malloc)(unsigned nbytes);
void (spi_ddi_k_free)(void *ap);
s32 (spi_ddi_sys_sleep_with_alarm)(s32 sec, s32 type, s32 status);
s32 (spi_ddi_sys_reboot)(void);
s32 (spi_ddi_sys_download)(u32 type);

void spi_ddi_sys_init(void);
void spi_ddi_sys_uninit(void);
void spi_ddi_sys_set_blocktime(u32 nMs);
s32 spi_ddi_sys_get_blocktime(void);

s32 spi_ddi_pin_input(u32 pinlenmin,u32 pinlenmax,u32 timeout,u32 bypassflag);
s32 spi_ddi_pin_input_press(u8 *keycode);
s32 spi_ddi_pin_input_cancel();
s32 spi_ddi_pin_getonlinepinblock (u32 pinAlgorithmMode,u32 keyindex,u32 cardlen,u8 *carddata,u8 *pinblockdata);
s32 spi_ddi_pin_getofflineencpin(u8 *offlineencdata,u32 dataLen);
s32 spi_ddi_pin_offlinepin_apdu(u32 nSlot,u32 pinAlgorithmMode, u8 *lpRApdu,u32 *lpRApduLen,u32 nRApduSize);


//串口
//串口属性结构体：
typedef struct _strComAttr
{
    s32 m_baud;    //波特率，9600、19200、38400、115200
    s32 m_databits;  //数据位，取值：5、6、7、8
    s32 m_parity;    //校验方式：0-无校验；1-奇校验；2-偶校验
    s32 m_stopbits;    //停止位：1 或 2
}strComAttr;

s32 (spi_ddi_com_open) (u32 nCom, const strComAttr *lpstrAttr);
s32 (spi_ddi_com_close) (u32 nCom);
s32 (spi_ddi_com_clear) (u32 nCom);
s32 (spi_ddi_com_read) (u32 nCom, u8 *lpOut, s32 nLe );
s32 (spi_ddi_com_write) (u32 nCom, u8 *lpIn, s32 nLe );
s32 (spi_ddi_com_ioctl)(u32 nCom, u32 nCmd, u32 lParam, u32 wParam);

//磁卡
s32 (spi_ddi_mag_open) (void);
s32 (spi_ddi_mag_close) (void);
s32 (spi_ddi_mag_clear)(void);
s32 (spi_ddi_mag_read) (u8 *lpTrack1, u8 *lpTrack2, u8 *lpTrack3);
s32 (spi_ddi_mag_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//IC PSAM
s32 (spi_ddi_iccpsam_open) (u32 nSlot);
s32 (spi_ddi_iccpsam_close)(u32 nSlot);
s32 (spi_ddi_iccpsam_poweron) (u32 nSlot, u8 *lpAtr);
s32 (spi_ddi_iccpsam_poweroff) (u32 nSlot);
s32 (spi_ddi_iccpsam_get_status) (u32 nSlot);
s32 (spi_ddi_iccpsam_exchange_apdu) (u32 nSlot, const u8* lpCApdu, u32 nCApduLen, u8*lpRApdu, u32* lpRApduLen, u32 nRApduSize);
s32 (spi_ddi_iccpsam_ioctl)(u32 nCmd, u32 lParam, u32 wParam);


//非接卡操作
s32 (spi_ddi_rf_open) (void);
s32 (spi_ddi_rf_close)(void);
s32 (spi_ddi_rf_poweron) (u32 nType);
s32 (spi_ddi_rf_poweroff) (void);
s32 (spi_ddi_rf_get_status) (void);
s32 (spi_ddi_rf_activate) (void);
s32 (spi_ddi_rf_exchange_apdu) (const u8*lpCApdu, u32 nCApduLen, u8*lpRApdu, u32 *lpRApduLen, u32 nRApduSize);
s32 (spi_ddi_rf_remove) (void);
s32 (spi_ddi_rf_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//热敏打印
//lpPrnCtrl：
//打印文本控制结构体：
typedef struct _strPrnTextCtrl
{
    u32   m_align;       //对齐方式
    u32   m_offset;            //列偏移
    u32  m_font;//字体，关联 ASCII 字符
    u32  m_ascsize;//ASCII 字符字号
    u32  m_asczoom;    //ASCII 字符放大方式
    u32  m_nativesize;//国语字号
    u32  m_nativezoom;   //国语放大方式
} strPrnTextCtrl;
//m_align:  对齐方式，0：左对齐  1：右对齐  2：居中对齐
//m_offset:  列偏移像素点个数，范围（0~383）

//nNum:  组合打印文本项个数
//lpPrnCombTextCtrl：打印文本控制结构体：
typedef struct _strPrnCombTextCtrl
{
    u32   m_x0;            //基于原点的横坐标
    u32   m_y0;            //基于原点的纵坐标
    u32  m_font;             //字体，关联 ASCII 字符
    u32  m_ascsize;          //ASCII 字符字号
    u32  m_asczoom;         //ASCII 字符放大方式
    u32  m_nativesize;        //国语字号
    u32  m_nativezoom;      //国语放大方式
    u8*   m_text;            //打印文本内容
}strPrnCombTextCtrl;
//m_x0:  基于原点的横坐标，范围（0~383）
//m_y0:  基于原点的纵坐标，范围（0~239）
//m_ font:  字体类型值
//0：宋体
//1：黑体
//2：幼圆
//m_ ascsize：ASCII 字符字号
//0：6*12
//1：8*16
//2：12*24
//3：16*32
//m_ asczoom：ASCII 字符放大方式
//0：基本字号
//1：纵向倍高
//2：横向倍宽
//3：整倍放大
//m_ nativesize：国语字符字号
//0：12*12
//1：16*16
//2：20*20
//3：24*24
//4：32*32
//m_ nativezoom：国语字符放大方式 
//	0：基本字号
//	1：纵向倍高
//	2：横向倍宽
//	3：整倍放大
//	m_text：打印文本内容，格式：以’\0’结束的字符串
s32 (spi_ddi_thmprn_open) (void);
s32 (spi_ddi_thmprn_close)(void);
s32 (spi_ddi_thmprn_feed_paper)(u32 nPixels);
s32 (spi_ddi_thmprn_print_image) (u32 nOrgLeft, u32 nImageWidth, u32 nImageHeight, const u8 *lpImage);
s32 (spi_ddi_thmprn_print_image_file)(u32 nOrgLeft, u32 nImageWidth, u32 nImageHeight, const u8 *lpImageName);
s32 (spi_ddi_thmprn_print_text) (strPrnTextCtrl *lpPrnCtrl, const u8 *lpText );
s32 (spi_ddi_thmprn_print_comb_text) (u32 nNum, const strPrnCombTextCtrl* lpPrnCombTextCtrl[] );
s32 (spi_ddi_thmprn_get_status) (void);
s32 (spi_ddi_thmprn_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//按键DDI
s32 (spi_ddi_key_open)(void);
s32 (spi_ddi_key_close)(void);
s32 (spi_ddi_key_clear)(void);
s32 (spi_ddi_key_read)(u32 *lpKey);
s32 (spi_ddi_key_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//显示

//lpstrLine：线端结构体。
typedef struct _strLine
{
    u32  m_x0;//线起点 X 坐标
    u32  m_y0;//线起点 Y 坐标
    u32  m_x1;//线终点 X 坐标
    u32  m_y1;//线终点 Y 坐标
} strLine ;

//lpstrRect：图形显示矩形区域。
typedef struct _strRect
{
    u32  m_x0;//矩形起点 X 坐标
    u32  m_y0;//矩形起点 Y 坐标
    u32  m_x1;//矩形终点 X 坐标
    u32  m_y1;//矩形终点 Y 坐标
} strRect;
//lpstrPic：图形数据。
typedef struct _strPicture
{
    u32  m_width;//图形像素点宽度
    u32  m_height;//图形像素点高度
    u32  * m_pic;//图形像素数据
} strPicture;
//m_pic：格式：aRGB,  排列规则：横向排列，从左到右、从上到下

s32 (spi_ddi_lcd_open) (void);
s32 (spi_ddi_lcd_close) (void);
s32 (spi_ddi_lcd_fill_rect) (const strRect *lpstrRect, u32 nRGB);
s32 (spi_ddi_lcd_clear_rect) (const strRect *lpstrRect);
s32 (spi_ddi_lcd_show_text) (u32 nX, u32 nY, const u8* lpText);
s32 (spi_ddi_lcd_show_picture)(const strRect *lpstrRect, const strPicture * lpstrPic);
s32 (spi_ddi_lcd_show_picture_file)(const strRect *lpstrRect,const u8 * lpBmpName);//显示图形文件
s32 (spi_ddi_lcd_show_pixel) (u32 nX, u32 nY);
s32 (spi_ddi_lcd_show_line)(const strLine  * lpstrLine );//划线
s32 (spi_ddi_lcd_show_rect)(const strRect  * lpstrRect);
s32 (spi_ddi_lcd_extract_rect)(const strRect *lpstrRect, strPicture * lpstrPic);
s32 (spi_ddi_lcd_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//音频
s32 (spi_ddi_audio_open) (void);
s32 (spi_ddi_audio_close) (void);
s32 (spi_ddi_audio_play) (const u8 * lpFile);
s32 (spi_ddi_audio_stop) (void);
s32 (spi_ddi_audio_pause) (void);
s32 (spi_ddi_audio_replay) (void);
s32 (spi_ddi_audio_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//触摸屏设备
//strTpPixels：
typedef struct _strTpPoint
{
    u32  m_x;//触摸屏点的横坐标
    u32  m_y;//触摸屏点的纵坐标
    u32  m_pressure;//触摸压力(电阻屏有效)
} strTpPoint;
//有效像素点集合结构体：
typedef struct _strTpPixels
{
    u32   m_num;            //有效像素点个数
    strTpPoint m_point[5];    //有效像素点参数
    u32   m_gesture;           //手势，
}strTpPixels;
//m_num：有效像素点个数，最大支持 5 点触摸。
//m_gesture：手势，取值范围（0~11） ，此域在手势模式下有效
//0：无手势
//1：滑动手势
//2：轻点手势
//3：双轻点手势
//4：按压手势
//5：缩小手势
//6：放大手势
//7：转动手势
//8：向上手势
//9：向下手势
//10：向左手势
//11：向右手势
//
s32 (spi_ddi_tp_open) (void);
s32 (spi_ddi_tp_close) (void);
s32 (spi_ddi_tp_read_pixels)(strTpPixels *lpTpPixels);
s32 (spi_ddi_tp_clear) (void);
s32 (spi_ddi_tp_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//无线通信
s32 (spi_ddi_wireless_open) (void);
s32 (spi_ddi_wireless_close)(void);
s32 (spi_ddi_wireless_destroy)(void);
s32 (spi_ddi_wireless_dial) (const u8 *lpApn, const u8 *lpUser, const u8 *lpPwd);
s32 (spi_ddi_wireless_hangup) (void);
s32 (spi_ddi_wireless_get_status) (void);
s32 (spi_ddi_wireless_sleep) (void);//内部保留使用
s32 (spi_ddi_wireless_wakeup) (void);
s32 (spi_ddi_wireless_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//modem

//lpstrDialPara：
//modem 拨号参数结构体：
typedef struct _strModemDialPara
{
    s32 m_mode;    //拨号方式
    u8 m_telnum[3][32];  //拨号号码，格式为以’\0’结束的字符串
    s32 m_timeout;    //拨号总超时时间，单位为秒
}strModemDialPara;
//m_mode:  拨号方式
//0：同步
//1：异步快速拨号
//2：异步慢速拨号
//3：FSK

s32 (spi_ddi_modem_open) (void);
s32 (spi_ddi_modem_close)(void);
s32 (spi_ddi_modem_dial) (const strModemDialPara *lpstrDialPara);
s32 (spi_ddi_modem_hangup) (void);
s32 (spi_ddi_modem_get_status) (void);
s32 (spi_ddi_modem_read) (u8 *lpOut, s32 nLe );
s32 (spi_ddi_modem_write)(u8 *lpIn, s32 nLe );
s32 (spi_ddi_modem_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//内置密钥
s32 (spi_ddi_innerkey_open) (void);
s32 (spi_ddi_innerkey_close)(void);
s32 (spi_ddi_innerkey_inject) (u32 nKeyArea, u32 nIndex, const u8* lpKeyData);
s32 (spi_ddi_innerkey_encrypt)(u32 nKeyArea, u32 nIndex, u32 nLen, const u8* lpIn, u8* lpOut);
s32 (spi_ddi_innerkey_decrypt)(u32 nKeyArea, u32 nIndex, u32 nLen, const u8* lpIn, u8* lpOut);
s32 (spi_ddi_innerkey_ioctl)(u32 nCmd, u32 lParam, u32 wParam);
typedef struct _HSM_ObjectProperty
{
	u8 m_CertName[10];
	u8 m_StrID[32];
	u8 m_StrLabel[32];
	u8 m_nObjectType;
	u8 m_nDataType;
	u8 m_nDataLength[4];
}HSM_ObjectProperty;

//证书
s32 (spi_ddi_certmodule_open) (void);
s32 (spi_ddi_certmodule_close)(void);
s32 (spi_ddi_certmodule_save) (HSM_ObjectProperty *pObjectProperty,u8 * pObjectData,u32 nDataLength);
s32 (spi_ddi_certmodule_readByName )(HSM_ObjectProperty *pObjectProperty,u8 * pObjectData,u32 *nDataLength);
s32 (spi_ddi_certmodule_readByID)(HSM_ObjectProperty *pObjectProperty,u8 * pObjectData,u32 *nDataLength);
s32 (spi_ddi_certmodule_querycount)(u32 *certnum);
s32 (spi_ddi_certmodule_delete)(u32 certid,HSM_ObjectProperty *pObjectProperty,u8 *verifydata);
s32 (spi_ddi_certmodule_deleteall)(void);

//虚拟文件系统
s32 (spi_ddi_vfs_open) (const u8 *lpPath, const u8 *lpMode);
s32 (spi_ddi_vfs_close) (s32 nHandle);
s32 (spi_ddi_vfs_write) (const u8*lpData , s32 nLen, s32 nHandle);
s32 (spi_ddi_vfs_read) (u8 * lpData, s32 nLen, s32 nHandle);
s32 (spi_ddi_vfs_seek)(s32 nHandle, s32 nOffset, s32 nWhence);
s32 (spi_ddi_vfs_tell) (s32 nHandle);
s32 (spi_ddi_vfs_remove) (const u8 *lpFileName);
s32 (spi_ddi_vfs_rename)(const u8 *lpOldName, const u8 *lpNewName);
s32 (spi_ddi_vfs_access)(const u8 *lpName);
s32 (spi_ddi_vfs_ioctl)(u32 nCmd, u32 lParam, u32 wParam);


//蓝牙
s32 (spi_ddi_bt_open) (void);
s32 (spi_ddi_bt_close)(void);
s32 (spi_ddi_bt_disconnect)(const u8 *lpMac);
s32 (spi_ddi_bt_write)(u8 *lpIn, s32 nLe);
s32 (spi_ddi_bt_read)(u8 *lpOut, s32 nLe );
s32 (spi_ddi_bt_get_status)(void);
s32 (spi_ddi_bt_ioctl)(u32 nCmd, u32 lParam, u32 wParam);


//音频通信
s32 (spi_ddi_audiocom_open) (void);
s32 (spi_ddi_audiocom_close) (void);
s32 (spi_ddi_audiocom_clear) (void);
s32 (spi_ddi_audiocom_read) (u8 *lpOut, s32 nLe);
s32 (spi_ddi_audiocom_write) (u8 *lpIn, s32 nLe);
s32 (spi_ddi_audiocom_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//LED
s32 (spi_ddi_led_open) (void);
s32 (spi_ddi_led_close) (void);
s32 (spi_ddi_led_sta_set) (u32 nLed, u32 nSta);
s32 (spi_ddi_led_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//USB
s32 (spi_ddi_usb_open)(u32 nClass);
s32 (spi_ddi_usb_close)(u32 nClass);
s32 (spi_ddi_usb_read)(u32 nClass, u8 *lpOut, u32 nLe);
s32 (spi_ddi_usb_write) (u32 nClass, u8 *lpIn, u32 nLe);
s32 (spi_ddi_usb_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

//dukpt
//lpstrDukptInitInfo：DUKPT 初始化参数
typedef struct _strDukptInitInfo
{
    u8 m_groupindex;     //密钥组号 0~1
    u8 m_keyindex;       //密钥索引号 0~9
    u8 m_initkey[24];     //初始密钥
    u8 m_keylen;         //初始密钥长度,固定为 16
    u8 m_ksnindex;       //ksn 索引号,目前与密钥索引号要保持一致
    u8 m_initksn[20];      //初始 ksn
    u8 m_ksnlen;         //初始 ksn 长度,固定为 10
}strDukptInitInfo;


s32 (spi_ddi_dukpt_open)(void);
s32 (spi_ddi_dukpt_close)(void);
s32 (spi_ddi_dukpt_inject) (strDukptInitInfo* lpstrDukptInitInfo);
s32 (spi_ddi_dukpt_encrypt)(u32 nKeyGroup, u32 nKeyIndex, u32 nLen, const u8* lpIn, u8* lpOut, u32 nMode);
s32 (spi_ddi_dukpt_decrypt)(u32 nKeyGroup, u32 nKsnIndex, u32 nLen, const u8* lpIn, u8* lpOut, u32 nMode);
s32 (spi_ddi_dukpt_getksn)(u32 nKeyGroup, u32 nKeyIndex, u32 *lpLen, u8* lpOut);
s32 (spi_ddi_dukpt_ioctl)(u32 nCmd, u32 lParam, u32 wParam);

/*********************************************************************************
***
***   define the struct contains all the function as follow
***
*********************************************************************************/
#if 0
struct  __core
{
    //系统
    core_sys_uart_printf    CoreApi_sys_uart_printf;
    core_spi_ddi_sys_msleep     CoreApi_spi_ddi_sys_msleep;
    core_spi_ddi_sys_set_time   CoreApi_spi_ddi_sys_set_time;
    core_spi_ddi_sys_get_time   CoreApi_spi_ddi_sys_get_time;
    core_spi_ddi_sys_get_tick   CoreApi_spi_ddi_sys_get_tick; //系统滴答
    core_spi_ddi_sys_read_dsn   CoreApi_spi_ddi_sys_read_dsn; 
    core_spi_ddi_sys_sleep      CoreApi_spi_ddi_sys_sleep;
    core_spi_ddi_sys_get_firmwarever CoreApi_spi_ddi_sys_get_firmwarever;
    core_spi_ddi_sys_bat_status CoreApi_spi_ddi_sys_bat_status;
    core_spi_ddi_sys_poweroff   CoreApi_spi_ddi_sys_poweroff;
    
    core_spi_ddi_k_malloc       CoreApi_spi_ddi_k_malloc;
    core_spi_ddi_k_free         CoreApi_spi_ddi_k_free;
    core_spi_ddi_sys_sleep_with_alarm CoreApi_spi_ddi_sys_sleep_with_alarm;
    core_spi_ddi_sys_reboot     CoreApi_spi_ddi_sys_reboot;
    core_spi_ddi_sys_download       CoreApi_spi_ddi_sys_download;
    char *Reserved1[(32-15)];

    //串口
    core_spi_ddi_com_open       CoreApi_spi_ddi_com_open;
    core_spi_ddi_com_close      CoreApi_spi_ddi_com_close;
    core_spi_ddi_com_clear      CoreApi_spi_ddi_com_clear;
    core_spi_ddi_com_read       CoreApi_spi_ddi_com_read;
    core_spi_ddi_com_write      CoreApi_spi_ddi_com_write;
    core_spi_ddi_com_ioctl      CoreApi_spi_ddi_com_ioctl;
    char *Reserved2[(32-6)];

    //磁卡
    core_spi_ddi_mag_open       CoreApi_spi_ddi_mag_open;
    core_spi_ddi_mag_close      CoreApi_spi_ddi_mag_close;
    core_spi_ddi_mag_clear      CoreApi_spi_ddi_mag_clear;
    core_spi_ddi_mag_read       CoreApi_spi_ddi_mag_read;
    core_spi_ddi_mag_ioctl      CoreApi_spi_ddi_mag_ioctl;
    char *Reserved3[(32-5)];
    
    //IC PSAM
    core_spi_ddi_iccpsam_open       CoreApi_spi_ddi_iccpsam_open;
    core_spi_ddi_iccpsam_close      CoreApi_spi_ddi_iccpsam_close;
    core_spi_ddi_iccpsam_poweron    CoreApi_spi_ddi_iccpsam_poweron;
    core_spi_ddi_iccpsam_poweroff   CoreApi_spi_ddi_iccpsam_poweroff;
    core_spi_ddi_iccpsam_get_status CoreApi_spi_ddi_iccpsam_get_status;
    core_spi_ddi_iccpsam_exchange_apdu  CoreApi_spi_ddi_iccpsam_exchange_apdu;
    core_spi_ddi_iccpsam_ioctl      CoreApi_spi_ddi_iccpsam_ioctl;
    char *Reserved4[(32-7)];


    //非接卡操作
    core_spi_ddi_rf_open            CoreApi_spi_ddi_rf_open;
    core_spi_ddi_rf_close           CoreApi_spi_ddi_rf_close;
    core_spi_ddi_rf_poweron         CoreApi_spi_ddi_rf_poweron;
    core_spi_ddi_rf_poweroff        CoreApi_spi_ddi_rf_poweroff;
    core_spi_ddi_rf_get_status      CoreApi_spi_ddi_rf_get_status;
    core_spi_ddi_rf_activate        CoreApi_spi_ddi_rf_activate;
    core_spi_ddi_rf_exchange_apdu   CoreApi_spi_ddi_rf_exchange_apdu;
    core_spi_ddi_rf_remove          CoreApi_spi_ddi_rf_remove;
    core_spi_ddi_rf_ioctl           CoreApi_spi_ddi_rf_ioctl;
    char *Reserved5[(32-9)];

    //热敏打印
    core_spi_ddi_thmprn_open            CoreApi_spi_ddi_thmprn_open;
    core_spi_ddi_thmprn_close           CoreApi_spi_ddi_thmprn_close;
    core_spi_ddi_thmprn_feed_paper      CoreApi_spi_ddi_thmprn_feed_paper;
    core_spi_ddi_thmprn_print_image     CoreApi_spi_ddi_thmprn_print_image;
    core_spi_ddi_thmprn_print_image_file     CoreApi_spi_ddi_thmprn_print_image_file;
    core_spi_ddi_thmprn_print_text      CoreApi_spi_ddi_thmprn_print_text;
    core_spi_ddi_thmprn_print_comb_text CoreApi_spi_ddi_thmprn_print_comb_text;
    core_spi_ddi_thmprn_get_status      CoreApi_spi_ddi_thmprn_get_status;
    core_spi_ddi_thmprn_ioctl           CoreApi_spi_ddi_thmprn_ioctl;
    char *Reserved6[(32-9)];
    
    //按键
    core_spi_ddi_key_open   CoreApi_spi_ddi_key_open;
    core_spi_ddi_key_close  CoreApi_spi_ddi_key_close;
    core_spi_ddi_key_clear  CoreApi_spi_ddi_key_clear;
    core_spi_ddi_key_read   CoreApi_spi_ddi_key_read;
    core_spi_ddi_key_ioctl  CoreApi_spi_ddi_key_ioctl;
    char *Reserved7[(32-5)];
        
    //显示
    core_spi_ddi_lcd_open               CoreApi_spi_ddi_lcd_open;
    core_spi_ddi_lcd_close              CoreApi_spi_ddi_lcd_close;
    core_spi_ddi_lcd_fill_rect          CoreApi_spi_ddi_lcd_fill_rect;
    core_spi_ddi_lcd_clear_rect         CoreApi_spi_ddi_lcd_clear_rect;
    core_spi_ddi_lcd_show_text          CoreApi_spi_ddi_lcd_show_text;
    core_spi_ddi_lcd_show_picture       CoreApi_spi_ddi_lcd_show_picture;
    core_spi_ddi_lcd_show_picture_file  CoreApi_spi_ddi_lcd_show_picture_file;
    core_spi_ddi_lcd_show_pixel         CoreApi_spi_ddi_lcd_show_pixel;
    core_spi_ddi_lcd_show_line          CoreApi_spi_ddi_lcd_show_line;//划线
    core_spi_ddi_lcd_show_rect          CoreApi_spi_ddi_lcd_show_rect;
    core_spi_ddi_lcd_extract_rect       CoreApi_spi_ddi_lcd_extract_rect;
    core_spi_ddi_lcd_ioctl              CoreApi_spi_ddi_lcd_ioctl;
    char *Reserved8[(32-12)];

    //音频
    core_spi_ddi_audio_open     CoreApi_spi_ddi_audio_open;
    core_spi_ddi_audio_close    CoreApi_spi_ddi_audio_close;
    core_spi_ddi_audio_play     CoreApi_spi_ddi_audio_play;
    core_spi_ddi_audio_stop     CoreApi_spi_ddi_audio_stop;
    core_spi_ddi_audio_pause    CoreApi_spi_ddi_audio_pause;
    core_spi_ddi_audio_replay   CoreApi_spi_ddi_audio_replay;
    core_spi_ddi_audio_ioctl    CoreApi_spi_ddi_audio_ioctl;
    char *Reserved9[(32-7)];

    //触摸屏设备
    core_spi_ddi_tp_open        CoreApi_spi_ddi_tp_open;
    core_spi_ddi_tp_close       CoreApi_spi_ddi_tp_close;
    core_spi_ddi_tp_read_pixels CoreApi_spi_ddi_tp_read_pixels;
    core_spi_ddi_tp_clear       CoreApi_spi_ddi_tp_clear;
    core_spi_ddi_tp_ioctl       CoreApi_spi_ddi_tp_ioctl;
    char *Reserved10[(32-5)];

    //无线通信
    core_spi_ddi_wireless_open          CoreApi_spi_ddi_wireless_open;
    core_spi_ddi_wireless_close         CoreApi_spi_ddi_wireless_close;
    core_spi_ddi_wireless_destroy       CoreApi_spi_ddi_wireless_destroy;
    core_spi_ddi_wireless_dial          CoreApi_spi_ddi_wireless_dial;
    core_spi_ddi_wireless_hangup        CoreApi_spi_ddi_wireless_hangup;
    core_spi_ddi_wireless_get_status    CoreApi_spi_ddi_wireless_get_status;
    core_spi_ddi_wireless_sleep         CoreApi_spi_ddi_wireless_sleep;//内部保留使用
    core_spi_ddi_wireless_wakeup        CoreApi_spi_ddi_wireless_wakeup;
    core_spi_ddi_wireless_ioctl         CoreApi_spi_ddi_wireless_ioctl;
    char *Reserved11[(32-9)];

    //modem
    core_spi_ddi_modem_open         CoreApi_spi_ddi_modem_open;
    core_spi_ddi_modem_close        CoreApi_spi_ddi_modem_close;
    core_spi_ddi_modem_dial         CoreApi_spi_ddi_modem_dial;
    core_spi_ddi_modem_hangup       CoreApi_spi_ddi_modem_hangup;
    core_spi_ddi_modem_get_status   CoreApi_spi_ddi_modem_get_status;
    core_spi_ddi_modem_read         CoreApi_spi_ddi_modem_read;
    core_spi_ddi_modem_write        CoreApi_spi_ddi_modem_write;
    core_spi_ddi_modem_ioctl        CoreApi_spi_ddi_modem_ioctl;
    char *Reserved12[(32-8)];

    //内置密钥
    core_spi_ddi_innerkey_open      CoreApi_spi_ddi_innerkey_open;
    core_spi_ddi_innerkey_close     CoreApi_spi_ddi_innerkey_close;
    core_spi_ddi_innerkey_inject    CoreApi_spi_ddi_innerkey_inject;
    core_spi_ddi_innerkey_encrypt   CoreApi_spi_ddi_innerkey_encrypt;
    core_spi_ddi_innerkey_decrypt   CoreApi_spi_ddi_innerkey_decrypt;
    core_spi_ddi_innerkey_ioctl     CoreApi_spi_ddi_innerkey_ioctl;
    char *Reserved13[(32-6)];

    //虚拟文件系统
    core_spi_ddi_vfs_open       CoreApi_spi_ddi_vfs_open;
    core_spi_ddi_vfs_close      CoreApi_spi_ddi_vfs_close;
    core_spi_ddi_vfs_write      CoreApi_spi_ddi_vfs_write;
    core_spi_ddi_vfs_read       CoreApi_spi_ddi_vfs_read;
    core_spi_ddi_vfs_seek       CoreApi_spi_ddi_vfs_seek;
    core_spi_ddi_vfs_tell       CoreApi_spi_ddi_vfs_tell;
    core_spi_ddi_vfs_remove     CoreApi_spi_ddi_vfs_remove;
    core_spi_ddi_vfs_rename     CoreApi_spi_ddi_vfs_rename;
    core_spi_ddi_vfs_access     CoreApi_spi_ddi_vfs_access;
    core_spi_ddi_vfs_ioctl      CoreApi_spi_ddi_vfs_ioctl;    
    char *Reserved14[(32-10)];

    //蓝牙
    core_spi_ddi_bt_open        CoreApi_spi_ddi_bt_open;
    core_spi_ddi_bt_close       CoreApi_spi_ddi_bt_close;
    core_spi_ddi_bt_disconnect  CoreApi_spi_ddi_bt_disconnect;
    core_spi_ddi_bt_write       CoreApi_spi_ddi_bt_write;
    core_spi_ddi_bt_read        CoreApi_spi_ddi_bt_read;
    core_spi_ddi_bt_get_status  CoreApi_spi_ddi_bt_get_status;
    core_spi_ddi_bt_ioctl       CoreApi_spi_ddi_bt_ioctl;
    char *Reserved15[(32-7)];
    
    //音频通信
    core_spi_ddi_audiocom_open      CoreApi_spi_ddi_audiocom_open;
    core_spi_ddi_audiocom_close     CoreApi_spi_ddi_audiocom_close;
    core_spi_ddi_audiocom_clear     CoreApi_spi_ddi_audiocom_clear;
    core_spi_ddi_audiocom_read      CoreApi_spi_ddi_audiocom_read;
    core_spi_ddi_audiocom_write     CoreApi_spi_ddi_audiocom_write;
    core_spi_ddi_audiocom_ioctl     CoreApi_spi_ddi_audiocom_ioctl;
    char *Reserved16[(32-6)];

    //LED
    core_spi_ddi_led_open       CoreApi_spi_ddi_led_open;
    core_spi_ddi_led_close      CoreApi_spi_ddi_led_close;
    core_spi_ddi_led_sta_set    CoreApi_spi_ddi_led_sta_set;
    core_spi_ddi_led_ioctl      CoreApi_spi_ddi_led_ioctl;    
    char *Reserved17[(32-4)];

    //USB
    core_spi_ddi_usb_open   CoreApi_spi_ddi_usb_open;
    core_spi_ddi_usb_close  CoreApi_spi_ddi_usb_close;
    core_spi_ddi_usb_read   CoreApi_spi_ddi_usb_read;
    core_spi_ddi_usb_write  CoreApi_spi_ddi_usb_write;
    core_spi_ddi_usb_ioctl  CoreApi_spi_ddi_usb_ioctl;
    char *Reserved18[(32-5)];

    //dukpt
    core_spi_ddi_dukpt_open     CoreApi_spi_ddi_dukpt_open;
    core_spi_ddi_dukpt_close    CoreApi_spi_ddi_dukpt_close;
    core_spi_ddi_dukpt_inject   CoreApi_spi_ddi_dukpt_inject;
    core_spi_ddi_dukpt_encrypt  CoreApi_spi_ddi_dukpt_encrypt;
    core_spi_ddi_dukpt_decrypt  CoreApi_spi_ddi_dukpt_decrypt;
    core_spi_ddi_dukpt_getksn   CoreApi_spi_ddi_dukpt_getksn;
    core_spi_ddi_dukpt_ioctl    CoreApi_spi_ddi_dukpt_ioctl;
    char *Reserved19[(32-7)];
};
#endif

#endif
