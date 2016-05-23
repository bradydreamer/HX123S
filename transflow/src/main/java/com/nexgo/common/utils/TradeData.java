package com.nexgo.common.utils;

import android.os.RemoteException;
import android.text.TextUtils;

import com.nexgo.common.ByteUtils;
import com.nexgo.common.GlobalHolder;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvHandler;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Arrays;
import java.util.List;

/**
 * Created by xiaox on 16/1/20.
 */
public class TradeData {
    private Logger log = LoggerFactory.getLogger(this.getClass().getName());
    private String amount;
    private String tk1;
    private String tk2;
    private String tk3;
    private int swipeType;
    private byte[] pin;
    private String cardNo;
    private List<String> appNameList;
    private byte[] cardSn;
    private DeviceServiceEngine deviceServiceEngine;

    private TradeData() {
        deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
    }

    public void clear() {
        amount = null;
        tk1 = null;
        tk2 = null;
        tk3 = null;
        swipeType = 0;
        pin = null;
        cardNo = null;
        appNameList = null;
        cardSn = null;
    }

    public boolean isBypass() {
        return pin == null;
    }


    public byte[] getCardSn() {
        return cardSn;
    }

    public void setCardSn(byte[] cardSn) {
        this.cardSn = cardSn;
    }

    public List<String> getAppNameList() {
        return appNameList;
    }

    public void setAppNameList(List<String> appNameList) {
        this.appNameList = appNameList;
    }

    public String getFormatAmount() {
        String amt = amount;
        if (TextUtils.isEmpty(amt)) amt = "0";
        NumberFormat format = NumberFormat.getNumberInstance();
        format.setGroupingUsed(false);
        format.setMinimumIntegerDigits(12);
        DecimalFormat df = new DecimalFormat("0.00");
        String tmp = df.format(Double.valueOf(amt)).replace(".", "");
        return format.format(Long.valueOf(tmp));
    }

    public void setCardNo(String cardNo) {
        this.cardNo = cardNo;
    }

    public byte[] getPin() {
        return pin;
    }

    public void setPin(byte[] pin) {
        this.pin = pin;
    }

    private static TradeData instance = new TradeData();

    public static TradeData getInstance() {
        return instance;
    }

    public String getAmount() {
        return amount;
    }

    public void setAmount(String amount) {
        this.amount = amount;
    }

    /*获取磁道数据仅供IC卡流程使用*/

    public String getTk2() {
        log.debug("jump into getTk2");
            byte[] data;
            EmvHandler emvHandler = null;

            try {
                emvHandler = deviceServiceEngine.getEmvHandler();
                data = emvHandler.getTlvs(new byte[]{(byte) 0x57}, EmvDataSource.FROMKERNEL);
                if (data == null || data.length == 0) {
                    log.debug("57 null");
                    return null;
                }
                tk2 = ByteUtils.byteArray2HexString(data).toUpperCase();
                if (tk2.endsWith("F")) {
                    tk2 = tk2.substring(0, tk2.length() - 1);
                }
                return tk2.replace("=", "D");
            } catch (RemoteException e) {
                e.printStackTrace();
            }
            return null;
    }
    public String getCardNo() {
        if (!TextUtils.isEmpty(cardNo)) {
            return cardNo;
        } else if (swipeType == 0) {
            if (tk2 != null) {
                String str = tk2;
                str = str.toUpperCase().replace('=', 'D');
                if (str.contains("D")) {
                    return str.substring(0, str.indexOf("D"));
                }
            }
        } else {
            try {
                byte[] data;
                String cardNo;
                EmvHandler emvHandler = deviceServiceEngine.getEmvHandler();
                data = emvHandler.getTlvs(new byte[]{(byte) 0x5a}, EmvDataSource.FROMKERNEL);
                if (data == null || data.length == 0) {
                    log.debug("5a null");
                    data = emvHandler.getTlvs(new byte[]{(byte) 0x57}, EmvDataSource.FROMKERNEL);
                    if (data == null || data.length == 0) {
                        log.debug("57 null");
                        return null;
                    }
                    cardNo = ByteUtils.byteArray2HexString(data);
                    cardNo = cardNo.toUpperCase().replace('=', 'D');
                    if (cardNo.contains("D")) {
                        cardNo = cardNo.substring(0, cardNo.indexOf("D"));
                    }
                } else {
                    cardNo = ByteUtils.byteArray2HexString(data);
                }
                cardNo = cardNo.toUpperCase();
                if (cardNo.endsWith("F")) {
                    cardNo = cardNo.substring(0, cardNo.length() - 1);
                }
                log.debug("cardNo {}", cardNo);
                return cardNo;
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        return null;
    }

    public int getSwipeType() {
        return swipeType;
    }

    public void setSwipeType(int swipeType) {
        this.swipeType = swipeType;
    }

    public String getCardExpiredDate() {
        if (swipeType == 0) {
            tk2 = tk2.toUpperCase().replace('=', 'D');
            if (!tk2.contains("D")) return null;
            int idx = tk2.indexOf("D");
            return tk2.substring(idx + 1, idx + 1 + 4);
        } else {
            try {
                EmvHandler emvHandler = deviceServiceEngine.getEmvHandler();
                byte[] tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x5f, (byte) 0x24}, EmvDataSource.FROMKERNEL);
                if (tlvs != null && tlvs.length != 0) {
                    return ByteUtils.byteArray2HexString(Arrays.copyOfRange(tlvs, 1, tlvs.length));
                }
            } catch (RemoteException e) {
                e.printStackTrace();
            }
        }
        return null;
    }
}
