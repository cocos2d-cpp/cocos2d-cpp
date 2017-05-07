/****************************************************************************
 Copyright (c) 2012 cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.

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

#ifndef __TEXTURE2D_TEST_H__
#define __TEXTURE2D_TEST_H__

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(Texture2DTests);

class TextureDemo : public TestCase
{
public:
    virtual ~TextureDemo();
    virtual void onEnter() override;
};

class TextureTIFF : public TextureDemo
{
public:
    static TextureTIFF* create()
    {
        auto ret = new TextureTIFF;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};


class TextureTGA : public TextureDemo
{
public:
    static TextureTGA* create()
    {
        auto ret = new TextureTGA;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePNG : public TextureDemo
{
public:
    static TexturePNG* create()
    {
        auto ret = new TexturePNG;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureJPEG : public TextureDemo
{
public:
    static TextureJPEG* create()
    {
        auto ret = new TextureJPEG;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureWEBP : public TextureDemo
{
public:
    static TextureWEBP* create()
    {
        auto ret = new TextureWEBP;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureWEBPNoAlpha : public TextureDemo
{
public:
    static TextureWEBPNoAlpha* create()
    {
        auto ret = new TextureWEBPNoAlpha;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TextureMipMap : public TextureDemo
{
public:
    static TextureMipMap* create()
    {
        auto ret = new TextureMipMap;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRTest : public TextureDemo
{
public:
    static TexturePVRTest* create()
    {
        auto ret = new TexturePVRTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR2BPP : public TextureDemo
{
public:
    static TexturePVR2BPP* create()
    {
        auto ret = new TexturePVR2BPP;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR4BPP : public TextureDemo
{
public:
    static TexturePVR4BPP* create()
    {
        auto ret = new TexturePVR4BPP;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA8888 : public TextureDemo
{
public:
    static TexturePVRRGBA8888* create()
    {
        auto ret = new TexturePVRRGBA8888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRBGRA8888 : public TextureDemo
{
public:
    static TexturePVRBGRA8888* create()
    {
        auto ret = new TexturePVRBGRA8888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444 : public TextureDemo
{
public:
    static TexturePVRRGBA4444* create()
    {
        auto ret = new TexturePVRRGBA4444;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444GZ : public TextureDemo
{
public:
    static TexturePVRRGBA4444GZ* create()
    {
        auto ret = new TexturePVRRGBA4444GZ;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444CCZ : public TextureDemo
{
public:
    static TexturePVRRGBA4444CCZ* create()
    {
        auto ret = new TexturePVRRGBA4444CCZ;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA5551 : public TextureDemo
{
public:
    static TexturePVRRGBA5551* create()
    {
        auto ret = new TexturePVRRGBA5551;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB565 : public TextureDemo
{
public:
    static TexturePVRRGB565* create()
    {
        auto ret = new TexturePVRRGB565;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB888 : public TextureDemo
{
public:
    static TexturePVRRGB888* create()
    {
        auto ret = new TexturePVRRGB888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRA8 : public TextureDemo
{
public:
    static TexturePVRA8* create()
    {
        auto ret = new TexturePVRA8;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRI8 : public TextureDemo
{
public:
    static TexturePVRI8* create()
    {
        auto ret = new TexturePVRI8;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVRAI88 : public TextureDemo
{
public:
    static TexturePVRAI88* create()
    {
        auto ret = new TexturePVRAI88;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual void onEnter() override;
};

class TexturePVR2BPPv3 : public TextureDemo
{
public:
    static TexturePVR2BPPv3* create()
    {
        auto ret = new TexturePVR2BPPv3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRII2BPPv3 : public TextureDemo
{
public:
    static TexturePVRII2BPPv3* create()
    {
        auto ret = new TexturePVRII2BPPv3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVR4BPPv3 : public TextureDemo
{
public:
    static TexturePVR4BPPv3* create()
    {
        auto ret = new TexturePVR4BPPv3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRII4BPPv3 : public TextureDemo
{
public:
    static TexturePVRII4BPPv3* create()
    {
        auto ret = new TexturePVRII4BPPv3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA8888v3 : public TextureDemo
{
public:
    static TexturePVRRGBA8888v3* create()
    {
        auto ret = new TexturePVRRGBA8888v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRBGRA8888v3 : public TextureDemo
{
public:
    static TexturePVRBGRA8888v3* create()
    {
        auto ret = new TexturePVRBGRA8888v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA4444v3 : public TextureDemo
{
public:
    static TexturePVRRGBA4444v3* create()
    {
        auto ret = new TexturePVRRGBA4444v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGBA5551v3 : public TextureDemo
{
public:
    static TexturePVRRGBA5551v3* create()
    {
        auto ret = new TexturePVRRGBA5551v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB565v3 : public TextureDemo
{
public:
    static TexturePVRRGB565v3* create()
    {
        auto ret = new TexturePVRRGB565v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRRGB888v3 : public TextureDemo
{
public:
    static TexturePVRRGB888v3* create()
    {
        auto ret = new TexturePVRRGB888v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRA8v3 : public TextureDemo
{
public:
    static TexturePVRA8v3* create()
    {
        auto ret = new TexturePVRA8v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRI8v3 : public TextureDemo
{
public:
    static TexturePVRI8v3* create()
    {
        auto ret = new TexturePVRI8v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRAI88v3 : public TextureDemo
{
public:
    static TexturePVRAI88v3* create()
    {
        auto ret = new TexturePVRAI88v3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRBadEncoding : public TextureDemo
{
public:
    static TexturePVRBadEncoding* create()
    {
        auto ret = new TexturePVRBadEncoding;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRMipMap : public TextureDemo
{
public:
    static TexturePVRMipMap* create()
    {
        auto ret = new TexturePVRMipMap;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRMipMap2 : public TextureDemo
{
public:
    static TexturePVRMipMap2* create()
    {
        auto ret = new TexturePVRMipMap2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNonSquare : public TextureDemo
{
public:
    static TexturePVRNonSquare* create()
    {
        auto ret = new TexturePVRNonSquare;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNPOT4444 : public TextureDemo
{
public:
    static TexturePVRNPOT4444* create()
    {
        auto ret = new TexturePVRNPOT4444;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePVRNPOT8888 : public TextureDemo
{
public:
    static TexturePVRNPOT8888* create()
    {
        auto ret = new TexturePVRNPOT8888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureAlias : public TextureDemo
{
public:
    static TextureAlias* create()
    {
        auto ret = new TextureAlias;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TexturePixelFormat : public TextureDemo
{
public:
    static TexturePixelFormat* create()
    {
        auto ret = new TexturePixelFormat;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureBlend : public TextureDemo
{
public:
    static TextureBlend* create()
    {
        auto ret = new TextureBlend;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureAsync : public TextureDemo
{
public:
    static TextureAsync* create()
    {
        auto ret = new TextureAsync;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual ~TextureAsync();

    virtual float getDuration() const override { return 5.0f; }
    void loadImages(float dt);
    void imageLoaded(cocos2d::Texture2D* texture);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
private:
    int _imageOffset;
};

class TextureGlRepeat : public TextureDemo
{
public:
    static TextureGlRepeat* create()
    {
        auto ret = new TextureGlRepeat;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual ~TextureGlRepeat();
    virtual std::string title() const override;

    virtual void onEnter() override;
};

class TextureGlClamp : public TextureDemo
{
public:
    static TextureGlClamp* create()
    {
        auto ret = new TextureGlClamp;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual ~TextureGlClamp();
    virtual std::string title() const override;

    virtual void onEnter() override;
};

class TextureSizeTest : public TextureDemo
{
public:
    static TextureSizeTest* create()
    {
        auto ret = new TextureSizeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureCache1 : public TextureDemo
{
public:
    static TextureCache1* create()
    {
        auto ret = new TextureCache1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
};

class TextureDrawAtPoint : public TextureDemo
{
public:
    static TextureDrawAtPoint* create()
    {
        auto ret = new TextureDrawAtPoint;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ~TextureDrawAtPoint();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
protected:
    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

    cocos2d::CustomCommand _renderCmd;
    cocos2d::Texture2D* _tex1, *_Tex2F;
};

class TextureDrawInRect : public TextureDemo
{
public:
    static TextureDrawInRect* create()
    {
        auto ret = new TextureDrawInRect;
        ret->init();
        ret->autorelease();
        return ret;
    }
    ~TextureDrawInRect();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
protected:
    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);

    cocos2d::CustomCommand _renderCmd;
    cocos2d::Texture2D* _tex1, *_Tex2F;
};

class TextureMemoryAlloc : public TextureDemo
{
public:
    static TextureMemoryAlloc* create()
    {
        auto ret = new TextureMemoryAlloc;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void updateImage(cocos2d::Ref* sender);
    void changeBackgroundVisible(cocos2d::Ref* sender);
private:
    cocos2d::Sprite* _background;
};

class TexturePVRv3Premult : public TextureDemo
{
public:
    static TexturePVRv3Premult* create()
    {
        auto ret = new TexturePVRv3Premult;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TexturePVRv3Premult();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void transformSprite(cocos2d::Sprite *sprite);
};

// ETC1 texture format test
class TextureETC1 : public TextureDemo
{
public:
    static TextureETC1* create()
    {
        auto ret = new TextureETC1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureETC1();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC Dxt1 texture format test
class TextureS3TCDxt1 : public TextureDemo
{
public:
    static TextureS3TCDxt1* create()
    {
        auto ret = new TextureS3TCDxt1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureS3TCDxt1();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC Dxt3 texture format test
class TextureS3TCDxt3 : public TextureDemo
{
public:
    static TextureS3TCDxt3* create()
    {
        auto ret = new TextureS3TCDxt3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureS3TCDxt3();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// S3TC Dxt5 texture format test
class TextureS3TCDxt5 : public TextureDemo
{
public:
    static TextureS3TCDxt5* create()
    {
        auto ret = new TextureS3TCDxt5;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureS3TCDxt5();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


// S3TC texture with no mipmaps test
class TextureS3TCWithNoMipmaps : public TextureDemo
{
public:
    static TextureS3TCWithNoMipmaps* create()
    {
        auto ret = new TextureS3TCWithNoMipmaps;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureS3TCWithNoMipmaps();
    
    virtual std::string title() const override;
};

// ATITC RGB texture format test
class TextureATITCRGB : public TextureDemo
{
public:
    static TextureATITCRGB* create()
    {
        auto ret = new TextureATITCRGB;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureATITCRGB();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;    
};

//ATITC RGBA Explicit texture format test
class TextureATITCExplicit : public TextureDemo
{
public:
    static TextureATITCExplicit* create()
    {
        auto ret = new TextureATITCExplicit;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureATITCExplicit();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

//ATITC RGBA Interpolated texture format test
class TextureATITCInterpolated : public TextureDemo
{
public:
    static TextureATITCInterpolated* create()
    {
        auto ret = new TextureATITCInterpolated;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TextureATITCInterpolated();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


// RGB888 texture convert test
class TextureConvertRGB888 : public TextureDemo
{
public:
    static TextureConvertRGB888* create()
    {
        auto ret = new TextureConvertRGB888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// RGBA8888 texture convert test
class TextureConvertRGBA8888 : public TextureDemo
{
public:
    static TextureConvertRGBA8888* create()
    {
        auto ret = new TextureConvertRGBA8888;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// I8 texture convert test
class TextureConvertI8 : public TextureDemo
{
public:
    static TextureConvertI8* create()
    {
        auto ret = new TextureConvertI8;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// AI88 texture convert test
class TextureConvertAI88 : public TextureDemo
{
public:
    static TextureConvertAI88* create()
    {
        auto ret = new TextureConvertAI88;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif // __TEXTURE2D_TEST_H__
