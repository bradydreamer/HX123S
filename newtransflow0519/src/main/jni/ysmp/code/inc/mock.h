#ifndef AFX_MOCK_H
#define AFX_MOCK_H

extern s32 GetTimerAutoRetValue(void);

extern void WaitEnterEsc(void);

extern u8 *SavePathOverlap(u8 *aid, u8 *pData, u32 size);

extern u8 *ReadPathOverlap(u8 *aid, u8 *pData, u32 size);

extern void DelPathFile(u8 * aid);

extern s32 sdkAudioCommState(u8 state[]);

extern s32 sdkAudioCommOpen();

extern void printByteArray(u8 *aid, u32 size);

#endif
