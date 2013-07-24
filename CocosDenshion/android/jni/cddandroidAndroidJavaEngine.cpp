#include "cddandroidAndroidJavaEngine.h"
#include "platform/android/jni/JniHelper.h"
#include "ccdandroidUtils.h"
#include <android/log.h>
#include <jni.h>

// logging
#define  LOG_TAG    "cocosdenshion::android::AndroidJavaEngine"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

// Java class
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

namespace CocosDenshion {
    namespace android {

        bool AndroidJavaEngine::getJNIStaticMethodInfo(cocos2d::JniMethodInfo &methodinfo,
                                                       const char *methodName,
                                                       const char *paramCode) {
            return cocos2d::JniHelper::getStaticMethodInfo(methodinfo,
                                                           CLASS_NAME,
                                                           methodName,
                                                           paramCode);
        }

        AndroidJavaEngine::~AndroidJavaEngine() {
            cocos2d::JniMethodInfo methodInfo;

            if (!getJNIStaticMethodInfo(methodInfo, "end", "()V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::preloadBackgroundMusic(const char* pszFilePath) {
            std::string fullPath = CocosDenshion::android::getFullPathWithoutAssetsPrefix(pszFilePath);

            // void playBackgroundMusic(String,boolean)
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "preloadBackgroundMusic", "(Ljava/lang/String;)V")) {
                return;
            }
        
            jstring stringArg = methodInfo.env->NewStringUTF(fullPath.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop) {
            std::string fullPath = CocosDenshion::android::getFullPathWithoutAssetsPrefix(pszFilePath);
        
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "playBackgroundMusic", "(Ljava/lang/String;Z)V")) {
                return;
            }
        
            jstring stringArg = methodInfo.env->NewStringUTF(fullPath.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, bLoop);
            methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::stopBackgroundMusic(bool bReleaseData) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "stopBackgroundMusic", "()V")) {
                return;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::pauseBackgroundMusic() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "pauseBackgroundMusic", "()V")) {
                return;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::resumeBackgroundMusic() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "resumeBackgroundMusic", "()V")) {
                return;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::rewindBackgroundMusic() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "rewindBackgroundMusic", "()V")) {
                return;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }


        bool AndroidJavaEngine::willPlayBackgroundMusic() {
            return true;
        }

        bool AndroidJavaEngine::isBackgroundMusicPlaying() {
            cocos2d::JniMethodInfo methodInfo;
            jboolean ret = false;
        
            if (! getJNIStaticMethodInfo(methodInfo, "isBackgroundMusicPlaying", "()Z")) {
                return ret;
            }
        
            ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
            return ret;
        }

        float AndroidJavaEngine::getBackgroundMusicVolume() {
            cocos2d::JniMethodInfo methodInfo;
            jfloat ret = -1.0;
        
            if (! getJNIStaticMethodInfo(methodInfo, "getBackgroundMusicVolume", "()F")) {
                return ret;
            }
        
            ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
            return ret;
        }

        void AndroidJavaEngine::setBackgroundMusicVolume(float volume) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "setBackgroundMusicVolume", "(F)V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        float AndroidJavaEngine::getEffectsVolume() {
            cocos2d::JniMethodInfo methodInfo;
            jfloat ret = -1.0;
        
            if (! getJNIStaticMethodInfo(methodInfo, "getEffectsVolume", "()F")) {
                return ret;
            }
        
            ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
            return ret;
        }

        void AndroidJavaEngine::setEffectsVolume(float volume) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "setEffectsVolume", "(F)V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        unsigned int AndroidJavaEngine::playEffect(const char* pszFilePath, bool bLoop,
                                                   float pitch, float pan, float gain) {
            cocos2d::JniMethodInfo methodInfo;
            int ret = 0;
            std::string fullPath = CocosDenshion::android::getFullPathWithoutAssetsPrefix(pszFilePath);
        
            if (! getJNIStaticMethodInfo(methodInfo, "playEffect", "(Ljava/lang/String;Z)I")) {
                return ret;
            }
        
            jstring stringArg = methodInfo.env->NewStringUTF(fullPath.c_str());
            ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID,
                                                      methodInfo.methodID,
                                                      stringArg,
                                                      bLoop);
            methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
            return (unsigned int)ret;
        }

        void AndroidJavaEngine::pauseEffect(unsigned int nSoundId) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "pauseEffect", "(I)V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::pauseAllEffects() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "pauseAllEffects", "()V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::resumeEffect(unsigned int nSoundId) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "resumeEffect", "(I)V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::resumeAllEffects() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "resumeAllEffects", "()V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::stopEffect(unsigned int nSoundId) {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "stopEffect", "(I)V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::stopAllEffects() {
            cocos2d::JniMethodInfo methodInfo;
        
            if (! getJNIStaticMethodInfo(methodInfo, "stopAllEffects", "()V")) {
                return ;
            }
        
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::preloadEffect(const char* pszFilePath) {
            cocos2d::JniMethodInfo methodInfo;
            std::string fullPath = CocosDenshion::android::getFullPathWithoutAssetsPrefix(pszFilePath);
        
            if (! getJNIStaticMethodInfo(methodInfo, "preloadEffect", "(Ljava/lang/String;)V")) {
                return ;
            }
        
            jstring stringArg = methodInfo.env->NewStringUTF(fullPath.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }

        void AndroidJavaEngine::unloadEffect(const char* pszFilePath) {
            cocos2d::JniMethodInfo methodInfo;
            std::string fullPath = CocosDenshion::android::getFullPathWithoutAssetsPrefix(pszFilePath);
        
            if (! getJNIStaticMethodInfo(methodInfo, "unloadEffect", "(Ljava/lang/String;)V")) {
                return ;
            }
        
            jstring stringArg = methodInfo.env->NewStringUTF(fullPath.c_str());
            methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
            methodInfo.env->DeleteLocalRef(stringArg);
            methodInfo.env->DeleteLocalRef(methodInfo.classID);
        }
    }
}
