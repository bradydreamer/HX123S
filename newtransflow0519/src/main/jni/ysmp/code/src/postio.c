/*****************************************************************************
** Descriptions:	����һ��ASC�ַ���
** Parameters:    	u8 *pasCaption  ��ʾ̧ͷ
                               u8 *pasTag       ��ʾ��ǩ
                               u8 *pasInfo      ��ʾ��ʾ
                               u8 *pucData  Ԥ����������
                               u8 nMin      ��С����
                               u8 nMax      ��󳤶�
                               u32 uiMask   ��������
** Returned value:	
** Created By:		wuzhonglin2012.12.22
** Remarks: 		
*****************************************************************************/
static s32 Inputstr(u8 *pasCaption, u8 *pasTag, u8 *pasInfo, u8 *pucData, u8 nMin, u8 nMax,
                    u32 uiMask) {
    u8 out[SDK_MAX_STR_LEN] = {0};
    s32 nRet = 0;
    u8 nLine = SDK_DISP_LINE1;
    if (pucData == NULL || nMin > nMax) {
        Assert(0);
        return -1;
    }

    sdkDispClearScreen();
    if (pasCaption != NULL) {
        sdkDispFillRowRam(nLine++, 0, pasCaption, SDK_DISP_DEFAULT);
    }

    if (pasTag != NULL) {
        sdkDispFillRowRam(nLine++, 0, pasTag, SDK_DISP_LEFT_DEFAULT);
    }

    if (pasInfo != NULL) sdkDispFillRowRam(SDK_DISP_LINE5, 0, pasInfo, SDK_DISP_DEFAULT);
    sdkDispBrushScreen();

    if (nMax > SDK_MAX_STR_LEN) nMax = SDK_MAX_STR_LEN;
    memset(out, 0x00, sizeof(out));
    out[0] = strlen(pucData);
    if (out[0] > nMax) out[0] = nMax;
    memcpy(&out[1], pucData, out[0]);
    TraceHex("", "pucData", pucData, strlen(pucData));
    nRet = sdkKbGetScanf(TV_AUTORET, out, nMin, nMax, uiMask, nLine);
    if (nRet == SDK_KEY_ENTER) {

        memset(pucData, 0x00, nMax);
        memcpy(pucData, &out[1], out[0]);
    }

    return nRet;
}


