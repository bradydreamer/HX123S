extern bool gIsMachineK200P;

cmd_Str const SecurityAuthCmdTab[] = {

        {DEVICEINFO_ENUM,   OnGetDeviceInfo, NULL},
        {RANDOMNUM_ENUM,    OnGetRandomNum,  NULL},
        {DEVICEAUTH_ENUM,   OnDeviceAuth,    NULL},
        {SECURITY_END_ENUM, NULL,            NULL},
};


/*****************************************************************************
** Descriptions:		��ȡ�豸Ӳ�����
** Parameters:    		ST_TRANSFLOW
** Returned value:	Ӳ����ų���
** Created By:		DongChangQiang 2014.08.20
** Remarks: 		
*****************************************************************************/
u16 getDeviceSN(u8 *devicebuf) {
    u16 deviceLen = 0;
    //sdkBcdToU16(&deviceLen, deviceSN, 2);
    //memcpy(devicebuf, &deviceSN[2], deviceLen);

    deviceLen = 10;
    sdkAscToBcd(devicebuf, MposDeviceId, 20);
    return deviceLen;
}

/*****************************************************************************
** Descriptions:		��ȡ�豸��Ϣ
** Parameters:    		ST_TRANSFLOW
** Returned value:	
** Created By:		DongChangQiang 2014.07.10
** Remarks: 			1. ��������ȡ������SN	 ��
					2. �̼��汾ȡ����:  fsiming
*****************************************************************************/
s32 OnGetDeviceInfo(ST_TRANSFLOW *pstTransFlow) {
    u8 count = 0;
    u16 bcdLen = 0;
    s32 deviceSNLen = 0;
    u8 deviceSN[32];
    u8 deviceid[16];

    //DEVICE_INFO deviceinfo;
    u8 *pSend = pstTransFlow->heReceiveData;       //0416Send==Recv


    //Xiaox 15-1-8 ����д�󶨺�̨��sn�ţ�12λ�ұ��ո�
    memset(deviceSN, 0, sizeof(deviceSN));
    sdkReadPosSn(deviceSN);
#ifdef DEBUG_CODE_TEST
    //��ҵ�����ƽ̨��Ҫ�󶨻����
    strcpy(deviceSN,"G2000001");
#endif
    memset(pSend + count, ' ', 12);
    bcdLen = strlen(deviceSN) > 12 ? 12 : strlen(deviceSN);
    memcpy(pSend + count, deviceSN + strlen(deviceSN) - bcdLen, bcdLen);        //ȡ��12���ֽ�
    count += 12;


    //	�豸���˻�״̬
    //pSend[count] = 0xff;
    pSend[count] = 0;
    count += 1;

    //	Ӧ�ð汾
    memset(&pSend[count], 0, 16);
    memcpy(&pSend[count], VERSION_R, strlen(VERSION_R) > 16 ? 16 : strlen(VERSION_R));
    count += 16;

    //	����
    memset(&pSend[count], 0, 10);
    count += 10;

    //	�豸״̬

    if (gIsMachineK200P)
        pSend[count] = 0x7D;
    else
        pSend[count] = 0x7F;


    count += 1;

    //	�̼��汾
    memset(&pSend[count], 0, 16);
    sdkSysGetFirmwareVer(&pSend[count], 2);
    count += 16;

    // �ͻ����к�
    //sdkAscToBcd(deviceSN, "00011003130000579254", 20);          //newland
    sdkAscToBcd(deviceid, MposDeviceId, 20);
    deviceSNLen = 10;

    //������豸ID��10���ֽ� �����ṩ
    bcdLen = deviceSNLen > 10 ? 10 : deviceSNLen;
    sdkU16ToBcd(&pSend[count], bcdLen, 2);
    count += 2;

    memcpy(&pSend[count], &deviceid[0], bcdLen);
    count += bcdLen;


    //	��Կ���к�
#if     1           //20150403 ������ʵ��KSN
    bcdLen = 30;
    sdkU16ToBcd(&pSend[count], bcdLen, 2);
    count += 2;
    memset(&pSend[count], 0,
           30);           //KSN���س���ʱ�̶�30�ֽڣ���������KSNÿ��ֻ��8�ֽڣ���Ҫ�ں��������ֽڵ�0�����ǽ��´�½����Ϣ������
    memcpy(&pSend[count], MposPinKsn, 8);
    count += 10;
    memcpy(&pSend[count], MposTrackKsn, 8);
    count += 10;
    memcpy(&pSend[count], MposPbocKsn, 8);
    count += 10;
#else
    bcdLen = 30;
    sdkU16ToBcd(&pSend[count], bcdLen, 2);
    count += 2;

    memset(&pSend[count], 0, 10);
    count += 10;

    pSend[count] = 0x10;
    count++;

    memset(&pSend[count], 0, 9);
    count += 9;

    pSend[count] = 0x20;
    count++;

    memset(&pSend[count], 0, 9);
    count += 9;
#endif

//	0x02 0x00   K200
//	0x47 0x02   G2
//	�¹�����0x00 0x05

    //	��ƷID //Xiaox 15-1-8
    if (gIsMachineK200P) {
        pSend[count] = 0x02;
        count++;
        pSend[count] = 0x00;
        count++;
    }
    else {
        pSend[count] = 0x47;
        count++;
        pSend[count] = 0x02;
        count++;
    }

    //	����ID
    pSend[count] = 0x00;
    count += 1;
    pSend[count] = 0x05;
    count += 1;

    //	����SN  ��
    //strcpy(deviceSN, "G2000001");
    deviceSNLen = strlen(deviceSN);
    sdkU16ToBcd(&pSend[count], deviceSNLen, 2);
    count += 2;
    memcpy(&pSend[count], &deviceSN[0], deviceSNLen);
    count += deviceSNLen;


    //	Boot  �汾
    sdkSysGetFirmwareVer(&pSend[count + 2], 0);
    bcdLen = strlen(&pSend[count + 2]);
    bcdLen = bcdLen > 40 ? 40 : bcdLen;
    sdkU16ToBcd(&pSend[count], bcdLen, 2);
    count += 2;

    count += bcdLen;

    pstTransFlow->usSendDataLen = count;

    Trace("dcq", "getDeviceInfo: %d", count);

    return INS_SUC;
}

