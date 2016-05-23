#ifndef _SDK_ES_H
#define _SDK_ES_H

#include "sdkGlobal.h"

#ifndef BYTE
#define BYTE unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

//���ο�ṹ��
typedef struct _RECT {
    u32 left;   //���ϵ�x
    u32 top;    //���ϵ�y
    u32 width;
    u32 height;
} SDK_RECT;

typedef enum {
    SDK_ES_CONTINUE,    //continue sign
    SDK_ES_SAVE,        //save sign, and exit
    SDK_ES_RESET,       //reset sign
    SDK_ES_ESC,         //cancel sign without save
} SDK_ES_CMD;

typedef SDK_ES_CMD (*SDK_ELECSIGN_IS_BREAK)(int x, int y);

typedef SDK_ES_CMD (*SDK_ELECSIGN_KEY_CALLBACK)(int key);


/* ����ǩ���ı�־λ */
#define SDK_ES_FLAG_DRAW_TS         0x0001      //����ǩ���Ƿ�����Ļ�л���������
#define SDK_ES_FLAG_CLEAR_BK        0x0002      //����ǩ���Ƿ������Ļ����


/* ����ǩ�������� */
typedef struct _sdk_com_es_data {
    u8 TransationSign[16];
    /* ���������� */
    int ComType;
    /* �������� 0-RS232��1-pinpad */
    int ComBaud;                    /* ���ڲ����ʣ�1Ϊ115200��0Ϊ38400 */
    u32 ShakeHandTimeOut;           /* �豸���ֳ�ʱʱ�䣬��λΪ����(ԭΪ3000ms) */
    u32 SignTimeOut;                /* ǩ����ʱʱ�䣬��λΪ���� */
} SDK_COM_ES_DATA;

/* ������ǩ�������� */
typedef struct _sdk_ts_es_data {
    u8 TransationSign[16];         /* ���������� */
    u8 *pasSaveFile;
    /* �洢����ǩ�����ļ��� */
    SDK_RECT ValidRect;             /* �������е���ǩ������Ч���� */
    u32 uiDestW;                    /* ���ɵĵ���ǩ���ļ��Ŀ�ȣ���СΪ237 */
    u32 uiDestH;                    /* ���ɵĵ���ǩ���ļ��ĸ߶ȣ���СΪ79 */
    u32 uiTimeOut;
    /* ǩ���ĳ�ʱʱ�䣬��λΪ����(�����涨���150s) */
    SDK_ELECSIGN_IS_BREAK pcb;      /* �жϴ������ڷǵ���ǩ������Ķ����Ļص����� */
} SDK_TS_ES_DATA;

/* �°津����ǩ�������ݣ���ȽϾ����ݽ���������˰����ص���*/
/* Ϊ�˼��ݣ�ԭ���Ľṹ�岻�� */
typedef struct _sdk_ts_es_data_ex {
    u8 TransationSign[16];         /* ���������� */
    u8 *pasSaveFile;
    /* �洢����ǩ�����ļ��� */
    SDK_RECT ValidRect;             /* �������е���ǩ������Ч���� */
    u32 uiDestW;                    /* ���ɵĵ���ǩ���ļ��Ŀ�ȣ���СΪ237 */
    u32 uiDestH;                    /* ���ɵĵ���ǩ���ļ��ĸ߶ȣ���СΪ79 */
    u32 uiTimeOut;
    /* ǩ���ĳ�ʱʱ�䣬��λΪ����(�����涨���150s) */
    SDK_ELECSIGN_IS_BREAK pcb;
    /* �жϴ������ڷǵ���ǩ������Ķ����Ļص����� */
    SDK_ELECSIGN_KEY_CALLBACK pkey_cb;
    /*�жϰ���ֵӦ����δ���Ļص�����*/
    void *pReserve;
    /* Ԥ������ʱ���� */
    void *pReserve2;                /* Ԥ������ʱ���� */
} SDK_TS_ES_DATA_EX;

/*****************************************************************************
** Descriptions:	�ж�������Ƿ���ָ���ľ��ο���
** Parameters:      SDK_RECT *lpRect    ָ���ľ�������
**                  int x               ������x����
**                  int y               ������y����
** Returned value:	������ھ��������ڣ�����true(1)�����򷵻�false(0)
** Created By:		tjb  2014.03.25
** Remarks:
*****************************************************************************/
extern bool sdkExtPtInRect(const SDK_RECT *lpRect, u32 x, u32 y);

