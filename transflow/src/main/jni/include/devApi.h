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
#ifndef __DEV_H__
#define __DEV_H__

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;

typedef signed char		s8;
typedef short			s16;
typedef int				s32;

#define   DDI_OK  0  //成功
#define   DDI_ERR  -1  //错误
#define   DDI_ETIMEOUT  -2  //超时
#define   DDI_EBUSY  -3  //设备繁忙
#define   DDI_ENODEV   -4 // 设备不存在
#define   DDI_EACCES   -5 // 无权限
#define   DDI_EINVAL   -6 // 参数无效
#define   DDI_EIO  -7  //设备未打开或设备操作出错
#define   DDI_EDATA  -8 // 数据错误
#define   DDI_EPROTOCOL  -9 // 协议错误
#define   DDI_ETRANSPORT  -10 // 传输错误

#define   BLUE_LED           1
#define   YELLOW_LED         2
#define   GREEN_LED          3
#define   RED_LED            4
#define   ALL_LED           0xff

#define   LED_OFF          0
#define   LED_ON           1
#define   PRINT_LOW_POWER           5
#define   PRINT_LOW_TEMP           6
#define   PRINT_M_POWER           2
#define   PRINT_M_TEMP           3
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
s32 spi_ddi_spi_communication_test (u8 *lpout, s32 nLe );
s32 ddi_sys_get_Hardware_Ver();
s32 spi_ddi_sys_set_dsn (s32 len,u8 *dsn);

void tp_timer(int sig);
int BodyNumber_process_download(void);
int BodyNumber_process_serial_read(void);
int open_port(void);
int close_port(void);


s32 spi_ddi_security_getstatus (u8 *status);
s32 ddi_sys_setCertHash(u8* hashData);
s32 ddi_sys_getCertHash(u8 *hash);

s32 spi_k21_ddi_com_clear (u32 nCom);
s32 spi_k21_ddi_com_read (u32 nCom, u8 *lpOut, s32 nLe );
s32 spi_k21_ddi_com_write (u32 nCom, u8 *lpIn, s32 nLe );


s32 spi_ddi_security_rand(u8 *rand);
s32 spi_ddi_get_debugStatus(void);
s32 spi_ddi_set_debugStatus(u8 status,u8 *password);
s32 spi_ddi_delete_cert_byPassword(u8 *password);
s32 spi_ddi_sys_set_beep(u32 time);

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

s32 printOneBitBMPImage(u8 *image);
s32 printOneBitBMPImageByBuffer(u8 *image);
s32 sys_mainBat_status(void);
s32 spi_ddi_sys_set_timeout(u32 timeout);
s32 spi_ddi_sys_get_timeout(u32* pTimeout);
s32 ddi_sys_get_chipID(u8* pChipID);
s32 printBlackBlock(u32 line);

s32 spi_ddi_thmprn_dot(u8* data,s32 dataLen);
s32 spi_ddi_thmprn_totalDot(u8 *buf,u32 len);
//串口
//串口属性结构体：
typedef struct _strComAttr
{
	s32 m_baud;    //波特率，9600、19200、38400、115200
	s32 m_databits;  //数据位，取值：5、6、7、8
	s32 m_parity;    //校验方式：0-无校验；1-奇校验；2-偶校验
	s32 m_stopbits;    //停止位：1 或 2
}strComAttr;
struct com_cfg_s{
	s32 baud; //波特率(1200 - 230400)
	s32 bits; //数据位(5，6，7, 8)
	s32 stops; //停止位（1，2）
	s32 parity;
	s32 flowctl; //流控制 0无流控制 1 硬件流 2 软件流
};
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
s32 (spi_ddi_mag_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);
//IC PSAM
s32 (spi_ddi_iccpsam_open) (u32 nSlot);
s32 (spi_ddi_iccpsam_close)(u32 nSlot);
s32 (spi_ddi_iccpsam_poweron) (u32 nSlot, u8 *lpAtr);
s32 (spi_ddi_iccpsam_poweroff) (u32 nSlot);
s32 (spi_ddi_iccpsam_get_status) (u32 nSlot);
s32 (spi_ddi_iccpsam_exchange_apdu) (u32 nSlot, const u8* lpCApdu, u32 nCApduLen, u8*lpRApdu, u32* lpRApduLen, u32 nRApduSize);
s32 (spi_ddi_iccpsam_ioctl)(u32 nCmd, u32 lParam, u32 wParam);
s32 (spi_ddi_iccpsam_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);

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
s32 (spi_ddi_rf_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);
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
s32 (spi_ddi_thmprn_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);
s32 (spi_ddi_thmprn_test) (void);

