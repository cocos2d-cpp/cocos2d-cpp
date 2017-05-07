#ifndef _TILEMAP_TEST_H_
#define _TILEMAP_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(TileMapTests);

class TileDemo : public TestCase
{
public:
    TileDemo();
    virtual ~TileDemo();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onExit() override;

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class TileMapTest : public TileDemo
{
public:
    static TileMapTest* create()
    {
        auto ret = new TileMapTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TileMapTest();
    virtual std::string title() const override;
};

class TileMapEditTest : public TileDemo
{
public:
    static TileMapEditTest* create()
    {
        auto ret = new TileMapEditTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TileMapEditTest ();
    virtual std::string title() const override;

    void updateMap(float dt);
};

class TMXOrthoTest : public TileDemo
{
public:
    static TMXOrthoTest* create()
    {
        auto ret = new TMXOrthoTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest();
    virtual std::string title() const override;

    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXStaggeredTest : public TileDemo
{
public:
    static TMXStaggeredTest* create()
    {
        auto ret = new TMXStaggeredTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXStaggeredTest();
    virtual std::string title() const override;
    
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXOrthoTest2 : public TileDemo
{
public:
    static TMXOrthoTest2* create()
    {
        auto ret = new TMXOrthoTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest2();
    virtual std::string title() const override;
};

class TMXOrthoTest3 : public TileDemo
{
public:
    static TMXOrthoTest3* create()
    {
        auto ret = new TMXOrthoTest3;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest3();
    virtual std::string title() const override;
};

class TMXOrthoTest4 : public TileDemo
{
public:
    static TMXOrthoTest4* create()
    {
        auto ret = new TMXOrthoTest4;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest4();
    void removeSprite(float dt);
    virtual std::string title() const override;
};

class TMXReadWriteTest : public TileDemo
{
    unsigned int _gid;
    unsigned int _gid2;
public:
    static TMXReadWriteTest* create()
    {
        auto ret = new TMXReadWriteTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXReadWriteTest();
    virtual std::string title() const override;

    void removeSprite(Node* sender);
    void updateCol(float dt);
    void repaintWithGID(float dt);
    void removeTiles(float dt);
};

class TMXHexTest : public TileDemo
{
public:
    static TMXHexTest* create()
    {
        auto ret = new TMXHexTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexTest();
    virtual std::string title() const override;
};

class TMXIsoTest : public TileDemo
{
public:
    static TMXIsoTest* create()
    {
        auto ret = new TMXIsoTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTest();
    virtual std::string title() const override;
};

class TMXIsoTest1 : public TileDemo
{
public:
    static TMXIsoTest1* create()
    {
        auto ret = new TMXIsoTest1;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTest1();
    virtual std::string title() const override;
};

class TMXIsoTest2 : public TileDemo
{
public:
    static TMXIsoTest2* create()
    {
        auto ret = new TMXIsoTest2;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTest2();
    virtual std::string title() const override;
};

class TMXUncompressedTest : public TileDemo
{
public:
    static TMXUncompressedTest* create()
    {
        auto ret = new TMXUncompressedTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXUncompressedTest();
    virtual std::string title() const override;
};

class TMXTilesetTest : public TileDemo
{
public:
    static TMXTilesetTest* create()
    {
        auto ret = new TMXTilesetTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXTilesetTest();
    virtual std::string title() const override;
};

class TMXCvsFormatTest : public TileDemo
{
public:
    static TMXCvsFormatTest* create()
    {
        auto ret = new TMXCvsFormatTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXCvsFormatTest();
    virtual std::string title() const override;
};

class TMXOrthoObjectsTest : public TileDemo
{
public:
    static TMXOrthoObjectsTest* create()
    {
        auto ret = new TMXOrthoObjectsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoObjectsTest();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXIsoObjectsTest : public TileDemo
{
public:
    static TMXIsoObjectsTest* create()
    {
        auto ret = new TMXIsoObjectsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoObjectsTest();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXResizeTest : public TileDemo
{
public:
    static TMXResizeTest* create()
    {
        auto ret = new TMXResizeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXResizeTest();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXIsoZorder : public TileDemo
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXIsoZorder* create()
    {
        auto ret = new TMXIsoZorder;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoZorder();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onExit()override;

    ~TMXIsoZorder();
    void repositionSprite(float dt);
};

class TMXOrthoZorder : public TileDemo
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXOrthoZorder* create()
    {
        auto ret = new TMXOrthoZorder;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoZorder();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual ~TMXOrthoZorder();
    void repositionSprite(float dt);
};

class TMXIsoVertexZ : public TileDemo
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXIsoVertexZ* create()
    {
        auto ret = new TMXIsoVertexZ;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoVertexZ();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    ~TMXIsoVertexZ();
    void repositionSprite(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXOrthoVertexZ : public TileDemo
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXOrthoVertexZ* create()
    {
        auto ret = new TMXOrthoVertexZ;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoVertexZ();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    ~TMXOrthoVertexZ();
    void repositionSprite(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXIsoMoveLayer : public TileDemo
{
public:
    static TMXIsoMoveLayer* create()
    {
        auto ret = new TMXIsoMoveLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoMoveLayer();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXOrthoMoveLayer : public TileDemo
{
public:
    static TMXOrthoMoveLayer* create()
    {
        auto ret = new TMXOrthoMoveLayer;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoMoveLayer();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXTilePropertyTest : public TileDemo
{
public:
    static TMXTilePropertyTest* create()
    {
        auto ret = new TMXTilePropertyTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXTilePropertyTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXOrthoFlipTest : public TileDemo
{
public:
    static TMXOrthoFlipTest* create()
    {
        auto ret = new TMXOrthoFlipTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFlipTest();
    virtual std::string title() const override;
};

class TMXOrthoFlipRunTimeTest : public TileDemo
{
public:
    static TMXOrthoFlipRunTimeTest* create()
    {
        auto ret = new TMXOrthoFlipRunTimeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFlipRunTimeTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void flipIt(float dt);
};

class TMXOrthoFromXMLTest : public TileDemo
{
public:
    static TMXOrthoFromXMLTest* create()
    {
        auto ret = new TMXOrthoFromXMLTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFromXMLTest();
    virtual std::string title() const override;
};

class TMXOrthoXMLFormatTest : public TileDemo
{
public:
    static TMXOrthoXMLFormatTest* create()
    {
        auto ret = new TMXOrthoXMLFormatTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoXMLFormatTest();
    virtual std::string title() const override;
};

class TMXBug987 : public TileDemo
{
public:
    static TMXBug987* create()
    {
        auto ret = new TMXBug987;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXBug987();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXBug787 : public TileDemo
{
public:
    static TMXBug787* create()
    {
        auto ret = new TMXBug787;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXBug787();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXGIDObjectsTest : public TileDemo
{
public:
    static TMXGIDObjectsTest* create()
    {
        auto ret = new TMXGIDObjectsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXGIDObjectsTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;    
};

class TMXHexOddXTest : public TileDemo
{
public:
    static TMXHexOddXTest* create()
    {
        auto ret = new TMXHexOddXTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexOddXTest();
    virtual std::string title() const override;
};

class TMXHexOddYTest : public TileDemo
{
public:
    static TMXHexOddYTest* create()
    {
        auto ret = new TMXHexOddYTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexOddYTest();
    virtual std::string title() const override;
};

class TMXHexEvenXTest : public TileDemo
{
public:
    static TMXHexEvenXTest* create()
    {
        auto ret = new TMXHexEvenXTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexEvenXTest();
    virtual std::string title() const override;
};

class TMXHexEvenYTest : public TileDemo
{
public:
    static TMXHexEvenYTest* create()
    {
        auto ret = new TMXHexEvenYTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexEvenYTest();
    virtual std::string title() const override;
};

class TMXHexAxisXTest : public TileDemo
{
public:
    static TMXHexAxisXTest* create()
    {
        auto ret = new TMXHexAxisXTest;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexAxisXTest();
    virtual std::string title() const override;

};

class Issue16105Test : public TileDemo
{
public:
    static Issue16105Test* create()
    {
        auto ret = new Issue16105Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue16105Test();
    virtual std::string title() const override;

};

class Issue16512Test : public TileDemo
{
public:
    static Issue16512Test* create()
    {
        auto ret = new Issue16512Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    Issue16512Test();
    virtual std::string title() const override;

};

#endif
