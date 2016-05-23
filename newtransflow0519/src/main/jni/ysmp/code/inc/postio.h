#ifndef AFX_POSTIO_H
#define AFX_POSTIO_H

extern s32 PostFlowInput(struct _INPUTMAP *pstInputMap, u8 *pasOutData,
                         void (*pDisFunc)(const u8 *pasDispData));

extern s32 InputBtName(u8 * pBtName);
extern s32 InputBtPwd(u8 * pBtPwd);

extern bool InputAny(struct _INPUTMAP *pstInputMap, u8 *pasAnyData,
                     s32(*pJudgeFunc)(u8 const *pasData, u8 const ucLen),
                     void (*pDisFunc)(const u8 *pasDispData));

extern u8 *ParseDispLineData(u8 *pasOut, const u8 ucMaxOutSize, const u8 *pasIn);

extern void DispLineData(const u8 *pasIn);

extern void DispClearContentRam(void);

#endif

