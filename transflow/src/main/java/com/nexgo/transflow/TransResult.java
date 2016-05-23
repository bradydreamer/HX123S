package com.nexgo.transflow;

/**
 * Created by xiaox on 16/3/12.
 */
public enum TransResult {
    /**
     * 交易成功，无论RespCode是不是00，只要卡片不拒绝交易，都应该返回APPROVED。
     */
    APPROVED,
    /**
     * 终端终止交易（除了卡片拒绝交易和用户主动取消交易外，目前都定义为终端终止交易）
     */
    TERMINATED,
    /**
     * 卡片拒绝交易
     */
    DECLINED,
    /**
     * 用户主动取消
     */
    CANCEL,
    /**
     * 交易错误
     */
    ERROR,
    /**
     * 脱机交易接受
     */
    OFFLINE_ACCEPTED,
    /**
     * 超时
     */
    TIMEOUT,
    /**
     * 刷卡出错,输密码出错,不确认卡号
     */
    DEVICE_ERR,
    /**
     * 无交易记得
     */
    SETTLE_NOTRECORD,



}