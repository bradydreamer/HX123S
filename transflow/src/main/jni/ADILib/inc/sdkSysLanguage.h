#ifndef _SDK_SYS_LANGUAGE_H_
#define _SDK_SYS_LANGUAGE_H_

#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

#ifndef __SDK_LANGUAGE_H__
#define __SDK_LANGUAGE_H__


typedef enum {
    SDK_SYS_LANG_CHN = 0,        //���Ĺ��������
    SDK_SYS_LANG_BIG5,            //����BIG�뷱��
    SDK_SYS_LANG_FAR,            //��˹��
    SDK_SYS_LANG_ARA,            //��������
    SDK_SYS_LANG_SJI,            //����
    SDK_SYS_LANG_HAN,            //����
    SDK_SYS_LANG_RUS,           // ����
    SDK_SYS_LANG_GER,           // ��³����
    SDK_SYS_LANG_UNICODE,
    SDK_SYS_LANG_ASCII,            //��Զ����������ڼ����ֿ����ݽṹ
    SDK_SYS_LANG_ENG,           //Ӣ��
    SDK_SYS_LANG_DEF,
} SDK_SYS_LANG;

typedef enum {
    SDK_SYS_LINE4 = 4,              //4��ģʽ
    SDK_SYS_LINE5 = 5               //5��ģʽ
} SDK_SYS_LINE_MODE;

typedef enum {
    SDK_SYS_LINE1_LINE4 = 1,        //ѡ��LINE1~4����ʾ
    SDK_SYS_LINE2_LINE5 = 2         //ѡ��LINE2~5����ʾ
} SDK_SYS_LINE_CHOICE;

typedef struct {
    SDK_SYS_LINE_MODE eLine;        //������
    SDK_SYS_LINE_CHOICE eLineMode;  //����ʾģʽ
    void *pVar;                     //Ԥ������
} SDK_SYS_LINE_INFOR;

extern s32 sdkSysMultiLangInit(SDK_SYS_LANG eLang, u8 uiMaxLine);

extern SDK_SYS_LANG sdkSysMultiGetLangMode(void);

s32 sdkSysMultiGetLineMode(SDK_SYS_LINE_INFOR *pstDipsLineInfo);

s32 sdkSysMultiSetLineMode(const SDK_SYS_LINE_INFOR *pstDipsLineInfo);

#endif

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */


#endif/* _SDK_SYS_LANGUAGE_H_ */



