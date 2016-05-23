#include <android/log.h>

#define  LOG_TAG    "daemon-Transflow"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern cmd_Str const DipCmdTab[];
extern cmd_Str const KeyBoardCmdTab[];
extern cmd_Str const StoreCmdTab[];
extern cmd_Str const IcCardCmdTab[];
extern cmd_Str const PrinterCmdTab[];
extern cmd_Str const TsCmdTab[];
extern cmd_Str const MagCardCmdTab[];
extern cmd_Str const SecurityAuthCmdTab[];
extern cmd_Str const PbocCmdTab[];
extern cmd_Str const DataTransCmdTab[];
extern cmd_Str const PinpadCmdTab[];

CmdClassStr const cmdClassTab[] = {
        {DISPLAY_CLASS,       (cmd_Str *) DipCmdTab},
        {KEYBOARD_CLASS,      (cmd_Str *) KeyBoardCmdTab},
        {STORE_REC_CLASS,     (cmd_Str *) StoreCmdTab},
        {MAGNETIC_CARD_CLASS, (cmd_Str *) MagCardCmdTab},
        {IC_CARD_CLASS,       (cmd_Str *) IcCardCmdTab},
        {SECURITY_CLASS,      (cmd_Str *) SecurityAuthCmdTab},
        {PIN_PAD_CLASS,       (cmd_Str *) PinpadCmdTab},
        //{ PRINT_CLASS,                          (cmd_Str *)PrinterCmdTab },
        {PBOC_CLASS,          (cmd_Str *) PbocCmdTab},
        {TER_MANAGE_CLASS,    (cmd_Str *) TsCmdTab},
        {DATA_TRANSMIT_CLASS, (cmd_Str *) DataTransCmdTab}
};

/*******************************************************************
   版    权: 深圳新国都股份有限公司
   函数功能: 打印命令对应的说明
   入口参数:
   返 回 值:
   备    注:
   修改信息:
 ********************************************************************/
