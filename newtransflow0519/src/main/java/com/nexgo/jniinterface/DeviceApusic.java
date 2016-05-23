package com.nexgo.jniinterface;

import android.os.Bundle;
import android.os.RemoteException;
import android.text.format.Time;
import android.util.Log;

import com.nexgo.common.ByteUtils;
import com.nexgo.common.GlobalHolder;
import com.nexgo.common.PrintScript;
import com.nexgo.common.SecAuthResult;
import com.nexgo.common.TradeTlv;
import com.nexgo.common.gesture.GestureResult;
import com.nexgo.db.DbProxyFactory;
import com.nexgo.db.DbRecordOperation;
import com.nexgo.db.Dboperation;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.pinpad.DesAlgorithmType;
import com.nexgo.smartpos.api.device.pinpad.MacAlgorithmType;
import com.nexgo.smartpos.api.device.pinpad.PinPad;
import com.nexgo.smartpos.api.device.pinpad.WorkKeyType;
import com.nexgo.smartpos.api.emv.EmvHandler;
import com.nexgo.smartpos.api.emv.EmvOnlineResult;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;

import de.greenrobot.event.EventBus;
import rx.subjects.AsyncSubject;

/**
 * C层回调JAVA操作外设接口
 * 包括 计算MAC、下载工作密钥、EMV流程2、打印
 * Created by caibaqun on 2016/3/9.
 */

public class DeviceApusic {

    private static final String TAG = "DeviceApusic";
    private static volatile AsyncSubject<LocalEvent> subject;
    private PinPad pinPad;
    private EmvHandler emvHandler;
    DeviceServiceEngine deviceServiceEngine;
    private TradeTlv tradeTlv = null;
    private int mKeyIdx = 0;//主密钥索引
    //华夏银行--begain
//    private int KLKIndex = 1;//传输密钥索引
//    private int AlgorithmType = 0;//加密算法类型
    private int AlgorithmType = 1;//加密算法类型
    //华夏银行--end
    private int TakEncry = 0;//是否磁道加密
    private Logger log = LoggerFactory.getLogger("DeviceApusic");

