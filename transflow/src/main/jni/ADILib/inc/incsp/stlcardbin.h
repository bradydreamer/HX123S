#ifndef AFX_STL_CARDBIN_H
#define AFX_STL_CARDBIN_H

typedef enum {
    CMP_BLACK = 0,         //�������ȽϷ�ʽ
    CMP_CARDBIN = 1       //��bin�ȽϷ�ʽ
} CMP_TYPE;

typedef enum {
    RSLT_SUCC = 0,        //�ɹ�
    RSLT_FAILS = 1,        //ʧ��
    RSLT_EXSIT = 2        //����
} RSLT;

#define MIN_CARDLEN 2
#define MAX_CARDLEN 19


/*=======BEGIN: sujianzhong 2013.01.18  15:25 modify===========*/
typedef enum {
    CARDBIN_RESTART = 0,        //���¿�ʼ��ȡ����
    CARDBIN_CONTINUE = 1        //������ȡ����
} GET_CARDBIN_TYPE;//��ȡ��bin�ķ�ʽ
/*====================== END======================== */
extern void CardBinGetLibVersion(unsigned char *lpOut);

extern bool CardBinInit(unsigned char *pasCardBinPath, CMP_TYPE cmptype);

extern void CardBinMemFree();

extern void CardBinBlackBitDisable(unsigned char whitchbit);

extern void CardBinBlackBitEnable(unsigned char whitchbit);

extern bool CardBinSearch(const unsigned char *pasCardNo, unsigned char len);

extern RSLT CardBinAppend(const unsigned char *pasCardNo, unsigned char len);

extern RSLT CardBinDelete(const unsigned char *pasCardNo, unsigned char len);

extern void CardBinCmpType(CMP_TYPE cmptype);

/*=======BEGIN: sujianzhong 2013.01.18  17:48 modify===========*/
extern bool CardBinGetTotalNum(unsigned int *puiTotalNum);

extern int CardBinGetData(GET_CARDBIN_TYPE getType, unsigned char *pasDataOut,
                          unsigned char *pucDataLen);
/*====================== END======================== */
#endif



