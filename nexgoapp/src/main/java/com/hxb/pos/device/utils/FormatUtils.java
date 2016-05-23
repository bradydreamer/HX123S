package com.hxb.pos.device.utils;

import java.util.Locale;

/**
 * 作者:liuting on 2016/3/25 16:47
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.com.hxbsdk.utils
 * TODO:  格式化工具类
 */
public class FormatUtils {
    public static String getFormatAmt(long amt){
        return String.format(Locale.getDefault(),"%d.%02d",amt/100,amt%100);
    }
    public static String getFormatTime(String time){
       StringBuilder stringBuilder=new StringBuilder(time);
        if(stringBuilder.length()!=0){
            stringBuilder.insert(2,":").insert(5,":");
            return stringBuilder.toString();
        }else{
            throw new RuntimeException("time should not be null!");
        }


    }
    public static String getFormatDate(String date){
        if(date!=null){
            StringBuilder stringBuilder=new StringBuilder(date);
            stringBuilder.insert(2,"-");
            return stringBuilder.toString();
        }else
            throw new RuntimeException("date should not be null!");
    }
    public static String getFormatCardNum(String cardNum){
        if(cardNum==null)
            throw new RuntimeException("cardNum should not be null!");
        if(cardNum.trim().length()<10)
            throw new RuntimeException("cardNum is not legal!");
        char[] temp=cardNum.toCharArray();
        for(int i=6;i<temp.length-4;i++){
            temp[i]='*';
        }
        return new String(temp);


    }
}
