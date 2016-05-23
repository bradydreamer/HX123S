package com.nexgo.common;

import android.content.SharedPreferences;

import com.nexgo.smartpos.api.engine.DeviceServiceEngine;


/**
 * Created by xiaox on 16/1/15.
 */
public class GlobalHolder {
    private DeviceServiceEngine deviceServiceEngine;
    private static GlobalHolder singletion = new GlobalHolder();
    private SharedPreferences prefs;

    public static GlobalHolder getInstance() {
        return singletion;
    }

    public DeviceServiceEngine
    getDeviceServiceEngine() {
        return deviceServiceEngine;
    }

    public void setDeviceServiceEngine(DeviceServiceEngine deviceServiceEngine) {
        this.deviceServiceEngine = deviceServiceEngine;
    }

    public SharedPreferences getPrefs() {
        return prefs;
    }

    public void setPrefs(SharedPreferences prefs) {
        this.prefs = prefs;
    }
}
