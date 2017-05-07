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

#ifndef _ACTIONS__EASE_TEST_H_
#define _ACTIONS__EASE_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(ActionsEaseTests);

class EaseSpriteDemo : public TestCase
{
protected:
    cocos2d::Sprite*    _grossini;
    cocos2d::Sprite*    _tamara;
    cocos2d::Sprite*    _kathia;

    std::string    _title;

public:
    EaseSpriteDemo(void);
    ~EaseSpriteDemo(void);

    virtual std::string subtitle() const override;
    virtual void onEnter() override;
    
    void centerSprites(unsigned int numberOfSprites);

    void positionForTwo();
};

class SpriteEase : public EaseSpriteDemo
{
public:
    static SpriteEase* create()
    {
        auto ret = new SpriteEase;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;

    void testStopAction(float dt);
};

class SpriteEaseInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseInOut* create()
    {
        auto ret = new SpriteEaseInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseExponential : public EaseSpriteDemo
{
public:
    static SpriteEaseExponential* create()
    {
        auto ret = new SpriteEaseExponential;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseExponentialInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseExponentialInOut* create()
    {
        auto ret = new SpriteEaseExponentialInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseSine : public EaseSpriteDemo
{
public:
    static SpriteEaseSine* create()
    {
        auto ret = new SpriteEaseSine;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseSineInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseSineInOut* create()
    {
        auto ret = new SpriteEaseSineInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseElastic : public EaseSpriteDemo
{
public:
    static SpriteEaseElastic* create()
    {
        auto ret = new SpriteEaseElastic;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseElasticInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseElasticInOut* create()
    {
        auto ret = new SpriteEaseElasticInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseBounce : public EaseSpriteDemo
{
public:
    static SpriteEaseBounce* create()
    {
        auto ret = new SpriteEaseBounce;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseBounceInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseBounceInOut* create()
    {
        auto ret = new SpriteEaseBounceInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseBack : public EaseSpriteDemo
{
public:
    static SpriteEaseBack* create()
    {
        auto ret = new SpriteEaseBack;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseBackInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseBackInOut* create()
    {
        auto ret = new SpriteEaseBackInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseBezier : public EaseSpriteDemo
{
public:
    static SpriteEaseBezier* create()
    {
        auto ret = new SpriteEaseBezier;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuadratic : public EaseSpriteDemo
{
public:
    static SpriteEaseQuadratic* create()
    {
        auto ret = new SpriteEaseQuadratic;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuadraticInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseQuadraticInOut* create()
    {
        auto ret = new SpriteEaseQuadraticInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuartic : public EaseSpriteDemo
{
public:
    static SpriteEaseQuartic* create()
    {
        auto ret = new SpriteEaseQuartic;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuarticInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseQuarticInOut* create()
    {
        auto ret = new SpriteEaseQuarticInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuintic : public EaseSpriteDemo
{
public:
    static SpriteEaseQuintic* create()
    {
        auto ret = new SpriteEaseQuintic;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseQuinticInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseQuinticInOut* create()
    {
        auto ret = new SpriteEaseQuinticInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};


class SpriteEaseCircle : public EaseSpriteDemo
{
public:
    static SpriteEaseCircle* create()
    {
        auto ret = new SpriteEaseCircle;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseCircleInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseCircleInOut* create()
    {
        auto ret = new SpriteEaseCircleInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseCubic : public EaseSpriteDemo
{
public:
    static SpriteEaseCubic* create()
    {
        auto ret = new SpriteEaseCubic;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteEaseCubicInOut : public EaseSpriteDemo
{
public:
    static SpriteEaseCubicInOut* create()
    {
        auto ret = new SpriteEaseCubicInOut;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string title() const override;
};
class SpeedTest : public EaseSpriteDemo
{
public:
    static SpeedTest* create()
    {
        auto ret = new SpeedTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void onEnter() override;
    virtual std::string subtitle() const override;

    void altertime(float dt);
};


#endif
