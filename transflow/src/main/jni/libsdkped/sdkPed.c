#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "pedprivate.h"

#ifndef  DEBUG_PED
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


// lint +efreeze
// pclintѡ������ɾ��
// lint -efreeze
// pclintѡ������ɾ��
//lint -e768


#define  SDK_PED_NEED_RETRY  (SDK_SYS_RET_ERR_MAX - 1)
static u8 gIsGetPin = 0; //�Ƿ��й����ù�GetPin
static u8 gIsOpenDukpt = 0; //�Ƿ��й����ù�GetPin

static s32 sdkPedDukptDealPin(u8 ucMode, SDK_PED_CRYPT eCrypt, u8 uKsnIndx, u8 *upInData,
                              u32 upDataLen, u8 *upOutData, u8 *upKsn, u8 *upKsnLen,
                              s32 siTimerOut);

/*====================== END======================== */
/*****************************************************************************
** Descriptions:	�Ƿ���������
** Parameters:          void
** Returned value:	true ��
                                false ����
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
bool sdkPEDIsWithPinpad(void) {
    return false;      //����
}

/*****************************************************************************
** Descriptions:	��������������к�
** Parameters:         ���к�����
** Returned value:
** Created By:		zxx2012.11.25
** Remarks:
*****************************************************************************/
s32 Private_sdkPEDSetSN(const u8 *pDataIn) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	   �����û��Զ������ݵ�PED
** Parameters:          const u8 *pDataIn
                               const s32 ucDataLen
** Returned value:
** Created By:		zxx  2013.05.08
** Remarks:
*****************************************************************************/
s32 sdkPEDSaveUserData(const u8 *pDataIn, const s32 ucDataLen) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	����������
** Parameters:         8�ֽ��Ҷ���bcd
** Returned value:
** Created By:		zxx2012.11.25
** Remarks:
*****************************************************************************/
s32 sdkPEDInputAmt(u8 *pbcAmtOut) {
    s32 rslt;
    u8 temp[14] = {0};

    if (NULL == pbcAmtOut) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    //��������ã����ش���
    rslt = sdkKbGetScanf(SDK_PED_TIMEOUT * 15, temp, 1, 12, SDK_MMI_NUMBER | SDK_MMI_POINT,
                         SDK_DISP_LINE3);
    TraceHex("zjp", "temp", temp, 14);
    Verify(sdkAscToBcdR(pbcAmtOut, &temp[1], 8) >= 0);
    TraceHex("zjp", "pbcAmtOut", pbcAmtOut, 14);
    return rslt;
}

/*****************************************************************************
** Descriptions:	��ʼ����ԿоƬ
** Parameters:
** Returned value:
** Created By:		zxx2012.11.25
** Remarks:
*****************************************************************************/
s32 sdkPEDInitKeyIC(void) {
    return SDK_ERR;
}

//��ȡ����Կʵ������
/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucVirtualTmkIndex ����Կ����0-99
                               SDK_PED_DES_TYPE eTmkType����Կ����
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
static u8 sdkPedGetFactTmkIndex(u8 ucVirtualTmkIndex, SDK_PED_DES_TYPE eTmkType) {
    u8 tmkindex = ucVirtualTmkIndex;

    if (eTmkType == SDK_PED_DES_TRIPLE) {
        tmkindex += MK_3DESTHRESHOLD;
    }
    return tmkindex;
}

/*****************************************************************************
** Descriptions:
** Parameters:          1. iTmkIndex: �洢����Կ������
                               0-99
                        2. eKeyType: ��Կ����
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]
                        3. pheKey: ��Կֵ
                        4. iTimeout: ��ʱʱ�� ������100ms
** Returned value:
                        SDK_OK: �ɹ�
                        SDK_PARA_ERR: ��������
                        SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)
** Created By:		lqq2012.11.27
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateTmk(s32 iTmkIndex, SDK_PED_DES_TYPE eKeyType, const u8 *pheKey, s32 iTimeout) {
    s32 rslt = SDK_ERR;
    u8 TmkIndex;

    // ��������Ϸ����ж�
    if (!(pheKey != NULL) || !(iTmkIndex >= 0 && iTmkIndex < 100) || !(iTimeout >= 100)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    // ���µ�������Կ
    TmkIndex = sdkPedGetFactTmkIndex((u8) iTmkIndex, eKeyType); //��Կ����

    if ((eKeyType != SDK_PED_DES_TRIPLE) && (SDK_PED_DES_SINGLE != eKeyType)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    // ��ע��Կ
    rslt = ddi_innerkey_inject(TMK_INDEX_AREA, TmkIndex, pheKey);      //0Ϊ����Կ��
    Trace("zjp", "ddi_innerkey_inject ret = %d\r\n", rslt);

    if (DDI_ERR == rslt || DDI_EBUSY == rslt) {
        rslt = SDK_ERR;
    }
    else {
        rslt = SDK_OK;
    }
    return rslt;
}

//����ʵ�ʹ�����Կ����λ��
/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucVirtualWkIndex  ������Կ��������0-124
                               SDK_PED_DES_TYPE eWkType ������Կ����
** Returned value:
** Created By:		lqq2012.11.26
** Remarks:
*****************************************************************************/
static u8 sdkPedGetFactWkIndex(u8 ucVirtualWkIndex, SDK_PED_DES_TYPE eWkType) {
    u8 wkindex = ucVirtualWkIndex;

    if (eWkType == SDK_PED_DES_TRIPLE) {
        wkindex += WK_3DESTHRESHOLD_DDI;
    }
    return wkindex;
}

/*****************************************************************************
** Descriptions:	���¹�����Կ
** Parameters:          const SDK_WK_CFG *pstWkCfg ������Կ���ýṹ
                               const u8 siWknum������Կ����
                               u32 siTimeout��ʱʱ�䲻����100ms��λms
** Returned value:
** Created By:		//zxx 2012.11.23 10:57
** Remarks:
   ����Կ 0-99
   ������Կ 0-124
*****************************************************************************/
//�����ڲ�ʹ��forѭ����forѭ�����޸���ѭ������
//lint -e850
s32 sdkPEDUpdateWk(SDK_PED_WK_CFG *pstWkCfg, const s32 iWknum, s32 iTimeout) {
    s32 i, rslt = 0;
    u8 tmkindex = 0;
    u8 tempwkindex;
    strInnerTKeyDEncryptPara sttkey;
    u8 buff[64] = {0};

    if (!(pstWkCfg != NULL) || !(iWknum > 0) || !(iTimeout >= 100)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < iWknum; i++) {
        //����Կ����
        if (!(pstWkCfg[i].ucTmkIndex < 101)) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        tmkindex = sdkPedGetFactTmkIndex(pstWkCfg[i].ucTmkIndex, pstWkCfg[i].eTmkType);

        /***********************���ӶԹ�����Կ���ȵ��ж����ײ���Ʋ�������**************************/         //zxx 2012.12.18 15:1
        if (pstWkCfg[0].bOnlyCalcCheckValue) //zxx 2012.12.18 15:0
        {
            pstWkCfg[i].ucCheckValueLen = pstWkCfg[i].ucCheckDataLen;
        }

        if (pstWkCfg[i].eWkType == SDK_PED_DES_SINGLE) {
            if (pstWkCfg[i].ucEnWkLen != 8 || pstWkCfg[i].ucCheckDataLen != 8 ||
                pstWkCfg[i].ucCheckValueLen > 8) {
                return SDK_PARA_ERR;
            }
        }
        else if (pstWkCfg[i].eWkType == SDK_PED_DES_TRIPLE) {
            if (pstWkCfg[i].ucEnWkLen != 16 || pstWkCfg[i].ucCheckDataLen != 16 ||
                pstWkCfg[i].ucCheckValueLen > 16) {
                return SDK_PARA_ERR;
            }
        }

        /*************************************************************************************************************/
        //WK���ĳ���
        if (!(pstWkCfg[i].ucEnWkLen <= 32)) {
            Assert(0);
            return SDK_PARA_ERR;
        }

        //WKЧ����Կ����
        if (!(pstWkCfg[i].ucCheckDataLen <= 32)) {
            Assert(0);
            return SDK_PARA_ERR;
        }
        //���ܹ�����Կ
        //��������Կ���������Կ���ģ�������ʱ��Կ����������ʱ��Կ�������ݶ�У�����ݼ���
        //rslt = sdkPedInsideCheckKey(TMK_INDEX_AREA, tmkindex);
        rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, TMK_INDEX_AREA, tmkindex);
        Trace("zjp", "ddi_innerkey_ioctl = %d\r\n", rslt);

        if (DDI_OK != rslt) {
            return SDK_ERR;
        }
        rslt = ddi_innerkey_decrypt(TMK_INDEX_AREA, tmkindex, pstWkCfg[i].ucEnWkLen,
                                    pstWkCfg[i].heEnWk, buff);          //�˴�Ϊ����Կ�洢�������ؽ��ܽ��
        Trace("zjp", "ddi_innerkey_decrypt = %d\r\n", rslt);

        if (DDI_OK != rslt) {
            return SDK_ERR;
        }
        memset(buff, 0, sizeof(buff));
        sttkey.m_indata = pstWkCfg[i].heCheckData; //&ucData[1 + 1 + wkEnDataLen + 1];
        sttkey.m_len = pstWkCfg[i].ucCheckDataLen; //ucData[1 + 1 + wkEnDataLen];
        sttkey.m_outdata = buff;
        rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_TKEY_ENCRYPT, (u32) & sttkey, 0);
        Trace("zjp", "ddi_innerkey_ioctl = %d\r\n", rslt);

        if (DDI_OK != rslt) {
            return SDK_ERR;
        }
        else {
            if (pstWkCfg[0].bOnlyCalcCheckValue) {
                memcpy(pstWkCfg[i].heCheckValue, sttkey.m_outdata, pstWkCfg[i].ucCheckValueLen);
            }
            else {
                if (memcmp(sttkey.m_outdata, pstWkCfg[i].heCheckValue,
                           pstWkCfg[i].ucCheckValueLen)) {
                    //return SDK_PED_TPK_ERR;
                    return SDK_ERR;
                }
            }
        }

        if (!pstWkCfg[0].bOnlyCalcCheckValue)         //��Ҫ���и�������Կ��
        {
            //������Կ����
            if (!(pstWkCfg[i].ucWkIndex < 125)) {
                Assert(0);
                return SDK_PARA_ERR;
            }
            tempwkindex = sdkPedGetFactWkIndex(pstWkCfg[i].ucWkIndex, pstWkCfg[i].eWkType);

            /*��Ϊddi����ʱ��Կ��ֻ��һ����ֻ�ܽ���һ�Σ�����һ�Σ�������������������Կ����������ֻ�õ���ucTpkIndex��һ��������
               ��������ȿ��Դ�tpk������Ҳ���Դ�tak����������ʱֻ�ø�ucTpkIndex��ֵ����*/
            rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_TKEY_INJECT, WK_INDEX_AREA, tempwkindex);
            Trace("zjp", "ddi_innerkey_ioctl = %d\r\n", rslt);

            if (DDI_OK != rslt) {
                return SDK_ERR;
            }
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ָ���Ĺ���A��Կ����B��Կ�������Ϊ�µĹ�����Կ���������
                    newKeyIndexλ��(���������Կ)
