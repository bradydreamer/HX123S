package com.nexgo.common.gesture;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.os.Handler;
import android.os.Message;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;

/**
 * guofanghua
 */
public class HandWritingView extends View{
    String TAG = "HandWritingView";
    private Paint mPaint;
    private int paintStrokeWidth = 5;

    private Paint paint;
    private float textsize = 50;

    /*传进来的随机数*/
    private String testString;
    private int charnums;
    private float left,top;

    private static final float MINP = 0.25f;
    private static final float MAXP = 0.75f;

    private Bitmap mBitmap;
    private Bitmap newbmp;
    private Canvas mCanvas;
    private Path mPath;
    private Paint mBitmapPaint;

    private long mStartTime;
    private long mEndTime;
    private boolean isStart = false;
    private Handler mHandler;
    private Context mContext;
    public IhandWriteCallBack handWriteCallBack;

    public IhandWriteCallBack getHandWriteCallBack() {
        return handWriteCallBack;
    }

    public void setHandWriteCallBack(IhandWriteCallBack handWriteCallBack) {
        this.handWriteCallBack = handWriteCallBack;
    }

    /**
     * 设置图片中央的随机码
     * @param code
     */
    public void setCode(String code){
        this.testString = code;
    }

    public HandWritingView(Context context,AttributeSet attrs) {
        super(context,attrs);
        mContext = context;
        mPath = new Path();
        mPaint = new Paint();
        mPaint.setAntiAlias(true); //图像边缘相对清晰一点,锯齿痕迹不那么明显
        mPaint.setDither(true);    //防抖
        mPaint.setColor(Color.BLACK);//画笔颜色
        mPaint.setStyle(Paint.Style.STROKE);   //设置画笔空心
        mPaint.setStrokeJoin(Paint.Join.ROUND);//转折处圆角
        mPaint.setStrokeCap(Paint.Cap.ROUND);  //画笔笔刷类型
        mPaint.setStrokeWidth(paintStrokeWidth);//设置画笔宽度
        mBitmapPaint = new Paint(Paint.DITHER_FLAG);

        paint = new Paint(Paint.ANTI_ALIAS_FLAG);
        paint.setStrokeWidth(3);
        paint.setTextSize(textsize);
        paint.setColor(Color.RED);

        mHandler = new Handler() {
            @Override
            public void handleMessage(Message msg) {
                // TODO Auto-generated method stub
                Log.d("shunxu","handleMessage");
                super.handleMessage(msg);
                mBitmap = Bitmap.createBitmap(mBitmap.getWidth(),
                        mBitmap.getHeight(), Bitmap.Config.ARGB_8888);//开始绘图
                mCanvas.setBitmap(mBitmap);//显示图片
                mCanvas.drawColor(Color.WHITE);//画布颜色

                invalidate();
                if (handWriteCallBack != null && newbmp != null) {
                    handWriteCallBack.callBack(newbmp);//写入SD卡
                }

            }
        };
    }

    public HandWritingView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        // TODO Auto-generated constructor stub
    }

    public void setPaint(Paint paint) {
        this.mPaint = paint;
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        Log.d("shunxu", "onSizeChanged");
        charnums = testString.length();
        left = w/2 - charnums*textsize/4;//特征码水平方向位置
        top = h/2 + textsize/2;//特征码竖直方向位置

        super.onSizeChanged(w, h, oldw, oldh);
        mBitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        mCanvas = new Canvas(mBitmap);
        mCanvas.drawColor(Color.WHITE);

        /*提前在画布上显示特征码*/
        Log.d("zbq", "testString = "+testString);
        mCanvas.drawText(testString,left, top, paint);
    }

    @Override
    protected void onDraw(Canvas canvas) {
        Log.d("shunxu","onDraw");
        canvas.drawColor(Color.WHITE);//填充画布颜色
        canvas.drawBitmap(mBitmap, 0, 0, mBitmapPaint);
        canvas.drawPath(mPath, mPaint);
    }

    private float mX, mY;
    private static final float TOUCH_TOLERANCE = 4;

    private void touch_start(float x, float y) {
        Log.d("shunxu","touch_start");
        mPath.reset();
        mPath.moveTo(x, y);
        mX = x;
        mY = y;
    }

    private void touch_move(float x, float y) {
        Log.d("shunxu","touch_move");
        float dx = Math.abs(x - mX);
        float dy = Math.abs(y - mY);
        if (dx >= TOUCH_TOLERANCE || dy >= TOUCH_TOLERANCE) {
            mPath.quadTo(mX, mY, (x + mX) / 2, (y + mY) / 2);
            mX = x;
            mY = y;
        }
    }

    private void touch_up() {
        Log.d("shunxu","touch_up");
        mPath.lineTo(mX, mY);
        mCanvas.drawPath(mPath, mPaint);
        mPath.reset();
    }

    public void clear(){
        Log.d("shunxu", "clear");
        mCanvas.drawColor(Color.WHITE);

        /*提前在画布上显示特征码*/
        mCanvas.drawText(testString,left, top, paint);

        invalidate();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        Log.d("shunxu","onTouchEvent");
        float x = event.getX();
        float y = event.getY();

        switch (event.getAction()) {
            case MotionEvent.ACTION_DOWN:
                mEndTime = System.currentTimeMillis();
                touch_start(x, y);
                invalidate();
                break;
            case MotionEvent.ACTION_MOVE:
                mEndTime = System.currentTimeMillis();
                touch_move(x, y);
                invalidate();
                break;
            case MotionEvent.ACTION_UP:
                mEndTime = System.currentTimeMillis();
                touch_up();
                invalidate();
                break;
        }
        return true;
    }

    public void drawComplete() {
        Log.d("shunxu","drawComplete");
        mEndTime = System.currentTimeMillis();
        invalidate();
        if (!isStart) {
            isStart = true;
            new Thread() {
                public void run() {
                    while (isStart) {
                        long time = System.currentTimeMillis();
                        if (time >= mEndTime + 800) {//0.8秒跳出循环
                            int width = mBitmap.getWidth();
                            int height = mBitmap.getHeight();
                            Log.d(TAG,"width = "+width);//440
                            Log.d(TAG,"height = "+height);//574
                            Matrix matrix = new Matrix();
                            int defaultWidth = width > (384 / 2) ? (384 / 2) : width;
                            int defaultHeight = height > (240 / 2) ? (240 / 2) : height;
                            float scaleWidth = ((float) defaultWidth / width);
                            float scaleHeight = ((float) defaultHeight / height);

                            matrix.postScale(scaleWidth, scaleHeight);//缩放
                            newbmp = Bitmap.createBitmap(mBitmap, 0, 0,//保存缩放后的图片
                                    width, height, matrix, true);
                            mHandler.sendEmptyMessage(0);
                            isStart = false;
                        }
                    }
                }
            }.start();
        }
    }

    public interface IhandWriteCallBack {
        void callBack(Bitmap bitmap);
    }

}
