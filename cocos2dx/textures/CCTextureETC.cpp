/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCTextureETC.h"
#include "platform/CCPlatformConfig.h"
#include "platform/CCFileUtils.h"
#include "CCConfiguration.h"
#include "etc/etc1.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

NS_CC_BEGIN

TextureETC::TextureETC()
: _name(0)
, _width(0)
, _height(0)
{}

TextureETC::~TextureETC()
{
}

bool TextureETC::initWithFile(const char *file)
{
    // Only Android supports ETC file format
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    bool ret = loadTextureV2(FileUtils::sharedFileUtils()->fullPathForFilename(file).c_str());
    return ret;
//#else
    //return false;
//#endif
}

unsigned int TextureETC::getName() const
{
    return _name;
}

unsigned int TextureETC::getWidth() const
{
    return _width;
}

unsigned int TextureETC::getHeight() const
{
    return _height;
}

// Call back function for java
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define  LOG_TAG    "CCTextureETC.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

static unsigned int sWidth = 0;
static unsigned int sHeight = 0;
static unsigned char *sData = NULL;
static unsigned int sLength = 0;

extern "C"
{
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxETCLoader_nativeSetTextureInfo(JNIEnv* env, jobject thiz, jint width, jint height, jbyteArray data, jint dataLength)
    {
        sWidth = (unsigned int)width;
        sHeight = (unsigned int)height;
        sLength = dataLength;
        sData = new unsigned char[sLength];
        env->GetByteArrayRegion(data, 0, sLength, (jbyte*)sData);
    }
}
#endif



bool TextureETC::loadTexture(const char* file)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxETCLoader", "loadTexture", "(Ljava/lang/String;)Z"))
    {
        jstring stringArg1 = t.env->NewStringUTF(file);
        jboolean ret = t.env->CallStaticBooleanMethod(t.classID, t.methodID, stringArg1);
        
        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(t.classID);
        
        if (ret)
        {
            _width = sWidth;
            _height = sHeight;
            
            
            glGenTextures(1, &_name);
            glBindTexture(GL_TEXTURE_2D, _name);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            
            glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, sLength, sData);
            
            glBindTexture(GL_TEXTURE_2D, 0);
            
            delete [] sData;
            sData = NULL;
            
            GLenum err = glGetError();
            if (err != GL_NO_ERROR)
            {
                LOGD("width %d, height %d, lenght %d", _width, _height, sLength);
                LOGD("cocos2d: TextureETC: Error uploading compressed texture %s glError: 0x%04X", file, err);
                return false;
            }
            
            return true;
        }
        else
        {
            return false;
        }
    }
#else
    return false;
#endif
}

bool TextureETC::loadTextureV2(const char* fileName)
{
    unsigned long etcFileSize = 0;
    etc1_byte* etcFileData = NULL;
    etcFileData = FileUtils::sharedFileUtils()->getFileData(fileName, "rb", &etcFileSize);
    
    if(etcFileSize == 0)
    {
        //LOGD("open etc file %s failed",fileName);
        this->release();
        return false;
    }
    
    if(!etc1_pkm_is_valid(etcFileData))
    {
        //LOGD("%s is not a valid pkm file",fileName);
        this->release();
        delete[] etcFileData;
        return  false;
    }
    
    _width = etc1_pkm_get_width(etcFileData);
    _height = etc1_pkm_get_height(etcFileData);
    
    if( 0 == _width || 0 == _height )
    {
        this->release();
        delete[] etcFileData;
        return false;
    }
    

    
    if(Configuration::sharedConfiguration()->supportsETC())
    {
#ifdef GL_ETC1_RGB8_OES
        //direct set to gl
        glGenTextures(1, &_name);
        glBindTexture(GL_TEXTURE_2D, _name);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, _width, _height, 0, etcFileSize - ETC_PKM_HEADER_SIZE,
                               etcFileData + ETC_PKM_HEADER_SIZE);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        delete[] etcFileData;
        etcFileData = NULL;
        
        return true;
#endif
    }
    //if it is not gles or device do not support ETC, decode texture by software
    {
        int bytePerPixel = 3;
        GLenum fallBackType = GL_UNSIGNED_BYTE;
        
        /*bool fallBackUseShort = false;
        if(fallBackUseShort)
        {
            bytePerPixel = 2;
            fallBackType = GL_UNSIGNED_SHORT_5_6_5;
        }
        */
        unsigned int stride = _width * bytePerPixel;
        
        std::vector<unsigned char> decodeImageData(stride * _height);
        
        etc1_decode_image(etcFileData + ETC_PKM_HEADER_SIZE, &decodeImageData[0], _width, _height, bytePerPixel, stride);
        
        //set decoded data to gl
        
        glGenTextures(1, &_name);
        glBindTexture(GL_TEXTURE_2D, _name);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, fallBackType, &decodeImageData[0]);
        
        glBindTexture(GL_TEXTURE_2D, 0);
        
        delete[] etcFileData;
        etcFileData = NULL;
        
        return true;
        
    }
    

    
    
}

NS_CC_END