/*****************************************************************************
** Descriptions:	������������
** Parameters:    	u8 *pBtPwd
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 InputBtPwd(u8 *pBtPwd) {
    s32 isok = SDK_ERR;
    u8 pwd[64] = {0};

    strcpy(pwd, pBtPwd);

    //ljh2015-01-09
    if (SDK_KEY_ENTER == Inputstr(NULL, "��������������", NULL, pwd, 6, 6,
                                  SDK_MMI_NUMBER | SDK_MMI_LETTER | SDK_MMI_SYMBOL)) {
        strcpy(pBtPwd, pwd);
        isok = SDK_OK;
    }
    else {
        isok = SDK_ESC;
    }
    return isok;
}

/*****************************************************************************
** Descriptions:	������������
** Parameters:    	u8 *pBtName ��������
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 InputBtName(u8 *pBtName) {
    s32 isok = SDK_ERR;
    u8 temp[128] = {0};

    strcpy(temp, pBtName);
    if (SDK_KEY_ENTER == Inputstr(NULL, "��������������", NULL, temp, 1, 64,
                                  SDK_MMI_NUMBER | SDK_MMI_LETTER | SDK_MMI_SYMBOL)) {
        strcpy(pBtName, temp);
        isok = SDK_OK;
    }
    else {
        isok = SDK_ESC;
    }
    return isok;
}

/*****************************************************************************
** Descriptions:	�������庯��
** Parameters:          const struct _InputMap *pstInputMap ����ṹ��
                               u8 *pasAnyData ���뷵������
                               bool (*pJudgeFunc)(const u8 *pasData,const u8 ucLen) �ⲿ���жϺ���
                               void (*pDisFunc)(const u8 *pasDispData)�ⲿ��ʾ����
** Returned value:
** Created By:		����2012.07.27
** Remarks:
*****************************************************************************/
bool InputAny(struct _INPUTMAP *pstInputMap, u8 *pasAnyData,
              s32(*pJudgeFunc)(u8 const *pasData, u8 const ucLen),
              void (*pDisFunc)(const u8 *pasDispData)) {
    u8 tmpno[41] = {0};
    s32 rslt = 0;
    bool isok = false;

    if (pasAnyData == NULL || pstInputMap == NULL) {
        Assert(0);
        return isok;
    }

    while (1) {
        memset(tmpno, 0, sizeof(tmpno));
        rslt = PostFlowInput(pstInputMap, tmpno, pDisFunc);

        if (rslt == SDK_KEY_ENTER) {
//            Trace("InputAny", "%d  %d %d \r\n", pstInputMap->bIsJudge, pstInputMap->ucMinLen, pstInputMap->ucMaxLen);
            if ((pstInputMap->bIsJudge == true)        //��Ҫ�ж�λ������
                && (tmpno[0] < pstInputMap->ucMaxLen) //����С����󳤶�
                && !((pstInputMap->ucMinLen == 0) //������Сֵ����0
                     && (tmpno[0] == 0)) //���볤��Ϊ0
                    ) {
//                rslt = sdkDispPromptBox(false, TV_AUTORET, "λ������,������\n��[ȷ��]����");
//
//                if (rslt == SDK_KEY_ENTER)
//                {
//                    continue;
//                }
//                else
//                {
//                    break;
//                }
                sdkDispLabel(pstInputMap->ucRowNo, "λ������", SDK_DISP_RIGHT_DEFAULT);
                while (1) {
                    rslt = sdkKbWaitKey(
                            SDK_KEY_MASK_DIGITAL | SDK_KEY_MASK_ENTER | SDK_KEY_MASK_ESC, TV_SHOW);
                    if (rslt == SDK_TIME_OUT || rslt == SDK_KEY_ENTER
                        || (rslt >= SDK_KEY_0 && rslt <= SDK_KEY_4)) {
                        break;
                    }
                    else if (rslt == SDK_KEY_ESC) {
                        return false;
                    }
                }
            }
            else {
                if (pJudgeFunc != NULL)        //�����жϺ���
                {
                    rslt = pJudgeFunc(&tmpno[1], tmpno[0]);
                    if (rslt == SDK_EQU)       //ȷ��Ҫ��������
                    {
                        continue;
                    }
                    else if (rslt == SDK_ESC)      //ȷ��Ҫ�˳�
                    {
                        //break;
                        return false;
                    }
                }
                isok = true;                       //ȷ����ȷ
                memcpy(pasAnyData, &tmpno[1], tmpno[0]);
                break;
            }
        }
        else if (rslt == SDK_KEY_ESC || rslt == SDK_TIME_OUT) {
            break;
        }
    }

    return isok;
}

/*****************************************************************************
** Descriptions:	�����������뺯��
** Parameters:          const struct _InputMap *pstInputMap :����ṹ��
                               u8 *pasOutData �������
                               void (*pDisFunc)(const u8 *pasDispData) ���Ժ���ָ��
** Returned value:	SDK_ERR����
                                SDK_KEY_ENTER ȷ��
                                SDK_KEY_ESC ȡ��
** Created By:		����2012.07.27
** Remarks: wuzhonglin, 2012/12/22
*****************************************************************************/
s32 PostFlowInput(struct _INPUTMAP *pstInputMap, u8 *pasOutData,
                  void (*pDisFunc)(const u8 *pasDispData)) {
    u8 tmpdisp[64] = {0};
    u32 displine = SDK_DISP_LINE2;  //�̶���ʾ�ڶ���
    u8 out[SDK_MAX_STR_LEN] = {0};
    s32 rslt = 0;
    u8 ucClearFlg;
    u8 *p = NULL;


    if (pstInputMap == NULL || pasOutData == NULL) {
        Assert(0);
        return SDK_ERR;
    }

    if (pDisFunc == NULL) {
        if (pstInputMap->asTag[0] == 0x80) {
            ucClearFlg = pstInputMap->asTag[1];
            if (ucClearFlg == 0xFF) {
                sdkDispClearScreen();
            }
            else {
                if (ucClearFlg & 0x01) sdkDispClearRowRam(SDK_DISP_LINE1);
                if (ucClearFlg & 0x02) sdkDispClearRowRam(SDK_DISP_LINE2);
                if (ucClearFlg & 0x04) sdkDispClearRowRam(SDK_DISP_LINE3);
                if (ucClearFlg & 0x08) sdkDispClearRowRam(SDK_DISP_LINE4);
                if (ucClearFlg & 0x10) sdkDispClearRowRam(SDK_DISP_LINE5);
            }

            p = &(pstInputMap->asTag[3]);
            //while(p && *p != '\0')
            while ((p != NULL) && (*p != '\0'))    //yangchangguang 20120221
            {
                memset(tmpdisp, 0, sizeof(tmpdisp));
                p = ParseDispLineData(tmpdisp, sizeof(tmpdisp) - 1, p);
                DispLineData(tmpdisp);
            }
            sdkDispBrushScreen();
        }
        else {
            DispClearContentRam();
            sprintf(tmpdisp, "������%s:", pstInputMap->asTag);
            sdkDispLabel(displine, tmpdisp, SDK_DISP_LEFT_DEFAULT);
        }
    }
    else {
        pDisFunc(pstInputMap->asTag);
    }

    //��ʱʱ��,��������,��СֵMinLen,���ֵMaxLen,���������Mask,  �����������RowNo
    rslt = sdkKbGetScanf(TV_AUTORET, out, pstInputMap->ucMinLen, pstInputMap->ucMaxLen,
                         pstInputMap->uiMask, pstInputMap->ucRowNo);
    Trace("PostFlowInput", "sdkKbGetScanf():rslt=%d\r\n", rslt);
    if (rslt == SDK_KEY_ENTER) {
        memcpy(pasOutData, out, (out[0] + 1));
        Trace("PostFlowInput", "%d  %s\r\n", pasOutData[0], &pasOutData[1]);
    }
    return rslt;
}

