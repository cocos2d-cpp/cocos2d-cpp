#include "LabelTest.h"
#include "../testResource.h"
#include "cocos2d.h"

using namespace cocos2d;

enum {
    kTagTileMap = 1,
    kTagSpriteManager = 1,
    kTagAnimation1 = 1,
    kTagBitmapAtlas1 = 1,
    kTagBitmapAtlas2 = 2,
    kTagBitmapAtlas3 = 3,
};

enum {
    kTagSprite1,
    kTagSprite2,
    kTagSprite3,
    kTagSprite4,
    kTagSprite5,
    kTagSprite6,
    kTagSprite7,
    kTagSprite8,
};

LabelTests::LabelTests()
{
    ADD_TEST_CASE(LabelAtlasTest);
    ADD_TEST_CASE(LabelAtlasColorTest);
    ADD_TEST_CASE(Atlas3);
    ADD_TEST_CASE(Atlas4);
    ADD_TEST_CASE(Atlas5);
    ADD_TEST_CASE(Atlas6);
    ADD_TEST_CASE(AtlasBitmapColor);
    ADD_TEST_CASE(AtlasFastBitmap);
    ADD_TEST_CASE(BitmapFontMultiLine);
    ADD_TEST_CASE(LabelsEmpty);
    ADD_TEST_CASE(LabelBMFontHD);
    ADD_TEST_CASE(LabelAtlasHD);
    ADD_TEST_CASE(LabelGlyphDesigner);
    ADD_TEST_CASE(LabelTTFTest);
    ADD_TEST_CASE(LabelTTFMultiline);
    ADD_TEST_CASE(LabelTTFChinese);
    ADD_TEST_CASE(LabelBMFontChinese);
    ADD_TEST_CASE(BitmapFontMultiLineAlignment);
    ADD_TEST_CASE(LabelTTFOpacityTest);
    ADD_TEST_CASE(BMFontOneAtlas);
    ADD_TEST_CASE(BMFontUnicode);
    ADD_TEST_CASE(BMFontInit);
    ADD_TEST_CASE(TTFFontInit);
    ADD_TEST_CASE(Issue1343);
    ADD_TEST_CASE(LabelTTFAlignment);
    ADD_TEST_CASE(LabelBMFontBounds);
    // should be moved to another test
    ADD_TEST_CASE(Atlas1);
    ADD_TEST_CASE(LabelBMFontCrashTest);
	ADD_TEST_CASE(LabelBMFontBinaryFormat);
}

//------------------------------------------------------------------
//
// Atlas1
//
//------------------------------------------------------------------
Atlas1::Atlas1()
{
    setGLProgram(GLProgramCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE));
    _textureAtlas = TextureAtlas::create(s_AtlasTest, 3); _textureAtlas->retain();
    
    auto s = Director::getInstance()->getWinSize();

    //
    // Notice: u,v tex coordinates are inverted
    //
    V3F_C4B_T2F_Quad quads[] = 
    {
        {
            {Vec3(0,0,0),Color4B(0,0,255,255),Tex2F(0.0f,1.0f),},                // bottom left
            {Vec3(s.width,0,0),Color4B(0,0,255,0),Tex2F(1.0f,1.0f),},            // bottom right
            {Vec3(0,s.height,0),Color4B(0,0,255,0),Tex2F(0.0f,0.0f),},            // top left
            {Vec3(s.width,s.height,0),Color4B(0,0,255,255),Tex2F(1.0f,0.0f),},    // top right
        },        
        {
            {Vec3(40,40,0),Color4B(255,255,255,255),Tex2F(0.0f,0.2f),},            // bottom left
            {Vec3(120,80,0),Color4B(255,0,0,255),Tex2F(0.5f,0.2f),},            // bottom right
            {Vec3(40,160,0),Color4B(255,255,255,255),Tex2F(0.0f,0.0f),},        // top left
            {Vec3(160,160,0),Color4B(0,255,0,255),Tex2F(0.5f,0.0f),},            // top right
        },

        {
            {Vec3(s.width/2,40,0),Color4B(255,0,0,255),Tex2F(0.0f,1.0f),},        // bottom left
            {Vec3(s.width,40,0),Color4B(0,255,0,255),Tex2F(1.0f,1.0f),},        // bottom right
            {Vec3(s.width/2-50,200,0),Color4B(0,0,255,255),Tex2F(0.0f,0.0f),},        // top left
            {Vec3(s.width,100,0),Color4B(255,255,0,255),Tex2F(1.0f,0.0f),},        // top right
        },
        
    };
    
    
    for( int i=0;i<3;i++) 
    {
        _textureAtlas->updateQuad(&quads[i], i);
    }
}

Atlas1::~Atlas1()
{
    _textureAtlas->release();
}

