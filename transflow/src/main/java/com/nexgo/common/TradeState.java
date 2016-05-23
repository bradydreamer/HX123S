package com.nexgo.common;

/**
 * Created by xuefeng on 2016/2/25.
 */
public class TradeState  {
    public static enum GlobalState{

        SDK_OK,
        SDK_EQU,
        SDK_ERR,
        SDK_TIME_OUT,
        SDK_ESC,
        SDK_PARA_ERR,
        SDK_FUN_NULL,//函数体为空
        SDK_EBUSY,/* 设备忙 */
        SDK_EIO,/* 设备未打开 */
        SDK_EDATA,/* 数据溢出或数据错误 */

    }
    //交易ID 区分哪种交易
    public static enum tradeId
    {
        TRADEID_UNKNOWN,
        TRADEID_WELCOME,                     //欢迎界面
        TRADEID_MAINMENU,                           //主界面
        TRADEID_MANLOGIN    ,                    //操作员签到
        TRADEID_POINTLOGIN,                         //积分签到
        TRADEID_POSPARAMDL,                         //引发参数下载
        TRADEID_TMSDOWNINFO,                        //TMS参数下载
        TRADEID_QUERY,                              //查询
        TRADEID_PREAUTH ,                           //预授权
        TRADEID_UNPREAUTH ,                         //预授权撤销
        TRADEID_ADDPREAUTH ,                       //追加预授权(附加TRADEID_PREAUTH)
        TRADEID_COMPLETE ,                              //预授权完成
        TRADEID_UNCOMPLETE ,                           //预授权完成撤销
        TRADEID_SALE ,                                           //消费
        TRADEID_UNSALE ,                                        //消费撤销
        TRADEID_OFFCOMPLETE ,                           //预授权完成通知
        TRADEID_REFUND ,                                    //退货
        TRADEID_OFFSETTLE ,                                 //离线结算
        TRADEID_ADJUST ,                                       //调整
        TRADEID_TIPS ,                                          //小费
        TRADEID_OFFSALE ,                                   //普通脱机消费
        TRADEID_OFFUNSALE ,
        TRADEID_ECOFFSALE ,                                     //电子现金脱机消费
        TRADEID_ECCACC ,                                        //电子现金指定账户充值
        TRADEID_ECCASH ,                                        //电子现金现金充值
        TRADEID_ECCACCNO ,                                      //电子现金非指定账户充值
        TRADEID_QPBOC ,                                            //qPBOC
        TRADEID_ECOFFREFUND               ,		//电子现金退货
        TRADEID_UNECCASH           ,		//电子现金现金充值撤销
        TRADEID_ECTRADELOG,                 //电子现金读交易记录
        TRADEID_ECBALANCE,                  //电子现金读余额
        TRADEID_ECLOADLOG,                   //圈存日志
        //电子钱包
        TRADEID_EPSALE                          ,		//电子钱包消费
        TRADEID_EPQUERY                       ,		//电子钱包余额查询
        TRADEID_EPLOAD_CASH               ,		//电子钱包现金充值
        TRADEID_EPLOAD_ACC                ,		//电子钱包指定账户充值
        TRADEID_EPLOAD_NOACC             ,		//电子钱包非指定账户充值
        TRADEID_EPUNLOAD_CASH           ,		//电子钱包现金充值撤销
        TRADEID_EPCACC ,
        TRADEID_EPCASH ,
        TRADEID_EPCACCNO,
        TRADEID_EPOFFSALE ,
        //预约类
        TRADEID_RESERVATION_SALE ,                       //预约消费
        TRADEID_RESERVATION_UNSALE ,                 //预约消费撤销
        TRADEID_RESERVATION_REFUND,                 //预约消费脱货
        //积分类
        TRADEID_POINTSALE_ISSUER ,                       //发卡行积分消费
        TRADEID_POINTUNSALE_ISSUER ,                     //发卡行积分消费撤销
        TRADEID_POINTSALE_UNION ,                                //联盟积分消费
        TRADEID_POINTUNSALE_UNION ,                      //联盟积分消费撤销
        TRADEID_POINTQUERY_UNION ,                           //联盟积分查询
        TRADEID_POINTREFUND_UNION ,                      //联盟积分退货
        //订购类
        TRADEID_MOTOSALE ,                                       //订购消费
        TRADEID_MOTOUNSALE ,                                 //订购消费撤销
        TRADEID_MOTOREFUND ,                                     //订购退货
        TRADEID_MOTOPREAUTH ,                                //订购预授权
        TRADEID_MOTOUNPREAUTH ,                          //预授权撤销
        TRADEID_MOTOCOMPLETE ,                               //预授权完成请求
        TRADEID_MOTOOFFCOMPLETE ,                            //预授权完成通知
        TRADEID_MOTOUNCOMPLETE ,                         //预授权完成撤销
        TRADEID_MOTOVERIFY,                             //持卡人身份验证
        //手机芯片
        TRADEID_MOBILEREFUND ,                          //手机芯片退货
        TRADEID_MOBILEPREAUTH ,                             //手机芯片预授权
        TRADEID_MOBILEUNPREAUTH ,                       //手机芯片预授权撤销
        TRADEID_MOBILECOMPLETE ,                        //手机芯片预授权完成请求
        TRADEID_MOBILEOFFCOMPLETE   ,               //手机芯片预授权完成通知
        TRADEID_MOBILEUNCOMPLETE ,                      //手机芯片预授权完成撤销
        TRADEID_MOBILEQUERY      ,                              //手机芯片手机芯片查询
        TRADEID_MOBILESALE                  ,            //手机芯片消费
        TRADEID_MOBILEUNSALE             ,              //手机芯片消费撤销
        TRADEID_MAGLOAD_VERIFY                ,		//磁卡现金充值账户验证
        TRADEID_MAGLOAD_CASH               ,		//磁卡现金充值
        TRADEID_MAGLOAD_AFFIRM                ,		//磁卡现金充值确认
        TRADEID_MAGLOAD_ACC                ,		//磁卡账户充值
        TRADEID_INSTALLMENTSALE ,                               //分期消费
        TRADEID_INSTALLMENTUNSALE ,                             //分期撤销
        TRADEID_SETTLE ,                             //结算

        TRADEID_CAPK,
        TRADEID_BLACKLIST,
        TRADEID_AID,
        TRADEID_UPLOAD,
        //以下非真正意义上的交易类型
        TRADEID_ECMIXSALE,      //可挥卡插卡的电子现金，根据刷卡方式最后转化为TRADEID_QPBOC和TRADEID_ECOFFSALE

    }
}
