# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

### emv
include $(CLEAR_VARS)
LOCAL_MODULE    := LibEMV
LOCAL_SRC_FILES := ADILib/lib/libemvandroid.so
include $(PREBUILT_SHARED_LIBRARY)

### spiddi
include $(CLEAR_VARS)
LOCAL_MODULE    := LibSPIDDI
LOCAL_SRC_FILES := ADILib/lib/libspipack.so
include $(PREBUILT_SHARED_LIBRARY)

### spiddijni
include $(CLEAR_VARS)
LOCAL_MODULE    := LibSPIDDIJNI
LOCAL_SRC_FILES := ADILib/lib/libxgd_ddi_jni.so
include $(PREBUILT_SHARED_LIBRARY)


### sdk smartpos
include $(CLEAR_VARS)

LOCAL_MODULE    := sdksmartpos

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ADILib/devinc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ADILib/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ADILib/mathinc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ADILib/pravteInc

LOCAL_C_INCLUDES += $(LOCAL_PATH)/project/libsdkdev/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/project/src/api
LOCAL_C_INCLUDES += $(LOCAL_PATH)/project/src/cpu
LOCAL_C_INCLUDES += $(LOCAL_PATH)/project/src/cpu/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/project/src/
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_C_INCLUDES += $(LOCAL_PATH)/libsdkemv/emvlibsrc/dllemv/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libsdkemv/emvlibsrc/dllemvv20/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/libsdkemv/emvlibsrc/dllemvv20/src


MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/libsdk*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/libsdk*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/libsdk*/*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/libsdk*/*/*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/project/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/project/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/project/*/*/*.c)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/project/*/*/*/*.c)

LOCAL_SRC_FILES := $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)
LOCAL_SHARED_LIBRARIES := LibEMV
LOCAL_SHARED_LIBRARIES += LibSPIDDI
#LOCAL_SHARED_LIBRARIES += LibSPIDDIJNI

LOCAL_CFLAGS := -DANDROID -DXGD_SDK_DEBUG -DDEBUG_EMV -DDEBUG_ICC -DSPI_DDI -DDEBUG_DEBUG
#LOCAL_CFLAGS := -DSPI_DDI
LOCAL_LDLIBS := -llog             
include $(BUILD_SHARED_LIBRARY)


### g870s
include $(CLEAR_VARS)
LOCAL_MODULE    := g870s

LOCAL_C_INCLUDES := $(LOCAL_PATH)/ADILib/inc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ADILib/mathinc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/ADILib/pravteInc
LOCAL_C_INCLUDES += $(LOCAL_PATH)/include

LOCAL_SRC_FILES := interface.c
LOCAL_SRC_FILES += $(wildcard $(LOCAL_PATH)/trade/*.c)
LOCAL_SHARED_LIBRARIES := LibEMV
LOCAL_SHARED_LIBRARIES += LibSPIDDI
LOCAL_SHARED_LIBRARIES += sdksmartpos

LOCAL_CFLAGS := -DANDROID -DXGD_SDK_DEBUG -DSPI_DDI -DDEBUG_DEBUG
#LOCAL_CFLAGS := -DSPI_DDI
LOCAL_LDLIBS := -llog
include $(BUILD_SHARED_LIBRARY)


