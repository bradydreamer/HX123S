#include "sdkGlobal.h" //sujianzhong 2013.07.22 14:28

#ifndef  DEBUG_EMV
#define TraceHex(pasTag, pasInfo, pheDate, uiLen)
#define Trace(ptag, ...)
//#define Assert(exp)
//#define Verify(exp)    ((void)(exp))
#endif


/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_FIXED_AID stFixedAid:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_FixedAidAbsentErr
                                SDK_OK
                                SDK_IccReturnErr
                                SDK_CancelTransErr
                                SDK_ERR
                                SDK_PARA_ERR
                                SDK_AppSelectTryAgainErr
                                SDK_IccCommandErr
                                SDK_UsePbocErr
                                SDK_AppBlockErr
                                SDK_NoAppSelErr
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl20140408   //Ҫ���·�װ��Щ�������߼����̵Ļ��������̵�Ҫ�����ͷŴ󲿷���Դ
*****************************************************************************/
s32 sdkIccGoToSelectApp(SDK_ICC_FIXED_AID stFixedAid, const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                        const SDK_ICC_PARAM *pstIccParam)   //sjz 20120726
{
    s32 rlt;

    rlt = sdkIccOrganizeAppList(stFixedAid, pstIccTradeParam,
                                pstIccParam);                                                 //��֯Ӧ��ѡ���б�

    if (SDK_FixedAidAbsentErr ==
        rlt)                                               //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
    {
        return SDK_FixedAidAbsentErr;
    }
    else if (rlt == SDK_UserSelect) {
        rlt = sdkIccUserSelectApp(pstIccTradeParam,
                                  pstIccParam);                                               //�û�ѡ��Ӧ��
    }

    if (rlt != SDK_OK) {
        return rlt;
    }
    return sdkIccFinalSelectedApp(pstIccTradeParam, pstIccParam);
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_NeedMagCardErr: FALLBACK
                                SDK_AppSelectTryAgainErr
                                SDK_OK: EMV/PBOC normal transflow
                                SDK_IccCommandErr
                                SDK_IccDataFormatErr
                                SDK_IccReturnErr
                                SDK_ERR
                                SDK_ReadECBalance
                                SDK_OnlineWait:
                                SDK_OfflineApprove:
                                SDK_OfflineDecline:
                                SDK_UsePbocErr
                                SDK_PARA_ERR
                                SDK_ECNotSupportErr
                                SDK_EcLackBalanceErr
                                SDK_OfflineDecline
                                SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr

** Created By:	     sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccGoToInitialApp(SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                         SDK_ICC_PARAM *pstIccParam)  //sjz 20120726
{
    s32 ret;
    u8 bcAmount[6] = {0};
    TraceHex("emv", "asamount:", pstIccTradeParam->pasTradeAmount, 12);

    if ((NULL != pstIccTradeParam->InputAmount) && (NULL != pstIccTradeParam->pasTradeAmount)
        && (strlen(pstIccTradeParam->pasTradeAmount) == 0)) //sujianzhong 2013.01.10 14:24
    {
        ret = (*pstIccTradeParam->InputAmount)(
                pstIccTradeParam->pasTradeAmount); //sujianzhong 2013.01.10 14:24
        Trace("emv", "pstIccTradeParam->InputAmount  return value = %d\r\n", ret);

        if (strlen(pstIccTradeParam->pasTradeAmount) <= 12) {
            Verify(sdkAscToBcdR(bcAmount, pstIccTradeParam->pasTradeAmount, 6) >= 0);
        }
        else {
            TraceHex("emv", "���׽���42949672.95Ԫ(�޷�������)", pstIccTradeParam->pasTradeAmount,
                     strlen(pstIccTradeParam->pasTradeAmount));
            return SDK_ERR;
        }

        if (ret != SDK_OK) {
            return ret;
        }
        else if (memcmp(bcAmount, "\x00\x42\x94\x96\x72\x95", 6) > 0) {
            TraceHex("emv", "���׽���42949672.95Ԫ(�޷�������)", pstIccTradeParam->pasTradeAmount,
                     strlen(pstIccTradeParam->pasTradeAmount));
            return SDK_ERR;
        }
    }
    ret = sdkIccInitialApp(pstIccTradeParam, pstIccParam);            //��ʼ��Ӧ��
    Trace("emv", "sdkIccInitialApp  return value = %d\r\n", ret);

    if (ret ==
        SDK_NeedMagCardErr)                                       //GPO����"94 81"����fallback����
    {
        if (pstIccTradeParam->bIsFallback) {
            return SDK_NeedMagCardErr;
        }
        else {
            return SDK_ERR;
        }
    }
    else if ((SDK_ReadECBalance == ret) ||
             (SDK_OfflineDecline == ret)) // �������ֽ�ARQC��AAC,ȥ�е����ֽ����
    {
        ret = sdkIccReadEcData(pstIccTradeParam,
                               pstIccParam);                                                   //�������ֽ����
        Trace("emv", "sdkIccReadEcData return value = %d\r\n", ret);

        if (SDK_OfflineDecline == ret) {
            return sdkIccTransComplete();                                                   //�������
        }
    }
    else if (SDK_OnlineWait == ret || SDK_OfflineApprove == ret) //qpboc����
    {
//        if((SDK_OK == sdkIccGetPAN(asPAN))//�Ѿ���ȡ������
//            && (NULL != pstIccTradeParam->VerifyCardNo))//sujianzhong 2013.07.18 17:29 qpboc�ȽϺ�����
//        {
//            s32 retcode;
//
//            retcode = (*pstIccTradeParam->VerifyCardNo)(asPAN);                                   // ȷ�Ͽ���
//            Trace("emv", "pstIccTradeParam->VerifyCardNo return value = %d\r\n", retcode);
//
//            if(retcode != SDK_OK)
//            {
//                return retcode;
//            }
//        }
    }
    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM *pstIccTradeParam:
                                const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_ERR
                                SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr
                                SDK_CancelTransErr
                                SDK_IccCommandErr
                                SDK_IccReturnErr
                                SDK_AppTimeOutErr
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccDealQpbocOnline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                          const SDK_ICC_PARAM *pstIccParam)      //sjz 20120726
{
    s32 ret;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam)) {
        return SDK_PARA_ERR;
    }
    Trace("emv", " sdkIccDealQpbocOnline\r\n");

    if (NULL != pstIccTradeParam->DispCardOut) {
        ret = (*pstIccTradeParam->DispCardOut)();                   // ��ʾ�ɽ���Ƭ�ƿ�

        if (ret != SDK_OK) {
            return ret;
        }
    }
    return sdkIccCardHolderVerf(pstIccTradeParam, pstIccParam);
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM *pstIccTradeParam:
                                const SDK_ICC_PARAM *pstIccParam:
