package com.nexgo.transflow;

import android.content.Context;
import android.os.Bundle;
import android.os.OperationCanceledException;
import android.os.RemoteException;

import com.nexgo.R;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.SecAuthResult;
import com.nexgo.common.TradeTlv;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.emv.EmvChannelType;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvProcessResult;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.Arrays;

import de.greenrobot.event.EventBus;

/**
 * Created by safin on 2016/5/10.
 */
public class ECQueryTrade extends BaseProcess {

    TradeTlv tradeTlv = null;
    private Logger log = LoggerFactory.getLogger("ECQueryTrade");

    public ECQueryTrade(Context context) {
        ECQueryTrade.context = context;
        tradeTlv = TradeTlv.getInstance();
    }

    @Override
    public void ProcessStart(IProcessListener processListener) {
        this.processListener = processListener;
        init();
        processListener.onStart();
        startTrade();
    }

    @Override
    public void ProcessContinue(IProcessListener processListener) {

    }

    @Override
    public void onSetOnlineResponse(byte[] data, int linkstate) {

    }

    @Override
    public void onSetCancel() {
        subject.onError(new OperationCanceledException());
    }

    protected void mFinish(int retCode, Bundle data) throws RemoteException {
        log.debug("onFinish");
        switch (retCode) {
            case ServiceResult.Emv_Qpboc_Online: {
                log.debug("qpboc联机");
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
                    subject.onNext(LocalEvent.EC_BALANCE);
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
    protected void doSearchCard() {
        try {
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
            rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }
    protected void doStartEmv() {
        log.debug("开始PBOC");
        ///test
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        try {
            emvHandler = deviceServiceEngine.getEmvHandler();
            if (isContact) {
                emvHandler.emvGetEcBalance(emvProcessListener, EmvChannelType.FROM_ICC);
            } else {
                emvHandler.emvGetEcBalance(emvProcessListener, EmvChannelType.FROM_PICC);
            }
          //  emvHandler.emvProcess(emvBundle, emvProcessListener);
        } catch (RemoteException e) {
            e.printStackTrace();
        }

    }
}
