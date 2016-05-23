#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304

/*****************************************************************************
** Descriptions:	����ϵͳ����
** Parameters:          void
** Returned value:
** Created By:		lilin2012.08.01
** Remarks:
*****************************************************************************/
void SaveAppPosCfg(void) {
    u8 temp[256], fn[256];
    s32 i = 0;

    Trace("lilin", "save.........\r\n");

    if (pgstSys == NULL) {
        Assert(0);
        return;
    }
    memset(fn, 0, sizeof(fn));
//    strcpy(fn, gasCurAppDir);
    strcpy(fn, "appposcfg");

    memset(temp, 0, sizeof(temp));
    //��ˮ��
    i = atoi(pgstSys->asVoucherNo);

    if ((i < 1) || (i > 999999)) {
        i = 1;
    }
    sprintf(pgstSys->asVoucherNo, "%06d", i);
    strcat(temp, pgstSys->asVoucherNo);

    //���κ�
    i = atoi(pgstSys->asBatchNo);

    if ((i < 1) || (i > 999999)) {
        i = 1;
    }
    sprintf(pgstSys->asBatchNo, "%06d", i);
    strcat(temp, pgstSys->asBatchNo);

    if (sdkSaveOverlap(fn, temp, 12) != SDK_FILE_OK) {
        Assert(0);
    }
}

/*****************************************************************************
** Descriptions:	����ϵͳ����
** Parameters:          void
** Returned value:
** Created By:		lilin2012.08.01
** Remarks:
*****************************************************************************/
void ReadAppPosCfg(void) {
    u8 fn[256], temp[256] = {0};
    s32 rslt = SDK_FILE_ERROR;
    s32 i = 0;
    bool saveflag = false;


    if (NULL == pgstSys) {
        Assert(0);
        return;
    }
    memset(fn, 0, sizeof(fn));
//    strcpy(fn, gasCurAppDir);
    strcat(fn, "appposcfg");

    rslt = sdkReadOverlap(fn, temp, 12);
    Trace("lilin", "rslt = %d\r\n", rslt);

    switch (rslt) {
        case SDK_FILE_OK:
            //����ˮ��
            memset(pgstSys->asVoucherNo, 0, sizeof(pgstSys->asVoucherNo));
            memcpy(pgstSys->asVoucherNo, temp, 6);
            i = atoi(pgstSys->asVoucherNo);

            if ((i < 1) || (i > 999999)) {
                i = 1;
                saveflag = true;
            }
            sprintf(pgstSys->asVoucherNo, "%06d", i);

            //������ˮ��
            memset(pgstSys->asBatchNo, 0, sizeof(pgstSys->asBatchNo));
            memcpy(pgstSys->asBatchNo, &temp[6], 6);
            i = atoi(pgstSys->asBatchNo);

            if ((i < 1) || (i > 999999)) {
                i = 1;
                saveflag = true;
            }
            sprintf(pgstSys->asBatchNo, "%06d", i);

            Trace("ReadAppPosCfg", "vouch=%s  batch=%s\r\n", pgstSys->asVoucherNo,
                  pgstSys->asBatchNo);

            pgstSys->ucYear = 0x00;
            pgstSys->ucMonth = 0x01;
            pgstSys->ucDay = 0x01;
            pgstSys->ucHour = 0x00;
            pgstSys->ucMinute = 0x00;
            pgstSys->ucSecond = 0x00;
            GetRtcSys();

            if (saveflag) {
                SaveAppPosCfg();
            }
            break;

        case SDK_FILE_EOF:
        default:
            Trace("ReadAppPosCfg", "no exist\r\n");
            pgstSys->ucYear = 0x00;
            pgstSys->ucMonth = 0x01;
            pgstSys->ucDay = 0x01;
            pgstSys->ucHour = 0x00;
            pgstSys->ucMinute = 0x00;
            pgstSys->ucSecond = 0x00;
            GetRtcSys();
            strcpy(pgstSys->asVoucherNo, "000001");         //����ˮ��
            strcpy(pgstSys->asBatchNo, "000001");           //������ˮ��
            SaveAppPosCfg();
            break;
    }
}

#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304

void ReadMposDeviceId() {
    u8 deviceid[16] = {0};
    s32 ret;
    s32 filelen = 10;

    memset(MposDeviceId, '0', sizeof(MposDeviceId));
    ret = sdkReadFile(IDFILE, deviceid, 0, &filelen);

    if (ret == SDK_FILE_OK) {
        sdkBcdToAsc(MposDeviceId, deviceid, 10);
    }
}

void WriteMposKSN() {
    u8 ksn[24] = {0};

    memcpy(ksn, MposPinKsn, 8);
    memcpy(ksn + 8, MposTrackKsn, 8);
    memcpy(ksn + 16, MposPbocKsn, 8);

    if (memcmp(MposPinKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) >= 0) {
        memcpy(MposPinKsn, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposTrackKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        memcpy(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposPbocKsn, "\x30\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        memcpy(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x01", 8);
    }
    //sdkWriteFile(KSNFILE, ksn, 24);
    sdkSaveOverlap(KSNFILE, ksn, 24);         //20150320 �滻�����ļ��ӿ�
}

void ReadMposKSN() {
    u8 ksn[24] = {0};
    s32 ret;
    u8 flg = 0;
    s32 filelen = 24;

    memcpy(MposPinKsn, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    memcpy(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x01", 8);
    memcpy(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x01", 8);

    ret = sdkReadFile(KSNFILE, ksn, 0, &filelen);

    if (ret == SDK_FILE_OK) {
        memcpy(MposPinKsn, ksn, 8);
        memcpy(MposTrackKsn, ksn + 8, 8);
        memcpy(MposPbocKsn, ksn + 16, 8);
    }

    if (memcmp(MposPinKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) >= 0) {
        flg = 1;
        memcpy(MposPinKsn, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposTrackKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        flg = 1;
        memcpy(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposPbocKsn, "\x30\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        flg = 1;
        memcpy(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (flg) {
        WriteMposKSN();
    }
}

void AddMposKSN(u8 tpye) {
    if (tpye & ADDPINKSN) {
        sdkBcdAdd(MposPinKsn, MposPinKsn, 8, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (tpye & ADDTRACEKSN) {
        sdkBcdAdd(MposTrackKsn, MposTrackKsn, 8, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (tpye & ADDPBOCKSN) {
        sdkBcdAdd(MposPbocKsn, MposPbocKsn, 8, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposPinKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) >= 0) {
        memcpy(MposPinKsn, "\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposTrackKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        memcpy(MposTrackKsn, "\x10\x00\x00\x00\x00\x00\x00\x01", 8);
    }

    if (memcmp(MposPbocKsn, "\x30\x00\x00\x00\x00\x00\x00\x00", 8) >= 0 ||
        memcmp(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x00", 8) <= 0) {
        memcpy(MposPbocKsn, "\x20\x00\x00\x00\x00\x00\x00\x01", 8);
    }
    WriteMposKSN();
}

