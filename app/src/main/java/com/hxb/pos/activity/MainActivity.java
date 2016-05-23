package com.hxb.pos.activity;

import java.util.HashMap;

import com.hxb.pos.R;
import com.hxb.pos.device.service.TransactionControllerAidl;
import com.hxb.pos.device.service.TransactionResult;

import android.app.Activity;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

public class MainActivity extends Activity {
	private String TAG = "MainActivity";
	private EditText inputAmount;
	private TransactionControllerAidl transactionControllerAidl;
	private ServiceConnection serviceConnection=new ServiceConnection() {
		@Override
		public void onServiceDisconnected(ComponentName name) {
			// TODO Auto-generated method stub
			transactionControllerAidl=null;
			Log.d(TAG,"解除绑定服务：TransactionControllerService");
		}

		@Override
		public void onServiceConnected(ComponentName name, IBinder service) {
			// TODO Auto-generated method stub
			transactionControllerAidl=TransactionControllerAidl.Stub.asInterface(service);
			Log.d(TAG,"成功绑定服务：TransactionControllerService");
		}
	};

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);

		Intent intent=new Intent();
		intent.setPackage("com.hxb.pos.device");
		intent.setAction("hxb.intent.action.transactionController");
		bindService(intent, serviceConnection, Context.BIND_AUTO_CREATE);
	}

	public void onClick(View view){
		switch (view.getId()){
			case R.id.downloadTerminalParam:
				Log.d(TAG,"终端参数下载");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("packageName", getPackageName());
					para.put("Op","99");//操作员号
//					para.put("ReveralTime","0");//冲正次数
					para.put("TerminalNo","05900003");//终端号
					para.put("MerchantNo","304391058120001");//商户号
					para.put("MerchantName","华夏智能POS测试");//商户名称
					para.put("MerchantEngName","HXB");//商户英文名称
					para.put("TPDU","6000020000");

					para.put("VoucherNo","000001");//流水号
					para.put("BatchNo","000001");//批次号
					para.put("ReferNo","123456789012");//参考号
					para.put("MoneyCode","156"); //货币代码

					para.put("PrintPaper","1");//打印联数
					para.put("IsPrivatenet","0");//公网专网选项
					para.put("TelOutsideNO","0");//交易重播次数

					para.put("MACType","1");//MAC算法标志 SDK_PED_ECB
					para.put("TakEncry","0");//磁道信息加密标志
					para.put("EncryType","1");//加密算法标志
					para.put("Tmkindex","0");//终端主秘钥索引

					para.put("Klkindex","1");//传输密钥索引

					para.put("AutoSignOut","0");//结算后自动签退
					para.put("UnsaleNeedPwd","0");//消费撤销输密
					para.put("UnsaleNeedSwipeCard","0");//消费撤销刷卡
					try {
						transactionControllerAidl.common("终端参数下载", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("终端参数下载","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
			case R.id.location:
				Log.d(TAG,"初始定位");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("packageName", getPackageName());
					para.put("Op","999");//操作员号
					para.put("jingdu","116.458194"); //经度
					para.put("weidu","39.9196");     //纬度
					try {
						transactionControllerAidl.common("初始定位", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("初始定位","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
			case R.id.downloadkey:
				Log.d(TAG,"下载传输秘钥");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("Op","99");//操作员号
					para.put("packageName", getPackageName());
					try {
						transactionControllerAidl.common("KLK下载", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("下载传输秘钥","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
			case R.id.downloadPMK:
				Log.d(TAG,"下载主秘钥");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("packageName", getPackageName());
					para.put("Op","99");//操作员号
					try {
						transactionControllerAidl.common("KEK下载", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("下载主秘钥","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
			case R.id.signIn:
				Log.d(TAG, "签到");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("packageName", getPackageName());
					para.put("Op","01");//操作员号
					try {
						transactionControllerAidl.common("签到", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("签到","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
			case R.id.consume:
				Log.d(TAG, "消费");
				if(transactionControllerAidl!=null){
					HashMap<String, String> para=new HashMap<String, String>();
					para.put("packageName", getPackageName());
					para.put("amount", "000000000001");
					para.put("Op","01");//操作员号
					try {
						transactionControllerAidl.common("消费", para, new TransactionResult.Stub() {
							@Override
							public void onResult(Intent intent) throws RemoteException {
								// TODO Auto-generated method stub
								String tradeCode = intent.getStringExtra("tradeCode");
								Log.d("消费","tradeCode = "+tradeCode);
							}
						});
					} catch (RemoteException e) {
						e.printStackTrace();
					}
				}
				break;
		}
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
		unbindService(serviceConnection);
	}
}
