package com.nexgo.smartpos.api.device.scanner;

interface OnScannedListener{
	void onScanResult(int retCode, in byte[] data);
}