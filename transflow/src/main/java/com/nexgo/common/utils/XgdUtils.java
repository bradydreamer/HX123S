package com.nexgo.common.utils;


import android.text.TextUtils;

import com.nexgo.common.ByteUtils;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.List;


/**
 * 将字符串转换为TLV对象
 */
public abstract class XgdUtils {
    private static Logger log = LoggerFactory.getLogger("XgdUtils");
    public final static byte[] SendEcho =
            new byte[]{0x04, 0x04, 0x04, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x07, 0x1d, 0x0a, 0x03, 0x31, 0x6b};
    private static final String TAG = "XGDUtils";


    //串口握手
    public synchronized static int echoTest(int times) {

        //if (ByteUtils.lockReceiverKeyValueThread() < 0) return -1;

        int i = times;
        while (true) {
            i--;
            log.error("向pos发送串口握手:" + ByteUtils.byteArray2HexStringWithSpace(SendEcho));
//            int out = USBConnect.getInstance().write(SendEcho);
//            if (out < 0) {
//                log.error("out<0");
//                return -1;
//            }
//            USBConnect.getInstance().setEchoTest(true);
//            byte[] buffer0 = new byte[64];
//            int read0 = USBConnect.getInstance().read(buffer0);
//            if (read0 > 0) {
//                byte[] readResult0 = new byte[read0];
//                System.arraycopy(buffer0, 0, readResult0, 0, read0);
//                com.nexgo.oaf.datahub.util.log.error("接收pos返回数据:" + ByteUtils.byteArray2HexStringWithSpace(readResult0));
//
//                com.nexgo.oaf.datahub.util.log.error("串口握手成功");
//                USBConnect.getInstance().setEchoTest(false);
//                return 0;
//            }
//            if (i <= 0) {
//                log.error("串口握手失败read");
//                USBConnect.getInstance().setEchoTest(false);
//                return -1;
//            }
        }
    }

    /**
     * 组TLV格式的数据
     *
     * @param tag    标签值
     * @param length 数据长度
     * @param data   数据
     * @return TLV格式的byte数组
     * @author lijinniu
     * @since 2015年4月13日
     */
    public static byte[] getTLVData(String tag, int length, byte[] data) {
        byte[] tmpData = new byte[1024];
        int len = 0;

        byte[] tagData = ByteUtils.hexString2ByteArray(tag);

        System.arraycopy(tagData, 0, tmpData, len, tagData.length);
        len += tagData.length;

        /*、、----
         * 子域长度（即L本身）的属性也为bit，占1～3个字节长度。具体编码规则如下：
           a)  当 L 字段最左边字节的最左 bit 位（即 bit8）为 0，表示该 L 字段占一个字节，
           它的后续 7个 bit 位（即 bit7～bit1）表示子域取值的长度，采用二进制数表示子域取值长度的十进制数。
           例如，某个域取值占 3 个字节，那么其子域取值长度表示为“00000011”。
           所以，若子域取值的长度在 1～127 字节之间，那么该 L 字段本身仅占一个字节。

           b)  当 L 字段最左边字节的最左 bit 位（即 bit8）为 1，表示该 L 字段不止占一个字节，
           那么它到底占几个字节由该最左字节的后续 7 个 bit 位（即 bit7～bit1）的十进制取值表示。
           例如，若最左字节为 10000010，表示 L 字段除该字节外，后面还有两个字节。其后续字节的十进制
取值表示子域取值的长度。例如，若 L 字段为“1000 0001 1111 1111”，表示该子域取值占255 个字节。
所以，若子域取值的长度在 127～255 字节之间，那么该 L 字段本身需占两个字节。
10000010 00000001 00000000-10000010 11111111 11111111
    	  */

        if (length > 127 && length <= 255) {//10000001 10000000---10000001 11111111
            tmpData[len++] = (byte) 0x81;
        }

        if (length > 255 && length <= 65535) {//10000010 00000001 00000000--10000010 11111111 11111111
            tmpData[len++] = (byte) 0x82;
            byte[] tmpLen = ByteUtils.short2ByteArrayHigh((short) length);
            tmpData[len++] = tmpLen[0];
            tmpData[len++] = tmpLen[1];
        }
        tmpData[len++] = (byte) length;

        System.arraycopy(data, 0, tmpData, len, length);
        len += length;

        //返回的数据
        byte[] tlvData = new byte[len];
        System.arraycopy(tmpData, 0, tlvData, 0, len);
        return tlvData;
    }

