package com.nexgo.transflow;

import android.content.Context;
import android.os.Bundle;
import android.os.OperationCanceledException;
import android.os.RemoteException;

import com.nexgo.R;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.SecAuthResult;
import com.nexgo.common.TradeTlv;
import com.nexgo.common.utils.TradeData;
import com.nexgo.common.utils.TradeHelper;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvProcessResult;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;
import java.util.List;

import de.greenrobot.event.EventBus;

/**
 * Created by caibaqun on 2016/5/4.
 */
public class ECSaleTrade extends BaseProcess {
    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("ECSaleTrade");

    public ECSaleTrade(Context context) {
        this.context = context;
        tradeTlv = TradeTlv.getInstance();
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        processListener.onStart();
        init();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {
        byte[] errtip = new byte[256];
        byte[] len = new byte[2];
        int ret = AnalyzePackage(data, linkstate, errtip, len);//收到数据，调用解包
        log.debug("消费返回 ret={}", ret);
        if (ret == CallNative.NOT_END) {//
            PackageAndSend();//组包并回调
        } else if (ret == CallNative.OK) {
            CallNative.formPrintScript(CallNative.SIGN_BILL);
            processListener.onFinish(TransResult.APPROVED, errtip, len[0]);
        } else {
            processListener.onFinish(TransResult.ERROR, errtip, len[0]);
        }
    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }
    protected void doSearchCard() {
        try {
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
            rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }
    @Override
    protected void mFinish(int retCode, Bundle data) throws RemoteException {
        log.debug("onFinish");
        switch (retCode) {
            case ServiceResult.Emv_Qpboc_Online: {
                log.debug("qpboc联机");
                tradeTlv.setTagValue(TradeTlv.TradeId, context.getString(R.string.SaleID).getBytes());//电子现金转联机把ID设回SALE
                byte[] track2 = emvHandler.getTlvs(new byte[]{0x57}, EmvDataSource.FROMKERNEL);//QPASS 电子现金转联机没有确认卡号
                if (track2 != null) {
                    String strTrack2 = ByteUtils.byteArray2HexString(track2);
                    int idx = strTrack2.toUpperCase().replace('=', 'D').indexOf('D');
                    pan = strTrack2.substring(0, idx);
                }
                subject.onNext(LocalEvent.PIN_INPUT);
            }
            break;
            case ServiceResult.Emv_Qpboc_Offline:
            case ServiceResult.Emv_Ec_Accept:
            case ServiceResult.Emv_Auth_Fail:
            case ServiceResult.Emv_Ec_Decliend: {
                subject.onCompleted();
                byte[] ecbalance = emvHandler.getTlvs(new byte[]{(byte) 0x9F, 0x79}, EmvDataSource.FROMCARD);//QPASS 取电子现金余额
                tradeTlv.setTagValue(TradeTlv.ECBanlance, ecbalance);
                if (!isContact) {//QPASS 闪付没确卡号，手动获取卡号保存
                    byte[] track2 = emvHandler.getTlvs(new byte[]{0x57}, EmvDataSource.FROMKERNEL);
                    if (track2 != null) {
                        String strTrack2 = ByteUtils.byteArray2HexString(track2);
                        int idx = strTrack2.toUpperCase().replace('=', 'D').indexOf('D');
                        pan = strTrack2.substring(0, idx);
                        tradeTlv.setTagValue(TradeTlv.CardNo, pan.getBytes());
                    }
                }
                if(isForeignCard()){
                    tradeTlv.setTagValue(TradeTlv.InOutCardFlag,"1".getBytes());
                }else{
                    tradeTlv.setTagValue(TradeTlv.InOutCardFlag,"0".getBytes());
                }
                saveOfflineRec(retCode);
            }
            break;
            case ServiceResult.Success:
            case ServiceResult.Emv_Offline_Accept:
                if (isSample) {
                    log.debug("pboc简化流程联机");
                    subject.onNext(LocalEvent.PIN_INPUT);
                } else {
                    log.debug("交易成功");
                    byte[] outc = emvHandler.getTlvs("9F26".getBytes(), EmvDataSource.FROMCARD);
                    EventBus.getDefault().post(new SecAuthResult(retCode, data.getByteArray(EmvProcessResult.SCRIPTRESULT), outc));
                    subject.onCompleted();
                }
                break;
            case ServiceResult.Emv_Cancel:
                subject.onError(new OperationCanceledException("交易取消"));
                subject.onCompleted();
                break;
            default:
                log.debug("交易失败");
                EventBus.getDefault().post(new SecAuthResult(retCode, null, null));
                subject.onError(new Exception("交易失败"));
                subject.onCompleted();
                break;
        }
    }
    @Override
    protected void mConfigEmvParam() {
        if (isContact) {
            emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.FULL);
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
            emvBundle.putBoolean(EmvTransDataConstrants.ISQPBOCFORCEONLINE, false);
            emvBundle.putBoolean(EmvTransDataConstrants.ISSUPPORTEC, true);
        } else {
            emvBundle.putString(EmvTransDataConstrants.TRANSAMT, new String(transAmt)); //金额
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.QPASS);
            emvBundle.putByte(EmvTransDataConstrants.B9C, (byte) 0x00);
            emvBundle.putBoolean(EmvTransDataConstrants.ISQPBOCFORCEONLINE, false);
            emvBundle.putBoolean(EmvTransDataConstrants.ISSUPPORTEC, true);
        }
    }

    /**
     * 电子现金是否内外卡交易
     * @return
     */
    private boolean isForeignCard(){
        try {
            byte[] firstCurrencyCode = emvHandler.getTlvs(new byte[]{(byte) 0x9F, 0x51}, EmvDataSource.FROMCARD);
            byte[] secondCurrencyCode = emvHandler.getTlvs(new byte[]{(byte) 0x9F, 0x71}, EmvDataSource.FROMCARD);//QPA
            if((firstCurrencyCode==null&&secondCurrencyCode==null)||(Arrays.equals(firstCurrencyCode,"".getBytes())&&Arrays.equals(secondCurrencyCode,"".getBytes()))) {
                return false;//找不到货币代码，默认为内卡
            }
            if(firstCurrencyCode!=null){
                if(Arrays.equals(new byte[]{0x01,0x56},firstCurrencyCode)){
                    return false;
                }
            }
            if(secondCurrencyCode!=null){
                if(Arrays.equals(new byte[]{0x01,0x56},firstCurrencyCode)){
                    return false;
                }
            }

        } catch (RemoteException e) {
            e.printStackTrace();
        }
        return true;
    }

}
