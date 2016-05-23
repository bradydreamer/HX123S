#include "sdkGlobal.h"

#ifndef  DEBUG_FILE
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define DEF_PROFILE_NUM_LEN     64

typedef struct _record {
    u8 comments[DEF_PROFILE_NUM_LEN];
    u8 key[DEF_PROFILE_NUM_LEN];
    u8 value[DEF_PROFILE_NUM_LEN];
    struct _record *next;
} record;

typedef struct _section {
    record *first;
    record *last;
    s32 sizeRecord;
    u8 comments[DEF_PROFILE_NUM_LEN];
    u8 name[DEF_PROFILE_NUM_LEN];
    struct _section *next;
} section;

struct _content {
    s32 sizeSection;
    section *first;
    section *last;
};

//content* pContent;
//char gfilename[255];

static struct _record *sdkIniGetRecord(const struct _section *psection, const u8 *lpKey);

static s32 iniLoad(const SDK_INI_HAND pHand, const char *lpFile) {
    FILE *fp;
    char buffer[255] = {0};

#if 0 /*Modify by wangmingming at 2014.05.20  10:58 */
    char comments[1024] = {0};
#else
    u8 *comments = (u8 *) sdkGetMem(600); //����600B�Ĵ�heap����

    if (NULL == comments) {
        Trace("test", "�ڴ�����ʧ��");
        return false;
    }
    memset(comments, 0, 600);
#endif /* if 0 */
    char NowSection[255] = {0};
    char key[255] = {0};
    char value[255] = {0};
    char *pdest;
    int idx, ret = 0;
    int len = 0;

    strcpy(comments, "");
    strcpy(NowSection, "");

    //��ֻ��ģʽ���ļ�
    if ((fp = fopen(lpFile, "r")) != NULL) //lint !e64
    {
        //���ļ��ж�ȡһ���ַ���
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
#if 0 /*Modify by wangmingming at 2014.05.27  9:45 */
            if ( feof(fp) )
            { //�ڶ�ȡ֮ǰ�ж��Ƿ��ȡ���ļ�β�����ѭ��fgets�����˳���bug
                Trace("ini", "fgets�Ѷ����ļ�β\r\n");
                break;
            }
#endif /* if 0 */

            /*=======BEGIN: fusuipu 2013.05.15  10:20 modify��linux��ߣ��س�������Ϊ0x0d 0x0a===========*/
            len = strlen(buffer);
            Trace("ini", "fgets buffer:%s\r\n", buffer);

            if (buffer[len - 2] == '\r' && buffer[len - 1] == '\n') { //����س��������з�
                buffer[len - 2] = 0X00;
                buffer[len - 1] = 0X00;
            }

            /*====================== END======================== */
            if (buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            if (buffer[len - 1] == '\r') {
                buffer[len - 1] = '\0';
            }

            switch (buffer[0]) {
                case '[': //��һ���ַ�Ϊ��ʼ������
                {
                    pdest = strrchr(buffer, ']'); //����ƥ��Ľ���������

                    if (pdest == NULL) {
                        fclose(fp);
                        if (comments != NULL) {
                            sdkFreeMem(comments);
                        }
                        Trace("ini", "parse ini error!\n");
                        return false;
                    }
                    idx = pdest - buffer; //section��������
                    memcpy(NowSection, buffer + 1, idx - 1); //����section������
                    NowSection[idx - 1] = '\0';
                    ret = sdkIniAddSection((struct _content *) pHand, NowSection, comments);
                    //Trace("ini", "sdkIniAddSection NowSection:%s, comments:%s\r\n",
                    //NowSection, comments);
                    strcpy(comments, "");
                }
                    break;

                case '#':
                case ';': {
                    if (strlen(comments) > 0) {
                        strcat(comments, "\n");
                    }
                    strcat(comments, buffer);
                    //Trace("ini", "sdkIniAddSection comments:%s\r\n", comments);
                }
                    break;

                default: {
                    pdest = strrchr(buffer, '=');

                    if (pdest == NULL) {
                        continue;
//                            fclose(fp);
//                            printf("parse ini error\n");
//                            return false;
                    }
                    idx = pdest - buffer;
                    memcpy(key, buffer, idx);
                    key[idx] = '\0';
                    memset(value, 0, sizeof(value)); //fusuipu modefied at 2013-5-15
                    memcpy(value, buffer + idx + 1, (strlen(buffer) - idx) -
                                                    1); //lint !e818 !e737 //fusuipu modefied at 2013-5-15
                    //Trace("ini", "sdkIniAddSection NowSection:%s, key:%s, comments:%s\r\n",
                    //NowSection, key, comments);
                    if (strcmp(NowSection, "") == 0) {
                        fclose(fp);
                        if (comments != NULL) {
                            sdkFreeMem(comments);
                        }
                        Trace("ini", "parse ini error\n");
                        return false;
                    }
                    else {
                        /*=======BEGIN: fusuipu 2013.05.14  15:58 modify===========*/
                        ret = sdkIniAddKey(pHand, NowSection, key, comments);  //��Ҫ���keyѡ��

                        if (ret < 0) {
                            return ret;     //key��Ӵ���ֱ�ӷ���
                        }
                        /*====================== END======================== */
                        ret = sdkIniAddKeyValue((struct _content *) pHand, NowSection, key, value,
                                                comments);
                        //Trace("ini", "sdkIniAddKeyValue NowSection:%s, key:%s, value:%s, comments=%s\r\n",
                        //NowSection, key, value, comments);
                        strcpy(comments, "");
                    }
                }
                    break;
            } //end swtich

            //Trace("ini", "buffer :%s\r\n", buffer);
            memset(buffer, 0, sizeof(buffer)); //fusuipu modefied at 2013-5-15
        } //end while

        fclose(fp);
    }
    else {
        Trace("ini", "open file error !\n");
        if (comments != NULL) {
            sdkFreeMem(comments);
        }
        return SDK_PARA_ERR;   //fusuipu 2013.03.20 8:39
    }
    if (comments != NULL) {
        sdkFreeMem(comments);
    }
    return ret;
}

