#define MAX_REC_NUM     100 //����¼�� ljh2014-12-16

const cmd_Str StoreCmdTab[] = {
        {STORE_INIT_REC,     OnInitRecord,   NULL},
        {STORE_GET_RECCOUNT, OnGetRecCount,  NULL},
        {STORE_ADD_REC,      OnAddRecord,    NULL},
        {STORE_UPDATE_REC,   OnUpdateRecord, NULL},
        {STORE_GET_REC,      OnGetRecord,    NULL},
        {STORE_CMD_END,      NULL,           NULL}
};

typedef struct STORE_REC_ {
    u16 recNameLen;             //��¼����
    u8 recName[14];             //��¼��,���ǵ��ֶ����Ӹ��ֽ�
    u16 recLen;                         //ÿ����¼����
    u16 index1Offset;           //�����ֶ�1ƫ��
    u16 index1Len;              //�����ֶ�1����
    u16 index2Offset;           //�����ֶ�2ƫ��
    u16 index2Len;              //�����ֶ�2����
} storeRecStr;

//storeRecStr gStoreRecInfo; //�ýṹ����25byte,������Ԥ��25byte
//�����ļ��ĸ�ʽΪ25byte�ṹ����Ϣ+25Ԥ��+N����¼����Ϣ


//zcl ȫ�񸶰汾�ѻ�������Ҫ�Լ��洢��offlineFile�ļ���,û�и��ļ���Ҫ�Լ���,���ܱ���
void InitPbocOfflineFile() {
    u8 pFileName[64] = {0};
    storeRecStr rec;

    //02000000000E00640001	����(�ѻ��ļ��´�½�ṩ�Ĳ���)
    //10�ֽ�: ÿ����¼����2Bytes+�����ֶ�1�ڼ�¼�е�ƫ��2bytes+�����ֶ�1�ĳ���2bytes
    //				 +�����ֶ�2�ڼ�¼�е�ƫ��2bytes+�����ֶ�2�ĳ���2bytes

    memset(&rec, 0, sizeof(storeRecStr));

    sprintf(pFileName, "%s%s", FileNamePrefix, OFFLINEFILE);

    if (!sdkAccessFile(pFileName)) {
        Trace("Lai", "sdkAccessFile %s not exist\r\n", pFileName);
        //zcl û���ļ������ļ�
        rec.recNameLen = strlen(OFFLINEFILE);

        if (rec.recNameLen > 14) {
            rec.recNameLen = 14;
        }
        memcpy(rec.recName, OFFLINEFILE, rec.recNameLen);
        rec.recLen = 512;
        rec.index1Offset = 0;
        rec.index1Len = 0x0E;
        rec.index2Offset = 0x64;
        rec.index2Len = 1;
        sdkWriteFile(pFileName, &rec, RECORD_HEAD_SIZE);
    }
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���ļ���ʼ���洢��¼�ṹ����Ϣ
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: �ļ��ĸ�ʽΪ25byte�ṹ����Ϣ+25Ԥ��+N����¼����Ϣ
 ********************************************************************/
bool GetStoreRecInfo(u8 *pFileName, storeRecStr *pRec) {
    s32 readlen = sizeof(storeRecStr);
    //u8 filename[64] = {0};

    memset(pRec, 0, sizeof(storeRecStr));

    if (!sdkAccessFile(pFileName)) {
        Trace("Lai", "sdkAccessFile %s not exist\r\n", pFileName);
#if 1
        return false;

#else

        //zcl û���ļ������ļ�
        if(!memcmp(FileNamePrefix, pFileName, strlen(FileNamePrefix)))
        {
            strcpy(filename, pFileName + strlen(FileNamePrefix));
        }
        else
        {
            strcpy(filename, pFileName);
        }
        pRec->recNameLen = strlen(filename);

        if(pRec->recNameLen > 14)
        {
            pRec->recNameLen = 14;
        }
        memcpy(pRec->recName, filename, pRec->recNameLen);
        pRec->recLen = 1024;
        sdkWriteFile(pFileName, pRec, RECORD_HEAD_SIZE);
#endif
    }
    s32 ret = sdkReadFile(pFileName, pRec, 0, &readlen);

    if (SDK_FILE_OK == ret) {
        Trace("Lai", "recNameLen:%d %s\r\n", pRec->recNameLen, pRec->recName);
        Trace("Lai", "recLen:%d,index1Offset:%d,index1Len:%d,index2Offset:%d,index2Len:%d\r\n",
              pRec->recLen, pRec->index1Offset, pRec->index1Len,
              pRec->index2Offset, pRec->index2Len);
    }
    else {
        Trace("Lai", "$$$ sdkReadFile %s fail:%d\r\n", pFileName, ret);
        return false;
    }
    return true;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ʼ���洢��¼
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: �յ�������Ḳ��д�ļ�,Ȼ��д����Ϣͷ �����õ��ļ��洢
 ********************************************************************/
s32 OnInitRecord(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnInitRecord =============\r\n");
    u16 nameLen;
    u8 index = 0;
    u8 pFileName[50] = {0};
    u8 buf[RECORD_HEAD_SIZE] = {0};
    storeRecStr rec;

    memset(&rec, 0, sizeof(rec));

    sdkBcdToU16(&nameLen, &pstTransFlow->heReceiveData[index], 2);
    rec.recNameLen = nameLen;
    index += 2;

    if (rec.recNameLen > 12) {
        rec.recNameLen = 12;
    }
    memcpy(rec.recName, &pstTransFlow->heReceiveData[index], rec.recNameLen);
    index += rec.recNameLen;

//	rec.recLen = *(u16*)&pstTransFlow->heReceiveData[index];
    sdkHexToU16(&rec.recLen, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
//	rec.index1Offset = *(u16*)&pstTransFlow->heReceiveData[index];
    sdkHexToU16(&rec.index1Offset, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
//	rec.index1Len = *(u16*)&pstTransFlow->heReceiveData[index];
    sdkHexToU16(&rec.index1Len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
//	rec.index2Offset = *(u16*)&pstTransFlow->heReceiveData[index];
    sdkHexToU16(&rec.index2Offset, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
//	rec.index2Len = *(u16*)&pstTransFlow->heReceiveData[index];
    sdkHexToU16(&rec.index2Len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;


//	rec.recLen = U16_ENDIAN(rec.recLen);
//	rec.index1Offset = U16_ENDIAN(rec.index1Offset);
//	rec.index1Len = U16_ENDIAN(rec.index1Len);
//	rec.index2Offset =U16_ENDIAN(rec.index2Offset);
//	rec.index2Len = U16_ENDIAN(rec.index2Len);

    memcpy(buf, &rec, sizeof(rec));

    sprintf(pFileName, "%s%s", FileNamePrefix, rec.recName);

    //�ж��Ƿ���� ���ھ�ɾ�� ljh2014-12-16
    if (sdkAccessFile(pFileName)) {
        sdkDelFile(pFileName);
    }
    u8 ret = sdkWriteFile(pFileName, buf, sizeof(buf));

    if (SDK_FILE_OK != ret) {
        Trace("Lai", "$$$ sdkWriteFile %s fail ret:%d\r\n", pFileName, ret);
        return INS_OTHER_ERR;
    }
    Trace("Lai", "sdkWriteFile %s ok filesize:%d\r\n", pFileName, sdkGetFileSize(pFileName));

    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�洢��¼��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע: �����ļ���С�ж��м�����¼,������Ƕ��ļ��洢������ô��
 ********************************************************************/
s32 OnGetRecCount(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnGetRecCount =============\r\n");

    u16 recNameLen;
    u8 recName[13] = {0};
    u8 pFileName[50] = {0};
    u32 recCount;
    storeRecStr rec;

    sdkBcdToU16(&recNameLen, &pstTransFlow->heReceiveData[0], 2);
    memcpy(recName, &pstTransFlow->heReceiveData[2], recNameLen);
    sprintf(pFileName, "%s%s", FileNamePrefix, recName);

    s32 filesize = sdkGetFileSize(pFileName);

    if (filesize <= 0) {
        Trace("Lai", "$$$ OnGetRecCount filesize < 0\r\n");
        return INS_OTHER_ERR;
        //Xiaox 15-1-8 ���ݲ��ԣ�termparm�ļ������ȳ�ʼ���ģ����ȷ���ȡ�ļ����ݣ��ٷ����²���������������ﷵ��ok�ˣ�����ͻ����ʧ�ܣ�
        //���ﷵ��ʧ�ܺ�apk���̻��ɷ���ȡ�ļ����ݷ�ʧ�ܣ��ٷ���ȡ������ʧ�ܣ��ٷ���ʼ��
    }
    GetStoreRecInfo(pFileName, &rec);

    if (filesize < RECORD_HEAD_SIZE) {
        return INS_OTHER_ERR;
    }
    else {
        recCount = (filesize - RECORD_HEAD_SIZE) / (rec.recLen + 2); //LL+����
    }
    Trace("Lai", "OnGetRecCount filesize:%d rec.recLen : %d\r\n", filesize, rec.recLen);
//	recCount = U32_ENDIAN(recCount);

//	memcpy(pstTransFlow->heSendData,&recCount,4);

    pstTransFlow->usSendDataLen = 4;
    sdkU32ToHex(pstTransFlow->heReceiveData, recCount, 4);      //0416Send==Recv
    Trace("Lai", "OnGetRecCount recCount:%d\r\n", recCount);
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���Ӵ洢��¼
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnAddRecord(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnAddRecord =============\r\n");

    u8 recName[14] = {0};
    u8 pFileName[50] = {0};
    u16 recCount = 0;
    u16 index = 0;
    s32 filesize = 0;    //ljh2014-12-16    
    s32 key = 0;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv

    storeRecStr rec;

    sdkBcdToU16(&recCount, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(recName, &pstTransFlow->heReceiveData[index], recCount);
    index += recCount;
    sprintf(pFileName, "%s%s", FileNamePrefix, recName);

    if (!GetStoreRecInfo(pFileName, &rec)) {
        Trace("Lai", "$$$ OnAddRecord !GetStoreRecInfo \r\n");
        return INS_OTHER_ERR;
    }
    u8 *pBuf = sdkGetMem(rec.recLen + 2);

    if (pBuf == NULL) {
        Trace("Lai", "$$$ OnAddRecord pBuf = sdkGetMem NULL \r\n");
        return INS_OTHER_ERR;
    }
    pstTransFlow->usSendDataLen = 1;

    memset(pBuf, 0, rec.recLen + 2);

    recCount = 0;
    sdkBcdToU16(&recCount, &pstTransFlow->heReceiveData[index], 2);   //index += 2;


    if (recCount > rec.recLen)    //�Ƿ�Ҫ+2 LL
    {
        Trace("Lai", "$$$ OnAddRecord too long \r\n");
        pSend[0] = 0x02;
        sdkFreeMem(pBuf);
        return INS_OTHER_ERR;
    }
    memcpy(pBuf, &pstTransFlow->heReceiveData[index], 2 + recCount); //LL..1024

    TraceHex("Lai", "AddRecord", pBuf, 2 + recCount);

    //�жϼ�¼���� ��������¼��������ʾ����(�������һ����¼) ljh2014-12-16
    filesize = sdkGetFileSize(pFileName);
    recCount = (filesize - RECORD_HEAD_SIZE) / (rec.recLen + 2);

    if (recCount >= MAX_REC_NUM) {
        //��ʾ���Ƿ�
        key = sdkDispMsgBox("��¼�����Ƿ񸲸�?", "[ȷ��] ���� \r[ȡ��] ������", 0,
                            SDK_KEY_ENTER | SDK_KEY_ESC);

        if (key == SDK_KEY_ENTER) {
            if (SDK_FILE_OK !=
                sdkInsertFile(pFileName, pBuf, RECORD_HEAD_SIZE + (recCount - 1) * (rec.recLen + 2),
                              rec.recLen + 2)) {
                Trace("Lai", "$$$ OnAddRecord sdkInsertFile %s fail\r\n", pFileName);
                sdkFreeMem(pBuf);
                pSend[0] = 0x01;
                return INS_OTHER_ERR;
            }
        }
        else {
            sdkFreeMem(pBuf);
            pSend[0] = 0x01;
            return INS_OTHER_ERR;
        }
    }
    else if (SDK_FILE_OK != sdkAppendFile(pFileName, pBuf, rec.recLen + 2)) {
        Trace("Lai", "$$$ sdkAppendFile %s fail\r\n", pFileName);
        sdkFreeMem(pBuf);
        pSend[0] = 0x01;
        return INS_OTHER_ERR;
    }
    Trace("Lai", "sdkAppendFile %s ok filesize:%d\r\n", pFileName, sdkGetFileSize(pFileName));

    pSend[0] = 0x00;
    sdkFreeMem(pBuf);
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���Ҵ洢��¼
   ��ڲ���:
   �� �� ֵ: ���ؼ�¼��,�Ҳ�������0
   ��	 ע: ֻ�����ڶ��ļ��洢
 ********************************************************************/
u32 FindRecord(u32 recNo, u8 *pIndex1, u16 indexLen1, u8 *pIndex2, u16 indexLen2,
               storeRecStr *pRec) {
    u8 pFileName[50] = {0};
    s32 filesize = 0;
    u16 i;
    u8 indexBuf1[32] = {0}, indexBuf2[32] = {0};
    u8 tmpbuf1[32], tmpbuf2[32];
    bool isRecFound = false;
    u8 *pRecData = NULL;
    u16 recCount = 0;
    s32 readLen = 0;

    sprintf(pFileName, "%s%s", FileNamePrefix, pRec->recName);
    filesize = sdkGetFileSize(pFileName);
    Trace("Lai", "### recNo:%d filesize = %d pRec->recLen = %d\r\n", recNo, filesize, pRec->recLen);

    if (filesize <= RECORD_HEAD_SIZE)  //û�м�¼
    {
        Trace("Lai", "$$$ filesize<=RECORD_HEAD_SIZE\r\n");
        return 0;
    }
    recCount = (filesize - RECORD_HEAD_SIZE) / (pRec->recLen + 2);
    readLen = pRec->recLen + 2;

    if (recNo != 0) {
        if (recNo <= recCount) {
            return recNo;
        }
        else {
            return 0;
        }
    }

    if (pRec->index1Len + pRec->index2Len <= 0) {
        return 0;
    }
    pRecData = sdkGetMem(readLen);    //Xiaox 15-1-8 ����ԭ������2�ֽڣ����汬��

    if (NULL == pRecData) {
        Trace("Lai", "$$$ NULL == pRecData\r\n");
        return 0;
    }
    memset(pRecData, 0, readLen);

    for (i = 0; i < recCount; ++i) {
        sdkReadFile(pFileName, pRecData, RECORD_HEAD_SIZE + i * (pRec->recLen + 2), &readLen);

        if (readLen != (pRec->recLen + 2)) {
            Trace("Lai", "$$$ read len error\r\n");
            sdkFreeMem(pRecData);
            return 0;
        }
        Trace("Lai", "--------------\r\n");
        Trace("Lai", "pRec->index1Offset+SKIP_LEN = %d\r\npRec->index1Len = %d\r\n",
              pRec->index1Offset + SKIP_LEN, pRec->index1Len);
        Trace("Lai", "pRec->index2Offset+SKIP_LEN = %d\r\npRec->index2Len = %d\r\n",
              pRec->index2Offset + SKIP_LEN, pRec->index2Len);
        Trace("Lai", "--------------\r\n");
        memcpy(indexBuf1, &pRecData[pRec->index1Offset + SKIP_LEN], pRec->index1Len);
        memcpy(indexBuf2, &pRecData[pRec->index2Offset + SKIP_LEN], pRec->index2Len);
        memset(tmpbuf1, 0, sizeof(tmpbuf1));
        memset(tmpbuf2, 0, sizeof(tmpbuf2));
        memcpy(tmpbuf1, pIndex1, indexLen1);      //Ϊ�˳��Ȳ�һ�µıȽ϶����һ��
        memcpy(tmpbuf2, pIndex2, indexLen2);
        Trace("Lai", "==============\r\n");
        Trace("Lai", "read index1:%s,index2:%s\r\n", indexBuf1, indexBuf2);
        Trace("Lai", "read tmpbuf1:%s,tmpbuf2:%s\r\n", tmpbuf1, tmpbuf2);
        Trace("Lai", "==============\r\n");
        //Xiaox 15-1-8 fix ����������������ֻ��һ������
//		if( memcmp(tmpbuf1,indexBuf1,pRec->index1Len)==0 &&
//			memcmp(tmpbuf2,indexBuf2,pRec->index2Len)==0 )
//		{
//			isRecFound = true;
//			break;
//		}
        isRecFound = false;

        if (indexLen1 > 0) {
            isRecFound = false;

            if (memcmp(tmpbuf1, indexBuf1, pRec->index1Len) == 0) {
                isRecFound = true;
            }
        }

        if (indexLen2 > 0) {
            isRecFound = false;

            if (memcmp(tmpbuf2, indexBuf2, pRec->index2Len) == 0) {
                isRecFound = true;
            }
        }

        if (isRecFound) {
            break;
        }
    }

    sdkFreeMem(pRecData);

    if (isRecFound == true) {
        return i + 1;
    }
    return 0;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ���´洢��¼
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnUpdateRecord(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnUpdateRecord =============\r\n");

    u16 index = 0;
    u16 recNamelen;
    u8 recName[13] = {0};
    u32 recNo, recIndex;
    u16 index1len, index2len, recConLen;
    u8 indexBuf1[32] = {0}, indexBuf2[32] = {0};
    u8 pFileName[50] = {0};

    u8 pTmpFileName[50] = {0};  //ljh2014-12-16
    u8 *pTmpFileBuf = NULL;
    u16 tmpSize = 0;  //��ʱ�ļ���buf��С
    u16 readPos;
    u8 i = 0;
    s32 readLen = 0;
    u16 recCount = 0;
    s32 filesize = 0;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    storeRecStr rec;

    sdkBcdToU16(&recNamelen, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(recName, &pstTransFlow->heReceiveData[index], recNamelen);
    index += recNamelen;
    sprintf(pFileName, "%s%s", FileNamePrefix, recName);

    if (!GetStoreRecInfo(pFileName, &rec)) {
        Trace("Lai", "$$$ OnUpdateRecord !GetStoreRecInfo\r\n");
        return INS_OTHER_ERR;
    }
    memcpy(&recNo, &pstTransFlow->heReceiveData[index], 4);
    index += 4;
    recNo = U32_ENDIAN(recNo);
    sdkBcdToU16(&index1len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(indexBuf1, &pstTransFlow->heReceiveData[index], index1len);
    index += index1len;

    sdkBcdToU16(&index2len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(indexBuf2, &pstTransFlow->heReceiveData[index], index2len);
    index += index2len;

    sdkBcdToU16(&recConLen, &pstTransFlow->heReceiveData[index], 2);   //index += 2;

    if (recConLen > rec.recLen) {
        Trace("Lai", "$$$ OnUpdateRecord too long\r\n");
        pstTransFlow->usSendDataLen = 1;
        pSend[0] = 0x02;
        return INS_OTHER_ERR;
    }

    if ((recIndex = FindRecord(recNo, indexBuf1, index1len, indexBuf2, index2len, &rec)) == 0) {
        Trace("Lai", "$$$ OnUpdateRecord FindRecord fail\r\n");
        return INS_OTHER_ERR;        //Xiaox
    }
    Trace("Lai", "recIndex:%d\r\n", recIndex);

    //�жϼ�¼����Ϊ0,�����¼ ljh2014-12-16
    if (recConLen == 0) {
        //������ʱ�ļ� ������Ҫ���µ����ݿ�������ʱ�ļ�
        //ɾ��ԭ�ļ� ������޸���ʱ�ļ�������Ϊԭ�ļ�������


        //��ȡ��¼�����ͱ�����¼����λ��
        filesize = sdkGetFileSize(pFileName);
        recCount = (filesize - RECORD_HEAD_SIZE) / (rec.recLen + 2);

        if ((rec.recLen + 2) < RECORD_HEAD_SIZE)      //�ж�Ҫ����Ĵ�С
        {
            tmpSize = RECORD_HEAD_SIZE;
        }
        else {
            tmpSize = rec.recLen + 2;
        }
        pTmpFileBuf = sdkGetMem(tmpSize);

        if (pTmpFileBuf == NULL) {
            Trace("Lai", "$$$ OnUpdateRecord pTmpFileBuf = sdkGetMem NULL \r\n");
            pstTransFlow->usSendDataLen = 1;
            pSend[0] = 0x02;
            return INS_OTHER_ERR;
        }
        memset(pTmpFileBuf, 0, tmpSize);
        readLen = RECORD_HEAD_SIZE;

        strcpy(pTmpFileName, pFileName);
        strcat(pTmpFileName, ".tmp");

        sdkDelFile(pTmpFileName);           //zcl ��ɾ

        if (sdkReadFile(pFileName, pTmpFileBuf, 0, &readLen) != SDK_FILE_OK) {
            Trace("Lai", "$$$ OnUpdateRecord sdkReadFile %s fail\r\n", pFileName);
            pstTransFlow->usSendDataLen = 1;
            pSend[0] = 0x02;
            sdkFreeMem(pTmpFileBuf);
            return INS_OTHER_ERR;
        }

        if (SDK_FILE_OK != sdkWriteFile(pTmpFileName, pTmpFileBuf, readLen))      //д��Ϣͷ
        {
            Trace("Lai", "$$$ OnUpdateRecord sdkWriteFile %s fail\r\n", pFileName);
            pstTransFlow->usSendDataLen = 1;
            pSend[0] = 0x02;
            sdkFreeMem(pTmpFileBuf);
            return INS_OTHER_ERR;
        }

        for (i = 1; i < recIndex; i++)  //�ü�¼֮ǰ����Ҫ����
        {
            memset(pTmpFileBuf, 0, tmpSize);
            readLen = rec.recLen + 2;
            readPos = RECORD_HEAD_SIZE + (i - 1) * (rec.recLen + 2);

            if (sdkReadFile(pFileName, pTmpFileBuf, readPos, &readLen) != SDK_FILE_OK) {
                Trace("Lai", "$$$ OnUpdateRecord sdkReadFile %s fail\r\n", pFileName);
                pstTransFlow->usSendDataLen = 1;
                pSend[0] = 0x02;
                sdkFreeMem(pTmpFileBuf);
                return INS_OTHER_ERR;
            }

            if (SDK_FILE_OK != sdkAppendFile(pTmpFileName, pTmpFileBuf, readLen))          //׷��
            {
                Trace("Lai", "$$$ OnUpdateRecord 111 sdkAppendFile %s fail\r\n", pFileName);
                pstTransFlow->usSendDataLen = 1;
                pSend[0] = 0x02;
                sdkFreeMem(pTmpFileBuf);
                return INS_OTHER_ERR;
            }
        }

        for (i = recIndex + 1; i <= recCount; i++) //�ü�¼֮�����Ҫ����     //20150114 modify by yc
        {
            memset(pTmpFileBuf, 0, tmpSize);
            readLen = rec.recLen + 2;
            readPos = RECORD_HEAD_SIZE + (i - 1) * (rec.recLen + 2);

            if (sdkReadFile(pFileName, pTmpFileBuf, readPos, &readLen) != SDK_FILE_OK) {
                Trace("Lai", "$$$ OnUpdateRecord sdkReadFile %s fail\r\n", pFileName);
                pstTransFlow->usSendDataLen = 1;
                pSend[0] = 0x02;
                sdkFreeMem(pTmpFileBuf);
                return INS_OTHER_ERR;
            }

            if (SDK_FILE_OK != sdkAppendFile(pTmpFileName, pTmpFileBuf, readLen))          //׷��
            {
                Trace("Lai", "$$$ OnUpdateRecord 222 sdkAppendFile %s fail\r\n", pFileName);
                pstTransFlow->usSendDataLen = 1;
                pSend[0] = 0x02;
                sdkFreeMem(pTmpFileBuf);
                return INS_OTHER_ERR;
            }
        }

        sdkDelFile(pFileName);
        sdkRenameFile(pTmpFileName, pFileName);

        sdkFreeMem(pTmpFileBuf);
    }
    else if (SDK_FILE_OK != sdkInsertFile(pFileName, &pstTransFlow->heReceiveData[index],
                                          RECORD_HEAD_SIZE + (recIndex - 1) * (rec.recLen + 2),
                                          2 + recConLen)) {
        Trace("Lai", "$$$ sdkInsertFile %s fail\r\n", pFileName);
        pstTransFlow->usSendDataLen = 1;
        pSend[0] = 0x01;
        return INS_OTHER_ERR;
    }
    Trace("Lai", "sdkInsertFile %s ok filesize:%d\r\n", pFileName, sdkGetFileSize(pFileName));
    pstTransFlow->usSendDataLen = 1;
    pSend[0] = 0x00;
    return INS_SUC;
}

/*******************************************************************
   ��	 ��: laijinhong
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ��ȡ�洢��¼
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnGetRecord(ST_TRANSFLOW *pstTransFlow) {
    Trace("Lai", "enter OnGetRecord =============\r\n");

    u16 index = 0;
    u16 recNamelen;
    u8 recName[13] = {0};
    u32 recNo, recIndex;
    u16 index1len, index2len;
    u8 indexBuf1[32] = {0}, indexBuf2[32] = {0};
    u8 pFileName[50] = {0};
    u16 len;
    u8 *data;

    u8 *pSend = &pstTransFlow->heReceiveData[0];          //0416Send==Recv
    storeRecStr rec;

    //pstTransFlow->usSendDataLen = 2;  //�Ҳ�����¼ ҲҪ����LLΪ0000, ljh2015-01-14

    sdkBcdToU16(&recNamelen, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(recName, &pstTransFlow->heReceiveData[index], recNamelen);
    index += recNamelen;
    sprintf(pFileName, "%s%s", FileNamePrefix, recName);

    if (!GetStoreRecInfo(pFileName, &rec)) {
        Trace("Lai", "$$$ OnGetRecord !GetStoreRecInfo\r\n");
        return INS_OTHER_ERR;
        //Xiaox 15-1-8 ���ݲ��ԣ�termparm�ļ������ȳ�ʼ���ģ����ȷ���ȡ�ļ����ݣ��ٷ����²���������������ﷵ��ok�ˣ�����ͻ����ʧ�ܣ�
        //���ﷵ��ʧ�ܺ�apk���̻��ɷ���ȡ�ļ����ݷ�ʧ�ܣ��ٷ���ȡ������ʧ�ܣ��ٷ���ʼ��
    }
    memcpy(&recNo, &pstTransFlow->heReceiveData[index], 4);
    index += 4;
    recNo = U32_ENDIAN(recNo);

    sdkBcdToU16(&index1len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(indexBuf1, &pstTransFlow->heReceiveData[index], index1len);
    index += index1len;

    sdkBcdToU16(&index2len, &pstTransFlow->heReceiveData[index], 2);
    index += 2;
    memcpy(indexBuf2, &pstTransFlow->heReceiveData[index], index2len);   //index += index2len;

    if ((recIndex = FindRecord(recNo, indexBuf1, index1len, indexBuf2, index2len, &rec)) == 0) {
        Trace("Lai", "$$$ OnGetRecord FindRecord fail\r\n");
        //return INS_OTHER_ERR;
        //20150324 �人���̲��ԣ��Ҳ������׼�¼����Ϊ�գ������Ƿ��ش���
        pstTransFlow->usSendDataLen = 2;
        memset(pSend, 0, pstTransFlow->usSendDataLen);
        return INS_SUC;
    }
    else {
        Trace("Lai", "### OnGetRecord FindRecord %d\r\n", recIndex);
    }
    s32 readlen = rec.recLen + 2;

    data = (u8 *) sdkGetMem(readlen);
    memset(data, 0, readlen);

    if (data == NULL) {
        return INS_OTHER_ERR;
    }

    if (SDK_FILE_OK !=
        sdkReadFile(pFileName, data, RECORD_HEAD_SIZE + (recIndex - 1) * (rec.recLen + 2),
                    &readlen)) //���Զ࿽������,��usSendDataLen���;���
    {
        Trace("Lai", "$$$ sdkReadFile %s fail\r\n", pFileName);
        sdkFreeMem(data);                  //20150330 �ڴ�й©������ͷ�
        return INS_OTHER_ERR;
    }

    if (readlen != rec.recLen + 2) {
        sdkFreeMem(data);                  //20150330 �ڴ�й©������ͷ�
        return INS_OTHER_ERR;
    }
    pstTransFlow->usSendDataLen = 2;
    sdkBcdToU16(&len, data, 2);
    pstTransFlow->usSendDataLen = len + 2;
    memcpy(pSend, data, pstTransFlow->usSendDataLen);
    sdkFreeMem(data);
    Trace("Lai", "sdkReadFile %s ok\r\n", pFileName);

    TraceHex("Lai", "", pSend, pstTransFlow->usSendDataLen);

    return INS_SUC;
}

