#include "rsaref.h"
#include "rsa.h"
#include "nn.h"
#include "sdkGlobal.h"

int RSAPublicBlock PROTO_LIST((unsigned char *, unsigned int *, unsigned char *, unsigned int,
                              R_RSA_PUBLIC_KEY *));
                  int RSAPrivateBlock
                  PROTO_LIST((unsigned char *, unsigned int *, unsigned char *, unsigned int,
                              R_RSA_PRIVATE_KEY *));

/*=======BEGIN: fusuipu 2013.01.05  16:50 modify===========*/
                  void R_memset(output, value, len)
                  POINTER output;                                             /* output block */
                  int value;                                                         /* value */
                  unsigned int len;                                        /* length of block */
{
    if (len)
        memset(output, value, len);
}

void R_memcpy(output, input, len)
        POINTER output;                                             /* output block */
        POINTER input;                                               /* input block */
        unsigned int len;                                       /* length of blocks */
{
    if (len)
        memcpy(output, input, len);
}

int R_memcmp(firstBlock, secondBlock, len)
        POINTER firstBlock;                                          /* first block */
        POINTER secondBlock;                                        /* second block */
        unsigned int len;                                       /* length of blocks */
{
    if (len)
        return (memcmp(firstBlock, secondBlock, len));
    else
        return (0);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	a
                               b
                               c
                               digits
** Returned value:	
** Created By:		fusuipu  2013.01.05
** Remarks: 		
*****************************************************************************/
NN_DIGIT NN_Add(a, b, c, digits)
        NN_DIGIT *a, *b, *c;
        unsigned int digits;
{
    NN_DIGIT ai, carry;
    unsigned int i;

    carry = 0;

    for (i = 0; i < digits; i++) {
        if ((ai = b[i] + carry) < carry)
            ai = c[i];
        else {
            if ((ai += c[i]) < c[i])
                carry = 1;
            else
                carry = 0;
        }
        a[i] = ai;
    }

    return (carry);
}

/*****************************************************************************
** Descriptions:	
** Parameters:    	block
                               blockLen
                               randomStruct
** Returned value:	
** Created By:		fusuipu  2013.01.05
** Remarks: 		
*****************************************************************************/
int R_GenerateBytes(block, blockLen, randomStruct)
        unsigned char *block;                                              /* block */
        unsigned int blockLen;                                   /* length of block */
        R_RANDOM_STRUCT *randomStruct;                          /* random structure */
{
    MD5_CTX context;
    unsigned int available, i;

    if (randomStruct->bytesNeeded)
        return (RE_NEED_RANDOM);

    available = randomStruct->outputAvailable;

    while (blockLen > available) {
        R_memcpy
                ((POINTER) block, (POINTER) & randomStruct->output[16 - available],
                 available);
        block += available;
        blockLen -= available;

        /* generate new output */
        MD5Init(&context);
        MD5Update(&context, randomStruct->state, 16);
        MD5Final(randomStruct->output, &context);
        available = 16;

        /* increment state */
        for (i = 0; i < 16; i++)
            if (randomStruct->state[15 - i]++)
                break;
    }

    R_memcpy
            ((POINTER) block, (POINTER) & randomStruct->output[16 - available], blockLen);
    randomStruct->outputAvailable = available - blockLen;

    return (0);
}

/*====================== END======================== */
//-----------------------------------------------------------------------------

/* Raw RSA public-key operation. Output has same length as modulus.

   Assumes inputLen < length of modulus.
   Requires input < modulus.
 */
int RSAPublicBlock(output, outputLen, input, inputLen, publicKey)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PUBLIC_KEY *publicKey;                              /* RSA public key */
{
#ifndef __uClinux__
    //NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],  //sxl1220
    //n[MAX_NN_DIGITS];
    NN_DIGIT *c = NULL, *e = NULL, *m = NULL, *n = NULL;
    unsigned int eDigits, nDigits;


    c = (NN_DIGIT *) sdkGetMem(MAX_NN_DIGITS * sizeof(NN_DIGIT));
    if (c == NULL) {
        Trace("rsa", "sdkGetMem failed to get %d mem\n", MAX_NN_DIGITS * sizeof(NN_DIGIT));
        goto end;
    }
    e = (NN_DIGIT *) sdkGetMem(MAX_NN_DIGITS * sizeof(NN_DIGIT));
    if (e == NULL) {
        Trace("rsa", "sdkGetMem failed to get %d mem\n", MAX_NN_DIGITS * sizeof(NN_DIGIT));
        goto end;
    }
    m = (NN_DIGIT *) sdkGetMem(MAX_NN_DIGITS * sizeof(NN_DIGIT));
    if (m == NULL) {
        Trace("rsa", "sdkGetMem failed to get %d mem\n", MAX_NN_DIGITS * sizeof(NN_DIGIT));
        goto end;
    }
    n = (NN_DIGIT *) sdkGetMem(MAX_NN_DIGITS * sizeof(NN_DIGIT));
    if (n == NULL) {
        Trace("rsa", "sdkGetMem failed to get %d mem\n", MAX_NN_DIGITS * sizeof(NN_DIGIT));
        goto end;
    }

    NN_Decode(m, MAX_NN_DIGITS, input, inputLen);
    NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX_RSA_MODULUS_LEN);
    NN_Decode(e, MAX_NN_DIGITS, publicKey->exponent, MAX_RSA_MODULUS_LEN);
    nDigits = NN_Digits(n, MAX_NN_DIGITS);
    eDigits = NN_Digits(e, MAX_NN_DIGITS);

    if (NN_Cmp(m, n, nDigits) >= 0) {
        sdkFreeMem(c);
        sdkFreeMem(e);
        sdkFreeMem(m);
        sdkFreeMem(n);
        return (RE_DATA);
    }

    /* Compute c = m^e mod n.
     */
    NN_ModExp(c, m, e, eDigits, n, nDigits);

    *outputLen = (publicKey->bits + 7) / 8;
    NN_Encode(output, *outputLen, c, nDigits);

    /* Zeroize sensitive information.
     */
    //R_memset ((POINTER)c, 0, sizeof (c));
    //R_memset ((POINTER)m, 0, sizeof (m));
    R_memset((POINTER) c, 0, MAX_NN_DIGITS * sizeof(NN_DIGIT));
    R_memset((POINTER) m, 0, MAX_NN_DIGITS * sizeof(NN_DIGIT));

    end:
    if (c) sdkFreeMem(c);
    if (e) sdkFreeMem(e);
    if (m) sdkFreeMem(m);
    if (n) sdkFreeMem(n);
    return (0);
#else
    return 0;
#endif
}

