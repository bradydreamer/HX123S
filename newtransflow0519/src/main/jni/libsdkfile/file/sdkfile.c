//#include "libdevheads.h"  //zhouzhihua 2013.07.03 11:34
#include "sdkdevOption.h"
#include "sdkglobal.h"

#ifdef __GNUC__
#include <unistd.h>
#endif

#ifndef  DEBUG_FILE
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#undef F_OK
#define F_OK 0

#define FAST_FILE_FLAG  1
#undef  FAST_FILE_FLAG

#define SDK_MAX_PATH                40

/*****************************************************************************
** Descriptions:	����д�ļ�,����ļ�������,�Ȱ��ļ�Ŀ¼��νӿڴ����ļ�
** Parameters:    	const u8 *lpFile:��·���ļ�����
                    const u8 *lpIn:��д������
                    u32 nLen:���ȣ���д��ĸ���
** Returned value:	
                    FILE_OK:    �����ļ��ɹ�
                    FILE_ERROR: �����ļ�ʧ��
** Created By:		������
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
                    ���Դ�ļ����ڣ�����������е����ݣ���д�����ݣ�
                    ���Դ�ļ������ڣ��Ȱ���pFileָ����·�������ļ���
                        ��д������.����ļ��򿪡�������д����ִ���
                        ����Exception����������ERRORNO_FILE��Ϊ�������
                        ��ʾ������Ϣ����Ҫ��ػ�����.
*****************************************************************************/
static s32 sdkXgdWriteFile(const u8 *lpFile, const u8 *lpIn, u32 nLen) {
    s32 fp;

    if (NULL == lpFile || NULL == lpIn) {
        Assert(0);
        return SDK_FILE_ERROR;
    }

    //�����㷽ʽ�򿪿ɶ�д�ļ�                                      
    fp = ddi_vfs_open(lpFile, "w+");
    if (fp == NULL) {
        Assert(0);
        return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
    }

    if (ddi_vfs_write(lpIn, nLen, fp) != nLen) {
        ddi_vfs_close(fp);//�ر��ļ�
        Assert(0);
        return SDK_FILE_ERROR;//д���ɹ�
    }
    else {
        ddi_vfs_close(fp);//�ر��ļ�
        return SDK_FILE_OK;
    }
    //�����ļ��ɹ�
}

