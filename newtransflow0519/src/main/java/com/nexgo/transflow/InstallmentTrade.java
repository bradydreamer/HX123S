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
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.text.DecimalFormat;

/**
 * Created by mai on 16/5/12.
 */
public class InstallmentTrade extends BaseProcess {

    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("InstallmentTrade");

    public InstallmentTrade(Context context) {
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
    protected void doShowPan() {
        log.debug("显示卡号");
        tradeTlv.setTagValue(TradeTlv.CardNo, pan.getBytes());
        DecimalFormat df = new DecimalFormat("0.00");
        String amountDecimal = df.format(Double.valueOf(new String(transAmt)) / 100);
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCanceledOnTouchOutside(false);//点击弹框外不退出
        dialog.setCancelable(false);
        LayoutInflater layoutInflater = LayoutInflater.from(context);
        View view = layoutInflater.inflate(ResContainer.getResourceId(context, ResContainer.ResType.LAYOUT, "activity_confirm_card_no"), null);
        dialog.setContentView(view);
        dialog.show();
        TextView amount = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "amount")));
        if (amount != null) {
            amount.setText(amountDecimal + context.getString(R.string.amtflag));
        }
        TextView cardno = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cardno")));
        if (cardno != null) {
            cardno.setText(pan);
        }
        Button confirm = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "confirm")));
        if (confirm != null) {
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
        }
        Button cancel = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cancel")));
        if (cancel != null) {
            cancel.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    dialog.dismiss();
                    if (isMagCard || IsInputCardno) {
                        doCancel();
                    } else {
                        try {
                            emvHandler.onSetConfirmCardNoResponse(false);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
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
            //rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    @Override
    protected void mConfigEmvParam()
    {
        emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
        emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
        emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);

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
