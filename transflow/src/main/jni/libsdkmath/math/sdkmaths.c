#include "sdkGlobal.h"
#include "rsaref.h"
#include "rsa.h"

#define SDK_MATH_TEST
// Ϊ�˷�װ�Ƚ����������¶�����������
// Ϊ�˱��ֻ�һ���Լ�������ı�������
// �벻Ҫ���ȥ��
#if (SDK_MAX_RSA_MODULUS_LEN != MAX_RSA_MODULUS_LEN) || (SDK_MAX_RSA_PRIME_LEN != MAX_RSA_PRIME_LEN)
#error " ���������������ĺ꣬��ͬ������������";
#endif

extern s32 sdkDesS(bool bEncrypt, u8 *pheDatat, const u8 *phekeyt);

extern s32 sdkDes3S(bool bEncrypt, u8 *pheDatat, const u8 *phekeyt);

extern void Hash(unsigned char *bb, unsigned long len, unsigned char *Message_Digest);

extern int sdk_rsa_public_encrypt(unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                  R_RSA_PUBLIC_KEY *, R_RANDOM_STRUCT *);

extern int sdk_rsa_private_encrypt(unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                   R_RSA_PRIVATE_KEY *);

extern int sdk_rsa_public_decrypt(unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                  R_RSA_PUBLIC_KEY *);

extern int sdk_rsa_private_decrypt(unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                   R_RSA_PRIVATE_KEY *);


const u16 CRCTAB[] =
        {       //CRC-16 x16+x15+x2+1 (����������) //���� ��λ��ǰ
                0x0000, 0x8005, 0x800f, 0x000a, 0x801b, 0x001e, 0x0014, 0x8011,
                0x8033, 0x0036, 0x003c, 0x8039, 0x0028, 0x802d, 0x8027, 0x0022,
                0x8063, 0x0066, 0x006c, 0x8069, 0x0078, 0x807d, 0x8077, 0x0072,
                0x0050, 0x8055, 0x805f, 0x005a, 0x804b, 0x004e, 0x0044, 0x8041,
                0x80c3, 0x00c6, 0x00cc, 0x80c9, 0x00d8, 0x80dd, 0x80d7, 0x00d2,
                0x00f0, 0x80f5, 0x80ff, 0x00fa, 0x80eb, 0x00ee, 0x00e4, 0x80e1,
                0x00a0, 0x80a5, 0x80af, 0x00aa, 0x80bb, 0x00be, 0x00b4, 0x80b1,
                0x8093, 0x0096, 0x009c, 0x8099, 0x0088, 0x808d, 0x8087, 0x0082,
                0x8183, 0x0186, 0x018c, 0x8189, 0x0198, 0x819d, 0x8197, 0x0192,
                0x01b0, 0x81b5, 0x81bf, 0x01ba, 0x81ab, 0x01ae, 0x01a4, 0x81a1,
                0x01e0, 0x81e5, 0x81ef, 0x01ea, 0x81fb, 0x01fe, 0x01f4, 0x81f1,
                0x81d3, 0x01d6, 0x01dc, 0x81d9, 0x01c8, 0x81cd, 0x81c7, 0x01c2,
                0x0140, 0x8145, 0x814f, 0x014a, 0x815b, 0x015e, 0x0154, 0x8151,
                0x8173, 0x0176, 0x017c, 0x8179, 0x0168, 0x816d, 0x8167, 0x0162,
                0x8123, 0x0126, 0x012c, 0x8129, 0x0138, 0x813d, 0x8137, 0x0132,
                0x0110, 0x8115, 0x811f, 0x011a, 0x810b, 0x010e, 0x0104, 0x8101,
                0x8303, 0x0306, 0x030c, 0x8309, 0x0318, 0x831d, 0x8317, 0x0312,
                0x0330, 0x8335, 0x833f, 0x033a, 0x832b, 0x032e, 0x0324, 0x8321,
                0x0360, 0x8365, 0x836f, 0x036a, 0x837b, 0x037e, 0x0374, 0x8371,
                0x8353, 0x0356, 0x035c, 0x8359, 0x0348, 0x834d, 0x8347, 0x0342,
                0x03c0, 0x83c5, 0x83cf, 0x03ca, 0x83db, 0x03de, 0x03d4, 0x83d1,
                0x83f3, 0x03f6, 0x03fc, 0x83f9, 0x03e8, 0x83ed, 0x83e7, 0x03e2,
                0x83a3, 0x03a6, 0x03ac, 0x83a9, 0x03b8, 0x83bd, 0x83b7, 0x03b2,
                0x0390, 0x8395, 0x839f, 0x039a, 0x838b, 0x038e, 0x0384, 0x8381,
                0x0280, 0x8285, 0x828f, 0x028a, 0x829b, 0x029e, 0x0294, 0x8291,
                0x82b3, 0x02b6, 0x02bc, 0x82b9, 0x02a8, 0x82ad, 0x82a7, 0x02a2,
                0x82e3, 0x02e6, 0x02ec, 0x82e9, 0x02f8, 0x82fd, 0x82f7, 0x02f2,
                0x02d0, 0x82d5, 0x82df, 0x02da, 0x82cb, 0x02ce, 0x02c4, 0x82c1,
                0x8243, 0x0246, 0x024c, 0x8249, 0x0258, 0x825d, 0x8257, 0x0252,
                0x0270, 0x8275, 0x827f, 0x027a, 0x826b, 0x026e, 0x0264, 0x8261,
                0x0220, 0x8225, 0x822f, 0x022a, 0x823b, 0x023e, 0x0234, 0x8231,
                0x8213, 0x0216, 0x021c, 0x8219, 0x0208, 0x820d, 0x8207, 0x0202
        };

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkMD5(u8 *pheDest,  const u8 *pheSrc, s32 siLen )
   ��������: MD5�����㷨
   �������: const u8 *pheSrc:���������ݣ�s32 siLen:���������ݳ���
   �������: u8 *pheDest:���ܺ�����
   ��   ��  ֵ: ��������ָ��Ϊ�����SDK_PARA_ERR
                       �ɹ������ȷ
   �޸ı�ע:
   ����ʱ��: 2012.05.16 17:03:31
*******************************************************************/
extern void MD5(char *digest, const char *buf, int buf_len);