void Atlas1::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    _customCommand.init(_globalZOrder);
    _customCommand.func = CC_CALLBACK_0(Atlas1::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void Atlas1::onDraw(const Mat4 & transform, uint32_t)
{
    getGLProgram()->use();
    getGLProgram()->setUniformsForBuiltins(transform);
    _textureAtlas->drawQuads();
}

std::string Atlas1::title() const
{
    return "CCTextureAtlas";
}

std::string Atlas1::subtitle() const
{
    return "Manual creation of TextureAtlas";
}

//------------------------------------------------------------------
//
// LabelAtlasTest
//
//------------------------------------------------------------------

LabelAtlasTest::LabelAtlasTest()
{
    _time = 0;

    auto label1 = LabelAtlas::create("123 Test", "fonts/tuffy_bold_italic-charmap.plist");
    addChild(label1, 0, kTagSprite1);
    label1->setPosition( Vec2(10,100) );
    label1->setOpacity( 200 );

    auto label2 = LabelAtlas::create("0123456789", "fonts/tuffy_bold_italic-charmap.plist");
    addChild(label2, 0, kTagSprite2);
    label2->setPosition( Vec2(10,200) );
    label2->setOpacity( 32 );

    schedule(CC_CALLBACK_1(LabelAtlasTest::step, this), "step_key");
}

void LabelAtlasTest::step(float dt)
{
    _time += dt;
    char string[12] = {0};
    sprintf(string, "%2.2f Test", _time);
    //string.format("%2.2f Test", _time);

    auto label1 = (LabelAtlas*)getChildByTag(kTagSprite1);
    label1->setString(string);

    auto label2 = (LabelAtlas*)getChildByTag(kTagSprite2);
    sprintf(string, "%d", (int)_time);
    //string.format("%d", (int)_time);
    label2->setString(string);
}

std::string LabelAtlasTest::title() const
{
    return "LabelAtlas";
}

std::string LabelAtlasTest::subtitle() const
{
    return "Updating label should be fast";
}

//------------------------------------------------------------------
//
// LabelAtlasColorTest
//
//------------------------------------------------------------------
LabelAtlasColorTest::LabelAtlasColorTest()
{
    auto label1 = LabelAtlas::create("123 Test", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
    addChild(label1, 0, kTagSprite1);
    label1->setPosition( Vec2(10,100) );
    label1->setOpacity( 200 );
    
    auto label2 = LabelAtlas::create("0123456789", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
    addChild(label2, 0, kTagSprite2);
    label2->setPosition( Vec2(10,200) );
    label2->setColor( Color3B::RED );

    auto fade = FadeOut::create(1.0f);
    auto fade_in = fade->reverse();
    auto cb = CallFunc::create(CC_CALLBACK_0(LabelAtlasColorTest::actionFinishCallback, this));
    auto seq = Sequence::create(
        to_action_ptr(fade),
        to_action_ptr(fade_in),
        to_action_ptr(cb)
    );
    auto repeat = RepeatForever::create( seq );
    label2->runAction( repeat );    

    _time = 0;
    
    schedule(CC_CALLBACK_1(LabelAtlasColorTest::step, this), "step_key");
}

void LabelAtlasColorTest::actionFinishCallback()
{
    CCLOG("Action finished");
}

void LabelAtlasColorTest::step(float dt)
{
    _time += dt;
    char string[12] = {0};
    sprintf(string, "%2.2f Test", _time);
    //std::string string = std::string::createWithFormat("%2.2f Test", _time);
    auto label1 = (LabelAtlas*)getChildByTag(kTagSprite1);
    label1->setString(string);
    
    auto label2 = (LabelAtlas*)getChildByTag(kTagSprite2);
    sprintf(string, "%d", (int)_time);
    label2->setString( string );    
}

std::string LabelAtlasColorTest::title() const
{
    return "CCLabelAtlas";
}

std::string LabelAtlasColorTest::subtitle() const
{
    return "Opacity + Color should work at the same time";
}


//------------------------------------------------------------------
//
// LabelTTFAlignment
//
//------------------------------------------------------------------
LabelTTFAlignment::LabelTTFAlignment()
{
    auto s = Director::getInstance()->getWinSize();

    const auto width           = s.width / 2;
    const auto height_distance = s.height / 6;
    auto       height          = height_distance;
    
    auto nextHeight = [height_distance, & height] {
        height += height_distance;
        return height;
    };

    for (auto hAlign : { TextHAlignment::LEFT,
                        TextHAlignment::CENTER,
                        TextHAlignment::RIGHT })
    {
        auto txt = "Alignment " + std::to_string(static_cast<long>(hAlign)) + '\n'
            + "new line";
        auto ttf = Label::createWithTTF(txt,
                                        "fonts/arial.ttf", 12,
                                        Size(256, 32), hAlign);

        ttf->setPosition( Vec2{width, nextHeight()} );
        ttf->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        this->addChild(ttf);
    }
}

std::string LabelTTFAlignment::title() const
{
    return "Label with TTF alignment";
}

std::string LabelTTFAlignment::subtitle() const
{
    return "Tests alignment values";
}

//------------------------------------------------------------------
//
// Atlas3
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------
Atlas3::Atlas3()
{
    _time = 0;

    auto col = LayerColor::create( Color4B(128,128,128,255) );
    addChild(col, -10);
    
    auto label1 = Label::createWithBMFont("fonts/bitmapFontTest2.fnt",
                                          "Test");
    
    // testing anchors
    label1->setAnchorPoint( Vec2::ANCHOR_BOTTOM_LEFT );
    addChild(label1, 0, kTagBitmapAtlas1);
    auto fade = FadeOut::create(1.0f);
    auto fade_in = fade->reverse();
    auto seq = Sequence::create(
        to_action_ptr(fade),
        to_action_ptr(fade_in)
    );
    auto repeat = RepeatForever::create(seq);
    label1->runAction(repeat);
    

    // VERY IMPORTANT
    // color and opacity work OK because bitmapFontAltas2 loads a BMP image (not a PNG image)
    // If you want to use both opacity and color, it is recommended to use NON premultiplied images like BMP images
    // Of course, you can also tell XCode not to compress PNG images, but I think it doesn't work as expected
    auto label2 = Label::createWithBMFont("fonts/bitmapFontTest2.fnt",
                                          "Test");
    // testing anchors
    label2->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    label2->setColor( Color3B::RED );
    addChild(label2, 0, kTagBitmapAtlas2);
    auto tint = Sequence::create(
        to_action_ptr(TintTo::create(1, 255, 0, 0)),
        to_action_ptr(TintTo::create(1, 0, 255, 0)),
        to_action_ptr(TintTo::create(1, 0, 0, 255))
    );
    label2->runAction( RepeatForever::create(tint) );
    
    auto label3 = Label::createWithBMFont("fonts/bitmapFontTest2.fnt",
                                          "Test");
    // testing anchors
    label3->setAnchorPoint( Vec2::ANCHOR_TOP_RIGHT );
    addChild(label3, 0, kTagBitmapAtlas3);
       
    label1->setPosition( VisibleRect::leftBottom() );
    label2->setPosition( VisibleRect::center() );
    label3->setPosition( VisibleRect::rightTop() );

    schedule(CC_CALLBACK_1(Atlas3::step, this), "step_key");
}

void Atlas3::step(float dt)
{
    _time += dt;
    //std::string string;
    char string[15] = {0};
    sprintf(string, "%2.2f Test j", _time);
    //string.format("%2.2f Test j", _time);
    
    auto label1 = (Label*) getChildByTag(kTagBitmapAtlas1);
    label1->setString(string);
    
    auto label2 = (Label*) getChildByTag(kTagBitmapAtlas2);
    label2->setString(string);
    
    auto label3 = (Label*) getChildByTag(kTagBitmapAtlas3);
    label3->setString(string);
}

std::string Atlas3::title() const
{
    return "CCLabel with BMFont";
}

std::string Atlas3::subtitle() const
{
    return "Testing alignment. Testing opacity + tint";
}

//------------------------------------------------------------------
//
// Atlas4
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------
Atlas4::Atlas4()
{
    _time = 0;

    auto s = Director::getInstance()->getWinSize();
    
    auto drawNode = DrawNode::create();
    drawNode->drawLine( Vec2(0, s.height/2), Vec2(s.width, s.height/2), Color4F(1.0, 1.0, 1.0, 1.0) );
    drawNode->drawLine( Vec2(s.width/2, 0), Vec2(s.width/2, s.height), Color4F(1.0, 1.0, 1.0, 1.0) );
    addChild(drawNode, -1);

    // Upper Label
    auto label = Label::createWithBMFont("fonts/bitmapFontTest.fnt",
                                         "Bitmap Font Atlas");
    addChild(label);
    
    label->setPosition( Vec2(s.width/2, s.height/2) );
    label->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    
    
    auto BChar = (Sprite*) label->getLetter(0);
    auto FChar = (Sprite*) label->getLetter(7);
    auto AChar = (Sprite*) label->getLetter(12);
    
    auto rotate = RotateBy::create(2, 360);
    auto rot_4ever = RepeatForever::create(rotate);
    
    auto scale = ScaleBy::create(2, 1.5f);
    auto scale_back = scale->reverse();
    auto scale_seq = Sequence::create(
        to_action_ptr(scale),
        to_action_ptr(scale_back)
    );
    auto scale_4ever = RepeatForever::create(scale_seq);
    
    auto jump = JumpBy::create(0.5f, Vec2::ZERO, 60, 1);
    auto jump_4ever = RepeatForever::create(jump);
    
    auto fade_out = FadeOut::create(1);
    auto fade_in = FadeIn::create(1);
    auto seq = Sequence::create(
        to_action_ptr(fade_out),
        to_action_ptr(fade_in)
    );
    auto fade_4ever = RepeatForever::create(seq);
    
    BChar->runAction(rot_4ever);
    BChar->runAction(scale_4ever);
    FChar->runAction(jump_4ever);
    AChar->runAction(fade_4ever);
    
    // Bottom Label
    auto label2 = Label::createWithBMFont("fonts/bitmapFontTest.fnt",
                                          "00.0");
    addChild(label2, 0, kTagBitmapAtlas2);
    label2->setPosition( Vec2(s.width/2.0f, 80) );
    
    auto lastChar = (Sprite*) label2->getLetter(3);
    lastChar->runAction( rot_4ever->clone() );
    
    schedule(CC_CALLBACK_1(Atlas4::step, this), 0.1f, "step_key");
}

void Atlas4::step(float dt)
{
    _time += dt;
    char string[10] = {0};
    sprintf(string, "%04.1f", _time);
//     std::string string;
//     string.format("%04.1f", _time);
    
    auto label1 = (Label*)getChildByTag(kTagBitmapAtlas2);
    label1->setString(string);
}

std::string Atlas4::title() const
{
    return "Label with BMFont";
}

std::string Atlas4::subtitle() const
{
    return "Using fonts as Sprite objects. Some characters should rotate.";
}

//------------------------------------------------------------------
//
// Atlas5
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------

Atlas5::Atlas5()
{
    auto label = Label::createWithBMFont("fonts/bitmapFontTest4.fnt",
                                         "abcdefg");
    addChild(label);
    
    auto s = Director::getInstance()->getWinSize();
    
    label->setPosition( Vec2(s.width / 2, s.height / 2) );
    label->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
}

std::string Atlas5::title() const
{
    return "Label with BMFont";
}

std::string Atlas5::subtitle() const
{
    return "Testing padding";
}
//------------------------------------------------------------------
//
// Atlas6
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------

Atlas6::Atlas6()
{
    auto s = Director::getInstance()->getWinSize();

    const auto         width  = s.width / 2;
    decltype(s.height) height = s.height / 2 + 50;

    auto nextHeight = [&height] {
        auto curr = height;
        height -= 50;
        return curr;
    };

    for (auto txt : {"FaFeFiFoFu", "fafefifofu", "aeiou"})
    {
        Label* label = Label::createWithBMFont(
            "fonts/bitmapFontTest5.fnt",
            txt
        );
        addChild(label);
        label->setPosition( Vec2{width, nextHeight()} );
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE ) ;
    }
}

std::string Atlas6::title() const
{
    return "Label with BMFont";
}

std::string Atlas6::subtitle() const
{
    return "Rendering should be OK. Testing offset";
}

//------------------------------------------------------------------
//
// AtlasBitmapColor
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------
AtlasBitmapColor::AtlasBitmapColor()
{
    auto s = Director::getInstance()->getWinSize();

    const auto width = s.width / 2;
    constexpr char font_fname[] = "fonts/bitmapFontTest5.fnt";

    struct {
        std::string        txt;
        const Color3B      color;
        decltype(s.height) height;
    } params[] = {
        {"Blue",  Color3B::BLUE,  s.height / 4,   },
        {"Red",   Color3B::RED,   s.height / 4 * 2},
        {"G",     Color3B::GREEN, s.height / 4 * 3}
    };

    Label* label = nullptr;
    
    for (auto p : params)
    {
        label = Label::createWithBMFont(font_fname, p.txt);
        label->setColor( p.color );
        addChild(label);
        label->setPosition( Vec2{width, p.height} );
        label->setAnchorPoint( Vec2::ANCHOR_MIDDLE );
    }

    if (label)
    {
        label->setString("Green");
    }
}

std::string AtlasBitmapColor::title() const
{
    return "Label with BMFont";
}

std::string AtlasBitmapColor::subtitle() const
{
    return "Testing color";
}

//------------------------------------------------------------------
//
// AtlasFastBitmap
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------

AtlasFastBitmap::AtlasFastBitmap()
{
    // Upper Label
    for (int i = 0; i < 100; i++) 
    {
        auto label = Label::createWithBMFont("fonts/bitmapFontTest.fnt",
                                             '-'+std::to_string(i)+'-');
        addChild(label);

        auto s = Director::getInstance()->getWinSize();

        auto p = Vec2( CCRANDOM_0_1() * s.width, CCRANDOM_0_1() * s.height);
        label->setPosition( p );
        label->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    }
}

std::string AtlasFastBitmap::title() const
{
    return "Label with BMFont";
}

std::string AtlasFastBitmap::subtitle() const
{
    return "Creating several Labels with BMFont with the same .fnt file should be fast";
}

//------------------------------------------------------------------
//
// BitmapFontMultiLine
//
// Use any of these editors to generate BMFonts:
//     http://glyphdesigner.71squared.com/ (Commercial, Mac OS X)
//     http://www.n4te.com/hiero/hiero.jnlp (Free, Java)
//     http://slick.cokeandcode.com/demos/hiero.jnlp (Free, Java)
//     http://www.angelcode.com/products/bmfont/ (Free, Windows only)
//
//------------------------------------------------------------------
BitmapFontMultiLine::BitmapFontMultiLine()
{
    Size s;

    // Left
    auto label1 = Label::createWithBMFont("fonts/bitmapFontTest3.fnt",
                                          " Multi line\n"
                                          "Left");
    label1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    addChild(label1, 0, kTagBitmapAtlas1);

    s = label1->getContentSize();
    CCLOG("content size: %.2fx%.2f", s.width, s.height);


    // Center
    auto label2 = Label::createWithBMFont("fonts/bitmapFontTest3.fnt",
                                          "Multi line\n"
                                          "Center");
    label2->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    addChild(label2, 0, kTagBitmapAtlas2);

    s= label2->getContentSize();
    CCLOG("content size: %.2fx%.2f", s.width, s.height);

    // right
    auto label3 = Label::createWithBMFont("fonts/bitmapFontTest3.fnt",
                                          "Multi line\n"
                                          "Right\n"
                                          "Three lines Three");
    label3->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    addChild(label3, 0, kTagBitmapAtlas3);

    s = label3->getContentSize();
    CCLOG("content size: %.2fx%.2f", s.width, s.height);

    label1->setPosition(VisibleRect::leftBottom());
    label2->setPosition(VisibleRect::center());
    label3->setPosition(VisibleRect::rightTop());
}

std::string BitmapFontMultiLine::title() const
{
    return "Label with BMFont";
}

std::string BitmapFontMultiLine::subtitle() const
{
    return "Multiline + anchor point";
}

//------------------------------------------------------------------
//
// LabelsEmpty
//
//------------------------------------------------------------------
//
LabelsEmpty::LabelsEmpty()
{
    auto s = Director::getInstance()->getWinSize();

    // Label with BMFont
    auto label1 = Label::createWithBMFont("fonts/bitmapFontTest3.fnt", "");
    addChild(label1, 0, kTagBitmapAtlas1);
    label1->setPosition(Vec2(s.width/2, s.height-100));

    // Label with TTF
    auto label2 = Label::createWithTTF("", "fonts/arial.ttf", 24);
    addChild(label2, 0, kTagBitmapAtlas2);
    label2->setPosition(Vec2(s.width / 2, s.height / 2));

    // LabelAtlas
    auto label3 = LabelAtlas::create("", "fonts/tuffy_bold_italic-charmap.png", 48, 64, ' ');
    addChild(label3, 0, kTagBitmapAtlas3);
    label3->setPosition(Vec2(s.width/2, 0+100));

    schedule(CC_CALLBACK_1(LabelsEmpty::updateStrings, this), 1.0f, "update_strings_key");

    setEmpty = false;
}

void LabelsEmpty::updateStrings(float /*dt*/)
{
    auto label1 = static_cast<Label*>( getChildByTag(kTagBitmapAtlas1) );
    auto label2 = static_cast<Label*>( getChildByTag(kTagBitmapAtlas2) );
    auto label3 = static_cast<LabelAtlas*>( getChildByTag(kTagBitmapAtlas3) );

    if( ! setEmpty )
    {
        label1->setString("not empty (BMFont)");
        label2->setString("not empty (TTF)");
        label3->setString("hi");

        setEmpty = true;
    }
    else
    {
        label1->setString("");
        label2->setString("");
        label3->setString("");

        setEmpty = false;
    }
}

std::string LabelsEmpty::title() const
{
    return "Testing empty labels";
}

std::string LabelsEmpty::subtitle() const
{
    return "3 empty labels: LabelAtlas, Labels with TTF and BMFont";
}

//------------------------------------------------------------------
//
// LabelBMFontHD
//
//------------------------------------------------------------------
LabelBMFontHD::LabelBMFontHD()
{
    auto s = Director::getInstance()->getWinSize();

    auto label1 = Label::createWithBMFont("fonts/konqa32.fnt",
                                          "TESTING RETINA DISPLAY");
    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/2));
}

