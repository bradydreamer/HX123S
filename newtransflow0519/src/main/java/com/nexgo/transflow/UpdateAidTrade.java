package com.nexgo.transflow;


import android.content.Context;
import android.os.OperationCanceledException;

import com.nexgo.R;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import rx.subjects.BehaviorSubject;

public class UpdateAidTrade extends BaseProcess {
    private TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("UpdateAidTrade");
    protected BehaviorSubject<LocalEvent> subject;
    private Context context;

    public UpdateAidTrade(Context context) {
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
       return 0;
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
    public void ProcessContinue(IProcessListener processListener){
        this.processListener = processListener;
        init();
        startonline();
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        processListener.onStart();
        PackageAndSend();
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

