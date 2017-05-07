#ifndef __PERFORMANCE_NODE_CHILDREN_TEST_H__
#define __PERFORMANCE_NODE_CHILDREN_TEST_H__

#include "BaseTest.h"

DEFINE_TEST_SUITE(PerformceNodeChildrenTests);

class NodeChildrenMainScene : public TestCase
{
public:
    virtual bool init() override;
    virtual void initWithQuantityOfNodes(unsigned int nNodes);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void updateQuantityOfNodes() = 0;

    const char* profilerName();
    void updateProfilerName();

    // for the profiler
    virtual const char* testName() = 0;

    void updateQuantityLabel();

    void dumpProfilerInfo(float dt);

    virtual void onExitTransitionDidStart() override;
    virtual void onEnterTransitionDidFinish() override;
protected:
    NodeChildrenMainScene();
    static int quantityOfNodes;
    char   _profilerName[256];
    int    lastRenderedCount;
    int    currentQuantityOfNodes;
    size_t autoTestIndex;
};

class IterateSpriteSheet : public NodeChildrenMainScene
{
public:
    virtual ~IterateSpriteSheet();
    virtual void updateQuantityOfNodes();
    virtual void initWithQuantityOfNodes(unsigned int nNodes);
    virtual void update(float dt) = 0;
    virtual const char* testName();

protected:
    cocos2d::SpriteBatchNode    *batchNode;
};

class IterateSpriteSheetForLoop : public IterateSpriteSheet
{
public:
    static IterateSpriteSheetForLoop* create()
    {
        auto ret = new IterateSpriteSheetForLoop;
        ret->init();
        ret->autorelease();
        return ret;
    }

    IterateSpriteSheetForLoop() {}
    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName() override;
};

class IterateSpriteSheetIterator : public IterateSpriteSheet
{
public:
    static IterateSpriteSheetIterator* create()
    {
        auto ret = new IterateSpriteSheetIterator;
        ret->autorelease();
        return ret;
    }

    IterateSpriteSheetIterator() {}
    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class IterateSpriteSheetForEach : public IterateSpriteSheet
{
public:
    static IterateSpriteSheetForEach* create()
    {
        auto ret = new IterateSpriteSheetForEach;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};


class AddRemoveSpriteSheet : public NodeChildrenMainScene
{
public:
    virtual ~AddRemoveSpriteSheet();
    virtual void updateQuantityOfNodes()override;
    virtual void initWithQuantityOfNodes(unsigned int nNodes)override;
    virtual void update(float dt)override = 0 ;
    virtual const char* testName()override;

protected:
    cocos2d::SpriteBatchNode    *batchNode;

#if CC_ENABLE_PROFILERS
    ProfilingTimer* _profilingTimer;
#endif
};

///

class CallFuncsSpriteSheetForEach : public IterateSpriteSheet
{
public:
    static CallFuncsSpriteSheetForEach* create()
    {
        auto ret = new CallFuncsSpriteSheetForEach;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

///

class AddSprite : public AddRemoveSpriteSheet
{
public:
    static AddSprite* create()
    {
        auto ret = new AddSprite;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class AddSpriteSheet : public AddRemoveSpriteSheet
{
public:
    static AddSpriteSheet* create()
    {
        auto ret = new AddSpriteSheet;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class GetSpriteSheet : public AddRemoveSpriteSheet
{
public:
    static GetSpriteSheet* create()
    {
        auto ret = new GetSpriteSheet;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class RemoveSprite : public AddRemoveSpriteSheet
{
public:
    static RemoveSprite* create()
    {
        auto ret = new RemoveSprite;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class RemoveSpriteSheet : public AddRemoveSpriteSheet
{
public:
    static RemoveSpriteSheet* create()
    {
        auto ret = new RemoveSpriteSheet;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class ReorderSpriteSheet : public AddRemoveSpriteSheet
{
public:
    static ReorderSpriteSheet* create()
    {
        auto ret = new ReorderSpriteSheet;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class SortAllChildrenSpriteSheet : public AddRemoveSpriteSheet
{
public:
    static SortAllChildrenSpriteSheet* create()
    {
        auto ret = new SortAllChildrenSpriteSheet;
        ret->autorelease();
        return ret;
    }

    virtual void update(float dt) override;

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName()override;
};

class VisitSceneGraph : public NodeChildrenMainScene
{
public:
    static VisitSceneGraph* create()
    {
        auto ret = new VisitSceneGraph;
        ret->autorelease();
        return ret;
    }

    void initWithQuantityOfNodes(unsigned int nodes) override;

    virtual void update(float dt) override;
    void updateQuantityOfNodes() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual const char* testName() override;
};

#endif // __PERFORMANCE_NODE_CHILDREN_TEST_H__
