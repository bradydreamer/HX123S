package com.nexgo.transflow;

import android.content.Context;
import android.os.Bundle;
import android.os.OperationCanceledException;
import android.os.RemoteException;

import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by mai on 16/5/12.
 */
public class InstallmentVoidTrade extends BaseProcess {

    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("InstallmentVoidTrade");

    public InstallmentVoidTrade(Context context) {
        this.context = context;
        tradeTlv = TradeTlv.getInstance();
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        startonline();
    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[] errtip = new byte[1024];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
        log.debug("分期付款返回 ret={}", ret);
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            CallNative.formPrintScript(CallNative.SIGN_BILL);
            processListener.onFinish(TransResult.APPROVED, errtip, len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR, errtip, len[0]);
        }
    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }

    @Override
    protected void doSearchCard() {
        try {
            magCardReader.searchCard(magCardListener, 60);
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
            rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    @Override
    protected void mConfigEmvParam()
    {
        emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
        emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
        emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.FULL);
    }

}