//按键DDI
s32 (spi_ddi_key_open)(void);
s32 (spi_ddi_key_close)(void);
s32 (spi_ddi_key_clear)(void);
s32 (spi_ddi_key_read)(u32 *lpKey);
s32 (spi_ddi_key_ioctl_for_java)(u32 nCmd, u32 lParam, u32 wParam);

int setGpioValue(int gpio_no, int gpio_val); 
int getGpioValue(int gpio_no);
int setScannerEnable(int on);
int getScannerEnableStatus(void);
int setScannerPowerOn(int on);
int getScannerPowerOnStatus(void);
int setScannerUsbSwitch(int on);
int getScannerUsbSwitchStatus(void);

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
s32 (spi_ddi_innerkey_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);
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
s32 (spi_ddi_led_ioctl_for_java)(u32 nCmd,u32 lplen ,u8* lParam, u32* wplen ,u8* wParam);

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
#define DEBUG_ANDROID 
//#undef  DEBUG_ANDROID

#ifdef DEBUG_ANDROID
#define android_debug printf
#else
#define android_debug(fmt,...) 
#endif

#define CRC_CCITT 0x1021    //CRC-CCITT多项式
#define START_FLAG 0x02//开始标志
#define END_FLAG 0x03//结束标志
#define XGDTEI_FLAG "XGDTEI"//命令标志
#define TEISEC_FLAG "TEISEC"//应答标志
#define START_BYTE 1//开始标志域长度
#define FLAG_BYTE 6//标志域长度
#define SERIAL_BYTE 1//流水号域长度
#define CMD_BYTE 2//命令域长度
#define LEN_BYTE 2//长度域长度
#define RESPONSE_BYTE 1//响应域长度
#define DATA_BYTE 4096//数据域最大长度，最小为0
#define END_BYTE 1//结束标志域长度
#define CRC_BYTE 2//校验域长度
#define CMD_MIN_BYTE (START_BYTE + FLAG_BYTE + SERIAL_BYTE + CMD_BYTE + LEN_BYTE + END_BYTE + CRC_BYTE)
#define CMD_MAX_BYTE (CMD_MIN_BYTE + DATA_BYTE)
#define ACK_MIN_BYTE (CMD_MIN_BYTE + RESPONSE_BYTE)
#define ACK_MAX_BYTE (ACK_MIN_BYTE + DATA_BYTE)
#define BUF_SIZE_BYTE 1024//缓冲区长度
#define CMD_TIMEOUT_MS 1000//命令超时时间，超时后清除缓冲区数据
#define ACK_TIMEOUT_MS 1000//应答超时时间，超时后清除缓冲区数据

//密钥管理类(0x60XX) 
//通讯方向:RICH CPU->SEC CPU
//SK--->SECRET_KEY
//#define SK_TYPE 0x6000//密钥类型
//#define SK_OPEN 0x6000 //打开密码键盘
//#define SK_CLOSE 0x6001 //关闭密码键盘
//#define SK_SELECT 0x6002 //选择密钥
//#define SK_ENCRYPT 0x6003 //加密数据
//#define SK_UPDATE 0x6004 //更新用户密钥
//#define SK_CAL_PIN_BLOCK 0x6005 //计算pin block
//#define SK_CAL_MAC 0x6006 //计算MAC
//#define SK_SET_PIN_LEN_MAX 0x6007 //设置pin最大长度
//#define SK_INJET 0x6008 //
//#define SK_DECRYPT 0x6009 //
//#define SK_IOCTL 0x600a//

