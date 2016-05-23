#ifndef DEL_CODE_EXCESS                //ɾ���������: 20150304


//�������ݶ���
#define QUEUE_MAX_LEN      1024  //MIS�·���������ݳ���      

static SDK_QUEUE_HAND sqOrderData = NULL;


/*****************************************************************************
** Descriptions:	�ͷŶ���
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 QueueRelease(void) {
    s32 rslt = 0;
    if (sqOrderData != NULL) {
        rslt = sdkQueueRelease(sqOrderData);
        if (SDK_OK == rslt) {
            sqOrderData = NULL;
        }
        return rslt;
    }
    return SDK_OK;
}

/*****************************************************************************
** Descriptions:	������д�С
** Parameters:    	u32 uiSize
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
bool QueueGetMallocSize(u32 uiSize) {
    if (sqOrderData == NULL) {
        sqOrderData = sdkQueueCreate(uiSize);
        if (sqOrderData == NULL) {
            return false;
        }
        sdkQueueEmpey(sqOrderData);
    }
    return true;
}

/*****************************************************************************
** Descriptions:	���ض��о��
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
SDK_QUEUE_HAND GetQueueHand(void) {
    return sqOrderData;
}

/*****************************************************************************
** Descriptions:	���ݲ������    
** Parameters:    	s32 siMis
** Returned value:	
** Created By:		lilin2014.06.30
** Remarks: 		
*****************************************************************************/
s32 InsertDataToQueue(void) {
    u8 data[10] = {0};
    s32 datalen = 0;

    // u16 len = 0;

    if (!QueueGetMallocSize(QUEUE_MAX_LEN)) {
        Assert(0);
        return SDK_PARA_ERR;
    }
    memset(data, 0, sizeof(data));
    datalen = AppCommPollQuery(data);

    if (datalen > 0) {
//        TraceHex("commu", "jieshouda de shuju ", data, datalen);
        if ((datalen + sdkQueueGetDataLen(sqOrderData)) > (QUEUE_MAX_LEN)) {
            Verify(sdkQueueEmpey(sqOrderData) >= 0);
        }
        Verify(sdkQueueInsertData(sqOrderData, data, datalen) > 0); /*****2��ѹ���������*****/
    }
    return datalen;

}


/*****************************************************************************
** Descriptions:        ��������Э��
** Parameters:    	void
** Returned value:	
** Created By:		lilin2014.07.01
** Remarks: 		
*****************************************************************************/
s32 ParseOrderProtocol(void) {
    s32 len = 0;
    u8 buffer[1] = {0};

    SDK_QUEUE_HAND hand = GetQueueHand();

    if (hand == NULL) {
        return SDK_PARA_ERR;
    }
    len = sdkQueueGetDataLen(hand); //����յ������ݳ���
    memset(buffer, 0, sizeof(buffer));
    if (len) {
        while (sdkQueueIsEmpey(hand) != SDK_OK) {
            len = sdkQueueGetData(hand, buffer, 1);
            if (len) {
                TraceHex("lilin", "�յ�����\r\n", buffer, len);
                //�յ�����֮��������ݱ��ģ���ȷ����������Ϣִ�����̣�������
                // PostTransFlow(u8 *buf,u16 len);



                /*
                        //ִ����֮�����ָ��ִ�н������Ӧ����
                        if(buffer[0] == '1')
                        {��
                            AppCommSendData("hello world!",12,0);
                        }
                        */
                sdkQueueEmpey(hand);        //�������
            }
        }
        sdkQueueEmpey(hand);        //�������
    }
    return SDK_EQU;


}


#endif	//DEL_CODE_EXCESS	ɾ���������: 20150304


