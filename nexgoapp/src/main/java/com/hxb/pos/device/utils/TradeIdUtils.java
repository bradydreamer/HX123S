package com.hxb.pos.device.utils;

import android.content.Context;

import com.hxb.pos.device.R;

/**
 * 作者:liuting on 2016/3/26 11:34
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.com.hxbsdk.utils
 * TODO:   交易Id的一些功能类
 */
public class TradeIdUtils {
    /**
     * @param tradeId
     * @return 返回交易id的中文字段
     */
    public static String getTradeIdName(Context context, String tradeId) {
        if (tradeId.equals("trade_id_signin")) {
            return context.getString(R.string.signin);
        } else if (tradeId.equals("trade_id_sale")) {
            return context.getString(R.string.sale);
        } else if (tradeId.equals("trade_id_query")) {
            return context.getString(R.string.query);
        } else if (tradeId.equals("trade_id_settle")) {
            return context.getString(R.string.settle);
        } else if (tradeId.equals("trade_id_unsale")) {
            return context.getString(R.string.consume_cancel);
        } else if (tradeId.equals("trade_id_refund")) {
            return context.getString(R.string.refund);
        } else if (tradeId.equals("trade_id_Reversal")) {
            return context.getString(R.string.reversal);
        } else if (tradeId.equals("trade_id_ICParamID")) {
            return context.getString(R.string.ic_params_download);
        } else if (tradeId.equals("trade_id_preauth")) {
            return context.getString(R.string.preauth);
        } else if (tradeId.equals("trade_id_unpreauth")) {
            return context.getString(R.string.preauth_cancel);
        } else if (tradeId.equals("trade_id_complete")) {
            return context.getString(R.string.preauth_finish_request);
        } else if (tradeId.equals("trade_id_uncomplete")) {
            return context.getString(R.string.preauthed_cancel);
        } else if (tradeId.equals("trade_id_Offcomplete")) {
            return context.getString(R.string.preauthed_cancel_notice);
        } else
            return null;
    }
}
