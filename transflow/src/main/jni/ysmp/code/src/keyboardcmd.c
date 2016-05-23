s32 getKeyboardBufValue(ST_TRANSFLOW *);

s32 getKeyboardValue(ST_TRANSFLOW *);

s32 getKeyboardString(ST_TRANSFLOW *);

s32 storeKeyboardString(ST_TRANSFLOW *);

cmd_Str const KeyBoardCmdTab[] = {

        {BUFVALUE_ENUM,     getKeyboardBufValue, NULL},
        {VALUE_ENUM,        getKeyboardValue,    NULL},
        {STRING_ENUM,       getKeyboardString,   NULL},
        {STORTSTRING_ENUM,  storeKeyboardString, NULL},
        {KEYBOARD_END_ENUM, NULL,                NULL}
};


/*****************************************************************************
** Descriptions:	      ��ȡ���̻������м�ֵ
** Parameters:    	      ST_TRANSFLOW
** Returned value:	������  SDK_MSG_KEEP	RETURN: must be SDK_MSG_KEEP
** Created By:		DongChangQiang 2014.07.10
** Remarks: 		
*****************************************************************************/

s32 getKeyboardBufValue(ST_TRANSFLOW *pStTransFlow) {
    s32 keyValue = 0;

    keyValue = sdkKbGetKey();

    Trace("dcq", "get buf key value: %d\r\n", keyValue);

    pStTransFlow->heReceiveData[0] = keyValue;     //0416Send==Recv
    pStTransFlow->usSendDataLen = 1;

    return INS_SUC;
}

/*****************************************************************************
** Descriptions:	      ��ȡ���̼�ֵ
** Parameters:    	      ST_TRANSFLOW
** Returned value:	������  SDK_MSG_KEEP	RETURN: must be SDK_MSG_KEEP
** Created By:		DongChangQiang 2014.07.11
** Remarks: 		
*****************************************************************************/

s32 getKeyboardValue(ST_TRANSFLOW *pStTransFlow) {
    s32 keyValue = 0;
    u32 time = 0;

//	memcpy(&time, pStTransFlow->heReceiveData, sizeof(time));
    sdkHexToU32(&time, pStTransFlow->heReceiveData, 4);//sujianzhong 2015.01.13 23:46

    keyValue = sdkKbWaitKey(SDK_KEY_MASK_ALL, time);
    if (keyValue == SDK_TIME_OUT || keyValue == SDK_ERR)
        keyValue = 0;

    Trace("dcq", "get key value: %d, time: %d\r\n", keyValue, time);

    pStTransFlow->heReceiveData[0] = keyValue;         //0416Send==Recv
    pStTransFlow->usSendDataLen = 1;

    return INS_SUC;
}

/*****************************************************************************
** Descriptions:	      ��ȡ�����ַ���
** Parameters:    	      ST_TRANSFLOW
** Returned value:	
** Created By:		DongChangQiang 2014.07.11
** Remarks: 		
*****************************************************************************/
/*
	ID		������				����			��ע

	1		��ʱʱ��			  4				��λ(ms)

	2		���������ʽ   	  1				0x01: ��������ʽ
													0x02: �������ִ�
													0x03: �����ַ���
	3		�ַ���С����	  1

	4		�ַ���󳤶�	  1

	5		������ʾ��ʽ	  1				0x00: ����, 0x01: ����

	6		������ʾ����	LL...20

	7		������ʾ����	LL...512
	
*/