/*****************************************************************************
** Descriptions:	���ļ�����λ�ö�ȡ���ⳤ�ȵ�����,����ļ�������,�Ȱ��ļ�Ŀ¼
                    ��νӿڴ����ļ�
** Parameters:    	const u8 *lpFile:��·���ļ�����
                    u8 *lpOut:��������
                    u32 nStart:��λ��
                    u32 *lpLen:����������
** Returned value:	
                    FILE_OK:    �����ļ��ɹ�
                    FILE_EOF:   �����ļ�β
                    FILE_ERROR: �����ļ�ʧ��
** Created By:		������
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
                    ���pFile�����ڣ��򴴽�֮�����ҵ���Exception��������
                    ����ļ��򿪻򴴽�ʧ�ܣ���������SDK_FILE_ERROR��
                    �����ȡ���ݶ����ļ�β����������SDK_FILE_EOF
*****************************************************************************/
static s32 sdkXgdReadFile(const u8 *lpFile, u8 *lpOut, u32 nStart, u32 *lpLen) {
    s32 fp;
    u32 i;

    //��ֻ���ķ�ʽ���Ѿ����ڵ��ļ�
/* BEGIN: Modified by wangmingming, 2014/5/19   PN:1 */
    //fp = sdkXgdOpenFile(lpFile, "rb");
    fp = ddi_vfs_open(lpFile, "rb+");
/* END:   Modified by wangmingming, 2014/5/19   PN:1 */
    if (NULL == fp) {
        *lpLen = 0;
        Assert(0);
        return SDK_FILE_ERROR;//�ļ��������ʧ��
    }

    //���ö�дλ��ΪnStart
    ddi_vfs_seek(fp, nStart, SEEK_SET);

    i = *lpLen;

    //������
    *lpLen = ddi_vfs_read(lpOut, i, fp);
    //Trace("test", "fread from %s read %d BYTE data:%s\r\n", lpFile, *lpLen, lpOut);
    if (*lpLen != i) {//�����ĸ����������Ĳ�һ��
        Assert(0);
        ddi_vfs_close(fp);
        return SDK_FILE_EOF;//������Ī��ʧ��,��Ϊ�����ļ�β
    }

    ddi_vfs_close(fp);
    return SDK_FILE_OK;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ļ�β��׷������
   ��ڲ���: 1:lpFile: ��·���ļ�����,2:lpIn: ��׷������,3: nLen: ����
   �� �� ֵ:
                        FILE_OK:	�����ļ��ɹ�
                        FILE_ERROR: �����ļ�ʧ��
   ��    ע:
            ��pasFile��ָ���ļ�������ļ������ڣ��򴴽��ö������ļ�����
                ���ļ���β��׷��pheSrcָ�������.
            ����ļ��������򿪡�д��������һ���ִ���������SDK_FILE_ERROR��
                �����ú���Exception(ERRORNO_FILE)��ʾ������ʾ��Ϣ��Ҫ��ػ����ԣ�
            ������ݳɹ�׷�ӣ���������SDK_FILE_OK
 ********************************************************************/
static s32 sdkXgdAppendFile(const u8 *lpFile, const u8 *lpIn, u32 nLen) {
    s32 fp;

    fp = ddi_vfs_open(lpFile, "wb+");
    if (fp == NULL) {
        Assert(0);
        return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
    }

    //��λ���ļ�β
    ddi_vfs_seek(fp, 0, SEEK_END);

    if (ddi_vfs_write(lpIn, nLen, fp) != nLen) {
        ddi_vfs_close(fp);
        Assert(0);
        return SDK_FILE_ERROR;//д���ɹ�
    }
    else {
        ddi_vfs_close(fp);//�ر��ļ�
        return SDK_FILE_OK;
    }

    //�����ļ��ɹ�
}

/*****************************************************************************
** Descriptions:	���ļ�����λ�ò������ⳤ�ȵ�����
** Parameters:    	const u8 *lpFile:��·���ļ�����
                    const u8 *lpIn:��׷������
                    u32 nStart:����λ��
                    u32 nLen:����
** Returned value:	
                    FILE_OK:    �����ļ��ɹ�
                    FILE_ERROR: �����ļ�ʧ��
                    FILE_SEEK_ERROR: �����Ƿ���λ
** Created By:		������
** Modified By:		wangmingming  2014.05.19
** Remarks: 	
                    ���pasFile�����ڣ��򴴽�֮;
                    ����ļ��򿪻򴴽�ʧ�ܣ���������SDK_FILE_ERROR;
                    ���uiStartֵ�����ļ����Ѵ����ݵĳ��ȣ���������SDK_PARA_ERR.
                    �������������쳣����������Exception��������
*****************************************************************************/
static s32 sdkXgdInsertFile(const u8 *lpFile, const u8 *lpIn, u32 nStart, u32 nLen) {
    s32 fp;

/* BEGIN: Modified by wangmingming, 2014/5/19   PN:1 */
    //�Ѷ�/д�ķ�ʽ��(����)�ļ�
    //fp = sdkXgdOpenFile(lpFile, "rb+");
    fp = ddi_vfs_open(lpFile, "rb+");//zhouxiaoxin 不能用wb+
/* END:   Modified by wangmingming, 2014/5/19   PN:1 */
    if (fp == NULL) {
        Assert(0);
        return SDK_FILE_ERROR;//�ļ��������ʧ��
    }
    ddi_vfs_seek(fp, 0, SEEK_END);
    if ((s32) nStart > ddi_vfs_tell(fp)) {//��д���λ�ó����ļ�β
        Assert(0);
        ddi_vfs_close(fp);//�ر��ļ�
        return SDK_FILE_ERROR;
    }

    //���ö�дλ�õ�nStart
    ddi_vfs_seek(fp, nStart, SEEK_SET);

    if (ddi_vfs_write(lpIn, nLen, fp) != nLen) {//ʵ��д��ĸ�������д��ĸ�������
        Assert(0);
        ddi_vfs_close(fp);//�ر��ļ�
        return SDK_FILE_ERROR;//д���ɹ�
    }
    else {
        ddi_vfs_close(fp);//�ر��ļ�
        return SDK_FILE_OK;
    }                                                                           //�����ļ��ɹ�
}

#if 0
/*****************************************************************************
** Descriptions:	�������ļ�,���ԭ�ļ������ڻ������Ͷ������ļ�.
** Parameters:    	const u8 *lpFile:��·���ļ�����
                    u8 *lpOut:�������ݻ���
                    u32 nLen:����
** Returned value:	
                    FILE_OK:    �����ļ��ɹ�
                    FILE_EOF:   �����ļ�β
                    FILE_ERROR: �����ļ�ʧ��
** Created By:		������
** Modified By:		wangmingming  2014.05.19
** Remarks: 	
                    �ȶ�ȡԭ�ļ�����ȡʧ�ܣ���������SDK_FILE_ERROR��
                    ԭ�ļ���ȡ�ɹ�������У����ȷ����������SDK_FILE_OK��
                    ԭ�ļ���ȡ�ɹ���У�鲻��ȷ��ԭ�ļ������ļ�β������£����ȡ�����ļ���
                    ��������ļ���ȡ�ɹ�������У��ɹ����������ļ������ݿ�����ԭ�ļ��У�
                        ��������SDK_FILE_OK��
                    ��������ļ�����β����������SDK_FILE_EOF;
                    ��������ļ���ȡʧ�ܣ��򷵻�SDK_FILE_ERROR;
                    ������ļ��Ĵ���ΪSDK_FILE_ERROR�����ú���Exception(ERRORNO_FILE)��ʾ������Ϣ��
                        Ҫ��ػ�����.
*****************************************************************************/
static s32 sdkXgdReadOverlap(const u8 *lpFile, u8 *lpOut, u32 nLen)
{
    u8 pBak[SDK_MAX_PATH] = {0};
    s32 ret;
    u16 crc, m;
#ifdef FAST_FILE_FLAG     
    u32 le;
#endif    
    u8 crcData[2];
    s32 fp;    
    
#ifdef FAST_FILE_FLAG    
    //��Դ�ļ�
    if(sdkIsFastFile(lpFile) == true)             //�����ļ�
    {
        le = nLen;
        ret = sdk_dev_fast_file_read((u8 *)lpFile, lpOut, 0, &le );
        if( ret == SDK_FILE_OK )
        {
            le = 2;
            ret = sdk_dev_fast_file_read(lpFile, crcData, nLen, &le);
        }    
    }
    else
#endif
    {       
        ret = SDK_FILE_EOF;
        fp = ddi_vfs_open(lpFile, "rb");
        if (NULL != fp)
        {
            //������
            if (nLen == ddi_vfs_read(lpOut, nLen, fp))
            {
                if (2 == ddi_vfs_read(crcData, 2, fp))
                {
                    ret = SDK_FILE_OK;
                }  
            }    
            ddi_vfs_close(fp); 
        }
    }

    switch (ret)
    {
         case SDK_FILE_OK:
            {
                crc = (u16)sdkCalcCrc16(lpOut, nLen);
                m = crcData[0];
                m <<= 8;
                m |= crcData[1];
                //Trace("file", "crc=%d, m=%d, le=%d, nLen=%d\r\n", crc, m, le, nLen);

                if (crc == m)
                {
                    break;
                }
            }
            break;

         case SDK_FILE_EOF:
            {
                //�����ļ�
                if( strlen(lpFile) + sizeof(".bak") > SDK_MAX_PATH )
                {
                    Assert(0);
                    return SDK_PARA_ERR;        
                }
                memset(pBak, 0, SDK_MAX_PATH);
                
                strcpy(pBak, lpFile);
                strcat(pBak, ".bak");  
#ifdef FAST_FILE_FLAG                  
                if(sdkIsFastFile(pBak) == true)             //�����ļ�
                {
                    le = nLen;
                    ret = sdk_dev_fast_file_read((u8 *)pBak, lpOut, 0, &le );
                    if( ret == SDK_FILE_OK )
                    {
                        le = 2;
                        ret = sdk_dev_fast_file_read(pBak, crcData, nLen, &le);
                    }    
                }
                else
#endif                
                {       
                    ret = SDK_FILE_EOF;
                    fp = ddi_vfs_open(pBak, "rb");
                    if (NULL != fp)
                    {
                        //������
                        if (nLen == ddi_vfs_read(lpOut, nLen, fp))
                        {
                            if (2 == ddi_vfs_read(crcData, 2, fp))
                            {
                                ret = SDK_FILE_OK;
                            }  
                        }    
                        ddi_vfs_close(fp); 
                    }
                }

                switch (ret)
                {
                     case SDK_FILE_OK:
                        {
                            crc = (u16)sdkCalcCrc16(lpOut, nLen);
                            m = crcData[0];
                            m <<= 8;
                            m |= crcData[1];
                            //Trace("file", "crc=%d, m=%d, le=%d, nLen=%d\r\n", crc, m, le, nLen);

                            if (crc == m)
                            {
                                fp = ddi_vfs_open(lpFile, "w+");
                                if (fp == NULL)
                                {
                                    Assert(0);
                                    return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
                                }
                                if( ddi_vfs_write(lpOut, nLen, fp) != nLen )
                                {
                                    ddi_vfs_close(fp);//�ر��ļ�
                                    Assert(0);
                                    return SDK_FILE_ERROR;//д���ɹ�
                                }
                                if( ddi_vfs_write(crcData, 2, fp) != nLen )
                                {
                                    ddi_vfs_close(fp);//�ر��ļ�
                                    Assert(0);
                                    return SDK_FILE_ERROR;//д���ɹ�
                                }        
                                ddi_vfs_close(fp);//�ر��ļ� 

                                break;
                            }
                        }
                        break;

                     case SDK_FILE_EOF:
                        {
                            //Trace("file", ">>> here <<<\r\n");
                            ret = SDK_FILE_EOF;
                        }
                        break;

                     default:
                        {
                            Assert(0);
                            ret = SDK_FILE_ERROR;
                        }
                        break;
                }
            }
            break;

         default:
            {
                Assert(0);
                ret = SDK_FILE_ERROR;
            }
            break;
    }

    return ret;
}
#endif

/*****************************************************************************
** Descriptions:	����ļ��Ƿ����
** Parameters:    	const u8* pasFile:�������ļ�������·����
** Returned value:	
                    true	�ļ�����
                    flase	�ļ�������
** Created By:		shiweisong  2013.01.17
** Modified By:     wangmingming  2014.05.12
** Remarks: 		
*****************************************************************************/
bool sdkAccessFile(const u8 *pasFile) {
    if (NULL == pasFile) {
        Assert(0);
        return false; //zhouzhihua 2013.07.01 8:31
    }

    /*=======BEGIN: wangmingming 2014.06.05  13:33 modify===========*/
    if (DDI_OK == ddi_vfs_access(pasFile)) {
        return true;
    }
    /*====================== END======================== */

    return false;
}

#ifdef FAST_FILE_FLAG
static bool sdkIsFastFile(const u8 *pasFile)
{
    return sdk_dev_is_fast_file(pasFile);   //�����ļ�
}
#endif

/*****************************************************************************
** Descriptions:	����д�ļ�,����ļ�������,�Ȱ��ļ�Ŀ¼��νӿڴ����ļ���
                    �����汸���ļ���
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    const u8 * pheSrc:��д������
                    s32 siSrclen:��д�����ݳ���
** Returned value:	
                    SDK_FILE_OK	    �����ļ���ȷ
                    SDK_PARA_ERR	�������󷵻�
                    SDK_FILE_ERROR	�����ļ�ʧ��
** Created By:		ʯ���� 2012.05.16 10:55:12
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
                    ���Դ�ļ����ڣ�����������е����ݣ���д�����ݣ�
                    ���Դ�ļ������ڣ��Ȱ���pasFileָ����·�������ļ�����д������.
                    ����ļ��򿪡�������д����ִ��󣬵���Exception������
                    ����ERRORNO_FILE��Ϊ���������ʾ������Ϣ����Ҫ��ػ�����.
*****************************************************************************/
s32 sdkWriteFile(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen) {
    s32 ret;

    if (NULL == pasFile || NULL == pheSrc || siSrclen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true)             //�����ļ�
    {
        ret =  sdk_dev_fast_file_insert(pasFile, pheSrc, 0, siSrclen);
    }
    else
#endif
    {
        ret = sdkXgdWriteFile((u8 *) pasFile, (u8 *) pheSrc, (u32) siSrclen);
    }

    if (ret == SDK_FILE_OK) {
        //Trace("sdkXgdWriteFile", "д�����ݳɹ�");
        return SDK_FILE_OK;
    }
    else {
        Assert(0);
        return SDK_FILE_ERROR;
    }
}

/*****************************************************************************
** Descriptions:	���ļ�����λ�ö�ȡ���ⳤ�ȵ�����,����ļ�������,
                    �Ȱ��ļ�Ŀ¼��νӿڴ����ļ�
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    u8 * pheDest:��������
                    s32 siStart:��λ��
                    s32 * psiDestlen:����������(����ʱ���汣��ʵ�ʶ�ȡ����)
** Returned value:	
                    ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
                    �����ļ���ȷ�򷵻�SDK_FILE_OK
                    �����ļ�β�򷵻�SDK_FILE_EOF
                    �����ļ�ʧ���򷵻�SDK_FILE_ERROR��
** Created By:		ʯ���� 2012.05.16 11:18:21
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
                    ���pFile�����ڣ��򴴽�֮�����ҵ���Exception��������
                    ����ļ��򿪻򴴽�ʧ�ܣ���������SDK_FILE_ERROR��
                    �����ȡ���ݶ����ļ�β����������SDK_FILE_EOF
*****************************************************************************/
s32 sdkReadFile(const u8 *pasFile, u8 *pheDest, s32 siStart, s32 *psiDestlen) {
    s32 ret;

    if (NULL == pasFile || NULL == pheDest || NULL == psiDestlen || siStart < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true)             //�����ļ�
    {
        ret =  sdk_dev_fast_file_read((u8 *)pasFile, pheDest, siStart, psiDestlen);
    }
    else
#endif
    {
        ret = sdkXgdReadFile(pasFile, pheDest, (u32) siStart, psiDestlen);
    }

    return ret;
}

/*****************************************************************************
** Descriptions:	���ļ�β��׷������
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    const u8 * pheSrc:��д������
                    s32 siSrclen:���ȣ���׷�ӵĸ���
** Returned value:	
                    ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
                    �����ļ���ȷ�򷵻�SDK_FILE_OK
                    �����ļ�ʧ���򷵻�SDK_FILE_ERROR��
** Created By:		ʯ���� 2012.05.16 10:55:12
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
                    ��pasFile��ָ���ļ�������ļ������ڣ��򴴽��ö������ļ�����
                        ���ļ���β��׷��pheSrcָ�������.
                    ����ļ��������򿪡�д��������һ���ִ���������SDK_FILE_ERROR��
                        �����ú���Exception(ERRORNO_FILE)��ʾ������ʾ��Ϣ��Ҫ��ػ����ԣ�
                    ������ݳɹ�׷�ӣ���������SDK_FILE_OK
*****************************************************************************/
s32 sdkAppendFile(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen) {
    s32 ret;

    if (NULL == pasFile || NULL == pheSrc || siSrclen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#ifdef FAST_FILE_FLAG

    if(sdkIsFastFile(pasFile) == true)             //�����ļ�
    {
        ret =  sdk_dev_fast_file_insert((u8 *)pasFile, pheSrc, sdkGetFileSize(pasFile), siSrclen);
    }
    else
#endif
    {
        ret = sdkXgdAppendFile((u8 *) pasFile, (u8 *) pheSrc, (u32) siSrclen);
    }

    return ret;
}

/*****************************************************************************
** Descriptions:	���ļ�����λ�ò������ⳤ�ȵ�����
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    const u8 * pheSrc:��׷������
                    s32 siStart:����λ��
                    s32 siSrclen:����
** Returned value:	
                    ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
                    SDK_FILE_OK:    �����ļ��ɹ�
                    SDK_FILE_ERROR: �����ļ�ʧ��
                    SDK_FILE_SEEK_ERROR: �����Ƿ���λ
** Created By:		ʯ���� 2012.05.16 11:43:10
** Modified By:		wangmingming  2014.05.19
** Remarks: 	
                    ���pasFile�����ڣ��򴴽�֮;
                    ����ļ��򿪻򴴽�ʧ�ܣ���������SDK_FILE_ERROR;
                    ���uiStartֵ�����ļ����Ѵ����ݵĳ��ȣ���������SDK_PARA_ERR.
                    �������������쳣����������Exception��������
*****************************************************************************/
s32 sdkInsertFile(const u8 *pasFile, const u8 *pheSrc, s32 siStart, s32 siSrclen) {
    s32 ret;
    s32 i;

    if (NULL == pasFile || NULL == pheSrc || siStart < 0 || siSrclen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (siStart != 0) {//�����ʼλ�ò�Ϊ0 ��Ҫ�϶�һ��
        //��ȡ�ļ���С
        i = sdkGetFileSize(pasFile);//shijianglong 2013.02.20 14:3û��Ҫ�жϣ����溯���������
        if (i < siStart) {//������λ�ó����ļ�β
            Assert(0);
            return SDK_PARA_ERR;
        }
    }
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true)                 //�����ļ�
    {
        ret =  sdk_dev_fast_file_insert((u8 *)pasFile, pheSrc, siStart, siSrclen);
    }
    else
#endif
    {
        ret = sdkXgdInsertFile((u8 *) pasFile, (u8 *) pheSrc, (u32) siStart, (u32) siSrclen);
    }

    return ret;
}

/*****************************************************************************
** Descriptions:	����д�ļ�,ͬʱ���汸���ļ�
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    const u8 * pheSrc:��д������
                    s32 siSrclen:����
** Returned value:	
                    ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
                    �����ļ���ȷ�򷵻�SDK_FILE_OK
                    �����ļ�ʧ���򷵻�SDK_FILE_ERROR��
** Created By:		ʯ���� 2012.05.16 10:55:12
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
*****************************************************************************/
s32 sdkSaveOverlap(const u8 *pasFile, const u8 *pheSrc, s32 siSrclen) {
    s32 fp;
    s32 ret;
    u16 crc;
    u8 crcData[2];
    u8 pBak[SDK_MAX_PATH] = {0};

    if (NULL == pasFile || NULL == pheSrc || siSrclen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }


    crc = (u16) sdkCalcCrc16(pheSrc, siSrclen);
    crcData[0] = (crc & 0xFF00) >> 8;
    crcData[1] = crc & 0x00FF;
    //Trace("test", "У��crc=%d��Ĵ�д������:%s\r\n", crc, pData);

    //�����ļ�
    if (strlen(pasFile) + sizeof(".bak") > SDK_MAX_PATH) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(pBak, 0, SDK_MAX_PATH);

    strcpy(pBak, pasFile);
    strcat(pBak, ".bak");
    //Trace("test", "Դ�ļ�:%s,�����ļ�:%s\r\n", lpFile, pBak);

    //����д���ݵ������ļ�                                
    fp = ddi_vfs_open(pBak, "w+");
    if (fp == NULL) {
        Assert(0);
        return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
    }
    if (ddi_vfs_write(pheSrc, siSrclen, fp) != siSrclen) {
        ddi_vfs_close(fp);//�ر��ļ�
        Assert(0);
        return SDK_FILE_ERROR;//д���ɹ�
    }
    if (ddi_vfs_write(crcData, 2, fp) != 2) {
        ddi_vfs_close(fp);//�ر��ļ�
        Assert(0);
        return SDK_FILE_ERROR;//д���ɹ�
    }
    ddi_vfs_close(fp);//�ر��ļ�
    //Trace("file","д���ݵ������ļ��ɹ�\r\n" );

    //����д���ݵ�Դ�ļ�
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true)             //�����ļ�
    {
        ret = sdk_dev_fast_file_insert(pasFile, pheSrc, 0, siSrclen);
        if( ret == SDK_FILE_OK )
        {
            ret = sdk_dev_fast_file_insert(pasFile, crcData, siSrclen, 2);
        }
    }
    else
#endif
    {
        fp = ddi_vfs_open(pasFile, "w+");
        if (fp == NULL) {
            Assert(0);
            return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
        }
        if (ddi_vfs_write(pheSrc, siSrclen, fp) != siSrclen) {
            ddi_vfs_close(fp);//�ر��ļ�
            Assert(0);
            return SDK_FILE_ERROR;//д���ɹ�
        }
        if (ddi_vfs_write(crcData, 2, fp) != 2) {
            ddi_vfs_close(fp);//�ر��ļ�
            Assert(0);
            return SDK_FILE_ERROR;//д���ɹ�
        }
        ddi_vfs_close(fp);//�ر��ļ�     
        ret = SDK_FILE_OK;
    }
    //Trace("file","д���ݵ�Դ�ļ��ɹ�\r\n" );

    //Trace("file", "ret file %d\r\n", ret);

    return ret;

}

/*****************************************************************************
** Descriptions:	�������ļ�,���ԭ�ļ������ڻ������Ͷ������ļ�.
** Parameters:    	const u8 * pasFile:��·���ļ�����
                    u8 * pheDest:�������ݻ���
                    s32 siDestlen:����
** Returned value:	
                    ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
                    SDK_FILE_OK:    �����ļ��ɹ�
                    SDK_FILE_EOF:   �����ļ�β
                    SDK_FILE_ERROR: �����ļ�ʧ��
** Created By:		ʯ���� 2012.05.16 11:52:52
** Modified By:		wangmingming  2014.05.19
** Remarks: 	
                    �ȶ�ȡԭ�ļ�����ȡʧ�ܣ���������SDK_FILE_ERROR��
                    ԭ�ļ���ȡ�ɹ�������У����ȷ����������SDK_FILE_OK��
                    ԭ�ļ���ȡ�ɹ���У�鲻��ȷ��ԭ�ļ������ļ�β������£����ȡ�����ļ���
                    ��������ļ���ȡ�ɹ�������У��ɹ����������ļ������ݿ�����ԭ�ļ��У�
                        ��������SDK_FILE_OK��
                    ��������ļ�����β����������SDK_FILE_EOF;
                    ��������ļ���ȡʧ�ܣ��򷵻�SDK_FILE_ERROR;
                    ������ļ��Ĵ���ΪSDK_FILE_ERROR�����ú���Exception(ERRORNO_FILE)��ʾ������Ϣ��
                        Ҫ��ػ�����.
*****************************************************************************/
s32 sdkReadOverlap(const u8 *pasFile, u8 *pheDest, s32 siDestlen) {
    u8 pBak[SDK_MAX_PATH] = {0};
    s32 ret;
    u16 crc, m;
    //u32 le;
    u8 crcData[2] = {0};
    s32 fp;

    if (NULL == pasFile || NULL == pheDest || siDestlen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }


    //��Դ�ļ�
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true)             //�����ļ�
    {
        le = siDestlen;
        ret = sdk_dev_fast_file_read((u8 *)pasFile, pheDest, 0, &le );
        if( ret == SDK_FILE_OK )
        {
            le = 2;
            ret = sdk_dev_fast_file_read(pasFile, crcData, siDestlen, &le);
        }    
    }
    else
#endif
    {
        ret = SDK_FILE_EOF;
        fp = ddi_vfs_open(pasFile, "rb");
        if (NULL != fp) {
            //������
            if (siDestlen == ddi_vfs_read(pheDest, siDestlen, fp)) {
                if (2 == ddi_vfs_read(crcData, 2, fp)) {
                    ret = SDK_FILE_OK;
                }
            }
            ddi_vfs_close(fp);
        }
    }

    switch (ret) {
        case SDK_FILE_OK: {
            crc = (u16) sdkCalcCrc16(pheDest, siDestlen);
            m = crcData[0];
            m <<= 8;
            m |= crcData[1];
            //Trace("file", "crc=%d, m=%d, le=%d, siDestlen=%d\r\n", crc, m, le, siDestlen);

            if (crc == m) {
                break;
            }
        }
            break;

        case SDK_FILE_EOF: {
            //�����ļ�
            if (strlen(pasFile) + sizeof(".bak") > SDK_MAX_PATH) {
                Assert(0);
                return SDK_PARA_ERR;
            }
            memset(pBak, 0, SDK_MAX_PATH);

            strcpy(pBak, pasFile);
            strcat(pBak, ".bak");
#ifdef FAST_FILE_FLAG
            if(sdkIsFastFile(pBak) == true)             //�����ļ�
            {
                le = siDestlen;
                ret = sdk_dev_fast_file_read((u8 *)pBak, pheDest, 0, &le );
                if( ret == SDK_FILE_OK )
                {
                    le = 2;
                    ret = sdk_dev_fast_file_read(pBak, crcData, siDestlen, &le);
                }
            }
            else
#endif
            {
                ret = SDK_FILE_EOF;
                fp = ddi_vfs_open(pBak, "rb");
                if (NULL != fp) {
                    //������
                    if (siDestlen == ddi_vfs_read(pheDest, siDestlen, fp)) {
                        if (2 == ddi_vfs_read(crcData, 2, fp)) {
                            ret = SDK_FILE_OK;
                        }
                    }
                    ddi_vfs_close(fp);
                }
            }

            switch (ret) {
                case SDK_FILE_OK: {
                    crc = (u16) sdkCalcCrc16(pheDest, siDestlen);
                    m = crcData[0];
                    m <<= 8;
                    m |= crcData[1];
                    //Trace("file", "crc=%d, m=%d, le=%d, siDestlen=%d\r\n", crc, m, le, siDestlen);

                    if (crc == m) {
                        fp = ddi_vfs_open(pasFile, "w+");
                        if (fp == NULL) {
                            Assert(0);
                            return SDK_FILE_ERROR;//�ļ��򿪻��ߴ���ʧ��
                        }
                        if (ddi_vfs_write(pheDest, siDestlen, fp) != siDestlen) {
                            ddi_vfs_close(fp);//�ر��ļ�
                            Assert(0);
                            return SDK_FILE_ERROR;//д���ɹ�
                        }
                        if (ddi_vfs_write(crcData, 2, fp) != siDestlen) {
                            ddi_vfs_close(fp);//�ر��ļ�
                            Assert(0);
                            return SDK_FILE_ERROR;//д���ɹ�
                        }
                        ddi_vfs_close(fp);//�ر��ļ�

                        break;
                    }
                }
                    break;

                case SDK_FILE_EOF: {
                    //Trace("file", ">>> here <<<\r\n");
                    ret = SDK_FILE_EOF;
                }
                    break;

                default: {
                    Assert(0);
                    ret = SDK_FILE_ERROR;
                }
                    break;
            }
        }
            break;

        default: {
            Assert(0);
            ret = SDK_FILE_ERROR;
        }
            break;
    }

    return ret;

}

