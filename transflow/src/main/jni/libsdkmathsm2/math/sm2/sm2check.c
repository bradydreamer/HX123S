//#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bn.h"
#include "ec.h"

//#include "err.h"
#include "ecdsa.h"
//#include "ecdh.h"
#include "sm2check.h"
#include "sm2ecc.h"

//#define ABORT do { \
//	fflush(stdout); \
//	fprintf(stderr, "%s:%d: ABORT\n", __FILE__, __LINE__); \
//	ERR_print_errors_fp(stderr); \
//	exit(1); \
//} while (0)
#define u8 unsigned char
#define u32 unsigned int
#define u16 unsigned short

#define FFA(x, y, z) ( (x) ^ (y) ^ (z))
#define FFB(x, y, z) (((x) & (y)) | ( (x) & (z)) | ( (y) & (z)))

#define GG0(x, y, z) ( (x) ^ (y) ^ (z))
#define GG1(x, y, z) (((x) & (y)) | ( (~(x)) & (z)) )


#define  SHL(x, n) (((x) & 0xFFFFFFFF) << n)
#define ROTL(x, n) (SHL((x), n) | ((x) >> (32 - n)))

#define P0(x) ((x) ^  ROTL((x), 9) ^ ROTL((x), 17))
#define P1(x) ((x) ^  ROTL((x), 15) ^ ROTL((x), 23))

