#ifndef _ATLAS_TEST_NEW_H_
#define _ATLAS_TEST_NEW_H_

#include "../BaseTest.h"

#include "base/ccTypes.h"
#include "GUI/CCControlExtension/CCControl.h"
#include "ui/UIRadioButton.h"
#include "ui/UISlider.h"

namespace cocos2d {

class Event;
class Label;
class MenuItemFont;
class Ref;
class Size;
class Sprite;
class Touch;

namespace extension {

class ControlStepper;

}
}

DEFINE_TEST_SUITE(NewLabelTests);

class AtlasDemoNew : public TestCase
{
};

class LabelFNTColorAndOpacity : public AtlasDemoNew
{
    float        _time;
public:
    static LabelFNTColorAndOpacity* create()
    {
        auto ret = new LabelFNTColorAndOpacity;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTColorAndOpacity();

    virtual void step(float dt);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTSpriteActions : public AtlasDemoNew
{
    float        _time;
public:
    static LabelFNTSpriteActions* create()
    {
        auto ret = new LabelFNTSpriteActions;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTSpriteActions();
    virtual void step(float dt);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTPadding : public AtlasDemoNew
{
public:
    static LabelFNTPadding* create()
    {
        auto ret = new LabelFNTPadding;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTPadding();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};


class LabelFNTOffset : public AtlasDemoNew
{
public:
    static LabelFNTOffset* create()
    {
        auto ret = new LabelFNTOffset;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTOffset();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTColor : public AtlasDemoNew
{
public:
    static LabelFNTColor* create()
    {
        auto ret = new LabelFNTColor;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTColor();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class LabelFNTOpacity : public AtlasDemoNew
{
public:
    static LabelFNTOpacity* create()
    {
        auto ret = new LabelFNTOpacity;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTOpacity();
    virtual std::string title() const override;

    virtual std::string subtitle() const override;
};

class LabelFNTHundredLabels : public AtlasDemoNew
{
public:
    static LabelFNTHundredLabels* create()
    {
        auto ret = new LabelFNTHundredLabels;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTHundredLabels();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTMultiLine : public AtlasDemoNew
{
public:
    static LabelFNTMultiLine* create()
    {
        auto ret = new LabelFNTMultiLine;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTMultiLine();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTandTTFEmpty : public AtlasDemoNew
{
public:
    static LabelFNTandTTFEmpty* create()
    {
        auto ret = new LabelFNTandTTFEmpty;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTandTTFEmpty();
    void updateStrings(float dt);
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
    bool setEmpty;
};

class LabelFNTRetina : public AtlasDemoNew
{
public:
    static LabelFNTRetina* create()
    {
        auto ret = new LabelFNTRetina;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTRetina();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTGlyphDesigner : public AtlasDemoNew
{
public:
    static LabelFNTGlyphDesigner* create()
    {
        auto ret = new LabelFNTGlyphDesigner;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTGlyphDesigner();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTMultiLineAlignment : public AtlasDemoNew
{
public:
    static LabelFNTMultiLineAlignment* create()
    {
        auto ret = new LabelFNTMultiLineAlignment;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTMultiLineAlignment();
    ~LabelFNTMultiLineAlignment();
    void snapArrowsToEdge();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    void stringChanged(cocos2d::Ref* sender);
    void alignmentChanged(cocos2d::Ref* sender);
    void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event* event);

public:
    cocos2d::Label* _labelShouldRetain;
    cocos2d::Sprite* _arrowsBarShouldRetain;
    cocos2d::Sprite* _arrowsShouldRetain;
    cocos2d::MenuItemFont *_lastSentenceItem, *_lastAlignmentItem;
    bool _drag;
};

class LabelFNTUNICODELanguages : public AtlasDemoNew
{
public:
    static LabelFNTUNICODELanguages* create()
    {
        auto ret = new LabelFNTUNICODELanguages;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTUNICODELanguages();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFNTBounds : public AtlasDemoNew
{
public:
    static LabelFNTBounds* create()
    {
        auto ret = new LabelFNTBounds;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFNTBounds();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFLongLineWrapping : public AtlasDemoNew
{
public:
    static LabelTTFLongLineWrapping* create()
    {
        auto ret = new LabelTTFLongLineWrapping;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFLongLineWrapping();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFColor : public AtlasDemoNew
{
public:
    static LabelTTFColor* create()
    {
        auto ret = new LabelTTFColor;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFColor();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFUnicodeNew : public AtlasDemoNew
{
public:
    static LabelTTFUnicodeNew* create()
    {
        auto ret = new LabelTTFUnicodeNew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFUnicodeNew();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFDynamicAlignment : public AtlasDemoNew
{
public:
    static LabelTTFDynamicAlignment* create()
    {
        auto ret = new LabelTTFDynamicAlignment;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFDynamicAlignment();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
    
private:
    
    void setAlignmentLeft(cocos2d::Ref* sender);
    void setAlignmentCenter(cocos2d::Ref* sender);
    void setAlignmentRight(cocos2d::Ref* sender);
    void updateAlignment();
    
    cocos2d::Label* _label;
    cocos2d::TextHAlignment   _horizAlign;
    
};


class LabelTTFCJKWrappingTest : public AtlasDemoNew
{
public:
    static LabelTTFCJKWrappingTest* create()
    {
        auto ret = new LabelTTFCJKWrappingTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFCJKWrappingTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
};

class LabelTTFFontsTestNew : public AtlasDemoNew
{
public:
    static LabelTTFFontsTestNew* create()
    {
        auto ret = new LabelTTFFontsTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFFontsTestNew();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFontDefTestNew : public AtlasDemoNew
{
public:
    static LabelFontDefTestNew* create()
    {
        auto ret = new LabelFontDefTestNew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFontDefTestNew();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

private:
};

class LabelTTFDistanceField : public AtlasDemoNew
{
public:
    static LabelTTFDistanceField* create()
    {
        auto ret = new LabelTTFDistanceField;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFDistanceField();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelOutlineAndGlowTest : public AtlasDemoNew
{
public:
    static LabelOutlineAndGlowTest* create()
    {
        auto ret = new LabelOutlineAndGlowTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelOutlineAndGlowTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelShadowTest : public AtlasDemoNew
{
public:
    static LabelShadowTest* create()
    {
        auto ret = new LabelShadowTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelShadowTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
private:
    cocos2d::Label* shadowLabelOutline;
    cocos2d::Label* shadowLabelTTF;
    cocos2d::Label* shadowLabelBMFont;
};

class LabelCharMapTest : public AtlasDemoNew
{
public:
    static LabelCharMapTest* create()
    {
        auto ret = new LabelCharMapTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelCharMapTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void step(float dt);

private:
    float _time; 
};

class LabelCharMapColorTest : public AtlasDemoNew
{
public:
    static LabelCharMapColorTest* create()
    {
        auto ret = new LabelCharMapColorTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelCharMapColorTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void step(float dt);
    void actionFinishCallback();

private:
    float _time; 
};

class LabelCrashTest : public AtlasDemoNew
{
public:
    static LabelCrashTest* create()
    {
        auto ret = new LabelCrashTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelCrashTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelTTFOldNew : public AtlasDemoNew
{
public:
    static LabelTTFOldNew* create()
    {
        auto ret = new LabelTTFOldNew;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelTTFOldNew();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelFontNameTest : public AtlasDemoNew
{
public:
    static LabelFontNameTest* create()
    {
        auto ret = new LabelFontNameTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelFontNameTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelAlignmentTest : public AtlasDemoNew
{
public:
    static LabelAlignmentTest* create()
    {
        auto ret = new LabelAlignmentTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAlignmentTest();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
private:
    void  setAlignmentLeft(cocos2d::Ref* sender);
    void  setAlignmentCenter(cocos2d::Ref* sender);
    void  setAlignmentRight(cocos2d::Ref* sender);
    void  setAlignmentTop(cocos2d::Ref* sender);
    void  setAlignmentMiddle(cocos2d::Ref* sender);
    void  setAlignmentBottom(cocos2d::Ref* sender);

    cocos2d::Label* _label;
};

class LabelIssue4428Test : public AtlasDemoNew
{
public:
    static LabelIssue4428Test* create()
    {
        auto ret = new LabelIssue4428Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue4428Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue4999Test : public AtlasDemoNew
{
public:
    static LabelIssue4999Test* create()
    {
        auto ret = new LabelIssue4999Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue4999Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelLineHeightTest : public AtlasDemoNew
{
public:
    static LabelLineHeightTest* create()
    {
        auto ret = new LabelLineHeightTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelLineHeightTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
private:
    cocos2d::Label* label;
};

class LabelAdditionalKerningTest : public AtlasDemoNew
{
public:
    static LabelAdditionalKerningTest* create()
    {
        auto ret = new LabelAdditionalKerningTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAdditionalKerningTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type);
private:
    cocos2d::Label* label;
};

class LabelIssue8492Test : public AtlasDemoNew
{
public:
    static LabelIssue8492Test* create()
    {
        auto ret = new LabelIssue8492Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue8492Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelMultilineWithOutline : public AtlasDemoNew
{
public:
    static LabelMultilineWithOutline* create()
    {
        auto ret = new LabelMultilineWithOutline;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelMultilineWithOutline();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

// we don't support linebreak mode

class LabelIssue9255Test : public AtlasDemoNew
{
public:
    static LabelIssue9255Test* create()
    {
        auto ret = new LabelIssue9255Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue9255Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelSmallDimensionsTest : public AtlasDemoNew
{
public:
    static LabelSmallDimensionsTest* create()
    {
        auto ret = new LabelSmallDimensionsTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelSmallDimensionsTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue10089Test : public AtlasDemoNew
{
public:
    static LabelIssue10089Test* create()
    {
        auto ret = new LabelIssue10089Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue10089Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelSystemFontColor : public AtlasDemoNew
{
public:
    static LabelSystemFontColor* create()
    {
        auto ret = new LabelSystemFontColor;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelSystemFontColor();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue10773Test : public AtlasDemoNew
{
public:
    static LabelIssue10773Test* create()
    {
        auto ret = new LabelIssue10773Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue10773Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue11576Test : public AtlasDemoNew
{
public:
    static LabelIssue11576Test* create()
    {
        auto ret = new LabelIssue11576Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue11576Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue11699Test : public AtlasDemoNew
{
public:
    static LabelIssue11699Test* create()
    {
        auto ret = new LabelIssue11699Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue11699Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue12409Test : public AtlasDemoNew
{
public:
    static LabelIssue12409Test* create()
    {
        auto ret = new LabelIssue12409Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue12409Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelAddChildTest : public AtlasDemoNew
{
public:
    static LabelAddChildTest* create()
    {
        auto ret = new LabelAddChildTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelAddChildTest();

    virtual std::string title() const override;
};

class LabelIssue12775Test : public AtlasDemoNew
{
public:
    static LabelIssue12775Test* create()
    {
        auto ret = new LabelIssue12775Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue12775Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue11585Test : public AtlasDemoNew
{
public:
    static LabelIssue11585Test* create()
    {
        auto ret = new LabelIssue11585Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue11585Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue10688Test : public AtlasDemoNew
{
public:
    static LabelIssue10688Test* create()
    {
        auto ret = new LabelIssue10688Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue10688Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue13202Test : public AtlasDemoNew
{
public:
    static LabelIssue13202Test* create()
    {
        auto ret = new LabelIssue13202Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue13202Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue9500Test : public AtlasDemoNew
{
public:
    static LabelIssue9500Test* create()
    {
        auto ret = new LabelIssue9500Test;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue9500Test();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelLayoutBaseTest : public AtlasDemoNew
{
public:
    LabelLayoutBaseTest();
    void updateDrawNodeSize(const cocos2d::Size &drawNodeSize);
    cocos2d::extension::ControlStepper* makeControlStepper();
    void valueChanged(cocos2d::Ref* sender, cocos2d::extension::Control::EventType controlEvent);

protected:
    void  setAlignmentLeft(cocos2d::Ref* sender);
    void  setAlignmentCenter(cocos2d::Ref* sender);
    void  setAlignmentRight(cocos2d::Ref* sender);
    void  setAlignmentTop(cocos2d::Ref* sender);
    void  setAlignmentMiddle(cocos2d::Ref* sender);
    void  setAlignmentBottom(cocos2d::Ref* sender);
    
    void initWrapOption(const cocos2d::Size& size);
    void initToggleLabelTypeOption(const cocos2d::Size& size);
    void initAlignmentOption(const cocos2d::Size& size);
    void initFontSizeChange(const cocos2d::Size& size);
    void initSliders(const cocos2d::Size& size);
    void initTestLabel(const cocos2d::Size& size);
    void initDrawNode(const cocos2d::Size& size);
    cocos2d::DrawNode* _drawNode;
    cocos2d::Label* _label;
    int _labelType;
};

class LabelWrapByWordTest : public LabelLayoutBaseTest
{
public:
    static LabelWrapByWordTest* create()
    {
        auto ret = new LabelWrapByWordTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelWrapByWordTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelWrapByCharTest : public LabelLayoutBaseTest
{
public:
    static LabelWrapByCharTest* create()
    {
        auto ret = new LabelWrapByCharTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelWrapByCharTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelShrinkByWordTest : public LabelLayoutBaseTest
{
public:
    static LabelShrinkByWordTest* create()
    {
        auto ret = new LabelShrinkByWordTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelShrinkByWordTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelShrinkByCharTest : public LabelLayoutBaseTest
{
public:
    static LabelShrinkByCharTest* create()
    {
        auto ret = new LabelShrinkByCharTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelShrinkByCharTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelResizeTest : public LabelLayoutBaseTest
{
public:
    static LabelResizeTest* create()
    {
        auto ret = new LabelResizeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelResizeTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelToggleTypeTest : public LabelLayoutBaseTest
{
public:
    static LabelToggleTypeTest* create()
    {
        auto ret = new LabelToggleTypeTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelToggleTypeTest();
    void initToggleCheckboxes();
    void onChangedRadioButtonSelect(cocos2d::ui::RadioButton* radioButton,cocos2d::ui::RadioButton::EventType type);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelSystemFontTest : public LabelLayoutBaseTest
{
public:
    static LabelSystemFontTest* create()
    {
        auto ret = new LabelSystemFontTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelSystemFontTest();
    void initToggleCheckboxes();
    void onChangedRadioButtonSelect(cocos2d::ui::RadioButton* radioButton,cocos2d::ui::RadioButton::EventType type);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelCharMapFontTest : public LabelLayoutBaseTest
{
public:
    static LabelCharMapFontTest* create()
    {
        auto ret = new LabelCharMapFontTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelCharMapFontTest();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue13846Test : public AtlasDemoNew
{
public:
    static LabelIssue13846Test* create()
    {
        auto ret = new LabelIssue13846Test;
        ret->init();
        ret->autorelease();
        return ret;
    }
    
    LabelIssue13846Test();
    
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelRichText : public AtlasDemoNew
{
public:
    static LabelRichText* create()
    {
        auto ret = new LabelRichText;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelRichText();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelItalics : public AtlasDemoNew
{
public:
    static LabelItalics* create()
    {
        auto ret = new LabelItalics;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelItalics();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1a;
    cocos2d::Label* _label2a;
};

class LabelBold : public AtlasDemoNew
{
public:
    static LabelBold* create()
    {
        auto ret = new LabelBold;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelBold();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1a;
    cocos2d::Label* _label2a;
};

class LabelUnderline : public AtlasDemoNew
{
public:
    static LabelUnderline* create()
    {
        auto ret = new LabelUnderline;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelUnderline();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1a;
    cocos2d::Label* _label2a;
};

class LabelUnderlineMultiline : public AtlasDemoNew
{
public:
    static LabelUnderlineMultiline* create()
    {
        auto ret = new LabelUnderlineMultiline;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelUnderlineMultiline();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1a;
    cocos2d::Label* _label2a;
};

class LabelStrikethrough : public AtlasDemoNew
{
public:
    static LabelStrikethrough* create()
    {
        auto ret = new LabelStrikethrough;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelStrikethrough();
    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1a;
    cocos2d::Label* _label2a;
};

class LabelLocalizationTest : public AtlasDemoNew
{
public:
    static LabelLocalizationTest* create()
    {
        auto ret = new LabelLocalizationTest;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelLocalizationTest();
    void onChangedRadioButtonSelect(cocos2d::ui::RadioButton* radioButton, cocos2d::ui::RadioButton::EventType type);

    virtual std::string title() const override;
    virtual std::string subtitle() const override;

    cocos2d::Label* _label1;
    cocos2d::Label* _label2;
};

class LabelIssue15214 : public AtlasDemoNew
{
public:
    static LabelIssue15214* create()
    {
        auto ret = new LabelIssue15214;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue15214();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue16293 : public AtlasDemoNew
{
public:
    static LabelIssue16293* create()
    {
        auto ret = new LabelIssue16293;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue16293();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

class LabelIssue16471 : public AtlasDemoNew
{
public:
    static LabelIssue16471* create()
    {
        auto ret = new LabelIssue16471;
        ret->init();
        ret->autorelease();
        return ret;
    }

    LabelIssue16471();

    virtual std::string title() const override;
    virtual std::string subtitle() const override;
};

#endif
