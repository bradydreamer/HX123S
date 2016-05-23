package com.nexgo.transflow;

import android.content.Context;

import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;

/**
 * Created by caibaqun on 2016/5/6.
 * 上送交易
 */
public class UploadTrade extends BaseProcess {
    public UploadTrade(Context context) {
        this.context = context;
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        processListener.onStart();
        PackageAndSend();//组包并回调
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[] errtip = new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            processListener.onFinish(TransResult.APPROVED, errtip, len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR, errtip, len[0]);
        }
    }

    @Override
    public void onSetCancel() {

    }

}
