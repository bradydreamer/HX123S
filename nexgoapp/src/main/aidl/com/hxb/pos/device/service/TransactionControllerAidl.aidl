package com.hxb.pos.device.service;

import java.util.Map;
import com.hxb.pos.device.service.TransactionResult;

interface TransactionControllerAidl {
	void common(String transactionName,in Map para,TransactionResult transactionResult);
}
