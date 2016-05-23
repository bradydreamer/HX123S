/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <stdio.h>
#include <string.h>
#include <jni.h>
#include <stdlib.h>
#include <android/log.h>


#define printf(...) __android_log_print(ANDROID_LOG_ERROR, "TAG", __VA_ARGS__);

#define  LOG_TAG    "your-log-tag"

#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

extern int appmain(int argc, const char **argv);

jstring
Java_com_nexgo_keyboardscan_KeyboardScanInterface_Read(JNIEnv *env,
                                                       jobject thiz) {
    int i = 16;
    char buffer[16] = {0};
//    itoa (i, buffer, 10);
    sprintf(buffer, "rand is %d", i);
    LOGD ("rand is %d", i);
    LOGE ("rand is %d", i);
    LOGI ("rand is %d", i);

    appmain(0, 0);

    return (*env)->NewStringUTF(env, buffer);
//    (*env)->NewS
}

jint
Java_com_nexgo_keyboardscan_KeyboardScanInterface_Open(JNIEnv *env,
                                                       jobject thiz) {
    LOGD ("open", 0);
    return 0;
}

jint
Java_com_nexgo_keyboardscan_KeyboardScanInterface_Close(JNIEnv *env,
                                                        jobject thiz) {
    return 1;
}
