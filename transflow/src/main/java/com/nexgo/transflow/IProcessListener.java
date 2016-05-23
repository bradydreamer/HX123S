package com.nexgo.transflow;

/**
 * Created by xiaox on 16/3/11.
 */
public interface IProcessListener {
    public abstract void onStart();
    public abstract void onOnlineRequest(final byte[] data);
    public abstract void onFinish(TransResult result,byte[]ErrTip,int len);
}
