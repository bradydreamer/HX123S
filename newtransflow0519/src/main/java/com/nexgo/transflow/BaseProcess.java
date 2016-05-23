package com.nexgo.transflow;

import android.app.Dialog;
import android.content.Context;
import android.os.Bundle;
import android.os.Message;
import android.os.OperationCanceledException;
import android.os.RemoteException;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.nexgo.R;
import com.nexgo.common.ByteUtils;
import com.nexgo.common.GlobalHolder;
import com.nexgo.common.SecAuthResult;
import com.nexgo.common.TradeTlv;
import com.nexgo.common.utils.TradeData;
import com.nexgo.common.utils.TradeHelper;
import com.nexgo.jniinterface.CallNative;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.pinpad.OnPinPadInputListener;
import com.nexgo.smartpos.api.device.pinpad.PinAlgorithmMode;
import com.nexgo.smartpos.api.device.pinpad.PinPad;
import com.nexgo.smartpos.api.device.reader.icc.IccCardReader;
import com.nexgo.smartpos.api.device.reader.icc.IccCardType;
import com.nexgo.smartpos.api.device.reader.icc.IccReaderSlot;
import com.nexgo.smartpos.api.device.reader.icc.OnSearchIccCardListener;
import com.nexgo.smartpos.api.device.reader.mag.MagCardInfoEntity;
import com.nexgo.smartpos.api.device.reader.mag.MagCardReader;
import com.nexgo.smartpos.api.device.reader.mag.OnSearchMagCardListener;
import com.nexgo.smartpos.api.emv.EmvChannelType;
import com.nexgo.smartpos.api.emv.EmvDataSource;
import com.nexgo.smartpos.api.emv.EmvHandler;
import com.nexgo.smartpos.api.emv.EmvOnlineRequest;
import com.nexgo.smartpos.api.emv.EmvProcessResult;
import com.nexgo.smartpos.api.emv.EmvTransDataConstrants;
import com.nexgo.smartpos.api.emv.EmvTransFlow;
import com.nexgo.smartpos.api.emv.OnEmvProcessListener;
import com.nexgo.smartpos.api.engine.DeviceServiceEngine;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.text.DecimalFormat;
import java.text.SimpleDateFormat;
import java.util.Arrays;
import java.util.Date;
import java.util.List;

import de.greenrobot.event.EventBus;
import rx.android.schedulers.AndroidSchedulers;
import rx.functions.Action1;
import rx.subjects.BehaviorSubject;

/**
 * Created by xiaox on 16/3/12.
 */
abstract class BaseProcess implements IProcess {
    /*组包和发数据*/
    public TradeTlv tradeTlv = null;
    byte[] package8583 = new byte[1024];
    byte[] jplen = new byte[2];
    byte[] tlvdata = new byte[512];
    byte[] transAmt = new byte[12];
    byte[] merId = new byte[15];
    byte[] termId = new byte[8];
    byte[] merName = new byte[64];
    byte[] mkeyId = new byte[2];
    protected boolean IsInputCardno = false;

    public BaseProcess() {
        tradeTlv = TradeTlv.getInstance();
        EventBus.getDefault().register(this);
        //获取时间并设置到TAG里，以这个时间为准
        date = (new SimpleDateFormat("yyyyMMdd").format(new Date())).getBytes();
        time = (new SimpleDateFormat("hhmmss").format(new Date())).getBytes();
        tradeTlv.setTagValue(TradeTlv.TraDate, date);
        tradeTlv.setTagValue(TradeTlv.TraTime, time);
    }

    /**
     * 设置是否手输卡号属性
     */
    public void setIfmanual() {
        byte[] payway = tradeTlv.getTagValue(TradeTlv.manualpay);
        byte[] manualpay = new byte[]{'1'};//手输卡号：1
        if (payway != null) {
            if (Arrays.equals(payway, manualpay)) {
                this.IsInputCardno = true;
            } else {
                this.IsInputCardno = false;
            }
        } else {
            this.IsInputCardno = false;
        }
    }

