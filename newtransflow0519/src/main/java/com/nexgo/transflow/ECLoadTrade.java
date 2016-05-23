package com.nexgo.transflow;

import android.content.Context;
import android.os.OperationCanceledException;
import android.os.RemoteException;

import com.nexgo.R;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

/**
 * Created by Mai on 2016/5/9.
 */
public class ECLoadTrade  extends BaseProcess {
    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("ECLoadTrade");

    public ECLoadTrade(Context context) {
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
        byte[] errtip = new byte[1024];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
        log.debug("圈存返回 ret={}", ret);
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
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
            rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    @Override
    protected void mConfigEmvParam(){

        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);

        log.debug("Trade ID {}", tradeTlv.TradeId);
        if (Arrays.equals(TradeId, context.getString(R.string.EC_CashID).getBytes())){
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x63);                         //电子现金充值
        }else if(Arrays.equals(TradeId, context.getString(R.string.EC_CaccID).getBytes())){
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x60);                         //电子现金指定账户圈存
        }else if(Arrays.equals(TradeId, context.getString(R.string.EC_CaccnoID).getBytes())){
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x62);                         //电子现金非指定账户圈存
        }else{
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x60); //默认指令账户圈存
        }

        emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
        emvBundle.putBoolean(EmvTransDataConstrants.ISSUPPORTEC, true);
        //Fixme 全流程要修改成圈存流程，现在服务层早不支持后面要修改
        if(isContact)
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.FULL);
        else
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.QPASS);

    }
}
