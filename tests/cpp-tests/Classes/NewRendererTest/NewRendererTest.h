#ifndef __NewRendererTest_H_
#define __NewRendererTest_H_

#include "../BaseTest.h"

#define kTagSpriteBatchNode 100
#define kTagClipperNode     101
#define kTagContentNode     102

DEFINE_TEST_SUITE(NewRendererTests);

class MultiSceneTest : public TestCase
{
public:
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:

};

class NewSpriteTest : public MultiSceneTest
{
public:
    static NewSpriteTest* create()
    {
        auto ret = new NewSpriteTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void createSpriteTest();
    void createNewSpriteTest();
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

protected:
    NewSpriteTest();
    virtual ~NewSpriteTest();
};

class GroupCommandTest : public MultiSceneTest
{
public:
    static GroupCommandTest* create()
    {
        auto ret = new GroupCommandTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    GroupCommandTest();
    virtual ~GroupCommandTest();
};

class NewClippingNodeTest : public MultiSceneTest
{
public:

    static NewClippingNodeTest* create()
    {
        auto ret = new NewClippingNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);

protected:
    NewClippingNodeTest();
    virtual ~NewClippingNodeTest();

    bool _scrolling;
    cocos2d::Vec2 _lastPoint;
};

class NewDrawNodeTest : public MultiSceneTest
{
public:

    static NewDrawNodeTest* create()
    {
        auto ret = new NewDrawNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    NewDrawNodeTest();
    virtual ~NewDrawNodeTest();
};

class NewCullingTest : public MultiSceneTest
{
public:
    
    static NewCullingTest* create()
    {
        auto ret = new NewCullingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    NewCullingTest();
    virtual ~NewCullingTest();
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event);
    void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event  *event);
    cocos2d::Vec2 _lastPos;
};

class VBOFullTest : public MultiSceneTest
{
public:
    static VBOFullTest* create()
    {
        auto ret = new VBOFullTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
protected:
    VBOFullTest();
    virtual ~VBOFullTest();
};

class CaptureScreenTest : public MultiSceneTest
{
    static const int childTag = 119;
public:
    static CaptureScreenTest* create()
    {
        auto ret = new CaptureScreenTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    CaptureScreenTest();
    ~CaptureScreenTest();

    void onCaptured(cocos2d::Ref*);
    void afterCaptured(bool succeed, const std::string& outputFile);

    std::string _filename;
};

class CaptureNodeTest : public MultiSceneTest
{
    static const int childTag = 120;
public:
    static CaptureNodeTest* create()
    {
        auto ret = new CaptureNodeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    CaptureNodeTest();
    ~CaptureNodeTest();

    void onCaptured(cocos2d::Ref*);

    std::string _filename;
};

class BugAutoCulling : public MultiSceneTest
{
public:
    static BugAutoCulling* create()
    {
        auto ret = new BugAutoCulling;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
protected:
    BugAutoCulling();
};

class RendererBatchQuadTri : public MultiSceneTest
{
public:
    static RendererBatchQuadTri* create()
    {
        auto ret = new RendererBatchQuadTri;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
protected:
    RendererBatchQuadTri();
};

class RendererUniformBatch : public MultiSceneTest
{
public:
    static RendererUniformBatch* create()
    {
        auto ret = new RendererUniformBatch;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
protected:
    RendererUniformBatch();

    cocos2d::GLProgramState* createBlurGLProgramState();
    cocos2d::GLProgramState* createSepiaGLProgramState();
};

class RendererUniformBatch2 : public MultiSceneTest
{
public:
    static RendererUniformBatch2* create()
    {
        auto ret = new RendererUniformBatch2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
protected:
    RendererUniformBatch2();

    cocos2d::GLProgramState* createBlurGLProgramState();
    cocos2d::GLProgramState* createSepiaGLProgramState();
};

#endif //__NewRendererTest_H_
