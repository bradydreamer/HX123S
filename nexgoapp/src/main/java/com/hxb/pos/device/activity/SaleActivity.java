package com.hxb.pos.device.activity;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.WindowManager;

import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.StatusCode;
import com.hxb.pos.device.utils.ToastUtil;
import com.nexgo.common.TradeTlv;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

public class SaleActivity extends Activity {

    private TradeTlv tradeTlv = null;
    private Logger log;
    //FIXME 加签到 标识
    private boolean Ifsign = false;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_sale);
        init();
        if (Ifsign) {
            tradeTlv.setTagValue(TradeTlv.TradeId, getString(R.string.SaleID).getBytes());//设置交易ID
            Intent i = new Intent(SaleActivity.this, ReadCardActivity.class);
            startActivityForResult(i, 1);
        } else {
            ToastUtil.showToast(SaleActivity.this, getString(R.string.sale_plssignin));
            Intent i = new Intent(SaleActivity.this, SigninActivity.class);
            startActivityForResult(i, 3);
        }
    }

    private void init() {
        log = LoggerFactory.getLogger(this.getClass().getName());
        tradeTlv = TradeTlv.getInstance();
        tradeTlv.cleanTlv();
        TradeTlv.getInstance().setTagValue(TradeTlv.Amount, getIntent().getStringExtra("Amount").getBytes());
        Log.d("SaleActivity_init", "TradeTlv.Amount = " + TradeTlv.getInstance().getTagValue(TradeTlv.Amount).toString());
        tradeTlv.setTagValue(TradeTlv.TradeId, getString(R.string.SaleID).getBytes());//设置交易ID
        byte[] singstate = tradeTlv.getTagValue(TradeTlv.SignState);
        if (singstate != null) {
            if (singstate[0] == 0x31) {
                Ifsign = true;
            }
        }
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 1) {//读卡界面返回
            if (resultCode == StatusCode.CANCEL) {//
                finish();
            } else if (resultCode == StatusCode.TIMEOUT) {
                ToastUtil.showToast(this,getString(R.string.operate_timeout));
                finish();
            } else {
                Intent i = new Intent(SaleActivity.this, ResponseResultActivity.class);
                if (resultCode == StatusCode.FAIL) {
                    i.putExtra("status", false);
                    if(data!=null){
                        String errtip=data.getExtras().getString("errtip");
                        i.putExtra("tip", errtip);
                    }else{
                        i.putExtra("tip", getString(R.string.trade_failed));
                    }

                } else {
                    i.putExtra("status", true);
                    i.putExtra("tip", getString(R.string.trade_succeed));
                }
//                if (resultCode == StatusCode.FAIL) {
//                    i.putExtra("status", false);
//                    i.putExtra("tip", getString(R.string.sale_failed));
//                } else {
//                    i.putExtra("status", true);
//                    i.putExtra("tip", getString(R.string.sale_succeed));
//                }
                startActivityForResult(i, 2);
            }
        } else if (requestCode == 3) {//从签到界面返回
            if (resultCode == 0) {
                tradeTlv.setTagValue(TradeTlv.TradeId, getString(R.string.SaleID).getBytes());//设置交易ID
                Intent i = new Intent(SaleActivity.this, ReadCardActivity.class);
                startActivityForResult(i, 1);
            } else if (resultCode == StatusCode.TIMEOUT) {
                ToastUtil.showToast(this,getString(R.string.operate_timeout));
                finish();
            } else {
                finish();
            }
        } else {
            finish();
        }

    }
}