#define SK_TYPE 0x6000//密钥类型
#define SK_OPEN 0x6000 //打开密码键盘
#define SK_CLOSE 0x6001 //关闭密码键盘
#define SK_INJET 0x6002 //安装或更新密钥
#define SK_ENCRYPT 0x6003 //指定密钥加密运算 
#define SK_DECRYPT 0x6004 //指定密钥解密运算
#define SK_IOCTL 0x6005 //控制内置密钥设备
//#define SK_GETKEY 0x6006 //控制内置密钥设备


#define KEY_TYPE 0x6100//按键类型
#define KEY_OPEN 0x6100 //打开按键
#define KEY_CLOSE 0x6101 //关闭按键
#define KEY_CLEAR 0x6102 //清除按键
#define KEY_READ 0x6103 //读取按键
#define KEY_IOCTRL 0x6104 //按键控制


#define PIN_TYPE 0x6500//密码键盘类型
#define PIN_INPUT 0x6500 //打开密码键盘
#define PIN_INPUT_PRESS 0x6501 //输入PIN 同步回显
#define PIN_INPUT_CANCEL 0x6502 //取消pin输入
#define PIN_PINBLOCK 0x6503 //获取联机加密PINBLOCK
#define PIN_OFFLINE_PIN 0x6504 //获取EMV IC卡脱机加密PIN数据
#define PIN_OFFLINE_PIN_APDU 0x6505  //处理EMV IC卡发送脱机明文PIN


//证书管理类(0x62XX)
//CRE--->CREDENTIALS
#define CRE_TYPE 0x6200//证书类型
#define CRE_OPEN 0x6200 //打开硬件安全模块
#define CRE_CLOSE 0x6201 //关闭硬件安全模块
#define CRE_SAVE 0x6202 //保存安全证书
#define CRE_DEL_ONE 0x6203 //删除指定证书
#define CRE_DEL_ALL 0x6204 //删除所有证书
#define CRE_QUERY_NUM 0x6205 //查询证书数量
#define CRE_READ_BY_NAME 0x6206 //通过name 读取证书
#define CRE_READ_BY_ID 0x6207 //通过ID 读取证书



#define DATA_MAX_LEN 1024

//LED类(0x63XX)
#define LED_TYPE 0x6300//LED类型
#define LED_OPEN 0x6300 //打开LED
#define LED_CLOSE 0x6301 //关闭LED
#define LED_STA_SET 0x6302 //设置LED
#define LED_IOCTRL 0x6303 //LED控制


#define SPI_COMMUNICATION_TEST 0x0000 


//管理操作类(0x6CXX)
//MGR--->MANAGER
#define MGR_TYPE 0x6C00//管理类型
#define MGR_READ_VER 0x6C00 //读取安全CPU硬件/boot/core版本号
#define MGR_GET_TAMPER_STATUS 0x6C01 //获取安全状态
#define MGR_UNLOCK_TAMPER_STATUS 0x6C02 //解除触发状态
#define MGR_SEND_TAMPER_STATUS 0x6C03 //安全CPU上报触发
#define MGR_GET_RTC_CHARGE 0x6C04 //获取RTC电池电量
#define MGR_GET_BUF_LEN 0x6C05 //获取安全CPU最大通讯buffer长度
#define MGR_FACTORY_INIT 0x6C06 //出厂初始化
#define MGR_LED_CTRL 0x6C07 //LED指示灯控制
#define MGR_PWR_OFF 0x6C08 //系统关机
#define MGR_START_K21 0x6C09
#define MGR_SET_TIMEOUT 0x6C1c //设置超时时间，超时后进入休眠
#define MGR_GET_TIMEOUT 0x6C1d //获取超时时间，超时后进入休眠
#define MGR_GET_STARTK21 0x6C09
#define MGR_GET_ID 0x6C1e //获取芯片ID
#define MGR_READ_DSN 0x6C0c
#define MGR_GET_RAND 0x6C0d
#define MGR_SET_PREVENTCUT 0x6C0e   //设置防切机信息
#define MGR_SET_BEEP 0x6c0f
#define MGR_GET_DEBUGSTATUS 0x6C10
#define MGR_SET_DEBUGSTATUS 0x6C11
#define MGR_SET_CERTHASH 0x6C12
#define MGR_GET_CERTHASH 0x6C13