    public void PackageAndSend() {
        byte[] senddata = null;
        int len = CallNative.Package(null, 0, null, null);//改了模式，入参出参暂时预留，暂不用到
        if (len > 0) {
            //华夏银行--begain
//            processListener.onOnlineRequest(tradeTlv.getTagValue(TradeTlv.SendMessage));
            processListener.onOnlineRequest(tradeTlv.getTagValue(TradeTlv.SendEncrptMessage));
            //华夏银行--end

        } else {
            log.debug("baba:组包错，返回：{}", len);
            processListener.onFinish(TransResult.ERROR, null, 0);
            return;
        }

    }

    //  0x00 OK,0x01:timeout  0x02 :interrut ,0x03 connect err 0x99 other err
    public int AnalyzePackage(byte[] respData, int linkstate, byte[] errtip, byte[] len) {
        byte[] datalen = new byte[2];
        byte[] tradeid = tradeTlv.getTagValue(TradeTlv.TradeId);
        len[0] = 0;
        switch (linkstate) {
            case 0:
                //华夏银行--begain
//                tradeTlv.setTagValue(TradeTlv.RecvMessage, respData);
                tradeTlv.setTagValue(TradeTlv.RecvEncrptMessage, respData);
                //华夏银行--end
                int ret = CallNative.UnPackage(new byte[]{0x00}, 1, errtip, datalen);
                len[0] = datalen[0];
                return ret;

            case 1:
                return CallNative.UnPackage(new byte[]{0x01}, 1, null, null);

            case 2:
                return CallNative.UnPackage(new byte[]{0x02}, 1, null, null);

            case 3:
                return CallNative.UnPackage(new byte[]{0x03}, 1, null, null);

            default:
                return CallNative.UnPackage(new byte[]{(byte) 0x99}, 1, null, null);

        }

    }

    protected IProcessListener processListener;
    protected static Context context;
    protected DeviceServiceEngine deviceServiceEngine;
    protected PinPad pinPad;
    protected EmvHandler emvHandler;
    protected Bundle emvBundle;
    protected MagCardReader magCardReader;
    protected IccCardReader icCardReader;
    protected IccCardReader rfCardReader;
    private Logger log = LoggerFactory.getLogger("BaseProcess");
    protected BehaviorSubject<LocalEvent> subject;
    protected boolean isSample = false;//是否简化流程
    protected boolean isContact = false;
    protected boolean isMagCard = false;
    public byte[] date = new byte[8];
    private byte[] time = new byte[6];