static struct _record *sdkIniGetRecord(const struct _section *psection, const u8 *lpKey) {
    bool found = false;
    struct _record *pRecord;

    if (psection != NULL) {
        pRecord = psection->first;

        while (pRecord != NULL) {
            if (strcmp(lpKey, pRecord->key) == 0) {
                found = true;
                break;
            }
            pRecord = pRecord->next;
        }

        if (found == true) {
            return pRecord;
        }
        else {
            return NULL;
        }
    }
    else {
        return NULL;
    }
}

static struct _section *sdkIniGetSection(const SDK_INI_HAND pHand, const char *lpSection) {
    struct _section *psection = ((struct _content *) pHand)->first;

    while (psection != NULL) {
        if (strcmp(psection->name, lpSection) == 0) {
            return psection;
        }
        psection = psection->next;
    }

    return NULL;
}   //lint !e818

/*****************************************************************************
** Descriptions:	����ini�ļ����
** Parameters:          void:
** Returned value:	������ini�ļ����
** Created By:		fusuipu  2013.05.20
** Modified By:		wangmingming  2014.05.20
** Remarks:
*****************************************************************************/
SDK_INI_HAND sdkCreatIni(void) {
    struct _content *pHand;

    pHand = (struct _content *) sdkGetMem(sizeof(struct _content));

    if (NULL == pHand) {
        Trace("ini", "cannot sdkGetMem memory !\n");
        return NULL;
    }
    //��ʼ��
    memset(pHand, 0, sizeof(struct _content)); //shiweisong 2013.09.29 15:42
    pHand->sizeSection = 0;
    pHand->first = NULL;
    pHand->last = NULL;

    return (SDK_INI_HAND) pHand;
}

/*****************************************************************************
** Descriptions:	����ini�ļ���Ϣ
** Parameters:          const u8* pFile:�ļ�·������
** Returned value:
                    NULL	ʧ�ܻ��߲�����
                    SDK_INI_HANDָ��    �ɹ�

** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.20
** Remarks:
*****************************************************************************/
SDK_INI_HAND sdkIniLoad(const u8 *pFile) {
    struct _content *pHand;

    if (NULL == pFile) {
        Assert(0);
        return NULL;
    }
#if 0 /*Modify by wangmingming at 2014.05.26  19:15 */
    pHand = (struct _content *) sdkGetMem(sizeof(struct _content));

    if (NULL == pHand)
    {
        Trace("ini", "cannot sdkGetMem memory !\n");
        return NULL;
    }
    memset(pHand, 0, (sizeof(struct _content)));
    pHand->sizeSection = 0;
    pHand->first = NULL;
    pHand->last = NULL;
#else
    pHand = (struct _content *) sdkCreatIni();

    if (NULL == pHand) {
        Trace("ini", "sdkCreatIni Err!\n");
        return NULL;
    }
#endif /* if 0 */

    if (SDK_PARA_ERR == iniLoad(pHand, pFile)) {
        Trace("ini", "initial parse file error !\n");
        sdkFreeMem(pHand);
        return NULL;
    }
    return (SDK_INI_HAND) pHand;
}

