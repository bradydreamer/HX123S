package com.nexgo.common;

import android.content.ContentValues;
import android.content.SharedPreferences;
import android.util.Log;

import com.nexgo.R;
import com.nexgo.db.DbProxyFactory;
import com.nexgo.db.DbRecordOperation;
import com.nexgo.db.Dboperation;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.UnsupportedEncodingException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

/**
 * Created by HGD on 2016/2/25.
 */
public class TradeTlv {
    public final static String TradeId = "FF9000";     //交易类型码
    public final static String CardNo = "FF9001";     //卡号
    public final static String Amount = "FF9002";    //交易金额
    public final static String VoucherNo = "FF9003";    //交易流水号
    public final static String TraTime = "FF9004";    //受卡方所在地时间(组包填入)HHMMSS
    public final static String TraDate = "FF9005";    //受卡方所在地日期(组包填入)YYYYMMDD
    public final static String CardVailDate = "FF9006";   //卡有效期
    public final static String SignState = "FF9007";   //签到状态
    public final static String PayWay = "FF9008";   //支付方式
    public final static String manualpay = "FF9009";   //手输卡号-01 刷卡-00
    public final static String CardSN = "FF900A";    //卡序列号
    public final static String Tk2 = "FF900D";    //二磁道信息
    public final static String Tk3 = "FF900E";    //三磁道信息
    public final static String ReferNo = "FF900F";     //参考号
    public final static String Repon = "FF9010";     //应答码
    public final static String MerchantNo = "FF9011";   //商户号
    public final static String TerminalNo = "FF9012";   //终端号
    public final static String MoneyCode = "FF901B";     //交易货币代码
    public final static String AccountType = "FF901E";     //账号类型
    public final static String AuthCode = "FF901F";  //授权码
    public final static String BalanceSign = "FF9020";    //余额符号
    public final static String Balance = "FF9021";    //余额
    public final static String Filed55 = "FF902D";    //55域
    public final static String BatchNo = "FF9022";    //批次号
    public final static String FF9026 = "FF9026";  //支持部分扣款和返回余额的标志
    public final static String OriCardNo = "FF9027";  //原交易卡号
    public final static String OriBatchNo = "FF9028";  //原交易批次号
    public final static String OriVoucherNo = "FF9029";  //原交易流水号....
    public final static String OriTraDate = "FF902A";  //原交易日期 ...
    public final static String Op = "FF9041";   //操作员代码
    public final static String Mac = "FF9045";   //MAC
    //交易相关报文域要素
    public final static String SupportReveral = "FF9050";   //交易冲正标识
    public final static String Tk1 = "FF9051";   //一磁信息
    public final static String OriTerminalNo = "FF9054";   //原交易终端号
    public final static String TerminalSN = "FF9055";       //设备序列号
    public final static String Pin = "FF901C";           //密码密文
    public final static String FF901E = "FF901E";  //账户类型

    public final static String OutCreCardAllNo = "FF903A";  //外卡贷记总笔数
    public final static String OutDebCardAllNo = "FF901A";  //外卡借记总笔数
    public final static String OutDebCardAllAmount = "FF9036";  //外卡借记总金额
    public final static String OutCreCardAllAmount = "FF9016";  //外卡贷记总金额
    public final static String InCreCardAllNo = "FF9019";  //内卡贷记总笔数
    public final static String InDebCardAllNo = "FF9039";  //内卡借记总笔数
    public final static String InDebCardAllAmount = "FF9037";  //内卡借记总金额
    public final static String InCreCardAllAmount = "FF9017";  //内卡贷记总金额
    public final static String FF9018 = "FF9018";  //交易明细总笔数
    public final static String FF9015 = "FF9015";  //内卡对账结果
    public final static String FF900B = "FF900B";  //外卡对账结果
    public final static String AcquiringBank = "FF9014";  //收单机构信息
    public final static String IssuingBank = "FF9013";  //发卡机构信息
    public final static String FF9025 = "FF9025";  //IC卡条件代码
    public final static String FF9024 = "FF9024";  //终端读取能力
    public final static String FF9023 = "FF9023";  //网络管理信息码
    public final static String SendMessage = "FF9030";  //发送报文
    public final static String RecvMessage = "FF9031";  //接收报文
    public final static String CardType = "FF9077";    //卡类型
    public final static String IsPrintOver = "FF9078";  //打印完成标识
    public final static String ReveralTime = "FF9079";  //冲正重发次数
    public final static String Tip = "FF907A";          //小费
    public final static String IsUnsale = "FF907B";  //撤销标志
    public final static String IsUpLoad = "FF907C";  //批上送标志
    public final static String Wk = "FF907D";  //工作秘钥密文

    public final static String ReveralNeed = "FF907E";  //冲正标识
    public final static String ReveralMes = "FF907F";  //冲正报文

    public final static String TradeTyFlag = "FF9075"; //借贷计标志
    public final static String InOutCardFlag = "FF9076";  //内外卡标志
    public final static String ScriptSengMess = "FF9073";  //执行脚本通知交易的原始发送报文
    public final static String ScriptRecvMess = "FF9074";  //执行脚本通知交易的原始接报文
    public final static String IsHaveScriptReslt = "FF9072";  //脚本处理结果是否存在
    public final static String PrintScript = "FF9071";  //打印脚本
    public final static String tradeflow = "FF9070";    //交易流程
    public final static String uploadMode = "FF906D";   //批上送模式
    public final static String upAdd = "FF906E";        //批上送断点
    public final static String TotalUpNo = "FF906F";   //批上送总数
    public final static String OriTradeId = "FF902B";     //原交易id
    public final static String IsReupload = "FF906C";     //批上送是否重发
    //  脚本POS中没有的TAG，从FF9080开始累加
    public final static String OfflineRes = "FF9080";     //脱机交易结果 00-脱机接受 01-脱机拒绝
    public final static String InstallmentNum = "FF9081";      //分期期数
    public final static String GoodsProjectCode = "FF9082";      //分期商品项目编码
    public final static String ChargePayMode = "FF9083";       //分期手续费支付方式
    public final static String firstPaymentAmount = "FF9084";     //分期首付款
    public final static String InstallmentCharge = "FF9085";      //分期手续费
    public final static String advertisement = "FF9086";      //广告，发卡行、收单行或银联的增值内容
    public final static String repaymentCurrency = "FF9087";       //分期还款币种
    public final static String cashPointAmount = "FF9088";     //兑换积分数
    public final static String pointBalance = "FF9089";      //积分余额
    public final static String pointPaidAmount = "FF908A";      //积分自付金额
    public final static String transferCardNo = "FF908B";       //转入卡卡号
    public final static String cardholderPayAmount = "FF908C";       //扣持卡人金额

    //华夏银行增加--begain
    public final static String SendEncrptMessage = "FF908D";  //发送报文全报文加密密文
    public final static String RecvEncrptMessage = "FF908E";  //接收报文全报文加密密文
    public final static Boolean isEncriptBagTag = true; //false;  //是否全报文加密
    //华夏银行增加--end

    //通用预设值参数
//    public final static String TelNo1 = "FF8000";  //交易电话号码1
//    public final static String TelNo2 = "FF8001";  //交易电话号码2
//    public final static String TelNo3 = "FF8002";  //交易电话号码3
//    public final static String FF8003 = "FF8003";  //外线号码
//    public final static String APN1 = "FF8004";  //APN1名称
//    public final static String APN2 = "FF8005";  //APN2名称
//    public final static String GPRSParam = "FF8006";  //GPRS接入服务器参数
//    public final static String CDMAParam = "FF8007";        //CDMA接入服务器参数
//    public final static String FF8008 = "FF8008";          //预拨号标识
//    public final static String LocalNetParam = "FF8009";  //IP接入服务器参数

