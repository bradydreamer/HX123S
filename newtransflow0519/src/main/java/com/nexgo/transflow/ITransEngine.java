package com.nexgo.transflow;

import android.content.Context;
import android.content.SharedPreferences;

/**
 * Created by xiaox on 16/3/12.
 */
public interface ITransEngine {
    public abstract SharedPreferences getSharedPreferences();

    public abstract SignInTrade getSignInTradeHandler();

    public abstract QueryTrade getQueryTradeHandler(Context context);

    public abstract SaleTrade getSaleTradeHandler(Context context);

    public abstract SettleTrade getSettleHandler(Context context);

    public abstract SaleVoidTrade getSaleVoidTradeHandler(Context context);

    public abstract RefundTrade getRefundTradeHandler(Context context);

    public abstract PreauthTrade getPreauthradeHandler(Context context);

    public abstract ECSaleTrade getEcSaleHandler(Context context);

    public abstract ECLoadTrade getEcLoadHandler(Context context);

    public abstract ECQueryTrade getEcQueryHandler(Context context);

    public abstract UploadTrade getUploadHandler(Context context);

    public abstract InstallmentTrade getInstallmentHandler(Context context);
    public abstract InstallmentVoidTrade getInstallmentVoidHandler(Context context);

    public abstract UpdateAidTrade getUpdateAidHandler(Context context);

    //华夏银行--begain
    public abstract LoadPrimaryKeyTrade getDownLoadPrimaryKeyHandler();
    public abstract LocationTrade getLocationHandler();
    //华夏银行--end

}
