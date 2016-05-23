package com.hxb.pos.device.activity;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.os.RemoteException;
import android.provider.Settings;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.LinearLayout;
import android.widget.Toast;

import com.hxb.pos.device.NexgoApp;
import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.AppGlobal;
import com.hxb.pos.device.utils.ExchangeBag;
import com.hxb.pos.device.utils.StatusCode;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;
import com.nexgo.transflow.IProcess;
import com.nexgo.transflow.IProcessListener;
import com.nexgo.transflow.TransResult;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.net.ConnectException;
import java.net.SocketTimeoutException;
import java.util.Arrays;
import java.util.concurrent.TimeoutException;

import rx.functions.Action1;

/**
 * Created by caibaqun on 2016/3/15.
 */
public class ReadCardActivity extends BaseFunctionActivity {
    private IProcess tradeFlow = null;
    private final static String TAG = "ReadCardActivity";
    private LinearLayout ll;
    private TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger(this.getClass().getName());

    @Override
    protected View initView() {
        //FIXME 2016/03/28 无法生效，是否聚焦问题？
        //屏蔽下拉
        Settings.System.putInt(getContentResolver(), "status_bar_disabled", 1);
        Window win = getWindow();
        //屏蔽home键
        win.addFlags(3);
        //屏蔽菜单键
        win.addFlags(5);
        View view = LayoutInflater.from(this).inflate(R.layout.activity_read_card, null);
        ll = (LinearLayout) view.findViewById(R.id.test);
        init();
        ll.setBackgroundResource(R.drawable.waiting_bg);
        tradeFlow.ProcessStart(iProcessListener);
        return view;
    }


    @Override
    protected String setActivityTitle() {
        return getString(R.string.readcard);
    }

    public void doBeforeFinish() {
        tradeFlow.onSetCancel();
        setResult(-1, null);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            tradeFlow.onSetCancel();
            setResult(-1, null);
            System.out.println("onKeyDown");
            finish();
            //   return true;
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    public void init() {
        tradeTlv = TradeTlv.getInstance();
        byte[] TradeId = tradeTlv.getTagValue(TradeTlv.TradeId);
        if (Arrays.equals(TradeId, getString(R.string.SaleID).getBytes())) {//消费
            tradeFlow = NexgoApp.getInstance().getTransFlow().getSaleTradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.QueryID).getBytes())) {//查余
            tradeFlow = NexgoApp.getInstance().getTransFlow().getQueryTradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.UnSaleID).getBytes())) {//消费撤销
            tradeFlow = NexgoApp.getInstance().getTransFlow().getSaleVoidTradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.RefundID).getBytes())) {//退货
            tradeFlow = NexgoApp.getInstance().getTransFlow().getRefundTradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.Preauth).getBytes())) {//预授权
            tradeFlow = NexgoApp.getInstance().getTransFlow().getPreauthradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.Unpreauth).getBytes())) {//预授权辙销
            tradeFlow = NexgoApp.getInstance().getTransFlow().getPreauthradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.Complete).getBytes())) {//预授权完成
            tradeFlow = NexgoApp.getInstance().getTransFlow().getPreauthradeHandler(this);
        } else if (Arrays.equals(TradeId, getString(R.string.Uncomplete).getBytes())) {//预授权完成辙销
            tradeFlow = NexgoApp.getInstance().getTransFlow().getPreauthradeHandler(this);
        }
        else{
            log.debug("未定义的消费类型");
        }

    }

    public void TurnToConnectView() {
        ll.setBackgroundColor(Color.parseColor("#303F9F"));
    }

    //链路状态  0x00 OK,0x01:timeout  0x02 :interrut ,0x03 connect err
    protected IProcessListener iProcessListener = new IProcessListener() {
        @Override
        public void onStart() {

        }

        @Override
        public void onOnlineRequest(final byte[] data) {
            new Thread(){
                @Override
                public void run() {
//                    boolean flag = true;
                    int len = 0;
                    Log.d(TAG, "发给华夏sdk 数据 = " + ByteUtils.byteArray2HexStringWithSpace(data));
                    byte[] buffer = new byte[2048];
                    try {
                        buffer = AppGlobal.getInstance().getDataExchangeAidl().exchangeData(data);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    if(buffer != null) {
                        len = ByteUtils.byte2int(buffer[0], buffer[1]);
                        if (len > 0) {
                            Log.d(TAG, "接收报文的长度 = " + len);
                            Log.d(TAG, "接收到华夏sdk 数据 = " + ByteUtils.byteArray2HexStringWithSpace(Arrays.copyOfRange(buffer, 0, len + 2)));
                            tradeFlow.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
                        }
                    }else {
                        onFinish(TransResult.ERROR,null,0);
                    }
//                    while (flag){
//                        len = ByteUtils.byte2int(buffer[0], buffer[1]);
//                        Log.d(TAG,".....");
//                        if (len > 0) {
//                            flag = false;
//                            Log.d(TAG, "接收报文的长度 = " + len);
//                            Log.d(TAG, "接收到华夏sdk 数据 = " + Arrays.toString(Arrays.copyOfRange(buffer, 0, len + 2)));
//                            signInTrade.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
//                        }
//                    }
                }
            }.start();
        }


        @Override
        public void onFinish(TransResult result, byte[] errtip, int len) {

            if (result == TransResult.DEVICE_ERR || result == TransResult.CANCEL) {
                setResult(StatusCode.CANCEL, null);
                finish();
            } else if (result == TransResult.APPROVED) {
                setResult(StatusCode.SUCCEED, null);
                finish();
            } else {
                if (errtip != null && len != 0) {
                    Intent data = new Intent();
                    Bundle bundle = new Bundle();
                    bundle.putString("errtip", new String(errtip));
                    data.putExtras(bundle);
                    setResult(StatusCode.FAIL, data);
                } else {
                    setResult(StatusCode.FAIL, null);
                }
                finish();
            }
        }
    };

}
