#ifndef _DRAW_PRIMITIVES_TEST_H_
#define _DRAW_PRIMITIVES_TEST_H_

#include "../BaseTest.h"

#include "renderer/CCPrimitiveCommand.h"

#include <string>

namespace cocos2d {
class Primitive;
class PrimitiveCommand;
}

DEFINE_TEST_SUITE(DrawNodeTests);

class DrawNodeBaseTest : public TestCase
{
public:
    virtual std::string title() const override;
};

class SimpleDrawNodeTest : public DrawNodeBaseTest
{
public:
    static SimpleDrawNodeTest* create()
    {
        auto ret = new SimpleDrawNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SimpleDrawNodeTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;

protected:
    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
    cocos2d::CustomCommand _customCommand;
};

class DrawNodeTest : public DrawNodeBaseTest
{
public:
    static DrawNodeTest* create()
    {
        auto ret = new DrawNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    DrawNodeTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PrimitivesCommandTest : public DrawNodeBaseTest
{
public:
    static PrimitivesCommandTest* create()
    {
        auto ret = new PrimitivesCommandTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    PrimitivesCommandTest();
    virtual ~PrimitivesCommandTest();

    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    cocos2d::Texture2D* _texture;
    cocos2d::GLProgramState* _programState;
    std::unique_ptr<cocos2d::Primitive> _primitive;

    cocos2d::PrimitiveCommand _primitiveCommand;
};

class Issue11942Test : public DrawNodeBaseTest
{
public:
    static Issue11942Test* create()
    {
        auto ret = new Issue11942Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Issue11942Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

};

#endif
