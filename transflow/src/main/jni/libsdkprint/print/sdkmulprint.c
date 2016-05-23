#include "sdkGlobal.h"
//#include "stdarg.h"

#ifndef  DEBUG_PRINT
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

/***********************************###########################*****************************************/
/*�º���2.00�Ժ�ʼʵ��*/
/***********************************###########################*****************************************/
/*****************************************************************************
** Descriptions:	��֯���д�ӡ����
** Parameters:		s32 siRowGap:�м��
                                        const SDK_PRINT_DATA *pData:һ�ж��������ӡ�ṹ��
                                        ...:�ɴ����SDK_PRINT_DATA�ṹ�壬���������һ��NULL
** Returned value:	SDK_OK:��֯���ݳɹ�
                                        SDK_PARA_ERR:pDataΪNULL���ߴ�ӡ���ݳ��ȳ���129�ֽڷ���
                                        SDK_PRINT_CACHE_ERR:�ڴ����
** Created By:		shijianglong  2013.03.25
** Remarks:			���д�ӡ�ṹ�����ɴ���������֧���Զ����С�
                                        ��ʼ��ӡ�����sdkPrintStartWithMultFont���������Ҫһ�д�ӡ����������
                                        ʹ��sdkPrintStr��sdkPrintStart����ϣ���ӡ��ʼ��������sdkPrintInit��
*****************************************************************************/
s32 sdkPrintMultStr(s32 siRowGap, const SDK_PRINT_DATA *pData, ...) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	һ�ж�������Ĵ�ӡ��ʼ
** Parameters:		void
** Returned value:	SDK_OK:��ӡ�ɹ�
                                        SDK_PARA_ERR:�޴�ӡ����
                                        SDK_TIME_OUT:��ӡ��ʱ
                                        SDK_PRINT_OUTOF_PAPER:��ӡ��ȱֽ
                                        SDK_PRINT_DEV_FAIL:��ӡ���豸����
                                        SDK_PRINT_HANDLE_BACK:�ֻ�û�Żص���
** Created By:		shijianglong  2013.04.08
** Remarks:			��sdkPrintInit��sdkPrintMultStr����ʹ�á�
*****************************************************************************/
s32 sdkPrintStartWithMultFont(void) {
    return SDK_PRINT_NO_INIT;
}

