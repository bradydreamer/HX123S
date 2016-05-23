#include "javaInterface.h"
#include "appglobal.h"
//extern s32 Private_sdkIccConfigPagTLV(const u8 *pheTag, const u8 *pheInData, s32 siInLen);
extern s32 Private_sdkIccImportRespCode(u8 ucOnlineResult, const u8 *pheRspCode);

static SDK_ICC_TRADE_PARAM gIccTradeParam;
static SDK_ICC_PARAM gIccParam;
static u8 gasTradeAmount[13] = {0};
static u8 gTradeTlv[512] = {0};
static u16 gTradeTlvLen = 0;
static u8 gTerminalAttributeTlv[512] = {0};
static u16 gTerminalAttributeTlvLen = 0;
static bool gisFirstSelApp = true;

struct _DoLTab {
    u8 *pheTag;
};
extern JAVA_INTERFACE *javaInterface;

extern s32 SdkIccEmv(bool isRf);

extern void ZipPagData(bool bisOnLineDeal, u8 *DOLData, u16 *DataLen);

extern void SetFlowResult(s32 result);

extern s32 AnalyticTlv(u8 *pDataIn, u16 datalen, u8 *pTag, u8 *pTagData, u16 *pTagLen);

//const SDK_ICC_CAPK_STRUCT defaultCAPK[] =  //slx? ���Գ������в�����

const SDK_ICC_EmvSysIni_STRUCT defaultAID[] = {
//Master AID
        {6, "\xA0\x00\x00\x00\x04\x10", 0, "\x00\x02",
                {0xFC, 0x50, 0xAC, 0xA0, 0x00},
                {0xF8, 0x50, 0xAC, 0xF8, 0x00},
                {0x04, 0x00, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},

        {6, "\xA0\x00\x00\x00\x04\x30", 0, "\x00\x02",
                {0xFC, 0x50, 0xAC, 0xA0, 0x00},
                {0xF8, 0x50, 0xAC, 0xF8, 0x00},
                {0x04, 0x00, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},
//Visa AID
        {5, "\xA0\x00\x00\x00\x03", 0, "\x00\x8c",
                {0xD8, 0x40, 0x00, 0xA8, 0x00}, //default
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //online//D84004F800
                {0x00, 0x10, 0x00, 0x00, 0x00}, //decline//D84000A800
                {0}, {0}, 0, 0, 3, {0x9F, 0x37, 0x04}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},

//JCB AID
        {7, "\xA0\x00\x00\x00\x65\x10\x10", 0, "\x00\x21",
                {0xfc, 0x60, 0x24, 0x28, 0x00},
                {0xfc, 0x60, 0xac, 0xf8, 0x00},
                {0x00, 0x10, 0x00, 0x00, 0x00},
                {0}, {0}, 0, 0, 0, {0}, 1,
                {0}, {0}, {0}, {0}, 0, 0, 0, {0}, 0},
// PBOC
        {8, "\xA0\x00\x00\x03\x33\x01\x01\x06", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 0, //招行不支持ONLINEPIN
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                 0, 0, 0, {0}, 0},


        {8, "\xA0\x00\x00\x03\x33\x01\x01\x01", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1, //招行不支持ONLINEPIN
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x01, 0x00, 0x00},
                 0, 0, 0, {0}, 0},

        {8, "\xA0\x00\x00\x03\x33\x01\x01\x02", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1,
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                0, 0, 0, {0}, 0},

        {8, "\xA0\x00\x00\x03\x33\x01\x01\x03", 0, "\x00\x20", //AidLen, AID, Asi, AppVerNum
                {0x08, 0x40, 0x00, 0xA8, 0x00}, //TacDefault   DF11
                {0xD8, 0x40, 0x04, 0xF8, 0x00}, //TacOnline    DF12
                {0x00, 0x10, 0x00, 0x00, 0x00}, //TacDecline   DF13
                {0}, {0}, 0, 0, 0, {0x9F, 0x37, 0x04}, 1,
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                {0x00, 0x00, 0x00, 0x10, 0x00, 0x00},
                0, 0, 0, {0}, 0},

        //以下是银联mpos给加上的
        {7,"\xA0\x00\x00\x00\x03\x10\x10", 0, "\x01\x40",
                {0xD8,0x40,0x00,0xA8,0x00}, //TacDefault   DF11
                {0xD8,0x40,0x04,0xF8,0x00}, //TacOnline    DF12
                {0x00,0x10,0x00,0x00,0x00}, //TacDecline   DF13
                {0x00,0x00,0x00,0x10}, {0},99,99, 3,{0x9F,0x37,0x04}, 1,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},

        {7,"\xA0\x00\x00\x00\x03\x20\x10", 0, "\x01\x40",
                {0xD8,0x40,0x00,0xA8,0x00},
                {0xD8,0x40,0x04,0xF8,0x00},
                {0x00,0x10,0x00,0x00,0x00},
                {0}, {0},99,99, 3,{0x9F,0x37,0x04}, 0,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},

        {7,"\xA0\x00\x00\x00\x03\x30\x10", 0, "\x01\x40",
                {0xD8,0x40,0x00,0xA8,0x00},
                {0xD8,0x40,0x04,0xF8,0x00},
                {0x00,0x10,0x00,0x00,0x00},
                {0}, {0},99,99, 3,{0x9F,0x37,0x04}, 0,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},

        {7,"\xA0\x00\x00\x00\x04\x10\x10", 0, "\x00\x02",
                {0xFC,0x50,0x80,0xA0,0x00},
                {0xF8,0x50,0x80,0xF8,0x00},
                {0x04,0x00,0x00,0x00,0x00},
                {0}, {0},99,99, 3,{0x9F,0x37,0x04}, 1,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},

        {7,"\xA0\x00\x00\x00\x04\x30\x60", 0, "\x00\x02",
                {0xFC,0x50,0x58,0xA0,0x00},
                {0xF8,0x50,0x58,0xF8,0x00},
                {0x04,0x00,0x00,0x00,0x00},
                {0}, {0},99,99, 3,{0x9F,0x37,0x04}, 1,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},

        {7,"\xA0\x00\x00\x00\x65\x10\x10", 0, "\x02\x00",
                {0xFC,0x60,0x24,0xA8,0x00},
                {0xFC,0x60,0xAC,0xF8,0x00},
                {0x00,0x10,0x00,0x00,0x00},
                {0}, {0},99,99, 3,{0x9F,0x37,0x04}, 0,
                {0},
                {0},
                {0},
                {0},
                0, 0, 0, {0}, 0},
};

const SDK_ICC_CAPK_STRUCT defaultCAPK[] = {
//VISA 的CAPK===============================
//对应结构CAPK_STRUCT,VSIA第1组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
        0x01,
        0x01,
        0x01,
        128,
        {0xC6, 0x96, 0x03, 0x42, 0x13, 0xD7, 0xD8, 0x54, 0x69, 0x84, 0x57, 0x9D, 0x1D, 0x0F, 0x0E, 0xA5,
        0x19, 0xCF, 0xF8, 0xDE, 0xFF, 0xC4, 0x29, 0x35, 0x4C, 0xF3, 0xA8, 0x71, 0xA6, 0xF7, 0x18, 0x3F,
        0x12, 0x28, 0xDA, 0x5C, 0x74, 0x70, 0xC0, 0x55, 0x38, 0x71, 0x00, 0xCB, 0x93, 0x5A, 0x71, 0x2C,
        0x4E, 0x28, 0x64, 0xDF, 0x5D, 0x64, 0xBA, 0x93, 0xFE, 0x7E, 0x63, 0xE7, 0x1F, 0x25, 0xB1, 0xE5,
        0xF5, 0x29, 0x85, 0x75, 0xEB, 0xE1, 0xC6, 0x3A, 0xA6, 0x17, 0x70, 0x69, 0x17, 0x91, 0x1D, 0xC2,
        0xA7, 0x5A, 0xC2, 0x8B, 0x25, 0x1C, 0x7E, 0xF4, 0x0F, 0x23, 0x65, 0x91, 0x24, 0x90, 0xB9, 0x39,
        0xBC, 0xA2, 0x12, 0x4A, 0x30, 0xA2, 0x8F, 0x54, 0x40, 0x2C, 0x34, 0xAE, 0xCA, 0x33, 0x1A, 0xB6,
        0x7E, 0x1E, 0x79, 0xB2, 0x85, 0xDD, 0x57, 0x71, 0xB5, 0xD9, 0xFF, 0x79, 0xEA, 0x63, 0x0B, 0x75},
        1,
        {0x03},
        0x14,
        {0xD3, 0x4A, 0x6A, 0x77, 0x60, 0x11, 0xC7, 0xE7, 0xCE, 0x3A, 0xEC, 0x5F, 0x03, 0xAD, 0x2F, 0x8C,
                0xFC, 0x55, 0x03, 0xCC},
        {0x20, 0x16, 0x12, 0x31}},

//对应结构CAPK_STRUCT,VSIA第2组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
        0x07,
        0x01,
        0x01,
        144,
        {0xA8, 0x9F, 0x25, 0xA5, 0x6F, 0xA6, 0xDA, 0x25, 0x8C, 0x8C, 0xA8, 0xB4, 0x04, 0x27, 0xD9, 0x27,
        0xB4, 0xA1, 0xEB, 0x4D, 0x7E, 0xA3, 0x26, 0xBB, 0xB1, 0x2F, 0x97, 0xDE, 0xD7, 0x0A, 0xE5, 0xE4,
        0x48, 0x0F, 0xC9, 0xC5, 0xE8, 0xA9, 0x72, 0x17, 0x71, 0x10, 0xA1, 0xCC, 0x31, 0x8D, 0x06, 0xD2,
        0xF8, 0xF5, 0xC4, 0x84, 0x4A, 0xC5, 0xFA, 0x79, 0xA4, 0xDC, 0x47, 0x0B, 0xB1, 0x1E, 0xD6, 0x35,
        0x69, 0x9C, 0x17, 0x08, 0x1B, 0x90, 0xF1, 0xB9, 0x84, 0xF1, 0x2E, 0x92, 0xC1, 0xC5, 0x29, 0x27,
        0x6D, 0x8A, 0xF8, 0xEC, 0x7F, 0x28, 0x49, 0x20, 0x97, 0xD8, 0xCD, 0x5B, 0xEC, 0xEA, 0x16, 0xFE,
        0x40, 0x88, 0xF6, 0xCF, 0xAB, 0x4A, 0x1B, 0x42, 0x32, 0x8A, 0x1B, 0x99, 0x6F, 0x92, 0x78, 0xB0,
        0xB7, 0xE3, 0x31, 0x1C, 0xA5, 0xEF, 0x85, 0x6C, 0x2F, 0x88, 0x84, 0x74, 0xB8, 0x36, 0x12, 0xA8,
        0x2E, 0x4E, 0x00, 0xD0, 0xCD, 0x40, 0x69, 0xA6, 0x78, 0x31, 0x40, 0x43, 0x3D, 0x50, 0x72, 0x5F},
        1,
        {0x03},
        0x14,
        {0xB4, 0xBC, 0x56, 0xCC, 0x4E, 0x88, 0x32, 0x49, 0x32, 0xCB, 0xC6, 0x43, 0xD6, 0x89, 0x8F, 0x6F,
        0xE5, 0x93, 0xB1, 0x72},
        {0x20, 0x16, 0x12, 0x31}},

//对应结构CAPK_STRUCT,VSIA第3组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
        0x08,
        0x01,
        0x01,
        176,
        {0xD9, 0xFD, 0x6E, 0xD7, 0x5D, 0x51, 0xD0, 0xE3, 0x06, 0x64, 0xBD, 0x15, 0x70, 0x23, 0xEA, 0xA1,
        0xFF, 0xA8, 0x71, 0xE4, 0xDA, 0x65, 0x67, 0x2B, 0x86, 0x3D, 0x25, 0x5E, 0x81, 0xE1, 0x37, 0xA5,
        0x1D, 0xE4, 0xF7, 0x2B, 0xCC, 0x9E, 0x44, 0xAC, 0xE1, 0x21, 0x27, 0xF8, 0x7E, 0x26, 0x3D, 0x3A,
        0xF9, 0xDD, 0x9C, 0xF3, 0x5C, 0xA4, 0xA7, 0xB0, 0x1E, 0x90, 0x70, 0x00, 0xBA, 0x85, 0xD2, 0x49,
        0x54, 0xC2, 0xFC, 0xA3, 0x07, 0x48, 0x25, 0xDD, 0xD4, 0xC0, 0xC8, 0xF1, 0x86, 0xCB, 0x02, 0x0F,
        0x68, 0x3E, 0x02, 0xF2, 0xDE, 0xAD, 0x39, 0x69, 0x13, 0x3F, 0x06, 0xF7, 0x84, 0x51, 0x66, 0xAC,
        0xEB, 0x57, 0xCA, 0x0F, 0xC2, 0x60, 0x34, 0x45, 0x46, 0x98, 0x11, 0xD2, 0x93, 0xBF, 0xEF, 0xBA,
        0xFA, 0xB5, 0x76, 0x31, 0xB3, 0xDD, 0x91, 0xE7, 0x96, 0xBF, 0x85, 0x0A, 0x25, 0x01, 0x2F, 0x1A,
        0xE3, 0x8F, 0x05, 0xAA, 0x5C, 0x4D, 0x6D, 0x03, 0xB1, 0xDC, 0x2E, 0x56, 0x86, 0x12, 0x78, 0x59,
        0x38, 0xBB, 0xC9, 0xB3, 0xCD, 0x3A, 0x91, 0x0C, 0x1D, 0xA5, 0x5A, 0x5A, 0x92, 0x18, 0xAC, 0xE0,
        0xF7, 0xA2, 0x12, 0x87, 0x75, 0x26, 0x82, 0xF1, 0x58, 0x32, 0xA6, 0x78, 0xD6, 0xE1, 0xED, 0x0B},
        1,
        {0x03},
        0x14,
        {0x20, 0xD2, 0x13, 0x12, 0x69, 0x55, 0xDE, 0x20,
        0x5A, 0xDC, 0x2F, 0xD2, 0x82, 0x2B, 0xD2, 0x2D,
        0xE2, 0x1C, 0xF9, 0xA8},
        {0x20, 0x16, 0x12, 0x31}},

//对应结构CAPK_STRUCT,VSIA第4组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x03},
        0x09,
        0x01,
        0x01,
        248,
        {0x9D, 0x91, 0x22, 0x48, 0xDE, 0x0A, 0x4E, 0x39, 0xC1, 0xA7, 0xDD, 0xE3, 0xF6, 0xD2, 0x58, 0x89,
        0x92, 0xC1, 0xA4, 0x09, 0x5A, 0xFB, 0xD1, 0x82, 0x4D, 0x1B, 0xA7, 0x48, 0x47, 0xF2, 0xBC, 0x49,
        0x26, 0xD2, 0xEF, 0xD9, 0x04, 0xB4, 0xB5, 0x49, 0x54, 0xCD, 0x18, 0x9A, 0x54, 0xC5, 0xD1, 0x17,
        0x96, 0x54, 0xF8, 0xF9, 0xB0, 0xD2, 0xAB, 0x5F, 0x03, 0x57, 0xEB, 0x64, 0x2F, 0xED, 0xA9, 0x5D,
        0x39, 0x12, 0xC6, 0x57, 0x69, 0x45, 0xFA, 0xB8, 0x97, 0xE7, 0x06, 0x2C, 0xAA, 0x44, 0xA4, 0xAA,
        0x06, 0xB8, 0xFE, 0x6E, 0x3D, 0xBA, 0x18, 0xAF, 0x6A, 0xE3, 0x73, 0x8E, 0x30, 0x42, 0x9E, 0xE9,
        0xBE, 0x03, 0x42, 0x7C, 0x9D, 0x64, 0xF6, 0x95, 0xFA, 0x8C, 0xAB, 0x4B, 0xFE, 0x37, 0x68, 0x53,
        0xEA, 0x34, 0xAD, 0x1D, 0x76, 0xBF, 0xCA, 0xD1, 0x59, 0x08, 0xC0, 0x77, 0xFF, 0xE6, 0xDC, 0x55,
        0x21, 0xEC, 0xEF, 0x5D, 0x27, 0x8A, 0x96, 0xE2, 0x6F, 0x57, 0x35, 0x9F, 0xFA, 0xED, 0xA1, 0x94,
        0x34, 0xB9, 0x37, 0xF1, 0xAD, 0x99, 0x9D, 0xC5, 0xC4, 0x1E, 0xB1, 0x19, 0x35, 0xB4, 0x4C, 0x18,
        0x10, 0x0E, 0x85, 0x7F, 0x43, 0x1A, 0x4A, 0x5A, 0x6B, 0xB6, 0x51, 0x14, 0xF1, 0x74, 0xC2, 0xD7,
        0xB5, 0x9F, 0xDF, 0x23, 0x7D, 0x6B, 0xB1, 0xDD, 0x09, 0x16, 0xE6, 0x44, 0xD7, 0x09, 0xDE, 0xD5,
        0x64, 0x81, 0x47, 0x7C, 0x75, 0xD9, 0x5C, 0xDD, 0x68, 0x25, 0x46, 0x15, 0xF7, 0x74, 0x0E, 0xC0,
        0x7F, 0x33, 0x0A, 0xC5, 0xD6, 0x7B, 0xCD, 0x75, 0xBF, 0x23, 0xD2, 0x8A, 0x14, 0x08, 0x26, 0xC0,
        0x26, 0xDB, 0xDE, 0x97, 0x1A, 0x37, 0xCD, 0x3E, 0xF9, 0xB8, 0xDF, 0x64, 0x4A, 0xC3, 0x85, 0x01,
        0x05, 0x01, 0xEF, 0xC6, 0x50, 0x9D, 0x7A, 0x41},
        1,
        {0x03},
        0x14,
        {0x1F, 0xF8, 0x0A, 0x40, 0x17, 0x3F, 0x52, 0xD7,
        0xD2, 0x7E, 0x0F, 0x26, 0xA1, 0x46, 0xA1, 0xC8,
        0xCC, 0xB2, 0x90, 0x46},
        {0x20, 0x16, 0x12, 0x31}},

//MASTER CAPK==========================
//对应结构CAPK_STRUCT,MASTER第1组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
        0x03,
        0x01,
        0x01,
        128,
        {0xC2, 0x49, 0x07, 0x47, 0xFE, 0x17, 0xEB, 0x05, 0x84, 0xC8, 0x8D, 0x47, 0xB1, 0x60, 0x27, 0x04,
        0x15, 0x0A, 0xDC, 0x88, 0xC5, 0xB9, 0x98, 0xBD, 0x59, 0xCE, 0x04, 0x3E, 0xDE, 0xBF, 0x0F, 0xFE,
        0xE3, 0x09, 0x3A, 0xC7, 0x95, 0x6A, 0xD3, 0xB6, 0xAD, 0x45, 0x54, 0xC6, 0xDE, 0x19, 0xA1, 0x78,
        0xD6, 0xDA, 0x29, 0x5B, 0xE1, 0x5D, 0x52, 0x20, 0x64, 0x5E, 0x3C, 0x81, 0x31, 0x66, 0x6F, 0xA4,
        0xBE, 0x5B, 0x84, 0xFE, 0x13, 0x1E, 0xA4, 0x4B, 0x03, 0x93, 0x07, 0x63, 0x8B, 0x9E, 0x74, 0xA8,
        0xC4, 0x25, 0x64, 0xF8, 0x92, 0xA6, 0x4D, 0xF1, 0xCB, 0x15, 0x71, 0x2B, 0x73, 0x6E, 0x33, 0x74,
        0xF1, 0xBB, 0xB6, 0x81, 0x93, 0x71, 0x60, 0x2D, 0x89, 0x70, 0xE9, 0x7B, 0x90, 0x07, 0x93, 0xC7,
        0xC2, 0xA8, 0x9A, 0x4A, 0x16, 0x49, 0xA5, 0x9B, 0xE6, 0x80, 0x57, 0x4D, 0xD0, 0xB6, 0x01, 0x45},
        1,
        {0x03},
        0x14,
        {0x5A, 0xDD, 0xF2, 0x1D, 0x09, 0x27, 0x86, 0x61,
        0x14, 0x11, 0x79, 0xCB, 0xEF, 0xF2, 0x72, 0xEA,
        0x38, 0x4B, 0x13, 0xBB},
        {0x20, 0x16, 0x12, 0x31}},
//对应结构CAPK_STRUCT,MASTER第2组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
        0x04,
        0x01,
        0x01,
        144,
        {0xA6, 0xDA, 0x42, 0x83, 0x87, 0xA5, 0x02, 0xD7, 0xDD, 0xFB, 0x7A, 0x74, 0xD3, 0xF4, 0x12, 0xBE,
        0x76, 0x26, 0x27, 0x19, 0x7B, 0x25, 0x43, 0x5B, 0x7A, 0x81, 0x71, 0x6A, 0x70, 0x01, 0x57, 0xDD,
        0xD0, 0x6F, 0x7C, 0xC9, 0x9D, 0x6C, 0xA2, 0x8C, 0x24, 0x70, 0x52, 0x7E, 0x2C, 0x03, 0x61, 0x6B,
        0x9C, 0x59, 0x21, 0x73, 0x57, 0xC2, 0x67, 0x4F, 0x58, 0x3B, 0x3B, 0xA5, 0xC7, 0xDC, 0xF2, 0x83,
        0x86, 0x92, 0xD0, 0x23, 0xE3, 0x56, 0x24, 0x20, 0xB4, 0x61, 0x5C, 0x43, 0x9C, 0xA9, 0x7C, 0x44,
        0xDC, 0x9A, 0x24, 0x9C, 0xFC, 0xE7, 0xB3, 0xBF, 0xB2, 0x2F, 0x68, 0x22, 0x8C, 0x3A, 0xF1, 0x33,
        0x29, 0xAA, 0x4A, 0x61, 0x3C, 0xF8, 0xDD, 0x85, 0x35, 0x02, 0x37, 0x3D, 0x62, 0xE4, 0x9A, 0xB2,
        0x56, 0xD2, 0xBC, 0x17, 0x12, 0x0E, 0x54, 0xAE, 0xDC, 0xED, 0x6D, 0x96, 0xA4, 0x28, 0x7A, 0xCC,
        0x5C, 0x04, 0x67, 0x7D, 0x4A, 0x5A, 0x32, 0x0D, 0xB8, 0xBE, 0xE2, 0xF7, 0x75, 0xE5, 0xFE, 0xC5},
        1,
        {0x03},
        0x14,
        {0x38, 0x1A, 0x03, 0x5D, 0xA5, 0x8B, 0x48, 0x2E,
        0xE2, 0xAF, 0x75, 0xF4, 0xC3, 0xF2, 0xCA, 0x46,
        0x9B, 0xA4, 0xAA, 0x6C},
        {0x20, 0x17, 0x12, 0x31}},

//对应结构CAPK_STRUCT,MASTER第3组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
        0x05,
        0x01,
        0x01,
        176,
        {0xB8, 0x04, 0x8A, 0xBC, 0x30, 0xC9, 0x0D, 0x97, 0x63, 0x36, 0x54, 0x3E, 0x3F, 0xD7, 0x09, 0x1C,
        0x8F, 0xE4, 0x80, 0x0D, 0xF8, 0x20, 0xED, 0x55, 0xE7, 0xE9, 0x48, 0x13, 0xED, 0x00, 0x55, 0x5B,
        0x57, 0x3F, 0xEC, 0xA3, 0xD8, 0x4A, 0xF6, 0x13, 0x1A, 0x65, 0x1D, 0x66, 0xCF, 0xF4, 0x28, 0x4F,
        0xB1, 0x3B, 0x63, 0x5E, 0xDD, 0x0E, 0xE4, 0x01, 0x76, 0xD8, 0xBF, 0x04, 0xB7, 0xFD, 0x1C, 0x7B,
        0xAC, 0xF9, 0xAC, 0x73, 0x27, 0xDF, 0xAA, 0x8A, 0xA7, 0x2D, 0x10, 0xDB, 0x3B, 0x8E, 0x70, 0xB2,
        0xDD, 0xD8, 0x11, 0xCB, 0x41, 0x96, 0x52, 0x5E, 0xA3, 0x86, 0xAC, 0xC3, 0x3C, 0x0D, 0x9D, 0x45,
        0x75, 0x91, 0x64, 0x69, 0xC4, 0xE4, 0xF5, 0x3E, 0x8E, 0x1C, 0x91, 0x2C, 0xC6, 0x18, 0xCB, 0x22,
        0xDD, 0xE7, 0xC3, 0x56, 0x8E, 0x90, 0x02, 0x2E, 0x6B, 0xBA, 0x77, 0x02, 0x02, 0xE4, 0x52, 0x2A,
        0x2D, 0xD6, 0x23, 0xD1, 0x80, 0xE2, 0x15, 0xBD, 0x1D, 0x15, 0x07, 0xFE, 0x3D, 0xC9, 0x0C, 0xA3,
        0x10, 0xD2, 0x7B, 0x3E, 0xFC, 0xCD, 0x8F, 0x83, 0xDE, 0x30, 0x52, 0xCA, 0xD1, 0xE4, 0x89, 0x38,
        0xC6, 0x8D, 0x09, 0x5A, 0xAC, 0x91, 0xB5, 0xF3, 0x7E, 0x28, 0xBB, 0x49, 0xEC, 0x7E, 0xD5, 0x97},
        1,
        {0x03},
        0x14,
        {0xEB, 0xFA, 0x0D, 0x5D, 0x06, 0xD8, 0xCE, 0x70,
        0x2D, 0xA3, 0xEA, 0xE8, 0x90, 0x70, 0x1D, 0x45,
        0xE2, 0x74, 0xC8, 0x45},
        {0x20, 0x20, 0x12, 0x31}},

//对应结构CAPK_STRUCT,MASTER第4组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x04},
        0x06,
        0x01,
        0x01,
        248,
        {0xCB, 0x26, 0xFC, 0x83, 0x0B, 0x43, 0x78, 0x5B, 0x2B, 0xCE, 0x37, 0xC8, 0x1E, 0xD3, 0x34, 0x62,
        0x2F, 0x96, 0x22, 0xF4, 0xC8, 0x9A, 0xAE, 0x64, 0x10, 0x46, 0xB2, 0x35, 0x34, 0x33, 0x88, 0x3F,
        0x30, 0x7F, 0xB7, 0xC9, 0x74, 0x16, 0x2D, 0xA7, 0x2F, 0x7A, 0x4E, 0xC7, 0x5D, 0x9D, 0x65, 0x73,
        0x36, 0x86, 0x5B, 0x8D, 0x30, 0x23, 0xD3, 0xD6, 0x45, 0x66, 0x76, 0x25, 0xC9, 0xA0, 0x7A, 0x6B,
        0x7A, 0x13, 0x7C, 0xF0, 0xC6, 0x41, 0x98, 0xAE, 0x38, 0xFC, 0x23, 0x80, 0x06, 0xFB, 0x26, 0x03,
        0xF4, 0x1F, 0x4F, 0x3B, 0xB9, 0xDA, 0x13, 0x47, 0x27, 0x0F, 0x2F, 0x5D, 0x8C, 0x60, 0x6E, 0x42,
        0x09, 0x58, 0xC5, 0xF7, 0xD5, 0x0A, 0x71, 0xDE, 0x30, 0x14, 0x2F, 0x70, 0xDE, 0x46, 0x88, 0x89,
        0xB5, 0xE3, 0xA0, 0x86, 0x95, 0xB9, 0x38, 0xA5, 0x0F, 0xC9, 0x80, 0x39, 0x3A, 0x9C, 0xBC, 0xE4,
        0x4A, 0xD2, 0xD6, 0x4F, 0x63, 0x0B, 0xB3, 0x3A, 0xD3, 0xF5, 0xF5, 0xFD, 0x49, 0x5D, 0x31, 0xF3,
        0x78, 0x18, 0xC1, 0xD9, 0x40, 0x71, 0x34, 0x2E, 0x07, 0xF1, 0xBE, 0xC2, 0x19, 0x4F, 0x60, 0x35,
        0xBA, 0x5D, 0xED, 0x39, 0x36, 0x50, 0x0E, 0xB8, 0x2D, 0xFD, 0xA6, 0xE8, 0xAF, 0xB6, 0x55, 0xB1,
        0xEF, 0x3D, 0x0D, 0x7E, 0xBF, 0x86, 0xB6, 0x6D, 0xD9, 0xF2, 0x9F, 0x6B, 0x1D, 0x32, 0x4F, 0xE8,
        0xB2, 0x6C, 0xE3, 0x8A, 0xB2, 0x01, 0x3D, 0xD1, 0x3F, 0x61, 0x1E, 0x7A, 0x59, 0x4D, 0x67, 0x5C,
        0x44, 0x32, 0x35, 0x0E, 0xA2, 0x44, 0xCC, 0x34, 0xF3, 0x87, 0x3C, 0xBA, 0x06, 0x59, 0x29, 0x87,
        0xA1, 0xD7, 0xE8, 0x52, 0xAD, 0xC2, 0x2E, 0xF5, 0xA2, 0xEE, 0x28, 0x13, 0x20, 0x31, 0xE4, 0x8F,
        0x74, 0x03, 0x7E, 0x3B, 0x34, 0xAB, 0x74, 0x7F},
        1,
        {0x03},
        0x14,
        {0xF9, 0x10, 0xA1, 0x50, 0x4D, 0x5F, 0xFB, 0x79,
        0x3D, 0x94, 0xF3, 0xB5, 0x00, 0x76, 0x5E, 0x1A,
        0xBC, 0xAD, 0x72, 0xD9},
        {0x20, 0x20, 0x12, 0x31}},

//===========JCB CAPK开始===========================================================
//对应结构CAPK_STRUCT,JCB第1组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
        0x09,            //CAPK索引
        0x01,            //HASH算法标识
        0x01,            //RSA算法标识，要与HASH算法标识相同
        0x80,            //模长度
        {0xB7, 0x2A, 0x8F, 0xEF, 0x5B, 0x27, 0xF2, 0xB5, 0x50, 0x39, 0x8F, 0xDC, 0xC2, 0x56, 0xF7, 0x14,
        0xBA, 0xD4, 0x97, 0xFF, 0x56, 0x09, 0x4B, 0x74, 0x08, 0x32, 0x8C, 0xB6, 0x26, 0xAA, 0x6F, 0x0E,
        0x6A, 0x9D, 0xF8, 0x38, 0x8E, 0xB9, 0x88, 0x7B, 0xC9, 0x30, 0x17, 0x0B, 0xCC, 0x12, 0x13, 0xE9,
        0x0F, 0xC0, 0x70, 0xD5, 0x2C, 0x8D, 0xCD, 0x0F, 0xF9, 0xE1, 0x0F, 0xAD, 0x36, 0x80, 0x1F, 0xE9,
        0x3F, 0xC9, 0x98, 0xA7, 0x21, 0x70, 0x50, 0x91, 0xF1, 0x8B, 0xC7, 0xC9, 0x82, 0x41, 0xCA, 0xDC,
        0x15, 0xA2, 0xB9, 0xDA, 0x7F, 0xB9, 0x63, 0x14, 0x2C, 0x0A, 0xB6, 0x40, 0xD5, 0xD0, 0x13, 0x5E,
        0x77, 0xEB, 0xAE, 0x95, 0xAF, 0x1B, 0x4F, 0xEF, 0xAD, 0xCF, 0x9C, 0x01, 0x23, 0x66, 0xBD, 0xDA,
        0x04, 0x55, 0xC1, 0x56, 0x4A, 0x68, 0x81, 0x0D, 0x71, 0x27, 0x67, 0x6D, 0x49, 0x38, 0x90, 0xBD},
        0x01, //指数长度
        {0x03}, //指数
        0x14, //CHECKSUM 长度
        {0x44, 0x10, 0xC6, 0xD5, 0x1C, 0x2F, 0x83, 0xAD, 0xFD, 0x92, 0x52, 0x8F, 0xA6, 0xE3, 0x8A, 0x32,
        0xDF, 0x04, 0x8D, 0x0A}, //CHECKSUM
        {0x20, 0x12, 0x12, 0x31}, //有效期
        },
        //对应结构CAPK_STRUCT,JCB第2组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
        0x10,            //CAPK索引
        0x01,            //HASH算法标识
        0x01,            //RSA算法标识，要与HASH算法标识相同
        0x90,            //模长度
        {0x99, 0xB6, 0x34, 0x64, 0xEE, 0x0B, 0x49, 0x57, 0xE4, 0xFD, 0x23, 0xBF, 0x92, 0x3D, 0x12, 0xB6,
        0x14, 0x69, 0xB8, 0xFF, 0xF8, 0x81, 0x43, 0x46, 0xB2, 0xED, 0x6A, 0x78, 0x0F, 0x89, 0x88, 0xEA,
        0x9C, 0xF0, 0x43, 0x3B, 0xC1, 0xE6, 0x55, 0xF0, 0x5E, 0xFA, 0x66, 0xD0, 0xC9, 0x80, 0x98, 0xF2,
        0x5B, 0x65, 0x9D, 0x7A, 0x25, 0xB8, 0x47, 0x8A, 0x36, 0xE4, 0x89, 0x76, 0x0D, 0x07, 0x1F, 0x54,
        0xCD, 0xF7, 0x41, 0x69, 0x48, 0xED, 0x73, 0x3D, 0x81, 0x63, 0x49, 0xDA, 0x2A, 0xAD, 0xDA, 0x22,
        0x7E, 0xE4, 0x59, 0x36, 0x20, 0x3C, 0xBF, 0x62, 0x8C, 0xD0, 0x33, 0xAA, 0xBA, 0x5E, 0x5A, 0x6E,
        0x4A, 0xE3, 0x7F, 0xBA, 0xCB, 0x46, 0x11, 0xB4, 0x11, 0x3E, 0xD4, 0x27, 0x52, 0x9C, 0x63, 0x6F,
        0x6C, 0x33, 0x04, 0xF8, 0xAB, 0xDD, 0x6D, 0x9A, 0xD6, 0x60, 0x51, 0x6A, 0xE8, 0x7F, 0x7F, 0x2D,
        0xDF, 0x1D, 0x2F, 0xA4, 0x4C, 0x16, 0x47, 0x27, 0xE5, 0x6B, 0xBC, 0x9B, 0xA2, 0x3C, 0x02, 0x85},
        0x01, //指数长度
        {0x03}, //指数
        0x14, //CHECKSUM 长度
        {0xC7, 0x5E, 0x52, 0x10, 0xCB, 0xE6, 0xE8, 0xF0, 0x59, 0x4A, 0x0F, 0x19, 0x11, 0xB0, 0x74, 0x18,
        0xCA, 0xDB, 0x5B, 0xAB}, //CHECKSUM
        {0x20, 0x17, 0x12, 0x31}, //有效期
        },
//对应结构CAPK_STRUCT,JCB第3组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
        0x12,            //CAPK索引
        0x01,            //HASH算法标识
        0x01,            //RSA算法标识，要与HASH算法标识相同
        0xB0,            //模长度
        {0xAD, 0xF0, 0x5C, 0xD4, 0xC5, 0xB4, 0x90, 0xB0, 0x87, 0xC3, 0x46, 0x7B, 0x0F, 0x30, 0x43, 0x75,
        0x04, 0x38, 0x84, 0x84, 0x61, 0x28, 0x8B, 0xFE, 0xFD, 0x61, 0x98, 0xDD, 0x57, 0x6D, 0xC3, 0xAD,
        0x7A, 0x7C, 0xFA, 0x07, 0xDB, 0xA1, 0x28, 0xC2, 0x47, 0xA8, 0xEA, 0xB3, 0x0D, 0xC3, 0xA3, 0x0B,
        0x02, 0xFC, 0xD7, 0xF1, 0xC8, 0x16, 0x79, 0x65, 0x46, 0x36, 0x26, 0xFE, 0xFF, 0x8A, 0xB1, 0xAA,
        0x61, 0xA4, 0xB9, 0xAE, 0xF0, 0x9E, 0xE1, 0x2B, 0x00, 0x98, 0x42, 0xA1, 0xAB, 0xA0, 0x1A, 0xDB,
        0x4A, 0x2B, 0x17, 0x06, 0x68, 0x78, 0x1E, 0xC9, 0x2B, 0x60, 0xF6, 0x05, 0xFD, 0x12, 0xB2, 0xB2,
        0xA6, 0xF1, 0xFE, 0x73, 0x4B, 0xE5, 0x10, 0xF6, 0x0D, 0xC5, 0xD1, 0x89, 0xE4, 0x01, 0x45, 0x1B,
        0x62, 0xB4, 0xE0, 0x68, 0x51, 0xEC, 0x20, 0xEB, 0xFF, 0x45, 0x22, 0xAA, 0xCC, 0x2E, 0x9C, 0xDC,
        0x89, 0xBC, 0x5D, 0x8C, 0xDE, 0x5D, 0x63, 0x3C, 0xFD, 0x77, 0x22, 0x0F, 0xF6, 0xBB, 0xD4, 0xA9,
        0xB4, 0x41, 0x47, 0x3C, 0xC3, 0xC6, 0xFE, 0xFC, 0x8D, 0x13, 0xE5, 0x7C, 0x3D, 0xE9, 0x7E, 0x12,
        0x69, 0xFA, 0x19, 0xF6, 0x55, 0x21, 0x5B, 0x23, 0x56, 0x3E, 0xD1, 0xD1, 0x86, 0x0D, 0x86, 0x81},
        0x01, //指数长度
        {0x03}, //指数
        0x14, //CHECKSUM 长度
        {0x87, 0x4B, 0x37, 0x9B, 0x7F, 0x60, 0x7D, 0xC1, 0xCA, 0xF8, 0x7A, 0x19, 0xE4, 0x00, 0xB6, 0xA9,
        0xE2, 0x51, 0x63, 0xE8}, //CHECKSUM
        {0x20, 0x20, 0x12, 0x31}, //有效期
        },
//对应结构CAPK_STRUCT,JCB第4组公钥
        {{0xA0, 0x00, 0x00, 0x00, 0x65}, //RID
        0x14,            //CAPK索引
        0x01,            //HASH算法标识
        0x01,            //RSA算法标识，要与HASH算法标识相同
        0xF8,            //模长度
        {0xAE, 0xED, 0x55, 0xB9, 0xEE, 0x00, 0xE1, 0xEC, 0xEB, 0x04, 0x5F, 0x61, 0xD2, 0xDA, 0x9A, 0x66,
        0xAB, 0x63, 0x7B, 0x43, 0xFB, 0x5C, 0xDB, 0xDB, 0x22, 0xA2, 0xFB, 0xB2, 0x5B, 0xE0, 0x61, 0xE9,
        0x37, 0xE3, 0x82, 0x44, 0xEE, 0x51, 0x32, 0xF5, 0x30, 0x14, 0x4A, 0x3F, 0x26, 0x89, 0x07, 0xD8,
        0xFD, 0x64, 0x88, 0x63, 0xF5, 0xA9, 0x6F, 0xED, 0x7E, 0x42, 0x08, 0x9E, 0x93, 0x45, 0x7A, 0xDC,
        0x0E, 0x1B, 0xC8, 0x9C, 0x58, 0xA0, 0xDB, 0x72, 0x67, 0x5F, 0xBC, 0x47, 0xFE, 0xE9, 0xFF, 0x33,
        0xC1, 0x6A, 0xDE, 0x6D, 0x34, 0x19, 0x36, 0xB0, 0x6B, 0x6A, 0x6F, 0x5E, 0xF6, 0xF6, 0x6A, 0x4E,
        0xDD, 0x98, 0x1D, 0xF7, 0x5D, 0xA8, 0x39, 0x9C, 0x30, 0x53, 0xF4, 0x30, 0xEC, 0xA3, 0x42, 0x43,
        0x7C, 0x23, 0xAF, 0x42, 0x3A, 0x21, 0x1A, 0xC9, 0xF5, 0x8E, 0xAF, 0x09, 0xB0, 0xF8, 0x37, 0xDE,
        0x9D, 0x86, 0xC7, 0x10, 0x9D, 0xB1, 0x64, 0x65, 0x61, 0xAA, 0x5A, 0xF0, 0x28, 0x9A, 0xF5, 0x51,
        0x4A, 0xC6, 0x4B, 0xC2, 0xD9, 0xD3, 0x6A, 0x17, 0x9B, 0xB8, 0xA7, 0x97, 0x1E, 0x2B, 0xFA, 0x03,
        0xA9, 0xE4, 0xB8, 0x47, 0xFD, 0x3D, 0x63, 0x52, 0x4D, 0x43, 0xA0, 0xE8, 0x00, 0x35, 0x47, 0xB9,
        0x4A, 0x8A, 0x75, 0xE5, 0x19, 0xDF, 0x31, 0x77, 0xD0, 0xA6, 0x0B, 0xC0, 0xB4, 0xBA, 0xB1, 0xEA,
        0x59, 0xA2, 0xCB, 0xB4, 0xD2, 0xD6, 0x23, 0x54, 0xE9, 0x26, 0xE9, 0xC7, 0xD3, 0xBE, 0x41, 0x81,
        0xE8, 0x1B, 0xA6, 0x0F, 0x82, 0x85, 0xA8, 0x96, 0xD1, 0x7D, 0xA8, 0xC3, 0x24, 0x24, 0x81, 0xB6,
        0xC4, 0x05, 0x76, 0x9A, 0x39, 0xD5, 0x47, 0xC7, 0x4E, 0xD9, 0xFF, 0x95, 0xA7, 0x0A, 0x79, 0x60,
        0x46, 0xB5, 0xEF, 0xF3, 0x66, 0x82, 0xDC, 0x29},
        0x01, //指数长度
        {0x03}, //指数
        0x14, //CHECKSUM 长度
        {0xC0, 0xD1, 0x5F, 0x6C, 0xD9, 0x57, 0xE4, 0x91, 0xDB, 0x56, 0xDC, 0xDD, 0x1C, 0xA8, 0x7A, 0x03,
        0xEB, 0xE0, 0x6B, 0x7B}, //CHECKSUM
        {0x20, 0x20, 0x12, 0x31}, //有效期
        },
//对应电子现金测试公钥 huxiang add 100726
        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
        0x08,            //CAPK索引
        0x01,            //HASH算法标识
        0x01,            //RSA算法标识，要与HASH算法标识相同
        0x90,            //模长度
        {0xB6, 0x16, 0x45, 0xED, 0xFD, 0x54, 0x98, 0xFB, 0x24, 0x64, 0x44, 0x03, 0x7A, 0x0F, 0xA1,
        0x8C, 0x0F, 0x10, 0x1E, 0xBD, 0x8E, 0xFA, 0x54, 0x57, 0x3C, 0xE6, 0xE6, 0xA7, 0xFB, 0xF6,
        0x3E, 0xD2, 0x1D, 0x66, 0x34, 0x08, 0x52, 0xB0, 0x21, 0x1C, 0xF5, 0xEE, 0xF6, 0xA1, 0xCD,
        0x98, 0x9F, 0x66, 0xAF, 0x21, 0xA8, 0xEB, 0x19, 0xDB, 0xD8, 0xDB, 0xC3, 0x70, 0x6D, 0x13,
        0x53, 0x63, 0xA0, 0xD6, 0x83, 0xD0, 0x46, 0x30, 0x4F, 0x5A, 0x83, 0x6B, 0xC1, 0xBC, 0x63,
        0x28, 0x21, 0xAF, 0xE7, 0xA2, 0xF7, 0x5D, 0xA3, 0xC5, 0x0A, 0xC7, 0x4C, 0x54, 0x5A, 0x75,
        0x45, 0x62, 0x20, 0x41, 0x37, 0x16, 0x96, 0x63, 0xCF, 0xCC, 0x0B, 0x06, 0xE6, 0x7E, 0x21,
        0x09, 0xEB, 0xA4, 0x1B, 0xC6, 0x7F, 0xF2, 0x0C, 0xC8, 0xAC, 0x80, 0xD7, 0xB6, 0xEE, 0x1A,
        0x95, 0x46, 0x5B, 0x3B, 0x26, 0x57, 0x53, 0x3E, 0xA5, 0x6D, 0x92, 0xD5, 0x39, 0xE5, 0x06,
               0x43, 0x60, 0xEA, 0x48, 0x50, 0xFE, 0xD2, 0xD1, 0xBF},
        0x01, //指数长度
        {0x03}, //指数
        0x14, //CHECKSUM 长度
        {0xEE, 0x23, 0xB6, 0x16, 0xC9, 0x5C, 0x02, 0x65, 0x2A, 0xD1, 0x88, 0x60, 0xE4, 0x87, 0x87, 0xC0,
        0x79, 0xE8, 0xE8, 0x5A}, //CHECKSUM
        {0x20, 0x49, 0x12, 0x31}, //有效期
        },