std::string LabelBMFontHD::title() const
{
    return "Testing Retina Display BMFont";
}

std::string LabelBMFontHD::subtitle() const
{
    return "loading arista16 or arista16-hd";
}

//------------------------------------------------------------------
//
// LabelAtlasHD
//
//------------------------------------------------------------------
LabelAtlasHD::LabelAtlasHD()
{
    auto s = Director::getInstance()->getWinSize();

    auto label1 = LabelAtlas::create("TESTING RETINA DISPLAY", "fonts/larabie-16.plist");
    label1->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/2));
}

std::string LabelAtlasHD::title() const
{
    return "LabelAtlas with Retina Display";
}

std::string LabelAtlasHD::subtitle() const
{
    return "loading larabie-16 / larabie-16-hd";
}

//------------------------------------------------------------------
//
// LabelGlyphDesigner
//
//------------------------------------------------------------------
LabelGlyphDesigner::LabelGlyphDesigner()
{
    auto s = Director::getInstance()->getWinSize();

    auto layer = LayerColor::create(Color4B(128,128,128,255));
    addChild(layer, -10);

    auto label1 = Label::createWithBMFont("fonts/futura-48.fnt",
                                          "Testing Glyph Designer");
    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/2));
}

std::string LabelGlyphDesigner::title() const
{
    return "Testing Glyph Designer";
}

