/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, xgd

******************************************************************************
   �� �� ��   : sdkList.c
   �� �� ��   : ����
   ��    ��   : shiweisong
   ��������   : 2012��12��27��
   ����޸�   :
   ��������   : ʵ��list����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��12��27��
    ��    ��   : ʵ��list����
    �޸�����   : �����ļ�

******************************************************************************/
#include "sdkDevOption.h"
#include "sdkGlobal.h"
#include "sdkList.h"

#ifndef  DEBUG_LIST
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


typedef struct {
    void *pData;
    void *pNext;
} ATTRIBUTE SDK_LIST_NODE;

           typedef struct {
               s32 siCnt;
               SDK_LIST_NODE *pFront;
               SDK_LIST_NODE *pEnd;
           } ATTRIBUTE SDK_SYS_LIST;
           static SDK_LIST_NODE *sSysGetNodeAt(SDK_SYS_LIST_HAND hand, s32 idx);

/*****************************************************************************
** Descriptions:	����listͷ
** Parameters:          void
** Returned value:	SDK_SYS_LIST_HAND
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
           SDK_SYS_LIST_HAND sdkSysListCreate(void)
{
    SDK_SYS_LIST_HAND pList = (SDK_SYS_LIST *) sdkGetMem(sizeof(SDK_SYS_LIST));

    if (pList != NULL) {
        memset(pList, 0, sizeof(SDK_SYS_LIST));
    }
    else {
        Assert(0);
    }
    return pList;
}

/*****************************************************************************
** Descriptions:	���������б�
** Parameters:          SDK_SYS_LIST_HAND hand �����б�ͷָ�룬�ɹ��ٵ����NULL
                    ����� ԭͷ
** Returned value:	SDK_SYS_LIST_HAND
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
SDK_SYS_LIST_HAND sdkSysListDestroy(SDK_SYS_LIST_HAND hand) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;
    SDK_LIST_NODE *pFree;
    SDK_LIST_NODE *ptemp = NULL;

    /*=======BEGIN: fusuipu 2013.03.08  15:11 modify===========*/
    if (NULL == hand) {
        return (SDK_SYS_LIST_HAND) SDK_PARA_ERR;
    }

    /*====================== END======================== */
    if (pList->siCnt == 0) {
        sdkFreeMem(hand);
        return NULL;
    }
    pFree = pList->pFront;

    while (pFree != NULL) {
        ptemp = (SDK_LIST_NODE *) pFree->pNext;
        sdkFreeMem(pFree);
        pFree = ptemp;
    }

    sdkFreeMem(hand);
    return NULL;
}

/*****************************************************************************
** Descriptions:	����list �Ƿ�Ϸ�
** Parameters:          SDK_SYS_LIST_HAND hand
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
static bool sbIsOkListHead(SDK_SYS_LIST_HAND const hand) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;

    if ((pList == NULL) || (pList->siCnt < 0)) //������������û����ô�ࡣ
    {
        return false;
    }

    if (pList->siCnt == 0) //�������Ϊ0ǰͷ��βָ�붼Ϊ��
    {
        if ((pList->pEnd != NULL) || (pList->pFront != NULL)) {
            return false;
        }
    }
    else if (pList->siCnt == 1) //�������Ϊ1��ôͷβָ��ض���ͬһ�����
    {
        if ((pList->pEnd != pList->pFront) || (pList->pEnd == NULL) ||
            (pList->pEnd->pNext != NULL)) {
            return false;
        }
    }
    else //����ж�����
    {
        int conter = 0;
        SDK_LIST_NODE *pTemp = pList->pFront;

        if ((pList->pEnd->pNext != NULL) || (pList->pFront->pNext == NULL)) {
            return false;
        }

        while (pTemp != NULL) {
            conter++;
            pTemp = (SDK_LIST_NODE *) pTemp->pNext;
        }

        return conter == sdkSysListGetCnt(hand);
    }
    return true;
}

/*****************************************************************************
** Descriptions:	���б��еõ�ĳλ�õĽ��
** Parameters:          SDK_SYS_LIST_HAND hand
                     s32 idx ��0��ʼ
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
static SDK_LIST_NODE *sSysGetNodeAt(SDK_SYS_LIST_HAND hand, s32 idx) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;
    SDK_LIST_NODE *pRetNode = NULL;

    if (sbIsOkListHead(pList) == false) {
        Assert(0);
        return NULL;
    }

    if (pList->siCnt <= idx)            //������������û����ô�ࡣ
    {
        Assert(0);
        return NULL;
    }

    if (idx == 0) {
        return pList->pFront;
    }
    else {
        pRetNode = pList->pFront;

        while (idx--) {
            Assert(pRetNode->pData != NULL);
            pRetNode = (SDK_LIST_NODE *) pRetNode->pNext;
        }
    }
    return pRetNode;
}

/*****************************************************************************
** Descriptions:	�õ�list��Ԫ�ظ���
** Parameters:          SDK_SYS_LIST_HAND hand
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
s32 sdkSysListGetCnt(SDK_SYS_LIST_HAND const hand) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;

    /*=======BEGIN: fusuipu 2013.03.11  8:35 modify===========*/
    if (NULL == hand)            //��������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    /*====================== END======================== */
    return pList->siCnt;
}

