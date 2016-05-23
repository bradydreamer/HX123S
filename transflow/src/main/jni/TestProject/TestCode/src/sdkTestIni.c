#include "sdkGlobal.h"
#include "SdkTest.h"


#define SDK_INI_NO_SECTION              -1

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.20
** Remarks:
*****************************************************************************/
static s32 test_sdkCreatTestIniFile(void) {
    u8 file_name[2][50] = {{"/mtd0/res/sample1.ini"},
                           {"/mtd0/res/sample2.ini"}};
    u8 i = 0;
//    u8 ret = 0;//shijianglong 2013.06.20 10:3
//    s32 j = 50000;
    FILE *fp = NULL;
    SDK_INI_HAND pContent = NULL;

    for (i = 0; i < 2; i++) {
#if 0 /*Modify by wangmingming at 2014.05.26  20:35 */
#if ((MACHINEMODEL != SDK_SYS_MACHINE_K501AM) && (MACHINEMODEL != SDK_SYS_MACHINE_K508A))

        if ((fp = fopen(file_name[i], "r")) != NULL)
        {
            Trace("test", "���ļ�%sʧ��\r\n", file_name[i]);
            fclose(fp);
            remove(file_name[i]);
        }
#endif

        pContent = sdkCreatIni();
        if(NULL == pContent)
        {
            TestLog("fusuipu", "Get pContent failed!\n");
        }
        sdkIniAddKeyValue(pContent, "ModeList", "04", "K301", "");
        TestLog("fusuipu", "step3 check\n");
        sdkIniAddKeyValue(pContent, "ModeList", "11", "K320", "");
        sdkIniAddKeyValue(pContent, "ModeList", "05", "K350", "");
        sdkIniAddKeyValue(pContent, "ModeList", "06", "K360", "");
        sdkIniAddKeyValue(pContent, "Config", "DegugMode", "1", "");
        sdkIniAddKeyValue(pContent, "Config", "NewMachine", "12", "");
        sdkIniAddKeyValue(pContent, "Config", "MachMode", "0", "");
        sdkIniAddKeyValue(pContent, "Seg", "Degug", "1", "");
        sdkIniAddKeyValue(pContent, "Seg", "New", "12", "");
        sdkIniAddKeyValue(pContent, "Seg", "Mach", "0", "");
        sdkIniSave(pContent, file_name[i]);
        sdkIniClose(pContent);
#endif /* if 0 */
#if ((MACHINEMODEL != SDK_SYS_MACHINE_K501AM) && (MACHINEMODEL != SDK_SYS_MACHINE_K508A))
        /*=======BEGIN: wangmingming 2014.06.05  13:50 modify===========*/
        if (access(file_name[i], F_OK) == 0)
        {//�ļ�����
            if (remove(file_name[i]) == SDK_OK )
            {
                Trace("test", "�ļ���ɾ���ɹ�\r\n");
            }
        }
        /*====================== END======================== */
#endif

        TEST_IS_TRUE_WITH_INFO("����Ini�ļ��������", ((pContent = sdkCreatIni()) != NULL));
        if (NULL == pContent) {
            TestLog("test", "����Ini�ļ����ʧ��!\n");
        }
        TEST_IS_TRUE_WITH_INFO("���ini�ؼ������ݲ���",
                               (sdkIniAddKeyValue(pContent, "TestSection", "TestKey",
                                                  "TestKeyValue", "TestComment") >= 0));
        TEST_IS_TRUE_WITH_INFO("����", (sdkIniSave(pContent, file_name[i]) == SDK_OK));
        TEST_IS_TRUE_WITH_INFO("����", (sdkIniClose(pContent) == SDK_OK));
    }

#if 0 /*Modify by wangmingming at 2014.05.29  11:8 */
    sdkFreeMem(pContent);
    if ( pContent != NULL )
    {
        pContent = NULL;
    }
#endif /* if 0 */

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniLoad(void)   //  1
{
    s32 i = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniLoad test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.26  20:10 */
    for(i = 0; i < 8; i++)
    {
        pContent = sdkIniLoad("/mtd0/res/sample1.ini");

        if(NULL == pContent)
        {
            break;
        }
        sdkIniClose(pContent);
        TestLog("ini test", "%d times test!\r\n", i);
    }

    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));
    pContent = sdkIniLoad(NULL);
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent == NULL));
    pContent = sdkIniLoad("/mtd0/res/sale2.ini");
    TEST_IS_TRUE_WITH_INFO ("Ini�ļ��뾶�������", (pContent == NULL));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));
    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ���������", (sdkIniClose(pContent) == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample2.ini")) != NULL));
    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ���������", (sdkIniClose(pContent) == SDK_OK));
    //TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (sdkIniLoad(NULL) == NULL));
    //TEST_IS_TRUE_WITH_INFO ("Ini�ļ��뾶�������", (sdkIniLoad("/mtd0/res/sale2.ini") == NULL));
