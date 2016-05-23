package com.hxb.pos.device.activity;

import android.app.Activity;
import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.RemoteException;
import android.util.Log;

import com.hxb.pos.device.NexgoApp;
import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.AppGlobal;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.TradeTlv;
import com.nexgo.transflow.IProcessListener;
import com.nexgo.transflow.LoadPrimaryKeyTrade;
import com.nexgo.transflow.LocationTrade;
import com.nexgo.transflow.SignInTrade;
import com.nexgo.transflow.TransResult;

import java.util.Arrays;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/28 08:36
 * Describe:
 */
public class LocationActivity extends Activity {

    private final static String TAG = "LocationActivity";
    private LocationTrade locationTrade = null;
    private TradeTlv tradeTlv = null;
    private Context context = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        context = NexgoApp.getContext();
        locationTrade = NexgoApp.getInstance().getTransFlow().getLocationHandler();
        Log.d("zbq", "jump out BaseProcess");
        tradeTlv = TradeTlv.getInstance();
        tradeTlv.cleanTlv();
        tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.LocateID).getBytes());//设置交易ID
        startLocate();
    }

    public void startLocate(){
        Log.d("zbq","jump into startLocate");
        locationTrade.ProcessStart(iProcessListener);//开始联机下载主密钥
    }

    protected IProcessListener iProcessListener = new IProcessListener() {
        @Override
        public void onStart() {
            Log.d(TAG, context.getString(R.string.download_primarykey));
        }

        @Override
        public void onOnlineRequest(final byte[] data) {
            new Thread() {
                @Override
                public void run() {
                    int len = 0;
//                    Log.d(TAG, "发给华夏sdk 数据 =  " + Arrays.toString(data));
                    Log.d(TAG,"发给华夏sdk 数据 = " + ByteUtils.byteArray2HexStringWithSpace(data));
                    byte[] buffer = new byte[2048];
                    try {
                        buffer = AppGlobal.getInstance().getDataExchangeAidl().exchangeData(data);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    if (buffer != null) {
                        len = ByteUtils.byte2int(buffer[0], buffer[1]);
                        if (len > 0) {
                            Log.d(TAG, "接收报文的长度 = " + len);
                            Log.d(TAG, "接收到华夏sdk 数据 = " + ByteUtils.byteArray2HexStringWithSpace(Arrays.copyOfRange(buffer, 0, len + 2)));
                            locationTrade.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
                        }
                    } else {
                        onFinish(TransResult.ERROR,null,0);
                    }
                }
            }.start();
        }

        @Override
        public void onFinish(TransResult result,byte[]errtip,int len) {
//            Intent intent = new Intent();
//            intent.putExtra("tradeCode", result.toString());
//            try {
//                AppGlobal.getInstance().getTransactionResult().onResult(intent);
//            } catch (RemoteException e) {
//                e.printStackTrace();
//            }
            if (result == TransResult.APPROVED) {
                Intent i = new Intent(LocationActivity.this, ResponseResultActivity.class);
                i.putExtra("status", true);
                i.putExtra("tip", getString(R.string.location_succeed));
                startActivity(i);

            } else {
                Intent i = new Intent(LocationActivity.this, ResponseResultActivity.class);
                i.putExtra("status", false);
                if(errtip!=null&&len!=0){
                    i.putExtra("tip", new String(errtip));
                }else {
                    i.putExtra("tip", getString(R.string.location_failed));
                }
                startActivity(i);
                setResult(-1);
            }
            finish();
        }
    };
}
