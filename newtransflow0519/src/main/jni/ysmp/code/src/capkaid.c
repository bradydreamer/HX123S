#define  AIDNUMINGROUP   20
#define  CAPKNUMINGROUP   20
#define CAPKDATANUM 8  //lechan 2006-1-26 03:54���� 
#define APPINIDATANUM 16

AppDATAELEMENT CapkDataTable[CAPKDATANUM];                //��Կ����Ԫ��
AppDATAELEMENT DownIniDataTable[APPINIDATANUM];            //AID����Ԫ��


//lechan Add 2006-1-9 09:07����

const AppDATAELEMENT DownCapkDataTable_Stored[CAPKDATANUM] =
        { //data with source from Center
                {/*"RID",1*/            "\x9F\x06", 255, 5,   0, 0,   1, 0},
                {/*"CAPKI",1*/          "\x9F\x22", 255, 1,   0, 5,   1, 0},
                {/*"��ϣ�㷨��ʶ",1*/       "\xDF\x06", 255, 1,   0, 6,   1, 0},
                {/*"��Կ�㷨��ʶ",1*/       "\xDF\x07", 255, 1,   0, 7,   1, 0},
                {/*"��Կģ��",1*/         "\xDF\x02", 0,   248, 0, 9,   1, 0},/*5*/
                {/*"��֤���Ĺ�Կָ��",1*/   "\xDF\x04", 1,   3,   0, 258, 1, 0},
                {/*"��֤���Ĺ�ԿУ��ֵ",1*/  "\xDF\x03", 0,   20,  0, 262, 1, 0},
                {/*"CAPK Valid Time",1*/"\xDF\x05", 255, 8,   2, 282, 1, 0},
        };

//���Ŀ����صĲ����ṹ
const AppDATAELEMENT DownInitDataTable_Stored[APPINIDATANUM] =
        { //data with source from Center
                {/*"AID",1,*/              "\x9F\x06", 5,   16,  0, 1,   0, 0},
                {/*"ASI",1,*/              "\xDF\x01", 255, 1,   0, 17,  0, 0},
                {/*"AppVerNum",0,*/        "\x9F\x09", 255, 2,   0, 18,  0, 0},//9F 08 ->9F 09
                {/*"TAC DEFAULT",0,*/      "\xDF\x11", 255, 5,   0, 20,  0, 0},
                {/*"TAC ONLINE",0,*/       "\xDF\x12", 255, 5,   0, 25,  0, 0},
                {/*"TAC DECLINE",0,*/      "\xDF\x13", 255, 5,   0, 30,  0, 0},
                {/*"FloorLimit",1,*/       "\x9F\x1B", 255, 4,   1, 35,  0, 0},
                {/*"���ƫ�÷�ֵ"*/            "\xDF\x15", 255, 4,   1, 39,  0, 0},
                {/*"���ƫ�����ٷֱ�"*/        "\xDF\x16", 255, 1,   2, 43,  0, 0},
                {/*"���ѡ���Ŀ��ٷ���"*/      "\xDF\x17", 255, 1,   2, 44,  0, 0},
                {/*"TermDDOL",1,*/         "\xDF\x14", 0,   128, 0, 46,  0, 0},
                {/*"�ն�����PIN֧������",1,*/"\xDF\x18", 255, 1,   0, 174, 0, 0},
                {/*"�����ֽ����޶�",1,*/     "\x9F\x7B", 255, 6,   0, 175, 0, 0},//huxiang add 091013 for Ec and qPBOC
                {/*"CVM ���ƽ��",1,*/       "\xDF\x21", 255, 6,   0, 181, 0, 0},//huxiang add 091013 for Ec and qPBOC
                {/*"�ǽ��ѻ����ƽ��",1,*/     "\xDF\x20", 255, 6,   0, 187, 0, 0},//huxiang add 091013 for Ec and qPBOC
                {/*"�ǽ�����ѻ����ƽ��",1,*/  "\xDF\x19", 255, 6,   0, 193, 0, 0},//huxiang add 091013 for Ec and qPBOC
        };


