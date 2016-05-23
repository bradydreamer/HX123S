#include "sdkGlobal.h"
#include "sm2/sm2check.h"//shijianglong 2013.05.28 16:53
/*****************************************************************************
** Descriptions:        ��ʼ����Բ���߲���
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
s32 sdkSM2InitElliptic(const u8 *pGroupP, const u8 *pGroupA, const u8 *pGroupB, const u8 *pGroupX,
                       const u8 *pGroupY, const u8 *pGroupN) {
    s32 ret = 0;

    if (pGroupP == NULL || pGroupA == NULL || pGroupB == NULL || pGroupX == NULL ||
        pGroupY == NULL || pGroupN == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = InitElliptic(pGroupP, pGroupA, pGroupB, pGroupX, pGroupY, pGroupN);
    if (ret == 1) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

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
s32 sdkSM3Encrypt(u8 *pSrcData, s32 siLen, u8 *pDestData) {
    s32 ret = 0;

    if (pSrcData == NULL || pDestData == NULL || siLen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = SM3Encrypt(pSrcData, siLen, pDestData);
    if (ret == 0) {
        return SDK_ERR;
    }
    else {
        return ret;
    }
}

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
s32 sdkSM2CheckSign(const u8 *pPkeyX, const u8 *pPkeyY, const u8 *pSignR, const u8 *pSignS,
                    const u8 *pSrcData, const u8 *pIDA) {
    s32 ret = 0;

    if (pPkeyX == NULL || pPkeyY == NULL || pSignR == NULL || pSignS == NULL || pIDA == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    ret = CheckSM2Sign(pPkeyX, pPkeyY, pSignR, pSignS, pSrcData, pIDA);
    if (ret == 1) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	�ͷſռ�
** Parameters:    	Input: None 
** Returned value:	SDK_OK :�ɹ�
                                SDK_ERR:ʧ��
** Created By:		shijianglong  2013.05.28
** Remarks: 	��ǩ�������ͷſռ䣬�����ڴ�й©
*****************************************************************************/
s32 sdkSM2ClearAll(void) {
    ClearAll();
    return SDK_OK;
}