#define MGR_COM_READ 0x6c14 //cl20151208 串口ddi
#define MGR_COM_WRITE 0x6c15 //cl20151208 串口 ddi
#define MGR_COM_CLEAR 0x6c16 //cl20151208 串口 ddi
#define MGR_READ_DSN 0x6C0c

#define MGR_SET_DSN 0x6C18

#define MGR_SET_BT_MAC 0x6C1F //设置蓝牙MAC
#define MGR_GET_BT_MAC 0x6C20 //获取蓝牙MAC 
#define MGR_GET_MACHINE_AND_HARDVER	0x6C21 //获取机型和硬件版本
#define MGR_FIG_GET_IMAGE  0x6c20 //获取指纹图像
//下载升级操作类(0x6EXX)
//DL--->DOWNLOAD
#define DL_TYPE 0x6E00//下载类型
#define DL_UPGRADE 0x6E00 //下载升级


//IC 卡操作类(0x64XX)
//IC--->INTEGRATED CIRCUIT
#define IC_TYPE 0x6400//IC卡类型
#define IC_OPEN 0x6400 //设备初始化,对IC卡驱动进行初始化
#define IC_CLOSE 0x6401 //设备回收
#define IC_POWERON 0x6402 //上电 ICC/PSAM 设备
#define IC_POWEROFF 0x6403 //下电 ICC/PSAM 设备
#define IC_GET_STATUS 0x6404 //获取 ICC/PSAM 设备状态
#define IC_EXCHANGE_APDU 0x6405 //读写 ICC/PSAM 设备
#define IC_IOCTL 0x6406 //控制 ICC/PSAM 设备

//非接IC 卡操作类(0x66XX)
//RF--->RADIO FREQUENCY
#define RF_TYPE 0x6600//非接IC卡类型
#define RF_OPEN 0x6600 //打开设备
#define RF_CLOSE 0x6601 //关闭设备
#define RF_POWERON 0x6602 //上电非接卡阅读器
#define RF_POWEROFF 0x6603 //下电非接卡阅读器
#define RF_GET_STATUS 0x6604 //获取非接卡状态
#define RF_ACTIVATE 0x6605 //激活非接卡片
#define RF_EXCHANGE_APDU 0x6606 //数据传输
#define RF_REMOVE 0x6607 //移除非接卡
#define RF_IOCTL 0x6608 //非接IC卡控制指令

/* IOCTL commands */
#define DDI_RF_CTL_VER                      0
#define DDI_RF_CTL_SAK                      1
#define DDI_RF_CTL_UID                      2
#define DDI_RF_CTL_MF_AUTH              3
#define DDI_RF_CTL_MF_READ_RAW          4
#define DDI_RF_CTL_MF_WRITE_RAW         5
#define DDI_RF_CTL_MF_READ_VALUE            6
#define DDI_RF_CTL_MF_WRITE_VALUE       7
#define DDI_RF_CTL_MF_INC_VALUE         8
#define DDI_RF_CTL_MF_DEC_VALUE         9
#define DDI_RF_CTL_MF_BACKUP_VALUE      (10)
#define DDI_RF_CTL_SET_PARAM                (11)
#define DDI_RF_CTL_GET_PARAM                (12)
#define DDI_RF_CTL_NFCBUILTIN               (13)
#define DDI_RF_CTL_RF_POWER_TEST			(14)

