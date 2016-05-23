package com.nexgo.smartpos.api.device.pinpad;

interface OnPinPadInputListener{
	void onInputResult(int retCode, in byte[] data);
	
	void onSendKey(byte keyCode);
}