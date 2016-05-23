package com.hxb.pos.device.activity;

import android.app.Activity;
import android.os.Bundle;
import android.os.CountDownTimer;
import android.view.View;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.TextView;

import com.hxb.pos.device.R;
import com.hxb.pos.device.utils.StatusCode;

/**
 * 作者:liuting on 2016/3/18 10:02
 * 邮箱:liuting@xinguodu.com
 * 项目名：ump
 * 包名：com.xgd.umsapp.activity
 * TODO:
 */
public abstract class BaseFunctionActivity extends Activity {

    protected TextView mTv_count;
    private MyCount mCount;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //屏幕常亮
        getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
                WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        setContentView(R.layout.activity_base_function);
        ImageView imageView = (ImageView) findViewById(R.id.iv_back_baseactivity);
        TextView title = (TextView) findViewById(R.id.tv_title);
        mTv_count = (TextView) findViewById(R.id.tv_countime_baseactivity);
        title.setText(setActivityTitle());//设置标题
        mCount = new MyCount(60000, 1000);
        imageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                doBeforeFinish();
                finish();
            }
        });
        FrameLayout content = (FrameLayout) findViewById(R.id.content_baseactivity);
        content.addView(initView());

    }

    /**
     * 重新计时
     */
    public void reSetCount() {
        mCount.cancel();
        mCount.start();
    }


    @Override
    protected void onResume() {
        super.onResume();
        mCount.start();
    }

    protected void cancelCount() {
        mCount.cancel();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mCount.cancel();
    }

    public void doBeforeFinish() {
    }

    ;

    protected abstract View initView();

    protected abstract String setActivityTitle();

    class MyCount extends CountDownTimer {

        public MyCount(long millisInFuture, long countDownInterval) {
            super(millisInFuture, countDownInterval);
        }

        @Override
        public void onTick(long millisUntilFinished) {
            mTv_count.setText(millisUntilFinished / 1000 + "");
        }

        @Override
        public void onFinish() {
            setResult(StatusCode.TIMEOUT);
            finish();
        }
    }
}
