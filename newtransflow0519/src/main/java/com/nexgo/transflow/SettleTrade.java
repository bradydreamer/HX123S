package com.nexgo.transflow;

import android.content.Context;
import android.os.OperationCanceledException;
import android.util.Log;

import com.nexgo.R;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.HashMap;

import rx.subjects.BehaviorSubject;

/**
 * Created by caibaqun on 2016/3/18.
 */
public class SettleTrade extends BaseProcess {
    private TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("SettleTrade");
    protected BehaviorSubject<LocalEvent> subject;
    private Context context;

    public SettleTrade(Context context) {
        tradeTlv = TradeTlv.getInstance();
        this.context = context;
    }


    protected void doError() {
        if (processListener != null) {
            processListener.onFinish(TransResult.DEVICE_ERR,null,0);
        }
    }

    protected void doCancel() {
        if (processListener != null) {
            processListener.onFinish(TransResult.CANCEL,null,0);
        }
    }

    private int getSettleList() {
        int increditnum = 0;//内卡贷记笔数
        int indebitnum = 0;//内卡贷记笔数
        int oucreditnum = 0;//外卡贷记笔数
        int oudebitnum = 0;//外卡贷记笔数
        long increditamti = 0;//内卡贷记金额
        long indebitamti = 0;//内卡贷记金额
        long oucreditamti = 0;//外卡贷记金额
        long oudebitamti = 0;//外卡贷记金额
        HashMap map;
        map = tradeTlv.settleResult(TradeTlv.SettleType.internalCardCredit);
        if (map != null) {
            increditnum = (int) map.get("Count");
            increditamti = (long) map.get("Amount");
        }
        map = tradeTlv.settleResult(TradeTlv.SettleType.internalCardDebit);
        if (map != null) {
            indebitnum = (int) map.get("Count");
            indebitamti = (long) map.get("Amount");
        }
        map = tradeTlv.settleResult(TradeTlv.SettleType.foreignCardCredit);
        if (map != null) {
            oucreditnum = (int) map.get("Count");
            oucreditamti = (long) map.get("Amount");
        }
        map = tradeTlv.settleResult(TradeTlv.SettleType.foreignCardDebit);
        if (map != null) {
            oudebitnum = (int) map.get("Count");
            oudebitamti = (long) map.get("Amount");
        }
        String increditamtstr = null;
        String oucreditamtstr = null;
        String indebitamtstr = null;
        String oudebitamtstr = null;

        increditamtstr = String.format("%012d", increditamti);
        oucreditamtstr = String.format("%012d", oucreditamti);
        indebitamtstr = String.format("%012d", indebitamti);
        oudebitamtstr = String.format("%012d", oudebitamti);

        Log.e("settle", "" + increditnum);
        Log.e("settle", "" + indebitnum);
        Log.e("settle", "" + oucreditnum);
        Log.e("settle", "" + oudebitnum);
        Log.e("settle", "" + increditamtstr);
        Log.e("settle", "" + oucreditamtstr);
        Log.e("settle", "" + indebitamtstr);
        Log.e("settle", "" + oudebitamtstr);
        tradeTlv.setTagValue(TradeTlv.InCreCardAllNo, String.valueOf(increditnum).getBytes());
        tradeTlv.setTagValue(TradeTlv.InDebCardAllNo, String.valueOf(indebitnum).getBytes());
        tradeTlv.setTagValue(TradeTlv.OutCreCardAllNo, String.valueOf(oucreditnum).getBytes());
        tradeTlv.setTagValue(TradeTlv.OutDebCardAllNo, String.valueOf(oudebitnum).getBytes());
        tradeTlv.setTagValue(TradeTlv.InCreCardAllAmount, ByteUtils.hexString2ByteArray(increditamtstr));
        tradeTlv.setTagValue(TradeTlv.OutCreCardAllAmount, ByteUtils.hexString2ByteArray(oucreditamtstr));
        tradeTlv.setTagValue(TradeTlv.InDebCardAllAmount, ByteUtils.hexString2ByteArray(indebitamtstr));
        tradeTlv.setTagValue(TradeTlv.OutDebCardAllAmount, ByteUtils.hexString2ByteArray(oudebitamtstr));
        return increditnum + indebitnum + oucreditnum + oudebitnum;
    }

    public boolean IfHasRecord() {
        if (tradeTlv.getrecordNums() == 0) {
            return false;
        } else {
            int recnum = getSettleList();
            if (recnum == 0) {
                return false;
            }
            return true;
        }
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        processListener.onStart();
        PackageAndSend();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[]errtip=new byte[256];
        byte[] len = new byte[2];
        tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.SettleID).getBytes());//设置交易ID
        int ret = AnalyzePackage(data, linkstate,errtip,len);//收到数据，调用解包
        if (ret == CallNative.NOT_END) {//
            tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.SettleID).getBytes());//设置交易ID
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            getSettleList();
            CallNative.formPrintScript(CallNative.COUNT_BILL);
            tradeTlv.deleteAllRecordForC();//
            processListener.onFinish(TransResult.APPROVED,errtip,len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR,errtip,len[0]);
        }
    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }
}
