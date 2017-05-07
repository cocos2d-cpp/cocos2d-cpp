#pragma once

#include <map>

#include "../BaseTest.h"

namespace cocos2d {
class DrawNode;
class Layer;
class MenuItemFont;
class PhysicsContact;
class PhysicsRayCastInfo;
class Sprite;
class SpriteBatchNode;
}

#if CC_USE_PHYSICS

DEFINE_TEST_SUITE(PhysicsTests);

class PhysicsDemo : public TestCase
{
public:
    PhysicsDemo();
    virtual ~PhysicsDemo();
    
    virtual bool init() override;
    virtual void onEnter() override;
    virtual std::string title() const override;
    
    void toggleDebugCallback(cocos2d::Ref* sender);
    
    cocos2d::Sprite* addGrossiniAtPosition(cocos2d::Vec2 p, float scale = 1.0);
    cocos2d::Sprite* makeBall(cocos2d::Vec2 point, float radius, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
    cocos2d::Sprite* makeBox(cocos2d::Vec2 point, cocos2d::Size size, int color = 0, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
    cocos2d::Sprite* makeTriangle(cocos2d::Vec2 point, cocos2d::Size size, int color = 0, cocos2d::PhysicsMaterial material = cocos2d::PHYSICSBODY_MATERIAL_DEFAULT);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void toggleDebug();
    
protected:
    
    const cocos2d::Texture2D* _spriteTexture;
    cocos2d::SpriteBatchNode* _ball;
    std::unordered_map<int, cocos2d::Node*> _mouses;
    bool _debugDraw;
};

class PhysicsDemoLogoSmash : public PhysicsDemo
{
public:
    static PhysicsDemoLogoSmash* create()
    {
        auto ret = new PhysicsDemoLogoSmash;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
};

class PhysicsDemoClickAdd : public PhysicsDemo
{
public:
    static PhysicsDemoClickAdd* create()
    {
        auto ret = new PhysicsDemoClickAdd;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual ~PhysicsDemoClickAdd();
    void onEnter() override;
    virtual std::string subtitle() const override;
    
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onAcceleration(cocos2d::Acceleration* acc, cocos2d::Event* event);
};

class PhysicsDemoPyramidStack : public PhysicsDemo
{
public:
    static PhysicsDemoPyramidStack* create()
    {
        auto ret = new PhysicsDemoPyramidStack;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    void updateOnce(float delta);
    virtual std::string title() const override;
};

class PhysicsDemoRayCast : public PhysicsDemo
{
public:
    static PhysicsDemoRayCast* create()
    {
        auto ret = new PhysicsDemoRayCast;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    PhysicsDemoRayCast();
    
    void onEnter() override;
    virtual std::string title() const override;
    void update(float delta) override;
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    
    void changeModeCallback(cocos2d::Ref* sender);
    
    bool anyRay(cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data);
    
private:
    float _angle;
    cocos2d::DrawNode* _node;
    int _mode;
};

class PhysicsDemoActions : public PhysicsDemo
{
public:
    static PhysicsDemoActions* create()
    {
        auto ret = new PhysicsDemoActions;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
};

class PhysicsDemoJoints : public PhysicsDemo
{
public:
    static PhysicsDemoJoints* create()
    {
        auto ret = new PhysicsDemoJoints;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
};

class PhysicsDemoPump : public PhysicsDemo
{
public:
    static PhysicsDemoPump* create()
    {
        auto ret = new PhysicsDemoPump;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    void update(float delta) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    float _distance;
    float _rotationV;
};

class PhysicsDemoOneWayPlatform : public PhysicsDemo
{
public:
    static PhysicsDemoOneWayPlatform* create()
    {
        auto ret = new PhysicsDemoOneWayPlatform;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
    
    bool onContactBegin(cocos2d::PhysicsContact& contact);
};

class PhysicsDemoSlice : public PhysicsDemo
{
public:
    static PhysicsDemoSlice* create()
    {
        auto ret = new PhysicsDemoSlice;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    bool slice(cocos2d::PhysicsWorld& world, const cocos2d::PhysicsRayCastInfo& info, void* data);
    void clipPoly(cocos2d::PhysicsShapePolygon* shape, cocos2d::Vec2 normal, float distance);
    
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    int _sliceTag;
};

class PhysicsDemoBug3988 : public PhysicsDemo
{
public:
    static PhysicsDemoBug3988* create()
    {
        auto ret = new PhysicsDemoBug3988;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PhysicsContactTest : public PhysicsDemo
{
public:
    static PhysicsContactTest* create()
    {
        auto ret = new PhysicsContactTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    void resetTest();
    bool onContactBegin(cocos2d::PhysicsContact& contact);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void onDecrease(cocos2d::Ref* sender);
    void onIncrease(cocos2d::Ref* sender);
    
private:
    int _yellowBoxNum;
    int _blueBoxNum;
    int _yellowTriangleNum;
    int _blueTriangleNum;
};

class PhysicsPositionRotationTest : public PhysicsDemo
{
public:
    static PhysicsPositionRotationTest* create()
    {
        auto ret = new PhysicsPositionRotationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
};

class PhysicsSetGravityEnableTest : public PhysicsDemo
{
public:
    static PhysicsSetGravityEnableTest* create()
    {
        auto ret = new PhysicsSetGravityEnableTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    void onScheduleOnce(float delta);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PhysicsDemoBug5482 : public PhysicsDemo
{
public:
    static PhysicsDemoBug5482* create()
    {
        auto ret = new PhysicsDemoBug5482;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void changeBodyCallback(cocos2d::Ref* sender);
private:
    cocos2d::Sprite* _nodeA;
    cocos2d::Sprite* _nodeB;
    cocos2d::PhysicsBody* _body;
    cocos2d::MenuItemFont* _button;
    bool _bodyInA;
};

class PhysicsFixedUpdate : public PhysicsDemo
{
public:
    static PhysicsFixedUpdate* create()
    {
        auto ret = new PhysicsFixedUpdate;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void onEnter() override;
    void updateStart(float delta);
    void addBall();
    virtual void update(float delta) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PhysicsTransformTest : public PhysicsDemo
{
public:
    static PhysicsTransformTest* create()
    {
        auto ret = new PhysicsTransformTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    
private:
    cocos2d::Sprite* _parentSprite;
    cocos2d::Layer* _rootLayer;
};

class PhysicsIssue9959 : public PhysicsDemo
{
public:
    static PhysicsIssue9959* create()
    {
        auto ret = new PhysicsIssue9959;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PhysicsIssue15932 : public PhysicsDemo
{
public:
    static PhysicsIssue15932* create()
    {
        auto ret = new PhysicsIssue15932;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif // #if CC_USE_PHYSICS