/*****************************************************************************
** Descriptions:	�õ��ļ���С
** Parameters:    	const u8 * pasFile:��·���ļ�����
** Returned value:	
                    ��ȷ�򷵻��ļ���С
                    �ļ������ڻ��ߴ�ʧ���򷵻�SDK_FILE_NONE��
                    ����ָ��Ϊ���򷵻�SDK_PARA_ERR
** Created By:		ʯ���� 2012.05.16 14:27:40
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
*****************************************************************************/
s32 sdkGetFileSize(const u8 *pasFile) {
#ifdef FAST_FILE_FLAG
    if(sdkIsFastFile(pasFile) == true) 
    {//�����ļ�
        return sdk_dev_fast_file_get_size(pasFile);
    }
    else
#endif
    {
        u32 i;
        s32 fp;

        if (NULL == pasFile) {
            Assert(0);
            return SDK_PARA_ERR;
        }

        fp = ddi_vfs_open(pasFile, "rb");
        if (0 == fp) {
            Assert(0);
            return SDK_FILE_NONE;                                                                                       //�ļ���ʧ��
        }

        //���ö�дָ�뵽�ļ�β
        ddi_vfs_seek(fp, 0, SEEK_END);

        //��ȡ�ļ���дλ��
        i = (u32) ddi_vfs_tell(fp);
        //Trace("test", "i=%d\r\n", i);

        //�ر��ļ�
        ddi_vfs_close(fp);

        //Trace("test", "i=%d\r\n", i);
        return i;

    }
}