std::string LabelGlyphDesigner::subtitle() const
{
    return "You should see a font with shawdows and outline";
}

//------------------------------------------------------------------
//
// LabelTTFTest
//
//------------------------------------------------------------------
LabelTTFTest::LabelTTFTest()
{
    auto blockSize = Size(200, 160);
    auto s = Director::getInstance()->getWinSize();

    auto colorLayer = LayerColor::create(Color4B(100, 100, 100, 255), blockSize.width, blockSize.height);
    colorLayer->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    colorLayer->setPosition(Vec2((s.width - blockSize.width) / 2, (s.height - blockSize.height) / 2));

    this->addChild(colorLayer);

    MenuItemFont::setFontSize(30);
    auto menu = Menu::create(
		MenuItemFont::create("Left", CC_CALLBACK_1(LabelTTFTest::setAlignmentLeft, this)),
		MenuItemFont::create("Center", CC_CALLBACK_1(LabelTTFTest::setAlignmentCenter, this)),
		MenuItemFont::create("Right", CC_CALLBACK_1(LabelTTFTest::setAlignmentRight, this)),
        nullptr);
    menu->alignItemsVerticallyWithPadding(4);
    menu->setPosition(Vec2(50, s.height / 2 - 20));
    this->addChild(menu);

    menu = Menu::create(
		MenuItemFont::create("Top", CC_CALLBACK_1(LabelTTFTest::setAlignmentTop, this)),
		MenuItemFont::create("Middle", CC_CALLBACK_1(LabelTTFTest::setAlignmentMiddle, this)),
		MenuItemFont::create("Bottom", CC_CALLBACK_1(LabelTTFTest::setAlignmentBottom, this)),
		nullptr);
    menu->alignItemsVerticallyWithPadding(4);
    menu->setPosition(Vec2(s.width - 50, s.height / 2 - 20));
    this->addChild(menu);

    _label = nullptr;
    _horizAlign = TextHAlignment::LEFT;
    _vertAlign = TextVAlignment::TOP;

    this->updateAlignment();
}

