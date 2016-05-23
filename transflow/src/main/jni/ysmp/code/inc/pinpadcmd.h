#ifndef PASSWDBOARD_H
#define PASSWDBOARD_H

#define TRANSPORT_INDEX 0
#define AUTHENTICATION_INDEX 101


#define TEST_MK_INDEX        20
#define TEST_AUTH_KEY_INDEX 21
#define TEST_APP_KEY_INDEX    22


#define MACCALC_FILENAME        "/mtd0/mpos/macCaleData.dat"
#define MAINACCOUNT_FILENAME    "/mtd0/mpos/mainAccountHashCode.dat"


enum {

    PASSWD_INPUT_ENUM = 0x01,
    LOAD_TMK_ENUM = 0x02,
    DATA_ENCRYPT_ENUM = 0x03,
    MAC_CALC_ENUM = 0x04,
    LOAD_WK_ENUM = 0x05,
    MAC_CALC_INNERINFO_ENUM = 0x08,
    SETTING_PASSWD_ENUM = 0x10,
    PASSWD_INPUT_OUTER = 0x11,
    DATA_ENCRYPT_OUTER = 0x13,
    MAC_CALC_OUTER = 0x14,
    LOAD_WK_OUTER = 0x15,
    LOAD_DUKPT_OUTER = 0x17,
    PINDPAD_END = 0xff
};

typedef enum cryptIndex {
    NULL_INDEX = 0x0,
    TMK_INDEX,
    WK_INDEX

} ENUM_CRYPTINDEX;

enum blockFlag {
    BLOCK_FIRST = 0,
    BLOCK_NEXT = 1,
    BLOCK_LAST = 2,
    BLOCK_ONLY = 3
};

typedef struct STRUCT_MACCAL {
    u8 cryptIndex;
    u8 cryptMode;
    u8 macMode;
    u8 blockFlag;
    u16 macLen;
    u8 *pMac;
    u16 cryptLen;
    u8 *encryptData;
    u16 tagLen;
    u8 *pTag;

} maccal_struct;

typedef struct STRUCT_PASSWDINPUT//С�ڴ� �ṹ����Ҫ�����ֽڶ��� �����ڴ��˷�
{
    u8 cryptIndex;
    u8 cryptType;
    u8 mainAccountFlag;
    u8 passwdLen;
    u8 enterClick;
    u8 enterExpire;
    u16 cryptLen;
    u16 displayDataLen;
    u8 *pMainAccount;
    u8 *pCryptData;
    u8 *pCryptExtra;
    u8 *pDisplayData;

} struct_passwdInput;


void LoadTestKey();


s32 OnPasswdInput(ST_TRANSFLOW * pstTransFlow);
s32 OnLoadTMK(ST_TRANSFLOW * pstTransFlow);
s32 OnDataEncrypt(ST_TRANSFLOW * pstTransFlow);
s32 OnCalcMAC(ST_TRANSFLOW * pstTransFlow);
s32 OnLoadWK(ST_TRANSFLOW * pstTransFlow);
s32 OnSetPinpadOuter(ST_TRANSFLOW * pstTransFlow);
s32 OnLoadDUKPT(ST_TRANSFLOW * pstTransFlow);
s32 OnCalcMACStoreTag(ST_TRANSFLOW * pstTransFlow);


#endif

