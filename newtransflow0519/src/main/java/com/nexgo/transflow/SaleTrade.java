package com.nexgo.transflow;

import android.content.Context;
import android.os.Bundle;
import android.os.OperationCanceledException;
import android.os.RemoteException;

import com.nexgo.R;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.device.reader.icc.OnSearchIccCardListener;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

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
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[] errtip = new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
        log.debug("消费返回 ret={}", ret);
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
    protected void useic() {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        if (Arrays.equals(TradeId, context.getString(R.string.SaleID).getBytes())) {
            tradetip(context.getString(R.string.useic));
        }else {
            tradetip(context.getString(R.string.plsinsercard));
        }
        doSearchCard();
    }
    @Override
    protected void doSearchCard() {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        try {
            if (Arrays.equals(TradeId, context.getString(R.string.SaleID).getBytes())) {
                rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
            }//积分消费没有非接
            magCardReader.searchCard(magCardListener, 60);
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    OnSearchIccCardListener rfCardListener = new OnSearchIccCardListener.Stub() {
        @Override
        public void onSearchResult(int retCode, Bundle bundle) throws RemoteException {
            if (retCode == ServiceResult.Success) {
                log.debug("寻到非接ic");
                tradeTlv.setTagValue(TradeTlv.PayWay, new byte[]{'0', '2'});
                byte[] rfchannel = tradeTlv.getTagValue(TradeTlv.Rfchannel);
                if (rfchannel != null) {
                    if (rfchannel[0] == 0x30) {
                        tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.EC_SaleID).getBytes());//
                    }
                }

                subject.onNext(LocalEvent.START_EMV);
            } else {
                subject.onError(new Exception("RF卡寻卡出错"));
            }
        }
    };

    @Override
    protected void mConfigEmvParam() {
        emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
        emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
    }

}