s32 sdkMD5(u8 *pheDest, const u8 *pheSrc, s32 siLen) {
    if (NULL == pheSrc || NULL == pheDest || siLen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    MD5((char *) pheDest, (char *) pheSrc, siLen);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkSHA1(const u8* pheSrc,s32 siLen,u8* pheDest)
   ��������: ��ȫ��ϣ�㷨
   �������: const u8* pheSrc:���������ݣ�s32 siLen:���������ݳ���
   �������: u8* pheDest:���ܺ�����
   ��   ��  ֵ: �����������Ϊ�����SDK_PARA_ERR
                            �ɹ����
   �޸ı�ע:
   ����ʱ��: 2012.05.17 16:37:25
*******************************************************************/
s32 sdkSHA1(const u8 *pheSrc, s32 siLen, u8 *pheDest) {

    if (NULL == pheSrc || NULL == pheDest || siLen <= 0) //ʯ���� 2012.08.20 11:32
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Hash((u8 *) pheSrc, siLen, (u8 *) pheDest);

    return SDK_OK;

}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkRSAPublicEncrypt(u8 *pheDest, s32 *psiDestlen,const u8 *pheSrc, s32 siSrclen,const SDK_RSA_PUBLIC_KEY *pstPublicKey,const u8*pstRandomStruct)
   ��������: RSA�����㷨,��Կ����
   �������: pheSrc:����ļ�������;siSrclen:����������ݳ���;pstPublicKey:��Կ;pstRandomStruct:�����
   �������: pheDest:���ܺ�����:psiDestlen:���ܺ����ݳ���
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��: 2012.05.17 09:06:37
*******************************************************************/
s32 sdkRSAPublicEncrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen,
                        const SDK_RSA_PUBLIC_KEY *pstPublicKey,
                        const SDK_RANDOM_STRUCT *pstRandomStruct) {
    if (NULL == pheSrc || NULL == pstPublicKey || NULL == pstRandomStruct) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((sizeof(SDK_RSA_PUBLIC_KEY) != sizeof(R_RSA_PUBLIC_KEY)) ||
        (sizeof(SDK_RANDOM_STRUCT) != sizeof(R_RANDOM_STRUCT))) //lint  !e506 !e845 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdk_rsa_public_encrypt(pheDest, (u32 *) psiDestlen, (u8 *) pheSrc, (u32) siSrclen,
                                  (R_RSA_PUBLIC_KEY *) pstPublicKey,
                                  (R_RANDOM_STRUCT *) pstRandomStruct);

}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkRSAPrivateEncrypt(u8 *pheDest, s32 *psiDestlen,const u8 *pucSrc, s32 siSrclen,const SDK_RSA_PRIVATE_KEY *pstPrivateKey)
   ��������: RSA�����㷨,˽Կ����
   �������: pucSrc:����ļ�������;pucSrc:����������ݳ���;pstPrivateKey:˽Կ
   �������: pheDest:���ܺ�����:puiDestlen:���ܺ����ݳ���
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��: 2012.05.17 09:06:37
*******************************************************************/
s32 sdkRSAPrivateEncrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pucSrc, s32 siSrclen,
                         const SDK_RSA_PRIVATE_KEY *pstPrivateKey) {

    if (NULL == pucSrc || NULL == pstPrivateKey || siSrclen <= 0 || pheDest == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(SDK_RSA_PRIVATE_KEY) != sizeof(R_RSA_PRIVATE_KEY))  //lint  !e506 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdk_rsa_private_encrypt(pheDest, (u32 *) psiDestlen, (u8 *) pucSrc, siSrclen,
                                   (R_RSA_PRIVATE_KEY *) pstPrivateKey);


}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkRSAPublicDecrypt(u8 *pucDest, s32 *psiDestlen,const u8 *pheSrc, s32 siSrclen,const SDK_RSA_PUBLIC_KEY *pstPublicKey)
   ��������: RSA�����㷨,��Կ����
   �������: pheSrc:����Ľ�������;siSrclen:����������ݳ���;pstPublicKey:��Կ
   �������: pucDest:���ܺ�����:psiDestlen:���ܺ����ݳ���
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��: 2012.05.17 09:06:37
*******************************************************************/
s32 sdkRSAPublicDecrypt(u8 *pucDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen,
                        const SDK_RSA_PUBLIC_KEY *pstPublicKey) {

    if (NULL == pheSrc || NULL == pstPublicKey || siSrclen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(SDK_RSA_PUBLIC_KEY) != sizeof(R_RSA_PUBLIC_KEY)) //lint  !e506 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdk_rsa_public_decrypt(pucDest, (u32 *) psiDestlen, (u8 *) pheSrc, siSrclen,
                                  (R_RSA_PUBLIC_KEY *) pstPublicKey);


}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkRSAPrivateDecrypt(u8 *pheDest, s32 *psiDestlen,const u8 *pucSrc, s32 siSrclen,const SDK_RSA_PRIVATE_KEY *pstPrivateKey)
   ��������: RSA�����㷨,˽Կ����
   �������: pucSrc:����Ľ�������;uiSrclen:����������ݳ���;pstPrivateKey:˽Կ
   �������: pheDest:���ܺ�����:puiDestlen:���ܺ����ݳ���
   ��   ��  ֵ:
   �޸ı�ע:
   ����ʱ��: 2012.05.17 09:06:37
*******************************************************************/
s32 sdkRSAPrivateDecrypt(u8 *pheDest, s32 *psiDestlen, const u8 *pucSrc, s32 siSrclen,
                         const SDK_RSA_PRIVATE_KEY *pstPrivateKey) {

    if (NULL == pucSrc || NULL == pstPrivateKey || siSrclen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(SDK_RSA_PRIVATE_KEY) != sizeof(R_RSA_PRIVATE_KEY))  //lint  !e506 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sdk_rsa_private_decrypt(pheDest, (u32 *) psiDestlen, (u8 *) pucSrc, siSrclen,
                                   (R_RSA_PRIVATE_KEY *) pstPrivateKey);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *pucDest
                               s32 *psiDestlen
                               const u8 *pheSrc
                               s32 siSrclen
                               const SDK_RSA_PUBLIC_KEY *pstPublicKey
** Returned value:	
** Created By:		shijianglong  2013.08.09
** Remarks: 		
*****************************************************************************/
extern int RSAPublicBlock PROTO_LIST((unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                     R_RSA_PUBLIC_KEY *));//shijianglong 2013.08.09 17:59
                         extern int RSAPrivateBlock
                         PROTO_LIST((unsigned char *, unsigned int *, unsigned char *, unsigned int,
                                     R_RSA_PRIVATE_KEY *));//shijianglong 2013.08.09 17:59

                         s32 Private_sdkRSAPublicBlock(u8 *pucDest, s32 *psiDestlen,
                                                       const u8 *pheSrc, s32 siSrclen,
                                                       const SDK_RSA_PUBLIC_KEY *pstPublicKey)
{
    if (NULL == pheSrc || NULL == pstPublicKey || siSrclen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(SDK_RSA_PUBLIC_KEY) != sizeof(R_RSA_PUBLIC_KEY)) //lint  !e506 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    return RSAPublicBlock(pucDest, psiDestlen, (u8 *) pheSrc, siSrclen,
                          (R_RSA_PUBLIC_KEY *) pstPublicKey);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	u8 *pucDest
                               s32 *psiDestlen
                               const u8 *pheSrc
                               s32 siSrclen
                               const SDK_RSA_PRIVATE_KEY *pstPrivateKey
** Returned value:	
** Created By:		shijianglong  2013.08.09
** Remarks: 		
*****************************************************************************/
s32 Private_sdkRSAPrivateBlock(u8 *pucDest, s32 *psiDestlen, const u8 *pheSrc, s32 siSrclen,
                               const SDK_RSA_PRIVATE_KEY *pstPrivateKey) {
    if (NULL == pheSrc || NULL == pstPrivateKey || siSrclen <= 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(SDK_RSA_PRIVATE_KEY) != sizeof(R_RSA_PRIVATE_KEY)) //lint  !e506 !e774
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    return RSAPrivateBlock(pucDest, psiDestlen, (u8 *) pheSrc, siSrclen,
                           (R_RSA_PRIVATE_KEY *) pstPrivateKey);
}


/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkAscToBcd(u8 *pbcDest , const u8 *pasSrc , s32 siSrclen)
   ��������: ��ascii��ת��ΪBCD��
   �������: const u8 *pasSrc :ascii���ַ;s32 siSrclen:ascii�ֽڸ���
   �������:u8 *pbcDest:ѹ��BCD�����ַ
   ��   ��  ֵ: ��ȷ���BCD���ȡ�
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���ߴ������ݲ�Ϊascii���SDK_PARA_ERR
   �޸ı�ע: ����Ϊ������ĩβ��0x00
   ����ʱ��: 2012.05.14 16:56:37