    /**
     * @value:普通的字符串，计算出tlv格式的数据
     * @author wanggaozhuo
     * @since 20150128
     */
    public static byte[] getTLVData(String tag, byte[] value) {
        byte[] data = null;
        try {
            data = getTLVData(tag, value.length, value);
        } catch (Exception e) {
            e.printStackTrace();
        }
        return data;
    }

    public static List<String> decodingTLV(String str, String tTag) {

        if (str == null || str.length() % 2 != 0) {
            return new ArrayList<>();
        }

//        List<String[]> ls = new ArrayList<String[]>();
        String vv = "";
        List<String> list = new ArrayList<>();
        for (int i = 0; i < str.length(); ) {
            try {

                String tag = str.substring(i, i = i + 2);
                // extra byte for TAG field
                if ((Integer.parseInt(tag, 16) & 0x1F) == 0x1F) {
                    tag += str.substring(i, i = i + 2);
                }
                String len = str.substring(i, i = i + 2);
                int length = Integer.parseInt(len, 16);
                // more than 1 byte for length
                if (length > 128) {//临界值，当是128即10000000时，长度还是一位，而不是两位
                    int bytesLength = length - 128;
                    len = str.substring(i, i = i + (bytesLength * 2));
                    length = Integer.parseInt(len, 16);
                }
                length *= 2;
                System.out.println("length=" + length);
                String value = str.substring(i, i = i + length);
                System.out.println("tag:" + tag + " len:" + len + " value:" + value);
//                ls.add(new String[] {tag, len, value});
                if (tTag.equalsIgnoreCase(tag)) {
                    vv = value;
                    list.add(value);

                }
            } catch (NumberFormatException e) {
                return list;
                //throw new RuntimeException("Error parsing number", e);
            } catch (IndexOutOfBoundsException e) {
                return list;
                //throw new RuntimeException("Error processing field", e);
            }
        }
        return list;
    }






    public static String getAmountToShow(byte[] amount){
        String ssAmount = new String(amount);
        long iAmount = Long.parseLong(ssAmount);
        float fAmount = (float) (iAmount/100.0);
        String sAmount = String.valueOf(fAmount);
        return sAmount;
    }

    /**
     * 检测MMDD格式日期是否有效
     * @param str
     * @return boolean
     */
    public static boolean isValidDate(String str) {
        boolean convertSuccess = true;
        SimpleDateFormat format = new SimpleDateFormat("MMdd");
        try {
            // 设置lenient为false. 否则SimpleDateFormat会比较宽松地验证日期，比如2007/02/29会被接受，并转换成2007/03/01
            format.setLenient(false);
            format.parse(str);
        } catch (ParseException e) {
            convertSuccess = false;
        }
        return convertSuccess;
    }
    /**
     * @param amount  转换为12位金额  eg: String 0.01  ---->  String 000000000001
     * @return
     */
    public static String getFormatAmount(String amount) {
        String amt = amount;
        if (TextUtils.isEmpty(amt)) amt = "0";
        NumberFormat format = NumberFormat.getNumberInstance();
        format.setGroupingUsed(false);
        format.setMinimumIntegerDigits(12);
        DecimalFormat df = new DecimalFormat("0.00");
        String tmp = df.format(Double.valueOf(amt)).replace(".", "");
        return format.format(Long.valueOf(tmp));
    }


}
