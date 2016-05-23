#include "sdkdevOption.h"
#include "sdkGlobal.h"
#include "sdkPrivateHead.h"
#include "ctype.h"

#ifndef  DEBUG_TOOL
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


#define IP_SUBNUM   4   //IP��ַ����Ϊ4
#define IP_SUBLEN   3   //ÿ�ε���󳤶�

static u8 gTickFlag = 0; //�Ƿ��һ�λ�ȡϵͳʱ��
static u32 gFirstTick = 0; //��һ�λ�ȡϵͳʱ��ʱ��ϵͳʱ��


/*IP�ṹ��*/
typedef struct {
    u16 SubIdx;                                              //������
    u16 Sublen[IP_SUBNUM];                                   //���ÿ�εĳ���
    char sSubIP[IP_SUBNUM][IP_SUBLEN + 1];                   //�Ķ�����
} T_IP;

/*=======BEGIN: zhouzhihua 2013.05.09  11:51 modify===========*/




/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Okolle Ltd,Co.
   ��������: ɾ���ַ�������ո�
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_TrimL(u8 *str) {
    u8 flag;
    u32 len, i, j;

    len = strlen(str);

    for (i = 0, j = 0, flag = false; i < len; i++) {
        if (str[i] != ' ') { flag = true; }

        if (flag) { str[j++] = str[i]; }
    }

    str[j] = 0;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Okolle Ltd,Co.
   ��������: ɾ���ַ������ҿո�
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_TrimR(u8 *str) {
    u32 len, i;

    len = strlen(str);

    for (i = len; i > 0; i--) {
        if (str[i - 1] != ' ') {
            break;
        }
    }

    str[i] = 0;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Okolle Ltd,Co.
   ��������: ɾ���ַ������߿ո�
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_Trim(u8 *str) {
    sdk_TrimL(str);
    sdk_TrimR(str);
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Xinguodu.
   ��������: ɾ���ַ�������ո��'['
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_IniTrimBracketL(u8 *str) {
    u8 flag;
    u32 len, i, j;

    len = strlen(str);

    for (i = 0, j = 0, flag = false; i < len; i++) {
        if ((str[i] != ' ') && (str[i] != '[')) { flag = true; }

        if (flag) { str[j++] = str[i]; }
    }

    str[j] = 0;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Xinguodu.
   ��������: ɾ���ַ������ҿո��']'
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_IniTrimBracketR(u8 *str) {
    u32 len, i;

    len = strlen(str);

    for (i = len; i > 0; i--) {
        if ((str[i - 1] != ' ') && (str[i - 1] != ']')) {
            break;
        }
    }

    str[i] = 0;
}

/*******************************************************************
   ��    ��: ������
   ��    Ȩ: Xinguodu.
   ��������: ɾ���ַ������߿ո��'[',']'
   ��ڲ���: 1: �ַ���
   �� �� ֵ:
   ��    ע:
   �޸���Ϣ:
 ********************************************************************/
static void sdk_IniTrimBracket(u8 *str) {
    u8 *buf;
    u32 len;

    len = strlen(str);
    buf = (u8 *) sdkGetMem(len + 1);

    if (NULL == buf) {
        Assert(0);
        return;
    }
    memset(buf, 0, len + 1);

    memcpy(buf, str, len);

    sdk_IniTrimBracketL(buf);
    sdk_IniTrimBracketR(buf);
    memset(str, 0, len);
    str[0] = '[';
    strcpy(str + 1, buf);
    strcat(str, "]");

    sdkFreeMem(buf);
}

/*******************************************************************
   ��	  ��: huxiang
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ��ļ�һ�е��ַ���
   ��ڲ���: 1��stream �ļ��� 2��line һ�е��ַ��� 3��maxlength һ���ַ�������󳤶�
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
static s32 sdk_IniWriteLine(const FILE *stream, const u8 *line) {
    u8 *buf;
    s32 len, i, j, flag;

    len = strlen(line);
    buf = (u8 *) sdkGetMem(len + 1);

    if (NULL == buf) {
        Assert(0);
        return SDK_ERR;
    }
    memset(buf, 0, len + 1);

    for (i = 0, j = 0, flag = 0; i < len; i++) {
        if ((line[i] != 0x0A) && (line[i] != 0x0D)) { flag = 1; }

        if (flag) { buf[j++] = line[i]; }
    }

    if ((line[0] != 0x0d) && (line[0] != 0x0a) && (len != 0)) {
        fwrite(buf, strlen(buf), 1, stream);
        fwrite("\x0D\x0A", 2, 1, stream);                 //д������
    }
    sdkFreeMem(buf);
    return 0;
}

/*******************************************************************
   ��	  ��: huxiang
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ��ļ�һ�е��ַ���
   ��ڲ���: 1��stream �ļ��� 2��line һ�е��ַ��� 3��maxlength һ���ַ�������󳤶�
   �� �� ֵ: �ַ��ĸ���,�ļ���������-1��
   ��	  ע:
 ********************************************************************/
static s32 sdk_IniGetLine(const FILE *stream, u8 *line, s32 maxlength) {
    s32 size = 0, rslt = feof(stream);  //lint !e838
    u8 ch;

    memset(line, 0, maxlength);

    while (rslt == 0) {
        if (size < maxlength) {
            ch = (u8) fgetc(stream);
            rslt = feof(stream);

            if (ch == 0x0d || ch == 0x0a) {
                break;
            }
            else {
                line[size] = ch;
                size++;
            }
        }
    }

    if (rslt != 0) {
        return -1;
    }
    else {
        return size;
    }
}

/*******************************************************************
   ��	  ��: huxiang
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ�Ini�ļ���ָ���ֶε�ָ����ֵ
   ��ڲ���: 1��file �ļ� 2��section �ֶ����� 3��key �� 4��value �õ��ļ�ֵ
   �� �� ֵ: 1���ɹ� 0��ʧ��
   ��	  ע:
 ********************************************************************/
static u8 sdk_IniReadKey(const u8 *file, const u8 *section, const u8 *key, u8 *value) {
    FILE *fp;
    u8 *value_buf;
    u8 *line;
//    u8 file_buf[128];
//    u8 section_buf[128], value_tmp[128];
//    u8 key_buf[128];
    u8 *file_buf, *section_buf, *value_tmp, *key_buf;
    u8 i, j, rslt = 0;
    s32 flag = 0;

    line = (u8 *) sdkGetMem(1024);

    if (line == NULL) {
        Assert(0);
        return rslt;
    }
    file_buf = (u8 *) sdkGetMem(128);

    if (file_buf == NULL) {
        sdkFreeMem(line);
        Assert(0);
        return rslt;
    }
    section_buf = (u8 *) sdkGetMem(128);

    if (NULL == section_buf) {
        sdkFreeMem(line);
        sdkFreeMem(file_buf);
        Assert(0);
        return rslt;
    }
    value_tmp = (u8 *) sdkGetMem(128);

    if (NULL == value_tmp) {
        sdkFreeMem(line);
        sdkFreeMem(file_buf);
        sdkFreeMem(section_buf);
        Assert(0);
        //return SDK_ERR;
        return rslt;
    }
    key_buf = (u8 *) sdkGetMem(128);

    if (NULL == key_buf) {
        sdkFreeMem(line);
        sdkFreeMem(file_buf);
        sdkFreeMem(section_buf);
        sdkFreeMem(value_tmp);
        Assert(0);
        return rslt;
    }
    memset(line, 0, 1024);
    memset(value_tmp, 0, 128);
    memset(file_buf, 0, 128);
    memset(section_buf, 0, 128);
    memset(key_buf, 0, 128);

    memcpy(file_buf, file, strlen(file));
    sdk_Trim(file_buf);

    memcpy(section_buf, section, strlen(section));
    sdk_IniTrimBracket(section_buf);

    memcpy(key_buf, key, strlen(key));
    sdk_Trim(key_buf);

    fp = fopen(file_buf, "r");  //lint !e64

    if (fp != NULL) {
        while (sdk_IniGetLine(fp, line, 1024) != -1) {
            if (strstr(line, section_buf) != NULL) {
                flag = 1;                                         //�ҵ��ֶ�
                continue;
            }

            if ((strchr(line, '[') != NULL) && (flag == 1)) {
                break;
            }

            if ((strstr(line, key_buf) != NULL) && (flag == 1)) {
                value_buf = strchr(line, '=');               //�ҵ�key

                if (value_buf != NULL) {
                    u8 *tmp = (u8 *) sdkGetMem(value_buf - line + 1);

                    if (tmp == NULL) {
                        Assert(0);
                        return rslt;
                    }
                    memset(tmp, 0, value_buf - line + 1);
                    memcpy(tmp, line, value_buf - line);
                    sdk_Trim(tmp);

                    if (strcmp(tmp, key_buf) == 0) {
                        memset(value_tmp, 0, 128);
                        strcpy(value_tmp, value_buf + 1);
                        rslt = 1;
                        break;
                    }
                    sdkFreeMem(tmp);
                }
            }
        }

        if (rslt == 1) {
            for (i = 0, j = 0; i < strlen(value_tmp); i++) {
                if (!(isspace(value_tmp[i]))) {
                    value[j] = value_tmp[i];
                    j++;
                }
            }

            value[j] = 0;
        }
        fclose(fp);
    }

    if (line != NULL) {
        sdkFreeMem(line);
//        line = NULL;
    }

    if (file_buf != NULL) { sdkFreeMem(file_buf); }

    if (section_buf != NULL) { sdkFreeMem(section_buf); }

    if (value_tmp != NULL) { sdkFreeMem(value_tmp); }

    if (key_buf != NULL) { sdkFreeMem(key_buf); }
    return rslt;
}

/*******************************************************************
   ��	  ��: huxiang
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: дINI�ļ����ƶ��ֶε�ָ����ֵ
   ��ڲ���: 1��file �ļ� 2��section �ֶ����� 3��key �� 4��value �õ��ļ�ֵ
   �� �� ֵ: 1���ɹ� 0��ʧ��
   ��	  ע:
 ********************************************************************/
static s32 sdk_IniWriteKey(const u8 *file, const u8 *section, const u8 *key, const u8 *value) {
    FILE *fp = NULL, *fp_bak = NULL;
    u8 *line = NULL;
    u8 rslt = 0;
//    u8 file_buf[128], file_bak[128];
//    u8 section_buf[128];
//    u8 key_buf[128];
//    u8 value_buf[128];
    u8 *file_buf = NULL, *file_bak = NULL, *section_buf = NULL, *key_buf = NULL, *value_buf = NULL;
    s32 flag = 0, flag_bak = 0;

    line = (u8 *) sdkGetMem(1024);

    if (NULL == line) {
        Assert(0);
        return SDK_ERR;
    }
    file_buf = (u8 *) sdkGetMem(128);

    if (file_buf == NULL) {
        Assert(0);
        goto end;
    }
    file_bak = (u8 *) sdkGetMem(128);

    if (file_bak == NULL) {
        Assert(0);
        goto end;
    }
    section_buf = (u8 *) sdkGetMem(128);

    if (section_buf == NULL) {
        Assert(0);
        goto end;
    }
    key_buf = (u8 *) sdkGetMem(128);

    if (key_buf == NULL) {
        Assert(0);
        goto end;
    }
    value_buf = (u8 *) sdkGetMem(128);

    if (value_buf == NULL) {
        Assert(0);
        goto end;
    }
    memset(line, 0, 1024);
    memset(file_bak, 0, 128);
    memset(file_buf, 0, 128);
    memset(section_buf, 0, 128);
    memset(key_buf, 0, 128);
    memset(value_buf, 0, 128);

    memcpy(file_buf, file, strlen(file));
    sdk_Trim(file_buf);
    memcpy(section_buf, section, strlen(section));
    sdk_IniTrimBracket(section_buf);
    memcpy(key_buf, key, strlen(key));
    sdk_Trim(key_buf);
    memcpy(value_buf, value, strlen(value));
    sdk_Trim(value_buf);

    fp = fopen(file_buf, "r+");     //lint !e64

    if (fp == NULL) {
//        if( NULL != line )
//        {
//            xgd_free( line );
//            line = NULL;
//        }
//        return SDK_PARA_ERR;
        perror("fopen");
        goto end;
        //fclose(fp);  20110608
    }
    else {
        strcpy(file_bak, file_buf);
        strcat(file_bak, ".bak");
        fp_bak = fopen(file_bak, "w+"); //lint !e64

        if (fp_bak == NULL) {
//            if( NULL != line )
//            {
//                free( line );
//                line = NULL;
//            }
//            return SDK_PARA_ERR;
            perror("fopen");
            goto end;
            //fclose(fp);  20110608
        }

        while (sdk_IniGetLine(fp, line, 1024) != -1) {
            if (strstr(line, section_buf) != NULL) {
                flag = 1;
                sdk_IniWriteLine(fp_bak, line);
                continue;
            }

            if ((strchr(line, '[') != NULL) && (flag == 1) && (flag_bak == 0)) {
                strcat(key_buf, "=");
                strcat(key_buf, value_buf);
                sdk_IniWriteLine(fp_bak, key_buf);
                flag_bak = 1;
                rslt = 1;
            }

            if ((strstr(line, key_buf) != NULL) && (flag == 1) && (flag_bak == 0)) {
                strcat(key_buf, "=");
                strcat(key_buf, value_buf);
                sdk_IniWriteLine(fp_bak, key_buf);
                rslt = 1;
                flag_bak = 1;
                continue;
            }
            sdk_IniWriteLine(fp_bak, line);
        } //end while

        if (rslt == 0) {
            if (flag == 0)                           //û���ҵ��ֶ���
            {
                sdk_IniWriteLine(fp_bak, section_buf);
                strcat(key_buf, "=");
                strcat(key_buf, value_buf);
                sdk_IniWriteLine(fp_bak, key_buf);
                rslt = 1;
            }
            else {
                strcat(key_buf, "=");
                strcat(key_buf, value_buf);
                sdk_IniWriteLine(fp_bak, key_buf);
                rslt = 1;
            }
        }

        if (rslt == 1) {
            memset(line, 0, 1024);
            strcpy(line, "mv -f ");
            strcat(line, file_bak);
            strcat(line, " ");
            strcat(line, file_buf);
            system(line);
        }
//        fclose(fp);
//        fclose(fp_bak);
    }
    end:

    if (fp) { fclose(fp); }

    if (fp_bak) { fclose(fp_bak); }

    if (line) { sdkFreeMem(line); }

    if (file_buf) { sdkFreeMem(file_buf); }

    if (file_bak) { sdkFreeMem(file_bak); }

    if (section_buf) { sdkFreeMem(section_buf); }

    if (key_buf) { sdkFreeMem(key_buf); }

    if (value_buf) { sdkFreeMem(value_buf); }
    return rslt;
}

/*====================== END======================== */

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: void sdkmSleep(const s32 siMs)
   ��������: ֧�ֶ��߳���ʱ,��λ����
   �������: const s32 siMs:������
   �������: ��
   ��   ��  ֵ: ��
   �޸ı�ע:
   ����ʱ��: 2012.05.15 15:38:19
*******************************************************************/
void sdkmSleep(const s32 siMs) {
//    OS_ERR tem;//sujianzhong 2015.05.20 17:24
//
//    OSTimeDly(siMs, 0, &tem);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkGetRandom(u8 *pheRdm , s32 siNum)
   ��������: u8 *pheRdm:�õ�num�������
   �������: s32 siNum:����
   �������: ���������
   ��   ��  ֵ: ����ָ��Ϊ���򷵻�SDK_PARA_ERR
                          �ɹ��򷵻�SDK_OK
   �޸ı�ע:
   ����ʱ��: 2012.05.15 16:40:21
*******************************************************************/
s32 sdkGetRandom(u8 *pheRdm, s32 siNum) {
    s32 i;
    u8 dateTime[8];
    u32 sec;

    if (pheRdm == NULL || siNum <= 0) {
        Assert(0); //ʯ���� 2012.08.20 15:24
        return SDK_PARA_ERR;
    }

    for (i = 0; i < siNum; i++) {
        memset(dateTime, 0, 8);
        ddi_sys_get_time(dateTime);

        ddi_sys_get_tick(&sec);
        sec += (dateTime[5] >> 4) * 10 + (dateTime[5] & 0x0F); //��ȡ��ǰ����

        srand(sec + i); //�������̫����,δ�����
        pheRdm[i] = rand();
    }

    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: bool sdkDf(const u8 *pasDiskCata,s32 *psiAvailSize,s32 *psiUsedSize,s32 *psiUsedRate)
   ��������: ��ȡָ��Ŀ¼���̿��ÿռ��С�����ÿռ�ռ�ܿռ�ı���
   �������:const u8 *pasDiskCata: ����Ŀ¼
   �������:  s32 *psiAvailSize :���ô�����������λΪk
                         s32 *psiUsedSize:���ô�����������λΪk
                         s32 *psiUsedRate:�������ðٷֱȣ����緵��50�����ʾ50%
   ��   ��  ֵ: �������󷵻�false
                      ��ȷ����true
   �޸ı�ע:
   ����ʱ��: 2012.05.15 19:03:17
*******************************************************************/
bool sdkDf(const u8 *pasDiskCata, s32 *psiAvailSize, s32 *psiUsedSize, s32 *psiUsedRate) {
    if (NULL == pasDiskCata || NULL == psiAvailSize || NULL == psiUsedSize || NULL == psiUsedRate) {
        Assert(0);
        return false;
    }
    return true; //sdk_dev_get_df(pasDiskCata, psiAvailSize, psiUsedSize, psiUsedRate);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkReadPosSn(u8 *pasDest)
   ��������: �������,��󲻳���16λ
   �������: ��
   �������: u8 *pasDest:�����
   ��   ��  ֵ: ���ָ��Ϊ���򷵻�SDK_PARA_ERR
                        �ɹ��򷵻ض������ȡ�
   �޸ı�ע:
   ����ʱ��: 2012.05.16 14:54:23
*******************************************************************/
s32 sdkReadPosSn(u8 *pasDest) {
    if (pasDest == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (DDI_OK == ddi_sys_read_dsn(pasDest)) {
        return strlen(pasDest);
    }
    else {
        return 0;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkGetRtc(u8 *pbcDest)
   ��������: ��ϵͳʱ��
   ��ʽΪ��/��/��/ʱ/��/�� ��6Byte��
   �������: ��
   �������:u8 *pbcDest: ���ʱ��(BCD)
   ��   ��  ֵ:��������Ϊ���򷵻�SDK_PARA_ERR
                        �ɹ��򷵻�SDK_OK
   �޸ı�ע:  RTC��ķ�Χ��1970��2037
                   ʵ��Ҫ��Χ1950��2049,����Ҫ����.
                   1970~1989 - 20 ����1950-1969
                   2010~2019 + 20 ����2030-2049
   ����ʱ��: 2012.05.15 17:57:07
*******************************************************************/
s32 sdkGetRtc(u8 *pbcDest) {
    if (NULL == pbcDest) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ddi_sys_get_time(pbcDest);
    if (sdkIsBcdNum(pbcDest, 6)) {
        return SDK_OK;
    }
    else {
        Assert(0);
        return SDK_PARA_ERR;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkSetRtc(const u8 *pbcSrc)
   ��������: ����ϵͳʱ��,���ò���Ϊѹ��BCD��
   ��ʽΪ��/��/��/ʱ/��/�� ��6Byte��
   �������: const u8 *pbcSrc:����ʱ��(BCD)
   �������: ��
   ��   ��  ֵ:����ָ��Ϊ���򷵻�SDK_PARA_ERR
                        ����ʱ���ʽ����ȷ�򷵻�SDK_ERR
                        �ɹ��򷵻�SDK_OK
   �޸ı�ע:  RTC��ķ�Χ��1970��2037
                   ʵ��Ҫ��Χ1950��2049,����Ҫ����.
                   1970~1989 - 20 ����1950-1969
                   2010~2019 + 20 ����2030-2049
   ����ʱ��: 2012.05.15 17:57:07
*******************************************************************/
s32 sdkSetRtc(const u8 *pbcSrc) {
//    u8 temp[8] = {0};
//    u8 i = 0;

    if (NULL == pbcSrc) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkJudgeTime(pbcSrc, 6) > 0) {
        if (ddi_sys_set_time(pbcSrc) == DDI_OK) {
            return SDK_OK;
        }
        else {
            return SDK_ERR;
        }
    }
    else {
        return SDK_ERR;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	sdkTimerGetId()
   ��������:	�õ�һ����ʱ��
   �������:     ��
   ��   ��   ֵ:     һ��ʱ��ڵ�
   ����ʱ��:2012.07.02 14:15:23
*******************************************************************/
u32 sdkTimerGetId(void) {
    u32 sys_tick;
//zhouxiaoxin
//    /*��¼��һ����ʱ��tickֵ*/
//    if (gTickFlag == 0) {
//        gTickFlag = 1;
//        ddi_sys_get_tick(&gFirstTick);
//    }
//
//    if (DDI_OK == ddi_sys_get_tick(&sys_tick)) {
//        return sys_tick;
//    }
//    else {
//        Assert(0);
//        return 0;
//    }
    sys_tick = 0;
    ddi_sys_get_tick(&sys_tick);
    return sys_tick;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��:ʯ����
   ��������:	bool sdkTimerIsEnd(u32 siId, u32 siMs)
   ��������:	 ��ⶨʱ���Ƿ�ʱ����
   �������:      siId:��ʱ���ڵ�;siMs:��ʱʱ��(��λ:����,����0.1ms)
   ����ʱ��:2012.07.02 14:18:28
*******************************************************************/
bool sdkTimerIsEnd(u32 uiId, u32 uiMs) {
    //zhouxiaoxin
//    u32 sys_tick;
//    if (uiMs == 0) { return false; }
//
//    if (DDI_OK == ddi_sys_get_tick(&sys_tick)) {
//        /*���ȵ�һ�λ�ȡ��tickֵС˵��Խ����*/
//        if (sys_tick < gFirstTick) {
//            gFirstTick = sys_tick;
//
//            if (uiId >= gFirstTick) //��ȡtimerIdʱ��û��Խ��
//            {
//                if (0xffffffff - uiId + sys_tick >= uiMs) {
//                    return true;
//                }
//                else {
//                    return false;
//                }
//            }
//            /*��ȡtimerIdʱ��Խ���˵Ļ���������������*/
//        }
//
//        if (sys_tick - uiId >= uiMs) {
//            return true;
//        }
//        else {
//            return false;
//        }
//    }
//    else {
//        Assert(0);
//        return false;
//    }
    u32 lCurid = 0;
    lCurid = sdkTimerGetId();

    if(lCurid < uiId)
    {
        return true;
    }
    lCurid -= uiMs;

    if(lCurid < uiId)
    {
        return false;
    }
    else
    {
        return true;
    }

}

/*****************************************************************************
** Descriptions:	�õ�ʱ����ַ���
** Parameters:          void
** Returned value:
** Created By:		shiweisong  2013.10.29
** Remarks:
*****************************************************************************/
const u8 *sdkTimerGetAsc(void) {
    Assert(0);
    return NULL;
}

/*****************************************************************************
** Descriptions:	        ���IP�Σ�����Ϊ0���ߴ���255
** Parameters:          T_IP�ṹ��
** Returned value:	false:���Ϸ�����Ҫ�Ķ� true:�Ϸ�
** Created By:		����Ȫ  2012.09.07
** Remarks:
*****************************************************************************/
static bool sdkCheckIPSectionOK(T_IP *pstIp) {
    s32 i = 0;

    if (pstIp == NULL) {
        Trace("libsdktool", "pointer is null, return\r\n");
        return false;
    }

    for (i = 0; i < IP_SUBNUM; i++) {
        Trace("libsdktool", "subidex=%d subip=%s len = %d\r\n", i, pstIp->sSubIP[i],
              pstIp->Sublen[i]);
/*=======BEGIN: tjb 2014.06.19  9:0 modify===========*/
#if 0

        if ((pstIp->Sublen[i] > 3) ||                                           //IP���ȴ���3
            (3 == pstIp->Sublen[i] && strcmp(pstIp->sSubIP[i], "255") > 0) ||   //IP�δ���256
            pstIp->Sublen[i] == 0)                                              //IP��Ϊ0
#else

        if ((pstIp->Sublen[i] > 3) ||                                         //IP���ȴ���3
            (3 == pstIp->Sublen[i] && strcmp(pstIp->sSubIP[i], "255") > 0))   //IP�δ���256
            //IP��Ӧ�ÿ���Ϊ0������sublen[i]Ϊ�ַ���������� "==0"
#endif
/*====================== END======================== */
        {
            return false;
        }
    }

    return true;
}

static s32 sdkIpInputKeyDownDeal(T_IP *pstIp) {
    int len;

//    bool ret = true;

    if (pstIp == NULL) {
        Trace("CheckIPSectionOK", "pointer is null, return\r\n");
        return SDK_PARA_ERR;
    }
    Trace("CheckIPSectionOK", "subidex=%d subip=%s len = %d\r\n", pstIp->SubIdx,
          pstIp->sSubIP[pstIp->SubIdx], pstIp->Sublen[pstIp->SubIdx]);
    len = pstIp->Sublen[pstIp->SubIdx];

    if (len == 0) {
        pstIp->sSubIP[pstIp->SubIdx][pstIp->Sublen[pstIp->SubIdx]++] = '0';
    }
    else if (len == 3) {
        /*IP���ܳ���255*/
        if (strcmp(pstIp->sSubIP[pstIp->SubIdx], "255") > 0) {
            strcpy(pstIp->sSubIP[pstIp->SubIdx], "255");
        }
    }

    /*������˵��ĶΣ������л�*/
    if (pstIp->SubIdx < IP_SUBNUM - 1) {
        pstIp->SubIdx++;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        IP���봦����������˸��
** Parameters:          T_IP *pstIp
** Returned value:
** Created By:		fusuipu  2014.04.07
** Remarks:
*****************************************************************************/
static s32 sdkIpInputBackSpKeyDeal(T_IP *pstIp) {
    Trace("Util_InputIp", "DEL:subidex=%d subip=%s len = %d\r\n", pstIp->SubIdx,
          pstIp->sSubIP[pstIp->SubIdx], pstIp->Sublen[pstIp->SubIdx]);

    /*���һ�εĳ���Ϊ0����ǰ����ǰһ��*/
    if (pstIp->Sublen[pstIp->SubIdx] == 0) {
        if (pstIp->SubIdx > 0) {
            pstIp->SubIdx--;
        }
    }

    /*���һ�ε����ֳ��ȴ���0��ɾ��һλ*/
    if (pstIp->Sublen[pstIp->SubIdx] > 0) {
        pstIp->sSubIP[pstIp->SubIdx][--pstIp->Sublen[pstIp->SubIdx]] = '\0';
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	    IP���봦�����ּ�
** Parameters:          T_IP * pstIp
** Returned value:
** Created By:		fusuipu  2014.04.07
** Remarks:
*****************************************************************************/
static s32 sdkIpInputNumKeyDeal(T_IP *pstIp, s32 siKey) {
    if ((siKey < '0') || (siKey > '9') || (pstIp->SubIdx > 3)) {
        return SDK_ERR;
    }

    /*��һ��IP�����������֣����л�����һ��*/
    if (pstIp->Sublen[pstIp->SubIdx] == IP_SUBLEN) {
        /*������˵��ĶΣ������л�*/
        if (pstIp->SubIdx < IP_SUBNUM - 1) {
            pstIp->SubIdx++;
        }
    }

    /*���һ��С���������֣�����Լ�������*/
    if (pstIp->Sublen[pstIp->SubIdx] < IP_SUBLEN) {
        pstIp->sSubIP[pstIp->SubIdx][pstIp->Sublen[pstIp->SubIdx]++] = siKey;
    }

    if (pstIp->Sublen[pstIp->SubIdx] == 3) {
        /*IP���ܳ���255*/
        if (strcmp(pstIp->sSubIP[pstIp->SubIdx], "255") > 0) {
            strcpy(pstIp->sSubIP[pstIp->SubIdx], "255");
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ʾ�м���̵�IP����
** Parameters:          T_IP *pstIp
                               u8 uAlign
** Returned value:
** Created By:		fusuipu  2014.04.07
** Remarks:
*****************************************************************************/
static s32 sdkDispIp(T_IP *pstIp, s32 siInputLine, u8 uAlign) {
#define IP_FMT  "%3s.%3s.%3s.%3s"
    static u8 tempOldIP[32] = {0};
    u8 i = 0;
    static u8 is_add_arrow = 0;     //  �Ƿ������˸��ͷ
    static bool flag = true;
    static s32 flash_time_id = 0;
    u8 temp_ip_buf[4][5] = {0};
    u8 temp[32] = {0};
    u8 disp_buf[3] = {SDK_DISP_FDISP | SDK_DISP_LDISP | SDK_DISP_INCOL,
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL,
                      SDK_DISP_FDISP | SDK_DISP_RDISP | SDK_DISP_INCOL};

    u8 place = 0;

    memset(temp_ip_buf, 0, sizeof(temp_ip_buf));

//  ��һ�λ�ȡʱ��ID,��ʱ��ʼ
    if (true == flag) {
        flash_time_id = sdkTimerGetId();
        flag = false;
    }

//ʹ����ʱ�ַ����洢IP��
    for (i = 0; i < 4; i++) {
        strcpy(temp_ip_buf[i], pstIp->sSubIP[i]);
    }

//�жϳ�ʱ���л���ͷ��˸״̬
    if (sdkTimerIsEnd(flash_time_id, 800)) {
        flash_time_id = sdkTimerGetId();
        is_add_arrow = (is_add_arrow + 1) % 2;
    }

//��ȡ��ͷ��˸�ĵص�
    if (pstIp->Sublen[pstIp->SubIdx] < IP_SUBLEN) {
        place = pstIp->SubIdx;
    }
    else {
        place = pstIp->SubIdx + 1;
    }

//��Ӽ�ͷ��˸
    if (is_add_arrow == 1) {
        strcat(temp_ip_buf[place], " "); //1.5S
    }
    else {
        strcat(temp_ip_buf[place], "|"); //1S
    }
//����֯�õ�IP�ַ�����ʾ
    sprintf((char *) temp, IP_FMT, temp_ip_buf[0], temp_ip_buf[1], temp_ip_buf[2], temp_ip_buf[3]);
    Trace("zjp", "tempOldIP = %s\r\n", tempOldIP);
    Trace("zjp", "temp = %s\r\n", temp);

    if (strcmp(tempOldIP, temp) != 0) {
        sdkDispRow(siInputLine, 0, temp, disp_buf[uAlign]);
        strcpy(tempOldIP, temp);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����IP��ַ
** Parameters:      uiDsipLine        ��ʾ��Ϣ��ʾ����
                    pAsPrompt         ��ʾ��Ϣ
                    uiInputLine       ����������ʾ����
                    pAsIP             ����IP��ַ
                    uAlign            ��ʾλ�ã�0����룻1���У�2���Ҷ���
                    uiTimeOut         ��ʱʱ�䣨�룩
** Returned value:  SDK_PARA_ERR      ��������
                    SDK_TIME_OUT      ��ʱ
                    SDK_ESC           ȡ��
                    SDK_FAIL          ʧ��
                    SDK_OK            �ɹ�
** Created By:		����Ȫ  2012.08.17
** Remarks:
*****************************************************************************/
s32 sdkInputIp(s32 siDsipLine, const u8 *pAsPrompt, s32 siInputLine, u8 *pAsIP, u8 uAlign,
               s32 siTimeOut) {
    u32 timerID;
    int ret = 0;
    u8 keyValue = 0;
    T_IP stIP = {0};

//    s32 siflash_time_id = 0;

    /*�жϲ����Ƿ�Ƿ�*/
    if (siDsipLine < SDK_DISP_LINE1 || siDsipLine > SDK_DISP_LINE5 ||
        siInputLine < SDK_DISP_LINE1 ||
        siInputLine > SDK_DISP_LINE5 || pAsIP == NULL || uAlign > 2 || siTimeOut < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    /*��ʾ��Ϣ*/
    if (pAsPrompt) {
        sdkDispRow((u32) siDsipLine, 0, (u8 *) pAsPrompt,
                   SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    }
    timerID = sdkTimerGetId();
    memset(&stIP, 0, sizeof(T_IP));         //fusuipu 2014.04.07 10:51

    while (1) {
        Trace("Util_InputIp", "brush ip1=%s ip2=%s ip3=%s ip4=%s\r\n", stIP.sSubIP[0],
              stIP.sSubIP[1], stIP.sSubIP[2], stIP.sSubIP[3]);
        sdkDispIp(&stIP, siInputLine, uAlign);

        if (0 != siTimeOut && sdkTimerIsEnd(timerID, (u32) siTimeOut)) //fusuipu 2014.04.07 15:49
        {
            return SDK_TIME_OUT;
        }
        Verify((ret = sdkKbGetKey()) >= 0);
        keyValue = (u8) ret;

        if (keyValue > 0) {
            timerID = sdkTimerGetId();
            sdkSysBeep(SDK_SYS_BEEP_OK);         //fusuipu add at 2013-2-18
        }

        switch (keyValue) {
            case SDK_KEY_ESC:
                return SDK_ESC;

            case SDK_KEY_ENTER:

                if (true == sdkCheckIPSectionOK(&stIP)) {
                    sprintf(pAsIP, "%s.%s.%s.%s", stIP.sSubIP[0], stIP.sSubIP[1], stIP.sSubIP[2],
                            stIP.sSubIP[3]);
                    return SDK_OK;         //strlen(ip);
                }
                else {
                    return SDK_ERR;
                }
            case SDK_KEY_DOWN:

                if ((ret = sdkIpInputKeyDownDeal(&stIP)) != SDK_OK) {
                    return ret;
                }
                break;

            case SDK_KEY_CLEAR:
            case SDK_KEY_BACKSPACE:
                sdkIpInputBackSpKeyDeal(&stIP);
                break;

            default:
                sdkIpInputNumKeyDeal(&stIP, keyValue);
                break;
        }

        /*���һ��С���������֣�����Լ�������*/
    }

//    return SDK_ERR;         //һ�㲻�ᵽ�������������������϶��ǳ�����
}

/*******************************************************************
   ��   ��: ����
   ��   Ȩ: �����¹����ɷ����޹�˾
   ��������: �ж�ʱ��
   ��ڲ���: pTimeʱ��yymmddhhmmss BCD��ָ��len��Ҫ�жϵĳ��ȴ��꿪ʼ
   �� �� ֵ: false:��ʽ����, true:��ʽ��ȷ
   ��   ע: //zhouzhihua 2013.05.06 16:9
 ********************************************************************/
bool sdk_JudgeTime(const u8 *pTime, u8 len) {
    u8 days;
    u16 i;
    bool leepyear;

    sdkBcdToU16(&i, pTime, 1);                                                  //ȡ��

    if (i < 50) { i += 2000; }
    else { i += 1900; }

    if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0)) {
        leepyear = true;
    }
    else {
        leepyear = false;
    }

    if (len <= 1) { return true; }
    sdkBcdToU16(&i, pTime + 1, 1);                              //ȡ�·�

    if (i > 12 || i == 0) {
        return false;
    }

    if (len <= 2) { return true; }

    if ((i == 1) || (i == 3) || (i == 5) || (i == 7) || (i == 8) || (i == 10) || (i == 12)) {
        days = 31;                                                                              //����31��
    }
    else if (i != 2) {
        days = 30;                                                                              //����30��
    }
    else if (leepyear) {
        days = 29;                                                                              //����29��
    }
    else {
        days = 28;                                                                              //����28��
    }
    sdkBcdToU16(&i, pTime + 2, 1);                                      //ȡ����

    if (i > days || i == 0) {
        return false;
    }

    if (len <= 3) { return true; }
    sdkBcdToU16(&i, pTime + 3, 1);                                      //ȡСʱ

    if (i > 23) { return false; }

    if (len <= 4) { return true; }
    sdkBcdToU16(&i, pTime + 4, 1);                                      //ȡ����

    if (i > 59) {
        return false;
    }

    if (len <= 5) { return true; }
    sdkBcdToU16(&i, pTime + 5, 1);                                      //ȡ��

    if (i > 59) {
        return false;
    }
    return true;
}

/*****************************�����ӿں�������͸��****************************/

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: bool sdkJudgeTime(const u8 *pbcTime, u8 bclen)
   ��������: �ж�ʱ���Ƿ���ϸ�ʽ
   �������: pbcTime:���ʱ�䣬��ʽ��yymmddhhmmss����
   ������ʱ���룬��ռһ���ֽڣ���Ϊѹ��BCD��.
   ����2010��12��2��13ʱ50��25��Ϊ��0x10 0x12 0x02 0x12 050x 0x25
                       bclen:pbcTime���ֽڸ�������Χ1~6������1��6��.
   ��ͬ���ȶ�Ӧ��pbcTime���ݲ�ͬ.�磺bclenֵΪ3����pbcTime����
   Ϊ�������գ����bclenΪ6����pbcTimeֵΪ��������ʱ����.
      1���� 2���� 3���� 4��ʱ 5���� 6����
   �������: ��
   ��   ��  ֵ: ��ʽ��ȷ����true��
                       ��ʽ������ߴ���ָ��Ϊ�շ���false.
   �޸ı�ע:�ж�pbcTimeʱ���Ƿ���ȷ. Ҫע��bclen���Ȳ�ͬ��
   pbcTime�������ݵĺ����ǲ�ͬ�ģ�Ҫ�жϵ�����Ҳ�ǲ�
   ͬ��.�����жϵ����ݰ������µķ�Χ��ʱ����ķ�Χ.
   ���磺�����2����28�죬����������ֵ���򷵻�false.
   ����ʱ��: 2012.05.15 17:00:05
*******************************************************************/
bool sdkJudgeTime(const u8 *pbcTime, s32 siBcdLen) {
    if (NULL == pbcTime) {
        Assert(0);
        return false;
    }
    TraceHex("judgetime", "judgetime time", pbcTime, siBcdLen);

    if (1 <= siBcdLen && siBcdLen <= 6) {
        return sdk_JudgeTime((u8 *) pbcTime, (u8) siBcdLen);
    }
    else {
        return false;
    }
}

bool Private_sdkJudgeChn(const u8 heC1, const u8 heC2, const u8 heC3, const u8 heC4); //��������
/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: bool sdkIsVisibleStr(u8 const *pasSrc , u32 siSrclen)
   ��������: �ж��Ƿ�ɼ��ַ�(���ֻ���ascii��)
   �������:u8 const *pasSrc:�������ݣ�u32 siSrclen:�������ݳ���
   �������:��
   ��   ��  ֵ:�ǿɼ��ַ��򷵻�true
                      �в��ɼ��ַ����ߴ�������Ϊ���򷵻�false��
   �޸ı�ע:
   ����ʱ��: 2012.05.20 16:37:08
*******************************************************************/
bool sdkIsVisibleStr(u8 const *pasSrc, s32 siSrclen) {
    u32 j;

    if (NULL == pasSrc || siSrclen < 0) {
        Assert(0);
        return false;
    }

    for (j = 0; j < (u32) siSrclen; j++) {
        if (Private_sdkJudgeChn(pasSrc[j], pasSrc[j + 1], 0, 0) == true)      //�ж��Ƿ�Ϊ����
        {
            if ((j + 1) == (u32) siSrclen) //shijianglong 2013.06.07 11:18
            {
                return false;
            }
            else {
                j++;
            }
        }
//        else if(((0xA1 <= pasSrc[j] && pasSrc[j] <= 0xA9) && (0xA1 <= pasSrc[j + 1] && pasSrc[j + 1] <= 0xFE))
//                || ((0xA8 <= pasSrc[j] && pasSrc[j] <= 0xA9) && ((0x40 <= pasSrc[j + 1] && pasSrc[j + 1] <= 0x7E) || (0x80 <= pasSrc[j + 1] && pasSrc[j + 1] <= 0xFE)))) //�ж��Ƿ�Ϊ���ַ���
//        {
//            j++;
//        }
        else {
            if (pasSrc[j] > 127) {
                return false;
            }
        }
    }

    return true;    //lint !e850
}

/*****************************************************************************
** Descriptions:	        �ж��Ƿ�Ϊ����(֧��GB18030��׼�����ֿ���Ϊ�ĸ��ֽ�)
** Parameters:          const u8 c1:���ֵĵ�һ���ֽ�
                               const u8 c2:���ֵĵڶ����ֽ�
                               const u8 c3:���ֵĵ������ֽ�
                               const u8 c4:���ֵĵ��ĸ��ֽ�
** Returned value:	�Ǻ��ַ���true�����Ǻ��ַ���false
** Created By:		shijianglong  2012.09.27
** Remarks:             xgd GB18030�ֿ�����ʱֻ֧�������ֽڣ�heC3��heC4Ϊ0���ɡ�
*****************************************************************************/
bool Private_sdkJudgeChn(const u8 heC1, const u8 heC2, const u8 heC3, const u8 heC4) {
    if (heC3 == 0 || heC4 == 0)                  //�����ֽڵĺ���
    {
        if (heC2 == 0x7F)                //�ڶ����ֽ�Ϊ7Fʱ��Ϊ����
        {
            return false;
        }

        if ((0xB0 <= heC1 && heC1 <= 0xF7) && (0xA1 <= heC2 && heC2 <= 0xFE))     //������˫�ֽ�2��
        {
            return true;
        }
        else if ((0x81 <= heC1 && heC1 <= 0xA0) &&
                 ((0x40 <= heC2 && heC2 <= 0x7E) || (0x80 <= heC2 && heC2 <= 0xFE))) //������˫�ֽ�3��
        {
            return true;
        }
        else if ((0xAA <= heC1 && heC1 <= 0xFE) &&
                 ((0x40 <= heC2 && heC2 <= 0x7E) || (0x80 <= heC2 && heC2 <= 0xA0))) //������˫�ֽ�4��
        {
            return true;
        }
        else if (((0xA1 <= heC1 && heC1 <= 0xA9) &&
                  (0xA1 <= heC2 && heC2 <= 0xFE)) //shijianglong 2013.05.22 11:1
                 || ((0xA8 <= heC1 && heC1 <= 0xA9) && ((0x40 <= heC2 && heC2 <= 0x7E) ||
                                                        (0x80 <= heC2 &&
                                                         heC2 <= 0xFE)))) //������˫�ֽ�4��
        {
            return true;
        }
        else {
            return false;
        }
    }
    else                //�ĸ��ֽڵĺ���
    {
        if ((0x81 <= heC1 && heC1 <= 0xFE) && (0x30 <= heC2 && heC2 <= 0x39) &&
            (0x81 <= heC3 && heC3 <= 0xFE) && (0x30 <= heC4 && heC4 <= 0x39)) {
            return true;
        }
        else {
            return false;
        }
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:bool sdkIsBcdNum(u8 const *pheSrc, u32 siSrclen)
   ��������:�ж��Ƿ�ΪBCD(0-9)
   �������:u8 const *pheSrc:�������ݣ�u32 siSrclen:�������ݳ���
   �������:��
   ��   ��  ֵ:��BCD(0-9)�򷵻�true
                          ���ǻ��ߴ���ָ��Ϊ���򷵻�false��
   �޸ı�ע:
   ����ʱ��: 2012.05.20 16:10:06
*******************************************************************/
bool sdkIsBcdNum(u8 const *pheSrc, s32 siSrclen) {
    if (NULL == pheSrc || siSrclen < 0) {
        Assert(0);
        return false;
    }

    while (siSrclen--) {
        if (((pheSrc[siSrclen] & 0x0F) > 0x09) || ((pheSrc[siSrclen] & 0xF0) > 0x90)) {
            return false;
        }
    }

    return true;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkIniReadKey(const u8 *pasFile, const u8 *pasSection ,const u8 *pasKey, u8 *pheValue)
   ��������: ��INI�ļ����ƶ��ֶε�ָ����ֵ
   �������: pasFile �ļ�;pSasection �ֶ����� ;pasKey �� ;
   �������: pheValue �õ��ļ�ֵ
   ��   ��  ֵ: ���ɹ��򷵻�SDK_OK
                       ��ʧ���򷵻�SDK_ERR��
                        ���봫��ָ��Ϊ���򷵻�SDK_PARA_ERR
   �޸ı�ע:��ȡ���ƶ��ļ���С��󲻳���4k�������ȡʧ��
   ����ʱ��: 2012.05.16 15:38:39
*******************************************************************/
s32 sdkIniReadKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey, u8 *pheValue) {
    u8 ret;

    if (NULL == pasFile || NULL == pasKey || NULL == pasSection || NULL == pheValue) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = sdk_IniReadKey((u8 *) pasFile, (u8 *) pasSection, (u8 *) pasKey, pheValue);

    if (ret == 0) {
        return SDK_ERR;
    }
    else {
        return SDK_OK;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkIniWriteKey(const u8 *pasFile, const u8 *pasSection ,const u8 *pasKey, const u8 *pheValue)
   ��������: дINI�ļ����ƶ��ֶε�ָ����ֵ
   �������: pasFile �ļ�;pasSection �ֶ����� ;pasKey �� ;pheValue �õ��ļ�ֵ
   �������: ��
   ��   ��  ֵ: д��ɹ��򷵻�SDK_OK
                        д��ʧ���򷵻�SDK_ERR��
                        ����ָ��Ϊ���򷵻�SDK_PARA_ERR
   �޸ı�ע:д����ƶ��ļ���С��󲻳���4k������д��ʧ�ܣ�����ļ�ֵ���Ȳ��ܴ���127������ʧ��
   ����ʱ��: 2012.05.16 15:38:39
*******************************************************************/
s32 sdkIniWriteKey(const u8 *pasFile, const u8 *pasSection, const u8 *pasKey, const u8 *pheValue) {
    u8 ret;

    if (NULL == pasFile || NULL == pasKey || NULL == pasSection || NULL == pheValue) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (strlen(pheValue) >= 127) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = (u8) sdk_IniWriteKey((u8 *) pasFile, (u8 *) pasSection, (u8 *) pasKey, (u8 *) pheValue);

    if (ret == 0) {
        return SDK_ERR;
    }
    else {
        return SDK_OK;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkTruncateChar(const u8 *pasSrc,s32 siAvailableChar)
   ��������: ���ָ����Χ�����ַ���������󲻳����ַ�������
   �������: const u8 *pasSrc:Դ��;s32 siAvailableChar:��Ҫ�����ַ��ĳ���
   �������: ��
   ��   ��  ֵ: ��ȷ�򷵻�ʵ�ʱ����ַ��ĳ���.
                        ����ָ��Ϊ���򷵻�SDK_PARA_ERR��
   �޸ı�ע:
   ����ʱ��: 2012.05.15 14:33:25
*******************************************************************/
s32 sdkTruncateChar(const u8 *pasSrc, s32 siAvailableChar) {
    s32 Len, i;

    if (pasSrc == NULL || siAvailableChar < 0) {
        return SDK_PARA_ERR;
    }
    //return TruncateChar((u8 *)pasSrc, siAvailableChar);
    //zhouzhihua 2013.05.06 16:11
    Len = strlen(
            pasSrc);                                                                               //ȡԭ���ĳ���

    for (i = 0; (i < siAvailableChar) && (i < Len); i++) {
        if (pasSrc[i] > 0x80) {
            i++;                                                                                                //�����ƽ�

            if ((i >= siAvailableChar) || (i >= Len)) {
                if (i >= Len) {
                    //zhouzhihua 2013.05.06 17:40
                    *((u8 * ) & pasSrc[i -
                                       1]) = 0;                                                                         //�޸�Դ���Ƿ��ַ�
                }
                i--;
                break;
            }
        }
    }

    return i;   //lint !e850
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkStrpos(const u8 *pasStr,const u8 asChr)
   ��������: ����ָ���ַ����ַ����е�λ��
   �������:const u8 *pasStr: �ַ���;const u8 asChr:�����ַ�
   �������: ��
   ��   ��  ֵ: ���ش����ַ��±�,���δ�ҵ�����SDK_ERR(-1).
                        ����ָ��Ϊ���򷵻�SDK_PARA_ERR
   �޸ı�ע:
   ����ʱ��: 2012.05.15 15:42:52
*******************************************************************/
s32 sdkStrpos(const u8 *pasStr, const u8 asChr) {
    u32 i = 0;

    if (NULL == pasStr) {
        Assert(0);
        return SDK_PARA_ERR;    //fusuipu 2013.07.09 9:33
    }

    // return strpos((u8 *)pasStr, asChr);
    //zhouzhihua 2013.05.06 16:51
    while (0 != pasStr[i]) {
        if (pasStr[i] == asChr) {
            return i;
        }
        else {
            i++;
        }
    }

    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkTrim(u8 *pasStr,enum SDK_ALIGN_MODE eMode)
   ��������: ɾ���ַ����ո�
   �������: u8 *pasStr:�����ַ���;SDK_ALIGN_MODE eMode:ɾ���ո��λ��
   �������: u8 *pasStr:�����ַ���
   ��   ��  ֵ: �����ַ�������Ϊ0�򷵻�SDK_PARA_ERR
                        �����ַ���Ϊ���򷵻�SDK_PARA_ERR
                       ɾ���ɹ��򷵻�SDK_OK.
   �޸ı�ע:
   ����ʱ��: 2012.05.15 15:50:07
*******************************************************************/
s32 sdkTrim(u8 *pasStr, enum SDK_ALIGN_MODE eMode) {
    u32 len;

    if (NULL == pasStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen(pasStr);

    if (len == 0) {
        return SDK_PARA_ERR;
    }
    else {
        if (eMode == SDK_LEFT) {
            sdk_TrimL(pasStr);
        }
        else if (eMode == SDK_RIGHT) {
            sdk_TrimR(pasStr);
        }
        else {
            sdk_Trim(pasStr);
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkIniTrimBracket(u8 *pasStr,enum SDK_ALIGN_MODE eMode)
   ��������: ɾ���ַ����ո��[,]
   �������: u8 *pasStr:�����ַ���;SDK_ALIGN_MODE eMode:ɾ���ո����[,]��λ��
   �������: u8 *pasStr:�����ַ���
   ��   ��  ֵ: �����ַ�������Ϊ0�򷵻�SDK_PARA_ERR
                        �����ַ���Ϊ���򷵻�SDK_PARA_ERR
                       ɾ���ɹ��򷵻�SDK_OK.
   �޸ı�ע:
   ����ʱ��: 2012.05.15 15:50:07
*******************************************************************/
s32 sdkIniTrimBracket(u8 *pasStr, enum SDK_ALIGN_MODE eMode) {
    u32 len;

    if (NULL == pasStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen(pasStr);

    if (len == 0) {
        return SDK_PARA_ERR;
    }
    else {
        if (eMode == SDK_LEFT) {
            sdk_IniTrimBracketL(pasStr);
        }
        else if (eMode == SDK_RIGHT) {
            sdk_IniTrimBracketR(pasStr);
        }
        else {
            sdk_IniTrimBracket(pasStr);
        }
    }
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkRegulateAmount(u8 *pasDest,const u8 *pbcAmount)
   ��������: ���滯���,ת���ɺ�������λС����ĸ�ʽ
   �������: const u8 *pbcAmount:����6���ֽ�BCD���
   �������: u8 *pasDest:ת�������.��ASCII��
   ��   ��  ֵ: ������Ϊnull�򷵻�SDK_PARA_ERR
                        ���滯�ɹ��򷵻�SDK_OK��
   �޸ı�ע:
   ����ʱ��: 2012.05.15 16:23:59
*******************************************************************/
s32 sdkRegulateAmount(u8 *pasDest, const u8 *pbcAmount) {
    u8 i, j = 0;
    u8 temp[32] = {0};

    if (pbcAmount == NULL || pasDest == NULL) {
        return SDK_PARA_ERR;
    }
    //RegulateAmount(pasDest, (u8 *)pbcAmount);
    //zhouzhihua 2013.05.06 16:52
    memset(temp, 0, sizeof(temp));
    sdkBcdToAsc(temp, pbcAmount, 6);                                            //�����ת����ASCII��
    i = (u8) strspn(temp, "0");

    if (i >
        9)                                                                                   //ֻ�нǷ�
    {
        pasDest[0] = '0';
        pasDest[1] = '.';
        pasDest[2] = temp[10];
        pasDest[3] = temp[11];
        pasDest[4] = 0;
    }
    else {
        for (j = 0; j < (10 - i); j++)                                   //���ڽǷ�
        {
            pasDest[j] = temp[i + j];
        }

        pasDest[j++] = '.';
        pasDest[j++] = temp[10];                                                //��
        pasDest[j++] = temp[11];                                                //��
        pasDest[j++] = 0;
    }
    sdkFreeMem(temp);
    return SDK_OK;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkFormBlankAlignChar(enum SDK_ALIGN_MODE eMode,u8 *pasDest,const u8 *pasSrc,u16 usAvailableChar)
   ��������: �ڳ���usAvailableChar�������£������ֻ�ȫ��Src�е��ַ���
   �ϲ���Dest��Ч�ַ������棨����'\0'��β�ĺ��棩������eModeҪ
   �����ո���ȷ��Src��Dest��Ч�ַ�������ľ���λ��
   �������: SDK_ALIGN_MODE eMode:���뷽ʽ(���Ҷ���);const u8 *pasSrc:��������:u16 usAvailableChar:��Ч�ַ�
   �������:  u8 *pasDest:�ϲ��������
   ��   ��  ֵ:��������Ϊ�շ���SDK_PARA_ERR
                         �ɹ��򷵻ؽ�ȡԭ���ַ���
   �޸ı�ע:
   ����ʱ��: 2012.05.15 16:28:29
*******************************************************************/
s32 sdkFormBlankAlignChar(enum SDK_ALIGN_MODE eMode, u8 *pasDest, const u8 *pasSrc,
                          s32 siAvailableChar) {
    u8 type = 0;
    s32 i, j;
    s32 RemainsChar, TrunSrc = 0;

    if (NULL == pasSrc || NULL == pasDest || siAvailableChar < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (eMode == SDK_RIGHT) {
        // type = RALIGN;//zhouzhihua 2013.07.02 10:13
        type = SDK_RIGHT;
    }
    else {
        // type = LALIGN;//zhouzhihua 2013.07.02 10:13
        type = SDK_LEFT;
    }
    //return FormBlankAlignChar(type, pasDest, (u8 *)pasSrc, siAvailableChar);
    //zhouzhihua 2013.05.06 17:4

    i = strlen(pasDest);
    j = strlen(pasSrc);
    Verify((i = sdkTruncateChar(pasDest, i)) >= 0);
    Verify((j = sdkTruncateChar(pasSrc, j)) >= 0);
    RemainsChar = siAvailableChar;

    if (RemainsChar >= (i + j)) {
        //if(type == RALIGN)
        if (type == SDK_RIGHT)           //zhouzhihua 2013.07.02 10:13
        {
            j = RemainsChar - (i + j);
            RemainsChar -= i;

            for (i = 0; i < j; i++) {
                strncat(pasDest, " ", 1);
                RemainsChar--;
            }

            j = sdkTruncateChar(pasSrc, RemainsChar);
            strncat(pasDest, pasSrc, j);
        }
        else {
            strcat(pasDest, pasSrc);
            RemainsChar = RemainsChar - (i + j);

            while (RemainsChar > 0) {
                strncat(pasDest, " ", 1);
                RemainsChar--;
            }
        }
        TrunSrc = strlen(pasSrc);
    }
    else {
        if (RemainsChar > i) {
            RemainsChar -= i;
            j = sdkTruncateChar(pasSrc, RemainsChar);
            strncat(pasDest, pasSrc, j);
            TrunSrc = j;
        }
        else {
            i = strlen(pasDest);
            j = sdkTruncateChar(pasDest, RemainsChar);

            if (i > j) {
                memset(&pasDest[j], 0, i - j);
            }
            TrunSrc = 0;
        }
        i = strlen(pasDest);

        if (siAvailableChar > i) {
            RemainsChar = siAvailableChar - i;

            while (RemainsChar > 0) {
                strncat(pasDest, " ", 1);
                RemainsChar--;
            }
        }
    }
    return TrunSrc;                                                                                     //���ؽ�ȡԭ���ַ���
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkCmpYear(u8 bcYear1,u8 bcYear2)
   ��������: �Ƚ��������С(1950~2049)��
   �������: �������Ƚ���ݣ�һ���ֽ�BCD��
   �������: ��
   ��   ��  ֵ:  SDK_EQU (0)	�����
                        SDK_OK (1)	nYear1 > nYear2
                        SDK_ERR (-1)	nYear1 < nYear2

   �޸ı�ע: ʱ��Ϊѹ��BCD.�����ܵ�ȡֵ��ΧΪ��
   0x00~0x99.С��0x50Ϊ2000~2049�꣬���ڵ���0x50Ϊ1950~1999��.
   ����ʱ��: 2012.05.15 16:53:55
*******************************************************************/
s32 sdkCmpYear(u8 bcYear1, u8 bcYear2) {
    u16 m = 0, n = 0;

    //return CmpYear(bcYear1, bcYear2);
    //zhouzhihua 2013.05.06 17:30
    sdkBcdToU16(&m, &bcYear1, 1);

    if (m < 50) { m += 100; }
    sdkBcdToU16(&n, &bcYear2, 1);

    if (n < 50) { n += 100; }

    if (m == n) { return 0; }
    else if (m > n) {
        return 1;
    }
    else { return -1; }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������:bool sdkIsAssicNum(u8 const *pheSrc)
   ��������:�ж��Ƿ�Ϊascii����(0-9)
   �������:u8 const *pheSrc:��������
   �������:��
   ��   ��  ֵ:��ascii����(0-9)�򷵻�true
                          ���ǻ���ָ��Ϊ���򷵻�false��
   �޸ı�ע:
   ����ʱ��: 2012.05.20 16:07:15
*******************************************************************/
bool sdkIsAsciiNum(u8 const *pheSrc) {
    u8 te = 0;
    u32 len;

    if (NULL == pheSrc) {
        Assert(0);
        return false;
    }
    len = strlen(pheSrc);


    while (len--) {
        te = pheSrc[len];

        if ((te > '9' || te < '0')) {
            return false;
        }
    }

    return true;
}

/*****************************************************************************
** Descriptions:	ɾ���ļ���ĳһ������
** Parameters:          const u8 *pasFile
                               s32 iStart
                               s32 iDelLen
** Returned value:
** Created By:		shijianglong  2012.11.21
** Remarks:
*****************************************************************************/
s32 sdkDelFileData(const u8 *pasFile, s32 iStart, s32 iDelLen) {
    u8 *buf1, *buf2;
    s32 size, read_size;
    s32 rlt = 0;

    if (pasFile == NULL || iStart < 0 || iDelLen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
#if 0 /*Modify by zhouzhihua at 2013.07.02  10:14 */

    if(access(pasFile, F_OK) != 0)                                                                 //?????
    {
        return SDK_FILE_NONE;
    }
#else

    if (sdkAccessFile(pasFile) ==
        false)                                                                 //?????
    {
        return SDK_FILE_NONE;
    }
#endif /* if 0 */
    size = sdkGetFileSize(pasFile);                 //��ȡ�ļ���С

    if (size < 0) {
        return SDK_FILE_NONE;
    }

    if (iStart + iDelLen > size) {
        rlt = SDK_PARA_ERR;         //����Ŀ�ʼλ�� + ɾ�����ݳ��� ���ļ��ܳ��Ȼ���
    }
    else {
        buf1 = sdkGetMem(size + 1);
        buf2 = sdkGetMem(size + 1 - iDelLen);

        if (NULL == buf1 || NULL == buf2) {
            Assert(0);
            return SDK_FILE_ERROR;
        }
        read_size = size;

        rlt = sdkReadFile(pasFile, buf1, 0, &read_size);

        if (rlt == SDK_FILE_OK) {
            memcpy(buf2, buf1, iStart);             //?buf1???????????buf2?
            memcpy(&buf2[iStart], &buf1[iStart + iDelLen], (size - iStart) - iDelLen);
            rlt = sdkWriteFile(pasFile, buf2, size - iDelLen);
        }
        sdkFreeMem(buf1); //yuyt
        sdkFreeMem(buf2);
    }
    return rlt;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkSplitStr(u8 *pheDest, u8 const *pheSrc, u32 siSrartPos, u32 siDestLen)
   ��������: ��ȡ�̶����ȿɼ��ַ�(���ֻ���ascii��)
   �������:u8 const *pheSrc:�������ݣ�u32 siSrartPos:��ʼλ��(���㿪ʼ)��u32 siDestLen:��Ҫ��ȡ�ĳ���
   �������:u8 *pheDest:��ȡ����������
   ��   ��  ֵ:��ȷ�򷵻�ʵ�ʽ�ȡ����
                      �в��ɼ��ַ����ߴ�������Ϊ���򷵻�SDK_PARA_ERR��
   �޸ı�ע:�����ȡ�������ַ������ȱ�siDestLen��������˵����ȡ�ַ�����ʱ���һ�������һ��Ϊ˫�ֽ��ַ���һ�룬Ϊ����Ҫȥ��
   �����ȡ�������ַ������ȱ�siDestLen��һ����˵����ȡ�ַ�����ʱ���һ��Ϊ˫�ֽ��ַ���һ�룬Ϊ����Ҫȥ��
   ����ʱ��: 2012.05.20 16:37:08
*******************************************************************/
s32 sdkSplitStr(u8 *pheDest, u8 const *pheSrc, s32 siSrartPos, s32 siDestLen) {
//    u32 si_i, si_j;

    if (NULL == pheDest || NULL == pheSrc || siSrartPos < 0 || siDestLen < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (strlen(pheSrc) == 0 || ((s32) strlen(pheSrc) < siSrartPos + siDestLen)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkIsVisibleStr(pheSrc, strlen(pheSrc)) == false) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    //��Ϊ�ɼ��ַ�
    if (sdkIsVisibleStr(pheSrc, siSrartPos) ==
        false) //shijianglong 2013.05.22 14:58˵����ȡ�ĵ�һ���ֽ�Ϊ˫�ֽ��ַ��ĺ�������
    {
        if (sdkIsVisibleStr(&pheSrc[siSrartPos + 1], siDestLen - 1) ==
            false) //˵������һ���ֽ���˫�ֽڿɼ��ַ���ǰ����ֽ�
        {
            memcpy(pheDest, &pheSrc[siSrartPos + 1], siDestLen - 2);
            memcpy(&pheDest[siDestLen - 2], "\x00", 1);
            return siDestLen - 2;
        }
        else {
            memcpy(pheDest, &pheSrc[siSrartPos + 1], siDestLen - 1);
            memcpy(&pheDest[siDestLen - 1], "\x00", 1);
            return siDestLen - 1;
        }
    }
    else {
        if (sdkIsVisibleStr(&pheSrc[siSrartPos], siDestLen) == false) {
            memcpy(pheDest, &pheSrc[siSrartPos], siDestLen - 1);
            memcpy(&pheDest[siDestLen], "\x00", 1);
            return siDestLen - 1;
        }
        else {
            memcpy(pheDest, &pheSrc[siSrartPos], siDestLen);
            memcpy(&pheDest[siDestLen], "\x00", 1);
            return siDestLen;
        }
    }
////////////////////////////////////////////////////////////
#if 0 /*Modify by shijianglong at 2013.05.22  15:52 */

    for(si_i = 0, si_j = 0; si_i < siSrartPos; si_i++)
    {
        if((pheSrc[si_i] & 0x80) != 0)
//        if(Private_sdkJudgeChn(pheSrc[si_i],pheSrc[si_i+1],0,0) == true)
        {
            si_j++;
        }
    }

    if(si_j % 2 != 0)                           //�Ӱ�����ֿ�ʼ��ȡ
    {
        for(si_i = 0, si_j = 0; si_i < (siDestLen + 1); si_i++)
        {
            if((pheSrc[siSrartPos + si_i + 1] & 0x80) != 0) //lint  !e690
            {
                si_j++;
            }
        }

        if(si_j % 2 != 0)                   //ȥ������������
        {
            memcpy(pheDest, &pheSrc[siSrartPos + 1], siDestLen - 2);
            memcpy(&pheDest[siDestLen - 2], "\x00", 1);
            return siDestLen - 2;
        }
        else
        {
            memcpy(pheDest, &pheSrc[siSrartPos + 1], siDestLen - 1);
            memcpy(&pheDest[siDestLen - 1], "\x00", 1);
            return siDestLen - 1;
        }
    }
    else                                //��ȡ��ǰ�治�ǰ������
    {
        for(si_i = 0, si_j = 0; si_i < siDestLen; si_i++)
        {
            if((pheSrc[siSrartPos + si_i] & 0x80) != 0)
            {
                si_j++;
            }
        }

        if(si_j % 2 != 0)                   //ȥ������������
        {
            memcpy(pheDest, &pheSrc[siSrartPos], siDestLen - 1);
            memcpy(&pheDest[siDestLen - 1], "\x00", 1);
            return siDestLen - 1;
        }
        else
        {
            memcpy(pheDest, &pheSrc[siSrartPos], siDestLen);
            memcpy(&pheDest[siDestLen], "\x00", 1);
            return siDestLen;
        }
    }
#endif /* if 0 */
}

/*******************************************************************
   ��	  ��: huxiang
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������: �õ��ļ�һ�е��ַ���
   ��ڲ���: 1��stream �ļ��� 2��line һ�е��ַ��� 3��maxlength һ���ַ�������󳤶�
   �� �� ֵ: �ַ��ĸ���,�ļ���������-1��
   ��	  ע:
 ********************************************************************/
static s32 IniGetLineX(FILE *stream, u8 *line, s32 maxlength) {
    s32 size = 0, rslt;
    u8 ch;

    memset(line, 0, maxlength);
    rslt = feof(stream);

    while (rslt == 0) {
        if (size < maxlength) {
            ch = (u8) fgetc(stream);
            rslt = feof(stream);

            if (ch == 0x0d || ch == 0x0a) {
                break;
            }
            else {
                line[size] = ch;
                size++;
            }
        }
        else {
            break; //shijianglong 2013.02.28 15:36֮ǰ�⺯��û��break��
        }
    }

    if (rslt != 0) {
        return -1;
    }
    else {
        return size;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkIniGetOneLine(FILE* pstream, u8 *pucline, s32 siLen)
   ��������: �õ��ļ�һ�е��ַ���
   �������: FILE* pstream: stream �ļ���
                          s32 siLen:һ���ַ�������󳤶�
   �������: u8 *pucline:line һ�е��ַ���
   ��   ��  ֵ: ��ȷ�򷵻��ַ�����
                       �ļ���������SDK_ERR��
                        ��������Ϊ���򷵻�SDK_PARA_ERR��
   �޸ı�ע:
   ����ʱ��: 2012.05.16 14:27:40
*******************************************************************/
s32 sdkIniGetOneLine(FILE *pstream, u8 *pucline, s32 siLen) {
    if (NULL == pstream || NULL == pucline || siLen <= 0) //shijianglong 2013.02.28 11:37��󳤶Ȳ���Ϊ0
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return IniGetLineX(pstream, pucline, siLen);
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkIniGetOneLine(FILE* pstream, u8 *pucline, s32 maxlen)
   ��������: д�ļ�һ�е��ַ���
   �������: FILE* pstream: stream �ļ���
                          u8 *pucline:line һ�е��ַ���
   �������: ��
   ��   ��  ֵ: ��ȷ�򷵻�SDK_OK
                       ʧ�ܷ���SDK_ERR��
                        ��������Ϊ���򷵻�SDK_PARA_ERR��
   �޸ı�ע:
   ����ʱ��: 2012.05.16 14:27:40
*******************************************************************/
s32 sdkIniWriteOneLine(const FILE *pstream, const u8 *pucline) {
    s32 ret;

    if (NULL == pstream || NULL == pucline) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    ret = sdk_IniWriteLine(pstream, (u8 *) pucline);

    if (ret == 0) {
        return SDK_OK;
    }
    else {
        return SDK_ERR;
    }
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 sdkSavePosSn(const u8 *pasSrc)
   ��������: ��������
   �������: const u8 *pasSrc:������ַ�,��󲻳���16λ
   �������:
   ��   ��  ֵ:���ȴ���16���߲�Ϊascii�򷵻�SDK_ERR
                       ����ָ��Ϊ���򷵻�SDK_PARA_ERR
                       �ɹ��򷵻�SDK_OK��
   �޸ı�ע:
   ����ʱ��: 2012.05.16 14:38:22
*******************************************************************/
s32 sdkSavePosSn(const u8 *pasSrc) {
    u32 len;

    if (NULL == pasSrc) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen((u8 *) pasSrc);

    if (len > 16 || sdkIsAscii(pasSrc) == false) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 Private_sdkReadFifo(FIFO * pstFifo, u32 sims)
   ��������: ��FIFO��������
   �������:ms: ����ʱ
   �������: pstFifo: �ܵ����ݻ���
   ��   ��  ֵ: ��������FIFO�����򷵻�SDK_OK
                         ����ָ��Ϊ���򷵻�SDK_PARA_ERR
                       δ����FIFO����Э�������򷵻�SDK_ERR��
   �޸ı�ע: ��ʱ���Ϊ0,ȱʡΪ10us,���Ϊ��������.
   ����ʱ��: 2012.05.16 15:55:33
*******************************************************************/
s32 Private_sdkReadFifo(SDK_PRIVATE_FIFO *pstFifo, s32 siMs) {
    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 Private_sdkWriteFifo(const FIFO *pstFifo)
   ��������: дPANEL�ܵ�����
   �������: pstFifo: �ܵ�����
   �������: ��
   ��   ��  ֵ: ����ָ��Ϊ���򷵻�SDK_PARA_ERR
                            �ɹ��򷵻�SDK_OK
   �޸ı�ע:
   ����ʱ��: 2012.05.16 16:19:35
*******************************************************************/
s32 Private_sdkWriteFifo(const SDK_PRIVATE_FIFO *pstFifo) {
    //return sdk_dev_sys_write_fifo(pstFifo);
    Trace("devlcd", "fun not work :%s\r\n", __FUNCTION__);
    return SDK_ERR;
}

/*******************************************************************
   ��          Ȩ: �¹���
   ��          ��: ʯ����
   ��������: s32 Private_sdkWriteSelfFifo(const FIFO *pstFifo)
   ��������: д�Լ��Ĺܵ�����
   �������: pstFifo: �ܵ�����
   �������: ��
   ��   ��  ֵ: ����ָ��Ϊ���򷵻�SDK_PARA_ERR
                            �ɹ��򷵻�SDK_OK
   �޸ı�ע:
*******************************************************************/
s32 Private_sdkWriteSelfFifo(const SDK_PRIVATE_FIFO *pstFifo) {
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:	��ȡͼƬ��Ⱥ͸߶�
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2014.04.16
** Remarks:
*****************************************************************************/
s32 sdkGetBmpSize(s32 *pWidth, s32 *pHeight, const u8 *pBmpFile) {
    FILE *bmpfp = NULL;

    if (NULL == pWidth || NULL == pHeight || NULL == pBmpFile) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    bmpfp = fopen(pBmpFile, "rb");

    if (NULL == bmpfp) {
        return SDK_ERR;
    }
    fseek(bmpfp, 18, SEEK_SET);

    if (1 != fread(pWidth, sizeof(s32), 1, bmpfp)) {
        fclose(bmpfp);
        return SDK_ERR;
    }

    if (1 != fread(pHeight, sizeof(s32), 1, bmpfp)) {
        fclose(bmpfp);
        return SDK_ERR;
    }
    fclose(bmpfp);
    Trace("libsdksys", "width : %d , height : %d\n", *pWidth, *pHeight);
    return SDK_OK;
}

