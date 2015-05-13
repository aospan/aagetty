LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES:= aagetty.c
LOCAL_CFLAGS := -O2 -g -D__USE_GNU
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE := aagetty
LOCAL_FORCE_STATIC_EXECUTABLE := true
LOCAL_MODULE_PATH := $(TARGET_ROOT_OUT_SBIN)
LOCAL_STATIC_LIBRARIES := libbinder libutils libstdc++ libcutils liblog libm libc

include $(BUILD_EXECUTABLE)
