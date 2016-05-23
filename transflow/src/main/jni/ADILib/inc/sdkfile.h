#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */

#ifndef SDKFILE_H
#define SDKFILE_H


extern s32 sdkWriteFile(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen);

extern s32 sdkReadFile(const u8 *pasFile, u8 *pheDest, s32 siStart, s32 *psiDestlen);

extern s32 sdkAppendFile(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen);

extern s32 sdkInsertFile(const u8 *pasFile, const u8 *pheSrc, s32 siStart, s32 siSrclen);

extern s32 sdkSaveOverlap(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen);

extern s32 sdkReadOverlap(const u8 *pasFile, u8 *pheDest, s32 siDestlen);

extern s32 sdkGetFileSize(const u8 *pasFile);

extern s32 sdkDelFile(const u8 *pasFile);

extern bool sdkAccessFile(const u8 *pasFile);

/*****************************************************************************
** Descriptions:	�������ļ�
** Parameters:    	const u8 * pasFile:ԭ�ļ�����·��
                    const u8 * pasFile:���ļ�����·��
** Returned value:	
                    ����ָ��Ϊ�ջ���·������(����256),ԭ���ļ�Ŀ¼��һ�� �򷵻�SDK_PARA_ERR
                    �ɹ��򷵻�SDK_OK
                    �����򷵻�SDK_ERR��
** Created By:		���ƽ  2014.11.26
** Remarks: 		ԭ�ļ������ļ�Ŀ¼����һ��,��:sdkRenameFile( "/mtd0/ttd/testRename.cfg", 
                    "/mtd0/ttd/newFileName.cfg" );��ȷ
                    sdkRenameFile( "/mtd0/ttd/testRename.cfg","/mtd0/ddi/newFileName.cfg" );����

*****************************************************************************/
extern s32 sdkRenameFile(const u8 *pasOldFile, const u8 *pasNewFile);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