/*****************************************************************************
** Descriptions:	��ȡ����С�ڸ���
** Parameters:          const SDK_INI_HAND pHand:
** Returned value:
                    ��ȷ����С�ڸ���
                    �������󷵻�SDK_PARA_ERR
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniGetSectionNum(const SDK_INI_HAND pHand) {
    s32 i = 0;

    struct _section *psection;

    if (NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    psection = ((struct _content *) pHand)->first;

    while (psection != NULL) {
        i++;
        psection = psection->next;
    }

    Trace("test", "sdkIniGetSectionNum i=%d\r\n", i);

    return i;
} //lint !e818

/*****************************************************************************
** Descriptions:	��ȡĳС�ڹؼ��ָ���
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    const u8* pSection:
** Returned value:
                    �ɹ����عؼ��ָ���
                    ����С�����ƻ�ڵ�ΪNULL����С�����Ƴ��ȳ����򷵻ز�����SDK_PARA_ERR
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniGetKeyNum(const SDK_INI_HAND pHand, const u8 *pSection) {
    s32 i = 0;
    struct _section *psection;
    struct _record *pRecord;

    if (NULL == pSection || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (sizeof(((struct _section *) 0)->name) < strlen(pSection))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (psection != NULL) {
        //   Trace("fusp", "sdkIniGetKeyNum test!\n");
        //   Trace("fusp", "comments = %s, name = %s\n", psection->comments, psection->name);

        if (psection->first == NULL) {
            Trace("fusp", "psection->first == NULL\n");
        }
        pRecord = psection->first;
        //   Trace("fusp", "comments = %s, key = %s,value = %s\n", pRecord->comments, pRecord->key, pRecord->value);

        while (pRecord != NULL) {
            i++;
            pRecord = pRecord->next;
        }

        return i;
    }
    else {
        return SDK_INI_NO_SECTION;
    }
}

/*****************************************************************************
** Descriptions:	��ȡĳһС������
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    s32 siIndex:���������㿪ʼΪ��һ��������
                    u8 *pSectionName:��ȡ��С������
** Returned value:
                    �ɹ����ػ�ȡ�����ֳ���
                    ����С�����ƻ�ڵ�ΪNULL�򷵻ز�����SDK_PARA_ERR
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniGetSectionName(const SDK_INI_HAND pHand, s32 siIndex, u8 *pSectionName) {
    s32 i = 0;
    struct _section *psection;

    if (NULL == pSectionName || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = ((struct _content *) pHand)->first;

    while (psection != NULL) {
//        if (strcmp(psection->name, pSection) == 0)
//        {
//            found = true;
//            break;
//        }
        if (i == siIndex) {
            strcpy(pSectionName, psection->name);
            break;
        }
        i++;
        psection = psection->next;
    }

    return strlen(pSectionName);
} //lint !e818

/*****************************************************************************
** Descriptions:	��ȡĳһС����ĳ���ؼ�������
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    const u8* pSection:С������
                    s32 siIndex:���������㿪ʼΪ��һ��������
                    u8 *pKeyName:��ȡ�Ĺؼ�������
** Returned value:
                    �ɹ����ػ�ȡ�����ֳ���
                    С�����Ʋ������򷵻�SDK_INI_NO_SECTION
                    ����С�����ơ��ڵ���߹ؼ�������ΪNULL�򷵻ز�����SDK_PARA_ERR
                    ����С�����Ƴ��ȳ��޷���SDK_PARA_ERR
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniGetKeyName(const SDK_INI_HAND pHand, const u8 *pSection, s32 siIndex, u8 *pKeyName) {
    s32 i = 0;
    struct _section *psection;
    struct _record *pRecord;

    if (NULL == pSection || NULL == pKeyName || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (sizeof(((struct _section *) 0)->name) < strlen(pSection))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (psection != NULL) {
        pRecord = psection->first;

        while (pRecord != NULL) {
            if (i == siIndex) {
                strcpy(pKeyName, pRecord->key);
                break;
            }
            i++;
            pRecord = pRecord->next;
        }

        return strlen(pKeyName);
    }
    else {
        return SDK_INI_NO_SECTION;
    }
}

/*****************************************************************************
** Descriptions:	��ȡ�ؼ�������
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    const u8* pSection:С������
                    const u8* pKey:�ؼ�������
                    u8* pBuffer:��ȡ������
** Returned value:
                    �ɹ����ػ�ȡ���ݳ���
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����ƻ��߹ؼ������Ƴ��ȳ����򷵻�SDK_PARA_ERR
                    û�и�С�ڷ���SDK_INI_NO_SECTION
                    û�иùؼ��ַ���SDK_INI_NO_KEY
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniGetKeyValue(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey, u8 *pBuffer) {
    struct _section *psection;

    struct _record *precord;

    if (NULL == pSection || NULL == pKey || NULL == pBuffer || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);
    precord = sdkIniGetRecord(psection, pKey);

    if ((sizeof(((struct _section *) 0)->name) < strlen(pSection)) ||
        (sizeof(((struct _record *) 0)->key) < strlen(pKey)))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (psection == NULL) {
        return SDK_INI_NO_SECTION;
    }

    if (precord == NULL) {
        return SDK_INI_NO_KEY;
    }
    else {
        strcpy(pBuffer, precord->value);
        return strlen(pBuffer);
    }
}

/*****************************************************************************
** Descriptions:	����ini��һ��С��
** Parameters:          const SDK_INI_HAND pHand:SDK_INI_CONTENTָ��
                    const u8* pSection:С������
                    const u8* pComment:ע��
** Returned value:
                    �ڴ���󷵻�:SDK_ERR
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����ƻ���ע�ͳ��ȳ����򷵻�SDK_PARA_ERR
                    ��ȷ����:С��������
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.20
** Remarks:
*****************************************************************************/
s32 sdkIniAddSection(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pComment) {
    struct _section *psection;

    if (NULL == pSection ||
        NULL == pHand) //|| NULL == pComment ����Ϊ��//shiweisong 2013.09.29 15:36
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((sizeof(((struct _section *) 0)->name) < strlen(pSection))) { //���pSection�������鶨�峤��
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((pComment != NULL) &&
        (sizeof(((struct _section *) 0)->comments) <= strlen(pComment))) { //���pComment�������鶨�峤��
        Assert(0);
        return SDK_PARA_ERR;
    }
    //��ȡsection
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (NULL == psection) {
        psection = (struct _section *) sdkGetMem(sizeof(struct _section));

        if (psection == NULL) {
            Trace("ini", "cannot sdkGetMem memory !\n");
            return SDK_ERR;
        }
        memset(psection, 0, sizeof(struct _section));

        strcpy(psection->name, pSection);

        if (pComment != NULL) //pComment �����Ϊ��shiweisong 2013.09.29 15:39
        {
            if ((pComment[0] != '#' || pComment[0] != ';') && (strlen(pComment) > 0)) {
                sprintf(psection->comments, "#%s", pComment);
            }
            else {
                strcpy(psection->comments, pComment);
            }
        }
        psection->first = NULL;
        psection->last = NULL;
        psection->next = NULL;
        psection->sizeRecord = 0;

        ((struct _content *) pHand)->sizeSection++;

        if (((struct _content *) pHand)->first == NULL) {
            ((struct _content *) pHand)->first = psection;
            ((struct _content *) pHand)->last = psection;
        }
        else {
            ((struct _content *) pHand)->last->next = psection;
            ((struct _content *) pHand)->last = psection;
        }
    }
    else {
        strcpy(psection->name, pSection);

        if (pComment != NULL) //pComment �����Ϊ��shiweisong 2013.09.29 15:39
        {
            if ((pComment[0] != '#' || pComment[0] != ';') && (strlen(pComment) > 0)) {
                sprintf(psection->comments, "#%s", pComment);
            }
            else {
                strcpy(psection->comments, pComment);
            }
        }
    }
    return strlen(pSection);
}

/*****************************************************************************
** Descriptions:	���ӹؼ���
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    const u8* pSection:С������
                    const u8* pKey:�ؼ�������
                    const u8* pComment:ע�����ƣ����û��ע�Ϳ�д""
** Returned value:
                    �ڴ���󷵻�:SDK_ERR
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����ơ��ؼ��ֻ���ע�ͳ��ȳ����򷵻�SDK_PARA_ERR
                    С�ڲ����ڷ���SDK_INI_NO_SECTION
                    ��ȷ����:�ؼ��ֳ���
** Created By:		shijianglong  2013.03.14
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniAddKey(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey, const u8 *pComment) {
    struct _section *psection;
    struct _record *precord;


    if (NULL == pSection || NULL == pKey || pHand == NULL)   //fusuipu 2013.05.15 17:13
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((sizeof(((struct _section *) 0)->name) < strlen(pSection)) ||
        (sizeof(((struct _record *) 0)->key) < strlen(pKey)))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((pComment != NULL && (sizeof(((struct _record *) 0)->comments) <= strlen(pComment)))) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    /*=======BEGIN: fusuipu 2013.05.15  15:58 modify===========*/
    if (psection == NULL) {
        sdkIniAddSection(pHand, pSection, pComment);
        psection = sdkIniGetSection(pHand, pSection);
    }

    if (psection == NULL) {
        return SDK_INI_NO_SECTION;
    }
    /*====================== END======================== */
    precord = sdkIniGetRecord(psection, pKey);

    if (precord == NULL) {
//        return SDK_INI_NO_KEY;
        precord = (struct _record *) sdkGetMem(sizeof(struct _record));

        if (precord == NULL) {
            Trace("ini", "cannot sdkGetMem memory !\n");
            return SDK_ERR;
        }
        memset(precord, 0, (sizeof(struct _record)));

        precord->next = NULL;

        psection->sizeRecord++;

        if (psection->first == NULL) {
            psection->first = precord;
            psection->last = precord;
        }
        else {
            psection->last->next = precord;
            psection->last = precord;
        }
    }

    if (pComment != NULL) //shiweisong 2013.09.29 16:7
    {
        if ((pComment[0] != '#' || pComment[0] != ';') && (strlen(pComment) > 0)) {
            sprintf(precord->comments, "#%s", pComment);
        }
        else {
            strcpy(precord->comments, pComment);
        }
    }
    strcpy(precord->key, pKey);
    {
        strcpy(precord->value, "");
    }
    return strlen(pKey);
}