** Returned value:	SDK_PARA_ERR
                                SDK_ERR
                                SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr
                                SDK_IccCommandErr
                                SDK_AppTimeOutErr
                                SDK_IccDataRedundErr
                                SDK_IccDataFormatErr
                                SDK_IccReturnErr
** Created By:		sujianzhong2012.08.09
** Remarks:              //sxl20140408
*****************************************************************************/
s32 sdkIccDealQpbocOffline(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                           const SDK_ICC_PARAM *pstIccParam)      //sjz 20120726
{
    s32 ret;

    if ((NULL == pstIccTradeParam) || (NULL == pstIccParam)) {
        return SDK_PARA_ERR;
    }
    Trace("emv", " qpboc_offline_trade\r\n");
    ret = sdkIccReadAppData(pstIccTradeParam, pstIccParam);
    Trace("emv", "sdkIccReadAppData return value = %d\r\n", ret);

    if (SDK_QpbocReadLastLogErr == ret) {
        return SDK_TransOfflineDeclineErr;
    }
    else if ((SDK_OfflineApprove != ret) && (SDK_OnlineWait != ret)) {
        if (ret == SDK_OK) {
            ret = SDK_ERR;
        }
        return ret;
    }

    if (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K370P) {
        sdkSysBeep(SDK_SYS_BEEP_OK);
    }
    else {
        sdkSysPlaySoundFile("emvbeepok.wav", sdkSysGetVolume()); //����˵�ͼ�������,��������ļ�ʱ�䳤һЩ
    }

    if (NULL != pstIccTradeParam->DispCardOut) {
        ret = (*pstIccTradeParam->DispCardOut)();                   // ��ʾ�ɽ���Ƭ�ƿ�

        if (ret != SDK_OK) {
            return ret;
        }
    }

    ret = sdkIccCardHolderVerf(pstIccTradeParam, pstIccParam);
    Trace("emv", "sdkIccCardHolderVerf return value = %d\r\n", ret);

    if (SDK_OfflineApprove != ret) {
        return ret;
    }
    ret = sdkIccProcessRestrict(pstIccTradeParam, pstIccParam);
    Trace("emv", "sdkIccProcessRestrict return value = %d\r\n", ret);

    if (SDK_AppEffectErr == ret || SDK_AppExpiredErr == ret) {
        return SDK_TransOfflineDeclineErr;
    }
    else if (SDK_OfflineApprove != ret) {
        return ret;
    }
    ret = sdkIccDataAuth(pstIccTradeParam, pstIccParam);
    Trace("emv", "sdkIccDataAuth return value = %d\r\n", ret);

    if (SDK_OfflineApprove == ret) {
        if (sdkSysGetMachineCode(NULL) == SDK_SYS_MACHINE_K370P) {
            sdkSysPlaySoundFile("emvbeepok.wav",
                                sdkSysGetVolume());           //����˵�ͼ�������,��������ļ�ʱ�䳤һЩ
        }
        return SDK_TransOfflineApprove;         //qpboc�ѻ���׼�ɹ�
    }
    else if (SDK_OfflineDecline == ret) {
        return SDK_TransOfflineDeclineErr; //qpboc�ѻ��ܾ�
    }
        //==================qpboc�ѻ�������֤ʧ�ܣ�ת��������============
    else {
        return ret; //qpboc�ѻ�������֤ʧ�ܣ�ת��������
    }
}

