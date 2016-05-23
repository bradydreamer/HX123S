package com.nexgo.smartpos.api.engine;

import com.nexgo.smartpos.api.device.beeper.Beeper;
import com.nexgo.smartpos.api.device.led.LEDDriver;
import com.nexgo.smartpos.api.device.printer.Printer;
import com.nexgo.smartpos.api.device.scanner.InnerScanner;
import com.nexgo.smartpos.api.card.cpu.CPUCardHandler;
import com.nexgo.smartpos.api.card.mifare.M1CardHandler;
import com.nexgo.smartpos.api.device.reader.mag.MagCardReader;
import com.nexgo.smartpos.api.device.reader.icc.IccCardReader;
import com.nexgo.smartpos.api.emv.EmvHandler;
import com.nexgo.smartpos.api.device.pinpad.PinPad;
import com.nexgo.smartpos.api.device.serialport.SerialPortDriver;
import com.nexgo.smartpos.api.network.NetWorkHandler;

interface DeviceServiceEngine{
	int login(String bussinessId);
	
	void logout();
	
	Bundle getDevInfo();
		
	Beeper getBeeper();
	
	LEDDriver getLEDDriver();
	
	Printer getPrinter();
	
	InnerScanner getInnerScanner();

	SerialPortDriver getSerialPortDriver(int portNo);
	
	MagCardReader getMagCardReader();
	
	IccCardReader getIccCardReader(int slotNo);
	
	CPUCardHandler getCPUCardHandler(in IccCardReader reader);
	
	M1CardHandler  getM1CardHandler(in IccCardReader reader);
	
	PinPad getPinPad();
	
	EmvHandler getEmvHandler();
	
	NetWorkHandler getNetWorkHandler();
	
	//YYYYMMDDHHMMSS
	void setSystemClock(String datetime);
}