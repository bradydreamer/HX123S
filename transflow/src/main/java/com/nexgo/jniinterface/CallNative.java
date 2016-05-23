package com.nexgo.jniinterface;

/**
 * Created by caibaqun on 2016/2/29.
 */
public class CallNative {
    static {
        System.loadLibrary("emvandroid");
        System.loadLibrary("g870s");
        System.loadLibrary("sdksmartpos");

    }

    public final static int OK=1;
    public final static int FAILD=-1;
    public final static int NOT_END=100;
    public final static int SIGN_BILL = 1;                  //签购单
    public final static int REPRINT_SIGN_BILL = 4;          //重打签购单
    public final static int DETAILED_BILL = 2;              //明细单
    public final static int COUNT_BILL = 3;                 //统计单


    public static native int jnitest();

    public static native int Package(byte[] tlvdata, int datalen, byte[] package8583, byte[] jplen);

    public static native int UnPackage(byte[] package8583, int plen, byte[] tlvdata, byte[] datalen);

    /**
     * 调用C层拼装打印 签购单，结算单
     *
     * @param printType 打印单据的类型
     *
     * @return 1
     */
    public static native int formPrintScript(int printType);

}