void printCmdString(int cmdClass, int cmdID) {
    u8 str1[30] = {0};
    u8 str2[30] = {0};
    u8 str3[60] = {0};

    switch (cmdClass) {
        case DISPLAY_CLASS:
            strcpy(str1, "显示类");
            break;

        case KEYBOARD_CLASS:
            strcpy(str1, "键盘类");

            switch (cmdID) {
                case BUFVALUE_ENUM:
                    strcpy(str2, "获取缓冲键值");
                    break;

                case VALUE_ENUM:
                    strcpy(str2, "获取按键值");
                    break;

                case STRING_ENUM:
                    strcpy(str2, "获取字符串");
                    break;

                case STORTSTRING_ENUM:
                    strcpy(str2, "获取字符串并保存");
                    break;
            }

            break;

        case STORE_REC_CLASS:
            strcpy(str1, "存储记录类");

            switch (cmdID) {
                case STORE_INIT_REC:
                    strcpy(str2, "初始化存储记录");
                    break;

                case STORE_GET_RECCOUNT:
                    strcpy(str2, "获取存储记录数");
                    break;

                case STORE_ADD_REC:
                    strcpy(str2, "增加存储记录");
                    break;

                case STORE_UPDATE_REC:
                    strcpy(str2, "更新存储记录");
                    break;

                case STORE_GET_REC:
                    strcpy(str2, "获取存储记录");
                    break;
            }

            break;

        case MAGNETIC_CARD_CLASS:
            strcpy(str1, "磁条卡类");

            switch (cmdID) {
                case MAG_OPEN_DEVICE:
                    strcpy(str2, "打开读卡器");
                    break;

                case MAG_CLOSE_DEVICE:
                    strcpy(str2, "关闭读卡器");
                    break;

                case MAG_READ_CARDDATA_NEN:
                    strcpy(str2, "读磁条卡数据(明文)");
                    break;

                case MAG_READ_CARDDATA_EN:
                    strcpy(str2, "读磁条卡数据(密文)");
                    break;

                case MAG_READ_CARDDATA_INSIDE_SAVE:
                    strcpy(str2, "读磁条卡数据内部保存");
                    break;

                case MAG_READ_CARDDATA_AUDIO:
                    strcpy(str2, "AUDIO");
                    break;
            }

            break;

        case IC_CARD_CLASS:
            strcpy(str1, "IC卡类");

            switch (cmdID) {
                case IC_CHECK:
                    strcpy(str2, "IC卡检测");
                    break;

                case IC_SET_TYPE:
                    strcpy(str2, "设置IC卡类型");
                    break;

                case IC_POWER_ON:
                    strcpy(str2, "IC卡上电");
                    break;

                case IC_POWER_OFF:
                    strcpy(str2, "IC卡下电");
                    break;

                case IC_COMMUN:
                    strcpy(str2, "IC卡通讯");
                    break;
            }

            break;

        case SECURITY_CLASS:
            strcpy(str1, "安全认证类");

            switch (cmdID) {
                case DEVICEINFO_ENUM:
                    strcpy(str2, "获取设备信息");
                    break;

                case RANDOMNUM_ENUM:
                    strcpy(str2, "获取随机数");
                    break;

                case DEVICEAUTH_ENUM:
                    strcpy(str2, "设备认证");
                    break;
            }

            break;

        case PIN_PAD_CLASS:
            strcpy(str1, "密码键盘类");

            switch (cmdID) {
                case PASSWD_INPUT_ENUM:
                    strcpy(str2, "密码输入");
                    break;

                case LOAD_TMK_ENUM:
                    strcpy(str2, "装载主密钥");
                    break;

                case DATA_ENCRYPT_ENUM:
                    strcpy(str2, "数据加解密");
                    break;

                case MAC_CALC_ENUM:
                    strcpy(str2, "MAC计算");
                    break;

                case LOAD_WK_ENUM:
                    strcpy(str2, "装载工作密钥");
                    break;

                case MAC_CALC_INNERINFO_ENUM:
                    strcpy(str2, "MAC计算(内部)");
                    break;

                case SETTING_PASSWD_ENUM:
                    strcpy(str2, "外部密码键盘透传");
                    break;

                case PASSWD_INPUT_OUTER:
                    strcpy(str2, "密码输入(外接密码键盘)");
                    break;

                case DATA_ENCRYPT_OUTER:
                    strcpy(str2, "数据加解密(外接密码键盘)");
                    break;

                case MAC_CALC_OUTER:
                    strcpy(str2, "MAC计算(外接密码键盘)");
                    break;

                case LOAD_WK_OUTER:
                    strcpy(str2, "装载工作密钥(外接密码键盘)");
                    break;

                case LOAD_DUKPT_OUTER:
                    strcpy(str2, "装载ksn,密钥");
                    break;
            }

            break;

        case PRINT_CLASS:
            strcpy(str1, "打印类");

            switch (cmdID) {
                case INITPRINT_ENUM:
                    strcpy(str2, "初始化指令");
                    break;

                case STATUSPRINT_ENUM:
                    strcpy(str2, "获取状态");
                    break;

                case TESTPRINT_ENUM:
                    strcpy(str2, "走纸");
                    break;

                case SETTINGPRINT_ENUM:
                    strcpy(str2, "字库设置");
                    break;

                case DATAPRINT_ENUM:
                    strcpy(str2, "打印数据");
                    break;
            }

            break;

        case PBOC_CLASS:
            strcpy(str1, "PBOC类");

            switch (cmdID) {
                case PBOC_SET_PKEY:
                    strcpy(str2, "设置公钥");
                    break;

                case PBOC_SET_AID:
                    strcpy(str2, "设置 AID");
                    break;

                case PBOC_SET_TERATTR:
                    strcpy(str2, "设置终端属性");
                    break;

                case PBOC_GET_DEALDATA:
                    strcpy(str2, "取交易数据");
                    break;

                case PBOC_EXE_FLOW:
                    strcpy(str2, "执行标准流程");
                    break;

                case PBOC_SEC_AUTH:
                    strcpy(str2, "二次授权");
                    break;

                case PBOC_END_FLOW:
                    strcpy(str2, "流程结束");
                    break;

                case PBOC_EXE_QPBOC:
                    strcpy(str2, "qPBOC 流程");
                    break;

                case PBOC_GET_LASTDEAL:
                    strcpy(str2, "取末笔信息");
                    break;
            }

            break;

        case TER_MANAGE_CLASS:
            strcpy(str1, "终端管理类");

            switch (cmdID) {
                case TM_SET_BEEP:
                    strcpy(str2, "蜂鸣器蜂鸣");
                    break;

                case TM_SET_LED:
                    strcpy(str2, "指示灯闪烁指令");
                    break;

                case TM_SET_DATETIME:
                    strcpy(str2, "设置时间日期");
                    break;

                case TM_GET_DATETIME:
                    strcpy(str2, "获取时间日期");
                    break;

                case TM_SET_TER_PARAM:
                    strcpy(str2, "设置终端参数");
                    break;

                case TM_GET_TER_PARAM:
                    strcpy(str2, "获取终端参数");
                    break;

                case TM_CANCEL_OPER:
                    strcpy(str2, "取消/复位操作");
                    break;

                case TM_UPDATE:
                    strcpy(str2, "升级固件/应用");
                    break;

                case TM_ECHO:
                    strcpy(str2, "回响");
                    break;

                case TM_CTRL_DEV:
                    strcpy(str2, "控制设备");
                    break;

                case TM_GET_STATUS_DEV:
                    strcpy(str2, "获取设备状态");
                    break;
            }

            break;

        case DATA_TRANSMIT_CLASS:
            strcpy(str1, "数据透传类");
            break;
    }

    strcat(str3, str1);
    strcat(str3, ",");
    strcat(str3, str2);

    Trace("Lai", "%s\n", str3);
}

