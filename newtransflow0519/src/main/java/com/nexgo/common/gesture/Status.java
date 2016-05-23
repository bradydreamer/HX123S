package com.nexgo.common.gesture;

/**
 * Created by zhanbiqiang
 * Email: zhanbiqiang@xinguodu.com
 * Date: 2016/3/17 21:02
 * Describe:
 */
public class Status {
    public static int OK = 0;
    public static int ERROR = 1;
    private int status;

    public Status(int status) {
        this.status = status;
    }

    public Status() {
        this.status = OK;
    }

    public int getStatus() {
        return this.status;
    }

    public boolean hasSuccess() {
        return this.status == OK;
    }

    protected void setStatus(int status) {
        this.status = status;
    }
}
