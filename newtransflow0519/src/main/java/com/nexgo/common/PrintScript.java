package com.nexgo.common;

import android.graphics.BitmapFactory;
import android.os.RemoteException;
import android.util.Log;

import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.device.printer.FontFamily;
import com.nexgo.smartpos.api.device.printer.OnPrintListener;
import com.nexgo.smartpos.api.device.printer.Printer;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import java.util.Timer;
import java.util.TimerTask;
import java.util.regex.Pattern;

import rx.Subscriber;


/**
 * Created by Mai on 2016/3/17.
 */
public class PrintScript {

    String TAG = "PrintScript";
    private boolean bMultipul = false;
    private Printer printer;
    private int paperWidth = 384;
    private int gray = 4;
    private boolean isBoldFont = false;
    private int fontSize = FontFamily.MIDDLE;
    private int curChnFontWidth = 16;
    private int curChnFonHeight = 16;
    private int curAscFontWidth = 8;
    private int curAscFontHeight = 16;
    private DeviceServiceEngine deviceServiceEngine;
    private OnPrintListener currPrtListener = null;
    private boolean bPrintErr = false;
    public final static int NOT_END = 100;                    //继续打印
    public final static int PRINT_END = 0;                     //结束打印

    public final static int SIGN_BILL = 1;                  //签购单
    public final static int REPRINT_SIGN_BILL = 4;          //重打签购单
    public final static int DETAILED_BILL = 2;              //明细单
    public final static int COUNT_BILL = 3;                 //统计单
    public final static int TRADE_COUNT_BILL = 5;                 //结算总计单


    private OnPrintListener printListener = new OnPrintListener.Stub() {
        @Override
        public void onPrintResult(int retCode) throws RemoteException {
            String result;
            result = "打印结果:" + ((retCode == 0) ? "成功" : "失败");
            Log.d(TAG, result);
            bMultipul = false;
        }
    };

    private OnPrintListener printingListener = new OnPrintListener.Stub() {
        @Override
        public void onPrintResult(int retCode) throws RemoteException {

            if (retCode != 0) {
                bPrintErr = true;
            } else
                retCode = 100;
            bMultipul = false;
            currPrtListener.onPrintResult(retCode);

        }
    };

    Timer timer = new Timer();

    TimerTask timertask = new TimerTask() {
        @Override
        public void run() {
            bMultipul = false;
        }
    };

