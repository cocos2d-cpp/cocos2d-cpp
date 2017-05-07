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

#ifndef _ActionsTest_H_
#define _ActionsTest_H_

#include "../BaseTest.h"

#include "2d/CCNode.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(ActionsTests);

class ActionsDemo : public TestCase
{
protected:
    cocos2d::Sprite*    _grossini;
    cocos2d::Sprite*    _tamara;
    cocos2d::Sprite*    _kathia;
public:
    virtual void onEnter() override;
    virtual void onExit() override;

    void centerSprites(unsigned int numberOfSprites);
    void alignSpritesLeft(unsigned int numberOfSprites);
    virtual std::string title() const override;
};

class ActionMove : public ActionsDemo
{
public:
    static ActionMove* create()
    {
        auto ret = new ActionMove;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionMove3D : public ActionsDemo
{
public:
    static ActionMove3D* create()
    {
        auto ret = new ActionMove3D;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionScale : public ActionsDemo
{
public:
    static ActionScale* create()
    {
        auto ret = new ActionScale;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionSkew : public ActionsDemo
{
public:
    static ActionSkew* create()
    {
        auto ret = new ActionSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionRotationalSkew : public ActionsDemo
{
public:
    static ActionRotationalSkew* create()
    {
        auto ret = new ActionRotationalSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionRotationalSkewVSStandardSkew : public ActionsDemo
{
public:
    static ActionRotationalSkewVSStandardSkew* create()
    {
        auto ret = new ActionRotationalSkewVSStandardSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionSkewRotateScale : public ActionsDemo
{
public:
    static ActionSkewRotateScale* create()
    {
        auto ret = new ActionSkewRotateScale;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionRotate : public ActionsDemo
{
public:
    static ActionRotate* create()
    {
        auto ret = new ActionRotate;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionRotateBy3D : public ActionsDemo
{
public:
    static ActionRotateBy3D* create()
    {
        auto ret = new ActionRotateBy3D;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionJump : public ActionsDemo
{
public:
    static ActionJump* create()
    {
        auto ret = new ActionJump;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionBezier : public ActionsDemo
{
public:
    static ActionBezier* create()
    {
        auto ret = new ActionBezier;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionBlink : public ActionsDemo
{
public:
    static ActionBlink* create()
    {
        auto ret = new ActionBlink;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionFade : public ActionsDemo
{
public:
    static ActionFade* create()
    {
        auto ret = new ActionFade;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionTint : public ActionsDemo
{
public:
    static ActionTint* create()
    {
        auto ret = new ActionTint;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionAnimate : public ActionsDemo
{
public:
    static ActionAnimate* create()
    {
        auto ret = new ActionAnimate;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    cocos2d::EventListenerCustom* _frameDisplayedListener;
};

class ActionSequence : public ActionsDemo
{
public:
    static ActionSequence* create()
    {
        auto ret = new ActionSequence;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionSequence2 : public ActionsDemo
{
public:
    static ActionSequence2* create()
    {
        auto ret = new ActionSequence2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    void callback1();
    void callback2(Node* sender);
    void callback3(Node* sender, long data);
};

class ActionSequence3 : public ActionsDemo
{
public:
    static ActionSequence3* create()
    {
        auto ret = new ActionSequence3;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionSpawn : public ActionsDemo
{
public:
    static ActionSpawn* create()
    {
        auto ret = new ActionSpawn;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionSpawn2 : public ActionsDemo
{
public:
    static ActionSpawn2* create()
    {
        auto ret = new ActionSpawn2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionReverse : public ActionsDemo
{
public:
    static ActionReverse* create()
    {
        auto ret = new ActionReverse;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionRepeat : public ActionsDemo
{
public:
    static ActionRepeat* create()
    {
        auto ret = new ActionRepeat;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionDelayTime : public ActionsDemo
{
public:
    static ActionDelayTime* create()
    {
        auto ret = new ActionDelayTime;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionReverseSequence : public ActionsDemo
{
public:
    static ActionReverseSequence* create()
    {
        auto ret = new ActionReverseSequence;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionReverseSequence2 : public ActionsDemo
{
public:
    static ActionReverseSequence2* create()
    {
        auto ret = new ActionReverseSequence2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionOrbit : public ActionsDemo
{
public:
    static ActionOrbit* create()
    {
        auto ret = new ActionOrbit;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string subtitle() const override;
};

class ActionRemoveSelf : public ActionsDemo
{
public:
    static ActionRemoveSelf* create()
    {
        auto ret = new ActionRemoveSelf;
        ret->init();
        ret->autorelease();
        return ret;
    }

	virtual void onEnter() override;
	virtual std::string subtitle() const override;
};

class ActionRepeatForever : public ActionsDemo
{
public:
    static ActionRepeatForever* create()
    {
        auto ret = new ActionRepeatForever;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    void repeatForever(Node* pTarget);
};

class ActionRotateToRepeat : public ActionsDemo
{
public:
    static ActionRotateToRepeat* create()
    {
        auto ret = new ActionRotateToRepeat;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionCallFuncN : public ActionsDemo
{
public:
    static ActionCallFuncN* create()
    {
        auto ret = new ActionCallFuncN;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void callback(Node* sender);
};

class ActionCallFuncND : public ActionsDemo
{
public:
    static ActionCallFuncND* create()
    {
        auto ret = new ActionCallFuncND;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void doRemoveFromParentAndCleanup(Node* sender, bool cleanup);
};

class ActionCallFunction : public ActionsDemo
{
public:
    static ActionCallFunction* create()
    {
        auto ret = new ActionCallFunction;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    void callback1();
    void callback2(Node* pTarget);
    void callback3(Node* pTarget, long data);
};


class ActionFollow : public ActionsDemo
{
public:
    static ActionFollow* create()
    {
        auto ret = new ActionFollow;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionFollowWithOffset : public ActionsDemo
{
    
public:
    static ActionFollowWithOffset* create()
    {
        auto ret = new ActionFollowWithOffset;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionTargeted : public ActionsDemo
{
public:
    static ActionTargeted* create()
    {
        auto ret = new ActionTargeted;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ActionTargetedReverse : public ActionsDemo
{
public:
    static ActionTargetedReverse* create()
    {
        auto ret = new ActionTargetedReverse;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ActionStacked : public ActionsDemo
{
public:
    static ActionStacked* create()
    {
        auto ret = new ActionStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void addNewSpriteWithCoords(cocos2d::Vec2 p);
    virtual void runActionsInSprite(cocos2d::Sprite* sprite);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class ActionMoveStacked : public ActionStacked
{
public:
    static ActionMoveStacked* create()
    {
        auto ret = new ActionMoveStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void runActionsInSprite(cocos2d::Sprite* sprite) override;
};

class ActionMoveJumpStacked : public ActionStacked
{
public:
    static ActionMoveJumpStacked* create()
    {
        auto ret = new ActionMoveJumpStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void runActionsInSprite(cocos2d::Sprite* sprite) override;
};

class ActionMoveBezierStacked : public ActionStacked
{
public:
    static ActionMoveBezierStacked* create()
    {
        auto ret = new ActionMoveBezierStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual void runActionsInSprite(cocos2d::Sprite* sprite) override;
};

class ActionCatmullRomStacked : public ActionsDemo
{
public:
    static ActionCatmullRomStacked* create()
    {
        auto ret = new ActionCatmullRomStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual ~ActionCatmullRomStacked();
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ActionCardinalSplineStacked : public ActionsDemo
{
public:
    static ActionCardinalSplineStacked* create()
    {
        auto ret = new ActionCardinalSplineStacked;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual ~ActionCardinalSplineStacked();
    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue1305 : public ActionsDemo
{
public:
    static Issue1305* create()
    {
        auto ret = new Issue1305;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    void log(Node* sender);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    cocos2d::node_ptr<cocos2d::Sprite> _spriteTmp;
};

class Issue1305_2 : public ActionsDemo
{
public:
    static Issue1305_2* create()
    {
        auto ret = new Issue1305_2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    void printLog1();
    void printLog2();
    void printLog3();
    void printLog4();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue1288 : public ActionsDemo
{
public:
    static Issue1288* create()
    {
        auto ret = new Issue1288;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue1288_2 : public ActionsDemo
{
public:
    static Issue1288_2* create()
    {
        auto ret = new Issue1288_2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue1327 : public ActionsDemo
{
public:
    static Issue1327* create()
    {
        auto ret = new Issue1327;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;
    void logSprRotation(cocos2d::Sprite* sender);
};

class Issue1398 : public ActionsDemo
{
public:
    static Issue1398* create()
    {
        auto ret = new Issue1398;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void incrementInteger();
    void incrementIntegerCallback(void* data);
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;
private:
    int _testInteger;
};

class Issue2599 : public ActionsDemo
{
public:
    static Issue2599* create()
    {
        auto ret = new Issue2599;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;
private:
    int _count;
};

class ActionCatmullRom : public ActionsDemo
{
public:
    static ActionCatmullRom* create()
    {
        auto ret = new ActionCatmullRom;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~ActionCatmullRom();
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;
};

class ActionCardinalSpline : public ActionsDemo
{
public:
    static ActionCardinalSpline* create()
    {
        auto ret = new ActionCardinalSpline;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~ActionCardinalSpline();
    
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;
};

class PauseResumeActions : public ActionsDemo
{
public:
    static PauseResumeActions* create()
    {
        auto ret = new PauseResumeActions;
        ret->init();
        ret->autorelease();
        return ret;
    }

    PauseResumeActions();
    virtual ~PauseResumeActions();
    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    virtual std::string title() const override;    
private:
    std::vector<Node *> _pausedTargets;
};

class ActionResize : public ActionsDemo 
{
public:
    static ActionResize* create()
    {
        auto ret = new ActionResize;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class ActionFloatTest : public ActionsDemo
{
public:
    static ActionFloatTest* create()
    {
        auto ret = new ActionFloatTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

#endif
