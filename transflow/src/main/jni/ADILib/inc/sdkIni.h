#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef __INI_H__
#define __INI_H__

//#include <windows.h>
//#include <tchar.h>


#define SDK_INI_NO_SECTION        -1
#define SDK_INI_NO_KEY            -2


typedef void *SDK_INI_HAND;


extern SDK_INI_HAND sdkIniLoad(const u8 *pFile);

//extern bool iniLoad(const char* lpFile,SDK_INI_CONTENT *pHand);
//bool iniLoad(const char* lpFile);
extern s32 sdkIniGetSectionNum(const SDK_INI_HAND pHand);

extern s32 sdkIniGetKeyNum(const SDK_INI_HAND pHand, const u8 *pSection);

extern s32 sdkIniGetSectionName(const SDK_INI_HAND pHand, s32 siIndex, u8 *pSectionName);

extern s32 sdkIniGetKeyName(const SDK_INI_HAND pHand, const u8 *pSection, s32 siIndex,
                            u8 *pKeyName);

extern s32 sdkIniGetKeyValue(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey,
                             u8 *pBuffer);

extern s32 sdkIniAddSection(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pComment);

extern s32 sdkIniAddKey(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey,
                        const u8 *pComment);

extern s32 sdkIniAddKeyValue(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey,
                             const u8 *pValue, const u8 *pComment);

extern s32 sdkIniClrSection(const SDK_INI_HAND pHand, const u8 *pSection);

extern s32 sdkIniRemoveSection(const SDK_INI_HAND pHand, const u8 *pSection);

extern s32 sdkIniRemoveSectionKey(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey);

extern s32 sdkIniSave(const SDK_INI_HAND pHand, const u8 *pFile);

extern s32 sdkIniClose(const SDK_INI_HAND pHand);

extern SDK_INI_HAND sdkCreatIni(void);


#if 0 /*Modify by shijianglong at 2013.03.12  18:46 */
extern BOOL iniWriteInt(const char* lpSection, const char* lpKey, unsigned int nValue, const char* lpComment,SDK_INI_CONTENT *pHand);
extern void iniGetInt(const char* lpSection, const char* lpKey, unsigned int* nDefault,SDK_INI_CONTENT *pHand);
extern void iniGetBool(const char* lpSection, const char* lpKey, BOOL* bDefault,SDK_INI_CONTENT *pHand);
extern BOOL iniWriteBool(const char* lpSection, const char* lpKey, BOOL bDefault, const char* lpComment,SDK_INI_CONTENT *pHand);
extern unsigned long iniInvertBool(const char* lpSection, const char* lpKey,SDK_INI_CONTENT *pHand);
#endif /* if 0 */

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