static const char GroupP[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFF";
static const char GroupA[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF00000000FFFFFFFFFFFFFFFC";
static const char GroupB[] = "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93";
static const char GroupX[] = "32C4AE2C1F1981195F9904466A39C9948FE30BBFF2660BE1715A4589334C74C7";
static const char GroupY[] = "BC3736A2F4F6779C59BDCEE36B692153D0A9877CC62A474002DF32E52139F0A0";
static const char GroupN[] = "FFFFFFFEFFFFFFFFFFFFFFFFFFFFFFFF7203DF6B21C6052B53BBF40939D54123";

//static const char GroupP[] = "8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3";
//static const char GroupA[] = "787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498";
//static const char GroupB[] = "63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A";
//static const char GroupX[] = "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D";
//static const char GroupY[] = "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2";
//static const char GroupN[] = "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7";

static const char rnd_seed[] = "string to make the random number generator think it has entropy";
static EC_KEY	*g_pEckey = NULL;
static BN_CTX   *g_pCtx = NULL;
static EC_GROUP *g_pGroup = NULL;
//static int  InitGroupData(EC_GROUP *pGroup);
static int InitGroupData(const char *pGroupP, const char *pGroupA, const char *pGroupB, const char *pGroupX,const char *pGroupY, const char *pGroupN);
static int  GetSummary (const char *pPkeyX, const char *pPkeyY,const char *pSrcData, const char *pIDA, u8 *pSummary);
//static void U32ToBcd(u8 *Bcd, u32 const Src,u32 Len);//shijianglong 2013.05.28 17:52
static void BcdToAsc(u8 *Dest,u8 *Src,u32 Len);
static void AscToBcd(u8 *Dest,u8 *Src,u32 Len);
static void U32ToHex(u8 *Hex, u32 const Src,u32 Len);
static void HexToU32(u32 *Dest,u8 *Hex,u32 Len);
static void sm3padding(u8 *input, u16 inlen, u8 *output, u16 *outlen);

//void U32ToBcd(u8 *Bcd, u32 const Src,u32 Len)//shijianglong 2013.05.28 17:52
//{
//	u32  i = 0;
//	u32 num = Src;
//	for(i = Len; i > 0; i--)
//	{
//		*(Bcd+i-1) = (((num%100) / 10) << 4) | ((num%100) % 10);
//		num /= 100;
//	}
//}

int InitGroupData(const char *pGroupP, const char *pGroupA, const char *pGroupB, const char *pGroupX,const char *pGroupY, const char *pGroupN)
{
	int nRet = 0;
	BIGNUM *p = NULL, *a = NULL, *b = NULL;
	EC_POINT *P = NULL, *Q = NULL;
	BIGNUM *x = NULL, *y = NULL, *z = NULL;
	if (!pGroupP || !pGroupA || !pGroupB || !pGroupX || !pGroupY || !pGroupN) 
	{
		goto builtin_err;
	}
	if (!g_pCtx || !g_pGroup) goto builtin_err;

	/* Curve SM2 (Chinese National Algorithm) */
	//http://www.oscca.gov.cn/News/201012/News_1197.htm
	p = BN_new();
	a = BN_new();
	b = BN_new();
	if (!p || !a || !b) goto builtin_err;

	if (!BN_hex2bn(&p, pGroupP)) goto builtin_err;
//	if (1 != BN_is_prime_ex(p, BN_prime_checks, g_pCtx, NULL)) goto builtin_err;
	if (!BN_hex2bn(&a, pGroupA)) goto builtin_err;
	if (!BN_hex2bn(&b, pGroupB)) goto builtin_err;
	if (!EC_GROUP_set_curve_GFp(g_pGroup, p, a, b, g_pCtx)) goto builtin_err;

	P = EC_POINT_new(g_pGroup);
	Q = EC_POINT_new(g_pGroup);
	if (!P || !Q) goto builtin_err;

	x = BN_new();
	y = BN_new();
	z = BN_new();
	if (!x || !y || !z) goto builtin_err;

	// sm2 testing P256 Vetor
	// p：8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
	// a：787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
	// b：63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
	// xG 421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
	// yG 0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
	// n: 8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7

	if (!BN_hex2bn(&x, pGroupX)) goto builtin_err;
	if (!EC_POINT_set_compressed_coordinates_GFp(g_pGroup, P, x, 0, g_pCtx)) goto builtin_err;
	if (!EC_POINT_is_on_curve(g_pGroup, P, g_pCtx)) goto builtin_err;
	if (!BN_hex2bn(&z, pGroupN)) goto builtin_err;
	if (!EC_GROUP_set_generator(g_pGroup, P, z, BN_value_one())) goto builtin_err;

	if (!EC_POINT_get_affine_coordinates_GFp(g_pGroup, P, x, y, g_pCtx)) goto builtin_err;
	/* G_y value taken from the standard: */
	if (!BN_hex2bn(&z, pGroupY)) goto builtin_err;
	if (0 != BN_cmp(y, z)) goto builtin_err;

	if (EC_GROUP_get_degree(g_pGroup) != 256) goto builtin_err;

	if (!EC_GROUP_get_order(g_pGroup, z, g_pCtx)) goto builtin_err;
	//if (!EC_GROUP_precompute_mult(g_pGroup, g_pCtx)) goto builtin_err;
	if (!EC_POINT_mul(g_pGroup, Q, z, NULL, NULL, g_pCtx)) goto builtin_err;
	if (!EC_POINT_is_at_infinity(g_pGroup, Q)) goto builtin_err;
	nRet = 1;
builtin_err:	
	EC_POINT_free(P);
	EC_POINT_free(Q);
	BN_free(p);
	BN_free(a);
	BN_free(b);
	BN_free(x);
	BN_free(y);
	BN_free(z);
	return nRet;
}
//int InitGroupData(EC_GROUP *pGroup, BN_CTX *pCtx)
//{
//	int nRet = 0;
//	BIGNUM *p = NULL, *a = NULL, *b = NULL;
//	EC_POINT *P = NULL, *Q = NULL;
//	BIGNUM *x = NULL, *y = NULL, *z = NULL;
//	if (!pCtx || !pGroup) goto builtin_err;
//
//	/* Curve SM2 (Chinese National Algorithm) */
//	//http://www.oscca.gov.cn/News/201012/News_1197.htm
//	p = BN_new();
//	a = BN_new();
//	b = BN_new();
//	if (!p || !a || !b) goto builtin_err;
//	
//	if (!BN_hex2bn(&p, GroupP)) goto builtin_err;
//	if (1 != BN_is_prime_ex(p, BN_prime_checks, pCtx, NULL)) goto builtin_err;
//	if (!BN_hex2bn(&a, GroupA)) goto builtin_err;
//	if (!BN_hex2bn(&b, GroupB)) goto builtin_err;
//	if (!EC_GROUP_set_curve_GFp(pGroup, p, a, b, pCtx)) goto builtin_err;
//
//	P = EC_POINT_new(pGroup);
//	Q = EC_POINT_new(pGroup);
//	if (!P || !Q) goto builtin_err;
//
//	x = BN_new();
//	y = BN_new();
//	z = BN_new();
//	if (!x || !y || !z) goto builtin_err;
//
//	// sm2 testing P256 Vetor
//	// p：8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3
//	// a：787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498
//	// b：63E4C6D3B23B0C849CF84241484BFE48F61D59A5B16BA06E6E12D1DA27C5249A
//	// xG 421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D
//	// yG 0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2
//	// n: 8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7
//
//	if (!BN_hex2bn(&x, GroupX)) goto builtin_err;
//	if (!EC_POINT_set_compressed_coordinates_GFp(pGroup, P, x, 0, pCtx)) goto builtin_err;
//	if (!EC_POINT_is_on_curve(pGroup, P, pCtx)) goto builtin_err;
//	if (!BN_hex2bn(&z, GroupN)) goto builtin_err;
//	if (!EC_GROUP_set_generator(pGroup, P, z, BN_value_one())) goto builtin_err;
//	
//	if (!EC_POINT_get_affine_coordinates_GFp(pGroup, P, x, y, pCtx)) goto builtin_err;
//	/* G_y value taken from the standard: */
//	if (!BN_hex2bn(&z, GroupY)) goto builtin_err;
//	if (0 != BN_cmp(y, z)) goto builtin_err;
//	
//	if (EC_GROUP_get_degree(pGroup) != 256) goto builtin_err;
//	
//	if (!EC_GROUP_get_order(pGroup, z, pCtx)) goto builtin_err;
//	if (!EC_GROUP_precompute_mult(pGroup, pCtx)) goto builtin_err;
//	if (!EC_POINT_mul(pGroup, Q, z, NULL, NULL, pCtx)) goto builtin_err;
//	if (!EC_POINT_is_at_infinity(pGroup, Q)) goto builtin_err;
//	nRet = 1;
//builtin_err:	
//	EC_POINT_free(P);
//	EC_POINT_free(Q);
//	BN_free(p);
//	BN_free(a);
//	BN_free(b);
//	BN_free(x);
//	BN_free(y);
//	BN_free(z);
//	return nRet;
//}
void BcdToAsc(u8 *Dest,u8 *Src,u32 Len)
{
	u32 i;
	for(i=0;i<Len;i++)
	{
		//高Nibble转换
		if(((*(Src + i) & 0xF0) >> 4) <= 9)
		{
			*(Dest + 2*i) = ((*(Src + i) & 0xF0) >> 4) + 0x30;
		}
		else
		{
			*(Dest + 2*i)  = ((*(Src + i) & 0xF0) >> 4) + 0x37;   //大写A~F
		}    
		//低Nibble转换
		if((*(Src + i) & 0x0F) <= 9)
		{
			*(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x30;
		}
		else
		{
			*(Dest + 2*i + 1) = (*(Src + i) & 0x0F) + 0x37;   //大写A~F
		}    
	}
}
void AscToBcd(u8 *Dest,u8 *Src,u32 Len)
{
	u32 i;
	u8 high = 0,low = 0;
	for(i = 0; i < Len; i++) 
	{
		//待转bcd码高Nibble
		if((*(Src + i) >= 0x61) && (*(Src + i) <= 0x66))      //range a~f
		{
			high = *(Src + i) - 0x57;
		}
		else if((*(Src + i) >= 0x41) && (*(Src + i) <= 0x46))  //range A~F
		{
			high = *(Src + i) - 0x37;
		}
		else if((*(Src + i) >= 0x30) && (*(Src + i) <= 0x39))  //range 0~9
		{
			high = *(Src + i) - 0x30;   
		}
		else
		{
			high = 0x00 ;                                       //其他
		}

		//待转bcd码低Nibble
		i++;
		if(i < Len)
		{
			if((*(Src + i) >= 0x61) && (*(Src + i) <= 0x66))    //range a~f
			{
				low = *(Src + i) - 0x57;
			}
			else if((*(Src + i) >= 0x41) && (*(Src + i) <= 0x46)) //range A~F
			{   
				low = *(Src + i) - 0x37;
			}
			else if((*(Src + i) >= 0x30) && (*(Src + i) <= 0x39))  //range 0~9
			{
				low = *(Src + i) - 0x30;
			}
			else
			{
				low = 0x00 ;                                       //其他
			}
		}
		else
		{
			i--;                                                //预防255个时溢出出错
			low = 0x00 ;                                       //如果是奇数个末尾补0x00 
		}
		*(Dest + i/2) = (high << 4) | low;                      //合并BCD码
	}
}
void U32ToHex(u8 *Hex, u32 const Src,u32 Len)
{
	u32 i;
	u32 num = Src;

	for(i = Len; i > 0; i--)
	{
		*(Hex+i-1) = num % 256;
		num /= 256;
	}
}
void HexToU32(u32 *Dest,u8 *Hex,u32 Len)
{
	u32 i;
	*Dest = 0;

	for(i = 0; i < Len; i++)
	{
		*Dest *= 256;
		*Dest += Hex[i];
	}
}
void sm3padding(u8 *input, u16 inlen, u8 *output, u16 *outlen)
{
    u16 i;
    u8 temp[128];
    u32 len;

	len = inlen * 8;
	memset(temp, 0, sizeof(temp));
	temp[0] = 0x80;

	i = (inlen + 1 + 8) % 64;

    if(i != 0)
    {
        i = 64 - i;
    }
    i += 9;
    U32ToHex(&temp[i - 8], len, 8);
    memcpy(output, temp, i);
    *outlen = i;
}
int SM3Encrypt(u8 *pInput, u16 ilen, u8 *pOutput)
{
	u8 temp[128], buf[64];
	u16 len;
	u32 SS1, SS2, TT1, TT2, W[68], W1[64];
	u32 A, B, C, D, E, F, G, H;
	u32 AA, BB, CC, DD, EE, FF, GG, HH;
	u32 Tt[64];
	u32 Temp1, Temp2, Temp3, Temp4, Temp5;
	u16 j, k, l,usValue;
    u8 flg;
	if (!pInput || !pOutput)
	{
		return 0;
	}

	//初始值
	AA= 0x7380166F;
	BB= 0x4914B2B9;
	CC= 0x172442D7;
	DD= 0xDA8A0600;
	EE= 0xA96F30BC;
	FF = 0x163138AA;
	GG= 0xE38DEE4D;
	HH= 0xB0FB0E4E;
	A = AA;
	B = BB;
	C = CC;
	D = DD;
	E = EE;
	F = FF;
	G = GG;
	H = HH;


	for(j = 0; j < 16; j++)
	{
		Tt[j] = 0x79CC4519;
	}

	for(j = 16; j < 64; j++)
	{
		Tt[j] = 0x7A879D8A;
	}

	memset(temp, 0, sizeof(temp));
	memset(buf, 0, sizeof(buf));
	sm3padding(pInput, ilen, temp, &len);

	 flg = 0;
     l = 0;
	for(k = 0; k < ilen + len && l < len; k +=  64)
	{
        if(flg)
        {
            if(l + 64 <= len)
            {
                memcpy(buf, temp + l, 64);
                l += 64;
            }
            else
            {
                memcpy(buf, temp + l, len - l);
                l = len;
            }
        }
        else if((k + 64) > ilen)
		{
			flg = 1;
			memcpy(buf, pInput + k, ilen % 64);
			l = 64 - (ilen % 64);
			memcpy(buf + (ilen % 64), temp, l);
		}
		else
		{
			memcpy(buf, pInput + k, 64);
		}

		for(j = 0; j < 16; j++)
		{
			HexToU32(W + j, buf + (j * 4), 4);
		}

		for(j = 16; j < 68; j++ )
		{
			Temp1 = W[j - 16] ^ W[j - 9];
			Temp2 = ROTL(W[j - 3], 15);
			Temp3 = Temp1 ^ Temp2;
			Temp4 = P1(Temp3);
			Temp5 =  ROTL(W[j - 13], 7) ^ W[j - 6];
			W[j] = Temp4 ^ Temp5;
		}

		for(j =  0; j < 64; j++)
		{
			W1[j] = W[j] ^ W[j + 4];
		}

		for(j = 0; j < 16; j++)
		{
			SS1 = ROTL((ROTL(A, 12) + E + ROTL(Tt[j], j)), 7);
			SS2 = SS1 ^ ROTL(A, 12);
			TT1 = FFA(A, B, C) + D + SS2 + W1[j];
			TT2 = GG0(E, F, G) + H + SS1 + W[j];
			D = C;
			C = ROTL(B, 9);
			B = A;
			A = TT1;
			H = G;
			G = ROTL(F, 19);
			F = E;
			E = P0(TT2);
		}

		for(j = 16; j < 64; j++)
		{
		    usValue = j % 32;
			SS1 = ROTL((ROTL(A, 12) + E + ROTL(Tt[j], usValue)), 7);
			SS2 = SS1 ^ ROTL(A, 12);
			TT1 = FFB(A, B, C) + D + SS2 + W1[j];
			TT2 = GG1(E, F, G) + H + SS1 + W[j];
			D = C;
			C = ROTL(B, 9);
			B = A;
			A = TT1;
			H = G;
			G = ROTL(F, 19);
			F = E;
			E = P0(TT2);
		}

		A ^= AA;
		B ^= BB;
		C ^= CC;
		D ^= DD;
		E ^= EE;
		F ^= FF;
		G ^= GG;
		H ^= HH;
		AA= A;
		BB= B;
		CC= C;
		DD= D;
		EE= E;
		FF= F;
		GG= G;
		HH= H;
	}

	U32ToHex(pOutput, A, 4);
	U32ToHex(pOutput + 4, B, 4);
	U32ToHex(pOutput + 8, C, 4);
	U32ToHex(pOutput + 12, D, 4);
	U32ToHex(pOutput + 16, E, 4);
	U32ToHex(pOutput + 20, F, 4);
	U32ToHex(pOutput + 24, G, 4);
	U32ToHex(pOutput + 28, H, 4);
	return 32;
}

int  GetSummary (const char *pPkeyX, const char *pPkeyY,const char *pSrcData, const char *pIDA, u8 *pSummary)
{
	int nLenSum = 0;
	int nSrcLen = 0;
	int nIDALen = 0;
	int nLenKeyX = 0;
	int nLenKeyY = 0;
	u8 *pTmp = NULL;
	u8 *pZaTmp = NULL;
	int nLen = 0;
	if ((!pSrcData && !pIDA) ||!pSummary)
	{
		return nLenSum;
	}
	nSrcLen = strlen(pSrcData);
	nIDALen = strlen(pIDA);
	if (nSrcLen == 0 && nIDALen == 0)
	{
		return nLenSum;
	}
	nLenKeyX = strlen(pPkeyX);
	nLenKeyY = strlen(pPkeyY);
	if (!pSrcData || nSrcLen == 0)
	{
		AscToBcd(pSummary, (u8*)pIDA, nIDALen);
		nLenSum = (nIDALen + 1) / 2;
		return nLenSum;
	}
	if (nSrcLen < 512)
	{
		nLen = 512+66;
	}
	else
	{
		nLen = nSrcLen + 66;
	}
	pTmp = (u8*)malloc(sizeof(u8) * nLen);
	if (!pTmp)
	{
		return 0;
	}
	pZaTmp = (u8*)malloc(sizeof(u8) * nLen/2);
	if (!pZaTmp)
	{
		if (pZaTmp)
		{
			free(pZaTmp);
			pZaTmp = NULL;
		}
		return 0;
	}
	nLen = 0;
	memcpy(pTmp, pIDA, nIDALen);
	nLen += nIDALen;
	memcpy(pTmp + nLen, GroupA, 64);
	nLen += 64;

	memcpy(pTmp + nLen, GroupB, 64);
	nLen += 64;

	memcpy(pTmp + nLen, GroupX, 64);
	nLen += 64;

	memcpy(pTmp + nLen, GroupY, 64);
	nLen += 64;

	memcpy(pTmp + nLen, pPkeyX, nLenKeyX);
	nLen += nLenKeyX;

	memcpy(pTmp + nLen, pPkeyY, nLenKeyY);
	nLen += nLenKeyY;

	AscToBcd(pZaTmp, pTmp, nLen);
	nLen = (nLen + 1) / 2;
	if (nIDALen != 0)
	{
		U32ToHex(pTmp, nIDALen *4, 2);
		memcpy(pTmp + 2, pZaTmp, nLen);
		nLen += 2;
	}
	else
	{
		memcpy(pTmp, pZaTmp, nLen);
	}
	
	pTmp[nLen] = '\0';
	nLenSum = SM3Encrypt(pTmp, nLen, pZaTmp);

	BcdToAsc(pTmp, pZaTmp, nLenSum);
	nLen = 2 * nLenSum;

	memcpy(pTmp + nLen, pSrcData, nSrcLen);
	nLen += nSrcLen;

	AscToBcd(pZaTmp, pTmp, nLen);
	nLen = (nLen + 1) / 2;

	nLenSum = SM3Encrypt(pZaTmp, nLen, pTmp);
	memcpy(pSummary, pTmp, nLenSum);
	if (pTmp)
	{
		free(pTmp);
		pTmp = NULL;
	}
	if (pZaTmp)
	{
		free(pZaTmp);
		pZaTmp = NULL;
	}
	return nLenSum;
}
int InitElliptic(const char *pGroupP, const char *pGroupA, const char *pGroupB, const char *pGroupX,const char *pGroupY, const char *pGroupN)
{
	int nRet = 0;
//	CRYPTO_set_mem_debug_functions(0, 0, 0, 0, 0);
//	CRYPTO_mem_ctrl(CRYPTO_MEM_CHECK_ON);
	if (!pGroupP || !pGroupA || !pGroupB || !pGroupX || !pGroupY || !pGroupN) 
	{
		return nRet;
	}
	if (!g_pEckey)
	{
		if ((g_pEckey = EC_KEY_new()) == NULL)
			return nRet;
	}
	if (!g_pCtx)
	{
		g_pCtx = BN_CTX_new();
		if (!g_pCtx) return nRet;
	}
	if (!g_pGroup)
	{
		g_pGroup = EC_GROUP_new(EC_GFp_mont_method());
		if (!g_pGroup)
		{
			return nRet;
		}
	}
	if (!InitGroupData(pGroupP, pGroupA, pGroupB, pGroupX,pGroupY, pGroupN))
	{
		return nRet;
	}
	if (EC_KEY_set_group(g_pEckey, g_pGroup) == 0)
	{
		return nRet;
	}
	nRet = 1;
	return nRet;
}
int CheckSM2Sign(const char *pPkeyX, const char *pPkeyY, const char *pSignR, const char *pSignS,const char *pSrcData, const char *pIDA)
{
	int nRet = 0;
	//BN_CTX *pCtx = NULL;
	//EC_GROUP *pGroup = NULL;
	//EC_KEY	*pEckey = NULL;
	EC_POINT *P = NULL, *R = NULL;
	BIGNUM *x = NULL, *y = NULL;
	ECDSA_SIG *pEcsig = NULL;
	u8 pDstBuf[512] = {0};
	int nLen = 0;
	
	if (!pPkeyX || !pPkeyY || !pSignR || !pSignS)
	{
		goto builtin_err;
	}
	//pCtx = BN_CTX_new();
	//if (!pCtx) goto builtin_err;
	//pGroup = EC_GROUP_new(EC_GFp_mont_method()); /* applications should use EC_GROUP_new_curve_GFp
 //	                                             * so that the library gets to choose the EC_METHOD */
	//if (!pGroup) goto builtin_err;
	//if (!InitGroupData(pGroup, pCtx))
	//{
	//	goto builtin_err;
	//}
	//if ((pEckey = EC_KEY_new()) == NULL)
	//	goto builtin_err;
	//if (EC_KEY_set_group(pEckey, pGroup) == 0)
	//{
	//	goto builtin_err;
	//}
	if (!g_pCtx || !g_pEckey || !g_pGroup)
	{
		if (!InitElliptic(GroupP, GroupA, GroupB, GroupX,GroupY, GroupN))
		{
			goto builtin_err;
		}
	}
	//pGroup = EC_KEY_get0_group(g_pEckey);
	//if (!pGroup)
	//{
	//	goto builtin_err;
	//}
	P = EC_POINT_new(g_pGroup);
	R = EC_POINT_new(g_pGroup);
	if (!P || !R) goto builtin_err;

	x = BN_new();
	y = BN_new();
	if (!x || !y ) goto builtin_err;
	if (!BN_hex2bn(&x, pPkeyX)) goto builtin_err;
	if (!BN_hex2bn(&y, pPkeyY)) goto builtin_err;
	if (!EC_POINT_set_affine_coordinates_GFp(g_pGroup,P, x, y, g_pCtx)) goto builtin_err;
	EC_KEY_set_public_key(g_pEckey, P);

	/* check key */
	if (!EC_KEY_check_key(g_pEckey))
	{
		goto builtin_err;
	}
	pEcsig = ECDSA_SIG_new();
	if (!pEcsig) goto builtin_err;
	if (!BN_hex2bn(&pEcsig->r, pSignR)) goto builtin_err;
	if (!BN_hex2bn(&pEcsig->s, pSignS)) goto builtin_err;
	nLen = GetSummary(pPkeyX, pPkeyY, pSrcData, pIDA, pDstBuf);
	if (nLen == 0 || nLen > 32)
	{
		goto builtin_err;
	}
	if (SM2_verify1(1, pDstBuf, nLen, pEcsig, g_pEckey) != 1)
	{
		goto builtin_err;
	}
	nRet = 1;
builtin_err:
	EC_POINT_free(P);
	EC_POINT_free(R);
	BN_free(x);
	BN_free(y);
	ECDSA_SIG_free(pEcsig);
	//EC_KEY_free(pEckey);
	//pEckey = NULL;
	//EC_GROUP_free(pGroup);
	//BN_CTX_free(pCtx);
	//CRYPTO_cleanup_all_ex_data();
	//ERR_remove_state(0);
	return nRet;
}

void ClearAll()
{
	EC_GROUP_free(g_pGroup);
	g_pGroup = NULL;
	EC_KEY_free(g_pEckey);
	g_pEckey = NULL;
	BN_CTX_free(g_pCtx);
	g_pCtx = NULL;
	CRYPTO_cleanup_all_ex_data();
//	ERR_remove_state(0);
}