/*******************************************************************
��    ��: ����
��    Ȩ: Okolle Ltd,Co.
��������: u8 VerifyCAPK(SDK_ICC_CAPK_STRUCT* capk)
��������: У��CAPK
��ڲ���:

�� �� ֵ: u8 �ɹ��ڷ�OK�ɹ�.ERR��
��ص���: 
��    ע: 
�޸���Ϣ:
********************************************************************/
u8 VerifyCAPK(SDK_ICC_CAPK_STRUCT *capk) {
    u8 checkSum[20], checkData[300];
    short index;

    memset(checkData, 0, 300);

    index = 0;
    memcpy(checkData, capk->RID, 5);
    checkData[5] = capk->CAPKI;
    index = 6;
    memcpy((u8 * ) & checkData[index], capk->Modul, capk->ModulLen);
    index += capk->ModulLen;
    memcpy((u8 * ) & checkData[index], capk->Exponent, capk->ExponentLen);
    index += capk->ExponentLen;
    TraceHex("wlz", "==checkData==\r\n", checkData, index);
    sdkSHA1(checkData, index, checkSum);
    TraceHex("wlz", "==CheckSum==\r\n", checkSum, 20);
    TraceHex("wlz", "==capk->CheckSum==\r\n", capk->CheckSum, 20);
    return OK;
/*	if(memcmp(capk->CheckSum,checkSum,20))
	{
		return ERR;
	}
	else
	{
		return OK;
	}*/
}

/*******************************************************************
��    ��: ����
��    Ȩ: Okolle Ltd,Co.
��������: u8 UnzipCapkDownLoadData(u8 *DataIn,u16 DataLen)
��������: �������
��ڲ���: u8 *DataIn(����)
		  u16 DataLen���ݳ���
�� �� ֵ: u8 �ɹ��ڷ�OK�ɹ�.ERR��
��ص���: CapkDataInfo�����OK���CAPKֵ
��    ע: 
�޸���Ϣ:
********************************************************************/
u8 UnzipCapkDownLoadData(u8 *DataIn, u16 DataLen) {
#if 0
    u8 i;
    u16 index = 0;
     if(gstAppSetting.bIsDebugSwitchOn)
     {

        if(UnzipPagData(DataIn,DataLen,CAPKFILE)==OK)
        {
            for(i=0;i<CAPKDATANUM;i++)
            {
                if(((CapkDataTable[i].flagM & 0x01)==0x01)&&(CapkDataTable[i].bExist==0))
                {
                    return UNSUCCESS;
                }
            }
            if(VerifyCAPK(&AppCapkDataInfo)==OK)
            {
                return SUCCESS;
            }
            else return UNSUCCESS;
        }
        else return UNSUCCESS;
     }
     else
     {
        memcpy(AppCapkDataInfo.RID, &DataIn[index], 5);
        TraceHex("wlz","==RID==\r\n",AppCapkDataInfo.RID, 5);
        index += 5;
        AppCapkDataInfo.CAPKI = DataIn[index];
        index += 1;
        AppCapkDataInfo.ArithInd = DataIn[index];
        index += 1;
        AppCapkDataInfo.HashInd= DataIn[index];
        index += 1;
        memcpy(AppCapkDataInfo.ExpireDate, &DataIn[index], 4);
        index += 4;
        TraceHex("wlz","==ExpireDate==\r\n",AppCapkDataInfo.ExpireDate, 4);
        AppCapkDataInfo.ModulLen= DataIn[index];
        index += 1;
        AppCapkDataInfo.ExponentLen= DataIn[index];
        index += 1;
        memcpy(AppCapkDataInfo.Modul, &DataIn[index], AppCapkDataInfo.ModulLen);
        index += AppCapkDataInfo.ModulLen;
        TraceHex("wlz","==Modul==\r\n",AppCapkDataInfo.Modul, AppCapkDataInfo.ModulLen);
        memcpy(AppCapkDataInfo.Exponent, &DataIn[index], AppCapkDataInfo.ExponentLen);
        index += AppCapkDataInfo.ExponentLen;
        TraceHex("wlz","==Modul==\r\n",AppCapkDataInfo.Exponent, AppCapkDataInfo.ExponentLen);
        AppCapkDataInfo.CheckSumLen = 0x14;
        Trace("wlz","==CheckSumLen %d==\r\n", AppCapkDataInfo.CheckSumLen);
        memcpy(AppCapkDataInfo.CheckSum, &DataIn[index],AppCapkDataInfo.CheckSumLen);
         index += 0x14;
        TraceHex("wlz","==CheckSum==\r\n",AppCapkDataInfo.CheckSum, AppCapkDataInfo.CheckSumLen);
        if(VerifyCAPK(&AppCapkDataInfo)==OK)
        {
            return SUCCESS;
        }
        else return UNSUCCESS;
     }
#endif
    return UNSUCCESS;
}

