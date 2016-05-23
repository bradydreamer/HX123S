package com.nexgo.db;

/**
 * Created by xuefeng on 2016/3/1.
 */
public class DbParamTables {

    public static String CardTypeTotalInfoTable = "CREATE TABLE IF NOT EXISTS CardTypeTotalInfo(" +
            "pid INTEGER PRIMARY KEY AUTOINCREMENT," +                      //标识ID，自增
            "TotalInSaleAmount TEXT DEFAULT 0," +                           //内卡消费交易总额
            "TotalInUnSaleAmount TEXT DEFAULT 0," +                         //内卡消费撤销
            "TotalInRefundAmount TEXT DEFAULT 0," +                         //内卡退货交易总额
            "TotalInCompleteAmount TEXT DEFAULT 0," +                       //内卡预授权完成交易总额
            "TotalInUnCompleteAmount TEXT DEFAULT 0," +                     //内卡预授权完成撤销交易总额

            "TotalInOfflineAmount TEXT DEFAULT 0," +              //内卡离线交易总额
            "TotalInOffCompleteAmount TEXT DEFAULT 0," +          //内卡预授权完成(离线)
            "TotalInECConsume TEXT DEFAULT 0," +                  //内卡电子现金交易
            "TotalInECCash TEXT DEFAULT 0," +                     //内卡现金圈存
            "TotalInMagLoadAmount TEXT DEFAULT 0," +              //内卡磁条卡充值金额
            "TotalInAppointLoadAmount TEXT DEFAULT 0," +          //指定帐户圈存总额
            "TotalInEcSaleAmount TEXT DEFAULT 0," +               //ec 消费

            "TotalInSaleNum TEXT DEFAULT 0," +                      //内卡消费交易总笔数
            "TotalInUnSaleNum TEXT DEFAULT 0," +                    //内卡消费撤销交易总笔数

            "TotalInRefundNum TEXT DEFAULT 0," +                   //内卡退货交易总笔数
            "TotalInCompleteNum TEXT DEFAULT 0," +                 //内卡预授权完成交易总笔数
            "TotalInUnCompleteNum TEXT DEFAULT 0," +               //内卡预授权完成撤销交易总笔数  xule120214


            "TotalInOfflineNum TEXT DEFAULT 0," +                  //内卡离线交易总笔数
            "TotalInOffCompleteNum TEXT DEFAULT 0," +              //内卡预授权完成(离线)笔数
            "TotalInECConsumeNum TEXT DEFAULT 0," +                //内卡电子现金交易笔数
            "TotalInECCashNum TEXT DEFAULT 0," +                   //内卡现金圈存笔数
            "TotalInMagLoadNum TEXT DEFAULT 0," +                  //内卡磁条卡充值笔数
            "TotalInAppointLoadNum TEXT DEFAULT 0," +
            "TotalInEcSaleNum TEXT DEFAULT 0," +                  //ec 消费


            "TotalOutSaleAmount TEXT DEFAULT 0," +                 //外卡消费交易总额
            "TotalOutUnSaleAmount TEXT DEFAULT 0," +               //外卡消费撤销交易总额

            "TotalOutRefundAmount TEXT DEFAULT 0," +               //外卡退货交易总额
            "TotalOutCompleteAmount TEXT DEFAULT 0," +             //外卡预授权完成交易总额
            "TotalOutUnCompleteAmount TEXT DEFAULT 0," +           //外卡预授权完成撤销交易总额  xule120214


            "TotalOutOfflineAmount TEXT DEFAULT 0," +              //外卡离线交易总额
            "TotalOutOffCompleteAmount TEXT DEFAULT 0," +           //外卡预授权完成(离线)
            "TotalOutECConsume TEXT DEFAULT 0," +                  //外卡电子现金交易
            "TotalOutECCash TEXT DEFAULT 0," +                     //外卡现金圈存
            "TotalOutMagLoadAmount TEXT DEFAULT 0," +               //外卡磁条卡充值金额
            "TotalOutEcSaleAmount TEXT DEFAULT 0," +               //ec 消费

