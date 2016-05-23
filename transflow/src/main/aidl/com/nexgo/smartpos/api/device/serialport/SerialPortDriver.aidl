package com.nexgo.smartpos.api.device.serialport;

interface SerialPortDriver{
	int connect(String cfg);
	
	int send(in byte[] data, int dataLen);
	
	int recv(out byte[] buffer, int recvLen, long timeout);
	
	int disconnect();
	
	void clrBuffer();
}