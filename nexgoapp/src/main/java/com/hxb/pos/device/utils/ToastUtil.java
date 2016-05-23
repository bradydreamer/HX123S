package com.hxb.pos.device.utils;

import android.content.Context;
import android.widget.Toast;

/**
 * 作者:liuting on 2016/3/10 14:22
 * 邮箱:liuting@xinguodu.com
 * 项目名：
 * 包名：
 * TODO:单例吐司
 */
public class ToastUtil {
    private static Toast toast;
    public static void showToast(Context context, String msg){
        if (toast==null){
            toast=Toast.makeText(context,msg,Toast.LENGTH_SHORT);
        }
        toast.setText(msg);
        toast.show();
    }
}