*******************************************************************/
s32 sdkAscToBcd(u8 *pbcDest, const u8 *pasSrc, s32 siSrclen) {
    s32 i = 0;
    u8 high = 0, low = 0;

    if ((NULL == pasSrc) || (NULL == pbcDest) || (siSrclen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < siSrclen; i++) {
        if ((pasSrc[i] < '0') || ((pasSrc[i] > '9') && (pasSrc[i] < 'A')) ||
            ((pasSrc[i] > 'F') && (pasSrc[i] < 'a')) || (pasSrc[i] > 'f')) {
            return SDK_PARA_ERR;
        }
    }

    for (i = 0; i < siSrclen; i++) {
        //��תbcd���Nibble
        if ((*(pasSrc + i) >= 0x61) && (*(pasSrc + i) <= 0x66))          //range a~f
        {
            high = *(pasSrc + i) - 0x57;
        }
        else if ((*(pasSrc + i) >= 0x41) && (*(pasSrc + i) <= 0x46))      //range A~F
        {
            high = *(pasSrc + i) - 0x37;
        }
        else if ((*(pasSrc + i) >= 0x30) && (*(pasSrc + i) <= 0x39))      //range 0~9
        {
            high = *(pasSrc + i) - 0x30;
        }
        else {
            high = 0x00;                                        //����
        }
        //��תbcd���Nibble
        i++;

        if (i < siSrclen) {
            if ((*(pasSrc + i) >= 0x61) && (*(pasSrc + i) <= 0x66))        //range a~f
            {
                low = *(pasSrc + i) - 0x57;
            }
            else if ((*(pasSrc + i) >= 0x41) && (*(pasSrc + i) <= 0x46)) //range A~F
            {
                low = *(pasSrc + i) - 0x37;
            }
            else if ((*(pasSrc + i) >= 0x30) && (*(pasSrc + i) <= 0x39))      //range 0~9
            {
                low = *(pasSrc + i) - 0x30;
            }
            else {
                low = 0x00;                                            //����
            }
        }
        else {
            i--;                                                    //Ԥ��255��ʱ�������
            low = 0x00;                                            //�����������ĩβ��0x00
        }
        *(pbcDest + i / 2) = (high << 4) | low;                      //�ϲ�BCD��
    }

    return (siSrclen + 1) / 2;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: bool sdkIsAscii(const u8 *pasSrc)
   ��������: �ж�Src�ַ������ַ��Ƿ���ascii
   �������: const u8 *pasSrc:�����ַ���
   �������: ��
   ��   ��  ֵ: ��ȷ���true��
                         ����ָ��Ϊ�����false
                        �������false��
   �޸ı�ע:
   ����ʱ��: 2012.05.15 08:57:23
*******************************************************************/
bool sdkIsAscii(const u8 *pasSrc) {
    u16 len, i;
    bool flag = true;

    if (NULL == pasSrc || strlen(pasSrc) == 0) {
        Assert(0);
        return false;
    }
    len = strlen(pasSrc);

    for (i = 0; i < len; i++) {
        if (pasSrc[i] > 0x80) {
            flag = false;
            break;
        }
    }

    return flag;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkU8ToBcd(u8 *pbcDest, u8 const ucSrc,u32 uiDestlen)
   ��������: ��һ��8λ�޷�����ת���ɹ̶��ֽ��Ҷ���ѹ��BCD��
   �������: u8 const uiSrc :ԭu8����;u32 uiDestlen:BCD�ֽڸ���
   �������: u8 *pbcDest:�Ҷ���ѹ��BCD��
   ��   ��  ֵ: ��ȷ���BCD�ֽڸ���
                          ���������u8 * pbcDest����NULL���ߴ���
                          ����uiDestlenΪ0ʱ���ز�������SDK_PARA_ERR
   �޸ı�ע: u8�������255,��Ŀ��ָ�������Ҫ2���ֽڴ洢�ռ�,
   ע��洢�ռ����.
   ����ʱ��:2012.05.11 10:28:19
*******************************************************************/
s32 sdkU8ToBcd(u8 *pbcDest, u8 const ucSrc, s32 siDestlen) {
    u32 i = 0;
    u8 num = ucSrc;

    if ((NULL == pbcDest) || (siDestlen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = siDestlen; i > 0; i--) {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return siDestlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdToAsc(u8 *pasDest , const u8 *pbcSrc , s32 siBcdSrclen)
   ��������: ��ѹ��BCD��ת��Ϊascii��
   �������:  const u8 *pbcSrc��ת��ѹ��BCD����Դ; s32 siBcdSrclen:BCD�ֽڸ���
   �������: u8 *pasDest:ascii���ַ
   ��   ��  ֵ: ��ȷ���ת���ɵ�ascii����
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
                          �������ݲ�Ϊbcd�����SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.14 16:56:37
*******************************************************************/
s32 sdkBcdToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen) {
    s32 i = 0;

    if ((NULL == pbcSrc) || (NULL == pasDest) || (siBcdSrclen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < siBcdSrclen; i++) {
        //��Nibbleת��
        if (((*(pbcSrc + i) & 0xF0) >> 4) <= 9) {
            *(pasDest + 2 * i) = ((*(pbcSrc + i) >> 4) & 0X0F) + 0x30;
        }
        else {
            *(pasDest + 2 * i) = ((*(pbcSrc + i) >> 4) & 0x0F) + 0x37;   //��дA~F
        }

        //��Nibbleת��
        if ((*(pbcSrc + i) & 0x0F) <= 9) {
            *(pasDest + 2 * i + 1) = (*(pbcSrc + i) & 0x0F) + 0x30;
        }
        else {
            *(pasDest + 2 * i + 1) = (*(pbcSrc + i) & 0x0F) + 0x37;   //��дA~F
        }
    }

    return siBcdSrclen * 2;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:s32 sdkU16ToAsc(u16 const usSrc , u8 *pasDest)
   ��������:��һ��8λ�޷�����ת����Ascii��
   �������:u16 const usSrc :16λ���޷�����
   �������:u8 *pasDest :ascii��洢��ַ
   ��   ��  ֵ:��ȷ���ת���ɵ�ascii�볤��
                      ����ָ��Ϊ�����SDK_PARA_ERR
   �޸ı�ע:u16�������65535��ע�ⳬ�����Ƚ���
   ������������ݴ���
   ����ʱ��: 2012.06.06 17:16:34
*******************************************************************/
s32 sdkU16ToAsc(u16 const usSrc, u8 *pasDest) {
    u8 pOut[10];
    u32 len;

    if (pasDest == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = (u32) sprintf((char *) pOut, "%d", usSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32) len);
    return len;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkU32ToHex(u8 *pheDest , u32 const uiSrc , u32 siHexlen)
   ��������: ��һ��32λ�޷�����ת���ɹ̶��ֽ��Ҷ���HEX
   �������: u32 const uiSrc :ԭu32����; u32 siHexlen:HEX�ֽڸ���
   �������: u8 *pheDest:HEX�����ַ
   ��   ��  ֵ: ��ȷ���HEX�ֽڸ���
                          ����ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע: HexΪ��λ��ǰ,��λ�ں�Ĵ洢˳��ת����16����
   ����Դ���ҿ���λ��ǰ,��λ�ں�,���ȵ�λ���ֽڼ�.u32����
   ���4G,��Ŀ��ָ�������Ҫ4���ֽڴ洢�ռ�,��Ȼ�����
   ���ռ���siHexlenȷ��.
   ����ʱ��: 2012.05.14 15:36:07
*******************************************************************/
s32 sdkU32ToHex(u8 *pheDest, u32 const uiSrc, s32 siHexlen) {
    u32 i = 0;
    u32 num = uiSrc;

    if ((NULL == pheDest) || (siHexlen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = siHexlen; i > 0; i--) {
        *(pheDest + i - 1) = num % 256;
        num /= 256;
    }

    return siHexlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:s32 sdkU32ToAsc(u32 const uiSrc , u8 *pasDest)
   ��������:��һ��32λ�޷�����ת����Ascii��
   �������:u32 const uiSrc :32λ���޷�����
   �������:u8 *pasDest :ascii��洢��ַ
   ��   ��  ֵ:��ȷ���ת���ɵ�ascii�볤��
                      ����ָ��Ϊ�����SDK_PARA_ERR
   �޸ı�ע:u32�������4G��ע�ⳬ�����Ƚ���
   ������������ݴ���.
   ����ʱ��: 2012.06.06 17:16:34
*******************************************************************/
s32 sdkU32ToAsc(u32 const uiSrc, u8 *pasDest) {
    u8 pOut[10];
    u32 len;

    if (pasDest == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = (u32) sprintf((char *) pOut, "%d", (s32) uiSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32) len);
    return len;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:s32 sdkU8ToAsc(u8 const ucSrc , u8 *pasDest)
   ��������:��һ��8λ�޷�����ת����Ascii��
   �������:u8 const ucSrc :8λ���޷�����
   �������:u8 *pasDest: ascii��洢��ַ
   ��   ��  ֵ:��ȷ���ת���ɵ�ascii�볤��
                      ����ָ��Ϊ�����SDK_PARA_ERR
   �޸ı�ע:u8�������255��ע�ⳬ�����Ƚ���
   ������������ݴ���
   ����ʱ��: 2012.06.06 17:16:34
*******************************************************************/
s32 sdkU8ToAsc(u8 const ucSrc, u8 *pasDest) {
    u8 pOut[10];
    u32 len;

    if (pasDest == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = (u32) sprintf((char *) pOut, "%d", ucSrc);
    Assert(len + 1 <= sizeof(pOut));
    memcpy(pasDest, pOut, (u32) len);
    return len;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:s32 sdkAscToBcdR(u8 *pbcDest, const u8 *pasSrc, s32 siBcdlen)
   ��������: ��ascii��ת��Ϊ�ҿ�BCD��
   �������:  const u8 *pasSrc:ascii���ַ;s32 siBcdlen:bcd�ֽڸ���
   �������:u8 *pbcDest:ѹ��BCD�����ַ
   ��   ��  ֵ: ��ȷ���BCD���ȡ�
                          ��������ָ��Ϊ�����SDK_PARA_ERR
                         �������ݳ���Ϊ0���ߴ���asc���ݳ��Ȼ��߲���0~F,a~f֮�����SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.14 16:56:37
*******************************************************************/
s32 sdkAscToBcdR(u8 *pbcDest, const u8 *pasSrc, s32 siBcdlen) {
    u8 tmp[3];
    s32 ui_i, ui_j, ui_k;

    if ((NULL == pasSrc) || (NULL == pbcDest) || (siBcdlen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ui_i = (u32) strlen(pasSrc);                      //��ȡ�����ַ�������

    for (ui_j = 0; ui_j < ui_i; ui_j++)              //���ַ�������
    {
        if ((pasSrc[ui_j] < '0') || ((pasSrc[ui_j] > 'F') && (pasSrc[ui_j] < 'a')) ||
            (pasSrc[ui_j] > 'f')) {
            return SDK_PARA_ERR;
        }
    }

    memset(pbcDest, 0, siBcdlen);

    if (ui_i > siBcdlen * 2)                                             //��ȡ
    {
        for (ui_j = 0, ui_k = 0; ui_j < siBcdlen;) //
        {
            if (ui_k >= siBcdlen * 2) {
                break;
            }
            Verify(sdkAscToBcd(&pbcDest[ui_j], &pasSrc[(ui_i - siBcdlen * 2) + ui_k], 2) > 0);
            ui_j = ui_j + 1;
            ui_k = ui_k + 2;
        }
    }
    else if (ui_i == siBcdlen * 2) {
        Verify(sdkAscToBcd(pbcDest, pasSrc, siBcdlen * 2) > 0);
    }
    else {
        if (ui_i % 2 == 0)                               //
        {
            for (ui_j = 0, ui_k = 0; ui_j < siBcdlen;) //,ui_k >= ui_i
            {
                if (ui_k >= ui_i) {
                    break;
                }
                Verify(sdkAscToBcd(&pbcDest[(siBcdlen - ui_i / 2) + ui_j], &pasSrc[ui_k], 2) > 0);
                ui_j = ui_j + 1;
                ui_k = ui_k + 2;
            }
        }
        else                                            //������һ��ǰ�油��һ��0x30
        {
            memset(tmp, 0x30, 2);
            tmp[2] = 0;
            memcpy(&tmp[1], &pasSrc[0], 1);
            Verify(sdkAscToBcd(&pbcDest[(siBcdlen - (ui_i + 1) / 2)], tmp, 2) > 0);

            for (ui_j = 1, ui_k = 1; ui_j < siBcdlen;) //,ui_k >= ui_i
            {
                if (ui_k >= ui_i) {
                    break;
                }
                Verify(sdkAscToBcd(&pbcDest[(siBcdlen - (ui_i + 1) / 2) + ui_j], &pasSrc[ui_k], 2) >
                       0);
                ui_j = ui_j + 1;
                ui_k = ui_k + 2;
            }
        }
    }
    return siBcdlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdToU16(u16 *pusDest,const u8 *pbcSrc,s32 siSrclen)
   ��������: ��ѹ��BCD��ת����һ��16λ�޷�����
   �������: const u8 *pbcSrc:ѹ��BCD�����ַ; s32 siSrclen:BCD�ֽڸ���
   �������: u16 *pusDest:u16���ݵ�ַ
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
                          �������ݲ�Ϊbcd�����SDK_PARA_ERR
   �޸ı�ע:��תBCD������ԴΪ�Ҷ���ѹ������,���ȵ�λ
   ���ֽڼ�. uiSrclenָ��Ϊ��תBCD������Դ�洢�ֽڳ���,��
   ����ѹ��BCD�������.Ŀ���������ͱ���Ϊu16��ָ��,����
   ����ָ������ʽ����.��תBCD������Դֵ��Ӧ�ó���65535,
   ����ֻ�ܵõ����������.
   ����ʱ��: 2012.05.14 12:48:55
*******************************************************************/
s32 sdkBcdToU16(u16 *pusDest, const u8 *pbcSrc, s32 siSrclen) {
    s32 i = 0;
    u32 temp = 0;

    if ((NULL == pbcSrc) || (NULL == pusDest) || (siSrclen <= 0) || (siSrclen > 3)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *pusDest = 0;
    temp = 0;

    for (i = 0; i < siSrclen; i++) {
        temp *= 100;
        temp += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    *pusDest = (u16) temp;

    if (temp > 65535) {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkU16ToBcd(u8 *pbcDest , u16 const usSrc , u32 uiDestlen)
   ��������: ��һ��16λ�޷�����ת���ɹ̶��ֽ��Ҷ���ѹ��BCD��
   �������: u16 const usSrc:ԭu16����;u32 uiDestlen:BCD�ֽڸ���
   �������: u8 *pbcDest:�Ҷ���ѹ��BCD��
   ��   ��  ֵ: ��ȷ���BCD�ֽڸ���
                         ���������u8 * pbcDest����NULL���ߴ��볤�� uiDestlenΪ0ʱ
                         ���ز�������SDK_PARA_ERR
   �޸ı�ע:u16�������65535,��Ŀ��ָ�������Ҫ3���ֽڴ洢�ռ�,
   ע��洢�ռ����.
   ����ʱ��: 2012.05.14 11:06:03
*******************************************************************/
s32 sdkU16ToBcd(u8 *pbcDest, u16 const usSrc, s32 uiDestlen) {
    u32 i = 0;
    u16 num = usSrc;

    if ((NULL == pbcDest) || (uiDestlen < 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = uiDestlen; i > 0; i--) {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return uiDestlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkXOR8(u8 *pheDest , const u8 *pheSrc1 , const u8 *pheSrc2)
   ��������: 8���ֽ����
   �������:  const u8 *pheSrc1 :Դ��ַ1;  const u8 *pheSrc2:Դ��ַ2
   �������: u8 *pheDest:Ŀ�ĵ�ַ
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�����SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.14 16:56:37
*******************************************************************/
s32 sdkXOR8(u8 *pheDest, const u8 *pheSrc1, const u8 *pheSrc2) {
    s32 i = 0;

    if ((NULL == pheSrc1) || (NULL == pheSrc2) || (NULL == pheDest)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = 0; i < 8; i++) {
        *(pheDest + i) = *(pheSrc1 + i) ^ *(pheSrc2 + i);
    }

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkEcb(u8 *pheDest,const u8 *pheSrc,s32 siLen)
   ��������: ��׼�й�����ECB�㷨������㷨
   �������: const u8 *pheSrc:��Mac����ָ��;s32 siLen:��Mac���ݳ���
   �������: u8 *pheDest:�������16���ֽ�ASCII����
   ��   ��  ֵ: ��ȷ���SDK_OK
                         ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.15 10:01:10
*******************************************************************/
s32 sdkEcb(u8 *pheDest, const u8 *pheSrc, s32 siLen) {
    u16 i;
//    u8 temp[256], A1[256];
    u8 *temp, *A1;

    if ((NULL == pheSrc) || (NULL == pheDest) || (siLen <= 0)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    temp = (u8 *) sdkGetMem(256);
    if (temp == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    A1 = (u8 *) sdkGetMem(256);
    if (A1 == NULL) {
        Assert(0);
        return SDK_ERR;
    }

    memset(A1, 0, 256);

    for (i = 0; i < (siLen / 8); i++) {
        sdkXOR8(A1, A1, &pheSrc[i * 8]);
    }

    if ((siLen % 8) != 0) {
        memset(temp, 0, 256);
        memcpy(temp, &pheSrc[(siLen / 8) * 8], siLen - (siLen / 8) * 8);
        sdkXOR8(A1, A1, temp);
    }
    sdkBcdToAsc(pheDest, A1, 8);

    sdkFreeMem(temp);
    sdkFreeMem(A1);

    return SDK_OK;
}

/*****************************�����ӿں�������͸��****************************/



#ifdef EXTEND_SDK
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkMathsGetVersion(u8 *pasDest)
   ��������: �����ѧ��汾��Ϣ
   �������: ��
   �������: u8 *pasDest:��ѧ��汾��
   ��   ��  ֵ: ��ѧ��汾�ų���
                          ����ָ��Ϊ�����SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.15 14:15:08
*******************************************************************/
#if 0 /*Modify by fusuipu at 2013.01.29  20:4 */
s32 sdkMathsGetVersion(u8 *pasDest)
{
    if(NULL == pasDest)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return sprintf((char *)pasDest, "V120515101");
}
#endif /* if 0 */

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkU32ToBcd(u8 *pbcDest , u32 const uiSrc , u32 uiDestlen)
   ��������: ��һ��32λ�޷�����ת���ɹ̶��ֽ��Ҷ���ѹ��BCD��
   �������: u32 const uiSrc:ԭu32����;u32 uiDestlen:BCD�ֽڸ���
   �������: u8 *pbcDest:�Ҷ���ѹ��BCD��
   ��   ��  ֵ: ��ȷ���BCD�ֽڸ���
                          ���������u8 * pbcDest����NULL���ߴ��볤�� uiDestlenΪ0ʱ
                          ���ز�������SDK_PARA_ERR
   �޸ı�ע: uClinux ��֧���з��ŵ��ַ���,ȴ֧���з�������
   u32�������4G,��Ŀ��ָ�������Ҫ5���ֽڴ洢�ռ�,ע��洢�ռ����.
   ����ʱ��: 2012.05.14 11:06:03
*******************************************************************/
s32 sdkU32ToBcd(u8 *pbcDest, u32 const uiSrc, s32 siDestlen)
{
    u32 i = 0;
    u32 num = uiSrc;

    if((NULL == pbcDest) || (siDestlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siDestlen; i > 0; i--)
    {
        *(pbcDest + i - 1) = (((num % 100) / 10) << 4) | ((num % 100) % 10);
        num /= 100;
    }

    return siDestlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdToU8(u8 *pucDest,const u8 *pbcSrc,u32 uiSrclen)
   ��������: ��ѹ��BCD��ת����һ��8λ�޷�����
   �������: const u8 *pbcSrc:ѹ��BCD�����ַ; s32 siSrclen:BCD�ֽڸ���
   �������: u8 *pucDest:u8���ݵ�ַ
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
                          �������ݲ�Ϊbcd�����SDK_PARA_ERR
   �޸ı�ע:��תBCD������ԴΪ�Ҷ���ѹ������,���ȵ�λ
   ���ֽڼ�.siSrclenָ��Ϊ��תBCD������Դ�洢�ֽڳ���,������
   ѹ��BCD�������.Ŀ�����ݺ�Դ�������ͱ���Ϊu8��ָ��,����
   ����ָ������ʽ����.��תBCD������Դֵ��Ӧ�ó���255,����
   ֻ�ܵõ����������.
   ����ʱ��: 2012.05.11 11:04:55
*******************************************************************/
s32 sdkBcdToU8(u8 *pucDest, const u8 *pbcSrc, s32 siSrclen)
{
    s32 i = 0;
    u32 sum = 0;

    if((NULL == pbcSrc) || (NULL == pucDest) || (siSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siSrclen; i++)
    {
        sum *= 100;
        sum += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    *pucDest = (u8)sum;

    if(sum > 255)
    {
        //  Assert(0);//fusuipu 2013.01.05 14:36
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdToU32(u32 *puiDest,const u8 *pbcSrc,u32 uiSrclen)
   ��������: ��ѹ��BCD��ת����һ��16λ�޷�����
   �������: const u8 *pbcSrc:ѹ��BCD�����ַ; u32 uiSrclen:BCD�ֽڸ���
   �������: u32 *puiDest:u32���ݵ�ַ
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
                          �������ݲ�Ϊbcd�����SDK_PARA_ERR
   �޸ı�ע:Ŀ���������ͱ���Ϊu32��ָ��,������ָ��
   �����ʽ����.��תBCD������Դֵ��Ӧ�ó���4G,����ֻ��
   �õ����������.
   ����ʱ��: 2012.05.14 12:48:55
*******************************************************************/
s32 sdkBcdToU32(u32 *puiDest, const u8 *pbcSrc, s32 siSrclen)
{
    s32 i = 0;

    if((NULL == pbcSrc) || (NULL == puiDest) || (siSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *puiDest = 0;

    for(i = 0; i < siSrclen; i++)
    {
        *puiDest *= 100;
        *puiDest += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    if(siSrclen >= 5 && pbcSrc[0] > 0x42)
    {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkHexToU16(u16 *pusDest ,const u8 *pheSrc , s32 siHexlen)
   ��������: ��HEX����ת����һ��16λ�޷�����
   �������: const u8 *pheSrc:HEX�����ַ; s32 siHexlen:Hex�ֽڸ���
   �������: u16 *pusDest :һ��16λ�޷�����
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע: HexΪ��λ��ǰ,��λ�ں�Ĵ洢˳��Ŀ����������
   ����Ϊu16��ָ��,Դ����Ϊu8��ָ��.��ת��16��������Դ���2��
   �ֽھͿ��Ա��һ��u16����.��Ȼ����Դ���ȿɱ�,�������ֵ
   ��Ӧ�ó���65535,����ᵼ���������.
   ����ʱ��: 2012.05.14 14:51:01
*******************************************************************/
s32 sdkHexToU16(u16 *pusDest, const u8 *pheSrc, s32 siHexlen)
{
    s32 i = 0;
    u32 temp = 0;

    if((NULL == pheSrc) || (NULL == pusDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siHexlen; i++)
    {
        temp *= 256;
        temp += pheSrc[i];
    }

    *pusDest = (u16)temp;

    if(temp > 65535)
    {
        return SDK_PARA_ERR;
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkHexToU32(u32 *puiDest ,const u8 *pheSrc , s32 siHexlen)
   ��������: ��HEX����ת����һ��32λ�޷�����
   �������: const u8 *pheSrc:HEX�����ַ;s32 siHexlen :Hex�ֽڸ���
   �������: u32 *puiDest:һ��32λ�޷�����
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע: HexΪ��λ��ǰ,��λ�ں�Ĵ洢˳��Ŀ����������
   ����Ϊu32��ָ��,Դ����Ϊu8��ָ��.��ת��16��������Դ���
   4���ֽھͿ��Ա��һ��u32����.��Ȼ����Դ���ȿɱ�,�����
   ��ֵ��Ӧ�ó���4G,����ᵼ���������.
   ����ʱ��: 2012.05.14 14:51:01
*******************************************************************/
s32 sdkHexToU32(u32 *puiDest, const u8 *pheSrc, s32 siHexlen)
{
    s32 i = 0;

    if((NULL == pheSrc) || (NULL == puiDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    *puiDest = 0;

    for(i = 0; i < siHexlen; i++)
    {
        *puiDest *= 256;
        *puiDest += pheSrc[i];
    }

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkU16ToHex(u8 *pheDest , u16 const usSrc , s32 siHexlen)
   ��������: ��һ��16λ�޷�����ת���ɹ̶��ֽ��Ҷ���HEX
   �������: u16 const usSrc:ԭu16����;s32 siHexlen:HEX�ֽڸ���
   �������: u8 *pheDest:HEX�����ַ
   ��   ��  ֵ: ��ȷ���HEX�ֽڸ���
                          ����ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע: HexΪ��λ��ǰ,��λ�ں�Ĵ洢˳��,u16�������
   ��65535,��Ŀ��ָ�������Ҫ2���ֽڴ洢�ռ�,��Ȼ����洢
   �ռ���siHexlenȷ��.
   ����ʱ��: 2012.05.14 15:36:07
*******************************************************************/
s32 sdkU16ToHex(u8 *pheDest, u16 const usSrc, s32 siHexlen)
{
    u32 i = 0;
    u16 num = usSrc;

    if((NULL == pheDest) || (siHexlen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = siHexlen; i > 0; i--)
    {
        *(pheDest + i - 1) = num % 256;
        num /= 256;
    }

    return siHexlen;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdToFloat(double *pDest , const u8 *pbcSrc , s32 siBcdSrclen)
   ��������: ��ѹ��BCD��ת����һ��float
   �������:  const u8 *pbcSrc:ѹ��BCD�����ַ;  s32 siBcdSrclen:BCD�ֽڸ���
   �������: double *pDest:float���ݵ�ַ
   ��   ��  ֵ: ��ȷ���SDK_OK
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
                          �������ݲ�Ϊbcd�����SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.14 16:56:37
*******************************************************************/
s32 sdkBcdToFloat(double *pDest, const u8 *pbcSrc, s32 siBcdSrclen)
{
    s32 i;

    if((NULL == pbcSrc) || (NULL == pDest) || (siBcdSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    
    *pDest = 0;

    for(i = 0; i < siBcdSrclen; i++)
    {
        *pDest *= 100;
        *pDest += 10 * ((*(pbcSrc + i) & 0xF0) >> 4) + (*(pbcSrc + i) & 0x0F);
    }

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdCardNumToAsc(u8 *pasDest , const u8 *pbcSrc , s32 siBcdSrclen)
   ��������: ���ŵ�bcd��ת����ascii
   �������: const u8 *pbcSrc:bdc��Ŀ��ţ� s32 siBcdSrclen:���ų���
   �������: u8 *pasDest :ת���ɵ�ascii��
   ��   ��  ֵ: ��ȷ���ת���ɵ�ascii����
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.18 10:56:24
*******************************************************************/
s32 sdkBcdCardNumToAsc(u8 *pasDest, const u8 *pbcSrc, s32 siBcdSrclen)
{
    s32 i;

    if((NULL == pbcSrc) || (NULL == pasDest) || (siBcdSrclen < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Verify(sdkBcdToAsc(pasDest, pbcSrc, siBcdSrclen) >= 0);

    for(i = 0; i < siBcdSrclen * 2; i++)
    {
        if((pasDest[i]) == 0X44)
        {
            *(pasDest + i) = 0x3d;
        }
    }

    return siBcdSrclen * 2;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Xinguodu Ltd,Co.
   ��������: void BcdAdd(u8 *Dest,u8 *Src,u32 Len)
   ��������: BCD��ӷ�����
   ��ڲ���: 1.ĿԴ��ַ; 2.Դ��ַ; 3.�ֽڸ���
   �� �� ֵ: ��
   ��ص���:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
 #ifndef __uClinux__
void BcdAdd(u8 *Dest, u8 *Src, u32 Len)
{
    u32 i;
    u8 high, low;
    u8 carry = 0;                     //�����λ��־

    for(i = Len; i > 0; i--)
    {
        //��nibble������
        low = (*(Dest + i - 1) & 0x0F) + (*(Src + i - 1) & 0x0F) + carry;
        carry = 0;

        if(low > 0x09)
        {
            low = low - 0x0A;
            carry = 1;
        }
        //��nibble������
        high  = ( (*(Dest + i - 1) & 0xF0) >> 4) + ((*(Src + i - 1) & 0xF0) >> 4) + carry;
        carry = 0;

        if(high > 0x09)
        {
            high = high - 0x0A;
            carry = 1;
        }
        *(Dest + i - 1) = (high << 4) | low;
    }
}
#endif
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdAdd(u8 *pbcDest , const u8 *pbcSrc1 , s32 siBcdSrc1len,const u8 *pbcSrc2,s32 siBcdSrc2len)
   ��������:  BCD��ӷ�����
   �������:  const u8 *pbcSrc1:����BCD�ӷ������һԴ����;
                            s32 siBcdSrc1len:ָ��BCD��һ���������ֽڳ���
                            const u8 *pbcSrc2:����BCD�ӷ�����ڶ�Դ����;
                            s32 siBcdSrc2len:ָ��BCD�ڶ����������ֽڳ���
   �������: u8 *pbcDest:����������ݴ��ָ��
   ��   ��  ֵ: ��ȷ������Ӻ�ĳ���
                            �����ڴ�������ʧ��
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���ߴ������ݲ�ΪBCD�������SDK_PARA_ERR
   �޸ı�ע:siBcdSrc2lenָ��ΪBCD��������Դ�ֽڳ���,������BCD�������.
   BCD����ֻ����0-9��������,��֧��A-F����,�������Դ�г��ַ�
   10��������,���ز�������.BCD�����λ����������
   ��λ,�����������Ч����
   ����ʱ��: 2012.05.11 11:04:55
*******************************************************************/
s32 sdkBcdAdd(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len)
{
    u32 len = siBcdSrc2len > siBcdSrc1len ? siBcdSrc2len : siBcdSrc1len;

    if((NULL == pbcSrc1) || (NULL == pbcSrc2) || (NULL == pbcDest) || (siBcdSrc1len < 0) || (siBcdSrc2len < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc2, siBcdSrc2len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc1, siBcdSrc1len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    /*=======BEGIN: fusuipu 2013.05.13  14:34 modify===========*/
    if(len == 0)
    {
        return len;
    }
    /*====================== END======================== */
    {
        u8 *pTemp = sdkGetMem(len);

        if(pTemp == NULL)
        {
            Assert(0);
            return SDK_ERR;
        }
        memset(pTemp, 0, len);

        if(siBcdSrc2len > siBcdSrc1len)
        {
            memcpy(&pTemp[siBcdSrc2len - siBcdSrc1len], pbcSrc1, (u32)siBcdSrc1len);
            BcdAdd(pTemp, ( u8 *)pbcSrc2, len);
        }
        else
        {
            memcpy(&pTemp[siBcdSrc1len - siBcdSrc2len], pbcSrc2, siBcdSrc2len);
            BcdAdd(pTemp, ( u8 *)pbcSrc1, len);
        }
//   BcdAdd(pTemp, ( u8 *)pbcSrc2 , uiBcdSrc2len);
        memcpy(pbcDest, pTemp, len);

        sdkFreeMem(pTemp);
    }
    return len;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Xinguodu Ltd,Co.
   ��������: void BcdSub(u8 *Dest,u8 *Src,u32 Len)
   ��������: BCD���������
   ��ڲ���: 1.ĿԴ��ַ; 2.Դ��ַ; 3.�ֽڸ���
   �� �� ֵ: ��
   ��ص���:
   ��    ע: LenΪu8,ָ��Src���������Ե�255,���������ʱ������,
          ԴĿ�Ĳ���������λ��һ��
   �޸���Ϣ:
 ********************************************************************/
 #ifndef __uClinux__
void BcdSub(u8 *Dest, u8 *Src, u32 Len)
{
    u32 i;
    u8 high, low;
    u8 carry = 0;                     //�����λ��־

    for(i = Len; i > 0; i--)
    {
        //��nibble������
        if( (*(Dest + i - 1) & 0x0F) < ( (*(Src + i - 1) & 0x0F) + carry) )
        {
            low = ((*(Dest + i - 1) & 0x0F) + 0x0A) - (*(Src + i - 1) & 0x0F) - carry;
            carry = 1;
        }
        else
        {
            low = (*(Dest + i - 1) & 0x0F) - (*(Src + i - 1) & 0x0F) - carry;
            carry = 0;
        }

        //��nibble������
        if( ((*(Dest + i - 1) & 0xF0) >> 4) < (((*(Src + i - 1) & 0xF0) >> 4) + carry) )
        {
            high = (((*(Dest + i - 1) & 0xF0) >> 4) + 0x0A) - ((*(Src + i - 1) & 0xF0) >> 4)  - carry;
            carry = 1;
        }
        else
        {
            high = ((*(Dest + i - 1) & 0xF0) >> 4) - ((*(Src + i - 1) & 0xF0) >> 4)  - carry;
            carry = 0;
        }
        *(Dest + i - 1) = (high << 4) | low;
    }
}
#endif
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkBcdSub(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len)
   ��������:  BCD���������
   �������:  const u8 *pbcSrc1:����BCD���������һԴ����;
                            s32 siBcdSrc1len:ָ��BCD��һ���������ֽڳ���
                            const u8 *pbcSrc2:����BCD��������ڶ�Դ����;
                            s32 siBcdSrc2len:ָ��BCD�ڶ����������ֽڳ���
   �������: u8 *pbcDest:����������ݴ��ָ��
   ��   ��  ֵ: ��ȷ������Ӻ�ĳ���
                            �����ڴ�������ʧ��
                          ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���ߴ������ݲ�ΪBCD�������SDK_PARA_ERR
   �޸ı�ע:uiBcdSrc2lenָ��ΪBCD��������Դ�ֽڳ���,������BCD�������.
   BCD����ֻ����0-9��������,��֧��A-F����,�������Դ�г��ַ�
   10��������,���ز�������.BCD�����λ����������
   ��λ,�����������Ч����
   ����ʱ��: 2012.05.11 11:04:55
*******************************************************************/
s32 sdkBcdSub(u8 *pbcDest, const u8 *pbcSrc1, s32 siBcdSrc1len, const u8 *pbcSrc2, s32 siBcdSrc2len)
{
    u32 len = siBcdSrc2len > siBcdSrc1len ? siBcdSrc2len : siBcdSrc1len;

    if((NULL == pbcSrc1) || (NULL == pbcSrc2) || (NULL == pbcDest) || (siBcdSrc1len < 0) || (siBcdSrc2len < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc2, siBcdSrc2len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if(!sdkIsBcdNum(pbcSrc1, siBcdSrc1len))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
	 /*=======BEGIN: fusuipu 2013.05.13  14:34 modify===========*/
    if(len == 0)
    {
        return len;
    }
    /*====================== END======================== */
    {
        u8 *pTemp = sdkGetMem(len);
        u8 *pTemp1 = sdkGetMem(len);//fusuipu add at 2013-1-28

        if(pTemp == NULL ||pTemp1 == NULL )
        {
            if(pTemp != NULL)
            {
                sdkFreeMem(pTemp);
            }
            if(pTemp1 != NULL)
            {
                sdkFreeMem(pTemp1);
            }
            Assert(0);
            return SDK_ERR;
        }
        memset(pTemp, 0, len);

        if(siBcdSrc2len > siBcdSrc1len)
        {
            memcpy(&pTemp[siBcdSrc2len - siBcdSrc1len], pbcSrc1, (u32)siBcdSrc1len);
            BcdSub(pTemp, ( u8 *)pbcSrc2, len);
            memcpy(pbcDest, pTemp, len);
        }
        else
        {
            memcpy(&pTemp[siBcdSrc1len - siBcdSrc2len], pbcSrc2, siBcdSrc2len);
             /*=======BEGIN: fusuipu 2013.01.27  20:6 modify===========*/
 //         BcdSub(( u8 *)pbcSrc1, pTemp, len);
 //         memcpy(pbcDest, pbcSrc1, len);
            memcpy(pTemp1, pbcSrc1, siBcdSrc1len);
            BcdSub(pTemp1, pTemp, len);
            memcpy(pbcDest, pTemp1, len);
 /*====================== END======================== */
        }
//        Verify(xgd_free(pTemp) >= 0);
//        Verify(xgd_free(pTemp1) >= 0);
        sdkFreeMem(pTemp);
        sdkFreeMem(pTemp1);
    }
    return len;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkCalcCUSUM(const u8 *pucSrc , s32 siNum)
   ��������: �����������ݸ��ֽڵ��ۼӺ�
   �������:const u8 *pucSrc:ԭ����; s32 siNum:�������ֽڸ���
   �������: ��
   ��   ��  ֵ: ��ȷ��ؼ����ۼӺͺ�Ľ��ֵ
                         ����ָ��Ϊ�ջ��ߴ������Ϊ0���SDK_PARA_ERR
   �޸ı�ע: ���㴫������ӵ�һ�ֽڿ�ʼsiNum���Լ����ۼӺ�
   ����ʱ��: 2012.05.11 11:04:55
*******************************************************************/
s32 sdkCalcCUSUM(const u8 *pucSrc, s32 siNum)
{
    s32 i = 0;                                                                                    //��ʱ����
    u8 bcc = 0;                                                 //�ۼӺ�

    if((NULL == pucSrc) || (siNum < 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for(i = 0; i < siNum; i++)
    {
		bcc += pucSrc[i];                                                                       //����У���
    }

    return bcc;                                                                                     //����У���
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s16 sdkCalcCrc16(const u8 *pucSrc, s32 siSrclen)
   ��������: ��������������ݵ�16λCRC
   �������: const u8 *pucSrc :������CRCֵ��һ����; s32 siSrclen: ���ݳ���
   �������:��
   ��   ��  ֵ: ��ȷ��ؼ���CRC��Ľ��ֵ
                         ����ָ��Ϊ�ջ��ߴ������ݳ���Ϊ0���SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.15 08:52:30
*******************************************************************/
s32 sdkCalcCrc16(const u8 *pucSrc, s32 siSrclen)
{
    u16 crcdata = 0;

    if((NULL == pucSrc) || (siSrclen <= 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    while(siSrclen > 0)
    {
        crcdata = (crcdata << 8) ^ CRCTAB[(crcdata >> 8) ^ (*pucSrc)];
        siSrclen--;
        pucSrc++;
    }

    return crcdata;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkE90S(u8 *pheDest,const u8 *pheSrc,s32 siLen,const u8 *pheWk, enum SDK_ ENCRYPT _MODE eMode)
   ��������: ��׼�й���������MAC������㷨
   �������: const u8 *pheSrc :����������,s32 siLen:����,const u8 *pheWk:��Կ ,enum SDK_ENCRYPT_MODE eMode:Des����:
   �������: u8 *pheDest:���8λ�������
   ��   ��  ֵ: ��ȷ���SDK_OK
                         ��������ָ��Ϊ�ջ��ߴ��볤��Ϊ0���SDK_PARA_ERR
   �޸ı�ע: ֧��DES��3DES
   ����ʱ��: 2012.05.15 10:35:06
*******************************************************************/
s32 sdkE90S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode)
{
//    u8 mac[256];
    u8 *mac;

    if(NULL == pheSrc || NULL == pheWk || (NULL == pheDest) || (siLen <= 0))
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    mac = (u8 *)sdkGetMem(256);
    if(mac == NULL)
    {
        Assert(0);
        return SDK_ERR;
    }
    memset(mac, 0, 256);
    sdkEcb(mac, pheSrc, siLen);

    if(eMode == SDK_DES)
    {
        sdkDesS(1, mac, pheWk);
    }
    else
    {
        sdkDes3S(1, mac, pheWk);
    }
    sdkXOR8(mac, mac, &mac[8]);

    if(eMode == SDK_DES)
    {
        sdkDesS(1, mac, pheWk);
    }
    else
    {
        sdkDes3S(1, mac, pheWk);
    }
    sdkBcdToAsc(pheDest, mac, 4);
    sdkFreeMem(mac);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkE98S(u8 *pheDest , const u8 *phePin , const u8 *phePan , const u8 *pheWk , enum SDK_ENCRYPT_MODE eMode)
   ��������: X9.8�����ʺ�PIN�����
   �������: const u8 *phePin:��������,onst u8 *phePan:����8���ʺ�(����������ʺ�,������ȫ0)
                   const u8 *pheWk:��Կ,enum SDK_ENCRYPT_MODE eMode:��Կ����
   �������: u8 *pheDest:8λ���
   ��   ��  ֵ: ��ȷ���SDK_OK
                         ��������ָ��Ϊ�����SDK_PARA_ERR
                        �������SDK_ERR��
   �޸ı�ע: ֧��DES��3DES,��������Կ����
   ����ʱ��: 2012.05.15 10:35:06
*******************************************************************/
s32 sdkE98S(u8 *pheDest, const u8 *phePin, const u8 *phePan, const u8 *pheWk, SDK_ENCRYPT_MODE eMode)
{
    u8 i = 0;
    u8 PINBLOCK[9] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0};
    u8 temphePin[12 + 1] = {0};

    if(NULL == phePin || NULL == phePan || NULL == pheWk || NULL == pheDest)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temphePin, 0, sizeof(temphePin));

    i = strlen(phePin);                                                                 //ȡ���볤��
    memcpy(temphePin, phePin, sizeof(temphePin));       //

    if(i >= 4 && i <= 12)                                                               //�������
    {
        PINBLOCK[0] = i;                                                                        //����
        sdkAscToBcd(PINBLOCK + 1, temphePin, i);                                //ת����BCD��

        if( (i % 2) != 0)                                                                       //����
        {
            PINBLOCK[(i / 2) + 1] |= 0x0F;                                      //ȷ��Ϊ0x0F
        }
        sdkXOR8(temphePin, phePan, PINBLOCK);                                                   //����PINBLOCK
    }
    else
    {
        return SDK_ESC;                                                                         //�����ʽ������
    }

    if(eMode == SDK_DES)
    {
        sdkDesS(1, temphePin, pheWk);
    }
    else
    {
        sdkDes3S(1, temphePin, pheWk);
    }
    memcpy(pheDest, temphePin, 8);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkE99S(u8 *pheDest,const u8 *pheSrc,s32 siLen,const u8 *pheWk, SDK_ ENCRYPT _MODE emode)
   ��������: X9.9������㷨
   �������: const u8 *pheSrc:��������, s32 siLen:���������ݳ���,
                   const u8 *pheWk:��Կ,enum SDK_ENCRYPT_MODE emode:��Կ����
   �������: u8 *pheDest:8λ���
   ��   ��  ֵ: ��ȷ���SDK_OK
                         ��������ָ��Ϊ�ջ��߳���Ϊ0���SDK_PARA_ERR
   �޸ı�ע: ֧��DES��3DES,��������Կ����
   ����ʱ��: 2012.05.15 10:35:06
*******************************************************************/
s32 sdkE99S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode)
{
    u8 temp[8] = {0}, A0[8] = {0}, A1[8] = {0};
    u16 i, j;

    if(NULL == pheSrc || NULL == pheWk || NULL == pheDest || siLen <= 0)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(A0, 0, sizeof(A0));
    memset(A1, 0, sizeof(A1));

    for(i = 0; i < ((siLen + 7) / 8); i++)
    {
        memset(temp, 0, sizeof(temp));

        if(((i + 1) * 8) > siLen)
        {
            j = siLen - ((siLen / 8) * 8);
        }
        else
        {
            j = 8;
        }
        memcpy(temp, &pheSrc[i * 8], j);
        sdkXOR8(A1, A0, temp);

        if(eMode == SDK_DES)
        {
            sdkDesS(1, A1, pheWk);
        }
        else
        {
            sdkDes3S(1, A1, pheWk);
        }
        memcpy(A0, A1, 8);
    }

    memcpy(pheDest, A0, 8);

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkE919S(u8 *pheDest,const u8 *pheSrc,s32 siLen,const u8 *pheWk, SDK_ ENCRYPT _MODE emode)
   ��������: X9.19������㷨
   �������: const u8 *pheSrc:��������,s32 siLen:���������ݳ���,
                    const u8 *pheWk: ��Կ,enum SDK_ENCRYPT_MODE emode:��Կ����
   �������: u8 *pheDest:8λ���
   ��   ��  ֵ: ��ȷ���SDK_OK
                         ��������ָ��Ϊ�ջ��߳���Ϊ0���SDK_PARA_ERR
   �޸ı�ע: ֧��DES��3DES,��������Կ����
   ����ʱ��: 2012.05.15 10:35:06
*******************************************************************/
s32 sdkE919S(u8 *pheDest, const u8 *pheSrc, s32 siLen, const u8 *pheWk, SDK_ENCRYPT_MODE eMode)
{
    u8 temp[8] = {0};
    
    if(NULL == pheSrc || NULL == pheWk || NULL == pheDest || siLen <= 0)
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(temp, 0, sizeof(temp));
    sdkE99S(temp, pheSrc, siLen, pheWk, SDK_DES);//fusuipu 2013.08.29 10:42
    TraceHex("zhouzhihua", "++++sdkE99S++++", temp, 8);
    if(eMode == SDK_DES)
    {
        sdkDesS(1, temp, pheWk);
    }
    else
    {
        sdkDes3S(1, temp, pheWk);
    }
    sdkBcdToAsc(pheDest, temp, 4);

    return SDK_OK;
}

#endif