** Parameters:          SDK_PED_DES_TYPE eSrKeyType:A��Կ��������
                    s32 siSrKeyIndex:A��Կ����
                    SDK_PED_DES_TYPE eTgKeyType:B��������
                    s32 siTgKeyIndex:B��Կ����
                    u8 newKeyIndex:����Կ����
** Returned value:
** Created By:		���ƽ  2014.09.24
** Remarks:             ����Կ����Կ������B��Կ����һ��
*****************************************************************************/
s32 sdkPedWkEncryptWk(SDK_PED_DES_TYPE eAWKDesType, u8 siAWKIndex, SDK_PED_DES_TYPE eBWKDesType,
                      u8 siBWKIndex, u8 newWKIndex) {
    struct _strInnerKeyTAKEncryptTAKPara     //ʹ�ù�����Կ���ܹ�����Կ
    {
        u32 m_area;             //��Կ����
        u32 m_target_index;    //�����ܹ�����Կ����
        u32 m_soure_index;    //����ʹ�õĹ�����Կ����
        u32 m_save_index;        //���浽��������
    } KEKData;
    s32 rslt;

    siAWKIndex = sdkPedGetFactWkIndex(siAWKIndex, eAWKDesType);
    siBWKIndex = sdkPedGetFactWkIndex(siBWKIndex, eBWKDesType);
    newWKIndex = sdkPedGetFactWkIndex(newWKIndex, eBWKDesType);

    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, WK_INDEX_AREA, siAWKIndex);
    Trace("zjp", "ddi_innerkey_ioctl ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    KEKData.m_area = WK_INDEX_AREA;
    KEKData.m_target_index = siBWKIndex;
    KEKData.m_soure_index = siAWKIndex;
    KEKData.m_save_index = newWKIndex;
    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_TAK_ENCRYPT_TAK, (u32)(&KEKData), 0);
    Trace("zjp", "ddi_innerkey_ioctl ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	 ������Կ��������
** Parameters:          u8 ucWkIndex ������Կʵ������
                               u8 *pheOut ���ܺ�����8byte
                               u8 *pheIn ���������ݣ�8byte
                               s32 siInLen���������ݳ��� Ŀǰû��ʵ������
** Returned value:
** Created By:		zxx2012.11.23
** Remarks:siInLen���ȱ�����8�ı���//taowz 2014.05.26 10:5
*****************************************************************************/
static s32 sdkPedWkDecryptData(u8 ucWkIndex, u8 *pheOut, const u8 *pheIn, s32 siInLen) {
    s32 rslt;
    //u16 i;
    u8 buf[8] = {0};

    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, WK_INDEX_AREA, ucWkIndex);
    Trace("zjp", "ddi_innerkey_ioctl ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    rslt = ddi_innerkey_decrypt(WK_INDEX_AREA, ucWkIndex, siInLen, pheIn, buf);
    Trace("zjp", "ddi_innerkey_decrypt ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    memcpy(pheOut, buf, siInLen);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������Կ��������
** Parameters:          u8 ucWkIndex������Կʵ������
                               u8 *pheOut���ܺ���8byte
                               u8 *pheIn����������
                               u8 ucLen���������ݳ���(Ŀǰ�ǲ���ֻ֧��8byte)
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
static s32 sdkPedWkEncryptData(u8 ucWkIndex, u8 *pheOut, const u8 *pheIn, u8 ucLen) {
    //u16 i = 0;
    s32 rslt;

    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, WK_INDEX_AREA, ucWkIndex);
    Trace("zjp", "ddi_innerkey_ioctl ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    rslt = ddi_innerkey_encrypt(WK_INDEX_AREA, ucWkIndex, ucLen, pheIn, pheOut);
    Trace("zjp", "ddi_innerkey_encrypt ret = %d\r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            rslt = SDK_PARA_ERR;
        }
        rslt = SDK_ERR;
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucTakIndex  takʵ������
                               u8 *pheOut ���ܺ���
                               u8 *pheIn����������
                               s32 siInLen���������ݳ���
** Returned value:
** Created By:		zxx2012.11.23
** Remarks:
*****************************************************************************/
static s32 sdkPedE90(u8 ucTakIndex, u8 *pheOut, const u8 *pheIn, s32 siInLen) {
    u8 temp[24], temp_rslt[8];
    s32 rslt;

    memset(temp, 0, sizeof(temp));
    sdkEcb(temp, pheIn, siInLen);
    rslt = sdkPedWkEncryptData(ucTakIndex, temp_rslt, temp, 8);

    if (rslt == SDK_OK) {
        Verify(sdkXOR8(temp, temp_rslt, &temp[8]) == SDK_OK);
        rslt = sdkPedWkEncryptData(ucTakIndex, temp_rslt, temp, 8);

        if (rslt == SDK_OK) {
            memset(temp, 0, sizeof(temp));
            Verify(sdkBcdToAsc(temp, temp_rslt, 4) == 8);
            memcpy(pheOut, temp, 8);
        }
    }
    return rslt;
}

/*****************************************************************************
** Descriptions:
** Parameters:         u16 uiCmd ���־  1 x9.19 ,0 x9.9
                       u8 ucwkIndex ����
                       u8 *pucInData ��������
                       u16 uiInLen �������ݳ���
                       u8 *pucOutData ���㷨�����Ľ��
                       u32 siTimer
** Returned value:	�õ����ݵ�ʵ�ʳ���
** Created By:		zhouzhihua  2013.07.18
** Remarks:
*****************************************************************************/
static s32 sdkPedInsideWkCbc(s32 siCmd, u8 ucwkIndex, u8 const *pucInData, u16 uiInLen,
                             u8 *pucOutData, u32 siTimer) {
    u8 temp[8 + 1], A0[8 + 1], A1[8 + 1];
    u16 i, j, t;
    s32 rslt;
    strInnerHKeyDEncryptPara stcrypt;

    if (pucInData == NULL || pucOutData == NULL || uiInLen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
//    return SDK_OK;
/*=======BEGIN: taowz 2014.05.26  11:53 modify===========*/
    //rslt = sdkPedInsideCheckKey(WK_INDEX_AREA, ucwkIndex);
    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, WK_INDEX_AREA, ucwkIndex);
    Trace("devped", "rslt = %d \r\n", rslt);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    //if(SDK_OK != rslt)
    //{
    //    return rslt;
    //}
/*====================== END======================== */
    //pucOutData[1] = 1;//�����������0x22/0x26Э�飬�Ȱ�Res��Ϊ1
    memset(A0, 0, sizeof(A0));
    memset(A1, 0, sizeof(A1));

    for (i = 0; i < ((uiInLen + 7) / 8); i++) {
        memset(temp, 0, sizeof(temp));

        if (((i + 1) * 8) > uiInLen) {
            j = uiInLen - ((uiInLen / 8) * 8);
        }
        else {
            j = 8;
        }
        memcpy(temp, &pucInData[i * 8], j);

        for (t = 0; t < 8; t++) {
            A1[t] = A0[t] ^ temp[t];
        }

        memset(temp, 0, sizeof(temp));

        if (siCmd == 0) //E99
        {
            rslt = ddi_innerkey_encrypt(WK_INDEX_AREA, ucwkIndex, 8, A1, temp);
            Trace("devped", "ddi_innerkey_encrypt = %d \r\n", rslt);

            if (DDI_OK != rslt) {
                if (DDI_EINVAL == rslt) {
                    return SDK_PARA_ERR;
                }
                return SDK_ERR;
            }
        }
        else //E919˫����Կ��ǰ���ֽ�
        {
            stcrypt.m_area = WK_INDEX_AREA;
            stcrypt.m_index = ucwkIndex;
            stcrypt.m_half = 0;
            stcrypt.m_indata = A1;
            stcrypt.m_outdata = temp;
            stcrypt.m_len = 8;

            rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_HKEY_ENCRYPT, (u32) & stcrypt, 0);
            Trace("devped", "ddi_innerkey_ioctl = %d \r\n", rslt);

            if (DDI_OK != rslt) {
                if (DDI_EINVAL == rslt) {
                    return SDK_PARA_ERR;
                }
                return SDK_ERR;
            }
        }
        memcpy(A0, temp, 8);
    }

    if (siCmd == 1) //E919
    {
        memset(temp, 0, sizeof(temp));
        stcrypt.m_area = WK_INDEX_AREA;
        stcrypt.m_index = ucwkIndex;
        stcrypt.m_half = 0;
        stcrypt.m_indata = A0;
        stcrypt.m_outdata = temp;
        stcrypt.m_len = 8;

        rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_HKEY_DECRYPT, (u32) & stcrypt, 0);
        Trace("devped", "ddi_innerkey_ioctl = %d \r\n", rslt);

        if (DDI_OK != rslt) {
            if (DDI_EINVAL == rslt) {
                return SDK_PARA_ERR;
            }
            return SDK_ERR;
        }
        rslt = ddi_innerkey_encrypt(WK_INDEX_AREA, ucwkIndex, 8, temp, A0);
        Trace("devped", "ddi_innerkey_encrypt = %d \r\n", rslt);

        if (DDI_OK != rslt) {
            if (DDI_EINVAL == rslt) {
                return SDK_PARA_ERR;
            }
            return SDK_ERR;
        }
    }
    //memcpy(pucOutData + 2, A0, 8);
    //pucOutData[1] = 0;//���ɹ������mac����ʱ����Res��Ϊ0
    memcpy(pucOutData, A0, 8);
    return 8; //
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 ucTakIndex  takʵ������
                               u8 *pheOut ���ܺ���
                               u8 *pheIn����������
                               s32 siInLen���������ݳ���
                               bool bX919 �Ƿ���919������99cbc
** Returned value:
** Created By:		zxx2012.11.23
** Remarks:         ���ݲ�Ʒ�������ѧ��ָʾ��K370ʹ����ԿоƬ��������������֧��X919�㷨
*****************************************************************************/
static s32 sdkPedCbc(u8 ucTakIndex, u8 *pheOut, const u8 *pheIn, s32 siInLen, bool bX919) {
#define SDK_DES_DATE_LEN 480         //zhouzhihua 2013.12.04 17:16
    u8 temp_rslt[8], temp_out[8] = {0};
    s32 rslt = 0, i, imax;
    u16 len;
    u8 temp[512] = {0};

    imax = (siInLen + SDK_DES_DATE_LEN - 1) / SDK_DES_DATE_LEN;

    len = 0;
    memset(temp, 0, sizeof(temp));
    memset(temp_rslt, 0, sizeof(temp_rslt));

    temp[0] = POS_PCIWKE99;
    temp[1] = ucTakIndex;

    for (i = 0; i < imax; i++) {
        if (i > 0) {
            len = 8;
        }

        if ((i + 1 == imax) && (siInLen % SDK_DES_DATE_LEN))         //���һ�ʣ��Ҳ���480��������
        {
            memset(&temp[2], 0, sizeof(temp) - 2);
            memcpy(&temp[2], temp_rslt,
                   len);   //fusuipu 2013.09.23 18:18 ��һ�����ݵļ��ܽ���������Ϊ��һ�μ�������
            memcpy(&temp[2] + len, pheIn + SDK_DES_DATE_LEN * i, siInLen % SDK_DES_DATE_LEN);
            len = len + (((((u16) siInLen % SDK_DES_DATE_LEN) + 7) / 8)) * 8;
        }
        else {
            memset(&temp[2], 0, sizeof(temp) - 2);
            memcpy(&temp[2], temp_rslt, len);   //fusuipu 2013.09.23 18:18
            memcpy(&temp[2] + len, pheIn + SDK_DES_DATE_LEN * i, SDK_DES_DATE_LEN);
            len += SDK_DES_DATE_LEN;
        }

        if ((i + 1 == imax) && (true == bX919)) {
            temp[0] = POS_PCIWKE919;            //fusuipu 2013.09.23 18:27
        }

        if (POS_PCIWKE919 == temp[0]) {
            rslt = sdkPedInsideWkCbc(1, temp[1], &temp[2], len, temp_out, 500);
            Trace("zjp", "sdkPedInsideWkCbc len =%d\r\n", rslt);

            if (rslt >= 0) {
                rslt = SDK_OK;
            }
        }
        else if (POS_PCIWKE99 == temp[0]) {
            rslt = sdkPedInsideWkCbc(0, temp[1], &temp[2], len, temp_out, 500);
            Trace("zjp", "sdkPedInsideWkCbc len =%d\r\n", rslt);

            if (rslt >= 0) {
                rslt = SDK_OK;
            }
        }

        if (SDK_OK != rslt) {
            return SDK_ERR;
        }
        else {
            if (i + 1 != imax)             //takΪ8�ֽڵ�ʱ�������������
            {
                sdkPedWkDecryptData(ucTakIndex, temp_rslt, temp_out, 8);
                TraceHex("x919", "temp_out", temp_out, 8);
            }
            else {
                memcpy(temp_rslt, temp_out, 8);
            }
        }
    }

    memcpy(pheOut, temp_rslt, 8);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����mac
** Parameters:          s32 iKeyIndex tak���� :0-124
                               SDK_PED_MAC_MODE eMode: ecb ,cbc,x919
                               SDK_PED_DES_TYPE eKeyType:des����
                               const u8 *pheIn:���������ݳ��Ȳ���8λ��8λ
                               s32 iInLen:�������ݳ���
                               u8 *pheOut:8�ֽ�mac��
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDCalcMac(s32 iKeyIndex, SDK_PED_MAC_MODE eMode, SDK_PED_DES_TYPE eKeyType, const u8 *pheIn,
                  s32 iInLen, u8 *pheOut) {
    u8 takindex;

    // ��������Ϸ����ж�
    if (!(pheIn != NULL) || !(pheOut != NULL) || !(iKeyIndex >= 0 && iKeyIndex < 125) ||
        !(iInLen > 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    takindex = sdkPedGetFactWkIndex((u8) iKeyIndex, eKeyType);

// -----------------------------------ECB---------------------------------------
    if (eMode == SDK_PED_ECB) {
        return sdkPedE90(takindex, pheOut, pheIn, iInLen);
    }
// ----------------------------------CBC----------------------------------
    else if (eMode == SDK_PED_CBC) {
        return sdkPedCbc(takindex, pheOut, pheIn, iInLen, 0);
    }
// ----------------------------------X9.19----------------------------------
    else if (eMode == SDK_PED_X919) {
        if (eKeyType ==
            SDK_PED_DES_SINGLE) //���919�㷨���õ��ǵ�������Կ���e99�㷨һ��//taowz 2014.05.23 10:39
        {
            return sdkPedCbc(takindex, pheOut, pheIn, iInLen, 0);
        }
        else {
            return sdkPedCbc(takindex, pheOut, pheIn, iInLen, 1);
        }
        /*====================== END======================== */
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:         u8 ucTpkIndex ʵ������
                               u8 *pasPin ����ASC����
                               u8 *phePan
                               u8 *pheOut ���ܺ���
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
static s32 sdkPedE98(u8 ucTpkIndex, const u8 *pasPin, const u8 *phePan, u8 *pheOut) {
    u8 pin[9];
    u8 i;
    u8 pinblock[9] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0};

    i = (u8) strlen(pasPin);                                                   // ��ȡ����ĳ���

    if (i >= 4 && i <= 12)                                          // ���볤��Ϊ4-12λ
    {
        pinblock[0] = i;                                          // ��һ���ֽڱ�ʾ���볤��
        sdkAscToBcd(pinblock + 1, pasPin,
                    i);                                        // ��ASCII����ת����BCD

        if ((i % 2) != 0)                                            // ����
        {
            pinblock[(i / 2) + 1] |= 0x0F;                                        // ȷ��Ϊ0x0F
        }
        memset(pin, 0, sizeof(pin));
        sdkXOR8(pin, phePan, pinblock);                                        // ����PINBLOCK
    }
    else {
        return SDK_ERR;                                        // �����ʽ������
    }
    return sdkPedWkEncryptData(ucTpkIndex, pheOut, pin, 8);
}

#if 0 /*Modify by ���ƽ at 2015.03.12  10:28 */

/*****************************************************************************
** Descriptions:    �������������������
** Parameters:      u8 ucMode  ���ΪDUKPT��ģʽ SDK_PED_DUKPT_GET_PIN_MODE
                    const SDK_PED_PIN_CFG *pstPinCfg
                    u8 *pucOutData ���ܺ������
                    u8 *pucKsn ksn��ֵ
** Returned value:
                    ����0	�����ɹ���������KSN�ĳ��ȣ����DUKPT��
                    SDK_PED_PIN_FORMAT_ERR	�����ʽ��
                    SDK_TIME_OUT	���������Ӧ��ʱ(��λ: ms)
                    SDK_ERR	����ʧ��
                    SDK_ESC	�û�ȡ������
                    SDK_PED_NOPIN	����������
** Created By:		zhouzhihua  2013.08.15
** Remarks:
*****************************************************************************/
s32 sdkPEDInsideInputPIN(u8 ucMode, const SDK_PED_PIN_CFG * pstPinCfg, u8 * pucOutData, u8 * pucKsn)
{
    u8 temp[32];
    u8 pin[32];
    u8 pan[32];
    s32 rslt = 0;
    u8 pinlen;


    if( pstPinCfg == NULL || pucOutData == NULL || pucKsn == NULL || pstPinCfg->eKeyType != SDK_PED_DES_DUKPT )
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkSysIsHaveSpeaker())                                        // ����
    {
        sdkSysPlaySound(SDK_SYS_BEEP_INPUT_PIN, sdkSysGetVolume());     // ������ʾ��������//fusuipu 2013.08.15 14:36
    }
    memset(temp, 0, sizeof(temp));
    pinlen = (pstPinCfg->ePinMode == SDK_PED_MAG_PIN) ? 6 : 12;
    /*=======BEGIN: ����20121106 modify===========*/
    rslt = sdkKbGetScanf(pstPinCfg->iTimeOut, temp, 0, (s32)pinlen, SDK_MMI_NUMBER | SDK_MMI_PWD, pstPinCfg->ucRow);

    if (rslt == SDK_KEY_ESC)
    {
        return SDK_ESC;
    }

    if (rslt == SDK_TIME_OUT)
    {
        return SDK_TIME_OUT;
    }

    /*================ END================== */
    if (temp[0] > 0 && temp[0] < 4)                                        // ���볤��С��4
    {
        return SDK_PED_PIN_FORMAT_ERR;
    }
    else
    {
        if (!temp[0])
        {
            return SDK_PED_NOPIN;
        }
    }
    /*����pin*/
    memset(pin, 0xff, sizeof(pin));
    pin[0] = temp[0];                                          // ��һ���ֽڱ�ʾ���볤��
    sdkAscToBcd(pin + 1, &temp[1], temp[0]);                                        // ��ASCII����ת����BCD

    if ((temp[0] % 2) != 0)                                            // ����
    {
        pin[(temp[0] / 2) + 1] |= 0x0F;                                        // ȷ��Ϊ0x0F
    }
    TraceHex("dukpt", "pin", pin, 16); //06 12 34 56 FF FF FF FF FF FF FF FF FF FF FF FF

    /*����pan*/
    memset(pan, 0, sizeof(pan) );
    memcpy(&pan[2], &pstPinCfg->hePan[2], 6);  //6�ֽ�bcd�����˺���Ϣ
    TraceHex("dukpt", "pan", pan, 16);  //00 00 40 12 34 56 78 90 00 00 00 00 00 00 00 00

    /*�������*/
    memset(temp, 0, sizeof(temp));
    sdkXOR8(temp, pin, pan);
    TraceHex("dukpt", "temp", temp, 16); //06 12 74 44 CB A9 87 6F 00 00 00 00 00 00 00 00

    return sdkPedDukptDealPin(ucMode, SDK_PED_ENCRYPT, 0, temp, 8, pucOutData, pucKsn, &pinlen, 0);
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:	��������pbcAmt
** Parameters:          const SDK_PED_PIN_CFG *pstPinCfg ������������
                                pbcAmt 6�ֽ�bcd�������ʱΪNULL�����̲���ʾ���
                               u8 *pheOut  ����������ݣ���һ���ֽڳ��ȣ���߸�ʵ����Ҫ����
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDInputPIN(const SDK_PED_PIN_CFG *pstPinCfg, const u8 *pbcAmt, u8 *pheOut) {
    u8 temp[16], i;
    u8 tpkindex;
    s32 rslt;

    //��������Ϸ����ж�
    if (!(pstPinCfg != NULL) || !(pheOut != NULL) || !(pstPinCfg->ucTpkIndex < 125)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    tpkindex = sdkPedGetFactWkIndex(pstPinCfg->ucTpkIndex, pstPinCfg->eKeyType);

    if (sdkSysIsHaveSpeaker())                                        // ����
    {
        sdkSysPlaySound(SDK_SYS_BEEP_INPUT_PIN,
                        sdkSysGetVolume());                                        // ������ʾ��������//fusuipu 2013.08.15 14:36
    }
    {
        memset(temp, 0, sizeof(temp));
        i = (pstPinCfg->ePinMode == SDK_PED_MAG_PIN) ? 6 : 12;
        /*=======BEGIN: ����20121106 modify===========*/
        rslt = sdkKbGetScanf(pstPinCfg->iTimeOut, temp, 0, i, SDK_MMI_NUMBER | SDK_MMI_PWD,
                             pstPinCfg->ucRow);

        if (rslt == SDK_KEY_ESC) {
            return SDK_ESC;
        }

        if (rslt == SDK_TIME_OUT) {
            return SDK_TIME_OUT;
        }

        /*================ END================== */
        if (temp[0] > 0 && temp[0] < 4)                                        // ���볤��С��4
        {
            return SDK_PED_PIN_FORMAT_ERR;
        }
        else {
            if (!temp[0]) {
                return SDK_PED_NOPIN;
            }

            if (pstPinCfg->ePinMode == SDK_PED_IC_OFFLINE_PIN) {
                memcpy(pheOut, &temp[0], (temp[0] + 1));
            }
            else {
                pheOut[0] = 8;
                i = temp[0];
                temp[i + 1] = 0;
                //rslt = sdkPedE98(tpkindex, &temp[1], pstPinCfg->hePan, &pheOut[1]);
                sdkPedE98(tpkindex, &temp[1], pstPinCfg->hePan, &pheOut[1]);
            }
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�������������ʾ���׽��
** Parameters:          u8 *pucAmt
                               s32 len
                               u8 *lpOut
** Returned value:
** Created By:		fusuipu  2013.12.16
** Remarks:
*****************************************************************************/
static s32 sdkPedDispAmtWithoutPinpad(const u8 *pucAmt, s32 siLen, SDK_PED_SYMBOL eSymbol,
                                      u8 *lpOut) {
    u8 temp[50] = {0};
    u8 buf[50] = {0};
    u32 i;

    if (NULL == pucAmt || NULL == lpOut) {
        return SDK_PARA_ERR;
    }

    for (i = 0; i < (u32) siLen; i++) {
        if (0 != pucAmt[i]) {
            break;
        }
    }

    sdkBcdToAsc(temp, &pucAmt[i], (siLen - (s32) i));
    i = 0;

    while ('0' == temp[i]) {
        i++;
    }

    strcpy(buf, &temp[i]);
    memset(temp, 0, sizeof(temp));
    memcpy(temp, "0.00", 4);

    if (strlen(buf) == 1)                //ֻ������һλ
    {
        temp[0] = '0';
        temp[1] = '.';
        temp[2] = '0';
        temp[3] = buf[0];
    }
    else if (strlen(buf) == 2)           //ֻ��������λ
    {
        temp[0] = '0';
        temp[1] = '.';
        temp[2] = buf[0];
        temp[3] = buf[1];
    }
    else if (strlen(buf) > 2)            //�������ĳ��ȴ���2
    {
        for (i = 0; i < (u32)(strlen(buf) - 2); i++) {
            temp[i] = buf[i];
        }

        temp[i] = '.';
        i++;
        temp[i] = buf[i - 1];
        i++;
        temp[i] = buf[i - 1];
    }

    if (SDK_PED_NEGATIVE == eSymbol) {
        strcpy(lpOut, "-");
    }
    strcat(lpOut, temp);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ʾ���(����)
** Parameters:          const u8 *pbcAmt 6�ֽ�bcd
** Returned value:	SDK_OK�����������쳣
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDDispAmt(SDK_PED_SYMBOL eSymbol, const u8 *pbcAmt) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_PED_BALANCE_SYMBOL eSymbol:
                                SDK_PED_POSITIVE: ���Ϊ��ֵ
                                SDK_PED_NEGATIVE: ���Ϊ��ֵ

                               const u8 *pbcBalance:��6 �ֽ��Ҷ���ѹ��BCD�룩
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDDispBalance(SDK_PED_SYMBOL eSymbol, const u8 *pbcBalance) {
    //u8 temp[256] = {0};
    u8 temp[20] = {0};

    //u16 len = 0;
    //u16 uiCmd[5] = {0};
    //s32 rslt;
    //UARTCFG stUartCfg;
    //UARTCFG *stUartCfg;

    if (pbcBalance == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    //if(false == sdkPEDIsWithPinpad())
    {
        if (SDK_OK == sdkPedDispAmtWithoutPinpad(pbcBalance, 6, eSymbol, temp)) {
            sdkDispClearScreen();
            sdkDispFillRowRam(SDK_DISP_LINE1, 0, "���",
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
            sdkDispFillRowRam(SDK_DISP_LINE3, 0, temp,
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
#if MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P        /*Modify by ���ƽ at 2014.06.16  10:10 */
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȡ��          ȷ��",
                              SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
#else
            sdkDispFillRowRam(SDK_DISP_LINE5, 0, "ȷ��          ȡ��", SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
#endif         /* if MACHINEMODEL == SDK_SYS_MACHINE_G2 || MACHINEMODEL == SDK_SYS_MACHINE_200P */
            sdkDispBrushScreen();
            sdkKbWaitKey(SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, 0);
        }
        return SDK_OK;
    }
}

/*****************************************************************************
** Descriptions: �������ȡ��
** Parameters:          void
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
void sdkPEDCancel(void) {
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 iKeyIndex  ��Կ����
                                bool ucTmkOrWk    ����Կ0x01   ������Կ0x02
                               u8 ucCrypt ����0x41             ����0x42
                               u8 *pheIn  8byte
                               u8 *pheOut 8byte
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
static s32 sdkPedDesCrypt(u8 ucKeyIndex, u8 ucTmkOrWk, u8 ucCrypt, const u8 *pheIn, u8 *pheOut) {
    //u16 i;
    s32 rslt;
    u8 TmkOrWk;

    TmkOrWk = ucTmkOrWk == 0x01 ? POS_PCIMKDES : POS_PCIWKDES;

    if (TmkOrWk == POS_PCIMKDES) {
        //stUartCfg->stSendData.pheSendBuf[2] -= 0x41;
        //len = sdkPedInsideEdcryptData(stUartCfg->stSendData.pheSendBuf[2], TMK_INDEX_AREA, stUartCfg->stSendData.pheSendBuf[3], (u8*)&stUartCfg->stSendData.pheSendBuf[4], stUartCfg->stSendData.usLen - 4, stUartCfg->stRecvData.heRecvBuf, 500);
        if ((ucCrypt - 0x41) == SDK_PED_DECRYPT)        //����
        {
            rslt = ddi_innerkey_decrypt(TMK_INDEX_AREA, ucKeyIndex, 8, pheIn,
                                        pheOut);    //�����������0x42Э��
        }
        else //����SDK_PED_ENCRYPT
        {
            rslt = ddi_innerkey_encrypt(TMK_INDEX_AREA, ucKeyIndex, 8, pheIn,
                                        pheOut);    //�����������0x41Э��//taowz 2014.05.19 15:48
        }

        if (DDI_OK != rslt) {
            if (DDI_EINVAL == rslt) {
                return SDK_PARA_ERR;
            }
            return SDK_ERR;
        }
    }
    else if (TmkOrWk == POS_PCIWKDES) {
        //sdkPedInsideWkDecryptData(stUartCfg->stSendData.pheSendBuf[2], (u8*)&stUartCfg->stSendData.pheSendBuf[5], stUartCfg->stSendData.usLen - 5, stUartCfg->stRecvData.heRecvBuf, 500);
        //rslt = sdkPedInsideCheckKey(WK_INDEX_AREA, ucKeyIndex);
        rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, WK_INDEX_AREA, ucKeyIndex);

        if (DDI_OK != rslt) {
            if (DDI_EINVAL == rslt) {
                return SDK_PARA_ERR;
            }
            return SDK_ERR;
        }
        rslt = ddi_innerkey_decrypt(WK_INDEX_AREA, ucKeyIndex, 8, pheIn, pheOut);

        if (DDI_OK != rslt) {
            if (DDI_EINVAL == rslt) {
                return SDK_PARA_ERR;
            }
            return SDK_ERR;
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����Կ�ӽ�������
** Parameters:          s32 iTmkIndex
                               SDK_PED_DES_TYPE eTmkType
                               SDK_PED_CRYPT eCrypt
                               const u8 *pheIn
                               u8 *pheOut
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDTmkDes(s32 iTmkIndex, SDK_PED_DES_TYPE eTmkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn,
                 u8 *pheOut) {
    u8 tmkindex, crypt;

    if ((pheIn == NULL) || (pheOut == NULL) || !(iTmkIndex >= 0 && iTmkIndex < 100)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    tmkindex = sdkPedGetFactTmkIndex((u8) iTmkIndex, eTmkType);
    crypt = (eCrypt == SDK_PED_ENCRYPT) ? 0x41
                                        : 0x42;                                        //ext_cmd  0x41:���� 0x42:����
    return sdkPedDesCrypt(tmkindex, 0x01, crypt, pheIn, pheOut);
}

/*****************************************************************************
** Descriptions:	������Կ�ӽ���
** Parameters:          s32 iWkIndex
                               SDK_PED_DES_TYPE eWkType
                               SDK_PED_CRYPT eCrypt
                               const u8 *pheIn
                               u8 *pheOut
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDWkDes(s32 iWkIndex, SDK_PED_DES_TYPE eWkType, SDK_PED_CRYPT eCrypt, const u8 *pheIn,
                u8 *pheOut) {
    u8 wkindex;

    // ��������Ϸ����ж�
//    Assert(pheIn != NULL);
//    Assert(pheOut != NULL);
//    Assert(iWkIndex >= 0 && iWkIndex < 125);
    if (!(pheIn != NULL) || !(pheOut != NULL) || !(iWkIndex >= 0 && iWkIndex < 125)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    wkindex = sdkPedGetFactWkIndex((u8) iWkIndex, eWkType);

//    crypt = (eCrypt == SDK_PED_ENCRYPT) ? 0x41 : 0x42;   //ext_cmd  0x41:���� 0x42:����
//    return sdkPedDesCrypt(wkindex, 0x02, crypt, pheIn, pheOut);
    if (eCrypt == SDK_PED_ENCRYPT) {
        return sdkPedWkEncryptData(wkindex, pheOut, (u8 *) pheIn, 8);
    }
    else {
        return sdkPedWkDecryptData(wkindex, pheOut, (u8 *) pheIn, 8);
    }
}

/*****************************************************************************
** Descriptions:	��ȡ������̰汾
** Parameters:          u8 *pasVer ��ȡ���İ汾��
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDGetPedVersion(u8 *pasPedVer) {
    s32 rslt;

    if (NULL == pasPedVer) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_VER, 0, (u32) pasPedVer);
    Trace("devped", "ddi_innerkey_ioctl rslt = %d\r\n", rslt);

    if (DDI_OK == rslt) {
        TraceHex("devped", "version info", pasPedVer, 18);
        return strlen(pasPedVer);
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ������̻����
** Parameters:          u8 *pasSn
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDGetPedSn(u8 *pasSn) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��PED��ȡ�û��Զ�������
** Parameters:      u8 *pDataOut ��ȡ����ռ�
                    const s32 ucOutLen ��ȡ����ռ�����ֵ
** Returned value:
                    SDK_OK	ʵ�ʶ�ȡ���ݳ���
                    SDK_PARA_ERR	��������
                    SDK_ERR	ʧ��
                    SDK_ESC	�û�ȡ��
                    SDK_TIME_OUT	���������Ӧ��ʱ(��λ: ms)

** Created By:		ZXX  2013.05.08
** Remarks:
*****************************************************************************/
s32 sdkPEDReadUserData(u8 *pDataOut, const s32 ucOutLen) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	���������������
** Parameters:          SDK_PED_LANGUAGE eLanguage
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:
*****************************************************************************/
s32 sdkPEDSetLanguage(SDK_PED_LANGUAGE eLanguage) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��dukpt
** Parameters:          Input: None
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
s32 sdkPedDukptOpen() {
    s32 rtn = ddi_dukpt_open();

    if (DDI_OK == rtn) {
        gIsGetPin = 0;
        gIsOpenDukpt = 1;
        return SDK_OK;
    }
    else if (DDI_ERR == rtn) {
        return SDK_ERR;
    }
    else {
        return SDK_EBUSY;
    }
}

/*****************************************************************************
** Descriptions:	�ر�dukpt
** Parameters:          Input: None
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
s32 sdkPedDukptClose() {
    s32 rtn = ddi_dukpt_close();

    if (DDI_OK == rtn) {
        gIsGetPin = 0;
        gIsOpenDukpt = 0;
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	ָ����Կ��������
** Parameters:          u32 nKeyGroup����Կ��ţ�0���洢��0��1���洢��1
                    u32 nKeyIndex����Կ������
                    u32 nLen�����������ݳ��ȣ�����Ϊ8�ı���
                    const u8* lpIn������������
                    u8* lpOut�����ܽ�������س������������ݳ���һ��
                    u32 nMode�����ܷ�ʽ��0��PIN���ܣ�1��MAC����
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
static s32 sdkPedDukptEncrypt(u32 nKeyGroup, u32 nKeyIndex, u32 nLen, const u8 *lpIn, u8 *lpOut,
                              u32 nMode) {
    s32 rtn = DDI_ERR;

    if (gIsOpenDukpt == 0) {
        rtn = sdkPedDukptOpen();

        if (gIsOpenDukpt == 0) {
            return rtn;
        }
    }

    if (nKeyGroup > 1 || nKeyIndex > 9 || nLen % 8 != 0 || lpIn == NULL || lpOut == NULL ||
        nMode > 1) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rtn = ddi_dukpt_encrypt(nKeyGroup, nKeyIndex, nLen, lpIn, lpOut, nMode);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EBUSY == rtn) {
        return SDK_EBUSY;
    }
    else if (DDI_EINVAL == rtn) {
        return SDK_PARA_ERR;
    }
    else {
        return SDK_ERR;
    }
}

#if 0
/*****************************************************************************
** Descriptions:	ָ����Կ��������
** Parameters:          u32 nKeyGroup����Կ��ţ�0���洢��0��1���洢��1
                    u32 nKeyindex����Կ������
                    u32 nLen�����������ݳ��ȣ�����Ϊ8�ı���
                    const u8* lpIn������������
                    u8* lpOut�����ܽ�������س������������ݳ���һ��
                    u32 nMode�����ܷ�ʽ��0��PIN���ܣ�1��MAC����
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
static s32 sdkPedDukptDecrypt(u32 nKeyGroup, u32 nKeyindex, u32 nLen, const u8* lpIn, u8* lpOut, u32 nMode)
{
    s32 rtn = DDI_ERR;

    if( gIsOpenDukpt == 0 )
    {
        rtn = sdkPedDukptOpen();

        if( gIsOpenDukpt == 0 )
        {
            return rtn;
        }
    }

    if( nKeyGroup > 1 || nKeyindex > 9 || nLen % 8 != 0 || lpIn == NULL || lpOut == NULL || nMode > 1 )
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rtn = ddi_dukpt_decrypt(nKeyGroup, nKeyindex, nLen, lpIn, lpOut, nMode);

    if( DDI_OK == rtn )
    {
        return SDK_OK;
    }
    else if( DDI_EBUSY == rtn )
    {
        return SDK_EBUSY;
    }
    else if( DDI_EINVAL == rtn )
    {
        return SDK_PARA_ERR;
    }
    else
    {
        return SDK_ERR;
    }
}

#endif

/*****************************************************************************
** Descriptions:   �����ʼ����
** Parameters:
                   u8 ucIndex ����
                   u8 *pucKey ��ʼkey DK
                   u16 uiKeyLen  key����
                   u8 *pucKsn ksn
                   u16 uiKsnLen ksn�ĳ���
                   s32 siTimer  ��ʱʱ��
** Returned value:
                    ����0	�����ɹ�
                    SDK_ESC	�û�ȡ��
                    SDK_PARA_ERR	��������
                    SDK_ESC	�û�ȡ��
                    SDK_TIME_OUT	��ʱ�˳�
                    SDK_ERR	����ʧ��
** Created By:		zhouzhihua  2013.07.24
** Remarks:
*****************************************************************************/
s32 sdkPEDDukptLoadAndInit(u8 ucIndex, const u8 *pucKey, u16 uiKeyLen, const u8 *pucKsn,
                           u16 uiKsnLen, s32 siTimer) {
    u8 ucksn[20] = {0};     //ucbuf[512] = {0};
    s32 rslt; //, i;
    //u16 ucCmd[1];
    //UARTCFG stUartCfg;
    strDukptInitInfo dukptInitInfo;
    u8 ksnindex = 0;

    //siTimer  = (siTimer < 500) ? 500 : siTimer;

    if (pucKey == NULL || NULL == pucKsn || uiKsnLen > SDK_DUKPT_KSN_LEN) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(ucksn, 0xFF, sizeof(ucksn));

    if (uiKsnLen <= SDK_DUKPT_KSN_LEN) {
        memcpy(&ucksn[SDK_DUKPT_KSN_LEN - uiKsnLen], pucKsn, uiKsnLen);
    }

    if (uiKeyLen != SDK_DUKPT_KEY_LEN)                                        //Ŀǰkey�ĳ��ȱ�����16�ֽ�
    {
        return SDK_PARA_ERR;
    }

    if (gIsOpenDukpt == 0) {
        //rslt = sdk_dev_sdkPedDukptOpen();
        rslt = ddi_dukpt_open();

        if (DDI_OK == rslt) {
            gIsGetPin = 0;
            gIsOpenDukpt = 1;
            //return SDK_OK;
        }
        else {
            return SDK_ERR;
        }
    }
    dukptInitInfo.m_groupindex = 0;
    dukptInitInfo.m_keyindex = ucIndex;
    memcpy(dukptInitInfo.m_initkey, pucKey, SDK_DUKPT_KEY_LEN);
    dukptInitInfo.m_keylen = SDK_DUKPT_KEY_LEN;
    dukptInitInfo.m_ksnindex = ksnindex;
    memcpy(dukptInitInfo.m_initksn, ucksn, SDK_DUKPT_KSN_LEN);
    dukptInitInfo.m_ksnlen = SDK_DUKPT_KSN_LEN;

    rslt = ddi_dukpt_inject(&dukptInitInfo);

    if (DDI_OK == rslt) {
        Trace("dukpt", "dukpt���ܳɹ�");
        return SDK_OK;
    }
    else if (DDI_EBUSY == rslt) {
        Trace("dukpt", "dukpt�����豸æ����Դ��ռ��");
        return SDK_ERR;
    }
    else if (DDI_EINVAL == rslt) {
        Trace("dukpt", "dukpt���ܲ�������");
        return SDK_PARA_ERR;
    }
    else {
        Trace("dukpt", "dukpt����ʧ��");
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	KSN�Լ�(DUKPT��Կ����)
** Parameters:          u8* lpOut���汾��
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
static s32 sdkPedDukptKsnNext(u32 nKeyGroup, u32 nKeyIndex) {
    s32 rtn = DDI_ERR;

    if (gIsOpenDukpt == 0) {
        rtn = ddi_dukpt_open();

        if (DDI_OK == rtn) {
            gIsGetPin = 0;
            gIsOpenDukpt = 1;
        }
        else {
            return SDK_ERR;
        }
    }

    if (nKeyGroup > 1 || nKeyIndex > 9) {
        return SDK_PARA_ERR;
    }
    rtn = ddi_dukpt_ioctl(DDI_DUKPT_CTL_KSN_NEXT, nKeyGroup, nKeyIndex);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else if (DDI_EINVAL == rtn) {
        return SDK_PARA_ERR;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:	��ȡKSN
** Parameters:          u32 nKeyGroup����Կ��ţ�0���洢��0��1���洢��1
                    u32 nKeyindex����Կ������
                    u32 lpLen��KSN����
                    u8* lpOut��KSN
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
static s32 sdkPedDukptGetKsn(u32 nKeyGroup, u32 nKeyIndex, u32 *lpLen, u8 *lpOut) {
    s32 rtn = DDI_ERR;

    if (gIsOpenDukpt == 0) {
        rtn = ddi_dukpt_open();

        if (DDI_OK == rtn) {
            gIsGetPin = 0;
            gIsOpenDukpt = 1;
        }
        else {
            return SDK_ERR;
        }
    }

    if (nKeyGroup > 1 || nKeyIndex > 9 || lpLen == NULL || lpOut == NULL) {
        return SDK_PARA_ERR;
    }
    rtn = ddi_dukpt_getksn(nKeyGroup, nKeyIndex, lpLen, lpOut);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EBUSY == rtn) {
        return SDK_EBUSY;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:        dukpt�ӽ���pin
** Parameters:          u8 ucMode
                               u8 *upPan ����������
                               u8 *upPwdLen���������ݳ���
                               u8 *upPinBlock���
                               u8 uKsnIndx ����
                               u8 *upKsn ����ksn
                               u8 *upKsnLen ����ksn����
                               s32 siTimerOut 0
** Returned value:
** Created By:		fusuipu  2013.08.13
** Remarks:
*****************************************************************************/
static s32 sdkPedDukptDealPin(u8 ucMode, SDK_PED_CRYPT eCrypt, u8 uKsnIndx, u8 *upInData,
                              u32 upDataLen, u8 *upOutData, u8 *upKsn, u8 *upKsnLen,
                              s32 siTimerOut) {
    s32 rtn = DDI_ERR;

    if (gIsOpenDukpt == 0) {
        rtn = ddi_dukpt_open();

        if (DDI_OK == rtn) {
            gIsGetPin = 0;
            gIsOpenDukpt = 1;
        }
        else {
            return SDK_ERR;
        }
    }

    if (uKsnIndx > 9 || upInData == NULL || upDataLen % 8 != 0 || upOutData == NULL ||
        upKsn == NULL || upKsnLen == NULL) {
        Trace("dukpt", "sdk_dev_ped_dukpt_deal_pin��������");
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (eCrypt == SDK_PED_ENCRYPT) {
        rtn = sdkPedDukptKsnNext(0, uKsnIndx);  //ksn �Լ�

        if (SDK_OK != rtn) {
            Trace("dukpt", "�Լ�ksnʧ��");
            Assert(0);
            return rtn; //�Լ�ksnʧ��
        }
        rtn = ddi_dukpt_encrypt(0, uKsnIndx, upDataLen, upInData, upOutData, 0);  //����

        if (DDI_OK == rtn) {
            u32 tmpKsnLen = 0;

            Trace("dukpt", "KSN���ܳɹ�");
            gIsGetPin = 1;
            rtn = sdkPedDukptGetKsn(0, uKsnIndx, &tmpKsnLen, upKsn);  //��ȡ����ʹ�õ�ksn
            *upKsnLen = (u8) tmpKsnLen;
            return rtn;
        }
        else if (DDI_EBUSY == rtn) {
            Trace("dukpt", "�豸æ����Դ��ռ��");
            Assert(0);
            return SDK_EBUSY;
        }
        else {
            Trace("dukpt", "����ʧ��");
            Assert(0);
            return SDK_ERR;
        }
    }
    else {
        rtn = ddi_dukpt_decrypt(0, uKsnIndx, upDataLen, upInData, upOutData, 0);  //����

        if (DDI_OK == rtn) {
            Trace("dukpt", "KSN���ܳɹ�");
            return SDK_OK; //��ȡ����ʹ�õ�ksn
        }
        else if (DDI_EBUSY == rtn) {
            Trace("dukpt", "����:�豸æ����Դ��ռ��");
            Assert(0);
            return SDK_EBUSY;
        }
        else {
            Trace("dukpt", "����ʧ��");
            Assert(0);
            return SDK_ERR;
        }
    }
}

/*****************************************************************************
** Descriptions:

** Parameters:              u8 ucMode 00 ����ģʽ��0x01 ����ģʽ
                        u8 ucIndex ����
                        SDK_PED_PIN_CFG * pstPinCfg ���ܷ�ʽ
                        u16 uiMinLen ������С����
                        u16 uiMaxLen ��������
                        u8 *pucPinBlock ���ܺ������
                        u8 *pucKsnLen KSN ���ȣ��̶�Ϊ10
                        s32 siTimer ��ʱ��
** Returned value:	����0 ksn�ĳ��ȣ�С�ڵ���0 ʧ��
** Created By:		zhouzhihua  2013.07.29
** Remarks:             Ŀǰ��������ʺż���
*****************************************************************************/
s32 sdkPEDDukptGetPin(u8 ucMode, u8 ucIndex, const SDK_PED_PIN_CFG *pstPinCfg, u8 uiMinLen,
                      u8 uiMaxLen, u8 *pucPinBlock, u8 *pucKsn, u8 *pucKsnLen, s32 siTimer) {
    u8 temp[32];
    u8 pin[32];
    u8 pan[32];
    s32 rslt = 0;
    u8 pinlen;


    if (pstPinCfg == NULL || pucPinBlock == NULL || pucKsn == NULL ||
        pstPinCfg->eKeyType != SDK_PED_DES_DUKPT) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkSysIsHaveSpeaker())                                        // ����
    {
        sdkSysPlaySound(SDK_SYS_BEEP_INPUT_PIN,
                        sdkSysGetVolume());                                        // ������ʾ��������//fusuipu 2013.08.15 14:36
    }
    memset(temp, 0, sizeof(temp));
    pinlen = (pstPinCfg->ePinMode == SDK_PED_MAG_PIN) ? 6 : 12;
    /*=======BEGIN: ����20121106 modify===========*/
    rslt = sdkKbGetScanf(pstPinCfg->iTimeOut, temp, 0, (s32) pinlen, SDK_MMI_NUMBER | SDK_MMI_PWD,
                         pstPinCfg->ucRow);

    if (rslt == SDK_KEY_ESC) {
        return SDK_ESC;
    }

    if (rslt == SDK_TIME_OUT) {
        return SDK_TIME_OUT;
    }

    /*================ END================== */
    if (temp[0] > 0 && temp[0] < 4)                                        // ���볤��С��4
    {
        return SDK_PED_PIN_FORMAT_ERR;
    }
    else {
        if (!temp[0]) {
            return SDK_PED_NOPIN;
        }
    }
    /*����pin*/
    memset(pin, 0xff, sizeof(pin));
    pin[0] = temp[0];                                          // ��һ���ֽڱ�ʾ���볤��
    sdkAscToBcd(pin + 1, &temp[1],
                temp[0]);                                        // ��ASCII����ת����BCD

    if ((temp[0] % 2) != 0)                                            // ����
    {
        pin[(temp[0] / 2) + 1] |= 0x0F;                                        // ȷ��Ϊ0x0F
    }
    TraceHex("dukpt", "pin", pin, 16);

    /*����pan*/
    memset(pan, 0, sizeof(pan));
    memcpy(&pan[2], &pstPinCfg->hePan[2], 6);  //6�ֽ�bcd�����˺���Ϣ
    TraceHex("dukpt", "pan", pan, 16);

    /*�������*/
    memset(temp, 0, sizeof(temp));
    sdkXOR8(temp, pin, pan);
    TraceHex("dukpt", "temp", temp, 16);

    return sdkPedDukptDealPin(ucMode, SDK_PED_ENCRYPT, 0, temp, 8, pucPinBlock, pucKsn, &pinlen, 0);


#if 0 /*Modify by ���ƽ at 2015.03.13  9:22 */
    u8 *ucbuf;
    s32 rslt, i, len = 0;
    u16 ucCmd[1];
    UARTCFG stUartCfg;

    if( (uiMinLen < 4) || (uiMaxLen > 12) || (uiMinLen > uiMaxLen) || (NULL == pucPinBlock) || (pucKsn == NULL))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if( siTimer < 1000 )
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    siTimer -= 500;                                       //ȥ��500ms

    ucbuf = (u8 *)sdkGetMem(1024);

    if( NULL == ucbuf )
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(ucbuf, 0, 1024);
    i = sdkPedFormDCReserved(1, DC_CMD_CTRL_RES, DC_CMD_EX_DUKPT_GET_PIN, ucbuf);

    if( i < 0 )
    {
        if( NULL != ucbuf )
        {
            sdkFreeMem(ucbuf);
        }
        Assert(0);
        return SDK_PARA_ERR;
    }
    ucbuf[i++] = ucMode;
    ucbuf[i++] = ucIndex;
    ucbuf[i++] = (u8)uiMinLen;
    ucbuf[i++] = (u8)uiMaxLen;

    if( pucPan == NULL )                                        //�����Ҫ�������ʺŵļ��ܿ�����˴���
    {
        memset(&ucbuf[i], 0, 6);
    }
    else
    {
        memcpy(&ucbuf[i], pucPan, 6);
    }
    i += 6;
    ucbuf[i++] = (u8)(((u32)siTimer & 0xFF000000) >> 24);
    ucbuf[i++] = (u8)((siTimer & 0x00FF0000) >> 16);
    ucbuf[i++] = (u8)((siTimer & 0x0000FF00) >> 8);
    ucbuf[i++] = (u8)((siTimer & 0x000000FF));
    ucCmd[0] = DC_CMD_EX_DUKPT_GET_PIN;

    TraceHex("zhouzhihua", "ucbuf  ", ucbuf, i);

    sSetUartCfg(&stUartCfg, POS_DC_COMMAND, ucbuf, (u16)i, ucCmd, 1, siTimer);
    rslt = sSendDataToPed(&stUartCfg);

    //�õ���������������1Byte + ��������(nBytes)+cmd(2Bytes)+ret(1Byte)+����
    //data ����pinblock����(1Byte)+pinblock����(nByte)+ksn����(1Byte)+ksn����(nByte)
    if(rslt == SDK_OK)
    {
        memset(ucbuf, 0, 1024);
        rslt = SDK_ERR;
        i = 1 + stUartCfg.stRecvData.heRecvBuf[0] + 2;                                        //���ؽ����λ��

        if( stUartCfg.stRecvData.heRecvBuf[i] == 0x00 )
        {
            len = stUartCfg.stRecvData.heRecvBuf[i + 1];                                        //pinblock ����
            memcpy(pucPinBlock, &stUartCfg.stRecvData.heRecvBuf[i + 1 + 1], 8);
            memcpy(pucKsn, &stUartCfg.stRecvData.heRecvBuf[i + 1 + 1 + len + 1], SDK_DUKPT_KSN_LEN);
            TraceHex("zhouzhihua", "pucKsn  ", pucKsn, 10);
            TraceHex("zhouzhihua", "pucPinBlock  ", pucPinBlock, 8);

            if( NULL != ucbuf )
            {
                sdkFreeMem(ucbuf);
            }
            return SDK_DUKPT_KSN_LEN;
        }
    }

    if( NULL != ucbuf )
    {
        sdkFreeMem(ucbuf);
    }
    return rslt;

#endif /* if 0 */
}

static s32 sdkDukptE90S(u8 nGroup, u8 uKeyIndex, u8 *pheDest, const u8 *pheSrc, s32 siLen) {
    u8 mac[128];
    u8 mac2[128];
    u32 rtn;

    if (gIsOpenDukpt == 0) {
        rtn = sdkPedDukptOpen();

        if (gIsOpenDukpt == 0) {
            return rtn;
        }
    }
    memset(mac, 0, 128);
    memset(mac2, 0, 128);
    TraceHex("zjp", "pheSrc", pheSrc, siLen);
    sdkEcb(mac, pheSrc, siLen);

    TraceHex("zjp", "mac", mac, 8);

    rtn = sdkPedDukptEncrypt(nGroup, uKeyIndex, 8, mac, mac2, 1);  //����

    if (SDK_OK != rtn) {
        Trace("dukpt", "����ʧ��");
        Assert(0);
        return SDK_ERR;
    }
    sdkXOR8(mac, mac2, &mac[8]);
    memset(mac2, 0, 128);
    rtn = sdkPedDukptEncrypt(nGroup, uKeyIndex, 8, mac, mac2, 1);  //����

    if (SDK_OK != rtn) {
        Trace("dukpt", "����ʧ��");
        Assert(0);
        return SDK_ERR;
    }
    sdkBcdToAsc(pheDest, mac2, 4);
    return rtn;
}

/*****************************************************************************
** Descriptions:	��ȡdukpt�汾��
** Parameters:          u8* lpOut���汾��
** Returned value:
** Created By:		���ƽ  2014.08.26
** Remarks:
*****************************************************************************/
s32 sdkPedDukptGetVersion(u8 *lpOut) {
    s32 rtn = DDI_ERR;

    if (gIsOpenDukpt == 0) {
        rtn = sdkPedDukptOpen();

        if (gIsOpenDukpt == 0) {
            return rtn;
        }
    }

    if (lpOut == NULL) {
        return SDK_PARA_ERR;
    }
    rtn = ddi_dukpt_ioctl(DDI_DUKPT_CTL_VER, 0, (u32) lpOut);

    if (DDI_OK == rtn) {
        return SDK_OK;
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*****************************************************************************
** Descriptions:    DUKPT��ȡmac
** Parameters:          u8 ucMode 0x00 ��key �� 0x01��key
                               u8 ucIndex  ����
                               u8 *pucInData ���ݱ���Ϊ ASCII
                               u16 uiDataLen ���ݳ���
                               u8 *pucMac �����mac
                               u8 *pucKsn
                               s32 siTimer
** Returned value:	����0 ksn�ĳ��� ��ȷ��С��0:ʧ�ܣ�ʧ�ܣ���ʱ���û�ȡ������������
** Created By:		zhouzhihua  2013.07.30
** Remarks:
*****************************************************************************/
s32 sdkPEDDukptGetMac(u8 ucMode, u8 ucIndex, const u8 *puascInData, u16 uiDataLen, u8 *pucMac,
                      u8 *pucKsn, u8 *pucKsnLen, s32 siTimer) {
    //u8 *ucbuf;
    //u8 ksnlen;
    s32 rtn; //, sitimer = 0, i, len = 0;
    u32 tmpKsnLen = 0;

    if ((NULL == puascInData) || (NULL == pucMac) || (NULL == pucKsn)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (uiDataLen > 1024) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    //sitimer = 0;
    //ksnlen  = 0;

    if (gIsOpenDukpt == 0) {
        rtn = sdkPedDukptOpen();

        if (gIsOpenDukpt == 0) {
            return rtn;
        }
    }
    Trace("dukpt", "*upDukptMode = %d\r\n", ucMode);
    Trace("dukpt", "gIsGetPin = %d\r\n", gIsGetPin);

    if (!(ucMode == 0 && gIsGetPin == 1)) //ʹ����key�� ��Ҫ��ksn�����Լ�
    {
        //sdk_dev_ped_dukpt_getksn( 0, ucIndex, &tmpKsnLen, pucKsn );
        rtn = ddi_dukpt_getksn(0, ucIndex, &tmpKsnLen, pucKsn);

        if (DDI_OK != rtn) {
            return SDK_ERR;
        }
        //ksnlen = (u8)tmpKsnLen;
        //TraceHex("dukpt", "oldKsn", pucKsn, ksnlen);

        //rtn = sdk_dev_ped_dukpt_KSN_next( 0, ucIndex );//ksn �Լ�
        rtn = ddi_dukpt_ioctl(DDI_DUKPT_CTL_KSN_NEXT, 0, ucIndex);

        if (DDI_OK != rtn) {
            return SDK_ERR;
        }
        //sdk_dev_ped_dukpt_getksn( 0, ucIndex, &tmpKsnLen, pucKsn );
        rtn = ddi_dukpt_getksn(0, ucIndex, &tmpKsnLen, pucKsn);
        //ksnlen = (u8)tmpKsnLen;
        //TraceHex("dukpt", "newKsn", pucKsn, ksnlen);

        if (DDI_OK != rtn) {
            Trace("dukpt", "�Լ�ksnʧ��");
            Assert(0);
            return rtn; //�Լ�ksnʧ��
        }
        gIsGetPin = 0;
    }
    TraceHex("dukpt", "upData", puascInData, uiDataLen);
    rtn = sdkDukptE90S(0, ucIndex, pucMac, puascInData, uiDataLen);
    TraceHex("dukpt", "sdk_dev_dukpt_E90S->upMac", pucMac, 8);

    if (SDK_OK == rtn) {
        Trace("dukpt", "KSN���ܳɹ�");
        //rtn = sdk_dev_ped_dukpt_getksn( 0, ucIndex, &tmpKsnLen, pucKsn );//��ȡ����ʹ�õ�ksn
        rtn = ddi_dukpt_getksn(0, ucIndex, &tmpKsnLen, pucKsn);

        if (DDI_OK == rtn) {
            rtn = SDK_OK;
        }
        else {
            rtn = SDK_ERR;
        }
        //ksnlen = (u8)tmpKsnLen;
        return rtn;
    }
    else {
        Trace("dukpt", "����ʧ��");
        Assert(0);
        return SDK_ERR;
    }
}

#if 0
/*****************************************************************************
** Descriptions:	 ���� �������������
** Parameters:          u16 usCmd����������
                               u8 *pucData��������
                               u16 usDataLen���ݳ���
** Returned value:
** Created By:		lqq2012.11.25
** Remarks:             //����������չʹ�ã�ֻ��sPinpadSend��sPinpadRecv�����fifo
*****************************************************************************/
static s32 sdkPedSendEx(u16 usCmd, const u8 * pucSend, u16 usSendLen)
{
    return SDK_ERR;
}

#endif

/*****************************************************************************
** Descriptions:   ������Կ������Կ�����ݼ���
** Parameters:     u8 uccrypt, 0-���� ��1-����
                   u32 nKeyArea, 0-����Կ�������� 1-������Կ������
                   s32 siWkIndex ��Կ����
                   u8 *pucData Ҫ��������(����ʵ�ʳ����������)
                   s32 siDataLen Ҫ�������ݳ���   ������8�ı��������������Կ��������8��16
                   u8 *pucOutData ���ܺ�����
                   s32 siTimer
** Returned value: ���ܺ����ݳ���
** Created By:
** Remarks:
*****************************************************************************/
static s32 sdkPedInsideEdcryptData(u8 uccrypt, u32 nKeyArea, s32 siWkIndex, const u8 *pucData,
                                   s32 siDataLen, u8 *pucOutData, s32 siTimer) {
    s32 rtn;

    if (pucData == NULL || pucOutData == NULL || siDataLen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    //rtn = sdkPedInsideCheckKey(nKeyArea, siWkIndex);
    rtn = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, nKeyArea, siWkIndex);
    Trace("zjp", "sdkPedInsideCheckKey rtn = %d \r\n", rtn);

    if (DDI_OK != rtn) {
        if (DDI_EINVAL == rtn) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }

    //pucOutData[0] = uccrypt;
    if (uccrypt == SDK_PED_DECRYPT) //����
    {
        rtn = ddi_innerkey_decrypt(nKeyArea, siWkIndex, siDataLen, pucData,
                                   pucOutData); //�����������0x42Э��
    }
    else //����SDK_PED_ENCRYPT
    {
        rtn = ddi_innerkey_encrypt(nKeyArea, siWkIndex, siDataLen, pucData,
                                   pucOutData); //�����������0x41Э��//taowz 2014.05.19 15:48
    }
    Trace("zjp", "sdkPedInsideEdcryptData rtn = %d \r\n", rtn);

    if (DDI_OK != rtn) {
        if (DDI_EINVAL == rtn) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return siDataLen;  // + 2; //�����������0x41&0x42Э��
}

/*****************************************************************************
** Descriptions:	˫������8�ֽڼӽ���
** Parameters:
** Returned value:
** Created By:		taowz  2014.05.15
** Remarks:
*****************************************************************************/
static s32 sdkPedInsideHbyteEdcryptData(s32 nCmd, s32 nKeyArea, s32 nIndex, s32 nHalf, s32 siInlen,
                                        const u8 *pucInData, u8 *pucOutData) {
    s32 ret;
    strInnerHKeyDEncryptPara stcrypt;

    if (NULL == pucOutData || NULL == pucOutData || siInlen < 0 || nIndex < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (!(nCmd == SDK_PED_ENCRYPT || nCmd == SDK_PED_DECRYPT)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((nKeyArea == TMK_INDEX_AREA && (nIndex < 102 || nIndex > 203))  //��Կ������˫�����ģ������
        || (nKeyArea == WK_INDEX_AREA && (nIndex < 128 || nIndex > 255))) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_KEY_CHECK, nKeyArea, nIndex);
    Trace("devped", "ddi_innerkey_ioctl rslt = %d \r\n", ret);

    if (DDI_OK != ret) {
        if (DDI_EINVAL == ret) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    stcrypt.m_area = nKeyArea;
    stcrypt.m_index = nIndex;
    stcrypt.m_half = nHalf;
    stcrypt.m_indata = pucInData;
    stcrypt.m_outdata = pucOutData;
    stcrypt.m_len = siInlen;


    nCmd = nCmd == SDK_PED_ENCRYPT ? DDI_INNERKEY_CTL_HKEY_ENCRYPT : DDI_INNERKEY_CTL_HKEY_DECRYPT;
    ret = ddi_innerkey_ioctl(nCmd, (u32) & stcrypt, 0);
    Trace("devped", "ddi_innerkey_ioctl rslt = %d \r\n", ret);

    if (DDI_OK != ret) {
        if (DDI_EINVAL == ret) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 iIndex
                               SDK_PED_KEY_TYPE eKeyType
                               SDK_PED_DES_TYPE eKeyDesType
                               SDK_PED_CRYPT eCrypt
                               SDK_PED_CRYPT_WAY eCryptWay
                               const u8 *pheIn
                               u16 usInLen
                               u8 *pheOut
** Returned value:
** Created By:		fusuipu  2013.08.02
** Remarks:
*****************************************************************************/
s32 sdkPEDDesEx(s32 iIndex, SDK_PED_KEY_TYPE eKeyType, SDK_PED_DES_TYPE eKeyDesType,
                SDK_PED_CRYPT eCrypt, SDK_PED_CRYPT_WAY eCryptWay, const u8 *pheIn, u16 usInLen,
                u8 *pheOut) {
    //u8 destype = 0;
    //u8 *temp;
    //u16 cmd[5] = {0};
    //s32 rslt;
    s32 len = 0;
    s32 nCmd;
    s32 nKeyArea;
    s32 nHalf;
    u8 uccrypt;
    u8 factindex = 0;

    if ((pheIn == NULL) || pheOut == NULL || usInLen > 512 || usInLen == 0 || usInLen % 8 != 0
        || (eKeyDesType == SDK_PED_DES_SINGLE &&
            (eCryptWay == SDK_PED_DES_MIDDLE8 || eCryptWay == SDK_PED_DES_LAST8 ||
             eCryptWay == SDK_PED_DES_TOP8))
        || (eKeyType == SDK_PED_TMK && iIndex >= 100)
        || (eKeyType == SDK_PED_WK && iIndex >= 125)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (eKeyType == SDK_PED_TMK) {
        nKeyArea = 0; //TMK_INDEX_AREA����Կ��
        factindex = sdkPedGetFactTmkIndex((u8) iIndex, eKeyDesType);
    }
    else if (eKeyType == SDK_PED_WK) {
        nKeyArea = 1; //WK_INDEX_AREA������Կ��
        factindex = sdkPedGetFactWkIndex((u8) iIndex, eKeyDesType);
    }
    else {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (eCrypt == SDK_PED_ENCRYPT) {
        uccrypt = SDK_PED_ENCRYPT; //����
        nCmd = SDK_PED_ENCRYPT;
    }
    else {
        uccrypt = SDK_PED_DECRYPT; //����
        nCmd = SDK_PED_DECRYPT;
    }

    switch (eCryptWay) {
        case SDK_PED_DES_ALL: {
            len = sdkPedInsideEdcryptData(uccrypt, nKeyArea, factindex, pheIn, usInLen, pheOut, 0);

            if (len > 0 && len <= 512) {
                return SDK_OK;
            }
            return len;
        }

        case SDK_PED_DES_TOP8: {
            nHalf = 0;
            break;
        }

        case SDK_PED_DES_MIDDLE8: {
            nHalf = 1;
            break;
        }

        case SDK_PED_DES_LAST8: {
            return SDK_ERR; //������24B��Կ��Ч��ddi�ӿڲ�֧��
        }

        default: {
            return SDK_PARA_ERR; //������24B��Կ��Ч��ddi�ӿڲ�֧��
        }
    }

    return sdkPedInsideHbyteEdcryptData(nCmd, nKeyArea, factindex, nHalf, usInLen, pheIn, pheOut);
}

/*****************************************************************************
** Descriptions:        �������滻����Կ( �¾���Կ����һ��Ϊ�滻)
** Parameters:          1. iOldTmkIndex: ������Կ������(��������Կ)0-99
                        2. eOldKeyType: ������Կ����(��������Կ)
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]
** Parameters:          3. iNewTmkIndex: �洢������Կ������0-99
                        4. eNewKeyType: ������Կ����
                                SDK_PED_DES_TYPE:DES��Կ	   [8�ֽ�]
                                SDK_PED_DES_TRIPLE:3DES��Կ   [16�ֽ�]
                        5. pheKey: ����Կ��������
                        6. iTimeout: ��ʱʱ�� ������100ms
** Returned value:
                        SDK_OK: �ɹ�
                        SDK_PARA_ERR: ��������
                        SDK_TIME_OUT: ���������Ӧ��ʱ(��λ: ms)
** Created By:	        ���ƽ 2014-08-08
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateTmkEx(s32 iOldTmkIndex, SDK_PED_DES_TYPE eOldKeyType, s32 iNewTmkIndex,
                      SDK_PED_DES_TYPE eNewKeyType, const u8 *pheKey, s32 iTimeout) {
    s32 rslt;
    u8 temp[64] = {0};
    u8 nindex;
    u8 nkeyLen = 8;

    if (pheKey == NULL || iOldTmkIndex >= 100 || iNewTmkIndex >= 100 || iTimeout < 100) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (SDK_PED_DES_TRIPLE == eNewKeyType) {
        nkeyLen = 16;
    }
    rslt = sdkPEDDesEx(iOldTmkIndex, SDK_PED_TMK, eOldKeyType, SDK_PED_DECRYPT, SDK_PED_DES_ALL,
                       pheKey, nkeyLen, temp);

    if (rslt != SDK_OK) {
        return rslt;
    }
    nindex = sdkPedGetFactTmkIndex((u8) iNewTmkIndex, eNewKeyType);

    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_TKEY_INJECT, TMK_INDEX_AREA, nindex);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_PED_WK_CFG *pstWkCfg
                               SDK_PED_CRYPT_WAY eCryptWay
** Returned value:
** Created By:		fusuipu  2013.08.02
** Remarks:
*****************************************************************************/
s32 sdkPEDUpdateWkEx(const SDK_PED_WK_CFG *pstWkCfg, SDK_PED_CRYPT_WAY eCryptWay) {
    s32 rslt;
    u8 temp[64] = {0};
    u8 nindex;

    if (pstWkCfg == NULL || pstWkCfg->ucTmkIndex >= 100 || pstWkCfg->ucWkIndex >= 125
        || pstWkCfg->ucEnWkLen == 0 || pstWkCfg->ucEnWkLen > 24 || pstWkCfg->ucEnWkLen % 8 != 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rslt = sdkPEDDesEx(pstWkCfg->ucTmkIndex, SDK_PED_TMK, pstWkCfg->eTmkType, SDK_PED_DECRYPT,
                       eCryptWay, pstWkCfg->heEnWk, pstWkCfg->ucEnWkLen, temp);

    if (rslt != SDK_OK) {
        return rslt;
    }
    nindex = sdkPedGetFactWkIndex((u8) pstWkCfg->ucWkIndex, pstWkCfg->eWkType);

    rslt = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_TKEY_INJECT, WK_INDEX_AREA, nindex);

    if (DDI_OK != rslt) {
        if (DDI_EINVAL == rslt) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����������̴��� ͨ��rate
** Parameters:          s32 siRate  ֻ֧�� 38400 �� 9600
** Returned value:
** Created By:		shiweisong  2013.10.23
** Remarks:             �����ñ��� ͨ��rate ���� �����̵�
*****************************************************************************/
s32 Private_sdkPEDSetUartBaudRate(s32 siRate) {
    return SDK_ERR;
}

/*=======BEGIN: taowz 2014.05.23  17:23 modify===========*/
/*****************************************************************************
** Descriptions:	��ʱ��Կ�ӽ���
** Parameters:                 SDK_PED_CRYPT eCrypt
                               const u8 *pheIn
                               s32 siInlen������8�ı���
                               u8 *pheOut
** Returned value:
** Created By:		zxx2012.11.26
** Remarks:         �������ȵ�������Կ���ܽӿڽ��ܣ����ܽ�����ŵ���ʱ��Կ����
*****************************************************************************/
s32 sdkPEDTemporaryKeyDes(SDK_PED_CRYPT eCrypt, const u8 *pheIn, s32 siInlen, u8 *pheOut) {
    s32 ret;
    strInnerTKeyDEncryptPara stcrypt;

    // ��������Ϸ����ж�
    if (pheIn == NULL || pheOut == NULL || siInlen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    stcrypt.m_indata = pheIn;
    stcrypt.m_outdata = pheOut;
    stcrypt.m_len = siInlen;
    ret = ddi_innerkey_ioctl(eCrypt, (u32) & stcrypt, 0);

    if (DDI_OK != ret) {
        if (DDI_EINVAL == ret) {
            return SDK_PARA_ERR;
        }
        return SDK_ERR;
    }
    return SDK_OK;
}

s32 sdkPEDDisplayStr(const u8 *pasStr, s32 ucRow, s32 ucCol, u32 ucAtr) {
    if (NULL == pasStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispClearScreen();

    /*=======BEGIN: fusuipu 2013.12.19  14:13 modify ������ʾ��ʽ�����ñ���һ�£�0Ϊ���ԣ�1Ϊ����===========*/
    if (1 == ucAtr)                      //1Ϊ����
    {
        ucAtr &= ~SDK_DISP_FDISP;
        ucAtr |= SDK_DISP_NOFDISP;
    }
    else                                //0Ϊ���ԣ�Ĭ������
    {
        ucAtr |= SDK_DISP_FDISP;
    }
    /*====================== END======================== */
    sdkDispFillRowRam(ucRow, ucCol, pasStr, ucAtr);
    sdkDispBrushScreen(); //��ʾ
    sdkmSleep(3000);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ������������ַ���
** Parameters:          SDK_PED_INPUT_STR eInputMode    �����ַ���ģʽ
                    u8 *pOut                        ��ȡ��������������ַ���
                    s32 siTimeOut                   ��ʱʱ��
                    void *pVar                      Ԥ��ָ�����
** Returned value:
** Created By:		fusuipu  2014.11.17
** Remarks:             SDK_PED_INPUT_PHONE     �����ֻ���,ģʽ���������Ƚ��������ƣ�ֻҪ�û����������λ��С�ڵ���11λ�����������
                    SDK_PED_INPUT_CHECK_NUM ��Data�����ʾ��������ʾ��LCD��һ�У�Ȼ����밴������״̬���ȴ��û����룬�û�����ȷ�ϡ�����pinpad��������
*****************************************************************************/
s32 sdkPEDGetInputStr(SDK_PED_INPUT_STR eInputMode, u8 *pOut, s32 siTimeOut, void *pVar) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��������Կ�豸
** Parameters:
** Returned value:
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkPedInsideOpen(void) {
    s32 rslt;

    rslt = ddi_innerkey_open();

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	�ر�������Կ�豸
** Parameters:
** Returned value:
** Created By:
** Remarks:
*****************************************************************************/
s32 sdkPedInsideClose(void) {
    s32 rslt;

    rslt = ddi_innerkey_close();

    if (DDI_OK == rslt) {
        return SDK_OK;
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	���������Կ�豸�汾
** Parameters:          u8 *pucOutData ��õİ汾��
** Returned value:
** Created By:		taowz  2014.05.13
** Remarks:
*****************************************************************************/
s32 sdkPedInsideVersion(u8 *pucOutData) {
    s32 rtn;

    if (NULL == pucOutData) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    rtn = ddi_innerkey_ioctl(DDI_INNERKEY_CTL_VER, 0, (u32) pucOutData);

    if (DDI_OK == rtn) {
        TraceHex("devped", "version info", pucOutData, 18);
        return strlen(pucOutData);
    }
    else if (DDI_EIO == rtn) {
        return SDK_EIO;
    }
    else {
        return SDK_ERR;
    }
}

/*====================== END======================== */