s32 getKeyboardString(ST_TRANSFLOW *pStTransFlow) {
    u8 *pRecv = NULL, *pSend = NULL, databuf[128];
    u8 len = 0, displayLineNum = 0, showMaxColumnNum = 0, showMaxCharacters = 0;
    u16 lenlen = 0;
    u32 count = 0;
    s32 ret = 0;
    SDK_DISP_PIXEL pix;

    typedef struct STRUCT_INPUTSTRING {

        u32 expireTime;
        u8 type;
        u8 min;
        u8 max;
        u8 showFormat;
        u8 showFirstLen;
        u8 *pShowFirstText;
        u8 showLen;
        u8 *pShowText;

    } struct_inputstring;

    struct_inputstring inputString;
    struct_inputstring *pInputStr = &inputString;

    pRecv = pStTransFlow->heReceiveData;
    pSend = pStTransFlow->heReceiveData;           //0416Send==Recv

    TraceHex("dcq", "receive: ", pRecv, pStTransFlow->ReceiveDataLen);

    pix = sdkDispGetScreenPixel();
    // by default, I use E_WORD_ASCII_16_32 as the character size;
    showMaxCharacters = pix.siX / 16;
    showMaxColumnNum = pix.siY / 32;
    if (showMaxColumnNum > sizeof(databuf))
        showMaxColumnNum = sizeof(databuf);

    memcpy(&pInputStr->expireTime, pRecv, 4);
    count += 4;
    // ������Ϊ, ���û�����ĵȴ�ʱ��Ӧ����5 s
    if (pInputStr->expireTime < 5000)
        pInputStr->expireTime = 5000;

    pInputStr->type = pRecv[count];
    count += 1;

    pInputStr->min = pRecv[count];
    count += 1;
    if (pInputStr->min > 127)
        pInputStr->min = 127;

    pInputStr->max = pRecv[count];
    count += 1;
    if (pInputStr->max > 127)
        pInputStr->max = 127;

    switch (pInputStr->type) {

        case 0x01:
            pInputStr->type = SDK_MMI_POINT;
            break;

        case 0x02:
            pInputStr->type = SDK_MMI_NUMBER;
            break;

        case 0x03:
            pInputStr->type = SDK_MMI_NUMBER | SDK_MMI_LETTER;
            break;

        default:
            pInputStr->type = SDK_MMI_NUMBER;
            break;
    }

    pInputStr->showFormat = pRecv[count];
    count += 1;

    sdkBcdToU16(&pInputStr->showFirstLen, &pRecv[count], 2);
    count += 2;

/*
	��������ʾ��Ϣ���������ʾֵʱ, ���²��ֽض�.
*/

    pInputStr->pShowFirstText = &pRecv[count];
    count += pInputStr->showFirstLen;
    if (pInputStr->showFirstLen > showMaxCharacters)
        len = showMaxCharacters;
    else
        len = pInputStr->showFirstLen;
    memcpy(databuf, pInputStr->pShowFirstText, len);
    sdkDispFillRowRam(displayLineNum, 0, pInputStr->pShowFirstText,
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    displayLineNum++;


/*
	������ӵ��߼�������ʾ�������������ֻ��˴���������.
*/

    sdkBcdToU16(&pInputStr->showLen, &pRecv[count], 2);
    count += 2;

    pInputStr->pShowText = &pRecv[count];
    while (displayLineNum < showMaxColumnNum - 1) {
        if (pInputStr->showLen > showMaxCharacters)
            len = showMaxCharacters - 1;
        else
            len = pInputStr->showLen;

        memcpy(databuf, &pInputStr->pShowText[showMaxCharacters * (displayLineNum - 1)], len);
        databuf[len] = '\0';
        Trace("dcq", "databuf: %s\n", databuf);

        sdkDispFillRowRam(displayLineNum, 0, databuf, SDK_DISP_FDISP | SDK_DISP_INCOL);
        displayLineNum++;
        if (len != showMaxCharacters)
            break;
        pInputStr->showLen -= showMaxCharacters;
    }
    sdkDispBrushScreen();

    databuf[0] = 0;
    ret = sdkKbGetScanf(pInputStr->expireTime, databuf, pInputStr->min, pInputStr->max, \
                            pInputStr->type, 3);
    Trace("dcq", "ret: %d", ret);

    switch (ret) {
        case SDK_TIME_OUT:
            ret = INS_OVERTIME;
            break;

        case SDK_PARA_ERR:
        case SDK_ERR:
            ret = INS_OTHER_ERR;
            break;

        case SDK_KEY_ENTER:
            pSend[0] = 0;
            ret = INS_SUC;
            break;

        case SDK_KEY_ESC:
            pSend[0] = 6;
            ret = INS_SUC;
            break;

        default:
            ret = INS_SUC;
            break;
    }

    pStTransFlow->usSendDataLen += 1;
    if (ret == INS_SUC) {
        lenlen = databuf[0];
        sdkU16ToBcd(&pSend[1], lenlen, sizeof(lenlen));
        pStTransFlow->usSendDataLen += 2;

        memcpy(&pSend[3], &databuf[1], lenlen);
        pStTransFlow->usSendDataLen += lenlen;
    }

    TraceHex("dcq", "keyboard string: ", pSend, pStTransFlow->usSendDataLen);

    return ret;
}

/*
	����Ĵ���readTag(), writeTag()  ���ڲ��Ա���tag  ����.
*/

s32 saveTag(u8 *, u16, u8 *);

s32 writeTag() {
    u8 tagbuf[4], databuf[128];
    u16 taglen = 4;

    memcpy(tagbuf, "123", 4);

    memset(databuf, 0, sizeof(databuf));
    memcpy(databuf, "slfjlsjfsf", strlen("slfjlsjfsf"));

    saveTag(tagbuf, taglen, databuf);
}

s32 readTag() {
    u8 tag[4];//, data[128];
    s32 readLen = 0, ret = 0;

    ret = sdkAccessFile(STORE_TAG_FILENAME);
    if (ret == false) {
    }
    else {
        readLen = 4;
        memset(tag, 0, sizeof(tag));
        sdkReadFile(STORE_TAG_FILENAME, tag, 0, &readLen);

        TraceHex("dcq", "read file\n", tag, 4);

    }
    return INS_SUC;
}

s32 saveTag(u8 *tagbuf, u16 taglen, u8 *databuf) {
    u8 count = 0;
    u8 tag[4], tagDst[4], data[128];
    s32 fileLen = 0, readLen = 0, ret = 0;

    memset(tag, 0, sizeof(tag));
    memcpy(tag, tagbuf, taglen);

    memset(data, 0, sizeof(data));
    memcpy(data, databuf, databuf[0] + 1);

    ret = sdkAccessFile(STORE_TAG_FILENAME);
    if (ret == false) {
        sdkWriteFile(STORE_TAG_FILENAME, tag, sizeof(tag));
        sdkInsertFile(STORE_TAG_FILENAME, data, sizeof(tag), sizeof(data));
    }
    else {
        fileLen = sdkGetFileSize(STORE_TAG_FILENAME);
        count = fileLen / (128 + 4);
        u8 i = 0;
        for (i = 0; i < count; i++) {
            readLen = 4;
            sdkReadFile(STORE_TAG_FILENAME, tagDst, i * (128 + 4), &readLen);
            if (!memcmp(tagDst, tag, sizeof(tag))) {
                sdkInsertFile(STORE_TAG_FILENAME, data, i * (128 + 4) + 4, sizeof(data));
            }
        }
    }

    return INS_SUC;
}

/*****************************************************************************
** Descriptions:	     ��ȡ�����ַ��������豸�ڱ���
** Parameters:    	      ST_TRANSFLOW
** Returned value:	
** Created By:		DongChangQiang 2014.07.12
** Remarks: 		
*****************************************************************************/

/*
	ID		������				����			��ע

	1		��ʱʱ��			  4				��λ(ms)

	2		���������ʽ   	  1				0x01: ��������ʽ
													0x02: �������ִ�
													0x03: �����ַ���
	3		�ַ���С����	  1

	4		�ַ���󳤶�	  1

	5		������ʾ��ʽ	  1				0x00: ����, 0x01: ����

	6		������ʾ����	LL...20

	7		������ʾ����	LL...512

	8		�豸�ڲ�����	LL...3			ָ����Tag Ϊ��Ҫ��POS �ڱ���ı�ǩ
			�ı�ǩֵ( TLV 						����,POS ��Ӧ��Ϊ�ý��������ݱ�����
			�е�Tag )								�ñ�ǩ �Ϲ������������ڲ�ʹ��.
*/


s32 storeKeyboardString(ST_TRANSFLOW *pStTransFlow) {
    u8 *pRecv = NULL, *pSend = NULL, databuf[128];
    u8 len = 0, displayLineNum = 0, showMaxColumnNum = 0, showMaxCharacters = 0;
    u16 lenlen = 0;
    u32 count = 0;
    s32 ret = 0;
    SDK_DISP_PIXEL pix;

    typedef struct STRUCT_INPUTSTRING {

        u32 expireTime;
        u8 type;
        u8 min;
        u8 max;
        u8 showFormat;
        u8 showFirstLen;
        u8 *pShowFirstText;
        u8 showLen;
        u8 *pShowText;
        u8 tagLen;
        u8 *pTag;

    } struct_inputstring;

    struct_inputstring inputString;
    struct_inputstring *pInputStr = &inputString;

    pRecv = pStTransFlow->heReceiveData;
    pSend = pStTransFlow->heReceiveData;       //0416Send==Recv

    TraceHex("dcq", "receive: ", pRecv, pStTransFlow->ReceiveDataLen);

    pix = sdkDispGetScreenPixel();
    // by default, I use E_WORD_ASCII_16_32 as the character size;
    showMaxCharacters = pix.siX / 16;
    showMaxColumnNum = pix.siY / 32;
    if (showMaxColumnNum > sizeof(databuf))
        showMaxColumnNum = sizeof(databuf);

    memcpy(&pInputStr->expireTime, pRecv, 4);
    count += 4;
    // ������Ϊ, ���û�����ĵȴ�ʱ��Ӧ����5 s
    if (pInputStr->expireTime < 5000)
        pInputStr->expireTime = 5000;

    pInputStr->type = pRecv[count];
    count += 1;

    pInputStr->min = pRecv[count];
    count += 1;
    if (pInputStr->min > 127)
        pInputStr->min = 127;

    pInputStr->max = pRecv[count];
    count += 1;
    if (pInputStr->max > 127)
        pInputStr->max = 127;

    switch (pInputStr->type) {

        case 0x01:
            pInputStr->type = SDK_MMI_POINT;
            break;

        case 0x02:
            pInputStr->type = SDK_MMI_NUMBER;
            break;

        case 0x03:
            pInputStr->type = SDK_MMI_NUMBER | SDK_MMI_LETTER;
            break;

        default:
            pInputStr->type = SDK_MMI_NUMBER;
            break;
    }

    pInputStr->showFormat = pRecv[count];
    count += 1;

    sdkBcdToU16(&pInputStr->showFirstLen, &pRecv[count], 2);
    count += 2;

/*
	��������ʾ��Ϣ���������ʾֵʱ, ���²��ֽض�.
*/

    pInputStr->pShowFirstText = &pRecv[count];
    count += pInputStr->showFirstLen;
    if (pInputStr->showFirstLen > showMaxCharacters)
        len = showMaxCharacters;
    else
        len = pInputStr->showFirstLen;
    memcpy(databuf, pInputStr->pShowFirstText, len);
    sdkDispFillRowRam(displayLineNum, 0, pInputStr->pShowFirstText,
                      SDK_DISP_FDISP | SDK_DISP_CDISP | SDK_DISP_INCOL);
    displayLineNum++;


/*
	������ӵ��߼�������ʾ�������������ֻ��˴���������.
*/

    sdkBcdToU16(&pInputStr->showLen, &pRecv[count], 2);
    count += 2;

    pInputStr->pShowText = &pRecv[count];
    count += pInputStr->showLen;
    while (displayLineNum < showMaxColumnNum - 1) {
        if (pInputStr->showLen > showMaxCharacters)
            len = showMaxCharacters - 1;
        else
            len = pInputStr->showLen;

        memcpy(databuf, &pInputStr->pShowText[showMaxCharacters * (displayLineNum - 1)], len);
        databuf[len] = '\0';
        Trace("dcq", "databuf: %s\n", databuf);

        sdkDispFillRowRam(displayLineNum, 0, databuf, SDK_DISP_FDISP | SDK_DISP_INCOL);
        displayLineNum++;
        if (len != showMaxCharacters)
            break;
        pInputStr->showLen -= showMaxCharacters;
    }
    sdkDispBrushScreen();

    databuf[0] = 0;
    ret = sdkKbGetScanf(pInputStr->expireTime, databuf, pInputStr->min, pInputStr->max, \
                            pInputStr->type, 3);
    Trace("dcq", "ret: %d", ret);

    sdkBcdToU16(&pInputStr->tagLen, &pRecv[count], 2);
    count += 2;

    if (pInputStr->tagLen) {
        pInputStr->pTag = &pRecv[count];
        saveTag(pInputStr->pTag, pInputStr->tagLen, databuf);
    }

    switch (ret) {
        case SDK_TIME_OUT:
            ret = INS_OVERTIME;
            break;

        case SDK_PARA_ERR:
        case SDK_ERR:
            ret = INS_OTHER_ERR;
            break;

        case SDK_KEY_ENTER:
            pSend[0] = 0;
            ret = INS_SUC;
            break;

        case SDK_KEY_ESC:
            pSend[0] = 6;
            ret = INS_SUC;
            break;

        default:
            ret = INS_SUC;
            break;
    }


    pStTransFlow->usSendDataLen += 1;
    if (ret == INS_SUC) {
        lenlen = databuf[0];
        sdkU16ToBcd(&pSend[1], lenlen, sizeof(lenlen));
        pStTransFlow->usSendDataLen += 2;

        memcpy(&pSend[3], &databuf[1], lenlen);
        pStTransFlow->usSendDataLen += lenlen;
    }

    TraceHex("dcq", "keyboard string: ", pSend, pStTransFlow->usSendDataLen);

    return ret;
}