/*******************************************************************
   版    权: 深圳新国都股份有限公司
   函数功能: GetInsNum  获得指令的数量
   入口参数:
   返 回 值:
   备    注:
   修改信息:
 ********************************************************************/
u8 GetInsNum(void) {
    return sizeof(cmdClassTab) / sizeof(CmdClassStr);
}

/********************************************************************************************
   **函数名称: u8 CheckTradeBag(u8 *pheData, u16 *pusLen, u8 uctpye)
   **函数描述: 校验数据包
   **输入参数:
 **                      pucIn:    数据传入指针
 **                      nLen:   数据长度
 **                      tpye:   数据包类型
 ****输出参数:
 **
 ****返回     值:       RSLT_ESC  校验失败  RSLT_OK  校验成功
 *******************************************************************************************/
s32 CheckTradeBag(u8 *pheData, u16 *pusLen, u8 uctpye) {
    //TraceHex("mpos", "CheckTradeBag:\r\n", pheData, *pusLen);

    u8 *pCrc = NULL;
    u8 crc, flag = 0;
    u16 templen, n;
    ST_TRADEREVBAG stTradeRevBag;

    if (*pusLen > sizeof(ST_TRADEREVBAG) || *pusLen < 9) {
        return SDK_ERR;
    }
    memset(&stTradeRevBag, 0, sizeof(ST_TRADEREVBAG));

    for (n = 0; n < (*pusLen); n++) {
        if (pheData[n] == uctpye) {
            memcpy(&stTradeRevBag, &pheData[n], *pusLen - n);
            {
                flag = 1;
            }
            break;
        }
    }

    if (flag) {
        sdkBcdToU16(&templen, stTradeRevBag.heMsglen, 2);

        if ((templen + 5) > sizeof(ST_TRADEREVBAG)) {
            return SDK_ERR;
        }
        crc = GetCRC((u8 * ) & stTradeRevBag.heMsglen, templen + 3);    //算效验位
        pCrc = (u8 * ) & stTradeRevBag;

        //Trace("","&&&&&&&& crc:%x  len:%d,,,pcrc:%x\r\n",crc,templen+5,pCrc[templen+5-1]);
        if (crc == pCrc[templen + 5 - 1]) //crc在倒数第二个byte
        {
            *pusLen = templen + 5; //数据总长度
            memcpy(pheData, &stTradeRevBag, *pusLen);
            return SDK_OK;
        }
        else {
            return SDK_ESC;
        }
    }
    else {
        return SDK_ERR;
    }
}