/*******************************************************************
��    ��: ����
��    Ȩ: Okolle Ltd,Co.
��������: u8 UnzipPagData(u8 *DataIn,u8 DataLen,u8 UnzipType)
��������: ���
��ڲ���: UnzipType�������
		1.CAPKFILE
		2.NORMALTRADE
		3.SYSINIFILE
�� �� ֵ: u8 �ɹ��ڷ�OK�ɹ�.ERR��
��ص���:
��    ע: 
�޸���Ϣ:
********************************************************************/
s32 UnzipPagData(u8 *DataIn, u16 DataLen, u8 UnzipType) {
    u16 index, k, i, j, t, len;
    u8 retCode, bInTable;

    //memcpy((u8*)&TermDataTableTemp,(u8*)&TermDataTable_Stored,TERMDATANUM*sizeof(AppDATAELEMENT));
    //memcpy((u8*)&ICCDataTableTemp,(u8*)&ICCDataTable_Stored,ICCDATANUM*sizeof(AppDATAELEMENT));
    memcpy((u8 * ) & CapkDataTable, (u8 * ) & DownCapkDataTable_Stored,
           CAPKDATANUM * sizeof(AppDATAELEMENT));
    memcpy((u8 * ) & DownIniDataTable, (u8 * ) & DownInitDataTable_Stored,
           APPINIDATANUM * sizeof(AppDATAELEMENT));

    index = 0;

    if (UnzipType == CAPKFILE)    //CAPKFILE
    {
        while (index < DataLen) {
            if (index >= DataLen) {
                retCode = ERR;
                break;
            }
            if (DataIn[index] == 0xFF || DataIn[index] == 0x00) {
                index++;
                continue;
            }
            bInTable = 0;
            k = DataIn[index];
            for (i = 0; i < CAPKDATANUM; i++) {
                if (k != CapkDataTable[i].Tag[0]) continue;
                if ((k & 0x1F) == 0x1F) {
                    if (DataIn[index + 1] != CapkDataTable[i].Tag[1]) continue;
                    index++;
                }
                index++;
                if (DataIn[index] <= 127) {
                    len = DataIn[index];
                    index++;
                }
                else {
                    len = 0;
                    t = DataIn[index] & 0x7F;
                    for (j = 1; j <= t; j++) {
                        len = len * 256 + DataIn[index + j];
                    }
                    index += t + 1;
                }

                if (CapkDataTable[i].bExist ==
                    1)//if the data is primitive data object and has existed, terminate trans.
                {
                    return SDK_IccDataRedundErr;
                }

                if (CapkDataTable[i].Len1 <= CapkDataTable[i].Len2) {
                    if (len < CapkDataTable[i].Len1 || len > CapkDataTable[i].Len2)
                        return SDK_IccDataFormatErr;
                }
                else {
                    if (len != CapkDataTable[i].Len2)
                        return SDK_IccDataFormatErr;
                }
                //added by Gu Bohua on 2002.09.28. for JCB test.
                if (CapkDataTable[i].bAmt == 2)
                    sdkAscToBcd((u8 * ) & AppCapkDataInfo + CapkDataTable[i].address,
                                DataIn + index, len);
                else
                    memcpy((u8 * ) & AppCapkDataInfo + CapkDataTable[i].address, DataIn + index,
                           len);
                if (CapkDataTable[i].Len1 <=
                    CapkDataTable[i].Len2) //if len is variable(presume that the len is right),than add len field.
                {
                    *((u8 * ) & AppCapkDataInfo + CapkDataTable[i].address - 1) = (u8) len;

                }

                CapkDataTable[i].bExist = 1;
                bInTable = 1;
                index += len;
                break;
            }
            if (!bInTable) {
                if ((k & 0x1F) == 0x1F) {
                    index++;
                }
                index++;
                if (DataIn[index] <= 127) {
                    len = DataIn[index];
                    index++;
                }
                else {
                    len = 0;
                    t = DataIn[index] & 0x7F;
                    for (j = 1; j <= t; j++) {
                        len = len * 256 + DataIn[index + j];
                    }
                    index += t + 1;
                }
                index += len;
            }
        };
    }//
    else if (UnzipType == SYSINIFILE)//�����ļ�
    {
        memset((u8 * ) & AppDownIniDataInfo, 0, sizeof(AppDownIniDataInfo));
        while (index < DataLen) {
            if (index >= DataLen) {
                retCode = ERR;
                break;
            }
            if (DataIn[index] == 0xFF || DataIn[index] == 0x00) {
                index++;
                continue;
            }
            bInTable = 0;
            k = DataIn[index];
            for (i = 0; i < APPINIDATANUM; i++) {
                if (k != DownIniDataTable[i].Tag[0]) continue;
                if ((k & 0x1F) == 0x1F) {
                    if (DataIn[index + 1] != DownIniDataTable[i].Tag[1]) continue;
                    index++;
                }
                index++;
                if (DataIn[index] <= 127) {
                    len = DataIn[index];
                    index++;
                }
                else {
                    len = 0;
                    t = DataIn[index] & 0x7F;
                    for (j = 1; j <= t; j++) {
                        len = len * 256 + DataIn[index + j];
                    }
                    index += t + 1;
                }

                if (DownIniDataTable[i].bExist ==
                    1)//if the data is primitive data object and has existed, terminate trans.
                {
                    return SDK_IccDataRedundErr;
                }

                if (DownIniDataTable[i].Len1 <= DownIniDataTable[i].Len2) {
                    if (len < DownIniDataTable[i].Len1 || len > DownIniDataTable[i].Len2)
                        return SDK_IccDataFormatErr;
                }
                else {
                    if (len != DownIniDataTable[i].Len2)
                        return SDK_IccDataFormatErr;
                }
                //added by Gu Bohua on 2002.09.28. for JCB test.
                memcpy((u8 * ) & AppDownIniDataInfo + DownIniDataTable[i].address, DataIn + index,
                       len);
                if (DownIniDataTable[i].Len1 <=
                    DownIniDataTable[i].Len2) //if len is variable(presume that the len is right),than add len field.
                {
                    *((u8 * ) & AppDownIniDataInfo + DownIniDataTable[i].address - 1) = (u8) len;
                }

                DownIniDataTable[i].bExist = 1;
                bInTable = 1;
                index += len;
                break;
            }
            if (!bInTable) {
                if ((k & 0x1F) == 0x1F) {
                    index++;
                }
                index++;
                if (DataIn[index] <= 127) {
                    len = DataIn[index];
                    index++;
                }
                else {
                    len = 0;
                    t = DataIn[index] & 0x7F;
                    for (j = 1; j <= t; j++) {
                        len = len * 256 + DataIn[index + j];
                    }
                    index += t + 1;
                }
                index += len;
            }
        };
    }
    else return ERR;

    return OK;
}

