package com.hxb.pos.device.utils;

/**
 * 作者:liuting on 2016/3/15 18:40
 * 邮箱:liuting@xinguodu.com
 * 项目名：
 * 包名：
 * TODO:  常量类
 */
public class Constant {
    /**
     * app第一次启动标志
     */
    public static final String APP_FirstInstall="APP_FirstInstall";
    /**
     * 系统参数配置文件的名字
     */
    public static final String APP_SETTING="applicationsetting";
    //商户参数中的字段
    /**
     * 商户参数中的商户号字段
     */
    public static final String MERCHANT_ID="FF9012";
    /**
     *商户参数中的商户名称字段
     */
    public static final String MERCHANT_NAME="FF8018";
    /**
     *商户参数中的商户英文名字段
     */
    public static final String MERCHANT_ENGNAME="merchant_engname";
    /**
     *商户参数设置中的终端号字段
     */
    public static final String TERMINAL_ID="FF9054";
    /**
     * 商户参数设置中的应用名称字段
     */
    public static final String APPLICATION_NAME="application_name";
    //交易管理设置中的字段
    /**
     * 交易管理设置中交易开关控制中传统消费字段
     */
    public static final String TRANSSWITCH_TRIDITIONALCONSUME="tridition_consume";
    /**
     * 交易管理设置中交易开关控制中接触式电子现金消费字段
     */
    public static final String TRANSSWITCH_CONELECCASH_CONSUME="contact_electroiccash";
    /**
     * 交易管理设置中交易开关控制中是否开启分期功能字段
     */
    public static final String TRANSSWITCH_INSTALLMENT="launchinstallment";
    /**
     * 交易管理设置中交易开关控制中是否开启积分功能字段
     */
    public static final String TRANSSWITCH_INTEGRATION="launchintegration";
    /**
     * 交易管理设置中交易开关控制中是否开启预约功能字段
     */
    public static final String TRANSSWITCH_RESERVATION="launchreservation";
    /**
     * 交易管理设置中交易开关控制中是否开启订购功能字段
     */
    public static final String TRANSSWITCH_SUBSCRIPTION="launchsubscription";
    /**
     * 交易管理设置中交易开关控制中是否开启其它功能字段
     */
    public static final String TRANSSWITCH_OTHER="launchother";
    /**
     * 交易管理设置中撤销交易是否需要刷卡字段
     */
    public static final String SWIPECARD_CANCEL="FF8023";
    /**
     * 交易管理设置中撤销交易是否需要输密字段
     */
    public static final String ENTERPASSWORD_CANCEL="FF8024";
    /**
     * 交易管理设置中结算后自动签退字段
     */
    public static final String AUTOSIGNOUT_AFTERSETTLEMENT="FF8046";
    /**
     * 交易管理设置中是否提示打印明细字段
     */
    public static final String PRINTDETAILS_HINT="printdetails_hint";

    public static final String Rfchannel="FF8030";//1-电子现金0-联机借贷记
    public static final String IssuporFlashcard="FF8031";//
    public static final String FlashcardDealtime="FF803A";//
    public static final String FlashcardLifetime="FF803C";//
    public static final String IsRfFast="FF8054";//
    public static final String BinA="FF8055";//
    public static final String BinB="FF8056";//
    public static final String CDCVM="FF8057";//
    public static final String QPS_NoPWDLimAmonut="FF8058";//
    public static final String NoSignLimAmonut="FF8059";//
    public static final String NoPWDLimAmonut="FF805A";//

    /**
     * 系统交易参数设置
     */
    public static final String Voucher="FF9003";
    public static final String Batch="FF9022";
    public static final String PrintPaper="FF9021";  //打印联数

    /**
     * 通讯管理设置
     */

    public static final String Ispredial="Ispredial";
    public static final String IssupportCDMA="IsSupportCDMA";
    public static final String IssupportEther="IssupportEther";
    public static final String IssupportSerial="IssupportSerial";
    public static final String IsupportTelnet="IsupportTelnet";

    public static final String tpdu="FF8011";
    public static final String GprsIP1="FF8012";
    public static final String GprsIP2="GprsIP2";
    public static final String Gprsport1="FF8013";
    public static final String Gprsport2="Gprsport2";
    public static final String TradeTimeout="FF8019";
    public static final String TelOutsideNO="FF8020";
    /**
     * 秘钥管理设置
     */
    public static final String TmkIndex="FF8016";
    public static final String Tmk="FF8022";
    public static final String EncryType="FF8017";
    public static final String IsTKEncry="FF8015";
    public static final String MACtype="FF8010";
    //不属于终端参数 暂保留这里
    public static final String SingState="FF9007";  //签到状态

}
