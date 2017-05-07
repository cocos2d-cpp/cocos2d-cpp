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

#ifndef _ACTIONS__PROGRESS_TEST_H_
#define _ACTIONS__PROGRESS_TEST_H_

#include "../BaseTest.h"

DEFINE_TEST_SUITE(ActionsProgressTests);

class SpriteDemo : public TestCase
{
public:
    SpriteDemo(void);
    ~SpriteDemo(void);

    virtual std::string title() const override;
    virtual void onEnter() override;
};

class SpriteProgressToRadial : public SpriteDemo
{
public:
    static SpriteProgressToRadial* create()
    {
        auto ret = new SpriteProgressToRadial;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressToHorizontal : public SpriteDemo
{
public:
    static SpriteProgressToHorizontal* create()
    {
        auto ret = new SpriteProgressToHorizontal;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressToVertical : public SpriteDemo
{
public:
    static SpriteProgressToVertical* create()
    {
        auto ret = new SpriteProgressToVertical;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressToRadialMidpointChanged : public SpriteDemo
{
public:
    static SpriteProgressToRadialMidpointChanged* create()
    {
        auto ret = new SpriteProgressToRadialMidpointChanged;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressBarVarious : public SpriteDemo
{
public:
    static SpriteProgressBarVarious* create()
    {
        auto ret = new SpriteProgressBarVarious;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressBarTintAndFade : public SpriteDemo
{
public:
    static SpriteProgressBarTintAndFade* create()
    {
        auto ret = new SpriteProgressBarTintAndFade;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class SpriteProgressWithSpriteFrame : public SpriteDemo
{
public:
    static SpriteProgressWithSpriteFrame* create()
    {
        auto ret = new SpriteProgressWithSpriteFrame;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

#endif // _ACTIONS__PROGRESS_TEST_H_