/* Raw RSA private-key operation. Output has same length as modulus.

   Assumes inputLen < length of modulus.
   Requires input < modulus.
 */
int RSAPrivateBlock(output, outputLen, input, inputLen, privateKey)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PRIVATE_KEY *privateKey;                           /* RSA private key */
{
#ifndef __uClinux__
    /*
    NN_DIGIT c[MAX_NN_DIGITS], cP[MAX_NN_DIGITS], cQ[MAX_NN_DIGITS],
    dP[MAX_NN_DIGITS], dQ[MAX_NN_DIGITS], mP[MAX_NN_DIGITS],
    mQ[MAX_NN_DIGITS], n[MAX_NN_DIGITS], p[MAX_NN_DIGITS], q[MAX_NN_DIGITS],
    qInv[MAX_NN_DIGITS], t[MAX_NN_DIGITS];
    */
    NN_DIGIT *c, *cP, *cQ, *dP, *dQ, *mP, *mQ, *n, *p, *q, *qInv, *t;
    unsigned int cDigits, nDigits, pDigits;

    c = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (c == NULL) {
        return -1;
    }
    cP = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (cP == NULL) {
        sdkFreeMem(c);
        return -1;
    }
    cQ = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (cQ == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        return -1;
    }
    dP = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (dP == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        return -1;
    }
    dQ = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (dQ == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        return -1;
    }
    mP = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (mP == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        return -1;
    }
    mQ = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (mQ == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        return -1;
    }
    n = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (n == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        return -1;
    }
    p = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (p == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        sdkFreeMem(n);
        return -1;
    }
    q = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (q == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        sdkFreeMem(n);
        sdkFreeMem(p);
        return -1;
    }
    qInv = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (qInv == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        sdkFreeMem(n);
        sdkFreeMem(p);
        sdkFreeMem(q);
        return -1;
    }
    t = (NN_DIGIT *) sdkGetMem(sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    if (t == NULL) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        sdkFreeMem(n);
        sdkFreeMem(p);
        sdkFreeMem(q);
        sdkFreeMem(qInv);
        return -1;
    }


    NN_Decode(c, MAX_NN_DIGITS, input, inputLen);
    NN_Decode(n, MAX_NN_DIGITS, privateKey->modulus, MAX_RSA_MODULUS_LEN);
    NN_Decode(p, MAX_NN_DIGITS, privateKey->prime[0], MAX_RSA_PRIME_LEN);
    NN_Decode(q, MAX_NN_DIGITS, privateKey->prime[1], MAX_RSA_PRIME_LEN);
    NN_Decode(dP, MAX_NN_DIGITS, privateKey->primeExponent[0], MAX_RSA_PRIME_LEN);
    NN_Decode(dQ, MAX_NN_DIGITS, privateKey->primeExponent[1], MAX_RSA_PRIME_LEN);
    NN_Decode(qInv, MAX_NN_DIGITS, privateKey->coefficient, MAX_RSA_PRIME_LEN);
    cDigits = NN_Digits(c, MAX_NN_DIGITS);
    nDigits = NN_Digits(n, MAX_NN_DIGITS);
    pDigits = NN_Digits(p, MAX_NN_DIGITS);

    if (NN_Cmp(c, n, nDigits) >= 0) {
        sdkFreeMem(c);
        sdkFreeMem(cP);
        sdkFreeMem(cQ);
        sdkFreeMem(dP);
        sdkFreeMem(dQ);
        sdkFreeMem(mP);
        sdkFreeMem(mQ);
        sdkFreeMem(n);
        sdkFreeMem(p);
        sdkFreeMem(q);
        sdkFreeMem(qInv);
        sdkFreeMem(t);

        return (RE_DATA);
    }

    /* Compute mP = cP^dP mod p  and  mQ = cQ^dQ mod q. (Assumes q has
       length at most pDigits, i.e., p > q.)
     */
    NN_Mod(cP, c, cDigits, p, pDigits);
    NN_Mod(cQ, c, cDigits, q, pDigits);
    NN_ModExp(mP, cP, dP, pDigits, p, pDigits);
    NN_AssignZero(mQ, nDigits);
    NN_ModExp(mQ, cQ, dQ, pDigits, q, pDigits);

    /* Chinese Remainder Theorem:
         m = ((((mP - mQ) mod p) * qInv) mod p) * q + mQ.
     */
    if (NN_Cmp(mP, mQ, pDigits) >= 0)
        NN_Sub(t, mP, mQ, pDigits);
    else {
        NN_Sub(t, mQ, mP, pDigits);
        NN_Sub(t, p, t, pDigits);
    }
    NN_ModMult(t, t, qInv, p, pDigits);
    NN_Mult(t, t, q, pDigits);
    NN_Add(t, t, mQ, nDigits);

    *outputLen = (privateKey->bits + 7) / 8;
    NN_Encode(output, *outputLen, t, nDigits);

    /* Zeroize sensitive information.
     */
    /*
	    R_memset ((POINTER)c, 0, sizeof (c));
	    R_memset ((POINTER)cP, 0, sizeof (cP));
	    R_memset ((POINTER)cQ, 0, sizeof (cQ));
	    R_memset ((POINTER)dP, 0, sizeof (dP));
	    R_memset ((POINTER)dQ, 0, sizeof (dQ));
	    R_memset ((POINTER)mP, 0, sizeof (mP));
	    R_memset ((POINTER)mQ, 0, sizeof (mQ));
	    R_memset ((POINTER)p, 0, sizeof (p));
	    R_memset ((POINTER)q, 0, sizeof (q));
	    R_memset ((POINTER)qInv, 0, sizeof (qInv));
	    R_memset ((POINTER)t, 0, sizeof (t));
	    */
    R_memset((POINTER) c, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) cP, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) cQ, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) dP, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) dQ, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) mP, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) mQ, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) p, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) q, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) qInv, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);
    R_memset((POINTER) t, 0, sizeof(NN_DIGIT) * MAX_NN_DIGITS);

    sdkFreeMem(c);
    sdkFreeMem(cP);
    sdkFreeMem(cQ);
    sdkFreeMem(dP);
    sdkFreeMem(dQ);
    sdkFreeMem(mP);
    sdkFreeMem(mQ);
    sdkFreeMem(n);
    sdkFreeMem(p);
    sdkFreeMem(q);
    sdkFreeMem(qInv);
    sdkFreeMem(t);
    return (0);
