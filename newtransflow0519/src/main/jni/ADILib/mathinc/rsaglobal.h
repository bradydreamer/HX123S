/* GLOBAL.H - RSAREF types and constants */

/* Copyright (C) RSA Laboratories, a division of RSA Data Security, Inc., created 1991. All rights reserved.*/

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 1

/* PROTOTYPES should be set to one if and only if the compiler supports function argument prototyping.
   The following makes PROTOTYPES default to 1 if it has not already been defined as 0 with C compiler flags. */

#ifndef PROTOTYPES
#define PROTOTYPES 1
#endif

/* POINTER defines a generic pointer type */
typedef unsigned char *POINTER;

/* UINT2 defines a two byte word */
typedef unsigned short int UINT2;

/* UINT4 defines a four byte word */
typedef unsigned long int UINT4;

#ifndef NULL_PTR
#define NULL_PTR ((POINTER)0)
#endif

#ifndef UNUSED_ARG
#define UNUSED_ARG(x) x = *(&x);
#endif

/* PROTO_LIST is defined depending on how PROTOTYPES is defined above.
   If using PROTOTYPES, then PROTO_LIST returns the list, otherwise it returns an empty list. */
#if PROTOTYPES
#define PROTO_LIST(list) list
#else
#define PROTO_LIST(list) ()
#endif

/*=======BEGIN: fusuipu 2013.01.06  9:8 modify===========*/
typedef struct {
    UINT4 state[4];
    /* state (ABCD) */
    UINT4 count[2];
    /* number of bits, modulo 2^64 (lsb first) */
    unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

void MD5Init PROTO_LIST((MD5_CTX * ));

void MD5Update PROTO_LIST((MD5_CTX * , unsigned
                                  char *, unsigned int));

void MD5Final PROTO_LIST((unsigned
                                 char[16], MD5_CTX *));
/*====================== END======================== */

#endif /* end _GLOBAL_H_ */

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