            "TotalOutSaleNum TEXT DEFAULT 0," +                  //外卡消费交易总笔数
            "TotalOutUnSaleNum TEXT DEFAULT 0," +                //外卡消费撤销交易总笔数

            "TotalOutRefundNum TEXT DEFAULT 0," +                //外卡退货交易总笔数
            "TotalOutCompleteNum TEXT DEFAULT 0," +              //外卡预授权完成交易总笔数
            "TotalOutUnCompleteNum TEXT DEFAULT 0," +            //外卡预授权完成撤销交易总笔数


            "TotalOutOfflineNum TEXT DEFAULT 0," +                //外卡离线交易总笔数
            "TotalOutOffCompleteNum TEXT DEFAULT 0," +             //外卡预授权完成(离线)笔数
            "TotalOutECConsumeNum TEXT DEFAULT 0," +              //外卡电子现金交易笔数
            "TotalOutECCashNum TEXT DEFAULT 0," +                 //外卡现金圈存笔数
            "TotalOutMagLoadNum TEXT DEFAULT 0," +                 //外卡磁条卡充值笔数
            "TotalOutEcSaleNum TEXT DEFAULT 0," +                 //ec 消费

            "TotalInDebitAmount TEXT DEFAULT 0," +                  //内卡总的借计金额
            "TotalInCreditAmount TEXT DEFAULT 0," +                 //内卡总的贷计金额
            "TotalInDebitNum TEXT DEFAULT 0," +                  //内卡总的借计笔数
            "TotalInCreditNum TEXT DEFAULT 0," +                 //内卡总的贷计笔数

            "TotalOutDebitAmount TEXT DEFAULT 0," +                 //外卡总的借计金额
            "TotalOutCreditAmount TEXT DEFAULT 0," +                //外卡总的贷计金额
            "TotalOutDebitNum TEXT DEFAULT 0," +                 //外卡总的借计笔数
            "TotalOutCreditNum TEXT DEFAULT 0)";                 //外卡总的贷计笔数

    public static String TradeTotalInfoTable = "CREATE TABLE IF NOT EXISTS TradeTotalInfo(" +

            "pid INTEGER PRIMARY KEY AUTOINCREMENT," +                 //标识ID，自增
            "TotalSaleNum  TEXT DEFAULT 0," +                     //消费交易总笔数(不包括已撤销)
            "TotalSaleAmount TEXT DEFAULT 0," +                     //消费交易总金额(不包括已撤销)

            "TotalSaleNumAll  TEXT DEFAULT 0," +                     //消费交易总笔数(包括已撤销)
            "TotalSaleAmountAll TEXT DEFAULT 0," +                   //消费交易总金额(包括已撤销)

            "TotalUnSaleNum  TEXT DEFAULT 0," +                   //消费撤销交易总笔数
            "TotalUnSaleAmount TEXT DEFAULT 0," +                    //消费撤销

            "TotalRefundNum  TEXT DEFAULT 0," +                   //退货交易总笔数
            "TotalRefundAmount TEXT DEFAULT 0," +                    //退货交易总额

            "TotalCompleteNum  TEXT DEFAULT 0," +                 //预授权完成交易总笔数(不包括已撤销)
            "TotalCompleteAmount TEXT DEFAULT 0," +                  //预授权完成交易总金额(不包括已撤销)

            "TotalCompleteNumAll  TEXT DEFAULT 0," +                 //预授权完成交易总笔数(包括已撤销)
            "TotalCompleteAmountAll TEXT DEFAULT 0," +               //预授权完成交易总金额(包括已撤销)

            "TotalUnCompleteNum  TEXT DEFAULT 0," +               //预授权完成撤销交易总笔数  xule120214
            "TotalUnCompleteAmount TEXT DEFAULT 0," +                //预授权完成撤销交易总额

            "TotalOfflineNum  TEXT DEFAULT 0," +                  //离线交易总笔数
            "TotalOfflineAmount TEXT DEFAULT 0," +                   //离线交易总额

            "TotalOffCompleteNum  TEXT DEFAULT 0," +                 //预授权完成(离线)笔数
            "TotalOffCompleteAmount TEXT DEFAULT 0," +                 //预授权完成(离线)

