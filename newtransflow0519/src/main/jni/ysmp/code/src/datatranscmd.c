cmd_Str const DataTransCmdTab[] = {
        {DATA_TRANSMIT, OnDataTransmit, NULL},
        {DATA_CMD_END,  NULL,           NULL}
};


/*******************************************************************
   ��	 ��: �����
   ��	 Ȩ: �����¹����ɷ����޹�˾
   ��������: ����͸��
   ��ڲ���:
   �� �� ֵ:
   ��	 ע:
 ********************************************************************/
s32 OnDataTransmit(ST_TRANSFLOW *pstTransFlow) {
    u16 usDataLen = pstTransFlow->ReceiveDataLen;
    u8 *pData;
    u16 len;
    u8 ucAuthKey[16] = {0}, deviceSN[64] = {0};
    s32 ret;
    u8 *outdata;
    u16 outdatalen;
    u8 Trankey[] = {0x36, 0x38, 0x34, 0x38, 0x32, 0x35, 0x36, 0x39, 0x36, 0x38, 0x34, 0x38, 0x32,
                    0x35, 0x36, 0x39};

    pData = &pstTransFlow->heReceiveData[0];
    outdata = &pstTransFlow->heReceiveData[0];     //0416Send==Recv

    if (usDataLen > 1024 || usDataLen == 0) {
        return INS_OTHER_ERR;
    }

    //���¼���Щ���ݲ���
    //�¹���MPOS��Կpos�Զ���Э��
    //�·�����ΪXGD-MPOS + 10�ֽ�deviceID + 16�ֽ�MIANKEY + 16�ֽ�AUTHKEY + 16�ֽ�APPKEY
    //��������Ϊ8�ֽ�MIANKEY��checkvalye  + 8�ֽ�AUTHKEY��checkvalye  + 8�ֽ�APPKEY��checkvalye  + �����
    if (usDataLen < 2 + 8 + 10 + 16 + 16 + 16) {
        return INS_SUC;
    }
    len = 2;

    if (memcmp("XGD-MPOS", pstTransFlow->heReceiveData + len, 8)) {
        return INS_SUC;
    }
    len += 8;

    memcpy(ucAuthKey, pstTransFlow->heReceiveData + len, 10);
    len += 10;
    TraceHex("zcl", "MPOS DEVICEID", ucAuthKey, 10);

    //ret = sdkWriteFile(IDFILE, ucAuthKey, 10);        
    ret = sdkSaveOverlap(IDFILE, ucAuthKey, 10);        //20150320  ��ǰ���ļ�д���ɹ�
    Trace("zcl", "����DEVICEID���: ret:%d DevID:%s\r\n", ret, ucAuthKey);
    if (ret != SDK_FILE_OK) {
        return INS_OTHER_ERR;
    }
    //20150320  ������Կ�Ը����豸ID =====start=====
    memset(deviceSN, 0, sizeof(deviceSN));
    sdkBcdToAsc(deviceSN, ucAuthKey, 10);
    //20150320  ������Կ�Ը����豸ID =====end=====

    memcpy(ucAuthKey, pstTransFlow->heReceiveData + len, 16);
    len += 16;
    sdkDes3S(0, ucAuthKey, Trankey);
    sdkDes3S(0, &ucAuthKey[8], Trankey);  //syh 20150408
    TraceHex("zcl", "MPOS KEY", ucAuthKey, 16);
    ret = sdkPEDUpdateTmk(MAINKEYINDEX, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);

    if (ret != SDK_OK) {
        return INS_OTHER_ERR;
    }
    memcpy(ucAuthKey, pstTransFlow->heReceiveData + len, 16);
    sdkDes3S(0, ucAuthKey, Trankey);
    sdkDes3S(0, &ucAuthKey[8], Trankey);  //syh 20150408
    TraceHex("zcl", "MPOS KEY", ucAuthKey, 16);
    len += 16;
    ret = sdkPEDUpdateTmk(AUTHKEYINDES, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);

    if (ret != SDK_OK) {
        return INS_OTHER_ERR;
    }
    memcpy(ucAuthKey, pstTransFlow->heReceiveData + len, 16);
    sdkDes3S(0, ucAuthKey, Trankey);
    sdkDes3S(0, &ucAuthKey[8], Trankey);  //syh 20150408
    TraceHex("zcl", "MPOS KEY", ucAuthKey, 16);
    len += 16;
    ret = sdkPEDUpdateTmk(APPKEYINDES, SDK_PED_DES_TRIPLE, ucAuthKey, SDK_PED_TIMEOUT);

    if (ret != SDK_OK) {
        return INS_OTHER_ERR;
    }
    outdatalen = 2;

    memset(ucAuthKey, 0, sizeof(ucAuthKey));
    sdkPEDTmkDes(MAINKEYINDEX, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, ucAuthKey, ucAuthKey);
    memcpy(outdata + outdatalen, ucAuthKey, 8);
    outdatalen += 8;
    memset(ucAuthKey, 0, sizeof(ucAuthKey));
    sdkPEDTmkDes(AUTHKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, ucAuthKey, ucAuthKey);
    memcpy(outdata + outdatalen, ucAuthKey, 8);
    outdatalen += 8;
    memset(ucAuthKey, 0, sizeof(ucAuthKey));
    sdkPEDTmkDes(APPKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, ucAuthKey, ucAuthKey);
    memcpy(outdata + outdatalen, ucAuthKey, 8);
    outdatalen += 8;

    //20150320  ������Կ�Ը����豸ID =====start=====
    memcpy(MposDeviceId, deviceSN, 20);
    //20150320  ������Կ�Ը����豸ID =====end=====

    memset(deviceSN, 0, sizeof(deviceSN));
    sdkReadPosSn(deviceSN);
    len = strlen(deviceSN);
    memcpy(outdata + outdatalen, deviceSN, len);
    outdatalen += len;

    sdkU16ToBcd(outdata, outdatalen, 2);
    pstTransFlow->usSendDataLen = outdatalen;

    return INS_SUC;
}