LabelTTFTest::~LabelTTFTest()
{
    CC_SAFE_RELEASE(_label);
}

void  LabelTTFTest::updateAlignment()
{
    auto blockSize = Size(200, 160);
    auto s = Director::getInstance()->getWinSize();

    if (_label)
    {
        _label->removeFromParentAndCleanup(true);
    }

    CC_SAFE_RELEASE(_label);

    _label = Label::createWithTTF(this->getCurrentAlignment(),
                                  "fonts/Marker Felt.ttf", 32,
                                  blockSize, _horizAlign, _vertAlign);
    _label->retain();

    _label->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _label->setPosition(Vec2((s.width - blockSize.width) / 2, (s.height - blockSize.height)/2 ));

    this->addChild(_label);
}

void LabelTTFTest::setAlignmentLeft(Ref* /*sender*/)
{
    _horizAlign = TextHAlignment::LEFT;
    this->updateAlignment();
}

void LabelTTFTest::setAlignmentCenter(Ref* /*sender*/)
{
    _horizAlign = TextHAlignment::CENTER;
    this->updateAlignment();
}

void LabelTTFTest::setAlignmentRight(Ref* /*sender*/)
{
    _horizAlign = TextHAlignment::RIGHT;
    this->updateAlignment();
}

void LabelTTFTest::setAlignmentTop(Ref* /*sender*/)
{
    _vertAlign = TextVAlignment::TOP;
    this->updateAlignment();
}

void LabelTTFTest::setAlignmentMiddle(Ref* /*sender*/)
{
    _vertAlign = TextVAlignment::CENTER;
    this->updateAlignment();
}

void LabelTTFTest::setAlignmentBottom(Ref* /*sender*/)
{
    _vertAlign = TextVAlignment::BOTTOM;
    this->updateAlignment();
}

std::string LabelTTFTest::getCurrentAlignment()
{
    std::string currentAlignment = "Alignment ";

    switch (_vertAlign) {
        case TextVAlignment::TOP:
            currentAlignment += "Top";
            break;
        case TextVAlignment::CENTER:
            currentAlignment += "Middle";
            break;
        case TextVAlignment::BOTTOM:
            currentAlignment += "Bottom";
            break;
    }

    currentAlignment += ' ';

    switch (_horizAlign) {
        case TextHAlignment::LEFT:
            currentAlignment += "Left";
            break;
        case TextHAlignment::CENTER:
            currentAlignment += "Center";
            break;
        case TextHAlignment::RIGHT:
            currentAlignment += "Right";
            break;
    }

    return currentAlignment;
}

