package com.nexgo.db;

import java.lang.reflect.Proxy;

/**
 * Created by HGD on 2016/3/12.
 *
 */
public class DbProxyFactory
{
    public static Object getProxy(Object target)throws Exception
    {
            //创建一个Dbhandler对象
        Dbhandler dbhandler=new Dbhandler();
        dbhandler.SetTarget(target);
        return Proxy.newProxyInstance(target.getClass().getClassLoader(),target.getClass().getInterfaces(),dbhandler);
    }
}
