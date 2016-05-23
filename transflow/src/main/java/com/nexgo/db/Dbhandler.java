package com.nexgo.db;

import android.util.Log;

import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;

/**
 * Created by HGD on 2016/3/12.
 */
public class Dbhandler implements InvocationHandler {
    private Object target;

    public void SetTarget(Object object) {
        this.target = object;
    }

    @Override
    public Object invoke(Object proxy, Method method, Object[] args) throws Exception {
        Log.d("hgd", "调用invokebefore");
        Object result = method.invoke(this.target, args);
        Log.d("hgd", "调用invokeafter");
        return result;
    }
}
