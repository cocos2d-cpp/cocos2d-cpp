//
// Bug-458 test case by nedrafehi
// http://code.google.com/p/cocos2d-iphone/issues/detail?id=458
//

#include "Bug-458.h"
#include "QuestionContainerSprite.h"

#include "2d/CCMenu.h"
#include "2d/CCMenuItem.h"
#include "base/CCDirector.h"

using namespace cocos2d;

bool Bug458Layer::init()
{
    if(BugsTestBase::init())
    {
        // ask director the the window size
        auto size = Director::getInstance()->getWinSize();

        auto question = new (std::nothrow) QuestionContainerSprite();
        auto question2 = new (std::nothrow) QuestionContainerSprite();
        
        auto sprite = MenuItemSprite::create(question2, question, CC_CALLBACK_1(Bug458Layer::selectAnswer, this) );
        auto layer = LayerColor::create(Color4B(0,0,255,255), 100, 100);
        question->release();
        question2->release();

        auto layer2 = LayerColor::create(Color4B(255,0,0,255), 100, 100);
        auto sprite2 = MenuItemSprite::create(layer, layer2, CC_CALLBACK_1(Bug458Layer::selectAnswer, this) );
        auto menu = Menu::create(sprite, sprite2, nullptr);
        menu->alignItemsVerticallyWithPadding(100);
        menu->setPosition(size.width / 2, size.height / 2);

        // add the label as a child to this Layer
        addChild(menu);

        return true;
    }
    return false;
}

void Bug458Layer::selectAnswer(Ref*)
{
    log("Selected");
}