//对应大运会公钥 huxiang add 101129
        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
        0x02,               //CAPK索引
        0x01,               //HASH算法标识
        0x01,               //RSA算法标识，要与HASH算法标识相同
        0x90,              //模长度
        {0xA3, 0x76, 0x7A, 0xBD, 0x1B, 0x6A, 0xA6, 0x9D, 0x7F, 0x3F, 0xBF, 0x28, 0xC0, 0x92, 0xDE, 0x9E,
        0xD1, 0xE6, 0x58, 0xBA, 0x5F, 0x09, 0x09, 0xAF, 0x7A, 0x1C, 0XCD, 0x90, 0x73, 0x73, 0xB7, 0x21,
        0x0F, 0xDE, 0xB1, 0x62, 0x87, 0xBA, 0x8E, 0x78, 0xE1, 0x52, 0x9F, 0x44, 0x39, 0x76, 0xFD, 0x27,
        0xF9, 0x91, 0xEC, 0x67, 0xD9, 0x5E, 0x5F, 0x4E, 0x96, 0xB1, 0x27, 0xCA, 0xB2, 0x39, 0x6A, 0x94,
        0xD6, 0xE4, 0x5C, 0xDA, 0x44, 0xCA, 0x4C, 0x48, 0x67, 0x57, 0x0D, 0x6B, 0x07, 0x54, 0x2F, 0x8D,
        0x4B, 0xF9, 0xFF, 0x97, 0x97, 0x5D, 0xB9, 0x89, 0x15, 0x15, 0xE6, 0x6F, 0x52, 0x5D, 0x2B, 0x3C,
        0xBE, 0xB6, 0xD6, 0x62, 0xBF, 0xB6, 0xC3, 0xF3, 0x38, 0xE9, 0x3B, 0x02, 0x14, 0x2B, 0xFC, 0x44,
        0x17, 0x3A, 0x37, 0x64, 0xC5, 0x6A, 0xAD, 0xD2, 0x02, 0x07, 0x5B, 0x26, 0xDC, 0x2F, 0x9F, 0x7D,
        0x7A, 0xE7, 0x4B, 0xD7, 0xD0, 0x0F, 0xD0, 0x5E, 0xE4, 0x30, 0x03, 0x26, 0x63, 0xD2, 0x7A, 0x57, },
        0x01, //指数长度
        {0x03}, //指数
        0x14,       //CHECKSUM 长度
        {0x03, 0xBB, 0x33, 0x5A, 0x85, 0x49, 0xA0, 0x3B, 0x87, 0xAB, 0x08, 0x9D, 0x00, 0x6F, 0x60, 0x85,
        0x2E, 0x4B, 0x80, 0x60}, //CHECKSUM
        {0x20, 0x20, 0x12, 0x31}, //有效期
        },


        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
        0x03,               //CAPK索引
        0x01,               //HASH算法标识
        0x01,               //RSA算法标识，要与HASH算法标识相同
        0xB0,              //模长度
        {0xB0, 0x62, 0x7D, 0xEE, 0x87, 0x86, 0x4F, 0x9C, 0x18, 0xC1, 0x3B, 0x9A, 0x1F, 0x02, 0x54, 0x48,
        0xBF, 0x13, 0xC5, 0x83, 0x80, 0xC9, 0x1F, 0x4C, 0xEB, 0xA9, 0xF9, 0xBC, 0xB2, 0x14, 0xFF, 0x84,
        0x14, 0xE9, 0xB5, 0x9D, 0x6A, 0xBA, 0x10, 0xF9, 0x41, 0xC7, 0x33, 0x17, 0x68, 0xF4, 0x7B, 0x21,
        0x27, 0x90, 0x7D, 0x85, 0x7F, 0xA3, 0x9A, 0xAF, 0x8C, 0xE0, 0x20, 0x45, 0xDD, 0x01, 0x61, 0x9D,
        0x68, 0x9E, 0xE7, 0x31, 0xC5, 0x51, 0x15, 0x9B, 0xE7, 0xEB, 0x2D, 0x51, 0xA3, 0x72, 0xFF, 0x56,
        0xB5, 0x56, 0xE5, 0xCB, 0x2F, 0xDE, 0x36, 0xE2, 0x30, 0x73, 0xA4, 0x4C, 0xA2, 0x15, 0xD6, 0xC2,
        0x6C, 0xA6, 0x88, 0x47, 0xB3, 0x88, 0xE3, 0x95, 0x20, 0xE0, 0x02, 0x6E, 0x62, 0x29, 0x4B, 0x55,
        0x7D, 0x64, 0x70, 0x44, 0x0C, 0xA0, 0xAE, 0xFC, 0x94, 0x38, 0xC9, 0x23, 0xAE, 0xC9, 0xB2, 0x09,
        0x8D, 0x6D, 0x3A, 0x1A, 0xF5, 0xE8, 0xB1, 0xDE, 0x36, 0xF4, 0xB5, 0x30, 0x40, 0x10, 0x9D, 0x89,
        0xB7, 0x7C, 0xAF, 0xAF, 0x70, 0xC2, 0x6C, 0x60, 0x1A, 0xBD, 0xF5, 0x9E, 0xEC, 0x0F, 0xDC, 0x8A,
        0x99, 0x08, 0x91, 0x40, 0xCD, 0x2E, 0x81, 0x7E, 0x33, 0x51, 0x75, 0xB0, 0x3B, 0x7A, 0xA3, 0x3D},
        0x01, //指数长度
        {0x03}, //指数
        0x14,       //CHECKSUM 长度
        {0x87, 0xF0, 0xCD, 0x7C, 0x0E, 0x86, 0xF3, 0x8F, 0x89, 0xA6, 0x6F, 0x8C, 0x47, 0x07, 0x1A, 0x8B,
        0x88, 0x58, 0x6F, 0x26}, //CHECKSUM
        {0x20, 0x20, 0x12, 0x31}, //有效期
        },


        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
        0x04,               //CAPK索引
        0x01,               //HASH算法标识
        0x01,               //RSA算法标识，要与HASH算法标识相同
        0xF8,              //模长度
        {0xBC, 0x85, 0x3E, 0x6B, 0x53, 0x65, 0xE8, 0x9E, 0x7E, 0xE9, 0x31, 0x7C, 0x94, 0xB0, 0x2D, 0x0A,
        0xBB, 0x0D, 0xBD, 0x91, 0xC0, 0x5A, 0x22, 0x4A, 0x25, 0x54, 0xAA, 0x29, 0xED, 0x9F, 0xCB, 0x9D,
        0x86, 0xEB, 0x9C, 0xCB, 0xB3, 0x22, 0xA5, 0x78, 0x11, 0xF8, 0x61, 0x88, 0xAA, 0xC7, 0x35, 0x1C,
        0x72, 0xBD, 0x9E, 0xF1, 0x96, 0xC5, 0xA0, 0x1A, 0xCE, 0xF7, 0xA4, 0xEB, 0x0D, 0x2A, 0xD6, 0x3D,
        0x9E, 0x6A, 0xC2, 0xE7, 0x83, 0x65, 0x47, 0xCB, 0x15, 0x95, 0xC6, 0x8B, 0xCB, 0xAF, 0xD0, 0xF6,
        0x72, 0x87, 0x60, 0xF3, 0xA7, 0xCA, 0x7B, 0x97, 0x30, 0x1B, 0x7E, 0x02, 0x20, 0x18, 0x4E, 0xFC,
        0x4F, 0x65, 0x30, 0x08, 0xD9, 0x3C, 0xE0, 0x98, 0xC0, 0xD9, 0x3B, 0x45, 0x20, 0x10, 0x96, 0xD1,
        0xAD, 0xFF, 0x4C, 0xF1, 0xF9, 0xFC, 0x02, 0xAF, 0x75, 0x9D, 0xA2, 0x7C, 0xD6, 0xDF, 0xD6, 0xD7,
        0x89, 0xB0, 0x99, 0xF1, 0x6F, 0x37, 0x8B, 0x61, 0x00, 0x33, 0x4E, 0x63, 0xF3, 0xD3, 0x5F, 0x32,
        0x51, 0xA5, 0xEC, 0x78, 0x69, 0x37, 0x31, 0xF5, 0x23, 0x35, 0x19, 0xCD, 0xB3, 0x80, 0xF5, 0xAB,
        0x8C, 0x0F, 0x02, 0x72, 0x8E, 0x91, 0xD4, 0x69, 0xAB, 0xD0, 0xEA, 0xE0, 0xD9, 0x3B, 0x1C, 0xC6,
        0x6C, 0xE1, 0x27, 0xB2, 0x9C, 0x7D, 0x77, 0x44, 0x1A, 0x49, 0xD0, 0x9F, 0xCA, 0x5D, 0x6D, 0x97,
        0x62, 0xFC, 0x74, 0xC3, 0x1B, 0xB5, 0x06, 0xC8, 0xBA, 0xE3, 0xC7, 0x9A, 0xD6, 0xC2, 0x57, 0x87,
        0x75, 0xB9, 0x59, 0x56, 0xB5, 0x37, 0x0D, 0x1D, 0x05, 0x19, 0xE3, 0x79, 0x06, 0xB3, 0x84, 0x73,
        0x62, 0x33, 0x25, 0x1E, 0x8F, 0x09, 0xAD, 0x79, 0xDF, 0xBE, 0x2C, 0x6A, 0xBF, 0xAD, 0xAC, 0x8E,
        0x4D, 0x86, 0x24, 0x31, 0x8C, 0x27, 0xDA, 0xF1},
        0x01, //指数长度
        {0x03}, //指数
        0x14,       //CHECKSUM 长度
        {0xF5, 0x27, 0x08, 0x1C, 0xF3, 0x71, 0xDD, 0x7E, 0x1F, 0xD4, 0xFA, 0x41, 0x4A, 0x66, 0x50, 0x36,
        0xE0, 0xF5, 0xE6, 0xE5}, //CHECKSUM
        {0x20, 0x20, 0x12, 0x31}, //有效期
        },

            //银联MPOS 添加上的
        {{0xA0, 0x00, 0x00, 0x03, 0x33}, //RID
        0x01,               //CAPK索引
        0x01,               //HASH算法标识
        0x01,               //RSA算法标识，要与HASH算法标识相同
        0x80,              //模长度
        {0xBB,0xE9,0x06,0x6D,0x25,0x17,0x51,0x1D,0x23,0x9C,0x7B,0xFA,0x77,0x88,0x41,0x44,
        0xAE,0x20,0xC7,0x37,0x2F,0x51,0x51,0x47,0xE8,0xCE,0x65,0x37,0xC5,0x4C,0x0A,0x6A,
        0x4D,0x45,0xF8,0xCA,0x4D,0x29,0x08,0x70,0xCD,0xA5,0x9F,0x13,0x44,0xEF,0x71,0xD1,
        0x7D,0x3F,0x35,0xD9,0x2F,0x3F,0x06,0x77,0x8D,0x0D,0x51,0x1E,0xC2,0xA7,0xDC,0x4F,
        0xFE,0xAD,0xF4,0xFB,0x12,0x53,0xCE,0x37,0xA7,0xB2,0xB5,0xA3,0x74,0x12,0x27,0xBE,
        0xF7,0x25,0x24,0xDA,0x7A,0x2B,0x7B,0x1C,0xB4,0x26,0xBE,0xE2,0x7B,0xC5,0x13,0xB0,
        0xCB,0x11,0xAB,0x99,0xBC,0x1B,0xC6,0x1D,0xF5,0xAC,0x6C,0xC4,0xD8,0x31,0xD0,0x84,
        0x87,0x88,0xCD,0x74,0xF6,0xD5,0x43,0xAD,0x37,0xC5,0xA2,0xB4,0xC5,0xD5,0xA9,0x3B
        },
        0x01, //指数长度
        {0x03}, //指数
        0x14,       //CHECKSUM 长度
        {0xE8,0x81,0xE3,0x90,0x67,0x5D,0x44,0xC2,0xDD,0x81,0x23,0x4D,0xCE,0x29,0xC3,0xF5,
        0xAB,0x22,0x97,0xA0
        }, //CHECKSUM
        {0x20, 0x09, 0x12, 0x31}, //有效期
        }
};

#ifndef  XGD_SDK_DEBUG
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
#endif

/*****************************************************************************
** Descriptions:	        ��ʼ��JNI����
** Parameters:           none
** Returned value:	     jint
** Created By:		liyang 2015-8-10
** Remarks:
*****************************************************************************/

JAVA_INTERFACE_EMVL2 *java_interface_emvl2;
JNIEXPORT jbyteArray JNICALL Java_com_nexgo_libpboc_EmvL2_startPBOC(JNIEnv *env,
                                                                jobject obj,
                                                                jbyteArray tlv,
                                                                jint tlvLen,
                                                                 jboolean isRf,
                                                                    jbyteArray key//xiaox 15/11/24 密钥
)
{
    s32 ret = 0;
    u8 dolData[512] = {0};
    s32 dolLen = 0;

	u16 len = 0;

    if (javaInterface == NULL) {
        javaInterface = (JAVA_INTERFACE *) malloc(sizeof(JAVA_INTERFACE));
        memset(javaInterface, 0, sizeof(JAVA_INTERFACE));
    }

    if (java_interface_emvl2 == NULL) {
        java_interface_emvl2 = (JAVA_INTERFACE_EMVL2 *) malloc(sizeof(JAVA_INTERFACE_EMVL2));
        memset(java_interface_emvl2, 0, sizeof(JAVA_INTERFACE_EMVL2));
    }

    javaInterface->myEnv = env;

    javaInterface->gJniClass = (*(javaInterface->myEnv))->FindClass((javaInterface->myEnv),
                                                                    "com/nexgo/libpboc/fetchdata/FetchDataDeal");
    if (javaInterface->gJniClass == 0 || javaInterface->gJniClass == NULL)
        return (*env)->NewStringUTF(env, "-1");

    javaInterface->construction_id = (*env)->GetMethodID(env, javaInterface->gJniClass, "<init>",
                                                         "()V");

    javaInterface->gJniObj = (*env)->NewObject(env, javaInterface->gJniClass,
                                               javaInterface->construction_id);


    javaInterface->jniApduIC = (*env)->GetMethodID(env, javaInterface->gJniClass, "jniApduIC",
                                                   "(Ljava/lang/String;)Ljava/lang/String;");
    if (javaInterface->jniApduIC == 0 || javaInterface->jniApduIC == NULL)
        return (*env)->NewStringUTF(env, "-1");
    //xiaox 15/11/24
    javaInterface->jniIcTrack = (*env)->GetMethodID(env, javaInterface->gJniClass, "jniIcTrack",
                                                   "()Ljava/lang/String;");
    if (javaInterface->jniIcTrack == 0 || javaInterface->jniIcTrack == NULL)
        return (*env)->NewStringUTF(env, "-1");

    //emvl2的回调
    java_interface_emvl2->myEnv = env;

    java_interface_emvl2->gJniClass = (*(env))->FindClass(env, "com/nexgo/libpboc/EmvL2");
    if (java_interface_emvl2->gJniClass == 0 || java_interface_emvl2->gJniClass == NULL)
        return (*env)->NewStringUTF(env, "-1");

    java_interface_emvl2->construction_id = (*env)->GetMethodID(env,
                                                                java_interface_emvl2->gJniClass,
                                                                "<init>",
                                                                "()V");

    java_interface_emvl2->gJniObj = (*env)->NewObject(env, java_interface_emvl2->gJniClass,
                                                      java_interface_emvl2->construction_id);

    java_interface_emvl2->jniInputPwd = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                            "jniInputPwd",
                                                            "(Ljava/lang/String;II)Ljava/lang/String;");
    if (java_interface_emvl2->jniInputPwd == 0 || java_interface_emvl2->jniInputPwd == NULL)
        return (*env)->NewStringUTF(env, "-1");

    java_interface_emvl2->jniConfirmCardNo = (*env)->GetMethodID(env,
                                                                 java_interface_emvl2->gJniClass,
                                                                 "jniConfirmCardNo",
                                                                 "(Ljava/lang/String;)Ljava/lang/String;");
    if (java_interface_emvl2->jniConfirmCardNo == 0 ||
        java_interface_emvl2->jniConfirmCardNo == NULL)
        return (*env)->NewStringUTF(env, "-1");

    java_interface_emvl2->jniSelApp = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                          "jniSelApp",
                                                          "([B)Ljava/lang/String;");
    if (java_interface_emvl2->jniSelApp == 0 || java_interface_emvl2->jniSelApp == NULL)
        return (*env)->NewStringUTF(env, "-1");

    java_interface_emvl2->jniCerVerify = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                            "jniCerVerify",
                                                            "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if (java_interface_emvl2->jniCerVerify == 0 || java_interface_emvl2->jniCerVerify == NULL)
        return (*env)->NewStringUTF(env, "-1");

    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);
    gTradeTlvLen = (u16) tlvLen;
    memset(gTradeTlv, 0, sizeof(gTradeTlv));
    memcpy(gTradeTlv, (u8 *) pTlv, gTradeTlvLen);

    TraceHex("emv","Recv TLV", gTradeTlv,gTradeTlvLen);
//    memset(gasTradeAmount,0,sizeof(gasTradeAmount));
//    memset(gasTradeAmount,'0',12);
//    memcpy(&gasTradeAmount[strlen(gasTradeAmount)-strlen(amount)],(u8*)amount,strlen(amount));
//    LOGD("gasTradeAmount %s", amount);
    (*env)->ReleaseByteArrayElements(env, tlv, pTlv, JNI_ABORT);
    //xiaox 15/11/24
    jbyte *pKey = (*env)->GetByteArrayElements(env, key, 0);
    if(pKey != NULL) {
        sdkIccSetFactorAndIcKey(pKey);
    }
    (*env)->ReleaseByteArrayElements(env, key, pKey, JNI_ABORT);

    ret = SdkIccEmv((bool)isRf);

    SetFlowResult(ret);
//xiaox 15/11/24
	memset(dolData,0,sizeof(dolData));
	dolLen = 0;

	sdkIccGetIcEnTrack(dolData);
	sdkBcdToU16(&len,dolData,2);
	dolLen += (len + 2);
    ZipPagData(false, (u8 *) &dolData[dolLen+2], (u16 * ) & len);
	sdkU16ToBcd(&dolData[dolLen],len,2);
	dolLen += (len+2);

    jbyteArray arrayData = (*env)->NewByteArray(env, dolLen);
    (*env)->SetByteArrayRegion(env, arrayData, 0, dolLen, (jbyte *) dolData);

    return arrayData;
}

