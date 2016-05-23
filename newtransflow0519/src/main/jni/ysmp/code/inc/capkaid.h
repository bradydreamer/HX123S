#ifndef CAPKAID_H_
#define CAPKAID_H_

typedef struct {
    unsigned char Tag[3];     //Second byte '00' means only first one byte tag.add third byte to make int variable oven aligned.
    unsigned char Len1;      //Len1>Len2: fix len with Len2;Len1<Len2 and Len2!=255: Len1<= len <=Len2; Len2=255: len=Len1+Len2
    unsigned char Len2;
    unsigned char bAmt;     //0- non numeric;1-numeric;2-compact numric
    unsigned short address;    //var address in struct TermInfo(TERMINFO)
    unsigned char flagM;     //'1'--bit0: mandatory;bit1: SDA mandatory; bit2: DDA mandatory;
    unsigned char bExist;     //0-not presented,1-have been existed.
} AppDATAELEMENT;

EXTERN s32
AppAidParamNum;
EXTERN SDK_ICC_EmvSysIni_STRUCT
AppAidParam[100];
EXTERN SDK_ICC_EmvSysIni_STRUCT
AppDownIniDataInfo;
EXTERN s32
AppTermCAPKNum;
EXTERN SDK_ICC_CAPK_STRUCT
AppTermCAPK[100];
EXTERN SDK_ICC_CAPK_STRUCT
AppCapkDataInfo;

extern void ReadAidParam(u8 * AidName);

extern u8 ParseAidParam(u8 *data, u16 len);

extern u8 SaveAidParam(u8 * AidName);
extern u8 DeleteAIDFile(u8 * aidname);
extern u8 SaveCapk(u8 * CapkName);
extern void ReadCapk(u8 * CapkName);

extern u8 ParseCapk(u8 *data, u16 len);

extern void ReadAidCapk(u8 type);

extern s32 UnzipPagData(u8 *DataIn, u16 DataLen, u8 UnzipType);

extern u8 VerifyCAPK(SDK_ICC_CAPK_STRUCT * capk);

extern u8 UnzipCapkDownLoadData(u8 *DataIn, u16 DataLen);

extern u8 UnzipIniDownLoadData(u8 *DataIn, u16 DataLen);

extern u8 AppendAidParam(SDK_ICC_EmvSysIni_STRUCT tempAid);

extern u8 AppendCAPKParam(SDK_ICC_CAPK_STRUCT tempCapk);

extern u8 ParseAndDeleteCapk(u8 *data, u16 len);

extern u8 ParseAndDeleteAidParam(u8 *data, u16 len);

#define     TYPECAPK    0X01
#define     TYPEAID    0X02

#endif


