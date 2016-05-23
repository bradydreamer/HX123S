package com.nexgo.smartpos.api.network;

interface NetWorkListener{
	void onOpenResult(int retCode);
	
	void onPingResult(int retCode);
	
	void onScanWifiResult(int retCode,in Bundle wifiSpots);

	void onCloseResult(int retCode);	
}