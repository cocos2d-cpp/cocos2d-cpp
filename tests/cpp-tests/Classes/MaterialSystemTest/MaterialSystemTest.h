/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.

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

#pragma once

#include "../BaseTest.h"

#include "renderer/CCRenderState.h"

namespace cocos2d {
class Sprite3D;
}

DEFINE_TEST_SUITE(MaterialSystemTest);

class MaterialSystemBaseTest : public TestCase
{
public:
    virtual std::string title() const override;
};

class Material_Sprite3DTest : public MaterialSystemBaseTest
{
public:
    static Material_Sprite3DTest* create()
    {
        auto ret = new Material_Sprite3DTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class Material_MultipleSprite3D : public MaterialSystemBaseTest
{
public:
    static Material_MultipleSprite3D* create()
    {
        auto ret = new Material_MultipleSprite3D;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class Material_2DEffects : public MaterialSystemBaseTest
{
public:
    static Material_2DEffects* create()
    {
        auto ret = new Material_2DEffects;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class EffectAutoBindingResolver;
class Material_AutoBindings : public MaterialSystemBaseTest
{
public:
    static Material_AutoBindings* create()
    {
        auto ret = new Material_AutoBindings;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Material_AutoBindings();
    virtual ~Material_AutoBindings();

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

private:
    EffectAutoBindingResolver *_resolver;
};

class Material_setTechnique : public MaterialSystemBaseTest
{
public:
    static Material_setTechnique* create()
    {
        auto ret = new Material_setTechnique;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

private:
    void changeMaterial(float dt);

    cocos2d::Sprite3D* _sprite;
    int _techniqueState;
};

class Material_clone : public MaterialSystemBaseTest
{
public:
    static Material_clone* create()
    {
        auto ret = new Material_clone;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
};

class Material_parsePerformance : public MaterialSystemBaseTest
{
public:
    static Material_parsePerformance* create()
    {
        auto ret = new Material_parsePerformance;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;
    void parsingTesting(unsigned int count);
protected:
    unsigned int _maxParsingCoumt;
};

class Material_invalidate : public MaterialSystemBaseTest
{
public:
    static Material_invalidate* create()
    {
        auto ret = new Material_invalidate;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

    cocos2d::CustomCommand _customCommand;
};

class Material_renderState : public MaterialSystemBaseTest
{
public:
    static Material_renderState* create()
    {
        auto ret = new Material_renderState;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string subtitle() const override;

    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

    cocos2d::RenderState::StateBlock _stateBlock;
    cocos2d::CustomCommand _customCommand;
};