/********************************************************************************************
   **函数名称:        u8 PostTransFlow( MSG *msg )
   **函数描述:        解析报文+ 发起执行指令流程
   **输入参数:
   **msg  消息结构体指针
   **输出参数:
 **
 ****返回     值:      SDK_OK   成功  SDK_OK 失败(失败是发错误应答报文然后退出)
 *******************************************************************************************/
s32 PostTransFlow(u8 *buf, u16 len) {
    ST_TRANSFLOW *TransFlow = NULL;
    ST_TRADEREVBAG stTradeRevBag;
    s32 rult;

    Trace("wlz", "====PostTransFlow=====\r\n");
    //sdkDispLabel(SDK_DISP_LINE2,"开始执行 PostTransFlow",SDK_DISP_CDISP|SDK_DISP_FDISP);

    memset(&stTradeRevBag, 0x00, sizeof(stTradeRevBag));

    TransFlow = (ST_TRANSFLOW *) sdkGetMem(sizeof(ST_TRANSFLOW));

    if (TransFlow == NULL) {
        return SDK_ERR;
    }
    //校验+解析报文
    Trace("wlz", "====解析校验报文=====\r\n");
    rult = CheckTradeBag(buf, &len, SOP_DATA);
    //rult =SDK_OK;

    //TraceHex("wlz", "===解析得到的数据=====\r\n", buf, len);
    if (SDK_ERR == rult)    //数据错乱
    {
        sdkFreeMem(TransFlow);
        Trace("wlz", "================无法识别的数据=================\r\n");
        return SDK_ERR;
    }
    else if (SDK_ESC == rult) {
        //校验包错误之后会死机

        Trace("wlz", "====CheckTradeBag err ===\r\n");
        //组织发送错误应答报文

        memcpy(&stTradeRevBag, buf, len);
        TransFlow->cmdClass = stTradeRevBag.hecmdClass;
        TransFlow->cmdID = stTradeRevBag.heCmdId;
        TransFlow->heIndicatbit = 0x2F;       //stTradeRevBag.heIndicatbit;

        if (stTradeRevBag.ucNum == 0xFF) {
            TransFlow->ucNum = 0x01;
        }
        else {
            TransFlow->ucNum = stTradeRevBag.ucNum + 1;
        }
        TransFlow->asCmdAck[0] = ACK_OK;
        TransFlow->asCmdAck[1] = ACK_LRC_FAIL;
        TransFlow->usSendDataLen = 0;
        AppCommMakeBagAndSend(TransFlow);

        if (TransFlow)                    //20150112 modify by yc sdkFreeMem
        {
            if (TransFlow->TempPointer != NULL) {
                sdkFreeMem(TransFlow->TempPointer);
                TransFlow->TempPointer = NULL;
            }
            sdkFreeMem(TransFlow);
            TransFlow = NULL;
        }
        return SDK_ERR;
    }
    /***********解析填充数据********************/
    memcpy(&stTradeRevBag, buf, len);
    TransFlow->cmdClass = stTradeRevBag.hecmdClass;
    TransFlow->cmdID = stTradeRevBag.heCmdId;
    TransFlow->heIndicatbit = 0x2F;   //stTradeRevBag.heIndicatbit;

    if (stTradeRevBag.ucNum == 0xFF) {
        TransFlow->ucNum = 0x01;
    }
    else {
        TransFlow->ucNum = stTradeRevBag.ucNum + 1;
    }

    if (len > 9) { TransFlow->ReceiveDataLen = len - 9; }   //有效数据长度
    else {
        TransFlow->ReceiveDataLen = 0;
    }

    if (TransFlow->ReceiveDataLen > 0) {
        memcpy(TransFlow->heReceiveData, stTradeRevBag.heData,
               TransFlow->ReceiveDataLen);   //传入数据指针
    }
    /***********发起执行指令********************/
    //sdkDispLabel(SDK_DISP_LINE2,"开始执行指令流程",SDK_DISP_CDISP|SDK_DISP_FDISP);
    //Trace("wlz","====发起执行指令=====\r\n");
    TransFlow->ucFuncRunStep = 1;    //设置初始化步骤
    InitTransFlow(TransFlow);

    sdkSysPackMessage(DealTransFlow, (u32)(TransFlow),
                      0);                      //发送应答报文在处理流程里面完成?释放资源在里面完成?

    return SDK_OK;
}