#else
    return 1;
#endif

}

/* RSA public-key encryption, according to PKCS #1.
 */
int sdk_rsa_public_encrypt(output, outputLen, input, inputLen, publicKey, randomStruct)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PUBLIC_KEY *publicKey;                              /* RSA public key */
        R_RANDOM_STRUCT *randomStruct;                          /* random structure */
{

    int status;
    unsigned char byte, pkcsBlock[MAX_RSA_MODULUS_LEN];
    unsigned int i, modulusLen;

    modulusLen = (publicKey->bits + 7) / 8;
    if (inputLen + 11 > modulusLen)
        return (RE_LEN);

    pkcsBlock[0] = 0;
    /* block type 2 */
    pkcsBlock[1] = 2;

    for (i = 2; i < modulusLen - inputLen - 1; i++) {
        /* Find nonzero random byte.
         */
        do {
            R_GenerateBytes(&byte, 1, randomStruct);
        }
        while (byte == 0);
        pkcsBlock[i] = byte;
    }
    /* separator */
    pkcsBlock[i++] = 0;

    R_memcpy((POINTER) & pkcsBlock[i], (POINTER) input, inputLen);

    status = RSAPublicBlock(output, outputLen, pkcsBlock, modulusLen, publicKey);

    /* Zeroize sensitive information.
     */
//    byte = 0;
    R_memset((POINTER) pkcsBlock, 0, sizeof(pkcsBlock));

    return (status);
}

