package com.nexgo.transflow;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.OperationCanceledException;
import android.os.RemoteException;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;

import com.nexgo.R;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.SecAuthResult;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.pinpad.PinAlgorithmMode;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvProcessResult;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

import de.greenrobot.event.EventBus;

/**
 * Created by caibaqun on 2016/3/30.
 */
public class RefundTrade extends BaseProcess {
    private Logger log = LoggerFactory.getLogger("RefundTrade");

    public RefundTrade(Context context) {
        this.context = context;
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        setIfmanual();//设置是否手输卡号
        startTrade();
        processListener.onStart();


    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        startonline();

    }

    @Override
    protected void doShowPan() {

        log.debug("显示卡号");
        tradeTlv.setTagValue(TradeTlv.CardNo, pan.getBytes());
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCanceledOnTouchOutside(false);//点击弹框外不退出
        dialog.setCancelable(false);//不响应返回键
        LayoutInflater layoutInflater = LayoutInflater.from(context);
        View view = layoutInflater.inflate(ResContainer.getResourceId(context, ResContainer.ResType.LAYOUT, "activity_confirm_card_query"), null);
        TextView cardno = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cardno")));
        cardno.setText(pan);
        Button confirm = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "confirm")));
        Button cancel = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cancel")));

        dialog.setContentView(view);
        dialog.show();
        confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
                try {

                    if (isMagCard|| IsInputCardno) {
                       // subject.onNext(LocalEvent.PIN_INPUT);
                        processListener.onFinish(TransResult.APPROVED, null, 0);
                        subject.onCompleted();

                    } else {
                        emvHandler.onSetConfirmCardNoResponse(true);
                    }
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        });
        cancel.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
                if (isContact) {
                    try {
                        emvHandler.onSetConfirmCardNoResponse(false);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                } else {
                    doCancel();
                }
            }
        });
    }
    @Override
    protected void useic() {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        if (Arrays.equals(TradeId, context.getString(R.string.RefundID).getBytes())) {
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
            if (Arrays.equals(TradeId, context.getString(R.string.RefundID).getBytes())) {
                rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
            }//积分没有非接
            magCardReader.searchCard(magCardListener, 60);
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[] errtip = new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
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
        CancelSearchcard();
    }

    protected void doPinInput() {
        //退货不用输密码，直接联机
        subject.onNext(LocalEvent.ONLINE_REQUEST);
    }

    @Override
    protected void mConfigEmvParam() {
        if (isContact) {
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);
            isSample = true;
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
        } else {
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.FULL);
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
        }
    }
    protected void mFinish(int retCode, Bundle data) throws RemoteException {
        log.debug("onFinish");
        switch (retCode) {
            case ServiceResult.Emv_Qpboc_Online:
            case ServiceResult.Success:
            case ServiceResult.Emv_Offline_Accept:
                log.debug("qpboc联机");
                processListener.onFinish(TransResult.APPROVED, null, 0);
                subject.onCompleted();
                break;
            case ServiceResult.Emv_Cancel:
                subject.onError(new OperationCanceledException("交易取消"));
                subject.onCompleted();
                break;
            default:
                log.debug("交易失败");
                subject.onError(new Exception("交易失败"));
                subject.onCompleted();
                break;
        }
    }
}
