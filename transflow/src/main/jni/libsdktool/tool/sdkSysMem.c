#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "sdkList.h"

#ifndef  DEBUG_MEM
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

#ifdef SDK_MEM_DEBUG

#define MEMORY_NMB  350
#define File_NMB    150

typedef struct sfiletable{
    u8 fileid;
    u8* fileName;
}sfiletable;

typedef struct memRec_{
    u16 memcount;//�Ѿ�������ڴ�����
    u32 memptr[MEMORY_NMB];//�ڴ�ָ��
    u16 mempcount[MEMORY_NMB];//������ڴ��С
    s16 lineNmb[MEMORY_NMB];//�к�
    u8 fileID[MEMORY_NMB];//�ļ��� 
}memRec;

static u8 fileNameBuf[2300] = {0};
static u16 fileNameIndex = 0;
static u8* fb[File_NMB + 1] = {0};
static u32 maxMemSize = 0;
static memRec *memRecInfo = NULL;
static u16 memTimes = 0;
static u8* get_file_name( u8* filename)
{
  u32 len = strlen(filename);
  s32 i = len-1;  

  while( i >= 0 )
  {
      if( filename[i] == '\\' )
      {
        return &filename[i+1];
      }
      i--;
  }
  return &filename[len];
}
/*****************************************************************************
** Descriptions:	��̬�ڴ�����
** Parameters:    	unsigned nbytes
** Returned value:	
** Created By:		���ƽ  2014.05.20
** Remarks: 		
*****************************************************************************/
void *sdkDebugGetMem(s32 nbytes, u8* strFun, s32 nline )
{
    u32 i = 0, j = 0;
    u8 memstr[256] = {0};
    void *p = NULL;
    u8* tempF;

    if( memRecInfo == NULL )
    {
        memRecInfo = (memRec *)ddi_k_malloc(sizeof(memRec));
        if( memRecInfo == NULL )
        {
            return NULL;
        }
        memRecInfo->memcount = 0;//�Ѿ�������ڴ�����
        memset( memRecInfo->memptr, 0, MEMORY_NMB*sizeof(u32) );//�ڴ�ָ��
        memset( memRecInfo->mempcount, 0, MEMORY_NMB*sizeof(u16) );//������ڴ��С
        memset( memRecInfo->lineNmb, -1, MEMORY_NMB*sizeof(s16) );//�к�
        memset( memRecInfo->fileID, 256, MEMORY_NMB*sizeof(u8) );//�ļ��� 
        
        memset( fileNameBuf, 0, 2300 );
        fileNameIndex = 0;
        memset( fb, 0, sizeof(fb) ); 
        fb[File_NMB] = "c�ļ�����";
        memTimes = 0;

        
        memRecInfo->memptr[0] = (u32)memRecInfo;
        memRecInfo->mempcount[0] = sizeof(memRec);
        memRecInfo->lineNmb[0] = 86; 
        
        j = 0;
        while( j < File_NMB )
        {
            tempF = get_file_name(strFun);
            u16 len = strlen(tempF) - 2;
            if( fb[j] == NULL )//��һ�η��� x.c�ļ�
            {
                if( fileNameIndex + len > 2300 )
                {
                    ddi_sys_printf("c�ļ����������,����SDK����ʦ�����ļ�����\r\n");
                }
                memcpy( fileNameBuf + fileNameIndex, tempF, len );
                fb[j] = fileNameBuf + fileNameIndex;
                fileNameIndex += len + 1;
                memRecInfo->fileID[0] = j;
                break;
                
            }
            else if( strncmp(fb[j], tempF, len) == 0 )
            {
                memRecInfo->fileID[0] = j;
                break;
            }
            j++;
        }
        if( j == File_NMB )//c�ļ���¼����
        {
            memRecInfo->fileID[0] = j;
            ddi_sys_printf("c�ļ���¼����,����SDK����ʦ�����ļ���������\r\n");
        }             
        memRecInfo->memcount += sizeof(memRec);
        memTimes++;
        
#if 0 /*Modify by ���ƽ at 2015.04.16  16:15 */
        memset( memstr, 0 , sizeof(memstr));
        sprintf( memstr, "T:%d,u:%d, f:%s, L:%d, S:%d\r\n", memTimes, memRecInfo->memcount, __FILE__, __LINE__, sizeof(memRec) );
        ddi_sys_printf(memstr);    
#endif /* if 0 */
    } 
    p = (void *)ddi_k_malloc(nbytes);
    if( p != NULL )
    {
        while( memRecInfo->memptr[i] != 0 && i < MEMORY_NMB)
        {
            i++;
        }
        if( i < MEMORY_NMB )
        {
            memRecInfo->memptr[i] = (u32)p;
            memRecInfo->mempcount[i] = nbytes;
            memRecInfo->lineNmb[i] = nline; 
            
            j = 0;
            while( j < File_NMB )
            {
                tempF = get_file_name(strFun);
                u16 len = strlen(tempF) - 2;
                if( fb[j] == NULL )//��һ�η��� x.c�ļ�
                {
                    if( fileNameIndex + len > 2300 )
                    {
                        ddi_sys_printf("c�ļ����������,����SDK����ʦ�����ļ�����\r\n");
                    }
                    memcpy( fileNameBuf + fileNameIndex, tempF, len );
                    fb[j] = fileNameBuf + fileNameIndex;
                    fileNameIndex += len + 1;
                    memRecInfo->fileID[i] = j;
                    break;
                    
                }
                else if( strncmp(fb[j], tempF, len) == 0 )
                {
                    memRecInfo->fileID[i] = j;
                    break;
                }
                j++;
            }
            if( j == File_NMB )//c�ļ���¼����
            {
                memRecInfo->fileID[i] = j;
                ddi_sys_printf("c�ļ���¼����,����SDK����ʦ�����ļ���������\r\n");
            }             
            memRecInfo->memcount += nbytes;
            memTimes++;
#if 0 /*Modify by ���ƽ at 2014.10.27  15:14 */
            //if( strncmp( tempF, "sdkDebug", 8) != 0 )
            {
                memset( memstr, 0 , sizeof(memstr));
                sprintf( memstr, "T:%d,u:%d, f:%s, L:%d, S:%d\r\n", memTimes, memRecInfo->memcount,  strFun, nline, nbytes );
                ddi_sys_printf(memstr); 
            }
#endif /* if 0 */
			if( memRecInfo->memcount > maxMemSize )
			{
				maxMemSize = memRecInfo->memcount;
                memset( memstr, 0 , sizeof(memstr));
                sprintf( memstr, "File:%s,Line:%d,nbytes:%d�ڴ�ʹ���¼�¼��%d\r\n", strFun, nline, nbytes, maxMemSize );
                ddi_sys_printf(memstr); 				
			}
        }
        else
        {
            u8* fileName = get_file_name(strFun);
            ddi_sys_printf("�ڴ��¼����,����SDK����ʦ�����ڴ��¼����\r\n");
            sprintf( memstr, "��ʹ���ڴ�%d, �ļ�%s, ��%d, ��С%d\r\n", memRecInfo->memcount, fileName, nline, nbytes );
            ddi_sys_printf(memstr);
        }
    }
    else
    {
        u8* fileName = get_file_name(strFun);
        memset( memstr, 0 , sizeof(memstr));
        sprintf( memstr, "\r\n�����ڴ�ʧ�� �ļ�%s, ��%d, ��С%d!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n", fileName, nline, nbytes );
        ddi_sys_printf(memstr);
        memset( memstr, 0 , sizeof(memstr));
        sprintf( memstr, "��ǰδ�ͷ��ڴ����%d,�ܴ�С:%d\r\n", memTimes, memRecInfo->memcount );
        ddi_sys_printf(memstr); 
        i = 0;
        while( i < MEMORY_NMB)
        {
            if( memRecInfo->memptr[i] != NULL )
            {
                memset( memstr, 0 , sizeof(memstr));
                sprintf( memstr, "%d.f:%s, L:%d, S:%d\r\n", i, fb[memRecInfo->fileID[i]], memRecInfo->lineNmb[i], memRecInfo->mempcount[i] );
                ddi_sys_printf(memstr);  
            }
            i++;
        }    
    }
    return p;
}