    public DeviceApusic() {
        try {
            EventBus.getDefault().register(this);
            deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
            pinPad = deviceServiceEngine.getPinPad();
            emvHandler = deviceServiceEngine.getEmvHandler();
            tradeTlv = TradeTlv.getInstance();
            mKeyIdx = Integer.valueOf(new String(tradeTlv.getTagValue(TradeTlv.Tmkindex)));
            TakEncry = Integer.valueOf(new String(tradeTlv.getTagValue(TradeTlv.TakEncry)));
            if (Integer.valueOf(new String(tradeTlv.getTagValue(TradeTlv.EncryType))) == DesAlgorithmType.DES) {
                AlgorithmType = DesAlgorithmType.DES;
            } else {
                AlgorithmType = DesAlgorithmType.TDES;
            }
//            //FIXME test
//            TakEncry = 0;
//            AlgorithmType = DesAlgorithmType.DES;
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    //华夏银行--begain

    /**
     * 获取系统时间
     * @return 6 byte array ：13 01 21 19 42 18 表示13年1月21日19点42 分18秒
     */
    public byte[] getSysTimeForC(){
        Log.d("zbq", "jump into getSysTimeForC");
        byte[] outdata = new byte[6];

        Time time = new Time("GMT+8");
        time.setToNow();
        int year = time.year;
        int month = time.month;
        int day = time.monthDay;
        int hour = time.hour;
        int minute = time.minute;
        int sec = time.second;

        System.arraycopy(ByteUtils.int2BCDByteArray(year),1,outdata,0,1);
        System.arraycopy(ByteUtils.int2BCDByteArray(month),0,outdata,1,1);
        System.arraycopy(ByteUtils.int2BCDByteArray(day),0,outdata,2,1);
        System.arraycopy(ByteUtils.int2BCDByteArray(hour),0,outdata,3,1);
        System.arraycopy(ByteUtils.int2BCDByteArray(minute),0,outdata,4,1);
        System.arraycopy(ByteUtils.int2BCDByteArray(sec), 0, outdata, 5, 1);

        Log.d("zbq", "SysTime outdata = " + ByteUtils.byteArray2HexStringWithSpace(outdata));
        return outdata;
    }


    /**
     * 使用 主密钥进行 Des 运算
     * @param indata 等待DES运算的数据
     * @param desMode 0-加密 1-解密
     * @return 运算结果
     */
    public byte[] desByTmsKeyForC(byte[] indata,int desMode){
        Log.d("zbq","jump into desByTmsKeyForC");
        int indatalen = indata.length;
        byte[] outdata = new byte[8];
        int tmsMKeyIdx = 1;//Integer.parseInt(tradeTlv.getTagValue(TradeTlv.Klkindex).toString());
        int desType = 1;//Integer.parseInt(tradeTlv.getTagValue(TradeTlv.EncryType).toString());

        log.debug("indata:{}, desmode:{},tmsMKeyIdx:{}, desType:{} ", ByteUtils.byteArray2HexString(indata), desMode, tmsMKeyIdx, desType);
        if(indatalen == 8) {
            try {
//                outdata = pinPad.desByTmsKey(tmsMKeyIdx, indata, indatalen, desType, desMode);
                outdata = pinPad.desByPlainKey(tmsMKeyIdx, indata, indatalen, desType, desMode);

                log.debug("outdata:{}", ByteUtils.byteArray2HexString(outdata));
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        log.debug("desByTmsKey:使用密码键盘密钥进行DES计算");
        return outdata;
    }

    /**
     * 使用除 PinKey 外的工作密钥进行 DES 加密
     * @param indata 等待DES运算的数据
     * @param wKeyType 0(int) PIN 密钥 ；1(int) MAC 密钥；2(int) 磁道加密密钥
     * @return 运算结果
     */
    public byte[] desEncByWKeyForC(byte[] indata,int wKeyType){

        Log.d("zbq","jump into desEncByWKeyForC");

        int indatalen = indata.length;
        byte[] outdata = new byte[8];

        if(indatalen == 8) {
            try {
                pinPad.desEncByWKey(mKeyIdx, wKeyType, indata, indatalen, outdata);
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        log.debug("desEncByWKeyForC:使用密码键盘工作密钥进行DES计算");
        return outdata;
    }

    /**
     * 存储主秘钥密文
     * @param keyData
     */
    public void LoadMKForC(byte[] keyData) {
        int keyDataLen = keyData.length;
        boolean isTmsKey = false;
        int result = 0;

        if (keyDataLen == 8 || keyDataLen == 16) {
            try {
                result = pinPad.loadPlainMKey(mKeyIdx, keyData, keyDataLen, isTmsKey);
            } catch (RemoteException e1) {
                e1.printStackTrace();
            }
            log.debug("loadMK:加载明文主密钥");
        }

    }
    /**
     * 存储传输密钥
     * @param keyData 传输密钥
     * @param KLKIndex 索引 默认1
     */
    public void LoadKLK(byte[] keyData,int KLKIndex){
        int keyDataLen = keyData.length;
        boolean isTmsKey = true;
        int result = 0;
        Log.d("zbq","jump into LoadKLK");
        log.debug("keyDataLen = {}", keyDataLen);
        if (keyDataLen == 8 || keyDataLen == 16) {
            try {
//                result = pinPad.loadPlainMKey(KLKIndex, keyData, keyDataLen, isTmsKey);
                log.debug("KLKData:{}", ByteUtils.byteArray2HexString(keyData));
                result = pinPad.loadPlainDesKey(KLKIndex, keyData, keyDataLen);
            } catch (RemoteException e1) {
                e1.printStackTrace();
            }
            log.debug("loadPlainDesKey:加载明文 Des 密钥");
        }
    }

    /**
     * 下载工作密钥
     *
     * @param WK 计算MAC数据源
     * @return boolean 是否下载工作密钥成功
     * @author caibaqun
     * @since 2016年3月10日
     */
    public boolean DownLoadWKForC(byte[] WK) {
        byte[] PIK = new byte[16];
        byte[] MAK = new byte[16];
        byte[] TDK = new byte[16];
        byte[] pikKCVValue = new byte[4];
        byte[] macKCVValue = new byte[4];
        byte[] tdkKCVValue = new byte[4];

        log.debug("工作密钥:{}", ByteUtils.byteArray2HexString(WK));
        if (WK.length != 24 && WK.length != 40 && WK.length != 56 && WK.length != 60 && WK.length != 84) {
            log.debug("工作密钥长度错误");
            return false;
        }
        //PIK
        if (AlgorithmType == DesAlgorithmType.TDES) {
            System.arraycopy(WK, 0, PIK, 0, 16);
            System.arraycopy(WK, 16, pikKCVValue, 0, 4);
        } else {
            System.arraycopy(WK, 0, PIK, 0, 8);
            System.arraycopy(WK, 8, pikKCVValue, 0, 4);
        }
        //MAK
        if (AlgorithmType == DesAlgorithmType.TDES) {
            System.arraycopy(WK, 20, MAK, 0, 16);
            System.arraycopy(WK, 36, macKCVValue, 0, 4);
        } else {
            System.arraycopy(WK, 12, MAK, 0, 8);
            System.arraycopy(WK, 20, macKCVValue, 0, 4);
        }
        //TDK
        if (AlgorithmType == DesAlgorithmType.TDES && TakEncry == 1) {
            System.arraycopy(WK, 40, TDK, 0, 16);
            System.arraycopy(WK, 56, tdkKCVValue, 0, 4);
        }

        log.debug("AlgorithmType = {},PIK = {}, MAK = {} ",AlgorithmType,ByteUtils.byteArray2HexStringWithSpace(PIK),ByteUtils.byteArray2HexStringWithSpace(MAK));

        //下载密钥
        try {
            int pinResult = 0;
            int macResult = 0;
            int tdkResult = 0;
            if (AlgorithmType == DesAlgorithmType.TDES) {
                pinResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.PINKEY, PIK, 16);
                macResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.MACKEY, MAK, 16);
                if (TakEncry == 1) {
                    tdkResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.TDKEY, TDK, 16);
                }

            } else {
                pinResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.PINKEY, PIK, 8);
                macResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.MACKEY, MAK, 8);
                if (TakEncry == 1&&AlgorithmType == DesAlgorithmType.TDES) {
                    tdkResult = pinPad.loadWKey(mKeyIdx, WorkKeyType.TDKEY, TDK, 8);
                }
            }

            if (pinResult != 0 || macResult != 0 || tdkResult != 0) {
                log.debug("工作密钥下载失败,pinResult = {}, macResult = {},tdkResult = {} ",pinResult, macResult ,tdkResult );
                return false;
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }

        //校验WK
        try {
            byte[] pKCV = {0};
            byte[] mKCV = {0};
            byte[] tKCV = {0};
            if (TakEncry == 1&&AlgorithmType == DesAlgorithmType.TDES) {
                pKCV = pinPad.calcWKeyKCV(mKeyIdx, WorkKeyType.PINKEY);
                mKCV = pinPad.calcWKeyKCV(mKeyIdx, WorkKeyType.MACKEY);
                tKCV = pinPad.calcWKeyKCV(mKeyIdx, WorkKeyType.TDKEY);
                if (Arrays.equals(pKCV, pikKCVValue) && Arrays.equals(mKCV, macKCVValue) && Arrays.equals(tKCV, tdkKCVValue)) {
                    log.debug("工作密码校验成功");
                    return true;
                }
            } else {
                pKCV = pinPad.calcWKeyKCV(mKeyIdx, WorkKeyType.PINKEY);
                mKCV = pinPad.calcWKeyKCV(mKeyIdx, WorkKeyType.MACKEY);
                if (Arrays.equals(pKCV, pikKCVValue) && Arrays.equals(mKCV, macKCVValue)) {
                    log.debug("工作密码校验成功");
                    return true;
                }
            }
            log.debug("pKCV = {}, pikKCVValue = {},mKCV = {}, macKCVValue = {} ",ByteUtils.byteArray2HexStringWithSpace(pKCV), ByteUtils.byteArray2HexStringWithSpace(pikKCVValue),ByteUtils.byteArray2HexStringWithSpace(mKCV), ByteUtils.byteArray2HexStringWithSpace(macKCVValue));
        } catch (RemoteException e) {
            e.printStackTrace();
        }

        log.debug("工作密码校验失败");
        return false;
    }

    //华夏银行--end

    /**
     * 磁道加密
     *
     * @param TrackData 二磁道或三磁道信息
     * @return 需要磁道加密返回加密后结果，不需要返回原磁道信息，异常情况返回全F
     * @throws RemoteException
     */
    public byte[] TrackDataEncryptForC(byte[] TrackData) throws RemoteException {

        if (TrackData.length > 9) {
            if (TakEncry == 0)
                    return null;

            byte[] tmpData = new byte[8];
            byte[] desResult = new byte[8];
            byte[] encryptData;
            int len = TrackData.length;
            encryptData = new byte[len];

            System.arraycopy(TrackData, len - 1 - 8, tmpData, 0, 8);
            System.arraycopy(TrackData, 0, encryptData, 0, len);
            pinPad.desEncByWKey(mKeyIdx, WorkKeyType.TDKEY, tmpData, tmpData.length, desResult);

            Log.e("trackData", ByteUtils.byteArray2HexString(desResult));
            System.arraycopy(desResult, 0, encryptData, len - 1 - 8, 8);
            Log.e("trackData", ByteUtils.byteArray2HexString(encryptData));
            return encryptData;

        } else {
            //异常情况返回8个FF
            return new byte[]{(byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF, (byte) 0xFF};
        }
    }

    /**
     * 工作密钥加密
     *
     * @param mKeyIdx 主密钥索引
     * @param workKeyType 工作密钥类型
     * @param plain 明文数据（长度为 8）
     * @return 返回加密后数据，或空
     * @throws RemoteException
     */
    public byte[] wkDesEncryptForC(int mKeyIdx, int workKeyType, byte[] plain) throws RemoteException {

        if (plain.length != 8) {
            byte[] desResult = new byte[8];
            pinPad.desEncByWKey(mKeyIdx, workKeyType, plain, plain.length, desResult);
            Log.e("trackData", ByteUtils.byteArray2HexString(desResult));
            return desResult;

        } else {
            return null;
        }
    }

    /**
     * 计算MAC
     *
     * @param data 计算MAC数据源
     * @return 8字节MAC数据
     * @author caibaqun
     * @since 2016年3月10日
     */
    public byte[] CalculateMacForC(byte[] data) {
        try {
            Log.d(TAG, "CalculateMacForC");
            return pinPad.getMac(mKeyIdx, MacAlgorithmType.ECB, AlgorithmType, data);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return null;
    }

    public class EmvDataSource {
        public final static int FROMKERNEL = 0;
        public final static int FROMCARD = 1;
    }

    /**
     * @param Tag    入参 EMV 标签
     * @param pathId 入参 EmvDataSource.FROMKERNEL ：从EMV内核中取   EmvDataSource.FROMCARD 从卡中读数据(会向卡片发getdata命令)
     * @return TAG值
     * @throws RemoteException
     */
    public byte[] EmvgetTlv(byte[] Tag, int pathId) throws RemoteException {

        byte[] data = emvHandler.getTlvs(Tag, pathId);
        return data;

    }

    /**
     * @param tag   入参 EMV 标签
     * @param value 入参EMV 标签值
     * @return 0：成功   －1 失败
     * @throws RemoteException
     */
    public int EmvsetTlv(byte[] tag, byte[] value) throws RemoteException {
        int a = emvHandler.setTlv(tag, value);
        return a;
    }


    /**
     * IC卡二次授权
     *
     * @param rejcode  出参 处理码
     * @param authcode 出参 授权码
     * @param recvf55  出参 arpc
     * @param outdata  出参 脚本处理结果
     * @return int 0为二次授权成功，<0失败 详见错误表
     * @author caibaqun
     * @since 2016年3月10日
     */

    private static volatile AsyncSubject<SecAuthResult> secSsubject;

    public static int SecondAuthorizeForC(String rejcode, String authcode, byte[] recvf55, byte[] outScriptdata, byte[] outTC) {

        if (EventBus.getDefault().hasSubscriberForEvent(Bundle.class)) {
            Bundle bundle = new Bundle();
            bundle.putString(EmvOnlineResult.REJCODE, rejcode);
            bundle.putString(EmvOnlineResult.AUTHCODE, authcode);
            bundle.putByteArray(EmvOnlineResult.RECVF55, recvf55);
            secSsubject = AsyncSubject.create();
            EventBus.getDefault().post(bundle);
            SecAuthResult secAuthResult = secSsubject.serialize()
//                    .take(30, TimeUnit.SECONDS)
                    .toBlocking()
                    .firstOrDefault(new SecAuthResult(SecAuthResult.ERR, null, null));
            secSsubject = null;
            int ret = secAuthResult.getRetcode();
            if (ServiceResult.Success == ret) {
                byte[] scriptresult = secAuthResult.getScriptresult();
                outScriptdata[0] = (byte) scriptresult.length;
                System.arraycopy(scriptresult, 0, outScriptdata, 1, scriptresult.length);
                outTC = secAuthResult.getTc();
            }
            return secAuthResult.getRetcode();
        }
        return SecAuthResult.ERR;

    }

    public void onEventMainThread(SecAuthResult secAuthResult) {
        log.debug("onEventMainThread");
        secSsubject.onNext(secAuthResult);
        secSsubject.onCompleted();
    }


    /**
     * 电签
     *
     * @param data 入参 随机数据
     * @return 图片路径
     */
    public String EcleSignForC(String data) {
//        Log.d("zbq", "EcleSignForC executed");
//        HashMap map = new HashMap();
//
//        map.put("EcleSignForC", data);
//        EventBus.getDefault().post(map);
//
//        subject = AsyncSubject.create();
//        LocalEvent ret = subject
//                .serialize()
//                .take(60, TimeUnit.SECONDS)
//                .toBlocking()
//                .firstOrDefault(LocalEvent.FAIL);
//        subject = null;
//
//        EventBus.getDefault().unregister(this);
//
//        if (ret == LocalEvent.OK)
//            return GestureActivity.getPath();
//        else {
//            return null;
//        }
        return null;
    }

    public void onEventBackgroundThread(GestureResult event) {
        Log.d("zbq", "onEventBackgroundThread executed");
        if (subject == null) return;
        subject.onNext(event.hasSuccess() ? LocalEvent.OK : LocalEvent.FAIL);
        subject.onCompleted();
    }

    private enum LocalEvent {
        OK,
        FAIL,
        END
    }

    /**
     * 生成一、二维码图片
     *
     * @param data 入参
     * @param type 入参 1：一维 2：二维
     * @return 图片路径
     */
    public static String BarcodeForC(String data, int type) {
        return null;
    }

    public static boolean setPrintScript(String printData) {

        PrintScript printer = new PrintScript();
        if (printer.startPrint(printData) == 1)
            return true;
        else
            return false;
    }

    public byte[] getTradeCount(String tradeID){
        int count;
        Dboperation tradeDB;
        HashMap tradeInfo = new HashMap();

        try {
            tradeDB = (Dboperation) DbProxyFactory.getProxy(DbRecordOperation.getInstance());
            tradeInfo = tradeDB.settleResult(tradeID);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(tradeInfo == null)
            count = 0;
        else
            count = (int)tradeInfo.get("Count");
        Log.e("tradeInfo","22222222222222222222222222222222222222222  <"+count+">");

        return (""+count).getBytes();
    }

    public byte[] getTradeAmount(String tradeID){
        long amount;
        StringBuffer amountStr;
        Dboperation tradeDB;
        HashMap tradeInfo = new HashMap();

        try {
            tradeDB = (Dboperation) DbProxyFactory.getProxy(DbRecordOperation.getInstance());
            tradeInfo = tradeDB.settleResult(tradeID);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(tradeInfo == null)
            amount = 0;
        else
            amount = (long)tradeInfo.get("Amount");
        Log.e("tradeInfo","22222222222222222222222222222222222222222 <"+amount+">");

        amountStr = new StringBuffer(""+amount);
        if(amountStr.length() >= 3){
            amountStr.insert(amountStr.length() -2, '.');
        }
        else if(amountStr.length() == 2){
            amountStr.insert(0, "0.");
        }
        else if(amountStr.length() == 1){
            amountStr.insert(0, "0.0");
        }
        else {
            amountStr = new StringBuffer("0.00");
        }
        return amountStr.toString().getBytes();
    }

    public byte[] getTradeCount(String tradeID, int cardFlag){
        int count;
        Dboperation tradeDB;
        HashMap tradeInfo = new HashMap();

        if(cardFlag != 0 && cardFlag != 1)
            return null;

        try {
            tradeDB = (Dboperation) DbProxyFactory.getProxy(DbRecordOperation.getInstance());
            tradeInfo = tradeDB.settleStatistics(tradeID, ""+cardFlag);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(tradeInfo == null)
            count = 0;
        else
            count = (int)tradeInfo.get("Count");
        //Log.e("tradeInfo","22222222222222222222222222222222222222222  <"+count+">");

        return (""+count).getBytes();
    }

    public byte[] getTradeAmount(String tradeID, int cardFlag){
        long amount;
        StringBuffer amountStr;
        Dboperation tradeDB;
        HashMap tradeInfo = new HashMap();

        if(cardFlag != 0 && cardFlag != 1)
            return null;

        try {
            tradeDB = (Dboperation) DbProxyFactory.getProxy(DbRecordOperation.getInstance());
            tradeInfo = tradeDB.settleStatistics(tradeID, ""+cardFlag);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if(tradeInfo == null)
            amount = 0;
        else
            amount = (long)tradeInfo.get("Amount");
        //Log.e("tradeInfo","22222222222222222222222222222222222222222 <"+amount+">");

        amountStr = new StringBuffer(""+amount);
        if(amountStr.length() >= 3){
            amountStr.insert(amountStr.length() -2, '.');
        }
        else if(amountStr.length() == 2){
            amountStr.insert(0, "0.");
        }
        else if(amountStr.length() == 1){
            amountStr.insert(0, "0.0");
        }
        else {
            amountStr = new StringBuffer("0.00");
        }
        return amountStr.toString().getBytes();
    }

    public int setAidPara(String[]  aidTlv, int num){

        List<String> aid = new ArrayList<String>();

        for(int i = 0; i < num; i++){
            aid.add(aidTlv[i]);
        }
        try{
            emvHandler.setAidParaTlvList(aid);
        }catch (RemoteException e){
            e.printStackTrace();
 //           return -1;
        }

        return 0;
    }

    public int setAidPara(String  aidTlv, int operation){

        List<String> aid = new ArrayList<String>();
        log.debug("setAidPara"+ aidTlv);
        aid.add(aidTlv);
        try{
            emvHandler.setAidParaTlvList(aid);
        }catch (RemoteException e){
            e.printStackTrace();
            return -1;
        }

        return 0;
    }

    public int setCapkPara(String[] capkTlv, int num){

        List<String> capk = new ArrayList<String>();

        for(int i = 0; i < num; i++){
            capk.add(capkTlv[i]);
        }

        try{
            emvHandler.setCAPKTlvList(capk);
        }catch (RemoteException e){
            e.printStackTrace();
            return -1;
        }

        return 0;
    }

    public int setCapkPara(String capkTlv, int operation){

        List<String> capk = new ArrayList<String>();
        log.debug("setCapkPara"+ capkTlv);
        capk.add(capkTlv);

        try{
            emvHandler.setCAPKTlvList(capk);
        }catch (RemoteException e){
            e.printStackTrace();
            return -1;
        }

        return 0;
    }
}