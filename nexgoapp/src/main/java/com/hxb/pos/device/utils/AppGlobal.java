package com.hxb.pos.device.utils;

import android.content.Context;
import android.util.Log;

import com.hxb.pos.device.service.TransactionResult;
import com.hxb.pos.service.DataExchangeAidl;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/20 16:17
 * Describe:
 */
public class AppGlobal {
    private String TAG = "AppGlobal";
    private  Context mContext;
    private static AppGlobal singletion = new AppGlobal();
    private  TransactionResult transactionResult;
    private  DataExchangeAidl dataExchangeAidl;
    public static AppGlobal getInstance() {
        return singletion;
    }

    public AppGlobal(){
        Log.d(TAG,"AppGlobal init OK");
    }

    public void setTransactionResult(TransactionResult transactionResult){
        this.transactionResult = transactionResult;
        Log.d(TAG,"setTransactionResult OK");
    }

    public TransactionResult getTransactionResult(){
        Log.d(TAG,"getTransactionResult OK");
        return transactionResult;
    }

    public DataExchangeAidl getDataExchangeAidl() {
        return dataExchangeAidl;
    }

    public void setDataExchangeAidl(DataExchangeAidl dataExchangeAidl) {
        this.dataExchangeAidl = dataExchangeAidl;
    }

    public Context getmContext() {
        return mContext;
    }

    public void setmContext(Context mContext) {
        this.mContext = mContext;
    }
}
