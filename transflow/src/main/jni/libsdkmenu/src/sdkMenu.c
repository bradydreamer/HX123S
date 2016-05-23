#include "sdkMenu.h"

#ifndef  DEBUG_LCD
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif

Menu MenuInfor = {"", SDK_UI_SHOW_MODE_1_ROW, 0, 0, 0, 10};

SDK_SYS_LIST_HAND mAviItemList;     // �ɼ��˵�list
SDK_SYS_LIST_HAND mHidItemList;     // ���ز˵�list


/*****************************************************************************
** Descriptions:    ���ò˵�����
** Parameters:      char const* pasTile
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:         ��sdkUIClearItems()�� sdkUISetShowMode����֮��ſ��Ե���        
*****************************************************************************/
s32 sdkUISetTile(const u8 *pasTile) {
    s32 max_letter_per_line = sdkSysGetMaxLetterPerLine();

    if (0 == strlen((char *) pasTile)) {
        return SDK_ERR;
    }
    memset(MenuInfor.pTitle, 0, sizeof(MenuInfor.pTitle));

    //��ֹ����������ְ���ֵ����
    if ((s32) strlen(pasTile) <= max_letter_per_line) {
        strcpy((char *) MenuInfor.pTitle, (char *) pasTile);
    }
    else {
        Verify(sdkSplitStr(MenuInfor.pTitle, pasTile, 0, max_letter_per_line) >= 0);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	����ģʽ�£��еĲ˵�1,2,3,4���е�1~8ѭ�����е�0~9ѭ��������
                    �ṩ�˲�����������
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.10.15
** Remarks:         ��sdkUIClearItems()�� sdkUISetShowMode����֮��ſ��Ե���
*****************************************************************************/
s32 sdkUISetModeitemNum(s32 siModeNum) {
    if (siModeNum < 0) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (0 == siModeNum) {
        siModeNum = 100;        //Ĭ��0��ʾ���к�һֱ�����ۼӣ�100���˵��㹻��
    }

    /*=======BEGIN: fusuipu 2013.10.16  9:51 modify Ϊ����sdk_keli֮ǰ�Ĳ˵���ͷ�ļ��汾����===========*/
    /*====================== END======================== */
    MenuInfor.siMaxTile = siModeNum;
    return siModeNum;
}

/*****************************************************************************
** Descriptions:    ����ģʽ
** Parameters:      SDK_UI_SHOW_MODE mode
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 sdkUISetShowMode(SDK_UI_SHOW_MODE eMode) {
    if (eMode >= SDK_UI_SHOW_MODE_Err || (s32) eMode < 0) {
        return SDK_PARA_ERR;
    }
    /*=======BEGIN: fusuipu 2014.07.14  16:34 modify
    ���ٵ���֧�֣��Զ�תΪSDK_UI_SHOW_MODE_1_ROW�����һ����Ҫ֧�֣���������	
    sdkUISetModeitemNum(4)��ʵ��===========*/
    if (eMode == SDK_UI_SHOW_MODE_1_ROW1) {
        eMode = SDK_UI_SHOW_MODE_1_ROW;
    }
    else if (eMode == SDK_UI_SHOW_MODE_NOFDISP_1_ROW1) {
        eMode = SDK_UI_SHOW_MODE_NOFDISP_1_ROW;
    }
    /*====================== END======================== */
    MenuInfor.Mode = eMode;

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ��Ӳ˵������
** Parameters:          const char* pasTile  �˵���ʾ��Ϣ
                        MenuCallBackFun pFun �˵���Ӧ�Ļص�����
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:             ��sdkUIClearItems()�� sdkUISetShowMode����֮��ſ��Ե���
*****************************************************************************/
s32 sdkUIAddItem(const u8 *pasTile, MenuCallBackFun pFun) {
    ITEM *itemnode = NULL;
    s32 avi_item_num = 0;
    s32 key = 0;

    if (pasTile == NULL || 0 == strlen((char *) pasTile)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    itemnode = (ITEM *) sdkGetMem(sizeof(ITEM));    //lint !e838
    Verify(itemnode != NULL);
    itemnode->pText = (u8 *) sdkGetMem(strlen((char *) pasTile) + 1);
    Verify(itemnode->pText != NULL);

    strcpy((char *) itemnode->pText, (char *) pasTile);
    itemnode->pFun = pFun;

    avi_item_num = sdkSysListGetCnt(mAviItemList); //lint !e838
    key = (avi_item_num + 1) % MenuInfor.siMaxTile;//lint !e838

    /*
        ���10���˵�ѭ����ʱ��,10�Ų˵���ֵΪ0
        ����10���˵�ѭ����ʱ��˵�������κ��
    */
    if (key == 0 && MenuInfor.siMaxTile != 10) {
        key = MenuInfor.siMaxTile;
    }
    itemnode->Key = key;     //��ֵ����ֵ 1��2��3...0
    itemnode->ID = MenuInfor.siID++;        //��¼��ǰ�ڵ�ڼ��β���

    Verify(sdkSysListAddAtEnd(mAviItemList, itemnode) == SDK_OK);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ��ʾ�˵�����
** Parameters:          u8*pTile �˵�������Ϣ
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:         ���ô˺���֮��֮ǰ���õı��������ʾ������
*****************************************************************************/
static s32 SdkShowTile(const u8 *pTile) {
    if (pTile != NULL) {
        if (strlen((char *) pTile) > 0) {
            Verify(sdkDispFillRowRam(SDK_DISP_LINE1, 0, pTile, SDK_DISP_DEFAULT) >= 0);
            return SDK_OK;
        }
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:        ����˵�������ռ����
** Parameters:          u8 *pucTile �˵�����
** Returned value:
** Created By:		fusuipu  2013.03.09
** Remarks:            Ŀǰ�����������Ϊ1����ʱ��֧�ֶ��б��� 
*****************************************************************************/
static s32 sdkUICacTileLine(const u8 *pucTile) {
    return strlen((char *) pucTile) > 0 ? 1 : 0;
}

/*****************************************************************************
** Descriptions:	���һҳ��Ϊ������ʾ��Ϊ���ұߵĲ˵�������룬��������ʾ��
                    �������е��ַ������ں�����ո�
** Parameters:      u8 const *pStr ��Ҫ�������ʾ��Ϣ
                    s32 iItemNo    ��Ҫ�жϵ��ַ���λ��
** Returned value:
** Created By:		fusuipu  2013.03.10
** Remarks:
*****************************************************************************/
static s32 sdkUIDealLastWord(u8 *pStr, s32 iItemNo) {
    s32 str_len = strlen((char *) pStr);
    s32 max_letter_per_line = sdkSysGetMaxLetterPerLine();

    if (str_len < (max_letter_per_line / 2)) {
        memset(&pStr[str_len], 0X20, (u32)(max_letter_per_line / 2 - str_len));
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	��ȡÿһҳ������ʾ�����˵�����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.11
** Remarks:
*****************************************************************************/
static s32 sdkUICalaItemPerPage(void) {
    s32 max_item = sdkSysGetSreenMaxLine() - sdkUICacTileLine(MenuInfor.pTitle);

    if (SDK_UI_SHOW_MODE_2_ROW1 == MenuInfor.Mode ||
        SDK_UI_SHOW_MODE_2_ROW2 == MenuInfor.Mode ||
        SDK_UI_SHOW_MODE_NOFDISP_2_ROW1 == MenuInfor.Mode ||
        SDK_UI_SHOW_MODE_NOFDISP_2_ROW2 == MenuInfor.Mode) {
        max_item *= 2;                      //˫��ģʽ�˵������ӱ�
    }

    if (max_item > MenuInfor.siMaxTile) {
        max_item = MenuInfor.siMaxTile;     //�˵�����������ܴ������õ����˵���
    }
    return max_item;
}

/*****************************************************************************
** Descriptions:	���㵱ǰ����˵��±��ƫ��
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.04.27
** Remarks:         ��ǰ�˵���ҳ�˵���idΪ��ǰҳ*ÿҳ�����˵�����
*****************************************************************************/
static s32 sdkUICaluOffset(s32 siPage) {
    return siPage * sdkUICalaItemPerPage();
}

/*****************************************************************************
** Descriptions:	���㵱ǰ��ʾҳ����ʾ�Ĳ˵�����
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.04.27
** Remarks:
*****************************************************************************/
static s32 sdkUICaluPresPageMaxItem(s32 siPage) {
    s32 i = sdkSysListGetCnt(mAviItemList);

    if (sdkUICaluOffset(siPage) + sdkUICalaItemPerPage() < i) {
        return sdkUICalaItemPerPage();
    }
    else {
        /*
            ʣ��˵���������һҳ
        */
        return (i - sdkUICaluOffset(siPage)) < 0 ? 0 : (i - sdkUICaluOffset(siPage));
    }
}

/*****************************************************************************
** Descriptions:        ����ģʽ1����ʾ��ʽ
** Parameters:          s32 StartLine
                        s32 siOffset
                        SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:         ������ʾģʽ
*****************************************************************************/
static s32 sdkUIMenuMode1(s32 StartLine) {
    s32 i = 0;
    u8 str_buf[256] = {0};
    s32 str_len = 0;
    s32 item_node_num = 0;                                      //�˵��ܽڵ����
    ITEM *item_node = NULL;
    s32 pres_page_item_no = 0;                                  //��ҳ�����ز˵���ĸ���
    u32 disp_mode = SDK_DISP_LEFT_DEFAULT;
    s32 max_letter_per_line = sdkSysGetMaxLetterPerLine();

    if (NULL == mAviItemList) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    item_node_num = sdkSysListGetCnt(mAviItemList);   //lint !e838             //�˵��ܽڵ����

    for (i = sdkUICaluOffset(MenuInfor.siPageNo);
         i < item_node_num && pres_page_item_no < sdkUICalaItemPerPage(); i++) //lint !e838
    {
        item_node = sdkSysListGetAt(mAviItemList, i);

        if (item_node == NULL || 0 == strlen((char *) item_node->pText)) {
            Assert(0);
            return SDK_ERR;
        }
        //��֯�˵���ʾ�ַ���
        memset(str_buf, 0, sizeof(str_buf));
        str_len = sprintf((char *) str_buf, "%d.", item_node->Key);

        if ((s32) strlen((char *) item_node->pText) <= max_letter_per_line - str_len) {
            strcpy((char *) &str_buf[str_len], (char *) item_node->pText);
        }
        else {
            Verify(sdkSplitStr(&str_buf[str_len], item_node->pText, 0,
                               (max_letter_per_line - str_len)) >= 0);
        }

        //����˵���ʾģʽ
        if ((SDK_UI_SHOW_MODE_NOFDISP_1_ROW == MenuInfor.Mode ||
             SDK_UI_SHOW_MODE_NOFDISP_1_ROW1 == MenuInfor.Mode) &&
            (MenuInfor.siNoDisp == pres_page_item_no)) {
            disp_mode &= (~SDK_DISP_FDISP);              //����
        }
        else {
            disp_mode |= SDK_DISP_FDISP;              //����
        }
        sdkDispFillRowRam(StartLine++, 0, str_buf, disp_mode);

        //������һ�еĲ˵��ڵ�
        pres_page_item_no++;
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ����ģʽ2����ʾ��ʽ
** Parameters:          s32 StartLine
                               s32 siOffset
                               SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
static s32 sdkUIMenuMode2(s32 StartLine) {
    u8 str_buf[256] = {0};
    s32 i = 0;
    s32 pres_page_item_no = 0;                                //��ҳ�����ز˵�����±�
    s32 str_len = 0;
    ITEM *item_node = NULL;
    s32 item_node_num = 0;
    u32 disp_mode = 0;
    s32 max_letter_per_line = sdkSysGetMaxLetterPerLine();

    if (NULL == mAviItemList) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    item_node_num = sdkSysListGetCnt(mAviItemList); //lint !e838

    for (i = sdkUICaluOffset(MenuInfor.siPageNo);
         i < item_node_num && pres_page_item_no < sdkUICalaItemPerPage(); i++) //lint !e838
    {
        item_node = sdkSysListGetAt(mAviItemList, i);

        if (item_node == NULL) {
            Assert(0);
            return SDK_ERR;
        }

        if (0 !=
            strlen((char *) item_node->pText))                       //����������ز˵�����ʾ������ֱ������
        {
            pres_page_item_no++;                                //���������Ѿ���ʾ�ķ����ز˵��Ľڵ���

            //��֯�˵���ʾ�ַ���
            memset(str_buf, 0, sizeof(str_buf));
            str_len = (s32) sprintf((char *) str_buf, "%d.", item_node->Key);

            if ((s32) strlen((char *) item_node->pText) <= (max_letter_per_line / 2) - str_len) {
                strcpy((char *) &str_buf[str_len], (char *) item_node->pText);
            }
            else {
                Verify(sdkSplitStr(&str_buf[str_len], item_node->pText, 0,
                                   (max_letter_per_line / 2) - str_len) >= 0);
            }
            sdkUIDealLastWord(str_buf, pres_page_item_no);

            //����˵���ʾģʽ
            disp_mode = (pres_page_item_no % 2 == 1 ? SDK_DISP_LEFT_DEFAULT
                                                    : SDK_DISP_RIGHT_DEFAULT);

            if ((SDK_UI_SHOW_MODE_NOFDISP_2_ROW1 == MenuInfor.Mode) &&
                (MenuInfor.siNoDisp == pres_page_item_no - 1)) {
                disp_mode &= (~SDK_DISP_FDISP);          //����
            }
            else {
                disp_mode |= SDK_DISP_FDISP;          //����
            }

            //������һ�еĲ˵��ڵ�
            sdkDispFillRowRam((pres_page_item_no % 2 == 1 ? StartLine : StartLine++), 0, str_buf,
                              disp_mode);
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ����ģʽ3����ʾ��ʽ
** Parameters:          s32 StartLine
                               s32 siOffset
                               SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
static s32 sdkUIMenuMode3(s32 StartLine) {
    bool endflag = false;
    s32 i = 0;
    s32 pres_page_item_no = 0;
    u8 str_buf[256] = {0};
    s32 str_len = 0;
    s32 flag = StartLine;
    ITEM *item_node = NULL;
    s32 item_node_num = 0;
    s32 half_page_item_num = 0;
    u32 disp_mode = 0;
    s32 max_letter_per_line = sdkSysGetMaxLetterPerLine();

    if (NULL == mAviItemList) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    item_node_num = sdkSysListGetCnt(mAviItemList); //lint !e838

    half_page_item_num = (sdkUICaluPresPageMaxItem(MenuInfor.siPageNo) + 1) /
                         2; //lint !e838  //�Ȼ�ȡ��Offset��ʼ����ߵ�������ʾ�˵�����һ���ж�����˵�

    for (i = sdkUICaluOffset(MenuInfor.siPageNo);
         i < item_node_num && pres_page_item_no < sdkUICalaItemPerPage(); i++) //lint !e838
    {
        item_node = sdkSysListGetAt(mAviItemList, i);

        if (item_node == NULL) {
            Assert(0);
            return SDK_ERR;
        }


        if (0 !=
            strlen((char *) item_node->pText))                             //����������ز˵�����ʾ������ֱ������
        {
            //��֯�˵���ʾ�ַ���
            memset(str_buf, 0, sizeof(str_buf));
            str_len = sprintf((char *) str_buf, "%d.", item_node->Key);

            if ((s32) strlen((char *) item_node->pText) <= (max_letter_per_line / 2) - str_len) {
                strcpy((char *) &str_buf[str_len], (char *) item_node->pText);
            }
            else {
                Verify(sdkSplitStr(&str_buf[str_len], item_node->pText, 0,
                                   (max_letter_per_line / 2) - str_len) >= 0);
            }
            sdkUIDealLastWord(str_buf, pres_page_item_no + 1);

            //��ȡ�˵���ʾģʽ
            disp_mode = (pres_page_item_no < half_page_item_num ? SDK_DISP_LEFT_DEFAULT
                                                                : SDK_DISP_RIGHT_DEFAULT);

            if ((SDK_UI_SHOW_MODE_NOFDISP_2_ROW2 == MenuInfor.Mode) &&
                (MenuInfor.siNoDisp == pres_page_item_no)) {
                disp_mode &= 0xFE;          //����
            }
            else {
                disp_mode |= 0x01;          //����
            }
            sdkDispFillRowRam(StartLine, 0, str_buf, disp_mode);

            pres_page_item_no++;
            StartLine++;

            //��ȡ��һ�в˵��ڵ�
            if (StartLine >= (half_page_item_num +
                              flag))   //StartLine��0��ʼ���������һ����ʾ5���ʱ��StartLine���ֻ����4���ﵽ5���뷭ҳ
            {
                if (false == endflag) {
                    endflag = true;
                    //������ͷ��ʼ
                    StartLine = flag;
                }
                else {
                    break;
                }
            }
        }
    }

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ѡ��ģʽ
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
static s32 sdkUIGetDispFillData(void) {
    s32 tile_line_num = 0;                             //������ռ����

    tile_line_num =
            SdkShowTile(MenuInfor.pTitle) > 0 ? sdkUICacTileLine(MenuInfor.pTitle) : 0; //lint !e838

    switch (MenuInfor.Mode) {
        case SDK_UI_SHOW_MODE_1_ROW:
        case SDK_UI_SHOW_MODE_NOFDISP_1_ROW:
        case SDK_UI_SHOW_MODE_1_ROW1:
        case SDK_UI_SHOW_MODE_NOFDISP_1_ROW1: {
            sdkUIMenuMode1(tile_line_num);
            break;
        }


        case SDK_UI_SHOW_MODE_2_ROW1:
        case SDK_UI_SHOW_MODE_NOFDISP_2_ROW1: {
            sdkUIMenuMode2(tile_line_num);
            break;
        }

        case SDK_UI_SHOW_MODE_2_ROW2:
        case SDK_UI_SHOW_MODE_NOFDISP_2_ROW2: {
            sdkUIMenuMode3(tile_line_num);
            break;
        }

        default: {
            sdkUIMenuMode1(tile_line_num);
        }
            break;
    }   //lint !e788

    return SDK_OK;                             //���α������ܽڵ����
}

/*****************************************************************************
** Descriptions:        ������·�ҳ�ļ�ͷ
** Parameters:          SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.08
** Remarks:
*****************************************************************************/
static s32 sdkUIAddArrow(void) {
    s32 total_num = 0;
    u8 arrow[5] = {0};

    memset(arrow, 0, sizeof(arrow));

    if (NULL == mAviItemList) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    //���㵱ǰ�˵��ܸ����Ƿ��һҳ
    total_num = (sdkSysListGetCnt(mAviItemList) + sdkUICalaItemPerPage() - 1) /
                sdkUICalaItemPerPage(); //lint !e838

    if (total_num > 1) {
        //��һҳ
        if (MenuInfor.siPageNo == 0) {
            strcpy((char *) arrow, "��");
        }
            //���һҳ
        else if ((MenuInfor.siPageNo + 1) == total_num) {
            strcpy((char *) arrow, "��");
        }
        else {
            strcpy((char *) arrow, "����");
        }
        sdkDispFillRowRam(SDK_DISP_LINE1, 0, arrow, SDK_DISP_RIGHT_DEFAULT);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        �·�ҳ�ص�����
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.09
** Remarks:         
*****************************************************************************/
static SDK_UI_EXIT_VALUE sdkUIKeyNextPage(s32 Index) {
    if (NULL == mAviItemList) {
        Assert(0);
        return SDK_UI_EXIT_INTERNAL_ERR;
    }
    //����Ƿ���ģʽ�������Բ˵�ID������ǰҳʱ��ҳ��++
    if (MenuInfor.Mode >= SDK_UI_SHOW_MODE_NOFDISP_1_ROW)        //����ģʽ
    {
        MenuInfor.siNoDisp++;                                   //���Լ�����־������һ���ɼ��˵�

        if (MenuInfor.siNoDisp >= sdkUICaluPresPageMaxItem(MenuInfor.siPageNo)) {
            MenuInfor.siPageNo++;

            //��������Ѿ����������һҳ����Ȼ�����·�ҳ������ص���1ҳ��1��˵�
            if (sdkUICaluPresPageMaxItem(MenuInfor.siPageNo) == 0) {
                MenuInfor.siNoDisp = 0;
                MenuInfor.siPageNo = 0;
            }
                //ÿ�η���ģʽ��ҳ֮������Զ�ѡ�е�ǰ��һ���˵�
            else {
                MenuInfor.siNoDisp = 0;
            }
        }
    }
        //����ģʽ��ҳΪ��λ���з�ҳ
    else                                                        //����ģʽ
    {
        if (sdkSysListGetCnt(mAviItemList) - sdkUICaluOffset(MenuInfor.siPageNo) >
            sdkUICalaItemPerPage()) {
            MenuInfor.siPageNo++;
        }
    }
    sdkUIShowMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
** Descriptions:	��ȡ��ǰģʽ�Ĳ˵�ÿҳ�����ҳ��
** Parameters:          void
** Returned value:
** Created By:		fusuipu  2013.05.12
** Remarks:
*****************************************************************************/
static s32 sdkUICalaMaxPage(void) {
    s32 max_item = sdkUICalaItemPerPage();  //ÿ��ģʽ�µ����˵�����

    if (mAviItemList == NULL) {
        Assert(0);
        return SDK_ERR;
    }
    //ҳ����0��ʼ�������������һҳΪ��ҳ��-1
    return (sdkSysListGetCnt(mAviItemList) + max_item - 1) / max_item - 1;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.09
** Remarks:
*****************************************************************************/
static SDK_UI_EXIT_VALUE sdkUIKeyPrePage(s32 Index) {
    s32 max_item_per_page = sdkUICalaItemPerPage();
    s32 item_num = sdkUICaluPresPageMaxItem(MenuInfor.siPageNo);
    s32 max_page = 0;

    //����ģʽ����������ݼ�������ݼ�����һҳ�������һҳ
    if (MenuInfor.Mode >= SDK_UI_SHOW_MODE_NOFDISP_1_ROW)    //����ģʽ
    {
        if (item_num > 0) {
            MenuInfor.siNoDisp--;
        }

        if (MenuInfor.siNoDisp < 0) {
            //����Ѿ������˵�һҳ��һ���˵�����Ȼ���Ϸ�ҳ����ת�����һҳ�����һ��
            if (MenuInfor.siPageNo <= 0) {
                max_page = sdkUICalaMaxPage();
                MenuInfor.siPageNo = max_page;
                MenuInfor.siNoDisp = sdkUICaluPresPageMaxItem(max_page) - 1;
            }
                //���������һҳ��������ѡ����һҳ�����һ��
            else {
                MenuInfor.siPageNo--;
                MenuInfor.siNoDisp = max_item_per_page - 1;
            }
        }
    }
        //����ģʽ��ҳΪ��λ���Ϸ�ҳ��������һҳ֮�������Ϸ�ת�����һҳ
    else                                                    //����ģʽ
    {
        if (0 == item_num) {
            return SDK_UI_EXIT_FORCE_OUT;
        }

        if (MenuInfor.siPageNo > 0) {
            MenuInfor.siPageNo--;
        }
        else {
            MenuInfor.siPageNo = sdkUICalaMaxPage();
        }
    }
    sdkUIShowMenu();
    return SDK_UI_EXIT_ENTER;
}

/*****************************************************************************
** Descriptions:	�жϲ���Ľڵ��Ƿ�Ϊ���·�ҳ�����ز˵��ڵ�
** Parameters:          s32 sikey
                               MenuCallBackFun pFun
** Returned value:
** Created By:		fusuipu  2013.03.15
** Remarks:         
*****************************************************************************/
static bool sdkUIIsPagingItemExist(s32 sikey, MenuCallBackFun pFun) {
    if (((SDK_KEY_DOWN == sikey) && (sdkUIKeyNextPage == pFun)) ||
        ((SDK_KEY_UP == sikey) && (sdkUIKeyPrePage == pFun))) {
        return true;
    }
    return false;
}

/*****************************************************************************
** Descriptions:        ������ز˵�
** Parameters:          s32 key
                        MenuCallBackFun pFun
** Returned value:
** Created By:		    fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 sdkUIAddHideItem(s32 sikey, MenuCallBackFun pFun) {
    s32 item_node_num = 0;
    s32 i = 0;
    ITEM *itemnode = NULL;

    if (0 > sikey || NULL == pFun || NULL == mHidItemList) {
        Assert(0);
        return SDK_ERR;
    }
    item_node_num = sdkSysListGetCnt(mHidItemList); //lint !e838

    if (item_node_num > 0) {
        for (i = 0; i < item_node_num; i++) {
            itemnode = sdkSysListGetAt(mHidItemList, i);

            if (NULL == itemnode) {
                Assert(0);
                return SDK_ERR;
            }

            //��������������ز˵���ֵ�Ѿ�ʹ�ù�ͬʱ�ֲ�������ķ�ҳ���ز˵�����ز�������
            if ((false == sdkUIIsPagingItemExist(sikey, pFun)) &&
                (sikey == itemnode->Key))                         //���ز˵����������ظ�������ز�������
            {
                Assert(0);
                return SDK_PARA_ERR;
            }

            //����ģʽϵͳռ��ȷ�ϼ����û������ز˵�������ռ��ȷ�ϼ�
            if ((MenuInfor.Mode >= SDK_UI_SHOW_MODE_NOFDISP_1_ROW) &&
                (sikey == SDK_KEY_ENTER))            //����ģʽ�������û������ز˵�ʹ��ȷ�ϼ�
            {
                Assert(0);
                return SDK_PARA_ERR;
            }
        }
    }
    itemnode = (ITEM *) sdkGetMem(sizeof(ITEM));
    Verify(itemnode != NULL);
    itemnode->pFun = pFun;
    itemnode->Key = sikey;
    itemnode->ID = MenuInfor.siID++;

    //һ������ز˵�������β����
    if (false == sdkUIIsPagingItemExist(sikey, pFun)) {
        Verify(sdkSysListAddAtEnd(mHidItemList, itemnode) == SDK_OK);
    }
        //���·�ҳ�����ز˵�������ͷ��
    else {
        Verify(sdkSysListAddAtFront(mHidItemList, itemnode) == SDK_OK);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	�ж��Ƿ������ӷ�ҳ�����ز˵�
** Parameters:      SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.15
** Remarks:
*****************************************************************************/
static s32 sdkUIIsAbleAddPagingItem() {
    s32 item_num = 0;
    ITEM *item_node = NULL;

    if (NULL == mAviItemList || NULL == mHidItemList) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (MenuInfor.Mode >= SDK_UI_SHOW_MODE_NOFDISP_1_ROW) {              //����ģʽһ��������·�ҳ����
        item_num = sdkSysListGetCnt(mHidItemList);

        if (0 == item_num) {
            return true;
        }
        item_node = sdkSysListGetAt(mHidItemList, 0);

        if (NULL == item_node) {
            return SDK_ERR;
        }

        if (false == sdkUIIsPagingItemExist(item_node->Key, item_node->pFun)) {
            return true;
        }
    }
    else            //����ģʽ����һҳ���Զ�������·�ҳ����
    {
        item_num = sdkSysListGetCnt(mAviItemList);

        if (item_num > sdkUICalaItemPerPage()) {
            item_num = sdkSysListGetCnt(mHidItemList);

            if (0 == item_num) {
                return true;
            }
            item_node = sdkSysListGetAt(mHidItemList, 0);

            if (NULL == item_node) {
                return SDK_ERR;
            }

            if (false == sdkUIIsPagingItemExist(item_node->Key, item_node->pFun)) {
                return true;
            }
        }
    }
    return false;
}

/*****************************************************************************
** Descriptions:	ֻ���ڲ˵�ѡ���һҳ��ʱ���������·�ҳ��ͷ
** Parameters:          SDK_SYS_LIST_HAND ItemList
** Returned value:
** Created By:		fusuipu  2013.03.15
** Remarks:
*****************************************************************************/
static s32 sdkUIAddPageTurningItem(void) {
    if (true == sdkUIIsAbleAddPagingItem()) {
        sdkUIAddHideItem(SDK_KEY_UP, sdkUIKeyPrePage);
        sdkUIAddHideItem(SDK_KEY_DOWN, sdkUIKeyNextPage);
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������浫��ˢ���������ˢ��
** Parameters:    	void
** Returned value:	
** Created By:		fusuipu  2014.03.14
** Remarks: 		
*****************************************************************************/
static s32 sdkUIDispClearScreenRam(void) {
    sdkDispClearRowRam(SDK_DISP_LINE1);
    sdkDispClearRowRam(SDK_DISP_LINE2);
    sdkDispClearRowRam(SDK_DISP_LINE3);
    sdkDispClearRowRam(SDK_DISP_LINE4);
    sdkDispClearRowRam(SDK_DISP_LINE5);

    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 sdkUIShowMenu() {
    sdkUIDispClearScreenRam();                     //���LCD��Ļ�ϵ����ݣ�����ˢ��
    sdkUIAddPageTurningItem();                     //�ж��Ƿ���Ҫ������·�ҳ�����ز˵�
    sdkUIGetDispFillData();                        //��ȡÿһҳ����ʾ����
    sdkUIAddArrow();                               //������·�ҳ�ļ�ͷ
    sdkDispBrushScreen();
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:        ���ݰ����ҵ���ʾ�˵��ڵ�
** Parameters:          s32 siKey
** Returned value:
** Created By:		fusuipu  2013.05.02
** Remarks:
*****************************************************************************/
static ITEM *sdkUIFindAviItem(s32 siKey) {
    s32 curr_index = 0;
    s32 i = 0;
    s32 offset = 0;
    ITEM *item_node = NULL;

    if (siKey <= 0) {
        Assert(0);
        return NULL;
    }
    i = sdkUICaluOffset(MenuInfor.siPageNo); //lint !e838

    if (sdkSysListGetCnt(mAviItemList) <= i) {
        return NULL;
    }
    item_node = sdkSysListGetAt(mAviItemList, i);   //lint !e838   //��ȡ��ҳ����˵��ڵ���Ϣ

    if (NULL == item_node)                       //�����ȡʧ�ܣ�ֱ�ӷ���
    {
        return NULL;
    }

    if (SDK_KEY_ENTER == siKey &&
        MenuInfor.Mode >= SDK_UI_SHOW_MODE_NOFDISP_1_ROW) {
        offset = i + MenuInfor.siNoDisp;        //����ģʽ��ҳ������Ϸ���ƫ��
    }
    else if (siKey >= SDK_KEY_0 && siKey <= SDK_KEY_9) {
        curr_index = siKey - SDK_KEY_0;               //������ֵת��Ϊ����

        if ((siKey == SDK_KEY_0) && (0 != item_node->Key)) {
            curr_index += 10;                        //Ĭ��0����Ӧ10��
        }
        offset = curr_index - item_node->Key;        //������ҳ����밴��ֵ֮���ƫ��

        if (offset < 0 &&
            (MenuInfor.siMaxTile + curr_index - item_node->Key <
             sdkUICaluPresPageMaxItem(MenuInfor.siPageNo)) &&
            (MenuInfor.siMaxTile + curr_index + i - item_node->Key < sdkSysListGetCnt(
                    mAviItemList)))                     //��ҳ������ʾ8,9��1��2��3��4...�����û���3
        {
            offset = MenuInfor.siMaxTile + curr_index - item_node->Key;
        }
        else if (offset >= MenuInfor.siMaxTile)       //���ƫ�������ܳ���һ���˵�ѭ����������к�
        {
            return NULL;
        }
        offset += i;
    }
    else {
        return NULL;
    }

    if (offset >=
        sdkSysListGetCnt(mAviItemList)) //�����ʾ�˵���߸�����û����ô��ڵ㣬���û����ⰴ���ˣ��ͻ����Assert����
    {
        return NULL;
    }
    item_node = sdkSysListGetAt(mAviItemList, offset);

    if (item_node != NULL && 0 != strlen((char *) item_node->pText) &&
        NULL != item_node->pFun)   //��ֹ��ʾ�˵���߲������ص����������
    {
        return item_node;
    }
    return NULL;
}

/*****************************************************************************
** Descriptions:	���ݰ����ҵ����ز˵�
** Parameters:      s32 siKey
** Returned value:
** Created By:		fusuipu  2013.05.02
** Remarks:
*****************************************************************************/
static ITEM *sdkUIFindHidItem(s32 siKey) {
    s32 i = 0;
    s32 item_node_num = 0;          //�˵��ܽڵ���
    ITEM *item_node = NULL;

    if (NULL == mHidItemList) {
        return NULL;
    }
    item_node_num = sdkSysListGetCnt(mHidItemList); //lint !e838

    for (i = 0; i < item_node_num; i++) //lint !e838
    {
        item_node = sdkSysListGetAt(mHidItemList, i);

        if (item_node == NULL) {
            Assert(0);
            return NULL;
        }

        if (item_node->Key == siKey)                       //��ҳ�ĵڼ����˵�ѡ��
        {
            return item_node;
        }
    }

    return NULL;
}

/*****************************************************************************
** Descriptions:        ִ�а�������Ӧ
** Parameters:          s32 key
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 sdkUIRevKey(s32 key) {
    ITEM *item_node = NULL;

    if (key > 0) {
        item_node = sdkUIFindAviItem(key);

        if (item_node ==
            NULL)                                           //�������ʾ�˵����û���ҵ�,��ȥ��ʽ�˵����ȥ��
        {
            item_node = sdkUIFindHidItem(key);
        }

        if (item_node != NULL) {
            return item_node->pFun(item_node->ID);
        }
    }
    return SDK_ERR;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.11
** Remarks:
*****************************************************************************/
static s32 sdkUIFreeItemNode(void) {
    ITEM *item_node;
    s32 item_node_num = 0;
    s32 i = 0;

    if (NULL != mAviItemList) {
        item_node_num = sdkSysListGetCnt(mAviItemList);         //���ͷſɼ��˵�

        for (i = 0; i < item_node_num; i++) {
            item_node = sdkSysListGetAt(mAviItemList, i);

            if (NULL == item_node) {
                Assert(0);
                return SDK_ERR;
            }
            else {
                if (item_node->pText != NULL) {
                    sdkFreeMem(item_node->pText);
                    item_node->pText = NULL;
                }
                sdkFreeMem(item_node);
                item_node = NULL;
            }
        }
    }

    if (NULL != mHidItemList) {
        item_node_num = sdkSysListGetCnt(mHidItemList);         //���ͷ����ز˵�

        for (i = 0; i < item_node_num; i++) {
            item_node = sdkSysListGetAt(mHidItemList, i);

            if (NULL == item_node) {
                Assert(0);
                return SDK_ERR;
            }
            sdkFreeMem(item_node);
        }
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:
** Parameters:          Input: None
** Returned value:
** Created By:		fusuipu  2013.03.07
** Remarks:
*****************************************************************************/
s32 sdkUIClearItems() {
    SDK_UI_SHOW_MODE emode = MenuInfor.Mode;
    s32 max_item = MenuInfor.siMaxTile;

    sdkUIFreeItemNode();                //���ͷ�������ߵĲ˵��ڵ�
    sdkSysListDestroy(mAviItemList);    //�ͷ���ʾ�˵�
    sdkSysListDestroy(mHidItemList);    //�ͷ����ز˵�
    Verify((mAviItemList = sdkSysListCreate()) != NULL);
    Verify((mHidItemList = sdkSysListCreate()) != NULL);
    memset(&MenuInfor, 0, sizeof(Menu));
    MenuInfor.Mode = emode;
    MenuInfor.siMaxTile = max_item;
    return SDK_OK;
}


/*****************************************************************************
** Descriptions:	���㵱ǰ���Բ˵����ڵ�λ�ã��ú����������������ö���ɺ����
** Parameters:          const s32 siCurrentNoDisp
** Returned value:
** Created By:		zhongjiahong  2013.12.02
** Remarks:
*****************************************************************************/
static s32 sdkCurrNoDispPosition(s32 siCurrentNoDisp, s32 siItemNum, Menu *pMenuInfor) {
    s32 item_num_per_page = 0;
    s32 CurrPage;
    s32 TotalPage;

    if (NULL == pMenuInfor || 0 >= siCurrentNoDisp || 0 >= siItemNum) {
        Assert(0);
        return SDK_PARA_ERR;
    }

    if (SDK_UI_SHOW_MODE_NOFDISP_1_ROW <= pMenuInfor->Mode) {
        item_num_per_page = sdkUICalaItemPerPage();
        TotalPage = (siItemNum - 1) / item_num_per_page;
        CurrPage = (siCurrentNoDisp - 1) / item_num_per_page;
        pMenuInfor->siPageNo = (CurrPage <= TotalPage) ? CurrPage : 0;
        pMenuInfor->siNoDisp = (CurrPage <= TotalPage) ? (siCurrentNoDisp - 1) % item_num_per_page
                                                       : 0;
        return SDK_OK;
    }
    else {
        Assert(0);
        return SDK_PARA_ERR;
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          s32 siCurrentNoDisp
                               s32 siItemNum
** Returned value:
** Created By:		zhongjiahong  2013.12.02
** Remarks:
*****************************************************************************/
s32 sdkUISetSelectPosition(s32 siCurrentNoDisp, s32 siItemNum) {
    return sdkCurrNoDispPosition(siCurrentNoDisp, siItemNum, &MenuInfor);
}


