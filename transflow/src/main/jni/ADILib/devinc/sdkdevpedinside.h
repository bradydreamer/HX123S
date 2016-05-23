#ifndef _SDKDEVPEDINSIDE_H
#define _SDKDEVPEDINSIDE_H

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */
/*=======BEGIN: zhouzhihua 2013.07.25  15:2 modify===========*/
#define SDK_DUKPT_KSN_LEN 10
#define SDK_DUKPT_KEY_LEN 16


/*====================== END======================== */

/*=======BEGIN: taowz 2014.05.29  11:55 modify===========*/
#define  TMK_INDEX_AREA         0//主密钥存储区
#define  WK_INDEX_AREA          1//工作密钥存储区

typedef struct _strInnerTKeyDEncryptPara 
{ 
    const u8 *m_indata; //待加解密数据
    u8 *m_outdata; //加解密结果 
    u32 m_len; //待加解密数据长度,必须为8的倍数
} strInnerTKeyDEncryptPara;

typedef struct _strInnerHKeyDEncryptPara 
{   
    u32 m_area; //密钥区域
    u32 m_index; //密钥索引
    u32 m_half; //密钥前后半部
    const u8 *m_indata; //待加密数据
    u8 *m_outdata; //加密结果
    u32 m_len; //待加密数据长度
} strInnerHKeyDEncryptPara;
/*====================== END======================== */
extern s32 sdk_dev_ped_inside_update_wk( const u8 *ucData, s32 siLen, u8 *ucVaule, s32 siTimer);
extern s32 sdk_dev_ped_inside_flush( SDK_PED_KEY_TYPE eKeyType, u8 ucKeyIndex );
extern s32 sdk_dev_ped_inside_set_tmk_mode_transform_key(u16 uiMode , const u8 *ucKey , s32 siTimer);
extern s32 sdk_dev_ped_inside_update_one_tmk(s32 siIndex, const u8 *ucTmk, s32 siLen, s32 siTimer);
extern s32 sdk_dev_ped_inside_wkflush(u8 ucTempTpk  ,u8 ucTpkIndex , u8 ucTempTak ,u8 ucTakIndex , u8 *pucOutData, s32 siTimer);
extern s32 sdk_dev_ped_inside_wkencrypt_data(u16 uiWkIndex , u8 *pucData , u16 uiDataLen ,u8 *pucOutData , s32 siTimer);
extern s32 sdk_dev_ped_inside_wkdecrypt_data(u8 *pucInData , u16 uiInLen , u8 *pucOutData ,s32 siTimer);
extern s32 sdk_dev_ped_inside_wkcbc(u16 uiCmd, u8 ucwkIndex, u8 *pucInData,u16 uiInLen, u8 *pucOutData, u32 siTimer);
//extern s32 sdk_dev_ped_inside_dukpt( u8 *pucSend,s32 siSendLen,u8 *pucRec);

extern s32 sdk_dev_ped_inside_wk_cbc(s32 siCmd, u8 ucwkIndex, u8 const *pucInData, u16 uiInLen, u8 *pucOutData,  u32 siTimer);
extern s32 sdk_dev_ped_inside_wk_decrypt_data(s32 siWkIndex, const u8 *pucInData, s32 siInLen, u8 *pucOutData, s32 siTimer);
extern s32 sdk_dev_ped_inside_wk_encrypt_data(s32 siWkIndex, const u8 *pucData, s32 siDataLen, u8 *pucOutData, s32 siTimer);
/*=======BEGIN: taowz 2014.05.26  13:46 modify===========*/
extern s32 sdk_dev_ped_inside_temporarykey_edcrypt_data(s32 nCmd,  s32 siInlen, const u8 *pucInData, u8 *pucOutData);
extern s32 sdk_dev_ped_inside_hbyte_edcrypt_data(s32 nCmd, s32 nKeyArea, s32 nIndex, s32 nHalf, s32 siInlen, const u8 *pucInData, u8 *pucOutData);
extern s32 sdk_dev_ped_inside_edcrypt_data(u8 uccrypt, u32 nKeyArea, s32 siWkIndex, const u8 *pucData, s32 siDataLen, u8 *pucOutData, s32 siTimer);
extern s32 sdk_dev_ped_inside_open(void);
extern s32 sdk_dev_ped_inside_close(void);
extern s32 sdk_dev_ped_inside_version(u8 *pucOutData);

extern s32 sdk_dev_ped_inside_wk_encrypt_wk(u8 siSrWkIndex, u8 siTgWkIndex, u8 newWkIndex );

/*====================== END======================== */
#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif

