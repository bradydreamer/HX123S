package com.nexgo.transflow;

import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;

/**
 * Created by xiaox on 16/3/12.
 */
public class SignInTrade extends BaseProcess {

    public SignInTrade() {
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
        byte[]errtip=new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate,errtip,len);//收到数据，调用解包
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            tradeTlv.setTagValue(TradeTlv.LastSingIn, date);//设置签到日期
            tradeTlv.setTagValue(TradeTlv.SignState, new byte[]{0x31});//设置签到状态
            //XgdUtils.testDate();//修改系统时间
            processListener.onFinish(TransResult.APPROVED,errtip,len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR,errtip,len[0]);
        }
    }

    @Override
    public void onSetCancel() {

    }

    public boolean isNeedLogin() {
        byte OldDate[] = tradeTlv.getTagValue(TradeTlv.LastSingIn);
        if (OldDate != null && date != null) {
            int iCurdate = Integer.valueOf(new String(date));
            int iOldedate = Integer.valueOf(new String(OldDate));
            if (iCurdate - iOldedate <= 0) {
                return false;
            }
        }
        return true;
    }


}