#else

void *sdkDebugGetMem(s32 siSize, u8 *pFile, s32 siLine) {
    //Modify by huangkanghui at 2015.03.24 17:00
    if (0 == siSize)
        return (void *) NULL;

    return (void *) ddi_k_malloc(siSize);
}

#endif


/*****************************************************************************
** Descriptions:	��̬�ڴ��ͷ�
** Parameters:    	void *ap
** Returned value:	
** Created By:		���ƽ  2014.05.20
** Remarks: 		
*****************************************************************************/
#ifdef SDK_MEM_DEBUG
s32 sdkFreeMem(void *ap )
{
    u32 i = 0;
    u8 memstr[256] = {0};
    u8* strFun = "sdkFreeMemDebug";
	u32 nline = __LINE__;
    if( ap != NULL && memRecInfo != NULL )
    {
        while( memRecInfo->memptr[i] != (u32)ap && i < MEMORY_NMB)
        {
            i++;
        }
        if( i < MEMORY_NMB )//�ҵ�Ҫ�ͷŵ�ָ���ˡ�
        {
            memRecInfo->memptr[i] = NULL;   
            memRecInfo->memcount -= memRecInfo->mempcount[i];    
            memRecInfo->mempcount[i] = 0;  

            memRecInfo->lineNmb[i] = -1;  
            memRecInfo->fileID[i] = 256;
            memTimes--;
            //sprintf( memstr, "free��ʹ���ڴ�%d,f:%s, l:%d\r\n", memRecInfo->memcount, strFun, nline );
            //ddi_sys_printf(memstr); 
        }
        else 
        {    
            u8* fileName = get_file_name(strFun);
            sprintf(memstr,"����Ұָ����!!!!!!!!!!!!!!!!!!!!!!!!!!,�ͷ����ͷ��ڴ��ļ�%s, ��%d\r\n", fileName, nline); 
            ddi_sys_printf(memstr);
            memset( memstr, 0, sizeof(memstr) );
            sprintf( memstr, "��ʹ���ڴ�%d\r\n", memRecInfo->memcount );
            ddi_sys_printf(memstr);
            
        }
        ddi_k_free(ap);
    }
    return 1;
}