std::string LabelTTFTest::title() const
{
    return "Testing Label created with TTF";
}

std::string LabelTTFTest::subtitle() const
{
    return "Select the buttons on the sides to change alignment";
}

LabelTTFMultiline::LabelTTFMultiline()
{
    auto s = Director::getInstance()->getWinSize();

    constexpr auto txt = "word wrap \"testing\" (bla0) bla1 'bla2' [bla3] (bla4) {bla5} {bla6} [bla7] (bla8) [bla9] 'bla0' \"bla1\"";

    auto center = Label::createWithTTF(txt,
                                       "fonts/Paint Boy.ttf",
                                       24,
                                       Size(s.width/2,200),
                                       TextHAlignment::CENTER,
                                       TextVAlignment::TOP);

    center->setPosition(Vec2(s.width / 2, 150));

    addChild(center);
}

std::string LabelTTFMultiline::title() const
{
    return "Testing Label with TTF: Word Wrap";
}

std::string LabelTTFMultiline::subtitle() const
{
    return "Word wrap using Label with a custom TTF font";
}

LabelTTFChinese::LabelTTFChinese()
{
    auto size = Director::getInstance()->getWinSize();
    auto label = Label::createWithTTF("中国", "fonts/Marker Felt.ttf", 30);
    label->setPosition(Vec2(size.width / 2, size.height /2));
    this->addChild(label);
}

std::string LabelTTFChinese::title() const
{
    return "Testing Label with TTF\nwith Chinese characters";
}

LabelBMFontChinese::LabelBMFontChinese()
{
    auto size = Director::getInstance()->getWinSize();
    auto label = Label::createWithBMFont("fonts/bitmapFontChinese.fnt",
                                         "中国");
    label->setPosition(Vec2(size.width / 2, size.height /2));
    this->addChild(label);
}

std::string LabelBMFontChinese::title() const
{
    return "Testing Label with BMFont\nwith Chinese characters";
}

/// BitmapFontMultiLineAlignment

#define LongSentencesExample "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua."
#define LineBreaksExample "Lorem ipsum dolor\nsit amet\nconsectetur adipisicing elit\nblah\nblah"
#define MixedExample "ABC\nLorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt\nDEF"

#define ArrowsMax 0.95
#define ArrowsMin 0.7

#define LeftAlign 0
#define CenterAlign 1
#define RightAlign 2

#define LongSentences 0
#define LineBreaks 1
#define Mixed 2