/*****************************************************************************
** Descriptions:		��ȡ�����
** Parameters:    		ST_TRANSFLOW
** Returned value:	
** Created By:		DongChangQiang 2014.07.10
** Remarks: 		
*****************************************************************************/
s32 OnGetRandomNum(ST_TRANSFLOW *pstTransFlow) {
    u8 *pSend = NULL;
    u8 random[8];
    s32 ret = -1;

    pSend = pstTransFlow->heReceiveData;       //0416Send==Recv

    ret = sdkGetRandom(random, 8);
    memcpy(pSend, random, 8);
    if (ret < 0)
        ret = INS_OTHER_ERR;
    else
        ret = INS_SUC;

    TraceHex("dcq", "getRandom", random, 8);

    pstTransFlow->usSendDataLen = 8;

    return ret;
}

/*****************************************************************************
** Descriptions:		�豸��֤
** Parameters:    		ST_TRANSFLOW
** Returned value:	
** Created By:		DongChangQiang 2014.07.10
** Remarks: 		
*****************************************************************************/

/*
	ID		������				����			��ע

	1		��֤ģʽ			  1 				0x01 �Գ���֤

	2		��֤��������	LL...256			һ��Ϊ��̨�·��������
									
*/

//	����ֵ

/*
	ID		������				����			��ע

	1		��֤�������	LL...256	 		�豸��֤�������

	2		��������			LL...24			һ��Ϊ�豸ID	, Ҳ���ǿͻ����к�(CSN)
*/


s32 OnDeviceAuth(ST_TRANSFLOW *pstTransFlow) {
    //u8 ucAuthKey[16] = {0};
    u8 ucTmpKey[16] = {0};
    u8 *pRecv = NULL, *pSend = NULL, deviceid[16], data[16], encryptbuf[16];
    u8 i = 0, recvCount = 0, sendCount = 0, authMode = 0;
    s32 ret = -1, deviceSNLen = 0;
    u16 len = 16, datalen = 0;


    pRecv = pstTransFlow->heReceiveData;
    pSend = pstTransFlow->heReceiveData;        //0416Send==Recv

    authMode = pRecv[recvCount];
    recvCount += 1;


    memset(deviceid, 0, sizeof(deviceid));
    sdkAscToBcd(deviceid, MposDeviceId, 20);
    deviceSNLen = 10;

    memset(encryptbuf, 0, sizeof(encryptbuf));

    memcpy(encryptbuf, deviceid + 4, 6);

    sdkBcdToU16(&datalen, &pRecv[recvCount], 2);
    recvCount += 2;
    datalen = datalen > 8 ? 8 : datalen;
    memcpy(&encryptbuf[6], &pRecv[recvCount], datalen);
    memcpy(&encryptbuf[14], "\x80\x00", 2);

    memset(data, 0, sizeof(data));
    memcpy(data, &pRecv[recvCount], datalen);
    for (i = 8; i < 16; i++)
        data[i] = ~data[i - 8];

    sdkU16ToBcd(pSend, len, 2);
    sendCount += sizeof(len);


    memcpy(ucTmpKey, data, 16);
    ret = sdkPEDTmkDes(AUTHKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, ucTmpKey, ucTmpKey);
    ret = sdkPEDTmkDes(AUTHKEYINDES, SDK_PED_DES_TRIPLE, SDK_PED_ENCRYPT, ucTmpKey + 8,
                       ucTmpKey + 8);
    TraceHex("zcl", "ucTmpKey", ucTmpKey, 16);


    sdkDes3S(1, encryptbuf, ucTmpKey);
    sdkDes3S(1, encryptbuf + 8, ucTmpKey);

    memcpy(&pSend[sendCount], encryptbuf, sizeof(encryptbuf));
    sendCount += sizeof(encryptbuf);

    len = deviceSNLen > 24 ? 24 : deviceSNLen;
    sdkU16ToBcd(&pSend[sendCount], len, 2);
    sendCount += 2;

    memcpy(&pSend[sendCount], deviceid, len);
    sendCount += len;

    pstTransFlow->usSendDataLen = sendCount;

    IsQmfMode = 1;
    return INS_SUC;
}


