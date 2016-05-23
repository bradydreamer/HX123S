#include "provider.h"

#include "appglobal.h"
#include <string.h>
#include <android/log.h>

extern JNIEnv* jniEnv;
#define SET_TAG_VALUE 			1
#define GET_TAG_VALUE 			2
#define GET_TAG_VALUE_LENGTH 	3
#define UPDATE_RECORD           4
#define DEL_ALL_RECORDE         5
#define DEL_ONE_RECORD			6
#define UPDATE_RECORD_ONE_TAG	7
#define SET_RECORD_TO_TAG_LIST	8
#define GET_RECORD_TOTAL		9



#define CALC_MAC 				1
#define UPDATE_WK 				2
#define SEC_AUTH 				3
#define E_SIGN           		4
#define PRINT_SCRIPT         	5
#define EMV_GET_TAG_VALUE		6
#define EMV_SET_TAG_VALUE		7
#define TRACK_DATA_ENCRYPT      8
#define DES_BY_TMSKEY      9
#define UPDATE_PMK         10
#define DES_BY_WKEY        11
#define SYS_TIME        12





jclass TradeTlv = NULL;
jclass DeviceApusic = NULL;
jobject mDeviceApusic= NULL;

//java层表内数据操作方法
jmethodID setTagValue = NULL;
jmethodID getTagValue = NULL;
jmethodID getTagValueLength = NULL;
jmethodID updateTagValueToDb = NULL;
jmethodID deleteAllRecord = NULL;
jmethodID deleteOneRecord = NULL;
jmethodID updateRecordOneTag = NULL;
jmethodID setRecordToTagList = NULL;
jmethodID getRecordTotal = NULL;





//设备操作方法
jmethodID calcMacFromJava = NULL;
jmethodID updateWk = NULL;
jmethodID updatePmk = NULL;
jmethodID desByTmsKey = NULL;
jmethodID desEncByWKey = NULL;
jmethodID secAuth = NULL;
jmethodID eSign = NULL;
jmethodID printScript = NULL;
jmethodID emvGetTagValue = NULL;
jmethodID emvSetTagValue = NULL;
jmethodID trackDataEncrypt = NULL;
jmethodID getSysTimeFromJava = NULL;

void freeTagValueObject();
void freeDeviceObject();
int GetProviderInstance(jclass obj_class);

/**
 * 初始化 类、对象、方法
 */
int InitTagValueProvider(int No) {

    //Log("cCallJava","InitProvider Begin  1 No = %d\n", No);
	if(jniEnv == NULL) {
		return 0;
	}

	if(TradeTlv == NULL) {
        //Log("cCallJava","InitProvider Begin 2 111111111111111111111111111");
		TradeTlv = (*jniEnv)->FindClass(jniEnv,"com/nexgo/common/TradeTlv");

        //Log("cCallJava","InitProvider Begin 2 2222222222222222222222222");
		if(TradeTlv == NULL){
			return -1;
		}

        //Log("cCallJava","InitProvider Begin 2 TradeTlv ok");
	}

	if (setTagValue == NULL && No == SET_TAG_VALUE) {

        //Log("cCallJava","InitProvider Begin 3 111111111111111111111111111");
        setTagValue = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "setTagValueForC","(Ljava/lang/String;[BI)V");
		if (setTagValue == NULL) {
			(*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
			return -2;
		}
        //Log("cCallJava","InitProvider Begin  4 setTagValue ok");
	}

	if (getTagValue == NULL && No == GET_TAG_VALUE) {
        getTagValue = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "getTagValueForC","(Ljava/lang/String;)[B");
		if (getTagValue == NULL) {
			(*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
			return -2;
		}
        //Log("cCallJava","InitProvider Begin  5 getTagValve ok");
	}

	if (getTagValueLength == NULL && No == GET_TAG_VALUE_LENGTH) {
        getTagValueLength = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "getTagValueLengthForC","(Ljava/lang/String;)I");
		if (getTagValueLength == NULL) {
			(*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
			return -2;
		}
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
	}

    if (updateTagValueToDb == NULL && No == UPDATE_RECORD) {
        updateTagValueToDb = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "updateTradetoDb","()Z");
        if (updateTagValueToDb == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
            return -2;
        }
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
    }

    if (deleteAllRecord == NULL && No == DEL_ALL_RECORDE) {
        deleteAllRecord = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "deleteAllRecordForC","()V");
        if (deleteAllRecord == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
            return -2;
        }
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
    }

    if (deleteOneRecord == NULL && No == DEL_ONE_RECORD) {
        deleteOneRecord = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "deleteRecordForC","(Ljava/lang/String;)Z");
        if (deleteOneRecord == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
            return -2;
        }
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
    }

	if (updateRecordOneTag == NULL && No == UPDATE_RECORD_ONE_TAG) {
        updateRecordOneTag = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "setTagValueinRecord","(Ljava/lang/String;Ljava/lang/String;[B)Z");
        if (updateRecordOneTag == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
            return -2;
        }
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
    }

	if (setRecordToTagList == NULL && No == SET_RECORD_TO_TAG_LIST) {
        setRecordToTagList = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "getOneTradeRecordByIndex","(I)Z");
        if (setRecordToTagList == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
            return -2;
        }
        //Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
    }

    if (getRecordTotal == NULL && No == GET_RECORD_TOTAL) {
            getRecordTotal = (*jniEnv)->GetStaticMethodID(jniEnv, TradeTlv, "getrecordNums","()I");
            if (getRecordTotal == NULL) {
                (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
                return -2;
            }
            Log("cCallJava","InitProvider Begin  6 getTagValueLength ok");
        }

    //Log("cCallJava","InitProvider Begin  7 getTagValueLength ok");
	return 1;

}