/*****************************************************************************
** Descriptions:	ɾ��ָ��·���ļ�
** Parameters:    	const u8 * pasFile:��ɾ�ļ�
** Returned value:	
                    ����ָ��Ϊ�ջ���·������(����256)�򷵻�SDK_PARA_ERR
                    �ɹ��򷵻�SDK_OK
                    �����򷵻�SDK_ERR��
** Created By:		ʯ���� 2012.05.16 14:15:57
** Modified By:		wangmingming  2014.05.19
** Remarks: 		
*****************************************************************************/
s32 sdkDelFile(const u8 *pasFile) {
    if (pasFile == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#ifdef FAST_FILE_FLAG
    //�����ļ�����رպ�ſ���ɾ��
    if(sdk_dev_is_fast_file(pasFile) == true)
    {
        Assert(0);
        return SDK_ERR;
    }
#endif
    /*=======BEGIN: wangmingming 2014.06.05  13:36 modify===========*/
    //return sdk_dev_sys_del_file(pasFile);
    if (DDI_OK == ddi_vfs_remove(pasFile)) {
        return SDK_OK;
    }

    return SDK_ERR;
    /*====================== END======================== */
}

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
s32 sdkRenameFile(const u8 *pasOldFile, const u8 *pasNewFile) {
    u32 len;
    u32 oldLen;
    u32 newLen;
    u32 i = 0;
    s32 j = -1;

    if (pasOldFile == NULL || pasNewFile == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    oldLen = strlen(pasOldFile);
    newLen = strlen(pasNewFile);
    len = oldLen < newLen ? oldLen : newLen;
    while (i < len) {
        if (pasOldFile[i] == pasNewFile[i]) {
            if (pasOldFile[i] == '/' || pasOldFile[i] == '\\') {
                j = i;
            }
            i++;
        }
        else {
            break;
        }

    }
    if (j == -1)//Ŀ¼��һ��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    j++;
    i = j;
    while (i < oldLen) {
        if (pasOldFile[i] == '/' || pasOldFile[i] == '\\') {
            Assert(0);
            return SDK_PARA_ERR;//Ŀ¼������
        }
        i++;
    }
    i = j;
    while (i < newLen) {
        if (pasNewFile[i] == '/' || pasNewFile[i] == '\\')//Ŀ¼������
        {
            Assert(0);
            return SDK_PARA_ERR;//Ŀ¼������
        }
        i++;
    }
    if (DDI_OK == ddi_vfs_rename(pasOldFile, pasNewFile)) {
        return SDK_OK;
    }
    return SDK_ERR;
    /*====================== END======================== */
}


/*****************************************************************************
** Descriptions:	���������ļ�
                    ����ļ������� ���Զ�����
                    �ô˺�������һ�����ٸ���
                    ��ͬһ���ļ��ڹر�ǰֻ���Ե���һ��
                    ������� ���������ķǿ����ļ��������� �����Բ������ļ�
                    ���� �������
** Parameters:          u8*pas
** Returned value:
** Created By:		shiweisong  2013.10.16
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkCreatFastFile(const u8 *pas) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_create_fast_temp_file(pas);
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	�رտ����ļ�
                    ɾ�����ٸ����ļ���
                    �Ѹ����ļ����Ƶ�Դ�ļ�
                    �˺����Ƚ��� һ��Ҫ�ȴ�����д����ٵ���
                    ������� �����������ļ��������� ���Բ������ļ���
** Parameters:          u8*pas
** Returned value:
** Created By:		shiweisong  2013.10.16
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkCloseFastFile(const u8 *pas) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_close_fast_file(pas);
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	��ȡFLASH�����ļ�
** Parameters:      u32 uiAddress, u8 * pheDest, s32 * psiDestlen
** Returned value:
** Created By:	����ҫ  2013.11.04
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkReadFlashData(u32 uiAddress, const u8 *pheSrc, const s32 *psiSrclen) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_read_flash_data(uiAddress, pheSrc, psiSrclen);
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	д������FLASH�ļ�
** Parameters:     u32 uiAddress, const u8 * pheSrc, s32 siSrclen
** Returned value:
** Created By:	����ҫ  2013.11.04
** Remarks:         501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkWriteFlashData(u32 uiAddress, const u8 *pheSrc, s32 siSrclen) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_write_flash_data(uiAddress, pheSrc, siSrclen);
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	����FLASH���ݿ�  OXFF
** Parameters:     u32 uiAddress
** Returned value:
** Created By:	����ҫ  2013.11.04
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkEraseFlashData(u32 uiAddress) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_erase_flash_data(uiAddress);
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	����ļ�ϵͳ
** Parameters:
** Returned value:
** Created By:	����ҫ  2013.11.04
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkCheckFileSys(void) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_check_file_system();
#else
    return SDK_ERR;
#endif
}

/*****************************************************************************
** Descriptions:	ɾ���ļ�ϵͳ
** Parameters:     u32 uiAddress
** Returned value:
** Created By:	����ҫ  2013.11.04
** Remarks:              501ר�� ��Ļ���������
*****************************************************************************/
s32 Private_sdkDeleteFileSys(void) {
#ifdef FAST_FILE_FLAG
    return sdk_dev_delete_file_system();
#else
    return SDK_ERR;
#endif
}

