package com.nexgo.transflow;

import android.app.Dialog;
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
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

/**
 * Created by xiaox on 16/3/12.
 */
public class QueryTrade extends BaseProcess {

    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("QueryTrade");

    public QueryTrade(Context context) {
        QueryTrade.context = context;
        tradeTlv = TradeTlv.getInstance();
    }


    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        processListener.onStart();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    protected void doSearchCard() {
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        try {
            if(Arrays.equals(TradeId,context.getString(R.string.QueryID).getBytes())){
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
        log.debug("查余返回 ret={}", ret);
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
        //TextView amount = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "amount")));
        // amount.setText(amountDecimal + context.getString(R.string.amtflag));

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
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }
    @Override
    protected void mConfigEmvParam()
    {
        emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x31);
    }

}
