#include <jni.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include "appglobal.h"

JNIEnv* jniEnv;
#ifdef __cplusplus   
extern "C" {  
#endif


//#define TraceHex(pasTag, pasInfo, pheDate, uiLen)



int Util_Asc2Bcd(char *AscBuf, char *BcdBuf, int AscLen)
{
	unsigned char is_high, by;

   is_high = !(AscLen % 2);
   *BcdBuf = 0x00;
   while(AscLen-->0)     
   {
	 by = *AscBuf++;

	 if ( !(by&0x10)&&(by>0x30)) 
	 {
		by += 9;

	 }
		 	 /*对字母和空格的处理,小写变大写,空格变0*/
     if (is_high)  *BcdBuf = by << 4;
     else
	   {
	   by&=0x0f;
	   *BcdBuf++ |= by;
	   }
	 is_high = !is_high;
	 }
   return 0;
}
/*
int Package(char *indata,int len,char *outdata,int *rlen)
{
	//static char cn8583[]="006e60000200006131043100280800003800010ac00016000007110735022900303030303030303030303030303031323334353637383132333435363738393031323334350011000000010010002853657175656e6365204e6f31354b33393030303030303031333130300003303031";
	//Util_Asc2Bcd(cn8583,outdata,strlen(cn8583));
	stpcpy(outdata,"baba");
	*rlen = 4;
	return 0;
}
int UnPackage(char *indata,int len,char *outdata,int *rlen)
{
	return 0;
}
*/

jint Java_com_nexgo_jniinterface_CallNative_initDdi(JNIEnv*  env,jobject this)
{
    int iRet = 0;

    Log("CallNative", "spi_ddi_sys_init iRet = %d",iRet);
    spi_ddi_sys_init();
    Log("CallNative", "spi_ddi_sys_init iRet = %d",iRet);
    return iRet;
}


jint Java_com_nexgo_jniinterface_CallNative_Package(JNIEnv*  env,jobject this ,jcharArray jtlvdata, jint datalen, jcharArray jpackage, jcharArray jplen)
{
    Log("JNI", "00000000000000000000000000");
	char tlvdata[512],package[512];
	int plen[10];
	int st=0;
	memset(tlvdata,0x00,sizeof(tlvdata));
	memset(package,0x00,sizeof(package));
	memset(plen,0x00,sizeof(plen));
	jniEnv = env;
	Log("Package","Enter Package  jniEnv = %d", jniEnv);
	if(jtlvdata==NULL){
		st=Package(NULL,datalen,package,plen);
	}
	else
	{
		(*env)->GetByteArrayRegion(env, jtlvdata, 0, datalen, (jbyte *) tlvdata);
		st=Package(tlvdata,datalen,package,plen);
	}
    Log("JNI", "2222222222222222222222222");
    //return 0;

	return st;//test

}
jint Java_com_nexgo_jniinterface_CallNative_UnPackage(JNIEnv*  env,jobject this , jcharArray jpackage, jint jplen,jcharArray jtlvdata, jcharArray datalen)
{
	
	char tlvdata[512],package[512];
	int tlvlen[10];
	int st=0;


	memset(tlvdata,0x00,sizeof(tlvdata));
	memset(package,0x00,sizeof(package));
	memset(tlvlen,0x00,sizeof(tlvlen));

	jniEnv = env;
	Log("UnPackage","Enter UnPackage  env = %d", env);
	(*env)->GetByteArrayRegion(env, jpackage, 0, jplen, (jbyte *) package);
	st=UnPackage(package,jplen,tlvdata,tlvlen);
	return st;
}

jint Java_com_nexgo_jniinterface_CallNative_jnitest(JNIEnv*  env,jobject this )
{
	jniEnv = env;


	return 88;
}

jint Java_com_nexgo_jniinterface_CallNative_formPrintScript(JNIEnv*  env,jobject this, jint printType)
{
	jniEnv = env;
	int iRet = 0;


	iRet = printMain(printType);
	return iRet;
}

jint Java_com_nexgo_jniinterface_CallNative_updateWkTest(JNIEnv*  env,jobject this )
{
	int iRet = 0;
	jboolean bRet;
	unsigned char buf[] = {0xF9,0x27,0x14,0xE8,0x0E,0x83,0x2E,0xDC,
						   0xAD,0xC6,0x7D,0x84,0x4B,0xF6,0xE9,0x1B,
						   0x1E,0x3A,0x9D,0x81,0xAD,0xC6,0x7D,0x84};
//    iRet = spi_ddi_innerkey_open();
//    Log("CallNative", "ddi_innerkey_open iRet = %d",iRet);
//    iRet = spi_ddi_innerkey_inject(0, 0x01, "12345678");
//    Log("CallNative", "ddi_innerkey_inject iRet = %d",iRet);

	jniEnv = env;
	bRet = ProviderUpdateWk(buf, 24);
	Log("CallNative", "ProviderUpdateWk iRet = %d",bRet);

	//iRet = ProviderSetTagValue("FF9000", "12345", 5);
	Log("CallNative", "ProviderSetTagValue iRet = %d",iRet);
	return iRet;
}

//int onSelectAid(int aidAmount, String[] aid);
//bool onConfirmCardNo(String cardNo);
//int onInputPwd();




#ifdef __cplusplus   
}  
#endif   