/* RSA private-key encryption, according to PKCS #1.
 */
int sdk_rsa_private_encrypt(output, outputLen, input, inputLen, privateKey)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PRIVATE_KEY *privateKey;                           /* RSA private key */
{
    int status;
    unsigned char pkcsBlock[MAX_RSA_MODULUS_LEN];
    unsigned int i, modulusLen;

    modulusLen = (privateKey->bits + 7) / 8;
    if (inputLen + 11 > modulusLen)
        return (RE_LEN);

    pkcsBlock[0] = 0;
    /* block type 1 */
    pkcsBlock[1] = 1;

    for (i = 2; i < modulusLen - inputLen - 1; i++)
        pkcsBlock[i] = 0xff;

    /* separator */
    pkcsBlock[i++] = 0;

    R_memcpy((POINTER) & pkcsBlock[i], (POINTER) input, inputLen);

    status = RSAPrivateBlock(output, outputLen, pkcsBlock, modulusLen, privateKey);

    /* Zeroize potentially sensitive information.
     */
    R_memset((POINTER) pkcsBlock, 0, sizeof(pkcsBlock));

    return (status);
}

/* RSA private-key decryption, according to PKCS #1.
 */
int sdk_rsa_private_decrypt(output, outputLen, input, inputLen, privateKey)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PRIVATE_KEY *privateKey;                           /* RSA private key */
{
    int status;
    unsigned char pkcsBlock[MAX_RSA_MODULUS_LEN];
    unsigned int i, modulusLen, pkcsBlockLen;

    modulusLen = (privateKey->bits + 7) / 8;
    if (inputLen > modulusLen)
        return (RE_LEN);

    status = RSAPrivateBlock(pkcsBlock, &pkcsBlockLen, input, inputLen, privateKey);
    if (0 != status)
        return (status);

    if (pkcsBlockLen != modulusLen)
        return (RE_LEN);

    /* Require block type 2.
     */
    if ((pkcsBlock[0] != 0) || (pkcsBlock[1] != 2))
        return (RE_DATA);

    for (i = 2; i < modulusLen - 1; i++)
        /* separator */
        if (pkcsBlock[i] == 0)
            break;

    i++;
    if (i >= modulusLen)
        return (RE_DATA);

    *outputLen = modulusLen - i;

    if (*outputLen + 11 > modulusLen)
        return (RE_DATA);

    R_memcpy((POINTER) output, (POINTER) & pkcsBlock[i], *outputLen);

    /* Zeroize sensitive information.
     */
    R_memset((POINTER) pkcsBlock, 0, sizeof(pkcsBlock));

    return (0);
}

/* RSA public-key decryption, according to PKCS #1.
 */