static float alignmentItemPadding = 50;
static float menuItemPaddingCenter = 50;
BitmapFontMultiLineAlignment::BitmapFontMultiLineAlignment()
{
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(BitmapFontMultiLineAlignment::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(BitmapFontMultiLineAlignment::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(BitmapFontMultiLineAlignment::onTouchesEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // ask director the the window size
    auto size = Director::getInstance()->getWinSize();

    // create and initialize a Label
    _labelShouldRetain = Label::createWithBMFont("fonts/markerFelt.fnt",
                                                 LongSentencesExample,
                                                 TextHAlignment::CENTER,
                                                 size.width / 1.5f);
    _labelShouldRetain->retain();

    _arrowsBarShouldRetain = Sprite::create("Images/arrowsBar.png");
    _arrowsBarShouldRetain->retain();
    _arrowsShouldRetain = Sprite::create("Images/arrows.png");
    _arrowsShouldRetain->retain();

    MenuItemFont::setFontSize(20);
    auto longSentences = MenuItemFont::create("Long Flowing Sentences", CC_CALLBACK_1(BitmapFontMultiLineAlignment::stringChanged, this));
    auto lineBreaks = MenuItemFont::create("Short Sentences With Intentional Line Breaks", CC_CALLBACK_1(BitmapFontMultiLineAlignment::stringChanged, this));
    auto mixed = MenuItemFont::create("Long Sentences Mixed With Intentional Line Breaks", CC_CALLBACK_1(BitmapFontMultiLineAlignment::stringChanged, this));
    auto stringMenu = Menu::create(longSentences, lineBreaks, mixed, nullptr);
    stringMenu->alignItemsVertically();

    longSentences->setColor(Color3B::RED);
    _lastSentenceItem = longSentences;
    longSentences->setTag(LongSentences);
    lineBreaks->setTag(LineBreaks);
    mixed->setTag(Mixed);

    MenuItemFont::setFontSize(30);

    auto left = MenuItemFont::create("Left", CC_CALLBACK_1(BitmapFontMultiLineAlignment::alignmentChanged, this));
    auto center = MenuItemFont::create("Center", CC_CALLBACK_1(BitmapFontMultiLineAlignment::alignmentChanged, this));
    auto right = MenuItemFont::create("Right", CC_CALLBACK_1(BitmapFontMultiLineAlignment::alignmentChanged, this));
    auto alignmentMenu = Menu::create(left, center, right, nullptr);
    alignmentMenu->alignItemsHorizontallyWithPadding(alignmentItemPadding);

    center->setColor(Color3B::RED);
    _lastAlignmentItem = center;
    left->setTag(LeftAlign);
    center->setTag(CenterAlign);
    right->setTag(RightAlign);

    // position the label on the center of the screen
    _labelShouldRetain->setPosition(Vec2(size.width/2, size.height/2));

    _arrowsBarShouldRetain->setVisible(false);

    float arrowsWidth = (ArrowsMax - ArrowsMin) * size.width;
    _arrowsBarShouldRetain->setScaleX(arrowsWidth / this->_arrowsBarShouldRetain->getContentSize().width);
    _arrowsBarShouldRetain->setPosition(Vec2(((ArrowsMax + ArrowsMin) / 2) * size.width, this->_labelShouldRetain->getPosition().y));

    this->snapArrowsToEdge();

    stringMenu->setPosition(Vec2(size.width/2, size.height - menuItemPaddingCenter));
    alignmentMenu->setPosition(Vec2(size.width/2, menuItemPaddingCenter+15));

    addChild(_labelShouldRetain);
    addChild(_arrowsBarShouldRetain);
    addChild(_arrowsShouldRetain);
    addChild(stringMenu);
    addChild(alignmentMenu);
}

BitmapFontMultiLineAlignment::~BitmapFontMultiLineAlignment()
{
    this->_labelShouldRetain->release();
    this->_arrowsBarShouldRetain->release();
    this->_arrowsShouldRetain->release();
}

std::string BitmapFontMultiLineAlignment::title() const
{
    return "";
}

std::string BitmapFontMultiLineAlignment::subtitle() const
{
    return "";
}

void BitmapFontMultiLineAlignment::stringChanged(cocos2d::Ref *sender)
{
    auto item = (MenuItemFont*)sender;
    item->setColor(Color3B::RED);
    this->_lastAlignmentItem->setColor(Color3B::WHITE);
    this->_lastAlignmentItem = item;

    switch(item->getTag())
    {
    case LongSentences:
        _labelShouldRetain->setString(LongSentencesExample);
        break;
    case LineBreaks:
        _labelShouldRetain->setString(LineBreaksExample);
        break;
    case Mixed:
        _labelShouldRetain->setString(MixedExample);
        break;

    default:
        break;
    }

    this->snapArrowsToEdge();
}

void BitmapFontMultiLineAlignment::alignmentChanged(cocos2d::Ref *sender)
{
    auto item = static_cast<MenuItemFont*>(sender);
    item->setColor(Color3B::RED);
    this->_lastAlignmentItem->setColor(Color3B::WHITE);
    this->_lastAlignmentItem = item;

    switch(item->getTag())
    {
    case LeftAlign:
        _labelShouldRetain->setAlignment(TextHAlignment::LEFT);
        break;
    case CenterAlign:
        _labelShouldRetain->setAlignment(TextHAlignment::CENTER);
        break;
    case RightAlign:
        _labelShouldRetain->setAlignment(TextHAlignment::RIGHT);
        break;

    default:
        break;
    }

    this->snapArrowsToEdge();
}

void BitmapFontMultiLineAlignment::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event*)
{
    auto touch = touches[0];
    auto location = touch->getLocationInView();

    if (this->_arrowsShouldRetain->getBoundingBox().containsPoint(location))
    {
        _drag = true;
        this->_arrowsBarShouldRetain->setVisible(true);
    }
}

void BitmapFontMultiLineAlignment::onTouchesEnded(const std::vector<Touch*>&, cocos2d::Event*)
{
    _drag = false;
    this->snapArrowsToEdge();

    this->_arrowsBarShouldRetain->setVisible(false);
}

void BitmapFontMultiLineAlignment::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event*)
{
    if (! _drag)
    {
        return;
    }

    auto touch = touches[0];
    auto location = touch->getLocationInView();

    auto winSize = Director::getInstance()->getWinSize();

    this->_arrowsShouldRetain->setPosition(Vec2(MAX(MIN(location.x, ArrowsMax*winSize.width), ArrowsMin*winSize.width), 
        this->_arrowsShouldRetain->getPosition().y));

    float labelWidth = fabs(this->_arrowsShouldRetain->getPosition().x - this->_labelShouldRetain->getPosition().x) * 2;

    _labelShouldRetain->setWidth(labelWidth);
}

void BitmapFontMultiLineAlignment::snapArrowsToEdge()
{
    this->_arrowsShouldRetain->setPosition(Vec2(this->_labelShouldRetain->getPosition().x + this->_labelShouldRetain->getContentSize().width/2,
        this->_labelShouldRetain->getPosition().y));
}

/// LabelTTFOpacityTest
LabelTTFOpacityTest::LabelTTFOpacityTest()
{
    auto s = Director::getInstance()->getWinSize();

    auto layer = LayerColor::create(Color4B(128, 128, 128, 255));
    addChild(layer, -10);

    auto label1 = Label::createWithTTF("Testing opacity", "fonts/Marker Felt.ttf", 48);
    addChild(label1);
    label1->setTextColor(Color4B::RED);
    label1->setPosition(Vec2(s.width / 2, s.height / 2));

    auto fadeOut = FadeOut::create(2);
    auto fadeIn = FadeIn::create(2);
    auto seq = Sequence::create(
        to_action_ptr(fadeOut),
        to_action_ptr(fadeIn)
    );
    auto forever = RepeatForever::create(seq);
    label1->runAction(forever);
}

std::string LabelTTFOpacityTest::title() const
{
    return "Label with TTF: Testing opacity";
}

std::string LabelTTFOpacityTest::subtitle() const
{
    return "RED label, fading In and Out in the center of the screen";
}

/// BMFontOneAtlas
BMFontOneAtlas::BMFontOneAtlas()
{
    auto s = Director::getInstance()->getWinSize();

    auto label1 = Label::createWithBMFont("fonts/helvetica-32.fnt", "This is Helvetica");
    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/3*2));

    auto label2 = Label::createWithBMFont("fonts/geneva-32.fnt", "And this is Geneva", TextHAlignment::LEFT, 0, Vec2(0, 128));
    addChild(label2);
    label2->setPosition(Vec2(s.width/2, s.height/3*1));
}

std::string BMFontOneAtlas::title() const
{
    return "Label with BMFont with one texture";
}

std::string BMFontOneAtlas::subtitle() const
{
    return "Using 2 .fnt definitions that share the same texture atlas.";
}