    protected String pan;
    protected static android.os.Handler handler = new android.os.Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case 0x00: {
                    Toast.makeText(context, msg.obj.toString(), Toast.LENGTH_SHORT).show();
                    break;
                }

            }
        }
    };

    public void tradetip(String tip) {
        Message message = handler.obtainMessage();
        message.what = 0x00;
        message.obj = tip;
        handler.sendMessage(message);
    }

    protected void doError() {
        if (processListener != null) {
            processListener.onFinish(TransResult.DEVICE_ERR, null, 0);
        }
    }

    /**
     * 取消寻卡
     */
    protected void CancelSearchcard() {
        try {
            if (magCardReader != null) magCardReader.stopSearch();
            if (icCardReader != null) icCardReader.stopSearch();
            if (rfCardReader != null) rfCardReader.stopSearch();
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    protected void doCancel() {
        try {
            if (magCardReader != null) magCardReader.stopSearch();
            if (icCardReader != null) icCardReader.stopSearch();
            if (rfCardReader != null) rfCardReader.stopSearch();
            if (emvHandler != null) {
                if (curEvent == LocalEvent.SEL_APP) emvHandler.onSetSelAppResponse(0);
                if (curEvent == LocalEvent.SHOW_PAN) emvHandler.onSetConfirmCardNoResponse(false);
                if (curEvent == LocalEvent.ONLINE_REQUEST)
                    emvHandler.onSetOnlineProcResponse(ServiceResult.Fail, null);
                if (curEvent == LocalEvent.TRISK_MANAGE) emvHandler.onSetTRiskManageResponse(null);
                if (curEvent == LocalEvent.CER_TVERIFY) emvHandler.onSetCertVerifyResponse(false);
            }
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        if (processListener != null) {
            processListener.onFinish(TransResult.CANCEL, null, 0);
        }
    }

    protected void init() {
        try {
            deviceServiceEngine = GlobalHolder.getInstance().getDeviceServiceEngine();
            pinPad = deviceServiceEngine.getPinPad();
            magCardReader = deviceServiceEngine.getMagCardReader();
            icCardReader = deviceServiceEngine.getIccCardReader(IccReaderSlot.ICSlOT1);
            rfCardReader = deviceServiceEngine.getIccCardReader(IccReaderSlot.RFSlOT);
            pinPad.setSupportPinLen(new int[]{0x00, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c});

        } catch (RemoteException e) {
            e.printStackTrace();
            doError();
        }
    }

    protected void doProcess(LocalEvent event) {
        switch (event) {
            case SEARCH_CARD:
                doSearchCard();
                break;
            case Input_CARDNO:
                inputcardno();
                break;
            case START_EMV:
                doStartEmv();
                break;
            case SHOW_PAN:
                doShowPan();
                break;
            case PIN_INPUT:
                doPinInput();
                break;
            case ONLINE_REQUEST:
                doOnlineRequest();
                break;
            case EC_BALANCE:
                getEcBalance();
                break;
            case SEL_APP:
                break;
            case CER_TVERIFY:
                break;
            case TRISK_MANAGE:
                break;
            default:
                break;
        }
    }

    private void getEcBalance() {
        byte[] ecbalance = new byte[6];//QPASS 取电子现金余额
        try {
            ecbalance = emvHandler.getTlvs(new byte[]{(byte) 0x9F, 0x79}, EmvDataSource.FROMKERNEL);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        if (ecbalance != null) {
            tradeTlv.setTagValue(TradeTlv.ECBanlance, ecbalance);
            processListener.onFinish(TransResult.OFFLINE_ACCEPTED, null, 0);
        } else {
            processListener.onFinish(TransResult.ERROR, null, 0);
        }

    }

    protected void saveOfflineRec(int retcode) {
        byte[] senddata = null;
        if ((retcode == ServiceResult.Emv_Qpboc_Offline || retcode == ServiceResult.Emv_Ec_Accept)) {
            tradeTlv.setTagValue(tradeTlv.OfflineRes, new byte[]{'0', '0'});
        } else {
            tradeTlv.setTagValue(tradeTlv.OfflineRes, new byte[]{'0', '1'});
        }
        int len = CallNative.Package(null, 0, null, null);//
        log.debug("BaseProcess 保存脱机记录，返回：{}", len);
        //int len = 1;
        if ((retcode == ServiceResult.Emv_Qpboc_Offline || retcode == ServiceResult.Emv_Ec_Accept) && len == 1) {
            CallNative.formPrintScript(CallNative.SIGN_BILL);
            processListener.onFinish(TransResult.OFFLINE_ACCEPTED, null, 0);

        } else {
            processListener.onFinish(TransResult.ERROR, null, 0);
        }
    }

    private void doOnlineRequest() {
        PackageAndSend();
    }

    protected void doStartEmv() {
        log.debug("开始PBOC");
        ///test
        byte[] TradeId = tradeTlv.getTagValue(tradeTlv.TradeId);
        try {
            emvHandler = deviceServiceEngine.getEmvHandler();
            emvBundle = new Bundle();
            emvBundle.putString(EmvTransDataConstrants.TRANSAMT, "000000000000"); //金额初始化
            emvBundle.putString(EmvTransDataConstrants.TRANSDATE, new String(date));
            emvBundle.putString(EmvTransDataConstrants.TRANSTIME, new String(time));
            emvBundle.putString(EmvTransDataConstrants.MERNAME, new String(merName));
            emvBundle.putString(EmvTransDataConstrants.MERID, new String(merId));
            emvBundle.putString(EmvTransDataConstrants.TERMID, new String(termId));
            emvBundle.putBoolean(EmvTransDataConstrants.ISSUPPORTEC, false);
            emvBundle.putInt(EmvTransDataConstrants.PROCTYPE, EmvTransFlow.FULL);
            emvBundle.putInt(EmvTransDataConstrants.MKEYIDX, Integer.valueOf(new String(mkeyId)));
            if (isContact) {
                emvBundle.putInt(EmvTransDataConstrants.CHANNELTYPE, EmvChannelType.FROM_ICC);
            } else {
                emvBundle.putInt(EmvTransDataConstrants.CHANNELTYPE, EmvChannelType.FROM_PICC);
                emvBundle.putBoolean(EmvTransDataConstrants.ISQPBOCFORCEONLINE, true);

            }
            mConfigEmvParam();
            emvHandler.emvProcess(emvBundle, emvProcessListener);

        } catch (
                RemoteException e
                )

        {
            e.printStackTrace();
        }
    }

    private void inputcardno() {
        tradeTlv.setTagValue(TradeTlv.PayWay, new byte[]{'0', '3'});
        byte cardno[] = tradeTlv.getTagValue(TradeTlv.CardNo);
        if (cardno != null) {
            pan = new String(cardno);
        }
        subject.onNext(LocalEvent.SHOW_PAN);

    }

    protected void doSearchCard() {
        try {
            magCardReader.searchCard(magCardListener, 60);
            icCardReader.searchCard(icCardListener, 60, new String[]{IccCardType.CPUCARD});
            rfCardReader.searchCard(rfCardListener, 60, new String[]{IccCardType.CPUCARD});
        } catch (RemoteException e) {
            doError();
        }
    }

    protected void doShowPan() {
        log.debug("显示卡号");
        tradeTlv.setTagValue(TradeTlv.CardNo, pan.getBytes());
        DecimalFormat df = new DecimalFormat("0.00");
        String amountDecimal = df.format(Double.valueOf(new String(transAmt)) / 100);
        final Dialog dialog = new Dialog(context);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);
        dialog.setCanceledOnTouchOutside(false);//点击弹框外不退出
        dialog.setCancelable(false);
        LayoutInflater layoutInflater = LayoutInflater.from(context);
        View view = layoutInflater.inflate(ResContainer.getResourceId(context, ResContainer.ResType.LAYOUT, "activity_confirm_card_no"), null);
        dialog.setContentView(view);
        dialog.show();
        TextView amount = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "amount")));
        if (amount != null) {
            amount.setText(amountDecimal + context.getString(R.string.amtflag));
        }
        TextView cardno = ((TextView) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cardno")));
        if (cardno != null) {
            cardno.setText(pan);
        }
        Button confirm = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "confirm")));
        if (confirm != null) {
            confirm.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    dialog.dismiss();
                    try {
                        if (isMagCard || IsInputCardno) {
                            subject.onNext(LocalEvent.PIN_INPUT);
                        } else {
                            emvHandler.onSetConfirmCardNoResponse(true);
                        }
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                }
            });
        }
        Button cancel = ((Button) view.findViewById(ResContainer.getResourceId(context, ResContainer.ResType.ID, "cancel")));
        if (cancel != null) {
            cancel.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    dialog.dismiss();
                    if (isMagCard || IsInputCardno) {
                        doCancel();
                    } else {
                        try {
                            emvHandler.onSetConfirmCardNoResponse(false);
                        } catch (RemoteException e) {
                            e.printStackTrace();
                        }
                    }
                }
            });
        }
    }

    protected void doPinInput() {
        try {
            pinPad.inputOnlinePin(tradeTlv.getTagValue(TradeTlv.CardNo), Integer.valueOf(new String(mkeyId)), PinAlgorithmMode.ISO9564FMT1, pinPadInputListener);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    private LocalEvent curEvent;

    protected void startonline() {
//        byte[] cardno = tradeTlv.getTagValue(TradeTlv.CardNo);
//        if (cardno != null) {
//            pan = new String(cardno);
//        }
        transAmt = tradeTlv.getTagValue(TradeTlv.Amount);
        mkeyId = tradeTlv.getTagValue(TradeTlv.Tmkindex);
        subject = BehaviorSubject.create(LocalEvent.PIN_INPUT);
        subject.asObservable()
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<LocalEvent>() {
                               @Override
                               public void call(LocalEvent localEvent) {
                                   curEvent = localEvent;
                                   doProcess(localEvent);
                               }
                           },
                        new Action1<Throwable>() {
                            @Override
                            public void call(Throwable throwable) {
                                if (throwable instanceof OperationCanceledException) {
                                    doCancel();
                                } else {
                                    doError();
                                }
                            }
                        });
    }

    protected void startTrade() {
        transAmt = tradeTlv.getTagValue(TradeTlv.Amount);
        merId = tradeTlv.getTagValue(TradeTlv.TerminalNo);
        termId = tradeTlv.getTagValue(TradeTlv.MerchantNo);
        merName = tradeTlv.getTagValue(TradeTlv.MerchantName);
        mkeyId = tradeTlv.getTagValue(TradeTlv.Tmkindex);
        if (IsInputCardno) {
            subject = BehaviorSubject.create(LocalEvent.Input_CARDNO);
        } else {
            subject = BehaviorSubject.create(LocalEvent.SEARCH_CARD);
        }


        subject.asObservable()
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(new Action1<LocalEvent>() {
                               @Override
                               public void call(LocalEvent localEvent) {
                                   curEvent = localEvent;
                                   doProcess(localEvent);
                               }
                           },
                        new Action1<Throwable>() {
                            @Override
                            public void call(Throwable throwable) {
                                if (throwable instanceof OperationCanceledException) {
                                    doCancel();
                                } else {
                                    doError();
                                }
                            }
                        });
    }

    OnSearchMagCardListener magCardListener = new OnSearchMagCardListener.Stub() {
        @Override
        public void onSearchResult(int retCode, MagCardInfoEntity mcie) throws RemoteException {
            if (retCode == ServiceResult.Success) {
                log.debug("寻到磁卡");
                if (TradeHelper.iswithIc(mcie.getTk2())) {
                    useic();
                    return;
                }
                isMagCard = true;
                StringBuilder sb;
                //二磁道
                if (mcie.getTk2() != null) {
                    tradeTlv.setTagValue(TradeTlv.PayWay, new byte[]{'0', '0'});
                    sb = new StringBuilder(mcie.getTk2().toUpperCase().replace('=', 'D'));
                    tradeTlv.setTagValue(TradeTlv.Tk2, sb.toString().getBytes());

                } else {
                    // subject.onError(new Exception("磁条卡格式出错"));
                    tradetip(context.getString(R.string.readcarderr));
                    doSearchCard();
                    return;
                }
                //三磁道
                if (mcie.getTk3() != null) {
                    sb = new StringBuilder(mcie.getTk3().toUpperCase().replace('=', 'D'));
                    tradeTlv.setTagValue(TradeTlv.Tk3, sb.toString().getBytes());
                }
//                //Pan
                int idx = mcie.getTk2().toUpperCase().replace('=', 'D').indexOf('D');
                pan = mcie.getTk2().substring(0, idx);
                subject.onNext(LocalEvent.SHOW_PAN);
            } else {
                subject.onError(new Exception("磁条卡寻卡出错"));
            }
        }
    };
    OnSearchIccCardListener icCardListener = new OnSearchIccCardListener.Stub() {
        @Override
        public void onSearchResult(int retCode, Bundle bundle) throws RemoteException {
            if (retCode == ServiceResult.Success) {
                log.debug("寻到接触ic");
                tradetip(context.getString(R.string.icoperating));
                isContact = true;
                tradeTlv.setTagValue(TradeTlv.PayWay, new byte[]{'0', '1'});
                subject.onNext(LocalEvent.START_EMV);
            } else {
                subject.onError(new Exception("IC卡寻卡出错"));
            }
        }
    };
    OnSearchIccCardListener rfCardListener = new OnSearchIccCardListener.Stub() {
        @Override
        public void onSearchResult(int retCode, Bundle bundle) throws RemoteException {
            if (retCode == ServiceResult.Success) {
                log.debug("寻到非接ic");
                tradeTlv.setTagValue(TradeTlv.PayWay, new byte[]{'0', '2'});
                subject.onNext(LocalEvent.START_EMV);
            } else {
                subject.onError(new Exception("RF卡寻卡出错"));
            }
        }
    };
    OnEmvProcessListener emvProcessListener = new OnEmvProcessListener.Stub() {

        @Override
        public void onSelApp(List<String> appNameList, boolean isFirstSelect) throws RemoteException {
            subject.onNext(LocalEvent.SEL_APP);
        }

        @Override
        public void onConfirmCardNo(String cardNo) throws RemoteException {
            mConfirmCardNo(cardNo);

        }

        @Override
        public void onCardHolderInputPin(boolean isOnlinePin, int leftTimes) throws RemoteException {
            //不处理
        }

        @Override
        public void onPinPress(byte keyCode) throws RemoteException {
            //不处理
        }

        @Override
        public void onCertVerify(String certName, String certInfo) throws RemoteException {
            subject.onNext(LocalEvent.CER_TVERIFY);
        }

        @Override
        public void onOnlineProc(Bundle data) throws RemoteException {
            log.debug("onOnlineProc");
            tradeTlv.setTagValue(TradeTlv.Pin, data.getByteArray(EmvOnlineRequest.PIN));
            byte[] sn = data.getByteArray(EmvOnlineRequest.CARDSN);
            tradeTlv.setTagValue(TradeTlv.CardSN, ByteUtils.byteArray2HexString(sn).getBytes());//序列号转成ASC发给JNI层
            byte[] F55data = TradeHelper.form55Field();
            tradeTlv.setTagValue(TradeTlv.Filed55, F55data); //接触全流程保存55域数据
            subject.onNext(LocalEvent.ONLINE_REQUEST);
        }

        @Override
        public void onFinish(int retCode, Bundle data) throws RemoteException {
            mFinish(retCode, data);
        }

        @Override
        public void onSetAIDParameter(String aid) throws RemoteException {
            //不处理
        }

        @Override
        public void onSetCAPubkey(String rid, int index, int algMode) throws RemoteException {
            //不处理
        }

        @Override
        public void onTRiskManage(String pan, String panSn) throws RemoteException {
            emvHandler.onSetTRiskManageResponse(null);
        }
    };
    OnPinPadInputListener pinPadInputListener = new OnPinPadInputListener.Stub() {
        @Override
        public void onInputResult(int retCode, byte[] data) throws RemoteException {
            if (retCode == ServiceResult.Success) {
                log.debug("联机交易");
                log.debug("保存密码密文：{}", ByteUtils.byteArray2HexString(data));
                tradeTlv.setTagValue(TradeTlv.Pin, data);//保存密钥密文
                subject.onNext(LocalEvent.ONLINE_REQUEST);
            } else if (retCode == ServiceResult.PinPad_No_Pin_Input) {
                log.debug("输入空");
                tradeTlv.setTagValue(TradeTlv.Pin, null);//保存密钥密文
                subject.onNext(LocalEvent.ONLINE_REQUEST);
            } else if (retCode == ServiceResult.PinPad_Input_Cancel) {
                log.debug("取消输入");
                subject.onError(new OperationCanceledException("取消密码输入"));
            } else {
                log.debug("交易失败");
                subject.onError(new Exception("输入密码错误"));
            }
        }

        @Override
        public void onSendKey(byte keyCode) throws RemoteException {
            //不处理
        }
    };

    public void onEventMainThread(Bundle bundle) {
        log.debug("onEventMainThread,onSetOnlineProcResponse");
        try {
            emvHandler.onSetOnlineProcResponse(0, bundle);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
    }

    protected enum LocalEvent {
        ONLINE_REQUEST,
        SEARCH_CARD,
        Input_CARDNO,
        SHOW_PAN,
        PIN_INPUT,
        START_EMV,
        SEL_APP,
        CER_TVERIFY,
        TRISK_MANAGE,
        EC_BALANCE
    }

    protected void mConfirmCardNo(String cardNo) {
        pan = cardNo;
        //保存磁道数据
        TradeData td = TradeData.getInstance();
        String tk2 = td.getTk2();
        if (tk2 != null) tradeTlv.setTagValue(TradeTlv.Tk2, tk2.getBytes());
        if (isSample || (!isContact)) {//如果是接触简化流程或者非接则获取55域数据  全流程在输完密码后获取55域数据
            byte[] F55data = TradeHelper.form55Field();
            tradeTlv.setTagValue(TradeTlv.Filed55, F55data);
        }
        subject.onNext(LocalEvent.SHOW_PAN);
    }

    protected void useic() {
        tradetip(context.getString(R.string.useic));
        doSearchCard();
    }

    protected void mConfigEmvParam() {

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
                    byte[] outc = emvHandler.getTlvs("9F26".getBytes(), EmvDataSource.FROMKERNEL);
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


}
