package com.nexgo.transflow;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.IBinder;
import android.os.RemoteException;

import com.nexgo.common.GlobalHolder;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

/**
 * Created by xiaox on 16/3/11.
 */
public class TransFlow implements ITransEngine {
    private final SharedPreferences prefs;
    private DeviceServiceEngine deviceServiceEngine;
    private Logger log = LoggerFactory.getLogger("TransFlow");
    private final ServiceConnection conn = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            log.debug("绑服务成功");
            deviceServiceEngine = DeviceServiceEngine.Stub.asInterface(service);
            GlobalHolder.getInstance().setDeviceServiceEngine(deviceServiceEngine);
            try {
                deviceServiceEngine.login(bussinessId);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            log.error("服务断开");
            deviceServiceEngine = null;
            GlobalHolder.getInstance().setDeviceServiceEngine(null);
        }
    };
    private static final String bussinessId = "00000000";
    private static TransFlow instance;

    private TransFlow(Context context) {
        Intent intent = new Intent("com.nexgo.smartpos.service");
        intent.setPackage("com.nexgo.smartpos.api");
        context.bindService(intent, conn, Context.BIND_AUTO_CREATE);

        prefs = context.getSharedPreferences("applicationsetting", Context.MODE_PRIVATE);
        GlobalHolder.getInstance().setPrefs(prefs);
    }

    public static TransFlow getInstance(Context context) {
        return instance == null ? instance = new TransFlow(context.getApplicationContext()) : instance;
    }

    public void SetReadCardContext(Context context){

    }

    @Override
    public SharedPreferences getSharedPreferences() {
        return prefs;
    }

    @Override
    public LoadPrimaryKeyTrade getDownLoadPrimaryKeyHandler() {
        return new LoadPrimaryKeyTrade();
    }

    @Override
    public LocationTrade getLocationHandler() {
        return new LocationTrade();
    }

    @Override
    public SignInTrade getSignInTradeHandler() {
        return new SignInTrade();
    }

    @Override
    public SaleTrade getSaleTradeHandler(Context context) {
        return new SaleTrade(context);
    }

    @Override
    public QueryTrade getQueryTradeHandler(Context context) {
        return new QueryTrade(context);
    }

    @Override
    public SettleTrade getSettleHandler(Context context) {
        return new SettleTrade(context);
    }

    @Override
    public SaleVoidTrade getSaleVoidTradeHandler(Context context) {
        return new SaleVoidTrade(context);
    }

    @Override
    public RefundTrade getRefundTradeHandler(Context context) {
        return new RefundTrade(context);
    }

    @Override
    public PreauthTrade getPreauthradeHandler(Context context) {
        return new PreauthTrade(context);
    }


}