JNIEXPORT jint JNICALL Java_com_nexgo_libpboc_EmvL2_transFlow1(JNIEnv *env,
                                                              jobject obj,
                                                              jbyteArray tlv,
                                                              jint tlvLen,
                                                               jboolean isRf
)
{
    s32 ret = 0;
    u8 dolData[256] = {0};
    s32 dolLen = 0;
    u8 asDolData[512] = {0};


    if (java_interface_emvl2 == NULL) {
        java_interface_emvl2 = (JAVA_INTERFACE_EMVL2 *) malloc(sizeof(JAVA_INTERFACE_EMVL2));
        memset(java_interface_emvl2, 0, sizeof(JAVA_INTERFACE_EMVL2));
    }

    //emvl2
    java_interface_emvl2->myEnv = env;

    java_interface_emvl2->gJniClass = (*(env))->FindClass(env, "com/nexgo/libpboc/EmvL2");
    if (java_interface_emvl2->gJniClass == 0 || java_interface_emvl2->gJniClass == NULL)
        return (jint)(-1);

    java_interface_emvl2->construction_id = (*env)->GetMethodID(env,
                                                                java_interface_emvl2->gJniClass,
                                                                "<init>",
                                                                "()V");

    java_interface_emvl2->gJniObj = (*env)->NewObject(env, java_interface_emvl2->gJniClass,
                                                      java_interface_emvl2->construction_id);

    java_interface_emvl2->jniInputPwd = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                            "jniInputPwd",
                                                            "(Ljava/lang/String;II)Ljava/lang/String;");
    if (java_interface_emvl2->jniInputPwd == 0 || java_interface_emvl2->jniInputPwd == NULL)
        return (jint)(-1);

    java_interface_emvl2->jniConfirmCardNo = (*env)->GetMethodID(env,
                                                                 java_interface_emvl2->gJniClass,
                                                                 "jniConfirmCardNo",
                                                                 "(Ljava/lang/String;)Ljava/lang/String;");
    if (java_interface_emvl2->jniConfirmCardNo == 0 ||
        java_interface_emvl2->jniConfirmCardNo == NULL)
        return (jint)(-1);

    java_interface_emvl2->jniSelApp = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                          "jniSelApp",
                                                          "([B)Ljava/lang/String;");
    if (java_interface_emvl2->jniSelApp == 0 || java_interface_emvl2->jniSelApp == NULL)
        return (jint)(-1);

    java_interface_emvl2->jniCerVerify = (*env)->GetMethodID(env, java_interface_emvl2->gJniClass,
                                                          "jniCerVerify",
                                                          "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
    if (java_interface_emvl2->jniCerVerify == 0 || java_interface_emvl2->jniCerVerify == NULL)
        return (jint)(-1);


    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);
    gTradeTlvLen = (u16) tlvLen;
    memset(gTradeTlv, 0, sizeof(gTradeTlv));
    memcpy(gTradeTlv, (u8 *) pTlv, gTradeTlvLen);

    ret = SdkIccEmv((bool)isRf);
    (*env)->ReleaseByteArrayElements(env, tlv, pTlv, JNI_ABORT);
    return (jint) ret;
}

void SetFlowResult(s32 result) {
    switch (result) {
        //成功
        case SDK_IccEasyFlow:
        case SDK_DispLog:
        case SDK_ReadLogEnd:
            sdkIccConfigTLV("\xdf\x75", "\x00", 1);
            break;
            //交易接受
        case SDK_TransOfflineApprove: //返回脱机批准
        case SDK_TransOnlineApprove: //交易联机批准
            sdkIccConfigTLV("\xdf\x75", "\x01", 1);
            break;
            //交易拒绝
        case SDK_TransOfflineDeclineErr: //交易脱机拒绝
        case SDK_TransOnlineDeclineErr: //交易联机拒绝
            sdkIccConfigTLV("\xdf\x75", "\x02", 1);
            break;
            //联机
        case SDK_OnlineWait: //返回直接联机
            sdkIccConfigTLV("\xdf\x75", "\x03", 1);
            break;
            //AAC
        case SDK_TransOK_ARPCErr: //APRC错但卡片允许交易成功
            sdkIccConfigTLV("\xdf\x75", "\x04", 1);
            break;
            //fallback
        case SDK_NeedMagCardErr: //返回fallback流程
            sdkIccConfigTLV("\xdf\x75", "\xfe", 1);
            break;
            //交易失败
        case SDK_QpbocDDAAuthFail://qpboc数据认证失败zxx 20130716 22:46
        case SDK_QpbocReadLastLogErr://qpboc读最后一条记录失败zxx 20130716 22:46
        case SDK_CancelTransErr: //直接退出
        case SDK_NeedReversal: //需要冲正
        case SDK_PSENotFoundErr:        //走非接手机芯片
        default:
            sdkIccConfigTLV("\xdf\x75", "\xff", 1);
            break;
    }
}
//zhouxiaoxin add
//JNIEXPORT jstring JNICALL Java_com_nexgo_libpboc_EmvL2_zipPageData(JNIEnv *env, jobject obj,
//                                                                         jstring tradeType)
//{
//    char dolData[256] = {0};
//    s32 dolLen = 0;
//    char asDolData[512] = {0};
//    const jbyte *trade = (*env)->GetStringUTFChars(env, tradeType, NULL);
//    ZipPagData(false,(u8 *) dolData, (u16 * ) & dolLen);
//    sdkBcdToAsc((u8*)asDolData, (u8*)dolData, dolLen);
//    (*env)->ReleaseStringUTFChars(env, tradeType, trade);
//    return (*env)->NewStringUTF(env, asDolData);
//}

JNIEXPORT jbyteArray JNICALL Java_com_nexgo_libpboc_EmvL2_secondAuthorize(JNIEnv *env, jobject obj,
                                                                      jbyteArray tlv,
                                                                      jint tlvLen)
{
    u8 tlvData[256] = {0};
    u16 tagLen = 0;
    u8 tagData[128] = {0};
    u8 asDolData[512] = {0};
    u8 script[512] = {0};
    u8 IssuerAuthData[64] = {0};
    u8 scriptResult[256] = {0};
    u8 rescode[2] = {0};
    s32 ilen = 0;
    s32 scriptLen =0;
    s32 issuerAuthDataLen = 0;

    //tag 有8a,89，71，72，91
    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);
    memcpy(tlvData, pTlv, tlvLen);
    (*env)->ReleaseByteArrayElements(env, tlv, pTlv, JNI_ABORT);

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv(tlvData, tlvLen, "\x8a", tagData, &tagLen);
    memcpy(rescode, tagData, 2);
    Private_sdkIccImportRespCode(tagLen>0, tagData);
        //xiaox 15/12/16
    Private_sdkIccUnzipPagData(tlvData,tlvLen);

//    tagLen = 0;
//    memset(tagData, 0, sizeof(tagData));
//    AnalyticTlv(tlvData, tlvLen, "\x89", tagData, &tagLen);
//    if (tagLen > 0) {
//        Private_sdkIccConfigPagTLV("\x89", tagData, tagLen);
//    }
//
//    tagLen = 0;
//    memset(tagData, 0, sizeof(tagData));
//    AnalyticTlv(tlvData, tlvLen, "\x71", tagData, &tagLen);
//    if (tagLen > 0) {
//        Private_sdkIccConfigPagTLV("\x71", tagData, tagLen);
//    }
//
//    tagLen = 0;
//    memset(tagData, 0, sizeof(tagData));
//    AnalyticTlv(tlvData, tlvLen, "\x72", tagData, &tagLen);
//    if (tagLen > 0) {
//        Private_sdkIccConfigPagTLV("\x72", tagData, tagLen);
//    }
//

//mai add start 2015-12-23 for get script result
    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv(tlvData, tlvLen, "\x71", tagData, &tagLen);
    Trace("emv", "Script 71 len = %d\r\n", tagLen);
    if (tagLen > 0) {
        script[scriptLen++] = 0x71;
        if (tagLen > 0x79) {
            script[scriptLen++] = 0x81;
        }
        script[scriptLen++] = tagLen;
        memcpy(&script[scriptLen], tagData, tagLen);
        scriptLen += tagLen;
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv(tlvData, tlvLen, "\x72", tagData, &tagLen);
    Trace("emv", "Script 72 len = %d\r\n", tagLen);
    if (tagLen > 0) {
        script[scriptLen++] = 0x72;
        if(tagLen > 0x79)
        {
            script[scriptLen++] = 0x81;
        }
        script[scriptLen++] = tagLen;
        memcpy(script + scriptLen,tagData, tagLen);
        scriptLen += tagLen;
    }

    tagLen = 0;
    issuerAuthDataLen = 0;
    memset(IssuerAuthData, 0, sizeof(IssuerAuthData));
    AnalyticTlv(tlvData, tlvLen, "\x91", IssuerAuthData, &tagLen);
    issuerAuthDataLen = tagLen;
    sdkIccGetOnlineData(1, rescode, IssuerAuthData, issuerAuthDataLen, script, scriptLen);
//mai add end

    Trace("emv","start sdkIccTransFlow2");
    s32 ret = sdkIccTransFlow2(&gIccTradeParam, &gIccParam);
    Trace("emv","sdkIccTransFlow2 return = %d",ret);
    memset(tlvData, 0, sizeof(tlvData));
    memset(asDolData, 0, sizeof(asDolData));


    sdkIccGetScriptResult(&ilen, scriptResult);
    Trace("emv", " Script result len = %d\r\n", ilen);
    if(ilen > 0)
    {
        sdkIccConfigTLV("\xDF\x31", scriptResult, ilen);
        TraceHex("emv", "Script result ", scriptResult, ilen);
    }

    SetFlowResult(ret);
    tagLen = 0;
    ZipPagData(true, (u8 *) tlvData, (u16 * ) & tagLen);

    jbyteArray arrayData = (*env)->NewByteArray(env, tagLen);
    (*env)->SetByteArrayRegion(env, arrayData, 0, tagLen, (jbyte *) tlvData);

    return arrayData;
}
JNIEXPORT jint JNICALL Java_com_nexgo_libpboc_EmvL2_setTerminalAttribute(JNIEnv *env, jobject obj,
                                                                        jbyteArray tlv,
                                                                        jint tlvLen)
{
    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);
    gTerminalAttributeTlvLen = tlvLen;

    memset(gTerminalAttributeTlv, 0, sizeof(gTerminalAttributeTlv));
    memcpy(gTerminalAttributeTlv, (u8 *) pTlv, gTerminalAttributeTlvLen);

    (*env)->ReleaseByteArrayElements(env, tlv,pTlv, JNI_ABORT);
    return (jint) 1;
}
JNIEXPORT jint JNICALL Java_com_nexgo_libpboc_EmvL2_setIccConfigTLV(JNIEnv *env, jobject obj,
                                                                   jbyteArray tag, jbyteArray in,
                                                                   jint inLen)
{
    jbyte *pTag = (*env)->GetByteArrayElements(env, tag, 0);
    jbyte *pInData = (*env)->GetByteArrayElements(env, in, 0);

    s32 ret = sdkIccConfigTLV((u8 *) pTag, (u8 *) pInData, (s32) inLen);

    (*env)->ReleaseByteArrayElements(env, tag, pTag, JNI_ABORT);
    (*env)->ReleaseByteArrayElements(env, in, pInData, JNI_ABORT);
    return (jint) ret;
}

JNIEXPORT jbyteArray JNICALL Java_com_nexgo_libpboc_EmvL2_getIccReadTLV(JNIEnv *env, jobject obj, jbyteArray tag,jboolean isFromCard)
{
    s32 ret;
    s32 dataLen = 128;
    jbyte *pTag = (*env)->GetByteArrayElements(env, tag, 0);
    u8 outData[128] = {0};
	if(isFromCard == JNI_TRUE){
		ret = sdkIccSendGetData(&gIccParam,(u8 *) pTag,(u8 *) outData,(s32 *) &dataLen);
	}else{
    	ret = sdkIccReadTLV((u8 *) pTag, (u8 *) outData, (s32 *) &dataLen);
	}
    TraceHex("emv", "tag", pTag,(((u8)pTag[0]&(u8)0x08) == 0x08)?2:1);
    if(ret == SDK_OK) {
        TraceHex("emv", "tagdata", outData, dataLen);
    }
    (*env)->ReleaseByteArrayElements(env, tag, pTag, JNI_ABORT);
    if (ret > 0) {
        jbyteArray arrayData = (*env)->NewByteArray(env, dataLen);
        jbyte *jby =(*env)->GetByteArrayElements(env,arrayData, 0);
        memcpy(jby,outData,dataLen);
        (*env)->SetByteArrayRegion(env, arrayData, 0, dataLen, (jbyte *) jby);
        (*env)->ReleaseByteArrayElements(env, arrayData, jby, JNI_ABORT);
        return arrayData;
    } else {
        return NULL;
    }
}

JNIEXPORT jint JNICALL Java_com_nexgo_libpboc_EmvL2_setAid(JNIEnv *env, jobject obj,
                                                          jbyteArray tlv,
                                                          jint tlvLen)
{
    u16 tagLen = 0;
    u8 tagData[512] = {0};

    SDK_ICC_EmvSysIni_STRUCT stAidInfo;
    memset(&stAidInfo, 0, sizeof(stAidInfo));

    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x06", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.AidLen = tagLen;
        memcpy(stAidInfo.Aid, tagData, tagLen);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x01", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.Asi = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x09", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.AppVerNum, tagData, 2);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x11", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.TacDefault, tagData, 5);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x12", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.TacOnline, tagData, 5);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x13", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.TacDecline, tagData, 5);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x1b", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.FloorLimit, tagData, 4);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x15", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.Threshold, tagData, 4);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x16", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.MaxTargetPercent = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x17", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.TargetPercent = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x14", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.TermDDOLLen = tagLen;
        memcpy(stAidInfo.TermDDOL, tagData, tagLen);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x18", tagData, &tagLen);
    if (tagLen > 0) {
        stAidInfo.TermPinCap = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x7b", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.vlptranslimit, tagData, 6);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x21", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.termcvm_limit, tagData, 6);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x20", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.clessofflineamt, tagData, 6);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x19", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stAidInfo.clessofflinelimitamt, tagData, 6);
    }
    stAidInfo.bShowRandNum = 0;
    stAidInfo.bLocalName = 0;
    stAidInfo.AppLocalNameLen = 0;
    memset(stAidInfo.AppLocalName, 0, sizeof(stAidInfo.AppLocalName));
    stAidInfo.bForceOnline = 1;

    s32 ret = sdkIccAddAnyAIDList(&stAidInfo, 1);
    (*env)->ReleaseByteArrayElements(env, tlv, pTlv, JNI_ABORT);
    LOGD("sdkIccAddAnyAIDList = %d", ret);
    return (jint) ret;
}

JNIEXPORT jint JNICALL Java_com_nexgo_libpboc_EmvL2_setCapk(JNIEnv *env, jobject obj,
                                                           jbyteArray tlv,
                                                           jint tlvLen)
{
    u16 tagLen = 0;
    u8 tagData[512] = {0};

    SDK_ICC_CAPK_STRUCT stCapkInfo;
    memset(&stCapkInfo, 0, sizeof(stCapkInfo));

    jbyte *pTlv = (*env)->GetByteArrayElements(env, tlv, 0);

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x06", tagData, &tagLen);
    if (tagLen > 0) {
        memcpy(stCapkInfo.RID, tagData, 5);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\x9f\x22", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.CAPKI = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x05", tagData, &tagLen);
    if (tagLen > 0) {
        if (tagLen == 3) {
            stCapkInfo.ExpireDate[0] = 0x20;
            memcpy(&stCapkInfo.ExpireDate[1], tagData, tagLen);
        } else {
            memcpy(stCapkInfo.ExpireDate, tagData, tagLen);
        }
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x06", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.HashInd = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x07", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.ArithInd = tagData[0];
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x02", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.ModulLen = tagLen;
        memcpy(stCapkInfo.Modul, tagData, tagLen);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x04", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.ExponentLen = tagLen;
        memcpy(stCapkInfo.Exponent, tagData, tagLen);
    }

    tagLen = 0;
    memset(tagData, 0, sizeof(tagData));
    AnalyticTlv((u8 *) pTlv, tlvLen, "\xdf\x03", tagData, &tagLen);
    if (tagLen > 0) {
        stCapkInfo.CheckSumLen = tagLen;
        memcpy(stCapkInfo.CheckSum, tagData, tagLen);
    }
    TraceHex("emv","stCapkInfo",&stCapkInfo,sizeof(stCapkInfo));
    s32 ret = sdkIccAddAnyCAPKList(&stCapkInfo, 1);
    (*env)->ReleaseByteArrayElements(env, tlv, pTlv, JNI_ABORT);
    LOGD("sdkIccAddAnyCAPKList = %d", ret);
    return (jint) ret;
}

