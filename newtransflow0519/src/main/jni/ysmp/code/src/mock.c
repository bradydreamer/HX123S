#include <android/log.h>

#define  LOG_TAG    "daemon-mock"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

s32 GetTimerAutoRetValue(void) {
    LOGD("GetTimerAutoRetValue");
    return 0;
}

void WaitEnterEsc(void) {
    LOGD("WaitEnterEsc");
}


u8 *SavePathOverlap(u8 *aid, u8 *pData, u32 size) {
    LOGD("SavePathOverlap:%s", aid);
    return aid;
}

u8 *ReadPathOverlap(u8 *aid, u8 *pData, u32 size) {
    LOGD("ReadPathOverlap:%s", aid);
    return aid;
}

void DelPathFile(u8 *aid) {
    LOGD("DelPathFile:%s", aid);
}


s32 sdkAudioCommState(u8 state[]) {
    LOGD("sdkAudioCommState");
    return 0;
}

s32 sdkAudioCommOpen() {
    LOGD("sdkAudioCommOpen");
    return 0;
}


void printByteArray(u8 *value, u32 size) {
    LOGD("printByteArray:%s", value);
}


