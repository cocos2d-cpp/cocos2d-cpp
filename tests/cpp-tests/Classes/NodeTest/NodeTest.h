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

#ifndef _NODE_TEST_H_
#define _NODE_TEST_H_

#include "../BaseTest.h"

#include "base/CCDirector.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(CocosNodeTests);

class TestCocosNodeDemo : public TestCase
{
public:
    virtual std::string title() const override;

protected:
    TestCocosNodeDemo();
    virtual ~TestCocosNodeDemo();

    cocos2d::Director::Projection _preProjection;
};

class NodeTest2 : public TestCocosNodeDemo
{
public:
    static NodeTest2* create()
    {
        auto ret = new NodeTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class NodeTest4 : public TestCocosNodeDemo
{
public:
    static NodeTest4* create()
    {
        auto ret = new NodeTest4;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void delay2(float dt);
    void delay4(float dt);

    virtual std::string subtitle() const override;

protected:
    NodeTest4();
};

class NodeTest5 : public TestCocosNodeDemo
{
public:
    static NodeTest5* create()
    {
        auto ret = new NodeTest5;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void addAndRemove(float dt);
    virtual std::string subtitle() const override;

protected:
    NodeTest5();
};

class NodeTest6 : public TestCocosNodeDemo
{
public:
    static NodeTest6* create()
    {
        auto ret = new NodeTest6;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void addAndRemove(float dt);
    virtual std::string subtitle() const override;

protected:
    NodeTest6();
};

class StressTest1 : public TestCocosNodeDemo
{
public:
    static StressTest1* create()
    {
        auto ret = new StressTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void shouldNotCrash(float dt);
    void removeMe(Node* node);
    virtual std::string subtitle() const override;

protected:
    StressTest1();
};

class StressTest2 : public TestCocosNodeDemo
{
public:
    static StressTest2* create()
    {
        auto ret = new StressTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void shouldNotLeak(float dt);
    virtual std::string subtitle() const override;

protected:
    StressTest2();
};

class SchedulerTest1 : public TestCocosNodeDemo
{
public:
    static SchedulerTest1* create()
    {
        auto ret = new SchedulerTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void doSomething(float dt);
    virtual std::string subtitle() const override;

protected:
    SchedulerTest1();
};

class SchedulerCallbackTest : public TestCocosNodeDemo
{
public:
    static SchedulerCallbackTest* create()
    {
        auto ret = new SchedulerCallbackTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string subtitle() const override;
    virtual void onEnter() override;

protected:
    float _total;
    SchedulerCallbackTest();
};


class NodeToWorld : public TestCocosNodeDemo
{
public:
    static NodeToWorld* create()
    {
        auto ret = new NodeToWorld;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string subtitle() const override;

protected:
    NodeToWorld();
};

class NodeToWorld3D : public TestCocosNodeDemo
{
public:
    static NodeToWorld3D* create()
    {
        auto ret = new NodeToWorld3D;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string subtitle() const override;

protected:
    NodeToWorld3D();
};

class CameraOrbitTest : public TestCocosNodeDemo
{
public:
    static CameraOrbitTest* create()
    {
        auto ret = new CameraOrbitTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string subtitle() const override;

protected:
    CameraOrbitTest();
};

class CameraZoomTest : public TestCocosNodeDemo
{
public:
    static CameraZoomTest* create()
    {
        auto ret = new CameraZoomTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void update(float dt) override;

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string subtitle() const override;

protected:
    CameraZoomTest();
    float    _z;
};

class CameraCenterTest : public TestCocosNodeDemo
{
public:
    static CameraCenterTest* create()
    {
        auto ret = new CameraCenterTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    virtual void onEnter() override;
    virtual void onExit() override;

protected:
    CameraCenterTest();
};

class CameraTest1 : public TestCocosNodeDemo
{
public:
    static CameraTest1* create()
    {
        auto ret = new CameraTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void onExit() override;

protected:
    CameraTest1();

    cocos2d::Sprite* _sprite1;
    cocos2d::Sprite* _sprite2;
};

class CameraTest2 : public TestCocosNodeDemo
{
public:
    static CameraTest2* create()
    {
        auto ret = new CameraTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    virtual void onExit() override;

protected:
    CameraTest2();

    cocos2d::Sprite* _sprite1;
    cocos2d::Sprite* _sprite2;
};

class ConvertToNode : public TestCocosNodeDemo
{
public:
    static ConvertToNode* create()
    {
        auto ret = new ConvertToNode;
        ret->init();
        ret->autorelease();
        return ret;
    }
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    ConvertToNode();
};

class NodeOpaqueTest : public TestCocosNodeDemo
{
public:
    static NodeOpaqueTest* create()
    {
        auto ret = new NodeOpaqueTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    NodeOpaqueTest();
};

class NodeNonOpaqueTest : public TestCocosNodeDemo
{
public:
    static NodeNonOpaqueTest* create()
    {
        auto ret = new NodeNonOpaqueTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    NodeNonOpaqueTest();
};

class NodeGlobalZValueTest : public TestCocosNodeDemo
{
public:
    static NodeGlobalZValueTest* create()
    {
        auto ret = new NodeGlobalZValueTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual void update(float dt) override;

protected:
    NodeGlobalZValueTest();
    cocos2d::Sprite* _sprite;
};

class NodeNormalizedPositionTest1 : public TestCocosNodeDemo
{
public:
    static NodeNormalizedPositionTest1* create()
    {
        auto ret = new NodeNormalizedPositionTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    NodeNormalizedPositionTest1();
};

class NodeNormalizedPositionTest2 : public TestCocosNodeDemo
{
public:
    static NodeNormalizedPositionTest2* create()
    {
        auto ret = new NodeNormalizedPositionTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void update(float dt) override;

protected:
    NodeNormalizedPositionTest2();

    cocos2d::Size _copyContentSize;
    float _accum;
};

class NodeNormalizedPositionBugTest : public TestCocosNodeDemo
{
public:
    static NodeNormalizedPositionBugTest* create()
    {
        auto ret = new NodeNormalizedPositionBugTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    void update(float dt) override;

protected:
    NodeNormalizedPositionBugTest();
    
    float _accum;
    cocos2d::Sprite* sprite;
};

class NodeNameTest : public TestCocosNodeDemo
{
public:
    static NodeNameTest* create()
    {
        auto ret = new NodeNameTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
    virtual void onEnter() override;
    virtual void onExit() override;

    void test(float dt);
};

class Issue16100Test : public TestCocosNodeDemo
{
public:
    static Issue16100Test* create()
    {
        auto ret = new Issue16100Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual void onEnter() override;
    virtual void onExit() override;
};

class Issue16735Test : public TestCocosNodeDemo
{
public:
    static Issue16735Test* create()
    {
        auto ret = new Issue16735Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual void onEnter() override;
    virtual void onExit() override;
};

#endif