JNIEXPORT void JNICALL Java_com_nexgo_libpboc_EmvL2_setAppId(JNIEnv *env,jobject obj, jstring appId)
{
    char dolData[256] = {0};
    s32 dolLen = 0;
    char asDolData[512] = {0};
    const jbyte *pAppId = (*env)->GetStringUTFChars(env, appId, NULL);
    sdkSysSetCurAppId((char *)pAppId);
    (*env)->ReleaseStringUTFChars(env, appId, pAppId);
}

JNIEXPORT void JNICALL Java_com_nexgo_libpboc_EmvL2_clearLog(JNIEnv *env,jobject obj)
{
    sdkIccClearEmvWater();
}

JNIEXPORT jobject JNICALL Java_com_nexgo_libpboc_EmvL2_readEcLog(JNIEnv *env,jobject obj,jint isRf)
{
    u8 pLogFormat[100] = {0}, pLogNumOut;
    SDK_ICC_APDU_RESP pLogData[10];

    if(isRf){
        gIccParam.ucCardMode = SDK_ICC_RF;
    }else {
        gIccParam.ucCardMode = SDK_ICC_ICC;
        gIccParam.ucSlotNo = 0;
    }
    memset(pLogData, 0, sizeof(pLogData));
    s32 rslt = sdkIccReadLogDetail(&gIccParam, 10, pLogFormat, pLogData, &pLogNumOut);
    if (rslt != SDK_ReadLogEnd) {
        return NULL;
    }

//    jclass intArrCls = (*env)->FindClass(env, "[B");
//    if ( NULL== intArrCls)
//        return NULL;
//
//    jobjectArray rtu = (*env)->NewObjectArray(env, pLogNumOut, intArrCls, NULL);
//
//    s32 i = 0;
//
//    for(; i < pLogNumOut; i++){
//
//        jbyteArray bArray = (*env)->NewByteArray(env, 100);
//
//        (*env)->SetByteArrayRegion(env, bArray, 0, 100, pLogData[i].DataOut);
//
//        (*env)->SetObjectArrayElement(env, rtu, i, bArray);
//
//        (*env)->DeleteLocalRef(env, bArray);
//
//    }
//    return rtu;

        jclass list_class = (*env)->FindClass(env, "java/util/ArrayList");
        if(list_class==NULL) {
                return NULL;
        }
        jmethodID list_init = (*env)->GetMethodID(env, list_class, "<init>", "()V");
        if(list_init==NULL) {
                return NULL;
        }
        jobject list_object = (*env)->NewObject(env, list_class, list_init,"");
        if(list_object==NULL) {
                return NULL;
        }
        jmethodID arraylist_add = (*env)->GetMethodID(env, list_class, "add", "(Ljava/lang/Object;)Z");
        if(arraylist_add==NULL) {
                return NULL;
        }
        s32 i = 0;
        for(; i < pLogNumOut; i++) {
                jbyteArray bArray = (*env)->NewByteArray(env, 100);
                (*env)->SetByteArrayRegion(env, bArray, 0, 100, pLogData[i].DataOut);
                (*env)->CallObjectMethod(env, list_object, arraylist_add, bArray);
                (*env)->DeleteLocalRef(env, bArray);
        }
        return list_object;
}

JNIEXPORT void JNICALL Java_com_nexgo_libpboc_EmvL2_delAllCapk(JNIEnv *env,jobject obj)
{
    sdkIccDelAllCAPKLists();
}

JNIEXPORT void JNICALL Java_com_nexgo_libpboc_EmvL2_delAllAid(JNIEnv *env,jobject obj)
{
	sdkIccDelAllAIDLists();
}

s32 AnalyticTlv(u8 *pDataIn, u16 datalen, u8 *pTag, u8 *pTagData, u16 *pTagLen) {
    u16 index = 0, len = 0, i = 0, j = 0;
    u8 k = 0;

    while (index < datalen) {
        if (index >= datalen) {
            break;
        }
        k = pDataIn[index];

        if (k != pTag[0]) {
            index++;
            continue;
        }
        if ((k & 0x1f) == 0x1f) {
            if (pDataIn[index + 1] != pTag[1]) {
                index++;
                continue;
            }
            index++;
        }
        index++;

        if (pDataIn[index] <= 127) {      //0x7F
            len = pDataIn[index];
            index++;
        } else {
            len = 0;
            i = pDataIn[index] & 0x7F;

            for (j = 1; j <= i; j++) {
                len = len * 256 + pDataIn[index + j];
            }

            index += i + 1;
        }
        memcpy((u8 *) pTagData, pDataIn + index, len);
        *pTagLen = len;
        return SDK_OK;
    }
    return SDK_ERR;
}


void sdkIccUserTransInitCb() {
    u8 temp[128] = {0};
    u16 len = 0;
    s32 ilen = 0;
    //赋值交易类型
    Trace("emv", "%s", __FUNCTION__);
    sdkIccConfigTLV("\x9C", "\x00", 1);
    sdkIccConfigTLV("\x9F\x09", "\x00\x30", 2);

    TraceHex("emv","TerminalAttr",gTerminalAttributeTlv,gTerminalAttributeTlvLen);
    //1
    //设置下发的终端属性
    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x35", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x35", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x33", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x33", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x40", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x40", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x39", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x39", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x01", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x01", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x15", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x15", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x16", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x16", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x5f\x2a", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x5f\x2a", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x5f\x36", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x5f\x36", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x3c", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x3c", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x3d", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x3d", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x1a", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x1a", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x1e", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x1e", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x1c", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x1c", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\xdf\x44", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\xdf\x44", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\xdf\x45", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\xdf\x45", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\x9f\x66", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x66", temp, len);
    }

    //要特殊处理
    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTerminalAttributeTlv, gTerminalAttributeTlvLen, "\xbf\x73", temp, &len);


    //2
    //设置交易下传的tag
    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x02", temp, &len);
    if (len > 0) {
        memset(gasTradeAmount, 0, sizeof(gasTradeAmount));
        sdkBcdToAsc(gasTradeAmount, temp, len);
    }
    LOGE("set gasTradeAmount = %s", gasTradeAmount);

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x41", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x41", temp, len);
    }


    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9a", temp, &len);
    if (len > 0) {
        if(len == 4){
             sdkIccConfigTLV("\x9a", &temp[1], len-1);
        }else {
             sdkIccConfigTLV("\x9a", temp, len);
        }
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x21", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x21", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x4e", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x4e", temp, len);
    }

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x16", temp, &len);
    if (len > 0) {
        sdkIccConfigTLV("\x9f\x16", temp, len);
    }

        len = 0;
        memset(temp, 0, sizeof(temp));
        AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9f\x1c", temp, &len);
        if (len > 0) {
                sdkIccConfigTLV("\x9f\x1c", temp, len);
        }
//xiaox 15/12/21 K100 do not support offline pin
	#ifndef SPI_DDI
	ilen = 0;
	memset(temp,0,sizeof(temp));
	sdkIccReadTLV("\x9f\x33",temp,&ilen);
	if(ilen > 0){
		temp[1] &= ~(0x01<<7);
		temp[1] &= ~(0x01<<4);
		sdkIccConfigTLV("\x9f\x33", temp, ilen);
	}

	#endif
}

s32 sdkIccInputAmountCb(u8 *amount) {
    Trace("emv", "%s get gasTradeAmount = %s", __FUNCTION__, gasTradeAmount);
    strcpy(amount, gasTradeAmount);
    return SDK_OK;
}

s32 sdkIccDispSelAgainCb() {
    Trace("emv", "%s", __FUNCTION__);
    return 1;
}
s32 getIcCardNo(u8 *cardNo){
	u8 temp[128] = {0};
	s32 len = 0;
	u8 pan[128]={0};
	s32 i = 0,j = 0;
	
        memset(temp, 0, sizeof(temp));
        len = 256;
        if (sdkIccReadTLV("\x5A", temp, &len) > 0) //读Pan信息
        {
                if (len)
                {
                		memset(pan,0,sizeof(pan));
                        sdkBcdToAsc(pan, temp, len);
                        if ((temp[len - 1] & 0x0f) == 0x0f)
                        {
                             memcpy(cardNo,pan,strlen(pan)-1);
                        }else{
                        	memcpy(cardNo,pan,strlen(pan));
                        }
						return SDK_OK;
                        
                }
                
        }
        if (sdkIccReadTLV("\x57", temp, &len) > 0)
        {
                if (len > 0)
                {
                        for (i = len - 1; i > 1; i--)
                        {
                                if (temp[i] != 0xff)
                                {
                                        break;
                                }
                        }

                        j = i + 1;

                        if ((temp[j - 1] & 0x0f) == 0x0f)
                        {
                                i = (j * 2 - 1);
                        }
                        else
                        {
                                i = (j * 2);
                        }

                        if (i > 37)
                        {
                                i = 37;                                                        // 防止数据溢出 zjz20101201
                        }
                        memset(pan,0,sizeof(pan));
                        sdkBcdToAsc(pan, temp, len);
                        for(i = 0;i<strlen(pan);i++){
							if(pan[i] == 'D' || pan[i] == 'd'){
								break;
							}
						}
						memcpy(cardNo,pan,i);
						return SDK_OK;
                }
        }
		return SDK_ERR;
}
s32 sdkIccDispCardOutCb() {
    Trace("emv", "%s", __FUNCTION__);
	s32 ret;
	u8 data[64]={0};
	s32 len = 0;
	u8 temp[64]={0};
	ret = sdkIccReadTLV("\x9f\x27",data,&len);
	if(ret == SDK_OK){
            TraceHex("emv","9f27",data,len);
		if((data[0] & 0x80) == 0x80){
			memset(temp,0,sizeof(temp));
			ret = getIcCardNo(temp);
                TraceHex("emv","cardNo",temp,strlen(temp));
			if(ret == SDK_OK){
				return sdkIccDispCardNoCb(temp);
			}else{
				return SDK_OK;
			}
		}
	}
    return SDK_OK;
}

s32 sdkIccDispCardNoCb(u8 const *pasPAN) {
    Trace("emv", "%s", __FUNCTION__);
        //非挥下电
        #ifdef SPI_DDI
        if(gIccParam.ucCardMode == SDK_ICC_RF) {
                ddi_rf_poweroff();
                ddi_rf_close();
        }
        #endif
    s32 ret = SDK_ERR;
    jstring result = (*(java_interface_emvl2->myEnv))->CallObjectMethod(java_interface_emvl2->myEnv,
                                                                        java_interface_emvl2->gJniObj,
                                                                        java_interface_emvl2->jniConfirmCardNo,
                                                                        (*(java_interface_emvl2->myEnv))->NewStringUTF(
                                                                                (java_interface_emvl2->myEnv),
                                                                                pasPAN));
    const char *No = (*(java_interface_emvl2->myEnv))->GetStringUTFChars(
            (java_interface_emvl2->myEnv), result, NULL);
        if(No == NULL){
                Trace("emv","内存分配失败");
                return SDK_ERR;
        }
    if (!memcmp(No, "-1", 2)) {
        ret = SDK_ERR;
    } else {
        ret = SDK_OK;
    }
    (*(java_interface_emvl2->myEnv))->ReleaseStringUTFChars((java_interface_emvl2->myEnv), result,
                                                            No);
    return ret;
}

s32 sdkInputCreditPwd(const u8 *pasTradeAmount, u8 PINTryCount, u8 IccEncryptWay, u8 *pCreditPwd) {
    Trace("emv", "%s", __FUNCTION__);
    s32 ret = SDK_ERR;
    jstring result = (*(java_interface_emvl2->myEnv))->CallObjectMethod(java_interface_emvl2->myEnv,
                                                                        java_interface_emvl2->gJniObj,
                                                                        java_interface_emvl2->jniInputPwd,
                                                                        (*(java_interface_emvl2->myEnv))->NewStringUTF(
                                                                                (java_interface_emvl2->myEnv),
                                                                                pasTradeAmount),
                                                                        (jint) PINTryCount,
                                                                        (jint) IccEncryptWay);
    const char *pwd = (*(java_interface_emvl2->myEnv))->GetStringUTFChars(
            (java_interface_emvl2->myEnv), result, NULL);
        if(pwd == NULL){
                Trace("emv","内存分配失败");
                return SDK_ERR;
        }
    if (!memcmp(pwd, "-1", 2)) {
        ret = SDK_ERR;
    } else {
        //xiaox 15/12/21 失败返回"-1",成功 bypass 返回"0",非则返回"1",底层根据pCreditPwd[0]==0来判断是否为 bypass,非给赋个非0值就可以了
        if(*pwd == '1'){
            pCreditPwd[0] = 1;
            //zhouxiaoxin return '*' for emvlib,tell it pos has an inside pinpad,if it request offline encryption pin
            pCreditPwd[1] = '*';
        }
        ret = SDK_OK;
    }
    (*(java_interface_emvl2->myEnv))->ReleaseStringUTFChars((java_interface_emvl2->myEnv), result,
                                                            pwd);
    return ret;
}

s32 sdkIccDispOnlineRandNumCb(u8 randNum) {
    Trace("emv", "%s", __FUNCTION__);
    return SDK_OK;
}


s32 sdkIccChooseEcCb() {
    Trace("emv", "%s", __FUNCTION__);
    return SDK_ESC;
}

s32 sdkIccDispCandAppListCb(u8 ucCandAppNum, u8 **pheCandApp) {
    SDK_ICC_AIDLIST *pstAidlist = NULL;
    s32 key = 0;
    u8 dispbuf[256] = {0};
    u8 temp[64] = {0};
    u8 len = 0;
    u8 offset = 0;
    u8 i = 0, j = 0, selectindex = 0;

    Trace("emv", "%s", __FUNCTION__);


    for (i = 0, j = 0; i < ucCandAppNum; i++) {
        pstAidlist = (SDK_ICC_AIDLIST * )(*(pheCandApp + i));
        memset(temp, 0, sizeof(temp));

        if (pstAidlist->ucAidLen != 0) {
            sdkAscToBcd(temp, pstAidlist->asAid, pstAidlist->ucAidLen);

            if (!memcmp(temp, "\xA0\x00\x00\x03\x33", 5)) {
                j++;
                selectindex = i + 1;
            }
        }
    }

    if (j == 1) {
        return selectindex;
    }

    dispbuf[0] = gisFirstSelApp ? 1 : 0;
    sdkU32ToBcd(&dispbuf[1], ucCandAppNum * 16, 2);
    offset = 3;
    for (i = 0; i < ucCandAppNum; i++) {
        pstAidlist = (SDK_ICC_AIDLIST * )(*(pheCandApp + i));

        if (pstAidlist->ucLocalName == 1)     //Merchant perferred name.
        {
            len = pstAidlist->ucAppLocalNameLen;

            if (len > sizeof(pstAidlist->asAppLocalName)) {
                len = sizeof(pstAidlist->asAppLocalName);
            }
            memcpy(&dispbuf[offset], pstAidlist->asAppLocalName, len);
            offset += 16;
        }
        else if (pstAidlist->ucPreferNameLen != 0 &&
                 pstAidlist->ucIcti == 0x01)     //Preferred name in card //huxiang 20090702
        {
            len = pstAidlist->ucPreferNameLen;

            if (len > sizeof(pstAidlist->asPreferName)) {
                len = sizeof(pstAidlist->asPreferName);
            }
            memcpy(&dispbuf[offset], pstAidlist->asPreferName, len);
            offset += 16;
        }
        else if (pstAidlist->ucAppLabelLen != 0)     //Label in card
        {
            len = pstAidlist->ucAppLabelLen;

            if (len > sizeof(pstAidlist->asAppLabel)) {
                len = sizeof(pstAidlist->asAppLabel);
            }
            memcpy(&dispbuf[offset], pstAidlist->asAppLabel, len);
            offset += 16;
        }
        else        //others display AID
        {
            len = pstAidlist->ucAidLen;
            memcpy(&dispbuf[offset], pstAidlist->asAid, len);
            offset += 16;
        }
    }

    jbyteArray appData = (*(java_interface_emvl2->myEnv))->NewByteArray(
            (java_interface_emvl2->myEnv), offset);
    (*(java_interface_emvl2->myEnv))->SetByteArrayRegion((java_interface_emvl2->myEnv), appData, 0,
                                                         offset, (jbyte *) dispbuf);
    jstring result = (*(java_interface_emvl2->myEnv))->CallObjectMethod(java_interface_emvl2->myEnv,
                                                                        java_interface_emvl2->gJniObj,
                                                                        java_interface_emvl2->jniSelApp,
                                                                        (appData)
    );
    (*(java_interface_emvl2->myEnv))->DeleteLocalRef((java_interface_emvl2->myEnv), appData);
    const char *index = (*(java_interface_emvl2->myEnv))->GetStringUTFChars(
            (java_interface_emvl2->myEnv), result, NULL);
    s32 ret = 0;
    if (!memcmp(index, "-1", 2)) {
        ret = SDK_ERR;
    } else {
        ret = atoi(index);
    }
    (*(java_interface_emvl2->myEnv))->ReleaseStringUTFChars((java_interface_emvl2->myEnv), result,
                                                            index);
    gisFirstSelApp = false;
    return ret;
}

