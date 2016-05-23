#ifndef _SDK_SYS_MEM_H_
#define _SDK_SYS_MEM_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

extern void *sdkDebugGetMem(s32 siSize, u8 *pFile, s32 siLine);

#define sdkGetMem(siSize)  sdkDebugGetMem((siSize) , (u8*)__FILE__, (s32)__LINE__)

extern s32 sdkDebugMemPrintNotFree(void);

extern s32 sdkFreeMem(void *pMem);

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif


