#include "sdkGlobal.h"

SDK_SYS_LIST_HAND mDispItemList;     // �ɼ��˵�list

typedef struct _DISPITEM {
    s32 siIndex;                           //�󶨵�key
    s32 siID;                                //ID
    s32 siMaxLine;                         //ÿҳ�������
} DISPITEM;


typedef struct _DISPNODE {
    u8 *DispStr;                         //����
    s32 siNode;                             //��ǰҳ��
    s32 siID;                               //��ǰ�˵��ڵ���
} DISPNODE;

#define SDK_DISP_MSG_BOX_NOT_RET     (SDK_SYS_RET_ERR_MAX - 500)

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.24
** Remarks:
*****************************************************************************/
static s32 sdkDispFreeItemNode(void) {
    DISPNODE *item_node;
    s32 item_node_num = 0;
    s32 i = 0;

    if (NULL != mDispItemList) {
        item_node_num = sdkSysListGetCnt(mDispItemList);         //���ͷſɼ��˵�

        for (i = 0; i < item_node_num; i++) {
            item_node = sdkSysListGetAt(mDispItemList, i);

            if (NULL == item_node) {
                Assert(0);
                return SDK_ERR;
            }
            else {
                if (item_node->DispStr != NULL) {
                    sdkFreeMem(item_node->DispStr);
                    item_node->DispStr = NULL;
                }
                sdkFreeMem(item_node);
            }
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:    �����ʾ���ݵĴ洢�б�
** Parameters:      Input: None
** Returned value:
** Created By:		fusuipu  2013.05.24
** Remarks:
*****************************************************************************/
static DISPITEM gDispItem = {0, 0, 0};

static s32 sdkDispInitItems(void) {
    Verify((mDispItemList = sdkSysListCreate()) != NULL);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          u8 *pStr
** Returned value:
** Created By:		fusuipu  2013.07.10
** Remarks:
*****************************************************************************/
static s32 sdkDispAddArrow(const u8 *pStr) {
    s32 item_node_num = 0;
    s32 item_per_page = 0;

    if (mDispItemList == NULL || NULL == pStr || strlen(pStr) == 0) {
        return SDK_PARA_ERR;
    }
    item_node_num = sdkSysListGetCnt(mDispItemList);    //lint !e838
    item_per_page = sdkSysGetSreenMaxLine() - 1;        //lint !e838

    if (item_node_num > item_per_page)    //����һҳ
    {
        if (gDispItem.siIndex + item_per_page <= item_per_page) {
            Verify(sdkDispFillRowRam(0, 0, "��", SDK_DISP_RIGHT_DEFAULT) >= 0);
        }
        else if (gDispItem.siIndex + item_per_page >= item_node_num) {
            Verify(sdkDispFillRowRam(0, 0, "��", SDK_DISP_RIGHT_DEFAULT) >= 0);
        }
        else {
            Verify(sdkDispFillRowRam(0, 0, "����", SDK_DISP_RIGHT_DEFAULT) >= 0);
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	          ��ʾ����
** Parameters:          const u8*pasTitle
** Returned value:
** Created By:		shiweisong  2013.02.04
** Remarks:
*****************************************************************************/
static bool sShowTitle(const u8 *pasTitle) {
    if (pasTitle != NULL) {
        if (strlen(pasTitle) > 0) {
            Verify(sdkDispClearRowRam(SDK_DISP_LINE1) >= 0);
            Verify(sdkDispFillRowRam(SDK_DISP_LINE1, 0, pasTitle, SDK_DISP_DEFAULT) >= 0);
            sdkDispAddArrow(pasTitle);
            return true;
        }
    }
    return false;
}

/*****************************************************************************
** Descriptions:	��������Ҫ��ʾ���ַ����Ľڵ�һ��һ������ӵ�����
** Parameters:          u8 *pStr
                    s32 siItemNo
                    s32 siLineNo
** Returned value:
** Created By:		fusuipu  2013.05.27
** Remarks:
*****************************************************************************/
static s32 sdkDispStoreLineStrToList(const u8 *pStr, s32 siItemNo, s32 siLineNo) {
    DISPNODE *itemnode = NULL;
    s32 len = 0;

    if (NULL == pStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    len = strlen(pStr);     //lint !e838

    if (len > 128) {
        len = 128;
    }
    itemnode = (DISPNODE *) sdkGetMem(sizeof(DISPNODE)); //lint !e838
    if (NULL == itemnode) {
        Assert(0);
        return SDK_ERR;
    }

    itemnode->DispStr = sdkGetMem(len + 1);
    if (NULL == itemnode->DispStr) {
        Assert(0);
        sdkFreeMem(itemnode);
        return SDK_ERR;
    }
    memset(itemnode->DispStr, 0, len + 1);
    strncpy(itemnode->DispStr, pStr, len);
    TraceHex("fsp", "DispStoreStr:", itemnode->DispStr, strlen(itemnode->DispStr));
    itemnode->siNode = siItemNo;
    itemnode->siID = siLineNo;
    Verify(sdkSysListAddAtEnd(mDispItemList, itemnode) == SDK_OK);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�ֽ�洢��ʾ�ַ�����һ��һ�е�д��ڵ�
** Parameters:      u8 *paStr
** Returned value:
** Created By:		fusuipu  2013.05.24
** Remarks:
*****************************************************************************/
static s32 sdkDispParaStr(const u8 *paStr) {
    u8 buf[256] = {0};
    s32 len = 0;
    s32 nodenum = 0;
    s32 id = 0;
    bool is_new_node = true;
    u8 max_byte = 0;            //һ�������ֽڸ���
    s32 real_byte = 0;           //Ŀǰ�ַ�����������������س���֮ǰ�����ֽڸ���
    s32 max_letter_per_line = Private_sdkMulitGetLcdOneLineLetterMaxNum(true);

    if (paStr == NULL) {
        return SDK_PARA_ERR;
    }

    Trace("fsppro", "max_letter_per_line = %d\r\n", max_letter_per_line);
    if ((*paStr == '\r') || (*paStr == '\n'))          //һ��ʼ�������˽�������Ҫ�洢�˽�����
    {
        memset(buf, 0, sizeof(buf));
        buf[0] = *paStr;
        sdkDispStoreLineStrToList(buf, nodenum, id);
        nodenum++;
        id++;
    }

    while ((*paStr == '\r') || (*paStr == '\n')) {
        paStr++;
    }

    while (*paStr != 0) {
        max_byte = (u8)((true == is_new_node) ? max_letter_per_line : max_letter_per_line -
                                                                      2); //��ȡ��ǰ���������ʾ���ַ�����

        len = Private_sdkMulitGetLcdOneLineLetterMaxLen(paStr,
                                                        max_byte);     //���㵱ǰ�ַ����������������ʹ�֮ǰ���ܻ�ȡ�Ĳ����� max_byte������ֽڸ���

        memset(buf, 0, sizeof(buf));
        real_byte = sdkMultiGetStrNumAsHalfAgle(paStr, true);

        if (real_byte <= 0) {
            return SDK_ERR;
        }
        Trace("fsppro", "max_byte = %d\r\nlen = %d\r\nreal_byte = %d\r\n", max_byte, len,
              real_byte);
        TraceHex("fsppro", "paStr:", paStr, strlen(paStr));

        if (real_byte >=
            max_byte)                                                           //�����ǰ�ַ�������һ������ʾ����ַ���
        {
            memset(buf, 0, sizeof(buf));

            if (max_byte == max_letter_per_line -
                            2)                             //һ���ַ����ܳ����سɶ���֮�󣬵�һ�����룬�ڶ��п�ͷӦ�ÿ�2���ַ�
            {
                memset(buf, 0x20, 2);
                memcpy(&buf[2], paStr, len);
            }
            else {
                memcpy(buf, paStr, len);
            }
            is_new_node = false;
            Trace("fsppro", "buf = %s\r\n", buf);
            sdkDispStoreLineStrToList(buf, nodenum, id);
            id++;                                         //����++

            if ((paStr[len] == '\r') || (paStr[len] == '\n'))        //�����20���ַ�����ž��ǻ��з���
            {
                nodenum++;                                //��ǰ����++;
                is_new_node = true;

                while ((paStr[len] == '\r') || (paStr[len] == '\n'))   //������/r/n��ϵĽ�������
                {
                    len++;                                //ָ��������ƫ��
                }
            }
            paStr += len;
        }
        else {
            memset(buf, 0, sizeof(buf));

            if (false == is_new_node) {
                memset(buf, 0x20, 2);                     //�ַ������л���֮�������˽���������Ҫ���������
                memcpy(&buf[2], paStr, len);
            }
            else                                          //����������������ֱ�������
            {
                memcpy(buf, paStr, len);
            }
            Trace("fsppro", "buf2 = %s\r\n", buf);
            sdkDispStoreLineStrToList(buf, nodenum, id);
            nodenum++;                                    //�ڵ���+1
            id++;                                         //ֻҪ����������
            is_new_node = true;

            while ((paStr[len] == '\r') || (paStr[len] == '\n')) {
                len++;
            }

            paStr += len;                                 //��¼��һ�е���ʼλ��
        }
    }

    return nodenum;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_SYS_LIST_HAND hand
                               const u8 *pTitle
** Returned value:
** Created By:		fusuipu  2014.01.14
** Remarks:
*****************************************************************************/
#ifdef XGD_SDK_DEBUG /*Modify by zhujinping at 2014.06.09  17:22 */
static s32 getDispSum(SDK_SYS_LIST_HAND hand, const u8 *pTitle)
{
    s32 conter = sdkSysListGetCnt(hand);
    s32 ret = 0;
    
    if(pTitle != NULL)
    {
        s32 len2 = strlen(pTitle);
        while(len2--)
        {
            ret += pTitle[len2];
        }
    }

    while(conter--)
    {
        DISPNODE *pnode = sdkSysListGetAt(hand, conter);
        s32 len = strlen(pnode->DispStr);

        ret += pnode->siID;
        ret += pnode->siNode;

        while(len--)
        {
            ret += pnode->DispStr[len];
        }
    }

    return ret;
}
#endif

/*****************************************************************************
** Descriptions:
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.07.10
** Remarks:             �����ϴεļ�¼
*****************************************************************************/
static s32 sdkDispIsNewStr(const u8 *pTitle) {
#ifndef XGD_SDK_DEBUG /*Modify by zhujinping at 2014.06.09  17:22 */
    memset(&gDispItem, 0, sizeof(gDispItem));
    return SDK_OK;
#else
    struct Save
    {
        s32 sum;
        s32 siId;
        s32 siIndex;
    };
    static s32 LastSum = 0;
    static struct Save store[4] = {0};
    struct Save temp_node = {0};
    s32 curSum = 0;
    s32 saveconter = sizeof(store) / sizeof(struct Save);
    u8 flag = 0;

    if(NULL == mDispItemList || sdkSysListGetCnt(mDispItemList) <= 0)
    {
        Trace( " mDispItemList = 0x%x, sdkSysListGetCnt(mDispItemList) = %d\r\n", mDispItemList, sdkSysListGetCnt(mDispItemList) );
        return SDK_ERR;
    }
    curSum = getDispSum(mDispItemList, pTitle);     //lint !e838

    saveconter = sizeof(store) / sizeof(struct Save); //lint !e838

    while(--saveconter)
    {
        if(store[saveconter].sum == curSum)
        {
            temp_node.siId = store[saveconter].siId;
            temp_node.siIndex = store[saveconter].siIndex;
            flag = 1;
            break;
        }
    }
    
    
    store[0] = store[1];
    store[1] = store[2];
    store[2] = store[3];
    store[3].sum = LastSum;
    store[3].siId = gDispItem.siID;
    store[3].siIndex = gDispItem.siIndex;
    LastSum = curSum;
    memset(&gDispItem, 0, sizeof(gDispItem));
    
    if(1 == flag)
    {
        gDispItem.siID = temp_node.siId;
        gDispItem.siIndex = temp_node.siIndex;
        return SDK_OK;
    }
    return SDK_ERR;

#endif /* if 0 */
}

/*****************************************************************************
** Descriptions:	׼����ʾ����
** Parameters:          u8 *psStr
** Returned value:
** Created By:		fusuipu  2013.05.28
** Remarks:
*****************************************************************************/
static s32 sdkDispReadyStr(const u8 *psStr, const u8 *pTitle) {
    if (NULL == psStr) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispInitItems();         //�������ʾ�б�
    sdkDispParaStr(psStr);      //�ֽ�洢��ʾ�ַ�����ÿһ����Ϊһ���ڵ������ʾ�б�
    sdkDispIsNewStr(pTitle);
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰ�ַ�����ʾģʽ
** Parameters:          &disp_mode
** Returned value:
** Created By:		fusuipu  2013.05.24
** Remarks:
*****************************************************************************/
static s32 sdkDispGetDispMode(const DISPNODE *ItemNode, u32 *disp_mode, bool isNoFDisp,
                              const s32 siDispMode) {
    DISPNODE *item_node = NULL;

    item_node = sdkSysListGetAt(mDispItemList, gDispItem.siID); //lint !e838
    (*disp_mode) = SDK_DISP_LEFT_DEFAULT;

    if (-1 != siDispMode) {
        *disp_mode = (u32) siDispMode;
    }

    if (ItemNode->siNode == item_node->siNode && isNoFDisp == true) {
        (*disp_mode) &= (~SDK_DISP_FDISP);             //����
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡ��ʾģʽ
** Parameters:          s32 siTileLine
                    bool isNoFDisp
** Returned value:
** Created By:		fusuipu  2013.05.28
** Remarks:
*****************************************************************************/
static s32 sdkDispPutStrInRom2(s32 siStartLine, s32 siEndLine, const s32 siDispMode) {
    s32 i = 0, j = 0;
    s32 max_line = sdkSysGetSreenMaxLine() - siStartLine;    //��ȥ���⣬�����ʾ����
    s32 node_num = 0;
    DISPNODE *item_node = NULL;

    max_line = siEndLine + 1 - siStartLine;  //lint !e838

    if (siEndLine + 1 > sdkSysGetSreenMaxLine()) {
        max_line = sdkSysGetSreenMaxLine() - siStartLine;
    }
    gDispItem.siMaxLine = max_line;                          //���µ�ǰģʽÿҳ�������
    node_num = sdkSysListGetCnt(mDispItemList); //lint !e838

    for (i = 0; i < max_line; i++)   //lint !e838
    {
        j = i + gDispItem.siIndex;

        if (j >= node_num) {
            break;
        }
        item_node = sdkSysListGetAt(mDispItemList, j);

        if (NULL == item_node) {
            Assert(0);
            return SDK_ERR;
        }

        if (0 == strlen(item_node->DispStr)) {
            Assert(0);
            return SDK_ERR;
        }
        sdkDispFillRowRam(siStartLine++, 0, item_node->DispStr, (u32) siDispMode);
    }

    return i;
}

/*****************************************************************************
** Descriptions:	��ȡ��ʾģʽ
** Parameters:          s32 siTileLine
                    bool isNoFDisp
** Returned value:
** Created By:		fusuipu  2013.05.28
** Remarks:
*****************************************************************************/
static s32 sdkDispPutStrInRom(s32 siStartLine, s32 siEndLine, bool isNoFDisp,
                              const s32 siDispMode) {
    s32 i = 0, j = 0;
    s32 max_line = 0;    //��ȥ���⣬�����ʾ����
    u32 disp_mode = 0;
    s32 node_num = 0;
    DISPNODE *item_node = NULL;

    max_line = siEndLine + 1 - siStartLine;     //lint !e838

    if (siEndLine + 1 > sdkSysGetSreenMaxLine()) {
        max_line = sdkSysGetSreenMaxLine() - siStartLine;
    }
    gDispItem.siMaxLine = max_line;                          //���µ�ǰģʽÿҳ�������
    node_num = sdkSysListGetCnt(mDispItemList); //lint !e838

    Trace("zjp", "sdkDispPutStrInRom:max_line = %d\r\n", max_line);
    Trace("zjp", "sdkDispPutStrInRom:node_num = %d\r\n", node_num);
    Trace("zjp", "sdkDispPutStrInRom:siStartLine = %d\r\n", siStartLine);
    for (i = 0; i < max_line; i++)   //lint !e838
    {
        j = i + gDispItem.siIndex;

        if (j >= node_num) {
            break;
        }
        item_node = sdkSysListGetAt(mDispItemList, j);

        if (NULL == item_node) {
            Assert(0);
            return SDK_ERR;
        }

        if (0 == strlen(item_node->DispStr)) {
            Assert(0);
            return SDK_ERR;
        }
        sdkDispGetDispMode(item_node, &disp_mode, isNoFDisp, siDispMode);   //��ȡ��/����ʾģʽ
        TraceHex("zjp", "sdkDispPutStrInRom:item_node->DispStr:\r\n", item_node->DispStr,
                 strlen(item_node->DispStr));
        sdkDispFillRowRam(siStartLine++, 0, item_node->DispStr, disp_mode);
    }
    //Trace( "zjp", "sdkDispPutStrInRom:i = %d\r\n", i );
    return i;
}

/*****************************************************************************
** Descriptions:	���㵱ǰ�ڵ��Խ������
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.27
** Remarks:
*****************************************************************************/
static s32 sdkDispCalaStep(s32 siForWard) {
    DISPNODE *item_node = NULL;
    s32 i = gDispItem.siID;
    s32 count = 0;
    s32 node_num = 0;

    item_node = sdkSysListGetAt(mDispItemList, i);  //lint !e838

    if (NULL == item_node) {
        return 0;
    }
    node_num = item_node->siNode;   //lint !e838                        //��ȡ��ǰ�ڵ��

    while (node_num == item_node->siNode) {
        if (SDK_KEY_DOWN == siForWard) {
            i++;
        }
        else {
            i--;

            if (i < 0) {
                i = sdkSysListGetCnt(mDispItemList) - 1;
            }
        }
        count++;

        if (i >= sdkSysListGetCnt(mDispItemList) || count >= sdkSysListGetCnt(mDispItemList)) {
            break;
        }
        item_node = sdkSysListGetAt(mDispItemList, i);
    }

    if (SDK_KEY_DOWN == siForWard)           //ÿ�����·�ҳ��ʱ��ָ��һ����¼�����һ��
    {
        node_num = item_node->siNode;
        i++;

        if (i < sdkSysListGetCnt(mDispItemList))                 //������¼��������һ�����Ҫ�ж�ֱ���˳�
        {
            item_node = sdkSysListGetAt(mDispItemList, i);

            while (node_num == item_node->siNode) {
                count++;
                i++;

                if (i >= sdkSysListGetCnt(mDispItemList)) {
                    break;
                }
                item_node = sdkSysListGetAt(mDispItemList, i);
            }
        }
    }

    if (count > gDispItem.siMaxLine) {
        count = gDispItem.siMaxLine;
    }
    return count;
}

/*****************************************************************************
** Descriptions:	������
** Parameters:          s32 key
                               s32 siTotalIndex
                               s32 siShowIndexEnd
                               s32 *pOffsetIndex
                               s32 *pSelectIndex
** Returned value:
** Created By:		fusuipu  2013.05.27
** Remarks:
*****************************************************************************/
static s32 sdkDispDealKey(s32 key) {
    DISPNODE *item_node = NULL;
    s32 node_num = 0;

    switch (key) {
        case SDK_KEY_UP:

            gDispItem.siID -= sdkDispCalaStep(SDK_KEY_UP);

            if (gDispItem.siID >= 0) {
                item_node = sdkSysListGetAt(mDispItemList, gDispItem.siID);
                node_num = item_node->siNode;

                while (gDispItem.siID > 0)           //���һ����Ϣ�ж��У���Ҫ�˵�������Ϣ�ĵ�һ����ʾ
                {
                    item_node = sdkSysListGetAt(mDispItemList, gDispItem.siID - 1);

                    if (node_num != item_node->siNode) {
                        break;
                    }
                    gDispItem.siID--;                //һֱ����
                }

                if (gDispItem.siID < gDispItem.siIndex) {
                    gDispItem.siIndex = gDispItem.siID;
                }
            }
            else {
                gDispItem.siID = sdkSysListGetCnt(mDispItemList) - 1;
                gDispItem.siIndex = gDispItem.siID + 1 - gDispItem.siMaxLine;

                if (gDispItem.siIndex < 0) {
                    gDispItem.siIndex = 0;
                }
            }
            return SDK_DISP_MSG_BOX_NOT_RET;


        case SDK_KEY_DOWN:

            if (gDispItem.siID < sdkSysListGetCnt(mDispItemList) - 1) {
                gDispItem.siID += sdkDispCalaStep(SDK_KEY_DOWN);

                if (gDispItem.siID >= sdkSysListGetCnt(mDispItemList)) {
                    gDispItem.siID = sdkSysListGetCnt(mDispItemList) - 1;     //��ֹ�������ڵ���
                }

                if (gDispItem.siID - gDispItem.siIndex >= gDispItem.siMaxLine) {
                    gDispItem.siIndex +=
                            (gDispItem.siID + 1 - gDispItem.siIndex) - gDispItem.siMaxLine;
                }
            }
            else {
                gDispItem.siID = 0;
                gDispItem.siIndex = 0;
            }
            return SDK_DISP_MSG_BOX_NOT_RET;


        case SDK_TIME_OUT:
            return SDK_TIME_OUT;


        case   SDK_KEY_ESC:
            return SDK_ESC;


        case SDK_KEY_ENTER:
            item_node = sdkSysListGetAt(mDispItemList, gDispItem.siID);
            return item_node->siNode;


        default:

            if (SDK_SYS_MACHINE_K501AM == sdkSysGetMachineCode(NULL) &&
                key == SDK_KEY_F3) //fusuipu 2013.04.15 11:2 ��K501AM����Ӧ�˳���
            {
                return SDK_ESC;
            }
            else {
                Assert(0);
                break;
            }
    }

    return SDK_ESC;
}

/*****************************************************************************
** Descriptions:	�˳�ʱ�����б�
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.28
** Remarks:
*****************************************************************************/
static s32 sdkDispClearList(void) {
    sdkDispFreeItemNode();               //���ͷ�������ߵ���ʾ�ڵ�
    sdkSysListDestroy(mDispItemList);    //�ͷ���ʾ�ڵ�
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������
** Parameters:          s32 key
                               s32 siTotalIndex
                               s32 siShowIndexEnd
                               s32 *pOffsetIndex
                               s32 *pSelectIndex
** Returned value:
** Created By:		fusuipu  2013.05.27
** Remarks:
*****************************************************************************/
static s32 sdkDispDealMsgKey(s32 key) {
    s32 node_num = sdkSysListGetCnt(mDispItemList);

    if (key < 0) {
//        Assert(0);
        return SDK_PARA_ERR;
    }
    Trace("node_num", "node_num = %d\r\n", node_num);
    //   printf("node_num = %d\r", node_num);

    if (node_num <= 0) {
        return node_num;
    }

    switch (key) {
        case SDK_KEY_UP:

            if (gDispItem.siIndex - gDispItem.siMaxLine >= 0) {
                gDispItem.siIndex -= gDispItem.siMaxLine;
            }
            return SDK_DISP_MSG_BOX_NOT_RET;

        case SDK_KEY_DOWN:

            if (gDispItem.siIndex + gDispItem.siMaxLine < node_num) {
                gDispItem.siIndex += gDispItem.siMaxLine;
            }
            else {
                gDispItem.siIndex = 0;               //���·�ҳ֧��ѭ��
            }
            return SDK_DISP_MSG_BOX_NOT_RET;

        default:
            return key;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const u8*pasTitle       �Ի������
                    const u8 *pasDispData  ��ʾ������
                    s32 siOvertimer        ��ʱ����ʱ�� Ϊ0��ʾ��Զ����ʱ
                    s32 siKeyMask         ��Ӧ����mask
** Returned value:
** Created By:		shiweisong  2013.01.21
** Remarks:
*****************************************************************************/
s32 sdkDispMsgBox(const u8 *pasTitle, const u8 *pasDispData, s32 siOvertimer, s32 siKeyMask) {
    s32 key = 0;
    bool bIsHaveTitle = false;
    s32 retFlag = SDK_ERR;

    if ((pasDispData == NULL) || siOvertimer < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispReadyStr(pasDispData, pasTitle);               //׼����Ҫ��ʾ������

    while (1) {
        sdkDispClearScreen();
        bIsHaveTitle = sShowTitle(pasTitle);        //��ʾ����
        sdkDispPutStrInRom(bIsHaveTitle, SDK_DISP_LINE5, false, -1);    //����ʾ���ݷ���LCD����ʾ
        sdkDispBrushScreen();                       //����

        key = sdkKbWaitKey(SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | (u32) siKeyMask, siOvertimer);

        //����������
        if ((retFlag = sdkDispDealMsgKey(key)) != SDK_DISP_MSG_BOX_NOT_RET) {
            break;
        }
    }

    sdkDispClearList();
    return retFlag;
}

/*****************************************************************************
** Descriptions:
** Parameters:      const u8*pasTitle       �Ի������
                    const u8 *pasDispData  ��ʾ������
                    s32 siOvertimer        ��ʱ����ʱ�� Ϊ0��ʾ��Զ����ʱ
** Returned value:
** Created By:		shiweisong  2013.01.21
** Remarks:
*****************************************************************************/
s32 sdkDispListBox(const u8 *pasTitle, const u8 *pasDispData, s32 siOvertimer) {
    s32 key = 0;
    bool bIsHaveTitle = false;
    s32 retFlag = SDK_ERR;

    if ((pasDispData == NULL) || siOvertimer < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    sdkDispReadyStr(pasDispData, pasTitle);               //׼����Ҫ��ʾ������

    while (1) {
        u32 uiMask = SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_ESC | SDK_KEY_MASK_ENTER;
        sdkDispClearScreen();
        bIsHaveTitle = sShowTitle(pasTitle);    //��ʾ����
        sdkDispPutStrInRom(bIsHaveTitle, SDK_DISP_LINE5, true, -1); //����ʾ���ݷ���LCD����ʾ
        sdkDispBrushScreen();                   //����

        if (SDK_SYS_MACHINE_K501AM == sdkSysGetMachineCode(NULL)) {
            uiMask = (uiMask | SDK_KEY_MASK_F3);
        }
        //�ȴ��û�����  �����·�ҳ
        key = sdkKbWaitKey(uiMask, siOvertimer);

        //����������
        if ((retFlag = sdkDispDealKey(key)) != SDK_DISP_MSG_BOX_NOT_RET) {
            break;
        }
    }

    sdkDispClearList();
    return retFlag;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ��ʾ��ʾ����BOX
   ��ڲ���:  bool IsSaveScreen,�Ƿ񱣴���
                s32 siOvertimer ��ʱʱ�䣬��ʱ����SDK_TIME_OUT  ��ʱ0��ʾ ��Զ����ʱ//shiweisong 2013.01.19 17:25
                u8 *DispData��Ҫ��ʾ����
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkDispPromptBox(bool const bIsSaveScreen, s32 const siOvertimer, const u8 *asDispData) {
    s32 key = 0;
    u8 i;
    void *pstreen = NULL;

    if ((bIsSaveScreen != false && bIsSaveScreen != true)
        || (asDispData == NULL) || siOvertimer < 0) {
        Assert(0);
        return SDK_PARA_ERR; //lilin 20120823
    }

    if (bIsSaveScreen) //shiweisong 2013.01.10 9:35  ������ŵ�����ǰ����Ȼ���󻹴��û��������
    {
        pstreen = sdkGetMem(sdkDispGetScreenBufferSize()); //�ں����ڲ�����shiweisong 2013.03.22 18:5

        if (pstreen != NULL) {
            sdkDispSaveScreen(pstreen, sdkDispGetScreenBufferSize());
        }
    }

    for (i = SDK_DISP_LINE2; i < (SDK_DISP_LINE5 + 1); i++) {
        sdkDispClearRowRam(i);
    }


    sdkDispReadyStr(asDispData, NULL);               //׼����Ҫ��ʾ������

    while (1) {
        for (i = SDK_DISP_LINE2; i < (SDK_DISP_LINE5 + 1); i++) {
            sdkDispClearRowRam(i);
        }

        sdkDispPutStrInRom2(1, SDK_DISP_LINE5, SDK_DISP_FDISP | SDK_DISP_CDISP |
                                               SDK_DISP_INCOL);            //����ʾ���ݷ���LCD����ʾ
        sdkDispBrushScreen();                   //����
        key = sdkKbWaitKey(
                SDK_KEY_MASK_ESC | SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_ENTER,
                siOvertimer);

        //����������
        if (sdkDispDealMsgKey(key) != SDK_DISP_MSG_BOX_NOT_RET) {
            break;
        }
    }

    sdkDispClearList();

    if (bIsSaveScreen) //shiweisong 2013.01.10 9:35
    {
        if (pstreen != NULL) {
            Verify(sdkDispRestoreScreen(pstreen, sdkDispGetScreenBufferSize()) >= 0);
            sdkFreeMem(pstreen);
        }
    }
    return key;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ��ʾ��ʾ����
   ��ڲ���:  s32 siOvertimer ��ʱʱ��
                s32 const siStartLine��ʼ����
                s32 const iEndLine ��������
                u8 const DispAtr���䷽ʽ
                u8 *DispData��Ҫ��ʾ����
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkDispPromptLabel(s32 const siOvertimer, s32 const siStartLine, s32 const iEndLine,
                       const u8 *asDispData, u8 const DispAtr) {
    s32 key = 0;
    s32 i;


    if (siStartLine < SDK_DISP_LINE1 || siStartLine > SDK_DISP_LINE5
        || iEndLine < SDK_DISP_LINE1 || iEndLine > SDK_DISP_LINE5
        || siStartLine > iEndLine || asDispData == NULL) {
        Assert(0);
        return SDK_PARA_ERR; //lilin 20120823//shijianglong 2013.05.20 10:47
    }

    for (i = siStartLine; i < (iEndLine + 1); i++) {
        sdkDispClearRowRam((u8) i);
    }

    sdkDispReadyStr(asDispData, NULL);               //׼����Ҫ��ʾ������

    while (1) {
        //shiweisong 2013.03.22 18:5
        for (i = siStartLine; i < (iEndLine + 1); i++) {
            sdkDispClearRowRam((u8) i);
        }

        sdkDispPutStrInRom2(siStartLine, iEndLine, DispAtr);           //����ʾ���ݷ���LCD����ʾ
        sdkDispBrushScreen();                   //����
        key = sdkKbWaitKey(
                SDK_KEY_MASK_ESC | SDK_KEY_MASK_UP | SDK_KEY_MASK_DOWN | SDK_KEY_MASK_ENTER,
                siOvertimer);

        //����������
        if (sdkDispDealMsgKey(key) != SDK_DISP_MSG_BOX_NOT_RET) {
            break;
        }
    }

    sdkDispClearList();
    return key;
}

/*******************************************************************
   ��	  ��: ����
   ��	  Ȩ: �����¹����ɷ����޹�˾
   ��������:  ��ʾ��ʾ����
   ��ڲ���: const u8 StartLine ��ʼ����
                        const u8 *asDispData ��ʾ����
                        const u8 DispAtr���뷽ʽ
   �� �� ֵ:
   ��	  ע:
 ********************************************************************/
s32 sdkDispLabel(s32 const siStartLine, u8 const *asDispData, u8 const DispAtr) {
    s32 ret = 0;
    s32 i;

    if ((asDispData == NULL) || (siStartLine < SDK_DISP_LINE1)
        || (siStartLine > SDK_DISP_LINE5)) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    for (i = siStartLine; i < (SDK_DISP_LINE5 + 1); i++) {
        sdkDispClearRowRam((u8) i);
    }

    sdkDispReadyStr(asDispData, NULL);               //׼����Ҫ��ʾ������
    ret = sdkDispPutStrInRom2(siStartLine, SDK_DISP_LINE5,
                              DispAtr);   //lint !e838        //����ʾ���ݷ���LCD����ʾ
    sdkDispBrushScreen();                       //����
    sdkDispClearList();

    return ret;
}