s32 sdkIccDispPromptDataCb(SDK_ICC_PROMPT_TYPE ePromptType) {
    Trace("emv", "%s", __FUNCTION__);
    return true;
}

s32 sdkIccVerifyIDCardCb(void) {
    Trace("emv", "%s", __FUNCTION__);
    const char IdTypeTab[][16] = {
            "身份证", "军官证", "护照", "入境证", "临时身份证", "其它"
    };
    s32 len = 0;
    u8 tagData[64] = {0};
    u8 idType[20] = {0};
    u8 idNo[40] = {0};
    //9f62
    len = 0;
    memset(tagData, 0, sizeof(tagData));
    sdkIccReadTLV("\x9f\x62", tagData, &len);
    sprintf(idType, "%.16s", IdTypeTab[tagData[0]]);
    //9f61
    len = 0;
    memset(tagData, 0, sizeof(tagData));
    sdkIccReadTLV("\x9f\x61", tagData, &len);
    memcpy(idNo, tagData, len);
    s32 ret = 0;
    jstring result = (*(java_interface_emvl2->myEnv))->CallObjectMethod(java_interface_emvl2->myEnv,
                                                                        java_interface_emvl2->gJniObj,
                                                                        java_interface_emvl2->jniCerVerify,
                                                                        (*(java_interface_emvl2->myEnv))->NewStringUTF(
                                                                                (java_interface_emvl2->myEnv),
                                                                                idType),
                                                                        (*(java_interface_emvl2->myEnv))->NewStringUTF(
                                                                                (java_interface_emvl2->myEnv),
                                                                                idNo));
    const char *tmp = (*(java_interface_emvl2->myEnv))->GetStringUTFChars(
            (java_interface_emvl2->myEnv), result, NULL);
    if (!memcmp(tmp, "-1", 2)) {
        ret = SDK_ERR;
    } else {
        ret = SDK_OK;
    }
    (*(java_interface_emvl2->myEnv))->ReleaseStringUTFChars((java_interface_emvl2->myEnv), result,
                                                            tmp);
    return ret;
}
void InitStdTransFlow(void)
{
    u8 temp[16] = {0};
    s32 len = 4;

        sdkIccUserTransInitCb();

    memset(temp, 0, sizeof(temp));
    len = 4;

    if(sdkIccReadTLV("\x9F\x66", temp, &len) > 0)		 //aid字节长度
    {
        temp[1] |= 0x80;
        len = sdkIccConfigTLV("\x9F\x66", temp, 4);
    }

}

void EmvCaseOption(SDK_ICC_TRADE_PARAM *pstIccTradeParam,bool isRf) {

    u8 ucCustomTradeType = 0;
    u8 temp[128] = {0};
    u16 len = 0;
    bool forceOnLine = false;
    bool isEasyFlow = false;

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\xdf\x72", temp, &len);
    forceOnLine = (temp[0] == 0x01) ? true : false;
    Trace("emv","forceOnLine = %02x",forceOnLine);

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\xdf\x71", temp, &len);
    isEasyFlow = (temp[0] == 0x01) ? true : false;
    Trace("emv","isEasyFlow = %02x",isEasyFlow);

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\xdf\x7c", temp, (u16 * ) & len);
    ucCustomTradeType = temp[0];
    Trace("emv","ucCustomTradeType = %02x",ucCustomTradeType);

    len = 0;
    memset(temp, 0, sizeof(temp));
    AnalyticTlv(gTradeTlv, gTradeTlvLen, "\x9c", temp, (u16 * ) & len);
    if(len != 0){
        sdkIccConfigTLV("\x9C", temp, len);
        Trace("emv","TradeType = %02x",temp[0]);
    }

    pstIccTradeParam->pasTradeAmount = gasTradeAmount;
    pstIccTradeParam->bIsForceOnline = forceOnLine;
    pstIccTradeParam->bIsFallback = false;
    pstIccTradeParam->bIsSupportQPBOC = false;
    pstIccTradeParam->bIsSupportCLPBOC = false;
    pstIccTradeParam->bIsSupportEc = false;
    pstIccTradeParam->bIsPromptChooseEc = false;
    pstIccTradeParam->bIsDefaultEc = false;
    pstIccTradeParam->eFlowMode = SDK_ICC_EASY_FLOW;//默认简化流程
    memset(&pstIccTradeParam->stFixedAid, 0, sizeof(SDK_ICC_FIXED_AID));
    //回调函数
    pstIccTradeParam->InputAmount = sdkIccInputAmountCb;
    pstIccTradeParam->DispReselectApp = NULL;
    pstIccTradeParam->DispCardOut = NULL;
    pstIccTradeParam->VerifyCardNo = sdkIccDispCardNoCb;
    pstIccTradeParam->InputPWD = sdkInputCreditPwd;
    pstIccTradeParam->DispOnlineRandNum = NULL;
    pstIccTradeParam->DispChooseEC = sdkIccChooseEcCb;
    pstIccTradeParam->DispPromptData = sdkIccDispPromptDataCb;
    pstIccTradeParam->DispCandAppList = sdkIccDispCandAppListCb;
    pstIccTradeParam->UserIccTransInit = sdkIccUserTransInitCb;
    pstIccTradeParam->VerifyIDCard = sdkIccVerifyIDCardCb;

    //mai 非接触强制联机
    if(isRf && (pstIccTradeParam->bIsForceOnline || ucCustomTradeType == 0x01)){
        Trace("emv","QPBOC Enter force online!");
        pstIccTradeParam->bIsSupportEc = false;
        pstIccTradeParam->bIsPromptChooseEc = false;
        pstIccTradeParam->bIsDefaultEc = false;
        pstIccTradeParam->UserIccTransInit = InitStdTransFlow;
        pstIccTradeParam->bIsForceOnline = false;		//9F66已经置位了
        ucCustomTradeType = 0x01;
    }

    if (!isEasyFlow) {
        switch (ucCustomTradeType) {
            case 0x01:         //接触式为EMV
                pstIccTradeParam->bIsSupportQPBOC = false;
                pstIccTradeParam->bIsSupportCLPBOC = false;
                if(isRf){
                    pstIccTradeParam->bIsSupportQPBOC = true;
                    pstIccTradeParam->DispCardOut = sdkIccDispCardOutCb;
                }
                pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
                break;

            case 0x0B:         //电子现金消费
                if(isRf) {
                    pstIccTradeParam->bIsSupportQPBOC = true;
                    pstIccTradeParam->VerifyCardNo = NULL;
                    pstIccTradeParam->VerifyIDCard = NULL;
                    pstIccTradeParam->InputPWD = NULL;
                    pstIccTradeParam->DispPromptData = NULL;
                }
                pstIccTradeParam->bIsSupportEc = true;
                pstIccTradeParam->bIsDefaultEc = true;
                pstIccTradeParam->eFlowMode = SDK_ICC_ALL_FLOW;
                break;

            case 0x21:         //电子现金指定账户圈存
            case 0x22://电子现金非指定账户圈存
            case 0x23://电子现金现金圈存
            case 0x24://电子现金圈提
                pstIccTradeParam->bIsForceOnline = true;
                pstIccTradeParam->bIsSupportCLPBOC = true;
                pstIccTradeParam->bIsSupportEc = true;
                pstIccTradeParam->bIsDefaultEc = true;
                pstIccTradeParam->eFlowMode = SDK_ICC_EC_LOAD;
                break;

            case 0x0E:         //读明细记录
                pstIccTradeParam->bIsSupportEc = true;
                pstIccTradeParam->bIsDefaultEc = true;
                pstIccTradeParam->eFlowMode = SDK_ICC_READLOG;
                pstIccTradeParam->VerifyCardNo = NULL;
                break;

            case 0x25:         //电子现金查询余额
                pstIccTradeParam->bIsSupportCLPBOC = true;
                pstIccTradeParam->eFlowMode = SDK_ICC_EC_QUERY;
                pstIccTradeParam->bIsSupportEc = true;
                pstIccTradeParam->bIsPromptChooseEc = false;
                pstIccTradeParam->bIsDefaultEc = true;
                break;
            case 0x26://电子现金现金圈存撤销
                pstIccTradeParam->bIsForceOnline = true;
                pstIccTradeParam->bIsFallback = false;
                pstIccTradeParam->bIsSupportEc = true;
                pstIccTradeParam->bIsSupportCLPBOC = true;
                pstIccTradeParam->bIsDefaultEc = true;
                pstIccTradeParam->eFlowMode = SDK_ICC_EC_LOAD;
                break;
            default:
                break;
        }
    }
}

s32 IccReadCard(u8 ucCardModeEn, SDK_ICC_TRADE_PARAM *pstgIccTradeParam,
                SDK_ICC_PARAM *pstgIccParam,
                SDK_ICC_CARDDATA *pstCardData) {
    u8 heUID[64] = {0};
    u32 timerID;
    s32 rslt;

    if ((pstgIccTradeParam == NULL || pstgIccParam == NULL ||
         sdkIccTransInit(pstgIccTradeParam) != SDK_OK) &&
        (ucCardModeEn & SDK_ICC_RF || ucCardModeEn & SDK_ICC_ICC)) {
        return SDK_ESC;
    }
    if (ucCardModeEn & SDK_ICC_MAG) {
        sdkIccOpenMag();
    }

    if (ucCardModeEn & SDK_ICC_ICC) {
        rslt = sdkIccOpenIcDev();
        Trace("emv", "sdkIccOpenIcDev rslt = %d", rslt);
    }

    if (ucCardModeEn & SDK_ICC_RF) {
        rslt = sdkIccOpenRfDev();
        Trace("emv", "sdkIccOpenRfDev rslt = %d", rslt);
    }

    timerID = sdkTimerGetId();

    while (1) {
//        if (ucCardModeEn & SDK_ICC_MAG) {
//            rslt = sdkIccGetMagData(pstgIccParam, pstCardData);
//            if (rslt != SDK_ICC_NOCARD) {
//                return rslt;
//            }
//        }

        if (ucCardModeEn & SDK_ICC_ICC) {
            rslt = sdkIccGetCardStatus(pstgIccParam, 100);
            if (rslt == SDK_OK || rslt != SDK_ICC_NOCARD) {
                return rslt;
            }
        }

        if (ucCardModeEn & SDK_ICC_RF) //非接读卡
        {
            rslt = sdkIccPowerOnAndSeek(pstgIccTradeParam, pstgIccParam, heUID);
            LOGD("sdkIccPowerOnAndSeek rslt = %d", rslt);
            if (rslt == SDK_OK || (rslt != SDK_ICC_NOCARD && rslt != SDK_ICC_MORECARD)) {
                return rslt;
            }
        }

        if (1 == sdkTimerIsEnd(timerID, 15000)) {
            return SDK_TIME_OUT;
        }
    }
}

//zhouxiaoxin add
static const struct _DoLTab const stSaleTab[] = {
        {"\x9f\x06"},
        {"\x9f\x26"},           //应用密文M
        {"\x9f\x27"},           //应用数据信息M
        {"\x9f\x10"},          //发卡行应用数据M
        {"\x9f\x37"},           //不可预知数据M
        {"\x9f\x36"},           //应用交易计数器M
        {"\x95"},                       //终端验证结果M
        {"\x9a"},                       //交易日期M
        {"\x9c"},                       //交易类型M
        {"\x9f\x02"},           //交易金额M
        {"\x5f\x2a"},           //交易货币代码M
        {"\x82"},              //应用交互特征M
        {"\x9f\x1a"},           //终端国家代码M
        {"\x9f\x03"},           //其他金额M
        {"\x9f\x33"},           //终端性能M
        {"\x9f\x34"},           //持卡人验证结果O
        {"\x9f\x35"},           //终端类型O
        {"\x9f\x1e"},           //接口设备序列号C
        {"\x84"},                       //专用文件名称O
        {"\x9f\x09"},           //应用版本号O
        {"\x9f\x41"},           //交易序列计数器O
        {"\x9f\x63"},           //卡产品标识信息O
        {"\x5f\x34"},
        {"\x5a"},
        {"\x57"},
        {"\x5f\x24"},
        {"\xdf\x36"},
        {"\xdf\x75"},
        {"\xdf\x77"},
};
static const struct _DoLTab const stOnLineDealTab[] = {

        {"\x9f\x26"},           //应用密文M
        {"\x9f\x37"},           //不可预知数据M
        {"\x9f\x36"},           //应用交易计数器M
        {"\x95"},                       //终端验证结果M
        {"\x9a"},                       //交易日期M
        {"\x9c"},
        {"\xdf\x31"},
        {"\xdf\x75"},
};

void ZipPagData(bool bisOnLineDeal, u8 *DOLData, u16 *DataLen) {
    u16 dollen = 0;
    u8 dol[256] = {0};
    u8 buf[255] = {0};
    u32 buflen = 0;
    u8 k = 0;
    s32 tabSize = 0;
    s32 rslt;
    struct _DoLTab stdoltab;

    if (bisOnLineDeal) {
        tabSize = sizeof(stOnLineDealTab) / sizeof(struct _DoLTab);
    } else {
        tabSize = sizeof(stSaleTab) / sizeof(struct _DoLTab);
    }

    for (k = 0; k < tabSize; k++) {
        if (bisOnLineDeal) {
            memcpy(&stdoltab, &stOnLineDealTab[k], sizeof(struct _DoLTab));
        } else {
            memcpy(&stdoltab, &stSaleTab[k], sizeof(struct _DoLTab));
        }
        memset(buf, 0, sizeof(buf));
        buflen = sizeof(buf);
        rslt = sdkIccReadTLV(stdoltab.pheTag, buf, &buflen);
        if ((stdoltab.pheTag[0] & 0x1F) == 0x1F) {
            memcpy(&dol[dollen], stdoltab.pheTag, 2);
            dollen += 2;
        } else {
            dol[dollen++] = stdoltab.pheTag[0];
        }
        if ((rslt == SDK_OK) && (buflen > 0)) {                //有标签
            dol[dollen++] = buflen;
            memcpy(&dol[dollen], buf, buflen);
            dollen += buflen;
        } else {
            dol[dollen++] = 0;
        }
    }
    *DataLen = dollen;
    memcpy(DOLData, dol, dollen);
}