int sdk_rsa_public_decrypt(output, outputLen, input, inputLen, publicKey)
        unsigned char *output;                                      /* output block */
        unsigned int *outputLen;                          /* length of output block */
        unsigned char *input;                                        /* input block */
        unsigned int inputLen;                             /* length of input block */
        R_RSA_PUBLIC_KEY *publicKey;                              /* RSA public key */
{
    int status;
    unsigned char pkcsBlock[MAX_RSA_MODULUS_LEN];
    unsigned int i, modulusLen, pkcsBlockLen;

    modulusLen = (publicKey->bits + 7) / 8;
    if (inputLen > modulusLen)
        return (RE_LEN);

    status = RSAPublicBlock(pkcsBlock, &pkcsBlockLen, input, inputLen, publicKey);
    if (0 != status)
        return (status);

    if (pkcsBlockLen != modulusLen)
        return (RE_LEN);

    /* Require block type 1.
     */
    if ((pkcsBlock[0] != 0) || (pkcsBlock[1] != 1))
        return (RE_DATA);

    for (i = 2; i < modulusLen - 1; i++)
        if (pkcsBlock[i] != 0xff)
            break;

    /* separator */
    if (pkcsBlock[i++] != 0)
        return (RE_DATA);

    *outputLen = modulusLen - i;

    if (*outputLen + 11 > modulusLen)
        return (RE_DATA);

    R_memcpy((POINTER) output, (POINTER) & pkcsBlock[i], *outputLen);

    /* Zeroize potentially sensitive information.
     */
    R_memset((POINTER) pkcsBlock, 0, sizeof(pkcsBlock));

    return (0);
}
//-----------------------------------------------------------------------------
#if 0 /*Modify by fusuipu at 2012.12.26  15:5 */
int RSAPublicDecrypt1(unsigned char *output, unsigned int *outputLen, unsigned char *input,unsigned int inputLen, R_RSA_PUBLIC_KEY *publicKey)
/* output block */
/* length of output block */
/* input block */
/* length of input block */
/* RSA public key */
{
	unsigned int modulusLen;
	unsigned int eDigits, nDigits;
	//NN_DIGIT c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS],n[MAX_NN_DIGITS];
	NN_DIGIT *c,*e,*m,*n;

	modulusLen = (publicKey->bits + 7) / 8;
	if(inputLen > modulusLen)
		return 1;

	c = (NN_DIGIT *)sdkGetMem(sizeof(MAX_NN_DIGITS)*MAX_NN_DIGITS);
	if(c == NULL)
	{
		return 1;
	}
	e = (NN_DIGIT *)sdkGetMem(sizeof(MAX_NN_DIGITS)*MAX_NN_DIGITS);
	if(e == NULL)
	{
	    sdkFreeMem(c);
		return 1;
	}
	m = (NN_DIGIT *)sdkGetMem(sizeof(MAX_NN_DIGITS)*MAX_NN_DIGITS);
	if(m == NULL)
	{
	    sdkFreeMem(c);
		sdkFreeMem(e);
		return 1;
	}
	n = (NN_DIGIT *)sdkGetMem(sizeof(MAX_NN_DIGITS)*MAX_NN_DIGITS);
	if(n == NULL)
	{
	    sdkFreeMem(c);
		sdkFreeMem(e);
		sdkFreeMem(m);
		return 1;
	}


	/* decode the required RSA function input data */
	NN_Decode(m, MAX_NN_DIGITS, input, inputLen);
	NN_Decode(n, MAX_NN_DIGITS, publicKey->modulus, MAX_RSA_MODULUS_LEN);
	NN_Decode(e, MAX_NN_DIGITS, publicKey->exponent, MAX_RSA_MODULUS_LEN);
	nDigits = NN_Digits(n, MAX_NN_DIGITS);
	eDigits = NN_Digits(e, MAX_NN_DIGITS);
/*	if(NN_Cmp(m, n, nDigits) >= 0)
		return(RE_DATA);
*/
//	*outputLen = (publicKey->bits + 7) / 8;

	/* Compute c = m^e mod n.  To perform actual RSA calc.*/

	NN_ModExp (c, m, e, eDigits, n, nDigits);
	/* encode output to standard form */
	NN_Encode (output, modulusLen, c, nDigits);

/*	printf("The output Data is:\n");
	for(eDigits=0;eDigits<modulusLen;eDigits++)
	{
		printf("%02X ",output[eDigits]);
	}
	printf("\n");
*/
    sdkFreeMem(c);
	sdkFreeMem(e);
	sdkFreeMem(m);
	sdkFreeMem(n);
    return 0;
}
#endif /* if 0 */