/// BMFontUnicode
BMFontUnicode::BMFontUnicode()
{
    auto strings = FileUtils::getInstance()->getValueMapFromFile("fonts/strings.xml");
    std::string chinese  = strings["chinese1"].asString();
    std::string russian  = strings["russian"].asString();
    std::string spanish  = strings["spanish"].asString();
    std::string japanese = strings["japanese"].asString();

    auto s = Director::getInstance()->getWinSize();

    auto label1 = Label::createWithBMFont("fonts/arial-unicode-26.fnt", spanish, TextHAlignment::LEFT, 200);
    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/10*6));

    auto label2 = Label::createWithBMFont("fonts/arial-unicode-26.fnt", chinese);
    addChild(label2);
    label2->setPosition(Vec2(s.width/2, s.height/10*5));

    auto label3 = Label::createWithBMFont("fonts/arial-26-en-ru.fnt", russian);
    addChild(label3);
    label3->setPosition(Vec2(s.width/2, s.height/10*4));

    auto label4 = Label::createWithBMFont("fonts/arial-unicode-26.fnt", japanese);
    addChild(label4);
    label4->setPosition(Vec2(s.width/2, s.height/10*3));
}

std::string BMFontUnicode::title() const
{
    return "Label with BMFont with Unicode support";
}

std::string BMFontUnicode::subtitle() const
{
    return "You should see 4 differnt labels:\nIn Spanish, Chinese, Russian and Korean";
}

// BMFontInit

BMFontInit::BMFontInit()
{
    auto s = Director::getInstance()->getWinSize();

    auto bmFont = Label::create();

    bmFont->setBMFontFilePath("fonts/helvetica-32.fnt");
    bmFont->setString("It is working!");
    this->addChild(bmFont);
    bmFont->setPosition(Vec2(s.width/2,s.height/4*2));
}

std::string BMFontInit::title() const
{
    return "Label create(), then BMFont";
}

std::string BMFontInit::subtitle() const
{
    return "Testing Label::create() wihtout params";
}

// TTFFontInit

TTFFontInit::TTFFontInit()
{
    auto s = Director::getInstance()->getWinSize();

    auto font = Label::create();

    font->setTTFConfig( TTFConfig("fonts/Marker Felt.ttf", 48) );

    font->setString("It is working!");
    this->addChild(font);
    font->setPosition( Vec2(s.width / 2, s.height / 2) );
}

std::string TTFFontInit::title() const
{
    return "Label config(), then TTF font";
}

std::string TTFFontInit::subtitle() const
{
    return "Testing Label::create() wihtout params";
}

// Issue1343

Issue1343::Issue1343()
{
    auto s = Director::getInstance()->getWinSize();

    auto bmFont = Label::create();

    bmFont->setBMFontFilePath("fonts/font-issue1343.fnt");
    bmFont->setString("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz.,'");

    this->addChild(bmFont);

    bmFont->setScale(0.3f);
    bmFont->setPosition(Vec2(s.width/2,s.height/4*2));
}

std::string Issue1343::title() const
{
    return "Issue 1343";
}

std::string Issue1343::subtitle() const
{
    return "You should see: ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890abcdefghijklmnopqrstuvwxyz.,'";
}

LabelBMFontBounds::LabelBMFontBounds()
{
    auto s = Director::getInstance()->getWinSize();
    
    auto layer = LayerColor::create(Color4B(128,128,128,255));
    addChild(layer, -10);
    
    // Label with BMFont
    auto label1 = Label::createWithBMFont("fonts/boundsTestFont.fnt", "Testing Glyph Designer");
    
    addChild(label1);
    label1->setPosition(Vec2(s.width/2, s.height/2));
    
    auto drawNode = DrawNode::create();
    
    auto labelSize = label1->getContentSize();
    auto origin = Director::getInstance()->getWinSize();
    
    origin.width = origin.width / 2 - (labelSize.width / 2);
    origin.height = origin.height / 2 - (labelSize.height / 2);
    
    Vec2 vertices[4]=
    {
        Vec2(origin.width, origin.height),
        Vec2(labelSize.width + origin.width, origin.height),
        Vec2(labelSize.width + origin.width, labelSize.height + origin.height),
        Vec2(origin.width, labelSize.height + origin.height)
    };
    drawNode->drawPoly(vertices, 4, true, Color4F(1.0, 1.0, 1.0, 1.0));
    addChild(drawNode);
}

std::string LabelBMFontBounds::title() const
{
    return "Testing Label with BMFont Bounds";
}

std::string LabelBMFontBounds::subtitle() const
{
    return "You should see string enclosed by a box";
}

// LabelBMFontCrashTest
void LabelBMFontCrashTest::onEnter()
{
    AtlasDemo::onEnter();
    
    auto winSize = Director::getInstance()->getWinSize();
    //Create a label and add it
    auto label1 = Label::create();
    label1->setBMFontFilePath("fonts/bitmapFontTest2.fnt");
    label1->setString("test");
    this->addChild(label1);
    // Visit will call draw where the function "ccGLBindVAO(m_uVAOname);" will be invoked.
    dynamic_cast<Node*>(label1)->visit();
    
    // Remove this label
    label1->removeFromParentAndCleanup(true);
    
    // Create a new label and add it (then crashes)
    auto label2 = Label::createWithBMFont("fonts/bitmapFontTest.fnt",
                                          "test 2");
    label2->setPosition(Vec2(winSize.width/2, winSize.height/2));
    this->addChild(label2);
}

std::string LabelBMFontCrashTest::title() const
{
    return "Label with BMFont Crash Test";
}

std::string LabelBMFontCrashTest::subtitle() const
{
    return "Should not crash.";
}

// LabelBMFontBinaryFormat
LabelBMFontBinaryFormat::LabelBMFontBinaryFormat()
{
    auto s = Director::getInstance()->getWinSize();

    auto bmFont = Label::create();

    bmFont->setBMFontFilePath("fonts/Roboto.bmf.fnt");
    bmFont->setString("It is working!");
    this->addChild(bmFont);
    bmFont->setPosition(Vec2(s.width/2,s.height/4*2));
}

std::string LabelBMFontBinaryFormat::title() const
{
    return "Label with BMFont Binary FNT File";
}

std::string LabelBMFontBinaryFormat::subtitle() const
{
    return "This label uses font file in AngelCode binary format";
}
