package com.nexgo.transflow;

import android.content.Context;
import android.os.OperationCanceledException;

import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by xiaox on 16/3/12.
 */
public class SaleTrade extends BaseProcess {

    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("SaleTrade");
    public SaleTrade(Context context) {
        this.context = context;
        tradeTlv = TradeTlv.getInstance();
    }


       @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;

        processListener.onStart();
        init();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data,int linkstate) {
        byte[] errtip = new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data,linkstate, errtip, len);//收到数据，调用解包
        log.debug("消费返回 ret={}",ret);
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            CallNative.formPrintScript(CallNative.SIGN_BILL);
            processListener.onFinish(TransResult.APPROVED,errtip, len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR,errtip, len[0]);
        }
    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }

}