/*****************************************************************************
** Descriptions:	д��ini�ؼ�������
** Parameters:      const u8* pSection:С������
                    const u8* pKey:�ؼ���
                    const u8* pValue:д������
                    const u8* pComment:д���ע�ͣ���������"".
                    SDK_INI_HAND *pHand:SDK_INI_CONTENTָ��
** Returned value:	�ڴ���󷵻�:SDK_ERR
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����ơ��ؼ��֡����ݻ���ע�ͳ��ȳ����򷵻�SDK_PARA_ERR
                    �޴�С�ڷ���SDK_INI_NO_SECTION
                    �޴˹ؼ��ַ���SDK_INI_NO_KEY
                    �ɹ�����:д�����ݳ���
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniAddKeyValue(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey,
                      const u8 *pValue, const u8 *pComment) {
    struct _section *psection;
    struct _record *precord;

    if (NULL == pSection || NULL == pKey || NULL == pValue ||
        NULL == pHand) //fusuipu 2013.05.15 17:17
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    /*=======BEGIN: fusuipu 2013.05.15  17:27 modify===========*/
    if ((sizeof(((struct _section *) 0)->name) < strlen(pSection)) ||
        (sizeof(((struct _record *) 0)->key) < strlen(pKey)) ||
        (sizeof(((struct _record *) 0)->value) < strlen(pValue)))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((pComment != NULL) &&
        (sizeof(((struct _record *) 0)->comments) <= strlen(pComment))) //shiweisong 2013.09.29 16:9
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    /*====================== END======================== */
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (psection == NULL) {
        sdkIniAddSection(pHand, pSection, pComment);
        psection = sdkIniGetSection(pHand, pSection);
    }

    if (psection == NULL) {
        return SDK_INI_NO_SECTION;
    }
    precord = sdkIniGetRecord(psection, pKey);

    /*=======BEGIN: fusuipu 2013.05.14  15:40 modify===========*/
    if (precord == NULL) {
        sdkIniAddKey(pHand, pSection, pKey, pComment);
        precord = sdkIniGetRecord(psection, pKey);
    }

    if (precord == NULL) {
        return SDK_INI_NO_SECTION;
    }

    if (pComment != NULL) {
        if ((pComment[0] != '#' || pComment[0] != ';') && (strlen(pComment) > 0)) {
            sprintf(precord->comments, "#%s", pComment);
        }
        else {
            strcpy(precord->comments, pComment);
        }
    }
    strcpy(precord->key, pKey);
    {
        strcpy(precord->value, pValue);
    }
    return strlen(pValue);
}