static bool sdkbIsListEmpty(SDK_SYS_LIST_HAND const hand) {
    return sdkSysListGetCnt(hand) == 0;
}

/*****************************************************************************
** Descriptions:	�õ�ĳλ�õ����ݾ�ָ��
** Parameters:          SDK_SYS_LIST_HAND hand
                    s32 idx
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
void *sdkSysListGetAt(SDK_SYS_LIST_HAND hand, s32 idx) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;
    SDK_LIST_NODE *pRetNode;

    if (sbIsOkListHead(pList) == false || idx < 0)            //��������
    {
        Assert(0);
        return NULL;
    }

    if (sdkSysListGetCnt(hand) <= idx)            //������������û����ô�ࡣ
    {
        Assert(0);
        return NULL;
    }
    pRetNode = sSysGetNodeAt(hand, idx);

    if (pRetNode != NULL) {
        return pRetNode->pData;
    }
    else {
        Assert(0);
    }
    return NULL;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_SYS_LIST_HAND hand
                    void  * pData
** Returned value:
** Created By:		shiweisong  2013.03.23
** Remarks:
*****************************************************************************/
static s32 sAddNodeToEmpeyList(SDK_SYS_LIST_HAND hand, void *pData) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;

    if (sbIsOkListHead(pList) == false || (pList->siCnt != 0))            //��������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }
    {
        SDK_LIST_NODE *pNewNode = (SDK_LIST_NODE *) sdkGetMem(sizeof(SDK_LIST_NODE));

        if (NULL == pNewNode) {
            Assert(0);
            return SDK_ERR;
        }
        pList->siCnt = 1;
        pNewNode->pData = pData;
        pNewNode->pNext = NULL;
        pList->pFront = pNewNode;    //��β��һ�¸�ָ���µ�
        pList->pEnd = pNewNode;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���б�ͷ������
** Parameters:          SDK_SYS_LIST_HAND hand
                     void  *const pData
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
s32 sdkSysListAddAtFront(SDK_SYS_LIST_HAND hand, void *const pData) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;

    if (sbIsOkListHead(pList) == false || NULL == pData)            //��������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkbIsListEmpty(hand) == true) {
        Verify(sAddNodeToEmpeyList(hand, pData) >= 0);
    }
    else {
        SDK_LIST_NODE *pNewNode = (SDK_LIST_NODE *) sdkGetMem(sizeof(SDK_LIST_NODE));

        if (NULL == pNewNode) {
            Assert(0);
            return SDK_ERR;
        }
        pNewNode->pNext = pList->pFront;
        pNewNode->pData = pData;            //��һ���½��

        pList->siCnt++;
        pList->pFront = pNewNode;            //��β��һ�¸�ָ���µ�
    }
    Assert(sbIsOkListHead(hand) == true);

    // Private_sdkSysListTrace(hand);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ӡlist��Ϣ
** Parameters:          SDK_SYS_LIST_HAND hand
** Returned value:
** Created By:		shiweisong  2013.03.23
** Remarks:
*****************************************************************************/
void Private_sdkSysListTrace(SDK_SYS_LIST_HAND hand) {
    int cotner = sdkSysListGetCnt(hand);
    SDK_LIST_NODE *pnote = NULL;

    while (cotner--) {
        pnote = sSysGetNodeAt(hand, cotner);
        Trace("list", " Private_sdkSysListTrace list %x: idx :%d pNote:%x pData:%x \r\n", hand,
              cotner, pnote, pnote->pData);
    }
}   //lint !e438 !e550

