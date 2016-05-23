#ifndef __ICON_BITMAP_PCK__
#define __ICON_BITMAP_PCK__


typedef struct _MC_BITMAP {
    unsigned int bmScan;
    unsigned int bmWidth;
    unsigned int bmHeight;
    unsigned int bmBitSize;
    unsigned int bmOffset;
    unsigned char *bmBits;
} MC_BITMAP, *pMC_BITMAP;


#define    ICON_G2_1GE    0
#define    ICON_G2_2GE    1
#define    ICON_G2_AUDIO    2
#define    ICON_G2_BT    3
#define    ICON_G2_BT_LINKING    4
#define    ICON_G2_CHONG0    5
#define    ICON_G2_CHONG1    6
#define    ICON_G2_CHONG2    7
#define    ICON_G2_DIANMAN    8
#define    ICON_G2_GUZHANG    9
#define    ICON_G2_USB    10
#define    ICON_G2_WAICHONG    11
#define    ICON_G2_WAIMAN    12
#define    ICON_G2_WUDIAN    13
#define    ICON_G2_X    14
#define    ICON_K200P_1GE    15
#define    ICON_K200P_2GE    16
#define    ICON_K200P_AUDIO    17
#define    ICON_K200P_BT    18
#define    ICON_K200P_BT_LINKING    19
#define    ICON_K200P_CHONG0    20
#define    ICON_K200P_CHONG1    21
#define    ICON_K200P_CHONG2    22
#define    ICON_K200P_DIANMAN    23
#define    ICON_K200P_GUZHANG    24
#define    ICON_K200P_USB    25
#define    ICON_K200P_WAICHONG    26
#define    ICON_K200P_WAIMAN    27
#define    ICON_K200P_WUDIAN    28
#define    ICON_K200P_X    29
#define    ICON_ERROR_FLAG        30


#endif