            "TotalECConsumeNum  TEXT DEFAULT 0," +                //电子现金交易笔数
            "TotalECConsumeAmount TEXT DEFAULT 0," +                 //电子现金交易

            "TotalECCashNum  TEXT DEFAULT 0," +                   //现金圈存笔数
            "TotalECCashAmount TEXT DEFAULT 0," +                    //现金圈存

            "TotalMagLoadNum  TEXT DEFAULT 0," +                  //磁条卡充值笔数
            "TotalMagLoadAmount TEXT DEFAULT 0," +                     //磁条卡充值金额

            "TotalAppointLoadNum  TEXT DEFAULT 0," +
            "TotalAppointLoadAmount TEXT DEFAULT 0," +               //指定帐户圈存总额

            "TotalEcSaleNum  TEXT DEFAULT 0," +                     //ec（电子现金） 消费
            "TotalEcSaleAmount TEXT DEFAULT 0)";                     //ec(电子现金) 消费

    public static String termParamTable = "CREATE TABLE IF NOT EXISTS TerminalParam(" +
            "pid INTEGER PRIMARY KEY AUTOINCREMENT," +                        //标识ID，自增
            "MerchantNO TEXT  DEFAULT '123456789011111'," +                          //商户编号
            "TerminalN0 TEXT  DEFAULT '11115001'," +                          //终端编号
            "SecurePwd TEXT DEFAULT 123456," +                            //安全密码
            "MerchantName TEXT NOT NULL DEFAULT '新国都'," +                     //商户名称
            "VoucherNO TEXT DEFAULT 000001," +                                //流水号 1-999999
            "BatchNO TEXT DEFAULT 000250," +                                    //批次号 1-999999
            "MaxRefundAmount TEXT  NOT NULL DEFAULT '10000'," +                  //最大退货金额
            "SettlePrintDetailSetting  TEXT DEFAULT 0," +                //结算打印明细设置
            "EnglishSetting TEXT DEFAULT '1'," +                            //英文设置 热敏时出现，缺省为是
            "VoucherSetting TEXT NOT NULL DEFAULT ''," +                     //签购单设置 针打时出现，缺省为新（true）
            "VoucherTitleSetting TEXT DEFAULT 银联POS签购单 ," +            //签购单抬头设置
            "DefaultTrade TEXT DEFAULT 00," +                            //默认交易设置 消费/预授权 默认消费(参考交易处理码)
            "SaleRevokeNeedCard TEXT DEFAULT 0," +                        //消费撤销是否刷卡 默认否
            "SaleRevokeNeedPwd TEXT DEFAULT 0," +                        //消费撤销是否输密 默认否
            "PreauthCompleteRevokeNeedCard TEXT DEFAULT 0," +            //预授权完成撤销是否刷卡 默认否
            "PreauthCompleteRevokeNeedPwd TEXT DEFAULT 1," +            //预授权完成撤销是否输密 默认是
            "PreauthRevokeNeedPwd TEXT DEFAULT 1," +                    //预授权撤销是否输密 默认是
            "PreauthCompleteNeedPwd TEXT DEFAULT 1)";                    //预授权完成（请求）是否输密 默认是


    public static String OperatorTable = "CREATE TABLE IF NOT EXISTS OperatorParam(" +
        "pid INTEGER PRIMARY KEY AUTOINCREMENT," +                        //标识ID，自增
        "OperatorNo TEXT NOT NULL DEFAULT 99," +                          //操作员号
        "OperatorKey TEXT NOT NULL DEFAULT 12345678)";                    //操作员密码


