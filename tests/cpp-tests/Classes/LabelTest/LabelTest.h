#ifndef _ATLAS_TEST_H_
#define _ATLAS_TEST_H_

#include "../BaseTest.h"

namespace cocos2d {
class Menu;
class MenuItemFont;
class Sprite;
class TextureAtlas;
}

DEFINE_TEST_SUITE(LabelTests);

class AtlasDemo : public TestCase
{
public:
};


class Atlas1 : public AtlasDemo
{
    cocos2d::TextureAtlas*        _textureAtlas;
public:
    static Atlas1* create()
    {
        auto ret = new Atlas1;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Atlas1();
    ~Atlas1();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    virtual void draw(cocos2d::Renderer* renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
protected:
    void onDraw(const cocos2d::Mat4& transform, uint32_t flags);
protected:
    cocos2d::CustomCommand _customCommand;
};

class LabelAtlasTest : public AtlasDemo
{
    float        _time;
public:
    static LabelAtlasTest* create()
    {
        auto ret = new LabelAtlasTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAtlasTest();

    virtual void step(float dt);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelAtlasColorTest : public AtlasDemo
{
    float        _time;
public:
    static LabelAtlasColorTest* create()
    {
        auto ret = new LabelAtlasColorTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAtlasColorTest();
    virtual void step(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void actionFinishCallback();
};

class LabelTTFAlignment : public AtlasDemo
{
public:
    static LabelTTFAlignment* create()
    {
        auto ret = new LabelTTFAlignment;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFAlignment();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


class Atlas3 : public AtlasDemo
{
    float        _time;
public:
    static Atlas3* create()
    {
        auto ret = new Atlas3;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Atlas3();

    virtual void step(float dt);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Atlas4 : public AtlasDemo
{
    float        _time;
public:
    static Atlas4* create()
    {
        auto ret = new Atlas4;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Atlas4();
    virtual void step(float dt);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Atlas5 : public AtlasDemo
{
public:
    static Atlas5* create()
    {
        auto ret = new Atlas5;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Atlas5();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


class Atlas6 : public AtlasDemo
{
public:
    static Atlas6* create()
    {
        auto ret = new Atlas6;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Atlas6();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class AtlasBitmapColor : public AtlasDemo
{
public:
    static AtlasBitmapColor* create()
    {
        auto ret = new AtlasBitmapColor;
        ret->init();
        ret->autorelease();
        return ret;
    }

    AtlasBitmapColor();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class AtlasFastBitmap : public AtlasDemo
{
public:
    static AtlasFastBitmap* create()
    {
        auto ret = new AtlasFastBitmap;
        ret->init();
        ret->autorelease();
        return ret;
    }

    AtlasFastBitmap();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class BitmapFontMultiLine : public AtlasDemo
{
public:
    static BitmapFontMultiLine* create()
    {
        auto ret = new BitmapFontMultiLine;
        ret->init();
        ret->autorelease();
        return ret;
    }

    BitmapFontMultiLine();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelsEmpty : public AtlasDemo
{
public:
    static LabelsEmpty* create()
    {
        auto ret = new LabelsEmpty;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelsEmpty();
    void updateStrings(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
    bool setEmpty;
};

class LabelBMFontHD : public AtlasDemo
{
public:
    static LabelBMFontHD* create()
    {
        auto ret = new LabelBMFontHD;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelBMFontHD();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelAtlasHD : public AtlasDemo
{
public:
    static LabelAtlasHD* create()
    {
        auto ret = new LabelAtlasHD;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAtlasHD();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelGlyphDesigner : public AtlasDemo
{
public:
    static LabelGlyphDesigner* create()
    {
        auto ret = new LabelGlyphDesigner;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelGlyphDesigner();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFTest : public AtlasDemo
{
public:
    static LabelTTFTest* create()
    {
        auto ret = new LabelTTFTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFTest();
    virtual ~LabelTTFTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    void  setAlignmentLeft(cocos2d::Ref* sender);
    void  setAlignmentCenter(cocos2d::Ref* sender);
    void  setAlignmentRight(cocos2d::Ref* sender);
    void  setAlignmentTop(cocos2d::Ref* sender);
    void  setAlignmentMiddle(cocos2d::Ref* sender);
    void  setAlignmentBottom(cocos2d::Ref* sender);
    void  updateAlignment();
    std::string getCurrentAlignment();
private:
    cocos2d::TextHAlignment _horizAlign;
    cocos2d::Label*         _label;
    cocos2d::TextVAlignment _vertAlign;
};

class LabelTTFMultiline : public AtlasDemo
{
public:
    static LabelTTFMultiline* create()
    {
        auto ret = new LabelTTFMultiline;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFMultiline();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFChinese : public AtlasDemo
{
public:
    static LabelTTFChinese* create()
    {
        auto ret = new LabelTTFChinese;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFChinese();
    virtual std::string title() const override;
};

class LabelBMFontChinese : public AtlasDemo
{
public:
    static LabelBMFontChinese* create()
    {
        auto ret = new LabelBMFontChinese;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelBMFontChinese();
    virtual std::string title() const override;
};

class BitmapFontMultiLineAlignment : public AtlasDemo
{
public:
    static BitmapFontMultiLineAlignment* create()
    {
        auto ret = new BitmapFontMultiLineAlignment;
        ret->init();
        ret->autorelease();
        return ret;
    }

    BitmapFontMultiLineAlignment();
    ~BitmapFontMultiLineAlignment();
    void snapArrowsToEdge();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void stringChanged(cocos2d::Ref* sender);
    void alignmentChanged(cocos2d::Ref* sender);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

public:
    cocos2d::Label        *_labelShouldRetain;
    cocos2d::Sprite       *_arrowsBarShouldRetain;
    cocos2d::Sprite       *_arrowsShouldRetain;
    cocos2d::MenuItemFont *_lastSentenceItem, *_lastAlignmentItem;
    bool _drag;
};

class LabelTTFOpacityTest : public AtlasDemo
{
public:
    static LabelTTFOpacityTest* create()
    {
        auto ret = new LabelTTFOpacityTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFOpacityTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class BMFontOneAtlas : public AtlasDemo
{
public:
    static BMFontOneAtlas* create()
    {
        auto ret = new BMFontOneAtlas;
        ret->init();
        ret->autorelease();
        return ret;
    }

    BMFontOneAtlas();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class BMFontUnicode : public AtlasDemo
{
public:
    static BMFontUnicode* create()
    {
        auto ret = new BMFontUnicode;
        ret->init();
        ret->autorelease();
        return ret;
    }

    BMFontUnicode();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class BMFontInit : public AtlasDemo
{
public:
    static BMFontInit* create()
    {
        auto ret = new BMFontInit;
        ret->init();
        ret->autorelease();
        return ret;
    }

    BMFontInit();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class TTFFontInit : public AtlasDemo
{
public:
    static TTFFontInit* create()
    {
        auto ret = new TTFFontInit;
        ret->init();
        ret->autorelease();
        return ret;
    }

    TTFFontInit();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class Issue1343 : public AtlasDemo
{
public:
    static Issue1343* create()
    {
        auto ret = new Issue1343;
        ret->init();
        ret->autorelease();
        return ret;
    }

    Issue1343();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelBMFontBounds : public AtlasDemo
{
public:
    static LabelBMFontBounds* create()
    {
        auto ret = new LabelBMFontBounds;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelBMFontBounds();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class NewLabelTTFUnicode : public AtlasDemo
{
public:
    static NewLabelTTFUnicode* create()
    {
        auto ret = new NewLabelTTFUnicode;
        ret->init();
        ret->autorelease();
        return ret;
    }

    NewLabelTTFUnicode();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class NewLabelBMFontTest : public AtlasDemo
{
public:
    static NewLabelBMFontTest* create()
    {
        auto ret = new NewLabelBMFontTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    NewLabelBMFontTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
};

class NewLabelFontDefTest : public AtlasDemo
{
public:
    static NewLabelFontDefTest* create()
    {
        auto ret = new NewLabelFontDefTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    NewLabelFontDefTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
};

class LabelBMFontCrashTest : public AtlasDemo
{
public:
    static LabelBMFontCrashTest* create()
    {
        auto ret = new LabelBMFontCrashTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    virtual void onEnter() override;
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelBMFontBinaryFormat : public AtlasDemo
{
public:
    static LabelBMFontBinaryFormat* create()
    {
        auto ret = new LabelBMFontBinaryFormat;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelBMFontBinaryFormat();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// we don't support linebreak mode

#endif