/* IOCTL commands */
#define DDI_INNERKEY_CTL_VER                      0
#define DDI_INNERKEY_CTL_TKEY_INJECT                     1
#define DDI_INNERKEY_CTL_TKEY_ENCRYPT                      2
#define DDI_INNERKEY_CTL_TKEY_DECRYPT            3
#define DDI_INNERKEY_CTL_HKEY_ENCRYPT       4
#define DDI_INNERKEY_CTL_HKEY_DECRYPT           5
#define DDI_INNERKEY_CTL_KEY_CHECK            6
#define DDI_INNERKEY_CTL_KEY_TAK_ENCRYPT_TAK       7

typedef struct
{
    s8 status;           
    u8 picctype;
    u8 AnticollGrade;    //防冲突等级
    u8 PiccSAK;          //
    u8 MLastSelectedsnrLen;
    u8 MLastSelectedSnr[20];//MLastSelectedSnr[0]保存长度
    u8 AtsLen;           //ATS长度
    u8 AtsData[20];      //ATS数据内容
    u8 TA;
    u8 TB;              //
    u8 TC;
    u8 FSCI;
                        //1etu = fc/128(106kbps)
    u8 FWI;           //FWI = (TB>>4);帧等待时间FWT =(256*16/fc) * (2**FWI) = 32 * (2**FWI) (etu) =32 * (2**(TB>>4)) (etu)
    u8 SFGI;          //帧保护时间SFGT =(256*16/fc) * (2**SFGI) , SFGI = (TB&0x0f);
    u8 atqflg;        
    u8 pupi[4];
    u8 app_dat[4];
    u8 prtcl_inf[4];
    u8 prtcl_inflen;       
    u8 TxIPCB;
    u8 RxIPCB; 
}nPICCData;

#define PICC_CIDCNTMAX          1

//磁条卡操作类(0x68XX)
//MAG--->MAGNETIC
#define MAG_TYPE 0x6800//磁条卡类型
#define MAG_OPEN 0x6800 //打开设备
#define MAG_CLOSE 0x6801 //关闭设备
#define MAG_CANCEL 0x6802 //取消刷卡
#define MAG_SEND 0x6803 //刷卡数据及状态//SEC CPU ->RICH CPU
#define MAG_CLEAR 0x6804 //清除
#define MAG_IOCTL 0x6805 //配置驱动
#define MAG_READ 0x6806 //read

#define DDI_MAG_CTL_VER 0

#define MAGIC_TRACK1_MAX_LEN    76+1
#define MAGIC_TRACK2_MIN_LEN    19
#define MAGIC_TRACK2_MAX_LEN    37+1
#define MAGIC_TRACK3_MAX_LEN    104+1


//打印机操作类(0x6AXX)
//PT--->PRINT
#define PT_TYPE 0x6A00//打印机类型
#define PT_OPEN 0x6A00
#define PT_CLOSE 0x6A01
#define PT_FEED_PAPER 0x6A02
#define PT_PRINT_IMAGE 0x6A03
#define PT_PRINT_IMAGE_FILE 0x6A04
#define PT_PRINT_TEXT 0x6A05
#define PT_PRINT_COMB_TEXT 0x6A06
#define PT_GET_STATUS 0x6A07
#define PT_IOCTL 0x6A08
#define PT_PRINT_DOT 0x6A09

typedef struct {
    u8 filenum;         //file number need to download
    u8 bagno;           //current download file bag NO.
    u8 bagnosum;        //current download file total bag number
    u8 machtyp;         //machine tyoe
    u8 hardver;         //hardware version
    u8 filename[51];    //file name must not overflow 50 bytes
    u32 filelength;     //current download file length
    u32 crc;            //file CRC
    u32 offset;         //offset of the current download bag in file
    
}FILEBAGINFO;