/********************************************************************************************
   **函数名称:           void InitTransFlow( _TRANSFLOW *TransFlow )
   **函数描述:           初始化交易流程
   **输入参数:
   **TransFlow:流程结构体指针
   **输出参数:
 **
 ****返回     值:
 *******************************************************************************************/
void InitTransFlow(ST_TRANSFLOW *TransFlow) {
    //memset(TransFlow->heSendData, 0x00, sizeof(TransFlow->heSendData));                          //发送数据缓存
    TransFlow->usSendDataLen = 0;                                                                                               //发送数据长度
    /***************流程输入数据**********************/
    TransFlow->usCurrentDealLen = 0;                                                                                                    //已处理接受数据长度
    /******************执行指令相关*************************/
    TransFlow->ucInsRunStep = 0;                                                                                                //指令执行步骤
    TransFlow->ucRepeatTimes = 0;                                                                                               //重复次数
    TransFlow->bisSwipingCard = 0;                                                                                              //指令是否刷卡
    TransFlow->TradeType = 0;                                                                                                           //ic卡交易类型
    TransFlow->InsClockTimer = 0;                                                                                               //指令定时器
    TransFlow->heFindCmd = 0;
    TransFlow->heFindCmdPos = 0;
    TransFlow->heFindCmdTabPos = 0;
    TransFlow->flag1 = FALSE;                                                                                                                   //指令执行控制位
    TransFlow->flag2 = FALSE;
    TransFlow->flag3 = FALSE;

    memset(TransFlow->asCmdAck, 0x30, sizeof(TransFlow->asCmdAck) - 1);
    //==============动态内存挂载================================
    TransFlow->TempPointer = NULL;
}

extern void SetIsICOnLineTrade(u8 Type);

extern volatile u32 gScreenTimer;//ljh2015-01-08

/*******************************************************************
   作    者:
   版    权: 深圳新国都股份有限公司
   函数名称void EndTransFlow(_TRANSFLOW *TransFlow)
   函数功能: 交易流程结束(释放申请的内存资源以及相关收尾工作)
   入口参数:
   返 回 值:
   备    注:
   修改信息:
 ********************************************************************/
void EndTransFlow(ST_TRANSFLOW *TransFlow, u8 Normal) {
    Trace("wlz", "==EndTransFlow==\r\n");
    u8 cmdClass = 0;
    u8 cmdId = 0;

    SetIsICOnLineTrade(FALSE);        //复位

    if (TransFlow != NULL) {
        cmdClass = TransFlow->cmdClass;
        cmdId = TransFlow->cmdID;

        if (TransFlow->TempPointer != NULL) {
            sdkFreeMem(TransFlow->TempPointer);
            TransFlow->TempPointer = NULL;
        }
        sdkFreeMem(TransFlow); /*释放内存*/
        TransFlow = NULL;
    }

    gScreenTimer = sdkTimerGetId();        //20150310  避免交易退出后马上进入低功耗
    //Trace("mpos", "EndTransFlow: end:%d\r\n", gScreenTimer);
    //20150313 替换新的刷屏判断
    if (bisNeedBrushScreenWhenBack) {
        MainFace();
    }
    else if (!bisNeedKeepScreenWhenBack) {
        pgstTradeData->ucTradeId = TRADEID_MAINMENU;
        //DispIcon();
        //DispSysRtc(SDK_DISP_LINE5 - ROW_OFFSET, false);
    }

#if 0
    if((TransFlow->cmdClass == DISPLAY_CLASS) && JudgeTradeFlow())
    {
        Trace("wlz","== DISPLAY_CLASS ==\r\n");
    }
    else
    {
        MainFace();
    }
#endif
    /*
    FinishTradeFlow();//Xiaox 15-1-14 不知原逻辑是怎么样的，有一些地方指令结束后没有调用FinishTradeFlow();
    if(bisNeedBrushScreenWhenBack) {//Xiaox 15-1-12
        MainFace();//显示类的不刷屏 细节待定
    }else if(bisNeedKeepScreenWhenBack) {
        ClearOthers();
    }else{
        pgstTradeData->ucTradeId = TRADEID_MAINMENU;
        ClearOthers();
    }
    */

}