#endif /* if 0 */
    TestLog("ini", "end sdkIniLoad test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniGetSectionNum(void)   //  1
{
    s32 ret = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniGetSectionNum test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.26  20:12 */
    pContent = sdkIniLoad("/mtd0/res/sample1.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    ret = sdkIniGetSectionNum(pContent);
    TEST_IS_TRUE_WITH_INFO ("��ȡ�ڵ��������", (ret == 3));

    ret = sdkIniGetSectionNum(NULL);
    TEST_IS_TRUE_WITH_INFO ("�������ȡ�ڵ��������", (ret == SDK_PARA_ERR));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Iin�ļ�����", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));
    TEST_IS_TRUE_WITH_INFO("��ȡ����С�ڸ�������", (sdkIniGetSectionNum(pContent) == 1));
    TEST_IS_TRUE_WITH_INFO("�Ƿ���������", (sdkIniGetSectionNum(NULL) == SDK_PARA_ERR));
    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ���������", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniGetSectionNum test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniGetKeyNum(void)   //  1
{
    s32 ret = 0;
    s32 i = 0;
    u8 buf[100] = {0};


    SDK_INI_HAND pContent;
    TestLog("ini", "start sdkIniGetKeyNum test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  10:52 */
    memset(buf, 0X31, 80);
    TestLog("ini", "start sdkIniGetKeyNum test!\r\n");
    pContent = sdkIniLoad("/mtd0/res/sample1.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    for(i = 0; i < 10; i++)
    {
        ret = sdkIniGetKeyNum(pContent, "ModeList");

        if(ret != 4)
        {
            break;
        }
    }

    TEST_IS_TRUE_WITH_INFO ("GetKeyNum����", (ret == 4));

    ret = sdkIniGetKeyNum(NULL, "ModeList");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyNum(pContent, NULL);
    TEST_IS_TRUE_WITH_INFO ("Ini�ļ��չؼ��ֲ���", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyNum(pContent, buf);
    TEST_IS_TRUE_WITH_INFO ("�ؼ��ֹ�������", (ret == SDK_PARA_ERR));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Iin�ļ�����", (ret == SDK_OK));

#else
    memset(buf, 0X31, 80);

    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("GetKeyNum����", (sdkIniGetKeyNum(pContent, "TestSection") == 1));

    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����", (sdkIniGetKeyNum(NULL, "ModeList") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("Ini�ļ��չؼ��ֲ���", (sdkIniGetKeyNum(pContent, NULL) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ؼ��ֹ�������", (sdkIniGetKeyNum(pContent, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ�����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniGetKeyNum test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniGetSectionName(void)   //  1
{
    u8 buf[100] = {0};
    s32 ret = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniGetSectionName test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  10:58 */
    pContent = sdkIniLoad("/mtd0/res/sample1.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    ret = sdkIniGetSectionName(pContent, 0, buf);
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (strcmp(buf, "ModeList") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (ret == strlen("ModeList")));

    ret = sdkIniGetSectionName(pContent, 1, buf);
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (strcmp(buf, "Config") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (ret == strlen("Config")));

    ret = sdkIniGetSectionName(pContent, 2, buf);
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (strcmp(buf, "Seg") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetSectionName", (ret == strlen("Seg")));

    ret = sdkIniGetSectionName(NULL, 0, buf);
    TEST_IS_TRUE_WITH_INFO ("�������ȡС�����Ʋ���", (ret == SDK_PARA_ERR));

    ret = sdkIniGetSectionName(pContent, 0, NULL);
    TEST_IS_TRUE_WITH_INFO ("�������ȡС�����Ʋ���", (ret == SDK_PARA_ERR));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Iin�ļ�����", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("��ȡһ��С�����Ƴ��Ȳ���",
                           (sdkIniGetSectionName(pContent, 0, buf) == strlen("TestSection")));
    TEST_IS_TRUE_WITH_INFO("��ȡһ��С�����Ʋ���", (strcmp(buf, "TestSection") == 0));

    TEST_IS_TRUE_WITH_INFO("�������ȡС�����Ʋ���",
                           (sdkIniGetSectionName(NULL, 0, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������ȡС�����Ʋ���",
                           (sdkIniGetSectionName(pContent, 0, NULL) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ�����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniGetSectionName test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniGetKeyName(void)   //  1
{
    u8 buf[100] = {0};
    s32 ret = 0;
    s32 i = 0;

    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniGetKeyName test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  11:1 */
    pContent = sdkIniLoad("/mtd0/res/sample1.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    ret = sdkIniGetKeyName(pContent, "Config", 0, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (strcmp(buf, "DegugMode") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (ret == strlen("DegugMode")));

    ret = sdkIniGetKeyName(pContent, "ModeList", 2, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (strcmp(buf, "05") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (ret == strlen("05")));

    for(i = 0; i < 10; i++)
    {
        ret = sdkIniGetKeyName(pContent, "Seg", 2, buf);

        if(strcmp(buf, "Mach") != 0)
        {
            break;
        }
    }

    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (strcmp(buf, "Mach") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (ret == strlen("Mach")));

    ret = sdkIniGetKeyName(NULL, "Seg", 2, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyName(pContent, NULL, 2, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName���Ϊ�ռ��", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyName(pContent, "Seg", 2, NULL);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName���Ϊ�ռ��", (ret == SDK_PARA_ERR));

    memset(buf, 0X31, 80);
    ret = sdkIniGetKeyName(pContent, buf, 2, NULL);
    TEST_IS_TRUE_WITH_INFO ("GetKeyName���Ϊ�ռ��", (ret == SDK_PARA_ERR));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Iin�ļ�����", (ret == SDK_OK));
#else
    ;
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("��ȡĳС�ڹؼ��������Ȳ���",
                           (sdkIniGetKeyName(pContent, "TestSection", 0, buf) ==
                            strlen("TestKey")));
    TEST_IS_TRUE_WITH_INFO("��ȡĳС�ڹؼ�����", (strcmp(buf, "TestKey") == 0));

    TEST_IS_TRUE_WITH_INFO("GetKeyName���Ϊ�ռ��",
                           (sdkIniGetKeyName(pContent, NULL, 0, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("GetKeyName���Ϊ�ռ��",
                           (sdkIniGetKeyName(pContent, "TestSection", 0, NULL) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ�����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */


    TestLog("ini", "end sdkIniGetKeyName test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniGetKeyValue(void)   //  1
{
    u8 buf[100] = {0};
    u8 buf1[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    memset(buf, 0, sizeof(buf));
    memset(buf1, 0, sizeof(buf1));

    TestLog("ini", "start sdkIniGetKeyValue test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  11:6 */
    pContent = sdkIniLoad("/mtd0/res/sample1.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    ret = sdkIniGetKeyValue(pContent, "Config", "NewMachine", buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue", (strcmp(buf, "12") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue", (ret == strlen("12")));

    for(i = 0; i < 10; i++)
    {
        ret = sdkIniGetKeyValue(pContent, "ModeList", "05", buf);

        if(strcmp(buf, "K350") != 0)
        {
            break;
        }
    }

    TEST_IS_TRUE_WITH_INFO ("GetKeyValue", (strcmp(buf, "K350") == 0));
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue", (ret == strlen("K350")));


    ret = sdkIniGetKeyValue(NULL, "ModeList", "05", buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyValue(pContent, NULL, "05", buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyValue(pContent, "ModeList", NULL, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyValue(pContent, "ModeList", "05", NULL);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    memset(buf1, 0x30, 80);
    ret = sdkIniGetKeyValue(pContent, buf1, "05", buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniGetKeyValue(pContent, "ModeList", buf1, buf);
    TEST_IS_TRUE_WITH_INFO ("GetKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Iin�ļ�����", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݳ��Ȳ���",
                           (sdkIniGetKeyValue(pContent, "TestSection", "TestKey", buf) ==
                            strlen("TestKeyValue")));
    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݲ���", (strcmp(buf, "TestKeyValue") == 0));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(NULL, "TestSection", "TestKey", buf) ==
                            SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(pContent, NULL, "TestKey", buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(pContent, "TestSection", NULL, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(pContent, "TestSection", "TestKey", NULL) ==
                            SDK_PARA_ERR));

    memset(buf1, 0x30, 80);
    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(pContent, buf1, "TestKey", buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("��ȡ�ؼ������ݷǷ���������",
                           (sdkIniGetKeyValue(pContent, "TestSection", buf1, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ر�Iin�ļ�����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */


    TestLog("ini", "end sdkIniGetKeyValue test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniAddSection(void)   //  1
{
    u8 buf[100] = {0};
    u8 temp[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    memset(buf, 0, sizeof(buf));
    memset(temp, 0, sizeof(temp));

    TestLog("ini", "start sdkIniAddSection test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  11:29 */
    for(i = 0; i < 8; i++)
    {
        pContent = sdkIniLoad("/mtd0/res/sample2.ini");
        sprintf(temp, "section%d", i);
        ret = sdkIniAddSection(pContent, temp, "");

        if(ret != strlen(temp))
        {
            break;
        }
        sdkIniSave(pContent, "/mtd0/res/sample2.ini");
        sdkIniClose(pContent);
    }

//    TEST_IS_TRUE_WITH_INFO ("AddSection", (ret == strlen(temp)));

    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    ret = sdkIniAddSection(NULL, temp, "");
    TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddSection(pContent, NULL, "");
    TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddSection(pContent, temp, NULL);
    TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (ret > 0));

    memset(buf, 0X30, 80);
    ret = sdkIniAddSection(pContent, buf, "");
    TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddSection(pContent, temp, buf);
    TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("�������С�����ݲ���", (ret == SDK_OK));

    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Ini����", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("AddSection�Ƿ���������",
                           (sdkIniAddSection(NULL, temp, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddSection�Ƿ���������",
                           (sdkIniAddSection(pContent, NULL, "") == SDK_PARA_ERR));

    //TEST_IS_TRUE_WITH_INFO ("AddSection�Ƿ���������", (sdkIniAddSection(pContent, temp, NULL) == SDK_PARA_ERR));

    memset(buf, 0X30, 80);

    TEST_IS_TRUE_WITH_INFO("AddSection�Ƿ���������",
                           (sdkIniAddSection(pContent, buf, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddSection�Ƿ���������",
                           (sdkIniAddSection(pContent, temp, buf) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddSection����",
                           (sdkIniAddSection(pContent, "TestSectionAdd", "TestCommentSectionAdd") ==
                            strlen("TestSectionAdd")));

    TEST_IS_TRUE_WITH_INFO("�������С�����ݲ���",
                           (sdkIniSave(pContent, "/mtd0/res/sample1.ini") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�ر�Ini����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniAddSection test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniAddKey(void)   //  1
{
    u8 temp[100] = {0};
    u8 temp2[100] = {0};
    u8 temp3[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniAddKey test!\r\n");
    //pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    //TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));
#if 0 /*Modify by wangmingming at 2014.05.27  13:47 */
    for(i = 0; i < 8; i++)
    {
        pContent = sdkIniLoad("/mtd0/res/sample2.ini");
        memset(temp, 0, sizeof(temp));
        memset(temp2, 0, sizeof(temp2));

        sprintf(temp, "section%d", i);
        sprintf(temp2, "key%d", i);
        ret = sdkIniAddKey(pContent, temp, temp2, "");

        if(ret != strlen(temp2))
        {
            break;
        }
        sdkIniSave(pContent, "/mtd0/res/sample2.ini");
        sdkIniClose(pContent);
    }

    TEST_IS_TRUE_WITH_INFO ("sdkIniAddKey", (ret == strlen(temp2)));

    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    ret = sdkIniAddKey(NULL, temp, temp2, "");
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKey(pContent, NULL, temp2, "");
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKey(pContent, temp, NULL, "");
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKey(pContent, temp, temp2, NULL);
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret > 0));

    memset(temp3, 0x30, 80);

    ret = sdkIniAddKey(pContent, temp3, temp2, "");
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKey(pContent, temp, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKey(pContent, temp, temp2, temp3);
    TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("sdkIniAddKey����",
                           (sdkIniAddKey(pContent, "TestSectionAdd", "TestKyeAdd",
                                         "TestCommentKyeAdd") == strlen("TestKyeAdd")));

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(NULL, temp, temp2, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(pContent, NULL, temp2, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(pContent, temp, NULL, "") == SDK_PARA_ERR));

    //TEST_IS_TRUE_WITH_INFO ("AddKey�Ƿ���������", (sdkIniAddKey(pContent, temp, temp2, NULL) == SDK_PARA_ERR));

    memset(temp3, 0x30, 80);

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(pContent, temp3, temp2, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(pContent, temp, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKey�Ƿ���������",
                           (sdkIniAddKey(pContent, temp, temp2, temp3) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���",
                           (sdkIniSave(pContent, "/mtd0/res/sample1.ini") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniAddKey test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniAddKeyValue(void)   //  1
{
    u8 temp[100] = {0};
    u8 temp1[100] = {0};
    u8 temp2[100] = {0};
    u8 temp3[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniAddKeyValue test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  13:54 */
    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));
    sdkIniClose(pContent);

    for(i = 0; i < 8; i++)
    {
        pContent = sdkIniLoad("/mtd0/res/sample2.ini");
        sprintf(temp, "section%d", i);
        sprintf(temp2, "key%d", i);
        sprintf(temp3, "%d", i);
        ret = sdkIniAddKeyValue(pContent, temp, temp2, temp3, "");

        if(ret != strlen(temp3))
        {
            break;
        }
        sdkIniSave(pContent, "/mtd0/res/sample2.ini");
        sdkIniClose(pContent);
    }

    TEST_IS_TRUE_WITH_INFO ("��ӹؼ������ݲ���", (ret == strlen(temp3)));

    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    ret = sdkIniAddKeyValue(NULL, temp, temp2, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, NULL, temp2, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, NULL, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, temp2, NULL, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, temp2, temp3, NULL);
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_OK));

    memset(temp1, 0x30, 80);

    ret = sdkIniAddKeyValue(pContent, temp1, temp2, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, temp1, temp3, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, temp2, temp1, "");
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniAddKeyValue(pContent, temp, temp2, temp3, temp1);
    TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("��ӹؼ������ݲ���",
                           (sdkIniAddKeyValue(pContent, "TestSectionAdd", "TestKeyAdd",
                                              "TestKeyValueAdd", "TestCommentKeyValueAdd") ==
                            strlen("TestKeyValueAdd")));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(NULL, temp, temp2, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, NULL, temp2, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp, NULL, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp, temp2, NULL, "") == SDK_PARA_ERR));

    //TEST_IS_TRUE_WITH_INFO ("AddKeyValue�Ƿ���������", (sdkIniAddKeyValue(pContent, temp, temp2, temp3, NULL) == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp1, temp2, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp, temp1, temp3, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp, temp2, temp1, "") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("AddKeyValue�Ƿ���������",
                           (sdkIniAddKeyValue(pContent, temp, temp2, temp3, temp1) ==
                            SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���",
                           (sdkIniSave(pContent, "/mtd0/res/sample1.ini") == SDK_OK));
    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */


    TestLog("ini", "end sdkIniAddKeyValue test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniClrSectionData(void) {
    u8 temp[100] = {0};
    u8 temp1[100] = {0};
//    u8 temp2[100] = {0};//shijianglong 2013.05.06 14:56
//    u8 temp3[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniClrSection test!\r\n");
    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����", (pContent != NULL));

    for (i = 0; i < 8; i++) {
        pContent = sdkIniLoad("/mtd0/res/sample2.ini");
        sprintf(temp, "section%d", i);
        sdkIniClrSection(pContent, temp);
        ret = sdkIniGetKeyNum(pContent, temp);

        if (ret != 0) {
            break;
        }
        sdkIniSave(pContent, "/mtd0/res/sample2.ini");
        sdkIniClose(pContent);
    }

    ret = sdkIniGetSectionNum(pContent);
    //  Trace("fusuipu", "node = %d", ret);
    TEST_IS_TRUE_WITH_INFO("����ڵ����", (ret == 3));

    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    ret = sdkIniClrSection(NULL, temp);
    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniClrSection(pContent, NULL);
    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    ret = sdkIniClrSection(pContent, temp1);
    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    TestLog("ini", "end sdkIniClrSection test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniRemoveSectionData(void) {
    u8 temp[100] = {0};
    u8 temp1[100] = {0};
//    u8 temp2[100] = {0};//shijianglong 2013.05.06 14:56
//    u8 temp3[100] = {0};
    s32 ret = 0;
    s32 i = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniRemoveSection test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  14:14 */
    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    for(i = 0; i < 8; i++)
    {
        pContent = sdkIniLoad("/mtd0/res/sample2.ini");
        sprintf(temp, "section%d", i);
        ret = sdkIniRemoveSection(pContent, temp);
        sdkIniSave(pContent, "/mtd0/res/sample2.ini");
        sdkIniClose(pContent);
    }

    ret = sdkIniGetSectionNum(pContent);
    //  Trace("fusuipu", "node = %d", ret);
    TEST_IS_TRUE_WITH_INFO ("ɾ���ڵ����", (ret == 3));

    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    ret = sdkIniRemoveSection(NULL, temp);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniRemoveSection(pContent, NULL);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    ret = sdkIniRemoveSection(pContent, temp1);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionData�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر��ļ�����", (ret == SDK_OK));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("ɾ���ڵ����",
                           (sdkIniRemoveSection(pContent, "TestSectionAdd") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������",
                           (sdkIniRemoveSection(NULL, temp) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������",
                           (sdkIniRemoveSection(pContent, NULL) == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    TEST_IS_TRUE_WITH_INFO("RemoveSectionData�Ƿ���������",
                           (sdkIniRemoveSection(pContent, temp1) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���",
                           (sdkIniSave(pContent, "/mtd0/res/sample1.ini") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�ر��ļ�����", (sdkIniClose(pContent) == SDK_OK));
#endif /* if 0 */

    TestLog("ini", "end sdkIniRemoveSection test!\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.19
** Remarks:
*****************************************************************************/
static void test_sdkIniRemoveSectionKey(void) {
    u8 temp[100] = {0};
    u8 temp1[100] = {0};
    u8 temp2[100] = {0};
//    u8 temp3[100] = {0};//shijianglong 2013.05.06 14:57
    s32 ret = 0;
    SDK_INI_HAND pContent;

    TestLog("ini", "start sdkIniRemoveSectionKey test!\r\n");
#if 0 /*Modify by wangmingming at 2014.05.27  14:18 */
    pContent = sdkIniLoad("/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("��Ini�ļ�����", (pContent != NULL));

    ret = sdkIniRemoveSectionKey(pContent, "ModeList", "11");
    TEST_IS_TRUE_WITH_INFO ("ɾ���ڵ�ؼ��ֲ���", (ret == SDK_OK));

    ret = sdkIniRemoveSectionKey(NULL, temp, temp2);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniRemoveSectionKey(pContent, NULL, temp2);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniRemoveSectionKey(pContent, temp, NULL);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionKey�Ƿ���������", (ret == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    ret = sdkIniRemoveSectionKey(pContent, temp1, temp2);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniRemoveSectionKey(pContent, temp, temp1);
    TEST_IS_TRUE_WITH_INFO ("RemoveSectionKey�Ƿ���������", (ret == SDK_PARA_ERR));

    ret = sdkIniSave(pContent, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("�������С�ڹؼ����ݲ���", (ret == SDK_OK));
    ret = sdkIniClose(pContent);
    TEST_IS_TRUE_WITH_INFO ("�ر�Ini�ļ�����", (ret == SDK_OK));

    ret = sdkIniSave(NULL, "/mtd0/res/sample2.ini");
    TEST_IS_TRUE_WITH_INFO ("����Ini�ļ��Ƿ���������", (ret == SDK_PARA_ERR));
    ret = sdkIniSave(pContent, NULL);
    TEST_IS_TRUE_WITH_INFO ("����Ini�ļ��Ƿ���������", (ret == SDK_PARA_ERR));
    ret = sdkIniClose(NULL);
    TEST_IS_TRUE_WITH_INFO ("�ر�Ini�ļ������Ƿ�����", (ret == SDK_PARA_ERR));
#else
    TEST_IS_TRUE_WITH_INFO("��Ini�ļ�����",
                           ((pContent = sdkIniLoad("/mtd0/res/sample1.ini")) != NULL));

    TEST_IS_TRUE_WITH_INFO("ɾ���ڵ�ؼ��ֲ���",
                           (sdkIniRemoveSectionKey(pContent, "TestSection", "TestKey") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionKey�Ƿ���������",
                           (sdkIniRemoveSectionKey(NULL, temp, temp2) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionKey�Ƿ���������",
                           (sdkIniRemoveSectionKey(pContent, NULL, temp2) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionKey�Ƿ���������",
                           (sdkIniRemoveSectionKey(pContent, temp, NULL) == SDK_PARA_ERR));

    memset(temp1, 0x30, 80);

    TEST_IS_TRUE_WITH_INFO("RemoveSectionKey�Ƿ���������",
                           (sdkIniRemoveSectionKey(pContent, temp1, temp2) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("RemoveSectionKey�Ƿ���������",
                           (sdkIniRemoveSectionKey(pContent, temp, temp1) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�������С�ڹؼ����ݲ���",
                           (sdkIniSave(pContent, "/mtd0/res/sample1.ini") == SDK_OK));

    TEST_IS_TRUE_WITH_INFO("�ر�Ini�ļ�����", (sdkIniClose(pContent) == SDK_OK));


    TEST_IS_TRUE_WITH_INFO("����Ini�ļ��Ƿ���������",
                           (sdkIniSave(NULL, "/mtd0/res/sample1.ini") == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("����Ini�ļ��Ƿ���������", (sdkIniSave(pContent, NULL) == SDK_PARA_ERR));

    TEST_IS_TRUE_WITH_INFO("�ر�Ini�ļ������Ƿ�����", (sdkIniClose(NULL) == SDK_PARA_ERR));
#endif /* if 0 */


    TestLog("ini", "end sdkIniRemoveSectionKey test!\r\n");
}

/*****************************************************************************
** Descriptions:	ini�����ļ��Զ�����
** Parameters:    	void:
** Returned value:	
** Created By:		fusuipu  2013.01.17
** Modified By:		wangmingming  2014.05.27
** Remarks: 		
*****************************************************************************/
void Test_sdkIniAuto(void) {
    TestLog("TestIni", "\r\n��ʼIni����\r\n ");

    sdkTestShowProcessing("��ʼini����...");

    test_sdkCreatTestIniFile();//OK

    test_sdkIniLoad();//OK

    sdkTestShowProcessing("�����10%...");

    test_sdkIniGetSectionNum();//OK

    sdkTestShowProcessing("�����30%...");

    test_sdkIniGetKeyNum();//OK

    test_sdkIniGetSectionName();//OK

    sdkTestShowProcessing("�����50%...");

    test_sdkIniGetKeyName();//OK

    test_sdkIniGetKeyValue();//OK

    sdkTestShowProcessing("�����70%...");

    test_sdkIniAddSection();//OK

    test_sdkIniAddKey();//OK

    test_sdkIniAddKeyValue();//OK

    sdkTestShowProcessing("�����90%...");

    test_sdkIniRemoveSectionData();//OK

    test_sdkIniRemoveSectionKey();//OK

    sdkTestShowProcessing("�����100%...");

    TestLog("TestIni", "\r\nIni���Խ���\r\n ");
    return;
}

