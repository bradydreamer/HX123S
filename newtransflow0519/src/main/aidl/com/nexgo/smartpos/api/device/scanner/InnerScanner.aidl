package com.nexgo.smartpos.api.device.scanner;

import com.nexgo.smartpos.api.device.scanner.OnScannedListener;
interface InnerScanner{
	void initScanner(in Bundle bundle);
	
	int startScan(int timeout, in OnScannedListener listener);
	
	void stopScan();
}