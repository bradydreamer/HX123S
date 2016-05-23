package com.nexgo.smartpos.api.device.reader.icc;

interface OnSearchIccCardListener{
	void onSearchResult(int retCode,in Bundle bundle);
}