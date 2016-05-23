package com.hxb.pos.device.activity;

import android.content.Intent;
import android.os.RemoteException;
import android.text.TextUtils;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.Toast;

import com.hxb.pos.device.NexgoApp;
import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.AppGlobal;
import com.hxb.pos.device.utils.DataExchangeControl;
import com.hxb.pos.device.utils.ExchangeBag;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;
import com.nexgo.jniinterface.DeviceApusic;
import com.nexgo.transflow.IProcessListener;
import com.nexgo.transflow.SignInTrade;
import com.nexgo.transflow.TransResult;

import java.util.Arrays;
import java.util.Objects;

import rx.functions.Action1;

public class SigninActivity extends BaseFunctionActivity {

    private final static String TAG = "SIGNIN";
    private SignInTrade signInTrade = null;
    private TradeTlv tradeTlv = null;

    @Override
    protected View initView() {
        View view = LayoutInflater.from(this).inflate(R.layout.activity_signin, null);
        init();
        return view;
    }

    @Override
    protected String setActivityTitle() {
        return getString(R.string.signin);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK && event.getRepeatCount() == 0) {
            FinishResult();
        }
        return super.onKeyDown(keyCode, event);
    }

    @Override
    public void doBeforeFinish() {
        setResult(-1);
        finish();
    }

    private void FinishResult() {
        setResult(-1);
        finish();
    }

    public void init() {
        signInTrade = NexgoApp.getInstance().getTransFlow().getSignInTradeHandler();
        tradeTlv = TradeTlv.getInstance();
        tradeTlv.cleanTlv();
        tradeTlv.setTagValue(TradeTlv.TradeId, getString(R.string.SignInID).getBytes());//设置交易ID
    }

    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.login_in:
                signInTrade.ProcessStart(iProcessListener);//开始签到
                break;
            case R.id.cancel:
                FinishResult();
                break;
        }
    }

    protected IProcessListener iProcessListener = new IProcessListener() {
        @Override
        public void onStart() {
            Log.d(TAG, getString(R.string.begin_signing));
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
                            signInTrade.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
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
        public void onFinish(TransResult result,byte[]errtip,int len) {
            if (result == TransResult.APPROVED) {
                Intent i = new Intent(SigninActivity.this, ResponseResultActivity.class);
                i.putExtra("status", true);
                i.putExtra("tip", getString(R.string.sign_succeed));
                startActivity(i);

            } else {
                Intent i = new Intent(SigninActivity.this, ResponseResultActivity.class);
                i.putExtra("status", false);
                if(errtip!=null&&len!=0){

                    i.putExtra("tip", new String(errtip));
                }else{
                    i.putExtra("tip", getString(R.string.sign_failed));
                }

                startActivity(i);
                setResult(-1);
            }
            finish();

        }
    };
}
