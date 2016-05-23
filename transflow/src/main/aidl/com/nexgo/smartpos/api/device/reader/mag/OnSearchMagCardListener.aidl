package com.nexgo.smartpos.api.device.reader.mag;

import com.nexgo.smartpos.api.device.reader.mag.MagCardInfoEntity;

interface OnSearchMagCardListener{
	void onSearchResult(int retCode, in MagCardInfoEntity mcie);
}