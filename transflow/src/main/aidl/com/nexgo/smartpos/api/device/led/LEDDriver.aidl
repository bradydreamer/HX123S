package com.nexgo.smartpos.api.device.led;

interface LEDDriver{
	void setLed(int light, boolean isOn);
}