package com.nexgo.transflow;

import android.content.ContentValues;
import android.content.Context;
import android.os.OperationCanceledException;

import com.nexgo.R;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

/**
 * Created by caibaqun on 2016/4/7.
 */
public class PreauthTrade extends BaseProcess {
    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("PreauthTrade");

    public PreauthTrade(Context context) {
        this.context = context;
        tradeTlv = TradeTlv.getInstance();
    }


    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        setIfmanual();
        processListener.onStart();
        init();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[]errtip=new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate,errtip,len);//收到数据，调用解包
        log.debug("预授权返回 ret={}", ret);
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            CallNative.formPrintScript(CallNative.SIGN_BILL);
            processListener.onFinish(TransResult.APPROVED,errtip,len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR,errtip,len[0]);
        }
    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }
    @Override
    protected void mConfigEmvParam()
    {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        if (isContact){
            if (Arrays.equals(TradeId, context.getString(R.string.Preauth).getBytes())) {//预授权
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x03);
            } else if (Arrays.equals(TradeId, context.getString(R.string.Complete).getBytes())) {//预授权完成
                emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
                isSample = true;
            } else if (Arrays.equals(TradeId, context.getString(R.string.Unpreauth).getBytes())) {//预授权辙销
                emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
                isSample = true;
            } else if (Arrays.equals(TradeId, context.getString(R.string.Uncomplete).getBytes())) {//预授权完成辙销
                emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
                isSample = true;
            }
        }else{
            if (Arrays.equals(TradeId, context.getString(R.string.Preauth).getBytes())) {//预授权
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x03);
            } else if (Arrays.equals(TradeId, context.getString(R.string.Complete).getBytes())) {//预授权完成
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
            } else if (Arrays.equals(TradeId, context.getString(R.string.Unpreauth).getBytes())) {//预授权辙销
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
            } else if (Arrays.equals(TradeId, context.getString(R.string.Uncomplete).getBytes())) {//预授权完成辙销
                emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
            }
        }

    }
}