/********************************************************************************************
   **函数名称:     u8 DealTransFlow( MSG *msg )
   **函数描述:     交易处理函数
   **输入参数:
   **msg  消息结构体指针
   **输出参数:
 **
 ****返回     值:       MSG_KEEP   维持消息    MSG_OUT  退出消息
 *******************************************************************************************/
s32 DealTransFlow(SDK_MSG *msg) {
    ST_TRANSFLOW *TransFlow = NULL;
    u8 result = 0;

    Trace("zcl", "开始处理: Class:%d ID:%d SETP:%d\r\n",
          TransFlow->cmdClass, TransFlow->cmdID, TransFlow->ucFuncRunStep);
    TransFlow = (ST_TRANSFLOW *) msg->uiWParam;

    SetScreenStatusWhenBack(TransFlow->cmdClass, TransFlow->cmdID);
    pgstTradeData->ucTradeId = TRADEID_UNKNOWN;     //Xiaox 15-1-12 只为改变tradeid值，防止交易过程中回oncirl刷新第五行时间
    giAutoRetTimer = sdkTimerGetId();    //Xiaox 15-1-12

#if 0
    //20150320 添加复位执行
    if( (GetCmdResetExe() == TRUE) && !((TER_MANAGE_CLASS == TransFlow->cmdClass) && (TM_CANCEL_OPER == TransFlow->cmdID)) )
    {
        EndTransFlow(TransFlow, FALSE);
        FinishTradeFlow();
        SetCmdResetExe(NO_NEED_RESET);
        return SDK_MSG_OUT;  
    }
#endif
    //Trace("dcq", "DealTransFlow: Class:%d ID:%d Step:%d\r\n", TransFlow->cmdClass, TransFlow->cmdID, TransFlow->ucFuncRunStep);
    switch (TransFlow->ucFuncRunStep) {
        //case 0:   /*初始化流程      */
        //    Trace("wlz","====InitTransFlow=====\r\n");
        //    InitTransFlow(TransFlow);
        //   TransFlow->ucFuncRunStep=1;
        //   break;

        case 1:  /*执行指令           */
            //Trace("wlz","====DealInsFunc=====\r\n");
            result = DealInsFunc(TransFlow);
            //根据指令直接结果发送应答报文
            TransFlow->ucFuncRunStep = result;
            break;

        case 2:  /*异常流程结束*/
            Trace("wlz", "异常流程结束!\r\n");
            //发送错误报文
            TransFlow->asCmdAck[1] = ACK_PARAM_ERR;
            AppCommMakeBagAndSend(TransFlow);
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 3:  /*正常流程结束*/
            //发送应答报文
            //TraceHex("wlz","输出数据",TransFlow->heSendData,TransFlow->usSendDataLen);
            TransFlow->asCmdAck[1] = ACK_OK;
            AppCommMakeBagAndSend(TransFlow);
            Trace("wlz", "正常流程结束!\r\n");
            EndTransFlow(TransFlow, TRUE);
            return SDK_MSG_OUT;

        case 4:        /*不支持的指令*/
            Trace("wlz", "ACK_CMD_NOTSUPPORT!\r\n");
            TransFlow->asCmdAck[1] = ACK_CMD_NOTSUPPORT;
            AppCommMakeBagAndSend(TransFlow);
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 5:       /*超时*/
            Trace("wlz", "ACK_TIMEOUT!\r\n");
            TransFlow->asCmdAck[1] = ACK_TIMEOUT;
            AppCommMakeBagAndSend(TransFlow);
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 6:       //参数错误
            Trace("wlz", "ACK_PARAM_ERR!\r\n");
            TransFlow->asCmdAck[1] = ACK_PARAM_ERR;
            AppCommMakeBagAndSend(TransFlow);
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 7:       //其他错误
            Trace("wlz", "ACK_OTHER_ERR!\r\n");
            TransFlow->asCmdAck[1] = ACK_OTHER_ERR;
            AppCommMakeBagAndSend(TransFlow);
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 8:       //其他错误
            Trace("Mai", "报文已经发出!\r\n");
            EndTransFlow(TransFlow, FALSE);
            return SDK_MSG_OUT;

        case 10:
            Trace("Mai", "只发出响应报文，不退出消息队列!\r\n");
            AppCommMakeBagAndSend(TransFlow);
            TransFlow->ucFuncRunStep = 1;
            break;

        default:
            break;
    }

    return SDK_MSG_KEEP;
}

