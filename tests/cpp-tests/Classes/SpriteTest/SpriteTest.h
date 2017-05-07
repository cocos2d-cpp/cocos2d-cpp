/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org

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

#ifndef _SPRITE_TEST_H_
#define _SPRITE_TEST_H_

#include "../BaseTest.h"

#include "base/CCEvent.h"
#include "base/CCTouch.h"
#include "math/Vec2.h"

#include <string>

namespace cocos2d {
class Sprite;
class SpriteBatchNode;
class Texture2D;
}

DEFINE_TEST_SUITE(SpriteTests);

class SpriteTestDemo : public TestCase
{
protected:
    std::string    _title;

public:
};

class Sprite1 : public SpriteTestDemo
{
public:
    static Sprite1* create()
    {
        auto ret = new Sprite1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite1();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class Sprite1ETC1Alpha : public SpriteTestDemo
{
public:
    static Sprite1ETC1Alpha* create()
    {
        auto ret = new Sprite1ETC1Alpha;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite1ETC1Alpha();
    bool init() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

    cocos2d::Sprite* _background;
};

class SpriteBatchNode1: public SpriteTestDemo
{
public:
    static SpriteBatchNode1* create()
    {
        auto ret = new SpriteBatchNode1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNode1();
    void addNewSpriteWithCoords(cocos2d::Vec2 p);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteColorOpacity : public SpriteTestDemo
{
public:
    static SpriteColorOpacity* create()
    {
        auto ret = new SpriteColorOpacity;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteColorOpacity();
    void removeAndAddSprite(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

};

class SpriteBatchNodeColorOpacity : public SpriteTestDemo
{
public:
    static SpriteBatchNodeColorOpacity* create()
    {
        auto ret = new SpriteBatchNodeColorOpacity;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeColorOpacity();
    void removeAndAddSprite(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteZOrder : public SpriteTestDemo
{
public:
    static SpriteZOrder* create()
    {
        auto ret = new SpriteZOrder;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteZOrder();
    void reorderSprite(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    int _dir;
};

class SpriteBatchNodeZOrder: public SpriteTestDemo
{
public:
    static SpriteBatchNodeZOrder* create()
    {
        auto ret = new SpriteBatchNodeZOrder;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeZOrder();
    void reorderSprite(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    int _dir;
};

class SpriteBatchNodeReorder : public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorder* create()
    {
        auto ret = new SpriteBatchNodeReorder;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeReorder();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeReorderIssue744: public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorderIssue744* create()
    {
        auto ret = new SpriteBatchNodeReorderIssue744;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeReorderIssue744();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeReorderIssue766 : public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorderIssue766* create()
    {
        auto ret = new SpriteBatchNodeReorderIssue766;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeReorderIssue766();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void reorderSprite(float dt);
    cocos2d::Sprite* makeSpriteZ(int aZ);

private:
    cocos2d::SpriteBatchNode* batchNode;
    cocos2d::Sprite* sprite1;
    cocos2d::Sprite* sprite2;
    cocos2d::Sprite* sprite3;    
};

class SpriteBatchNodeReorderIssue767 : public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorderIssue767* create()
    {
        auto ret = new SpriteBatchNodeReorderIssue767;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeReorderIssue767();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void reorderSprites(float dt);
};

class SpriteZVertex: public SpriteTestDemo
{
public:
    static SpriteZVertex* create()
    {
        auto ret = new SpriteZVertex;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    SpriteZVertex();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    int _dir;
    float _time;
};

class SpriteBatchNodeZVertex: public SpriteTestDemo
{
public:
    static SpriteBatchNodeZVertex* create()
    {
        auto ret = new SpriteBatchNodeZVertex;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    SpriteBatchNodeZVertex();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    int _dir;
    float _time;
};

class SpriteAnchorPoint : public SpriteTestDemo
{
public:
    static SpriteAnchorPoint* create()
    {
        auto ret = new SpriteAnchorPoint;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteAnchorPoint();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeAnchorPoint : public SpriteTestDemo
{
public:
    static SpriteBatchNodeAnchorPoint* create()
    {
        auto ret = new SpriteBatchNodeAnchorPoint;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteBatchNodeAnchorPoint();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteAnchorPointFromFile : public SpriteTestDemo
{
public:
    static SpriteAnchorPointFromFile* create()
    {
        auto ret = new SpriteAnchorPointFromFile;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Sprite6 : public SpriteTestDemo
{
public:
    static Sprite6* create()
    {
        auto ret = new Sprite6;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Sprite6();
    virtual std::string title() const override;
};

class SpriteFlip : public SpriteTestDemo
{
public:
    static SpriteFlip* create()
    {
        auto ret = new SpriteFlip;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteFlip();
    void flipSprites(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeFlip : public SpriteTestDemo
{
public:
    static SpriteBatchNodeFlip* create()
    {
        auto ret = new SpriteBatchNodeFlip;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeFlip();
    void flipSprites(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteAliased : public SpriteTestDemo
{
public:
    static SpriteAliased* create()
    {
        auto ret = new SpriteAliased;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteAliased();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeAliased : public SpriteTestDemo
{
public:
    static SpriteBatchNodeAliased* create()
    {
        auto ret = new SpriteBatchNodeAliased;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeAliased();
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteNewTexture : public SpriteTestDemo
{
public:
    static SpriteNewTexture* create()
    {
        auto ret = new SpriteNewTexture;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteNewTexture();
    virtual ~SpriteNewTexture();
    void addNewSprite();
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    bool _usingTexture1;
    cocos2d::Texture2D* _texture1;
    cocos2d::Texture2D* _texture2;
};

class SpriteBatchNodeNewTexture : public SpriteTestDemo
{
public:
    static SpriteBatchNodeNewTexture* create()
    {
        auto ret = new SpriteBatchNodeNewTexture;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeNewTexture();
    virtual ~SpriteBatchNodeNewTexture();
    void addNewSprite();
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

protected:
    const cocos2d::Texture2D *_texture1;
    const cocos2d::Texture2D *_texture2;
};

class SpriteFrameTest: public SpriteTestDemo
{
public:
    static SpriteFrameTest* create()
    {
        auto ret = new SpriteFrameTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void startIn05Secs(float dt);
    void flipSprites(float dt);
private:
    
    cocos2d::Sprite* _sprite1;
    cocos2d::Sprite* _sprite2;
    int      _counter;
};

class SpriteFrameAliasNameTest : public SpriteTestDemo
{
public:
    static SpriteFrameAliasNameTest* create()
    {
        auto ret = new SpriteFrameAliasNameTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteFramesFromFileContent : public SpriteTestDemo
{
public:
    static SpriteFramesFromFileContent* create()
    {
        auto ret = new SpriteFramesFromFileContent;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual std::string sheetName() const;
};

class SpritePolygonFromFileContent : public SpriteFramesFromFileContent
{
public:
    static SpritePolygonFromFileContent* create()
    {
        auto ret = new SpritePolygonFromFileContent;
        ret->init();
        ret->autorelease();
        return ret;
    }
    virtual std::string subtitle() const override;
    virtual std::string sheetName() const override;
};

class SpriteOffsetAnchorRotation: public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorRotation* create()
    {
        auto ret = new SpriteOffsetAnchorRotation;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorRotation: public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorRotation* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorRotation;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorRotation();
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorScale: public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorScale* create()
    {
        auto ret = new SpriteOffsetAnchorScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorScale();
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorScale: public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorScale* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorScale();
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorSkew : public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorSkew* create()
    {
        auto ret = new SpriteOffsetAnchorSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorSkew();
    virtual ~SpriteOffsetAnchorSkew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorRotationalSkew : public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorRotationalSkew* create()
    {
        auto ret = new SpriteOffsetAnchorRotationalSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorRotationalSkew();
    virtual ~SpriteOffsetAnchorRotationalSkew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorSkew : public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorSkew* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorSkew();
    virtual ~SpriteBatchNodeOffsetAnchorSkew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorRotationalSkewScale : public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorRotationalSkewScale* create()
    {
        auto ret = new SpriteOffsetAnchorRotationalSkewScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorRotationalSkewScale();
    virtual ~SpriteOffsetAnchorRotationalSkewScale();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorRotationalSkew : public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorRotationalSkew* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorRotationalSkew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorRotationalSkew();
    virtual ~SpriteBatchNodeOffsetAnchorRotationalSkew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorSkewScale : public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorSkewScale* create()
    {
        auto ret = new SpriteOffsetAnchorSkewScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorSkewScale();
    virtual ~SpriteOffsetAnchorSkewScale();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorSkewScale : public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorSkewScale* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorSkewScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorSkewScale();
    virtual ~SpriteBatchNodeOffsetAnchorSkewScale();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorRotationalSkewScale : public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorRotationalSkewScale* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorRotationalSkewScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorRotationalSkewScale();
    virtual ~SpriteBatchNodeOffsetAnchorRotationalSkewScale();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteOffsetAnchorFlip : public SpriteTestDemo
{
public:
    static SpriteOffsetAnchorFlip* create()
    {
        auto ret = new SpriteOffsetAnchorFlip;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteOffsetAnchorFlip();
    virtual ~SpriteOffsetAnchorFlip();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeOffsetAnchorFlip : public SpriteTestDemo
{
public:
    static SpriteBatchNodeOffsetAnchorFlip* create()
    {
        auto ret = new SpriteBatchNodeOffsetAnchorFlip;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeOffsetAnchorFlip();
    virtual ~SpriteBatchNodeOffsetAnchorFlip();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteAnimationSplit : public SpriteTestDemo
{
public:
    static SpriteAnimationSplit* create()
    {
        auto ret = new SpriteAnimationSplit;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteAnimationSplit();
    virtual void onExit() override;
    virtual std::string title() const override;
};

class SpriteHybrid: public SpriteTestDemo
{
public:
    static SpriteHybrid* create()
    {
        auto ret = new SpriteHybrid;
        ret->init();
        ret->autorelease();
        return ret;
    }

    SpriteHybrid();
    void reparentSprite(float dt);
    virtual std::string title() const override;
    virtual void onExit() override;

protected:
    bool     _usingSpriteBatchNode;
};

class SpriteBatchNodeChildren: public SpriteTestDemo
{
public:
    static SpriteBatchNodeChildren* create()
    {
        auto ret = new SpriteBatchNodeChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeChildren();
    virtual void onExit() override;
    virtual std::string title() const override;
};

class SpriteBatchNodeChildrenZ : public SpriteTestDemo
{
public:
    static SpriteBatchNodeChildrenZ* create()
    {
        auto ret = new SpriteBatchNodeChildrenZ;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeChildrenZ();
    virtual void onExit() override;
    virtual std::string title() const override;
};

class SpriteChildrenVisibility: public SpriteTestDemo
{
public:
    static SpriteChildrenVisibility* create()
    {
        auto ret = new SpriteChildrenVisibility;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteChildrenVisibility();
    virtual void onExit() override;
    virtual std::string title() const override;
};

class SpriteChildrenVisibilityIssue665 : public SpriteTestDemo
{
public:
    static SpriteChildrenVisibilityIssue665* create()
    {
        auto ret = new SpriteChildrenVisibilityIssue665;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteChildrenVisibilityIssue665();
    virtual ~SpriteChildrenVisibilityIssue665();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteChildrenAnchorPoint: public SpriteTestDemo
{
public:
    static SpriteChildrenAnchorPoint* create()
    {
        auto ret = new SpriteChildrenAnchorPoint;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteChildrenAnchorPoint();
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeChildrenAnchorPoint: public SpriteTestDemo
{
public:
    static SpriteBatchNodeChildrenAnchorPoint* create()
    {
        auto ret = new SpriteBatchNodeChildrenAnchorPoint;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeChildrenAnchorPoint();
    virtual void onExit() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeChildrenScale: public SpriteTestDemo
{
public:
    static SpriteBatchNodeChildrenScale* create()
    {
        auto ret = new SpriteBatchNodeChildrenScale;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeChildrenScale();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteChildrenChildren: public SpriteTestDemo
{
public:
    static SpriteChildrenChildren* create()
    {
        auto ret = new SpriteChildrenChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteChildrenChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


class SpriteBatchNodeChildrenChildren: public SpriteTestDemo
{
public:
    static SpriteBatchNodeChildrenChildren* create()
    {
        auto ret = new SpriteBatchNodeChildrenChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeChildrenChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteNilTexture: public SpriteTestDemo
{
public:
    static SpriteNilTexture* create()
    {
        auto ret = new SpriteNilTexture;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteNilTexture();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteSubclass : public SpriteTestDemo
{
public:
    static SpriteSubclass* create()
    {
        auto ret = new SpriteSubclass;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSubclass();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class AnimationCacheTest : public SpriteTestDemo
{
public:
    static AnimationCacheTest* create()
    {
        auto ret = new AnimationCacheTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    AnimationCacheTest();
    virtual ~AnimationCacheTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class NodeSort : public SpriteTestDemo
{
public:
    static NodeSort* create()
    {
        auto ret = new NodeSort;
        ret->init();
        ret->autorelease();
        return ret;
    }
    NodeSort();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void reorderSprite(float dt);

private:
    cocos2d::Node* _node;
    cocos2d::Sprite* _sprite1;
    cocos2d::Sprite* _sprite2;
    cocos2d::Sprite* _sprite3;
    cocos2d::Sprite* _sprite4;
    cocos2d::Sprite* _sprite5;
};

class SpriteBatchNodeReorderSameIndex : public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorderSameIndex* create()
    {
        auto ret = new SpriteBatchNodeReorderSameIndex;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeReorderSameIndex();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void reorderSprite(float dt);

private:
    cocos2d::SpriteBatchNode* _batchNode;
    cocos2d::Sprite* _sprite1;
    cocos2d::Sprite* _sprite2;
    cocos2d::Sprite* _sprite3;
    cocos2d::Sprite* _sprite4;
    cocos2d::Sprite* _sprite5;
};

class SpriteBatchNodeReorderOneChild : public SpriteTestDemo
{
public:
    static SpriteBatchNodeReorderOneChild* create()
    {
        auto ret = new SpriteBatchNodeReorderOneChild;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeReorderOneChild();
    void reorderSprite(float dt);
    virtual std::string title() const override;
private:
    cocos2d::SpriteBatchNode* _batchNode;
    cocos2d::Sprite* _reorderSprite;
};

class SpriteBatchNodeSkewNegativeScaleChildren : public SpriteTestDemo
{
public:
    static SpriteBatchNodeSkewNegativeScaleChildren* create()
    {
        auto ret = new SpriteBatchNodeSkewNegativeScaleChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeSkewNegativeScaleChildren();
    virtual ~SpriteBatchNodeSkewNegativeScaleChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchNodeRotationalSkewNegativeScaleChildren : public SpriteTestDemo
{
public:
    static SpriteBatchNodeRotationalSkewNegativeScaleChildren* create()
    {
        auto ret = new SpriteBatchNodeRotationalSkewNegativeScaleChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchNodeRotationalSkewNegativeScaleChildren();
    virtual ~SpriteBatchNodeRotationalSkewNegativeScaleChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteSkewNegativeScaleChildren : public SpriteTestDemo
{
public:
    static SpriteSkewNegativeScaleChildren* create()
    {
        auto ret = new SpriteSkewNegativeScaleChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSkewNegativeScaleChildren();
    virtual ~SpriteSkewNegativeScaleChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteRotationalSkewNegativeScaleChildren : public SpriteTestDemo
{
public:
    static SpriteRotationalSkewNegativeScaleChildren* create()
    {
        auto ret = new SpriteRotationalSkewNegativeScaleChildren;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteRotationalSkewNegativeScaleChildren();
    virtual ~SpriteRotationalSkewNegativeScaleChildren();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteDoubleResolution : public SpriteTestDemo
{
public:
    static SpriteDoubleResolution* create()
    {
        auto ret = new SpriteDoubleResolution;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteDoubleResolution();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class AnimationCacheFile : public SpriteTestDemo
{
public:
    static AnimationCacheFile* create()
    {
        auto ret = new AnimationCacheFile;
        ret->init();
        ret->autorelease();
        return ret;
    }

    AnimationCacheFile();
    virtual ~AnimationCacheFile();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteBatchBug1217 : public SpriteTestDemo
{
public:
    static SpriteBatchBug1217* create()
    {
        auto ret = new SpriteBatchBug1217;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteBatchBug1217();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteCullTest1 : public SpriteTestDemo
{
public:
    static SpriteCullTest1* create()
    {
        auto ret = new SpriteCullTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteCullTest1();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class SpriteCullTest2 : public SpriteTestDemo
{
public:
    static SpriteCullTest2* create()
    {
        auto ret = new SpriteCullTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteCullTest2();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Sprite3DRotationTest : public SpriteTestDemo
{
public:
    static Sprite3DRotationTest* create()
    {
        auto ret = new Sprite3DRotationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Sprite3DRotationTest();
    virtual std::string title() const override { return "3D Rotation Test"; };
    virtual std::string subtitle() const override { return "Rotation should based on the anchor point"; };
    
protected:
    cocos2d::Sprite* sprite1;
    cocos2d::Sprite* sprite2;
    
    cocos2d::Vec3 rotation;
};

class SpriteSlice9Test1 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test1* create()
    {
        auto ret = new SpriteSlice9Test1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test1();
    virtual std::string title() const override { return "Slice 9 Test #1"; };
    virtual std::string subtitle() const override { return "Testing Sprite::create()"; };
};

class SpriteSlice9Test2 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test2* create()
    {
        auto ret = new SpriteSlice9Test2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test2();
    virtual std::string title() const override { return "Slice 9 Test #2"; };
    virtual std::string subtitle() const override { return "Testing Sprite::createName()"; }
};

class SpriteSlice9Test3 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test3* create()
    {
        auto ret = new SpriteSlice9Test3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test3();
    virtual std::string title() const override { return "Slice 9 Test #3"; };
    virtual std::string subtitle() const override { return "Testing scaling a button"; }
};

class SpriteSlice9Test4 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test4* create()
    {
        auto ret = new SpriteSlice9Test4;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test4();
    virtual std::string title() const override { return "Slice 9 Test #4"; };
    virtual std::string subtitle() const override { return "Testing scaling a button"; }
};

class SpriteSlice9Test5 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test5* create()
    {
        auto ret = new SpriteSlice9Test5;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test5();
    virtual std::string title() const override { return "Slice 9 Test #5"; };
    virtual std::string subtitle() const override { return "Changing setCenterRect in runtime"; }

    virtual void update(float dt) override;

private:
    cocos2d::Sprite* _sprites[3];
    float _elapsed;
};

class SpriteSlice9Test6 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test6* create()
    {
        auto ret = new SpriteSlice9Test6;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test6();
    virtual std::string title() const override { return "Slice 9 Test #6"; };
    virtual std::string subtitle() const override { return "Changing setCenterRect in runtime #2"; }

    virtual void update(float dt) override;

private:
    cocos2d::Sprite* _sprites[3];
    float _elapsed;
};

class SpriteSlice9Test7 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test7* create()
    {
        auto ret = new SpriteSlice9Test7;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test7();
    virtual std::string title() const override { return "Slice 9 Test #7"; };
    virtual std::string subtitle() const override { return "Offset"; }
};

class SpriteSlice9Test8 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test8* create()
    {
        auto ret = new SpriteSlice9Test8;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test8();
    virtual std::string title() const override { return "Slice 9 Test #8"; };
    virtual std::string subtitle() const override { return "Flipped sprites"; }
};

class SpriteSlice9Test9 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test9* create()
    {
        auto ret = new SpriteSlice9Test9;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test9();
    virtual std::string title() const override { return "Slice 9 Test #9"; };
    virtual std::string subtitle() const override { return "Flipped sprites #2"; }
};


class SpriteSlice9Test10 : public SpriteTestDemo
{
public:
    static SpriteSlice9Test10* create()
    {
        auto ret = new SpriteSlice9Test10;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteSlice9Test10();
    virtual std::string title() const override { return "Slice 10 Test #9"; };
    virtual std::string subtitle() const override { return "Rotated Sprites"; }
};


class SpriteGetSpriteFrameTest : public SpriteTestDemo
{
public:
    static SpriteGetSpriteFrameTest* create()
    {
        auto ret = new SpriteGetSpriteFrameTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    SpriteGetSpriteFrameTest();
    virtual std::string title() const override { return "Sprite::getSpriteFrame(*)"; };
    virtual std::string subtitle() const override { return "setting after getting should be the same"; }
};

#endif
