#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* __cplusplus */


#ifndef SDK_TOOLS_H
#define SDK_TOOLS_H


enum SDK_FILE_RSLT {
    SDK_FILE_NONE = SDK_SYS_RET_ERR_MAX +
                    (-5),                     //�ļ������ڻ��ߴ�ʧ��//file does not exist or open file failed
    SDK_FILE_CRCERR = SDK_SYS_RET_ERR_MAX + (-4),                   // �ļ�У���//check file error
    SDK_FILE_ERROR =
    SDK_SYS_RET_ERR_MAX + (-3),                    // �ļ��򿪻򴴽�ʧ��//open or create file failed
    SDK_FILE_SEEK_ERROR =
    SDK_SYS_RET_ERR_MAX + (-2),               // д�ļ���λԽ��//write file positioning cross the border
    SDK_FILE_EOF = SDK_SYS_RET_ERR_MAX +
                   (-1),                      // ������ʱ�����ļ�β//get end-of -file when writing
    SDK_FILE_OK =
    SDK_SYS_RET_OK_MIN + (1)                         // �ļ������ɹ�//file operations success
};

enum SDK_ALIGN_MODE {
    SDK_LEFT = SDK_SYS_RET_OK_MIN + 1,                              // ���//left
    SDK_RIGHT = SDK_SYS_RET_OK_MIN + 2,                             // �ұ�//right
    SDK_BOTH = SDK_SYS_RET_OK_MIN + 3                              // ����//both sides
};                                                                  // ɾ���Ŀո�λ�û����Ķ��뷽ʽAlignMode(���뷽ʽ)��//the place of deleted bland space or align mode after filling

#if SDK_IS_UCOS()
#ifndef FILE
#define FILE int
#endif  //end ifndef
#endif  //end if

/*******************************************************
   ��׼��Standard Version
 ********************************************************/

extern void sdkmSleep(const s32 siMs);

extern s32 sdkGetRandom(u8 *pheRdm, s32 siNum);

extern bool sdkDf(const u8 *pasDiskCata, s32 *psiAvailSize, s32 *psiUsedSize, s32 *psiUsedRate);

extern s32 sdkReadPosSn(u8 * pasDest);
extern s32 sdkGetRtc(u8 * pbcDest);

extern s32 sdkSetRtc(const u8 *pbcSrc);


//ʯ���� 2012.08.09 15:35  ��ʱ����ʹ��������������//functions about timer
extern u32 sdkTimerGetId(void);

extern bool sdkTimerIsEnd(u32 siId, u32 siMs);

extern const u8 *sdkTimerGetAsc(void);

extern s32 sdkInputIp(s32 siDsipLine, const u8 *pAsPrompt, s32 siInputLine, u8 *pAsIP, u8 uAlign,
                      s32 siTimeOut);

/*****************************�����ӿں�������͸��Other functions call these functions****************************/
extern bool sdkJudgeTime(const u8 *pbcTime, s32 siBcdLen);

extern bool sdkIsVisibleStr(u8 const
*pasSrc,
s32 siSrclen
);
extern bool sdkIsBcdNum(u8 const
*pheSrc,
s32 siSrclen
);

extern s32 sdkTruncateChar(const u8 *pasSrc, s32 siAvailableChar);

extern s32 sdkIniWriteKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey,
                          const u8 *pheValue);

extern s32 sdkIniReadKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey, u8 *pheValue);

//extern s32 sdkToolsGetVersion(u8 *pasDest);  //fusuipu modefied at 2013-1-29
extern s32 sdkStrpos(const u8 *pasStr, const u8 asChr);

extern s32 sdkTrim(u8 *pasStr, enum SDK_ALIGN_MODE eMode);

extern s32 sdkIniTrimBracket(u8 *pasStr, enum SDK_ALIGN_MODE eMode);

extern s32 sdkRegulateAmount(u8 *pasDest, const u8 *pbcAmount);

extern s32 sdkFormBlankAlignChar(enum SDK_ALIGN_MODE eMode, u8 *pasDest, const u8 *pasSrc,
                                 s32 siAvailableChar);

extern s32 sdkCmpYear(u8 bcYear1, u8 bcYear2);

extern bool sdkIsAsciiNum(u8 const
*pheSrc);

extern s32 sdkDelFileData(const u8 *pasFile, s32 iStart, s32 iDelLen);

extern s32 sdkSplitStr(u8 * pheDest, u8 const
*pheSrc,
s32 siSrartPos, s32
siDestLen);

extern s32 sdkIniGetOneLine(FILE *pstream, u8 *pucline, s32 siLen);

extern s32 sdkIniWriteOneLine(const FILE *pstream, const u8 *pucline);

extern s32 sdkSavePosSn(const u8 *pasSrc);

extern s32 sdkGetBmpSize(s32 *pWidth, s32 *pHeight, const u8 *pBmpFile);

#endif


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