    public final static String MACType = "FF8010";  //MAC算法标志
    public final static String TPDU = "FF8011";  //TPDU
    public final static String IP = "FF8012";  //服务器IP1
    public final static String Port = "FF8013";  //服务器PORT1
    public final static String IP2 = "FF807D";  //服务器IP1
    public final static String Port2 = "FF807E";  //服务器PORT2
    public final static String TakEncry = "FF8015";  //磁道信息加密标志
    public final static String Tmkindex = "FF8016";  //终端主密钥索引
    public final static String EncryType = "FF8017";  //加密算法标志
    public final static String MerchantName = "FF8018";  //商户名称
    public static final String PrintPaper = "FF8021";  //打印联数
    public static final String TradeTimeout = "FF8019";
    public static final String TelOutsideNO = "FF8020";
    public static final String Tmk = "FF8022";
    public static final String UnsaleNeedSwipeCard = "FF8023";
    public static final String UnsaleNeedPwd = "FF8024";
    public static final String AutoSignOut = "FF8046";
    public static final String LastSingIn = "FF804E";//上次签到日期

    public static final String Rfchannel = "FF8030";//0-电子现金1-联机借贷记  -change by cbq  这里数值反过来
    public static final String IssuporFlashcard = "FF8031";
    public static final String FlashcardDealtime = "FF803A";
    public static final String FlashcardLifetime = "FF803C";
    public static final String IsRfFast = "FF8054";
    public static final String BinA = "FF8055";
    public static final String BinB = "FF8056";
    public static final String CDCVM = "FF8057";
    public static final String QPS_NoPWDLimAmonut = "FF8058";
    public static final String NoSignLimAmonut = "FF8059";
    public static final String NoPWDLimAmonut = "FF805A";
    public final static String AppName = "FF807F";  //应用名称
    public final static String MerchantEngName = "FF807C";  //商户英文名称
    public final static String SystemPwd = "FF807A";  //系统密码
    public final static String ManagePwd = "FF807B";  //主管密码
    public final static String securePwd = "FF8070";  //安全密码
    public final static String IsPrivatenet = "FF806F";     //公网专网选项
    public final static String IsPrintdetail = "FF8071";     //是否打印明细
    public final static String LimitOffline = "FF8072";     //脱机交易笔数，超过此值自动上送

    //华夏银行增加--begain
    public final static String Klkindex = "FF8080";  //终端传输密钥索引
    public final static String PMKId = "FF8081";     //主密钥ID号
    public final static String aucEncryptKey = "FF8082";     //全包文加密密钥
    public final static String jingdu = "FF8083";  //定位 经度
    public final static String weidu = "FF8084";   //定位 纬度
    //华夏银行增加--end

    //全局变量，作为变量用，与交易数据无关
    public final static String ECBanlance = "FFA101";     //电子现金余额
    public final static String LinkState = "FFA102";     //链路状态
    public final static String IsneedValiddate = "FFA103";     //是否需要输卡有效日期

    //    //EMVTAG：POOC设置公钥
    //    public final static String E9F22 = "E9F22";  //与 RID 一起标识认证中心的公钥
    //    //EMVTAG：POOC设置AID
    //    public final static String E9F09 = "9F09";  //应用版本号
    //    public final static String E9F1B = "9F1B";  //终端最低限额 IC 卡消费时终端允许的最低
    //    public final static String E9F7B = "9F7B";  //终端电子现金交易限额
    //    //PBOC 设置终端属性
    //    public final static String E9F40 = "9F40";  //终端附加性能
    //    public final static String E9F39 = "9F39";  //Pos entry
    //    public final static String E9F01 = "9F01";  //Acq_id
    //    public final static String E9F15 = "9F7B";  //商户类型码
    //    public final static String E5F2A = "5F2A";  //交易货币代码
    //    public final static String E5F36 = "5F36";  //交易货币指数
    //    public final static String E9F3C = "9F3C";  //交易参考货币代码
    //    public final static String E9F3D = "9F3D";  //交易参考货币指数
    //    public final static String E9F1C = "9F3C";  //终端号
    //    public final static String E9F66 = "9F66";  //终端交易属性
    //    public final static String EBF73 = "BF73";  //主密钥索引
    //
    //    //PBOC 取交易数据
    //
    //    //二次授权
    //    public final static String E8A = "8A"; //授权响应吗，如无法联机，授权响应吗由交易系统指定构造
    //    public final static String E89 = "89";//授权码，授权机构为每一个批准的交易提供的值
    //    public final static String E71 = "71";//发卡行脚本1
    //    public final static String E72 = "72";//发卡行脚本2
    //    public final static String E91 = "91";//发卡行认证数据
    //
    //    //EMVTAG：执行标准程序
    //    public final static String E9F02 = "9F02";  //授权金额
    //    public final static String E9F03 = "9F03";  //其他金额
    //    public final static String E9C = "9C";        //交易类型
    //    public final static String E9A = "9A";  //交易日期
    //    public final static String E9F21 = "9F21";  //交易时间
    //    public final static String E9F4E = "9F4E";  //商户名称
    //    public final static String E9F16 = "9F16";  //商户号
    //    public final static String E9F06 = "9F06";  //AID
    //    public final static String E9F26 = "9F26";  //应用密文
    //    public final static String E9F10 = "9F10";  //IAD
    //    public final static String E9F37 = "9F37";  //不可预知数
    //    public final static String E95 = "95";  //终端验证结果
    //    public final static String E82 = "82";  //应用交互特征
    //    public final static String E9F1A = "9F1A";  //终端国家代码
    //    public final static String E9F33 = "9F33";  //终端性能
    //    public final static String E9F34 = "9F34";  //电子现金发卡行授权码
    //    public final static String E9F74 = "9F74";  //持卡人验证方法结果
    //    public final static String E9F35 = "9F35";  //终端类型
    //    public final static String E9F1E = "9F1E";  //接口设备序列号
    //    public final static String E9F41 = "9F41";  //交易序列计数器
    //    public final static String E9F63 = "9F63";  //卡产品标识信息
    //    public final static String E5F34 = "5F34";  //卡序列号
    //    public final static String E5A = "5A";    //主帐号
    //    public final static String E57 = "57";    //二磁道等效信息
    //    public final static String E5F24 = "5F24";    //应用失效日期
    //    //qPBOC 流程
    //    public final static String E9F27 = "9F27";    //应用信息数据
    //    public final static String E9F36 = "9F36";    //应用交易计数
    //    public final static String E84 = "84";    //专用文件名称

    private static Dboperation dbRecord;
    private int buflen = 2048;
    private static HashMap<String, byte[]> TlvMap = new HashMap<>();
    private static ContentValues RecordMap = null;
    private static ArrayList<String> Recodrdlist = new ArrayList<>();//打印list
    private static ArrayList<String> Filelist = new ArrayList<>();//文件类list
    private SharedPreferences SP = GlobalHolder.getInstance().getPrefs();
    private static Logger log = LoggerFactory.getLogger("TradeTlv");


