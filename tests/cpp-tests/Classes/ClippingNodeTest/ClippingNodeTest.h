#ifndef __CLIPPINGNODETEST_H__
#define __CLIPPINGNODETEST_H__

#include "../BaseTest.h"

#include "base/CCRef.h"
#include "renderer/CCCustomCommand.h"

#include <list>

namespace cocos2d {
class ClippingNode;
class DrawNode;
class Sprite;
}

DEFINE_TEST_SUITE(ClippingNodeTests);

class BaseClippingNodeTest : public TestCase
{
public:
    static BaseClippingNodeTest* create()
    {
        auto ret = new BaseClippingNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~BaseClippingNodeTest();
    virtual bool init() override;

	virtual std::string title() const override;
    virtual void setup();
};

class BasicTest : public BaseClippingNodeTest
{
public:
    static BasicTest* create()
    {
        auto ret = new BasicTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup() override;

	virtual std::unique_ptr<cocos2d::Action> actionRotate();
	virtual std::unique_ptr<cocos2d::Action> actionScale();

	virtual cocos2d::DrawNode* shape();
	virtual cocos2d::Sprite* grossini();

	virtual cocos2d::Node* stencil();
	virtual cocos2d::ClippingNode* clipper();
	virtual cocos2d::Node* content();
};

class ShapeTest : public BasicTest
{
public:
    static ShapeTest* create()
    {
        auto ret = new ShapeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual cocos2d::Node* stencil() override;
    virtual cocos2d::Node* content() override;
};

class ShapeInvertedTest : public ShapeTest
{
public:
    static ShapeInvertedTest* create()
    {
        auto ret = new ShapeInvertedTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual cocos2d::ClippingNode* clipper() override;
};

class SpriteTest : public BasicTest
{
public:
    static SpriteTest* create()
    {
        auto ret = new SpriteTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual cocos2d::Node* stencil() override;
    virtual cocos2d::ClippingNode* clipper() override;
    virtual cocos2d::Node* content() override;
};

class SpriteNoAlphaTest : public SpriteTest
{
public:
    static SpriteNoAlphaTest* create()
    {
        auto ret = new SpriteNoAlphaTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual cocos2d::ClippingNode* clipper() override;
};

class SpriteInvertedTest : public SpriteTest
{
public:
    static SpriteInvertedTest* create()
    {
        auto ret = new SpriteInvertedTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual cocos2d::ClippingNode* clipper() override;
};

class NestedTest : public BaseClippingNodeTest
{
public:
    static NestedTest* create()
    {
        auto ret = new NestedTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup() override;
};

class HoleDemo : public BaseClippingNodeTest
{
public:
    static HoleDemo* create()
    {
        auto ret = new HoleDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~HoleDemo();
    virtual void setup() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void pokeHoleAtPoint(cocos2d::Vec2 point);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
	cocos2d::ClippingNode* _outerClipper;
    cocos2d::Node* _holes;
    cocos2d::Node* _holesStencil;
};

class ScrollViewDemo : public BaseClippingNodeTest
{
public:
    static ScrollViewDemo* create()
    {
        auto ret = new ScrollViewDemo;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup() override;
	void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
private:
	bool _scrolling;
    cocos2d::Vec2 _lastPoint;
};

//#if COCOS2D_DEBUG > 1

class RawStencilBufferTest : public BaseClippingNodeTest
{
public:
    static RawStencilBufferTest* create()
    {
        auto ret = new RawStencilBufferTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    ~RawStencilBufferTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup() override;
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;

	virtual void setupStencilForClippingOnPlane(GLint plane);
	virtual void setupStencilForDrawingOnPlane(GLint plane);

protected:
    std::list<cocos2d::CustomCommand> _renderCmds;
    void onEnableStencil();
    void onDisableStencil();
    void onBeforeDrawClip(int planeIndex, const cocos2d::Vec2& pt);
    void onBeforeDrawSprite(int planeIndex, const cocos2d::Vec2& pt);

protected:
    using container_type = std::vector<cocos2d::node_ptr<cocos2d::Sprite>>;

    container_type _sprites;
    container_type _spritesStencil;
};

class RawStencilBufferTest2 : public RawStencilBufferTest
{
public:
    static RawStencilBufferTest2* create()
    {
        auto ret = new RawStencilBufferTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void setupStencilForClippingOnPlane(GLint plane) override;
    virtual void setupStencilForDrawingOnPlane(GLint plane) override;
};

class RawStencilBufferTest3 : public RawStencilBufferTest
{
public:
    static RawStencilBufferTest3* create()
    {
        auto ret = new RawStencilBufferTest3;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void setupStencilForClippingOnPlane(GLint plane) override;
    virtual void setupStencilForDrawingOnPlane(GLint plane) override;
};

class RawStencilBufferTestAlphaTest : public RawStencilBufferTest
{
public:
    virtual void setup() override;
};

class RawStencilBufferTest4 : public RawStencilBufferTestAlphaTest
{
public:
    static RawStencilBufferTest4* create()
    {
        auto ret = new RawStencilBufferTest4;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void setupStencilForClippingOnPlane(GLint plane) override;
    virtual void setupStencilForDrawingOnPlane(GLint plane) override;
};

class RawStencilBufferTest5 : public RawStencilBufferTestAlphaTest
{
public:
    static RawStencilBufferTest5* create()
    {
        auto ret = new RawStencilBufferTest5;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string subtitle() const override;
    virtual void setupStencilForClippingOnPlane(GLint plane) override;
    virtual void setupStencilForDrawingOnPlane(GLint plane) override;
};

class RawStencilBufferTest6 : public RawStencilBufferTestAlphaTest
{
public:
    static RawStencilBufferTest6* create()
    {
        auto ret = new RawStencilBufferTest6;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void setupStencilForClippingOnPlane(GLint plane) override;
    virtual void setupStencilForDrawingOnPlane(GLint plane) override;

    // override
    virtual void setup() override;
    virtual std::string subtitle() const override;
};

//#endif //COCOS2D_DEBUG > 1

class ClippingToRenderTextureTest : public BaseClippingNodeTest
{
public:
    static ClippingToRenderTextureTest* create()
    {
        auto ret = new ClippingToRenderTextureTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    void expectedBehaviour();
    void reproduceBug();

    // override
    virtual void setup() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class ClippingRectangleNodeTest : public BaseClippingNodeTest
{
public:
    static ClippingRectangleNodeTest* create()
    {
        auto ret = new ClippingRectangleNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void setup() override;
};

#endif //__CLIPPINGNODETEST_H__
