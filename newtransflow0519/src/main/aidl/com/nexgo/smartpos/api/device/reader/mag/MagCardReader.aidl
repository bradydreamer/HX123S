package com.nexgo.smartpos.api.device.reader.mag;

import  com.nexgo.smartpos.api.device.reader.mag.OnSearchMagCardListener;

interface MagCardReader{
	int searchCard(in OnSearchMagCardListener listener, int timeout);
	
	int searchCardEx(in OnSearchMagCardListener listener, int timeout,in Bundle data);
		
	void stopSearch();
	
	void setIsCheckLrc(boolean isCheckLrc);
}