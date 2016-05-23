package com.nexgo.smartpos.api.card.cpu;

import com.nexgo.smartpos.api.card.cpu.APDUCmd;

interface CPUCardHandler{
	boolean setPowerOn(out byte[] atr);
	
	void setPowerOff();
	
	int exchangeAPDUCmd(inout APDUCmd cmd);
	
	boolean active();
}