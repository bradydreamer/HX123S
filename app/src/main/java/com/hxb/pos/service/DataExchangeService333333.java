package com.hxb.pos.service;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.net.UnknownHostException;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

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
public class DataExchangeService333333 extends Service {
	public static String ip = "223.72.175.128";

	private DataExchangeAidl.Stub stub = new DataExchangeAidl.Stub() {

		private String getSignMsg(byte[] pos8583data) {
			byte[] tokenByte = "122b32ccf50111e39663266444c93d65".getBytes();
			byte[] sha1Bytes = new byte[pos8583data.length + tokenByte.length];
			System.arraycopy(pos8583data, 0, sha1Bytes, 0, pos8583data.length);
			System.arraycopy(tokenByte, 0, sha1Bytes, pos8583data.length,
					tokenByte.length);
			return sha1(sha1Bytes);// 对合成的byte数据计算sha1值
		}

		private String sha1(byte[] sha1Bytes) {
			try {
				MessageDigest digest = java.security.MessageDigest
						.getInstance("SHA-1");
				digest.update(sha1Bytes);
				byte messageDigest[] = digest.digest();
				// Create Hex String
				StringBuffer hexString = new StringBuffer();
				// 字节数组转换为 十六进制 数
				for (int i = 0; i < messageDigest.length; i++) {
					String shaHex = Integer
							.toHexString(messageDigest[i] & 0xFF);
					if (shaHex.length() < 2) {
						hexString.append(0);
					}
					hexString.append(shaHex);
				}

				Log.e("data", hexString.toString());
				return hexString.toString();

			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			}
			return "";
		}

		private Socket socket_connect_host = null;

		private byte[] getDataFromInputStream(InputStream inputStream)
				throws IOException {
			int firstByte = 0;
			byte b[] = null;
			if ((firstByte = inputStream.read()) != -1) {
				int cnt = 0;
				cnt = inputStream.available();
				b = new byte[cnt + 1];
				b[0] = (byte) firstByte;
				inputStream.read(b, 1, cnt);
			}
			return b;
		}

		private HttpClient httpClient = new DefaultHttpClient();

		private byte[] httpPost(String uri, byte[] iso8583) {

			HttpPost httpPost = new HttpPost(uri);

			HttpResponse httpResponse = null;

			httpPost.setEntity(new ByteArrayEntity(iso8583));

			try {
				httpResponse = httpClient.execute(httpPost);
			} catch (ClientProtocolException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			HttpEntity httpEntity = httpResponse.getEntity();

			try {

				byte[] back = getResponse(httpEntity.getContent());
				Log.e("data-size-back", back.length + "");
				return back;
			} catch (IllegalStateException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				return null;
			}
			// in.read
			// byte buffer[] = EntityUtils.toByteArray(httpEntity);
			// return buffer;
		}

		private byte[] getResponse(InputStream inStream) {
			ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
			int len = -1;
			byte[] buffer = new byte[1024];
			try {
				while ((len = inStream.read(buffer)) != -1) {
					outputStream.write(buffer, 0, len);
				}
			} catch (IOException e) {
				e.printStackTrace();
			} finally {
				if (outputStream != null) {
					try {
						outputStream.close();
					} catch (IOException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}
				}
			}
			byte rtbyte[] = outputStream.toByteArray();

			return rtbyte;
		}

//		@Override
//		public byte[] exchangeData(byte[] data) throws RemoteException {
//			// TODO Auto-generated method stub
//			Log.e("data-size-enter", data.length + "");
//			return httpPost("http://111.205.207.103/appuatweb/lais?mac="
//					+ getSignMsg(data), data);
//		}

		@Override
		public byte[] exchangeData(byte[] data) throws RemoteException {
			// TODO Auto-generated method stub

			// TODO Auto-generated method stub
			try {
				if (ip == "") {
					ip = "192.168.1.103";
				}
				// InetAddress inetAddress=InetAddress.getByName(ip);
				// SocketAddress socketAddress=new
				// InetSocketAddress(inetAddress, 6690);
				// socket_connect_host.connect(socketAddress);
				socket_connect_host = new Socket(ip, 19004);
				socket_connect_host.setSoTimeout(50 * 1000);
				Log.e("data", "socket_connect_host已经连接地址" + ip);
				OutputStream outputStream = socket_connect_host
						.getOutputStream();
				outputStream.write(data);
				outputStream.flush();
				Log.e("data", "socket_connect_host往收单平台发送数据" + data.length);

			} catch (UnknownHostException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (IOException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
				Log.e("socket", e1.toString());
			}
			try {

				byte[] dataSendToServer = getDataFromInputStream(socket_connect_host
						.getInputStream());

				Log.e("data", "SDK 从收单平台返回的数据：" + dataSendToServer.length + "");

				return dataSendToServer;
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} finally {
				try {
					if (socket_connect_host != null) {
						socket_connect_host.close();
						Log.e("data", "socket_connect_host已经关闭");

					}

				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}

			return null;

		}
	};

	/*
	 * (non-Javadoc)
	 * 
	 * @see android.app.Service#onBind(android.content.Intent)
	 */
	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return stub;
	}

}
