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

#ifndef _SPRITE3D_TEST_H_
#define _SPRITE3D_TEST_H_

#include "BaseTest.h"

#include "3d/CCOBB.h"

#include <string>

namespace cocos2d {
    class Animate3D;
    class Delay;
    class DrawNode3D;
    class GLProgramState;
    class MenuItemFont;
    class MotionStreak3D;
    class Ray;
    class Sprite3D;
}

DEFINE_TEST_SUITE(Sprite3DTests);

class Sprite3DTestDemo : public TestCase
{
public:
    // overrides
    virtual std::string title() const override;
    
    virtual ~Sprite3DTestDemo();
};

class Sprite3DForceDepthTest : public Sprite3DTestDemo
{
public:
    static Sprite3DForceDepthTest* create()
    {
        auto ret = new Sprite3DForceDepthTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DForceDepthTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Sprite3DEmptyTest : public Sprite3DTestDemo
{
public:
    static Sprite3DEmptyTest* create()
    {
        auto ret = new Sprite3DEmptyTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DEmptyTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Sprite3DBasicTest : public Sprite3DTestDemo
{
public:
    static Sprite3DBasicTest* create()
    {
        auto ret = new Sprite3DBasicTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DBasicTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class Sprite3DUVAnimationTest : public Sprite3DTestDemo
{
public:
    static Sprite3DUVAnimationTest* create()
    {
        auto ret = new Sprite3DUVAnimationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DUVAnimationTest();
    virtual ~Sprite3DUVAnimationTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    void cylinderUpdate(float dt);
    
    float _cylinder_texture_offset;
    float _shining_duraion;
    cocos2d::GLProgramState * _state;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

class Sprite3DFakeShadowTest : public Sprite3DTestDemo
{
public:
    enum State
    {
        State_None = 0,
        State_Idle = 0x01,
        State_Move = 0x02,
        State_Rotate = 0x04
    };
    static Sprite3DFakeShadowTest* create()
    {
        auto ret = new Sprite3DFakeShadowTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DFakeShadowTest();
    virtual ~Sprite3DFakeShadowTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void Move(cocos2d::Ref* sender,int value);
    void updateCamera(float fDelta);
    void move3D(float elapsedTime);
    void updateState(float elapsedTime);
    bool isState(unsigned int state,unsigned int bit) const;
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
    cocos2d::Camera * _camera;
    cocos2d::Vec3 _targetPos;
    unsigned int   _curState;
    cocos2d::Sprite3D * _plane;
    cocos2d::Sprite3D * _orc;
    cocos2d::GLProgramState * _state;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

class Sprite3DLightMapTest : public Sprite3DTestDemo
{
public:
    static Sprite3DLightMapTest* create()
    {
        auto ret = new Sprite3DLightMapTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DLightMapTest();
    virtual ~Sprite3DLightMapTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
private:
    cocos2d::Camera * _camera;
};

class Sprite3DBasicToonShaderTest : public Sprite3DTestDemo
{
public:
    static Sprite3DBasicToonShaderTest* create()
    {
        auto ret = new Sprite3DBasicToonShaderTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DBasicToonShaderTest();
    virtual ~Sprite3DBasicToonShaderTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    cocos2d::GLProgramState * _state;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif

};

class Sprite3DHitTest : public Sprite3DTestDemo
{
public:
    static Sprite3DHitTest* create()
    {
        auto ret = new Sprite3DHitTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DHitTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Sprite3DEffectTest : public Sprite3DTestDemo
{
public:
    static Sprite3DEffectTest* create()
    {
        auto ret = new Sprite3DEffectTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DEffectTest();
    virtual ~Sprite3DEffectTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
protected:
    std::vector<cocos2d::Sprite3D*> _sprites;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

class AsyncLoadSprite3DTest : public Sprite3DTestDemo
{
public:
    static AsyncLoadSprite3DTest* create()
    {
        auto ret = new AsyncLoadSprite3DTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    AsyncLoadSprite3DTest();
    virtual ~AsyncLoadSprite3DTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void menuCallback_asyncLoadSprite(cocos2d::Ref* sender);
    
    void asyncLoad_Callback(cocos2d::Sprite3D* sprite, void* param);
    
protected:
    std::vector<std::string> _paths; //model paths to be loaded
};

class Sprite3DWithSkinTest : public Sprite3DTestDemo
{
public:
    static Sprite3DWithSkinTest* create()
    {
        auto ret = new Sprite3DWithSkinTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DWithSkinTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
    void switchAnimationQualityCallback(cocos2d::Ref* sender);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    std::string getAnimationQualityMessage() const;
private:
    std::vector<cocos2d::Sprite3D*> _sprits;
    int _animateQuality;
    cocos2d::MenuItemFont* _menuItem;
};

class Sprite3DWithSkinOutlineTest : public Sprite3DTestDemo
{
public:
    static Sprite3DWithSkinOutlineTest* create()
    {
        auto ret = new Sprite3DWithSkinOutlineTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DWithSkinOutlineTest();
    virtual ~Sprite3DWithSkinOutlineTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
protected:
    std::vector<cocos2d::Sprite3D*> _sprites;
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

class Animate3DTest : public Sprite3DTestDemo
{
public:
    static Animate3DTest* create()
    {
        auto ret = new Animate3DTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Animate3DTest();
    ~Animate3DTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    virtual void update(float dt) override;
    
protected:
    void addSprite3D();
    
    enum class State
    {
        SWIMMING,
        SWIMMING_TO_HURT,
        HURT,
        HURT_TO_SWIMMING,
    };
    
    void reachEndCallBack();
    
    void renewCallBack();
    
    cocos2d::Sprite3D* _sprite;
    
    std::unique_ptr<cocos2d::Action> _swim;
    std::unique_ptr<cocos2d::Animate3D> _hurt;

    float _elapseTransTime;
    
    State   _state;
};

class AttachmentTest : public Sprite3DTestDemo
{
public:
    static AttachmentTest* create()
    {
        auto ret = new AttachmentTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    AttachmentTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
protected:
    bool _hasWeapon;
    cocos2d::Sprite3D* _sprite;
};

class Sprite3DReskinTest : public Sprite3DTestDemo
{
public:
    static Sprite3DReskinTest* create()
    {
        auto ret = new Sprite3DReskinTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DReskinTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
    void menuCallback_reSkin(cocos2d::Ref* sender);
protected:
    void applyCurSkin();
    
    enum class SkinType
    {
        UPPER_BODY = 0,
        PANTS,
        SHOES,
        HAIR,
        FACE,
        HAND,
        GLASSES,
        MAX_TYPE,
    };
    
    std::vector<std::string> _skins[(int)SkinType::MAX_TYPE]; //all skins
    int                      _curSkin[(int)SkinType::MAX_TYPE]; //current skin index
    cocos2d::Sprite3D* _sprite;
};
class Sprite3DWithOBBPerformanceTest : public Sprite3DTestDemo
{
public:
    static Sprite3DWithOBBPerformanceTest* create()
    {
        auto ret = new Sprite3DWithOBBPerformanceTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DWithOBBPerformanceTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void update(float dt) override;
    void addNewOBBWithCoords(cocos2d::Vec2 p);
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void addOBBCallback(cocos2d::Ref* sender);
    void delOBBCallback(cocos2d::Ref* sender);
    void addOBBWithCount(float value);
    void delOBBWithCount(float value);
protected:
    cocos2d::Sprite3D*        _sprite;
    std::vector<cocos2d::OBB>          _obb;
    cocos2d::DrawNode3D*               _drawOBB;
    cocos2d::Label*                    _labelCubeCount;
    cocos2d::MoveTo*                   _moveAction;
    cocos2d::OBB                       _obbt;
    cocos2d::OBB                       _obbtOri; //tortoise origin obb
    cocos2d::DrawNode3D*               _drawDebug;
    bool                      _hasCollider;
    std::set<int>             _intersetList;
    void initDrawBox();
    void reachEndCallBack();
    
    void unproject(const cocos2d::Mat4& viewProjection, const cocos2d::Size* viewport, cocos2d::Vec3* src, cocos2d::Vec3* dst);
    void calculateRayByLocationInView(cocos2d::Ray* ray, const cocos2d::Vec2& location);
};

class Sprite3DMirrorTest : public Sprite3DTestDemo
{
public:
    static Sprite3DMirrorTest* create()
    {
        auto ret = new Sprite3DMirrorTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DMirrorTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    
protected:
    bool _hasWeapon;
    cocos2d::Sprite3D* _sprite;
    cocos2d::Sprite3D* _mirrorSprite;
};

class QuaternionTest : public Sprite3DTestDemo
{
public:
    static QuaternionTest* create()
    {
        auto ret = new QuaternionTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    QuaternionTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    virtual void update(float delta) override;
    
protected:
    cocos2d::Sprite3D* _sprite;
    float              _arcSpeed;
    float              _radius;
    float              _accAngle;
};

// 3d + 2d use case
class UseCaseSprite3D : public Sprite3DTestDemo
{
public:
    static UseCaseSprite3D* create()
    {
        auto ret = new UseCaseSprite3D;
        ret->init();
        ret->autorelease();
        return ret;
    }
    UseCaseSprite3D();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    virtual void update(float delta) override;
    
    void menuCallback_Message(cocos2d::Ref* sender);
    
protected:
    
    void switchCase();
    
    enum class USECASE{
        _3D_WITH_2D,
        _UI_3D_UI,
        MAX_CASE_NUM,
    };
    cocos2d::Label*      _label;
    int                  _caseIdx; // use case index
    std::string          _useCaseTitles[(int)USECASE::MAX_CASE_NUM];
};

// node animation test, cocos2d-x supports both skeletal animation and node animation
class NodeAnimationTest : public Sprite3DTestDemo
{
public:
    static NodeAnimationTest* create()
    {
        auto ret = new NodeAnimationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    NodeAnimationTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
protected:
    std::vector<cocos2d::Sprite3D*> _sprites;
    int _vectorIndex;
};

namespace cocos2d
{
class TextureCube;
class Skybox;
}
class Sprite3DCubeMapTest : public Sprite3DTestDemo
{
public:
    static Sprite3DCubeMapTest* create()
    {
        auto ret = new Sprite3DCubeMapTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DCubeMapTest();
    ~Sprite3DCubeMapTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords(cocos2d::Vec2);

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

protected:
    cocos2d::TextureCube* _textureCube;
    cocos2d::Skybox* _skyBox;
    cocos2d::Sprite3D* _teapot;
    cocos2d::Camera *_camera;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

/// issue 9767 setGLProgram
class Issue9767 : public Sprite3DTestDemo
{
    enum class ShaderType
    {
        SHADER_TEX,
        SHADER_COLOR,
    };
public:
    static Issue9767* create()
    {
        auto ret = new Issue9767;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue9767();
    ~Issue9767();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void menuCallback_SwitchShader(cocos2d::Ref* sender);
    
protected:
    ShaderType          _shaderType;
    cocos2d::Sprite3D*  _sprite;
};

/// Clipping Sprite3D
class Sprite3DClippingTest : public Sprite3DTestDemo
{
public:
    static Sprite3DClippingTest* create()
    {
        auto ret = new Sprite3DClippingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DClippingTest();
    ~Sprite3DClippingTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Animate3DCallbackTest : public Sprite3DTestDemo
{
public:
    static Animate3DCallbackTest* create()
    {
        auto ret = new Animate3DCallbackTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Animate3DCallbackTest();
    ~Animate3DCallbackTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    cocos2d::Sprite3D* _sprite3d;
};

class Sprite3DTestMeshLight : public Sprite3DTestDemo
{
public:
    static Sprite3DTestMeshLight* create()
    {
        auto ret = new Sprite3DTestMeshLight;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DTestMeshLight();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    cocos2d::Sprite3D* _sprite;
};

class CameraBackgroundClearTest : public Sprite3DTestDemo
{
public:
    static CameraBackgroundClearTest* create()
    {
        auto ret = new CameraBackgroundClearTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    CameraBackgroundClearTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void switch_CameraClearMode(cocos2d::Ref* sender);
protected:
    cocos2d::Camera* _camera;
    cocos2d::Label* _label;
};

class Sprite3DVertexColorTest : public Sprite3DTestDemo
{
public:
    static Sprite3DVertexColorTest* create()
    {
        auto ret = new Sprite3DVertexColorTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DVertexColorTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    virtual ~Sprite3DVertexColorTest();
protected:
    cocos2d::Sprite3D* _sprite;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    cocos2d::EventListenerCustom* _backToForegroundListener;
#endif
};

class MotionStreak3DTest : public Sprite3DTestDemo
{
public:
    static MotionStreak3DTest* create()
    {
        auto ret = new MotionStreak3DTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    MotionStreak3DTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void update(float delta) override;
    
protected:
    cocos2d::Sprite3D* _sprite;
    cocos2d::MotionStreak3D* _streak;
};

class Sprite3DNormalMappingTest : public Sprite3DTestDemo
{
public:
    static Sprite3DNormalMappingTest* create()
    {
        auto ret = new Sprite3DNormalMappingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DNormalMappingTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual ~Sprite3DNormalMappingTest();
};

class Sprite3DPropertyTest : public Sprite3DTestDemo
{
public:
    static Sprite3DPropertyTest* create()
    {
        auto ret = new Sprite3DPropertyTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DPropertyTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void update(float delta) override;

    void printMeshName(cocos2d::Ref* sender);
    void removeUsedTexture(cocos2d::Ref* sender);
    void resetTexture(cocos2d::Ref* sender);

    void refreshSpriteRender();
protected:
    cocos2d::Sprite3D* _sprite;
    cocos2d::Texture2D* _meshTex;
    std::string _texFile;
};

class Issue16155Test : public Sprite3DTestDemo
{
public:
    static Issue16155Test* create()
    {
        auto ret = new Issue16155Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue16155Test();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif
