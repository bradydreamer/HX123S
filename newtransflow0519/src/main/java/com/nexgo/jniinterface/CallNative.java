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

    public final static int OK = 1;
    public final static int FAILD = -1;
    public final static int NOT_END = 100;
    public final static int SIGN_BILL = 1;                  //签购单
    public final static int REPRINT_SIGN_BILL = 4;          //重打签购单
    public final static int DETAILED_BILL = 2;              //明细单
    public final static int COUNT_BILL = 3;                 //统计单


    public static native int jnitest();

    /**
     * 处理交易逻辑并组织8583报文
     *
     * @param Intlvdata     预留 填NULL
     * @param Indatalen     预留 填0
     * @param Outpackage8583 预留 填 NULL
     * @param Outplen       预留 填NULL
     * @return   >0:组报文成功  <0：组报文失败
     * eg:int ret = Package(null,0,null,null);
     */
    public static native int Package(byte[] Intlvdata, int Indatalen, byte[] Outpackage8583, byte[] Outplen);


    /**解析8583报文并处理交易逻辑与存交易数据
     * @param Innetstate 当前POS机网络状态  0x00：OK 0x01:timeout 0x02 :interrut 0x03 connect err 0x99：other err
     * @param Inplen     入参长度 填1
     * @param Outtlvdata 错误提示信息 最大256字节
     * @param Outdatalen 出参长度 最大2字节
     * @return 1:交易成功  <0：交易失败   100:交易处理未完成，须再次调用Package函数
     * eg: byte errtip[]=new byte[256];
     *     byte datalen[]=new byte[2];
     *     int ret = UnPackage(new byte[]{0x00}, 1, errtip, datalen);
     *     if(ret == 100){Package(null,0,null,null);}
     */
    public static native int UnPackage(byte[] Innetstate, int Inplen, byte[] Outtlvdata, byte[] Outdatalen);

    /**
     * 调用C层拼装打印 签购单，结算单
     *
     * @param printType 打印单据的类型
     * @return 1
     */
    public static native int formPrintScript(int printType);

}