/*****************************************************************************
   �� �� ��  : ������������ʾ����
   ��������  :
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ��   ��:������ // wuzhonglin, 2012/12/22
*****************************************************************************/
//#define LINEBYTE                               (SDKDISPMAXLINELEN + 4)
u8 *ParseDispLineData(u8 *pasOut, const u8 ucMaxOutSize, const u8 *pasIn) {
    u8 *p = NULL, *q = NULL;
    u8 uc_len = 0, uc_size = 0;

    //uc_size = (ucMaxOutSize > LINEBYTE) ? LINEBYTE : ucMaxOutSize;
    uc_size = ucMaxOutSize;

    if (pasIn == NULL) {
        return NULL;
    }
    q = (u8 *) pasIn;

    p = strstr(q, "\n");

    if (p == NULL) {
        strncpy(pasOut, pasIn, uc_size);
        return NULL;
    }

    if (q != NULL && p != NULL) {
        if (p > q) {
            uc_len = p - q;

            if (uc_len > uc_size) {
                uc_len = uc_size;
            }
            memcpy(pasOut, q, uc_len);
            return p + 1;
        }
    }
    return NULL;
}

/*****************************************************************************
   �� �� ��  : ������ʾ
   ��������  :
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ��   ��:������ //wuzhonglin, 2012/12/22
*****************************************************************************/
void DispLineData(const u8 *pasIn) {
    u8 uc_line;
    u8 uc_atr = SDK_DISP_INCOL;

    if (pasIn && strlen(pasIn) > 2) {
        switch (pasIn[0]) {
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
                uc_line = (pasIn[0] - 0x31);
                break;

            default:
                uc_line = SDK_DISP_LINE2;
                break;
        }

        switch (pasIn[1]) {
            case 'R':
            case 'r':
                uc_atr |= SDK_DISP_RDISP;
                break;

            case 'C':
            case 'c':
                uc_atr |= SDK_DISP_CDISP;
                break;

            default:
                uc_atr |= SDK_DISP_LDISP;
                break;
        }

        if (pasIn[2] != 'N' && pasIn[2] != 'n') {
            uc_atr |= SDK_DISP_FDISP;
        }
        else {
            uc_atr |= SDK_DISP_NOFDISP;
        }
        sdkDispFillRowRam(uc_line, 0, &pasIn[3], uc_atr);
    }
}

/*****************************************************************************
   �� �� ��  : DispClearContentRam
   ��������  : ���2~5����Ļ����
   �������  : void
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��3��10��
    ��    ��   : zhangjun
    �޸�����   : �����ɺ���

*****************************************************************************/
void DispClearContentRam(void) {
    sdkDispClearRowRam(SDK_DISP_LINE2);              //�������
    sdkDispClearRowRam(SDK_DISP_LINE3);              //�������
    sdkDispClearRowRam(SDK_DISP_LINE4);              //�������
    sdkDispClearRowRam(SDK_DISP_LINE5);              //�������
}


