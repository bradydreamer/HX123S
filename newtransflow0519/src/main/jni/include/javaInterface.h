//liyang 2015-8-5 add
//��JNIͨ�Žӿ�
#include <jni.h>

typedef struct {
    JNIEnv *myEnv;
    jobject gJniObj;
    jclass gJniClass;
    jmethodID jniMethodIdTestCallback;
    jmethodID construction_id;
    jmethodID jniApduIC;
    jmethodID jniIcTrack;//xiaox 15/11/24
} JAVA_INTERFACE;
typedef struct {
    JNIEnv *myEnv;
    jobject gJniObj;
    jclass gJniClass;
    jmethodID construction_id;
    jmethodID jniInputPwd;
    jmethodID jniConfirmCardNo;
    jmethodID jniSelApp;
    jmethodID jniCerVerify;
} JAVA_INTERFACE_EMVL2;


