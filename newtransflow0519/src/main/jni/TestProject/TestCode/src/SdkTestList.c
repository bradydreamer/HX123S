#include "sdkGlobal.h"
#include "SdkTest.h"


#define LISTCODENUM 20      //������ͷ���ӽ��ĸ���
#if 0 /*Modify by huacong at 2013.03.21  10:41 */
#if 0 /*Modify by huacong at 2013.03.20  10:22 */
typedef struct
{
    void * pData;
    void *pNext;
}SDK_LIST_NODE;

typedef struct
{
    s32 siCnt;
    SDK_LIST_NODE *pFront;
    SDK_LIST_NODE *pEnd;
}SDK_SYS_LIST;
#endif /* if 0 */
typedef  void * SDK_SYS_LIST_HAND;
SDK_SYS_LIST_HAND gHand, gOtherList;

s32 gListCodeNum = 0, gOtherListNum = 0;                   //��������Ľ������


//u8 *p_first, *p_scend, *p_third, *p_four, *p_five, *p_sixe;
u8 *q_first, *q_scend, *q_third, *q_four, *q_five;

/*****************************************************************************
** Descriptions:	��������ͷ����,������һ��ȫ������
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListCreate(void)
{
    TestLog("Test_sdkSysListCreate", "\r\n Test_sdkSysListCreate starts\r\n");

    /*��������������в���*/
    (SDK_SYS_LIST_HAND)gHand = (SDK_SYS_LIST_HAND)sdkSysListCreate();
    (SDK_SYS_LIST_HAND)gOtherList = (SDK_SYS_LIST_HAND)sdkSysListCreate();

    /*��ʼ����ָ�븳ֵ*/
    p_first = (u8 *)sdkGetMem(sizeof(char) * 12);
    p_scend = (u8 *)sdkGetMem(sizeof(char) * 12);
    p_third = (u8 *)sdkGetMem(sizeof(char) * 12);
    p_four  = (u8 *)sdkGetMem(sizeof(char) * 12);
    p_five  = (u8 *)sdkGetMem(sizeof(char) * 12);
    p_sixe  = (u8 *)sdkGetMem(sizeof(char) * 12);

    q_first = (u8 *)sdkGetMem(sizeof(char) * 12);
    q_scend = (u8 *)sdkGetMem(sizeof(char) * 12);
    q_third = (u8 *)sdkGetMem(sizeof(char) * 12);
    q_four  = (u8 *)sdkGetMem(sizeof(char) * 12);
    q_five  = (u8 *)sdkGetMem(sizeof(char) * 12);

    /*�ж϶�̬���벻�ɹ�*/
    if(p_first == NULL || p_scend == NULL || p_third == NULL || p_four == NULL ||
       p_five == NULL  || p_sixe == NULL  || q_first == NULL || q_scend == NULL ||
       q_third == NULL || q_four == NULL  || q_five == NULL)
    {
        Assert(0);
        TestLog("Test_sdkSysListCreate", "\r\n get sdkGetMem failed  \r\n");
        return;
    }
    strcpy(p_first, "first");
    strcpy(p_scend, "scend");
    strcpy(p_third, "third");
    strcpy(p_four,  "four");
    strcpy(p_five,  "five");
    strcpy(p_sixe,  "sixe");

    strcpy(q_first, "first");
    strcpy(q_scend, "scend");
    strcpy(q_third, "third");
    strcpy(q_four,  "four");
    strcpy(q_five,  "five");

    TestLog("Test_sdkSysListCreate", "\r\n Test_sdkSysListCreate ends  \r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListGetCnt(void)
{
    TestLog("Test_sdkSysListGetCnt", "\r\n Test_sdkSysListGetCnt starts\r\n");

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(gHand) == gListCodeNum);
    printf("glistcodenum is %d\r\n", gListCodeNum);

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(gOtherList) == gOtherListNum);

    TestLog("Test_sdkSysListGetCnt", "\r\n Test_sdkSysListGetCnt ends\r\n");
}

