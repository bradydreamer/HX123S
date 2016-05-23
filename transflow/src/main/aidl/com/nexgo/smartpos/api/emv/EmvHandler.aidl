package com.nexgo.smartpos.api.emv;

import com.nexgo.smartpos.api.emv.EmvCardLog;
import com.nexgo.smartpos.api.emv.EmvAidPara;
import com.nexgo.smartpos.api.emv.EmvCapk;
import com.nexgo.smartpos.api.emv.OnEmvProcessListener;

interface EmvHandler{
	int emvProcess(in Bundle data, in OnEmvProcessListener listener);

	int initTermConfig(in Bundle cfg);
	
	int setAidParaList(in List<EmvAidPara> aidParaList);

	int setAidParaTlvList(in List<String> tlvList);

	int setCAPKList(in List<EmvCapk> capkList);

	int setCAPKTlvList(in List<String> tlvList);

	byte[] getTlvs(in byte[] tag, int pathId);
	
	int setTlv(in byte[] tag, in byte[] value);
	
	int getEmvCardLog(int channelType,in OnEmvProcessListener listener);
	
	int clearLog();
	
	int emvGetEcBalance(in OnEmvProcessListener listener,int channelType);
	
	void onSetSelAppResponse(int selResult);
	
	void onSetConfirmCardNoResponse(boolean isConfirm);
	
	void onSetCertVerifyResponse(boolean isVerify);
	
	void onSetOnlineProcResponse(int retCode,in Bundle data);
	
	void onSetTRiskManageResponse(String result);
}