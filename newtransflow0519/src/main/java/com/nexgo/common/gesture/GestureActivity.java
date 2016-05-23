package com.nexgo.common.gesture;

import android.app.Activity;
import android.graphics.Bitmap;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


import com.nexgo.R;

import de.greenrobot.event.EventBus;

public class GestureActivity extends Activity {
    private HandWritingView handWriteView;
    private static final String printFileSubDirect = "XGDhardWrite";
    private static final String printFileName = "signature.bmp";
    private static String path = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.d("zbq", "GestureActivity  onCreate executed");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_gesture);
        initView();
        EventBus.getDefault().register(this);
        setCode(getIntent().getStringExtra("code"));
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        EventBus.getDefault().unregister(this);
    }

    private void initView() {
        handWriteView = (HandWritingView) findViewById(R.id.handWriting);
        handWriteView.setHandWriteCallBack(new HandWritingView.IhandWriteCallBack() {
            @Override
            public void callBack(Bitmap bitmap) {
                path = FileUtils.writeToSDCardFromBitmap(bitmap, printFileSubDirect, printFileName).getPath();
                GestureResult gestureResult = new GestureResult(Status.OK);
                EventBus.getDefault().post(gestureResult);
            }
        });
    }

    public void onEventMainThread(GestureActivity gestureActivity) {
        Log.d("zbq", "onEvent showGestureDialog executed");
    }

    /**
     * 设置图片中央的随机码
     * @param code
     */
    private void setCode(String code){
        Log.d("zbq","code = "+code);
        handWriteView.setCode(code);
    }

    /**
     * 生成的电子签名的路径
     * @return
     */
    public static String getPath(){
        return path;
    }

    public void onClick(View view) {
        int id = view.getId();
        if(id == R.id.btn_ok) {
            handWriteView.drawComplete();
            finish();
        }else if(id == R.id.btn_clear) {
            handWriteView.clear();
        }
    }
}