/*****************************************************************************
** Descriptions:
** Parameters:          const SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_OK
                                SDK_ERR
                                SDK_IccCommandErr
                                SDK_AppTimeOutErr
                                SDK_IccDataRedundErr
                                SDK_IccDataFormatErr
                                SDK_IccReturnErr
                                SDK_PARA_ERR
                                SDK_CancelTransErr
                                SDK_TransOnlineApprove
                                SDK_TransOnlineDeclineErr
                                SDK_TransOfflineApprove
                                SDK_TransOfflineDeclineErr
                                SDK_TransNotAcceptErr
                                SDK_TransTerminateErr
                                SDK_CDAFormatErr:
                                SDK_IccEasyFlow
                                SDK_NeedMagCardErr
                                SDK_ECNotSupportErr
                                SDK_EcLackBalanceErr
** Created By:		sujianzhong2012.08.09
** Remarks:               //sxl20140408
*****************************************************************************/
s32 sdkIccDealEmvFlowProcess(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                             const SDK_ICC_PARAM *pstIccParam)      //sjz 20120726
{
    u8 onlineRandNum;
//    u32 len=0;
    s32 ret;
    SDK_ICC_CARDDATA *pstcarddata = NULL;


    Trace("emv", "\r\n Is  EMV or PBOC  trade\r\n");
    //==============����ΪEMV/PBOC����=========================
    ret = sdkIccReadAppData(pstIccTradeParam,
                            pstIccParam);                                           //��Ӧ��
    Trace("emv", "sdkIccReadAppData return value = %d\r\n", ret);

    if (SDK_ReadECBalance == ret) //  �������ֽ�û�з�����Ȩ��,����,��Ҫȥ�����
    {
        ret = sdkIccReadEcData(pstIccTradeParam,
                               pstIccParam);                                           //�������ֽ����
        Trace("emv", "sdkIccReadEcData return value = %d\r\n", ret);

        if (SDK_OfflineDecline == ret) {
            return sdkIccTransComplete();                                           //�������
        }
        else if (SDK_OK == ret) //sujianzhong 2013.02.24 12:50
        {
            return SDK_ECNotSupportErr;
        }
        else {
            return ret;
        }
    }
    else if (SDK_OK != ret) {
        return ret;
    }

    if (NULL != pstIccTradeParam->VerifyCardNo)     //sjz �ȽϺ�����
    {
        pstcarddata = (SDK_ICC_CARDDATA *) emvb_malloc(sizeof(SDK_ICC_CARDDATA));
        memset(pstcarddata, 0, sizeof(SDK_ICC_CARDDATA));

        if (SDK_OK != sdkIccGetCardData(pstcarddata)) {
            memset(pstcarddata, 0, sizeof(SDK_ICC_CARDDATA));
        }
        TraceHex("emv", "����", pstcarddata->asCardNO,
                 pstcarddata->ucCardLen); //sujianzhong 2013.01.10 14:36
        ret = (*pstIccTradeParam->VerifyCardNo)(
                pstcarddata->asCardNO);                                       // ȷ�Ͽ���
        Trace("emv", "pstIccTradeParam->VerifyCardNo return value = %d\r\n", ret);
        emvb_free(pstcarddata);

        if (ret != SDK_OK) {
            return ret;
        }
    }

    if (pstIccTradeParam->eFlowMode ==
        SDK_ICC_EASY_FLOW)                                           //������
    {
        return SDK_IccEasyFlow;
    }
//    if(pstIccTradeParam->eFlowMode == SDK_ICC_ALL_FLOW &&
//       memcmp(pstIccTradeParam->bcTradeAmount, "\x00\x00\x00\x00\x00\x00", 6) == 0)
//    { //�ж��Ƿ�������
//        sdkIccDispNoAmount();
//        return SDK_ERR;
//    }
#ifdef SDK_MEM_DEBUG
    sdkGetMemStatus();
#endif
    ret = sdkIccDataAuth(pstIccTradeParam,
                         pstIccParam);                                            //��̬������֤

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccProcessRestrict(pstIccTradeParam,
                                pstIccParam);                                            //��������

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccCardHolderVerf(pstIccTradeParam,
                               pstIccParam);                                            //�ֿ�����֤

    if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTermRiskManage(pstIccTradeParam,
                               pstIccParam);                                            //�ն˷��չ���

    if (SDK_ShowRandNum == ret) {
        sdkIccGetOnlineRandNum(&onlineRandNum);

        if (NULL != pstIccTradeParam->DispOnlineRandNum) {
            ret = (*pstIccTradeParam->DispOnlineRandNum)(onlineRandNum);             // ��ʾ�����

            if (ret != SDK_OK) {
                return ret;
            }
        }
    }
    else if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTermiAnalys(pstIccTradeParam,
                            pstIccParam);                                           //�ն���Ϊ����

    if (SDK_NeedMagCardErr == ret) {
        if (pstIccTradeParam->bIsFallback) {
            return SDK_NeedMagCardErr;
        }
        else {
            return SDK_ERR;
        }
    }
    else if (ret != SDK_OK) {
        return ret;
    }
    ret = sdkIccTransAnalys(pstIccTradeParam, pstIccParam); //���׷���

    if (ret != SDK_OK) //��������SDK_OnlineWait
    {
        return ret;
    }
#ifdef SDK_MEM_DEBUG
    sdkGetMemStatus();
#endif
    return sdkIccTransComplete();                                               //�������
}

