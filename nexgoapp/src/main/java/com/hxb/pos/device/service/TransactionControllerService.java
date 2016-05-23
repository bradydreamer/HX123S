package com.hxb.pos.device.service;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import com.hxb.pos.device.activity.LoadTransportkeyActivity;
import com.hxb.pos.device.activity.LocationActivity;
import com.hxb.pos.device.activity.SaleActivity;
import com.hxb.pos.device.activity.SigninActivity;
import com.hxb.pos.device.activity.LoadPrimaryKeyActivity;
import com.hxb.pos.device.utils.AppGlobal;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;

import java.util.Map;

public class TransactionControllerService extends Service {
    String TAG = "TransactionControllerService";
    private Context context;
    private Intent intent ;
    private TradeTlv tradeTlv;
    private TransactionControllerAidl.Stub stub = new TransactionControllerAidl.Stub() {
        /**
         * 功能：通用交易函数。
         参数说明：
         * @param transactionName 交易名称
         * @param para 各交易需要的参数。
         *             para参数里必须传入key为packageName的参数，其参数值为华夏SDK所在App的包名。
         *             还必须传入经纬度参数其中jingdu:经度  weidu:维度
         * @param transactionResult 交易结果的回调接口
         */
        @Override
        public void common(String transactionName, Map para, TransactionResult transactionResult) throws RemoteException {
            AppGlobal.getInstance().setTransactionResult(transactionResult);
            switch (transactionName){
                case "终端参数下载":
                    Log.d(TAG, "终端参数下载");
                    initparam(para);
                    break;
                case "初始定位":
                    Log.d(TAG,"初始定位");
                    tradeTlv.setTagValue(TradeTlv.jingdu, para.get("jingdu").toString().getBytes());
                    tradeTlv.setTagValue(TradeTlv.weidu, para.get("weidu").toString().getBytes());
                    tradeTlv.setTagValue(TradeTlv.Op, para.get("Op").toString().getBytes());
                    Log.d("hgd", "jingdu" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.jingdu)));
                    Log.d("hgd", "weidu" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.weidu)));
                    intent = new Intent(context, LocationActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                    startActivity(intent);
                    break;
                case "KLK下载":
                    Log.d(TAG, "装载传输密钥");
                    tradeTlv.setTagValue(TradeTlv.Op, para.get("Op").toString().getBytes());
                    intent = new Intent(context, LoadTransportkeyActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                    startActivity(intent);
                    break;
                case "KEK下载":
                    Log.d(TAG, "下载主密钥密文");
                    //发报文，收报文，解报文62域获取主密钥密文，用KLK解密将明文写入密码键盘
//                    LoadPrimaryKeyActivity loadPrimaryKey = new LoadPrimaryKeyActivity();
//                    loadPrimaryKey.startLoad();
                    tradeTlv.setTagValue(TradeTlv.Op, para.get("Op").toString().getBytes());
                    intent = new Intent(context, LoadPrimaryKeyActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                    startActivity(intent);
                    break;
                case "签到":
                    Log.d(TAG, "签到");
                    tradeTlv.setTagValue(TradeTlv.Op, para.get("Op").toString().getBytes());
                    intent = new Intent(context, SigninActivity.class);
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                    startActivity(intent);
                    break;
                case "消费":
                    Log.d(TAG, "消费");
                    tradeTlv.setTagValue(TradeTlv.Op, para.get("Op").toString().getBytes());
                    intent = new Intent(context, SaleActivity.class);
                    intent.putExtra("Amount",para.get("amount").toString());
                    intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK | Intent.FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
                    startActivity(intent);
                    break;
                default:
                    break;
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        context = this;
        AppGlobal.getInstance().setmContext(this);
        tradeTlv = TradeTlv.getInstance();
        return stub;
    }

    private void initparam(Map map) {
//        SharedPreferences sp = NexgoApp.getInstance().getTransFlow().getSharedPreferences();
//        //// FIXME: 2016/3/16 测试参数用
//        sp.edit().putString(TradeTlv.Op, map.get("Op").toString())
//                .putString(TradeTlv.TerminalNo, map.get("TerminalNo").toString())
//                .putString(TradeTlv.MerchantNo, map.get("MerchantNo").toString())
//                .putString(TradeTlv.MerchantName, map.get("MerchantName").toString())
//                .putString(TradeTlv.MerchantEngName, map.get("MerchantEngName").toString())
//                .putString(TradeTlv.VoucherNo, map.get("VoucherNo").toString())
//                .putString(TradeTlv.BatchNo, map.get("BatchNo").toString())
//                .putString(TradeTlv.Tmkindex, map.get("Tmkindex").toString())
//                .putString(TradeTlv.TakEncry, map.get("TakEncry").toString())
//                .putString(TradeTlv.UnsaleNeedPwd, map.get("UnsaleNeedPwd").toString())
//                .putString(TradeTlv.UnsaleNeedSwipeCard, map.get("UnsaleNeedSwipeCard").toString())
//                .putString(TradeTlv.EncryType, map.get("EncryType").toString())
//                .putString(TradeTlv.TelOutsideNO, map.get("TelOutsideNO").toString())
//                .putString(TradeTlv.AutoSignOut, map.get("AutoSignOut").toString())
//                .putString(TradeTlv.IsPrivatenet, map.get("IsPrivatenet").toString())
//                .commit();
        tradeTlv.setTagValue(TradeTlv.Op, map.get("Op").toString().getBytes());
//        tradeTlv.setTagValue(TradeTlv.ReveralTime, map.get("ReveralTime").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.MACType, map.get("MACType").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.TerminalNo, map.get("TerminalNo").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.MerchantNo, map.get("MerchantNo").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.MerchantName, map.get("MerchantName").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.MerchantEngName, map.get("MerchantEngName").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.VoucherNo, map.get("VoucherNo").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.ReferNo, map.get("ReferNo").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.TPDU, map.get("TPDU").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.MoneyCode, map.get("MoneyCode").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.BatchNo, map.get("BatchNo").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.Tmkindex, map.get("Tmkindex").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.Klkindex, map.get("Klkindex").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.TakEncry, map.get("TakEncry").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.UnsaleNeedPwd, map.get("UnsaleNeedPwd").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.UnsaleNeedSwipeCard, map.get("UnsaleNeedSwipeCard").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.EncryType, map.get("EncryType").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.TelOutsideNO, map.get("TelOutsideNO").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.AutoSignOut, map.get("AutoSignOut").toString().getBytes());
        tradeTlv.setTagValue(TradeTlv.IsPrivatenet, map.get("IsPrivatenet").toString().getBytes());
        Log.d("hgd", "TerminalNo" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.TerminalNo)));
        Log.d("hgd", "MerchantNo" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.MerchantNo)));
        Log.d("hgd", "Tmkindex" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.Tmkindex)));
        Log.d("hgd", "Klkindex" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.Klkindex)));
        Log.d("hgd", "MerchantName" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.MerchantName)));
        Log.d("hgd", "BatchNo" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.BatchNo)));
        Log.d("hgd", "VoucherNo" + ByteUtils.asciiByteArray2String(tradeTlv.getTagValue(TradeTlv.VoucherNo)));

        Intent intent = new Intent();
        intent.putExtra("tradeCode", "0");//成功
        try {
            AppGlobal.getInstance().getTransactionResult().onResult(intent);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }
}
