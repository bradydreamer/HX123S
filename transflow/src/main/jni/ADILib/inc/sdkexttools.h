#ifndef _SDK_EXT_TOOLS_H_
#define _SDK_EXT_TOOLS_H_

#include "sdkGlobal.h"
#include "sdkwifi.h"

typedef enum {  //һά��ĸ�ʽ
    SDK_BARCODE_ANY = 0,    //choose best-fit
    SDK_BARCODE_EAN,        //ŷ����Ʒ����(European Article Number):13/12λ����(EAN-13), 8/7λ����(EAN-8)
    SDK_BARCODE_UPC,        //upc == 12-digit ean
    SDK_BARCODE_ISBN,       //isbn numbers (still EAN13)
    SDK_BARCODE_39,         //code 39 
    SDK_BARCODE_128,        //code 128 (a,b,c: autoselection)
    SDK_BARCODE_128C,       //code 128 (compact form for digits)
    SDK_BARCODE_128B,       //code 128, full printable ascii 
    SDK_BARCODE_I25,        //interleaved 2 of 5 (only digits) 
    SDK_BARCODE_128RAW,     //Raw code 128 (by Leonid A. Broukhis) 
    SDK_BARCODE_CBR,        //Codabar (by Leonid A. Broukhis) 
    SDK_BARCODE_MSI,        //MSI (by Leonid A. Broukhis) 
    SDK_BARCODE_PLS,        //Plessey (by Leonid A. Broukhis)
    SDK_BARCODE_93          //code 93 (by Nathan D. Holmes)
} SDK_BARCODE_FORMAT;

/*****************************************************************************
** Descriptions:	��ȡ������IP��ַ
** Parameters:    	const char *pucName    -������
                    u32 uiTimeOut          -��ʱʱ�䣬��λ��(s)
                    char pcIpList[][16]    -����IP��ַ������
                    u32 uiListNum          -����������
** Returned value:	�ɹ����ػ�ȡ��IP��ַ�ĸ���, �������󷵻�SDK_PARA_ERR, ���󷵻�SDK_ERR
** Created By:		tjb  2014.03.06
** Remarks: 		
*****************************************************************************/
extern int sdkExtGetHostByName(const char *pucName, u32 uiTimeOut, char pcIpList[][16],
                               u32 uiListNum);

/*****************************************************************************
** Descriptions:	����һά�������ͼƬ
** Parameters:    	const char *pcbarcode       ����������
                    SDK_BARCODE_FORMAT format   ������ĸ�ʽ
                    const char *pcBmpFile       ����Ŀ��BMPͼƬ�ļ���(����·��)
** Returned value:	�ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR���������󷵻�SDK_PARA_ERR
** Created By:		tjb  2014.03.21
** Remarks: 		Ŀǰ��֧��1λλͼ���bmpͼƬ�������ڶ�̬���ļ�libxgdbarcode.so��
                    �����ص�pos��/mtd0/dllĿ¼��
*****************************************************************************/
extern int sdkExtBarcodeCreateBmp(const char *pcbarcode, SDK_BARCODE_FORMAT format,
                                  const char *pcBmpFile);

/*****************************************************************************
** Descriptions:	���ɶ�ά���ͼƬ
** Parameters:    	const char *pcbarcode       ��ά�������
                    const char *pcBmpFile       ����Ŀ��BMPͼƬ�ļ���(����·��)
** Returned value:	�ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR���������󷵻�SDK_PARA_ERR
** Created By:		tjb  2014.03.21
** Remarks: 		Ŀǰ��֧��1λλͼ���bmpͼƬ���ú���������ִ���ļ���qrencode��png2bmp��
                    �Լ���̬���ļ�libxgdbarcode.so�������ص�pos��/mtd0/dllĿ¼��
*****************************************************************************/
extern int sdkExtQRcodeCreateBmp(const char *pcbarcode, const char *pcBmpFile);

