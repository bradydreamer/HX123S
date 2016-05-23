package com.hxb.pos.device.utils;

import android.content.Context;

/**
 * 作者:liuting on 2016/3/10 14:22
 * 邮箱:liuting@xinguodu.com
 * 项目名：
 * 包名：
 * TODO: dp和px的单位换算
 */
public class DesityUtil {
	 /** 
     * 根据手机的分辨率从 dip 的单位 转成为 px(像素) 
     */  
    public static int dip2px(Context context, float dpValue) {  
        final float scale = context.getResources().getDisplayMetrics().density;  
        return (int) (dpValue * scale + 0.5f);  
    }  
  
    /** 
     * 根据手机的分辨率从 px(像素) 的单位 转成为 dp 
     */  
    public static int px2dip(Context context, float pxValue) {  
        final float scale = context.getResources().getDisplayMetrics().density;  
        return (int) (pxValue / scale + 0.5f);  
    }  
}