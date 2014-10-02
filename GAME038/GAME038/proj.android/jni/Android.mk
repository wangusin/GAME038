LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := game_shared

LOCAL_MODULE_FILENAME := libgame

MY_CPP_LIST := $(wildcard $(LOCAL_PATH)/*.cpp)
MY_CPP_LIST += $(wildcard $(LOCAL_PATH)/../../Classes/*.cpp)

echo "MY_CPP_LIST = $MY_CPP_LIST"

LOCAL_SRC_FILES := hellocpp/main.cpp
LOCAL_SRC_FILES += $(MY_CPP_LIST:$(LOCAL_PATH)/%=%)

echo "LOCAL_SRC_FILES = $LOCAL_SRC_FILES"

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static cocosdenshion_static cocos_extension_static box2d_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)  \
$(call import-module,CocosDenshion/android) \
$(call import-module,extensions) \
$(call import-module,external/Box2D)





