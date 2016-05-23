#include "sdkGlobal.h"

#ifndef  DEBUG_PRINT
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

/*************************************************************************************/
/************************************��֧�ִ�ӡ��*************************************/
/*************************************************************************************/

/*****************************************************************************
** Descriptions:	��ӡ����ʼ��
** Parameters:		void
** Returned value:	SDK_TIME_OUT			��ȡ״̬��ʱ
                                        SDK_PRINT_LOSE_COMMAND	δ��ô�ӡ���
                                        SDK_PRINT_OUTOF_PAPER	ȱֽ
                                        SDK_PRINT_DEV_FAIL		��ӡ������
                                        SDK_OK					��ӡ����ʼ���ɹ�
** Created By:		����Ȫ  2012.08.20
** Remarks:			��ʼ��������ӡ��״̬(���Ƿ�ȱֽ)�����ô�ӡ����
                                        ��������ӡǰʹ��
*****************************************************************************/
s32 sdkPrintInit(void) {
    return SDK_PRINT_NO_INIT;
}

/*******************************************************************
   ��    Ȩ:	�¹���
   ��    ��:	Joon
   ��������:	sdkPrintFeedPaper
   ��������:	����ֽ����ֽ
   �������:	RollDirection :��ֽ����ֽ;
                        ucDot���˵ĵ���
   �������:
   �� �� ֵ:	SDK_PRINT_NO_INIT	δ��ʼ����ӡ��
                        SDK_PARA_ERR		��������
                        SDK_TIME_OUT		��ʱ
                        SDK_OK				�ɹ�
   �޸ı�ע:
   ����ʱ��:	2012.05.16 11:31:24
*******************************************************************/
s32 sdkPrintFeedPaper(SDK_PRINT_ROLL_DIRECTION eRollDirection, u8 ucDot) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	�ַ�����ӡ
** Parameters:		pAsStr:��Ҫ��ӡ�ַ�����Ϣ
                                        uiFont:�����С
                                        eAlign:���뷽ʽ
                                        siOffset:��ӡ��ƫ�������Ҷ���ʱ��Ч
                                        siRowGap:�м��
** Returned value:	SDK_PARA_ERR			�������Ϸ�
                                        SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_PRINT_CACHE_ERR		�滺��ʧ��
                                        SDK_OK					�ɹ�
** Created By:		����Ȫ  2012.08.20
** Remarks:			���ڵ��ô˺�������ֱ�Ӵ�ӡ�ģ��μ�Print_Start
*****************************************************************************/
s32 sdkPrintStr(u8 const *pAsStr, SDK_PRINT_FONT uiFont, SDK_PRINT_ALIGNMODE eAlign, s32 siOffset,
                s32 siRowGap) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	��ӡͼƬ
** Parameters:		pAsBmpFile:��Ҫ��ӡͼƬ������·��
                                        eAlign:���뷽ʽ
                                        siOffset:��ƫ��
** Returned value:	SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_PARA_ERR			��������
                                        SDK_PRINT_CACHE_ERR		�滺��ʧ��
                                        SDK_OK					����ɹ�
** Created By:		����Ȫ  2012.08.20
** Remarks:			�ļ�������ʱ����ʾ������
*****************************************************************************/
s32 sdkPrintBitMap(const u8 *pAsBmpFile, SDK_PRINT_ALIGNMODE eAlign, s32 siOffset) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	��ӡ��ʼ
** Parameters:		void
** Returned value:	SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_OK					��ӡ�ɹ�
                                        SDK_PARA_ERR			û�д�ӡ����
                                        SDK_TIME_OUT			��ӡ��ʱ
                                        SDK_PRINT_OUTOF_PAPER	��ӡȱֽ
                                        SDK_PRINT_DEV_FAIL		��ӡ���豸����
                                        SDK_PRINT_HANDLE_BACK	�ֱ�δ�Żص���
** Created By:		����Ȫ  2012.08.20
** Remarks:			����Print_Str/Print_BitMap����ֱ�Ӵ�ӡ�ģ���ʹ����������
                                        ���������ݴ��뻺�棬�����ô˺�����ӡ��
*****************************************************************************/
s32 sdkPrintStart(void) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	�ȴ��ӡ��ɺ��Զ���ֽ
** Parameters:		void
** Returned value:	SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_OK					��ӡ�ɹ�
                                        SDK_PARA_ERR			û�д�ӡ����
                                        SDK_TIME_OUT			��ӡ��ʱ
                                        SDK_PRINT_OUTOF_PAPER	��ӡȱֽ
                                        SDK_PRINT_DEV_FAIL		��ӡ���豸����
                                        SDK_PRINT_HANDLE_BACK	�ֱ�δ�Żص���
** Created By:		fusuipu  2013.11.14
** Remarks:
*****************************************************************************/
s32 sdkPrintStartNoRollPaper(void) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	��Ҫ�����ʱ��Ҫ�Զ��ع�
** Parameters:		void
** Returned value:	SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_OK					��ӡ�ɹ�
                                        SDK_PARA_ERR			û�д�ӡ����
                                        SDK_TIME_OUT			��ӡ��ʱ
                                        SDK_PRINT_OUTOF_PAPER	��ӡȱֽ
                                        SDK_PRINT_DEV_FAIL		��ӡ���豸����
                                        SDK_PRINT_HANDLE_BACK	�ֱ�δ�Żص���
** Created By:		shiweisong  2013.10.28
** Remarks:
*****************************************************************************/
s32 sdkPrintStartNoRollBack(void) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	����ҳ���ӡ��������һ�е�ʵ���м��
** Parameters:		s32 siDoit
** Returned value:	���ش�ӡ�������һ�е�ʵ���м��
                                        SDK_PARA_ERR			��ӡ����Ϊ��
                                        SDK_PRINT_NO_INIT		��ӡ��δ��ʼ��
                                        SDK_OK					�����ӡ��
** Created By:		huacong  2013.03.01
** Remarks:			�����ڵ���sdkPrintStr��sdkPrintStart֮ǰʹ�ã���ֹȫ�ֻ���g_pstPrintSheet���ͷ�
*****************************************************************************/
s32 sdkPrintSetLastRowGap(s32 siDoit) {
    return SDK_PRINT_NO_INIT;
}

/*****************************************************************************
** Descriptions:	    ��ӡ�Ҷ�����
** Parameters:    	u32 grayValue:�Ҷ�ֵ
** Returned value:	
                        �ɹ��������õĻҶ�ֵ
                        �������󷵻�SDK_PARA_ERR
** Created By:		wangmingming  2014.07.03
** Modified By:		
** Remarks: 		    ��ӡ�Ҷ�ȡֵ��Χ0~2,�ýӿ���ʱ���ṩ��Ӧ��
*****************************************************************************/
s32 Private_sdkPrintGraySet(u32 setValue) {
    return SDK_PRINT_NO_INIT;
}