/*****************************************************************************
** Descriptions:	���ini�ļ���ĳ��С�ڵ�����
** Parameters:      const char* pSection:С������
                    const SDK_INI_HAND *pHand:SDK_INI_CONTENTָ��
** Returned value:	�޴�С�ڷ���:SDK_INI_NO_SECTION
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����Ƴ��ȳ����򷵻�SDK_PARA_ERR
                    �ɹ�����:С���ڹؼ��ָ���
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:         �����С���ڵ����ݣ������ؼ��ֵȣ�ֻ��С�����ơ�
*****************************************************************************/
s32 sdkIniClrSection(const SDK_INI_HAND pHand, const u8 *pSection) {
    struct _section *psection;
    struct _record *precord;
    int del_num = 0;

    if (NULL == pSection || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(((struct _section *) 0)->name) < strlen(pSection))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (psection == NULL) {
        return SDK_INI_NO_SECTION;
    }
    precord = psection->first;

    while (precord != NULL) {
        psection->first = precord->next;
        psection->sizeRecord--;
        sdkFreeMem(precord);
        del_num++;
        precord = psection->first;
    }

    return del_num;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_INI_HAND pHand
                               const char* lpSection
** Returned value:
** Created By:		fusuipu  2013.05.16
** Remarks:
*****************************************************************************/
static s32 sdkIniDeleSection(SDK_INI_HAND pHand, const char *lpSection) {
    struct _section *psecpre = NULL;
    struct _section *psection = ((struct _content *) pHand)->first;
    s32 conter = 1;

    while (psection != NULL) {
        conter++;

        if (strcmp(psection->name, lpSection) == 0) {
            if (conter == 1) {
                ((struct _content *) pHand)->first = psection->next;
            }
            else {
                psecpre->next = psection->next; //lint !e613
            }
            sdkFreeMem(psection);
            ((struct _content *) pHand)->sizeSection--;
            return SDK_OK;
        }
        psecpre = psection;
        psection = psection->next;
    }


    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	���һ��С��
** Parameters:          const SDK_INI_HAND pHand:����ini�ļ����ص�SDK_INI_HANDָ��
                    const u8* pSection:С������
** Returned value:
                    SDK_PARA_ERR    ����С�ڻ��߽ڵ�ΪNULL���ߴ���С�����Ƴ��ȳ����򷵻�
                    SDK_OK  ����ɹ�
                    SDK_ERR ���ʧ��

** Created By:		fusuipu  2013.05.16
** Modified By:		wangmingming  2014.05.26
** Remarks:             ɾ������С�����Ƶ�ȫ��С�����ݡ�
*****************************************************************************/
s32 sdkIniRemoveSection(const SDK_INI_HAND pHand, const u8 *pSection) {
    s32 ret;

    if (NULL == pSection || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sizeof(((struct _section *) 0)->name) < strlen(pSection))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = sdkIniClrSection(pHand, pSection);

    if (ret < 0) {
        return ret;
    }
    return sdkIniDeleSection(pHand, pSection);
}

/*****************************************************************************
** Descriptions:	ɾ��ĳС���ڵ�һ���ؼ���
** Parameters:      const char* pSection:С������
                    const char* pKey:�ؼ���
                    const SDK_INI_HAND *pHand:SDK_INI_CONTENTָ��
** Returned value:  С�ڲ����ڷ���SDK_INI_NO_SECTION
                    key�����ڷ���SDK_INI_NO_KEY
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    ����С�����ơ��ؼ��ֳ��ȳ����򷵻�SDK_PARA_ERR
                    ɾ���ɹ�����SDK_OK
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniRemoveSectionKey(const SDK_INI_HAND pHand, const u8 *pSection, const u8 *pKey) {
    struct _section *psection;
    struct _record *precord1, *precord2;
    int del_num = 0;

    if (NULL == pSection || NULL == pKey || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if ((sizeof(((struct _section *) 0)->name) < strlen(pSection)) ||
        (sizeof(((struct _record *) 0)->key) < strlen(pKey)))       //���С�����鶨�峤��
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = sdkIniGetSection((struct _content *) pHand, pSection);

    if (psection == NULL) {
        return SDK_INI_NO_SECTION;
    }
    precord1 = psection->first;

    if (precord1 == NULL) {
        return SDK_INI_NO_KEY;
    }

    if (strcmp(pKey, precord1->key) == 0) {
        psection->first = precord1->next;
        psection->sizeRecord--;
        sdkFreeMem(precord1);
        return SDK_OK;
    }

    while (precord1 != NULL) {
        if (precord1->next != NULL) {
            if (strcmp(pKey, precord1->next->key) == 0) {
                precord2 = precord1->next;
                precord1->next = precord1->next->next;
                psection->sizeRecord--;
                sdkFreeMem(precord2);
                del_num = SDK_OK;
                break;
            }
        }
        precord1 = precord1->next;
    }

    return del_num;
}

/*****************************************************************************
** Descriptions:	�����޸ĺ��ini�ļ�
** Parameters:      const u8* pFile:�ļ�·������
                    const SDK_INI_HAND *pHand:SDK_INI_CONTENTָ��
** Returned value:	�ļ�����ʧ�ܷ���:SDK_ERR
                    �������ΪNULL�򷵻�SDK_PARA_ERR
                    �ɹ�����:SDK_OK
** Created By:		shijianglong  2013.03.13
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniSave(const SDK_INI_HAND pHand, const u8 *pFile) {
    FILE *fp;
    struct _section *psection;
    struct _record *precord;

    if (NULL == pFile || NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = ((struct _content *) pHand)->first;

    if ((fp = fopen(pFile, "w")) == NULL) //lint !e64
    {
        Trace("ini", "open file error\n");
        return SDK_ERR;
    }

    while (psection != NULL) {
        if (strlen(psection->comments) != 0) {
            fprintf(fp, "%s\r\n", psection->comments);
        }
        fprintf(fp, "[%s]\r\n", psection->name);
        precord = psection->first;

        while (precord != NULL) {
            if (strlen(precord->comments) != 0) {
                fprintf(fp, "%s\r\n", precord->comments);
            }
            fprintf(fp, "%s=%s\r\n", precord->key, precord->value);

            precord = precord->next;
        }

        psection = psection->next;
    }

    Trace("test", "�ļ�%s�ĳ���=%d\r\n", pFile, ftell(fp));

    fclose(fp);
    return SDK_OK;
} //lint !e818

/*****************************************************************************
** Descriptions:	�ر�ini�ļ�����
** Parameters:      const SDK_INI_HAND pHand:�ļ�����ָ��
** Returned value:	�������󷵻�SDK_PARA_ERR
                    �ɹ�����SDK_OK
** Created By:		shijianglong  2013.03.14
** Modified By:		wangmingming  2014.05.26
** Remarks:
*****************************************************************************/
s32 sdkIniClose(const SDK_INI_HAND pHand) {
    struct _section *psection;
    struct _record *precord;
    struct _section *ptempsection;
    struct _record *ptemprecord;

    if (NULL == pHand) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    psection = ((struct _content *) pHand)->first; //fusuipu 2013.03.20 11:4

    while (psection != NULL) {
        precord = psection->first;

        while (precord != NULL) {
            ptemprecord = precord->next;
            sdkFreeMem(precord);
            precord = ptemprecord;
        }

        ptempsection = psection->next;
        sdkFreeMem(psection);
        psection = ptempsection;
    }

    sdkFreeMem((struct _content *) pHand);
    return SDK_OK;
}

