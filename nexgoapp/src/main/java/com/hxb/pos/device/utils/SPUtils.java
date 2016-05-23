package com.hxb.pos.device.utils;

import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;

/**
 * 	本地保存密码用户名的专门工具类
 *
 */
public class SPUtils
{

	public static void setBoolean(SharedPreferences sp,String key,Boolean value) {
		Editor editor=sp.edit();
		editor.putBoolean(key, value);
		editor.apply();
	}
	public static void setString(SharedPreferences sp,String key,String value) {
		Editor editor=sp.edit();
		editor.putString(key, value);
		editor.apply();
	}
	/**
	 * @return 取出指定sp文件中的key的值，若没有设定过或该sp文件不存在，默认为false
	 */
	public static Boolean getBoolean(SharedPreferences sp,String key) {
		return getBoolean(sp, key, false);
	}
	/**
	 * @return 取出指定sp文件中的key的值，若没有设定过或该sp文件不存在，设置默认值defaulValue。
	 */
	public static Boolean getBoolean(SharedPreferences sp,String key,Boolean defaulValue) {
		return sp.getBoolean(key, defaulValue);
	}
	/**
	 * @return 取出指定sp文件中的key的值，若没有设定过或该sp文件不存在，约定为null
	 */
	public static String getString(SharedPreferences sp,String key) {
		return sp.getString(key,null);
	}
}
