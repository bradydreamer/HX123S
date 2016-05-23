#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */


#ifndef SDK_TYPEDEF_H
#define SDK_TYPEDEF_H

#ifndef __cplusplus

#undef true
#define true 1

#undef false
#define false 0

#undef bool
typedef unsigned char bool;

#endif  /* __cplusplus */


#undef TRUE
#undef FALSE

#undef BOOL
typedef unsigned int BOOL;


#undef u8
#define  u8   unsigned char

#undef U8

#undef s8
#define s8  signed char

#undef S8

#undef u16
#define  u16  unsigned short

#undef U16


#undef s16
#define  s16   signed short

#undef S16


#undef u32
#define  u32  unsigned int

#undef U32


#undef s32
#define  s32    signed int

#undef S32


#define BIT_0     (0x1ul << 0)
#define BIT_1     (0x1ul << 1)
#define BIT_2     (0x1ul << 2)
#define BIT_3     (0x1ul << 3)
#define BIT_4     (0x1ul << 4)
#define BIT_5     (0x1ul << 5)
#define BIT_6    (0x1ul << 6)
#define BIT_7     (0x1ul << 7)
#define BIT_8     (0x1ul << 8)
#define BIT_9     (0x1ul << 9)
#define BIT_10   (0x1ul << 10)
#define BIT_11     (0x1ul << 11)
#define BIT_12     (0x1ul << 12)
#define BIT_13     (0x1ul << 13)
#define BIT_14     (0x1ul << 14)
#define BIT_15     (0x1ul << 15)
#define BIT_16     (0x1ul << 16)
#define BIT_17     (0x1ul << 17)
#define BIT_18     (0x1ul << 18)
#define BIT_19     (0x1ul << 19)
#define BIT_20     (0x1ul << 20)
#define BIT_21     (0x1ul << 21)
#define BIT_22     (0x1ul << 22)
#define BIT_23     (0x1ul << 23)
#define BIT_24     (0x1ul << 24)
#define BIT_25     (0x1ul << 25)
#define BIT_26     (0x1ul << 26)
#define BIT_27     (0x1ul << 27)
#define BIT_28     (0x1ul << 28)
#define BIT_29     (0x1ul << 29)
#define BIT_30     (0x1ul << 30)
#define BIT_31     (0x1ul << 31)

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