/*****************************************************************************
** Descriptions:	������ͷ�������µĽ��
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListAddAtFront(void)
{
    TestLog("Test_sdkSysListAddAtFront", "\r\n Test_sdkSysListAddAtFront starts\r\n");

    u8 *p = (u8 *)sdkGetMem(sizeof(char) * 12);
    u8 *p2 = NULL;
    SDK_SYS_LIST_HAND hand = NULL;

    if(p == NULL)
    {
        Assert(0);
        TestLog("Test_sdkSysListAddAtFront", "\r\n get sdkGetMem failed  \r\n");
        return;
    }
    strcpy(p, "123456");
    TEST_IS_TRUE_WITH_INFO("������ͷ������Ϊ��ָ�����", sdkSysListAddAtFront(hand, p) == SDK_PARA_ERR);

    //TEST_IS_TRUE_WITH_INFO("������ͷ������Ϊ��ָ�����", sdkSysListAddAtFront(gHand, p2) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("������ͷ�������ʼ�������", sdkSysListAddAtFront(gHand, p_first) == SDK_OK);
    gListCodeNum++;

    TEST_IS_TRUE_WITH_INFO("������ͷ������������������", sdkSysListAddAtFront(gHand, p_scend) == SDK_OK);
    gListCodeNum++;

    TEST_IS_TRUE_WITH_INFO("������ͷ������������������", sdkSysListAddAtFront(gHand, p_third) == SDK_OK);
    gListCodeNum++;

    sdkFreeMem(p);
    p = NULL;
    TestLog("Test_sdkSysListAddAtFront", "\r\n Test_sdkSysListAddAtFront ends\r\n");
}

/*****************************************************************************
** Descriptions:	������β�������µĽ��
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListAddAtEnd(void)
{
    TestLog("Test_sdkSysListAddAtEnd", "\r\n Test_sdkSysListAddAtEnd starts\r\n");

    u8 *p = (u8 *)sdkGetMem(sizeof(char) * 12);
    u8 *p1 = NULL;
    SDK_SYS_LIST_HAND hand = NULL;

    if(p == NULL)
    {
        Assert(0);
        TestLog("Test_sdkSysListCreate", "\r\n get sdkGetMem failed  \r\n");
    }
    strcpy(p, "123");
    TEST_IS_TRUE_WITH_INFO("������β������Ϊ��ָ�����", sdkSysListAddAtEnd(hand, p) == SDK_PARA_ERR);

    //TEST_IS_TRUE_WITH_INFO("������β������Ϊ��ָ�����ݲ���", sdkSysListAddAtEnd(hand, p1) == SDK_PARA_ERR);

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gHand, p_four) == SDK_OK);
    gListCodeNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gHand, p_five) == SDK_OK);
    gListCodeNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gHand, p_sixe) == SDK_OK);
    gListCodeNum++;

    TEST_IS_TRUE_WITH_INFO("������β�������ʼ�������", sdkSysListAddAtEnd(gOtherList, q_first) == SDK_OK);
    gOtherListNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gOtherList, q_scend) == SDK_OK);
    gOtherListNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gOtherList, q_third) == SDK_OK);
    gOtherListNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gOtherList, q_four) == SDK_OK);
    gOtherListNum++;

    TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(gOtherList, q_five) == SDK_OK);
    gOtherListNum++;

    sdkFreeMem(p);
    p = NULL;
    TestLog("Test_sdkSysListAddAtEnd", "\r\n Test_sdkSysListAddAtEnd ends\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListDestroy(void)
{
    TestLog("Test_sdkSysListDestroy", "\r\n Test_sdkSysListDestroy starts\r\n");

    SDK_SYS_LIST_HAND hand = NULL;

    TEST_IS_TRUE_WITH_INFO("�ݻ���������-������ָ�����", sdkSysListDestroy(hand) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("�ݻ���������-ȫ���������", (SDK_SYS_LIST_HAND)sdkSysListDestroy(gHand) == NULL);
    TEST_IS_TRUE_WITH_INFO("�ݻ���������-ȫ���������", (SDK_SYS_LIST_HAND)sdkSysListDestroy(gOtherList) == NULL);

    TestLog("Test_sdkSysListDestroy", "\r\n sdkFreeMem zhizhen\r\n");

    /*�ͷ�ȫ��ָ��,����ָ��ָ��NULL*/
    sdkFreeMem(p_first);
    sdkFreeMem(p_scend);
    sdkFreeMem(p_third);
    sdkFreeMem(p_four);
    sdkFreeMem(p_five);
    sdkFreeMem(p_sixe);
    sdkFreeMem(q_first);
    sdkFreeMem(q_scend);
    sdkFreeMem(q_third);
    sdkFreeMem(q_four);
    sdkFreeMem(q_five);

    gListCodeNum = 0;               //ȫ�ֱ����ǵ�Ҫ��0
    gOtherListNum = 0;
    TestLog("Test_sdkSysListDestroy", "\r\n Test_sdkSysListDestroy ends\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListGetAt(void)
{
    TestLog("Test_sdkSysListGetAt", "\r\n Test_sdkSysListGetAt starts\r\n");

    SDK_SYS_LIST_HAND hand = NULL;

    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-������ָ�����", (void *)sdkSysListGetAt(hand, 1) == NULL);
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-�Ҳ�����������", (void *)sdkSysListGetAt(gHand, 100) == NULL);
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-������ͷ����", (void *)sdkSysListGetAt(gHand, 0) != NULL);
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-������β����", (void *)sdkSysListGetAt(gHand, (gListCodeNum - 1)) != NULL);
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-�������м�λ�ò���", (void *)sdkSysListGetAt(gHand, 5) != NULL);

    TestLog("Test_sdkSysListGetAt", "\r\n Test_sdkSysListGetAt ends\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListRemoveAt(void)
{
    TestLog("Test_sdkSysListRemoveAt", "\r\n Test_sdkSysListRemoveAt starts\r\n");

    SDK_SYS_LIST_HAND hand = NULL;

    TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý�������ָ�����", sdkSysListRemoveAt(hand, 5) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý��ɾ��β������", sdkSysListRemoveAt(gHand, (gListCodeNum - 1)) == SDK_OK);
    gListCodeNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý��ɾ��ͷ������", sdkSysListRemoveAt(gHand, 0) == SDK_OK);
    gListCodeNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý���м����", sdkSysListRemoveAt(gHand, 2) == SDK_OK);
    gListCodeNum--;

    TestLog("Test_sdkSysListRemoveAt", "\r\n Test_sdkSysListRemoveAt ends\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkSysListRemoveNode(void)
{
    TestLog("Test_sdkSysListRemoveNode", "\r\n Test_sdkSysListRemoveNode starts\r\n");

    SDK_SYS_LIST_HAND hand = NULL;
    u8 *p = NULL;
    s32 ret1 = 0, ret2 = 0, ret3 = 0;
    u8 *p1 = sdkGetMem(sizeof(char) * 12);

    if(p1 == NULL)
    {
        Assert(0);
        TestLog("Test_sdkSysListRemoveNode", "\r\n get sdkGetMem failed  \r\n");
    }
    strcpy(p1, "123456");
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ������ݲ���", sdkSysListRemoveData(gHand, p) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ�������ָ�����", sdkSysListRemoveData(hand, p) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ���Ҳ������ݲ���", (ret1 = sdkSysListRemoveData(gHand, p1)) == SDK_ERR);

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", (ret2 = sdkSysListRemoveData(gHand, p_scend)) == SDK_OK);
    gListCodeNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", (ret3 = sdkSysListRemoveData(gHand, p_five)) == SDK_OK);
    gListCodeNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", sdkSysListRemoveData(gOtherList, q_first) == SDK_OK);
    gOtherListNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", sdkSysListRemoveData(gOtherList, q_scend) == SDK_OK);
    gOtherListNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", sdkSysListRemoveData(gOtherList, q_third) == SDK_OK);
    gOtherListNum--;

    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", sdkSysListRemoveData(gOtherList, q_four) == SDK_OK);
    gOtherListNum--;

    //TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������", sdkSysListRemoveData(gOtherList, q_five) == SDK_OK);
    //gOtherListNum--;

    printf("ret1 is %d\r\n ret2 is %d\r\n ret3 is %d\r\n", ret1, ret2, ret3);

    sdkFreeMem(p1);
    p1 = NULL;
    TestLog("Test_sdkSysListRemoveNode", "\r\n Test_sdkSysListRemoveNode ends\r\n");
}

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.20
** Remarks:
*****************************************************************************/
void Test_sdkList(void)
{
    TestLog("Test_sdkListAuto", "\r\n Test_sdkListAuto starts\r\n");

    Test_sdkSysListCreate();            //����������̬����ȫ��ָ��
    Test_sdkSysListGetCnt();            //�˶����������

    Test_sdkSysListAddAtFront();        //��ͷ�����ӽ��
    Test_sdkSysListGetCnt();            //�ٴκ˶Խ�����

    Test_sdkSysListAddAtEnd();          //��β�����ӽ��
    Test_sdkSysListGetCnt();            //�ٴκ˶Խ�����

    Test_sdkSysListGetAt();             //�����������ҽ��

    Test_sdkSysListRemoveAt();          //ͨ��λ������ɾ�����
    Test_sdkSysListGetCnt();             //�ٴκ˶Խ�����

    Test_sdkSysListRemoveNode();        //ͨ������ָ��ɾ�����
    Test_sdkSysListGetCnt();             //�ٴκ˶Խ�����

    Test_sdkSysListDestroy();           //�ݻ����������ͷ�ȫ��ָ��

    TestLog("Test_sdkListAuto", "\r\n Test_sdkListAuto ends  \r\n");
}

#endif /* if 0 */

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		huacong  2013.03.21
** Remarks:
*****************************************************************************/
void Test_sdkListAuto(void) {
    SDK_SYS_LIST_HAND hand, otherhand = NULL;
    s32 sListNum = 0;
    s32 i = 0;
    u8 j = 0;
    u8 *p = NULL, *p_error, *p_temp;
    u8 *p_hand[50];

    /*1.��ʼ��ָ�룬��̬�����ڴ�*/
    for (i = 0; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        p_hand[i] = (u8 *) sdkGetMem(sizeof(char) * (i + 5));

        if (p_hand[i] == NULL) {
            Assert(0);
            TestLog("Test_sdkSysListCreate", "\r\n get sdkGetMem failed  \r\n");
            return;
        }
    }

    p_error = (u8 *) sdkGetMem(sizeof(char) * 10);

    if (p_error == NULL) {
        Assert(0);
        TestLog("Test_sdkSysListCreate", "\r\n get sdkGetMem failed  \r\n");
        return;
    }

    /*2.��ָ�븳ֵ*/
    for (i = 0; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        sprintf(p_hand[i], "%c", j);
        j++;
    }

    strcpy(p_error, "abc");

    /*3.���������������������*/
    hand = (SDK_SYS_LIST_HAND) sdkSysListCreate();
    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(hand) == sListNum);

    /*4.������ͷ�ӽ�㲢���Խ�����*/
    TEST_IS_TRUE_WITH_INFO("������ͷ������Ϊ��ָ�����", sdkSysListAddAtFront(hand, p) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("������ͷ������Ϊ���������",
                           sdkSysListAddAtFront(otherhand, p_error) == SDK_PARA_ERR);
    sdkTestShowProcessing("�����10%...");
    for (i = 0; i < LISTCODENUM; i++) {
        TEST_IS_TRUE_WITH_INFO("������ͷ�������ʼ�������",
                               sdkSysListAddAtFront(hand, p_hand[i]) == SDK_OK);
        sListNum++;
    }

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(hand) == sListNum);


    /*5.������β���Ͻ�㲢���Խ�����*/
    for (i = LISTCODENUM; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        TEST_IS_TRUE_WITH_INFO("������β���������", sdkSysListAddAtEnd(hand, p_hand[i]) == SDK_OK);
        sListNum++;
    }

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(hand) == sListNum);


    /*6.����ָ��λ�ý������*/
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-������ָ�����",
                           (void *) sdkSysListGetAt(otherhand, 1) == NULL);
    TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ��-�Ҳ�����������",
                           (void *) sdkSysListGetAt(hand, 1000) == NULL);

    for (i = 0; i < LISTCODENUM; i++) {
        p_temp = (u8 *) sdkSysListGetAt(hand, i);
        TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ�����",
                               memcmp(p_temp, p_hand[LISTCODENUM - 1 - i], 1) == 0);
    }

    for (i = LISTCODENUM; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        p_temp = (u8 *) sdkSysListGetAt(hand, i);
        TEST_IS_TRUE_WITH_INFO("��ȡָ��λ������ָ�����", memcmp(p_temp, p_hand[i], 1) == 0);
    }

    sdkTestShowProcessing("�����50%...");
    /*7.ɾ��ָ��λ�ý�㲢���Խ�����*/
    TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý�������ָ�����",
                           sdkSysListRemoveAt(otherhand, 5) == SDK_PARA_ERR);

    for (i = 0; i < LISTCODENUM; i++) {
        TEST_IS_TRUE_WITH_INFO("ɾ��ָ��λ�ý�����", sdkSysListRemoveAt(hand, 0) == SDK_OK);
        sListNum--;
    }

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(hand) == sListNum);

    /*8.ɾ����������ָ��Ľ��*/
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ������ݲ���", sdkSysListRemoveData(hand, p) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ�������ָ�����",
                           sdkSysListRemoveData(otherhand, p_error) == SDK_PARA_ERR);
    TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ���Ҳ������ݲ���",
                           sdkSysListRemoveData(hand, p_error) == SDK_ERR);

    for (i = LISTCODENUM; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        TEST_IS_TRUE_WITH_INFO("ɾ���������ݵĽ����������",
                               sdkSysListRemoveData(hand, p_hand[i]) == SDK_OK);
        sListNum--;
    }

    TEST_IS_TRUE_WITH_INFO("����Ԫ�ظ�������", sdkSysListGetCnt(hand) == sListNum);

    /*9.�ݻ���������*/
    //   TEST_IS_TRUE_WITH_INFO("�ݻ���������-������ָ�����", sdkSysListDestroy(otherhand) == SDK_PARA_ERR);
//    TEST_IS_TRUE_WITH_INFO("�ݻ���������-ȫ���������", (SDK_SYS_LIST_HAND)sdkSysListDestroy(hand) == NULL);

    /*10.������֮���ͷŶ�̬�����ָ��*/
    for (i = 0; i < sizeof(p_hand) / sizeof(u8 * ); i++) {
        sdkFreeMem(p_hand[i]);
        p_hand[i] = NULL;
    }

    sdkFreeMem(p_error);
    p_error = NULL;
    sListNum = 0;
}