/*****************************************************************************
** Descriptions:	ͨ����������ȡ����ǩ����ͼƬ
** Parameters:    	SDK_TS_ES_DATA *pstTsData   ���������ɵ���ǩ����������
** Returned value:	�ɹ�����    SDK_OK
                    �������󷵻�SDK_PARA_ERR
                    ��ʱ����    SDK_TIME_OUT
                    ʧ�ܷ���    SDK_ERR
** Created By:		tjb  2014.04.14
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsGetSignByTs(SDK_TS_ES_DATA *pstTsData);

/*****************************************************************************
** Descriptions:	ͨ����������ȡ����ǩ����ͼƬ
** Parameters:    	SDK_TS_ES_DATA *pstTsData   ���������ɵ���ǩ����������
** Returned value:	�ɹ�����    SDK_OK
                    �������󷵻�SDK_PARA_ERR
                    ��ʱ����    SDK_TIME_OUT
                    ʧ�ܷ���    SDK_ERR
** Created By:		tjb  2014.04.14
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsGetSignByTsEx(SDK_TS_ES_DATA_EX *pstTsData);

/*****************************************************************************
** Descriptions:	ͨ������������̵�ǩ�ְ��ȡǩ��
** Parameters:    	const SDK_COM_ES_DATA *pstEsData    ����ǩ����Ҫ������
                    char *pBmpFile                      �洢����ǩ�����ļ�������·��
** Returned value:	�ɹ�����SDK_OK����ʱ����SDK_TIME_OUT�������쳣����SDK_PARA_ERR
** Created By:		tjb  2014.03.28
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsGetSignByCom(const SDK_COM_ES_DATA *pstEsData, char *pBmpFile);

/*****************************************************************************
** Descriptions:	��bmpͼת���ɿ���ȫ����ӡ��ͼƬ,
** Parameters:      u8 *BmpFile         BMP�ļ��ĵ���ǩ��
                    u8 *PrintBmp        ��ӡʹ�õĵ���ǩ���ļ�
** Returned value:  �ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR, �������󷵻�SDK_PARA_ERR
** Created By:		tjb  2014.03.28
** Remarks:         �ײ��趨Ϊ�߶�����Ϊ24��������,��ת����384����
*****************************************************************************/
extern int sdkExtEsTransBmpToPrint(const u8 *pucBmpFile, const u8 *pucPrintBmp);

/*****************************************************************************
** Descriptions:	���ŵ���ǩ����ͼƬ
** Parameters:    	const u8 *pucSrcFile        ԴͼƬ�ļ�
                    const u8 *pucDestFile       ���ź���ļ�
                    u32 uiDestW                 ���ź���ļ����
                    u32 uiDestH                 ���ź���ļ��߶�
** Returned value:	�ɹ�����SDK_OK��ʧ�ܷ���SDK_ERR, �������󷵻�SDK_PARA_ERR
** Created By:		tjb  2014.04.11
** Remarks: 		�ǵȱ����ŵ�Ч��һ��
*****************************************************************************/
extern int sdkExtEsStretch(const u8 *pucSrcFile, const u8 *pucDestFile, u32 uiDestW, u32 uiDestH);

/*****************************************************************************
** Descriptions:	��BMP�ļ�ת��ΪJBIG��ʽ�ļ�
** Parameters:    	u8 *pucBmpFile      ԭBMP�ļ���
                    u8 *pucJbigFile     Ŀ��JBIG�ļ���
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.04.10
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsBmpToJbig(const u8 *pucBmpFile, const u8 *pucJbigFile);

/*****************************************************************************
** Descriptions:	��BMP�ļ�ת��ΪJBIG85��ʽ�ļ�
** Parameters:    	u8 *pucBmpFile      ԭBMP�ļ���
                    u8 *pucJbigFile     Ŀ��JBIG85�ļ���
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.04.14
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsBmpToJbig85(const u8 *pucBmpFile, const u8 *pucJbigFile);

/*****************************************************************************
** Descriptions:	��JBIG�ļ�ת��ΪBMP��ʽ�ļ�
** Parameters:    	const u8 *pucBmpFile      ԭBMP�ļ���
                    const u8 *pucJbigFile     Ŀ��JBIG�ļ���
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.04.14
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsJbigToBmp(const u8 *pucJbigFile, const u8 *pucBmpFile);

/*****************************************************************************
** Descriptions:	��JBIG85�ļ�ת��ΪBMP��ʽ�ļ�
** Parameters:    	const u8 *pucBmpFile      ԭBMP�ļ���
                    const u8 *pucJbigFile     Ŀ��JBIG85�ļ���
** Returned value:	�ɹ�����SDK_OK�������쳣����SDK_PARA_ERR, ʧ�ܷ���SDK_ERR
** Created By:		tjb  2014.04.14
** Remarks: 		
*****************************************************************************/
extern int sdkExtEsJbig85ToBmp(const u8 *pucJbigFile, const u8 *pucBmpFile);

/*****************************************************************************
** Descriptions:	��ʼ������ǩ��ʹ�õ��Ĳ���
** Parameters:    	u16 sw          ��Ļ�Ŀ��
                    u16 sh          ��Ļ�ĸ߶�
                    u16 scalex      ��������Űٷ�ֵ����Ŵ�1.2��ʱ����120��û������ʱ��Ϊ0��
                    u16 scaley      (����K370P\K320P�ǲ�ɫ�Ĵ������������ֵ���зŴ�)
                    bool bcolor     �Ƿ�Ϊ��ɫ��Ļ
                    u32 esflags     ����ǩ���ı�־
                    u16 font_size   ��������ʾ�������С
                    u16 font_clr    ��������ʾ��������ɫ���ڰ�����������ɫֻ��ʹ��Ĭ�ϵ���ɫ
** Returned value:	
** Created By:		tjb  2014.12.11
** Remarks: 		Ϊ�˱��ֺ��������ʹ�ã���Щ������Ӧ�ô���
*****************************************************************************/
extern int sdkExtEsInit(u16 sw, u16 sh, u16 scalex, u16 scaley, bool bcolor, u32 esflags,
                        u16 font_size, u16 font_clr);

#endif
