package com.nexgo.common;

/**
 * Created by caibaqun on 2016/3/11.
 */
public class SecAuthResult {
    private int retcode;
    private byte[] scriptresult = null;
    private byte[] tc=null;
    public final static int ERR = -1;

    public SecAuthResult(int retcode, byte[] scriptresult,byte[]tc) {
        this.retcode = retcode;
        this.scriptresult = scriptresult;
        this.tc=tc;
    }

    public int getRetcode() {
        return this.retcode;
    }

    public byte[] getScriptresult() {
        return this.scriptresult;
    }

    public byte[] getTc(){
        return tc;
    }


}