int InitDeviceProvider(int No) {

    Log("cCallJava","InitDeviceProvider Begin  1");
    if(jniEnv == NULL) {
        return 0;
    }

    if(DeviceApusic == NULL) {
        DeviceApusic = (*jniEnv)->FindClass(jniEnv,"com/nexgo/jniinterface/DeviceApusic");
        if(DeviceApusic == NULL){
            return -1;
        }
        Log("cCallJava","InitDeviceProvider Begin 2 DeviceApusic ok");
    }

    if (mDeviceApusic == NULL) {
        if (GetProviderInstance(DeviceApusic) != 1) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            return -1;
        }
        Log("cCallJava","InitDeviceProvider Begin  3 ok");
    }

    if (getSysTimeFromJava == NULL && No == SYS_TIME) {
    	getSysTimeFromJava = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "getSysTimeForC","()[B");
		if (getSysTimeFromJava == NULL) {
			(*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
			(*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
			return -2;
		}
		Log("cCallJava","InitDeviceProvider Begin  6111111111 getSysTimeFromJava ok");
	}

    if (calcMacFromJava == NULL && No == CALC_MAC) {
        calcMacFromJava = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "CalculateMacForC","([B)[B");
        if (calcMacFromJava == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitDeviceProvider Begin  6111111111 CalculateMacForC ok");
    }

    if (desByTmsKey == NULL && No == DES_BY_TMSKEY) {
        desByTmsKey = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "desByTmsKeyForC","([BI)[B");
        if (desByTmsKey == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitDeviceProvider Begin  622222222222222 desByTmsKeyForC ok");
    }

    if (desEncByWKey == NULL && No == DES_BY_WKEY) {
    	desEncByWKey = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "desEncByWKeyForC","([BI)[B");
		if (desEncByWKey == NULL) {
			(*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
			(*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
			return -2;
		}
		Log("cCallJava","InitDeviceProvider Begin  622222222222222 desEncByWKeyForC ok");
    }

    if (updateWk == NULL && No == UPDATE_WK) {
        updateWk = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "DownLoadWKForC","([B)Z");
        if (updateWk == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitDeviceProvider Begin  622222222222222 DownLoadWKForC ok");
    }

    if (updatePmk == NULL && No == UPDATE_PMK) {
        updatePmk = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "LoadMKForC","([B)V");
        if (updatePmk == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitDeviceProvider Begin  622222222222222 LoadMKForC ok");
    }

    if (secAuth == NULL && No == SEC_AUTH) {
        secAuth = (*jniEnv)->GetStaticMethodID(jniEnv, DeviceApusic, "SecondAuthorizeForC","(Ljava/lang/String;Ljava/lang/String;[B[B[B)I");
        if (secAuth == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitDeviceProvider Begin  633333333333333333 secAuth ok");
    }

//    if (eSign == NULL && No == E_SIGN) {
//        eSign = (*jniEnv)->GetStaticMethodID(jniEnv, DeviceApusic, "EcleSignForC","(Ljava/lang/String;)Ljava/lang/String;");
//        if (eSign == NULL) {
//            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
//            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
//            return -2;
//        }
//        //Log("cCallJava","InitProvider Begin  6444444444444444444 getTagValueLength ok");
//    }
//
    if (printScript == NULL && No == PRINT_SCRIPT) {
        printScript = (*jniEnv)->GetStaticMethodID(jniEnv, DeviceApusic, "setPrintScript","(Ljava/lang/String;)Z");
        if (printScript == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitProvider Begin  655555555555555555555 setPrintScript ok");
    }

    if (emvGetTagValue == NULL && No == EMV_GET_TAG_VALUE) {
        emvGetTagValue = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "EmvgetTlv","([BI)[B");
        if (emvGetTagValue == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
        Log("cCallJava","InitProvider Begin  655555555555555555555 EmvgetTlv ok");
    }

    if (emvSetTagValue == NULL && No == EMV_SET_TAG_VALUE) {
        emvSetTagValue = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "EmvsetTlv", "([B[B)I");
        if (emvSetTagValue == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
    }

    if (trackDataEncrypt == NULL && No == TRACK_DATA_ENCRYPT) {
        trackDataEncrypt = (*jniEnv)->GetMethodID(jniEnv, DeviceApusic, "TrackDataEncryptForC", "([B)[B");
        if (trackDataEncrypt == NULL) {
            (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
            (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
            return -2;
        }
    }
    //Log("cCallJava","InitDeviceProvider Begin  7 getTagValueLength ok");
    return 1;

}

int GetProviderInstance(jclass obj_class) {

	if(obj_class == NULL) {
		return 0;
	}

	jmethodID construction_id = (*jniEnv)->GetMethodID(jniEnv, obj_class, "<init>", "()V");
	if (construction_id == 0) {
		return -1;
	}

    mDeviceApusic = (*jniEnv)->NewObject(jniEnv, obj_class, construction_id);
	if ( mDeviceApusic == NULL) {
		return -2;
	}

	return 1;
}

/**
 *
 * 获取java层tag列表的值的长度
 *
 * @param tag 待获取值的TAG
 *
 * @return 成功，返回值的长度；-4 参数错误，-1其他错误
 */
int ProviderGetTagValueLength(char* tag)
{
    int result = 1;
    jstring jTag = NULL;

    ////Log("cCallJava","Enter ProviderGetTagValueLength");
    if(TradeTlv == NULL || getTagValueLength == NULL) {
        result = InitTagValueProvider(GET_TAG_VALUE_LENGTH);
    }

    if(tag == NULL){
        result = -4;
    }

    ////Log("cCallJava","ProviderGetTagValueLength 11111111111111111");
    if (result == 1) {
        jTag =(*jniEnv)->NewStringUTF(jniEnv, tag);
        result = (*jniEnv)->CallStaticIntMethod(jniEnv, TradeTlv, getTagValueLength, jTag);
        (*jniEnv)->DeleteLocalRef(jniEnv, jTag);
        //Log("cCallJava","ProviderGetTagValueLength 22222222 len = %d",result);
    }

    freeTagValueObject();
    return result;
}

/**
 *
 * 获取java层tag列表的值
 *
 * @param tag 待获取值的TAG
 * @param value 获取到的值
 *
 * @return 成功，返回值的长度；-4 参数错误，-1其他错误
 */
int ProviderGetTagValue(char* tag, unsigned char* value) {

    int result = 1;
    jsize valueLen = 0;
    jstring jTag = NULL;
    jbyteArray jValue = NULL;
    //char str[512] = {0};

    //Log("cCallJava","Enter ProviderGetTagValue");
    if(TradeTlv == NULL || getTagValue == NULL) {
        result = InitTagValueProvider(GET_TAG_VALUE);
    }

    if(tag == NULL || value == NULL){
        result = -4;
    }

    //Trace("cCallJava","ProviderGetTagValue 111111111111111 tag = %s", tag);
    if (result == 1) {
        //Log("cCallJava","ProviderGetTagValue 222222222222222");
        result = 0;
        jTag = (*jniEnv)->NewStringUTF(jniEnv, tag);
        jValue = (*jniEnv)->CallStaticObjectMethod(jniEnv, TradeTlv, getTagValue, jTag);

        if(jValue != NULL){
            valueLen = (*jniEnv)->GetArrayLength(jniEnv,jValue);
            result = valueLen;

            //Log("cCallJava","ProviderGetTagValue 33333333333333333 result =%d", result);
            (*jniEnv)->GetByteArrayRegion(jniEnv, jValue, 0, result, value);
            //sdkBcdToAsc(str, value, result);
            //Log("cCallJava","ProviderGetTagValue 33333333333333333 str =%s", str);
        }

        (*jniEnv)->DeleteLocalRef(jniEnv, jTag);
        //Log("cCallJava","ProviderGetTagValue 33333333333333333");
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderGetTagValue 444444444444444444444444");
    return result;
}

/**
 *
 * 设置java层tag列表的值
 *
 * @param tag 待设置的TAG
 * @param value 设置的值
 * @param len 设置的值的长度
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderSetTagValue(char* tag, unsigned char* value, int len) {

    int result = 1;
    jstring jTag = NULL;
    jbyteArray jValue = NULL;
    Trace("cCallJava","Enter ProviderSetTagValue");

    if(TradeTlv == NULL || setTagValue == NULL) {
        result = InitTagValueProvider(SET_TAG_VALUE);
    }

    //Trace("cCallJava","ProviderSetTagValue 111111111111111");
    if(tag == NULL || value == NULL || len == 0){
        result = -4;
    }

    //Trace("cCallJava","ProviderSetTagValue 22222222222222");
    if (result == 1) {
        //Trace("cCallJava","ProviderSetTagValue 3333333333333 len = %d", len);
        jTag = (*jniEnv)->NewStringUTF(jniEnv, tag);
        //Log("cCallJava","ProviderSetTagValue 4444444444444 len = %d", len);
        jValue = (*jniEnv)->NewByteArray(jniEnv, len);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jValue, 0, len, (jbyte *)value);
        if(jTag == NULL || jValue == NULL) {
            //Trace("cCallJava","ProviderSetTagValue jTag == NULL || jValue == NULL");
            return -1;
        }
        (*jniEnv)->CallStaticVoidMethod(jniEnv, TradeTlv, setTagValue, jTag, jValue, len);
        (*jniEnv)->DeleteLocalRef(jniEnv, jTag);
        (*jniEnv)->DeleteLocalRef(jniEnv, jValue);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderSetTagValue 44444444444444");
    return 1;
}

/**
 *
 * 设置java层tag列表的值到数据库
 *
 * @return 1成功，-1其他错误
 */
int ProviderUpdateValueToDb() {

    int result = 1;
    jboolean bResult = false;

    Trace("cCallJava","Enter ProviderUpdateValueToDb");

    if(TradeTlv == NULL || updateTagValueToDb == NULL) {
        result = InitTagValueProvider(UPDATE_RECORD);
    }

    //Trace("cCallJava","ProviderUpdateValueToDb 22222222222222");
    if (result == 1) {
        bResult = (*jniEnv)->CallStaticBooleanMethod(jniEnv, TradeTlv, updateTagValueToDb);
        result = (bResult ? 1: -1);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderUpdateValueToDb 44444444444444");
    return result;
}

/**
 *
 * 删除整张交易明细表
 *
 * @return 1成功，-1其他错误
 */
int ProviderDeleteTradeRecord() {

    int result = 1;
    jboolean bResult = false;

    Trace("cCallJava","Enter ProviderUpdateValueToDb");

    if(TradeTlv == NULL || deleteAllRecord == NULL) {
        result = InitTagValueProvider(DEL_ALL_RECORDE);
    }

    //Trace("cCallJava","ProviderUpdateValueToDb 22222222222222");
    if (result == 1) {
        (*jniEnv)->CallStaticVoidMethod(jniEnv, TradeTlv, deleteAllRecord);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderUpdateValueToDb 44444444444444");
    return result;
}

/**
 *
 * 删除一条明细记录
 *
 * @param voucher 流水号（例如“000001”）
 *
 * @return 1成功，-1其他错误
 */
int ProviderDeleteTradeOneRecord(char *voucher) {

    int result = 1;
    jstring jVoucher = NULL;
    jboolean bResult;

    Trace("cCallJava","Enter ProviderUpdateValueToDb");

    if(TradeTlv == NULL || deleteOneRecord == NULL) {
        result = InitTagValueProvider(DEL_ONE_RECORD);
    }

    //Trace("cCallJava","ProviderUpdateValueToDb 22222222222222");
    if (result == 1) {
        jVoucher = (*jniEnv)->NewStringUTF(jniEnv, voucher);
        bResult = (*jniEnv)->CallStaticBooleanMethod(jniEnv, TradeTlv, deleteOneRecord, jVoucher);
        (*jniEnv)->DeleteLocalRef(jniEnv, jVoucher);
         result = (bResult? 1: -1);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderUpdateValueToDb 44444444444444");
    return result;
}

int ProviderUpdateRecordOneTag(char *voucher, char* tag, unsigned char* value, int len) {

    int result = 1;
    jstring jVoucher = NULL;
	jstring jTag = NULL;
	jbyteArray jValue = NULL;
    jboolean bResult;

//    Trace("cCallJava","Enter ProviderUpdateValueToDb");

    if(TradeTlv == NULL || deleteOneRecord == NULL) {
        result = InitTagValueProvider(UPDATE_RECORD_ONE_TAG);
    }

    //Trace("cCallJava","ProviderUpdateValueToDb 22222222222222");
    if (result == 1) {
        jVoucher = (*jniEnv)->NewStringUTF(jniEnv, voucher);
		jTag = (*jniEnv)->NewStringUTF(jniEnv, tag);
		jValue = (*jniEnv)->NewByteArray(jniEnv, len);
		(*jniEnv)->SetByteArrayRegion(jniEnv, jValue, 0, len, (jbyte *)value);
		
        bResult = (*jniEnv)->CallStaticBooleanMethod(jniEnv, TradeTlv, updateRecordOneTag, jVoucher, jTag, jValue);
        result = (bResult? 1: -1);

		(*jniEnv)->DeleteLocalRef(jniEnv, jTag);
        (*jniEnv)->DeleteLocalRef(jniEnv, jValue);
		(*jniEnv)->DeleteLocalRef(jniEnv, jVoucher);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderUpdateValueToDb 44444444444444");
    return result;
}

/**
 *
 * 读数据库记录到Tag 列表
 *
 * @param index 索引
 *
 * 备注： 从0开始到第N-1条，N可以通过 ProviderGetRecordTotal获取
 * @return 返回1成功，-4 参数错误，-1其他错误
 */
int ProviderSetRecordToTagList(int index) {

    int result = 1;
    jboolean bResult;

//    Trace("cCallJava","Enter ProviderUpdateValueToDb");

    if(TradeTlv == NULL || setRecordToTagList == NULL) {
        result = InitTagValueProvider(SET_RECORD_TO_TAG_LIST);
    }

    //Trace("cCallJava","ProviderUpdateValueToDb 22222222222222");
    if (result == 1) {
		
        bResult = (*jniEnv)->CallStaticBooleanMethod(jniEnv, TradeTlv, setRecordToTagList, index);
        result = (bResult? 1: -1);
    }

    freeTagValueObject();
    //Log("cCallJava","ProviderUpdateValueToDb 44444444444444");
    return result;
}

/**
 *
 * 获取数据库交易记录总数
 *
 * 备注：用于批上送
 * @return 成功返回总数，-4 参数错误，-1其他错误
 */
int ProviderGetRecordTotal() 
{

    int result = 1;

    Log("cCallJava","Enter ProviderGetRecordTotal");

    if(TradeTlv == NULL || getRecordTotal == NULL) {
        result = InitTagValueProvider(GET_RECORD_TOTAL);
    }

    Log("cCallJava","ProviderGetRecordTotal 22222222222222");
    if (result == 1) {
        result = (*jniEnv)->CallStaticIntMethod(jniEnv, TradeTlv, getRecordTotal);
    }

    Log("cCallJava","ProviderGetRecordTotal 44444444444444  total = %d", result);
    freeTagValueObject();
    return result;
}



/***********************************************************************************************************************/

/**
 *
 * 计算Mac
 *
 * @param heData 计算mac的数据
 * @param dataLen 计算mac数据的长度
 * @param heMac mac值
 * @param pMacLen mac值的长度
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderCalcMac(unsigned char *heData, int dataLen, unsigned char *heMac, int *pMacLen){

    int result = 1;
    jbyteArray jData = NULL;
    jbyteArray jMac = NULL;

    Log("cCallJava","Enter ProviderCalcMac");
    Log("cCallJava","Enter ProviderCalcMac  jniEnv = %d", jniEnv);
    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(CALC_MAC);
    }

    Log("cCallJava","ProviderCalcMac 111111111111111");
    if(heData == NULL || heMac == NULL || dataLen == 0){
        result = -4;
    }

    *pMacLen = 0;
    Log("cCallJava","ProviderCalcMac 22222222222222");
    if (result == 1) {
        Log("cCallJava","ProviderCalcMac 4444444444444 len = %d", dataLen);
        jData = (*jniEnv)->NewByteArray(jniEnv, dataLen);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jData, 0, dataLen, (jbyte *)heData);
        if(jData == NULL) {
            Log("cCallJava","ProviderCalcMac jTag == NULL || jValue == NULL");
            result = -1;
        }
        else{
            Log("cCallJava","ProviderCalcMac 4444444444444 1111111111111");
            jMac = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, calcMacFromJava, jData);
            Log("cCallJava","ProviderCalcMac 4444444444444 222222222222");
            result = -1;
            if(jMac != NULL) {
                *pMacLen = (*jniEnv)->GetArrayLength(jniEnv,jMac);
                Log("cCallJava","ProviderCalcMac 4444444444444 33333333333333333  = %d", jMac);
                Log("cCallJava","ProviderCalcMac 4444444444444 pMacLen = %d", *pMacLen);
                (*jniEnv)->GetByteArrayRegion(jniEnv, jMac, 0, *pMacLen, heMac);
                Log("cCallJava","ProviderCalcMac 666666666666666666666666666666666666");
                result = 1;
            }
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            (*jniEnv)->DeleteLocalRef(jniEnv, jMac);
            Log("cCallJava","ProviderCalcMac 6666666666666666666666666666666666661111111111111");
        }

    }

    Log("cCallJava","ProviderCalcMac 666666666666666666666666666666666666");
    freeDeviceObject();
    Log("cCallJava","ProviderCalcMac 777777777777777777777777777777777777777777");
    return result;
}

/**
 *
 * 获取系统时间
 * ptimeArray : 6 byte array ：13 01 21 19 42 18 表示13年1月21日19点42 分18秒
 *
 * @return 成功，返回值的长度；-4 参数错误，-1其他错误
 */
int ProviderGetSysTime(unsigned char *ptimeArray,int *pOutdataLen)
{
    int result = 1;
    jbyteArray jOutData = NULL;
	u8 aucTempData[256] = {0};

    Log("cCallJava","jump into ProviderGetSysTime");

    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
		result = InitDeviceProvider(SYS_TIME);
		Log("cCallJava","ProviderGetSysTime 111111111111111");
	}

    if(ptimeArray == NULL){
		result = -4;
		Log("cCallJava","ProviderGetSysTime 22222222222222");
	}
    if (result == 1) {
		Log("cCallJava","ProviderGetSysTime 4444444444444");
		jOutData = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, getSysTimeFromJava);
		result = -1;
		if(jOutData != NULL) {
			Log("cCallJava","jOutData not null");
			*pOutdataLen = (*jniEnv)->GetArrayLength(jniEnv,jOutData);
			Log("cCallJava","ProviderGetSysTime 4444444444444 pOutdataLen = %d", *pOutdataLen);
			(*jniEnv)->GetByteArrayRegion(jniEnv, jOutData, 0, *pOutdataLen, ptimeArray);

			memset(aucTempData,0,256);
			sdkBcdToAsc(aucTempData,ptimeArray,8);
			Log("cCallJava","jOutData = %s", aucTempData);
			result = 1;
			Log("cCallJava","ProviderGetSysTime 666666666666666666666666666666666666");
		}
		(*jniEnv)->DeleteLocalRef(jniEnv, jOutData);
		Log("cCallJava","ProviderGetSysTime 66666666666666666666661 result = %d", result);

	}

	Log("cCallJava","ProviderGetSysTime 666666666666666666666666666666666666");
	freeDeviceObject();
	Log("cCallJava","ProviderGetSysTime 777777777777777777777777777777777777777777");
	return result;
}

/**
 * 使用 TMS 主密钥进行 Des 运算
 * @param indata 待DES计算的数据
 * @param dataLen 8字节数据
 * @param desMode DES模式： 0-加密 1-解密
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderDesByTmsKey(unsigned char *indata, int dataLen, unsigned char *Outdata, int *pOutdataLen, int desMode){

    int result = 1;
    jbyteArray jData = NULL;
    jbyteArray jOutData = NULL;
    u8 aucTempData[256] = {0};

    Log("cCallJava","ProviderDesByTmsKey 000000000000000");
    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(DES_BY_TMSKEY);
        Log("cCallJava","ProviderDesByTmsKey 111111111111111");
    }

    if(indata == NULL || dataLen == 0 || dataLen!=8 ){
        result = -4;
        Log("cCallJava","ProviderDesByTmsKey 22222222222222");
    }

    if (result == 1) {
        Log("cCallJava","ProviderDesByTmsKey 4444444444444 len = %d", dataLen);
        jData = (*jniEnv)->NewByteArray(jniEnv, dataLen);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jData, 0, dataLen, (jbyte *)indata);
        if(jData == NULL) {
            Log("cCallJava","ProviderDesByTmsKey jTag == NULL || jValue == NULL");
            result = -1;
        }
        else{
            Log("cCallJava","ProviderDesByTmsKey 4444444444444 1111111111111");
            jOutData = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, desByTmsKey, jData, desMode);

            result = -1;
            if(jOutData != NULL) {
            	Log("cCallJava","jOutData not null");
                *pOutdataLen = (*jniEnv)->GetArrayLength(jniEnv,jOutData);
//                Log("cCallJava","ProviderDesByTmsKey 4444444444444 33333333333333333  = %d", jOutData);
                Log("cCallJava","ProviderDesByTmsKey 4444444444444 pOutdataLen = %d", *pOutdataLen);
                (*jniEnv)->GetByteArrayRegion(jniEnv, jOutData, 0, *pOutdataLen, Outdata);

//            	jsize len  = (*jniEnv)->GetArrayLength(jniEnv,jOutData);
//				jbyte *jbarray = (jbyte *)malloc(len * sizeof(jbyte));
//				env->GetByteArrayRegion(jOutData,0,len,jbarray);
//				BYTE *dDate=(BYTE*)jbarray;

                memset(aucTempData,0,256);
				sdkBcdToAsc(aucTempData,Outdata,8);
				Log("cCallJava","jOutData = %s", aucTempData);
                Log("cCallJava","ProviderDesByTmsKey 666666666666666666666666666666666666");
                result = 1;
            }
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            (*jniEnv)->DeleteLocalRef(jniEnv, jOutData);
            Log("cCallJava","ProviderDesByTmsKey 66666666666666666666661 result = %d", result);
        }
    }

    Log("cCallJava","ProviderDesByTmsKey 666666666666666666666666666666666666");
    freeDeviceObject();
    Log("cCallJava","ProviderDesByTmsKey 777777777777777777777777777777777777777777");
    return result;
}

/**
 * 使用除 PinKey 外的工作密钥进行 DES 加密
 * @param indata 等待DES运算的数据
 * @param wKeyType 0(int) PIN 密钥 ；1(int) MAC 密钥；2(int) 磁道加密密钥
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderDesEncByWKey(unsigned char *indata, int dataLen, unsigned char *Outdata, int *pOutdataLen,int wKeyType){

    int result = 1;
    jbyteArray jData = NULL;
    jbyteArray jOutData = NULL;

    Log("cCallJava","ProviderDesEncByWKey 000000000000000");
    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(DES_BY_WKEY);
    }

    Log("cCallJava","ProviderDesEncByWKey 111111111111111");
    if(indata == NULL || dataLen == 0 || dataLen!=8 ){
        result = -4;
    }

    Log("cCallJava","ProviderDesEncByWKey 22222222222222");
    if (result == 1) {
        Log("cCallJava","ProviderDesEncByWKey 4444444444444 len = %d", dataLen);
        jData = (*jniEnv)->NewByteArray(jniEnv, dataLen);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jData, 0, dataLen, (jbyte *)indata);
        if(jData == NULL) {
            Log("cCallJava","ProviderDesEncByWKey jTag == NULL || jValue == NULL");
            result = -1;
        }
        else{
            Log("cCallJava","ProviderDesEncByWKey 4444444444444 1111111111111");
            jOutData = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, desEncByWKey, jData, wKeyType);
            result = -1;
            if(jOutData != NULL) {
                *pOutdataLen = (*jniEnv)->GetArrayLength(jniEnv,jOutData);
                Log("cCallJava","ProviderDesEncByWKey 4444444444444 33333333333333333  = %d", jOutData);
                Log("cCallJava","ProviderDesEncByWKey 4444444444444 pOutdataLen = %d", *pOutdataLen);
                (*jniEnv)->GetByteArrayRegion(jniEnv, jOutData, 0, *pOutdataLen, Outdata);
                Log("cCallJava","ProviderDesEncByWKey 666666666666666666666666666666666666");
                result = 1;
            }
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            (*jniEnv)->DeleteLocalRef(jniEnv, jOutData);
            Log("cCallJava","ProviderDesEncByWKey 66666666666666666666661 result = %d", result);
        }
    }

    Log("cCallJava","ProviderDesEncByWKey 666666666666666666666666666666666666");
    freeDeviceObject();
    Log("cCallJava","ProviderDesEncByWKey 777777777777777777777777777777777777777777");
    return result;
}

/**
 * 更新工作密钥
 * @param heWkData 签到收到62域的数据
 * @param dataLen 24或40或56或60或84个字节
 *
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderUpdateWk(unsigned char *heWkData, int dataLen){

    int result = 1;
    jbyteArray jData = NULL;
    jboolean bResult;

    Log("cCallJava","ProviderUpdateWk 000000000000000");
    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(UPDATE_WK);
    }

    Log("cCallJava","ProviderUpdateWk 111111111111111");
    if(heWkData == NULL || dataLen == 0 ){
        result = -4;
    }

    Log("cCallJava","ProviderUpdateWk 22222222222222");
    if (result == 1) {
        Log("cCallJava","ProviderUpdateWk 4444444444444 len = %d", dataLen);
        jData = (*jniEnv)->NewByteArray(jniEnv, dataLen);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jData, 0, dataLen, (jbyte *)heWkData);
        if(jData == NULL) {
            Log("cCallJava","ProviderUpdateWk jTag == NULL || jValue == NULL");
            result = -1;
        }
        else{
            Log("cCallJava","Provider/UpdateWk 4444444444444 1111111111111");
            bResult = (*jniEnv)->CallBooleanMethod(jniEnv, mDeviceApusic, updateWk, jData);
            result = (bResult ? 1: -1);
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            Log("cCallJava","ProviderUpdateWk 66666666666666666666661 result = %d", result);
        }
    }

    Log("cCallJava","ProviderUpdateWk 666666666666666666666666666666666666");
    freeDeviceObject();
    Log("cCallJava","ProviderUpdateWk 777777777777777777777777777777777777777777");
    return result;
}

/**
 * 下载主密钥
 * @param heWkData 签到收到62域的数据
 * @param dataLen 24或40或56或60或84个字节
 *
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderUpdatePMK(unsigned char *heWkData, int dataLen){

    int result = 1;
    jbyteArray jData = NULL;

    Log("cCallJava","ProviderUpdateWk 000000000000000");
    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(UPDATE_PMK);
    }

    Log("cCallJava","ProviderUpdateWk 111111111111111");
    if(heWkData == NULL || dataLen == 0 ){
        result = -4;
    }

    Log("cCallJava","ProviderUpdateWk 22222222222222");
    if (result == 1) {
        Log("cCallJava","ProviderUpdateWk 4444444444444 len = %d", dataLen);
        jData = (*jniEnv)->NewByteArray(jniEnv, dataLen);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jData, 0, dataLen, (jbyte *)heWkData);
        if(jData == NULL) {
            Log("cCallJava","ProviderUpdateWk jTag == NULL || jValue == NULL");
            result = -1;
        }
        else{
            Log("cCallJava","ProviderUpdateWk 4444444444444 1111111111111");
            (*jniEnv)->CallBooleanMethod(jniEnv, mDeviceApusic, updatePmk, jData);
            result = 1;
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            Log("cCallJava","ProviderUpdateWk 66666666666666666666661 result = %d", result);
        }
    }

    Log("cCallJava","ProviderUpdateWk 666666666666666666666666666666666666");
    freeDeviceObject();
    Log("cCallJava","ProviderUpdateWk 777777777777777777777777777777777777777777");
    return result;
}

/**
 *
 * 二次授权
 *
 * @param asRespCode 39域的应答码
 * @param asAuthCode 38域授权码
 * @param heRecvF55 55域数据 （必须存在 ARPC）
 * @param uiRecvF55Len 55域数据长度
 * @param pOutData 执行二次授权传出的数据（执行发卡行脚本结果）
 *
 * @return 1 成功 ，-4 参数错误，-1其他错误
 */
int ProviderSecAuth(char *asRespCode, char *asAuthCode, unsigned char *heRecvF55, unsigned int uiRecvF55Len, unsigned char *pOutData, int *pOutLen){

    int result = 1, scriptLen = 0, tcLen = 0, offset = 0;
    jstring jRespCode = NULL;
    jstring jAuthCode = NULL;
    jbyteArray jRecvF55 = NULL;
    jbyteArray jOutData = NULL;
    jbyteArray jTcData = NULL;

    if(DeviceApusic == NULL) {
        result = InitDeviceProvider(SEC_AUTH);
    }

    if(asRespCode == NULL || heRecvF55 == 0 || uiRecvF55Len == 0){
        result = -4;
    }

    if (result == 1) {

        jRespCode = (*jniEnv)->NewStringUTF(jniEnv, asRespCode);
        jAuthCode = (*jniEnv)->NewStringUTF(jniEnv, asAuthCode);
        jRecvF55 = (*jniEnv)->NewByteArray(jniEnv, uiRecvF55Len);
        jOutData = (*jniEnv)->NewByteArray(jniEnv, 512);
        jTcData = (*jniEnv)->NewByteArray(jniEnv, 32);

        (*jniEnv)->SetByteArrayRegion(jniEnv, jRecvF55, 0, uiRecvF55Len, (jbyte *)heRecvF55);
        if(jRespCode == NULL || jAuthCode == NULL || jRecvF55 == NULL || jOutData) {
            Log("cCallJava","ProviderSecAuth jRespCode == NULL || jAuthCode == NULL || jRecvF55 == NULL");
            result = -1;
        }
        else{

            result = (*jniEnv)->CallStaticIntMethod(jniEnv, DeviceApusic, secAuth, jRespCode, jAuthCode, jRecvF55, jOutData, jTcData);
            if(result == 0) {
                result = -1;
            }
            else{
                scriptLen = (*jniEnv)->GetArrayLength(jniEnv, jOutData);
                tcLen = (*jniEnv)->GetArrayLength(jniEnv, jTcData);
            }

            offset = 0;
            if(scriptLen>0){
                pOutData[offset++] = 0xDF;
                pOutData[offset++] = 0x31;
                pOutData[offset++] = scriptLen;
                (*jniEnv)->SetByteArrayRegion(jniEnv, jOutData, 0, scriptLen, &pOutData[offset]);
                offset += scriptLen;
            }

            if(tcLen>0){
                pOutData[offset++] = 0x9F;
                pOutData[offset++] = 0x26;
                pOutData[offset++] = tcLen;
                (*jniEnv)->SetByteArrayRegion(jniEnv, jTcData, 0, tcLen, &pOutData[offset]);
                offset += scriptLen;
            }

            *pOutLen = offset;
            Log("cCallJava","ProviderSecAuth 6666666666666666666666666666666666661111111111111");
        }

        (*jniEnv)->DeleteLocalRef(jniEnv, jRespCode);
        (*jniEnv)->DeleteLocalRef(jniEnv, jAuthCode);
        (*jniEnv)->DeleteLocalRef(jniEnv, jRecvF55);
        (*jniEnv)->DeleteLocalRef(jniEnv, jOutData);
        (*jniEnv)->DeleteLocalRef(jniEnv, jTcData);
    }

    freeDeviceObject();
    return result;
}

#if 0
/**
 *
 * 电子签名
 *
 * @param asData 签名板上的编号，也可为空
 * @param asPath 签名结束返回的路径+文件名，建议asPath大于128字节
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderESign(char *asData, char *asPath){

    int result = 1;
    int len = 0;
    jstring jData = NULL;
    jstring jPath = NULL;

    if(DeviceApusic == NULL) {
        result = InitDeviceProvider(E_SIGN4);
    }

    if(asPath == NULL){
        result = -4;
    }

    if (result == 1) {

        jData = (*jniEnv)->NewStringUTF(jniEnv, asData);
        if(jData == NULL) {
            Log("cCallJava","ProviderESign jData");
            result = -1;
        }
        else{

            jPath = (*jniEnv)->CallStaticObjectMethod(jniEnv, DeviceApusic, eSign, jData);
            if(jPath == NULL) {
                result = -1;
            }
            else{
                len = (*jniEnv)->GetStringUTFLength(jniEnv, jPath);
                (*jniEnv)->GetStringUTFRegion(jniEnv, jPath, 0, len, asPath);
                asPath[len] = '\0';
            }
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            Log("cCallJava","ProviderSecAuth 6666666666666666666666666666666666661111111111111");
        }
    }

    freeDeviceObject();
    return result;
}
#endif

/**
 *
 * 打印脚本数据
 *
 * @param asPrintData 待打印的脚本数据
 *
 * @return 1成功，-4 参数错误，-1其他错误
 */
int ProviderPrintScript(char *asPrintData){

    int result = 1;
    int len = 0;
    jstring jData = NULL;
    jboolean bResult;

    Log("cCallJava","ProviderPrintScript asPrintData =  %s",asPrintData);
    if(DeviceApusic == NULL) {
        result = InitDeviceProvider(PRINT_SCRIPT);
    }

    if(asPrintData == NULL){
        result = -4;
    }

    if (result == 1) {

        jData = (*jniEnv)->NewStringUTF(jniEnv, asPrintData);
        if(jData == NULL) {
            Log("cCallJava","ProviderPrintScript jData");
            result = -1;
        }
        else{

            bResult = (*jniEnv)->CallStaticBooleanMethod(jniEnv, DeviceApusic, printScript, jData);
            result = (bResult ? 1: -1);
            (*jniEnv)->DeleteLocalRef(jniEnv, jData);
            Log("cCallJava","ProviderPrintScript 6666666666666666666666666666666666661111111111111");
        }
    }

    freeDeviceObject();
    return result;
}

/**
 *
 * 获取Emv内核或IC的某个Tag的值
 *
 * @param tag 要获取的tag
 * @param pathID 通道ID，0是通过emv内核，1通过发送APDU到卡片获取
 * @param value 获取到的值
 *
 * @return 返回value的长度，-4 参数错误，-1其他错误
 */
int ProviderGetEmvTagValue(unsigned char *tag, int pathID, unsigned char *value)
{
    int result = 1;
    jbyteArray jValue = NULL;
    jbyteArray jTag = NULL;

    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(EMV_GET_TAG_VALUE);
    }

    if(tag == NULL || value == NULL || pathID > 2){
        result = -4;
    }

    if (result == 1) {

        if((tag[0]&0x1F) == 0x1F){
            jTag = (*jniEnv)->NewByteArray(jniEnv, 2);
            (*jniEnv)->SetByteArrayRegion(jniEnv, jTag, 0, 2, (jbyte *)tag);
        }
        else{
            jTag = (*jniEnv)->NewByteArray(jniEnv, 1);
            (*jniEnv)->SetByteArrayRegion(jniEnv, jTag, 0, 1, (jbyte *)tag);
        }

        jValue = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, emvGetTagValue, jTag, pathID);
        if(jValue == NULL){
            result = 1;

        }
        else{
            result = (*jniEnv)->GetArrayLength(jniEnv,jValue);
            (*jniEnv)->GetByteArrayRegion(jniEnv, jValue, 0, result, (jbyte *)value);
        }
		(*jniEnv)->DeleteLocalRef(jniEnv, jTag);

    }

    freeDeviceObject();
    return result;
}

/**
 *
 * 设置Emv内核某个Tag的值
 *
 * @param tag 要设置的tag
 * @param value 要设置的值
 * @param len 要设置的值的长度
 *
 * @return 返回1成功，-4 参数错误，-1其他错误
 */
int ProviderSetEmvTagValue(unsigned char *tag, unsigned char *value, int len)
{
    int result = 1;
    jbyteArray jValue = NULL;
    jbyteArray jTag = NULL;

    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(EMV_SET_TAG_VALUE);
    }

    if(tag == NULL || value == NULL || len == 0){
        result = -4;
    }

    if (result == 1) {

        if((tag[0]&0x1F) == 0x1F){
            jTag = (*jniEnv)->NewByteArray(jniEnv, 2);
            (*jniEnv)->SetByteArrayRegion(jniEnv, jTag, 0, 2, (jbyte *)tag);
        }
        else{
            jTag = (*jniEnv)->NewByteArray(jniEnv, 1);
            (*jniEnv)->SetByteArrayRegion(jniEnv, jTag, 0, 1, (jbyte *)tag);
        }

        jValue = (*jniEnv)->NewByteArray(jniEnv, len);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jValue, 0, len, (jbyte *)value);

        result = (*jniEnv)->CallIntMethod(jniEnv, mDeviceApusic, emvSetTagValue, jTag, jValue);
        if(result == 0){
            result = 1;
        }
        else{
            result = -1;
        }
		(*jniEnv)->DeleteLocalRef(jniEnv, jTag);
		(*jniEnv)->DeleteLocalRef(jniEnv, jValue);

    }

    freeDeviceObject();
    return result;
}

/**
 *
 * 设置Emv内核某个Tag的值
 *
 * @param trackData 磁道数据 （例如 “6212345678901234D763821767912”）
 * @param phEncryptData 加密后数据
 * @param pOutLen 加密后长度
 *
 * 备注：磁道数据的填充默认填充 0，如果需要填充其他值，
 * 请先填充好再传入。
 * @return 返回1成功，-4 参数错误，-1其他错误
 */
int ProviderTrackDataEncrypt(char *trackData, unsigned char *phEncryptData, int *pOutLen)
{
    int result = 1;
    int dataLen = 0;
	int len;
	u8 hTrack[128] = {0};
    jbyteArray jValue = NULL;
	jbyteArray jEncryptData = NULL;
    

    if(DeviceApusic == NULL || mDeviceApusic == NULL) {
        result = InitDeviceProvider(TRACK_DATA_ENCRYPT);
    }

    if(trackData == NULL){
        result = -4;
    }

    //Log("cCallJava", "trackData =%s", trackData);
    dataLen = strlen(trackData);
    sdkAscToBcd(hTrack, trackData, dataLen);
    //Log("cCallJava", " trackData hTrack =0x%02x0x%02x0x%02x0x%02x0x%02x", hTrack[0], hTrack[1], hTrack[2],hTrack[3], hTrack[4]);
	len = dataLen/2 + dataLen%2;
    if (result == 1) {

        jValue = (*jniEnv)->NewByteArray(jniEnv, len);
        (*jniEnv)->SetByteArrayRegion(jniEnv, jValue, 0, len, (jbyte *)hTrack);

        jEncryptData = (*jniEnv)->CallObjectMethod(jniEnv, mDeviceApusic, trackDataEncrypt, jValue);
        if(jEncryptData == NULL){
            result = -1;
        }
        else{
            len = (*jniEnv)->GetArrayLength(jniEnv,jEncryptData);
			(*jniEnv)->GetByteArrayRegion(jniEnv, jEncryptData, 0, len, (jbyte *)phEncryptData);
            //Log("cCallJava", "trackData len =%d", len);
            //Log("cCallJava", "trackData =0x%02x0x%02x0x%02x0x%02x0x%02x", \
            //    phEncryptData[0], phEncryptData[1], phEncryptData[2],phEncryptData[3], phEncryptData[4]);
			*pOutLen = len;
        }
		
		(*jniEnv)->DeleteLocalRef(jniEnv, jValue);
    }

    freeDeviceObject();
    return result;
}


void freeTagValueObject(){

    if(TradeTlv != NULL)
        (*jniEnv)->DeleteLocalRef(jniEnv, TradeTlv);
    TradeTlv = NULL;

    setTagValue = NULL;
    getTagValue = NULL;
    getTagValueLength = NULL;
	updateTagValueToDb = NULL;
	deleteAllRecord = NULL;
	deleteOneRecord = NULL;
	updateRecordOneTag = NULL;
}

void freeDeviceObject(){

    if(DeviceApusic != NULL)
        (*jniEnv)->DeleteLocalRef(jniEnv, DeviceApusic);
    DeviceApusic = NULL;

    Log("cCallJava","freeDeviceObject 1111111111111111111111111111111");
    if(mDeviceApusic != NULL)
        (*jniEnv)->DeleteLocalRef(jniEnv, mDeviceApusic);
    mDeviceApusic = NULL;

    calcMacFromJava = NULL;
    updateWk = NULL;
    updatePmk = NULL;
    desByTmsKey = NULL;
    desEncByWKey = NULL;
    secAuth = NULL;
    eSign = NULL;
    printScript = NULL;
    emvGetTagValue = NULL;
    emvSetTagValue = NULL;
	trackDataEncrypt = NULL;
	getSysTimeFromJava = NULL;
}


