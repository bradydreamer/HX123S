package com.hxb.pos.device;

import android.app.Application;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.IBinder;
import android.util.Log;

import com.hxb.pos.device.utils.AppGlobal;
import com.hxb.pos.service.DataExchangeAidl;
import com.nexgo.transflow.TransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by xiaox on 16/3/23.
 */
public class NexgoApp extends Application {
    private Logger log = LoggerFactory.getLogger("NexgoApp");
    private static NexgoApp singletion;
    private static TransFlow transFlow;
    private static Context mContext;

    private DataExchangeAidl dataExchangeAidl;
    private final ServiceConnection conn = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            log.debug("绑DataExchangeService服务成功");
            dataExchangeAidl = DataExchangeAidl.Stub.asInterface(service);
            AppGlobal.getInstance().setDataExchangeAidl(dataExchangeAidl);
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            log.error("DataExchangeService服务断开");
            dataExchangeAidl = null;
        }
    };

    @Override
    public void onCreate() {
        super.onCreate();
        mContext = getApplicationContext();
        transFlow = TransFlow.getInstance(mContext);
        bindHxbSdkService();
    }

    public static NexgoApp getInstance() {
        if (singletion == null) {
            singletion = new NexgoApp();
        }
        return singletion;
    }

    public static Context getContext(){
        return mContext;
    }

    public TransFlow getTransFlow() {
        return transFlow;
    }

    private void bindHxbSdkService(){
        Intent intent = new Intent();
        intent.setPackage("com.hxb.pos");
        intent.setAction("hxb.intent.action.DataExchangeService");
        getApplicationContext().bindService(intent, conn, Context.BIND_AUTO_CREATE);
    }
}
