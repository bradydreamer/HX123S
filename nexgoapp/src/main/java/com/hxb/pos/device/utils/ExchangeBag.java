package com.hxb.pos.device.utils;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.nfc.Tag;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.support.annotation.MainThread;
import android.util.Log;

import com.hxb.pos.device.NexgoApp;
import com.hxb.pos.device.R;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.ref.WeakReference;
import java.util.Arrays;
import java.util.concurrent.TimeUnit;

import rx.Observable;
import rx.Subscriber;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/20 17:19
 * Describe:
 */
public class ExchangeBag {
    private static String TAG = "ExchangeBag";
    private static ProgressDialog proDia = null;
    private static Logger log = LoggerFactory.getLogger(ExchangeBag.class.getName());
    private AlertDialog.Builder builder;
    private Dialog dialog;
    private static MyHandler handler;

    static class MyHandler extends Handler {
        WeakReference<ExchangeBag> mWeakReference;

        MyHandler(ExchangeBag m) {
            mWeakReference = new WeakReference<ExchangeBag>(m);
        }

        @Override
        public void handleMessage(Message msg) {
            ExchangeBag exchangeBag = mWeakReference.get();
            switch (msg.what) {
                case 0x00: {
                    ExchangeBag.proDia.setTitle(R.string.commu_trade);
                    ExchangeBag.proDia.setMessage(msg.obj.toString());
                    ExchangeBag.proDia.onStart();
                    ExchangeBag.proDia.show();
                    break;
                }
                case 0x01: {
                    try {
                        ExchangeBag.proDia.dismiss();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                }
            }
        }
    }

    public static Observable<byte[]> sendAndRecv(final Context context,final byte[] sendData){
        Log.d(TAG,"jump into sendAndRecv,sendData = "+Arrays.toString(sendData));
        return Observable.create(new rx.Observable.OnSubscribe<byte[]>() {
             @Override
             public void call(Subscriber<? super byte[]> subscriber) {
//                    Message messagerecv = handler.obtainMessage();
//                    messagerecv.what = 0x00;
//                    messagerecv.obj = context.getString(R.string.commu_recv);
//                    handler.sendMessage(messagerecv);
                 synchronized (this) {
                     try {
                         Log.d(TAG, "发给华夏sdk 数据 = " + Arrays.toString(sendData));
                         byte[] buffer = new byte[2048];
                         buffer = AppGlobal.getInstance().getDataExchangeAidl().exchangeData(sendData);
                         Log.d(TAG, "接收到华夏sdk 数据 = " + Arrays.toString(buffer));

                         try {
                             this.wait();
                             if (!buffer.equals(new byte[2048])) {
                                 this.notify();
                             }
                         } catch (InterruptedException e) {
                             e.printStackTrace();
                         }
                         subscriber.onNext(buffer);
                         subscriber.onCompleted();
                     } catch (RemoteException e) {
                         Log.d(TAG, "RemoteException");
                         e.printStackTrace();
                         subscriber.onError(e);
                     }
                 }
             }
         }).timeout(60 * 1000, TimeUnit.MILLISECONDS);
    }
}