s32 sdkIccDealEmvFlow(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                      const SDK_ICC_PARAM *pstIccParam) {
    s32 ret;

    ret = sdkIccDealEmvFlowProcess(pstIccTradeParam, pstIccParam);
    return ret;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam
                                const SDK_ICC_PARAM * pstIccParam
** Returned value:	SDK_OK:��������
                                SDK_ERR ����ʧ��
                                SDK_IccDataFormatErr ���ݸ�ʽ��
                                SDK_IccCommandErr ����ʧ��
                                SDK_IccReturnErr ������ʧ��
                                SDK_NoAppSelErr û��Ӧ��
                                SDK_IccDataRedundErr:�������ݴ�
                                SDK_CancelTransErr ȡ������
                                SDK_CardBlockErr ��������
                                SDK_AppTimeOutErr:��ʱ
                                SDK_NeedMagCardErr ʹ��fallback����
                                SDK_DispLog:��ʾ������ϸ
                                SDK_CDAFormatErr :CDA��ʽ��
                                SDK_OnlineWait:�����ȴ�(�ȴ�����)
                                SDK_TransOfflineApprove �ѻ���׼(�ѻ��ɹ�)
                                SDK_TransOfflineDeclineErr �ѻ��ܾ�(��������)
                                SDK_TransNotAcceptErr:���ײ�����
                                SDK_TransTerminateErr ��ֹ����
                                SDK_IccEasyFlow:ֻ��ȡ���ʺţ��߼�����
                                SDK_UsePbocErr:��ʹ��PBOC
                                SDK_PARA_ERR:��������
                                SDK_ICC_RESET_ERR:��λʧ��
                                SDK_EcLackBalanceErr:�����ֽ�����
                                SDK_ECNotSupportErr:  �������ֽ𿨲�֧�ָù���
                                SDK_QpbocAuthNoPerformed:������δ����˫����֤

** Created By:		sujianzhong2012.07.30
** Remarks:
*****************************************************************************/
s32 sdkIccTransFlow1Process(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam) {
    //u8 OutBuf[300];
    u8 OutBuf[66] = {0};
    s32 OutLen = 0;
    s32 ret;
    SDK_ICC_FIXED_AID FixedAid;
    u8 selectnextflag = 0;

    if ((pstIccTradeParam == NULL) || (pstIccParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if ((SDK_ICC_RF != pstIccParam->ucCardMode) && (SDK_ICC_ICC !=
                                                    pstIccParam->ucCardMode))                                           //����IC��
    {
        return SDK_ERR;
    }

    if (pstIccParam->ucCLType == 0x02) //�Ѿ�ȷ������qpboc����ʱ��ض�����stm32�����
    {
        return sdkIccDealQReaderTrade(pstIccTradeParam); //���������״���
    }

    if (SDK_OK != sdkIccTransInit(
            pstIccTradeParam))                 //��ʼ������                          //jiguang 20120519
    {
        return SDK_PARA_ERR;
    }

    Trace("emv", "pstIccParam->ucCardMode1 = %d", pstIccParam->ucCardMode);
#if 0 /*Modify by ���ƽ at 2015.04.09  14:19 */
    if(SDK_ICC_ICC == pstIccParam->ucCardMode)
    {
        sdkmSleep(800);     //�Ӵ�ʽIC����λҪͣ��800ms,��Ȼ�п��ܻḴλ���ɹ�
    }
#endif /* if 0 */
//zhouxiaoxin   正常应该为外部去 poweron 或 active,否则重复做两次,这里会失败
//#ifndef SPI_DDI
//    Trace("emv", "StartResetIcc\r\n");
//
//    ret = sdkIccResetIcc(pstIccParam, OutBuf, &OutLen);       //IC����λ
//
//    Trace("emv", "sdkIccResetIcc==%d\r\n", ret);
//
//    if (SDK_OK != ret) {
//        if (pstIccTradeParam->bIsFallback) {
//            return SDK_NeedMagCardErr;
//        }
//        else {
//            return SDK_ICC_RESET_ERR;
//        }
//    }
//#endif
    Trace("emv", "pstIccParam->ucCardMode2 = %d", pstIccParam->ucCardMode);
    ret = sdkIccGetAppCandidate(pstIccTradeParam,
                                pstIccParam);                                           // ��ȡӦ�ú�ѡ�б�
    Trace("emv", "sdkIccGetAppCandidate==%d\r\n", ret);
    sdkmSleep(1000);
    if (ret != SDK_OK) {
        return ret;
    }
    memcpy(&FixedAid, &pstIccTradeParam->stFixedAid, sizeof(SDK_ICC_FIXED_AID));

    //while(1)
    {
        if (selectnextflag)  //sxl ֻ��Ҫ����������ʼ��
        {
            EMVB_restarttrade_SelectNext(pstIccTradeParam, pstIccParam);
        }

        ret = sdkIccGoToSelectApp(FixedAid, pstIccTradeParam, pstIccParam);      //ȥѡ��Ӧ��
        selectnextflag = 1;

        if (ret == SDK_FixedAidAbsentErr)       //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
        {
            FixedAid.bIsFixed = false;           //ѡ��һ��֮�������־ sjz 20120508
            //continue;
        }
        else if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
        {
            if (pstIccTradeParam->bIsFallback) {
                return SDK_NeedMagCardErr;
            }
        }
        else if (ret == SDK_OK) {
            Trace("emv", "pstIccTradeParam->eFlowMode = %d\r\n", pstIccTradeParam->eFlowMode);

            //��������ϸ
            if (SDK_ICC_READLOG == pstIccTradeParam->eFlowMode) {
                return SDK_DispLog;                                           //��ʾ������ϸ
            }
                //��ѯ���
            else if (SDK_ICC_EC_QUERY == pstIccTradeParam->eFlowMode) {
                return sdkIccReadECBalance(pstIccTradeParam, pstIccParam);     //jiguang  20120602
            }
            Trace("emv", "start init app\r\n");
            ret = sdkIccGoToInitialApp(pstIccTradeParam, pstIccParam);      //ȥ��ʼ��Ӧ��
        }
        Trace("emv", "ret  = %d\r\n", ret);
        if (ret == SDK_AppSelectTryAgainErr) {
            if (NULL != pstIccTradeParam->DispReselectApp) {
                ret = (*pstIccTradeParam->DispReselectApp)();                                              // ��ʾ"����ѡ��"

                if (ret != SDK_OK) {
                    return ret;
                }
            }
            ret = sdkIccReSelectApp(); //����ѡ��Ӧ��

            if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
            {
                if (pstIccTradeParam->bIsFallback) {
                    return SDK_NeedMagCardErr;
                }
            }

            if (ret != SDK_OK) {
                return ret;
            }
            //continue;  //sxl continue���̵�Ҫ���´���
        }
        else if (SDK_OnlineWait ==
                 ret)                                               // qpboc����(�Ǵ������ֽ��˻�)
        {
            return sdkIccDealQpbocOnline(pstIccTradeParam, pstIccParam);
        }
        else if (SDK_OfflineApprove ==
                 ret)                                               //qPboc�ѻ���׼
        {
            return sdkIccDealQpbocOffline(pstIccTradeParam, pstIccParam);        //jiguang 20120602
        }
        else if (ret != SDK_OK) {
            return ret;
        }
        //break;
    }
#ifdef SDK_MEM_DEBUG
    sdkGetMemStatus();
#endif
    return sdkIccDealEmvFlow(pstIccTradeParam, pstIccParam);      //lint !e438 ����emv����
}

s32 sdkIccTransFlow1(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam) {
    s32 retCode;

    retCode = sdkIccTransFlow1Process(pstIccTradeParam, pstIccParam);
    sdkIccSaveEmvTradeGlbValue();
    sdkIccTransFreePartMem(retCode);

    return retCode;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_PARA_ERR:��������
                                SDK_ERR:����ʧ��
                                SDK_TransOnlineApprove:�����ɹ�
                                SDK_TransOnlineDeclineErr:�����ܾ�
                                SDK_TransOfflineApprove:�ѻ���׼
                                SDK_TransOfflineDeclineErr:�ѻ��ܾ�
                                SDK_TransNotAcceptErr:��Ƭ������������ķ���
                                SDK_TransTerminateErr:����ʧ��
                                SDK_IccCommandErr :��������
                                SDK_CDAFormatErr:CDA���ݸ�ʽ��
                                SDK_AppTimeOutErr: �������ʱ
                                SDK_TransOK_ARPCErr

** Created By:		sujianzhong2012.07.30
** Remarks:
*****************************************************************************/
s32 sdkIccTransFlow2Process(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                            const SDK_ICC_PARAM *pstIccParam) {
    s32 ret;

    if ((pstIccTradeParam == NULL) || (pstIccParam == NULL)) {
        return SDK_PARA_ERR;
    }

//    if(SDK_ICC_RF == pstIccParam->eCardType)
    if (SDK_ICC_RF == pstIccParam->ucCardMode) //zxx 20130705 17:49
    {
        if (pstIccParam->ucCLType != 0x01) //�ǽ�PBOC�����������,qpboc�����������
        {
//            return SDK_ERR;
            return SDK_TransOnlineApprove; //zxx 20130705 17:11 �ϲ�02Э��Qת�����������flow2�����·���ʧ�ܳ���
        }
    }

    if (SDK_ICC_ICC != pstIccParam->ucCardMode &&
        SDK_ICC_RF != pstIccParam->ucCardMode)     //���ǽӴ�ʽIC��,qpboc����Ҫ��������
    {
        return SDK_ERR;
    }

    while (1) {
        ret = sdkIccTransProcess(pstIccTradeParam,
                                 pstIccParam);                                            //���״���
#if 0

        if(SDK_NeedRefer == ret)
        {
            sdkIccProcessRefer(1);                                           //����ο�
            continue;
        }
        else
#endif

        if ((ret == SDK_NeedReversal) || (SDK_OK == ret) || (SDK_ECLoadErr == ret)) {
            break;
        }
        else {
            return ret;
        }
    }

    return sdkIccTransComplete(); //�������
}

s32 sdkIccTransFlow2(const SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                     const SDK_ICC_PARAM *pstIccParam) {
    s32 retCode;

    retCode = sdkIccTransFlow2Process(pstIccTradeParam, pstIccParam);
    sdkIccSaveEmvTradeGlbValue();
    sdkIccTransFreePartMem(retCode);

    return retCode;
}

/*****************************************************************************
** Descriptions:
** Parameters:          SDK_ICC_TRADE_PARAM * pstIccTradeParam:
                                const SDK_ICC_PARAM * pstIccParam:
** Returned value:	SDK_NeedMagCardErr ʹ��fallback����
                                SDK_ERR ����ʧ��
                                SDK_IccCommandErr ����ʧ��
                                SDK_CardBlockErr ��������
                                SDK_IccDataFormatErr ���ݸ�ʽ��
                                SDK_CancelTransErr ȡ������
                                SDK_NoAppSelErr û��Ӧ��
                                SDK_AppTimeOutErr:��ʱ
                                SDK_IccDataRedundErr:�������ݴ�
                                SDK_CDAFormatErr :CDA��ʽ��
                                SDK_IccReturnErr ������ʧ��
                                SDK_OK:��������
                                SDK_PARA_ERR:��������
                                SDK_ICC_RESET_ERR:��λʧ��
                                SDK_ECNotSupportErr:  �������ֽ𿨲�֧�ָù���
** Created By:		sujianzhong2012.07.30
** Remarks:               //sxl?���ƻ���Ҫ�ٰ�װһ�£��鿴һ��Ӧ�ó���Ҫ��δ���
*****************************************************************************/
s32 Private_sdkIccLoadAccNoProcess(SDK_ICC_TRADE_PARAM *pstIccTradeParam,
                                   SDK_ICC_PARAM *pstIccParam) {
    u8 OutBuf[300];
    u8 tvr[5];
    s32 OutLen = 0;
    s32 ret, tvr_len;
    SDK_ICC_FIXED_AID FixedAid;
    SDK_ICC_CARDDATA carddata;
    u8 selectnextflag = 0;

    if ((pstIccTradeParam == NULL) || (pstIccParam == NULL)) {
        return SDK_PARA_ERR;
    }

    if (SDK_ICC_ICC != pstIccParam->ucCardMode)                                           //����IC��
    {
        return SDK_ERR;
    }
//    DispWait();

    if (SDK_OK != sdkIccTransInit(
            pstIccTradeParam))                 //��ʼ������                          //jiguang 20120519
    {
        return SDK_PARA_ERR;
    }

    if (SDK_ICC_ICC == pstIccParam->ucCardMode) {
        sdkmSleep(800);     //�Ӵ�ʽIC����λҪͣ��800ms,��Ȼ�п��ܻḴλ���ɹ�
    }
    Trace("emv", "StartResetIcc\r\n");

    ret = sdkIccResetIcc(pstIccParam, OutBuf, &OutLen);       //IC����λ

    Trace("emv", "sdkIccResetIcc==%d\r\n", ret);

    if (SDK_OK != ret) {
        if (pstIccTradeParam->bIsFallback) {
            return SDK_NeedMagCardErr;
        }
        else {
            return SDK_ICC_RESET_ERR;
        }
    }
    ret = sdkIccGetAppCandidate(pstIccTradeParam,
                                pstIccParam);                                          // ��ȡӦ�ú�ѡ�б�

    if (ret != SDK_OK) {
        return ret;
    }
    memset(&FixedAid, 0, sizeof(SDK_ICC_FIXED_AID));

    while (1) {
        if (selectnextflag)  //sxl ֻ��Ҫ����������ʼ��
        {
            EMVB_restarttrade_SelectNext(pstIccTradeParam, pstIccParam);
        }
        ret = sdkIccGoToSelectApp(FixedAid, pstIccTradeParam, pstIccParam);     //ȥѡ��Ӧ��
        selectnextflag = 1;


        if (ret == SDK_FixedAidAbsentErr)       //ѡ�񲻵��̶�AID,������֯��ѡ�б��û�ѡ��
        {
            FixedAid.bIsFixed = false;           //ѡ��һ��֮�������־ sjz 20120508
            continue;
        }
        else if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
        {
            if (pstIccTradeParam->bIsFallback) {
                return SDK_NeedMagCardErr;
            }
        }
        else if (ret == SDK_OK) {
            Trace("emv", "pstIccTradeParam->eFlowMode = %d\r\n", pstIccTradeParam->eFlowMode);
            ret = sdkIccGoToInitialApp(pstIccTradeParam, pstIccParam);      //ȥ��ʼ��Ӧ��
        }

        if (ret == SDK_AppSelectTryAgainErr) {
            if (NULL != pstIccTradeParam->DispReselectApp) {
                ret = (*pstIccTradeParam->DispReselectApp)();                                              // ��ʾ"����ѡ��"

                if (ret != SDK_OK) {
                    return ret;
                }
            }
            ret = sdkIccReSelectApp();         //����ѡ��Ӧ��

            if (SDK_NoAppSelErr == ret)     //��ѡӦ�ú�û�к�ѡӦ�ÿ�ѡ��������FallBack����
            {
                if (pstIccTradeParam->bIsFallback) {
                    return SDK_NeedMagCardErr;
                }
            }

            if (ret != SDK_OK) {
                return ret;
            }
            continue;
        }
        else if (ret != SDK_OK) {
            return ret;
        }
        break;
    }

    ret = sdkIccReadAppData(pstIccTradeParam,
                            pstIccParam);                                           //��Ӧ��
    Trace("emv", "sdkIccReadAppData return value = %d\r\n", ret);

    if (SDK_OK != ret) {
        return ret;
    }

    if (NULL != pstIccTradeParam->VerifyCardNo)     //sjz �ȽϺ�����
    {
        memset(&carddata, 0, sizeof(SDK_ICC_CARDDATA));

        if (SDK_OK != sdkIccGetCardData(&carddata)) {
            memset(&carddata, 0, sizeof(SDK_ICC_CARDDATA));
        }
        TraceHex("emv", "����", carddata.asCardNO,
                 carddata.ucCardLen); //sujianzhong 2013.01.10 14:36
        ret = (*pstIccTradeParam->VerifyCardNo)(
                carddata.asCardNO);                                       // ȷ�Ͽ���
        Trace("emv", "pstIccTradeParam->VerifyCardNo return value = %d\r\n", ret);

        if (ret != SDK_OK) {
            return ret;
        }
    }
    ret = sdkIccDataAuth(pstIccTradeParam, pstIccParam);     //��̬������֤

    if (SDK_OK == ret) {
        if (SDK_OK == sdkIccReadTLV("\x95", tvr, &tvr_len)) {
            if (tvr[0] & 0x4C) {     //������֤����
                ret = SDK_ERR;
            }
        }
        else {
            ret = SDK_ERR;
        }
    }
    return ret;//lint !e438
}

s32 Private_sdkIccLoadAccNo(SDK_ICC_TRADE_PARAM *pstIccTradeParam, SDK_ICC_PARAM *pstIccParam) {
    s32 retCode;

    retCode = Private_sdkIccLoadAccNoProcess(pstIccTradeParam, pstIccParam);
    sdkIccSaveEmvTradeGlbValue();
    sdkIccTransFreePartMem(retCode);

    return retCode;
}