    public PrintScript() {
        try {
            deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
            printer = deviceServiceEngine.getPrinter();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public PrintScript(int width) {
        paperWidth = width;
        Log.d(TAG, "paper width" + paperWidth);
        try {
            deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
            printer = deviceServiceEngine.getPrinter();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    public void setChnFont(String type) {
        int width = curChnFontWidth, height = curChnFonHeight;

        if (type.startsWith("s")) {
            fontSize = FontFamily.SMALL;
        } else if (type.startsWith("l") || type.startsWith("sl") || type.startsWith("nl")) {
            fontSize = FontFamily.BIG;
        } else {
            fontSize = FontFamily.MIDDLE;
        }

        setChnFont(width, height);
    }

    public void setChnFont(int width, int height) {
        curChnFontWidth = width;
        curChnFonHeight = height;
    }


    public void setAscFont(int width, int height) {
        curAscFontWidth = width;
        curAscFontHeight = height;
    }

    public void setAscFont(String type) {
        int width = curAscFontWidth, height = curAscFontHeight;

        if (type.startsWith("s")) {
            fontSize = FontFamily.SMALL;
        } else if (type.startsWith("l") || type.startsWith("sl") || type.startsWith("nl")) {
            fontSize = FontFamily.BIG;
        } else {
            fontSize = FontFamily.MIDDLE;
        }

        setAscFont(width, height);
    }

    public void setFontGray(int grayLevel) {

        gray = grayLevel;
        isBoldFont = (gray > 5);

    }

    private int parseOnePrintCmd(String[] cmdArray) {

        String[] tmpStr;

        Log.d(TAG, "num :" + cmdArray.length);
        Log.d(TAG, "num :" + cmdArray[0]);

        for (String str : cmdArray) {

            Log.d(TAG, "String str 11111"+ str);
            str = str.trim();
            Log.d(TAG, "String str 222"+ str);
            tmpStr = str.split("[ ]");

            if (bPrintErr)
                return -1;

            if (tmpStr[0].startsWith("!hz")) {
                Log.d(TAG, tmpStr[0] + " 11111111  " + tmpStr[1]);
                setChnFont(tmpStr[1]);

            } else if (tmpStr[0].startsWith("!asc")) {

                Log.d(TAG, tmpStr[0] + " 11111111  " + tmpStr[1]);
                setAscFont(tmpStr[1]);

                Log.d(TAG, "asc");
            } else if (tmpStr[0].startsWith("!gray")) {

                setFontGray(Integer.parseInt(tmpStr[1]));
                Log.d(TAG, "gray");
            } else if (tmpStr[0].startsWith("!yspace")) {
                Log.d(TAG, "yspace");
            } else if (tmpStr[0].startsWith("*text")) {
                String prtData;

                if(tmpStr.length > 2){
                    prtData = tmpStr[2];
                    for (int j = 3; j < tmpStr.length; j++) {
                        prtData += (" " + tmpStr[j]);
                    }
                }else{
                    prtData = " ";
                }

                Log.d(TAG, prtData);
                try {
                    printer.appendPrnStr(prtData, fontSize, isBoldFont);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }

                Log.d(TAG, "text");
            } else if (tmpStr[0].startsWith("*image")) {
                try {
                    printer.appendImage(BitmapFactory.decodeFile(tmpStr[1]));
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
                Log.d(TAG, "image");
            } else if (tmpStr[0].startsWith("*pause")) {
                Log.d(TAG, "pause");

                int pauseTime = Integer.parseInt(tmpStr[1]);
                try {
                    bMultipul = true;
                    printer.startPrint(printingListener);
                } catch (RemoteException e) {
                    e.printStackTrace();
                }

                timer.schedule(timertask, 5000);    ///等待上一联的状态
                while (bMultipul) ;
                try {
                    Thread.currentThread().sleep(pauseTime * 1000);//阻断 pauseTime 秒
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

            } else {
                Log.d(TAG, "other");
            }
        }

        try {
            printer.startPrint(currPrtListener);
        } catch (RemoteException e) {
            e.printStackTrace();
        }


        return 1;
    }

    private String[] splitPrintCmd(String printData) {

        String[] strs;

        strs = printData.split("[\n]+");
        for (int i = 0; i < strs.length; i++) {
            Log.d(TAG, "N0:" + i);
            Log.d(TAG, strs[i]);
            //System.out.println(strs[i]);
        }
        return strs;
    }

    public int startPrint(String printData) {
        String[] cmdArray;

        if (printData == null)
            return -1;

        bPrintErr = false;
        currPrtListener = printListener;
        cmdArray = splitPrintCmd(printData);
        if (cmdArray != null) {
            parseOnePrintCmd(cmdArray);
            return -1;
        }
        return 1;
    }

    public int startPrint(String printData, OnPrintListener printListener) {
        String[] cmdArray;

        if (printData == null)
            return -1;

        bPrintErr = false;
        if (printListener != null)
            currPrtListener = printListener;

        cmdArray = splitPrintCmd(printData);
        if (cmdArray != null) {
            parseOnePrintCmd(cmdArray);
            return -1;
        }
        return 1;
    }

    public int startPrintSync(String printData) {
        int result;
        String[] cmdArray;

        cmdArray = splitPrintCmd(printData);
        if (cmdArray == null) {
            return -1;
        }

        result = parseOnePrintCmd(cmdArray);
        if (result != 1) {
            return -1;
        }

        Subscriber<String> mySubscriber = new Subscriber<String>() {
            @Override
            public void onNext(String s) {
                System.out.println(s);
            }

            @Override
            public void onCompleted() {
            }

            @Override
            public void onError(Throwable e) {
            }
        };
        return 1;

    }

    public int setPrintType(int type) {
        return CallNative.formPrintScript(type);
    }

}