    public static String commParamTable = "CREATE TABLE IF NOT EXISTS CommunicateParam(" +
            "pid INTEGER PRIMARY KEY AUTOINCREMENT," +                        //标识ID，自增
            "IsSupportTelnet TEXT DEFAULT 1," +                        //是否支持拨号
            "IsSupportSerial TEXT DEFAULT 0," +                        //是否支持串口
            "IsSupportGprs TEXT DEFAULT 1," +                            //是否支持GPRS
            "IsSupportCDMA TEXT DEFAULT 0," +                            //是否支持CDMA
            "IsSupportEther TEXT DEFAULT 1," +                            //是否支持以太网
            "TPDU TEXT DEFAULT 6000000000," +                                        //TPDU 默认 60
            "FF8008 TEXT DEFAULT 0," +                                //IsPredial 是否预拨号 默认是
            "TradeTimeout TEXT DEFAULT 60," +                            //交易超时时间 默认 60S
            "TradeRetryDialAmount TEXT DEFAULT 3," +                    //交易重拔次数 默认 3次
            "FF8003 TEXT NOT NULL  DEFAULT ''," +                                //TelOutsideNO外线号码
            "FF8000 TEXT  NOT NULL DEFAULT ''," +                            //TelCenterTradeNO1中心交易号码 1
            "FF8001 TEXT  NOT NULL DEFAULT ''," +                            //TelCenterTradeNO2中心交易号码 2
            "FF8002 TEXT  NOT NULL DEFAULT ''," +                            //TelCenterTradeNO3中心交易号码 3

            "GprsInsertNO TEXT NOT NULL DEFAULT 0," +                                //接入号码
            "FF8004 TEXT NOT NULL  DEFAULT ''," +                                //GprsAPN1Name APN1名称
            "GprsHost1IPAdr TEXT  NOT NULL DEFAULT ''," +                            //主机 1 IP 地址
            "GprsHost1Port TEXT  NOT NULL DEFAULT ''," +                                //主机 1 端口
            "GprsHost2IPAdr TEXT  NOT NULL DEFAULT ''," +                            //主机 2 IP 地址
            "GprsHost2Port TEXT  NOT NULL DEFAULT ''," +                                //主机 2 端口
            "GprsIsUserSetting TEXT DEFAULT 0," +                        //用户名设置 默认为否
////////////
            "CdmaInsertNO TEXT NOT NULL DEFAULT 0," +                                //接入号码
            "FF8005 TEXT NOT NULL DEFAULT ''," +                                //CdmaAPN1Name APN2名称
            "CdmaHost1IPAdr TEXT  NOT NULL DEFAULT ''," +                            //主机 1 IP 地址
            "CdmaHost1Port TEXT  NOT NULL DEFAULT ''," +                                //主机 1 端口
            "CdmaHost2IPAdr TEXT  NOT NULL DEFAULT ''," +                            //主机 2 IP 地址
            "CdmaHost2Port TEXT  NOT NULL DEFAULT ''," +                                //主机 2 端口
            "CdmaUserName TEXT NOT NULL DEFAULT ''," +                                //用户名
            "CdmaUserPwd TEXT NOT NULL DEFAULT ''," +                                //密码
//////////
            "EtherLocalIP TEXT NOT NULL DEFAULT '172.21.1.59'," +                                //本地IP
            "EtherSubnetMask TEXT NOT NULL DEFAULT '255.255.255.0'," +                            //子网掩码
            "EtherGateway TEXT NOT NULL DEFAULT '172.21.1.0'," +                                //网关
            "EtherHost1IPAdr TEXT  NOT NULL DEFAULT '172.21.1.59'," +                            //主机 1 IP 地址
            "EtherHost1Port TEXT  NOT NULL DEFAULT '8000'," +                  //主机 1 端口
            "EtherHost2IPAdr TEXT  NOT NULL DEFAULT '172.21.1.59'," +                            //主机 2 IP 地址
            "EtherHost2Port TEXT  NOT NULL  DEFAULT '8000'," +                             //主机 2 端口
            "FF8010 TEXT NOT NULL DEFAULT 0," +                                       //MacTag Mac算法标识
            "FF8015 TEXT  NOT NULL  DEFAULT 0," +                                       //TdInfo 磁道信息加密标志 0-不加密 1-加密
            "FF8016 TEXT NOT NULL DEFAULT 1," +                                //TerKeyIndex 终端密钥索引
            "FF8017 TEXT NOT NULL DEFAULT 0)";                                     //KeyTag 加密算法标识 0-单倍长 6-双倍长
}
