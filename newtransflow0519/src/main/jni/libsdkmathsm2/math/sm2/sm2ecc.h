// \file:sm2.h
//SM2 Algorithm
//2011-11-09
//author:goldboar
//email:goldboar@163.com
//comment:2011-11-10 sm2-sign-verify sm2-dh

#ifndef __SM2ECC_H__
#define __SM2ECC_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bn.h"
#include "ec.h"
//#include "rand.h"
//#include "err.h"
#include "ecdsa.h"
//#include "ecdh.h>

//#include "err.h"

//#include <openssl/ec_lcl.h>


/*
 * This file implements the wNAF-based interleaving multi-exponentation method
 * (<URL:http://www.informatik.tu-darmstadt.de/TI/Mitarbeiter/moeller.html#multiexp>);
 * for multiplication with precomputation, we use wNAF splitting
 * (<URL:http://www.informatik.tu-darmstadt.de/TI/Mitarbeiter/moeller.html#fastexp>).
 */




/* structure for precomputed multiples of the generator */
typedef struct ec_pre_comp_st {
	const EC_GROUP *group; /* parent EC_GROUP object */
	size_t blocksize;      /* block size for wNAF splitting */
	size_t numblocks;      /* max. number of blocks for which we have precomputation */
	size_t w;              /* window size */
	EC_POINT **points;     /* array with pre-calculated multiples of generator:
	                        * 'num' pointers to EC_POINT objects followed by a NULL */
	size_t num;            /* numblocks * 2^(w-1) */
	int references;
} EC_PRE_COMP;

int AppEC_POINTs_mul(const EC_GROUP *group, EC_POINT *r, const BIGNUM *scalar,
	size_t num, const EC_POINT *points[], const BIGNUM *scalars[], BN_CTX *ctx);

int AppEC_POINT_mul(const EC_GROUP *group, EC_POINT *r, const BIGNUM *g_scalar,
	const EC_POINT *point, const BIGNUM *p_scalar, BN_CTX *ctx);

//SM2_sign_setup
int  SM2_sign_setup(EC_KEY *eckey, BN_CTX *ctx_in, BIGNUM **kinvp, BIGNUM **rp);

//SM2_sign_ex
int	  SM2_sign_ex(int type, const unsigned char *dgst, int dlen, unsigned char 
	*sig, unsigned int *siglen, const BIGNUM *kinv, const BIGNUM *r, 
	EC_KEY *eckey);
//SM2_sign
int	  SM2_sign(int type, const unsigned char *dgst, int dlen, unsigned char 
		*sig, unsigned int *siglen, EC_KEY *eckey);

//SM2_verify
//int SM2_verify(int type, const unsigned char *dgst, int dgst_len,
//		const unsigned char *sigbuf, int sig_len, EC_KEY *eckey);
//SM2_verify
int SM2_verify1(int type, const unsigned char *dgst, int dgst_len,
				const ECDSA_SIG *sig, EC_KEY *eckey);
//SM2 DH, comupting shared point
int SM2_DH_key(const EC_GROUP * group, const EC_POINT *b_pub_key_r, const EC_POINT *b_pub_key, const BIGNUM *a_r,EC_KEY *a_eckey,
			   unsigned char *outkey,size_t keylen);
#endif

