#include "TextInputTest.h"

using namespace cocos2d;

#define FONT_NAME                       "fonts/Thonburi.ttf"
#define FONT_SIZE                       36

TextInputTests::TextInputTests()
{
    ADD_TEST_CASE(TextFieldTTFDefaultTest);
    ADD_TEST_CASE(TextFieldTTFActionTest);
    ADD_TEST_CASE(TextFieldTTFSecureTextEntryTest);
}

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

std::string KeyboardNotificationLayer::title() const
{
    return "text input test";
}

//////////////////////////////////////////////////////////////////////////
// implement KeyboardNotificationLayer
//////////////////////////////////////////////////////////////////////////

KeyboardNotificationLayer::KeyboardNotificationLayer()
: _trackNode(0)
{
    // Register Touch Event
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(KeyboardNotificationLayer::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void KeyboardNotificationLayer::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    CCLOG("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
        info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

    if (! _trackNode)
    {
        return;
    }

    auto rectTracked = getRect(_trackNode);
    CCLOG("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
        rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! rectTracked.intersectsRect(info.end))
    {
        return;
    }

    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    CCLOG("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

    // move all the children node of KeyboardNotificationLayer
    auto& children = getChildren();
    Node * node = 0;
    ssize_t count = children.size();
    Vec2 pos;
    for (int i = 0; i < count; ++i)
    {
        node = children.at(i).get();
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}

// Layer function

bool KeyboardNotificationLayer::onTouchBegan(Touch *touch, Event*)
{
    CCLOG("++++++++++++++++++++++++++++++++++++++++++++");
    _beginPos = touch->getLocation();    
    return true;
}

void KeyboardNotificationLayer::onTouchEnded(Touch *touch, Event*)
{
    if (! _trackNode)
    {
        return;
    }
    
    auto endPos = touch->getLocation();    

    float delta = 5.0f;
    if (std::abs(endPos.x - _beginPos.x) > delta
        || std::abs(endPos.y - _beginPos.y) > delta)
    {
        // not click
        _beginPos.x = _beginPos.y = -1;
        return;
    }

    // decide the trackNode is clicked.
    Rect rect;
    rect.size = _trackNode->getContentSize();
    auto clicked = isScreenPointInRect(endPos, Camera::getVisitingCamera(), _trackNode->getWorldToNodeTransform(), rect, nullptr);
    this->onClickTrackNode(clicked);
    CCLOG("----------------------------------");
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFDefaultTest
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFDefaultTest::subtitle() const
{
    return "TextFieldTTF with default behavior test";
}

void TextFieldTTFDefaultTest::onClickTrackNode(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFDefaultTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();

    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();

    auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
        FONT_NAME,
        FONT_SIZE);
    addChild(pTextField);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
    // on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position to make it visable
    pTextField->setPosition(Vec2(s.width / 2, s.height/2 + 50));
#else
    pTextField->setPosition(Vec2(s.width / 2, s.height / 2));
#endif
    
    _trackNode = pTextField;
}

//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFActionTest
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFActionTest::subtitle() const
{
    return "CCTextFieldTTF with action and char limit test";
}

void TextFieldTTFActionTest::onClickTrackNode(bool bClicked)
{
    auto pTextField = (TextFieldTTF*)_trackNode;
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        CCLOG("TextFieldTTFActionTest:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
    }
    else
    {
        // TextFieldTTFTest not be clicked
        CCLOG("TextFieldTTFActionTest:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void TextFieldTTFActionTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();

    _charLimit = 12;

    _textFieldAction = RepeatForever::create(
        Sequence::create(
            to_action_ptr(FadeOut::create(0.25)),
            to_action_ptr(FadeIn::create(0.25))
        ));
    _textFieldAction->retain();
    _action = false;

    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();

    _textField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
        FONT_NAME,
        FONT_SIZE);
    addChild(_textField);

    _textField->setDelegate(this);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)    
    // on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position
    _textField->setPosition(Vec2(s.width / 2, s.height/2 + 50));
#else
    _textField->setPosition(Vec2(s.width / 2, s.height / 2));
#endif

    _trackNode = _textField;
}

void TextFieldTTFActionTest::onExit()
{
    KeyboardNotificationLayer::onExit();
    _textFieldAction->release();
}

// TextFieldDelegate protocol
bool TextFieldTTFActionTest::onTextFieldAttachWithIME(TextFieldTTF *)
{
    if (! _action)
    {
        _textField->runAction(_textFieldAction);
        _action = true;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDetachWithIME(TextFieldTTF *)
{
    if (_action)
    {
        _textFieldAction->stop();
        _textField->setOpacity(255);
        _action = false;
    }
    return false;
}

bool TextFieldTTFActionTest::onTextFieldInsertText(TextFieldTTF *sender, const char * text, size_t /*nLen*/)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    
    // if the textfield's char count more than _charLimit, doesn't insert text anymore.
    if (sender->getCharCount() >= _charLimit)
    {
        return true;
    }

    // create a insert text sprite and do some action
    auto label = Label::createWithSystemFont(text, FONT_NAME, FONT_SIZE);
    this->addChild(label);
    Color3B color(226, 121, 7);
    label->setColor(color);

    // move the sprite from top to position
    auto endPos = sender->getPosition();
    if (sender->getCharCount())
    {
        endPos.x += sender->getContentSize().width / 2;
    }
    auto inputTextSize = label->getContentSize();
    Vec2 beginPos(endPos.x, Director::getInstance()->getWinSize().height - inputTextSize.height * 2); 

    float duration = 0.5;
    label->setPosition(beginPos);
    label->setScale(8);

    auto seq = Sequence::create(
        to_action_ptr(
            Spawn::create(
                MoveTo::create(duration, endPos),
                ScaleTo::create(duration, 1),
                FadeOut::create(duration),
                nullptr
            )
        ),
        to_action_ptr(
            CallFuncN::create(CC_CALLBACK_1(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction, this))
        )
    );
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onTextFieldDeleteBackward(TextFieldTTF * sender, const char * delText, size_t /*nLen*/)
{
    // create a delete text sprite and do some action
    auto label = Label::createWithSystemFont(delText, FONT_NAME, FONT_SIZE);
    this->addChild(label);

    // move the sprite to fly out
    auto beginPos = sender->getPosition();
    auto textfieldSize = sender->getContentSize();
    auto labelSize = label->getContentSize();
    beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
    
    auto winSize = Director::getInstance()->getWinSize();
    Vec2 endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));

    float duration = 1;
    float rotateDuration = 0.2f;
    int repeatTime = 5; 
    label->setPosition(beginPos);
    
    auto moveto = MoveTo::create(duration, endPos);
    auto repeat = Repeat::create(RotateBy::create(rotateDuration, (rand() % 2) ? 360 : -360),
                                 repeatTime);
    auto fadeout = FadeOut::create(duration);
    
    auto spawn = Spawn::create(moveto, repeat, fadeout, std::nullptr_t());

    auto seq = Sequence::create(
        to_action_ptr(spawn),
        to_action_ptr(
            CallFuncN::create(CC_CALLBACK_1(TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction, this))
        )
    );
    label->runAction(seq);
    return false;
}

bool TextFieldTTFActionTest::onDraw(TextFieldTTF *)
{
    return false;
}

void TextFieldTTFActionTest::callbackRemoveNodeWhenDidAction(Node * node)
{
    this->removeChild(node, true);
}


//////////////////////////////////////////////////////////////////////////
// implement TextFieldTTFSecureTextEntryTest
//////////////////////////////////////////////////////////////////////////

std::string TextFieldTTFSecureTextEntryTest::subtitle() const
{
    return "TextFieldTTF with SecureTextEntry test";
}

void TextFieldTTFSecureTextEntryTest::onEnter()
{
    KeyboardNotificationLayer::onEnter();
    
    // add TextFieldTTF
    auto s = Director::getInstance()->getWinSize();
    
    auto pTextField = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>",
                                                             FONT_NAME,
                                                             FONT_SIZE);
    addChild(pTextField);
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    // on android, TextFieldTTF cannot auto adjust its position when soft-keyboard pop up
    // so we had to set a higher position to make it visable
    pTextField->setPosition(Vec2(s.width / 2, s.height/2 + 50));
#else
    pTextField->setPosition(Vec2(s.width / 2, s.height / 2));
#endif
    pTextField->setSecureTextEntry(true);
    
    _trackNode = pTextField;
}
