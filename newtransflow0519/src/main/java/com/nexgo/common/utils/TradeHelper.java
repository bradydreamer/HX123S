package com.nexgo.common.utils;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.RemoteException;
import android.preference.PreferenceManager;
import android.text.TextUtils;

import com.nexgo.common.ByteUtils;
import com.nexgo.common.GlobalHolder;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvHandler;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.nio.ByteBuffer;
import java.util.Arrays;

/**
 * Created by xiaox on 16/1/18.
 */
public class TradeHelper {
    private static Logger log = LoggerFactory.getLogger("TradeHelper");

    public static String maskCardNo(String cardNo) {
        if (TextUtils.isEmpty(cardNo) || cardNo.length() < 13) return null;
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append(cardNo.substring(0, 6));
        for (int i = 0; i < cardNo.length() - 10; i++) {
            stringBuilder.append("*");
        }
        stringBuilder.append(cardNo.substring(cardNo.length() - 4, cardNo.length()));
        return stringBuilder.toString();
    }

    public static byte[] form55Field() {
        DeviceServiceEngine deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
        try {
            ByteBuffer byteBuffer = ByteBuffer.allocate(512);
            byte[] tlvs;
            EmvHandler emvHandler = deviceServiceEngine.getEmvHandler();
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x26}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f26", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x27}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f27", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x10}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f10", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x37}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f37", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x36}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f36", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x95}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("95", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9a}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9a", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9c}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9c", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x02}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f02", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x2a}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f2a", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x82}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("82", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x1a}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f1a", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x03}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f03", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x33}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f33", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x34}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f34", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x35}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f35", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x1e}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f1e", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x84}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("84", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x09}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f09", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x41}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f41", tlvs));
            }
            tlvs = emvHandler.getTlvs(new byte[]{(byte) 0x9f, (byte) 0x63}, EmvDataSource.FROMKERNEL);
            if (tlvs != null) {
                byteBuffer.put(ByteUtils.getTLVData("9f63", tlvs));
            }
            return Arrays.copyOfRange(byteBuffer.array(), 0, byteBuffer.position());
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return null;
    }

    public static boolean iswithIc(String tk2) {
        if (TextUtils.isEmpty(tk2)) return false;
        tk2 = tk2.toUpperCase().replace('=', 'D');
        if (!tk2.contains("D")) return false;
        int idx = tk2.indexOf("D");
        return tk2.charAt(idx + 5) == '2' || tk2.charAt(idx + 5) == '6';
    }

    public static byte[] getPan(String cardNo) {
        if (cardNo == null || cardNo.length() < 13 || cardNo.length() > 19) return null;
        byte[] pan = new byte[8];
        log.debug("pan {}", cardNo.substring(cardNo.length() - 13, cardNo.length() - 13 + 12));
        byte[] tmp = ByteUtils.str2Bcd(cardNo.substring(cardNo.length() - 13, cardNo.length() - 13 + 12));
        System.arraycopy(tmp, 0, pan, 2, tmp.length);
        return pan;
    }


    public static void increaseVoucherNo(Context context) {
        String voucherNo = PreferenceManager.getDefaultSharedPreferences(context).getString("voucherno_text", "");
        if (TextUtils.isEmpty(voucherNo)) {
            voucherNo = "000001";
        } else {
            int i = Integer.valueOf(voucherNo);
            i++;
            voucherNo = String.format("%06d", i);
        }
        SharedPreferences.Editor editor = PreferenceManager.getDefaultSharedPreferences(context).edit();
        editor.putString("voucherno_text", voucherNo);
        editor.apply();
    }

    public static String getTransIp(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("ip_text", "");
    }

    public static int getTransPort(Context context) {
        return Integer.valueOf(PreferenceManager.getDefaultSharedPreferences(context).getString("port_text", ""));
    }

    public static String getTpdu(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("tpdu_text", "");
    }

    public static int getMkeyIdx(Context context) {
        String str = PreferenceManager.getDefaultSharedPreferences(context).getString("key_idx_text", "");
        if (TextUtils.isEmpty(str)) {
            return 0;
        } else {
            return Integer.valueOf(str);
        }
    }

    public static int getMkeyType(Context context) {
        String str = PreferenceManager.getDefaultSharedPreferences(context).getString("key_type_list", "");
        if (TextUtils.isEmpty(str)) {
            return 0;
        } else {
            return Integer.valueOf(str);
        }
    }

    public static boolean getSupportTdk(Context context) {
        String str = PreferenceManager.getDefaultSharedPreferences(context).getString("key_tdk_list", "");
        if (TextUtils.isEmpty(str)) {
            return false;
        } else {
            return Integer.valueOf(str) == 1;
        }
    }

    public static String getVoucherNo(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("voucherno_text", "");
    }

    public static String getTerminalNo(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("terminalno_text", "");
    }

    public static String getMerchantNo(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("merchantno_text", "");
    }

    public static String getBatchNo(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("batchno_text", "");
    }

    public static String getMerchantName(Context context) {
        return PreferenceManager.getDefaultSharedPreferences(context).getString("merchant_name_text", "");
    }
}