    //文件类交易记录TAG
    //FIXME: 需要添加
    private void Filelistinit() {
        //华夏银行增加--begain
        Filelist.add(ReferNo);
        Filelist.add(PMKId);
        Filelist.add(Klkindex);
        Filelist.add(jingdu);
        Filelist.add(weidu);
        //华夏银行增加--end

        Filelist.add(VoucherNo);
        Filelist.add(Op);
        Filelist.add(BatchNo);
        Filelist.add(SignState);
        Filelist.add(Tmkindex);
        Filelist.add(TerminalNo);
        Filelist.add(MerchantName);
        Filelist.add(MerchantNo);
        Filelist.add(TakEncry);
        Filelist.add(MACType);
        Filelist.add(EncryType);
        Filelist.add(TPDU);
        Filelist.add(IP);
        Filelist.add(Port);
        Filelist.add(IP2);
        Filelist.add(Port2);
        Filelist.add(PrintPaper);
        Filelist.add(Tmkindex);
        Filelist.add(TradeTimeout);
        Filelist.add(TelOutsideNO);
        Filelist.add(UnsaleNeedSwipeCard);
        Filelist.add(UnsaleNeedPwd);
        Filelist.add(AutoSignOut);
        Filelist.add(securePwd);
        Filelist.add(SystemPwd);
        Filelist.add(ManagePwd);
        Filelist.add(IsPrivatenet);
        Filelist.add(LastSingIn);
        Filelist.add(IsPrintdetail);
        Filelist.add(MoneyCode);
        Filelist.add(Rfchannel);
        Filelist.add(LimitOffline);
    }

    //数据库交易记录TAG
    private void Recodrdlistinit() {
        Recodrdlist.add(SendMessage);
        Recodrdlist.add(RecvMessage);
        Recodrdlist.add(Filed55);
        Recodrdlist.add(CardType);
        Recodrdlist.add(MerchantName);
        Recodrdlist.add(MerchantNo);
        Recodrdlist.add(MoneyCode);
        Recodrdlist.add(AccountType);
        Recodrdlist.add(TerminalNo);
        Recodrdlist.add(IssuingBank);
        Recodrdlist.add(AcquiringBank);
        Recodrdlist.add(CardNo);
        Recodrdlist.add(PayWay);
        Recodrdlist.add(BatchNo);
        Recodrdlist.add(VoucherNo);
        Recodrdlist.add(AuthCode);
        Recodrdlist.add(ReferNo);
        Recodrdlist.add(TraDate);
        Recodrdlist.add(TraTime);
        Recodrdlist.add(Amount);
        Recodrdlist.add(IsPrintOver);
        Recodrdlist.add(CardType);
        Recodrdlist.add(TradeTyFlag);
        Recodrdlist.add(InOutCardFlag);
        Recodrdlist.add(Op);
        Recodrdlist.add(IsUnsale);
        Recodrdlist.add(IsUpLoad);
        Recodrdlist.add(CardVailDate);
        Recodrdlist.add(TradeId);
        Recodrdlist.add(OfflineRes);
        Recodrdlist.add(InstallmentNum);
        Recodrdlist.add(GoodsProjectCode);
        Recodrdlist.add(ChargePayMode);
        Recodrdlist.add(firstPaymentAmount);
        Recodrdlist.add(InstallmentCharge);
        Recodrdlist.add(advertisement);
        Recodrdlist.add(repaymentCurrency);
        Recodrdlist.add(cashPointAmount);
        Recodrdlist.add(pointBalance);
        Recodrdlist.add(pointPaidAmount);
        Recodrdlist.add(transferCardNo);
        Recodrdlist.add(cardholderPayAmount);
    }

    private TradeTlv() {
        Recodrdlistinit();
        Filelistinit();
        try {
            dbRecord = (Dboperation) DbProxyFactory.getProxy(DbRecordOperation.getInstance());
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //内部类保证线程安全
    private static class SingletonFactory {
        public static TradeTlv instance = new TradeTlv();
    }

    /* 静态工程方法，创建实例 */
    public static TradeTlv getInstance() {
        return SingletonFactory.instance;
    }

    /**
     * 组集合所有TLV返回报文
     */
    public byte[] formtlvPackage() {
        int len = 0;
        byte[] temp = new byte[buflen];
        for (Map.Entry<String, byte[]> entry : TlvMap.entrySet()) {
            String tag = entry.getKey();
            byte[] Tlvdata = entry.getValue();
            if (Tlvdata == null) {
                continue;
            }
            byte[] tep = TlvUtils.getTLVData(tag, Tlvdata.length, Tlvdata);
            System.arraycopy(tep, 0, temp, len, tep.length);
            len += tep.length;
        }

        byte[] buf = new byte[len];
        System.arraycopy(temp, 0, buf, 0, len);
        log.debug("Tlvdata：{}", ByteUtils.byteArray2HexStringWithSpace(buf));
        return buf;
    }

//    /**
//     * 解TLV报文
//     *
//     * @param TlvPackage TLV报文  String
//     */
//    public boolean SetTagsVaule(String TlvPackage) {
//        byte[] tlvPackage = ByteUtils.hexString2ByteArray(TlvPackage);
//        log.debug( "tlc:{}" , ByteUtils.byteArray2HexString(tlvPackage));
//        boolean Presult = SetTagsValue(tlvPackage);
//        return Presult;
//    }

    /**
     * 解析TLV
     *
     * @param tlvPackage TLV报文  byte[]
     * @return HashMap  以Map形式返回
     */
    public HashMap<String, byte[]> ParseTLV(byte[] tlvPackage) {
        HashMap<String, byte[]> buf = new HashMap<>();
        if (tlvPackage == null || tlvPackage.length < 4) {
            throw new RuntimeException("Invalid tlv, null or odd length");
        } else {
            try {
                int i;
                String StrTag;
                for (i = 0; i < tlvPackage.length; ) {
                    if (tlvPackage[i] != (byte) 0xff) {
                        throw new RuntimeException("TAG Error");
                    }
                    byte[] TAG = new byte[3];
                    System.arraycopy(tlvPackage, i, TAG, 0, TAG.length);
                    StrTag = ByteUtils.byteArray2HexString(TAG);
                    i = i + 3;
                    byte[] ValueData;
                    int ValueiLen;
                    log.debug(" & 0x80:{}", String.valueOf(tlvPackage[i] & 0x80));
                    if ((tlvPackage[i] & 0x80) == 0x80)    //L第一个字节最高位为1
                    {
                        int Lenlen = (int) tlvPackage[i] & (byte) 0x7f;
                        if (Lenlen > 3) {
                            return null;
                        }
                        log.debug("tlvPackage[i] &0x7f:{}", String.valueOf(tlvPackage[i] & (byte) 0x7f) + "   " + String.valueOf(Lenlen));
                        byte[] LenData = new byte[Lenlen];
                        //取L字段
                        System.arraycopy(tlvPackage, ++i, LenData, 0, Lenlen);
                        log.debug("Len:{}", Lenlen + "LenData{}", (ByteUtils.byteArray2HexStringWithSpace(LenData)));
                        if (Lenlen > 1) //两个字节长度值
                        {
                            ValueiLen = ByteUtils.byte2int(LenData[0], LenData[1]);
                            ValueData = new byte[ValueiLen];
                            log.debug("ValueiLen:{}", String.valueOf(ValueiLen));
                            i += Lenlen;
                            System.arraycopy(tlvPackage, i, ValueData, 0, ValueiLen);
                            i += ValueiLen;
                        } else if (Lenlen == 1) {
                            ValueiLen = (int) tlvPackage[i];
                            ValueData = new byte[ValueiLen];
                            System.arraycopy(tlvPackage, ++i, ValueData, 0, ValueiLen);
                            i += ValueiLen;
                        } else    //长度值为0是不存在的
                        {
                            //待定
                            return null;
                        }
                    } else//第一字节最高位为0
                    {
                        ValueiLen = (int) tlvPackage[i];
                        i++;
                        log.debug("ValueiLen:{}", String.valueOf(ValueiLen));
                        ValueData = new byte[ValueiLen];
                        System.arraycopy(tlvPackage, i, ValueData, 0, ValueiLen);
                        i += ValueiLen;

                    }
                    buf.put(StrTag.toUpperCase(), ValueData);

                }
            } catch (NumberFormatException e) {
                throw new RuntimeException("Error parsing number", e);
            } catch (IndexOutOfBoundsException e) {
                throw new RuntimeException("Error processing field", e);
            }
        }
        return buf;
    }

    /**
     * 设置多个TAG值
     *
     * @param tlvPackage TLV报文  tag hexbyte[]+length+databyte[]
     *                   报文Taghexbyte[],文件和数据库Tag用ASCIIbyte[].
     */
    public boolean SetTagsValue(byte[] tlvPackage) {
        //  HashMap<String, byte[]> RetTlvmap = new HashMap<>();
        if (tlvPackage == null || tlvPackage.length < 4) {
            throw new RuntimeException("Invalid tlv, null or odd length");
        } else {
            try {
                int i;
                String StrTag;
                for (i = 0; i < tlvPackage.length; ) {
                    if (tlvPackage[i] != (byte) 0xff) {
                        throw new RuntimeException("TAG Error");
                    }
                    byte[] TAG = new byte[3];
                    System.arraycopy(tlvPackage, i, TAG, 0, TAG.length);
                    StrTag = ByteUtils.byteArray2HexString(TAG);
                    i = i + 3;
                    byte[] ValueData;
                    int ValueiLen;
                    log.debug(" & 0x80:{}", String.valueOf(tlvPackage[i] & 0x80));

                    if ((tlvPackage[i] & 0x80) == 0x80)    //L第一个字节最高位为1
                    {

                        int Lenlen = (int) tlvPackage[i] & (byte) 0x7f;
                        if (Lenlen > 3) {
                            return false;
                        }
                        log.debug("tlvPackage[i] &0x7f:{}", String.valueOf(tlvPackage[i] & (byte) 0x7f) + "Lenlen:{}", String.valueOf(Lenlen));
                        byte[] LenData = new byte[Lenlen];
                        //取L字段
                        System.arraycopy(tlvPackage, ++i, LenData, 0, Lenlen);
                        log.debug("Len:{}", Lenlen + "LenData:{}", (ByteUtils.byteArray2HexStringWithSpace(LenData)));
                        if (Lenlen > 1) //两个字节长度值
                        {
                            ValueiLen = ByteUtils.byte2int(LenData[0], LenData[1]);
                            ValueData = new byte[ValueiLen];
                            log.debug("ValueiLen:{}", String.valueOf(ValueiLen));
                            i += Lenlen;
                            System.arraycopy(tlvPackage, i, ValueData, 0, ValueiLen);
                            i += ValueiLen;
                        } else if (Lenlen == 1) {
                            ValueiLen = (int) tlvPackage[i];
                            ValueData = new byte[ValueiLen];
                            System.arraycopy(tlvPackage, ++i, ValueData, 0, ValueiLen);
                            i += ValueiLen;
                        } else//长度值为0是不存在的
                        {
                            //待定
                            return false;
                        }
                    } else//第一字节最高位为0
                    {
                        ValueiLen = (int) tlvPackage[i];
                        i++;
                        log.debug("ValueiLen:{}", String.valueOf(ValueiLen));
                        ValueData = new byte[ValueiLen];
                        System.arraycopy(tlvPackage, i, ValueData, 0, ValueiLen);
                        i += ValueiLen;
                    }
                    setTagValue(StrTag, ValueData);

                }
            } catch (NumberFormatException e) {
                throw new RuntimeException("Error parsing number", e);
            } catch (IndexOutOfBoundsException e) {
                throw new RuntimeException("Error processing field", e);
            }
            return true;
        }
    }

    /**
     * 更新 清除全局变量数据
     */
    public void cleanTlv() {
        TlvMap.clear();
    }

    /**
     * 获取TAG对应的长度值
     *
     * @param TAG Tag
     * @return Value 数组长度，null返回0；
     */
    public int getTagValueLength(String TAG) {
        if (getTagValue(TAG) == null) {
            return 0;
        }
        return getTagValue(TAG).length;
    }

    /**
     * @param TAG
     * @return byte[] 数组
     */
    public byte[] getTagValue(String TAG) {
        if (Filelist.contains(TAG.toUpperCase())) {
            if (SP.getString(TAG.toUpperCase(), null) == null) {
                return null;
            } else if (SP.getString(TAG.toUpperCase(), null).equals("")) {
                return null;
            }
            log.debug("取文件TAG:{}", SP.getString(TAG.toUpperCase(), null));

            return SP.getString(TAG.toUpperCase(), null).getBytes();

            //  return ByteUtils.string2ASCIIByteArray(SP.getString(TAG.toUpperCase(), null));
//        }
//        else if (TlvMap.get(TAG.toUpperCase()) != null) {
//            Log.d("hgd","Map有值;"+ByteUtils.byteArray2HexString(TlvMap.get(TAG.toUpperCase())));
//            return TlvMap.get(TAG.toUpperCase());
        } else if (TAG.toUpperCase().equals(ReveralNeed)) {

            if (dbRecord.getReversalData(ReveralNeed) == null || dbRecord.getReversalData(ReveralNeed).equals("")) {
                return null;
            } else {
                log.debug("取冲正标签:{}", ByteUtils.byteArray2HexString(dbRecord.getReversalData(ReveralNeed).getBytes()));
                return ByteUtils.string2ASCIIByteArray(dbRecord.getReversalData(ReveralNeed));

            }
        } else if (TAG.toUpperCase().equals(ReveralMes)) {

            if (dbRecord.getReversalData(ReveralMes) == null || dbRecord.getReversalData(ReveralMes).equals("")) {
                return null;
            } else {
                log.debug("取冲正报文:{}", dbRecord.getReversalData(ReveralMes));
                return ByteUtils.hexString2ByteArray(dbRecord.getReversalData(ReveralMes));
            }
        } else if (TAG.toUpperCase().equals(ScriptRecvMess) || TAG.toUpperCase().equals(ScriptSengMess)) {
            if (dbRecord.getScriptData(TAG.toUpperCase()) == null || dbRecord.getScriptData(TAG.toUpperCase()).equals("")) {
                return null;
            } else {
                log.debug("取脚本报文:{}", dbRecord.getScriptData(TAG.toUpperCase()));

                return ByteUtils.hexString2ByteArray(dbRecord.getScriptData(TAG.toUpperCase()));
            }
        } else if (TAG.toUpperCase().equals(IsHaveScriptReslt)) {
            if (dbRecord.getScriptData(IsHaveScriptReslt) == null || dbRecord.getScriptData(IsHaveScriptReslt).equals("")) {
                return null;
            } else {
                log.debug("取脚本结果:{}", dbRecord.getScriptData(IsHaveScriptReslt));
                return ByteUtils.string2ASCIIByteArray(dbRecord.getScriptData(IsHaveScriptReslt));
            }
        } else if (TAG.toUpperCase().equals(upAdd) || TAG.toUpperCase().equals(TotalUpNo) || TAG.toUpperCase().equals(uploadMode)) {
            log.debug("取批上送:TAG:{}，Value:{}", TAG.toUpperCase(), dbRecord.getOtherInfoTableData(TAG.toUpperCase()));
            if (dbRecord.getOtherInfoTableData(TAG.toUpperCase()) == null || dbRecord.getOtherInfoTableData(TAG.toUpperCase()).equals("")) {
                return null;
            }
            log.debug("取批上送:TAG:{}，Value:{}", TAG.toUpperCase(), dbRecord.getOtherInfoTableData(TAG.toUpperCase()));
            if (TAG.toUpperCase().equals(uploadMode)) {
                return ByteUtils.hexString2ByteArray(dbRecord.getOtherInfoTableData(TAG.toUpperCase()));
            } else {
                return ByteUtils.string2ASCIIByteArray(dbRecord.getOtherInfoTableData(TAG.toUpperCase()));
            }
        }
        //内存Tag直接内存返回
        log.debug("取内存TAG:{},Value:{}", TAG, ByteUtils.byteArray2HexStringWithSpace(TlvMap.get(TAG.toUpperCase())));
        return TlvMap.get(TAG.toUpperCase());
    }

    /*
     * 根据TAG数组获取对应TAG值
     * @param TagBuf
     * @return byte[] TLV数组
     */
    public byte[] getTagsVaule(String[] TagBuf) {
        byte[] Temp = new byte[buflen];       //暂时定buflen2048
        byte[] Buf;
        int index = 0;
        try {
            for (int i = 0; i < TagBuf.length; i++) {
                byte[] Tagdata = getTagValue(TagBuf[i]);
                log.debug("Tagdata：{}", ByteUtils.byteArray2HexString(Tagdata));
                if (Tagdata == null) {
                    continue;
                }
                byte[] allTagdata = TlvUtils.getTLVData(TagBuf[i], Tagdata.length, Tagdata);
                System.arraycopy(allTagdata, 0, Temp, index, allTagdata.length);
                index += allTagdata.length;
                log.debug("allTagdata:{}", ByteUtils.byteArray2HexString(allTagdata));
                if (Tagdata.length > buflen) {
                    throw new RuntimeException("Error !数组申请内存不足");
                }
            }
            Log.d("hgd", ByteUtils.byteArray2HexString(Temp));
            Buf = new byte[index];
            System.arraycopy(Temp, 0, Buf, 0, index);
        } catch (NumberFormatException e) {
            throw new RuntimeException("Error parsing number", e);
        }
        Log.d("hgd", ByteUtils.byteArray2HexString(Buf));
        return Buf;
    }
//       /*
//        * 根据TAG数组获取对应TAG值
//        * @param TagBuf
//        * @return HashMap<String,byte[]>
//        */
//    public HashMap<String,byte[]> getTagsVaule(String[] TagBuf)
//    {
//        HashMap<String,byte[]> TagMap=new HashMap<>();
//        int index = 0;
//        try {
//            for (int i = 0; i < TagBuf.length; i++) {
//                byte[] Tagdata = TlvMap.get(TagBuf[i].toUpperCase());
//                Log.d("hgd", "Tag:" +TlvMap.get(TagBuf[i].toUpperCase()+"Tagdata:" + ByteUtils.byteArray2HexString(Tagdata));
//                TagMap.put(TagBuf[i].toUpperCase(),Tagdata);
//            }
//        } catch (NumberFormatException e) {
//            e.printStackTrace();
//
//            }
//        return TagMap;
//    }

    /**
     * @param TAG   TAG
     * @param valve 文件类：ASCII数组，
     */
    public void setTagValue(String TAG, byte[] valve) {
        //先放内存
        TlvMap.put(TAG.toUpperCase(), valve);
        log.debug("setTAG：TAG:{},Value:{}", TAG.toUpperCase(), ByteUtils.byteArray2HexStringWithSpace(valve));

        // FIXME: 还没确定好2016/3/22
        if (Filelist.contains(TAG.toUpperCase())) {
            log.debug("存文件：TAG:{},Value:{}", TAG.toUpperCase(), new String(valve));

            try {
                SP.edit().putString(TAG.toUpperCase(), new String(valve, "UTF-8")).commit();//UTF-8兼容ASCii,
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
        //  交易类更新到数据库
//        if (Storelist.contains(TAG.toUpperCase())) {
//            if (TlvMap.get(VoucherNo) == null) {
//                throw new RuntimeException("VoucherNo==null can not save");
//            }
//            ContentValues updatemap = new ContentValues();
//            String Voucherno = ByteUtils.asciiByteArray2String(TlvMap.get(VoucherNo));
//            updatemap.put(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
//            dbRecord.update(updatemap, Voucherno);
//        }
        //保存冲正报文

        else if (TAG.toUpperCase().equals(ReveralNeed)) {
            dbRecord.updateReversalTbale(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
            log.debug("保存冲正标签{}", new String(valve));

        } else if (TAG.toUpperCase().equals(ReveralMes)) {
            dbRecord.updateReversalTbale(TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve));
            log.debug("保存冲正报文:{}", ByteUtils.byteArray2HexStringWithSpace(valve));
        } else if (TAG.toUpperCase().equals(ScriptSengMess) || TAG.toUpperCase().equals(ScriptRecvMess)) {
            dbRecord.updateScriptTbale(TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve));
            log.debug("脚本报文{}", ByteUtils.byteArray2HexString(valve));

        } else if (TAG.toUpperCase().equals(IsHaveScriptReslt)) {
            dbRecord.updateScriptTbale(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
            log.debug("脚本结果{}", ByteUtils.asciiByteArray2String(valve));
        } else if (TAG.toUpperCase().equals(TotalUpNo) || TAG.toUpperCase().equals(upAdd) || TAG.toUpperCase().equals(uploadMode)) {
            log.debug("批上送TAG:{}，value:{}", TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
            if (TAG.toUpperCase().equals(uploadMode)) {
                dbRecord.updateOtherInfoTable(uploadMode, ByteUtils.byteArray2HexString(valve));
            } else {
                dbRecord.updateOtherInfoTable(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
            }
        }
    }

    /**
     * @param TAG        TAG
     * @param valve      ASCII数组
     * @param IsUpdateDb 这个参数正对数据库有的TAG才会更新到数据库
     */
    public void setTagValue(String TAG, byte[] valve, boolean IsUpdateDb) {
        // 更新到数据库
        if (IsUpdateDb) {
            if (Recodrdlist.contains(TAG.toUpperCase())) {
                if ((TAG.toUpperCase().equals(VoucherNo))) {
                    TlvMap.put(TAG.toUpperCase(), valve);
                    dbRecord.update(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve), ByteUtils.byteArray2HexString(valve));
                    return;
                }
                if (TlvMap.get(VoucherNo) == null) {
                    throw new RuntimeException("VoucherNo==null can not save");
                }
                String Voucherno = ByteUtils.asciiByteArray2String(TlvMap.get(VoucherNo));
                //  TlvMap.put(TAG.toUpperCase(), valve);
                if (TAG.toUpperCase().equals(SendMessage)
                        || TAG.toUpperCase().equals(RecvMessage)
                        || TAG.toUpperCase().equals(Filed55)) {
                    dbRecord.update(TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve), Voucherno);
                    log.debug("报文");
                    return;
                }
                dbRecord.update(TAG.toUpperCase(), new String(valve), Voucherno);
            }
            //保存冲正
            else if (TAG.toUpperCase().equals(ReveralMes)
                    || TAG.toUpperCase().equals(ReveralNeed)) {
                if (TAG.toUpperCase().equals(ReveralMes)) {
                    boolean ret = dbRecord.updateReversalTbale(TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve));
                    log.debug("保存冲正报文到数据库" + ret);
                } else {
                    boolean reta = dbRecord.updateReversalTbale(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
                    log.debug("保存冲正标签到数据库" + reta);
                }
            }
            //脚本
            else if (TAG.toUpperCase().equals(ScriptRecvMess)
                    || TAG.toUpperCase().equals(ScriptSengMess)
                    || TAG.toUpperCase().equals(IsHaveScriptReslt)) {
                if (TAG.toUpperCase().equals(ScriptRecvMess)
                        || TAG.toUpperCase().equals(ScriptSengMess)) {
                    boolean ret = dbRecord.updateScriptTbale(TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve));
                    log.debug("保存脚本到数据库{}", ret);
                } else {
                    boolean reta = dbRecord.updateReversalTbale(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
                    log.debug("保存脚本结果数据库" + reta);
                }
            } else if (TAG.toUpperCase().equals(TotalUpNo) || TAG.toUpperCase().equals(upAdd) || TAG.toUpperCase().equals(uploadMode)) {
                log.debug("批上送TAG:{}，value:{}", TAG.toUpperCase(), ByteUtils.byteArray2HexString(valve));
                if (TAG.toUpperCase().equals(uploadMode)) {
                    dbRecord.updateOtherInfoTable(uploadMode, ByteUtils.byteArray2HexString(valve));
                } else {
                    dbRecord.updateOtherInfoTable(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve));
                }
            } else {
                throw new RuntimeException("Tag cannot Update to db!!!");
            }
        } else {
            TlvMap.put(TAG.toUpperCase(), valve);
        }
    }

    /**
     * 获取TAG对应的长度值
     *
     * @param TAG    tag
     * @param Length 长度
     * @param valve  统一以ASCII码数组做入参
     */
    //存入指定长度的数组
    public void setTagValue(String TAG, byte[] valve, int Length) {
        //保存报文
        byte[] buf = new byte[Length];
        System.arraycopy(valve, 0, buf, 0, Length);
        Log.d("hgd", ByteUtils.byteArray2HexString(buf));
        setTagValue(TAG.toUpperCase(), buf);
    }


    public static void setTagValueForC(String TAG, byte[] valve, int Length) {
        SingletonFactory.instance.setTagValue(TAG, valve, Length);
    }

    public static boolean getOneTradeRecordForC(String voucherNo) {
        return (SingletonFactory.instance.getOneTradeRecord(voucherNo) != null);
    }

    public static byte[] getTagValueForC(String TAG) {
        return SingletonFactory.instance.getTagValue(TAG);
    }

    public static int getTagValueLengthForC(String TAG) {
        return SingletonFactory.instance.getTagValueLength(TAG);
    }

    public static boolean updateTradetoDb() {
        return SingletonFactory.instance.UpDateTradetoDb();
    }

    public static void deleteAllRecordForC() {
        SingletonFactory.instance.deleteAllRecord();
    }

    public static boolean deleteRecordForC(String Voucher) {
        return SingletonFactory.instance.deleteRecord(Voucher);
    }

    /*
    * 返回整个Map
    * @return HashMap
    */
    public HashMap getMap() {
        return TlvMap;
    }

    /*
     * 更新TAG值到数据库，从内存中获取。
     * 只更新交易明细表有的TAG。
     * @return 更新结果  true or false
     * */
    public boolean UpDateTradetoDb() {
        if (TlvMap.get(VoucherNo) == null) {
            return false;
        }
        String Voucherno = ByteUtils.asciiByteArray2String(TlvMap.get(VoucherNo));
        ContentValues updatemap = new ContentValues();
        for (String Tag : Recodrdlist) {
            if (Tag.toUpperCase().equals(SendMessage)
                    || Tag.toUpperCase().equals(RecvMessage)
                    || Tag.toUpperCase().equals(Filed55)) {
                updatemap.put(Tag.toUpperCase(), ByteUtils.byteArray2HexString(TlvMap.get(Tag.toUpperCase())));
                log.debug("报文qqq111TAG:{} Value:{}", Tag.toUpperCase(), ByteUtils.byteArray2HexString(TlvMap.get(Tag.toUpperCase())));
                continue;
            } else if (getTagValue(Tag.toUpperCase()) == null) {
                continue;
            }
            log.debug("报文qqq222TAG:{} Value:{}", Tag.toUpperCase(), ByteUtils.byteArray2HexString(getTagValue(Tag.toUpperCase())));
            // updatemap.put(Tag.toUpperCase(), ByteUtils.asciiByteArray2String(getTagValue(Tag.toUpperCase())));
            try {
                updatemap.put(Tag.toUpperCase(), new String(getTagValue(Tag.toUpperCase()), "UTF-8"));
            } catch (UnsupportedEncodingException e) {
                e.printStackTrace();
            }
        }
        //更新到数据库
        boolean rst = dbRecord.update(updatemap, Voucherno);
        log.debug("结果保存记录：{}", rst);

        return rst;
    }

    /**
     * 根据流水号获取消费记录详情
     *
     * @param VoucherNo 主要给C封
     * @return 结果true or false
     */
    public boolean getTradeRecord(String VoucherNo) {
        RecordMap = dbRecord.getOneRecord(VoucherNo);
        if (RecordMap == null) {
            return false;
        }
        return true;
    }

    /**
     * 流水号获取一条交易记录 重新填TAG
     *
     * @param voucherNo
     * @return CV
     */
    public ContentValues getOneTradeRecord(String voucherNo) {
        ContentValues recordmap = dbRecord.getOneRecord(voucherNo);
        if (recordmap == null) {
            return null;
        }
        for (Map.Entry<String, Object> tag : recordmap.valueSet()) {
            if (!tag.getValue().equals("")) {
                if (tag.getKey().equals("PID")) {
                    continue;
                } else if (tag.getKey().equals(TradeId)) {
                    TlvMap.put(OriTradeId, tag.getValue().toString().getBytes());
                } else if (tag.getKey().equals(RecvMessage) ||
                        tag.getKey().equals(SendMessage) ||
                        tag.getKey().equals(Filed55)) {
                    TlvMap.put(tag.getKey(), ByteUtils.hexString2ByteArray(tag.getValue().toString()));
                    log.debug("报文TAG:{},value:{},", tag.getKey(), tag.getValue().toString());
                } else if (tag.getKey().equals(VoucherNo)) {
                    TlvMap.put(OriVoucherNo, tag.getValue().toString().getBytes());
                    log.debug("OriVoucherNo:{},value:{},", OriVoucherNo, ByteUtils.asciiByteArray2String(tag.getValue().toString().getBytes()));

                } else if (tag.getKey().equals(TraDate)) {
                    TlvMap.put(OriTraDate, tag.getValue().toString().getBytes());
                    log.debug("OriTraDate:{},value:{},", OriTraDate, new String(tag.getValue().toString().getBytes()));

                } else {
                    TlvMap.put(tag.getKey().toUpperCase(), tag.getValue().toString().getBytes());
                    log.debug("TAG:{},value:{}", tag.getKey().toUpperCase(), new String(tag.getValue().toString().getBytes()));
                }

            } else {
                TlvMap.put(tag.getKey(), null);
                log.debug("TAGvalue空:{}", tag.getKey());


            }
        }
        return dbRecord.getOneRecord(voucherNo);
    }


    /**
     * 从数据库中查询最近的所有交易明细
     *
     * @param index
     * @return
     */
    public ArrayList<ContentValues> getAllInfo(int index) {
        return dbRecord.getAllInfo(index);
    }

    /**
     * 按交易类型统计笔数和金额
     *
     * @param tradeId  交易ID
     * @param cardFlag 内外卡 0-内卡 1-外卡
     * @return
     */
    public HashMap settleStatistics(String tradeId, String cardFlag) {
        return dbRecord.settleStatistics(tradeId, cardFlag);
    }


    /**
     * zbq
     * 从数据库中查询最近的消费交易明细，用于撤销和重打印显示。
     * 筛选条件：消费交易；
     *
     * @param index
     * @return 除了报文不返回，其余信息都返回
     */
    public ArrayList<ContentValues> getSucessConsumeInfo(int index) {
        return dbRecord.getSucessConsumeInfo(index);
    }

    /**
     * 根据流水号获取消费记录详情使用此方法先调用获取记录函数，才会返回值，不然返回null.
     *
     * @param Tag （主要给C封）
     * @return String TagValue
     */
    public String getRecordTagValue(String Tag) {
        if (RecordMap == null || !Recodrdlist.contains(Tag.toUpperCase()) || RecordMap.get(Tag) == null) {
            return null;
        }
        return (String) RecordMap.get(Tag.toUpperCase());
    }

    /*
     * 判断Tag存不存在
     * @return 结果true or false
     */
    public boolean IsContainsTag(String TAG) {
        return TlvMap.containsKey(TAG.toUpperCase());
    }

    /**
     * 解EMV报文并保存到TAG
     */

    public void decodingEMVTLV(byte[] emv) {

        TlvMap.put(Filed55, emv);
        Log.e("hgd", "TLVBbyte:" + ByteUtils.byteArray2HexString(emv));
        String str = ByteUtils.byteArray2HexString(emv);

        if (str.equals("") || str.length() % 2 != 0) {
            throw new RuntimeException("Invalid tlv, null or odd length");
        }
        for (int i = 0; i < str.length(); ) {
            try {

                String tag = str.substring(i, i = i + 2);
                // extra byte for TAG field
                if ((Integer.parseInt(tag, 16) & 0x1F) == 0x1F) {
                    tag += str.substring(i, i = i + 2);
                }

                String len = str.substring(i, i = i + 2);
                int length = Integer.parseInt(len, 16);
                // more than 1 byte for length
                if (length > 128) {  //临界值，当是128即10000000时，长度还是一位，而不是两位
                    int bytesLength = length - 128;
                    len = str.substring(i, i = i + (bytesLength * 2));
                    length = Integer.parseInt(len, 16);
                }
                length *= 2;
                String value = str.substring(i, i = i + length);
                Log.e("hgd", "tag:" + tag + "TAG U:" + tag.toUpperCase() + " len:" + len + " value:" + value);
                TlvMap.put(tag.toUpperCase(), ByteUtils.hexString2ByteArray(value));
            } catch (NumberFormatException e) {
                throw new RuntimeException("Error parsing number", e);
            } catch (IndexOutOfBoundsException e) {
                throw new RuntimeException("Error processing field", e);
            }
        }
    }

    /**
     * @return 交易数据笔数
     */
    public static int getrecordNums() {
        return dbRecord.getrecordNums();
    }

    /**
     * 根据结算种类获得结算的笔数和总金额
     * -ZBQ
     * @param type SettleType枚举类型，表示结算种类
     * @return HashMap Count Amount
     */

    public HashMap settleResult(int type) {
        HashMap map;
        switch (type) {
            case SettleType.internalCardDebit:
                map = dbRecord.settleResult("0", "0");
                break;
            case SettleType.internalCardCredit:
                map = dbRecord.settleResult("1", "0");
                break;
            case SettleType.foreignCardDebit:
                map = dbRecord.settleResult("0", "1");
                break;
            case SettleType.foreignCardCredit:
                map = dbRecord.settleResult("1", "1");
                break;
            default:
                return null;
        }
        return map;
    }


    public static class SettleType {
        public static final int internalCardDebit = 0;     //内卡借计
        public static final int internalCardCredit = 1;    //内卡贷计
        public static final int foreignCardDebit = 2;      //外卡借计
        public static final int foreignCardCredit = 3;     //外卡贷计
    }

    /**
     * 根据交易类型统计交易总笔数
     * @param tradeID 交易ID 如："trade_id_sale"
     * @param state 0-未上送 1-上送成功 2-失败 3-上送被拒
     * @return 交易总笔数
     */
    public int getofflineCount(String tradeID,int state) {
        int count;
        HashMap tradeInfo = new HashMap();
        try {
            tradeInfo = dbRecord.settleStatistics(tradeID,state);
        } catch (Exception e) {
            e.printStackTrace();
        }

        if (tradeInfo == null) {
            count = 0;
        } else {
            count = (int) tradeInfo.get("Count");
        }
        return count;

    }

//    public void testSettle() {
//        ContentValues contentValues = new ContentValues();
//        String VoucherNO;
//
//        contentValues.clear();
//        VoucherNO = "000001";
//        contentValues.put(Amount, "000000012060");
//        contentValues.put(TradeTyFlag, "0");
//        contentValues.put(InOutCardFlag, "0");
//        dbRecord.update(contentValues, VoucherNO);
//
//        contentValues.clear();
//        VoucherNO = "000002";
//        contentValues.put(Amount, "000000011000");
//        contentValues.put(TradeTyFlag, "1");
//        contentValues.put(InOutCardFlag, "0");
//        dbRecord.update(contentValues, VoucherNO);
//
//        contentValues.clear();
//        VoucherNO = "000003";
//        contentValues.put(Amount, "000000021000");
//        contentValues.put(TradeTyFlag, "0");
//        contentValues.put(InOutCardFlag, "1");
//        dbRecord.update(contentValues, VoucherNO);
//
//        contentValues.clear();
//        VoucherNO = "000004";
//        contentValues.put(Amount, "000000031080");
//        contentValues.put(TradeTyFlag, "1");
//        contentValues.put(InOutCardFlag, "1");
//        dbRecord.update(contentValues, VoucherNO);
//
//        contentValues.clear();
//        VoucherNO = "000006";
//        contentValues.put(Amount, "000000001220");
//        contentValues.put(TradeTyFlag, "0");
//        contentValues.put(InOutCardFlag, "0");
//        dbRecord.update(contentValues, VoucherNO);
//        settleResult(SettleType.internalCardDebit);
//        settleResult(SettleType.internalCardCredit);
//        settleResult(SettleType.foreignCardDebit);
//        settleResult(SettleType.foreignCardCredit);
//    }

//    //交易详情表专用
//    private HashMap<String, String> recordmap = new HashMap<>();
//
//    /**
//     * 设置RecordTag函数
//     *
//     * @param TAG   只能是数据库TAG
//     * @param valve ASCII数组
//     * @return boolean  设置成功失败结果
//     */
//    public boolean setTagValueinRecord(String TAG, byte[] valve) {
//        if (!Recodrdlist.contains(TAG.toUpperCase())) {
//            log.error( "不是交易详情表TAG");
//            return false;
//        }
//        if (TAG.toUpperCase().equals(VoucherNo)) {
//            recordmap.put(VoucherNo, ByteUtils.asciiByteArray2String(valve));
//        }
//        if (recordmap.get(VoucherNo) == null) {
//            throw new RuntimeException("VoucherNo==null!");
//        }
//
//        return dbRecord.update(TAG.toUpperCase(), ByteUtils.asciiByteArray2String(valve), recordmap.get(VoucherNo));
//    }
//
//    /**
//     * 设置RecordTag函数
//     *
//     * @param tlv tlv报文     hexTag+length+Asciidata
//     * @return boolean  设置成功失败结果
//     */
//    public boolean setTagValueinRecord(byte[] tlv) {
//        if (recordmap.get(VoucherNo) == null) {
//            throw new RuntimeException("VoucherNo==null!");
//        }
//        HashMap temp = ParseTLV(tlv);
//        if (temp == null) {
//            return false;
//        }
//        ContentValues cv = new ContentValues();
//        Iterator keySet = temp.keySet().iterator();
//        while (keySet.hasNext()) {
//            String key = keySet.next().toString();
//            String valve = ByteUtils.asciiByteArray2String((byte[]) temp.get(key.toUpperCase()));
//            cv.put(key, valve);
//        }
//        return dbRecord.update(cv, recordmap.get(VoucherNo));
//    }
//
//    /**
//     * 设置RecordTag函数
//     *
//     * @param TAG （根据设置在内存的流水号查找）
//     * @return byte[] 数据库对应结果
//     */
//    public byte[] getTagValueinRecord(String TAG) {
//        String voucher = recordmap.get(VoucherNo);
//        if (TAG.toUpperCase().equals(VoucherNo)) {
//            return ByteUtils.string2ASCIIByteArray(voucher);
//        }
//        return ByteUtils.string2ASCIIByteArray(dbRecord.getData(TAG.toUpperCase(), voucher));
//    }
//
//    /**
//     * 设置RecordTag函数
//     *
//     * @param TAG     数据库TAG
//     * @param voucher 流水号
//     * @return byte[] 数据库对应结果
//     */
//    public byte[] getTagValueinRecord(String TAG, String voucher) {
//        return ByteUtils.string2ASCIIByteArray(dbRecord.getData(TAG.toUpperCase(), voucher));
//    }

    /**
     * 删除数据库交易详情记录
     */
    public int deleteAllRecord() {
        return dbRecord.deleteAll();
    }

    /**
     * 删除数据库交易某条记录
     *
     * @param Voucher 流水号
     */
    public boolean deleteRecord(String Voucher) {
        return dbRecord.delete(Voucher);
    }
//    /**
//     *获得结算信息
//     *将结算信息写进对应Tag
//     *
//     */
//    private void getSettleList() {
//        int increditnum = 0;//内卡贷记笔数
//        int indebitnum = 0;//内卡借记笔数
//        int oucreditnum = 0;//外卡贷记笔数
//        int oudebitnum = 0;//外卡借记笔数
//        long increditamti = 0;//内卡贷记金额
//        long indebitamti = 0;//内卡借记金额
//        long oucreditamti = 0;//外卡贷记金额
//        long oudebitamti = 0;//外卡借记金额
//        HashMap map;
//        map = settleResult(SettleType.internalCardCredit);
//        if (map != null) {
//            increditnum = (int) map.get("Count");
//            increditamti = (long) map.get("Amount");
//        }
//        map = settleResult(SettleType.internalCardDebit);
//        if (map != null) {
//            indebitnum = (int) map.get("Count");
//            indebitamti = (long) map.get("Amount");
//        }
//        map =settleResult(SettleType.foreignCardCredit);
//        if (map != null) {
//            oucreditnum = (int) map.get("Count");
//            oucreditamti = (long) map.get("Amount");
//        }
//        map =settleResult(SettleType.foreignCardDebit);
//        if (map != null) {
//            oudebitnum = (int) map.get("Count");
//            oudebitamti  = (long) map.get("Amount");
//        }
//
//
//        String increditamtstr = String.format("%012d", increditamti);
//        String oucreditamtstr = String.format("%012d", oucreditamti);
//        String indebitamtstr = String.format("%012d", indebitamti);
//        String oudebitamtstr = String.format("%012d", oudebitamti);
//
//        Log.e("settle",""+increditnum);
//        Log.e("settle",""+indebitnum);
//        Log.e("settle",""+oucreditnum);
//        Log.e("settle",""+oudebitnum);
//        Log.e("settle",""+increditamtstr);
//        Log.e("settle",""+oucreditamtstr);
//        Log.e("settle",""+indebitamtstr);
//        Log.e("settle",""+oudebitamtstr);
//
//        TlvMap.put(InCreCardAllNo, String.valueOf(increditnum).getBytes());
//        TlvMap.put(InDebCardAllNo, String.valueOf(indebitnum).getBytes());
//        TlvMap.put(OutCreCardAllNo, String.valueOf(oucreditnum).getBytes());
//        TlvMap.put(OutDebCardAllNo, String.valueOf(oudebitnum).getBytes());
//        TlvMap.put(InCreCardAllAmount, ByteUtils.hexString2ByteArray(increditamtstr));
//        TlvMap.put(OutCreCardAllAmount, ByteUtils.hexString2ByteArray(oucreditamtstr));
//        TlvMap.put(InDebCardAllAmount, ByteUtils.hexString2ByteArray(indebitamtstr));
//        TlvMap.put(OutDebCardAllAmount, ByteUtils.hexString2ByteArray(oudebitamtstr));
//    }

    /**
     * 根据流水号setTag 只能设置交易详情表
     *
     * @param VoucherN0 流水号
     * @param TAG       tag
     * @param Value     值   报文hex, 其他ASCII
     * @return 结果 true 或 false
     */
    public static boolean setTagValueinRecord(String VoucherN0, String TAG, byte[] Value) {
        if (VoucherN0 == null || TAG == null || Value == null) {
            return false;
        }
        if (!(dbRecord.isVoucherNOExist(VoucherN0) == 1)) {
            return false;
        }
        String value = null;
        if (TAG.toUpperCase().equals(SendMessage) || TAG.toUpperCase().equals(RecvMessage) || TAG.toUpperCase().equals(Filed55)) {
            value = ByteUtils.byteArray2HexString(Value);
            log.debug("TAG{},报文转换：{}", TAG, value);
        } else {
            value = ByteUtils.asciiByteArray2String(Value);
            log.debug("TAG{},其他详情转换：{}", TAG, value);
        }
        return dbRecord.update(TAG.toUpperCase(), value, VoucherN0);
    }

    /**
     * 清除交易冲正
     */
    public void clearReavalInfo() {
        setTagValue(ReveralMes, "".getBytes());
        setTagValue(ReveralNeed, "0".getBytes());
    }

    /**
     * 流水号获取一条交易记录 重新填TAG
     *
     * @param index
     * @return result
     */
    public static boolean getOneTradeRecordByIndex(int index) {

        ContentValues recordmap = dbRecord.getOneRecordByIndex(index);
        if (recordmap == null) {
            return false;
        }
        //清内存前保存tradeid
        byte[] curtradeid = TlvMap.get(TradeId);
        //清内存
        TlvMap.clear();
        //还原tradeid
        TlvMap.put(TradeId, curtradeid);
        for (Map.Entry<String, Object> tag : recordmap.valueSet()) {
            if (!tag.getValue().equals("")) {
                if (tag.getKey().equals("PID")) {
                    continue;
                } else if (tag.getKey().equals(TradeId)) {
                    TlvMap.put(OriTradeId, tag.getValue().toString().getBytes());
                } else if (tag.getKey().equals(VoucherNo)) {
                    TlvMap.put(OriVoucherNo, tag.getValue().toString().getBytes());

                } else if (tag.getKey().equals(RecvMessage) ||
                        tag.getKey().equals(SendMessage) ||
                        tag.getKey().equals(Filed55)) {
                    TlvMap.put(tag.getKey(), ByteUtils.hexString2ByteArray(tag.getValue().toString()));
                    log.debug("TAG报文:{},value:{}", tag.getKey(), tag.getValue().toString());
                } else {
                    TlvMap.put(tag.getKey().toUpperCase(), tag.getValue().toString().getBytes());
                    log.debug("TAG:{},value:{}", tag.getKey().toUpperCase(), new String(tag.getValue().toString().getBytes()));
                }
            } else {
                TlvMap.put(tag.getKey(), null);
                log.debug("TAGvalue空:{}", tag.getKey());

            }
        }
        return true;
    }

}