/*****************************************************************************
** Descriptions:	����ָ�����������ɶ�ά���ͼƬ
** Parameters:    	const char *pcbarcode   ��ά������ݣ������ݵĳ���2953���ֽ�,
                    const char *pcBmpFile   ����Ŀ��BMPͼƬ�ļ���(����·��)
                    u8 size                 ��ά��ͼƬ��С
                    u8 marge                Ĭ��Ϊ4 �ߴ���С1-30
                    u8 version              qr��ά��汾 1-40, ͨ������Ϊ1
                    u8 offset               ƫ�ƣ���ʱû�õ�
** Returned value:	
** Created By:		tjb  2014.12.22
** Remarks: 		�����Զ����ɵĶ�ά��ͼƬ������128X64��С����ʾ�����ͨ��
                    �ú�������ָ�����������ɶ�ά��
*****************************************************************************/
extern int sdkExtQRcodeCreateBmpEx(const char *pcbarcode, const char *pcBmpFile, u8 size, u8 marge,
                                   u8 version, u16 offset);

/*****************************************************************************
** Descriptions:	��ʼ����ά��ɨ���豸
** Parameters:      Input: None
** Returned value:  SDK_OK: ��ʼ���ɹ���SDK_ERR: ʧ��
** Created By:		tjb  2014.03.21
** Remarks:         K ƽ̨����û�����Ӳ������֧�ֶ�ά��ɨ��
*****************************************************************************/
extern int sdkExtQRcodeSetInit();

/*****************************************************************************
** Descriptions:	ж�ض�ά��ɨ���豸
** Parameters:    	Input: None 
** Returned value:	SDK_OK: ��ʼ���ɹ���SDK_ERR: ʧ��
** Created By:		tjb  2014.03.21
** Remarks: 		K ƽ̨����û�����Ӳ������֧�ֶ�ά��ɨ��
*****************************************************************************/
extern int sdkExtQRcodeSetUninstall();

/*****************************************************************************
** Descriptions:	ɨ���ά�룬������Ӧ������
** Parameters:    	char *pasData       �洢���ݵĻ�����
                    u32 uLen            �������Ĵ�С, ���ֵΪ1024
** Returned value:	SDK_ERR-ʧ�ܣ�SDK_PARA_ERR�������󣻳ɹ�ʱ����ȡ�õ����ݴ�С
** Created By:		tjb  2014.03.21
** Remarks: 		K ƽ̨����û�����Ӳ������֧�ֶ�ά��ɨ��
*****************************************************************************/
extern int sdkExtQRcodeSetScan(char *pasData, u32 uLen);

/*****************************************************************************
** Descriptions:	�л���WIFI�����ý���
** Parameters:    	SDK_EXT_WIFI_INFO *pstInfo  pstInfo����WIFI�Ĳ�����
                                                   Ϊ��ʱ���ý���������
** Returned value:	
** Created By:		tjb  2014.05.09
** Remarks: 		���Ǿɵ�wifiapp.so ʵ��ģʽ�������Ӧ���л���������Ļ����
                    �ı�
*****************************************************************************/
extern int sdkExtWifiSwitchtoSetting(SDK_EXT_WIFI_INFO *pstInfo);

/*****************************************************************************
** Descriptions:	�ж�panel֧������wifiʵ�ַ�ʽ
** Parameters:    	Input: None 
** Returned value:	true    ֧���µ� libsdkwifi ��ʽ
                    false   ֧�־ɵ� wifiapp ��ʽ
** Created By:		tjb  2014.12.30
** Remarks: 		
*****************************************************************************/
extern bool sdkExtIsPanelSupportWifiApi();

/*****************************************************************************
** Descriptions:	������
** Parameters:    	SDK_COMM_STLANPARAM *pstComm   �����������
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.05.09
** Remarks: 		SDK_COMM_STCOMMPARAM �ṹ�����
*****************************************************************************/
extern int sdkExtOpenEthernet(SDK_COMM_STLANPARAM *pstComm);