/*******************************************************************
   作    者:
   版    权: 深圳新国都股份有限公司
   函数名称void EndTransFlow(_TRANSFLOW *TransFlow)
   函数功能:处理指令函数
   入口参数:
   返 回 值:
   备    注:
   修改信息:
 ********************************************************************/
s32 DealInsFunc(ST_TRANSFLOW *TransFlow) {
    s32 result;
    u8 i, j;
    cmd_Str *pTab;

    //Trace("wlz","=====DealInsFunc======\r\n");
    if (0 == TransFlow->heFindCmd) {
        //Trace("wlz"," GetInsNum  %d \r\n",GetInsNum());
        Trace("wlz", "==找指令=(%d)==(%d)==\r\n", TransFlow->cmdClass, TransFlow->cmdID);
        for (i = 0; i < GetInsNum(); i++) {
            if (TransFlow->cmdClass == cmdClassTab[i].cmdClass && cmdClassTab[i].pTab != NULL) {
                pTab = cmdClassTab[i].pTab;

                for (j = 0; pTab[j].cmdID != 0xFF; j++) {
                    if (TransFlow->cmdID == pTab[j].cmdID && pTab[j].pDealCmdFun != NULL) {
                        printCmdString(TransFlow->cmdClass, TransFlow->cmdID);
                        Trace("wlz", "=====找到了指令=(%d)==(%d)==\r\n", TransFlow->cmdClass,
                              TransFlow->cmdID);
                        //找到了指令
                        TransFlow->heFindCmd = 1;
                        TransFlow->heFindCmdTabPos = i;
                        TransFlow->heFindCmdPos = j;
                        Trace("wlz", "heFindCmdPos %d \r\n", TransFlow->heFindCmdPos);
                        break;
                    }
                }

                break;
            }
        }
    }

    /***********执行指令**************/
    if (1 == TransFlow->heFindCmd) {
        pTab = cmdClassTab[TransFlow->heFindCmdTabPos].pTab;
        result = (*pTab[TransFlow->heFindCmdPos].pDealCmdFun)(TransFlow);
    }
    else {
        //如果没有找到要指令的指令返回错误应答报文
        /************************************************************/
        Trace("wlz", "=====ERR==没有找到指令==2=\r\n");

        result = INS_NOSUPPORT;

        //待完善
    }

    /************************************************************/
    switch (result) {
        case INS_SUC:
            result = 3;
            break;

        case INS_REPEAT:
        case INS_KEEP:
            /*输入数据处理*/
            result = 1;
            break;

        case INS_EXIT: /*结束所有交易流程*/
            bisNeedBrushScreenWhenBack = true;             //Xiaox 15-1-13
            result = 2;
            break;

        case INS_OVERTIME:
            bisNeedBrushScreenWhenBack = true;             //Xiaox 15-1-13
            result = 5;
            break;

        case INS_NOSUPPORT:
            bisNeedBrushScreenWhenBack = true;             //Xiaox 15-1-13
            result = 4;
            break;

        case INS_PARAM_ERR:            //参数错误
            bisNeedBrushScreenWhenBack = true;             //Xiaox 15-1-13
            result = 6;
            break;

        case INS_OTHER_ERR:    //其他错误
            bisNeedBrushScreenWhenBack = true;             //Xiaox 15-1-13
            result = 7;
            break;

        case INS_NOT_RETURN:       //响应报文已发出
            result = 8;
            break;

        case INS_SEND_NOT_OUT:       //响应报文已发出,不退出消息队列
            result = 10;
            break;

        default:
            result = 9;
            break;
    }

    //Trace("wlz","=====DealInsFunc===%d\r\n",result);
    return result;
}