/*******************************************************************
��    ��: ����
��    Ȩ: Okolle Ltd,Co.
��������: u8 UnzipIniDownLoadData(u8 *DataIn,u16 DataLen)
��������: ��CAPK��
��ڲ���: u8 *DataIn(����)
		  u16 DataLen���ݳ���		  
�� �� ֵ: u8 �ɹ��ڷ�OK�ɹ�.ERR��
��ص���: 
��    ע: 
�޸���Ϣ:
********************************************************************/
u8 UnzipIniDownLoadData(u8 *DataIn, u16 DataLen) {
    if (UnzipPagData(DataIn, DataLen, SYSINIFILE) == OK) return SUCCESS;
    else return UNSUCCESS;
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��Aid����
��ڲ���: 
�� �� ֵ: 
��	  ע: 
********************************************************************/
void ReadAidParam(u8 *AidName) {
    u8 rslt, *pData;

    pData = (u8 *) sdkGetMem(sizeof(AppAidParam) + 2);
    memset(pData, 0, sizeof(AppAidParam) + 2);
    rslt = ReadPathOverlap(AidName, pData, sizeof(AppAidParam) + 1);
    switch (rslt) {
        case SDK_FILE_OK:
            AppAidParamNum = pData[0];
            if (AppAidParamNum > 100) AppAidParamNum = 100;
            memcpy((u8 * ) & AppAidParam, &pData[1], sizeof(AppAidParam));
            break;

        default:
            memset((u8 *) AppAidParam, 0, sizeof(AppAidParam));
            AppAidParamNum = 0;
            SaveAidParam(AidName);
            break;
    }

    Trace("wlz", "\r\n------read AIDNum=%d-----\r\n", AppAidParamNum);

    sdkFreeMem(pData);

}

/*******************************************************************
��	  ��: 
��	  Ȩ: �����¹����ɷ����޹�˾
��������:����������aid����������ӵ�Ӧ�����б���
��ڲ���: Index = AID����CAPK����
�� �� ֵ: 
��	  ע: 
********************************************************************/
void ReadAidCapk(u8 type) {
    u8 AidName[10];
    memset(AidName, 0, sizeof(AidName));
    if (TYPEAID == type) strcpy(AidName, "aid");
    else strcpy(AidName, "capk");

    if (TYPEAID == type) ReadAidParam(AidName);
    else ReadCapk(AidName);
}


/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����AidParam����
��ڲ���: 
�� �� ֵ: TRUE, ����ɹ���FALSE: ��¼����
��	  ע: 
********************************************************************/
u8 AppendAidParam(SDK_ICC_EmvSysIni_STRUCT tempAid) {
    u8 i;
    u8 flag = FALSE;

    for (i = 0; i < AppAidParamNum; i++) {
        if (AppAidParam[i].AidLen == tempAid.AidLen &&
            !memcmp(AppAidParam[i].Aid, tempAid.Aid, AppAidParam[i].AidLen))    //��ȸ���
        {
            memcpy(&AppAidParam[i], &tempAid, sizeof(SDK_ICC_EmvSysIni_STRUCT));        //����
            flag = TRUE;
            break;
        }
    }
    if (!flag && AppAidParamNum < 100) {
        memcpy(&AppAidParam[AppAidParamNum], &tempAid,
               sizeof(SDK_ICC_EmvSysIni_STRUCT));    //׷���µ�AID
        AppAidParamNum++;
        flag = TRUE;
    }
    return flag;
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����Aid����
��ڲ���: 
�� �� ֵ:  
��	  ע: 
********************************************************************/
u8 ParseAidParam(u8 *data, u16 len) {
    u32 result;
    result = UnzipIniDownLoadData(data, len);

    Trace("wlz", "UnzipIniDownLoadData =%d\r\n", result);

    if (result != SUCCESS) return result;
    result = AppendAidParam(AppDownIniDataInfo);                //׷��AID

    Trace("wlz", "AppendAidParam =%d\r\n", result);


    if (result != TRUE) result = UNSUCCESS;
    else result = SUCCESS;
    return result;
}

/*******************************************************************
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����Aid������ɾ��һ��aid
��ڲ���: 
�� �� ֵ:  
��	  ע: 
********************************************************************/
u8 ParseAndDeleteAidParam(u8 *data, u16 len) {
    s32 result;
    result = UnzipIniDownLoadData(data, len);

    Trace("wlz", "UnzipIniDownLoadData =%d\r\n", result);

    if (result != SUCCESS) return result;
    result = sdkIccDelOneAID(AppDownIniDataInfo.Aid, AppDownIniDataInfo.AidLen);

    Trace("wlz", "sdkIccDelOneAID =%d\r\n", result);


    if (result != SDK_OK) result = UNSUCCESS;
    else result = SUCCESS;
    return result;
}


/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: �洢Aid����
��ڲ���: 
�� �� ֵ: 
��	  ע: 
********************************************************************/
u8 SaveAidParam(u8 *AidName) {
    u8 *pData, result;

    pData = (u8 *) sdkGetMem(sizeof(AppAidParam) + 2);
    memset(pData, 0, sizeof(AppAidParam) + 2);
    pData[0] = AppAidParamNum;
    memcpy(&pData[1], (u8 *) AppAidParam, sizeof(AppAidParam));
    result = SavePathOverlap(AidName, pData, sizeof(AppAidParam) + 1);
    if (SDK_FILE_OK != result) result = UNSUCCESS;
    else result = SUCCESS;
    sdkFreeMem(pData);
    return result;

}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ɾ��AID�ļ�
��ڲ���: 
�� �� ֵ: 
��	  ע: 
********************************************************************/
u8 DeleteAIDFile(u8 *aidname) {
    DelPathFile(aidname);
    return 0;
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ��CAPK����
��ڲ���: 
�� �� ֵ: 
��	  ע: 
********************************************************************/
void ReadCapk(u8 *CapkName) {
    u8 rslt, *pData;
    pData = (u8 *) sdkGetMem(sizeof(AppTermCAPK) + 2);
    if (pData == NULL) {
        return;
    }
    memset(pData, 0, sizeof(AppTermCAPK) + 2);
    rslt = ReadPathOverlap(CapkName, pData, sizeof(AppTermCAPK) + 1);
    switch (rslt) {
        case SDK_FILE_OK:
            AppTermCAPKNum = pData[0];
            if (AppTermCAPKNum > 100) AppTermCAPKNum = 100;
            memcpy((u8 * ) & AppTermCAPK, &pData[1], sizeof(AppTermCAPK));
            break;

        default:
            memset((u8 *) AppTermCAPK, 0, sizeof(AppTermCAPK));
            AppTermCAPKNum = 0;
            SaveCapk(CapkName);
            break;
    }

    Trace("wlz", "\r\n------read AppTermCAPKNum=%d-----\r\n", AppTermCAPKNum);
    sdkFreeMem(pData);
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����CAPK����
��ڲ���: 
�� �� ֵ: TRUE, ����ɹ���FALSE: ��¼����
��	  ע: 
********************************************************************/
u8 AppendCAPKParam(SDK_ICC_CAPK_STRUCT tempCapk) {
    u8 i;
    u8 flag = FALSE;

    for (i = 0; i < AppTermCAPKNum; i++) {
        if ((!memcmp(tempCapk.RID, AppTermCAPK[i].RID, 5))                    //��ȸ���
            && (tempCapk.CAPKI == AppTermCAPK[i].CAPKI)) {
            memcpy(&AppTermCAPK[i], &tempCapk, sizeof(SDK_ICC_CAPK_STRUCT));            //����CAPK
            flag = TRUE;
            break;
        }
    }
    if (!flag && AppTermCAPKNum < 100) {
        memcpy(&AppTermCAPK[AppTermCAPKNum], &tempCapk,
               sizeof(SDK_ICC_CAPK_STRUCT));    //׷���µ�CAPK
        AppTermCAPKNum++;
        flag = TRUE;
    }

    return flag;
}


/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ����capk����
��ڲ���: 
�� �� ֵ:
��	  ע: 
********************************************************************/
u8 ParseCapk(u8 *data, u16 len) {

    u32 result;
    result = UnzipCapkDownLoadData(data, len);

    Trace("wlz", "UnzipCapkDownLoadData =%d\r\n", result);

    if (result != SUCCESS) return result;

    result = AppendCAPKParam(AppCapkDataInfo);                //׷��CAPK

    Trace("wlz", "AppendCAPKParam =%d\r\n", result);


    if (result != TRUE) result = UNSUCCESS;
    else result = SUCCESS;
    return result;


}

/*******************************************************************
��	  Ȩ: �����¹����ɷ����޹�˾
��������: ������ɾ��һ��capk����
��ڲ���: 
�� �� ֵ:
��	  ע: 
********************************************************************/
u8 ParseAndDeleteCapk(u8 *data, u16 len) {
    s32 result;
    result = UnzipCapkDownLoadData(data, len);

    Trace("wlz", "UnzipCapkDownLoadData =%d\r\n", result);
    if (result != SUCCESS) return result;

    result = sdkIccDelOneCAPK(AppCapkDataInfo.RID, AppCapkDataInfo.CAPKI);
    Trace("wlz", "sdkIccDelOneCAPK =%d\r\n", result);

    if (result != SDK_OK) result = UNSUCCESS;
    else result = SUCCESS;
    return result;
}

/*******************************************************************
��	  ��: ������
��	  Ȩ: �����¹����ɷ����޹�˾
��������: �洢Capk(����
��ڲ���: 
�� �� ֵ: 
��	  ע: 
********************************************************************/
u8 SaveCapk(u8 *CapkName) {
    u8 *pData, result;
    pData = (u8 *) sdkGetMem(sizeof(AppTermCAPK) + 2);
    if (pData == NULL) {
        return UNSUCCESS;
    }
    memset(pData, 0, sizeof(AppTermCAPK) + 2);
    pData[0] = AppTermCAPKNum;
    memcpy(&pData[1], (u8 *) AppTermCAPK, sizeof(AppTermCAPK));
    result = SavePathOverlap(CapkName, pData, sizeof(AppTermCAPK) + 1);
    if (SDK_FILE_OK != result) result = UNSUCCESS;
    else result = SUCCESS;
    sdkFreeMem(pData);
    return result;
}