#define DEVICE_NAME "/dev/gpio_cdev"

#define GPIO_SET      0xAC  
#define GPIO_GET      0xAB
#define SCANNER_GPIO_ENABLE_NO 0    //  缂栧彿浠?寮€濮嬶紝鍏蜂綋缂栧彿鐢眘ys_config.fex涓璯pio_para鐨勯『搴忓喅瀹?
#define SCANNER_GPIO_POWER_ON_NO 1    //  缂栧彿浠?寮€濮嬶紝鍏蜂綋缂栧彿鐢眘ys_config.fex涓璯pio_para鐨勯『搴忓喅瀹?
#define SCANNER_GPIO_USB_SWITCH_NO 2    //  缂栧彿浠?寮€濮嬶紝鍏蜂綋缂栧彿鐢眘ys_config.fex涓璯pio_para鐨勯『搴忓喅瀹?
#define GPIO_HIGH      1
#define GPIO_LOW       0

typedef struct {
    int count;  //GPIO搴忓垪
    int data;   //GPIO鐢靛钩鐘舵€?
} gpio_userspace_t;


struct COMM_LOAD
{
	unsigned char  rev_buffer[64];
	//u8  rev_buffer[2048]; 
	unsigned char	rev_frame[2048];
	unsigned char	send_frame[1024];
	unsigned int rev_frame_len;
	unsigned int frame_len;
	unsigned int frame_head_num;
	unsigned int	frame_send_len;
	unsigned int	frame_send_bytes;
	unsigned int timeover;
	unsigned int count;
};

struct _COMM_FRAME
{	
	unsigned char flag;
	unsigned char command;
	unsigned int len;
	unsigned char	data[2048];
	unsigned char  checksum;
	unsigned char	ready;
};

struct _FILE_FLAG
{
    unsigned char file_flag[4];
    unsigned char pid_flag;
	unsigned char sn_flag;
    unsigned char testid_flag;
    unsigned char ini_flag;
	unsigned char baude_flag;
	unsigned char sn_checksum;
    unsigned char pid_checksum;
    unsigned char testid_checksum;
    unsigned char ini_file_checksum;
	unsigned char baude_checksum;
};

struct _SAVE_FILE
{
	struct _FILE_FLAG recv_flag;
	unsigned char pid_len;
	unsigned char pid_data[32];
	unsigned char sn_len;
	unsigned char sn_data[32];
	unsigned char testid_len;
	unsigned char testid_data[20];
	unsigned short ini_len;
	unsigned char ini_data[2048];
	unsigned char baude_len;
	unsigned long baude_data;
};

struct _SEND_DATA
{
    unsigned short send_len;
	unsigned char send_data[2048];
	unsigned char ready;
};

#define PID_LOAD 0x31
#define TESTER_LOAD 0x32
#define RTC_LOAD 0x33
#define INI_LOAD 0x34
#define DEL_LOG  0x36
#define SN_LOAD  0x37

#define BAUDERATE_LOAD 0x38
#define VIRTUAL_KEY_LOAD 0x39 
#define AUTOTEST_ITEM_SELECT 0x41

#define ASSEMBLY_RESULT_LOAD 0X01
#define HT_AGING_RESULT_LOAD 0X02
#define NT_AGING_RESULT_LOAD 0X03
#define WIRELESS_TEST_IMEI   0x04
#define MACHINE_TYPE_UPLOAD 0x06

#define FTAME_TIMEOVER		500


#define QUERY_FUSE_VERSION  	0xA0
#define QUERY_CPUID				0xA1
#define QUERY_FLASHID			0xA2
#define ENCRYPTED_INFO_LOAD	0xA4
#define MACID_LOAD				0xA3


#define BODY_NUMBER_LOAD 		0x9f
#define BODY_NUMBER_READ 		0xdc

//#define PTE_CFG  "/tmp/udisk/ptecfg.bin"
#define PTE_CFG  "/data/ptecfg.ini"

#endif
