package com.nexgo.smartpos.api.device.pinpad;

import com.nexgo.smartpos.api.device.pinpad.OnPinPadInputListener;

interface PinPad{
	int initPinPad(int type);
	
	int loadPlainMKey(int mKeyIdx, in byte[] keyData, int keyDataLen,boolean isTmsKey);
	
	int loadEncryptMKey(int mKeyIdx, in byte[] keyData, int keyDataLen, int decMKeyIdx, boolean isTmsKey);
	
	int loadKeyByCom(int portNo, int keyType);
	
	int loadWKey(int mKeyIdx, int keyType, in byte[] keyData, int keyDataLen);
	
	int loadPlainDesKey(int keyIdx, in byte[] keyData, int keyLen);
	
	byte[] calcWKeyKCV(int mKeyIdx, int wKeyType);

	int desEncByWKeyEx(int mKeyIdx, int wKeyType, in byte[] data, int dataLen, int desType,out byte[] desResult);

	int desEncByWKey(int mKeyIdx, int wKeyType, in byte[] data, int dataLen, out byte[] desResult);

	byte[] getMac(int mKeyIdx, int mode,int type, in byte[] data);
	
	byte[] desByPlainKey(int desKeyId, in byte[] data, int dataLen, int desType, int desMode);
	
	byte[] desByTmsKey(int tmsMKeyId, in byte[] data, int dataLen, int desType, int desMode);
	
	boolean deleteMKey(int mKeyIdx);
	
	boolean format();
	
	byte[] getRandom();
	
	int inputText(in OnPinPadInputListener listener, int mode);
	
	int inputOnlinePin(in byte[] panBlock, int mKeyId, int pinAlgMode, in OnPinPadInputListener listener);

	boolean isInputting();
	
	boolean cancelInput();
	
	void setTimeOut(int timeout);
	
	void setSupportPinLen(in int[] pinLen);
}