/*****************************************************************************
** Descriptions:	����βͷ������
** Parameters:          SDK_SYS_LIST_HAND hand
                      void  *const pData
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
s32 sdkSysListAddAtEnd(SDK_SYS_LIST_HAND hand, void *const pData) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;


    if (sbIsOkListHead(pList) == false || NULL == pData)            //��������
    {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (sdkbIsListEmpty(hand) == true) {
        Verify(sAddNodeToEmpeyList(hand, pData) >= 0);
    }
    else {
        SDK_LIST_NODE *pNewNode = (SDK_LIST_NODE *) sdkGetMem(sizeof(SDK_LIST_NODE));

        if (NULL == pNewNode) {
            Assert(0);
            return SDK_ERR;
        }
        Assert((pList->pEnd->pNext == NULL));

        pNewNode->pNext = NULL;
        pNewNode->pData = pData;            //��һ���½��
        pList->siCnt++;
        /*=======BEGIN: fusuipu 2012.12.31  9:17 modify===========*/
        pList->pEnd->pNext = pNewNode;
        pList->pEnd = pNewNode;
        /*====================== END======================== */
    }
    Assert(sbIsOkListHead(hand) == true);
    //Private_sdkSysListTrace(hand);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ָ��λ��ɾ�����
** Parameters:          SDK_SYS_LIST_HAND hand
                        s32 idx  ��0��ʼ
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
s32 sdkSysListRemoveAt(SDK_SYS_LIST_HAND hand, s32 idx) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;

    SDK_LIST_NODE *pRemovNode = NULL;


    if (sbIsOkListHead(pList) == false || ((pList->siCnt == 1) && (idx != 0))) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (pList->siCnt == 1) //����Ѿ������һ����
    {
        Assert(idx == 0);  //��������һ������ô��������Ϊ0
        pList->siCnt--;

        sdkFreeMem(pList->pFront);
        pList->pFront = NULL;
        pList->pEnd = NULL;
    }
    else if (idx == 0) {
        pList->siCnt--;
        pRemovNode = pList->pFront;
        pList->pFront = (SDK_LIST_NODE *) pList->pFront->pNext;            //��β��һ�¸�ָ���µ�
        Assert(pRemovNode != NULL);
        sdkFreeMem(pRemovNode);
    }
    else {
        SDK_LIST_NODE *pPreNode;
        pRemovNode = sSysGetNodeAt(hand, idx);
        pPreNode = sSysGetNodeAt(hand, idx - 1);
        Assert(pRemovNode != NULL);
        Assert(pPreNode != NULL);

        if (sdkSysListGetCnt(hand) == (idx + 1)) //if del the end node
        {
            pList->pEnd = pPreNode;
        }
        pList->siCnt--;
        pPreNode->pNext = pRemovNode->pNext;
        sdkFreeMem(pRemovNode);
    }

    if (1 == pList->siCnt) {
        pList->pEnd = pList->pFront;
    }
    Assert(sbIsOkListHead(pList) == true);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	���б���ɾ������ĳ�����ݵĽ��
** Parameters:          SDK_SYS_LIST_HAND hand
                    void const *pData
** Returned value:
** Created By:		shiweisong  2012.12.28
** Remarks:
*****************************************************************************/
s32 sdkSysListRemoveData(SDK_SYS_LIST_HAND hand, void const *pData) {
    SDK_SYS_LIST *pList = (SDK_SYS_LIST *) hand;
    s32 Index;

    if (sbIsOkListHead(pList) == false || pData == NULL) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    Assert(sdkbIsListEmpty(hand) == false);

    for (Index = 0; Index < pList->siCnt; Index++) {
        SDK_LIST_NODE *pRemovNode = sSysGetNodeAt(hand, Index);

        if (pRemovNode->pData == pData) {
            return sdkSysListRemoveAt(hand, Index);
        }
    }

    Assert(sbIsOkListHead(hand) == true);
    return SDK_ERR;
}

