#ifndef __SM2CHECK_H__
#define __SM2CHECK_H__

#ifdef __cplusplus  
extern "C" {  
#endif

/*****************************************************************************
 Function: InitElliptic
 Description:
    ��ʼ����Բ���߲���
 Parameter:
 int:
	const char *pGroupP        Gp����ֵ
	const char *pGroupA        Ga����ֵ
	const char *pGroupB        Gb����ֵ
    const char *pGroupX        Gx����ֵ
    const char *pGroupY        Gy����ֵ
	const char *pGroupN        Gn����ֵ
 out:
    NA 
 Return:
    == 1    �ɹ�
	!= 1    ʧ��
 Create by chenfeng 20130307
Comment:
1.ÿ�����ֵ������ASC�룬�����н�����
*****************************************************************************/
int InitElliptic(const char *pGroupP, const char *pGroupA, const char *pGroupB, const char *pGroupX,const char *pGroupY, const char *pGroupN);
/*****************************************************************************
 Function: SM3Encrypt
 Description:
    SM3����
 Parameter:
 int:
    unsigned char *pInput      ����������
    unsigned short usLen       ���ݳ���
 out:
    unsigned char *pOutput     ���ܺ����� 
 Return:
    == 32    ���ܳɹ���̶�����
	!= 32    ʧ��
 Create by chenfeng 20130307
Comment:�����ǩ����������Ϊ�գ����pIDA�����Ѿ�SM3���ܹ���Ĵ���֤���ݲ�����ֱ����֤������ͨ��pIDA��pSrcData����SM3���к�����֤
*****************************************************************************/
int SM3Encrypt(unsigned char *pInput, unsigned short usLen, unsigned char *pOutput);
/*****************************************************************************
 Function: CheckSM2Sign
 Description:
    У��SM2ǩ���Ϸ���
 Parameter:
 int:
    const char *pPkeyX         ��ԿX����ֵ
    const char *pPkeyY         ��ԿY����ֵ
    const char *pSignR         ǩ��Rֵ
	const char *pSignS         ǩ��Sֵ
	const char *pSrcData       ��ǩ������
	const char *pIDA           �û����
 out:
    NA
 Return:
    ==1   �ɹ�
    !=1   ʧ��
 Create by chenfeng 20130307
Comment:
1.�����ǩ����������Ϊ�գ����pIDA�����Ѿ�SM3���ܹ���Ĵ���֤���ݲ�ֱ����֤������ͨ��pIDA��pSrcData����SM3���������֤
2.ÿ�����ֵ������ASC�룬�����н�����
*****************************************************************************/
int  CheckSM2Sign(const char *pPkeyX, const char *pPkeyY, const char *pSignR, const char *pSignS,const char *pSrcData, const char *pIDA);

/*****************************************************************************
 Function: ClearAll
 Description:
    �ͷſռ�
 Parameter:
 int:
    NA
 out:
    NA
 Return:
    NA
 Create by chenfeng 20130307
Comment:
��ǩ�������ͷſռ䣬�����ڴ�й©
*****************************************************************************/
void ClearAll();

#ifdef __cplusplus  
}  
#endif 

#endif

