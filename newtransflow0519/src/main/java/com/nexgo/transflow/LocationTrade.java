package com.nexgo.transflow;

import com.nexgo.jniinterface.CallNative;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/28 08:48
 * Describe:
 */
public class LocationTrade extends BaseProcess {

    public LocationTrade(){
        super();
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
        int ret = AnalyzePackage(data,linkstate, errtip, len);//收到数据，调用解包
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            processListener.onFinish(TransResult.APPROVED,errtip, len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR,errtip, len[0]);
        }
    }

    @Override
    public void onSetCancel() {

    }
}
