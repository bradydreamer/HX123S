package com.nexgo.db;

import com.nexgo.common.TradeTlv;

import java.lang.annotation.Target;

/**
 * Created by xuefeng on 2016/3/1.
 */
public class DbRecordTables {
        /*交易详情表*/
        public static String TradeDetailInfoTable = "CREATE TABLE IF NOT EXISTS TradeDetailInfo(" +
                "PID INTEGER PRIMARY KEY AUTOINCREMENT," +       //标识ID，自增
                //报文
                TradeTlv.SendMessage + " TEXT NOT NULL DEFAULT ''," +             //发送报文
                TradeTlv.RecvMessage + " TEXT NOT NULL DEFAULT ''," +             //接收报文
                TradeTlv.Filed55 + " TEXT NOT NULL DEFAULT ''," +                                //芯片卡数据55域（芯片卡必选）

                //历史库标志域
                TradeTlv.IsPrintOver + " TEXT NOT NULL DEFAULT '0'," +          //isPrint 打印完成标志
                TradeTlv.IsUnsale + " TEXT NOT NULL DEFAULT '0'," +          //已撤销标志（已撤销不清该交易流水,默认1未撤销）
                TradeTlv.IsUpLoad + " TEXT NOT NULL DEFAULT '0'," +          //已上送标志（批上送）

                //重要数据
                TradeTlv.MerchantName + " TEXT NOT NULL DEFAULT ''," +             //商户名称：	POS终端
                TradeTlv.MerchantNo + " TEXT NOT NULL DEFAULT ''," +             //商户编号：	42域
                TradeTlv.TerminalNo + " TEXT NOT NULL DEFAULT ''," +             //终端编号：	41域
                TradeTlv.AcquiringBank + " TEXT NOT NULL DEFAULT ''," +             //收单行标识码：	44域
                TradeTlv.IssuingBank + " TEXT NOT NULL DEFAULT ''," +             //发卡行标识码：	44域
                TradeTlv.CardNo + " TEXT NOT NULL DEFAULT ''," +             //卡号：	2域
                TradeTlv.Op + " TEXT NOT NULL DEFAULT ''," +             //操作员号：	POS终端
                TradeTlv.TradeId + " TEXT NOT NULL DEFAULT ''," +             //交易类型：sale、void、refund...：	0域
                TradeTlv.CardVailDate + " TEXT NOT NULL DEFAULT '' ," +                                //卡有效期（可选）：	14域
                TradeTlv.BatchNo + " TEXT NOT NULL DEFAULT ''," +             //批次号：	60域
                TradeTlv.VoucherNo + " TEXT NOT NULL DEFAULT ''," +           //凭证号、流水号：	11域
                TradeTlv.TraDate + " TEXT NOT NULL DEFAULT ''," +             //交易日期：	12域
                TradeTlv.TraTime + " TEXT NOT NULL DEFAULT ''," +             //交易时间：	13域
                TradeTlv.AuthCode + " TEXT NOT NULL DEFAULT '' ," +                                //授权码（可选）：	38域
                TradeTlv.ReferNo + " TEXT NOT NULL DEFAULT ''," +             //参考号：	37域
                TradeTlv.Amount + " TEXT NOT NULL DEFAULT ''," +            //交易金额：4域-48域
                TradeTlv.Tip + " TEXT NOT NULL DEFAULT ''," +            //小费(可选)：48域
                TradeTlv.MoneyCode + " TEXT NOT NULL DEFAULT ''," +             //货币代码：49域
                TradeTlv.AccountType + " TEXT NOT NULL DEFAULT ''," +             //账户类型：储蓄、支票、信用透支、通用、投资账户：域3
                TradeTlv.PayWay + " TEXT NOT NULL DEFAULT ''," +             //支付方式（SICMN）：底层设置
                TradeTlv.TradeTyFlag + " TEXT NOT NULL DEFAULT ''," +              //借贷记标志 0-借计，1-贷计：底层设置
                TradeTlv.InOutCardFlag + " TEXT NOT NULL DEFAULT ''," +              //内外卡标志 0-内卡，1-外卡：底层设置
                TradeTlv.CardType + " TEXT NOT NULL DEFAULT '',"+              //国际信用卡公司代码 MCC、CUP...：63.1域
                TradeTlv.OfflineRes + " TEXT NOT NULL DEFAULT '',"+             //脱机交易结果 00-脱机接受 01-脱机拒绝
                TradeTlv.cardholderPayAmount + " TEXT NOT NULL DEFAULT '',"+             //扣持卡人金额(可选)
                TradeTlv.InstallmentNum + " TEXT NOT NULL DEFAULT '',"+             //分期付款期数(可选)
                TradeTlv.firstPaymentAmount + " TEXT NOT NULL DEFAULT '',"+             //分期付款持卡人首期金额(可选)
                TradeTlv.repaymentCurrency + " TEXT NOT NULL DEFAULT '',"+             //分期付款持卡人还款币种(可选)
                TradeTlv.InstallmentCharge + " TEXT NOT NULL DEFAULT '',"+             //分期付款持卡人手续费(可选)
                TradeTlv.GoodsProjectCode + " TEXT NOT NULL DEFAULT '',"+             //商品代码(可选)
                TradeTlv.cashPointAmount + " TEXT NOT NULL DEFAULT '',"+             //兑换积分(可选)
                TradeTlv.pointBalance + " TEXT NOT NULL DEFAULT '',"+             //积分余额(可选)
                TradeTlv.pointPaidAmount + " TEXT NOT NULL DEFAULT '',"+             //自付金额(可选)
                TradeTlv.transferCardNo + " TEXT NOT NULL DEFAULT '')";             //转入卡卡号(可选)


        //POS中心标识码（可选）
        //总金额(可选)
        //卡片序列号（可选）


        /*冲正表*/
        public static String TradeReversalTable = "CREATE TABLE IF NOT EXISTS TradeReversalTable(" +
                TradeTlv.ReveralMes + " TEXT NOT NULL DEFAULT ''," +            //冲正报文
                TradeTlv.ReveralNeed + " TEXT NOT NULL DEFAULT '0')";           //isReversal冲正标志,默认不需冲正

        /*脚本上送结果表*/
        public static String ScriptDealResult = "CREATE TABLE IF NOT EXISTS ScriptDealResult(" +
                TradeTlv.ScriptSengMess + " TEXT NOT NULL DEFAULT ''," +          //执行脚本通知交易的原始发送报文
                TradeTlv.ScriptRecvMess + " TEXT NOT NULL DEFAULT ''," +            //执行脚本通知交易的原始接收报文
                TradeTlv.IsHaveScriptReslt + " TEXT NOT NULL DEFAULT '0')";      //脚本处理结果是否存在的标志

        public static String OtherInfoTable = "CREATE TABLE IF NOT EXISTS OtherInfoTable(" +
            /*结算批上送*/
                TradeTlv.upAdd + " TEXT NOT NULL DEFAULT 'FFFF'," +          //批上送断点(已经上送到第几笔)
                TradeTlv.uploadMode + "  NOT NULL DEFAULT ''," +          //批上送模式
                TradeTlv.TotalUpNo  + " TEXT NOT NULL DEFAULT '0')";      //已经上送总笔数
}