/*****************************************************************************
** Descriptions:	����������������ַ
** Parameters:    	const u8 *pasDnsPri     ��ѡDNS������IP��ַ
                    const u8 *pasDnsSec     ��ѡDNS������IP��ַ������Ϊ��
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.05.21
** Remarks: 		Ŀǰֻ֧��Linuxϵͳ, ������WIFIģʽ
*****************************************************************************/
extern int sdkExtSetDNS(const u8 *pasDnsPri, const u8 *pasDnsSec);


/*****************************************************************************
** Descriptions:    ��ʼ����Բ���߲���
** Parameters:    	const u8 *pGroupP:Gp����ֵ
                    const u8 *pGroupA:Ga����ֵ
                    const u8 *pGroupB:Gb����ֵ
                    const u8 *pGroupX:Gx����ֵ
                    const u8 *pGroupY:Gy����ֵ
                    const u8 *pGroupN:Gn����ֵ
** Returned value:	SDK_PARA_ERR:�������������ΪNULL
                    SDK_OK :�ɹ�
                    SDK_ERR:ʧ��
** Created By:		shijianglong  2013.05.28
** Remarks: 1.ÿ�����ֵ������ASC�룬�����н�����		
*****************************************************************************/
extern int sdkSM2InitElliptic(const u8 *pGroupP, const u8 *pGroupA, const u8 *pGroupB,
                              const u8 *pGroupX, const u8 *pGroupY, const u8 *pGroupN);

/*****************************************************************************
** Descriptions:	SM3����
** Parameters:    	u8 *pSrcData:����������
                    s32 siLen:���ݳ���
                    u8 *pDestData: ���ܺ�����
** Returned value:	SDK_PARA_ERR:�������������ΪNULL������С��0���ء�
                    32 :�ɹ��������ݳ���32
                    SDK_ERR:ʧ��
** Created By:		shijianglong  2013.05.29
** Remarks: 		�����ǩ����������Ϊ�գ����pIDA�����Ѿ�SM3���ܹ���Ĵ���֤���ݲ�����ֱ����֤������ͨ��pIDA��pSrcData����SM3���к�����֤
*****************************************************************************/
extern int sdkSM3Encrypt(u8 *pSrcData, int siLen, u8 *pDestData);//shijianglong 2013.05.30 16:15

/*****************************************************************************
** Descriptions:	У��SM2ǩ���Ϸ���
** Parameters:    	const u8 *pPkeyX:��ԿX����ֵ
                    const u8 *pPkeyY:��ԿY����ֵ
                    const u8 *pSignR:ǩ��Rֵ
                    const u8 *pSignS:ǩ��Sֵ
                    const u8 *pSrcData:��ǩ������
                    const u8 *pIDA:�û����
** Returned value:	SDK_PARA_ERR:�������������ΪNULL(��ǩ���������ݿ���Ϊ��)
                    SDK_OK :�ɹ�
                    SDK_ERR:ʧ��
** Created By:		shijianglong  2013.05.28
** Remarks: 1.�����ǩ����������Ϊ�գ����pIDA�����Ѿ�SM3���ܹ���Ĵ���֤���ݲ�ֱ����֤������ͨ��pIDA��pSrcData����SM3���������֤
2.ÿ�����ֵ������ASC�룬�����н�����	
*****************************************************************************/
extern int sdkSM2CheckSign(const u8 *pPkeyX, const u8 *pPkeyY, const u8 *pSignR, const u8 *pSignS,
                           const u8 *pSrcData, const u8 *pIDA);

/*****************************************************************************
** Descriptions:	�ͷ�SM2�ռ�
** Parameters:    	Input: None 
** Returned value:	SDK_OK :�ɹ�
                    SDK_ERR:ʧ��
** Created By:		shijianglong  2013.05.28
** Remarks: 	��ǩ�������ͷſռ䣬�����ڴ�й©
*****************************************************************************/
extern int sdkSM2ClearAll(void);

#endif
