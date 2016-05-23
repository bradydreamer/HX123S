#include "sdkglobal.h"
#include "sdkIccPrivateHead.h"

#ifndef  DEBUG_EMV
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


s32 sdkIccReadPanelVersion(u8 *pVersion) { return SDK_ERR; }

void sdkIccIncReaderCurrentSn() { }

s32 sdkIccSendDataToReader() { return SDK_ERR; }

void sdkIccDispCommuReader() { }

u8 sdkIccRecDataFormReader(u16 tv, u8 resend_times) { return 0; }

void sdkIccDispReaderErrRespond(u8 respond) { }

void sdkIccDispCommuReaderError() { }

void sdkIccDispReaderErrorInfo() { }

void sdkIccDispOperationReaderRlt(u8 rlt) { }

u8 sdkIccCommuToReader(u16 tv, u8 resend_times, u8 success_cmd) { return 0; }

void sdkIccFillSendReaderData(u8 sn, u8 cmd, const u8 *buf, u16 len) { }

u8 sdkIccSetReaderCapability() { return 0; }

u8 sdkIccSetReaderTime() { return 0; }

u8 sdkIccResumeReader() { return 0; }

u8 sdkIccSetReaderCapk(const CAPK_STRUCT *capk, u8 type) { return 0; }

u8 sdkIccSetReaderAid(EmvSysIni_STRUCT *aid, u8 type, u8 app_id) { return 0; }

u8 sdkIccSetReaderCvm() { return 0; }

u8 sdkIccSetReaderPBOCParam() { return 0; }

u8 sdkIccSetReaderPBOCTag() { return 0; }

u8 sdkIccPollReader() { return RC_POLL_N; }

void sdkIccSaveQReaderKey() { }

void sdkIccInitReaderIMEKmdkIAEKmdk() { }

void sdkIccReadQReaderKey() { }

u8 sdkIccSetReaderDesType() { return EMV_ERR; }

void sdkIccInitReaderParam() { }

void sdkIccGetRandomNew(u8 *rdm, u32 num) { }

u8 sdkIccGetReaderCurrentKey(u8 type, u8 ucIndex) { return 0; }

u8 sdkIccDealReaderCommuInit() { return 0; }

u8 sdkIccReaderCommuInit(u8 type, u8 ucIndex) { return 0; }

u8 sdkIccDoubleAuthReader(u8 type, u8 ucIndex) { return 0; }

void sdkIccGetKeyName(u8 type, u8 *name) { }

u8 sdkIccCreateReaderKey(u8 type, u8 ucIndex) { return 0; }

u8 sdkIccFirstAuthReader() { return 0; }

u8 sdkIccNormalPowerOnReader() { return 0; }

u8 sdkIccReaderKeyInit() { return 0; }

u8 sdkIccSetReaderWorkMode(u8 cmd, u8 mode) { return 0; }

u8 sdkIccSetReaderSelPara() { return 0; }

u8 sdkSetReaderDefaultParamDetail() { return 0; }

u8 sdkIccSetReaderDefaultParam() { return 0; }

u8 sdkIccAddOneAidToReader(EmvSysIni_STRUCT *aid) { return 0; }

u8 sdkIccAddAllAidToReader() { return 0; }

u8 sdkIccDelOneAidToReader(EmvSysIni_STRUCT *aid) { return 0; }

u8 sdkIccDelAllAidToReader() { return 0; }

u8 sdkIccAddOneCapkToReader(const CAPK_STRUCT *capk) { return 0; }

u8 sdkIccAddAllCapkToReader() { return 0; }

u8 sdkIccDelOneCapkToReader(const CAPK_STRUCT *capk) { return 0; }

u8 sdkIccDelAllCapkToReader() { return 0; }

u8 sdkIccGetReaderId(u8 *id) { return 0; }

void sdkIccInitReaderSys() { }

u8 sdkIccIfHaveQReader() { return 0; }

u8 sdkIccJudgeEnableQReaderTrade() { return 0; }

void sdkIccResetQReader() { }

u8 sdkIccDealReaderQPBOCSuccess(u8 *buf, u16 len) { return QREDER_RLT_FAIL; }

u8 sdkIccSyncReaderTime() { return 0; }

void sdkIccSaveQReaderId() { }

void sdkIccReadQReaderId() { }

void sdkIccQReaderDispTradeStatus(u8 success) { }

void sdkIccInitqReaderTermInfo() { }

void sdkIccSetCheckState(u8 stata) { }

bool sdkIccCheckStateSet(void) { return true; }

void sdkIccSendQReaderQuery() { }

void sdkIccDealQReaderQuery() { }

u8 sdkIccDisableQreader() { return 0; }

u8 sdkIccFirstPowerOnreader() { return 0; }

u8 sdkIccSyncReaderTermNo() { return 0; }

void sdkIccDispBlackCardError() { }

u8 sdkIccCheckQreaderType() { return QREADER_TYPE_IN; }

void sdkIccDealSyncQreaderCapkErr() { }

void sdkIccDealSyncQreaderAidErr() { }

void sdkIccDealQreaderBackErr(const u8 *info, s32 siLen) { }

void sdkIccSaveQreaderCurrentStatusRamdisk(void) { }

u8 SdkIccReadqReaderCurrentStatusRamdisk() { return 0; }

void sdkIccSwitchAppResumeQreader() { }

u8 sdkIccCheckCurAppNeedStm32() { return 1; }

u8 sdkIccSetQreaderPollP() { return 0; }

s32 sdkIccSendQReaderSale(const u8 *pAmount_Bcd) { return SDK_OK; }

s32 sdkIccDealQReaderTrade(const SDK_ICC_TRADE_PARAM *pIccTradeParam) { return SDK_ERR; }

void sdkIccPowerStartReader() { }

bool sdkIccCheckReaderParamOK(void) { return false; }

void sdkIccSendQReaderBlacklist(u8 ucCode) { }

