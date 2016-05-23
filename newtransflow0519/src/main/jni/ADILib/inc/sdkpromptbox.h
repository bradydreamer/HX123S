#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef AFX_PROM_BOX_H
#define AFX_PROM_BOX_H

extern s32 sdkDispPromptBox(bool const bIsSaveScreen, s32 const siOvertimer, const u8 *asDispData);

extern s32 sdkDispLabel(s32 const
siStartLine,
u8 const *asDispData, u8 const
DispAtr);
extern s32 sdkDispPromptLabel(s32 const
siOvertimer,
s32 const siStartLine, s32 const
iEndLine,
const u8 *asDispData, u8 const
DispAtr);

extern s32 sdkDispMsgBox(const u8 *pasTitle, const u8 *pasDispData, s32 siOvertimer, s32 siKeyMask);

extern s32 sdkDispListBox(const u8 *pasTitle, const u8 *pasDispData, s32 siOvertimer);


#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

