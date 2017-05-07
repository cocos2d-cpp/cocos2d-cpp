#ifndef _TILEMAP_TEST_NEW_H_
#define _TILEMAP_TEST_NEW_H_

#include "../BaseTest.h"

namespace cocos2d {
class Sprite;
}

DEFINE_TEST_SUITE(FastTileMapTests);

class TileDemoNew : public TestCase
{
public:
    TileDemoNew();
    virtual ~TileDemoNew();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onExit() override;

    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
};

class TileMapTestNew : public TileDemoNew
{
public:
    static TileMapTestNew* create()
    {
        auto ret = new TileMapTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TileMapTestNew();
    virtual std::string title() const override;
};

class TileMapEditTestNew : public TileDemoNew
{
public:
    static TileMapEditTestNew* create()
    {
        auto ret = new TileMapEditTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TileMapEditTestNew ();
    virtual std::string title() const override;

    void updateMap(float dt);
};

class TMXOrthoTestNew : public TileDemoNew
{
public:
    static TMXOrthoTestNew* create()
    {
        auto ret = new TMXOrthoTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTestNew();
    virtual std::string title() const override;

    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXOrthoTest2New : public TileDemoNew
{
public:
    static TMXOrthoTest2New* create()
    {
        auto ret = new TMXOrthoTest2New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest2New();
    virtual std::string title() const override;
};

class TMXOrthoTest3New : public TileDemoNew
{
public:
    static TMXOrthoTest3New* create()
    {
        auto ret = new TMXOrthoTest3New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest3New();
    virtual std::string title() const override;
};

class TMXOrthoTest4New : public TileDemoNew
{
public:
    static TMXOrthoTest4New* create()
    {
        auto ret = new TMXOrthoTest4New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoTest4New();
    void removeSprite(float dt);
    virtual std::string title() const override;
};

class TMXReadWriteTestNew : public TileDemoNew
{
    unsigned int _gid;
    unsigned int _gid2;
public:
    static TMXReadWriteTestNew* create()
    {
        auto ret = new TMXReadWriteTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXReadWriteTestNew();
    virtual std::string title() const override;

    void removeSprite(Node* sender);
    void updateCol(float dt);
    void repaintWithGID(float dt);
    void removeTiles(float dt);
};

class TMXHexTestNew : public TileDemoNew
{
public:
    static TMXHexTestNew* create()
    {
        auto ret = new TMXHexTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXHexTestNew();
    virtual std::string title() const override;
};

class TMXIsoTestNew : public TileDemoNew
{
public:
    static TMXIsoTestNew* create()
    {
        auto ret = new TMXIsoTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTestNew();
    virtual std::string title() const override;
};

class TMXIsoTest1New : public TileDemoNew
{
public:
    static TMXIsoTest1New* create()
    {
        auto ret = new TMXIsoTest1New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTest1New();
    virtual std::string title() const override;
};

class TMXIsoTest2New : public TileDemoNew
{
public:
    static TMXIsoTest2New* create()
    {
        auto ret = new TMXIsoTest2New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoTest2New();
    virtual std::string title() const override;
};

class TMXUncompressedTestNew : public TileDemoNew
{
public:
    static TMXUncompressedTestNew* create()
    {
        auto ret = new TMXUncompressedTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXUncompressedTestNew();
    virtual std::string title() const override;
};

class TMXTilesetTestNew : public TileDemoNew
{
public:
    static TMXTilesetTestNew* create()
    {
        auto ret = new TMXTilesetTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXTilesetTestNew();
    virtual std::string title() const override;
};

class TMXOrthoObjectsTestNew : public TileDemoNew
{
public:
    static TMXOrthoObjectsTestNew* create()
    {
        auto ret = new TMXOrthoObjectsTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoObjectsTestNew();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXIsoObjectsTestNew : public TileDemoNew
{
public:
    static TMXIsoObjectsTestNew* create()
    {
        auto ret = new TMXIsoObjectsTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoObjectsTestNew();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXResizeTestNew : public TileDemoNew
{
public:
    static TMXResizeTestNew* create()
    {
        auto ret = new TMXResizeTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXResizeTestNew();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class TMXIsoZorderNew : public TileDemoNew
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXIsoZorderNew* create()
    {
        auto ret = new TMXIsoZorderNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoZorderNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void onExit()override;

    ~TMXIsoZorderNew();
    void repositionSprite(float dt);
};

class TMXOrthoZorderNew : public TileDemoNew
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXOrthoZorderNew* create()
    {
        auto ret = new TMXOrthoZorderNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoZorderNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    virtual ~TMXOrthoZorderNew();
    void repositionSprite(float dt);
};

class TMXIsoVertexZNew : public TileDemoNew
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXIsoVertexZNew* create()
    {
        auto ret = new TMXIsoVertexZNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoVertexZNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    ~TMXIsoVertexZNew();
    void repositionSprite(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXOrthoVertexZNew : public TileDemoNew
{
    cocos2d::Sprite*    _tamara;
public:
    static TMXOrthoVertexZNew* create()
    {
        auto ret = new TMXOrthoVertexZNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoVertexZNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    ~TMXOrthoVertexZNew();
    void repositionSprite(float dt);
    virtual void onEnter() override;
    virtual void onExit() override;
};

class TMXIsoMoveLayerNew : public TileDemoNew
{
public:
    static TMXIsoMoveLayerNew* create()
    {
        auto ret = new TMXIsoMoveLayerNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXIsoMoveLayerNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXOrthoMoveLayerNew : public TileDemoNew
{
public:
    static TMXOrthoMoveLayerNew* create()
    {
        auto ret = new TMXOrthoMoveLayerNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoMoveLayerNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXTilePropertyTestNew : public TileDemoNew
{
public:
    static TMXTilePropertyTestNew* create()
    {
        auto ret = new TMXTilePropertyTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXTilePropertyTestNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXOrthoFlipTestNew : public TileDemoNew
{
public:
    static TMXOrthoFlipTestNew* create()
    {
        auto ret = new TMXOrthoFlipTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFlipTestNew();
    virtual std::string title() const override;
};

class TMXOrthoFlipRunTimeTestNew : public TileDemoNew
{
public:
    static TMXOrthoFlipRunTimeTestNew* create()
    {
        auto ret = new TMXOrthoFlipRunTimeTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFlipRunTimeTestNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void flipIt(float dt);
};

class TMXOrthoFromXMLTestNew : public TileDemoNew
{
public:
    static TMXOrthoFromXMLTestNew* create()
    {
        auto ret = new TMXOrthoFromXMLTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoFromXMLTestNew();
    virtual std::string title() const override;
};

class TMXOrthoXMLFormatTestNew : public TileDemoNew
{
public:
    static TMXOrthoXMLFormatTestNew* create()
    {
        auto ret = new TMXOrthoXMLFormatTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXOrthoXMLFormatTestNew();
    virtual std::string title() const override;
};

class TMXBug987New : public TileDemoNew
{
public:
    static TMXBug987New* create()
    {
        auto ret = new TMXBug987New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXBug987New();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXBug787New : public TileDemoNew
{
public:
    static TMXBug787New* create()
    {
        auto ret = new TMXBug787New;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXBug787New();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TMXGIDObjectsTestNew : public TileDemoNew
{
public:
    static TMXGIDObjectsTestNew* create()
    {
        auto ret = new TMXGIDObjectsTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }
    TMXGIDObjectsTestNew();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;   
};

#endif
