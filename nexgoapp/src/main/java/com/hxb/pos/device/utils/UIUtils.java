package com.hxb.pos.device.utils;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;

import static android.content.Context.WINDOW_SERVICE;

/**
 * 作者:liuting on 2016/3/18 14:08
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.com.hxbsdk.utils
 * TODO: ui界面的功能辅助类
 */
public class UIUtils {
    /**
     * @return  获取设备的屏幕宽度
     */
    public static int getScreenWidth(Context context){
        WindowManager manager= (WindowManager) context.getSystemService(WINDOW_SERVICE);
        return manager.getDefaultDisplay().getWidth();
    }

    /**
     * @return  获取设备的屏幕高度
     */
    public static int getScreenHeight(Context context){
        WindowManager manager= (WindowManager) context.getSystemService(WINDOW_SERVICE);
        return manager.getDefaultDisplay().getHeight();
    }

    /**
     * @param activity
     *  关闭系统软键盘
     */
    public static void hideSoftKeyboard(Activity activity) {
        View view = activity.getWindow().peekDecorView();
        if (view != null) {
            InputMethodManager inputmanger = (InputMethodManager)activity.getSystemService(Context.INPUT_METHOD_SERVICE);
            inputmanger.hideSoftInputFromWindow(view.getWindowToken(), 0);
        }
    }

}
