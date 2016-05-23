package com.hxb.pos.device.utils;

import android.os.RemoteException;

import com.nexgo.common.ByteUtils;
import com.nexgo.transflow.IProcess;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/4/26 10:50
 * Describe:
 */
public class DataExchangeControl {
    boolean SuccessRecevflag = false;
    Logger logger = LoggerFactory.getLogger("DataExchangeControl");
    private byte[] data;
    private byte[] buffer;
    IProcess iProcess;
    DataExchangeThread dataExchangeThread;
    ExchangeResultThread exchangeResultThread;

    public DataExchangeControl(byte[] sendData,IProcess iProcess){
        data = sendData;
        buffer = new byte[2048];
        this.iProcess = iProcess;
        dataExchangeThread = new DataExchangeThread();
        exchangeResultThread = new ExchangeResultThread();
        dataExchangeThread.start();
        exchangeResultThread.start();
    }

    private class DataExchangeThread extends Thread{
        @Override
        public void run() {
            synchronized (dataExchangeThread) {
                int len = 0;
                logger.debug("发给华夏sdk 数据 = {}", Arrays.toString(data));
                try {
                    buffer = AppGlobal.getInstance().getDataExchangeAidl().exchangeData(data);
                    len = ByteUtils.byte2int(buffer[0], buffer[1]);
                    logger.debug("接收报文的长度 = {}", len);
                    if (len > 0) {
                        SuccessRecevflag = true;
                    }
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
                try {
                    dataExchangeThread.wait();
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }

                logger.debug("接收到华夏sdk 数据 = {} ", Arrays.toString(Arrays.copyOfRange(buffer, 0, len + 2)));
                iProcess.onSetOnlineResponse(Arrays.copyOfRange(buffer, 0, len + 2), 0);
            }
        }
    }

    private class ExchangeResultThread extends Thread{
        @Override
        public void run() {
            while (true){
                if(SuccessRecevflag) {
                    SuccessRecevflag = false;
                    dataExchangeThread.notify();
                    return;
                }
            }
        }
    }
}