void sdkCleanMemTable()
{
    u32 i = 0;
    u32 flag = 0;
    u8 memstr[256] = {0};

    if( memRecInfo != NULL )
    {
        for( i = 0; i < MEMORY_NMB; i++ )
        {
            if( memRecInfo->memptr[i] != 0 )
            {
                if( memRecInfo->memcount >= memRecInfo->mempcount[i] )
                {
                    memset( memstr, 0 , sizeof(memstr));
                    sprintf( memstr, "!!!!!!!!!!!!!!!!!!!!!!!!!!���ڴ�δ�ͷ�:�ļ�%s, ��%d, ��С%d;!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n", fb[memRecInfo->fileID[i]], memRecInfo->lineNmb[i], memRecInfo->mempcount[i] );
                    ddi_sys_printf(memstr);   
                    flag = 1;
                }
                else
                {
                    memset( memstr, 0 , sizeof(memstr));
                    sprintf( memstr, "##########################���������⣬�����ڴ�δ�ͷ�:�ļ�%s, ��%d, ��С%d;#####################\n", fb[memRecInfo->fileID[i]], memRecInfo->lineNmb[i], memRecInfo->mempcount[i] );
                    ddi_sys_printf(memstr);        
                    flag = 1;
                }
            }
        }
        if( flag == 0 )
        {
            memset( memstr, 0 , sizeof(memstr));
            ddi_sys_printf("******************************û�з����ڴ��쳣����****************************\r\n");         
        }
    }
    else
    {
        memset( memstr, 0 , sizeof(memstr));
        ddi_sys_printf("************************δ�����ڴ���ڴ������δ��ʼ��,����ϵsdk����ʦ**********************\r\n");     
    }

}

void sdkGetMemStatus()
{
    u32 i = 0;
    u32 flag = 0;
    u8 memstr[256] = {0};

    ddi_sys_printf("*********************************����ڴ��ͷ����************************************");  

    if( memRecInfo != NULL )
    {
        for( i = 0; i < MEMORY_NMB; i++ )
        {
            if( memRecInfo->memptr[i] != 0 )
            {
                if( memRecInfo->memcount >= memRecInfo->mempcount[i] )
                {
                    memset( memstr, 0 , sizeof(memstr));
                    sprintf( memstr, "!!!!!!!!!!!!!!!!!!!!!!!!!!���ڴ�δ�ͷ�:�ļ�%s, ��%d, ��С%d;!!!!!!!!!!!!!!!!!!!!!!!!!!\r\n", fb[memRecInfo->fileID[i]], memRecInfo->lineNmb[i], memRecInfo->mempcount[i] );
                    ddi_sys_printf(memstr);   
                    flag = 1;
                }
                else
                {
                    memset( memstr, 0 , sizeof(memstr));
                    sprintf( memstr, "##########################���������⣬�����ڴ�δ�ͷ�:�ļ�%s, ��%d, ��С%d;#####################\n", fb[memRecInfo->fileID[i]], memRecInfo->lineNmb[i], memRecInfo->mempcount[i] );
                    ddi_sys_printf(memstr);        
                    flag = 1;
                }
            }
        }
        if( flag == 0 )
        {
            memset( memstr, 0 , sizeof(memstr));
            ddi_sys_printf("******************************û�з����ڴ��쳣����****************************\r\n");         
        }
        memset( memstr, 0 , sizeof(memstr));
        sprintf( memstr, "�Ѿ�ʹ�õ��ڴ��С%d; \n",  memRecInfo->memcount ); 
        ddi_sys_printf(memstr);           
    }
    else
    {
        memset( memstr, 0 , sizeof(memstr));
        ddi_sys_printf("************************δ�����ڴ���ڴ������δ��ʼ��,����ϵsdk����ʦ**********************\r\n");     
    }
}

#else

s32 sdkFreeMem(void *ap) {
    ddi_k_free(ap);
    return 1;
}

#endif









