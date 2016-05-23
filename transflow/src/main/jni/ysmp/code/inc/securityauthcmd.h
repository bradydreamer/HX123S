#ifndef SECURITYAUTH_H
#define SECURITYAUTH_H

#define VERSION_R "YS.20150407.01"     //�汾�����16���ֽ�


enum {

    DEVICEINFO_ENUM = 0x01,
    RANDOMNUM_ENUM,
    DEVICEAUTH_ENUM,
    SECURITY_END_ENUM = 0xff
};


enum {

    DECRYPT_ENUM = 0x0,
    ENCRYPT_ENUM = 0x1
};

typedef struct STRUCT_DEVICE_INFO {
    u8 deviceSN[12];
    u8 devicePrivateStatus;
    u8 version[16];
    u8 reverse[10];
    u8 deviceStatus;
    u8 firmwareVersion[16];
    u8 customerSNLen[2];
    u8 cryptSNLen[2];
    u8 productID[2];
    u8 menufactureID[2];
    u8 produceIDLen[2];
    u8 bootVersionLen[2];

} DEVICE_INFO;

s32 OnGetDeviceInfo(ST_TRANSFLOW * pstTransFlow);
s32 OnGetRandomNum(ST_TRANSFLOW * pstTransFlow);
s32 OnDeviceAuth(ST_TRANSFLOW * pstTransFlow);


#endif 


