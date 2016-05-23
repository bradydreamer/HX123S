package com.nexgo.transflow;


import android.content.Context;

/**
 * Created by xiaox on 16/3/11.
 */
public interface IProcess {
    public abstract void ProcessStart(IProcessListener processListener);
    public abstract void ProcessContinue(IProcessListener processListener);
    public abstract void onSetOnlineResponse(byte[] data,int linkstate);

    public abstract void onSetCancel();
}
