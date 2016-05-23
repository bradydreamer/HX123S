package com.nexgo.transflow;

import android.app.Dialog;
import android.content.ContentValues;
import android.content.Context;
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
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.text.DecimalFormat;
import java.util.ArrayList;
import java.util.Arrays;

/**
 * Created by caibaqun on 2016/3/24.
 */
public class SaleVoidTrade extends BaseProcess {

    private TradeTlv tradeTlv = null;
    private ArrayList<ContentValues> mRecord = null;
    private int mIndex = 0;
    private Logger log = LoggerFactory.getLogger("SaleVoidTrade");

    public SaleVoidTrade(Context context) {
        this.context = context;
        tradeTlv = TradeTlv.getInstance();
    }

    public ArrayList<ContentValues> getSucessConsumeInfo(int index) {
        mRecord = tradeTlv.getSucessConsumeInfo(index);
        return mRecord;
    }

    public ContentValues getOneTradeRecord(String VoucherNo) {
        return tradeTlv.getOneTradeRecord(VoucherNo);
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
    protected void useic() {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        if (Arrays.equals(TradeId, context.getString(R.string.UnSaleID).getBytes())) {
            tradetip(context.getString(R.string.useic));
        }else {
            tradetip(context.getString(R.string.plsinsercard));
        }
        doSearchCard();
    }

    @Override
    protected void doShowPan() {
        log.debug("辙销显示卡号");
        String Cardno = new String(tradeTlv.getTagValue(TradeTlv.CardNo));
        if (!pan.equals(Cardno)) {
            log.debug("与原交易卡号不一致");
            tradetip(context.getString(R.string.errcardno));
            if (isContact) {
                try {
                    emvHandler.onSetConfirmCardNoResponse(false);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            } else {
                doCancel();
            }
            return;
        }
        DecimalFormat df = new DecimalFormat("0.00");
        String amountDecimal = df.format(Double.valueOf(new String(transAmt)) / 100);
        final Dialog dialog = new Dialog(context);
        dialog.setCanceledOnTouchOutside(false);//点击弹框外不退出
        dialog.setCancelable(false);//不响应返回键
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        LayoutInflater layoutInflater = LayoutInflater.from(context);
        View view = layoutInflater.inflate(ResContainer.getResourceId(context, ResContainer.ResType.LAYOUT, "activity_confirm_card_no"), null);
        dialog.setContentView(view);
        dialog.show();
        TextView amount = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "amount")));
        amount.setText(amountDecimal + context.getString(R.string.amtflag));
        TextView cardno = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cardno")));
        cardno.setText(pan);
        Button confirm = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "confirm")));
        Button cancel = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cancel")));
        confirm.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                dialog.dismiss();
                try {
                    if (isMagCard) {
                        subject.onNext(LocalEvent.PIN_INPUT);
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
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[]errtip=new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate,errtip,len);//收到数据，调用解包
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
    protected void mConfigEmvParam(){
        if(isContact){
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.SIMPLE);
            isSample = true;
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
        }else {
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x20);
        }

    }

}
