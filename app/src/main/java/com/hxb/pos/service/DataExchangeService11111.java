package com.hxb.pos.service;

import java.io.ByteArrayOutputStream;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.InetSocketAddress;
import java.net.Socket;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;
import java.util.logging.Logger;

import org.apache.http.HttpEntity;
import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.entity.ByteArrayEntity;
import org.apache.http.impl.client.DefaultHttpClient;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;

/**
 * 类说明：
 */
public class DataExchangeService11111 extends Service {
//	public static String ip = "223.72.175.128";
//	private static String port = "19004";
	public static String ip = "192.168.1.192";
	private static int port = Integer.parseInt("8000");
	private static int timeOut = 50 * 1000;

	private DataExchangeAidl.Stub stub = new DataExchangeAidl.Stub() {
		@Override
		public byte[] exchangeData(byte[] data) throws RemoteException {
			Log.d("exchangeData", "jump into 华夏sdk exchangeData");
			Log.d("exchangeData", "ip=" + ip + "，port=" + port);
			Socket socket = new Socket();
			try {
				socket.setSoTimeout(timeOut);
				socket.setTcpNoDelay(true);
				socket.connect(new InetSocketAddress(ip, port), (timeOut - 1) * 1000);
				Log.d("exchangeData","socket.isConnected() = "+socket.isConnected()+", socket.isClosed() = "+ socket.isClosed());
				if (socket.isConnected() && !socket.isClosed()) {
					DataOutputStream outputStream = new DataOutputStream(socket.getOutputStream());
					DataInputStream inputStream = new DataInputStream(socket.getInputStream());

					//发送
					Log.d("exchangeData", "发给收单平台报文 = " + Arrays.toString(data));
					outputStream.write(data);
					outputStream.flush();
					//接收
					byte[] buffer = new byte[2048];
					inputStream.read(buffer);
					Log.d("exchangeData", "接收收单平台报文 = " + Arrays.toString(buffer));

					socket.shutdownOutput();
					socket.shutdownInput();
					outputStream.close();
					inputStream.close();
					socket.close();
					int len = byte2int(buffer[0], buffer[1]);
					return Arrays.copyOfRange(buffer, 0, len + 2);
				}else {
					return null;
				}
			} catch (SocketException e) {
				Log.d("exchangeData","SocketException");
				e.printStackTrace();
				return null;
			} catch (IOException e) {
				Log.d("exchangeData","IOException");
				e.printStackTrace();
				return null;
			} finally {
				socket = null;
			}
		}
	};

	private int byte2int(byte res0, byte res1) {
		return res1 & 255 | res0 << 8 & '\uff00';
	}

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return stub;
	}

}
