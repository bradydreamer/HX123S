#ifndef __SM2CHECK_H__
#define __SM2CHECK_H__

#ifdef __cplusplus  
extern "C" {  
#endif

/*****************************************************************************
 Function: InitElliptic
 Description:
    初始化椭圆曲线参数
 Parameter:
 int:
	const char *pGroupP        Gp坐标值
	const char *pGroupA        Ga坐标值
	const char *pGroupB        Gb坐标值
    const char *pGroupX        Gx坐标值
    const char *pGroupY        Gy坐标值
	const char *pGroupN        Gn坐标值
 out:
    NA 
 Return:
    == 1    成功
	!= 1    失败
 Create by chenfeng 20130307
Comment:
1.每个入参值必须是ASC码，并且有结束符
*****************************************************************************/
int InitElliptic(const char *pGroupP, const char *pGroupA, const char *pGroupB, const char *pGroupX,const char *pGroupY, const char *pGroupN);
/*****************************************************************************
 Function: SM3Encrypt
 Description:
    SM3加密
 Parameter:
 int:
    unsigned char *pInput      待加密数据
    unsigned short usLen       数据长度
 out:
    unsigned char *pOutput     加密后数据 
 Return:
    == 32    加密成功后固定长度
	!= 32    失败
 Create by chenfeng 20130307
Comment:如果待签名数据内容为空，入参pIDA当成已经SM3加密过后的待验证数据并进行直接验证，否则通过pIDA和pSrcData进行SM3运行后再验证
*****************************************************************************/
int SM3Encrypt(unsigned char *pInput, unsigned short usLen, unsigned char *pOutput);
/*****************************************************************************
 Function: CheckSM2Sign
 Description:
    校验SM2签名合法性
 Parameter:
 int:
    const char *pPkeyX         公钥X坐标值
    const char *pPkeyY         公钥Y坐标值
    const char *pSignR         签名R值
	const char *pSignS         签名S值
	const char *pSrcData       待签名数据
	const char *pIDA           用户身份
 out:
    NA
 Return:
    ==1   成功
    !=1   失败
 Create by chenfeng 20130307
Comment:
1.如果待签名数据内容为空，入参pIDA当成已经SM3加密过后的待验证数据并直接验证，否则通过pIDA和pSrcData进行SM3运算后再验证
2.每个入参值必须是ASC码，并且有结束符
*****************************************************************************/
int  CheckSM2Sign(const char *pPkeyX, const char *pPkeyY, const char *pSignR, const char *pSignS,const char *pSrcData, const char *pIDA);

/*****************************************************************************
 Function: ClearAll
 Description:
    释放空间
 Parameter:
 int:
    NA
 out:
    NA
 Return:
    NA
 Create by chenfeng 20130307
Comment:
验签完后必须释放空间，否则内存泄漏
*****************************************************************************/
void ClearAll();

#ifdef __cplusplus  
}  
#endif 

#endif

