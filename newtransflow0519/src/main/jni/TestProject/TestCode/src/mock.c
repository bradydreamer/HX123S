#include "sdkGlobal.h"
#include "SdkTest.h"

#include <android/log.h>

#define  LOG_TAG    "test-mock"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

s32 Test_sdkPrintShowErr(s32 codeErr) {
    LOGE("Test_sdkPrintShowErr:%d", codeErr);
    return 0;
}

