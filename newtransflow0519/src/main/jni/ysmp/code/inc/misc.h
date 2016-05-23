#ifndef AFX_MISC_H
#define AFX_MISC_H


extern void FinishTradeFlow(void);

extern void GotoTradeFlow(void);

extern bool JudgeTradeFlow(void);

extern bool CheckTime(void);

extern void GetRtcSys(void);

extern void DispIcon(void);

extern void DispSysRtc(u8 RowNo, bool bDisp);

extern void ClearOthers(void);

extern u8 RegulateMoney(u8 * string, u8 * money);

extern s32 SposIccReadCardEx(ST_TRANSFLOW *pstTransFlow, u8 mode,
                             SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam,
                             SDK_ICC_CARDDATA *pstCardData, const u32 eDispRow, u8 *pcardTypeMask);

extern s32 AppGetMagCardNo(SDK_ICC_CARDDATA const
*pstCardData,
u8 *pasOut
);

extern void SetCmdResetExe(u8 CmdReset);

extern bool GetCmdResetExe(void);

extern bool AdjustTrackData(SDK_ICC_PARAM * pstIccParam, SDK_ICC_CARDDATA * pstCardData);


extern u8 GetCRC(u8 *pBuf, u16 iLen);


extern void CmdTestFun(u8 *buf, u16 len);

extern SDK_UI_EXIT_VALUE DrawLine(s32 ix);

extern SDK_UI_EXIT_VALUE DrawRect(s32 ix);

extern SDK_UI_EXIT_VALUE DrawBmp(s32 ix);

extern SDK_UI_EXIT_VALUE ClearScreen(s32 index);

extern SDK_UI_EXIT_VALUE SetBackLight(s32 ix);

extern SDK_UI_EXIT_VALUE DrawText(s32 ix);

extern SDK_UI_EXIT_VALUE LoadMenu(s32 ix);

extern SDK_UI_EXIT_VALUE updateTMK_Test(s32 ix);

extern SDK_UI_EXIT_VALUE testSaveTLV2(s32 ix);

extern SDK_UI_EXIT_VALUE testReadTLV(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnSetBeep(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnSetLED(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnSetDateTime(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnGetDateTime(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnSetTerParam(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnGetTerParam(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnEcho(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnCtrlDev(s32 ix);


extern SDK_UI_EXIT_VALUE TestOnDataEncrypt(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnMAC(s32 ix);


extern SDK_UI_EXIT_VALUE TestOnOpenMagCardDevice(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnCloseMagCardDevice(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnReadMagCard(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnReadMagCardEncrypt(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnReadMagCardSave(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnReadMagCardAudio(s32 ix);

extern SDK_UI_EXIT_VALUE TestOnCancelOper(s32 ix);


extern SDK_UI_EXIT_VALUE CheckICCard(s32 ix);

extern SDK_UI_EXIT_VALUE PowerOnICCard(s32 ix);

extern SDK_UI_EXIT_VALUE PowerOffICCard(s32 ix);

extern SDK_UI_EXIT_VALUE ICCardCommun(s32 ix);


extern SDK_UI_EXIT_VALUE GetRec(s32 ix);

extern SDK_UI_EXIT_VALUE InitStoreRec(s32 ix);

extern SDK_UI_EXIT_VALUE GetRecCount(s32 ix);

extern SDK_UI_EXIT_VALUE AddRec(s32 ix);

extern SDK_UI_EXIT_VALUE UpdateRec(s32 ix);

extern s32 pkcs5Padding(u8 *ucInData, u32 uiInDataLen, u8 *ucOutData);

extern s32 pkcs5UnPadding(u8 *ucInData, u32 uiInDataLen);

extern void SetScreenStatusWhenBack(u8 cmdClass, u8 cmdID);//Xiaox 15-1-12
#endif

