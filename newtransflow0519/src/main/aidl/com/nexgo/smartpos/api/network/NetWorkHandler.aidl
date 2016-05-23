package com.nexgo.smartpos.api.network;

import com.nexgo.smartpos.api.network.NetWorkListener;

interface NetWorkHandler{
	int setNetworkConfig(in Bundle bundle);
	
	boolean open(int networkType,in NetWorkListener listener);
	
	boolean close(int networkType,in NetWorkListener listener);

	void ping(String serverIp, int timeout, in NetWorkListener listener);
	
	void scanWifi(in NetWorkListener listener);
}