s32 SdkIccEmv(bool isRf) {
    s32 ret;
    SDK_ICC_CARDDATA CardData;
    s32 len = 0;
    s32 num = 0;
    u8 temp[128] = {0};

    memset(&gIccTradeParam, 0, sizeof(gIccTradeParam));
    memset(&gIccParam, 0, sizeof(gIccParam));

    sdkIccPowerStartInit();
#if 1
    num = 0;
    sdkIccGetAIDListNum(&num);
    if (num == 0) {
        Trace("emv", "load default aid");
        ret = sdkIccAddAnyAIDList(defaultAID, sizeof(defaultAID) / sizeof(defaultAID[0]));
        if (ret != SDK_OK) {
            return SDK_ERR;
        }
    }
    num = 0;
    sdkIccGetCAPKListNum(&num);
    if (num == 0) {
        Trace("emv", "load default capk");
        ret = sdkIccAddAnyCAPKList(defaultCAPK, sizeof(defaultCAPK) / sizeof(defaultCAPK[0]));
        if (ret != SDK_OK) {
            return SDK_ERR;
        }
    }
#endif
    EmvCaseOption(&gIccTradeParam,isRf);
#if 0
    ret = IccReadCard(SDK_ICC_ICC, &gIccTradeParam, &gIccParam, &CardData);
    Trace("emv", "IccReadCard ret = %d",ret);
    if (ret != SDK_OK) {
        return SDK_ERR;
    }
#else
    if(isRf){
        gIccParam.ucCardMode = SDK_ICC_RF;
    }else {
        gIccParam.ucCardMode = SDK_ICC_ICC;
    }
#endif
    gisFirstSelApp = true;
    sdkIccTransInit(&gIccTradeParam);
    Trace("emv","start sdkIccTransFlow1");
    ret = sdkIccTransFlow1(&gIccTradeParam, &gIccParam);
    Trace("emv", "sdkIccTransFlow1 return = %d", ret);
    return ret;
}


typedef enum{
    TYPE_DES_SINGLE,
    TYPE_DES_DOUBLE,
    TYPE_DES_TRIPLE,
}eDesType;

typedef struct DES_ALG_STRUCT{
    u8 *pucData;
    u32 *puiDataLen;
    u8 *pucCipher;
    u32 *puiCipherLen;
    u8 *pucKey;
    eDesType desType;
    bool bIsEncrypt;
}stDesAlg;


/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:PKCS #5 填充
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
s32 pkcs5Padding(u8 *ucInData, u32 uiInDataLen,u8 *ucOutData)
{
    u8 ucPadByte = 0x00;
    u32 uiTmpLen = 0;

    if(ucInData == NULL || ucOutData == NULL)
        return uiTmpLen;

    uiTmpLen = (uiInDataLen /8)*8 + 8;
    ucPadByte = uiTmpLen - uiInDataLen;

    memcpy(ucOutData, ucInData, uiInDataLen);
    memset(ucOutData + uiInDataLen, ucPadByte, ucPadByte);

    return uiTmpLen;
}

/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:PKCS #5 去掉填充
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
s32 pkcs5Unpadding(u8 *ucInData, u32 uiInDataLen,u8 *ucOutData)
{
    u32 uiTmpLen = 0;

    if(ucInData == NULL || ucOutData == NULL)
        return uiTmpLen;

    uiTmpLen = uiInDataLen - ucInData[uiInDataLen-1];
    memcpy(ucOutData, ucInData, uiTmpLen);
    return uiTmpLen;
}

/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:Des 或3Des 加密ECB模式
 输入参数:
 输出参数:
 返 回 值
 备    注:
 修改信息:
********************************************************************/
static s32 desEcb(stDesAlg *pstDesData)
{
    u32 uiTmpLen = 0;
    u8 tmpBuf[8] = {0};
    s32 i = 0, iRet;

    //Trace("Mai","Enter desEcb \r\n");

    //Trace("Mai","DesEcb = %d,%d,%d,%d,%d \r\n", pstDesData, pstDesData->pucCipher, pstDesData->pucData,pstDesData->puiCipherLen, pstDesData->puiDataLen);
    if((pstDesData == NULL) || (pstDesData->pucCipher == NULL) || (pstDesData->pucData == NULL)
       || (pstDesData->puiCipherLen == NULL) || (pstDesData->puiDataLen == NULL))
        return SDK_PARA_ERR;

    //Trace("Mai","Enter desEcb 111111111\r\n");
    if((pstDesData->bIsEncrypt) && ((*pstDesData->puiDataLen)%8 != 0))
        return SDK_PARA_ERR;

    //Trace("Mai","Enter desEcb 222222222\r\n");
    if((!pstDesData->bIsEncrypt) && ((*pstDesData->puiCipherLen)%8 != 0))
        return SDK_PARA_ERR;


    //sdkmSleep(1000);
    if(pstDesData->bIsEncrypt)
    {
        uiTmpLen = *pstDesData->puiDataLen;
        if(pstDesData->desType == TYPE_DES_SINGLE)
        {
            //TraceHex("Mai", "Data", tmpBuf, 8);
            //sdkmSleep(1000);
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucData+i, 8);
                //TraceHex("Mai", "Data", tmpBuf, 8);
                //sdkmSleep(1000);
                iRet = sdkDesS(true, tmpBuf, pstDesData->pucKey);
                //Trace("Mai","iRet = %d\r\n", iRet);
                //sdkmSleep(1000);
                memcpy(pstDesData->pucCipher+i, tmpBuf, 8);
                //TraceHex("Mai", "Data", pstDesData->pucCipher, 8);
                //sdkmSleep(1000);
            }

        }
        else if(pstDesData->desType == TYPE_DES_DOUBLE)
        {
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucData+i, 8);
                sdkDes3S(true, tmpBuf, pstDesData->pucKey);
                memcpy(pstDesData->pucCipher+i, tmpBuf, 8);
            }
        }
        else if(pstDesData->desType == TYPE_DES_TRIPLE)
        {
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucData+i, 8);
                sdkDesS(true, tmpBuf, pstDesData->pucKey);
                sdkDesS(false, tmpBuf, pstDesData->pucKey+8);
                sdkDesS(true, tmpBuf, pstDesData->pucKey+16);
                memcpy(pstDesData->pucCipher+i, tmpBuf, 8);
            }
        }
        else
            return SDK_PARA_ERR;

        *pstDesData->puiCipherLen = uiTmpLen;
        TraceHex("Mai", "pucCipher", pstDesData->pucCipher, uiTmpLen);
    }
    else
    {
        uiTmpLen = *pstDesData->puiCipherLen;
        if(pstDesData->desType == TYPE_DES_SINGLE)
        {
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucCipher+i, 8);
                sdkDesS(false, tmpBuf, pstDesData->pucKey);
                memcpy(pstDesData->pucData+i, tmpBuf, 8);
            }

        }
        else if(pstDesData->desType == TYPE_DES_DOUBLE)
        {
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucCipher+i, 8);
                sdkDes3S(false, tmpBuf, pstDesData->pucKey);
                memcpy(pstDesData->pucData+i, tmpBuf, 8);
            }
        }
        else if(pstDesData->desType == TYPE_DES_TRIPLE)
        {
            for(i = 0; i <= uiTmpLen; i += 8)
            {
                memcpy(tmpBuf, pstDesData->pucCipher+i, 8);
                sdkDesS(false, tmpBuf, pstDesData->pucKey);
                sdkDesS(true, tmpBuf, pstDesData->pucKey+8);
                sdkDesS(false, tmpBuf, pstDesData->pucKey+16);
                memcpy(pstDesData->pucData+i, tmpBuf, 8);
            }
        }
        else
            return SDK_PARA_ERR;

        *pstDesData->puiDataLen = uiTmpLen;
        TraceHex("Mai", "pucData", pstDesData->pucData, uiTmpLen);
    }

    Trace("Mai","Enter desEcb 333333333\r\n");
    return SDK_OK;
}

/********************************************************************
 作    者: 麦浩卿
 版    权: 深圳市新国都技术股份有限公司
 函数功能:PKCS #5 填充加解密3DES单倍长
 输入参数:
 输出参数:
 返 回 值:	返回加解密后的长度
 备    注: 		密钥必须是16字节
 修改信息:
********************************************************************/
s32 DesPkcs5Padding(stDesAlg *pstDesData)
{
    s32 iRet = 0;
    u32 uiLen = 0;
    u8* pTmp = NULL;
    u8* pTmpBuf = NULL;

    //Trace("Mai","Enter DesPkcs5Padding \r\n");
    if(pstDesData == NULL)
        return SDK_PARA_ERR;

    //Trace("Mai","DesPkcs5Padding 111111 \r\n");
    Trace("Mai", "adr = %d \r\n", pstDesData->puiDataLen);
    if(pstDesData->bIsEncrypt)
    {
        //Trace("Mai","DesPkcs5Padding 111222 \r\n");
        uiLen = *pstDesData->puiDataLen;
        pTmpBuf = (u8*)sdkGetMem(uiLen+8);
        memset(pTmpBuf, 0x00, uiLen+8);
        uiLen = pkcs5Padding(pstDesData->pucData, uiLen, pTmpBuf);
        pTmp = pstDesData->pucData;
        pstDesData->pucData = pTmpBuf;
        *pstDesData->puiDataLen = uiLen;
    }

    iRet = desEcb(pstDesData);
    //Trace("Mai","DesPkcs5Padding 222222 iRet = %d\r\n", iRet);
    if(pstDesData->bIsEncrypt)
    {
        sdkFreeMem(pTmpBuf);
        pstDesData->pucData = pTmp;
        TraceHex("Mai", "pucCipher", pstDesData->pucCipher, *pstDesData->puiCipherLen);
    }
    else
    {
        uiLen = pstDesData->pucData[*pstDesData->puiDataLen - 1];
        pTmp = pstDesData->pucData + *pstDesData->puiDataLen - uiLen;
        memset(pTmp, 0x00, uiLen);
        TraceHex("Mai", "pucData", pstDesData->pucData, *pstDesData->puiDataLen);
        //uiLen = pkcs5Unpadding(pstDesData->pucData, uiLen, pTmpBuf);
        *pstDesData->puiDataLen -= uiLen;

        TraceHex("Mai", "pucData", pstDesData->pucData, *pstDesData->puiDataLen);
    }

    Trace("Mai", "adr = %d \r\n", pstDesData->puiDataLen);
    Trace("Mai","Out DesPkcs5Padding\r\n");
    return iRet;
}

s32 GenDesKey(u8 *vetor, u8 ucAlgNo, u8 *ucKey)
{
    u8 tmp[8] = {0};

    if(vetor == NULL || ucKey == NULL)
        return SDK_PARA_ERR;

    memset(tmp, ucAlgNo, sizeof(tmp));
    switch(ucAlgNo)
    {
        case 0x01:
            sdkXOR8(ucKey, tmp, vetor);
            break;

        case 0x02:
            sdkXOR8(ucKey, tmp, vetor);
            memset(tmp, 0xFF, sizeof(tmp));
            sdkXOR8(ucKey+8, tmp, vetor);
            break;

        case 0x03:
            sdkXOR8(ucKey, tmp, vetor);
            memset(tmp, 0xFF, sizeof(tmp));
            sdkXOR8(ucKey+8, tmp, vetor);
            memset(tmp, 0x77, sizeof(tmp));
            sdkXOR8(ucKey+16, tmp, vetor);
            break;

        default:
            return SDK_PARA_ERR;
    }

    return SDK_OK;

}

void FreeDesAlg(stDesAlg *pstDesData)
{
    sdkFreeMem(pstDesData->pucCipher);
    sdkFreeMem(pstDesData->pucData);
    sdkFreeMem(pstDesData->pucKey);
    sdkFreeMem(pstDesData);
}

s32 DealPlainTrackData(u8 *pTrackData, u32 usTrackLen, u8 *pOutData, u16 *pusOutLen,
                       u8 ucAlgNo, u8 *ucRandom, bool bIsEncrypt)
{
    s32 iRet = 0;
    u32 len = 0;
    u8 *pTmp = NULL;
    stDesAlg *pstDesData = NULL;


    Trace("Mai","Enter DealPlainTrackData \r\n");
    if((pTrackData == NULL) || (pOutData == NULL) || (pusOutLen == NULL) || (ucRandom == NULL))
        return SDK_PARA_ERR;

    pstDesData = (stDesAlg*)sdkGetMem(sizeof(stDesAlg));
    memset(pstDesData, 0x00, sizeof(stDesAlg));

    pstDesData->bIsEncrypt = bIsEncrypt;
    if(bIsEncrypt)
    {
        pstDesData->pucData = pTrackData;
        pstDesData->puiDataLen = &usTrackLen;
        pTmp = (u8*)sdkGetMem(usTrackLen+8);
        pstDesData->pucCipher = pTmp;
        pstDesData->puiCipherLen = &len;
    }
    else
    {
        pstDesData->pucCipher = pTrackData;
        pstDesData->puiCipherLen = &usTrackLen;
        pTmp = (u8*)sdkGetMem(usTrackLen);
        pstDesData->pucData = pTmp;
        pstDesData->puiDataLen = &len;
    }

    Trace("Mai","DealPlainTrackData 1111111 \r\n");
    pstDesData->pucKey = (u8*)sdkGetMem(24);
    iRet = GenDesKey(ucRandom, ucAlgNo, pstDesData->pucKey);
    if(iRet != SDK_OK)
    {
        FreeDesAlg(pstDesData);
        return iRet;
    }

    //Trace("Mai", "adr = %d \r\n", pstDesData->puiDataLen);
    //TraceHex("Mai","Key",pstDesData->pucKey, 24);
    Trace("Mai","DealPlainTrackData 222222 \r\n");
    switch(ucAlgNo)
    {
        case 0x01:

            pstDesData->desType = TYPE_DES_SINGLE;
            iRet = DesPkcs5Padding(pstDesData);
            break;

        case 0x02:
            pstDesData->desType = TYPE_DES_DOUBLE;
            iRet = DesPkcs5Padding(pstDesData);
            break;

        case 0x03:
            pstDesData->desType = TYPE_DES_DOUBLE;
            iRet = DesPkcs5Padding(pstDesData);
            break;

        default:
            iRet = SDK_PARA_ERR;
            break;
    }

    //Trace("Mai", "adr = %d \r\n", pstDesData->puiDataLen);
    *pusOutLen = len;
    memcpy(pOutData, pTmp, len);
    FreeDesAlg(pstDesData);
    Trace("Mai","Out DealPlainTrackData\r\n");
    return iRet;

}

void TestPlainTrackApi()
{
    u8 tmp[64] = {0};
    u8 tmp1[64] = {0};
    u8 random[8] = {0xe3,0x9c,0xf1,0xa2,0xcc,0x0b,0xc7,0xeb};
    u8 *trackData1 = "6259570000091113D14042060000076600001";
    u16 usOutLen = 0;
    u16 usOutLen1 = 0;
    s32 iRet = 0;

    Trace("Mai"," 算法 加密<11111111111111111111111111111111111111111111111111111111111> \r\n");
    Trace("Mai","trackData1 = %s\r\n",trackData1);
    iRet = DealPlainTrackData(trackData1, strlen(trackData1), tmp, &usOutLen, 0x01, random, true);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","加密后",tmp, usOutLen);

    Trace("Mai"," 算法 解密<11111111111111111111111111111111111111111111111111111111111> \r\n");
    memset(tmp1, 0x00, sizeof(tmp1));
    iRet = DealPlainTrackData(tmp, usOutLen, tmp1, &usOutLen1, 0x01, random, false);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","解密后",tmp1, usOutLen1);

    Trace("Mai"," 算法 加密<2222222222222222222222222222222222222222222222222222222222> \r\n");
    Trace("Mai","trackData1 = %s\r\n",trackData1);
    iRet = DealPlainTrackData(trackData1, strlen(trackData1), tmp, &usOutLen, 0x02, random, true);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","加密后",tmp, usOutLen);

    Trace("Mai"," 算法 解密<2222222222222222222222222222222222222222222222222222222222> \r\n");
    memset(tmp1, 0x00, sizeof(tmp1));
    iRet = DealPlainTrackData(tmp, usOutLen, tmp1, &usOutLen1, 0x02, random, 0);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","解密后",tmp1, usOutLen1);

    Trace("Mai"," 算法 加密<333333333333333333333333333333333333333333333333333333333> \r\n");
    Trace("Mai","trackData1 = %s\r\n",trackData1);
    iRet = DealPlainTrackData(trackData1, strlen(trackData1), tmp, &usOutLen, 0x03, random, true);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","加密后",tmp, usOutLen);

    Trace("Mai"," 算法 解密<3333333333333333333333333333333333333333333333333333333333> \r\n");
    memset(tmp1, 0x00, sizeof(tmp1));
    iRet = DealPlainTrackData(tmp, usOutLen, tmp1, &usOutLen1, 0x03, random, false);
    Trace("Mai","iRet = %d\r\n",iRet);
    TraceHex("Mai","解密后",tmp1, usOutLen1);

}



