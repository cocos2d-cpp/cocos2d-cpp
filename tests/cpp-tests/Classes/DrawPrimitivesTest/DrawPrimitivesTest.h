#ifndef _DRAW_PRIMITIVES_TEST_H_
#define _DRAW_PRIMITIVES_TEST_H_

#include "cocos2d.h"
#include "../BaseTest.h"

#include <string>

DEFINE_TEST_SUITE(DrawNodeTests);

class DrawNodeBaseTest : public TestCase
{
public:
    virtual std::string title() const override;
};

class SimpleDrawNodeTest : public DrawNodeBaseTest
{
public:
    CREATE_FUNC(SimpleDrawNodeTest);
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
    CREATE_FUNC(DrawNodeTest);

    DrawNodeTest();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class PrimitivesCommandTest : public DrawNodeBaseTest
{
public:
    CREATE_FUNC(PrimitivesCommandTest);

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
    CREATE_FUNC(Issue11942Test);

    Issue11942Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

};

#endif
