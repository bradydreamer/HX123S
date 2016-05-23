package com.hxb.pos.device.activity;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.os.Handler;
import android.os.Message;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import com.hxb.pos.device.R;
import com.hxb.pos.device.fragment.ResponseFailFragment;
import com.hxb.pos.device.fragment.ResponseSuccessFragment;
import com.hxb.pos.device.utils.AppGlobal;
import com.nexgo.common.PrintScript;
import com.nexgo.common.TradeTlv;
import com.nexgo.smartpos.api.ServiceResult;
import com.nexgo.smartpos.api.device.printer.OnPrintListener;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.lang.ref.WeakReference;

/**
 * 作者:liuting on 2016/3/17 17:18
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.activity
 * TODO: 响应结果界面
 */
public class ResponseResultActivity extends FragmentActivity implements View.OnClickListener {
    private TextView mTv_count;
    private boolean success;
    private String tip;
    private TradeTlv tradeTlv;
    private Logger log = LoggerFactory.getLogger("ResponseResultActivity");
    private ProgressDialog proDia = null;
    private MyCount count = null;
    private static int SHOWPRODIA = 0;
    private MyHandler handler;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_response_result);
        tradeTlv = TradeTlv.getInstance();
        Intent intent = getIntent();
        tip = intent.getStringExtra("tip");
        Toast.makeText(ResponseResultActivity.this, tip, Toast.LENGTH_SHORT).show();
        success = intent.getBooleanExtra("status", false);
        count = new MyCount(5000, 1000);
        handler = new MyHandler(this);
        initFragment();
        mTv_count = (TextView) findViewById(R.id.tv_counttime_transresult);
        mTv_count.setOnClickListener(this);
    }

    public void doHandle(int type, String str) {
        Message message = handler.obtainMessage();
        message.what = type;
        message.obj = str;
        handler.sendMessage(message);
    }

    private void initFragment() {
        FragmentManager supportFragmentManager = getSupportFragmentManager();
        FragmentTransaction fragmentTransaction = supportFragmentManager.beginTransaction();
        Bundle bundle = new Bundle();
        bundle.putCharSequence("tip", tip);
        if (success) {
            ResponseSuccessFragment fragment = new ResponseSuccessFragment();
            fragment.setArguments(bundle);
            fragmentTransaction.replace(R.id.fl_transresult_content, fragment);
            fragmentTransaction.commit();
            String tradeId = new String(tradeTlv.getTagValue(TradeTlv.TradeId));
            if (tradeId.equals(getString(R.string.SignInID))) {
                count.start();
            }else if (tradeId.equals(getString(R.string.DownLoadPrimaryKeyID))) {
                count.start();
            }
            else if (tradeId.equals(getString(R.string.LocateID))) {
                count.start();
            } else {
                proDia = new ProgressDialog(this);
                proDia.setCancelable(false);
                proDia.setCanceledOnTouchOutside(false);
                doHandle(SHOWPRODIA, getString(R.string.print_plswait));
                new Thread(new Runnable() {
                    @Override
                    public void run() {
                        PrintScript prtScript = new PrintScript();
                        byte[] printscript = tradeTlv.getTagValue(TradeTlv.PrintScript);
                        if (printscript != null) {
                            prtScript.startPrint(new String(printscript), printListener);
                        }

                    }
                }).start();
            }
        } else {
            ResponseFailFragment fragment = new ResponseFailFragment();
            fragment.setArguments(bundle);
            fragmentTransaction.replace(R.id.fl_transresult_content, fragment);
            fragmentTransaction.commit();
            count.start();
        }


    }

    public OnPrintListener printListener = new OnPrintListener.Stub() {
        @Override
        public void onPrintResult(int retCode) throws RemoteException {
            int DISMISSPRODIA = 1;
            int SHOWLACKPAPER = 2;
            int SHOWERR = 3;
            log.debug("打印返回:{}", retCode);
            if (retCode == PrintScript.NOT_END) {
                doHandle(SHOWPRODIA, getString(R.string.print_plstearoff));//请撕纸
            } else if (retCode == PrintScript.PRINT_END) {
                doHandle(DISMISSPRODIA, null);//关显示
                count.start();
            } else if (retCode == ServiceResult.Printer_PaperLack) {
                doHandle(SHOWLACKPAPER, null);//缺纸提示
            } else if (retCode == ServiceResult.Printer_Fault) {
                doHandle(SHOWERR, getString(R.string.print_deviceerr));//打印故障
            } else {
                doHandle(SHOWERR, getString(R.string.print_othererr));//其它错误
            }
        }

    };

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
            case R.id.tv_counttime_transresult:
                finish();
                break;
            default:
                break;
        }
    }

    @Override
    public void finish() {
        Intent intent = new Intent();
        if(success) {
            intent.putExtra("tradeCode", "0");//成功
        }else {
            intent.putExtra("tradeCode", "-1");//失败
        }
        try {
            AppGlobal.getInstance().getTransactionResult().onResult(intent);
        } catch (RemoteException e) {
            e.printStackTrace();
        }
        super.finish();
    }

    static class MyHandler extends Handler {
        WeakReference<ResponseResultActivity> mWeakReference;

        MyHandler(ResponseResultActivity activity) {
            mWeakReference = new WeakReference<ResponseResultActivity>(activity);
        }

        @Override
        public void handleMessage(Message msg) {
            final ResponseResultActivity activity = mWeakReference.get();
            switch (msg.what) {
                case 0x00: {//显示打印进度条
                    activity.proDia.setTitle(R.string.print_printing);
                    activity.proDia.setMessage(msg.obj.toString());
                    activity.proDia.onStart();
                    activity.proDia.show();
                    break;
                }
                case 0x01: {//关闭打印进度条
                    try {
                        activity.proDia.dismiss();
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
                }
                case 0x02: {//关闭打印进度条并弹出提示框
                    try {
                        activity.proDia.dismiss();
                        String str;
                        final AlertDialog.Builder dialog = new AlertDialog.Builder(activity);
                        dialog.setTitle(R.string.print_lackpapper);
                        dialog.setMessage(R.string.print_lacktip);
                        dialog.setCancelable(false);
                        dialog.setPositiveButton(R.string.print_OK, new DialogInterface.
                                OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                                activity.doHandle(SHOWPRODIA, activity.getString(R.string.print_plswait));
                                new Thread(new Runnable() {
                                    @Override
                                    public void run() {
                                        PrintScript prtScript = new PrintScript();
                                        prtScript.startPrint(new String(activity.tradeTlv.getTagValue(TradeTlv.PrintScript)), activity.printListener);
                                    }
                                }).start();
                            }
                        });
                        dialog.setNegativeButton(R.string.print_CANCEL, new DialogInterface.
                                OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                                activity.count.start();
                            }
                        });
                        dialog.show();
                        break;
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;

                }
                case 0x03:
                    try {
                        activity.proDia.dismiss();
                        final AlertDialog.Builder dialog = new AlertDialog.Builder(activity);
                        dialog.setTitle(R.string.print_err);
                        dialog.setMessage(msg.obj.toString());
                        dialog.setCancelable(false);
                        dialog.setPositiveButton(R.string.print_OK, new DialogInterface.
                                OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                dialog.dismiss();
                                activity.count.start();

                            }
                        });
                        dialog.show();
                        break;
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                    break;
            }

        }
    }

    class MyCount extends CountDownTimer {

        public MyCount(long millisInFuture, long countDownInterval) {
            super(millisInFuture, countDownInterval);
        }

        @Override
        public void onTick(long millisUntilFinished) {
            mTv_count.setText(millisUntilFinished / 1000 + getString(R.string.seconds_later_gohome));
        }

        @Override
        public void onFinish() {
            finish();
        }
    }
}
