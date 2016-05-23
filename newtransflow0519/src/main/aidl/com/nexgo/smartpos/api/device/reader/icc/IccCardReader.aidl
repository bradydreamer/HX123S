package com.nexgo.smartpos.api.device.reader.icc;

import com.nexgo.smartpos.api.device.reader.icc.OnSearchIccCardListener;

interface IccCardReader{
	int searchCard(in OnSearchIccCardListener listener, int timeout, in String[] cardType);

	void stopSearch();
	
	boolean isCardExists();
	
	boolean setupReaderConfig(in Bundle bundle);
}