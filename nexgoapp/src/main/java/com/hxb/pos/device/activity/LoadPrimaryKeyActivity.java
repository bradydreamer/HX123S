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
import com.nexgo.transflow.SignInTrade;
import com.nexgo.transflow.TransResult;

import java.util.Arrays;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/28 08:36
 * Describe:
 */
public class LoadPrimaryKeyActivity extends Activity {

    private final static String TAG = "LoadPrimaryKeyActivity";
    private LoadPrimaryKeyTrade loadPrimaryKeyTrade = null;
    private TradeTlv tradeTlv = null;
    private Context context = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        context = NexgoApp.getContext();
        loadPrimaryKeyTrade = NexgoApp.getInstance().getTransFlow().getDownLoadPrimaryKeyHandler();
        Log.d("zbq", "jump out BaseProcess");
        tradeTlv = TradeTlv.getInstance();
        tradeTlv.cleanTlv();
        tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.DownLoadPrimaryKeyID).getBytes());//设置交易ID

        startLoad();
    }

//    public LoadPrimaryKeyActivity(){
//        context = NexgoApp.getContext();
//        loadPrimaryKeyTrade = NexgoApp.getInstance().getTransFlow().getDownLoadPrimaryKeyHandler();
//        Log.d("zbq", "jump out BaseProcess");
//        tradeTlv = TradeTlv.getInstance();
//        tradeTlv.cleanTlv();
//        tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.DownLoadPrimaryKeyID).getBytes());//设置交易ID
//    }

    public void startLoad(){
        Log.d("zbq","jump into startLoad");
        loadPrimaryKeyTrade.ProcessStart(iProcessListener);//开始联机下载主密钥
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
//                    byte[] buffer = {(byte)0x00,(byte)0X8D,
//                            (byte)0x60,(byte)0x00,(byte)0x00,(byte)0x00,(byte)0x02,(byte)0x60,(byte)0x02,(byte)0x04,
//                            (byte)0x00,(byte)0x00,(byte)0x00,(byte)0x08,(byte)0x10,(byte)0x00,(byte)0x38,(byte)0x00,
//                            (byte)0x00,(byte)0x0a,(byte)0xc0,(byte)0x00,(byte)0x36,(byte)0x00,(byte)0x00,(byte)0x02,
//                            (byte)0x15,(byte)0x33,(byte)0x13,(byte)0x05,(byte)0x10,(byte)0x31,(byte)0x35,(byte)0x33,
//                            (byte)0x33,(byte)0x31,(byte)0x33,(byte)0x32,(byte)0x34,(byte)0x34,(byte)0x38,(byte)0x38,
//                            (byte)0x39,(byte)0x30,(byte)0x30,(byte)0x30,(byte)0x35,(byte)0x39,(byte)0x30,(byte)0x30,
//                            (byte)0x30,(byte)0x30,(byte)0x33,(byte)0x33,(byte)0x30,(byte)0x34,(byte)0x33,(byte)0x39,
//                            (byte)0x31,(byte)0x30,(byte)0x35,(byte)0x38,(byte)0x31,(byte)0x32,(byte)0x30,(byte)0x30,
//                            (byte)0x30,(byte)0x31,(byte)0x08,(byte)0xbd,(byte)0xbb,(byte)0xd2,(byte)0xd7,(byte)0xb3,
//                            (byte)0xc9,(byte)0xb9,(byte)0xa6,(byte)0x00,(byte)0x11,(byte)0x00,(byte)0x00,(byte)0x00,
//                            (byte)0x01,(byte)0x99,(byte)0x9f,(byte)0x00,(byte)0x41,(byte)0x43,(byte)0x30,(byte)0x30,
//                            (byte)0x42,(byte)0x46,(byte)0x46,(byte)0x37,(byte)0x38,(byte)0x43,(byte)0x43,(byte)0x41,
//                            (byte)0x42,(byte)0x35,(byte)0x37,(byte)0x30,(byte)0x39,(byte)0x32,(byte)0x36,(byte)0x43,
//                            (byte)0x43,(byte)0x42,(byte)0x43,(byte)0x34,(byte)0x44,(byte)0x35,(byte)0x39,(byte)0x46,
//                            (byte)0x45,(byte)0x35,(byte)0x37,(byte)0x42,(byte)0x31,(byte)0x41,(byte)0x45,(byte)0x42,
//                            (byte)0x34,(byte)0x34,(byte)0x33,(byte)0x36,(byte)0x31,(byte)0x35,(byte)0x00,(byte)0x13,
//                            (byte)0x39,(byte)0x39,(byte)0x39,(byte)0x20,(byte)0x20,(byte)0x58,(byte)0x47,(byte)0x44,
//                            (byte)0x7c,(byte)0x47,(byte)0x38,(byte)0x37,(byte)0x30
//                    };

//                    byte[] buffer = {(byte)0x00,(byte)0X8D,
//                            (byte)0x60,(byte)0x00,(byte)0x00,(byte)0x00,(byte)0x02,
////                            (byte)0x60,(byte)0x02,(byte)0x04,(byte)0x00,(byte)0x00,(byte)0x00,
//                            (byte)0x85,(byte)0x04,(byte)0x07,(byte)0x05,(byte)0x90,(byte)0x00,(byte)0x03,(byte)0x00,(byte)0x67,
//                            (byte)0x08,(byte)0x10,(byte)0x00,(byte)0x38,(byte)0x00,
//                            (byte)0x00,(byte)0x0a,(byte)0xc0,(byte)0x00,(byte)0x36,(byte)0x00,(byte)0x00,(byte)0x02,
//                            (byte)0x15,(byte)0x33,(byte)0x13,(byte)0x05,(byte)0x10,(byte)0x31,(byte)0x35,(byte)0x33,
//                            (byte)0x33,(byte)0x31,(byte)0x33,(byte)0x32,(byte)0x34,(byte)0x34,(byte)0x38,(byte)0x38,
//                            (byte)0x39,(byte)0x30,(byte)0x30,(byte)0x30,(byte)0x35,(byte)0x39,(byte)0x30,(byte)0x30,
//                            (byte)0x30,(byte)0x30,(byte)0x33,(byte)0x33,(byte)0x30,(byte)0x34,(byte)0x33,(byte)0x39,
//                            (byte)0x31,(byte)0x30,(byte)0x35,(byte)0x38,(byte)0x31,(byte)0x32,(byte)0x30,(byte)0x30,
//                            (byte)0x30,(byte)0x31,(byte)0x08,(byte)0xbd,(byte)0xbb,(byte)0xd2,(byte)0xd7,(byte)0xb3,
//                            (byte)0xc9,(byte)0xb9,(byte)0xa6,(byte)0x00,(byte)0x11,(byte)0x00,(byte)0x00,(byte)0x00,
//                            (byte)0x01,(byte)0x99,(byte)0x9f,(byte)0x00,(byte)0x41,(byte)0x43,(byte)0x30,(byte)0x30,
//                            (byte)0x42,(byte)0x46,(byte)0x46,(byte)0x37,(byte)0x38,(byte)0x43,(byte)0x43,(byte)0x41,
//                            (byte)0x42,(byte)0x35,(byte)0x37,(byte)0x30,(byte)0x39,(byte)0x32,(byte)0x36,(byte)0x43,
//                            (byte)0x43,(byte)0x42,(byte)0x43,(byte)0x34,(byte)0x44,(byte)0x35,(byte)0x39,(byte)0x46,
//                            (byte)0x45,(byte)0x35,(byte)0x37,(byte)0x42,(byte)0x31,(byte)0x41,(byte)0x45,(byte)0x42,
//                            (byte)0x34,(byte)0x34,(byte)0x33,(byte)0x36,(byte)0x31,(byte)0x35,(byte)0x00,(byte)0x13,
//                            (byte)0x39,(byte)0x39,(byte)0x39,(byte)0x20,(byte)0x20,(byte)0x58,(byte)0x47,(byte)0x44,
//                            (byte)0x7c,(byte)0x47,(byte)0x38,(byte)0x37,(byte)0x30};
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                    if (buffer != null) {
                        len = ByteUtils.byte2int(buffer[0], buffer[1]);
                        if (len > 0) {
                            Log.d(TAG, "接收报文的长度 = " + len);
                            Log.d(TAG, "接收到华夏sdk 数据 = " + ByteUtils.byteArray2HexStringWithSpace(Arrays.copyOfRange(buffer, 0, len + 2)));
                            loadPrimaryKeyTrade.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
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
                Intent i = new Intent(LoadPrimaryKeyActivity.this, ResponseResultActivity.class);
                i.putExtra("status", true);
                i.putExtra("tip", getString(R.string.load_kek_succeed));
                startActivity(i);

            } else {
                Intent i = new Intent(LoadPrimaryKeyActivity.this, ResponseResultActivity.class);
                i.putExtra("status", false);
                if(errtip!=null&&len!=0){

                    i.putExtra("tip", new String(errtip));
                }else {
                    i.putExtra("tip", getString(R.string.load_kek_failed));
                }
                startActivity(i);
                setResult(-1);
            }
            finish();
        }
    };
}
